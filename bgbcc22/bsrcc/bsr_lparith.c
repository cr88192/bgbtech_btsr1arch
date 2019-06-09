/*
 * Long Long Arithmetic
 */

int BGBCC_BSRC_EmitBinaryLong_ShlImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int cdreg, int shl)
{
	int tr0, tr1, tr2, tr3;
	int cdreg2;
	int nm1, nm2;
	int i, j, k;

	cdreg2=BGBCC_SH_REG_R0+(cdreg&15);

	if(shl==0)
	{
		return(1);
	}

	if(shl>=64)
	{
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, cdreg2+0);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, cdreg2+1);
		return(1);
	}

	if(shl==32)
	{
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+0, cdreg2+1);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
			0, cdreg2+0);
		return(1);
	}

	if(shl>32)
	{
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+0, cdreg2+1);
			
		nm1=-1; nm2=-1;
		switch(shl)
		{
		case 33: nm1=BGBCC_SH_NMID_SHLL1; nm2=-1; break;
		case 34: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
		case 35: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
		case 36: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
		case 40: nm1=BGBCC_SH_NMID_SHLL8; nm2=-1; break;
		case 41: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL1; break;
		case 42: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2; break;
		case 48: nm1=BGBCC_SH_NMID_SHLL16; nm2=-1; break;
		case 49: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL1; break;
		case 50: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL2; break;
		case 56: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL8; break;
		}

//		if(nm1>=0)
		if(0)
		{
			BGBCC_BSR_EmitOpReg(sctx, nm1, cdreg2+1);
			if(nm2>=0)
				BGBCC_BSR_EmitOpReg(sctx, nm2, cdreg2+1);
		}else
		{
//			BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
//				shl-32, cdreg2+0);
//			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD,
//				cdreg2+0, cdreg2+1);
			BGBCC_BSRC_EmitLeaShlRegImm(ctx, sctx, cdreg2+1, shl-32);
		}

		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
			0, cdreg2+0);
		return(1);
	}
	
	if(shl<=3)
	{
		for(i=0; i<shl; i++)
		{
			BGBCC_BSR_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
				cdreg2+0, cdreg2+0);
			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC, 
				cdreg2+1, cdreg2+1);
		}
		return(1);
	}
	
//	tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
//	tr2=BGBCC_SH_REG_R0+(tr0&15);

	tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+0, tr0);
//	BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, shl, tr1);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+0);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+1);

	BGBCC_BSRC_EmitLeaShlRegImm(ctx, sctx, cdreg2+0, shl);
	BGBCC_BSRC_EmitLeaShlRegImm(ctx, sctx, cdreg2+1, shl);

//	BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -(32-shl), tr1);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, tr0);
	BGBCC_BSRC_EmitLeaShrRegImm(ctx, sctx, tr0, 32-shl);

	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr0, cdreg2+1);

	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
	return(1);
}

int BGBCC_BSRC_EmitBinaryLong_ShrImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int cdreg, int shl)
{
	int tr0, tr1, tr2, tr3;
	int cdreg2;
	int nm1, nm2;
	int i, j, k;

	cdreg2=BGBCC_SH_REG_R0+(cdreg&15);

	if(shl==0)
	{
		return(1);
	}

	if(shl>=64)
	{
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, cdreg2+0);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, cdreg2+1);
		return(1);
	}

	if(shl==32)
	{
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+1, cdreg2+0);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
			0, cdreg2+1);
		return(1);
	}

	if(shl>32)
	{
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+1, cdreg2+0);
		switch(shl)
		{
		case 33: nm1=BGBCC_SH_NMID_SHLR1; nm2=-1; break;
		case 34: nm1=BGBCC_SH_NMID_SHLR2; nm2=-1; break;
		case 35: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR1; break;
		case 36: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2; break;
		case 40: nm1=BGBCC_SH_NMID_SHLR8; nm2=-1; break;
		case 41: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR1; break;
		case 42: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2; break;
		case 48: nm1=BGBCC_SH_NMID_SHLR16; nm2=-1; break;
		case 49: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR1; break;
		case 50: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2; break;
		case 56: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8; break;
		default: nm1=-1; nm2=-1; break;
		}

