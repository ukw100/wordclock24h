/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tetris.c - play tetris on wclock24h
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "wclock24h-config.h"

#if WCLOCK24H == 1
#define TETRIS_ROWS     WC_ROWS
#define TETRIS_COLS     12
#define ROW_OFFSET      0
#define COL_OFFSET      2
#else
#define TETRIS_ROWS     WC_ROWS
#define TETRIS_COLS     11
#define ROW_OFFSET      0
#define COL_OFFSET      0
#endif

#include "esp8266.h"
#include "display.h"
#include "delay.h"
#include "main.h"
#include "tetris.h"

#define LEFT_WALL   (0)
#define RIGHT_WALL  (TETRIS_COLS - 1)
#define LOWER_WALL  (TETRIS_ROWS - 1)

static uint8_t field[TETRIS_ROWS][TETRIS_COLS];

/*------------------------------------------------------------------------------------------------------------------------
 * tetromino I
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  ....        ..X.        ....        .X..
 *  XXXX        ..X.        ....        .X..
 *  ....        ..X.        XXXX        .X..
 *  ....        ..X.        ....        .X..
 *
 * tetromino J
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  X...        .XX.        ....        .X..
 *  XXX.        .X..        XXX.        .X..
 *  ....        .X..        ..X.        XX..
 *  ....        ....        ....        ....
 *
 * tetromino L
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  ..X.        .X..        ....        XX..
 *  XXX.        .X..        XXX.        .X..
 *  ....        .XX.        X...        .X..
 *  ....        ....        ....        ....
 *
 * tetromino O
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  .XX.        .XX.        .XX.        .XX.
 *  .XX.        .XX.        .XX.        .XX.
 *  ....        ....        ....        ....
 *  ....        ....        ....        ....
 *
 * tetromino S
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  .XX.        .X..        ....        X...
 *  XX..        .XX.        .XX.        XX..
 *  ....        ..X.        XX..        .X..
 *  ....        ....        ....        ....
 *
 * tetromino T
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  .X..        .X..        ....        .X..
 *  XXX.        .XX.        XXX.        XX..
 *  ....        .X..        .X..        .X..
 *  ....        ....        ....        ....
 *
 * tetromino Z
 *
 *  Pos1        Pos2        Pos3        Pos4
 *  XX..        ..X.        ....        .X..
 *  .XX.        .XX.        XX..        XX..
 *  ....        .X..        .XX.        X...
 *  ....        ....        ....        ....
 *
 *------------------------------------------------------------------------------------------------------------------------
 */

#define N_TETROMINOS                7

static const DSP_COLORS tetromino_colors[N_TETROMINOS] =
{
    DSP_CYAN_COLOR,
    DSP_BLUE_COLOR,
    DSP_GREY_COLOR,
    DSP_YELLOW_COLOR,
    DSP_GREEN_COLOR,
    DSP_MAGENTA_COLOR,
    DSP_RED_COLOR
};

static const DSP_COLORS dsp_color1 = DSP_WHITE_COLOR;

static LED_RGB rgb[N_TETROMINOS];
static LED_RGB rgb0;
static LED_RGB rgb1;

