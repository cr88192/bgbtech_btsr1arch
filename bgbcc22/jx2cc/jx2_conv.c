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

int BGBCC_JX2C_EmitConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dreg, ccxl_register sreg)
{
	ccxl_type dty2, sty2;
	s64 li;
	int csreg, ctreg, cdreg;
	int tr0, tr1, tr2, tr3;
	int nm1, nm2, tg0, tg1;
	int dt, st;
	int i, j, k;

//	dt=dtype.val;
//	st=stype.val;

//	BGBCC_CCXL_TypeCanonizeOverflow(ctx, &dty2, dtype);
//	BGBCC_CCXL_TypeCanonizeOverflow(ctx, &sty2, stype);
//	dt=dty2.val;
//	st=sty2.val;

	dt=BGBCC_CCXL_TypeAsCanonical(ctx, dtype);
	st=BGBCC_CCXL_TypeAsCanonical(ctx, stype);

	if(dt==st)
	{
		return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
			dtype, dreg, sreg));
	}

	if(BGBCC_CCXL_TypeSquareArrayPtrP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeSquareArrayP(ctx, stype))
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

			if(ctx->arch_sizeof_ptr==8)
			{
				BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, csreg, 8, cdreg);
			}else
			{
				BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVL, csreg, 4, cdreg);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeVarRefP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeVarRefP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(	BGBCC_CCXL_IsRegImmIntP(ctx, sreg)		||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg)		||
			BGBCC_CCXL_IsRegImmFloatP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(
				ctx, sctx, dtype, dreg, sreg));
		}

		if(	BGBCC_CCXL_IsRegImmStringP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmWStringP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmW4StringP(ctx, sreg)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(
				ctx, sctx, dtype, dreg, sreg));
		}

		if((BGBCC_CCXL_TypeRefStringP(ctx, dtype) ||
			BGBCC_CCXL_TypeVariantP(ctx, dtype) )
			&&
			(BGBCC_CCXL_TypeCStringP(ctx, stype) ||
			BGBCC_CCXL_TypeCWStringP(ctx, stype)))
		{
			tg0=CCXL_LVA_LookupTagIndexForName(ctx, "_String");

			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, tr1, 0x0000FFFFFFFFFFFFULL);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_AND, tr1, tr0);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, tr1, ((u64)tg0)<<48);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR, tr1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitConvFromVRegVRegVariant(ctx, sctx,
				stype, dreg, sreg));
		}
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
#if 1
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			
			if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, cdreg))
			{
				tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, cdreg);
				BGBCC_JX2_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_MOV, csreg, tr0+1);
				if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
				{
					BGBCC_JX2_EmitOpImmReg(sctx,
						BGBCC_SH_NMID_MOV, 0, tr0+0);
				}else
				{
					BGBCC_JX2_EmitOpRegImmReg(sctx,
						BGBCC_SH_NMID_SHADQ, csreg, -63, tr0+0);
				}
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_MOVTT, tr0, 0x08, tr0);
			}else
			{
				BGBCC_JX2_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_CONVFXI, csreg, cdreg);
			}
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
#endif
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, stype) ||
			BGBCC_CCXL_TypeDoubleP(ctx, stype))
		{
#if 1
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, cdreg))
			{
				tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, cdreg);
				BGBCC_JX2_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_MOV, csreg, tr0+1);
				BGBCC_JX2_EmitOpImmReg(sctx,
					BGBCC_SH_NMID_MOV, 0x5000000000000000ULL, tr0+0);
			}else
			{
				BGBCC_JX2_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_CONVFLI, csreg, cdreg);
			}
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
#endif
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitConvFromVRegVRegVariant(ctx, sctx,
				stype, dreg, sreg));
		}
	}
	
	if(BGBCC_CCXL_TypeSquareArrayP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeVarRefP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
	}

	if(	BGBCC_CCXL_TypePointerP(ctx, dtype) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeArrayP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

//		if(BGBCC_CCXL_TypeRefArrayP(ctx, stype))
//			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
//				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeSmallIntP(ctx, stype))
		{
			if(sctx->is_addr64 && (st==CCXL_TY_UI))
			{
				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRSIZEDIFF);
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					BGBCC_SH_NMID_EXTUL, dreg, sreg));
			}

			if(sctx->is_addr64)
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}

			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			if(sctx->is_addr64)
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}

			BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRSIZEDIFF);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, 
				BGBCC_JX2CC_PSREG_RQARG(0));
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeValueObjectP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeVariantP(ctx, stype))
		{
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_toptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}else
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_toptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_RQRET);
				return(1);
			}
		}

		if(BGBCC_CCXL_TypeRefArrayP(ctx, stype))
		{
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvox_arraygetptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}else
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvo_arraygetptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_RQRET);
				return(1);
			}
		}

		if(BGBCC_CCXL_TypeVarRefP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(	BGBCC_CCXL_TypePointerP(ctx, dtype) &&
		BGBCC_CCXL_TypeQuadPointerP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, stype) &&
			BGBCC_CCXL_TypeQuadPointerP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, stype) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, stype))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_GBH, tr0+1);

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);

