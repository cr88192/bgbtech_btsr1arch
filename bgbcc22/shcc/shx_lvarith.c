/*
 * Variant Arithmetic
 */

int BGBCC_SHXC_EmitBinaryVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, rcls;
	int i, j, k;

	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);

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
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, rcls;
	int i, j, k;
	
	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegVariant(ctx, sctx, type, dreg, opr, treg);
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
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);

//	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
//	i=BGBCC_SHXC_EmitBinaryVRegVRegVariant(ctx, sctx, type, dreg, opr, treg);
//	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}
	
	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);

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
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitConvToVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int fr0, dr0;
	int tri, tro;
	int rcls;

//	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	if(sctx->is_addr64)
	{
		tri=BGBCC_SH_REG_RQ4;
		tro=BGBCC_SH_REG_RQ0;

		if(sctx->no_fpu)
		{
			fr0=BGBCC_SH_REG_RD0;
			dr0=BGBCC_SH_REG_RQ0;
		}else
		{
			fr0=BGBCC_SH_REG_FR0;
			dr0=BGBCC_SH_REG_DR0;
		}
	}else
	{
		tri=BGBCC_SH_REG_LR4;
		tro=BGBCC_SH_REG_LR0;

		if(sctx->no_fpu)
		{
			fr0=BGBCC_SH_REG_R0;
			dr0=BGBCC_SH_REG_LR0;
		}else
		{
			fr0=BGBCC_SH_REG_FR0;
			dr0=BGBCC_SH_REG_DR0;
		}
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_toi32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_toi64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_tof32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, fr0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, fr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, fr0);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_tof64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, dr0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, dr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, dr0);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_toptr");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

#if 0
	if(0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, opn);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitConvFromVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int fr4, dr4;
	int tri, tro;

	if(sctx->is_addr64)
	{
		tri=BGBCC_SH_REG_RQ4;
		tro=BGBCC_SH_REG_RQ0;
		
		if(sctx->no_fpu)
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
		tro=BGBCC_SH_REG_LR0;

		if(sctx->no_fpu)
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
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromi32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tri);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tri);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromi64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tri);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, fr4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, fr4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromf32");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, fr4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, dr4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, dr4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromf64");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, dr4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}


	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__lva_conv_fromptr");
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, tro);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tro);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tro);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
