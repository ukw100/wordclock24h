/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables.h - wordclock layout tables
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef TABLES_H
#define TABLES_H

#include "wclock24h-config.h"

#if WCLOCK24H == 1
#define HOUR_COUNT              24                      // 24 hours per day
#define MINUTE_COUNT            60                      // 60 minutes per hour
#else
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_COUNT            12                      // 12 minutes per hour
#endif

#define WP_COUNT                128                     // max. number of words on display

#define MAX_HOUR_WORDS          4                       // max number of words to display one hour, "e.g. five o'clock" (no end token)
#define MAX_MINUTE_WORDS        7                       // how many words for minute display (no end token)

#define WP_IF_HOUR_IS_0         0xFE                    // alternate word in hours_table if hour is 0
#define WP_IF_MINUTE_IS_0       0xFF                    // alternate word in hours_table if minute is 0

// possible values in MINUTEDISPLAY.flags:

#define MDF_IT_IS_0             0x00                    // no IT IS
#define MDF_IT_IS_1             0x01                    // IT IS

#define MDF_HOUR_OFFSET_0       0x00                    // no offset
#define MDF_HOUR_OFFSET_1       0x02                    // offset of 1 hour
#define MDF_HOUR_OFFSET_2       0x04                    // offset of 2 hours

typedef struct
{
    uint8_t flags;
    uint8_t word_idx[MAX_MINUTE_WORDS];
} MINUTEDISPLAY;

typedef struct
{
    uint8_t row;
    uint8_t col;
    uint8_t len;
} WORD_ILLUMINATION;

typedef struct
{
    uint_fast8_t            modes_count;
    uint_fast8_t            hour_modes_count;
    uint_fast8_t            hour_count;
    uint_fast8_t            max_hour_words;
    uint_fast8_t            minute_modes_count;
    uint_fast8_t            minute_count;
    uint_fast8_t            max_minute_words;

    uint8_t                 it_is[2];
    uint8_t                 hours[HOUR_COUNT][MAX_HOUR_WORDS];
    MINUTEDISPLAY           minutes[MINUTE_COUNT];
#if WCLOCK24H == 1
    MINUTEDISPLAY           temperature[MINUTE_COUNT];
#endif
    WORD_ILLUMINATION       illumination[WP_COUNT];
    uint_fast8_t            complete;
} TABLES_GLOBALS;

extern TABLES_GLOBALS       tables;


extern void                 tables_init (void);
extern void                 tables_get (uint_fast8_t);
extern void                 tables_tabinfo (char *);
extern void                 tables_tabillu (char *);
#if WCLOCK24H == 1
extern void                 tables_tabt (char *);
#endif
extern void                 tables_tabh (char *);
extern void                 tables_tabm (char *);
extern uint_fast8_t         tables_fill_words (uint8_t *, uint_fast8_t, uint_fast8_t, uint_fast8_t);

#endif // TABLES_H
