int BGBCC_JX2C_NormalizeImmVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register *rtreg)
{
	ccxl_register treg, treg2;
	double f;
	s64 li;

	treg=*rtreg;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
				return(1);
		
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			BGBCC_CCXL_GetRegForIntValue(ctx, &treg2, li);
			*rtreg=treg2;
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
		{
			if(BGBCC_CCXL_IsRegImmLongP(ctx, treg))
				return(1);

			li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
			BGBCC_CCXL_GetRegForLongValue(ctx, &treg2, li);
			*rtreg=treg2;
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
			BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
				return(1);

			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
			BGBCC_CCXL_GetRegForFloatValue(ctx, &treg2, f);
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
	}

	return(0);
}
 
int BGBCC_JX2C_EmitBinaryVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	char *sn0;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4;
	int i, j, k;

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
//	if(0)
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

//		nm1=-1;
//		if((opr==CCXL_BINOP_AND) && (j==255))
//			nm1=BGBCC_SH_NMID_EXTUB;
//		if((opr==CCXL_BINOP_AND) && (j==65535))
//			nm1=BGBCC_SH_NMID_EXTUW;

		nm1=-1; k=0;
		if((opr==CCXL_BINOP_ADD) && (j==(sbyte)j))
			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
		if((opr==CCXL_BINOP_SUB) && ((-j)==(sbyte)(-j)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

//		if(sctx->has_bjx1ari && (nm1<0))
		if(nm1<0)
		{
			if((opr==CCXL_BINOP_ADD) && (j==(s16)j))
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if((opr==CCXL_BINOP_SUB) && ((-j)==(s16)(-j)))
				{ nm1=BGBCC_SH_NMID_ADD; k=-j; }
		}

		if(nm1>=0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			i=BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, k, cdreg);
			if(i<=0)
			{
				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD; nm2=-1; break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB; nm2=-1; break;
		case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=-1; break;
		case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=-1; break;
		case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=-1; break;
		default:		nm1=-1; nm2=-1; break;
		}

		if(nm1>=0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			k=j; nm3=nm1;
			if(opr==CCXL_BINOP_SUB)
				{ nm3=BGBCC_SH_NMID_ADD; k=-j; }
			i=BGBCC_JX2_TryEmitOpImmReg(sctx, nm3, k, cdreg);
			if(i>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
			
			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, j);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}
		
		nm1=-1;	nm2=-1;
		nm3=-1;	nm4=-1;

		if(opr==CCXL_BINOP_SHL)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitLeaShlRegImm(ctx, sctx, cdreg, j);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if((opr==CCXL_BINOP_SHR) || (opr==CCXL_BINOP_SHRR))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type) || (opr==CCXL_BINOP_SHRR))
				BGBCC_JX2C_EmitLeaShrRegImm(ctx, sctx, cdreg, j);
			else
				BGBCC_JX2C_EmitLeaSarRegImm(ctx, sctx, cdreg, j);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

#if 0
#if 1
		if(opr==CCXL_BINOP_SHL)
		{
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLL1; break;
			case 2: nm1=BGBCC_SH_NMID_SHLL2; break;
			case 3: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 5: nm1=BGBCC_SH_NMID_SHLL4; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 6: nm1=BGBCC_SH_NMID_SHLL4; nm2=BGBCC_SH_NMID_SHLL2; break;

			case 8: nm1=BGBCC_SH_NMID_SHLL8; break;
			case 9: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 10: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2; break;

			case 16: nm1=BGBCC_SH_NMID_SHLL16; break;
			case 17: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 18: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 24: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL8; break;
			}
		}

		if((opr==CCXL_BINOP_SHRR) ||
			((opr==CCXL_BINOP_SHR) &&
				BGBCC_CCXL_TypeUnsignedP(ctx, type)))
		{
			nm1=-1;	nm2=-1;
			nm3=-1;	nm4=-1;
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLR; break;
			case 2: nm1=BGBCC_SH_NMID_SHLR2; break;
			case 8: nm1=BGBCC_SH_NMID_SHLR8; break;
			case 16: nm1=BGBCC_SH_NMID_SHLR16; break;

			case 3: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR; break;
			case 4: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2; break;
			case 9: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR; break;
			case 10: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2; break;
			case 17: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR; break;
			case 18: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2; break;
			case 24: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8; break;
			}
			
			if(sctx->is_addr64)
				nm1=-1;
		}

		if((opr==CCXL_BINOP_SHR) && (j==1) &&
			!BGBCC_CCXL_TypeUnsignedP(ctx, type) && !sctx->is_addr64)
				nm1=BGBCC_SH_NMID_SHAR;
#endif

		if(sctx->has_bjx1ari && (nm2>=0))
			{ nm1=-1; }

		if(nm1>=0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, cdreg);
			if(nm2>=0)
				BGBCC_JX2C_EmitOpReg(ctx, sctx, nm2, cdreg);
			if(nm3>=0)
				BGBCC_JX2C_EmitOpReg(ctx, sctx, nm3, cdreg);
			if(nm4>=0)
				BGBCC_JX2C_EmitOpReg(ctx, sctx, nm4, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if(sctx->has_shad)
		{
			if(opr==CCXL_BINOP_SHL)
			{
				nm1=BGBCC_SH_NMID_SHAD; k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_SHLD;
			}
			if(opr==CCXL_BINOP_SHR)
			{
				nm1=BGBCC_SH_NMID_SHAD; k=-j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_SHLD;
			}
		}

		if(nm1>=0)
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			if(BGBCC_JX2_TryEmitOpRegImmReg(sctx, nm1, cdreg, k, cdreg)>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
//				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
				return(1);
			}

//			BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);
			
			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}
#endif

