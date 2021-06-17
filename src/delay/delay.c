/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay.c - delay functions
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdint.h>
#include "delay.h"

static uint_fast8_t         resolution  = DELAY_DEFAULT_RESOLUTION;             // resolution in usec, see delay.h for default
static uint32_t             msec_factor = 1000 / DELAY_DEFAULT_RESOLUTION;      // factor for msec delays

volatile uint32_t           delay_counter;                                      // counts down in units of resolution, see above

void SysTick_Handler(void);                                                     // keep compiler happy

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * SysTick_Handler() - decrement delay_counter
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
SysTick_Handler(void)
{
    if (delay_counter > 0)
    {
        delay_counter--;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay_usec() - delay n microseconds (usec)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
delay_usec (uint32_t usec)
{
    if (resolution == DELAY_RESOLUTION_1_US)
    {
        delay_counter = usec;
    }
    else
    {
        delay_counter = usec / resolution;
    }

    while (delay_counter != 0)
    {
        ;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay_msec() - delay n milliseconds (msec)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
delay_msec (uint32_t msec)
{
    delay_counter = msec * msec_factor;

    while (delay_counter != 0)
    {
        ;
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay_msec() - delay n seconds (sec)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
delay_sec (uint32_t sec)
{
    while (sec--)
    {
        delay_msec (1000);
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay_init() - init delay functions
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
delay_init (uint_fast8_t res)
{
    uint32_t    divider;

    if (res == 0)
    {
        res = DELAY_RESOLUTION_100_US;                          // default is resolution of 100us
    }

    divider         = 1000000 / res;
    msec_factor     = 1000 / res;

    SysTick_Config (SystemCoreClock / divider);
    resolution = res;
}
