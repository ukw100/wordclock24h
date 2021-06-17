/*-------------------------------------------------------------------------------------------------------------------------------------------
 * remote-ir.h - declaration of remote IR routines using IRMP
 *
 * Copyright (c) 2014-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef REMOTE_IR_H
#define REMOTE_IR_H

#define REMOTE_IR_CMD_INVALID                       0xFF

/*-----------------------------------------------------
 * New Modes (future use):
#define REMOTE_IR_CMD_POWER                         0
#define REMOTE_IR_CMD_OK                            1
#define REMOTE_IR_CMD_DECREMENT                     2
#define REMOTE_IR_CMD_INCREMENT                     3

#define REMOTE_IR_CMD_DISPLAY_MODE                  4
#define REMOTE_IR_CMD_COLOR_ANIMATION_MODE          5
#define REMOTE_IR_CMD_ANIMATION_MODE                6
#define REMOTE_IR_CMD_HOUR                          7
#define REMOTE_IR_CMD_MINUTE                        8
#define REMOTE_IR_CMD_BRIGHTNESS_RED                9
#define REMOTE_IR_CMD_BRIGHTNESS_GREEN              10
#define REMOTE_IR_CMD_BRIGHTNESS_BLUE               11
#define REMOTE_IR_CMD_BRIGHTNESS                    12
#define REMOTE_IR_CMD_AUTO_BRIGHTNESS_CONTROL       13
#define REMOTE_IR_CMD_GET_TEMPERATURE               14
*------------------------------------------------------
*/

#define REMOTE_IR_CMD_POWER                         0
#define REMOTE_IR_CMD_OK                            1

#define REMOTE_IR_CMD_DECREMENT_DISPLAY_MODE        2
#define REMOTE_IR_CMD_INCREMENT_DISPLAY_MODE        3

#define REMOTE_IR_CMD_DECREMENT_ANIMATION_MODE      4
#define REMOTE_IR_CMD_INCREMENT_ANIMATION_MODE      5

#define REMOTE_IR_CMD_DECREMENT_HOUR                6
#define REMOTE_IR_CMD_INCREMENT_HOUR                7

#define REMOTE_IR_CMD_DECREMENT_MINUTE              8
#define REMOTE_IR_CMD_INCREMENT_MINUTE              9

#define REMOTE_IR_CMD_DECREMENT_BRIGHTNESS_RED      10
#define REMOTE_IR_CMD_INCREMENT_BRIGHTNESS_RED      11

#define REMOTE_IR_CMD_DECREMENT_BRIGHTNESS_GREEN    12
#define REMOTE_IR_CMD_INCREMENT_BRIGHTNESS_GREEN    13

#define REMOTE_IR_CMD_DECREMENT_BRIGHTNESS_BLUE     14
#define REMOTE_IR_CMD_INCREMENT_BRIGHTNESS_BLUE     15

#define REMOTE_IR_CMD_DECREMENT_BRIGHTNESS          16
#define REMOTE_IR_CMD_INCREMENT_BRIGHTNESS          17

#define REMOTE_IR_CMD_AUTO_BRIGHTNESS_CONTROL       18

#define REMOTE_IR_CMD_GET_TEMPERATURE               19

#define N_REMOTE_IR_CMDS                            20

extern uint_fast8_t remote_ir_get_cmd (void);
extern uint_fast8_t remote_ir_learn (void);
extern uint_fast8_t remote_ir_read_codes_from_eeprom (void);
extern uint_fast8_t remote_ir_write_codes_to_eeprom (void);

#endif
