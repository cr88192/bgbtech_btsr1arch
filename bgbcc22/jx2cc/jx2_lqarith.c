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
Native Quadword Ops
*/


int BGBCC_JX2C_EmitBinaryVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	s64 li, lj;
	int csreg, ctreg, cdreg;
	int tr0;
	int nm1, nm2;
	int i, j, k, shl;

#if 1
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	if(BGBCC_CCXL_IsRegImmSmallLongP(ctx, treg))
	{
//		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		lj=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		shl=-1;
		j=lj;

		if(!(lj&(lj-1)))
		{
			for(i=0; i<31; i++)
				if(lj==(1LL<<i))
					{ shl=i; break; }
		}

		nm1=-1; k=0;
//		if((opr==CCXL_BINOP_ADD) && (j==(sbyte)j))
//			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
//		if((opr==CCXL_BINOP_SUB) && ((-j)==(sbyte)(-j)))
//			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if((opr==CCXL_BINOP_ADD) && (lj==(s16)lj))
			{ nm1=BGBCC_SH_NMID_ADD; k=lj; }
		if((opr==CCXL_BINOP_SUB) && ((-lj)==(s16)(-lj)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-lj; }

#if 1
		if((opr==CCXL_BINOP_SHL) && (lj==(lj&63)))
		{
			k=lj;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}

		if((opr==CCXL_BINOP_SHR) && (lj==(lj&63)))
		{
			k=-lj;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}

		if((opr==CCXL_BINOP_SHRR) && (lj==(lj&63)))
		{
			k=-lj;
			nm1=BGBCC_SH_NMID_SHLDQ;
		}
#endif

		if(nm1>=0)
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, nm1, k, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

#if 1
		if((opr==CCXL_BINOP_MUL) && (shl>=0))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ nm1=BGBCC_SH_NMID_SHLDQ; }
			else
				{ nm1=BGBCC_SH_NMID_SHADQ; }
			
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
				nm1, cdreg, shl, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if((opr==CCXL_BINOP_DIV) && (shl>=0))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
					BGBCC_SH_NMID_SHLDQ, cdreg, -shl, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			if(BGBCC_CCXL_TypeSignedP(ctx, type) &&
				ctx->arch_has_predops)
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_CMPGT, 0, cdreg);
				BGBCC_JX2C_EmitOpImmRegPredF(ctx, sctx,
					BGBCC_SH_NMID_ADD, j-1, cdreg);
				BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
					BGBCC_SH_NMID_SHADQ, cdreg, -shl, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}
		
		if((opr==CCXL_BINOP_MOD) && (shl>=0))
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_AND, j-1, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}
#endif
	}
#endif

	nm1=-1;
	if(opr==CCXL_BINOP_ADD)
		nm1=BGBCC_SH_NMID_ADD;
	if(opr==CCXL_BINOP_SUB)
		nm1=BGBCC_SH_NMID_SUB;
//	if(opr==CCXL_BINOP_MUL)
//		nm1=BGBCC_SH_NMID_MULL;

	if(opr==CCXL_BINOP_AND)
		nm1=BGBCC_SH_NMID_AND;
	if(opr==CCXL_BINOP_OR)
		nm1=BGBCC_SH_NMID_OR;
	if(opr==CCXL_BINOP_XOR)
		nm1=BGBCC_SH_NMID_XOR;

	if(sctx->has_qmul&1)
	{
		if(opr==CCXL_BINOP_MUL)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MULUQ;
			else
				nm1=BGBCC_SH_NMID_MULSQ;
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_DIVUQ;
			else
				nm1=BGBCC_SH_NMID_DIVSQ;
		}

		if(opr==CCXL_BINOP_MOD)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MODUQ;
			else
			{
//				nm1=BGBCC_SH_NMID_MODSQ;
			}
		}
	}


	if(nm1>=0)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHLDQ;
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLRQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}else
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHADQ;
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHARQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}

//	if(nm1>=0)
//	if((nm1>=0) && sctx->has_bjx1ari)
	if((nm1>=0) && sctx->has_bjx1ari && sctx->has_bjx1mov)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, cdreg, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if((nm1>=0) && sctx->has_bjx1ari)
	{
//		BGBCC_DBGBREAK
	}

	if(opr==CCXL_BINOP_MUL)
	{
//		if(sctx->is_betav && !sctx->has_bjx1ari)
//		if(sctx->is_betav && (!sctx->has_bjx1ari || !sctx->has_bjx1mov))
		if(1)
		{
			/* B64V: Use function call for 'long' multiply. */

			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
		
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);

//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);

			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__umulsq"); }
			else
				{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__smulsq"); }

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);

//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			return(1);
		}

