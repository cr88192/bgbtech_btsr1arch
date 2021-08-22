#include <bgbccc.h>

BGBCC_CCXL_VirtOp *BGBCC_CCXL_AllocVirtOp(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtOp *tmp, *tmpa;
	int i;
	
	tmp=ctx->virtop_free;
	if(tmp)
	{
		ctx->virtop_free=*(BGBCC_CCXL_VirtOp **)tmp;
		memset(tmp, 0, sizeof(BGBCC_CCXL_VirtOp));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(256*sizeof(BGBCC_CCXL_VirtOp));
	for(i=0; i<255; i++)
	{
		*(BGBCC_CCXL_VirtOp **)tmp=ctx->virtop_free;
		ctx->virtop_free=tmp++;
	}

	memset(tmp, 0, sizeof(BGBCC_CCXL_VirtOp));
	return(tmp);
}

BGBCC_CCXL_VirtTr *BGBCC_CCXL_AllocVirtTr(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtTr *tmp, *tmpa;
	int i;
	
	tmp=ctx->virttr_free;
	if(tmp)
	{
		ctx->virttr_free=*(BGBCC_CCXL_VirtTr **)tmp;
		memset(tmp, 0, sizeof(BGBCC_CCXL_VirtTr));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(256*sizeof(BGBCC_CCXL_VirtTr));
	for(i=0; i<255; i++)
	{
		*(BGBCC_CCXL_VirtTr **)tmp=ctx->virttr_free;
		ctx->virttr_free=tmp++;
	}

	memset(tmp, 0, sizeof(BGBCC_CCXL_VirtTr));
	return(tmp);
}

BGBCC_CCXL_VirtOp *BGBCC_CCXL_CloneVirtOp(BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *vop)
{
	BGBCC_CCXL_VirtOp *tmp;
	tmp=BGBCC_CCXL_AllocVirtOp(ctx);
	memcpy(tmp, vop, sizeof(BGBCC_CCXL_VirtOp));
	return(tmp);
}

int BGBCC_CCXL_AddVirtOp(BGBCC_TransState *ctx, BGBCC_CCXL_VirtOp *op)
{
	int i;

	if(!ctx->vop)
	{
		ctx->vop=bgbcc_malloc(4096*sizeof(BGBCC_CCXL_VirtOp *));
		ctx->n_vop=0;
		ctx->m_vop=4096;
	}

	if((ctx->n_vop+1)>(ctx->m_vop))
	{
		i=ctx->m_vop+(ctx->m_vop>>1);
		ctx->vop=bgbcc_realloc(ctx->vop,
			i*sizeof(BGBCC_CCXL_VirtOp *));
//		ctx->n_vop=0;
		ctx->m_vop=i;
	}

	i=ctx->n_vop++;
	ctx->vop[i]=op;
	return(i);
}

int BGBCC_CCXL_AddVirtTr(BGBCC_TransState *ctx, BGBCC_CCXL_VirtTr *tr)
{
	int i;

	if(!ctx->vtr)
	{
		ctx->vtr=bgbcc_malloc(256*sizeof(BGBCC_CCXL_VirtOp *));
		ctx->n_vtr=0;
		ctx->m_vtr=256;
	}

	if((ctx->n_vtr+1)>(ctx->m_vtr))
	{
		i=ctx->m_vtr+(ctx->m_vtr>>1);
		ctx->vtr=bgbcc_realloc(ctx->vtr,
			i*sizeof(BGBCC_CCXL_VirtTr *));
//		ctx->n_vtr=0;
		ctx->m_vtr=i;
	}

	i=ctx->n_vtr++;
	ctx->vtr[i]=tr;
	return(i);
}

int BGBCC_CCXL_EmitMarkEndTrace(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtTr *tr;
	int i, n;

	n=ctx->n_vop-ctx->s_vop;
	if(!n)
		return(-1);

	if(n<0)
		{ BGBCC_DBGBREAK }
	
	tr=BGBCC_CCXL_AllocVirtTr(ctx);

	tr->b_ops=ctx->s_vop;
	tr->n_ops=ctx->n_vop-tr->b_ops;
	ctx->s_vop=ctx->n_vop;

	i=BGBCC_CCXL_AddVirtTr(ctx, tr);
	return(i);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable_VReg(BGBCC_TransState *ctx,
	ccxl_register reg, int afl)
{
	BGBCC_CCXL_RegisterInfo *rfn;
	BGBCC_CCXL_LiteralInfo *litobj;
	int n;
	int i;

//	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) ||
//		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) ||
//		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL))
//			return(0);

	if(BGBCC_CCXL_IsRegNonReachTraceP(ctx, reg))
		return(0);

	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
		BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		if(i<=0)
			return(0);
		
		rfn=ctx->reg_globals[i];
		if(!rfn)
			return(0);
		
		BGBCC_CCXL_GlobalMarkReachableB(ctx, rfn, afl);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmLiteralP(ctx, reg))
	{
		i=reg.val&CCXL_REGINT_MASK;
		litobj=ctx->literals[i];

		if(litobj->decl && litobj->decl->listdata)
		{
			n=litobj->decl->n_listdata;
			for(i=0; i<n; i++)
			{
				BGBCC_CCXL_GlobalMarkReachable_VReg(ctx,
					litobj->decl->listdata[i], afl|1);
			}
		}
		return(1);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
		return(0);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
		return(0);

	return(0);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable_Func(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_register *args;
	ccxl_register treg;
	int n;
	int i, j;

//	if(!strcmp(obj->name, "tk_fat_init"))
//		j=-1;
//	if(!strcmp(obj->name, "tk_vfile_init"))
//		j=-1;

	for(i=0; i<obj->n_vop; i++)
	{
		op=obj->vop[i];
		
		if(op->opn==CCXL_VOP_CALL)
		{
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->dst, 1);
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srca, 0);
//			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srca, 1);
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srcb, 0);
		
			n=op->imm.call.na;
			args=op->imm.call.args;

			for(j=0; j<n; j++)
			{
				treg=args[j];
				BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, treg, 1);
			}
		}else
		{
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->dst, 1);
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srca, 1);
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srcb, 1);
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srcc, 1);
			BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srcd, 1);
		}
	}
	return(1);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	return(BGBCC_CCXL_GlobalMarkReachableB(ctx, obj, 0));
}

