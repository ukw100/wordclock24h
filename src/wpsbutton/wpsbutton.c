/*-------------------------------------------------------------------------------------------------------------------------------------------
 * wpsbutton.c - read WPS button of STM32F4xx Nucleo and STM32F103
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "wpsbutton.h"

#if defined (STM32F4XX)                                                 // STM32F4xx Nucleo Board PC5
#define WPSBUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define WPSBUTTON_PERIPH               RCC_AHB1Periph_GPIOC
#define WPSBUTTON_PORT                 GPIOC                            // PC5
#define WPSBUTTON_PIN                  GPIO_Pin_5
#define WPSBUTTON_PRESSED              Bit_RESET                        // pressed if low

#elif defined (STM32F103)                                               // STM32F103 Mini Development Board PA6

#define WPSBUTTON_PERIPH_CLOCK_CMD     RCC_APB2PeriphClockCmd
#define WPSBUTTON_PERIPH               RCC_APB2Periph_GPIOA
#define WPSBUTTON_PORT                 GPIOA                            // PA7
#define WPSBUTTON_PIN                  GPIO_Pin_7
#define WPSBUTTON_PRESSED              Bit_RESET                        // pressed if low

#else
#error STM32 unknown
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize WPS button port
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
wpsbutton_init (void)
{
    GPIO_InitTypeDef gpio;

    GPIO_StructInit (&gpio);
    WPSBUTTON_PERIPH_CLOCK_CMD (WPSBUTTON_PERIPH, ENABLE);

    gpio.GPIO_Pin = WPSBUTTON_PIN;

#if defined (STM32F10X)
    gpio.GPIO_Mode = GPIO_Mode_IPU;                         // use pin as input with internal pull up
#elif defined (STM32F4XX)
    gpio.GPIO_Mode = GPIO_Mode_IN;                          // use pin as input with internal pull up
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
#endif

    GPIO_Init(WPSBUTTON_PORT, &gpio);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check if WPS button pressed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
wpsbutton_pressed (void)
{
    if (GPIO_ReadInputDataBit(WPSBUTTON_PORT, WPSBUTTON_PIN) == WPSBUTTON_PRESSED)
    {
        return 1;
    }
    return 0;
}
