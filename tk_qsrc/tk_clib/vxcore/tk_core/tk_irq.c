static int (*irq_timer[16])();
static int n_irq_timer=0;

// int tk_iskernel();
void *__snipe_dc(void *addr);
void *__snipe_ic(void *addr);

TKPE_TaskInfo *TK_SpawnNewThread2B(
	TKPE_TaskInfo *btask, void *func, void *uptr, TKPE_CreateTaskInfo *info);
TKPE_TaskInfo *TK_SpawnNewThreadB(
	TKPE_TaskInfo *btask, void *func, void *uptr);

TKPE_TaskInfo *tk_task_syscall;

#ifndef __TK_CLIB_ONLY__
extern TKPE_TaskInfo *tk_task_syscall;
extern u64	tk_vmem_pageglobal;
#endif

int tk_issyscall()
{
#ifndef __TK_CLIB_ONLY__
	return(TK_GET_TBR==((tk_kptr)tk_task_syscall));
#else
	return(0);
#endif
}

int tk_iskerneltask()
{
#ifndef __TK_CLIB_ONLY__
	TKPE_TaskInfo *task;
	if(!tk_iskernel())
		return(0);
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task==tk_task_syscall)
		return(1);
//	if(task->pid==0)
	if(task->pid==1)
		return(1);
	if(!(task->SysCall))
		return(1);
	return(0);
#else
	return(0);
#endif
}

int tk_issupertask()
{
	if(tk_iskerneltask())
		return(1);
	return(0);
}

int irq_addTimerIrq(void *fcn)
{
#ifndef __TK_CLIB_ONLY__
	if(!tk_iskernel())
	{
		TK_SetUserIrqV(0xC001, fcn);
		return(1);
	}
#endif

	irq_timer[n_irq_timer++]=fcn;
	return(1);
}

#ifndef __TK_CLIB_ONLY__
int tk_irq_setUserIrq(int irq, void *fcn)
{
	if(!tk_iskernel())
		return(-1);

	if(0xC001)
	{
		irq_addTimerIrq(fcn);
		return(1);
	}

	return(0);
}
#endif

int TK_SetUserIrqV(int irq, void *ptr)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	p=NULL;
	ar[0].i=irq;
	ar[1].p=ptr;
	i=tk_syscall(NULL, TK_UMSG_SETUSRIRQ, &p, ar);
	return(i);
}

#ifndef __TK_CLIB_ONLY__
TKPE_TaskInfo *tk_sched_taskarray[1024];
int tk_sched_ntask;

TKPE_TaskInfo *TK_CheckSchedNewTask(TKPE_TaskInfo *oldtask, int flag)
{
	TKPE_TaskInfo *newtask;
	u64 yres, tt;
	int i, j, k, tid;
	
	newtask=NULL;
	tid=oldtask->sch_id;

	if((tid<=0) && !(flag&1))
	{
		/* Non-Scheduled tasks are not preemptible. */
		return(NULL);
	}
	
	i=tid;
	while(1)
	{
		i=i+1;
		if(i>=tk_sched_ntask)
			i=1;
		if(i==tid)
		{
			newtask=NULL;
			break;
		}
		newtask=tk_sched_taskarray[i];

		if(newtask==tk_task_syscall)
			newtask=NULL;

		if(newtask && !(newtask->status))
		{
			yres=newtask->ystatus;
			if(yres)
			{
				if(yres>>56)
				{
					/* Remove from scheduler. */
					tk_sched_taskarray[i]=NULL;
					newtask->sch_id=0;
					newtask=NULL;
					continue;
				}else
				{
					tt=TK_GetTimeUs();
					if(yres>tt)
					{
						/* Still sleeping. */
						newtask=NULL;
						continue;
					}
				}
			}
			break;
		}
	}

	return(newtask);
//	return(NULL);
}

int TK_SchedAddTask(TKPE_TaskInfo *newtask)
{
	int i, j, k;
	
	if(newtask->sch_id>0)
	{
		i=newtask->sch_id;
		if(tk_sched_taskarray[i]==newtask)
			return(i);
		__debugbreak();
	}
	
	if(!tk_sched_ntask)
	{
		i=1;
		tk_sched_taskarray[0]=NULL;
		tk_sched_taskarray[1]=newtask;
		tk_sched_ntask=2;
		newtask->sch_id=1;
		return(i);
	}
	
	for(i=1; i<tk_sched_ntask; i++)
	{
		if(!tk_sched_taskarray[i])
			break;
	}
	
	if(i>=tk_sched_ntask)
		i=tk_sched_ntask++;

	tk_sched_taskarray[i]=newtask;
	newtask->sch_id=i;
	return(i);
}
#endif

#ifdef __BJX2__

extern volatile u64 __arch_ttb;
extern volatile u64 __arch_tea;
extern volatile u64 __arch_exsr;
extern volatile u64 __arch_mmcr;
extern volatile u64 __arch_sttb;
extern volatile u64 __arch_krr;
extern volatile u64 __arch_tbr;

extern volatile u64 __arch_pch;
extern volatile u64 __arch_gbh;

extern volatile void *__arch_isrsave;		/* Pseudo */

#ifndef __TK_CLIB_ONLY__

// int __isr_interrupt(int irq)
#ifdef __BGBCC__
__interrupt
#endif
void __isr_interrupt(void)
{
	TKPE_TaskInfo *task, *task2;
	TKPE_TaskInfoKern *taskern, *taskern2;
	void (*run)();
	int i, irq;

//	irq=__get_exsr();
	irq=__arch_exsr;

	if(((u16)irq)==0xC001)
	{
		task=(TKPE_TaskInfo *)__arch_tbr;
		if(task && task->qtick)
		{
			task->qtick--;
		}
		else if(task && task->krnlptr)
		{
//			task2=TK_CheckSchedNewTask(task, 0);
			task2=NULL;

#if 0
			if((task->pid>2) && ((task->us_lastsleep+500000)<TK_GetTimeUs()))
//			if((task->pid>2) && ((task->us_lastsleep+200000)<TK_GetTimeUs()))
//			if((task->pid>2) && ((task->us_lastsleep+150000)<TK_GetTimeUs()))
			{
				task2=TK_CheckSchedNewTask(task, 0);
			}
#endif

			if(task2 && (task2!=task))
			{
				taskern=(TKPE_TaskInfoKern *)task->krnlptr;
				memcpy(
					taskern->ctx_regsave,
					__arch_isrsave, 
					__ARCH_SIZEOF_REGSAVE__);

//				if(__ARCH_SIZEOF_REGSAVE__<512)
				__ifarch(!has_xgpr)
				{
					taskern->ctx_regsave[TKPE_REGSAVE_GBR] =
						taskern->ctx_regsave[TKPE_REGSAVE_GBR_LO];
					taskern->ctx_regsave[TKPE_REGSAVE_LR] =
						taskern->ctx_regsave[TKPE_REGSAVE_LR_LO];
					taskern->ctx_regsave[TKPE_REGSAVE_SPC] =
						taskern->ctx_regsave[TKPE_REGSAVE_SPC_LO];
					taskern->ctx_regsave[TKPE_REGSAVE_EXSR] =
						taskern->ctx_regsave[TKPE_REGSAVE_EXSR_LO];
				}

				taskern->ctx_regsave[TKPE_REGSAVE_KRR]=__arch_krr;
				taskern->ctx_regsave[TKPE_REGSAVE_TTB]=__arch_ttb;

//				if(__arch_ifarch__has_xmov)
				__ifarch(has_xmov)
				{
					taskern->ctx_regsave[TKPE_REGSAVE_PCH]=__arch_pch;
					taskern->ctx_regsave[TKPE_REGSAVE_GBH]=__arch_gbh;
				}

				taskern2=(TKPE_TaskInfoKern *)task2->krnlptr;

//				if(__ARCH_SIZEOF_REGSAVE__<512)
				__ifarch(!has_xgpr)
				{
					taskern2->ctx_regsave[TKPE_REGSAVE_GBR_LO] =
						taskern2->ctx_regsave[TKPE_REGSAVE_GBR];
					taskern2->ctx_regsave[TKPE_REGSAVE_LR_LO] =
						taskern2->ctx_regsave[TKPE_REGSAVE_LR];
					taskern2->ctx_regsave[TKPE_REGSAVE_SPC_LO] =
						taskern2->ctx_regsave[TKPE_REGSAVE_SPC];
					taskern2->ctx_regsave[TKPE_REGSAVE_EXSR_LO] =
						taskern2->ctx_regsave[TKPE_REGSAVE_EXSR];
				}

				memcpy(
					__arch_isrsave, 
					taskern2->ctx_regsave,
					__ARCH_SIZEOF_REGSAVE__);

				__arch_krr=taskern2->ctx_regsave[TKPE_REGSAVE_KRR];
				__arch_ttb=taskern2->ctx_regsave[TKPE_REGSAVE_TTB];

//				if(__arch_ifarch__has_xmov)
				__ifarch(has_xmov)
				{
					__arch_pch=taskern2->ctx_regsave[TKPE_REGSAVE_PCH];
					__arch_gbh=taskern2->ctx_regsave[TKPE_REGSAVE_GBH];
				}

				task2->qtick=16;
				__arch_tbr=(u64 *)task2;
			}else
			{
				task->qtick=16;
			}
		}

		if((n_irq_timer<0) || (n_irq_timer>16))
		{
			__debugbreak();
			n_irq_timer=0;
		}
	
		for(i=0; i<n_irq_timer; i++)
		{
			run=irq_timer[i];
			if(!run)
				continue;
			run();

//			irq_timer[i]();
		}
		return;
	}

	if(((u16)irq)==0xC08F)
	{
	}

//	tk_printf("IRQ %X?\n", irq);
//	return;
}

