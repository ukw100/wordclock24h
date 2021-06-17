/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * tempsensor.c - temperature sensor routines
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "ds18xx.h"
#include "tempsensor.h"
#include "eeprom.h"
#include "eeprom-data.h"

TEMP_GLOBALS    gtemp =
{
    0,                                                                      // correction
    0xFF                                                                    // index
};

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * temp_start_conversion () - start conversion
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_start_conversion (uint_fast8_t do_wait)
{
    uint_fast8_t    rtc;

    rtc = ds18xx_start_conversion (do_wait);

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * temp_read_temp_index () - read temperature
 *
 *    temperature_index =   0 ->   0°C
 *    temperature_index = 250 -> 125°C
 *    temperature_index = 255 -> Error
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_read_temp_index (void)
{
    uint_fast8_t    resolution;
    uint_fast8_t    is_negative;
    uint_fast16_t   raw_temp;
    uint_fast8_t    index = 0xFF;

    if (ds18xx_read_raw_temp (&resolution, &is_negative, &raw_temp))
    {
        if (! is_negative)
        {
            index = raw_temp;
            index -= gtemp.correction;                              // correct temperature due to self-heating

        }
        gtemp.index = index;
    }
    return index;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read configuration from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_read_config_from_eeprom (uint32_t eeprom_version)
{
    uint_fast8_t            rtc = 0;
    uint8_t                 temp_correction8;

    if (eeprom_is_up)
    {
        if (eeprom_version >= EEPROM_VERSION_2_1)
        {
            rtc = eeprom_read (EEPROM_DATA_OFFSET_DS18XX_TEMP_CORR, &temp_correction8, EEPROM_DATA_SIZE_DS18XX_TEMP_CORR);

            if (temp_correction8 > 10)
            {
                temp_correction8 = 0;
            }
        }
        else
        {
            temp_correction8 = 0;
            rtc = 1;
        }

        gtemp.correction = temp_correction8;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write configuration to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_write_config_to_eeprom (void)
{
    uint_fast8_t            rtc = 0;
    uint8_t                 temp_correction8;

    temp_correction8    = gtemp.correction;

    if (eeprom_is_up)
    {
        if (eeprom_write (EEPROM_DATA_OFFSET_DS18XX_TEMP_CORR, &temp_correction8, EEPROM_DATA_SIZE_DS18XX_TEMP_CORR))
        {
            rtc = 1;
        }
    }

    return rtc;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * get temperature correction
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_get_temp_correction (void)
{
    return gtemp.correction;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * set temperature correction
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_set_temp_correction (uint_fast8_t new_temp_correction)
{
    gtemp.correction = new_temp_correction;
    temp_write_config_to_eeprom ();
    return gtemp.correction;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * temp_init () - initialize temperature sensor routines
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
temp_init (void)
{
    uint_fast8_t    rtc;

    rtc = ds18xx_init (DS_RESOLUTION_9_BIT);

    return rtc;
}

