#include <bgbccc.h>


void BGBCC_CCXL_Warn(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_warn++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: warning: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Error(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_error++;
//	bgbcc_frcc_err++;
	printf("%s:%d: error: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Note(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_note++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: note: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_StubWarnLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Warn(ctx, "StubWarn %s:%d\n", file, line);
}

void BGBCC_CCXL_StubErrorLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "StubError %s:%d\n", file, line);
	BGBCC_DBGBREAK
}

void BGBCC_CCXL_SanityErrorLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "SanityError %s:%d\n", file, line);
	BGBCC_DBGBREAK
}


void BGBCC_CCXL_Warn2(BGBCC_TransState *ctx,
	int lfni, int lln, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_warn++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: warning: ", ctx->fnidx_str[lfni], lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Error2(BGBCC_TransState *ctx,
	int lfni, int lln, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_error++;
//	bgbcc_frcc_err++;
	printf("%s:%d: error: ", ctx->fnidx_str[lfni], lln);
	vprintf(str, lst);

	va_end(lst);
}

char *BGBCC_CCXL_TagGetMessage(int tag)
{
	char *str;

	str=NULL;
	switch(tag)
	{
	case CCXL_TERR_GENERIC:
		str="Generic Error"; break;
	case CCXL_TERR_STACK_OVERFLOW:
		str="Register Stack Overflow";	break;
	case CCXL_TERR_STACK_UNDERFLOW:
		str="Register Stack Underflow";	break;
	case CCXL_TERR_FORMAT_SANITY:
		str="Format sanity check fail";	break;
	case CCXL_TERR_CONV_PTRRANGELOSS:
		str="Pointer conversion loss of range";	break;
	case CCXL_TERR_CONV_PTRSIZEDIFF:
		str="Conversion to pointer size differs";	break;
	case CCXL_TERR_RETVOID:
		str="Returning a value in a function returning void";	break;
	case CCXL_TERR_MISSINGPROTO:
//		str="Missing prototype for function";	break;
		str="Undeclared function";	break;
		
	case CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE):
		str="Unhandled Type";	break;
	case CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS):
		str="Bad Operation Arguments";	break;
	
	default:
		str="(Unknown)";	break;
	}
	return(str);
}

void BGBCC_CCXL_TagErrorLLn(BGBCC_TransState *ctx, int tag,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "TagError(%04X) %s:%d: %s\n", tag, file, line,
		BGBCC_CCXL_TagGetMessage(tag));
}

void BGBCC_CCXL_TagWarnLLn(BGBCC_TransState *ctx, int tag,
	char *file, int line)
{
	int i, j, k;

//	BGBCC_CCXL_Warn(ctx, "TagWarn(%04X) %s:%d: %s\n", tag, file, line,
//		BGBCC_CCXL_TagGetMessage(tag));
//	BGBCC_CCXL_Warn(ctx, "(%04X) %s\n", tag,
//		BGBCC_CCXL_TagGetMessage(tag));

	BGBCC_CCXL_IndexForCurLfn(ctx);
	for(i=0; i<ctx->tagw_n_warn; i++)
	{
		if(	(ctx->tagw_lfn[i]==ctx->lfni) &&
			(ctx->tagw_lln[i]==ctx->lln) &&
			(ctx->tagw_tag[i]==tag))
				break;
	}
	if(i<ctx->tagw_n_warn)
	{
		ctx->tagw_skip=1;
		return;
	}

	ctx->tagw_skip=0;
	i=ctx->tagw_n_warn++;
	ctx->tagw_lfn[i]=ctx->lfni;
	ctx->tagw_lln[i]=ctx->lln;
	ctx->tagw_tag[i]=tag;
	ctx->tagw_pix[i]=ctx->tagw_n_parm;
	ctx->tagw_pcn[i]=0;
}

void BGBCC_CCXL_TagWarnAddParmStr(BGBCC_TransState *ctx, char *str)
{
	int i, j, k;

	if(ctx->tagw_skip)
		return;
	i=ctx->tagw_n_warn-1;
	
	j=BGBCC_CCXL_IndexFName(ctx, str);
	
	k=ctx->tagw_n_parm++;
	ctx->tagw_parm[k]=j;
	ctx->tagw_pcn[i]++;
}

void BGBCC_CCXL_PrintTagWarn(BGBCC_TransState *ctx)
{
	char *stra[16];
	int lfn, lln, tag, pix, pcn;
	int i, j, k;

	stra[0]=NULL;
	stra[1]=NULL;

	for(i=0; i<ctx->tagw_n_warn; i++)
	{
		lfn=ctx->tagw_lfn[i];
		lln=ctx->tagw_lln[i];
		tag=ctx->tagw_tag[i];
		pix=ctx->tagw_pix[i];
		pcn=ctx->tagw_pcn[i];
		
		for(j=0; j<pcn; j++)
		{
			k=ctx->tagw_parm[pix+j];
			stra[j]=ctx->fnidx_str[k];
		}
		
		switch(ctx->tagw_pcn[i])
		{
		case 0:
			BGBCC_CCXL_Warn2(ctx,
				lfn, lln, "(%04X) %s\n", tag,
				BGBCC_CCXL_TagGetMessage(tag));
			break;
		case 1:
			BGBCC_CCXL_Warn2(ctx,
				lfn, lln, "(%04X) %s %s\n", tag,
				BGBCC_CCXL_TagGetMessage(tag), stra[0]);
			break;
		case 2:
			BGBCC_CCXL_Warn2(ctx,
				lfn, lln, "(%04X) %s %s %s\n", tag,
				BGBCC_CCXL_TagGetMessage(tag), stra[0], stra[1]);
			break;
		default:
			BGBCC_CCXL_Warn2(ctx,
				lfn, lln, "(%04X) %s\n", tag,
				BGBCC_CCXL_TagGetMessage(tag));
			break;
		}
	}
	ctx->tagw_n_warn=0;
}

int BGBCC_CCXL_IndexFName(BGBCC_TransState *ctx, char *str)
{
	int i;
	
	for(i=0; i<ctx->fnidx_num; i++)
	{
		if(!strcmp(ctx->fnidx_str[i], str))
			return(i);
	}
	
	i=ctx->fnidx_num++;
	ctx->fnidx_str[i]=bgbcc_strdup(str);
	return(i);
}

int BGBCC_CCXL_IndexForCurLfn(BGBCC_TransState *ctx)
{
	int i;
	
	if(ctx->fnidx_str[ctx->lfni]==ctx->lfn)
		return(ctx->lfni);
	i=BGBCC_CCXL_IndexFName(ctx, ctx->lfn);
	ctx->lfni=i;
	return(i);
}

ccxl_label BGBCC_CCXL_GenSym(BGBCC_TransState *ctx)
{
//	char buf[32];
	ccxl_label l;
	int i, j;
//	int id;

	if(!(ctx->gs_srcpos))
	{
		ctx->gs_srcpos=bgbcc_malloc(1024*sizeof(int));
		ctx->gs_srcmax=1024;
	}

	if((ctx->gs_seq+1)>=(ctx->gs_srcmax))
	{
		i=ctx->gs_srcmax+((ctx->gs_srcmax)>>1);
		ctx->gs_srcpos=bgbcc_realloc(ctx->gs_srcpos, i*sizeof(int));
		ctx->gs_srcmax=i;
	}

	i=BGBCC_CCXL_IndexForCurLfn(ctx);
	j=(ctx->gs_seq++);
	ctx->gs_srcpos[j]=(i<<16)|((ctx->lln)&65535);
	
	l.id=CCXL_LBL_GENSYMBASE+j;
//	l.id=CCXL_LBL_GENSYMBASE+(ctx->gs_seq++);
	return(l);

//	sprintf(buf, "GS%d", ctx->gs_seq++);
//	return(bgbcc_strdup(buf));
}

ccxl_label BGBCC_CCXL_GenSym2(BGBCC_TransState *ctx)
{
	return(BGBCC_CCXL_GenSym(ctx));
//	return(BS1_RStrDup(BASM_GenSym()));
}

int BGBCC_CCXL_CompileSwitchJmpR(
	BGBCC_TransState *ctx,
	ccxl_label *cl, s64 *clv,
	int clm, int cln,
	ccxl_label dfl, ccxl_label dfl2)
{
	ccxl_label lclc, dflz;
	s64 cla, cla1;
	s64 ncv, df;
	int ncl, clc, clc0, clc1, clm0, cln0;
	int i, j, k;
	
	/* Stack: Cond -- */
	
	ncl=cln-clm;
	
	ncv=clv[cln-1]-clv[clm];
	clc0=(clm+cln)>>1;
	
	if(ncl>16)
	{
		cla=(clv[cln-1]+clv[clm])/2;

#if 1
		if(ncv<(1LL<<32))
		{
			cla1=0;
			for(i=0; i<ncl; i++)
				cla1+=clv[clm+i];
			cla1/=ncl;
			cla=(cla+cla1)/2;
		}
#endif
		
		clm0=clm; cln0=cln;
		clc0=(clm0+cln0)>>1;
		for(i=0; i<4; i++)
		{
			if(cla>=clv[clc0])
			{
				clm0=clc0;
				clc1=(clm0+cln0)>>1;
				if(clc1>=(cln-3))
					break;
				clc0=clc1;
			}else
			{
				cln0=clc0;
				clc1=(clm0+cln0)>>1;
				if(clc1<=(clm+3))
					break;
				clc0=clc1;
			}
		}
	}
	
	if(((ncl*2)>ncv) && (ncv>1))
	{
		df=(ncl*256)/ncv;
	}else
	{
		df=0;
	}
	
	if(((s32)clv[clm])!=clv[clm])
		df=0;
	if(((s32)clv[cln])!=clv[cln])
		df=0;
	
//	if(ncl<4)
//	if(ncl<6)
	if(ncl<8)
//	if(ncl<10)
	{
		for(i=0; i<ncl; i++)
		{
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, clv[clm+i]);
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[clm+i]);
		}

//		BGBCC_CCXL_StackPop(ctx);
		if(dfl.id)
		{
			BGBCC_CCXL_CompileJmp(ctx, dfl);
		}
		return(0);
	}else
#if 1
//		if((ncl<256) && (df>=224))
//		if((ncl<1024) && (df>=224))
		if((ncl<1024) && (df>=192))
//		if(ncl<1024)
	{
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_StackCompileJmpTab(ctx, clm, cln, cl, clv, dfl, dfl2);
		return(0);
	}else
#endif
	{
//		clc=(clm+cln)>>1;
		clc=clc0;

		lclc=BGBCC_CCXL_GenSym(ctx);
//		dflz=dfl;
		dflz=dfl2;
		if(clv[clc]==(clv[clc-1]+1))
			dflz.id=0;

//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_StackPushConstInt(ctx, clv[clc]);
		BGBCC_CCXL_CompileJmpCond(ctx, ">=", lclc);
		
		BGBCC_CCXL_CompileSwitchJmpR(ctx, cl, clv, clm, clc, dflz, dfl2);
		BGBCC_CCXL_EmitLabel(ctx, lclc);
		BGBCC_CCXL_CompileSwitchJmpR(ctx, cl, clv, clc, cln, dfl, dfl2);
		return(0);
	}
}

int BGBCC_CCXL_CompileSwitch_SortR(
	BGBCC_TransState *ctx,
	ccxl_label *cl, s64 *clv,
	int clm, int cln)
{
	s64 li, lc;
	ccxl_label l0;
	int ncl;
	int lo, hi, cn;
	int bi, bj;
	int i, j, k;

	ncl=cln-clm;

//	printf("BGBCC_CCXL_CompileSwitch_SortR ncl=%d\n", ncl);
//	for(i=0; i<ncl; i++)
//	{
//		printf("%X %d\n", (int)(cl[i].id), (int)clv[i]);
//	}

#if 1
//	if(1)
//	if(ncl<32)
//	if(ncl<16)
	if(ncl<8)
//	if(ncl<4)
	{
		for(i=0; i<ncl; i++)
			for(j=i+1; j<ncl; j++)
		{
			bi=clm+i;	bj=clm+j;

			if(clv[bj]==clv[bi])
			{
				printf("BGBCC_CCXL_CompileSwitch_SortR: "
					"Duplicate Label %d\n", (int)(clv[bi]));
			}

			if(clv[bj]<clv[bi])
			{
				li=clv[bj]; clv[bj]=clv[bi]; clv[bi]=li;
				l0=cl[bj]; cl[bj]=cl[bi]; cl[bi]=l0;
			}
		}
	}else
	{
		lo=clm; hi=cln;
		cn=(lo+hi)>>1;
		lc=clv[cn];
		while(lo<hi)
		{
			if(clv[lo]<lc)
			{
				lo++;
			}
			else
			{
				hi--;
				li=clv[lo]; clv[lo]=clv[hi]; clv[hi]=li;
				l0=cl[lo]; cl[lo]=cl[hi]; cl[hi]=l0;
			}
		}
		BGBCC_CCXL_CompileSwitch_SortR(ctx, cl, clv, clm, lo);
		BGBCC_CCXL_CompileSwitch_SortR(ctx, cl, clv, lo, cln);
	}
#endif
	return(0);
}

int BGBCC_CCXL_CompileSwitch(BGBCC_TransState *ctx, BCCX_Node *l)
{
	ccxl_label t_cl[512];
	s64 t_clv[512];
	ccxl_label t_cl2[512];
	s64 t_clv2[512];

	BGBCC_CCXL_RegisterInfo *ri;
	ccxl_label *cl, *cl2;
	s64 *clv, *clv2;
	char *s;
	ccxl_label l0, dfl, lbrk, fxdfl;
	s64 li;
	int oldlclst;
	int i, j, ncl, mcl, na, ci;
	BCCX_Node *c, *t, *u, *v, *n;

//	cl=bgbcc_malloc(4096*sizeof(ccxl_label));
//	clv=bgbcc_malloc(4096*sizeof(s64));
//	mcl=4096;

	cl=t_cl;
	clv=t_clv;
	mcl=512;
	
	cl2=NULL;
	clv2=NULL;

	lbrk=BGBCC_CCXL_GenSym(ctx);

#if 1
	dfl.id=0;
	fxdfl.id=0;
//	ncl=0; c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//	while(c)
//	{

	ncl=0;
	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);

		if((ncl+1)>=mcl)
		{
			if(cl==t_cl)
			{
				i=4096;
				cl2=bgbcc_malloc(i*sizeof(ccxl_label));
				clv2=bgbcc_malloc(i*sizeof(s64));
				memcpy(cl2, cl, mcl*sizeof(ccxl_label));
				memcpy(clv2, clv, mcl*sizeof(s64));
				
				cl=cl2;
				clv=clv2;
				mcl=i;
				continue;
			}
			
			i=mcl+(mcl>>1);
			cl=bgbcc_realloc(cl, i*sizeof(ccxl_label));
			clv=bgbcc_realloc(clv, i*sizeof(s64));
			mcl=i;
			continue;
		}
	
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case"))
		{
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");
			v=BGBCC_CCXL_ReduceExprConst2(ctx, v);
			li=-999999999;

			if(BGBCC_CCXL_IsIntP(ctx, v))
			{
				li=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
//					printf("Case Imm %d\n", (int)li);
			}else if(BGBCC_CCXL_IsCharP(ctx, v))
			{
				s=BCCX_GetCst(v, &bgbcc_rcst_value, "value");
				if(s) { li=BGBCP_ParseChar(&s); }
			}else if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
			{
				s=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
				ri=BGBCC_CCXL_LookupGlobal(ctx, s);
				if(ri && BGBCC_CCXL_IsRegImmIntP(ctx, ri->value))
				{
					li=BGBCC_CCXL_GetRegImmIntValue(ctx, ri->value);
//					printf("%s %d\n", s, (int)li);
				}else
				{
					BGBCC_CCXL_StubError(ctx);
				}
			}else
			{
				BGBCC_CCXL_StubError(ctx);
			}

			i=ncl++;
			cl[i]=BGBCC_CCXL_GenSym(ctx);
			clv[i]=li;
//			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			dfl=BGBCC_CCXL_GenSym(ctx);
//			BGBCC_CCXL_CompileJmp(ctx, cl[i++]);
//			c=BCCX_Next(c);
			continue;
		}
//		c=BCCX_Next(c);
	}

	BGBCC_CCXL_CompileExpr(ctx,
		BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"));
	
	/* small switch, use if/else */
	if(ncl<8)
//	if(1)
	{
		for(i=0; i<ncl; i++)
		{
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, clv[i]);
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[i]);
		}

		if(dfl.id)
		{
			BGBCC_CCXL_CompileJmp(ctx, dfl);
			BGBCC_CCXL_StackPop(ctx);
		}else
		{
			BGBCC_CCXL_StackPop(ctx);
			BGBCC_CCXL_CompileJmp(ctx, lbrk);
		}
		
		cl2=NULL;
		clv2=NULL;
	}else
	{
		/* larger switch, use binary dispatch */
		if(ncl<512)
		{
			cl2=t_cl2;
			clv2=t_clv2;
		}else
		{
			cl2=bgbcc_malloc(mcl*sizeof(ccxl_label));
			clv2=bgbcc_malloc(mcl*sizeof(s64));
		}

		/* generate sorted list of cases */
		for(i=0; i<ncl; i++)
			{ cl2[i]=cl[i]; clv2[i]=clv[i]; }

#if 0
		/* generate sorted list of cases */
		for(i=0; i<ncl; i++)
			{ cl2[i]=cl[i]; clv2[i]=clv[i]; }
		for(i=0; i<ncl; i++)
			for(j=i+1; j<ncl; j++)
		{
			if(clv2[j]<clv2[i])
			{
				li=clv2[j]; clv2[j]=clv2[i]; clv2[i]=li;
				l0=cl2[j]; cl2[j]=cl2[i]; cl2[i]=l0;
			}
		}
#endif

		BGBCC_CCXL_CompileSwitch_SortR(ctx, cl2, clv2, 0, ncl);
		
		if(!dfl.id)
		{
			fxdfl=BGBCC_CCXL_GenSym(ctx);
			dfl=fxdfl;
		}
		BGBCC_CCXL_CompileSwitchJmpR(ctx, cl2, clv2, 0, ncl, dfl, dfl);
	}

#endif

#if 0
	ncl=0; c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case") ||
			BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			cl[ncl++]=BGBCC_CCXL_GenSym(ctx);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_CompileExpr(ctx,
		BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"));

	i=0; c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case"))
		{
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_CompileExpr(ctx,
				BCCX_FetchCst(c, &bgbcc_rcst_value, "value"));
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			BGBCC_CCXL_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}
#endif

	if(ctx->breakstackpos>=64)
		{ BGBCC_DBGBREAK }

//	lbrk=BGBCC_CCXL_GenSym(ctx);
	ctx->breakstack[ctx->breakstackpos++]=lbrk;
	oldlclst=ctx->loop_localstate;
	ctx->loop_localstate&=~BGBCC_LOOPFL_MASK;

//	l0=BGBCC_CCXL_GenSym(ctx);
//	ctx->breakstack[ctx->breakstackpos++]=l0;
//	BGBCC_CCXL_CompileBreak(ctx);

	i=0;
//	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//	while(c)
//	{
	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case"))
		{
			/* case labels: Cond -- */
			BGBCC_CCXL_StackPushVoid(ctx);
			BGBCC_CCXL_EmitLabel(ctx, cl[i++]);
			BGBCC_CCXL_StackPop(ctx);
//			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			/* default label: Cond -- */
			BGBCC_CCXL_StackPushVoid(ctx);
			BGBCC_CCXL_EmitLabel(ctx, dfl);
			BGBCC_CCXL_StackPop(ctx);
//			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_goto_case, "goto_case"))
		{
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");
			v=BGBCC_CCXL_ReduceExprConst(ctx, v);

			li=-1;
			if(BGBCC_CCXL_IsIntP(ctx, v))
			{
				li=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
			}
			
			for(j=0; j<ncl; j++)
				if(clv[j]==li)
			{
				BGBCC_CCXL_CompileJmp(ctx, cl[j]);
				break;
			}
			
			if(j>=ncl)
			{
				BGBCC_CCXL_StubError(ctx);
			}

//			c=BCCX_Next(c);
			continue;
		}

		BGBCC_CCXL_CompileStatement(ctx, c);
//		c=BCCX_Next(c);
	}
	
	if(fxdfl.id)
	{
		BGBCC_CCXL_StackPushVoid(ctx);
		BGBCC_CCXL_EmitLabel(ctx, fxdfl);
		BGBCC_CCXL_StackPop(ctx);
	}

	BGBCC_CCXL_EmitLabel(ctx, lbrk);
//	BGBCC_CCXL_StackPop(ctx);

	if(cl!=t_cl)
	{
		bgbcc_free(cl);
		bgbcc_free(clv);
	}

	if(cl2 && (cl2!=t_cl2))
	{
		bgbcc_free(cl2);
		bgbcc_free(clv2);
	}

	ctx->loop_localstate=oldlclst;
	ctx->breakstackpos--;
	return(0);
}

