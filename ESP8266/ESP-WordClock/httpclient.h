/*----------------------------------------------------------------------------------------------------------------------------------------
 * httpclient.h - http client
 *
 * Copyright (c) 2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

extern int    httpclient (const char *, const char *, const char *);
extern int    httpclient_read (int *);
extern int    httpclient_read_line (unsigned char *, int, int *);
extern void   httpclient_stop (void);

#endif
