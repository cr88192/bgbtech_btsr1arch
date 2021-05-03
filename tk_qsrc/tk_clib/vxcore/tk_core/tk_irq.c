static int (*irq_timer[16])();
static int n_irq_timer=0;

int irq_addTimerIrq(void *fcn)
{
	if(!tk_iskernel())
	{
		TK_SetUserIrqV(0xC001, fcn);
		return(1);
	}

	irq_timer[n_irq_timer++]=fcn;
	return(1);
}

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

int TK_SetUserIrqV(int irq, void *ptr)
{
	TK_SysArg ar[4];
	void *p;
	int i;
	
	ar[0].i=irq;
	ar[1].p=ptr;
	i=tk_syscall(NULL, TK_UMSG_SETUSRIRQ, &p, ar);
	return(i);
}

extern int __arch_exsr;
extern void *__arch_tbr;

// int __isr_interrupt(int irq)
#ifdef __BGBCC__
__interrupt
#endif
void __isr_interrupt(void)
{
	int i, irq;

//	irq=__get_exsr();
	irq=__arch_exsr;

	if(((u16)irq)==0xC001)
	{
		for(i=0; i<n_irq_timer; i++)
			irq_timer[i]();
		return;
	}

	tk_printf("IRQ %X?\n", irq);
//	return;
}

TKPE_TaskInfo *tk_task_list[1024];
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
	TKPE_TaskInfo *task;
	int pid;

	pid=TK_FindFreePid();
	if(pid<=0)
		return(NULL);

	task=tk_malloc(sizeof(TKPE_TaskInfo));
	memset(task, 0, sizeof(TKPE_TaskInfo));
	task->pid=pid;
	tk_task_list[pid]=task;
	
	return(task);
}

int TK_DestroyTaskInfo(void *tkptr)
{
#if 1
	TKPE_TaskInfo *task;
	int pid;
	
	tk_puts("TK_DestroyTask\n");

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
	__arch_tbr=task;
}

TKPE_TaskInfo *TK_GetCurrentTask()
{
	TKPE_TaskInfo *task;

	task=__arch_tbr;
	return(task);
}

TK_EnvContext *TK_GetCurrentEnvContext()
{
	TKPE_TaskInfo *task;
	TK_EnvContext *env;

	task=__arch_tbr;
	if(!task)
	{
		task=TK_AllocNewTask();
		TK_SetCurrentTask(task);
//		return(NULL);
	}
	env=(void *)(task->envctx);
	if(env)
		return(env);

	tk_printf("TK_GetCurrentEnvContext: New %p:%p\n", task, env);

	env=TK_EnvCtx_AllocContext();
	task->envctx=(tk_kptr)env;
	return(env);
}

static void *tk_dummyallocaptr;

void **TK_GetAllocaMark()
{
	TKPE_TaskInfo *task;
//	TK_EnvContext *env;

	task=__arch_tbr;
	if(!task)
	{
		return(&tk_dummyallocaptr);
//		return(NULL);
	}
	return(&(task->allocaptr));
}

int TK_TaskAddPageAlloc(TKPE_TaskInfo *task, void *base, int size)
{
	int i;

	if(!task || !base)
		return(0);
		
	i=task->n_span++;
	task->span_ptr[i]=base;
	task->span_sz[i]=size;
	return(i);
}

int TK_TaskGetCwd(TKPE_TaskInfo *task, char *buf, int sz)
{
	if(task->envctx)
	{
//		strncpy(buf, task->envctx->cwd, sz);
		TK_EnvCtx_GetCwd(task->envctx, buf, sz);
		return(1);
	}
	
	return(-1);
}

int TK_TaskSetCwd(TKPE_TaskInfo *task, char *buf)
{
	if(task->envctx)
	{
		TK_EnvCtx_SetCwd(task->envctx, buf);
		return(1);
	}
	
	return(-1);
}

#ifdef __TK_CLIB_ONLY__
TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	return(-1);
}
#endif


void TK_FlushCacheL1D_INVDC(void *ptr);
void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz);
__asm {
TK_FlushCacheL1D_INVDC:
	INVDC	R4
	NOP
	NOP
	RTS

TK_FlushCacheL1D_ReadBuf:
	.L0:
	MOV.Q	(R4), R7
	ADD		-16, R5
	ADD		16, R4
	CMP/GT	0, R5
	BT		.L0
	RTS
};

void TK_FlushCacheL1D()
{
	u64 *pptr;
	int i, j, k;

	pptr=(u64 *)tk_task_list;	/* need memory to read from. */
	pptr=(u64 *)(((u64)pptr)&(~16777215));	/* Align */

	TK_FlushCacheL1D_INVDC(NULL);
//	TK_FlushCacheL1D_ReadBuf(pptr, 65536);
	TK_FlushCacheL1D_ReadBuf(pptr, 262144);
	TK_FlushCacheL1D_INVDC(NULL);
//	TK_FlushCacheL1D_ReadBuf(pptr, 65536);
	TK_FlushCacheL1D_ReadBuf(pptr, 262144);
}
