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



int BGBCC_JX2C_EmitOpNone(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpNone(sctx, nmid);
	else
		BGBCC_JX2_EmitOpNone(sctx, nmid);
	return(0);
}

int BGBCC_JX2C_EmitOpReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int reg)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpReg(sctx, nmid, reg);
	else
		BGBCC_JX2_EmitOpReg(sctx, nmid, reg);
	return(0);
}

int BGBCC_JX2C_EmitOpImm(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, s64 imm)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpImm(sctx, nmid, imm);
	else
		BGBCC_JX2_EmitOpImm(sctx, nmid, imm);
	return(0);
}

int BGBCC_JX2C_EmitOpMReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int reg)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpMReg(sctx, nmid, reg);
	else
		BGBCC_JX2_EmitOpMReg(sctx, nmid, reg);
	return(0);
}

int BGBCC_JX2C_EmitOpRegReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegReg(sctx, nmid, rm, rn);
	else
		BGBCC_JX2_EmitOpRegReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegRegPredT(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd)
		{ BGBCC_DBGBREAK }

//	if(sctx->use_emitqueue)
//		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
//	else
		BGBCC_JX2_EmitOpRegRegPred(sctx, nmid, rm, rn, 4);
	return(0);
}

int BGBCC_JX2C_EmitOpRegRegPredF(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd)
		{ BGBCC_DBGBREAK }

//	if(sctx->use_emitqueue)
//		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
//	else
		BGBCC_JX2_EmitOpRegRegPred(sctx, nmid, rm, rn, 5);
	return(0);
}

int BGBCC_JX2C_EmitOpRegStReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegStReg(sctx, nmid, rm, rn);
	else
		BGBCC_JX2_EmitOpRegStReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpLdRegReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpLdRegReg(sctx, nmid, rm, rn);
	else
		BGBCC_JX2_EmitOpLdRegReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegStDecReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegStDecReg(sctx, nmid, rm, rn);
	else
		BGBCC_JX2_EmitOpRegStDecReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpLdIncRegReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpLdIncRegReg(sctx, nmid, rm, rn);
	else
		BGBCC_JX2_EmitOpLdIncRegReg(sctx, nmid, rm, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegStRegDisp(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int rn, int disp)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegStRegDisp(sctx, nmid, rm, rn, disp);
	else
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, nmid, rm, rn, disp);
	return(0);
}

int BGBCC_JX2C_EmitOpLdRegDispReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int disp, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpLdRegDispReg(sctx, nmid, rm, disp, rn);
	else
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nmid, rm, disp, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegStReg2(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int ro, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegStReg2(sctx, nmid, rm, ro, rn);
	else
		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid, rm, ro, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpLdReg2Reg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int ro, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpLdReg2Reg(sctx, nmid, rm, ro, rn);
	else
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid, rm, ro, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegStReg2Disp(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegStReg2Disp(sctx, nmid, rm, ro, rn, disp);
	else
		BGBCC_JX2_EmitOpRegStReg2Disp(sctx, nmid, rm, ro, rn, disp);
	return(0);
}

int BGBCC_JX2C_EmitOpLdReg2DispReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpLdReg2DispReg(sctx, nmid, rm, ro, disp, rn);
	else
		BGBCC_JX2_EmitOpLdReg2DispReg(sctx, nmid, rm, ro, disp, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegRegReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rs, int rt, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegRegReg(sctx, nmid, rs, rt, rn);
	else
		BGBCC_JX2_EmitOpRegRegReg(sctx, nmid, rs, rt, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpImmReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, s64 imm, int reg)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
	else
		BGBCC_JX2_EmitOpImmReg(sctx, nmid, imm, reg);
	return(0);
}

int BGBCC_JX2C_EmitOpImmRegPredT(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, s64 imm, int reg)
{
	if(ctx->cur_vop && ctx->cur_vop->prd)
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
	else
		BGBCC_JX2_EmitOpImmRegPred(sctx, nmid, imm, reg, 4);
	return(0);
}

int BGBCC_JX2C_EmitOpImmRegPredF(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, s64 imm, int reg)
{
	if(ctx->cur_vop && ctx->cur_vop->prd)
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
	else
		BGBCC_JX2_EmitOpImmRegPred(sctx, nmid, imm, reg, 5);
	return(0);
}

int BGBCC_JX2C_EmitOpRegImmReg(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, s64 imm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd && !(sctx->op_is_wex2&12))
		{ BGBCC_DBGBREAK }

	if(sctx->use_emitqueue)
		BGBCC_JX2_QueueOpRegImmReg(sctx, nmid, rm, imm, rn);
	else
		BGBCC_JX2_EmitOpRegImmReg(sctx, nmid, rm, imm, rn);
	return(0);
}

int BGBCC_JX2C_EmitOpRegImmRegPredT(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, s64 imm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd)
		{ BGBCC_DBGBREAK }

//	if(sctx->use_emitqueue)
//		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
//	else
		BGBCC_JX2_EmitOpRegImmRegPred(sctx, nmid, rm, imm, rn, 4);
	return(0);
}

int BGBCC_JX2C_EmitOpRegImmRegPredF(
	BGBCC_TransState *ctx, BGBCC_JX2_Context *sctx,
	int nmid, int rm, s64 imm, int rn)
{
	if(ctx->cur_vop && ctx->cur_vop->prd)
		{ BGBCC_DBGBREAK }

//	if(sctx->use_emitqueue)
//		BGBCC_JX2_QueueOpImmReg(sctx, nmid, imm, reg);
//	else
		BGBCC_JX2_EmitOpRegImmRegPred(sctx, nmid, rm, imm, rn, 5);
	return(0);
}





