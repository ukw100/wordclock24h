/*-----------------------------------------------------------------------------------------
* wctris.c
*
* Copyright (C) 2017-2018 Frank Meyer - frank(at)fli4l.de
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*-----------------------------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#ifdef unix

#include <unistd.h>
#include <termio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define closesocket close

static struct termio                            oldmode;
static struct termio                            newmode;

#else

#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")                                                       // Winsock Library

#endif

#define TRUE        1
#define FALSE       0

#ifdef unix

#define myperror perror

static uint_fast8_t
termio_init(void)
{
	uint_fast8_t    rtc = 0;
	int             fd;

	fd = fileno(stdin);

	if (ioctl(fd, TCGETA, &oldmode) >= 0 || ioctl(fd, TCGETA, &newmode) >= 0)
	{
		newmode.c_lflag &= ~ICANON;                                         // switch off canonical input
		newmode.c_lflag &= ~ECHO;                                           // switch off echo
		newmode.c_iflag &= ~ICRNL;                                          // switch off CR->NL mapping
		newmode.c_oflag &= ~TAB3;                                           // switch off TAB conversion
		newmode.c_cc[VINTR] = '\377';                                       // disable VINTR VQUIT
		newmode.c_cc[VQUIT] = '\377';                                       // but don't touch VSWTCH
		newmode.c_cc[VMIN] = 0;                                             // set timeout
		newmode.c_cc[VTIME] = 2;                                            // in tenths of seconds

		if (ioctl(fd, TCSETAW, &newmode) >= 0)
		{
			rtc = 1;
		}
	}

	return rtc;
}

static void
termio_done(void)
{
	int     fd;

	fd = fileno(stdin);

	(void)ioctl(fd, TCSETAW, &oldmode);
}

#else

static void
myperror(char * s)
{
	int dw;
	TCHAR lpMsgBuf[1024];

	wchar_t ws[256];
	mbstowcs(ws, s, 256);
	dw = WSAGetLastError();

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		lpMsgBuf,
		1024,
		NULL);

	fwprintf(stderr, L"%s failed with error %d: %hs\n", ws, dw, lpMsgBuf);
}
#endif

static int
udp_socket(char * ipaddress, int port, struct sockaddr_in * addrp)
{
	struct hostent * target_p;
	int              fd;

	memset(addrp, 0, sizeof(struct sockaddr_in));

	if ((addrp->sin_addr.s_addr = inet_addr(ipaddress)) == INADDR_NONE)
	{
		target_p = gethostbyname(ipaddress);

		if (!target_p)
		{
			fprintf(stderr, "cannot resolve host name %s", ipaddress);
			return (-1);
		}

		(void)memcpy((char *)(&(addrp->sin_addr)), target_p->h_addr, target_p->h_length);
	}

	addrp->sin_family = AF_INET;
	addrp->sin_port = htons(port);

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (fd < 0)
	{
		myperror(ipaddress);
	}

	return fd;
}

static void
usage(char * pgm)
{
	fprintf(stderr, "usage:  %s ipaddress [wctris|snake] \n", pgm);
	exit(1);
}

#define KEY_ESC     27
#define KEY_SPACE   32
#define KEY_LEFT    37
#define KEY_UP      38
#define KEY_RIGHT   39
#define KEY_DOWN    40

#ifdef unix

static unsigned short
get_keypress(void)
{
	int     ch;

	if ((ch = getchar()) == KEY_ESC)
	{
		if ((ch = getchar()) == '[')
		{
			ch = getchar();

			switch (ch)
			{
			case 'A':   return KEY_UP;
			case 'B':   return KEY_DOWN;
			case 'C':   return KEY_RIGHT;
			case 'D':   return KEY_LEFT;
			}
		}
		else if (ch == KEY_ESC || ch == EOF)                                                    // 2 x ESC or timeout
		{
			return KEY_ESC;
		}
	}
	else if (ch == ' ')
	{
		return KEY_SPACE;
	}
	else if (ch == 's' || ch == 'S')
	{
		return 'S';
	}
	return 0;
}

#else

static unsigned short
get_keypress(void)
{
	HANDLE         input_handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD          events = 0;
	INPUT_RECORD   input_record;
	DWORD          input_size = 1;
	unsigned short virtual_key = 0;

	int not_a_keypress = 1;

	do
	{
		FlushConsoleInputBuffer(input_handle);
		ReadConsoleInput(input_handle, &input_record, input_size, &events);

		if (input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown)
		{
			virtual_key = input_record.Event.KeyEvent.wVirtualKeyCode;
			not_a_keypress = 0;
		}
	} while (not_a_keypress);

	return virtual_key;
}

#endif

/*----------------------------------------------------------------------------------------------------
* main function
*----------------------------------------------------------------------------------------------------
*/
int
main(int argc, char ** argv)
{
	struct sockaddr_in addr;
	int             fd;
	int             port = 2424;
	char *          ipaddress;
	int             ch;
	char            message[16];
	char *          pgm;
	char *          prefix = "gT";
	int             offset;
#ifndef unix
	WORD            wVersionRequested;
	WSADATA         wsaData;
	int             err;
#endif
	int             rtc = 1;

	pgm = argv[0];

	if (argc != 2 && argc != 3)
	{
		usage(pgm);
	}

	ipaddress = argv[1];

	if (argc == 3)
	{
		if (!strcmp(argv[2], "snake"))
		{
			prefix = "gS";
		}
		else if (!strcmp(argv[2], "wctris"))
		{
			prefix = "gT";
		}
		else
		{
			usage(pgm);
		}
	}

#ifndef unix
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		fprintf(stderr, "WSAStartup failed with error: %d\n", err);
		return 1;
	}
#endif

	fd = udp_socket(ipaddress, port, &addr);

	if (fd < 0)
	{
		exit(1);
	}

	strcpy(message, prefix);
	offset = strlen(prefix);

	puts("Press S to start, use cursor keys to move, Press SPACE to drop, ESC to quit");

#ifdef unix
	termio_init();
#endif

	while (1)
	{
		ch = get_keypress();

		if (ch > 0)
		{
			switch (ch)
			{
			case KEY_ESC:    message[offset] = 'q';         break;          // ESC
			case KEY_SPACE:  message[offset] = 'm';         break;          // SPACE
			case KEY_LEFT:   message[offset] = 'l';         break;          // Left
			case KEY_UP:     message[offset] = 'u';         break;          // Cursor Up
			case KEY_RIGHT:  message[offset] = 'r';         break;          // Cursor Right
			case KEY_DOWN:   message[offset] = 'd';         break;          // Cursor Down
			case 'S':        message[offset] = 's';         break;          // Key 'S': Start
			default: ch = 0;                                break;          // ignore key
			}

			if (ch > 0)
			{
#ifdef DEBUG
				message[offset + 1] = '\0';
				printf("key = %d, sending %s\n", ch, message);
#endif
				sendto(fd, message, offset + 1, 0, (struct sockaddr *) &addr, sizeof(addr));

				if (ch == KEY_ESC)
				{
					break;
				}
			}
		}
	}

#ifdef unix
	termio_done();
#endif

	(void)closesocket(fd);

	return (rtc);
}
