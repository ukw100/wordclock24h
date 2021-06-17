/*----------------------------------------------------------------------------------------------------------------------------------------
 * httpclient.cpp - http client
 *
 * Copyright (c) 2018 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *----------------------------------------------------------------------------------------------------------------------------------------
 */
#include <ESP8266WiFi.h>
#include <strings.h>
#include "base.h"

static WiFiClient      client;

int
httpclient_read_header (int * lenp)
{
    char    linebuf[256];
    char *  p;
    int     cnt = 0;
    int     ch;
    int     len = 0;
    int     errorcode = 0;

    while (client.available())                           // skip http header
    {
        ch = client.read();

        if (ch == '\n')
        {
            if (cnt == 0)
            {
                break;
            }

            linebuf[cnt] = '\0';

            if (! mystrnicmp (linebuf, "HTTP", 4))
            {
                p = strchr (linebuf, ' ');

                if (p)
                {
                    while (*p == ' ')
                    {
                        p++;
                    }
                    errorcode = atoi (p);
                }
            }
            else if (! mystrnicmp (linebuf, "Content-Length: ", 16))
            {
                p = strchr (linebuf, ' ');

                if (p)
                {
                    while (*p == ' ')
                    {
                        p++;
                    }

                    len = atoi (p);
                }
            }

            cnt = 0;
        }
        else if (ch != '\r')
        {
            if (cnt < 256 - 1)
            {
                linebuf[cnt++] = ch;
            }
        }
    }

    *lenp = len;
    return errorcode;
}

int
httpclient (const char * host, const char * path, const char * file)
{
    const int       port = 80;
    int             errorcode;
    int             len;

    if (! client.connect(host, port))
    {
        return -1;
    }

    client.print (String("GET ") + "/" + path + "/" + file + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

    unsigned long timeout = millis();

    while (client.available() == 0)
    {
        if (millis() - timeout > 5000)
        {
            client.stop();
            return -1;
        }
    }

    errorcode = httpclient_read_header (&len);

    if (errorcode != 200)                                                               // webserver errorcode != 200 (OK)
    {
        client.stop ();
        return -1;
    }

    return len;                                                                         // length of content to be read
}

int
httpclient_read (int * lenp)
{
    int     len = *lenp;
    int     ch = -1;

    if (len > 0)
    {
        while (! client.available())
        {
            ;
        }

        ch = client.read();
        len--;
        *lenp = len;
    }
    return ch;
}

int
httpclient_read_line (unsigned char * bufp, int buflen, int * lenp)
{
    int     bufpos;
    int     ch;
    int     len = *lenp;

    bufpos = 0;

    while (len > 0 && bufpos < buflen)
    {
        while (! client.available())
        {
            ;
        }

        ch = client.read();

        *bufp++ = ch;
        bufpos++;

        len--;

        if (ch == '\n')
        {
            break;
        }
        yield ();
    }

    *bufp = '\0';
    *lenp = len;
    return bufpos;
}

void
httpclient_stop (void)
{
    client.stop ();
}
