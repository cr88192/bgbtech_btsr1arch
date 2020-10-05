int BGBCC_JX2C_EmitDebugCheckReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int sreg)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, type))
//		BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
	return(0);
}

int BGBCC_JX2C_EmitConvOpRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int sreg, int dreg)
{
	int sreg2, dreg2;
	int i, j, k;

#if 0
	i=BGBCC_JX2_TryEmitOpRegReg(sctx,
		nmid, sreg, dreg);
	if(i>0)
		return(i);
#endif

	if(nmid==BGBCC_SH_NMID_LDHF16)
	{
		if(sctx->fpu_soft)
		{
			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
//			BGBCC_JX2C_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
			BGBCC_JX2_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__sfp_ldhf16");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R2, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
			return(1);
		}

		if(BGBCC_JX2_TryEmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDCH, sreg, dreg)>0)
		{
			return(1);
		}
	
//		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR2);
		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__ldhf16");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FMOV, BGBCC_SH_REG_FR2, dreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR2);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_STHF16)
	{
		if(sctx->fpu_soft)
		{
//			BGBCC_JX2C_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);
			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
			BGBCC_JX2_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__sfp_sthf16");
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
			BGBCC_JX2_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R2, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
			return(1);
		}

		if(BGBCC_JX2_TryEmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTCH, sreg, dreg)>0)
		{
			return(1);
		}
	
//		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);
		BGBCC_JX2C_EmitScratchSyncRegistersSafeMask(ctx, sctx, 0xC0);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FMOV, sreg, BGBCC_SH_REG_FR4);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__sthf16");
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR4);
		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R2, dreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_LDBF16)
	{
//		BGBCC_JX2_EmitOpRegReg(sctx,
//			BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpReg(sctx,
//			BGBCC_SH_NMID_SHLL16, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLD, sreg, 16, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpReg(sctx,
			BGBCC_SH_NMID_FLDCF, dreg);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_STBF16)
	{
		BGBCC_JX2_EmitOpReg(sctx,
			BGBCC_SH_NMID_FSTCF, sreg);
//		BGBCC_JX2_EmitOpReg(sctx,
//			BGBCC_SH_NMID_SHLR16, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegReg(sctx,
//			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, dreg);
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLD, BGBCC_SH_REG_R0, -16, dreg);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_SHLL32)
	{
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLDQ, sreg, 32, dreg);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_SHLR32)
	{
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHLDQ, sreg, -32, dreg);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_SHLL64)
	{
		sreg2=BGBCC_SH_REG_RQ0+(sreg&31);
		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);

		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, sreg2+0, dreg2+1);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_MOV, 0, dreg2+0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_SHLR64)
	{
		sreg2=BGBCC_SH_REG_RQ0+(sreg&31);
		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);

		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, sreg2+1, dreg2+0);
		return(1);
	}

#if 1
	i=BGBCC_JX2_TryEmitOpRegReg(sctx,
		nmid, sreg, dreg);
	if(i>0)
		return(i);
#endif

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitLoadSlotVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	char tb[256];
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	ccxl_type type2;
	int csreg, ctreg, cdreg;
	int tr0, tr1;
	int nm1, nm2, nm3, nm4, ty, sz;
	int vtix, vtoffs;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
	
	fi=NULL;
	
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_FIELD)
		fi=obj->fields[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_ARGS)
		fi=obj->args[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_REGS)
		fi=obj->regs[fid&CCXL_FID_BASEMASK];

	type2=fi->type;

	ty=type.val;

//	if(!strcmp(ctx->cur_func->name, "RecursiveLightPoint") && !sctx->is_simpass)
//		k=-1;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		i=BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
			type, dreg, sreg, gblid, fid);
		return(i);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type2) ||
		BGBCC_CCXL_TypeArrayP(ctx, type2))
	{
		i=BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
			type, dreg, sreg, gblid, fid);
		return(i);
	}

	if((obj->flagsint&BGBCC_TYFL_NONPOD) &&
		(fi->flagsint&BGBCC_TYFL_VIRTUAL))
	{
//		if((BGBCC_CCXL_TypeFunctionP(ctx, type) ||
//			BGBCC_CCXL_TypeFunctionP(ctx, type2)) &&
//			!(BGBCC_CCXL_TypePointerP(ctx, type2) ||
//			  BGBCC_CCXL_TypeFatPointerP(ctx, type2)))
		if	((fi->regtype==CCXL_LITID_FUNCTION) ||
			 (fi->regtype==CCXL_LITID_PROTOTYPE))
		{
//			vtix=fi->fxmoffs;
			vtix=fi->fxmoffs;
			vtoffs=vtix*ctx->arch_sizeof_ptr;

			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
	//		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

			if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, cdreg))
			{
				tr0=BGBCC_SH_REG_R0|((cdreg&31)+0);
				tr1=BGBCC_SH_REG_R0|((cdreg&31)+1);
			
				BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, csreg, 0, tr0);
				BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, tr0, vtoffs, tr0);
				BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, tr1);
			}else
			{
				BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, csreg, 0, cdreg);
				BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, cdreg, vtoffs, cdreg);
			}

	//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
	}

