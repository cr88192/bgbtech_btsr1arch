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
 * Int128 Arithmetic
 */

int BGBCC_JX2C_IndexLitInt128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	s64 val_lo, s64 val_hi)
{
	int i, j, k;

	if(!sctx->lvt16_lbl)
	{
		sctx->lvt16_lbl=sctx->t_lvt16_lbl;
		sctx->lvt16_val=sctx->t_lvt16_val;
		sctx->lvt16_n_idx=0;
		sctx->lvt16_m_idx=1024;
	}
	
	for(i=0; i<sctx->lvt16_n_idx; i++)
	{
		if((sctx->lvt16_val[i*2+0]==val_lo) &&
			(sctx->lvt16_val[i*2+1]==val_hi))
				{ return(sctx->lvt16_lbl[i]); }
	}
	
	i=sctx->lvt16_n_idx++;
	k=BGBCC_JX2_GenLabel(sctx);
	sctx->lvt16_lbl[i]=k;
	sctx->lvt16_val[i*2+0]=val_lo;
	sctx->lvt16_val[i*2+1]=val_hi;
	return(k);
}

int BGBCC_JX2C_EmitBinaryVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	s64 li, lj;
	int csreg, ctreg, cdreg;
	int tr0, tr1, tr2, tr3;
	char *s0;
	int nm1, nm2, shl, mul2x_sh;
	int i, j, k;
	
	nm1=-1;
	
	if(sctx->has_alux)
	{
		nm1=-1;
		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADDX;	break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUBX;	break;
		case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_ANDX;	break;
		case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_ORX;	break;
		case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XORX;	break;

		case CCXL_BINOP_SHL:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDX;
			else
				nm1=BGBCC_SH_NMID_SHADX;
			break;
		case CCXL_BINOP_SHR:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLRX;
			else
				nm1=BGBCC_SH_NMID_SHARX;
			break;
		default:
			break;
		}
	}

	if(nm1>=0)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			nm1, cdreg, ctreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmSmallLongP(ctx, treg) && (sctx->has_alux))
	{
		lj=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		shl=-1;
		j=lj;

		if(!(lj&(lj-1)))
		{
			for(i=0; i<31; i++)
				if(lj==(1LL<<i))
					{ shl=i; break; }
		}

		mul2x_sh=-1;
		if(opr==CCXL_BINOP_MUL)
		{
			if(j==lj)
				mul2x_sh=BGBCC_JX2C_GetMulSh2xForImm(j);
		}

#if 1
		if(mul2x_sh>0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;

			if((mul2x_sh&15)!=0)
			{
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR2);
			}else
			{
				tr0=csreg;
			}
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);
			tr2=-1;
			tr3=-1;
			if(((mul2x_sh>>8)&15)!=0)
			{
				tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR2);
			}
			if(((mul2x_sh>>12)&15)!=0)
			{
				tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR2);
			}

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			if(csreg!=tr0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>0)&15), tr0);
			}
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>4)&15), tr1);

			if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>8)&15), tr2);
			}
			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>12)&15), tr3);
			}

			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr0, tr1, tr1);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr2, tr3, tr3);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr1, tr3, cdreg);
			}else
				if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr0, tr1, cdreg);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, cdreg, tr2, cdreg);
			}else
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr0, tr1, cdreg);
			}

			if(csreg!=tr0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			if(tr2>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
			if(tr3>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif
	}


	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xli_add"; break;
	case CCXL_BINOP_SUB:	s0="__xli_sub"; break;
	case CCXL_BINOP_MUL:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_umul";
		else
			s0="__xli_smul";
		break;
	case CCXL_BINOP_DIV:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_udiv";
		else
			s0="__xli_sdiv";
		break;
	case CCXL_BINOP_MOD:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_umod";
		else
			s0="__xli_smod";
		break;
	case CCXL_BINOP_AND:	s0="__xli_and"; break;
	case CCXL_BINOP_OR:		s0="__xli_or"; break;
	case CCXL_BINOP_XOR:	s0="__xli_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xli_shl"; break;
	case CCXL_BINOP_SHR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_shlr";
		else
			s0="__xli_shar";
		break;
	case CCXL_BINOP_SHRR:
		s0="__xli_shlr";
		break;
	}

#if 1
	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	s64 lj;
	int csreg, ctreg, cdreg;
	int tr0, tr1, tr2, tr3;
	char *s0;
	int nm1, nm2, shl, mul2x_sh;
	int i, j, k;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
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
			i=BGBCC_JX2C_EmitBinaryVRegVRegInt128(ctx, sctx,
				type, dreg, opr, sreg);
			return(i);
		}
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		if(opr==CCXL_BINOP_SHL)
		{
			if(j==1)
			{
				return(BGBCC_JX2C_EmitBinaryVRegVRegVRegInt128(
					ctx, sctx,
					type, dreg,
					CCXL_BINOP_ADD, sreg, sreg));
			}
		}
	}

	nm1=-1;
	
	if(sctx->has_alux)
	{
		nm1=-1;
		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADDX;	break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUBX;	break;
		case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_ANDX;	break;
		case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_ORX;	break;
		case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XORX;	break;

		case CCXL_BINOP_SHL:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDX;
			else
				nm1=BGBCC_SH_NMID_SHADX;
			break;
		case CCXL_BINOP_SHR:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLRX;
			else
				nm1=BGBCC_SH_NMID_SHARX;
			break;

		default:
			break;
		}
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, treg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
					nm1, cdreg, cdreg, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

			BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
				nm1, csreg, cdreg, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, treg))
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

			BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
				nm1, csreg, csreg, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			nm1, csreg, ctreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}


	if(BGBCC_CCXL_IsRegImmSmallLongP(ctx, treg) && (sctx->has_alux))
	{
		lj=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		shl=-1;
		j=lj;

		if(!(lj&(lj-1)))
		{
			for(i=0; i<31; i++)
				if(lj==(1LL<<i))
					{ shl=i; break; }
		}

		mul2x_sh=-1;
		if(opr==CCXL_BINOP_MUL)
		{
			if(j==lj)
				mul2x_sh=BGBCC_JX2C_GetMulSh2xForImm(j);
		}

#if 1
		if(mul2x_sh>0)
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, dreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				csreg=cdreg;
			}else
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			}

			if((mul2x_sh&15)!=0)
			{
				tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR2);
			}else
			{
				tr0=csreg;
			}
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);
			tr2=-1;
			tr3=-1;
			if(((mul2x_sh>>8)&15)!=0)
			{
				tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR2);
			}
			if(((mul2x_sh>>12)&15)!=0)
			{
				tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
					BGBCC_SH_REGCLS_QGR2);
			}

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			if(csreg!=tr0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>0)&15), tr0);
			}
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>4)&15), tr1);

			if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>8)&15), tr2);
			}
			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADX, csreg, ((mul2x_sh>>12)&15), tr3);
			}

			if(tr3>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr0, tr1, tr1);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr2, tr3, tr3);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr1, tr3, cdreg);
			}else
				if(tr2>=0)
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr0, tr1, cdreg);
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, cdreg, tr2, cdreg);
			}else
			{
				BGBCC_JX2_EmitOpRegRegReg(sctx,
					BGBCC_SH_NMID_ADDX, tr0, tr1, cdreg);
			}

			if(csreg!=tr0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			if(tr2>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
			if(tr3>=0)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);

			if(csreg!=cdreg)
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xli_add"; break;
	case CCXL_BINOP_SUB:	s0="__xli_sub"; break;
	case CCXL_BINOP_MUL:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_umul";
		else
			s0="__xli_smul";
		break;
	case CCXL_BINOP_DIV:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_udiv";
		else
			s0="__xli_sdiv";
		break;
	case CCXL_BINOP_MOD:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_umod";
		else
			s0="__xli_smod";
		break;
	case CCXL_BINOP_AND:	s0="__xli_and"; break;
	case CCXL_BINOP_OR:		s0="__xli_or"; break;
	case CCXL_BINOP_XOR:	s0="__xli_xor"; break;
	case CCXL_BINOP_SHL:	s0="__xli_shl"; break;
	case CCXL_BINOP_SHR:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_shlr";
		else
			s0="__xli_shar";
		break;
	case CCXL_BINOP_SHRR:
		s0="__xli_shlr";
		break;
	}

