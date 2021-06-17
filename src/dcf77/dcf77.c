/*-------------------------------------------------------------------------------------------------------------------------------------------
 * dcf77.c - dcf77 routines
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <string.h>
#include "wclock24h-config.h"
#include "dcf77.h"
#include "eeprom.h"
#include "eeprom-data.h"
#include "log.h"
#include "board-led.h"
#include "io.h"

#if defined (STM32F4XX)                                     // STM32F4xx Nucleo Board: DATA=PC11 PON=PC12

#  define DCF77_DATA_PERIPH_CLOCK_CMD   RCC_AHB1PeriphClockCmd
#  define DCF77_DATA_PERIPH             RCC_AHB1Periph_GPIOC
#  define DCF77_DATA_PORT               GPIOC
#  define DCF77_DATA_PIN                GPIO_Pin_11

#  define DCF77_PON_PERIPH_CLOCK_CMD    RCC_AHB1PeriphClockCmd
#  define DCF77_PON_PERIPH              RCC_AHB1Periph_GPIOC
#  define DCF77_PON_PORT                GPIOC
#  define DCF77_PON_PIN                 GPIO_Pin_12

#elif defined (STM32F103)                                   // STM32F103 Mini Development Board: DATA=PB8 PON=PB9

#  define DCF77_DATA_PERIPH_CLOCK_CMD   RCC_APB2PeriphClockCmd
#  define DCF77_DATA_PERIPH             RCC_APB2Periph_GPIOB
#  define DCF77_DATA_PORT               GPIOB
#  define DCF77_DATA_PIN                GPIO_Pin_8

#  define DCF77_PON_PERIPH_CLOCK_CMD    RCC_APB2PeriphClockCmd
#  define DCF77_PON_PERIPH              RCC_APB2Periph_GPIOB
#  define DCF77_PON_PORT                GPIOB
#  define DCF77_PON_PIN                 GPIO_Pin_9

#else
#  error STM32 unknown
#endif

static uint_fast8_t                     time_is_valid = 0;

#define STATE_PON                       0xFF                    // wait 1 sec, then set PON to GND (only for very old Pollin DCF1 versions)
#define STATE_UNKNOWN                   0
#define STATE_WAIT                      1
#define STATE_0                         2
#define STATE_1                         3

static volatile uint_fast8_t            isdst;
static volatile uint_fast8_t            minute;
static volatile uint_fast8_t            hour;
static volatile uint_fast8_t            wday;
static volatile uint_fast8_t            mday;
static volatile uint_fast8_t            month;
static volatile uint_fast16_t           year;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * DCF77 get time
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
dcf77_time (struct tm * tm)
{
    uint_fast8_t    rtc = time_is_valid;

    if (time_is_valid)
    {
        tm->tm_year     = year + 100;                   // tm_year begins with 1900
        tm->tm_mon      = month - 1;                    // tm_month begins with 0
        tm->tm_mday     = mday;
        tm->tm_wday     = wday == 7 ? 0 : wday;
        tm->tm_isdst    = 0;
        tm->tm_hour     = hour;
        tm->tm_min      = minute;
        tm->tm_sec      = 15;

        time_is_valid   = 0;
    }
    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set pon (high)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dcf77_pon_set (void)
{
    GPIO_SET_BIT(DCF77_PON_PORT, DCF77_PON_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * reset pon (low)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dcf77_pon_reset (void)
{
    GPIO_RESET_BIT(DCF77_PON_PORT, DCF77_PON_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * DCF77 init
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dcf77_init (void)
{
    GPIO_InitTypeDef gpio;

    // Initialize DATA pin:
    GPIO_StructInit (&gpio);
    DCF77_DATA_PERIPH_CLOCK_CMD (DCF77_DATA_PERIPH, ENABLE);

    gpio.GPIO_Pin   = DCF77_DATA_PIN;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;

#if defined (STM32F10X)
    gpio.GPIO_Mode  = GPIO_Mode_IN_FLOATING;     // use pin as input, no pullup
#elif defined (STM32F4XX)
    gpio.GPIO_Mode  = GPIO_Mode_IN;              // use pin as input, no pullup
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;          // possible values: GPIO_PuPd__NOPULL  GPIO_PuPd__UP   GPIO_PuPd__DOWN
#endif

    GPIO_Init(DCF77_DATA_PORT, &gpio);

    // Initialize PON pin:
    GPIO_StructInit (&gpio);
    DCF77_PON_PERIPH_CLOCK_CMD (DCF77_PON_PERIPH, ENABLE);

    gpio.GPIO_Pin   = DCF77_PON_PIN;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;

#if defined (STM32F10X)
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;         // use pin as output, push-pull
#elif defined (STM32F4XX)
    gpio.GPIO_Mode  = GPIO_Mode_OUT;            // use pin as output
    gpio.GPIO_OType = GPIO_OType_PP;            // push-pull
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;         // no pullup/pulldown
#endif

    GPIO_Init(DCF77_PON_PORT, &gpio);

    dcf77_pon_set ();                           // Pollin DCF modules want a pulse on PON
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * DCF77 tick - statemachine, called every 1/100 of a second
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dcf77_tick (void)
{
    static uint_fast8_t cnt         = 0;
    static uint_fast8_t state       = STATE_PON;
    static uint_fast8_t last_pin    = 0;
    static uint_fast8_t bitno       = 0xff;
    static uint_fast8_t p1          = 0;
    static uint_fast8_t p2          = 0;
    static uint_fast8_t p3          = 0;
    static uint_fast8_t time_complete= 0;
    uint_fast16_t       pin;
    static uint_fast8_t last_h      = 0;
    static uint_fast8_t last_m      = 0;

    if (state == STATE_PON)                             // pseudo state: wait 1 sec before resetting PON after boot
    {
        static uint_fast8_t pon_cnt = 0;

        if (pon_cnt == 100)
        {
            dcf77_pon_reset ();
            state = STATE_UNKNOWN;                      // start state machine now!
        }
        else
        {
            pon_cnt++;
        }

        return;
    }

    if (GPIO_ReadInputDataBit(DCF77_DATA_PORT, DCF77_DATA_PIN) == Bit_RESET)
    {
        board_led_off ();
        pin = 0;
    }
    else
    {
        board_led_on();
        pin = 1;
    }

    switch (state)
    {
        case STATE_UNKNOWN:
        {
            state = STATE_WAIT;
            last_pin = pin;
            break;
        }
        case STATE_WAIT:
        {
            if (last_pin != pin)
            {
                state = (pin == 0) ? STATE_0 : STATE_1;
                last_pin = pin;
                bitno = 0xff;
                cnt = 0;
            }
            break;
        }
        case STATE_0:
        {
            if (pin == last_pin)
            {
                cnt++;
            }
            else
            {
                state = STATE_1;
                last_pin = pin;

                if (cnt > 120)
                {
                    bitno = 0;
                }
                else if (cnt < 75)
                {
                    state = STATE_UNKNOWN;
                }

                cnt = 0;
            }
            break;
        }
        default: // STATE_1:
        {
            if (pin == last_pin)
            {
                cnt++;
            }
            else
            {
                state = STATE_0;
                last_pin = pin;

                if (cnt <= 5 || cnt >= 25)
                {
                    if (cnt <= 5)
                    {
                        log_message ("DCF77 Error: High pulse <= 5");
                    }
                    else
                    {
                        log_message ("DCF77 Error: High pulse >= 25");
                    }

                    state = STATE_UNKNOWN;
                }
                else if (cnt > 15)
                {
                    switch (bitno)
                    {
                        case 17:
                            isdst   =  1;
                            break;

                        case 21:
                            minute +=  1;
                            p1++;
                            break;
                        case 22:
                            minute +=  2;
                            p1++;
                            break;
                        case 23:
                            minute +=  4;
                            p1++;
                            break;
                        case 24:
                            minute +=  8;
                            p1++;
                            break;
                        case 25:
                            minute += 10;
                            p1++;
                            break;
                        case 26:
                            minute += 20;
                            p1++;
                            break;
                        case 27:
                            minute += 40;
                            p1++;
                            break;

                        case 28:
                        {
                            if (! (p1 & 0x01))
                            {
                                log_message ("DCF77 Error: parity P1 incorrect");
                                state = STATE_UNKNOWN;
                            }
                            break;
                        }

                        case 29:
                            hour   +=  1;
                            p2++;
                            break;
                        case 30:
                            hour   +=  2;
                            p2++;
                            break;
                        case 31:
                            hour   +=  4;
                            p2++;
                            break;
                        case 32:
                            hour   +=  8;
                            p2++;
                            break;
                        case 33:
                            hour   += 10;
                            p2++;
                            break;
                        case 34:
                            hour   += 20;
                            p2++;
                            break;

                        case 35:
                        {
                            if (! (p2 & 0x01))
                            {
                                log_message ("DCF77 Error: parity P2 incorrect");
                                state = STATE_UNKNOWN;
                            }
                            break;
                        }

                        case 36:
                            mday   +=  1;
                            p3++;
                            break;
                        case 37:
                            mday   +=  2;
                            p3++;
                            break;
                        case 38:
                            mday   +=  4;
                            p3++;
                            break;
                        case 39:
                            mday   +=  8;
                            p3++;
                            break;
                        case 40:
                            mday   += 10;
                            p3++;
                            break;
                        case 41:
                            mday   += 20;
                            p3++;
                            break;

                        case 42:
                            wday   +=  1;
                            p3++;
                            break;
                        case 43:
                            wday   +=  2;
                            p3++;
                            break;
                        case 44:
                            wday   +=  4;
                            p3++;
                            break;

                        case 45:
                            month  +=  1;
                            p3++;
                            break;
                        case 46:
                            month  +=  2;
                            p3++;
                            break;
                        case 47:
                            month  +=  4;
                            p3++;
                            break;
                        case 48:
                            month  +=  8;
                            p3++;
                            break;
                        case 49:
                            month  += 10;
                            p3++;
                            break;

                        case 50:
                            year   +=  1;
                            p3++;
                            break;
                        case 51:
                            year   +=  2;
                            p3++;
                            break;
                        case 52:
                            year   +=  4;
                            p3++;
                            break;
                        case 53:
                            year   +=  8;
                            p3++;
                            break;
                        case 54:
                            year   += 10;
                            p3++;
                            break;
                        case 55:
                            year   += 20;
                            p3++;
                            break;
                        case 56:
                            year   += 40;
                            p3++;
                            break;
                        case 57:
                            year   += 80;
                            p3++;
                            break;

                        case 58:
                        {
                            if (! (p3 & 0x01))
                            {
                                log_message ("DCF77 Error: parity P3 incorrect");
                                state = STATE_UNKNOWN;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    switch (bitno)
                    {
                        case 28:
                        {
                            if (p1 & 0x01)
                            {
                                log_message ("DCF77 Error: parity P1 incorrect");
                                state = STATE_UNKNOWN;
                            }
                            break;
                        }

                        case 35:
                        {
                            if (p2 & 0x01)
                            {
                                log_message ("DCF77 Error: parity P2 incorrect");
                                state = STATE_UNKNOWN;
                            }
                            break;
                        }

                        case 58:
                        {
                            if (p3 & 0x01)
                            {
                                log_message ("DCF77 Error: parity P3 incorrect");
                                state = STATE_UNKNOWN;
                            }
                            break;
                        }
                    }

                }

                if (bitno == 15 && time_complete)                       // deliver DCF77 time at hh:mm:15 to avoid abrupt minute changes
                {
                    time_is_valid = 1;
                    time_complete = 0;
                }

                if (bitno == 16)
                {
                    time_is_valid   = 0;
                    time_complete   = 0;

                    p1      = 0;
                    p2      = 0;
                    p3      = 0;

                    isdst   = 0;
                    minute  = 0;
                    hour    = 0;
                    wday    = 0;
                    mday    = 0;
                    month   = 0;
                    year    = 0;
                }

                if (bitno != 0xff)
                {
                    bitno++;

                    if (bitno == 59)
                    {
                        if (last_h == hour && last_m + 1 == minute)
                        {
                            time_complete = 1;
                        }

                        last_h = hour;
                        last_m = minute;
                    }
                    else if (bitno >= 60)
                    {
                        state = STATE_UNKNOWN;
                    }
                }

                cnt = 0;
            }
        }
        break;
    }

    return;
}
