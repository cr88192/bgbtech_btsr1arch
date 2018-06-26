int BGBCC_JX2C_EmitConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dreg, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	int tr0, tr1;
	int nm1, nm2;
	int dt, st;
	int i, j, k;

	dt=dtype.val;
	st=stype.val;

	if(dt==st)
	{
		return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
			dtype, dreg, sreg));
	}

	if(BGBCC_CCXL_TypePointerP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeArrayP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

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
//				BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRSIZEDIFF);
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
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeValueObjectP(ctx, stype))
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

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

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_I) ||
		((dt==CCXL_TY_NL) && !sctx->is_addr64))
	{
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
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));	}
		if(st==CCXL_TY_I)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, dtype,
				dreg, sreg)); }
		if(st==CCXL_TY_UI)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, dtype,
				dreg, sreg)); }

		if((st==CCXL_TY_NL) && !sctx->is_addr64)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if((st==CCXL_TY_UNL) && !sctx->is_addr64)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL)) &&
			sctx->is_addr64)
		{
			if(sctx->has_bjx1egpr)
			{
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					BGBCC_SH_NMID_EXTSL, dreg, sreg));
			}

#if 0
			if(sctx->has_bjx1egpr)
			{
				csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

				if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
					csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
					cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

				nm1=BGBCC_SH_NMID_EXTSL;
				BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
#endif

#if 1
			nm1=BGBCC_SH_NMID_EXTSL;
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpReg(sctx, nm1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
#endif
		}

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			return(BGBCC_JX2C_ConvLpRegisterNarrowing(ctx, sctx,
				BGBCC_SH_NMID_MOV, sreg, dreg));

#if 0
			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//			nm1=BGBCC_SH_NMID_EXTSL;
			nm1=BGBCC_SH_NMID_MOV;
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
#endif

#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
#endif
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R5);
			BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R5, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

//		if(st==CCXL_TY_L)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_NL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_UL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_UNL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
				!sctx->is_addr64)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }


		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
			sctx->is_addr64)
		{
			nm1=BGBCC_SH_NMID_EXTSL;
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpReg(sctx, nm1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_UI) ||
		((dt==CCXL_TY_UNL) && !sctx->is_addr64))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, dtype, dreg, (u32)i);
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
		{	return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));	}
		if(st==CCXL_TY_I)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if(st==CCXL_TY_UI)
			{ return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if((st==CCXL_TY_NL) && !sctx->is_addr64)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		if((st==CCXL_TY_UNL) && !sctx->is_addr64)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL)) &&
			sctx->is_addr64)
		{
			if(sctx->has_bjx1egpr)
			{
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					BGBCC_SH_NMID_EXTUL, dreg, sreg));
			}

			nm1=BGBCC_SH_NMID_EXTUL;
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpReg(sctx, nm1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{

			return(BGBCC_JX2C_ConvLpRegisterNarrowing(ctx, sctx,
				BGBCC_SH_NMID_MOV, sreg, dreg));

#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
#endif
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
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
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) &&
			sctx->is_addr64)
		{
			nm1=BGBCC_SH_NMID_EXTUL;
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpReg(sctx, nm1, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
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

		if(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL)) &&
			sctx->is_addr64)
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);

//			tr0=BGBCC_SH_REG_R4;
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, tr0, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			return(BGBCC_JX2C_ConvLpRegisterNarrowing(ctx, sctx,
				nm1, sreg, dreg));

#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			tr0=BGBCC_SH_REG_R4;
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, tr0, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
#endif
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype))
		{
			BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRRANGELOSS);
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);

			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if(st==CCXL_TY_D)
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
		}


		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
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

		if(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_L) || (st==CCXL_TY_UL)) &&
			sctx->is_addr64)
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

			if(sctx->has_bjx1egpr)
			{
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					nm1, dreg, sreg));
			}

			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);

//			tr0=BGBCC_SH_REG_R4;
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpRegReg(sctx, nm1, tr0, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

			return(BGBCC_JX2C_ConvLpRegisterNarrowing(ctx, sctx,
				nm1, sreg, dreg));

#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2_EmitOpRegReg(sctx, nm1,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

			return(1);
#endif
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype))
		{
			BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONV_PTRRANGELOSS);
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);

			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if(st==CCXL_TY_D)
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg);
			BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, dreg);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

