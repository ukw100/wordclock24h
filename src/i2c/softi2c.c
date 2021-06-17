#if 0 // yet not used
/*-------------------------------------------------------------------------------------------------------------------------------------------
 * softi2c.c - soft I2C routines
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
 * Protocol:
 *
 * ----------------------------------------------------------------------------------------------------------------------------
 * Writing one byte of data for a register:
 *
 *   Master   ST|Device Address[7:1]|W     Register Address[7:0]    Data[7:0]    SP
 *   Slave                              AK                       AK           AK
 * -----------------------------------------------------------------------------------------------------------------------------
 * Writing multiple bytes of data for a register:
 *
 *   Master   ST|Device Address[7:1]|W     Register Address[7:0]    Data[7:0]    Data[7:0]    SP
 *   Slave                              AK                       AK           AK           AK
 * -----------------------------------------------------------------------------------------------------------------------------
 * Reading one byte of data for a register:
 *
 *   Master   ST|Device Address[7:1]|W     Register Address[7:0]    SR|Device Address[7:1]|R              NAK|SP
 *   Slave                              AK                       AK                          AK|Data[7:0]
 * -----------------------------------------------------------------------------------------------------------------------------
 * Reading multiple bytes of data for a register:
 *
 *   Master   ST|Device Address[7:1]|W     Register Address[7:0]    SR|Device Address[7:1]|R              AK           NAK|SP
 *   Slave                              AK                       AK                          AK|Data[7:0]    Data[7:0]
 * -----------------------------------------------------------------------------------------------------------------------------
 *   ST  = Start condition
 *   SP  = Stop condition
 *   SR  = Repeated start condition
 *   AK  = ACK
 *   NAK = NACK
 *   W   = Write == 0
 *   R   = Read  == 1
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include "softi2c.h"
#include "delay.h"

#define NACK		    1
#define ACK			    0
#define I2C_TIMEOUT     5                                // timeout: 5 msec
#define FOR_READ        1
#define FOR_WRITE       0

#if defined (STM32F4XX)

#define SCL_PERIPH_CMD  RCC_AHB1PeriphClockCmd
#define SDA_PERIPH_CMD  RCC_AHB1PeriphClockCmd
#define SCL_PERIPH      RCC_AHB1Periph_GPIOA
#define SDA_PERIPH      RCC_AHB1Periph_GPIOC
#define SCL_PORT        GPIOA
#define SCL_PIN         GPIO_Pin_8
#define SDA_PORT        GPIOC
#define SDA_PIN         GPIO_Pin_9

#elif defined (STM32F10X)

#define SCL_PERIPH_CMD  RCC_APB2PeriphClockCmd
#define SDA_PERIPH_CMD  RCC_APB2PeriphClockCmd
#define SCL_PERIPH      RCC_APB2Periph_GPIOB
#define SDA_PERIPH      RCC_APB2Periph_GPIOB
#define SCL_PORT        GPIOB
#define SCL_PIN         GPIO_Pin_6
#define SDA_PORT        GPIOB
#define SDA_PIN         GPIO_Pin_7

#else
#error STM32 undefined
#endif

static int			started		= 0;

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * delay of 5 usec
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
softi2c_delay (void)
{
    delay_usec (5);
}

static uint_fast8_t
read_SCL (void)
{
    return GPIO_ReadInputDataBit(SCL_PORT, SCL_PIN);
}

static uint_fast8_t
read_SDA (void)
{
    return GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN);
}

static void
set_SDA (void)
{
    GPIO_SET_BIT(SDA_PORT, SDA_PIN;
}

static void
clear_SDA (void)
{
    GPIO_RESET_BIT(SDA_PORT, SDA_PIN);
}

static void
set_SCL (void)
{
    GPIO_SET_BIT(SCL_PORT, SCL_PIN);
}

static void
clear_SCL (void)
{
    GPIO_RESET_BIT(SCL_PORT, SCL_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * check if slave stretches the clock
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static unsigned int
softi2c_clock_stretching (void)
{
    uint_fast8_t   timeout = 10 * I2C_TIMEOUT;                                  // timeout is 5 msec, raster is 100 usec

	while (read_SCL () == 0)
	{
        delay_usec (100);

		if (timeout == 0)
		{
			return 0;
		}
		timeout--;
	}

	return 1;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * send start condition
 *
 * return values:
 *   ACK    - okay
 *  NACK    - error: arbitration lost, another device pulls down SDA
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_start_cond (void)
{
	uint_fast8_t rtc = ACK;

	if (started)
	{										// if started, do a restart condition
		set_SDA ();		    				// set SDA to 1
		softi2c_delay ();
		set_SCL ();
		softi2c_clock_stretching ();
		softi2c_delay ();					// repeated start setup time, minimum 4.7us
	}

	if (read_SDA () == 0)
	{
		rtc = NACK;                         // arbitration lost
	}

	clear_SDA ();							// SCL is high, set SDA from 1 to 0.
	softi2c_delay ();
	clear_SCL ();

	started = 1;

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * send stop condition
 *
 * return values:
 *   ACK    - okay
 *  NACK    - error: arbitration lost, another device pulls down SDA
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_stop_cond (void)
{
    uint_fast8_t rtc = ACK;

	clear_SDA ();
	softi2c_delay ();
	set_SCL ();
	softi2c_clock_stretching ();

	softi2c_delay();						// stop bit setup time, minimum 4us
	set_SDA();								// SCL is high, set SDA from 0 to 1
	softi2c_delay();

	if (read_SDA () == 0)
	{
		rtc = NACK;                         // arbitration lost
	}

	started = 0;

	return rtc;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write a bit
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_write_bit (unsigned int bit)
{
    uint_fast8_t    rtc = ACK;

	if (bit)
	{
		set_SDA();
	}
	else
	{
		clear_SDA();
	}

	softi2c_delay();				// SDA change propagation delay
	set_SCL();						// set SCL high to indicate a new valid SDA value is available
	softi2c_delay();				// wait for SDA value to be read by slave, minimum of 4us for standard mode
	softi2c_clock_stretching ();

	if (bit && (read_SDA() == 0))	// SCL is high, now data is valid
	{								// if SDA is high, check that nobody else is driving SDA
		rtc = NACK;                 // arbitration lost
	}

	clear_SCL();					// clear the SCL to low in preparation for next change
	return rtc;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read a bit
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static int
softi2c_read_bit(void)
{
	int bit;

	set_SDA();						// Let the slave drive data
	softi2c_delay();				// Wait for SDA value to be written by slave, minimum of 4us for standard mode
	set_SCL();						// Set SCL high to indicate a new valid SDA value is available
	softi2c_clock_stretching ();
	softi2c_delay();				// Wait for SDA value to be written by slave, minimum of 4us for standard mode
	bit = read_SDA();				// SCL is high, read out bit
	clear_SCL();					// Set SCL low in preparation for next operation

	return bit;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write a byte
 *
 * Return values:
 *  ACK = write okay
 * NACK = write failed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_write_byte (uint_fast8_t send_start, uint_fast8_t send_stop, uint_fast8_t byte)
{
	uint_fast8_t    bit;
	uint_fast8_t    nack;
    uint_fast8_t    rtc = ACK;

	if (send_start)
	{
		if (softi2c_start_cond () != ACK)
        {
            return NACK;                                    // arbitration lost
        }
	}

	for (bit = 0; bit < 8; ++bit)
	{
		if (softi2c_write_bit ((byte & 0x80) != 0) != ACK)
        {
            rtc = NACK;
        }

		byte <<= 1;
	}

	nack = softi2c_read_bit();

	if (nack)
	{
		rtc = NACK;
	}

	if (send_stop)
	{
		if (softi2c_stop_cond() != ACK)
        {
            rtc = NACK;
        }
	}

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read a byte
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_read_byte (uint_fast8_t send_nack, uint_fast8_t send_stop, uint_fast8_t * bytep)
{
	uint_fast8_t	byte = 0;
	uint_fast8_t    bit;
    uint_fast8_t    rtc = ACK;

	for (bit = 0; bit < 8; ++bit)
	{
		byte = (byte << 1) | softi2c_read_bit();
	}

	if (send_nack)
	{
		if (softi2c_write_bit (NACK) != ACK)
        {
            rtc = NACK;
        }
	}
	else
	{
		if (softi2c_write_bit (ACK) != ACK)
        {
            rtc = NACK;
        }
	}

	if (send_stop)
	{
		if (softi2c_stop_cond() != ACK)
        {
            rtc = NACK;
        }
	}

    if (rtc == ACK)
    {
        *bytep = byte;
    }

	return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * send slave address
 *
 * Parameters:
 *  slave_addr = I2C address of device << 1 (already shifted left by 1)
 *  rw         = read/write, values: FOR_READ or FOR_WRITE
 *
 * Return values:
 *   ACK = write okay
 *  NACK = write failed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_send_slave_address (uint_fast8_t slave_addr, uint_fast8_t rw)
{
    uint_fast8_t    rtc;

    if (rw == FOR_READ)
    {
        slave_addr |= 0x01;
    }

    rtc = softi2c_write_byte (1, 0, slave_addr);

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * send register address
 *
 * Return values:
 *  ACK = write okay
 * NACK = write failed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static uint_fast8_t
softi2c_send_register_address (uint_fast16_t addr, uint_fast8_t is_16_bit_addr)
{
    uint_fast8_t    rtc;

    if (is_16_bit_addr)
    {
        uint8_t addrh;
        uint8_t addrl;

        addrh = (addr >> 8);
        addrl = (addr & 0xFF);

        if ((rtc = softi2c_write_byte (0, 0, addrh)) == ACK)                        // send upper byte of address
        {
            rtc = softi2c_write_byte (0, 0, addrl);                                 // send lower byte of address
        }
    }
    else
    {
        rtc = softi2c_write_byte (0, 0, addr);
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * clear I2C bus
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
soft_i2c_clear_bus (void)
{
    uint_fast8_t    idx;

    set_SDA();
    set_SCL();
    softi2c_delay();

    for (idx = 0; idx < 9; idx++)
    {
        clear_SCL();
        softi2c_delay();
        set_SCL();
        softi2c_delay();
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize I2C
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
softi2c_init (void)
{
    GPIO_InitTypeDef    gpio;

    GPIO_StructInit (&gpio);

    SCL_PERIPH_CMD(SCL_PERIPH, ENABLE);                         // for SCL
    SDA_PERIPH_CMD(SDA_PERIPH, ENABLE);                         // for SDA

#if defined (STM32F4XX)

    gpio.GPIO_Pin   = SCL_PIN;                                  // SCL pin
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;

#elif defined (STM32F10X)

    gpio.GPIO_Pin   = SCL_PIN;                                  // SCL Pin
    gpio.GPIO_Mode  = GPIO_Mode_Out_OD;                         // OpenDrain

#else
#error STM32 undefined
#endif

    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCL_PORT, &gpio);                                 // SCL Port
    gpio.GPIO_Pin   = SDA_PIN;                                  // SDA Pin
    GPIO_Init(SDA_PORT, &gpio);                                 // SDA Port

    soft_i2c_clear_bus ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read one or more bytes
 *
 * return values:
 *   I2C_OK    - okay
 *   I2C_ERROR - error
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
int_fast16_t
softi2c_read (uint_fast8_t slave_addr, uint_fast16_t addr, uint_fast8_t is_16_bit_addr, uint8_t * data, uint_fast16_t cnt)
{
    uint_fast16_t   n;
    uint_fast8_t    value;
    uint_fast8_t    send_nack = 0;
    uint_fast8_t    send_stop = 0;
    uint_fast8_t    rtc = I2C_OK;

    if (softi2c_send_slave_address (slave_addr, FOR_WRITE) != ACK ||
        softi2c_send_register_address (addr, is_16_bit_addr) != ACK ||
        softi2c_send_slave_address (slave_addr, FOR_READ) != ACK)
    {
        return I2C_ERROR;
    }

    for (n = 0; n < cnt; n++)                                               // read all data
    {
        if (n + 1 == cnt)
        {
            send_nack = 1;
            send_stop = 1;
        }

        if (softi2c_read_byte (send_nack, send_stop, &value) != ACK)
        {
            rtc = I2C_ERROR;
            break;
        }
        else
        {
            data[n] = value;
        }
    }

    return rtc;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write one or more bytes
 *
 * return values:
 *   I2C_OK    - okay
 *   I2C_ERROR - error
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
int_fast16_t
softi2c_write (uint_fast8_t slave_addr, uint_fast16_t addr, uint_fast8_t is_16_bit_addr, uint8_t * data, uint_fast16_t cnt)
{
    uint8_t         value;
    uint_fast8_t    send_stop = 0;
    uint_fast16_t   n;
    uint_fast8_t    rtc = I2C_OK;

    if (softi2c_send_slave_address (slave_addr, FOR_WRITE) != ACK ||
        softi2c_send_register_address (addr, is_16_bit_addr) != ACK)
    {
        return I2C_ERROR;
    }

    for (n = 0; n < cnt; n++)                                               // send all data
    {
        if (n + 1 == cnt)
        {
            send_stop = 1;
        }

        value = *data++;                                                    // read data from buffer

        if (softi2c_write_byte (0, send_stop, value) != ACK)
        {
            rtc = I2C_ERROR;
            break;
        }
    }

    return rtc;
}
#endif // 0
