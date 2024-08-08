void __halt(void);
u64 tk_gettimeus_v(void);
void tk_sprintf(char *dst, char *str, ...);
void tk_con_putc(int ch);

int TKUSB_KbHit(void);
int TKUSB_KbTryGetch(void);

int tk_ps2kb_kbhit(void);
int tk_ps2trygetch(void);
int tk_ps2getch(void);

void tk_vsprintf(char *buf, char *str, va_list lst);

_tkgdi_context_t *TKGDI_GetCurrentGdiContext();

char *tk_puts_gbltemp;
TK_SysArg *tk_puts_tempargs;

byte tk_dbg_iscopy;

void tk_dbg_putc_i(int val)
{
	while(P_MMIO_DEBUG_STS&8);
	P_MMIO_DEBUG_TX=val;

//	__debugbreak();
}

#ifdef __riscv
u64 MMIO_BASE_E = 0xFFFFF000E000ULL;
#endif

static char *tk_hextab="0123456789ABCDEF";
static char *tk_hextab_lc="0123456789abcdef";

void tk_dbg_putc(int val)
{
	if(val<=0)
		return;

	if(val<=0x7F)
	{
		tk_dbg_putc_i(val);
		return;
	}

	if((val>=0x600) && (val<=0x6FF))
	{
		tk_dbg_putc_i(tk_hextab[(val>>4)&15]);
		tk_dbg_putc_i(tk_hextab[(val>>0)&15]);
		return;
	}

	if(val<=0x7FF)
	{
		tk_dbg_putc_i(0xC0|((val>>6)&0x1F));
		tk_dbg_putc_i(0x80|((val   )&0x3F));
		return;
	}

	if(val<=0xFFFF)
	{
		tk_dbg_putc_i(0xE0|((val>>12)&0x0F));
		tk_dbg_putc_i(0x80|((val>> 6)&0x3F));
		tk_dbg_putc_i(0x80|((val    )&0x3F));
		return;
	}

	if(val<=0x1FFFFF)
	{
		tk_dbg_putc_i(0xF0|((val>>18)&0x07));
		tk_dbg_putc_i(0x80|((val>>12)&0x3F));
		tk_dbg_putc_i(0x80|((val>> 6)&0x3F));
		tk_dbg_putc_i(0x80|((val    )&0x3F));
		return;
	}
}

int tk_dbg_kbhit(void)
{
//	return(tk_ps2kb_kbhit());
	return(P_MMIO_DEBUG_STS&1);
}

int tk_dbg_getch(void)
{
	while(!(P_MMIO_DEBUG_STS&1))
	{
		__halt();
//		sleep_0();
	}
	return(P_MMIO_DEBUG_RX);
}

#ifdef __TK_CLIB_ONLY__
void tk_con_disable()
{
}
#endif


#ifndef __TK_CLIB_ONLY__
void tk_putc_tty(int val, int tty)
{
	char tb[16];
	_tkgdi_context_t *ctx;
	TKGHDC hdc;

	if((tty&0xF0000000)==0x10000000)
	{
		ctx=TKGDI_GetCurrentGdiContext();
		hdc=tty&0x00FFFFFF;
		tb[0]=val;	tb[1]=0;
		ctx->vt->DrawString(ctx, hdc, -1, -1, tb, 0, 0);
		return;
	}
}

void tk_putc_i(int val)
{
	TKPE_TaskInfo *task;
	int ttyid, hstdo;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
	{
		ttyid=task->ttyid;
		hstdo=task->redir_stdout;
	}
	else
	{
		ttyid=0;
		hstdo=0;
	}

	if(hstdo)
	{
		tk_hputc(task, hstdo, val);
		return;
	}

	if(val=='\n')
	{
//		if(!tk_dbg_iscopy)
			tk_dbg_putc('\r');

		if(ttyid && !tk_issyscall())
			{ tk_putc_tty('\r', ttyid); }
		else
			{ tk_con_putc('\r'); }
	}
//	if(!tk_dbg_iscopy)
		tk_dbg_putc(val);

	if(ttyid && !tk_issyscall())
		{ tk_putc_tty(val, ttyid); }
	else
		{ tk_con_putc(val); }
}
#endif

void *tk_puts_gettempargs()
{
	static TK_SysArg t_arr[16];
	TK_SysArg *ar;
	static int rec=0;

//	if(rec)
	if(1)
	{
		ar=t_arr;
		return(ar);
	}

#if 0
	rec=1;

	ar=tk_puts_tempargs;
	if(!ar)
	{
		tk_puts_tempargs=tk_malloc_cat(16*sizeof(TK_SysArg), TKMM_MCAT_GLOBAL);
		ar=tk_puts_tempargs;
	}

	rec=0;
	return(ar);
#endif
}

void tk_putc_v(int val)
{
	TK_SysArg ar[4];
//	TK_SysArg *ar;
	void *p;
	
//	ar=tk_puts_gettempargs();
	
	p=NULL;
	ar[0].i=val;
	tk_syscall(NULL, TK_UMSG_CONPUTC, &p, ar);
}

void tk_putsn_v(char *str, int n)
{
	TK_SysArg ar[4];
//	TK_SysArg *ar;
	char *cs;
	void *p;
	int k;

//	ar=tk_puts_gettempargs();
	
	if(!tk_puts_gbltemp)
	{
		tk_puts_gbltemp=tk_malloc_cat(256, TKMM_MCAT_GLOBAL);
	}

	if(n<256)
	{
		memcpy(tk_puts_gbltemp, str, n);
		tk_puts_gbltemp[n]=0;

		p=NULL;
		ar[0].p=tk_puts_gbltemp;
		ar[1].i=n;
		tk_syscall(NULL, TK_UMSG_CONPUTSN, &p, ar);
		return;
	}

	cs=str; k=n;
	while(k>=252)
	{
		memcpy(tk_puts_gbltemp, cs, 252);
		tk_puts_gbltemp[252]=0;

		p=NULL;
		ar[0].p=tk_puts_gbltemp;
		ar[1].i=252;
		tk_syscall(NULL, TK_UMSG_CONPUTSN, &p, ar);
		
		cs+=252;
		k-=252;
	}

	memcpy(tk_puts_gbltemp, cs, k);
	tk_puts_gbltemp[k]=0;

	p=NULL;
	ar[0].p=tk_puts_gbltemp;
	ar[1].i=k;
	tk_syscall(NULL, TK_UMSG_CONPUTSN, &p, ar);
	return;

//	p=NULL;
//	ar[0].p=str;
//	ar[1].i=n;
//	tk_syscall(NULL, TK_UMSG_CONPUTSN, &p, ar);
}

