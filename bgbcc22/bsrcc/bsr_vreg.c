

int BGBCC_BSRC_EmitOpNone(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpNone(sctx, nmid);
	else
		BGBCC_BSR_EmitOpNone(sctx, nmid);
	return(0);
}

int BGBCC_BSRC_EmitOpReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int reg)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpReg(sctx, nmid, reg);
	else
		BGBCC_BSR_EmitOpReg(sctx, nmid, reg);
	return(0);
}

int BGBCC_BSRC_EmitOpImm(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int imm)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpImm(sctx, nmid, imm);
	else
		BGBCC_BSR_EmitOpImm(sctx, nmid, imm);
	return(0);
}

int BGBCC_BSRC_EmitOpMReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int reg)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpMReg(sctx, nmid, reg);
	else
		BGBCC_BSR_EmitOpMReg(sctx, nmid, reg);
	return(0);
}

int BGBCC_BSRC_EmitOpRegReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegReg(sctx, nmid, rm, rn);
	else
		BGBCC_BSR_EmitOpRegReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpRegStReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegStReg(sctx, nmid, rm, rn);
	else
		BGBCC_BSR_EmitOpRegStReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpLdRegReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpLdRegReg(sctx, nmid, rm, rn);
	else
		BGBCC_BSR_EmitOpLdRegReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpRegStDecReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegStDecReg(sctx, nmid, rm, rn);
	else
		BGBCC_BSR_EmitOpRegStDecReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpLdIncRegReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpLdIncRegReg(sctx, nmid, rm, rn);
	else
		BGBCC_BSR_EmitOpLdIncRegReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpRegStRegDisp(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int rn, int disp)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegStRegDisp(sctx, nmid, rm, rn, disp);
	else
		BGBCC_BSR_EmitOpRegStRegDisp(sctx, nmid, rm, rn, disp);
	return(0);
}

int BGBCC_BSRC_EmitOpLdRegDispReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int disp, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpLdRegDispReg(sctx, nmid, rm, disp, rn);
	else
		BGBCC_BSR_EmitOpLdRegDispReg(sctx, nmid, rm, disp, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpRegStReg2(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int ro, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegStReg2(sctx, nmid, rm, ro, rn);
	else
		BGBCC_BSR_EmitOpRegStReg2(sctx, nmid, rm, ro, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpLdReg2Reg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int ro, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpLdReg2Reg(sctx, nmid, rm, ro, rn);
	else
		BGBCC_BSR_EmitOpLdReg2Reg(sctx, nmid, rm, ro, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpRegStReg2Disp(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegStReg2Disp(sctx, nmid, rm, ro, rn, disp);
	else
		BGBCC_BSR_EmitOpRegStReg2Disp(sctx, nmid, rm, ro, rn, disp);
	return(0);
}

int BGBCC_BSRC_EmitOpLdReg2DispReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpLdReg2DispReg(sctx, nmid, rm, ro, disp, rn);
	else
		BGBCC_BSR_EmitOpLdReg2DispReg(sctx, nmid, rm, ro, disp, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpRegRegReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rs, int rt, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegRegReg(sctx, nmid, rs, rt, rn);
	else
		BGBCC_BSR_EmitOpRegRegReg(sctx, nmid, rs, rt, rn);
	return(0);
}

int BGBCC_BSRC_EmitOpImmReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int imm, int reg)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpImmReg(sctx, nmid, imm, reg);
	else
		BGBCC_BSR_EmitOpImmReg(sctx, nmid, imm, reg);
	return(0);
}

int BGBCC_BSRC_EmitOpRegImmReg(
	BGBCC_TransState *ctx, BGBCC_BSR_Context *sctx,
	int nmid, int rm, int imm, int rn)
{
	if(sctx->use_emitqueue)
		BGBCC_BSR_QueueOpRegImmReg(sctx, nmid, rm, imm, rn);
	else
		BGBCC_BSR_EmitOpRegImmReg(sctx, nmid, rm, imm, rn);
	return(0);
}





int BGBCC_BSRC_EmitMovRegReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int sreg, int dreg)
{
	int sreg2, dreg2, sw;

	if(sreg==dreg)
		return(1);

	if(BGBCC_BSRC_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		return(1);
	}
	
	if(BGBCC_BSR_EmitCheckRegExtGPR(sctx, sreg) &&
		BGBCC_BSR_EmitCheckRegExtGPR(sctx, dreg))
	{
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_BSRC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_BSRC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_LDS, sreg, BGBCC_SH_REG_FPUL);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg);
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		BGBCC_BSRC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FLDS, sreg, BGBCC_SH_REG_FPUL);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg);
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
//		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sreg2=BGBCC_SH_REG_FR0+(sreg&15);
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
	
		if(BGBCC_BSRC_GetStatusFpscrSz(ctx, sctx))
		{
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg2, dreg2);
		}else
		{
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg2+0, dreg2+0);
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg2+1, dreg2+1);
		}
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
//		sreg2=BGBCC_SH_REG_R0+(sreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		sreg2=BGBCC_BSR_GETLREG_LO(sreg);
		dreg2=BGBCC_BSR_GETLREG_LO(dreg);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg2+0, dreg2+0);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg2+1, dreg2+1);
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_R0+(sreg&15);
//		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
		sw=(sctx->is_le!=0);

		BGBCC_BSRC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_LDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));

		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_LDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));
		return(1);
	}

	if(BGBCC_BSRC_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_BSRC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
		sreg2=BGBCC_SH_REG_FR0+(sreg&15);
		sw=(sctx->is_le!=0);

		BGBCC_BSRC_ClearStatusFpscrSz(ctx, sctx);
		
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FLDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FLDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));

		return(1);
	}

	printf("BGBCC_BSRC_EmitMovRegReg: "
		"Bad MOV Regs Rm=%02X Rn=%02X\n", sreg, dreg);
	BGBCC_CCXL_StubError(ctx);
	return(0);

