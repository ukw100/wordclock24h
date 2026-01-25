/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * max11820.c - interface declarations of XPT2046 Touch driver
 *
 * Copyright (c) 2018-2026 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef XPT2046_H
#define XPT2046_H

#if defined (BLACK_BOARD)       // STM32F407VE Blackboard with TFT & Touch

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"

#include "misc.h"

extern volatile uint_fast8_t    xpt2046_touched;                            // flag: touched (set by IRQ)

extern uint32_t                 xpt2046_get_xy (uint32_t *, uint32_t *);    // get x & y coordinate
extern uint_fast8_t             xpt2046_init (void);                        // init

#endif // BLACK_BOARD

#endif // XPT2046_H
