/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * night.c - night time routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
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
#include "night.h"
#include "log.h"

/*--------------------------------------------------------------------------------------------------------------------------------------
 * globals:
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
NIGHT_TIME              night_time[MAX_NIGHT_TIMES];
NIGHT_TIME              ambilight_night_time[MAX_NIGHT_TIMES];

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read configuration data from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
night_read_data_from_eeprom (uint32_t eeprom_version)
{
    unsigned char   night_time_buf[EEPROM_DATA_SIZE_NIGHT_TIME];
    uint_fast8_t    i;
    uint_fast8_t    offset;
    uint_fast8_t    reinit = 0;
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        if (eeprom_version >= EEPROM_VERSION_1_7)
        {
            if (eeprom_read (EEPROM_DATA_OFFSET_NIGHT_TIME, night_time_buf, EEPROM_DATA_SIZE_NIGHT_TIME))
            {
                for (offset = 0, i = 0; i < MAX_NIGHT_TIMES; i++)
                {
                    night_time[i].flags      = night_time_buf[offset++];
                    night_time[i].minutes    = night_time_buf[offset++] * 60;
                    night_time[i].minutes   += night_time_buf[offset++];

                    if (night_time[i].minutes > 24 * 60)                                // invalid EEPROM values?
                    {
                        night_time[i].flags      = 0;
                        night_time[i].minutes    = 0;
                        reinit = 1;                                                     // must re-initialize eeprom data
                    }
                }

                rtc = 1;
            }

            if (eeprom_version >= EEPROM_VERSION_2_5)
            {
                if (eeprom_read (EEPROM_DATA_OFFSET_AMBI_NIGHT_TIME, night_time_buf, EEPROM_DATA_SIZE_AMBI_NIGHT_TIME))
                {
                    for (offset = 0, i = 0; i < MAX_NIGHT_TIMES; i++)
                    {
                        ambilight_night_time[i].flags      = night_time_buf[offset++];
                        ambilight_night_time[i].minutes    = night_time_buf[offset++] * 60;
                        ambilight_night_time[i].minutes   += night_time_buf[offset++];

                        if (ambilight_night_time[i].minutes > 24 * 60)                                // invalid EEPROM values?
                        {
                            ambilight_night_time[i].flags      = 0;
                            ambilight_night_time[i].minutes    = 0;
                            reinit = 1;                                                     // must re-initialize eeprom data
                        }
                    }
                }
            }

            if (reinit)
            {
                night_write_data_to_eeprom ();
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
night_write_data_to_eeprom (void)
{
    unsigned char   night_time_buf[EEPROM_DATA_SIZE_NIGHT_TIME];
    uint_fast8_t    i;
    uint_fast8_t    offset;
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        for (offset = 0, i = 0; i < MAX_NIGHT_TIMES; i++)
        {
            night_time_buf[offset++] = night_time[i].flags;
            night_time_buf[offset++] = night_time[i].minutes / 60;                      // hh
            night_time_buf[offset++] = night_time[i].minutes % 60;                      // mm
        }

        if (eeprom_write (EEPROM_DATA_OFFSET_NIGHT_TIME, night_time_buf, EEPROM_DATA_SIZE_NIGHT_TIME))
        {
            for (offset = 0, i = 0; i < MAX_NIGHT_TIMES; i++)
            {
                night_time_buf[offset++] = ambilight_night_time[i].flags;
                night_time_buf[offset++] = ambilight_night_time[i].minutes / 60;        // hh
                night_time_buf[offset++] = ambilight_night_time[i].minutes % 60;        // mm
            }

            if (eeprom_write (EEPROM_DATA_OFFSET_AMBI_NIGHT_TIME, night_time_buf, EEPROM_DATA_SIZE_AMBI_NIGHT_TIME))
            {
                rtc = 1;
            }
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check night_times
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
night_check_night_times (uint_fast8_t is_ambilight, uint_fast8_t power_is_on, uint_fast8_t wday, uint_fast16_t m)
{
    NIGHT_TIME *    ntp;
    uint_fast8_t    i;
    uint_fast8_t    condition;
    uint_fast8_t    rtc = 0;

    if (is_ambilight)
    {
        ntp = ambilight_night_time;
    }
    else
    {
        ntp = night_time;
    }

    for (i = 0; i < MAX_NIGHT_TIMES; i++, ntp++)
    {
        if ((ntp->flags & NIGHT_TIME_FLAG_ACTIVE) && ntp->minutes == m)
        {
            condition = ntp->flags & NIGHT_TIME_FLAG_SWITCH_ON;

            if (power_is_on)
            {
                condition = !condition;
            }

            if (condition)
            {
                uint_fast8_t   from_wday    = (ntp->flags & NIGHT_TIME_FROM_DAY_MASK) >> 3;
                uint_fast8_t   to_wday      = (ntp->flags & NIGHT_TIME_TO_DAY_MASK);

                if (from_wday == to_wday)                                       // e.g. Su-Su 0-0
                {
                    rtc = (wday == from_wday);
                }
                else if (from_wday < to_wday)                                   // e.g. Mo-Fr 1-5
                {
                    rtc = (wday >= from_wday && wday <= to_wday);
                }
                else                                                            // e.g. Sa-We 6-3
                {
                    rtc = ! (wday > to_wday && wday < from_wday);
                }

                if (rtc)
                {
                    break;
                }
            }
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * init night times
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
night_init (void)
{
    return;                                                                 // nothing to do
}