int BGBCC_CCXL_CompileInitArrayMultiR(BGBCC_TransState *ctx,
	char *name, ccxl_type ty, BCCX_Node *l,
	int *cidx, int nidx)
{
	BCCX_Node *cur;
	ccxl_type bty;
	char *s0, *s1;
	int i, j, k, idx, asz, len, na, ci;

	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);
	asz=BGBCC_CCXL_TypeGetArraySize(ctx, ty);

	s1=NULL;
	if(!BGBCC_CCXL_TypeArrayP(ctx, bty))
		s1=BGBCC_CCXL_TypeGetSig(ctx, bty);

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_string, "string"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_value, "value");
		len=BGBCP_StrlenUTF8(s0);
		
		if(len>asz)
			len=asz;
			
		idx=0;

		for(i=0; i<len; i++)
		{
			k=BGBCP_ParseChar(&s0);
//			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
//			BGBCC_JX2C_BuildGlobal_EmitLitAsType(ctx, sctx,
//				tty, treg);
			BGBCC_CCXL_StackPushConstInt(ctx, k);

			BGBCC_CCXL_StackCastSig(ctx, s1);

			BGBCC_CCXL_PushLoad(ctx, name);
			for(j=0; j<nidx; j++)
				BGBCC_CCXL_StackLoadIndexConst(ctx, cidx[j]);
			BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
			idx++;
		}
		for(i=len; i<asz; i++)
		{
			BGBCC_CCXL_StackPushConstInt(ctx, 0);
			BGBCC_CCXL_StackCastSig(ctx, s1);
			BGBCC_CCXL_PushLoad(ctx, name);
			for(j=0; j<nidx; j++)
				BGBCC_CCXL_StackLoadIndexConst(ctx, cidx[j]);
			BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
			idx++;
		}

		return(0);
	}
	
//	cur=BCCX_Child(l);
	idx=0;
//	while(cur)
//	{
	na=BCCX_GetNodeChildCount(l);
	for(ci=0; ci<na; ci++)
	{
		cur=BCCX_GetNodeIndex(l, ci);
		if(BGBCC_CCXL_TypeArrayP(ctx, bty))
		{
			cidx[nidx]=idx;
			BGBCC_CCXL_CompileInitArrayMultiR(ctx,
				name, bty, cur, cidx, nidx+1);
		}else
		{
			BGBCC_CCXL_CompileExpr(ctx, cur);
			BGBCC_CCXL_StackCastSig(ctx, s1);

			BGBCC_CCXL_PushLoad(ctx, name);
			for(i=0; i<nidx; i++)
				BGBCC_CCXL_StackLoadIndexConst(ctx, cidx[i]);
			BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		}

		idx++;
//		cur=BCCX_Next(cur);
	}

	return(0);
}

int BGBCC_CCXL_CompileInitArrayMulti(BGBCC_TransState *ctx,
	char *name, char *sig, BCCX_Node *l)
{
	int cidx[16];
	BCCX_Node *cur;
	ccxl_type ty, bty;
	char *s1;
	int idx;

	BGBCC_CCXL_TypeFromSig(ctx, &ty, sig);

	BGBCC_CCXL_CompileInitArrayMultiR(ctx, name, ty, l, cidx, 0);
	return(0);

#if 0
	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);

	s1=BGBCC_CCXL_TypeGetSig(ctx, bty);
	
	cur=BCCX_Child(l); idx=0;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
		BGBCC_CCXL_StackCastSig(ctx, s1);

		BGBCC_CCXL_PushLoad(ctx, name);
		BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		idx++;

		cur=BCCX_Next(cur);
	}
#endif

	return(0);
}

int BGBCC_CCXL_CompileInitArray(BGBCC_TransState *ctx,
	char *name, char *sig, BCCX_Node *l)
{
	BCCX_Node *cur;
	ccxl_type ty, bty;
	char *s1;
	int idx, na, ci;

	name=BGBCC_CCXL_CompileRemapName(ctx, name);

	BGBCC_CCXL_TypeFromSig(ctx, &ty, sig);
	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);

	if(BGBCC_CCXL_TypeArrayP(ctx, bty))
	{
		BGBCC_CCXL_CompileInitArrayMulti(ctx, name, sig, l);
		return(false);
	}

	s1=BGBCC_CCXL_TypeGetSig(ctx, bty);
	
//	cur=BCCX_Child(l);
	idx=0;
//	while(cur)
//	{
	na=BCCX_GetNodeChildCount(l);
	for(ci=0; ci<na; ci++)
	{
		cur=BCCX_GetNodeIndex(l, ci);

#if 0
		if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
		{
//			BGBCC_CCXL_StubError(ctx);

			BGBCC_CCXL_CompileInitValueObject(
				ctx, bty, cur);
			BGBCC_CCXL_PushLoad(ctx, name);
			BGBCC_CCXL_StackStoreIndexConst(ctx, idx);

			idx++;
			cur=BCCX_Next(cur);
			continue;
		}
#endif
	
		BGBCC_CCXL_CompileExprAsTypeSig(ctx, s1, cur);
//		BGBCC_CCXL_CompileExpr(ctx, cur);
//		BGBCC_CCXL_StackCastSig(ctx, s1);

		BGBCC_CCXL_PushLoad(ctx, name);
		BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		idx++;

//		cur=BCCX_Next(cur);
	}

	return(0);
}

void BGBCC_CCXL_CompileInitVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *type, BCCX_Node *value)
{
	BGBCC_CCXL_LiteralInfo *sti;
	char *s0, *s1;
	BCCX_Node *t, *c, *nl;
	s64 li;
	int na;
	int i, j, k;

	if(!name)
		return;

	name=BGBCC_CCXL_CompileRemapName(ctx, name);

	li=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");
	if(li&BGBCC_TYFL_STATIC)
		return;

	s0=BGBCC_CCXL_VarTypeString(ctx, type);

//	nl=BCCX_FetchCst(type, &bgbcc_rcst_size, "size");
	nl=BCCX_FindTagCst(type, &bgbcc_rcst_size, "size");

	if(value)
	{
		if(*s0=='U')
		{
			sti=BGBCC_CCXL_LookupTypedefForSig2(ctx, s0);
			if(sti)
				{ s0=BGBCC_CCXL_TypeGetSig(ctx, sti->decl->type); }
		}

		if(*s0=='A')
		{
			BGBCC_CCXL_StackInitVar(ctx, name);
			BGBCC_CCXL_CompileInitArray(ctx, name, s0, value);
			return;
		}

		t=BGBCC_CCXL_ReduceExpr(ctx, value);

//		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_CompileExprAsType(ctx, type, t);
		BGBCC_CCXL_StackInitVarValue(ctx, name);
	}else
	{
		if((*s0=='Q') && nl)
		{
			na=1;
			if((s0[1]>='0') && (s0[1]<='9'))
				na=s0[1]-'0';

			s1=NULL;
			
			switch(na)
			{
			case 0:		s1=NULL;	break;
			case 1:		s1="__alloca_initvla1";	break;
			case 2:		s1="__alloca_initvla2";	break;
			case 3:		s1="__alloca_initvla3";	break;
			case 4:		s1="__alloca_initvla4";	break;
			case 5:		s1="__alloca_initvla5";	break;
			case 6:		s1="__alloca_initvla6";	break;
			case 7:		s1="__alloca_initvla7";	break;
			case 8:		s1="__alloca_initvla8";	break;
			case 9:		s1="__alloca_initvla9";	break;
			}
		
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExprListReverseB(ctx, nl);
			BGBCC_CCXL_StackPushConstString(ctx, s0+2);
			BGBCC_CCXL_StackCallName(ctx, s1, 0);

			BGBCC_CCXL_StackInitVarValue(ctx, name);
			return;
		}

		BGBCC_CCXL_StackInitVar(ctx, name);
	}
}

char *BGBCC_CCXL_FlattenDottedName(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tbuf[256];
	char *ct, *s0, *s1;
	BCCX_Node *c, *v;
	
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		s0=BGBCC_CCXL_FlattenDottedName(ctx, v);
		if(!s0)
			return(NULL);
		s1=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		sprintf(tbuf, "%s/%s", s0, s1);
		return(bgbcc_strdup(tbuf));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		return(s0);
	}
	
	return(NULL);

#if 0
	if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
		BCCX_TagIsCstP(rn, &bgbcc_rcst_objref, "objref"))
	{
		s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_CompileExpr(ctx,
			BCCX_FetchCst(rn, &bgbcc_rcst_value, "value"));
		s1=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StackLoadSlotStore(ctx, s1, s0);
		return;
	}
#endif
}

void BGBCC_CCXL_CompileStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	static int warn_rov;
	char tb[256];
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	BCCX_Node *ni, *nc, *ns, *nb;
	ccxl_label l0, l1, l2, l3;
	ccxl_type bty, dty, sty, tty, lty, rty, lty2, rty2;
	long long li, lj;
	char *s0, *s1, *s2;
	int sqn, sqon, oldlclst;
	int i0, i1, i2, i3, na, ci;
	int i, j, k;

	if(!l)
	{
		return;
	}

	s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s0 && (s0!=ctx->lfn))
	{
		if(!ctx->reqlfn)
			{ BGBCC_CCXL_StackFn(ctx, s0); }
		ctx->lfn=s0;
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if((i>0) && (i!=ctx->lln))
	{
		if(!ctx->reqlfn)
			{ BGBCC_CCXL_StackLn(ctx, i); }
		ctx->lln=i;
	}

	if(ctx->lln==12271)
	{
		i=1;
	}

	if(ctx->reqlfn)
	{
		BGBCC_CCXL_StackFn(ctx, ctx->lfn);
		BGBCC_CCXL_StackLn(ctx, ctx->lln);
		ctx->reqlfn=0;
	}

	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		l=BGBCC_CCXL_ReduceExpr(ctx, l);
	}

	if(!l)
	{
#if 0
//		BGBCC_CCXL_Warn(ctx,
//			"BGBCC_CCXL_CompileStatement: Statement Reduced Away\n");

		warn_rov++;
		if(!(warn_rov&63))
			BGBCC_CCXL_Warn(ctx, "There was a statement here, it's gone now\n");
		else
			BGBCC_CCXL_Warn(ctx, "Statement Reduced Away\n");
#endif
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_switch, "switch"))
	{
		BGBCC_CCXL_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_goto, "goto"))
	{
		BGBCC_CCXL_CompileJmp(ctx,
			BGBCC_CCXL_LabelFromName(ctx,
				BCCX_GetCst(l, &bgbcc_rcst_name, "name")));
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_label, "label"))
	{
		BGBCC_CCXL_EmitLabel(ctx,
			BGBCC_CCXL_LabelFromName(ctx,
				BCCX_GetCst(l, &bgbcc_rcst_name, "name")));
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_continue, "continue"))
	{
		if(ctx->contstackpos<=0)
		{
			BGBCC_CCXL_Error(ctx, "'continue' outside a loop\n");
			return;
		}
		ctx->loop_localstate|=BGBCC_LOOPFL_CONTINUE;
		l0=ctx->contstack[ctx->contstackpos-1];
		BGBCC_CCXL_CompileJmp(ctx, l0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_break, "break"))
	{
		if(ctx->breakstackpos<=0)
		{
			BGBCC_CCXL_Error(ctx, "'break' outside a loop or switch\n");
			return;
		}
		ctx->loop_localstate|=BGBCC_LOOPFL_BREAK;
		l0=ctx->breakstack[ctx->breakstackpos-1];
		BGBCC_CCXL_CompileJmp(ctx, l0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		if(!t)
		{
			BGBCC_CCXL_StackRetV(ctx);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackRet(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(s0)
		{
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref"))
//			if(0)
			{
				s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");

#if 1
				if(	!strcmp(s0, "+") &&
					!BGBCC_CCXL_CheckIsStaticLib(ctx) &&
					BGBCC_CCXL_IsBinaryP(ctx, rn, "*") &&
					BGBCC_CCXL_InferExprCleanP(ctx, rn))
				{
					ln2=BCCX_FetchCst(rn, &bgbcc_rcst_left, "left");
					rn2=BCCX_FetchCst(rn, &bgbcc_rcst_right, "right");
					ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
					rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
					BGBCC_CCXL_InferExpr(ctx, ln2, &lty2);
					BGBCC_CCXL_InferExpr(ctx, rn2, &rty2);

					BGBCC_CCXL_GetTypeBinaryDest(ctx,
						CCXL_BINOP_MUL, lty2, rty2, &bty);
					BGBCC_CCXL_GetTypeBinaryDest(ctx,
						CCXL_BINOP_ADD, lty, bty, &dty);

					if(	(	BGBCC_CCXL_TypeSmallIntP(ctx, dty) &&
							BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) &&
							ctx->arch_has_imac	) ||
						(	BGBCC_CCXL_TypeBaseRealP(ctx, dty) &&
							BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) &&
							ctx->arch_has_fmac	)	)
					{
						BGBCC_CCXL_CompileExpr(ctx, ln2);
						BGBCC_CCXL_CompileExpr(ctx, rn2);
						BGBCC_CCXL_CompileExpr(ctx, ln);
						BGBCC_CCXL_StackTrinaryOpStore(ctx, "MAC", s1);
						return;
					}
				}
#endif
			
//				s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
				return;
			}

			if(BGBCC_CCXL_InferExprCleanP(ctx, ln))
//			if(1)
//			if(0)
			{
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOp(ctx, s0);
				BGBCC_CCXL_CompileAssign(ctx, ln);
			}else
			{
				BGBCC_CCXL_CompileExprRef(ctx, ln);
				BGBCC_CCXL_StackDupClean(ctx);
				BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOp(ctx, s0);
//				BGBCC_CCXL_StackDupClean(ctx);
				BGBCC_CCXL_StackDupIdx(ctx, 1);
				BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
//				BGBCC_CCXL_StackExch(ctx);
				BGBCC_CCXL_StackPop(ctx);
			}
			return;
		}

		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_binary, "binary"))
		{
			s0=BCCX_GetCst(rn, &bgbcc_rcst_op, "op");
			s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");

			if(strcmp(s0, "&&") && strcmp(s0, "||"))
			{
				ln2=BCCX_FetchCst(rn, &bgbcc_rcst_left, "left");
				rn2=BCCX_FetchCst(rn, &bgbcc_rcst_right, "right");
				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
				BGBCC_CCXL_CompileExpr(ctx, ln2);
				BGBCC_CCXL_CompileExpr(ctx, rn2);
				BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
				return;
			}
		}

//		if(BCCX_TagIsCstP(rn, &bgbcc_rcst_ref, "ref"))
//		{
//			s0=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
//			if(!strcmp(s0, "cshift_empty"))
//			{
//				k=0;
//			}
//		}

		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
			s1=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");

			i=0;
			if(	(ctx->lang==BGBCC_LANG_CPP)	||
				(ctx->lang==BGBCC_LANG_CS)	||
				(ctx->lang==BGBCC_LANG_BS2)	||
				(ctx->lang==BGBCC_LANG_BS)	||
				(ctx->lang==BGBCC_LANG_JAVA)	)
			{
				if(!bgbcp_strcmp(s1, "this"))	i=1;
				if(!bgbcp_strcmp(s1, "null"))	i=1;
				if(!bgbcp_strcmp(s1, "true"))	i=1;
				if(!bgbcp_strcmp(s1, "false"))	i=1;
			}

			if((*s0=='_') || (*s1=='_'))
			{
				/* Pessimism: Something funky might be going on. */
				i=1;
			}

			if(!i)
			{
				BGBCC_CCXL_MovLoadStore(ctx, s0, s1);
				return;
			}
		}

#if 1
		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_funcall, "funcall"))
		{
			s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_CompileFuncallStore(ctx, rn, s0);
			return;
		}
#endif

#if 1
		if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_unary, "unary") &&
			!(	BCCX_AttrIsCstP(rn, &bgbcc_rcst_op, "op", "*") ||
				BCCX_AttrIsCstP(rn, &bgbcc_rcst_op, "op", "&")))
		{
			s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
			s1=BCCX_GetCst(rn, &bgbcc_rcst_op, "op");
			v=BCCX_FetchCst(rn, &bgbcc_rcst_value, "value");
			
			if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
			{
				s2=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
				if(!strcmp(s2, s0))
				{
					BGBCC_CCXL_StackUnaryOpName(ctx, s1, s0);
					return;
				}
			}
			
			BGBCC_CCXL_CompileExpr(ctx, v);
			BGBCC_CCXL_StackUnaryOpStore(ctx, s1, s0);
			return;
		}
#endif

#if 0
		if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_unary, "unary"))
		{
//			!(	BCCX_AttrIsCstP(rn, &bgbcc_rcst_op, "op", "*") ||
//				BCCX_AttrIsCstP(rn, &bgbcc_rcst_op, "op", "&")))

			s1=BCCX_GetCst(rn, &bgbcc_rcst_op, "op");
			
			if(!strcmp(s1, "*") || !strcmp(s1, "&"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				v=BCCX_FetchCst(rn, &bgbcc_rcst_value, "value");
				
				if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
				{
					s2=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
					if(!strcmp(s2, s0))
					{
						BGBCC_CCXL_StackUnaryOpName(ctx, s1, s0);
						return;
					}
				}
				
				BGBCC_CCXL_CompileExpr(ctx, v);
				BGBCC_CCXL_StackUnaryOpStore(ctx, s1, s0);
				return;
			}
		}
#endif

//		if(BGBCC_CCXL_TypeCompatibleP(ctx, lty, rty) &&
		if(BGBCC_CCXL_TypeCompatibleArchP(ctx, lty, rty) &&
			!BGBCC_CCXL_TypeVariantP(ctx, lty) &&
			!BGBCC_CCXL_TypeVariantP(ctx, rty))
//		if(0)
		{
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_objref, "objref"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_CompileExpr(ctx,
					BCCX_FetchCst(rn, &bgbcc_rcst_value, "value"));
				s1=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_StackLoadSlotStore(ctx, s1, s0);
				return;
			}

#if 1
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_getindex, "getindex"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				ln2=BCCX_FetchCst(rn, &bgbcc_rcst_array, "array");
				rn2=BCCX_FetchCst(rn, &bgbcc_rcst_index, "index");
//				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);

				if(BGBCC_CCXL_IsIntP(ctx, rn2))
				{
					i=BCCX_GetIntCst(rn2, &bgbcc_rcst_value, "value");
					BGBCC_CCXL_CompileExpr(ctx, ln2);
					BGBCC_CCXL_StackLoadIndexConstStore(ctx, i, s0);
				}else
				{
					BGBCC_CCXL_CompileExpr(ctx, ln2);
					BGBCC_CCXL_CompileExpr(ctx, rn2);
					BGBCC_CCXL_StackLoadIndexStore(ctx, s0);
				}
				return;
			}
#endif

			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_cast, "cast"))
			{
				t=BCCX_FetchCst(rn, &bgbcc_rcst_value, "value");
				if(BCCX_TagIsCstP(t, &bgbcc_rcst_list, "list"))
				{
					BGBCC_CCXL_CompileExpr(ctx, rn);
					BGBCC_CCXL_CompileAssign(ctx, ln);
					return;
				}
				
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_CompileExpr(ctx, t);
				s1=BGBCC_CCXL_VarTypeString(ctx,
					BCCX_FindTagCst(rn, &bgbcc_rcst_type, "type"));
				BGBCC_CCXL_StackCastSigStore(ctx, s1, s0);
				return;
			}

			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_int, "int"))
			{
				s0=BCCX_GetCst(rn, &bgbcc_rcst_tysuf, "tysuf");
				li=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");

				s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				s1=BGBCC_CCXL_CompileRemapName(ctx, s1);

				if(!s0)
				{
//					BGBCC_CCXL_StackPushConstInt(ctx, li);
//					BGBCC_CCXL_PopStore(ctx, s1);
					BGBCC_CCXL_ConstIntStore(ctx, li, s1);
				}else
				{
					BGBCC_CCXL_CompileExpr(ctx, rn);
					BGBCC_CCXL_PopStore(ctx, s1);
				}

//				BGBCC_CCXL_CompileExpr(ctx, rn);
//				BGBCC_CCXL_CompileAssign(ctx, ln);
				return;
			}

			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_ref, "ref"))
			{
				s0=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");	
				s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
//				BGBCC_CCXL_PushLoad(ctx, s0);

				s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				s1=BGBCC_CCXL_CompileRemapName(ctx, s1);
//				BGBCC_CCXL_PopStore(ctx, s1);

				if((*s1!='_') && (*s0!='_'))
				{
					BGBCC_CCXL_MovLoadStore(ctx, s1, s0);
	//				BGBCC_CCXL_RefRefStore(ctx, s0, s1);
					return;
				}

				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_CompileAssign(ctx, ln);
				return;
			}
		}

		BGBCC_CCXL_CompileExpr(ctx, rn);
		BGBCC_CCXL_CompileAssign(ctx, ln);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_preinc, "preinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpName(ctx, "++", s0);
			return;
		}

		if(BGBCC_CCXL_InferExprCleanP(ctx, t))
