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

	if(sctx->has_xgpr&1)
	{
		if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
			return(1);
		if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
			return(1);
		if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0)
			return(1);
	}

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

int BGBCC_JX2C_MapLpRegToQgr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	int tr0;
	
	if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		tr0=BGBCC_SH_REG_RQ0+(dreg&0x1E);
		if(dreg&1)
			tr0+=32;
		return(tr0);
	}
	return(dreg);
}

int BGBCC_JX2C_MapLpRegToDgr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	int tr0;
	
	if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		tr0=BGBCC_SH_REG_RD0+(dreg&0x1E);
		if(dreg&1)
			tr0+=32;
		return(tr0);
	}
	return(dreg);
}

int BGBCC_JX2C_MapLpRegToGr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	int tr0;
	
	if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		tr0=BGBCC_SH_REG_RQ0+(dreg&0x1E);
		if(dreg&1)
			tr0+=32;
		return(tr0);
	}
	return(dreg);
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

int BGBCC_JX2C_CalcDivideRecipShr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int divisor, u32 *rrcp, int *rshl)
{
	u64 rcp, q1, q2, n1, n2, nv;
	int j, l;

	j=divisor;
	if(j<=0)
	{
		*rrcp=0;
		*rshl=0;
		return(-1);
	}

	l=31+ceil(log(j)/log(2));
	l--;

	rcp=((1ULL<<l)+(j-1))/j;

	if(rcp>=(1ULL<<32))
	{
		*rrcp=0;
		*rshl=0;
		return(-1);
	}

#if 1
	n1=((1LL<<31)-1);
			
	n2=((n1/j)*j)+10;
	if(n2<n1)
	{
		n1=n2;
		n2=n1-20;
	}else
	{
		n2=n1-(2*j);
	}
			
	for(nv=n1; nv>n2; nv--)
	{
		q1=(nv*rcp)>>l;
		q2=nv/j;
		if(q1!=q2)
		{
			rcp=0;
			break;
		}
	}
#endif

	*rrcp=rcp;
	*rshl=l;
	return(0);
}
