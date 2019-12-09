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
	TK_SysArg *args;
	void *p;
	int ret, sz;

//	u64 ttb, tea;
//	u16 exsr;
	
//	ttb=__arch_ttb;
//	tea=__arch_tea;
//	exsr=(u16)(__arch_exsr);
//	ptetlb_rethow_exc=0;

	task=TK_GetCurrentTask();

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
				*((void **)vParm1)=tk_con_getctx();
				ret=TK_URES_TRUE;
				break;
			case 0x02:
				tk_con_reset();
				ret=TK_URES_TRUE;
				break;
			
			case 0x03:
				tk_sysc_exitpgm(args[0].i);
				break;

			case 0x04:
				sz=args[0].i;
				p=TKMM_PageAlloc(sz);
				*((void **)vParm1)=p;
				TK_TaskAddPageAlloc(task, p, sz);
				tk_printf("SYSC: Page Alloc, vParm=%p, p=%p\n", vParm1, p);
				break;
			case 0x05:
				ret=TKMM_PageFree(args[0].p, args[1].i);
				break;
			
			case 0x06:
				tk_irq_setUserIrq(args[0].i, args[1].p);
				break;
			
			case 0x20:
				ret=tk_hfopen(args[0].p, args[1].p);
				break;
			case 0x21:
				ret=tk_hopendir(args[0].p);
				break;
			case 0x22:
				ret=tk_unlink(args[0].p);
				break;
			case 0x23:
				ret=tk_rename(args[0].p, args[1].p);
				break;
			case 0x24:
				ret=tk_fstat(args[0].p, args[1].p);
				break;
			case 0x25:
				ret=tk_fmount(args[0].p, args[1].p, args[2].p, args[3].p);
				break;
			case 0x26:
				ret=tk_hreaddir(args[0].i, args[1].p, args[2].i, args[3].i);
				break;
			case 0x27:
				ret=tk_hclosedir(args[0].i);
				break;
			case 0x28:
				ret=tk_hread(args[0].i, args[1].p, args[2].i);
				break;
			case 0x29:
				ret=tk_hwrite(args[0].i, args[1].p, args[2].i);
				break;
			case 0x2A:
				*((s64 *)vParm1)=tk_hseek(args[0].i, args[1].l, args[2].i);
				ret=TK_URES_TRUE;
				break;
			case 0x2B:
				ret=tk_hclose(args[0].i);
				break;
			case 0x2C:
				ret=tk_hioctl(args[0].i, args[1].i, args[2].p);
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	
	return(ret);
}
