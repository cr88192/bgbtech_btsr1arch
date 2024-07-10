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
	BGBCC_CCXL_VirtOp *op1;
	BGBCC_CCXL_VirtTr *tr;
	int i, b, n, fn;

	b=ctx->s_vop;
	n=ctx->n_vop-ctx->s_vop;
	if(!n)
		return(-1);

	if(n<0)
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_LoadslotCacheFlush(ctx);

	tr=BGBCC_CCXL_AllocVirtTr(ctx);

	tr->b_ops=ctx->s_vop;
	tr->n_ops=ctx->n_vop-tr->b_ops;
	ctx->s_vop=ctx->n_vop;

	fn=0;
	for(i=0; i<n; i++)
	{
		op1=ctx->vop[b+i];
		if(op1->opn==CCXL_VOP_TEMP_PHI)
			continue;
#if 0
		if(op1->opn==CCXL_VOP_LABEL)
			continue;
		if(op1->opn==CCXL_VOP_DBGLN)
			continue;
		if(op1->opn==CCXL_VOP_DBGFN)
			continue;
#endif
		fn++;
	}
	
	tr->n_fops=fn;

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

ccxl_status BGBCC_CCXL_GlobalMarkReachable_BinaryOp(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0, *s1, *s2, *s3;
	int i, j, k;
	int opr;

	s0=NULL;
	s1=NULL;
	s2=NULL;
	s3=NULL;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_MUL:
//			s0="__lva_mul";
			break;

		case CCXL_BINOP_DIV:
			s0="__sdivsi3";
			s1="__udivsi3";
			s2="__udivsi3_tab";
			break;
		case CCXL_BINOP_MOD:
			s0="__smodsi3";
			s1="__umodsi3";
			break;

//		case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
//		case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
		}
	}else
		if(BGBCC_CCXL_TypeSmallLongP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_MUL:
			s0="__smuldq";
			s1="__umuldq";
			break;

		case CCXL_BINOP_DIV:
			s0="__sdivdi3";
			s1="__udivdi3";
			break;
		case CCXL_BINOP_MOD:
			s0="__smoddi3";
			s1="__umoddi3";
			break;

//		case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
//		case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
		}
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_ADD:	s0="__lva_add"; break;
		case CCXL_BINOP_SUB:	s0="__lva_sub"; break;

		case CCXL_BINOP_AND:	s0="__lva_and"; break;
		case CCXL_BINOP_OR:		s0="__lva_or"; break;
		case CCXL_BINOP_XOR:	s0="__lva_xor"; break;

		case CCXL_BINOP_MUL:	s0="__lva_mul"; break;
		case CCXL_BINOP_DIV:	s0="__lva_div"; break;
		case CCXL_BINOP_MOD:	s0="__lva_mod"; break;

		case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
		case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
		}
	}
	
	if(BGBCC_CCXL_TypeSgInt128P(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_ADD:	s0="__xli_add"; break;
		case CCXL_BINOP_SUB:	s0="__xli_sub"; break;

		case CCXL_BINOP_AND:	s0="__xli_and"; break;
		case CCXL_BINOP_OR:		s0="__xli_or"; break;
		case CCXL_BINOP_XOR:	s0="__xli_xor"; break;

		case CCXL_BINOP_MUL:
			s0="__xli_umul";
			s1="__xli_smul";
			break;
		case CCXL_BINOP_DIV:
			s0="__xli_udiv";
			s1="__xli_sdiv";
			break;
		case CCXL_BINOP_MOD:
			s0="__xli_umod";
			s1="__xli_smod";
			break;

		case CCXL_BINOP_SHL:	s0="__xli_shl"; break;
		case CCXL_BINOP_SHR:
			s0="__xli_shlr";
			s1="__xli_shar";
			break;
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_DIV:
			s0="__fpu_fdiv_s";
			s1="__fpu_fdiv_sf";
			s2="__fpu_frcp_s";
			s3="__fpu_frcp_sf";
			break;
		}
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_DIV:
			s0="__fpu_fdiv";
			s1="__fpu_frcp";
			break;
		}
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_ADD:	s0="__xlf_add"; break;
		case CCXL_BINOP_SUB:	s0="__xlf_sub"; break;
		case CCXL_BINOP_MUL:	s0="__xlf_mul"; break;
		case CCXL_BINOP_DIV:	s0="__xlf_div"; break;
		}
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_BINOP_ADD:	s0="__xbi_add"; break;
		case CCXL_BINOP_SUB:	s0="__xbi_sub"; break;

		case CCXL_BINOP_AND:	s0="__xbi_and"; break;
		case CCXL_BINOP_OR:		s0="__xbi_or"; break;
		case CCXL_BINOP_XOR:	s0="__xbi_xor"; break;

		case CCXL_BINOP_MUL:
			s0="__xbi_umul";
			s1="__xbi_smul";
			break;
		case CCXL_BINOP_DIV:
			s0="__xbi_udiv";
			s1="__xbi_sdiv";
			break;
		case CCXL_BINOP_MOD:
			s0="__xbi_umod";
			s1="__xbi_smod";
			break;

		case CCXL_BINOP_SHL:	s0="__xbi_shl"; break;
		case CCXL_BINOP_SHR:
			s0="__xbi_shlr";
			s1="__xbi_shar";
			break;
		}
	}

	
	if(BGBCC_CCXL_TypeVecP(ctx, op->type))
	{
		i=BGBCC_CCXL_GetTypeBaseType(ctx, op->type);
		opr=op->opr;
		
		if(op->opn==CCXL_VOP_UNARY)
		{
			if(op->opr==CCXL_UNOP_MOV)
				opr=CCXL_BINOP_ADD;
			if(op->opr==CCXL_UNOP_NEG)
				opr=CCXL_BINOP_SUB;
		}
		
		switch(i)
		{
		case CCXL_TY_VEC2F:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v2f_add";		break;
			case CCXL_BINOP_SUB:	s0="__vnf_v2f_sub";		break;
			case CCXL_BINOP_MUL:	s0="__vnf_v2f_mul";		break;
			case CCXL_BINOP_DIV:	s0="__vnf_v2f_div";		break;
			case CCXL_BINOP_XOR:	s0="__vnf_v2f_dot";		break;
			case CCXL_BINOP_MOD:	s0="__vnf_v2f_cross";	break;
			}
			break;
		case CCXL_TY_VEC3F:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v3f_add";		break;
			case CCXL_BINOP_SUB:	s0="__vnf_v3f_sub";		break;
			case CCXL_BINOP_MUL:	s0="__vnf_v3f_mul";		break;
			case CCXL_BINOP_DIV:	s0="__vnf_v3f_div";		break;
			case CCXL_BINOP_XOR:	s0="__vnf_v3f_dot";		break;
			case CCXL_BINOP_MOD:	s0="__vnf_v3f_cross";	break;
			}
			break;
		case CCXL_TY_VEC4F:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v4f_add";		break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4f_sub";		break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4f_mul";		break;
			case CCXL_BINOP_DIV:	s0="__vnf_v4f_div";		break;
			case CCXL_BINOP_XOR:	s0="__vnf_v4f_dot";		break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4f_cross";	break;
			}
			break;
		case CCXL_TY_VEC4H:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v4h_add";		break;
			case CCXL_BINOP_SUB:	s0="__vnf_v4h_sub";		break;
			case CCXL_BINOP_MUL:	s0="__vnf_v4h_mul";		break;
			case CCXL_BINOP_DIV:	s0="__vnf_v4h_div";		break;
			case CCXL_BINOP_XOR:	s0="__vnf_v4h_dot";		break;
			case CCXL_BINOP_MOD:	s0="__vnf_v4h_cross";	break;
			}
			break;

		case CCXL_TY_VEC2D:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v2d_add";		break;
			case CCXL_BINOP_SUB:	s0="__vnf_v2d_sub";		break;
			case CCXL_BINOP_MUL:	s0="__vnf_v2d_mul";		break;
			case CCXL_BINOP_DIV:	s0="__vnf_v2d_div";		break;
			case CCXL_BINOP_XOR:	s0="__vnf_v2d_dot";		break;
			case CCXL_BINOP_MOD:	s0="__vnf_v2d_cross";	break;
			}
			break;

		case CCXL_TY_VEC3FQ:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v3fq_add";	break;
			case CCXL_BINOP_SUB:	s0="__vnf_v3fq_sub";	break;
			case CCXL_BINOP_MUL:	s0="__vnf_v3fq_mul";	break;
			case CCXL_BINOP_DIV:	s0="__vnf_v3fq_div";	break;
			case CCXL_BINOP_XOR:	s0="__vnf_v3fq_dot";	break;
			case CCXL_BINOP_MOD:	s0="__vnf_v3fq_cross";	break;
			}
			break;
		case CCXL_TY_VEC3FX:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_v3fx_add";	break;
			case CCXL_BINOP_SUB:	s0="__vnf_v3fx_sub";	break;
			case CCXL_BINOP_MUL:	s0="__vnf_v3fx_mul";	break;
			case CCXL_BINOP_DIV:	s0="__vnf_v3fx_div";	break;
			case CCXL_BINOP_XOR:	s0="__vnf_v3fx_dot";	break;
			case CCXL_BINOP_MOD:	s0="__vnf_v3fx_cross";	break;
			}
			break;

		case CCXL_TY_FCOMPLEX:
		case CCXL_TY_FIMAG:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_c2f_add";	break;
			case CCXL_BINOP_SUB:	s0="__vnf_c2f_sub";	break;
			case CCXL_BINOP_MUL:	s0="__vnf_c2f_mul";	break;
			case CCXL_BINOP_DIV:	s0="__vnf_c2f_div";	break;
			}
			break;
		case CCXL_TY_DCOMPLEX:
		case CCXL_TY_DIMAG:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_c2d_add";	break;
			case CCXL_BINOP_SUB:	s0="__vnf_c2d_sub";	break;
			case CCXL_BINOP_MUL:	s0="__vnf_c2d_mul";	break;
			case CCXL_BINOP_DIV:	s0="__vnf_c2d_div";	break;
			}
			break;

		case CCXL_TY_QUATF:
			switch(opr)
			{
			case CCXL_BINOP_ADD:	s0="__vnf_vqf_add"; break;
			case CCXL_BINOP_SUB:	s0="__vnf_vqf_sub"; break;
			case CCXL_BINOP_MUL:	s0="__vnf_vqf_mul"; break;
			case CCXL_BINOP_DIV:	s0="__vnf_vqf_div"; break;
			case CCXL_BINOP_XOR:	s0="__vnf_vqf_dot"; break;
			case CCXL_BINOP_MOD:	s0="__vnf_vqf_cross"; break;
			}
			break;
		}
	
	}
	
	if(s0)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s0);
	}

	if(s1)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s1);
	}

	if(s2)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s2);
	}

	if(s3)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s3);
	}

	return(0);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable_UnaryOp(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0, *s1;

	if(BGBCC_CCXL_TypeVecP(ctx, op->type))
	{
		BGBCC_CCXL_GlobalMarkReachable_BinaryOp(ctx, obj, op);
		return(0);
	}

	s0=NULL;
	s1=NULL;
	if(BGBCC_CCXL_TypeVariantP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_UNOP_NEG:		s0="__lva_neg"; break;
		case CCXL_UNOP_NOT:		s0="__lva_not"; break;
		case CCXL_UNOP_LNOT:	s0="__lva_lnot"; break;
		case CCXL_UNOP_INC:		s0="__lva_inc"; break;
		case CCXL_UNOP_DEC:		s0="__lva_dec"; break;
		}
	}
	
	if(BGBCC_CCXL_TypeSgInt128P(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_UNOP_NEG:		s0="__xli_neg"; break;
		case CCXL_UNOP_NOT:		s0="__xli_not"; break;
		case CCXL_UNOP_LNOT:	s0="__xli_lnot"; break;
		case CCXL_UNOP_INC:		s0="__xli_inc"; break;
		case CCXL_UNOP_DEC:		s0="__xli_dec"; break;
		}
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_UNOP_NEG:		s0="__xlf_neg"; break;
		}
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_UNOP_NEG:		s0="__xbi_neg"; break;
		case CCXL_UNOP_NOT:		s0="__xbi_not"; break;
		case CCXL_UNOP_LNOT:	s0="__xbi_lnot"; break;
		case CCXL_UNOP_INC:		s0="__xbi_inc"; break;
		case CCXL_UNOP_DEC:		s0="__xbi_dec"; break;
		}
	}
	
	if(s0)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s0);
	}

	if(s1)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s1);
	}

	return(0);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable_CompareOp(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0, *s1;

	s0=NULL;
	s1=NULL;
	if(BGBCC_CCXL_TypeVariantP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_CMP_EQ:		s0="__lva_cmp_eq"; break;
		case CCXL_CMP_NE:		s0="__lva_cmp_eq"; break;
		case CCXL_CMP_LT:		s0="__lva_cmp_gt"; break;
		case CCXL_CMP_GT:		s0="__lva_cmp_gt"; break;
		case CCXL_CMP_LE:		s0="__lva_cmp_gt"; break;
		case CCXL_CMP_GE:		s0="__lva_cmp_gt"; break;
		case CCXL_CMP_TST:		s0="__lva_cmp_tst"; break;
		case CCXL_CMP_NTST:		s0="__lva_cmp_tst"; break;
		}
	}
	
	if(BGBCC_CCXL_TypeSgInt128P(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_CMP_EQ:		s0="__xli_cmp_eq"; break;
		case CCXL_CMP_NE:		s0="__xli_cmp_ne"; break;

		case CCXL_CMP_LT:		case CCXL_CMP_GT:
				s0="__xli_cmp_gt";
				s1="__xli_cmp_hi";
				break;

		case CCXL_CMP_LE:		case CCXL_CMP_GE:
				s0="__xli_cmp_ge";
				s1="__xli_cmp_he";
				break;
		}
	}
	
	if(BGBCC_CCXL_TypeFloat128P(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_CMP_EQ:		s0="__xlf_cmp_eq"; break;
		case CCXL_CMP_NE:		s0="__xlf_cmp_ne"; break;
		case CCXL_CMP_LT:		s0="__xlf_cmp_gt"; break;
		case CCXL_CMP_GT:		s0="__xlf_cmp_gt"; break;
		case CCXL_CMP_LE:		s0="__xlf_cmp_ge"; break;
		case CCXL_CMP_GE:		s0="__xlf_cmp_ge"; break;
		}
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, op->type))
	{
		switch(op->opr)
		{
		case CCXL_CMP_EQ:		s0="__xbi_cmp_eq"; break;
		case CCXL_CMP_NE:		s0="__xbi_cmp_ne"; break;

		case CCXL_CMP_LT:		case CCXL_CMP_GT:
				s0="__xbi_cmp_gt";
				s1="__xbi_cmp_hi";
				break;

		case CCXL_CMP_LE:		case CCXL_CMP_GE:
				s0="__xbi_cmp_ge";
				s1="__xbi_cmp_he";
				break;
		}
	}
	
	if(s0)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s0);
	}

	if(s1)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s1);
	}

	return(0);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable_ConvOp(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0, *s1;

	s0=NULL;
	s1=NULL;

	if(BGBCC_CCXL_TypeVariantP(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, op->stype))
		{
			s0="__lva_fromi32";
			s1="__lva_fromi64";
		}

		if(BGBCC_CCXL_TypeSgInt128P(ctx, op->stype))
		{
			s0="__lva_fromi128";
		}

		if(BGBCC_CCXL_TypeRealP(ctx, op->stype))
		{
			s0="__lva_fromf32";
			s1="__lva_fromf64";
		}

		if(BGBCC_CCXL_TypeFloat128P(ctx, op->stype))
		{
			s0="__lva_fromf128";
		}

		if(BGBCC_CCXL_TypePointerP(ctx, op->stype))
		{
			s0="__lva_conv_fromptr";

			if(BGBCC_CCXL_TypeCStringP(ctx, op->stype))
				{ s1="__lva_conv_fromstr"; }
			if(BGBCC_CCXL_TypeCWStringP(ctx, op->stype))
				{ s1="__lva_conv_fromwstr"; }
		}
	}

	if(BGBCC_CCXL_TypePointerP(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeVariantP(ctx, op->stype))
		{
			if(BGBCC_CCXL_TypeCWStringP(ctx, op->type))
				{ s0="__lva_conv_towstr"; }
			else if(BGBCC_CCXL_TypeCStringP(ctx, op->type))
				{ s0="__lva_conv_tostr"; }
			else
				{ s0="__lva_conv_toptr"; }
		}
		if(BGBCC_CCXL_TypeRefArrayP(ctx, op->stype))
		{
			s0="__lvo_arraygetptr";
		}
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeBcd64P(ctx, op->stype))
			{ s0="__bcd64_toint"; }
		if(BGBCC_CCXL_TypeBcd128P(ctx, op->stype))
			{ s0="__bcd128_toint"; }

		if(BGBCC_CCXL_TypeVariantP(ctx, op->stype))
		{
			s0="__lva_conv_toi32";
			s1="__lva_conv_toi64";
		}
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeVariantP(ctx, op->stype))
			{ s0="__lva_conv_toi128"; }
		if(BGBCC_CCXL_TypeBcd128P(ctx, op->stype))
			{ s0="__bcd128_toint128"; }
	}

	if(BGBCC_CCXL_TypeRealP(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeVariantP(ctx, op->stype))
		{
			s0="__lva_conv_tof32";
			s1="__lva_conv_tof64";
		}
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeVariantP(ctx, op->stype))
		{
			s0="__lva_tof128";
		}
	}

	if(BGBCC_CCXL_TypeBcd64P(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, op->stype))
		{
			s0="__bcd64_fromint";
		}
	}
	if(BGBCC_CCXL_TypeBcd128P(ctx, op->type))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, op->stype))
		{
			s0="__bcd128_fromint";
		}
	}

	if(s0)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s0);
	}

	if(s1)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, s1);
	}

	return(0);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachable_Func(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_register *args;
	ccxl_register treg;
	char *s0, *s1;
	int n, lvl, oldmult;
	int i, j;

	oldmult=ctx->trace_mult;

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->flagsint&BGBCC_TYFL_REGISTER)
		{
			obj->regflags|=BGBCC_REGFL_GOFAST;
		}
		
		if(BGBCC_CCXL_TypeVaListP(ctx, obj->locals[i]->type))
		{
			BGBCC_CCXL_GlobalMarkReachableName(ctx, "__va64_saveargs");
			BGBCC_CCXL_GlobalMarkReachableName(ctx, "__va64_arg_l");
			BGBCC_CCXL_GlobalMarkReachableName(ctx, "__va64_arg_x");
		}
	}