//		if(0)
		{
			BGBCC_CCXL_CompileExpr(ctx, t);
			BGBCC_CCXL_StackUnaryOp(ctx, "++");
			BGBCC_CCXL_CompileAssign(ctx, t);
		}else
		{
			BGBCC_CCXL_CompileExprRef(ctx, t);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			BGBCC_CCXL_StackUnaryOp(ctx, "++");
			BGBCC_CCXL_StackDupIdx(ctx, 1);
			BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
			BGBCC_CCXL_StackPop(ctx);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_predec, "predec") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpName(ctx, "--", s0);
			return;
		}

		if(BGBCC_CCXL_InferExprCleanP(ctx, t))
//		if(0)
		{
			BGBCC_CCXL_CompileExpr(ctx, t);
			BGBCC_CCXL_StackUnaryOp(ctx, "--");
			BGBCC_CCXL_CompileAssign(ctx, t);
		}else
		{
			BGBCC_CCXL_CompileExprRef(ctx, t);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			BGBCC_CCXL_StackUnaryOp(ctx, "--");
			BGBCC_CCXL_StackDupIdx(ctx, 1);
			BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
			BGBCC_CCXL_StackPop(ctx);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))	{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
//				c=BCCX_Next(c);
				continue;
			}

			s0=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

			BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
//			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_var_init, "var_init"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		if(ctx->lln==12271)
		{
			i=1;
		}
	
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
		
		if(i==1)
		{
			BGBCC_CCXL_CompileStatement(ctx, ln);
			return;
		}
		if(i==0)
		{
			if(rn)BGBCC_CCXL_CompileStatement(ctx, rn);
			return;
		}

		if(ctx->arch_has_predops &&
			!BGBCC_CCXL_CheckIsStaticLib(ctx))
		{
			if(ctx->curprd)
				{ BGBCC_DBGBREAK }
		
			i=BGBCC_CCXL_InferBlockPredSafeP(ctx, ln);
			if(i && rn)
			{
				j=BGBCC_CCXL_InferBlockPredSafeP(ctx, rn);
				if(j<=0)i=0;
			}
			
			if(i)
			{
				j=BGBCC_CCXL_InferExprSimpleCmpP(ctx, t);
				if(j<=0)i=0;
			}
			
			if(i)
			{
				BGBCC_CCXL_CompilePredExpr(ctx, t);

				BGBCC_CCXL_StackSetPred(ctx, 2);
				BGBCC_CCXL_CompileStatement(ctx, ln);

				if(rn)
				{
					BGBCC_CCXL_StackSetPred(ctx, 3);
					BGBCC_CCXL_CompileStatement(ctx, rn);
				}

				BGBCC_CCXL_StackSetPred(ctx, 0);
				return;
			}
		}

		l0=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileJCF(ctx, t, l0);

		BGBCC_CCXL_CompileStatement(ctx, ln);
		if(rn)
		{
			l1=BGBCC_CCXL_GenSym(ctx);
			BGBCC_CCXL_CompileJmp(ctx, l1);
			BGBCC_CCXL_EmitLabel(ctx, l0);
			BGBCC_CCXL_CompileStatement(ctx, rn);
			l0=l1;
		}
		BGBCC_CCXL_EmitLabel(ctx, l0);
		return;
	}

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ifarch, "ifarch"))
	{
		if(ctx->lln==12271)
		{
			i=1;
		}
	
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
//		t=BGBCC_CCXL_ReduceExpr(ctx, t);
		t=BGBCC_CCXL_ReduceExprIfArch(ctx, t);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

#if 0
		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");

			sprintf(tb, "__arch_ifarch__%s", s0);
			t=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
			BCCX_SetCst(t, &bgbcc_rcst_name, "name", tb);
		}
#endif

#if 1
		if(i==1)
		{
			BGBCC_CCXL_CompileStatement(ctx, ln);
			return;
		}
		if(i==0)
		{
			if(rn)BGBCC_CCXL_CompileStatement(ctx, rn);
			return;
		}
#endif

		l0=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileJCF(ctx, t, l0);

		BGBCC_CCXL_CompileStatement(ctx, ln);
		if(rn)
		{
			l1=BGBCC_CCXL_GenSym(ctx);
			BGBCC_CCXL_CompileJmp(ctx, l1);
			BGBCC_CCXL_EmitLabel(ctx, l0);
			BGBCC_CCXL_CompileStatement(ctx, rn);
			l0=l1;
		}
		BGBCC_CCXL_EmitLabel(ctx, l0);
		return;
	}
#endif

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		sqon=ctx->vlcl_curseq;
//		sqn=ctx->vlcl_seq++;
		sqn=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");

		if(ctx->vlcl_stackpos>=256)
			{ BGBCC_DBGBREAK }

		ctx->vlcl_curseq=sqn;
		ctx->vlcl_stack[ctx->vlcl_stackpos++]=sqn;
		
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			BGBCC_CCXL_CompileStatement(ctx, c);
//			c=BCCX_Next(c);
		}

		ctx->vlcl_stackpos--;
		ctx->vlcl_curseq=sqon;

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for"))
	{
		ni=BCCX_FetchCst(l, &bgbcc_rcst_init, "init");
		nc=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		nb=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		ns=BCCX_FetchCst(l, &bgbcc_rcst_step, "step");

#if 1
		i=1;
		i=i&&BGBCC_CCXL_IsFixIntAssignRVP(ctx, ni, &s0, &i0);
		i=i&&BGBCC_CCXL_IsFixIntCompareRVP(ctx, nc, &s1, &s2, &i1);
		i=i&&(	BGBCC_CCXL_IsTagVarRVP(ctx, ns, "postinc", s0) ||
				BGBCC_CCXL_IsTagVarRVP(ctx, ns, "preinc", s0));
		i=i&&(!strcmp(s0, s1));
		i=i&&(!strcmp(s2, "<") || !strcmp(s2, "<="));
//		i=i&&(	BCCX_TagIsCstP(nb, &bgbcc_rcst_assign, "assign") ||
//				BCCX_TagIsCstP(nb, &bgbcc_rcst_funcall, "funcall") ||
//				BCCX_TagIsCstP(nb, &bgbcc_rcst_setindex, "setindex"));
//		i=i&&(	BCCX_TagIsCstP(nb, &bgbcc_rcst_assign, "assign") ||
//				BCCX_TagIsCstP(nb, &bgbcc_rcst_funcall, "funcall"));

		ctx->infer_var_loopconst=s0;
		i=i&&BGBCC_CCXL_InferBlockPredSafeP(ctx, nb);
		ctx->infer_var_loopconst=NULL;
		
//		if(i && ((i1-i0)<=8))
		if(i && ((i1-i0)<=16))
		{
			if(!strcmp(s2, "<="))i1++;
			for(i=i0; i<i1; i++)
			{
				BGBCC_CCXL_StackPushConstInt(ctx, i);
				BGBCC_CCXL_PopStore(ctx, s0);
				BGBCC_CCXL_CompileStatement(ctx, nb);
			}

			return;
		}
#endif

		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);

		if(ctx->contstackpos>=64)
			{ BGBCC_DBGBREAK }
		if(ctx->breakstackpos>=64)
			{ BGBCC_DBGBREAK }

		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;
		oldlclst=ctx->loop_localstate;
		ctx->loop_localstate&=~BGBCC_LOOPFL_MASK;
		
//		ctx->loop_localstate|=BGBCC_LOOPFL_CONTINUE;

		if(ni)
			{ BGBCC_CCXL_CompileStatement(ctx, ni); }

		if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
			(ctx->optmode==BGBCC_OPT_SPEED2))
		{
			if(nc)
				{ BGBCC_CCXL_CompileJCF(ctx, nc, l2); }

			BGBCC_CCXL_EmitLabelLvl(ctx, l0, ctx->contstackpos);

			BGBCC_CCXL_CompileStatement(ctx, nb);

			k=ctx->loop_localstate;
			if(k&BGBCC_LOOPFL_CONTINUE)
				BGBCC_CCXL_EmitLabelLvl(ctx, l1, ctx->contstackpos);

			if(ns)
				{ BGBCC_CCXL_CompileStatement(ctx, ns); }

			if(nc)
				{ BGBCC_CCXL_CompileJCT(ctx, nc, l0); }
			else
				BGBCC_CCXL_CompileJmp(ctx, l0);

			BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);
		}else
		{
			BGBCC_CCXL_EmitLabelLvl(ctx, l0, ctx->contstackpos);

			if(nc)
				{ BGBCC_CCXL_CompileJCF(ctx, nc, l2); }

			BGBCC_CCXL_CompileStatement(ctx, nb);

			k=ctx->loop_localstate;
			if(k&BGBCC_LOOPFL_CONTINUE)
				BGBCC_CCXL_EmitLabelLvl(ctx, l1, ctx->contstackpos);

			if(ns)
				{ BGBCC_CCXL_CompileStatement(ctx, ns); }

			BGBCC_CCXL_CompileJmp(ctx, l0);
			BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);
		}

		ctx->loop_localstate=oldlclst;
		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		l3=BGBCC_CCXL_GenSym(ctx);

//		ctx->contstack[ctx->contstackpos++]=l1;
//		ctx->breakstack[ctx->breakstackpos++]=l2;
//		BGBCC_CCXL_EmitLabel(ctx, l1);

		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			if(ctx->contstackpos>=64)
				{ BGBCC_DBGBREAK }
			if(ctx->breakstackpos>=64)
				{ BGBCC_DBGBREAK }

			ctx->contstack[ctx->contstackpos++]=l1;
			ctx->breakstack[ctx->breakstackpos++]=l2;
			oldlclst=ctx->loop_localstate;
			ctx->loop_localstate&=~BGBCC_LOOPFL_MASK;

			BGBCC_CCXL_EmitLabelLvl(ctx, l1, ctx->contstackpos);
			BGBCC_CCXL_CompileStatement(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_body, "body"));
			BGBCC_CCXL_CompileJmp(ctx, l1);
			BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);

			ctx->loop_localstate=oldlclst;
			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
			(ctx->optmode==BGBCC_OPT_SPEED2))
		{
			if(ctx->contstackpos>=64)
				{ BGBCC_DBGBREAK }
			if(ctx->breakstackpos>=64)
				{ BGBCC_DBGBREAK }

			ctx->contstack[ctx->contstackpos++]=l3;
			ctx->breakstack[ctx->breakstackpos++]=l2;
			oldlclst=ctx->loop_localstate;
			ctx->loop_localstate&=~BGBCC_LOOPFL_MASK;

			ctx->loop_localstate|=BGBCC_LOOPFL_CONTINUE;

//			BGBCC_CCXL_CompileJCF(ctx, t, l2);
			BGBCC_CCXL_CompileJmp(ctx, l3);

			BGBCC_CCXL_EmitLabelLvl(ctx, l1, ctx->contstackpos);

			BGBCC_CCXL_CompileStatement(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_body, "body"));
			k=ctx->loop_localstate;

//			if(k&BGBCC_LOOPFL_CONTINUE)
				BGBCC_CCXL_EmitLabelLvl(ctx, l3, ctx->contstackpos);

			BGBCC_CCXL_CompileJCT(ctx, t, l1);

			BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);

			ctx->loop_localstate=oldlclst;
			ctx->contstackpos--;
			ctx->breakstackpos--;
		}else
		{
			if(ctx->contstackpos>=64)
				{ BGBCC_DBGBREAK }
			if(ctx->breakstackpos>=64)
				{ BGBCC_DBGBREAK }

			ctx->contstack[ctx->contstackpos++]=l1;
			ctx->breakstack[ctx->breakstackpos++]=l2;
			oldlclst=ctx->loop_localstate;
			ctx->loop_localstate&=~BGBCC_LOOPFL_MASK;

//			ctx->loop_localstate|=BGBCC_LOOPFL_CONTINUE;

			BGBCC_CCXL_EmitLabelLvl(ctx, l1, ctx->contstackpos);
			BGBCC_CCXL_CompileJCF(ctx, t, l2);
			BGBCC_CCXL_CompileStatement(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_body, "body"));
			BGBCC_CCXL_CompileJmp(ctx, l1);
			BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);

			ctx->loop_localstate=oldlclst;
			ctx->contstackpos--;
			ctx->breakstackpos--;
		}

		return;
	}


	if(BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while"))
	{
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		l3=BGBCC_CCXL_GenSym(ctx);

		if(ctx->contstackpos>=64)
			{ BGBCC_DBGBREAK }
		if(ctx->breakstackpos>=64)
			{ BGBCC_DBGBREAK }

		ctx->contstack[ctx->contstackpos++]=l3;
		ctx->breakstack[ctx->breakstackpos++]=l2;
		oldlclst=ctx->loop_localstate;
		ctx->loop_localstate&=~BGBCC_LOOPFL_MASK;

//		ctx->loop_localstate|=BGBCC_LOOPFL_CONTINUE;

		BGBCC_CCXL_EmitLabelLvl(ctx, l1, ctx->contstackpos);
		BGBCC_CCXL_CompileStatement(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"));

		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);

		if(i==0)
		{
			if(ctx->loop_localstate&BGBCC_LOOPFL_CONTINUE)
				BGBCC_CCXL_EmitLabelLvl(ctx, l3, ctx->contstackpos);
			if(ctx->loop_localstate&BGBCC_LOOPFL_BREAK)
				BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);

			ctx->loop_localstate=oldlclst;
			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		if(i==1)
		{
			if(ctx->loop_localstate&BGBCC_LOOPFL_CONTINUE)
				BGBCC_CCXL_EmitLabelLvl(ctx, l3, ctx->contstackpos);
//			BGBCC_CCXL_CompileContinue(ctx);
			BGBCC_CCXL_CompileJmp(ctx, l1);
			if(ctx->loop_localstate&BGBCC_LOOPFL_BREAK)
				BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);

			ctx->loop_localstate=oldlclst;
			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		if(ctx->loop_localstate&BGBCC_LOOPFL_CONTINUE)
			BGBCC_CCXL_EmitLabelLvl(ctx, l3, ctx->contstackpos);
		BGBCC_CCXL_CompileJCT(ctx, t, l1);
		if(ctx->loop_localstate&BGBCC_LOOPFL_BREAK)
			BGBCC_CCXL_EmitLabelLvl(ctx, l2, ctx->contstackpos-1);

		ctx->loop_localstate=oldlclst;
		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		BGBCC_CCXL_CompileFuncallStmt(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin"))
	{
		BGBCC_CCXL_CompileFuncallStmt(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_throw, "throw"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		if(!t)
		{
			BGBCC_CCXL_StubError(ctx);
//			BGBCC_CCXL_StackRetV(ctx);
			return;
		}

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_throw", 1);
//		BGBCC_CCXL_StackCallName2(ctx, s0, dname, flag);

//		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_try_catch, "try_catch"))
	{
		//lame: for now just spit out try block...
		t=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		BGBCC_CCXL_CompileStatement(ctx, t);

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_msvc_asm, "msvc_asm"))
	{
		c=BCCX_Child(l);
		s0=BCCX_Text(c);

//		BGBCC_CCXL_AddAsmBlob(ctx, s0);
		BGBCC_CCXL_InlineAsmBlob(ctx, s0);
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_gcc_asm, "gcc_asm"))
	{
		c=BCCX_Child(l);
		s0=BCCX_Text(c);

//		BGBCC_CCXL_AddAsmBlob(ctx, s0);
		BGBCC_CCXL_InlineAsmBlob(ctx, s0);
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		BGBCC_CCXL_CompileExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		BGBCC_CCXL_StackPop(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_asm_blob, "asm_blob"))
	{
		c=BCCX_Child(l);
		s0=BCCX_Text(c);

//		BGBCC_CCXL_AddAsmBlob(ctx, s0);
		BGBCC_CCXL_InlineAsmBlob(ctx, s0);
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_linecomment, "linecomment")) { return; }
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_blockcomment, "blockcomment")) { return; }

	BGBCC_CCXL_CompileExpr(ctx, l);
	BGBCC_CCXL_StackPop(ctx);
}


char *BGBCC_CCXL_VarTypeString_FlattenExpr(BGBCC_TransState *ctx,
	char *dst, BCCX_Node *l, int fl)
{
	BCCX_Node *c, *n;
	char *s, *t;
	int na, ci;
	
	t=dst;
	
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BGBCC_CCXL_VarTypeString_FlattenName(ctx, dst, s, fl, 0);
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_emptyindex, "emptyindex"))
	{
		*t++='Q';
		n=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sharpcall, "sharpcall"))
	{
		*t++='<';
//		c=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, c, fl);
//			c=BCCX_Next(c);
		}
		*t++='>';
		n=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_anytype, "anytype"))
	{
		*t++='C';
		*t++='z';
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(!strcmp(s, "..."))
		{
			*t++='C';
			*t++='z';
			return(t);
		}

		if(!strcmp(s, "*"))*t++='P';
		if(!strcmp(s, "&"))*t++='R';
		if(!strcmp(s, "^"))*t++='P';

		n=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}
	
	return(t);
}

BCCX_Node *BGBCC_CCXL_LookupStructureNodeForNameI(BGBCC_TransState *ctx,
	char *name)
{
	BCCX_Node *c, *n;
	char *s;
	int i, j, k, ci, hi;

#if 0
	c=ctx->structs;
	while(c)
	{
		if(BCCX_AttrIsCstP(c, &bgbcc_rcst_name, "name", name))
		{
			return(c);
		}
//		c=BCCX_Next(c);
	}
#endif

//	s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
//	i=BGBCC_CCXL_HashName(s)&255;
//	c->hnext=ctx->struct_hash[i];

#ifdef BGBCC_BCCX2

	hi=BGBCC_CCXL_HashName(name)&255;
	ci=ctx->struct_hash_ix[hi];
	while(ci>=0)
	{
		c=BCCX_GetNodeIndex(ctx->structs, ci);
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
		ci=BCCX_GetIntCst(c, &bgbcc_rcst_Shnext, "$hnext");
	}

#else
	i=BGBCC_CCXL_HashName(name)&255;
	c=ctx->struct_hash[i];
	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(!strcmp(s, name))
			return(c);
		c=c->hnext;
	}
#endif

	return(NULL);
}

BCCX_Node *BGBCC_CCXL_LookupStructureNodeForName(BGBCC_TransState *ctx,
	char *name)
{
	char tb[256];
	BCCX_Node *c, *n;
	char *s;
	int i;
	
	n=NULL;

	for(i=(ctx->n_imp-1); i>=0; i--)
	{
		sprintf(tb, "%s/%s", ctx->imp_ns[i], name);
		c=BGBCC_CCXL_LookupStructureNodeForNameI(ctx, tb);
		if(c)	{ n=c; break; }
	}

	if(!n)
		{ n=BGBCC_CCXL_LookupStructureNodeForNameI(ctx, name); }
	return(n);
}

char *BGBCC_CCXL_StrPrintInt(char *t, s32 val)
{
	return(BGBCC_StrPrintInt(t, val));
}

