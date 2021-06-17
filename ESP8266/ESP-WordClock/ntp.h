/*----------------------------------------------------------------------------------------------------------------------------------------
 * ntp.h - declarations of public NTP functions
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef NTP_H
#define NTP_H
extern void     ntp_setup (void);
extern void     ntp_poll_time (void);
extern void     ntp_get_time (IPAddress);
extern void     ntp_get_time (char *);
extern void     ntp_get_time (void);

#endif
