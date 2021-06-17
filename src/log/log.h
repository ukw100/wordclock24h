/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * log.h - declarations of logging routines
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdarg.h>

#undef UART_PREFIX
#define UART_PREFIX                 log
#include "uart.h"

#define log_putc(c)                 do { log_uart_putc (c); } while (0)
#define log_puts(s)                 do { log_uart_puts (s); } while (0)
#define log_message(s)              do { log_uart_puts (s); log_uart_puts ("\r\n"); } while (0)
#define log_flush()                 do { log_uart_flush (); } while (0)

#ifdef DEBUG
#define debug_log_message(s)        log_message(s)
#define debug_log_printf(fmt,...)   log_printf(fmt, __VA_ARGS__)
#define debug_log_flush()           log_flush()
#else
#define debug_log_message(s)
#define debug_log_printf(fmt,...)
#define debug_log_flush()
#endif

extern void                         log_vprintf (const char *, va_list);
extern void                         log_printf (const char *, ...);
extern void                         log_init (uint32_t);