char *BGBCC_CCXL_VarTypeString_FlattenName(BGBCC_TransState *ctx,
	char *t, char *s, int fl, int vsz)
{
	BCCX_Node *c, *n;
	BGBCC_CCXL_LiteralInfo *sti;
	char *s1, *t1;
	int lang;
	int i, j, k;

	t1=t;

	if(ctx)lang=ctx->lang;
		else lang=BGBCC_LANG_C;

	if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
	{
		if(fl&BGBCC_TYFL_UNSIGNED)
		{
			if(!strcmp(s, "void"))*t++='v';
			if(!strcmp(s, "char"))*t++='h';
			if(!strcmp(s, "short"))*t++='t';
			if(ctx->arch_sizeof_int==2)
			{
				if(!strcmp(s, "int"))*t++='t';
				if(!strcmp(s, "long"))*t++='j';
			}else
			{
				if(!strcmp(s, "int"))*t++='j';
				if(!strcmp(s, "long"))*t++='m';
			}
//			if(!strcmp(s, "int"))				
//				*t++=(ctx->arch_sizeof_int==2)?'t':'j';
//				*t++='j';
//			if(!strcmp(s, "long"))*t++='m';
//			if(!strcmp(s, "llong"))*t++='y';
			if(!strcmp(s, "long_long"))*t++='y';
			if(!strcmp(s, "int128"))*t++='o';

			if(!strcmp(s, "float"))
				{ *t++='C'; *t++='f'; }
			if(!strcmp(s, "double"))
				{ *t++='C'; *t++='d'; }
			if(!strcmp(s, "float128"))
				{ *t++='C'; *t++='g'; }
			if(!strcmp(s, "float16"))
				{ *t++='C'; *t++='k'; }

			if(!strcmp(s, "long_double"))
				{ *t++='C'; *t++='e'; }
		}else
			if(fl&BGBCC_TYFL_SIGNED)
		{
			if(!strcmp(s, "float"))
				{ *t++='G'; *t++='f'; }
			if(!strcmp(s, "double"))
				{ *t++='G'; *t++='d'; }
			if(!strcmp(s, "float128"))
				{ *t++='G'; *t++='g'; }
			if(!strcmp(s, "float16"))
				{ *t++='G'; *t++='k'; }

			if(!strcmp(s, "long_double"))
				{ *t++='G'; *t++='e'; }
		}else
			if(fl&BGBCC_TYFL_PACKED)
		{
			if(!strcmp(s, "m128"))
				{ *t++='C'; *t++='p'; }
			if(!strcmp(s, "m128p"))
				{ *t++='C'; *t++='p'; }
		}
	}

	if(t==t1)
	{
		if(!strcmp(s, "void"))*t++='v';
		if(!strcmp(s, "short"))*t++='s';

		if(ctx->arch_sizeof_int!=2)
		{
			if(!strcmp(s, "int"))*t++='i';
		}

//		if(!strcmp(s, "int"))
//			*t++=(ctx->arch_sizeof_int==2)?'s':'i';
//			*t++='i';

		if(!strcmp(s, "float"))
		{
			*t++='f';
		}
		if(!strcmp(s, "double"))
		{
			*t++='d';
		}

		if(!strcmp(s, "void_ctor"))*t++='v';
	}
	
	if(t==t1)
	{
		if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
		{
			if(!strcmp(s, "char"))*t++='c';

			if(ctx->arch_sizeof_int==2)
			{
				if(!strcmp(s, "int"))*t++='s';
				if(!strcmp(s, "long"))*t++='i';
			}else
			{
				if(!strcmp(s, "int"))*t++='i';
				if(!strcmp(s, "long"))*t++='l';
			}

//			if(!strcmp(s, "long"))*t++='l';
//			if(!strcmp(s, "llong"))*t++='x';
			if(!strcmp(s, "long_long"))*t++='x';
			if(!strcmp(s, "bool"))*t++='b';

			if(!strcmp(s, "object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "string"))
				{ *t++='C'; *t++='s'; }

		}else if(
			(lang==BGBCC_LANG_BS2) ||
			(lang==BGBCC_LANG_BS)	)
		{
			if(!strcmp(s, "char"))*t++='w';
			if(!strcmp(s, "object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "string"))
				{ *t++='C'; *t++='s'; }

			if(!strcmp(s, "cstring"))
				{ *t++='P'; *t++='c'; }

			if(!strcmp(s, "wstring"))
				{ *t++='D'; *t++='s'; }
			if(!strcmp(s, "ustring"))
				{ *t++='D'; *t++='t'; }

			if(!strcmp(s, "bool"))*t++='b';

			if(!strcmp(s, "byte"))*t++='h';
			if(!strcmp(s, "sbyte"))*t++='c';
			if(!strcmp(s, "ubyte"))*t++='h';

			if(!strcmp(s, "ushort"))*t++='t';
			if(!strcmp(s, "uint"))*t++='j';

			if(!strcmp(s, "long"))*t++='x';
			if(!strcmp(s, "ulong"))*t++='y';

//			if(!strcmp(s, "llong"))*t++='x';
//			if(!strcmp(s, "ullong"))*t++='y';
			if(!strcmp(s, "long_long"))*t++='x';
			if(!strcmp(s, "ullong_long"))*t++='y';

			if(!strcmp(s, "hfloat"))*t++='k';
			if(!strcmp(s, "decimal"))*t++='g';

			if(!strcmp(s, "variant"))*t++='r';
			if(!strcmp(s, "var"))*t++='r';

			if(!strcmp(s, "number"))*t++='d';

		}else if(lang==BGBCC_LANG_CS)
		{
			if(!strcmp(s, "char"))*t++='w';
			if(!strcmp(s, "object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "string"))
				{ *t++='C'; *t++='s'; }

			if(!strcmp(s, "bool"))*t++='b';

			if(!strcmp(s, "byte"))*t++='h';
			if(!strcmp(s, "sbyte"))*t++='c';
			if(!strcmp(s, "ubyte"))*t++='h';

			if(!strcmp(s, "ushort"))*t++='t';
			if(!strcmp(s, "uint"))*t++='j';

			if(!strcmp(s, "long"))*t++='x';
			if(!strcmp(s, "ulong"))*t++='y';

			if(!strcmp(s, "hfloat"))*t++='k';

			if(!strcmp(s, "decimal"))*t++='g';
		}else if(lang==BGBCC_LANG_JAVA)
		{
			if(!strcmp(s, "char"))*t++='w';
			if(!strcmp(s, "long"))*t++='l';

			if(!strcmp(s, "byte"))*t++='c';
			if(!strcmp(s, "boolean"))*t++='b';

			if(!strcmp(s, "Object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "String"))
				{ *t++='C'; *t++='t'; }
		}else
		{
			if(!strcmp(s, "char"))*t++='c';
			if(!strcmp(s, "long"))*t++='l';
//			if(!strcmp(s, "llong"))*t++='x';
			if(!strcmp(s, "long_long"))*t++='x';
		}
	}

	if(t==t1)
	{
		if(!strcmp(s, "_Bool"))*t++='b';

		if(!strcmp(s, "int8"))*t++='a';
		if(!strcmp(s, "uint8"))*t++='h';
		if(!strcmp(s, "int16"))*t++='s';
		if(!strcmp(s, "uint16"))*t++='t';
		if(!strcmp(s, "int32"))*t++='i';
		if(!strcmp(s, "uint32"))*t++='j';

		if(!strcmp(s, "int64"))*t++='x';
		if(!strcmp(s, "uint64"))*t++='y';

		if(!strcmp(s, "int128"))*t++='n';
		if(!strcmp(s, "uint128"))*t++='o';

//		if(!strcmp(s, "intptr"))*t++='l';
//		if(!strcmp(s, "uintptr"))*t++='m';

		if(!strcmp(s, "intptr"))*t++='p';
		if(!strcmp(s, "uintptr"))*t++='q';

		if(!strcmp(s, "ptrdiff"))*t++='p';

		if(!strcmp(s, "ssize_t"))*t++='l';
		if(!strcmp(s, "usize_t"))*t++='m';

		if(!strcmp(s, "bfloat16"))*t++='u';
		if(!strcmp(s, "float16"))*t++='k';
		if(!strcmp(s, "float32"))*t++='f';
		if(!strcmp(s, "float64"))*t++='d';
		if(!strcmp(s, "float128"))*t++='g';
		if(!strcmp(s, "long_double"))*t++='g';

		if(!strcmp(s, "float8s"))
			{ *t++='G'; *t++='a'; }
		if(!strcmp(s, "float8u"))
			{ *t++='G'; *t++='h'; }

		if(!strcmp(s, "vec4fp8s"))
			{ *t++='D'; *t++='v'; }
		if(!strcmp(s, "vec4fp8u"))
			{ *t++='D'; *t++='w'; }

		if(!strcmp(s, "auto"))*t++='r';
		if(!strcmp(s, "var"))*t++='r';
		if(!strcmp(s, "variant"))*t++='r';
		if(!strcmp(s, "variantf"))
			{ *t++='C'; *t++='r'; }

		if(!strcmp(s, "variant_this"))
			{ *t++='C'; *t++='x'; }
		if(!strcmp(s, "variant_cls"))
			{ *t++='C'; *t++='y'; }
		if(!strcmp(s, "variant_va"))
			{ *t++='C'; *t++='z'; }

		if(!strcmp(s, "builtin_va_list") ||
				!strcmp(s, "gnuc_va_list"))
			{ *t++='D'; *t++='z'; }

		if(!strcmp(s, "fcomplex"))
			{ *t++='C'; *t++='f'; }
		if(!strcmp(s, "dcomplex"))
			{ *t++='C'; *t++='d'; }
		if(!strcmp(s, "hcomplex"))
			{ *t++='C'; *t++='k'; }

		if(!strcmp(s, "vec2"))
			{ *t++='C'; *t++='a'; }
		if(!strcmp(s, "vec3"))
			{ *t++='C'; *t++='b'; }
		if(!strcmp(s, "vec4"))
			{ *t++='C'; *t++='c'; }
		if(!strcmp(s, "quat"))
			{ *t++='C'; *t++='q'; }

		if(!strcmp(s, "vec2f"))
			{ *t++='C'; *t++='a'; }
		if(!strcmp(s, "vec3f"))
			{ *t++='C'; *t++='b'; }
		if(!strcmp(s, "vec4f"))
			{ *t++='C'; *t++='c'; }
		if(!strcmp(s, "quatf"))
			{ *t++='C'; *t++='q'; }

		if(!strcmp(s, "m64"))
			{ *t++='C'; *t++='m'; }
		if(!strcmp(s, "m128"))
			{ *t++='C'; *t++='n'; }
		if(!strcmp(s, "m128p"))
			{ *t++='C'; *t++='p'; }

		if(!strcmp(s, "vec4w"))
			{ *t++='C'; *t++='w'; }
		if(!strcmp(s, "vec4i"))
			{ *t++='C'; *t++='j'; }
		if(!strcmp(s, "vec4sw"))
			{ *t++='C'; *t++='v'; }
		if(!strcmp(s, "vec4si"))
			{ *t++='C'; *t++='i'; }
		if(!strcmp(s, "vec4uw"))
			{ *t++='C'; *t++='w'; }
		if(!strcmp(s, "vec4ui"))
			{ *t++='C'; *t++='j'; }

		if(!strcmp(s, "vec4h"))
			{ *t++='C'; *t++='l'; }
		if(!strcmp(s, "vec4sf"))
			{ *t++='C'; *t++='l'; }

		if(!strcmp(s, "vec2si"))
			{ *t++='D'; *t++='i'; }
		if(!strcmp(s, "vec2ui"))
			{ *t++='D'; *t++='j'; }

		if(!strcmp(s, "vec2d"))
			{ *t++='C'; *t++='h'; }

		if(!strcmp(s, "vec3d"))
			{ *t++='D'; *t++='b'; }
		if(!strcmp(s, "vec4d"))
			{ *t++='D'; *t++='c'; }
		if(!strcmp(s, "quatd"))
			{ *t++='D'; *t++='q'; }

		if(!strcmp(s, "bcd64"))
			{ *t++='D'; *t++='m'; }
		if(!strcmp(s, "bcd128"))
			{ *t++='D'; *t++='o'; }

		if(!strcmp(s, "vec3fx") || !strcmp(s, "vec3xf"))
			{ *t++='D'; *t++='h'; }
		if(!strcmp(s, "vec3fq"))
			{ *t++='D'; *t++='l'; }

		if(!strcmp(s, "vec3h") || !strcmp(s, "vec3sf"))
			{ *t++='D'; *t++='k'; }

		if(!strcmp(s, "bigint"))
			{ *t++='G'; *t++='w'; }
		if(!strcmp(s, "fixnum"))
			{ *t++='G'; *t++='x'; }
		if(!strcmp(s, "flonum"))
			{ *t++='G'; *t++='y'; }

		if(!strcmp(s, "number"))
			{ *t++='d'; }

		if(!strncmp(s, "bitint_", 7))
		{
			k=atoi(s+7);
			k/=128;
			if(fl&BGBCC_TYFL_UNSIGNED)
				{ *t++='D'; *t++='y'; }
			else
				{ *t++='D'; *t++='x'; }

			if(k>=100)
			{
				*t++='0'+(k/100);
				*t++='0'+((k/10)%10);
				*t++='0'+(k%10);
			}else
				if(k>=10)
			{
				*t++='0'+(k/10);
				*t++='0'+(k%10);
			}else
			{
				*t++='0'+k;
			}
		}

		*t=0;
	}

	if((vsz>0) && (t!=t1))
	{
		if(*t1=='f')
		{
			if(vsz&1)
				vsz=(vsz>>1)*4;
			t=t1;
			switch(vsz)
			{
				case 4:		*t++='f';				break;
				case 8:		*t++='C';	*t++='a';	break;
				case 12:	*t++='C';	*t++='b';	break;
				case 16:	*t++='C';	*t++='c';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/4);
					*t++='f';
					break;
			}
		}else if(*t1=='d')
		{
			if(vsz&1)
				vsz=(vsz>>1)*8;
			t=t1;
			switch(vsz)
			{
				case 8:		*t++='d';				break;
				case 16:	*t++='C';	*t++='h';	break;
				case 24:	*t++='D';	*t++='b';	break;
				case 32:	*t++='D';	*t++='c';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/8);
					*t++='d';
					break;
			}
		}else if(*t1=='i')
		{
			if(vsz&1)
				vsz=(vsz>>1)*4;
			t=t1;
			switch(vsz)
			{
				case 4:		*t++='i';				break;
				case 8:		*t++='D';	*t++='i';	break;
				case 16:	*t++='C';	*t++='i';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/4);
					*t++='i';
					break;
			}
		}else if(*t1=='j')
		{
			if(vsz&1)
				vsz=(vsz>>1)*4;
			t=t1;
			switch(vsz)
			{
				case 4:		*t++='j';				break;
				case 8:		*t++='D';	*t++='j';	break;
				case 16:	*t++='C';	*t++='j';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/4);
					*t++='j';
					break;
			}
		}else if(*t1=='k')
		{
			if(vsz&1)
				vsz=(vsz>>1)*2;
			t=t1;
			switch(vsz)
			{
				case 2:		*t++='k';				break;
//				case 4:		*t++='C';	*t++='l';	break;
				case 8:		*t++='C';	*t++='l';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/2);
					*t++='k';
					break;
			}
		}else if(*t1=='s')
		{
			if(vsz&1)
				vsz=(vsz>>1)*2;
			t=t1;
			switch(vsz)
			{
				case 2:		*t++='s';				break;
//				case 4:		*t++='C';	*t++='v';	break;
				case 8:		*t++='C';	*t++='v';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/2);
					*t++='s';
					break;
			}
		}else if((*t1=='t') || (*t1=='w'))
		{
			if(vsz&1)
				vsz=(vsz>>1)*2;
			t=t1;
			switch(vsz)
			{
				case 2:		*t++='t';				break;
//				case 4:		*t++='C';	*t++='w';	break;
				case 8:		*t++='C';	*t++='w';	break;
				default:
					*t++='D';
					t=BGBCC_CCXL_StrPrintInt(t, vsz/2);
					*t++='t';
					break;
			}
		}else if((*t1=='a') || (*t1=='c'))
		{
			if(vsz&1)
				vsz=(vsz>>1)*1;
			t=t1;
			*t++='D';
			t=BGBCC_CCXL_StrPrintInt(t, vsz);
			*t++='a';
		}else if(*t1=='h')
		{
			if(vsz&1)
				vsz=(vsz>>1)*1;
			t=t1;
			*t++='D';
			t=BGBCC_CCXL_StrPrintInt(t, vsz);
			*t++='h';
		}
	}

	if(ctx && (t==t1))
	{
#if 0
		c=ctx->structs;
		while(c)
		{
			if(BCCX_AttrIsCstP(c, &bgbcc_rcst_name, "name", s))
			{
				if(BCCX_TagIsCstP(c, &bgbcc_rcst_enum, "enum"))
				{
					*t++='i';
					*t=0;
					break;
				}

				if((ctx->ctxflags&BGBCC_CTXFL_SAVENAMES) ||
					(fl&BGBCC_TYFL_EXPNAME) || ctx->ril_ip)
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}

//					sprintf(t, "X%s;", s);
//					t+=strlen(t);

					s1=s;
					*t++='X';
					while(*s1)
						*t++=*s1++;
					*t++=';';
					*t=0;
				}else
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}
					if(i>0)
					{
//						sprintf(t, "X%d", i);
//						t+=strlen(t);
						*t++='X';
						t=BGBCC_CCXL_StrPrintInt(t, i);
						break;
					}

//					sprintf(t, "X%s;", s);
//					t+=strlen(t);

					s1=s;
					*t++='X';
					while(*s1)
						*t++=*s1++;
					*t++=';';
					*t=0;
				}
				break;
			}
			c=BCCX_Next(c);
		}
#endif

#if 1
		c=NULL;
		if(	(ctx->lang==BGBCC_LANG_C) ||
			(ctx->lang==BGBCC_LANG_CPP))
		{
			c=BGBCC_CCXL_LookupStructureNodeForName(ctx, s);
		}
		if(c)
		{
			if(BCCX_TagIsCstP(c, &bgbcc_rcst_enum, "enum"))
			{
				*t++='i';
				*t=0;
			}else
			{
				if((ctx->ctxflags&BGBCC_CTXFL_SAVENAMES) ||
					(fl&BGBCC_TYFL_EXPNAME) || ctx->ril_ip)
				{
					s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");

					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}

//					sprintf(t, "X%s;", s);
//					t+=strlen(t);

					s1=s;
					*t++='X';
					while(*s1)
						*t++=*s1++;
					*t++=';';
					*t=0;
				}else
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}
					if(i>0)
					{
//						sprintf(t, "X%d", i);
//						t+=strlen(t);

						*t++='X';
						t=BGBCC_CCXL_StrPrintInt(t, i);
					}else
					{
//						sprintf(t, "X%s;", s);
//						t+=strlen(t);

						s1=s;
						*t++='X';
						while(*s1)
							*t++=*s1++;
						*t++=';';
						*t=0;
					}
				}
			}
		}else if(	(ctx->lang!=BGBCC_LANG_C) &&
					(ctx->lang!=BGBCC_LANG_CPP))
		{
			sti=BGBCC_CCXL_LookupStructureRns(ctx, s);
			if(sti)
			{
				if(sti->littype==CCXL_LITID_ENUMDEF)
				{
					*t++='i';
					*t=0;
				}else
					if((ctx->ctxflags&BGBCC_CTXFL_SAVENAMES) ||
						(fl&BGBCC_TYFL_EXPNAME) || ctx->ril_ip)
				{
					s1=sti->name;
					*t++='X';
					while(*s1)
						*t++=*s1++;
					*t++=';';
					*t=0;
				}else
				{
					*t++='X';
					t=BGBCC_CCXL_StrPrintInt(t, sti->litid);
				}
			}
		}
#endif
	}

	if(t==t1)
	{
#if 0
		sprintf(t, "U%s;", s);
		sti=BGBCC_CCXL_LookupTypedefForSig2(ctx, t);
//		if(sti->decl->sig && *sti->decl->sig)
//			return(sti->decl->sig);
		s1=BGBCC_CCXL_TypeGetSig(ctx, sti->decl->type);
		if(s1 && *s1)
		{
			strcpy(t, s1);
			t+=strlen(t);
			return(t);
		}
#endif
//		sprintf(t, "U%s;", s);
//		t+=strlen(t);

		if(lang==BGBCC_LANG_BS)
		{
			*t++='r';
			*t=0;
		}else
			if(	(lang==BGBCC_LANG_CS) ||
				(lang==BGBCC_LANG_BS2))
		{
//			*t++='r';
			*t++='C';
			*t++='o';
			*t=0;
		}else
		{
			s1=s;
			*t++='U';
			while(*s1)
				*t++=*s1++;
			*t++=';';
			*t=0;
		}
	}
		
	return(t);
}

