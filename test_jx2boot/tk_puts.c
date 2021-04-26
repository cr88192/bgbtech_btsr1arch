void __halt(void);


void tk_dbg_putc(int val)
{
	while(P_MMIO_DEBUG_STS&8);
	P_MMIO_DEBUG_TX=val;
}

int tk_dbg_kbhit(void)
{
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

void tk_putc(int val)
{
	if(val=='\n')
	{
		tk_dbg_putc('\r');
		tk_con_putc('\r');
	}

	tk_dbg_putc(val);
	tk_con_putc(val);
}

void putc(int val)
{
	tk_putc(val);
}

int kbhit(void)
{
#ifndef JX2UC
	if(tk_ps2kb_kbhit())
		return(1);
#endif
	if(tk_dbg_kbhit())
		return(1);
	return(0);

//	return(tk_ps2kb_kbhit());
//	return(P_MMIO_DEBUG_STS&1);
}

int getch(void)
{
	while(1)
	{
#ifndef JX2UC
		if(tk_ps2kb_kbhit())
//			return(tk_ps2getch());
			return(tk_ps2trygetch());
#endif
		if(P_MMIO_DEBUG_STS&1)
			return(P_MMIO_DEBUG_RX);
//		__halt();
		sleep_0();
	}

#if 0
	while(!(P_MMIO_DEBUG_STS&1))
	{
		__halt();
//		sleep_0();
	}
	return(P_MMIO_DEBUG_RX);
#endif
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
		if(i<=0)
			continue;
		
		if(i>=0x80)
			continue;
		
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


char *async_gets(char *buf)
{
	char *t;
	int i, eol;

	if(!kbhit())
		return(NULL);
	
	t=buf; eol=0;
	while(*t)
		t++;

	while(1)
	{
		if(!kbhit())
			break;

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
			{ putc('\n'); eol=1; break; }
		if(i=='\n')
			{ putc('\n'); eol=1; break; }
		putc(i);
		*t++=i;
	}
	*t=0;
	if(eol)
		return(buf);
	return(NULL);
}

void print_hex(u32 v)
{
	static char *chrs="0123456789ABCDEF";
//	int i;

//	char *chrs;
//	chrs="0123456789ABCDEF";

//	i=chrs[(v>>28)&15];
//	__debugbreak();

	putc(chrs[(v>>28)&15]);
	putc(chrs[(v>>24)&15]);
	putc(chrs[(v>>20)&15]);
	putc(chrs[(v>>16)&15]);
	putc(chrs[(v>>12)&15]);
	putc(chrs[(v>> 8)&15]);
	putc(chrs[(v>> 4)&15]);
	putc(chrs[(v    )&15]);
}

void print_hex_u64(u64 v)
{
	print_hex(v>>32);
	print_hex(v);
}

void print_hex_n(u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	if(n>8)
	{
//		__debugbreak();

//		puts("!%X!\n");
		puts("!%X! ");
		print_hex(n);
		puts(": ");
		print_hex(v);

		puts("\n");
		__debugbreak();
	}

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
		
//		if((i<0) | (i>=10))
//			__debugbreak();
		
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

// #ifdef ARCH_HAS_FPU
#if 1
void print_float(double val)
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

	if(sg)putc('-');
	print_decimal(ip);
	putc('.');
	print_decimal_n(fp, 6);
}
#endif

void printf(char *str, ...)
{
//	void **plst;
	va_list lst;
	char pcfill;
	char *s, *s1;
	int v, w;

//	plst=(void **)(&str);
//	plst++;
	va_start(lst, str);
	
//	__debugbreak();
	
	s=str;
	while(*s)
	{
		if(*s!='%')
			{ putc(*s++); continue; }
		if(s[1]=='%')
			{ s+=2; putc('%'); continue; }
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
//			v=(int)(*plst++);
			v=va_arg(lst, int);
			if(w)
			{
				print_decimal_n(v, w);
			}else
			{
				print_decimal(v);
			}
			break;
		case 'X':
//			if(!w)w=8;
//			v=(int)(*plst++);
			v=va_arg(lst, int);

//			__debugbreak();

			if(!w)w=print_hex_genw(v);

//			__debugbreak();
//			print_hex(v);
			print_hex_n(v, w);
//			__debugbreak();
			break;
		case 's':
//			s1=*plst++;
			s1=va_arg(lst, char *);
			puts(s1);
			break;

		case 'p':
			s1=va_arg(lst, char *);
			print_hex((u32)s1);
			break;

// #ifdef ARCH_HAS_FPU
#if 1
		case 'f':
			print_float(va_arg(lst, double));
			break;
#endif

		default:
//			plst++;
			break;
		}
	}
	va_end(lst);
}

#ifdef JX2UC
byte tk_ralloc_tbuf[512];
#endif

byte *tk_ralloc_bufs=NULL;
byte *tk_ralloc_bufe;
byte *tk_ralloc_bufr;

void *tk_ralloc(int sz)
{
	void *p;
#ifndef JX2UC
	if(!tk_ralloc_bufs)
	{
		tk_ralloc_bufs=malloc(4096);
		tk_ralloc_bufe=tk_ralloc_bufs+4096;
		tk_ralloc_bufr=tk_ralloc_bufs;
	}
#else
	if(!tk_ralloc_bufs)
	{
		tk_ralloc_bufs=tk_ralloc_tbuf;
		tk_ralloc_bufe=tk_ralloc_bufs+512;
		tk_ralloc_bufr=tk_ralloc_bufs;
	}
#endif
	
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

char **tk_rsplit(char *str)
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
		if(*s>' ')
		{
			t=tb;
			while(*s>' ')
				*t++=*s++;
			*t++=0;
			ta[nta++]=tk_rstrdup(tb);
		}
	}
	
	ta2=tk_ralloc((nta+1)*sizeof(char *));
	for(i=0; i<nta; i++)
		ta2[i]=ta[i];
	ta2[i]=NULL;
	return(ta2);
}


u64 TK_GetTimeUs(void)
{
	volatile u32 *sreg;
	u32 us_lo, us_hi;
	u64 us;
	int ms;

	sreg=(volatile u32 *)0xF000E000;
	us_lo=sreg[0];
//	us=0;
	us_hi=sreg[1];
	us=(((u64)us_hi)<<32)|us_lo;
//	ms=us>>10;
//	__debugbreak();
	return(us);
}

u32 TK_GetTimeMs(void)
{
	volatile u32 *sreg;
	u32 us_lo, us_hi;
	u64 us;
	int ms;

	sreg=(volatile u32 *)0xF000E000;
	us_lo=sreg[0];
//	us=0;
	us_hi=sreg[1];
	us=(((u64)us_hi)<<32)|us_lo;
	ms=us>>10;

//	__debugbreak();

	return(ms);
}

int I_GetTime (void)
{
	int t;
	t = TK_GetTimeMs();
//	t=t>>5;
//	t=t*(TICRATE/1000.0);
	t=(t*35LL)>>10;
	return(t);
}