//			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
//				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_IsRegImmInt128P(ctx, sreg))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_IsRegImmILP(ctx, sreg))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2_EmitOpImmReg(sctx,
				BGBCC_SH_NMID_MOV, 0, tr0+1);

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2_EmitOpImmReg(sctx,
				BGBCC_SH_NMID_MOV, 0, tr0+1);

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if(BGBCC_CCXL_TypeSgInt128P(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(ctx->arch_sizeof_ptr==16)
		{
			if(BGBCC_CCXL_TypeFunctionP(ctx, stype))
				{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg)); }

			if(BGBCC_CCXL_TypeValueObjectP(ctx, stype))
				{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg)); }

			if(BGBCC_CCXL_TypeVariantP(ctx, stype))
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvax_conv_toptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}

			if(BGBCC_CCXL_TypeRefArrayP(ctx, stype))
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvox_arraygetptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}

			if(BGBCC_CCXL_TypeVarRefP(ctx, stype))
				{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg)); }
		}else
		{
			if(	BGBCC_CCXL_TypeFunctionP(ctx, stype))
			{
				tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
					BGBCC_SH_REGCLS_QGR2);
				tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PCH, tr0+1);

				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
				return(1);
			}

			if(	BGBCC_CCXL_TypeValueObjectP(ctx, stype) ||
				BGBCC_CCXL_TypeVarRefP(ctx, stype))
			{
				tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
					BGBCC_SH_REGCLS_QGR2);
				tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBH, tr0+1);

				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
				return(1);
			}

			if(	BGBCC_CCXL_TypeVariantP(ctx, stype))
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lva_conv_toptr");
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBH, BGBCC_SH_REG_R3);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}

			if(BGBCC_CCXL_TypeRefArrayP(ctx, stype))
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvo_arraygetptr");
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBH, BGBCC_SH_REG_R3);
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}

		}


		BGBCC_CCXL_StubError(ctx);
		return(0);
	}


	if(BGBCC_CCXL_TypeArrayP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeArrayP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeRefArrayP(ctx, stype))
		{
//			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
//				dtype, dreg, sreg));
			if(ctx->arch_sizeof_ptr==16)
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvox_arraygetptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_LRRET);
				return(1);
			}else
			{
				BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
				BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
				BGBCC_JX2C_EmitCallName(ctx, sctx, "__lvo_arraygetptr");
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					dreg, BGBCC_JX2CC_PSREG_RQRET);
				return(1);
			}
		}


		if(BGBCC_CCXL_TypeSmallIntP(ctx, stype))
		{
			if(sctx->is_addr64 && (st==CCXL_TY_UI))
			{
				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRSIZEDIFF);
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					BGBCC_SH_NMID_EXTUL, dreg, sreg));
			}

			if(sctx->is_addr64)
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}

			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			if(sctx->is_addr64)
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeBitIntMaxP(ctx, dtype, 64))
		{
			if(BGBCC_CCXL_TypeBitIntMaxP(ctx, stype, 64))
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}else
				if(BGBCC_CCXL_TypeBitIntMaxP(ctx, stype, 128))
			{
				dty2=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UL);
				sty2=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128);
				return(BGBCC_JX2C_EmitConvVRegVReg(ctx, sctx,
					dty2, sty2, dreg, sreg));
			}else
				if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}
		}else
			if(BGBCC_CCXL_TypeBitIntMaxP(ctx, dtype, 128))
		{
			if(BGBCC_CCXL_TypeBitIntMaxP(ctx, stype, 64))
			{
				dty2=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128);
				sty2=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UL);
				return(BGBCC_JX2C_EmitConvVRegVReg(ctx, sctx,
					dty2, sty2, dreg, sreg));
			}else
				if(BGBCC_CCXL_TypeBitIntMaxP(ctx, stype, 128))
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}
		}
	
		if(BGBCC_CCXL_TypeSmallBitIntP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_BIGINT)
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitConvFromVRegVRegVariant2(
				ctx, sctx, dtype, stype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeVarRefP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(	(dt==CCXL_TY_I) ||
		((dt==CCXL_TY_NL) && (ctx->arch_sizeof_long==4)))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, dtype, dreg, (s32)i);
			return(1);
		}

		if(st==CCXL_TY_SB)
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));	}
		if(st==CCXL_TY_UB)
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));	}
		if(st==CCXL_TY_SS)
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSW, dreg, sreg));	}
		if(st==CCXL_TY_US)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));
		}

		if((st==CCXL_TY_I) ||
			((st==CCXL_TY_NL) && (ctx->arch_sizeof_long==4)))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, dtype,
				dreg, sreg));
		}
		if((st==CCXL_TY_UI) ||
			((st==CCXL_TY_UNL) && (ctx->arch_sizeof_long==4)))
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSL, dreg, sreg));
		}

		if(st==CCXL_TY_M32)
		{
			nm1=BGBCC_SH_NMID_EXTUL;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if((st==CCXL_TY_NL)		|| (st==CCXL_TY_UNL)	||
			(st==CCXL_TY_L)		|| (st==CCXL_TY_UL)		||
			(st==CCXL_TY_SI48)	|| (st==CCXL_TY_UI48)	)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSL, dreg, sreg));
		}

