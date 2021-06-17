/*-------------------------------------------------------------------------------------------------------------------------------------------
 * vars.c - synchronisation of variables/parameters between STM32 and ESP8266
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>

#include "vars.h"
#include "display.h"
#include "overlay.h"
#include "eeprom.h"
#include "eeprom-data.h"
#include "main.h"
#include "power.h"
#include "ldr.h"
#include "tempsensor.h"
#include "ds18xx.h"
#include "rtc.h"
#include "timeserver.h"
#include "night.h"
#include "alarm.h"
#include "weather.h"
#include "dfplayer.h"
#include "delay.h"

#include "log.h"
#include "esp8266.h"

#undef UART_PREFIX
#define UART_PREFIX         esp8266
#include "uart.h"

typedef struct tm   TM;
uint_fast8_t        var_send_busy;

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send buffer to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_buf (char * buf)
{
    uint_fast8_t    rtc;

    esp8266_uart_puts ("var ");
    esp8266_uart_puts (buf);
    esp8266_uart_puts ("\r\n");
    esp8266_uart_flush ();

    debug_log_printf ("var_send: %s\r\n", buf);
    debug_log_flush ();

    var_send_busy = 1;

    while ((rtc = schedule_esp8266_messages ()) != ESP8266_OK)
    {
        ;
    }
}

static void
var_send_byte (const char * id, uint_fast32_t var, uint_fast8_t value)
{
    char            buf[32];

    sprintf (buf, "%s%02x%02x", id, (int) var, value);
    var_send_buf (buf);
}

static void
var_send_short (const char * id, uint_fast32_t var, uint_fast16_t value)
{
    char            buf[32];

    sprintf (buf, "%s%02x%04x", id, (int) var, value & 0xFFFF);
    var_send_buf (buf);
}

static void
var_send_string (const char * id, uint_fast32_t var, const char * value)
{
    char            buf[64];

    sprintf (buf, "%s%02x%s", id, (int) var, value);
    var_send_buf (buf);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a numeric variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_num_variable (NUM_VARIABLE var, unsigned int value)
{
    char            buf[32];

    if (var < MAX_NUM_VARIABLES)
    {
        sprintf (buf, "N%02x%02x%02x", (int) var, value & 0xFF, (value >> 8) & 0xFF);
        var_send_buf (buf);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a numeric array (n * uint8_t) to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_num8_array (NUM8_ARRAY var, uint_fast8_t * p, uint_fast8_t n)
{
    char            buf[32];
    uint_fast8_t    i;

    if (var < MAX_NUM8_ARRAYS)
    {
        for (i = 0; i < n; i++)
        {
            sprintf (buf, "n%02x%02x%02x", var, i, p[i]);
            var_send_buf (buf);
        }
    }
}

#if 0 // yet not used
/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a numeric (n * uint16_t) array to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_num16_array (NUM16_ARRAY var, uint_fast16_t * p, uint_fast8_t n)
{
    char            buf[32];
    uint_fast8_t    i;

    if (var < MAX_NUM16_ARRAYS)
    {
        for (i = 0; i < n; i++)
        {
            sprintf (buf, "m%02x%02x%02x%02x", var, i, p[i] & 0xFF, (p[i] >> 8) & 0xFF);
            var_send_buf (buf);
        }
    }
}
#endif // 0

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a string variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_str_variable (STR_VARIABLE var, const char * value)
{
    char            buf[64];

    if (var < MAX_STR_VARIABLES && value)
    {
        sprintf (buf, "S%02x%s", (int) var, value);
        var_send_buf (buf);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a tm variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_tm_variable (TM_VARIABLE var, TM * tm)
{
    char            buf[32];

    if (var < MAX_TM_VARIABLES)
    {
        sprintf (buf, "T%02x%04d%02d%02d%02d%02d%02d", (int) var, tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        var_send_buf (buf);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a dsp color variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_dsp_color_variable (DSP_COLOR_VARIABLE var, DSP_COLORS * dsp_colors)
{
    char buf[32];

#if DSP_USE_SK6812_RGBW == 1
    sprintf (buf, "DC%02x%02x%02x%02x%02x", (int) var, dsp_colors->red, dsp_colors->green, dsp_colors->blue, dsp_colors->white);
#else
    sprintf (buf, "DC%02x%02x%02x%02x00", (int) var, dsp_colors->red, dsp_colors->green, dsp_colors->blue);
#endif
    var_send_buf (buf);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send an animation variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_display_animation_name (DISPLAY_ANIMATION_VARIABLE var, const char * name)
{
    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        var_send_string ("AN", (uint_fast32_t) var, name);
    }
}

static void
var_send_display_animation_deceleration (DISPLAY_ANIMATION_VARIABLE var, uint_fast8_t deceleration)
{
    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        var_send_byte ("AD", (uint_fast32_t) var, deceleration);
    }
}

static void
var_send_display_animation_default_deceleration (DISPLAY_ANIMATION_VARIABLE var, uint_fast8_t default_deceleration)
{
    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        var_send_byte ("AE", (uint_fast32_t) var, default_deceleration);
    }
}

static void
var_send_display_animation_flags (DISPLAY_ANIMATION_VARIABLE var, uint_fast8_t flags)
{
    if (var < MAX_DISPLAY_ANIMATION_VARIABLES)
    {
        var_send_byte ("AF", (uint_fast32_t) var, flags);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a color animation variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_color_animation_name (COLOR_ANIMATION_VARIABLE var, const char * name)
{
    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        var_send_string ("CN", (uint_fast32_t) var, name);
    }
}

static void
var_send_color_animation_deceleration (COLOR_ANIMATION_VARIABLE var, uint_fast8_t deceleration)
{
    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        var_send_byte ("CD", (uint_fast32_t) var, deceleration);
    }
}

static void
var_send_color_animation_default_deceleration (COLOR_ANIMATION_VARIABLE var, uint_fast8_t default_deceleration)
{
    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        var_send_byte ("CE", (uint_fast32_t) var, default_deceleration);
    }
}

static void
var_send_color_animation_flags (COLOR_ANIMATION_VARIABLE var, uint_fast8_t flags)
{
    if (var < MAX_COLOR_ANIMATION_VARIABLES)
    {
        var_send_byte ("CF", (uint_fast32_t) var, flags);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a ambilight mode variable to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_ambilight_mode_name (AMBILIGHT_MODE_VARIABLE var, const char * name)
{
    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        var_send_string ("MN", (uint_fast32_t) var, name);
    }
}

static void
var_send_ambilight_mode_deceleration (AMBILIGHT_MODE_VARIABLE var, uint_fast8_t deceleration)
{
    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        var_send_byte ("MD", (uint_fast32_t) var, deceleration);
    }
}

static void
var_send_ambilight_mode_default_deceleration (AMBILIGHT_MODE_VARIABLE var, uint_fast8_t default_deceleration)
{
    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        var_send_byte ("ME", (uint_fast32_t) var, default_deceleration);
    }
}

static void
var_send_ambilight_mode_flags (AMBILIGHT_MODE_VARIABLE var, uint_fast8_t flags)
{
    if (var < MAX_AMBILIGHT_MODE_VARIABLES)
    {
        var_send_byte ("MF", (uint_fast32_t) var, flags);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send overlay data to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_n_overlays (uint_fast8_t n_overlays)
{
    var_send_num_variable (OVERLAY_N_OVERLAYS_NUM_VAR, n_overlays);
}

static void
var_send_overlay_type (uint32_t var_idx, uint_fast8_t type)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_byte ("OT", (uint_fast32_t) var_idx, type);
    }
}

static void
var_send_overlay_interval (uint32_t var_idx, uint_fast8_t interval)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_byte ("OI", (uint_fast32_t) var_idx, interval);
    }
}

static void
var_send_overlay_duration (uint32_t var_idx, uint_fast8_t duration)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_byte ("OD", var_idx, duration);
    }
}

static void
var_send_overlay_date_code (uint32_t var_idx, uint_fast8_t date_code)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_byte ("OC", var_idx, date_code);
    }
}

static void
var_send_overlay_date_start (uint32_t var_idx, uint_fast16_t date_start)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_short ("OS", var_idx, date_start);
    }
}

static void
var_send_overlay_days (uint32_t var_idx, uint_fast8_t days)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_byte ("OY", var_idx, days);
    }
}

static void
var_send_overlay_text (uint32_t var_idx, const char * text)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_string ("ON", var_idx, text);
    }
}

static void
var_send_overlay_flags (uint32_t var_idx, uint_fast8_t flags)
{
    if (var_idx < overlay.n_overlays)
    {
        var_send_byte ("OF", var_idx, flags);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send night time variables to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_night_time (NIGHT_TIME_VARIABLE var, uint_fast16_t minutes, uint_fast8_t flags)
{
    char buf[32];

    if (var < MAX_NIGHT_TIME_VARIABLES)
    {
        sprintf (buf, "t%02x%02x%02x%02x", (int) var, minutes & 0xFF, (minutes >> 8) & 0xFF, flags);
        var_send_buf (buf);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send ambilight night time variables to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_ambilight_night_time (NIGHT_TIME_VARIABLE var, uint_fast16_t minutes, uint_fast8_t flags)
{
    char buf[32];

    if (var < MAX_NIGHT_TIME_VARIABLES)
    {
        sprintf (buf, "a%02x%02x%02x%02x", (int) var, minutes & 0xFF, (minutes >> 8) & 0xFF, flags);
        var_send_buf (buf);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send alarm time variables to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_alarm_time (ALARM_TIME_VARIABLE var, uint_fast16_t minutes, uint_fast8_t flags)
{
    char buf[32];

    if (var < MAX_ALARM_TIME_VARIABLES)
    {
        sprintf (buf, "l%02x%02x%02x%02x", (int) var, minutes & 0xFF, (minutes >> 8) & 0xFF, flags);
        var_send_buf (buf);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send global variables to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
var_send_use_rgbw (void)
{
#if DSP_USE_SK6812_RGBW == 1
    var_send_num_variable (DISPLAY_USE_RGBW_NUM_VAR, 1);
#else
    var_send_num_variable (DISPLAY_USE_RGBW_NUM_VAR, 0);
#endif
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send global variables to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
var_send_eeprom_is_up (void)
{
    var_send_num_variable (EEPROM_IS_UP_NUM_VAR, eeprom_is_up);
}

static void
var_send_hardware_configuration (void)
{
    var_send_num_variable (HARDWARE_CONFIGURATION_NUM_VAR, gmain.hardware_configuration);
}

static void
var_send_rtc_is_up (void)
{
    var_send_num_variable (RTC_IS_UP_NUM_VAR, grtc.rtc_is_up);
}

void
var_send_display_power (void)
{
    var_send_num_variable (DISPLAY_POWER_NUM_VAR, display.display_power_is_on);
}

static void
var_send_display_ambilight_power (void)
{
    var_send_num_variable (DISPLAY_AMBILIGHT_POWER_NUM_VAR, display.ambilight_power_is_on);
}

void
var_send_display_mode (void)
{
    var_send_num_variable (DISPLAY_MODE_NUM_VAR, display.display_mode);
}

void
var_send_display_brightness (void)
{
    var_send_num_variable (DISPLAY_BRIGHTNESS_NUM_VAR, display.display_brightness);
}

static void
var_send_display_flags (void)
{
    var_send_num_variable (DISPLAY_FLAGS_NUM_VAR, display.display_flags);
}

void
var_send_display_automatic_brightness_active (void)
{
    var_send_num_variable (DISPLAY_AUTOMATIC_BRIGHTNESS_ACTIVE_NUM_VAR, display.automatic_brightness);
}

void
var_send_animation_mode (void)
{
    var_send_num_variable (ANIMATION_MODE_NUM_VAR, display.animation_mode);
}

void
var_send_ambilight_mode (void)
{
    var_send_num_variable (AMBILIGHT_MODE_NUM_VAR, display.ambilight_mode);
}

static void
var_send_ambilight_leds (void)
{
    var_send_num_variable (AMBILIGHT_LEDS_NUM_VAR, display.ambilight_leds);
}

static void
var_send_ambilight_offset (void)
{
    var_send_num_variable (AMBILIGHT_OFFSET_NUM_VAR, display.ambilight_led_offset);
}

void
var_send_ambilight_brightness (void)
{
    var_send_num_variable (AMBILIGHT_BRIGHTNESS_NUM_VAR, display.ambilight_brightness);
}

void
var_send_color_animation_mode (void)
{
    var_send_num_variable (COLOR_ANIMATION_MODE_NUM_VAR, display.color_animation_mode);
}

void
var_send_ldr_raw_value (void)
{
    var_send_num_variable (LDR_RAW_VALUE_NUM_VAR, ldr.ldr_raw_value);
}

void
var_send_ldr_min_value (void)
{
    var_send_num_variable (LDR_MIN_VALUE_NUM_VAR, ldr.ldr_min_value);
}

void
var_send_ldr_max_value (void)
{
    var_send_num_variable (LDR_MAX_VALUE_NUM_VAR, ldr.ldr_max_value);
}

static void
var_send_timezone (void)
{
    uint16_t    tz;

    if (timeserver.timezone < 0)
    {
        tz = -timeserver.timezone;
        tz |= 0x100;
    }
    else
    {
        tz = timeserver.timezone;
    }

    var_send_num_variable (TIMEZONE_NUM_VAR, tz);
}

static void
var_send_ds18xx_is_up (void)
{
    var_send_num_variable (DS18XX_IS_UP_NUM_VAR, ds18xx.is_up);
}

void
var_send_rtc_temp_index (void)
{
    var_send_num_variable (RTC_TEMP_INDEX_NUM_VAR, grtc.rtc_temperature_index);
}

static void
var_send_rtc_temp_correction (void)
{
    var_send_num_variable (RTC_TEMP_CORRECTION_NUM_VAR, grtc.rtc_temp_correction);
}

void
var_send_ds18xx_temp_index (void)
{
    var_send_num_variable (DS18XX_TEMP_INDEX_NUM_VAR, gtemp.index);
}

static void
var_send_ds18xx_temp_correction (void)
{
    var_send_num_variable (DS18XX_TEMP_CORRECTION_NUM_VAR, gtemp.correction);
}

static void
var_send_ticker_deceleration (void)
{
    var_send_num_variable (TICKER_DECELRATION_NUM_VAR, display.ticker_deceleration);
}

static void
var_send_ticker_text (void)
{
    ; // nothing to do
}

void
var_send_version (void)
{
    var_send_str_variable (VERSION_STR_VAR, VERSION);
}

void
var_send_eeprom_version (void)
{
    var_send_str_variable (EEPROM_VERSION_STR_VAR, gmain.eeprom_version);
}

static void
var_send_esp8266_version (void)
{
    // nothing to do
}

void
var_send_timeserver (void)
{
    var_send_str_variable (TIMESERVER_STR_VAR, timeserver.timeserver);
}

void
var_send_weather_appid (void)
{
    var_send_str_variable (WEATHER_APPID_STR_VAR, weather.appid);
}

void
var_send_weather_city (void)
{
    var_send_str_variable (WEATHER_CITY_STR_VAR, weather.city);
}

void
var_send_weather_lon (void)
{
    var_send_str_variable (WEATHER_LON_STR_VAR, weather.lon);
}

void
var_send_weather_lat (void)
{
    var_send_str_variable (WEATHER_LAT_STR_VAR, weather.lat);
}

static void
var_send_update_host (void)
{
    var_send_str_variable (UPDATE_HOST_VAR, gmain.update_host);
}

static void
var_send_update_path (void)
{
    var_send_str_variable (UPDATE_PATH_VAR, gmain.update_path);
}

static void
var_send_date_ticker_format (void)
{
    var_send_str_variable (DATE_TICKER_FORMAT_VAR, (char *) display.date_ticker_format);
}

void
var_send_tm (void)
{
    var_send_tm_variable (CURRENT_TM_VAR, &(gmain.tm));
}

void
var_send_display_colors (void)
{
    var_send_dsp_color_variable (DISPLAY_DSP_COLOR_VAR, &(display.display_colors));
}

void
var_send_ambilight_colors (void)
{
    var_send_dsp_color_variable (AMBILIGHT_DSP_COLOR_VAR, &(display.ambilight_colors));
}

void
var_send_ambilight_marker_colors (void)
{
    var_send_dsp_color_variable (AMBILIGHT_MARKER_DSP_COLOR_VAR, &(display.ambilight_marker_colors));
}

void
var_send_display_animations (void)
{
    DISPLAY_ANIMATION_VARIABLE  idx;

    for (idx = 0; idx < ANIMATION_MODES; idx++)
    {
        var_send_display_animation_name (idx, display.animations[idx].name);
        var_send_display_animation_deceleration (idx, display.animations[idx].deceleration);
        var_send_display_animation_default_deceleration (idx, display.animations[idx].default_deceleration);
        var_send_display_animation_flags (idx, display.animations[idx].flags);
    }
}

void
var_send_color_animations (void)
{
    COLOR_ANIMATION_VARIABLE  idx;

    for (idx = 0; idx < COLOR_ANIMATION_MODES; idx++)
    {
        var_send_color_animation_name (idx, display.color_animations[idx].name);
        var_send_color_animation_deceleration (idx, display.color_animations[idx].deceleration);
        var_send_color_animation_default_deceleration (idx, display.color_animations[idx].default_deceleration);
        var_send_color_animation_flags (idx, display.color_animations[idx].flags);
    }
}

void
var_send_ambilight_modes (void)
{
    AMBILIGHT_MODE_VARIABLE  idx;

    for (idx = 0; idx < AMBILIGHT_MODES; idx++)
    {
        var_send_ambilight_mode_name (idx, display.ambilight_modes[idx].name);
        var_send_ambilight_mode_deceleration (idx, display.ambilight_modes[idx].deceleration);
        var_send_ambilight_mode_default_deceleration (idx, display.ambilight_modes[idx].default_deceleration);
        var_send_ambilight_mode_flags (idx, display.ambilight_modes[idx].flags);
    }
}

void
var_send_overlays (void)
{
    uint32_t    idx;

    var_send_n_overlays (overlay.n_overlays);

    for (idx = 0; idx < overlay.n_overlays; idx++)
    {
        var_send_overlay_type (idx, overlay.overlays[idx].type);
        var_send_overlay_interval (idx, overlay.overlays[idx].interval);
        var_send_overlay_duration (idx, overlay.overlays[idx].duration);
        var_send_overlay_date_code (idx, overlay.overlays[idx].date_code);
        var_send_overlay_date_start (idx, overlay.overlays[idx].date_start);
        var_send_overlay_days (idx, overlay.overlays[idx].days);
        var_send_overlay_text (idx, overlay.overlays[idx].text);
        var_send_overlay_flags (idx, overlay.overlays[idx].flags);
    }
}

static void
var_send_night_times (void)
{
    NIGHT_TIME_VARIABLE  idx;

    for (idx = 0; idx < MAX_NIGHT_TIMES; idx++)
    {
        var_send_night_time (idx, night_time[idx].minutes, night_time[idx].flags);
    }
}

static void
var_send_ambilight_night_times (void)
{
    NIGHT_TIME_VARIABLE  idx;

    for (idx = 0; idx < MAX_NIGHT_TIMES; idx++)
    {
        var_send_ambilight_night_time (idx, ambilight_night_time[idx].minutes, ambilight_night_time[idx].flags);
    }
}

static void
var_send_alarm_times (void)
{
    ALARM_TIME_VARIABLE  idx;

    for (idx = 0; idx < MAX_ALARM_TIMES; idx++)
    {
        var_send_alarm_time (idx, alarm_time[idx].minutes, alarm_time[idx].flags);
    }
}

static void
var_send_dimmed_display_colors (void)
{
    var_send_num8_array (DISPLAY_DIMMED_DISPLAY_COLORS, display.dimmed_display_colors, sizeof (display.dimmed_display_colors));
}

static void
var_send_dimmed_ambilight_colors (void)
{
    var_send_num8_array (DISPLAY_DIMMED_AMBILIGHT_COLORS, display.dimmed_ambilight_colors, sizeof (display.dimmed_ambilight_colors));
}

static void
var_send_dfplayer_is_up (void)
{
    var_send_num_variable (DFPLAYER_IS_UP_NUM_VAR, dfplayer.is_up);
}

static void
var_send_dfplayer_version (void)
{
    var_send_num_variable (DFPLAYER_VERSION_NUM_VAR, dfplayer.version);
}

static void
var_send_dfplayer_volume (void)
{
    var_send_num_variable (DFPLAYER_VOLUME_NUM_VAR, dfplayer.volume);
}

static void
var_send_dfplayer_silence_start (void)
{
    var_send_num_variable (DFPLAYER_SILENCE_START_NUM_VAR, dfplayer.silence_start);
}

static void
var_send_dfplayer_silence_stop (void)
{
    var_send_num_variable (DFPLAYER_SILENCE_STOP_NUM_VAR, dfplayer.silence_stop);
}

static void
var_send_dfplayer_mode (void)
{
    var_send_num_variable (DFPLAYER_MODE_NUM_VAR, dfplayer.mode);
}

static void
var_send_dfplayer_bell_flags (void)
{
    var_send_num_variable (DFPLAYER_BELL_FLAGS_NUM_VAR, dfplayer.bell_flags);
}

static void
var_send_dfplayer_speak_cycle (void)
{
    var_send_num_variable (DFPLAYER_SPEAK_CYCLE_NUM_VAR, dfplayer.speak_cycle);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send all variables to ESP8266
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
var_send_all_variables (void)
{
    var_send_use_rgbw ();
    var_send_eeprom_is_up ();
    var_send_hardware_configuration ();
    var_send_rtc_is_up ();
    var_send_display_power ();
    var_send_display_ambilight_power ();
    var_send_display_mode ();
    var_send_display_brightness ();
    var_send_display_flags ();
    var_send_display_automatic_brightness_active ();
    var_send_animation_mode ();
    var_send_ambilight_mode ();
    var_send_ambilight_leds ();
    var_send_ambilight_offset ();
    var_send_ambilight_brightness ();
    var_send_color_animation_mode ();
    var_send_ldr_raw_value ();
    var_send_ldr_min_value ();
    var_send_ldr_max_value ();
    var_send_timezone ();
    var_send_ds18xx_is_up ();
    var_send_rtc_temp_index ();
    var_send_rtc_temp_correction ();
    var_send_ds18xx_temp_index ();
    var_send_ds18xx_temp_correction ();
    var_send_ticker_deceleration ();
    var_send_ticker_text ();
    var_send_version ();
    var_send_eeprom_version ();
    var_send_esp8266_version ();
    var_send_timeserver ();
    var_send_weather_appid ();
    var_send_weather_city ();
    var_send_weather_lon ();
    var_send_weather_lat ();
    var_send_update_host ();
    var_send_update_path ();
    var_send_date_ticker_format ();

    var_send_tm ();

    var_send_display_colors ();
    var_send_ambilight_colors ();
    var_send_ambilight_marker_colors ();
    var_send_display_animations ();
    var_send_color_animations ();
    var_send_ambilight_modes ();
    var_send_overlays ();
    var_send_night_times ();
    var_send_ambilight_night_times ();
    var_send_dimmed_display_colors ();
    var_send_dimmed_ambilight_colors ();
    var_send_dfplayer_is_up ();
    var_send_dfplayer_version ();
    var_send_dfplayer_volume ();
    var_send_dfplayer_silence_start ();
    var_send_dfplayer_silence_stop ();
    var_send_dfplayer_mode ();
    var_send_dfplayer_bell_flags ();
    var_send_dfplayer_speak_cycle ();
    var_send_alarm_times ();
}

