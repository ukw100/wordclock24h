/*-------------------------------------------------------------------------------------------------------------------------------------------
 * button.c - read user button of STM32F4 Discovery / STM32F4xx Nucleo / STM32F103
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "button.h"

#if defined (STM32F407VG)                                               // STM32F4 Discovery Board PA0
#define BUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define BUTTON_PERIPH               RCC_AHB1Periph_GPIOA
#define BUTTON_PORT                 GPIOA
#define BUTTON_PIN                  GPIO_Pin_0
#define BUTTON_PRESSED              Bit_SET                             // pressed if high

#elif defined (STM32F4XX)                                               // STM32F4xx Nucleo Board PC13
#define BUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define BUTTON_PERIPH               RCC_AHB1Periph_GPIOC
#define BUTTON_PORT                 GPIOC
#define BUTTON_PIN                  GPIO_Pin_13
#define BUTTON_PRESSED              Bit_RESET                           // pressed if low

#elif defined (STM32F103)                                               // STM32F103 Mini Development Board PA6

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
    GPIO_InitTypeDef gpio;

    GPIO_StructInit (&gpio);
    BUTTON_PERIPH_CLOCK_CMD (BUTTON_PERIPH, ENABLE);

    gpio.GPIO_Pin = BUTTON_PIN;

#if defined (STM32F10X)
    gpio.GPIO_Mode = GPIO_Mode_IPU;                         // use pin as input with internal pullup
#elif defined (STM32F4XX)
    gpio.GPIO_Mode = GPIO_Mode_IN;                          // use pin as input, button has already an external pullup
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
#endif

    GPIO_Init(BUTTON_PORT, &gpio);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check if button pressed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
button_pressed (void)
{
    if (GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == BUTTON_PRESSED)
    {
        return 1;
    }
    return 0;
}