int BGBCC_CCXL_VarTypeString_ModifierChar(BGBCC_TransState *ctx, s64 i)
{
	int c;

	if(!i)return(0);

	if(ctx->arch==BGBCC_ARCH_X86)
	{
		switch(i)
		{
		case BGBCC_TYFL_CDECL: c='c'; break;
		case BGBCC_TYFL_STDCALL: c='w'; break;
		default: c=0; break;
		}
		if(c)return(c);
	}
	if(ctx->arch==BGBCC_ARCH_X64)
	{
		switch(i)
		{
		case BGBCC_TYFL_WIN64: c='w'; break;
		case BGBCC_TYFL_AMD64: c='f'; break;
		default: c=0; break;
		}
		if(c)return(c);
	}

	switch(i)
	{
	case BGBCC_TYFL_ABSTRACT:		c='a'; break;
	case BGBCC_TYFL_BIGENDIAN:		c='b'; break;
	case BGBCC_TYFL_CDECL:			c='c'; break;
	case BGBCC_TYFL_DELEGATE:		c='d'; break;
//	case BGBCC_TYFL_EVENT: c='e'; break;
	case BGBCC_TYFL_EXTERN:			c='e'; break;
	//f (fastcall/amd64)
//	case BGBCC_TYFL_GETTER: c='g'; break;
//	case BGBCC_TYFL_SETTER: c='h'; break;
	case BGBCC_TYFL_INLINE:			c='i'; break;
	case BGBCC_TYFL_FINAL:			c='j'; break;
	case BGBCC_TYFL_CONST:			c='k'; break;
	case BGBCC_TYFL_LTLENDIAN:		c='l'; break;
	case BGBCC_TYFL_PACKED:			c='m'; break;
	case BGBCC_TYFL_NATIVE:			c='n'; break;
	case BGBCC_TYFL_OVERRIDE:		c='o'; break;
	case BGBCC_TYFL_PUBLIC:			c='p'; break;
	case BGBCC_TYFL_PRIVATE:		c='q'; break;
	case BGBCC_TYFL_PROTECTED:		c='r'; break;
	case BGBCC_TYFL_STATIC:			c='s'; break;
	//t (thiscall)
	//u (special)
	case BGBCC_TYFL_VIRTUAL:		c='v'; break;
	case BGBCC_TYFL_STDCALL:		c='w'; break;
	case BGBCC_TYFL_XCALL:			c='x'; break;
	case BGBCC_TYFL_TRANSIENT:		c='y'; break;
	//z
//	case BGBCC_TYFL_VOLATILE: c='z'; break;

//	case BGBCC_TYFL_TRANSIENT: c=('C'<<8)|'t'; break;

//	case BGBCC_TYFL_ASYNC:			c=('C'<<8)|'a'; break;
	case BGBCC_TYFL_BYREF:			c=('C'<<8)|'b'; break;

	case BGBCC_TYFL_EVENT:			c=('C'<<8)|'e'; break;
	case BGBCC_TYFL_INTERRUPT:		c=('C'<<8)|'f'; break;
	case BGBCC_TYFL_SYSCALL:		c=('C'<<8)|'g'; break;

	case BGBCC_TYFL_INTERFACE:		c=('C'<<8)|'i'; break;

	case BGBCC_TYFL_RESTRICT:		c=('C'<<8)|'r'; break;
	case BGBCC_TYFL_SYNCHRONIZED:	c=('C'<<8)|'s'; break;
	case BGBCC_TYFL_MAYALIAS:		c=('C'<<8)|'a'; break;

	case BGBCC_TYFL_VOLATILE:		c=('C'<<8)|'v'; break;

	case BGBCC_TYFL_NEAR:			c=('C'<<8)|'x'; break;
	case BGBCC_TYFL_FAR:			c=('C'<<8)|'y'; break;

	case BGBCC_TYFL_IFARCH:			c=('D'<<8)|'a'; break;
	case BGBCC_TYFL_IFNARCH:		c=('D'<<8)|'b'; break;
	case BGBCC_TYFL_NOCULL:			c=('D'<<8)|'c'; break;
	case BGBCC_TYFL_FENVACC:		c=('D'<<8)|'f'; break;

	case BGBCC_TYFL_DLLEXPORT:		c=('D'<<8)|'e'; break;
	case BGBCC_TYFL_DLLIMPORT:		c=('D'<<8)|'i'; break;

	case BGBCC_TYFL_THREAD:			c=('D'<<8)|'t'; break;
	default: c=0; break;
	}
	return(c);
}

char *BGBCC_CCXL_VarTypeString_FlattenModifiers(BGBCC_TransState *ctx,
	char *t, s64 fl)
{
	int i, j, k;

	k=0;
//	for(i=0; i<30; i++)
	for(i=0; i<60; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)k++;
	}
	
	if(k>2)
	{
		*t++='M';
//		for(i=0; i<31; i++)
		for(i=0; i<60; i++)
		{
			j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
			if(j)
			{
				if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
				if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
				if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
				if(j&0xFF)*t++=j&0xFF;
			}
		}
		*t++=';';
		
		return(t);
	}

//	for(i=0; i<31; i++)
	for(i=0; i<60; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			*t++='F';
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_CCXL_VarTypeString_FlattenModifiers2(BGBCC_TransState *ctx,
	char *t, s64 fl)
{
	int i, j, k;

//	for(i=0; i<31; i++)
	for(i=0; i<60; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_CCXL_VarTypeString(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char buf[256];
	int asz[16];
	int nasz, nqlvl, isvla;
	char *s, *t, *t1, *tyn;
	BCCX_Node *c, *n, *n0;
	s64 li;
	int ind, vsz, vsc, atsz;
	int i, j, k, na, ci;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		ty=BCCX_FetchCst(ty, &bgbcc_rcst_type, "type");
	if(!ty)
		{ return(bgbcc_strdup("v")); }

	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		{ return(bgbcc_strdup("v")); }

	vsz=BGBCC_CCXL_GetNodeAttributeInt(ctx, ty, "vector_size");
	if(vsz!=0)
	{
		/* vector_size, in bytes */
		if(vsz<0)vsz=0;
		if(vsz&1)vsz++;  //pad to even
	}else
	{
		vsz=BGBCC_CCXL_GetNodeAttributeInt(ctx, ty, "vector");
		if(vsz!=0)
		{
			/* vector_size, in elements. */
			if(vsz<0)vsz=0;
			if(vsz>0)
				{ vsz=(vsz<<1)|1; }
		}
	}

	if(ty)
	{
#if 0
		nasz=0;
	
		c=BCCX_Child(ty);
		while(c)
		{
			if(BCCX_TagIsCstP(c, &bgbcc_rcst_arrayq, "arrayq"))
			{
				*t++='Q';
			}
			c=BCCX_Next(c);
		}
#endif

#if 0
		c=BCCX_FetchCst(ty, &bgbcc_rcst_size, "size");
		if(c)
		{
			*t++='A';
			while(c)
			{
				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
				{
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_if, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
					BGBCC_DBGBREAK
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
				if(i<0)
				{
					BGBCC_CCXL_Warn(ctx,
						"Negative Size Array\n");
					*t++='1'; c=BCCX_Next(c); continue;
				}
				
//				if(ctx->cur_func && ctx->cur_func->name &&
//					!strcmp(ctx->cur_func->name, "D_PolysetDraw"))
//						{ k=-1; BGBCC_DBGBREAK }
				
//				sprintf(t, "%d", i);
//				t+=strlen(t);
				t=BGBCC_CCXL_StrPrintInt(t, i);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
			*t++=';';
		}
#endif

		ind=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");

		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		tyn=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		vsc=64;
		if(tyn)
		{
			if(!strcmp(tyn, "char"))	vsc=1;
			if(!strcmp(tyn, "short"))	vsc=2;
			if(!strcmp(tyn, "int"))		vsc=4;
			if(!strcmp(tyn, "long"))	vsc=8;
			if(!strcmp(tyn, "llong"))	vsc=8;
			if(!strcmp(tyn, "int128"))	vsc=16;

			if(!strcmp(tyn, "float"))	vsc=4;
			if(!strcmp(tyn, "double"))	vsc=8;

			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			if(i)
				vsc=8;
		}

		if(ind!=0)
		{
			if(li&BGBCC_TYFL_HUGE)
				{ *t++='A'; *t++='h'; }
			else if(li&BGBCC_TYFL_NEAR)
				{ *t++='A'; *t++='n'; }
			else if(li&BGBCC_TYFL_FAR)
				{ *t++='A'; *t++='f'; }

			if(li&BGBCC_TYFL_LTLENDIAN)
				{ *t++='A'; *t++='l'; }
			else if(li&BGBCC_TYFL_BIGENDIAN)
				{ *t++='A'; *t++='b'; }
			else if(li&BGBCC_TYFL_PACKED)
				{ *t++='A'; *t++='p'; }

			if(li&BGBCC_TYFL_MAYALIAS)
				{ *t++='A'; *t++='a'; }
			else if(li&BGBCC_TYFL_RESTRICT)
				{ *t++='A'; *t++='r'; }
			else if(li&BGBCC_TYFL_VOLATILE)
				{ *t++='A'; *t++='v'; }
		}

#if 1
		nasz=0;		nqlvl=0;
		isvla=0;
	
//		c=BCCX_Child(ty);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(ty);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(ty, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_arrayq, "arrayq"))
			{
				n=BCCX_Child(c);
				if(n)
				{
					n=BGBCC_CCXL_ReduceExprConst(ctx, n);
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
					{
						i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
						if(i>0)
						{
							asz[nasz++]=i;
//							c=BCCX_Next(c);
							continue;
						}
					}
				}
				nqlvl++;
			}
//			c=BCCX_Next(c);
		}

		atsz=1;
//		c=BCCX_FetchCst(ty, &bgbcc_rcst_size, "size");
		n0=BCCX_FindTagCst(ty, &bgbcc_rcst_size, "size");
//		if(c)
		if(n0)
		{
//			while(c)
//			{
			na=BCCX_GetNodeChildCount(n0);
			for(ci=0; ci<na; ci++)
			{
				c=BCCX_GetNodeIndex(n0, ci);

				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
				{
#if 0
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_if, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						asz[nasz++]=1;
//						c=BCCX_Next(c);
						continue;
					}

					asz[nasz++]=0;
					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
//					c=BCCX_Next(c);
					continue;
#endif

#if 1
//					nqlvl++;
					isvla=1;
					asz[nasz++]=0;
//					c=BCCX_Next(c);
					continue;
#endif
				}

				i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
				if(i<0)
				{
					BGBCC_CCXL_Warn(ctx, "Negative Size Array\n");
					asz[nasz++]=1;
//					c=BCCX_Next(c);
					continue;
				}
				
				asz[nasz++]=i;
				
				if(i>0)
					atsz*=i;

//				c=BCCX_Next(c);
			}
		}
		
		if((atsz*vsc)>8192)
		{
			if(li&BGBCC_TYFL_LOCAL)
			{
				BGBCC_CCXL_Warn(ctx, "Large Local Array, szest=%d\n",
					atsz*vsc);
				if((nasz==1) && (vsc<32))
					isvla=1;
			}
		}

		for(i=0; i<nqlvl; i++)
			*t++='Q';

		if(nasz)
		{
			if(isvla)
			{
				*t++='Q';
				t=BGBCC_CCXL_StrPrintInt(t, nasz);

#if 0
				if(nasz>1)
				{
					*t++='Q';
					t=BGBCC_CCXL_StrPrintInt(t, nasz);
				}else
				{
					*t++='Q';
				}
#endif
			}else
			{
				*t++='A';
				for(j=0; j<nasz; j++)
				{
					i=asz[j];
					t=BGBCC_CCXL_StrPrintInt(t, i);
					if((j+1)<nasz)
						*t++=',';
				}
				*t++=';';
			}
		}
#endif

		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		if(i&BGBCC_TYFL_WIDE)
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='W';
		}else
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='P';
		}

		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

		n=BCCX_FetchCst(ty, &bgbcc_rcst_expr, "expr");
		if(n)
		{
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(
				ctx, t, n, i|BGBCC_TYFL_EXPNAME);
		}else
		{
			if(s)
			{
				t=BGBCC_CCXL_VarTypeString_FlattenName(
					ctx, t, s, i|BGBCC_TYFL_EXPNAME, vsz);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_CCXL_VarImageTypeString(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char buf[256];
	int asz[16];
	int nasz, nqlvl, isvla;
	char *s, *t, *t1, *tyn;
	BCCX_Node *c, *n, *n0;
	s64 li;

	int i, j, vsz, na, ci, atsz, vsc;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		ty=BCCX_FetchCst(ty, &bgbcc_rcst_type, "type");
	if(!ty)
		{ return(bgbcc_strdup("v")); }

	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		{ return(bgbcc_strdup("v")); }

	vsz=BGBCC_CCXL_GetNodeAttributeInt(ctx, ty, "vector_size");

	if(ty)
	{
#if 0
		c=BCCX_Child(ty);
		while(c)
		{
			if(BCCX_TagIsCstP(c, &bgbcc_rcst_arrayq, "arrayq"))
			{
				*t++='Q';
			}
			c=BCCX_Next(c);
		}
#endif

#if 0
		c=BCCX_FetchCst(ty, &bgbcc_rcst_size, "size");
		if(c)
		{
			t1=t;

			*t++='A';
			while(c)
			{
				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
				{
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_if, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					if(BCCX_TagIsCstP(n, &bgbcc_rcst_sizeof, "sizeof"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Sizeof array-size Hack\n");
//						*t++='1024';
						*t++='1';	*t++='0';
						*t++='2';	*t++='4';
						c=BCCX_Next(c); continue;
					}

					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
//				sprintf(t, "%d", i);
//				t+=strlen(t);
				t=BGBCC_CCXL_StrPrintInt(t, i);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
			
//			if(((t-t1)==2) && t1[1]=='0')
//			{
//				t-=2;
//				*t++='P';
//			}
		}
#endif

		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		tyn=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");
		
		vsc=64;
		if(tyn)
		{
			if(!strcmp(tyn, "char"))	vsc=1;
			if(!strcmp(tyn, "short"))	vsc=2;
			if(!strcmp(tyn, "int"))		vsc=4;
			if(!strcmp(tyn, "long"))	vsc=8;
			if(!strcmp(tyn, "llong"))	vsc=8;
			if(!strcmp(tyn, "int128"))	vsc=16;

			if(!strcmp(tyn, "float"))	vsc=4;
			if(!strcmp(tyn, "double"))	vsc=8;


			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			if(i)
				vsc=8;
		}

#if 1
		nasz=0;		nqlvl=0;
		isvla=0;
	
//		c=BCCX_Child(ty);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(ty);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(ty, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_arrayq, "arrayq"))
			{
				n=BCCX_Child(c);
				if(n)
				{
					n=BGBCC_CCXL_ReduceExprConst(ctx, n);
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
					{
						i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
						if(i>0)
						{
							asz[nasz++]=i;
//							c=BCCX_Next(c);
							continue;
						}
					}
				}
				nqlvl++;
			}
//			c=BCCX_Next(c);
		}

		atsz=1;
//		c=BCCX_FetchCst(ty, &bgbcc_rcst_size, "size");
		n0=BCCX_FindTagCst(ty, &bgbcc_rcst_size, "size");
//		if(c)
		if(n0)
		{
//			while(c)
//			{
			na=BCCX_GetNodeChildCount(n0);
			for(ci=0; ci<na; ci++)
			{
				c=BCCX_GetNodeIndex(n0, ci);

				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
				{
#if 0
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_if, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						asz[nasz++]=1;
//						c=BCCX_Next(c);
						continue;
					}

					asz[nasz++]=0;
					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
//					c=BCCX_Next(c);
					continue;
#endif

#if 1
					isvla=1;
					asz[nasz++]=0;
//					c=BCCX_Next(c);
					continue;
#endif
				}

				i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
				if(i<0)
				{
					BGBCC_CCXL_Warn(ctx, "Negative Size Array\n");
					asz[nasz++]=1;
//					c=BCCX_Next(c);
					continue;
				}
				
				asz[nasz++]=i;
				if(i>0)
					atsz*=i;

//				c=BCCX_Next(c);
			}
		}

		for(i=0; i<nqlvl; i++)
			*t++='Q';

		if((atsz*vsc)>8192)
		{
			if(li&BGBCC_TYFL_LOCAL)
			{
				BGBCC_CCXL_Warn(ctx, "Large Local Array, szest=%d\n",
					atsz*vsc);
				if((nasz==1) && (vsc<32))
					isvla=1;
			}
		}
		

		if(isvla)
		{
			*t++='Q';
			*t++='0'+nasz;
		}else
			if(nasz)
		{
			*t++='A';
			for(j=0; j<nasz; j++)
			{
				i=asz[j];
				t=BGBCC_CCXL_StrPrintInt(t, i);
				if((j+1)<nasz)
					*t++=',';
			}
			*t++=';';
		}
#endif

		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		if(li&BGBCC_TYFL_WIDE)
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='W';
		}else
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='P';
		}

		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

		n=BCCX_FetchCst(ty, &bgbcc_rcst_expr, "expr");
		if(n)
		{
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, li);
		}else
		{
			if(s)
			{
				t=BGBCC_CCXL_VarTypeString_FlattenName(ctx, t, s, li, vsz);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_CCXL_VarTypeStringNoA0(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char tb[256];
	char *sig;
	
	sig=BGBCC_CCXL_VarTypeString(ctx, ty);
	if(!strncmp(sig, "A0;", 3))
	{
		tb[0]='P';
		strcpy(tb+1, sig+3);
		return(bgbcc_strdup(tb));
	}
	if(!strncmp(sig, "A0", 2))
	{
		tb[0]='P';
		strcpy(tb+1, sig+2);
		return(bgbcc_strdup(tb));
	}
	
	return(sig);
}

char *BGBCC_CCXL_VarImageTypeStringNoA0(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char tb[256];
	char *sig;
	
	sig=BGBCC_CCXL_VarImageTypeString(ctx, ty);
	if(!strncmp(sig, "A0;", 3))
	{
		tb[0]='P';
		strcpy(tb+1, sig+3);
		return(bgbcc_strdup(tb));
	}
	if(!strncmp(sig, "A0", 2))
	{
		tb[0]='P';
		strcpy(tb+1, sig+2);
		return(bgbcc_strdup(tb));
	}
	
	return(sig);
}

char *BGBCC_CCXL_VarTypeFlagsString(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	s64 li;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		ty=BCCX_FetchCst(ty, &bgbcc_rcst_type, "type");
	if(!ty)return(bgbcc_strdup(""));

	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		return(bgbcc_strdup(""));

	if(ty)
	{
		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		if(ctx->ccxl_isdef_methodproto)
		{
			if(li&BGBCC_TYFL_STATIC)
			{
				/* 'static' methods may never be 'virtual' */
				li&=~BGBCC_TYFL_VIRTUAL;
				li|=BGBCC_TYFL_FINAL;
			}else
			{
				/*
					C++ and C# default to 'final'
					BS2 and others default to 'virtual'
				*/

				if((ctx->lang==BGBCC_LANG_CS) ||
					(ctx->lang==BGBCC_LANG_CPP))
				{
					if(	!(li&BGBCC_TYFL_VIRTUAL) &&
						!(li&BGBCC_TYFL_OVERRIDE))
							li|=BGBCC_TYFL_FINAL;
				}else
				{
					if(!(li&BGBCC_TYFL_FINAL))
						li|=BGBCC_TYFL_VIRTUAL;
				}
			}
		}

		t=BGBCC_CCXL_VarTypeString_FlattenModifiers2(ctx, t, li);
	}
	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_CCXL_VarMangleName(BGBCC_TransState *ctx,
	char *name, char *sig, BCCX_Node *ty)
{
	char tb[256];
	char *t;
	s64 li;
	int i;

	if(ctx->lang==BGBCC_LANG_C)
		return(name);

	li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	
	if(li&BGBCC_TYFL_NATIVE)
	{
		return(name);
	}

	if(!BGBCC_CCXL_NameIsQName(ctx, name) && (*sig=='('))
	{
		if(	(ctx->lang==BGBCC_LANG_BS)	||
			(ctx->lang==BGBCC_LANG_BS2)	)
				return(name);
	}

//	sprintf(tb, "%s%s", name, sig);
	
	t=BGBCC_StrPrintRawStr(tb, name);
	BGBCC_StrPrintRawStr(t, sig);

	return(bgbcc_strdup(tb));
}

void BGBCC_CCXL_EmitVarFunc(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *args)
{
	BCCX_Node *t, *t1;
	char *s, *s1, *s2, *s3, *ifa;
	s64 li;
	int i;

//	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	s=BGBCC_CCXL_VarSigProto(ctx, ty, name, args);
	s1=name;
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";
	
//	s1=BGBCC_CCXL_VarMangleName(ctx, s1, s, ty);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(ctx->cur_struct)
	{
		BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_THISNAME, ctx->cur_struct);
	}

	li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	
//	if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "dllexport") ||
//		!strcmp(s1, "main") || !strcmp(s1, "WinMain"))
	if(!strcmp(s1, "main") || !strcmp(s1, "WinMain"))
		{ li|=BGBCC_TYFL_EXPNAME|BGBCC_TYFL_DLLEXPORT; }


	t=BCCX_FindTagCst(ty, &bgbcc_rcst_declspec, "declspec");
	t1=BCCX_FindTagCst(ty, &bgbcc_rcst_attribute, "attribute");

	ifa=NULL;

	if(t || t1)
	{
		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "dllexport"))
			{ li|=BGBCC_TYFL_EXPNAME|BGBCC_TYFL_DLLEXPORT; }

		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "dllimport"))
			{ li|=BGBCC_TYFL_DLLIMPORT; }
		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "interrupt"))
			{ li|=BGBCC_TYFL_INTERRUPT; }
		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "syscall"))
			{ li|=BGBCC_TYFL_SYSCALL; }
		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "nocull"))
			{ li|=BGBCC_TYFL_NOCULL; }

		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "may_alias"))
			{ li|=BGBCC_TYFL_MAYALIAS; }
		if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "__may_alias__"))
			{ li|=BGBCC_TYFL_MAYALIAS; }

		s3=BGBCC_CCXL_GetNodeAttributeStringOrRef(ctx, ty, "ifarch");
		if(s3)
		{
			li|=BGBCC_TYFL_IFARCH;
			ifa=s3;
		}

		s3=BGBCC_CCXL_GetNodeAttributeStringOrRef(ctx, ty, "ifnarch");
		if(s3)
		{
			li|=BGBCC_TYFL_IFNARCH;
			ifa=s3;
		}
	}
	
	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);
	
	if(ifa)
	{
		BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_IFARCH, ifa);
	}
}

