/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-se.h - extern declarations for swedish WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 swedish
#define HOUR_MODES_COUNT        1                       // 1 swedish
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // SWEDISH
    WP_KLOCKAN,
    WP_AER,
    WP_FEM_1,
    WP_FEM_I,
    WP_TIO_1,
    WP_TIO_I,
    WP_KVART,
    WP_KVART_I,
    WP_TJUGO,
    WP_TJUGO_I,
    WP_OEVER,
    WP_HALV,
    WP_ETT,
    WP_TVA,
    WP_TRE,
    WP_FYRA,
    WP_FEM_2,
    WP_SEX,
    WP_SJU,
    WP_ATTA,
    WP_NIO,
    WP_TIO_2,
    WP_ELVA,
    WP_TOLV,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