//	nm=BGBCC_SH_NMID_MOV;
//	if(BGBCC_BSRC_EmitRegIsFpReg(ctx, sctx, creg))
//		nm=BGBCC_SH_NMID_FMOV;
//	BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm, creg, dreg);
//	BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
//	return(1);
}

int BGBCC_BSRC_EmitLoadVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int creg, nm, pr;

#if 0
	if(BGBCC_CCXL_IsRegImmStringP(ctx, sreg))
	{
		BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
		return(1);
	}
#endif
	
	creg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
//		nm=BGBCC_SH_NMID_MOV;
//		if(BGBCC_BSRC_EmitRegIsFpReg(ctx, sctx, creg))
//			nm=BGBCC_SH_NMID_FMOV;
//		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm, creg, dreg);
		BGBCC_BSRC_EmitMovRegReg(ctx, sctx, creg, dreg);

		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	pr=BGBCC_BSRC_GetVRegPriority(ctx, sctx, sreg);
	if((sctx->vsp_rsv>0) && (pr<sctx->vsp_rsv))
	{
//		BGBCC_DBGBREAK;
		creg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		BGBCC_BSRC_EmitMovRegReg(ctx, sctx, creg, dreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
	return(1);

//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_BSRC_EmitStoreVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register dreg, int sreg)
{
	int creg, pr;
	
	creg=BGBCC_BSRC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
//		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg, creg);
		BGBCC_BSRC_EmitMovRegReg(ctx, sctx, sreg, creg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	pr=BGBCC_BSRC_GetVRegPriority(ctx, sctx, dreg);
	if((sctx->vsp_rsv>0) && (pr<sctx->vsp_rsv))
		{ BGBCC_DBGBREAK; }

	BGBCC_BSRC_EmitStoreFrameVRegReg(ctx, sctx, dreg, sreg);
	return(1);
}

int BGBCC_BSRC_EmitMovVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, ccxl_register sreg)
{
	s64 li;
	int cdreg, csreg;
	int tr0, rcls;
	int i, j, k;

	BGBCC_BSRC_NormalizeImmVRegInt(ctx, sctx, type, &sreg);

#if 1
	rcls=BGBCC_BSRC_TypeGetRegClassP(ctx, type);
	if((rcls==BGBCC_SH_REGCLS_VO_GR) || (rcls==BGBCC_SH_REGCLS_VO_GR2) ||
		(rcls==BGBCC_SH_REGCLS_VO_QGR) ||
		(rcls==BGBCC_SH_REGCLS_VO_QGR2) ||
		(rcls==BGBCC_SH_REGCLS_WGR))
	{

		if((BGBCC_CCXL_IsRegLocalP(ctx, sreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, sreg)) &&
			(BGBCC_CCXL_IsRegLocalP(ctx, dreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, dreg)))
		{
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, rcls);
//			BGBCC_BSRC_EmitLoadFrameVRegByValReg
			BGBCC_BSRC_EmitLoadFrameVRegByValReg(ctx, sctx, sreg, tr0);
			BGBCC_BSRC_EmitStoreFrameVRegByValReg(ctx, sctx, dreg, tr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);

			return(1);
		}
	}
#endif

	if(sctx->is_addr64 &&
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			li=(u32)li;
			BGBCC_BSRC_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_BSRC_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
//	if(0)
//	if(1)
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			BGBCC_BSRC_EmitMovVRegImm(ctx, sctx, type, dreg, j);
			return(1);
		}

		csreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_BSRC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		if((cdreg&0xF0) != (csreg&0xF0))
		{
			if(((cdreg&BGBCC_SH_REG_RTMASK5)==(csreg&BGBCC_SH_REG_RTMASK5)) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
			{
			}else
			if(((cdreg&BGBCC_SH_REG_RTMASK5)==(csreg&BGBCC_SH_REG_RTMASK5)) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0))
			{
			}else

//			if(((cdreg&0xF0)==BGBCC_SH_REG_RD0) &&
//				((csreg&0xF0)==BGBCC_SH_REG_RQ0))
			if(((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
			{
				csreg=BGBCC_SH_REG_RD0+(csreg&31);
//			}else if(((cdreg&0xF0)==BGBCC_SH_REG_RQ0) &&
//				((csreg&0xF0)==BGBCC_SH_REG_RD0))
			}else if(((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0))
			{
				csreg=BGBCC_SH_REG_RQ0+(csreg&31);
			}else
			{
				printf("BGBCC_BSRC_EmitMovVRegVReg: "
					"Bad MOV Regs Rm=%02X(%016llX) Rn=%02X(%016llX)\n",
					csreg, sreg.val, cdreg, dreg.val);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
				cdreg=BGBCC_SH_REG_ZZR;
				csreg=BGBCC_SH_REG_ZZR;
			}
		}
	}else
	{
		csreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_BSRC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	}

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
//			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_BSRC_EmitMovRegReg(ctx, sctx, csreg, cdreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if(sctx->fpu_soft)
			{
				if(sctx->is_addr64)
				{
					tr0=BGBCC_BSRC_ScratchAllocReg(
						ctx, sctx, BGBCC_SH_REGCLS_QGR);
				}else
				{
					tr0=BGBCC_BSRC_ScratchAllocReg(
						ctx, sctx, BGBCC_SH_REGCLS_GR2);
				}
			}else
			{
				tr0=BGBCC_BSRC_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_FR2);
			}
			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegLongP(ctx, sreg))
		if(BGBCC_CCXL_TypeSgLongP(ctx, type) ||
			BGBCC_CCXL_TypeVariantP(ctx, type))
		{
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR2);
//			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type))
		{
			if(sctx->fpu_soft)
			{
				tr0=BGBCC_BSRC_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_GR);
			}else
			{
				tr0=BGBCC_BSRC_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_FR);
			}

//			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);
//		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);

		tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);

		return(1);
	}
}


