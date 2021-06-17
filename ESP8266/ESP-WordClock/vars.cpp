/*----------------------------------------------------------------------------------------------------------------------------------------
 * vars.cpp - synchronization of variables between STM32 and ESP8266
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#include "Arduino.h"
#include "base.h"
#include "vars.h"

#define CMD_CODE_NUMERIC_VAR                            'N'                        // command:   numeric variable
#define CMD_CODE_NUMERIC_ARRAY                          'n'                        // command:   numeric array
#define CMD_CODE_STRING_VAR                             'S'                        // command:   string variable

#define CMD_CODE_TIME_VAR                               'T'                        // command:   time variable

#define CMD_CODE_DISPLAY_VAR                            'D'                        // command:   display variable
#define PAR_CODE_DISPLAY_COLOR                          'C'                        // parameter: display color

#define CMD_CODE_ANIMATION_VAR                          'A'                        // command:   animation variable
#define PAR_CODE_ANIMATION_MODE_NAME                    'N'                        // parameter: animation mode name
#define PAR_CODE_ANIMATION_DECELERATION                 'D'                        // parameter: animation deceleration
#define PAR_CODE_ANIMATION_DEFAULT_DECELERATION         'E'                        // parameter: animation default deceleration
#define PAR_CODE_ANIMATION_FLAGS                        'F'                        // parameter: animation flags

#define CMD_CODE_COLOR_ANIMATION_VAR                    'C'                        // command:   color animation variable
#define PAR_CODE_COLOR_ANIMATION_MODE_NAME              'N'                        // parameter: animation mode name
#define PAR_CODE_COLOR_ANIMATION_DECELERATION           'D'                        // parameter: animation deceleration
#define PAR_CODE_COLOR_ANIMATION_DEFAULT_DECELERATION   'E'                        // parameter: animation default deceleration
#define PAR_CODE_COLOR_ANIMATION_FLAGS                  'F'                        // parameter: animation flags

#define CMD_CODE_AMBILIGHT_MODE_VAR                     'M'                        // command:   ambilight mode variable
#define PAR_CODE_AMBILIGHT_MODE_NAME                    'N'                        // parameter: animation mode name
#define PAR_CODE_AMBILIGHT_MODE_DECELERATION            'D'                        // parameter: animation deceleration
#define PAR_CODE_AMBILIGHT_MODE_DEFAULT_DECELERATION    'E'                        // parameter: animation default deceleration
#define PAR_CODE_AMBILIGHT_MODE_FLAGS                   'F'                        // parameter: animation flags

#define CMD_CODE_OVERLAY_VAR                            'O'                        // command:   overlay variable
#define PAR_CODE_OVERLAY_TYPE                           'T'                        // parameter: overlay type
#define PAR_CODE_OVERLAY_INTERVAL                       'I'                        // parameter: overlay interval
#define PAR_CODE_OVERLAY_DURATION                       'D'                        // parameter: overlay duration
#define PAR_CODE_OVERLAY_DATE_CODE                      'C'                        // parameter: overlay date code
#define PAR_CODE_OVERLAY_DATE_START                     'S'                        // parameter: overlay date start
#define PAR_CODE_OVERLAY_DAYS                           'Y'                        // parameter: overlay days
#define PAR_CODE_OVERLAY_TEXT                           'N'                        // parameter: overlay name or text
#define PAR_CODE_OVERLAY_FLAGS                          'F'                        // parameter: overlay flags

#define CMD_CODE_NIGHT_TIME_TABLE                       't'                        // command:   night time table
#define CMD_CODE_AMBILIGHT_NIGHT_TIME_TABLE             'a'                        // command:   ambilight night time table

#define CMD_CODE_ALARM_TIME_TABLE                       'l'                        // command:   alarm time table

unsigned int
rpc (RPC_VARIABLE var)
{
    unsigned int   rtc = 0;

    if (var < MAX_RPC_VARIABLES)
    {
        Serial.printf ("CMD R%02x\r\n", (int) var);
        Serial.flush ();
        rtc = 1;
    }

    return rtc;
}

unsigned int     numvars[MAX_NUM_VARIABLES];

unsigned int
get_numvar (NUM_VARIABLE var)
{
    unsigned int   rtc = 0;

    if (var < MAX_NUM_VARIABLES)
    {
        rtc =  numvars[var];
    }

    return rtc;
}

unsigned int
set_numvar (NUM_VARIABLE var, unsigned int value)
{
    unsigned int   rtc = 0;

    if (var < MAX_NUM_VARIABLES)
    {
        numvars[var] = value;
        Serial.printf ("CMD N%02x%02x%02x\r\n", (int) var, value & 0xFF, (value >> 8) & 0xFF);
        Serial.flush ();
        rtc = 1;
    }

    return rtc;
}

uint8_t   dimmed_display_colors[MAX_BRIGHTNESS + 1];
uint8_t   dimmed_ambilight_colors[MAX_BRIGHTNESS + 1];

uint_fast8_t
get_num8_array (NUM8_ARRAY var, uint32_t idx)
{
    uint_fast8_t  rtc = 0;

    switch (var)
    {
        case DISPLAY_DIMMED_DISPLAY_COLORS:
        {
            if (idx < MAX_BRIGHTNESS + 1)
            {
                rtc = dimmed_display_colors[idx];
            }
            break;
        }
        case DISPLAY_DIMMED_AMBILIGHT_COLORS:
        {
            if (idx < MAX_BRIGHTNESS + 1)
            {
                rtc = dimmed_ambilight_colors[idx];
            }
            break;
        }
    }

    return rtc;
}

uint_fast8_t
set_num8_array (NUM8_ARRAY var, uint32_t idx, uint_fast8_t value)
{
    uint_fast8_t  rtc = 0;

    switch (var)
    {
        case DISPLAY_DIMMED_DISPLAY_COLORS:
        {
            if (idx < MAX_BRIGHTNESS + 1)
            {
                dimmed_display_colors[idx] = value;
                Serial.printf ("CMD n%02x%02x%02x\r\n", (int) var, idx, value);
                Serial.flush ();
                rtc = 1;
            }
            break;
        }
        case DISPLAY_DIMMED_AMBILIGHT_COLORS:
        {
            if (idx < MAX_BRIGHTNESS + 1)
            {
                dimmed_ambilight_colors[idx] = value;
                Serial.printf ("CMD n%02x%02x%02x\r\n", (int) var, idx, value);
                Serial.flush ();
                rtc = 1;
            }
            break;
        }
    }

    return rtc;
}

#if 0 // uint16_t arrays, yet not used

uint_fast16_t
get_num16_array (NUM16_ARRAY var, uint32_t idx)
{
    uint_fast16_t  rtc = 0;

    switch (var)
    {
        case FOO_ARRAY:
            if (idx < FOO_ARRAY_ENTRIES)
            {
                rtc = foo_array[idx];
            }
            break;
    }

    return rtc;
}

uint_fast8_t
set_num16_array (NUM16_ARRAY var, uint32_t idx, uint_fast16_t value)
{
    uint_fast8_t  rtc = 0;

    switch (var)
    {
        case DFPLAYER_PLAYLIST_ARRAY:
            if (idx < MAX_PLAYLIST_TRACKS)
            {
                dfplayer_playlist[idx] = value;
                Serial.printf ("CMD m%02x%02x%02x%02x\r\n", (int) var, idx, value & 0xFF, (value >> 8) & 0xFF);
                Serial.flush ();
                rtc = 1;
            }
    }

    return rtc;
}

#endif // 0

static char ticker_text[MAX_TICKER_TEXT_LEN + 1];
static char version[MAX_VERSION_TEXT_LEN + 1];
static char eeprom_version[MAX_EEPROM_VERSION_TEXT_LEN + 1];
static char esp8266_version[MAX_ESP8266_VERSION_TEXT_LEN + 1];
static char timeserver_name[MAX_TIMESERVER_NAME_LEN + 1];
static char weather_appid[MAX_WEATHER_APPID_LEN + 1];
static char weather_city[MAX_WEATHER_CITY_LEN + 1];
static char weather_lon[MAX_WEATHER_LON_LEN + 1];
static char weather_lat[MAX_WEATHER_LAT_LEN + 1];
static char update_host[MAX_UPDATE_HOST_LEN + 1];
static char update_path[MAX_UPDATE_PATH_LEN + 1];
static char date_ticker_format[MAX_DATE_TICKER_FORMAT_LEN + 1];

STR_VAR strvars[MAX_STR_VARIABLES] =
{
    { ticker_text,          MAX_TICKER_TEXT_LEN },
    { version,              MAX_VERSION_TEXT_LEN },
    { eeprom_version,       MAX_EEPROM_VERSION_TEXT_LEN },
    { esp8266_version,      MAX_ESP8266_VERSION_TEXT_LEN },
    { timeserver_name,      MAX_TIMESERVER_NAME_LEN },
    { weather_appid,        MAX_WEATHER_APPID_LEN },
    { weather_city,         MAX_WEATHER_CITY_LEN },
    { weather_lon,          MAX_WEATHER_LON_LEN },
    { weather_lat,          MAX_WEATHER_LAT_LEN },
    { update_host,          MAX_UPDATE_HOST_LEN },
    { update_path,          MAX_UPDATE_PATH_LEN },
    { date_ticker_format,   MAX_DATE_TICKER_FORMAT_LEN },
};


STR_VAR *
get_strvar (STR_VARIABLE var)
{
    STR_VAR *   rtc = (STR_VAR *) 0;

    if (var < MAX_STR_VARIABLES)
    {
        rtc = &(strvars[var]);
    }

    return rtc;
}

unsigned int
set_strvar (STR_VARIABLE var, const char * p)
{
    unsigned int   rtc = 0;

    if (var < MAX_STR_VARIABLES)
    {
        strncpy (strvars[var].str, p, strvars[var].maxlen);
        Serial.printf ("CMD S%02x%s\r\n", (int) var, p);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

TM tmvars[MAX_TM_VARIABLES];

TM *
get_tm_var (TM_VARIABLE var)
{
    TM *   rtc = (TM *) 0;

    if (var < MAX_TM_VARIABLES)
    {
        rtc = &(tmvars[var]);
    }

    return rtc;

}

unsigned int
set_tm_var (TM_VARIABLE var, TM * tm)
{
    unsigned int   rtc = 0;

    if (var < MAX_TM_VARIABLES)
    {
        memcpy (&tmvars[var], tm, sizeof (TM));
        Serial.printf ("CMD T%02x%04d%02d%02d%02d%02d%02d\r\n", (int) var, tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

DSP_COLORS dspcolorvars[MAX_DSP_COLOR_VARIABLES];

unsigned int
get_dsp_color_var (DSP_COLOR_VARIABLE var, DSP_COLORS * t)
{
    unsigned int   rtc = 0;

    if (var < MAX_DSP_COLOR_VARIABLES)
    {
        memcpy (t, &(dspcolorvars[var]), sizeof (DSP_COLORS));
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_dsp_color_var (DSP_COLOR_VARIABLE var, DSP_COLORS * s, int use_rgbw)
{
    unsigned int   rtc = 0;

    if (var < MAX_DSP_COLOR_VARIABLES)
    {
        memcpy (&(dspcolorvars[var]), s, sizeof (DSP_COLORS));

        if (use_rgbw)
        {
            Serial.printf ("CMD DC%02x%02x%02x%02x%02x\r\n", (int) var, s->red, s->green, s->blue, s->white);
        }
        else
        {
            Serial.printf ("CMD DC%02x%02x%02x%02x\r\n", (int) var, s->red, s->green, s->blue);
        }

        Serial.flush ();
        rtc = 1;
    }

    return rtc;
}

DISPLAY_ANIMATION displayanimationvars[MAX_DISPLAY_ANIMATION_VARIABLES];

DISPLAY_ANIMATION *
get_display_animation_var (DISPLAY_ANIMATION_VARIABLE var)
{
    DISPLAY_ANIMATION *   rtc = (DISPLAY_ANIMATION *) 0;

    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        rtc = &(displayanimationvars[var]);
    }

    return rtc;
}

unsigned int
set_display_animation_name (DISPLAY_ANIMATION_VARIABLE var, char * name)
{
    unsigned int   rtc = 0;

    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        strncpy (displayanimationvars[var].name, name, MAX_DISPLAY_ANIMATION_NAME_LEN);
        Serial.printf ("CMD AN%02x%s\r\n", (int) var, name);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_display_animation_deceleration (DISPLAY_ANIMATION_VARIABLE var, uint_fast8_t deceleration)
{
    unsigned int   rtc = 0;

    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        displayanimationvars[var].deceleration = deceleration;
        Serial.printf ("CMD AD%02x%02x\r\n", (int) var, deceleration);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_display_animation_default_deceleration (DISPLAY_ANIMATION_VARIABLE var, uint_fast8_t default_deceleration)
{
    unsigned int   rtc = 0;

    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        displayanimationvars[var].default_deceleration = default_deceleration;
        Serial.printf ("CMD AE%02x%02x\r\n", (int) var, default_deceleration);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_display_animation_flags (DISPLAY_ANIMATION_VARIABLE var, uint_fast8_t flags)
{
    unsigned int   rtc = 0;

    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        displayanimationvars[var].flags = flags;
        Serial.printf ("CMD AF%02x%02x\r\n", (int) var, flags);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

COLOR_ANIMATION coloranimationvars[MAX_COLOR_ANIMATION_VARIABLES];

COLOR_ANIMATION *
get_color_animation_var (COLOR_ANIMATION_VARIABLE var)
{
    COLOR_ANIMATION *   rtc = (COLOR_ANIMATION *) 0;

    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        rtc = &(coloranimationvars[var]);
    }

    return rtc;
}

unsigned int
set_color_animation_name (COLOR_ANIMATION_VARIABLE var, char * name)
{
    unsigned int   rtc = 0;

    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        strncpy (coloranimationvars[var].name, name, MAX_COLOR_ANIMATION_NAME_LEN);
        Serial.printf ("CMD CN%02x%s\r\n", (int) var, name);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_color_animation_deceleration (COLOR_ANIMATION_VARIABLE var, uint_fast8_t deceleration)
{
    unsigned int   rtc = 0;

    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        coloranimationvars[var].deceleration = deceleration;
        Serial.printf ("CMD CD%02x%02x\r\n", (int) var, deceleration);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_color_animation_default_deceleration (COLOR_ANIMATION_VARIABLE var, uint_fast8_t default_deceleration)
{
    unsigned int   rtc = 0;

    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        coloranimationvars[var].default_deceleration = default_deceleration;
        Serial.printf ("CMD CE%02x%02x\r\n", (int) var, default_deceleration);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_color_animation_flags (COLOR_ANIMATION_VARIABLE var, uint_fast8_t flags)
{
    unsigned int   rtc = 0;

    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        coloranimationvars[var].flags = flags;
        Serial.printf ("CMD CF%02x%02x\r\n", (int) var, flags);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

AMBILIGHT_MODE ambilightmodevars[MAX_AMBILIGHT_MODE_VARIABLES];

AMBILIGHT_MODE *
get_ambilight_mode_var (AMBILIGHT_MODE_VARIABLE var)
{
    AMBILIGHT_MODE *   rtc = (AMBILIGHT_MODE *) 0;

    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        rtc = &(ambilightmodevars[var]);
    }

    return rtc;
}

unsigned int
set_ambilight_mode_name (AMBILIGHT_MODE_VARIABLE var, char * name)
{
    unsigned int   rtc = 0;

    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        strncpy (ambilightmodevars[var].name, name, MAX_AMBILIGHT_MODE_VARIABLES);
        Serial.printf ("CMD MN%02x%s\r\n", (int) var, name);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_ambilight_mode_deceleration (AMBILIGHT_MODE_VARIABLE var, uint_fast8_t deceleration)
{
    unsigned int   rtc = 0;

    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        ambilightmodevars[var].deceleration = deceleration;
        Serial.printf ("CMD MD%02x%02x\r\n", (int) var, deceleration);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_ambilight_mode_default_deceleration (AMBILIGHT_MODE_VARIABLE var, uint_fast8_t default_deceleration)
{
    unsigned int   rtc = 0;

    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        ambilightmodevars[var].default_deceleration = default_deceleration;
        Serial.printf ("CMD ME%02x%02x\r\n", (int) var, default_deceleration);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_ambilight_mode_flags (AMBILIGHT_MODE_VARIABLE var, uint_fast8_t flags)
{
    unsigned int   rtc = 0;

    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        ambilightmodevars[var].flags = flags;
        Serial.printf ("CMD MF%02x%02x\r\n", (int) var, flags);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

unsigned int
set_overlay_var (uint_fast8_t idx)
{
    Serial.printf ("CMD OT%02x%02x\r\n", idx, overlays[idx].type);
    Serial.printf ("CMD OI%02x%02x\r\n", idx, overlays[idx].interval);
    Serial.printf ("CMD OD%02x%02x\r\n", idx, overlays[idx].duration);
    Serial.printf ("CMD OC%02x%02x\r\n", idx, overlays[idx].date_code);
    Serial.printf ("CMD OS%02x%04x\r\n", idx, overlays[idx].date_start);
    Serial.printf ("CMD OY%02x%02x\r\n", idx, overlays[idx].days);
    Serial.printf ("CMD ON%02x%s\r\n",   idx, overlays[idx].text);
    Serial.printf ("CMD OF%02x%02x\r\n", idx, overlays[idx].flags);

    return 1;
}

NIGHT_TIME nighttimevars[MAX_NIGHT_TIME_VARIABLES];
NIGHT_TIME ambilightnighttimevars[MAX_NIGHT_TIME_VARIABLES];

NIGHT_TIME *
get_night_time_var (uint_fast8_t is_ambilight, NIGHT_TIME_VARIABLE var)
{
    NIGHT_TIME *   rtc = (NIGHT_TIME *) 0;

    if (var < MAX_NIGHT_TIME_VARIABLES)
    {
        if (is_ambilight)
        {
            rtc = &(ambilightnighttimevars[var]);
        }
        else
        {
            rtc = &(nighttimevars[var]);
        }
    }

    return rtc;
}

unsigned int
set_night_time_var (uint_fast8_t is_ambilight, NIGHT_TIME_VARIABLE var, uint_fast16_t minutes, uint_fast8_t flags)
{
    unsigned int   rtc = 0;

    if (var < MAX_NIGHT_TIME_VARIABLES)
    {
        if (is_ambilight)
        {
            ambilightnighttimevars[var].minutes = minutes;
            ambilightnighttimevars[var].flags = flags;
            Serial.printf ("CMD a%02x%02x%02x%02x\r\n", (int) var, minutes & 0xFF, (minutes >> 8) & 0xFF, flags);
        }
        else
        {
            nighttimevars[var].minutes = minutes;
            nighttimevars[var].flags = flags;
            Serial.printf ("CMD t%02x%02x%02x%02x\r\n", (int) var, minutes & 0xFF, (minutes >> 8) & 0xFF, flags);
        }
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

ALARM_TIME alarmtimevars[MAX_ALARM_TIME_VARIABLES];

ALARM_TIME *
get_alarm_time_var (ALARM_TIME_VARIABLE var)
{
    ALARM_TIME *   rtc = (ALARM_TIME *) 0;

    if (var < MAX_ALARM_TIME_VARIABLES)
    {
        rtc = &(alarmtimevars[var]);
    }

    return rtc;
}

unsigned int
set_alarm_time_var (ALARM_TIME_VARIABLE var, uint_fast16_t minutes, uint_fast8_t flags)
{
    unsigned int   rtc = 0;

    if (var < MAX_ALARM_TIME_VARIABLES)
    {
        alarmtimevars[var].minutes = minutes;
        alarmtimevars[var].flags = flags;
        Serial.printf ("CMD l%02x%02x%02x%02x\r\n", (int) var, minutes & 0xFF, (minutes >> 8) & 0xFF, flags);
        Serial.flush ();
        rtc =  1;
    }

    return rtc;
}

OVERLAY      overlays[MAX_OVERLAYS];

void
var_set_parameter (char * parameters)
{
    uint_fast8_t        cmd_code;
    uint_fast8_t        var_idx;

#ifdef DEBUG
    debugmsg ("VAR", parameters);
#endif

    cmd_code    = *parameters++;

    switch (cmd_code)
    {
        case CMD_CODE_NUMERIC_VAR:                                          // N: numeric variable: Niillhh
        {
            uint_fast8_t    lo;
            uint_fast8_t    hi;
            uint_fast16_t   val;

            var_idx = htoi (parameters, 2);
            parameters += 2;
            lo      = htoi (parameters, 2);
            parameters += 2;
            hi      = htoi (parameters, 2);
            parameters += 2;
            val     = (hi << 8) | lo;

            if (var_idx < MAX_NUM_VARIABLES)
            {
                numvars[var_idx] = val;
            }
            break;
        }

        case CMD_CODE_NUMERIC_ARRAY:                                        // A: numeric array: Niinnbb
        {
            uint_fast8_t    n;
            uint_fast8_t    val;

            var_idx = htoi (parameters, 2);
            parameters += 2;

            n = htoi (parameters, 2);
            parameters += 2;

            val = htoi (parameters, 2);

            switch (var_idx)
            {
                case DISPLAY_DIMMED_DISPLAY_COLORS:
                {
                    if (n < sizeof (dimmed_display_colors))
                    {
                        if (val <= MAX_BRIGHTNESS)
                        {
                            dimmed_display_colors[n] = val;
                        }
                    }
                    break;
                }
                case DISPLAY_DIMMED_AMBILIGHT_COLORS:
                {
                    if (n < sizeof (dimmed_ambilight_colors))
                    {
                        if (val <= MAX_BRIGHTNESS)
                        {
                            dimmed_ambilight_colors[n] = val;
                        }
                    }
                    break;
                }
            }
            break;
        }

        case CMD_CODE_STRING_VAR:                                           // S string variable: Siissssssss...
        {
            var_idx = htoi (parameters, 2);
            parameters += 2;

            if (var_idx < MAX_STR_VARIABLES)
            {
                strncpy (strvars[var_idx].str, parameters, strvars[var_idx].maxlen);
            }
            break;
        }

        case CMD_CODE_TIME_VAR:                                             // T time: TYYYYMMDDhhmmss
        {
            var_idx = htoi (parameters, 2);
            parameters += 2;

            if (var_idx < MAX_TM_VARIABLES)
            {
                tmvars[var_idx].tm_year = 1000 * (parameters[0]  - '0') +
                                           100 * (parameters[1]  - '0') +
                                            10 * (parameters[2]  - '0') +
                                             1 * (parameters[3]  - '0');
                tmvars[var_idx].tm_mon  =   10 * (parameters[4]  - '0') +
                                             1 * (parameters[5]  - '0');
                tmvars[var_idx].tm_mday =   10 * (parameters[6]  - '0') +
                                             1 * (parameters[7]  - '0');
                tmvars[var_idx].tm_hour =   10 * (parameters[8]  - '0') +
                                             1 * (parameters[9]  - '0');
                tmvars[var_idx].tm_min  =   10 * (parameters[10] - '0') +
                                             1 * (parameters[11] - '0');
                tmvars[var_idx].tm_sec =    10 * (parameters[12] - '0') +
                                             1 * (parameters[13] - '0');

                tmvars[var_idx].tm_wday = dayofweek (tmvars[var_idx].tm_mday, tmvars[var_idx].tm_mon + 1, tmvars[var_idx].tm_year + 1900);
            }

            break;
        }

        case CMD_CODE_DISPLAY_VAR:                                          // D: display
        {
            cmd_code = *parameters++;
            var_idx = htoi (parameters, 2);
            parameters += 2;

            switch (cmd_code)
            {
                case PAR_CODE_DISPLAY_COLOR:                                // DC: Display Color
                {
                    uint_fast8_t use_rgbw = get_numvar (DISPLAY_USE_RGBW_NUM_VAR);

                    if (var_idx < MAX_DSP_COLOR_VARIABLES)
                    {
                        dspcolorvars[var_idx].red = htoi (parameters, 2);
                        parameters += 2;
                        dspcolorvars[var_idx].green = htoi (parameters, 2);
                        parameters += 2;
                        dspcolorvars[var_idx].blue = htoi (parameters, 2);
                        parameters += 2;

                        if (use_rgbw)
                        {
                            dspcolorvars[var_idx].white = htoi (parameters, 2);
                            parameters += 2;
                        }
                    }
                    break;
                }
            }
            break;
        }

        case CMD_CODE_ANIMATION_VAR:                                        // A: animation
        {
            cmd_code = *parameters++;
            var_idx = htoi (parameters, 2);
            parameters += 2;

            switch (cmd_code)
            {
                case PAR_CODE_ANIMATION_MODE_NAME:                          // AN: Animation mode Name
                {
                    if (var_idx < MAX_DISPLAY_ANIMATION_VARIABLES)
                    {
                        strncpy (displayanimationvars[var_idx].name, parameters, MAX_DISPLAY_ANIMATION_NAME_LEN);
                    }
                    break;
                }

                case PAR_CODE_ANIMATION_DECELERATION:                       // AD: Animation Deceleration
                {
                    if (var_idx < MAX_DISPLAY_ANIMATION_VARIABLES)
                    {
                        uint_fast8_t deceleration = htoi (parameters, 2);
                        parameters += 2;
                        displayanimationvars[var_idx].deceleration = deceleration;
                    }
                    break;
                }

                case PAR_CODE_ANIMATION_DEFAULT_DECELERATION:               // AE: Animation default deceleration
                {
                    if (var_idx < MAX_DISPLAY_ANIMATION_VARIABLES)
                    {
                        uint_fast8_t default_deceleration = htoi (parameters, 2);
                        parameters += 2;
                        displayanimationvars[var_idx].default_deceleration = default_deceleration;
                    }
                    break;
                }

                case PAR_CODE_ANIMATION_FLAGS:                              // AF: Animation Flags
                {
                    if (var_idx < MAX_DISPLAY_ANIMATION_VARIABLES)
                    {
                        uint_fast8_t flags = htoi (parameters, 2);
                        parameters += 2;
                        displayanimationvars[var_idx].flags = flags;
                    }
                    break;
                }
            }
            break;
        }

        case CMD_CODE_COLOR_ANIMATION_VAR:                                  // C: Color animation
        {
            cmd_code = *parameters++;
            var_idx = htoi (parameters, 2);
            parameters += 2;

            switch (cmd_code)
            {
                case PAR_CODE_COLOR_ANIMATION_MODE_NAME:                    // CN: Color animation Name
                {
                    if (var_idx < MAX_COLOR_ANIMATION_VARIABLES)
                    {
                        strncpy (coloranimationvars[var_idx].name, parameters, MAX_COLOR_ANIMATION_NAME_LEN);
                    }
                    break;
                }

                case PAR_CODE_COLOR_ANIMATION_DECELERATION:                 // CD: Color animation Deceleration
                {
                    if (var_idx < MAX_COLOR_ANIMATION_VARIABLES)
                    {
                        uint_fast8_t deceleration = htoi (parameters, 2);
                        parameters += 2;
                        coloranimationvars[var_idx].deceleration = deceleration;
                    }
                    break;
                }

                case PAR_CODE_COLOR_ANIMATION_DEFAULT_DECELERATION:         // CE: Color animation default deceleration
                {
                    if (var_idx < MAX_COLOR_ANIMATION_VARIABLES)
                    {
                        uint_fast8_t default_deceleration = htoi (parameters, 2);
                        parameters += 2;
                        coloranimationvars[var_idx].default_deceleration = default_deceleration;
                    }
                    break;
                }

                case PAR_CODE_COLOR_ANIMATION_FLAGS:                        // CF: Animation Flags
                {
                    if (var_idx < MAX_COLOR_ANIMATION_VARIABLES)
                    {
                        uint_fast8_t flags = htoi (parameters, 2);
                        parameters += 2;
                        coloranimationvars[var_idx].flags = flags;
                    }
                    break;
                }
            }
            break;
        }

        case CMD_CODE_AMBILIGHT_MODE_VAR:                                   // Ambilight mode
        {
            cmd_code = *parameters++;
            var_idx = htoi (parameters, 2);
            parameters += 2;

            switch (cmd_code)
            {
                case PAR_CODE_AMBILIGHT_MODE_NAME:                          // MN: Ambilight mode Name
                {
                    if (var_idx < MAX_AMBILIGHT_MODE_VARIABLES)
                    {
                        strncpy (ambilightmodevars[var_idx].name, parameters, MAX_AMBILIGHT_MODE_NAME_LEN);
                    }
                    break;
                }

                case PAR_CODE_AMBILIGHT_MODE_DECELERATION:                  // MD: Ambilight mode Deceleration
                {
                    if (var_idx < MAX_AMBILIGHT_MODE_VARIABLES)
                    {
                        uint_fast8_t deceleration = htoi (parameters, 2);
                        parameters += 2;
                        ambilightmodevars[var_idx].deceleration = deceleration;
                    }
                    break;
                }

                case PAR_CODE_AMBILIGHT_MODE_DEFAULT_DECELERATION:          // ME: Ambilight mode default deceleration
                {
                    if (var_idx < MAX_AMBILIGHT_MODE_VARIABLES)
                    {
                        uint_fast8_t default_deceleration = htoi (parameters, 2);
                        parameters += 2;
                        ambilightmodevars[var_idx].default_deceleration = default_deceleration;
                    }
                    break;
                }

                case PAR_CODE_AMBILIGHT_MODE_FLAGS:                         // MF: Ambilight mode Flags
                {
                    if (var_idx < MAX_AMBILIGHT_MODE_VARIABLES)
                    {
                        uint_fast8_t flags = htoi (parameters, 2);
                        parameters += 2;
                        ambilightmodevars[var_idx].flags = flags;
                    }
                    break;
                }
            }
            break;
        }

        case CMD_CODE_OVERLAY_VAR:                                          // O: overlay
        {
            cmd_code = *parameters++;
            var_idx = htoi (parameters, 2);
            parameters += 2;

            switch (cmd_code)
            {
                case PAR_CODE_OVERLAY_TYPE:                                 // OT: overlay type
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast8_t type = htoi (parameters, 2);
                        parameters += 2;
                        overlays[var_idx].type = type;
                    }
                    break;
                case PAR_CODE_OVERLAY_INTERVAL:                             // OI: overlay interval
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast8_t interval = htoi (parameters, 2);
                        parameters += 2;
                        overlays[var_idx].interval = interval;
                    }
                    break;
                case PAR_CODE_OVERLAY_DURATION:                             // OD: overlay duration
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast8_t duration = htoi (parameters, 2);
                        parameters += 2;
                        overlays[var_idx].duration = duration;
                    }
                    break;
                case PAR_CODE_OVERLAY_DATE_CODE:                            // OC: overlay date code
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast8_t date_code = htoi (parameters, 2);
                        parameters += 2;
                        overlays[var_idx].date_code = date_code;
                    }
                    break;
                case PAR_CODE_OVERLAY_DATE_START:                           // OS: overlay date start
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast16_t date_start = htoi (parameters, 4);
                        parameters += 4;
                        overlays[var_idx].date_start = date_start;
                    }
                    break;
                case PAR_CODE_OVERLAY_DAYS:                                 // OY: overlay date end
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast16_t days = htoi (parameters, 2);
                        parameters += 2;
                        overlays[var_idx].days = days;
                    }
                    break;
                case PAR_CODE_OVERLAY_TEXT:                                 // ON: overlay name or text
                    if (var_idx < MAX_OVERLAYS)
                    {
                        strncpy (overlays[var_idx].text, parameters, OVERLAY_MAX_TEXT_LEN);
                    }
                    break;
                case PAR_CODE_OVERLAY_FLAGS:                                // OF: overlay flags
                    if (var_idx < MAX_OVERLAYS)
                    {
                        uint_fast8_t flags = htoi (parameters, 2);
                        parameters += 2;
                        overlays[var_idx].flags = flags;
                    }
                    break;
            }
            break;
        }

        case CMD_CODE_NIGHT_TIME_TABLE:                                     // tiimm night table: minutes + flags
        case CMD_CODE_AMBILIGHT_NIGHT_TIME_TABLE:                           // aiimm ambilight night table: minutes + flags
        {
            uint_fast16_t minutes;
            uint_fast8_t flags;

            var_idx = htoi (parameters, 2);
            parameters += 2;
            minutes = htoi (parameters, 2) + (htoi (parameters + 2, 2) << 8);
            parameters += 4;
            flags = htoi (parameters, 2);
            parameters += 2;

            if (var_idx < MAX_NIGHT_TIME_VARIABLES)
            {
                if (cmd_code == 't')
                {
                    nighttimevars[var_idx].minutes = minutes;
                    nighttimevars[var_idx].flags = flags;
                }
                else
                {
                    ambilightnighttimevars[var_idx].minutes = minutes;
                    ambilightnighttimevars[var_idx].flags = flags;
                }
            }

            break;
        }

        case CMD_CODE_ALARM_TIME_TABLE:                                     // liimm alarm table: minutes + flags
        {
            uint_fast16_t minutes;
            uint_fast8_t flags;

            var_idx = htoi (parameters, 2);
            parameters += 2;
            minutes = htoi (parameters, 2) + (htoi (parameters + 2, 2) << 8);
            parameters += 4;
            flags = htoi (parameters, 2);
            parameters += 2;

            if (var_idx < MAX_ALARM_TIME_VARIABLES)
            {
                alarmtimevars[var_idx].minutes = minutes;
                alarmtimevars[var_idx].flags = flags;
            }

            break;
        }
    }
}

void
vars_init (void)
{
    numvars[HARDWARE_CONFIGURATION_NUM_VAR] = 0xFFFF;
}