void (*tk_putc_fn)(int val);

void tk_putc(int val)
{
#ifndef __TK_CLIB_ONLY__
	if(tk_putc_fn)
	{
		tk_putc_fn(val);
		return;
	}
	
	tk_putc_fn=tk_putc_i;
	if(!tk_iskernel())
	{
		tk_putc_fn=tk_putc_v;
	}

	tk_putc_fn(val);
	return;
#else
	tk_putc_v(val);
#endif
}

#ifndef __TK_CLIB_ONLY__
int tk_kbpump_tty(
	TKPE_TaskInfo *task,
	TKPE_TaskInfoUser *tusr,
	int ttyid)
{
	char tb[16];
	TKGDI_EVENT t_imsg;
	_tkgdi_context_t *ctx;
	TKGDI_EVENT *imsg;
	TKGHDC hdc;
	u32 fseq;
	int i, j, k, dn, kbeg, kend, lim;

	imsg=&t_imsg;

	if((ttyid&0xF0000000)==0x10000000)
	{
		ctx=TKGDI_GetCurrentGdiContext();
		hdc=ttyid&0x00FFFFFF;
		fseq=0;
		dn=0;
		lim=64;

		while(1)
		{
			if(lim<=0)
				break;
			lim--;
		
			j=ctx->vt->QueryDisplay(ctx, hdc, TKGDI_FCC_poll, NULL, imsg);
			if(j<1)
				break;
			if(imsg->fccMsg==0)
				break;
			if(imsg->fccMsg==TKGDI_FCC_keyb)
			{
				k=imsg->wParm1;
				kbeg=tusr->kbfifo_beg;
				kend=tusr->kbfifo_end;
				
				if(k<1)
					continue;

				if((k&0x7FFF)>0xFE)
				{
					tusr->kbfifo_dat[(kend+0)&255]=0x80;
					tusr->kbfifo_dat[(kend+1)&255]=(k>>8)&0xFF;
					tusr->kbfifo_dat[(kend+2)&255]=k&0xFF;
					tusr->kbfifo_end=kend+3;
				}else
					if((k&0x7FFF)>0x7E)
				{
					tusr->kbfifo_dat[(kend+0)&255]=((k>>8)&0x80)|0x7F;
					tusr->kbfifo_dat[(kend+1)&255]=k&0xFF;
					tusr->kbfifo_end=kend+2;
				}else
				{
					tusr->kbfifo_dat[kend&255]=((k>>8)&0x80)|(k&0x7F);
					tusr->kbfifo_end=kend+1;
				}
				
//				tk_dbg_printf("tk_kbpump_tty: Pump key: %d\n",
//					imsg->wParm1);
			}else
				if(imsg->fccMsg==TKGDI_FCC_mous)
			{
			}else
			{
				if(fseq==imsg->evSeqNum)
					dn=1;
				if(!fseq)
					fseq=imsg->evSeqNum;
				j=ctx->vt->QueryDisplay(ctx, hdc, TKGDI_FCC_poll, imsg, NULL);
				if(dn)
					break;
			}
		}

//		tb[0]=val;	tb[1]=0;
//		ctx->vt->DrawString(ctx, hdc, -1, -1, tb, 0, 0);
		return(0);
	}
}

int tk_kbhit_tty(int ttyid)
{
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *tusr;
	int kbeg, kend;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	tusr=(TKPE_TaskInfoUser *)(task->usrptr);
	tk_kbpump_tty(task, tusr, ttyid);
	kbeg=tusr->kbfifo_beg;
	kend=tusr->kbfifo_end;

	return(kbeg!=kend);
}

int tk_getch_tty(int ttyid)
{
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *tusr;
	int kbeg, kend;
	int k;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	tusr=(TKPE_TaskInfoUser *)(task->usrptr);
	tk_kbpump_tty(task, tusr, ttyid);
	kbeg=tusr->kbfifo_beg;
	kend=tusr->kbfifo_end;
	if(kbeg==kend)
		return(-1);
	k=tusr->kbfifo_dat[kbeg];
	tusr->kbfifo_beg=kbeg+1;
	return(k);
}

int tk_get_ttyid(void)
{
	TKPE_TaskInfo *task;
	int ttyid;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		ttyid=task->ttyid;
	else
		ttyid=0;
	return(ttyid);
}

int tk_get_redir_stdin(void)
{
	TKPE_TaskInfo *task;
	int ttyid;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		ttyid=task->redir_stdin;
	else
		ttyid=0;
	return(ttyid);
}

int tk_get_redir_stdout(void)
{
	TKPE_TaskInfo *task;
	int ttyid;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		ttyid=task->redir_stdout;
	else
		ttyid=0;
	return(ttyid);
}

int tk_set_redir_stdin(int hdl)
{
	TKPE_TaskInfo *task;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		task->redir_stdin=hdl;
	return(hdl);
}

int tk_set_redir_stdout(int hdl)
{
	TKPE_TaskInfo *task;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		task->redir_stdout=hdl;
	return(hdl);
}


int tk_get_next_redir_stdin(void)
{
	TKPE_TaskInfo *task;
	int ttyid;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		ttyid=task->next_redir_stdin;
	else
		ttyid=0;
	return(ttyid);
}

int tk_get_next_redir_stdout(void)
{
	TKPE_TaskInfo *task;
	int ttyid;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		ttyid=task->next_redir_stdout;
	else
		ttyid=0;
	return(ttyid);
}

int tk_set_next_redir_stdin(int hdl)
{
	TKPE_TaskInfo *task;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		task->next_redir_stdin=hdl;
	return(hdl);
}

int tk_set_next_redir_stdout(int hdl)
{
	TKPE_TaskInfo *task;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		task->next_redir_stdout=hdl;
	return(hdl);
}

int tk_kbhit_i(void)
{
	TKPE_TaskInfo *task;
	int ttyid;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
		ttyid=task->ttyid;
	else
		ttyid=0;

	if(ttyid && !tk_issyscall())
		{ return(tk_kbhit_tty(ttyid)); }

#if 1
	if(!tk_dbg_iscopy)
	{
		if(tk_dbg_kbhit())
			return(1);
	}
#endif
	if(TKUSB_KbHit())
		return(1);
	return(tk_ps2kb_kbhit());
}

