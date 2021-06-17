/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ldr.h - adc functions
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef LDR_H
#define LDR_H

#if defined (STM32F10X)
#  include "stm32f10x.h"
#  include "stm32f10x_gpio.h"
#  include "stm32f10x_rcc.h"
#  include "stm32f10x_adc.h"
#elif defined (STM32F4XX)
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_rcc.h"
#  include "stm32f4xx_adc.h"
#endif

typedef struct
{
    uint8_t             ldr_value;
    uint16_t            ldr_raw_value;
    uint16_t            ldr_min_value;
    uint16_t            ldr_max_value;
} LDR_GLOBALS;

extern LDR_GLOBALS      ldr;

extern void             ldr_start_conversion (void);
extern uint_fast8_t     ldr_poll_brightness (void);
extern void             ldr_read_config_from_eeprom (uint32_t);
extern uint_fast8_t     ldr_write_config_to_eeprom (void);
extern uint_fast16_t    ldr_set_min_value (void);
extern uint_fast16_t    ldr_set_max_value (void);
extern void             ldr_init (void);

#endif // LDR_H
