/*
 * BJX2
 * Rn: Register is full-width, but the value only occupies the low 32 bits.
 *   The value is effectively kept in a sign or zero extended form.
 * RDn: Only the low bits are used, the high bits are undefined.
 * RQn: The full 64 bit register is used.
 */

int BGBCC_JX2C_ScratchCheckRegFree(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
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

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=8)
			return(0);
		if(sctx->sfreg_live&(1<<(reg&15)))
			return(0);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchSafeStompReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=2)
			sctx->is_leaf&=(~4);

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(1<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
	}

	sctx->is_leaf&=(~4);

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
		return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(3<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
	}

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sfreg_live&(1<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
	}

	return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
}

int BGBCC_JX2C_ScratchStompReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=2)
			sctx->is_leaf&=(~4);

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<(reg&15);
		return(1);
	}

	sctx->is_leaf&=(~4);

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
//		if((reg&15)>=12)
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&15)>=12)
		if((reg&15)>=8)
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

#if 0
//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=3<<(reg&15);
//		sctx->sfreg_live|=3<<(2*(reg&7)+0);
//		sctx->sfreg_live|=1<<(2*(reg&7)+1);
		return(1);
	}
#endif

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1<<(reg&15);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchHoldReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)>=2)
			sctx->is_leaf&=(~4);

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<(reg&15);
		return(1);
	}

	sctx->is_leaf&=(~4);

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
		if((reg&8)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
		if((reg&8)>=8)
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

#if 0
//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
//		sctx->sfreg_held|=1<<(2*(reg&7)+0);
//		sctx->sfreg_held|=1<<(2*(reg&7)+1);
		sctx->sfreg_held|=1<<((reg&15)+0);
		sctx->sfreg_held|=1<<((reg&15)+1);
		return(1);
	}
#endif

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1<<(reg&15);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchReleaseReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
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
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1<<(reg&15));
		sctx->sfreg_live&=~(1<<(reg&15));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
		if((reg&15)>=8)
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

#if 0
//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }

		sctx->sfreg_held&=~(3<<(reg&15));
		sctx->sfreg_live&=~(3<<(reg&15));

//		sctx->sfreg_held&=~(3<<(2*(reg&7)+0));
//		sctx->sfreg_live&=~(3<<(2*(reg&7)+0));
		return(1);
	}
#endif

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

int BGBCC_JX2C_ScratchAllocTsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int cls)
{
	int i;

	i=1;
	if(!(sctx->sreg_live&(1<<i)))
	{
		sctx->sreg_live|=(1<<i);
		if(sctx->is_addr64)
			return(BGBCC_SH_REG_RQ0+i);
		return(BGBCC_SH_REG_R0+i);
	}
	
	i=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, cls);
	return(i);
}

