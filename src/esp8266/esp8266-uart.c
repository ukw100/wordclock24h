/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * esp8266-uart.c - definitions of uart driver routines
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
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
 * Possible UARTs of STM32F4xx Nucleo:
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

#define UART_PREFIX             esp8266                 // see also esp8266.c

#if defined (STM32F4XX)                                 // STM32F4xx Nucleo Board: we use USART6 ALT1: PA11 | PA12
#  define UART_NUMBER           6                       // UART number on STM32F4xx (1-6 for UART)
#  define UART_ALTERNATE        1                       // ALTERNATE number
#elif defined (STM32F103)                               // STM32F103C8T6 Mini Development Board: we use USART2 ALT0: PA2 | PA3
#  define UART_NUMBER           2                       // UART number on STM32F1xx
#  define UART_ALTERNATE        0                       // ALTERNATE number
#else
#  error unknown STM32
#endif

#define UART_TXBUFLEN           128                     // ringbuffer size for UART TX
#define UART_RXBUFLEN           256                     // ringbuffer size for UART RX

#include "uart-driver.h"