#if 0
		ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterDirty(ctx, sctx, dreg);

		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MULQ, ctreg, cdreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
#endif
	}

	if(opr==CCXL_BINOP_DIV)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__udivsq"); }
		else
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__sdivsq"); }

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);

//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);

		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__umodsq"); }
		else
			{ BGBCC_JX2C_EmitCallName(ctx, sctx, "__smodsq"); }

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);

//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
		return(1);
	}


	nm1=-1;

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHLDQ;
//		if(opr==CCXL_BINOP_SHR)
//			nm1=BGBCC_SH_NMID_SHLDQ;
//		if(opr==CCXL_BINOP_SHRR)
//			nm1=BGBCC_SH_NMID_SHLDQ;

		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLRQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}else
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHADQ;
//		if(opr==CCXL_BINOP_SHR)
//			nm1=BGBCC_SH_NMID_SHADQ;
//		if(opr==CCXL_BINOP_SHRR)
//			nm1=BGBCC_SH_NMID_SHLDQ;

		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHARQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHARQ;
	}

	if(nm1>=0)
//	if((nm1>=0) && sctx->has_bjx1ari)
	{
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}


	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLDQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLDQ;
	}else
	{
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHADQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLDQ;
	}

	if(nm1>=0)
	{
//		tr0=BGBCC_SH_REG_RQ0;
		tr0=BGBCC_SH_REG_RQ2;
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_NEG, ctreg, tr0);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, tr0, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}


	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	s64	li;
	int csreg, ctreg, cdreg;
	int tr0;
	int nm1, nm2;
	int i, j, k, shl;

	li=-1;
	j=-999;
	k=0;

