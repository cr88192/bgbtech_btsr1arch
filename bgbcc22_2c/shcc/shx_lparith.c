/*
 * Long Long Arithmetic
 */

int BGBCC_SHXC_EmitBinaryLong_ShlImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg2+0, 0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, 0);
		return(1);
	}

	if(shl==32)
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+0, cdreg2+1);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+0, 0);
		return(1);
	}

	if(shl>32)
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
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

		if(nm1>=0)
		{
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg2+1);
			if(nm2>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg2+1);
		}else
		{
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+0, shl-32);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD,
				cdreg2+0, cdreg2+1);
		}

		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+0, 0);
		return(1);
	}
	
	if(shl<=3)
	{
		for(i=0; i<shl; i++)
		{
			BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC, cdreg2+0, cdreg2+0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC, cdreg2+1, cdreg2+1);
		}
		return(1);
	}
	
//	tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
//	tr2=BGBCC_SH_REG_R0+(tr0&15);

	tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+0, tr0);
	BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, shl);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+1);
	BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, -(32-shl));
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, tr0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr0, cdreg2+1);

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
	return(1);
}

int BGBCC_SHXC_EmitBinaryLong_ShrImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg2+0, 0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, 0);
		return(1);
	}

	if(shl==32)
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+1, cdreg2+0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+1, 0);
		return(1);
	}

	if(shl>32)
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+1, cdreg2+0);
		switch(shl)
		{
		case 33: nm1=BGBCC_SH_NMID_SHLR; nm2=-1; break;
		case 34: nm1=BGBCC_SH_NMID_SHLR2; nm2=-1; break;
		case 35: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR; break;
		case 36: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2; break;
		case 40: nm1=BGBCC_SH_NMID_SHLR8; nm2=-1; break;
		case 41: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR; break;
		case 42: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2; break;
		case 48: nm1=BGBCC_SH_NMID_SHLR16; nm2=-1; break;
		case 49: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR; break;
		case 50: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2; break;
		case 56: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8; break;
		default: nm1=-1; nm2=-1; break;
		}

		if(nm1>=0)
		{
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg2+0);
			if(nm2>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg2+0);
		}else
		{
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+1, -(shl-32));
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD,
				cdreg2+1, cdreg2+0);
		}

		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
			cdreg2+1, 0);
		return(1);
	}
	
	if(shl<=3)
	{
		for(i=0; i<shl; i++)
		{
			BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+1);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+0);
		}
		return(1);
	}
	
//	tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
//	tr2=BGBCC_SH_REG_R0+(tr0&15);

	tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, tr0);
	BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, -shl);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+1);
	BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, 32-shl);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, tr0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr0, cdreg2+0);

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
	return(1);
}

int BGBCC_SHXC_EmitBinaryLong_SarImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int cdreg, int shl)
{
	int tr0, tr1, tr2, tr3;
	int cdreg2;
	int nm1, nm2;
	int i, j, k;

	if(!BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, cdreg))
		{ BGBCC_DBGBREAK }

	cdreg2=BGBCC_SH_REG_R0+(cdreg&15);

	if(shl==0)
	{
		return(1);
	}

	if(shl>=63)
	{
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,	cdreg2+0, -31);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, cdreg2+0, cdreg2+1);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, cdreg2+0);
		return(1);
	}

	if(shl==32)
	{
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, cdreg2+0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr0, -31);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr0, cdreg2+1);
//		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,	cdreg2+1, -1);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(shl>32)
	{
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, cdreg2+0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr0, -(shl-32));
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr0, cdreg2+0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr0, -31);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr0, cdreg2+1);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}
	
	if(shl<=4)
	{
//		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		for(i=0; i<shl; i++)
		{
//			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+1, tr0);
//			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCL, tr0);
//			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+1);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHAR, cdreg2+1);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_ROTCR, cdreg2+0);
		}
//		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}
	
	tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, cdreg2+1, tr0);
	BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, -shl);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, cdreg2+0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, tr1, cdreg2+1);
	BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, 32-shl);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHLD, tr1, tr0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr0, cdreg2+0);

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
	return(1);
}


