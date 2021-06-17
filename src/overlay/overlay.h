/*-------------------------------------------------------------------------------------------------------------------------------------------
 * overlay.h - handle overlays
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef OVERLAY_H
#define OVERLAY_H

#define MAX_OVERLAYS                               32                   // maximum number of possible overlays

#define OVERLAY_TYPE_NONE                           0                   // overlay undefined
#define OVERLAY_TYPE_ICON                           1                   // icon
#define OVERLAY_TYPE_DATE                           2                   // date
#define OVERLAY_TYPE_TEMPERATURE                    3                   // temperature
#define OVERLAY_TYPE_WEATHER_ICON                   4                   // weather icon
#define OVERLAY_TYPE_WEATHER                        5                   // weather
#define OVERLAY_TYPE_TICKER                         6                   // ticker
#define OVERLAY_TYPE_MP3                            7                   // mp3
#define OVERLAY_TYPE_WEATHER_FC_ICON                8                   // weather forecast icon
#define OVERLAY_TYPE_WEATHER_FC                     9                   // weather forecast

#define OVERLAY_TYPE_LEN                            1                   // 1 byte
#define OVERLAY_INTERVAL_LEN                        1                   // 1 byte
#define OVERLAY_DURATION_LEN                        1                   // 1 byte
#define OVERLAY_DATE_CODE_LEN                       1                   // 1 byte
#define OVERLAY_DATE_START_LEN                      2                   // mm = 1st byte, dd = 2nd byte
#define OVERLAY_DAYS_LEN                            1                   // 1 byte
#define OVERLAY_MAX_TEXT_LEN                        32                  // name of icon or content of ticker
#define OVERLAY_FLAGS_LEN                           1                   // 1 byte

#define OVERLAY_ENTRY_SIZE                          (OVERLAY_TYPE_LEN + OVERLAY_INTERVAL_LEN + OVERLAY_DURATION_LEN + OVERLAY_DATE_CODE_LEN \
                                                     + OVERLAY_DATE_START_LEN + OVERLAY_DAYS_LEN + OVERLAY_MAX_TEXT_LEN + OVERLAY_FLAGS_LEN)
#define OVERLAY_ENTRIES_SIZE                        (MAX_OVERLAYS * OVERLAY_ENTRY_SIZE)

#define OVERLAY_DATE_CODE_NONE                      0
#define OVERLAY_DATE_CODE_CARNIVAL_MONDAY           1
#define OVERLAY_DATE_CODE_EASTER_SUNDAY             2
#define OVERLAY_DATE_CODE_ADVENT1                   3
#define OVERLAY_DATE_CODE_ADVENT2                   4
#define OVERLAY_DATE_CODE_ADVENT3                   5
#define OVERLAY_DATE_CODE_ADVENT4                   6

#define OVERLAY_FLAG_NONE                        0x00                   // no flag set
#define OVERLAY_FLAG_ACTIVE                      0x01                   // flag: overlay is active

typedef struct
{
    uint_fast8_t    type;                                               // type of overlay
    uint_fast8_t    interval;                                           // display interval
    uint_fast8_t    duration;                                           // display duration in seconds
    uint_fast8_t    date_code;                                          // date code: eastern, advent1 etc
    uint_fast16_t   date_start;                                         // display start date: mm = upper byte, dd = lower byte
    uint_fast8_t    days;                                               // number of days, if start date given
    char            text[OVERLAY_MAX_TEXT_LEN + 1];                     // value: name of icon or content of ticker
    uint_fast8_t    flags;                                              // flags
    // runtime values:
    uint_fast16_t   date_end;                                           // runtime: display end date: mm = upper byte, dd = lower byte
} OVERLAY;

typedef struct
{
    uint_fast8_t    n_overlays;
    OVERLAY         overlays[MAX_OVERLAYS];
} OVERLAY_GLOBALS;

extern OVERLAY_GLOBALS  overlay;

extern uint_fast8_t     overlay_read_config_from_eeprom (uint32_t);
extern uint_fast8_t     overlay_write_config_to_eeprom (void);

extern void             overlay_set_type (uint_fast8_t, uint_fast8_t);
extern void             overlay_set_interval (uint_fast8_t, uint_fast8_t);
extern void             overlay_set_duration (uint_fast8_t, uint_fast8_t);
extern void             overlay_set_date_code (uint_fast8_t, uint_fast8_t);
extern void             overlay_set_date_start (uint_fast8_t, uint_fast16_t);
extern void             overlay_set_days (uint_fast8_t, uint_fast8_t);
extern void             overlay_set_text (uint_fast8_t, char *);
extern void             overlay_set_flags (uint_fast8_t, uint_fast8_t);
extern void             overlay_calc_dates (uint_fast8_t, uint_fast16_t);
extern void             overlay_set_n_overlays (uint_fast8_t);
extern void             overlay_init (void);

#endif // OVERLAY_H
