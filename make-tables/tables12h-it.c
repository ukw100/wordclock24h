/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-it.c - tables for italian WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout:
 *
 * Italian Variant 1:
 *   SONORLEBORE
 *   ERLUNASDUEZ
 *   TREOTTONOVE
 *   DIECIUNDICI
 *   DODICISETTE
 *   QUATTROCSEI
 *   CINQUEAMENO
 *   EKUNLQUARTO
 *   VENTICINQUE
 *   DIECILMEZZA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-it.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "ITALIAN" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // ITALIAN
    {                                                               // tbl_hours[1][] = hh:mm (not hh:00)
        {WP_SONO_IT,  WP_LE_IT, WP_DODICI_IT},                      // 00:mm
        {WP_E_IT,     WP_L_IT,  WP_UNA_IT},                         // 01:mm
        {WP_SONO_IT,  WP_LE_IT, WP_DUE_IT},                         // 02:mm
        {WP_SONO_IT,  WP_LE_IT, WP_TRE_IT},                         // 03:mm
        {WP_SONO_IT,  WP_LE_IT, WP_QUATTRO_IT},                     // 04:mm
        {WP_SONO_IT,  WP_LE_IT, WP_CINQUE_IT},                      // 05:mm
        {WP_SONO_IT,  WP_LE_IT, WP_SEI_IT},                         // 06:mm
        {WP_SONO_IT,  WP_LE_IT, WP_SETTE_IT},                       // 07:mm
        {WP_SONO_IT,  WP_LE_IT, WP_OTTO_IT},                        // 08:mm
        {WP_SONO_IT,  WP_LE_IT, WP_NOVE_IT},                        // 09:mm
        {WP_SONO_IT,  WP_LE_IT, WP_DIECI_IT},                       // 10:mm
        {WP_SONO_IT,  WP_LE_IT, WP_UNDICI_IT}                       // 11:mm
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                                                           // tbl_minutes[4][] = ITALIAN
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                                        }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_E_1_IT,  WP_CINQUE_1_IT               }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_E_1_IT,  WP_DIECI_1_IT                }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_E_1_IT,  WP_UN_IT,       WP_QUARTO_IT }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_E_1_IT,  WP_VENTI_IT                  }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_E_1_IT,  WP_VENTICINQUE_IT            }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_E_1_IT,  WP_MEZZA_IT                  }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENO_IT, WP_VENTICINQUE_IT            }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENO_IT, WP_VENTI_IT                  }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENO_IT, WP_UN_IT,       WP_QUARTO_IT }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENO_IT, WP_DIECI_1_IT                }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_MENO_IT, WP_CINQUE_1_IT               }},         // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        //  0 = WP_END_OF_WORDS   = ""
    {0,0,4 | ILLUMINATION_FLAG_IT_IS},                              //  1 = WP_SONO_IT        = "SONO"
    {0,5,2 | ILLUMINATION_FLAG_IT_IS},                              //  2 = WP_LE_IT          = "LE"
    {1,0,1 | ILLUMINATION_FLAG_IT_IS},                              //  3 = WP_E_IT           = "E"
    {1,2,1 | ILLUMINATION_FLAG_IT_IS},                              //  4 = WP_L_IT           = "L"
    {1,3,3},                                                        //  5 = WP_UNA_IT         = "UNA"
    {1,7,3},                                                        //  6 = WP_DUE_IT         = "DUE"
    {2,0,3},                                                        //  7 = WP_TRE_IT         = "TRE"
    {2,3,4},                                                        //  8 = WP_OTTO_IT        = "OTTO"
    {2,7,4},                                                        //  9 = WP_NOVE_IT        = "NOVE"
    {3,0,5},                                                        // 10 = WP_DIECI_IT       = "DIECI"
    {3,5,6},                                                        // 11 = WP_UNDICI_IT      = "UNDICI"
    {4,0,6},                                                        // 12 = WP_DODICI_IT      = "DODICI"
    {4,6,5},                                                        // 13 = WP_SETTE_IT       = "SETTE"
    {5,0,7},                                                        // 14 = WP_QUATTRO_IT     = "QUATTRO"
    {5,8,3},                                                        // 15 = WP_SEI_IT         = "SEI"
    {6,0,6},                                                        // 16 = WP_CINQUE_IT      = "CINQUE"
    {6,7,4},                                                        // 17 = WP_MENO_IT        = "MENO"
    {7,0,1},                                                        // 18 = WP_E_1_IT         = "E"
    {7,2,2},                                                        // 19 = WP_UN_IT          = "UN"
    {7,5,6},                                                        // 20 = WP_QUARTO_IT      = "QUARTO"
    {8,0,5},                                                        // 21 = WP_VENTI_IT       = "VENTI"
    {8,0,11},                                                       // 22 = WP_VENTICINQUE_IT = "VENTICINQUE"
    {8,5,6},                                                        // 23 = WP_CINQUE_1_IT    = "CINQUE"
    {9,0,5},                                                        // 24 = WP_DIECI_1_IT     = "DIECI"
    {9,6,5},                                                        // 25 = WP_MEZZA_IT       = "MEZZA"
    {0,8,3}                                                         // 26 = WP_ORE_IT         = "ORE"
};

#if 0 // ITALIAN
const char * display[WC_ROWS] =
{
    "SONORLEBORE",
    "ERLUNASDUEZ",
    "TREOTTONOVE",
    "DIECIUNDICI",
    "DODICISETTE",
    "QUATTROCSEI",
    "CINQUEAMENO",
    "EKUNLQUARTO",
    "VENTICINQUE",
    "DIECILMEZZA"
};
#endif // 0

#endif
