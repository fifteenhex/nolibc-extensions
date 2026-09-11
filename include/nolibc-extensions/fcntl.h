/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * fcntl() for NOLIBC, plus the O_ASYNC flag userspace expects. nolibc
 * pulls in the uapi asm/fcntl.h, so the open flags and the F_* commands
 * are already in scope; only the wrapper and O_ASYNC are missing.
 */

#ifndef __NOLIBC_EXT_FCNTL_H
#define __NOLIBC_EXT_FCNTL_H

/* The uapi calls it FASYNC; userspace usually asks for O_ASYNC. */
#ifndef O_ASYNC
#define O_ASYNC FASYNC
#endif

/*
 * Variadic like the libc one: F_GETFL and friends take no argument, the
 * rest take an int or a pointer. The kernel ignores the extra argument
 * for the no-arg commands, so always passing one is harmless.
 */
static __attribute__((unused))
int fcntl(int fd, int cmd, ...)
{
	__builtin_va_list ap;
	long arg;

	__builtin_va_start(ap, cmd);
	arg = __builtin_va_arg(ap, long);
	__builtin_va_end(ap);

	return __sysret(__nolibc_syscall3(__NR_fcntl, fd, cmd, arg));
}

#endif /* __NOLIBC_EXT_FCNTL_H */