int BGBCC_JX2C_ScratchAllocReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int cls)
{
	int i;

	sctx->is_leaf&=(~4);
	
	if((sctx->is_tr_leaf&1) && !sctx->is_simpass)
		{ BGBCC_DBGBREAK }

	if(!cls ||
//		(cls==BGBCC_SH_REGCLS_GR) ||
//		(cls==BGBCC_SH_REGCLS_VO_GR) ||
		(cls==BGBCC_SH_REGCLS_QGR) ||
		(cls==BGBCC_SH_REGCLS_VO_QGR) ||
		(cls==BGBCC_SH_REGCLS_VO_REF) ||
		(cls==BGBCC_SH_REGCLS_AR_REF) ||
		(cls==BGBCC_SH_REGCLS_WGR))
	{
//		for(i=1; i<8; i++)
		for(i=7; i>=2; i--)
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
		(cls==BGBCC_SH_REGCLS_VO_GR) ||
		(cls==BGBCC_SH_REGCLS_WGR))
	{
//		for(i=1; i<8; i++)
		for(i=7; i>=2; i--)
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
//		for(i=0; i<8; i++)
		for(i=7; i>=0; i--)
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

	if(cls==BGBCC_SH_REGCLS_DR)
	{
//		for(i=0; i<8; i++)
		for(i=7; i>=0; i--)
		{
			if(!(sctx->sfreg_live&(1<<i)))
			{
				sctx->sfreg_live|=(1<<i);
				return(BGBCC_SH_REG_DR0+i);
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
		for(i=6; i>=2; i--)
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
		for(i=0; i<8; i+=2)
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

int BGBCC_JX2C_SaveFrameOfsForReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int ofs;
	
	if(!BGBCC_JX2_EmitCheckRegExtGPR(sctx, reg))
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

int BGBCC_JX2C_SaveFrameOfsForFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
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

	case BGBCC_SH_REG_FR11: ofs=-56; break;
	case BGBCC_SH_REG_FR10: ofs=-64; break;
	case BGBCC_SH_REG_FR9: ofs=-72; break;
	case BGBCC_SH_REG_FR8: ofs=-80; break;
	}

	if(sctx->is_addr64)
		ofs*=2;

	return(ofs);
}

int BGBCC_JX2C_EmitSaveFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
#if 1
	int ofs;
	ofs=BGBCC_JX2C_SaveFrameOfsForReg(ctx, sctx, reg);
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
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_JX2C_SaveFrameOfsForFpReg(ctx, sctx, reg);
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
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}
#endif

	return(0);
}

int BGBCC_JX2C_EmitReloadSavedFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int ofs;

#if 0
	ofs=BGBCC_JX2C_SaveFrameOfsForReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(!(sctx->reg_save&(1<<(reg&15))))
		if(!(sctx->reg_save&(1<<(reg&31))))
			return(0);

//		sctx->reg_save|=1<<(reg&15);
		BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

	ofs=BGBCC_JX2C_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(!(sctx->freg_save&(1<<(reg&15))))
		if(!(sctx->freg_save&(1<<(reg&31))))
			return(0);

		BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}
#endif

	return(0);
}

#if 0
// const byte bgbcc_jx2_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
// const byte bgbcc_jx2_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const byte bgbcc_jx2_cachereg[8]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8, BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_jcachereg[8]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8, BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_qcachereg[8]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8, BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_maxreg=7;
#endif

#if 0
// const byte bgbcc_jx2_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
// const byte bgbcc_jx2_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const byte bgbcc_jx2_cachereg[16]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8, 
	BGBCC_SH_REG_R31, BGBCC_SH_REG_R30,
	BGBCC_SH_REG_R29, BGBCC_SH_REG_R28,
	BGBCC_SH_REG_R27, BGBCC_SH_REG_R26,
	BGBCC_SH_REG_R25, BGBCC_SH_REG_R24, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_jcachereg[16]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD31, BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29, BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD27, BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25, BGBCC_SH_REG_RD24, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_qcachereg[16]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ31, BGBCC_SH_REG_RQ30,
	BGBCC_SH_REG_RQ29, BGBCC_SH_REG_RQ28,
	BGBCC_SH_REG_RQ27, BGBCC_SH_REG_RQ26,
	BGBCC_SH_REG_RQ25, BGBCC_SH_REG_RQ24, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_maxreg=7;
const byte bgbcc_jx2_maxreg_egpr=15;
#endif


#if 1
const byte bgbcc_jx2_jcachereg[16]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD7, BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5, BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3, BGBCC_SH_REG_RD2,
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_qcachereg[16]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ7, BGBCC_SH_REG_RQ6,
	BGBCC_SH_REG_RQ5, BGBCC_SH_REG_RQ4,
	BGBCC_SH_REG_RQ3, BGBCC_SH_REG_RQ2,
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_pcachereg[16]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8,
	BGBCC_SH_REG_R7, BGBCC_SH_REG_R6,
	BGBCC_SH_REG_R5, BGBCC_SH_REG_R4,
	BGBCC_SH_REG_R3, BGBCC_SH_REG_R2,
	BGBCC_SH_REG_ZZR };

const byte bgbcc_jx2_jcachereg_egpr[32]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD31, BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29, BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD27, BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25, BGBCC_SH_REG_RD24, 
	BGBCC_SH_REG_RD7,  BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5,  BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3,  BGBCC_SH_REG_RD2,
	BGBCC_SH_REG_RD23, BGBCC_SH_REG_RD22,
	BGBCC_SH_REG_RD21, BGBCC_SH_REG_RD20,
	BGBCC_SH_REG_RD19, BGBCC_SH_REG_RD18,