#if 1
		if((opr==CCXL_BINOP_DIV) && (j>0))
		{
			if(j==1)
				{ return(1); }

			if(j==2)
			{
//				BGBCC_JX2C_SetModeDqClear(ctx, sctx);
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpReg(ctx, sctx,
					BGBCC_SH_NMID_SHAR, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

//			BGBCC_JX2C_SetModeDqClear(ctx, sctx);
//			k=(s32)((4294967296LL+(j/2))/j);
			k=(s32)((4294967296LL+(j-1))/j);

			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				ctreg, k);

			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_DMULU, ctreg, cdreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_DMULS, ctreg, cdreg);
			}

			if(BGBCC_JX2_EmitCheckRegExt4(sctx, cdreg))
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
					BGBCC_SH_REG_MACH, ctreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV,
					ctreg, cdreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
					BGBCC_SH_REG_MACH, cdreg);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

		if((opr==CCXL_BINOP_MOD) && (j>0))
		{
			if(j==1)
				{ return(1); }

//			if(j==2)
			if((j>0) && !(j&(j-1)))
			{
				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, j-1);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_AND, ctreg, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
				return(1);
			}

//			BGBCC_JX2C_SetModeDqClear(ctx, sctx);
//			k=(s32)((4294967296LL+(j/2))/j);
			k=(s32)((4294967296LL+(j-1))/j);

			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			csreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);

			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_DMULU, ctreg, cdreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_DMULS, ctreg, cdreg);
			}

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACH, csreg);

			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				ctreg, j);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_DMULU, ctreg, csreg);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACL, csreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_SUB, csreg, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

			return(1);
		}
#endif
	}
	
	nm1=-1; nm2=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD;	nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB;	nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND;	nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR;	nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR;	nm2=-1; break;

	case CCXL_BINOP_SHL:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;	nm2=-1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;

	case CCXL_BINOP_SHR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;
			nm2=BGBCC_SH_NMID_NEG;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;
	case CCXL_BINOP_SHRR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHLD;
			nm2=BGBCC_SH_NMID_NEG;
			break;
		}
		break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
//		if((nm1==BGBCC_SH_NMID_SHAD) || (nm1==BGBCC_SH_NMID_SHLD))
//			{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
			cdreg=BGBCC_JX2C_EmitTryGetRegisterDirty(ctx, sctx, dreg);

			if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

			if(nm2>=0)
			{
				csreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, cdreg, csreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, cdreg, cdreg);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterDirty(ctx, sctx, dreg);

		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

//		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		if(nm2>=0)
		{
			csreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, ctreg, csreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
		}
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

#if 1
	if(opr==CCXL_BINOP_MUL)
	{
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterDirty(ctx, sctx, dreg);

		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

#if 1
		i=BGBCC_JX2_TryEmitOpRegRegReg(sctx,
			BGBCC_SH_NMID_MULL, ctreg, cdreg, cdreg);
		if(i>0)
		{
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

//		BGBCC_JX2C_SetModeDqClear(ctx, sctx);

//		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MULL, ctreg, cdreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
#endif

	sn0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_DIV:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ sn0="__udivsi3"; }
		else
			{ sn0="__sdivsi3"; }
		break;
	case CCXL_BINOP_MOD:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ sn0="__umodsi3"; }
		else
			{ sn0="__smodsi3"; }
		break;
	case CCXL_BINOP_SHL:
		sn0="__ashlsi3";
		break;
	case CCXL_BINOP_SHR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ sn0="__lshrsi3"; }
		else
			{ sn0="__ashrsi3"; }
		break;
	case CCXL_BINOP_SHRR:
		sn0="__lshrsi3";
		break;
	}

	if(sn0)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);

		BGBCC_JX2C_EmitCallName(ctx, sctx, sn0);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		return(1);
	}

#if 0
	if(opr==CCXL_BINOP_DIV)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__udivsi3");
		}else
		{
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__sdivsi3");
		}

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
//		BGBCC_JX2C_EmitCallName(ctx, sctx, "__moddi3");

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__umodsi3");
		}else
		{
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__smodsi3");
		}

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int tr0;
	int nm1, nm2, nm3;
	int i, j, k, shl;

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

		if((j==0) && ((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB) ||
			(opr==CCXL_BINOP_OR) || (opr==CCXL_BINOP_XOR)))
		{
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
			return(1);
		}

//		if(sctx->has_bjx1ari)
//		if(sctx->has_bjx1ari && sctx->has_bjx1mov)
		if(1)
		{
			shl=-1;
			if(!(j&(j-1)))
			{
				for(i=0; i<31; i++)
					if(j==(1<<i))
						{ shl=i; break; }
			}

			nm1=-1; k=0;

#if 0
			if((opr==CCXL_BINOP_ADD) && (j==(byte)j))
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if((opr==CCXL_BINOP_SUB) && (j==(byte)j))
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//			if((opr==CCXL_BINOP_MUL) && (j==(sbyte)j))
//				{ nm1=BGBCC_SH_NMID_MULL; k=j; }
#endif

#if 0
			if((opr==CCXL_BINOP_ADD) && ((-j)==(byte)(-j)))
				{ nm1=BGBCC_SH_NMID_SUB; k=-j; }
			if((opr==CCXL_BINOP_SUB) && ((-j)==(byte)(-j)))
				{ nm1=BGBCC_SH_NMID_ADD; k=-j; }
#endif

#if 0
			if((opr==CCXL_BINOP_AND) && (j==(byte)j))
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if((opr==CCXL_BINOP_OR) && (j==(byte)j))
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if((opr==CCXL_BINOP_XOR) && (j==(byte)j))
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }
#endif

			if(opr==CCXL_BINOP_ADD)
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if(opr==CCXL_BINOP_SUB)
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
			if(opr==CCXL_BINOP_AND)
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if(opr==CCXL_BINOP_OR)
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if(opr==CCXL_BINOP_XOR)
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }

