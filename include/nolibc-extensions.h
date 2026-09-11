/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * The one header to include, the way nolibc has nolibc.h. Force
 * include it, and after nolibc's own:
 *
 *   -include /path/to/nolibc/nolibc.h
 *   -include /path/to/nolibc-extensions/include/nolibc-extensions.h
 *
 * The order is what makes this work against either a stock nolibc or
 * one carrying patches. Headers a nolibc might already have keep
 * nolibc's own include guards rather than guards of their own: when
 * nolibc has the header, its guard is already defined by the time this
 * is read and our copy compiles to nothing.
 */

#ifndef _NOLIBC_EXTENSIONS_H
#define _NOLIBC_EXTENSIONS_H

#include "nolibc-extensions/socket.h"
#include "nolibc-extensions/statfs.h"
#include "nolibc-extensions/sendfile.h"
#include "nolibc-extensions/signal.h"
#include "nolibc-extensions/unistd.h"
#include "nolibc-extensions/xattr.h"

#endif /* _NOLIBC_EXTENSIONS_H */
