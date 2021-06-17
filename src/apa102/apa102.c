/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * apa102.c - APA102 driver
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * APA102 format : Start FRAME | LED1 | LED2 | LED3 | ... | LEDn | End Frame
 * Start frame:    32 x 0
 * LED data:       111 Brightness | 8 x Blue | 8 x Green | 8 x Red
 *                 5 bits Brightness, 00000 = 0, 11111 = 31 (MSB first)
 *                 8 bits per color (MSB first)
 * End frame:      N_LEDS / 2
 *
 * Clock:          Raising edge, Speed: typ. 10MHz
 *
 * See also:       https://cpldcpu.wordpress.com/2014/11/30/understanding-the-apa102-superled/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "apa102.h"
#include "apa102-config.h"
#include "io.h"

#if DSP_USE_APA102 == 1

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * SPI for data: DMA1, channel 5, SPI2
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (STM32F4XX)
// SPI device
#  define APA102_SPI_DEVICE             SPI2                                                // SPI device
#  define APA102_SPI_CLOCK_CMD          RCC_APB1PeriphClockCmd                              // APB1
#  define APA102_SPI_CLOCK              RCC_APB1Periph_SPI2                                 // APB1 SPI2

// GPIO SPI:
#  define APA102_GPIO_CLOCK_CMD         RCC_AHB1PeriphClockCmd                              // AHB1
#  define APA102_GPIO_CLOCK             RCC_AHB1Periph_GPIOB                                // AHB1 GPIOB
#  define APA102_GPIO_PORT              GPIOB                                               // GPIOB
#  define APA102_GPIO_SCK_PIN           GPIO_Pin_13                                         // SCK = PB13
#  define APA102_GPIO_SCK_SOURCE        GPIO_PinSource13
#  define APA102_GPIO_MOSI_PIN          GPIO_Pin_15                                         // MOSI = PB15
#  define APA102_GPIO_MOSI_SOURCE       GPIO_PinSource15
#  define APA102_SPI_AF                 GPIO_AF_SPI2                                        // GPIO SPI22 alternate function

// DMA: DMA1, Channel5, Stream4
#  define APA102_DMA_CLOCK_CMD          RCC_AHB1PeriphClockCmd                              // AHB1
#  define APA102_DMA_CLOCK              RCC_AHB1Periph_DMA1                                 // AHB1 DMA1
#  define APA102_DMA_STREAM             DMA1_Stream4                                        // DMA1 Stream5
#  define APA102_DMA_CHANNEL            DMA_Channel_5                                       // DMA1 Channel5
// transfer complete interrupt - DMA1, Stream4
#  define APA102_DMA_CHANNEL_IRQn       DMA1_Stream4_IRQn                                   // DMA1 Stream4 IRQn
#  define APA102_DMA_CHANNEL_ISR        DMA1_Stream4_IRQHandler                             // DMA1 Stream 4 IRQ Handler
#  define APA102_DMA_CHANNEL_IRQ_FLAG   DMA_IT_TCIF4                                        // transfer complete on Stream4

#elif defined (STM32F10X)

// SPI device
#  define APA102_SPI_DEVICE             SPI2                                                // SPI device
#  define APA102_SPI_CLOCK_CMD          RCC_APB1PeriphClockCmd                              // APB1
#  define APA102_SPI_CLOCK              RCC_APB1Periph_SPI2                                 // APB1 SPI2

// GPIO SPI:
#  define APA102_GPIO_CLOCK_CMD         RCC_APB2PeriphClockCmd                              // APB2
#  define APA102_GPIO_CLOCK             (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)        // GPIOB
#  define APA102_GPIO_PORT              GPIOB                                               // GPIOB
#  define APA102_GPIO_SCK_PIN           GPIO_Pin_13                                         // SCK = PB13
#  define APA102_GPIO_SCK_SOURCE        GPIO_PinSource13
#  define APA102_GPIO_MOSI_PIN          GPIO_Pin_15                                         // MOSI = PB15

