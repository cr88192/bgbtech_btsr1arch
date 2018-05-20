/*
 * R0/R1/R2/R3: Return Value, Scratch
 *   R0: ReturnA, Default Const-Load
 *   R1: ReturnB, Scratch
 *   R2: Struct Return Pointer
 *   R3: Temp Opr Working Reg
 * R4/R5/R6/R7: Arg Passing/Scratch
 * R8..R11: Var Cache (Callee Save)
 * R12: Var Cache / Context Pointer (GOT)
 * R13: Var Cache (Callee Save)
 * R14: Var Cache / Frame Pointer
 * R15: Stack Pointer
 * FR5,FR4,FR7,FR6,FR9,FR8,FR11,FR10: Float/Double Args, Scratch
 * FR12..FR15: Var Cache (Callee Save)
 *
 * Frame (Frame-Ref Point)
 *   (- 4) Saved PR
 *   (- 8) Saved R14
 *   (-12) Saved R13
 *   (-16) Saved R12
 *   (-20) Saved R11
 *   (-24) Saved R10
 *   (-28) Saved R9
 *   (-32) Saved R8
 *   (-36) Saved FR15
 *   (-40) Saved FR14
 *   (-44) Saved FR13
 *   (-48) Saved FR12
 *   ...
 *   SP (Stack Ref Point)
 */

int BGBCC_BSRC_ScratchCheckRegFree(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=8)
			return(0);
		if(sctx->sreg_live&(1<<(reg&15)))
			return(0);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			return(0);
		if(sctx->sreg_live&(3<<(reg&15)))
			return(0);
		return(1);
	}

	return(0);
}

int BGBCC_BSRC_ScratchSafeStompReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(1<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_BSRC_ScratchStompReg(ctx, sctx, reg));
	}

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(1<<(reg&31)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_BSRC_ScratchStompReg(ctx, sctx, reg));
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(3<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_BSRC_ScratchStompReg(ctx, sctx, reg));
	}

	return(BGBCC_BSRC_ScratchStompReg(ctx, sctx, reg));
}

int BGBCC_BSRC_ScratchStompReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<(reg&15);
		return(1);
	}

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }
	
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<(reg&31);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=3<<((reg&15)+0);
//		sctx->sreg_live|=1<<((reg&15)+1);
		return(1);
	}

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=3<<(reg&15);
//		sctx->sfreg_live|=3<<(2*(reg&7)+0);
//		sctx->sfreg_live|=1<<(2*(reg&7)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<(reg&15);
		return(1);
	}

	return(0);
}

int BGBCC_BSRC_ScratchHoldReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<(reg&15);
		return(1);
	}

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<(reg&31);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<((reg&15)+0);
		sctx->sreg_held|=1<<((reg&15)+1);
		return(1);
	}

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
//		sctx->sfreg_held|=1<<(2*(reg&7)+0);
//		sctx->sfreg_held|=1<<(2*(reg&7)+1);
		sctx->sfreg_held|=1<<((reg&15)+0);
		sctx->sfreg_held|=1<<((reg&15)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<(reg&15);
		return(1);
	}

	return(0);
}

int BGBCC_BSRC_ScratchReleaseReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1<<(reg&15));
		sctx->sreg_live&=~(1<<(reg&15));
		return(1);
	}

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1<<(reg&31));
		sctx->sreg_live&=~(1<<(reg&31));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1<<(reg&15));
		sctx->sfreg_live&=~(1<<(reg&15));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(3<<((reg&15)+0));
//		sctx->sreg_held&=~(1<<((reg&15)+1));
		sctx->sreg_live&=~(3<<((reg&15)+0));
//		sctx->sreg_live&=~(1<<((reg&15)+1));
		return(1);
	}

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=12)
			{ BGBCC_DBGBREAK }

		sctx->sfreg_held&=~(3<<(reg&15));
		sctx->sfreg_live&=~(3<<(reg&15));

//		sctx->sfreg_held&=~(3<<(2*(reg&7)+0));
//		sctx->sfreg_live&=~(3<<(2*(reg&7)+0));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1<<(reg&15));
		sctx->sreg_live&=~(1<<(reg&15));
		return(1);
	}

	return(0);
}