//		if(nm1>=0)
		if(0)
		{
			BGBCC_BSR_EmitOpReg(sctx, nm1, cdreg2+0);
			if(nm2>=0)
				BGBCC_BSR_EmitOpReg(sctx, nm2, cdreg2+0);
		}else
		{
//			BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
//				-(shl-32), cdreg2+1);
//			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD,
//				cdreg2+1, cdreg2+0);

			BGBCC_BSRC_EmitLeaShrRegImm(ctx, sctx, cdreg2+0, shl-32);
		}

		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
			0, cdreg2+1);
		return(1);
	}
	
	if(shl<=3)
	{
		for(i=0; i<shl; i++)
		{
			BGBCC_BSR_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
			BGBCC_BSR_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+1);
			BGBCC_BSR_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+0);
		}
		return(1);
	}
	
//	tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
//	tr2=BGBCC_SH_REG_R0+(tr0&15);

	tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, tr0);

//	BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -shl, tr1);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+0);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+1);
	BGBCC_BSRC_EmitLeaShrRegImm(ctx, sctx, cdreg2+0, shl);
	BGBCC_BSRC_EmitLeaShrRegImm(ctx, sctx, cdreg2+1, shl);

//	BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 32-shl, tr1);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, tr0);
	BGBCC_BSRC_EmitLeaShlRegImm(ctx, sctx, tr0, 32-shl);

	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr0, cdreg2+0);

	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
	return(1);
}

int BGBCC_BSRC_EmitBinaryLong_SarImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int cdreg, int shl)
{
	int tr0, tr1, tr2, tr3;
	int cdreg2;
	int nm1, nm2;
	int i, j, k;

	if(!BGBCC_BSRC_EmitRegIsLpReg(ctx, sctx, cdreg))
		{ BGBCC_DBGBREAK }

	cdreg2=BGBCC_SH_REG_R0+(cdreg&15);

	if(shl==0)
	{
		return(1);
	}

	if(shl>=63)
	{
//		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -31, cdreg2+0);
//		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, cdreg2+0, cdreg2+1);
		BGBCC_BSRC_EmitLeaSarRegImm(ctx, sctx, cdreg2+1, 31);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, cdreg2+0);
		return(1);
	}

	if(shl==32)
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, cdreg2+0);
//		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -31, tr0);
//		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr0, cdreg2+1);
		BGBCC_BSRC_EmitLeaSarRegImm(ctx, sctx, cdreg2+1, 31);

		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(shl>32)
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, cdreg2+0);

//		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -(shl-32), tr0);
//		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr0, cdreg2+0);
		BGBCC_BSRC_EmitLeaSarRegImm(ctx, sctx, cdreg2+0, shl-32);

//		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -31, tr0);
//		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr0, cdreg2+1);
		BGBCC_BSRC_EmitLeaSarRegImm(ctx, sctx, cdreg2+1, 31);

		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}
	
	if(shl<=4)
	{
//		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		for(i=0; i<shl; i++)
		{
//			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+1, tr0);
//			BGBCC_BSR_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCL, tr0);
//			BGBCC_BSR_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+1);
			BGBCC_BSR_EmitOpReg(sctx, BGBCC_SH_NMID_SHAR, cdreg2+1);
			BGBCC_BSR_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+0);
		}
//		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}
	
	tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, tr0);

//	BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, -shl, tr1);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+0);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr1, cdreg2+1);
	BGBCC_BSRC_EmitLeaShrRegImm(ctx, sctx, cdreg2+0, shl);
	BGBCC_BSRC_EmitLeaSarRegImm(ctx, sctx, cdreg2+1, shl);

