/*-------------------------------------------------------------------------------------------------------------------------------------------
 * wc.c - display tables in WEB interface
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

#define FNAME_WC24H_DE      "wc24h-tables-de.txt"
#define FNAME_WC24H_DE2     "wc24h-tables-de2.txt"
#define FNAME_WC24H_PL      "wc24h-tables-pl.txt"
#define FNAME_WC24H_PL2     "wc24h-tables-pl2.txt"
#define FNAME_WC12H_CH1     "wc12h-tables-ch1.txt"
#define FNAME_WC12H_CH2     "wc12h-tables-ch2.txt"
#define FNAME_WC12H_DE      "wc12h-tables-de.txt"
#define FNAME_WC12H_DE2     "wc12h-tables-de2.txt"
#define FNAME_WC12H_EN1     "wc12h-tables-en1.txt"
#define FNAME_WC12H_EN2     "wc12h-tables-en2.txt"
#define FNAME_WC12H_FR      "wc12h-tables-fr.txt"
#define FNAME_WC12H_ES      "wc12h-tables-es.txt"
#define FNAME_WC12H_IT      "wc12h-tables-it.txt"
#define FNAME_WC12H_SE      "wc12h-tables-se.txt"

#include "tables-gen.h"

static uint_fast8_t         version;
static uint_fast8_t         wc_rows;
static uint_fast8_t         wc_columns;
static uint_fast8_t         wp_count;
static WORD_ILLUMINATION *  illumination;

static uint_fast8_t         display_modes_count;
static DISPLAY_MODES *      tbl_modes;

static uint_fast8_t         hour_modes_count;
static uint_fast8_t         hour_count;
static uint_fast8_t         max_hour_words;
static uint8_t ***          tbl_hours;

static uint_fast8_t         minute_modes_count;
static uint_fast8_t         minute_count;
static uint_fast8_t         max_minute_words;
static MINUTEDISPLAY **     tbl_minutes;

static uint_fast8_t         show_it_is;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * hex to integer
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint16_t
htoi (char * buf, uint8_t max_digits)
{
    uint8_t     i;
    uint8_t     x;
    uint16_t    sum = 0;

    for (i = 0; i < max_digits && *buf; i++)
    {
        x = buf[i];

        if (x >= '0' && x <= '9')
        {
            x -= '0';
        }
        else if (x >= 'A' && x <= 'F')
        {
            x -= 'A' - 10;
        }
        else if (x >= 'a' && x <= 'f')
        {
            x -= 'a' - 10;
        }
        else
        {
            x = 0;
        }
        sum <<= 4;
        sum += x;
    }

    return (sum);
}

int
readhex (FILE * fp)
{
    char  buf[2];
    int   ch;

    do
    {
        ch = fgetc (fp);
    } while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t');

    if (ch == EOF)
    {
        return (EOF);
    }

    buf[0] = ch;
    ch = fgetc (fp);

    if (ch == EOF)
    {
        return (EOF);
    }
    
    buf[1] = ch;
    return htoi (buf, 2);
}

void
readfile (char * fname)
{
    FILE *          fp;
    int             i;
    int             j;
    int             k;
    uint_fast8_t    version_magic;
    int             h_offset;
    uint8_t         tbl_it_is[2];
    int             ch;
    

    fp = fopen (fname, "rb");

    if (! fp)
    {
        perror (fname);
        exit (1);
    }

    version_magic   = readhex (fp);

    if (version_magic == TABLES_VERSION_MAGIC)
    {
        version = readhex (fp);
        wc_rows = readhex (fp);
    }
    else
    {
        version = 0;
        wc_rows = version_magic;
    }

    wc_columns      = readhex (fp);
    wp_count        = readhex (fp);

    illumination = calloc (wp_count, sizeof (WORD_ILLUMINATION));

    for (i = 0; i < wp_count; i++)
    {
        illumination[i].row = readhex (fp);
        illumination[i].col = readhex (fp);
        illumination[i].len = readhex (fp);
    }

    if (version == 0)
    {
        tbl_it_is[0] = readhex (fp);                                    // old: tbl_it_is[0]

        if (tbl_it_is[0] > 0)
        {
            illumination[tbl_it_is[0]].len |= ILLUMINATION_FLAG_IT_IS;
        }

        tbl_it_is[1] = readhex (fp);                                    // old: tbl_it_is[1]

        if (tbl_it_is[1]  > 0)
        {
            illumination[tbl_it_is[1]].len |= ILLUMINATION_FLAG_IT_IS;
        }
    }
    else
    {
        tbl_it_is[0] = TABLES_VERSION_MAGIC;
        tbl_it_is[1] = version;
    }

    display_modes_count = readhex (fp);

    tbl_modes = calloc (display_modes_count, sizeof (DISPLAY_MODES));

    for (i = 0; i < display_modes_count; i++)
    {
        tbl_modes[i].hour_idx   = readhex (fp);
        tbl_modes[i].minute_idx = readhex (fp);

        j = 0;

        while ((ch = readhex (fp)) != '\0' && ch != EOF)
        {
            if (j < DISPLAY_MODE_TEXT_LEN)
            {
                tbl_modes[i].description[j++] = ch;
            }
        } 

        tbl_modes[i].description[j] = '\0';
    }

    hour_modes_count    = readhex (fp);
    hour_count          = readhex (fp);
    max_hour_words      = readhex (fp);

    if (version == 0)
    {
        if (tbl_it_is[0] > 0)
        {
            max_hour_words++;
        }

        if (tbl_it_is[1] > 0)
        {
            max_hour_words++;
        }
    }

    tbl_hours = calloc (hour_modes_count, sizeof (uint8_t **));

    for (i = 0; i < hour_modes_count; i++)
    {
        tbl_hours[i] = calloc (hour_count, sizeof (uint8_t *));

        for (j = 0; j < hour_count; j++)
        {
            tbl_hours[i][j] = calloc (max_hour_words, sizeof (uint8_t));

            h_offset = 0;

            if (version == 0)
            {
                if (tbl_it_is[0] > 0)
                {
                    tbl_hours[i][j][h_offset] = tbl_it_is[0];
                    h_offset++;
                }

                if (tbl_it_is[1] > 0)
                {
                    tbl_hours[i][j][h_offset] = tbl_it_is[1];
                    h_offset++;
                }
            }

            for (k = h_offset; k < max_hour_words; k++)
            {
                ch = readhex (fp);
                tbl_hours[i][j][k] = ch;

                if (ch == 0)
                {
                    break;
                }
            }
        }
    }

    minute_modes_count  = readhex (fp);
    minute_count        = readhex (fp);
    max_minute_words    = readhex (fp);

    tbl_minutes = calloc (minute_modes_count, sizeof (MINUTEDISPLAY *));

    for (i = 0; i < minute_modes_count; i++)
    {
        tbl_minutes[i] = calloc (minute_count, sizeof (MINUTEDISPLAY));

        for (j = 0; j < minute_count; j++)
        {
            tbl_minutes[i][j].flags = readhex (fp);

            for (k = 0; k < max_minute_words; k++)
            {
                ch = readhex (fp);
                tbl_minutes[i][j].word_idx[k] = ch;

                if (ch == 0)
                {
                    break;
                }
            }
        }
    }

    fclose (fp);
}

#define OFF         0
#define ON          1

static const char * display_wc24h_de =
{
    "ES#IST#VIERTELEINS"
    "DREINERSECHSIEBEN#"
    "ELFÜNFNEUNVIERACHT"
    "NULLZWEI#ZWÖLFZEHN"
    "UND#ZWANZIGVIERZIG"
    "DREISSIGFÜNFZIGUHR"
    "MINUTEN#VORUNDNACH"
    "EINDREIVIERTELHALB"
    "SIEBENEUNULLZWEINE"
    "FÜNFSECHSNACHTVIER"
    "DREINSUND#ELF#ZEHN"
    "ZWANZIGGRADREISSIG"
    "VIERZIGZWÖLFÜNFZIG"
    "MINUTENUHR#FRÜHVOR"
    "ABENDSMITTERNACHTS"
    "MORGENSWARMMITTAGS"
};

static const char * display_wc24h_de2 =
{
    "ES#IST#NULL#EIN"
    "ZWEI#DREI#VIER#"
    "#FÜNF###SECHS##"
    "SIEBEN###ACHT##"
    "NEUN##ZEHN##ELF"
    "#ZWÖLF###UND###"
    "ZWANZIG#UHR#UND"
    "EINE#ZWEI#DREI#"
    "VIER#FÜNF#SECHS"
    "##SIEBEN#ACHT##"
    "#NEUN#ZEHN#ELF#"
    "###ZWÖLF##UND##"
    "ZWANZIG#DREIßIG"
    "#VIERZIG##NULL#"
    "FÜNFZIG#MINUTEN"
};

static const char * display_wc24h_pl =
{
    "JEST*TRZECIASZOSTA"
    "DZIEWIATADZIESIATA"
    "SIODMACZWARTAOSMA*"
    "PIATADRUGAPIERWSZA"
    "DWUNASTAJEDENASTA*"
    "*PIECDZIESIAT*****"
    "****DWADZIESCIA***"
    "ZEROCZTERDZIESCI**"
    "*TRZYDZIESCI**ZERO"
    "DZIESIECTRZYNASCIE"
    "JEDENASCIEDZIEWIEC"
    "*****SZESNASCIE***"
    "***DZIEWIETNASCIE*"
    "CZTERNASCIE*SZESC*"
    "******PIETNASCIE**"
    "OSIEMNASCIE*CZTERY"
    "PIECDWANASCIE*****"
    "SIEDEMNASCIE*PM*AM"
};

static const char * display_wc24h_pl2 =
{
    "JEST*SIEDEMNASTAZERO"
    "SIODMADZIESIATA*OSMA"
    "*CZWARTA*DWUDZIESTA*"
    "DRUGA*SZOSTA*TRZECIA"
    "TRZYNASTA**DWUNASTA*"
    "OSIEMNASTA*PIETNASTA"
    "JEDENASTA*SZESNASTA*"
    "*DZIEWIETNASTA*PIATA"
    "PIERWSZA*CZTERNASTA*"
    "DZIEWIATA*SZESNASCIE"
    "TRZYDZIESCI*DZIESIEC"
    "ZERO***DWADZIESCIA**"
    "PIETNASCIEJEDENASCIE"
    "***CZTERDZIESCI*ZERO"
    "*PIECDZIESIAT**JEDEN"
    "OSIEMNASCIEPIECSZESC"
    "*CZTERY***TRZYNASCIE"
    "SIEDEMNASCIEDZIEWIEC"
    "CZTERNASCIEDWANASCIE"
    "***DZIEWIETNASCIE*C*"
};

static const char * display_wc12h_ch1 =
{
    "ESKISCHAFÜF"
    "VIERTUBFZÄÄ"
    "ZWÄNZGSIVOR"
    "ABOHAUBIEPM"
    "EISZWÖISDRÜ"
    "VIERIFÜFIQT"
    "SÄCHSISIBNI"
    "ACHTINÜNIEL"
    "ZÄNIERBEUFI"
    "ZWÖUFIAMUHR"
};

static const char * display_wc12h_ch2 =
{
    "ESKESCHAZÄÄ"
    "FÖIFCVIERTU"
    "ZWÄNZGSIVOR"
    "ABOHAUBIEGE"
    "EISZWÖISDRÜ"
    "VIERITFÖIFI"
    "SÄCHSISEBNI"
    "ACHTIENÜNIL"
    "ZÄNIERBELFI"
    "ZWÖLFINAUHR"
};

static const char * display_wc12h_de =
{
    "ESKISTLFÜNF"
    "ZEHNZWANZIG"
    "DREIVIERTEL"
    "TGNACHVORJM"
    "HALBQZWÖLFP"
    "ZWEINSIEBEN"
    "KDREIRHFÜNF"
    "ELFNEUNVIER"
    "WACHTZEHNRS"
    "BSECHSFMUHR"
};

static const char * display_wc12h_de2 =
{
    "ES*IST*FÜNF"
    "ZEHNZWANZIG"
    "DREIVIERTEL"
    "VOR****NACH"
    "HALB*ELFÜNF"
    "EINS***ZWEI"
    "DREI***VIER"
    "SECHS**ACHT"
    "SIEBENZWÖLF"
    "ZEHN****UHR"
};

static const char * display_wc12h_en1 =
{
    "ITLISASAMPM"
    "ACQUARTERDC"
    "TWENTYFIVEX"
    "HALFSTENFTO"
    "PASTERUNINE"
    "ONESIXTHREE"
    "FOURFIVETWO"
    "EIGHTELEVEN"
    "SEVENTWELVE"
    "TENSEOCLOCK"
};

static const char * display_wc12h_en2 =
{
    "ITKISGHALFE"
    "TENYQUARTER"
    "DTWENTYFIVE"
    "TOPASTEFOUR"
    "FIVETWONINE"
    "THREETWELVE"
    "BELEVENONES"
    "SEVENWEIGHT"
    "ITENSIXTIES"
    "TINEOICLOCK"
};

static const char * display_wc12h_fr =
{
    "ILNESTODEUX"
    "QUATRETROIS"
    "NEUFUNESEPT"
    "HUITSIXCINQ"
    "MIDIXMINUIT"
    "ONZERHEURES"
    "MOINSOLEDIX"
    "ETRQUARTPMD"
    "VINGT-CINQU"
    "ETSDEMIEPAM"
};

static const char * display_wc12h_es =
{
    "ESONELASUNA"
    "DOSITRESOAM"
    "CUATROCINCO"
    "SEISASIETEN"
    "OCHONUEVEPM"
    "LADIEZSONCE"
    "DOCELYMENOS"
    "OVEINTEDIEZ"
    "VEINTICINCO"
    "MEDIACUARTO"
};

static const char * display_wc12h_it =
{
    "SONORLEBORE"
    "ERLUNASDUEZ"
    "TREOTTONOVE"
    "DIECIUNDICI"
    "DODICISETTE"
    "QUATTROCSEI"
    "CINQUEAMENO"
    "EKUNLQUARTO"
    "VENTICINQUE"
    "DIECILMEZZA"
};

static const char * display_wc12h_se =
{
    "KLOCKANTÄRK"
    "FEMYISTIONI"
    "KVARTQIENZO"
    "TJUGOLIVIPM"
    "ÖVERKAMHALV"
    "ETTUSVLXTVA"
    "TREMYKYFYRA"
    "FEMSFLORSEX"
    "SJUÄTTAINIO"
    "TIOELVATOLV"
};

static const char *     display;
static char *           wcname;

static unsigned char ** front;
static unsigned char ** fronton;

static void
show_letter (uint8_t y, uint8_t x, uint8_t onoff)
{
    front[y][x] = display[wc_columns * y + x];
    fronton[y][x] = onoff;
}

static void
show_table (void)
{
    uint8_t y;
    uint8_t x;

    for (y = 0; y < wc_rows; y++)
    {
        for (x = 0; x < wc_columns; x++)
        {
            show_letter (y, x, OFF);
        }
    }
}

static void
show_word (uint8_t i, uint_fast8_t pm_mode, uint_fast8_t it_is_mode, uint_fast8_t full_or_half_hour)
{
    uint8_t row             = illumination[i].row;
    uint8_t col             = illumination[i].col;
    uint8_t len             = illumination[i].len & ILLUMINATION_LEN_MASK;
    uint8_t is_it_is_word   = illumination[i].len & ILLUMINATION_FLAG_IT_IS;
    uint8_t is_am_word      = illumination[i].len & ILLUMINATION_FLAG_AM;
    uint8_t is_pm_word      = illumination[i].len & ILLUMINATION_FLAG_PM;
    uint8_t do_show         = 1;

    if (! full_or_half_hour && is_it_is_word && ((! show_it_is) || (! it_is_mode)))
    {
        do_show = 0;
    }
    else
    {
        if (pm_mode == 0)
        {
            if (is_pm_word)
            {
                do_show = 0;
            }
        }
        else // if (pm_mode == 1)
        {
            if (is_am_word)
            {
                do_show = 0;
            }
        }
    }

    if (do_show)
    {
        while (len--)
        {
            show_letter (row, col, 1);
            col++;
        }
    }
}

static void
show_clock (uint8_t mode, uint8_t hour, uint8_t minute)
{
    static uint8_t *        words;
    const MINUTEDISPLAY *   tbl_minute;
    uint8_t                 hour_mode;
    uint8_t                 minute_mode;
    uint_fast8_t            it_is_mode;
    uint8_t *               word_idx_p;
    int                     is_midnight = 0;
    int                     pm_mode = 0;
    uint8_t                 idx;

    if (! words)
    {
        words = calloc (wp_count, sizeof (uint8_t));
    }

    hour_mode = tbl_modes[mode].hour_idx;
    minute_mode = tbl_modes[mode].minute_idx;

    printf ("%s %02d:%02d&nbsp;&nbsp;<pre>", wcname, hour, minute);
    memset (words, 0, wp_count);
    show_table ();

    // get the minute words from minute table
    if (minute_count == 12)
    {
        tbl_minute  = &tbl_minutes[minute_mode][minute / 5];
    }
    else
    {
        tbl_minute = &tbl_minutes[minute_mode][minute];
    }

    it_is_mode = tbl_minute->flags & MDF_IT_IS_1;

    for (idx = 0; idx < max_minute_words && tbl_minute->word_idx[idx] != 0; idx++)
    {
        words[tbl_minute->word_idx[idx]] = 1;
    }

    if (hour >= 12)
    {
        pm_mode = 1;                                            // it's PM
    }

    if (tbl_minute->flags & MDF_HOUR_OFFSET_1)
    {
        hour += 1;                                              // correct hour offset
    }
    else if (tbl_minute->flags & MDF_HOUR_OFFSET_2)             // only used in jester mode
    {
        hour += 2;                                              // correct hour offset
    }

    if (hour == 0 || hour == 24)                                // we have midnight
    {
        is_midnight = 1;
    }

    while (hour >= hour_count)                                  // hour: 25 -> 13 -> 01
    {
        hour -= hour_count;
    }

    word_idx_p = tbl_hours[hour_mode][hour];                    // get the hour words from hour table

    for (idx = 0; idx < MAX_HOUR_WORDS && word_idx_p[idx] != 0; idx++)
    {
        if (word_idx_p[idx] == WP_IF_MINUTE_IS_0)               // if minute is null take word index + 1, otherwise word index + 2
        {                                                       // this handles "EIN UHR" instead of "EINS UHR" at 01:00 & 13:00
            if (minute == 0)
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

    for (idx = 0; idx < wp_count; idx++)
    {
        if (words[idx])
        {
            show_word (idx, pm_mode, it_is_mode, minute == 30 || minute == 0);
        }
    }

    puts ("</pre><P>");
}

static void
show_mode (int mode)
{
    printf ("<small>Mode %d: %s</small>", mode, tbl_modes[mode].description);
}

char *
to_html (int ch)
{
    static char str[16];

    switch (ch)
    {
        case 0xc4: strcpy (str, "&Auml"); break;
        case 0xd6: strcpy (str, "&Ouml"); break;
        case 0xdc: strcpy (str, "&Uuml"); break;
        case 0xdf: strcpy (str, "&szlig"); break;
        default : str[0] = ch; str[1] = '\0'; break;
    }
    return str;
}

int
main (int argc, char ** argv)
{
    char *  fname;
    char *  url;
    uint8_t y;
    uint8_t x;
    uint8_t hour        = 0;
    uint8_t minute      = 0;
    uint8_t mode        = 0;
    uint8_t i;
    uint8_t step;

    if (argc != 7)
    {
        return 1;
    }

    url = argv[1];

    wcname = argv[2];

    if (! strcmp (wcname, "wc12h-ch1"))
    {
        fname = FNAME_WC12H_CH1;
        display = display_wc12h_ch1;
    }
    else if (! strcmp (wcname, "wc12h-ch2"))
    {
        fname = FNAME_WC12H_CH2;
        display = display_wc12h_ch2;
    }
    else if (! strcmp (wcname, "wc12h-de"))
    {
        fname = FNAME_WC12H_DE;
        display = display_wc12h_de;
    }
    else if (! strcmp (wcname, "wc12h-de2"))
    {
        fname = FNAME_WC12H_DE2;
        display = display_wc12h_de2;
    }
    else if (! strcmp (wcname, "wc12h-en1"))
    {
        fname = FNAME_WC12H_EN1;
        display = display_wc12h_en1;
    }
    else if (! strcmp (wcname, "wc12h-en2"))
    {
        fname = FNAME_WC12H_EN2;
        display = display_wc12h_en2;
    }
    else if (! strcmp (wcname, "wc12h-es"))
    {
        fname = FNAME_WC12H_ES;
        display = display_wc12h_es;
    }
    else if (! strcmp (wcname, "wc12h-fr"))
    {
        fname = FNAME_WC12H_FR;
        display = display_wc12h_fr;
    }
    else if (! strcmp (wcname, "wc12h-it"))
    {
        fname = FNAME_WC12H_IT;
        display = display_wc12h_it;
    }
    else if (! strcmp (wcname, "wc12h-se"))
    {
        fname = FNAME_WC12H_SE;
        display = display_wc12h_se;
    }
    else if (! strcmp (wcname, "wc24h-pl"))
    {
        fname = FNAME_WC24H_PL;
        display = display_wc24h_pl;
        wcname = "wc24h-pl";
    }
    else if (! strcmp (wcname, "wc24h-pl2"))
    {
        fname = FNAME_WC24H_PL2;
        display = display_wc24h_pl2;
        wcname = "wc24h-pl2";
    }
    else if (! strcmp (wcname, "wc24h-de"))
    {
        fname = FNAME_WC24H_DE;
        display = display_wc24h_de;
        wcname = "wc24h-de";
    }
    else if (! strcmp (wcname, "wc24h-de2"))
    {
        fname = FNAME_WC24H_DE2;
        display = display_wc24h_de2;
        wcname = "wc24h-de2";
    }
    else
    {
        fprintf (stderr, "unknown layout name: %s\n", wcname);
        return 1;
    }

    readfile (fname);

    mode = atoi (argv[3]);

    if (mode >= display_modes_count)
    {
        mode = 0;
    }

    hour = atoi (argv[4]);

    hour %= 24;

    minute = atoi (argv[5]);

    if (minute_count == 12)
    {
        minute = 5 * (minute / 5);
    }

    minute %= 60;

    show_it_is = atoi (argv[6]);

    front = calloc (wc_rows, sizeof (unsigned char *));

    for (i = 0; i < wc_rows; i++)
    {
        front[i] = calloc (wc_columns, sizeof (unsigned char));
    }

    fronton = calloc (wc_rows, sizeof (unsigned char *));

    for (i = 0; i < wc_rows; i++)
    {
        fronton[i] = calloc (wc_columns, sizeof (unsigned char));
    }

    puts ("<html>");

    puts ("  <head>\n");
    puts ("    <link rel=\"stylesheet\" href=\"/styles.css\">\n");
    puts ("    <title>WC24h</title>\n");
    puts ("  </head>\n");
    puts ("  <body>\n");

    puts ("<table border=0 cellspacing=0>");
    puts ("<tr valign=top><td width=120px>");

    if (minute_count == 12)
    {
        step = 5;
    }
    else
    {
        step = 1;
    }

    if (! strcmp (wcname, "wc24h-de"))
    {
        printf ("<a href=%s?w=wc24h-de><font color=red>wc24h-de</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc24h-de>wc24h-de</a><BR>", url);
    }

    if (! strcmp (wcname, "wc24h-de2"))
    {
        printf ("<a href=%s?w=wc24h-de2><font color=red>wc24h-de2</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc24h-de2>wc24h-de2</a><BR>", url);
    }

    if (! strcmp (wcname, "wc24h-pl"))
    {
        printf ("<a href=%s?w=wc24h-pl><font color=red>wc24h-pl</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc24h-pl>wc24h-pl</a><BR>", url);
    }

    if (! strcmp (wcname, "wc24h-pl2"))
    {
        printf ("<a href=%s?w=wc24h-pl2><font color=red>wc24h-pl2</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc24h-pl2>wc24h-pl2</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-de"))
    {
        printf ("<a href=%s?w=wc12h-de><font color=red>wc12h-de</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-de>wc12h-de</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-de2"))
    {
        printf ("<a href=%s?w=wc12h-de2><font color=red>wc12h-de2</font></a><BR>\n", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-de2>wc12h-de2</a><BR>\n", url);
    }

    if (! strcmp (wcname, "wc12h-ch1"))
    {
        printf ("<a href=%s?w=wc12h-ch1><font color=red>wc12h-ch1</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-ch1>wc12h-ch1</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-ch2"))
    {
        printf ("<a href=%s?w=wc12h-ch2><font color=red>wc12h-ch2</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-ch2>wc12h-ch2</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-en1"))
    {
        printf ("<a href=%s?w=wc12h-en1><font color=red>wc12h-en1</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-en1>wc12h-en1</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-en2"))
    {
        printf ("<a href=%s?w=wc12h-en2><font color=red>wc12h-en2</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-en2>wc12h-en2</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-es"))
    {
        printf ("<a href=%s?w=wc12h-es><font color=red>wc12h-es</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-es>wc12h-es</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-fr"))
    {
        printf ("<a href=%s?w=wc12h-fr><font color=red>wc12h-fr</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-fr>wc12h-fr</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-it"))
    {
        printf ("<a href=%s?w=wc12h-it><font color=red>wc12h-it</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-it>wc12h-it</a><BR>", url);
    }

    if (! strcmp (wcname, "wc12h-se"))
    {
        printf ("<a href=%s?w=wc12h-se><font color=red>wc12h-se</font></a><BR>", url);
    }
    else
    {
        printf ("<a href=%s?w=wc12h-se>wc12h-se</a><BR>", url);
    }


    puts ("</td><td valign=top width=230px>");

    for (i = 0; i < display_modes_count; i++)
    {
        if (i == mode)
        {
            printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d><font color=red>%s</font></a><BR>", url, wcname, i, hour, minute, tbl_modes[i].description);
        }
        else
        {
            printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d>%s</a><BR>", url, wcname, i, hour, minute, tbl_modes[i].description);
        }
    }

    puts ("<BR>");
    printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d&i=%d>Stunde -</a>&nbsp;&nbsp;&nbsp;", url, wcname, mode, hour > 0 ? hour - 1 : 23, minute, show_it_is);
    printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d&i=%d>Stunde +</a>&nbsp;&nbsp;&nbsp;", url, wcname, mode, hour < 23 ? hour + 1 : 0, minute, show_it_is);
    puts ("<BR>");
    printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d&i=%d>Minute -</a>&nbsp;&nbsp;&nbsp;", url, wcname, mode, hour, minute >= step ? minute - step : 60 - step, show_it_is);
    printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d&i=%d>Minute +</a>&nbsp;&nbsp;&nbsp;", url, wcname, mode, hour, minute <= 60 - step ? minute + step : 0, show_it_is);
    puts ("<BR>");

    if (show_it_is)
    {
        printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d&i=0>NO IT IS</a>&nbsp;&nbsp;&nbsp;", url, wcname, mode, hour, minute);
    }
    else
    {
        printf ("<a href=%s?w=%s&x=%d&h=%d&m=%d&i=1>IT IS</a>&nbsp;&nbsp;&nbsp;", url, wcname, mode, hour, minute);
    }

    puts ("</td><td>");
    show_table ();
    show_clock (mode, hour, minute);
    show_mode (mode);

    puts ("<table border=0 cellspacing=0>");

    for (y = 0; y < wc_rows; y++)
    {
        puts ("<tr>");

        for (x = 0; x < wc_columns; x++)
        {
            printf ("<td style=\"padding-left:3px;padding-right:3px;\" align=center bgcolor=black><font color=%s>%s</font></td>\n",
                    fronton[y][x]? "red" : "grey", to_html (front[y][x]));
        }

        puts ("</tr>");
    }

    puts ("</table>");

    puts ("</td></tr>");
    puts ("</table>");

    puts ("</body>");
    puts ("</html>");

    return 0;
}
