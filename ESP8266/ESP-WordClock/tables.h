/*----------------------------------------------------------------------------------------------------------------------------------------
 * tables.cpp - layout table functions
 *
 * Copyright (c) 2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#define MAX_HOUR_WORDS          4                       // max number of words to display one hour, "e.g. five o'clock" (no end token)
#define MAX_MINUTE_WORDS        7                       // how many words for minute display (no end token)

#define DISPLAY_MODE_TEXT_LEN   32

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
    uint8_t hour_idx;
    uint8_t minute_idx;
    char    description[DISPLAY_MODE_TEXT_LEN + 1];
} DISPLAY_MODES;

extern uint_fast8_t     display_modes_count;
extern DISPLAY_MODES *  tbl_modes;
extern int              tables_corrupt;

extern int              tables_init (void);
extern char *           tables_fname (void);
extern uint_fast8_t     tables_info (void);
extern uint_fast8_t     tables_illumination (uint_fast8_t);
extern uint_fast8_t     tables_hours (uint_fast8_t, uint_fast8_t);
extern uint_fast8_t     tables_minutes (uint_fast8_t, uint_fast8_t, const char *);

