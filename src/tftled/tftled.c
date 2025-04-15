/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * tftled.c - LED simulator displaying ASCII characters for each LED
 *
 * Copyright (c) 2018-2024 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>
#include "base.h"
#include "esp8266.h"
#include "tft.h"
#include "font.h"
#include "tftled.h"
#include "display-config.h"
#include "wclock24h-config.h"

#include "log.h"

#if DSP_USE_TFTLED_RGB                                                              // TFT & SSD1963 only for STM32F407

#define COLOR_DARKRED           0x3800
#define COLOR_DARKGREEN         0x01E0
#define COLOR_DARKBLUE          0x0007
#define COLOR_DARKCYAN          (COLOR_DARKGREEN  | COLOR_DARKBLUE)
#define COLOR_DARKMAGENTA       (COLOR_DARKRED    | COLOR_DARKBLUE)
#define COLOR_DARKYELLOW        (COLOR_DARKRED    | COLOR_DARKGREEN)
#define COLOR_GRAY              (COLOR_DARKRED    | COLOR_DARKGREEN   | COLOR_DARKBLUE)
#define COLOR_DARKGRAY          (0x1800 | 0x0E0| 0x0003 )

#define TFTLED_MAX_LEDS         (DSP_STATUS_LEDS + DSP_MINUTE_LEDS + DSP_DISPLAY_LEDS)


#if WCLOCK24H == 1

#define WC_FONT                 FONT_16x26
#define FONT_HEIGHT             26
#define FONT_WIDTH              21

#else

#define WC_FONT                 FONT_24x40
#define FONT_HEIGHT             40
#define FONT_WIDTH              32

#endif

static uint8_t                  layout_table[DSP_DISPLAY_LEDS];

#define MARGIN                  ((480 - (WC_ROWS * FONT_HEIGHT)) / 2)                   // upper/left margin in pixels

static uint16_t                 new_rgb_buf[TFTLED_MAX_LEDS];
static uint16_t                 current_rgb_buf[TFTLED_MAX_LEDS];

void
tftled_layout (uint_fast8_t row)
{
    char    buf[32];

    sprintf (buf, "%d\",\"%d", row, WC_COLUMNS);
    esp8266_send_cmd ("disp", buf, 0);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * redraw display after getting display layout
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
tftled_redraw_display (void)
{
    uint_fast16_t   idx;
    unsigned char   letter;
    uint_fast16_t   y;
    uint_fast16_t   x;
    uint_fast16_t   py;
    uint_fast16_t   px;
    uint_fast16_t   fcolor565;
    uint_fast16_t   bcolor565;

    idx = (DSP_STATUS_LEDS + DSP_MINUTE_LEDS);

    py = MARGIN;

    for (y = 0; y < WC_ROWS; y++)
    {
        px = MARGIN;

        for (x = 0; x < WC_COLUMNS; x++)
        {
            current_rgb_buf[idx] = new_rgb_buf[idx];

            fcolor565   = current_rgb_buf[idx];

            if (fcolor565 == 0x0000)
            {
                fcolor565 = COLOR_DARKGRAY;
            }

            bcolor565   = 0x0000;

            letter = layout_table[idx - (DSP_STATUS_LEDS + DSP_MINUTE_LEDS)];
            draw_letter (letter, py, px, fcolor565, bcolor565);

            px += FONT_WIDTH;
            idx++;
        }

        py += FONT_HEIGHT;
    }
}

void
tftled_layout_get_line (char * str)
{
    uint_fast8_t    row;
    uint32_t        offset;

    row = htoi (str, 2);
    str += 2;

    offset = row * WC_COLUMNS;

    while (* str)
    {
        if (offset < DSP_DISPLAY_LEDS)
        {
            layout_table[offset] = *str;
            offset++;
        }
        str++;
    }

    if (row < WC_ROWS - 1)
    {
        row++;
        tftled_layout (row);
    }
    else
    {
        tftled_redraw_display ();
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * clear all LEDs
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#if 0 // yet not used
static void
tftled_clear_all (uint_fast16_t n_leds)
{
    TFTLED_RGB  rgb = { 0, 0, 0 };

    tftled_set_all_leds (&rgb, n_leds, 1);
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * refresh buffer
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tftled_refresh (uint_fast16_t n_leds)
{
    uint_fast16_t   idx;
    unsigned char   letter;
    uint_fast16_t   y;
    uint_fast16_t   x;
    uint_fast16_t   py;
    uint_fast16_t   px;
    uint_fast16_t   fcolor565;
    uint_fast16_t   bcolor565;

    if (n_leds > TFTLED_MAX_LEDS)
    {
        n_leds = TFTLED_MAX_LEDS;
    }

    idx = (DSP_STATUS_LEDS + DSP_MINUTE_LEDS);

    py = MARGIN;

    for (y = 0; y < WC_ROWS; y++)
    {
        px = MARGIN;

        for (x = 0; x < WC_COLUMNS; x++)
        {
            if (current_rgb_buf[idx] != new_rgb_buf[idx])  // optimization: only update LEDs, which have been changed
            {
                current_rgb_buf[idx] = new_rgb_buf[idx];

                fcolor565   = current_rgb_buf[idx];

                if (fcolor565 == 0x0000)
                {
                    fcolor565 = COLOR_DARKGRAY;
                }

                bcolor565   = 0x0000;

                letter = layout_table[idx - (DSP_STATUS_LEDS + DSP_MINUTE_LEDS)];
                draw_letter (letter, py, px, fcolor565, bcolor565);
            }

            px += FONT_WIDTH;
            idx++;
        }

        py += FONT_HEIGHT;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set one RGB value
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tftled_set_led (uint_fast16_t n, TFTLED_RGB * rgb)
{
    if (n < TFTLED_MAX_LEDS)
    {
        uint_fast16_t color565;

        color565 = tft_rgb256_to_color565 (rgb->red, rgb->green, rgb->blue);
        new_rgb_buf[n] = color565;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set all LEDs to RGB value
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tftled_set_all_leds (TFTLED_RGB * rgb, uint_fast16_t n_leds, uint_fast8_t refresh)
{
    uint_fast16_t n;
    uint_fast16_t color565;

    if (n_leds > TFTLED_MAX_LEDS)
    {
        n_leds = TFTLED_MAX_LEDS;
    }

    color565 = tft_rgb256_to_color565 (rgb->red, rgb->green, rgb->blue);

    for (n = 0; n < n_leds; n++)
    {
        new_rgb_buf[n] = color565;
    }

    if (refresh)
    {
        tftled_refresh (n_leds);
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * initialize TFTLED
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tftled_init (void)
{
    tft_init ();
    tft_fill_screen (0x0000);
    set_font (WC_FONT);
    memset (layout_table, '*', DSP_DISPLAY_LEDS);
    memset (current_rgb_buf, 0xFF, TFTLED_MAX_LEDS * sizeof (uint16_t));
    tftled_refresh (TFTLED_MAX_LEDS);
}

#endif // DSP_USE_TFTLED
