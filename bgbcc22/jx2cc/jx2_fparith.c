/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/


int BGBCC_JX2C_EmitBinaryVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	double f;
	char *s0;
	s64 li;
	u32 imm_f32;
	u16 imm_f16, imm_f16b;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, rt1, rt2;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitBinaryVRegVRegSoftFloat(
			ctx, sctx, type, dreg, opr, treg));
	}

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

#if 1
//	if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
	if(
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
		if(opr==CCXL_BINOP_DIV)
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
//			if((f>0.01) && (f<999999.0))
			if((f>0.1) && (f<9999.0))
			{
				BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
				opr=CCXL_BINOP_MUL;
			}
		}
	}
#endif

	if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
		if(f==0)
		{
			if(opr==CCXL_BINOP_ADD)
				return(1);
			if(opr==CCXL_BINOP_SUB)
				return(1);

			if(opr==CCXL_BINOP_MUL)
			{
				BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, 0);
				return(1);
			}
		}
	}
		
	nm1=-1; nm2=-1; nm3=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:
		nm1=BGBCC_SH_NMID_FADD;	nm2=-1;
		nm3=BGBCC_SH_NMID_FADD;
		break;
	case CCXL_BINOP_SUB:
		nm1=BGBCC_SH_NMID_FSUB;	nm2=-1; break;
	case CCXL_BINOP_MUL:
		nm1=BGBCC_SH_NMID_FMUL;	nm2=-1;
		nm3=BGBCC_SH_NMID_FMUL;
		break;
	case CCXL_BINOP_DIV:
		if(sctx->has_qmul&2)
			nm1=BGBCC_SH_NMID_FDIV;
		break;

	default:		nm1=-1; nm2=-1; break;
	}
	
	if(ctx->cur_func->flagsint&BGBCC_TYFL_FENVACC)
	{
		if(nm1==BGBCC_SH_NMID_FADD)
			nm1=BGBCC_SH_NMID_FADDG;
		if(nm1==BGBCC_SH_NMID_FSUB)
			nm1=BGBCC_SH_NMID_FSUBG;
		if(nm1==BGBCC_SH_NMID_FMUL)
			nm1=BGBCC_SH_NMID_FMULG;
	}

	if(	(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		 BGBCC_CCXL_TypeFloat16P(ctx, type)) &&
		 (sctx->has_fpvsf&2))
	{
		if(nm1==BGBCC_SH_NMID_FADD)
			nm1=BGBCC_SH_NMID_FADDA;
		if(nm1==BGBCC_SH_NMID_FSUB)
			nm1=BGBCC_SH_NMID_FSUBA;
		if(nm1==BGBCC_SH_NMID_FMUL)
			nm1=BGBCC_SH_NMID_FMULA;
	}


	if(sctx->fpu_lite)
	{
		if(opr==CCXL_BINOP_DIV)
			nm1=-1;
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			nm1=-1;
	}

#if 0
	if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
		
		if(sctx->has_fpim&1)
		{
			memcpy(&li, &f, sizeof(double));
		
			rt1=BGBCC_JX2_ConstConvDoubleToFloat(li, &imm_f32);
			rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
			imm_f16b=imm_f16&0xFFC0;

			if(!sctx->is_simpass)
				{ sctx->stat_fpimm_totchk10++; }

			if((rt1>0) && (rt2>0) && (imm_f16b==imm_f16) &&
				(nm3>=0))
			{
				if(!sctx->is_simpass)
					{ sctx->stat_fpimm_hitchk10++; }

				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2_EmitOpImmReg(sctx, nm3, imm_f16, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}
	}
#endif

	if(nm1>=0)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		if(sctx->fpu_gfp)
		{
//			if((cdreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
			if((cdreg&BGBCC_SH_REG_RTMASK6)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
//			if((ctreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
			if((ctreg&BGBCC_SH_REG_RTMASK6)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
		}else
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
//	case CCXL_BINOP_DIV:	s0="__fpu_fdiv";	break;
	case CCXL_BINOP_DIV:
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			s0="__fpu_fdiv";
		else
			s0="__fpu_fdiv_s";
		break;
	default:				s0=NULL; break;
	}

	if(s0)
	{
		if(sctx->fpu_gfp)
		{
#if 1
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg,
				BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, 
				BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_RQRET);
#endif
			return(1);
		}else
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
	double f;
	char *s0;
	u64 li;
	u32 imm_f32;
	u16 imm_f16, imm_f16b;
	int csreg, ctreg, cdreg;
	int rt1, rt2;
	int nm1, nm2, nm3;
	int i, j;

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

//	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg) &&
		!BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, treg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL))
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegFloat(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
		
//		BGBCC_CCXL_StubError(ctx);
//		return(0);
	}

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

