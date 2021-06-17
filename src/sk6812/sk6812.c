/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * sk6812.c - SK6812 RGBW driver
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * Timings:
 *          SK6812
 *   T0H    300 ns  +/- 150 ns
 *   T1H    600 ns  +/- 150 ns
 *   T0L    900 ns  +/- 150 ns
 *   T1L    600 ns  +/- 150 ns
 *   TRESET  80 us
 *
 * SK6812 RGBW format : (8G 8R 8B 8W)
 *   32bit per LED  (32 * 1.2us = 36us per LED)
 *    8bit per color (MSB first)
 *
 * SK6812 RGB format : (8G 8R 8B)
 *   24bit per LED  (32 * 1.2us = 28.8us per LED)
 *    8bit per color (MSB first)
 *
 * After each frame of n LEDs there has to be a pause of >= 80us (TRESET)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

#include <string.h>
#include "sk6812-config.h"
#include "sk6812.h"
#include "delay.h"
#include "io.h"

#include "log.h"

#if DSP_USE_SK6812_RGBW == 1 || DSP_USE_SK6812_RGB == 1

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * timer calculation:
 *
 *  freq = SK6812_TIM_CLK / (SK6812_TIM_PRESCALER + 1) / (SK6812_TIM_PERIOD + 1)
 *
 *  STM32F4XX:
 *    freq = 84000000 / (0 + 1) / (100 + 1) = 832 kHz = 1.202 us
 *
 *  STM32F10X:
 *    freq = 72000000 / (0 + 1) / ( 85 + 1) = 837 kHz = 1.194 us
 *
 *  (SK6812_TIM_PERIOD + 1) = (SK6812_TIM_CLK / (1 + SK6812_TIM_PRESCALER)) / freq
 *  SK6812_TIM_PERIOD       = (SK6812_TIM_CLK / (1 + SK6812_TIM_PRESCALER)) * 1.2us - 1
 *  SK6812_TIM_PERIOD       = ((SK6812_TIM_CLK / (1 + SK6812_TIM_PRESCALER)) * 1.2) / 1000000 - 1
 *  SK6812_TIM_PERIOD       = ((SK6812_TIM_CLK / (1 + SK6812_TIM_PRESCALER)) * 12)  / 10000000 - 1
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (STM32F401RE)                                       // STM32F401 Nucleo Board
#define SK6812_TIM_CLK               84L                        // 84 MHz = 11.90ns
#elif defined (STM32F411RE)                                     // STM32F411 Nucleo Board
#define SK6812_TIM_CLK              100L                        // 100 MHz = 10.00ns
#elif defined (STM32F446RE)                                     // STM32F446 Nucleo Board
#define SK6812_TIM_CLK              180L                        // 180 MHz = 5.55ns
#elif defined (STM32F103)                                       // STM32F103 Mini Development Board
#define SK6812_TIM_CLK               72L                        // 72 MHz = 13.89ns
#else
#error STM32 unknown
#endif

#if DSP_USE_SK6812_RGBW == 1
#define SK6812_BIT_PER_LED          32                          // 4 * 8bit per LED, each bit costs 1.20us time
#elif DSP_USE_SK6812_RGB == 1
#define SK6812_BIT_PER_LED          24                          // 3 * 8bit per LED, each bit costs 1.20us time
#endif

#if 0

#define SK6812_TIM_PERIOD_TIME      1200                        // 1200 ns
#define SK6812_TIM_PRESCALER        0                           // no prescaler
#define SK6812_T0H_TIME             300                         // 300ns
#define SK6812_T0L_TIME             900                         // 900ns
#define SK6812_T1H_TIME             600                         // 600ns
#define SK6812_T1L_TIME             600                         // 600ns
#define SK6812_PAUSE_TIME           80000                       // pause, should be longer than 80us

#else

#define SK6812_TIM_PERIOD_TIME      1400                        // 1400 ns
#define SK6812_TIM_PRESCALER        0                           // no prescaler
#define SK6812_T0H_TIME             400                         // 400ns
#define SK6812_T0L_TIME             1000                        // 1000ns
#define SK6812_T1H_TIME             700                         // 700ns
#define SK6812_T1L_TIME             700                         // 700ns
#define SK6812_PAUSE_TIME           100000                      // pause, should be longer than 80us

