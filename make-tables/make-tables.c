/*-------------------------------------------------------------------------------------------------------------------------------------------
 * make-tables.c - generate tables
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(WCLOCK24H_DE)
#include "tables24h-de.h"
#define FNAME   "wc24h-tables-de.txt"

#elif defined(WCLOCK24H_DE2)
#include "tables24h-de2.h"
#define FNAME   "wc24h-tables-de2.txt"

#elif defined(WCLOCK24H_PL)
#include "tables24h-pl.h"
#define FNAME   "wc24h-tables-pl.txt"

#elif defined(WCLOCK24H_PL2)
#include "tables24h-pl2.h"
#define FNAME   "wc24h-tables-pl2.txt"

#elif defined(WCLOCK12H_CH1)
#include "tables12h-ch1.h"
#define FNAME   "wc12h-tables-ch1.txt"

#elif defined(WCLOCK12H_CH2)
#include "tables12h-ch2.h"
#define FNAME   "wc12h-tables-ch2.txt"

#elif defined(WCLOCK12H_DE)
#include "tables12h-de.h"
#define FNAME   "wc12h-tables-de.txt"

#elif defined(WCLOCK12H_DE2)
#include "tables12h-de2.h"
#define FNAME   "wc12h-tables-de2.txt"

#elif defined(WCLOCK12H_EN1)
#include "tables12h-en1.h"
#define FNAME   "wc12h-tables-en1.txt"

#elif defined(WCLOCK12H_EN2)
#include "tables12h-en2.h"
#define FNAME   "wc12h-tables-en2.txt"

#elif defined(WCLOCK12H_FR)
#include "tables12h-fr.h"
#define FNAME   "wc12h-tables-fr.txt"

#elif defined(WCLOCK12H_ES)
#include "tables12h-es.h"
#define FNAME   "wc12h-tables-es.txt"

#elif defined(WCLOCK12H_IT)
#include "tables12h-it.h"
#define FNAME   "wc12h-tables-it.txt"

#elif defined(WCLOCK12H_SE)
#include "tables12h-se.h"
#define FNAME   "wc12h-tables-se.txt"

#else
#error "no language defined"
#endif

#include "tables-gen.h"
#include "tables-ext.h"


static uint_fast8_t     newline_written = 0;

static void
fputcrnl (FILE * fp)
{
    if (! newline_written)
    {
        fputc ('\r', fp);
        fputc ('\n', fp);
        newline_written = 1;
    }
}

static void
fputhex (uint_fast8_t ch, FILE * fp)
{
    static int cnt = 0;

    fprintf (fp, "%02x", ch);
    newline_written = 0;

    cnt++;

    if (cnt == 16)
    {
        fputcrnl (fp);
        cnt = 0;
    }
}

int
main ()
{
    char *          fname = FNAME;
    const char *    p;
    FILE *          fp;
    int             i;
    int             j;
    int             k;

    fp = fopen (fname, "wb");

    if (! fp)
    {
        perror (fname);
        exit (1);
    }

    fputhex (TABLES_VERSION_MAGIC, fp);                     // Version Header
    fputhex (TABLES_VERSION, fp);                           // current version

    fputhex (WC_ROWS, fp);
    fputhex (WC_COLUMNS, fp);

    fputhex (WP_COUNT, fp);

    for (i = 0; i < WP_COUNT; i++)
    {
        fputhex (illumination[i].row, fp);
        fputhex (illumination[i].col, fp);
        fputhex (illumination[i].len, fp);
    }

    fputhex (DISPLAY_MODES_COUNT, fp);

    for (i = 0; i < DISPLAY_MODES_COUNT; i++)
    {
        fputhex (tbl_modes[i].hour_idx, fp);
        fputhex (tbl_modes[i].minute_idx, fp);

        for (p = tbl_modes[i].description; *p; p++)
        {
            fputhex (*p, fp);
        }
        fputhex ('\0', fp);
    }

    fputhex (HOUR_MODES_COUNT, fp);
    fputhex (HOUR_COUNT, fp);
    fputhex (MAX_HOUR_WORDS, fp);

    for (i = 0; i < HOUR_MODES_COUNT; i++)
    {
        for (j = 0; j < HOUR_COUNT; j++)
        {
            for (k = 0; k < MAX_HOUR_WORDS; k++)
            {
                fputhex (tbl_hours[i][j][k], fp);

                if (tbl_hours[i][j][k] == WP_END_OF_WORDS)
                {
                    break;
                }
            }
        }
    }

    fputhex (MINUTE_MODES_COUNT, fp);
    fputhex (MINUTE_COUNT, fp);
    fputhex (MAX_MINUTE_WORDS, fp);

    for (i = 0; i < MINUTE_MODES_COUNT; i++)
    {
        for (j = 0; j < MINUTE_COUNT; j++)
        {
            fputhex (tbl_minutes[i][j].flags, fp);

            for (k = 0; k < MAX_MINUTE_WORDS; k++)
            {
                fputhex (tbl_minutes[i][j].word_idx[k], fp);

                if (tbl_minutes[i][j].word_idx[k] == WP_END_OF_WORDS)
                {
                    break;
                }
            }
        }
    }

    fputcrnl (fp);
    fclose (fp);
    return 0;
}
