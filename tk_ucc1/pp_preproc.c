int TKUCC_PpEnterInclude(TKUCC_MainContext *ctx, char *name)
{
	char tb[256];
	FILE *fd;
	int i, j;
	
	fd=NULL;
	for(i=0; i<ctx->pp_n_incpath; i++)
	{
		sprintf(tb, "%s/%s", ctx->pp_incpath[i], name);
		fd=fopen(tb, "rb");
		if(fd)
			break;
	}
	
	if(fd)
	{
		j=ctx->src_fd_stkpos++;
		ctx->src_fd_stk[j]=ctx->src_fd;
		ctx->src_fd=fd;
		return(1);
	}
	
	return(-1);
}

int TKUCC_PpGetRawLine(TKUCC_MainContext *ctx, char *obuf)
{
	if(feof(ctx->src_fd))
	{
		if(ctx->src_fd_stkpos>0)
		{
			fclose(ctx->src_fd);
			ctx->src_fd_stkpos--;
			ctx->src_fd=ctx->src_fd_stk[ctx->src_fd_stkpos];
			return(TKUCC_PpGetRawLine(ctx, obuf));
		}

		obuf[0]=0;
		return(-1);
	}
	
	fgets(obuf, 256, ctx->src_fd);
	return(0);
}

int TKUCC_PpGetRawLineNoWs(TKUCC_MainContext *ctx, char *obuf)
{
	char *s, *t;
	int i, j, l;

	i=TKUCC_PpGetRawLine(ctx, obuf);
	if(i<0)
		return(i);

	l=strlen(obuf);
	while(l>0)
	{
		j=obuf[l-1];
		/* Eliminate trailing whitespace */
		if((j!='\n') && (j!='\r') && (j!='\t') && (j!=' '))
			break;
		obuf[l-1]=0;
		l=strlen(obuf);
	}

#if 1
	s=obuf; t=obuf;
	while(*s && (*s<=' '))
		s++;
	if(s!=t)
	{
		/* Eliminate leading whitespace */
		l=strlen(s);
		memmove(t, s, l+1);
	}
#endif

	return(0);
}

TKUCC_LineBuf *TKUCC_PpGetSourceLine(TKUCC_MainContext *ctx)
{
	char tbuf[256];
	TKUCC_LineBuf *tmp;
	char *s, *t;
	int instr, inchr;
	int i, j, k, l;
	
	i=TKUCC_PpGetRawLineNoWs(ctx, tbuf);
	if(i<0)
		return(NULL);

#if 1
	/* Strip comments but ignore those inside strings */
	s=tbuf; instr=0; inchr=0;
	while(*s)
	{
		if((s[0]=='\"') && !inchr)
		{
			instr=!instr;
			s++;
			continue;
		}
		
		if((s[0]=='\'') && !instr)
		{
			inchr=!inchr;
			s++;
			continue;
		}
		
		if(instr || inchr)
		{
			if(s[0]=='\\')
			{
				s++;
			}
		}else
		{
			if((s[0]=='/') && (s[1]=='/'))
			{
				s[0]=0;
				break;
			}
		}
		
		s++;
	}
#endif
	
	l=strlen(tbuf);
	while(tbuf[l-1]=='\\')
	{
		TKUCC_PpGetRawLineNoWs(ctx, tbuf+(l-1));
		l=strlen(tbuf);
	}

	tmp=TKUCC_AllocLineForString(ctx, tbuf);
	return(tmp);
}

int TKUCC_PpHashName(char *str)
{
	char *s;
	int h;
	
	s=str; h=0;
	while(*s)
	{
		h=h*251+(*s++)+31;
	}
	h=h*251+31;
	h=h*251+31;
	return((h>>8)&255);
}

void TKUCC_PpAddDefine(TKUCC_MainContext *ctx,
	char *name, char **args, char *body)
{
	TKUCC_PpDefine *ppd;
	char *s0;
	int i, h;
	
	ppd=TKUCC_ZMalloc(ctx, sizeof(TKUCC_PpDefine), TKUCC_ZID_PREPROC);
	ppd->name=TKUCC_StrdupInternPp(ctx, name);
	ppd->body=TKUCC_StrdupInternPp(ctx, body);
	
	if(args)
	{
		for(i=0; i<16; i++)
		{
			s0=args[i];
			if(!s0)
			{
				ppd->args[i]=NULL;
				break;
			}
			ppd->args[i]=TKUCC_StrdupInternPp(ctx, s0);
		}
	}else
	{
		ppd->args[0]=NULL;
	}
	
	h=TKUCC_PpHashName(name);
	ppd->next=ctx->ppdef[h];
	ctx->ppdef[h]=ppd;
}

