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

jmp_buf	tk_sysc_exit;

int tk_sysc_exitpt()
{
	int i;
	i=setjmp(tk_sysc_exit);
	if(i)
	{
//		__debugbreak();
	}
	return(i);
}

void tk_sysc_exitpgm(int val)
{
//	__debugbreak();
	longjmp(tk_sysc_exit, val|0x10000);
}

// __declspec(syscall)
// int __isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)

// __declspec(dllexport)
TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	TKPE_TaskInfo *task;
	TK_EnvContext *env;
	TK_SysArg *args;
	s64 li;
	void *p;
	int ret, sz;

//	__debugbreak();

//	u64 ttb, tea;
//	u16 exsr;
	
//	ttb=__arch_ttb;
//	tea=__arch_tea;
//	exsr=(u16)(__arch_exsr);
//	ptetlb_rethow_exc=0;

//	tk_printf("SYSC: uMsg=%X vParm1=%p, vParm2=%p\n", uMsg, vParm1, vParm2);

	task=TK_GetCurrentTask();
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
				tk_printf("SYSC: ConGetCtx vParm=%p, p=%p\n", vParm1, p);
				ret=TK_URES_TRUE;
				break;
			case 0x02:
				tk_con_reset();
				ret=TK_URES_TRUE;
				break;
			
			case 0x03:
				tk_sysc_exitpgm(args[0].i);
				ret=TK_URES_TRUE;
				break;

			case 0x04:
				sz=args[0].i;
				p=TKMM_PageAlloc(sz);
				*((void **)vParm1)=p;
				TK_TaskAddPageAlloc(task, p, sz);
//				tk_printf("SYSC: Page Alloc, vParm=%p, p=%p\n", vParm1, p);
				ret=TK_URES_TRUE;
				break;
			case 0x05:
				ret=TKMM_PageFree(args[0].p, args[1].i);
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
				tk_putc(args[0].i);
				ret=TK_URES_TRUE;
				break;
			case 0x09:
				sz=tk_getch();
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x0A:
				sz=tk_kbhit();
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			
			case 0x0B:
				sz=TK_EnvCtx_GetEnvVarIdx(env, args[0].i,
					args[1].p, args[2].p, args[3].i, args[4].i);
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x0C:
				sz=TK_EnvCtx_GetEnvVar(env, args[0].p, args[1].p, args[2].i);
				*((int *)vParm1)=sz;
				ret=TK_URES_TRUE;
				break;
			case 0x0D:
				sz=TK_EnvCtx_SetEnvVar(env, args[0].p, args[1].p, args[2].i);
				*((int *)vParm1)=sz;
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
				sz=TK_SpawnNewThreadB(task, args[0].p, args[1].p);
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

			case 0x1E:
				p=TK_DlOpenB(task, args[0].p, args[1].i);
				*((void **)vParm1)=p;
				ret=TK_URES_TRUE;
				break;
			case 0x1F:
				p=TK_DlSymB(task, args[0].p, args[1].p, args[2].i);
				*((void **)vParm1)=p;
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
				ret=tk_fmount(args[0].p, args[1].p, args[2].p, args[3].p);
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

			default:
				tk_printf("SYSC: BAD sObj=%p, uMsg=%X, vParm1=%p, vParm1=%p\n",
					sObj, uMsg, vParm1, vParm2);
				break;
			}
			break;
		default:
			tk_printf("SYSC: BAD sObj=%p, uMsg=%X, vParm1=%p, vParm1=%p\n",
				sObj, uMsg, vParm1, vParm2);
			break;
		}
		break;
	default:
		tk_printf("SYSC: BAD sObj=%p, uMsg=%X, vParm1=%p, vParm1=%p\n",
			sObj, uMsg, vParm1, vParm2);
		break;
	}
	
	return(ret);
}
