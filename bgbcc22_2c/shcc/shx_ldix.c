int BGBCC_SHXC_EmitLdix_FillSzNmTy(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	int *rsz, int *rnm1, int *rnm2, int *rnm3, int *rnm4)
{
	int nm1, nm2, nm3, nm4, ty, sz;
	ty=type.val;

	sz=-1; nm1=-1; nm2=-1; nm3=-1; nm4=-1;
	switch(ty)
	{
//	case CCXL_TY_I:		case CCXL_TY_UI:
//	case CCXL_TY_NL:	case CCXL_TY_UNL:
	case CCXL_TY_I:
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;

	case CCXL_TY_UI:
		if(sctx->is_addr64)
		{	sz=4; nm1=BGBCC_SH_NMID_MOVL;
			nm2=BGBCC_SH_NMID_EXTUL;
			nm4=BGBCC_SH_NMID_MOVUL; break; }
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;

	case CCXL_TY_SB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;
	case CCXL_TY_UB:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=BGBCC_SH_NMID_EXTUB;
		nm4=BGBCC_SH_NMID_MOVUB; break;
	case CCXL_TY_SS:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=-1; break;
	case CCXL_TY_US:
		sz=2; nm1=BGBCC_SH_NMID_MOVW; nm2=BGBCC_SH_NMID_EXTUW;
		nm4=BGBCC_SH_NMID_MOVUW; break;

	case CCXL_TY_V:
		sz=1; nm1=BGBCC_SH_NMID_MOVB; nm2=-1; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		if(sctx->is_addr64)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;

	case CCXL_TY_F:
		if(sctx->fpu_soft)
			{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break; }
		sz=4; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_D:
		if(sctx->fpu_soft)
		{
			if(sctx->is_addr64)
				{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
			sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
		}
		sz=8; nm1=BGBCC_SH_NMID_FMOVS; nm2=-1; break;
	case CCXL_TY_L:	case CCXL_TY_UL:
		if(sctx->is_addr64)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;

	case CCXL_TY_F16:
		sz=2; nm1=BGBCC_SH_NMID_MOVW;
		nm2=BGBCC_SH_NMID_LDHF16; break;
	case CCXL_TY_VARIANT:
		if(sctx->is_addr64)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; break; }
		sz=8; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; break;
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(sctx->is_addr64)
			{ sz=8; nm1=BGBCC_SH_NMID_MOVQ; nm2=-1; }
		else
			{ sz=4; nm1=BGBCC_SH_NMID_MOVL; nm2=-1; }
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;
	}
	
	*rsz=sz;
	*rnm1=nm1;
	*rnm2=nm2;
	*rnm3=nm3;
	*rnm4=nm4;
	return(0);
}

int BGBCC_SHXC_EmitLdixVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	ccxl_type tty;
	int csreg, ctreg, cdreg, ctreg2;
	int nm1, nm2, nm3, nm4, ty, sz;
	int i, j, k;

	ty=type.val;

	if(BGBCC_CCXL_TypeFunctionP(ctx, type))
	{
		if(imm)
		{
			BGBCC_CCXL_StubError(ctx);
			return(0);
		}
		
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
			type, dreg, sreg);
		return(1);
	}
	
//	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	if(	BGBCC_CCXL_TypeArrayP(ctx, type) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
//		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		nm1=BGBCC_SH_NMID_MOVL;
//		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		
		if(BGBCC_CCXL_TypeValueObjectP(ctx, type) && (sz&3))
			{ BGBCC_DBGBREAK }
		
		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}


	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

//	nm4=-1;

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
//			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		}

		ctreg2=cdreg;

		if(BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
//			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
//				{ BGBCC_DBGBREAK }
//			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			ctreg2=BGBCC_SH_REG_R1;
		}

		if(nm4>=0)
		{
			i=BGBCC_SHX_TryEmitOpLdRegDispReg(sctx, nm4,
				csreg, imm*sz, ctreg2);
			if(i>0)
			{
				if(nm3>=0)
					{ BGBCC_SHX_EmitOpRegReg(sctx, nm3, ctreg2, cdreg); }

				if(ctreg2!=cdreg)
					BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
					BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}
		
		BGBCC_SHXC_EmitLoadBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, ctreg2);
		if(nm2>=0)
			{ BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg2, cdreg); }

		if(ctreg2!=cdreg)
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdixVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	ccxl_type tty;
	int csreg, ctreg, cdreg, ctreg2;
	int nm1, nm2, nm3, nm4, ty, sz, asz, bsz;
	int i, j, k;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		i=BGBCC_SHXC_EmitLdixVRegVRegImm(ctx, sctx, type, dreg, sreg, j);
		return(i);
	}

	ty=type.val;