int BGBCC_JX2C_EmitMovRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int sreg, int dreg)
{
	int sreg2, dreg2, sw, srix, drix, sty, dty, nm1;
	ccxl_register svreg, dvreg;
	ccxl_type tty;

	if(sreg==dreg)
		return(1);

	if(BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		if((sreg&63)==(dreg&63))
			return(1);
	
		srix=BGBCC_JX2C_GetRegisterIndexForReg(ctx, sctx, sreg);
		drix=BGBCC_JX2C_GetRegisterIndexForReg(ctx, sctx, dreg);
		
		sty=-1;
		dty=-1;
		if(srix>=0)
		{
			svreg=sctx->regalc_map[srix];
			tty=BGBCC_CCXL_GetRegType(ctx, svreg);
			sty=tty.val;
		}
		if(drix>=0)
		{
			svreg=sctx->regalc_map[drix];
			tty=BGBCC_CCXL_GetRegType(ctx, svreg);
			dty=tty.val;
		}
		
		nm1=BGBCC_SH_NMID_MOV;
//		if((sty>0) && (sty<16) && (sty==dty))
		if((sty>=0) && (sty<16))
		{
			if(sty==CCXL_TY_I)
				{ nm1=BGBCC_SH_NMID_EXTSL; }
			if(sty==CCXL_TY_UI)
				{ nm1=BGBCC_SH_NMID_EXTUL; }

			if(sty==CCXL_TY_SS)
				{ nm1=BGBCC_SH_NMID_EXTSW; }
			if(sty==CCXL_TY_US)
				{ nm1=BGBCC_SH_NMID_EXTUW; }

			if(sty==CCXL_TY_SB)
				{ nm1=BGBCC_SH_NMID_EXTSB; }
			if(sty==CCXL_TY_UB)
				{ nm1=BGBCC_SH_NMID_EXTUB; }
		}
	
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, sreg, dreg);
		return(1);
	}
	
	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		sreg2=BGBCC_SH_REG_R0+(sreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		sreg2=BGBCC_JX2_GETLREG_LO(sreg);
//		dreg2=BGBCC_JX2_GETLREG_LO(dreg);

//		if(sctx->has_xgpr&2)
		if(sctx->has_xgpr&1)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_XMOV, sreg, dreg);
		}else
		{
			sreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, sreg);
			dreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV,
				sreg2+0, dreg2+0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV,
				sreg2+1, dreg2+1);
		}
		return(1);
	}

	if(	BGBCC_JX2_EmitCheckRegExtGPR(sctx, sreg) &&
		BGBCC_JX2_EmitCheckRegExtGPR(sctx, dreg))
	{
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_ClearStatusFpscrSz(ctx, sctx);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_LDS, sreg, BGBCC_SH_REG_FPUL);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_DLR);
		BGBCC_JX2C_EmitOpReg(ctx, sctx,
			BGBCC_SH_NMID_FLDCF, dreg);

		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_ClearStatusFpscrSz(ctx, sctx);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_FLDS, sreg, BGBCC_SH_REG_FPUL);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg);

		BGBCC_JX2C_EmitOpReg(ctx, sctx,
			BGBCC_SH_NMID_FSTCF, sreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_DLR, dreg);

		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_MOV, sreg, BGBCC_SH_REG_DLR);
//		BGBCC_JX2C_EmitOpReg(ctx, sctx,
//			BGBCC_SH_NMID_FLDCD, dreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FMOV, sreg, dreg);

		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_EmitOpReg(ctx, sctx,
//			BGBCC_SH_NMID_FSTCD, sreg);
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_DLR, dreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FMOV, sreg, dreg);

		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg, dreg);
		return(1);

#if 0
//		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
//		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sreg2=BGBCC_SH_REG_FR0+(sreg&15);
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);

		if(BGBCC_JX2C_GetStatusFpscrSz(ctx, sctx))
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg2, dreg2);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg2+0, dreg2+0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_FMOV, sreg2+1, dreg2+1);
		}
#endif

		return(1);
	}

#if 0
	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_R0+(sreg&15);
//		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
		sw=(sctx->is_le!=0);

		BGBCC_JX2C_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_LDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_LDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));
		return(1);
	}
#endif

#if 0
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
		sreg2=BGBCC_SH_REG_FR0+(sreg&15);
		sw=(sctx->is_le!=0);

		BGBCC_JX2C_ClearStatusFpscrSz(ctx, sctx);
		
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FLDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FLDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));

		return(1);
	}
#endif

	printf("BGBCC_JX2C_EmitMovRegReg: "
		"Bad MOV Regs Rm=%02X Rn=%02X\n", sreg, dreg);
	BGBCC_CCXL_StubError(ctx);
	return(0);

//	nm=BGBCC_SH_NMID_MOV;
//	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, creg))
//		nm=BGBCC_SH_NMID_FMOV;
//	BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm, creg, dreg);
//	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
//	return(1);
}

int BGBCC_JX2C_EmitLoadVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	ccxl_type bty;
	int creg, nm, pr, nm1, sty;

#if 0
	if(BGBCC_CCXL_IsRegImmStringP(ctx, sreg))
	{
		BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, sreg);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
		return(1);
	}
#endif

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, sreg);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
		return(1);
	}

	creg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
		bty=BGBCC_CCXL_GetRegType(ctx, sreg);
		sty=bty.val;

		if(BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, creg) &&
			BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg) &&
			((creg&63)!=(dreg&63)) &&
			(sty>=0) && (sty<16))
		{
			nm1=BGBCC_SH_NMID_MOV;

			if(sty==CCXL_TY_I)
				{ nm1=BGBCC_SH_NMID_EXTSL; }
			if(sty==CCXL_TY_UI)
				{ nm1=BGBCC_SH_NMID_EXTUL; }

			if(sty==CCXL_TY_SS)
				{ nm1=BGBCC_SH_NMID_EXTSW; }
			if(sty==CCXL_TY_US)
				{ nm1=BGBCC_SH_NMID_EXTUW; }

			if(sty==CCXL_TY_SB)
				{ nm1=BGBCC_SH_NMID_EXTSB; }
			if(sty==CCXL_TY_UB)
				{ nm1=BGBCC_SH_NMID_EXTUB; }
	
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, creg, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

//		if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, creg))
//			nm=BGBCC_SH_NMID_FMOV;
//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm, creg, dreg);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, creg, dreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	pr=BGBCC_JX2C_GetVRegPriority(ctx, sctx, sreg);
	if((sctx->vsp_rsv>0) && (pr<sctx->vsp_rsv))
	{
//		BGBCC_DBGBREAK;
		creg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, creg, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, sreg);
	BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
	return(1);

//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_JX2C_EmitStoreVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register dreg, int sreg)
{
	ccxl_type bty;
	int creg, pr, nm1, sty;
	
	if(!BGBCC_JX2C_ProbeVRegInRegisterP(ctx, sctx, dreg) &&
		!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, dreg))
	{
		BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, sreg);
		return(1);
	}

	creg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
		bty=BGBCC_CCXL_GetRegType(ctx, dreg);
		sty=bty.val;

		if(BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, creg) &&
			BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, sreg) &&
			((creg&63)!=(sreg&63)) &&
			(sty>=0) && (sty<16))
		{
			nm1=BGBCC_SH_NMID_MOV;

			if(sty==CCXL_TY_I)
				{ nm1=BGBCC_SH_NMID_EXTSL; }
			if(sty==CCXL_TY_UI)
				{ nm1=BGBCC_SH_NMID_EXTUL; }

			if(sty==CCXL_TY_SS)
				{ nm1=BGBCC_SH_NMID_EXTSW; }
			if(sty==CCXL_TY_US)
				{ nm1=BGBCC_SH_NMID_EXTUW; }

			if(sty==CCXL_TY_SB)
				{ nm1=BGBCC_SH_NMID_EXTSB; }
			if(sty==CCXL_TY_UB)
				{ nm1=BGBCC_SH_NMID_EXTUB; }
	
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, sreg, creg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

//		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, BGBCC_SH_NMID_MOV, sreg, creg);
		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, sreg, creg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, sreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, sreg))
	{
	}else
	{
		pr=BGBCC_JX2C_GetVRegPriority(ctx, sctx, dreg);
		if((sctx->vsp_rsv>0) && (pr<sctx->vsp_rsv))
			{ BGBCC_DBGBREAK; }
	}

	BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, dreg, sreg);
	return(1);
}

