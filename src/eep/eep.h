/*-------------------------------------------------------------------------------------------------------------------------------------------
 * eep.h - eeprom/flash wrapper
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef EEP_H
#define EEP_H

#if defined (STM32F10X)
#include "stm32f10x.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#endif

#if defined (BLACK_BOARD)       // use w25q16 flash on STM32F407VE Black Board

#include "flash.h"

#define eep_init(x)             flash_init ()
#define eep_get_address()       flash_get_address ()
#define eep_read(a,b,l)         flash_read (a, b, l)
#define eep_write(a,b,l)        flash_write (a, b, l)
#define eep_is_up               flash_is_up
#define eep_flush(x)            flash_flush(x)

#else

#include "eeprom.h"

#define eep_init(x)             eeprom_init (x)
#define eep_get_address()       eeprom_get_address ()
#define eep_read(a,b,l)         eeprom_read (a, b, l)
#define eep_write(a,b,l)        eeprom_write (a, b, l)
#define eep_is_up               eeprom_is_up
#define eep_flush(x)

#endif

#endif // EEP_H
