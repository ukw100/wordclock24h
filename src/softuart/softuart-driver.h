/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * softuart-driver.h - SOFTWARE UART driver routines for STM32F4XX or STM32F10X
 *
 * THIS DRIVER IS CURRENTLY UNUSED AND UNTESTET!
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * example of usage:
 *
 * #define UART_PREFIX         log                      // prefix for all USART functions, e.g. log_uart_puts()
 * #define UART_GPIO_LETTER_TX B                        // GPIO port TX
 * #define UART_PIN_TX         10                       // GPIO pin TX
 * #define UART_GPIO_LETTER_RX B                        // GPIO port RX
 * #define UART_PIN_RX         11                       // GPIO pin RX
 *
 * #define UART_TXBUFLEN       64                       // ringbuffer size for SOFTUART TX
 * #define UART_RXBUFLEN       64                       // ringbuffer size for SOFTUART RX
 *
 * include "softuart-driver.h"                          // at least include this file
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (defined STM32F4XX)
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

#elif (defined STM32F10X)
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#endif

#include "softuart.h"
#include "io.h"

#if defined(STM32F4XX)

#define SOFTUART_PERIPH_CLOCK_CMD               RCC_AHB1PeriphClockCmd
#define SOFTUART_PERIPH_TX                      UART_CONCAT(RCC_AHB1Periph_GPIO, UART_GPIO_LETTER_TX)
#define SOFTUART_PERIPH_RX                      UART_CONCAT(RCC_AHB1Periph_GPIO, UART_GPIO_LETTER_RX)

#elif defined(STM32F10X)

#define SOFTUART_PERIPH_CLOCK_CMD               RCC_APB2PeriphClockCmd
#define SOFTUART_PERIPH_TX                      UART_CONCAT(RCC_APB2Periph_GPIO, UART_GPIO_LETTER_TX)
#define SOFTUART_PERIPH_RX                      UART_CONCAT(RCC_APB2Periph_GPIO, UART_GPIO_LETTER_RX)

#endif

#define UART_GPIO_TX                            UART_CONCAT(GPIO, UART_GPIO_LETTER_TX)
#define SOFTUART_TX_PIN                         UART_CONCAT(GPIO_Pin_, UART_PIN_NUMBER_TX)
#define SOFTUART_TX_PINSOURCE                   UART_CONCAT(GPIO_PinSource,  UART_PIN_NUMBER_TX)

#define UART_GPIO_RX                            UART_CONCAT(GPIO, UART_GPIO_LETTER_RX)
#define SOFTUART_RX_PIN                         UART_CONCAT(GPIO_Pin_, UART_PIN_NUMBER_RX)
#define SOFTUART_RX_PINSOURCE                   UART_CONCAT(GPIO_PinSource, UART_PIN_NUMBER_RX)

#define UART_PREFIX_INIT                        UART_CONCAT(UART_PREFIX, _uart_init)
#define UART_PREFIX_PUTC                        UART_CONCAT(UART_PREFIX, _uart_putc)
#define UART_PREFIX_PUTS                        UART_CONCAT(UART_PREFIX, _uart_puts)
#define UART_PREFIX_CHAR_AVAILABLE              UART_CONCAT(UART_PREFIX, _uart_char_available)
#define UART_PREFIX_GETC                        UART_CONCAT(UART_PREFIX, _uart_getc)
#define UART_PREFIX_POLL                        UART_CONCAT(UART_PREFIX, _uart_poll)
#define UART_PREFIX_RSIZE                       UART_CONCAT(UART_PREFIX, _uart_rsize)
#define UART_PREFIX_FLUSH                       UART_CONCAT(UART_PREFIX, _uart_flush)

#define RX_STATE_WAITING_FOR_STARBIT_PART_1     1
#define RX_STATE_WAITING_FOR_STARBIT_PART_2     2
#define RX_STATE_WAITING_FOR_STARBIT_PART_3     3
#define RX_STATE_WAITING_FOR_DATABIT_PART_1     4
#define RX_STATE_WAITING_FOR_DATABIT_PART_2     5
#define RX_STATE_WAITING_FOR_DATABIT_PART_3     6
#define RX_STATE_WAITING_FOR_STOPBIT_PART_1     7
#define RX_STATE_WAITING_FOR_STOPBIT_PART_2     8
#define RX_STATE_WAITING_FOR_STOPBIT_PART_3     9

#define SOFTUART_RX_DATA_BITS                   8               // 8 data bits
#define SOFTUART_TX_BITS                        10              // 1 startbit + 8 data bits + 1 stopbit