//	if(!strcmp(obj->name, "tk_fat_init"))
//		j=-1;
//	if(!strcmp(obj->name, "tk_vfile_init"))
//		j=-1;

	if(!strcmp(obj->name, "vfw_begin_decompress"))
		j=-1;

	lvl=0;
	for(i=0; i<obj->n_vop; i++)
	{
		op=obj->vop[i];

		if(op->opn==CCXL_VOP_LABEL)
		{
			lvl=op->llvl;
			
			if(obj->regflags&BGBCC_REGFL_GOFAST)
				lvl+=2;
			
			ctx->trace_mult=lvl;
		}else
		{
			op->tgt_mult=lvl;
			ctx->trace_mult=lvl;
		}

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

		if(op->opn==CCXL_VOP_BINARY)
		{
			BGBCC_CCXL_GlobalMarkReachable_BinaryOp(ctx, obj, op);
		}

		if(op->opn==CCXL_VOP_UNARY)
		{
			BGBCC_CCXL_GlobalMarkReachable_UnaryOp(ctx, obj, op);
		}

		if(	(op->opn==CCXL_VOP_COMPARE)	||
			(op->opn==CCXL_VOP_JCMP) ||
			(op->opn==CCXL_VOP_JCMP_ZERO))
		{
			BGBCC_CCXL_GlobalMarkReachable_CompareOp(ctx, obj, op);
		}

		if(op->opn==CCXL_VOP_CONV)
		{
			BGBCC_CCXL_GlobalMarkReachable_ConvOp(ctx, obj, op);
		}
	}

	ctx->trace_mult=oldmult;
	return(1);
}