#if 1
	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
		return(1);
	}
#endif

	BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_JX2C_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_JX2C_EmitUnaryVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	ccxl_register ttreg;
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}

	if(opr==CCXL_UNOP_LNOT)
	{
//		ttreg.val=CCXL_REGTY_IMM_INT;
		BGBCC_CCXL_GetRegForInt128Value(ctx, &ttreg, 0, 0);
		i=BGBCC_JX2C_EmitCompareVRegVRegVRegInt128(ctx, sctx,
			type, dreg, CCXL_CMP_EQ, sreg, ttreg);
		return(0);
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__xli_neg"; break;
	case CCXL_UNOP_NOT:	s0="__xli_not"; break;
	}

#if 1
	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		return(1);
	}
#endif

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg, csreg2, ctreg2;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, isptr;
	int i, j, k;

	nm1=-1;
	nm2=-1;
	nm3=-1;
	noflip=0;

	isptr=0;
	if(BGBCC_CCXL_TypeQuadPointerP(ctx, type))
		isptr=1;

	if(sctx->has_alux)
	{
		nm3=-1; flip=0;
		switch(opr)
		{
		case CCXL_CMP_EQ:
			nm1=BGBCC_SH_NMID_CMPXEQ;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_NE:
			nm1=BGBCC_SH_NMID_CMPXEQ;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_LT:
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BT;
			}
			break;
		case CCXL_CMP_GT:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BT;
			}
			break;
		case CCXL_CMP_LE:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BF;
			}
			break;
		case CCXL_CMP_GE:
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BF;
			}
			break;

