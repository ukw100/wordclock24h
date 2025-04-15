/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-ch2.c - tables for swiss (variant 2) WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout:
 *
 * Swiss German Variant 2:
 *   ESKESCHAZÄÄ
 *   FÖIFCVIERTU
 *   ZWÄNZGSIVOR
 *   ABOHAUBIEGE
 *   EISZWÖISDRÜ
 *   VIERITFÖIFI
 *   SÄCHSISEBNI
 *   ACHTIENÜNIL
 *   ZÄNIERBRLFI
 *   ZWÖLFINAUHR
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H == 0
#include "tables12h-ch2.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { 0, 0, "SCHWEIZERDEUTSCH 2" },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS] =
{
    // SWISS2
    {                                                               // tbl_hours[2][] = hh:mm (not hh:00)
        {WP_ES_CH2, WP_ESCH_CH2, WP_ZWOELFI_CH2},                   // 00:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_EIS_CH2},                       // 01:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_ZWOEI_CH2},                     // 02:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_DRUE_CH2},                      // 03:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_VIERI_CH2},                     // 04:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_FOEIFI_CH2},                    // 05:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_SAECHSI_CH2},                   // 06:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_SEBNI_CH2},                     // 07:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_ACHTI_CH2},                     // 08:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_NUENI_CH2},                     // 09:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_ZAENI_CH2},                     // 10:mm
        {WP_ES_CH2, WP_ESCH_CH2, WP_ELFI_CH2}                       // 11:mm
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT] =
{
    {                                                               // tbl_minutes[5][] = SCHWEIZERDEUTSCH2
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                                            }},         // 00
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_FOIF_CH2,    WP_AB_CH2                    }},         // 05
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZAEAE_CH2,   WP_AB_CH2                    }},         // 10
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_VIERTU_CH2,  WP_AB_CH2                    }},         // 15
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZWAENZ_CH2,  WP_AB_CH2                    }},         // 20
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FOIF_CH2,    WP_VOR_CH2, WP_HAUBI_CH2     }},         // 25
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_HAUBI_CH2                                 }},         // 30
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FOIF_CH2,    WP_AB_CH2,  WP_HAUBI_CH2     }},         // 35
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_ZWAENZ_CH2,  WP_VOR_CH2                   }},         // 40
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_VIERTU_CH2,  WP_VOR_CH2                   }},         // 45
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_ZAEAE_CH2,   WP_VOR_CH2                   }},         // 50
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_1, {WP_FOIF_CH2,    WP_VOR_CH2                   }},         // 55
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT] =
{
    {0,0,0},                                                        // WP_END_OF_WORDS = ""

    {0,0,2 | ILLUMINATION_FLAG_IT_IS},                              // 49 = WP_ES_CH2       = "ES"
    {0,3,4 | ILLUMINATION_FLAG_IT_IS},                              // 50 = WP_ESCH_CH2     = "ESCH"
    {1,0,4},                                                        // 51 = WP_FOIF_CH2     = "FOIF"
    {1,5,6},                                                        // 52 = WP_VIERTU_CH2   = "VIERTU"
    {0,8,3},                                                        // 53 = WP_ZAEAE_CH2    = "ZÄÄ"
    {2,0,6},                                                        // 54 = WP_ZWAENZ_CH2   = "ZWÄNZ"
    {2,8,3},                                                        // 55 = WP_VOR_CH2      = "VOR"
    {3,0,2},                                                        // 56 = WP_AB_CH2       = "AB"
    {3,3,5},                                                        // 57 = WP_HAUBI_CH2    = "HAUBI"
    {4,0,3},                                                        // 58 = WP_EIS_CH2      = "EIS"
    {4,3,4},                                                        // 59 = WP_ZWOEI_CH2    = "ZWÖI"
    {4,8,3},                                                        // 60 = WP_DRUE_CH2     = "DRÜ"
    {5,0,5},                                                        // 61 = WP_VIERI_CH2    = "VIERI"
    {5,6,5},                                                        // 62 = WP_FOEIFI_CH2   = "FÖIFI"
    {6,0,6},                                                        // 63 = WP_SAECHSI_CH2  = "SÄCHSI"
    {6,6,5},                                                        // 64 = WP_SEBNI_CH2    = "SEBNI"
    {7,0,5},                                                        // 65 = WP_ACHTI_CH2    = "ACHTI"
    {7,6,4},                                                        // 66 = WP_NUENI_CH2    = "NÜNI"
    {8,0,4},                                                        // 67 = WP_ZAENI_CH2    = "ZÄNI"
    {8,7,4},                                                        // 68 = WP_ELFI_CH2     = "ELFI"
    {9,0,6},                                                        // 69 = WP_ZWOELFI_CH2  = "ZWÖLFI"
    {9,8,3},                                                        // 70 = WP_UHR_CH2      = "UHR"
};

#if 0 // SWISS2
const char * display[WC_ROWS] =
{
    "ESKESCHAZÄÄ",
    "FÖIFCVIERTU",
    "ZWÄNZGSIVOR",
    "ABOHAUBIEGE",
    "EISZWÖISDRÜ",
    "VIERITFÖIFI",
    "SÄCHSISEBNI",
    "ACHTIENÜNIL",
    "ZÄNIERBELFI",
    "ZWÖLFINAUHR"
};
#endif // 0

#endif
