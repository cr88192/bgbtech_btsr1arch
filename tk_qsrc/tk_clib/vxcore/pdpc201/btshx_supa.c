// #include <stdio.h>
// #include <stddef.h>

// #include <vxcore.h>

TK_FILE *btshx_tk_handles[256];
int btshx_tk_nhandles=3;

void tk_setGpioOutputs(int val)
{
	*(int *)GPIO_BASE=val;
}

#if 0
void tk_putc(int val)
{
	while(P_UART_STAT&8);
//	*(int *)(GPIO_BASE+0x104)=val;
	P_UART_TX=val;
	
	tk_con_putc();
}
#endif

void sleep_0();

#if 0
int tk_kbhit(void)
{
	return(tk_ps2kb_kbhit());
//	return(P_UART_STAT&1);
}

int tk_getch(void)
{
	return(tk_ps2getch());
//	while(!(P_UART_STAT&1))
//		sleep_0();
//	return(P_UART_RX);
}
#endif

int tk_ptrIsRam(void *ptr)
{
	u32 a;
	a=(u32)ptr;
	a&=0x1FFFFFFF;
	if(a<0x0C000000)
		return(0);
	if(a>=0x18000000)
		return(0);
	return(1);
}

#if 0
int tk_puts(char *msg)
{
	char *s;
	
	s=msg;
	while(*s)
		{ tk_putc(*s++); }
}

void tk_puts_n(char *msg, int n)
{
	char *s;
	
	s=msg;
	while(n--)
		{ tk_putc(*s++); }
}