int BGBCC_JX2C_EmitStoreVRegByValReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register dreg, int sreg)
{
	ccxl_type bty;
	int creg, pr, nm1, sty;
	
	creg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
		bty=BGBCC_CCXL_GetRegType(ctx, dreg);
		sty=bty.val;

		if(BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, creg) &&
			BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, sreg) &&
			((creg&63)!=(sreg&63)) &&
			(sty>=0) && (sty<16))
		{
			nm1=BGBCC_SH_NMID_MOV;

			if(sty==CCXL_TY_I)
				{ nm1=BGBCC_SH_NMID_EXTSL; }
			if(sty==CCXL_TY_UI)
				{ nm1=BGBCC_SH_NMID_EXTUL; }

			if(sty==CCXL_TY_SS)
				{ nm1=BGBCC_SH_NMID_EXTSW; }
			if(sty==CCXL_TY_US)
				{ nm1=BGBCC_SH_NMID_EXTUW; }

			if(sty==CCXL_TY_SB)
				{ nm1=BGBCC_SH_NMID_EXTSB; }
			if(sty==CCXL_TY_UB)
				{ nm1=BGBCC_SH_NMID_EXTUB; }
	
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, sreg, creg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		BGBCC_JX2C_EmitMovRegReg(ctx, sctx, sreg, creg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx, dreg, sreg);
	return(1);
}

int BGBCC_JX2C_EmitMovVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, ccxl_register sreg)
{
	bgbcc_vfloat128	qf;
	s64 li, lj;
	double f, g;
	int cdreg, csreg, tr0, tr1, tg0, tg1;
	int rcls, nm1, rejimm;
	int i, j, k;

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		return(0);

	BGBCC_JX2C_NormalizeImmVRegInt(ctx, sctx, type, &sreg);

#if 1
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);
	if(	(rcls==BGBCC_SH_REGCLS_VO_GR) ||
		(rcls==BGBCC_SH_REGCLS_VO_GR2) ||
		(rcls==BGBCC_SH_REGCLS_VO_QGR) ||
		(rcls==BGBCC_SH_REGCLS_VO_QGR2) )
//		(rcls==BGBCC_SH_REGCLS_VO_QGR2) ||
//		(rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_QGR2) ||
//		(rcls==BGBCC_SH_REGCLS_WGR))
	{

		if((BGBCC_CCXL_IsRegLocalP(ctx, sreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, sreg)) &&
			(BGBCC_CCXL_IsRegLocalP(ctx, dreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, dreg)))
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, rcls);
			BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, sreg);
//			BGBCC_JX2C_EmitLoadFrameVRegByValReg
			BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

			return(1);
		}
	}
#endif

//	if(sctx->is_addr64 &&
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	if(BGBCC_CCXL_TypePointerP(ctx, type))
	if(	BGBCC_CCXL_TypePointerP(ctx, type) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
//			li=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
//			li=(u32)li;
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeQuadVariantP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			if(BGBCC_CCXL_IsRegImmUnsignedP(ctx, sreg))
				lj=0;
			else
				lj=li>>63;

			lj&=0x0FFFFFFFFFFFFFFFULL;
			lj|=0x4000000000000000ULL;
			BGBCC_JX2C_EmitMovVRegImm128(ctx, sctx, type, dreg, lj, li);
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmInt128P(ctx, sreg))
		{
			BGBCC_CCXL_GetRegImmInt128Value(ctx, sreg, &li, &lj);

			lj&=0x0FFFFFFFFFFFFFFFULL;
			lj|=0x4000000000000000ULL;
			BGBCC_JX2C_EmitMovVRegImm128(ctx, sctx, type, dreg, lj, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			(*(double *)(&lj))=f;
			li=0x5000000000000000ULL;

#if 0
			qf=BGBCC_Float128_FromDouble(f);

			li=qf.hi>>4;
			li&=0x0FFFFFFFFFFFFFFFULL;
			li|=0x5000000000000000ULL;
			lj=(qf.hi<<60)|(qf.lo>>4);
#endif

			BGBCC_JX2C_EmitMovVRegImm128(ctx, sctx, type, dreg, li, lj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloat128P(ctx, sreg))
		{
			BGBCC_CCXL_GetRegImmFloat128Value(ctx, sreg, &li, &lj);
			li=(li>>4)|(lj<<60);
			lj=(lj>>4);
			lj&=0x0FFFFFFFFFFFFFFFULL;
			lj|=0x5000000000000000ULL;
			BGBCC_JX2C_EmitMovVRegImm128(ctx, sctx, type, dreg, lj, li);
			return(1);
		}

		if(	BGBCC_CCXL_IsRegImmStringP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmWStringP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmW4StringP(ctx, sreg)	)
		{
			tg0=CCXL_LVA_LookupTagIndexForName(ctx, "_String");
//			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
//				BGBCC_SH_REGCLS_QGR);
//			BGBCC_JX2_EmitLoadRegImm64P(sctx, tr1, ((u64)tg0)<<48);

			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_XMOVTT,
				tg0, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
	}

//	if(sctx->is_addr64 &&
//		BGBCC_CCXL_TypeVarRefP(ctx, type))
	if(BGBCC_CCXL_TypeVarRefP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			li&=0x3FFFFFFFFFFFFFFFULL;
			li|=0x4000000000000000ULL;
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			(*(double *)(&lj))=f;

			li=lj>>2;
			li&=0x3FFFFFFFFFFFFFFFULL;
			li|=0x8000000000000000ULL;
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(	BGBCC_CCXL_IsRegImmStringP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmWStringP(ctx, sreg)	||
			BGBCC_CCXL_IsRegImmW4StringP(ctx, sreg)	)
		{
			tg0=CCXL_LVA_LookupTagIndexForName(ctx, "_String");
			tr1=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, tr1, ((u64)tg0)<<48);

			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_OR,
				tr1, cdreg);

			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr1);
			return(1);
		}
	}

