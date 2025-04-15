/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h.h - WC24H tables
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H_DE == 1

#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

#define WC_ROWS                 16                              // 16 rows
#define WC_COLUMNS              18                              // 18 columns

#define DISPLAY_MODES_COUNT     27                              // count of different display modes
#define HOUR_COUNT              24                              // 24 plus one to distinguish between before and after full hour
#define MINUTE_COUNT            60

enum HourMode
{
    HM_0,                                                       // 0 = Mode 0: "leer, fuer Temperaturanzeige"
    HM_1,                                                       // 1 = Mode 1: "MM - HH (12)"
    HM_2,                                                       // 2 = Mode 3: "MM - HH UHR (12) NACHTS"
    HM_3,                                                       // 3 = Mode 4: "MM - HH UHR (24)"
    HM_4,                                                       // 4 = Mode 5: "HH UHR (12) - MM"
    HM_5,                                                       // 5 = Mode 6: "HH UHR 24) - MM"
    HM_6,                                                       // 6 = Mode 7: "MITTERNACHT (0 UHR)"
    HOUR_MODES_COUNT                                            // Number of HourModes
};

enum MinuteMode
{
    MM_1,                                                       //  0 = Mode  1: "MM NACH"
    MM_2,                                                       //  1 = Mode  2: "MM MINUTEN NACH"
    MM_3,                                                       //  2 = Mode  3: "OSSI - MM MINUTEN NACH (VIERTEL NACH, HALB, VIERTEL VOR)"
    MM_4,                                                       //  3 = Mode  4: "OESI - MINUTEN NACH (VIERTEL NACH, HALB, DREIVIERTEL)"
    MM_5,                                                       //  4 = Mode  5: "RHEIN/ RUHR - MINUTEN NACH (VIERTEL, HALB, DREIVIERTEL)"
    MM_6,                                                       //  5 = Mode  6: "SCHWABEN - MM MINUTEN NACH (VIERTEL NACH, HALB, DREIVIERTEL)"
    MM_7,                                                       //  6 = Mode  7: "WESSI - MM MINUTEN NACH (VIERTEL, HALB, DREIVIERTEL)"
    MM_8,                                                       //  7 = Mode  8: "TIROL - VIERTEL NACH, aber DREIVIERTEL"
    MM_9,                                                       //  8 = Mode  9: "MM"
    MM_10,                                                      //  9 = Mode 10: "UND MM MINUTEN"
    MM_11,                                                      // 10 = Mode 11: "MM MINUTEN VOR"
    MM_12,                                                      // 11 = Mode 12: "JESTER"
    MM_13,                                                      // 11 = Mode 13: "OSSI - MM NACH (VIERTEL NACH, HALB, VIERTEL VOR)"
    MM_14,                                                      // 11 = Mode 14: "OESI - NACH (VIERTEL NACH, HALB, DREIVIERTEL)"
    MM_15,                                                      // 11 = Mode 15: "RHEIN/ RUHR - MM NACH (VIERTEL, HALB, DREIVIERTEL)"
    MM_16,                                                      // 11 = Mode 16: "SCHWABEN - MM NACH (VIERTEL NACH, HALB, DREIVIERTEL)"
    MM_17,                                                      // 11 = Mode 17: "WESSI - MM NACH (VIERTEL, HALB, DREIVIERTEL)"
    MM_18,                                                      // 11 = Mode 18: "TIROL - MM NACH (VIERTEL NACH, aber DREIVIERTEL)"
    MM_19,                                                      // 12 = Mode 19: "TEMPERATUR CC GRAD"
    MINUTE_MODES_COUNT                                          // Number of MinuteModes
};

