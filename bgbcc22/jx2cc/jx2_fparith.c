
int BGBCC_JX2C_EmitBinaryVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	char *s0;
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegSoftFloat(
			ctx, sctx, type, dreg, opr, treg));
	}

	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_FADD;	nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_FSUB;	nm2=-1; break;
	case CCXL_BINOP_MUL:	nm1=BGBCC_SH_NMID_FMUL;	nm2=-1; break;
//	case CCXL_BINOP_DIV:	nm1=BGBCC_SH_NMID_FDIV;	nm2=-1; break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(sctx->fpu_lite)
	{
		if(opr==CCXL_BINOP_DIV)
			nm1=-1;
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			nm1=-1;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		if(nm2>=0)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, nm2, ctreg, BGBCC_SH_REG_FR0);
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_FR0, cdreg);
		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
		}
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

#if 1
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__fpu_fadd";	break;
	case CCXL_BINOP_SUB:	s0="__fpu_fsub";	break;
	case CCXL_BINOP_MUL:	s0="__fpu_fmul";	break;
	case CCXL_BINOP_DIV:	s0="__fpu_fdiv";	break;
	default:				s0=NULL; break;
	}

	if(s0)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_DR5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR2);
			return(1);
		}else
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_FR5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR2);
			return(1);
		}
	}
#endif

#if 0
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__lfp_fadd_f64";	break;
		case CCXL_BINOP_SUB:	s0="__lfp_fsub_f64";	break;
		case CCXL_BINOP_MUL:	s0="__lfp_fmul_f64";	break;
		case CCXL_BINOP_DIV:	s0="__lfp_fdiv_f64";	break;
		default:				s0=NULL; break;
		}

		if(s0)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_DR6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR6);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR0);
			return(1);
		}
	}else
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__lfp_fadd_f32";	break;
		case CCXL_BINOP_SUB:	s0="__lfp_fsub_f32";	break;
		case CCXL_BINOP_MUL:	s0="__lfp_fmul_f32";	break;
		case CCXL_BINOP_DIV:	s0="__lfp_fdiv_f32";	break;
		default:				s0=NULL; break;
		}

		if(s0)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_FR5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR0);
			return(1);
		}
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	char *s0;
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i;

	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegSoftFloat(
			ctx, sctx, type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegFloat(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL))
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegFloat(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
		
		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

#if 0
//	if(sctx->has_bjx1ari)
	if(sctx->has_bjx1ari && sctx->has_bjx1mov)
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_FADD;	nm2=-1; break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_FSUB;	nm2=-1; break;
		case CCXL_BINOP_MUL:	nm1=BGBCC_SH_NMID_FMUL;	nm2=-1; break;
		case CCXL_BINOP_DIV:	nm1=BGBCC_SH_NMID_FDIV;	nm2=-1; break;

		default:		nm1=-1; nm2=-1; break;
		}

		if(sctx->fpu_lite)
		{
			if(opr==CCXL_BINOP_DIV)
				nm1=-1;
			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
				nm1=-1;
		}

		if(nm1>=0)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
				{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
			else
				{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{
				BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
					{ BGBCC_DBGBREAK }
				if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
					{ BGBCC_DBGBREAK }
			}
			else
			{
				BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
				if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
			}

			BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
	}
#endif

	BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_JX2C_EmitBinaryVRegVRegFloat(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_JX2C_EmitUnaryVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	ccxl_register treg;
	int csreg, ctreg, czreg, cdreg;
	int nm1, nm2;
	s32 imm;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitUnaryVRegVRegSoftFloat(
			ctx, sctx, type, dreg, opr, sreg));
	}

	if(sctx->fpu_lite)
	{
#if 0
		if(opr==CCXL_UNOP_LNOT)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
				BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 0.0);
			else
				BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 0.0);

			return(BGBCC_JX2C_EmitCompareVRegVRegVRegFloat(
				ctx, sctx, type, dreg, CCXL_CMP_EQ, sreg, treg));
		}
#endif

#if 1
		if(opr==CCXL_UNOP_LNOT)
		{
			nm1=BGBCC_SH_NMID_FCMPEQ;
			nm2=BGBCC_SH_NMID_BT;

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

			// yeah... FIXME

//			BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, csreg);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, ctreg);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_XOR, ctreg, ctreg);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
				ctreg, BGBCC_SH_REG_FPUL);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FSTS,
				BGBCC_SH_REG_FPUL, cdreg);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif

		if((opr==CCXL_UNOP_NEG) && BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_FNEG, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
	}

	if(opr==CCXL_UNOP_LNOT)
	{
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BT;

//		BGBCC_JX2C_EmitCompareVRegVRegVRegFloat(ctx, sctx, type, dreg, )
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

#if 0
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			czreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR2);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_FLDI0, czreg);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_XOR, ctreg, ctreg);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
				ctreg, BGBCC_SH_REG_FPUL);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FSTS,
				BGBCC_SH_REG_FPUL, BGBCC_SH_REG_FR0+(czreg&15));
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FSTS,
				BGBCC_SH_REG_FPUL, BGBCC_SH_REG_FR1+(czreg&15));
		}else
		{
			czreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_FLDI0, czreg);
		}