#if 1
	if(BGBCC_CCXL_TypeBaseRealP(ctx, type) &&
		(	BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg)))
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
		(*(double *)(&lj))=f;
		BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, lj);
		return(1);
	}
#endif

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		(BGBCC_CCXL_TypePointerP(ctx, type) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, type)))
//	if(0)
//	if(1)
	{
		rejimm=0;
		if(	BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, sreg) ||
			BGBCC_JX2C_ProbeVRegInRegisterActiveP(ctx, sctx, sreg))
		{
			if(	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
				BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
			{
				rejimm=1;
			}
		}

		if(!rejimm && BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
//			li=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(!rejimm && BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(sctx->is_leaftiny&1)
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		}else
		{
			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);

			if(BGBCC_JX2C_ProbeVRegInRegisterP(ctx, sctx, dreg) ||
				BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, dreg))
			{
				cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
			}else
			{
				cdreg=BGBCC_SH_REG_ZZR;
			}
		}

		if(((csreg<0) || (csreg==BGBCC_SH_REG_ZZR)) &&
			BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, sreg))
		{
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		}

		if(((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR)) &&
			BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, dreg))
		{
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		}


#if 0
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
		{
			if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
			{
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}
#endif

#if 1

#if 0
		if((csreg!=BGBCC_SH_REG_ZZR) && (cdreg!=BGBCC_SH_REG_ZZR) &&
			((csreg&BGBCC_SH_REG_RTMASK5)==(cdreg&BGBCC_SH_REG_RTMASK5)))
		{
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
			{
				BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
			
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}else
		{
			if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
			{
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
		}
#endif

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		
//		if((cdreg&0xF0) != (csreg&0xF0))
		if((cdreg&BGBCC_SH_REG_RTMASK5)!=(csreg&BGBCC_SH_REG_RTMASK5))
		{
//			if(((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0) &&
//				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0))
			if(	((cdreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0) &&
				((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0))
			{
				nm1=BGBCC_SH_NMID_EXTSL;

				if(BGBCC_CCXL_TypeUnsignedP(ctx, type) ||
					BGBCC_CCXL_TypePointerP(ctx, type))
						nm1=BGBCC_SH_NMID_EXTUL;
			
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					nm1, csreg, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			if(
				((cdreg&BGBCC_SH_REG_RTMASK6)==(csreg&BGBCC_SH_REG_RTMASK6)) &&
				((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0))
			{
			}else
				if(
				((cdreg&BGBCC_SH_REG_RTMASK6)==(csreg&BGBCC_SH_REG_RTMASK6)) &&
				((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0))
			{
			}else
				if(
				((cdreg&BGBCC_SH_REG_RTMASK6)==(csreg&BGBCC_SH_REG_RTMASK6)) &&
				((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0))
			{
			}else

//			if(((cdreg&0xF0)==BGBCC_SH_REG_RD0) &&
//				((csreg&0xF0)==BGBCC_SH_REG_RQ0))
//			if(((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0) &&
//				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
			if(((cdreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0) &&
				((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0))
			{
//				csreg=BGBCC_SH_REG_RD0+(csreg&31);
				csreg=BGBCC_SH_REG_RD0+(csreg&63);
//			}else if(((cdreg&0xF0)==BGBCC_SH_REG_RQ0) &&
//				((csreg&0xF0)==BGBCC_SH_REG_RD0))
			}else
//				if(	((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0) &&
//					((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
				if(	((cdreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0) &&
					((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0))
			{
//				csreg=BGBCC_SH_REG_R0+(csreg&31);
				csreg=BGBCC_SH_REG_R0+(csreg&63);
			}else
//				if(	((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0) &&
//					((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0))
				if(	((cdreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0) &&
					((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0))
			{
//				csreg=BGBCC_SH_REG_RQ0+(csreg&31);
				csreg=BGBCC_SH_REG_RQ0+(csreg&63);
			}else
//				if(	((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0) &&
//					((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0))
				if(	((cdreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0) &&
					((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0))
			{
//				csreg=BGBCC_SH_REG_RD0+(csreg&31);
				csreg=BGBCC_SH_REG_RD0+(csreg&63);
			}else
//				if(	((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0) &&
//					((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0))
				if(	((cdreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0) &&
					((csreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0))
			{
//				csreg=BGBCC_SH_REG_RQ0+(csreg&31);
				csreg=BGBCC_SH_REG_RQ0+(csreg&63);
			}else
			{
				printf("BGBCC_JX2C_EmitMovVRegVReg: "
					"Bad MOV Regs Rm=%02X(%016llX) Rn=%02X(%016llX)\n",
					csreg, sreg.val, cdreg, dreg.val);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				cdreg=BGBCC_SH_REG_ZZR;
				csreg=BGBCC_SH_REG_ZZR;
			}
		}
	}else
	{
//		csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if(sctx->is_leaftiny&1)
		{
			if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
		}

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
	}

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
//			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//				BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if(sctx->fpu_soft || sctx->fpu_gfp)
			{
				if(sctx->is_addr64)
				{
					tr0=BGBCC_JX2C_ScratchAllocReg(
						ctx, sctx, BGBCC_SH_REGCLS_QGR);
				}else
				{
					BGBCC_DBGBREAK
//					tr0=BGBCC_JX2C_ScratchAllocReg(
//						ctx, sctx, BGBCC_SH_REGCLS_GR2);
				}
			}else
			{
//				tr0=BGBCC_JX2C_ScratchAllocReg(
//					ctx, sctx, BGBCC_SH_REGCLS_FR2);
				tr0=BGBCC_JX2C_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_DR);
			}
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegLongP(ctx, sreg))
		if(BGBCC_CCXL_TypeSgLongP(ctx, type) ||
			BGBCC_CCXL_TypeVariantP(ctx, type))
		{
//			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR2);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type))
		{
			if(sctx->fpu_gfp)
			{
				tr0=BGBCC_JX2C_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_QGR);
			}else
				if(sctx->fpu_soft)
			{
				tr0=BGBCC_JX2C_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_GR);
			}else
			{
				tr0=BGBCC_JX2C_ScratchAllocReg(
					ctx, sctx, BGBCC_SH_REGCLS_FR);
			}

//			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);
//		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);

//		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, rcls);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

		return(1);
	}
}


int BGBCC_JX2C_EmitFpConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nm1, int nm2,
	ccxl_register dreg, ccxl_register sreg)
{
	ccxl_type sty, tty;
	char *s0;
	int lr4, lr0;
	int tr0;
	int cdreg, csreg;

	if(sctx->fpu_soft)
	{
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
//		dty=BGBCC_CCXL_GetRegType(ctx, dreg);

		s0=NULL;
		lr4=-1; lr0=-1;
	
		if(nm2==BGBCC_SH_NMID_FLOAT)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, dreg))
			{
				s0="__sfp_float_f64";
				lr4=BGBCC_SH_REG_RD4;
				lr0=BGBCC_SH_REG_RQ2;
			}else
			{
				s0="__sfp_float_f32";
				lr4=BGBCC_SH_REG_RD4;
				lr0=BGBCC_SH_REG_RD2;
			}
		}

		if(nm1==BGBCC_SH_NMID_FTRC)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
			{
				s0="__sfp_ftrc_f64";
				lr4=BGBCC_SH_REG_RQ4;
				lr0=BGBCC_SH_REG_RD2;
			}else
			{
				s0="__sfp_ftrc_f32";
				lr4=BGBCC_SH_REG_RD4;
				lr0=BGBCC_SH_REG_RD2;
			}
		}

		if(nm2==BGBCC_SH_NMID_FCNVSD)
		{
			s0="__sfp_fcnvsd";
			lr4=BGBCC_SH_REG_RD4;
			lr0=BGBCC_SH_REG_RQ2;
		}

		if(nm1==BGBCC_SH_NMID_FCNVDS)
		{
			s0="__sfp_fcnvds";
			lr4=BGBCC_SH_REG_RQ4;
			lr0=BGBCC_SH_REG_RD2;
		}

		if(!sctx->is_addr64)
		{
			if(lr4==BGBCC_SH_REG_RQ4)
				lr4=BGBCC_SH_REG_LR4;
			else
				lr4=BGBCC_SH_REG_R4;

			if(lr0==BGBCC_SH_REG_RQ2)
				lr0=BGBCC_SH_REG_LR2;
			else
				lr0=BGBCC_SH_REG_R2;
		}

		if(s0)
		{
			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr4);

			if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
			{
				tr0=BGBCC_SH_REG_R0;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
				{
					if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, lr4))
						{ BGBCC_JX2_EmitOpReg(sctx,
							BGBCC_SH_NMID_EXTUL, lr4); }
				}else
				{
					if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, lr4))
						{ BGBCC_JX2_EmitOpReg(sctx,
							BGBCC_SH_NMID_EXTSL, lr4); }
				}
			}

			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
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
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					nm1, csreg, BGBCC_SH_REG_FPUL);
				BGBCC_JX2C_EmitOpReg(ctx, sctx, nm2, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			if(nm1==BGBCC_SH_NMID_FTRC)
			{
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
				BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, csreg);
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					nm2, BGBCC_SH_REG_FPUL, cdreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
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
				lr0=BGBCC_SH_REG_DR2;
			}else
			{
				s0="__lfp_float_f32";
				lr4=BGBCC_SH_REG_R4;
				lr0=BGBCC_SH_REG_FR2;
			}
		}

		if(nm1==BGBCC_SH_NMID_FTRC)
		{
			if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
			{
				s0="__lfp_ftrc_f64";
				lr4=BGBCC_SH_REG_DR4;
				lr0=BGBCC_SH_REG_R2;
			}else
			{
				s0="__lfp_ftrc_f32";
				lr4=BGBCC_SH_REG_FR4;
				lr0=BGBCC_SH_REG_R2;
			}
		}

		if(nm2==BGBCC_SH_NMID_FCNVSD)
		{
			s0="__lfp_fcnvsd";
			lr4=BGBCC_SH_REG_FR4;
			lr0=BGBCC_SH_REG_DR2;
		}

		if(nm1==BGBCC_SH_NMID_FCNVDS)
		{
			s0="__lfp_fcnvds";
			lr4=BGBCC_SH_REG_DR4;
			lr0=BGBCC_SH_REG_FR2;
		}

		if(s0)
		{
			BGBCC_JX2C_EmitScratchSyncRegisters(ctx, sctx);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr4);
			BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, lr4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, s0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr4);
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, lr0);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, lr0);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, lr0);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
#endif

	if(sctx->fpu_gfp)
	{
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);

		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(nm2==BGBCC_SH_NMID_FLOAT)
		{
			if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
			{
				tr0=BGBCC_SH_REG_R0;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
				{
//					BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//						BGBCC_SH_NMID_EXTUL, csreg, tr0);
					if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, csreg))
						{ BGBCC_JX2_EmitOpReg(sctx,
							BGBCC_SH_NMID_EXTUL, csreg); }
				}else
				{
//					BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//						BGBCC_SH_NMID_EXTSL, csreg, tr0);
					if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, csreg))
						{ BGBCC_JX2_EmitOpReg(sctx,
							BGBCC_SH_NMID_EXTSL, csreg); }
				}
				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FLDCI, csreg, cdreg);
