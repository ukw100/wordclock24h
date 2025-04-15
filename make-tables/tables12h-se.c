/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-se.c - swedisch table for WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layouts
 *
 * Swedish:
 *   KLOCKANTÄRK
 *   FEMYISTIONI
 *   KVARTQIENZO
 *   TJUGOLIVIPM
 *   ÖVERKAMHALV
 *   ETTUSVLXTVA
 *   TREMYKYFYRA
 *   FEMSFLORSEX
 *   SJUÄTTAINIO
 *   TIOELVATOLV
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-se.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "SWEDISH" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // SWEDISH
    {                                                               // tbl_hours[5][] = hh:mm
        {WP_KLOCKAN, WP_AER, WP_TOLV},                                             // 00:00
        {WP_KLOCKAN, WP_AER, WP_ETT},                                              // 01:00
        {WP_KLOCKAN, WP_AER, WP_TVA},                                              // 02:00
        {WP_KLOCKAN, WP_AER, WP_TRE},                                              // 03:00
        {WP_KLOCKAN, WP_AER, WP_FYRA},                                             // 04:00
        {WP_KLOCKAN, WP_AER, WP_FEM_2},                                            // 05:00
        {WP_KLOCKAN, WP_AER, WP_SEX},                                              // 06:00
        {WP_KLOCKAN, WP_AER, WP_SJU},                                              // 07:00
        {WP_KLOCKAN, WP_AER, WP_ATTA},                                             // 08:00
        {WP_KLOCKAN, WP_AER, WP_NIO},                                              // 09:00
        {WP_KLOCKAN, WP_AER, WP_TIO_2},                                            // 10:00
        {WP_KLOCKAN, WP_AER, WP_ELVA}                                              // 11:00
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                               // tbl_minutes[8][] = SWEDISH
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                           }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_FEM_1, WP_OEVER          }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TIO_1, WP_OEVER          }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_KVART, WP_OEVER          }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TJUGO, WP_OEVER          }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FEM_1, WP_FEM_I, WP_HALV }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_HALV                     }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FEM_1, WP_FEM_I, WP_HALV }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TJUGO, WP_TJUGO_I        }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_KVART, WP_KVART_I        }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_TIO_1, WP_TIO_I          }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FEM_1, WP_FEM_I          }},         // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,  0, 0 },                                                        //  0 = WP_END_OF_WORDS = ""
    {0,  0, 7 | ILLUMINATION_FLAG_IT_IS },                              //  1 = WP_KLOCKAN      = "KLOCKAN"
    {0,  8, 2 | ILLUMINATION_FLAG_IT_IS },                              //  2 = WP_AER          = "ÄR"
    {1,  0, 3 },                                                        //  3 = WP_FEM_1        = "FEM"
    {1,  4, 1 },                                                        //  4 = WP_FEM_I        = "I"
    {1,  6, 3 },                                                        //  5 = WP_TIO_1        = "TIO"
    {1, 10, 1 },                                                        //  6 = WP_TIO_I        = "I"
    {2,  0, 5 },                                                        //  7 = WP_KVART        = "KVART"
    {2,  6, 1 },                                                        //  8 = WP_KVART_I      = "I"
    {3,  0, 5 },                                                        //  9 = WP_TJUGO        = "TJUGO"
    {3,  6, 1 },                                                        // 10 = WP_TJUGO_I      = "I"
    {4,  0, 4 },                                                        // 11 = WP_OEVER        = "ÖVER"
    {4,  7, 4 },                                                        // 12 = WP_HALV         = "HALV"
    {5,  0, 3 },                                                        // 13 = WP_ETT          = "ETT"
    {5,  8, 3 },                                                        // 14 = WP_TVA          = "TVA"
    {6,  0, 3 },                                                        // 15 = WP_TRE          = "TRE"
    {6,  7, 4 },                                                        // 16 = WP_FYRA         = "FYRA"
    {7,  0, 3 },                                                        // 17 = WP_FEM_2        = "FEM"
    {7,  8, 3 },                                                        // 18 = WP_SEX          = "SEX"
    {8,  0, 3 },                                                        // 19 = WP_SJU          = "SJU"
    {8,  3, 4 },                                                        // 20 = WP_ATTA         = "ATTA"
    {8,  8, 3 },                                                        // 21 = WP_NIO          = "NIO"
    {9,  0, 3 },                                                        // 22 = WP_TIO_2        = "TIO"
    {9,  3, 4 },                                                        // 23 = WP_ELVA         = "ELVA"
    {9,  7, 4 },                                                        // 24 = WP_TOLV         = "TOLV"
};

#if 0 // SWEDISH
const char * display[WC_ROWS] =
{
    "KLOCKANTÄRK",
    "FEMYISTIONI",
    "KVARTQIENZO",
    "TJUGOLIVIPM",
    "ÖVERKAMHALV",
    "ETTUSVLXTVA",
    "TREMYKYFYRA",
    "FEMSFLORSEX",
    "SJUÄTTAINIO",
    "TIOELVATOLV"
};
#endif // 0

#endif