int tk_getch_i(void)
{
	TKPE_TaskInfo *task;
	int ttyid, hstdi, sz;

	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(task)
	{
		ttyid=task->ttyid;
		hstdi=task->redir_stdin;
	}
	else
	{
		ttyid=0;
		hstdi=0;
	}

	if(hstdi)
	{
		sz=tk_hgetc(task, hstdi);
		if(sz<0)
			task->stdin_eof=1;
		return(sz);
	}

	if(ttyid && !tk_issyscall())
		{ return(tk_getch_tty(ttyid)); }

#if 1
	if(!tk_dbg_iscopy)
	{
		while(1)
		{
			if(tk_dbg_kbhit())
				return(tk_dbg_getch());
			if(TKUSB_KbHit())
				return(TKUSB_KbTryGetch());
			if(tk_ps2kb_kbhit())
				return(tk_ps2getch());
			if(tk_issyscall())
				break;
//			__halt();
			TK_YieldCurrentThread();
		}
	}
#endif
	while(1)
	{
		if(tk_ps2kb_kbhit())
			return(tk_ps2trygetch());
		if(TKUSB_KbHit())
			return(TKUSB_KbTryGetch());
		if(tk_issyscall())
			break;
		TK_YieldCurrentThread();
	}

	return(-1);
//	return(tk_ps2getch());
}
#endif

int tk_kbhit_v(void)
{
	TK_SysArg ar[4];
//	TK_SysArg *ar;
	int ttyid;
	int i;

#if 0
	ttyid=tk_get_ttyid();
	if(ttyid>0)
	{
		return(tk_kbhit_tty(ttyid));
	}
#endif

//	ar=tk_puts_gettempargs();
	
	i=0;
	ar[0].i=0;
	tk_syscall(NULL, TK_UMSG_CONKBHIT, &i, ar);
	return(i);
}

int tk_getch_v0(void)
{
	TK_SysArg ar[4];
//	TK_SysArg *ar;
	int ttyid;
	int i;

#if 0
	ttyid=tk_get_ttyid();

//	ar=tk_puts_gettempargs();

	if(ttyid && !tk_issyscall())
		{ return(tk_getch_tty(ttyid)); }
#endif

	i=0;
	ar[0].i=0;
	tk_syscall(NULL, TK_UMSG_CONGETCH, &i, ar);
	return(i);
}

int tk_getch_v(void)
{
	int i;
	while(1)
	{
		if(!tk_kbhit_v())
		{
			TK_YieldCurrentThread();
			continue;
		}
		i=tk_getch_v0();
		if(i>=0)
			break;
		TK_YieldCurrentThread();
	}
	return(i);
}

u64 tk_gettimeus_v(void)
{
	TK_SysArg ar[4];
//	TK_SysArg *ar;
	u64 li;

//	ar=tk_puts_gettempargs();
	
	li=0;
	ar[0].i=1;
//	tk_syscall(NULL, TK_UMSG_CONGETCH, &i, ar);
	tk_syscall(NULL, TK_UMSG_CONKBHIT, &li, ar);
	return(li);
}

int (*tk_kbhit_fn)(void);
int (*tk_getch_fn)(void);

int tk_kbhit(void)
{
#ifndef __TK_CLIB_ONLY__
	if(tk_kbhit_fn)
	{
		return(tk_kbhit_fn());
	}
	
	tk_kbhit_fn=tk_kbhit_i;
	if(!tk_iskernel() && !tk_issyscall())
	{
		tk_kbhit_fn=tk_kbhit_v;
	}

	return(tk_kbhit_fn());
#else
	return(tk_kbhit_v());
#endif
}

int tk_getch(void)
{
#ifndef __TK_CLIB_ONLY__
	if(tk_getch_fn)
	{
		return(tk_getch_fn());
	}
	
	tk_getch_fn=tk_getch_i;
	if(!tk_iskernel() && !tk_issyscall())
	{
		tk_getch_fn=tk_getch_v;
	}

	return(tk_getch_fn());
#else
	return(tk_getch_v());
#endif
}

byte *TK_EmitCharUtf8(byte *ct, int v)
{
	if(v<0x80)
		{ *ct++=v; }
	else if(v<0x0800)
	{
		*ct++=0xC0|((v>>6)&0x1F);
		*ct++=0x80|((v   )&0x3F);
	}
	else if(v<0x10000)
	{
		*ct++=0xE0|((v>>12)&0x0F);
		*ct++=0x80|((v>> 6)&0x3F);
		*ct++=0x80|((v    )&0x3F);
	}
	else if(v<0x200000)
	{
		*ct++=0xF0|((v>>18)&0x07);
		*ct++=0x80|((v>>12)&0x3F);
		*ct++=0x80|((v>> 6)&0x3F);
		*ct++=0x80|((v    )&0x3F);
	}
	return(ct);
}

int TK_ReadCharUtf8(byte **rcs)
{
	byte *cs;
	int i, j, k, l;

	cs=*rcs;
	i=*cs++;
	if(!(i&0x80))
	{
		*rcs=cs;
		return(i);
	}
	else
		if(!(i&0x40))	/* 80..BF */
	{
		*rcs=cs;
		return(i);		
	}else
		if(!(i&0x20))	/* C0..DF */
	{
		j=*cs;
		if((j&0xC0)==0x80)
		{
			cs++;
			i=((i&0x1F)<<6)|(j&0x3F);
			*rcs=cs;
			return(i);
		}
	}else
		if(!(i&0x10))	/* E0..EF */
	{
		j=cs[0];
		k=cs[1];
		if(((j&0xC0)==0x80) && ((k&0xC0)==0x80))
		{
			cs+=2;
			i=((i&0x0F)<<12)|((j&0x3F)<<6)|(k&0x3F);
			*rcs=cs;
			return(i);
		}
	}else
		if(!(i&0x08))	/* F0..F7 */
	{
		j=cs[0];
		k=cs[1];
		l=cs[2];
		if(((j&0xC0)==0x80) && ((k&0xC0)==0x80) && ((l&0xC0)==0x80))
		{
			cs+=3;
			i=((i&0x07)<<18)|((j&0x3F)<<12)|((k&0x3F)<<6)|(l&0x3F);
			*rcs=cs;
			return(i);
		}
	}
	
	*rcs=cs;
	return(i);
}

void tk_puts(char *msg)
{
#ifndef __TK_CLIB_ONLY__
	char *s;
	int i;
	
	if(!tk_iskernel())
	{
		tk_puts_n(msg, strlen(msg));
		return;
	}
	
	s=msg;
	while(*s)
	{
//		tk_putc(*s++);

		i=TK_ReadCharUtf8((byte **)(&s));
		tk_putc(i);
	}
#else
	tk_puts_n(msg, strlen(msg));
#endif
}

