#define ARCH_SH4

#ifdef ARCH_SH4

#ifndef ARCH_HAS_FPU
#define ARCH_HAS_FPU
#endif

#ifndef ARCH_HAS_MMU
#define ARCH_HAS_MMU
#endif

#endif

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef volatile u32 vol_u32;

#ifdef ARCH_HAS_FPU
typedef float f32;
typedef double f64;
#endif

#define GPIO_BASE 0xABCD0000
#define SPI_BASE 0xABCD0040
#define UART_BASE 0xABCD0100
#define AIC_BASE 0xABCD0200

#define UART_RX		(UART_BASE+0x00)
#define UART_TX		(UART_BASE+0x04)
#define UART_STAT	(UART_BASE+0x08)
#define UART_CTRL	(UART_BASE+0x0C)

#define SPI_CTRL	(SPI_BASE+0x00)
#define SPI_DATA	(SPI_BASE+0x04)

#define AIC_CTRL0			(AIC_BASE+0x00)
#define AIC_BRKADD			(AIC_BASE+0x04)
#define AIC_ILEVELS			(AIC_BASE+0x08)
#define AIC_CTRL1			(AIC_BASE+0x0C)
#define AIC_PIT_THROTTLE	(AIC_BASE+0x10)
#define AIC_PIT_COUNTER		(AIC_BASE+0x14)
#define AIC_CLK_PERIOD		(AIC_BASE+0x18)
#define AIC_IGNORE0			(AIC_BASE+0x1C)
#define AIC_RTC_SEC_HI		(AIC_BASE+0x20)
#define AIC_RTC_SEC_LO		(AIC_BASE+0x24)
#define AIC_RTC_NSEC		(AIC_BASE+0x28)

#define P_UART_RX	(*(vol_u32 *)UART_RX)
#define P_UART_TX	(*(vol_u32 *)UART_TX)
#define P_UART_STAT	(*(vol_u32 *)UART_STAT)
#define P_UART_CTRL	(*(vol_u32 *)UART_CTRL)

#define P_SPI_CTRL	(*(vol_u32 *)SPI_CTRL)
#define P_SPI_DATA	(*(vol_u32 *)SPI_DATA)

#define P_AIC_RTC_SEC_HI	(*(vol_u32 *)AIC_RTC_SEC_HI)
#define P_AIC_RTC_SEC_LO	(*(vol_u32 *)AIC_RTC_SEC_LO)
#define P_AIC_RTC_NSEC		(*(vol_u32 *)AIC_RTC_NSEC)

#define MMREG_BASE 0xFF000000
#define MMREG_PTEH	(MMREG_BASE+0x00)
#define MMREG_PTEL	(MMREG_BASE+0x04)
#define MMREG_TTB	(MMREG_BASE+0x08)
#define MMREG_TEA	(MMREG_BASE+0x0C)
#define MMREG_MMUCR	(MMREG_BASE+0x10)

#define P_MMREG_PTEH	(*(vol_u32 *)MMREG_PTEH)
#define P_MMREG_PTEL	(*(vol_u32 *)MMREG_PTEL)
#define P_MMREG_TTB		(*(vol_u32 *)MMREG_TTB)
#define P_MMREG_TEA		(*(vol_u32 *)MMREG_TEA)
#define P_MMREG_MMUCR	(*(vol_u32 *)MMREG_MMUCR)

#ifdef ARCH_SH4
#define TKMM_PAGEBASE	0x90000000
#define TKMM_PAGEEND	0x98000000
#else
#define TKMM_PAGEBASE	0x11000000
#define TKMM_PAGEEND	0x18000000
#endif

#define INITRD_ADDR	0x1003F010
#define INITRD_SIZE	0x1003F014
#define KINIT_ADDR	0x1003F100

#define P_INITRD_ADDR	(*(vol_u32 *)INITRD_ADDR)
#define P_INITRD_SIZE	(*(vol_u32 *)INITRD_SIZE)
#define P_KINIT_ADDR	((char *)KINIT_ADDR)

#define P_INITRD_ABSADDR	((void *)(0x10000000+P_INITRD_ADDR))

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef __builtin_va_list va_list;
#endif

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)

#endif

#ifndef __cplusplus

#ifndef _BOOL_T
#define _BOOL_T
typedef unsigned char bool;
#endif

#ifndef true
#define true 1
#define false 0
#endif

#endif

typedef unsigned int size_t;

void putc(int val)
{
	while(P_UART_STAT&8);
	P_UART_TX=val;
}

void sleep_0();

int kbhit(void)
	{ return(P_UART_STAT&1); }

int getch(void)
{
	while(!(P_UART_STAT&1))
		sleep_0();
	return(P_UART_RX);
}

