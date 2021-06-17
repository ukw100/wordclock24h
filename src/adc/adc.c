/*-------------------------------------------------------------------------------------------------------------------------------------------
 * adc.c - adc functions
 *
 * Copyright (c) 2015-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "adc.h"
#include "adc-config.h"

ADC_GLOBALS             gadc;

static uint_fast8_t     conversion_started = 0;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize ADC (single conversion mode)
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
adc_init (void)
{
    static uint_fast8_t     already_called;

    if (!already_called)
    {
        GPIO_InitTypeDef        gpio;
        ADC_InitTypeDef         adc;
        GPIO_StructInit (&gpio);
        ADC_StructInit (&adc);

#if defined (STM32F10X)
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);
#endif

        ADC_GPIO_CLOCK_CMD (ADC_GPIO_CLOCK, ENABLE);

        gpio.GPIO_Pin   = ADC_PIN;                                                  // configure pin as Analog input

#if defined (STM32F4XX)

        gpio.GPIO_Mode  = GPIO_Mode_AN;
        gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;

#elif defined (STM32F10X)

        gpio.GPIO_Mode  = GPIO_Mode_AIN;

#endif

        GPIO_Init (ADC_PORT, &gpio);

        ADC_ADC_CLOCK_CMD (ADC_ADC_CLOCK, ENABLE);

#if defined (STM32F4XX)
        ADC_CommonInitTypeDef adc_common;

        ADC_CommonStructInit (&adc_common);

        // configure ADC:
        adc_common.ADC_Mode                 = ADC_Mode_Independent;
        adc_common.ADC_Prescaler            = ADC_PRESCALER;
        adc_common.ADC_DMAAccessMode        = ADC_DMAAccessMode_Disabled;
        adc_common.ADC_TwoSamplingDelay     = ADC_TwoSamplingDelay_5Cycles;
        ADC_CommonInit(&adc_common);

        adc.ADC_Resolution             = ADC_Resolution_12b;
        adc.ADC_ScanConvMode           = DISABLE;
        adc.ADC_ContinuousConvMode     = DISABLE;
        adc.ADC_ExternalTrigConvEdge   = ADC_ExternalTrigConvEdge_None;
        adc.ADC_DataAlign              = ADC_DataAlign_Right;
        adc.ADC_NbrOfConversion        = 1;

#elif defined (STM32F10X)
        adc.ADC_Mode                   = ADC_Mode_Independent;

        adc.ADC_ScanConvMode           = DISABLE;
        adc.ADC_ContinuousConvMode     = DISABLE;
        adc.ADC_ExternalTrigConv       = ADC_ExternalTrigConv_None;
        adc.ADC_DataAlign              = ADC_DataAlign_Right;
        adc.ADC_NbrOfChannel = 1;

#endif

        ADC_Init(ADC_NUMBER, &adc);

#if defined (STM32F4XX)

        ADC_RegularChannelConfig (ADC_NUMBER, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);     // configure conversion channel

#elif defined (STM32F10X)

        ADC_RegularChannelConfig (ADC_NUMBER, ADC_CHANNEL, 1, ADC_SampleTime_1Cycles5);    // configure conversion channel

#endif

        ADC_Cmd(ADC_NUMBER, ENABLE);

#if defined (STM32F10X)                                                                 // Self Calibration
        ADC_ResetCalibration(ADC_NUMBER);

        while (ADC_GetResetCalibrationStatus (ADC_NUMBER))
        {
            ;
        }

        ADC_StartCalibration (ADC_NUMBER);

        while (ADC_GetCalibrationStatus (ADC_NUMBER))
        {
            ;
        }
#endif

        gadc.is_up = 1;
    }

    already_called = 1;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * start conversion
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
adc_start_single_conversion (void)
{
#if defined (STM32F4XX)
    ADC_SoftwareStartConv (ADC_NUMBER);
#elif defined (STM32F10X)
    ADC_SoftwareStartConvCmd (ADC_NUMBER, ENABLE);
#endif
    conversion_started = 1;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * poll adc value
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
adc_poll_conversion_value (uint16_t * value_p)
{
    uint_fast8_t    rtc = 0;

    if (conversion_started && ADC_GetFlagStatus (ADC_NUMBER, ADC_FLAG_EOC) != RESET)       // conversion started and ready?
    {
        *value_p = ADC_GetConversionValue (ADC_NUMBER);
        conversion_started = 0;
        rtc = 1;
    }
    return rtc;
}
