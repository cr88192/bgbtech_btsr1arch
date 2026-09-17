/*
System Call interface for ROM.

This isn't a true interrupt, and as such may stomp the scratch registers.

sObj will be the "self object" as relevant for the call.
  The meaning of this object will depend on the message.

uMsg: Will identify the request.
  (15:12): Subsystem ID
  (11: 0): Request ID (Magic Number)

vParm1/vParm2: Will be message-specific parameters (such as user-provided structures).

One scheme may operate as:
  vParm1 = pointer to return data / value.
  vParm2 = pointer to an arguments list.

*/

#include <setjmp.h>

// jmp_buf	tk_sysc_exit;
// u64	tk_sysc_exit[128];
u64		*tk_sysc_exit;

int __setj_sys(u64 *buf);
int __longj_sys(u64 *buf, int ret);

TKPE_TaskInfo *TK_SpawnNewThread2B(
	TKPE_TaskInfo *btask, void *func, void *uptr, TKPE_CreateTaskInfo *info);
TKPE_TaskInfo *TK_SpawnNewThreadB(
	TKPE_TaskInfo *btask, void *func, void *uptr);

void TKGDI_ComGlueDispatch(TKPE_TaskInfo *task,
	void *sObj, int idx, void *pret, void *args);

int TK_RegCache_GetKeyPath_Str(TK_RegContext *ctx, char *path,
	char *buf, int bsz);
int TK_RegCache_SetKeyPath_Str(TK_RegContext *ctx, char *path, char *buf);
int TK_RegGbl_SetKeyPath_Str(char *path, char *buf);
int TK_RegGbl_GetKeyPath_Str(char *path, char *buf, int bufsz);


int tk_sysc_exitpt()
{
	volatile int chk;
	volatile int *rchk;
	int i;
//	i=setjmp(tk_sysc_exit);
	if(!tk_sysc_exit)
		tk_sysc_exit=tk_malloc_krn(128*8);

	chk=999;
	rchk=&chk;
	*rchk=0x12345;

	i=__setj_sys(tk_sysc_exit);
	if(i)
	{
	}

	if(chk!=0x12345)
		__debugbreak();

	return(i);
}

void tk_sysc_exitpgm(int val)
{
//	__debugbreak();
//	longjmp(tk_sysc_exit, val|0x10000);
	__longj_sys(tk_sysc_exit, val|0x10000);
}

void *TK_DlGetApiContextA(u64 apiname, u64 subname);

// __declspec(syscall)
// int __isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)

int tk_syscall2(void *sObj, int uMsg, void *vParm1, void *vParm2);

// __declspec(dllexport)
TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	int i;
	
	i=0;
	if(vParm1)
		{ i+=*(int *)vParm1; }
	if(vParm2)
		{ i+=*(int *)vParm2; }

	return(tk_syscall2(sObj, uMsg, vParm1, vParm2));
//	TKPE_TaskInfo *task;
//	task=TK_GetCurrentTask();
//	return(TK_HandleSyscall(task, sObj, uMsg, vParm1, vParm2));
}

void tk_isr_syscall_rv();

#ifdef __BJX2__

__asm {
//called from RISC-V mode into BJX2 mode
tk_isr_syscall_rv:
	MOV		R10, R4
	MOV		R11, R5
	MOV		R12, R6
	MOV		R13, R7
	
	MOV		R18, R10
	MOV		R19, R11
	MOV		R20, R28
	MOV		R21, R29
	MOV		R22, R30
	MOV		R23, R31
	
	BSR		tk_isr_syscall

	MOV		R10, R18
	MOV		R11, R19
	MOV		R28, R20
	MOV		R29, R21
	MOV		R30, R22
	MOV		R31, R23
	
	MOV		R2, R10
	
	RTS
};

#endif

char *TK_SyscLnx_RemapPath(TKPE_TaskInfo *task, char *path)
{
	static char tbuf[512];
	char *s0;
	
	s0=path;

	if(!strncmp(s0, "/lib/", 5))
	{
		strcpy(tbuf, "/usr/lib/rv64/");
		strcat(tbuf, s0+5);
		s0=tbuf;
	}
#if 0
	else
		if(!strncmp(s0, "/usr/lib/", 9))
	{
		strcpy(tbuf, "/usr/lib/rv64/");
		strcat(tbuf, s0+9);
		s0=tbuf;
	}
#endif

	return(s0);
}