#define TK_REGSAVE_TBR

/* Transfer control between userland task to syscall task. */
#ifdef TK_REGSAVE_TBR
// __regsave_tbr __interrupt void __isr_syscall(void)
__interrupt_tbrsave void __isr_syscall(void)
#else
__interrupt void __isr_syscall(void)
#endif
{
	TKPE_TaskInfo *task, *task2, *task3;
	TKPE_TaskInfoKern *taskern, *taskern2;
	u64 *isrsave, *args, *pret;
	u64 ttb, tea, exc, yres, uobj;
	u32 reg_sr, umsg;
	u16 exsr;
	
	ttb=__arch_ttb;
	tea=__arch_tea;
	exc=__arch_exsr;
	exsr=(u16)(exc);
	reg_sr=exc>>32;
//	tea=exc>>16;

	task=(TKPE_TaskInfo *)__arch_tbr;
	if(!task)
		__debugbreak();
	if(task->magic0!=TKPE_TASK_MAGIC)
		__debugbreak();

	taskern=(TKPE_TaskInfoKern *)task->krnlptr;
	if(taskern->magic0!=TKPE_TASK_MAGIC)
		__debugbreak();

#ifdef TK_REGSAVE_TBR
	isrsave=(u64 *)(task->regsave);
#else
	isrsave=__arch_isrsave;
#endif

	if((exsr&15)==0)
	{
		if(reg_sr&(1<<26))
		{
			uobj=isrsave[TKPE_REGSAVE_R10];
			umsg=isrsave[TKPE_REGSAVE_R11];
			pret=(u64 *)(isrsave[TKPE_REGSAVE_R12]);
			args=(u64 *)(isrsave[TKPE_REGSAVE_R13]);
		}
		else
		{
			uobj=isrsave[TKPE_REGSAVE_R4];
			umsg=isrsave[TKPE_REGSAVE_R5];
			pret=(u64 *)(isrsave[TKPE_REGSAVE_R6]);
			args=(u64 *)(isrsave[TKPE_REGSAVE_R7]);
		}

		task2=NULL;

		if(		TK_VMem_CheckAddrIsPhysPage(pret)	&&
				TK_VMem_CheckAddrIsPhysPage(args)	)
		{
	//		if((umsg==TK_UMSG_YIELDTHREAD) ||
	//			(umsg==TK_UMSG_PGMEXIT))
			if(umsg==TK_UMSG_YIELDTHREAD)
			{
				yres=args[0];
				task->ystatus=yres;
				task->us_lastsleep=TK_GetTimeUs();
				
	//			if(umsg==TK_UMSG_PGMEXIT)
	//			{
	//				yres=(u32)yres;
	//				yres|=(1ULL<<56);
	//			}

				if(yres>>56)
				{
					if(taskern->task_join_ret)
					{
						task2=(TKPE_TaskInfo *)(taskern->task_join_ret);
					}
				}

				if(!task2)
				{
					task2=TK_CheckSchedNewTask(task, 1);
					if(task2 && (task2!=task))
					{
						task->us_lastsleep=TK_GetTimeUs();
						task2->us_lastsleep=TK_GetTimeUs();
					}
				}

				if(!task2)
				{
					if(!(yres>>56))
					{
						/* yield or sleep, continue running task. */
						task2=task;
					}
				}
				
//				tk_dbg_printf("YIELD: %p %p\n", task, task2);
			}

			if(umsg==TK_UMSG_CONKBHIT)
			{
				yres=args[0];
				if(yres==0)
				{
					/* Bounce back to caller. */
					*(int *)pret=tk_kbhit();
					task2=task;
				}else
					if(yres==1)
				{
					/* Bounce back to caller. */
					*(u64 *)pret=TK_GetTimeUs();
					task2=task;
				}
			}
			
			if(	(umsg>=TK_UMSG_COMGLUE_VMT4) &&
				(umsg<=TK_UMSG_COMGLUE_VMT63)	&&
				TK_VMem_CheckAddrIsPhysPage(uobj)	)
			{
			}
		}

		if(!task2)
		{
			task2=tk_task_syscall;
			if(!task2)
				__debugbreak();
			taskern2=(TKPE_TaskInfoKern *)task2->krnlptr;
			taskern2->task_sysc_user=(tk_kptr)task;
		}else
		{
			taskern2=(TKPE_TaskInfoKern *)task2->krnlptr;
		}
	}else
		if((exsr&15)==1)
	{
		task2=taskern->task_sysc_user;
		if(!task2)
			__debugbreak();
		if(task2==tk_task_syscall)
			__debugbreak();

#if 1
//		if((task2->us_lastsleep+100000)<TK_GetTimeUs())
		if(task2->krnlptr &&
			(task2->pid>2) &&
			((task2->us_lastsleep+150000)<TK_GetTimeUs()))
//			((task2->us_lastsleep+100000)<TK_GetTimeUs()))
//			((task2->us_lastsleep+50000)<TK_GetTimeUs()))
		{
//			task3=TK_CheckSchedNewTask(task2, 1);
			task3=TK_CheckSchedNewTask(task2, 0);
			if(task3 && (task3!=task2))
			{
				task2->us_lastsleep=TK_GetTimeUs();
				task3->us_lastsleep=TK_GetTimeUs();
				task2=task3;
			}
		}
#endif
	}else
	{
		__debugbreak();
	}

	if(task2!=task)
	{
		if(task2->magic0!=TKPE_TASK_MAGIC)
			__debugbreak();

		taskern2=(TKPE_TaskInfoKern *)task2->krnlptr;
		if(taskern2->magic0!=TKPE_TASK_MAGIC)
			__debugbreak();

		if(!taskern2->ctx_regsave[TKPE_REGSAVE_SPC])
			__debugbreak();

#ifndef TK_REGSAVE_TBR
		memcpy(
			taskern->ctx_regsave,
			isrsave, 
			__ARCH_SIZEOF_REGSAVE__);

//		if(__ARCH_SIZEOF_REGSAVE__<512)
		__ifarch(!has_xgpr)
		{
			taskern->ctx_regsave[TKPE_REGSAVE_GBR] =
				taskern->ctx_regsave[TKPE_REGSAVE_GBR_LO];
			taskern->ctx_regsave[TKPE_REGSAVE_LR] =
				taskern->ctx_regsave[TKPE_REGSAVE_LR_LO];
			taskern->ctx_regsave[TKPE_REGSAVE_SPC] =
				taskern->ctx_regsave[TKPE_REGSAVE_SPC_LO];
			taskern->ctx_regsave[TKPE_REGSAVE_EXSR] =
				taskern->ctx_regsave[TKPE_REGSAVE_EXSR_LO];
		}
#endif

	//	__debugbreak();

		taskern->ctx_regsave[TKPE_REGSAVE_KRR]=__arch_krr;
		taskern->ctx_regsave[TKPE_REGSAVE_TTB]=__arch_ttb;

//		if(__arch_ifarch__has_xmov)
		__ifarch(has_xmov)
		{
			taskern->ctx_regsave[TKPE_REGSAVE_PCH]=__arch_pch;
			taskern->ctx_regsave[TKPE_REGSAVE_GBH]=__arch_gbh;
		}

#ifndef TK_REGSAVE_TBR
//		if(__ARCH_SIZEOF_REGSAVE__<512)
		__ifarch(!has_xgpr)
		{
			taskern2->ctx_regsave[TKPE_REGSAVE_GBR_LO] =
				taskern2->ctx_regsave[TKPE_REGSAVE_GBR];
			taskern2->ctx_regsave[TKPE_REGSAVE_LR_LO] =
				taskern2->ctx_regsave[TKPE_REGSAVE_LR];
			taskern2->ctx_regsave[TKPE_REGSAVE_SPC_LO] =
				taskern2->ctx_regsave[TKPE_REGSAVE_SPC];
			taskern2->ctx_regsave[TKPE_REGSAVE_EXSR_LO] =
				taskern2->ctx_regsave[TKPE_REGSAVE_EXSR];
		}

		memcpy(
			isrsave, 
			taskern2->ctx_regsave,
			__ARCH_SIZEOF_REGSAVE__);
#endif

		ttb=taskern2->ctx_regsave[TKPE_REGSAVE_TTB];
		if(!ttb)
			{ ttb=tk_vmem_pageglobal; }

		__arch_krr=taskern2->ctx_regsave[TKPE_REGSAVE_KRR];
		__arch_ttb=ttb;

//		if(__arch_ifarch__has_xmov)
		__ifarch(has_xmov)
		{
			__arch_pch=taskern2->ctx_regsave[TKPE_REGSAVE_PCH];
			__arch_gbh=taskern2->ctx_regsave[TKPE_REGSAVE_GBH];
		}

		__arch_tbr=(u64 *)task2;
	}

	/* Returns into new task. */
}

