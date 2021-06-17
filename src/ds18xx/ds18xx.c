/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds18xx.c - DS18xx routines
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This library supports the following temperature sensors:
 *
 *   Sensor      Family Code
 *   DS1820         0x10
 *   DS18S20        0x10
 *   DS1822         0x22
 *   DS18B20        0x28
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "onewire.h"
#include "ds18xx.h"
#include "ds18xx-config.h"
#include "eeprom.h"
#include "eeprom-data.h"

#define ONEWIRE_CONVERT_CMD         0x44                                        // start temperature conversion
#define ONEWIRE_RD_SCRATCH_CMD      0xBE                                        // read temperature
#define ONEWIRE_WR_SCRATCH_CMD      0x4E                                        // write configuration register
#define ONEWIRE_COPY_SCRATCH_CMD    0x48                                        // copy configuration register into EEPROM

#define DS18XX_ROM_CODE_LEN         8                                           // ROM code is 8 bytes long
#define DS18XX_FAMILY_CODE_POS      0                                           // first byte is family code

#define DS18XX_DATA_BUF_LEN         9

DS18XX_GLOBALS                      ds18xx;

static uint8_t                      ds18xx_rom_code[DS18XX_ROM_CODE_LEN];       // ROM code of DS18xx
static uint8_t                      ds18xx_data_buf[DS18XX_DATA_BUF_LEN];       // data from DS18xx
static uint_fast8_t                 family_code;

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds1822B20_set_resolution () - set resolution
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint8_t
ds1822B20_set_resolution (uint_fast8_t resolution)
{
    uint8_t   rtc = 0;
    uint8_t   conf_reg;

    conf_reg = (resolution - 9) << 5;                                   // 9->0x00 10->0x20 11->0x40 12->0x60

    if (onewire_reset ())
    {
        onewire_write_byte (ONEWIRE_MATCH_ROM_CMD);                     // send command: select slave
        onewire_write (ds18xx_rom_code, DS18XX_ROM_CODE_LEN);           // send ROM code
        onewire_write_byte (ONEWIRE_WR_SCRATCH_CMD);                    // send command: write configuration register

        onewire_write_byte (0x7F);                                      // alarm high register
        onewire_write_byte (0x80);                                      // alarm low register
        onewire_write_byte (conf_reg);                                  // configuration byte

        onewire_reset ();
        rtc = 1;
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds1822B20_write_resolution () - store resolution in EEPROM
 *
 *  Max EEPROM write cycle time is 10 ms.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#define EEPROM_WRITE_TIMEOUT        (15000/60)                          // we wait 15ms (in 60us slots)

static uint8_t
ds1822B20_write_resolution (uint_fast8_t resolution)
{
    uint8_t         rtc = 0;
    uint_fast8_t    wait_bit;
    uint32_t        timeout = 0;

    rtc = ds1822B20_set_resolution (resolution);

    if (rtc)
    {
        if (onewire_reset ())
        {
            onewire_write_byte (ONEWIRE_MATCH_ROM_CMD);                 // send command: select slave
            onewire_write (ds18xx_rom_code, DS18XX_ROM_CODE_LEN);       // send ROM code
            onewire_write_byte (ONEWIRE_COPY_SCRATCH_CMD);              // send command: copy configuration register into EEPROM

            timeout = 0;

            do
            {
                wait_bit = onewire_read_bit ();
                timeout++;
            } while (! wait_bit && timeout < EEPROM_WRITE_TIMEOUT);

            onewire_reset ();

            if (timeout < EEPROM_WRITE_TIMEOUT)
            {
                rtc = 1;
            }
        }
    }

    return rtc;
}

#define CONVERSION_TIMEOUT_MAX  (800000/60)                         // timeout after 800ms
/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds18xx_start_conversion () - start conversion
 *
 * conversion times:
 *   DS_RESOLUTION_12_BIT: 12 bit resolution 0.0625 °C - conversion time ~750 ms: wait 800ms = 13333 x 60 us
 *   DS_RESOLUTION_11_BIT: 11 bit resolution 0.125  °C - conversion time ~375 ms: wait 400ms =  6666 x 60 us
 *   DS_RESOLUTION_10_BIT: 10 bit resolution 0.25   °C - conversion time ~187 ms: wait 200ms =  3333 x 60 us
 *   DS_RESOLUTION_9_BIT:   9 bit resolution 0.5    °C - conversion time  ~93 ms: wait 100ms =  1666 x 60 us
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
ds18xx_start_conversion (uint_fast8_t do_wait)
{
    uint_fast8_t    wait_bit;
    uint32_t        timeout = 0;
    uint_fast8_t rtc = 1;                                                                       // assume success

    if (onewire_reset ())
    {
        onewire_write_byte (ONEWIRE_MATCH_ROM_CMD);                                             // send command: select slave
        onewire_write (ds18xx_rom_code, DS18XX_ROM_CODE_LEN);                                   // send ROM code
        onewire_write_byte (ONEWIRE_CONVERT_CMD);                                               // send command: start temp measurement

        timeout = 0;

        if (do_wait)
        {
            do
            {
                wait_bit = onewire_read_bit ();
                timeout++;
            } while (! wait_bit && timeout < CONVERSION_TIMEOUT_MAX);
        }

        if (timeout >= CONVERSION_TIMEOUT_MAX)
        {
            onewire_reset ();                                                                   // timeout -> reset
            rtc = 0;
        }
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds18xx_read_raw_temp () - read temperature
 *
 * conversion times:
 *   DS_RESOLUTION_12_BIT: 12 bit resolution 0.0625 °C - conversion time ~750 ms: wait 800ms = 13333 x 60 us
 *   DS_RESOLUTION_11_BIT: 11 bit resolution 0.125  °C - conversion time ~375 ms: wait 400ms =  6666 x 60 us
 *   DS_RESOLUTION_10_BIT: 10 bit resolution 0.25   °C - conversion time ~187 ms: wait 200ms =  3333 x 60 us
 *   DS_RESOLUTION_9_BIT:   9 bit resolution 0.5    °C - conversion time  ~93 ms: wait 100ms =  1666 x 60 us
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

uint_fast8_t
ds18xx_read_raw_temp (uint_fast8_t * resolution_p, uint_fast8_t * is_negative_p, uint_fast16_t * raw_temp_p)
{
    uint_fast8_t    rtc = 0;

    if (onewire_reset ())
    {
        onewire_write_byte (ONEWIRE_MATCH_ROM_CMD);                                     // send command: select slave
        onewire_write (ds18xx_rom_code, DS18XX_ROM_CODE_LEN);                           // send ROM code
        onewire_write_byte (ONEWIRE_RD_SCRATCH_CMD);                                    // send command: read temperature
        onewire_read (ds18xx_data_buf, DS18XX_DATA_BUF_LEN);                            // read 9 bytes from slave

        onewire_reset ();

        switch (family_code)
        {
            case DS1822_FAMILY_CODE:
            case DS18B20_FAMILY_CODE:
            {
                uint8_t lower_byte = ds18xx_data_buf[0];                                // lower byte of temperature
                uint8_t upper_byte = ds18xx_data_buf[1];                                // higher byte of temperature
                uint8_t conf_reg   = ds18xx_data_buf[4];                                // configuration register
                uint8_t resolution = ((conf_reg >> 5) & 0x03) + DS_RESOLUTION_9_BIT;    // resolution: 9, 10, 11, or 12

                *resolution_p   = resolution;
                *is_negative_p  = (upper_byte & 0x80) ? 1 : 0;
                *raw_temp_p     = ((upper_byte & 0x07) << (resolution - 4)) | (lower_byte >> (DS_RESOLUTION_12_BIT - resolution));
                rtc = 1;
                break;
            }

            case DS1820_FAMILY_CODE:
            default:
            {
                *resolution_p   = DS_RESOLUTION_9_BIT;
                *is_negative_p  = (ds18xx_data_buf[1] & 0x80) ? 1 : 0;                  // 9 bit resolution
                *raw_temp_p     = ds18xx_data_buf[0];
                rtc = 1;
                break;
            }
        }
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds18xx_read_temp () - read temperature
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
ds18xx_read_temp (float * temperature_p)
{
    float           rtc = 0;
    uint_fast8_t    is_negative;
    uint_fast8_t    resolution;
    uint_fast16_t   raw_temp;
    uint_fast16_t   xor_mask;
    float           divisor;

    if (ds18xx_start_conversion (1))
    {
        rtc = ds18xx_read_raw_temp (&resolution, &is_negative, &raw_temp);

        if (rtc)
        {
            divisor = (float) (1 << (resolution - DS_RESOLUTION_9_BIT + 1));            // 9->2.0,  10->4.0,  11->8.0,  12->16.0

            if (is_negative)
            {
                switch (resolution)
                {
                    case DS_RESOLUTION_12_BIT: xor_mask = 0x07FF; break;
                    case DS_RESOLUTION_11_BIT: xor_mask = 0x03FF; break;
                    case DS_RESOLUTION_10_BIT: xor_mask = 0x01FF; break;
                    default:
                    case DS_RESOLUTION_9_BIT:  xor_mask = 0x00FF; break;
                }

                raw_temp ^= xor_mask;
                raw_temp++;
                *temperature_p = -((float) raw_temp / divisor);
            }
            else
            {
                *temperature_p = (float) raw_temp / divisor;
            }
        }
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds18xx_get_family_code () - get family code of DS18xx
 *
 * Return values:
 *
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
ds18xx_get_family_code (void)
{
    return family_code;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * ds18xx_init () - initialize DS18xx routines
 *
 * resolution:  only for DS18B20 and DS1822, otherwise ignored:
 *              DS_RESOLUTION_9_BIT
 *              DS_RESOLUTION_10_BIT
 *              DS_RESOLUTION_11_BIT
 *              DS_RESOLUTION_12_BIT
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
ds18xx_init (uint_fast8_t resolution)
{
    uint_fast8_t rtc = 0;
    uint_fast8_t conf_reg;
    uint_fast8_t old_resolution;

    if (resolution >= DS_RESOLUTION_9_BIT && resolution <= DS_RESOLUTION_12_BIT)
    {
        onewire_init ();

        if (onewire_reset ())
        {
            rtc = onewire_get_rom_code (ds18xx_rom_code);
            family_code = ds18xx_rom_code[DS18XX_FAMILY_CODE_POS];

            ds18xx.is_up = 1;

            switch (family_code)                                                // get and set resolution (DS18B20 and DS1822 only)
            {
                case DS1822_FAMILY_CODE:
                case DS18B20_FAMILY_CODE:
                {
                    if (onewire_reset ())
                    {
                        onewire_write_byte (ONEWIRE_MATCH_ROM_CMD);             // send command: select slave
                        onewire_write (ds18xx_rom_code, DS18XX_ROM_CODE_LEN);   // send rom code
                        onewire_write_byte (ONEWIRE_RD_SCRATCH_CMD);            // send command: read from scratch
                        onewire_read (ds18xx_data_buf, DS18XX_DATA_BUF_LEN);    // read 9 bytes from slave

                        conf_reg = ds18xx_data_buf[4];                          // configuration register
                        old_resolution = ((conf_reg >> 5) & 0x03) + 9;          // resolution: 9, 10, 11, or 12

                        if (old_resolution != resolution)
                        {
                            ds1822B20_write_resolution (resolution);
                        }
                    }
                    break;
                }
            }
        }
    }

    return rtc;
}