#endif

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			czreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_DR);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, czreg, 0);
		}else
		{
			czreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, czreg, 0);
		}

		BGBCC_JX2_EmitOpRegReg(sctx, nm1, czreg, csreg);
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, ctreg);

//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
//			ctreg, BGBCC_SH_REG_FPUL);
//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLOAT,
//			BGBCC_SH_REG_FPUL, cdreg);

		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
			ctreg, BGBCC_SH_REG_MACL);
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_FLDCI, cdreg);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, czreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if((opr==CCXL_UNOP_INC) || (opr==CCXL_UNOP_DEC))
	{
		j=CCXL_BINOP_ADD;
		if(opr==CCXL_UNOP_DEC)
			j=CCXL_BINOP_SUB;
	
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0);
		else
			BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0);
	
		BGBCC_JX2C_EmitBinaryVRegVRegVRegFloat(ctx, sctx,
			type, dreg, j, sreg, treg);
		return(1);
	}

	switch(opr)
	{
	case CCXL_UNOP_MOV:	nm1=BGBCC_SH_NMID_FMOV; nm2=-1; break;
	case CCXL_UNOP_NEG:	nm1=BGBCC_SH_NMID_FNEG; nm2=-1; break;
//	case CCXL_UNOP_ABS:	nm1=BGBCC_SH_NMID_FABS; nm2=-1; break;
	default:		nm1=-1; nm2=-1; break;
	}

	if(sctx->fpu_lite)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			nm1=-1;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2_EmitOpReg(sctx, nm1, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		}else
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, csreg, cdreg);

			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);

			BGBCC_JX2_EmitOpReg(sctx, nm1, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		}
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitCompareVRegVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, swst, lfp;
	
	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitCompareVRegVRegVRegSoftFloat(
			ctx, sctx, type, dreg, cmp, sreg, treg));
	}

	lfp=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BT; swst=0;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BF; swst=0;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BT; swst=1;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BT; swst=0;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BF; swst=0;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BF; swst=1;
		break;
	default:
		nm1=-1;
		nm2=-1;
		swst=-1;
		break;
	}
	
	if(sctx->fpu_lite)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
//			nm1=-1;
			lfp=1;
		}
	}

	if((nm1>=0) && (nm2>=0) && !lfp)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
//			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_R0)
//			if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, cdreg))
//				{ BGBCC_DBGBREAK }
			if(!BGBCC_JX2_EmitCheckRegExtGPR(sctx, cdreg))
				{ BGBCC_DBGBREAK }
		}
		else
		{
			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
//			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_R0)
//				{ BGBCC_DBGBREAK }
//			if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, cdreg))
//				{ BGBCC_DBGBREAK }
			if(!BGBCC_JX2_EmitCheckRegExtGPR(sctx, cdreg))
				{ BGBCC_DBGBREAK }
		}

		if(swst)
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, csreg);
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);
		BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		
		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, 1, cdreg);
		}
		
//		BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if((nm1>=0) && (nm2>=0) && lfp)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
		if(!BGBCC_JX2_EmitCheckRegExtGPR(sctx, cdreg))
			{ BGBCC_DBGBREAK }

		if(swst)
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, csreg);
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);
		BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		
		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, 1, cdreg);
		}
		
//		BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2, sw, lfp;
	
	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegSoftFloat(
			ctx, sctx, type, sreg, treg, cmp, lbl));
	}

	lfp=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BT; sw=0;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BF; sw=0;
		break;
	case CCXL_CMP_LT:
//		nm1=BGBCC_SH_NMID_FCMPGE;
		nm1=BGBCC_SH_NMID_FCMPGT;
//		nm2=BGBCC_SH_NMID_BF;
		nm2=BGBCC_SH_NMID_BT; sw=1;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BT; sw=0;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BF; sw=0;
		break;
	case CCXL_CMP_GE:
//		nm1=BGBCC_SH_NMID_FCMPGE;
		nm1=BGBCC_SH_NMID_FCMPGT;
//		nm2=BGBCC_SH_NMID_BT;
		nm2=BGBCC_SH_NMID_BF; sw=1;
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}

	if(sctx->fpu_lite)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
//			nm1=-1;
			lfp=1;
		}
	}

	if((nm1>=0) && (nm2>=0) && !lfp)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		if(sw)
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, csreg);

//		if(sctx->need_farjmp)
//			BGBCC_JX2_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);
		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if((nm1>=0) && (nm2>=0) && lfp)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

		if(sw)
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, csreg);

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
