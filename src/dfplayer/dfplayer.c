/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer.c - DFPLAYER MINI routines
 *
 * Copyright (c) 2017-2018 Frank Meyer - frank(at)fli4l.de
 *
 * ------------------------------------------------------------------------------------------------------------------
 *
 *  Serial Communication Format:
 *  ============================
 *
 *  $S          Start byte 0x7E                         Each command begins with 0x7E
 *  VER         Version                                 Version Information
 *  LEN         Number of bytes after LEN               End byte not counted
 *  CMD         Commands                                Indicate the specific operations, such as play / pause, etc.
 *  FEB         Command feedback                        If need for feedback, 1: feedback, 0: no feedback
 *  PAR1        Parameter 1                             Data high byte
 *  PAR2        Parameter 2                             Data low byte
 *  CHK1        Checksum 1                              Checksum high byte (do not include start byte]
 *  CHK2        Checksum 2                              Checksum low byte
 *  $O          End byte                                End byte 0xEF
 * ------------------------------------------------------------------------------------------------------------------
 *
 *  Serial Communication Commands:
 *  ==============================
 *  1) Directly send commands, no parameters returned:
 *
 *  0x01        PLAY_NEXT               Next
 *  0x02        PLAY_PREVIOUS           Previous
 *  0x03        PLAY_TRACK              Specify tracking(NUM)                   0-2999
 *  0x04        INCREASE_VOLUME         Increase volume
 *  0x05        DECREASE_VOLUME         Decrease volume
 *  0x06        SET_VOLUME              Specify volume                          0-30
 *  0x07        SET_EQ                  Specify EQ (0/1/2/3/4/5)                Normal/Pop/Rock/Jazz/Classic/Base
 *  0x08        SET_PLAYBACK_MODE       Specify playback mode (0/1/2/3)         Repeat/folder repeat/single repeat/ random
 *  0x09        SET_SOURCE_DEVICE       Specify playback source (0/1/2/3/4)     U/SD/AUX/SLEEP/FLASH
 *  0x0A        SET_STANDBY             Enter into standby - low power loss
 *  0x0B        SET_NORMAL_WORKING      Normal working
 *  0x0C        RESET_MODULE            Reset module
 *  0x0D        PLAY                    Playback
 *  0x0E        PAUSE                   Pause
 *  0x0F        PLAY_FOLDER             Specify folder to playback              1~10 (need to set by user)
 *  0x10        OUTPUT_SETTING          Volume adjust set                       DH = 1: Open volume adjust, DL: set volume gain 0~31
 *  0x11        REPEAT_PLAY             Repeat play                             1: start repeat play, 0:stop play
 *  0x12        PLAY_MP3_FOLDER         set MP3 folder
 *  0x13        PLAY_ADVERT             play advert
 *  0x14        PLAY_LARGE_FOLDER       play large folder
 *  0x15        STOP_ADVERT             stop advert
 *  0x16        STOP                    stop
 *  0x17        LOOP_FOLDER             loop folder
 *  0x18        RANDOM_ALL              random all
 *  0x19        ENABLE_DISABLE_LOOP     loop enable/disable
 *  0x1A        ENABLE_DISABLE_DAC      DAC enable/disable
 *
 *  2) Query the System Parameters:
 *
 *  0x3C        FINISH_PB_USB_STICK     playback of track nn on USB stick finished
 *  0x3D        FINISH_PB_SD_CARD       playback of track nn on SD card finished
 *  0x3E        FINISH_PB_FLASH         playback of track nn on flash finished
 *  0x3F        SEND_INIT               Send initialization parameters          0 - 0x0F (each bit represent one device of the low-four bits)
 *  0x40        RETURN_ERROR            Returns an error, request retransmission
 *  0x41        REPLY                   Reply
 *  0x42        QUERY_STATUS            Query the current status
 *  0x43        QUERY_VOLUME            Query the current volume
 *  0x44        QUERY_EQ                Query the current EQ
 *  0x45        QUERY_PLAYBACK_MODE     Query the current playback mode
 *  0x46        QUERY_SOFTWARE_VER      Query the current software version
 *  0x47        QUERY_SD_CARD_FILES     Query the total number of SD card files
 *  0x48        QUERY_USB_STICK_FILES   Query the total number of USB stick files
 *  0x49        QUERY_FLASH_FILES       Query the total number of flash files
 *  0x4A        KEEP_ON                 Keep on
 *  0x4B        QUERY_SD_TRACK          Queries the current track of SD card
 *  0x4C        QUERY_USB_STICK_TRACK   Queries the current track of USB stick
 *  0x4D        QUERY_FLASH_TRACK       Queries the current track of Flash
 *
 * ----------------------------------------------------------------------------------------------------------
 *
 *  Examples:
 *  =========
 *                                                      $S   VER  LEN  CMD  FEB  PAR1 PAR2 CHK1 CHK2   $0
 *  Specify playback source flash (4):                  7E   FF   06   09   00    00   04   FE   EE    EF
 *  Specify volume:                                     7E   FF   06   06   00    00   0F   FE   E6    EF
 *  Specify track 1:                                    7E   FF   06   03   00    00   01   FE   F7    EF
 *  Specify folder "01", file "100.mp3":                7E   FF   06   0F   00    01   64   FE   87    EF
 *
 * ----------------------------------------------------------------------------------------------------------
 *
 *  Additional Informations:
 *  ========================
 *  The checksums given in examples of DFPlayer Mini Manual are incorrect.
 *
 * ----------------------------------------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "delay.h"