void (*tk_putsn_fn)(char *msg, int n);

int tk_putsn_check_nonascii(char *msg, int cnt)
{
	char *cs;
	int n;
	
	cs=msg; n=cnt;
	while(n>=8)
	{
		if((*(long long *)cs)&0x8080808080808080ULL)
			return(1);
		cs+=8; n-=8;
	}
	
	while(n>0)
	{
		if((*cs)&0x80)
			return(1);
		cs++; n--;
	}
	return(0);
}

void tk_puts_n_tty(char *msg, int len, int tty)
{
	_tkgdi_context_t *ctx;
	TKGHDC hdc;
	char *s;
	int n;

	if((tty&0xF0000000)==0x10000000)
	{
		if(!tk_puts_gbltemp)
		{
			tk_puts_gbltemp=tk_malloc_cat(256, TKMM_MCAT_GLOBAL);
		}

		ctx=TKGDI_GetCurrentGdiContext();
		hdc=tty&0x00FFFFFF;
		
		s=msg; n=len;
		while(n>=252)
		{
			memcpy(tk_puts_gbltemp, s, 252);
			tk_puts_gbltemp[252]=0;
			ctx->vt->DrawString(ctx, hdc, -1, -1, tk_puts_gbltemp, 0, 0);
			s+=252;
			n-=252;
		}
		
		memcpy(tk_puts_gbltemp, s, n);
		tk_puts_gbltemp[n]=0;
	
//		ctx->vt->DrawString(ctx, hdc, -1, -1, msg, 0, 0);
		ctx->vt->DrawString(ctx, hdc, -1, -1, tk_puts_gbltemp, 0, 0);
		return;
	}
}

void tk_puts_tty(char *msg, int tty)
{
	tk_puts_n_tty(msg, strlen(msg), tty);
}

void tk_puts_n(char *msg, int n)
{
	char tb[128];
	TKPE_TaskInfo *task;
	int ttyid, hstdo;

	char *s;
	int i, nonasc;

	nonasc=tk_putsn_check_nonascii(msg, n);

	if(tk_iskernel() && !nonasc)
	{
		task=(TKPE_TaskInfo *)TK_GET_TBR;
		if(task)
		{
			ttyid=task->ttyid;
			hstdo=task->redir_stdout;
		}
		else
		{
			ttyid=0;
			hstdo=0;
		}
		

		if(hstdo)
		{
			tk_hwrite(task, hstdo, msg, n);
			return;
		}

		if(ttyid && !tk_issyscall())
		{		
//			tk_puts_tty(msg, ttyid);
			tk_puts_n_tty(msg, n, ttyid);
			return;
		}
	}
	
	if(tk_iskernel() || nonasc)
	{
		s=msg;
		while(n--)
		{
	//		tk_putc(*s++);

			i=TK_ReadCharUtf8((byte **)(&s));
			tk_putc(i);
		}
		return;
	}
	
	tk_putsn_v(msg, n);
	
#if 0
	s=msg;
	while(n--)
	{
//		tk_putc(*s++);

		i=TK_ReadCharUtf8((byte **)(&s));
		tk_putc(i);
	}
#endif
}

void tk_dbg_puts_n(char *msg, int n)
{
	char *s;
	int i;
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel() || tk_issyscall())
	{
		s=msg;
		while(n--)
		{
			i=TK_ReadCharUtf8((byte **)(&s));
			tk_dbg_putc(i);
		}
		return;
	}
#endif
}

void tk_dbg_puts(char *msg)
{
#ifndef __TK_CLIB_ONLY__
	tk_dbg_puts_n(msg, strlen(msg));
#endif
}

void tk_gets(char *buf)
{
	char *t;
	int i;

	if(tk_issyscall())
		return;

//	tk_puts("\x1f\b");

	t=buf;
	while(1)
	{
#if 0
		if(tk_iskernel() &&
			tk_con_isdisabled() &&
			!tk_get_ttyid())
		{
			TK_YieldCurrentThread();
			continue;
		}
#endif
	
		i=tk_getch();
		if(i<=0)
		{
			if(tk_get_redir_stdin())
				break;
			TK_YieldCurrentThread();
			continue;
		}
		
//		if(i>=0x80)
//			continue;

		if(i>=0x7F)
		{
			if((i==0x7F) || (i==0xFF))
				i=tk_getch();
			continue;
		}
		
//		if((i=='\b') || (i==127))
		if(i=='\b')
		{
			if(t>buf)
			{
				tk_puts("\b \b");
//				tk_puts("\b\x1f \b\b");
				t--;
			}
			*t=0;
			continue;
		}
//		if(i=='\r')
//			{ tk_putc('\n'); break; }
//		if(i=='\n')
//			{ tk_putc('\n'); break; }

		if((i=='\r') || (i=='\n'))
		{
			tk_puts(" \b\n");
			break;
		}

		tk_putc(i);
		*t++=i;
		tk_puts(" \b");
//		tk_puts("\x1f\b");
	}
	*t=0;
}

void tk_gets_n(char *msg, int n)
{
	char *s;
	
	s=msg;
	while(n--)
		{ *s++=tk_getch(); }
}

char *tk_async_gets(char *buf)
{
	char *t;
	int i, eol;
	
	t=buf; eol=0;
	while(*t)
		t++;

	while(1)
	{
		if(!tk_kbhit())
			break;

		i=tk_getch();
		if(i<=0)
			continue;

		if(i>=0x7F)
			continue;

//		if((i=='\b') || (i==127))
		if(i=='\b')
		{
			if(t>buf)
			{
				tk_puts("\b \b");
				t--;
			}
			*t=0;
			continue;
		}
		if(i=='\r')
			{ tk_putc('\n'); eol=1; break; }
		if(i=='\n')
			{ tk_putc('\n'); eol=1; break; }
		tk_putc(i);
		*t++=i;
	}
	*t=0;
	if(eol)
		return(buf);
	return(NULL);
}

void tk_print_hex(u32 v)
{
	static char *chrs="0123456789ABCDEF";
//	int i;

//	char *chrs;
//	chrs="0123456789ABCDEF";

//	i=chrs[(v>>28)&15];
//	__debugbreak();

	tk_putc(chrs[(v>>28)&15]);
	tk_putc(chrs[(v>>24)&15]);
	tk_putc(chrs[(v>>20)&15]);
	tk_putc(chrs[(v>>16)&15]);
	tk_putc(chrs[(v>>12)&15]);
	tk_putc(chrs[(v>> 8)&15]);
	tk_putc(chrs[(v>> 4)&15]);
	tk_putc(chrs[(v    )&15]);
}

