/*-------------------------------------------------------------------------------------------------------------------------------------------
 * log.c - log routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include "log.h"

#undef UART_PREFIX
#define UART_PREFIX     log
#include "uart.h"

#define STRBUF_SIZE 256

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * log a formatted message (by va_list)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
log_vprintf (const char * fmt, va_list ap)
{
    static char str_buf[STRBUF_SIZE];

    (void) vsnprintf ((char *) str_buf, STRBUF_SIZE, fmt, ap);
    (void) log_uart_puts (str_buf);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * log a formatted message (varargs)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
log_printf (const char * fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    log_vprintf (fmt, ap);
    va_end (ap);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * init log functions
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
log_init (uint32_t baudrate)
{
    log_uart_init (baudrate);
}