#include "eeprom.h"
#include "eeprom-data.h"
#include "dfplayer.h"

#undef UART_PREFIX
#define UART_PREFIX                     dfplayer

#include "uart.h"

#define START_BYTE                      0x7E
#define END_BYTE                        0xEF

#define DFPLAYER_PLAY_NEXT              0x01
#define DFPLAYER_PLAY_PREVIOUS          0x02
#define DFPLAYER_PLAY_TRACK             0x03
#define DFPLAYER_INCREASE_VOLUME        0x04
#define DFPLAYER_DECREASE_VOLUME        0x05
#define DFPLAYER_SET_VOLUME             0x06
#define DFPLAYER_SET_EQ                 0x07
#define DFPLAYER_SET_PLAYBACK_MODE      0x08
#define DFPLAYER_SET_SOURCE_DEVICE      0x09
#define DFPLAYER_SET_STANDBY            0x0A
#define DFPLAYER_SET_NORMAL_WORKING     0x0B
#define DFPLAYER_RESET_MODULE           0x0C
#define DFPLAYER_PLAY                   0x0D
#define DFPLAYER_PAUSE                  0x0E
#define DFPLAYER_PLAY_FOLDER            0x0F
#define DFPLAYER_OUTPUT_SETTING         0x10
#define DFPLAYER_REPEAT_PLAY            0x11
#define DFPLAYER_PLAY_MP3_FOLDER        0x12
#define DFPLAYER_PLAY_ADVERT            0x13
#define DFPLAYER_PLAY_LARGE_FOLDER      0x14
#define DFPLAYER_STOP_ADVERT            0x15
#define DFPLAYER_STOP                   0x16
#define DFPLAYER_LOOP_FOLDER            0x17
#define DFPLAYER_RANDOM_ALL             0x18
#define DFPLAYER_ENABLE_DISABLE_LOOP    0x19
#define DFPLAYER_ENABLE_DISABLE_DAC     0x1A

#define DFPLAYER_PUSH_IN                0x3A
#define DFPLAYER_PULL_OUT               0x3B
#define DFPLAYER_FINISH_PB_USB_STICK    0x3C
#define DFPLAYER_FINISH_PB_SD_CARD      0x3D
#define DFPLAYER_FINISH_PB_FLASH        0x3E
#define DFPLAYER_SEND_INIT              0x3F
#define DFPLAYER_RETURN_ERROR           0x40
#define DFPLAYER_REPLY                  0x41
#define DFPLAYER_QUERY_STATUS           0x42
#define DFPLAYER_QUERY_VOLUME           0x43
#define DFPLAYER_QUERY_EQ               0x44
#define DFPLAYER_QUERY_PLAYBACK_MODE    0x45
#define DFPLAYER_QUERY_SOFTWARE_VER     0x46
#define DFPLAYER_QUERY_SD_CARD_FILES    0x47
#define DFPLAYER_QUERY_USB_STICK_FILES  0x48
#define DFPLAYER_QUERY_FLASH_FILES      0x49
#define DFPLAYER_KEEP_ON                0x4A
#define DFPLAYER_QUERY_SD_TRACK         0x4B
#define DFPLAYER_QUERY_USB_STICK_TRACK  0x4C
#define DFPLAYER_QUERY_FLASH_TRACK      0x4D

DFPLAYER        dfplayer;

#define         MAX_QUEUE_SIZE          16

