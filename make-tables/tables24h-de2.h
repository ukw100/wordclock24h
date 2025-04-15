/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h-de2.h - WC24H DE2 tables - 15x15 layout
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H_DE2 == 1

#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

#define WC_ROWS                 15                              // 15 rows
#define WC_COLUMNS              15                              // 15 columns

#define DISPLAY_MODES_COUNT     2                               // count of different display modes
#define HOUR_COUNT              24                              // 24 plus one to distinguish between before and after full hour
#define MINUTE_COUNT            60

enum HourMode
{
    HM_0,                                                       // 0 = Mode 0: "leer, fuer Temperaturanzeige"
    HM_1,                                                       // 1 = Mode 1: "Uhrzeit"
    HOUR_MODES_COUNT                                            // Number of HourModes
};

enum MinuteMode
{
    MM_0,                                                       // 0 = Mode 1: "Uhrzeit"
    MM_1,                                                       // 1 = Mode 2: "Temperatur"
    MINUTE_MODES_COUNT                                          // Number of MinuteModes
};

enum WordPos
{
    WP_END_OF_WORDS,                                            // ""
    WP_ES,                                                      // "ES"
    WP_IST,                                                     // "IST"
    WP_NULL_1,                                                  // "NULL"
    WP_EIN_1,                                                   // "EIN"
    WP_ZWEI_1,                                                  // "ZWEI"
    WP_DREI_1,                                                  // "DREI"
    WP_VIER_1,                                                  // "VIER"
    WP_FUENF_1,                                                 // "FÜNF"
    WP_SECH_1,                                                  // "SECHS"
    WP_SECHS_1,                                                 // "SECHS"
    WP_SIEB_1,                                                  // "SIEBEN"
    WP_SIEBEN_1,                                                // "SIEBEN"
    WP_ACHT_1,                                                  // "ACHT"
    WP_NEUN_1,                                                  // "NEUN"
    WP_ZEHN_1,                                                  // "ZEHN"
    WP_ELF_1,                                                   // "ELF"
    WP_ZWOELF_1,                                                // "ZWÖLF"
    WP_UND_1,                                                   // "UND"
    WP_ZWANZIG_1,                                               // "ZWANZIG"
    WP_UHR_1,                                                   // "UHR"
    WP_UND_2,                                                   // "UND"
    WP_EIN_2,                                                   // "EIN"
    WP_EINE_1,                                                  // "EINE"
    WP_ZWEI_2,                                                  // "ZWEI"
    WP_DREI_2,                                                  // "DREI"
    WP_VIER_2,                                                  // "VIER"
    WP_FUENF_2,                                                 // "FÜNF"
    WP_SECH_2,                                                  // "SECH"
    WP_SECHS_2,                                                 // "SECHS"
    WP_SIEB_2,                                                  // "SIEB"
    WP_SIEBEN_2,                                                // "SIEBEN"
    WP_ACHT_2,                                                  // "ACHT"
    WP_NEUN_2,                                                  // "NEUN"
    WP_ZEHN_2,                                                  // "ZEHN"
    WP_ELF_2,                                                   // "ELF"
    WP_ZWOELF_2,                                                // "ZWÖLF"
    WP_UND_3,                                                   // "UND"
    WP_ZWANZIG_2,                                               // "ZWANZIG"
    WP_DREISSIG_1,                                              // "DREIßIG"
    WP_VIERZIG_1,                                               // "VIERZIG"
    WP_NULL_2,                                                  // "NULL"
    WP_FUENFZIG_1,                                              // "FÜNFZIG"
    WP_MINUTE_1,                                                // "MINUTE"
    WP_MINUTEN_1,                                               // "MINUTEN"
    WP_COUNT,                                                   // number of words
};

#endif // TABLES_H
#endif // WCLOCK24H == 1