//	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	if(BGBCC_CCXL_TypeArrayP(ctx, type) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
//		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			nm1=BGBCC_SH_NMID_MOVL;
			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
				nm1, cdreg, ctreg, sz, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		nm1=BGBCC_SH_NMID_MOVL;
//		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

//	nm4=-1;

	if(nm1>=0)
	{
//		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
//			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
//				__debugbreak();

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			ctreg2=cdreg;

			if(BGBCC_CCXL_TypeFloat16P(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
				ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			}
			
			BGBCC_SHXC_EmitLoadBRegIRegScReg(ctx, sctx,
				nm1, cdreg, ctreg, sz, ctreg2);
			if(nm2>=0)
				{ BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg2, cdreg); }

			if(ctreg2!=cdreg)
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		ctreg2=cdreg;

		if(BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_SHX_EmitOpRegReg(sctx,
//				BGBCC_SH_NMID_STHF16, csreg, ctreg);
		}

		if(sctx->has_bjx1mov && (nm4>=0) &&
			(csreg!=BGBCC_SH_REG_R0) &&
			(ctreg!=BGBCC_SH_REG_R0) &&
//			BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, cdreg) &&
			BGBCC_SHXC_CheckNmidScaleMatch(ctx, sctx, nm1, sz))
		{
			i=BGBCC_SHX_TryEmitOpLdReg2Reg(sctx, nm4,
				csreg, ctreg, cdreg);

			if(i>0)
			{
				if(nm3>=0)
					{ BGBCC_SHX_EmitOpRegReg(sctx, nm3, ctreg2, cdreg); }
				if(ctreg2!=cdreg)
					BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}
		
		BGBCC_SHXC_EmitLoadBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, ctreg2);
		if(nm2>=0)
			{ BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg2, cdreg); }

		if(ctreg2!=cdreg)
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}



#if 1
int BGBCC_SHXC_EmitLdixVRegVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg, int imm)
{
	ccxl_type tty;
	int csreg, ctreg, cdreg, ctreg2;
	int nm1, nm2, nm3, nm4, ty, sz, asz, bsz;
	int i, j, k;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		i=BGBCC_SHXC_EmitLdixVRegVRegImm(ctx, sctx, type, dreg, sreg, j+imm);
		return(i);
	}

	ty=type.val;

#if 0
//	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	if(BGBCC_CCXL_TypeArrayP(ctx, type) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
//		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			nm1=BGBCC_SH_NMID_MOVL;
			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
				nm1, cdreg, ctreg, sz, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		nm1=BGBCC_SH_NMID_MOVL;
//		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
#endif

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
//		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
//			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
//				__debugbreak();

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			ctreg2=cdreg;

			if(BGBCC_CCXL_TypeFloat16P(ctx, type))
			{
				if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
					{ BGBCC_DBGBREAK }
				ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			}
			
			BGBCC_SHXC_EmitLoadBRegIRegScDispReg(ctx, sctx,
				nm1, cdreg, ctreg, sz, imm*sz, ctreg2);
			if(nm2>=0)
				{ BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg2, cdreg); }

			if(ctreg2!=cdreg)
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		ctreg2=cdreg;

		if(BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
//			BGBCC_SHX_EmitOpRegReg(sctx,
//				BGBCC_SH_NMID_STHF16, csreg, ctreg);
		}

		BGBCC_SHXC_EmitLoadBRegIRegScDispReg(ctx, sctx,
			nm1, csreg, ctreg, sz, imm*sz, ctreg2);
		if(nm2>=0)
			{ BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg2, cdreg); }

		if(ctreg2!=cdreg)
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
#endif


int BGBCC_SHXC_EmitStixVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

#if 0
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_DBGBREAK
		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

	ty=type.val;

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(nm2==BGBCC_SH_NMID_LDHF16)
		nm2=BGBCC_SH_NMID_STHF16;


	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;

		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		
		al=4;
		if(sz&2)al=2;
		if(sz&1)al=1;
		
		if(imm)
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
				nm1, cdreg, imm*sz, ctreg);
			BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, ctreg, csreg, sz, al);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
		}else
		{
			BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, cdreg, csreg, sz, al);
		}

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=csreg;

		if(nm4>=0)
		{
			i=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, nm4,
				csreg, cdreg, imm*sz);
			if(i>0)
			{
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}
		
#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, type))
//			BGBCC_SHX_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, csreg);
		BGBCC_SHXC_EmitDebugCheckReg(ctx, sctx, type, csreg);
