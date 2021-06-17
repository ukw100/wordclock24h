/*-------------------------------------------------------------------------------------------------------------------------------------------
 * eeprom-data.h - data structure of EEPROM
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef EEPROM_DATA_H
#define EEPROM_DATA_H

#include "night.h"                                              // need MAX_NIGHT_TIMES
#include "alarm.h"                                              // need MAX_ALARM_TIMES
#include "display.h"
#include "weather.h"
#include "remote-ir.h"
#include "overlay.h"

// Note: All EEPROM versions must have MSB=0x00 and LSB=0x00, only the 2nd and 3rd byte should differ from 0x00
#define EEPROM_VERSION_0_0          0x00000000                  // version 0.0 (reset to defaults)
#define EEPROM_VERSION_1_5          0x00010500                  // version 1.5
#define EEPROM_VERSION_1_6          0x00010600                  // version 1.6
#define EEPROM_VERSION_1_7          0x00010700                  // version 1.7
#define EEPROM_VERSION_1_8          0x00010800                  // version 1.8
#define EEPROM_VERSION_1_9          0x00010900                  // version 1.9
#define EEPROM_VERSION_2_0          0x00020000                  // version 2.0
#define EEPROM_VERSION_2_1          0x00020100                  // version 2.1
#define EEPROM_VERSION_2_2          0x00020200                  // version 2.2
#define EEPROM_VERSION_2_3          0x00020300                  // version 2.3
#define EEPROM_VERSION_2_4          0x00020400                  // version 2.4
#define EEPROM_VERSION_2_5          0x00020500                  // version 2.5
#define EEPROM_VERSION_2_6          0x00020600                  // version 2.6
#define EEPROM_VERSION_2_7          0x00020700                  // version 2.7
#define EEPROM_VERSION_2_8          0x00020800                  // version 2.8
#define EEPROM_VERSION_2_9          0x00020900                  // version 2.9
#define EEPROM_VERSION              EEPROM_VERSION_2_9          // current version

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Some packed data to minimize used EEPROM space
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define PACKED_IRMP_DATA_SIZE       5                           // 1 byte for protocol, 2 bytes for address, 2 bytes for command

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Sizes:
 *
 *      EEPROM Version                     4 Bytes   (  1 *  4)           0     4
 *      IRMP data                        160 Bytes   ( 32 *  5)           4   160
 *      Display RGB colors                 3 Bytes   (  3 *  1)         164     3
 *      Display mode                       1 Byte    (  1 *  1)         167     1
 *      Animation mode                     1 Byte    (  1 *  1)         168     1
 *      Color animation mode               1 Byte    (  1 *  1)         169     1
 *      Time server                       16 Bytes   (  1 * 16)         170    16
 *      Time zone                          2 Bytes   (  1 *  2)         186     2
 *      Display flags                      1 Byte    (  1 *  1)         188     1
 *      Display brightness                 1 Byte    (  1 *  1)         189     1
 *      Automatic brightness               1 Byte    (  1 *  1)         190     1
 *      Night time                        24 Byte    (  8 *  3)         191    24
 *      Ambilight RGB colors               3 Bytes   (  3 *  1)         215     3
 *      Ambilight brightness               1 Byte    (  1 *  1)         218     1
 *      Ambilight mode                     1 Byte    (  1 *  1)         219     1
 *      Ambilight leds                     1 Byte    (  1 *  1)         220     1
 *      Ambilight offset sec=0             1 Byte    (  1 *  1)         221     1
 *      RTC temperature correction         1 Byte    (  1 *  1)         222     1
 *      DS18xx temperature correction      1 Byte    (  1 *  1)         223     1
 *      Not used (old: temp interval)      1 Byte    (  1 *  1)         224     1
 *      LDR minimum value                  2 Bytes   (  1 *  2)         225     2
 *      LDR maximum value                  2 Bytes   (  1 *  2)         227     2
 *      Animation values                  64 Bytes   ( 64 *  1)         229    64
 *      Color animation values            16 Bytes   ( 16 *  1)         293    16
 *      Ambilight mode value              16 Bytes   ( 16 *  1)         309    16
 *      Display W color                    1 Byte    (  1 *  1)         325     1
 *      Ambilight W color                  1 Byte    (  1 *  1)         326     1
 *      Weather appid                     32 Bytes   ( 32 *  1)         327    32
 *      Weather city                      32 Bytes   ( 32 *  1)         359    32
 *      Weather longitude                  8 Bytes   (  8 *  1)         391     8
 *      Weather latitude                   8 Bytes   (  8 *  1)         399     8
 *      Not used (old: overlay intervals) 64 Bytes   ( 64 *  1)         407    64
 *      Ambilight night times             32 Bytes   (  8 *  4)         471    24
 *      Dimmed display colors             16 Bytes   ( 16 *  1)         495    16
 *      Update host                       64 Bytes   ( 64 *  1)         511    64
 *      Update path                       64 Bytes   ( 64 *  1)         575    64
 *      Ticker deceleration                1 Byte    (  1 *  1)         639     1
 *      DFPlayer volume                    1 Byte    (  1 *  1)         640     1
 *      DFPlayer start silence             2 Bytes   (  2 *  1)         641     2
 *      DFPlayer stop silence              2 Bytes   (  2 *  1)         643     2
 *      DFPlayer mode                      1 Byte    (  1 *  1)         645     1
 *      DFPlayer bell flags                1 Byte    (  1 *  1)         646     1
 *      DFPlayer speak cycle               1 Byte    (  1 *  1)         647     1
 *      Alarm times                       32 Byte    (  8 *  4)         648    24
 *      Number of overlays                 1 Byte    (  1 *  1)         672     1
 *      Overlays                        1280 Bytes   ( 32 * 40)         673  1280
 *      Ambilight marker colors            3 Bytes   (  3 *  1)         1953    3
 *      Ambilight marker color white       1 Bytes   (  1 *  1)         1956    1
 *      Date ticker format                 5 Bytes   (  5 *  1)         1957    6
 *      Dimmed ambilight colors           16 Bytes   ( 16 *  1)         1963   16
 *      =========================================================================
 *      Sum                             1979 Bytes
 *
 *  EEPROM size of AT24C32: 32KBit = 4096 Bytes
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#define EEPROM_MAX_IR_CODES                         32                      // max IR codes, should be greater or equal than N_REMOTE_IR_CMDS
#if N_REMOTE_IR_CMDS > EEPROM_MAX_IR_CODES
#error value for EEPROM_MAX_IR_CODES is too low
#endif

#define EEPROM_MAX_IPADDR_LEN                       16                      // max length of IP address xxx.xxx.xxx.xxx + '\0'
#define EEPROM_MAX_HOSTNAME_LEN                     64                      // max length of hostname + '\0'
#define EEPROM_MAX_UPDATE_PATH_LEN                  64                      // max length of update path + '\0'
#define EEPROM_DATE_TICKER_FORMAT_LEN               DATE_TICKER_FORMAT_LEN  // max length of date ticker path + '\0'
#define EEPROM_MAX_TIMEZONE_LEN                      2                      // max length of timezone: one byte for +/-, one byte for hour offset
#define EEPROM_MAX_NIGHT_TIME_LEN                   (MAX_NIGHT_TIMES * 3)   // length of night times: 1 byte flags, 2 bytes minute
#define EEPROM_MAX_ALARM_TIME_LEN                   (MAX_ALARM_TIMES * 3)   // length of alarm times: 1 byte flags, 2 bytes minute
#define EEPROM_OVERLAY_ENTRY_SIZE                   (OVERLAY_ENTRY_SIZE)

#define EEPROM_MAX_OVERLAY_INTERVALS                64                      // max overlay intervals

#define EEPROM_DATA_SIZE_VERSION                    sizeof (uint32_t)
#define EEPROM_DATA_SIZE_IRMP_DATA                  (EEPROM_MAX_IR_CODES * PACKED_IRMP_DATA_SIZE)
#define EEPROM_DATA_SIZE_DSP_COLORS                 (3 * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_DISPLAY_MODE               sizeof (uint8_t)
#define EEPROM_DATA_SIZE_ANIMATION_MODE             sizeof (uint8_t)
#define EEPROM_DATA_SIZE_COLOR_ANIMATION_MODE       sizeof (uint8_t)
#define EEPROM_DATA_SIZE_TIMESERVER                 (EEPROM_MAX_IPADDR_LEN)
#define EEPROM_DATA_SIZE_TIMEZONE                   (EEPROM_MAX_TIMEZONE_LEN)
#define EEPROM_DATA_SIZE_DISPLAY_FLAGS              sizeof (uint8_t)
#define EEPROM_DATA_SIZE_BRIGHTNESS                 sizeof (uint8_t)
#define EEPROM_DATA_SIZE_AUTO_BRIGHTNESS            sizeof (uint8_t)
#define EEPROM_DATA_SIZE_NIGHT_TIME                 (EEPROM_MAX_NIGHT_TIME_LEN)
#define EEPROM_DATA_SIZE_AMBI_COLORS                (3 * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_AMBI_BRIGHTNESS            sizeof (uint8_t)
#define EEPROM_DATA_SIZE_AMBI_MODE                  sizeof (uint8_t)
#define EEPROM_DATA_SIZE_AMBI_LEDS                  sizeof (uint8_t)        // number of ambilight LEDs
#define EEPROM_DATA_SIZE_AMBI_OFFSET_SEC0           sizeof (uint8_t)        // ambilight offset of second 0
#define EEPROM_DATA_SIZE_RTC_TEMP_CORR              sizeof (uint8_t)        // RTC temperature correction
#define EEPROM_DATA_SIZE_DS18XX_TEMP_CORR           sizeof (uint8_t)        // DS18xx temperature correction
#define EEPROM_DATA_SIZE_NOT_USED_01                sizeof (uint8_t)        // not used anymore, obsolete: show temperature every n minutes
#define EEPROM_DATA_SIZE_LDR_MIN_VALUE              sizeof (uint16_t)       // minimum LDR value
#define EEPROM_DATA_SIZE_LDR_MAX_VALUE              sizeof (uint16_t)       // maximum LDR value
#define EEPROM_DATA_SIZE_ANIMATION_VALUES           (EEPROM_MAX_ANIMATION_MODES * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_COLOR_ANIMATION_VALUES     (EEPROM_MAX_COLOR_ANIMATION_MODES * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_AMBILIGHT_MODE_VALUES      (EEPROM_MAX_AMBILIGHT_MODES * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_DSP_W_COLOR                sizeof (uint8_t)
#define EEPROM_DATA_SIZE_AMBI_W_COLOR               sizeof (uint8_t)
#define EEPROM_DATA_SIZE_WEATHER_APPID              (MAX_WEATHER_APPID_LEN * sizeof (char))
#define EEPROM_DATA_SIZE_WEATHER_CITY               (MAX_WEATHER_CITY_LEN * sizeof (char))
#define EEPROM_DATA_SIZE_WEATHER_LON                (MAX_WEATHER_LON_LEN * sizeof (char))
#define EEPROM_DATA_SIZE_WEATHER_LAT                (MAX_WEATHER_LAT_LEN * sizeof (char))
#define EEPROM_DATA_SIZE_OVERLAY_INTERVALS          (EEPROM_MAX_OVERLAY_INTERVALS * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_AMBI_NIGHT_TIME            (EEPROM_MAX_NIGHT_TIME_LEN)
#define EEPROM_DATA_SIZE_DIMMED_DISPLAY_COLORS      ((MAX_BRIGHTNESS + 1) * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_UPDATE_HOSTNAME            (EEPROM_MAX_HOSTNAME_LEN)
#define EEPROM_DATA_SIZE_UPDATE_PATH                (EEPROM_MAX_UPDATE_PATH_LEN)
#define EEPROM_DATA_SIZE_TICKER_DECELERATION        sizeof (uint8_t)
#define EEPROM_DATA_SIZE_DFPLAYER_VOLUME            sizeof (uint8_t)
#define EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START     sizeof (uint16_t)
#define EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP      sizeof (uint16_t)
#define EEPROM_DATA_SIZE_DFPLAYER_MODE              sizeof (uint8_t)
#define EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS        sizeof (uint8_t)
#define EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE       sizeof (uint8_t)
#define EEPROM_DATA_SIZE_ALARM_TIME                 (EEPROM_MAX_ALARM_TIME_LEN)
#define EEPROM_DATA_SIZE_N_OVERLAYS                 sizeof (uint8_t)
#define EEPROM_DATA_SIZE_OVERLAY                    (OVERLAY_ENTRIES_SIZE)
#define EEPROM_DATA_SIZE_AMBI_MARKER_COLORS         (3 * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_AMBI_MARKER_W_COLOR        (1 * sizeof (uint8_t))
#define EEPROM_DATA_SIZE_DATE_TICKER_FORMAT         (EEPROM_DATE_TICKER_FORMAT_LEN)
#define EEPROM_DATA_SIZE_DIMMED_AMBILIGHT_COLORS    ((MAX_BRIGHTNESS + 1) * sizeof (uint8_t))

#define EEPROM_DATA_OFFSET_VERSION                  0
#define EEPROM_DATA_OFFSET_IRMP_DATA                (EEPROM_DATA_OFFSET_VERSION                 + EEPROM_DATA_SIZE_VERSION)
#define EEPROM_DATA_OFFSET_DSP_COLORS               (EEPROM_DATA_OFFSET_IRMP_DATA               + EEPROM_DATA_SIZE_IRMP_DATA)
#define EEPROM_DATA_OFFSET_DISPLAY_MODE             (EEPROM_DATA_OFFSET_DSP_COLORS              + EEPROM_DATA_SIZE_DSP_COLORS)
#define EEPROM_DATA_OFFSET_ANIMATION_MODE           (EEPROM_DATA_OFFSET_DISPLAY_MODE            + EEPROM_DATA_SIZE_DISPLAY_MODE)
#define EEPROM_DATA_OFFSET_COLOR_ANIMATION_MODE     (EEPROM_DATA_OFFSET_ANIMATION_MODE          + EEPROM_DATA_SIZE_ANIMATION_MODE)
#define EEPROM_DATA_OFFSET_TIMESERVER               (EEPROM_DATA_OFFSET_COLOR_ANIMATION_MODE    + EEPROM_DATA_SIZE_COLOR_ANIMATION_MODE)
#define EEPROM_DATA_OFFSET_TIMEZONE                 (EEPROM_DATA_OFFSET_TIMESERVER              + EEPROM_DATA_SIZE_TIMESERVER)
#define EEPROM_DATA_OFFSET_DISPLAY_FLAGS            (EEPROM_DATA_OFFSET_TIMEZONE                + EEPROM_DATA_SIZE_TIMEZONE)
#define EEPROM_DATA_OFFSET_BRIGHTNESS               (EEPROM_DATA_OFFSET_DISPLAY_FLAGS           + EEPROM_DATA_SIZE_DISPLAY_FLAGS)
#define EEPROM_DATA_OFFSET_AUTO_BRIGHTNESS          (EEPROM_DATA_OFFSET_BRIGHTNESS              + EEPROM_DATA_SIZE_BRIGHTNESS)
#define EEPROM_DATA_OFFSET_NIGHT_TIME               (EEPROM_DATA_OFFSET_AUTO_BRIGHTNESS         + EEPROM_DATA_SIZE_AUTO_BRIGHTNESS)
#define EEPROM_DATA_OFFSET_AMBI_COLORS              (EEPROM_DATA_OFFSET_NIGHT_TIME              + EEPROM_DATA_SIZE_NIGHT_TIME)
#define EEPROM_DATA_OFFSET_AMBI_BRIGHTNESS          (EEPROM_DATA_OFFSET_AMBI_COLORS             + EEPROM_DATA_SIZE_AMBI_COLORS)
#define EEPROM_DATA_OFFSET_AMBI_MODE                (EEPROM_DATA_OFFSET_AMBI_BRIGHTNESS         + EEPROM_DATA_SIZE_AMBI_BRIGHTNESS)
#define EEPROM_DATA_OFFSET_AMBI_LEDS                (EEPROM_DATA_OFFSET_AMBI_MODE               + EEPROM_DATA_SIZE_AMBI_MODE)
#define EEPROM_DATA_OFFSET_AMBI_OFFSET_SEC0         (EEPROM_DATA_OFFSET_AMBI_LEDS               + EEPROM_DATA_SIZE_AMBI_LEDS)
#define EEPROM_DATA_OFFSET_RTC_TEMP_CORR            (EEPROM_DATA_OFFSET_AMBI_OFFSET_SEC0        + EEPROM_DATA_SIZE_AMBI_OFFSET_SEC0)
#define EEPROM_DATA_OFFSET_DS18XX_TEMP_CORR         (EEPROM_DATA_OFFSET_RTC_TEMP_CORR           + EEPROM_DATA_SIZE_RTC_TEMP_CORR)
#define EEPROM_DATA_OFFSET_NOT_USED_01              (EEPROM_DATA_OFFSET_DS18XX_TEMP_CORR        + EEPROM_DATA_SIZE_DS18XX_TEMP_CORR)
#define EEPROM_DATA_OFFSET_LDR_MIN_VALUE            (EEPROM_DATA_OFFSET_NOT_USED_01             + EEPROM_DATA_SIZE_NOT_USED_01)
#define EEPROM_DATA_OFFSET_LDR_MAX_VALUE            (EEPROM_DATA_OFFSET_LDR_MIN_VALUE           + EEPROM_DATA_SIZE_LDR_MIN_VALUE)
#define EEPROM_DATA_OFFSET_ANIMATION_VALUES         (EEPROM_DATA_OFFSET_LDR_MAX_VALUE           + EEPROM_DATA_SIZE_LDR_MAX_VALUE)
#define EEPROM_DATA_OFFSET_COLOR_ANIMATION_VALUES   (EEPROM_DATA_OFFSET_ANIMATION_VALUES        + EEPROM_DATA_SIZE_ANIMATION_VALUES)
#define EEPROM_DATA_OFFSET_AMBILIGHT_MODE_VALUES    (EEPROM_DATA_OFFSET_COLOR_ANIMATION_VALUES  + EEPROM_DATA_SIZE_COLOR_ANIMATION_VALUES)
#define EEPROM_DATA_OFFSET_DSP_W_COLOR              (EEPROM_DATA_OFFSET_AMBILIGHT_MODE_VALUES   + EEPROM_DATA_SIZE_AMBILIGHT_MODE_VALUES)
#define EEPROM_DATA_OFFSET_AMBI_W_COLOR             (EEPROM_DATA_OFFSET_DSP_W_COLOR             + EEPROM_DATA_SIZE_DSP_W_COLOR)
#define EEPROM_DATA_OFFSET_WEATHER_APPID            (EEPROM_DATA_OFFSET_AMBI_W_COLOR            + EEPROM_DATA_SIZE_AMBI_W_COLOR)
#define EEPROM_DATA_OFFSET_WEATHER_CITY             (EEPROM_DATA_OFFSET_WEATHER_APPID           + EEPROM_DATA_SIZE_WEATHER_APPID)
#define EEPROM_DATA_OFFSET_WEATHER_LON              (EEPROM_DATA_OFFSET_WEATHER_CITY            + EEPROM_DATA_SIZE_WEATHER_CITY)
#define EEPROM_DATA_OFFSET_WEATHER_LAT              (EEPROM_DATA_OFFSET_WEATHER_LON             + EEPROM_DATA_SIZE_WEATHER_LON)
#define EEPROM_DATA_OFFSET_OVERLAY_INTERVALS        (EEPROM_DATA_OFFSET_WEATHER_LAT             + EEPROM_DATA_SIZE_WEATHER_LAT)
#define EEPROM_DATA_OFFSET_AMBI_NIGHT_TIME          (EEPROM_DATA_OFFSET_OVERLAY_INTERVALS       + EEPROM_DATA_SIZE_OVERLAY_INTERVALS)
#define EEPROM_DATA_OFFSET_DIMMED_DISPLAY_COLORS    (EEPROM_DATA_OFFSET_AMBI_NIGHT_TIME         + EEPROM_DATA_SIZE_AMBI_NIGHT_TIME)
#define EEPROM_DATA_OFFSET_UPDATE_HOSTNAME          (EEPROM_DATA_OFFSET_DIMMED_DISPLAY_COLORS   + EEPROM_DATA_SIZE_DIMMED_DISPLAY_COLORS)
#define EEPROM_DATA_OFFSET_UPDATE_PATH              (EEPROM_DATA_OFFSET_UPDATE_HOSTNAME         + EEPROM_DATA_SIZE_UPDATE_HOSTNAME)
#define EEPROM_DATA_OFFSET_TICKER_DECELERATION      (EEPROM_DATA_OFFSET_UPDATE_PATH             + EEPROM_DATA_SIZE_UPDATE_PATH)
#define EEPROM_DATA_OFFSET_DFPLAYER_VOLUME          (EEPROM_DATA_OFFSET_TICKER_DECELERATION     + EEPROM_DATA_SIZE_TICKER_DECELERATION)
#define EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_START   (EEPROM_DATA_OFFSET_DFPLAYER_VOLUME         + EEPROM_DATA_SIZE_DFPLAYER_VOLUME)
#define EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_STOP    (EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_START  + EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START)
#define EEPROM_DATA_OFFSET_DFPLAYER_MODE            (EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_STOP   + EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP)
#define EEPROM_DATA_OFFSET_DFPLAYER_BELL_FLAGS      (EEPROM_DATA_OFFSET_DFPLAYER_MODE           + EEPROM_DATA_SIZE_DFPLAYER_MODE)
#define EEPROM_DATA_OFFSET_DFPLAYER_SPEAK_CYCLE     (EEPROM_DATA_OFFSET_DFPLAYER_BELL_FLAGS     + EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS)
#define EEPROM_DATA_OFFSET_ALARM_TIME               (EEPROM_DATA_OFFSET_DFPLAYER_SPEAK_CYCLE    + EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE)
#define EEPROM_DATA_OFFSET_N_OVERLAYS               (EEPROM_DATA_OFFSET_ALARM_TIME              + EEPROM_DATA_SIZE_ALARM_TIME)
#define EEPROM_DATA_OFFSET_OVERLAY                  (EEPROM_DATA_OFFSET_N_OVERLAYS              + EEPROM_DATA_SIZE_N_OVERLAYS)
#define EEPROM_DATA_OFFSET_AMBI_MARKER_COLORS       (EEPROM_DATA_OFFSET_OVERLAY                 + EEPROM_DATA_SIZE_OVERLAY)
#define EEPROM_DATA_OFFSET_AMBI_MARKER_W_COLOR      (EEPROM_DATA_OFFSET_AMBI_MARKER_COLORS      + EEPROM_DATA_SIZE_AMBI_MARKER_COLORS)
#define EEPROM_DATA_OFFSET_DATE_TICKER_FORMAT       (EEPROM_DATA_OFFSET_AMBI_MARKER_W_COLOR     + EEPROM_DATA_SIZE_AMBI_MARKER_W_COLOR)
#define EEPROM_DATA_OFFSET_DIMMED_AMBILIGHT_COLORS  (EEPROM_DATA_OFFSET_DATE_TICKER_FORMAT      + EEPROM_DATA_SIZE_DATE_TICKER_FORMAT)

#define EEPROM_DATA_END                             (EEPROM_DATA_OFFSET_DIMMED_AMBILIGHT_COLORS + EEPROM_DATA_SIZE_DIMMED_AMBILIGHT_COLORS)

#endif