#if 1
		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_R0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
#endif

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
				!sctx->is_addr64)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
			sctx->is_addr64)
		{
			if(!sctx->is_addr_x32)
				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRRANGELOSS);

			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSL, dreg, sreg));

#if 0
			nm1=BGBCC_SH_NMID_EXTSL;
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
//			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToDgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpReg(sctx, nm1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
#endif
		}

		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_UI) ||
		((dt==CCXL_TY_UNL) && (ctx->arch_sizeof_long==4)))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, dtype, dreg, (u32)i);
			return(1);
		}

		if(st==CCXL_TY_SB)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUL, dreg, dreg));
		}
		if(st==CCXL_TY_UB)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));
		}
		if(st==CCXL_TY_SS)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSW, dreg, sreg));
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUL, dreg, dreg));
		}
		if(st==CCXL_TY_US)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));
		}
		if(st==CCXL_TY_I)
		{ 
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUL, dreg, sreg));
		}
		if(st==CCXL_TY_UI)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_NL) && (ctx->arch_sizeof_long==4))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		if((st==CCXL_TY_NL) && (ctx->arch_sizeof_long==4))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(st==CCXL_TY_M32)
		{
			nm1=BGBCC_SH_NMID_EXTUL;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

//		if(st==CCXL_TY_D)
		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if((st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_SI48) || (st==CCXL_TY_UI48))
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUL, dreg, sreg));
		}

#if 1
		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_R0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
#endif

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
			!sctx->is_addr64)
		{
			if(!sctx->is_addr_x32)
				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRRANGELOSS);

			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
			sctx->is_addr64)
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUL, dreg, sreg));

#if 0
			nm1=BGBCC_SH_NMID_EXTUL;
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
//			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToDgr(ctx, sctx, tr1);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpReg(sctx, nm1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
#endif
		}

		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_SB) || (dt==CCXL_TY_UB))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			if(dt==CCXL_TY_SB)		i=(sbyte)i;
			if(dt==CCXL_TY_UB)		i=(byte)i;
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, dtype, dreg, i);
			return(1);
		}
	
		nm1=BGBCC_SH_NMID_EXTSB;
		if(dt==CCXL_TY_UB)
			nm1=BGBCC_SH_NMID_EXTUB;

		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) && !sctx->is_addr64) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(	(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_SI48) || (st==CCXL_TY_UI48))
		{
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype))
		{
			BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRRANGELOSS);
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_R0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_SS) || (dt==CCXL_TY_US))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			if(dt==CCXL_TY_SS)		i=(s16)i;
			if(dt==CCXL_TY_US)		i=(u16)i;
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, dtype, dreg, i);
			return(1);
		}
	
		if(st==CCXL_TY_SB)
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));	}
		if(st==CCXL_TY_UB)
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));	}

		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) && !sctx->is_addr64) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(st==CCXL_TY_M16)
		{
			nm1=BGBCC_SH_NMID_EXTUW;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(	(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_SI48) || (st==CCXL_TY_UI48))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype))
		{
			BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRRANGELOSS);
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

