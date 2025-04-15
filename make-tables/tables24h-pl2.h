/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h-pl2.h - WC24H tables (polski2)
 *
 * Copyright (c) 2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H_PL2 == 1

#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

#define WC_ROWS                 20                              // 20 rows - POLSKI2
#define WC_COLUMNS              20                              // 20 columns

#define DISPLAY_MODES_COUNT     2                               // count of different display modes
#define HOUR_COUNT              24                              // 24 plus one to distinguish between before and after full hour
#define MINUTE_COUNT            60

enum HourMode
{
    HM_0,                                                       // 0 = Mode 0: "leer, fuer Temperaturanzeige"
    HM_1,                                                       // 1 = Mode 1: "Polski2"
    HOUR_MODES_COUNT                                            // Number of HourModes
};

enum MinuteMode
{
    MM_0,                                                       // 0 = Mode 1: "Polski2"
    MM_1,                                                       // 1 = Mode 2: "Temperature"
    MINUTE_MODES_COUNT                                          // Number of MinuteModes
};

enum WordPos
{
    WP_END_OF_WORDS,                                            // 00 = ""
    WP_JEST,                                                    // 01
    WP_PIERWSZA,                                                // 02
    WP_DRUGA,                                                   // 03
    WP_TRZECIA,                                                 // 04
    WP_CZWARTA,                                                 // 05
    WP_PIATA,                                                   // 06
    WP_SZOSTA,                                                  // 07
    WP_SIODMA,                                                  // 08
    WP_OSMA,                                                    // 09
    WP_DZIEWIATA,                                               // 10
    WP_DZIESIATA,                                               // 11
    WP_JEDENASTA,                                               // 12
    WP_DWUNASTA,                                                // 13
    WP_TRZYNASTA,                                               // 14
    WP_CZTERNASTA,                                              // 15
    WP_PIETNASTA,                                               // 16
    WP_SZESNASTA,                                               // 17
    WP_SIEDEMNASTA,                                             // 18
    WP_OSIEMNASTA,                                              // 19
    WP_DZIEWIETNASTA,                                           // 20
    WP_DWUDZIESTA,                                              // 21
    WP_ZERO,                                                    // 22
    WP_ZERO_1,                                                  // 23
    WP_ZERO_2,                                                  // 24
    WP_JEDEN,                                                   // 25
    WP_DWA,                                                     // 26
    WP_TRZY,                                                    // 27
    WP_CZTERY,                                                  // 28
    WP_PIEC,                                                    // 29
    WP_SZESC,                                                   // 30
    WP_SIEDEM,                                                  // 31
    WP_OSIEM,                                                   // 32
    WP_DZIEWIEC,                                                // 33
    WP_DZIESIEC,                                                // 34
    WP_JEDENASCIE,                                              // 35
    WP_DWANASCIE,                                               // 36
    WP_TRZYNASCIE,                                              // 37
    WP_CZTERNASCIE,                                             // 38
    WP_PIETNASCIE,                                              // 39
    WP_SZESNASCIE,                                              // 40
    WP_SIEDEMNASCIE,                                            // 41
    WP_OSIEMNASCIE,                                             // 42
    WP_DZIEWIETNASCIE,                                          // 43
    WP_DWADZIESCIA,                                             // 44
    WP_TRZYDZIESCI,                                             // 45
    WP_CZTERDZIESCI,                                            // 46
    WP_PIECDZIESIAT,                                            // 47
    WP_C,                                                       // 48
    WP_COUNT,                                                   // number of words
};

#endif // TABLES_H
#endif // WCLOCK24H == 1
