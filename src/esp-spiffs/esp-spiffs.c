/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp-spiffs.c - access SPIFFS filesystem of ESP8266
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "base.h"
#include "esp8266.h"
#include "display.h"
#include "log.h"
#include "esp-spiffs.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp_spiffs_open_file - open file on ESP8266
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if 0 // currently not used
int_fast16_t
esp_spiffs_open_file (const char * fname)
{
    uint_fast8_t    code;
    int_fast16_t   len = -1;

    esp8266_send_cmd ("file-open", fname, 1);

    while ((code = esp8266_get_message ()) != ESP8266_FILEOPEN)                     // esp8266_get_message() may be only called in main loop!
    {
        if (code != ESP8266_TIMEOUT)
        {
            log_printf ("esp_spiffs_open_file: expected ESP8266_FILEOPEN, got %d\r\n", code);

            if (code == ESP8266_ERROR)
            {
                return -1;
            }
        }
    }

    len = atoi (esp8266.u.filedata);

    return len;
}
#endif // 0

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp_spiffs_close_file - close file on ESP8266
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if 0 // currently not used
void
esp_spiffs_close_file (void)
{
    uint_fast8_t    code;

    esp8266_send_cmd ("file-close", (const char *) NULL, 1);

    while ((code = esp8266_get_message ()) != ESP8266_FILECLOSE)                     // esp8266_get_message() may be only called in main loop!
    {
        if (code != ESP8266_TIMEOUT)
        {
            log_printf ("esp_spiffs_close_file: expected ESP8266_FILECLOSE, got %d\r\n", code);

            if (code == ESP8266_ERROR)
            {
                return;
            }
        }
    }
}
#endif // 0

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp_spiffs_close_file - close file on ESP8266
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if 0 // currently not used
int_fast16_t
esp_spiffs_read_file (unsigned char * bufp, int_fast16_t maxsize)
{
    uint_fast8_t    code;
    int_fast16_t    len = 0;
    char *          p;

    for (;;)
    {
        esp8266_send_cmd ("file-read", (const char *) NULL, 0);

        while ((code = esp8266_get_message ()) != ESP8266_FILEDATA)                     // esp8266_get_message() may be only called in main loop!
        {
            if (code != ESP8266_TIMEOUT)
            {
                log_printf ("esp_spiffs_read_file: expected ESP8266_FILEDATA, got %d\r\n", code);
            }

            if (code == ESP8266_ERROR)
            {
                return -1;
            }
        }

        p = esp8266.u.filedata;

        while (*p && len < maxsize)
        {
            *bufp = htoi (p, 2);
            bufp++;
            len++;
            p += 2;
        }

        if (p == esp8266.u.filedata || len == maxsize)
        {
            break;
        }
    }

    return len;
}
#endif // 0

static int  icon_block;                                                         // block number of icon data

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp_diffs_get_icon - send icon command
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
esp_diffs_get_icon (const char * fname, const char * name)
{
    char            argbuf[64];

    sprintf (argbuf, "%s\",\"%s", fname, name);
    esp8266_send_cmd ("icon", argbuf, 1);
    icon_block = 0;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp_diffs_read_icon - read icon data
 *
 * 1st Block (icon_block == 0):
 * Byte     Value
 * 1+2      #rows
 * 3+4      #cols
 * 5..8     #len colors
 * 9..12    #len animation
 *
 * Return values:
 *  0       error occured
 *  1       block read
 *  2       receive complete
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
esp_diffs_read_icon (DISPLAY_ICON * dip)
{
    static int_fast16_t     color_len;
    static int_fast16_t     anim_on_len;
    static int_fast16_t     anim_off_len;
    static int_fast16_t     color_idx;
    static int_fast16_t     anim_on_idx;
    static int_fast16_t     anim_off_idx;
    static int_fast16_t     icon_len;
    char *                  p;
    uint_fast8_t            rtc = 0;

    if (icon_block == 0)
    {
        p = esp8266.u.filedata;

        dip->rows       = htoi (p, 2);
        p += 2;
        dip->cols       = htoi (p, 2);
        p += 2;
        color_len       = htoi (p, 4);
        p += 4;
        anim_on_len     = htoi (p, 4);
        p += 4;
        anim_off_len    = htoi (p, 4);

        icon_len        = dip->rows * dip->cols;
        color_idx       = 0;
        anim_on_idx     = 0;
        anim_off_idx    = 0;

        if (dip->rows > 0 && dip->cols > 0)
        {
            esp8266_send_cmd ("icon", (const char *) NULL, 0);
            rtc = 1;
        }
    }
    else
    {
        p = esp8266.u.filedata;

        while (*p)
        {
            if (color_idx < color_len)
            {
                if (color_idx < WC_ROWS * WC_COLUMNS)
                {
                    dip->colors[color_idx++] = htoi (p, 2);
                }
            }
            else if (anim_on_idx < anim_on_len)
            {
                if (anim_on_idx < WC_ROWS * WC_COLUMNS)
                {
                    dip->animation_on[anim_on_idx++] = htoi (p, 2);
                }
            }
            else if (anim_off_idx < anim_off_len)
            {
                if (anim_off_idx < WC_ROWS * WC_COLUMNS)
                {
                    dip->animation_off[anim_off_idx++] = htoi (p, 2);
                }
            }

            p += 2;
        }

        if (p == esp8266.u.filedata || color_len + anim_on_len + anim_off_len == color_idx + anim_on_idx + anim_off_idx)
        {
            while (color_idx < icon_len && color_idx < WC_ROWS * WC_COLUMNS)
            {
                dip->colors[color_idx++] = 0;
            }

            while (anim_on_idx < icon_len && anim_on_idx < WC_ROWS * WC_COLUMNS)
            {
                dip->animation_on[anim_on_idx++] = 0;
            }

            while (anim_off_idx < icon_len && anim_off_idx < WC_ROWS * WC_COLUMNS)
            {
                dip->animation_off[anim_off_idx++] = 0;
            }

            rtc = 2;                                                                // indicate end of transmission
        }
        else
        {
            esp8266_send_cmd ("icon", (const char *) NULL, 0);
            rtc = 1;
        }
    }

    icon_block++;
    return rtc;
}
