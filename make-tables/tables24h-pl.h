/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h-pl.h - WC24H tables (polski)
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H_PL == 1

#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

#define WC_ROWS                 18                              // 16 rows - POLSKI 18
#define WC_COLUMNS              18                              // 18 columns

#define DISPLAY_MODES_COUNT     2                               // count of different display modes
#define HOUR_COUNT              24                              // 24 plus one to distinguish between before and after full hour
#define MINUTE_COUNT            60

enum HourMode
{
    HM_0,                                                       // 0 = Mode 0: "leer, fuer Temperaturanzeige"
    HM_1,                                                       // 1 = Mode 1: "Polski"
    HOUR_MODES_COUNT                                            // Number of HourModes
};

enum MinuteMode
{
    MM_0,                                                       // 0 = Mode 1: "Polski"
    MM_1,                                                       // 1 = Mode 2: "Temperature"
    MINUTE_MODES_COUNT                                          // Number of MinuteModes
};

enum WordPos
{
    WP_END_OF_WORDS,                                            // 00 = ""
    WP_PIERWSZA,                                                // 01
    WP_DRUGA,                                                   // 02
    WP_TRZECIA,                                                 // 03
    WP_CZWARTA,                                                 // 04
    WP_PIATA,                                                   // 05
    WP_SZOSTA,                                                  // 06
    WP_SIODMA,                                                  // 07
    WP_OSMA,                                                    // 08
    WP_DZIEWIATA,                                               // 09
    WP_DZIESIATA,                                               // 10
    WP_JEDENASTA,                                               // 11
    WP_DWUNASTA,                                                // 12
    WP_ZERO,                                                    // 13
    WP_ZERO_1,                                                  // 14
    WP_JEDEN,                                                   // 15
    WP_DWA,                                                     // 16
    WP_TRZY,                                                    // 17
    WP_CZTERY,                                                  // 18
    WP_PIEC,                                                    // 19
    WP_SZESC,                                                   // 20
    WP_SIEDEM,                                                  // 21
    WP_OSIEM,                                                   // 22
    WP_DZIEWIEC,                                                // 23
    WP_DZIESIEC,                                                // 24
    WP_JEDENASCIE,                                              // 25
    WP_DWANASCIE,                                               // 26
    WP_TRZYNASCIE,                                              // 27
    WP_CZTERNASCIE,                                             // 28
    WP_PIETNASCIE,                                              // 29
    WP_SZESNASCIE,                                              // 30
    WP_SIEDEMNASCIE,                                            // 31
    WP_OSIEMNASCIE,                                             // 32
    WP_DZIEWIETNASCIE,                                          // 33
    WP_DWADZIESCIA,                                             // 34
    WP_TRZYDZIESCI,                                             // 35
    WP_CZTERDZIESCI,                                            // 36
    WP_PIECDZIESIAT,                                            // 37
    WP_PM,                                                      // 38
    WP_AM,                                                      // 39
    WP_JEST,                                                    // 40
    WP_COUNT,                                                   // number of words
    
};

#endif // TABLES_H
#endif // WCLOCK24H == 1