void tk_print_hex_u64(u64 v)
{
	tk_print_hex(v>>32);
	tk_print_hex(v);
}

void tk_print_hex_n(u64 v, int n)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	if(n>16)
	{
//		__debugbreak();

//		tk_puts("!%X!\n");
		tk_puts("!%X! ");
		tk_print_hex(n);
		tk_puts(": ");
		tk_print_hex(v);

		tk_puts("\n");
		__debugbreak();
	}

	if(n>8)
	{
		if(n>15)tk_putc(chrs[(v>>60)&15]);
		if(n>14)tk_putc(chrs[(v>>56)&15]);
		if(n>13)tk_putc(chrs[(v>>52)&15]);
		if(n>12)tk_putc(chrs[(v>>48)&15]);
		if(n>11)tk_putc(chrs[(v>>44)&15]);
		if(n>10)tk_putc(chrs[(v>>40)&15]);
		if(n> 9)tk_putc(chrs[(v>>36)&15]);
		if(n> 8)tk_putc(chrs[(v>>32)&15]);
	}

	if(n>7)tk_putc(chrs[(v>>28)&15]);
	if(n>6)tk_putc(chrs[(v>>24)&15]);
	if(n>5)tk_putc(chrs[(v>>20)&15]);
	if(n>4)tk_putc(chrs[(v>>16)&15]);
	if(n>3)tk_putc(chrs[(v>>12)&15]);
	if(n>2)tk_putc(chrs[(v>> 8)&15]);
	if(n>1)tk_putc(chrs[(v>> 4)&15]);
	if(n>0)tk_putc(chrs[(v    )&15]);
}

void tk_print_hexptr(long v)
{
//	if(sizeof(void *)>4)
	if(sizeof(long)>4)
	{
		tk_print_hex_n(v>>32, 4);
	}
	tk_print_hex((u32)v);
}

int tk_print_hex_genw(u64 v)
{
	u64 w;
	int i;

	w=v;	i=1;
	while(w>=16)
		{ w=w>>4; i++; }

	if(i<16)
	{
		if((v>>(i*4))!=0)
			__debugbreak();
	}

//	__debugbreak();

#if 0
	w=v; i=1;
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
#endif

	return(i);
}

void tk_print_decimal(int val)
{
	char tb[256];
	char *t, *te;
	int i, k, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb; te=tb+24;
	if(!k)*t++='0';	
	while(k>0)
	{
		if(t>te)
			{ __debugbreak(); }
	
		i=k%10;
		
//		if((i<0) | (i>=10))
//			__debugbreak();
		
		*t++='0'+i;
		k=k/10;
	}
	if(s)*t++='-';
	
	while(t>tb)
		{ t--; tk_putc(*t); }
}

void tk_print_decimal_n(int val, int num)
{
	char tb[256];
	char *t;
	int i, k, n, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
//	if(n>=256)
//		{ __debugbreak(); }
	
	t=tb; n=num;
//	if(!k)*t++=0;	
	while(n>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
		n--;
	}

//	if(s)*t++='-';
	
	while(t>tb)
		{ t--; tk_putc(*t); }
}

#ifdef ARCH_HAS_FPU
void tk_print_float(double val)
{
	int ip, fp, sg;
	
	
//	print_hex(((u32 *)(&val))[0]);
//	print_hex(((u32 *)(&val))[1]);
//	putc(' ');
	
	sg=0;
	if(val<0)
		{ val=-val; sg=1; }
	
//	print_hex(((u32 *)(&val))[0]);
//	print_hex(((u32 *)(&val))[1]);
//	putc(' ');

	ip=(int)val;
	fp=(int)((val-ip)*1000000);

	if(sg)tk_putc('-');
	tk_print_decimal(ip);
	tk_putc('.');
	tk_print_decimal_n(fp, 6);
}
#endif

#if 1
// void tk_printf(char *str, ...)
void tk_vprintf(char *str, va_list lst)
{
//	void **plst;
//	va_list lst;
	char pcfill;
	char *s, *s1;
	long long v;
	int i, w, ll, ljust;

//	plst=(void **)(&str);
//	plst++;
//	va_start(lst, str);
	
//	__debugbreak();
	
	s=str;
	while(*s)
	{
		if(*s!='%')
		{
//			tk_putc(*s++);

			i=TK_ReadCharUtf8((byte **)(&s));
			tk_putc(i);
			continue;
		}
		if(s[1]=='%')
			{ s+=2; tk_putc('%'); continue; }
		s++;
		
		if(*s=='-')
		{
			ljust=1;
			s++;
		}else
		{
			ljust=0;
		}
		
		if(*s=='0')
		{
			pcfill='0';
			s++;
		}else
		{
			pcfill=' ';
		}
		
		w=0;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				w=(w*10)+((*s++)-'0');
		}
		
		ll=0;
		if((*s=='l') || (*s=='L'))
		{
			ll=1;
			s++;
			if(*s=='l')
			{
				s++;
			}
		}
		
		
		switch(*s++)
		{
		case 'd':
			if(ll)
			{
				v=va_arg(lst, long long);
			}else
			{
	//			v=(int)(*plst++);
				v=va_arg(lst, int);
			}

			if(w)
			{
				tk_print_decimal_n(v, w);
			}else
			{
				tk_print_decimal(v);
			}
			break;

		case 'X':
		case 'x':
			if(ll)
			{
				v=va_arg(lst, long long);
			}else
			{
//				if(!w)w=8;
//				v=(int)(*plst++);
				v=va_arg(lst, int);
			}

//			__debugbreak();

			if(!w)w=tk_print_hex_genw(v);

//			__debugbreak();
//			print_hex(v);
			tk_print_hex_n(v, w);
//			__debugbreak();
			break;

		case 's':
//			s1=*plst++;
			s1=va_arg(lst, char *);
			if(!s1)
				s1="(null)";
			if(w)
			{
				i=w-strlen(s1);
				if(ljust)
				{
					tk_puts(s1);
					while(i>0)
					{
						tk_putc(' ');
						i--;
					}
				}else
				{
					while(i>0)
					{
						tk_putc(' ');
						i--;
					}
					tk_puts(s1);
				}
			}else
			{
				tk_puts(s1);
			}
			break;

		case 'p':
			s1=va_arg(lst, char *);
			tk_print_hexptr((long)s1);
			break;

#ifdef ARCH_HAS_FPU
		case 'f':
			tk_print_float(va_arg(lst, double));
			break;
#endif

		default:
//			plst++;
			break;
		}
	}
	va_end(lst);
}

