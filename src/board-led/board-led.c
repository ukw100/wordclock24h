/*-------------------------------------------------------------------------------------------------------------------------------------------
 * board-led.c - LED routines
 *
 * Copyright (c) 2014-2026 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if defined (STM32F10X)
#include "stm32f10x.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#endif
#include "board-led.h"
#include "io.h"

#if defined (DISCO_BOARD)                                                                       // STM32F4 Discovery Board: PD12
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOD
#define BOARD_LED_PORT              GPIOD
#define BOARD_LED_PIN               GPIO_Pin_12
#define BOARD_LED_ON                GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)                 // active high
#define BOARD_LED_OFF               GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)

#elif defined (BLACK_BOARD)                                                                     // STM32F407VE Black Board: PA6
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOA
#define BOARD_LED_PORT              GPIOA
#define BOARD_LED_PIN               GPIO_Pin_6
#define BOARD_LED_ON                GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)               // active low
#define BOARD_LED_OFF               GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)

#elif defined (NUCLEO_BOARD)                                                                    // STM32F4xx Nucleo Board: PA5
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOA
#define BOARD_LED_PORT              GPIOA
#define BOARD_LED_PIN               GPIO_Pin_5
#define BOARD_LED_ON                GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)                 // active high
#define BOARD_LED_OFF               GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)

#elif defined (BLACKPILL_BOARD)                                                                 // STM32F4x1 BlackPill Board: PC13
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOC
#define BOARD_LED_PORT              GPIOC
#define BOARD_LED_PIN               GPIO_Pin_13
#define BOARD_LED_ON                GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)               // active low
#define BOARD_LED_OFF               GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)

#elif defined (BLUEPILL_BOARD)                                                                  // STM32F103 BluePill Board: PC13
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_APB2PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_APB2Periph_GPIOC
#define BOARD_LED_PORT              GPIOC
#define BOARD_LED_PIN               GPIO_Pin_13
#define BOARD_LED_ON                GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)               // active low
#define BOARD_LED_OFF               GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_PIN)

#else
#error STM32 unknown
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize LED port
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
board_led_init (void)
{
    BOARD_LED_PERIPH_CLOCK_CMD (BOARD_LED_PERIPH, ENABLE);     // enable clock for LED Port
    GPIO_SET_PIN_OUT_PP(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_Speed_2MHz);
    board_led_off ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * green LED on
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
board_led_on (void)
{
    BOARD_LED_ON;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * green LED off
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
board_led_off (void)
{
    BOARD_LED_OFF;
}
