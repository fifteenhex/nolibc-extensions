/* SPDX-License-Identifier: LGPL-2.1 OR MIT */

#ifndef __NOLIBC_EXT_MODULES_H
#define __NOLIBC_EXT_MODULES_H

static long _sys_finit_module(int fd, const char *params, int flags)
{
	return __nolibc_syscall3(__NR_finit_module, fd, params, flags);
}

static int finit_module(int fd, const char *params, int flags)
{
	return __sysret(_sys_finit_module(fd, params, flags));
}

static long _sys_delete_module(const char *name, unsigned int flags)
{
	return __nolibc_syscall2(__NR_delete_module, name, flags);
}

/* flags takes O_NONBLOCK and O_TRUNC, not the open() ones they look
 * like: O_NONBLOCK means do not wait for the module to become unused,
 * O_TRUNC means unload even if still in use. rmmod passes O_NONBLOCK.
 */
static int delete_module(const char *name, unsigned int flags)
{
	return __sysret(_sys_delete_module(name, flags));
}

#endif /* __NOLIBC_EXT_MODULES_H */
