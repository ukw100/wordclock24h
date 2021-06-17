/*-------------------------------------------------------------------------------------------------------------------------------------------
 * eeprom.c - EEPROM routines
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "eeprom.h"
#include "i2c.h"

#define SHOW_SIZES              0

#if SHOW_SIZES == 1
#include "eeprom-data.h"
#include "log.h"
#endif

#define EEPROM_FIRST_ADDR       0xA0                            // I2C address << 1
#define EEPROM_WAITSTATES       15                              // we have to wait 15ms after each write cycle

uint_fast8_t                    eeprom_is_up = 0;
volatile uint_fast8_t           eeprom_ms_tick;                 // should be set every 1 ms by IRQ, see main.c

static  uint_fast8_t            eeprom_addr;


/*--------------------------------------------------------------------------------------------------------------------------------------
 * eeprom_waitstates() - wait 15 ms after each write cycle
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
eeprom_waitstates (void)
{
    uint_fast16_t  cnt = 0;

    while (1)
    {
        if (eeprom_ms_tick)
        {
            eeprom_ms_tick = 0;

            cnt++;

            if (cnt > EEPROM_WAITSTATES)
            {
                break;
            }
        }
    }
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize EEPROM functions
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
eeprom_init (uint32_t clockspeed)
{
    uint_fast8_t    i;
    uint8_t         value;

    i2c_init (clockspeed);

    for (i = 0; i < 8; i++)                                     // test read 0xA0, 0xA2, 0xA4, 0xA6 .. 0xAE
    {
        eeprom_addr = EEPROM_FIRST_ADDR | (i << 1);

        if (i2c_read (eeprom_addr, 0x00, 1, &value, 1) == I2C_OK)
        {
            eeprom_is_up = 1;
            break;
        }
    }

#if SHOW_SIZES == 1
    log_printf ("EEPROM_DATA_OFFSET_VERSION                  = %4d %4d\r\n", EEPROM_DATA_OFFSET_VERSION, EEPROM_DATA_SIZE_VERSION);
    log_printf ("EEPROM_DATA_OFFSET_IRMP_DATA                = %4d %4d\r\n", EEPROM_DATA_OFFSET_IRMP_DATA, EEPROM_DATA_SIZE_IRMP_DATA);
    log_printf ("EEPROM_DATA_OFFSET_DSP_COLORS               = %4d %4d\r\n", EEPROM_DATA_OFFSET_DSP_COLORS, EEPROM_DATA_SIZE_DSP_COLORS);
    log_printf ("EEPROM_DATA_OFFSET_DISPLAY_MODE             = %4d %4d\r\n", EEPROM_DATA_OFFSET_DISPLAY_MODE, EEPROM_DATA_SIZE_DISPLAY_MODE);
    log_printf ("EEPROM_DATA_OFFSET_ANIMATION_MODE           = %4d %4d\r\n", EEPROM_DATA_OFFSET_ANIMATION_MODE, EEPROM_DATA_SIZE_ANIMATION_MODE);
    log_printf ("EEPROM_DATA_OFFSET_COLOR_ANIMATION_MODE     = %4d %4d\r\n", EEPROM_DATA_OFFSET_COLOR_ANIMATION_MODE, EEPROM_DATA_SIZE_COLOR_ANIMATION_MODE);
    log_printf ("EEPROM_DATA_OFFSET_TIMESERVER               = %4d %4d\r\n", EEPROM_DATA_OFFSET_TIMESERVER, EEPROM_DATA_SIZE_TIMESERVER);
    log_printf ("EEPROM_DATA_OFFSET_TIMEZONE                 = %4d %4d\r\n", EEPROM_DATA_OFFSET_TIMEZONE, EEPROM_DATA_SIZE_TIMEZONE);
    log_printf ("EEPROM_DATA_OFFSET_DISPLAY_FLAGS            = %4d %4d\r\n", EEPROM_DATA_OFFSET_DISPLAY_FLAGS, EEPROM_DATA_SIZE_DISPLAY_FLAGS);
    log_printf ("EEPROM_DATA_OFFSET_BRIGHTNESS               = %4d %4d\r\n", EEPROM_DATA_OFFSET_BRIGHTNESS, EEPROM_DATA_SIZE_BRIGHTNESS);
    log_printf ("EEPROM_DATA_OFFSET_AUTO_BRIGHTNESS          = %4d %4d\r\n", EEPROM_DATA_OFFSET_AUTO_BRIGHTNESS, EEPROM_DATA_SIZE_AUTO_BRIGHTNESS);
    log_printf ("EEPROM_DATA_OFFSET_NIGHT_TIME               = %4d %4d\r\n", EEPROM_DATA_OFFSET_NIGHT_TIME, EEPROM_DATA_SIZE_NIGHT_TIME);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_COLORS              = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_COLORS, EEPROM_DATA_SIZE_AMBI_COLORS);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_BRIGHTNESS          = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_BRIGHTNESS, EEPROM_DATA_SIZE_AMBI_BRIGHTNESS);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_MODE                = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_MODE, EEPROM_DATA_SIZE_AMBI_MODE);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_LEDS                = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_LEDS, EEPROM_DATA_SIZE_AMBI_LEDS);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_OFFSET_SEC0         = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_OFFSET_SEC0, EEPROM_DATA_SIZE_AMBI_OFFSET_SEC0);
    log_printf ("EEPROM_DATA_OFFSET_RTC_TEMP_CORR            = %4d %4d\r\n", EEPROM_DATA_OFFSET_RTC_TEMP_CORR, EEPROM_DATA_SIZE_RTC_TEMP_CORR);
    log_printf ("EEPROM_DATA_OFFSET_DS18XX_TEMP_CORR         = %4d %4d\r\n", EEPROM_DATA_OFFSET_DS18XX_TEMP_CORR, EEPROM_DATA_SIZE_DS18XX_TEMP_CORR);
    log_printf ("EEPROM_DATA_OFFSET_NOT_USED_01              = %4d %4d\r\n", EEPROM_DATA_OFFSET_NOT_USED_01, EEPROM_DATA_SIZE_NOT_USED_01);
    log_printf ("EEPROM_DATA_OFFSET_LDR_MIN_VALUE            = %4d %4d\r\n", EEPROM_DATA_OFFSET_LDR_MIN_VALUE, EEPROM_DATA_SIZE_LDR_MIN_VALUE);
    log_printf ("EEPROM_DATA_OFFSET_LDR_MAX_VALUE            = %4d %4d\r\n", EEPROM_DATA_OFFSET_LDR_MAX_VALUE, EEPROM_DATA_SIZE_LDR_MAX_VALUE);
    log_printf ("EEPROM_DATA_OFFSET_ANIMATION_VALUES         = %4d %4d\r\n", EEPROM_DATA_OFFSET_ANIMATION_VALUES, EEPROM_DATA_SIZE_ANIMATION_VALUES);
    log_printf ("EEPROM_DATA_OFFSET_COLOR_ANIMATION_VALUES   = %4d %4d\r\n", EEPROM_DATA_OFFSET_COLOR_ANIMATION_VALUES, EEPROM_DATA_SIZE_COLOR_ANIMATION_VALUES);
    log_printf ("EEPROM_DATA_OFFSET_AMBILIGHT_MODE_VALUES    = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBILIGHT_MODE_VALUES, EEPROM_DATA_SIZE_AMBILIGHT_MODE_VALUES);
    log_printf ("EEPROM_DATA_OFFSET_DSP_W_COLOR              = %4d %4d\r\n", EEPROM_DATA_OFFSET_DSP_W_COLOR, EEPROM_DATA_SIZE_DSP_W_COLOR);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_W_COLOR             = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_W_COLOR, EEPROM_DATA_SIZE_AMBI_W_COLOR);
    log_printf ("EEPROM_DATA_OFFSET_WEATHER_APPID            = %4d %4d\r\n", EEPROM_DATA_OFFSET_WEATHER_APPID, EEPROM_DATA_SIZE_WEATHER_APPID);
    log_printf ("EEPROM_DATA_OFFSET_WEATHER_CITY             = %4d %4d\r\n", EEPROM_DATA_OFFSET_WEATHER_CITY, EEPROM_DATA_SIZE_WEATHER_CITY);
    log_printf ("EEPROM_DATA_OFFSET_WEATHER_LON              = %4d %4d\r\n", EEPROM_DATA_OFFSET_WEATHER_LON, EEPROM_DATA_SIZE_WEATHER_LON);
    log_printf ("EEPROM_DATA_OFFSET_WEATHER_LAT              = %4d %4d\r\n", EEPROM_DATA_OFFSET_WEATHER_LAT, EEPROM_DATA_SIZE_WEATHER_LAT);
    log_printf ("EEPROM_DATA_OFFSET_OVERLAY_INTERVALS        = %4d %4d\r\n", EEPROM_DATA_OFFSET_OVERLAY_INTERVALS, EEPROM_DATA_SIZE_OVERLAY_INTERVALS);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_NIGHT_TIME          = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_NIGHT_TIME, EEPROM_DATA_SIZE_AMBI_NIGHT_TIME);
    log_printf ("EEPROM_DATA_OFFSET_DIMMED_DISPLAY_COLORS    = %4d %4d\r\n", EEPROM_DATA_OFFSET_DIMMED_DISPLAY_COLORS, EEPROM_DATA_SIZE_DIMMED_DISPLAY_COLORS);
    log_printf ("EEPROM_DATA_OFFSET_UPDATE_HOSTNAME          = %4d %4d\r\n", EEPROM_DATA_OFFSET_UPDATE_HOSTNAME, EEPROM_DATA_SIZE_UPDATE_HOSTNAME);
    log_printf ("EEPROM_DATA_OFFSET_UPDATE_PATH              = %4d %4d\r\n", EEPROM_DATA_OFFSET_UPDATE_PATH, EEPROM_DATA_SIZE_UPDATE_PATH);
    log_printf ("EEPROM_DATA_OFFSET_TICKER_DECELERATION      = %4d %4d\r\n", EEPROM_DATA_OFFSET_TICKER_DECELERATION, EEPROM_DATA_SIZE_TICKER_DECELERATION);
    log_printf ("EEPROM_DATA_OFFSET_DFPLAYER_VOLUME          = %4d %4d\r\n", EEPROM_DATA_OFFSET_DFPLAYER_VOLUME, EEPROM_DATA_SIZE_DFPLAYER_VOLUME);
    log_printf ("EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_START   = %4d %4d\r\n", EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_START, EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START);
    log_printf ("EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_STOP    = %4d %4d\r\n", EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_STOP, EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP);
    log_printf ("EEPROM_DATA_OFFSET_DFPLAYER_MODE            = %4d %4d\r\n", EEPROM_DATA_OFFSET_DFPLAYER_MODE, EEPROM_DATA_SIZE_DFPLAYER_MODE);
    log_printf ("EEPROM_DATA_OFFSET_DFPLAYER_BELL_FLAGS      = %4d %4d\r\n", EEPROM_DATA_OFFSET_DFPLAYER_BELL_FLAGS, EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS);
    log_printf ("EEPROM_DATA_OFFSET_DFPLAYER_SPEAK_CYCLE     = %4d %4d\r\n", EEPROM_DATA_OFFSET_DFPLAYER_SPEAK_CYCLE, EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE);
    log_printf ("EEPROM_DATA_OFFSET_ALARM_TIME               = %4d %4d\r\n", EEPROM_DATA_OFFSET_ALARM_TIME, EEPROM_DATA_SIZE_ALARM_TIME);
    log_printf ("EEPROM_DATA_OFFSET_N_OVERLAYS               = %4d %4d\r\n", EEPROM_DATA_OFFSET_N_OVERLAYS, EEPROM_DATA_SIZE_N_OVERLAYS);
    log_printf ("EEPROM_DATA_OFFSET_OVERLAY                  = %4d %4d\r\n", EEPROM_DATA_OFFSET_OVERLAY, EEPROM_DATA_SIZE_OVERLAY);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_MARKER_COLORS       = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_MARKER_COLORS, EEPROM_DATA_SIZE_AMBI_MARKER_COLORS);
    log_printf ("EEPROM_DATA_OFFSET_AMBI_MARKER_W_COLOR      = %4d %4d\r\n", EEPROM_DATA_OFFSET_AMBI_MARKER_W_COLOR, EEPROM_DATA_SIZE_AMBI_MARKER_W_COLOR);
    log_printf ("EEPROM_DATA_OFFSET_DATE_TICKER_FORMAT       = %4d %4d\r\n", EEPROM_DATA_OFFSET_DATE_TICKER_FORMAT, EEPROM_DATA_SIZE_DATE_TICKER_FORMAT);
    log_printf ("EEPROM_DATA_OFFSET_DIMMED_AMBILIGHT_COLORS  = %4d %4d\r\n", EEPROM_DATA_OFFSET_DIMMED_AMBILIGHT_COLORS, EEPROM_DATA_SIZE_DIMMED_AMBILIGHT_COLORS);
#endif

    return eeprom_is_up;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * get address of EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
eeprom_get_address (void)
{
    return eeprom_addr;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
eeprom_read (uint_fast16_t start_addr, uint8_t * buffer, uint_fast16_t cnt)
{
    uint_fast8_t    rtc;

    if (eeprom_is_up)
    {
        if (i2c_read (eeprom_addr, start_addr, 1, buffer, cnt) == I2C_OK)
        {
            rtc = 1;
        }
        else
        {
            rtc = 0;
        }
    }
    else
    {
        rtc = 0;
    }
    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
eeprom_write (uint_fast16_t start_addr, uint8_t * buffer, uint_fast16_t cnt)
{
    uint_fast8_t rtc;

    if (eeprom_is_up)
    {
        // we must write every single byte, because we have to wait 15ms every cycle
        while (cnt--)
        {
            if (i2c_write (eeprom_addr, start_addr, 1, buffer, 1) == I2C_OK)
            {
                rtc = 1;
            }
            else
            {
                rtc = 0;
                break;
            }
            eeprom_waitstates ();
            start_addr++;
            buffer++;
        }
    }
    else
    {
        rtc = 0;
    }
    return rtc;
}
