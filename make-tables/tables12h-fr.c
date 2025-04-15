/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-fr.c - tables for french WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout:
 *
 * French:
 *   ILNESTODEUX
 *   QUATRETROIS
 *   NEUFUNESEPT
 *   HUITSIXCINQ
 *   MIDIXMINUIT
 *   ONZERHEURES
 *   MOINSOLEDIX
 *   ETRQUARTPMD
 *   VINGT-CINQU
 *   ETSDEMIEPAM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-fr.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "FRANCAIS" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // FRENCH
    {                                                               // tbl_hours[6][] = hh:mm (AM)
        {WP_IL, WP_EST, WP_IF_HOUR_IS_0, WP_MINUIT, WP_MIDI},                      // 00:00 or 12:00
        {WP_IL, WP_EST, WP_UNE, WP_HEURE},                                         // 01:00
        {WP_IL, WP_EST, WP_DEUX, WP_HEURES},                                       // 02:00
        {WP_IL, WP_EST, WP_TROIS, WP_HEURES},                                      // 03:00
        {WP_IL, WP_EST, WP_QUADRE, WP_HEURES},                                     // 04:00
        {WP_IL, WP_EST, WP_CINQ_1, WP_HEURES},                                     // 05:00
        {WP_IL, WP_EST, WP_SIX, WP_HEURES},                                        // 06:00
        {WP_IL, WP_EST, WP_SEPT, WP_HEURES},                                       // 07:00
        {WP_IL, WP_EST, WP_HUIT, WP_HEURES},                                       // 08:00
        {WP_IL, WP_EST, WP_NEUF, WP_HEURES},                                       // 09:00
        {WP_IL, WP_EST, WP_DIX_1, WP_HEURES},                                      // 10:00
        {WP_IL, WP_EST, WP_ONZE, WP_HEURES},                                       // 11:00
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                               // tbl_minutes[9][] = FRENCH
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                         }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CINQ_2                 }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DIX_2                  }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ET_1, WP_QUART         }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_VINGT                  }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_VINGTCINQ              }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ET_2, WP_DEMIE         }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MOINS, WP_VINGTCINQ    }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MOINS, WP_VINGT        }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MOINS, WP_LE, WP_QUART }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MOINS, WP_DIX_2        }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MOINS, WP_CINQ_2       }},         // 55
    }
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        // WP_END_OF_WORDS = ""

    {0,0,2 | ILLUMINATION_FLAG_IT_IS },                             // WP_IL           = "IL"
    {0,3,3 | ILLUMINATION_FLAG_IT_IS },                             // WP_EST          = "EST"
    {0,7,4},                                                        // WP_DEUX         = "DEUX"
    {1,0,6},                                                        // WP_QUADRE       = "QUADRE"
    {1,6,5},                                                        // WP_TROIS        = "TROIS"
    {2,0,4},                                                        // WP_NEUF         = "NEUF"
    {2,4,3},                                                        // WP_UNE          = "UNE"
    {2,7,4},                                                        // WP_SEPT         = "SEPT"
    {3,0,4},                                                        // WP_HUIT         = "HUIT"
    {3,4,3},                                                        // WP_SIX          = "SIX"
    {3,7,4},                                                        // WP_CINQ_1       = "CINQ"
    {4,0,4},                                                        // WP_MIDI         = "MIDI"
    {4,2,3},                                                        // WP_DIX_1        = "DIX"
    {4,5,6},                                                        // WP_MINUIT       = "MINUIT"
    {5,0,4},                                                        // WP_ONZE         = "ONZE"
    {5,5,5},                                                        // WP_HEURE        = "HEURE"
    {5,5,6},                                                        // WP_HEURES       = "HEURES"
    {6,0,5},                                                        // WP_MOINS        = "MOINS"
    {6,6,2},                                                        // WP_LE           = "LE"
    {6,8,3},                                                        // WP_DIX_2        = "DIX"
    {7,0,2},                                                        // WP_ET_1         = "ET"
    {7,3,5},                                                        // WP_QUART        = "QUART"
    {8,0,5},                                                        // WP_VINGT        = "VINGT"
    {8,0,10},                                                       // WP_VINGTCINQ    = "VINGT-CINQ"
    {8,6,4},                                                        // WP_CINQ_2       = "CINQ"
    {9,0,2},                                                        // WP_ET_2         = "ET"
    {9,3,5},                                                        // WP_DEMIE        = "DEMIE"
};

#if 0 // FRENCH:
const char * display[WC_ROWS] =
{
    "ILNESTODEUX",
    "QUATRETROIS",
    "NEUFUNESEPT",
    "HUITSIXCINQ",
    "MIDIXMINUIT",
    "ONZERHEURES",
    "MOINSOLEDIX",
    "ETRQUARTPMD",
    "VINGT-CINQU",
    "ETSDEMIEPAM"
};
#endif // 0

#endif