//	BGBCC_SH_REG_RD17, BGBCC_SH_REG_RD16, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_qcachereg_egpr[32]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ31, BGBCC_SH_REG_RQ30,
	BGBCC_SH_REG_RQ29, BGBCC_SH_REG_RQ28,
	BGBCC_SH_REG_RQ27, BGBCC_SH_REG_RQ26,
	BGBCC_SH_REG_RQ25, BGBCC_SH_REG_RQ24, 
	BGBCC_SH_REG_RQ7,  BGBCC_SH_REG_RQ6,
	BGBCC_SH_REG_RQ5,  BGBCC_SH_REG_RQ4,
	BGBCC_SH_REG_RQ3,  BGBCC_SH_REG_RQ2,
	BGBCC_SH_REG_RQ23, BGBCC_SH_REG_RQ22,
	BGBCC_SH_REG_RQ21, BGBCC_SH_REG_RQ20,
	BGBCC_SH_REG_RQ19, BGBCC_SH_REG_RQ18,
//	BGBCC_SH_REG_RQ17, BGBCC_SH_REG_RQ16, 
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_pcachereg_egpr[32]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8,
	BGBCC_SH_REG_R31, BGBCC_SH_REG_R30,
	BGBCC_SH_REG_R29, BGBCC_SH_REG_R28,
	BGBCC_SH_REG_R27, BGBCC_SH_REG_R26,
	BGBCC_SH_REG_R25, BGBCC_SH_REG_R24, 
	BGBCC_SH_REG_R7,  BGBCC_SH_REG_R6,
	BGBCC_SH_REG_R5,  BGBCC_SH_REG_R4,
	BGBCC_SH_REG_R3,  BGBCC_SH_REG_R2,
	BGBCC_SH_REG_R23, BGBCC_SH_REG_R22,
	BGBCC_SH_REG_R21, BGBCC_SH_REG_R20,
	BGBCC_SH_REG_R19, BGBCC_SH_REG_R18,
//	BGBCC_SH_REG_R17, BGBCC_SH_REG_R16, 
	BGBCC_SH_REG_ZZR };


const byte bgbcc_jx2_jcachereg_egt[24]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD7, BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5, BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3, BGBCC_SH_REG_RD2,
	BGBCC_SH_REG_RD23, BGBCC_SH_REG_RD22,
	BGBCC_SH_REG_RD21, BGBCC_SH_REG_RD20,
	BGBCC_SH_REG_RD19, BGBCC_SH_REG_RD18,
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_qcachereg_egt[24]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ7, BGBCC_SH_REG_RQ6,
	BGBCC_SH_REG_RQ5, BGBCC_SH_REG_RQ4,
	BGBCC_SH_REG_RQ3, BGBCC_SH_REG_RQ2,
	BGBCC_SH_REG_RQ23, BGBCC_SH_REG_RQ22,
	BGBCC_SH_REG_RQ21, BGBCC_SH_REG_RQ20,
	BGBCC_SH_REG_RQ19, BGBCC_SH_REG_RQ18,
	BGBCC_SH_REG_ZZR };
const byte bgbcc_jx2_pcachereg_egt[24]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8,
	BGBCC_SH_REG_R7, BGBCC_SH_REG_R6,
	BGBCC_SH_REG_R5, BGBCC_SH_REG_R4,
	BGBCC_SH_REG_R3, BGBCC_SH_REG_R2,
	BGBCC_SH_REG_R23, BGBCC_SH_REG_R22,
	BGBCC_SH_REG_R21, BGBCC_SH_REG_R20,
	BGBCC_SH_REG_R19, BGBCC_SH_REG_R18,
	BGBCC_SH_REG_ZZR };