#endif

#endif

#ifndef __TK_CLIB_ONLY__
void TK_Task_SyscallGetArgs(
	TKPE_TaskInfo **rtask,
	void **rsobj, int *rumsg, void **rrptr, void **rargs)
{
	TKPE_TaskInfo *task, *task2;
	TKPE_TaskInfoKern *taskern, *taskern2;
	u32 umsg, reg_sr;
	u64 *regs;
	void *sobj;
	void *rptr;
	void *args;
	
//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	taskern=(TKPE_TaskInfoKern *)task->krnlptr;

	task2=taskern->task_sysc_user;
	if(!task)
	{
		*rtask=NULL;
		*rsobj=NULL;
		*rumsg=0;
		*rrptr=NULL;
		*rargs=NULL;
		return;
	}
	
	taskern2=(TKPE_TaskInfoKern *)task2->krnlptr;
	
	regs=taskern2->ctx_regsave;
	reg_sr=regs[TKPE_REGSAVE_EXSR]>>32;
	if(reg_sr&(1<<26))
	{
		/* RISC-V Mode */
		sobj=regs[TKPE_REGSAVE_R10];
		umsg=regs[TKPE_REGSAVE_R11];
		rptr=regs[TKPE_REGSAVE_R12];
		args=(void *)(regs[TKPE_REGSAVE_R13]);
	}else
	{
		/* BJX2 Mode */
		sobj=regs[TKPE_REGSAVE_R4];
		umsg=regs[TKPE_REGSAVE_R5];
		rptr=regs[TKPE_REGSAVE_R6];
		args=(void *)(regs[TKPE_REGSAVE_R7]);
	}
	
	*rtask=task2;
	*rsobj=sobj;
	*rumsg=umsg;
	*rrptr=rptr;
	*rargs=args;
}
#endif

void tk_syscall2_rtuser();

#ifndef __BJX2__

void tk_syscall2_rtuser()
{
}

#endif

#ifndef __TK_CLIB_ONLY__
void TK_Task_SyscallReturnToUser(TKPE_TaskInfo *utask)
{
	void *aptr[2];
	TKPE_TaskInfo *task, *task2;
	TKPE_TaskInfoKern *taskern, *taskern2;

//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	taskern=(TKPE_TaskInfoKern *)task->krnlptr;

	aptr[0]=utask;

	taskern->task_sysc_user=(tk_kptr)utask;
	tk_syscall2_rtuser();

	if(aptr[0]!=utask)
	{
		__debugbreak();
	}
}

void TK_Task_SyscallSetReturn(TKPE_TaskInfo *task, u64 val)
{
	TKPE_TaskInfoKern *taskern;
	u32 reg_sr;

	taskern=(TKPE_TaskInfoKern *)task->krnlptr;

	reg_sr=taskern->ctx_regsave[TKPE_REGSAVE_EXSR]>>32;
	if(reg_sr&(1<<26))
	{
		/* RISC-V Mode */
		taskern->ctx_regsave[TKPE_REGSAVE_R10]=val;
	}else
	{
		taskern->ctx_regsave[TKPE_REGSAVE_R2]=val;
	}
}

int TK_Task_SyscallTaskIsRiscV(TKPE_TaskInfo *task)
{
	TKPE_TaskInfoKern *taskern;
	u32 reg_sr;

	taskern=(TKPE_TaskInfoKern *)task->krnlptr;

	reg_sr=taskern->ctx_regsave[TKPE_REGSAVE_EXSR]>>32;
	if(reg_sr&(1<<26))
		return(1);
	return(0);
}
#endif

int TK_Task_SyscallLoop(void *uptr)
{
#ifndef __TK_CLIB_ONLY__
	TKPE_TaskInfo *task, *task2;
	TKPE_TaskInfoKern *taskern;
	int umsg;
	u64 *argsl;
	void *sobj, *rptr, *args;
	int		rc;

	task=NULL;	sobj=NULL;
	umsg=0;		rptr=NULL;
	args=NULL;

	tk_dbg_printf("TK_Task_SyscallLoop: Start 0\n");

	/* Not handling syscalls yet, return to spawner. */
	TK_Task_SyscallReturnToUser(uptr);

	tk_dbg_printf("TK_Task_SyscallLoop: Start 1\n");

	/* This loop spins indefinately as part of a syscall handler task. */
	while(1)
	{
		TK_Task_SyscallGetArgs(&task, &sobj, &umsg, &rptr, &args);
		if(task && umsg)
		{
			if(umsg==TK_UMSG_PGMEXIT)
			{
				if(task->magic0!=TKPE_TASK_MAGIC)
					__debugbreak();

				taskern=(TKPE_TaskInfoKern *)(task->krnlptr);
				argsl=args;

				task2=(TKPE_TaskInfo *)(taskern->task_join_ret);
				if(task2==tk_task_syscall)
					__debugbreak();
				
				rc=argsl[0];
				task->ystatus=((u32)rc)|(1ULL<<56);

				TKGDI_HalCleanupForTask(task);
				TK_TaskFreeAllPageAlloc(task);
//				TK_DestroyTaskInfo(task);
				
				if(!task2)
				{
					task2=TK_CheckSchedNewTask(task, 1);
				}
				
				if(task2 && (task2!=task))
				{
					if(task2->magic0!=TKPE_TASK_MAGIC)
						__debugbreak();

					TK_Task_SyscallReturnToUser(task2);
					continue;
				}
				
				__debugbreak();
			}

			rc=TK_HandleSyscall(task, sobj, umsg, rptr, args);
			TK_Task_SyscallSetReturn(task, rc);
		}
		TK_Task_SyscallReturnToUser(task);
	}

	/* control should never get here. */
	__debugbreak();
#endif
}

int TK_Task_TryJoinOnReturn(TKPE_TaskInfo *task)
{
#ifndef __TK_CLIB_ONLY__
	TKPE_TaskInfo *ctask;
	TKPE_TaskInfoKern *taskern;
	u64 yres;
	u32 reg_sr;
	int rv;

	yres=task->ystatus;
	if(!(yres>>56))
	{
		return(-EAGAIN);
	}

	rv=yres;
	return(rv);
#endif
}

