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

int BGBCC_JX2C_EmitBinaryVRegVRegBitInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, sz;
	int i, j, k;

	sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, type);
	sz=(sz+127)/128;

	nm1=-1;

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xbi_add"; break;
	case CCXL_BINOP_SUB:	s0="__xbi_sub"; break;
	case CCXL_BINOP_MUL:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_umul";
		else
			s0="__xbi_smul";
		break;
	case CCXL_BINOP_DIV:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_udiv";
		else
			s0="__xbi_sdiv";
		break;
	case CCXL_BINOP_MOD:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_umod";
		else
			s0="__xbi_smod";
		break;
	case CCXL_BINOP_AND:	s0="__xbi_and"; break;
	case CCXL_BINOP_OR:		s0="__xbi_or"; break;
	case CCXL_BINOP_XOR:	s0="__xbi_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xbi_shl"; break;
	case CCXL_BINOP_SHR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_shlr";
		else
			s0="__xbi_shar";
		break;
	case CCXL_BINOP_SHRR:
		s0="__xbi_shlr";
		break;
	}

#if 1
	if(s0)
	{
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R7, sz);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R6);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegBitInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, sz;
	int i, j, k;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegBitInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		k=0;
		switch(opr)
		{
		case CCXL_BINOP_ADD:
		case CCXL_BINOP_MUL:	case CCXL_BINOP_AND:
		case CCXL_BINOP_OR:		case CCXL_BINOP_XOR:
			k=1; break;
		default:
			break;
		}

		if(k)
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegBitInt(ctx, sctx,
				type, dreg, opr, sreg);
			return(i);
		}
	}

	sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, type);
	sz=(sz+127)/128;

	nm1=-1;

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xbi_add"; break;
	case CCXL_BINOP_SUB:	s0="__xbi_sub"; break;
	case CCXL_BINOP_MUL:	s0="__xbi_smul"; break;
	case CCXL_BINOP_DIV:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_udiv";
		else
			s0="__xbi_sdiv";
		break;
	case CCXL_BINOP_MOD:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_umod";
		else
			s0="__xbi_smod";
		break;
	case CCXL_BINOP_AND:	s0="__xbi_and"; break;
	case CCXL_BINOP_OR:		s0="__xbi_or"; break;
	case CCXL_BINOP_XOR:	s0="__xbi_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xbi_shl"; break;
	case CCXL_BINOP_SHR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_shlr";
		else
			s0="__xbi_shar";
		break;
	case CCXL_BINOP_SHRR:
		s0="__xbi_shlr";
		break;
	}

#if 1
	if(s0)
	{
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R7, sz);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R6);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitUnaryVRegVRegBitInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	ccxl_register ttreg;
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4, sz;
	s32 imm;
	int i, j, k;
	
	sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, type);
	sz=(sz+127)/128;

	if(opr==CCXL_UNOP_MOV)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
		BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, cdreg, csreg, sz*16, 8);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);

//		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
//		return(1);
	}

#if 0
	if(opr==CCXL_UNOP_LNOT)
	{
//		ttreg.val=CCXL_REGTY_IMM_INT;
		BGBCC_CCXL_GetRegForBitIntValue(ctx, &ttreg, 0, 0);
		i=BGBCC_JX2C_EmitCompareVRegVRegVRegBitInt(ctx, sctx,
			type, dreg, CCXL_CMP_EQ, sreg, ttreg);
		return(0);
	}
#endif

	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__xbi_neg"; break;
	case CCXL_UNOP_NOT:	s0="__xbi_not"; break;
	}

