/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * tempsensor.h - declaration of temperature sensor routines
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#if defined (STM32F10X)
#  include "stm32f10x.h"
#elif defined (STM32F4XX)
#  include "stm32f4xx.h"
#endif

typedef struct
{
    uint_fast8_t                correction;
    uint_fast8_t                index;
} TEMP_GLOBALS;

extern TEMP_GLOBALS             gtemp;

extern uint_fast8_t             temp_correction;
extern uint_fast8_t             temp_index;

extern uint_fast8_t             temp_start_conversion (uint_fast8_t);
extern uint_fast8_t             temp_read_temp_index (void);
extern uint_fast8_t             temp_read_config_from_eeprom (uint32_t);
extern uint_fast8_t             temp_write_config_to_eeprom (void);
extern uint_fast8_t             temp_get_temp_correction (void);
extern uint_fast8_t             temp_set_temp_correction (uint_fast8_t);
extern uint_fast8_t             temp_init (void);

#endif
