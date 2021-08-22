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
//	__debugbreak();
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
//		__debugbreak();
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

void TK_FlushCacheL1D()
{
	u64 *pptr;
	int i, j, k;

	pptr=(u64 *)tk_task_list;	/* need memory to read from. */
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

u64 TK_GetRandom();

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

void *TK_WithKrrSetuid(void *func, u64 key)
{
	TK_SysArg ar[4];
	void *p;
	
	if(tk_iskernel())
	{
		p=TK_WithKrrSetuidB(func, key);
		return(p);
	}

//	tk_printf("TK_WithKeySetuid: sz=%d\n", sz);
	
	p=NULL;
	ar[0].p=func;
	ar[1].i=key;
	tk_syscall(NULL, TK_UMSG_WITHKRRSETUID, &p, ar);

//	tk_printf("TK_WithKeySetuid: Vp=%p, p=%p\n", &p, p);

	return(p);

}