#if 0
		case CCXL_CMP_TST:
			nm1=BGBCC_SH_NMID_TSTQ;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_NTST:
			nm1=BGBCC_SH_NMID_TSTQ;
			nm2=BGBCC_SH_NMID_BT;
			break;
#endif

		default:
			nm1=-1;
			nm2=-1;
			nm3=-1;
			break;
		}
	}

	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(isptr)
		{
			csreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
				BGBCC_SH_REGCLS_QGR2);
			ctreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
				BGBCC_SH_REGCLS_QGR2);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, csreg, csreg2);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, ctreg, ctreg2);

			if(flip)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg2, ctreg2);
			else
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg2, csreg2);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg2);
		}else
		{
			if(flip)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
			else
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);
		}

		if(nm2==BGBCC_SH_NMID_BF)
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
		else
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	flip=0;
	s0=NULL;
	switch(opr)
	{
	case CCXL_CMP_EQ:
		s0="__xli_cmp_eq";	break;
	case CCXL_CMP_NE:
		s0="__xli_cmp_ne";	break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xli_cmp_hi"; flip=1; }
		else
			{ s0="__xli_cmp_gt"; flip=1; }
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_cmp_hi";
		else
			s0="__xli_cmp_gt";
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xli_cmp_he"; flip=1; }
		else
			{ s0="__xli_cmp_ge"; flip=1; }
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_cmp_he";
		else
			s0="__xli_cmp_ge";
		break;
	case CCXL_CMP_TST:	
		s0="__xli_cmp_tst";		break;
	case CCXL_CMP_NTST:	
		s0="__xli_cmp_ntst";	break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		if(flip)
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(0));
		}else
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		}
		
		if(isptr)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, BGBCC_JX2CC_PSREG_LRARG(0), BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, BGBCC_JX2CC_PSREG_LRARG(2), BGBCC_JX2CC_PSREG_LRARG(2));
		}
		
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R2);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, cdreg, csreg2, ctreg2;
	char *s0;
	int nm1, nm2, nm3, flip, noflip, isptr;
	int i, j, k;
	

	nm1=-1;
	nm2=-1;
	nm3=-1;
	noflip=0;

	isptr=0;
	if(BGBCC_CCXL_TypeQuadPointerP(ctx, type))
		isptr=1;

	if(sctx->has_alux)
	{
		nm3=-1; flip=0;
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			nm1=BGBCC_SH_NMID_CMPXEQ;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_NE:
			nm1=BGBCC_SH_NMID_CMPXEQ;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_LT:
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BT;
			}
			break;
		case CCXL_CMP_GT:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BT;
			}
			break;
		case CCXL_CMP_LE:
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BF;
			}
			break;
		case CCXL_CMP_GE:
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPXHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPXGT;
				nm2=BGBCC_SH_NMID_BF;
			}
			break;

#if 0
		case CCXL_CMP_TST:
			nm1=BGBCC_SH_NMID_TSTQ;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_NTST:
			nm1=BGBCC_SH_NMID_TSTQ;
			nm2=BGBCC_SH_NMID_BT;
			break;
