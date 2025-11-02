/*-------------------------------------------------------------------------------------------------------------------------------------------
 * button.c - read user button of STM32F4 Discovery / STM32F4xx Nucleo / STM32F103
 *
 * Copyright (c) 2014-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "button.h"
#include "io.h"

#if defined (DISCO_BOARD)                                               // STM32F4 Discovery Board PA0
#define BUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define BUTTON_PERIPH               RCC_AHB1Periph_GPIOA
#define BUTTON_PORT                 GPIOA
#define BUTTON_PIN                  GPIO_Pin_0
#define BUTTON_PRESSED              Bit_SET                             // pressed if high

#elif defined (BLACK_BOARD)                                             // STM32F407VE Black Board PE4
#define BUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define BUTTON_PERIPH               RCC_AHB1Periph_GPIOE
#define BUTTON_PORT                 GPIOE
#define BUTTON_PIN                  GPIO_Pin_4
#define BUTTON_PRESSED              Bit_RESET                           // pressed if low

#elif defined (NUCLEO_BOARD)                                            // STM32F4xx Nucleo Board PC13
#define BUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define BUTTON_PERIPH               RCC_AHB1Periph_GPIOC
#define BUTTON_PORT                 GPIOC
#define BUTTON_PIN                  GPIO_Pin_13
#define BUTTON_PRESSED              Bit_RESET                           // pressed if low

#elif defined (BLACKPILL_BOARD)                                         // STM32F4xx Nucleo Board PC13
#define BUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define BUTTON_PERIPH               RCC_AHB1Periph_GPIOA
#define BUTTON_PORT                 GPIOA
#define BUTTON_PIN                  GPIO_Pin_0
#define BUTTON_PRESSED              Bit_RESET                           // pressed if low

#elif defined (BLUEPILL_BOARD)                                          // STM32F103 BluePill PA6
#define BUTTON_PERIPH_CLOCK_CMD     RCC_APB2PeriphClockCmd
#define BUTTON_PERIPH               RCC_APB2Periph_GPIOA
#define BUTTON_PORT                 GPIOA
#define BUTTON_PIN                  GPIO_Pin_6
#define BUTTON_PRESSED              Bit_RESET                           // pressed if low

#else
#error STM32 unknown
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize button port
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
button_init (void)
{
    BUTTON_PERIPH_CLOCK_CMD (BUTTON_PERIPH, ENABLE);

#if defined (DISCO_BOARD)                                   // STM32F407 Disco Board with external pulldown
    GPIO_SET_PIN_IN_NOPULL(BUTTON_PORT, BUTTON_PIN, GPIO_Speed_2MHz);
#elif defined (BLACK_BOARD)                                 // STM32F407 Black Board with internal pullup
    GPIO_SET_PIN_IN_UP(BUTTON_PORT, BUTTON_PIN, GPIO_Speed_2MHz);
#elif defined (NUCLEO_BOARD)                                // STM32F4x1 Nucleo with external pullup
    GPIO_SET_PIN_IN_NOPULL(BUTTON_PORT, BUTTON_PIN, GPIO_Speed_2MHz);
#elif defined (BLACKPILL_BOARD)                             // STM32F4x1 BlackPill Board with internal pullup
    GPIO_SET_PIN_IN_UP(BUTTON_PORT, BUTTON_PIN, GPIO_Speed_2MHz);
#elif defined (BLUEPILL_BOARD)                              // STM32F103 BluePill with internal pullup
    GPIO_SET_PIN_IN_UP(BUTTON_PORT, BUTTON_PIN, GPIO_Speed_2MHz);

#else
#error STM32 unknown
#endif
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check if button pressed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
button_pressed (void)
{
    if (GPIO_GET_BIT(BUTTON_PORT, BUTTON_PIN) == BUTTON_PRESSED)
    {
        return 1;
    }
    return 0;
}
