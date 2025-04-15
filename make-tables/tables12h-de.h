/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-de.h - extern declarations for german WordClock12h
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H == 0

#ifndef TABLES12H_H
#define TABLES12H_H

#include <stdint.h>

#define WC_ROWS                 10                      // 10 rows
#define WC_COLUMNS              11                      // 11 columns

#define DISPLAY_MODES_COUNT     6                       // 6 german variants
#define HOUR_MODES_COUNT        1                       // 1 german hour mode
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // GERMAN:
    WP_ES,
    WP_IST,
    WP_FUENF_1,
    WP_ZEHN_1,
    WP_ZWANZIG,
    WP_DREI_1,
    WP_VIER,
    WP_VIERTEL,
    WP_DREIVIERTEL,
    WP_NACH,
    WP_VOR,
    WP_HALB,
    WP_ZWOELF,
    WP_ZWEI,
    WP_EIN,
    WP_EINS,
    WP_SIEBEN,
    WP_DREI_2,
    WP_FUENF_2,
    WP_ELF,
    WP_NEUN,
    WP_VIER_2,
    WP_ACHT,
    WP_ZEHN_2,
    WP_SECHS,
    WP_UHR,
    WP_ZIG,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
