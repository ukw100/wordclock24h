/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart.h - declaration of UART driver routines for STM32F4XX or STM32F10X
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (STM32F10X)
#  include "stm32f10x.h"
#  include "stm32f10x_gpio.h"
#  include "stm32f10x_usart.h"
#  include "stm32f10x_rcc.h"
#elif defined (STM32F4XX)
#  include "stm32f4xx.h"
#  include "stm32f4xx_gpio.h"
#  include "stm32f4xx_usart.h"
#  include "stm32f4xx_rcc.h"
#endif

#include "misc.h"

#define _UART_CONCAT(a,b)                a##b
#define UART_CONCAT(a,b)                 _UART_CONCAT(a,b)

extern void             UART_CONCAT(UART_PREFIX, _uart_init)            (uint32_t);
extern void             UART_CONCAT(UART_PREFIX, _uart_putc)            (uint_fast8_t);
extern void             UART_CONCAT(UART_PREFIX, _uart_puts)            (const char *);
extern uint_fast8_t     UART_CONCAT(UART_PREFIX, _uart_char_available)  (void);
extern uint_fast8_t     UART_CONCAT(UART_PREFIX, _uart_getc)            (void);
extern uint_fast8_t     UART_CONCAT(UART_PREFIX, _uart_poll)            (uint_fast8_t *);
extern uint_fast16_t    UART_CONCAT(UART_PREFIX, _uart_rsize)           (void);
extern void             UART_CONCAT(UART_PREFIX, _uart_flush)           (void);
extern uint_fast16_t    UART_CONCAT(UART_PREFIX, _uart_read)            (char *, uint_fast16_t);
extern uint_fast16_t    UART_CONCAT(UART_PREFIX, _uart_write)           (char *, uint_fast16_t);