//	if(type.val==0x0D)
	if((type.val==0x0D) &&
		((dreg.val&4095)==(sreg.val&4095)) &&
		((dreg.val>>56)==(sreg.val>>56)))
	{
		if(opr==CCXL_BINOP_SHL)
		{
			i=-1;
		}
	}

	if(	BGBCC_CCXL_IsRegImmLongP(ctx, treg) ||
		BGBCC_CCXL_IsRegImmUIntP(ctx, treg))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
		j=(int)li;
		
		if(j!=li)
		{
			k=-1;
		}
	}
	
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) || (j==li))
	{
//		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		j=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);

		if((j==0) && ((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB) ||
			(opr==CCXL_BINOP_OR) || (opr==CCXL_BINOP_XOR) ||
			(opr==CCXL_BINOP_SHL) || (opr==CCXL_BINOP_SHR)))
		{
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
			return(1);
		}
		
		if(opr==CCXL_BINOP_SHR)
		{
			i=-1;
		}
		

		shl=-1;
		if(!(j&(j-1)))
		{
			for(i=0; i<31; i++)
				if(j==(1<<i))
					{ shl=i; break; }
		}

		nm1=-1; k=0;
		if((opr==CCXL_BINOP_ADD) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
		if((opr==CCXL_BINOP_SUB) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//		if((opr==CCXL_BINOP_MUL) && (j==(sbyte)j))
//			{ nm1=BGBCC_SH_NMID_MULQ; k=j; }

		if((opr==CCXL_BINOP_ADD) && ((-j)==(byte)(-j)))
			{ nm1=BGBCC_SH_NMID_SUB; k=-j; }
		if((opr==CCXL_BINOP_SUB) && ((-j)==(byte)(-j)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if((opr==CCXL_BINOP_AND) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_AND; k=j; }
		if((opr==CCXL_BINOP_OR) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_OR; k=j; }
		if((opr==CCXL_BINOP_XOR) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_XOR; k=j; }

#if 0
		if((opr==CCXL_BINOP_SHL) && (j==(j&63)))
		{
			k=j;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}

		if((opr==CCXL_BINOP_SHR) && (j==(j&63)))
		{
			k=-j;
			nm1=BGBCC_SH_NMID_SHADQ;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLDQ;
		}
#endif

//		if((nm1<0) && (j==((j<<12)>>12)))
//		if((nm1<0) && (j==((j<<16)>>16)))
//		if(nm1<0)
		if(sctx->has_jumbo && (nm1<0))
		{
			if(opr==CCXL_BINOP_ADD)
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if(opr==CCXL_BINOP_SUB)
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//			if(opr==CCXL_BINOP_MUL)
//				{ nm1=BGBCC_SH_NMID_MULL; k=j; }

			if(opr==CCXL_BINOP_AND)
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if(opr==CCXL_BINOP_OR)
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if(opr==CCXL_BINOP_XOR)
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }
		}

#if 1
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			if((opr==CCXL_BINOP_SHL) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=j; }
			if((opr==CCXL_BINOP_SHR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }
			if((opr==CCXL_BINOP_SHRR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }

			if(opr==CCXL_BINOP_MUL && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=shl; }
			if(opr==CCXL_BINOP_DIV && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-shl; }

			if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
				{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
		}else
		{
			if((opr==CCXL_BINOP_SHL) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=j; }
			if((opr==CCXL_BINOP_SHR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=-j; }
			if((opr==CCXL_BINOP_SHRR) && (j==(j&63)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }

			if(opr==CCXL_BINOP_MUL && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=shl; }
//			if(opr==CCXL_BINOP_DIV && (shl>=0))
//				{ nm1=BGBCC_SH_NMID_SHADQ; k=-shl; }

//			if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
//				{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
		}
#endif

		if(sctx->has_jumbo && (nm1<0))
		{
			if(opr==CCXL_BINOP_ADD)
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if(opr==CCXL_BINOP_SUB)
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
//			if(opr==CCXL_BINOP_MUL)
//				{ nm1=BGBCC_SH_NMID_MULL; k=j; }

			if(opr==CCXL_BINOP_AND)
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if(opr==CCXL_BINOP_OR)
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if(opr==CCXL_BINOP_XOR)
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }
		}

#if 1
		if(sctx->has_qmul&1)
		{
			if(opr==CCXL_BINOP_MUL)
			{
				k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_MULUQ;
				else
					nm1=BGBCC_SH_NMID_MULSQ;
			}

			if(opr==CCXL_BINOP_DIV)
			{
				k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_DIVUQ;
				else
					nm1=BGBCC_SH_NMID_DIVSQ;
			}

			if(opr==CCXL_BINOP_MOD)
			{
				k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_MODUQ;
				else
				{
	//				nm1=BGBCC_SH_NMID_MODSQ;
				}
			}
		}
#endif

//		if(nm1>=0)
//		if((nm1>=0) && sctx->has_bjx1ari)
		if((nm1>=0) && sctx->has_bjx1ari && sctx->has_bjx1mov)
		{
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
				csreg=cdreg;
				BGBCC_JX2_EmitOpRegImmReg(sctx, nm1, csreg, k, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//			i=-1;
//			if((k==((k<<23)>>23)) || sctx->has_jumbo)

			i=BGBCC_JX2_TryEmitOpRegImmReg(sctx, nm1, csreg, k, cdreg);
			if(i>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			BGBCC_DBGBREAK

#if 0
			if((nm1==BGBCC_SH_NMID_SHADQ) || (nm1==BGBCC_SH_NMID_SHLDQ))
				BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
			BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);

//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
#endif
		}

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(type.val==0x0D)
	{
		if(opr==CCXL_BINOP_SHL)
		{
			i=-1;
		}
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		if(	(opr==CCXL_BINOP_SHL) ||
			(opr==CCXL_BINOP_SHR) )
		{
			i=-1;
		}

		i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx,
			type, dreg, opr, treg);
		return(i);
	}

	if(	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR))
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}

	if(	BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg) &&
		!BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, treg))
	{
		if(	(opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR)  ||
			(opr==CCXL_BINOP_XOR) )
		{
			i=BGBCC_JX2C_EmitBinaryVRegVRegVRegQLong(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}

	nm1=-1;
	if(opr==CCXL_BINOP_ADD)
		nm1=BGBCC_SH_NMID_ADD;
	if(opr==CCXL_BINOP_SUB)
		nm1=BGBCC_SH_NMID_SUB;
//	if(opr==CCXL_BINOP_MUL)
//		nm1=BGBCC_SH_NMID_MULQ;

	if(opr==CCXL_BINOP_AND)
		nm1=BGBCC_SH_NMID_AND;
	if(opr==CCXL_BINOP_OR)
		nm1=BGBCC_SH_NMID_OR;
	if(opr==CCXL_BINOP_XOR)
		nm1=BGBCC_SH_NMID_XOR;

	if(opr==CCXL_BINOP_SHL)
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			nm1=BGBCC_SH_NMID_SHLDQ;
		else
			nm1=BGBCC_SH_NMID_SHADQ;
	}
	if(opr==CCXL_BINOP_SHR)
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			nm1=BGBCC_SH_NMID_SHLRQ;
		else
			nm1=BGBCC_SH_NMID_SHARQ;
	}


	if(sctx->has_qmul&1)
	{
		if(opr==CCXL_BINOP_MUL)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MULUQ;
			else
				nm1=BGBCC_SH_NMID_MULSQ;
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_DIVUQ;
			else
				nm1=BGBCC_SH_NMID_DIVSQ;
		}

		if(opr==CCXL_BINOP_MOD)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_MODUQ;
			else
			{
//				nm1=BGBCC_SH_NMID_MODSQ;
			}
		}
	}

//	if(	BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
//		BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
//	{
//		nm1=-1;
//	}

//	if(nm1>=0)
//	if((nm1>=0) && sctx->has_bjx1ari)
	if((nm1>=0) && sctx->has_bjx1ari && sctx->has_bjx1mov)
//	if(0)
	{
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

//		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		BGBCC_JX2_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		if(	(opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR)  ||
			(opr==CCXL_BINOP_XOR) )
		{
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, treg);
			i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx,
				type, dreg, opr, sreg);
			return(i);
		}

		BGBCC_DBGBREAK
	}

	BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_JX2C_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_JX2C_EmitUnaryVRegVRegQLong(
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

	nm1=-1;
	if(opr==CCXL_UNOP_NEG)
		nm1=BGBCC_SH_NMID_NEG;
	if(opr==CCXL_UNOP_NOT)
		nm1=BGBCC_SH_NMID_NOT;

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
//			BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, cdreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}else
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
//			BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
	}

	if(opr==CCXL_UNOP_LNOT)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_TSTQ, cdreg, cdreg);
