/*-------------------------------------------------------------------------------------------------------------------------------------------
 * dcf77.h - declaration of dcf77 routines
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DCF77_H
#define DCF77_H

#include <stdint.h>

#if defined (STM32F10X)
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#endif

#include <time.h>

extern void             dcf77_tick (void);
extern uint_fast8_t     dcf77_time (struct tm *);
extern void             dcf77_init (void);

#endif
