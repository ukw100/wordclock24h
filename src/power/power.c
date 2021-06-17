/*-------------------------------------------------------------------------------------------------------------------------------------------
 * power.c - power routines for LED stripes
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
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
#include "io.h"
#include "log.h"

#if defined (STM32F4XX)                                         // STM32F4xx Nucleo Board PC8
#define POWER_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define POWER_PERIPH            RCC_AHB1Periph_GPIOC
#define POWER_PORT              GPIOC
#define POWER_PIN               GPIO_Pin_8

#elif defined (STM32F103)
#define POWER_PERIPH_CLOCK_CMD  RCC_APB2PeriphClockCmd          // STM32F103 PB0
#define POWER_PERIPH            RCC_APB2Periph_GPIOB
#define POWER_PORT              GPIOB
#define POWER_PIN               GPIO_Pin_0

#else
#error STM32 unknown
#endif

POWER_GLOBALS                   power =
{
    0xFF                                                        // power.is_on
};

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize power port
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
power_init (void)
{
    GPIO_InitTypeDef gpio;

    GPIO_StructInit (&gpio);
    POWER_PERIPH_CLOCK_CMD (POWER_PERIPH, ENABLE);     // enable clock for power port

    gpio.GPIO_Pin   = POWER_PIN;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;

#if defined (STM32F10X)
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
#elif defined (STM32F4XX)
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
#endif

    GPIO_Init(POWER_PORT, &gpio);
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
