/*----------------------------------------------------------------------------------------------------------------------------------------
 * display.cpp - display layout functions (TFT version)
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
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
#include <LittleFS.h>
#include "display.h"
#include "base.h"

#define MAX_LAYOUT_SIZE       (20*20)

static char                 fname[64];
static uint8_t              layout_table[MAX_LAYOUT_SIZE];
static uint32_t             layout_size = 0;
static uint32_t             display_layout_read = 0;


static int
readchar (File fp)
{
    int   ch;

    do
    {
        ch = fp.read();
    } while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t');

    return ch;
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * readfile
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static int
readfile (const char * fname)
{
    File            fp;
    int             ch;

    fp = LittleFS.open (fname, "r");

    if (! fp)
    {
        Serial.println ("- open failed");
        return 0;
    }

    layout_size = 0;

    while ((ch = readchar (fp)) != EOF)
    {
        if (layout_size < MAX_LAYOUT_SIZE)
        {
            layout_table[layout_size++] = ch;
        }
    }

    display_layout_read = 1;
    return 1;
}

int
display_layout_init (void)
{
    Dir           dir;
    const char *  p;
    int           rtc = 0;

    fname[0] = '\0';

    LittleFS.begin();

    dir = LittleFS.openDir("");

    while (dir.next())
    {
        String str = dir.fileName();

        p = str.c_str ();

        if (! strncmp (p, "wc12h-display-", 14) || ! strncmp (p, "wc24h-display-", 14))
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
        Serial.print ("- display layout found: ");
        Serial.println (fname);

        if (readfile (fname))
        {
            rtc = 1;
        }
        else
        {
            Serial.println ("- readfile failed");
        }
    }
    else
    {
        Serial.println ("- info: TFT display layout file not found");
    }

    LittleFS.end();
    return rtc;
}

char *
display_layout_fname (void)
{
    if (display_layout_read)
    {
        return fname;
    }
    return (char *) NULL;
}

uint_fast8_t
display_layout_values (uint_fast8_t cur_line, uint_fast8_t cols)
{
    uint32_t    offset = cur_line * cols;
    uint32_t    idx;

    if (display_layout_read)
    {
        Serial.printf ("DISP %02X", cur_line);

        for (idx = 0; idx < cols; idx++)
        {
            if (offset + idx < layout_size)
            {
                uint8_t ch = layout_table[offset + idx];
                Serial.write (ch);
            }
        }

        Serial.print ("\r\n");
        Serial.flush ();
    }
    return display_layout_read;
}