const byte bgbcc_jx2_maxreg=7;
const byte bgbcc_jx2_maxreg_egpr=15;
const byte bgbcc_jx2_maxreg_lf=13;
// const byte bgbcc_jx2_maxreg_egpr_lf=29;
const byte bgbcc_jx2_maxreg_egpr_lf=27;
const byte bgbcc_jx2_maxreg_egt=7;
const byte bgbcc_jx2_maxreg_egt_lf=19;

#endif

/* For leaf traces, rotate index to prefer scratch registers.
 */
int BGBCC_JX2C_EmitRotateRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx)
{
	int m1, m2, m3;
	int i1;

	if(!(sctx->is_tr_leaf&1))
		return(idx);

	m1=sctx->maxreg_gpr;
	m2=sctx->maxreg_gpr_lf;
	m3=m2-m1;
	
	if(idx<m3)
		i1=(m1+idx);
	else
		i1=(idx-m3);
	if((i1<0) || (i1>=m2))
		{ BGBCC_DBGBREAK }
	return(i1);
}

/* Try to get Variable as register.
 * Will return a value as a register if possible, but may fail.
 * Returns ZZR if it is unable to do so.
 */
int BGBCC_JX2C_EmitTryGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int maxreg, vrsave;
	int creg, excl, nsv, userq, usepq, rcls;
	int pr0, pr1;
	int i, j, bi;

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//		BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//	{
//		return(BGBCC_JX2C_EmitTryGetFpRegister(ctx, sctx, reg, fl));
//	}

//	if(	BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//		BGBCC_CCXL_IsRegFloat16P(ctx, reg))
//	if(rcls==BGBCC_SH_REGCLS_FR)
	if(	(rcls==BGBCC_SH_REGCLS_FR) ||
		(rcls==BGBCC_SH_REGCLS_DR) )
	{
		return(BGBCC_JX2C_EmitTryGetFpRegister(ctx, sctx, reg, fl));
	}

//	if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))

//	if(rcls==BGBCC_SH_REGCLS_FR2)
//	{
//		return(BGBCC_JX2C_EmitTryGetDpRegister(ctx, sctx, reg, fl));
//	}

//	if((BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
//		BGBCC_CCXL_IsRegVariantP(ctx, reg)) && !(sctx->is_addr64))
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2))
//	if(rcls==BGBCC_SH_REGCLS_GR2)
	if(rcls==BGBCC_SH_REGCLS_GR2)
	{
		return(BGBCC_JX2C_EmitTryGetLpRegister(ctx, sctx, reg, fl));
	}

	usepq=0;

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

	if(BGBCC_CCXL_IsRegPointerP(ctx, reg))
	{
		usepq=1;
	}

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

	maxreg=sctx->maxreg_gpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_gpr_lf;
	vrsave=0x00FF00FC;

#if 1
	/* value already in a register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
	{
//		if(excl&(1<<i))
		if((excl>>i)&1)
			continue;

//		if(!((sctx->regalc_live)&(1<<i)))
//			continue;

//		if(!((sctx->regalc_save)&(1<<i)))
//			continue;

		j=(sctx->regalc_save)>>i;
//		j=(sctx->regalc_save|vrsave)>>i;
		if(!(j&1))
		{
			if(!j)
				break;
			continue;
		}

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
//			creg=bgbcc_jx2_cachereg[i];
//			if(userq)
//				creg=bgbcc_jx2_qcachereg[i];
//			else if(sctx->is_addr64)
//				creg=bgbcc_jx2_jcachereg[i];
			if(userq)
				creg=sctx->qcachereg[i];
			else if(usepq)
				creg=sctx->pcachereg[i];
			else
				creg=sctx->jcachereg[i];
			return(creg);
		}
	}
#endif

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

//	if((fl&3)==1)
	if(((fl&3)==1) || ((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv)))
	{
		bi=-1; nsv=0;
//		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	
		/* Check for registers not holding a live value. */