//	BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 32-shl, tr1);
//	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, tr0);
	BGBCC_BSRC_EmitLeaShlRegImm(ctx, sctx, tr0, 32-shl);

	BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr0, cdreg2+0);

	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
	return(1);
}


int BGBCC_BSRC_EmitBinaryVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg, cdreg2;
	s64 limm, lrcp;
	char *s0;
	int nm1, nm2, nm3, nm4, shl;
	int i, j, k;

	shl=-1; limm=0;
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		shl=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		limm=shl;
	}

	if(BGBCC_CCXL_IsRegImmLongP(ctx, treg))
	{
		limm=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		shl=limm;
	}

	nm1=-1;	nm2=-1;
	nm3=-1;	nm4=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:
		nm1=BGBCC_SH_NMID_ADDC; nm2=nm1;
		nm3=BGBCC_SH_NMID_CLRT;
		break;
	case CCXL_BINOP_SUB:
		nm1=BGBCC_SH_NMID_SUBC; nm2=nm1;
		nm3=BGBCC_SH_NMID_CLRT;
		break;

	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND;	nm2=nm1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR;	nm2=nm1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR;	nm2=nm1; break;
	default:		nm1=-1; nm2=-1; break;
	}
	
	if(nm1>=0)
	{
		cdreg=BGBCC_BSRC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, treg);

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((ctreg>=0) && (ctreg!=BGBCC_SH_REG_ZZR))
			{
				if(nm3>=0)
					BGBCC_BSR_EmitOpNone(sctx, nm3);
				BGBCC_BSR_EmitOpRegReg(sctx, nm1, ctreg+0, cdreg+0);
				BGBCC_BSR_EmitOpRegReg(sctx, nm2, ctreg+1, cdreg+1);

				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}else
			{
				BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR4);

				if(nm3>=0)
					BGBCC_BSR_EmitOpNone(sctx, nm3);
				BGBCC_BSR_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg+0);
				BGBCC_BSR_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, cdreg+1);

				BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}else if((ctreg>=0) && (ctreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			if(nm3>=0)
				BGBCC_BSR_EmitOpNone(sctx, nm3);
			BGBCC_BSR_EmitOpRegReg(sctx, nm1, ctreg+0, BGBCC_SH_REG_R4);
			BGBCC_BSR_EmitOpRegReg(sctx, nm2, ctreg+1, BGBCC_SH_REG_R5);

			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

		if(nm4>=0)
		{
			BGBCC_BSR_EmitOpRegReg(sctx, nm4, BGBCC_SH_REG_R6, BGBCC_SH_REG_R6);
			BGBCC_BSR_EmitOpRegReg(sctx, nm4, BGBCC_SH_REG_R7, BGBCC_SH_REG_R7);
		}

		if(nm3>=0)
			BGBCC_BSR_EmitOpNone(sctx, nm3);
		BGBCC_BSR_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R6, BGBCC_SH_REG_R4);
		BGBCC_BSR_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R7, BGBCC_SH_REG_R5);

		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		return(1);
	}
	
	if(opr==CCXL_BINOP_MUL)
	{
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R6);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R2);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R3);

		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R7);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R0);

		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R6);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R1);

		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);

		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}

#if 1
	if((opr==CCXL_BINOP_DIV) &&
		(BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, treg)) &&
		(limm>2))
	{
//		lrcp=(((1ULL<<63)+(u64)(limm/2))/((u64)limm))*2;
		lrcp=(((1ULL<<63)+(u64)(limm-1))/((u64)limm))*2;
		
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);

//		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_BSR_EmitLoadRegImm64P(sctx, BGBCC_SH_REG_LR6, lrcp);

		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);

		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R6);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R0);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R3);

		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R7);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R0);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R2);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R3, BGBCC_SH_REG_R2);

		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R6);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R1);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R3);

		//Low DWORD Carry
//		BGBCC_BSR_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R2, BGBCC_SH_REG_R3);

		//Mid DWORD Carry