//				BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
//					BGBCC_SH_NMID_FLDCI, tr0, cdreg);
			}else if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
			{
				if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
				{
					BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
						BGBCC_SH_NMID_FLDCIU, csreg, cdreg);
				}else
				{
					BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
						BGBCC_SH_NMID_FLDCI, csreg, cdreg);
				}
			}else
			{
				BGBCC_DBGBREAK
			}
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		if(nm1==BGBCC_SH_NMID_FTRC)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_FSTCI, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		if(nm2==BGBCC_SH_NMID_FCNVSD)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		if(nm1==BGBCC_SH_NMID_FCNVDS)
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

#if 1
	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	if(	(	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, csreg) ||
			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, csreg)	) &&
		(	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, cdreg) ||
			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, cdreg)	) )
	{
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_FMOV, csreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(	(	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, csreg) ||
			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, csreg)	) &&
			BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, cdreg)		)
	{
		BGBCC_JX2C_EmitOpReg(ctx, sctx,
			BGBCC_SH_NMID_FSTCI, csreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_DLR, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, csreg)	&&
		(	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, cdreg) ||
			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, cdreg)	) )
	{
		if(!BGBCC_JX2_EmitCheckRegQuad(sctx, csreg))
		{
			BGBCC_JX2C_EmitOpReg(ctx, sctx,
				BGBCC_SH_NMID_EXTSL, csreg);
		}
	
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
			BGBCC_SH_NMID_MOV, csreg, BGBCC_SH_REG_DLR);
		BGBCC_JX2C_EmitOpReg(ctx, sctx,
			BGBCC_SH_NMID_FLDCI, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
#endif


	BGBCC_CCXL_StubError(ctx);
	return(0);

#if 0
	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
//	BGBCC_JX2C_EmitMovRegReg(ctx, sctx, csreg, cdreg);

	if(	BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, csreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, cdreg))
		{ BGBCC_JX2C_SetStatusFpscrDouble(ctx, sctx); }
	else
		{ BGBCC_JX2C_SetStatusFpscrFloat(ctx, sctx); }

	csreg=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, csreg);
	cdreg=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, cdreg);
	if((nm1==BGBCC_SH_NMID_LDS) || (nm1==BGBCC_SH_NMID_FLDS))
	{
		if(BGBCC_JX2_EmitCheckRegNonBaseGPR(sctx, csreg))
//		if(0)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, csreg, tr0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, tr0, BGBCC_SH_REG_FPUL);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, BGBCC_SH_REG_FPUL);
		}
	}
	else
		{ BGBCC_JX2C_EmitOpReg(ctx, sctx, nm1, csreg); }

	if((nm2==BGBCC_SH_NMID_STS) || (nm2==BGBCC_SH_NMID_FSTS))
	{
		if(BGBCC_JX2_EmitCheckRegNonBaseGPR(sctx, cdreg))
//		if(0)
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, BGBCC_SH_REG_FPUL, tr0);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
				BGBCC_SH_NMID_MOV, tr0, cdreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm2, BGBCC_SH_REG_FPUL, cdreg);
		}
	}
	else
		{ BGBCC_JX2C_EmitOpReg(ctx, sctx, nm2, cdreg); }

	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	return(1);