void BGBCC_CCXL_EmitVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	s64 li;
	int i;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s1=name;

	if(ty)
	{
		s=BGBCC_CCXL_VarImageTypeString(ctx, ty);
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	}else
	{
		s="r";
		s2="";
		li=0;
	}

	if(!s)s="v";

	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);

	if(v)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_EmitVarValueR2(ctx, v);
		BGBCC_CCXL_End(ctx);
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_EmitVarArg(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	s64 li;
	int i;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s1=name;

	if(ty)
	{
		s=BGBCC_CCXL_VarImageTypeStringNoA0(ctx, ty);
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	}else
	{
		s="r";
		s2="";
		li=0;
	}

	if(!s)s="v";

	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);

	if(v)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_EmitVarValueR2(ctx, v);
		BGBCC_CCXL_End(ctx);
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_EmitVar2(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char tb[256];
	char *s, *s1, *s2;
	s64 li;
	int i, op;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	if(ty)
	{
		s=BGBCC_CCXL_VarImageTypeString(ctx, ty);
		li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	}else
	{
		s="r";
		li=0;
	}

	s1=name;

	if(!s)s="v";

	if(!s1)s1="_";

	if(li&BGBCC_TYFL_EXTERN)
	{
		if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
			CCXL_CMD_GLOBALVARDECL, name))
		{
			return;
		}
//		return;
	}

//	if(ctx->vlcl_curseq && !(li&BGBCC_TYFL_STATIC))
	if(ctx->vlcl_curseq &&
		!(li&BGBCC_TYFL_DYNAMIC) &&
		!(li&BGBCC_TYFL_EXTERN))
//	if(ctx->vlcl_curseq)
	{
		sprintf(tb, "%s!%d", s1, ctx->vlcl_curseq);
		s1=bgbcc_strdup(tb);
	}

	op=CCXL_CMD_VARDECL;
	if(li&BGBCC_TYFL_STATIC)
		op=CCXL_CMD_STATICVARDECL;
	if(li&BGBCC_TYFL_EXTERN)
		op=CCXL_CMD_GLOBALVARDECL;

#if 0
	if(ctx->cur_struct && (*s=='('))
	{
		if(li&BGBCC_TYFL_STATIC)
			return;

		op=CCXL_CMD_METHODPROTO;
	}
#endif

//	li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

	BGBCC_CCXL_BeginName(ctx, op, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);

	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);

	if(ctx->cur_structdef &&
		BCCX_TagIsCstP(ctx->cur_structdef,
			&bgbcc_rcst_enum, "enum"))
	{
		if(v)
		{
			ctx->cur_idx4=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
		}

		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
			ctx->cur_idx4++);
		BGBCC_CCXL_End(ctx);
	}else
	{
		if(v)
		{
			BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
			BGBCC_CCXL_EmitVarValueR2(ctx, v);
			BGBCC_CCXL_End(ctx);
		}
	}

	BGBCC_CCXL_End(ctx);

	if(ctx->cur_struct)
	{
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"field", ctx->cur_idx++, s1);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s1, s);
//		BGBCC_CCXL_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindFieldInfo(ctx, ctx->cur_struct, s1, "flagstr", s2);
	}
}

void BGBCC_CCXL_EmitVar3(BGBCC_TransState *ctx, char *name, char *ty)
{
	char *s, *s1;
	int i;

	if(BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=ty;
	s1=name;

	if(!s)s="v";
	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_End(ctx);

	if(ctx->cur_struct)
	{
//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"method", ctx->cur_idx2++, s1);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s1, s);
	}
}

void BGBCC_CCXL_EmitVarProperty(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *n1, *v, *t;
	char *s, *s1, *s2;
	int i;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");

	n=BCCX_New1("var", t);
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", "value");

	n1=BCCX_New("type");
	BCCX_SetCst(n1, &bgbcc_rcst_name, "name", "void");

	v=BCCX_FindTagCst(l, &bgbcc_rcst_get, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);
//		s1=BGBCC_CCXL_GenProtoSig(ctx, t, NULL);
//		BGBCC_CCXL_EmitVar3(ctx, s2, s1);
		BGBCC_CCXL_CompileMethodProto(ctx, t, s2, NULL);
	}

	v=BCCX_FindTagCst(l, &bgbcc_rcst_set, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);
//		s1=BGBCC_CCXL_GenProtoSig(ctx, NULL, n);
//		BGBCC_CCXL_EmitVar3(ctx, s2, s1);
//		v=BCCX_FetchCst(v, &bgbcc_rcst_body, "body");
		BGBCC_CCXL_CompileMethodProto(ctx, n1, s2, n);
	}

	if(ctx->cur_struct)
	{
//		s1=BGBCC_CCXL_VarTypeString(ctx, t);
//		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);
//		i=BCCX_GetIntCst(t, &bgbcc_rcst_flags, "flags");

//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"property", ctx->cur_idx3++, s);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s, s1);
//		BGBCC_CCXL_BindFieldInfoI(ctx, ctx->cur_struct, s, "flags", i);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindFieldInfo(ctx, ctx->cur_struct, s, "flagstr", s2);
	}
}

void BGBCC_CCXL_CompileVarProperty(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *n1, *v, *t;
	char *s, *s1, *s2;
	int i;

#if 1
	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");

	n=BCCX_New1("var", t);
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", "value");

	n1=BCCX_New("type");
	BCCX_SetCst(n1, &bgbcc_rcst_name, "name", "void");

	v=BCCX_FindTagCst(l, &bgbcc_rcst_get, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_FetchCst(v, &bgbcc_rcst_body, "body");
		BGBCC_CCXL_CompileBlock(ctx, t, s2, NULL, v);
	}

	v=BCCX_FindTagCst(l, &bgbcc_rcst_set, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_FetchCst(v, &bgbcc_rcst_body, "body");
		BGBCC_CCXL_CompileBlock(ctx, n1, s2, n, v);
	}
#endif

}

int BGBCC_CCXL_CompileVarDummyP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(	BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_break, "break") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_case, "case") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_case_default, "case_default") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_classdef, "classdef") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_comma, "comma") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_continue, "continue") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_enumdef, "enumdef") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_goto, "goto") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_goto_case, "goto_case") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_ifarch, "ifarch") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_label, "label") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_methodcall, "methodcall") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_msvc_asm, "msvc_asm") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_null_statement, "null_statement") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_predec, "predec") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_preinc, "preinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_switch, "switch") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_throw, "throw") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_try_catch, "try_catch") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_linecomment, "linecomment"))
		{ return(1); }
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_blockcomment, "blockcomment"))
		{ return(1); }

	return(0);
}

void BGBCC_CCXL_CompileVarStatementBlock(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	int na, ci;
	
//	c=l;
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(l);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(l, ci);

		BGBCC_CCXL_CompileVarStatement(ctx, c);
//		c=BCCX_Next(c);
	}
}

int BGBCC_CCXL_CompileVarStatementCompound(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	int sqn, sqon;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		sqon=ctx->vlcl_curseq;
//		sqn=ctx->vlcl_seq++;
		sqn=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");

		if(ctx->vlcl_stackpos>=256)
			{ BGBCC_DBGBREAK }

		ctx->vlcl_curseq=sqn;
		ctx->vlcl_stack[ctx->vlcl_stackpos++]=sqn;

//		c=BCCX_Child(l);
//		BGBCC_CCXL_CompileVarStatementBlock(ctx, c);
		BGBCC_CCXL_CompileVarStatementBlock(ctx, l);
		
		ctx->vlcl_stackpos--;
		ctx->vlcl_curseq=sqon;

 		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		c=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
		}

		c=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
		}

 		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ifarch, "ifarch"))
	{
		c=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
		}

		c=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
		}

 		return(1);
	}

	if(
		BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_switch, "switch") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		c=BCCX_FetchCst(l, &bgbcc_rcst_init, "init");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
		}

//		c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		c=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatementBlock(ctx, c);
		}else
		{
			c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
			if(c)
			{
				BGBCC_CCXL_CompileVarStatement(ctx, c);
			}
		}
 		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		return(1);
	}

	return(0);
}

void BGBCC_CCXL_CompileVarStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s0, *s1;
	int i, na, ci;

	s=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if(i)ctx->lln=i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
//				c=BCCX_Next(c);
				continue;
			}

			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");
//			v=NULL;

//			fi=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//			if(ctx->cur_struct && (fi&BGBCC_TYFL_STATIC))
//				continue;

			BGBCC_CCXL_EmitVar2(ctx, s, t, v);

//			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_var, "var"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_EmitVar2(ctx, s, t, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
//		s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");

		if(ctx->cur_struct && !ctx->cf_n)
		{
			BGBCC_CCXL_CompileMethodProto(ctx, t, s, n);
		}else
		{
			s1=BGBCC_CCXL_GenProtoSig(ctx, t, n);
			BGBCC_CCXL_EmitVar3(ctx, s, s1);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_constructor, "constructor"))
	{
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");

		s1=BGBCC_CCXL_GenProtoSig(ctx, NULL, n);
//		BGBCC_CCXL_EmitVar3(ctx, "new", s1);
		BGBCC_CCXL_EmitVar3(ctx, "__ctor", s1);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_property, "property"))
	{
		BGBCC_CCXL_EmitVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CCXL_CompileVarStatementCompound(ctx, l))
		return;

	if(BGBCC_CCXL_CompileVarDummyP(ctx, l))
		return;

	BGBCC_CCXL_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

void BGBCC_CCXL_CompileVarStatement2(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t, *ntl;
	char *s, *s0;
	s64 fi;
	int i, na, ci;

	s=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if(i)ctx->lln=i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
//				c=BCCX_Next(c);
				continue;
			}

			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

			fi=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
			if(ctx->cur_struct && !(fi&BGBCC_TYFL_STATIC))
			{
//				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CCXL_EmitTopVar(ctx, s, t, v);

//			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
		if(ctx->ccxl_top_only)
			return;
	
//		s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//		ntl=BCCX_FetchCst(l, &bgbcc_rcst_argdecls, "argdecls");

		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		ntl=BCCX_FindTagCst(l, &bgbcc_rcst_argdecls, "argdecls");

		if(!strcmp(s, "LinkPolyobj"))
		{
			i=-1;
		}

		BGBCC_CCXL_CompileBlock2(ctx, t, s, n, v, ntl);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_constructor, "constructor"))
	{
		if(ctx->ccxl_top_only)
			return;

//		s="new";
		s="__ctor";
		t=NULL;
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");

		BGBCC_CCXL_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_property, "property"))
	{
		if(ctx->ccxl_top_only)
			return;

		BGBCC_CCXL_CompileVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CCXL_CompileVarDummyP(ctx, l))
		return;

	BGBCC_CCXL_Error(ctx, "Unhandled var statement2 %s\n", BCCX_Tag(l));
}

BCCX_Node *BGBCC_CCXL_ArgDeclsTypeForName(BGBCC_TransState *ctx,
	BCCX_Node *adecl, char *name)
{
	BCCX_Node *c, *t, *n, *u, *v, *c2;
	char *s0, *s1;
	int na, ci, na2, ci2;
	
	if(!adecl || !name)
		return(NULL);
	
//	c=adecl;
//	while(c)
//	{

	na=BCCX_GetNodeChildCount(adecl);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(adecl, ci);

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_vars, "vars"))
		{
//			c=BCCX_Next(c);
			continue;
		}
		
//		c2=BCCX_Child(c);
//		while(c2)
//		{

		na2=BCCX_GetNodeChildCount(c);
		for(ci2=0; ci2<na2; ci2++)
		{
			c2=BCCX_GetNodeIndex(c, ci2);

			if(!BCCX_TagIsCstP(c2, &bgbcc_rcst_var, "var"))
			{
//				c2=BCCX_Next(c2);
				continue;
			}

			s0=BCCX_GetCst(c2, &bgbcc_rcst_name, "name");
			t=BCCX_FindTagCst(c2, &bgbcc_rcst_type, "type");
//			v=BCCX_Fetch(c2, "value");

			if(!strcmp(s0, name))
				return(t);

//			BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
//			c2=BCCX_Next(c2);
		}

//		c=BCCX_Next(c);
	}
	
	return(NULL);
}

BCCX_Node *BGBCC_CCXL_CompileBlock(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	return(BGBCC_CCXL_CompileBlock2(ctx, type, name, args, body, NULL));
}

BCCX_Node *BGBCC_CCXL_CompileBlock2(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body,
	BCCX_Node *adecl)
{
	char tb[128];
	ccxl_label cname[16], bname[16];

	BGBCC_CCXL_LiteralInfo *obj;

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u, *t2;
	BCCX_Node *ocf_ty;
	char *ocf_n, *ocs_n;
	char *s, *s0, *s1;
	char *sig;
	int tk, o_nimp, o_infunc;
	int i, j, k, na, ci;

//	if(name && !strcmp(name, "D_PolysetDraw"))
//	{
//		k=-1;
//		BGBCC_DBGBREAK
//	}

	BGBCC_CCXL_SetupLambdaFold(ctx, args);
	BGBCC_CCXL_DoStatementLambdaFold(ctx, body);

#if 0
	while(ctx->dynobj)
	{
		c=ctx->dynobj;
		ctx->dynobj=NULL;
		ctx->dynobj_e=NULL;
		while(c)
		{
			BGBCC_CCXL_CompileTopStatement(ctx, c);
			c=BCCX_Next(c);
		}
	}
#endif

#if 1
	while(ctx->dynobj_stkpos>0)
	{
		c=ctx->dynobj_stk[--ctx->dynobj_stkpos];
		BGBCC_CCXL_CompileTopStatement(ctx, c);
	}
#endif

	i=0;
	if(type)i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");

	name=BGBCC_CCXL_QualifyNameNS(ctx, name);
//	name=BGBCC_CCXL_QualifyNameNSFl(ctx, name, i);

	sig=BGBCC_CCXL_VarSigProto(ctx, type, name, args);
	name=BGBCC_CCXL_VarMangleName(ctx, name, sig, type);

	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	ctx->contstack=cname;
	ctx->breakstack=bname;
	ctx->contstackpos=0;
	ctx->breakstackpos=0;

	BGBCC_CCXL_LoadslotCacheFlush(ctx);

	ocs_n=ctx->cur_struct;
	ocf_n=ctx->cf_n;
	ocf_ty=ctx->cf_ty;
	o_nimp=ctx->n_imp;
	o_infunc=ctx->ccxl_in_func;
	
	ctx->cf_n=name;
	ctx->cf_ty=type;

	if(!ctx->cur_struct)
	{
		strcpy(tb, name);
		s=tb+strlen(tb);
		while((s>tb) && (*s!='/'))
			s--;
		if(s>tb)
		{
			*s=0;

			obj=BGBCC_CCXL_LookupStructure(ctx, tb);
			if(obj)
			{
				ctx->cur_struct=bgbcc_strdup(tb);
			}
		}
	}

	ctx->ccxl_in_func=1;

#if 0
	if(!strcmp(name, "TKRA_UnpackMatrix16fv"))
	{
		__debugbreak();
	}
#endif

#if 1
	if(!strcmp(name, "generateHashValue"))
		{ k=-1; }
#endif

#if 1
	if(!strcmp(name, "SelectPathDir"))
		{ k=-1; }
#endif
	

	tk=0;

	if(BCCX_TagIsCstP(body, &bgbcc_rcst_begin, "begin"))
	{
		tk=BCCX_GetIntCst(body, &bgbcc_rcst_tokens, "tokens");
//		body=BCCX_Child(body);
	}


	BGBCC_CCXL_BeginName(ctx, (ctx->cur_struct)?
		CCXL_CMD_METHOD:CCXL_CMD_FUNCTION, ctx->cf_n);

	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_SRCTOK, tk);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
//	c=args;
//	while(c)
//	{

	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
//			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		s0=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		
		if(adecl)
		{
			t2=BGBCC_CCXL_ArgDeclsTypeForName(ctx, adecl, s0);
			if(t2)t=t2;
		}
		
//		BGBCC_CCXL_EmitVar(ctx, s0, t, NULL);
		BGBCC_CCXL_EmitVarArg(ctx, s0, t, NULL);
//		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_LOCALS);
//	c=body;
	ctx->vlcl_seq=0;	ctx->vlcl_stackpos=0;
	ctx->vlcl_curseq=ctx->vlcl_seq++;
//	while(c)
//	{
//		BGBCC_CCXL_CompileVarStatement(ctx, c);
//		c=BCCX_Next(c);
//	}

	BGBCC_CCXL_CompileVarStatement(ctx, body);

	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_BODY);
	ctx->reqlfn=1;

//	BGBCC_CCXL_StackFn(ctx, ctx->lfn);
//	BGBCC_CCXL_StackLn(ctx, ctx->lln);

//	c=body;
	ctx->vlcl_seq=0;	ctx->vlcl_stackpos=0;
	ctx->vlcl_curseq=ctx->vlcl_seq++;
//	while(c)
//	{
//		BGBCC_CCXL_CompileStatement(ctx, c);
//		c=BCCX_Next(c);
//	}
	BGBCC_CCXL_CompileStatement(ctx, body);
	BGBCC_CCXL_End(ctx);
	BGBCC_CCXL_End(ctx);

	ctx->cur_struct=ocs_n;
	ctx->cf_n=ocf_n;
	ctx->cf_ty=ocf_ty;
	ctx->n_imp=o_nimp;
	ctx->ccxl_in_func=o_infunc;

	BGBCC_CCXL_LoadslotCacheFlush(ctx);

	return(NULL);
}

char *BGBCC_CCXL_GenProtoSig(BGBCC_TransState *ctx,
	BCCX_Node *type, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;
	int na, ci;

	BCCX_Node *c, *n;
	int i, j, k;

//	c=args;
	t=tb;
	*t++='(';
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			*t++='z';
//			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		if(n)
		{
//			s=BGBCC_CCXL_VarTypeString(ctx, n);
			s=BGBCC_CCXL_VarTypeStringNoA0(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

//		c=BCCX_Next(c);
	}
	*t++=')';

	if(type)
	{
//		s=BGBCC_CCXL_VarTypeString(ctx, type);
		s=BGBCC_CCXL_VarTypeStringNoA0(ctx, type);
		if(s) { strcpy(t, s); t+=strlen(t); }
	}else
	{
//		*t++='Pv';
		*t++='P';
		*t++='v';
	}

	*t++=0;

	return(bgbcc_strdup(tb));
}

void BGBCC_CCXL_EmitSigProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
#if 0
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k, na, ci;

	if(!type)return;

	i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");
	if(i&BGBCC_TYFL_STATIC)return;

//	c=args;
	t=tb;
	*t++='(';
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			*t++='z';
//			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		if(n)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

//		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_CCXL_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

//	BGBCC_CCXL_BindSig(ctx, name, tb);

//	s=BGBCC_CCXL_VarTypeFlagsString(ctx, type);
//	if(s && (*s))
//			BGBCC_CCXL_BindVarInfo(ctx, name, "flagstr", s);
#endif
}