#if 0
			if((nm1<0) && (j==((j<<12)>>12)))
			{
				if(opr==CCXL_BINOP_ADD)
					{ nm1=BGBCC_SH_NMID_ADD; k=j; }
				if(opr==CCXL_BINOP_SUB)
					{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//				if(opr==CCXL_BINOP_MUL)
//					{ nm1=BGBCC_SH_NMID_MULL; k=j; }

				if(opr==CCXL_BINOP_AND)
					{ nm1=BGBCC_SH_NMID_AND; k=j; }
				if(opr==CCXL_BINOP_OR)
					{ nm1=BGBCC_SH_NMID_OR; k=j; }
				if(opr==CCXL_BINOP_XOR)
					{ nm1=BGBCC_SH_NMID_XOR; k=j; }

//				if((opr==CCXL_BINOP_ADD) && (j==(s16)j))
//					{ nm1=-1; }		/* more efficient special case */
//				if(j==((j<<20)>>20))
//					{ nm1=-1; }		/* more efficient special case */
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
#if 1
				if((opr==CCXL_BINOP_SHL) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=j; }
				if((opr==CCXL_BINOP_SHR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-j; }
				if((opr==CCXL_BINOP_SHRR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-j; }

				if(opr==CCXL_BINOP_MUL && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHLD; k=shl; }
				if(opr==CCXL_BINOP_DIV && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-shl; }

				if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
					{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
#endif
			}else
			{
				if((opr==CCXL_BINOP_SHL) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHAD; k=j; }
				if((opr==CCXL_BINOP_SHR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHAD; k=-j; }
				if((opr==CCXL_BINOP_SHRR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-j; }

#if 1
				if(opr==CCXL_BINOP_MUL && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHAD; k=shl; }
//				if(opr==CCXL_BINOP_DIV && (shl>=0))
//					{ nm1=BGBCC_SH_NMID_SHAD; k=-shl; }

				if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
					{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
#endif
			}
#endif

			if(nm1>=0)
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
				i=BGBCC_JX2_TryEmitOpRegImmReg(sctx, nm1, csreg, k, cdreg);
				if(i>0)
				{
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
					return(1);
				}

				BGBCC_DBGBREAK

//				if((nm1==BGBCC_SH_NMID_SHAD) || (nm1==BGBCC_SH_NMID_SHLD))
//					{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

#if 0
				if((nm1==BGBCC_SH_NMID_AND) && ((k==255) || (k==65535)))				{
					nm2=BGBCC_SH_NMID_EXTUB;
					if(k==65535)	nm2=BGBCC_SH_NMID_EXTUW;
					BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, csreg, cdreg);

					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
					return(1);
				}

				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
				BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);

				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
#endif
			}
			
//			if(((sbyte)j)==j)
//			{
//				printf("BGBCC_JX2C_EmitBinaryVRegVRegVRegInt: "
//					"Fallthrough, opr=%d j=%d\n", opr, j);
//			}
		}

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_JX2C_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR))
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegInt(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR))
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegInt(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}
	
	nm1=-1; nm2=-1; nm3=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD; nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB; nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=-1; break;

	case CCXL_BINOP_SHL:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;	nm2=-1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;

	case CCXL_BINOP_SHR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;
			nm2=BGBCC_SH_NMID_NEG;
			nm3=BGBCC_SH_NMID_SHAR;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_SHLD;
				nm3=BGBCC_SH_NMID_SHLR;
			}
			break;
		}
		break;
	case CCXL_BINOP_SHRR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHLD;
			nm2=BGBCC_SH_NMID_NEG;
			break;
		}
		break;

	default:		nm1=-1; nm2=-1; nm3=-1; break;
	}

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		if((nm1==BGBCC_SH_NMID_SHAD) || (nm1==BGBCC_SH_NMID_SHLD))
//			BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

//		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(nm3>=0)
		{
			i=BGBCC_JX2_TryEmitOpRegRegReg(sctx,
				nm3, csreg, ctreg, cdreg);
			if(i>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}

//		if((nm2<0) &&
//			(nm1!=BGBCC_SH_NMID_SHAD) &&
//			(nm1!=BGBCC_SH_NMID_SHLD))
		if(nm2<0)
		{
			i=BGBCC_JX2_TryEmitOpRegRegReg(sctx,
				nm1, csreg, ctreg, cdreg);
			if(i>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}

//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		if(nm2>=0)
		{
//			if((nm1==BGBCC_SH_NMID_SHAD) || (nm1==BGBCC_SH_NMID_SHLD))
//				BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, ctreg, tr0);
			if(csreg!=cdreg)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, ctreg, BGBCC_SH_REG_R0);
//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, BGBCC_SH_REG_R0, cdreg);
		}else if(ctreg==cdreg)
		{
//			if((nm1==BGBCC_SH_NMID_SHAD) || (nm1==BGBCC_SH_NMID_SHLD))
//				BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, ctreg, tr0);
			if(csreg!=cdreg)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
//			if((nm1==BGBCC_SH_NMID_SHAD) || (nm1==BGBCC_SH_NMID_SHLD))
//				BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
		}

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_MUL)
	{
		csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

		i=BGBCC_JX2_TryEmitOpRegRegReg(sctx,
			BGBCC_SH_NMID_MULL, csreg, ctreg, cdreg);
		if(i>0)
		{
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

//		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2C_SetModeDqClear(ctx, sctx);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MULL, ctreg, csreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_DIV)
	{
		if(sctx->is_addr64)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RD5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RD4);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD2);
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__udivsi3"); }
			else
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__sdivsi3"); }
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD5);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD2);
		}else
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
	//		BGBCC_JX2C_EmitCallName(ctx, sctx, "__sdivsi3");
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__udivsi3"); }
			else
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__sdivsi3"); }
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		}

		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		if(sctx->is_addr64)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RD5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RD4);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD2);
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__umodsi3"); }
			else
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__smodsi3"); }
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD5);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD2);
		}else
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
	//		BGBCC_JX2C_EmitCallName(ctx, sctx, "__moddi3");
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__umodsi3"); }
			else
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__smodsi3"); }
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		}

		return(1);
	}

	if((opr==CCXL_BINOP_SHL) ||
		(opr==CCXL_BINOP_SHR) ||
		(opr==CCXL_BINOP_SHRR))
	{
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_JX2C_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitBinaryVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		if(sctx->fpu_soft)
			return(BGBCC_JX2C_EmitBinaryVRegVRegVRegSoftFloat(ctx, sctx,
				type, dreg, opr, sreg, treg));
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegFloat(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type) &&
		!(sctx->is_addr64))
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegLong(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if((BGBCC_CCXL_TypeSgLongP(ctx, type) ||
		BGBCC_CCXL_TypeSgNLongP(ctx, type)) &&
		(sctx->is_addr64))
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegInt128(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, type))
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegVRegVariant(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitDstRegOp(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int dreg)
{
	int treg;

	if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, dreg))
	{
		treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2C_EmitOpReg(ctx, sctx, nmid, treg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, treg, dreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_JX2C_EmitOpReg(ctx, sctx, nmid, dreg);
	return(1);
}

int BGBCC_JX2C_EmitUnaryVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	s32 imm;
	int i, j, k;

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		nm1=-1; // k=0;

		if(opr==CCXL_UNOP_MOV)
		{
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, imm);
			return(1);
		}

