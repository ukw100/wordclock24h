/*-------------------------------------------------------------------------------------------------------------------------------------------
 * remote-ir.c - remote IR routines using IRMP
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "wclock24h-config.h"
#include "irmp.h"
#include "remote-ir.h"
#include "display.h"
#include "eeprom.h"
#include "eeprom-data.h"
#include "log.h"

static  IRMP_DATA   irmp_data_array[N_REMOTE_IR_CMDS];

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * search for a previously stored IR command
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
search_cmd (IRMP_DATA * ip, uint_fast8_t max_cmds)
{
    uint_fast8_t    rtc = REMOTE_IR_CMD_INVALID;
    uint_fast8_t    i;

    for (i = 0; i < max_cmds; i++)
    {
        if (ip->protocol == irmp_data_array[i].protocol &&
            ip->address  == irmp_data_array[i].address  &&
            ip->command  == irmp_data_array[i].command)
        {
            rtc = i;
            break;
        }
    }
    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read an IR command
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
remote_ir_get_cmd (void)
{
    static uint_fast16_t    frame;
    static uint_fast8_t     last_repetition_flag;
    IRMP_DATA               irmp_data;
    uint_fast8_t            rtc = REMOTE_IR_CMD_INVALID;

    if (irmp_get_data (&irmp_data))
    {
        frame++;

        if (last_repetition_flag == 0 && (irmp_data.flags & IRMP_FLAG_REPETITION))  // ignore first repetition frame
        {
            last_repetition_flag = 1;
            return rtc;
        }

        last_repetition_flag = (irmp_data.flags & IRMP_FLAG_REPETITION) ? 1: 0;
        rtc = search_cmd (&irmp_data, N_REMOTE_IR_CMDS);
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * learn remote IR control
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
remote_ir_learn (void)
{
    uint_fast8_t    rtc = 1;
    uint_fast8_t    i;
    IRMP_DATA       dummy;
    const char * t;

    for (i = 0; i < N_REMOTE_IR_CMDS; i++)
    {
        switch (i)
        {
            case REMOTE_IR_CMD_POWER:                         t = "power off/on";                     break;
            case REMOTE_IR_CMD_OK:                            t = "ok";                               break;
            case REMOTE_IR_CMD_DECREMENT_DISPLAY_MODE:        t = "decrement display mode";           break;
            case REMOTE_IR_CMD_INCREMENT_DISPLAY_MODE:        t = "increment display mode";           break;
            case REMOTE_IR_CMD_DECREMENT_ANIMATION_MODE:      t = "decrement animation mode";         break;
            case REMOTE_IR_CMD_INCREMENT_ANIMATION_MODE:      t = "increment animation mode";         break;
            case REMOTE_IR_CMD_DECREMENT_HOUR:                t = "decrement hour";                   break;
            case REMOTE_IR_CMD_INCREMENT_HOUR:                t = "increment hour";                   break;
            case REMOTE_IR_CMD_DECREMENT_MINUTE:              t = "decrement minute";                 break;
            case REMOTE_IR_CMD_INCREMENT_MINUTE:              t = "increment minute";                 break;
            case REMOTE_IR_CMD_DECREMENT_BRIGHTNESS_RED:      t = "decrement red brightness";         break;
            case REMOTE_IR_CMD_INCREMENT_BRIGHTNESS_RED:      t = "increment red brightness";         break;
            case REMOTE_IR_CMD_DECREMENT_BRIGHTNESS_GREEN:    t = "decrement green brightness";       break;
            case REMOTE_IR_CMD_INCREMENT_BRIGHTNESS_GREEN:    t = "increment green brightness";       break;
            case REMOTE_IR_CMD_DECREMENT_BRIGHTNESS_BLUE:     t = "decrement blue brightness";        break;
            case REMOTE_IR_CMD_INCREMENT_BRIGHTNESS_BLUE:     t = "increment blue brightness";        break;
            case REMOTE_IR_CMD_DECREMENT_BRIGHTNESS:          t = "decrement global brightness";      break;
            case REMOTE_IR_CMD_INCREMENT_BRIGHTNESS:          t = "increment global brightness";      break;
            case REMOTE_IR_CMD_AUTO_BRIGHTNESS_CONTROL:       t = "toggle auto brightness";           break;
            case REMOTE_IR_CMD_GET_TEMPERATURE:               t = "get temperature";                  break;
        }

        display_set_ticker ((const unsigned char *) t, 1);
        log_message (t);

        irmp_get_data (&dummy);

        while (1)
        {
            if (irmp_get_data (&irmp_data_array[i]))                                            // read ir data
            {
                if ((irmp_data_array[i].flags & IRMP_FLAG_REPETITION) == 0)                     // no repetition
                {                                                                               // accept if ...
                    if (i == 0 ||                                                               // ... it's the first key (power)
                        search_cmd (&irmp_data_array[i], i) == REMOTE_IR_CMD_INVALID ||         // ... key has not been stored previously
                        search_cmd (&irmp_data_array[i], i) == REMOTE_IR_CMD_POWER)             // ... user wants to skip it (by power key)
                    {
                        break;                                                                  // next command...
                    }
                }
            }
        }
    }

    display_set_ticker ((const unsigned char *) "  Thank you!", 1);

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read IR codes from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
remote_ir_read_codes_from_eeprom (void)
{
    uint8_t         packed_irmp_data[PACKED_IRMP_DATA_SIZE];
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        uint_fast8_t    i;
        uint_fast16_t   start_addr = EEPROM_DATA_OFFSET_IRMP_DATA;

        for (i = 0; i < N_REMOTE_IR_CMDS; i++)
        {
            rtc = eeprom_read (start_addr, (uint8_t *) &packed_irmp_data, PACKED_IRMP_DATA_SIZE);

            irmp_data_array[i].protocol = packed_irmp_data[0];
            irmp_data_array[i].address  = packed_irmp_data[1] | (packed_irmp_data[2] << 8);
            irmp_data_array[i].command  = packed_irmp_data[3] | (packed_irmp_data[4] << 8);

            start_addr += PACKED_IRMP_DATA_SIZE;
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write IR codes to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
remote_ir_write_codes_to_eeprom (void)
{
    uint8_t         packed_irmp_data[PACKED_IRMP_DATA_SIZE];
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        uint_fast8_t    i;
        uint_fast16_t   start_addr = EEPROM_DATA_OFFSET_IRMP_DATA;

        for (i = 0; i < N_REMOTE_IR_CMDS; i++)
        {
            packed_irmp_data[0]         = irmp_data_array[i].protocol;
            packed_irmp_data[1]         = irmp_data_array[i].address & 0xFF;
            packed_irmp_data[2]         = irmp_data_array[i].address >> 8;
            packed_irmp_data[3]         = irmp_data_array[i].command & 0xFF;
            packed_irmp_data[4]         = irmp_data_array[i].command >> 8;

            rtc = eeprom_write (start_addr, (uint8_t *) &packed_irmp_data, PACKED_IRMP_DATA_SIZE);
            start_addr += PACKED_IRMP_DATA_SIZE;
        }
    }

    return rtc;
}
