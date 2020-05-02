int BGBCC_BSRC_EmitDebugCheckReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, int sreg)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, type))
//		BGBCC_BSR_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
	return(0);
}

int BGBCC_BSRC_EmitConvOpRegReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int nmid, int sreg, int dreg)
{
	int i, j, k;

#if 0
	i=BGBCC_BSR_TryEmitOpRegReg(sctx,
		nmid, sreg, dreg);
	if(i>0)
		return(i);
#endif

	if(nmid==BGBCC_SH_NMID_LDHF16)
	{
		if(sctx->fpu_soft)
		{
			BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);
//			BGBCC_BSRC_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);

			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_BSR_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R4);
			BGBCC_BSRC_EmitCallName(ctx, sctx, "__sfp_ldhf16");
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_BSR_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, dreg);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	
//		BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);
		BGBCC_BSRC_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);

		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR0);
		BGBCC_BSR_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R4);
		BGBCC_BSRC_EmitCallName(ctx, sctx, "__ldhf16");
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_BSR_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FMOV, BGBCC_SH_REG_FR0, dreg);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_STHF16)
	{
		if(sctx->fpu_soft)
		{
//			BGBCC_BSRC_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);
			BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_BSR_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R4);
			BGBCC_BSRC_EmitCallName(ctx, sctx, "__sfp_sthf16");
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_BSR_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, dreg);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}

//		BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);
		BGBCC_BSRC_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);

		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR4);
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_BSR_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FMOV, sreg, BGBCC_SH_REG_FR4);
		BGBCC_BSRC_EmitCallName(ctx, sctx, "__sthf16");
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR4);
		BGBCC_BSR_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, dreg);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

#if 1
	i=BGBCC_BSR_TryEmitOpRegReg(sctx,
		nmid, sreg, dreg);
	if(i>0)
		return(i);
#endif

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitLoadSlotVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	ccxl_type type2;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
	fi=obj->fields[fid];

	type2=fi->type;

	ty=type.val;

//	if(!strcmp(ctx->cur_func->name, "RecursiveLightPoint") && !sctx->is_simpass)
//		k=-1;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		i=BGBCC_BSRC_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
			type, dreg, sreg, gblid, fid);
		return(i);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type2) ||
		BGBCC_CCXL_TypeArrayP(ctx, type2))
	{
		i=BGBCC_BSRC_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
			type, dreg, sreg, gblid, fid);
		return(i);
	}

#if 0
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;

	case CCXL_TY_F16:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_LDHF16; break;

	case CCXL_TY_VARIANT:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }
#endif

	BGBCC_BSRC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_BSRC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
			csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
			
			if(!BGBCC_BSR_EmitCheckRegExtAddrGPR(sctx, csreg))
			{
				BGBCC_DBGBREAK
				csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
			}
		}
	
//		if(((csreg&15)==15) && ((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }
//		if(((cdreg&15)==15) && ((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }

		ctreg=cdreg;

		if(!sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
					{ BGBCC_DBGBREAK }
			}
			else if(BGBCC_CCXL_TypeFloatP(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
			}
			else if(BGBCC_CCXL_TypeFloat16P(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
				ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			}
		}else
		{
			if(BGBCC_CCXL_TypeFloat16P(ctx, type))
			{
//				ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}
		}

		BGBCC_BSRC_EmitLoadBRegOfsReg(ctx, sctx,
			nm1, csreg, fi->fxoffs, ctreg);
		if(nm2>=0)
			{ BGBCC_BSRC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg, cdreg); }

		if(ctreg!=cdreg)
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitStoreSlotVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int tr0, tr1, tr2;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
	fi=obj->fields[fid];

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, dreg);
		ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		al=4;
		if(fi->fxoffs&3)al=2;
		if(fi->fxoffs&1)al=1;

//		i=BGBCC_BSRC_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
//			type, ctreg, sreg, gblid, fid);
		BGBCC_BSRC_EmitLeaBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, cdreg, fi->fxoffs, ctreg);

		BGBCC_BSRC_EmitValueCopyRegRegSz(ctx, sctx, ctreg, csreg, sz, al);

		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, ctreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	ty=type.val;

#if 0
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_F16:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_STHF16; break;
	case CCXL_TY_VARIANT:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }
#endif

	BGBCC_BSRC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(nm2==BGBCC_SH_NMID_LDHF16)
		nm2=BGBCC_SH_NMID_STHF16;

	if(nm1>=0)
	{
		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, dreg);

		ctreg=csreg;

