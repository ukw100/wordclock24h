/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * base.c - base routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include "base.h"

#ifdef unix
#define log_printf printf
#else
#include "log.h"
#endif

const char *                wdays_en[7] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };
const char *                wdays_de[7] = { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" };

#define SUNDAY               0                                              // first day of a unix week
#define HOURS_PER_DAY       24                                              // hours per day
#define DAYS_PER_WEEK        7                                              // days per week

#define IS_LEAP_YEAR(y)     ((((y) % 4) == 0) && ((((y) % 100) != 0) || (((y) % 400) == 0)))

static int                  g_days_per_month[]    = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define INT(x)              (x)                                             // integer function

#define TO_MMDD(mm,dd)      ((mm) << 8 | (dd))

static uint_fast16_t        date_codes[N_DATE_CODES];

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * add_days () - add n days
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast16_t
add_days (uint_fast16_t mmdd, int start_year, int n_days)
{
    int start_day;
    int start_month;
    int days_this_month;

    start_month = (mmdd >> 8) & 0xFF;
    start_day   = mmdd & 0xFF;

    while (n_days > 0)
    {
        start_day += n_days;

        days_this_month = g_days_per_month[start_month - 1];

        if (start_month == 1 && IS_LEAP_YEAR(start_year))
        {
            days_this_month++;
        }

        if (start_day > days_this_month)
        {
            n_days      = start_day - days_this_month - 1;
            start_day   = 1;
            start_month++;

            if (start_month > 12)
            {
                start_month = 1;
                start_year++;
            }
        }
        else
        {
            n_days = 0;
        }
    }

    while (n_days < 0)
    {
        start_day += n_days;

        if (start_day <= 0)
        {
            n_days = start_day;

            start_month--;

            if (start_month == 0)
            {
                start_month = 12;
                start_year--;
            }

            days_this_month = g_days_per_month[start_month - 1];

            if (start_month == 1 && IS_LEAP_YEAR(start_year))
            {
                days_this_month++;
            }

            start_day = days_this_month;
        }
        else
        {
            n_days = 0;
        }
    }

    return (start_month << 8) | start_day;
} // add_days (start_day, start_month, start_year, n_days, new_day_p, new_month_p)

/*--------------------------------------------------------------------------------------------------------------------------------------
 * get day of week (0=Sunday, 1=Monday, ... 6=Saturday)
 *
 *  day         - day of month
 *  month       - month beginning with 1
 *  year        - greater than 2000
 *
 *  example:    int rtc = dayofweek (tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
int
dayofweek (int d, int m, int y)
{
   return (d += m < 3 ? y-- : y - 2 , 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * get days of month
 *
 *  month       - month beginning with 1
 *  year        - greater than 2000
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
days_of_month (uint_fast8_t month, uint_fast16_t year)
{
    uint_fast8_t    days = g_days_per_month[month - 1];

    if (month == 2 && IS_LEAP_YEAR(year))
    {
        days++;
    }
    return days;
}

static unsigned int
get_easter (int year)
{
    int         a, b, c, d, e;
    int         m, s, M, N, D;
    int         offset;
    int         ostern_day;
    int         ostern_month;

    a = year % 19;
    b = year % 4;
    c = year % 7;

    m = INT ((8 * INT (year/100) + 13) / 25) - 2;
    s = INT (year/100) - INT (year/400) - 2;

    M = (15 + s - m) % 30;
    N = (6 + s) % 7;

    d = (M + 19 * a) % 30;

    if (d == 29)
    {
        D = 28;
    }
    else if (d == 28 && a >= 11)
    {
        D = 27;
    }
    else
    {
        D = d;
    }

    e = (2 * b + 4 * c + 6 * D + N) % 7;

    offset = D + e + 1;

    ostern_day = 21 + offset;
    ostern_month = 3;

    while (ostern_day > 31)
    {
        ostern_day -= 31;
        ostern_month++;
    }

    return (ostern_month << 8) | ostern_day;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * init_date_codes () - set date codes for special days
 *
 *  day         - day of month
 *  month       - month beginning with 1
 *  year        - greater than 2000
 *
 *  example:    int rtc = is_holiday (tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
init_date_codes (int year)
{
    static int      last_year;
    static int      day_of_xmas;
    uint_fast16_t   mmdd;

    if (last_year != year)
    {
        last_year = year;

        mmdd = get_easter (year);

        date_codes[DATE_CODE_NEW_YEAR]          = TO_MMDD( 1,  1);                      // Neujahr
        date_codes[DATE_CODE_THREE_MAGI]        = TO_MMDD( 1,  6);                      // Heilige drei Könige
        date_codes[DATE_CODE_FIRST_MAY]         = TO_MMDD( 5,  1);                      // Maifeiertag
        date_codes[DATE_CODE_GERMANY_UNITY_DAY] = TO_MMDD(10,  3);                      // Tag der deutschen Einheit
        date_codes[DATE_CODE_CHRISTMAS_DAY1]    = TO_MMDD(12, 25);                      // 1. Weihnachtsfeiertag
        date_codes[DATE_CODE_CHRISTMAS_DAY2]    = TO_MMDD(12, 26);                      // 1. Weihnachtsfeiertag

        date_codes[DATE_CODE_CARNIVAL_MONDAY]   = add_days (mmdd, year, -48);
        date_codes[DATE_CODE_GOOD_FRIDAY]       = add_days (mmdd, year,  -2);
        date_codes[DATE_CODE_EASTER_SUNDAY]     = add_days (mmdd, year,   0);
        date_codes[DATE_CODE_EASTER_MONDAY]     = add_days (mmdd, year,   1);
        date_codes[DATE_CODE_ASCENSION_DAY]     = add_days (mmdd, year,  39);
        date_codes[DATE_CODE_PENTECOST_SUNDAY]  = add_days (mmdd, year,  49);
        date_codes[DATE_CODE_PENTECOST_MONDAY]  = add_days (mmdd, year,  50);
        date_codes[DATE_CODE_CORPUS_CHRISTI]    = add_days (mmdd, year,  60);

        day_of_xmas = dayofweek (25, 12, year);
        date_codes[DATE_CODE_ADVENT1] = add_days (TO_MMDD(12, 25), year, -day_of_xmas - 21);
        date_codes[DATE_CODE_ADVENT2] = add_days (TO_MMDD(12, 25), year, -day_of_xmas - 14);
        date_codes[DATE_CODE_ADVENT3] = add_days (TO_MMDD(12, 25), year, -day_of_xmas -  7);
        date_codes[DATE_CODE_ADVENT4] = add_days (TO_MMDD(12, 25), year, -day_of_xmas);

        debug_log_printf ("new year            %2d %04d-%02u-%02u\r\n", DATE_CODE_NEW_YEAR, year, date_codes[DATE_CODE_NEW_YEAR] >> 8, date_codes[DATE_CODE_NEW_YEAR] & 0xFF);
        debug_log_printf ("three magi          %2d %04d-%02u-%02u\r\n", DATE_CODE_THREE_MAGI, year, date_codes[DATE_CODE_THREE_MAGI] >> 8, date_codes[DATE_CODE_THREE_MAGI] & 0xFF);
        debug_log_printf ("first may           %2d %04d-%02u-%02u\r\n", DATE_CODE_FIRST_MAY, year, date_codes[DATE_CODE_FIRST_MAY] >> 8, date_codes[DATE_CODE_FIRST_MAY] & 0xFF);
        debug_log_printf ("germany unity day   %2d %04d-%02u-%02u\r\n", DATE_CODE_GERMANY_UNITY_DAY, year, date_codes[DATE_CODE_GERMANY_UNITY_DAY] >> 8, date_codes[DATE_CODE_GERMANY_UNITY_DAY] & 0xFF);
        debug_log_printf ("christmas day1      %2d %04d-%02u-%02u\r\n", DATE_CODE_CHRISTMAS_DAY1, year, date_codes[DATE_CODE_CHRISTMAS_DAY1] >> 8, date_codes[DATE_CODE_CHRISTMAS_DAY1] & 0xFF);
        debug_log_printf ("christmas day2      %2d %04d-%02u-%02u\r\n", DATE_CODE_CHRISTMAS_DAY2, year, date_codes[DATE_CODE_CHRISTMAS_DAY2] >> 8, date_codes[DATE_CODE_CHRISTMAS_DAY2] & 0xFF);
        debug_log_printf ("carnival monday     %2d %04d-%02u-%02u\r\n", DATE_CODE_CARNIVAL_MONDAY, year, date_codes[DATE_CODE_CARNIVAL_MONDAY] >> 8, date_codes[DATE_CODE_CARNIVAL_MONDAY] & 0xFF);
        debug_log_printf ("good friday         %2d %04d-%02u-%02u\r\n", DATE_CODE_GOOD_FRIDAY, year, date_codes[DATE_CODE_GOOD_FRIDAY] >> 8, date_codes[DATE_CODE_GOOD_FRIDAY] & 0xFF);
        debug_log_printf ("easter sunday       %2d %04d-%02u-%02u\r\n", DATE_CODE_EASTER_SUNDAY, year, date_codes[DATE_CODE_EASTER_SUNDAY] >> 8, date_codes[DATE_CODE_EASTER_SUNDAY] & 0xFF);
        debug_log_printf ("easter monday       %2d %04d-%02u-%02u\r\n", DATE_CODE_EASTER_MONDAY, year, date_codes[DATE_CODE_EASTER_MONDAY] >> 8, date_codes[DATE_CODE_EASTER_MONDAY] & 0xFF);
        debug_log_printf ("ascension day       %2d %04d-%02u-%02u\r\n", DATE_CODE_ASCENSION_DAY, year, date_codes[DATE_CODE_ASCENSION_DAY] >> 8, date_codes[DATE_CODE_ASCENSION_DAY] & 0xFF);
        debug_log_printf ("pentecost sunday    %2d %04d-%02u-%02u\r\n", DATE_CODE_PENTECOST_SUNDAY, year, date_codes[DATE_CODE_PENTECOST_SUNDAY] >> 8, date_codes[DATE_CODE_PENTECOST_SUNDAY] & 0xFF);
        debug_log_printf ("pentecost monday    %2d %04d-%02u-%02u\r\n", DATE_CODE_PENTECOST_MONDAY, year, date_codes[DATE_CODE_PENTECOST_MONDAY] >> 8, date_codes[DATE_CODE_PENTECOST_MONDAY] & 0xFF);
        debug_log_printf ("corpus christi      %2d %04d-%02u-%02u\r\n", DATE_CODE_CORPUS_CHRISTI, year, date_codes[DATE_CODE_CORPUS_CHRISTI] >> 8, date_codes[DATE_CODE_CORPUS_CHRISTI] & 0xFF);
        debug_log_printf ("advent1             %2d %04d-%02u-%02u\r\n", DATE_CODE_ADVENT1, year, date_codes[DATE_CODE_ADVENT1] >> 8, date_codes[DATE_CODE_ADVENT1] & 0xFF);
        debug_log_printf ("advent2             %2d %04d-%02u-%02u\r\n", DATE_CODE_ADVENT2, year, date_codes[DATE_CODE_ADVENT2] >> 8, date_codes[DATE_CODE_ADVENT2] & 0xFF);
        debug_log_printf ("advent3             %2d %04d-%02u-%02u\r\n", DATE_CODE_ADVENT3, year, date_codes[DATE_CODE_ADVENT3] >> 8, date_codes[DATE_CODE_ADVENT3] & 0xFF);
        debug_log_printf ("advent4             %2d %04d-%02u-%02u\r\n", DATE_CODE_ADVENT4, year, date_codes[DATE_CODE_ADVENT4] >> 8, date_codes[DATE_CODE_ADVENT4] & 0xFF);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * get_date_code () - get date code of a day
 *
 *  day         - day of month
 *  month       - month beginning with 1
 *  year        - greater than 2000
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
get_date_code (uint_fast16_t mmdd, int year)
{
    static uint_fast8_t     date_code;
    static uint_fast16_t    last_mmdd;
    uint_fast8_t            idx;

    if (last_mmdd != mmdd)
    {
        init_date_codes (year);

        date_code = 0;

        for (idx = 1; idx < N_DATE_CODES; idx++)
        {
            if (date_codes[idx] == mmdd)
            {
                date_code = idx;
                break;
            }
        }
    }

    return date_code;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * get_date_by_date_code () - get date mmdd by date code
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast16_t
get_date_by_date_code (uint_fast8_t date_code, int year)
{
    init_date_codes (year);

    if (date_code < N_DATE_CODES)
    {
        return date_codes[date_code];
    }
    return 0;
}

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

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * substitute characters
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
strsubst (char * s, int old, int new)
{
    while (*s)
    {
        if (*s == old)
        {
            *s = new;
        }

        s++;
    }
}