int BGBCC_BSRC_ScratchAllocReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int cls)
{
	int i;

	if(!cls ||
//		(cls==BGBCC_SH_REGCLS_GR) ||
//		(cls==BGBCC_SH_REGCLS_VO_GR) ||
		(cls==BGBCC_SH_REGCLS_QGR) ||
		(cls==BGBCC_SH_REGCLS_VO_QGR) ||
		(cls==BGBCC_SH_REGCLS_VO_REF) ||
		(cls==BGBCC_SH_REGCLS_AR_REF))
	{
//		for(i=1; i<8; i++)
		for(i=7; i>=0; i--)
		{
			if(!(sctx->sreg_live&(1<<i)))
			{
				sctx->sreg_live|=(1<<i);
				if(sctx->is_addr64)
					return(BGBCC_SH_REG_RQ0+i);
				return(BGBCC_SH_REG_R0+i);
			}
		}

		if(sctx->has_bjx1egpr)
		{
			for(i=23; i>=16; i--)
			{
				if(!(sctx->sreg_live&(1<<i)))
				{
					sctx->sreg_live|=(1<<i);
					if(sctx->is_addr64)
						return(BGBCC_SH_REG_RQ0+i);
					return(BGBCC_SH_REG_R0+i);
				}
			}
		}

		BGBCC_DBGBREAK
		return(-1);
	}

	if(!cls ||
		(cls==BGBCC_SH_REGCLS_GR) ||
		(cls==BGBCC_SH_REGCLS_VO_GR))
	{
//		for(i=1; i<8; i++)
		for(i=7; i>=0; i--)
		{
			if(!(sctx->sreg_live&(1<<i)))
			{
				sctx->sreg_live|=(1<<i);
				if(sctx->is_addr64)
					return(BGBCC_SH_REG_RD0+i);
				return(BGBCC_SH_REG_R0+i);
			}
		}
		if(sctx->has_bjx1egpr)
		{
			for(i=23; i>=16; i--)
			{
				if(!(sctx->sreg_live&(1<<i)))
				{
					sctx->sreg_live|=(1<<i);
					if(sctx->is_addr64)
						return(BGBCC_SH_REG_RD0+i);
					return(BGBCC_SH_REG_R0+i);
				}
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if(cls==BGBCC_SH_REGCLS_FR)
	{
		for(i=0; i<12; i++)
		{
			if(!(sctx->sfreg_live&(1<<i)))
			{
				sctx->sfreg_live|=(1<<i);
				return(BGBCC_SH_REG_FR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if((cls==BGBCC_SH_REGCLS_GR2) ||
		(cls==BGBCC_SH_REGCLS_VO_GR2) ||
		(cls==BGBCC_SH_REGCLS_VO_QGR2))
	{
//		for(i=1; i<7; i++)
		for(i=6; i>=0; i--)
		{
			if(!(sctx->sreg_live&(3<<i)))
			{
				sctx->sreg_live|=(3<<i);
				return(BGBCC_SH_REG_LR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if(cls==BGBCC_SH_REGCLS_FR2)
	{
//		for(i=0; i<6; i++)
		for(i=0; i<12; i+=2)
		{
			if(!(sctx->sfreg_live&(3<<i)))
			{
				sctx->sfreg_live|=(3<<i);
				return(BGBCC_SH_REG_DR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	BGBCC_DBGBREAK
	return(-1);
}

int BGBCC_BSRC_SaveFrameOfsForReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	int ofs;
	
	if(!BGBCC_BSR_EmitCheckRegExtGPR(sctx, reg))
		return(0);
	
	ofs=0;
//	switch(reg)
	switch(reg&31)
	{
	case BGBCC_SH_REG_PR: ofs=-4; break;
	case BGBCC_SH_REG_R14: ofs=-8; break;
	case BGBCC_SH_REG_R13: ofs=-12; break;
	case BGBCC_SH_REG_R12: ofs=-16; break;
	case BGBCC_SH_REG_R11: ofs=-20; break;
	case BGBCC_SH_REG_R10: ofs=-24; break;
	case BGBCC_SH_REG_R9: ofs=-28; break;
	case BGBCC_SH_REG_R8: ofs=-32; break;

	case BGBCC_SH_REG_R31: ofs=-36; break;
	case BGBCC_SH_REG_R30: ofs=-40; break;
	case BGBCC_SH_REG_R29: ofs=-44; break;
	case BGBCC_SH_REG_R28: ofs=-48; break;
	case BGBCC_SH_REG_R27: ofs=-52; break;
	case BGBCC_SH_REG_R26: ofs=-56; break;
	case BGBCC_SH_REG_R25: ofs=-60; break;
	case BGBCC_SH_REG_R24: ofs=-64; break;

//	case BGBCC_SH_REG_RQ15: ofs=-8; break;
	case BGBCC_SH_REG_RQ14: ofs=-8; break;
	case BGBCC_SH_REG_RQ13: ofs=-12; break;
	case BGBCC_SH_REG_RQ12: ofs=-16; break;
	case BGBCC_SH_REG_RQ11: ofs=-20; break;
	case BGBCC_SH_REG_RQ10: ofs=-24; break;
	case BGBCC_SH_REG_RQ9: ofs=-28; break;
	case BGBCC_SH_REG_RQ8: ofs=-32; break;

//	case BGBCC_SH_REG_RD15: ofs=-8; break;
	case BGBCC_SH_REG_RD14: ofs=-8; break;
	case BGBCC_SH_REG_RD13: ofs=-12; break;
	case BGBCC_SH_REG_RD12: ofs=-16; break;
	case BGBCC_SH_REG_RD11: ofs=-20; break;
	case BGBCC_SH_REG_RD10: ofs=-24; break;
	case BGBCC_SH_REG_RD9: ofs=-28; break;
	case BGBCC_SH_REG_RD8: ofs=-32; break;

	case BGBCC_SH_REG_RQ31: ofs=-36; break;
	case BGBCC_SH_REG_RQ30: ofs=-40; break;
	case BGBCC_SH_REG_RQ29: ofs=-44; break;
	case BGBCC_SH_REG_RQ28: ofs=-48; break;
	case BGBCC_SH_REG_RQ27: ofs=-52; break;
	case BGBCC_SH_REG_RQ26: ofs=-56; break;
	case BGBCC_SH_REG_RQ25: ofs=-60; break;
	case BGBCC_SH_REG_RQ24: ofs=-64; break;

	case BGBCC_SH_REG_RD31: ofs=-36; break;
	case BGBCC_SH_REG_RD30: ofs=-40; break;
	case BGBCC_SH_REG_RD29: ofs=-44; break;
	case BGBCC_SH_REG_RD28: ofs=-48; break;
	case BGBCC_SH_REG_RD27: ofs=-52; break;
	case BGBCC_SH_REG_RD26: ofs=-56; break;
	case BGBCC_SH_REG_RD25: ofs=-60; break;
	case BGBCC_SH_REG_RD24: ofs=-64; break;
	}
	
	if(sctx->is_addr64)
		ofs*=2;
	
	return(ofs);
}

int BGBCC_BSRC_SaveFrameOfsForFpReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	int ofs;
	
	ofs=0;
	switch(reg)
	{
	case BGBCC_SH_REG_FR15: ofs=-36; break;
	case BGBCC_SH_REG_FR14: ofs=-40; break;
	case BGBCC_SH_REG_FR13: ofs=-44; break;
	case BGBCC_SH_REG_FR12: ofs=-48; break;
	}

	if(sctx->is_addr64)
		ofs*=2;

	return(ofs);
}

int BGBCC_BSRC_EmitSaveFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
#if 1
	int ofs;
	ofs=BGBCC_BSRC_SaveFrameOfsForReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(sctx->reg_save&(1<<(reg&15)))
		if(sctx->reg_save&(1<<(reg&31)))
			return(0);

//		if(!sctx->is_simpass && !(sctx->reg_vsave&(1<<(reg&15))))
		if(!sctx->is_simpass && !(sctx->reg_vsave&(1<<(reg&31))))
			{ BGBCC_DBGBREAK }

//		sctx->reg_save|=1<<(reg&15);
		sctx->reg_save|=1<<(reg&31);
//		BGBCC_BSRC_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_BSRC_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(sctx->freg_save&(1<<(reg&15)))
		if(sctx->freg_save&(1<<(reg&31)))
			return(0);

//		if(!sctx->is_simpass && !(sctx->freg_vsave&(1<<(reg&15))))
		if(!sctx->is_simpass && !(sctx->freg_vsave&(1<<(reg&31))))
			{ BGBCC_DBGBREAK }

//		sctx->freg_save|=1<<(reg&15);
		sctx->freg_save|=1<<(reg&31);
//		BGBCC_BSRC_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}
#endif

	return(0);
}

int BGBCC_BSRC_EmitReloadSavedFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int reg)
{
	int ofs;

#if 0
	ofs=BGBCC_BSRC_SaveFrameOfsForReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(!(sctx->reg_save&(1<<(reg&15))))
		if(!(sctx->reg_save&(1<<(reg&31))))
			return(0);

//		sctx->reg_save|=1<<(reg&15);
		BGBCC_BSRC_EmitLoadFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_BSRC_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(!(sctx->freg_save&(1<<(reg&15))))
		if(!(sctx->freg_save&(1<<(reg&31))))
			return(0);

		BGBCC_BSRC_EmitLoadFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}
#endif

	return(0);
}

#if 0
// const byte bgbcc_bsr_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
// const byte bgbcc_bsr_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const byte bgbcc_bsr_cachereg[8]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8, BGBCC_SH_REG_ZZR };
const byte bgbcc_bsr_jcachereg[8]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8, BGBCC_SH_REG_ZZR };
const byte bgbcc_bsr_qcachereg[8]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8, BGBCC_SH_REG_ZZR };
const byte bgbcc_bsr_maxreg=7;
#endif

#if 1
// const byte bgbcc_bsr_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
// const byte bgbcc_bsr_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const byte bgbcc_bsr_cachereg[16]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8, 
	BGBCC_SH_REG_R31, BGBCC_SH_REG_R30,
	BGBCC_SH_REG_R29, BGBCC_SH_REG_R28,
	BGBCC_SH_REG_R27, BGBCC_SH_REG_R26,
	BGBCC_SH_REG_R25, BGBCC_SH_REG_R24, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_bsr_jcachereg[16]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD31, BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29, BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD27, BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25, BGBCC_SH_REG_RD24, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_bsr_qcachereg[16]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ31, BGBCC_SH_REG_RQ30,
	BGBCC_SH_REG_RQ29, BGBCC_SH_REG_RQ28,
	BGBCC_SH_REG_RQ27, BGBCC_SH_REG_RQ26,
	BGBCC_SH_REG_RQ25, BGBCC_SH_REG_RQ24, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_bsr_maxreg=7;
const byte bgbcc_bsr_maxreg_egpr=15;
#endif

/* Try to get Variable as register.
 * Will return a value as a register if possible, but may fail.
 * Returns ZZR if it is unable to do so.
 */
int BGBCC_BSRC_EmitTryGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int creg, excl, nsv, userq, rcls;
	int pr0, pr1;
	int i, bi;

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_BSRC_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//		BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//	{
//		return(BGBCC_BSRC_EmitTryGetFpRegister(ctx, sctx, reg, fl));
//	}

//	if(	BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//		BGBCC_CCXL_IsRegFloat16P(ctx, reg))
//	if(rcls==BGBCC_SH_REGCLS_FR)
//	{
//		return(BGBCC_BSRC_EmitTryGetFpRegister(ctx, sctx, reg, fl));
//	}

//	if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))

//	if(rcls==BGBCC_SH_REGCLS_FR2)
//	{
//		return(BGBCC_BSRC_EmitTryGetDpRegister(ctx, sctx, reg, fl));
//	}

//	if((BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
//		BGBCC_CCXL_IsRegVariantP(ctx, reg)) && !(sctx->is_addr64))
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2))
//	if(rcls==BGBCC_SH_REGCLS_GR2)
	if(rcls==BGBCC_SH_REGCLS_GR2)
	{
		return(BGBCC_BSRC_EmitTryGetLpRegister(ctx, sctx, reg, fl));
	}

	switch(rcls)
	{
	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_VO_REF:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_GR2:
	case BGBCC_SH_REGCLS_VO_QGR2:
	case BGBCC_SH_REGCLS_AR_REF:
		if(sctx->is_addr64)
			userq=1;
		else
			userq=0;
		break;
	default:
		userq=0;
		break;
	}

#if 0
	if((BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
		BGBCC_CCXL_IsRegPointerP(ctx, reg)) &&
		(sctx->is_addr64))
			{ userq=1; }
		else
			{ userq=0; }
#endif

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

#if 1
	/* value already in a register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
	{
		if(excl&(1<<i))
			continue;

//		if(!((sctx->regalc_live)&(1<<i)))
//			continue;

		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=1<<i;
			if(fl&1)
			{
				sctx->regalc_dirty|=1<<i;
				sctx->regalc_map[i]=reg;
			}

			if(sctx->regalc_ltcnt[i]>0)
				sctx->regalc_ltcnt[i]--;
			sctx->regalc_utcnt[i]++;
			creg=bgbcc_bsr_cachereg[i];
			if(userq)
				creg=bgbcc_bsr_qcachereg[i];
			else if(sctx->is_addr64)
				creg=bgbcc_bsr_jcachereg[i];
			return(creg);
		}
	}
#endif

	pr0=BGBCC_BSRC_GetVRegPriority(ctx, sctx, reg);

//	if((fl&3)==1)
	if(((fl&3)==1) || ((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv)))
	{
		bi=-1; nsv=0;
//		pr0=BGBCC_BSRC_GetVRegPriority(ctx, sctx, reg);
	
		/* Check for registers not holding a live value. */
//		for(i=0; i<5; i++)
//		for(i=0; i<bgbcc_bsr_maxreg; i++)
		for(i=0; i<sctx->maxreg_gpr; i++)
		{
			if(excl&(1<<i))
				continue;

			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

			if(!((sctx->regalc_save)&(1<<i)))
			{
				creg=bgbcc_bsr_cachereg[i];
//				if(sctx->reg_save&(1<<(creg&15)))
				if(sctx->reg_save&(1<<(creg&31)))
					{ bi=i; break; }

				nsv++;
				continue;
			}

//			if(!((sctx->regalc_live)&(1<<i)))
			if(!((sctx->regalc_live)&(1<<i)) &&
				!((sctx->regalc_dirty)&(1<<i)))
			{
				reg1=sctx->regalc_map[i];
				pr1=BGBCC_BSRC_GetVRegPriority(ctx, sctx, reg1);

				if(BGBCC_BSRC_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i; break; }
				else
				{
//					if((bi<0) || 
//						(sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
//					if(pr0<=pr1)
					if((bi<0) ||
						((sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]) &&
						(pr0<=pr1)))
					{
//						if((pr1>=0) && (pr0>=0) && (pr0>=pr1))
							bi=i;
					}
				}
			}
		}

		/* Check if we are better off allocating another register */
		if(bi>=0)
		{
			reg1=sctx->regalc_map[bi];
			if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
				sctx->is_simpass &&
				!BGBCC_BSRC_CheckVRegIsZRegP(ctx, sctx, reg1))
			{
				bi=-1;
			}
		}

		if((pr0>=0) && (pr0<sctx->vsp_rsv))
			bi=pr0;
		
#if 1
		if(bi>=0)
		{
			i=bi;

			BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i);
			BGBCC_BSRC_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=1<<i;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_map[i]=reg;
//			sctx->regalc_utcnt[i]=1;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_live|=1<<i;
			if(fl&1)
				{ sctx->regalc_dirty|=1<<i; }
			creg=bgbcc_bsr_cachereg[i];
			if(userq)
				creg=bgbcc_bsr_qcachereg[i];
			else if(sctx->is_addr64)
				creg=bgbcc_bsr_jcachereg[i];
			BGBCC_BSRC_EmitSaveFrameReg(ctx, sctx, creg);
//			if(fl&2)
//				{ BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
#endif
	}

	return(BGBCC_SH_REG_ZZR);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_BSRC_EmitGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int creg, lng, excl, bi, nsv, userq, rcls;
	int pr0, pr1;
	int i;

	creg=BGBCC_BSRC_EmitTryGetRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_BSRC_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//		BGBCC_CCXL_IsRegFloat16P(ctx, reg) ||
//		BGBCC_CCXL_IsRegDoubleP(ctx, reg))

//	if((rcls==BGBCC_SH_REGCLS_FR) ||
//		(rcls==BGBCC_SH_REGCLS_FR2))
//	{
//		return(BGBCC_BSRC_EmitGetFpRegister(ctx, sctx, reg, fl));
//	}
	
//	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) && !(sctx->is_addr64))
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2))
	if(rcls==BGBCC_SH_REGCLS_GR2)
	{
		return(BGBCC_BSRC_EmitGetLpRegister(ctx, sctx, reg, fl));
	}

	switch(rcls)
	{
	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_VO_REF:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_GR2:
	case BGBCC_SH_REGCLS_VO_QGR2:
	case BGBCC_SH_REGCLS_AR_REF:
		if(sctx->is_addr64)
			userq=1;
		else
			userq=0;
		break;
	default:
		userq=0;
		break;
	}

#if 0
	if((BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
		BGBCC_CCXL_IsRegPointerP(ctx, reg)) &&
		(sctx->is_addr64))
			{ userq=1; }
		else
			{ userq=0; }
#endif

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	pr0=BGBCC_BSRC_GetVRegPriority(ctx, sctx, reg);
	bi=-1; nsv=0;
	/* Check for registers not holding a live value. */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
	{
		if(excl&(1<<i))
			continue;

		if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

		if(!((sctx->regalc_save)&(1<<i)))
		{
			creg=bgbcc_bsr_cachereg[i];
//			if(sctx->reg_save&(1<<(creg&15)))
			if(sctx->reg_save&(1<<(creg&31)))
				{ bi=i; break; }

			nsv++;
			continue;
		}
//		if(!((sctx->regalc_live)&(1<<i)))
		if(!((sctx->regalc_live)&(1<<i)) &&
			!((sctx->regalc_dirty)&(1<<i)))
		{
			reg1=sctx->regalc_map[i];
			pr1=BGBCC_BSRC_GetVRegPriority(ctx, sctx, reg1);

			if(BGBCC_BSRC_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i; break; }
			else
			{
	//			if((bi<0) || (sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
//				if((bi<0) || (sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
//				if(pr0<=pr1)
				if((bi<0) ||
					((sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]) &&
					(pr0<=pr1)))
				{
//					if((pr1>=0) && (pr0>=0) && (pr0>=pr1))
						bi=i;
				}
			}
		}
	}
	
	if(bi<0)
	{
//		for(i=0; i<bgbcc_bsr_maxreg; i++)
		for(i=0; i<sctx->maxreg_gpr; i++)
		{
			if(excl&(1<<i))
				continue;

			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

			if(!((sctx->regalc_save)&(1<<i)))
				{ continue; }
			if(!((sctx->regalc_live)&(1<<i)))
			{
				reg1=sctx->regalc_map[i];
				if((bi<0) || (sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
					bi=i;
				if(BGBCC_BSRC_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i; break; }
			}
		}
	}

//	if((pr0>=0) && (pr0<2))
//		bi=pr0;

	/* Check if we are better off allocating another register */
	if(bi>=0)
	{
		reg1=sctx->regalc_map[bi];
		if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
			sctx->is_simpass &&
			!BGBCC_BSRC_CheckVRegIsZRegP(ctx, sctx, reg1))
		{
			bi=-1;
		}
	}

//	if((sctx->vsp_rsv>0) && (bi<sctx->vsp_rsv) && (pr0>=sctx->vsp_rsv))
//		bi=-1;

	if((pr0>=0) && (pr0<sctx->vsp_rsv))
		bi=pr0;

#if 1
	if(bi>=0)
	{
		i=bi;

		BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i);
		BGBCC_BSRC_StompLpRegisterIndex(ctx, sctx, i);

		if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
			{ BGBCC_DBGBREAK }

		sctx->regalc_save|=1<<i;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_live|=1<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;
		creg=bgbcc_bsr_cachereg[i];
		if(userq)
			creg=bgbcc_bsr_qcachereg[i];
		else if(sctx->is_addr64)
			creg=bgbcc_bsr_jcachereg[i];
		BGBCC_BSRC_EmitSaveFrameReg(ctx, sctx, creg);
//		if(fl&2)
		if((fl&2) && (i>=sctx->vsp_rsv))
			{ BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		return(creg);
	}
#endif

	/* Check for unallocated registers. */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
	{
		if(excl&(1<<i))
			continue;

		if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

		if((sctx->regalc_save)&(1<<i))
			continue;
		
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_save|=1<<i;
		sctx->regalc_live|=1<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;

		creg=bgbcc_bsr_cachereg[i];		
		if(userq)
			creg=bgbcc_bsr_qcachereg[i];
		else if(sctx->is_addr64)
			creg=bgbcc_bsr_jcachereg[i];
		BGBCC_BSRC_EmitSaveFrameReg(ctx, sctx, creg);
//		if(fl&2)
//		if((fl&2) && (i>=sctx->vsp_rsv))
//			{ BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }

		if(fl&2)
		{
//			printf("load from uninitialized register\n");
		}
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

/* Dirty: Register is being modified. Needs old value and writeback. */
int BGBCC_BSRC_EmitGetRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_BSRC_EmitGetRegister(ctx, sctx, reg, 3));
}