int BGBCC_BSRC_EmitFpConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int nm1, int nm2,
	ccxl_register dreg, ccxl_register sreg)
{
	char *s0;
	int lr4, lr0;
	int tr0;
	int cdreg, csreg;

	if(sctx->fpu_soft)
	{
		if(nm2==BGBCC_SH_NMID_FLOAT)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, dreg))
			{
				s0="__sfp_float_f64";
				lr4=BGBCC_SH_REG_RD4;
				lr0=BGBCC_SH_REG_RQ0;
			}else
			{
				s0="__sfp_float_f32";
				lr4=BGBCC_SH_REG_RD4;
				lr0=BGBCC_SH_REG_RD0;
			}
		}

		if(nm1==BGBCC_SH_NMID_FTRC)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
			{
				s0="__sfp_ftrc_f64";
				lr4=BGBCC_SH_REG_RQ4;
				lr0=BGBCC_SH_REG_RD0;
			}else
			{
				s0="__sfp_ftrc_f32";
				lr4=BGBCC_SH_REG_RD4;
				lr0=BGBCC_SH_REG_RD0;
			}
		}

		if(nm2==BGBCC_SH_NMID_FCNVSD)
		{
			s0="__sfp_fcnvsd";
			lr4=BGBCC_SH_REG_RD4;
			lr0=BGBCC_SH_REG_RQ0;
		}

		if(nm1==BGBCC_SH_NMID_FCNVDS)
		{
			s0="__sfp_fcnvds";
			lr4=BGBCC_SH_REG_RQ4;
			lr0=BGBCC_SH_REG_RD0;
		}

		if(!sctx->is_addr64)
		{
			if(lr4==BGBCC_SH_REG_RQ4)
				lr4=BGBCC_SH_REG_LR4;
			else
				lr4=BGBCC_SH_REG_R4;

			if(lr0==BGBCC_SH_REG_RQ0)
				lr0=BGBCC_SH_REG_LR0;
			else
				lr0=BGBCC_SH_REG_R0;
		}

		if(s0)
		{
			BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, lr4);
			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
			BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, lr4);
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, lr0);
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, lr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, lr0);
			return(1);
		}
	}

