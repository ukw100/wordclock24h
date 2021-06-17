/*----------------------------------------------------------------------------------------------------------------------------------------
 * tables.cpp - layout table functions
 *
 * Copyright (c) 2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "FS.h"
#include "tables.h"
#include "base.h"

static uint_fast8_t         wc_rows;
static uint_fast8_t         wc_columns;
static uint_fast8_t         wp_count;
static WORD_ILLUMINATION *  illumination;
static uint_fast8_t         tbl_it_is[2];

uint_fast8_t                display_modes_count;
DISPLAY_MODES *             tbl_modes;

static uint_fast8_t         hour_modes_count;
static uint_fast8_t         hour_count;
static uint_fast8_t         max_hour_words;
static uint8_t ***          tbl_hours;

static uint_fast8_t         minute_modes_count;
static uint_fast8_t         minute_count;
static uint_fast8_t         max_minute_words;
static MINUTEDISPLAY **     tbl_minutes;

static char                 fname[64];
static int                  tables_read = 0;
int                         tables_corrupt = 0;

int
readhex (File fp)
{
    char  buf[2];
    int   ch;

    do
    {
        ch = fp.read();
    } while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t');

    if (ch == EOF)
    {
        return (EOF);
    }

    buf[0] = ch;
    ch = fp.read();

    if (ch == EOF)
    {
        return (EOF);
    }
    
    buf[1] = ch;
    return htoi (buf, 2);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * checkfile
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static int
checkfile (const char * fname)
{
    File            fp;
    int             local_wc_rows;
    int             local_wc_columns;
    int             local_wp_count;
    int             local_row;
    int             local_col;
    int             local_len;
    int             local_tbl_it_is[2];
    int             local_display_modes_count;
    int             local_hour_idx;
    int             local_minute_idx;
    int             local_hour_modes_count;
    int             local_hour_count;
    int             local_max_hour_words;
    int             local_minute_modes_count;
    int             local_minute_count;
    int             local_max_minute_words;
    int             i;
    int             j;
    int             k;
    int             ch;

    fp = SPIFFS.open (fname, "r");

    if (! fp)
    {
        Serial.println ("- open failed");
        return 0;
    }

    local_wc_rows     = readhex(fp);

    if (local_wc_rows == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    local_wc_columns  = readhex(fp);

    if (local_wc_columns == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    local_wp_count    = readhex(fp);

    if (local_wp_count == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    if (local_wc_rows > 24)
    {
        fp.close ();
        Serial.print ("- error: wc_rows = ");
        Serial.println (local_wc_rows);
        return 0;
    }
    
    if (local_wc_columns > 24)
    {
        fp.close ();
        Serial.print ("- error: wc_columns = ");
        Serial.println (local_wc_columns);
        return 0;
    }
    
    for (i = 0; i < local_wp_count; i++)
    {
        local_row = readhex(fp);

        if (local_row == EOF)
        {
            fp.close ();
            Serial.println ("- error: unexpected EOF");
            return 0;
        }

        if (local_row >= local_wc_rows)
        {
            fp.close ();
            Serial.print ("- error: row = ");
            Serial.println (local_row);
            return 0;
        }

        local_col = readhex(fp);

        if (local_col == EOF)
        {
            fp.close ();
            Serial.println ("- error: unexpected EOF");
            return 0;
        }

        if (local_col >= local_wc_columns)
        {
            fp.close ();
            Serial.print ("- error: col = ");
            Serial.println (local_col);
            return 0;
        }

        local_len = readhex(fp);

        if (local_col + local_len > local_wc_columns)
        {
            fp.close ();
            Serial.print ("- error: col + len = ");
            Serial.println (local_col + local_len);
            return 0;
        }
    }

    local_tbl_it_is[0]        = readhex(fp);

    if (local_tbl_it_is[0] == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    if (local_tbl_it_is[0] >= local_wp_count)
    {
        fp.close ();
        Serial.print ("- error: tbl_it_is[0] = ");
        Serial.println (local_tbl_it_is[0]);
        return 0;
    }

    local_tbl_it_is[1]        = readhex(fp);

    if (local_tbl_it_is[1] == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    if (local_tbl_it_is[1] >= local_wp_count)
    {
        fp.close ();
        Serial.print ("- error: tbl_it_is[1] = ");
        Serial.println (local_tbl_it_is[1]);
        return 0;
    }

    local_display_modes_count = readhex(fp);

    if (local_display_modes_count == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    for (i = 0; i < local_display_modes_count; i++)
    {
        local_hour_idx   = readhex(fp);

        if (local_hour_idx == EOF)
        {
            fp.close ();
            Serial.println ("- error: unexpected EOF");
            return 0;
        }

        local_minute_idx = readhex(fp);

        if (local_minute_idx == EOF)
        {
            fp.close ();
            Serial.println ("- error: unexpected EOF");
            return 0;
        }

        j = 0;

        while ((ch = readhex(fp)) != '\0' && ch != EOF)
        {
            if (j >= DISPLAY_MODE_TEXT_LEN)
            {
                fp.close ();
                Serial.println ("- error: description too long");
                return 0;
            }
        }

        if (ch == EOF)
        {
            fp.close ();
            Serial.println ("- error: unexpected EOF");
            return 0;
        }
    }

    local_hour_modes_count  = readhex (fp);

    if (local_hour_modes_count == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    local_hour_count        = readhex(fp);

    if (local_hour_count == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    local_max_hour_words    = readhex(fp);

    if (local_max_hour_words == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    for (i = 0; i < local_hour_modes_count; i++)
    {
        for (j = 0; j < local_hour_count; j++)
        {
            for (k = 0; k < local_max_hour_words; k++)
            {
                ch = readhex(fp);

                if (ch == EOF)
                {
                    fp.close ();
                    Serial.println ("- error: unexpected EOF");
                    return 0;
                }

                if (ch == 0)
                {
                    break;
                }
            }
        }
    }

    local_minute_modes_count  = readhex(fp);

    if (local_minute_modes_count == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    local_minute_count        = readhex(fp);

    if (local_minute_count == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    local_max_minute_words    = readhex(fp);

    if (local_max_minute_words == EOF)
    {
        fp.close ();
        Serial.println ("- error: unexpected EOF");
        return 0;
    }

    for (i = 0; i < local_minute_modes_count; i++)
    {
        for (j = 0; j < local_minute_count; j++)
        {
            ch = readhex(fp);

            if (ch == EOF)
            {
                fp.close ();
                Serial.println ("- error: unexpected EOF");
                return 0;
            }

            for (k = 0; k < local_max_minute_words; k++)
            {
                ch = readhex(fp);

                if (ch == EOF)
                {
                    fp.close ();
                    Serial.println ("- error: unexpected EOF");
                    return 0;
                }

                if (ch == 0)
                {
                    break;
                }
            }
        }
    }

    if (readhex(fp) != EOF)              // here the fp must be at EOF!
    {
        fp.close ();
        Serial.println ("- error: too much data");
        return 0;
    }

    fp.close ();
    Serial.println ("- check tables successful");

    return 1;
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * readfile
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static int
readfile (const char * fname)
{
    File            fp;
    uint_fast8_t    i;
    uint_fast8_t    j;
    uint_fast8_t    k;
    int             ch;

    fp = SPIFFS.open (fname, "r");

    if (! fp)
    {
        Serial.println ("- open failed");
        return 0;
    }

    wc_rows     = readhex(fp);
    wc_columns  = readhex(fp);
    wp_count    = readhex(fp);

    if (! illumination)
    {
        illumination = (WORD_ILLUMINATION *) calloc (wp_count, sizeof (WORD_ILLUMINATION));
    }

    for (i = 0; i < wp_count; i++)
    {
        illumination[i].row = readhex(fp);
        illumination[i].col = readhex(fp);
        illumination[i].len = readhex(fp);
    }

    tbl_it_is[0]        = readhex(fp);
    tbl_it_is[1]        = readhex(fp);
    display_modes_count = readhex(fp);

    if (! tbl_modes)
    {
        tbl_modes = (DISPLAY_MODES *) calloc (display_modes_count, sizeof (DISPLAY_MODES));
    }

    for (i = 0; i < display_modes_count; i++)
    {
        tbl_modes[i].hour_idx   = readhex(fp);
        tbl_modes[i].minute_idx = readhex(fp);

        j = 0;

        while ((ch = readhex(fp)) != '\0' && ch != EOF)
        {
            if (j < DISPLAY_MODE_TEXT_LEN)
            {
                tbl_modes[i].description[j++] = ch;
            }
        }

        tbl_modes[i].description[j] = '\0';

        if (ch == EOF)
        {
            Serial.println ("- unexpected EOF");
            fp.close ();
            return 0;
        }
    }

    hour_modes_count  = readhex (fp);
    hour_count        = readhex(fp);
    max_hour_words    = readhex(fp);

    if (! tbl_hours)
    {
        tbl_hours = (uint8_t ***) calloc (hour_modes_count, sizeof (uint8_t **));
    }

    for (i = 0; i < hour_modes_count; i++)
    {
        if (! tbl_hours[i])
        {
            tbl_hours[i] = (uint8_t **) calloc (hour_count, sizeof (uint8_t *));
        }

        for (j = 0; j < hour_count; j++)
        {
            if (! tbl_hours[i][j])
            {
                tbl_hours[i][j] = (uint8_t *) calloc (max_hour_words, sizeof (uint8_t));
            }

            for (k = 0; k < max_hour_words; k++)
            {
                ch = readhex(fp);
                tbl_hours[i][j][k] = ch;

                if (ch == 0)
                {
                    break;
                }
            }
        }
    }

    minute_modes_count  = readhex(fp);
    minute_count        = readhex(fp);
    max_minute_words    = readhex(fp);

    if (! tbl_minutes)
    {
        tbl_minutes = (MINUTEDISPLAY **) calloc (minute_modes_count, sizeof (MINUTEDISPLAY *));
    }

    for (i = 0; i < minute_modes_count; i++)
    {
        if (! tbl_minutes[i])
        {
            tbl_minutes[i] = (MINUTEDISPLAY *) calloc (minute_count, sizeof (MINUTEDISPLAY));
        }

        for (j = 0; j < minute_count; j++)
        {
            tbl_minutes[i][j].flags = readhex(fp);

            for (k = 0; k < max_minute_words; k++)
            {
                ch = readhex(fp);
                tbl_minutes[i][j].word_idx[k] = ch;

                if (ch == 0)
                {
                    break;
                }
            }
        }
    }

    fp.close ();

    tables_read = 1;
    return tables_read;
}

int
tables_init (void)
{
    Dir           dir;
    const char *  p;
    int           rtc = 0;

    fname[0] = '\0';

    SPIFFS.begin();

    dir = SPIFFS.openDir("");

    while (dir.next())
    {
        String str = dir.fileName();

        p = str.c_str ();

        if (! strncmp (p, "wc12h-tables-", 13) || ! strncmp (p, "wc24h-tables-", 13))
        {
            int len = strlen (p);
 
            if (len > 4 && ! strcmp (p + len - 4, ".txt"))
            {
                strncpy (fname, p, 32);
                break;
            }
        }
    }

    if (fname[0])
    {
        Serial.print ("- tables found; ");
        Serial.println (fname);

        if (checkfile (fname))
        {
            if (readfile (fname))
            {
                Serial.println ("TABLES");
                rtc = 1;
            }
            else
            {
                Serial.println ("- readfile failed");
            }
        }
        else
        {
            tables_corrupt = 1;
            Serial.println ("- checkfile failed");
        }
    }
    else
    {
        Serial.println ("- tables file not found");
    }

    SPIFFS.end();
    return rtc;
}

char *
tables_fname (void)
{
    if (tables_read)
    {
        return fname;
    }
    return (char *) NULL;
}

uint_fast8_t
tables_info (void)
{
    if (tables_read)
    {
        Serial.printf ("TABINFO %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\r\n",
                       wc_rows, wc_columns, wp_count, tbl_it_is[0], tbl_it_is[1], display_modes_count, hour_count, max_hour_words, minute_count, max_minute_words);
        Serial.flush ();
    }
    return tables_read;
}

uint_fast8_t
tables_illumination (uint_fast8_t idx)
{
    uint_fast8_t rtc = 0;

    if (tables_read && idx < wp_count)
    {
        Serial.printf ("TABILLU %02x%02x%02x%02x\r\n", idx, illumination[idx].row, illumination[idx].col, illumination[idx].len);
        rtc = 1;
    }

    return rtc;
}

uint_fast8_t
tables_hours (uint_fast8_t mode, uint_fast8_t hour)
{
    uint_fast8_t  hour_idx;
    uint_fast8_t  k;
    uint_fast8_t  ch;
    uint_fast8_t  rtc = 0;

    if (tables_read && mode < display_modes_count && hour < hour_count)
    {
        hour_idx = tbl_modes[mode].hour_idx;

        Serial.printf ("TABH %02x", hour);

        for (k = 0; k < max_hour_words; k++)
        {
            ch = tbl_hours[hour_idx][hour][k];
            Serial.printf ("%02x", ch);

            if (ch == 0)
            {
                break;
            }
        }

        Serial.print ("\r\n");
        rtc = 1;
    }
    return rtc;
}

uint_fast8_t
tables_minutes (uint_fast8_t mode, uint_fast8_t minute, const char * answer_key)
{
    uint_fast8_t  minute_idx;
    uint_fast8_t  k;
    uint_fast8_t  ch;
    uint_fast8_t  rtc = 0;

    if (tables_read && mode < display_modes_count && minute < minute_count)
    {
        minute_idx = tbl_modes[mode].minute_idx;
        Serial.print (answer_key);
        Serial.printf (" %02x%02x", minute, tbl_minutes[minute_idx][minute].flags);

        for (k = 0; k < max_minute_words; k++)
        {
            ch = tbl_minutes[minute_idx][minute].word_idx[k];
            Serial.printf ("%02x", ch);

            if (ch == 0)
            {
                break;
            }
        }

        Serial.print ("\r\n");
        rtc = 1;
    }
    return rtc;
}

