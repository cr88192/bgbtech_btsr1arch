/*
 * Variant Arithmetic
 */

int BGBCC_JX2C_EmitBinaryVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, rcls;
	int i, j, k;

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__lva_add"; break;
	case CCXL_BINOP_SUB:	s0="__lva_sub"; break;
	case CCXL_BINOP_MUL:	s0="__lva_mul"; break;
	case CCXL_BINOP_DIV:	s0="__lva_div"; break;
	case CCXL_BINOP_MOD:	s0="__lva_mod"; break;
	case CCXL_BINOP_AND:	s0="__lva_and"; break;
	case CCXL_BINOP_OR:		s0="__lva_or"; break;
	case CCXL_BINOP_XOR:	s0="__lva_xor"; break;
	case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
	case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
	}

	if(s0)
	{
		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, rcls;
	int i, j, k;
	
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegVariant(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__lva_add"; break;
	case CCXL_BINOP_SUB:	s0="__lva_sub"; break;
	case CCXL_BINOP_MUL:	s0="__lva_mul"; break;
	case CCXL_BINOP_DIV:	s0="__lva_div"; break;
	case CCXL_BINOP_MOD:	s0="__lva_mod"; break;
	case CCXL_BINOP_AND:	s0="__lva_and"; break;
	case CCXL_BINOP_OR:		s0="__lva_or"; break;
	case CCXL_BINOP_XOR:	s0="__lva_xor"; break;
	case CCXL_BINOP_SHL:	s0="__lva_shl"; break;
	case CCXL_BINOP_SHR:	s0="__lva_shr"; break;
	}

	if(s0)
	{
		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);

//	BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
//	i=BGBCC_JX2C_EmitBinaryVRegVRegVariant(ctx, sctx, type, dreg, opr, treg);
//	return(i);
}

int BGBCC_JX2C_EmitUnaryVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4, rcls;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}
	
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__lva_neg"; break;
	case CCXL_UNOP_NOT:	s0="__lva_not"; break;
	}

	if(s0)
	{
		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvToVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int fr0, dr0;
	int tri, tro;
	int pri, pro;
	int rcls;

//	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);
	if(sctx->is_addr64)
	{
		tri=BGBCC_SH_REG_RQ4;
		tro=BGBCC_SH_REG_RQ2;

		pri=BGBCC_SH_REG_RQ4;
		pro=BGBCC_SH_REG_RQ2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr0=BGBCC_SH_REG_RD2;
			dr0=BGBCC_SH_REG_RQ2;
		}else
		{
			fr0=BGBCC_SH_REG_FR2;
			dr0=BGBCC_SH_REG_DR2;
		}
	}else
	{
		tri=BGBCC_SH_REG_LR4;
		tro=BGBCC_SH_REG_LR2;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr0=BGBCC_SH_REG_R2;
			dr0=BGBCC_SH_REG_LR2;
		}else
		{
			fr0=BGBCC_SH_REG_FR2;
			dr0=BGBCC_SH_REG_DR2;
		}
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_toi32");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD2);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_toi64");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_tof32");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, fr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, fr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, fr0);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_tof64");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, dr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, dr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, dr0);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(BGBCC_CCXL_TypeCStringP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_tostr");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, pro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pro);
			return(1);
		}
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_toptr");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, pro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pro);
		return(1);
	}

#if 0
	if(0)
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, opn);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvFromVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int fr4, dr4;
	int tri, tro;
	int pri, pro;

	if(sctx->is_addr64)
	{
		tri=BGBCC_SH_REG_RQ4;
		tro=BGBCC_SH_REG_RQ2;

		pri=BGBCC_SH_REG_RQ4;
		pro=BGBCC_SH_REG_RQ2;
		
		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_RD4;
			dr4=BGBCC_SH_REG_RQ4;
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}else
	{
		tri=BGBCC_SH_REG_LR4;
		tro=BGBCC_SH_REG_LR2;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_R4;
			dr4=BGBCC_SH_REG_LR4;
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromi32");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromi64");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, fr4);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, fr4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromf32");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, fr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, dr4);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, dr4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromf64");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, dr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}


	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, pri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromptr");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvFromVRegVRegVarString(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int fr4, dr4;
	int tri, tro;
	int pri, pro;

	if(sctx->is_addr64)
	{
		tri=BGBCC_SH_REG_RQ4;
		tro=BGBCC_SH_REG_RQ2;

		pri=BGBCC_SH_REG_RQ4;
		pro=BGBCC_SH_REG_RQ2;
		
		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_RD4;
			dr4=BGBCC_SH_REG_RQ4;
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}else
	{
		tri=BGBCC_SH_REG_LR4;
		tro=BGBCC_SH_REG_LR2;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_R4;
			dr4=BGBCC_SH_REG_LR4;
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}

//	if(BGBCC_CCXL_TypePointerP(ctx, type))
	if(BGBCC_CCXL_TypeCStringP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, pri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromstr");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeCWStringP(ctx, type))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pri);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, pri);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromwstr");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pri);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