//	if((BGBCC_CCXL_TypeFunctionP(ctx, type) ||
//		BGBCC_CCXL_TypeFunctionP(ctx, type2)) &&
//		!(BGBCC_CCXL_TypePointerP(ctx, type2) ||
//		  BGBCC_CCXL_TypeFatPointerP(ctx, type2)))
	if	((fi->regtype==CCXL_LITID_FUNCTION) ||
		 (fi->regtype==CCXL_LITID_PROTOTYPE))
	{
//		sprintf(tb, "%s/%s", obj->name, fi->name);
		sprintf(tb, "%s/%s", obj->qname, fi->name);
		j=BGBCC_CCXL_LookupGlobalIndex(ctx, tb);

		if(j>0)
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

			nm1=BGBCC_SH_NMID_MOVL;
			if(sctx->is_addr64)
				nm1=BGBCC_SH_NMID_MOVQ;

			if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, cdreg))
			{
				tr0=BGBCC_SH_REG_R0|((cdreg&31)+0);
				tr1=BGBCC_SH_REG_R0|((cdreg&31)+1);

				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, tr0, k);
				BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, tr1);
			}else
			{
//				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, cdreg, k);
			}

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_DBGBREAK
	}

	BGBCC_JX2C_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			
			if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, csreg))
			{
				BGBCC_JX2C_LoadVectorField128(ctx, sctx,
					type, fi->fxoffs,
					csreg, cdreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
			
			if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(sctx, csreg))
			{
				BGBCC_DBGBREAK
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			}
		}
	
//		if(((csreg&15)==15) && ((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }
//		if(((cdreg&15)==15) && ((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }

		ctreg=cdreg;

		if(!sctx->fpu_soft && !sctx->fpu_gfp)
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
			else if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//				ctreg=BGBCC_SH_REG_R1;
			}
		}else
		{
#if 0
			if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}

			if(BGBCC_CCXL_TypeFloatP(ctx, type) && sctx->fpu_gfp)
			{
				ctreg=BGBCC_SH_REG_R1;
			}
#endif
		}

		if(nm4>=0)
		{
			i=BGBCC_JX2_TryEmitOpLdRegDispReg(sctx, nm4,
				csreg, fi->fxoffs, ctreg);
			if(i>0)
			{
				if(ctreg!=cdreg)
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
					BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}

		if(nm2>=0)
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg=BGBCC_SH_REG_R1;
		}

		BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
			nm1, csreg, fi->fxoffs, ctreg);
		if(nm2>=0)
			{ BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx, nm2, ctreg, cdreg); }

		if(ctreg!=cdreg)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitStoreSlotVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
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
//	fi=obj->fields[fid];

	fi=NULL;
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_FIELD)
		fi=obj->fields[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_ARGS)
		fi=obj->args[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_REGS)
		fi=obj->regs[fid&CCXL_FID_BASEMASK];

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		al=8;
//		if(fi->fxoffs&15)al=8;
		if(fi->fxoffs&7)al=4;
		if(fi->fxoffs&3)al=2;
		if(fi->fxoffs&1)al=1;

//		i=BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
//			type, ctreg, sreg, gblid, fid);
		BGBCC_JX2C_EmitLeaBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, cdreg, fi->fxoffs, ctreg);

		BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, ctreg, csreg, sz, al);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	ty=type.val;

	BGBCC_JX2C_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(nm2==BGBCC_SH_NMID_LDHF16)
		nm2=BGBCC_SH_NMID_STHF16;
	if(nm2==BGBCC_SH_NMID_FLDCF)
		nm2=BGBCC_SH_NMID_FSTCF;
	if(nm2==BGBCC_SH_NMID_FLDCH)
		nm2=BGBCC_SH_NMID_FSTCH;

	if(nm2==BGBCC_SH_NMID_SHLL32)
		nm2=BGBCC_SH_NMID_SHLR32;
	if(nm2==BGBCC_SH_NMID_SHLL64)
		nm2=BGBCC_SH_NMID_SHLR64;

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);

		ctreg=csreg;