void tk_printf(char *str, ...)
{
	va_list lst;

	va_start(lst, str);
	tk_vprintf(str, lst);
	va_end(lst);
}
#endif

#if 0
// void tk_sprintf(char *buf, char *str, ...)
void tk_vsprintf(char *buf, char *str, va_list lst)
{
//	void **plst;
//	va_list lst;
	char pcfill;
	char *s, *s1, *ct;
	int v, w;

//	plst=(void **)(&str);
//	plst++;
//	va_start(lst, str);
	
//	__debugbreak();
	
	ct=buf;
	s=str;
	while(*s)
	{
		if(*s!='%')
			{ *ct++=(*s++); continue; }
		if(s[1]=='%')
			{ s+=2; *ct++='%'; continue; }
		s++;
		
		if(*s=='0')
		{
			pcfill='0';
			s++;
		}else
		{
			pcfill=' ';
		}
		
		w=0;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				w=(w*10)+((*s++)-'0');
		}
		
		switch(*s++)
		{
		case 'd':
#if 0
//			v=(int)(*plst++);
			v=va_arg(lst, int);
			if(w)
			{
				tk_print_decimal_n(v, w);
			}else
			{
				tk_print_decimal(v);
			}
#endif
			break;
		case 'X':
#if 0
//			if(!w)w=8;
//			v=(int)(*plst++);
			v=va_arg(lst, int);

//			__debugbreak();

			if(!w)w=tk_print_hex_genw(v);

//			__debugbreak();
//			print_hex(v);
			tk_print_hex_n(v, w);
//			__debugbreak();
#endif
			break;
		case 's':
//			s1=*plst++;
			s1=va_arg(lst, char *);
			if(!s1)
				s1="(null)";

//			tk_puts(s1);
			while(*s1)
				*ct++=*s1++;
			break;

		case 'p':
			s1=va_arg(lst, char *);
			tk_print_hexptr((long)s1);
			break;

#ifdef ARCH_HAS_FPU
//		case 'f':
//			tk_print_float(va_arg(lst, double));
//			break;
#endif

		default:
//			plst++;
			break;
		}
	}
	*ct=0;
//	va_end(lst);
}
#endif

void tk_sprintf(char *buf, char *str, ...)
{
	va_list lst;

	va_start(lst, str);
	tk_vsprintf(buf, str, lst);
	va_end(lst);
}

void tk_dbg_printf(char *str, ...)
{
//	static char tbuf[512];
//	char tbuf[512];
	char tbuf[256];
	va_list lst;

	va_start(lst, str);
	
//	__debugbreak();
	
	tk_vsprintf(tbuf, str, lst);
	tk_dbg_puts(tbuf);
	va_end(lst);
}



byte *tk_ralloc_bufs=NULL;
byte *tk_ralloc_bufe;
byte *tk_ralloc_bufr;

void *tk_ralloc(int sz)
{
	void *p;
	if(!tk_ralloc_bufs)
	{
		tk_ralloc_bufs=malloc(4096);
		tk_ralloc_bufe=tk_ralloc_bufs+4096;
		tk_ralloc_bufr=tk_ralloc_bufs;
	}
	
	if((tk_ralloc_bufr+sz)>=tk_ralloc_bufe)
	{
		tk_ralloc_bufr=tk_ralloc_bufs;
	}
	
	p=tk_ralloc_bufr;
	tk_ralloc_bufr+=sz;
	return(p);
}

char *tk_rstrdup(char *s)
{
	char *t;
	int l;
	
	l=strlen(s);
	t=tk_ralloc(l+1);
	memcpy(t, s, l+1);
	return(t);
}

char **tk_rsplit_sep(char *str, int sep)
{
	char tb[64];
	char *ta[32];
	char **ta2;
	char *s, *t;
	int i, nta;
	
	nta=0;
	s=str;
	while(*s)
	{
		while(*s && (*s<=' '))
			s++;
		
		if(*s=='\"')
		{
			s++;
			
			t=tb;
			while(*s && (*s!='\"'))
			{
				if(*s=='\\')
				{
					s++;
					i=*s++;
					switch(i)
					{
					case '\\':	*t++='\\'; break;
					case 'r':	*t++='\r'; break;
					case 'n':	*t++='\n'; break;
					case 't':	*t++='\t'; break;
					default:	*t++=i; break;
					}
					continue;
				}
				*t++=*s++;
			}
			if(*s=='\"')s++;
			*t++=0;
			ta[nta++]=tk_rstrdup(tb);
			continue;
		}

		if(sep<=' ')
		{
			if(*s>' ')
			{
				t=tb;
				while(*s>' ')
					*t++=*s++;
				*t++=0;
				ta[nta++]=tk_rstrdup(tb);
				continue;
			}
		}else
		{
			if(*s)
			{
				t=tb;
				while(*s && (*s!=sep))
					*t++=*s++;
				*t++=0;
				ta[nta++]=tk_rstrdup(tb);
				if(*s && (*s==sep))
					s++;
				continue;
			}
		}
	}
	
	ta2=tk_ralloc((nta+1)*sizeof(char *));
	for(i=0; i<nta; i++)
		ta2[i]=ta[i];
	ta2[i]=NULL;
	return(ta2);
}

char **tk_rsplit(char *str)
{
	return(tk_rsplit_sep(str, ' '));
}

char *tk_getstrline(char *buf, int sz, char *str)
{
	char *cs, *ct;
	cs=str;
	ct=buf;
	while(*cs && (*cs!='\r') && (*cs!='\n'))
		*ct++=*cs++;
	*ct=0;
	if(*cs=='\r')
		cs++;
	if(*cs=='\n')
		cs++;
	return(cs);
}

TK_FILE *tk_dbg_recvfile;

