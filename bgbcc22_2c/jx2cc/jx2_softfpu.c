/*
Stuff to deal with Software Floating Point.
 */

int BGBCC_JX2C_NormalizeImmVRegSoftFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register *rtreg)
{
	ccxl_register treg, treg2;
	double f;

	treg=*rtreg;

	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type) ||
		BGBCC_CCXL_TypeHFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeBFloat16P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmUIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg))
		{
			f=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
				BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg2, f);
			else
				BGBCC_CCXL_GetRegForFloatValue(ctx, &treg2, f);
			*rtreg=treg2;
			return(1);
		}
	}

	return(0);
}
 
int BGBCC_JX2C_EmitBinaryVRegVRegSoftFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int lr4, lr5, lr0;
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, rcls;
	int i, j, k;

	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &treg);

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	s0=NULL;

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__sfp_fadd_f64"; break;
		case CCXL_BINOP_SUB:	s0="__sfp_fsub_f64"; break;
		case CCXL_BINOP_MUL:	s0="__sfp_fmul_f64"; break;
		case CCXL_BINOP_DIV:	s0="__sfp_fdiv_f64"; break;
		}
	}else
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__sfp_fadd_f32"; break;
		case CCXL_BINOP_SUB:	s0="__sfp_fsub_f32"; break;
		case CCXL_BINOP_MUL:	s0="__sfp_fmul_f32"; break;
		case CCXL_BINOP_DIV:	s0="__sfp_fdiv_f32"; break;
		}
	}

	lr0=-1;
	if(sctx->is_addr64)
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_RD4;
			lr5=BGBCC_SH_REG_RD5;
			lr0=BGBCC_SH_REG_RD2;
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			lr4=BGBCC_SH_REG_RQ4;
			lr5=BGBCC_SH_REG_RQ5;
			lr0=BGBCC_SH_REG_RQ2;
		}
	}else
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_R4;
			lr5=BGBCC_SH_REG_R5;
			lr0=BGBCC_SH_REG_R2;
		}

		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			lr4=BGBCC_SH_REG_LR4;
			lr5=BGBCC_SH_REG_LR6;
			lr0=BGBCC_SH_REG_LR2;
		}
	}
	
	if(s0 && (lr0>=0))
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, lr5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, lr4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr5);
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, lr0, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegSoftFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int lr4, lr5, lr0;
	char *s0;
	int csreg, ctreg, cdreg;
	int nm1, nm2, rcls;
	int i;
	
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &treg);

	s0=NULL;

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__sfp_fadd_f64"; break;
		case CCXL_BINOP_SUB:	s0="__sfp_fsub_f64"; break;
		case CCXL_BINOP_MUL:	s0="__sfp_fmul_f64"; break;
		case CCXL_BINOP_DIV:	s0="__sfp_fdiv_f64"; break;
		}
	}else
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__sfp_fadd_f32"; break;
		case CCXL_BINOP_SUB:	s0="__sfp_fsub_f32"; break;
		case CCXL_BINOP_MUL:	s0="__sfp_fmul_f32"; break;
		case CCXL_BINOP_DIV:	s0="__sfp_fdiv_f32"; break;
		}
	}

	lr0=-1;
	if(sctx->is_addr64)
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_RD4;
			lr5=BGBCC_SH_REG_RD5;
			lr0=BGBCC_SH_REG_RD2;
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			lr4=BGBCC_SH_REG_RQ4;
			lr5=BGBCC_SH_REG_RQ5;
			lr0=BGBCC_SH_REG_RQ2;
		}
	}else
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_R4;
			lr5=BGBCC_SH_REG_R5;
			lr0=BGBCC_SH_REG_R2;
		}

		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			lr4=BGBCC_SH_REG_LR4;
			lr5=BGBCC_SH_REG_LR6;
			lr0=BGBCC_SH_REG_LR2;
		}
	}
	
	if(s0 && (lr0>=0))
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, lr5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr5);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, lr0, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitUnaryVRegVRegSoftFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int lr4, lr5, lr0;
	int tr0, tr1, tr2;
	char *s0;
	ccxl_register treg;
	int csreg, ctreg, czreg, cdreg;
	int nm1, nm2, rcls;
	s32 imm;
	int i, j, k;

	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &sreg);

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	s0=NULL;

