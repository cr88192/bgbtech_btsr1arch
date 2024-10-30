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
	
	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		switch(opr)
		{
			case CCXL_BINOP_ADD:	s0="__lvax_add"; break;
			case CCXL_BINOP_SUB:	s0="__lvax_sub"; break;
			case CCXL_BINOP_MUL:	s0="__lvax_mul"; break;
			case CCXL_BINOP_DIV:	s0="__lvax_div"; break;
			case CCXL_BINOP_MOD:	s0="__lvax_mod"; break;
			case CCXL_BINOP_AND:	s0="__lvax_and"; break;
			case CCXL_BINOP_OR:		s0="__lvax_or"; break;
			case CCXL_BINOP_XOR:	s0="__lvax_xor"; break;
			case CCXL_BINOP_SHL:	s0="__lvax_shl"; break;
			case CCXL_BINOP_SHR:	s0="__lvax_shr"; break;
			case CCXL_BINOP_SHRR:	s0="__lvax_shrr"; break;

			case CCXL_BINOP_CONS:	s0="__lvax_rcons"; break;
		}
	}else
	{
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
			case CCXL_BINOP_SHRR:	s0="__lva_shrr"; break;

			case CCXL_BINOP_CONS:	s0="__lva_rcons"; break;
		}
	}

	if(s0)
	{
		if(	(rcls==BGBCC_SH_REGCLS_GR2) ||
			(rcls==BGBCC_SH_REGCLS_QGR2))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_RQRET);
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
	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		switch(opr)
		{
		case CCXL_BINOP_ADD:	s0="__lvax_add"; break;
		case CCXL_BINOP_SUB:	s0="__lvax_sub"; break;
		case CCXL_BINOP_MUL:	s0="__lvax_mul"; break;
		case CCXL_BINOP_DIV:	s0="__lvax_div"; break;
		case CCXL_BINOP_MOD:	s0="__lvax_mod"; break;
		case CCXL_BINOP_AND:	s0="__lvax_and"; break;
		case CCXL_BINOP_OR:		s0="__lvax_or"; break;
		case CCXL_BINOP_XOR:	s0="__lvax_xor"; break;
		case CCXL_BINOP_SHL:	s0="__lvax_shl"; break;
		case CCXL_BINOP_SHR:	s0="__lvax_shr"; break;
		case CCXL_BINOP_SHRR:	s0="__lvax_shrr"; break;
		case CCXL_BINOP_CONS:	s0="__lvax_rcons"; break;
		}
	}else
	{
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
		case CCXL_BINOP_SHRR:	s0="__lva_shrr"; break;
		case CCXL_BINOP_CONS:	s0="__lva_rcons"; break;
		}
	}

	if(s0)
	{
		if(	(rcls==BGBCC_SH_REGCLS_GR2) ||
			(rcls==BGBCC_SH_REGCLS_QGR2))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg,
				BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg,
				BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRARG(2));

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, 
				BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg,
				BGBCC_JX2CC_PSREG_ARG(1));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg,
				BGBCC_JX2CC_PSREG_ARG(0));
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
	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		switch(opr)
		{
			case CCXL_UNOP_NEG: s0="__lvax_neg"; break;
			case CCXL_UNOP_NOT: s0="__lvax_not"; break;
			case CCXL_UNOP_INC: s0="__lvax_inc"; break;
			case CCXL_UNOP_DEC: s0="__lvax_dec"; break;
		}
	}else
	{
		switch(opr)
		{
			case CCXL_UNOP_NEG: s0="__lva_neg"; break;
			case CCXL_UNOP_NOT: s0="__lva_not"; break;
			case CCXL_UNOP_INC: s0="__lva_inc"; break;
			case CCXL_UNOP_DEC: s0="__lva_dec"; break;
		}
	}

	if(s0)
	{
		if(	(rcls==BGBCC_SH_REGCLS_GR2) ||
			(rcls==BGBCC_SH_REGCLS_QGR2))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg,
				BGBCC_JX2CC_PSREG_LRARG(0));

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg,
				BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg,
				BGBCC_JX2CC_PSREG_RQRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx,
				BGBCC_JX2CC_PSREG_RQRET);
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
	ccxl_type tty;
	int fr0, dr0;
	int xri, xro;
	int tri, tro;
	int pri, pro;
	int rcls;

	tty=BGBCC_CCXL_GetRegType(ctx, sreg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	if(sctx->is_addr64)
	{
		xri=BGBCC_JX2CC_PSREG_LRARG(0);
		xro=BGBCC_JX2CC_PSREG_LRRET;

		tri=BGBCC_JX2CC_PSREG_ARG(0);
		tro=BGBCC_JX2CC_PSREG_RQRET;

		if(ctx->arch_sizeof_ptr==16)
		{
			pri=BGBCC_JX2CC_PSREG_LRARG(0);
			pro=BGBCC_JX2CC_PSREG_LRRET;
		}else
		{
			pri=BGBCC_JX2CC_PSREG_ARG(0);
			pro=BGBCC_JX2CC_PSREG_RQRET;
		}

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr0=BGBCC_SH_REG_RD2;
			dr0=BGBCC_JX2CC_PSREG_RQRET;
		}else
		{
			fr0=BGBCC_SH_REG_FR2;
			dr0=BGBCC_SH_REG_DR2;
		}
	}else
	{
		xri=-1;
		xro=-1;
	
		tri=BGBCC_JX2CC_PSREG_LRARG(0);
		tro=BGBCC_JX2CC_PSREG_LRRET;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr0=BGBCC_SH_REG_R2;
			dr0=BGBCC_JX2CC_PSREG_LRRET;
		}else
		{
			fr0=BGBCC_SH_REG_FR2;
			dr0=BGBCC_SH_REG_DR2;
		}
	}

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_toi32");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RD2);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RD2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RD2);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_toi64");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallInt128P(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_toi128");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
			BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_tof32");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, fr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, fr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, fr0);
			return(1);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_tof64");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, dr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, dr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, dr0);
			return(1);
		}

		if(BGBCC_CCXL_TypePointerP(ctx, type))
		{
			if(BGBCC_CCXL_TypeCStringP(ctx, type))
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_tostr");
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pro);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, pro);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pro);
				return(1);
			}
		
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_toptr");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, pro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pro);
			return(1);
		}
	}
	else
	{
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

	//	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
		if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
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

		if(BGBCC_CCXL_TypeSmallInt128P(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_toi128");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
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
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, opn);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRRET);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}
	#endif
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvFromVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	ccxl_type tty;
	int fr4, dr4;
	int xri, xro;
	int tri, tro;
	int pri, pro;
	int rcls, islvax;

	tty=BGBCC_CCXL_GetRegType(ctx, dreg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);
	islvax=0;

	if(sctx->is_addr64)
	{
		xri=BGBCC_JX2CC_PSREG_LRARG(0);
		xro=BGBCC_JX2CC_PSREG_LRRET;

		tri=BGBCC_JX2CC_PSREG_ARG(0);
		tro=BGBCC_JX2CC_PSREG_RQRET;

		if(ctx->arch_sizeof_ptr==16)
		{
			pri=BGBCC_JX2CC_PSREG_LRARG(0);
			pro=BGBCC_JX2CC_PSREG_LRRET;
			islvax=1;
		}else
		{
			pri=BGBCC_JX2CC_PSREG_ARG(0);
			pro=BGBCC_JX2CC_PSREG_RQRET;
		}
		
		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_RD4;
			dr4=BGBCC_JX2CC_PSREG_ARG(0);
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}else
	{
		tri=BGBCC_JX2CC_PSREG_LRARG(0);
		tro=BGBCC_JX2CC_PSREG_LRRET;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;
		
		xri=-1; xro=-1;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_R4;
			dr4=BGBCC_JX2CC_PSREG_LRARG(0);
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}