void tk_gets(char *buf)
{
	char *t;
	int i;
	
	t=buf;
	while(1)
	{
		i=tk_getch();
		if((i=='\b') || (i==127))
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
			{ tk_putc('\n'); break; }
		if(i=='\n')
			{ tk_putc('\n'); break; }
		tk_putc(i);
		*t++=i;
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
#endif

u32 __moddi3(u32 a, u32 b)
{
	u32 q, r;
	
	q=a/b;
	r=a-(q*b);
	return(r);
}

u32 __umodsi3(u32 a, u32 b)
{
	u32 q, r;
	
	q=a/b;
	r=a-(q*b);

//	if(((s32)r)<0)
//		{ *(int *)-1=-1; }
//	if(r>=b)
//		{ *(int *)-1=-1; }

	return(r);
}

s32 __smodsi3(s32 a, s32 b)
{
	s32 q, r;
	
	q=a/b;
	r=a-(q*b);
	return(r);
}

#if 0
typedef struct u64_obj_s u64_obj_t;
struct u64_obj_s {
u32 lo;
u32 hi;
};

u64_obj_t __shllli(u64_obj_t ival, int shl)
{
	u64_obj_t oval;
	
	if(!(shl&31))
	{
		if(!shl)
			return(ival);
		if(shl==32)
		{
			oval.lo=0;
			oval.hi=ival.lo;
			return(oval);
		}

		oval.lo=0;
		oval.hi=0;
		return(oval);
	}
	
	oval.lo=ival.lo<<shl;
	oval.hi=(ival.hi<<shl)|(ival.lo<<(32-shl));
	return(oval);
}

u64_obj_t __shrlli(u64_obj_t ival, int shl)
{
	u64_obj_t oval;
	
	if(!(shl&31))
	{
		if(!shl)
			return(ival);
		if(shl==32)
		{
			oval.lo=ival.hi;
			oval.hi=0;
			return(oval);
		}

		oval.lo=0;
		oval.hi=0;
		return(oval);
	}

	oval.hi=ival.hi>>shl;
	oval.lo=(ival.lo>>shl)|(ival.hi<<(32-shl));
	return(oval);
}

u64_obj_t __sarlli(u64_obj_t ival, int shl)
{
	u64_obj_t oval;
	u32 m;

	m=0;
	if(ival.hi&0x80000000)
		m=(u32)(-1);

	if(!(shl&31))
	{
		if(!shl)
			return(ival);
		if(shl==32)
			{	oval.lo=ival.hi;	oval.hi=m;	return(oval);	}
		if(m>0)
			{	oval.lo=m;	oval.hi=m;	return(oval);	}
		oval.lo=0;	oval.hi=0;
		return(oval);
	}

	oval.hi=(ival.hi>>shl)|(m<<(32-shl));
	oval.lo=(ival.lo>>shl)|(ival.hi<<(32-shl));
	return(oval);
}

u64_obj_t __udivlli(u64_obj_t a, u64_obj_t b)
{
	u64 lc, ld;
	u64_obj_t c, d, e;

	if(!b.hi)
	{
		if(!a.hi)
		{
			c.lo=a.lo/b.lo;
			c.hi=0;
			return(c);
		}

		c.hi=a.hi/b.lo;
		c.lo=a.lo/b.lo;
		d.hi=a.hi%b.lo;
		d.lo=a.lo%b.lo;
		
		lc=((u64)d.hi)*((u64)b.lo);
		*(u64 *)(&c)+=lc;
		return(c);
	}

	if(!a.hi)
		{ c.lo=0; c.hi=0; return(c); }
	if(b.hi>a.hi)
		{ c.lo=0; c.hi=0; return(c); }

	c.lo=a.hi/b.hi;
	c.hi=0;
	return(c);

//	c.lo=a.lo/b.hi;
//	d.hi=a.hi/b.lo;
//	d.lo=a.lo/b.lo;
}

//u64_obj_t __sdivlli(u64_obj_t a, u64_obj_t b)
//{
//}

s64 __sdivlli(s64 a, s64 b)
{
	u64 ua, ub, uc;
	int sg;
	
	sg=0;
	
	ua=(u64)a;
	if(ua&0x8000000000000000ULL)
		{ ua=-ua; sg=1; }
	ub=(u64)a;
	if(ub&0x8000000000000000ULL)
		{ ub=-ub; sg^=1; }
	uc=ua/ub;
	if(sg)
		{ uc=-uc; }
	return((s64)uc);
}
#endif

#if 1
u64 __udivdi3(u64 n, u64 d);
s64 __sdivdi3(s64 a, s64 b);


u64 __udivsq(u64 a, u64 b)
{

	return(__udivdi3(a, b));

#if 0
	u64_obj_t c, d, e;
	u64 v, t;

#if 1
	if((((u32)a)==a) && (((u32)b)==b))
	{
		return(((u32)a)/((u32)b));
	}
#endif

	if((b&(b-1))==0)
	{
		v=a;	t=b;
		while(t>1)
			{ v=v>>1; t=t>>1; }
		return(v);
	}
	
	*(u64 *)(&c)=a;
	*(u64 *)(&d)=b;
	e=__udivlli(c, d);
//	*(u64_obj_t *)(&v)=__udivlli(c, d);
	v=*(u64 *)(&e);
	return(v);
#endif
}

s64 __sdivsq(s64 a, s64 b)
{
	return(__sdivdi3(a, b));

#if 0
#if 1
	if((((s32)a)==a) && (((s32)b)==b))
	{
		return(((s32)a)/((s32)b));
	}
#endif
	
	return(__sdivlli(a, b));
#endif
}

u64 __umodsq(u64 a, u64 b)
{
	u64 c, d;

#if 1
	if((((u32)a)==a) && (((u32)b)==b))
	{
		if(((u32)(b&(b-1)))==0)
			return(((u32)a)&((u32)(b-1)));
	
		return(((u32)a)%((u32)b));
	}
#endif

	if((b&(b-1))==0)
	{
//		__debugbreak();
		return(a&((u32)(b-1)));
	}

	c=a/b;
	d=a-(c*b);

	if(((s64)d)<0)
		d+=b;

	if(d>=b)
	{
		d-=b;
		if(d>=b)
		{
			tk_printf("a/b = %X_%X / %X_%X\n",
				(u32)(a>>32), (u32)a, (u32)(b>>32), (u32)b);
			tk_printf("c,d = %X_%X , %X_%X\n",
				(u32)(c>>32), (u32)c, (u32)(d>>32), (u32)d);
		
			__debugbreak();
			return(0);
		}
//		while(d>=b)
//			d-=b;
	}
	return(d);
}

s64 __smodsq(s64 a, s64 b)
{
	s64 c, d;

	c=__sdivsq(a, b);
	d=c*b;
	return(a-d);

#if 0
	if(b<0)
	{
//		c=__sdivsq(a, -b)
//		return(-__smodsq(a, -b));
	}

	if(a<0)
		{ return(-__umodsq(-a, b)); }
	else
		{ return(__umodsq(a, b)); }
#endif

}
#endif

#if 0
float __ldhf16(unsigned short iv)
{
	u32 v;
	if(!(iv&0x7FFF))
		return(0.0);
	v=((iv&0x7FFF)<<13)+(112<<23)+((iv&0x8000)<<16);
	return(*(float *)(&v));
}

int __sthf16(float ivf)
{
	u32 v0, v1;
	u16 v2;
	
	v0=*(u32 *)(&ivf);
	v1=(((v0+4095)&0x7FFFFFFF)>>13)-(112<<10);
//	if(v1>>15)
	if((v1>>10)>30)
	{
		if(v1>>24)
			return(0x0000);
		v1=0x7C00;
	}
	v2=v1|((v0>>16)&0x8000);
	return(v2);
}
#endif


int __read(int handle, void *buf, size_t len, int *errind)
{
	int i;

	if(handle<3)
	{
//		tk_gets_n(buf, len);
		return(0);
	}

	if(errind)*errind=0;
	i=tk_fread(buf, 1, len, btshx_tk_handles[handle]);
	if(i<0)
		*errind=1;
//	read(handle, buf, len);
	return(i);
}

int __write(int handle, const void *buf, size_t len, int *errind)
{
	void *ptr1;
	int i;

	if(handle<3)
	{
		ptr1=(void *)buf;
		tk_puts_n(ptr1, len);
		return(0);
	}

	if(errind)*errind=0;
	ptr1=(void *)buf;
	i=tk_fwrite(ptr1, 1, len, btshx_tk_handles[handle]);
	if(i<0)
		*errind=1;
//	write(handle, buf, len);
	return(i);
}

void __seek(int handle, long offset, int whence)
{
	tk_fseek(btshx_tk_handles[handle], offset, whence);
//	lseek(handle, offset, whence);
}

long __tell(int handle)
{
	return(tk_ftell(btshx_tk_handles[handle]));
}

int __open(const char *a, int b, int *rc)
{
	char tfn[512];
	TK_FILE *fd;
	char *s, *md;
	int i;

	tk_vfile_init();

	md="rb";
	if(b==1)
		md="wb";
	if(b==2)
		md="r+b";

	s=(char *)a;
	TK_Env_GetCwdQualifyName(tfn, 512, s);
	TKSH_NormalizePath(tfn, tfn);
	
//	tk_printf("__open: %s\n", tfn);

//	fd=tk_fopen(s, "rb");
//	fd=tk_fopen(s, md);
	fd=tk_fopen(tfn, md);
	if(!fd)
	{
//		tk_printf("__open: %s, tk_fopen returned NULL\n", tfn);
//		*(int *)((long)((unsigned int)c))=-1;
		*rc=-1;
		return(-1);
	}

//	tk_printf("__open: %s  Z\n", tfn);

//	*(int *)((u32)c)=0;
//	*(int *)((long)((unsigned int)c))=0;
	*rc=0;
	
	for(i=3; i<btshx_tk_nhandles; i++)
	{
		if(!btshx_tk_handles[i])
		{
//			tk_printf("__open: %s  A\n", tfn);
			*rc=0;
			btshx_tk_handles[i]=fd;
			return(i);
		}
	}
	
	if(btshx_tk_nhandles>=256)
	{
		tk_printf("__open: %s, no free handles\n", tfn);
//		*(int *)((u32)c)=-1;
//		*(int *)((long)((unsigned int)c))=-1;
		*rc=-1;
		return(-1);
	}

//	tk_printf("__open: %s  B\n", tfn);

	*rc=0;
	i=btshx_tk_nhandles++;
	btshx_tk_handles[i]=fd;
	return(i);
	
//	return(open(a, b, c));
}

void __close(int handle)
{
	TK_FILE *fd;

	if(handle<3)
		return;

	fd=btshx_tk_handles[handle];
	btshx_tk_handles[handle]=NULL;
	tk_fclose(fd);
//	close(handle);
}

void __ioctl(int handle, int req, void *ptr)
{
	TK_FILE *fd;
	fd=btshx_tk_handles[handle];
	tk_fioctl(fd, req, ptr);
}

int __unlink(const char *a, int b)
{
	char tfn[512];
	TK_FILE *fd;
	char *s, *md;
	int i;

	s=(char *)a;
	TK_Env_GetCwdQualifyName(tfn, 512, s);
	TKSH_NormalizePath(tfn, tfn);
	
	tk_printf("__unlink: %s\n", tfn);

	tk_unlink(tfn);
	return(0);
}

int __fsctl(const char *a, int b, void *c)
{
	char tfn[512];
	TK_FILE *fd;
	char *s, *md;
	int i;

	s=(char *)a;
	TK_Env_GetCwdQualifyName(tfn, 512, s);
	TKSH_NormalizePath(tfn, tfn);
	
	tk_printf("__fsctl: %s\n", tfn);

	tk_fsctl(tfn, b, c);
	return(0);
}

void __remove(const char *filename)
{
	__unlink(filename, 0);
}

int __rename2(const char *oldfn, const char *newfn, const char *mode)
{
	char tfno[512];
	char tfnn[512];
	TK_FILE *fd;
	char *md;
	int i;

	if(*mode=='S')
	{
		strcpy(tfno, oldfn);
	}else
	{
		TK_Env_GetCwdQualifyName(tfno, 512, (char *)oldfn);
		TKSH_NormalizePath(tfno, tfno);
	}

	TK_Env_GetCwdQualifyName(tfnn, 512, (char *)newfn);
	TKSH_NormalizePath(tfnn, tfnn);
	
	tk_printf("__rename2: %s %s %s\n", tfno, tfnn, mode);

	tk_rename(tfno, tfnn, mode);
	return(0);
}

void __rename(const char *oldfn, const char *newfn)
{
	__rename2(oldfn, newfn, "r");
}

#if 1
int unlink(const char *path)
{
	__unlink(path, 0);
	return(0);
}

int link(const char *oldpath, const char *newpath)
{
	__rename2(oldpath, newpath, "l");
	return(0);
}

int symlink(const char *oldpath, const char *newpath)
{
	__rename2(oldpath, newpath, "S");
	return(0);
}
#endif

#if 1

int open(const char *name, int flags, int mode)
{
	int h, c, md;

	md=0;

	switch((flags>>12)&0xA)
	{
	case 0x0:		md=0; break;
	case 0x2:		md=0; break;
	case 0x8:		md=1; break;
	case 0xA:		md=2; break;
	}
	
	h=__open(name, md, &c);
	return(h);
}

int creat(const char *path, int mode)
{
	return(open(path, 0x8042, mode));
}

int read(int handle, void *buf, size_t len)
{
	int c;
	return(__read(handle, buf, len, &c));
}

int write(int handle, const void *buf, size_t len)
{
	int c;
	return(__write(handle, buf, len, &c));
}

long lseek(int handle, long offset, int whence)
{
	__seek(handle, offset, whence);
	return(__tell(handle));
}

int close(int handle)
{
	__close(handle);
	return(0);
}

int ioctl(int handle, int req, void *ptr)
{
	__ioctl(handle, req, ptr);
	return(0);
}
#endif

void __exita(int status)
{
	if(!tk_iskernel())
		TK_ExitV(status);
	__debugbreak();
//	*(int *)-1=-1;
//	vx_exit(status);
}

u32 TK_GetTimeMs(void)
{
#if 1
	u64 *sreg;
	u64 us;
	int ms;

	sreg=(u64 *)0xF000E000UL;
	us=sreg[0];
//	us=(us*131)>>7;		//correct for (us>>10) vs us/1000.
	us=(us*2097)>>11;	//correct for (us>>10) vs us/1000.
	ms=us>>10;
	return(ms);
#endif

#if 0
	u32 *sreg;
	u32 us_lo, us_hi;
	u64 us;
	int ms;

//	sreg=(int *)0xA000E000;
	sreg=(int *)0xF000E000UL;
	us_lo=sreg[0];
	us_hi=sreg[1];
	us=(((u64)us_hi)<<32)|us_lo;
	
	us=(us*131)>>7;		//correct for (us>>10) vs us/1000.
	ms=us>>10;

//	sreg=(int *)0x007F8000;
//	ms=(P_AIC_RTC_NSEC>>20)|(P_AIC_RTC_SEC_LO*1000);
//	ms=sreg[4];
	return(ms);
#endif
}

s64 TK_GetTimeUs(void)
{
#if 1
	u64 *sreg;
	u64 us;
	int ms;

	sreg=(u64 *)0xF000E000UL;
	us=sreg[0];
	return(us);
#endif

#if 0
	u32 *sreg;
	u32 us_lo, us_hi;
	u64 us;
	int ms;

//	sreg=(int *)0xA000E000;
	sreg=(int *)0xF000E000;
	us_lo=sreg[0];
	us_hi=sreg[1];
	us=(((u64)us_hi)<<32)|us_lo;
//	ms=us>>10;

//	sreg=(int *)0x007F8000;
//	ms=(P_AIC_RTC_NSEC>>20)|(P_AIC_RTC_SEC_LO*1000);
//	ms=sreg[4];
//	return(ms);
	return(us);
#endif
}

s64 TK_GetTimeCycles(void)
{
	u32 *sreg;
	u32 us_lo, us_hi;
	u64 us;
	int ms;

//	sreg=(int *)0xA000E340;
	sreg=(int *)0xF000E340;
	us_lo=sreg[0];
	us_hi=sreg[1];
	us=(((u64)us_hi)<<32)|us_lo;
	return(us);
}

int TK_GetApproxMHz(void)
{
	static int mhz=0;
	s64 li, lj;
	int i, j, k;
	
	if((mhz>1) && (mhz<4096))
		return(mhz);
	
	li=TK_GetTimeCycles();
	lj=TK_GetTimeUs();
	i=(li>>10);
	j=(lj>>10);
	k=li/lj;
	mhz=k;
	return(k);
}
void __datetime(void *ptr)
{
	struct dt_s {
		int year;
		int month;
		int day;
		int hours;
		int minutes;
		int seconds;
		int hundredths;
	} dt, *pdt;

	int ms;
	
	ms=TK_GetTimeMs();
	
	pdt=ptr;
	pdt->hundredths=(ms/10)%100;
	pdt->seconds=(ms/1000)%60;
	pdt->minutes=(ms/60000)%60;
	pdt->hours=0;
	pdt->day=0;
	pdt->month=0;
	pdt->year=0;

//	vx_datetime(ptr);
}

__PDPCLIB_API__ void *_alloca(int sz)
{
//	return((void *)vx_malloc(sz));
	return(TKMM_Malloc(sz));
}

void __allocmem(size_t size, void **rptr)
{
	void *ptr;
//	*ptr=(void *)vx_malloc(size);
	ptr=TKMM_Malloc(size);
//	tk_printf("__allocmem: %p..%p %d\n", ptr, ptr+size, size);
	*rptr=ptr;
//	*ptr=TKMM_MMList_AllocBrk(size);
}

void __freemem(void *ptr)
{
//	vx_free(ptr);
//	TKMM_Free(ptr);
}

unsigned char *__envptr;

void __exec(char *cmd, void *env)
{
//	vx_system(cmd);
}

int __start_init();


int __start_first()
{
#ifndef __TK_CLIB_ONLY__
	tk_con_init();
#endif
}

void *(*_malloc_fptr)(size_t size, int cat);
void (*_free_fptr)(void *ptr);
void *(*_realloc_fptr)(void *ptr, size_t size);
size_t (*_msize_fptr)(void *ptr);

void *tk_malloc(int sz);
void *tk_malloc_cat(int sz, int cat);
int tk_free(void *ptr);
void *tk_realloc(void *ptr, int sz);
int tk_msize(void *ptr);


int __start_early()
{
	TKMM_Init();
	_malloc_fptr=tk_malloc_cat;
	_free_fptr=tk_free;
	_realloc_fptr=tk_realloc;
	_msize_fptr=tk_msize;
	return(0);
}

int __start_late()
{
//	tk_vfile_init();
	__start_init();
	return(0);
}

char *__get_cmdline()
{
//	static char tbuf[1024];
	TKPE_TaskInfo *task;
	char **argv;
	char *ct;
	int i, j, k;
	
	task=TK_GetCurrentTask();
	if(!task)
		return("");

	ct=(char *)(task->argv);
	return(ct);

#if 0
	argv=task->argv;
	ct=tbuf;
	for(i=0; argv[i]; i++)
	{
		if(i)*ct++=' ';
		strcpy(ct, argv[i]);
		ct+=strlen(ct);
	}
	return(tbuf);
#endif
}

char **__get_cmdenv()
{
	return(NULL);
}

//int __longj(void *ptr)
//	{ }

// int __setj(jmp_buf env)
// { }


#if 0
void tk_print_hex(u32 v)
{
	static char *chrs="0123456789ABCDEF";

	tk_putc(chrs[(v>>28)&15]);
	tk_putc(chrs[(v>>24)&15]);
	tk_putc(chrs[(v>>20)&15]);
	tk_putc(chrs[(v>>16)&15]);
	tk_putc(chrs[(v>>12)&15]);
	tk_putc(chrs[(v>> 8)&15]);
	tk_putc(chrs[(v>> 4)&15]);
	tk_putc(chrs[(v    )&15]);
}

void tk_print_hex_n(u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";

	if(n>7)tk_putc(chrs[(v>>28)&15]);
	if(n>6)tk_putc(chrs[(v>>24)&15]);
	if(n>5)tk_putc(chrs[(v>>20)&15]);
	if(n>4)tk_putc(chrs[(v>>16)&15]);
	if(n>3)tk_putc(chrs[(v>>12)&15]);
	if(n>2)tk_putc(chrs[(v>> 8)&15]);
	if(n>1)tk_putc(chrs[(v>> 4)&15]);
	if(n>0)tk_putc(chrs[(v    )&15]);
}

int tk_print_hex_genw(u32 v)
{
	u32 w;
	int i;

	i=1;
	while(v>=16)
		{ v=v>>4; i++; }

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
	
	if(val==0)
	{
		tk_putc('0');
		return;
	}
	
//	if(val==(-2147483648))
	if(val==(1<<31))
	{
		tk_puts("-2147483648");
		return;
	}
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb; te=tb+18;
	if(!k)*t++='0';	
	while((k>0) && (t<te))
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
	}
	if(s)*t++='-';
	
	if(tb[0]=='-')
	{
//		*(int *)-1=-1;
		__debugbreak();
	}
	
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
	
	sg=0;
	if(val<0)
		{ val=-val; sg=1; }
	
	ip=(int)val;
	fp=(int)((val-ip)*1000000);

//	__debugbreak();

//	if(ip==(-2147483648))
	if(ip==(1<<31))
	{
//		*(int *)-1=-1;
		tk_puts("#OVF");
		return;
	}

//	*(int *)-1=-1;

	if(sg)tk_putc('-');
	tk_print_decimal(ip);
	tk_putc('.');
	tk_print_decimal_n(fp, 6);
}

void tk_print_float_ss(float val)
{
	int ip, fp, sg;
	
	sg=0;
	if(val<0.0f)
		{ val=-val; sg=1; }
	
	ip=(int)val;
	fp=(int)((val-ip)*1000000);

//	*(int *)-1=-1;

	if(sg)tk_putc('-');
	tk_print_decimal(ip);
	tk_putc('.');
	tk_print_decimal_n(fp, 6);
}
#endif

void tk_printf(char *str, ...)
{
	va_list lst;
	double f;
	char pcfill;
	char *s, *s1;
	int v, w, wf;

	va_start(lst, str);
	
	s=str;
	while(*s)
	{
		if(*s!='%')
			{ tk_putc(*s++); continue; }

		if(s[1]=='%')
			{ s+=2; tk_putc('%'); continue; }
		s++;

#if 1
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
		
		wf=0;
		if(*s=='.')
		{
			s++;
			if((*s>='0') && (*s<='9'))
			{
				while((*s>='0') && (*s<='9'))
					wf=(wf*10)+((*s++)-'0');
			}
		}
#endif

#if 1
		switch(*s++)
		{
		case 'c':
			v=va_arg(lst, int);
			tk_putc(v);
			break;

		case 'd':
//			v=999;
			v=va_arg(lst, int);

//			*(int *)-1=-1;

			if(w)
				{ tk_print_decimal_n(v, w); }
			else
				{ tk_print_decimal(v); }
			break;
		case 'X':
			v=va_arg(lst, int);

			if(!w)w=tk_print_hex_genw(v);
			tk_print_hex_n(v, w);
			break;
		case 's':
			s1=va_arg(lst, char *);
			tk_puts(s1);
			break;

		case 'p':
			s1=va_arg(lst, char *);
			tk_print_hexptr((long)s1);
			break;

#ifdef ARCH_HAS_FPU
		case 'f':
			f=6969.6969;
			f=va_arg(lst, double);
//			__debugbreak();
			tk_print_float(f);
			break;
#endif

		default:
			break;
		}
#endif
	}
	va_end(lst);
}
#endif


void tk_vprintf(char *str, va_list lst)
{
	double f;
	char pcfill;
	char *s, *s1;
	int v, w, wf;

	s=str;
	while(*s)
	{
		if(*s!='%')
			{ tk_putc(*s++); continue; }

		if(s[1]=='%')
			{ s+=2; tk_putc('%'); continue; }
		s++;

#if 1
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
		
		wf=0;
		if(*s=='.')
		{
			s++;
			if((*s>='0') && (*s<='9'))
			{
				while((*s>='0') && (*s<='9'))
					wf=(wf*10)+((*s++)-'0');
			}
		}
#endif

#if 1
		switch(*s++)
		{
		case 'c':
			v=va_arg(lst, int);
			tk_putc(v);
			break;

		case 'd':
//			v=999;
			v=va_arg(lst, int);

//			*(int *)-1=-1;

			if(w)
				{ tk_print_decimal_n(v, w); }
			else
				{ tk_print_decimal(v); }
			break;
		case 'X':
			v=va_arg(lst, int);

			if(!w)w=tk_print_hex_genw(v);
			tk_print_hex_n(v, w);
			break;
		case 's':
			s1=va_arg(lst, char *);
			tk_puts(s1);
			break;

		case 'p':
			s1=va_arg(lst, char *);
			tk_print_hex((u32)s1);
			break;

#ifdef ARCH_HAS_FPU
		case 'f':
			f=6969.6969;
			f=va_arg(lst, double);
			tk_print_float(f);
			break;
#endif

		default:
			break;
		}
#endif
	}
}




char *tk_sprint_hex(char *ct, u32 v)
{
	static char *chrs="0123456789ABCDEF";

	*ct++=(chrs[(v>>28)&15]);
	*ct++=(chrs[(v>>24)&15]);
	*ct++=(chrs[(v>>20)&15]);
	*ct++=(chrs[(v>>16)&15]);
	*ct++=(chrs[(v>>12)&15]);
	*ct++=(chrs[(v>> 8)&15]);
	*ct++=(chrs[(v>> 4)&15]);
	*ct++=(chrs[(v    )&15]);
	return(ct);
}

char *tk_sprint_hex_n(char *ct, u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";

	if(n>7)*ct++=(chrs[(v>>28)&15]);
	if(n>6)*ct++=(chrs[(v>>24)&15]);
	if(n>5)*ct++=(chrs[(v>>20)&15]);
	if(n>4)*ct++=(chrs[(v>>16)&15]);
	if(n>3)*ct++=(chrs[(v>>12)&15]);
	if(n>2)*ct++=(chrs[(v>> 8)&15]);
	if(n>1)*ct++=(chrs[(v>> 4)&15]);
	if(n>0)*ct++=(chrs[(v    )&15]);
	return(ct);
}

char *tk_sprint_decimal(char *ct, int val)
{
	char tb[256];
	char *t;
	int i, k, s;
	
	if(val==0)
	{
		*ct++='0';
		return(ct);
	}
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb;
//	if(k==0)
//		*t++='0';	
	while(k>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
	}
	if(s)*t++='-';
	
	while(t>tb)
		{ t--; *ct++=*t; }
	return(ct);
}

char *tk_sprint_decimal_n(char *ct, int val, int num)
{
	char tb[256];
	char *t;
	int i, k, n, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb; n=num;
	while(n>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
		n--;
	}

//	if(s)*t++='-';
	
	while(t>tb)
		{ t--; *ct++=*t; }
	return(ct);
}

void tk_vsprintf(char *dst, char *str, va_list lst)
{
	double f;
	char pcfill;
	char *s, *s1;
	char *ct;
	int v, w, wf;

	ct=dst;
	s=str;
	while(*s)
	{
		if(*s!='%')
			{ *ct++=*s++; continue; }

		if(s[1]=='%')
			{ s+=2; *ct++='%'; continue; }
		s++;

#if 1
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
		
		wf=0;
		if(*s=='.')
		{
			s++;
			if((*s>='0') && (*s<='9'))
			{
				while((*s>='0') && (*s<='9'))
					wf=(wf*10)+((*s++)-'0');
			}
		}
#endif

#if 1
		switch(*s++)
		{
		case 'c':
			v=va_arg(lst, int);
			*ct++=v;
			break;

		case 'd':
		case 'i':
			v=va_arg(lst, int);
			if(w)
				{ ct=tk_sprint_decimal_n(ct, v, w); }
			else
				{ ct=tk_sprint_decimal(ct, v); }
			break;
		case 'X':
			v=va_arg(lst, int);

			if(!w)w=tk_print_hex_genw(v);
			ct=tk_sprint_hex_n(ct, v, w);
			break;
		case 's':
			s1=va_arg(lst, char *);
			while(*s1)
				{ *ct++=*s1++; }
			break;

		case 'p':
			s1=va_arg(lst, char *);
			ct=tk_sprint_hex(ct, (u32)s1);
			break;

// #ifdef ARCH_HAS_FPU
#if 0
		case 'f':
			f=6969.6969;
			f=va_arg(lst, double);
			ct=tk_sprint_float(ct, f);
			break;
#endif

		default:
			break;
		}
#endif
	}
	
	*ct++=0;
}

void tk_sprintf(char *dst, char *str, ...)
{
	va_list lst;

	va_start(lst, str);	
	tk_vsprintf(dst, str, lst);
	va_end(lst);
}
