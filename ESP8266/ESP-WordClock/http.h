/*----------------------------------------------------------------------------------------------------------------------------------------
 * http.h - extern declarations for http server
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef HTTP_H
#define HTTP_H

#define http_send_FS(x)     http_send(FS(x))                                         // leave string constants in flash memory

extern uint_fast8_t         http (const char *, const char *);
extern void                 http_send (const char *);
extern void                 http_flush (void);
extern void                 http_server_loop (void);
extern void                 http_server_begin (void);

#endif