#if 1
	if(1)
	{
#if 1
//		if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		if(	BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
		{
			if(opr==CCXL_BINOP_DIV)
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
//				if((f>0.01) && (f<999999.0))
				if((f>0.1) && (f<9999.0))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}
		}
#endif

		nm1=-1; nm2=-1;	nm3=-1;
		switch(opr)
		{
		case CCXL_BINOP_ADD:
			nm1=BGBCC_SH_NMID_FADD;	nm2=-1;
			nm3=BGBCC_SH_NMID_FADD;		break;
		case CCXL_BINOP_SUB:
			nm1=BGBCC_SH_NMID_FSUB;	nm2=-1;
			nm3=BGBCC_SH_NMID_FSUB;		break;
		case CCXL_BINOP_MUL:
			nm1=BGBCC_SH_NMID_FMUL;	nm2=-1;
			nm3=BGBCC_SH_NMID_FMUL;		break;

		case CCXL_BINOP_DIV:
			if(sctx->has_qmul&2)
				nm1=BGBCC_SH_NMID_FDIV;
			break;

		default:		nm1=-1; nm2=-1; break;
		}
	
		if(ctx->cur_func->flagsint&BGBCC_TYFL_FENVACC)
		{
			if(nm1==BGBCC_SH_NMID_FADD)
				nm1=BGBCC_SH_NMID_FADDG;
			if(nm1==BGBCC_SH_NMID_FSUB)
				nm1=BGBCC_SH_NMID_FSUBG;
			if(nm1==BGBCC_SH_NMID_FMUL)
				nm1=BGBCC_SH_NMID_FMULG;
		}

		if(	(BGBCC_CCXL_TypeFloatP(ctx, type) ||
			 BGBCC_CCXL_TypeFloat16P(ctx, type)) &&
			 (sctx->has_fpvsf&2))
		{
			if(nm1==BGBCC_SH_NMID_FADD)
				nm1=BGBCC_SH_NMID_FADDA;
			if(nm1==BGBCC_SH_NMID_FSUB)
				nm1=BGBCC_SH_NMID_FSUBA;
			if(nm1==BGBCC_SH_NMID_FMUL)
				nm1=BGBCC_SH_NMID_FMULA;
		}

		if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
			
			if((f==0.0) && ((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB)))
			{
				BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
				return(1);
			}

			if((f==1.0) && ((opr==CCXL_BINOP_MUL) || (opr==CCXL_BINOP_DIV)))
			{
				BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
				return(1);
			}

			if((f==0.0) && ((opr==CCXL_BINOP_MUL) || (opr==CCXL_BINOP_DIV)))
			{
				BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, 0);
				return(1);
			}

			if(sctx->has_fpim&1)
			{
				memcpy(&li, &f, sizeof(double));
			
				rt1=BGBCC_JX2_ConstConvDoubleToFloat(li, &imm_f32);
				rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);

#if 0
//				j=((imm_f16-0x3800)>>8)&0x1F;
//				imm_f16b=0x3800+(j<<8);

//				j=((imm_f16-0x3000)>>8)&0x1F;
//				imm_f16b=0x3000+(j<<8);

//				j=((imm_f16-0x3400)>>8)&0x1F;
				j=((imm_f16-0x3000)>>8)&0x1F;
				if(!imm_f16)
					j=0;
//				imm_f16b=0x3400+(j<<8);
				imm_f16b=0x3000+(j<<8);
				if(!j)
					imm_f16b=0;
#endif

				if(!sctx->is_simpass)
					{ sctx->stat_fpimm_totchk5++; }

				j=BGBCC_JX2_ConstConvHalfToFP5A(imm_f16);

//				if((rt1>0) && (rt2>0) && (imm_f16b==imm_f16) &&
				if((rt1>0) && (rt2>0) && (j>=0) &&
					(nm3>=0))
				{
					if(!sctx->is_simpass)
						{ sctx->stat_fpimm_hitchk5++; }

					csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
					cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
					BGBCC_JX2_EmitOpRegImmReg(sctx, nm3, csreg, j, cdreg);
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
					return(1);
				}

#if 1
				if(nm3>=0)