/* Write: Register is being written. Old value is not needed. */
int BGBCC_BSRC_EmitGetRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_BSRC_EmitGetRegister(ctx, sctx, reg, 1));
}

/* Read: Register is being read. No writeback needed. */
int BGBCC_BSRC_EmitGetRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_BSRC_EmitGetRegister(ctx, sctx, reg, 2));
}

/* Dirty: Register is being modified. Needs old value and writeback. */
int BGBCC_BSRC_EmitTryGetRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_BSRC_EmitTryGetRegister(ctx, sctx, reg, 3));
}

/* Write: Register is being written. Old value is not needed. */
int BGBCC_BSRC_EmitTryGetRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_BSRC_EmitTryGetRegister(ctx, sctx, reg, 1));
}

/* Read: Register is being read. No writeback needed. */
int BGBCC_BSRC_EmitTryGetRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_BSRC_EmitTryGetRegister(ctx, sctx, reg, 2));
}

int BGBCC_BSRC_GetVRegPriority(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	BGBCC_CCXL_RegisterInfo *fcn;
	BGBCC_BSR_VarSpan *vsp, *vsp1;
	int i, j, k;

	fcn=ctx->cur_func;

	for(i=0; i<sctx->vspan_num; i++)
	{
		vsp=sctx->vspan[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vsp->reg, reg))
			return(i);
	}
	return(99999);
}

