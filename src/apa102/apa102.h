/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * apa102.h - interface declarations of APA102 driver
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef APA102_H
#define APA102_H

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

#include "apa102-config.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * RGB LED color definition
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} APA102_RGB;

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * APA102 interface definition
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
extern void apa102_init (void);
extern void apa102_refresh (uint_fast16_t);
extern void apa102_set_led (uint_fast16_t, APA102_RGB *);
extern void apa102_set_all_leds (APA102_RGB *, uint_fast16_t, uint_fast8_t);

#endif