#if 0
	if(sctx->fpu_lite)
	{
		s0=NULL;

#if 1
		if(!BGBCC_CCXL_IsRegDoubleP(ctx, dreg) &&
			!BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
		{
			if(nm2==BGBCC_SH_NMID_FLOAT)
			{
				csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
				BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
					nm1, csreg, BGBCC_SH_REG_FPUL);
				BGBCC_BSRC_EmitOpReg(ctx, sctx, nm2, cdreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			if(nm1==BGBCC_SH_NMID_FTRC)
			{
				csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
				BGBCC_BSRC_EmitOpReg(ctx, sctx, nm1, csreg);
				BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
					nm2, BGBCC_SH_REG_FPUL, cdreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}
#endif

		if(nm2==BGBCC_SH_NMID_FLOAT)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, dreg))
			{
				s0="__lfp_float_f64";
				lr4=BGBCC_SH_REG_R4;
				lr0=BGBCC_SH_REG_DR0;
			}else
			{
				s0="__lfp_float_f32";
				lr4=BGBCC_SH_REG_R4;
				lr0=BGBCC_SH_REG_FR0;
			}
		}

		if(nm1==BGBCC_SH_NMID_FTRC)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
			{
				s0="__lfp_ftrc_f64";
				lr4=BGBCC_SH_REG_DR4;
				lr0=BGBCC_SH_REG_R0;
			}else
			{
				s0="__lfp_ftrc_f32";
				lr4=BGBCC_SH_REG_FR4;
				lr0=BGBCC_SH_REG_R0;
			}
		}

		if(nm2==BGBCC_SH_NMID_FCNVSD)
		{
			s0="__lfp_fcnvsd";
			lr4=BGBCC_SH_REG_FR4;
			lr0=BGBCC_SH_REG_DR0;
		}

		if(nm1==BGBCC_SH_NMID_FCNVDS)
		{
			s0="__lfp_fcnvds";
			lr4=BGBCC_SH_REG_DR4;
			lr0=BGBCC_SH_REG_FR0;
		}

		if(s0)
		{
			BGBCC_BSRC_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, lr4);
			BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
			BGBCC_BSRC_EmitCallName(ctx, sctx, s0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, lr4);
			BGBCC_BSRC_ScratchSafeStompReg(ctx, sctx, lr0);
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, lr0);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, lr0);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

	return(0);

#if 0
	csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_BSRC_EmitGetRegisterWrite(ctx, sctx, dreg);