#endif

#define SK6812_TIM_PERIOD_FLOAT     (((SK6812_TIM_CLK / (1 + SK6812_TIM_PRESCALER)) * SK6812_TIM_PERIOD_TIME) / 1000.0 - 1.0)   //  99,8 @84MHz
#define SK6812_TIM_PERIOD           (uint16_t) (SK6812_TIM_PERIOD_FLOAT + 0.5)                                                  // 100
#define SK6812_T0H                  (uint16_t) ((SK6812_TIM_PERIOD_FLOAT * SK6812_T0H_TIME) / SK6812_TIM_PERIOD_TIME + 0.5)     //  25
#define SK6812_T1H                  (uint16_t) ((SK6812_TIM_PERIOD_FLOAT * SK6812_T1H_TIME) / SK6812_TIM_PERIOD_TIME + 0.5)     //  50
#define SK6812_T0L                  (uint16_t) ((SK6812_TIM_PERIOD_FLOAT * SK6812_T0L_TIME) / SK6812_TIM_PERIOD_TIME + 0.5)     //  75
#define SK6812_T1L                  (uint16_t) ((SK6812_TIM_PERIOD_FLOAT * SK6812_T1L_TIME) / SK6812_TIM_PERIOD_TIME + 0.5)     //  50
#define SK6812_PAUSE_LEN            (uint16_t) (SK6812_PAUSE_TIME / SK6812_TIM_PERIOD_TIME + 1)                                 //  67

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * DMA buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static volatile uint_fast16_t       current_dma_buf_pos;
static volatile uint_fast16_t       current_led_offset;
static volatile uint_fast16_t       current_data_pause_len;
static volatile uint_fast16_t       current_leds;

#define DATA_LEN(n)                 ((n) * SK6812_BIT_PER_LED)                                  // number of total bytes to transfer data
#define PAUSE_LEN                   (SK6812_PAUSE_LEN)                                          // number of total bytes to transfer pause
#define DMA_BUF_LEN                 (2 * SK6812_BIT_PER_LED)                                    // DMA buffer length: 2 LEDs

static volatile uint32_t            sk6812_dma_status;                                          // DMA status
static SK6812_RGBW                  rgbw_buf[2][SK6812_MAX_LEDS];                               // RGBW values (double buffered)
static volatile uint_fast8_t        current_rgbw_buf_idx;                                       // current rgbw buffer index
static uint_fast8_t                 next_rgbw_buf_idx;                                          // next rgbw buffer index

#if defined (STM32F4XX)                                                                         // STM32F4xx
typedef uint16_t                    DMA_BUFFER_TYPE;                                            // 16bit DMA buffer, must be aligned to 16 bit
#else                                                                                           // STM32F10x
typedef uint8_t                     DMA_BUFFER_TYPE;                                            // 8bit DMA buffer saves RAM
#endif
static volatile DMA_BUFFER_TYPE     dma_buf[DMA_BUF_LEN];                                       // DMA buffer

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * Timer for data: TIM3 for STM32F4xx, TIM1 for STM32F10X
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (STM32F4XX)
// Timer:
#  define SK6812_TIM_CLOCK_CMD          RCC_APB1PeriphClockCmd
#  define SK6812_TIM_CLOCK              RCC_APB1Periph_TIM3
#  define SK6812_TIM                    TIM3
#  define SK6812_TIM_AF                 GPIO_AF_TIM3
#  define SK6812_TIM_CCR_REG1           TIM3->CCR1
#  define SK6812_TIM_DMA_TRG1           TIM_DMA_CC1
// GPIO:
#  define SK6812_GPIO_CLOCK_CMD         RCC_AHB1PeriphClockCmd
#  define SK6812_GPIO_CLOCK             RCC_AHB1Periph_GPIOC
#  define SK6812_GPIO_PORT              GPIOC
#  define SK6812_GPIO_PIN               GPIO_Pin_6
#  define SK6812_GPIO_SOURCE            GPIO_PinSource6
// DMA TIM3 - DMA1, Channel5, Stream4
#  define SK6812_DMA_CLOCK_CMD          RCC_AHB1PeriphClockCmd
#  define SK6812_DMA_CLOCK              RCC_AHB1Periph_DMA1
#  define SK6812_DMA_STREAM             DMA1_Stream4
#  define SK6812_DMA_CHANNEL            DMA_Channel_5
// transfer complete interrupt - DMA1, Stream4
#  define SK6812_DMA_CHANNEL_IRQn       DMA1_Stream4_IRQn
#  define SK6812_DMA_CHANNEL_ISR        DMA1_Stream4_IRQHandler
#  define SK6812_DMA_CHANNEL_IRQ_TC     DMA_IT_TCIF4                    // transfer complete interrupt
#  define SK6812_DMA_CHANNEL_IRQ_HT     DMA_IT_HTIF4                    // half-transfer interrupt