int BGBCC_BSRC_GetVRegLiveRange(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg,
	int *rbeg, int *rend)
{
	BGBCC_CCXL_RegisterInfo *fcn;
	BGBCC_BSR_VarSpan *vsp, *vsp1;
	BGBCC_BSR_VarSpan2 *vspb;
	BGBCC_CCXL_VirtOp *vop;
	BGBCC_CCXL_VirtTr *vtr;
	ccxl_register *targs;
	int bb, be, bfl, trn, btrn, tna, barg;
	int i, j, k;
	
	fcn=ctx->cur_func;

	bb=-1; be=-1; bfl=0; trn=0;
	for(i=0; i<sctx->vspan_num; i++)
	{
		vsp=sctx->vspan[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vsp->reg, reg))
		{
			bb=vsp->bbeg;
			be=vsp->bend;
			if(vsp->tbeg!=vsp->tend)
				bfl|=1;

#if 0
			j=(reg.val>>12)&4093;
			if((j>0) && (j<63))
			{
				vspb=vsp->seq+j;
				if(vspb->cnt>0)
				{
					bb=vspb->bbeg;
					be=vspb->bend;
					if(vspb->tbeg!=vspb->tend)
						bfl|=1;
					else
						bfl&=~1;
				}
			}
#endif
			
			break;
		}
	}