//			ctreg=cdreg&31;
			ctreg=cdreg&63;
//			BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_TSTQ, csreg, csreg);
//		ctreg=cdreg&31;
		ctreg=cdreg&63;
//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
		BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, ctreg);
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
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, j, cdreg);
//			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
//				BGBCC_SH_NMID_LEAB, cdreg, j, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		j=(opr==CCXL_UNOP_INC)?1:(-1);
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		BGBCC_JX2_EmitOpLdRegDispReg(sctx,
//			BGBCC_SH_NMID_LEAB, csreg, j, cdreg);

#if 1
		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, csreg, j, cdreg);
		if(i<=0)
//		if(1)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, j, cdreg);
		}
#endif

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCompareVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg, csreg2, ctreg2;
	s32 imm;
	int flip, noflip, doptrshl;
	int nm1, nm2, nm3, cmp1;
	int i;
	
	noflip=0;
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
//	{
//		noflip=1;
//		if(sctx->is_betav && !sctx->has_bjx1ari)
//			noflip=0;
//	}
	
	doptrshl=0;
	if(sctx->abi_spillpad&4)
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
		{
			doptrshl=1;
		}
	}
	
	nm3=-1; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
//		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
//		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
//				nm3=BGBCC_SH_NMID_BRLT;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_BRGT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
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
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
//				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
//		nm3=-1;
		break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(nm1==BGBCC_SH_NMID_CMPQEQ)
			nm1=BGBCC_SH_NMID_CMPPEQ;
		if(nm1==BGBCC_SH_NMID_CMPQGT)
			nm1=BGBCC_SH_NMID_CMPPGT;
		if(nm1==BGBCC_SH_NMID_CMPQGE)
			nm1=BGBCC_SH_NMID_CMPPGE;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		csreg2=csreg;
		ctreg2=ctreg;
		if(doptrshl)
		{
			csreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, csreg, 16, csreg2);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, ctreg, 16, ctreg2);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, csreg, csreg2);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, ctreg, ctreg2);
		}

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg2, ctreg2);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg2, csreg2);
//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);

#if 0
		BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		
		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, 1, cdreg);
		}
#endif

		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVNT, cdreg);
		}else
		{
			BGBCC_JX2C_EmitDstRegOp(ctx, sctx, BGBCC_SH_NMID_MOVT, cdreg);
		}
		
		if(doptrshl)
		{
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg2);
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

int BGBCC_JX2C_EmitJCmpVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, flip, noflip, doptrshl;
	int csreg2, ctreg2;
//	s32 imm;
	s64 imm;
	int nm1, nm2, nm3;
	
	noflip=0;
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
//		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
		if(((imm&1023)==imm) || ((imm|(~1023))==imm) || sctx->has_jumbo)
			noflip=1;
	}
	
	if(BGBCC_CCXL_IsRegImmLongP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
//		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
		if(((imm&1023)==imm) || ((imm|(~1023))==imm) || sctx->has_jumbo)
			noflip=1;
	}
	
	doptrshl=0;
	if(sctx->abi_spillpad&4)
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
		{
			doptrshl=1;
		}
	}
	
	nm3=-1; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
