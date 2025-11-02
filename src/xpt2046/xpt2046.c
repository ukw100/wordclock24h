/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * max11820.c - XPT2046 Touch driver
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (BLACK_BOARD)                                       // STM32F407VE Blackboard with TFT & Touch

#include <stdint.h>
#include "xpt2046.h"
#include "delay.h"
#include "log.h"
#include "io.h"

#define Z_THRESHOLD                             3500            // pressure threshold

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * global variables
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
volatile uint_fast8_t                           xpt2046_touched = 0;
static volatile uint32_t                        xpt2046_data;
static volatile uint_fast8_t                    xpt2046_data_valid;

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * port/pin definitions
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
// CS Pin = PB12
#define XPT2046_CS_PORT                         GPIOB
#define XPT2046_CS_PIN                          GPIO_Pin_12

// SPI Port = Port B
#define RCC_SPI_PORT                            RCC_AHB1Periph_GPIOB
#define SPI_PORT                                GPIOB

// SCK Pin = PB13
#define SPI_SCK_PIN                             GPIO_Pin_13
#define SPI_SCK_PINSOURCE                       GPIO_PinSource13

// MISO Pin = PB14
#define SPI_MISO_PIN                            GPIO_Pin_14
#define SPI_MISO_PINSOURCE                      GPIO_PinSource14

// MOSI Pin = PB15
#define SPI_MOSI_PIN                            GPIO_Pin_15
#define SPI_MOSI_PINSOURCE                      GPIO_PinSource15

// T_PEN_IRQ Pin = PC5
#define XPT2046_PENIRQ_PORT                     GPIOC                                           // port C
#define XPT2046_PENIRQ_PIN                      GPIO_Pin_5                                      // pin 5
#define XPT2046_PENIRQ_EXTI_PORTSOURCE          EXTI_PortSourceGPIOC                            // port C
#define XPT2046_PENIRQ_EXTI_PINSOURCE           EXTI_PinSource5                                 // pin 5
#define XPT2046_PENIRQ_LINE                     EXTI_Line5                                      // pin 5
#define XPT2046_PENIRQ_CHANNEL                  EXTI9_5_IRQn                                    // pin 5
#define XPT2046_PENIRQ_RCC_AHB1PERIPH_GPIO      RCC_AHB1Periph_GPIOC                            // Port C

typedef enum
{
    KAL_PKT1 = 0,
    KAL_PKT2,
    KAL_PKT3,
    KAL_READY,
    KAL_OK,
    KAL_ERR
} TOUCH_KAL_PKT_t;