static uint8_t  dfplayer_queue_idx = 0;
static uint8_t  dfplayer_queue_len = 0;
static uint8_t  dfplayer_queue[MAX_QUEUE_SIZE];

static void     dfplayer_init2 (void);

static uint16_t
calc_checksum (uint8_t * buffer)
{
    uint16_t    sum = 0;
    int         i;

    for (i = 1; i <= 6; i++)
    {
        sum += buffer[i];
    }

    return -sum;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * send a command
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_send_cmd (uint_fast8_t cmd, uint16_t arg)
{
    uint8_t         buf[10];
    uint8_t         hi;
    uint8_t         lo;
    uint16_t        chksum;
    uint_fast8_t    idx;

    buf[0]      = START_BYTE;           // 0: start byte: 0x7E
    buf[1]      = 0xFF;                 // 1: VER
    buf[2]      = 6;                    // 2: length: 6 bytes (VER to CHECKSUM)
    buf[3]      = cmd;                  // 3: command
    buf[4]      = 0;                    // 4: 0 = no feedback

    hi = (arg >> 8) & 0xFF;
    lo = arg & 0xFF;

    buf[5]      = hi;                   // 5: high byte arg
    buf[6]      = lo;                   // 6: low byte arg

    chksum = calc_checksum (buf);

    hi = (chksum >> 8) & 0xFF;
    lo = chksum & 0xFF;

    buf[7]      = hi;                   // 7: high byte checksum
    buf[8]      = lo;                   // 8: low byte checksum
    buf[9]      = END_BYTE;             // 9: stop byte: 0xEF

    for (idx = 0; idx < 10; idx++)
    {
        debug_log_printf ("%02x ", buf[idx]);
        dfplayer_uart_putc (buf[idx]);
    }
    debug_log_message ("");
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read message - this function should be called in ~1 second intervals
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
#define START_BYTE_POS  0
#define VER_BYTE_POS    1
#define LEN_BYTE_POS    2
#define CMD_BYTE_POS    3
#define FEB_BYTE_POS    4
#define PAR1_BYTE_POS   5
#define PAR2_BYTE_POS   6
#define CHK1_BYTE_POS   7
#define CHK2_BYTE_POS   8
#define END_BYTE_POS    9
#define N_BYTES         10

uint_fast8_t
dfplayer_read_message (void)
{
    static uint_fast8_t     last_cmd = 0xFF;
    static uint_fast16_t    last_par = 0xFFFF;                                    // finished message occurs 2 times, ignore 2nd
    uint8_t                 buf[N_BYTES];
    uint_fast8_t            cmd;
    uint_fast16_t           par;
    uint_fast16_t           chksum;
    uint_fast8_t            idx;
    uint_fast8_t            rtc = 0;

    if (dfplayer_uart_rsize () >= 10)
    {
        buf[START_BYTE_POS] = dfplayer_uart_getc ();

        if (buf[START_BYTE_POS] == START_BYTE)
        {
            buf[VER_BYTE_POS] = dfplayer_uart_getc ();
            buf[LEN_BYTE_POS] = dfplayer_uart_getc ();

            if (buf[LEN_BYTE_POS] == 0x06)
            {
                for (idx = CMD_BYTE_POS; idx < N_BYTES; idx++)
                {
                    buf[idx] = dfplayer_uart_getc ();
                }

                chksum = calc_checksum (buf);

                if (chksum != (((uint_fast16_t) buf[CHK1_BYTE_POS] << 8) | buf[CHK2_BYTE_POS]))
                {
                    log_printf ("DFPlayer: wrong checksum\r\n");
                }

#ifdef DEBUG
                log_printf ("%s:", "DFPlayer");

                for (idx = START_BYTE_POS; idx < N_BYTES; idx++)
                {
                    debug_log_printf (" %02X", buf[idx]);
                }

                log_puts ("\r\n");
#endif
                par = (buf[PAR1_BYTE_POS] << 8) | buf[PAR2_BYTE_POS];

                if (buf[END_BYTE_POS] == END_BYTE)
                {
                    cmd = buf[CMD_BYTE_POS];

                    int finished = 0;

                    if (cmd == DFPLAYER_FINISH_PB_SD_CARD)
                    {
                        if (dfplayer.version == 0x0005)                 // in version 0x0005 finished message occurs 2 times, ignore 2nd
                        {
                            if (last_cmd == DFPLAYER_FINISH_PB_SD_CARD && last_par == par)
                            {
                                finished = 1;
                            }
                        }
                        else
                        {
                            finished = 1;
                        }
                    }

                    if (finished)
                    {
                        last_cmd = 0xFF;
                        last_par = 0xFFFF;

                        if (dfplayer_queue_idx < dfplayer_queue_len)
                        {
                            dfplayer_play_folder (QUEUE_FOLDER, dfplayer_queue[dfplayer_queue_idx]);
                            dfplayer_queue_idx++;
                        }
                        else
                        {
                            dfplayer_queue_len = 0;
                        }
                        return rtc;
                    }

                    last_cmd    = cmd;
                    last_par    = par;

                    switch (cmd)
                    {
                        case DFPLAYER_PUSH_IN:
                            if (par == 1)
                            {
                                log_message ("DFPlayer: inserted USB stick");
                            }
                            else if (par == 2)
                            {
                                log_message ("DFPlayer: inserted SD card");
                            }
                            else
                            {
                                log_message ("DFPlayer: inserted unknown device");
                            }
                            break;
                        case DFPLAYER_PULL_OUT:
                            if (par == 1)
                            {
                                log_message ("DFPlayer: removed USB stick");
                            }
                            else if (par == 2)
                            {
                                log_message ("DFPlayer: removed SD card");
                            }
                            else
                            {
                                log_message ("DFPlayer: DFPlayer removed unknown device");
                            }
                            break;
                        case DFPLAYER_FINISH_PB_USB_STICK:
                            log_printf ("DFPlayer: track %d on USB stick finished.\r\n", par);
                            break;
                        case DFPLAYER_FINISH_PB_SD_CARD:
                            log_printf ("DFPlayer: track %d on SD card finished.\r\n", par);
                            break;
                        case DFPLAYER_FINISH_PB_FLASH:
                            log_printf ("DFPlayer: track %d on Flash finished.\r\n", par);
                            break;
                        case DFPLAYER_SEND_INIT:
                            if (par == 0)
                            {
                                log_message ("DFPlayer: No memory found");
                            }
                            else
                            {
                                if (par & 0x01)
                                {
                                    log_message ("DFPlayer: USB stick online");
                                }
                                if (par & 0x02)
                                {
                                    log_message ("DFPlayer: SD card online");
                                }
                                if (par & 0x04)
                                {
                                    log_message ("DFPlayer: PC (AUX) online");
                                }
                                if (par & 0x08)
                                {
                                    log_message ("DFPlayer: Flash online");
                                }
                            }
                            break;
                        case DFPLAYER_RETURN_ERROR:
                            switch (par)
                            {
                                case 0:
                                    log_message ("DFPlayer: Module is busy");
                                    break;
                                case 1:
                                    log_message ("DFPlayer: Frame data incomplete");
                                    break;
                                case 2:
                                    log_message ("DFPlayer: Verification error");
                                    break;
                                case 6:
                                    log_message ("DFPlayer: File not found");
                                    break;
                                default:
                                    log_printf  ("DFPlayer: Unknown error: 0x%02X 0x%02X\r\n", buf[PAR1_BYTE_POS], buf[PAR2_BYTE_POS]);
                                    break;
                            }
                            break;
                        case DFPLAYER_REPLY:
                            log_message ("DFPlayer: Reply??");
                            break;
                        case DFPLAYER_QUERY_STATUS:
                            log_printf ("DFPlayer: Status %02X %02X\r\n", buf[PAR1_BYTE_POS], buf[PAR2_BYTE_POS]);
                            break;
                        case DFPLAYER_QUERY_VOLUME:
                            log_printf ("DFPlayer: Volume %d\r\n", par);
                            break;
                        case DFPLAYER_QUERY_EQ:
                            switch (par)
                            {
                                case DFPLAYER_EQ_NORMAL:    log_message ("DFPlayer: EQ Normal");    break;
                                case DFPLAYER_EQ_POP:       log_message ("DFPlayer: EQ Pop");       break;
                                case DFPLAYER_EQ_ROCK:      log_message ("DFPlayer: EQ Rock");      break;
                                case DFPLAYER_EQ_JAZZ:      log_message ("DFPlayer: EQ Jazz");      break;
                                case DFPLAYER_EQ_CLASSIC:   log_message ("DFPlayer: EQ Classic");   break;
                                case DFPLAYER_EQ_BASS:      log_message ("DFPlayer: EQ Bass");      break;
                                default:                    log_message ("DFPlayer: EQ unknown");   break;
                            }
                            break;
                        case DFPLAYER_QUERY_PLAYBACK_MODE:
                            switch (par)
                            {
                                case DFPLAYER_PLAYBACK_MODE_REPEAT:         log_message ("DFPlayer: Playback mode Normal");     break;
                                case DFPLAYER_PLAYBACK_MODE_REPEAT_FOLDER:  log_message ("DFPlayer: Playback mode Folder");     break;
                                case DFPLAYER_PLAYBACK_MODE_REPEAT_SINGLE:  log_message ("DFPlayer: Playback mode Single");     break;
                                case DFPLAYER_PLAYBACK_MODE_REPEAT_RANDOM:  log_message ("DFPlayer: Playback mode Random");     break;
                                default:                                    log_message ("DFPlayer: Playback mode unknown");    break;
                            }
                            break;
                        case DFPLAYER_QUERY_SOFTWARE_VER:
                            log_printf ("DFPlayer: Version %04X\r\n", par);
                            dfplayer.version = par;
                            break;
                        case DFPLAYER_QUERY_SD_CARD_FILES:
                            log_printf ("DFPlayer: SD card files %d\r\n", par);
                            break;
                        case DFPLAYER_QUERY_USB_STICK_FILES:
                            log_printf ("DFPlayer: USB stick files %d\r\n", par);
                            break;
                        case DFPLAYER_QUERY_FLASH_FILES:
                            log_printf ("DFPlayer: Flash files %d\r\n", par);
                            break;
                        case DFPLAYER_KEEP_ON:
                            log_printf ("DFPlayer: Keep on???\r\n", par);
                            break;
                        case DFPLAYER_QUERY_SD_TRACK:
                            log_printf ("DFPlayer: SD card track %d\r\n", par);
                            break;
                        case DFPLAYER_QUERY_USB_STICK_TRACK:
                            log_printf ("DFPlayer: USB stick track %d\r\n", par);
                            break;
                        case DFPLAYER_QUERY_FLASH_TRACK:
                            log_printf ("DFPlayer: Flash track %d\r\n", par);
                            break;
                    }

                    if (! dfplayer.is_up)
                    {
                        dfplayer.is_up = 1;                        // 1st set up, then call init2
                        dfplayer_init2 ();
                    }

                    rtc = 1;
                }
            }
        }

#if 0
        if (finished)
        {
            if (dfplayer_queue_idx < dfplayer_queue_len)
            {
                dfplayer_play_folder (QUEUE_FOLDER, dfplayer_queue[dfplayer_queue_idx]);
                dfplayer_queue_idx++;
            }
            else
            {
                dfplayer_queue_len = 0;
            }
        }
#endif
    }

    return rtc;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play next
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_next (void)
{
    dfplayer_send_cmd (DFPLAYER_PLAY_NEXT, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play previous
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_previous (void)
{
    dfplayer_send_cmd (DFPLAYER_PLAY_PREVIOUS, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play track
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_track (uint_fast16_t file)
{
    log_printf ("DFPlayer: play track %d\r\n", file);
    dfplayer_send_cmd (DFPLAYER_PLAY_TRACK, file);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * volume up
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_volume_up (void)
{
    dfplayer_send_cmd (DFPLAYER_INCREASE_VOLUME, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * volume down
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_volume_down (void)
{
    dfplayer_send_cmd (DFPLAYER_DECREASE_VOLUME, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * set volume (1..30)
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_volume (uint_fast8_t volume)
{
    log_printf ("DFPlayer: set volume %d\r\n", volume);
    dfplayer_send_cmd (DFPLAYER_SET_VOLUME, volume);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * set EQ
 *
 * Parameters:
 *   DFPLAYER_EQ_NORMAL     Normal
 *   DFPLAYER_EQ_POP        Pop
 *   DFPLAYER_EQ_ROCK       Rock
 *   DFPLAYER_EQ_JAZZ       Jazz
 *   DFPLAYER_EQ_CLASSIC    Classic
 *   DFPLAYER_EQ_BASS       Bass
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_eq (uint_fast8_t eq)
{
    log_printf ("DFPlayer: set eq %d\r\n", eq);
    dfplayer_send_cmd (DFPLAYER_SET_EQ, eq);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * set playback mode
 *
 * Possible modes:
 *   DFPLAYER_PLAYBACK_MODE_REPEAT              Repeat
 *   DFPLAYER_PLAYBACK_MODE_REPEAT_FOLDER       Folder Repeat
 *   DFPLAYER_PLAYBACK_MODE_REPEAT_SINGLE       Single Repeat
 *   DFPLAYER_PLAYBACK_MODE_REPEAT_RANDOM       Random Repeat
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_playback_mode (uint16_t mode)
{
    dfplayer_send_cmd (DFPLAYER_SET_PLAYBACK_MODE, mode);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * set source device
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_source_device (uint8_t device)
{
    log_printf ("DFPlayer: set source device: %d\r\n", device);
    dfplayer_send_cmd (DFPLAYER_SET_SOURCE_DEVICE, device);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * standby
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_standby (void)
{
    dfplayer_send_cmd (DFPLAYER_SET_STANDBY, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * normal working
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_normal_working (void)
{
    dfplayer_send_cmd (DFPLAYER_SET_NORMAL_WORKING, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * reset
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_reset (void)
{
    dfplayer_send_cmd (DFPLAYER_RESET_MODULE, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * start play
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play (void)
{
    dfplayer_send_cmd (DFPLAYER_PLAY, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * pause
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_pause (void)
{
    dfplayer_send_cmd (DFPLAYER_PAUSE, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play folder
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_folder (uint_fast8_t folder, uint_fast8_t file)
{
    dfplayer_send_cmd (DFPLAYER_PLAY_FOLDER, (folder << 8) | file);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * output setting
 *
 * Parameters:
 *   enable: 1 or 0
 *   gain:   0..31
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_output_setting (uint_fast8_t enable, uint_fast8_t gain)
{
    dfplayer_send_cmd (DFPLAYER_OUTPUT_SETTING, (enable << 8) | gain);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * enable loop all
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_enable_loop_all (void)
{
    dfplayer_send_cmd (DFPLAYER_REPEAT_PLAY, 1);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * disable loop all
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_disable_loop_all (void)
{
    dfplayer_send_cmd (DFPLAYER_REPEAT_PLAY, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play mp3 folder
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_mp3_folder (uint_fast8_t file)
{
    dfplayer_send_cmd (DFPLAYER_PLAY_MP3_FOLDER, file);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play advert
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_advert (uint_fast8_t file)
{
    dfplayer_send_cmd (DFPLAYER_PLAY_ADVERT, file);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * play large folder
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_play_large_folder (uint_fast8_t folder, uint_fast16_t file)
{
    dfplayer_send_cmd (DFPLAYER_PLAY_LARGE_FOLDER, (((uint16_t) folder) << 12) | file);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * stop advert
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_stop_advert (void)
{
    dfplayer_send_cmd (DFPLAYER_STOP_ADVERT, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * stop
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_stop (void)
{
    dfplayer_send_cmd (DFPLAYER_STOP, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * loop folder
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_loop_folder (uint16_t folder)
{
    dfplayer_send_cmd (DFPLAYER_LOOP_FOLDER, folder);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * random all
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_random_all (void)
{
    dfplayer_send_cmd (DFPLAYER_RANDOM_ALL, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * enable loop
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_enable_loop (void)
{
    dfplayer_send_cmd (DFPLAYER_ENABLE_DISABLE_LOOP, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * disable loop
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_disable_loop (void)
{
    dfplayer_send_cmd (DFPLAYER_ENABLE_DISABLE_LOOP, 1);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * enable DAC - cancel mute
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_enable_dac (void)
{
    dfplayer_send_cmd (DFPLAYER_ENABLE_DISABLE_DAC, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * disable DAC - mute
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_disable_dac (void)
{
    dfplayer_send_cmd (DFPLAYER_ENABLE_DISABLE_DAC, 1);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read status
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_status (void)
{
    dfplayer_send_cmd (DFPLAYER_QUERY_STATUS, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read volume
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_volume (void)
{
    dfplayer_send_cmd (DFPLAYER_QUERY_VOLUME, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read equalizer values
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_eq (void)
{
    dfplayer_send_cmd (DFPLAYER_QUERY_EQ, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read playback mode
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_playback_mode (void)
{
    dfplayer_send_cmd (DFPLAYER_QUERY_PLAYBACK_MODE, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read software version
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_software_version (void)
{
    log_message ("DFPlayer: query software version");
    dfplayer_send_cmd (DFPLAYER_QUERY_SOFTWARE_VER, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read file count
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_file_count (uint8_t device)
{
    uint_fast8_t    cmd = 0;

    switch (device)
    {
        case DFPLAYER_DEVICE_SD:        cmd = DFPLAYER_QUERY_SD_CARD_FILES;     break;
        case DFPLAYER_DEVICE_USB_STICK: cmd = DFPLAYER_QUERY_USB_STICK_FILES;   break;
        case DFPLAYER_DEVICE_FLASH:     cmd = DFPLAYER_QUERY_FLASH_FILES;       break;
    }

    if (cmd)
    {
        dfplayer_send_cmd (cmd, 0);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * keep on
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_keep_on (void)
{
    dfplayer_send_cmd (DFPLAYER_KEEP_ON, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read current file number
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_current_file_number (uint8_t device)
{
    uint_fast8_t    cmd = 0;

    switch (device)
    {
        case DFPLAYER_DEVICE_SD:        cmd = DFPLAYER_QUERY_SD_TRACK;          break;
        case DFPLAYER_DEVICE_USB_STICK: cmd = DFPLAYER_QUERY_USB_STICK_TRACK;   break;
        case DFPLAYER_DEVICE_FLASH:     cmd = DFPLAYER_QUERY_FLASH_TRACK;       break;
    }

    if (cmd)
    {
        dfplayer_send_cmd (cmd, 0);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * read file counts in folder
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_query_file_counts_in_folder (uint8_t folder)
{
    dfplayer_send_cmd (0x4E, folder);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * read configuration from EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
dfplayer_read_config_from_eeprom (uint32_t eeprom_version)
{
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        rtc = 1;

        if (eeprom_version >= EEPROM_VERSION_2_7)
        {
            uint8_t     volume_buf8[EEPROM_DATA_SIZE_DFPLAYER_VOLUME];
            uint8_t     silence_start_buf8[EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START];
            uint8_t     silence_stop_buf8[EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP];
            uint8_t     mode_buf8[EEPROM_DATA_SIZE_DFPLAYER_MODE];
            uint8_t     bell_flags_buf8[EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS];
            uint8_t     speak_cycle_buf8[EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE];

            eeprom_read (EEPROM_DATA_OFFSET_DFPLAYER_VOLUME,        volume_buf8,        EEPROM_DATA_SIZE_DFPLAYER_VOLUME);
            eeprom_read (EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_START, silence_start_buf8, EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START);
            eeprom_read (EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_STOP,  silence_stop_buf8,  EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP);
            eeprom_read (EEPROM_DATA_OFFSET_DFPLAYER_MODE,          mode_buf8,          EEPROM_DATA_SIZE_DFPLAYER_MODE);
            eeprom_read (EEPROM_DATA_OFFSET_DFPLAYER_BELL_FLAGS,    bell_flags_buf8,    EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS);
            eeprom_read (EEPROM_DATA_OFFSET_DFPLAYER_SPEAK_CYCLE,   speak_cycle_buf8,   EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE);

            dfplayer.volume         = volume_buf8[0];
            dfplayer.silence_start  = silence_start_buf8[0] | (silence_start_buf8[1] << 8);
            dfplayer.silence_stop   = silence_stop_buf8[0]  | (silence_stop_buf8[1]  << 8);
            dfplayer.mode           = mode_buf8[0];
            dfplayer.bell_flags     = bell_flags_buf8[0];
            dfplayer.speak_cycle    = speak_cycle_buf8[0];
        }
        else
        {
            dfplayer.volume         = DFPLAYER_MAX_VOLUME;
            dfplayer.silence_start  = 0;
            dfplayer.silence_stop   = 0;
            dfplayer.mode           = DFPLAYER_MODE_NONE;
            dfplayer.bell_flags     = DFPLAYER_MODE_BELL_FLAG_15 | DFPLAYER_MODE_BELL_FLAG_30 | DFPLAYER_MODE_BELL_FLAG_45;
            dfplayer.speak_cycle    = 5;                                            // speak every 5 minutes
        }
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write volume to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_save_volume (void)
{
    uint8_t volume_buf8[EEPROM_DATA_SIZE_DFPLAYER_VOLUME];

    volume_buf8[0] = dfplayer.volume;
    eeprom_write (EEPROM_DATA_OFFSET_DFPLAYER_VOLUME, volume_buf8, EEPROM_DATA_SIZE_DFPLAYER_VOLUME);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write silence start time to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_save_silence_start (void)
{
    uint8_t silence_start_buf8[EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START];

    silence_start_buf8[0] = dfplayer.silence_start & 0xFF;
    silence_start_buf8[1] = (dfplayer.silence_start >> 8) & 0xFF;
    eeprom_write (EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_START, silence_start_buf8, EEPROM_DATA_SIZE_DFPLAYER_SILENCE_START);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write silence stop time to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_save_silence_stop (void)
{
    uint8_t silence_stop_buf8[EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP];

    silence_stop_buf8[0] = dfplayer.silence_stop & 0xFF;
    silence_stop_buf8[1] = (dfplayer.silence_stop >> 8) & 0xFF;
    eeprom_write (EEPROM_DATA_OFFSET_DFPLAYER_SILENCE_STOP, silence_stop_buf8, EEPROM_DATA_SIZE_DFPLAYER_SILENCE_STOP);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write mode to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_save_mode (void)
{
    uint8_t buf8[EEPROM_DATA_SIZE_DFPLAYER_MODE];

    buf8[0] = dfplayer.mode;
    eeprom_write (EEPROM_DATA_OFFSET_DFPLAYER_MODE, buf8, EEPROM_DATA_SIZE_DFPLAYER_MODE);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write bell flags to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_save_bell_flags (void)
{
    uint8_t buf8[EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS];

    buf8[0] = dfplayer.bell_flags;
    eeprom_write (EEPROM_DATA_OFFSET_DFPLAYER_BELL_FLAGS, buf8, EEPROM_DATA_SIZE_DFPLAYER_BELL_FLAGS);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write speak cycle to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_save_speak_cycle (void)
{
    uint8_t buf8[EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE];

    buf8[0] = dfplayer.speak_cycle;
    eeprom_write (EEPROM_DATA_OFFSET_DFPLAYER_SPEAK_CYCLE, buf8, EEPROM_DATA_SIZE_DFPLAYER_SPEAK_CYCLE);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * write configuration to EEPROM
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
dfplayer_write_config_to_eeprom (void)
{
    uint_fast8_t    rtc = 0;

    if (eeprom_is_up)
    {
        dfplayer_save_volume ();
        dfplayer_save_silence_start ();
        dfplayer_save_silence_stop ();
        dfplayer_save_mode ();
        dfplayer_save_bell_flags ();
        dfplayer_save_speak_cycle ();

        rtc = 1;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set volume
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_new_volume (uint_fast8_t volume)
{
    dfplayer.volume = volume;
    dfplayer_set_volume (dfplayer.volume);
    dfplayer_save_volume ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set silence start time
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_silence_start (uint_fast16_t silence_start)
{
    dfplayer.silence_start = silence_start;
    dfplayer_save_silence_start ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set silence stop time
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_silence_stop (uint_fast16_t silence_stop)
{
    dfplayer.silence_stop = silence_stop;
    dfplayer_save_silence_stop ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set mode
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_mode (uint_fast8_t mode)
{
    dfplayer.mode = mode;
    dfplayer_save_mode ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set bell flags
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_bell_flags (uint_fast8_t bell_flags)
{
    dfplayer.bell_flags = bell_flags;
    dfplayer_save_bell_flags ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * set speak cycle
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_set_speak_cycle (uint_fast8_t speak_cycle)
{
    dfplayer.speak_cycle = speak_cycle;
    dfplayer_save_speak_cycle ();
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer enqueue
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_enqueue (uint_fast8_t track)
{
    if (dfplayer_queue_len < MAX_QUEUE_SIZE)
    {
        dfplayer_queue[dfplayer_queue_len] = track;
        dfplayer_queue_len++;
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer flush queue
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_flush_queue (void)
{
    dfplayer_queue_idx = 0;
    dfplayer_queue_len = 0;
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer start queue
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_start_queue (void)
{
    dfplayer_queue_idx = 0;

    if (dfplayer_queue_idx < dfplayer_queue_len)
    {
        dfplayer_play_folder (QUEUE_FOLDER, dfplayer_queue[dfplayer_queue_idx]);
        dfplayer_queue_idx++;
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer init
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
void
dfplayer_init (void)
{
    dfplayer_uart_init (9600);
    log_puts ("DFPLAYER LOGGER\r\n");
    dfplayer_reset ();
}

/*--------------------------------------------------------------------------------------------------------------------------------------
 * dfplayer init step 2
 *--------------------------------------------------------------------------------------------------------------------------------------
 */
static void
dfplayer_init2 (void)
{
    dfplayer_query_software_version ();
    delay_msec (100);
    dfplayer_read_message ();

    dfplayer_source_device (DFPLAYER_DEVICE_SD);
    delay_msec (100);
    dfplayer_read_message ();

    dfplayer_set_eq (DFPLAYER_EQ_NORMAL);
    delay_msec (100);
    dfplayer_read_message ();

    dfplayer_set_volume (dfplayer.volume);
    delay_msec (100);
    dfplayer_read_message ();
    log_message ("DFPlayer is up");
}
