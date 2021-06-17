/*-------------------------------------------------------------------------------------------------------------------------------------------
 * snake.c - play snake on wclock24h
 *
 * Copyright (c) 2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "wclock24h-config.h"
#include "esp8266.h"
#include "display.h"
#include "delay.h"
#include "main.h"
#include "snake.h"

#define SNAKE_ROWS      WC_ROWS
#define SNAKE_COLS      WC_COLUMNS

static const DSP_COLORS snake_color = DSP_GREEN_COLOR;
static DSP_COLORS       snake_rgb_dimmed;
static LED_RGB          snake_rgb;

static const DSP_COLORS food_color  = DSP_RED_COLOR;
static DSP_COLORS       food_rgb_dimmed;
static LED_RGB          food_rgb;

static LED_RGB          rgb0;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    QUIT
} DIRECTION;

typedef enum
{
    SNAKE_SUCCESS,
    SNAKE_FAILURE
} STATUS;

#define MAX_FOOD        3
#define MAX_SNAKE_LEN   ((SNAKE_ROWS * SNAKE_COLS) - MAX_FOOD)

static uint8_t          food_y[MAX_FOOD];
static uint8_t          food_x[MAX_FOOD];

#define EMPTY_CELL      0xFF
#define NO_NEXT_CELL    0xFE

static uint8_t          snake_list[SNAKE_ROWS][SNAKE_COLS];
static uint8_t          snake_y;
static uint8_t          snake_x;

static uint_fast16_t    points;
static uint_fast16_t    level;                                                              // level, beginning with 1
static uint_fast16_t    snake_len;                                                          // snake length, beginning with 2
static uint32_t         delay_loops;                                                        // in units of 10 msec

static int
food_at_cell (int y, int x)
{
    int     i;

    for (i = 0; i < MAX_FOOD; i++)
    {
        if (food_y[i] == y && food_x[i] == x)
        {
            return i;
        }
    }
    return -1;
}

static void
add_new_food (int idx)
{
    int y;
    int x;

    food_y[idx] = 0xFF;
    food_x[idx] = 0xFF;

    do
    {
        y = rand() % SNAKE_ROWS;
        x = rand() % SNAKE_COLS;

    } while (snake_list[y][x] != EMPTY_CELL || food_at_cell (y, x) >= 0);

    food_y[idx] = y;
    food_x[idx] = x;
}

static void
cutoff_snake (void)
{
    int     y       = snake_y;
    int     x       = snake_x;
    int     new_y   = snake_y;
    int     new_x   = snake_x;

    while (1)
    {
        switch (snake_list[y][x])
        {
            case UP:        new_y = y - 1;      break;
            case DOWN:      new_y = y + 1;      break;
            case LEFT:      new_x = x - 1;      break;
            case RIGHT:     new_x = x + 1;      break;
        }

        if (snake_list[new_y][new_x] == NO_NEXT_CELL)                                   // reached end
        {
            snake_list[new_y][new_x]    = EMPTY_CELL;
            snake_list[y][x]            = NO_NEXT_CELL;
            display_set_display_led (new_y * WC_COLUMNS + new_x, &rgb0, 0);
            break;
        }
        else
        {
            y = new_y;
            x = new_x;
        }
    }
}

static STATUS
move_snake (DIRECTION dir)
{
    int     new_y   = snake_y;
    int     new_x   = snake_x;
    int     idx;
    STATUS  rtc     = SNAKE_SUCCESS;

    switch (dir)
    {
        case UP:        new_y = snake_y - 1;        break;
        case DOWN:      new_y = snake_y + 1;        break;
        case LEFT:      new_x = snake_x - 1;        break;
        case RIGHT:     new_x = snake_x + 1;        break;
        case QUIT:                                  break;
    }

    if (new_y < 0 || new_x < 0 || new_y >= SNAKE_ROWS || new_x >= SNAKE_COLS || snake_list[new_y][new_x] != EMPTY_CELL)
    {
        rtc = SNAKE_FAILURE;
    }
    else
    {
        snake_y = new_y;                                                                                            // store new position
        snake_x = new_x;

        switch (dir)
        {                                                                                                           // set the new beginning of the snake
            case UP:        snake_list[snake_y][snake_x] = DOWN;        break;
            case DOWN:      snake_list[snake_y][snake_x] = UP;          break;
            case LEFT:      snake_list[snake_y][snake_x] = RIGHT;       break;
            case RIGHT:     snake_list[snake_y][snake_x] = LEFT;        break;
            case QUIT:                                                  break;
        }

        if ((idx = food_at_cell (snake_y, snake_x)) >= 0)                                                           // food at cell?
        {
            points += level;
            snake_len++;
            add_new_food (idx);                                                                                     // yes, add new food
        }
        else
        {
            cutoff_snake ();                                                                                        // cutoff end of snake
        }
    }

    return rtc;
}

static void
display_food (void)
{
    int     idx;

    for (idx = 0; idx < MAX_FOOD; idx++)
    {
        display_set_display_led (WC_COLUMNS * food_y[idx] + food_x[idx], &food_rgb, 0);
    }
}

static void
display_snake (void)
{
    int y = snake_y;
    int x = snake_x;

    while (1)
    {
        display_set_display_led (WC_COLUMNS * y + x, &snake_rgb, 0);

        switch (snake_list[y][x])
        {
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
        }

        if (snake_list[y][x] == NO_NEXT_CELL)
        {
            display_set_display_led (WC_COLUMNS * y + x, &snake_rgb, 0);
            break;
        }
    }
}

static DIRECTION
get_next_move (DIRECTION previous)
{
    int         ch = 0;
    int         msg_rtc;
    uint32_t    i;

    for (i = 0; i < delay_loops; i++)
    {
        msg_rtc = esp8266_get_message ();

        if (msg_rtc == ESP8266_CMD)
        {
            char *              parameters;
            uint_fast8_t        cmd_code;

            parameters = esp8266.u.cmd;
            cmd_code = *parameters++;

            if (cmd_code == 'G')            // GSx
            {
                cmd_code = *parameters++;

                if (cmd_code == 'S')
                {
                    ch = *parameters;
                    break;
                }
            }
        }

        delay_msec (10);
    }

    switch (ch)
    {
        case 'l': if (previous != RIGHT) return LEFT;
        case 'r': if (previous != LEFT) return RIGHT;
        case 'u': if (previous != DOWN) return UP;
        case 'd': if (previous != UP) return DOWN;
        case 'q': return QUIT;
        default: return previous;
    }
}

void
snake (void)
{
    unsigned char   message[32];
    DIRECTION       dir;
    int             y;
    int             x;
    int             n;
    STATUS          status;
    int             idx;

    display_dim_display_dsp_colors (&snake_rgb_dimmed, &snake_color, display.display_brightness, 1);
    CALC_LED_RGB(snake_rgb, snake_rgb_dimmed);
    display_dim_display_dsp_colors (&food_rgb_dimmed, &food_color, display.display_brightness, 1);
    CALC_LED_RGB(food_rgb, food_rgb_dimmed);
    RESET_LED_RGB(rgb0);

    points      = 0;
    delay_loops = 30;                                                                       // initial delay = 30 x 10 = 300 msec
    level       = 1;                                                                        // initial level

    while (1)
    {
        for (y = 0; y < SNAKE_ROWS; y++)
        {
            for (x = 0; x < SNAKE_COLS; x++)
            {
                snake_list[y][x] = EMPTY_CELL;
            }
        }

        snake_list[3][2]    = UP;
        snake_list[2][2]    = NO_NEXT_CELL;
        snake_y             = 3;
        snake_x             = 2;
        snake_len           = 2;
        dir                 = RIGHT;

        for (idx = 0; idx < MAX_FOOD; idx++)
        {
            add_new_food (idx);
        }

        for (n = 0; n < WC_ROWS * WC_COLUMNS; n++)
        {
            display_set_display_led (n, &rgb0, 0);
        }

        while (1)
        {
            display_snake ();
            display_food ();
            display_refresh_display_leds ();

            dir = get_next_move (dir);

            if (dir == QUIT)
            {
                break;
            }

            status  = move_snake (dir);

            if (status == SNAKE_FAILURE || snake_len >= MAX_SNAKE_LEN)                      // GAME OVER or LEVEL finished
            {
                break;
            }

        }

        if (status == SNAKE_FAILURE || dir == QUIT)
        {
            break;                                                                          // GAME OVER
        }

        if (delay_loops > 10)                                                               // next level: make it faster
        {
            level++;
            delay_loops -= 10;
        }
    }

    delay_msec (200);

    sprintf ((char *) message, "Punkte: %d", points);
    display_set_ticker (message, 1);
}