#if 0
	bb=-1; be=-1; bfl=0; trn=0;
	for(i=0; i<fcn->n_vop; i++)
	{
		vtr=fcn->vtr[trn];
		vop=fcn->vop[i];
		while(i>=(vtr->b_ops+vtr->n_ops) && (trn<fcn->n_vtr))
		{
			trn++;
			vtr=fcn->vtr[trn];
		}
		
		barg=-1;
		if(vop->opn==CCXL_VOP_CALL)
		{
			tna=vop->imm.call.na;
			targs=vop->imm.call.args;
			for(j=0; j<tna; j++)
			{
//				if(BGBCC_CCXL_RegisterIdentEqualP(ctx, targs[j], reg))
				if(BGBCC_CCXL_RegisterSequenceEqualP(ctx, targs[j], reg))
					barg=j;
			}
		}
		
//		if(BGBCC_CCXL_RegisterSequenceEqualP(ctx, vop->dst, reg) ||
//			BGBCC_CCXL_RegisterSequenceEqualP(ctx, vop->srca, reg) ||
//			BGBCC_CCXL_RegisterSequenceEqualP(ctx, vop->srcb, reg) ||
//			(barg>=0))
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->dst, reg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->srca, reg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->srcb, reg) ||
			(barg>=0))
		{
			if(bb<0)
			{
				bb=i;
				btrn=trn;
			}
			be=i;
			if(btrn!=trn)
				bfl|=1;
			
//			if(i<=vtr->b_ops)
//				bfl|=2;
//			if(i>=(vtr->b_ops+vtr->n_ops-1))
//				bfl|=2;
		}
	}
