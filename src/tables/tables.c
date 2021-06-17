/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables.c - wordclock layout tables
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdint.h>

#include "wclock24h-config.h"
#include "esp8266.h"
#include "display.h"
#include "tables.h"
#include "base.h"
#include "log.h"

TABLES_GLOBALS      tables;

static uint_fast8_t wp_count;
static uint_fast8_t current_mode;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_init () - initialize tables
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tables_init (void)
{
    esp8266_send_cmd ("tabinfo", (const char *) NULL, 1);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_get () - get a layout table
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tables_get (uint_fast8_t mode)
{
    char            buf[16];

    current_mode = mode;
    tables.complete = 0;
    sprintf (buf, "%d\",\"0", current_mode);
    esp8266_send_cmd ("tabh", buf, 1);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_get () - get temperature table (only WCLOCK24H)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H == 1
static void
tables_get_temperature_table (void)
{
    char            buf[16];

    sprintf (buf, "%d\",\"0", tables.modes_count - 1);
    esp8266_send_cmd ("tabt", buf, 1);
}
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_tabinfo () - get tables info data
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tables_tabinfo (char * info)
{
    uint_fast8_t    wc_rows;
    uint_fast8_t    wc_columns;

    wc_rows = htoi (info, 2);
    info += 2;
    wc_columns = htoi (info, 2);
    info += 2;

    if (wc_rows == WC_ROWS && wc_columns == WC_COLUMNS)
    {
        wp_count            = htoi (info, 2);
        info += 2;

        if (wp_count < WP_COUNT)
        {
            tables.it_is[0]         = htoi (info, 2);
            info += 2;

            tables.it_is[1]         = htoi (info, 2);
            info += 2;

            tables.modes_count      = htoi (info, 2);
            info += 2;

            tables.hour_count       = htoi (info, 2);
            info += 2;

            tables.max_hour_words   = htoi (info, 2);
            info += 2;

            tables.minute_count     = htoi (info, 2);
            info += 2;

            tables.max_minute_words = htoi (info, 2);
            info += 2;

            esp8266_send_cmd ("tabillu", "0", 1);
        }
        else
        {
            log_printf ("wp_count %d exceeds WP_COUNT\r\n", wp_count);
        }
    }
    else
    {
        log_printf ("wc_rows/columns %d/%d not correct\r\n", wc_rows, wc_columns);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_tabillu () - store word illumination data
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tables_tabillu (char * illu)
{
    uint_fast8_t    idx;

    idx = htoi (illu, 2);
    illu += 2;
    tables.illumination[idx].row = htoi (illu, 2);
    illu += 2;
    tables.illumination[idx].col = htoi (illu, 2);
    illu += 2;
    tables.illumination[idx].len = htoi (illu, 2);
    illu += 2;

    idx++;

    if (idx < wp_count)
    {
        char buf[8];
        sprintf (buf, "%d", idx);

        esp8266_send_cmd ("tabillu", buf, 0);
    }
    else
    {
        if (display.display_mode >= tables.modes_count)
        {
            display.display_mode = 0;
            display_save_display_mode ();
        }

#if WCLOCK24H == 1
        tables_get_temperature_table ();                            // get temperature table, then current display table
#else
        tables_get (display.display_mode);                          // get current_display table now
#endif
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_tabt () - store temperature table (only WCLOCK24H)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if WCLOCK24H == 1
void
tables_tabt (char * tabt)
{
    char            buf[16];
    uint_fast8_t    idx;
    uint_fast8_t    k;

    idx = htoi (tabt, 2);
    tabt += 2;

    tables.temperature[idx].flags = htoi (tabt, 2);
    tabt += 2;

    for (k = 0; k < tables.max_minute_words; k++)
    {
        tables.temperature[idx].word_idx[k] = htoi (tabt, 2);
        tabt += 2;

        if (tables.temperature[idx].word_idx[k] == 0)
        {
            break;
        }
    }

    idx++;

    if (idx < tables.minute_count)
    {
        sprintf (buf, "%d\",\"%d", tables.modes_count - 1, idx);
        esp8266_send_cmd ("tabt", buf, 0);
    }
    else
    {
        tables_get (display.display_mode);                              // get current_display table now
    }
}
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_tabh () - store hour table
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tables_tabh (char * tabh)
{
    char            buf[16];
    uint_fast8_t    idx;
    uint_fast8_t    k;

    idx = htoi (tabh, 2);
    tabh += 2;

    for (k = 0; k < tables.max_hour_words; k++)
    {
        tables.hours[idx][k] = htoi (tabh, 2);
        tabh += 2;

        if (tables.hours[k] == 0)
        {
            break;
        }
    }

    idx++;

    if (idx < tables.hour_count)
    {
        sprintf (buf, "%d\",\"%d", current_mode, idx);
        esp8266_send_cmd ("tabh", buf, 0);
    }
    else
    {
        sprintf (buf, "%d\",\"0", current_mode);
        esp8266_send_cmd ("tabm", buf, 1);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_tabm () - store minute table
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
tables_tabm (char * tabm)
{
    char            buf[16];
    uint_fast8_t    idx;
    uint_fast8_t    k;

    idx = htoi (tabm, 2);
    tabm += 2;

    tables.minutes[idx].flags = htoi (tabm, 2);
    tabm += 2;

    for (k = 0; k < tables.max_minute_words; k++)
    {
        tables.minutes[idx].word_idx[k] = htoi (tabm, 2);
        tabm += 2;

        if (tables.minutes[idx].word_idx[k] == 0)
        {
            break;
        }
    }

    idx++;

    if (idx < tables.minute_count)
    {
        sprintf (buf, "%d\",\"%d", current_mode, idx);
        esp8266_send_cmd ("tabm", buf, 0);
    }
    else
    {
        log_message ("tables complete");
        tables.complete = 1;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables_fill_words () - fill words to display
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
tables_fill_words (uint8_t * words, uint_fast8_t hh, uint_fast8_t mm, uint_fast8_t do_show_it_is)
{
    uint_fast8_t            is_midnight = 0;
    const MINUTEDISPLAY *   tbl_minute;
    const uint8_t *         word_idx_p;
    uint_fast16_t           idx;

    if (tables.complete)
    {
        memset (words, 0, WP_COUNT);
        tbl_minute  = &(tables.minutes[mm]);

        if (do_show_it_is)
        {
            if (tbl_minute->flags & MDF_IT_IS_1)
            {
                words[tables.it_is[0]] = 1;
                words[tables.it_is[1]] = 1;
            }
        }

        for (idx = 0; idx < tables.max_minute_words && tbl_minute->word_idx[idx] != 0; idx++)
        {
            words[tbl_minute->word_idx[idx]] = 1;
        }

        if (tbl_minute->flags & MDF_HOUR_OFFSET_1)
        {
            hh += 1;                                                // correct hour offset
        }
        else if (tbl_minute->flags & MDF_HOUR_OFFSET_2)             // only used in jester mode
        {
            hh += 2;                                                // correct hour offset
        }

        if (hh == 0 || hh == 24)                                    // we have midnight
        {
            is_midnight = 1;
        }

        while (hh >= tables.hour_count)                             // hour: 25 -> 13 -> 01
        {
            hh -= tables.hour_count;
        }

        word_idx_p = tables.hours[hh];                              // get the hour words from hour table

        for (idx = 0; idx < MAX_HOUR_WORDS && word_idx_p[idx] != 0; idx++)
        {
            if (word_idx_p[idx] == WP_IF_MINUTE_IS_0)               // if minute is null take word index + 1, otherwise word index + 2
            {                                                       // this handles "EIN UHR" instead of "EINS UHR" at 01:00 & 13:00
                if (mm == 0)
                {
                    words[word_idx_p[idx + 1]] = 1;
                }
                else
                {
                    words[word_idx_p[idx + 2]] = 1;
                }
                idx += 2;
            }
            else if (word_idx_p[idx] == WP_IF_HOUR_IS_0)            // if hour is null take word index + 1, otherwise word index + 2
            {                                                       // this handles "MINUIT" instead of "MIDI" at 00:00 in french moede
                if (is_midnight)
                {
                    words[word_idx_p[idx + 1]] = 1;
                }
                else
                {
                    words[word_idx_p[idx + 2]] = 1;
                }
                idx += 2;
            }
            else
            {
                words[word_idx_p[idx]] = 1;
            }
        }
    }

    return tables.complete;
}