// DMA: DMA1, Channel5
#  define APA102_DMA_CLOCK_CMD          RCC_AHBPeriphClockCmd                               // AHB
#  define APA102_DMA_CLOCK              RCC_AHBPeriph_DMA1                                  // DMA1
#  define APA102_DMA_STREAM             DMA1_Channel5                                       // DMA1 Channel5
// transfer complete interrupt - DMA1, Channel5
#  define APA102_DMA_CHANNEL_IRQn       DMA1_Channel5_IRQn                                  // DMA1 Channel5 IRQn
#  define APA102_DMA_CHANNEL_ISR        DMA1_Channel5_IRQHandler                            // IRQ Handler
#  define APA102_DMA_CHANNEL_IRQ_FLAG   DMA1_IT_TC5                                         // transfer complete on Channel5

#endif

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * DMA buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

#define  APA102_START_FRAME_BYTES       4                           // 32 bits for start frame
#define  APA102_LED_BYTES(n)            (4 * (n))                   // 1 * 8 bits for global, 3 * 8bit per LED
#define  APA102_END_FRAME_BYTES(n)      (((n) / 2 / 8) + 1)         // n / 2 bits for end frame (rounded up to byte)

#define APA102_BUF_LEN(n)               (APA102_START_FRAME_BYTES + APA102_LED_BYTES(n) + APA102_END_FRAME_BYTES(n))    // DMA buffer length

static volatile uint32_t                apa102_dma_status;                              // DMA status
static APA102_RGB                       rgb_buf[APA102_MAX_LEDS];                       // RGB values
#if defined (STM32F4XX)                                                                 // STM32F4xx
static uint16_t                         apa102_buf[APA102_BUF_LEN(APA102_MAX_LEDS)];    // 16bit DMA buffer, must be aligned to 16 bit
#else                                                                                   // STM32F10x
static uint8_t                          apa102_buf[APA102_BUF_LEN(APA102_MAX_LEDS)];    // 8bit DMA buffer saves RAM
#endif