ccxl_status BGBCC_CCXL_GlobalMarkReachableAsmBlob(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj, char *name)
{
	char *cs, *tk0, *tk1, *tk2;
	int i, j, k;
	
	if(obj->regflags&BGBCC_REGFL_RECTRACE)
	{
		i=ctx->trace_mult;
		obj->gblrefcnt+=1+4*(i*i);
		return(0);
	}
	
	cs=(char *)(obj->text);
	while(cs && *cs)
	{
		cs=BGBCC_JX2A_ParseTokenAlt(cs, &tk0);
		if(!cs || !(*cs))
			break;
		
		if(!strcmp(tk0, "I.global") ||
			!strcmp(tk0, "I.globl") )
		{
			cs=BGBCC_JX2A_ParseTokenAlt(cs, &tk1);
			if((tk1[0]=='I') && !strcmp(tk1+1, name))
				break;
		}
		
		if((tk0[0]=='I') && (cs[0]==':'))
		{
			if(!strcmp(tk0+1, name))
				break;
		}
	}

	if(!cs || !*cs)
		return(0);

	obj->regflags|=BGBCC_REGFL_RECTRACE;
	obj->regflags|=BGBCC_REGFL_ACCESSED;
	obj->regflags&=~BGBCC_REGFL_CULL;

	cs=(char *)(obj->text);
	while(cs && *cs)
	{
		cs=BGBCC_JX2A_ParseTokenAlt(cs, &tk0);
		if(!cs || !(*cs))
			break;

		if(!strcmp(tk0, "I.extern") ||
			!strcmp(tk0, "I.weak") )
		{
			cs=BGBCC_JX2A_ParseTokenAlt(cs, &tk1);
			if(tk1[0]=='I')
			{
				tk2=bgbcc_strdup(tk1+1);
				BGBCC_CCXL_GlobalMarkReachableName(ctx, tk2);
			}
		}
	}

	return(1);
}

