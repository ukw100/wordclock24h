/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft.c - TFT routines
 *
 * Copyright (c) 2018-2024 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (BLACK_BOARD)                                   // TFT & SSD1963 only for STM32F407

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "ssd1963.h"
#include "delay.h"
#include "tft.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_fadein_backlight ()  - fade in backlight
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_fadein_backlight (uint32_t delay_ms)
{
    uint_fast8_t    pwmf;
    uint_fast8_t    pwm;
    uint_fast8_t    pwm_conf_mask;
    uint_fast8_t    man_br;
    uint_fast8_t    min_br;
    uint_fast8_t    prescaler;
    uint_fast8_t    backlight_duty_cycle;
    int             idx;

    ssd1963_get_pwm_conf (&pwmf, &pwm, &pwm_conf_mask, &man_br, &min_br, &prescaler, &backlight_duty_cycle);

    for (idx = pwm; idx <= 0xFF; idx++)
    {
        ssd1963_set_pwm_conf (pwmf, idx, pwm_conf_mask, man_br, min_br, prescaler);
        delay_msec (delay_ms);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_fadeout_backlight () - fade out backlight
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_fadeout_backlight (uint32_t delay_ms)
{
    uint_fast8_t    pwmf;
    uint_fast8_t    pwm;
    uint_fast8_t    pwm_conf_mask;
    uint_fast8_t    man_br;
    uint_fast8_t    min_br;
    uint_fast8_t    prescaler;
    uint_fast8_t    backlight_duty_cycle;
    signed int      idx;

    ssd1963_get_pwm_conf (&pwmf, &pwm, &pwm_conf_mask, &man_br, &min_br, &prescaler, &backlight_duty_cycle);

    for (idx = pwm; idx >= 0; idx--)
    {
        ssd1963_set_pwm_conf (pwmf, idx, pwm_conf_mask, man_br, min_br, prescaler);
        delay_msec (delay_ms);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_backlight_on () - switch backlight on
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_backlight_on (void)
{
    uint_fast8_t    pwmf;
    uint_fast8_t    pwm;
    uint_fast8_t    pwm_conf_mask;
    uint_fast8_t    man_br;
    uint_fast8_t    min_br;
    uint_fast8_t    prescaler;
    uint_fast8_t    backlight_duty_cycle;

    ssd1963_get_pwm_conf (&pwmf, &pwm, &pwm_conf_mask, &man_br, &min_br, &prescaler, &backlight_duty_cycle);
    pwm     = 0xFF;
    man_br  = 0xFF;
    ssd1963_set_pwm_conf (pwmf, pwm, pwm_conf_mask, man_br, min_br, prescaler);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_backlight_off () - switch backlight off
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_backlight_off (void)
{
    uint_fast8_t    pwmf;
    uint_fast8_t    pwm;
    uint_fast8_t    pwm_conf_mask;
    uint_fast8_t    man_br;
    uint_fast8_t    min_br;
    uint_fast8_t    prescaler;
    uint_fast8_t    backlight_duty_cycle;

    ssd1963_get_pwm_conf (&pwmf, &pwm, &pwm_conf_mask, &man_br, &min_br, &prescaler, &backlight_duty_cycle);
    pwm     = 0x00;
    man_br  = 0x00;
    ssd1963_set_pwm_conf (pwmf, pwm, pwm_conf_mask, man_br, min_br, prescaler);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_set_area ()  - set area
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_set_area (uint_fast16_t x0, uint_fast16_t x1, uint_fast16_t y0, uint_fast16_t y1)
{
    ssd1963_set_column_address (x0, x1);
    ssd1963_set_page_address (y0, y1);
    ssd1963_write_memory_start ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_pixel () - draw pixel
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_pixel (uint_fast16_t x, uint_fast16_t y, uint_fast16_t color565)
{
    tft_set_area (x, x, y, y);
    ssd1963_write_data (color565);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_horizontal_line () - draw horizontal line
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_horizontal_line (uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t len, uint_fast16_t color565)
{
    uint_fast16_t   l;

    tft_set_area (x0, x0 + len - 1, y0, y0);

    for (l = 0; l < len; l++)
    {
        ssd1963_write_data (color565);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_vertical_line () - draw vertical line
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_vertical_line (uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t height, uint_fast16_t color565)
{
    uint_fast16_t   h;

    tft_set_area (x0, x0, y0, y0 + height - 1);

    for (h = 0; h < height; h++)
    {
        ssd1963_write_data (color565);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_rectangle () - draw rectangle
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_rectangle (uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1, uint16_t color565)
{
    uint_fast16_t   l = x1 - x0 + 1;
    uint_fast16_t   h = y1 - y0 + 1;

    tft_draw_horizontal_line (x0, y0, l, color565);
    tft_draw_horizontal_line (x0, y1, l, color565);

    tft_draw_vertical_line (x0, y0, h, color565);
    tft_draw_vertical_line (x1, y0, h, color565);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_fill_rectangle () - fill rectangle
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_fill_rectangle (uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1, uint_fast16_t color565)
{
    uint_fast16_t   n;
    uint_fast16_t   i;

    n = (x1 - x0 + 1) * (y1 - y0 + 1);

    tft_set_area (x0, x1, y0, y1);

    for (i = 0; i < n; i++)
    {
        ssd1963_write_data (color565);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_fill_screen () - clear total screen
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_fill_screen (uint_fast16_t color565)
{
    unsigned int x, y;

    tft_set_area (0, TFT_WIDTH - 1 , 0, TFT_HEIGHT - 1);

    for (x = 0; x < TFT_WIDTH; x++)
    {
        for (y = 0; y < TFT_HEIGHT; y++)
        {
            ssd1963_write_data (color565);
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_line () - draw a line using a compact variant of Bresenham algorithm
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_line (int x0, int y0, int x1, int y1, uint_fast16_t color565)
{
    int     dx  =  abs (x1 - x0);
    int     dy  = -abs (y1 - y0);
    int     sx  = x0 < x1 ? 1 : -1;
    int     sy  = y0 < y1 ? 1 : -1;
    int     err = dx + dy;
    int     e2;

    while(1)
    {
        tft_draw_pixel (x0, y0, color565);

        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        e2 = 2 * err;

        if (e2 > dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void
tft_draw_thick_line (int x0, int y0, int x1, int y1, uint_fast16_t color565)
{
    tft_draw_line (x0 + 0, y0 + 0, x1 + 0, y1 + 0, color565);
    tft_draw_line (x0 + 0, y0 + 1, x1 + 0, y1 + 1, color565);
    tft_draw_line (x0 + 1, y0 + 0, x1 + 1, y1 + 0, color565);
    tft_draw_line (x0 + 1, y0 + 1, x1 + 1, y1 + 1, color565);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_circle () - draw a circle using a variant of Bresenham algorithm
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_circle (int x0, int y0, int radius, uint_fast16_t color565)
{
    int     f       = 1 - radius;
    int     ddF_x   = 0;
    int     ddF_y   = -2 * radius;
    int     x       = 0;
    int     y       = radius;

    tft_draw_pixel (x0, y0 + radius, color565);
    tft_draw_pixel (x0, y0 - radius, color565);
    tft_draw_pixel (x0 + radius, y0, color565);
    tft_draw_pixel (x0 - radius, y0, color565);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        tft_draw_pixel (x0 + x, y0 + y, color565);
        tft_draw_pixel (x0 - x, y0 + y, color565);
        tft_draw_pixel (x0 + x, y0 - y, color565);
        tft_draw_pixel (x0 - x, y0 - y, color565);
        tft_draw_pixel (x0 + y, y0 + x, color565);
        tft_draw_pixel (x0 - y, y0 + x, color565);
        tft_draw_pixel (x0 + y, y0 - x, color565);
        tft_draw_pixel (x0 - y, y0 - x, color565);
    }
}

void
tft_draw_thick_circle (int x0, int y0, int radius, uint_fast16_t color565)
{
    tft_draw_circle (x0 + 0, y0 + 0, radius, color565);
    tft_draw_circle (x0 + 0, y0 + 1, radius, color565);
    tft_draw_circle (x0 + 1, y0 + 0, radius, color565);
    tft_draw_circle (x0 + 1, y0 + 1, radius, color565);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_draw_image () - draw an image
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_draw_image (uint_fast16_t x, uint_fast16_t y, uint_fast16_t l, uint_fast16_t h, uint16_t * image)
{
    tft_set_area (x, x + l - 1, y, y + h - 1);

    while (h--)
    {
        while (l--)
        {
            ssd1963_write_data (*image);
            image++;
        }
    }

}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_rgb64_to_color565 () - convert RGB64 to 16 bit value: 5 bits of red, 6 bits of green, 5 bits of blue
 *
 * bits:   15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 * format: R5 R4 R3 R2 R1 G5 G4 G3 G2 G1 G0 B5 B4 B3 B2 B1
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast16_t
tft_rgb64_to_color565 (uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue)
{
    return (((red >> 1) & 0x1F) << 10) | ((green & 0x3F) << 5) | ((blue >> 1) & 0x1F);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_rgb256_to_color565 () - convert RGB256 to 16 bit value: 5 bits of red, 6 bits of green, 5 bits of blue
 *
 * bits:   15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 * format: R5 R4 R3 R2 R1 G5 G4 G3 G2 G1 G0 B5 B4 B3 B2 B1
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast16_t
tft_rgb256_to_color565 (uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue)
{
    return (((red >> 3) & 0x1F) << 11) | (((green >> 2) & 0x3F) << 5) | ((blue >> 3) & 0x1F);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tft_init ()
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tft_init (void)
{
    ssd1963_init ();
}

#endif // BLACK_BOARD