//		for(i=0; i<5; i++)
//		for(i=0; i<bgbcc_jx2_maxreg; i++)
//		for(i=0; i<sctx->maxreg_gpr; i++)
		for(i=0; i<maxreg; i++)
		{
//			if(excl&(1<<i))
			if((excl>>i)&1)
				continue;

			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

			if(!((sctx->regalc_save)&(1<<i)))
			{
//				creg=bgbcc_jx2_cachereg[i];
				creg=sctx->jcachereg[i];
//				if(sctx->reg_save&(1<<(creg&15)))
//				if(sctx->reg_save&(1<<(creg&31)))
				if((sctx->reg_save|vrsave)&(1<<(creg&31)))
					{ bi=i; break; }

				nsv++;
				continue;
			}

//			if(!((sctx->regalc_live)&(1<<i)))
			if(!((sctx->regalc_live)&(1<<i)) &&
				!((sctx->regalc_dirty)&(1<<i)))
			{
				reg1=sctx->regalc_map[i];
				pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
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
				!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
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

			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i);
			BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=1<<i;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_map[i]=reg;
//			sctx->regalc_utcnt[i]=1;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_live|=1<<i;
			if(fl&1)
				{ sctx->regalc_dirty|=1<<i; }
//			creg=bgbcc_jx2_cachereg[i];
//			if(userq)
//				creg=bgbcc_jx2_qcachereg[i];
//			else if(sctx->is_addr64)
//				creg=bgbcc_jx2_jcachereg[i];
			if(userq)
				creg=sctx->qcachereg[i];
			else if(usepq)
				creg=sctx->pcachereg[i];
			else
				creg=sctx->jcachereg[i];
			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//			if(fl&2)
//				{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
#endif
	}

	return(BGBCC_SH_REG_ZZR);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_JX2C_EmitGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int maxreg, vrsave;
	int creg, lng, excl, bi, nsv, userq, usepq, rcls;
	int pr0, pr1;
	int i, i1;

	creg=BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
//		BGBCC_CCXL_IsRegFloat16P(ctx, reg) ||
//		BGBCC_CCXL_IsRegDoubleP(ctx, reg))

//	if((rcls==BGBCC_SH_REGCLS_FR) ||
//		(rcls==BGBCC_SH_REGCLS_FR2))
	if(	(rcls==BGBCC_SH_REGCLS_FR) ||
		(rcls==BGBCC_SH_REGCLS_DR) )
	{
		return(BGBCC_JX2C_EmitGetFpRegister(ctx, sctx, reg, fl));
	}
	
//	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) && !(sctx->is_addr64))
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2))
	if(rcls==BGBCC_SH_REGCLS_GR2)
	{
		return(BGBCC_JX2C_EmitGetLpRegister(ctx, sctx, reg, fl));
	}

	usepq=0;
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

	if(BGBCC_CCXL_IsRegPointerP(ctx, reg))
	{
		usepq=1;
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

	maxreg=sctx->maxreg_gpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_gpr_lf;
	vrsave=0x00FF00FC;

//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	bi=-1; nsv=0;
	/* Check for registers not holding a live value. */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
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
//			creg=bgbcc_jx2_cachereg[i];
			creg=sctx->jcachereg[i];
//			if(sctx->reg_save&(1<<(creg&15)))
//			if(sctx->reg_save&(1<<(creg&31)))
			if((sctx->reg_save|vrsave)&(1<<(creg&31)))
				{ bi=i; break; }

			nsv++;
			continue;
		}
//		if(!((sctx->regalc_live)&(1<<i)))
		if(!((sctx->regalc_live)&(1<<i)) &&
			!((sctx->regalc_dirty)&(1<<i)))
		{
			reg1=sctx->regalc_map[i];
			pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

			if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
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
//		for(i=0; i<bgbcc_jx2_maxreg; i++)
//		for(i=0; i<sctx->maxreg_gpr; i++)
		for(i=0; i<maxreg; i++)
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
				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
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
			!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
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

		BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i);
		BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

//		if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
		if(BGBCC_CCXL_IsRegZzP(ctx, reg))
			{ BGBCC_DBGBREAK }

		sctx->regalc_save|=1<<i;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_live|=1<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;
