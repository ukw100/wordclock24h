#if 0 // yet not used
/*-------------------------------------------------------------------------------------------------------------------------------------------
 * softi2c.h - declarations of soft I2C routines
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef I2C_H
#define I2C_H

#if defined (STM32F10X)
#  include "stm32f10x.h"
#  include "stm32f10x_gpio.h"
#  include "stm32f10x_rcc.h"
#  include "stm32f10x_i2c.h"
#elif defined (STM32F4XX)
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_rcc.h"
#  include "stm32f4xx_i2c.h"
#endif

#define I2C_OK                  (0)
#define I2C_ERROR               (-1)

void            softi2c_init  (void);
int_fast16_t    softi2c_read  (uint_fast8_t, uint_fast16_t, uint_fast8_t, uint8_t *, uint_fast16_t);
int_fast16_t    softi2c_write (uint_fast8_t, uint_fast16_t, uint_fast8_t, uint8_t *, uint_fast16_t);

#endif
#endif // 0