#if 1
		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_R0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
#endif

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(	(dt==CCXL_TY_I) || (dt==CCXL_TY_UI) ||
		(dt==CCXL_TY_L) || (dt==CCXL_TY_UL) ||
		(dt==CCXL_TY_NL) || (dt==CCXL_TY_UNL) ||
		(dt==CCXL_TY_SB) || (dt==CCXL_TY_UB) ||
		(dt==CCXL_TY_SS) || (dt==CCXL_TY_US) ||
		(dt==CCXL_TY_SI48) || (dt==CCXL_TY_UI48) )
	{
		nm1=-1;
		if(dt==CCXL_TY_UB)	nm1=BGBCC_SH_NMID_EXTUB;
		if(dt==CCXL_TY_SB)	nm1=BGBCC_SH_NMID_EXTSB;
		if(dt==CCXL_TY_US)	nm1=BGBCC_SH_NMID_EXTUW;
		if(dt==CCXL_TY_SS)	nm1=BGBCC_SH_NMID_EXTSW;
		if(dt==CCXL_TY_UI)	nm1=BGBCC_SH_NMID_EXTUL;
		if(dt==CCXL_TY_I)	nm1=BGBCC_SH_NMID_EXTSL;

//		if(dt==CCXL_TY_NL)	nm1=BGBCC_SH_NMID_EXTSL;
//		if(dt==CCXL_TY_UNL)	nm1=BGBCC_SH_NMID_EXTUL;
//		if(dt==CCXL_TY_L)	nm1=BGBCC_SH_NMID_MOV;
//		if(dt==CCXL_TY_UL)	nm1=BGBCC_SH_NMID_MOV;

		if(st==CCXL_TY_BCD64)
		{
			BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__bcd64_toint");
			if(nm1>=0)
				BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_JX2CC_PSREG_RQRET);			
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_JX2CC_PSREG_RQRET);
			return(1);
		}

		if(st==CCXL_TY_BCD128)
		{
			BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__bcd128_toint");
			if(nm1>=0)
				BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_JX2CC_PSREG_RQRET);			
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_JX2CC_PSREG_RQRET);
			return(1);
		}

		if(BGBCC_CCXL_TypeBitFieldP(ctx, stype))
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			
			j=BGBCC_CCXL_TypeBitFieldGetBits(ctx, stype);
			k=BGBCC_CCXL_TypeBitFieldGetOffset(ctx, stype);
			
			if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHLDQ, tr0, 64-(j+k), tr0);
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHLDQ, tr0, -(64-j), tr0);
			}else
			{
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, tr0, 64-(j+k), tr0);
				BGBCC_JX2_EmitOpRegImmReg(sctx,
					BGBCC_SH_NMID_SHADQ, tr0, -(64-j), tr0);
			}
			
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}
	}