#endif
		
		if(BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
//			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
//				{ BGBCC_DBGBREAK }
//			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			ctreg=BGBCC_SH_REG_R1;
			BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_STHF16, csreg, ctreg);
		}

		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
			nm1, cdreg, imm*sz, ctreg);

//		if(ctreg!=csreg)
//			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStixVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg, ctreg2;
	int tr0;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		i=BGBCC_SHXC_EmitStixVRegVRegImm(ctx, sctx, type, dreg, treg, j);
		return(i);
	}

#if 0
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_DBGBREAK
		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(nm2==BGBCC_SH_NMID_LDHF16)
		nm2=BGBCC_SH_NMID_STHF16;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;

		al=4;
		if(sz&2)al=2;
		if(sz&1)al=1;

		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		
//		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, csreg);

//		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, ctreg);

		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, cdreg, csreg, sz, tr0);

		BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, tr0, ctreg, sz, al);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		ctreg2=ctreg;

		if(BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		}

#if 1
		if(nm4>=0)
		{
			i=BGBCC_SHX_TryEmitOpRegStReg2(sctx, nm4,
				ctreg, csreg, cdreg);
			if(i>0)
			{
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}
#endif

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, type))
//			BGBCC_SHX_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, ctreg);
		BGBCC_SHXC_EmitDebugCheckReg(ctx, sctx, type, ctreg);
#endif
		
		if(nm2>=0)
			BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg, ctreg2);

		BGBCC_SHXC_EmitStoreBRegIRegScReg(ctx, sctx,
			nm1, cdreg, csreg, sz, ctreg);

		if(ctreg2!=ctreg)
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

#if 1
int BGBCC_SHXC_EmitStixVRegVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg, int imm)
{
	int csreg, ctreg, cdreg, ctreg2;
	int tr0;
	int nm1, nm2, nm3, nm4, ty, sz, al;
	int i, j, k;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		i=BGBCC_SHXC_EmitStixVRegVRegImm(ctx, sctx, type, dreg, treg, j+imm);
		return(i);
	}

#if 0
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_DBGBREAK
		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(nm2==BGBCC_SH_NMID_LDHF16)
		nm2=BGBCC_SH_NMID_STHF16;


#if 0
	if(BGBCC_CCXL_TypeValueObjectP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		nm1=BGBCC_SH_NMID_MOVL; nm2=-1;

		al=4;
		if(sz&2)al=2;
		if(sz&1)al=1;

		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		
//		BGBCC_SHXC_EmitStoreBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, csreg);

//		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
//			nm1, cdreg, imm*sz, ctreg);

		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, cdreg, csreg, sz, tr0);

		BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, tr0, ctreg, sz, al);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}
#endif

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		ctreg2=ctreg;

		if(BGBCC_CCXL_TypeFloat16P(ctx, type))
		{
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		}
		
#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, type))
//			BGBCC_SHX_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, ctreg);
		BGBCC_SHXC_EmitDebugCheckReg(ctx, sctx, type, ctreg);
#endif
		
		if(nm2>=0)
			BGBCC_SHXC_EmitConvOpRegReg(ctx, sctx, nm2, ctreg, ctreg2);
		
		BGBCC_SHXC_EmitStoreBRegIRegScDispReg(ctx, sctx,
			nm1, cdreg, csreg, sz, imm*sz, ctreg);

		if(ctreg2!=ctreg)
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
#endif


int BGBCC_SHXC_EmitLeaVRegVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, s32 imm)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz;
	int i, j, k;

	ty=type.val;

//	if(BGBCC_CCXL_TypeArrayP(ctx, type))
//	{
//		i=BGBCC_SHXC_EmitLeaVRegVRegImm(ctx, sctx, type, dreg, sreg, imm);
//		return(i);
//	}

	if(	BGBCC_CCXL_TypeArrayP(ctx, type) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		nm1=BGBCC_SH_NMID_MOVL;
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		
		if(BGBCC_CCXL_TypeValueObjectP(ctx, type) && (sz&3))
			{ BGBCC_DBGBREAK }
			
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
				nm1, cdreg, imm*sz, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
	
	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
//			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		}
		
//		if(cdreg==csreg)
//			__debugbreak();
		
		BGBCC_SHXC_EmitLeaBRegOfsReg(ctx, sctx,
			nm1, csreg, imm*sz, cdreg);
//		if(nm2>=0)
//			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLeaVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4, ty, sz;
	int i, j, k;

	ty=type.val;

//	if(BGBCC_CCXL_TypeArrayP(ctx, type))
//	{
//		i=BGBCC_SHXC_EmitLeaVRegVRegVReg(ctx, sctx, type, dreg, sreg, treg);
//		return(i);
//	}

//	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
//		BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
//			__debugbreak();

	if(BGBCC_CCXL_TypeArrayP(ctx, type) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			nm1=BGBCC_SH_NMID_MOVL;
			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
				nm1, cdreg, ctreg, sz, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
				__debugbreak();

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		nm1=BGBCC_SH_NMID_MOVL;
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

			BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
				nm1, cdreg, ctreg, sz, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
	
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
				__debugbreak();

//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

//		if(cdreg==csreg)
//			__debugbreak();
//		if(csreg==ctreg)
//			__debugbreak();
//		if(cdreg==ctreg)
//			__debugbreak();

		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
			nm1, csreg, ctreg, sz, cdreg);
//		if(nm2>=0)
//			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLdaVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;

//	cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
	cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
	BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx, sreg, cdreg);
	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
	return(1);
}