//		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);
//		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

		//Actual Results
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULS,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R7);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R2);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R3);

//		BGBCC_BSR_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R2);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_R2);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, BGBCC_SH_REG_R0);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}
#endif
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__lli_add"; break;
	case CCXL_BINOP_SUB:	s0="__lli_sub"; break;
	case CCXL_BINOP_MUL:	s0="__lli_mul"; break;
	case CCXL_BINOP_DIV:
		s0="__lli_sdiv";
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__lli_udiv";
		break;
	case CCXL_BINOP_MOD:
		s0="__lli_smod";
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__lli_umod";
		break;
	case CCXL_BINOP_AND:	s0="__lli_and"; break;
	case CCXL_BINOP_OR:		s0="__lli_or"; break;
	case CCXL_BINOP_XOR:	s0="__lli_xor"; break;
//	case CCXL_BINOP_SHL:	s0="__lli_shl"; break;
//	case CCXL_BINOP_SHR:	s0="__lli_shr"; break;
	}

	if(s0)
	{
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_SHL:
		s0="__lli_shl";
		break;
	case CCXL_BINOP_SHR:
	case CCXL_BINOP_SHRR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			s0="__lli_shr";
		}else
		{
			s0="__lli_sar";
		}
		break;
	}

#if 1
	if((opr==CCXL_BINOP_SHL) && (shl>=0))
	{
		cdreg=BGBCC_BSRC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_BSRC_EmitBinaryLong_ShlImm(ctx, sctx, cdreg, shl);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 1
	if((opr==CCXL_BINOP_SHR) && (shl>=0) &&
		BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		cdreg=BGBCC_BSRC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_BSRC_EmitBinaryLong_ShrImm(ctx, sctx, cdreg, shl);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 1
	if((opr==CCXL_BINOP_SHRR) && (shl>=0))
	{
		cdreg=BGBCC_BSRC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_BSRC_EmitBinaryLong_ShrImm(ctx, sctx, cdreg, shl);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 1
	if((opr==CCXL_BINOP_SHR) && (shl>=0) &&
		!BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		cdreg=BGBCC_BSRC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_BSRC_EmitBinaryLong_SarImm(ctx, sctx, cdreg, shl);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 0
	if((opr==CCXL_BINOP_SHL) && (shl==32))
	{
		cdreg=BGBCC_BSRC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			cdreg2=BGBCC_SH_REG_R0+(cdreg&15);
			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+0, cdreg2+1);
			BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
				0, cdreg2+0);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R5);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
			0, BGBCC_SH_REG_R4);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}

	if((opr==CCXL_BINOP_SHR) && (shl==32) &&
		BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		cdreg=BGBCC_BSRC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			cdreg2=BGBCC_SH_REG_R0+(cdreg&15);
			BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+1, cdreg2+0);
			BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
				0, cdreg2+1);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSR_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R4);
		BGBCC_BSR_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
			0, BGBCC_SH_REG_R5);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}