//		if(((csreg&15)==15) && ((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }
//		if(((cdreg&15)==15) && ((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0))
//			{ BGBCC_DBGBREAK }

		if(!sctx->fpu_soft && !sctx->fpu_gfp)
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
			else if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
				if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//				ctreg=BGBCC_SH_REG_R1;
			}
		}else
		{
#if 0
			if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}

			if(BGBCC_CCXL_TypeFloatP(ctx, type) && sctx->fpu_gfp)
			{
				ctreg=BGBCC_SH_REG_R1;
			}
#endif
		}

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, type))
//		{
//			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
//			BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVB,
//				csreg, tr0);
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, csreg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
//		}

		BGBCC_JX2C_EmitDebugCheckReg(ctx, sctx, type, csreg);
#endif

		if(nm2>=0)
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg=BGBCC_SH_REG_R1;
		}

		if(nm2>=0)
			{ BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx, nm2, csreg, ctreg); }
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			nm1, cdreg, fi->fxoffs, ctreg);

		if(ctreg!=csreg)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
//	fi=obj->fields[fid];

	fi=NULL;
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_FIELD)
		fi=obj->fields[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_ARGS)
		fi=obj->args[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_REGS)
		fi=obj->regs[fid&CCXL_FID_BASEMASK];

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
//		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
//		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);

		al=4;
		if(fi->fxoffs&3)al=2;
		if(fi->fxoffs&1)al=1;
		
		if(BGBCC_CCXL_TypeValueObjectP(ctx, type) && (al!=4))
			{ BGBCC_DBGBREAK }

		BGBCC_JX2C_EmitLeaBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, csreg, fi->fxoffs, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	ty=type.val;

	BGBCC_JX2C_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		BGBCC_JX2C_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, fi->fxoffs, cdreg);
//		if(nm2>=0)
//			{ BGBCC_JX2_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


#if 1
int BGBCC_JX2C_EmitLoadSlotRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	ccxl_type type2;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
//	fi=obj->fields[fid];

	fi=NULL;
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_FIELD)
		fi=obj->fields[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_ARGS)
		fi=obj->args[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_REGS)
		fi=obj->regs[fid&CCXL_FID_BASEMASK];

	type2=fi->type;

	ty=type.val;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		i=BGBCC_JX2C_EmitLoadSlotAddrRegVRegImm(ctx, sctx,
			type, dreg, sreg, gblid, fid);
		return(i);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type2) ||
		BGBCC_CCXL_TypeArrayP(ctx, type2))
	{
		i=BGBCC_JX2C_EmitLoadSlotAddrRegVRegImm(ctx, sctx,
			type, dreg, sreg, gblid, fid);
		return(i);
	}

	BGBCC_JX2C_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		cdreg=dreg;
		
		if(!BGBCC_JX2_EmitCheckRegExtAddrGPR(sctx, csreg))
		{
			BGBCC_DBGBREAK
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		}
	
		ctreg=cdreg;

		if(!sctx->fpu_soft && !sctx->fpu_gfp)
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
			else if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			}
		}else
		{
#if 0
			if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}

			if(BGBCC_CCXL_TypeFloatP(ctx, type) && sctx->fpu_gfp)
			{
				ctreg=BGBCC_SH_REG_R1;
			}
#endif
		}
		
		if(nm2>=0)
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg=BGBCC_SH_REG_R1;
		}

		BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
			nm1, csreg, fi->fxoffs, ctreg);
		if(nm2>=0)
			{ BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx, nm2, ctreg, cdreg); }

		if(ctreg!=cdreg)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitStoreSlotVRegRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int tr0, tr1, tr2;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

//	obj=ctx->reg_globals[gblid];
	obj=ctx->literals[gblid]->decl;
//	fi=obj->fields[fid];

	fi=NULL;
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_FIELD)
		fi=obj->fields[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_ARGS)
		fi=obj->args[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_REGS)
		fi=obj->regs[fid&CCXL_FID_BASEMASK];

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
//		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		csreg=sreg;
		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		al=4;
		if(fi->fxoffs&3)al=2;
		if(fi->fxoffs&1)al=1;