#if 0
		if(nm1>=0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, imm);
//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, BGBCC_SH_REG_R0, cdreg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R2, imm);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, BGBCC_SH_REG_R2, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif
	}

	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}

	switch(opr)
	{
	case CCXL_UNOP_MOV:	nm1=BGBCC_SH_NMID_MOV; nm2=-1; break;
	case CCXL_UNOP_NEG:	nm1=BGBCC_SH_NMID_NEG; nm2=-1; break;
	case CCXL_UNOP_NOT:	nm1=BGBCC_SH_NMID_NOT; nm2=-1; break;
	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

			if(nm2>=0)
			{
				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, cdreg, ctreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, cdreg, cdreg);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(nm2>=0)
		{
			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, csreg, ctreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
		}
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(opr==CCXL_UNOP_LNOT)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST, cdreg, cdreg);
//			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST, csreg, csreg);
//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if((opr==CCXL_UNOP_INC) ||
		(opr==CCXL_UNOP_DEC))
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			j=(opr==CCXL_UNOP_INC)?1:(-1);
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, j, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		j=(opr==CCXL_UNOP_INC)?1:(-1);
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, csreg, j, cdreg);
		if(i<=0)
//		if(1)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, j, cdreg);
		}
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitUnaryVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitUnaryVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		if(sctx->fpu_soft)
			return(BGBCC_JX2C_EmitUnaryVRegVRegSoftFloat(ctx, sctx,
				type, dreg, opr, sreg));
		return(BGBCC_JX2C_EmitUnaryVRegVRegFloat(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type) &&
		!(sctx->is_addr64))
	{
		return(BGBCC_JX2C_EmitUnaryVRegVRegLong(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if((BGBCC_CCXL_TypeSgLongP(ctx, type) ||
		BGBCC_CCXL_TypeSgNLongP(ctx, type)) &&
		(sctx->is_addr64))
	{
		return(BGBCC_JX2C_EmitUnaryVRegVRegQLong(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		return(BGBCC_JX2C_EmitUnaryVRegVRegInt128(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, type))
	{
		return(BGBCC_JX2C_EmitUnaryVRegVRegVariant(ctx, sctx,
			type, dreg, opr, sreg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}



int BGBCC_JX2C_EmitCompareVRegVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	byte flip, noflip;
	s32 imm;
	int nm1, nm2, cmp1, tr0;
	int i;

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

#if 0
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		nm1=-1; // k=0;
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
//		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
//		nm1=-1; // k=0;

		switch(cmp)
		{
		case CCXL_CMP_EQ:	cmp1=CCXL_CMP_EQ;	break;
		case CCXL_CMP_NE:	cmp1=CCXL_CMP_NE;	break;
		case CCXL_CMP_LT:	cmp1=CCXL_CMP_GT;	break;
		case CCXL_CMP_GT:	cmp1=CCXL_CMP_LT;	break;
		case CCXL_CMP_LE:	cmp1=CCXL_CMP_GE;	break;
		case CCXL_CMP_GE:	cmp1=CCXL_CMP_LE;	break;
		default:			cmp1=-1;			break;
		}

		i=BGBCC_JX2C_EmitCompareVRegVRegVRegInt(ctx, sctx,
			type, dreg, cmp1, treg, sreg);
		return(i);
	}
#endif
	
	noflip=0;
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		noflip=1;
	
	flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
#if 0
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BT;
		break;
#endif

	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BF;
//				nm3=BGBCC_SH_NMID_BRLT;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_BRGT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BF;
//			nm3=BGBCC_SH_NMID_BRLE;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BT;
//				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

#if 0
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, cdreg))
		if(0)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, tr0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV,
				tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}
#endif

		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
		}
		else
		{
//			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		}
		
//		if(nm2==BGBCC_SH_NMID_BF)
//		{
//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
//			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, 1, cdreg);
//		}
		
//		BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitCompareVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type) && !sctx->is_addr64)
	{
		return(BGBCC_JX2C_EmitCompareVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeSgLongP(ctx, type) ||
		BGBCC_CCXL_TypeSgNLongP(ctx, type)) &&
		sctx->is_addr64)
	{
		return(BGBCC_JX2C_EmitCompareVRegVRegVRegQLong(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_JX2C_EmitCompareVRegVRegVRegFloat(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitReturnVoid(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	int i, j, k;

	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
	
//	if((sctx->tr_trnum+1)==ctx->cur_func->n_vtr)
	if(BGBCC_JX2C_BuildFunctionLastTraceP(ctx, sctx))
		return(1);
	
	i=BGBCC_JX2_EmitOpAutoLabel(sctx,
		BGBCC_SH_NMID_BRAN, sctx->lbl_ret);
//	BGBCC_JX2C_EmitOpNone(ctx, sctx,
//		BGBCC_SH_NMID_NOP);
	return(i);
}

int BGBCC_JX2C_EmitNormalizeRegForType(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int creg)
{
//	return(0);

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	if(BGBCC_CCXL_TypeSgIntP(ctx, type))
	{
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			if(BGBCC_JX2_EmitCheckRegNeedZx(sctx, creg))
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTUL, creg);
		}else
		{
			if(BGBCC_JX2_EmitCheckRegNeedSx(sctx, creg))
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, creg);
		}
	}
	return(0);
}

int BGBCC_JX2C_EmitReturnVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register sreg)
{
	int rcls;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	{
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
//	}

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	if(
		(rcls==BGBCC_SH_REGCLS_GR) ||
		(rcls==BGBCC_SH_REGCLS_WGR))
	{
		if(sctx->is_addr64)
		{
//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RD0);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RD2);
//			BGBCC_JX2C_EmitNormalizeRegForType(ctx, sctx,
//				type, BGBCC_SH_REG_RD2);
		}
		else
		{
//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R2);
		}
	}else if(
		(rcls==BGBCC_SH_REGCLS_VO_REF) ||
		(rcls==BGBCC_SH_REGCLS_AR_REF))
	{
		if(sctx->is_addr64)
		{
//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ0);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_RQ2);
		}
		else
		{
//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R2);
		}
	}else if((rcls==BGBCC_SH_REGCLS_GR2))
	{
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR0);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR2);
	}else if((rcls==BGBCC_SH_REGCLS_FR))
	{
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_FR0);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_FR2);
	}else if((rcls==BGBCC_SH_REGCLS_FR2))
	{
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR0);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR2);
	}else if((rcls==BGBCC_SH_REGCLS_DR))
	{
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR0);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR2);
	}else if(rcls==BGBCC_SH_REGCLS_VO_GR)
	{
//		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_R0);
		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R2);
	}else if(rcls==BGBCC_SH_REGCLS_VO_GR2)
	{
//		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_LR0);
		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
			sreg, BGBCC_SH_REG_LR2);
	}else if(rcls==BGBCC_SH_REGCLS_VO_QGR)
	{
//		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
			sreg, BGBCC_SH_REG_RQ2);
	}else if(rcls==BGBCC_SH_REGCLS_QGR)
	{
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_RQ2);
	}else if(rcls==BGBCC_SH_REGCLS_VO_QGR2)
	{
//		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_LR0);
		BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
			sreg, BGBCC_SH_REG_LR2);
	}else
	{
		BGBCC_DBGBREAK
	}

	if(BGBCC_JX2C_BuildFunctionLastTraceP(ctx, sctx))
		return(1);

	i=BGBCC_JX2_EmitOpAutoLabel(sctx,
		BGBCC_SH_NMID_BRAN, sctx->lbl_ret);
