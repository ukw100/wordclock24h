/*-------------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer.h - declarations of DFPLAYER MINI routines
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef DFPLAYER_H
#define DFPLAYER_H

#include <string.h>
#include <stdint.h>
#include <time.h>

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * source devices
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DFPLAYER_DEVICE_USB_STICK               1
#define DFPLAYER_DEVICE_SD                      2
#define DFPLAYER_DEVICE_AUX                     3       // aux not used
#define DFPLAYER_DEVICE_PC                      4
#define DFPLAYER_DEVICE_FLASH                   5
#define DFPLAYER_DEVICE_SLEEP                   6

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * EQ values
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DFPLAYER_EQ_NORMAL                      0
#define DFPLAYER_EQ_POP                         1
#define DFPLAYER_EQ_ROCK                        2
#define DFPLAYER_EQ_JAZZ                        3
#define DFPLAYER_EQ_CLASSIC                     4
#define DFPLAYER_EQ_BASS                        5

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Volume values
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DFPLAYER_MIN_VOLUME                     0
#define DFPLAYER_MAX_VOLUME                     30

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * playback modes
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
#define DFPLAYER_PLAYBACK_MODE_REPEAT           0               // Repeat
#define DFPLAYER_PLAYBACK_MODE_REPEAT_FOLDER    1               // Folder Repeat
#define DFPLAYER_PLAYBACK_MODE_REPEAT_SINGLE    2               // Single Repeat
#define DFPLAYER_PLAYBACK_MODE_REPEAT_RANDOM    3               // Random Repeat

#define BELL_FOLDER                             1
#define QUEUE_FOLDER                            2
#define ALARM_FOLDER                            3

typedef enum
{
    DFPLAYER_TRACK_HOUR0,
    DFPLAYER_TRACK_HOUR1,
    DFPLAYER_TRACK_HOUR2,
    DFPLAYER_TRACK_HOUR3,
    DFPLAYER_TRACK_HOUR4,
    DFPLAYER_TRACK_HOUR5,
    DFPLAYER_TRACK_HOUR6,
    DFPLAYER_TRACK_HOUR7,
    DFPLAYER_TRACK_HOUR8,
    DFPLAYER_TRACK_HOUR9,
    DFPLAYER_TRACK_HOUR10,
    DFPLAYER_TRACK_HOUR11,
    DFPLAYER_TRACK_HOUR12,
    DFPLAYER_TRACK_HOUR_15M,
    DFPLAYER_TRACK_HOUR_30M,
    DFPLAYER_TRACK_HOUR_45M,
    DFPLAYER_TRACKS_USED                            // should be the last one
} DFPLAYER_TRACK;

#define DFPLAYER_MODE_NONE              0
#define DFPLAYER_MODE_BELL              1           // play tracks at xx:00, xx:15, xx:30, xx:45 o'clock
#define DFPLAYER_MODE_SPEAK             2           // speak the current time
#define DFPLAYER_MODE_3                 3
#define DFPLAYER_MODE_4                 4
#define DFPLAYER_MODE_5                 5
#define DFPLAYER_MODE_6                 6
#define DFPLAYER_MODE_7                 7

#define DFPLAYER_MODE_BELL_FLAG_NONE    0x00
#define DFPLAYER_MODE_BELL_FLAG_15      0x01
#define DFPLAYER_MODE_BELL_FLAG_30      0x02
#define DFPLAYER_MODE_BELL_FLAG_45      0x04

typedef struct
{
    uint_fast8_t        is_up;
    uint_fast16_t       version;
    uint_fast8_t        volume;
    uint_fast16_t       silence_start;
    uint_fast16_t       silence_stop;
    uint_fast8_t        mode;
    uint_fast8_t        bell_flags;
    uint_fast8_t        speak_cycle;                // speak every 1, 5, 15, 30, 60 minutes
} DFPLAYER;

extern DFPLAYER         dfplayer;

extern uint_fast8_t     dfplayer_read_message (void);

extern void             dfplayer_play_next (void);
extern void             dfplayer_play_previous (void);
extern void             dfplayer_play_track (uint_fast16_t);
extern void             dfplayer_volume_up (void);
extern void             dfplayer_volume_down (void);
extern void             dfplayer_set_volume (uint_fast8_t);
extern void             dfplayer_set_eq (uint_fast8_t eq);
extern void             dfplayer_set_playback_mode (uint16_t);
extern void             dfplayer_source_device (uint8_t);
extern void             dfplayer_standby (void);
extern void             dfplayer_normal_working (void);
extern void             dfplayer_reset (void);
extern void             dfplayer_play (void);
extern void             dfplayer_pause (void);
extern void             dfplayer_play_folder (uint_fast8_t, uint_fast8_t);
extern void             dfplayer_output_setting (uint_fast8_t, uint_fast8_t);
extern void             dfplayer_enable_loop_all (void);
extern void             dfplayer_disable_loop_all (void);
extern void             dfplayer_play_mp3_folder (uint_fast8_t);
extern void             dfplayer_play_advert (uint_fast8_t);
extern void             dfplayer_play_large_folder (uint_fast8_t, uint_fast16_t);
extern void             dfplayer_stop_advert (void);
extern void             dfplayer_stop (void);
extern void             dfplayer_loop_folder (uint16_t);
extern void             dfplayer_random_all (void);
extern void             dfplayer_enable_loop (void);
extern void             dfplayer_disable_loop (void);
extern void             dfplayer_enable_dac (void);
extern void             dfplayer_disable_dac (void);
extern void             dfplayer_query_status (void);
extern void             dfplayer_query_volume (void);
extern void             dfplayer_query_eq (void);
extern void             dfplayer_query_playback_mode (void);
extern void             dfplayer_query_software_version (void);
extern void             dfplayer_query_file_count (uint8_t);
extern void             dfplayer_keep_on (void);
extern void             dfplayer_query_current_file_number (uint8_t);
extern void             dfplayer_query_file_counts_in_folder (uint8_t);

extern uint_fast8_t     dfplayer_read_config_from_eeprom (uint32_t);
extern uint_fast8_t     dfplayer_write_config_to_eeprom (void);

extern void             dfplayer_set_new_volume (uint_fast8_t);
extern void             dfplayer_set_silence_start (uint_fast16_t);
extern void             dfplayer_set_silence_stop (uint_fast16_t);
extern void             dfplayer_set_mode (uint_fast8_t);
extern void             dfplayer_set_bell_flags (uint_fast8_t);
extern void             dfplayer_set_speak_cycle (uint_fast8_t);

extern void             dfplayer_enqueue (uint_fast8_t);
extern void             dfplayer_flush_queue (void);
extern void             dfplayer_start_queue (void);
extern void             dfplayer_init (void);

#endif
