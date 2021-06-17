/*----------------------------------------------------------------------------------------------------------------------------------------
 * udpsrv.cpp - UDP server for Android App
 *
 * Copyright (c) 2016-2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "udpsrv.h"
#include "vars.h"
#include "base.h"

#define MAX_UDP_PACKET_SIZE        100

#define LISTENER_SET_COLOR_CODE                 'C'                             // set display color
#define LISTENER_SET_AMBILIGHT_COLOR_CODE       'c'                             // set ambilight color
#define LISTENER_POWER_CODE                     'P'                             // power on/off
#define LISTENER_DISPLAY_MODE_CODE              'D'                             // set display mode
#define LISTENER_AMBILIGHT_MODE_CODE            'd'                             // set ambilight mode
#define LISTENER_ANIMATION_MODE_CODE            'A'                             // set animation mode
#define LISTENER_COLOR_ANIMATION_MODE_CODE      'F'                             // set color animation mode
#define LISTENER_DISPLAY_TEMPERATURE_CODE       'W'                             // display temperature
#define LISTENER_SET_DISPLAY_FLAGS_CODE         'G'                             // set display flags
#define LISTENER_SET_BRIGHTNESS_CODE            'B'                             // set brightness
#define LISTENER_SET_AMBILIGHT_BRIGHTNESS_CODE  'b'                             // set ambilight brightness
#define LISTENER_SET_AUTOMATIC_BRIHGHTNESS_CODE 'L'                             // automatic brightness control on/off
#define LISTENER_TEST_DISPLAY_CODE              'X'                             // test display
#define LISTENER_SET_DATE_TIME_CODE             'T'                             // set date/time
#define LISTENER_SET_NET_DATE_TIME_CODE         't'                             // set date/time (timeserver)
#define LISTENER_GET_NET_TIME_CODE              'N'                             // Get net time
#define LISTENER_IR_LEARN_CODE                  'I'                             // IR learn
#define LISTENER_SET_NIGHT_TIME                 'J'                             // set night off time
#define LISTENER_SAVE_DISPLAY_CONFIGURATION     'S'                             // save display configuration
#define LISTENER_PRINT_TICKER_CODE              'p'                             // print ticker
#define LISTENER_TETRIS_CODE                    'g'                             // play tetris
#define LISTENER_DISCOVER_CODE                  'U'                             // discovery request
#define LISTENER_PLAY_FOLDER_TRACK_CODE         'f'                             // play track t in folder f
#define LISTENER_DISPLAY_OVERLAY_CODE           'o'                             // display overlay

static WiFiUDP         server_udp;
static unsigned int    server_udp_local_port = 2424;

/*----------------------------------------------------------------------------------------------------------------------------------------
 * setup udp service
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
udp_server_setup (void)
{
    Serial.println("- setup server UDP");
    server_udp.begin(server_udp_local_port);
    Serial.print("- local port: ");
    Serial.println(server_udp.localPort());
    Serial.flush ();
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * check for incoming udp packet
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
udp_server_loop (void)
{
    char udp_server_packet_buffer[MAX_UDP_PACKET_SIZE];

    int noBytes = server_udp.parsePacket();

    if (noBytes)
    {
#ifdef DEBUG
        Serial.print ("- ");
        Serial.print(millis() / 1000);
        Serial.print("sec: Packet of ");
        Serial.print(noBytes);
        Serial.print(" bytes received from ");
        Serial.print(server_udp.remoteIP());
        Serial.print(":");
        Serial.println(server_udp.remotePort());
        Serial.flush ();
#endif

        if (noBytes <= MAX_UDP_PACKET_SIZE)
        {
            server_udp.read (udp_server_packet_buffer, noBytes);            // read packet

            switch (udp_server_packet_buffer[0])
            {
                case LISTENER_ANIMATION_MODE_CODE:                          // set animation mode
                {
                    if (noBytes == 2)
                    {
                        set_numvar (ANIMATION_MODE_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }
                case LISTENER_COLOR_ANIMATION_MODE_CODE:                    // set color animation mode
                {
                    if (noBytes == 2)
                    {
                        set_numvar (COLOR_ANIMATION_MODE_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }
                case LISTENER_DISPLAY_MODE_CODE:                            // set display mode
                {
                    if (noBytes == 2)
                    {
                        set_numvar (DISPLAY_MODE_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }
                case LISTENER_AMBILIGHT_MODE_CODE:                          // set ambilight mode
                {
                    if (noBytes == 2)
                    {
                        set_numvar (AMBILIGHT_MODE_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }

                case LISTENER_SET_BRIGHTNESS_CODE:                          // set display brightness
                {
                    if (noBytes == 2)
                    {
                        set_numvar (DISPLAY_BRIGHTNESS_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }

                case LISTENER_SET_AMBILIGHT_BRIGHTNESS_CODE:                // set ambilight brightness
                {
                    if (noBytes == 2)
                    {
                        set_numvar (AMBILIGHT_BRIGHTNESS_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }

                case LISTENER_SET_COLOR_CODE:                               // set display RGB Colors
                {
                    if (noBytes == 4 || noBytes == 5)
                    {
                        DSP_COLORS rgb;

                        rgb.red     = udp_server_packet_buffer[1];
                        rgb.green   = udp_server_packet_buffer[2];
                        rgb.blue    = udp_server_packet_buffer[3];

                        if (noBytes == 5)
                        {
                            rgb.white = udp_server_packet_buffer[4];
                            set_dsp_color_var (DISPLAY_DSP_COLOR_VAR, &rgb, 1);
                        }
                        else
                        {
                            rgb.white = 0;
                            set_dsp_color_var (DISPLAY_DSP_COLOR_VAR, &rgb, 0);
                        }
                    }
                    break;
                }

                case LISTENER_SET_AMBILIGHT_COLOR_CODE:                     // set ambilight RGB Colors
                {
                    if (noBytes == 4 || noBytes == 5)
                    {
                        DSP_COLORS rgb;
                        rgb.red     = udp_server_packet_buffer[1];
                        rgb.green   = udp_server_packet_buffer[2];
                        rgb.blue    = udp_server_packet_buffer[3];

                        if (noBytes == 5)
                        {
                            rgb.white = udp_server_packet_buffer[4];
                            set_dsp_color_var (AMBILIGHT_DSP_COLOR_VAR, &rgb, 1);
                        }
                        else
                        {
                            rgb.white = 0;
                            set_dsp_color_var (AMBILIGHT_DSP_COLOR_VAR, &rgb, 0);
                        }
                    }
                    break;
                }

                case LISTENER_SET_DATE_TIME_CODE:                           // set Date/Time
                {
                    if (noBytes == 7)
                    {
                        TM tm;

                        tm.tm_year  = udp_server_packet_buffer[1] + 100;           // tm: year since 1900  | UDP packet buffer: year since 2000
                        tm.tm_mon   = udp_server_packet_buffer[2] - 1;             // tm: month 0..11      | UDP packet buffer: month 1..12
                        tm.tm_mday  = udp_server_packet_buffer[3];                 // tm: day 1..31        | UDP packet buffer: day 1..31
                        tm.tm_hour  = udp_server_packet_buffer[4];                 // tm: hour 1..24       | UDP packet buffer: hour 1..31
                        tm.tm_min   = udp_server_packet_buffer[5];                 // tm: minute 1..59     | UDP packet buffer: minute 1..31
                        tm.tm_sec   = udp_server_packet_buffer[6];                 // tm: sec 1..59        | UDP packet buffer: sec 1..59

                        set_tm_var (CURRENT_TM_VAR, &tm);
                    }
                    break;
                }

                case LISTENER_SET_AUTOMATIC_BRIHGHTNESS_CODE:               // automatic Brightness control per LDR
                {
                    if (noBytes == 2)
                    {
                        set_numvar (DISPLAY_AUTOMATIC_BRIGHTNESS_ACTIVE_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }

                case LISTENER_SET_DISPLAY_FLAGS_CODE:                       // set display flags
                {
                    if (noBytes == 2)
                    {
                        set_numvar (DISPLAY_FLAGS_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }

                case LISTENER_POWER_CODE:                                   // power on/off
                {
                    if (noBytes == 2)
                    {
                        set_numvar (DISPLAY_POWER_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }

                case LISTENER_DISPLAY_TEMPERATURE_CODE:                     // temperature
                {
                    if (noBytes == 1)
                    {
                        rpc (DISPLAY_TEMPERATURE_RPC_VAR);
                    }
                    break;
                }

                case LISTENER_GET_NET_TIME_CODE:                            // get net time
                {
                    if (noBytes == 1)
                    {
                        rpc (GET_NET_TIME_RPC_VAR);
                    }
                    break;
                }

                case LISTENER_PRINT_TICKER_CODE:                            // print ticker
                {
                    udp_server_packet_buffer[MAX_TICKER_TEXT_LEN + 1] = '\0';           // terminate ticker text!
                    set_strvar (TICKER_TEXT_STR_VAR, udp_server_packet_buffer + 1);
                }

                case LISTENER_TETRIS_CODE:
                {
                    udp_server_packet_buffer[0] = 'G';                                  // gTx -> gTx
                    Serial.print ("CMD ");
                    Serial.print (udp_server_packet_buffer);
                    Serial.print ("\r\n");
                    Serial.flush ();
                }

                case LISTENER_DISCOVER_CODE:
                {
                    if (strcmp(udp_server_packet_buffer, FS("UDISCOVER_WC24h")) == 0)
                    {
                        Serial.print(F("- discovery request from "));
                        Serial.print(server_udp.remoteIP());
                        Serial.print(F(":"));
                        Serial.println(server_udp.remotePort());
                        Serial.flush();
                        server_udp.beginPacket(server_udp.remoteIP(), server_udp.remotePort());
                        server_udp.print(F("URESPONSE_WC2h\0"));
                        server_udp.endPacket();
                    }
                }

                case LISTENER_PLAY_FOLDER_TRACK_CODE:
                {
                    if (noBytes == 3)
                    {
                        uint_fast8_t  folder = udp_server_packet_buffer[1];
                        uint_fast8_t  track  = udp_server_packet_buffer[2];
                        set_numvar (DFPLAYER_PLAY_FOLDER_TRACK_NUM_VAR, folder << 8 | track);
                    }
                    break;
                }

                case LISTENER_DISPLAY_OVERLAY_CODE:
                {
                    if (noBytes == 2)
                    {
                        set_numvar (DISPLAY_OVERLAY_NUM_VAR, udp_server_packet_buffer[1]);
                    }
                    break;
                }


            }
        }
        else
        {
            Serial.print ("- UDP packet too large: ");
            Serial.print (noBytes);
            Serial.print (" bytes, max is ");
            Serial.println (MAX_UDP_PACKET_SIZE);
        }

        Serial.flush ();
    }
}

