/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-en2.h - extern declarations for english (variant 2) WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 english variant 2
#define HOUR_MODES_COUNT        1                       // 1 english1 variant 2
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // ENGLISH2
    WP_IT_EN2,
    WP_IS_EN2,
    WP_QUARTER_EN2,
    WP_TWENTY_EN2,
    WP_FIVE_1_EN2,
    WP_HALF_EN2,
    WP_TEN_1_EN2,
    WP_TO_EN2,
    WP_PAST_EN2,
    WP_NINE_EN2,
    WP_ONE_EN2,
    WP_SIX_EN2,
    WP_THREE_EN2,
    WP_FOUR_EN2,
    WP_FIVE_2_EN2,
    WP_TWO_EN2,
    WP_EIGHT_EN2,
    WP_ELEVEN_EN2,
    WP_SEVEN_EN2,
    WP_TWELVE_EN2,
    WP_TEN_2_EN2,
    WP_O_EN2,
    WP_CLOCK_EN2,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
