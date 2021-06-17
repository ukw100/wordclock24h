/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * base.h - declarations of base routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef BASE_H
#define BASE_H

#include <string.h>
#include <stdint.h>
#include <time.h>

enum
{
    DATE_CODE_NONE,                                                                         // no date code
    DATE_CODE_NEW_YEAR,                                                                     // Neujahr
    DATE_CODE_THREE_MAGI,                                                                   // Heilige drei Könige
    DATE_CODE_FIRST_MAY,                                                                    // Maifeiertag
    DATE_CODE_GERMANY_UNITY_DAY,                                                            // Tag der deutschen Einheit
    DATE_CODE_CHRISTMAS_DAY1,                                                               // 1. Weihnachtsfeiertag
    DATE_CODE_CHRISTMAS_DAY2,                                                               // 2. Weihnachtsfeiertag

    DATE_CODE_CARNIVAL_MONDAY,                                                              // Rosenmontag
    DATE_CODE_GOOD_FRIDAY,                                                                  // Karfreitag
    DATE_CODE_EASTER_SUNDAY,                                                                // Ostersonntag
    DATE_CODE_EASTER_MONDAY,                                                                // Ostermontag
    DATE_CODE_ASCENSION_DAY,                                                                // Christi Himmelfahrt
    DATE_CODE_PENTECOST_SUNDAY,                                                             // Pfingstsonntag
    DATE_CODE_PENTECOST_MONDAY,                                                             // Pfingstmontag
    DATE_CODE_CORPUS_CHRISTI,                                                               // Fronleichnam
    DATE_CODE_ADVENT1,                                                                      // 1st Advent
    DATE_CODE_ADVENT2,                                                                      // 2nd Advent
    DATE_CODE_ADVENT3,                                                                      // 3rd Advent
    DATE_CODE_ADVENT4,                                                                      // 4th Advent

    N_DATE_CODES
};

extern const char * wdays_en[7];
extern const char * wdays_de[7];

extern uint_fast16_t    add_days (uint_fast16_t, int, int);
extern int              dayofweek (int, int, int);
extern uint_fast8_t     days_of_month (uint_fast8_t, uint_fast16_t);
extern void             init_date_codes (int);
extern uint_fast8_t     get_date_code (uint_fast16_t, int);
uint_fast16_t           get_date_by_date_code (uint_fast8_t , int);
extern uint16_t         htoi (char *, uint8_t);
extern void             strsubst (char *, int, int);
#endif