//		i=BGBCC_JX2C_EmitLoadSlotAddrVRegVRegImm(ctx, sctx,
//			type, ctreg, sreg, gblid, fid);
		BGBCC_JX2C_EmitLeaBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, cdreg, fi->fxoffs, ctreg);

		BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, ctreg, csreg, sz, al);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	ty=type.val;

	BGBCC_JX2C_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(nm2==BGBCC_SH_NMID_LDHF16)
		nm2=BGBCC_SH_NMID_STHF16;
	if(nm2==BGBCC_SH_NMID_FLDCF)
		nm2=BGBCC_SH_NMID_FSTCF;
	if(nm2==BGBCC_SH_NMID_FLDCH)
		nm2=BGBCC_SH_NMID_FSTCH;

	if(nm2==BGBCC_SH_NMID_SHLL32)
		nm2=BGBCC_SH_NMID_SHLR32;
	if(nm2==BGBCC_SH_NMID_SHLL64)
		nm2=BGBCC_SH_NMID_SHLR64;

	if(nm1>=0)
	{
//		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		csreg=sreg;
		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);

		ctreg=csreg;

		if(!sctx->fpu_soft && !sctx->fpu_gfp)
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
			else if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
				if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//				ctreg=BGBCC_SH_REG_R1;
			}
		}else
		{
#if 0
			if(BGBCC_CCXL_TypeFloat16P(ctx, type) ||
				BGBCC_CCXL_TypeBFloat16P(ctx, type))
			{
//				ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
				ctreg=BGBCC_SH_REG_R1;
			}

			if(BGBCC_CCXL_TypeFloatP(ctx, type) && sctx->fpu_gfp)
			{
				ctreg=BGBCC_SH_REG_R1;
			}
#endif
		}

#if 1	//Debug: Deref pointers to check validity
		BGBCC_JX2C_EmitDebugCheckReg(ctx, sctx, type, csreg);
#endif

		if(nm2>=0)
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			ctreg=BGBCC_SH_REG_R1;
		}

		if(nm2>=0)
			{ BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx, nm2, csreg, ctreg); }
		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			nm1, cdreg, fi->fxoffs, ctreg);

		if(ctreg!=csreg)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
#endif

int BGBCC_JX2C_EmitLoadSlotAddrRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int dreg,
	ccxl_register sreg, int gblid, int fid)
{
	BGBCC_CCXL_RegisterInfo *obj, *fi;
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

	obj=ctx->literals[gblid]->decl;
//	fi=obj->fields[fid];

	fi=NULL;
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_FIELD)
		fi=obj->fields[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_ARGS)
		fi=obj->args[fid&CCXL_FID_BASEMASK];
	if((fid&CCXL_FID_TAGMASK)==CCXL_FID_TAG_REGS)
		fi=obj->regs[fid&CCXL_FID_BASEMASK];

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		cdreg=dreg;
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);

		al=4;
		if(fi->fxoffs&3)al=2;
		if(fi->fxoffs&1)al=1;
		
		if(BGBCC_CCXL_TypeValueObjectP(ctx, type) && (al!=4))
			{ BGBCC_DBGBREAK }

		BGBCC_JX2C_EmitLeaBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, csreg, fi->fxoffs, cdreg);

//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	ty=type.val;

	BGBCC_JX2C_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		cdreg=dreg;
		
		BGBCC_JX2C_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, fi->fxoffs, cdreg);
//		if(nm2>=0)
//			{ BGBCC_JX2_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

#if 1
int BGBCC_JX2C_EmitLoadThisIdxVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register sreg,
	int dreg)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register tvreg;
	ccxl_type tty;
	int gblid;
	int scid, fid;
	int i;

	tty=ctx->cur_func->clz_type;
	i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
	if(i>=256)
		{ gblid=i-256; }
	else
		{ BGBCC_DBGBREAK }
	obj=ctx->literals[gblid];
	
//	scid=(sreg.val>>12)&4095;
	scid=(sreg.val>>12)& 255;
	fid =(sreg.val    )&4095;
	fid|=(sreg.val<< 4)&0x0F000000;

	while(scid)
	{
		tty=obj->decl->args[0]->type;
		i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
		if(i>=256)
			{ gblid=i-256; }
		else
			{ BGBCC_DBGBREAK }
		obj=ctx->literals[gblid];
		
		scid--;
	}

	tvreg.val=CCXL_REGSP_THIS;
	i=BGBCC_JX2C_EmitLoadSlotRegVRegImm(ctx, sctx,
		type, dreg, tvreg, gblid, fid);
	return(i);