int BGBCC_SHXC_EmitDiffPtrVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	ccxl_register sreg, ccxl_register treg)
{
	ccxl_register treg2;
	ccxl_type tty;
	int csreg, ctreg, cdreg, ctreg2;
	int nm1, nm2, nm3, nm4, ty, sz, shl;
	int i, j, k;

	ty=type.val;

	BGBCC_SHXC_EmitLdix_FillSzNmTy(ctx, sctx, type,
		&sz, &nm1, &nm2, &nm3, &nm4);
	if(sz&(sz-1))			nm1=-1;
	if((sz<=0) || (sz>4))	nm1=-1;

	if(nm1>=0)
	{
		shl=0; j=sz;
		while(j>1)
		 { shl++; j=j>>1; }
	
		switch(sz)
		{
		case 1: nm1=-1; nm2=-1; break;
//		case 2: nm1=BGBCC_SH_NMID_SHLR; nm2=-1; break;
//		case 4: nm1=BGBCC_SH_NMID_SHLR2; nm2=-1; break;
//		case 8: nm1=BGBCC_SH_NMID_SHLR2;
//			nm2=BGBCC_SH_NMID_SHLR; break;
//		case 16: nm1=BGBCC_SH_NMID_SHLR2;
//			nm2=BGBCC_SH_NMID_SHLR2; break;

		case 2: nm1=BGBCC_SH_NMID_SHAR; nm2=-1; break;
		case 4: nm1=BGBCC_SH_NMID_SHAR; nm2=BGBCC_SH_NMID_SHAR; break;

		default:
			BGBCC_DBGBREAK
			nm1=-1; nm2=-1;
			break;
		}

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
				__debugbreak();

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
//		BGBCC_SHXC_EmitLeaBRegIRegScReg(ctx, sctx,
//			nm1, csreg, ctreg, sz, cdreg);
//		if(nm2>=0)
//			{ BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, cdreg); }

#if 1
		i=BGBCC_SHX_TryEmitOpRegRegReg(sctx, BGBCC_SH_NMID_SUB,
			csreg, ctreg, cdreg);
		if(i>0)
		{
			if(shl>0)
			{
				if(sctx->is_addr64)
				{
					j=BGBCC_SHX_TryEmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHADQ,
						cdreg, -shl, cdreg);
					if(j<=0)
					{
						ctreg2=BGBCC_SHXC_ScratchAllocReg(
							ctx, sctx, BGBCC_SH_REGCLS_QGR);
						BGBCC_SHX_EmitLoadRegImm(sctx,
							BGBCC_SH_NMID_MOV, ctreg2, -shl);
						BGBCC_SHX_EmitOpRegReg(sctx,
							BGBCC_SH_NMID_SHADQ, ctreg2, cdreg);
						BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
					}
				}
				else
				{
					j=BGBCC_SHX_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHAD,
						cdreg, -shl, cdreg);
					if(j<=0)
					{
						ctreg2=BGBCC_SHXC_ScratchAllocReg(
							ctx, sctx, BGBCC_SH_REGCLS_GR);
						BGBCC_SHX_EmitLoadRegImm(sctx,
							BGBCC_SH_NMID_MOV, ctreg2, -shl);
						BGBCC_SHX_EmitOpRegReg(sctx,
							BGBCC_SH_NMID_SHAD, ctreg2, cdreg);
						BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
					}
				}
			}
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

		i=BGBCC_SHX_TryEmitOpRegRegReg(sctx, BGBCC_SH_NMID_SUB,
			csreg, ctreg, cdreg);
		if(i<=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ctreg, cdreg);
		}

		if(shl==0)
		{
			/* Do Nothing */
		}else if((nm1>=0) && !sctx->is_addr64)
		{
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
			if(nm2>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg);
		}else if(sctx->has_shad)
		{
			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg2, -shl);
			if(sctx->is_addr64)
			{
				BGBCC_SHXC_CheckSetModeDqSet(ctx, sctx);
				BGBCC_SHX_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_SHADQ, ctreg2, cdreg);
			}
			else
			{
				BGBCC_SHX_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_SHAD, ctreg2, cdreg);
			}
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
		}else
		{
			BGBCC_SHXC_EmitLeaShrRegImm(ctx, sctx, cdreg, shl);
//			BGBCC_CCXL_StubError(ctx);
		}

