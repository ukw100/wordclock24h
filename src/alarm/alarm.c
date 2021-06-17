/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * alarm.c - alarm time routines
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include "eeprom.h"
#include "eeprom-data.h"
#include "alarm.h"
#include "log.h"

/*--------------------------------------------------------------------------------------------------------------------------------------
 * globals:
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
ALARM_TIME              alarm_time[MAX_ALARM_TIMES];

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read configuration data from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
alarm_read_data_from_eeprom (uint32_t eeprom_version)
{
    unsigned char   alarm_time_buf[EEPROM_DATA_SIZE_ALARM_TIME];
    uint_fast8_t    i;
    uint_fast8_t    offset;
    uint_fast8_t    reinit = 0;
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        if (eeprom_version >= EEPROM_VERSION_2_7)
        {
            if (eeprom_read (EEPROM_DATA_OFFSET_ALARM_TIME, alarm_time_buf, EEPROM_DATA_SIZE_ALARM_TIME))
            {
                for (offset = 0, i = 0; i < MAX_ALARM_TIMES; i++)
                {
                    alarm_time[i].flags      = alarm_time_buf[offset++];
                    alarm_time[i].minutes    = alarm_time_buf[offset++] * 60;
                    alarm_time[i].minutes   += alarm_time_buf[offset++];

                    if (alarm_time[i].minutes > 24 * 60)                                // invalid EEPROM values?
                    {
                        alarm_time[i].flags      = 0;
                        alarm_time[i].minutes    = 0;
                        reinit = 1;                                                     // must re-initialize eeprom data
                    }
                }

                rtc = 1;
            }

            if (reinit)
            {
                alarm_write_data_to_eeprom ();
            }
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write configuration data to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
alarm_write_data_to_eeprom (void)
{
    unsigned char   alarm_time_buf[EEPROM_DATA_SIZE_ALARM_TIME];
    uint_fast8_t    i;
    uint_fast8_t    offset;
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        for (offset = 0, i = 0; i < MAX_ALARM_TIMES; i++)
        {
            alarm_time_buf[offset++] = alarm_time[i].flags;
            alarm_time_buf[offset++] = alarm_time[i].minutes / 60;                      // hh
            alarm_time_buf[offset++] = alarm_time[i].minutes % 60;                      // mm
        }

        if (eeprom_write (EEPROM_DATA_OFFSET_ALARM_TIME, alarm_time_buf, EEPROM_DATA_SIZE_ALARM_TIME))
        {
            rtc = 1;
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check alarm_times
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
alarm_check_alarm_times (uint_fast8_t wday, uint_fast16_t m)
{
    uint_fast8_t    idx;
    uint_fast8_t    alarm_found = 0;
    uint_fast8_t    rtc = 0;

    for (idx = 0; idx < MAX_ALARM_TIMES; idx++)
    {
        if ((alarm_time[idx].flags & ALARM_TIME_FLAG_ACTIVE) && alarm_time[idx].minutes == m)
        {
            uint_fast8_t   from_wday    = (alarm_time[idx].flags & ALARM_TIME_FROM_DAY_MASK) >> 3;
            uint_fast8_t   to_wday      = (alarm_time[idx].flags & ALARM_TIME_TO_DAY_MASK);

            if (from_wday == to_wday)                                           // e.g. Su-Su 0-0
            {
                alarm_found = (wday == from_wday);
            }
            else if (from_wday < to_wday)                                       // e.g. Mo-Fr 1-5
            {
                alarm_found = (wday >= from_wday && wday <= to_wday);
            }
            else                                                                // e.g. Sa-We 6-3
            {
                alarm_found = ! (wday > to_wday && wday < from_wday);
            }

            if (alarm_found)
            {
                rtc = idx + 1;                                              // 001.mp3 ... 008.mp3
                break;
            }
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * init alarm times
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
alarm_init (void)
{
    return;                                                                 // nothing to do
}
