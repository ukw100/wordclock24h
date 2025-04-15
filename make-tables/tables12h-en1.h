/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-en1.h - extern declarations for english (variant 1) WordClock12h
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
#define HOUR_MODES_COUNT        1                       // 1 english variant 2
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // ENGLISH1
    WP_IT_EN,
    WP_IS_EN,
    WP_AM_EN,
    WP_PM_EN,
    WP_QUARTER_EN,
    WP_TWENTY_EN,
    WP_FIVE_1_EN,
    WP_HALF_EN,
    WP_TEN_1_EN,
    WP_TO_EN,
    WP_PAST_EN,
    WP_NINE_EN,
    WP_ONE_EN,
    WP_SIX_EN,
    WP_THREE_EN,
    WP_FOUR_EN,
    WP_FIVE_2_EN,
    WP_TWO_EN,
    WP_EIGHT_EN,
    WP_ELEVEN_EN,
    WP_SEVEN_EN,
    WP_TWELVE_EN,
    WP_TEN_2_EN,
    WP_OCLOCK_EN,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
