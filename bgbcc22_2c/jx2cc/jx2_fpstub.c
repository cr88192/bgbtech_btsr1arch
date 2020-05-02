int BGBCC_JX2C_EmitRegIsGpReg(
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

int BGBCC_JX2C_EmitRegIsExtGpReg(
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

int BGBCC_JX2C_EmitRegIsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	return(0);
}

int BGBCC_JX2C_EmitRegIsDpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
//	if((dreg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);
	return(0);
}

int BGBCC_JX2C_EmitRegIsLpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	return(0);
}

int BGBCC_JX2C_EmitRegIsExtLpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
		return(1);
	return(0);
}

int BGBCC_JX2C_RegNormalizeReg(
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

int BGBCC_JX2C_CheckVRegIsZRegP(
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

int BGBCC_JX2C_GetStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(sctx->cur_fpscr&BGBCC_SH_FPSCR_SZ);
}

int BGBCC_JX2C_GetStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(sctx->cur_fpscr&BGBCC_SH_FPSCR_PR);
}

int BGBCC_JX2C_UpdateStatusFpscr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	u32 state)
{
	return(0);
}

int BGBCC_JX2C_ClearStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_JX2C_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr&(~BGBCC_SH_FPSCR_SZ)));
}

int BGBCC_JX2C_ClearStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	sctx->cnt_set_fp32++;
	return(BGBCC_JX2C_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr&(~BGBCC_SH_FPSCR_PR)));
}

int BGBCC_JX2C_SetStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_JX2C_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr|BGBCC_SH_FPSCR_SZ));
}

int BGBCC_JX2C_SetStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_SetStatusFpscrFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_SetStatusFpscrDouble(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	u32 fl1;
	return(0);
}

int BGBCC_JX2C_ResetModeDqLocal(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_ResetModeDqUnknown(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_SetModeDqSet(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_SetModeDqClear(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_CheckSetModeDqSet(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_CheckSetModeDqClear(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

/** Reset active FPSCR to function-local defaults */
int BGBCC_JX2C_ResetFpscrLocal(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

/** Reset active FPSCR to ABI defaults */
int BGBCC_JX2C_ResetFpscrDefaults(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_ResetFpscrUnknown(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(0);
}