//	if(rcls==BGBCC_SH_REGCLS_QGR2)
	if(islvax)
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromi32");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}

		//	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
		if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromi64");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallInt128P(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromi128");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, fr4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, fr4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromf32");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, fr4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, dr4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, dr4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromf64");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, dr4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}


		if(BGBCC_CCXL_TypePointerP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, pri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromptr");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}
	}
	else
	{
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

		//	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
		if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
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

		if(BGBCC_CCXL_TypeSmallInt128P(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromi128");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
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
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvFromVRegVRegVarString(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	ccxl_type tty;
	int fr4, dr4;
	int tri, tro;
	int pri, pro;
	int xri, xro;
	int rcls;

	tty=BGBCC_CCXL_GetRegType(ctx, dreg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(sctx->is_addr64)
	{
		tri=BGBCC_JX2CC_PSREG_ARG(0);
		tro=BGBCC_JX2CC_PSREG_RQRET;

		xri=BGBCC_JX2CC_PSREG_LRARG(0);
		xro=BGBCC_JX2CC_PSREG_LRRET;

		if(ctx->arch_sizeof_ptr==16)
		{
			pri=BGBCC_JX2CC_PSREG_LRARG(0);
			pro=BGBCC_JX2CC_PSREG_LRRET;
		}else
		{
			pri=BGBCC_JX2CC_PSREG_ARG(0);
			pro=BGBCC_JX2CC_PSREG_RQRET;
		}
		
		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_RD4;
			dr4=BGBCC_JX2CC_PSREG_ARG(0);
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}else
	{
		tri=BGBCC_JX2CC_PSREG_LRARG(0);
		tro=BGBCC_JX2CC_PSREG_LRRET;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_R4;
			dr4=BGBCC_JX2CC_PSREG_LRARG(0);
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
	//	if(BGBCC_CCXL_TypePointerP(ctx, type))
		if(BGBCC_CCXL_TypeCStringP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, pri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromstr");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}

		if(BGBCC_CCXL_TypeCWStringP(ctx, type))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, pri);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, pri);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromwstr");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, pri);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
			return(1);
		}
	}else
	{
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
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvFromVRegVRegVariant2(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type dtype, ccxl_type stype, ccxl_register dreg, ccxl_register sreg)
{
	int fr4, dr4;
	int xri, xro;
	int tri, tro;
	int pri, pro;
	ccxl_type tty;
	int rcls;

	tty=BGBCC_CCXL_GetRegType(ctx, dreg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(dtype.val==CCXL_TY_VARIANT)
	{
		return(BGBCC_JX2C_EmitConvFromVRegVRegVariant(ctx, sctx,
			stype, dreg, sreg));
	}

	if(sctx->is_addr64)
	{
		xri=BGBCC_JX2CC_PSREG_LRARG(0);
		xro=BGBCC_JX2CC_PSREG_LRRET;

		tri=BGBCC_JX2CC_PSREG_ARG(0);
		tro=BGBCC_JX2CC_PSREG_RQRET;

		if(ctx->arch_sizeof_ptr==16)
		{
			pri=BGBCC_JX2CC_PSREG_LRARG(0);
			pro=BGBCC_JX2CC_PSREG_LRRET;
		}else
		{
			pri=BGBCC_JX2CC_PSREG_ARG(0);
			pro=BGBCC_JX2CC_PSREG_RQRET;
		}
		
		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_RD4;
			dr4=BGBCC_JX2CC_PSREG_ARG(0);
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}else
	{
		tri=-1;
		tro=-1;

		tri=BGBCC_JX2CC_PSREG_LRARG(0);
		tro=BGBCC_JX2CC_PSREG_LRRET;

		pri=BGBCC_SH_REG_R4;
		pro=BGBCC_SH_REG_R2;

		if(sctx->no_fpu || sctx->fpu_gfp)
		{
			fr4=BGBCC_SH_REG_R4;
			dr4=BGBCC_JX2CC_PSREG_LRARG(0);
		}else
		{
			fr4=BGBCC_SH_REG_FR4;
			dr4=BGBCC_SH_REG_DR4;
		}
	}

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		if(dtype.val==CCXL_TY_BIGINT)
		{
			if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromi64big");
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
				return(1);
			}

			if(BGBCC_CCXL_TypeSmallInt128P(ctx, stype))
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_fromi128");
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xro);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, xro);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xro);
				return(1);
			}
		}
	}else
	{
		if(dtype.val==CCXL_TY_BIGINT)
		{
			if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tri);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tri);
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromi64big");
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tri);
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
				return(1);
			}

			if(BGBCC_CCXL_TypeSmallInt128P(ctx, stype))
			{
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, xri);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, xri);
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_fromi128");
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, xri);
				BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tro);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tro);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tro);
				return(1);
			}
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitJCmpVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, cdreg, rcls;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, invert;
	int i, j, k;
	
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	nm1=-1;
	nm2=-1;
	nm3=-1;
	noflip=0;

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		if(sctx->has_alux)
		{
			nm3=-1; flip=0;
			switch(cmp)
			{
			case CCXL_CMP_EQQ:
				nm1=BGBCC_SH_NMID_CMPXEQ;
				nm2=BGBCC_SH_NMID_BT;
				break;
			case CCXL_CMP_NEQQ:
				nm1=BGBCC_SH_NMID_CMPXEQ;
				nm2=BGBCC_SH_NMID_BF;
				break;

			default:
				nm1=-1;
				nm2=-1;
				nm3=-1;
				break;
			}
		}
	}else
	{
	//	if(sctx->has_alux)
		if(1)
		{
			nm3=-1; flip=0;
			switch(cmp)
			{
			case CCXL_CMP_EQQ:
				nm1=BGBCC_SH_NMID_CMPQEQ;
				nm2=BGBCC_SH_NMID_BT;
				break;
			case CCXL_CMP_NEQQ:
				nm1=BGBCC_SH_NMID_CMPQEQ;
				nm2=BGBCC_SH_NMID_BF;
				break;

			default:
				nm1=-1;
				nm2=-1;
				nm3=-1;
				break;
			}
		}
	}

	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		s0=NULL; flip=0; invert=0;
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			s0="__lvax_cmp_eq";	break;
		case CCXL_CMP_NE:
			s0="__lvax_cmp_eq";
			invert=1;
			break;
		case CCXL_CMP_LT:
			s0="__lvax_cmp_gt";
			flip=1;
			break;
		case CCXL_CMP_GT:
			s0="__lvax_cmp_gt";
			break;
		case CCXL_CMP_LE:
			s0="__lvax_cmp_gt";
			invert=1;
			break;
		case CCXL_CMP_GE:
			s0="__lvax_cmp_gt";
			flip=1;
			invert=1;
			break;
		case CCXL_CMP_TST:	
			s0="__lvax_cmp_tst";		break;
		case CCXL_CMP_NTST:	
			s0="__lvax_cmp_tst";
			invert=1;
			break;
		}

		if(s0)
		{
			if(flip)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(2));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(0));
			}else
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
			}

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST,
				BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
			if(invert)
				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			else
				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);

			return(1);
		}
	}
	else
	{
		s0=NULL; flip=0; invert=0;
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			s0="__lva_cmp_eq";	break;
		case CCXL_CMP_NE:
			s0="__lva_cmp_eq";
			invert=1;
			break;
	//		s0="__lva_cmp_ne";	break;
		case CCXL_CMP_LT:
	//		s0="__lva_cmp_ge";
			s0="__lva_cmp_gt";
			flip=1;
			break;
		case CCXL_CMP_GT:
			s0="__lva_cmp_gt";
			break;
		case CCXL_CMP_LE:
			s0="__lva_cmp_gt";
	//		flip=1;
			invert=1;
			break;
		case CCXL_CMP_GE:
	//		s0="__lva_cmp_ge";
			s0="__lva_cmp_gt";
			flip=1;
			invert=1;
			break;
		case CCXL_CMP_TST:	
			s0="__lva_cmp_tst";		break;
		case CCXL_CMP_NTST:	
			s0="__lva_cmp_tst";
			invert=1;
			break;
	//		s0="__lva_cmp_ntst";	break;
		}

		if(s0)
		{
			if(flip)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(1));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(0));
			}else
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(1));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
			}

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST,
				BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
			if(invert)
				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			else
				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);

			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegVariant(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg, rcls;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, invert;
	int i, j, k;
	
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	nm1=-1;
	nm2=-1;
	nm3=-1;
	noflip=0;

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		if(sctx->has_alux)
		{
			nm3=-1; flip=0;
			switch(cmp)
			{
			case CCXL_CMP_EQQ:
				nm1=BGBCC_SH_NMID_CMPXEQ;
				nm2=BGBCC_SH_NMID_BT;
				break;
			case CCXL_CMP_NEQQ:
				nm1=BGBCC_SH_NMID_CMPXEQ;
				nm2=BGBCC_SH_NMID_BF;
				break;

			default:
				nm1=-1;
				nm2=-1;
				nm3=-1;
				break;
			}
		}
	}else
	{
	//	if(sctx->has_alux)
		if(1)
		{
			nm3=-1; flip=0;
			switch(cmp)
			{
			case CCXL_CMP_EQQ:
				nm1=BGBCC_SH_NMID_CMPQEQ;
				nm2=BGBCC_SH_NMID_BT;
				break;
			case CCXL_CMP_NEQQ:
				nm1=BGBCC_SH_NMID_CMPQEQ;
				nm2=BGBCC_SH_NMID_BF;
				break;

			default:
				nm1=-1;
				nm2=-1;
				nm3=-1;
				break;
			}
		}
	}

	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
		}else
		{
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		}

