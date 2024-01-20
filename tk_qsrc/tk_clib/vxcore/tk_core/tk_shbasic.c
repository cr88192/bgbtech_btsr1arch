/*
Simplistic BASIC variant.
 */

#ifdef __BGBCC__

typedef struct TKSH_BasicCtx_s TKSH_BasicCtx;

struct TKSH_BasicCtx_s {
	__variant rootenv;
	
	char *lbl_name[256];
	int lbl_lnum[256];
	int n_lbl;
};

__object __lvo_wrapstring(char *str);
__object __lvo_emptyobject(void);

__variant  __lvo_getslot(__object obj, char *name);
void __lvo_setslot(__object obj, char *name, __variant val);

__variant TKSH_BasParseEvalExpr(TKSH_BasicCtx *ctx, void *rva);

TKSH_BasicCtx *TKSH_BasAllocContext()
{
	TKSH_BasicCtx *ctx;
	ctx=tk_malloc(sizeof(TKSH_BasicCtx));
	memset(ctx, 0, sizeof(TKSH_BasicCtx));
	
	ctx->rootenv=__lvo_emptyobject();
	
	return(ctx);
}

int TKSH_BasDestroyContext(TKSH_BasicCtx *ctx)
{
	tk_free(ctx);
	return(0);
}

char *TKSK_BasEatWhite(char *str)
{
	char *s;
	s=str;
	while(*s && *s<=' ')
		s++;
	return(s);
}