TKUCC_PpDefine *TKUCC_PpLookupDefine(TKUCC_MainContext *ctx,
	char *name)
{
	TKUCC_PpDefine *ppd;
	int h;

	h=TKUCC_PpHashName(name);

	ppd=ctx->ppdef[h];
	while(ppd)
	{
		if(!strcmp(ppd->name, name))
			return(ppd);
		ppd=ppd->next;
	}
	return(NULL);
}

TKUCC_PpDefine *TKUCC_PpLookupDefineTok(TKUCC_MainContext *ctx,
	char *name)
{
	if(!TKUCC_TokenNameP(name))
		return(NULL);
	return(TKUCC_PpLookupDefine(ctx, name));
}

int TKUCC_PpDoExpandLine(TKUCC_MainContext *ctx,
	char *otbuf, char *osbuf)
{
	char *tda[256];
	TKUCC_PpDefine *ppd;
	char **a;
	char *t, *s0;
	int ntda, nt;
	int i, j, k, l;

	a=TKUCC_TokSplitLine(osbuf);
	
	for(i=0; a[i]; i++)
		{ tda[i]=TKUCC_StrdupInternPpTemp(ctx, a[i]); }
	tda[i]=0;
	
	nt=1;
	while(nt)
	{
		t=otbuf; nt=0;
		for(i=0; tda[i]; i++)
		{
			s0=tda[i];

			ppd=TKUCC_PpLookupDefineTok(ctx, s0);
			if(ppd)
			{
				strcpy(t, ppd->body);
				t+=strlen(t);
				nt++;
				continue;
			}

			t=TKUCC_TokFlattenTokenBuf(t, s0);
		}

		if(nt)
		{
			a=TKUCC_TokSplitLine(otbuf);

			for(i=0; a[i]; i++)
				{ tda[i]=TKUCC_StrdupInternPpTemp(ctx, a[i]); }
			tda[i]=0;
		}
	}
	return(0);
}



s64 TKUCC_PpParseEvalExprI(TKUCC_MainContext *ctx, void *rva)
{
	char **a;
	char *s0, *s1;
	s64 v0, v1;
	long li;
	double f;

	a=*(char ***)rva;

#if 0
	if(TKUCC_TokenNameP(a[0]))
	{
		s0=a[0];
		
		s1=a[1];
		if(s1 && !strcmp(s1, "("))
		{
			a+=2;
			v1=TKUCC_PpParseEvalFunArgs(ctx, &a);
			v0=TKSH_BasEvalDispatchFunction(ctx, s0, v1);
			*(char ***)rva=a+1;
			return(v1);
		}
		
		*(char ***)rva=a+1;
		return(TKSH_BasGetVar(ctx, s0));
	}
#endif

	if(TKUCC_TokenNumberP(a[0]))
	{
		s0=a[0];
		*(char ***)rva=a+1;
		f=atof(s0);
		li=f;
		return(li);
	}

#if 0
	if(TKUCC_TokenStringP(a[0]))
//	if(1)
	{
		s0=a[0];
		v1=__lvo_wrapstring(s0);
		*(char ***)rva=a+1;
		
//		__debugbreak();
//		tk_printf(" %s->%p ", s0, v1);
		return(v1);
	}
#endif

	if(!strcmp(a[0], "("))
	{
		a++;
		v0=TKUCC_PpParseEvalExpr(ctx, &a);

		if(!strcmp(a[0], ")"))
		{
			*(char ***)rva=a+1;
			return(v0);
		}
	}

//	tk_printf("?%s?\n", a[0]);
//	__debugbreak();

	*(char ***)rva=a+1;
	return(0);
}