//				if(nm1>=0)
				{
					csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
					cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
//					BGBCC_JX2_EmitOpRegImmReg(sctx, nm3, csreg, li, cdreg);
					BGBCC_JX2_EmitOpRegImmReg(sctx, nm1, csreg, li, cdreg);
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
					return(1);
				}
#endif
			}
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

			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				ctreg=cdreg;
			}else
			{
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			}

#if 0
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
#endif

			BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			if(ctreg!=cdreg)
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

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);

#if 1
	if(opr==CCXL_UNOP_LNOT)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			if(sctx->emit_riscv&0x11)
			{
				ctreg=cdreg&63;
				BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
					BGBCC_SH_NMID_FCMPEQ, cdreg, BGBCC_SH_REG_ZZR, ctreg);
//				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//					BGBCC_SH_NMID_XOR, ctreg, 1, ctreg);
			}else
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_TSTQ, cdreg, cdreg);
	//			ctreg=cdreg&31;
				ctreg=cdreg&63;
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
			}
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCI,
				ctreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		ctreg=cdreg&31;
		ctreg=cdreg&63;

		if(sctx->emit_riscv&0x11)
		{
			BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
				BGBCC_SH_NMID_FCMPEQ, csreg, BGBCC_SH_REG_ZZR, ctreg);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_XOR, ctreg, 1, ctreg);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_TSTQ, csreg, csreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx,
				BGBCC_SH_NMID_MOVT, ctreg);
		}
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCI,
			ctreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if(opr==CCXL_UNOP_NEG)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		}
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FNEG, csreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		if(csreg!=cdreg)
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(opr==CCXL_UNOP_MOV)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			return(1);

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
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
#endif

#if 0
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
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_TSTQ, cdreg, cdreg);
//				ctreg=cdreg&31;
				ctreg=cdreg&63;
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			nm1=BGBCC_SH_NMID_FCMPEQ;
			nm2=BGBCC_SH_NMID_BT;

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//			ctreg=cdreg&31;
			ctreg=cdreg&63;

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_TSTQ, csreg, csreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx,
				BGBCC_SH_NMID_MOVT, ctreg);

#if 0
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
#endif

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif

		if((opr==CCXL_UNOP_NEG) && BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				csreg=cdreg;
			}else
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			}
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_FNEG, cdreg);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FNEG, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			if(csreg!=cdreg)
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
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
	}
#endif

#if 0
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

		if(sctx->fpu_gfp)
		{
			if((csreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
		}else

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

		if(sctx->fpu_gfp)
		{
			czreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, czreg, 0);
		}else
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

//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
//			ctreg, BGBCC_SH_REG_MACL);
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_FLDCI, cdreg);

		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCI,
			ctreg, cdreg);

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

//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
//			BGBCC_JX2_EmitOpReg(sctx, nm1, cdreg);
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		}
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitCompareVRegVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	ccxl_register treg2;
	double f;

	int csreg, ctreg, cdreg;
	int nm1, nm2, swst, lfp, cmp1;
	int i, j;
	
	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitCompareVRegVRegVRegSoftFloat(
			ctx, sctx, type, dreg, cmp, sreg, treg));
	}

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

#if 1
	if(	 (	BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg) ) &&
		!(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, treg)) )
	{
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

		i=BGBCC_JX2C_EmitCompareVRegVRegVRegFloat(ctx, sctx,
			type, dreg, cmp1, treg, sreg);
		return(i);
	}
#endif

	if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
		
#if 0
		if(sctx->fpu_gfp && (f==0))
		{
			BGBCC_CCXL_GetRegForLongValue(ctx, &treg2, 0);
			i=BGBCC_JX2C_EmitCompareVRegVRegVRegQLong(ctx, sctx,
				type, dreg, cmp, sreg, treg2);
			return(i);
		}
#endif
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

		if(sctx->fpu_gfp)
		{
//			if((csreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
			if((csreg&BGBCC_SH_REG_RTMASK6)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
//			if((ctreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
			if((ctreg&BGBCC_SH_REG_RTMASK6)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
		}else
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

		if(sctx->emit_riscv&0x11)
		{
			if(!swst)
				BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);
			else
				BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, ctreg, csreg, cdreg);

			if(nm2==BGBCC_SH_NMID_BF)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_XOR, cdreg, 1, cdreg);
			}
		}else
		{
			if(swst)
				BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, ctreg);
			else
				BGBCC_JX2_EmitOpRegReg(sctx, nm1, ctreg, csreg);
	//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);

			if(nm2==BGBCC_SH_NMID_BF)
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
			else
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
			
