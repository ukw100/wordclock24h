/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * touch.h - declarations of TFT touch functions
 *
 * Copyright (c) 2018-2026 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (BLACK_BOARD)       // STM32F407VE Blackboard with TFT & Touch

#include "xpt2046.h"

typedef struct
{
    uint32_t    x;
    uint32_t    y;
    uint32_t    w;
    uint32_t    h;
    void        (*f)(uint32_t, uint32_t, uint32_t);
} TOUCH_BUTTON;

extern void     touch_check (void);
extern void     touch_init (void);


#endif // BLACK_BOARD
