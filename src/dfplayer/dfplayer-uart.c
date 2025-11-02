/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer-uart.c - definitions of (soft-)uart driver routines
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
 *
 * Possible UARTs of STM32F10x:
 *           ALTERNATE=0    ALTERNATE=1    ALTERNATE=2
 *  +--------------------------------------------------+
 *  | UART | TX   | RX   || TX   | RX   || TX   | RX   |
 *  |======|======|======||======|======||======|======|
 *  | 1    | PA9  | PA10 || PB6  | PB7  ||      |      |
 *  | 2    | PA2  | PA3  || PD5  | PD6  ||      |      |
 *  | 3    | PB10 | PB11 || PC10 | PC11 || PD8  | PD9  |
 *  +--------------------------------------------------+
 *
 * Possible UARTs of STM32F4x1:
 *           ALTERNATE=0    ALTERNATE=1    ALTERNATE=2
 *  +--------------------------------------------------+
 *  | UART | TX   | RX   || TX   | RX   || TX   | RX   |
 *  |======|======|======||======|======||======|======|
 *  | 1    | PA9  | PA10 || PB6  | PB7  ||      |      |
 *  | 2    | PA2  | PA3  || PD5  | PD6  ||      |      |
 *  | 6    | PC6  | PC7  || PA11 | PA12 ||      |      |
 *  +--------------------------------------------------+
 *
 * Possible UARTs of STM32F407:
 *           ALTERNATE=0    ALTERNATE=1    ALTERNATE=2
 *  +--------------------------------------------------+
 *  | UART | TX   | RX   || TX   | RX   || TX   | RX   |
 *  |======|======|======||======|======||======|======|
 *  | 1    | PA9  | PA10 || PB6  | PB7  ||      |      |
 *  | 2    | PA2  | PA3  || PD5  | PD6  ||      |      |
 *  | 3    | PB10 | PB11 || PC10 | PC11 || PD8  | PD9  |
 *  | 4    | PA0  | PA1  || PC10 | PC11 ||      |      |
 *  | 5    | PC12 | PD2  ||      |      ||      |      |
 *  | 6    | PC6  | PC7  || PG14 | PG9  ||      |      |
 *  +--------------------------------------------------+
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

#if defined (BLACK_BOARD)                               // STM32F407VE Black Board
#  define UART_NUMBER           1                       // UART number on STM32F407 (USART1 (ALT1): TX=PB6 RX=PB7)
#  define UART_ALTERNATE        1                       // ALTERNATE number

#elif defined (NUCLEO_BOARD)                            // STM32F4x1 Nucleo Board
#  define UART_NUMBER           1                       // UART number on STM32F4xx (USART1 (ALT1): TX=PB6 RX=PB7)
#  define UART_ALTERNATE        1                       // ALTERNATE number

#elif defined (BLACKPILL_BOARD)                         // STM32F4x1 BlackPill Board
#  define UART_NUMBER           6                       // UART number on STM32F401 (ALT1): TX=PA11 RX=PA12)
#  define UART_ALTERNATE        1                       // ALTERNATE number

#elif defined (BLUEPILL_BOARD)                          // STM32F103C8T6 BluePill Board: we use USART3 ALT0: PB10 | PB11
#  define UART_NUMBER           3                       // UART number on STM32F1xx
#  define UART_ALTERNATE        0                       // ALTERNATE number
#else
#  errror unknown STM32
#endif

#include "uart-driver.h"