//		if(nm1>=0)
//			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
//		if(nm2>=0)
//			BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
	if(sz<=0)
	{
		BGBCC_DBGBREAK
		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	if(!(sz&(sz-1)))
	{
		shl=0; j=sz;
		while(j>1)
		 { shl++; j=j>>1; }
	
		switch(sz)
		{
		case 1: nm1=-1; nm2=-1; break;
		case 2: nm1=BGBCC_SH_NMID_SHAR; nm2=-1; break;
		case 4: nm1=BGBCC_SH_NMID_SHAR; nm2=BGBCC_SH_NMID_SHAR; break;

//		case 4: nm1=BGBCC_SH_NMID_SHLR2; nm2=-1; break;
//		case 8: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR; break;
//		case 16: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2; break;
		default:
			nm1=-1; nm2=-1;
			break;
		}

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
				__debugbreak();

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

#if 1
		i=BGBCC_SHX_TryEmitOpRegRegReg(sctx, BGBCC_SH_NMID_SUB,
			csreg, ctreg, cdreg);
		if(i>0)
		{
			if(shl>0)
			{
				if(sctx->is_addr64)
				{
					j=BGBCC_SHX_TryEmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHADQ,
						cdreg, -shl, cdreg);
					if(j<=0)
					{
						ctreg2=BGBCC_SHXC_ScratchAllocReg(
							ctx, sctx, BGBCC_SH_REGCLS_QGR);
						BGBCC_SHX_EmitLoadRegImm(sctx,
							BGBCC_SH_NMID_MOV, ctreg2, -shl);
						BGBCC_SHX_EmitOpRegReg(sctx,
							BGBCC_SH_NMID_SHADQ, ctreg2, cdreg);
						BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
					}
				}else
				{
					BGBCC_SHX_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHAD,
						cdreg, -shl, cdreg);
				}
			}
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

		i=BGBCC_SHX_TryEmitOpRegRegReg(sctx, BGBCC_SH_NMID_SUB,
			csreg, ctreg, cdreg);
		if(i<=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ctreg, cdreg);
		}

		if(nm1>=0)
		{
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
			if(nm2>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg);
		}else if(sctx->has_shad && !sctx->is_addr64)
		{
			ctreg2=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg2, -shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD, ctreg2, cdreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg2);
		}else
		{
			BGBCC_SHXC_EmitLeaShrRegImm(ctx, sctx, cdreg, shl);
//			BGBCC_CCXL_StubError(ctx);
		}

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	if(1)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
				__debugbreak();

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ctreg, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		
		BGBCC_CCXL_GetRegForIntValue(ctx, &treg2, sz);
		tty.val=0;
		BGBCC_SHXC_EmitBinaryVRegVRegInt(ctx, sctx,
			tty, dreg, CCXL_BINOP_DIV, treg2);
		
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitLeaShrRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg, int shl)
{
	int nm1, nm2, nm3, nm4;

	if(shl<0)
		return(0);
	if(!shl)
		return(1);

	nm1=-1; nm2=-1; nm3=-1; nm4=-1;
	switch(shl)
	{
	case 0:
		break;
	case 1:
		nm1=BGBCC_SH_NMID_SHLR;		break;
	case 2:
		nm1=BGBCC_SH_NMID_SHLR2;	break;
	case 3:
		nm1=BGBCC_SH_NMID_SHLR2;	nm2=BGBCC_SH_NMID_SHLR;		break;
	case 4:
		nm1=BGBCC_SH_NMID_SHLR2;	nm2=BGBCC_SH_NMID_SHLR2;	break;
	case 5:
		nm1=BGBCC_SH_NMID_SHLR2;	nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR;		break;
	case 6:
		nm1=BGBCC_SH_NMID_SHLR2;	nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR2;	break;
	case 7:
		nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR2; nm3=BGBCC_SH_NMID_SHLR; break;
//	case 6: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLR2; break;
//	case 7:
//		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHAR;		break;
	case 8:
		nm1=BGBCC_SH_NMID_SHLR8;	break;
	case 9:
		nm1=BGBCC_SH_NMID_SHLR8;	nm2=BGBCC_SH_NMID_SHLR;		break;
	case 10:
		nm1=BGBCC_SH_NMID_SHLR8;	nm2=BGBCC_SH_NMID_SHLR2;	break;
	case 11:
		nm1=BGBCC_SH_NMID_SHLR8;	nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR;		break;
	case 12:
		nm1=BGBCC_SH_NMID_SHLR8;	nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR2;	break;
	case 13:
		nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR;		break;
	case 14:
		nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2;
		nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR2;		break;

//	case 15: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHAR; break;
	case 16: nm1=BGBCC_SH_NMID_SHLR16; break;

	default: break;
	}

#if 1
	if(sctx->has_shad)
	{
		if((nm3>=0) && (dreg!=BGBCC_SH_REG_R0))
		{
			BGBCC_SHXC_CheckSetModeDqClear(ctx, sctx);
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, -shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD,
				BGBCC_SH_REG_R0, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}
#endif

	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, dreg);
	if(nm2>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm2, dreg);
	if(nm3>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm3, dreg);
	if(nm4>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm4, dreg);

	return(1);
}


