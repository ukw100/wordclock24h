/*-------------------------------------------------------------------------------------------------------------------------------------------
 * esp-spiffs.h - access SPIFFS filesystem of ESP8266
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef ESP_DIFFS_H
#define ESP_DIFFS_H

#if 0 // currently not used
extern int_fast16_t         esp_spiffs_open_file (const char *);
extern void                 esp_spiffs_close_file (void);
extern int_fast16_t         esp_spiffs_read_file (unsigned char *, int_fast16_t);
#endif

extern void                 esp_diffs_get_icon (const char *, const char *);
extern uint_fast8_t         esp_diffs_read_icon (DISPLAY_ICON *);
#endif // ESP_DIFFS_H