//	BGBCC_BSRC_EmitMovRegReg(ctx, sctx, csreg, cdreg);

	if(	BGBCC_BSRC_EmitRegIsDpReg(ctx, sctx, csreg) ||
		BGBCC_BSRC_EmitRegIsDpReg(ctx, sctx, cdreg))
		{ BGBCC_BSRC_SetStatusFpscrDouble(ctx, sctx); }
	else
		{ BGBCC_BSRC_SetStatusFpscrFloat(ctx, sctx); }

	csreg=BGBCC_BSRC_RegNormalizeReg(ctx, sctx, csreg);
	cdreg=BGBCC_BSRC_RegNormalizeReg(ctx, sctx, cdreg);
	if((nm1==BGBCC_SH_NMID_LDS) || (nm1==BGBCC_SH_NMID_FLDS))
	{
		if(BGBCC_BSR_EmitCheckRegNonBaseGPR(sctx, csreg))
//		if(0)
		{
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, tr0);
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm1, tr0, BGBCC_SH_REG_FPUL);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm1, csreg, BGBCC_SH_REG_FPUL);
		}
	}
	else
		{ BGBCC_BSRC_EmitOpReg(ctx, sctx, nm1, csreg); }

	if((nm2==BGBCC_SH_NMID_STS) || (nm2==BGBCC_SH_NMID_FSTS))
	{
		if(BGBCC_BSR_EmitCheckRegNonBaseGPR(sctx, cdreg))
//		if(0)
		{
			tr0=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm2, BGBCC_SH_REG_FPUL, tr0);
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, tr0, cdreg);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm2, BGBCC_SH_REG_FPUL, cdreg);
		}
	}
	else
		{ BGBCC_BSRC_EmitOpReg(ctx, sctx, nm2, cdreg); }

	BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
	BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
	return(1);
#endif
}


int BGBCC_BSRC_EmitMovVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, s64 imm)
{
	int cdreg, csreg;
	int rcls;
	
	cdreg=BGBCC_BSRC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((((s32)imm)!=imm) &&
			BGBCC_BSR_EmitCheckRegQuad(sctx, cdreg))
		{
			BGBCC_BSR_EmitLoadRegImm64P(sctx, cdreg, imm);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
	
		BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg, imm);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	rcls=BGBCC_BSRC_TypeGetRegClassP(ctx, type);

	if((rcls==BGBCC_SH_REGCLS_QGR) || (rcls==BGBCC_SH_REGCLS_VO_QGR))
	{
		csreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
//		BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, csreg, imm);
		BGBCC_BSR_EmitLoadRegImm64P(sctx, csreg, imm);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, csreg);
		return(1);
	}

	csreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, csreg, imm);
	BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
	BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, csreg);

//	BGBCC_BSR_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, imm);
//	BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
	return(1);
}


int BGBCC_BSRC_EmitOpNmidVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type, int nmid,
	ccxl_register dreg, ccxl_register sreg)
{
	int cdreg, csreg, ctreg;
	
	cdreg=BGBCC_BSRC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	csreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nmid, csreg, cdreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nmid, cdreg, cdreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nmid, csreg, ctreg);
			BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

		ctreg=BGBCC_BSRC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_BSRC_EmitLoadVRegReg(ctx, sctx, sreg, ctreg);
		BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nmid, ctreg, ctreg);
		BGBCC_BSRC_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
		BGBCC_BSRC_ScratchReleaseReg(ctx, sctx, ctreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_BSRC_EmitJCmpVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, flip, noflip;
	s32 imm;
	int nm1, nm2, nm3;
	
	noflip=0;

	if((type.val==CCXL_TY_UB) || (type.val==CCXL_TY_US))
		type.val=CCXL_TY_I;

#if 1
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		noflip=1;
//		if(sctx->is_betav && !sctx->has_bjx1ari)
		if(sctx->is_betav && (!sctx->has_bjx1ari || !sctx->has_bjx1mov))
			noflip=0;
	}
#endif
	
	nm3=-1; flip=0;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BT;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BF;
				nm3=BGBCC_SH_NMID_BRLT;
			}
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_BRGT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_BRLE;
		}
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BF;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BF;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BT;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BT;
				nm3=BGBCC_SH_NMID_BRGE;
			}
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TST;
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

			csreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_BSR_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
				BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);
				BGBCC_BSR_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, treg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, treg);
		}else
		{
			csreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_BSRC_EmitTryGetRegisterRead(ctx, sctx, treg);

			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, treg);
		}