static char *bgbcc_ccxl_reachhash[4096];

ccxl_status BGBCC_CCXL_GlobalMarkReachableName(BGBCC_TransState *ctx,
	char *name)
{
//	char tb[256];
	BGBCC_CCXL_RegisterInfo *obj;
	char *cs, *ct;
	int i, j, k, h;

	if(!name || !(*name))
		return(0);

	/* If name has already been marked, avoid trying to do so again. */
	h=BGBCC_CCXL_HashName(name)&4095;
	if(bgbcc_ccxl_reachhash[h] && !strcmp(bgbcc_ccxl_reachhash[h], name))
		return(0);
	bgbcc_ccxl_reachhash[h]=bgbcc_strdup(name);

#if 0
	cs=name;
	while(*cs && (*cs!=' '))
		cs++;
	if(*cs)
	{
		cs=name;
		ct=tb;
		while(*cs)
		{
			while(*cs==' ')
				cs++;
			while(*cs && (*cs!=' '))
				*ct++=*cs++;
			*ct++=0;
			if(tb[0])
			{
				BGBCC_CCXL_GlobalMarkReachableName(ctx, bgbcc_strdup(tb));
			}
			ct=tb;
		}
		return(0);
	}
#endif

	if(!strncmp(name, "__lva_", 6))
	{
		k=-1;
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if(obj->qname && !strcmp(obj->qname, name))
		{
			BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
		}

		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_CCXL_GlobalMarkReachableAsmBlob(ctx, obj, name);
		}
	}

	return(0);
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
//		obj->gblrefcnt++;
		i=ctx->trace_mult;
		obj->gblrefcnt+=1+4*(i*i);
