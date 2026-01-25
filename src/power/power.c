/*-------------------------------------------------------------------------------------------------------------------------------------------
 * power.c - power routines for LED stripes
 *
 * Copyright (c) 2016-2026 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if defined (STM32F10X)
#include "stm32f10x.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#endif
#include "power.h"
#include "display-config.h"
#include "tft.h"
#include "io.h"
#include "log.h"

POWER_GLOBALS power =
{
    0xFF                                                                            // power.is_on
};


#if DSP_USE_TFTLED_RGB                                                              // TFT & SSD1963 only for STM32F407

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize Backlight
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_init (void)
{
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * switch power on
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_on (void)
{
    if (power.is_on != 1)
    {
        tft_backlight_on ();
        log_message ("switching power on");
        power.is_on = 1;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * switch power off
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_off (void)
{
    if (power.is_on != 0)
    {
        tft_backlight_off ();
        log_message ("switching power off");
        power.is_on = 0;
    }
}

#else // ! DSP_USE_TFTLED_RGB

#if defined (NUCLEO_BOARD)                                      // STM32F4xx Nucleo Board: PC8
#define POWER_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define POWER_PERIPH            RCC_AHB1Periph_GPIOC
#define POWER_PORT              GPIOC
#define POWER_PIN               GPIO_Pin_8

#elif defined (BLACKPILL_BOARD)                                 // STM32F4x1 BlackPill Board: PB0
#define POWER_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define POWER_PERIPH            RCC_AHB1Periph_GPIOB
#define POWER_PORT              GPIOB
#define POWER_PIN               GPIO_Pin_0

#elif defined (BLUEPILL_BOARD)
#define POWER_PERIPH_CLOCK_CMD  RCC_APB2PeriphClockCmd          // STM32F103 BluePill Board: PB0
#define POWER_PERIPH            RCC_APB2Periph_GPIOB
#define POWER_PORT              GPIOB
#define POWER_PIN               GPIO_Pin_0

#else
#error STM32 unknown
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize power port
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_init (void)
{
    POWER_PERIPH_CLOCK_CMD (POWER_PERIPH, ENABLE);              // enable clock for power port
    GPIO_SET_PIN_OUT_PP(POWER_PORT, POWER_PIN, GPIO_Speed_2MHz);
    log_message ("power_init() called");
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * switch power on
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_on (void)
{
    if (power.is_on != 1)
    {
        GPIO_SET_BIT(POWER_PORT, POWER_PIN);
        log_message ("switching power on");
        power.is_on = 1;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * switch power off
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_off (void)
{
    if (power.is_on != 0)
    {
        GPIO_RESET_BIT(POWER_PORT, POWER_PIN);
        log_message ("switching power off");
        power.is_on = 0;
    }
}

#endif // DSP_USE_TFTLED_RGB