#endif

	if(s0)
	{
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R6);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R6);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitBinaryVRegVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_BSRC_EmitBinaryVRegVRegLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_BSRC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_BSRC_EmitBinaryVRegVRegLong(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_BSRC_EmitUnaryVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
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
		BGBCC_BSRC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}

	nm1=-1;	nm2=-1;
	nm3=-1;	nm4=-1;
	switch(opr)
	{
	case CCXL_UNOP_NEG:
		nm1=BGBCC_SH_NMID_NEGC; nm2=nm1;
		nm3=BGBCC_SH_NMID_CLRT;
		break;
	case CCXL_UNOP_NOT:
		nm1=BGBCC_SH_NMID_NOT; nm2=nm1;
//		nm3=BGBCC_SH_NMID_CLRT;
		break;

	case CCXL_UNOP_INC:
		nm1=BGBCC_SH_NMID_ADDC; nm2=nm1;
		nm3=BGBCC_SH_NMID_SETT;
		break;
	case CCXL_UNOP_DEC:
		nm1=BGBCC_SH_NMID_SUBC; nm2=nm1;
		nm3=BGBCC_SH_NMID_SETT;
		break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		cdreg=BGBCC_BSRC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
			{
				if(nm3>=0)
					BGBCC_BSR_EmitOpNone(sctx, nm3);
				BGBCC_BSR_EmitOpRegReg(sctx, nm1,
					csreg+0, cdreg+0);
				BGBCC_BSR_EmitOpRegReg(sctx, nm2,
					csreg+1, cdreg+1);

				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}else
			{
				BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

				if(nm3>=0)
					BGBCC_BSR_EmitOpNone(sctx, nm3);
				BGBCC_BSR_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg+0);
				BGBCC_BSR_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, cdreg+1);

				BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}else if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);

			if(nm3>=0)
				BGBCC_BSR_EmitOpNone(sctx, nm3);
			BGBCC_BSR_EmitOpRegReg(sctx, nm1, csreg+0, BGBCC_SH_REG_R4);
			BGBCC_BSR_EmitOpRegReg(sctx, nm2, csreg+1, BGBCC_SH_REG_R5);

			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

		if(nm3>=0)
			BGBCC_BSR_EmitOpNone(sctx, nm3);
		BGBCC_BSR_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
		BGBCC_BSR_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, BGBCC_SH_REG_R5);

		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}

	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:		s0="__lli_neg"; break;
	case CCXL_UNOP_NOT:		s0="__lli_not"; break;
	case CCXL_UNOP_LNOT:	s0="__lli_lnot"; break;
	case CCXL_UNOP_INC:		s0="__lli_inc"; break;
	case CCXL_UNOP_DEC:		s0="__lli_dec"; break;
	}

	if(s0)
	{
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_BSRC_EmitJCmpVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2, nm3, nm4, nm5, nm6;
	int fl1, fl3, fl5;
	int l0, l1, l2;

	nm1=-1;		nm2=-1;
	nm3=-1;		nm4=-1;
	nm5=-1;		nm6=-1;
	fl1=0;	fl3=0;	fl5=0;
	l0=lbl; l1=lbl; l2=lbl;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;		nm2=BGBCC_SH_NMID_BF;	fl1=0;
		nm3=BGBCC_SH_NMID_CMPEQ;		nm4=BGBCC_SH_NMID_BT;	fl3=0;
		l0=BGBCC_BSR_GenLabel(sctx);
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;		nm2=BGBCC_SH_NMID_BF;	fl1=0;
		nm3=BGBCC_SH_NMID_CMPEQ;		nm4=BGBCC_SH_NMID_BF;	fl3=0;
		break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;	nm2=BGBCC_SH_NMID_BT;	fl1=0;
//			nm5=BGBCC_SH_NMID_CMPHS;	nm6=BGBCC_SH_NMID_BF;	fl5=0;
//			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BF;	fl3=0;
			nm5=BGBCC_SH_NMID_CMPHI;	nm6=BGBCC_SH_NMID_BT;	fl5=1;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BT;	fl3=1;

		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;	nm2=BGBCC_SH_NMID_BT;	fl1=0;
//			nm5=BGBCC_SH_NMID_CMPGE;	nm6=BGBCC_SH_NMID_BF;	fl5=0;
			nm5=BGBCC_SH_NMID_CMPGT;	nm6=BGBCC_SH_NMID_BT;	fl5=1;
//			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BF;	fl3=0;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BT;	fl3=1;
		}
		l0=BGBCC_BSR_GenLabel(sctx);
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
//			nm1=BGBCC_SH_NMID_CMPHS;	nm2=BGBCC_SH_NMID_BF;	fl1=0;
			nm1=BGBCC_SH_NMID_CMPHI;	nm2=BGBCC_SH_NMID_BT;	fl1=1;
			nm5=BGBCC_SH_NMID_CMPHI;	nm6=BGBCC_SH_NMID_BT;	fl5=0;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BT;	fl3=0;
		}else
		{
//			nm1=BGBCC_SH_NMID_CMPGE;	nm2=BGBCC_SH_NMID_BF;	fl1=0;
			nm1=BGBCC_SH_NMID_CMPGT;	nm2=BGBCC_SH_NMID_BT;	fl1=1;
			nm5=BGBCC_SH_NMID_CMPGT;	nm6=BGBCC_SH_NMID_BT;	fl5=0;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BT;	fl3=0;
		}
		l0=BGBCC_BSR_GenLabel(sctx);
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;	nm2=BGBCC_SH_NMID_BT;	fl1=0;
//			nm5=BGBCC_SH_NMID_CMPHS;	nm6=BGBCC_SH_NMID_BF;	fl5=0;
			nm5=BGBCC_SH_NMID_CMPHI;	nm6=BGBCC_SH_NMID_BT;	fl5=1;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BF;	fl3=0;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;	nm2=BGBCC_SH_NMID_BT;	fl1=0;
