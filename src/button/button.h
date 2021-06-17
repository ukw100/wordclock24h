/*-------------------------------------------------------------------------------------------------------------------------------------------
 * button.h - declaration of user button functions (STM32F4 Discovery / STM32F4xx Nucleo / STM32F103)
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef BUTTON_H
#define BUTTON_H

#ifdef STM32F10X
#  include "stm32f10x.h"
#  include "stm32f10x_gpio.h"
#  include "stm32f10x_rcc.h"
#elif defined STM32F4XX
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_rcc.h"
#endif

extern void         button_init (void);
extern uint_fast8_t button_pressed (void);

#endif