#elif defined (STM32F10X)
// Timer:
#  define SK6812_TIM_CLOCK_CMD          RCC_APB2PeriphClockCmd
#  define SK6812_TIM_CLOCK              RCC_APB2Periph_TIM1
#  define SK6812_TIM                    TIM1
#  define SK6812_TIM_AF                 GPIO_AF_TIM1
#  define SK6812_TIM_CCR_REG1           TIM1->CCR1
#  define SK6812_TIM_DMA_TRG1           TIM_DMA_CC1
// GPIO:
#  define SK6812_GPIO_CLOCK_CMD         RCC_APB2PeriphClockCmd
#  define SK6812_GPIO_CLOCK             RCC_APB2Periph_GPIOA
#  define SK6812_GPIO_PORT              GPIOA
#  define SK6812_GPIO_PIN               GPIO_Pin_8
#  define SK6812_GPIO_SOURCE            GPIO_PinSource8
// DMA TIM1 - DMA1, Channel2
#  define SK6812_DMA_CLOCK_CMD          RCC_AHBPeriphClockCmd
#  define SK6812_DMA_CLOCK              RCC_AHBPeriph_DMA1
#  define SK6812_DMA_STREAM             DMA1_Channel2
// transfer complete interrupt - DMA1, Channel2
#  define SK6812_DMA_CHANNEL_IRQn       DMA1_Channel2_IRQn
#  define SK6812_DMA_CHANNEL_ISR        DMA1_Channel2_IRQHandler
#  define SK6812_DMA_CHANNEL_IRQ_TC     DMA1_IT_TC2                    // transfer complete interrupt
#  define SK6812_DMA_CHANNEL_IRQ_HT     DMA1_IT_HT2                    // half-transfer interrupt