#define set_tx_pin_high()                       GPIO_SET_BIT(UART_GPIO_TX, SOFTUART_TX_PIN)
#define set_tx_pin_low()                        GPIO_RESET_BIT(UART_GPIO_TX, SOFTUART_TX_PIN)
#define rx_pin_status()                         (GPIO_ReadInputDataBit(UART_GPIO_RX, SOFTUART_RX_PIN) == Bit_SET)

static volatile uint8_t                         uart_txbuf[UART_TXBUFLEN];                  // tx ringbuffer
static volatile uint_fast16_t                   uart_txsize = 0;                            // tx size
static volatile uint8_t                         uart_rxbuf[UART_RXBUFLEN];                  // rx ringbuffer
static volatile uint_fast16_t                   uart_rxsize = 0;                            // rx size

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * timer definitions:
 *
 *      F_INTERRUPTS    = TIM_CLK / (TIM_PRESCALER + 1) / (TIM_PERIOD + 1)
 * <==> TIM_PRESCALER   = TIM_CLK / F_INTERRUPTS / (TIM_PERIOD + 1) - 1
 *
 * STM32F4x1:
 *      TIM_PERIOD      =    1 - 1 =   0
 *      TIM_PRESCALER   = 2917 - 1 = 2916
 *      F_INTERRUPTS    = 84000000 / 2917 / 1 = 28797: baudrate error: 0,01%
 * STM32F103:
 *      TIM_PERIOD      =    1 - 1 =   0
 *      TIM_PRESCALER   = 2500 - 1 = 2449
 *      F_INTERRUPTS    = 72000000 / 2500 / 1 = 28800: baudrate error: 0,00%
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (STM32F4XX)                                                     // STM32F4xx Nucleo Board PC13
#define TIM_CLK                 84000000L                                   // timer clock, 84MHz on STM32F401/411 Nucleo Board
#define TIM_PERIOD              0

#elif defined (STM32F103)
#define TIM_CLK                 72000000L                                   // timer clock, 72MHz on STM32F103
#define TIM_PERIOD              0

#else
#error STM32 unknown
#endif

#define TIM_PRESCALER           ((TIM_CLK / (baudrate * 3)) / (TIM_PERIOD + 1) - 1)


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize timer5
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
timer5_init (uint32_t baudrate)
{
    TIM_TimeBaseInitTypeDef     tim;
    NVIC_InitTypeDef            nvic;

    TIM_TimeBaseStructInit (&tim);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);

    tim.TIM_ClockDivision   = TIM_CKD_DIV1;
    tim.TIM_CounterMode     = TIM_CounterMode_Up;
    tim.TIM_Period          = TIM_PERIOD;
    tim.TIM_Prescaler       = TIM_PRESCALER;
    TIM_TimeBaseInit (TIM5, &tim);

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    nvic.NVIC_IRQChannel                    = TIM5_IRQn;
    nvic.NVIC_IRQChannelCmd                 = ENABLE;
    nvic.NVIC_IRQChannelPreemptionPriority  = 0x0F;
    nvic.NVIC_IRQChannelSubPriority         = 0x0F;
    NVIC_Init (&nvic);

    TIM_Cmd(TIM5, ENABLE);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * timer5 IRQ handler
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
extern void TIM5_IRQHandler (void);                                             // keep compiler happy

