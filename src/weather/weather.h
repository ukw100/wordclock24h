/*-------------------------------------------------------------------------------------------------------------------------------------------
 * weather.h - some global definitions/declarations for functions using openweathermap.org
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef WEATHER_H
#define WEATHER_H

#include <string.h>
#include <stdint.h>

#define MAX_WEATHER_APPID_LEN       32
#define MAX_WEATHER_CITY_LEN        32
#define MAX_WEATHER_LON_LEN          8
#define MAX_WEATHER_LAT_LEN          8

// parameters for weather_query():
#define WEATHER_QUERY_ID_ICON       0                                       // query weather icon
#define WEATHER_QUERY_ID_ICON_FC    1                                       // query weather forecast icon
#define WEATHER_QUERY_ID_TEXT       2                                       // query weather text
#define WEATHER_QUERY_ID_TEXT_FC    3                                       // query weather forecast text


typedef struct
{
    char                appid[MAX_WEATHER_APPID_LEN + 1];
    char                city[MAX_WEATHER_CITY_LEN + 1];
    char                lon[MAX_WEATHER_LON_LEN + 1];
    char                lat[MAX_WEATHER_LAT_LEN + 1];

} WEATHER_GLOBALS;

extern WEATHER_GLOBALS  weather;

extern uint_fast8_t     weather_read_config_from_eeprom (void);
extern uint_fast8_t     weather_write_config_to_eeprom (void);
extern void             weather_set_appid (char *);
extern void             weather_set_city (char *);
extern void             weather_set_lon (char *);
extern void             weather_set_lat (char *);
extern void             weather_query (uint_fast8_t);

#endif