#if 1
	if(	(dt==CCXL_TY_L) || (dt==CCXL_TY_UL) ||
		(dt==CCXL_TY_NL) || (dt==CCXL_TY_UNL) ||
		(dt==CCXL_TY_SI48) || (dt==CCXL_TY_UI48))
	{
		if(	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmUIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, dtype, dreg, li);
			return(1);
		}

		nm1=-1;
		if(st==CCXL_TY_UB)	nm1=BGBCC_SH_NMID_EXTUB;
		if(st==CCXL_TY_SB)	nm1=BGBCC_SH_NMID_EXTSB;
		if(st==CCXL_TY_US)	nm1=BGBCC_SH_NMID_EXTUW;
		if(st==CCXL_TY_SS)	nm1=BGBCC_SH_NMID_EXTSW;
		if(st==CCXL_TY_UI)	nm1=BGBCC_SH_NMID_EXTUL;
		if(st==CCXL_TY_I)	nm1=BGBCC_SH_NMID_EXTSL;

		if(st==CCXL_TY_NL)		nm1=BGBCC_SH_NMID_EXTSL;
		if(st==CCXL_TY_UNL)		nm1=BGBCC_SH_NMID_EXTUL;
		if(st==CCXL_TY_L)		nm1=BGBCC_SH_NMID_MOV;
		if(st==CCXL_TY_UL)		nm1=BGBCC_SH_NMID_MOV;
		if(st==CCXL_TY_SI48)	nm1=BGBCC_SH_NMID_MOV;
		if(st==CCXL_TY_UI48)	nm1=BGBCC_SH_NMID_MOV;
		
		if(ctx->arch_sizeof_long==8)
		{
			if(st==CCXL_TY_NL)	nm1=BGBCC_SH_NMID_MOV;
			if(st==CCXL_TY_UNL)	nm1=BGBCC_SH_NMID_MOV;
		}else
			if(ctx->arch_sizeof_long==4)
		{
			if((st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
				(st==CCXL_TY_NL) || (st==CCXL_TY_UNL))
			{
				if(dt==CCXL_TY_NL)	nm1=BGBCC_SH_NMID_EXTSL;
				if(dt==CCXL_TY_UNL)	nm1=BGBCC_SH_NMID_EXTUL;
			}
		}
	
		if(nm1>0)
		{
			if(nm1==BGBCC_SH_NMID_MOV)
			{
				return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
					dtype, dreg, sreg));
			}

			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

#if 1
		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_RQ0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
#endif

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
//			BGBCC_CCXL_TypeArrayP(ctx, stype))

		if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, stype) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

#if 1
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, stype))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_RQ0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
#endif

		if(	(st==CCXL_TY_M64)		||	(st==CCXL_TY_VEC2F)		||
			(st==CCXL_TY_VEC4SW)	||	(st==CCXL_TY_VEC4UW)	||
			(st==CCXL_TY_VEC4H)		||	(st==CCXL_TY_VEC3FQ)	||
			(st==CCXL_TY_VEC3H)		||	(st==CCXL_TY_BCD64)		||
			(st==CCXL_TY_VEC3H_48)	||	 
			(st==CCXL_TY_VEC4_FP8S)	||	(st==CCXL_TY_VEC4_FP8U)	||
			(st==CCXL_TY_VEC4_FP8A)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

	if((dt==CCXL_TY_F) ||
		(dt==CCXL_TY_F16) ||
		(dt==CCXL_TY_BF16) ||
		(dt==CCXL_TY_FP8S) ||
		(dt==CCXL_TY_FP8U))
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) && !sctx->is_addr64) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB) ||
			(st==CCXL_TY_SI48) || (st==CCXL_TY_UI48))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FCNVDS, BGBCC_SH_NMID_FSTS, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_M16)
		{
			nm1=BGBCC_SH_NMID_FLDCH;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(st==CCXL_TY_M32)
		{
			nm1=BGBCC_SH_NMID_FLDCF;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) ||
			((st==CCXL_TY_SI48) || (st==CCXL_TY_UI48)))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_D) || (dt==CCXL_TY_F48))
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) && !sctx->is_addr64) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB) )
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if((st==CCXL_TY_F) ||
			(st==CCXL_TY_F16) ||
			(st==CCXL_TY_BF16) ||
			(st==CCXL_TY_FP8S) ||
			(st==CCXL_TY_FP8U))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FLDS, BGBCC_SH_NMID_FCNVSD, dreg, sreg));
		}

		if((st==CCXL_TY_D) || (st==CCXL_TY_F48))
		{
			return(1);
		}

		if(st==CCXL_TY_F128)
		{
			return(
				BGBCC_JX2C_EmitConvVRegVRegFromFloat128(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) ||
			((st==CCXL_TY_SI48) || (st==CCXL_TY_UI48)))
		{
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if(st==CCXL_TY_M64)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_I128) || (dt==CCXL_TY_UI128))
	{
		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_M128)		||	(st==CCXL_TY_M128P)		||
			(st==CCXL_TY_VEC4SI)	||	(st==CCXL_TY_VEC4UI)	||
			(st==CCXL_TY_VEC3F)		||	(st==CCXL_TY_VEC4F))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		
		if(	BGBCC_CCXL_TypePointerP(ctx, stype) &&
			BGBCC_CCXL_TypeQuadPointerP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	BGBCC_CCXL_TypePointerP(ctx, stype) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, stype))
		{
//			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
//				dtype, dreg, sreg));
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
//			if(sctx->has_xgpr&2)
			if((sctx->has_xgpr&2) && (ctx->arch_sizeof_ptr==16))
			{
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, BGBCC_SH_REG_GBH, tr0+1);
			}else
			{
				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, 0, tr0+1);
			}

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

