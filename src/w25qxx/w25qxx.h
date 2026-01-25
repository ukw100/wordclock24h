/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * w25qxx.h - interface declarations of W25QXX driver
 *
 * Copyright (c) 2018-2026 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef W25QXX_H
#define W25QXX_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"

#include "misc.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * W25QXX interface definition
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
extern void             w25qxx_init (void);
extern uint_fast8_t     w25qxx_device_id (void);
extern uint_fast8_t     w25qxx_statusreg1 (void);
extern uint_fast8_t     w25qxx_statusreg2 (void);
extern char *           w25qxx_unique_id (void);
extern void             w25qxx_read_page (uint8_t *, uint32_t, uint32_t);
extern void             w25qxx_write_page (uint8_t *, uint32_t, uint32_t);
extern void             w25qxx_erase_sector (uint32_t);

#endif
