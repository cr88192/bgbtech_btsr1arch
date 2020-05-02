/*
 * Int128 Arithmetic
 */

int BGBCC_SHXC_IndexLitInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	s64 val_lo, s64 val_hi)
{
	int i, j, k;

	if(!sctx->lvt16_lbl)
	{
		sctx->lvt16_lbl=sctx->t_lvt16_lbl;
		sctx->lvt16_val=sctx->t_lvt16_val;
		sctx->lvt16_n_idx=0;
		sctx->lvt16_m_idx=1024;
	}
	
	for(i=0; i<sctx->lvt16_n_idx; i++)
	{
		if((sctx->lvt16_val[i*2+0]==val_lo) &&
			(sctx->lvt16_val[i*2+1]==val_hi))
				{ return(sctx->lvt16_lbl[i]); }
	}
	
	i=sctx->lvt16_n_idx++;
	k=BGBCC_SHX_GenLabel(sctx);
	sctx->lvt16_lbl[i]=k;
	sctx->lvt16_val[i*2+0]=val_lo;
	sctx->lvt16_val[i*2+1]=val_hi;
	return(k);
}

int BGBCC_SHXC_EmitBinaryVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2;
	int i, j, k;
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xli_add"; break;
	case CCXL_BINOP_SUB:	s0="__xli_sub"; break;
	case CCXL_BINOP_MUL:	s0="__xli_smul"; break;
	case CCXL_BINOP_DIV:	s0="__xli_sdiv"; break;
	case CCXL_BINOP_AND:	s0="__xli_and"; break;
	case CCXL_BINOP_OR:		s0="__xli_or"; break;
	case CCXL_BINOP_XOR:	s0="__xli_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xli_shl"; break;
	case CCXL_BINOP_SHR:	s0="__xli_shr"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R2);
		if(BGBCC_CCXL_IsRegTempP(ctx, dreg))
		{
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R2);
		}

		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__xli_neg"; break;
	case CCXL_UNOP_NOT:	s0="__xli_not"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R2);
		if(BGBCC_CCXL_IsRegTempP(ctx, dreg))
		{
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R2);
		}

		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
//		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
//		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
