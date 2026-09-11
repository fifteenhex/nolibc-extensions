/* SPDX-License-Identifier: LGPL-2.1 OR MIT */

#ifndef __NOLIBC_EXT_SIGNAL_H
#define __NOLIBC_EXT_SIGNAL_H

#define SIGSETSZ 8

/*
 * The uapi headers disagree about how many signals there are: m68k has
 * NSIG at the old 32 and no _NSIG, the generic ones have _NSIG at 64
 * and no NSIG. 64 is what the kernel takes on all of them.
 */
#ifndef _NSIG
#define _NSIG 64
#endif

#define __NOLIBC_EXT_STR(_s) #_s
#define NOLIBC_EXT_STR(_s) __NOLIBC_EXT_STR(_s)

/*
 * The uapi struct sigaction is the shape libcs want, not the one
 * rt_sigaction() takes: handler, flags, restorer, mask. They match on
 * x86-64 but not on m68k.
 */
struct ksigaction {
	__sighandler_t handler;
	unsigned long flags;
#ifdef SA_RESTORER
	void (*restorer)(void);
#endif
	unsigned long mask[SIGSETSZ / sizeof(unsigned long)];
};

#if defined(SA_RESTORER) && defined(__x86_64__)
#define NOLIBC_EXT_HAS_RESTORER
/* x86-64 doesn't have trampolines in the kernel, so we must provide it here */
__asm__(
	".text\n"
	".weak __nolibc_ext_restorer\n"
	"__nolibc_ext_restorer:\n"
	"	movq $" NOLIBC_EXT_STR(__NR_rt_sigreturn) ", %rax\n"
	"	syscall\n"
);
void __nolibc_ext_restorer(void);
#endif

static long _sys_sigaction(int sig,
			   const struct ksigaction *act,
			   struct ksigaction *oldact)
{
	return __nolibc_syscall4(__NR_rt_sigaction,
				 (long)sig,
				 (long)act,
				 (long)oldact,
				 SIGSETSZ);
}

/* oldact isn't converted back, nothing here wants it */
static int sigaction(int sig,
		     const struct sigaction *act,
		     struct sigaction *oldact)
{
	struct ksigaction kact = { 0 };

	/* We don't use oldact yet, so don't handle it */
	if (oldact)
		return __sysret(-EINVAL);

	if (!act)
		return __sysret(_sys_sigaction(sig, NULL, NULL));

	kact.handler = act->sa_handler;
	kact.flags = act->sa_flags;
#ifdef NOLIBC_EXT_HAS_RESTORER
	kact.flags |= SA_RESTORER;
	kact.restorer = __nolibc_ext_restorer;
#endif

	return __sysret(_sys_sigaction(sig, &kact, NULL));
}

#endif /* __NOLIBC_EXT_SIGNAL_H */