static const char *tk_lnxsc_syscallnames[512]={
	"IOSETUP",					"IODESTROY",				/*  0 */
	"IOSUBMIT",					"IOCANCEL",					/*  2 */
	"IOGETEVENTS",				"SEXTATTR",					/*  4 */
	"LSETXATTR",				"FSETXATTR",				/*  6 */
	"GETXATTR",					"LGETXATTR",				/*  8 */
	"FGETXATTR",				"LISTXATTR",				/* 10 */
	"LLISTXATTR",				"FLISTXATTR",				/* 12 */
	"REMOVEXATTR",				"LREMOVEXATTR",				/* 14 */
	"FREMOVEXATTR",				"GETCWD",					/* 16 */
	"LOOKUP_DCOOKIE",			"EVENTFD2",					/* 18 */
	"EPOLL_CREATE1",			"EPOLL_CTL",				/* 20 */
	"EPOLL_PWAIT",				"DUP",						/* 22 */
	"DUP3",						"FCNTL64",					/* 24 */
	"INOTIFY_INIT1",			"INOTIFY_ADD_WATCH",		/* 26 */	
	"INOTIFY_RM_WATCH",			"IOCTL",					/* 28 */
	"IOPRIO_SET",				"IOPRIO_GET",				/* 30 */
	"FLOCK",					"MKNODAT",					/* 32 */
	"MKDIRAT",					"UNLINKAT",					/* 34 */
	"SYMLINKAT",				"LINKAT",					/* 36 */
	"RENAMEAT",					"UMOUNT",					/* 38 */
	"MOUNT",					"PIVOT_ROOT",				/* 40 */
	"NI_SYSCALL",				"STATFS64",					/* 42 */
	"FSTATFS64",				"TRUNCATE64",				/* 44 */
	"FTRUNCATE64",				"FALLOCATE",				/* 46 */
	"FACCESSAT",				"CHDIR",					/* 48 */
	"FCHDIR",					"CHROOT",					/* 50 */
	"FCHMOD",					"FCHMODAT",					/* 52 */
	"FCHOWNAT",					"FCHOWN",					/* 54 */
	"OPENAT",					"CLOSE",					/* 56 */
	"VHANGUP",					"PIPE2",					/* 58 */
	"QUOTACTL",					"GETDENTS64",				/* 60 */
	"LSEEK",					"READ",						/* 62 */
	"WRITE",					"READV",					/* 64 */
	"WRITEV",					"PREAD64",					/* 66 */
	"PWRITE64",					"PREADV",					/* 68 */
	"PWRITEV",					"SENDFILE64",				/* 70 */
	"PSELECT6_TIME32",			"PPOLL_TIME32",				/* 72 */
	"SIGNALFD4",				"VMSPLICE",					/* 74 */
	"SPLICE",					"TEE",						/* 76 */
	"READLINKAT",				"NEWFSTATAT",				/* 78 */
	"NEWFSTAT",					"SYNC",						/* 80 */
	"FSYNC",					"FDATASYNC",				/* 82 */
	"SYNC_FILE_RANGE",			"TIMERFD_CREATE",			/* 84 */
	NULL,						NULL,						/* 86 */
	NULL,						"ACCT",						/* 88 */
	"CAPGET",					"CAPSET",					/* 90 */
	"PERSONALITY",				"EXIT",						/* 92 */
	"EXIT_GROUP",				"WAITID",					/* 94 */
	"SET_TID_ADDRESS",			"UNSHARE",					/* 96 */
	"FUTEX",					"SET_ROBUST_LIST",			/* 98 */
	"GET_ROBUST_LIST",			"NANOSLEEP",				/* 100 */
	"GETITIMER",				"SETITIMER",				/* 102 */
	"KEXEC_LOAD",				"INIT_MODULE",				/* 104 */
	"DELETE_MODULE",			"TIMER_CREATE",				/* 106 */
	NULL,						"TIMER_GETOVERRUN",			/* 108 */
	NULL,						"TIMER_DELETE",				/* 110 */
	NULL,						NULL,						/* 112 */
	NULL,						NULL,						/* 114 */
	"SYSLOG",					"PTRACE",					/* 116 */
	"SCHED_SETPARAM",			"SCHED_SETSCHEDULER",		/* 118 */
	"SCHED_GETSCHEDULER",		"SCHED_GETPARAM",			/* 120 */
	"SCHED_SETAFFINITY",		"SCHED_GETAFFINITY",		/* 122 */
	"SCHED_YIELD",				"SCHED_GET_PRIORITY_MAX",	/* 124 */
	"SCHED_GET_PRIORITY_MIN",	NULL,						/* 126 */
	"RESTART_SYSCALL",			"KILL",						/* 128 */
	"TKILL",					"TGKILL",					/* 130 */
	"SIGALTSTACK",				"RT_SIGSUSPEND",			/* 132 */
	"RT_SIGACTION",				"RT_SIGPROCMASK",			/* 134 */
	"RT_SIGPENDING",			"RT_SIGTIMEDWAIT_TIME32",	/* 136 */
	"RT_SIGQUEUEINFO",			NULL,						/* 138 */
	"SETPRIORITY",				"GETPRIORITY",				/* 140 */
	"REBOOT",					"SETREGID",					/* 142 */
	"SETGID",					"SETREUID",					/* 144 */
	"SETUID",					"SETRESUID",				/* 146 */
	"GETRESUID",				"SETRESGID",				/* 148 */
	"GETRESGID",				"SETFSUID",					/* 150 */
	"SETFSGID",					"TIMES",					/* 152 */
	"SETPGID",					"GETPGID",					/* 154 */
	"GETSID",					"SETSID",					/* 156 */
	"GETGROUPS",				"SETGROUPS",				/* 158 */
	"NEWUNAME",					"SETHOSTNAME",				/* 160 */
	"SETDOMAINNAME",			"GETRLIMIT",				/* 162 */
	"SETRLIMIT",				"GETRUSAGE",				/* 164 */
	"UMASK",					"PRCTL",					/* 166 */
	"GETCPU",					"GETTIMEOFDAY",				/* 168 */
	"SETTIMEOFDAY",				"ADJTIMEX",					/* 170 */
	"GETPID",					"GETPPID",					/* 172 */
	"GETUID",					"GETEUID",					/* 174 */
	"GETGID",					"GETEGID",					/* 176 */
	"GETTID",					"SYSINFO",					/* 178 */
	"MQ_OPEN",					"MQ_UNLINK",				/* 180 */
	NULL,						NULL,						/* 182 */
	"MQ_NOTIFY",				"MQ_GETSETATTR",			/* 184 */
	"MSGGET",					"MSGCTL",					/* 186 */
	"MSGRCV",					"MSGSND",					/* 188 */
	"SEMGET",					"SEMCTL",					/* 190 */
	NULL,						"SEMOP",					/* 192 */
	"SHMGET",					"SHMCTL",					/* 194 */
	"SHMAT",					"SHMDT",					/* 196 */
	"SOCKET",					"SOCKETPAIR",				/* 198 */
	"BIND",						"LISTEN",					/* 200 */
	"ACCEPT",					"CONNECT",					/* 202 */
	"GETSOCKNAME",				"GETPEERNAME",				/* 204 */
	"SENDTO",					"RECVFROM",					/* 206 */
	"SETSOCKOPT",				"GETSOCKOPT",				/* 208 */
	"SHUTDOWN",					"SENDMSG",					/* 210 */
	"RECVMSG",					"READAHEAD",				/* 212 */
	"BRK",						"MUNMAP",					/* 214 */
	"MREMAP",					"ADD_KEY2",					/* 216 */
	"REQUEST_KEY",				"KEYCTL",					/* 218 */
	"CLONE",					"EXECVE",					/* 220 */
	"MMAP",						"FADVISE64_64",				/* 222 */
	"SWAPON",					"SWAPOFF",					/* 224 */
	"MPROTECT",					"MSYNC",					/* 226 */
	"MLOCK",					"MUNLOCK",					/* 228 */
	"MLOCKALL",					"MUNLOCKALL",				/* 230 */
	"MINCORE",					"MADVISE",					/* 232 */
	"REMAP_FILE_PAGES",			"MBIND",					/* 234 */
	"GET_MEMPOLICY",			"SET_MEMPOLICY",			/* 236 */
	"MIGRATE_PAGES",			"MOVE_PAGES",				/* 238 */
	"RT_TGSIGQUEUEINFO",		"PERF_EVENT_OPEN",			/* 240 */
	"ACCEPT4",					"RECVMMSG_TIME32",			/* 242 */
	NULL,						NULL,						/* 244 */
	NULL,						NULL,						/* 246 */
	NULL,						NULL,						/* 248 */
	NULL,						NULL,						/* 250 */
	NULL,						NULL,						/* 252 */
	NULL,						NULL,						/* 254 */
	NULL,						NULL,						/* 256 */
	NULL,						NULL,						/* 258 */
	"WAIT4",					"PRLIMIT64",				/* 260 */
	"FANOTIFY_INIT",			"FANOTIFY_MARK",			/* 262 */
	"NAME_TO_HANDLE_AT",		"OPEN_BY_HANDLE_AT",		/* 264 */
	NULL,						"SYNCFS",					/* 266 */
	"SETNS",					"SENDMMSG",					/* 268 */
	"PROCESS_VM_READV",			"PROCESS_VM_WRITEV",		/* 270 */
	"KCMP",						"FINIT_MODULE",				/* 272 */
	"SCHED_SETATTR",			"SCHED_GETATTR",			/* 274 */
	"RENAMEAT2",				"SECCOMP",					/* 276 */
	"GETRANDOM",				"MEMFD_CREATE",				/* 278 */
	"BPF",						"EXECVEAT",					/* 280 */
	"USERFAULTFD",				"MEMBARRIER",				/* 282 */
	"MLOCK2",					"COPY_FILE_RANGE",			/* 284 */
	"PREADV2",					"PWRITEV2",					/* 286 */
	"PKEY_MPROTECT",			"PKEY_ALLOC",				/* 288 */
	"PKEY_FREE",				"STATX",					/* 290 */
	"IO_PGETEVENTS",			"RSEQ",						/* 292 */
	"KEXEC_FILE_LOAD",			NULL,						/* 294 */
	NULL,						NULL,						/* 296 */
	NULL,						NULL,						/* 298 */
	NULL,						NULL,						/* 300 */
	NULL,						NULL,						/* 302 */
	NULL,						NULL,						/* 304 */
	NULL,						NULL,						/* 306 */
	NULL,						NULL,						/* 308 */
	NULL,						NULL,						/* 310 */
	NULL,						NULL,						/* 312 */
	NULL,						NULL,						/* 314 */
	NULL,						NULL,						/* 316 */
	NULL,						NULL,						/* 318 */
	NULL,						NULL,						/* 320 */
	NULL,						NULL,						/* 322 */
	NULL,						NULL,						/* 324 */
	NULL,						NULL,						/* 326 */
	NULL,						NULL,						/* 328 */
	NULL,						NULL,						/* 330 */
	NULL,						NULL,						/* 332 */
	NULL,						NULL,						/* 334 */
	NULL,						NULL,						/* 336 */
	NULL,						NULL,						/* 338 */
	NULL,						NULL,						/* 340 */
	NULL,						NULL,						/* 342 */
	NULL,						NULL,						/* 344 */
	NULL,						NULL,						/* 346 */
	NULL,						NULL,						/* 348 */
	NULL,						NULL,						/* 350 */
	NULL,						NULL,						/* 352 */
	NULL,						NULL,						/* 354 */
	NULL,						NULL,						/* 356 */
	NULL,						NULL,						/* 358 */
	NULL,						NULL,						/* 360 */
	NULL,						NULL,						/* 362 */
	NULL,						NULL,						/* 364 */
	NULL,						NULL,						/* 366 */
	NULL,						NULL,						/* 368 */
	NULL,						NULL,						/* 370 */
	NULL,						NULL,						/* 372 */
	NULL,						NULL,						/* 374 */
	NULL,						NULL,						/* 376 */
	NULL,						NULL,						/* 378 */
	NULL,						NULL,						/* 380 */
	NULL,						NULL,						/* 382 */
	NULL,						NULL,						/* 384 */
	NULL,						NULL,						/* 386 */
	NULL,						NULL,						/* 388 */
	NULL,						NULL,						/* 390 */
	NULL,						NULL,						/* 392 */
	NULL,						NULL,						/* 394 */
	NULL,						NULL,						/* 396 */
	NULL,						NULL,						/* 398 */
	NULL,						NULL,						/* 400 */
	NULL,						"CLOCK_GETTIME",			/* 402 */
	"CLOCK_SETTIME",			"CLOCK_ADJTIME",			/* 404 */
	"CLOCK_GETRES",				"CLOCK_NANOSLEEP",			/* 406 */
	"TIMER_GETTIME",			"TIMER_SETTIME",			/* 408 */
	"TIMERFD_GETTIME",			"TIMERFD_SETTIME",			/* 410 */
	"UTIMENSAT",				NULL,						/* 412 */
	NULL,						NULL,						/* 416 */
	"MQ_TIMEDSEND",				"MQ_TIMEDRECEIVE",			/* 418 */
	"SEMTIMEDOP",				NULL,						/* 420 */
	"FUTEX",					"SCHED_RR_GET_INTERVAL",	/* 422 */
	"PIDFD_SEND_SIGNAL",		"IO_URING_SETUP",			/* 424 */
	"IO_URING_ENTER",			"IO_URING_REGISTER",		/* 426 */
	"OPEN_TREE",				"MOVE_MOUNT",				/* 428 */
	"FSOPEN",					"FSCONFIG",					/* 430 */
	"FSMOUNT",					"FSPICK",					/* 432 */
	"PIDFD_OPEN",				"CLONE3",					/* 434 */
	"CLOSE_RANGE",				"OPENAT2",					/* 436 */
	"PIDFD_GETFD",				"FACCESSAT2",				/* 438 */
	"PROCESS_MADVISE",			NULL,						/* 440 */
	};