//		tty=BGBCC_CCXL_GetRegType(ctx, sreg);
//		tvreg.val=CCXL_REGSP_THIS;
//		BGBCC_JX2C_EmitLoadSlotRegVRegImm(ctx, sctx, tty,
//			dreg, sreg, 
	
//		treg=BGBCC_SH_REG_R1;
//		k=sctx->frm_offs_thisptr;
//		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
}

int BGBCC_JX2C_EmitStoreThisIdxVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, ccxl_register sreg,
	int vreg)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register tvreg;
	ccxl_type tty;
	int gblid;
	int scid, fid;
	int i;

	tty=ctx->cur_func->clz_type;
	i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
	if(i>=256)
		{ gblid=i-256; }
	else
		{ BGBCC_DBGBREAK }
	obj=ctx->literals[gblid];
	
//	scid=(sreg.val>>12)&4095;
	scid=(sreg.val>>12)&255;
	fid =(sreg.val    )&4095;
	fid|=(sreg.val<< 4)&0x0F000000;

	while(scid)
	{
		tty=obj->decl->args[0]->type;
		i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
		if(i>=256)
			{ gblid=i-256; }
		else
			{ BGBCC_DBGBREAK }
		obj=ctx->literals[gblid];
		
		scid--;
	}

	tvreg.val=CCXL_REGSP_THIS;
	i=BGBCC_JX2C_EmitStoreSlotVRegRegImm(ctx, sctx,
		type, tvreg, vreg, gblid, fid);
	return(i);
}
#endif

/** Emit logic to copy value-type given by registers and size. */
int BGBCC_JX2C_EmitValueCopyRegRegSz(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int dreg, int sreg, int sz, int al)
{
	char *s0;
	int tr0, tr1, tr2, tr3;
	int l0, al0;
	int nm1, nm2;
	int step;
	int i, j, k;

//	if(!strcmp(ctx->cur_func->name, "Mod_LoadBrushModel"))
//	{
//		k=-1;
//		__debugbreak();
//	}

	al0=al;
	if(sctx->has_misalgn)
	{
		/*	if we have misaligned load/store,
			pretend copy is aligned. */
		if((al<4) && !(sz&3))
			al=4;
		if(!(sz&7))
			al=8;
	}

	nm1=BGBCC_SH_NMID_MOVL;
	nm2=BGBCC_SH_NMID_MOVL;
	if(((sz&3)==1) && (al<=1))nm2=BGBCC_SH_NMID_MOVB;
	if(((sz&3)==2) && (al<=2))nm2=BGBCC_SH_NMID_MOVW;

	if(sz<=4)
	{
		if(al>=4)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_JX2_EmitOpLdRegReg(sctx, nm1, sreg, BGBCC_SH_REG_R0);
//			BGBCC_JX2_EmitOpRegStReg(sctx, nm1, BGBCC_SH_REG_R0, dreg);
			BGBCC_JX2_EmitOpLdRegReg(sctx, nm1, sreg, tr0);
			BGBCC_JX2_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		if(al==2)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			tr0=BGBCC_SH_REG_R0;
//			tr0=BGBCC_SH_REG_R1;
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tr0);

			nm1=BGBCC_SH_NMID_MOVW;
			if(sz>2)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1,	sreg, 0, tr0);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1,	tr0, dreg, 0);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1,	sreg, 2, tr0);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 2);
			}else
			{
				BGBCC_JX2_EmitOpLdRegReg(sctx, nm1, sreg, tr0);
				BGBCC_JX2_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			}

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		nm1=BGBCC_SH_NMID_MOVB;
//		tr0=BGBCC_SH_REG_R0;
//		tr0=BGBCC_SH_REG_R1;
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, tr0);
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		for(i=0; i<sz; i++)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
		}
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
	
