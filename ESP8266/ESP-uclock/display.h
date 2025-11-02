/*----------------------------------------------------------------------------------------------------------------------------------------
 * display.h - display layout functions (TFT version)
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
extern int              display_layout_init (void);
extern char *           display_layout_fname (void);
extern uint_fast8_t     display_layout_values (uint_fast8_t, uint_fast8_t);