void puts(char *msg)
{
	char *s;
	
	s=msg;
	while(*s)
		{ putc(*s++); }
}

void gets(char *buf)
{
	char *t;
	int i;
	
	t=buf;
	while(1)
	{
		i=getch();
		if((i=='\b') || (i==127))
		{
			if(t>buf)
			{
				puts("\b \b");
				t--;
			}
			*t=0;
			continue;
		}
		if(i=='\r')
			{ putc('\n'); break; }
		if(i=='\n')
			{ putc('\n'); break; }
		putc(i);
		*t++=i;
	}
	*t=0;
}

void print_hex(u32 v)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	putc(chrs[(v>>28)&15]);
	putc(chrs[(v>>24)&15]);
	putc(chrs[(v>>20)&15]);
	putc(chrs[(v>>16)&15]);
	putc(chrs[(v>>12)&15]);
	putc(chrs[(v>> 8)&15]);
	putc(chrs[(v>> 4)&15]);
	putc(chrs[(v    )&15]);
}

void print_hex_n(u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	if(n>7)putc(chrs[(v>>28)&15]);
	if(n>6)putc(chrs[(v>>24)&15]);
	if(n>5)putc(chrs[(v>>20)&15]);
	if(n>4)putc(chrs[(v>>16)&15]);
	if(n>3)putc(chrs[(v>>12)&15]);
	if(n>2)putc(chrs[(v>> 8)&15]);
	if(n>1)putc(chrs[(v>> 4)&15]);
	if(n>0)putc(chrs[(v    )&15]);
}

int print_hex_genw(u32 v)
{
	u32 w;
	int i;

	i=1;
	while(v>=16)
//	while(v>>4)
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

void print_decimal(int val)
{
	char tb[256];
	char *t;
	int i, k, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb;
	if(!k)*t++='0';	
	while(k>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
	}
	if(s)*t++='-';
	
	while(t>tb)
		{ t--; putc(*t); }
}

void print_decimal_n(int val, int num)
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
		{ t--; putc(*t); }
}

#ifdef ARCH_HAS_FPU
void print_float(double val)
{
	int ip, fp, sg;
	
	sg=0;
	if(val<0)
		{ val=-val; sg=1; }
	
	ip=(int)val;
	fp=(int)((val-ip)*1000000);

//	*(int *)-1=-1;

	if(sg)putc('-');
	print_decimal(ip);
	putc('.');
	print_decimal_n(fp, 6);
}

void print_float_ss(float val)
{
	int ip, fp, sg;
	
	sg=0;
	if(val<0.0f)
		{ val=-val; sg=1; }
	
	ip=(int)val;
	fp=(int)((val-ip)*1000000);

//	*(int *)-1=-1;

	if(sg)putc('-');
	print_decimal(ip);
	putc('.');
	print_decimal_n(fp, 6);
}
#endif

void printf(char *str, ...)
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
			{ putc(*s++); continue; }

//		putc(*s++); continue;

		if(s[1]=='%')
			{ s+=2; putc('%'); continue; }
		s++;

//		putc(*s++); continue;

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

//		putc(*s++); continue;
#endif

#if 1
		switch(*s++)
		{
		case 'c':
			v=va_arg(lst, int);
			putc(v);
			break;

		case 'd':
//			v=999;
			v=va_arg(lst, int);

//			*(int *)-1=-1;

			if(w)
			{
				print_decimal_n(v, w);
			}else
			{
				print_decimal(v);
			}
			break;
		case 'X':
			v=va_arg(lst, int);

			if(!w)w=print_hex_genw(v);
			print_hex_n(v, w);
			break;
		case 's':
			s1=va_arg(lst, char *);
			puts(s1);
			break;

		case 'p':
			s1=va_arg(lst, char *);
			print_hex((u32)s1);
			break;

#ifdef ARCH_HAS_FPU
		case 'f':
			f=6969.6969;
			f=va_arg(lst, double);
//			*(int *)-1=-1;
			print_float(f);
			break;
#endif

		default:
			break;
		}
#endif
	}
	va_end(lst);
}

// byte tk_malloc_buf[1<<18];
byte *tk_malloc_rov=NULL;
// byte *tk_malloc_rov=TKMM_PAGEBASE;

void *malloc(int sz)
{
	byte *p;

	if(!tk_malloc_rov)
	{
//		tk_malloc_rov=tk_malloc_buf;
		tk_malloc_rov=(byte *)TKMM_PAGEBASE;
	}
	
	sz=(sz+3)&(~3);
	p=tk_malloc_rov;
	tk_malloc_rov=p+sz;
	return(p);
	
//	return(TKMM_Malloc(sz));
}