char *BGBCC_CCXL_VarSigProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k, na, ci;

	if(!type)
		return(NULL);

	i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");

//	c=args;
	t=tb;
	*t++='(';
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			*t++='z';
//			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		if(n)
		{
//			s=BGBCC_CCXL_VarImageTypeString(ctx, n);
			s=BGBCC_CCXL_VarImageTypeStringNoA0(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}else
		{
			*t++='r';
		}

//		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_CCXL_VarImageTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;
	
	return(bgbcc_strdup(tb));
}

BCCX_Node *BGBCC_CCXL_CompileProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	BCCX_Node *octy;
	char *ocfn;
	char *sig;
	int i, j, k, na, ci;

//	i=0;
//	if(type)i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");

	name=BGBCC_CCXL_QualifyNameNS(ctx, name);
//	name=BGBCC_CCXL_QualifyNameNSFl(ctx, name, i);

	sig=BGBCC_CCXL_VarSigProto(ctx, type, name, args);
	name=BGBCC_CCXL_VarMangleName(ctx, name, sig, type);

	/* avoid redefining the same stuff */
	if(BGBCC_CCXL_LookupGlobal(ctx, name))
		{ return(NULL); }

	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	ocfn=ctx->cf_n;
	octy=ctx->cf_ty;
	ctx->cf_n=name;
	ctx->cf_ty=type;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_PROTOTYPE, ctx->cf_n);
	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
//	c=args;
//	while(c)
//	{

	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
//			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		BGBCC_CCXL_EmitVar(ctx,
			BCCX_GetCst(c, &bgbcc_rcst_name, "name"), t, NULL);
//		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_End(ctx);

	ctx->cf_n=ocfn;
	ctx->cf_ty=octy;

	return(NULL);
}

BCCX_Node *BGBCC_CCXL_CompileMethodProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	BCCX_Node *octy;
	char *ocfn;
	char *sig;
	int i, j, k, na, ci;

	sig=BGBCC_CCXL_VarSigProto(ctx, type, name, args);
	name=BGBCC_CCXL_VarMangleName(ctx, name, sig, type);

//	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	ocfn=ctx->cf_n;
	octy=ctx->cf_ty;
	ctx->cf_n=name;
	ctx->cf_ty=type;
	ctx->ccxl_isdef_methodproto=1;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_METHODPROTO, ctx->cf_n);
	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
//	c=args;
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
//			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		BGBCC_CCXL_EmitVar(ctx,
			BCCX_GetCst(c, &bgbcc_rcst_name, "name"), t, NULL);
//		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_End(ctx);

	ctx->ccxl_isdef_methodproto=0;

	ctx->cf_n=ocfn;
	ctx->cf_ty=octy;

	return(NULL);
}

void BGBCC_CCXL_CompileStruct(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s0, *os0, *s1;
	int i, j, na, ci;

	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

	if(*s0=='!')
	{
		/* Template Related, Ignore */
		return;
	}

	s=BGBCC_CCXL_QualifyNameNS(ctx, s0);

	if(!strcmp(s, "bot_matchvariable_s"))
		{ j=-1; }

	/* avoid redefining the same stuff */
	if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP))
	{
		cur=BGBCC_CCXL_LookupStructure(ctx, s);
		if(cur)
		{
			if(cur->decl && cur->decl->n_fields)
				return;
		}
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "struct");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_STRUCT, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);

//	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);

		BGBCC_CCXL_CompileVarStatement(ctx, c);
//		c=BCCX_Next(c);
	}

//	BGBCC_CCXL_CompileVarStatement(ctx, c);

	BGBCC_CCXL_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CCXL_CompileUnion(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s0, *os0;
	int i, j, na, ci;

	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

	if(*s0=='!')
	{
		/* Template Related, Ignore */
		return;
	}

	s=BGBCC_CCXL_QualifyNameNS(ctx, s0);

	/* avoid redefining the same stuff */
	if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP))
	{
		cur=BGBCC_CCXL_LookupStructure(ctx, s);
		if(cur)
		{
			if(cur->decl && cur->decl->n_fields)
				return;
		}
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "union");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_UNION, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);

//	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//	while(c)
//	{
//		BGBCC_CCXL_CompileVarStatement(ctx, c);
//		c=BCCX_Next(c);
//	}

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileVarStatement(ctx, c);
	}

	BGBCC_CCXL_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CCXL_CompileClass(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0, *qn;
	int i, j, j1, j2, na, ci;

	if(ctx->cf_n)
		{ BGBCC_DBGBREAK }

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

	if(*s=='!')
	{
		/* Template Related, Ignore */
		return;
	}

	qn=BGBCC_CCXL_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP))
	{
		cur=BGBCC_CCXL_LookupStructure(ctx, qn);
		if(cur)
		{
			if(cur->decl && cur->decl->n_fields)
				return;
		}
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, qn, "class");
//	BGBCC_CCXL_BindStructInfoI(ctx, qn, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=qn;
	j=ctx->cur_idx;
	j1=ctx->cur_idx2;
	j2=ctx->cur_idx3;

	ctx->cur_idx=0;
	ctx->cur_idx2=0;
	ctx->cur_idx3=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_CLASS, qn);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, qn);

//	c=BCCX_FetchCst(l, &bgbcc_rcst_super, "super");
	n=BCCX_FindTagCst(l, &bgbcc_rcst_super, "super");
//	if(c)
	if(n)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_EXTENDS);

//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			s=BGBCC_CCXL_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s);
//				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
				BGBCC_CCXL_End(ctx);
			}
//			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

//	c=BCCX_FetchCst(l, &bgbcc_rcst_impl, "impl");
	n=BCCX_FindTagCst(l, &bgbcc_rcst_impl, "impl");
//	if(c)
	if(n)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_IMPLEMENTS);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			s=BGBCC_CCXL_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
				BGBCC_CCXL_End(ctx);
			}
//			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

#if 0
	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		if(ctx->cf_n)
			{ BGBCC_DBGBREAK }

		c=BCCX_Next(c);
	}
#endif

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileVarStatement(ctx, c);
	}

	BGBCC_CCXL_End(ctx);

//	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//	while(c)
//	{
//		BGBCC_CCXL_CompileVarStatement2(ctx, c);
//		c=BCCX_Next(c);
//	}

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileVarStatement2(ctx, c);
	}


	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
}

void BGBCC_CCXL_CompileEnum(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s1, *s2, *s3, *os0;
	int op;
	int i, j, na, ci;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

	if(*s=='!')
	{
		/* Template Related, Ignore */
		return;
	}

	s=BGBCC_CCXL_QualifyNameNS(ctx, s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "enum");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;
	ctx->cur_idx4=0;

	if((ctx->lang==BGBCC_LANG_C) ||
		(ctx->lang==BGBCC_LANG_CPP))
	{
//		c=BCCX_FetchCst(l, &bgbcc_rcst_defs, "defs");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_defs, "defs");
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_def, "def"))
			{
				op=CCXL_CMD_VARDECL;
				j=BCCX_GetIntCst(c, &bgbcc_rcst_value, "value");
				s1=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				s2="i";
				s3="k";

				if(BGBCC_CCXL_CheckDefinedContextName(ctx,
					CCXL_CMD_VARDECL, s1))
				{
//					c=BCCX_Next(c);
					continue;
				}

				BGBCC_CCXL_BeginName(ctx, op, s1);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s2);

				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s3);

				BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
				BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, j++);
				BGBCC_CCXL_End(ctx);

				BGBCC_CCXL_End(ctx);
			}
//			c=BCCX_Next(c);
		}
	}

	if((ctx->lang==BGBCC_LANG_CS) ||
		(ctx->lang==BGBCC_LANG_BS2) ||
		(ctx->lang==BGBCC_LANG_BS))
	{
		BGBCC_CCXL_BeginName(ctx, CCXL_CMD_ENUMDEF, s);
	
//		c=BCCX_FetchCst(l, &bgbcc_rcst_defs, "defs");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_defs, "defs");
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_def, "def"))
			{
				j=BCCX_GetIntCst(c, &bgbcc_rcst_value, "value");
				s1=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				s2="i";
				s3="k";

				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_STATICVARDECL, s1);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s2);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s3);
				BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
				BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, j++);
				BGBCC_CCXL_End(ctx);
				BGBCC_CCXL_End(ctx);
			}
//			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

BCCX_Node *BGBCC_CCXL_CompileSProto(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u, *a;
	char *s, *s1;
	int na, ci;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_S_PROTOTYPE, s);

	t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//	a=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
	a=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
	BGBCC_CCXL_EmitVarFunc(ctx, s, t, a);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
//	c=a;
//	while(c)
//	{

	na=BCCX_GetNodeChildCount(a);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(a, ci);

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
//			c=BCCX_Next(c);
			continue;
		}
		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
//			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
		s1=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_EmitVar(ctx, s1, t, NULL);
//		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);
	BGBCC_CCXL_End(ctx);


//	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//	t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//	u=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");

//	s1=BGBCC_CCXL_GenProtoSig(ctx, t, u);
//	BGBCC_CCXL_BindStructSig(ctx, s, "func");
//	BGBCC_CCXL_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

BCCX_Node *BGBCC_CCXL_GetNodeAttribute(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *cn, *cn1;
	char *s;
	int na, ci, na1, ci1;

//	cn=BCCX_Child(l);
//	while(cn)
//	{

	na=BCCX_GetNodeChildCount(l);
	for(ci=0; ci<na; ci++)
	{
		cn=BCCX_GetNodeIndex(l, ci);

		if(!BCCX_TagIsCstP(cn, &bgbcc_rcst_declspec, "declspec") &&
			!BCCX_TagIsCstP(cn, &bgbcc_rcst_attribute, "attribute"))
		{
//			cn=BCCX_Next(cn);
			continue;
		}

//		cn1=BCCX_Child(cn);
//		while(cn1)
//		{
		na1=BCCX_GetNodeChildCount(cn);
		for(ci1=0; ci1<na1; ci1++)
		{
			cn1=BCCX_GetNodeIndex(cn, ci1);

			if(!BCCX_TagIsCstP(cn1, &bgbcc_rcst_attr, "attr"))
			{
//				cn1=BCCX_Next(cn1);
				continue;
			}

			s=BCCX_Get(cn1, "name");
			if(s && !strcmp(s, name))
				return(cn1);

//			cn1=BCCX_Next(cn1);
		}
//		cn=BCCX_Next(cn);
	}
	
	return(NULL);
}

char *BGBCC_CCXL_GetNodeAttributeString(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	char *s;
	
	attr=BGBCC_CCXL_GetNodeAttribute(ctx, l, name);
	if(!attr)return(NULL);

	s=BCCX_Get(attr, "name");

	cn=BCCX_FetchCst(attr, &bgbcc_rcst_args, "args");
	if(!cn)return(NULL);

	if(!BCCX_TagIsCstP(cn, &bgbcc_rcst_string, "string"))
		return(NULL);
	s=BCCX_Get(cn, "value");
	return(s);
}

char *BGBCC_CCXL_GetNodeAttributeStringOrRef(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	char *s;
	
	attr=BGBCC_CCXL_GetNodeAttribute(ctx, l, name);
	if(!attr)return(NULL);

	s=BCCX_Get(attr, "name");

	cn=BCCX_FetchCst(attr, &bgbcc_rcst_args, "args");
	if(!cn)return(NULL);

	if(BCCX_TagIsCstP(cn, &bgbcc_rcst_string, "string"))
	{
		s=BCCX_Get(cn, "value");
		return(s);
	}

	if(BCCX_TagIsCstP(cn, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_Get(cn, "name");
		return(s);
	}

	return(NULL);
}

s64 BGBCC_CCXL_GetNodeAttributeInt(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	s64 li;
	char *s;
	
	attr=BGBCC_CCXL_GetNodeAttribute(ctx, l, name);
	if(!attr)return(0);

	s=BCCX_Get(attr, "name");

	cn=BCCX_FetchCst(attr, &bgbcc_rcst_args, "args");
	if(!cn)return(0);

	if(!BCCX_TagIsCstP(cn, &bgbcc_rcst_int, "int"))
		return(0);
	li=BCCX_GetIntCst(cn, &bgbcc_rcst_value, "value");
	return(li);
}

void BGBCC_CCXL_CompileTypedef(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t, *n1;
	char *s, *s1, *s2;
	int i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_var, "var"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		
		n1=BCCX_FindTagCst(t, &bgbcc_rcst_size, "size");
		if(n1)
		{
			s1=BGBCC_CCXL_VarTypeString(ctx, t);
			s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);

			BGBCC_CCXL_BeginName(ctx, CCXL_CMD_TYPEDEF, s);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
			BGBCC_CCXL_End(ctx);
		}

#if 0
//		s1=BGBCC_CCXL_VarTypeString(ctx, t);
//		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);
//		BGBCC_CCXL_BindTypeSig(ctx, s, s1);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindTypeInfo(ctx, s, "flagstr", s2);

		s2=BGBCC_CCXL_GetNodeAttributeString(ctx, t, "dyt_name");
		if(s2)
		{
			printf("BGBCC_CCXL_CompileTypedef: DyType %s->%s\n", s, s2);
		
//			BGBCC_CCXL_BindTypeInfo(ctx, s, "dytype", s2);
//			BGBCC_CCXL_BindDyTypeSig(ctx, s2, s1);
		}
#endif

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_proto, "proto"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		v=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");

		s1=BGBCC_CCXL_VarSigProto(ctx, t, s, v);
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);

		BGBCC_CCXL_BeginName(ctx, CCXL_CMD_TYPEDEF, s);
		BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
		BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s1);
		BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
		BGBCC_CCXL_End(ctx);

#if 0
		n1=BCCX_FindTagCst(t, &bgbcc_rcst_size, "size");
		if(n1)
		{
			s1=BGBCC_CCXL_VarTypeString(ctx, t);
			s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);

			BGBCC_CCXL_BeginName(ctx, CCXL_CMD_TYPEDEF, s);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
			BGBCC_CCXL_End(ctx);
		}
#endif
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_template, "template"))
	{
		/* Templates disappear after the frontend. */
		return;
	}

	BGBCC_CCXL_StubError(ctx);

	return;
}

void BGBCC_CCXL_CompileSetupNsPath(BGBCC_TransState *ctx, char *nsp)
{
	char tb[256];
	char *s, *t;
	int i;
	
	ctx->n_imp=0;
	
	s=nsp; t=tb;
	while(*s)
	{
		if(*s==';')
		{
			*t=0;
			i=ctx->n_imp++;
			ctx->imp_ns[i]=bgbcc_strdup(tb);
			
			s++;
			t=tb;
			continue;
		}
		
		*t++=*s++;
	}
}

void BGBCC_CCXL_CompileStructs(BGBCC_TransState *ctx)
{
	BCCX_Node *c, *t, *n, *l;
	BGBCC_CCXL_LiteralInfo *litobj;
	char *nsp;
	int i, na, ci;

//	c=ctx->types;
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(ctx->types);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->types, ci);

		BGBCC_CCXL_CompileTypedef(ctx, c);
//		c=BCCX_Next(c);
	}

//	c=ctx->structs;
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(ctx->structs);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->structs, ci);

		nsp=BCCX_GetCst(c, &bgbcc_rcst_nspath, "nspath");
		if(nsp)
		{
			BGBCC_CCXL_CompileSetupNsPath(ctx, nsp);
		}

		l=c;
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_struct, "struct"))
			{ BGBCC_CCXL_CompileStruct(ctx, c); }
		else if(BCCX_TagIsCstP(c, &bgbcc_rcst_union, "union"))
			{ BGBCC_CCXL_CompileUnion(ctx, c); }
		else if(BCCX_TagIsCstP(c, &bgbcc_rcst_enum, "enum"))
			{ BGBCC_CCXL_CompileEnum(ctx, c); }
		else if(BCCX_TagIsCstP(c, &bgbcc_rcst_s_proto, "s_proto"))
			{ BGBCC_CCXL_CompileSProto(ctx, c); }
		else if(BCCX_TagIsCstP(c, &bgbcc_rcst_func, "func"))
			{ BGBCC_CCXL_CompileSProto(ctx, c); }
		else if(BCCX_TagIsCstP(c, &bgbcc_rcst_class, "class"))
			{ BGBCC_CCXL_CompileClass(ctx, c); }
		else
		{
			BGBCC_CCXL_StubError(ctx);
		}

//		c=BCCX_Next(c);
	}

#if 1
	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 0);
	}
#endif

//	BGBCC_CCXL_CheckSanityGlobals(ctx);		//BGB: debug
}

void BGBCC_CCXL_EmitVarValue(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BGBCC_CCXL_RegisterInfo *decl;
	BCCX_Node *t, *nt1, *nv1;
	ccxl_register rval;
	char *s;
	int i;

	v=BGBCC_CCXL_ReduceExprConst(ctx, v);

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_int, "int"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_tysuf, "tysuf");
		if(s &&
			(!strcmp(s, "L") || !strcmp(s, "LL") ||
			 !strcmp(s, "UL") || !strcmp(s, "ULL") ||
			 !strcmp(s, "LX") || !strcmp(s, "ULX")))
		{
			BGBCC_CCXL_LiteralLong(ctx, CCXL_ATTR_VALUE,
				BCCX_GetIntCst(v, &bgbcc_rcst_value, "value"));
			return;
		}
	
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
			BCCX_GetIntCst(v, &bgbcc_rcst_value, "value"));
		return;
	}
	if(BCCX_TagIsCstP(v, &bgbcc_rcst_real, "real"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_tysuf, "tysuf");
		if(s && !strcmp(s, "F"))
		{
			BGBCC_CCXL_LiteralFloat(ctx, CCXL_ATTR_VALUE,
				BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value"));
			return;
		}

		BGBCC_CCXL_LiteralDouble(ctx, CCXL_ATTR_VALUE,
			BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value"));
		return;
	}
	if(BCCX_TagIsCstP(v, &bgbcc_rcst_string, "string"))
	{
		BGBCC_CCXL_LiteralStr(ctx, CCXL_ATTR_VALUE,
			BCCX_GetCst(v, &bgbcc_rcst_value, "value"));
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_charstring, "charstring"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_value, "value");
		if(s) { i=BGBCP_ParseChar(&s); }
			else i=0;
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, i);
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_attr, "attr"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
		nv1=BCCX_FetchCst(v, &bgbcc_rcst_value, "value");
		
		BGBCC_CCXL_LiteralSetField(ctx, CCXL_ATTR_VALUE, s);
		BGBCC_CCXL_EmitVarValue(ctx, nv1);
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
		decl=BGBCC_CCXL_LookupGlobal(ctx, s);
		if(decl)
		{
			rval=decl->value;
			
			if(BGBCC_CCXL_IsRegImmIntP(ctx, rval))
			{
				BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmIntValue(ctx, rval));
				return;
			}
			if(BGBCC_CCXL_IsRegImmLongP(ctx, rval))
			{
				BGBCC_CCXL_LiteralLong(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmLongValue(ctx, rval));
				return;
			}

			if(BGBCC_CCXL_IsRegImmFloatP(ctx, rval))
			{
				BGBCC_CCXL_LiteralFloat(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmFloatValue(ctx, rval));
				return;
			}
			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, rval))
			{
				BGBCC_CCXL_LiteralDouble(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmDoubleValue(ctx, rval));
				return;
			}

			if(decl->regtype==CCXL_LITID_FUNCTION)
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(decl->sig && (*decl->sig=='A'))
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(ctx->cf_n)
				return;

			if(!rval.val)
				return;
			
			BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: "
				"Bad Reg Literal Type %08X\n", rval.val);
			return;
		}

		if(ctx->cf_n)
			return;

		BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: "
			"Undeclared Var %s\n", s);
		return;
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, v, "&"))
	{
		t=BCCX_FetchCst(v, &bgbcc_rcst_value, "value");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			decl=BGBCC_CCXL_LookupGlobal(ctx, s);
			if(decl)
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_getindex, "getindex"))
		{
			nt1=BCCX_FetchCst(t, &bgbcc_rcst_array, "array");
			nv1=BCCX_FetchCst(t, &bgbcc_rcst_index, "index");
			nt1=BGBCC_CCXL_ReduceExpr(ctx, nt1);
			nv1=BGBCC_CCXL_ReduceExprConst(ctx, nv1);

			if(	BCCX_TagIsCstP(nt1, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(nv1, &bgbcc_rcst_int, "int"))
			{
				s=BCCX_GetCst(nt1, &bgbcc_rcst_name, "name");
				i=BCCX_GetIntCst(nv1, &bgbcc_rcst_value, "value");
				
				decl=BGBCC_CCXL_LookupGlobal(ctx, s);
				if(decl)
				{
					BGBCC_CCXL_LiteralGlobalAddrIdx(ctx, CCXL_ATTR_VALUE,
						decl->gblid, i);
					return;
				}
			}
		}
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_getindex, "getindex"))
	{
		nt1=BCCX_FetchCst(v, &bgbcc_rcst_array, "array");
		nv1=BCCX_FetchCst(v, &bgbcc_rcst_index, "index");
		nt1=BGBCC_CCXL_ReduceExpr(ctx, nt1);
		nv1=BGBCC_CCXL_ReduceExprConst(ctx, nv1);

		if(	BCCX_TagIsCstP(nt1, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(nv1, &bgbcc_rcst_int, "int"))
		{
			s=BCCX_GetCst(nt1, &bgbcc_rcst_name, "name");
			i=BCCX_GetIntCst(nv1, &bgbcc_rcst_value, "value");
			
			decl=BGBCC_CCXL_LookupGlobal(ctx, s);
			if(decl)
			{
				BGBCC_CCXL_LiteralGlobalAddrIdx(ctx, CCXL_ATTR_VALUE,
					decl->gblid, i);
				return;
			}
		}
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_cast, "cast"))
	{
		t=BCCX_FetchCst(v, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_EmitVarValue(ctx, t);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, v, "+"))
	{
		nt1=BCCX_FetchCst(v, &bgbcc_rcst_left, "left");
		nv1=BCCX_FetchCst(v, &bgbcc_rcst_right, "right");
		nt1=BGBCC_CCXL_ReduceExpr(ctx, nt1);
		nv1=BGBCC_CCXL_ReduceExprConst(ctx, nv1);

		if(	BCCX_TagIsCstP(nt1, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(nv1, &bgbcc_rcst_int, "int"))
		{
			s=BCCX_GetCst(nt1, &bgbcc_rcst_name, "name");
			i=BCCX_GetIntCst(nv1, &bgbcc_rcst_value, "value");
			
			decl=BGBCC_CCXL_LookupGlobal(ctx, s);
			if(decl && BGBCC_CCXL_TypeArrayP(ctx, decl->type))
			{
				BGBCC_CCXL_LiteralGlobalAddrIdx(ctx, CCXL_ATTR_VALUE,
					decl->gblid, i);
				return;
			}
		}
	}

	ctx->ccxl_var_needsinit=1;
	BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, 0);		//placeholder

	if(ctx->cf_n)
		return;
	if(ctx->ccxl_top_only)
		return;

	s=BCCX_Tag(v);
