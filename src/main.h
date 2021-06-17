/*-------------------------------------------------------------------------------------------------------------------------------------------
 * main.h - some global definitions/declarations
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef MAIN_H
#define MAIN_H

#include <time.h>
#include "eeprom-data.h"

#define VERSION                     "2.9.5"

#define MAX_EEPROM_VERSION_LEN      4

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * hardware configuration word: 0 xxx xxx LLL WWW SSS
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define HW_STM32_MASK               (0x07 << 0)                         // 3 bits for microprocessor mask
#define HW_STM32_F103               (0x00 << 0)                         // 0: F103
#define HW_STM32_F401               (0x01 << 0)                         // 1: F401
#define HW_STM32_F411               (0x02 << 0)                         // 2: F411
#define HW_STM32_F446               (0x03 << 0)                         // 3: F446

#define HW_WC_MASK                  (0x07 << 3)                         // 3 bits for WC mask
#define HW_WC_24H                   (0x00 << 3)                         // 0: WC24
#define HW_WC_12H                   (0x01 << 3)                         // 1: WC12

#define HW_LED_MASK                 (0x07 << 6)                         // 3 bits for LED type mask
#define HW_LED_WS2812_GRB_LED       (0x00 << 6)                         // 0: WS2812 GRB
#define HW_LED_WS2812_RGB_LED       (0x01 << 6)                         // 1: WS2812 RGB
#define HW_LED_APA102_RGB_LED       (0x02 << 6)                         // 2: APA102 RGB
#define HW_LED_SK6812_RGB_LED       (0x03 << 6)                         // 3: SK6812 RGB
#define HW_LED_SK6812_RGBW_LED      (0x04 << 6)                         // 4: SK6812 RGBW

typedef struct
{
    char                            eeprom_version[MAX_EEPROM_VERSION_LEN + 1];
    uint16_t                        hardware_configuration;
    struct tm                       tm;
    char                            update_host[EEPROM_MAX_HOSTNAME_LEN];
    char                            update_path[EEPROM_MAX_UPDATE_PATH_LEN];
    volatile uint_fast8_t           wday;                               // current weekday, 0=Sunday
    volatile uint_fast16_t          year;                               // current year;
    volatile uint_fast8_t           month;                              // current month;
    volatile uint_fast8_t           mday;                               // current day of month 1..31;
    volatile uint_fast8_t           hour;                               // current hour
    volatile uint_fast8_t           minute;                             // current minute
    volatile uint_fast8_t           second;                             // current second
} MAIN_GLOBALS;

extern MAIN_GLOBALS                 gmain;
extern volatile uint32_t            uptime;

extern uint_fast8_t                 schedule_esp8266_messages (void);
extern void                         set_overlay_idx (uint_fast8_t);
extern void                         show_icon (const char *, uint_fast8_t);
extern char *                       get_date (void);
extern char *                       get_time (void);
extern void                         main_set_ambilight_clock_wait_cycles (void);

#endif
