/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-es.c - swedisch table for WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layouts
 *
 * Spanish:
 *   ESONELASUNA
 *   DOSITRESOAM
 *   CUATROCINCO
 *   SEISASIETEN
 *   OCHONUEVEPM
 *   LADIEZSONCE
 *   DOCELYMENOS
 *   OVEINTEDIEZ
 *   VEINTICINCO
 *   MEDIACUARTO
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-es.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "SPANISH" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // SPANISH
    {                                                               // tbl_hours[0][] = hh:mm
        {WP_SON, WP_LAS, WP_DOCE},                                                  // 00:00
        {WP_ES,  WP_LA,  WP_UNA},                                                   // 01:00
        {WP_SON, WP_LAS, WP_DOS},                                                   // 02:00
        {WP_SON, WP_LAS, WP_TRES},                                                  // 03:00
        {WP_SON, WP_LAS, WP_CUATRO},                                                // 04:00
        {WP_SON, WP_LAS, WP_CINCO},                                                 // 05:00
        {WP_SON, WP_LAS, WP_SEIS},                                                  // 06:00
        {WP_SON, WP_LAS, WP_SIETE},                                                 // 07:00
        {WP_SON, WP_LAS, WP_OCHO},                                                  // 08:00
        {WP_SON, WP_LAS, WP_NUEVE},                                                 // 09:00
        {WP_SON, WP_LAS, WP_DIEZ},                                                  // 10:00
        {WP_SON, WP_LAS, WP_ONCE}                                                   // 11:00
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                               // tbl_minutes[0][] = SPANISH
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                          }},           // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_Y, WP_CINCO_2           }},           // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_Y, WP_DIEZ_2            }},           // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_Y, WP_CUARTO            }},           // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_Y, WP_VEINTE            }},           // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_Y, WP_VEINTICINCO       }},           // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_Y, WP_MEDIA             }},           // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENOS, WP_VEINTICINCO   }},           // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENOS, WP_VEINTE        }},           // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENOS, WP_CUARTO        }},           // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENOS, WP_DIEZ_2        }},           // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENOS, WP_CINCO_2       }},           // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        // WP_END_OF_WORDS = ""
    {0,0,2 | ILLUMINATION_FLAG_IT_IS},                              // WP_ES
    {0,1,3 | ILLUMINATION_FLAG_IT_IS},                              // WP_SON
    {0,5,2 | ILLUMINATION_FLAG_IT_IS},                              // WP_LA
    {0,5,3 | ILLUMINATION_FLAG_IT_IS},                              // WP_LAS
    {0,8,3},                                                        // WP_UNA
    {1,0,3},                                                        // WP_DOS
    {1,4,4},                                                        // WP_TRES
    {2,0,6},                                                        // WP_CUATRO
    {2,6,5},                                                        // WP_CINCO
    {3,0,4},                                                        // WP_SEIS
    {3,5,5},                                                        // WP_SIETE
    {4,0,4},                                                        // WP_OCHO
    {4,4,5},                                                        // WP_NUEVE
    {5,2,4},                                                        // WP_DIEZ
    {5,7,4},                                                        // WP_ONCE
    {6,0,4},                                                        // WP_DOCE
    {6,5,1},                                                        // WP_Y
    {6,6,5},                                                        // WP_MENOS
    {7,1,6},                                                        // WP_VEINTE
    {7,7,4},                                                        // WP_DIEZ_2
    {8,0,11},                                                       // WP_VEINTICINCO
    {8,6,5},                                                        // WP_CINCO_2
    {9,0,5},                                                        // WP_MEDIA
    {9,5,6},                                                        // WP_CUARTO
};

#if 0 // SPANISH
const char * display[WC_ROWS] =
{
    "ESONELASUNA",
    "DOSITRESOAM",
    "CUATROCINCO",
    "SEISASIETEN",
    "OCHONUEVEPM",
    "LADIEZSONCE",
    "DOCELYMENOS",
    "OVEINTEDIEZ",
    "VEINTICINCO",
    "MEDIACUARTO"
};
#endif // 0

#endif