#if 1
	if(((dt==CCXL_TY_L) || (dt==CCXL_TY_UL) ||
		(dt==CCXL_TY_NL) || (dt==CCXL_TY_UNL)) &&
		sctx->is_addr64)
	{
	
		nm1=BGBCC_SH_NMID_EXTSL;
		if(st==CCXL_TY_UB)nm1=BGBCC_SH_NMID_EXTUB;
		if(st==CCXL_TY_SB)nm1=BGBCC_SH_NMID_EXTSB;
		if(st==CCXL_TY_US)nm1=BGBCC_SH_NMID_EXTUW;
		if(st==CCXL_TY_SS)nm1=BGBCC_SH_NMID_EXTSW;
		if(st==CCXL_TY_UI)nm1=BGBCC_SH_NMID_EXTUL;
		if(st==CCXL_TY_I)nm1=BGBCC_SH_NMID_EXTSL;
	
		if((st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_UB) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_UI) || ((st==CCXL_TY_UNL) && !sctx->is_addr64))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			if((nm1==BGBCC_SH_NMID_EXTUL) || (nm1==BGBCC_SH_NMID_EXTSL))
				BGBCC_JX2_EmitOpReg(sctx, nm1, tr1);
			else
				BGBCC_JX2_EmitOpRegReg(sctx, nm1, tr1, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if((st==CCXL_TY_SB) || (st==CCXL_TY_SS) || (st==CCXL_TY_I))
		{
#if 0
			if(sctx->has_bjx1egpr)
			{
				return(BGBCC_JX2C_EmitOpNmidVRegVReg(ctx, sctx, dtype,
					nm1, dreg, sreg));
			}
#endif

			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			tr0=BGBCC_SH_REG_RD0+(tr1&31);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			if((nm1==BGBCC_SH_NMID_EXTUL) || (nm1==BGBCC_SH_NMID_EXTSL))
				BGBCC_JX2_EmitOpReg(sctx, nm1, tr1);
			else
				BGBCC_JX2_EmitOpRegReg(sctx, nm1, tr1, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr1);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVQ, tr1, 0, tr1);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

//	if((dt==CCXL_TY_L) || (dt==CCXL_TY_UL) ||
//		((dt==CCXL_TY_NL) && sctx->is_addr64) ||
//		((dt==CCXL_TY_UNL) && sctx->is_addr64))
	if(((dt==CCXL_TY_L) || (dt==CCXL_TY_UL)) &&
		!sctx->is_addr64)
	{
//		if((st==CCXL_TY_L) || (st==CCXL_TY_UL) ||
//			((st==CCXL_TY_NL) && sctx->is_addr64) ||
//			((st==CCXL_TY_UNL) && sctx->is_addr64))
		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if((st==CCXL_TY_UB) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_UI) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_SS) ||
			(st==CCXL_TY_I) || (st==CCXL_TY_NL))
		{
			BGBCC_JX2C_ConvLpRegisterWiden(ctx, sctx,
				stype, sreg, dreg);
			return(1);
		}

#if 0
		if((st==CCXL_TY_UB) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_UI) || ((st==CCXL_TY_UNL) && !sctx->is_addr64))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R5, 0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
		}

		if((st==CCXL_TY_SB) || (st==CCXL_TY_SS) ||
			(st==CCXL_TY_I) || ((st==CCXL_TY_NL) && !sctx->is_addr64))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLeaSarRegImm(ctx, sctx, BGBCC_SH_REG_R5, 31);

			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

			return(1);
		}
#endif

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R6);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R6);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R6, 0, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R6, 4, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R6);

			return(1);
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		if((BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype)) && !sctx->is_addr64)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R5, 0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
			return(1);
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype) ||
			BGBCC_CCXL_TypeArrayP(ctx, stype))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

//	if(dt==CCXL_TY_F)
	if((dt==CCXL_TY_F) ||
		(dt==CCXL_TY_F16))
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) && !sctx->is_addr64) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
//			BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FCNVDS, BGBCC_SH_NMID_FSTS, dreg, sreg));
		}

		if(	(st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_D)
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(((st==CCXL_TY_NL) || (st==CCXL_TY_UNL)) && !sctx->is_addr64) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if((st==CCXL_TY_F) ||
			(st==CCXL_TY_F16))
		{
//			BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_JX2C_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FLDS, BGBCC_SH_NMID_FCNVSD, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
			return(1);
		}

		if(st==CCXL_TY_VARIANT)
		{
			return(
				BGBCC_JX2C_EmitConvToVRegVRegVariant(ctx, sctx,
					dtype, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_VARIANT)
	{
		if(st==CCXL_TY_VARIANT)
		{
			return(BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg));
		}
		
		return(
			BGBCC_JX2C_EmitConvFromVRegVRegVariant(ctx, sctx,
				stype, dreg, sreg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