//	BGBCC_JX2C_EmitOpNone(ctx, sctx,
//		BGBCC_SH_NMID_NOP);
	return(i);
}

int BGBCC_JX2C_EmitCsrvVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg)
{
	int ctreg;
	int rcls;
	int i, j, k;

	if(sctx->csrv_skip==1)
	{
		sctx->csrv_skip=0;
		return(1);
	}

	sctx->is_leaf=0;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		(BGBCC_CCXL_TypePointerP(ctx, type) && !sctx->is_addr64))
	{
#if 0
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_JX2C_EmitOpReg(ctx, sctx,
				BGBCC_SH_NMID_EXTUL, BGBCC_SH_REG_R2);
		}else
		{
			BGBCC_JX2C_EmitOpReg(ctx, sctx,
				BGBCC_SH_NMID_EXTSL, BGBCC_SH_REG_R2);
		}
#endif

#if 1
//		if(sctx->frm_size>56)
		if(sctx->has_bjx1mov && !sctx->is_pic &&
			BGBCC_CCXL_IsRegGlobalP(ctx, dreg))
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_R2, BGBCC_SH_REG_R3);

//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}
#endif

		if(sctx->no_ext32)
//		if(1)
		{
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_R2, BGBCC_SH_REG_R3);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}

//		ctreg=BGBCC_SH_REG_R0;
//		if(sctx->is_addr64)
//			ctreg=BGBCC_SH_REG_RD0;
		ctreg=BGBCC_SH_REG_R2;
		if(sctx->is_addr64)
			ctreg=BGBCC_SH_REG_RD2;
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, ctreg);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		return(1);
	}
	
	if(	(BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type) ||
		BGBCC_CCXL_TypeVariantP(ctx, type)) &&
		sctx->is_addr64)
	{
#if 1
		if(sctx->has_bjx1mov && !sctx->is_pic &&
			BGBCC_CCXL_IsRegGlobalP(ctx, dreg))
		{
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ3);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}
#endif

		if(sctx->no_ext32)
//		if(1)
		{
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ3);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
		return(1);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, type) ||
		BGBCC_CCXL_TypeVariantP(ctx, type))
	{
#if 0
		if(sctx->frm_size>56)
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R2);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}
#endif
	
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}


	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{	
		if(sctx->fpu_soft)
		{
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
			return(1);
		}

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR2);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		if(sctx->fpu_soft)
		{
			if(sctx->is_addr64)
			{
//				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
//				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);

				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
				return(1);
			}

//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
			return(1);
		}

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR2);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR2);
		return(1);
	}
	
	if(type.val==CCXL_TY_V)
	{
		return(1);
	}

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);
	if(rcls==BGBCC_SH_REGCLS_VO_REF)
	{
		return(1);
	}

	if(rcls==BGBCC_SH_REGCLS_VO_GR)
	{
//		ctreg=BGBCC_SH_REG_R0;
//		if(sctx->is_addr64)
//			ctreg=BGBCC_SH_REG_RD0;

		ctreg=BGBCC_SH_REG_R2;
		if(sctx->is_addr64)
			ctreg=BGBCC_SH_REG_RD2;

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, ctreg);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx, dreg, ctreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		return(1);
	}

	if(rcls==BGBCC_SH_REGCLS_VO_GR2)
	{
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
//			dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
			dreg, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}

	if(rcls==BGBCC_SH_REGCLS_VO_QGR)
	{
		if(sctx->no_ext32)
//		if(1)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//				BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
				dreg, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			return(1);
		}

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
//			dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
			dreg, BGBCC_SH_REG_RQ2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);

		return(1);
	}

	if(rcls==BGBCC_SH_REGCLS_VO_QGR2)
	{
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
//			dreg, BGBCC_SH_REG_LR0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
			dreg, BGBCC_SH_REG_LR2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR2);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCallBuiltin(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name)
{
	int l0, l1, l2;

#if 0
	if(!strcmp(name, "strcpy"))
	{
		l0=BGBCC_JX2_GenLabelTemp(sctx);
		
		BGBCC_JX2_EmitLabel(sctx, l0);

//		BGBCC_JX2_EmitOpLdIncRegReg(sctx,
//			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R5, BGBCC_SH_REG_R3);
		BGBCC_JX2_EmitOpLdRegReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R5, BGBCC_SH_REG_R3);
		BGBCC_JX2_EmitOpRegStReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R3, BGBCC_SH_REG_R4);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, 1, BGBCC_SH_REG_R5);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, 1, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_TST, BGBCC_SH_REG_R3, BGBCC_SH_REG_R3);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);
		return(1);
	}

	if(!strcmp(name, "strcmp"))
	{
		l0=BGBCC_JX2_GenLabelTemp(sctx);
		l1=BGBCC_JX2_GenLabelTemp(sctx);
		
		BGBCC_JX2_EmitLabel(sctx, l0);

		BGBCC_JX2_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R4, BGBCC_SH_REG_R2);
		BGBCC_JX2_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R5, BGBCC_SH_REG_R3);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_TST, BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BT, l1);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_CMPEQ, BGBCC_SH_REG_R2, BGBCC_SH_REG_R3);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BT, l0);

		BGBCC_JX2_EmitLabel(sctx, l1);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R2, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_SUB, BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_SUB, BGBCC_SH_REG_R3, BGBCC_SH_REG_R2);

		return(1);
	}