char *TK_SyscLnx_NameForSyscall(TKPE_TaskInfo *task, int idx)
{
	static char tb[64];
	char *scn;
	
	scn=NULL;
	if((idx>=0) && (idx<512))
		scn=tk_lnxsc_syscallnames[idx];

	if(!scn)
	{
		tk_sprintf(tb, "SC%u", idx);
		scn=tb;
	}
	
	return(scn);
}

s64 TK_HandleSyscallLnx(TKPE_TaskInfo *task,
	int uMsg, TK_SysArg *args)
{
	char tbuf[512];
	TK_FSTAT tstat;
	TKPE_TaskInfoKern *taskern;
	TK_EnvContext *env;
	s64 ret, p0, p1, p2, p3;
	char *s0, *s1;
	int sz, dfd, fd, fl, md, n;
	int i, j, k, l;

	taskern=(TKPE_TaskInfoKern *)(task->krnlptr);
	env=(void *)task->envctx;

	ret=-1;

	tk_dbg_printf("TK_HandleSyscallLnx: %d %s\n", uMsg, 
		TK_SyscLnx_NameForSyscall(task, uMsg));

	if(sizeof(TK_FSTAT)!=128)
		{ __debugbreak(); }

	switch(uMsg)
	{
	case TK_SCLNX_GETCWD:
		sz=TK_EnvCtx_GetEnvVar(env, "PWD", args[0].p, args[1].i);
		ret=sz;
		break;
	case TK_SCLNX_CHDIR:
		break;
	
	case TK_SCLNX_OPENAT:
		dfd=args[0].i;
		s0=args[1].p;
		fl=args[2].i;
		md=args[3].i;

		tk_dbg_printf("  openat: %d %s %08X %08X\n", dfd, s0, fl, md);
		
		if(!s0)
			break;
			
		s0=TK_SyscLnx_RemapPath(task, s0);

		tk_dbg_printf("  openat %s\n", s0);
		
		s1="rb";
		fd=tk_hfopen(task, s0, s1);
		if(fd<=0)
		{
			tk_dbg_printf("    open failed\n", fd);
			ret=-ENOENT;
			break;
		}
		
//		taskern->lclfd[dfd]=fd;
		tk_dbg_printf("  fd=%d\n", fd);

//		__debugnop(0x100);

		ret=fd;
		break;

	case TK_SCLNX_CLOSE:
		tk_hclose(task, args[0].i);
		ret=0;
		break;

	case TK_SCLNX_LSEEK:
		ret=tk_hseek(task,
			args[0].i, args[2].l, args[4].i);
		*(s64 *)(args[3].p)=ret;
		break;

	case TK_SCLNX_READ:
		ret=tk_hread(task, args[0].i, args[1].p, args[2].i);
		break;
	case TK_SCLNX_WRITE:
		if(args[0].i<3)
		{
			tk_puts_n(args[1].p, args[2].i);
			ret=args[2].i;
			break;
		}
		ret=tk_hwrite(task, args[0].i, args[1].p, args[2].i);
		break;
	
	case TK_SCLNX_WRITEV:
		p0=args[1].p;
		n=args[2].i;
		
		if(args[0].i<3)
		{
//			__debugbreak();
		
			if(n<=0)
			{
				ret=0;
				break;
			}

			ret=0;
			for(i=0; i<n; i++)
			{
				p2=((u64 *)p0)[i*2+0];
				p3=((u64 *)p0)[i*2+1];
				j=p3;
				if(j<=0)
					continue;
				tk_puts_n((char *)p2, j);
				ret+=j;
			}

//			tk_puts_n(args[1].p, args[2].i);
//			ret=args[2].i;
			break;
		}
		break;

	case TK_SCLNX_MMAP:
		p0=args[0].p;
		sz=args[1].i;
		md=args[2].i;
		fl=args[3].i;
		fd=args[4].i;
		p1=args[5].i;

		tk_dbg_printf("  mmap %p %d %04X %04X %d %p\n", p0, sz, md, fl, fd, p1);
		
		md|=TKMM_PROT_ALLOW;
		
		p2=tk_mmap2(task, p0, sz, md, fl, fd, p1);

		tk_dbg_printf("  mmap -> %p\n", p2);

		ret=p2;
		break;

	case TK_SCLNX_MPROTECT:
		p0=args[0].p;
		sz=args[1].i;
		md=args[2].i;

		md|=TKMM_PROT_ALLOW;
		tk_mprotect2(task, p0, sz, md);
		ret=0;
		break;

	case TK_SCLNX_FACCESSAT:
		s0=args[1].p;
		fl=args[2].i;
		md=args[3].i;
		
		if(!s0)
			break;
		
		s0=TK_SyscLnx_RemapPath(task, s0);

		tk_dbg_printf("  faccessat %s\n", s0);

		i=tk_fstat(s0, &tstat);
		if(i<0)
		{
			ret=-ENOENT;
			break;
		}
		ret=0;
		break;
	
	case TK_SCLNX_NEWFSTAT:
//		s0=args[1].p;
//		fl=args[2].i;
//		md=args[3].i;

		k=args[0].i;
		
		if(k<2)
		{
			ret=-1;
			break;
		}
		
		p0=args[1].p;
		
		memset(&tstat, 0, sizeof(TK_FSTAT));
		tstat.st_ino=args[0].i;		//for now
		tstat.st_mode=0100755;
		
		p1=tk_hseek(task, k, 0, 1);
		p2=tk_hseek(task, k, 0, 2);
		tk_hseek(task, k, p1, 0);
		tstat.st_size=p2;
		tstat.st_blksize=512;
		tstat.st_blocks=(p2+511)>>9;
		
		memcpy(p0, &tstat, 128);
		
		ret=0;
		break;

	case TK_SCLNX_NEWFSTATAT:
		k=args[0].i;
		s0=args[1].p;
		p0=args[2].p;
		fl=args[3].i;

//		__debugbreak();

		tk_dbg_printf("  newfstatat: %d %s %p %08X\n", k, s0, p0, fl);
		
		if(!s0)
			break;
		
		s0=TK_SyscLnx_RemapPath(task, s0);

		tk_dbg_printf("  newfstatat %s\n", s0);

		memset(&tstat, 0, 128);
//		memcpy(&tstat, p0, 128);
		i=tk_fstat(s0, &tstat);
		if(i<0)
		{
			tk_dbg_printf("    stat failed\n", fd);
			ret=-ENOENT;
			break;
		}
		memcpy(p0, &tstat, 128);

		tk_dbg_printf("    st_mode %08X\n", tstat.st_mode);
		
		for(i=0; i<16; i++)
		{
			tk_dbg_printf("    %02X: %016llX\n", i*8, ((u64 *)p0)[i]);
		}

//		__debugnop(0x100);

#if 0
		ret=-ENOENT;
		break;
#endif

		ret=0;
		break;

	case TK_SCLNX_SET_TID_ADDRESS:
		/* Ignore for now. Sets an address to be cleared when a thread exits. */
		p0=args[0].p;
		if(p0)
			*(u32 *)p0=0;
		ret=task->pid;
		break;
	case TK_SCLNX_SET_ROBUST_LIST:
		/* Ignore for now. Sets list of futexes. */
		ret=0;
		break;

	case TK_SCLNX_PRLIMIT64:
		/* Ignore for now. Sets resource limits. */
		ret=0;
		break;
	
	case TK_SCLNX_RSEQ:
		ret=-ENOSYS;
		break;
	
	case TK_SCLNX_RISCV_HWPROBE:
		s1=args[0].p;
		memset(s1, 0, 16);
		ret=0;
		break;
	
	case TK_SCLNX_BRK:
		s1=args[0].p;
		if(!s1)
		{
			if(!taskern->brk_base)
			{
				s0=tk_mmap2(task, NULL, 1<<20,
					TKMM_PROT_RWX, TKMM_MAP_PRIVATE, -1, 0);
				taskern->brk_base=(tk_kptr)s0;
				taskern->brk_end=(tk_kptr)s0+(1<<20);
				taskern->brk_cur=(tk_kptr)s0;
			}
			
			ret=taskern->brk_cur;
			break;
		}
		
		p0=(tk_kptr)s1;
		if((p0>=taskern->brk_base) && (p0<taskern->brk_end))
		{
			taskern->brk_cur=p0;
			ret=p0;
			break;
		}
		
		ret=taskern->brk_cur;
		
//		taskern->brk_base=(tk_kptr)s0;
//		taskern->brk_end=(tk_kptr)s0+(1<<20);
//		taskern->brk_cur=(tk_kptr)s0;
		break;

	case TK_SCLNX_FUTEX2:
		p0=args[0].p;
		fl=args[1].i;
		md=args[2].i;
		p1=args[3].p;
		p2=args[4].p;
		fd=args[5].i;

		tk_dbg_printf("    futex %p %08X %08X %p %p %08X\n",
			p0, fl, md, p1, p2, fd);

//		*(u32 *)p0=0;

		if((fl&0x7F)==0)
		{
			*(u32 *)p0=0;
//			*(u32 *)p0=md;
			ret=0;
			break;
		}

		if((fl&0x7F)==1)
		{
//			*(u32 *)p0=md;
			ret=0;
			break;
		}

//		if(fl&0x80)
//		{
//			return(-ENOSYS);
//		}

		ret=0;
		break;

	case TK_SCLNX_GETTID:
		ret=task->pid;
		break;

	case TK_SCLNX_GETPID:
		ret=task->pid;
		break;

	case TK_SCLNX_TGKILL:
		fd=args[0].i;
		dfd=args[1].i;
		md=args[2].i;
		tk_dbg_printf("    tgkill %d %d %d\n",
			fd, dfd, md);

		ret=0;
		break;

#if 1
	case TK_SCLNX_RT_SIGPROCMASK:
		fd=args[0].i;
		p0=args[1].p;
		p1=args[2].p;
		tk_dbg_printf("    sigprocmask %d %p %p\n",
			fd, p0, p1);
		ret=0;
		break;
	case TK_SCLNX_RT_SIGACTION:
		fd=args[0].i;
		p0=args[1].p;
		p1=args[2].p;
		tk_dbg_printf("    sigaction %d %p %p\n",
			fd, p0, p1);
		ret=0;
		break;
#endif

	case TK_SCLNX_GETRANDOM:
		s1=args[0].p;
		sz=args[1].i;
		
		for(i=0; i<sz; i++)
			s1[i]=rand();
//		memset(s1, 0, 16);
		ret=0;
		break;

	case TK_SCLNX_CLOCK_GETTIME:
	case TK_SCLNX_CLOCK_GETTIME2:
		fd=args[0].i;
		p0=args[1].p;
		((u64 *)p0)[0]=TK_GetTimeUs()>>20;
		((u64 *)p0)[1]=(TK_GetTimeUs()&0xFFFFF)*953;
		ret=0;
		break;

	default:
		tk_dbg_printf("TK_HandleSyscallLnx: Unhandled %d\n", uMsg);
		break;
	}
	
	return(ret);
}

