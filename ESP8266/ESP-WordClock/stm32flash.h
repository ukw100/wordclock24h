/*----------------------------------------------------------------------------------------------------------------------------------------
 * stm32flash.h - flash STM32 per Bootloader API
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef STM32FLASH_H
#define STM32FLASH_H

extern void stm32_flash_from_server (const char *, const char *, const char *);
extern void stm32_flash_from_local (void);
extern void stm32_reset (void);
extern void stm32_flash_init (void);

#endif