int free(void *ptr)
{
//	return(TKMM_Free(ptr));
}

#if 1
int strcpy(char *dst, char *src)
{
	char *ct, *cs;
	
	ct=dst; cs=src;
//	while(*cs)
//		{ *ct++=*cs++; }
//	*ct++=0;

	while(*ct++=*cs++);

	return(0);
}
#endif

#if 1
int strcmp(char *dst, char *src)
{
//	u32 *cti, *csi;
	char *ct, *cs;

#if 0
	int it, is;
	
	ct=dst; cs=src;	
	it=*ct++; is=*cs++;
	while(it && (it==is))
		{ it=*ct++; is=*cs++; }
	return(it-is);
#endif	

#if 1
	ct=dst; cs=src;	
	while(*cs && (*cs!=*ct))
		{ cs++; ct++; }
	return(*ct-*cs);
#endif

#if 0
	ct=dst; cs=src;	
//	while(*cs && *ct)
	while(*cs)
	{
		if(*cs!=*ct)
			break;
		cs++; ct++;
	}
	
	return(*ct-*cs);
#endif
	
//	if(*cs)
//		return(-1);
//	if(*ct)
//		return( 1);
//	return(0);
}
#endif

u32 TK_GetTimeMs(void)
{
	u32 *sreg;
	int ms;

	sreg=(int *)0x007F8000;
//	ms=(P_AIC_RTC_NSEC>>20)|(P_AIC_RTC_SEC_LO*1000);
	ms=sreg[4];
	return(ms);
}

int clock(void)
{
	return(TK_GetTimeMs());
//	return(0);
}

int time(void)
{
	return(clock()>>10);
}

int main(int argc, char *argv[]);

void __start()
{
	char *t_argv[4];
	
	puts("__start: OK A\n");
	
	t_argv[0]="tk_dummy";
	main(1, t_argv);
}


#if 1
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
#endif

#if 1
typedef struct u128_obj_s u128_obj_t;
struct u128_obj_s {
u32 la;
u32 lb;
u32 lc;
u32 ld;
};

u128_obj_t __negxli(u128_obj_t va)
{
	u128_obj_t oval;
	u64 t;
	
	t=((u64)(~va.la))+1;
	oval.la=t; t=t>>32;
	t=t+((u64)(~va.lb))+0;
	oval.lb=t; t=t>>32;
	t=t+((u64)(~va.lc))+0;
	oval.lc=t; t=t>>32;
	t=t+((u64)(~va.ld))+0;
	oval.ld=t;
	return(oval);
}

u128_obj_t __addxli(u128_obj_t va, u128_obj_t vb)
{
	u128_obj_t oval;
	u64 t;
	
	t=((u64)va.la)+vb.la;
	oval.la=t; t=t>>32;
	t=t+((u64)va.lb)+vb.lb;
	oval.lb=t; t=t>>32;
	t=t+((u64)va.lc)+vb.lc;
	oval.lc=t; t=t>>32;
	t=t+((u64)va.ld)+vb.ld;
	oval.ld=t;
	return(oval);
}

#if 1
u128_obj_t __subxli(u128_obj_t va, u128_obj_t vb)
{
	u128_obj_t oval, vc;
	u64 t;
	
	vc=__negxli(vb);
	oval=__addxli(va, vc);
	return(oval);
}

u128_obj_t __andxli(u128_obj_t va, u128_obj_t vb)
{
	u128_obj_t oval;
	oval.la=va.la&va.la;
	oval.lb=va.lb&va.lb;
	oval.lc=va.lc&va.lc;
	oval.ld=va.ld&va.ld;
	return(oval);
}

u128_obj_t __orxli(u128_obj_t va, u128_obj_t vb)
{
	u128_obj_t oval;
	oval.la=va.la|va.la;
	oval.lb=va.lb|va.lb;
	oval.lc=va.lc|va.lc;
	oval.ld=va.ld|va.ld;
	return(oval);
}

u128_obj_t __xorxli(u128_obj_t va, u128_obj_t vb)
{
	u128_obj_t oval;
	oval.la=va.la^va.la;
	oval.lb=va.lb^va.lb;
	oval.lc=va.lc^va.lc;
	oval.ld=va.ld^va.ld;
	return(oval);
}
#endif