//		obj->gblrefcnt+=1+(i+(i>>1));
//		obj->gblrefcnt+=1+(i);
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
//	obj->gblrefcnt++;

	i=ctx->trace_mult;
	obj->gblrefcnt+=1+4*(i*i);
//	obj->gblrefcnt+=1+(i+(i>>1));
//	obj->gblrefcnt+=1+(i);

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
		if(!(obj->n_vop))
			BGBCC_CCXL_GlobalMarkReachableName(ctx, obj->qname);
		BGBCC_CCXL_GlobalMarkReachable_Func(ctx, obj);
//		obj->regflags&=~BGBCC_REGFL_RECTRACE;
		return(1);
	}

	if(obj->regtype==CCXL_LITID_PROTOTYPE)
	{
		BGBCC_CCXL_GlobalMarkReachableName(ctx, obj->qname);
//		BGBCC_CCXL_GlobalMarkReachable_Func(ctx, obj);
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

int BGBCC_CCXL_CheckVirtOpNoSwaps(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op)
{
	if(	(op->opn==CCXL_VOP_UNARY)			||
		(op->opn==CCXL_VOP_BINARY)			||
		(op->opn==CCXL_VOP_MOV)				||
		(op->opn==CCXL_VOP_CONV)			||

		(op->opn==CCXL_VOP_DBGFN)			||
		(op->opn==CCXL_VOP_DBGLN)			||
		(op->opn==CCXL_VOP_COMPARE)			||

#if 1
		(op->opn==CCXL_VOP_LDIXIMM)			||
		(op->opn==CCXL_VOP_STIXIMM)			||
		(op->opn==CCXL_VOP_LDIX)			||
		(op->opn==CCXL_VOP_STIX)			||
		(op->opn==CCXL_VOP_LOADSLOT)		||
		(op->opn==CCXL_VOP_STORESLOT)		||
#endif

#if 1
		(op->opn==CCXL_VOP_LEAIMM)			||
		(op->opn==CCXL_VOP_LEA)				||
#endif

#if 1
		(op->opn==CCXL_VOP_LDAVAR)			||
		(op->opn==CCXL_VOP_SIZEOFVAR)		||
		(op->opn==CCXL_VOP_DIFFPTR)			||
		(op->opn==CCXL_VOP_OFFSETOF)		||
#endif

#if 1
		(op->opn==CCXL_VOP_LOADSLOTADDR)	||
		(op->opn==CCXL_VOP_LOADSLOTADDRID)	||
#endif
		0
		)
	{
		return(0);
	}
	return(1);
}

int BGBCC_CCXL_CheckVirtOpIsLoadStore(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op)
{
	if(
		(op->opn==CCXL_VOP_LDIXIMM)			||
		(op->opn==CCXL_VOP_STIXIMM)			||
		(op->opn==CCXL_VOP_LDIX)			||
		(op->opn==CCXL_VOP_STIX)			||
		(op->opn==CCXL_VOP_LOADSLOT)		||
		(op->opn==CCXL_VOP_STORESLOT)		||
		0)
	{
		return(1);
	}

#if 0
	if(	(op->opn==CCXL_VOP_LEA)		||
		(op->opn==CCXL_VOP_LEAIMM)	)
	{
		return(1);
	}
#endif

	return(0);
}

int BGBCC_CCXL_CheckVirtOpIsStore(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op)
{
	if(
//		(op->opn==CCXL_VOP_LDIXIMM)			||
		(op->opn==CCXL_VOP_STIXIMM)			||
//		(op->opn==CCXL_VOP_LDIX)			||
		(op->opn==CCXL_VOP_STIX)			||
//		(op->opn==CCXL_VOP_LOADSLOT)		||
		(op->opn==CCXL_VOP_STORESLOT)		||
		0)
	{
		return(1);
	}
	return(0);
}

int BGBCC_CCXL_CheckVirtOpIsLoad(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op)
{
	if(
		(op->opn==CCXL_VOP_LDIXIMM)			||
		(op->opn==CCXL_VOP_LDIX)			||
		(op->opn==CCXL_VOP_LOADSLOT)		||
		0)
	{
		return(1);
	}

#if 0
	if(	(op->opn==CCXL_VOP_LEA)		||
		(op->opn==CCXL_VOP_LEAIMM)	)
	{
		return(1);
	}
#endif
	
	return(0);
}

int BGBCC_CCXL_CheckVirtOpOrderDep(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op1,
	BGBCC_CCXL_VirtOp *op2)
{
//	if(op1->dst.val==CCXL_REGID_REG_Z)
//		return(0);

	if(	BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srca) ||
		BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srcb) ||
		BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srcc) ||
		BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srcd))
	{
		if(op1->dst.val!=CCXL_REGID_REG_Z)
			return(1);
	}
	return(0);
}