//		BGBCC_JX2_EmitOpLdRegReg(sctx, nm2, sreg, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegStReg(sctx, nm2, BGBCC_SH_REG_R0, dreg);
		return(1);
	}

	if(sctx->has_pushx2 && (al0>=8) && !(sz&15) && (sz>32) && (sz<=256) &&
		(BGBCC_JX2C_ScratchQueryReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2)>2))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);
		tr2=-1;
		tr3=-1;
		nm1=BGBCC_SH_NMID_MOVX2;

		i=0;

		if((sz>=64) &&
			(BGBCC_JX2C_ScratchQueryReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2)>2))
		{
			tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);
			tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);

			while((i+64)<=sz)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+16, tr1);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+32, tr2);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+48, tr3);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, i+16);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr2, dreg, i+32);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr3, dreg, i+48);
				i+=64;
			}
		}

		while((i+32)<=sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+16, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, i+16);
			i+=32;
		}

		while((i+16)<=sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
			i+=16;
		}

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		if(tr2>=0)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
		if(tr3>=0)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
		return(1);
	}

	if((al>=8) && sctx->is_addr64 && sctx->has_bjx1mov)
	{
		if(sz==8)
		{
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}

		if(sz==16)
		{
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}

		if(sz==24)
		{
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr1);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 16, tr2);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 8);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr2, dreg, 16);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
			return(1);
		}

		if(sz==32)
		{
#if 0
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 8);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 16, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 24, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 16);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 24);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
#endif

#if 1
			nm1=BGBCC_SH_NMID_MOVQ;
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr1);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 16, tr2);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 24, tr3);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 8);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr2, dreg, 16);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr3, dreg, 24);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
#endif
			return(1);
		}
	}

#if 1
	if((sz<=128) && (al>=8))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr2=-1;			tr3=-1;
		if(sz>=64)
		{
			tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		}

		nm1=BGBCC_SH_NMID_MOVQ;
		if(!(sz&7))
			nm2=BGBCC_SH_NMID_MOVQ;

		i=0;

		while(((i+32)<=sz) && (tr3>0))
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 8, tr1);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+16, tr2);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+24, tr3);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, i+ 8);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr2, dreg, i+16);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr3, dreg, i+24);
			i+=32;
		}

		while((i+16)<sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 8, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, i+ 8);
			i+=16;
		}
		if((sz-i)>8)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg, i+8, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, i+8);
		}else if((sz-i)>0)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr0, dreg, i);
		}

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		if(tr2>0)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
		if(tr3>0)
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
		return(1);
	}
#endif

	if((sz<=8) && (al>=4))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg, 4, tr1);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, 4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if((sz<=12) && (al>=4))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg, 8, tr0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr0, dreg, 8);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if((sz<=16) && (al>=4))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 0, tr0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 4, tr1);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, 4);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, 8, tr0);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg,12, tr1);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 8);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg,12);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

#if 1
	if((sz<=64) && (al>=4))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		i=0;
		while((i+8)<sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+ 4, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+ 0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr1, dreg, i+ 4);
			i+=8;
		}
		if((sz-i)>4)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1, sreg, i+0, tr0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg, i+4, tr1);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i+0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr1, dreg, i+4);
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm2, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm2, tr0, dreg, i);
		}

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}
#endif

#if 1
	if(sz<=64)
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		i=0;
		while((i+8)<=sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVQ, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVQ, tr0, dreg, i);
			i+=8;
		}

		if((i+4)<=sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVL, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVL, tr0, dreg, i);
			i+=4;
		}

		if((i+2)<=sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVW, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVW, tr0, dreg, i);
			i+=2;
		}

		if((i+1)<=sz)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVB, sreg, i, tr0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVB, tr0, dreg, i);
			i+=1;
		}

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}
#endif

#if 0
	if((al>=8) && !(sz&7) && sctx->sreg_live)
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		l0=BGBCC_JX2_GenLabelTemp(sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, dreg, tr2);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, sreg, tr3);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, (sz+7)/8);

		BGBCC_JX2_EmitLabel(sctx, l0);

		BGBCC_JX2_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVQ, tr3, tr0);
		BGBCC_JX2_EmitOpRegStReg(sctx,
			BGBCC_SH_NMID_MOVQ, tr0, tr2);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, 8, tr2);
//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_DT, tr1);
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, -1, tr1);
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_CMPEQ, 0, tr1);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
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
			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				sreg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			return(1);
		}
	}
#endif

#if 1
	if((al>=8) && !(sz&15) && !sctx->sreg_live)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R5);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__memcpy128");
		return(1);
	}
#endif

#if 1
	if((al>=8) && !sctx->sreg_live)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R5);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__memcpy64");
		return(1);
	}
#endif

#if 1
	if((al>=4) && !sctx->sreg_live)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R5);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__memcpy32");
		return(1);
	}
#endif

