/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer-uart.c - definitions of (soft-)uart driver routines
 *
 * Copyright (c) 2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#define UART_PREFIX             dfplayer                // see also dfplayer.c
#define UART_TXBUFLEN           32                      // ringbuffer size for UART TX
#define UART_RXBUFLEN           32                      // ringbuffer size for UART RX

#if defined (STM32F4XX)                                 // STM32F4xx Nucleo Board: we use USART1 ALT1: PB6 | PB7
#  define UART_NUMBER           1                       // UART number on STM32F1xx
#  define UART_ALTERNATE        1                       // ALTERNATE number
#elif defined (STM32F103)                               // STM32F103C8T6 Mini Development Board: we use USART3 ALT0: PB10 | PB11
#  define UART_NUMBER           3                       // UART number on STM32F1xx
#  define UART_ALTERNATE        0                       // ALTERNATE number
#else
#  errror unknown STM32
#endif

#include "uart-driver.h"
