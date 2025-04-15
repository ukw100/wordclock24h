/*-------------------------------------------------------------------------------------------------------------------------------------------
 * wpsbutton.c - read WPS button of STM32F4xx Nucleo and STM32F103
 *
 * Copyright (c) 2017-2024 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "wpsbutton.h"
#include "io.h"

#if defined (BLACK_BOARD)                                               // STM32F407VE Black Board PE3
#define WPSBUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define WPSBUTTON_PERIPH               RCC_AHB1Periph_GPIOE
#define WPSBUTTON_PORT                 GPIOE                            // PE3
#define WPSBUTTON_PIN                  GPIO_Pin_3

#elif defined (NUCLEO_BOARD)                                            // STM32F4xx Nucleo Board PC5
#define WPSBUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define WPSBUTTON_PERIPH               RCC_AHB1Periph_GPIOC
#define WPSBUTTON_PORT                 GPIOC                            // PC5
#define WPSBUTTON_PIN                  GPIO_Pin_5

#elif defined (BLACKPILL_BOARD)                                         // STM32F4x1 BlackPill Board PB5
#define WPSBUTTON_PERIPH_CLOCK_CMD     RCC_AHB1PeriphClockCmd
#define WPSBUTTON_PERIPH               RCC_AHB1Periph_GPIOA
#define WPSBUTTON_PORT                 GPIOA                            // PA7
#define WPSBUTTON_PIN                  GPIO_Pin_7

#elif defined (BLUEPILL_BOARD)                                          // STM32F103 BluePill Board PA7
#define WPSBUTTON_PERIPH_CLOCK_CMD     RCC_APB2PeriphClockCmd
#define WPSBUTTON_PERIPH               RCC_APB2Periph_GPIOA
#define WPSBUTTON_PORT                 GPIOA                            // PA7
#define WPSBUTTON_PIN                  GPIO_Pin_7

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
    WPSBUTTON_PERIPH_CLOCK_CMD (WPSBUTTON_PERIPH, ENABLE);
    GPIO_SET_PIN_IN_UP(WPSBUTTON_PORT, WPSBUTTON_PIN, GPIO_Speed_2MHz);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check if WPS button pressed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
wpsbutton_pressed (void)
{
    if (GPIO_GET_BIT(WPSBUTTON_PORT, WPSBUTTON_PIN) == Bit_RESET)
    {
        return 1;
    }
    return 0;
}