int TK_Dbg_RecvFileXM(char *name)
{
	byte buf[256];
	int ch, state, nbufb, blkn, blkn2, blkskp, csum;
	int i, j, k;

	tk_dbg_recvfile=tk_fopen(name, "wb");
	if(!tk_dbg_recvfile)
	{
		tk_printf("Fail Open %s\n", name);
//		return(-1);
	}
	
	tk_dbg_iscopy=1;
	state=1;
	nbufb=0;
	blkn=1;
	blkskp=0;
	
	while(state>0)
	{
		if(!tk_dbg_kbhit())
		{
			__halt();
			continue;
		}

		ch=tk_dbg_getch();
		
		switch(state)
		{
		case 1:
			if(ch==0x01)	/* SOH: Start of Packet */
			{
				if(tk_dbg_recvfile && (nbufb>=128))
					{ tk_fwrite(buf, 1, 128, tk_dbg_recvfile); }
				state=2; break;
			}
			if(ch==0x04)	/* EOT: End of Data */
			{
				while((nbufb>0) && (buf[nbufb-1]==0x1A))
					nbufb--;
				if(tk_dbg_recvfile && (nbufb>=0))
					{ tk_fwrite(buf, 1, nbufb, tk_dbg_recvfile); }
				state=0;
				break;
			}

			if(ch==0x03)	/* ETX: CTRL-C / Cancel */
			{
				state=0;
				break;
			}
			
			/* Unknown Byte, Ignore. */
			break;

		case 2:		/* First byte of header */
			blkskp=0;
			blkn2=(ch&255);
			if((blkn2!=blkn) && (blkn2!=((blkn-1)&255)))
			{
				state=1;
				tk_dbg_putc(0x15);	/* NAK */
				break;
			}
			state=3;
			break;

		case 3:		/* Second byte of header */
//			if((255-(ch&255))!=blkn)
			if((255-(ch&255))!=blkn2)
			{
				state=1;
				tk_dbg_putc(0x15);	/* NAK */
				break;
			}
			state=4;
			nbufb=0;
			csum=0;
			break;

		case 4:		/* Packet Data */
			buf[nbufb++]=ch;
			csum=(csum+ch)&255;
			if(nbufb>=128)
				state=5;
			break;

		case 5:		/* Checksum Byte */
//			k=0;
//			for(j=0; j<128j j++)
//				k+=buf[j];
//			if((k&255)!=(ch&255))
			if(csum!=(ch&255))
			{
				tk_dbg_putc(0x15);	/* NAK */
				break;
			}
			
			if(blkn!=blkn2)
			{
				nbufb=0;
			}
			
//			blkn=(blkn+1)&255;
			blkn=(blkn2+1)&255;
			tk_dbg_putc(0x06);	/* ACK */
			state=1;
			break;
		}
	}

	tk_dbg_iscopy=0;
	
	if(tk_dbg_recvfile)
	{
		tk_fclose(tk_dbg_recvfile);
		tk_dbg_recvfile=NULL;
	}
	return(0);
}


s64 tk_strtol(char *str, int rdx)
{
	char *s;
	s64 t;
	int i, j;
	
	s=str; t=0;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		i=*s++; j=-1;
		if((i>='0') && (i<='9'))
			j=0+(i-'0');
		if((i>='A') && (i<='Z'))
			j=10+(i-'A');
		if((i>='a') && (i<='z'))
			j=10+(i-'a');
		if((j<0) || (j>=rdx))
			break;
		t=t*rdx+j;
	}
	return(t);
}

s64 tk_atoi(char *str)
{
	s64 li;
	char *s;
	int sg;
	
	if(*str=='0')
	{
		if(str[1]=='x')
			return(tk_strtol(str+2, 16));
		if(str[1]=='d')
			return(tk_strtol(str+2, 10));
		if(str[1]=='b')
			return(tk_strtol(str+2, 2));
		return(tk_strtol(str+1, 8));
	}
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }
	li=tk_strtol(s, 10);
	if(sg)li=-li;
	return(li);
}

int TK_FormatUuidAsString(char *str, void *ruid)
{
	byte *vuid;
	int i0, i1, i2, i3;

	vuid=ruid;
	i0=vuid[0];	i1=vuid[1];
	i2=vuid[2];	i3=vuid[3];
	str[0]=tk_hextab_lc[(i0>>4)&15];
	str[1]=tk_hextab_lc[(i0>>0)&15];
	str[2]=tk_hextab_lc[(i1>>4)&15];
	str[3]=tk_hextab_lc[(i1>>0)&15];
	str[4]=tk_hextab_lc[(i2>>4)&15];
	str[5]=tk_hextab_lc[(i2>>0)&15];
	str[6]=tk_hextab_lc[(i3>>4)&15];
	str[7]=tk_hextab_lc[(i3>>0)&15];
	str[8]='-';
	i0=vuid[4];	i1=vuid[5];
	i2=vuid[6];	i3=vuid[7];
	str[ 9]=tk_hextab_lc[(i0>>4)&15];
	str[10]=tk_hextab_lc[(i0>>0)&15];
	str[11]=tk_hextab_lc[(i1>>4)&15];
	str[12]=tk_hextab_lc[(i1>>0)&15];
	str[13]='-';
	str[14]=tk_hextab_lc[(i2>>4)&15];
	str[15]=tk_hextab_lc[(i2>>0)&15];
	str[16]=tk_hextab_lc[(i3>>4)&15];
	str[17]=tk_hextab_lc[(i3>>0)&15];
	str[18]='-';
	i0=vuid[ 8];	i1=vuid[ 9];
	i2=vuid[10];	i3=vuid[11];
	str[19]=tk_hextab_lc[(i0>>4)&15];
	str[20]=tk_hextab_lc[(i0>>0)&15];
	str[21]=tk_hextab_lc[(i1>>4)&15];
	str[22]=tk_hextab_lc[(i1>>0)&15];
	str[23]='-';
	str[24]=tk_hextab_lc[(i2>>4)&15];
	str[25]=tk_hextab_lc[(i2>>0)&15];
	str[26]=tk_hextab_lc[(i3>>4)&15];
	str[27]=tk_hextab_lc[(i3>>0)&15];
	i0=vuid[12];	i1=vuid[13];
	i2=vuid[14];	i3=vuid[15];
	str[28]=tk_hextab_lc[(i0>>4)&15];
	str[29]=tk_hextab_lc[(i0>>0)&15];
	str[30]=tk_hextab_lc[(i1>>4)&15];
	str[31]=tk_hextab_lc[(i1>>0)&15];
	str[32]=tk_hextab_lc[(i2>>4)&15];
	str[33]=tk_hextab_lc[(i2>>0)&15];
	str[34]=tk_hextab_lc[(i3>>4)&15];
	str[35]=tk_hextab_lc[(i3>>0)&15];
	str[36]=0;
	return(0);
}

int TK_FormatGuidAsString(char *str, void *ruid)
{
	TK_FormatUuidAsString(str+1, ruid);
	str[ 0]='{';
	str[37]='}';
	str[38]=0;
	return(0);
}

