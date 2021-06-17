/*-------------------------------------------------------------------------------------------------------------------------------------------
 * adc-config.h - adc pin configuration
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

#if defined (STM32F4XX)

#define ADC_NUMBER          ADC1
#define ADC_PORT            GPIOC
#define ADC_PIN             GPIO_Pin_4
#define ADC_ADC_CLOCK_CMD   RCC_APB2PeriphClockCmd
#define ADC_ADC_CLOCK       RCC_APB2Periph_ADC1
#define ADC_GPIO_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define ADC_GPIO_CLOCK      RCC_AHB1Periph_GPIOC
#define ADC_CHANNEL         ADC_Channel_14

#elif defined (STM32F10X)

#define ADC_NUMBER          ADC1
#define ADC_PORT            GPIOA
#define ADC_PIN             GPIO_Pin_5
#define ADC_ADC_CLOCK_CMD   RCC_APB2PeriphClockCmd
#define ADC_ADC_CLOCK       RCC_APB2Periph_ADC1
#define ADC_GPIO_CLOCK_CMD  RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLOCK      (RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO)
#define ADC_CHANNEL         ADC_Channel_5

#endif


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * maximum ADC frequency: 36MHz
 * base frequency:        84MHz (APB2)
 * possible prescaler:    ADC_Prescaler_Div2 - frequency:  42 MHz
 *                        ADC_Prescaler_Div4 - frequency:  21 MHz
 *                        ADC_Prescaler_Div6 - frequency:  14 MHz
 *                        ADC_Prescaler_Div8 - frequency: 10.5 MHz
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define ADC_PRESCALER   ADC_Prescaler_Div4

#endif // ADC_CONFIG_H