int BGBCC_SHXC_EmitBinaryVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
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

	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=nm1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=nm1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=nm1; break;
	default:		nm1=-1; nm2=-1; break;
	}
	
	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((ctreg>=0) && (ctreg!=BGBCC_SH_REG_ZZR))
			{
				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg+0, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg+1, cdreg+1);

				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}else
			{
				BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR4);

				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, cdreg+1);

				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}else if((ctreg>=0) && (ctreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			if(nm3>=0)
				BGBCC_SHX_EmitOpNone(sctx, nm3);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg+0, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg+1, BGBCC_SH_REG_R5);

			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

		if(nm4>=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm4, BGBCC_SH_REG_R6, BGBCC_SH_REG_R6);
			BGBCC_SHX_EmitOpRegReg(sctx, nm4, BGBCC_SH_REG_R7, BGBCC_SH_REG_R7);
		}

		if(nm3>=0)
			BGBCC_SHX_EmitOpNone(sctx, nm3);
		BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R6, BGBCC_SH_REG_R4);
		BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R7, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		return(1);
	}
	
	if(opr==CCXL_BINOP_MUL)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R6);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R3);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R7);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R0);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R6);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R1);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
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
		
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);

//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHX_EmitLoadRegImm64P(sctx, BGBCC_SH_REG_LR6, lrcp);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R6);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R3);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R7);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R3, BGBCC_SH_REG_R2);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULU,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R6);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R1);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R3);

		//Low DWORD Carry
//		BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R2, BGBCC_SH_REG_R3);

		//Mid DWORD Carry
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

		//Actual Results
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_DMULS,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R7);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACH, BGBCC_SH_REG_R3);

//		BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_CLRT);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, 0);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADDC,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}
#endif
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__addlli"; break;
	case CCXL_BINOP_SUB:	s0="__sublli"; break;
	case CCXL_BINOP_MUL:	s0="__mullli"; break;
	case CCXL_BINOP_DIV:
		s0="__sdivlli";
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__udivlli";
		break;
	case CCXL_BINOP_MOD:
		s0="__smodlli";
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__umodlli";
		break;
	case CCXL_BINOP_AND:	s0="__andlli"; break;
	case CCXL_BINOP_OR:		s0="__orlli"; break;
	case CCXL_BINOP_XOR:	s0="__xorlli"; break;
//	case CCXL_BINOP_SHL:	s0="__shllli"; break;
//	case CCXL_BINOP_SHR:	s0="__shrlli"; break;
	}

	if(s0)
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

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_SHL:
		s0="__shllli";
#if 0
		switch(shl)
		{
		case 1: s0="__shllli_1"; break;
		case 2: s0="__shllli_2"; break;
		case 3: s0="__shllli_3"; break;
		case 4: s0="__shllli_4"; break;
		case 8: s0="__shllli_8"; break;
		case 16: s0="__shllli_16"; break;
		case 24: s0="__shllli_24"; break;
		case 32: s0="__shllli_32"; break;
		case 40: s0="__shllli_40"; break;
		case 48: s0="__shllli_48"; break;
		case 56: s0="__shllli_56"; break;
		default: break;
		}
#endif
		break;
	case CCXL_BINOP_SHR:
	case CCXL_BINOP_SHRR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			s0="__shrlli";
#if 0
			switch(shl)
			{
			case 1: s0="__shrlli_1"; break;
			case 2: s0="__shrlli_2"; break;
			case 3: s0="__shrlli_3"; break;
			case 4: s0="__shrlli_4"; break;
			case 8: s0="__shrlli_8"; break;
			case 16: s0="__shrlli_16"; break;
			case 24: s0="__shrlli_24"; break;
			case 32: s0="__shrlli_32"; break;
			case 40: s0="__shrlli_40"; break;
			case 48: s0="__shrlli_48"; break;
			case 56: s0="__shrlli_56"; break;
			default: break;
			}
#endif
		}else
		{
			s0="__sarlli";
#if 0
			switch(shl)
			{
			case 1: s0="__sarlli_1"; break;
			case 2: s0="__sarlli_2"; break;
			case 3: s0="__sarlli_3"; break;
			case 4: s0="__sarlli_4"; break;
			case 8: s0="__sarlli_8"; break;
			case 16: s0="__sarlli_16"; break;
			case 24: s0="__sarlli_24"; break;
			case 32: s0="__sarlli_32"; break;
			case 40: s0="__sarlli_40"; break;
			case 48: s0="__sarlli_48"; break;
			case 56: s0="__sarlli_56"; break;
			default: break;
			}
#endif
		}
		break;
	}

