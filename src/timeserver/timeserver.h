/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * timeserver.h - declarations of ESP8266 routines
 *
 * Copyright (c) 2014-2024 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <string.h>
#include <stdint.h>
#include <time.h>
#include "esp8266.h"

#define MAX_IPADDR_LEN          16
#define GMT                      0                              // GMT offset

typedef struct
{
    char            timeserver[MAX_IPADDR_LEN + 1];             // timeserver
    int_fast16_t    timezone;                                   // from -12 to +12
    uint_fast8_t    observe_summertime;                         // flag: observe summertime
} TIMESERVER_GLOBALS;

extern TIMESERVER_GLOBALS   timeserver;

extern uint_fast8_t         timeserver_read_data_from_eep (void);
extern uint_fast8_t         timeserver_write_data_to_eep (void);
extern void                 timeserver_cmd (void);
extern uint_fast8_t         timeserver_set_timezone (int_fast16_t newtimezone, uint_fast8_t observe_summertime);
extern uint_fast8_t         timeserver_set_timeserver (char * new_timeserver);
extern void                 timeserver_start_timeserver_request (void);
extern void                 timeserver_convert_time (struct tm * tmp, uint32_t seconds_since_1900);

#endif
