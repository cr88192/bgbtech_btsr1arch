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
	ccxl_register reg)
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
		
		BGBCC_CCXL_GlobalMarkReachable(ctx, rfn);
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
					litobj->decl->listdata[i]);
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
		
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->dst);
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srca);
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, op->srcb);
		
		if(op->opn==CCXL_VOP_CALL)
		{
			n=op->imm.call.na;
			args=op->imm.call.args;

			for(j=0; j<n; j++)
			{
				treg=args[j];
				BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, treg);
			}
		}
	}
	return(1);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	if(obj->regflags&BGBCC_REGFL_RECTRACE)
	{
		return(0);
	}

	obj->regflags|=BGBCC_REGFL_RECTRACE;
	obj->regflags|=BGBCC_REGFL_ACCESSED;
	obj->regflags&=~BGBCC_REGFL_CULL;

//	if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
	if(obj->regtype==CCXL_LITID_FUNCTION)
	{
		BGBCC_CCXL_GlobalMarkReachable_Func(ctx, obj);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}

	if(obj->regtype==CCXL_LITID_GLOBALVAR)
	{
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, obj->value);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}

	if(obj->regtype==CCXL_LITID_STATICVAR)
	{
		BGBCC_CCXL_GlobalMarkReachable_VReg(ctx, obj->value);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}
	
//	obj->regflags&=~BGBCC_REGFL_RECTRACE;
	return(0);
}
