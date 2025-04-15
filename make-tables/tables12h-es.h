/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-es.h - extern declarations for spanish WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 spanish
#define HOUR_MODES_COUNT        1                       // 1 spanish
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // SPANISH
    WP_ES,                                                      // ES
    WP_SON,                                                     // SON
    WP_LA,                                                      // LA
    WP_LAS,                                                     // LAS
    WP_UNA,                                                     // UNA
    WP_DOS,                                                     // DOS
    WP_TRES,                                                    // TRES
    WP_CUATRO,                                                  // CUATRO
    WP_CINCO,                                                   // CINCO
    WP_SEIS,                                                    // SEIS
    WP_SIETE,                                                   // SIETE
    WP_OCHO,                                                    // OCHO
    WP_NUEVE,                                                   // NUEVE
    WP_DIEZ,                                                    // DIEZ
    WP_ONCE,                                                    // ONCE
    WP_DOCE,                                                    // DOCE
    WP_Y,                                                       // Y
    WP_MENOS,                                                   // MENOS
    WP_VEINTE,                                                  // VEINTE
    WP_DIEZ_2,                                                  // DIEZ
    WP_VEINTICINCO,                                             // VEINTICINCO
    WP_CINCO_2,                                                 // CINCO
    WP_MEDIA,                                                   // MEDIA
    WP_CUARTO,                                                  // CUARTO

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