static const uint8_t tetrominos[N_TETROMINOS][4][4] =
{
    // tetromino I
    {
        {
            0x00,   // ....
            0x0F,   // XXXX
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x02,   // ..X.
            0x02,   // ..X.
            0x02,   // ..X.
            0x02,   // ..X.
        },
        {
            0x00,   // ....
            0x00,   // ....
            0x0F,   // XXXX
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x04,   // .X..
            0x04,   // .X..
            0x04,   // .X..
        },
    },

    // tetromino J
    {
        {
            0x08,   // X...
            0x0E,   // XXX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x06,   // .XX.
            0x04,   // .X..
            0x04,   // .X..
            0x00,   // ....
        },
        {
            0x00,   // ....
            0x0E,   // XXX.
            0x02,   // ..X.
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x04,   // .X..
            0x0C,   // XX..
            0x00,   // ....
        },
    },

    // tetromino L
    {
        {
            0x02,   // ..X.
            0x0E,   // XXX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x04,   // .X..
            0x06,   // .XX.
            0x00,   // ....
        },
        {
            0x00,   // ....
            0x0E,   // XXX.
            0x08,   // X...
            0x00,   // ....
        },
        {
            0x0C,   // XX..
            0x04,   // .X..
            0x04,   // .X..
            0x00,   // ....
        },
    },

    // tetromino 0
    {
        {
            0x06,   // .XX.
            0x06,   // .XX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x06,   // .XX.
            0x06,   // .XX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x06,   // .XX.
            0x06,   // .XX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x06,   // .XX.
            0x06,   // .XX.
            0x00,   // ....
            0x00,   // ....
        },
    },

    // tetromino S
    {
        {
            0x06,   // .XX.
            0x0C,   // XX..
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x06,   // .XX.
            0x02,   // ..X.
            0x00,   // ....
        },
        {
            0x00,   // ....
            0x06,   // .XX.
            0x0C,   // XX..
            0x00,   // ....
        },
        {
            0x08,   // X...
            0x0C,   // XX..
            0x04,   // .X..
            0x00,   // ....
        },
    },

    // tetromino T
    {
        {
            0x04,   // .X..
            0x0E,   // XXX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x06,   // .XX.
            0x04,   // .X..
            0x00,   // ....
        },
        {
            0x00,   // ....
            0x0E,   // XXX.
            0x04,   // .X..
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x0C,   // XX..
            0x04,   // .X..
            0x00,   // ....
        },
    },

    // tetromino Z
    {
        {
            0x0C,   // XX..
            0x06,   // .XX.
            0x00,   // ....
            0x00,   // ....
        },
        {
            0x02,   // ..X.
            0x06,   // .XX.
            0x04,   // .X..
            0x00,   // ....
        },
        {
            0x00,   // ....
            0x0C,   // XX..
            0x06,   // .XX.
            0x00,   // ....
        },
        {
            0x04,   // .X..
            0x0C,   // XX..
            0x08,   // X...
            0x00,   // ....
        },
    },
};


uint8_t tetromino_widths[N_TETROMINOS] = { 4, 3, 3, 3, 3, 3, 3 };

typedef struct
{
    int tetromino;
    int pos;
    int y;
    int x;
} TETROMINO_STATUS;

TETROMINO_STATUS    current_tetromino;

static void
draw_tetromino (int draw)
{
    int row;
    int col;

    for (row = 0; row < 4; row++)
    {
        if (current_tetromino.y + row < TETRIS_ROWS)
        {
            for (col = 0; col < 4; col++)
            {
                if (tetrominos[current_tetromino.tetromino][current_tetromino.pos][row] & (1 << col))
                {
                    if (current_tetromino.x + col < TETRIS_COLS)
                    {
                        uint_fast16_t n = (ROW_OFFSET + current_tetromino.y + row) * WC_COLUMNS + COL_OFFSET + current_tetromino.x + col;

                        if (draw)
                        {
                            display_set_display_led (n, &(rgb[current_tetromino.tetromino]), 0);
                        }
                        else
                        {
                            display_set_display_led (n, &rgb0, 0);
                        }
                    }
                }
            }
        }
    }
}


static void
set_field (int y, int x, int t)
{
    if (y >= 0 && y < TETRIS_ROWS && x >= 0 && x < TETRIS_COLS)
    {
        field[y][x] = t;
    }
}

static int
check_field (int y, int x)
{
    int rtc = 0;

    if (y >= 0 && y < LOWER_WALL && x >= LEFT_WALL && x < RIGHT_WALL)
    {
        if (field[y][x])
        {
            rtc = 1;
        }
    }
    return rtc;
}

static int
check_move (int pos, int y, int x)
{
    int row;
    int col;
    int rtc = 1;

    for (col = 0; col < 4; col++)
    {
        for (row = 0; row < 4; row++)
        {
            if ((tetrominos[current_tetromino.tetromino][pos][row] & (1 << (col))))
            {
                if (y + row >= LOWER_WALL || x + col <= LEFT_WALL || x + col >= RIGHT_WALL || check_field (y + row, x + col))
                {
                    rtc = 0;
                    break;
                }
            }
        }

        if (! rtc)
        {
            break;
        }
    }

    return rtc;
}