int BGBCC_CCXL_CheckVirtOpOrderOnly(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op1,
	BGBCC_CCXL_VirtOp *op2)
{
	if(BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, op1))
		return(1);
	if(BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, op2))
		return(1);

#if 1
	if(	BGBCC_CCXL_CheckVirtOpIsLoadStore(ctx, op1) &&
		BGBCC_CCXL_CheckVirtOpIsLoadStore(ctx, op2)	&&
		(	BGBCC_CCXL_CheckVirtOpIsStore(ctx, op1) ||
			BGBCC_CCXL_CheckVirtOpIsStore(ctx, op2) ) &&
		!(	BGBCC_CCXL_CheckVirtOpIsStore(ctx, op1) &&
			BGBCC_CCXL_CheckVirtOpIsStore(ctx, op2) )
		)
#endif
//	if(	BGBCC_CCXL_CheckVirtOpIsLoadStore(ctx, op1) &&
//		BGBCC_CCXL_CheckVirtOpIsLoadStore(ctx, op2)	)
	{
		return(1);
	}

#if 0
//	if(op1->dst.val==CCXL_REGID_REG_Z)
//		return(0);

	if(	BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srca) ||
		BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srcb) ||
		BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srcc) ||
		BGBCC_CCXL_RegisterIdentEqualP(ctx,
			op1->dst, op2->srcd))
	{
		if(op1->dst.val!=CCXL_REGID_REG_Z)
			return(1);
	}