#endif

		default:
			nm1=-1;
			nm2=-1;
			nm3=-1;
			break;
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

		if(isptr)
		{
			csreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
				BGBCC_SH_REGCLS_QGR2);
			ctreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx,
				BGBCC_SH_REGCLS_QGR2);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, csreg, csreg2);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, ctreg, ctreg2);

			if(flip)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg2, ctreg2);
			else
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg2, csreg2);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg2);
		}else
		{
			if(flip)
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
			else
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);
		}

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	s0=NULL; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		s0="__xli_cmp_eq";	break;
	case CCXL_CMP_NE:
		s0="__xli_cmp_ne";	break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xli_cmp_hi"; flip=1; }
		else
			{ s0="__xli_cmp_gt"; flip=1; }
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_cmp_hi";
		else
			s0="__xli_cmp_gt";
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ s0="__xli_cmp_he"; flip=1; }
		else
			{ s0="__xli_cmp_ge"; flip=1; }
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			s0="__xli_cmp_he";
		else
			s0="__xli_cmp_ge";
		break;
	case CCXL_CMP_TST:	
		s0="__xli_cmp_tst";		break;
	case CCXL_CMP_NTST:	
		s0="__xli_cmp_ntst";	break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
	
		if(flip)
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(0));
		}else
		{
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		}

		if(isptr)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, BGBCC_JX2CC_PSREG_LRARG(0), BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOVZT, BGBCC_JX2CC_PSREG_LRARG(2), BGBCC_JX2CC_PSREG_LRARG(2));
		}
		
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
//			dreg, BGBCC_SH_REG_R2);

		if(sctx->emit_riscv&0x33)
		{
			BGBCC_JX2_EmitOpRegRegLbl(sctx,
				BGBCC_SH_NMID_BRNE,
				BGBCC_JX2CC_PSREG_RQRET, BGBCC_SH_REG_ZZR,
				lbl);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TST,
				BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);
		}

		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}