//		creg=bgbcc_jx2_cachereg[i];
//		if(userq)
//			creg=bgbcc_jx2_qcachereg[i];
//		else if(sctx->is_addr64)
//			creg=bgbcc_jx2_jcachereg[i];
		if(userq)
			creg=sctx->qcachereg[i];
		else if(usepq)
			creg=sctx->pcachereg[i];
		else
			creg=sctx->jcachereg[i];
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//		if(fl&2)
		if((fl&2) && (i>=sctx->vsp_rsv))
			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		return(creg);
	}
#endif

	/* Check for unallocated registers. */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
	{
		i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i);
	
		if(excl&(1<<i1))
			continue;

		if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

		if((sctx->regalc_save)&(1<<i1))
			continue;
		
		sctx->regalc_ltcnt[i1]=0;
		sctx->regalc_map[i1]=reg;
		sctx->regalc_utcnt[i1]=1;
		sctx->regalc_save|=1<<i1;
		sctx->regalc_live|=1<<i1;
		if(fl&1)
			sctx->regalc_dirty|=1<<i1;

//		creg=bgbcc_jx2_cachereg[i1];		
//		if(userq)
//			creg=bgbcc_jx2_qcachereg[i1];
//		else if(sctx->is_addr64)
//			creg=bgbcc_jx2_jcachereg[i1];
		if(userq)
			creg=sctx->qcachereg[i1];
		else if(usepq)
			creg=sctx->pcachereg[i1];
		else
			creg=sctx->jcachereg[i1];
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//		if(fl&2)
//		if((fl&2) && (i>=sctx->vsp_rsv))
//			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }

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
int BGBCC_JX2C_EmitGetRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitGetRegister(ctx, sctx, reg, 3));
}

/* Write: Register is being written. Old value is not needed. */
int BGBCC_JX2C_EmitGetRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitGetRegister(ctx, sctx, reg, 1));
}

/* Read: Register is being read. No writeback needed. */
int BGBCC_JX2C_EmitGetRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitGetRegister(ctx, sctx, reg, 2));
}

/* Dirty: Register is being modified. Needs old value and writeback. */
int BGBCC_JX2C_EmitTryGetRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, reg, 3));
}

/* Write: Register is being written. Old value is not needed. */
int BGBCC_JX2C_EmitTryGetRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, reg, 1));
}

/* Read: Register is being read. No writeback needed. */
int BGBCC_JX2C_EmitTryGetRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, reg, 2));
}

int BGBCC_JX2C_GetVRegPriority(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
//	BGBCC_CCXL_RegisterInfo *fcn;
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	ccxl_register vreg;
	int v, v1, h;
	int i, j, k;

//	fcn=ctx->cur_func;

//	v=reg.val&CCXL_REGTY_REGMASK;
	v=(reg.val&CCXL_REGID_BASEMASK)^
		((reg.val&CCXL_REGTY_REGMASK)>>56);
	h=v*251;
	h=(h>>8)&63;
	
	i=sctx->vspan_hash[h];
	while(i>=0)
	{
		vsp=sctx->vspan[i];
		vreg=vsp->reg;
//		v1=vreg.val&CCXL_REGTY_REGMASK;
		v1=(vreg.val&CCXL_REGID_BASEMASK)^
			((vreg.val&CCXL_REGTY_REGMASK)>>56);

//		if((vreg.val&CCXL_REGTY_REGMASK)!=(reg.val&CCXL_REGTY_REGMASK))
		if(v1!=v)
			{ i=sctx->vspan_chn[i]; continue; }
		
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vreg, reg))
			return(i);
		i=sctx->vspan_chn[i];
	}

	return(99999);

#if 0
	for(i=0; i<sctx->vspan_num; i++)
	{
		vsp=sctx->vspan[i];
		vreg=vsp->reg;

		if((vreg.val&CCXL_REGTY_REGMASK)!=(reg.val&CCXL_REGTY_REGMASK))
			continue;
		
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vreg, reg))
			return(i);
	}
	return(99999);
#endif
}