#endif

	if(BGBCC_CCXL_CheckVirtOpOrderDep(ctx, op1, op2))
		return(1);
	if(BGBCC_CCXL_CheckVirtOpOrderDep(ctx, op2, op1))
		return(1);

	return(0);
}

int BGBCC_CCXL_CheckVirtOpCanSwap(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_VirtOp *op1,
	BGBCC_CCXL_VirtOp *op2)
{
	if(BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, op1))
		return(0);
	if(BGBCC_CCXL_CheckVirtOpNoSwaps(ctx, op2))
		return(0);

	if(BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, op1, op2))
		return(0);
	if(BGBCC_CCXL_CheckVirtOpOrderOnly(ctx, op2, op1))
		return(0);

	return(1);
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
			case 'a':	fl|=BGBCC_TYFL_MAYALIAS;		break;
			case 'b':	fl|=BGBCC_TYFL_BYREF;			break;
			case 'e':	fl|=BGBCC_TYFL_EVENT;			break;
			case 'f':	fl|=BGBCC_TYFL_INTERRUPT;		break;
			case 'g':	fl|=BGBCC_TYFL_SYSCALL;			break;
			case 'i':	fl|=BGBCC_TYFL_INTERFACE;		break;

			case 'r':	fl|=BGBCC_TYFL_RESTRICT;		break;
			case 's':	fl|=BGBCC_TYFL_SYNCHRONIZED;	break;

			case 'v':	fl|=BGBCC_TYFL_VOLATILE;		break;
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

			case 's':	fl|=BGBCC_TYFL_TBRSAVE;			break;
			case 't':	fl|=BGBCC_TYFL_THREAD;			break;

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