typedef enum
{
    MW_NONE = 0,
    MW_4
} TOUCH_MW_t;

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * xpt2046_set_cs() - set CS
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
xpt2046_set_cs (void)
{
    GPIO_RESET_BIT (XPT2046_CS_PORT, XPT2046_CS_PIN);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * xpt2046_reset_cs() - reset CS
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
xpt2046_reset_cs (void)
{
    GPIO_SET_BIT (XPT2046_CS_PORT, XPT2046_CS_PIN);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * xpt2046_write() - write data into register
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if 000
static void
xpt2046_write (uint8_t reg, uint8_t val)
{
    xpt2046_set_cs ();
    delay_usec (1);

    SPI_I2S_SendData (SPI2, reg << 1);
    (void) xpt2046_get_data ();                                                     // wait until xpt2046_data_valid is true, but ignore data
    SPI_I2S_SendData (SPI2, val);
    (void) xpt2046_get_data ();                                                     // wait until xpt2046_data_valid is true, but ignore data

    delay_usec (1);
    xpt2046_reset_cs ();
}
#endif
/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * xpt2046_get_data() - get data
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint32_t
xpt2046_get_data (void)
{
    uint32_t    rtc;

    while (! xpt2046_data_valid)
    {
        ;
    }

    rtc = xpt2046_data;
    xpt2046_data_valid = 0;

    return rtc;
}

static uint32_t
xpt2046_get (uint32_t channel)
{
    uint32_t    tp_msb;
    uint32_t    tp_lsb;
    uint32_t    tp;

    xpt2046_set_cs ();
    delay_usec (1);
    SPI_I2S_SendData (SPI2, channel);                                           // X or y
    (void) xpt2046_get_data ();                                                 // wait until xpt2046_data_valid is true, but ignore data
    // delay_usec (100);                                                           // wait until ready

    SPI_I2S_SendData (SPI2, 0x00);
    tp_msb = xpt2046_get_data ();
    SPI_I2S_SendData (SPI2, 0x00);
    tp_lsb = xpt2046_get_data ();
    delay_usec (1);
    xpt2046_reset_cs ();

    tp = ((tp_msb << 8) | (tp_lsb)) >> 3;                                       // format is xMMMMMMM MLLLLxxx
    return tp;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * xpt2046_get_xy() - get x & y coordinate
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
uint32_t
xpt2046_get_xy (uint32_t * px, uint32_t * py)
{
    uint32_t    tp_x = 0;
    uint32_t    tp_y = 0;
    uint32_t    idx;

    for (idx = 0; idx < 16; idx++)
    {
        tp_x += xpt2046_get (0xD0);
        delay_usec (1);
        tp_y += xpt2046_get (0x90);
    }

    *px = tp_x;
    *py = tp_y;

    return 1;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Handle EXTI interrupt for EXTI 5 line (PC5)
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void EXTI9_5_IRQHandler (void);                                                     // keep compiler happy

void
EXTI9_5_IRQHandler (void)
{
    if (EXTI_GetITStatus (XPT2046_PENIRQ_LINE) == SET)
    {
        EXTI_ClearITPendingBit (XPT2046_PENIRQ_LINE);
        xpt2046_touched = 1;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Handle SPI2 interrupt
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void SPI2_IRQHandler (void);                                                        // keep compiler happy

void
SPI2_IRQHandler (void)
{
    if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET)
    {
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
        xpt2046_data = SPI_I2S_ReceiveData (SPI2);
        xpt2046_data_valid = 1;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * xpt2046_init() - init XPT2046
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
xpt2046_init (void)
{
    SPI_InitTypeDef     spi;
    EXTI_InitTypeDef    exti;
    NVIC_InitTypeDef    nvic;
    uint32_t            x;
    uint32_t            y;
    uint_fast8_t        rtc = 0;

    RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI2, ENABLE);

    spi.SPI_CPOL                = SPI_CPOL_Low;
    spi.SPI_CPHA                = SPI_CPHA_1Edge;
    spi.SPI_FirstBit            = SPI_FirstBit_MSB;
#if 0
    spi.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_64;                         // F4xx: APB2CLK/64  = 84/64  = 1.3 MHz (max ist 2 MHz)
#else
    spi.SPI_BaudRatePrescaler   = SPI_BaudRatePrescaler_256;                        // F4xx: APB2CLK/256 = 84/256 = 328 kHz (max ist 2 MHz)
#endif
    spi.SPI_Direction           = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_Mode                = SPI_Mode_Master;
    spi.SPI_DataSize            = SPI_DataSize_8b;
    spi.SPI_NSS                 = SPI_NSS_Soft;
    spi.SPI_FirstBit            = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial       = 0;                                                // no CRC
    SPI_Init(SPI2, &spi);

    RCC_AHB1PeriphClockCmd (RCC_SPI_PORT, ENABLE);

    GPIO_PinAFConfig(GPIOB, SPI_SCK_PINSOURCE, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, SPI_MISO_PINSOURCE, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, SPI_MOSI_PINSOURCE, GPIO_AF_SPI2);

    GPIO_SET_PIN_AF_PP(SPI_PORT, SPI_SCK_PIN | SPI_MISO_PIN | SPI_MOSI_PIN, GPIO_Speed_50MHz);

    GPIO_SET_PIN_OUT_PP(XPT2046_CS_PORT, XPT2046_CS_PIN, GPIO_Speed_50MHz);
    GPIO_SetBits(SPI_PORT, XPT2046_CS_PIN);

    //Start SPI2 with interrupts
    SPI_Init(SPI2, &spi);
    nvic.NVIC_IRQChannel                   = SPI2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority        = 1;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);

    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);

    SPI_Cmd(SPI2, ENABLE);

    RCC_AHB1PeriphClockCmd(XPT2046_PENIRQ_RCC_AHB1PERIPH_GPIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // external Interrupt at PC5 as input
    GPIO_SET_PIN_IN_UP (XPT2046_PENIRQ_PORT, XPT2046_PENIRQ_PIN, GPIO_Speed_50MHz);
    SYSCFG_EXTILineConfig(XPT2046_PENIRQ_EXTI_PORTSOURCE, XPT2046_PENIRQ_EXTI_PINSOURCE);

    exti.EXTI_Line                          = XPT2046_PENIRQ_LINE;
    exti.EXTI_LineCmd                       = ENABLE;
    exti.EXTI_Mode                          = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger                       = EXTI_Trigger_Falling;
    EXTI_Init (&exti);

    nvic.NVIC_IRQChannel                    = XPT2046_PENIRQ_CHANNEL;
    nvic.NVIC_IRQChannelPreemptionPriority  = 0x0F;                                 // 0x00 ?
    nvic.NVIC_IRQChannelSubPriority         = 0x0F;                                 // 0x00 ?
    nvic.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&nvic);

    xpt2046_get_xy (&x, &y);

    if (x <= 0x0FFF && y <= 0x0FFF)
    {
        rtc = 1;                                                                    // touch exists
    }

    return rtc;
}

#endif // BLACK_BOARD
