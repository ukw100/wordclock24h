/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * alarm.h - declarations of alarm time routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef ALARM_H
#define ALARM_H

#include <string.h>
#include <stdint.h>

#define MAX_ALARM_TIMES             8

#define ALARM_TIME_FLAG_ACTIVE      0x80                                    // flag if entry is active (1) or not (0)
#define ALARM_TIME_NOT_USED         0x40                                    // bit not used
#define ALARM_TIME_FROM_DAY_MASK    0x38                                    // 3 bits for from day
#define ALARM_TIME_TO_DAY_MASK      0x07                                    // 3 bits for to day

typedef struct
{
    uint_fast8_t        flags;                                              // flags
    uint_fast16_t       minutes;                                            // time in minutes 0 - 1439
} ALARM_TIME;

extern ALARM_TIME       alarm_time[MAX_ALARM_TIMES];
extern ALARM_TIME       ambilight_alarm_time[MAX_ALARM_TIMES];

extern uint_fast8_t     alarm_read_data_from_eeprom (uint32_t);
extern uint_fast8_t     alarm_write_data_to_eeprom (void);
extern uint_fast8_t     alarm_check_alarm_times (uint_fast8_t, uint_fast16_t);
extern void             alarm_init (void);

#endif
