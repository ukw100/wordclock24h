/*-------------------------------------------------------------------------------------------------------------------------------------------
 * main.h - some global definitions/declarations
 *
 * Copyright (c) 2014-2026 Frank Meyer - frank(at)uclock.de
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

#define VERSION                     "3.1.3"

#define MAX_EEPROM_VERSION_LEN      4

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * hardware configuration word: 0 xxx FFF LLL WWW SSS
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define HW_STM32_MASK               (0x07 << 0)                         // 3 bits for microprocessor mask
#define HW_STM32_F103C8             (0x00 << 0)                         // 0: F103C8 BluePill Board
#define HW_STM32_F401RE             (0x01 << 0)                         // 1: F401RE Nucleo
#define HW_STM32_F411RE             (0x02 << 0)                         // 2: F411RE Nucleo
#define HW_STM32_F446RE             (0x03 << 0)                         // 3: F446RE Nucleo
#define HW_STM32_F407VE             (0x04 << 0)                         // 4: F407VE Black Board
#define HW_STM32_F401CC             (0x05 << 0)                         // 5: F401CC BlackPill Board
#define HW_STM32_F411CE             (0x06 << 0)                         // 6: F411CE BlackPill Board
#define HW_STM32_RESERVED_7         (0x07 << 0)                         // 7: Reserved

#define HW_CLOCK_MASK               (0x07 << 3)                         // 3 bits for clock type mask
#define HW_CLOCK_WC24H              (0x00 << 3)                         // 0: WC24
#define HW_CLOCK_WC12H              (0x01 << 3)                         // 1: WC12
#define HW_CLOCK_UCLOCK             (0x02 << 3)                         // 2: UCLOCK
#define HW_CLOCK_RESERVED_3         (0x03 << 3)                         // 3: Reserved
#define HW_CLOCK_RESERVED_4         (0x04 << 3)                         // 4: Reserved
#define HW_CLOCK_RESERVED_5         (0x05 << 3)                         // 5: Reserved
#define HW_CLOCK_RESERVED_6         (0x06 << 3)                         // 6: Reserved
#define HW_CLOCK_RESERVED_7         (0x07 << 3)                         // 7: Reserved

#define HW_DISPLAY_MASK             (0x07 << 6)                         // 3 bits for LED type mask
#define HW_DISPLAY_WS2812_GRB_LED   (0x00 << 6)                         // 0: WS2812 GRB
#define HW_DISPLAY_WS2812_RGB_LED   (0x01 << 6)                         // 1: WS2812 RGB
#define HW_DISPLAY_APA102_RGB_LED   (0x02 << 6)                         // 2: APA102 RGB
#define HW_DISPLAY_SK6812_RGB_LED   (0x03 << 6)                         // 3: SK6812 RGB
#define HW_DISPLAY_SK6812_RGBW_LED  (0x04 << 6)                         // 4: SK6812 RGBW
#define HW_DISPLAY_TFTLED_RGB_LED   (0x05 << 6)                         // 5: TFTLED RGB
#define HW_DISPLAY_RESERVED_6       (0x06 << 6)                         // 6: Reserved
#define HW_DISPLAY_RESERVED_7       (0x07 << 6)                         // 7: Reserved

#define HW_OSC_FREQUENCY_MASK       (0x07 << 9)                         // 3 bits for OSC frequency mask
#define HW_OSC_FREQUENCY_8MHZ       (0x00 << 9)                         // 0: standard oscillator frequency 8MHz
#define HW_OSC_FREQUENCY_25MHZ      (0x01 << 9)                         // 1: BlackPill with 25 MHz oscillator
#define HW_OSC_FREQUENCY_RESERVED_2 (0x02 << 9)                         // 2: Reserved
#define HW_OSC_FREQUENCY_RESERVED_3 (0x03 << 9)                         // 2: Reserved
#define HW_OSC_FREQUENCY_RESERVED_4 (0x04 << 9)                         // 2: Reserved
#define HW_OSC_FREQUENCY_RESERVED_5 (0x05 << 9)                         // 2: Reserved
#define HW_OSC_FREQUENCY_RESERVED_6 (0x06 << 9)                         // 2: Reserved
#define HW_OSC_FREQUENCY_RESERVED_7 (0x07 << 9)                         // 2: Reserved

#if defined (STM32F103C8)
#  define HW_STM32_CONFIGURATION    HW_STM32_F103C8
#elif defined (STM32F401RE)
#  define HW_STM32_CONFIGURATION    HW_STM32_F401RE
#elif defined (STM32F401CC)
#  define HW_STM32_CONFIGURATION    HW_STM32_F401CC
#elif defined (STM32F411CE)
#  define HW_STM32_CONFIGURATION    HW_STM32_F411CE
#elif defined (STM32F411RE)
#  define HW_STM32_CONFIGURATION    HW_STM32_F411RE
#elif defined (STM32F446RE)
#  define HW_STM32_CONFIGURATION    HW_STM32_F446RE
#elif defined (STM32F407VE)
#  define HW_STM32_CONFIGURATION    HW_STM32_F407VE
#else
#  error unknown STM32 hardware
#endif

#if WCLOCK24H == 1
#  define HW_CLOCK_CONFIGURATION    HW_CLOCK_WC24H
#elif WCLOCK24H == 0
#  define HW_CLOCK_CONFIGURATION    HW_CLOCK_WC12H
#else
#  error unknown HW clock configuration (WCLOCK24H)
#endif

#if DSP_USE_WS2812_GRB == 1
#  define HW_DISPLAY_CONFIGURATION  HW_DISPLAY_WS2812_GRB_LED
#elif DSP_USE_WS2812_RGB == 1
#  define HW_DISPLAY_CONFIGURATION  HW_DISPLAY_WS2812_RGB_LED
#elif DSP_USE_APA102 == 1
#  define HW_DISPLAY_CONFIGURATION  HW_DISPLAY_APA102_RGB_LED
#elif DSP_USE_SK6812_RGB == 1
#  define HW_DISPLAY_CONFIGURATION  HW_DISPLAY_SK6812_RGB_LED
#elif DSP_USE_SK6812_RGBW == 1
#  define HW_DISPLAY_CONFIGURATION  HW_DISPLAY_SK6812_RGBW_LED
#elif DSP_USE_TFTLED_RGB == 1
#  define HW_DISPLAY_CONFIGURATION  HW_DISPLAY_TFTLED_RGB_LED
#else
# error unknown DISPLAY type
#endif

#if HSE_VALUE == 25000000
#  define HW_OSC_CONFIGURATION      HW_OSC_FREQUENCY_25MHZ
#elif HSE_VALUE == 8000000
#  define HW_OSC_CONFIGURATION      HW_OSC_FREQUENCY_8MHZ
#else
# error unknown HW oscillator frequency
#endif

typedef struct
{
    char                            eep_version[MAX_EEPROM_VERSION_LEN + 1];
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