ccxl_status BGBCC_CCXL_GlobalMarkReachableB(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj, int afl)
{
	BGBCC_CCXL_RegisterInfo *objarr[64];
	BGBCC_CCXL_RegisterInfo *objlst, *objcur;
	int i, n;

	if(obj->regflags&BGBCC_REGFL_RECTRACE)
	{
		obj->gblrefcnt++;
		return(0);
	}
	
	if(obj->regflags&BGBCC_REGFL_IFACULL)
	{
		obj->regflags|=BGBCC_REGFL_RECTRACE;

		objlst=BGBCC_CCXL_LookupGlobalList(ctx, obj->qname);
		objcur=objlst; n=0;
		while(objcur)
			{ objarr[n++]=objcur; objcur=objcur->chain; }
		for(i=0; i<n; i++)
			{ BGBCC_CCXL_GlobalMarkReachable(ctx, objarr[i]); }

		return(0);
	}

	obj->regflags|=BGBCC_REGFL_RECTRACE;
	obj->regflags|=BGBCC_REGFL_ACCESSED;
	obj->regflags&=~BGBCC_REGFL_CULL;
	obj->gblrefcnt++;

	if(obj->regtype==CCXL_LITID_FUNCTION)
	{
		if((obj->flagsint&BGBCC_TYFL_DLLEXPORT) || (afl&1))
		{
			obj->regflags|=BGBCC_REGFL_ALIASPTR;
		}
	}

//	if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
	if(obj->regtype==CCXL_LITID_FUNCTION)
	{
		BGBCC_CCXL_GlobalMarkReachable_Func(ctx, obj);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}

	if(obj->regtype==CCXL_LITID_GLOBALVAR)
	{
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, obj->value, 1);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}

	if(obj->regtype==CCXL_LITID_STATICVAR)
	{
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, obj->value, 1);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}

	if(obj->regtype==CCXL_LITID_ASMFUNC)
	{
		return(1);
	}
	