int TK_Task_JoinOnReturn(TKPE_TaskInfo *task)
{
#ifndef __TK_CLIB_ONLY__
	TKPE_TaskInfo *ctask;
	TKPE_TaskInfoKern *taskern;
	u64 yres;
	u32 reg_sr;
	int rv;

	yres=task->ystatus;
	while(!(yres>>56))
	{
		taskern=(TKPE_TaskInfoKern *)task->krnlptr;
		if(!taskern->task_join_ret)
		{
			ctask=TK_GetCurrentTask();
			if(ctask==tk_task_syscall)
				__debugbreak();
			taskern->task_join_ret=(tk_kptr)ctask;
		}
		TK_Task_SyscallReturnToUser(task);
		yres=task->ystatus;
	}

	rv=yres;
	return(rv);
#endif

#if 0
	reg_sr=taskern->ctx_regsave[TKPE_REGSAVE_EXSR]>>32;
	if(reg_sr&(1<<26))
	{
		/* RISC-V Mode */
		rv=taskern->ctx_regsave[TKPE_REGSAVE_R10];
	}else
	{
		rv=taskern->ctx_regsave[TKPE_REGSAVE_R2];
	}
	return(rv);
#endif
}

#ifndef __TK_CLIB_ONLY__

TKPE_TaskInfo *tk_task_list[4096];
int tk_task_max;

int TK_FindFreePid()
{
	int i;
	
	for(i=1; i<tk_task_max; i++)
	{
		if(!tk_task_list[i])
			return(i);
	}
	
	if(i>=tk_task_max)
	{
		tk_task_max=i+1;
		return(i);
	}

	return(-1);
}

void *TK_AllocNewTask()
{
	TKPE_TaskInfo		*task;
	TKPE_TaskInfoUser	*tusr;
	TKPE_TaskInfoKern	*tknl;
	int pid;

	pid=TK_FindFreePid();
	if(pid<=0)
		return(NULL);

//	task=tk_malloc(sizeof(TKPE_TaskInfo));
//	tusr=tk_malloc(sizeof(TKPE_TaskInfoUser));
//	tknl=tk_malloc(sizeof(TKPE_TaskInfoKern));

	task=TKMM_MallocURo(sizeof(TKPE_TaskInfo));
	tusr=TKMM_MallocUsr(sizeof(TKPE_TaskInfoUser));
	tknl=TKMM_MallocKrn(sizeof(TKPE_TaskInfoKern));

	tk_dbg_printf("TK_AllocNewTask: task=%p tknl=%p pid=%d\n", task, tknl, pid);

	memset(task, 0, sizeof(TKPE_TaskInfo));
	memset(tusr, 0, sizeof(TKPE_TaskInfoUser));
	memset(tknl, 0, sizeof(TKPE_TaskInfoKern));
	task->pid=pid;
	tk_task_list[pid]=task;
	
	task->usrptr=(tk_kptr)tusr;
	task->krnlptr=(tk_kptr)tknl;
	task->allocaptr=(tk_kptr)(&(tusr->allocamrk));
	task->tlsptr=(tk_kptr)(tusr->tlsdat);
	task->regsave=(tk_kptr)(tknl->ctx_regsave);
	
	task->magic0=TKPE_TASK_MAGIC;
	tknl->magic0=TKPE_TASK_MAGIC;
	tusr->magic0=TKPE_TASK_MAGIC;
	
	return(task);
}

int TK_DestroyTaskInfo(void *tkptr)
{
#if 1
	TKPE_TaskInfo *task;
	int pid;
	
	tk_dbg_printf("TK_DestroyTask\n");

	task=tkptr;

	pid=task->pid;
	if(pid>0)
	{
		tk_task_list[pid]=NULL;
	}

	n_irq_timer=0;
	
	tk_free(task);
#endif
}

void TK_SetCurrentTask(TKPE_TaskInfo *task)
{
//	__arch_tbr=task;
	TK_SET_TBR((tk_kptr)task);
}

int TK_Task_TryJoinOnReturnPid(int pid)
{
	TKPE_TaskInfo *task;

	if((pid<=0) || (pid>=4096))
	{
		tk_dbg_printf("TK_Task_TryJoinOnReturnPid: Invalid pid=%d\n", pid);
		return(-EINVAL);
	}

	task=tk_task_list[pid];
	if(!task)
	{
		tk_dbg_printf("TK_Task_TryJoinOnReturnPid: No task for pid=%d\n", pid);
		return(-ESRCH);
	}

	return(TK_Task_TryJoinOnReturn(task));
}

#endif

int TK_Task_PidTryJoinReturnV(int pid)
{
	TK_SysArg ar[4];
	s64 p;
		
	p=0;
	ar[0].i=pid;
	tk_syscall(NULL, TK_UMSG_TRYJOINRESULT, &p, ar);

	return(p);
}


/*
 * Wait for the task associated with a PID to terminate.
 * Returns the return value for the task.
 */
int TK_Task_PidJoinOnReturn(int pid)
{
	int rv;

	if(tk_issyscall())
		return(-1);

// #ifndef __TK_CLIB_ONLY__
#if 0
//	if(tk_iskernel())
	if(tk_iskerneltask())
	{
		rv=TK_Task_JoinOnReturn(tk_task_list[pid]);
		return(rv);
	}
#endif

// #ifndef __TK_CLIB_ONLY__
#if 0
	if(tk_iskerneltask())
	{
		while(1)
		{
//			TK_YieldCurrentThread();
		}
	}
#endif

	rv=TK_Task_PidTryJoinReturnV(pid);
	while(rv==(-EAGAIN))
	{
		TK_YieldCurrentThread();
		rv=TK_Task_PidTryJoinReturnV(pid);
	}
	return(rv);
}



TKPE_TaskInfo *TK_GetCurrentTask()
{
	TKPE_TaskInfo *task;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
//	__debugbreak();
	return(task);
}

#ifndef __TK_CLIB_ONLY__
TKPE_TaskInfo *TK_GetUserOrCurrentTask()
{
	TKPE_TaskInfo *task, *task1;
	TKPE_TaskInfoKern *krnl;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	krnl=(TKPE_TaskInfoKern *)(task->krnlptr);
	task1=task;
	
	if(krnl->task_sysc_user)
	{
		task1=(TKPE_TaskInfo *)(krnl->task_sysc_user);
	}

//	__debugbreak();
//	return(task);
	return(task1);
}

TKPE_TaskInfo *TK_GetSyscallUserTask()
{
	TKPE_TaskInfo *task, *task1;
	TKPE_TaskInfoKern *krnl;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	krnl=(TKPE_TaskInfoKern *)(task->krnlptr);
	task1=(TKPE_TaskInfo *)(krnl->task_sysc_user);
	return(task1);
}
#endif

TK_EnvContext *TK_GetCurrentEnvContext()
{
#ifndef __TK_CLIB_ONLY__
	TKPE_TaskInfo *task;
	TK_EnvContext *env;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
	{
		task=TK_AllocNewTask();
		TK_SetCurrentTask(task);
//		__debugbreak();
//		return(NULL);
	}
	env=(void *)(task->envctx);
	if(env)
		return(env);

	tk_dbg_printf("TK_GetCurrentEnvContext: New %p:%p\n", task, env);

	env=TK_EnvCtx_AllocContext();
	task->envctx=(tk_kptr)env;
	return(env);
#endif

#ifdef __TK_CLIB_ONLY__
	TKPE_TaskInfo *task;
	TK_EnvContext *env;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	env=(void *)(task->envctx);
	return(env);
#endif
}

#ifndef __TK_CLIB_ONLY__
TKPE_TaskInfoKern *TK_GetCurrentTaskInfoKern()
{
	TKPE_TaskInfo *task;
	TKPE_TaskInfoKern *krnl;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
	{
		task=TK_AllocNewTask();
		TK_SetCurrentTask(task);
//		__debugbreak();
//		return(NULL);
	}

	krnl=(TKPE_TaskInfoKern *)(task->krnlptr);
	if(krnl)
		return(krnl);

	__debugbreak();

//	tk_printf("TK_GetCurrentEnvContext: New %p:%p\n", task, env);
//	env=TK_EnvCtx_AllocContext();
//	task->envctx=(tk_kptr)env;
	return(NULL);
}
#endif


TK_EnvContext *TK_GetTaskEnvContext(TKPE_TaskInfo *task)
{
#ifndef __TK_CLIB_ONLY__
	TK_EnvContext *env;

	if(!task)
		{ return(NULL); }
	env=(void *)(task->envctx);
	if(env)
		return(env);

	tk_dbg_printf("TK_GetCurrentEnvContext: New %p:%p\n", task, env);

	env=TK_EnvCtx_AllocContext();
	task->envctx=(tk_kptr)env;
	return(env);
#endif

#ifdef __TK_CLIB_ONLY__
	TK_EnvContext *env;
	env=(void *)(task->envctx);
	return(env);
#endif
}