s64 TK_HandleSyscall(TKPE_TaskInfo *task,
	void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	TKPE_TaskInfo *newtask;
	TK_EnvContext *env;
	TK_SysArg *args;
	s64 li;
	char *s0;
	void *p;
	s64 ret;
	int sz;

//	__debugbreak();

//	u64 ttb, tea;
//	u16 exsr;
	
//	ttb=__arch_ttb;
//	tea=__arch_tea;
//	exsr=(u16)(__arch_exsr);
//	ptetlb_rethow_exc=0;

//	tk_dbg_printf("SYSC: uMsg=%X vParm1=%p, vParm2=%p\n", uMsg, vParm1, vParm2);

//	task=TK_GetCurrentTask();
	env=(void *)task->envctx;

	ret=TK_URES_ERR_BADMSG;
	args=(TK_SysArg *)vParm2;
	
	switch((uMsg>>12)&15)
	{
	case 0:
		if(!sObj || (uMsg<2) || (uMsg>4095))
		{
			tk_dbg_printf("SYSC: COMGLUE Bad Req "
				"sObj=%p uMsg=%d vParm1=%p, vParm2=%p\n",
					sObj, uMsg, vParm1, vParm2);
			ret=TK_URES_FALSE;
			break;
		}
		/* COMGLUE */
		TKGDI_ComGlueDispatch(task,
			sObj, uMsg&4095,
			vParm1, vParm2);
		ret=TK_URES_TRUE;
		break;

	case 1:
		switch((uMsg>>8)&15)
		{
		case 0:
			switch(uMsg&255)
			{
			case 0x01:
				p=tk_con_getctx();
				*((void **)vParm1)=p;
				tk_dbg_printf("SYSC: ConGetCtx vParm=%p, p=%p\n", vParm1, p);
				ret=TK_URES_TRUE;
				break;
			case 0x02:
				tk_con_reset();
				ret=TK_URES_TRUE;
				break;
			
			case 0x03:
				__debugbreak();
//				tk_sysc_exitpgm(args[0].i);
				ret=TK_URES_TRUE;
				break;

			case 0x04:
				sz=args[0].i;
//				if(sz<=0)
//					{ __debugbreak(); }

				if(sz<=0)
				{
					tk_dbg_printf("SYSC: Page Alloc, Bad Size, "
						"vParm1=%p, vParm2=%p, sz=%08X\n",
						vParm1, vParm2, sz);
					*((void **)vParm1)=NULL;
					ret=TK_URES_TRUE;
					break;
				}

				p=TKMM_PageAllocUsc(sz);
				*((void **)vParm1)=p;

				if(!p)
				{
					tk_dbg_printf("SYSC: Page Alloc, Failed Alloc, "
						"vParm1=%p, vParm2=%p, sz=%08X\n",
						vParm1, vParm2, sz);
					*((void **)vParm1)=NULL;
					ret=TK_URES_TRUE;
					break;
				}

				TK_TaskAddPageAlloc(task, p, sz);
//				tk_dbg_printf("SYSC: Page Alloc, vParm=%p, p=%p, sz=%08X\n", 
//					vParm1, p, sz);

				ret=TK_URES_TRUE;
				break;
			case 0x05:
				ret=TKMM_PageFreeUsc(args[0].p, args[1].i);
				break;
			
			case 0x06:
				tk_irq_setUserIrq(args[0].i, args[1].p);
				ret=TK_URES_TRUE;
				break;

			case 0x07:
				p=TK_WithKrrSetuidB(args[0].p, args[1].i);
				*((void **)vParm1)=p;
				ret=TK_URES_TRUE;
				break;

			case 0x08:
				if(task->redir_stdout)
				{
					tk_hputc(task, task->redir_stdout, args[0].i);
					ret=TK_URES_TRUE;
					break;
				}
			
				if(task->ttyid)
				{
					tk_dbg_putc(args[0].i);
					tk_putc_tty(args[0].i, task->ttyid);
					ret=TK_URES_TRUE;
					break;
				}
				tk_putc(args[0].i);
				ret=TK_URES_TRUE;
				break;
			case 0x09:
				if(task->redir_stdin)
				{
					sz=tk_hgetc(task, task->redir_stdin);
					if(sz<0)
						task->stdin_eof=1;
					*((int *)vParm1)=sz;
					ret=TK_URES_TRUE;
					break;
				}
			
				if(task->ttyid)
				{
					sz=tk_getch_tty(task->ttyid);
					*((int *)vParm1)=sz;
					ret=TK_URES_TRUE;
					break;
				}
				
				if(args[0].i==0)
				{
					sz=tk_getch();
					*((int *)vParm1)=sz;
				}else
					if(args[0].i==1)
				{
					li=TK_GetTimeUs();
					*((s64 *)vParm1)=li;
				}else
				{
					sz=0;
					if(tk_kbhit())
						sz=tk_getch();
					*((int *)vParm1)=sz;
				}
				ret=TK_URES_TRUE;
				break;
			case 0x0A:
				if(task->redir_stdin)
				{
					sz=!(task->stdin_eof);
					*((int *)vParm1)=sz;
					ret=TK_URES_TRUE;
					break;
				}

				if(task->ttyid)
				{
					sz=tk_kbhit_tty(task->ttyid);
					*((int *)vParm1)=sz;
					ret=TK_URES_TRUE;
					break;
				}
				
				if(args[0].i==0)
				{
					sz=tk_kbhit();
					*((int *)vParm1)=sz;
				}else
					if(args[0].i==1)
				{
					li=TK_GetTimeUs();
					*((s64 *)vParm1)=li;
				}else
				{
					sz=tk_kbhit();
					*((int *)vParm1)=sz;
				}
				ret=TK_URES_TRUE;
				break;
			
			case 0x0B:
				sz=TK_EnvCtx_GetEnvVarIdx(env, args[0].i,
					args[1].p, args[2].p, args[3].i, args[4].i);
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x0C:
				s0=args[0].p;
				if(*s0=='/')
				{
					sz=TK_RegGbl_GetKeyPath_Str(s0, args[1].p, args[2].i);
					*((int *)vParm1)=sz;
					ret=TK_URES_TRUE;
					break;
				}
				
				sz=TK_EnvCtx_GetEnvVar(env, args[0].p, args[1].p, args[2].i);
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x0D:
				s0=args[0].p;
				if(*s0=='/')
				{
					sz=TK_RegGbl_SetKeyPath_Str(s0, args[1].p);
					*((int *)vParm1)=sz;
					ret=TK_URES_TRUE;
					break;
				}
				
				sz=TK_EnvCtx_SetEnvVar(env, args[0].p, args[1].p);
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x0E:
				if(task->redir_stdout)
				{
					tk_dbg_puts_n(args[0].p, args[1].i);
					tk_hwrite(task, task->redir_stdout, args[0].p, args[1].i);
					ret=TK_URES_TRUE;
					break;
				}
			
				if(task->ttyid)
				{
					tk_dbg_puts_n(args[0].p, args[1].i);
//					tk_puts_tty(args[0].p, task->ttyid);
					tk_puts_n_tty(args[0].p, args[1].i, task->ttyid);
					ret=TK_URES_TRUE;
					break;
				}
				tk_puts_n(args[0].p, args[1].i);
				ret=TK_URES_TRUE;
				break;

			case 0x10:
				p=tk_mmap2(task,
					args[0].p, args[1].l, args[2].i, args[3].i,
					args[4].i, args[5].l);
				*((void **)vParm1)=p;
				break;
			case 0x11:
				sz=tk_munmap2(task, args[0].p, args[1].l);
				*((int *)vParm1)=sz;
				break;
			case 0x12:
				sz=tk_mprotect2(task, args[0].p, args[1].l, args[2].i);
				*((int *)vParm1)=sz;
				break;
			case 0x13:
				sz=tk_msync2(task, args[0].p, args[1].l, args[2].i);
				*((int *)vParm1)=sz;
				break;

			case 0x14:
				sz=tk_mlock2(task, args[0].p, args[1].l);
				*((int *)vParm1)=sz;
				break;
			case 0x15:
				sz=tk_munlock2(task, args[0].p, args[1].l);
				*((int *)vParm1)=sz;
				break;
			case 0x16:
				sz=tk_mlockall2(task, args[1].i);
				*((int *)vParm1)=sz;
				break;
			case 0x17:
				sz=tk_munlockall2(task);
				*((int *)vParm1)=sz;
				break;

			case 0x18:
				TK_YieldCurrentThreadB(args[0].l);
				ret=TK_URES_TRUE;
				break;
			case 0x19:
				sz=TK_AllocNewTlsB(task);
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;

			case 0x1A:
				newtask=TK_SpawnNewThread2B(task, args[0].p,
					args[1].p, args[2].p);
				if(newtask)
					{ sz=newtask->pid; }
				else
					{ sz=-1; }
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x1B:
				TK_SuspendThreadB(args[0].i, args[1].l);
				ret=TK_URES_TRUE;
				break;
			case 0x1C:
				li=TK_GetThreadStatusB(args[0].i);
				*((s64 *)vParm1)=li;
				ret=TK_URES_TRUE;
				break;

			case 0x1D:
//				p=TK_DlGetApiContextB(task, args[0].l, args[1].l);
				p=TK_DlGetApiContextWrapB(task, args[0].l, args[1].l);
				*((void **)vParm1)=p;
				ret=TK_URES_TRUE;
				break;
			case 0x1E:
				p=TK_DlOpenB(task, args[0].p, args[1].i);
				*((void **)vParm1)=p;
				ret=TK_URES_TRUE;
				break;
			case 0x1F:
				p=TK_DlSymB(task, args[0].p, args[1].p, args[2].i);
				*((void **)vParm1)=p;
				tk_printf("Syscall: DlSym %p\n", p);
				ret=TK_URES_TRUE;
				break;

			case 0x20:
				ret=tk_hfopen(task, args[0].p, args[1].p);
				break;
			case 0x21:
				ret=tk_hopendir(task, args[0].p);
				break;
			case 0x22:
				ret=tk_hunlink(task, args[0].p);
				break;
			case 0x23:
				ret=tk_hrename(task, args[0].p, args[1].p, args[2].p);
				break;
			case 0x24:
				ret=tk_hfstat(task, args[0].p, args[1].p);
				break;
			case 0x25:
				ret=tk_fmount(args[0].p, args[1].p, args[2].p,
					args[3].p, args[4].p);
				break;
			case 0x26:
				ret=tk_hreaddir(task,
					args[0].i, args[1].p, args[2].i, args[3].i);
				break;
			case 0x27:
				ret=tk_hclosedir(task, args[0].i);
				break;
			case 0x28:
				ret=tk_hread(task, args[0].i, args[1].p, args[2].i);
				break;
			case 0x29:
				ret=tk_hwrite(task, args[0].i, args[1].p, args[2].i);
				break;
			case 0x2A:
				*((s64 *)vParm1)=tk_hseek(task,
					args[0].i, args[1].l, args[2].i);
				ret=TK_URES_TRUE;
				break;
			case 0x2B:
				ret=tk_hclose(task, args[0].i);
				break;
			case 0x2C:
				ret=tk_hioctl(task, args[0].i, args[1].i, args[2].p);
				break;
			case 0x2D:
				ret=tk_hfsctl(task, args[0].p, args[1].i, args[2].p);
				break;
			case 0x2E:
				ret=tk_hmkdir(task, args[0].p, args[1].p);
				break;
			case 0x2F:
				ret=tk_hrmdir(task, args[0].p);
				break;
			case 0x30:
				ret=tk_hsend(task, args[0].i, args[1].i,
					args[2].p, args[3].i, args[4].i,
					args[5].p, args[6].i);
				break;
			case 0x31:
				ret=tk_hrecv(task, args[0].i, args[1].i,
					args[2].p, args[3].i, args[4].i,
					args[5].p, args[6].i);
				break;

			case 0x40:
				ret=TK_CreateProcessB(task,
					args[0].p, args[1].p,
					args[2].p, args[3].p,
					args[4].i, args[5].p);
				*((s32 *)vParm1)=ret;
				break;

			case 0x41:
				*((s64 *)vParm1)=TK_Task_TryJoinOnReturnPid(args[0].i);
				ret=TK_URES_TRUE;
				break;

			case 0x42:
				*((s64 *)vParm1)=TK_MutexTryLockB(task, args[0].p, args[1].i);
				ret=TK_URES_TRUE;
				break;
			case 0x43:
				*((s64 *)vParm1)=TK_MutexTryReleaseB(task, args[0].p);
				ret=TK_URES_TRUE;
				break;

			default:
				tk_printf("SYSC: BAD sObj=%p, uMsg=%X, vParm1=%p, vParm1=%p\n",
					sObj, uMsg, vParm1, vParm2);
				break;
			}
			break;

		case 2:
		case 3:
			/* COMGLUE */
			TKGDI_ComGlueDispatch(task,
				sObj, uMsg-TK_UMSG_COMGLUE_VMT0,
				vParm1, vParm2);
			break;

		case 8:		case 9:
		case 10:	case 11:
			ret=TK_HandleSyscallLnx(task, uMsg-TK_UMSG_LNXSC, vParm2);
//			tk_dbg_printf("SYSC: Lnx uMsg=%X res=%X\n", uMsg, ret);
			break;

		default:
			tk_printf("SYSC: BAD A sObj=%p, uMsg=%X, vParm1=%p, vParm1=%p\n",
				sObj, uMsg, vParm1, vParm2);
			break;
		}
		break;
	default:
		tk_printf("SYSC: BAD B sObj=%p, uMsg=%X, vParm1=%p, vParm1=%p\n",
			sObj, uMsg, vParm1, vParm2);
		break;
	}
	
	return(ret);
}
