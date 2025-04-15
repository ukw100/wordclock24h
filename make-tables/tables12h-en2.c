/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-en2.c - tables for english (variant 2) WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout:
 *
 * English Variant 2:
 *   ITKISGHALFE
 *   TENYQUARTER
 *   DTWENTYFIVE
 *   TOPASTEFOUR
 *   FIVETWONINE
 *   THREETWELVE
 *   BELEVENONES
 *   SEVENWEIGHT
 *   ITENSIXTIES
 *   TINEOICLOCK
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-en2.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "ENGLISH 2" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // ENGLISH2
    {                                                               // tbl_hours[4][] = hh:mm (not hh:00)
        {WP_IT_EN2, WP_IS_EN2, WP_TWELVE_EN2    },                  // 00:mm
        {WP_IT_EN2, WP_IS_EN2, WP_ONE_EN2       },                  // 01:mm
        {WP_IT_EN2, WP_IS_EN2, WP_TWO_EN2       },                  // 02:mm
        {WP_IT_EN2, WP_IS_EN2, WP_THREE_EN2     },                  // 03:mm
        {WP_IT_EN2, WP_IS_EN2, WP_FOUR_EN2      },                  // 04:mm
        {WP_IT_EN2, WP_IS_EN2, WP_FIVE_2_EN2    },                  // 05:mm
        {WP_IT_EN2, WP_IS_EN2, WP_SIX_EN2       },                  // 06:mm
        {WP_IT_EN2, WP_IS_EN2, WP_SEVEN_EN2     },                  // 07:mm
        {WP_IT_EN2, WP_IS_EN2, WP_EIGHT_EN2     },                  // 08:mm
        {WP_IT_EN2, WP_IS_EN2, WP_NINE_EN2      },                  // 09:mm
        {WP_IT_EN2, WP_IS_EN2, WP_TEN_2_EN2     },                  // 10:mm
        {WP_IT_EN2, WP_IS_EN2, WP_ELEVEN_EN2    }                   // 11:mm
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                               // tbl_minutes[7][] = ENGLISH var. 2
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_O_EN2, WP_CLOCK_EN2                      }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_FIVE_1_EN2,  WP_PAST_EN2                 }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TEN_1_EN2,   WP_PAST_EN2                 }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_QUARTER_EN2, WP_PAST_EN2                 }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TWENTY_EN2,  WP_PAST_EN2                 }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TWENTY_EN2,  WP_FIVE_1_EN2, WP_PAST_EN2  }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_HALF_EN2,    WP_PAST_EN2                 }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TWENTY_EN2,  WP_FIVE_1_EN2, WP_TO_EN2    }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TWENTY_EN2,  WP_TO_EN2                   }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_QUARTER_EN2, WP_TO_EN2                   }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TEN_1_EN2,   WP_TO_EN2                   }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FIVE_1_EN2,  WP_TO_EN2                   }},         // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        //  WP_END_OF_WORDS = ""

    {0,0,2 | ILLUMINATION_FLAG_IT_IS },                             //  WP_IT_EN2       = "IT"
    {0,3,2 | ILLUMINATION_FLAG_IT_IS },                             //  WP_IS_EN2       = "IS"
    {1,4,7},                                                        //  WP_QUARTER_EN2  = "QUARTER"
    {2,1,6},                                                        //  WP_TWENTY_EN2   = "TWENTY"
    {2,7,4},                                                        //  WP_FIVE_1_EN2   = "FIVE"
    {0,6,4},                                                        //  WP_HALF_EN2     = "HALF"
    {1,0,3},                                                        //  WP_TEN_EN2      = "TEN"
    {3,0,2},                                                        //  WP_TO_EN2       = "TO"
    {3,2,4},                                                        //  WP_PAST_EN2     = "PAST"
    {4,7,4},                                                        //  WP_NINE_EN2     = "NINE"
    {6,7,3},                                                        //  WP_ONE_EN2      = "ONE"
    {8,4,3},                                                        //  WP_SIX_EN2      = "SIX"
    {5,0,5},                                                        //  WP_THREE_EN2    = "THREE"
    {3,7,4},                                                        //  WP_FOUR_EN2     = "FOUR"
    {4,0,4},                                                        //  WP_FIVE_2_EN2   = "FIVE"
    {4,4,3},                                                        //  WP_TWO_EN2      = "TWO"
    {7,6,5},                                                        //  WP_EIGHT_EN2    = "EIGHT"
    {6,1,6},                                                        //  WP_ELEVEN_EN2   = "ELEVEN"
    {7,0,5},                                                        //  WP_SEVEN_EN2    = "SEVEN"
    {5,5,6},                                                        //  WP_TWELVE_EN2   = "TWELVE"
    {8,1,3},                                                        //  WP_TEN_2_EN2    = "TEN"
    {9,4,1},                                                        //  WP_O_EN2        = "O"
    {9,6,5},                                                        //  WP_CLOCK_EN2    = "CLOCK"

};

#if 0 // ENGLISH2
const char * display[WC_ROWS] =
{
    "ITKISGHALFE",
    "TENYQUARTER",
    "DTWENTYFIVE",
    "TOPASTEFOUR",
    "FIVETWONINE",
    "THREETWELVE",
    "BELEVENONES",
    "SEVENWEIGHT",
    "ITENSIXTIES",
    "TINEOICLOCK"
};
#endif // 0

#endif