static void *tk_dummyallocaptr;

void **TK_GetAllocaMark()
{
	TKPE_TaskInfo *task;
//	TK_EnvContext *env;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
	{
		return(&tk_dummyallocaptr);
//		return(NULL);
	}
//	return(&(task->allocaptr));
	return((void **)(task->allocaptr));
}

int TK_TaskAddPageAlloc(TKPE_TaskInfo *task, void *base, int size)
{
	TKPE_TaskInfoKern *krnl;
	int i;

	if(!task || !base)
		return(0);
	
	krnl=(TKPE_TaskInfoKern *)(task->krnlptr);
	
//	i=task->n_span++;
//	task->span_ptr[i]=base;
//	task->span_sz[i]=size;

	i=krnl->n_span++;
	krnl->span_ptr[i]=(tk_kptr)base;
	krnl->span_sz[i]=size;

	return(i);
}

int TK_TaskFreeAllPageAlloc(TKPE_TaskInfo *task)
{
#ifndef __TK_CLIB_ONLY__
	TKPE_TaskInfoKern *krnl;
	int i;

	if(!task)
		return(0);

	krnl=(TKPE_TaskInfoKern *)(task->krnlptr);
	if(!krnl)
		return(0);

	for(i=0; i<krnl->n_span; i++)
	{
		TK_MMap_VaPageFree((void *)(krnl->span_ptr[i]), krnl->span_sz[i]);
	}
#endif
	return(0);
}

int TK_TaskGetCwd(TKPE_TaskInfo *task, char *buf, int sz)
{
	if(task->envctx)
	{
//		strncpy(buf, task->envctx->cwd, sz);
		TK_EnvCtx_GetCwd((void *)(task->envctx), buf, sz);
		return(1);
	}
	
	return(-1);
}

int TK_TaskSetCwd(TKPE_TaskInfo *task, char *buf)
{
	if(task->envctx)
	{
		TK_EnvCtx_SetCwd((void *)(task->envctx), buf);
		return(1);
	}
	
	return(-1);
}

#ifdef __TK_CLIB_ONLY__
TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	__debugbreak();
	return(-1);
}
#endif


#ifdef __BJX2__

void TK_FlushCacheL1D_INVDC(void *ptr);
void TK_FlushCacheL1D_INVIC(void *ptr);
void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz);
__asm {
TK_FlushCacheL1D_INVDC:
	MOV		LR, R1
	INVDC	R4
	NOP
	NOP
	JMP		R1
//	RTS

TK_FlushCacheL1D_INVIC:
	MOV		LR, R1
	INVIC	R4
	NOP
	NOP
	JMP		R1
//	RTS

TK_FlushCacheL1D_ReadBuf:
	.L0:
	MOV.Q	(R4), R7
	ADD		-16, R5
	ADD		16, R4
	CMP/GT	0, R5
	BT		.L0
	RTS
};

#else

void TK_FlushCacheL1D_INVDC(void *ptr)
{
}

void TK_FlushCacheL1D_INVIC(void *ptr)
{
}

void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz)
{
}

void *__snipe_ic(void *ptr)
{
	return(ptr);
}

void *__snipe_dc(void *ptr)
{
	return(ptr);
}

#endif

#if 1
void TK_FlushCacheL1D()
{
	static int tempvar;
	u64 *pptr;
	int i, j, k;

//	pptr=(u64 *)tk_task_list;	/* need memory to read from. */
	pptr=(u64 *)(&tempvar);	/* need memory to read from. */
	pptr=(u64 *)(((u64)pptr)&(~16777215));	/* Align */

	TK_FlushCacheL1D_INVDC(NULL);
	TK_FlushCacheL1D_ReadBuf(pptr, 65536);
//	TK_FlushCacheL1D_ReadBuf(pptr, 262144);
	TK_FlushCacheL1D_INVDC(NULL);
	TK_FlushCacheL1D_ReadBuf(pptr, 65536);
//	TK_FlushCacheL1D_ReadBuf(pptr, 262144);
}


void TK_SmallFlushL1D(void *ptr, int sz)
{
	byte *cs, *cse, *cs2;
	int fn;

	fn=0;
	cs=((byte *)ptr)-8;
	cse=((byte *)ptr)+sz+8;
	cs2=__snipe_dc(cs);

	if(cs2)
	{
		while(cs<cse)
		{
			cs2=__snipe_dc(cs);
			fn+=*((int *)cs2);
			cs+=16;
		}
		return;
	}

	while(cs<cse)
	{
		TK_FlushCacheL1D_INVDC(cs);
		fn+=*((int *)cs);
		TK_FlushCacheL1D_INVDC(cs);
		cs+=16;
	}
	return;
}

void TK_SmallFlushL1I(void *ptr, int sz)
{
	void (*fnb)();
	byte *cs, *cse, *cs2;
	int fn;

	fn=0;
	cs=((byte *)ptr)-8;
	cse=((byte *)ptr)+sz+8;
	cs2=__snipe_ic(cs);

	if(cs2)
	{
		while(cs<cse)
		{
			fnb=__snipe_ic(cs);
			fnb();
			cs+=16;
		}
		return;
	}

	while(cs<cse)
	{
		TK_FlushCacheL1D_INVIC(cs);
		cs+=16;
	}
	return;
}

#endif


u64 tk_seed1_aslr=0x12345678ULL;
u64 tk_seed2_aslr=0x12345678ULL;

u16 TK_GetRandom16ASLR()
{
	u64 hi;
	tk_seed1_aslr=(tk_seed1_aslr*65521)+(tk_seed1_aslr>>32)+1;
	tk_seed2_aslr=(tk_seed2_aslr*65521)+(tk_seed2_aslr>>32)+1;
	tk_seed1_aslr+=tk_seed2_aslr>>37;
	tk_seed2_aslr+=tk_seed1_aslr>>43;
	hi=(tk_seed1_aslr>>16)&65535;
	return(hi);
}

u64 TK_GetRandom48ASLR()
{
	u64 addr;
	addr=TK_GetRandom16ASLR();
	addr=(addr<<16)|TK_GetRandom16ASLR();
	addr=(addr<<16)|TK_GetRandom16ASLR();
	return(addr);
}

u64 TK_GetRandom();

int TK_SeedRandomASLR()
{
	int i;
	i=128+(TK_GetRandom()&255);
	while(i--)
		TK_GetRandom();
	tk_seed1_aslr=TK_GetRandom();
	i=128+(TK_GetRandom()&255);
	while(i--)
		TK_GetRandom();
	tk_seed2_aslr=TK_GetRandom();
}

#ifndef __BJX2__
u64 tk_getrandom_seed;

u64 TK_GetRandom()
{
	int i, j, k;

	if(!tk_getrandom_seed)
	{
		tk_getrandom_seed=TK_GetTimeUs();
		k=(tk_getrandom_seed&255)+127;
		while(k--)
		{
			j=tk_getrandom_seed>>48;
			tk_getrandom_seed*=65521;
			tk_getrandom_seed+=j+1;
		}
	}

	tk_getrandom_seed+=TK_GetTimeUs();
	k=((tk_getrandom_seed>>56)&63)+31;
	while(k--)
	{
		j=tk_getrandom_seed>>48;
		tk_getrandom_seed*=65521;
		tk_getrandom_seed+=j+1;
	}
	return(tk_getrandom_seed);
}
#endif

#ifdef __BJX2__

__asm {
TK_GetRandom:
	CPUID	31
	NOP
	NOP
	MOV		DLR, R2
	RTS
};

__uint128 TK_WithKrrEncodeKey(u64 key);

__asm {
TK_WithKrrEncodeKey:
	MOV		R4, R0
	NOP
	LDEENC
	NOP
	NOP
	MOV		R0, R2
	MOV		R1, R3
	RTS
};

u16	*tk_vmem_usrexpage;		//User, Execute Only, Memory
u16	*tk_vmem_usrexonly;		//User, Execute Only, Ex-Only Addr
int	tk_vmem_usrexoffs;		//User, Execute Only, Offset
byte tk_vmem_useldekrr;

