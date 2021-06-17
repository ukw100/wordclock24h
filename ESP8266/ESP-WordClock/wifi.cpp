/*----------------------------------------------------------------------------------------------------------------------------------------
 * wifi.cpp - WIFI stuff
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
#include "wifi.h"
#include "base.h"
#include "http.h"

static int  wifi_started = 0;

int         wifi_ap_mode = false;
char        wifi_ssid[WIFI_MAX_SSID_LEN + 1];
char        wifi_ip_address[WIFI_MAX_IP_ADDRESS_LEN + 1];

void
set_local_ip_address (void)
{
    IPAddress    localAddr  = { 0, 0, 0, 0 };
    int          oct1       = 0;
    int          oct2       = 0;
    int          oct3       = 0;
    int          oct4       = 0;

    if (wifi_ap_mode)
    {
        localAddr = WiFi.softAPIP();
    }
    else
    {
        localAddr = WiFi.localIP();
    }

    if (localAddr)
    {
        oct1      = localAddr[0];
        oct2      = localAddr[1];
        oct3      = localAddr[2];
        oct4      = localAddr[3];
    }

    sprintf(wifi_ip_address, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
}

void
set_ssid (void)
{
    strncpy (wifi_ssid, WiFi.SSID().c_str(), WIFI_MAX_SSID_LEN);
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * connect to AP
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
int
wifi_connect(const char * ssid, const char * key)
{
    int cnt;
    int connected = false;

    statusmsg ("OK", "cap");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin(ssid, key);

    for (cnt = 0; cnt < 30; cnt++)                                          // check 15 seconds if connected
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            connected = true;
            break;
        }

        delay(500);
    }

    if (connected)
    {
        http_server_begin();

        set_ssid ();
        wifi_ap_mode = false;
        set_local_ip_address ();

        statusmsg ("AP", wifi_ssid);
        statusmsg ("MODE", "client");
        statusmsg ("IPADDRESS", wifi_ip_address);

        wifi_started = true;
    }
    else
    {
        debugmsg ("cannot connect to", ssid);
    }
    return connected;
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * connect to AP
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
wifi_wps (void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin("", "");                                                               // tricky: make a failed connection or connect to last ssid
    delay (4000);

    if (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.beginWPSConfig ())
        {                                                                             // in case of a timeout we have an empty ssid
            if (WiFi.SSID().length() > 0)
            {                                                                         // connection successful
                wifi_connect (WiFi.SSID().c_str(), WiFi.psk().c_str());               // reconnect now to make ssid & key permanent
            }
        }
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------
 * setup local AP
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
void
wifi_ap (const char * ssid, const char * key)
{
    statusmsg ("OK", "ap");

    WiFi.mode(WIFI_AP);

    debugmsg ("disconnecting...");
    WiFi.disconnect();
    delay(100);
    debugmsg ("starting AP mode...");

    WiFi.softAP(ssid, key);

    debugmsg ("begin server...");
    http_server_begin ();

    statusmsg ("AP", ssid);
    statusmsg ("MODE", "ap");

    wifi_ap_mode = true;
    set_local_ip_address ();

    statusmsg ("IPADDRESS", wifi_ip_address);

    wifi_started = true;
}

void
wifi_check_if_started (void)
{
    if (! wifi_ap_mode)
    {
        if (! wifi_started)
        {
            int status = WiFi.status();

            if (status == WL_CONNECTED)
            {
                set_ssid ();
                set_local_ip_address ();

                debugmsg ("connected to AP");
                statusmsg ("AP", wifi_ssid);
                statusmsg ("MODE", "client");
                statusmsg ("IPADDRESS", wifi_ip_address);

                wifi_started = true;
                http_server_begin ();
            }
            else if (status == WL_DISCONNECTED)
            {
                IPAddress ipaddress = WiFi.softAPIP();

                if (ipaddress[0] != 0 || ipaddress[1] != 0 || ipaddress[2] != 0 || ipaddress[3] != 0)
                {
                    wifi_ap_mode = true;
                    set_local_ip_address ();

                    debugmsg ("working as AP");
                    statusmsg ("MODE", "ap");
                    statusmsg ("IPADDRESS", wifi_ip_address);

                    wifi_started = true;
                    http_server_begin ();
                }
            }
        }
    }
}