int BGBCC_SHXC_EmitLeaShlRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg, int shl)
{
	int nm1, nm2, nm3, nm4;

	if(shl<0)
		return(0);
	if(!shl)
		return(1);

#if 0
//	if(sctx->is_addr64 &&
//		!BGBCC_SHX_EmitCheckRegGPR(sctx, dreg))
	if(sctx->is_addr64)
	{
//		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, shl);
//		BGBCC_SHX_EmitOpRegRegReg(sctx, BGBCC_SH_NMID_SHAD,
//			dreg, BGBCC_SH_REG_R0, dreg);

		BGBCC_SHXC_SetModeDqSet(ctx, sctx);
		BGBCC_SHX_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHADQ,
			dreg, shl, dreg);
		return(1);
	}
#endif

	nm1=-1; nm2=-1; nm3=-1; nm4=-1;
	switch(shl)
	{
	case 0:
		break;
	case 1:
		nm1=BGBCC_SH_NMID_SHLL1;	break;
	case 2:
		nm1=BGBCC_SH_NMID_SHLL2;	break;
	case 3:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL1;	break;
	case 4:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL2;	break;
	case 5:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL1;	break;
	case 6:
		nm1=BGBCC_SH_NMID_SHLL2;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2;	break;
//	case 7:
//		nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2;
//		nm3=BGBCC_SH_NMID_SHLL2; nm3=BGBCC_SH_NMID_SHLL1; break;
//	case 6: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLR2; break;
	case 7:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHAR;		break;
	case 8:
		nm1=BGBCC_SH_NMID_SHLL8;	break;
	case 9:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL1;	break;
	case 10:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL2;	break;
	case 11:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL1;	break;
	case 12:
		nm1=BGBCC_SH_NMID_SHLL8;	nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2;	break;
	case 13:
		nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2; nm4=BGBCC_SH_NMID_SHLL1;		break;
	case 14:
		nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2;
		nm3=BGBCC_SH_NMID_SHLL2; nm4=BGBCC_SH_NMID_SHLL2;		break;

	case 15: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHAR; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL16; break;

	default: break;
	}

#if 1
	if(sctx->has_shad)
	{
		if(BGBCC_SHX_EmitCheckRegNonBaseGPR(sctx, dreg))
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHADQ,
				BGBCC_SH_REG_R0, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	
		if((nm3>=0) && (dreg!=BGBCC_SH_REG_R0))
		{
			if(sctx->is_addr64)
			{
				BGBCC_SHXC_CheckSetModeDqSet(ctx, sctx);
//				if(BGBCC_SHX_EmitCheckModeDQ(sctx) && 
//					sctx->has_bjx1ari)
//				if(sctx->has_bjx1ari)
				if(sctx->has_bjx1ari && sctx->has_bjx1mov)
//				if(sctx->has_bjx1ari &&
//					BGBCC_SHX_EmitCheckModeSetDQ(sctx, BGBCC_SH_NMID_SHADQ))
				{
					BGBCC_SHX_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_SHADQ,
						dreg, shl, dreg);
				}else
				{
					BGBCC_SHXC_CheckSetModeDqSet(ctx, sctx);
					BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
					BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
						BGBCC_SH_REG_R0, shl);
//					BGBCC_SHX_EmitOpRegRegReg(sctx, BGBCC_SH_NMID_SHADQ,
//						dreg, BGBCC_SH_REG_R0, dreg);
					BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHADQ,
						BGBCC_SH_REG_R0, dreg);
					BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
				}
				return(1);
			}

			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD,
				BGBCC_SH_REG_R0, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}