void *TK_WithKrrSetuidB(void *func, u64 key)
{
	__uint128	kx;
	u64 fna, khi, klo;
	u16 *cts, *ct;
	int ofs, h, h0, h1, fl;

	if(!tk_vmem_useldekrr)
	{
		return(func);
	}

	kx=TK_WithKrrEncodeKey(key);
	khi=kx>>64;
	klo=kx;
	
	fna=(u64)func;
	
	ofs=tk_vmem_usrexoffs;
	ct=tk_vmem_usrexpage+ofs;
	cts=ct;
	
	h=(ofs*251)>>8;
	h0=((h>>0)&3)+1;
	h1=((h>>2)&3)+1;
	if((h1==h0))
	{
		if(h&0x40)
			h1=((h0-1)&3)+1;
		else
			h1=((h0+1)&3)+1;
	}
	
	*ct++=0xCFC0;					// ADD		-64, SP
	*ct++=0x4511;					// MOV		LR, R1
	*ct++=0x4110;					// MOV.Q	R1, (SP, 0)

	*ct++=0x30A2;					// SVEKRR
	*ct++=0x3000;					// NOP
	*ct++=0x3000;					// NOP
	*ct++=0x4100|h0;				// MOV.Q	R0, (SP, 16)
	*ct++=0x4110|h1;				// MOV.Q	R1, (SP, 24)

//	if((h>>4)&2)
	if(h&0x20)
	{
//		if((h>>4)&1)
		if(h&0x10)
		{	*ct++=0x1701;			// XOR		R1, R0
			*ct++=0x1010;	}		// ADD		R0, R1
		else
		{	*ct++=0x1701;			// XOR		R1, R0
			*ct++=0x1110;	}		// SUB		R0, R1
	}else
	{
//		if((h>>4)&1)
		if(h&0x10)
		{	*ct++=0x1001;			// ADD		R1, R0
			*ct++=0x1710;	}		// XOR		R0, R1
		else
		{	*ct++=0x1101;			// SUB		R1, R0
			*ct++=0x1710;	}		// XOR		R0, R1
	}

	if((h0!=1) && (h1!=1))
		*ct++=0x4101;				// MOV.Q	R0, (SP, 8)
	if((h0!=2) && (h1!=2))
		*ct++=0x4112;				// MOV.Q	R1, (SP, 16)
	if((h0!=3) && (h1!=3))
		*ct++=0x4103;				// MOV.Q	R0, (SP, 24)
	if((h0!=4) && (h1!=4))
		*ct++=0x4114;				// MOV.Q	R1, (SP, 32)

	*ct++=0xFF00|((klo>>56)&0x00FF);
	*ct++=0x0000|((klo>>40)&0xFFFF);
	*ct++=0xFF00|((klo>>32)&0x00FF);
	*ct++=0x0000|((klo>>16)&0xFFFF);
	*ct++=0xF800;
	*ct++=0x0000|((klo>> 0)&0xFFFF);
	
	*ct++=0xFF00|((khi>>56)&0x00FF);
	*ct++=0x0000|((khi>>40)&0xFFFF);
	*ct++=0xFF00|((khi>>32)&0x00FF);
	*ct++=0x0000|((khi>>16)&0xFFFF);
	*ct++=0xF801;
	*ct++=0x0000|((khi>> 0)&0xFFFF);

	*ct++=0x3000;					// NOP
	*ct++=0x3000;					// NOP
	*ct++=0x30A2;					// LDEKRR
	
	
	*ct++=0xFF00|((fna>>40)&0x00FF);
	*ct++=0x0000|((fna>>24)&0xFFFF);
	*ct++=0xFB00|((fna>>16)&0x00FF);
	*ct++=0x0000|((fna>> 0)&0xFFFF);

	*ct++=0x4500|h0;				// MOV.Q	(SP, 16), R0
	*ct++=0x4510|h1;				// MOV.Q	(SP, 24), R1
	*ct++=0x3000;					// NOP
	*ct++=0x3000;					// NOP
	*ct++=0x30A2;					// LDEKRR

	*ct++=0x4510;					// MOV.Q	(SP, 0), R1
	*ct++=0xCF40;					// ADD		64, SP
	*ct++=0x3210;					// JMP		R1

	TK_SmallFlushL1D(cts, (ct-cts)*2);


	cts=tk_vmem_usrexonly+ofs;

	return(cts);
}

#else

void *TK_WithKrrSetuidB(void *func, u64 key)
{
}

#endif

void *TK_WithKrrSetuid(void *func, u64 key)
{
	TK_SysArg ar[4];
	void *p;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		p=TK_WithKrrSetuidB(func, key);
		return(p);
	}
#endif

//	tk_printf("TK_WithKeySetuid: sz=%d\n", sz);
	
	p=NULL;
	ar[0].p=func;
	ar[1].i=key;
	tk_syscall(NULL, TK_UMSG_WITHKRRSETUID, &p, ar);

//	tk_printf("TK_WithKeySetuid: Vp=%p, p=%p\n", &p, p);

	return(p);

}

int TK_GetCurrentThreadId()
{
	TKPE_TaskInfo *task;

//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
		{ return(1); }
	return(task->pid);
}

int TK_GetCurrentProcessId()
{
	TKPE_TaskInfo *task, *task1;

//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
		{ return(1); }

	task1=(TKPE_TaskInfo *)(task->boottbr);
	if(task1 && (task1!=task))
		return(task1->pid);

	return(task->pid);
}

TKPE_TaskInfo *TK_GetTaskProcessTask(TKPE_TaskInfo *task)
{
	TKPE_TaskInfo *task1;

//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
		{ return(NULL); }

	task1=(TKPE_TaskInfo *)(task->boottbr);
	if(task1 && (task1!=task))
		return(task1);

	return(task);
}

#ifndef __TK_CLIB_ONLY__
int TK_YieldCurrentThreadB(s64 res)
{
	TKPE_TaskInfo *task, *task1;
	TKPE_TaskInfoKern *tk0, *tk1;
	s64 tt0, tt1;
	int pid, pid1, nosch;
	int st;

	tt0=TK_GetTimeUs();

//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
		return(0);

	pid=task->pid;
	task1=NULL;

	pid1=pid+1;
	if(pid1>=tk_task_max)
		pid1=1;
		
	task1=TK_CheckSchedNewTask(task, 1);

#if 0
	while(!task1 && (pid1!=pid))
	{
		task1=tk_task_list[pid1];
		
//		nosch=0;
		if(task1==tk_task_syscall)
			task1=NULL;
//			nosch=1;
		
//		if(task1 && !nosch)
		if(task1)
		{
			tt1=task1->ystatus;
			if(!tt1)
				break;		/* Running */
			if((tt1>>56)==0x00)
			{
				/* Sleeping */
				if(tt1>tt0)
					task1=NULL;
			}else
			{
				/* Terminated / etc */
				task1=NULL;
			}
		}
		
		pid1=pid1+1;
		if(pid1>=tk_task_max)
			pid1=1;
	}
#endif
	
	if(!task1 || (task1==task))
		return(0);
		
	tk0=(TKPE_TaskInfoKern *)(task->krnlptr);
	tk1=(TKPE_TaskInfoKern *)(task1->krnlptr);
	
	task->ystatus=res;	/* non-zero for sleep or termination. */
	TK_Task_SyscallReturnToUser(task1);
    return(0);

#if 0
	st=__setj(tk0->ctx_regsave);
	if(st>0)
	{
		/* We have arrived back in this thread. */
		return(st);
	}

	/* Transfer to next thread. */
    __longj(tk1->ctx_regsave, 1);
    __debugbreak();		//we should never get here...    
    return(0);
#endif
}
#endif

/*
Res: Result Code
  0000_0000_0000_0000: Yield
  00zz_zzzz_zzzz_zzzz: Sleep Until (usec)
  0100_0000_zzzz_zzzz: Terminate with 32-bit status
 */

void TK_YieldCurrentThreadA(s64 res)
{
	TK_SysArg ar[4];
	void *p;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_issyscall())
	{
		return;
	}

#if 0
	if(tk_iskernel())
	{
		TK_YieldCurrentThreadB(res);
		return;
	}
#endif

#endif
	
	p=NULL;
	ar[0].l=res;
	tk_syscall(NULL, TK_UMSG_YIELDTHREAD, &p, ar);
	return;
}

void TK_YieldCurrentThread(void)
{
	TK_YieldCurrentThreadA(0);
}

