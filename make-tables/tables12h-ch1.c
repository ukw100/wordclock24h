/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-ch1.c - tables for swiss (variant 1) WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout:
 *
 * Swiss German Variant 1:
 *   ESKISCHAFÜF
 *   VIERTUBFZÄÄ
 *   ZWÄNZGSIVOR
 *   ABOHAUBIEPM
 *   EISZWÖISDRÜ
 *   VIERIFÜFIQT
 *   SÄCHSISIBNI
 *   ACHTINÜNIEL
 *   ZÄNIERBEUFI
 *   ZWÖUFIAMUHR
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-ch1.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "SCHWEIZERDEUTSCH 1" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // SWISS1
    {                                                               // tbl_hours[1][] = hh:mm (not hh:00)
        {WP_ES_CH, WP_ISCH_CH, WP_ZWOEUFI_CH},                      // 00:mm
        {WP_ES_CH, WP_ISCH_CH, WP_EIS_CH},                          // 01:mm
        {WP_ES_CH, WP_ISCH_CH, WP_ZWOEI_CH},                        // 02:mm
        {WP_ES_CH, WP_ISCH_CH, WP_DRUE_CH},                         // 03:mm
        {WP_ES_CH, WP_ISCH_CH, WP_VIERI_CH},                        // 04:mm
        {WP_ES_CH, WP_ISCH_CH, WP_FUEFI_CH},                        // 05:mm
        {WP_ES_CH, WP_ISCH_CH, WP_SAECHSI_CH},                      // 06:mm
        {WP_ES_CH, WP_ISCH_CH, WP_SIBNI_CH},                        // 07:mm
        {WP_ES_CH, WP_ISCH_CH, WP_ACHTI_CH},                        // 08:mm
        {WP_ES_CH, WP_ISCH_CH, WP_NUENI_CH},                        // 09:mm
        {WP_ES_CH, WP_ISCH_CH, WP_ZAENI_CH},                        // 10:mm
        {WP_ES_CH, WP_ISCH_CH, WP_EUFI_CH}                          // 11:mm
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                                                           // tbl_minutes[4][] = SCHWEIZERDEUTSCH1
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                                        }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_FUEF_CH,    WP_AB_CH                  }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZAEAE_CH,   WP_AB_CH                  }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_VIERTU_CH,  WP_AB_CH                  }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZWAENZG_CH, WP_AB_CH                  }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FUEF_CH,    WP_VOR_CH, WP_HAUBI_CH    }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_HAUBI_CH                              }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FUEF_CH,    WP_AB_CH,  WP_HAUBI_CH    }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_ZWAENZG_CH, WP_VOR_CH                 }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_VIERTU_CH,  WP_VOR_CH                 }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_ZAEAE_CH,   WP_VOR_CH                 }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FUEF_CH,    WP_VOR_CH                 }},         // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        //  0 = WP_END_OF_WORDS = ""
    {0,0,2 | ILLUMINATION_FLAG_IT_IS},                              // 27 = WP_ES_CH        = "ES"
    {0,3,4 | ILLUMINATION_FLAG_IT_IS},                              // 28 = WP_ISCH_CH      = "ISCH"
    {0,8,3},                                                        // 29 = WP_FUEF_CH      = "FÜF"
    {1,0,6},                                                        // 30 = WP_VIERTU_CH    = "VIERTU"
    {1,8,3},                                                        // 31 = WP_ZAEAE_CH     = "ZÄÄ"
    {2,0,6},                                                        // 32 = WP_ZWAENZG_CH   = "ZWÄNZG"
    {2,8,3},                                                        // 33 = WP_VOR_CH       = "VOR"
    {3,0,2},                                                        // 34 = WP_AB_CH        = "AB"
    {3,3,5},                                                        // 35 = WP_HAUBI_CH     = "HAUBI"
    {4,0,3},                                                        // 36 = WP_EIS_CH       = "EIS"
    {4,3,4},                                                        // 37 = WP_ZWOEI_CH     = "ZWÖI"
    {4,8,3},                                                        // 38 = WP_DRUE_CH      = "DRÜ"
    {5,0,5},                                                        // 39 = WP_VIERI_CH     = "VIERI"
    {5,5,4},                                                        // 40 = WP_FUEFI_CH     = "FÜFI"
    {6,0,6},                                                        // 41 = WP_SAECHSI_CH   = "SÄCHSI"
    {6,6,5},                                                        // 42 = WP_SIBNI_CH     = "SIBNI"
    {7,0,5},                                                        // 43 = WP_ACHTI_CH     = "ACHTI"
    {7,5,4},                                                        // 44 = WP_NUENI_CH     = "NÜNI"
    {8,0,4},                                                        // 45 = WP_ZAENI_CH     = "ZÄNI"
    {8,7,4},                                                        // 46 = WP_EUFI_CH      = "EUFI"
    {9,0,6},                                                        // 47 = WP_ZWOEFI_CH    = "ZWÖUFI"
    {9,8,3},                                                        // 48 = WP_UHR_CH       = "UHR"

};

#if 0 // SWISS1
const char * display[WC_ROWS] =
{
    "ESKISCHAFÜF",
    "VIERTUBFZÄÄ",
    "ZWÄNZGSIVOR",
    "ABOHAUBIEPM",
    "EISZWÖISDRÜ",
    "VIERIFÜFIQT",
    "SÄCHSISIBNI",
    "ACHTINÜNIEL",
    "ZÄNIERBEUFI",
    "ZWÖUFIAMUHR"
};
#endif // 0

#endif
