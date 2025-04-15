/*-------------------------------------------------------------------------------------------------------------------------------------------
 * io.h - I/O macros
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2015-2024 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 *
 * In older versions of stm32f4xx.h the following struct is defined:
 *
 * typedef struct
 * {
 *   __IO uint32_t MODER;
 *   __IO uint32_t OTYPER;
 *   __IO uint32_t OSPEEDR;
 *   __IO uint32_t PUPDR;
 *   __IO uint32_t IDR;
 *   __IO uint32_t ODR;
 *   __IO uint16_t BSRRL;
 *   __IO uint16_t BSRRH;
 *   __IO uint32_t LCKR;
 *   __IO uint32_t AFR[2];
 * } GPIO_TypeDef;
 *
 * The definitions of BSRRL and BSRRH are wrong. There should be a 32 bit register named BSRR.
 * The definition above prevents access to the actual 32-bit register BSRR.
 *
 * There are now two ways to correct this.
 * Instead of
 *
 *   __IO uint16_t BSRRL;
 *   __IO uint16_t BSRRH;
 *
 *  we write:
 *
 *  union
 *  {
 *     __IO uint32_t BSRR;
 *     struct
 *     {
 *       __IO uint16_t BSRRL;
 *       __IO uint16_t BSRRH;
 *     };
 *  };
 *
 *  The second possibility is a 32-bit access to the register pair BSRRL/BSRRH:
 *
 *   (__IO uint32_t *) &(PORTx->BSRRL)
 *
 *  But this hack runs into a type punning error if using -O2.
 *
 *  Solution, if we don't want to modify stm32f4xx.h:
 *
 *    1. We use a struct named GPIO_TypedefExt with the above union.
 *    2. We cast every PORTx into pointer to GPIO_TypedefExt.
 *
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef IO_H
#define IO_H

#if defined (STM32F4XX)
#include "stm32f4xx_gpio.h"

typedef struct
{
  __IO uint32_t MODER;          /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;         /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;        /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;          /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;            /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;            /*!< GPIO port output data register,        Address offset: 0x14      */
  union
  {
    __IO uint32_t BSRR;         /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    struct
    {
      __IO uint16_t BSRRL;      /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
      __IO uint16_t BSRRH;      /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
    };
  };
  __IO uint32_t LCKR;           /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];         /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDefExt;

#define GPIO_SET_MODE_OUT_OD(gp, pins, speed)       \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_OUT;                \
    gp.GPIO_OType   = GPIO_OType_OD;                \
    gp.GPIO_PuPd    = GPIO_PuPd_NOPULL;             \
} while (0)

#define GPIO_SET_MODE_OUT_PP(gp, pins, speed)       \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_OUT;                \
    gp.GPIO_OType   = GPIO_OType_PP;                \
    gp.GPIO_PuPd    = GPIO_PuPd_NOPULL;             \
} while (0)

#define GPIO_SET_MODE_AF_PP(gp, pins, speed)        \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_AF;                 \
    gp.GPIO_OType   = GPIO_OType_PP;                \
    gp.GPIO_PuPd    = GPIO_PuPd_NOPULL;             \
} while (0)

#define GPIO_SET_MODE_AF_OD(gp, pins, speed)        \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_AF;                 \
    gp.GPIO_OType   = GPIO_OType_OD;                \
    gp.GPIO_PuPd    = GPIO_PuPd_NOPULL;             \
} while (0)

#define GPIO_SET_MODE_IN_UP(gp, pins, speed)        \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_IN;                 \
    gp.GPIO_PuPd    = GPIO_PuPd_UP;                 \
} while (0)

#define GPIO_SET_MODE_IN_DOWN(gp, pins, speed)      \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_IN;                 \
    gp.GPIO_PuPd    = GPIO_PuPd_DOWN;               \
} while (0)

#define GPIO_SET_MODE_IN_NOPULL(gp, pins, speed)    \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_IN;                 \
    gp.GPIO_PuPd    = GPIO_PuPd_NOPULL;             \
} while (0)

#define GPIO_SET_MODE_AIN(gp, pins)                 \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Mode    = GPIO_Mode_AN;                 \
    gp.GPIO_PuPd    = GPIO_PuPd_NOPULL;             \
} while (0)

#define GPIO_RESET_BIT(port,pinmask)                do { (port)->BSRRH = (pinmask); } while (0)
#define GPIO_SET_BIT(port,pinmask)                  do { (port)->BSRRL = (pinmask); } while (0)
#define GPIO_SET_VALUE(port,mask,value)             do { ((GPIO_TypeDefExt *) (port))->BSRR = ((mask) << 16) | (value); } while (0)
#define GPIO_GET_BIT(port,pinmask)                  (((port)->IDR & (pinmask)) ? Bit_SET : Bit_RESET)

#elif defined (STM32F10X)

#define GPIO_SET_MODE_OUT_OD(gp, pins, speed)       \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_Out_OD;             \
} while (0)

#define GPIO_SET_MODE_OUT_PP(gp, pins, speed)       \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_Out_PP;             \
} while (0)

#define GPIO_SET_MODE_AF_PP(gp, pins, speed)        \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_AF_PP;              \
} while (0)

#define GPIO_SET_MODE_AF_OD(gp, pins, speed)        \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_AF_OD;              \
} while (0)

#define GPIO_SET_MODE_IN_UP(gp, pins, speed)        \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_IPU;                \
} while (0)

#define GPIO_SET_MODE_IN_DOWN(gp, pins, speed)      \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_IPD;                \
} while (0)

#define GPIO_SET_MODE_IN_NOPULL(gp, pins, speed)    \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Speed   = (speed);                      \
    gp.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        \
} while (0)

#define GPIO_SET_MODE_AIN(gp, pins)                 \
do                                                  \
{                                                   \
    gp.GPIO_Pin     = (pins);                       \
    gp.GPIO_Mode    = GPIO_Mode_AIN;                \
} while (0)

#define GPIO_RESET_BIT(port,pinmask)                do { (port)->BRR  = (pinmask); } while (0)
#define GPIO_SET_BIT(port,pinmask)                  do { (port)->BSRR = (pinmask); } while (0)
#define GPIO_SET_VALUE(port,mask,value)             do { (port)->BSRR = ((mask) << 16) | (value); } while (0)
#define GPIO_GET_BIT(port,pinmask)                  (((port)->IDR & (pinmask)) ? Bit_SET : Bit_RESET)

#define GPIO_Speed_100MHz                           GPIO_Speed_50MHz // no 100MHz on STM32F10X, define here to keep source simply

#endif

#define GPIO_SET_PIN_OUT_OD(port, pinmask, sp)      do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_OUT_OD(gp,    pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_OUT_PP(port, pinmask, sp)      do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_OUT_PP(gp,    pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_AF_PP(port, pinmask, sp)       do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_AF_PP(gp,     pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_AF_OD(port, pinmask, sp)       do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_AF_OD(gp,     pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_IN_UP(port, pinmask, sp)       do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_IN_UP(gp,     pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_IN_DOWN(port, pinmask, sp)     do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_IN_DOWN(gp,   pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_IN_NOPULL(port, pinmask, sp)   do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_IN_NOPULL(gp, pinmask, sp); GPIO_Init(port, &gp); } while (0)
#define GPIO_SET_PIN_AIN(port, pinmask)             do { GPIO_InitTypeDef gp; GPIO_StructInit (&gp); GPIO_SET_MODE_AIN(gp,       pinmask);     GPIO_Init(port, &gp); } while (0)

#endif // IO_H
