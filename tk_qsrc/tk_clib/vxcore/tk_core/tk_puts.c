void __halt(void);

void tk_dbg_putc(int val)
{
	while(P_MMIO_DEBUG_STS&8);
	P_MMIO_DEBUG_TX=val;
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


int tk_kbhit(void)
{
	return(tk_ps2kb_kbhit());
}

int tk_getch(void)
{
	return(tk_ps2getch());
}

void tk_puts(char *msg)
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
		
		if(i>=0x80)
			continue;
		
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

void tk_print_hex_n(u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";

//	char *chrs;
//	chrs="0123456789ABCDEF";

	if(n>8)
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

void tk_print_decimal(int val)
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

void tk_printf(char *str, ...)
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
			{ tk_putc(*s++); continue; }
		if(s[1]=='%')
			{ s+=2; tk_putc('%'); continue; }
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
				tk_print_decimal_n(v, w);
			}else
			{
				tk_print_decimal(v);
			}
			break;
		case 'X':
//			if(!w)w=8;
//			v=(int)(*plst++);
			v=va_arg(lst, int);

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
			tk_puts(s1);
			break;

		case 'p':
			s1=va_arg(lst, char *);
			tk_print_hex((u32)s1);
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

char *tk_rsplit(char *str)
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