#endif
	
	*rbeg=bb;
	*rend=be;
	return(bfl);
}

int BGBCC_BSRC_CheckVRegLiveRange(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	int bb, be, bfl;

//	return(1);

#if 1
//	if(!strcmp(ctx->cur_func->name, "Q_memcpy"))
//	{
//		bfl=-1;
//		return(1);
//	}

	bfl=BGBCC_BSRC_GetVRegLiveRange(ctx, sctx, reg, &bb, &be);
	if(bfl&1)
		return(1);
	if(bfl&2)
		return(1);
	if(bb<0)
		return(1);

	if(sctx->tr_opnum<bb)
		return(0);
	if(sctx->tr_opnum>be)
		return(0);
	return(1);
#endif
}

int BGBCC_BSRC_EmitReleaseRegister(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	ccxl_register reg1;
	int creg, regfl;
	int i;

//	if(!sctx->no_fpu && !sctx->fpu_soft)
//	{
//		if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//			BGBCC_CCXL_IsRegFloat16P(ctx, reg) ||
//			BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//		{
//			return(BGBCC_BSRC_EmitReleaseFpRegister(ctx, sctx, reg));
//		}
//	}

	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) && !sctx->is_addr64)
	{
		return(BGBCC_BSRC_EmitReleaseLpRegister(ctx, sctx, reg));
	}

	/* value in register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
			
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
		{
			sctx->regalc_live|=1<<i;
//			if(sctx->regalc_utcnt[i]>0)
//				sctx->regalc_utcnt[i]--;

			if(sctx->regalc_utcnt[i]<=0)
				__debugbreak();

			sctx->regalc_utcnt[i]--;

			regfl=BGBCC_BSRC_GetFrameVRegFlags(ctx, sctx, reg);

			if(!sctx->regalc_utcnt[i])
			{
//				BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i);

				if((regfl&BGBCC_REGFL_ALIASPTR))
				{
					BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i);
				}

				sctx->regalc_live&=~(1<<i);

//				if((sctx->regalc_dirty)&(1<<i))
//					if((reg1.val&0xFFF)==0xFFF)
//						{ BGBCC_DBGBREAK }
			}

			return(1);
		}
	}

	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_BSRC_EmitSyncRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int rgix)
{
	return(BGBCC_BSRC_EmitSyncRegisterIndex2(ctx, sctx, rgix, 3));
}

int BGBCC_BSRC_EmitSyncRegisterIndex2(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	int rgix, int sfl)
{
	static int rchk=0;
	ccxl_register reg;
	ccxl_type tty;
	int creg, regfl, userq, rcls;
	int i;

	i=rgix;

	if(!((sctx->regalc_save)&(1<<i)))
		return(0);

	reg=sctx->regalc_map[i];
	regfl=BGBCC_BSRC_GetFrameVRegFlags(ctx, sctx, reg);

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_BSRC_TypeGetRegClassP(ctx, tty);

	switch(rcls)
	{
	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_VO_REF:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_GR2:
	case BGBCC_SH_REGCLS_VO_QGR2:
	case BGBCC_SH_REGCLS_AR_REF:
		if(sctx->is_addr64)
			userq=1;
		else
			userq=0;
		break;
	default:
		userq=0;
		break;
	}


	if(((sctx->regalc_dirty)&(1<<i)) && (sfl&1))
	{
		reg=sctx->regalc_map[i];
		creg=bgbcc_bsr_cachereg[i];		

		if(userq)
			creg=bgbcc_bsr_qcachereg[i];
		else if(sctx->is_addr64)
			creg=bgbcc_bsr_jcachereg[i];

		if((reg.val&CCXL_REGID_REGMASK)==CCXL_REGID_REGMASK)
			return(0);

#if 1
//		if((
//			BGBCC_CCXL_IsRegTempP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//			BGBCC_CCXL_IsRegLocalP(ctx, reg)) &&
//			!(regfl&BGBCC_REGFL_ALIASPTR))
//		if(BGBCC_CCXL_IsRegTempP(ctx, reg) &&
//			!(regfl&BGBCC_REGFL_ALIASPTR))
		if(1)
		{
			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				/* Register-Reserved Vars don't need write back */
				sctx->regalc_dirty&=~(1<<i);
				return(0);
			}
		}
