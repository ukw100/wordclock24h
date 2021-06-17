/*-------------------------------------------------------------------------------------------------------------------------------------------
 * weather.c - functions using openweathermap.org
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "base.h"
#include "weather.h"
#include "esp8266.h"
#include "eeprom.h"
#include "eeprom-data.h"

/*--------------------------------------------------------------------------------------------------------------------------------------
 * globals:
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
WEATHER_GLOBALS   weather;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read configuration from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
weather_read_config_from_eeprom (void)
{
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up &&
        eeprom_read (EEPROM_DATA_OFFSET_WEATHER_APPID,  (uint8_t *) weather.appid,  EEPROM_DATA_SIZE_WEATHER_APPID) &&
        eeprom_read (EEPROM_DATA_OFFSET_WEATHER_CITY,   (uint8_t *) weather.city,   EEPROM_DATA_SIZE_WEATHER_CITY) &&
        eeprom_read (EEPROM_DATA_OFFSET_WEATHER_LON,    (uint8_t *) weather.lon,    EEPROM_DATA_SIZE_WEATHER_LON) &&
        eeprom_read (EEPROM_DATA_OFFSET_WEATHER_LAT,    (uint8_t *) weather.lat,    EEPROM_DATA_SIZE_WEATHER_LAT))
    {
        if (*(unsigned char *) (weather.appid) == 0xFF || *(unsigned char *) (weather.city) == 0xFF ||
            *(unsigned char *) (weather.lon) == 0xFF || *(unsigned char *) (weather.lat) == 0xFF)
        {
            weather.appid[0] = '\0';
            weather.city[0] = '\0';
            weather.lon[0] = '\0';
            weather.lat[0] = '\0';

            weather_write_config_to_eeprom ();                          // repair uninitialized EEPROM range
        }
        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save appid
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
weather_save_appid (void)
{
    uint_fast8_t    rtc;

    rtc = eeprom_write (EEPROM_DATA_OFFSET_WEATHER_APPID, (uint8_t *) weather.appid, EEPROM_DATA_SIZE_WEATHER_APPID);

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save city
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
weather_save_city (void)
{
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up && eeprom_write (EEPROM_DATA_OFFSET_WEATHER_CITY, (uint8_t *) weather.city, EEPROM_DATA_SIZE_WEATHER_CITY))
    {
        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save lon
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
weather_save_lon (void)
{
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up &&
        eeprom_write (EEPROM_DATA_OFFSET_WEATHER_LON, (uint8_t *) weather.lon, EEPROM_DATA_SIZE_WEATHER_LON))
    {
        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * save lat
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
weather_save_lat (void)
{
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up &&
        eeprom_write (EEPROM_DATA_OFFSET_WEATHER_LAT, (uint8_t *) weather.lat, EEPROM_DATA_SIZE_WEATHER_LAT))
    {
        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write complete configuration to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
weather_write_config_to_eeprom (void)
{
    uint_fast8_t    rtc = 0;

    if (weather_save_appid () &&
        weather_save_city () &&
        weather_save_lon () &&
        weather_save_lat ())
    {
        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set appid
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
weather_set_appid (char * new_appid)
{
    strncpy (weather.appid, new_appid, MAX_WEATHER_APPID_LEN);
    weather_save_appid ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set city
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
weather_set_city (char * new_city)
{
    strncpy (weather.city, new_city, MAX_WEATHER_CITY_LEN);
    weather_save_city ();
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set lon
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
weather_set_lon (char * new_lon)
{
    strsubst (new_lon, ',', '.');
    strncpy (weather.lon, new_lon, MAX_WEATHER_LON_LEN);
    weather_save_lon ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set lat
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
weather_set_lat (char * new_lat)
{
    strsubst (new_lat, ',', '.');
    strncpy (weather.lat, new_lat, MAX_WEATHER_LAT_LEN);
    weather_save_lat ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * query weather
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
weather_query (uint_fast8_t query_id)
{
    if (*weather.appid)
    {
        if (*weather.lon && *weather.lat)
        {
            char args[16 + MAX_WEATHER_APPID_LEN + MAX_WEATHER_LON_LEN + MAX_WEATHER_LAT_LEN];

            strcpy (args, weather.appid);
            strcat (args, "\",\"");
            strcat (args, weather.lon);
            strcat (args, "\",\"");
            strcat (args, weather.lat);

            switch (query_id)
            {
                case WEATHER_QUERY_ID_ICON:
                {
                    esp8266_send_cmd ("wicon", args, 1);
                    break;
                }
                case WEATHER_QUERY_ID_ICON_FC:
                {
                    esp8266_send_cmd ("wicon_fc", args, 1);
                    break;
                }
                case WEATHER_QUERY_ID_TEXT:
                {
                    esp8266_send_cmd ("weather", args, 1);
                    break;
                }
                case WEATHER_QUERY_ID_TEXT_FC:
                {
                    esp8266_send_cmd ("weather_fc", args, 1);
                    break;
                }
            }
        }
        else if (*weather.city)
        {
            char args[16 + MAX_WEATHER_APPID_LEN + MAX_WEATHER_CITY_LEN];

            strcpy (args, weather.appid);
            strcat (args, "\",\"");
            strcat (args, weather.city);

            switch (query_id)
            {
                case WEATHER_QUERY_ID_ICON:
                {
                    esp8266_send_cmd ("wicon", args, 1);
                    break;
                }
                case WEATHER_QUERY_ID_ICON_FC:
                {
                    esp8266_send_cmd ("wicon_fc", args, 1);
                    break;
                }
                case WEATHER_QUERY_ID_TEXT:
                {
                    esp8266_send_cmd ("weather", args, 1);
                    break;
               }
                case WEATHER_QUERY_ID_TEXT_FC:
                {
                    esp8266_send_cmd ("weather_fc", args, 1);
                    break;
                }
            }
        }
    }
}
