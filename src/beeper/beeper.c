/*-------------------------------------------------------------------------------------------------------------------------------------------
 * beeper.c - beeper routines
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
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
#include "beeper.h"

#if 0  // yet not used

#if defined (STM32F4XX)                                                 // STM32F4xx Nucleo Board PB1 - TIM3/CH4
#define BEEPER_PERIPH_GPIO_CLOCK_CMD    RCC_AHB1PeriphClockCmd
#define BEEPER_PERIPH_GPIO              RCC_AHB1Periph_GPIOB
#define BEEPER_GPIO                     GPIOB
#define BEEPER_PIN                      GPIO_Pin_1
#define BEEPER_PINSOURCE                GPIO_PinSource1
#define BEEPER_PERIPH_TIM_CMD           RCC_APB1PeriphClockCmd
#define BEEPER_PERIPH_TIM               RCC_APB1Periph_TIM3
#define BEEPER_TIM                      TIM3                            // STM32F4xx only
#define BEEPER_GPIO_AF_TIM              GPIO_AF_TIM3                    // STM32F4xx only
#define BEEPER_TIM_OC_INIT_CMD          TIM_OC4Init                     // OC4 = Channel 4
#define BEEPER_TIM_OC_PRELOAD_CONFIG    TIM_OC4PreloadConfig            // STM32F4xx only

#elif defined (STM32F103)                                               // STM32F103C8t6 PB1 - TIM3/CH4
#define BEEPER_PERIPH_GPIO_CLOCK_CMD    RCC_APB2PeriphClockCmd
#define BEEPER_PERIPH_GPIO              (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)
#define BEEPER_GPIO                     GPIOB
#define BEEPER_PIN                      GPIO_Pin_1
#define BEEPER_PERIPH_TIM_CMD           RCC_APB1PeriphClockCmd
#define BEEPER_PERIPH_TIM               RCC_APB1Periph_TIM3
#define BEEPER_TIM                      TIM3
#define BEEPER_TIM_OC_INIT_CMD          TIM_OC4Init                     // OC4 = Channel 4
#define BEEPER_TIM_OC_PRELOAD_CONFIG    TIM_OC4PreloadConfig            // STM32F4xx only?

#else
#error STM32 unknown
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize beeper
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
beeper_init (void)
{
    GPIO_InitTypeDef        gpio;
    TIM_TimeBaseInitTypeDef timebase;
    TIM_OCInitTypeDef       oc;
    int                     period      = 1000 - 1;                 // 1000 msec
    int                     prescaler   = 71;
    int                     pulse       = (period + 1) / 2;         // 500 mec

    BEEPER_PERIPH_GPIO_CLOCK_CMD (BEEPER_PERIPH_GPIO, ENABLE);      // enable clock for beeper gpio
    BEEPER_PERIPH_TIM_CMD (BEEPER_PERIPH_TIM, ENABLE);              // enable clock for timer

    GPIO_StructInit (&gpio);

    gpio.GPIO_Pin   = BEEPER_PIN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

#if defined (STM32F10X)
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;                              // alternate function
#elif defined (STM32F4XX)
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;                             // or GPIO_PuPd_UP ?
#endif

    GPIO_Init(BEEPER_GPIO, &gpio);

#if defined (STM32F4XX)                                             // STM32F4xx only: bind alternate function to pin
    GPIO_PinAFConfig(BEEPER_GPIO, BEEPER_PINSOURCE, BEEPER_GPIO_AF_TIM);
#endif

    timebase.TIM_ClockDivision    = TIM_CKD_DIV1;
    timebase.TIM_CounterMode      = TIM_CounterMode_Up;
    timebase.TIM_Period           = period;
    timebase.TIM_Prescaler        = prescaler;
    TIM_TimeBaseInit (BEEPER_TIM, &timebase);

    oc.TIM_OCMode         = TIM_OCMode_PWM1;
    oc.TIM_OCIdleState    = TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState   = TIM_OCNIdleState_Set;
    oc.TIM_OCPolarity     = TIM_OCPolarity_High;
    oc.TIM_OCNPolarity    = TIM_OCNPolarity_High;
    oc.TIM_OutputState    = TIM_OutputState_Enable;
    oc.TIM_OutputNState   = TIM_OutputNState_Disable;
    oc.TIM_Pulse          = pulse;                                      // 0 <= pulse <= period

    BEEPER_TIM_OC_INIT_CMD (BEEPER_TIM, &oc);
    BEEPER_TIM_OC_PRELOAD_CONFIG (BEEPER_TIM, TIM_OCPreload_Enable);    // activate preload of CCR1
    TIM_ARRPreloadConfig(BEEPER_TIM, ENABLE);                           // preload timer config

    beeper_off ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * beeper on
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
beeper_on (int pulse)
{
    TIM_Cmd(BEEPER_TIM, ENABLE);
    BEEPER_TIM->CCR1 = pulse;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set beeper pulse width
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
beeper_set_pulse (int pulse)
{
    BEEPER_TIM->CCR1 = pulse;                               // same as TIM_SetCompare4 (BEEPER_TIM, pulse);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * beeper off
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
beeper_off (void)
{
    TIM_Cmd(BEEPER_TIM, DISABLE);
}
#endif