int BGBCC_JX2C_GetVRegLiveRange(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg,
	int *rbeg, int *rend)
{
	BGBCC_CCXL_RegisterInfo *fcn;
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	BGBCC_JX2_VarSpan2 *vspb;
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

int BGBCC_JX2C_CheckVRegLiveRange(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
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

	bfl=BGBCC_JX2C_GetVRegLiveRange(ctx, sctx, reg, &bb, &be);
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

int BGBCC_JX2C_EmitReleaseRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	ccxl_register reg1;
	int creg, regfl;
	int i;

	if(!sctx->no_fpu && !sctx->fpu_soft)
	{
		if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
			BGBCC_CCXL_IsRegFloat16P(ctx, reg) ||
			BGBCC_CCXL_IsRegBFloat16P(ctx, reg) ||
			BGBCC_CCXL_IsRegDoubleP(ctx, reg))
		{
			return(BGBCC_JX2C_EmitReleaseFpRegister(ctx, sctx, reg));
		}
	}

	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) && !sctx->is_addr64)
	{
		return(BGBCC_JX2C_EmitReleaseLpRegister(ctx, sctx, reg));
	}

	/* value in register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
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

			regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

			if(!sctx->regalc_utcnt[i])
			{
//				BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i);

				if((regfl&BGBCC_REGFL_ALIASPTR))
				{
					BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i);
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

int BGBCC_JX2C_EmitSyncRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int rgix)
{
	return(BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, rgix, 3));
}

int BGBCC_JX2C_EmitSyncRegisterIndex2(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int rgix, int sfl)
{
	static int rchk=0;
	ccxl_register reg;
	ccxl_type tty;
	int creg, regfl, userq, usepq, rcls;
	int i;

	i=rgix;

	if(!((sctx->regalc_save)&(1<<i)))
		return(0);

	reg=sctx->regalc_map[i];
	regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	usepq=0;
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

//		creg=bgbcc_jx2_cachereg[i];
//		if(userq)
//			creg=bgbcc_jx2_qcachereg[i];
//		else if(sctx->is_addr64)
//			creg=bgbcc_jx2_jcachereg[i];
		if(userq)
			creg=sctx->qcachereg[i];
		else if(usepq)
			creg=sctx->pcachereg[i];
		else
			creg=sctx->jcachereg[i];

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
			if(!BGBCC_JX2C_CheckVRegLiveRange(ctx, sctx, reg))
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
				creg=bgbcc_jx2_lcachereg[i+1];
		}

		if(!rchk)
		{
			rchk=1;
			BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
			rchk=0;
		}

		sctx->regalc_dirty&=~(1<<i);
	}

	if(sfl&2)
	{
		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
			BGBCC_CCXL_IsRegThisIdxP(ctx, reg) ||
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

int BGBCC_JX2C_EmitSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	int i;

	BGBCC_JX2C_EmitSyncFpRegisters(ctx, sctx);

	/* value in register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i);
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_live&=~(1<<i);
	}

	return(0);
}

int BGBCC_JX2C_EmitLabelFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	int i;

	BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);

	sctx->jcachereg=bgbcc_jx2_jcachereg;
	sctx->qcachereg=bgbcc_jx2_qcachereg;
	sctx->pcachereg=bgbcc_jx2_pcachereg;
	sctx->maxreg_gpr=bgbcc_jx2_maxreg;
	sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_lf;

	if(sctx->has_bjx1egpr)
	{
		sctx->jcachereg=bgbcc_jx2_jcachereg_egt;
		sctx->qcachereg=bgbcc_jx2_qcachereg_egt;
		sctx->pcachereg=bgbcc_jx2_pcachereg_egt;
		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egt;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_egt_lf;
	}

	if(sctx->has_bjx1egpr && sctx->use_egpr)
	{
		sctx->jcachereg=bgbcc_jx2_jcachereg_egpr;
		sctx->qcachereg=bgbcc_jx2_qcachereg_egpr;
		sctx->pcachereg=bgbcc_jx2_pcachereg_egpr;
		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;
	}

//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
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

int BGBCC_JX2C_EmitScratchSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->sreg_live)
		{ BGBCC_DBGBREAK }

	return(0);
}

int BGBCC_JX2C_EmitScratchSyncRegistersSafeMask(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int mask)
{
	if(sctx->sreg_live & (~mask))
		{ BGBCC_DBGBREAK }

	return(0);
}
