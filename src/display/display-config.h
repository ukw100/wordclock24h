/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * display-config.h - configuration of display driver
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * various possible LED types, don't change!
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#define WS2812_GRB_LED              1                                   // use WS2812 GRB  LEDs, order G R B (default)
#define WS2812_RGB_LED              2                                   // use WS2812 RGB  LEDs, order R G B
#define APA102_RGB_LED              3                                   // use APA102 RGB  LEDs
#define SK6812_RGB_LED              4                                   // use SK6812 RGB  LEDs
#define SK6812_RGBW_LED             5                                   // use SK6812 RGBW LEDs

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * change here:
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DSP_USE_LED
#define DSP_USE_LED                 WS2812_GRB_LED                      // set exactly one of the values above here, default is WS2812_GRB_LED
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * configuration settings, don't change below!
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if DSP_USE_LED == WS2812_GRB_LED
#define DSP_USE_WS2812_GRB          1
#define DSP_USE_WS2812_RGB          0
#define DSP_USE_APA102              0
#define DSP_USE_SK6812_RGB          0
#define DSP_USE_SK6812_RGBW         0
#elif DSP_USE_LED == WS2812_RGB_LED
#define DSP_USE_WS2812_GRB          0
#define DSP_USE_WS2812_RGB          1
#define DSP_USE_APA102              0
#define DSP_USE_SK6812_RGB          0
#define DSP_USE_SK6812_RGBW         0
#elif DSP_USE_LED == APA102_RGB_LED
#define DSP_USE_WS2812_GRB          0
#define DSP_USE_WS2812_RGB          0
#define DSP_USE_APA102              1
#define DSP_USE_SK6812_RGB          0
#define DSP_USE_SK6812_RGBW         0
#elif DSP_USE_LED == SK6812_RGB_LED
#define DSP_USE_WS2812_GRB          0
#define DSP_USE_WS2812_RGB          0
#define DSP_USE_APA102              0
#define DSP_USE_SK6812_RGB          1
#define DSP_USE_SK6812_RGBW         0
#elif DSP_USE_LED == SK6812_RGBW_LED
#define DSP_USE_WS2812_GRB          0
#define DSP_USE_WS2812_RGB          0
#define DSP_USE_APA102              0
#define DSP_USE_SK6812_RGB          0
#define DSP_USE_SK6812_RGBW         1
#else
#error invalid LED type
#endif

#define DSP_AMBILIGHT_EMULATION     0                                       // don't change: this is only for ambilight test!

#if DSP_AMBILIGHT_EMULATION == 1
#  if WCLOCK24H == 1
#    define DSP_STATUS_LEDS         1                                       // emulation: 1 status LED
#    define DSP_MINUTE_LEDS         0                                       // emulation: 0 minute LEDs
#    define DSP_DISPLAY_LEDS        0                                       // emulation: 16 display LEDs
#  else // WC12H
#    define DSP_STATUS_LEDS         0                                       // emulation: no status LED
#    define DSP_MINUTE_LEDS         4                                       // emulation: 4 minute LEDs
#    define DSP_DISPLAY_LEDS        0                                       // emulation: display LEDs
#  endif
#else
#  if WCLOCK24H == 1
#    define DSP_STATUS_LEDS         1                                       // 1 status LED
#    define DSP_MINUTE_LEDS         0                                       // 0 minute LEDs
#    define DSP_DISPLAY_LEDS        288                                     // 288 display LEDs
#  else // WC12H
#    define DSP_STATUS_LEDS         0                                       // no status LED
#    define DSP_MINUTE_LEDS         4                                       // 4 minute LEDs
#    define DSP_DISPLAY_LEDS        110                                     // 110 display LEDs
#  endif
#endif

#define DSP_AMBILIGHT_LEDS          120                                     // max. 120 ambilight LEDs

#define DSP_STATUS_LED_OFFSET       0                                               // offset in LED chain
#define DSP_MINUTE_LED_OFFSET       (DSP_STATUS_LED_OFFSET + DSP_STATUS_LEDS)       // offset of minute LEDs
#define DSP_DISPLAY_LED_OFFSET      (DSP_MINUTE_LED_OFFSET + DSP_MINUTE_LEDS)       // offset of display LEDs
#define DSP_AMBILIGHT_LED_OFFSET    (DSP_DISPLAY_LED_OFFSET + DSP_DISPLAY_LEDS)     // offset of ambilight LEDs
#define DSP_MAX_LEDS                (DSP_STATUS_LEDS + DSP_MINUTE_LEDS + DSP_DISPLAY_LEDS + DSP_AMBILIGHT_LEDS) // maximum number of LEDs

#endif