#if 0
	if((al>=4) || sctx->sreg_live)
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		l0=BGBCC_JX2_GenLabelTemp(sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, dreg, tr2);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, sreg, tr3);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, (sz+3)/4);

		BGBCC_JX2_EmitLabel(sctx, l0);

		BGBCC_JX2_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVL, tr3, tr0);
		BGBCC_JX2_EmitOpRegStReg(sctx,
			BGBCC_SH_NMID_MOVL, tr0, tr2);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, 4, tr2);
		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_DT, tr1);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
		return(1);
	}
#endif

#if 0
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
			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				dreg, BGBCC_SH_REG_R4);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				sreg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			return(1);
		}
	}
#endif

	if(!sctx->sreg_live)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R5);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "memcpy");
		return(1);
	}

#if 1
	if(1)
	{
		/* Kinda stuck, inline copy loop... */

		if(!(sz&7))
			{ nm2=BGBCC_SH_NMID_MOVQ; step=8; }
		else if(!(sz&3))
			{ nm2=BGBCC_SH_NMID_MOVL; step=4; }
		else if(!(sz&1))
			{ nm2=BGBCC_SH_NMID_MOVW; step=2; }
		else
			{ nm2=BGBCC_SH_NMID_MOVB; step=1; }

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		l0=BGBCC_JX2_GenLabelTemp(sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, dreg, tr2);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, sreg, tr3);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, sz/step);

		BGBCC_JX2_EmitLabel(sctx, l0);

		BGBCC_JX2_EmitOpLdIncRegReg(sctx,
			nm2, tr3, tr0);
		BGBCC_JX2_EmitOpRegStReg(sctx,
			nm2, tr0, tr2);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, step, tr2);
//		BGBCC_JX2C_EmitOpReg(ctx, sctx, BGBCC_SH_NMID_DT, tr1);
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, -1, tr1);
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_CMPEQ, 0, tr1);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
		return(1);
	}
#endif
	
	BGBCC_DBGBREAK
	return(0);
}


int BGBCC_JX2C_EmitLoadTypeBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int sreg, int ofs, int dreg)
{
	int nm1, nm2, ty, sz;
	
	ty=type.val;

	switch(ty)
	{
	case CCXL_TY_I:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	case CCXL_TY_UI:
		sz=4; nm1=BGBCC_SH_NMID_MOVUL; nm2=-1; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		if(ctx->arch_sizeof_long==8)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		else
			{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break; }

	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW; break;

	case CCXL_TY_F:
		if(sctx->fpu_soft)
			{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break; }
		else
			{ sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break; }
	case CCXL_TY_D:
		if(sctx->fpu_soft)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		else
			{ sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break; }
	case CCXL_TY_L:	case CCXL_TY_UL:
		if(sctx->is_addr64)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		else
			{ sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break; }

	case CCXL_TY_F16:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_LDHF16; break;

	case CCXL_TY_VARIANT:
	case CCXL_TY_VARSTRING:
	case CCXL_TY_VAROBJECT:
		if(sctx->is_addr64)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		else
			{ sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break; }

	case CCXL_TY_M64:
	case CCXL_TY_VEC2F:
	case CCXL_TY_VEC4SW:
	case CCXL_TY_VEC4UW:
	case CCXL_TY_FCOMPLEX:
		sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1;
		break;

	case CCXL_TY_M128:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
	case CCXL_TY_VEC2D:
	case CCXL_TY_VEC4SI:
	case CCXL_TY_VEC4UI:
	case CCXL_TY_DCOMPLEX:
		sz=16; nm1=BGBCC_SH_NMID_MOVX2; nm2=-1;
		break;

	case CCXL_TY_FIMAG:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=BGBCC_SH_NMID_SHLL32;
		break;
	case CCXL_TY_DIMAG:
		sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=BGBCC_SH_NMID_SHLL32;
		break;

	case CCXL_TY_M128P:
		sz=16; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1;
		break;
		
	default:
		sz=1; nm1=-1; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(sctx->is_addr64)
		{
//			if(sctx->is_addr_x32)
			if(ctx->arch_sizeof_ptr==4)
			{
				sz=4; nm1=BGBCC_SH_NMID_MOVUL; nm2=-1;
			}else
			{
				sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1;
			}
		}else
		{
			sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1;
		}
	}

	BGBCC_JX2C_EmitLoadOp2BRegOfsReg(ctx, sctx,
		nm1, nm2, sreg, ofs, dreg);
	return(1);
}