int BGBCC_CCXL_NormalizeImmVReg(
	BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register treg, ccxl_register *rtreg)
{
//	ccxl_register treg, treg2;
	ccxl_register treg2;
	double f;
	float bf;
	s64 li, lj, li0, li1;
	u32 ui;
	int bty, dty, stv;
	int i, j, k, sg;

//	treg=*rtreg;

	*rtreg=treg;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmUIntP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmULongP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
//		if(BGBCC_CCXL_TypeSmallSIntP(ctx, type))
		if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
		{
//			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
//				return(1);
		
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			stv=0;

			li0=li;

			bty=BGBCC_CCXL_GetTypeBaseType(ctx, type);
			switch(bty)
			{
			case CCXL_TY_SB:	li=(signed char)li;		stv=4; break;
			case CCXL_TY_UB:	li=(unsigned char)li;		stv=5; break;
			case CCXL_TY_SS:	li=(signed short)li;		stv=6; break;
			case CCXL_TY_US:	li=(unsigned short)li;		stv=7; break;
			case CCXL_TY_I:		li=(signed int)li;			stv=0; break;
			case CCXL_TY_UI:	li=(unsigned int)li;		stv=1; break;
			default:
				break;
			}
			
			if((li0!=li) && (li0!=((s32)li)) && (((s32)li0)!=li))
			{
				BGBCC_CCXL_Warn(ctx, "Constant Conversion, Loss of Range\n");
			}
			
//			if(((s32)li)!=li)
//				{ BGBCC_DBGBREAK }

//			if(BGBCC_CCXL_TypeUnsignedP(ctx, dty))
//			BGBCC_CCXL_GetRegForUIntValue(ctx, &treg2, li);

//			BGBCC_CCXL_GetRegForIntValue(ctx, &treg2, li);
			BGBCC_CCXL_GetRegForSmallIntValue(ctx, &treg2, li, stv);
			*rtreg=treg2;
			return(1);
		}

#if 0
		if(BGBCC_CCXL_TypeSmallUIntP(ctx, type))
		{
//			if(BGBCC_CCXL_IsRegImmUIntP(ctx, treg))
//				return(1);
		
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			
//			if(((u32)li)!=li)
//			if((((u32)li)!=li) && (((s32)li)!=li))
//				{ BGBCC_DBGBREAK }

			bty=BGBCC_CCXL_GetTypeBaseType(ctx, type);
			switch(bty)
			{
			case CCXL_TY_SB: li=(signed char)li; break;
			case CCXL_TY_UB: li=(unsigned char)li; break;
			case CCXL_TY_SS: li=(signed short)li; break;
			case CCXL_TY_US: li=(unsigned short)li; break;
			case CCXL_TY_I: li=(signed int)li; break;
			case CCXL_TY_UI: li=(unsigned int)li; break;
			default:
				break;
			}
			
			BGBCC_CCXL_GetRegForUIntValue(ctx, &treg2, li);
			*rtreg=treg2;
			return(1);
		}
#endif

		if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
		{
//			if(BGBCC_CCXL_IsRegImmLongP(ctx, treg))
//				return(1);

			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				BGBCC_CCXL_GetRegForULongValue(ctx, &treg2, li);
			else
				BGBCC_CCXL_GetRegForLongValue(ctx, &treg2, li);
			*rtreg=treg2;
			return(1);
		}

		if(	BGBCC_CCXL_TypeBFloat16P(ctx, type) ||
			BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
				return(1);

			bf=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);

			memcpy(&ui, &bf, 4);
			ui=((ui+4096)>>13)<<13;
			memcpy(&bf, &ui, 4);

			BGBCC_CCXL_GetRegForFloatValue(ctx, &treg2, bf);
			*rtreg=treg2;
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type))
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
				return(1);

			bf=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);

//			memcpy(&ui, &bf, 4);
//			ui=((ui+4)>>3)<<3;
//			memcpy(&bf, &ui, 4);

			BGBCC_CCXL_GetRegForFloatValue(ctx, &treg2, bf);
			*rtreg=treg2;
			return(1);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
				return(1);

			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
			BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg2, f);
			*rtreg=treg2;
			return(1);
		}

		if(	BGBCC_CCXL_TypePointerP(ctx, type) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, type))
		{
			if(ctx->arch_sizeof_ptr==8)
			{
				li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
				BGBCC_CCXL_GetRegForLongValue(ctx, &treg2, li);
				*rtreg=treg2;
				return(1);
			}

			if(ctx->arch_sizeof_ptr==4)
			{
				li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
				BGBCC_CCXL_GetRegForUIntValue(ctx, &treg2, li);
				*rtreg=treg2;
				return(1);
			}
		}
		
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, type))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			lj=0;
			BGBCC_CCXL_GetRegForInt128Value(ctx, &treg2, li, lj);
			*rtreg=treg2;
			return(1);
		}

		if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			lj=li>>63;
			if(BGBCC_CCXL_IsRegImmULongP(ctx, treg))
				lj=0;
			BGBCC_CCXL_GetRegForInt128Value(ctx, &treg2, li, lj);
			*rtreg=treg2;
			return(1);
		}
		
		if(BGBCC_CCXL_TypeBcd64P(ctx, type))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			
			sg=0;
			if(li<0)
				{ li=-li; sg=1; }
			
			li0=0;
			for(i=0; i<16; i++)
			{
				j=li%10;
				li=li/10;
				li0|=((u64)j)<<(i*4);
			}
			
			if(sg)
				{ li0=0x9999999999999999ULL-li0; }

			BGBCC_CCXL_GetRegForX128Value(ctx,
				&treg2, li0, 0, CCXL_REGVEC_TY_BCD64);
			*rtreg=treg2;
			return(1);
		}

		if(BGBCC_CCXL_TypeBcd128P(ctx, type))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			
			sg=0;
			if(li<0)
				{ li=-li; sg=1; }
			
			li0=0;
			li1=0;
			for(i=0; i<16; i++)
			{
				j=li%10;
				li=li/10;
				li0|=((u64)j)<<(i*4);
			}
			for(i=0; i<16; i++)
			{
				j=li%10;
				li=li/10;
				li1|=((u64)j)<<(i*4);
			}
			
			if(sg)
			{
				li0=0x9999999999999999ULL-li0;
				li1=0x9999999999999999ULL-li1;
			}

			BGBCC_CCXL_GetRegForX128Value(ctx,
				&treg2, li0, li1, CCXL_REGVEC_TY_BCD128);
			*rtreg=treg2;
			return(1);
		}
	}

	return(0);
}