int TK_ParseHexByteFromString(char *cs)
{
	int c0, c1, i0, i1;
	int i;

	c0=cs[0];	c1=cs[1];
	i0=-1;		i1=-1;
	if((c0>='0') && (c0<='9'))
		i0=c0-'0';
	if((c0>='A') && (c0<='F'))
		i0=c0-('A'-10);
	if((c0>='a') && (c0<='f'))
		i0=c0-('a'-10);

	if((c1>='0') && (c1<='9'))
		i1=c1-'0';
	if((c0>='A') && (c1<='F'))
		i1=c1-('A'-10);
	if((c0>='a') && (c1<='f'))
		i1=c1-('a'-10);
	i=(i0<<4)|i1;
	return(i);
}

int TK_ParseUuidFromString(char *str, void *ruid)
{
	byte *vuid;
	int i0, i1, i2, i3;

	vuid=ruid;
	
	i0=str[ 8];	i1=str[13];
	i2=str[18];	i3=str[23];
	if((i0|i1|i2|i3)!='-')
		return(0);
	if((i0&i1&i2&i3)!='-')
		return(0);
	
	i0=TK_ParseHexByteFromString(str+ 0);
	i1=TK_ParseHexByteFromString(str+ 2);
	i2=TK_ParseHexByteFromString(str+ 4);
	i3=TK_ParseHexByteFromString(str+ 6);
	vuid[ 0]=i0;	vuid[ 1]=i1;
	vuid[ 2]=i2;	vuid[ 3]=i3;
	if((i0|i1|i2|i3)<0)
		return(0);

	i0=TK_ParseHexByteFromString(str+ 9);
	i1=TK_ParseHexByteFromString(str+11);
	i2=TK_ParseHexByteFromString(str+14);
	i3=TK_ParseHexByteFromString(str+16);
	vuid[ 4]=i0;	vuid[ 5]=i1;
	vuid[ 6]=i2;	vuid[ 7]=i3;
	if((i0|i1|i2|i3)<0)
		return(0);

	i0=TK_ParseHexByteFromString(str+19);
	i1=TK_ParseHexByteFromString(str+21);
	i2=TK_ParseHexByteFromString(str+24);
	i3=TK_ParseHexByteFromString(str+26);
	vuid[ 8]=i0;	vuid[ 9]=i1;
	vuid[10]=i2;	vuid[11]=i3;
	if((i0|i1|i2|i3)<0)
		return(0);

	i0=TK_ParseHexByteFromString(str+28);
	i1=TK_ParseHexByteFromString(str+30);
	i2=TK_ParseHexByteFromString(str+32);
	i3=TK_ParseHexByteFromString(str+34);
	vuid[12]=i0;	vuid[13]=i1;
	vuid[14]=i2;	vuid[15]=i3;
	if((i0|i1|i2|i3)<0)
		return(0);

	return(1);
}

int TK_ParseGuidFromString(char *str, void *ruid)
{
	if((str[0]!='{') || (str[37]!='}'))
		return(0);
	return(TK_ParseUuidFromString(str, ruid));
}

int TK_CheckDWordIsFourcc(u32 val)
{
	u32 mv;
	
	mv=(val|(val+0x01010101U))&0x80808080U;
	if(mv)
		return(0);
	if(!((val+0x60606060U)&0x80808080U))
		return(0);
	return(1);

#if 0	
	if(val<0x20202020U)
		return(0);
	if(val>0x7E7E7E7EU)
		return(0);
	if(val&0x80808080U)
		return(0);
	if((val+0x01010101U)&0x80808080U)
		return(0);
	if(!((val+0x60606060U)&0x80808080U))
		return(0);
	return(1);
#endif
}

int TK_CheckQWordIsFourcc(u64 val)
{
	return((((u32)val)==val) && TK_CheckDWordIsFourcc(val));
}

int TK_CheckQWordIsEightcc(u64 val)
{
	return(TK_CheckDWordIsFourcc(val) && TK_CheckDWordIsFourcc(val>>32));
}

/*
 * 1: UUID/GUID
 * 2: Dual FOURCC
 * 3: Dual EIGHTCC or SIXTEENCC
 */
int TK_UidCheckCategory(void *ruid)
{
	u64 v0, v1;
	v0=((u64 *)ruid)[0];
	v1=((u64 *)ruid)[1];

	if(TK_CheckQWordIsEightcc(v0) && TK_CheckQWordIsEightcc(v1))
	{
		return(3);
	}

	if(TK_CheckQWordIsFourcc(v0) && TK_CheckQWordIsFourcc(v1))
	{
		return(2);
	}
	return(1);
}

int TK_FormatCatIdAsString(char *str, void *ruid)
{
	int cat;
	cat=TK_UidCheckCategory(ruid);
	
	if(cat==1)
	{
		return(TK_FormatGuidAsString(str, ruid));
	}

	if(cat==2)
	{
		str[ 0]='\'';
		str[ 1]=((byte *)ruid)[ 0];
		str[ 2]=((byte *)ruid)[ 1];
		str[ 3]=((byte *)ruid)[ 2];
		str[ 4]=((byte *)ruid)[ 3];
		str[ 5]='\'';
		str[ 6]='/';
		str[ 7]='\'';
		str[ 8]=((byte *)ruid)[ 8];
		str[ 9]=((byte *)ruid)[ 9];
		str[10]=((byte *)ruid)[10];
		str[11]=((byte *)ruid)[11];
		str[12]='\'';
		str[13]=0;
		return(0);
	}

	if(cat==3)
	{
		str[ 0]='\'';
		str[ 1]=((byte *)ruid)[ 0];
		str[ 2]=((byte *)ruid)[ 1];
		str[ 3]=((byte *)ruid)[ 2];
		str[ 4]=((byte *)ruid)[ 3];
		str[ 5]=((byte *)ruid)[ 4];
		str[ 6]=((byte *)ruid)[ 5];
		str[ 7]=((byte *)ruid)[ 6];
		str[ 8]=((byte *)ruid)[ 7];
		str[ 9]=((byte *)ruid)[ 8];
		str[10]=((byte *)ruid)[ 9];
		str[11]=((byte *)ruid)[10];
		str[12]=((byte *)ruid)[11];
		str[13]=((byte *)ruid)[12];
		str[14]=((byte *)ruid)[13];
		str[15]=((byte *)ruid)[14];
		str[16]=((byte *)ruid)[15];
		str[17]='\'';
		str[18]=0;
		return(0);
	}
	
	return(-1);
}