s64 TKUCC_PpParseEvalExprMD(TKUCC_MainContext *ctx, void *rva)
{
	char **a;
	char *s0;
	s64 v0, v1;

	a=*(char ***)rva;
	v0=TKUCC_PpParseEvalExprI(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "*"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprI(ctx, &a);
			v0=v0*v1;
			continue;
		}
		if(!strcmp(s0, "/"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprI(ctx, &a);
			v0=v0/v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

s64 TKUCC_PpParseEvalExprAS(TKUCC_MainContext *ctx, void *rva)
{
	char **a;
	char *s0;
	s64 v0, v1;

	a=*(char ***)rva;
	v0=TKUCC_PpParseEvalExprMD(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "+"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprMD(ctx, &a);
			v0=v0+v1;
			continue;
		}
		if(!strcmp(s0, "-"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprMD(ctx, &a);
			v0=v0-v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

s64 TKUCC_PpParseEvalExprBit(TKUCC_MainContext *ctx, void *rva)
{
	char **a;
	char *s0;
	s64 v0, v1;

	a=*(char ***)rva;
	v0=TKUCC_PpParseEvalExprAS(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "&"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprAS(ctx, &a);
			v0=v0&v1;
			continue;
		}
		if(!strcmp(s0, "|"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprAS(ctx, &a);
			v0=v0|v1;
			continue;
		}
		if(!strcmp(s0, "^"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprAS(ctx, &a);
			v0=v0|v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

s64 TKUCC_PpParseEvalExprRelCmp(TKUCC_MainContext *ctx, void *rva)
{
	char **a;
	char *s0;
	s64 v0, v1;

	a=*(char ***)rva;
	v0=TKUCC_PpParseEvalExprBit(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "=="))
		{
			a++;
			v1=TKUCC_PpParseEvalExprBit(ctx, &a);
			v0=v0==v1;
			continue;
		}
		if(!strcmp(s0, "!="))
		{
			a++;
			v1=TKUCC_PpParseEvalExprBit(ctx, &a);
			v0=v0!=v1;
			continue;
		}
		if(!strcmp(s0, "<"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprBit(ctx, &a);
			v0=v0<v1;
			continue;
		}
		if(!strcmp(s0, ">"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprBit(ctx, &a);
			v0=v0>v1;
			continue;
		}
		if(!strcmp(s0, "<="))
		{
			a++;
			v1=TKUCC_PpParseEvalExprBit(ctx, &a);
			v0=v0<=v1;
			continue;
		}
		if(!strcmp(s0, ">="))
		{
			a++;
			v1=TKUCC_PpParseEvalExprBit(ctx, &a);
			v0=v0>=v1;
			continue;
		}
		break;
	}
	*(char ***)rva=a;
	return(v0);
}

s64 TKUCC_PpParseEvalExprLogic(TKUCC_MainContext *ctx, void *rva)
{
	char **a;
	char *s0;
	s64 v0, v1;

	a=*(char ***)rva;
	v0=TKUCC_PpParseEvalExprRelCmp(ctx, &a);
	while(a[0])
	{
		s0=a[0];
		if(!strcmp(s0, "&&"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprRelCmp(ctx, &a);
			v0=(v0!=0)&(v1!=0);
			continue;
		}

		if(!strcmp(s0, "||"))
		{
			a++;
			v1=TKUCC_PpParseEvalExprRelCmp(ctx, &a);
			v0=(v0!=0)|(v1!=0);
			continue;
		}

		break;
	}
	*(char ***)rva=a;
	return(v0);
}

s64 TKUCC_PpParseEvalExprInt(TKUCC_MainContext *ctx, char **toks)
{
	return(TKUCC_PpParseEvalExprLogic(ctx, &toks));
}

TKUCC_LineBuf *TKUCC_PpGetProcessedLines(TKUCC_MainContext *ctx)
{
	char osbuf[1024];
	char otbuf[1024];
	char *tda[32];
	TKUCC_LineBuf *line;
	TKUCC_PpDefine *ppd;
	char **a, **a1;
	char *t, *sn, *s0, *s1;
	int ntda, nt;
	int i, j, k, l;
	
	line=TKUCC_PpGetSourceLine(ctx);

	strcpy(osbuf, line->text);
	l=strlen(osbuf);
	while(osbuf[l-1]=='\\')
	{
		TKUCC_FreeLine(ctx, line);
		line=TKUCC_PpGetSourceLine(ctx);
		strcpy(osbuf+(l-1), line->text);
		l=strlen(osbuf);
	}
	TKUCC_FreeLine(ctx, line);

	a=TKUCC_TokSplitLine(osbuf);
	
	if(TKUCC_TokenCheckOperatorP(a[0], "#"))
	{		
		if(TKUCC_TokenCheckNameP(a[1], "define"))
		{
			sn=TKUCC_StrdupInternPp(ctx, a[2]);

			ntda=0;
			if(TKUCC_TokenCheckOperatorP(a[3], "("))
			{
				a1=a+4;
				while(*a1)
				{
					if(TKUCC_TokenCheckOperatorP(a1[0], ")"))
					{
						a1++;
						break;
					}
					
					if(TKUCC_TokenNameP(a1[0]))
					{
						tda[ntda++]=TKUCC_StrdupInternPp(ctx, a1[0]);
						a1++;
						if(TKUCC_TokenCheckOperatorP(a1[1], ","))
							a1++;
					}
				}
			}else
			{
				a1=a+3;
			}
			tda[ntda]=NULL;
			
			TKUCC_TokFlattenTokenListBuf(otbuf, a1);
			
			TKUCC_PpAddDefine(ctx, sn, tda, otbuf);

			line=TKUCC_AllocLineForString(ctx, "");
			return(line);
		}

		if(TKUCC_TokenCheckNameP(a[1], "ifdef"))
		{
			if(ctx->pp_lvl_iff>0)
			{
				ctx->pp_lvl_iff++;
			}else
			{
				ppd=TKUCC_PpLookupDefineTok(ctx, a[2]);
				if(ppd)
					ctx->pp_lvl_ift++;
				else
					ctx->pp_lvl_iff++;
			}
		}

		if(TKUCC_TokenCheckNameP(a[1], "ifndef"))
		{
			if(ctx->pp_lvl_iff>0)
			{
				ctx->pp_lvl_iff++;
			}else
			{
				ppd=TKUCC_PpLookupDefineTok(ctx, a[2]);
				if(!ppd)
					ctx->pp_lvl_ift++;
				else
					ctx->pp_lvl_iff++;
			}
		}

		if(TKUCC_TokenCheckNameP(a[1], "endif"))
		{
			if(ctx->pp_lvl_iff>0)
			{
				ctx->pp_lvl_iff--;
			}else
				if(ctx->pp_lvl_ift>0)
			{
				ctx->pp_lvl_ift--;
			}else
			{
				TKUCC_PpError(ctx, "Unexpected #endif");
			}
		}

		if(TKUCC_TokenCheckNameP(a[1], "include"))
		{
			if(TKUCC_TokenStringP(a[2]))
			{
				strcpy(otbuf, a[2]);
			}else if(TKUCC_TokenCheckOperatorP(a[2], "<"))
			{
				t=otbuf; a1=a+3;
				while(*a1)
				{
					if(TKUCC_TokenCheckOperatorP(a1[0], ">"))
						break;
					t=TKUCC_TokFlattenTokenBufNoPad(t, a1[0]);
				}
			}
			TKUCC_PpEnterInclude(ctx, otbuf);
		}

		if(TKUCC_TokenCheckNameP(a[1], "if"))
		{
			t=otbuf; a1=a+2;
			while(*a1)
			{
				t=TKUCC_TokFlattenTokenBuf(t, a1[0]);
			}

			if(ctx->pp_lvl_iff>0)
			{
				ctx->pp_lvl_iff++;
			}else
			{
				TKUCC_PpDoExpandLine(ctx, otbuf, otbuf);
				a1=TKUCC_TokSplitLine(otbuf);			
				j=TKUCC_PpParseEvalExprInt(ctx, a1);

				if(j)
					ctx->pp_lvl_ift++;
				else
					ctx->pp_lvl_iff++;
			}
		}

		line=TKUCC_AllocLineForString(ctx, "");
		return(line);
	}

	/* Check for macro expansions. */
	for(i=0; a[i]; i++)
	{
		ppd=TKUCC_PpLookupDefineTok(ctx, a[i]);
		if(ppd)
			break;
	}

	if(!a[i])
	{
		/* No macro expansions. */
		TKUCC_TokFlattenTokenListBuf(otbuf, a);
		line=TKUCC_AllocLineForString(ctx, otbuf);
		return(line);
	}

	TKUCC_PpDoExpandLine(ctx, otbuf, osbuf);

	line=TKUCC_AllocLineForString(ctx, otbuf);
	return(line);
}