//		if(((csreg&15)==15) && ((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }
//		if(((cdreg&15)==15) && ((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }

		if(!sctx->fpu_soft)
		{
			if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{
				if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_DR0)
					{ BGBCC_DBGBREAK }
			}
			else if(BGBCC_CCXL_TypeFloatP(ctx, type))
			{
				if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
			}
			else if(BGBCC_CCXL_TypeFloat16P(ctx, type))
			{
				if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
//				ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}
		}else
		{
			if(BGBCC_CCXL_TypeFloat16P(ctx, type))
			{
//				ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}
		}

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, type))
//		{
//			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
//			BGBCC_BSR_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVB,
//				csreg, tr0);
//			BGBCC_BSR_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, csreg);
//			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
//		}

		BGBCC_BSRC_EmitDebugCheckReg(ctx, sctx, type, csreg);
#endif

		if(nm2>=0)
			{ BGBCC_BSRC_EmitConvOpRegReg(ctx, sctx, nm2, csreg, ctreg); }
		BGBCC_BSRC_EmitStoreBRegOfsReg(ctx, sctx,
			nm1, cdreg, fi->fxoffs, ctreg);

		if(ctreg!=csreg)
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitLoadSlotAddrVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
	fi=obj->fields[fid];

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
//		cdreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
//		ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);

		al=4;
		if(fi->fxoffs&3)al=2;
		if(fi->fxoffs&1)al=1;
		
		if(BGBCC_CCXL_TypeValueObjectP(ctx, type) && (al!=4))
			{ BGBCC_DBGBREAK }

		BGBCC_BSRC_EmitLeaBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, csreg, fi->fxoffs, cdreg);

		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	ty=type.val;

#if 0
	sz=-1; nm1=-1;
	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;
	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_F16:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_VARIANT:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }
#endif

	BGBCC_BSRC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		BGBCC_BSRC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, fi->fxoffs, cdreg);
//		if(nm2>=0)
//			{ BGBCC_BSR_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

/** Emit logic to copy value-type given by registers and size. */
int BGBCC_BSRC_EmitValueCopyRegRegSz(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int dreg, int sreg, int sz, int al)
{
	char *s0;
	int tr0, tr1, tr2, tr3;
	int l0;
	int nm1, nm2;
	int i, j, k;

//	if(!strcmp(ctx->cur_func->name, "Mod_LoadBrushModel"))
//	{
//		k=-1;
//		__debugbreak();
//	}

	if(sctx->has_misalgn)
	{
		/*	if we have misaligned load/store,
			pretend copy is aligned. */
		if((al<4) && !(sz&3))
			al=4;
	}

	nm1=BGBCC_SH_NMID_MOVL;
	nm2=BGBCC_SH_NMID_MOVL;
	if(((sz&3)==1) && (al<=1))nm2=BGBCC_SH_NMID_MOVB;
	if(((sz&3)==2) && (al<=2))nm2=BGBCC_SH_NMID_MOVW;

	if(sz<=4)
	{
		if(al>=4)
		{
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
//			tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_BSR_EmitOpLdRegReg(sctx, nm1, sreg, BGBCC_SH_REG_R0);
//			BGBCC_BSR_EmitOpRegStReg(sctx, nm1, BGBCC_SH_REG_R0, dreg);
			BGBCC_BSR_EmitOpLdRegReg(sctx, nm1, sreg, tr0);
			BGBCC_BSR_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
//			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		if(al==2)
		{
//			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
//			tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			tr0=BGBCC_SH_REG_R0;
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);

			nm1=BGBCC_SH_NMID_MOVW;
			if(sz>2)
			{
				BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1,	sreg, 0, tr0);
				BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1,	tr0, dreg, 0);
				BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1,	sreg, 2, tr0);
				BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 2);
			}else
			{
				BGBCC_BSR_EmitOpLdRegReg(sctx, nm1, sreg, tr0);
				BGBCC_BSR_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			}

			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
//			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		nm1=BGBCC_SH_NMID_MOVB;
		tr0=BGBCC_SH_REG_R0;
		BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		for(i=0; i<sz; i++)
		{
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, i, tr0);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
		}
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
	
//		BGBCC_BSR_EmitOpLdRegReg(sctx, nm2, sreg, BGBCC_SH_REG_R0);
//		BGBCC_BSR_EmitOpRegStReg(sctx, nm2, BGBCC_SH_REG_R0, dreg);
		return(1);
	}

	if((al>=8) && sctx->is_addr64 && sctx->has_bjx1mov)
	{
		if(sz==8)
		{
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}

		if(sz==16)
		{
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
	}

	if((sz<=8) && (al>=4))
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm2, sreg, 4, tr1);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, 4);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if((sz<=12) && (al>=4))
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm2, sreg, 8, tr0);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm2, tr0, dreg, 8);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if((sz<=16) && (al>=4))
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr0);
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm2, sreg,12, tr1);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 8);
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg,12);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

