/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft.h - definitions/declarations of TFT routines
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef TFT_H
#define TFT_H

#include <stdint.h>
#include "ssd1963.h"

extern void             tft_fadein_backlight (uint32_t);
extern void             tft_fadeout_backlight (uint32_t);
extern void             tft_backlight_on (void);
extern void             tft_backlight_off (void);

extern void             tft_set_area (uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t);
extern void             tft_draw_pixel (uint_fast16_t, uint_fast16_t, uint_fast16_t);
extern void             tft_draw_horizontal_line (uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t);
extern void             tft_draw_vertical_line (uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t);
extern void             tft_draw_rectangle (uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint16_t);
extern void             tft_fill_rectangle (uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t);
extern void             tft_fill_screen (uint_fast16_t);
extern void             tft_draw_line (int, int, int, int, uint_fast16_t);
extern void             tft_draw_thick_line (int, int, int, int, uint_fast16_t);
extern void             tft_draw_circle (int, int, int, uint_fast16_t);
extern void             tft_draw_thick_circle (int, int, int, uint_fast16_t);
extern void             tft_draw_image (uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint16_t *);
extern uint_fast16_t    tft_rgb64_to_color565 (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern uint_fast16_t    tft_rgb256_to_color565 (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern void             tft_init (void);

#endif // TFT_H