//			if(nm2==BGBCC_SH_NMID_BF)
//			{
//				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
//				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, 1, cdreg);
//			}
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
	double f;
	u64 li;
	u32 imm_f32;
	u16 imm_f16, imm_f16b;
	int csreg, ctreg;
	int nm1, nm2, nm3, nm4, sw, sw1, lfp;
	int rt0, rt1, rt2;
	int i;
	
	if(sctx->no_fpu || sctx->fpu_soft)
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegSoftFloat(
			ctx, sctx, type, sreg, treg, cmp, lbl));
	}

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);
	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &treg);

	if(	 (	BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg)) &&
		!(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, treg)))
	{
		rt2=-1;
		switch(cmp)
		{
			case CCXL_CMP_EQ:	rt2=CCXL_CMP_EQ;	break;
			case CCXL_CMP_NE:	rt2=CCXL_CMP_NE;	break;
			case CCXL_CMP_LT:	rt2=CCXL_CMP_GT;	break;
			case CCXL_CMP_GT:	rt2=CCXL_CMP_LT;	break;
			case CCXL_CMP_LE:	rt2=CCXL_CMP_GE;	break;
			case CCXL_CMP_GE:	rt2=CCXL_CMP_LE;	break;
			default:			rt2=-1;				break;
		}
		
		if(rt2>=0)
		{
			return(BGBCC_JX2C_EmitJCmpVRegVRegFloat(ctx, sctx,
				type, treg, sreg, rt2, lbl));
		}
	}

	lfp=0; sw=0;
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
		sw=0;
		break;
	}

	if(	BGBCC_CCXL_IsRegImmFloatP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
		
#if 0
		if(sctx->fpu_gfp && (f==0))
		{
			i=BGBCC_JX2C_EmitJCmpVRegZeroQLong(ctx, sctx,
				type, sreg, cmp, lbl);
			return(i);
		}
#endif
		
		if(sctx->has_fpim&1)
		{
			memcpy(&li, &f, sizeof(double));
		
			rt1=BGBCC_JX2_ConstConvDoubleToFloat(li, &imm_f32);
			rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
			imm_f16b=imm_f16&0xFFC0;

			if(!sctx->is_simpass)
				{ sctx->stat_fpimm_totchk10++; }

			nm3=nm1;
			nm4=nm2;
			sw1=sw;
			
			if(sw1)
			{
				if((nm1==BGBCC_SH_NMID_FCMPGT) && (nm2==BGBCC_SH_NMID_BT))
					{ nm3=BGBCC_SH_NMID_FCMPGE; nm4=BGBCC_SH_NMID_BF; sw1=0; }
				if((nm1==BGBCC_SH_NMID_FCMPGT) && (nm2==BGBCC_SH_NMID_BF))
					{ nm3=BGBCC_SH_NMID_FCMPGE; nm4=BGBCC_SH_NMID_BT; sw1=0; }
			}

			if((rt1>0) && (rt2>0) && (imm_f16b==imm_f16) &&
				(nm3>=0) && !sw1)
			{
				if(!sctx->is_simpass)
					{ sctx->stat_fpimm_hitchk10++; }

				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				BGBCC_JX2_EmitOpImmReg(sctx, nm3, imm_f16, csreg);
				BGBCC_JX2_EmitOpAutoLabel(sctx, nm4, lbl);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}
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

		if(sctx->fpu_gfp)
		{
//			if((csreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
			if((csreg&BGBCC_SH_REG_RTMASK6)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
//			if((ctreg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_RQ0)
			if((ctreg&BGBCC_SH_REG_RTMASK6)!=BGBCC_SH_REG_RQ0)
				{ BGBCC_DBGBREAK }
		}else
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

		if(sctx->emit_riscv&0x11)
		{
			if(sw)
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					nm1, ctreg, csreg, BGBCC_JX2CC_PSREG_TS0);
			else
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					nm1, csreg, ctreg, BGBCC_JX2CC_PSREG_TS0);

			if(nm2==BGBCC_SH_NMID_BT)
				BGBCC_JX2_EmitOpRegRegLbl(sctx,
					BGBCC_SH_NMID_BRNE,
					BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_ZZR,
					lbl);
			if(nm2==BGBCC_SH_NMID_BF)
				BGBCC_JX2_EmitOpRegRegLbl(sctx,
					BGBCC_SH_NMID_BREQ,
					BGBCC_JX2CC_PSREG_TS0, BGBCC_SH_REG_ZZR,
					lbl);
		}else
		{
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
		}
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
