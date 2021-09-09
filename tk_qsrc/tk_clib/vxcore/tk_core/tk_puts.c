void __halt(void);

byte tk_dbg_iscopy;

void tk_dbg_putc(int val)
{
	while(P_MMIO_DEBUG_STS&8);
	P_MMIO_DEBUG_TX=val;
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


#ifndef __TK_CLIB_ONLY__
void tk_putc_i(int val)
{
	if(val=='\n')
	{
		if(!tk_dbg_iscopy)
			tk_dbg_putc('\r');
		tk_con_putc('\r');
	}
	if(!tk_dbg_iscopy)
		tk_dbg_putc(val);
	tk_con_putc(val);
}
#endif

void tk_putc_v(int val)
{
	TK_SysArg ar[4];
	void *p;
	
	p=NULL;
	ar[0].i=val;
	tk_syscall(NULL, TK_UMSG_CONPUTC, &p, ar);
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
int tk_kbhit_i(void)
{
#if 1
	if(!tk_dbg_iscopy)
	{
		if(tk_dbg_kbhit())
			return(1);
	}
#endif
	return(tk_ps2kb_kbhit());
}

int tk_getch_i(void)
{
#if 1
	if(!tk_dbg_iscopy)
	{
		while(1)
		{
			if(tk_dbg_kbhit())
				return(tk_dbg_getch());
			if(tk_ps2kb_kbhit())
				return(tk_ps2getch());
			__halt();
		}
	}
#endif
	return(tk_ps2getch());
}
#endif

int tk_kbhit_v(void)
{
	TK_SysArg ar[4];
	int i;
	i=0;
	tk_syscall(NULL, TK_UMSG_CONKBHIT, &i, ar);
	return(i);
}

int tk_getch_v(void)
{
	TK_SysArg ar[4];
	int i;
	i=0;
	tk_syscall(NULL, TK_UMSG_CONGETCH, &i, ar);
	return(i);
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
	if(!tk_iskernel())
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
	if(!tk_iskernel())
	{
		tk_getch_fn=tk_getch_v;
	}

	return(tk_getch_fn());
#else
	return(tk_getch_v());
#endif
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

//	tk_puts("\x1f\b");

	t=buf;
	while(1)
	{
		i=tk_getch();
		if(i<=0)
			continue;
		
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

void tk_print_hexptr(long v)
{
//	if(sizeof(void *)>4)
	if(sizeof(long)>4)
	{
		tk_print_hex_n(v>>32, 4);
	}
	tk_print_hex((u32)v);
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
			if(!s1)
				s1="(null)";
			tk_puts(s1);
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


void tk_sprintf(char *buf, char *str, ...)
{
//	void **plst;
	va_list lst;
	char pcfill;
	char *s, *s1, *ct;
	int v, w;

//	plst=(void **)(&str);
//	plst++;
	va_start(lst, str);
	
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

char *tk_rsplit_sep(char *str, int sep)
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

char *tk_rsplit(char *str)
{
	return(tk_rsplit_sep(str, ' '));
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
					{ fwrite(buf, 1, 128, tk_dbg_recvfile); }
				state=2; break;
			}
			if(ch==0x04)	/* EOT: End of Data */
			{
				while((nbufb>0) && (buf[nbufb-1]==0x1A))
					nbufb--;
				if(tk_dbg_recvfile && (nbufb>=0))
					{ fwrite(buf, 1, nbufb, tk_dbg_recvfile); }
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
