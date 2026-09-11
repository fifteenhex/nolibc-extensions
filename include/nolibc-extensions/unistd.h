/* SPDX-License-Identifier: LGPL-2.1 OR MIT */

#ifndef __NOLIBC_EXT_UNISTD_H
#define __NOLIBC_EXT_UNISTD_H

/* archs that don't have setgid32 never had 16 bit gids, same for uids */
#ifndef __NR_setgid32
#define __NR_setgid32 __NR_setgid
#endif

#ifndef __NR_setuid32
#define __NR_setuid32 __NR_setuid
#endif

static long _sys_setgid(gid_t gid)
{
	return __nolibc_syscall1(__NR_setgid32, gid);
}

static long setgid(gid_t gid)
{
        return __sysret(_sys_setgid(gid));
}

static long _sys_setuid(uid_t uid)
{
	return __nolibc_syscall1(__NR_setuid32, uid);
}

static long setuid(uid_t uid)
{
        return __sysret(_sys_setuid(uid));
}

/* The generic syscall table has renameat and no plain rename */
#ifdef __NR_rename
static long _sys_rename(const char *old, const char *new)
{
	return __nolibc_syscall2(__NR_rename, old, new);
}
#else
static long _sys_rename(const char *old, const char *new)
{
	return __nolibc_syscall4(__NR_renameat, AT_FDCWD, old, AT_FDCWD, new);
}
#endif

static int rename(const char *old, const char *new)
{
	return __sysret(_sys_rename(old, new));
}

static long _sys_sync(void)
{
	return __nolibc_syscall0(__NR_sync);
}

static void sync(void)
{
	_sys_sync();
}

/*
 * The generic syscall table has no alarm either, so it is built out of
 * the timer that is there. Rounding a part second up matches what the
 * kernel's own alarm() does.
 */
#ifdef __NR_alarm
static long _sys_alarm(unsigned int seconds)
{
	return __nolibc_syscall1(__NR_alarm, seconds);
}

static unsigned int alarm(unsigned int seconds)
{
	return __sysret(_sys_alarm(seconds));
}
#else
/* struct itimerval and ITIMER_REAL come in with the uapi via types.h */
static unsigned int alarm(unsigned int seconds)
{
	struct itimerval new = { .it_value = { .tv_sec = seconds } };
	struct itimerval old;

	if (__sysret(__nolibc_syscall3(__NR_setitimer, ITIMER_REAL,
				       &new, &old)) < 0)
		return 0;

	return old.it_value.tv_sec + !!old.it_value.tv_usec;
}
#endif

#endif /* __NOLIBC_EXT_UNISTD_H */