//	obj->regflags&=~BGBCC_REGFL_RECTRACE;
	return(0);
}

s64 BGBCC_CCXL_DecodeFlagStr(BGBCC_TransState *ctx, char *str)
{
	s64 fl;
	char *s;
	int i;
	
	fl=0;
	s=str;
	while(*s)
	{
		i=*s++;
		switch(i)
		{
		case 'a':	fl|=BGBCC_TYFL_ABSTRACT;	break;
		case 'b':	fl|=BGBCC_TYFL_BIGENDIAN;	break;
		case 'c':	fl|=BGBCC_TYFL_CDECL;		break;
		case 'd':	fl|=BGBCC_TYFL_DELEGATE;	break;
		case 'e':	fl|=BGBCC_TYFL_EXTERN;		break;

		case 'i':	fl|=BGBCC_TYFL_INLINE;		break;
		case 'j':	fl|=BGBCC_TYFL_FINAL;		break;
		case 'k':	fl|=BGBCC_TYFL_CONST;		break;
		case 'l':	fl|=BGBCC_TYFL_LTLENDIAN;	break;
		case 'm':	fl|=BGBCC_TYFL_PACKED;		break;
		case 'n':	fl|=BGBCC_TYFL_NATIVE;		break;
		case 'o':	fl|=BGBCC_TYFL_OVERRIDE;	break;
		case 'p':	fl|=BGBCC_TYFL_PUBLIC;		break;
		case 'q':	fl|=BGBCC_TYFL_PRIVATE;		break;
		case 'r':	fl|=BGBCC_TYFL_PROTECTED;	break;
		case 's':	fl|=BGBCC_TYFL_STATIC;		break;

		case 'v':	fl|=BGBCC_TYFL_VIRTUAL;		break;
		case 'w':	fl|=BGBCC_TYFL_STDCALL;		break;
		case 'x':	fl|=BGBCC_TYFL_XCALL;		break;
		case 'y':	fl|=BGBCC_TYFL_TRANSIENT;	break;

		case 'C':
			i=*s++;
			switch(i)
			{
			case 'b':	fl|=BGBCC_TYFL_BYREF;			break;
			case 'e':	fl|=BGBCC_TYFL_EVENT;			break;
			case 'f':	fl|=BGBCC_TYFL_INTERRUPT;		break;
			case 'g':	fl|=BGBCC_TYFL_SYSCALL;			break;
			case 'i':	fl|=BGBCC_TYFL_INTERFACE;		break;
			case 's':	fl|=BGBCC_TYFL_SYNCHRONIZED;	break;
			case 'x':	fl|=BGBCC_TYFL_NEAR;			break;
			case 'y':	fl|=BGBCC_TYFL_FAR;				break;
			default:
				break;
			}
			break;
		case 'D':
			i=*s++;
			switch(i)
			{
			case 'a':	fl|=BGBCC_TYFL_IFARCH;			break;
			case 'b':	fl|=BGBCC_TYFL_IFNARCH;			break;
			case 'c':	fl|=BGBCC_TYFL_NOCULL;			break;
			case 'f':	fl|=BGBCC_TYFL_FENVACC;			break;

			case 'e':	fl|=BGBCC_TYFL_DLLEXPORT;		break;
			case 'i':	fl|=BGBCC_TYFL_DLLIMPORT;		break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	return(fl);
}