//		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if(rcls==BGBCC_SH_REGCLS_QGR2)
	{
		s0=NULL; flip=0; invert=0;
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			s0="__lvax_cmp_eq";	break;
		case CCXL_CMP_NE:
			s0="__lvax_cmp_eq";
			invert=1;
			break;
		case CCXL_CMP_LT:
			s0="__lvax_cmp_gt";
			flip=1;
			break;
		case CCXL_CMP_GT:
			s0="__lvax_cmp_gt";
			break;
		case CCXL_CMP_LE:
			s0="__lvax_cmp_gt";
			invert=1;
			break;
		case CCXL_CMP_GE:
			s0="__lvax_cmp_gt";
			flip=1;
			invert=1;
			break;
		case CCXL_CMP_TST:	
			s0="__lvax_cmp_tst";		break;
		case CCXL_CMP_NTST:	
			s0="__lvax_cmp_tst";
			invert=1;
			break;
		}

		if(s0)
		{
			if(flip)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(2));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(0));
			}else
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
			}

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST,
//				BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
//			if(invert)
//				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
//			else
//				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);

			if(invert)
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_TST, BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx,
					BGBCC_SH_NMID_MOVT, BGBCC_SH_REG_R2);
			}

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);

			return(1);
		}
	}
	else
	{
		s0=NULL; flip=0; invert=0;
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			s0="__lva_cmp_eq";	break;
		case CCXL_CMP_NE:
			s0="__lva_cmp_eq";
			invert=1;
			break;
	//		s0="__lva_cmp_ne";	break;
		case CCXL_CMP_LT:
	//		s0="__lva_cmp_ge";
			s0="__lva_cmp_gt";
			flip=1;
			break;
		case CCXL_CMP_GT:
			s0="__lva_cmp_gt";
			break;
		case CCXL_CMP_LE:
			s0="__lva_cmp_gt";
	//		flip=1;
			invert=1;
			break;
		case CCXL_CMP_GE:
	//		s0="__lva_cmp_ge";
			s0="__lva_cmp_gt";
			flip=1;
			invert=1;
			break;
		case CCXL_CMP_TST:	
			s0="__lva_cmp_tst";		break;
		case CCXL_CMP_NTST:	
			s0="__lva_cmp_tst";
			invert=1;
			break;
	//		s0="__lva_cmp_ntst";	break;
		}

		if(s0)
		{
			if(flip)
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(1));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(0));
			}else
			{
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_ARG(1));
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
			}

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);

//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST,
//				BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
//			if(invert)
//				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
//			else
//				BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);

			if(invert)
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_TST, BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
				BGBCC_JX2C_EmitDstRegOp(ctx, sctx,
					BGBCC_SH_NMID_MOVT, BGBCC_SH_REG_R2);
			}

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);

			return(1);
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
