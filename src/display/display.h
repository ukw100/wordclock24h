/*-------------------------------------------------------------------------------------------------------------------------------------------
 * display.h - interface declaration of LED display routines
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "display-config.h"
#include "tables.h"

#define MAX_BRIGHTNESS                          15

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * display flags:
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DISPLAY_FLAGS_NONE                      0x00                    // no display flag
#define DISPLAY_FLAGS_PERMANENT_IT_IS           0x01                    // show "ES IST" permanently
#define DISPLAY_FLAGS_SYNC_AMBILIGHT            0x02                    // synchronize display and ambilight
#define DISPLAY_FLAGS_SYNC_CLOCK_MARKERS        0x04                    // synchronize display and clock markers
#define DISPLAY_FLAGS_FADE_CLOCK_SECONDS        0x08                    // fade clock seconds

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * clock display flags:
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DISPLAY_CLOCK_FLAG_NONE                 0x00                    // nothing to do
#define DISPLAY_CLOCK_FLAG_UPDATE_MINUTES       0x01                    // update minute LEDs
#define DISPLAY_CLOCK_FLAG_UPDATE_NO_ANIMATION  0x02                    // update without animation, e.g. colors
#define DISPLAY_CLOCK_FLAG_UPDATE_ALL           0x04                    // update complete display
#define DISPLAY_CLOCK_FLAG_POWER_ON             0x08                    // power on LED stripe
#define DISPLAY_CLOCK_FLAG_POWER_OFF            0x10                    // power off LED stripe

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * display colors:
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
typedef struct                                                          // display colors
{
    uint8_t red;                                                        // red, 0...63 (MAX_COLOR_STEPS)
    uint8_t green;                                                      // green, 0...63 (MAX_COLOR_STEPS)
    uint8_t blue;                                                       // blue, 0...63 (MAX_COLOR_STEPS)
#if DSP_USE_SK6812_RGBW == 1
    uint8_t white;                                                      // white, 0...63 (MAX_COLOR_STEPS)
#endif
} DSP_COLORS;

#if DSP_USE_SK6812_RGBW
#define WHITE_NULL          0
#else
#define WHITE_NULL
#endif

#define MAX_COLOR_STEPS     64

#define DSP_BLACK_COLOR     {  0,                       0,                      0,                      WHITE_NULL }        // 0 black
#define DSP_RED_COLOR       { MAX_COLOR_STEPS - 1,      0,                      0,                      WHITE_NULL }        // 1 red
#define DSP_DARK_RED_COLOR  { MAX_COLOR_STEPS / 2,      0,                      0,                      WHITE_NULL }        // 1 red
#define DSP_GREEN_COLOR     {  0,                       MAX_COLOR_STEPS - 1,    0,                      WHITE_NULL }        // 2 green
#define DSP_YELLOW_COLOR    { MAX_COLOR_STEPS - 1,      MAX_COLOR_STEPS - 1,    0,                      WHITE_NULL }        // 3 yellow
#define DSP_BLUE_COLOR      {  0,                       0,                      MAX_COLOR_STEPS - 1,    WHITE_NULL }        // 4 blue
#define DSP_MAGENTA_COLOR   { MAX_COLOR_STEPS - 1,      0,                      MAX_COLOR_STEPS - 1,    WHITE_NULL }        // 5 magenta
#define DSP_CYAN_COLOR      {  0,                       MAX_COLOR_STEPS - 1,    MAX_COLOR_STEPS - 1,    WHITE_NULL }        // 6 cyan
#define DSP_WHITE_COLOR     { MAX_COLOR_STEPS - 1,      MAX_COLOR_STEPS - 1,    MAX_COLOR_STEPS - 1,    WHITE_NULL }        // 7 white
#define DSP_BROWN_COLOR     { MAX_COLOR_STEPS / 2,      MAX_COLOR_STEPS / 2,    0,                      WHITE_NULL }        // 8 brown
#define DSP_GREY_COLOR      { MAX_COLOR_STEPS / 2,      MAX_COLOR_STEPS / 2,    MAX_COLOR_STEPS / 2,    WHITE_NULL }        // 9 grey

#if DSP_USE_APA102 == 1
#include "apa102.h"
#define LED_RGB             APA102_RGB
#define led_init            apa102_init
#define led_refresh         apa102_refresh
#define led_set_led         apa102_set_led
#define led_set_all_leds    apa102_set_all_leds
#elif DSP_USE_WS2812_GRB == 1 || DSP_USE_WS2812_RGB == 1
#include "ws2812.h"
#define LED_RGB             WS2812_RGB
#define led_init            ws2812_init
#define led_refresh         ws2812_refresh
#define led_set_led         ws2812_set_led
//#define led_set_all_leds    ws2812_set_all_leds
#else
#include "sk6812.h"
#define LED_RGB             SK6812_RGBW
#define led_init            sk6812_init
#define led_refresh         sk6812_refresh
#define led_set_led         sk6812_set_led
#define led_set_all_leds    sk6812_set_all_leds
#endif

#if DSP_USE_SK6812_RGBW == 1
#define CALC_LED_RGB(t,s)               \
do                                      \
{                                       \
    (t).red     = pwmtable8[(s).red];   \
    (t).green   = pwmtable8[(s).green]; \
    (t).blue    = pwmtable8[(s).blue];  \
    (t).white   = pwmtable8[(s).white]; \
} while (0)

#define COPY_LED_RGB(t,s)               \
do                                      \
{                                       \
    (t).red     = (s).red;              \
    (t).green   = (s).green;            \
    (t).blue    = (s).blue;             \
    (t).white   = (s).white;            \
} while (0)

#define RESET_LED_RGB(t)                \
do                                      \
{                                       \
    (t).red     = 0;                    \
    (t).green   = 0;                    \
    (t).blue    = 0;                    \
    (t).white   = 0;                    \
} while (0)

#define SET_DSP_RGB(t,r,g,b,w)          \
do                                      \
{                                       \
    (t).red     = (r);                  \
    (t).green   = (g);                  \
    (t).blue    = (b);                  \
    (t).white   = (w);                  \
} while (0)

#define SET_DSP_RGB_SAVE(t,r,g,b,w)                                     \
do                                                                      \
{                                                                       \
    (t).red     = (r) < MAX_COLOR_STEPS ? (r) : MAX_COLOR_STEPS - 1;    \
    (t).green   = (g) < MAX_COLOR_STEPS ? (g) : MAX_COLOR_STEPS - 1;    \
    (t).blue    = (b) < MAX_COLOR_STEPS ? (b) : MAX_COLOR_STEPS - 1;    \
    (t).white   = (w) < MAX_COLOR_STEPS ? (w) : MAX_COLOR_STEPS - 1;    \
} while (0)

#define COPY_DSP_RGB(t,s)               \
do                                      \
{                                       \
    (t).red     = (s).red;              \
    (t).green   = (s).green;            \
    (t).blue    = (s).blue;             \
    (t).white   = (s).white;            \
} while (0)

#define COPY_DSP_RGB_TO_PTR(tp,s)       \
do                                      \
{                                       \
    (tp)->red     = (s).red;            \
    (tp)->green   = (s).green;          \
    (tp)->blue    = (s).blue;           \
    (tp)->white   = (s).white;          \
} while (0)

#define COPY_DSP_RGB_SAFE(t,sp)         \
do                                      \
{                                       \
    (t).red   = ((sp)->red   < MAX_COLOR_STEPS) ? (sp)->red   : MAX_COLOR_STEPS - 1;      \
    (t).green = ((sp)->green < MAX_COLOR_STEPS) ? (sp)->green : MAX_COLOR_STEPS - 1;      \
    (t).blue  = ((sp)->blue  < MAX_COLOR_STEPS) ? (sp)->blue  : MAX_COLOR_STEPS - 1;      \
    (t).white = ((sp)->white < MAX_COLOR_STEPS) ? (sp)->white : MAX_COLOR_STEPS - 1;      \
} while (0)

#else

#define CALC_LED_RGB(t,s)               \
do                                      \
{                                       \
    (t).red     = pwmtable8[(s).red];   \
    (t).green   = pwmtable8[(s).green]; \
    (t).blue    = pwmtable8[(s).blue];  \
} while (0)

#define SET_LED_RGB(t,r,g,b,w)          \
do                                      \
{                                       \
    (t).red     = (r);                  \
    (t).green   = (g);                  \
    (t).blue    = (b);                  \
} while (0)

#define COPY_LED_RGB(t,s)               \
do                                      \
{                                       \
    (t).red     = (s).red;              \
    (t).green   = (s).green;            \
    (t).blue    = (s).blue;             \
} while (0)

#define RESET_LED_RGB(t)                \
do                                      \
{                                       \
    (t).red     = 0;                    \
    (t).green   = 0;                    \
    (t).blue    = 0;                    \
} while (0)

#define SET_DSP_RGB(t,r,g,b,w)          \
do                                      \
{                                       \
    (t).red     = (r);                  \
    (t).green   = (g);                  \
    (t).blue    = (b);                  \
} while (0)

#define SET_DSP_RGB_SAVE(t,r,g,b,w)                                     \
do                                                                      \
{                                                                       \
    (t).red     = (r) < MAX_COLOR_STEPS ? (r) : MAX_COLOR_STEPS - 1;    \
    (t).green   = (g) < MAX_COLOR_STEPS ? (g) : MAX_COLOR_STEPS - 1;    \
    (t).blue    = (b) < MAX_COLOR_STEPS ? (b) : MAX_COLOR_STEPS - 1;    \
} while (0)

#define COPY_DSP_RGB(t,s)               \
do                                      \
{                                       \
    (t).red     = (s).red;              \
    (t).green   = (s).green;            \
    (t).blue    = (s).blue;             \
} while (0)

#define COPY_DSP_RGB_TO_PTR(t,s)        \
do                                      \
{                                       \
    (t)->red     = (s).red;             \
    (t)->green   = (s).green;           \
    (t)->blue    = (s).blue;            \
} while (0)

#define COPY_DSP_RGB_SAFE(t,sp)         \
do                                      \
{                                       \
    (t).red   = ((sp)->red   < MAX_COLOR_STEPS) ? (sp)->red   : MAX_COLOR_STEPS - 1;      \
    (t).green = ((sp)->green < MAX_COLOR_STEPS) ? (sp)->green : MAX_COLOR_STEPS - 1;      \
    (t).blue  = ((sp)->blue  < MAX_COLOR_STEPS) ? (sp)->blue  : MAX_COLOR_STEPS - 1;      \
} while (0)

#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ambilight modes:
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define AMBILIGHT_MODE_NORMAL                   0                       // normal ambilight mode
#define AMBILIGHT_MODE_CLOCK                    1                       // show clock seconds (20/30/60 LEDs)
#define AMBILIGHT_MODE_CLOCK2                   2                       // show clock seconds (20/30/60 LEDs) - increasing/decreasing ring
#define AMBILIGHT_MODE_RAINBOW                  3                       // rainbow colors
#define AMBILIGHT_MODE_DAYLIGHT                 4                       // daylight colors
#define AMBILIGHT_MODES                         5                       // number of ambilight modes

#define EEPROM_MAX_AMBILIGHT_MODES              16                      // maximum number of ambilight modes stored in EEPROM
#define EEPROM_AMBILIGHT_DECELERATION_MASK      0x0F                    // lower 4 bits (0...3) = deceleration

#define AMBILIGHT_MODE_MAX_DECELERATION         15                      // deceleration = 15 is lowest deceleration, min is 0

#define AMBILIGHT_FLAG_NONE                     0x00                    // no animation flag
#define AMBILIGHT_FLAG_CONFIGURABLE             0x01                    // ambilight mode is configurable
#define AMBILIGHT_FLAG_SECONDS_MARKER           0x02                    // only for CLOCK ambilight mode: five-seconds-marker

#define AMBILIGHT_CLOCK_TICK_COUNT_PER_LED      20                      // fade ambilight leds over 32 steps in clock mode

typedef struct
{
    const char *    name;
    uint_fast8_t    deceleration;                                       // 0 = fastest, 15 = slowest
    uint_fast8_t    default_deceleration;                               // 0 = fastest, 15 = slowest
    uint_fast8_t    flags;
} AMBILIGHT_MODE;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * animation modes:
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define ANIMATION_MODE_NONE                      0                      // no animation
#define ANIMATION_MODE_FADE                      1                      // fade
#define ANIMATION_MODE_ROLL                      2                      // roll
#define ANIMATION_MODE_EXPLODE                   3                      // explode
#define ANIMATION_MODE_RANDOM                    4                      // random animation
#define ANIMATION_MODE_SNAKE                     5                      // snake
#define ANIMATION_MODE_TELETYPE                  6                      // teletype
#define ANIMATION_MODE_CUBE                      7                      // cube
#define ANIMATION_MODE_GREEN_MATRIX              8                      // green matrix
#define ANIMATION_MODE_DROP                      9                      // drop letters
#define ANIMATION_MODE_SQUEEZE                  10                      // squeeze letters
#define ANIMATION_MODE_FLICKER                  11                      // flicker LEDs
#define ANIMATION_MODE_MATRIX                   12                      // matrix in current display colors
#define ANIMATION_MODES                         13                      // number of animation modes

#define EEPROM_MAX_ANIMATION_MODES              64                      // maximum number of animation modes stored in EEPROM
#define EEPROM_ANIMATION_DECELERATION_MASK      0x0F                    // lower 4 bits (0...3) = deceleration
#define EEPROM_ANIMATION_FAVOURITE_MASK         0x10                    // bit 4 = favourite flag

#define ANIMATION_MIN_DECELERATION               1
#define ANIMATION_MAX_DECELERATION              15

#define ANIMATION_FLAG_NONE                     0x00                    // no animation flag
#define ANIMATION_FLAG_CONFIGURABLE             0x01                    // animation selectable per random animation
#define ANIMATION_FLAG_FAVOURITE                0x02                    // animation is favourite

typedef struct
{
    const char *    name;
    void (* func) (void);
    uint_fast8_t    deceleration;                                       // 1 = fastest, 15 = slowest
    uint_fast8_t    default_deceleration;                               // 1 = fastest, 15 = slowest
    uint_fast8_t    flags;
} ANIMATION;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * color animation modes:
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define COLOR_ANIMATION_MODE_NONE                   0                   // no color animation
#define COLOR_ANIMATION_MODE_RAINBOW                1                   // rainbow
#define COLOR_ANIMATION_MODE_DAYLIGHT               2                   // daylight
#define COLOR_ANIMATION_MODES                       3                   // number of color animations

#define EEPROM_MAX_COLOR_ANIMATION_MODES            16                  // maximum number of color animation modes stored in EEPROM
#define EEPROM_COLOR_ANIMATION_DECELERATION_MASK    0x0F                // lower 4 bits (0...3) = deceleration

#define COLOR_ANIMATION_MAX_DECELERATION            15                  // deceleration = 15 is lowest deceleration, min is 0

#define COLOR_ANIMATION_FLAG_NONE                   0x00                // no color animation flag
#define COLOR_ANIMATION_FLAG_CONFIGURABLE           0x01                // animation is configurable

typedef struct
{
    const char *    name;
    uint_fast8_t    deceleration;                                       // 1 = fastest, 15 = slowest
    uint_fast8_t    default_deceleration;                               // 1 = fastest, 15 = slowest
    uint_fast8_t    flags;                                              // yet not used
} COLOR_ANIMATION;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * icon
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
    uint_fast8_t    rows;
    uint_fast8_t    cols;
    char            colors[WC_ROWS * WC_COLUMNS];
    char            animation_on[WC_ROWS * WC_COLUMNS];
    char            animation_off[WC_ROWS * WC_COLUMNS];
    uint_fast8_t    duration;
} DISPLAY_ICON;


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * globals
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DATE_TICKER_FORMAT_LEN                  6

typedef struct
{
    uint_fast8_t    display_mode;
    uint_fast8_t    ambilight_mode;
    uint_fast8_t    color_animation_mode;
    uint_fast8_t    display_brightness;
    uint_fast8_t    ambilight_brightness;
    uint_fast8_t    saved_ambilight_brightness;
    uint_fast8_t    automatic_brightness;
    uint_fast8_t    display_flags;
    uint_fast8_t    animation_mode;
    uint_fast8_t    animation_start_flag;
    uint_fast8_t    animation_stop_flag;
    uint_fast8_t    display_power_is_on;
    uint_fast8_t    ambilight_power_is_on;
    uint_fast8_t    do_display_icon;
    uint_fast8_t    n_icons;
    uint_fast8_t    ambilight_led_offset;
    uint_fast8_t    ambilight_leds;
    DSP_COLORS      display_colors;
    DSP_COLORS      ambilight_colors;
    DSP_COLORS      saved_ambilight_colors;
    DSP_COLORS      ambilight_marker_colors;
    ANIMATION       animations[ANIMATION_MODES];
    COLOR_ANIMATION color_animations[COLOR_ANIMATION_MODES];
    AMBILIGHT_MODE  ambilight_modes[AMBILIGHT_MODES];
    uint_fast8_t    dimmed_display_colors[MAX_BRIGHTNESS + 1];                  // 15 + 1 = 16
    uint_fast8_t    dimmed_ambilight_colors[MAX_BRIGHTNESS + 1];                // 15 + 1 = 16
    uint_fast8_t    ticker_deceleration;
    uint8_t         date_ticker_format[DATE_TICKER_FORMAT_LEN];                 // default: d-m-Y
} DISPLAY_GLOBALS;

extern DISPLAY_GLOBALS  display;
extern const uint16_t   pwmtable8[MAX_COLOR_STEPS];

extern void             display_set_status_led (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern void             display_set_status_or_minute_leds (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern void             display_set_display_led (uint_fast16_t, LED_RGB *, uint_fast8_t);
extern void             display_set_dimmed_display_led (uint_fast16_t, DSP_COLORS *);
extern void             display_refresh_display_leds (void);
extern void             display_reset_led_states (void);
extern void             display_temperature (uint_fast8_t);
extern void             display_fireworks (void);
extern void             display_clock (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern void             display_seconds (uint_fast8_t);
extern void             display_animation (void);

extern void             display_dim_display_dsp_colors (DSP_COLORS *, const DSP_COLORS *, uint_fast8_t, uint_fast8_t);
extern void             display_dim_ambilight_dsp_colors (DSP_COLORS *, const DSP_COLORS *, uint_fast8_t, uint_fast8_t);

extern uint_fast8_t     display_set_dimmed_display_color (uint_fast8_t, uint_fast8_t);
extern uint_fast8_t     display_set_dimmed_ambilight_color (uint_fast8_t, uint_fast8_t);

extern uint_fast8_t     display_set_ticker_deceleration (uint_fast8_t);
extern void             display_set_date_ticker_format (char *);
extern uint_fast8_t     display_set_display_flags (uint_fast8_t);

extern void             display_set_ambilight_power (uint_fast8_t);
extern uint_fast8_t     display_set_ambilight_mode (uint_fast8_t, uint_fast8_t);
extern uint_fast8_t     display_set_number_of_ambilight_leds (uint_fast8_t);
extern uint_fast8_t     display_set_ambilight_led_offset (uint_fast8_t);

extern uint_fast8_t     display_set_display_mode (uint_fast8_t, uint_fast8_t);
extern uint_fast8_t     display_increment_display_mode (uint_fast8_t);
extern uint_fast8_t     display_decrement_display_mode (uint_fast8_t);
extern void             display_set_automatic_brightness (uint_fast8_t, uint_fast8_t);

extern uint_fast8_t     display_set_animation_mode (uint_fast8_t, uint_fast8_t);
extern uint_fast8_t     display_increment_animation_mode (uint_fast8_t);
extern uint_fast8_t     display_decrement_animation_mode (uint_fast8_t);

extern uint_fast8_t     display_set_color_animation_mode (uint_fast8_t, uint_fast8_t);
extern uint_fast8_t     display_increment_color_animation_mode (uint_fast8_t);
extern uint_fast8_t     display_decrement_color_animation_mode (uint_fast8_t);

extern void             display_increment_display_color_red (uint_fast8_t);
extern void             display_decrement_display_color_red (uint_fast8_t);
extern void             display_increment_display_color_green (uint_fast8_t);
extern void             display_decrement_display_color_green (uint_fast8_t);
extern void             display_increment_display_color_blue (uint_fast8_t);
extern void             display_decrement_display_color_blue (uint_fast8_t);
#if DSP_USE_SK6812_RGBW == 1
extern void             display_increment_display_color_white (uint_fast8_t);
extern void             display_decrement_display_color_white (uint_fast8_t);
#endif
extern void             display_set_display_colors (DSP_COLORS *);

extern void             display_increment_ambilight_color_red (uint_fast8_t);
extern void             display_decrement_ambilight_color_red (uint_fast8_t);
extern void             display_increment_ambilight_color_green (uint_fast8_t);
extern void             display_decrement_ambilight_color_green (uint_fast8_t);
extern void             display_increment_ambilight_color_blue (uint_fast8_t);
extern void             display_decrement_ambilight_color_blue (uint_fast8_t);
#if DSP_USE_SK6812_RGBW == 1
extern void             display_increment_ambilight_color_white (uint_fast8_t);
extern void             display_decrement_ambilight_color_white (uint_fast8_t);
#endif
extern void             display_set_ambilight_colors (DSP_COLORS *);
extern void             display_set_ambilight_marker_colors (DSP_COLORS *);

extern void             display_set_display_brightness  (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern void             display_decrement_display_brightness (uint_fast8_t);
extern void             display_increment_display_brightness (uint_fast8_t);

extern void             display_set_ambilight_brightness  (uint_fast8_t, uint_fast8_t, uint_fast8_t);
extern void             display_decrement_ambilight_brightness (uint_fast8_t);
extern void             display_increment_ambilight_brightness (uint_fast8_t);

extern void             display_set_animation_flags (uint_fast8_t, uint_fast8_t);
extern void             display_set_animation_deceleration (uint_fast8_t, uint_fast8_t);
extern void             display_set_color_animation_deceleration (uint_fast8_t, uint_fast8_t);
extern void             display_set_ambilight_mode_deceleration (uint_fast8_t, uint_fast8_t);
extern void             display_set_ambilight_mode_flags (uint_fast8_t, uint_fast8_t);

extern void             display_test (void);
extern void             display_set_ticker (const unsigned char *, uint_fast8_t);
extern uint_fast8_t     display_ticker_active (void);
extern void             display_get_icon (const char *, uint_fast8_t);
extern void             display_get_weather_icon (const char *, uint_fast8_t);
extern uint_fast8_t     display_read_icon (void);
extern uint_fast8_t     display_read_config_from_eeprom (uint32_t);
extern uint_fast8_t     display_write_config_to_eeprom (void);
extern void             display_save_display_mode (void);
extern void             display_init (void);

#endif