#ifndef __TK_CLIB_ONLY__
void TK_SuspendThreadB(int pid, s64 res)
{
	TKPE_TaskInfo *task0, *task1;
	TKPE_TaskInfoKern *tk0, *tk1;
//	s64 tt0, tt1;
//	int pid, pid1;
	int st;

//	tt0=TK_GetTimeUs();

//	task0=(TKPE_TaskInfo *)__arch_tbr;
	task0=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task0)
		return;

	task1=tk_task_list[pid];
	if(task0==task1)
	{
		TK_YieldCurrentThreadB(res);
		return;
	}
	
	task1->ystatus=res;
}
#endif

void TK_SuspendThreadA(int pid, s64 res)
{
	TK_SysArg ar[4];
	void *p;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		TK_SuspendThreadB(pid, res);
		return;
	}
#endif
	
	p=NULL;
	ar[0].i=pid;
	ar[1].l=res;
	tk_syscall(NULL, TK_UMSG_SUSPTHREAD, &p, ar);
	return;
}

void TK_DetachThread(int pid)
{
	s64 res1;
	res1=0x0100000000000000LL;
	TK_SuspendThreadA(pid, res1);
}

#ifndef __TK_CLIB_ONLY__
s64 TK_GetThreadStatusB(int pid)
{
	TKPE_TaskInfo *task0, *task1;
	TKPE_TaskInfoKern *tk0, *tk1;
//	s64 tt0, tt1;
//	int pid, pid1;
	int st;

//	tt0=TK_GetTimeUs();

//	task0=(TKPE_TaskInfo *)__arch_tbr;
	task0=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task0)
		return(0);

	task1=tk_task_list[pid];
	if(task0==task1)
		return(0);
	return(task1->ystatus);
}
#endif

s64 TK_GetThreadStatusA(int pid)
{
	TK_SysArg ar[4];
	s64 st;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		return(TK_GetThreadStatusB(pid));
	}
#endif
	
	st=0;
	ar[0].i=pid;
	tk_syscall(NULL, TK_UMSG_THREADSTATUS, &st, ar);
	return(st);
}

s64 TK_JoinThread(int pid)
{
	s64 res;

	res=TK_GetThreadStatusA(pid);
	while(!(res<<56))
	{
		TK_YieldCurrentThread();
		res=TK_GetThreadStatusA(pid);
	}
	return(res);
}

int TK_SpawnNewThread2(void *func, void *uptr, TKPE_CreateTaskInfo *info)
{
	TK_SysArg ar[4];
	TKPE_TaskInfo *ntask;
	void *p;
	int tid;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		ntask=TK_SpawnNewThread2B(
			(TKPE_TaskInfo *)TK_GET_TBR, func, uptr, info);
		tid=ntask->pid;
		return(tid);
	}
#endif

	tid=0;
//	p=NULL;
	ar[0].p=func;
	ar[1].p=uptr;
	ar[2].p=info;
	tk_syscall(NULL, TK_UMSG_NEWTHREAD, &tid, ar);
	return(tid);
}

int TK_SpawnNewThread(void *func, void *uptr)
{
	TK_SpawnNewThread2(func, uptr, NULL);
}

void TK_ExitCurrentThread(int res)
{
	s64 res1;
	res1=((u32)res)|0x0100000000000000LL;
	TK_YieldCurrentThreadA(res);
}

void TK_SleepCurrentThread(int usec)
{
	s64 tt0;
	s64 res1;
	
	tt0=TK_GetTimeUs();
	res1=tt0+usec;
	TK_YieldCurrentThreadA(res1);
}

void tk_thread_entry(void)
{
	TKPE_TaskInfo *task;
	int (*run)(void *ptr);
	void *uptr;
	int res;

//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	uptr=(void *)(task->argv);
	run=(void *)(task->bootptr);
	res=run(uptr);
	TK_ExitCurrentThread(res);
}

int tk_getpid(void)
{
	TKPE_TaskInfo *task, *task2;
	void *uptr;
	int res;

//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	task2=(TKPE_TaskInfo *)task->boottbr;
	if(!task2)
		task2=task;
	return(task2->pid);
}

#ifndef __TK_CLIB_ONLY__
#ifdef __BJX2__

TKPE_TaskInfo *TK_SpawnNewThread2B(
	TKPE_TaskInfo *btask, void *func, void *uptr, TKPE_CreateTaskInfo *info)
{
	TKPE_TaskInfo *ptask, *task, *ctask;
	TKPE_TaskInfoKern *tk0, *tk1, *tkp;
	TKPE_TaskInfoUser *tu0, *tu1, *tup;
	byte *boot_newspb, *boot_newsp;
	byte *boot_newspbk, *boot_newspk;
	TK_EnvContext *env0, *env1;
	void *bootgbr, *boottbr, *bootptr, *sysc;
	long bootptrbits;
	int st, spusz, spksz, rqspusz;

//	ctask=(TKPE_TaskInfo *)__arch_tbr;
	ctask=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!ctask)
		return(NULL);

	ptask=TK_GetTaskProcessTask(btask);
	if(!ptask)
		return(NULL);

	tkp=(TKPE_TaskInfoKern *)(ptask->krnlptr);
	tup=(TKPE_TaskInfoUser *)(ptask->usrptr);

	task=TK_AllocNewTask();

	bootgbr=ptask->basegbr;
	boottbr=task;
	bootptr=tk_thread_entry;
	
	bootptrbits=(long)bootptr;
	if(bootptrbits&1)
	{
		/* Clear LSB, the mode needs to be handled explicitly. */
		bootptr=(void *)(bootptrbits&(~1));
	}

//	spusz=(ptask->boot_spe - task->boot_sps) + 4095;
//	spusz=(tkp->usr_boot_spe - tkp->usr_boot_sps) + 4095;
//	spksz=(tkp->krn_boot_spe - tkp->krn_boot_sps) + 4095;
//	spusz=(spusz>>14)<<14;
//	spksz=(spksz>>14)<<14;

	spusz=(1<<17);

	rqspusz=0;
	if(info)
		rqspusz=info->szStack;
	
	if((rqspusz>=(1<<15)) && (rqspusz<(1<<20)))
		{ spusz=rqspusz; }

	spusz=(spusz>>14)<<14;

//	boot_newspb=TKMM_PageAlloc(1<<19);
//	boot_newsp=boot_newspb+((1<<19)-1024);

	boot_newspb=TKMM_PageAlloc(spusz);
	boot_newsp=boot_newspb+(spusz-1024);

	tk_dbg_printf("TK_SpawnNewThreadB: task=%p stack=%p..%p pid=%d\n",
		task, boot_newspb, boot_newsp);

//	boot_newspbk=TKMM_PageAlloc(spksz);
//	boot_newspk=boot_newspbk+(spksz-1024);

//	env0=TK_GetCurrentEnvContext();
	env0=ptask->envctx;
	env1=TK_EnvCtx_CloneContext(env0);

	if(info)
	{
		task->ttyid=info->idTty;
	}

	task->baseptr=ptask->baseptr;
	task->bootptr=(tk_kptr)func;
	task->basegbr=(tk_kptr)bootgbr;
	task->boottbr=(tk_kptr)boottbr;
	task->argv=(tk_kptr)uptr;

	task->boot_sps=(tk_kptr)boot_newspb;
	task->boot_spe=(tk_kptr)boot_newsp;
	
	task->envctx=(tk_kptr)env1;
	sysc=tk_isr_syscall;
	task->SysCall=(tk_kptr)sysc;

	tk0=(TKPE_TaskInfoKern *)(ctask->krnlptr);
	tk1=(TKPE_TaskInfoKern *)(task->krnlptr);

	tu0=(TKPE_TaskInfoKern *)(ctask->usrptr);
	tu1=(TKPE_TaskInfoKern *)(task->usrptr);
	
	tu1->gdictx=tu0->gdictx;
	
	tk1->usr_boot_sps=(tk_kptr)boot_newspb;
	tk1->usr_boot_spe=(tk_kptr)boot_newsp;
//	tk1->krn_boot_sps=(tk_kptr)boot_newspbk;
//	tk1->krn_boot_spe=(tk_kptr)boot_newspk;

	if(info && (info->flSched&1))
	{
		TK_SchedAddTask(task);
	}

#if 0
	st=__setj(tk0->ctx_regsave);
	if(st>0)
	{
		/* We have arrived back in this thread. */
		return(task->pid);
	}

	/* Transfer to new thread. */
	__tk_farcall(bootptr, bootgbr, boot_newsp, boottbr);
#endif

//	__debugbreak();