#endif

	return(0);
}

int BGBCC_JX2C_EmitCallBuiltinArgs(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dst,
	char *name,
	int narg, ccxl_register *args)
{
	int csreg, ctreg, cdreg, cureg, cvreg;
	ccxl_type dty, sty, bty;
	char *s0, *s1;
	int l0, l1;
	int tr0, tr1;
	int sz, al;

	if(!strcmp(name, "__debugbreak"))
	{
		BGBCC_JX2C_EmitOpNone(ctx, sctx, BGBCC_SH_NMID_BRK);
		return(1);
	}

	if(!strcmp(name, "__halt"))
	{
		BGBCC_JX2C_EmitOpNone(ctx, sctx, BGBCC_SH_NMID_SLEEP);
		return(1);
	}

	if(!strcmp(name, "__hint_use_egpr"))
	{
		sctx->use_egpr|=5;
		sctx->csrv_skip=1;
		return(1);
	}

	if(!strcmp(name, "__hint_cc_dbgbreak"))
	{
		__debugbreak();
		sctx->csrv_skip=1;
		return(1);
	}

	if((!strcmp(name, "__float32_getbits") ||
		!strcmp(name, "__float32_frombits") ||
		!strcmp(name, "__float64_getbits") ||
		!strcmp(name, "__float64_frombits")) && (narg==1))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[0]);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dst);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dst);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[0]);
		sctx->csrv_skip=1;
		return(1);
	}

	if(!strcmp(name, "__int_min") && (narg==2))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[0]);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[1]);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dst);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_CMPGT, csreg, ctreg);
		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			BGBCC_SH_NMID_CSELT, csreg, ctreg, cdreg);

//		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dst);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[0]);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[1]);
		sctx->csrv_skip=1;
		return(1);
	}

	if(!strcmp(name, "__int_max") && (narg==2))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[0]);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[1]);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dst);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_CMPGT, csreg, ctreg);
		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			BGBCC_SH_NMID_CSELT, ctreg, csreg, cdreg);

//		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dst);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[0]);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[1]);
		sctx->csrv_skip=1;
		return(1);
	}

	if(!strcmp(name, "__int_clamp") && (narg==3))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[0]);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[1]);
		cureg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[2]);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dst);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_CMPGT, csreg, ctreg);
		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			BGBCC_SH_NMID_CSELT, ctreg, csreg, cdreg);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_CMPGT, cdreg, cureg);
		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			BGBCC_SH_NMID_CSELT, cdreg, cureg, cdreg);

//		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dst);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[0]);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[1]);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[2]);
		sctx->csrv_skip=1;
		return(1);
	}

#if 1
	if(!strcmp(name, "memcpy") && (narg==3))
	{
		if(!BGBCC_CCXL_IsRegImmIntP(ctx, args[2]))
			return(0);

		sz=BGBCC_CCXL_GetRegImmIntValue(ctx, args[2]);

		dty=BGBCC_CCXL_GetRegType(ctx, args[0]);
		sty=BGBCC_CCXL_GetRegType(ctx, args[1]);
		if(dty.val!=sty.val)
			return(0);
		BGBCC_CCXL_TypeDerefType(ctx, dty, &bty);

		sctx->sreg_live=sctx->sreg_held;
		sctx->sfreg_live=sctx->sfreg_held;

		al=1;
		if(BGBCC_CCXL_TypeSgIntP(ctx, bty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, bty) ||
			BGBCC_CCXL_TypeSgLongP(ctx, bty) ||
			BGBCC_CCXL_TypeFloatP(ctx, bty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, type) ||
			BGBCC_CCXL_TypeDoubleP(ctx, bty))
				al=4;
		if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
			al=4;

//		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, args[0], tr0);
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, args[1], tr1);

		tr0=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[0]);
		tr1=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[1]);
		BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, tr0, tr1, sz, al);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[0]);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[1]);

//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if(!strcmp(name, "strcpy") && (narg==2))
	{
		if(!BGBCC_CCXL_IsRegImmStringP(ctx, args[1]))
			return(0);

		dty=BGBCC_CCXL_GetRegType(ctx, args[0]);

		if(BGBCC_CCXL_IsRegLocalP(ctx, args[0]))
		{
			if(BGBCC_CCXL_TypeArrayP(ctx, dty))
			{
				sctx->sreg_live=sctx->sreg_held;
				sctx->sfreg_live=sctx->sfreg_held;

				s0=BGBCC_CCXL_GetRegImmStringValue(ctx, args[1]);
//				l0=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
				sz=strlen(s0);
				sz=(sz+4)&(~3);
				al=4;

//				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//				tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, args[0], tr0);
//				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, args[1], tr1);
				tr0=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[0]);
				tr1=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, args[1]);
				BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, tr0, tr1, sz, al);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[0]);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, args[1]);
//				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
//				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
				return(1);
			}
		}
	}
#endif

	return(0);
}

