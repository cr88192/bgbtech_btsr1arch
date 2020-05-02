int BGBCC_BSRC_EmitRegIsGpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);

	return(0);
}

int BGBCC_BSRC_EmitRegIsExtGpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);

	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	return(0);
}

int BGBCC_BSRC_EmitRegIsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	return(0);
}

int BGBCC_BSRC_EmitRegIsDpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
//	if((dreg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);
	return(0);
}

int BGBCC_BSRC_EmitRegIsLpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	return(0);
}

int BGBCC_BSRC_RegNormalizeReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
//	if((dreg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		return(BGBCC_SH_REG_FR0+(dreg&7)*2);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(BGBCC_SH_REG_FR0+(dreg&15));
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(BGBCC_SH_REG_R0+(dreg&15));
	return(dreg);
}

int BGBCC_BSRC_CheckVRegIsZRegP(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	if(reg.val==CCXL_REGID_REG_DZ)
		return(0);
	if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
		return(1);
	return(0);
}

int BGBCC_BSRC_GetStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(sctx->cur_fpscr&BGBCC_SH_FPSCR_SZ);
}

int BGBCC_BSRC_GetStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(sctx->cur_fpscr&BGBCC_SH_FPSCR_PR);
}

int BGBCC_BSRC_UpdateStatusFpscr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	u32 state)
{
	return(0);
}

int BGBCC_BSRC_ClearStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_BSRC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr&(~BGBCC_SH_FPSCR_SZ)));
}

int BGBCC_BSRC_ClearStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	sctx->cnt_set_fp32++;
	return(BGBCC_BSRC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr&(~BGBCC_SH_FPSCR_PR)));
}

int BGBCC_BSRC_SetStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_BSRC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr|BGBCC_SH_FPSCR_SZ));
}

int BGBCC_BSRC_SetStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_SetStatusFpscrFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_SetStatusFpscrDouble(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	u32 fl1;
	return(0);
}

int BGBCC_BSRC_ResetModeDqLocal(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_ResetModeDqUnknown(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_SetModeDqSet(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_SetModeDqClear(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_CheckSetModeDqSet(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_CheckSetModeDqClear(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

/** Reset active FPSCR to function-local defaults */
int BGBCC_BSRC_ResetFpscrLocal(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

/** Reset active FPSCR to ABI defaults */
int BGBCC_BSRC_ResetFpscrDefaults(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_BSRC_ResetFpscrUnknown(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}
