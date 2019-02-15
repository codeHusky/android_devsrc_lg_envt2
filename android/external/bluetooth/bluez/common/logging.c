/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2004-2009  Marcel Holtmann <marcel@holtmann.org>
 *  Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdarg.h>

#ifdef ANDROID
#define LOG_TAG "bluetoothd"
#include <cutils/log.h>
#else
#include <syslog.h>
#endif /* ANDROID */

#include "logging.h"


static volatile int debug_enabled = 0;

static inline void vinfo(const char *format, va_list ap)
{
#ifdef ANDROID
	LOG_PRI_VA(ANDROID_LOG_INFO, LOG_TAG, format, ap);
#else
	vsyslog(LOG_INFO, format, ap);
#endif
}

void info(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);

	vinfo(format, ap);

	va_end(ap);
}

void error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);

#ifdef ANDROID
	LOG_PRI_VA(ANDROID_LOG_ERROR, LOG_TAG, format, ap);
#else
	vsyslog(LOG_ERR, format, ap);
#endif

	va_end(ap);
}

void debug(const char *format, ...)
{
	va_list ap;

	if (!debug_enabled)
		return;

	va_start(ap, format);

#ifdef ANDROID
	LOG_PRI_VA(ANDROID_LOG_DEBUG, LOG_TAG, format, ap);
#else
	vsyslog(LOG_DEBUG, format, ap);
#endif

	va_end(ap);
}

void toggle_debug(void)
{
	debug_enabled = (debug_enabled + 1) % 2;
}

void enable_debug(void)
{
	debug_enabled = 1;
}

void disable_debug(void)
{
	debug_enabled = 0;
}

void start_logging(const char *ident, const char *message, ...)
{
	va_list ap;

#ifndef ANDROID
	openlog(ident, LOG_PID | LOG_NDELAY | LOG_PERROR, LOG_DAEMON);
#endif

	va_start(ap, message);

	vinfo(message, ap);

	va_end(ap);
}

void stop_logging(void)
{
#ifndef ANDROID
	closelog();
#endif
}
