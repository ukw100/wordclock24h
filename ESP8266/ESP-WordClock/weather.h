/*----------------------------------------------------------------------------------------------------------------------------------------
 * weather.h - weather stuff
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef WEATHER_H
#define WEATHER_H

extern void get_weather (char *, char *);
extern void get_weather (char *, char *, char *);
extern void get_weather_fc (char *, char *);
extern void get_weather_fc (char *, char *, char *);
extern void get_weather_icon (char *, char *);
extern void get_weather_icon (char *, char *, char *);
extern void get_weather_icon_fc (char *, char *);
extern void get_weather_icon_fc (char *, char *, char *);

#endif