static void
fix_tetromino (void)
{
    int row;
    int col;
    int rtc = 1;

    for (col = 0; col < 4; col++)
    {
        for (row = 0; row < 4; row++)
        {
            if ((tetrominos[current_tetromino.tetromino][current_tetromino.pos][row] & (1 << (col))))
            {
                set_field (current_tetromino.y + row, current_tetromino.x + col, current_tetromino.tetromino + 1);
            }
        }

        if (! rtc)
        {
            break;
        }
    }
}

static int
tetromino_down (void)
{
    int new_y;
    int rtc = 0;

    new_y = current_tetromino.y + 1;

    if (check_move (current_tetromino.pos, new_y, current_tetromino.x))
    {
        draw_tetromino (0);
        current_tetromino.y = new_y;
        draw_tetromino (1);
        rtc = 1;
    }

    return rtc;
}

static int
tetromino_left (void)
{
    int new_x;
    int rtc = 0;

    new_x = current_tetromino.x - 1;

    if (check_move (current_tetromino.pos, current_tetromino.y, new_x))
    {
        draw_tetromino (0);
        current_tetromino.x = new_x;
        draw_tetromino (1);
        rtc = 1;
    }

    return rtc;
}

static int
tetromino_right (void)
{
    int new_x;
    int rtc = 0;

    new_x = current_tetromino.x + 1;

    if (check_move (current_tetromino.pos, current_tetromino.y, new_x))
    {
        draw_tetromino (0);
        current_tetromino.x = new_x;
        draw_tetromino (1);
        rtc = 1;
    }

    return rtc;
}

static int
tetromino_rotate (void)
{
    int new_pos;
    int rtc = 0;

    if (current_tetromino.pos == 3)
    {
        new_pos = 0;
    }
    else
    {
        new_pos = current_tetromino.pos + 1;
    }

    if (check_move (new_pos, current_tetromino.y, current_tetromino.x))
    {
        draw_tetromino (0);
        current_tetromino.pos = new_pos;
        draw_tetromino (1);
        rtc = 1;
    }
    return rtc;
}

static int
delete_full_lines (void)
{
    int y;
    int x;
    int xx;
    int yy;
    int n;
    int lines_deleted = 0;

    for (y = LOWER_WALL - 1; y >= 0; y--)
    {
        for (x = LEFT_WALL + 1; x < RIGHT_WALL; x++)
        {
            if (! check_field (y, x))
            {
                break;
            }
        }

        if (x == RIGHT_WALL)
        {
            lines_deleted++;

            for (yy = y; yy > 0; yy--)
            {
                n = (ROW_OFFSET + yy) * WC_COLUMNS;

                for (xx = LEFT_WALL + 1; xx < RIGHT_WALL; xx++)
                {
                    field[yy][xx] = field[yy - 1][xx];

                    if (check_field (yy, xx))
                    {
                        display_set_display_led (n + COL_OFFSET + xx, &(rgb[field[yy][xx] - 1]), 0);
                    }
                    else
                    {
                        display_set_display_led (n + COL_OFFSET + xx, &rgb0, 0);
                    }
                }
            }

            n = (ROW_OFFSET + 0) * WC_COLUMNS;

            for (xx = LEFT_WALL + 1; xx < RIGHT_WALL; xx++)
            {
                field[0][xx] = 0;
                display_set_display_led (n + COL_OFFSET + xx, &rgb0, 0);
            }

            y++;            // check this line again
        }
    }

    return lines_deleted;
}

