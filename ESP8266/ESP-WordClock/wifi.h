/*----------------------------------------------------------------------------------------------------------------------------------------
 * wifi.h - extern declarations for WIFI stuff
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef WIFI_H
#define WIFI_H

#define WIFI_MAX_SSID_LEN           32
#define WIFI_MAX_IP_ADDRESS_LEN     15

extern int      wifi_ap_mode;
extern char     wifi_ssid[WIFI_MAX_SSID_LEN + 1];
extern char     wifi_ip_address[WIFI_MAX_IP_ADDRESS_LEN + 1];

extern int      wifi_connect(const char *, const char *);
extern void     wifi_wps (void);
extern void     wifi_ap (const char *, const char *);
extern void     wifi_check_if_started (void);

#endif
