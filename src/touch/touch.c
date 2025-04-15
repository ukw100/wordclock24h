/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * touch.c - TFT touch functions
 *
 * Copyright (c) 2018-2024 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (BLACK_BOARD)           // STM32F407VE Blackboard with TFT & Touch

#include "touch.h"
#include "log.h"

#define MAX_TOUCH_BUTTONS       1

TOUCH_BUTTON                    touch_button[MAX_TOUCH_BUTTONS];

static void
touch_log (uint32_t idx, uint32_t x, uint32_t y)
{
    log_printf ("touch: button idx = %d, x = %d, y = %d\r\n", idx, x, y);
}

void
touch_check (void)
{
    static uint32_t real_touch = 0;
    uint32_t        x;
    uint32_t        y;
    uint32_t        idx;

    if (xpt2046_touched)
    {
        xpt2046_touched = 0;

        xpt2046_get_xy (&x, &y);

        if (x == 0)
        {
            if (real_touch == 1)
            {
                real_touch = 0;

                log_message ("pen up");
                log_flush ();
            }
            return;
        }

        if (real_touch == 0)
        {
            real_touch = 1;

            log_message ("pen down");
            log_flush ();
            xpt2046_get_xy (&x, &y);                                            // ignore 1st sample after pen down, read again

            for (idx = 0; idx < MAX_TOUCH_BUTTONS; idx++)
            {
                if (x >= touch_button[idx].x &&
                    y >= touch_button[idx].y &&
                    x <= touch_button[idx].x + touch_button[idx].w &&
                    y <= touch_button[idx].y + touch_button[idx].h)
                {
                    (*touch_button[idx].f) (idx, x, y);
                    break;
                }
            }

            if (idx == MAX_TOUCH_BUTTONS)
            {
                log_printf ("touch: no button found: x = %d y = %d\r\n", x, y);
            }
        }
    }
}

void
touch_init (void)
{
    xpt2046_init ();

    touch_button[0].x = 0;
    touch_button[0].y = 0;
    touch_button[0].w = 800;
    touch_button[0].h = 480;
    touch_button[0].f = touch_log;
}
#endif // BLACK_BOARD
