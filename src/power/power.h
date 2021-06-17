/*-------------------------------------------------------------------------------------------------------------------------------------------
 * power.h - declarations of power routines for LED stripes
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef POWER_H
#define POWER_H

#if defined (STM32F1XX)
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#endif

typedef struct
{
    uint_fast8_t        is_on;
} POWER_GLOBALS;

extern POWER_GLOBALS    power;

extern void             power_init (void);
extern void             power_on (void);
extern void             power_off (void);

#endif