#if 1
	if(s0)
	{
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R6, sz);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegBitInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, invert, sz;
	int i, j, k;

	sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, type);
	sz=(sz+127)/128;

	nm1=-1;
	nm2=-1;
	nm3=-1;
	noflip=0;
	invert=0;

	flip=0;
	s0=NULL;
	switch(opr)
	{
	case CCXL_CMP_EQ:
		s0="__xbi_cmp_eq";	break;
	case CCXL_CMP_NE:
		s0="__xbi_cmp_ne";	break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xbi_cmp_he"; flip=1; }
		else
			{ s0="__xbi_cmp_ge"; flip=1; }
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_cmp_hi";
		else
			s0="__xbi_cmp_gt";
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xbi_cmp_hi"; flip=1; }
		else
			{ s0="__xbi_cmp_gt"; flip=1; }
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_cmp_he";
		else
			s0="__xbi_cmp_ge";
		break;
	case CCXL_CMP_TST:	
		s0="__xbi_cmp_tst";		break;
	case CCXL_CMP_NTST:	
		s0="__xbi_cmp_ntst";	break;
	}

	if(s0)
	{
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R6, sz);

		if(flip)
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R4);
		}else
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		}
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
//		if(invert)
//		{
//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//				BGBCC_SH_NMID_NOT, BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
//		}
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R2);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVRegBitInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, sz;
	int i, j, k;
	

	sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, type);
	sz=(sz+127)/128;

	nm1=-1;
	nm2=-1;
	nm3=-1;
	noflip=0;

	s0=NULL; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		s0="__xbi_cmp_eq";	break;
	case CCXL_CMP_NE:
		s0="__xbi_cmp_ne";	break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xbi_cmp_he"; flip=1; }
		else
			{ s0="__xbi_cmp_ge"; flip=1; }
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_cmp_hi";
		else
			s0="__xbi_cmp_gt";
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xbi_cmp_hi"; flip=1; }
		else
			{ s0="__xbi_cmp_gt"; flip=1; }
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xbi_cmp_he";
		else
			s0="__xbi_cmp_ge";
		break;
	case CCXL_CMP_TST:	
		s0="__xbi_cmp_tst";		break;
	case CCXL_CMP_NTST:	
		s0="__xbi_cmp_ntst";	break;
	}

	if(s0)
	{
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R6, sz);

		if(flip)
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R4);
		}else
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		}

		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST,
			BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);

		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitConvFromVRegVRegBitInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type dtype, ccxl_type stype, ccxl_register dreg, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, sz, dsz, ssz, msz, nsz;
	int i, j, k;

	dsz=BGBCC_CCXL_TypeGetBitIntSize(ctx, dtype);
	ssz=BGBCC_CCXL_TypeGetBitIntSize(ctx, stype);

	dsz=(dsz+127)/128;
	ssz=(ssz+127)/128;
	
	msz=dsz;
	nsz=dsz;
	if(ssz<msz)
		msz=ssz;
	if(ssz>nsz)
		nsz=ssz;

#if 0
	if(dtype.val==stype.val)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
		BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, cdreg, csreg, dsz*16, 8);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);

//		BGBCC_JX2C_EmitUnaryVRegVRegBitInt(ctx, sctx,
//			type, dreg, CCXL_UNOP_MOV, sreg);
//		return(1);
	}
#endif

	if(BGBCC_CCXL_TypeBitIntP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			if(dsz<=ssz)
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
				BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
					cdreg, csreg, dsz*16, 8);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			}else
			{
				if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
					{ s0="__xbi_expand_ubi"; }
				else
					{ s0="__xbi_expand_sbi"; }
			
				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R7, dsz);
				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R6, ssz);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R5);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
				BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			}

			return(1);
		}

		if(BGBCC_CCXL_TypeSgInt128P(ctx, stype))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
				{ s0="__xbi_convfrom_uxi"; }
			else
				{ s0="__xbi_convfrom_sxi"; }
		
			BGBCC_JX2_EmitLoadRegImm(sctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R7, dsz);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R6);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

			return(1);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
				{ s0="__xbi_convfrom_uli"; }
			else
				{ s0="__xbi_convfrom_sli"; }
		
			BGBCC_JX2_EmitLoadRegImm(sctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R6, dsz);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, dtype))
	{
		s0="__xbi_convtoxi";
		BGBCC_JX2_EmitLoadRegImm(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R5, ssz);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
		return(1);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, dtype))
	{
		s0="__xbi_convtoli";
		BGBCC_JX2_EmitLoadRegImm(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R5, ssz);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
		return(1);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, dtype))
	{
		s0="__xbi_convtoli";
		BGBCC_JX2_EmitLoadRegImm(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R5, ssz);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD2);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