int BGBCC_JX2C_EmitCallFcnVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register fcn)
{
	ccxl_type tty;
	int treg, tr0, tr1;
	int i, j, k;

	sctx->is_leaf=0;

	tty=BGBCC_CCXL_GetRegType(ctx, fcn);
//	if(sctx->has_bjx1mov && !sctx->is_pic &&
	if(
		BGBCC_CCXL_IsRegGlobalP(ctx, fcn) &&
		BGBCC_CCXL_TypeFunctionP(ctx, tty) &&
		!BGBCC_CCXL_TypePointerP(ctx, tty))
	{
		j=BGBCC_CCXL_GetRegID(ctx, fcn);
		k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
//		BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx, BGBCC_SH_NMID_JSR,
//			BGBCC_SH_REG_R3, k);
		BGBCC_JX2_EmitOpAutoLabel(sctx,
			BGBCC_SH_NMID_BSR, k);
//		BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
		return(1);
	}

	treg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, fcn);
	
	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, treg))
	{
		/* If calling a register pair, assume thiscall. */
		tr0=BGBCC_SH_REG_R0+(treg&31);
		tr1=BGBCC_SH_REG_R0+((treg&31)+1);

		if((treg&31)>=16)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				tr0, BGBCC_SH_REG_R1);
			tr0=BGBCC_SH_REG_R1;
		}
	
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			tr1, BGBCC_SH_REG_R3);
		i=BGBCC_JX2C_EmitOpReg(ctx, sctx,
			BGBCC_SH_NMID_JSR, tr0);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, fcn);
		return(i);
	}
	
	tr0=treg;
	if((treg&31)>=16)
	{
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			tr0, BGBCC_SH_REG_R1);
		tr0=BGBCC_SH_REG_R1;
	}
	
	i=BGBCC_JX2C_EmitOpReg(ctx, sctx,
		BGBCC_SH_NMID_JSR, tr0);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, fcn);
	return(i);

#if 0
	BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
		fcn, BGBCC_SH_REG_R3);
	i=BGBCC_JX2C_EmitOpReg(ctx, sctx,
		BGBCC_SH_NMID_JSR, BGBCC_SH_REG_R3);
//	BGBCC_JX2C_EmitOpNone(ctx, sctx,
//		BGBCC_SH_NMID_NOP);

//	BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
	return(i);
#endif

}

int BGBCC_JX2C_EmitCallName(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name)
{
	int lv0;
	int i, j, k;

	sctx->is_leaf=0;

//	BGBCC_JX2C_SetModeDqClear(ctx, sctx);

#if 1
	j=BGBCC_CCXL_LookupGlobalIndex(ctx, name);
	if(j>0)
	{
		k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
		BGBCC_JX2_EmitOpAutoLabel(sctx,
			BGBCC_SH_NMID_BSR, k);
		return(1);
	}
#endif

#if 1
	j=BGBCC_JX2_LookupNamedLabel(sctx, name);
	if(j>0)
	{
		BGBCC_JX2_EmitOpAutoLabel(sctx,
			BGBCC_SH_NMID_BSR, j);
		return(1);
	}
#endif

#if 0
	j=BGBCC_JX2_GetNamedLabel(sctx, name);
	if(j>0)
	{
		BGBCC_JX2_EmitOpAutoLabel(sctx,
			BGBCC_SH_NMID_BSR, j);
		return(1);
	}
#endif

#if 1
	lv0=sctx->sreg_live;

	sctx->sreg_live&=~1;
	BGBCC_JX2C_EmitLoadGblNameReg(ctx, sctx,
		name, BGBCC_SH_REG_R3);
//	sctx->sreg_live|=1;
	i=BGBCC_JX2C_EmitOpReg(ctx, sctx,
		BGBCC_SH_NMID_JSR, BGBCC_SH_REG_R3);
//	BGBCC_JX2C_EmitOpNone(ctx, sctx,
//		BGBCC_SH_NMID_NOP);

	sctx->sreg_live|=lv0&1;

//	BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);

	return(i);
#endif
}

int BGBCC_JX2C_EmitCallVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dst,
	ccxl_register fcn,
	int narg, ccxl_register *args)
{
	ccxl_register treg;
	ccxl_type tty;
	char *fname;
	int tr0, tr1;
	int ni, nf, ns, rcls, retrcls;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	{
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		return(1);
//	}

	fname=NULL;
	if((fcn.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=(int)(fcn.val&CCXL_REGID_REGMASK);
		fname=ctx->reg_globals[i]->name;
	}
	
	if(fname && BGBCC_JX2C_EmitCallBuiltinArgs(
		ctx, sctx, type, dst, fname, narg, args))
			return(1);

	tty=BGBCC_CCXL_GetRegType(ctx, dst);
	retrcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(retrcls==BGBCC_SH_REGCLS_VO_REF)
	{
//		sctx->sreg_live|=0x0004;
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
//			dst, BGBCC_SH_REG_R2);

//		BGBCC_JX2C_EmitLdaFrameVRegReg(ctx, sctx,
//			dst, BGBCC_SH_REG_R2);
//		if(BGBCC_CCXL_IsRegTempP(ctx, dst))
//		{
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
//				dst, BGBCC_SH_REG_R2);
//		}

		if(sctx->is_addr64)
		{
			BGBCC_JX2C_EmitLdaValSyncFrameVRegReg(ctx, sctx,
				dst, BGBCC_SH_REG_RQ2);
		}else
		{
			BGBCC_JX2C_EmitLdaValSyncFrameVRegReg(ctx, sctx,
				dst, BGBCC_SH_REG_R2);
		}
	}

	if(sctx->sreg_live&0x00F000F0)
	{
		BGBCC_DBGBREAK
	}

//	if(sctx->sfreg_live&0x0FF0)
	if(sctx->sfreg_live&0x00F0)
	{
		BGBCC_DBGBREAK
	}

	sctx->sreg_live|=0x00F000F0;
//	sctx->sfreg_live|=0x0FF0;
	sctx->sfreg_live|=0x00F0;

	ni=0; nf=0; ns=0;
	for(j=0; j<narg; j++)
	{
		treg=args[j];
		tty=BGBCC_CCXL_GetRegType(ctx, treg);

		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
//			BGBCC_CCXL_TypePointerP(ctx, tty) ||
//			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if(	(rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_WGR))
		{
			if(sctx->is_addr64)
			{
				if(ni<4)
				{
					BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
					ni++;
					continue;
				}else if((ni<8) && sctx->has_bjx1egpr)
				{
					BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					continue;
				}else
				{
					tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
						BGBCC_SH_REGCLS_GR);
					BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
					BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
					ns++;
					continue;
				}
			}else if(ni<4)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
				ni++;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_R3;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns++;
				continue;
			}
		}

		if(
//			(rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF) ||
			(rcls==BGBCC_SH_REGCLS_QGR))
		{
			if(sctx->is_addr64)
			{
				if(ni<4)
				{
					BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
					ni++;
					continue;
				}else if((ni<8) && sctx->has_bjx1egpr)
				{
					BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					continue;
				}else
				{
					ns=(ns+1)&(~1);
					tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
						BGBCC_SH_REGCLS_QGR);
					BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
					BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
					ns+=2;
					continue;
				}
			}else if(ni<4)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
				ni++;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_R3;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns++;
				continue;
			}
		}