enum WordPos
{
    WP_END_OF_WORDS,                                            //  0 = ""
    WP_ES,                                                      //  1 = "ES"
    WP_IST,                                                     //  3 = "IST"
    WP_VIERTEL_1,                                               //  5 = "VIERTEL"
    WP_EIN_1,                                                   //  6 = "EIN"
    WP_EINS_1,                                                  //  7 = "EINS"
    WP_IN,                                                      //  8 = "IN"
    WP_DREI_1,                                                  //  9 = "DREI"
    WP_EIN_2,                                                   // 10 = "EIN"
    WP_EINE_1,                                                  // 11 = "EINE"
    WP_EINER,                                                   // 12 = "EINER"
    WP_SECH_1,                                                  // 13 = "SECH"
    WP_SECHS_1,                                                 // 14 = "SECHS"
    WP_SIEB_1,                                                  // 15 = "SIEB"
    WP_SIEBEN_1,                                                // 16 = "SIEBEN"
    WP_ELF_1,                                                   // 17 = "ELF"
    WP_FUENF_1,                                                 // 18 = "FÜNF"
    WP_NEUN_1,                                                  // 19 = "NEUN"
    WP_VIER_1,                                                  // 20 = "VIER"
    WP_ACHT_1,                                                  // 21 = "ACHT"
    WP_NULL_1,                                                  // 22 = "NULL"
    WP_ZWEI_1,                                                  // 23 = "ZWEI"
    WP_ZWOELF_1,                                                // 24 = "ZWÖLF"
    WP_ZEHN_1,                                                  // 25 = "ZEHN"
    WP_UND_1,                                                   // 26 = "UND"
    WP_ZWANZIG_1,                                               // 27 = "ZWANZIG"
    WP_VIERZIG_1,                                               // 28 = "VIERZIG"
    WP_DREISSIG_1,                                              // 29 = "DREISSIG"
    WP_FUENFZIG_1,                                              // 30 = "FÜNFZIG"
    WP_UHR_1,                                                   // 31 = "UHR"
    WP_MINUTE_1,                                                // 32 = "MINUTE"
    WP_MINUTEN_1,                                               // 33 = "MINUTEN"
    WP_VOR_1,                                                   // 34 = "VOR"
    WP_UND_2,                                                   // 35 = "UND"
    WP_NACH_1,                                                  // 36 = "NACH"
    WP_EIN_3,                                                   // 37 = "EIN"
    WP_DREIVIERTEL,                                             // 38 = "DREIVIERTEL"
    WP_VIERTEL_2,                                               // 39 = "VIERTEL"
    WP_HALB,                                                    // 40 = "HALB"
    WP_SIEB_2,                                                  // 41 = "SIEB"
    WP_SIEBEN_2,                                                // 42 = "SIEBEN"
    WP_NEUN_2,                                                  // 43 = "NEUN"
    WP_NULL_2,                                                  // 44 = "NULL"
    WP_ZWEI_2,                                                  // 45 = "ZWEI"
    WP_EIN_4,                                                   // 46 = "EIN"
    WP_EINE_2,                                                  // 47 = "EINE"
    WP_FUENF_2,                                                 // 48 = "FÜNF"
    WP_SECH_2,                                                  // 49 = "SECH"
    WP_SECHS_2,                                                 // 50 = "SECHS"
    WP_NACH_2,                                                  // 51 = "NACH"
    WP_ACHT_2,                                                  // 52 = "ACHT"
    WP_VIER_2,                                                  // 53 = "VIER"
    WP_DREI_2,                                                  // 54 = "DREI"
    WP_EIN_5,                                                   // 55 = "EIN"
    WP_EINS_2,                                                  // 56 = "EINS"
    WP_UND_3,                                                   // 57 = "UND"
    WP_ELF_2,                                                   // 58 = "ELF"
    WP_ZEHN_2,                                                  // 59 = "ZEHN"
    WP_ZWANZIG_2,                                               // 60 = "ZWANZIG"
    WP_GRAD,                                                    // 61 = "GRAD"
    WP_DREISSIG_2,                                              // 62 = "DREISSIG"
    WP_VIERZIG_2,                                               // 63 = "VIERZIG"
    WP_ZWOELF_2,                                                // 64 = "ZWÖLF"
    WP_FUENFZIG_2,                                              // 65 = "FÜNFZIG"
    WP_MINUTE_2,                                                // 66 = "MINUTE"
    WP_MINUTEN_2,                                               // 67 = "MINUTEN"
    WP_UHR_2,                                                   // 68 = "UHR"
    WP_FRUEH,                                                   // 69 = "FRÜH"
    WP_VOR_2,                                                   // 70 = "VOR"
    WP_ABENDS,                                                  // 71 = "ABENDS"
    WP_MITTERNACHT,                                             // 72 = "MITTERNACHT"
    WP_NACH_3,                                                  // 73 = "NACH"
    WP_NACHTS,                                                  // 74 = "NACHTS"
    WP_MORGENS,                                                 // 75 = "MORGENS"
    WP_WARM,                                                    // 76 = "WARM"
    WP_MITTAGS,                                                 // 77 = "MITTAGS"
    WP_COUNT,                                                   // number of words
};

#endif // TABLES_H
#endif // WCLOCK24H == 1