#endif

#if 1
		if(BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!(regfl&BGBCC_REGFL_ALIASPTR))
//		if((BGBCC_CCXL_IsRegTempP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//			BGBCC_CCXL_IsRegLocalP(ctx, reg)) &&
//			!(regfl&BGBCC_REGFL_ALIASPTR))
		{
			if(!BGBCC_BSRC_CheckVRegLiveRange(ctx, sctx, reg))
			{
				//discard
				sctx->regalc_dirty&=~(1<<i);
				return(0);
			}
		}
#endif

		if(!sctx->is_addr64)
		{
			if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
				BGBCC_CCXL_IsRegVariantP(ctx, reg))
				creg=bgbcc_bsr_lcachereg[i+1];
		}

		if(!rchk)
		{
			rchk=1;
			BGBCC_BSRC_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
			rchk=0;
		}

		sctx->regalc_dirty&=~(1<<i);
	}

	if(sfl&2)
	{
		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
			(regfl&BGBCC_REGFL_ALIASPTR))
		{
			sctx->regalc_map[i].val=-1;
			sctx->regalc_utcnt[i]=0;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_dirty&=~(1<<i);
			sctx->regalc_live&=~(1<<i);
		}
	}

	return(1);
}

int BGBCC_BSRC_EmitSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx)
{
	int i;

//	BGBCC_BSRC_EmitSyncFpRegisters(ctx, sctx);

	/* value in register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
	{
		BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i);
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_live&=~(1<<i);
	}

	return(0);
}

int BGBCC_BSRC_EmitLabelFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx)
{
	int i;

//	BGBCC_BSRC_EmitLabelFlushFpRegisters(ctx, sctx);

	sctx->maxreg_gpr=bgbcc_bsr_maxreg;
	if(sctx->has_bjx1egpr && sctx->use_egpr)
		sctx->maxreg_gpr=bgbcc_bsr_maxreg_egpr;

//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_bsr_maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;

		sctx->regalc_map[i].val=-1;
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_dirty&=~(1<<i);
		sctx->regalc_live&=~(1<<i);
	}
	
	return(0);
}

int BGBCC_BSRC_EmitScratchSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx)
{
	if(sctx->sreg_live)
		{ BGBCC_DBGBREAK }

	return(0);
}

int BGBCC_BSRC_EmitScratchSyncRegistersSafeMask(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx, int mask)
{
	if(sctx->sreg_live & (~mask))
		{ BGBCC_DBGBREAK }

	return(0);
}