#if 1
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SI48) || (st==CCXL_TY_UI48))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
//			tr0=BGBCC_SH_REG_RQ0|(tr1&31);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);
			if(BGBCC_CCXL_TypeSmallIntP(ctx, stype))
			{
//				tr0=BGBCC_SH_REG_RD0|(tr1&31);
				tr0=BGBCC_JX2C_MapLpRegToDgr(ctx, sctx, tr1);
			}

			tr2=tr0+1;

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);

			if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
			{
				BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
					BGBCC_SH_NMID_MOV, 0, tr2);
			}else
			{
				if(BGBCC_CCXL_TypeSmallIntP(ctx, stype))
				{
					BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
						BGBCC_SH_NMID_SHAD, tr0, -31, tr2);
				}else
				{
					BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
						BGBCC_SH_NMID_SHADQ, tr0, -63, tr2);
				}
			}

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
#endif

		if(BGBCC_CCXL_TypeBitIntP(ctx, stype))
		{
			BGBCC_JX2C_EmitConvFromVRegVRegBitInt(ctx, sctx,
				dtype, stype, dreg, sreg);
			return(1);
		}

		if(st==CCXL_TY_BCD128)
		{
			BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__bcd128_toint128");
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_M64)
	{
		if(	(st==CCXL_TY_M64)		||	(st==CCXL_TY_VEC2F)		||
			(st==CCXL_TY_VEC4SW)	||	(st==CCXL_TY_VEC4UW)	||
			(st==CCXL_TY_D)			||	(st==CCXL_TY_L)			||
			(st==CCXL_TY_UL)		||	(st==CCXL_TY_VEC4H)		||
			(st==CCXL_TY_VEC3FQ)	||	(st==CCXL_TY_VEC3H)		||
			(st==CCXL_TY_BCD64)		||
			(st==CCXL_TY_SI48)		||	(st==CCXL_TY_UI48)		||
			(st==CCXL_TY_F48)		||	(st==CCXL_TY_VEC3H_48)	||
			(st==CCXL_TY_VEC4_FP8S)	||	(st==CCXL_TY_VEC4_FP8U)	||
			(st==CCXL_TY_VEC4_FP8A)
			)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_M128) || (dt==CCXL_TY_M128P))
	{
		if(	(st==CCXL_TY_I128) ||
			(st==CCXL_TY_UI128) ||
			(st==CCXL_TY_F128))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_M128)		||	(st==CCXL_TY_M128P)		||
			(st==CCXL_TY_VEC4SI)	||	(st==CCXL_TY_VEC4UI)	||
			(st==CCXL_TY_VEC3F)		||	(st==CCXL_TY_VEC4F)		||
			(st==CCXL_TY_QUATF)		||	(st==CCXL_TY_VEC2D)		||
			(st==CCXL_TY_BCD128)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_M32)
	{
		if(	(st==CCXL_TY_M32)		||
			(st==CCXL_TY_I)	||	(st==CCXL_TY_UI)
			)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_VEC4_FP8S)	||	(st==CCXL_TY_VEC4_FP8U)	||
			(st==CCXL_TY_VEC4_FP8A)	)
		{
			nm1=BGBCC_SH_NMID_PCVTHTOF8;
			if(st==CCXL_TY_VEC4_FP8U)
				nm1=BGBCC_SH_NMID_PSTCM8UH;
			if(st==CCXL_TY_VEC4_FP8A)
				nm1=BGBCC_SH_NMID_PCVTH2F8A;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if((st==CCXL_TY_F) || (st==CCXL_TY_F16) || (st==CCXL_TY_D))
		{
			nm1=BGBCC_SH_NMID_FSTCF;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_M16)
	{
		if(	(st==CCXL_TY_M16)		||
			(st==CCXL_TY_SS)	||	(st==CCXL_TY_US)
			)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_F16) || (st==CCXL_TY_F) || (st==CCXL_TY_D))
		{
			nm1=BGBCC_SH_NMID_FSTCH;
			return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_VEC2F)
	{
		if(	(st==CCXL_TY_M64)		||
			(st==CCXL_TY_VEC2F)		||
			(st==CCXL_TY_FCOMPLEX)	||
			(st==CCXL_TY_L)			||
			(st==CCXL_TY_UL)		)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) &&
			(ctx->arch_sizeof_long==8))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_VEC2D)
	{
		if(	(st==CCXL_TY_M128)		||
			(st==CCXL_TY_M128P)		||
			(st==CCXL_TY_VEC2D)		||
			(st==CCXL_TY_DCOMPLEX)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_FCOMPLEX)
	{
		if(	(st==CCXL_TY_M64)		||
			(st==CCXL_TY_VEC2F)		||
			(st==CCXL_TY_FCOMPLEX)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(	(dt==CCXL_TY_DCOMPLEX)	||
		(dt==CCXL_TY_VEC2D)		)
	{
		if(	(st==CCXL_TY_M128)		||
			(st==CCXL_TY_M128P)		||
			(st==CCXL_TY_VEC2D)		||
			(st==CCXL_TY_DCOMPLEX)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);

			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCI,
				tr0, tr0);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
				0, tr0+1);

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);

//			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
//				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_F) || (st==CCXL_TY_D) || (st==CCXL_TY_F16))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
			tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, tr1);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);