#if 1
/** Emit logic to copy value-type given by registers and size. */
int BGBCC_JX2C_EmitValueZeroRegSz(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int dreg, int sz, int al)
{
	char *s0;
	int tr0, tr1, tr2, tr3;
	int l0, al0;
	int nm1, nm2;
	int step;
	int i, j, k;

	al0=al;
	if(sctx->has_misalgn)
	{
		/*	if we have misaligned load/store,
			pretend copy is aligned. */
		if((al<4) && !(sz&3))
			al=4;
		if(!(sz&7))
			al=8;
	}

	nm1=BGBCC_SH_NMID_MOVL;
	nm2=BGBCC_SH_NMID_MOVL;
	if(((sz&3)==1) && (al<=1))nm2=BGBCC_SH_NMID_MOVB;
	if(((sz&3)==2) && (al<=2))nm2=BGBCC_SH_NMID_MOVW;

	if(sz<=4)
	{
		if(al>=4)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);
			BGBCC_JX2_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}
		
		if(al==2)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);

			nm1=BGBCC_SH_NMID_MOVW;
			if(sz>2)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1,	tr0, dreg, 0);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, 2);
			}else
			{
				BGBCC_JX2_EmitOpRegStReg(sctx, nm1, tr0, dreg);
			}

			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
		
		nm1=BGBCC_SH_NMID_MOVB;
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);
		for(i=0; i<sz; i++)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
		}
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(sctx->has_pushx2 && (al0>=8) && !(sz&15) && (sz>32) && (sz<=256) &&
		(BGBCC_JX2C_ScratchQueryReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2)>1))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR2);
		tr2=(tr0&31)+0;
		tr3=(tr0&31)+1;
		nm1=BGBCC_SH_NMID_MOVX2;

		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr2);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr3);

		i=0;

		while((i+16)<=sz)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
			i+=16;
		}
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if((al>=8) && (!(sz&7) || (al0>=8)) && (sz<=128))
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
		nm1=BGBCC_SH_NMID_MOVQ;
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);
		i=0;
		while((i+8)<=sz)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
			i+=8;
		}
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(al<8)
	{
		if((al>=4) && (!(sz&3) || (al0>=4)) && (sz<=32))
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			nm1=BGBCC_SH_NMID_MOVL;
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);
			i=0;
			while((i+4)<=sz)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
				i+=4;
			}
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}

		if((al>=2) && !(sz&1) && (sz<=16))
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			nm1=BGBCC_SH_NMID_MOVW;
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);
			i=0;
			while((i+4)<=sz)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
				i+=4;
			}
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}

		if(sz<=8)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			nm1=BGBCC_SH_NMID_MOVB;
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_MOV, 0, tr0);
			i=0;
			while(i<sz)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, nm1, tr0, dreg, i);
				i++;
			}
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}
	}

#if 1
	if((al>=8) && !(sz&15) && !sctx->sreg_live)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
//		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
//			sreg, BGBCC_SH_REG_R5);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R5, 0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
//		BGBCC_JX2C_EmitCallName(ctx, sctx, "__memset_movx");
		BGBCC_JX2C_EmitCallName(ctx, sctx, "memset_movx");
		return(1);
	}
#endif

	if(!sctx->sreg_live)
	{
		BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
			dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R5, 0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R6, sz);
		BGBCC_JX2C_EmitCallName(ctx, sctx, "memset");
		return(1);
	}

#if 1
	if(1)
	{
		/* Kinda stuck, inline copy loop... */

		if(!(sz&7))
			{ nm2=BGBCC_SH_NMID_MOVQ; step=8; }
		else if(!(sz&3))
			{ nm2=BGBCC_SH_NMID_MOVL; step=4; }
		else if(!(sz&1))
			{ nm2=BGBCC_SH_NMID_MOVW; step=2; }
		else
			{ nm2=BGBCC_SH_NMID_MOVB; step=1; }

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		tr2=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//		tr3=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

		l0=BGBCC_JX2_GenLabelTemp(sctx);

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, dreg, tr2);
//		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, sreg, tr3);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr0, 0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, sz/step);

		BGBCC_JX2_EmitLabel(sctx, l0);

		BGBCC_JX2_EmitOpRegStReg(sctx,
			nm2, tr0, tr2);
		BGBCC_JX2_EmitOpImmReg(sctx,
			BGBCC_SH_NMID_ADD, step, tr2);
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_ADD, -1, tr1);
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx, BGBCC_SH_NMID_CMPEQ, 0, tr1);
		BGBCC_JX2_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr2);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr3);
		return(1);
	}
#endif
	
	BGBCC_DBGBREAK
	return(0);
}
#endif