//	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
//	{
//	}else
//	{
//	}

#if 0
	if(opr==CCXL_UNOP_NEG)
	{
		csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				tr0, 0x80000000);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XOR, tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, tr0, 0x8000000000000000LL);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XOR, tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}

		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			tr1=BGBCC_JX2_GETLREG_LO(cdreg);
			tr2=BGBCC_JX2_GETLREG_HI(cdreg);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				tr0, 0x80000000);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XOR, tr0, tr2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
#endif

	if((opr==CCXL_UNOP_INC) || (opr==CCXL_UNOP_DEC))
	{
		j=CCXL_BINOP_ADD;
		if(opr==CCXL_UNOP_DEC)
			j=CCXL_BINOP_SUB;
	
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0);
		else
			BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0);
	
		BGBCC_JX2C_EmitBinaryVRegVRegVRegSoftFloat(ctx, sctx,
			type, dreg, j, sreg, treg);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		switch(opr)
		{
		case CCXL_UNOP_NEG:		s0="__sfp_neg_f64"; break;
		case CCXL_UNOP_LNOT:	s0="__sfp_lnot_f64"; break;
		}
	}else
	{
		switch(opr)
		{
		case CCXL_UNOP_NEG:		s0="__sfp_neg_f32"; break;
		case CCXL_UNOP_LNOT:	s0="__sfp_lnot_f32"; break;
		}
	}

	lr0=-1;
	if(sctx->is_addr64)
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_RD4;
			lr5=BGBCC_SH_REG_RD5;
			lr0=BGBCC_SH_REG_RD2;
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			lr4=BGBCC_SH_REG_RQ4;
			lr5=BGBCC_SH_REG_RQ5;
			lr0=BGBCC_SH_REG_RQ2;
		}
	}else
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_R4;
			lr5=BGBCC_SH_REG_R5;
			lr0=BGBCC_SH_REG_R2;
		}

		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			lr4=BGBCC_SH_REG_LR4;
			lr5=BGBCC_SH_REG_LR6;
			lr0=BGBCC_SH_REG_LR2;
		}
	}
	
	if(s0 && (lr0>=0))
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, lr0, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegSoftFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int lr4, lr5, lr0;
	char *s0;
	int csreg, ctreg, cdreg;
	int nm1, nm2, rcls;
	int i;

	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &treg);

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	s0=NULL;

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		switch(cmp)
		{
		case CCXL_CMP_EQ:	s0="__sfp_cmpeq_f64"; break;
		case CCXL_CMP_NE:	s0="__sfp_cmpne_f64"; break;
		case CCXL_CMP_LT:	s0="__sfp_cmplt_f64"; break;
		case CCXL_CMP_GT:	s0="__sfp_cmpgt_f64"; break;
		case CCXL_CMP_LE:	s0="__sfp_cmple_f64"; break;
		case CCXL_CMP_GE:	s0="__sfp_cmpge_f64"; break;
		}
	}else
	{
		switch(cmp)
		{
		case CCXL_CMP_EQ:	s0="__sfp_cmpeq_f32"; break;
		case CCXL_CMP_NE:	s0="__sfp_cmpne_f32"; break;
		case CCXL_CMP_LT:	s0="__sfp_cmplt_f32"; break;
		case CCXL_CMP_GT:	s0="__sfp_cmpgt_f32"; break;
		case CCXL_CMP_LE:	s0="__sfp_cmple_f32"; break;
		case CCXL_CMP_GE:	s0="__sfp_cmpge_f32"; break;
		}
	}

	lr0=-1;
	if(sctx->is_addr64)
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_RD4;
			lr5=BGBCC_SH_REG_RD5;
			lr0=BGBCC_SH_REG_RD2;
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			lr4=BGBCC_SH_REG_RQ4;
			lr5=BGBCC_SH_REG_RQ5;
			lr0=BGBCC_SH_REG_RQ2;
		}
	}else
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_R4;
			lr5=BGBCC_SH_REG_R5;
			lr0=BGBCC_SH_REG_R2;
		}

		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			lr4=BGBCC_SH_REG_LR4;
			lr5=BGBCC_SH_REG_LR6;
			lr0=BGBCC_SH_REG_LR2;
		}
	}
	
	if(s0 && (lr0>=0))
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, lr5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr5);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, lr0, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVRegSoftFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int lr4, lr5, lr0;
	char *s0;
	int csreg, ctreg, cdreg;
	int nm1, nm2, rcls, flip;
	int i;

	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &treg);

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	s0=NULL;

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		flip=0;
		nm1=BGBCC_SH_NMID_BF;
		
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			s0="__sfp_cmpeq_f64";	flip=0; nm1=BGBCC_SH_NMID_BF;	break;
		case CCXL_CMP_NE:
			s0="__sfp_cmpeq_f64";	flip=0; nm1=BGBCC_SH_NMID_BT;	break;
		case CCXL_CMP_LT:
			s0="__sfp_cmpgt_f64";	flip=1; nm1=BGBCC_SH_NMID_BF;	break;
		case CCXL_CMP_GT:
			s0="__sfp_cmpgt_f64";	flip=0; nm1=BGBCC_SH_NMID_BF;	break;
		case CCXL_CMP_LE:
			s0="__sfp_cmpgt_f64";	flip=0; nm1=BGBCC_SH_NMID_BT;	break;
		case CCXL_CMP_GE:
			s0="__sfp_cmpgt_f64";	flip=1;	nm1=BGBCC_SH_NMID_BT;	break;
		}
	}else
	{
		flip=0;
		nm1=BGBCC_SH_NMID_BF;

		switch(cmp)
		{
//		case CCXL_CMP_EQ:	s0="__sfp_cmpeq_f32"; break;
//		case CCXL_CMP_NE:	s0="__sfp_cmpne_f32"; break;
//		case CCXL_CMP_LT:	s0="__sfp_cmplt_f32"; break;
//		case CCXL_CMP_GT:	s0="__sfp_cmpgt_f32"; break;
//		case CCXL_CMP_LE:	s0="__sfp_cmple_f32"; break;
//		case CCXL_CMP_GE:	s0="__sfp_cmpge_f32"; break;
		case CCXL_CMP_EQ:
			s0="__sfp_cmpeq_f32";	flip=0; nm1=BGBCC_SH_NMID_BF;	break;
		case CCXL_CMP_NE:
			s0="__sfp_cmpeq_f32";	flip=0; nm1=BGBCC_SH_NMID_BT;	break;
		case CCXL_CMP_LT:
			s0="__sfp_cmpgt_f32";	flip=1; nm1=BGBCC_SH_NMID_BF;	break;
		case CCXL_CMP_GT:
			s0="__sfp_cmpgt_f32";	flip=0; nm1=BGBCC_SH_NMID_BF;	break;
		case CCXL_CMP_LE:
			s0="__sfp_cmpgt_f32";	flip=0; nm1=BGBCC_SH_NMID_BT;	break;
		case CCXL_CMP_GE:
			s0="__sfp_cmpgt_f32";	flip=1;	nm1=BGBCC_SH_NMID_BT;	break;
		}
	}

	lr0=-1;
	if(sctx->is_addr64)
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_RD4;
			lr5=BGBCC_SH_REG_RD5;
			lr0=BGBCC_SH_REG_R2;
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			lr4=BGBCC_SH_REG_RQ4;
			lr5=BGBCC_SH_REG_RQ5;
			lr0=BGBCC_SH_REG_R2;
		}
	}else
	{
		if(rcls==BGBCC_SH_REGCLS_GR)
		{
			lr4=BGBCC_SH_REG_R4;
			lr5=BGBCC_SH_REG_R5;
			lr0=BGBCC_SH_REG_R2;
		}

		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			lr4=BGBCC_SH_REG_LR4;
			lr5=BGBCC_SH_REG_LR6;
			lr0=BGBCC_SH_REG_R2;
		}
	}
	
	if(s0 && (lr0>=0))
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr5);
		if(flip)
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, lr4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr5);
		}else
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, lr5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
		}
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr5);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr0);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_TST, lr0, lr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
//		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);
		BGBCC_JX2_EmitOpAutoLabel(sctx, nm1, lbl);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