//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLDCI,
//				tr0, tr0);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV,
				0, tr0+1);

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);

//			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
//				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(	(dt==CCXL_TY_VEC4SW) ||
		(dt==CCXL_TY_VEC4UW))
	{
		if(	(st==CCXL_TY_M64)		||
			(st==CCXL_TY_VEC4SW)	||
			(st==CCXL_TY_VEC4UW)	||
			(st==CCXL_TY_L)			||
			(st==CCXL_TY_UL)		)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_BCD64)
	{
		if(	(st==CCXL_TY_M64)		||
			(st==CCXL_TY_BCD64)		)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) )
		{
			BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__bcd64_fromint");
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_JX2CC_PSREG_RQRET);
			return(1);
		}
	}

	if(dt==CCXL_TY_BCD128)
	{
		if(	(st==CCXL_TY_M128)		||
			(st==CCXL_TY_BCD128)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) )
		{
			BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_ARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__bcd128_fromint");
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}

		if(	(st==CCXL_TY_I128)		||
			(st==CCXL_TY_UI128)	)
		{
			BGBCC_JX2C_EmitSyncLeafRegisters(ctx, sctx);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_JX2CC_PSREG_LRARG(0));
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__bcd128_fromint128");
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
				dreg, BGBCC_JX2CC_PSREG_LRRET);
			return(1);
		}
	}

	if(	(dt==CCXL_TY_VEC4H) || (dt==CCXL_TY_VEC3H) ||
		(dt==CCXL_TY_VEC3H_48) ||
		(dt==CCXL_TY_VEC4_FP8S)	||	(dt==CCXL_TY_VEC4_FP8U)	||
		(dt==CCXL_TY_VEC4_FP8A) )
	{
		if(	(st==CCXL_TY_M64)		||
			(st==CCXL_TY_VEC4SW)	||
			(st==CCXL_TY_VEC4UW)	||
			(st==CCXL_TY_VEC4H)		||
			(st==CCXL_TY_VEC3H)		||
			(st==CCXL_TY_VEC3H_48)	||
			(st==CCXL_TY_L)			||
			(st==CCXL_TY_UL)		||
			(st==CCXL_TY_VEC4_FP8S)	||
			(st==CCXL_TY_VEC4_FP8U)	||
			(st==CCXL_TY_VEC4_FP8A)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(st==CCXL_TY_M32)
		{
			nm1=-1;
			if(dt==CCXL_TY_VEC4_FP8S)
				nm1=BGBCC_SH_NMID_PCVTF8TOH;
			if(dt==CCXL_TY_VEC4_FP8U)
				nm1=BGBCC_SH_NMID_PLDCM8UH;
			if(dt==CCXL_TY_VEC4_FP8A)
				nm1=BGBCC_SH_NMID_PCVTF8A2H;
			if(nm1>0)
			{
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					nm1, dreg, sreg));
			}
		}


		if(st==CCXL_TY_VEC3FQ)
		{
			return(BGBCC_JX2C_EmitConvVRegVRegVec(ctx, sctx,
				dtype, stype, dreg, sreg));
		}

		if(	(st==CCXL_TY_VEC3F) ||
			(st==CCXL_TY_VEC4F)	)
		{
			return(BGBCC_JX2C_EmitConvVRegVRegVec(ctx, sctx,
				dtype, stype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_VEC4SI) || (dt==CCXL_TY_VEC4UI))
	{
		if(	(st==CCXL_TY_M128)		||
			(st==CCXL_TY_M128P)		||
			(dt==CCXL_TY_VEC4SI)	||
			(dt==CCXL_TY_VEC4UI)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	if(dt==CCXL_TY_VEC3F)
	{
		if((st==CCXL_TY_M128) || (st==CCXL_TY_M128P) ||
			(st==CCXL_TY_VEC3F))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		
		if(st==CCXL_TY_VEC4F)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		
		if(st==CCXL_TY_VEC3FQ)
		{
			return(BGBCC_JX2C_EmitConvVRegVRegVec(ctx, sctx,
				dtype, stype, dreg, sreg));
		}
		
		if(	(st==CCXL_TY_VEC4H) || (st==CCXL_TY_VEC3H) ||
			(st==CCXL_TY_VEC3H_48) ||
			(st==CCXL_TY_VEC4_FP8S)	||	(st==CCXL_TY_VEC4_FP8U)	||
			(st==CCXL_TY_VEC4_FP8A)	)
		{
			return(BGBCC_JX2C_EmitConvVRegVRegVec(ctx, sctx,
				dtype, stype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	if(dt==CCXL_TY_VEC4F)
	{
		if(	(st==CCXL_TY_M128)	||
			(st==CCXL_TY_M128P)	||
			(st==CCXL_TY_VEC4F)	||
			(st==CCXL_TY_QUATF)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VEC3F)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_VEC4H) || (st==CCXL_TY_VEC3H) ||
			(st==CCXL_TY_VEC3H_48) ||
			(st==CCXL_TY_VEC4_FP8S)	||
			(st==CCXL_TY_VEC4_FP8U)	||
			(st==CCXL_TY_VEC4_FP8A)	)
		{
			return(BGBCC_JX2C_EmitConvVRegVRegVec(ctx, sctx,
				dtype, stype, dreg, sreg));
		}

		if(st==CCXL_TY_VEC3FQ)
		{
			return(BGBCC_JX2C_EmitConvVRegVRegVec(ctx, sctx,
				dtype, stype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	if(dt==CCXL_TY_QUATF)
	{
		if(	(st==CCXL_TY_M128)	||
			(st==CCXL_TY_M128P)	||
			(st==CCXL_TY_QUATF)	||
			(st==CCXL_TY_VEC4F)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}


	if(dt==CCXL_TY_F128)
	{
		if(	(st==CCXL_TY_M128)	||
			(st==CCXL_TY_M128P)	)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
	
		return(
			BGBCC_JX2C_EmitConvVRegVRegToFloat128(
				ctx, sctx,
				stype, dreg, sreg));
	}

	if((dt==CCXL_TY_VARIANT) ||
		(dt==CCXL_TY_VAROBJECT))
	{
		if((st==CCXL_TY_VARIANT) ||
			(st==CCXL_TY_VAROBJECT))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		
		if(BGBCC_CCXL_TypeVarRefP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		
		return(
			BGBCC_JX2C_EmitConvFromVRegVRegVariant(ctx, sctx,
				stype, dreg, sreg));
	}
	
	if(dt==CCXL_TY_VARSTRING)
	{
		return(
			BGBCC_JX2C_EmitConvFromVRegVRegVarString(ctx, sctx,
				stype, dreg, sreg));
	}

	if(BGBCC_CCXL_TypeBitFieldP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				stype, dreg, sreg));
		}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
