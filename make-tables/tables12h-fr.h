/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-fr.h - extern declarations for french WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 french
#define HOUR_MODES_COUNT        1                       // 1 french
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // FRENCH
    WP_IL,
    WP_EST,
    WP_DEUX,
    WP_QUADRE,
    WP_TROIS,
    WP_NEUF,
    WP_UNE,
    WP_SEPT,
    WP_HUIT,
    WP_SIX,
    WP_CINQ_1,
    WP_MIDI,
    WP_DIX_1,
    WP_MINUIT,
    WP_ONZE,
    WP_HEURE,
    WP_HEURES,
    WP_MOINS,
    WP_LE,
    WP_DIX_2,
    WP_ET_1,
    WP_QUART,
    WP_VINGT,
    WP_VINGTCINQ,
    WP_CINQ_2,
    WP_ET_2,
    WP_DEMIE,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
