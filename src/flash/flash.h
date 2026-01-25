/*------------------------------------------------------------------------------------------------------------------------------------
 * flash.h - emulate an EEPROM with FLASH memory and cache the data
 *
 * Copyright (c) 2018-2026 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef FLASH_H
#define FLASH_H

extern uint_fast8_t     flash_is_up;

extern uint_fast8_t     flash_flush (uint_fast8_t);
extern uint_fast8_t     flash_read (uint32_t, uint8_t *, uint32_t);
extern uint_fast8_t     flash_write (uint32_t, uint8_t *, uint32_t);
extern void             flash_init (void);

#endif