#if 0
u128_obj_t __mulxli(u128_obj_t va, u128_obj_t vb)
{
	u128_obj_t oval;
	u64 p00, p01, p02, p03;
	u64 p10, p11, p12;
	u64 p20, p21;
	u64 p30;
	u32 s0, s1, s2, s3;
	u32     t1, t2, t3;
	u32         u2, u3;
	u32             v3;
	s64 t;
	
	p00=((u64)va.la)*vb.la;	p01=((u64)va.la)*vb.lb;
		p02=((u64)va.la)*vb.lc;	p03=((u64)va.la)*vb.ld;
	p10=((u64)va.lb)*vb.la;	p11=((u64)va.lb)*vb.lb;
		p12=((u64)va.lb)*vb.lc;
	p20=((u64)va.lc)*vb.la;	p21=((u64)va.lc)*vb.lb;
	p30=((u64)va.ld)*vb.la;

	t=p00;			s0=t;
	t=(t>>32)+p01;	s1=t;
	t=(t>>32)+p02;	s2=t;
	t=(t>>32)+p03;	s3=t;

	t=p10;			t1=t;
	t=(t>>32)+p20;	t2=t;
	t=(t>>32)+p30;	t3=t;

	t=p11;			u2=t;
	t=(t>>32)+p12;	u3=t;

	v3=p21;

	t=((u64)s1)+t1;			s1=t;
	t=(t>>32)+((u64)s2)+t2;	s2=t;
	t=(t>>32)+((u64)s3)+t3;	s3=t;
	
	t=((u64)s2)+u2;			s2=t;
	t=(t>>32)+((u64)s3)+u3;	s3=t;
	
	t=((u64)s3)+v3;			s3=t;

	oval.la=s0;	oval.lb=s1;
	oval.lc=s2;	oval.lc=s3;
	return(oval);
}
#endif

#if 1
u128_obj_t __shlxli(u128_obj_t va, int shl)
{
	u128_obj_t oval;

	oval.la=va.la;	oval.lb=va.lb;
	oval.lc=va.lc;	oval.ld=va.ld;
	
	if(!shl)
	{
		oval.la=va.la;	oval.lb=va.lb;
		oval.lc=va.lc;	oval.ld=va.ld;
		return(oval);
	}

	if(shl==32)
	{
		oval.la=0;		oval.lb=va.la;
		oval.lc=va.lb;	oval.ld=va.lc;
		return(oval);
	}

	if(shl==64)
	{
		oval.la=0;		oval.lb=0;
		oval.lc=va.la;	oval.ld=va.lb;
		return(oval);
	}

	if(shl==96)
	{
		oval.la=0;		oval.lb=0;
		oval.lc=0;		oval.ld=va.la;
		return(oval);
	}

#if 1
	if(shl>96)
	{
		oval.la=0;		oval.lb=0;
		oval.lc=0;		oval.ld=va.la<<(shl-96);
		return(oval);
	}

	if(shl>64)
	{
		oval.la=0;		oval.lb=0;
		oval.lc=va.la<<(shl-64);
		oval.ld=(va.lb<<(shl-64))|(va.la>>(96-shl));
		return(oval);
	}

	if(shl>32)
	{
		oval.la=0;
		oval.lb=(va.la<<(shl-32));
		oval.lc=(va.lb<<(shl-32))|(va.la>>(64-shl));
		oval.ld=(va.lc<<(shl-32))|(va.lb>>(64-shl));
		return(oval);
	}

//	if(!shl)
	if(1)
	{
		oval.la=(va.la<<shl);
		oval.lb=(va.lb<<shl)|(va.la>>(32-shl));
		oval.lc=(va.lc<<shl)|(va.lb>>(32-shl));
		oval.ld=(va.ld<<shl)|(va.lc>>(32-shl));
		return(oval);
	}
#endif
}
#endif

u128_obj_t __shrxli(u128_obj_t va, int shl)
{
	u128_obj_t oval;
	
//	printf("__shrxli: %8X_%8X_%8X_%8X>>%d\n",
//		va.la, va.lb, va.lc, va.ld, shl);

//	oval.la=va.la;	oval.lb=va.lb;
//	oval.lc=va.lc;	oval.ld=va.ld;
	
//	*(int *)-1=-1;
	
	if(!shl)
	{
		oval.la=va.la;	oval.lb=va.lb;
		oval.lc=va.lc;	oval.ld=va.ld;
		return(oval);
//		return(va);
	}

	if(shl==32)
	{
		oval.la=va.lb;	oval.lb=va.lc;
		oval.lc=va.ld;	oval.ld=0;
		return(oval);
	}

	if(shl==64)
	{
		oval.la=va.lc;	oval.lb=va.ld;
		oval.lc=0;		oval.ld=0;
		return(oval);
	}

	if(shl==96)
	{
		oval.la=va.ld;	oval.lb=0;
		oval.lc=0;		oval.ld=0;

//		*(int *)-1=-1;
		return(oval);
	}

	oval.la=0x55AA55AA;	oval.lb=0x55AA55AA;
	oval.lc=0x55AA55AA;	oval.ld=0x55AA55AA;
	return(oval);

//	return(va);
}

#endif