#endif
}


int BGBCC_JX2C_EmitMovVRegImm128(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, u64 immlo, u64 immhi)
{
	int tr0, tr1;
	int cdreg, csreg;
	int rcls;
	
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
		{ BGBCC_DBGBREAK }

	tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, cdreg);

//	BGBCC_JX2_EmitLoadRegImm64P(sctx, tr0+0, immlo);
//	BGBCC_JX2_EmitLoadRegImm64P(sctx, tr0+1, immhi);
	BGBCC_JX2_EmitLoadRegImm128P(sctx, tr0, immlo, immhi);

	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
	return(1);
}

int BGBCC_JX2C_EmitMovVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, s64 imm)
{
	int cdreg, csreg;
	int rcls;
	
	cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

	if(((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR)) &&
		BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, dreg))
	{
		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);
	}

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((((s32)imm)!=imm) &&
			BGBCC_JX2_EmitCheckRegQuad(sctx, cdreg))
		{
			BGBCC_JX2_EmitLoadRegImm64P(sctx, cdreg, imm);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
	
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg, imm);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, type);

	if((rcls==BGBCC_SH_REGCLS_QGR) || (rcls==BGBCC_SH_REGCLS_VO_QGR))
	{
		if(BGBCC_CCXL_TypeRealP(ctx, type))
			csreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
		else
			csreg=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);

//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, csreg, imm);
		BGBCC_JX2_EmitLoadRegImm64P(sctx, csreg, imm);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg);
		return(1);
	}

	if(BGBCC_CCXL_TypeRealP(ctx, type))
		csreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	else
		csreg=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

	BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, csreg, imm);
	BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, csreg);

//	BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, imm);
//	BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
	return(1);
}


int BGBCC_JX2C_EmitOpNmidVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type, int nmid,
	ccxl_register dreg, ccxl_register sreg)
{
	int cdreg, csreg, ctreg;

	BGBCC_JX2C_NormalizeImmVRegSoftFloat(ctx, sctx, type, &sreg);

	if(BGBCC_CCXL_IsRegFloatP(ctx, dreg) ||
		BGBCC_CCXL_IsRegDoubleP(ctx, dreg))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmUIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			BGBCC_DBGBREAK
		}
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		cdreg=BGBCC_JX2C_EmitGetRegisterDirty(ctx, sctx, dreg);

		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nmid, cdreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

//	cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
//	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);

	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nmid, csreg, cdreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nmid, cdreg, cdreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
//			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			ctreg=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nmid, csreg, ctreg);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

//		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		ctreg=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, ctreg);
		BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nmid, ctreg, ctreg);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitJCmpVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg, flip, noflip, jcflip;
	s32 imm, immj;
	int nm1, nm2, nm3, is_imm10un;
	int i, j, k;
	
	noflip=0;

#if 0
	if(BGBCC_CCXL_IsRegFloatP(ctx, sreg) ||
		BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmUIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg))
		{
			BGBCC_DBGBREAK
		}
	}
#endif
	
	if((	BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, sreg)) &&
		!(	BGBCC_CCXL_IsRegImmIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, treg)))
	{
		nm1=cmp;
		switch(cmp)
		{
			case CCXL_CMP_EQ:		nm1=CCXL_CMP_EQ; break;
			case CCXL_CMP_NE:		nm1=CCXL_CMP_NE; break;
			case CCXL_CMP_LT:		nm1=CCXL_CMP_GT; break;
			case CCXL_CMP_GT:		nm1=CCXL_CMP_LT; break;
			case CCXL_CMP_LE:		nm1=CCXL_CMP_GE; break;
			case CCXL_CMP_GE:		nm1=CCXL_CMP_LE; break;
			case CCXL_CMP_TST:		nm1=CCXL_CMP_TST; break;
			case CCXL_CMP_NTST:		nm1=CCXL_CMP_NTST; break;
		}
		
		return(BGBCC_JX2C_EmitJCmpVRegVRegInt(
			ctx, sctx, type, treg, sreg, nm1, lbl));
	}

	if((type.val==CCXL_TY_UB) || (type.val==CCXL_TY_US))
		type.val=CCXL_TY_I;

	imm=0; immj=-1;
	is_imm10un=0;

#if 1
//	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	if(BGBCC_CCXL_IsRegImmSmallSIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		immj=imm;

		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			is_imm10un=1;
			
		if(sctx->has_jumbo && !(sctx->has_jcmp&2))
			is_imm10un=1;

//		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
		if(((imm&1023)==imm) || ((imm|(~1023))==imm) || sctx->has_jumbo)
			noflip=1;

//		if(sctx->is_betav && !sctx->has_bjx1ari)
//		if(sctx->is_betav && (!sctx->has_bjx1ari || !sctx->has_bjx1mov))
//			noflip=0;
	}
#endif

	if((imm==immj) && (imm==0))
	{
		return(BGBCC_JX2C_EmitJCmpVRegZeroInt(
			ctx, sctx, type, sreg, cmp, lbl));
	}
	
	nm3=-1; flip=0;
	jcflip=0;
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

				nm3=BGBCC_SH_NMID_BRLTUL;
				jcflip=0;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BT;

				nm3=BGBCC_SH_NMID_BRLTL;
				jcflip=0;