#if 1
	if((opr==CCXL_BINOP_SHL) && (shl>=0))
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_SHXC_EmitBinaryLong_ShlImm(ctx, sctx, cdreg, shl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 1
	if((opr==CCXL_BINOP_SHR) && (shl>=0) &&
		BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_SHXC_EmitBinaryLong_ShrImm(ctx, sctx, cdreg, shl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 1
	if((opr==CCXL_BINOP_SHR) && (shl>=0) &&
		!BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		i=BGBCC_SHXC_EmitBinaryLong_SarImm(ctx, sctx, cdreg, shl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(i);
	}
#endif

#if 0
	if((opr==CCXL_BINOP_SHL) && (shl==32))
	{
		cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			cdreg2=BGBCC_SH_REG_R0+(cdreg&15);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+0, cdreg2+1);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+0, 0);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R5);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R4, 0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}

	if((opr==CCXL_BINOP_SHR) && (shl==32) &&
		BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			cdreg2=BGBCC_SH_REG_R0+(cdreg&15);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+1, cdreg2+0);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
				cdreg2+1, 0);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R5, BGBCC_SH_REG_R4);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R5, 0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}
#endif

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R6);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegLong(
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
		i=BGBCC_SHXC_EmitBinaryVRegVRegLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegLong(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegLong(
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
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
			{
				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1,
					csreg+0, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2,
					csreg+1, cdreg+1);

				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}else
			{
				BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, cdreg+1);

				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}else if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);

			if(nm3>=0)
				BGBCC_SHX_EmitOpNone(sctx, nm3);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg+0, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, csreg+1, BGBCC_SH_REG_R5);

			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

		if(nm3>=0)
			BGBCC_SHX_EmitOpNone(sctx, nm3);
		BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
		BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}

	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:		s0="__neglli"; break;
	case CCXL_UNOP_NOT:		s0="__notlli"; break;
	case CCXL_UNOP_LNOT:	s0="__lnotlli"; break;
	case CCXL_UNOP_INC:		s0="__inclli"; break;
	case CCXL_UNOP_DEC:		s0="__declli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitJCmpVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2, nm3, nm4, nm5, nm6;
	int l0, l1, l2;

	nm1=-1;		nm2=-1;
	nm3=-1;		nm4=-1;
	nm5=-1;		nm6=-1;
	l0=lbl; l1=lbl; l2=lbl;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_CMPEQ;		nm4=BGBCC_SH_NMID_BT;
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_CMPEQ;		nm4=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;	nm2=BGBCC_SH_NMID_BT;
			nm5=BGBCC_SH_NMID_CMPHS;	nm6=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BF;

		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;	nm2=BGBCC_SH_NMID_BT;
			nm5=BGBCC_SH_NMID_CMPGE;	nm6=BGBCC_SH_NMID_BF;
//			nm3=BGBCC_SH_NMID_CMPGE;	nm4=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BF;
		}
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHS;	nm2=BGBCC_SH_NMID_BF;
			nm5=BGBCC_SH_NMID_CMPHI;	nm6=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGE;	nm2=BGBCC_SH_NMID_BF;
			nm5=BGBCC_SH_NMID_CMPGT;	nm6=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_CMPGT;	nm4=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BT;
		}
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;	nm2=BGBCC_SH_NMID_BT;
			nm5=BGBCC_SH_NMID_CMPHS;	nm6=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;	nm2=BGBCC_SH_NMID_BT;
			nm5=BGBCC_SH_NMID_CMPGE;	nm6=BGBCC_SH_NMID_BF;
//			nm3=BGBCC_SH_NMID_CMPGT;	nm4=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_CMPHI;	nm4=BGBCC_SH_NMID_BF;
		}
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHS;	nm2=BGBCC_SH_NMID_BF;
			nm5=BGBCC_SH_NMID_CMPHI;	nm6=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGE;	nm2=BGBCC_SH_NMID_BF;
			nm5=BGBCC_SH_NMID_CMPGT;	nm6=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_CMPGE;	nm4=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_CMPHS;	nm4=BGBCC_SH_NMID_BT;
		}
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg+1, csreg+1);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, l0);
		if(nm5>=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm5, ctreg+1, csreg+1);
			BGBCC_SHX_EmitOpAutoLabel(sctx, nm6, l2);
		}
		BGBCC_SHX_EmitOpRegReg(sctx, nm3, ctreg+0, csreg+0);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm4, l1);

//		if(l0!=lbl)
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, l0);
//		if(l1!=lbl)
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, l1);

		if(l0!=lbl)
			BGBCC_SHX_EmitLabel(sctx, l0);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	return(0);
}