#if 1
	if((sz<=64) && (al>=4))
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);

		i=0;
		while((i+8)<sz)
		{
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 4, tr1);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, i+ 4);
			i+=8;
		}
		if((sz-i)>4)
		{
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm1, sreg, i+0, tr0);
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm2, sreg, i+4, tr1);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+0);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, i+4);
		}else
		{
			BGBCC_BSR_EmitOpLdRegDispReg(sctx, nm2, sreg, i, tr0);
			BGBCC_BSR_EmitOpRegStRegDisp(sctx, nm2, tr0, dreg, i);
		}

		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}
#endif

#if 0
	if((sz<=64) && (al>=4))
	{
		switch((sz+3)/4)
		{
		case  1: s0="__memcpy32_4"; break;
		case  2: s0="__memcpy32_8"; break;
		case  3: s0="__memcpy32_12"; break;
		case  4: s0="__memcpy32_16"; break;
		case  5: s0="__memcpy32_20"; break;
		case  6: s0="__memcpy32_24"; break;
		case  7: s0="__memcpy32_28"; break;
		case  8: s0="__memcpy32_32"; break;
		case  9: s0="__memcpy32_36"; break;
		case 10: s0="__memcpy32_40"; break;
		case 11: s0="__memcpy32_44"; break;
		case 12: s0="__memcpy32_48"; break;
		case 13: s0="__memcpy32_52"; break;
		case 14: s0="__memcpy32_58"; break;
		case 15: s0="__memcpy32_60"; break;
		case 16: s0="__memcpy32_64"; break;
		default: s0=NULL; break;
		}
		
		if(s0)
		{
			BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R4);
			BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
				sreg, BGBCC_SH_REG_R5);
			BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
			return(1);
		}
	}
#endif

#if 1	
	if((al>=4) && !sctx->sreg_live)
	{
		BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R5);
		BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_BSRC_EmitCallName(ctx, sctx, "__memcpy32");
		return(1);
	}
#endif

#if 1
	if(al>=4)
	{
		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);

		tr2=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		tr3=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);

		l0=BGBCC_BSR_GenLabelTemp(sctx);

		BGBCC_BSRC_EmitMovRegReg(ctx, sctx, dreg, tr2);
		BGBCC_BSRC_EmitMovRegReg(ctx, sctx, sreg, tr3);
		BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, (sz+3)/4);

		BGBCC_BSR_EmitLabel(sctx, l0);

		BGBCC_BSR_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVL, tr3, tr0);
		BGBCC_BSR_EmitOpRegStReg(sctx,
			BGBCC_SH_NMID_MOVL, tr0, tr2);
		BGBCC_BSR_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, 4, tr2);
		BGBCC_BSRC_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_DT, tr1);
		BGBCC_BSR_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);

		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr1);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr2);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr3);
		return(1);
	}
#endif

#if 1
	if((sz<=64) && (al>=1) && !(sz&3))
	{
		switch((sz+3)/4)
		{
		case  1: s0="__memcpy8_4"; break;
		case  2: s0="__memcpy8_8"; break;
		case  3: s0="__memcpy8_12"; break;
		case  4: s0="__memcpy8_16"; break;
		case  5: s0="__memcpy8_20"; break;
		case  6: s0="__memcpy8_24"; break;
		case  7: s0="__memcpy8_28"; break;
		case  8: s0="__memcpy8_32"; break;
#if 0
		case  9: s0="__memcpy8_36"; break;
		case 10: s0="__memcpy8_40"; break;
		case 11: s0="__memcpy8_44"; break;
		case 12: s0="__memcpy8_48"; break;
		case 13: s0="__memcpy8_52"; break;
		case 14: s0="__memcpy8_58"; break;
		case 15: s0="__memcpy8_60"; break;
		case 16: s0="__memcpy8_64"; break;
#endif
		default: s0=NULL; break;
		}
		
		if(s0)
		{
			BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R4);
			BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
				sreg, BGBCC_SH_REG_R5);
			BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
			return(1);
		}
	}
#endif

	BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

	BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
		dreg, BGBCC_SH_REG_R4);
	BGBCC_BSRC_EmitMovRegReg(ctx, sctx,
		sreg, BGBCC_SH_REG_R5);
	BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R6, sz);
	BGBCC_BSRC_EmitCallName(ctx, sctx, "memcpy");
	return(1);
}


int BGBCC_BSRC_EmitLoadTypeBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, int sreg, int ofs, int dreg)
{
	int nm1, nm2, ty, sz;
	
	ty=type.val;

	switch(ty)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;

	case CCXL_TY_F:
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:	case CCXL_TY_UL:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;

	case CCXL_TY_F16:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_LDHF16; break;
	case CCXL_TY_VARIANT:
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
		
	default:
		sz=1; nm1=-1; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
		{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }

	BGBCC_BSRC_EmitLoadOp2BRegOfsReg(ctx, sctx,
		nm1, nm2, sreg, ofs, dreg);
	return(1);
}
