/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay.h - declaration of delay functions
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DELAY_H
#define DELAY_H

#if defined (STM32F10X)
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#endif

// resolution of delay functions
#define DELAY_RESOLUTION_1_US             1
#define DELAY_RESOLUTION_5_US             5
#define DELAY_RESOLUTION_10_US           10
#define DELAY_RESOLUTION_100_US         100

#define DELAY_DEFAULT_RESOLUTION        DELAY_RESOLUTION_100_US

extern volatile uint32_t                delay_counter;              // counts down in units of resolution

extern void delay_usec (uint32_t);                                  // delay of n usec, only reasonable if resolution is 1us or 5us
extern void delay_msec (uint32_t);                                  // delay of n msec
extern void delay_sec  (uint32_t);                                  // delay of n sec
extern void delay_init (uint_fast8_t);                              // init delay functions

#endif