//		if(BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			if(sctx->is_addr64)
			{
				BGBCC_DBGBREAK
			}else
				if(ni<3)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR4+ni);
				ni+=2;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_LR1;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}

		if(rcls==BGBCC_SH_REGCLS_VO_GR)
		{
			if(sctx->is_addr64)
			{
				if(ni<4)
				{
					BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
					ni++;
					continue;
				}else if((ni<8) && sctx->has_bjx1egpr)
				{
					BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					continue;
				}else
				{
					tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
						BGBCC_SH_REGCLS_GR);
					BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
					BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
					ns++;
					continue;
				}
			}else
			{
				if(ni<4)
				{
					BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_R4+ni);
					ni++;
					continue;
				}else
				{
					tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
						BGBCC_SH_REGCLS_GR);
					BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
					BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
					ns++;
					continue;
				}
			}
		}

		if(rcls==BGBCC_SH_REGCLS_VO_QGR)
		{
			if(ni<4)
			{
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ4+ni);
				ni++;
				continue;
			}else if((ni<8) && sctx->has_bjx1egpr)
			{
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ20+(ni-4));
				ni++;
				continue;
			}else
			{
				ns=(ns+1)&(~1);
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
					BGBCC_SH_REGCLS_QGR);
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}

		if(rcls==BGBCC_SH_REGCLS_VO_GR2)
		{
			if(sctx->is_addr64)
			{
				BGBCC_DBGBREAK
			}else
				if(ni<3)
			{
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR4+ni);
				ni+=2;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_LR1;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}

		if(rcls==BGBCC_SH_REGCLS_VO_QGR2)
		{
			if(ni<3)
			{
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR4+ni);
				ni+=2;
				continue;
			}else
				if((ni==4) && sctx->has_bjx1egpr)
			{
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR16);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_R16, BGBCC_SH_REG_R7);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_R17, BGBCC_SH_REG_R20);
				ni+=2;
			}else
				if((ni<7) && sctx->has_bjx1egpr)
			{
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR20+(ni-4));
				ni+=2;
			}else
			{
				ns=(ns+1)&(~1);
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
				BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=4;
				continue;
			}
		}
		
//		if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		if(rcls==BGBCC_SH_REGCLS_FR)
		{
			if(nf<4)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_FR4+nf);
				nf++;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_FR0;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns++;
				continue;
			}
		}

		if(rcls==BGBCC_SH_REGCLS_DR)
		{
			if(nf<4)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_DR4+nf);
				nf++;
				continue;
			}else
			{
				ns=(ns+1)&(~1);
//				tr0=BGBCC_SH_REG_FR0;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_DR);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}

#if 0
//		if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		if(rcls==BGBCC_SH_REGCLS_FR2)
		{
			if(nf<7)
			{
				if(nf&1)nf++;
//				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
//					treg, BGBCC_SH_REG_DR2+(nf/2));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_DR4+nf);
				nf+=2;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_DR0;
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR2);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_JX2C_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}
#endif

		BGBCC_CCXL_StubError(ctx);
	}

	if(fname && BGBCC_JX2C_EmitCallBuiltin(ctx, sctx, fname))
	{
		sctx->sreg_live&=~0x00F000F0;
//		sctx->sfreg_live&=~0x0FF0;
		sctx->sfreg_live&=~0x00F0;
		return(1);
	}

//	BGBCC_JX2C_ResetFpscrDefaults(ctx, sctx);
//	BGBCC_JX2C_SetModeDqClear(ctx, sctx);

	BGBCC_JX2C_EmitCallFcnVReg(ctx, sctx, fcn);

	sctx->sreg_live&=~0x00F000F0;
//	sctx->sfreg_live&=~0x0FF0;
	sctx->sfreg_live&=~0x00F0;

	if(retrcls==BGBCC_SH_REGCLS_VO_REF)
	{
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
	}

	return(1);
}




int BGBCC_JX2C_EmitCSeltSelectVRegVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	byte flip, noflip;
	s32 imm;
	int nm1, nm2, cmp1, tr0;
	int i;
	
	if(1)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			BGBCC_SH_NMID_CSELT, csreg, ctreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_EmitCSeltCompareVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int cmp, ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	byte flip, noflip;
	s32 imm;
	int nm1, nm2, cmp1, tr0;
	int i;
	
	noflip=0;
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		noflip=1;
	
	flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BF;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BT;
			}
		}
		break;
	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		if(nm2==BGBCC_SH_NMID_BF)
			BGBCC_JX2C_EmitOpNone(ctx, sctx, BGBCC_SH_NMID_NOTT);
		

#if 0
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, cdreg))
		if(0)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, tr0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV,
				tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
//			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		}
		
		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, 1, cdreg);
		}
		
//		BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);
#endif

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCSeltVRegVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
		ccxl_register sreg, ccxl_register treg,
	ccxl_type ptype, int cmp,
		ccxl_register ureg, ccxl_register vreg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ptype) ||
		(BGBCC_CCXL_TypePointerP(ctx, ptype) && !sctx->is_addr64))
	{
		BGBCC_JX2C_EmitCSeltCompareVRegVRegInt(ctx, sctx,
			ptype, cmp, ureg, vreg);
		BGBCC_JX2C_EmitCSeltSelectVRegVRegVRegInt(ctx, sctx,
			type, dreg, sreg, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