int BGBCC_JX2C_EmitBinaryVRegVRegFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2;
	int i, j, k;
	
	nm1=-1;
	
	if(sctx->has_fpux)
	{
		nm1=-1;
		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_FADDX;	break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_FSUBX;	break;
		case CCXL_BINOP_MUL:	nm1=BGBCC_SH_NMID_FMULX;	break;
		default:
			break;
		}
	}

	if(nm1>=0)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			nm1, cdreg, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xlf_add"; break;
	case CCXL_BINOP_SUB:	s0="__xlf_sub"; break;
	case CCXL_BINOP_MUL:	s0="__xlf_mul"; break;
	case CCXL_BINOP_DIV:	s0="__xlf_div"; break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2;
	int i, j, k;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_JX2C_EmitBinaryVRegVRegFloat128(ctx, sctx, type,
			dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		k=0;
		switch(opr)
		{
		case CCXL_BINOP_ADD:	k=1; break;
		case CCXL_BINOP_MUL:	k=1; break;
		}

		if(k>0)
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegFloat128(ctx, sctx, type,
				dreg, opr, sreg);
			return(i);
		}
	}

	nm1=-1;
	
	if(sctx->has_fpux)
	{
		nm1=-1;
		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_FADDX;	break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_FSUBX;	break;
		case CCXL_BINOP_MUL:	nm1=BGBCC_SH_NMID_FMULX;	break;
		default:
			break;
		}
	}

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2C_EmitOpRegRegReg(ctx, sctx,
			nm1, csreg, ctreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__xlf_add"; break;
	case CCXL_BINOP_SUB:	s0="__xlf_sub"; break;
	case CCXL_BINOP_MUL:	s0="__xlf_mul"; break;
	case CCXL_BINOP_DIV:	s0="__xlf_div"; break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_JX2CC_PSREG_LRARG(2));
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
		return(1);
	}

	BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_JX2C_EmitBinaryVRegVRegFloat128(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_JX2C_EmitUnaryVRegVRegFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
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
		BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__xlf_neg"; break;
//	case CCXL_UNOP_NOT:	s0="__xlf_not"; break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvVRegVRegToFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type stype, ccxl_register dreg, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	int st, trs;
	s32 imm;
	int i, j, k;

	st=stype.val;

	s0=NULL;
	trs=BGBCC_JX2CC_PSREG_LRARG(0);

	if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
	{
		s0="__xlf_fromint";
		trs=BGBCC_JX2CC_PSREG_ARG(0);
	}

	if(	BGBCC_CCXL_TypeDoubleP(ctx, stype) ||
		BGBCC_CCXL_TypeFloatP(ctx, stype) ||
		BGBCC_CCXL_TypeFloat16P(ctx, stype))
	{
		s0="__xlf_fromdbl";
		trs=BGBCC_JX2CC_PSREG_ARG(0);
	}

//	switch(opr)
//	{
//	case CCXL_UNOP_NEG:	s0="__xlf_neg"; break;
//	case CCXL_UNOP_NOT:	s0="__xlf_not"; break;
//	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, trs);
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_JX2CC_PSREG_LRRET);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitConvVRegVRegFromFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type dtype, ccxl_register dreg, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	int trd;
	int dt;
	s32 imm;
	int i, j, k;

	dt=dtype.val;

	s0=NULL;
	trd=BGBCC_JX2CC_PSREG_LRRET;

	if(BGBCC_CCXL_TypeSmallLongP(ctx, dtype))
	{
		s0="__xlf_toint";
		trd=BGBCC_JX2CC_PSREG_RQRET;
	}

	if(	BGBCC_CCXL_TypeDoubleP(ctx, dtype) ||
		BGBCC_CCXL_TypeFloatP(ctx, dtype) ||
		BGBCC_CCXL_TypeFloat16P(ctx, dtype))
	{
		s0="__xlf_todbl";
		trd=BGBCC_JX2CC_PSREG_RQRET;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
		BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, trd);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, flip;
	int i, j, k;
	

	nm1=-1;
	nm2=-1;
	nm3=-1;

	if(sctx->has_fpux)
	{
		nm3=-1; flip=0;
		switch(opr)
		{
		case CCXL_CMP_EQ:
			nm1=BGBCC_SH_NMID_FCMPXEQ;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_NE:
			nm1=BGBCC_SH_NMID_FCMPXEQ;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_LT:
			flip=1;
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_GT:
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_LE:
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_GE:
			flip=1;
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BF;
			break;

		default:
			nm1=-1;
			nm2=-1;
			nm3=-1;
			break;
		}
	}

	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		if(nm2==BGBCC_SH_NMID_BF)
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
		else
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	s0=NULL;	flip=0;
	switch(opr)
	{
		case CCXL_CMP_EQ:	s0="__xlf_cmp_eq";	break;
		case CCXL_CMP_NE:	s0="__xlf_cmp_ne";	break;
		case CCXL_CMP_GT:	s0="__xlf_cmp_gt";	break;
		case CCXL_CMP_LT:	s0="__xlf_cmp_gt"; flip=1;	break;
		case CCXL_CMP_GE:	s0="__xlf_cmp_ge";	break;
		case CCXL_CMP_LE:	s0="__xlf_cmp_ge"; flip=1;	break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

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
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R2);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVRegFloat128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, flip;
	int i, j, k;
	
	nm1=-1;
	nm2=-1;
	nm3=-1;

	if(sctx->has_fpux)
	{
		nm3=-1; flip=0;
		switch(cmp)
		{
		case CCXL_CMP_EQ:
			nm1=BGBCC_SH_NMID_FCMPXEQ;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_NE:
			nm1=BGBCC_SH_NMID_FCMPXEQ;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_LT:
			flip=1;
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_GT:
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BT;
			break;
		case CCXL_CMP_LE:
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BF;
			break;
		case CCXL_CMP_GE:
			flip=1;
			nm1=BGBCC_SH_NMID_FCMPXGT;
			nm2=BGBCC_SH_NMID_BF;
			break;

		default:
			nm1=-1;
			nm2=-1;
			nm3=-1;
			break;
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

	s0=NULL; flip=0;
	switch(cmp)
	{
		case CCXL_CMP_EQ:	s0="__xlf_cmp_eq";	break;
		case CCXL_CMP_NE:	s0="__xlf_cmp_ne";	break;
		case CCXL_CMP_GT:	s0="__xlf_cmp_gt";	break;
		case CCXL_CMP_LT:	s0="__xlf_cmp_gt"; flip=1;	break;
		case CCXL_CMP_GE:	s0="__xlf_cmp_ge";	break;
		case CCXL_CMP_LE:	s0="__xlf_cmp_ge"; flip=1;	break;
	}

	if(s0)
	{
		BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);

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
		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BF, lbl);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


