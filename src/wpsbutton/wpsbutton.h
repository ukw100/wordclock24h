/*-------------------------------------------------------------------------------------------------------------------------------------------
 * wpsbutton.h - declaration of user wps button functions (STM32F4xx Nucleo and STM32F103)
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef WPSBUTTON_H
#define WPSBUTTON_H

#ifdef STM32F10X
#  include "stm32f10x.h"
#  include "stm32f10x_gpio.h"
#  include "stm32f10x_rcc.h"
#elif defined STM32F4XX
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_rcc.h"
#endif

extern void         wpsbutton_init (void);
extern uint_fast8_t wpsbutton_pressed (void);

#endif
