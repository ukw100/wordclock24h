/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-ch2.h - extern declarations for swiss (variant 2) WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 swiss2
#define HOUR_MODES_COUNT        1                       // 1 swiss2
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // SWISS2
    WP_ES_CH2,
    WP_ESCH_CH2,
    WP_FOIF_CH2,
    WP_VIERTU_CH2,
    WP_ZAEAE_CH2,
    WP_ZWAENZ_CH2,
    WP_VOR_CH2,
    WP_AB_CH2,
    WP_HAUBI_CH2,
    WP_EIS_CH2,
    WP_ZWOEI_CH2,
    WP_DRUE_CH2,
    WP_VIERI_CH2,
    WP_FOEIFI_CH2,
    WP_SAECHSI_CH2,
    WP_SEBNI_CH2,
    WP_ACHTI_CH2,
    WP_NUENI_CH2,
    WP_ZAENI_CH2,
    WP_ELFI_CH2,
    WP_ZWOELFI_CH2,
    WP_UHR_CH2,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
