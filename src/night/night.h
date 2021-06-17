/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * night.h - declarations of night time routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef NIGHT_H
#define NIGHT_H

#include <string.h>
#include <stdint.h>

#define MAX_NIGHT_TIMES             8

#define NIGHT_TIME_FLAG_ACTIVE      0x80                                    // flag if entry is active (1) or not (0)
#define NIGHT_TIME_FLAG_SWITCH_ON   0x40                                    // flag if entry switches on (1) or off (0)
#define NIGHT_TIME_FROM_DAY_MASK    0x38                                    // 3 bits for from day
#define NIGHT_TIME_TO_DAY_MASK      0x07                                    // 3 bits for to day

typedef struct
{
    uint_fast8_t        flags;                                              // flags
    uint_fast16_t       minutes;                                            // time in minutes 0 - 1439
} NIGHT_TIME;

extern NIGHT_TIME       night_time[MAX_NIGHT_TIMES];
extern NIGHT_TIME       ambilight_night_time[MAX_NIGHT_TIMES];

extern uint_fast8_t     night_read_data_from_eeprom (uint32_t);
extern uint_fast8_t     night_write_data_to_eeprom (void);
extern uint_fast8_t     night_check_night_times (uint_fast8_t, uint_fast8_t, uint_fast8_t, uint_fast16_t);
extern void             night_init (void);

#endif