//		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
//		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
//				nm3=BGBCC_SH_NMID_BRLT;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
//			nm3=BGBCC_SH_NMID_BRGT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
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
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(nm1==BGBCC_SH_NMID_CMPQEQ)
			nm1=BGBCC_SH_NMID_CMPPEQ;
		if(nm1==BGBCC_SH_NMID_CMPQGT)
			nm1=BGBCC_SH_NMID_CMPPGT;
		if(nm1==BGBCC_SH_NMID_CMPQGE)
			nm1=BGBCC_SH_NMID_CMPPGE;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
//		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) && !flip)
		if((BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg)) && !flip)
		{
//			imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
			imm=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);

			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
				BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
				BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}else
		{
			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);

			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}

#if 0
		if(nm3>=0)
		{
			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

//			BGBCC_JX2_EmitOpCmpRegRegLabel(sctx, nm3, ctreg, csreg, lbl);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

		csreg2=csreg;
		ctreg2=ctreg;
		if(doptrshl)
		{
			csreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, csreg, 16, csreg2);
//			BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
//				BGBCC_SH_NMID_SHLDQ, ctreg, 16, ctreg2);

			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, csreg, csreg2);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOVZT, ctreg, ctreg2);
		}

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg2, ctreg2);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg2, csreg2);

		if(doptrshl)
		{
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg2);
		}

		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegZeroQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	int csreg, ctreg, doptrshl;
	int nm1, nm2, nm3;
	int i, j, k;

	doptrshl=0;
	if(sctx->abi_spillpad&4)
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
		{
			doptrshl=1;
		}
	}

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(cmp==CCXL_CMP_LT)
		{
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_GE)
		{
			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_LE)
			{ cmp=CCXL_CMP_EQ; }
		if(cmp==CCXL_CMP_GT)
			{ cmp=CCXL_CMP_NE; }
	}
	
	nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
//		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
//		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;

#if 1
	case CCXL_CMP_LT:
//		nm1=BGBCC_SH_NMID_CMPPZ;
		nm1=BGBCC_SH_NMID_CMPQGE;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLT;
		break;
	case CCXL_CMP_GT:
//		nm1=BGBCC_SH_NMID_CMPPL;
		nm1=BGBCC_SH_NMID_CMPQGT;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGT;
		break;
	case CCXL_CMP_LE:
//		nm1=BGBCC_SH_NMID_CMPPL;
		nm1=BGBCC_SH_NMID_CMPQGT;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLE;
		break;
	case CCXL_CMP_GE:
//		nm1=BGBCC_SH_NMID_CMPPZ;
		nm1=BGBCC_SH_NMID_CMPQGE;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGE;
		break;
#endif

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if(doptrshl)
		nm1=-1;
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);

#if 0
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1))
		{
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2C_SetModeDqClear(ctx, sctx);

//			BGBCC_JX2_EmitOpCmpRegLabel(sctx, nm3, csreg, lbl);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

		if((nm3>=0) && (sctx->has_jcmp&1) &&
			BGBCC_JX2_EmitCheckRegExtGPR(sctx, csreg))
		{
			j=BGBCC_JX2_TryEmitOpRegLbl(sctx, nm3, lbl, csreg);
			if(j>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_JX2_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(1)
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_JX2C_EmitJCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
//	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}


int BGBCC_JX2C_EmitPredCmpVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp)
{
	int csreg, ctreg, flip, noflip;
	s32 imm;
	int nm1, nm2, nm3;
	
	noflip=0;
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			noflip=1;
	}
	
	nm3=-1; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) && !flip)
		{
			imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
				sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//				BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//				BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}else
		{
			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);

			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);
		}

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitPredCmpVRegZeroQLong(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp)
{
	ccxl_register treg;
	int csreg, ctreg;
	int nm1, nm2, nm3;

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(cmp==CCXL_CMP_LT)
		{
			sctx->pred_tfmd=0;
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_GE)
		{
			sctx->pred_tfmd=1;
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_LE)
			{ cmp=CCXL_CMP_EQ; }
		if(cmp==CCXL_CMP_GT)
			{ cmp=CCXL_CMP_NE; }
	}
	
	nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);

		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);

		BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, csreg);

		sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(1)
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_JX2C_EmitPredCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitCSeltSelectVRegVRegVRegQLong(
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

int BGBCC_JX2C_EmitCSeltCompareVRegVRegQLong(
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
	
	flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPQEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BF;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPQHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPQGT;
			nm2=BGBCC_SH_NMID_BF;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPQHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPQGE;
				nm2=BGBCC_SH_NMID_BT;
			}
		}
		break;
	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TSTQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TSTQ;
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

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

		if(nm2==BGBCC_SH_NMID_BF)
			BGBCC_JX2C_EmitOpNone(ctx, sctx, BGBCC_SH_NMID_NOTT);
		
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
