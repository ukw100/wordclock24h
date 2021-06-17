/*-------------------------------------------------------------------------------------------------------------------------------------------
 * i2c.c - I2C routines using I2C3 on STM32F4x1 Nucleo Board, I2C1 on STM32F103
 *
 * Ports/Pins:
 *
 *  +------+------------------+---------------+
 *  |      | STM32F4x1 Nucleo | STM32F103C8T6 |
 *  +------+------------------+---------------+
 *  | SCL  |       PA8        |      PB6      |
 *  | SDA  |       PC9        |      PB7      |
 *  +------+------------------+---------------+
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "i2c.h"
#include "delay.h"
#include "io.h"
#include "log.h"

#define   I2C_TIMEOUT   5                                // timeout: 5 msec

static uint32_t         i2c_clockspeed;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * init i2c bus
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
i2c_init_i2c (void)
{
    I2C_InitTypeDef  i2c;

    I2C_StructInit (&i2c);

    I2C_DeInit(I2C_CHANNEL);

    i2c.I2C_Mode                  = I2C_Mode_I2C;
    i2c.I2C_DutyCycle             = I2C_DutyCycle_2;
    i2c.I2C_OwnAddress1           = 0x00;
    i2c.I2C_Ack                   = I2C_Ack_Enable;
    i2c.I2C_AcknowledgedAddress   = I2C_AcknowledgedAddress_7bit;
    i2c.I2C_ClockSpeed            = i2c_clockspeed;

    I2C_Init (I2C_CHANNEL, &i2c);
    I2C_Cmd (I2C_CHANNEL, ENABLE);

    // fm 2017-10-18 +
    // I2C_ClearFlag(I2C_Module, I2C_FLAG_AF);
    // I2C_ClearFlag(I2C_Module, I2C_FLAG_ARLO);
    // I2C_ClearFlag(I2C_Module, I2C_FLAG_BERR);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * handle timeout
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
i2c_handle_timeout (void)
{
    I2C_GenerateSTOP (I2C_CHANNEL, ENABLE);
    I2C_SoftwareResetCmd (I2C_CHANNEL, ENABLE);
    I2C_SoftwareResetCmd (I2C_CHANNEL, DISABLE);

    I2C_DeInit (I2C_CHANNEL);
    i2c_init_i2c ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * wait for flags
 *
 * return values:
 * == 1  OK, got flag
 * == 0  timeout
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
i2c_wait_for_flags (uint32_t flag1, uint32_t flag2)
{
    uint32_t  timeout = I2C_TIMEOUT;

    if (flag2)
    {
        while ((!I2C_GetFlagStatus(I2C_CHANNEL, flag1)) || (!I2C_GetFlagStatus(I2C_CHANNEL, flag2)))
        {
            if (timeout > 0)
            {
                delay_msec(1);
                timeout--;
            }
            else
            {
                i2c_handle_timeout ();
                return 0;
            }
        }
    }
    else
    {
        while (! I2C_GetFlagStatus(I2C_CHANNEL, flag1))
        {
            if (timeout > 0)
            {
                delay_msec(1);
                timeout--;
            }
            else
            {
                i2c_handle_timeout ();
                return 0;
            }
        }
    }

    return 1;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * send address
 *
 * return values:
 * ==  0 I2C_OK
 *  <  0 Error
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static int_fast16_t
i2c_send_address (uint_fast8_t slave_addr, uint_fast16_t addr, uint_fast8_t is_16_bit_addr, uint_fast8_t disable_ack)
{
    I2C_GenerateSTART(I2C_CHANNEL, ENABLE);

    if (! i2c_wait_for_flags (I2C_FLAG_SB, 0))
    {
        return I2C_ERROR_NO_FLAG_SB;
    }

    if (disable_ack)
    {
        I2C_AcknowledgeConfig(I2C_CHANNEL, DISABLE);                               // ACK disable
    }
    else
    {
        I2C_AcknowledgeConfig(I2C_CHANNEL, ENABLE);                                // ACK enable
    }


    I2C_Send7bitAddress(I2C_CHANNEL, slave_addr, I2C_Direction_Transmitter);        // send slave address (transmitter)

    if (! i2c_wait_for_flags (I2C_FLAG_ADDR, 0))
    {
        return I2C_ERROR_NO_FLAG_ADDR;
    }

    I2C_CHANNEL->SR2;                                                              // clear ADDR-Flag

    if (! i2c_wait_for_flags (I2C_FLAG_TXE, 0))
    {
        return I2C_ERROR_NO_FLAG_TXE;
    }

    if (is_16_bit_addr)
    {
        uint8_t addrh;
        uint8_t addrl;

        addrh = (addr >> 8);
        addrl = (addr & 0xFF);

        I2C_SendData (I2C_CHANNEL, addrh);                                         // send upper byte of address
        I2C_SendData (I2C_CHANNEL, addrl);                                         // send lower byte of address
    }
    else
    {
        I2C_SendData(I2C_CHANNEL, addr);                                           // send address
    }

    return I2C_OK;
}

#if defined (STM32F4XX)

#define PERIPH_SCL_PORT     RCC_AHB1Periph_GPIOA
#define SCL_PORT            GPIOA                                   // SCL PA8
#define SCL_PIN             GPIO_Pin_8
#define SCL_PIN_SOURCE      GPIO_PinSource8

#define PERIPH_SDA_PORT     RCC_AHB1Periph_GPIOC
#define SDA_PORT            GPIOC                                   // SDA PC9
#define SDA_PIN             GPIO_Pin_9
#define SDA_PIN_SOURCE      GPIO_PinSource9

#elif defined (STM32F10X)

#define PERIPH_SCL_SDA_PORT RCC_APB2Periph_GPIOB
#define SCL_PORT            GPIOB                                   // SCL PB6
#define SCL_PIN             GPIO_Pin_6

#define SDA_PORT            GPIOB                                   // SDA PB7
#define SDA_PIN             GPIO_Pin_7

#else
#error STM32 undefined
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * reset I2C bus
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
i2c_reset_bus (void)
{
    GPIO_InitTypeDef    gpio;
    uint_fast8_t        idx;

#if defined (STM32F4XX)
    RCC_AHB1PeriphClockCmd(PERIPH_SCL_PORT, ENABLE);           // for SCL
    RCC_AHB1PeriphClockCmd(PERIPH_SDA_PORT, ENABLE);           // for SDA

    GPIO_StructInit (&gpio);

    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin   = SCL_PIN;                                      // SCL pin PA8
    GPIO_Init(SCL_PORT, &gpio);

    GPIO_StructInit (&gpio);

    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin   = SDA_PIN;                                      // SDA pin PC9

    GPIO_Init(SDA_PORT, &gpio);

#elif defined (STM32F10X)

    RCC_APB2PeriphClockCmd(PERIPH_SCL_SDA_PORT, ENABLE);            // for SCL & SDA

    GPIO_StructInit (&gpio);
    gpio.GPIO_Pin   = SCL_PIN;                                      // SCL pin PB6
    gpio.GPIO_Mode  = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SCL_PORT, &gpio);

    GPIO_StructInit (&gpio);
    gpio.GPIO_Pin   = SDA_PIN;                                      // SDA pin PB7
    gpio.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SDA_PORT, &gpio);

#else
#error STM32 undefined
#endif

    if (GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN) == Bit_RESET)      // SDA low?
    {                                                               // yes...
        GPIO_RESET_BIT(SDA_PORT, SCL_PIN);                          // try to release it here....
        delay_msec (1);
    }

    for (idx = 0; idx < 9; idx++)
    {
        GPIO_SET_BIT(SCL_PORT, SCL_PIN);
        delay_msec (1);

        if (GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN) == Bit_SET)    // look for SDA = HIGH
        {                                                           // step 2!
            break;
        }

        GPIO_RESET_BIT(SCL_PORT, SCL_PIN);
        delay_msec (1);
    }

    GPIO_SET_BIT(SCL_PORT, SCL_PIN);
    delay_msec (1);

    if (GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN) == Bit_RESET)      // SDA still low
    {                                                               // yes...
        log_message ("error: cannot reset I2C bus");
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize I2C
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
i2c_init (uint32_t clockspeed)
{
    static uint32_t     last_clockspeed;
    GPIO_InitTypeDef    gpio;

    if (last_clockspeed == clockspeed)                              // if already called with same clockspeed, return
    {
        return;
    }

    last_clockspeed = clockspeed;

    I2C_DeInit(I2C_CHANNEL);

    i2c_reset_bus ();                                               // if 2i2c bus hangs, reset it by toggling SCK 9 times

    GPIO_StructInit (&gpio);

#if defined (STM32F4XX)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
    RCC_AHB1PeriphClockCmd(PERIPH_SCL_PORT, ENABLE);                // for SCL
    RCC_AHB1PeriphClockCmd(PERIPH_SDA_PORT, ENABLE);                // for SDA

    // I2C reset
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, DISABLE);

    GPIO_PinAFConfig(SCL_PORT, SCL_PIN_SOURCE, GPIO_AF_I2C3);       // SCL: PA8
    GPIO_PinAFConfig(SDA_PORT, SDA_PIN_SOURCE, GPIO_AF_I2C3);       // SDA: PC9

    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_OD;                                // OpenDrain!
    gpio.GPIO_PuPd  = GPIO_PuPd_UP; // GPIO_PuPd_NOPULL;

    gpio.GPIO_Pin = SCL_PIN;                                        // SCL pin
    GPIO_Init(SCL_PORT, &gpio);

    gpio.GPIO_Pin = SDA_PIN;                                        // SDA pin
    GPIO_Init(SDA_PORT, &gpio);

#elif defined (STM32F10X)

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(PERIPH_SCL_SDA_PORT, ENABLE);            // for SCL & SDA

    // I2C reset
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    gpio.GPIO_Pin   = SCL_PIN | SDA_PIN;                            // SCL=PB6, SDA=PB7
    gpio.GPIO_Mode  = GPIO_Mode_AF_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCL_PORT, &gpio);

#else
#error STM32 undefined
#endif

    i2c_clockspeed = clockspeed;
    i2c_init_i2c ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read one or more bytes
 *
 * return values:
 * ==  0 OK
 *  <  0 Error
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
int_fast16_t
i2c_read (uint_fast8_t slave_addr, uint_fast16_t addr, uint_fast8_t is_16_bit_addr, uint8_t * data, uint_fast16_t cnt)
{
    uint_fast16_t   n;
    int_fast16_t    rtc;

    if ((rtc = i2c_send_address (slave_addr, addr, is_16_bit_addr, cnt == 1 ? 1 : 0)) != I2C_OK)
    {
        return rtc;
    }

    if (! i2c_wait_for_flags (I2C_FLAG_TXE, I2C_FLAG_BTF))
    {
        return I2C_ERROR_NO_TXE_OR_BTF;
    }

    I2C_GenerateSTART(I2C_CHANNEL, ENABLE);                                        // start sequence

    if (! i2c_wait_for_flags (I2C_FLAG_SB, 0))
    {
        return I2C_ERROR_NO_FLAG_SB2;
    }

    I2C_Send7bitAddress(I2C_CHANNEL, slave_addr, I2C_Direction_Receiver);          // send slave address (receiver)

    if (! i2c_wait_for_flags (I2C_FLAG_ADDR, 0))
    {
        return I2C_ERROR_NO_FLAG_ADDR2;
    }

    I2C_CHANNEL->SR2;                                                              // clear ADDR flag

    for (n = 0; n < cnt; n++)                                               // read all data
    {
        if (n + 1 == cnt)
        {
            I2C_AcknowledgeConfig(I2C_CHANNEL, DISABLE);                           // ACK disable
            I2C_GenerateSTOP(I2C_CHANNEL, ENABLE);                                 // stop sequence

            while (I2C_GetFlagStatus(I2C_CHANNEL, I2C_FLAG_BUSY))
            {
                ;
            }
        }

        if (! i2c_wait_for_flags (I2C_FLAG_RXNE, 0))
        {
            return I2C_ERROR_NO_FLAG_RXNE;
        }

        data[n] = I2C_ReceiveData(I2C_CHANNEL);                                    // read data
    }

    I2C_AcknowledgeConfig(I2C_CHANNEL, ENABLE);                                    // ACK enable

    return I2C_OK;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write one or more bytes
 *
 * return values:
 * ==  0 I2C_OK
 *  <  0 Error
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
int_fast16_t
i2c_write (uint_fast8_t slave_addr, uint_fast16_t addr, uint_fast8_t is_16_bit_addr, uint8_t * data, uint_fast16_t cnt)
{
    uint8_t         value;
    uint_fast16_t   n;
    int_fast16_t    rtc;

    if ((rtc = i2c_send_address (slave_addr, addr, is_16_bit_addr, 0)) != I2C_OK)
    {
        return rtc;
    }

    if (! i2c_wait_for_flags (I2C_FLAG_TXE, 0))
    {
        return I2C_ERROR_NO_FLAG_TXE;
    }

    for (n = 0; n < cnt; n++)                                               // send all data
    {
        value = *data++;                                                    // read data from buffer

        I2C_SendData(I2C_CHANNEL, value);                                   // send data

        if (! i2c_wait_for_flags (I2C_FLAG_TXE, I2C_FLAG_BTF))
        {
            return I2C_ERROR_NO_TXE_OR_BTF;
        }
    }

    I2C_GenerateSTOP(I2C_CHANNEL, ENABLE);                                  // stop sequence

    while (I2C_GetFlagStatus(I2C_CHANNEL, I2C_FLAG_BUSY))
    {
        ;
    }

    return I2C_OK;
}
