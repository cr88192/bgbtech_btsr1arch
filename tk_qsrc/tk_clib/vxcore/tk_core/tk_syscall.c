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

s64 TK_HandleSyscallLnx(TKPE_TaskInfo *task,
	int uMsg, TK_SysArg *args)
{
	TKPE_TaskInfoKern *taskern;
	TK_EnvContext *env;
	s64 ret;
	char *s0, *s1;
	int sz, dfd, fd, fl, md;

	taskern=(TKPE_TaskInfoKern *)(task->krnlptr);
	env=(void *)task->envctx;

	ret=-1;

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
		s1="r+b";
		fd=tk_hfopen(task, s0, s1);
//		taskern->lclfd[dfd]=fd;
		ret=fd;
		break;

	case TK_SCLNX_CLOSE:
		tk_hclose(task, args[0].i);
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
	
	case TK_SCLNX_SET_TID_ADDRESS:
		/* Ignore for now. Sets an address to be cleared when a thread exits. */
		ret=0;
		break;
	
	default:
		tk_dbg_printf("TK_HandleSyscallLnx: Unhandled %04X\n", uMsg);
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
