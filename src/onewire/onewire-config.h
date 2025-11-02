/*-----------------------------------------------------------------------------------------------------------------------------------------------
 * onewire-config.h - onewire configuration
 *
 * Copyright (c) 2015-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-----------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef ONEWIRE_CONFIG_H
#define ONEWIRE_CONFIG_H

//-----------------------------------------------------------------------------------------------------------------------------------------------
// OneWire data pin:
//-----------------------------------------------------------------------------------------------------------------------------------------------

#if defined (BLACK_BOARD)
#  define ONE_WIRE_PIN              GPIO_Pin_3                  // STM32F407VE Black Board: PD3
#  define ONE_WIRE_PORT             GPIOD
#  define ONE_WIRE_CLK_CMD          RCC_AHB1PeriphClockCmd
#  define ONE_WIRE_CLK              RCC_AHB1Periph_GPIOD
#elif defined (NUCLEO_BOARD)
#  define ONE_WIRE_PIN              GPIO_Pin_2                  // STM32F4xx Nucleo Board: PD2
#  define ONE_WIRE_PORT             GPIOD
#  define ONE_WIRE_CLK_CMD          RCC_AHB1PeriphClockCmd
#  define ONE_WIRE_CLK              RCC_AHB1Periph_GPIOD
#elif defined (BLACKPILL_BOARD)
#  define ONE_WIRE_PIN              GPIO_Pin_5                  // STM32F401 BlackPill Board: PB5
#  define ONE_WIRE_PORT             GPIOB
#  define ONE_WIRE_CLK_CMD          RCC_AHB1PeriphClockCmd
#  define ONE_WIRE_CLK              RCC_AHB1Periph_GPIOB
#elif defined (BLUEPILL_BOARD)                                  // STM32F103 Mini Development Board: PB5
#  define ONE_WIRE_PIN              GPIO_Pin_5
#  define ONE_WIRE_PORT             GPIOB
#  define ONE_WIRE_CLK_CMD          RCC_APB2PeriphClockCmd
#  define ONE_WIRE_CLK              RCC_APB2Periph_GPIOB
#endif

#endif // ONEWIRE_CONFIG_H