#if 1
	tk1->ctx_regsave[TKPE_REGSAVE_TTB]=tk0->ctx_regsave[TKPE_REGSAVE_TTB];

	tk1->ctx_regsave[TKPE_REGSAVE_EXSR]|=0xC000000000000000ULL;

	if(bootptrbits&1)
	{
		tk1->ctx_regsave[TKPE_REGSAVE_EXSR]|=
//			(((bootptrbits>>48)&0x000C)<<56) |
//			(((bootptrbits>>48)&0x00F0)<<48);
			((bootptrbits<<8)&0x0C00000000000000ULL) |
			( bootptrbits    &0x00F0000000000000ULL) ;
	}

	tk1->ctx_regsave[TKPE_REGSAVE_R4]=uptr;

	tk1->ctx_regsave[TKPE_REGSAVE_SPC]=bootptr;
	tk1->ctx_regsave[TKPE_REGSAVE_GBR]=bootgbr;
	tk1->ctx_regsave[TKPE_REGSAVE_SSP]=boot_newsp;

	if(ctask!=tk_task_syscall)
	{
		TK_Task_SyscallReturnToUser(task);
	}
#endif

	return(task);
}

TKPE_TaskInfo *TK_SpawnNewThreadB(TKPE_TaskInfo *btask, void *func, void *uptr)
{
	return(TK_SpawnNewThread2B(btask, func, uptr, NULL));
}

#else

TKPE_TaskInfo *TK_SpawnNewThread2B(
	TKPE_TaskInfo *btask, void *func, void *uptr, TKPE_CreateTaskInfo *info)
{
	return(NULL);
}

TKPE_TaskInfo *TK_SpawnNewThreadB(TKPE_TaskInfo *btask, void *func, void *uptr)
{
	return(TK_SpawnNewThread2B(btask, func, uptr, NULL));
}

#endif
#endif


int TK_SpawnSyscallTask(TKPE_TaskInfo *btask)
{
#ifndef __TK_CLIB_ONLY__
#ifdef __BJX2__
	TKPE_TaskInfo *sctask;
	TKPE_TaskInfoKern *taskern;

	sctask=TK_SpawnNewThreadB(btask, TK_Task_SyscallLoop, btask);
	tk_task_syscall=sctask;

	sctask->ystatus=1024;

	taskern=(TKPE_TaskInfoKern *)(sctask->krnlptr);
	taskern->ctx_regsave[TKPE_REGSAVE_EXSR]|=0xC000000000000000ULL;
#endif
#endif
}


int TK_Task_ShellLoop(void *uptr)
{
#ifndef __TK_CLIB_ONLY__
	char tb_cwd[256];
	char tbuf[256];

	TKPE_TaskInfo *task, *task2;
	TKPE_TaskInfoKern *taskern;
	int umsg;
	u64 *argsl;
	void *sobj, *rptr, *args;
	int		rc;

	task=NULL;	sobj=NULL;
	umsg=NULL;	rptr=NULL;
	args=NULL;

	task=(TKPE_TaskInfo *)TK_GET_TBR;

	TK_YieldCurrentThread();

	tk_dbg_printf("TK_Task_ShellLoop: Start task=%p tty=%08X pid=%04X\n",
		task, task->ttyid, task->pid);

	if(task->pid==3)
	{
		TKSH_TryLoad_ext("/boot/autoexec", NULL);
//		TKSH_TryLoad_ext("autoexec", NULL);
	}

	while(1)
	{
		if(tk_iskernel() &&
			tk_con_isdisabled() &&
			!tk_get_ttyid())
		{
			tk_con_chkreset();
		}

		TK_Env_GetCwd(tb_cwd, 256);
//		tk_con_chkreset();
		tk_puts(tb_cwd);
		tk_puts("$ ");
//		tk_puts("$  \b");
		tk_gets(tbuf);
		TKSH_ExecCmd(tbuf);
		
		while(TKSH_IsCmdEntryMode())
		{
			tk_gets(tbuf);
			TKSH_ExecCmdEntry(tbuf);
		}
	}

	/* control should never get here. */
	__debugbreak();
#endif
}

int TK_SpawnShellTask(TKPE_TaskInfo *btask, int ttyid)
{
#ifndef __TK_CLIB_ONLY__
	TKPE_CreateTaskInfo t_tinf;
	TKPE_CreateTaskInfo *tinf;
	TKPE_TaskInfo *sctask;
	TKPE_TaskInfoKern *taskern;

	tinf=&t_tinf;
	tinf->szInfo=sizeof(TKPE_CreateTaskInfo);
	tinf->idTty=ttyid;
	tinf->szStack=0;	//use default
	tinf->flSched=1;

//	sctask=TK_SpawnNewThread2(TK_Task_ShellLoop, btask, tinf);
	sctask=TK_SpawnNewThread2B(btask, TK_Task_ShellLoop, btask, tinf);
//	tk_task_syscall=sctask;

//	sctask->ttyid=ttyid;

	tk_dbg_printf("TK_SpawnShellTask: Start task=%p tty=%08X pid=%04X\n",
		sctask, sctask->ttyid, sctask->pid);

	taskern=(TKPE_TaskInfoKern *)(sctask->krnlptr);
	taskern->ctx_regsave[TKPE_REGSAVE_EXSR]|=0xC000000000000000ULL;
#endif
}



int tk_tlsid = 1;

int TK_AllocNewTlsGbl()
{
	return(tk_tlsid++);
}

int TK_AllocNewTlsB(TKPE_TaskInfo *task)
{
	TKPE_TaskInfo *ptask;
	int ix;

	ptask=TK_GetTaskProcessTask(task);
	if(!ptask)
		return(-1);

//	if(ptask->n_tlsv<=0)
//		ptask->n_tlsv=1;
	ix=ptask->n_tlsv++;
	return(ix+512);

//	return(tk_tlsid++);
}

int TK_AllocNewTlsA(void)
{
	TK_SysArg ar[4];
	void *p;
	int tid;

#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
#ifdef __BJX2__
		tid=TK_AllocNewTlsB((TKPE_TaskInfo *)__arch_tbr);
		return(tid);
#endif
	}
#endif

	tid=0;
	tk_syscall(NULL, TK_UMSG_TLSNEW, &tid, ar);
	return(tid);
}

s64 TK_TlsGetB(TKPE_TaskInfo *task, int key)
{
	s64 *tlsd;
	tlsd=(s64 *)(task->tlsptr);
	return(tlsd[key]);
}

s64 TK_TlsSetB(TKPE_TaskInfo *task, int key, s64 val)
{
	s64 *tlsd;
	s64 oval;
	
	tlsd=(s64 *)(task->tlsptr);
	oval=tlsd[key];
	tlsd[key]=val;
	return(oval);
}

s64 TK_TlsGet(int key)
{
	TKPE_TaskInfo *task;
	s64 *tlsd;
	
//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	tlsd=(s64 *)(task->tlsptr);
	return(tlsd[key]);
}

s64 TK_TlsSet(int key, s64 val)
{
	TKPE_TaskInfo *task;
	s64 *tlsd;
	s64 oval;
	
//	task=(TKPE_TaskInfo *)__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	tlsd=(s64 *)(task->tlsptr);
	oval=tlsd[key];
	tlsd[key]=val;
	return(oval);
}



tk_kptr TK_PboGbrGetB(TKPE_TaskInfo *task, int key)
{
	tk_kptr *tlsd;
	tlsd=(tk_kptr *)(task->imggbrptrs);
	return(tlsd[key]);
}

tk_kptr TK_PboGbrSetB(TKPE_TaskInfo *task, int key, tk_kptr val)
{
	tk_kptr *tlsd;
	tk_kptr oval;
	tlsd=(tk_kptr *)(task->imggbrptrs);
	oval=tlsd[key];
	tlsd[key]=val;
	return(oval);
}

tk_kptr TK_PboImgBaseGetB(TKPE_TaskInfo *task, int key)
{
	tk_kptr *tlsd;
	tlsd=(tk_kptr *)(task->imgbaseptrs);
	return(tlsd[key]);
}

tk_kptr TK_PboImgBaseSetB(TKPE_TaskInfo *task, int key, tk_kptr val)
{
	tk_kptr *tlsd;
	tk_kptr oval;
	tlsd=(tk_kptr *)(task->imgbaseptrs);
	oval=tlsd[key];
	tlsd[key]=val;
	return(oval);
}