//			nm5=BGBCC_SH_NMID_CMPGE;	nm6=BGBCC_SH_NMID_BF;	fl5=0;
			nm5=BGBCC_SH_NMID_CMPGT;	nm6=BGBCC_SH_NMID_BT;	fl5=1;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BF;	fl3=0;
		}
		l0=BGBCC_BSR_GenLabel(sctx);
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
//			nm1=BGBCC_SH_NMID_CMPHS;	nm2=BGBCC_SH_NMID_BF;	fl1=0;
			nm1=BGBCC_SH_NMID_CMPHI;	nm2=BGBCC_SH_NMID_BT;	fl1=1;
			nm5=BGBCC_SH_NMID_CMPHI;	nm6=BGBCC_SH_NMID_BT;	fl5=0;
//			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BT;	fl3=0;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BF;	fl3=1;
		}else
		{
//			nm1=BGBCC_SH_NMID_CMPGE;	nm2=BGBCC_SH_NMID_BF;	fl1=0;
			nm1=BGBCC_SH_NMID_CMPGT;	nm2=BGBCC_SH_NMID_BT;	fl1=1;
			nm5=BGBCC_SH_NMID_CMPGT;	nm6=BGBCC_SH_NMID_BT;	fl5=0;
//			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BT;	fl3=0;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BF;	fl3=1;
		}
		l0=BGBCC_BSR_GenLabel(sctx);
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);

		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, treg);

		if(fl1)
			BGBCC_BSR_EmitOpRegReg(sctx, nm1, csreg+1, ctreg+1);
		else
			BGBCC_BSR_EmitOpRegReg(sctx, nm1, ctreg+1, csreg+1);
		BGBCC_BSR_EmitOpAutoLabel(sctx, nm2, l0);
		if(nm5>=0)
		{
			if(fl5)
				BGBCC_BSR_EmitOpRegReg(sctx, nm5, csreg+1, ctreg+1);
			else
				BGBCC_BSR_EmitOpRegReg(sctx, nm5, ctreg+1, csreg+1);
			BGBCC_BSR_EmitOpAutoLabel(sctx, nm6, l2);
		}

		if(fl3)
			BGBCC_BSR_EmitOpRegReg(sctx, nm3, csreg+0, ctreg+0);
		else
			BGBCC_BSR_EmitOpRegReg(sctx, nm3, ctreg+0, csreg+0);
		BGBCC_BSR_EmitOpAutoLabel(sctx, nm4, l1);

//		if(l0!=lbl)
//			BGBCC_BSR_EmitOpLabel(sctx, nm2, l0);
//		if(l1!=lbl)
//			BGBCC_BSR_EmitOpLabel(sctx, nm2, l1);

		if(l0!=lbl)
			BGBCC_BSR_EmitLabel(sctx, l0);

		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	return(0);
}
