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

#endif /* __NOLIBC_EXT_UNISTD_H */