//	BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: Bad Value %s\n", s);
	BGBCC_CCXL_Warn(ctx, "Non-Constant Initializer (tag: %s)\n", s);
	return;
}

void BGBCC_CCXL_EmitVarValueR(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BCCX_Node *c, *n;
	int na, ci;

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_list, "list"))
	{
//		c=BCCX_Child(v);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(v);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(v, ci);

			BGBCC_CCXL_EmitVarValueR(ctx, c);
//			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_array, "array") ||
		BCCX_TagIsCstP(v, &bgbcc_rcst_vector, "vector") ||
		BCCX_TagIsCstP(v, &bgbcc_rcst_object, "object"))
	{
		n=BCCX_FindTagCst(v, &bgbcc_rcst_list, "list");
		BGBCC_CCXL_EmitVarValueR(ctx, n);

#if 0
		c=BCCX_FetchCst(v, &bgbcc_rcst_list, "list");
		while(c)
		{
			BGBCC_CCXL_EmitVarValueR(ctx, c);
			c=BCCX_Next(c);
		}
#endif
		return;
	}

	BGBCC_CCXL_EmitVarValue(ctx, v);
}

void BGBCC_CCXL_EmitVarValueR2(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BCCX_Node *c, *n;
	int na, ci;

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_list, "list"))
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_LIST);

//		c=BCCX_Child(v);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(v);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(v, ci);

			BGBCC_CCXL_EmitVarValueR2(ctx, c);
//			c=BCCX_Next(c);
		}

		BGBCC_CCXL_End(ctx);
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_array, "array") ||
		BCCX_TagIsCstP(v, &bgbcc_rcst_vector, "vector") ||
		BCCX_TagIsCstP(v, &bgbcc_rcst_object, "object"))
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_LIST);

		n=BCCX_FetchCst(v, &bgbcc_rcst_list, "list");
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);

			BGBCC_CCXL_EmitVarValueR2(ctx, c);
//			c=BCCX_Next(c);
		}

//		c=BCCX_FetchCst(v, &bgbcc_rcst_list, "list");
//		while(c)
//		{
//			BGBCC_CCXL_EmitVarValueR2(ctx, c);
//			c=BCCX_Next(c);
//		}

		BGBCC_CCXL_End(ctx);
		return;
	}

	BGBCC_CCXL_EmitVarValue(ctx, v);
}

char *BGBCC_CCXL_QualifyNameNS(BGBCC_TransState *ctx, char *name)
{
	return(BGBCC_CCXL_QualifyNameNSFl(ctx, name, 0));
}

char *BGBCC_CCXL_QualifyNameNSFl(BGBCC_TransState *ctx, char *name, s64 fl)
{
	char tb[256];
	char *s, *t;

	if(BGBCC_CCXL_NameIsQName(ctx, name))
	{
		return(name);
	}

	if(fl&BGBCC_TYFL_STATIC)
	{
		if(ctx->cur_struct)
		{
			sprintf(tb, "TU%08X/%s/%s", ctx->tuidx, ctx->cur_struct, name);
			s=bgbcc_strdup(tb);
			return(s);
		}

		if(ctx->cur_ns)
		{
			sprintf(tb, "TU%08X/%s/%s", ctx->tuidx, ctx->cur_ns, name);
			s=bgbcc_strdup(tb);
			return(s);
		}

//		sprintf(tb, "TU%08X/%s", ctx->tuidx, name);

		t=tb;
		*t++='T';
		*t++='U';
		t=BGBCC_StrPrintInt8X(t, ctx->tuidx);
		*t++='/';
		t=BGBCC_StrPrintRawStr(t, name);

		s=bgbcc_strdup(tb);
		return(s);
	}

	if(ctx->cur_struct)
	{
//		sprintf(tb, "%s/%s", ctx->cur_struct, name);
		t=BGBCC_StrPrintRawStr(tb, ctx->cur_struct);
		*t++='/';
		t=BGBCC_StrPrintRawStr(t, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	if(ctx->cur_ns)
	{
//		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		t=BGBCC_StrPrintRawStr(tb, ctx->cur_ns);
		*t++='/';
		t=BGBCC_StrPrintRawStr(t, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	return(name);
}

void BGBCC_CCXL_EmitTopVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c, *n;
	ccxl_type tty;
	s64 li;
	int i, j;

	if(!name)
		return;

//	if(!strcmp(name, "anims"))
//	{
//		BGBCC_DBGBREAK
//	}

	i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
//	s1=BGBCC_CCXL_QualifyNameNS(ctx, name);
	s1=BGBCC_CCXL_QualifyNameNSFl(ctx, name, i);

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, s1))
	{
		return;
	}

	li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindVarSig(ctx, s1, s);
//	if(s2 && (*s2))
//			BGBCC_CCXL_BindVarInfo(ctx, s1, "flagstr", s2);

	BGBCC_CCXL_TypeFromSig(ctx, &tty, s);

	if(!s1)return;
	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);

	if(v)
	{
		ctx->ccxl_var_needsinit=0;

		if(BGBCC_CCXL_TypeVarRefP(ctx, tty))
		{
			ctx->ccxl_var_needsinit=1;
		}else
		{
			BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
			BGBCC_CCXL_EmitVarValueR2(ctx, v);
			BGBCC_CCXL_End(ctx);
		}
		
		if(ctx->ccxl_var_needsinit)
		{
			n=BCCX_NewCst(&bgbcc_rcst_var_init, "var_init");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_type, "type", ty));
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_value, "value", v));
			BCCX_AddEnd(ctx->static_init, n);
		}
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_CompileTopStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t, *ntl;
	char *s, *s1, *t1;
	int o_nimp;
	int i, na, ci;

	s=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if(i)ctx->lln=i;

//	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
//		c=BCCX_Child(l);
//		while(c)
//		{

		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_proto, "proto"))
			{
				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
//				n=BCCX_FetchCst(c, &bgbcc_rcst_args, "args");
				n=BCCX_FindTagCst(c, &bgbcc_rcst_args, "args");

				BGBCC_CCXL_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
				v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

				BGBCC_CCXL_EmitTopVar(ctx, s, t, v);
			}

//			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
//		s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//		tk=BCCX_GetIntCst(l, &bgbcc_rcst_tokens, "tokens");
//		ntl=BCCX_FetchCst(l, &bgbcc_rcst_argdecls, "argdecls");

		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		ntl=BCCX_FindTagCst(l, &bgbcc_rcst_argdecls, "argdecls");

		if(!strcmp(s, "LinkPolyobj"))
		{
			i=-1;
		}

		if(ctx->ccxl_top_only)
		{
			BGBCC_CCXL_CompileProto(ctx, t, s, n);
		}else
		{
			BGBCC_CCXL_CompileBlock2(ctx, t, s, n, v, ntl);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_proto, "proto"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");

		BGBCC_CCXL_CompileProto(ctx, t, s, n);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_struct, "struct"))
	{
		BGBCC_CCXL_CompileStruct(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_union, "union"))
	{
		BGBCC_CCXL_CompileUnion(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_class, "class"))
	{
		BGBCC_CCXL_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_classdef, "classdef"))
	{
//		BGBCC_CCXL_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_enum, "enum"))
	{
		BGBCC_CCXL_CompileEnum(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_enumdef, "enumdef"))
	{
//		BGBCC_CCXL_CompileEnum(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_import, "import"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		i=ctx->n_imp++;
		ctx->imp_ns[i]=s;
//		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_using, "using"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		i=ctx->n_imp++;
		ctx->imp_ns[i]=s;
//		BGBCC_CCXL_StubError(ctx);
		return;
	}

#if 0
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_package, "package"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");

		if(ctx->cur_ns)
		{
//			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			t1=BGBCC_StrPrintRawStr(tb, ctx->cur_ns);
			*t1++='/';
			BGBCC_StrPrintRawStr(t1, s);

			s=bgbcc_strdup(tb);
		}

//		BGBCC_CCXL_BindStructSig(ctx, s, "namespace");
//		BGBCC_CCXL_StubError(ctx);

		BGBCC_CCXL_GetNamespaceBuildPath(ctx, s);

		s1=ctx->cur_ns;
		o_nimp=ctx->n_imp;

		ctx->cur_ns=s;

		i=ctx->n_imp++;
		ctx->imp_ns[i]=s;

#if 1
		if(n)
		{
			c=BCCX_Child(n);
			while(c)
			{
				BGBCC_CCXL_CompileTopStatement(ctx, c);
				c=BCCX_Next(c);
			}
			ctx->cur_ns=s1;
		}
#endif

		ctx->cur_ns=s1;
		ctx->n_imp=o_nimp;

		return;
	}
#endif

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_namespace, "namespace") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_package, "package"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		c=BCCX_Child(l);

		if(ctx->cur_ns)
		{
//			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			t1=BGBCC_StrPrintRawStr(tb, ctx->cur_ns);
			*t1++='/';
			BGBCC_StrPrintRawStr(t1, s);

			s=bgbcc_strdup(tb);
		}

//		BGBCC_CCXL_BindStructSig(ctx, s, "namespace");
//		BGBCC_CCXL_StubError(ctx);

		BGBCC_CCXL_GetNamespaceBuildPath(ctx, s);

		s1=ctx->cur_ns;
		o_nimp=ctx->n_imp;
		
		ctx->cur_ns=s;

		i=ctx->n_imp++;
		ctx->imp_ns[i]=s;
		
//		c=BCCX_Child(l);
//		while(c)
//		{

		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			BGBCC_CCXL_CompileTopStatement(ctx, c);
//			c=BCCX_Next(c);
		}

		ctx->cur_ns=s1;
		ctx->n_imp=o_nimp;
		
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_null_statement, "null_statement"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_linecomment, "linecomment"))
		{ return; }
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_blockcomment, "blockcomment"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_pragma, "pragma"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_asm_blob, "asm_blob"))
	{
		c=BCCX_Child(l);
		BGBCC_CCXL_AddAsmBlob(ctx, BCCX_Text(c));
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_msvc_asm, "msvc_asm"))
	{
		c=BCCX_Child(l);
		BGBCC_CCXL_AddAsmBlob(ctx, BCCX_Text(c));
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	BGBCC_CCXL_Error(ctx, "unhandled top statement %s\n", BCCX_Tag(l));
}


int BGBCC_CCXL_CompileModuleCTX(
	BGBCC_TransState *ctx, char *name, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *n1;
	char *s;
	int i, j, ci, na;

	ctx->n_warn=0;
	ctx->n_error=0;
	ctx->ccxl_top_only=0;
	ctx->n_imp=0;
	ctx->static_init=NULL;
	ctx->loop_localstate=0;
	ctx->opt_ptrcache=1;
//	ctx->opt_ptrcache=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "noptropt"))
		ctx->opt_ptrcache=0;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "ptropts"))
		ctx->opt_ptrcache=3;

	if(BGBCC_CCXL_CheckForOptStr(ctx, "ptropt0"))
		ctx->opt_ptrcache=0;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "ptropt1"))
		ctx->opt_ptrcache=1;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "ptropt2"))
		ctx->opt_ptrcache=2;
	if(BGBCC_CCXL_CheckForOptStr(ctx, "ptropt3"))
		ctx->opt_ptrcache=3;

	s=BCCX_GetCst(l, &bgbcc_rcst_lang, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_GetCst(l, &bgbcc_rcst_arch, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
	ctx->tuidx=i;
	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_TUIDX, i);

	ctx->types=BCCX_FindTagCst(l, &bgbcc_rcst_types, "types");
	ctx->structs=BCCX_FindTagCst(l, &bgbcc_rcst_structs, "structs");


#ifdef BGBCC_BCCX2

	for(i=0; i<256; i++)
		ctx->struct_hash_ix[i]=-1;

	na=BCCX_GetNodeChildCount(ctx->structs);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->structs, ci);
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		i=BGBCC_CCXL_HashName(s)&255;
		j=ctx->struct_hash_ix[i];
		ctx->struct_hash_ix[i]=ci;
		BCCX_SetIntCst(c, &bgbcc_rcst_Shnext, "$hnext", j);
	}

#else

//	ctx->types=BCCX_FetchCst(l, &bgbcc_rcst_types, "types");
//	ctx->structs=BCCX_FetchCst(l, &bgbcc_rcst_structs, "structs");

#if 1
	for(i=0; i<256; i++)
		ctx->struct_hash[i]=NULL;
//	c=ctx->structs;
	c=BCCX_Child(ctx->structs);

//	c=BCCX_FetchCst(l, &bgbcc_rcst_structs, "structs");

	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");

		if(!strcmp(s, "bot_matchvariable_s"))
			{ i=-1; }

		i=BGBCC_CCXL_HashName(s)&255;
		c->hnext=ctx->struct_hash[i];
		ctx->struct_hash[i]=c;
//		c=c->next;
		c=BCCX_Next(c);
	}
#endif

#endif


	BGBCC_CCXL_CompileStructs(ctx);

//	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//	while(c)
//	{
//		BGBCC_CCXL_CompileTopStatement(ctx, c);
//		c=BCCX_Next(c);
//	}

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileTopStatement(ctx, c);
	}

#if 0
	while(ctx->dynobj)
	{
		c=ctx->dynobj;
		ctx->dynobj=NULL;
		ctx->dynobj_e=NULL;
		while(c)
		{
			BGBCC_CCXL_CompileTopStatement(ctx, c);
			c=BCCX_Next(c);
		}
	}
#endif

#if 1
	while(ctx->dynobj_stkpos>0)
	{
		c=ctx->dynobj_stk[--ctx->dynobj_stkpos];
		BGBCC_CCXL_CompileTopStatement(ctx, c);
	}
#endif
	
	if(ctx->static_init)
	{
		i=ctx->static_init_seq++;
		sprintf(tb, "__prestart_init!%d", i);
		s=bgbcc_strdup(tb);
	
		n1=BCCX_New("type");
		BCCX_SetCst(n1, &bgbcc_rcst_name, "name", "void");

		BGBCC_CCXL_CompileBlock(ctx, n1, s, NULL,
			ctx->static_init);
	}
	
	BGBCC_CCXL_PrintTagWarn(ctx);

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(-1);
	}

	return(0);
}

#if 0
char *BGBCC_CCXL_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_TransState *ctx;
	BCCX_Node *c;
	char *s;
	int i;

	ctx=bgbcc_malloc(sizeof(BGBCC_TransState));
	memset(ctx, 0, sizeof(BGBCC_TransState));

#if 0
	ctx->n_warn=0;
	ctx->n_error=0;
	ctx->ccxl_top_only=0;
	ctx->n_imp=0;
	ctx->static_init=NULL;

	s=BCCX_GetCst(l, &bgbcc_rcst_lang, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_GetCst(l, &bgbcc_rcst_arch, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
	ctx->tuidx=i;
	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_TUIDX, i);

	ctx->types=BCCX_FetchCst(l, &bgbcc_rcst_types, "types");
	ctx->structs=BCCX_FetchCst(l, &bgbcc_rcst_structs, "structs");

	BGBCC_CCXL_CompileStructs(ctx);

	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
	while(c)
	{
		BGBCC_CCXL_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_PrintTagWarn(ctx);

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(NULL);
	}
#endif

//	return(ctx->ips);
//	return(NULL);
}
#endif


int BGBCC_CCXL_CompileModuleTopOnlyCTX(
	BGBCC_TransState *ctx, char *name, BCCX_Node *l)
{
	BCCX_Node *c, *n;
	char *s;
	int i, j, ci, na;

	ctx->n_warn=0;
	ctx->n_error=0;
	ctx->ccxl_top_only=1;
	ctx->n_imp=0;
	
	ctx->static_init=NULL;

	s=BCCX_GetCst(l, &bgbcc_rcst_lang, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_GetCst(l, &bgbcc_rcst_arch, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
	ctx->tuidx=i;
	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_TUIDX, i);

	ctx->types=BCCX_FindTagCst(l, &bgbcc_rcst_types, "types");
	ctx->structs=BCCX_FindTagCst(l, &bgbcc_rcst_structs, "structs");

#ifdef BGBCC_BCCX2

	for(i=0; i<256; i++)
		ctx->struct_hash_ix[i]=-1;

	na=BCCX_GetNodeChildCount(ctx->structs);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->structs, ci);
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		i=BGBCC_CCXL_HashName(s)&255;
		j=ctx->struct_hash_ix[i];
		ctx->struct_hash_ix[i]=ci;
		BCCX_SetIntCst(c, &bgbcc_rcst_Shnext, "$hnext", j);
	}

#else

//	ctx->types=BCCX_FetchCst(l, &bgbcc_rcst_types, "types");
//	ctx->structs=BCCX_FetchCst(l, &bgbcc_rcst_structs, "structs");

#if 1
	for(i=0; i<256; i++)
		ctx->struct_hash[i]=NULL;
	c=ctx->structs;
	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");

		if(!strcmp(s, "bot_matchvariable_s"))
			{ i=-1; }

		i=BGBCC_CCXL_HashName(s)&255;
		c->hnext=ctx->struct_hash[i];
		ctx->struct_hash[i]=c;
		c=c->next;
//		c=BCCX_Next(c);
	}
#endif

#endif

	BGBCC_CCXL_CompileStructs(ctx);

#if 0
	c=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
	while(c)
	{
		BGBCC_CCXL_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}
#endif

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileTopStatement(ctx, c);
	}

#if 0
	while(ctx->dynobj)
	{
		c=ctx->dynobj;
		ctx->dynobj=NULL;
		ctx->dynobj_e=NULL;
		while(c)
		{
			BGBCC_CCXL_CompileTopStatement(ctx, c);
			c=BCCX_Next(c);
		}
	}
#endif

#if 1
	while(ctx->dynobj_stkpos>0)
	{
		c=ctx->dynobj_stk[--ctx->dynobj_stkpos];
		BGBCC_CCXL_CompileTopStatement(ctx, c);
	}
#endif

	BGBCC_CCXL_PrintTagWarn(ctx);

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(-1);
	}

	ctx->ccxl_top_only=0;

	return(0);
}