//				nm3=-1;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BF;

				nm3=BGBCC_SH_NMID_BRLTUL;
				jcflip=0;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BF;

				nm3=BGBCC_SH_NMID_BRLTL;
				jcflip=0;
//				nm3=-1;
			}
		}
//		nm3=-1;
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BT;

			nm3=BGBCC_SH_NMID_BRGTUL;
			jcflip=0;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BT;

			nm3=BGBCC_SH_NMID_BRGTL;
			jcflip=0;
//			nm3=-1;
		}
//		nm3=-1;
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BF;

			nm3=BGBCC_SH_NMID_BRLEUL;
			jcflip=0;
//			nm3=-1;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BF;

			nm3=BGBCC_SH_NMID_BRLEL;
			jcflip=0;
//			nm3=-1;
		}
//		nm3=-1;
		break;
	case CCXL_CMP_GE:
		if(!noflip)
		{
			flip=1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHI;
				nm2=BGBCC_SH_NMID_BF;

//				nm3=BGBCC_SH_NMID_BRGTUL;
				nm3=BGBCC_SH_NMID_BRLEUL;
				jcflip=1;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGT;
				nm2=BGBCC_SH_NMID_BF;

				nm3=BGBCC_SH_NMID_BRLEL;
				jcflip=1;
//				nm3=-1;
			}
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				nm1=BGBCC_SH_NMID_CMPHS;
				nm2=BGBCC_SH_NMID_BT;

				nm3=BGBCC_SH_NMID_BRLEUL;
				jcflip=1;
			}else
			{
				nm1=BGBCC_SH_NMID_CMPGE;
				nm2=BGBCC_SH_NMID_BT;

				nm3=BGBCC_SH_NMID_BRLEL;
				jcflip=1;
//				nm3=-1;
			}
		}
//		nm3=-1;
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BF;

//		if(imm!=immj)
		if(0)
		{
			nm3=BGBCC_SH_NMID_BRTSTNL;
			jcflip=0;
		}

		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BT;

//		if(imm!=immj)
		if(0)
		{
			nm3=BGBCC_SH_NMID_BRTSTL;
			jcflip=0;
		}
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}

//	nm3=-1;
	
	if((nm1>=0) && (nm2>=0))
	{
//		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) && !flip)
//		if(BGBCC_CCXL_IsRegImmSmallSIntP(ctx, treg) && !flip)
		if(BGBCC_CCXL_IsRegImmSmallSIntP(ctx, treg) && is_imm10un && !flip)
		{
			imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

//			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
//			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
				BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
				BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

//			ctreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, treg);
//			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
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
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1) && !flip &&
			BGBCC_JX2_EmitCheckRegBaseGPR(sctx, csreg) &&
			BGBCC_JX2_EmitCheckRegBaseGPR(sctx, ctreg))
		{
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2C_SetModeDqClear(ctx, sctx);

			BGBCC_JX2_EmitOpCmpRegRegLabel(sctx, nm3, ctreg, csreg, lbl);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

#if 1
//		if(nm3>=0)
		if((nm3>=0) && (sctx->has_jcmp&2) &&
			BGBCC_JX2_EmitCheckRegExtGPR(sctx, csreg) &&
			BGBCC_JX2_EmitCheckRegExtGPR(sctx, ctreg))
		{
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2C_SetModeDqClear(ctx, sctx);

			if(jcflip)
				j=BGBCC_JX2_TryEmitOpRegRegLbl(sctx, nm3, csreg, ctreg, lbl);
			else
				j=BGBCC_JX2_TryEmitOpRegRegLbl(sctx, nm3, ctreg, csreg, lbl);
			if(j>0)
			{
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}
#endif

//		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_JX2_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_JX2_EmitOpLabel(sctx, nm2, lbl);
		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
//	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
//	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	if(lbl==CCXL_LBL_RETURN)
		lbl=sctx->lbl_ret;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		(BGBCC_CCXL_TypePointerP(ctx, type) && !sctx->is_addr64))
//		(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) && !sctx->is_addr64))
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) && 
		BGBCC_CCXL_TypeQuadPointerP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegInt128(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

//	if(BGBCC_CCXL_TypePointerP(ctx, type) && sctx->is_addr_x32)
//	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) && sctx->is_addr_x32)
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) &&
		(ctx->arch_sizeof_ptr==4))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) &&
		(ctx->arch_sizeof_ptr==8))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{
		if(sctx->fpu_soft)
		{
			return(BGBCC_JX2C_EmitJCmpVRegVRegSoftFloat(ctx, sctx,
				type, sreg, treg, cmp, lbl));
		}else
		{
			return(BGBCC_JX2C_EmitJCmpVRegVRegFloat(ctx, sctx,
				type, sreg, treg, cmp, lbl));
		}
	}

#if 1
	if((BGBCC_CCXL_TypeSgLongP(ctx, type) ||
		BGBCC_CCXL_TypeSgNLongP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type)) &&
		BGBCC_CCXL_TypeArrayOrPointerP(ctx, type)) &&
			sctx->is_addr64)
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}
#endif

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegInt128(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegFloat128(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegBitInt(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeVarRefP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegVariant(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeVecP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegVRegVec(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegZeroInt(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	int csreg, ctreg;
	int nm1, nm2, nm3;
	int i, j, k;

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

	if(cmp==CCXL_CMP_TST)
	{
//		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
		return(1);
	}

	if(cmp==CCXL_CMP_NTST)
	{
		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
		return(1);
	}
	
	nm1=-1; nm2=-1; nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQL;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNEL;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLTL;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGTL;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLEL;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGEL;
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
		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);

#if 0
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, csreg))
		{
			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			return(BGBCC_JX2C_EmitJCmpVRegVRegInt(ctx, sctx,
				type, sreg, treg, cmp, lbl));

//			return(1);
		}
#endif

#if 0
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1) &&
			BGBCC_JX2_EmitCheckRegBaseGPR(sctx, csreg))
		{
//			BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
			BGBCC_JX2C_SetModeDqClear(ctx, sctx);

			BGBCC_JX2_EmitOpCmpRegLabel(sctx, nm3, csreg, lbl);
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

		BGBCC_JX2C_SetModeDqClear(ctx, sctx);

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
	
	BGBCC_CCXL_StubError(ctx);
//	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}

int BGBCC_JX2C_EmitJCmpVRegZero(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	
	if(lbl==CCXL_LBL_RETURN)
		lbl=sctx->lbl_ret;
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

//	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
//		BGBCC_CCXL_TypeArrayP(ctx, type)) &&
//			(!sctx->is_addr64 || sctx->is_addr_x32))
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type) &&
		(ctx->arch_sizeof_ptr==4))
	{
		return(BGBCC_JX2C_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

#if 1
	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) &&
		(ctx->arch_sizeof_ptr==8))
	{
		return(BGBCC_JX2C_EmitJCmpVRegZeroQLong(ctx, sctx,
			type, sreg, cmp, lbl));
	}
#endif

#if 0
	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		if(!sctx->fpu_soft)
		{
			return(BGBCC_JX2C_EmitJCmpVRegZeroFloat(ctx, sctx,
				type, sreg, treg, cmp, lbl));
		}
	}
