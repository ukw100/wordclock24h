/*-------------------------------------------------------------------------------------------------------------------------------------------
 * adc.h - adc functions
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef ADC_H
#define ADC_H

#ifdef STM32F10X
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#elif defined STM32F4XX
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#endif

typedef struct
{
    uint_fast8_t        is_up;
} ADC_GLOBALS;

extern ADC_GLOBALS      gadc;

extern void             adc_init (void);
extern void             adc_start_single_conversion (void);
extern uint_fast8_t     adc_poll_conversion_value (uint16_t *);

#endif // ADC_H