char *TKSK_BasReadToken(char *str, char *buf, int *rtokty)
{
	char *s, *t;
	
	t=buf;
	s=str;
	s=TKSK_BasEatWhite(s);

	if(!*s)
	{
		*buf=0;
		*rtokty=0;
		return(s);
	}

	if(	((*s>='A') && (*s<='Z')) ||
		((*s>='a') && (*s<='z')) ||
		*s=='_')
	{
		while(	((*s>='A') && (*s<='Z')) ||
			((*s>='a') && (*s<='z')) ||
			((*s>='0') && (*s<='9')) ||
			*s=='_')
		{
			*t++=*s++;
		}
		*rtokty=1;
		*t=0;
		return(s);
	}
	
	if(*s=='0')
	{
		if((s[1]=='x') || (s[1]=='X'))
		{
			*t++=*s++;
			*t++=*s++;
			while(	((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f')) ||
				((*s>='0') && (*s<='9')) ||
				*s=='_')
			{
				if(*s=='_')
					{ s++; continue; }
				*t++=*s++;
			}
			*rtokty=2;
			*t=0;
			return(s);
		}
	}

	if((*s>='0') && (*s<='9'))
	{
		while(
			((*s>='0') && (*s<='9')) ||
			*s=='_')
		{
			if(*s=='_')
				{ s++; continue; }
			*t++=*s++;
		}
		*rtokty=2;
		*t=0;
		return(s);
	}

	if(*s=='\"')
	{
		s++;
		while(*s && *s!='\"')
		{
			*t++=*s++;
		}
		if(*s=='\"')
			s++;

		*rtokty=3;
		*t=0;
		return(s);
	}

	if(s[1]=='=')
	{
		if(	(s[0]=='<') ||
			(s[0]=='>') ||
			(s[0]=='=') ||
			(s[0]=='!'))
		{
			*rtokty=4;
			*t++=*s++;
			*t++=*s++;
			*t=0;
			return(s);
		}
	}

	*rtokty=4;
	*t++=*s++;
	*t=0;
	return(s);
}

char **TKSH_BasSplitLine(char *str)
{
	char tbuf[256];
	char *ta[64];
	char *s;
	char **a;
	int i, n, tty;
	
	tty=0;
	s=str; n=0;
	while(*s)
	{
		s=TKSK_BasReadToken(s, tbuf+1, &tty);
		if(!tty)
			continue;
		tbuf[0]='A'+tty;
		ta[n++]=tk_rstrdup(tbuf);
	}
	ta[n]=NULL;
	
	a=tk_ralloc((n+1)*sizeof(char *));
	for(i=0; i<=n; i++)
	{
		s=ta[i];
		if(s)s=s+1;
		a[i]=s;
	}
	return(a);
}

int TKSH_BasTokenNameP(char *tok)
{
	if(*(tok-1)=='B')
		return(1);

//	if((*tok>='A') && (*tok<='Z'))
//		return(1);
//	if((*tok>='a') && (*tok<='z'))
//		return(1);
//	if(*tok=='_')
//		return(1);
	return(0);
}

int TKSH_BasTokenNumberP(char *tok)
{
	if(*(tok-1)=='C')
		return(1);

//	if((*tok>='0') && (*tok<='9'))
//		return(1);
	return(0);
}

int TKSH_BasTokenStringP(char *tok)
{
	if(*(tok-1)=='D')
		return(1);
//	if((*tok>='0') && (*tok<='9'))
//		return(1);
	return(0);
}

__variant TKSH_BasGetVar(TKSH_BasicCtx *ctx, char *name)
{
	return(__lvo_getslot(ctx->rootenv, name));
}

int TKSH_BasSetVar(TKSH_BasicCtx *ctx, char *name, __variant val)
{
	__lvo_setslot(ctx->rootenv, name, val);
}

__variant TKSH_BasParseEvalFunArgs(TKSH_BasicCtx *ctx, void *rva)
{
	__variant va[16];

	__variant v0, v1;
	char **a;
	char *s0, *s1;
	int na;

	a=*(char ***)rva;

	na=0;
	s0=a[0];
	while(s0)
	{
		if(!strcmp(s0, ")"))
		{
			a++;
			break;
		}
		
		v0=TKSH_BasParseEvalExpr(ctx, &a);
		va[na++]=v0;

		s0=a[0];
		if(!strcmp(s0, ","))
		{
			a++;
			s0=a[0];
		}
	}
	
	v0=__lvo_newvararray_vn(va, na);
	return(v0);
}

__variant TKSH_BasEvalDispatchFunction(TKSH_BasicCtx *ctx,
	char *name, __variant args)
{
}

__variant TKSH_BasParseEvalExprI(TKSH_BasicCtx *ctx, void *rva)
{
	char **a;
	char *s0, *s1;
	__variant v0, v1;
	long li;
	double f;

	a=*(char ***)rva;

	if(TKSH_BasTokenNameP(a[0]))
	{
		s0=a[0];
		
		s1=a[1];
		if(s1 && !strcmp(s1, "("))
		{
			a+=2;
			v1=TKSH_BasParseEvalFunArgs(ctx, &a);
			v0=TKSH_BasEvalDispatchFunction(ctx, s0, v1);
			*(char ***)rva=a+1;
			return(v1);
		}
		
		*(char ***)rva=a+1;
		return(TKSH_BasGetVar(ctx, s0));
	}

	if(TKSH_BasTokenNumberP(a[0]))
	{
		s0=a[0];
		*(char ***)rva=a+1;
		f=atof(s0);
		
		li=f;
		if(f==li)
		{
			return((__variant)li);
		}

		return((__variant)f);
	}
	
	if(TKSH_BasTokenStringP(a[0]))
//	if(1)
	{
		s0=a[0];
		v1=__lvo_wrapstring(s0);
		*(char ***)rva=a+1;
		
//		__debugbreak();
//		tk_printf(" %s->%p ", s0, v1);
		return(v1);
	}

	if(!strcmp(a[0], "("))
	{
		a++;
		v0=TKSH_BasParseEvalExpr(ctx, &a);

		if(!strcmp(a[0], ")"))
		{
			*(char ***)rva=a+1;
			return(v0);
		}
	}

	tk_printf("?%s?\n", a[0]);
//	__debugbreak();

	*(char ***)rva=a+1;
	return(__object_null);
}

__variant TKSH_BasParseEvalExprMD(TKSH_BasicCtx *ctx, void *rva)
{
	char **a;
	char *s0;
	__variant v0, v1;

	a=*(char ***)rva;
	v0=TKSH_BasParseEvalExprI(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "*"))
		{
			a++;
			v1=TKSH_BasParseEvalExprI(ctx, &a);
			v0=v0*v1;
			continue;
		}
		if(!strcmp(s0, "/"))
		{
			a++;
			v1=TKSH_BasParseEvalExprI(ctx, &a);
			v0=v0/v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

__variant TKSH_BasParseEvalExprAS(TKSH_BasicCtx *ctx, void *rva)
{
	char **a;
	char *s0;
	__variant v0, v1;

	a=*(char ***)rva;
	v0=TKSH_BasParseEvalExprMD(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "+"))
		{
			a++;
			v1=TKSH_BasParseEvalExprMD(ctx, &a);
			v0=v0+v1;
			continue;
		}
		if(!strcmp(s0, "-"))
		{
			a++;
			v1=TKSH_BasParseEvalExprMD(ctx, &a);
			v0=v0-v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

__variant TKSH_BasParseEvalExprBit(TKSH_BasicCtx *ctx, void *rva)
{
	char **a;
	char *s0;
	__variant v0, v1;

	a=*(char ***)rva;
	v0=TKSH_BasParseEvalExprAS(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "&"))
		{
			a++;
			v1=TKSH_BasParseEvalExprAS(ctx, &a);
			v0=v0&v1;
			continue;
		}
		if(!strcmp(s0, "|"))
		{
			a++;
			v1=TKSH_BasParseEvalExprAS(ctx, &a);
			v0=v0|v1;
			continue;
		}
		if(!strcmp(s0, "^"))
		{
			a++;
			v1=TKSH_BasParseEvalExprAS(ctx, &a);
			v0=v0|v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

__variant TKSH_BasParseEvalExprRelCmp(TKSH_BasicCtx *ctx, void *rva)
{
	char **a;
	char *s0;
	__variant v0, v1;

	a=*(char ***)rva;
	v0=TKSH_BasParseEvalExprBit(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "=="))
		{
			a++;
			v1=TKSH_BasParseEvalExprBit(ctx, &a);
			v0=v0==v1;
			continue;
		}
		if(!strcmp(s0, "!="))
		{
			a++;
			v1=TKSH_BasParseEvalExprBit(ctx, &a);
			v0=v0!=v1;
			continue;
		}
		if(!strcmp(s0, "<"))
		{
			a++;
			v1=TKSH_BasParseEvalExprBit(ctx, &a);
			v0=v0<v1;
			continue;
		}
		if(!strcmp(s0, ">"))
		{
			a++;
			v1=TKSH_BasParseEvalExprBit(ctx, &a);
			v0=v0>v1;
			continue;
		}
		if(!strcmp(s0, "<="))
		{
			a++;
			v1=TKSH_BasParseEvalExprBit(ctx, &a);
			v0=v0<=v1;
			continue;
		}
		if(!strcmp(s0, ">="))
		{
			a++;
			v1=TKSH_BasParseEvalExprBit(ctx, &a);
			v0=v0>=v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

__variant TKSH_BasParseEvalExprLogic(TKSH_BasicCtx *ctx, void *rva)
{
	char **a;
	char *s0;
	__variant v0, v1;

	a=*(char ***)rva;
	v0=TKSH_BasParseEvalExprRelCmp(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "&&") || !stricmp(s0, "and"))
		{
			a++;
			v1=TKSH_BasParseEvalExprRelCmp(ctx, &a);
			v0=(v0!=0)&(v1!=0);
			continue;
		}

		if(!strcmp(s0, "||") || !stricmp(s0, "or"))
		{
			a++;
			v1=TKSH_BasParseEvalExprRelCmp(ctx, &a);
			v0=(v0!=0)|(v1!=0);
			continue;
		}

		break;
	}
	*(char ***)rva=a;
	return(v0);
}


__variant TKSH_BasParseEvalExpr(TKSH_BasicCtx *ctx, void *rva)
{
//	return(TKSH_BasParseEvalExprAS(ctx, rva));
//	return(TKSH_BasParseEvalExprRelCmp(ctx, rva));
	return(TKSH_BasParseEvalExprLogic(ctx, rva));
}

int TKSH_BasRunCommandTok(TKSH_BasicCtx *ctx, int seq, char **args)
{
	char tbuf[256];
	char **a1;
	char *s0;
	long li;
	double f;
	__variant v1, v2;
	int i, j, k;

	if(!stricmp(args[0], "end"))
		return(-1);

	if(!stricmp(args[0], "rem"))
		return(seq+1);

	if(
		TKSH_BasTokenNameP(args[0]) &&
		!strcmp(args[1], "="))
	{
		a1=args+2;
		v1=TKSH_BasParseEvalExpr(ctx, &a1);
		TKSH_BasSetVar(ctx, args[0], v1);
		return(seq+1);
	}

	if(
		TKSH_BasTokenNameP(args[0]) &&
		!strcmp(args[1], ":"))
	{
		return(seq+1);
	}

	if(	!stricmp(args[0], "let") &&
		TKSH_BasTokenNameP(args[1]) &&
		!strcmp(args[2], "="))
	{
		a1=args+3;
		v1=TKSH_BasParseEvalExpr(ctx, &a1);
		TKSH_BasSetVar(ctx, args[1], v1);
		return(seq+1);
	}

	if(!stricmp(args[0], "print"))
	{
		a1=args+1;
		while(*a1)
		{
			if(!strcmp(*a1, ","))
				{ a1++; continue; }
			if(!strcmp(*a1, ";"))
			{
				a1++;
				continue;
			}
			v1=TKSH_BasParseEvalExpr(ctx, &a1);
			
//			if(__lvo_stringp(v1))
			if(v1 __instanceof __string)
			{
				s0=(char *)v1;
//				__debugbreak();

				tk_puts(s0);
//				tk_printf("S:%s", s0);
				if(!(*a1))
					{ tk_puts("\n"); }
				continue;
			}

//			if(__lvo_fixnump(v1))
			if(v1 __instanceof __fixnum)
			{
				li=(long)v1;
				tk_printf("%d", li);
				if(!(*a1))
					{ tk_puts("\n"); }
				continue;
			}

			if(v1 __instanceof __flonum)
			{
				f=(double)v1;
				tk_printf("%f", f);
				if(!(*a1))
					{ tk_puts("\n"); }
				continue;
			}
			
			tk_printf("?Type:%p", __object_getbits(v1));
			if(!(*a1))
				{ tk_puts("\n"); }
		}
		return(seq+1);
	}

	if(!stricmp(args[0], "goto"))
	{
//		a1=args+1;

		for(i=0; i<ctx->n_lbl; i++)
		{
			if(!stricmp(ctx->lbl_name[i], args[1]))
			{
				return(ctx->lbl_lnum[i]);
			}
		}

		tk_printf("? Missing Label, %s\n", args[1]);
		return(-1);
	}

	if(!strcmp(args[0], "if"))
	{
		a1=args+1;
		v1=TKSH_BasParseEvalExpr(ctx, &a1);
//		TKSH_BasSetVar(ctx, args[0], v1);

		if(v1 __instanceof __fixnum)
		{
			li=(long)v1;
			if(li)
			{
				for(i=0; a1[i]; i++)
				{
					if(!strcmp(a1[i], "then"))
					{
						return(TKSH_BasRunCommandTok(ctx, seq, a1+i+1));
					}
				}
			}else
			{
				for(i=0; a1[i]; i++)
				{
					if(!strcmp(a1[i], "else"))
					{
						return(TKSH_BasRunCommandTok(ctx, seq, a1+i+1));
					}
				}
			}
		}

		return(seq+1);
	}

	if(	!stricmp(args[0], "input") &&
		TKSH_BasTokenStringP(args[1]) &&
		(!strcmp(args[2], ";") || !strcmp(args[2], ",")) &&
		TKSH_BasTokenNameP(args[3]))
	{
		tk_puts(args[1]);
		tk_gets(tbuf);

		a1=TKSH_BasSplitLine(tbuf);
		if(a1[0] && !a1[1])
		{
			if(TKSH_BasTokenNumberP(a1[0]))
			{
				v1=TKSH_BasParseEvalExprI(ctx, &a1);
				TKSH_BasSetVar(ctx, args[3], v1);
			}else
			{
				v1=__lvo_wrapstring(a1[0]);
				TKSH_BasSetVar(ctx, args[3], v1);
			}
		}else
		{
			i=strlen(tbuf);
			if((i>0) && (tbuf[i-1]=='\r'))
				tbuf[i-1]=0;

			v1=__lvo_wrapstring(tbuf);
			TKSH_BasSetVar(ctx, args[3], v1);
		}

//		a1=args+3;
//		v1=TKSH_BasParseEvalExpr(ctx, &a1);
//		TKSH_BasSetVar(ctx, args[1], v1);
		return(seq+1);
	}



	tk_printf("? Syntax Error, %s\n", args[0]);
	return(-1);
}

int TKSH_BasRunCommand(TKSH_BasicCtx *ctx, int seq, char *line)
{
	char **args;
	
	if(seq<0)
		return(seq);
	
//	tk_printf("TKSH_BasRunCommand: %s\n", line);
	
	args=TKSH_BasSplitLine(line);
	if(!args[0])
		return(seq+1);
	
	return(TKSH_BasRunCommandTok(ctx, seq, args));
}

int TKSH_BasPreScanCommand(TKSH_BasicCtx *ctx, int seq, char *line)
{
	char **args;
	char **a1;
	char *s0;
	int i, j, k;
	
	if(!line)
	{
		if(seq<1)
		{
			ctx->n_lbl=0;
			return(0);
		}
		return(0);
	}

	if(seq<0)
		return(seq);
	
//	tk_printf("TKSH_BasRunCommand: %s\n", line);
	
	args=TKSH_BasSplitLine(line);
	if(!args[0])
		return(seq+1);

	if(
		TKSH_BasTokenNameP(args[0]) &&
		!strcmp(args[1], ":"))
	{
		i=ctx->n_lbl++;
		ctx->lbl_name[i]=tk_strdup_in(args[0]);
		ctx->lbl_lnum[i]=seq;
		return(seq+1);
	}

	return(seq+1);
}

int TKSH_BasCountBufferLines(char *buf, int szbuf)
{
	char *cs, *cse;
	int i, ln;
	
	cs=buf; cse=buf+szbuf; ln=1;
	while(cs<cse)
	{
		i=*cs++;
		if(i==0)
			break;
		if(i=='\n')
			ln++;
	}
	return(ln);
}

int TKSH_BasSplitBufferLines(
	char **linebuf, char *txtbuf,
	char *buf, int szbuf)
{
	char *cs, *cse, *ct, *ct0;
	int i, ln;
	
	ct=txtbuf; ct0=ct;
	cs=buf; cse=buf+szbuf; ln=0;
	while(cs<cse)
	{
		i=*cs++;

		if(i==0)
			break;

		if(i=='\n')
		{
			*ct++=0;
			linebuf[ln++]=ct0;
			ct0=ct;
		}
		if(i=='\r')
			continue;

		*ct++=i;
	}
	
	if(ct>ct0)
	{
		*ct++=0;
		linebuf[ln++]=ct0;
	}

	return(ln);
}

int TKSH_BasRunBasicBufferCtx(TKSH_BasicCtx *ctx, char *buf, int szbuf)
{
	char **lbuf;
	char *tbuf;
	int lc, runseq, kk, key, dn, ctrl;
	int i, j, k;
	
	lc=TKSH_BasCountBufferLines(buf, szbuf);

	lbuf=tk_malloc((lc+8)*sizeof(char *));
	tbuf=tk_malloc(szbuf+(2*lc));
	lc=TKSH_BasSplitBufferLines(lbuf, tbuf, buf, szbuf);

	TKSH_BasPreScanCommand(ctx, 0, NULL);
	for(j=1; j<=lc; j++)
	{
		TKSH_BasPreScanCommand(ctx, j, lbuf[j-1]);
	}
	
	runseq=1; ctrl=0;
	while((runseq>0) && (runseq<=lc))
	{
		while(tk_kbhit())
		{
			kk=tk_getch();
			if(kk==0x7F)
				{ key=tk_getch(); dn=1; }
			else if(kk==0xFF)
				{ key=tk_getch(); dn=0; }
			else if(kk==0x80)
			{
				key=tk_getch();
				key=(key<<8)|tk_getch();
				dn=!(key&0x8000);
			}else
			{
				key=kk&0x7F;
				dn=!(kk&0x80);
			}

			if(key==TK_K_CTRL)
			{
				ctrl=dn;
				continue;
			}

			if(ctrl && (key=='c'))
			{
				runseq=-10;
				continue;
			}
		}

		runseq=TKSH_BasRunCommand(
			ctx, runseq,
			lbuf[runseq-1]);
	}
	
	tk_free(tbuf);
	tk_free(lbuf);
	
	return(0);
}

int TKSH_BasRunBasicBuffer(char *buf, int szbuf)
{
	TKSH_BasicCtx *ctx;
	int i;

	ctx=TKSH_BasAllocContext();
	i=TKSH_BasRunBasicBufferCtx(ctx, buf, szbuf);
	TKSH_BasDestroyContext(ctx);
	return(i);
}

#endif

