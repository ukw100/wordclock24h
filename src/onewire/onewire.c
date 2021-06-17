/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire.c - onewire lib
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "onewire.h"
#include "onewire-config.h"
#include "delay.h"
#include "io.h"

#define RESOLUTION                  DELAY_RESOLUTION_5_US                       // we use a resolution of 5 usec

#define DATA_LOW                    GPIO_RESET_BIT(ONE_WIRE_PORT, ONE_WIRE_PIN)
#define DATA_HIGH                   GPIO_SET_BIT(ONE_WIRE_PORT, ONE_WIRE_PIN)

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_get_time_of_low_pulse - get time of low pulse in usec
 *
 * Return value: time in units of usec
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint32_t
onewire_get_time_of_low_pulse (uint32_t timeout)
{
    uint32_t rtc    = 0;
    uint32_t start  = 0;
    uint32_t stop   = 0;

    delay_counter = timeout / RESOLUTION;

    do
    {
        if (GPIO_ReadInputDataBit (ONE_WIRE_PORT, ONE_WIRE_PIN) == Bit_RESET)
        {
            if (start == 0)
            {
                start = delay_counter;
            }
        }
        else if (start > 0 && stop == 0)
        {
            stop = delay_counter;
        }
    } while (delay_counter > 0);

    if (start && stop)
    {
        rtc = RESOLUTION * (start - stop);
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_reset () - reset all slaves, check if one slave is connected
 *
 * Timing:
 *   Master: Holds Bus min. 480us low, waits 15-60us
 *   Slave shows presence when it pulls bus low in the next 60us for 60-240us
 *   Master waits 480us (pulse above) + 480us presence detection following
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
onewire_reset (void)
{
    uint_fast8_t  rtc       = 0;
    uint32_t      low_time  = 0;
    uint_fast8_t  is_high;

    delay_usec (100);
    DATA_LOW;
    delay_usec (560);
    DATA_HIGH;
    delay_usec (20);
    low_time = onewire_get_time_of_low_pulse (300);

    delay_usec (50);
    is_high = (GPIO_ReadInputDataBit (ONE_WIRE_PORT, ONE_WIRE_PIN) == Bit_SET) ? 1 : 0;
    delay_usec (120);

    if (low_time > 30 && is_high)
    {
        rtc = 1;
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_read_bit () - read 1 bit
 *
 * Timing:
 *    Master: 1 - 15 us Low
 *    Slave:  Holds Bus Low if bit is 0
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
onewire_read_bit (void)
{
    uint_fast8_t rtc;

    DATA_LOW;
    delay_usec (5);
    DATA_HIGH;
    delay_usec (10);

    rtc = (GPIO_ReadInputDataBit (ONE_WIRE_PORT, ONE_WIRE_PIN) == Bit_SET) ? 1 : 0;
    delay_usec (50);

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_read_byte () - read 1 byte
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint8_t
onewire_read_byte (void)
{
    uint8_t         rtc     = 0;
    uint8_t         mask    = 0x01;
    uint_fast8_t    idx;

    for (idx = 0; idx < 8; idx++)
    {
        if (onewire_read_bit ())
        {
            rtc |= mask;
        }

        mask <<= 1;
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_read () - read n bytes
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
onewire_read (uint8_t * value, uint8_t len)
{
    while (len--)
    {
        *value = onewire_read_byte ();
        value++;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_write_byte () - write 1 byte
 *
 * Timing:
 *    1:    Low:  1 -  15us
 *    0:    Low: 60 - 120us
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
onewire_write_byte (uint8_t value)
{
    uint8_t         mask = 0x01;
    uint_fast8_t    idx;

    for (idx = 0; idx < 8; idx++)
    {
        if (value & mask)
        {
            DATA_LOW;                       // write high bit
            delay_usec (5);
            DATA_HIGH;
            delay_usec (80);
        }
        else
        {
            DATA_LOW;                       // write low bit
            delay_usec (80);
            DATA_HIGH;
            delay_usec (5);
        }

        mask <<= 1;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_write () - write n bytes
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
onewire_write (uint8_t * value, uint8_t len)
{
    while (len--)
    {
        onewire_write_byte (*value);
        value++;
    }
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_get_rom_code () - get rom code of one connected slave
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
onewire_get_rom_code (uint8_t * rom_code)
{
    uint_fast8_t    rtc = 0;

    if (onewire_reset ())
    {
        onewire_write_byte (ONEWIRE_READ_ROM_CMD);
        onewire_read (rom_code, 8);
        onewire_reset ();
        rtc = 1;
    }
    else
    {
        memset (rom_code, 0, 8);
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire_init () - initialize OneWire routines
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
void
onewire_init (void)
{
    static uint32_t             already_called;
    GPIO_InitTypeDef            gpio;

    if (already_called)
    {
        return;
    }

    delay_init (RESOLUTION);

    already_called = 1;

    GPIO_StructInit (&gpio);

    ONE_WIRE_CLK_CMD (ONE_WIRE_CLK, ENABLE);                        // clock enable

    gpio.GPIO_Pin       = ONE_WIRE_PIN;

#if defined (STM32F4XX)
    gpio.GPIO_Mode      = GPIO_Mode_OUT;
    gpio.GPIO_OType     = GPIO_OType_OD;                            // configure as OpenDrain
    gpio.GPIO_PuPd      = GPIO_PuPd_UP;
#elif defined (STM32F10X)
    gpio.GPIO_Mode      = GPIO_Mode_Out_OD;                         // configure as OpenDrain
#endif

    gpio.GPIO_Speed     = GPIO_Speed_50MHz;

    GPIO_Init (ONE_WIRE_PORT, &gpio);

    DATA_HIGH;
}