#endif

	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, dreg);
	if(nm2>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm2, dreg);
	if(nm3>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm3, dreg);
	if(nm4>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm4, dreg);

	return(1);
}

int BGBCC_SHXC_EmitLeaBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int treg;
	int opw1, opw2;
	int nm1, nm2, nm3;
	int p0, p1, shl, bs, cbo, cmo;
	int i, j, k;

	switch(sc)
	{
	case 1: shl=0; bs=2; break;
	case 2: shl=1; bs=2; break;
	case 3: shl=0; bs=3; break;
	case 4: shl=2; bs=2; break;
	case 5: shl=0; bs=5; break;
	case 6: shl=1; bs=3; break;
	case 7: shl=0; bs=7; break;

	case 8: shl=3; bs=2; break;
	case 9: shl=0; bs=9; break;
	case 10: shl=1; bs=5; break;
	case 11: shl=0; bs=11; break;
	case 12: shl=2; bs=3; break;
	case 13: shl=0; bs=13; break;
	case 14: shl=1; bs=7; break;
	case 15: shl=0; bs=15; break;
	
	case 16: shl=4; bs=2; break;
	case 17: shl=0; bs=17; break;
	case 18: shl=1; bs=9; break;
	case 20: shl=2; bs=5; break;
	case 22: shl=1; bs=11; break;
	case 24: shl=3; bs=3; break;
	case 26: shl=1; bs=13; break;
	case 28: shl=2; bs=7; break;
	case 30: shl=1; bs=15; break;

	case 32: shl=5; bs=2; break;
	case 34: shl=1; bs=17; break;
	case 36: shl=2; bs=9; break;
	case 40: shl=3; bs=5; break;
	case 44: shl=2; bs=11; break;
	case 48: shl=4; bs=3; break;
	case 52: shl=2; bs=13; break;
	case 56: shl=3; bs=7; break;
	case 60: shl=2; bs=15; break;

	case 64: shl=6; bs=2; break;
	case 68: shl=2; bs=17; break;
	case 72: shl=3; bs=9; break;
	case 80: shl=4; bs=5; break;
	case 88: shl=3; bs=11; break;
	case 96: shl=5; bs=3; break;
	case 104: shl=3; bs=13; break;
	case 112: shl=4; bs=7; break;
	case 120: shl=3; bs=15; break;

	case 128: shl=7; bs=2; break;
	case 176: shl=4; bs=11; break;
	case 192: shl=6; bs=3; break;
	case 136: shl=3; bs=17; break;
	case 144: shl=4; bs=9; break;
	case 208: shl=4; bs=13; break;
	case 240: shl=4; bs=15; break;

	case 256: shl=8; bs=2; break;
	case 272: shl=4; bs=17; break;
	case 384: shl=7; bs=3; break;

	case 512: shl=9; bs=2; break;
	case 768: shl=8; bs=3; break;

	case 1024: shl=10; bs=2; break;
	case 2048: shl=11; bs=2; break;
	case 4096: shl=12; bs=2; break;
	case 8192: shl=13; bs=2; break;
	case 16384: shl=14; bs=2; break;
	case 32768: shl=15; bs=2; break;
	case 65536: shl=16; bs=2; break;

	default: shl=-1; bs=-1; break;
	}
	
	switch(bs)
	{
	case 2: cbo=1; break;
	case 3: cbo=3; break;
	case 5: cbo=3; break;
	case 7: cbo=4; break;
	case 9: cbo=4; break;
	case 11: cbo=5; break;
	case 13: cbo=5; break;
	case 15: cbo=6; break;
	case 17: cbo=6; break;
	default: cbo=-999; break;
	}
	switch(shl)
	{
	case 0: cbo+=0; break;
	case 1: cbo+=1; break;
	case 2: cbo+=1; break;
	case 3: cbo+=2; break;
	case 4: cbo+=2; break;
//	case 5: cbo+=3; break;
//	case 6: cbo+=3; break;
	case 7: cbo+=2; break;
	case 8: cbo+=1; break;
	case 9: cbo+=2; break;
//	case 10: cbo+=3; break;
//	case 11: cbo+=3; break;
//	case 12: cbo+=3; break;
//	default: cbo+=4; break;

	default: cbo+=2; break;
	}
	
	cmo=3;
	if(sc>=128)cmo+=3;
	
	if((dreg==ireg) && (breg!=dreg) && (bs==2))
		cbo-=1;
	if((dreg==ireg) && (bs!=2))
		shl=-1;
		
