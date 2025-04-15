/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables-gen.c - generic data of WordClock tables
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define TABLES_VERSION_MAGIC    0xFF                    // Magic of version header
#define TABLES_VERSION          2                       // Version, used for header, e.g. FF 02

#define MAX_HOUR_WORDS          6                       // max number of words to display one hour, "e.g. five o'clock" (no end token)
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

#define ILLUMINATION_LEN_MASK   0x1F                    // 5 bits for length of word (0..31), rest used as flags
#define ILLUMINATION_FLAG_IT_IS 0x80                    // flag: word is member of "IT IS"
#define ILLUMINATION_FLAG_AM    0x40                    // flag: word is "AM"
#define ILLUMINATION_FLAG_PM    0x20                    // flag: word is "PM"

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