void
TIM5_IRQHandler (void)
{
    static uint_fast8_t     uart_txstart = 0;                                   // head
    static uint_fast8_t     uart_rxstop  = 0;                                   // tail

    static uint_fast8_t     tx_busy;
    static uint_fast8_t     tx_counter;
    static uint_fast8_t     tx_bits_left;
    static uint_fast16_t    tx_character;

    static uint_fast8_t     rx_state = RX_STATE_WAITING_FOR_STARBIT_PART_1;     // rx state
    static uint_fast8_t     rx_mask;                                            // rx mask: will be shifted every databit
    static uint_fast8_t     rx_data_bit;                                        // input bit
    static uint_fast8_t     rx_character;                                       // the character itself
    static uint_fast8_t     rx_bits_left;                                       // number of bits left to receive

    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    if (! tx_busy)
    {
        if (uart_txsize > 0)                                                    // tx buffer empty?
        {                                                                       // no
            uint8_t ch = uart_txbuf[uart_txstart++];                            // get character to send, increment offset

            if (uart_txstart == UART_TXBUFLEN)                                  // at end of tx buffer?
            {                                                                   // yes
                uart_txstart = 0;                                               // reset to beginning
            }

            uart_txsize--;                                                      // decrement size

            tx_counter      = 3;                                                // sample rate is 3 x 9600 = 28800
            tx_bits_left    = SOFTUART_TX_BITS;                                 // 1 startbit + 8 data bits + 1 stopbit
            tx_character    = (ch << 1) | 0x200;                                // add startbit (lsb = 0) and stopbit (msb = 1)
            tx_busy         = 1;                                                // invoke UART transmit
        }
    }

    if (tx_busy)
    {
        if (--tx_counter == 0)
        {
            if (tx_character & 0x01)
            {
                set_tx_pin_high();
            }
            else
            {
                set_tx_pin_low();
            }

            tx_character >>= 1;
            tx_counter = 3;

            if (--tx_bits_left == 0)
            {
                tx_busy = 0;
            }
        }
    }

    switch (rx_state)
    {
        case RX_STATE_WAITING_FOR_STARBIT_PART_1:
            if (rx_pin_status() == 0)                                       // test for start bit #1
            {
                rx_state = RX_STATE_WAITING_FOR_STARBIT_PART_2;
            }
            break;

        case RX_STATE_WAITING_FOR_STARBIT_PART_2:
            if (rx_pin_status() == 0)                                       // test for start bit #2
            {
                rx_state = RX_STATE_WAITING_FOR_STARBIT_PART_3;
            }
            else
            {                                                               // start bit too short
                rx_state = RX_STATE_WAITING_FOR_STARBIT_PART_1;             // start again
            }
            break;

        case RX_STATE_WAITING_FOR_STARBIT_PART_3:                           // ignore start bit #3
            rx_state        = RX_STATE_WAITING_FOR_DATABIT_PART_1;          // wait for data bit n #1
            rx_character    = 0;                                            // clear character value
            rx_bits_left    = SOFTUART_RX_DATA_BITS;                        // set number of bits we have to read
            rx_mask         = 1;                                            // set mask
            break;

        case RX_STATE_WAITING_FOR_DATABIT_PART_1:
            rx_state        = RX_STATE_WAITING_FOR_DATABIT_PART_2;          // wait for data bit n #2
            break;

        case RX_STATE_WAITING_FOR_DATABIT_PART_2:
            rx_data_bit = rx_pin_status();                                  // read rx pin
            rx_state    = RX_STATE_WAITING_FOR_DATABIT_PART_3;              // wait for data bit n #3
            break;

        case RX_STATE_WAITING_FOR_DATABIT_PART_3:                           // ignore data bit #3
            if (rx_data_bit)                                                // bit set?
            {                                                               // yes...
                rx_character |= rx_mask;                                    // store bit
            }

            rx_mask <<= 1;                                                  // shift mask
            rx_bits_left--;                                                 // decrement counter

            if (rx_bits_left)                                               // more to receive?
            {                                                               // yes...
                rx_state = RX_STATE_WAITING_FOR_DATABIT_PART_1;             // wait for data bit n+1 #1
            }
            else
            {                                                               // nothing more to receive
                rx_state = RX_STATE_WAITING_FOR_STOPBIT_PART_1;             // wait for stop bit #1
            }
            break;

        case RX_STATE_WAITING_FOR_STOPBIT_PART_1:
            rx_state = RX_STATE_WAITING_FOR_STOPBIT_PART_2;                 // wait for stop bit #2
            break;

        case RX_STATE_WAITING_FOR_STOPBIT_PART_2:
            rx_data_bit = rx_pin_status();                                  // read data bit

            if (rx_data_bit)                                                // stop bit must be 1
            {
                rx_state = RX_STATE_WAITING_FOR_STOPBIT_PART_3;             // wait for stop bit #3
            }
            else
            {                                                               // error: stopbit is not 1
                rx_state = RX_STATE_WAITING_FOR_STARBIT_PART_1;             // wait for next start bit
            }
            break;

        case RX_STATE_WAITING_FOR_STOPBIT_PART_3:                           // ignore stopbit #3
            if (uart_rxsize < UART_RXBUFLEN)                                // buffer full?
            {                                                               // no
                uart_rxbuf[uart_rxstop++] = rx_character;                   // store character

                if (uart_rxstop >= UART_RXBUFLEN)                           // at end of ringbuffer?
                {                                                           // yes
                    uart_rxstop = 0;                                        // reset to beginning
                }

                uart_rxsize++;                                              // increment used size
            }
            // NO break;
        default:
            rx_state = RX_STATE_WAITING_FOR_STARBIT_PART_1;                 // wait for next start bit
            break;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_init ()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void
UART_PREFIX_INIT (uint32_t baudrate)
{
    GPIO_InitTypeDef    gpio_tx;
    GPIO_InitTypeDef    gpio_rx;

    GPIO_StructInit (&gpio_tx);
    GPIO_StructInit (&gpio_rx);

    SOFTUART_PERIPH_CLOCK_CMD (SOFTUART_PERIPH_TX, ENABLE);     // enable clock for TX Port
    SOFTUART_PERIPH_CLOCK_CMD (SOFTUART_PERIPH_RX, ENABLE);     // enable clock for RX Port

    gpio_tx.GPIO_Pin    = SOFTUART_TX_PIN;
    gpio_tx.GPIO_Speed  = GPIO_Speed_50MHz;
#if defined (STM32F10X)
    gpio_tx.GPIO_Mode   = GPIO_Mode_Out_PP;
#elif defined (STM32F4XX)
    gpio_tx.GPIO_Mode   = GPIO_Mode_OUT;
    gpio_tx.GPIO_OType  = GPIO_OType_PP;
    gpio_tx.GPIO_PuPd   = GPIO_PuPd_NOPULL;
#endif
    GPIO_Init(UART_GPIO_TX, &gpio_tx);

    gpio_rx.GPIO_Pin    = SOFTUART_RX_PIN;
#if defined (STM32F10X)
   gpio_rx.GPIO_Mode    = GPIO_Mode_IPU;                        // enable internal pullup
#elif defined (STM32F4XX)
   gpio_rx.GPIO_Mode    = GPIO_Mode_IN;
   gpio_rx.GPIO_PuPd    = GPIO_PuPd_UP;                         // enable internal pullup
#endif

    GPIO_Init(UART_GPIO_RX, &gpio_rx);

    timer5_init (baudrate);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_putc ()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void
UART_PREFIX_PUTC (uint_fast8_t ch)
{
    static uint_fast8_t uart_txstop  = 0;                                       // tail

    while (uart_txsize >= UART_TXBUFLEN)                                        // buffer full?
    {                                                                           // yes
        ;                                                                       // wait
    }

    uart_txbuf[uart_txstop++] = ch;                                             // store character

    if (uart_txstop >= UART_TXBUFLEN)                                           // at end of ringbuffer?
    {                                                                           // yes
        uart_txstop = 0;                                                        // reset to beginning
    }

    __disable_irq();
    uart_txsize++;                                                              // increment used size
    __enable_irq();
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_puts ()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void
UART_PREFIX_PUTS (const char * s)
{
    uint_fast8_t ch;

    while ((ch = (uint_fast8_t) *s) != '\0')
    {
        UART_PREFIX_PUTC (ch);
        s++;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_char_available ()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
UART_PREFIX_CHAR_AVAILABLE (void)
{
    uint_fast8_t    rtc;

    if (uart_rxsize == 0)                                                       // rx buffer empty?
    {
        rtc = 0;
    }
    else
    {
        rtc = 1;
    }
    return rtc;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_getc ()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
UART_PREFIX_GETC (void)
{
    static uint_fast8_t  uart_rxstart = 0;                                      // head
    uint_fast8_t         ch;

    while (uart_rxsize == 0)                                                    // rx buffer empty?
    {                                                                           // yes, wait
        ;
    }

    ch = uart_rxbuf[uart_rxstart++];                                            // get character from ringbuffer

    if (uart_rxstart == UART_RXBUFLEN)                                          // at end of rx buffer?
    {                                                                           // yes
        uart_rxstart = 0;                                                       // reset to beginning
    }

    __disable_irq();
    uart_rxsize--;                                                              // decrement size
    __enable_irq();

    return (ch);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_poll()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
UART_PREFIX_POLL (uint_fast8_t * chp)
{
    static uint_fast8_t uart_rxstart = 0;                                       // head
    uint_fast8_t        ch;

    if (uart_rxsize == 0)                                                       // rx buffer empty?
    {                                                                           // yes, return 0
        return 0;
    }

    ch = uart_rxbuf[uart_rxstart++];                                            // get character from ringbuffer

    if (uart_rxstart == UART_RXBUFLEN)                                          // at end of rx buffer?
    {                                                                           // yes
        uart_rxstart = 0;                                                       // reset to beginning
    }

    __disable_irq();
    uart_rxsize--;                                                              // decrement size
    __enable_irq();

    *chp = ch;
    return 1;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_rsize()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast16_t
UART_PREFIX_RSIZE (void)
{
    return uart_rxsize;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * uart_flush ()
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void
UART_PREFIX_FLUSH ()
{
    while (uart_txsize > 0)                                                     // tx buffer empty?
    {
        ;                                                                       // no, wait
    }
}