//	if(bs!=2)		//Debug
//		shl=-1;

	if((shl>=0) && (bs==2) && (cbo<=cmo))
	{
		opw1=-1;
		switch(shl)
		{
			case 0: opw1=BGBCC_SH_NMID_LEAB; break;
			case 1: opw1=BGBCC_SH_NMID_LEAW; break;
			case 2: opw1=BGBCC_SH_NMID_LEAL; break;
			case 3: opw1=BGBCC_SH_NMID_LEAQ; break;
			default:	break;
		}
		if((opw1>0) && (breg!=BGBCC_SH_REG_R0))
		{
			if((breg==dreg) && (shl==0) && !(sctx->is_addr64))
			{
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
				return(1);
			}

			i=BGBCC_SHX_TryEmitOpLdReg2Reg(sctx,
				opw1, breg, ireg, dreg);
			if(i>0)
				return(1);
		}
		
//		if(sctx->has_bjx1ari && !sctx->is_addr64)
//		if(sctx->has_bjx1ari)
		if(sctx->has_bjx1ari && sctx->has_bjx1mov)
		{
			if(breg==dreg)
			{
				treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
				if(shl)
				{	BGBCC_SHX_EmitOpRegImmReg(sctx,
						BGBCC_SH_NMID_SHAD, ireg, shl, treg);	}
				else
				{	BGBCC_SHX_EmitOpRegReg(sctx,
						BGBCC_SH_NMID_MOV, ireg, treg);			}
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}else
			{
				if(shl)
				{	BGBCC_SHX_EmitOpRegImmReg(sctx,
						BGBCC_SH_NMID_SHAD, ireg, shl, dreg);	}
				else
				{	BGBCC_SHX_EmitOpRegReg(sctx,
						BGBCC_SH_NMID_MOV, ireg, dreg);			}
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
				return(1);
			}
		}
	
		if(breg==dreg)
		{
//			treg=BGBCC_SH_REG_R1;
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, treg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, treg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		}
		return(1);
	}

	if((shl>=0) && (bs==3) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
//			treg=BGBCC_SH_REG_R1;
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, treg);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL1, treg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, treg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, treg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL1, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}

	if((shl>=0) && (bs==5) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
//			treg=BGBCC_SH_REG_R1;
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, treg);
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, treg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, treg);
			BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, treg, shl);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}

#if 0
	if((shl>=0) && ((bs==7)||(bs==9)) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				breg, BGBCC_SH_REG_R1);
			breg=BGBCC_SH_REG_R1;
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL1, dreg);
		if(bs==7)
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ireg, dreg);
		else
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}

	if((shl>=0) && ((bs==11)||(bs==13)) && (cbo<=cmo))
	{
		if(breg==dreg)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				breg, BGBCC_SH_REG_R1);
			breg=BGBCC_SH_REG_R1;
		}

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL1, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL2, dreg);
		if(bs==11)
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB, ireg, dreg);
		else
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ireg, dreg);
		BGBCC_SHXC_EmitLeaShlRegImm(ctx, sctx, dreg, shl);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		return(1);
	}
#endif

//	if(shl<0)
	if(1)
	{
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//			ireg, dreg);

		treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

		if(breg==dreg)
		{
			i=BGBCC_SHX_TryEmitOpRegImmReg(sctx, BGBCC_SH_NMID_MULL,
				ireg, sc, treg);
			if(i>0)
			{
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
					treg, dreg);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}
		}else
		{
			i=BGBCC_SHX_TryEmitOpRegImmReg(sctx, BGBCC_SH_NMID_MULL,
				ireg, sc, dreg);
			if(i>0)
			{
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}
		}

#if 1
		if(sctx->has_bjx1ari && sctx->has_bjx1egpr)
		{
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				treg, sc);
			BGBCC_SHX_EmitOpRegRegReg(sctx, BGBCC_SH_NMID_MULL,
				ireg, treg, treg);
			BGBCC_SHX_EmitOpRegRegReg(sctx, BGBCC_SH_NMID_ADD,
				breg, treg, dreg);
//			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, treg, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}
#endif

		BGBCC_SHXC_SetModeDqClear(ctx, sctx);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			treg, sc);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MULL,
			ireg, treg);
		if(breg==dreg)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACL, treg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
				treg, dreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACL, dreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
		}
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
