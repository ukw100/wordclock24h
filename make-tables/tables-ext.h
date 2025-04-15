/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables-ext.h - extern declarations of data of WordClock tables
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
extern const DISPLAY_MODES      tbl_modes[DISPLAY_MODES_COUNT];
extern const uint8_t            tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS];
extern const MINUTEDISPLAY      tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT];
extern const WORD_ILLUMINATION  illumination[WP_COUNT];
