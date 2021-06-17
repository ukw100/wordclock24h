/*-------------------------------------------------------------------------------------------------------------------------------------------
 * i2c.h - declarations of I2C routines
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
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
#define I2C_CHANNEL     I2C1
#elif defined (STM32F4XX)
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_rcc.h"
#  include "stm32f4xx_i2c.h"
#define I2C_CHANNEL     I2C3
#endif

#define I2C_OK                  (0)
#define I2C_ERROR_NO_FLAG_SB    (-1)
#define I2C_ERROR_NO_FLAG_ADDR  (-2)
#define I2C_ERROR_NO_FLAG_TXE   (-3)
#define I2C_ERROR_NO_TXE_OR_BTF (-4)
#define I2C_ERROR_NO_FLAG_SB2   (-5)
#define I2C_ERROR_NO_FLAG_ADDR2 (-6)
#define I2C_ERROR_NO_FLAG_RXNE  (-7)

void            i2c_init  (uint32_t);
int_fast16_t    i2c_read  (uint_fast8_t, uint_fast16_t, uint_fast8_t, uint8_t *, uint_fast16_t);
int_fast16_t    i2c_write (uint_fast8_t, uint_fast16_t, uint_fast8_t, uint8_t *, uint_fast16_t);

#endif