#endif

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
//		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
//		return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
//			type, sreg, treg, cmp, lbl));
		return(BGBCC_JX2C_EmitJCmpVRegZeroQLong(ctx, sctx,
			type, sreg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeSgNLongP(ctx, type) &&
		(ctx->arch_sizeof_long==4))
	{
//		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
//		return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
//			type, sreg, treg, cmp, lbl));
		return(BGBCC_JX2C_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeSgNLongP(ctx, type) &&
		(ctx->arch_sizeof_long==8))
	{
//		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
//		return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
//			type, sreg, treg, cmp, lbl));
		return(BGBCC_JX2C_EmitJCmpVRegZeroQLong(ctx, sctx,
			type, sreg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, type))
		{
			BGBCC_CCXL_GetRegForInt128Value(ctx, &treg, 0, 0);
			return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
				type, sreg, treg, cmp, lbl));
		}else
		{
			BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
			return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
				type, sreg, treg, cmp, lbl));
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{
		BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 0.0);
		return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 0.0);
		return(BGBCC_JX2C_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeVecP(ctx, type))
	{
		return(BGBCC_JX2C_EmitJCmpVRegZeroVec(ctx, sctx,
			type, sreg, cmp, lbl));
	}


	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2C_EmitJmpTab(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	s64 vmin, int nlbl,
	ccxl_label *lbls)
{
	ccxl_label dfl;
	int csreg, ctreg;
	int i, j, k;

//	dfl=lbls[nlbl];
	dfl=lbls[nlbl+1];

	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

	BGBCC_JX2C_EmitOpRegImmReg(ctx, sctx,
		BGBCC_SH_NMID_SUB, csreg, vmin, ctreg);

	if(dfl.id)
	{
		BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
			BGBCC_SH_NMID_CMPHI, nlbl, ctreg);
		BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, dfl.id);
	}else
	{
		dfl=lbls[nlbl];
		if(dfl.id)
		{
			BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
				BGBCC_SH_NMID_CMPHI, nlbl, ctreg);
			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, dfl.id);
		}else
		{
			BGBCC_DBGBREAK
		}
	}

//	sctx->op_wex_align|=1;

	BGBCC_JX2_EmitPadForLabel(sctx);

//	BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_SHLL1, ctreg);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ctreg, ctreg);
//	BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_BRA2F, ctreg);
	BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_BRAF, ctreg);
//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);

	for(i=0; i<=nlbl; i++)
	{
//		if(!BGBCC_JX2_CheckPadAlign32(sctx) && !(sctx->is_simpass))
//			{ BGBCC_DBGBREAK }
		j=BGBCC_JX2_TryEmitOpFar20Label(sctx, BGBCC_SH_NMID_BRAN, lbls[i].id);
		if(j<=0)
			{ BGBCC_DBGBREAK }
//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
	}


	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
	BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
	return(1);
}

int BGBCC_JX2C_EmitInitObj(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg)
{
	BGBCC_CCXL_RegisterInfo *obj;
	int ctreg, rcls;
	int i, j, k;
	
	obj=ctx->cur_func;

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		i=sreg.val&CCXL_REGID_BASEMASK;
	
		if(obj->regs[i]->regflags&BGBCC_REGFL_CULL)
			return(0);

		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);

		if((rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF))
		{
			j=obj->regs[i]->fxmoffs+(sctx->frm_offs_fix);
			k=obj->regs[i]->fxoffs;

			ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//			ctreg=BGBCC_JX2C_ScratchAllocTsReg(ctx, sctx, 0);
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R3);
			if(sctx->is_addr64)
			{
				BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, j, ctreg);
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, ctreg);
			}else
			{
				BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, j, ctreg);
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, ctreg);
			}
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);
		}
		
		return(1);
	}
	
	return(0);
}	

/*
int BGBCC_JX2C_EmitMovVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, s64 imm)
*/

int BGBCC_JX2C_SizeofVar(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register dreg)
{
	ccxl_type tty;
	int tsz;

	tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
	
	if(tsz<=0)
	{
		BGBCC_CCXL_StubError(ctx);
	}

	tty=BGBCC_CCXL_GetRegType(ctx, dreg);
	BGBCC_JX2C_EmitMovVRegImm(ctx, sctx, tty, dreg, tsz);

//	BGBCC_CCXL_StubError(ctx);

	return(1);
}



int BGBCC_JX2C_EmitPredCmpVRegVRegInt(
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
	
	if((type.val==CCXL_TY_UB) || (type.val==CCXL_TY_US))
		type.val=CCXL_TY_I;

#if 1
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		if(((imm&1023)==imm) || ((imm|(~1023))==imm))
			noflip=1;
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

			csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_JX2_TryEmitOpImmReg(sctx, nm1, imm, csreg)>0)
			{
//				BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//				BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);

				sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//				if(nm2==BGBCC_SH_NMID_BT)
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOTT);

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

		BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx);

		if(flip)
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, csreg, ctreg);
		else
			BGBCC_JX2C_EmitOpRegReg(ctx, sctx, nm1, ctreg, csreg);

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
//		BGBCC_JX2_EmitOpAutoLabel(sctx, nm2, lbl);

		sctx->pred_tfmd=(nm2==BGBCC_SH_NMID_BT);

//		if(nm2==BGBCC_SH_NMID_BT)
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOTT);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitPredCmpVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp)
{
//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		(BGBCC_CCXL_TypePointerP(ctx, type) && !sctx->is_addr64))
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_JX2C_EmitPredCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp));
	}

	if(	BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		return(BGBCC_JX2C_EmitPredCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp));
	}

//	if(BGBCC_CCXL_TypePointerP(ctx, type) && sctx->is_addr_x32)
	if(	BGBCC_CCXL_TypePointerP(ctx, type) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, type) &&
		(ctx->arch_sizeof_ptr==4))
	{
		return(BGBCC_JX2C_EmitPredCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp));
	}

//	if(BGBCC_CCXL_TypePointerP(ctx, type) && sctx->is_addr64)
	if(	BGBCC_CCXL_TypePointerP(ctx, type) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, type) &&
		(ctx->arch_sizeof_ptr==8))
	{
		return(BGBCC_JX2C_EmitPredCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
