/*-------------------------------------------------------------------------------------------------------------------------------------------
 * overlay.c - handle overlays
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "eeprom.h"
#include "eeprom-data.h"
#include "base.h"
#include "log.h"
#include "overlay.h"

OVERLAY_GLOBALS  overlay;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read configuration from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
overlay_read_config_from_eeprom (uint32_t eeprom_version)
{
    uint_fast8_t  idx;
    uint_fast8_t  rtc = 0;

    if (eeprom_is_up)
    {
        rtc = 1;

        if (eeprom_version >= EEPROM_VERSION_2_8)
        {
            uint8_t         overlay_buffer8[EEPROM_OVERLAY_ENTRY_SIZE];
            uint8_t         n_overlays8 = 0;
            uint_fast8_t    bidx;

            eeprom_read (EEPROM_DATA_OFFSET_N_OVERLAYS,  &n_overlays8, EEPROM_DATA_SIZE_N_OVERLAYS);

            if (n_overlays8 >= MAX_OVERLAYS)
            {
                n_overlays8 = 0;
            }

            overlay.n_overlays = n_overlays8;

            for (idx = 0; idx < overlay.n_overlays; idx++)
            {
                eeprom_read (EEPROM_DATA_OFFSET_OVERLAY + idx * EEPROM_OVERLAY_ENTRY_SIZE, overlay_buffer8, EEPROM_OVERLAY_ENTRY_SIZE);

                bidx = 0;
                overlay.overlays[idx].type = overlay_buffer8[bidx];
                bidx += OVERLAY_TYPE_LEN;

                overlay.overlays[idx].interval      = overlay_buffer8[bidx];
                bidx += OVERLAY_INTERVAL_LEN;

                overlay.overlays[idx].duration      = overlay_buffer8[bidx];
                bidx += OVERLAY_DURATION_LEN;

                overlay.overlays[idx].date_code     = overlay_buffer8[bidx];
                bidx += OVERLAY_DATE_CODE_LEN;

                overlay.overlays[idx].date_start    = overlay_buffer8[bidx] | (overlay_buffer8[bidx + 1] << 8);
                bidx += OVERLAY_DATE_START_LEN;

                overlay.overlays[idx].days          = overlay_buffer8[bidx];
                bidx += OVERLAY_DAYS_LEN;

                memcpy (overlay.overlays[idx].text, overlay_buffer8 + bidx, OVERLAY_MAX_TEXT_LEN);
                overlay.overlays[idx].text[OVERLAY_MAX_TEXT_LEN] = '\0';
                bidx += OVERLAY_MAX_TEXT_LEN;

                overlay.overlays[idx].flags = overlay_buffer8[bidx];
                bidx += OVERLAY_FLAGS_LEN;
            }
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save number of overlays in EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
overlay_save_n_overlays (void)
{
    uint8_t n_overlays8 = overlay.n_overlays;

    eeprom_write (EEPROM_DATA_OFFSET_N_OVERLAYS,  &n_overlays8, EEPROM_DATA_SIZE_N_OVERLAYS);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save overlay in EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
overlay_save_overlay (uint_fast8_t idx)
{
    uint8_t         overlay_buffer8[EEPROM_OVERLAY_ENTRY_SIZE];
    uint_fast8_t    bidx;

    bidx = 0;
    overlay_buffer8[bidx] = overlay.overlays[idx].type;
    bidx += OVERLAY_TYPE_LEN;

    overlay_buffer8[bidx] = overlay.overlays[idx].interval;
    bidx += OVERLAY_INTERVAL_LEN;

    overlay_buffer8[bidx] = overlay.overlays[idx].duration;
    bidx += OVERLAY_DURATION_LEN;

    overlay_buffer8[bidx] = overlay.overlays[idx].date_code;
    bidx += OVERLAY_DATE_CODE_LEN;

    overlay_buffer8[bidx]       = overlay.overlays[idx].date_start & 0xFF;                          // lower byte
    overlay_buffer8[bidx + 1]   = (overlay.overlays[idx].date_start >> 8) & 0xFF;                   // upper byte
    bidx += OVERLAY_DATE_START_LEN;

    overlay_buffer8[bidx]       = overlay.overlays[idx].days;
    bidx += OVERLAY_DAYS_LEN;

    memcpy (overlay_buffer8 + bidx, overlay.overlays[idx].text, OVERLAY_MAX_TEXT_LEN);
    bidx += OVERLAY_MAX_TEXT_LEN;

    overlay_buffer8[bidx] = overlay.overlays[idx].flags;
    bidx += OVERLAY_FLAGS_LEN;

    eeprom_write (EEPROM_DATA_OFFSET_OVERLAY + idx * EEPROM_OVERLAY_ENTRY_SIZE, overlay_buffer8, EEPROM_OVERLAY_ENTRY_SIZE);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save all defined overlays in EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
overlay_save_overlays (void)
{
    uint_fast8_t    idx;

    for (idx = 0; idx < overlay.n_overlays; idx++)
    {
        overlay_save_overlay (idx);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write configuration to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
overlay_write_config_to_eeprom (void)
{
    uint_fast8_t            rtc = 0;

    if (eeprom_is_up)
    {
        overlay_save_n_overlays ();
        overlay_save_overlays ();

        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay type
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_type (uint_fast8_t idx, uint_fast8_t type)
{
    if (idx < overlay.n_overlays)
    {
        overlay.overlays[idx].type = type;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay interval
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_interval (uint_fast8_t idx, uint_fast8_t interval)
{
    if (idx < overlay.n_overlays)
    {
        overlay.overlays[idx].interval = interval;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay duration
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_duration (uint_fast8_t idx, uint_fast8_t duration)
{
    if (idx < overlay.n_overlays)
    {
        overlay.overlays[idx].duration = duration;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay date code
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_date_code (uint_fast8_t idx, uint_fast8_t date_code)
{
    if (idx < overlay.n_overlays)
    {
        overlay.overlays[idx].date_code = date_code;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay date start
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_date_start (uint_fast8_t idx, uint_fast16_t date_start)
{
    if (idx < overlay.n_overlays)
    {
        overlay.overlays[idx].date_start = date_start;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay days
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_days (uint_fast8_t idx, uint_fast8_t days)
{
    if (idx < overlay.n_overlays)
    {
        if (days < 1)
        {
            days = 1;
        }

        overlay.overlays[idx].days = days;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay text or name
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_text (uint_fast8_t idx, char * text)
{
    if (idx < overlay.n_overlays)
    {
        strncpy (overlay.overlays[idx].text, text, OVERLAY_MAX_TEXT_LEN);
        overlay.overlays[idx].text[OVERLAY_MAX_TEXT_LEN] = '\0';
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay flags and save it to EEPROM!
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_flags (uint_fast8_t idx, uint_fast8_t flags)
{
    if (idx < overlay.n_overlays)
    {
        overlay.overlays[idx].flags = flags;
        overlay_save_overlay (idx);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * caluclate overlay date end
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_calc_dates (uint_fast8_t idx, uint_fast16_t year)
{
    if (idx < overlay.n_overlays)
    {
        switch (overlay.overlays[idx].date_code)
        {
            case OVERLAY_DATE_CODE_NONE:
                break;
            case OVERLAY_DATE_CODE_CARNIVAL_MONDAY:
                overlay.overlays[idx].date_start = get_date_by_date_code (DATE_CODE_CARNIVAL_MONDAY, year);
                break;
            case OVERLAY_DATE_CODE_EASTER_SUNDAY:
                overlay.overlays[idx].date_start = get_date_by_date_code (DATE_CODE_EASTER_SUNDAY, year);
                break;
            case OVERLAY_DATE_CODE_ADVENT1:
                overlay.overlays[idx].date_start = get_date_by_date_code (DATE_CODE_ADVENT1, year);
                break;
            case OVERLAY_DATE_CODE_ADVENT2:
                overlay.overlays[idx].date_start = get_date_by_date_code (DATE_CODE_ADVENT2, year);
                break;
            case OVERLAY_DATE_CODE_ADVENT3:
                overlay.overlays[idx].date_start = get_date_by_date_code (DATE_CODE_ADVENT3, year);
                break;
            case OVERLAY_DATE_CODE_ADVENT4:
                overlay.overlays[idx].date_start = get_date_by_date_code (DATE_CODE_ADVENT4, year);
                break;
            default:
                log_printf ("overlay_calc_overlay_dates: invalid date_code: %d\r\n", overlay.overlays[idx].date_code);
                break;
        }

        overlay.overlays[idx].date_end = add_days (overlay.overlays[idx].date_start, year, overlay.overlays[idx].days - 1);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set overlay flags and save it to EEPROM!
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
overlay_set_n_overlays (uint_fast8_t n)
{
    overlay.n_overlays = n;
    overlay_save_n_overlays ();
}

void
overlay_init (void)
{
    overlay.n_overlays              = 0;                                        // default: no overlays
}