#endif

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * INTERN: initialize DMA
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
sk6812_dma_init (void)
{
    DMA_InitTypeDef         dma;
    DMA_StructInit (&dma);

    DMA_Cmd(SK6812_DMA_STREAM, DISABLE);
    DMA_DeInit(SK6812_DMA_STREAM);

    dma.DMA_Mode                = DMA_Mode_Circular;
    dma.DMA_PeripheralBaseAddr  = (uint32_t) &SK6812_TIM_CCR_REG1;
#if defined(STM32F4XX)                                                      // STM32F4xx
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;          // 16bit
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;              // 16bit
#else                                                                       // STM32F10x
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;          // 16bit
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;                  // 8bit, saves RAM
#endif
    dma.DMA_BufferSize          = DMA_BUF_LEN;
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    dma.DMA_Priority            = DMA_Priority_VeryHigh;                    // DMA_Priority_High;

#if defined(STM32F4XX)

    dma.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;
    dma.DMA_Channel             = SK6812_DMA_CHANNEL;
    dma.DMA_Memory0BaseAddr     = (uint32_t)dma_buf;
    dma.DMA_FIFOMode            = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull;
    dma.DMA_MemoryBurst         = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;

#elif defined (STM32F10X)

    dma.DMA_DIR                 = DMA_DIR_PeripheralDST;
    dma.DMA_M2M                 = DMA_M2M_Disable;
    dma.DMA_MemoryBaseAddr      = (uint32_t)dma_buf;

#endif

    DMA_Init(SK6812_DMA_STREAM, &dma);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * start DMA & timer (stopped when Transfer-Complete-Interrupt arrives)
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
sk6812_dma_start (void)
{
    sk6812_dma_status = 1;                                                          // set status to "busy"

    TIM_Cmd (SK6812_TIM, DISABLE);                                                  // disable timer
    DMA_Cmd (SK6812_DMA_STREAM, DISABLE);                                           // disable DMA
    DMA_SetCurrDataCounter(SK6812_DMA_STREAM, DMA_BUF_LEN);                   // set new buffer size
    DMA_ITConfig(SK6812_DMA_STREAM, DMA_IT_TC | DMA_IT_HT, ENABLE);                 // enable transfer complete interrupt
    DMA_Cmd (SK6812_DMA_STREAM, ENABLE);                                            // enable DMA
    TIM_Cmd(SK6812_TIM, ENABLE);                                                    // Timer enable
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * clear all LEDs
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
sk6812_clear_all (uint_fast16_t n_leds)
{
    SK6812_RGBW  rgbw =
    {
        0,
        0,
        0,
#if DSP_USE_SK6812_RGBW == 1
        0
#endif
    };

    while (sk6812_dma_status != 0)
    {
        ;
    }

    sk6812_set_all_leds (&rgbw, n_leds, 1);
}


/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * setup timer buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
sk6812_setup_dma_buf (uint_fast8_t at_half_pos)
{
    uint_fast8_t                i;
    uint_fast16_t               dma_buf_pos;
    SK6812_RGBW *               led;
    volatile DMA_BUFFER_TYPE *  timer_p;

    dma_buf_pos = current_dma_buf_pos;

    if (at_half_pos)
    {
        timer_p = dma_buf + DMA_BUF_LEN / 2;
    }
    else
    {
        timer_p = dma_buf;
    }

    if (current_led_offset < current_leds)
    {
        led = rgbw_buf[current_rgbw_buf_idx] + current_led_offset;

        for (i = 0x80; i != 0; i >>= 1)                         // color green
        {
            *timer_p++ = (led->green & i) ? SK6812_T1H : SK6812_T0H;
        }

        for (i = 0x80; i != 0; i >>= 1)                         // color red
        {
            *timer_p++ = (led->red & i) ? SK6812_T1H : SK6812_T0H;
        }

        for (i = 0x80; i != 0; i >>= 1)                         // color blue
        {
            *timer_p++ = (led->blue & i) ? SK6812_T1H : SK6812_T0H;
        }

#if DSP_USE_SK6812_RGBW == 1
        for (i = 0x80; i != 0; i >>= 1)                         // color white
        {
            *timer_p++ = (led->white & i) ? SK6812_T1H : SK6812_T0H;
        }
#endif

        dma_buf_pos += SK6812_BIT_PER_LED;
        current_led_offset++;
    }
    else
    {
        uint_fast16_t bytes_to_write = SK6812_BIT_PER_LED;

        while (bytes_to_write > 0 && dma_buf_pos < current_data_pause_len)                  // pause (min. 80us)
        {
            *timer_p++ = 0;
            dma_buf_pos++;
            bytes_to_write--;
        }

        while (bytes_to_write > 0)                                                          // fill rest of buffer with 0
        {
            *timer_p++ = 0;
            bytes_to_write--;
        }
    }

    current_dma_buf_pos = dma_buf_pos;
}


/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ISR DMA (will be called, when all data has been transferred)
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void SK6812_DMA_CHANNEL_ISR (void);
void
SK6812_DMA_CHANNEL_ISR (void)
{
#if defined (STM32F4XX)
    if (DMA_GetITStatus(SK6812_DMA_STREAM, SK6812_DMA_CHANNEL_IRQ_HT))              // check half-transfer interrupt flag
    {
        DMA_ClearITPendingBit (SK6812_DMA_STREAM, SK6812_DMA_CHANNEL_IRQ_HT);       // reset flag
        sk6812_setup_dma_buf (0);
    }
    if (DMA_GetITStatus(SK6812_DMA_STREAM, SK6812_DMA_CHANNEL_IRQ_TC))              // check transfer complete interrupt flag
    {
        DMA_ClearITPendingBit (SK6812_DMA_STREAM, SK6812_DMA_CHANNEL_IRQ_TC);       // reset flag

        if (current_dma_buf_pos < current_data_pause_len)
        {
            sk6812_setup_dma_buf (1);
        }
        else
        {
            DMA_Cmd (SK6812_DMA_STREAM, DISABLE);                                   // disable DMA
            sk6812_dma_status = 0;                                                  // set status to ready
        }
    }
#elif defined (STM32F10X)
    if (DMA_GetITStatus(SK6812_DMA_CHANNEL_IRQ_HT))                                 // check half-transfer interrupt flag
    {
        DMA_ClearITPendingBit (SK6812_DMA_CHANNEL_IRQ_HT);                          // reset flag
        sk6812_setup_dma_buf (0);
    }
    if (DMA_GetITStatus(SK6812_DMA_CHANNEL_IRQ_TC))                                 // check transfer complete interrupt flag
    {
        DMA_ClearITPendingBit (SK6812_DMA_CHANNEL_IRQ_TC);                          // reset flag

        if (current_dma_buf_pos < current_data_pause_len)
        {
            sk6812_setup_dma_buf (1);
        }
        else
        {
            DMA_Cmd (SK6812_DMA_STREAM, DISABLE);                                   // disable DMA
            sk6812_dma_status = 0;                                                  // set status to ready
        }
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * refresh buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
sk6812_refresh (uint_fast16_t n_leds)
{
    uint_fast16_t   i;

    while (sk6812_dma_status != 0)
    {
        ;                                                                           // wait until DMA transfer is ready
    }

    current_rgbw_buf_idx    = next_rgbw_buf_idx;
    next_rgbw_buf_idx       = next_rgbw_buf_idx ? 0 : 1;

    current_dma_buf_pos     = 0;
    current_led_offset      = 0;
    current_data_pause_len  = DATA_LEN(n_leds) + PAUSE_LEN;
    current_leds            = n_leds;

    sk6812_setup_dma_buf (0);
    sk6812_setup_dma_buf (1);
    sk6812_dma_start();

    for (i = 0; i < SK6812_MAX_LEDS; i++)                                           // copy current rgb buffer during DMA transfer
    {
        rgbw_buf[next_rgbw_buf_idx][i] = rgbw_buf[current_rgbw_buf_idx][i];
    }

}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set one RGBW value
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
sk6812_set_led (uint_fast16_t n, SK6812_RGBW * rgbw)
{
    if (n < SK6812_MAX_LEDS)
    {
        rgbw_buf[next_rgbw_buf_idx][n].red     = rgbw->red;
        rgbw_buf[next_rgbw_buf_idx][n].green   = rgbw->green;
        rgbw_buf[next_rgbw_buf_idx][n].blue    = rgbw->blue;
#if DSP_USE_SK6812_RGBW == 1
        rgbw_buf[next_rgbw_buf_idx][n].white   = rgbw->white;
#endif
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set all LEDs to RGBW value
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
sk6812_set_all_leds (SK6812_RGBW * rgbw, uint_fast16_t n_leds, uint_fast8_t refresh)
{
    uint_fast16_t n;

    for (n = 0; n < n_leds; n++)
    {
        rgbw_buf[next_rgbw_buf_idx][n].red      = rgbw->red;
        rgbw_buf[next_rgbw_buf_idx][n].green    = rgbw->green;
        rgbw_buf[next_rgbw_buf_idx][n].blue     = rgbw->blue;
#if DSP_USE_SK6812_RGBW == 1
        rgbw_buf[next_rgbw_buf_idx][n].white    = rgbw->white;
#endif
    }

    if (refresh)
    {
        sk6812_refresh (n_leds);
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * initialize SK6812
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
sk6812_init (void)
{
    GPIO_InitTypeDef        gpio;
    TIM_TimeBaseInitTypeDef tb;
    TIM_OCInitTypeDef       toc;
    NVIC_InitTypeDef        nvic;

    sk6812_dma_status = 0;

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize gpio
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    GPIO_StructInit (&gpio);
    SK6812_GPIO_CLOCK_CMD (SK6812_GPIO_CLOCK, ENABLE);          // clock enable

    gpio.GPIO_Pin     = SK6812_GPIO_PIN;

#if defined (STM32F4XX)

    // 1st path: set data pin to input with pulldown, then check if external pullup connected:
    gpio.GPIO_Mode    = GPIO_Mode_IN;                                           // set as input
    gpio.GPIO_PuPd    = GPIO_PuPd_DOWN;                                         // with internal pulldown
    gpio.GPIO_Speed   = GPIO_Speed_100MHz;
    GPIO_Init(SK6812_GPIO_PORT, &gpio);
    delay_msec (1);                                                             // wait a moment

    // 2nd path: if external pullup detected, use open-drain, else use push-pull
    if (GPIO_ReadInputDataBit(SK6812_GPIO_PORT, SK6812_GPIO_PIN) == Bit_SET)    // external 4k7 pullup connected?
    {
        log_message ("sk6812: external pullup detected");
        gpio.GPIO_OType   = GPIO_OType_OD;                                      // yes, set output type to open-drain
    }
    else
    {
        log_message ("sk6812: no external pullup detected");
        gpio.GPIO_OType   = GPIO_OType_PP;                                      // no, set output type to push-pull
    }

    gpio.GPIO_Mode    = GPIO_Mode_AF;                                           // set as alternate output
    gpio.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed   = GPIO_Speed_100MHz;

    GPIO_Init(SK6812_GPIO_PORT, &gpio);
    GPIO_RESET_BIT(SK6812_GPIO_PORT, SK6812_GPIO_PIN);                           // set pin to Low
    GPIO_PinAFConfig(SK6812_GPIO_PORT, SK6812_GPIO_SOURCE, SK6812_TIM_AF);

#elif defined (STM32F10X)

    gpio.GPIO_Mode  = GPIO_Mode_IPD;                                            // set as input with internal pulldown
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SK6812_GPIO_PORT, &gpio);
    delay_msec (1);                                                             // wait a moment

    if (GPIO_ReadInputDataBit(SK6812_GPIO_PORT, SK6812_GPIO_PIN) == Bit_SET)    // external 4k7 pullup connected?
    {
        log_message ("sk6812: external pullup detected");
        gpio.GPIO_Mode    = GPIO_Mode_AF_OD;                                    // yes, use open-drain
    }
    else
    {
        log_message ("sk6812: no external pullup detected");
        gpio.GPIO_Mode    = GPIO_Mode_AF_PP;                                    // no, use push-pull
    }

    gpio.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(SK6812_GPIO_PORT, &gpio);
    GPIO_RESET_BIT(SK6812_GPIO_PORT, SK6812_GPIO_PIN);                          // set pin to Low

#endif

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize TIMER
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    TIM_TimeBaseStructInit (&tb);
    TIM_OCStructInit (&toc);

    SK6812_TIM_CLOCK_CMD (SK6812_TIM_CLOCK, ENABLE);            // clock enable (TIM)
    SK6812_DMA_CLOCK_CMD (SK6812_DMA_CLOCK, ENABLE);            // clock Enable (DMA)

    tb.TIM_Period           = SK6812_TIM_PERIOD;
    tb.TIM_Prescaler        = SK6812_TIM_PRESCALER;
    tb.TIM_ClockDivision    = TIM_CKD_DIV1;
    tb.TIM_CounterMode      = TIM_CounterMode_Up;
    TIM_TimeBaseInit (SK6812_TIM, &tb);

    toc.TIM_OCMode          = TIM_OCMode_PWM1;
    toc.TIM_OutputState     = TIM_OutputState_Enable;
    toc.TIM_Pulse           = 0;
    toc.TIM_OCPolarity      = TIM_OCPolarity_High;

    TIM_OC1Init(SK6812_TIM, &toc);
    TIM_OC1PreloadConfig (SK6812_TIM, TIM_OCPreload_Enable);    // fm: necessary on STM32F1xx?
    TIM_ARRPreloadConfig (SK6812_TIM, ENABLE);                  // timer enable, fm: necessary on STM32F1xx?
    TIM_CtrlPWMOutputs(SK6812_TIM, ENABLE);
    TIM_DMACmd (SK6812_TIM, SK6812_TIM_DMA_TRG1, ENABLE);

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize NVIC
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    nvic.NVIC_IRQChannel                    = SK6812_DMA_CHANNEL_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  = 0;
    nvic.NVIC_IRQChannelSubPriority         = 0;
    nvic.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&nvic);

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize DMA
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    sk6812_dma_init ();
    sk6812_clear_all (SK6812_MAX_LEDS);
}

#endif // DSP_USE_SK6812_RGB || DSP_USE_SK6812_RGBW
