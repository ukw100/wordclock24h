/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-ch1.h - extern declarations for swiss (variant 1) WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 swiss1
#define HOUR_MODES_COUNT        1                       // 1 swiss1
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // SWISS1
    WP_ES_CH,
    WP_ISCH_CH,
    WP_FUEF_CH,
    WP_VIERTU_CH,
    WP_ZAEAE_CH,
    WP_ZWAENZG_CH,
    WP_VOR_CH,
    WP_AB_CH,
    WP_HAUBI_CH,
    WP_EIS_CH,
    WP_ZWOEI_CH,
    WP_DRUE_CH,
    WP_VIERI_CH,
    WP_FUEFI_CH,
    WP_SAECHSI_CH,
    WP_SIBNI_CH,
    WP_ACHTI_CH,
    WP_NUENI_CH,
    WP_ZAENI_CH,
    WP_EUFI_CH,
    WP_ZWOEUFI_CH,
    WP_UHR_CH,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