/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * INTERN: initialize DMA
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
apa102_dma_init (uint16_t n_leds)
{
    DMA_InitTypeDef         dma;
    DMA_StructInit (&dma);

    DMA_Cmd(APA102_DMA_STREAM, DISABLE);
    DMA_DeInit(APA102_DMA_STREAM);

    dma.DMA_Mode                = DMA_Mode_Normal;                          // normal dma mode
    dma.DMA_PeripheralBaseAddr  = (uint32_t)&SPI2->DR;                      // base addr of periph
#if defined(STM32F4XX)                                                      // STM32F4xx
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;          // 16bit
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;              // 16bit
#else                                                                       // STM32F10x
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;          // 16bit
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;                  // 8bit, saves RAM
#endif
    dma.DMA_BufferSize          = APA102_BUF_LEN(n_leds);                   // buffer size
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;                // disable periph inc
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;                     // enable memory inc
    dma.DMA_Priority            = DMA_Priority_High;                        // or DMA_Priority_VeryHigh;

#if defined(STM32F4XX)

    dma.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;
    dma.DMA_Channel             = APA102_DMA_CHANNEL;
    dma.DMA_Memory0BaseAddr     = (uint32_t)apa102_buf;
    dma.DMA_FIFOMode            = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull;
    dma.DMA_MemoryBurst         = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;

#elif defined (STM32F10X)

    dma.DMA_DIR                 = DMA_DIR_PeripheralDST;                    // direction: mem -> periph
    dma.DMA_M2M                 = DMA_M2M_Disable;                          // mem to mem: disabled
    dma.DMA_MemoryBaseAddr      = (uint32_t)apa102_buf;                     // memory base addr

#endif

    DMA_Init(APA102_DMA_STREAM, &dma);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * start DMA & timer (stopped when Transfer-Complete-Interrupt arrives)
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
apa102_dma_start (uint16_t n_leds)
{
    apa102_dma_status = 1;                                                          // set status to "busy"

    DMA_Cmd (APA102_DMA_STREAM, DISABLE);                                           // disable DMA
    DMA_SetCurrDataCounter(APA102_DMA_STREAM, APA102_BUF_LEN(n_leds));              // set new buffer size
    DMA_ITConfig(APA102_DMA_STREAM, DMA_IT_TC, ENABLE);                             // enable transfer complete interrupt
    DMA_Cmd(APA102_DMA_STREAM, ENABLE);                                             // DMA enable
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * clear all LEDs
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
apa102_clear_all (uint_fast16_t n_leds)
{
    APA102_RGB  rgb = { 0, 0, 0 };

    while (apa102_dma_status != 0)
    {
        ;
    }

    apa102_set_all_leds (&rgb, n_leds, 1);
}


/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * setup timer buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
apa102_setup_apa102_buf (uint_fast16_t n_leds)
{
    uint_fast16_t   n;
    uint_fast16_t   pos;
    APA102_RGB *    led;

    pos = 0;
    led = rgb_buf;

    for (n = 0; n < APA102_START_FRAME_BYTES; n++)
    {
        apa102_buf[pos++] = 0x00;
    }

    for (n = 0; n < n_leds; n++)
    {
        apa102_buf[pos++] = 0xFF;                                   // set GLOBAL to full brightness
        apa102_buf[pos++] = led->blue;                              // blue brightness
        apa102_buf[pos++] = led->green;                             // green brightness
        apa102_buf[pos++] = led->red;                               // red brightness
        led++;
    }

    for (n = 0; n < APA102_END_FRAME_BYTES(n_leds); n++)            // end frame
    {
        apa102_buf[pos++] = 0xFF;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ISR DMA (will be called, when all data has been transferred)
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void APA102_DMA_CHANNEL_ISR (void);
void
APA102_DMA_CHANNEL_ISR (void)
{
#if defined (STM32F4XX)
    if (DMA_GetITStatus(APA102_DMA_STREAM, APA102_DMA_CHANNEL_IRQ_FLAG))            // check transfer complete interrupt flag
    {
        DMA_ClearITPendingBit (APA102_DMA_STREAM, APA102_DMA_CHANNEL_IRQ_FLAG);     // reset flag
#elif defined (STM32F10X)
    if (DMA_GetITStatus(APA102_DMA_CHANNEL_IRQ_FLAG))                               // check transfer complete interrupt flag
    {
        DMA_ClearITPendingBit (APA102_DMA_CHANNEL_IRQ_FLAG);
#endif
        apa102_dma_status = 0;                                                      // set status to ready
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * refresh buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
apa102_refresh (uint_fast16_t n_leds)
{
    while (apa102_dma_status != 0)
    {
        ;                                                                           // wait until DMA transfer is ready
    }

    apa102_setup_apa102_buf (n_leds);
    apa102_dma_start(n_leds);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set one RGB value
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
apa102_set_led (uint_fast16_t n, APA102_RGB * rgb)
{
    if (n < APA102_MAX_LEDS)
    {
        rgb_buf[n].red      = rgb->red;
        rgb_buf[n].green    = rgb->green;
        rgb_buf[n].blue     = rgb->blue;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set all LEDs to RGB value
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
apa102_set_all_leds (APA102_RGB * rgb, uint_fast16_t n_leds, uint_fast8_t refresh)
{
    uint_fast16_t n;

    for (n = 0; n < n_leds; n++)
    {
        rgb_buf[n].red      = rgb->red;
        rgb_buf[n].green    = rgb->green;
        rgb_buf[n].blue     = rgb->blue;
    }

    if (refresh)
    {
        apa102_refresh (n_leds);
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * initialize APA102
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
apa102_init (void)
{
    GPIO_InitTypeDef        gpio;
    NVIC_InitTypeDef        nvic;

    apa102_dma_status = 0;

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize gpio
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    GPIO_StructInit (&gpio);
    APA102_GPIO_CLOCK_CMD (APA102_GPIO_CLOCK, ENABLE);              // clock enable

    gpio.GPIO_Pin     = APA102_GPIO_SCK_PIN | APA102_GPIO_MOSI_PIN;

#if defined (STM32F4XX)

    gpio.GPIO_Mode    = GPIO_Mode_AF;
    gpio.GPIO_OType   = GPIO_OType_PP;                              // GPIO_OType_PP: PushPull, GPIO_OType_OD: Open Drain, needs extern PullUp
    gpio.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed   = GPIO_Speed_100MHz;
    GPIO_Init(APA102_GPIO_PORT, &gpio);
    GPIO_RESET_BIT(APA102_GPIO_PORT, APA102_GPIO_SCK_PIN);          // set SCK to Low
    GPIO_PinAFConfig(APA102_GPIO_PORT, APA102_GPIO_SCK_SOURCE, APA102_SPI_AF);
    GPIO_PinAFConfig(APA102_GPIO_PORT, APA102_GPIO_MOSI_SOURCE, APA102_SPI_AF);

#elif defined (STM32F10X)

    gpio.GPIO_Mode    = GPIO_Mode_AF_PP;                            // GPIO_Mode_AF_PP: PushPull, GPIO_Mode_AF_OD: Open Drain, needs extern PullUp
    gpio.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(APA102_GPIO_PORT, &gpio);
    GPIO_RESET_BIT(APA102_GPIO_PORT, APA102_GPIO_SCK_PIN);          // set SCK to Low

#endif

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize SPI
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    SPI_InitTypeDef spi;

    APA102_SPI_CLOCK_CMD (APA102_SPI_CLOCK, ENABLE);                // clock enable (TIM)
    APA102_DMA_CLOCK_CMD (APA102_DMA_CLOCK, ENABLE);                // clock Enable (DMA)

    spi.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_4;          // F103: APB1CLK/4 = 36/4 = 9MHz, F4xx: APB1CLK/4 = 42/4 = 10.5 MHz
    spi.SPI_CPOL                = SPI_CPOL_Low;                     // passive state of SCK is low
    spi.SPI_CPHA                = SPI_CPHA_2Edge;                   // read data on falling edge, write data on rising edge (depends from CPOL!)
    spi.SPI_CRCPolynomial       = 0;                                // no CRC
    spi.SPI_DataSize            = SPI_DataSize_8b;                  // size of data word is 8 bit
    spi.SPI_Direction           = SPI_Direction_1Line_Tx;           // send only
    spi.SPI_FirstBit            = SPI_FirstBit_MSB;                 // MSB first
    spi.SPI_Mode                = SPI_Mode_Master;                  // Master
    spi.SPI_NSS                 = SPI_NSS_Soft;                     // NSS controlled by software, here not needed
    SPI_Init(APA102_SPI_DEVICE, &spi);

    SPI_I2S_DMACmd(APA102_SPI_DEVICE, SPI_I2S_DMAReq_Tx, ENABLE);   // enable DMA TX on SPI, RX not needed

    SPI_Cmd(APA102_SPI_DEVICE, ENABLE);                             // SPI enable

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize NVIC
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    nvic.NVIC_IRQChannel                    = APA102_DMA_CHANNEL_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  = 0;
    nvic.NVIC_IRQChannelSubPriority         = 0;
    nvic.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&nvic);

    /*-------------------------------------------------------------------------------------------------------------------------------------------
     * initialize DMA
     *-------------------------------------------------------------------------------------------------------------------------------------------
     */
    apa102_dma_init (APA102_MAX_LEDS);
// apa102_dma_start (APA102_MAX_LEDS);
    apa102_clear_all (APA102_MAX_LEDS);
}

#endif // DSP_USE_APA102