#if 0
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1) && !flip &&
			BGBCC_BSR_EmitCheckRegBaseGPR(sctx, csreg) &&
			BGBCC_BSR_EmitCheckRegBaseGPR(sctx, ctreg))
		{
//			BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);
			BGBCC_BSRC_SetModeDqClear(ctx, sctx);

			BGBCC_BSR_EmitOpCmpRegRegLabel(sctx, nm3, ctreg, csreg, lbl);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

//		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_BSRC_CheckSetModeDqClear(ctx, sctx);

		if(flip)
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_BSRC_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_BSR_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_BSR_EmitOpLabel(sctx, nm2, lbl);
		BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);

		BGBCC_BSR_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
//	BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
//	BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, treg);
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitJCmpVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		(BGBCC_CCXL_TypePointerP(ctx, type) && !sctx->is_addr64))
	{
		return(BGBCC_BSRC_EmitJCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_BSRC_EmitJCmpVRegVRegSoftFloat(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

#if 0
	if((BGBCC_CCXL_TypeSgLongP(ctx, type) ||
		BGBCC_CCXL_TypeSgNLongP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type)) &&
			sctx->is_addr64)
	{
		return(BGBCC_BSRC_EmitJCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}
#endif

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		return(BGBCC_BSRC_EmitJCmpVRegVRegLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitJCmpVRegZeroInt(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	int csreg, ctreg;
	int nm1, nm2, nm3;

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(cmp==CCXL_CMP_LT)
		{
//			BGBCC_BSR_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_GE)
		{
			BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);
			BGBCC_BSR_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_LE)
			{ cmp=CCXL_CMP_EQ; }
		if(cmp==CCXL_CMP_GT)
			{ cmp=CCXL_CMP_NE; }
	}

	if(cmp==CCXL_CMP_TST)
	{
//		BGBCC_BSR_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
		return(1);
	}

	if(cmp==CCXL_CMP_NTST)
	{
		BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);
		BGBCC_BSR_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
		return(1);
	}
	
	nm1=-1; nm2=-1; nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLT;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGT;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLE;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGE;
		break;

//	case CCXL_CMP_TST:
//		nm1=BGBCC_SH_NMID_TST;
//		nm2=BGBCC_SH_NMID_BF;
//		break;
//	case CCXL_CMP_NTST:
//		nm1=BGBCC_SH_NMID_TST;
//		nm2=BGBCC_SH_NMID_BT;
//		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_BSRC_EmitGetRegisterRead(ctx, sctx, sreg);

		if(!BGBCC_BSR_EmitCheckRegBaseGPR(sctx, csreg))
		{
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			return(BGBCC_BSRC_EmitJCmpVRegVRegInt(ctx, sctx,
				type, sreg, treg, cmp, lbl));

//			return(1);
		}

#if 0
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1) &&
			BGBCC_BSR_EmitCheckRegBaseGPR(sctx, csreg))
		{
//			BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);
			BGBCC_BSRC_SetModeDqClear(ctx, sctx);

			BGBCC_BSR_EmitOpCmpRegLabel(sctx, nm3, csreg, lbl);
			BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

//		BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);

		BGBCC_BSRC_SetModeDqClear(ctx, sctx);

		BGBCC_BSRC_EmitOpReg(ctx, sctx, nm1, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_BSR_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_BSR_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_BSRC_ResetFpscrLocal(ctx, sctx);
		BGBCC_BSR_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
//	BGBCC_BSRC_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}

int BGBCC_BSRC_EmitJCmpVRegZero(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_BSRC_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) && !sctx->is_addr64)
	{
		return(BGBCC_BSRC_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

#if 0
	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) && sctx->is_addr64)
	{
		return(BGBCC_BSRC_EmitJCmpVRegZeroQLong(ctx, sctx,
			type, sreg, cmp, lbl));
	}
#endif

#if 0
	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_BSRC_EmitJCmpVRegZeroFloat(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}
#endif

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_BSRC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if((BGBCC_CCXL_TypeSgNLongP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) &&
		sctx->is_addr64)
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_BSRC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 0.0);
		return(BGBCC_BSRC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 0.0);
		return(BGBCC_BSRC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
