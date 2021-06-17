/*----------------------------------------------------------------------------------------------------------------------------------------
 * ESP-WordClock.ino - some ESP8266 network routines with communication interface via UART to WordClock (STM32)
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Arduino Settings ("Werkzeuge"):
 *     Platine:                 Generic ESP8266 Module
 *     Flash Mode:              DIO
 *     Flash Frequency          40 MHz
 *     Upload Using             Serial
 *     CPU Frequency            80 MHz
 *     Flash Size               512K (64K SPIFFS) for ESP-01
 *     Flash Size               4M (1M SPIFFS) for ESP-12F
 *     Reset Method             nodemcu, if connected to STM32 (wordclock)
 *     Upload Speed             115200
 * 
 * Commands:
 *    cap apname,key            - connect to AP
 *    ap apname,key             - start local AP
 *    time [timeserver]         - get time from timeserver
 *    weather appid,city        - get weather of city
 *    weather appid,lon,lat     - get weather of location with coordinates (lon/lat)
 *
 * Return values:
 *    OK [string]               - Ok
 *    ERROR [string]            - Error
 * 
 * Examples:
 *    cap "fm7320","4711471147114711"
 *    ap "wordclock","1234567890"
 *    time "129.6.15.28"
 *    weather "123456789012345678901234567890","koeln"
 *    weather "123456789012345678901234567890","6.957","50.937"
 *    var "..."
 * 
 * Asynchronous Messages terminated with CR LF:
 *    - string                - Debug message, should be ignored
 *    FIRMWARE x.x.x          - Firmware version
 *    AP ssid                 - SSID of AP connected or own AP ssid
 *    MODE client             - working as WLAN client
 *    MODE ap                 - working as AP
 *    IPADDRESS x.x.x.x       - IP address of module is x.x.x.x
 *    TIME sec                - Time in seconds since 1900
 *    CMD xx ...              - Got command followed by parameters (printed in hex)
 *    HTTP GET path [param]   - HTTP request with path and optional params. Waits for answer lines until single dot (".") arrives.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiUdp.h>
#include "FS.h"
#include "base.h"
#include "stm32flash.h"
#include "wifi.h"
#include "http.h"
#include "udpsrv.h"
#include "ntp.h"
#include "weather.h"
#include "vars.h"
#include "tables.h"
#include "version.h"


#define CMD_BUFFER_SIZE     128                                             // maximum size of command buffer

/*----------------------------------------------------------------------------------------------------------------------------------------
 * global setup
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
setup() 
{
    Serial.begin(115200);
    delay(1);
    Serial.println ("");
    Serial.flush ();
    delay(1000);

#if 0
    Serial.println ("- formatting SPIFFS");
    Serial.flush ();
    SPIFFS.begin ();
    SPIFFS.format ();
    SPIFFS.end ();
    Serial.println ("- formatting ready");
    Serial.flush ();
#endif

    ntp_setup ();
    udp_server_setup ();
    Serial.print ("FIRMWARE ");
    Serial.println (ESP_VERSION);
    Serial.flush ();
    vars_init ();
    stm32_flash_init ();
    tables_init ();
}

#define MAX_ICON_SIZE   (20*20)

static char           icon_name[32 + 1];
static char           icon_colors[MAX_ICON_SIZE + 1];
static uint_fast16_t  icon_colors_len;
static char           icon_animations_on[MAX_ICON_SIZE + 1];
static char           icon_animations_off[MAX_ICON_SIZE + 1];
static uint_fast16_t  icon_anim_on_len;
static uint_fast16_t  icon_anim_off_len;
static uint_fast8_t   icon_rows;
static uint_fast8_t   icon_cols;
static uint_fast8_t   icon_found;

static uint_fast16_t  colors_pos;
static uint_fast16_t  anim_on_pos;
static uint_fast16_t  anim_off_pos;

static void
icon_info (const char * fname, const char * name)
{
    int     ch;
    int     i;
    int     len;
    File    fp;

    icon_rows = 0;
    icon_cols = 0;
    icon_colors_len = 0;
    icon_anim_on_len = 0;
    icon_anim_off_len = 0;
    icon_found = 0;

    SPIFFS.begin ();

    fp = SPIFFS.open (fname, "r");

    if (fp)
    {
        ch = fp.read ();

        while (ch == '*')
        {
            i = 0;

            while ((ch = fp.read ()) != EOF)
            {
                if (ch == '\r' || ch == '\n')
                {
                    break;
                }

                if (i < 32)
                {
                    icon_name[i++] = ch;
                }
            }

            icon_name[i] = '\0';
            trim (icon_name);

            i                   = 0;
            icon_cols           = 0;
            icon_rows           = 0;
            icon_colors_len     = 0;
            icon_anim_on_len    = 0;
            icon_anim_off_len   = 0;
            len                 = 0;

            while ((ch = fp.read()) >= 0)
            {
                if (ch != '\r' && ch != '\n')
                {
                    if (ch == '*' || ch == '-')
                    {
                        break;
                    }

                    if (ch != ' ' && ch != '\t')
                    {
                        if (i < MAX_ICON_SIZE)
                        {
                          icon_colors[i++] = ch;
                        }
                        len++;
                    }
                }
                else
                {
                    if (icon_cols == 0)
                    {
                        if (i > 0)
                        {
                            icon_cols = i;
                        }
                    }
                }
            }

            icon_colors[i] = '\0';
            icon_colors_len = i;

            icon_rows = len / icon_cols;

            // animations on section
            if (ch == '-')
            {
                i = 0;

                while ((ch = fp.read ()) != EOF)
                {
                    if (ch != '\r' && ch != '\n')
                    {
                        if (ch == '*' || ch == '-')
                        {
                            break;
                        }

                        if (ch != ' ' && ch != '\t')
                        {
                            if (i < MAX_ICON_SIZE)
                            {
                                icon_animations_on[i++] = ch;
                            }
                        }
                    }
                }

                icon_animations_on[i] = '\0';
                icon_anim_on_len = i;

                // animations on section
                if (ch == '-')
                {
                    i = 0;

                    while ((ch = fp.read ()) != EOF)
                    {
                        if (ch != '\r' && ch != '\n')
                        {
                            if (ch == '*' || ch == '-')
                            {
                                break;
                            }

                            if (ch != ' ' && ch != '\t')
                            {
                                if (i < MAX_ICON_SIZE)
                                {
                                    icon_animations_off[i++] = ch;
                                }
                            }
                        }
                    }

                    icon_animations_off[i] = '\0';
                    icon_anim_off_len = i;
                }
                else
                {
                    icon_animations_off[0] = '\0';
                }
            }
            else
            {
                icon_animations_on[0] = '\0';
            }

            if (! strcmp (icon_name, name))
            {
                icon_found = 1;
                break;
            }
        }

        fp.close ();
    }

    SPIFFS.end ();
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * main loop
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
loop() 
{
    static char cmd_buffer[CMD_BUFFER_SIZE];
    static int  cmd_len = 0;

    wifi_check_if_started ();
    http_server_loop ();
    udp_server_loop ();
    ntp_poll_time ();                                                       // poll NTP

    while (Serial.available())
    {
        char ch = Serial.read ();
    
        if (ch == '\n')
        {
            cmd_buffer[cmd_len] = '\0';

            if (! strncmp (cmd_buffer, "var ", 4))
            {
                char * parameter;

                parameter = cmd_buffer + 4;

                var_set_parameter (parameter);
                Serial.println (".");                                       // "silent" OK
                Serial.flush ();
            }
            else if (! strcmp (cmd_buffer, "time"))
            {
                if (! wifi_ap_mode)
                {
                    ntp_get_time ();
                }
            }
            else if (! strncmp (cmd_buffer, "time ", 5))
            {
                if (! wifi_ap_mode)
                {
                    int       syntax_ok = false;
                    char *    p = cmd_buffer + 5;
                    char *    pp;

                    if (*p == '"')
                    {
                        pp = strchr (p + 1, '"');

                        if (pp)
                        {
                            *pp = '\0';

                            ntp_get_time (p + 1);
                            syntax_ok = true;
                        }
                    }

                    if (! syntax_ok)
                    {
                        Serial.println ("ERROR syntax error");
                        Serial.flush ();
                    }
                }
            }
            else if (! strncmp (cmd_buffer, "ap ", 3))
            {
                int     syntax_ok = false;
                char *  ssid;
                char *  key;
                char *  p = cmd_buffer + 3;
                char *  pp;

                if (*p == '"')
                {
                    pp = strchr (p + 1, '"');

                    if (pp)
                    {
                        *pp = '\0';
                        ssid = p + 1;
                        p = pp + 1;

                        if (*p == ',' && *(p + 1) == '"')
                        {
                            pp = strchr (p + 2, '"');

                            if (pp)
                            {
                                syntax_ok = true;
                                *pp = '\0';
                                key = p + 2;

                                wifi_ap (ssid, key);
                            }
                        }
                    }
                }

                if (! syntax_ok)
                {
                    Serial.println ("ERROR syntax error");
                    Serial.flush ();
                }
            }
            else if (! strncmp (cmd_buffer, "cap ", 4))
            {
                int     syntax_ok = false;
                char *  ssid;
                char *  key;
                char *  p = cmd_buffer + 4;
                char *  pp;

                if (*p == '"')
                {
                    pp = strchr (p + 1, '"');

                    if (pp)
                    {
                        *pp = '\0';
                        ssid = p + 1;
                        p = pp + 1;
                        if (*p == ',' && *(p + 1) == '"')
                        {
                            pp = strchr (p + 2, '"');

                            if (pp)
                            {
                                syntax_ok = true;
                                *pp = '\0';
                                key = p + 2;

                                wifi_connect (ssid, key);
                            }
                        }
                    }
                }

                if (! syntax_ok)
                {
                    Serial.println ("ERROR syntax error");
                    Serial.flush ();
                }
            }
            else if (! strncmp (cmd_buffer, "weather ", 8))
            {
                if (! wifi_ap_mode)
                {
                    int     syntax_ok = false;
                    char *  appid;
                    char *  city;
                    char *  lon;
                    char *  lat;
                    char *  p = cmd_buffer + 8;
                    char *  pp;

                    if (*p == '"')
                    {
                        pp = strchr (p + 1, '"');

                        if (pp)
                        {
                            *pp = '\0';
                            appid = p + 1;
                            p = pp + 1;

                            if (*p == ',' && *(p + 1) == '"')
                            {
                                pp = strchr (p + 2, '"');

                                if (pp)
                                {
                                    *pp = '\0';

                                    if (*(pp + 1))                                       // "appid","lon","lat"
                                    {
                                        lon = p + 2;
                                        p = pp + 1;

                                        if (*p == ',' && *(p + 1) == '"')
                                        {
                                            pp = strchr (p + 2, '"');

                                            if (pp)
                                            {
                                                *pp = '\0';
                                                lat = p + 2;
                                                syntax_ok = true;
                                                get_weather (appid, lon, lat);
                                            }
                                        }
                                    }
                                    else                                                // "appid","city"
                                    {
                                        city = p + 2;
                                        syntax_ok = true;
                                        get_weather (appid, city);
                                    }
                                }
                            }
                        }
                    }

                    if (! syntax_ok)
                    {
                        Serial.println ("ERROR syntax error");
                        Serial.flush ();
                    }
                }
            }
            else if (! strncmp (cmd_buffer, "weather_fc ", 11))
            {
                if (! wifi_ap_mode)
                {
                    int     syntax_ok = false;
                    char *  appid;
                    char *  city;
                    char *  lon;
                    char *  lat;
                    char *  p = cmd_buffer + 11;
                    char *  pp;

                    if (*p == '"')
                    {
                        pp = strchr (p + 1, '"');

                        if (pp)
                        {
                            *pp = '\0';
                            appid = p + 1;
                            p = pp + 1;

                            if (*p == ',' && *(p + 1) == '"')
                            {
                                pp = strchr (p + 2, '"');

                                if (pp)
                                {
                                    *pp = '\0';

                                    if (*(pp + 1))                                       // "appid","lon","lat"
                                    {
                                        lon = p + 2;
                                        p = pp + 1;

                                        if (*p == ',' && *(p + 1) == '"')
                                        {
                                            pp = strchr (p + 2, '"');

                                            if (pp)
                                            {
                                                *pp = '\0';
                                                lat = p + 2;
                                                syntax_ok = true;
                                                get_weather_fc (appid, lon, lat);
                                            }
                                        }
                                    }
                                    else                                                // "appid","city"
                                    {
                                        city = p + 2;
                                        syntax_ok = true;
                                        get_weather_fc (appid, city);
                                    }
                                }
                            }
                        }
                    }

                    if (! syntax_ok)
                    {
                        Serial.println ("ERROR syntax error");
                        Serial.flush ();
                    }
                }
            }
            else if (! strncmp (cmd_buffer, "wicon ", 6))
            {
                if (! wifi_ap_mode)
                {
                    int     syntax_ok = false;
                    char *  appid;
                    char *  city;
                    char *  lon;
                    char *  lat;
                    char *  p = cmd_buffer + 6;
                    char *  pp;

                    if (*p == '"')
                    {
                        pp = strchr (p + 1, '"');

                        if (pp)
                        {
                            *pp = '\0';
                            appid = p + 1;
                            p = pp + 1;

                            if (*p == ',' && *(p + 1) == '"')
                            {
                                pp = strchr (p + 2, '"');

                                if (pp)
                                {
                                    *pp = '\0';

                                    if (*(pp + 1))                                       // "appid","lon","lat"
                                    {
                                        lon = p + 2;
                                        p = pp + 1;

                                        if (*p == ',' && *(p + 1) == '"')
                                        {
                                            pp = strchr (p + 2, '"');

                                            if (pp)
                                            {
                                                *pp = '\0';
                                                lat = p + 2;
                                                syntax_ok = true;
                                                get_weather_icon (appid, lon, lat);
                                            }
                                        }
                                    }
                                    else                                                // "appid","city"
                                    {
                                        city = p + 2;
                                        syntax_ok = true;
                                        get_weather_icon (appid, city);
                                    }
                                }
                            }
                        }
                    }

                    if (! syntax_ok)
                    {
                        Serial.println ("ERROR syntax error");
                        Serial.flush ();
                    }
                }
            }
            else if (! strncmp (cmd_buffer, "wicon_fc ", 9))
            {
                if (! wifi_ap_mode)
                {
                    int     syntax_ok = false;
                    char *  appid;
                    char *  city;
                    char *  lon;
                    char *  lat;
                    char *  p = cmd_buffer + 9;
                    char *  pp;

                    if (*p == '"')
                    {
                        pp = strchr (p + 1, '"');

                        if (pp)
                        {
                            *pp = '\0';
                            appid = p + 1;
                            p = pp + 1;

                            if (*p == ',' && *(p + 1) == '"')
                            {
                                pp = strchr (p + 2, '"');

                                if (pp)
                                {
                                    *pp = '\0';

                                    if (*(pp + 1))                                       // "appid","lon","lat"
                                    {
                                        lon = p + 2;
                                        p = pp + 1;

                                        if (*p == ',' && *(p + 1) == '"')
                                        {
                                            pp = strchr (p + 2, '"');

                                            if (pp)
                                            {
                                                *pp = '\0';
                                                lat = p + 2;
                                                syntax_ok = true;
                                                get_weather_icon_fc (appid, lon, lat);
                                            }
                                        }
                                    }
                                    else                                                // "appid","city"
                                    {
                                        city = p + 2;
                                        syntax_ok = true;
                                        get_weather_icon_fc (appid, city);
                                    }
                                }
                            }
                        }
                    }

                    if (! syntax_ok)
                    {
                        Serial.println ("ERROR syntax error");
                        Serial.flush ();
                    }
                }
            }
            else if (! strcmp (cmd_buffer, "wps"))
            {
                wifi_wps ();
            }
            else if (! strcmp (cmd_buffer, "tabinfo"))
            {
                tables_info ();
            }
            else if (! strncmp (cmd_buffer, "tabillu ", 8))
            {
                int     syntax_ok = false;
                char *  p = cmd_buffer + 8;
                char *  pp;

                if (*p == '"')
                {
                    pp = strchr (p + 1, '"');

                    if (pp)
                    {
                        *pp = '\0';
                        tables_illumination (atoi (p + 1));
                        syntax_ok = true;
                    }
                }
                if (! syntax_ok)
                {
                    Serial.println ("ERROR syntax error");
                    Serial.flush ();
                }
            }
            else if (! strncmp (cmd_buffer, "tabh ", 5))
            {
                int     syntax_ok = false;
                char *  modep;
                char *  idxp;
                char *  p = cmd_buffer + 5;
                char *  pp;

                if (*p == '"')
                {
                    pp = strchr (p + 1, '"');

                    if (pp)
                    {
                        *pp = '\0';
                        modep = p + 1;
                        p = pp + 1;
                        if (*p == ',' && *(p + 1) == '"')
                        {
                            pp = strchr (p + 2, '"');

                            if (pp)
                            {
                                syntax_ok = true;
                                *pp = '\0';
                                idxp = p + 2;

                                tables_hours (atoi (modep), atoi (idxp));
                            }
                        }
                    }
                }

                if (! syntax_ok)
                {
                    Serial.println ("ERROR syntax error");
                    Serial.flush ();
                }
            }
            else if (! strncmp (cmd_buffer, "tabm ", 5) || ! strncmp (cmd_buffer, "tabt ", 5))
            {
                int     syntax_ok = false;
                char *  modep;
                char *  idxp;
                char *  p = cmd_buffer + 5;
                char *  pp;

                if (*p == '"')
                {
                    pp = strchr (p + 1, '"');

                    if (pp)
                    {
                        *pp = '\0';
                        modep = p + 1;
                        p = pp + 1;
                        if (*p == ',' && *(p + 1) == '"')
                        {
                            pp = strchr (p + 2, '"');

                            if (pp)
                            {
                                syntax_ok = true;
                                *pp = '\0';
                                idxp = p + 2;

                                if (! strncmp (cmd_buffer, "tabm ", 5))
                                {
                                    tables_minutes (atoi (modep), atoi (idxp), "TABM");
                                }
                                else
                                {
                                    tables_minutes (atoi (modep), atoi (idxp), "TABT");
                                }
                            }
                        }
                    }
                }

                if (! syntax_ok)
                {
                    Serial.println ("ERROR syntax error");
                    Serial.flush ();
                }
            }
#if 0 // yet not used
            else if (! strncmp (cmd_buffer, "file-open ", 10))
            {
                char *  p = cmd_buffer + 10;
                char *  pp;
                char *  fname;
                int     size;

                if (*p == '"' && (pp = strchr (p + 1, '"')) != (char *) NULL)
                {
                    *pp = '\0';
                    fname = p + 1;

                    SPIFFS.begin ();
                    fp = SPIFFS.open (fname, "r");
    
                    Serial.print ("OPEN ");
    
                    if (fp)
                    {
                        size = fp.size();
                    }
                    else
                    {
                        size = -1;
                    }
    
                    if (size <= 0)
                    {
                        if (fp)
                        {
                            fp.close ();
                            fp = (File) 0;
                        }
                        SPIFFS.end ();
                    }

                    Serial.println (size);
                }
                else
                {
                    Serial.println ("ERROR syntax error");
                }
                Serial.flush ();
            }
            else if (! strcmp (cmd_buffer, "file-close"))
            {
                if (fp)
                {
                    fp.close ();
                    fp = (File) 0;
                    SPIFFS.end ();
                }

                Serial.println ("CLOSE");
                Serial.flush ();
            }
            else if (! strcmp (cmd_buffer, "file-read"))
            {
                int idx;
                int ch;
                Serial.print ("FILE ");

                if (fp)
                {
                    for (idx = 0; idx < 16; idx++)
                    {
                        ch = fp.read ();

                        if (ch < 0)
                        {
                            break;
                        }
                        Serial.printf ("%02x", ch);
                    }
                }

                Serial.println ("");
                Serial.flush ();
            }
#endif
            else if (! strncmp (cmd_buffer, "icon ", 5))            // open icon file and send infos about icon
            {
                int     syntax_ok = false;
                char *  fname;
                char *  icon_name;
                char *  p = cmd_buffer + 5;
                char *  pp;

                if (*p == '"')
                {
                    pp = strchr (p + 1, '"');

                    if (pp)
                    {
                        *pp = '\0';
                        fname = p + 1;
                        p = pp + 1;

                        if (*p == ',' && *(p + 1) == '"')
                        {
                            pp = strchr (p + 2, '"');

                            if (pp)
                            {
                                char answer[32];

                                syntax_ok = true;
                                *pp = '\0';
                                icon_name = p + 2;

                                icon_info (fname, icon_name);
                                sprintf (answer, "ICON %02x%02x%04x%04x%04x", icon_rows, icon_cols, icon_colors_len, icon_anim_on_len, icon_anim_off_len);

                                colors_pos = 0;
                                anim_on_pos = 0;
                                anim_off_pos = 0;
                                Serial.println (answer);
                            }
                        }
                    }
                }

                if (! syntax_ok)
                {
                    Serial.println ("ERROR syntax error");
                    Serial.flush ();
                }

                Serial.flush ();
            }
            else if (! strcmp (cmd_buffer, "icon"))            // send icon data in 16-byte blocks
            {
                int idx;
                int ch;
                Serial.print ("ICON ");

                if (icon_found)
                {
                    for (idx = 0; idx < 16; idx++)
                    {
                        if (colors_pos < icon_colors_len)
                        {
                            ch = icon_colors[colors_pos];
                            colors_pos++;
                        }
                        else if (anim_on_pos < icon_anim_on_len)
                        {
                            ch = icon_animations_on[anim_on_pos];
                            anim_on_pos++;
                        }
                        else if (anim_off_pos < icon_anim_off_len)
                        {
                            ch = icon_animations_off[anim_off_pos];
                            anim_off_pos++;
                        }
                        else
                        {
                            break;
                        }
                        Serial.printf ("%02x", ch);
                    }
                }

                Serial.println ("");
                Serial.flush ();
            }
            else
            {
                Serial.println ("ERROR invalid command");
                Serial.flush ();
            }

            cmd_buffer[0] = '\0';
            cmd_len = 0;
        }
        else
        {
            if (ch != '\r')
            {
                if (cmd_len < CMD_BUFFER_SIZE - 1)
                {
                    cmd_buffer[cmd_len++] = ch;
                }
            }
        }
    }
}