void
tetris (void)
{
    DSP_COLORS      dsp_rgb_dimmed;
    unsigned char   message[32];
    int             msg_rtc;
    int             idx;
    int             ch;
    int             n;
    uint32_t        i;
    uint_fast8_t    level = 1;
    uint32_t        total_lines_deleted = 0;
    uint32_t        points = 0;
    uint32_t        loops = 50;                                 // 50 x 10 msec = 500 msec delay
    int             rtc = 1;

    for (idx = 0; idx < N_TETROMINOS; idx++)
    {
        display_dim_display_dsp_colors (&dsp_rgb_dimmed, &(tetromino_colors[idx]), display.display_brightness, 1);
        CALC_LED_RGB(rgb[idx], dsp_rgb_dimmed);
    }

    display_dim_display_dsp_colors (&dsp_rgb_dimmed, &dsp_color1, display.display_brightness, 1);
    CALC_LED_RGB(rgb1, dsp_rgb_dimmed);
    RESET_LED_RGB(rgb0);

    for (n = 0; n < WC_ROWS * WC_COLUMNS; n++)
    {
        display_set_display_led (n, &rgb0, 0);
    }

    n = (ROW_OFFSET + LOWER_WALL) * WC_COLUMNS;

    for (i = LEFT_WALL; i < RIGHT_WALL; i++)
    {
        display_set_display_led (n + COL_OFFSET + i, &rgb1, 0);
    }

    for (i = 0; i < TETRIS_ROWS; i++)
    {
        memset (field[i], 0, TETRIS_COLS);

        display_set_display_led ((ROW_OFFSET + i) * WC_COLUMNS + COL_OFFSET + LEFT_WALL, &rgb1, 0);
        set_field (i, LEFT_WALL, '|');

        display_set_display_led ((ROW_OFFSET + i) * WC_COLUMNS + COL_OFFSET + RIGHT_WALL, &rgb1, 0);
        set_field (i, RIGHT_WALL, '|');
    }

    idx = uptime % 7;

    current_tetromino.tetromino = idx;
    current_tetromino.pos       = 0;
    current_tetromino.y         = 0 - tetromino_widths[idx] + 2;
    current_tetromino.x         = (TETRIS_COLS - tetromino_widths[idx]) / 2;

    draw_tetromino (1);

    do
    {
        display_refresh_display_leds ();
        ch = 0;

        for (i = 0; i < loops; i++)
        {
            msg_rtc = esp8266_get_message ();

            if (msg_rtc == ESP8266_CMD)
            {
                char *              parameters;
                uint_fast8_t        cmd_code;

                parameters = esp8266.u.cmd;
                cmd_code = *parameters++;

                if (cmd_code == 'G')            // GTx
                {
                    cmd_code = *parameters++;

                    if (cmd_code == 'T')
                    {
                        ch = *parameters;
                        break;
                    }
                }
            }

            delay_msec (10);
        }

        if (ch == 'q')
        {
            break;
        }

        switch (ch)
        {
            case 'l': tetromino_left ();      break;
            case 'r': tetromino_right ();     break;
            case 'u': tetromino_rotate ();    break;
            case 'd': tetromino_down ();      break;
            case 'm':
            {
                while (tetromino_down ())
                {
                    display_refresh_display_leds ();
                    delay_msec (50);
                }
            }
            /* no break */

            default:
            {
                if (! tetromino_down ())
                {
                    uint_fast8_t lines_deleted;

                    fix_tetromino ();

                    lines_deleted = delete_full_lines ();

                    if (lines_deleted)
                    {
                        switch (lines_deleted)
                        {
                            case 1:  points +=   40 * level;    break;          // 1 line deleted:          add   40 points
                            case 2:  points +=  100 * level;    break;          // 2 lines deleted:         add  100 points
                            case 3:  points +=  300 * level;    break;          // 3 lines deleted:         add  300 points
                            default: points += 1200 * level;    break;          // 4 or more lines deleted: add 1200 points
                        }

                        total_lines_deleted += lines_deleted;

                        if (total_lines_deleted >= 10)
                        {
                            total_lines_deleted = 0;

                            level++;                                            // next level:

                            if (loops >= 10)
                            {
                                loops -= 5;                                     // make delay shorter, but not shorter than 5 x 10 msec = 50msec
                            }
                        }

                        display_refresh_display_leds ();
                    }

                    idx = uptime % 7;

                    current_tetromino.tetromino = idx;
                    current_tetromino.pos       = 0;
                    current_tetromino.y         = 0 - tetromino_widths[idx] + 2;
                    current_tetromino.x         = (TETRIS_COLS - tetromino_widths[idx]) / 2;

                    draw_tetromino (1);

                    if (! check_move (current_tetromino.pos, current_tetromino.y, current_tetromino.x))
                    {
                        rtc = 0;
                    }
                }
                break;
            }
        }
    } while (rtc == 1);

    delay_msec (200);

    sprintf ((char *) message, "Punkte: %ld", points);
    display_set_ticker (message, 1);
}
