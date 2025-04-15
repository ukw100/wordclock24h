/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-en1.c - tables for english (variant 1) WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Supported Layouts:
 *
 * English Variant 1:
 *   ITLISASAMPM
 *   ACQUARTERDC
 *   TWENTYFIVEX
 *   HALFSTENFTO
 *   PASTERUNINE
 *   ONESIXTHREE
 *   FOURFIVETWO
 *   EIGHTELEVEN
 *   SEVENTWELVE
 *   TENSEOCLOCK
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-en1.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "ENGLISH 1" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // ENGLISH1
    {                                                               // tbl_hours[3][] = hh:mm (not hh:00)
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_TWELVE_EN},           // 00:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_ONE_EN},              // 01:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_TWO_EN},              // 02:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_THREE_EN},            // 03:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_FOUR_EN},             // 04:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_FIVE_2_EN},           // 05:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_SIX_EN},              // 06:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_SEVEN_EN},            // 07:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_EIGHT_EN},            // 08:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_NINE_EN},             // 09:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_TEN_2_EN},            // 10:mm
        {WP_IT_EN, WP_IS_EN, WP_AM_EN, WP_PM_EN, WP_ELEVEN_EN}            // 11:mm
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                               // tbl_minutes[6][] = ENGLISH var. 1
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_OCLOCK_EN                                }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_FIVE_1_EN,  WP_PAST_EN                   }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TEN_1_EN,   WP_PAST_EN                   }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_QUARTER_EN, WP_PAST_EN                   }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TWENTY_EN,  WP_PAST_EN                   }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TWENTY_EN,  WP_FIVE_1_EN, WP_PAST_EN     }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_HALF_EN,    WP_PAST_EN                   }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TWENTY_EN,  WP_FIVE_1_EN, WP_TO_EN       }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TWENTY_EN,  WP_TO_EN                     }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_QUARTER_EN, WP_TO_EN                     }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TEN_1_EN,   WP_TO_EN                     }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FIVE_1_EN,  WP_TO_EN                     }},         // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        // WP_END_OF_WORDS = ""

    {0,0,2 | ILLUMINATION_FLAG_IT_IS},                              // WP_IT_EN        = "IT"
    {0,3,2 | ILLUMINATION_FLAG_IT_IS},                              // WP_IS_EN        = "IS"
    {0,7,2 | ILLUMINATION_FLAG_AM},                                 // WP_AM_EN        = "AM"
    {0,9,2 | ILLUMINATION_FLAG_PM},                                 // WP_PM_EN        = "PM"
    {1,2,7},                                                        // WP_QUARTER_EN   = "QUARTER"
    {2,0,6},                                                        // WP_TWENTY_EN    = "TWENTY"
    {2,6,4},                                                        // WP_FIVE_1_EN    = "FIVE"
    {3,0,4},                                                        // WP_HALF_EN      = "HALF"
    {3,5,3},                                                        // WP_TEN_EN       = "TEN"
    {3,9,2},                                                        // WP_TO_EN        = "TO"
    {4,0,4},                                                        // WP_PAST_EN      = "PAST"
    {4,7,4},                                                        // WP_NINE_EN      = "NINE"
    {5,0,3},                                                        // WP_ONE_EN       = "ONE"
    {5,3,3},                                                        // WP_SIX_EN       = "SIX"
    {5,6,5},                                                        // WP_THREE_EN     = "THREE"
    {6,0,4},                                                        // WP_FOUR_EN      = "FOUR"
    {6,4,4},                                                        // WP_FIVE_2_EN    = "FIVE"
    {6,8,3},                                                        // WP_TWO_EN       = "TWO"
    {7,0,5},                                                        // WP_EIGHT_EN     = "EIGHT"
    {7,5,6},                                                        // WP_ELEVEN_EN    = "ELEVEN"
    {8,0,5},                                                        // WP_SEVEN_EN     = "SEVEN"
    {8,5,6},                                                        // WP_TWELVE_EN    = "TWELVE"
    {9,0,3},                                                        // WP_TEN_2_EN     = "TEN"
    {9,5,6},                                                        // WP_CLOCK_EN     = "OCLOCK"

};

#if 0 // ENGLISH1
const char * display[WC_ROWS] =
{
    "ITLISASAMPM",
    "ACQUARTERDC",
    "TWENTYFIVEX",
    "HALFSTENFTO",
    "PASTERUNINE",
    "ONESIXTHREE",
    "FOURFIVETWO",
    "EIGHTELEVEN",
    "SEVENTWELVE",
    "TENSEOCLOCK"
};
#endif // 0

#endif
