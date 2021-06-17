/*----------------------------------------------------------------------------------------------------------------------------------------
 * weather.cpp - weather stuff
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#include <ESP8266WiFi.h>
#include "weather.h"
#include "base.h"

WiFiClient openweather_client;

static int
round_up (char * degree)
{
    int   deg;
    int   i;
    bool  round_up = false;

    for (i = 0; degree[i]; i++)
    {
        if (degree[i] == '.')
        {
            if (degree[i + 1] >= '5')
            {
                round_up = true;
            }

            break;
        }
    }

    deg = atoi (degree);

    if (round_up)
    {
        deg++;
    }

    return deg;
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * parse_json - simple json parser
 *
 * ArduinoJson parser is too fat and fails with 9 forecast data lines
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static int
parse_json (const char * str, const char * pattern, int cnt, char * result, int max_len)
{
    int     len    = strlen (pattern);
    int     found  = 0;

    while (*str)
    {
        if (*str == '"' && ! strncmp (str + 1, pattern, len) && *(str + len + 1) == '"' && *(str + len + 2) == ':')
        {
            if (found == cnt)
            {
                int l = 0;

                str += len + 3;

                if (*str == '"')
                {
                    str++;

                    while (*(str + l) && *(str + l) != '"')
                    {
                        l++;
                    }
                }               
                else
                {
                    while (*(str + l) && ((*(str + l) >= '0' && *(str + l) <= '9') || *(str + l) == '.'))
                    {
                        l++;
                    }
                }               

                if (l > max_len - 1)
                {
                    l = max_len - 1;
                }

                strncpy (result, str, l);
                *(result + l) = '\0';
                return 1;
            }

            found++;
        }
        str++;
    }

    return 0;
}

#define MAX_LEN_COD             8
#define MAX_LEN_TEMP            8
#define MAX_LEN_DESCRIPTION     32
#define MAX_LEN_ICON            8

/*----------------------------------------------------------------------------------------------------------------------------------------
 * parse_weather () - parse the answer for forecast and store the valuse in weather_fc struct
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static void
parse_weather (const char * answer, uint_fast8_t do_get_icon)
{
    char cod[MAX_LEN_COD];

    if (parse_json (answer, "cod", 0, cod, MAX_LEN_COD) == 1)
    {
        if (atoi (cod) == 200)
        {
            if (do_get_icon)
            {
                char icon[MAX_LEN_ICON];

                if (parse_json (answer, "icon", 0, icon, MAX_LEN_ICON) == 1)
                {
                    Serial.print("WICON ");
                    Serial.println (icon);
                }
            }
            else
            {
                char  degree[MAX_LEN_TEMP];
                char  description[MAX_LEN_DESCRIPTION];
                int   deg;

                Serial.print ("WEATHER ");
                Serial.print ("Wetter heute: ");

                if (parse_json (answer, "temp", 0, degree, MAX_LEN_TEMP) == 1)
                {
                    deg = round_up (degree);
                    Serial.print (deg);
                    Serial.print (" Grad, ");
                }

                if (parse_json (answer, "description", 0, description, MAX_LEN_DESCRIPTION) == 1)
                {
                    char * description_iso8 = (char *) convert_utf8_to_iso8859 ((const unsigned char *) description);
                    Serial.print (description_iso8);
                }

                Serial.println ("");
            }
        }
        else
        {
            Serial.print ("WEATHER Wetter heute: Error ");
            Serial.println (cod);
        }
    }
    else
    {
        Serial.println ("WEATHER Wetter heute: Parse Error");
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * parse_weather_fc () - parse the answer for forecast and store the valuse in weather_fc struct
 * we get 9 of max. 36 records:
 * 0 : current weather
 * 1 : current weather + 3h
 * 2 : current weather + 6h
 * ...
 * 8 : current weather + 24h
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static void
parse_weather_fc (const char * answer, uint_fast8_t do_get_icon)
{
    char cod[MAX_LEN_COD];

    if (parse_json (answer, "cod", 0, cod, MAX_LEN_COD) == 1)
    {
        if (atoi (cod) == 200)
        {
            if (do_get_icon)
            {
                char icon[MAX_LEN_ICON];

                if (parse_json (answer, "icon", 8, icon, MAX_LEN_ICON) == 1)                           // line index == 8
                {
                    Serial.print("WICON_FC ");
                    Serial.println (icon);
                }
            }
            else
            {
                char  degree[MAX_LEN_TEMP];
                char  description[MAX_LEN_DESCRIPTION];
                int   deg;

                Serial.print ("WEATHER_FC ");
                Serial.print ("Wetter morgen: ");

                if (parse_json (answer, "temp", 8, degree, MAX_LEN_TEMP) == 1)                         // line index == 8
                {
                    deg = round_up (degree);
                    Serial.print (deg);
                    Serial.print (" Grad, ");
                }

                if (parse_json (answer, "description", 8, description, MAX_LEN_DESCRIPTION) == 1)      // line index == 8
                {
                    char * description_iso8 = (char *) convert_utf8_to_iso8859 ((const unsigned char *) description);
                    Serial.print (description_iso8);
                }

                Serial.println ("");
            }
        }
        else
        {
            Serial.print ("WEATHER_FC Wetter morgen: Error ");
            Serial.println (cod);
        }
    }
    else
    {
        Serial.println ("WEATHER_FC Wetter morgen: Parse Error");
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * query_weather () - query weather for a coordinate or city
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
static void
query_weather (char * appid, char * lon, char * lat, char * city, int do_get_icon, int fc)
{
    const char *    hostname = "api.openweathermap.org";
    String          url;

    if (fc)
    {
        url = (String) "/data/2.5/forecast";                                                                    // get 3h forecast
    }
    else
    {
        url = (String) "/data/2.5/weather";
    }

    if (city)
    {
        char * pp;

        url += (String) "?q=";

        do
        {
            pp = strchr (city, ' ');

            if (pp)
            {
                *pp = '\0';
                url += city;
                url += "%20";
                city = pp + 1;
            }
        } while (pp);

        url += (String) city + "&lang=de&units=metric&APPID=" + appid;
    }
    else
    {
        url += (String) "?lon=" + lon + "&lat=" + lat + "&lang=de&units=metric&APPID=" + appid;
    }

    if (fc)
    {
        url += "&cnt=9";                                                                                    // forecast: we need only 9 records of 36 records, limit output
    }

    if (openweather_client.connect(hostname, 80))
    {
        debugmsg ("Connected to server");
        openweather_client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + hostname + "\r\n" + "Connection: close\r\n\r\n");
        // debugmsg (String("GET ") + url + " HTTP/1.1<CR><LF>" + "Host: " + hostname + "<CR><LF>" + "Connection: close<CR><LF><CR><LF>");

        delay (200);    // 100 results in timeout sometimes

        if (openweather_client.available())
        {
            String          line;
            int             cnt = 0;
            const char *    p;

            do
            {
                line = openweather_client.readStringUntil('\n');
                p = line.c_str();

                if (p)
                {
                    while (*p == '\r' || *p == '\n')
                    {
                        p++;
                    }

                    // debugmsg ("answer", p);

                    if (! *p)
                    {
                        // debugmsg ("empty answer line found");
                        break;
                    }
                    cnt++;
                }
                else
                {
                    debugmsg ("null");
                }
            } while (openweather_client.available() && cnt < 20);

            if (openweather_client.available())
            {
                line = openweather_client.readStringUntil('\n');
                p = line.c_str();
                // debugmsg ("data0", p);

                if (p && strlen (p) < 10 && openweather_client.available())       // 1st data line is length of following line in Hex, e.g. "1da"
                {
                    line = openweather_client.readStringUntil('\n');            // read next data line
                    p = line.c_str();
                    // debugmsg ("data1", p);
                }

                if (p)
                {
                    if (fc)
                    {
                        parse_weather_fc (p, do_get_icon);
                    }
                    else
                    {
                        parse_weather (p, do_get_icon);
                    }
                }
            }
        }

        openweather_client.stop ();
    }
    else
    {
        Serial.println(String ("ERROR connection to ") + hostname + " failed");
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather () - get weather for a coordinate
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather (char * appid, char * lon, char * lat)
{
    query_weather (appid, lon, lat, (char *) NULL, 0, 0);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather () - get weather for a city
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather (char * appid, char * city)
{
    query_weather (appid, (char *) NULL, (char *) NULL, city, 0, 0);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather_fc () - get weather for a coordinate
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather_fc (char * appid, char * lon, char * lat)
{
    query_weather (appid, lon, lat, (char *) NULL, 0, 1);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather_fc () - get weather for a city
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather_fc (char * appid, char * city)
{
    query_weather (appid, (char *) NULL, (char *) NULL, city, 0, 1);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather_icon () - get weather icon for a coordinate
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather_icon (char * appid, char * lon, char * lat)
{
    query_weather (appid, lon, lat, (char *) NULL, 1, 0);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather () - get weather icon for a city
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather_icon (char * appid, char * city)
{
    query_weather (appid, (char *) NULL, (char *) NULL, city, 1, 0);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather_icon_fc () - get forecast weather icon for a coordinate
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather_icon_fc (char * appid, char * lon, char * lat)
{
    query_weather (appid, lon, lat, (char *) NULL, 1, 1);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * get_weather_icon_fc () - get forecast weather icon for a city
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
get_weather_icon_fc (char * appid, char * city)
{
    query_weather (appid, (char *) NULL, (char *) NULL, city, 1, 1);
}
