/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * sk6812.h - interface declarations of SK6812 RGBW driver
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef SK6812_H
#define SK6812_H

#if defined (STM32F10X)
#  include "stm32f10x.h"
#  include "stm32f10x_gpio.h"
#  include "stm32f10x_rcc.h"
#  include "stm32f10x_tim.h"
#  include "stm32f10x_dma.h"
#elif defined (STM32F4XX)
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_rcc.h"
#  include "stm32f4xx_tim.h"
#  include "stm32f4xx_dma.h"
#endif

#include "misc.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * RGB(W) LED color definition
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
#if DSP_USE_SK6812_RGBW == 1
  uint8_t white;
#endif
} SK6812_RGBW;

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * SK6812 interface definition
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
extern void sk6812_init (void);
extern void sk6812_refresh (uint_fast16_t);

extern void sk6812_set_led (uint_fast16_t, SK6812_RGBW *);
extern void sk6812_set_all_leds (SK6812_RGBW *, uint_fast16_t, uint_fast8_t);

#endif
