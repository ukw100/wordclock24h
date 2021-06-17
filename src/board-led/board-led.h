/*-------------------------------------------------------------------------------------------------------------------------------------------
 * board-led.h - declarations of LED routines
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef BOARD_LED_H
#define BOARD_LED_H

#if defined (STM32F1XX)
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#endif

extern void         board_led_init (void);
extern void         board_led_on (void);
extern void         board_led_off (void);

#endif
