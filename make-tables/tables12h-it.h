/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables12h-it.h - extern declarations for italian WordClock12h
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

#define DISPLAY_MODES_COUNT     1                       // 1 italian
#define HOUR_MODES_COUNT        1                       // 1 italian
#define HOUR_COUNT              12                      // 12 hours per day
#define MINUTE_MODES_COUNT      DISPLAY_MODES_COUNT     // MODES_COUNT is identical with MODES_COUNT on WC12h
#define MINUTE_COUNT            12                      // 12 steps per hour: min = 0, 5, 10, 15 ....

enum wc12h_words
{
    WP_END_OF_WORDS,

    // ITALIAN
    WP_SONO_IT,
    WP_LE_IT,
    WP_E_IT,
    WP_L_IT,
    WP_UNA_IT,
    WP_DUE_IT,
    WP_TRE_IT,
    WP_OTTO_IT,
    WP_NOVE_IT,
    WP_DIECI_IT,
    WP_UNDICI_IT,
    WP_DODICI_IT,
    WP_SETTE_IT,
    WP_QUATTRO_IT,
    WP_SEI_IT,
    WP_CINQUE_IT,
    WP_MENO_IT,
    WP_E_1_IT,
    WP_UN_IT,
    WP_QUARTO_IT,
    WP_VENTI_IT,
    WP_VENTICINQUE_IT, 
    WP_CINQUE_1_IT,
    WP_DIECI_1_IT,
    WP_MEZZA_IT,
    WP_ORE_IT,

    WP_COUNT                                                    // last item is count
};

#endif // TABLES12H_H

#endif
