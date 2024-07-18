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

/*
 * BJX2
 * Rn: Register is full-width, but the value only occupies the low 32 bits.
 *   The value is effectively kept in a sign or zero extended form.
 * RDn: Only the low bits are used, the high bits are undefined.
 * RQn: The full 64 bit register is used.
 */

static int bgbcc_rvi_remapbasetab[32] = {
	 5,  1,  6,  7,  10, 11, 12, 13, 
	20, 21, 22, 23,  18, 19,  8,  2,
	28, 29, 30, 31,  14, 15, 16, 17,
	 0,  0,  0,  9,  24, 25, 26, 27
};

static int bgbcc_rvi_remap_jx2rv[2048];
static int bgbcc_rvi_remap_rv2jx[2048];
static int bgbcc_rvi_remap_init=0;

int BGBCC_JX2C_InitRemaps(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	int i, j, k;

	if(bgbcc_rvi_remap_init)
		return(0);
	bgbcc_rvi_remap_init=1;
	
	for(i=0; i<2048; i++)
	{
		bgbcc_rvi_remap_jx2rv[i]=0;
		bgbcc_rvi_remap_rv2jx[i]=0;
	}
	return(0);
}



int BGBCC_JX2C_RemapRegJx2Rv(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int reg1;
	
	reg1=reg;
	return(reg1);
}

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
		if(sctx->sreg_live&(1ULL<<(reg&15)))
			return(0);
		return(1);
	}

#if 0
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			return(0);
		if(sctx->sreg_live&(3ULL<<(reg&15)))
			return(0);
		return(1);
	}
#endif

#if 1
	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16) )
	{
		if((reg&15)>=8)
			return(0);
		if(reg&1)
		{
			if(sctx->sreg_live&(3ULL<<((reg&30)+32)))
				return(0);
		}else
		{
			if(sctx->sreg_live&(3ULL<<(reg&30)))
				return(0);
		}
		return(1);
	}
#endif

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=8)
			return(0);
		if(sctx->sfreg_live&(1ULL<<(reg&15)))
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
		{
#if 0
//			if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
			if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
					!(sctx->is_leaftiny&8))
				printf("Reject LeafTiny RegStomp %s\n",
					ctx->cur_func->qname);
#endif

			sctx->is_leaf&=(~4);
			sctx->is_leaftiny|=8;

			if(ctx->cur_vtr)
			{
//				ctx->cur_vtr->trfl&=~CCXL_TRFL_LEAF;
//				ctx->cur_vtr->trfl|=CCXL_TRFL_USES_SCRATCH;
			}

			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
			ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;
		}

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sreg_live&(1ULL<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
	}

#if 0
//	if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
	if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
			!(sctx->is_leaftiny&8))
		printf("Reject LeafTiny RegStomp2 %s\n",
			ctx->cur_func->qname);
#endif

	sctx->is_leaf&=(~4);
	sctx->is_leaftiny|=8;

	if(ctx->cur_vtr)
	{
//		ctx->cur_vtr->trfl&=~CCXL_TRFL_LEAF;
//		ctx->cur_vtr->trfl|=CCXL_TRFL_USES_SCRATCH;
	}

	ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
//		if(sctx->sreg_live&(1ULL<<(reg&31)))
		if(sctx->sreg_live&(1ULL<<(reg&63)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if(!(reg&1))
		{
			if((reg&15)>=7)
				{ BGBCC_DBGBREAK }
			if(sctx->sreg_live&(3ULL<<(reg&15)))
				{ BGBCC_DBGBREAK }
			return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
		}
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
	{
		if(!(reg&1))
		{
			if((reg&15)>=7)
				{ BGBCC_DBGBREAK }
			if(sctx->sreg_live&(3ULL<<(reg&30)))
	//		if(sctx->sreg_live&(3ULL<<(reg&63)))
				{ BGBCC_DBGBREAK }
			return(BGBCC_JX2C_ScratchStompReg(ctx, sctx, reg));
		}
	}

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sfreg_live&(1ULL<<(reg&15)))
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
		{
#if 0
//			if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
			if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
					!(sctx->is_leaftiny&8))
				printf("Reject LeafTiny RegStomp3 %s\n",
					ctx->cur_func->qname);
#endif

			sctx->is_leaf&=(~4);
			sctx->is_leaftiny|=8;

			if(ctx->cur_vtr)
			{
//				ctx->cur_vtr->trfl&=~CCXL_TRFL_LEAF;
//				ctx->cur_vtr->trfl|=CCXL_TRFL_USES_SCRATCH;
			}

			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
			ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;
		}

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1ULL<<(reg&15);
		return(1);
	}

#if 0
//	if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
	if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
			!(sctx->is_leaftiny&8))
		printf("Reject LeafTiny RegStomp4 %s\n",
			ctx->cur_func->qname);
#endif

	sctx->is_leaf&=(~4);
	sctx->is_leaftiny|=8;
	ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }
	
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1ULL<<(reg&63);

		sctx->is_leaf&=(~4);
		sctx->is_leaftiny|=8;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

		return(1);
	}

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R32 ) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48 ) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48) )
	{
		if(!sctx->has_xgpr)
			{ BGBCC_DBGBREAK }
	
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1ULL<<(reg&63);

		sctx->is_leaf&=(~4);
		sctx->is_leaftiny|=8;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
//		if((reg&15)>=12)
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1ULL<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&15)>=12)
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1ULL<<(reg&15);
		return(1);
	}

#if 0
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=3ULL<<((reg&15)+0);
//		sctx->sreg_live|=1ULL<<((reg&15)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
	{
		if((reg&15)>=7)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=3ULL<<((reg&31)+0);
		return(1);
	}
#endif

#if 1
	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16) )
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }

		if(reg&1)
		{
			sctx->sreg_live|=3ULL<<((reg&30)+32);
		}else
		{
			sctx->sreg_live|=3ULL<<((reg&30)+0);
		}
		return(1);
	}
#endif

#if 0
//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=3ULL<<(reg&15);
//		sctx->sfreg_live|=3ULL<<(2*(reg&7)+0);
//		sctx->sfreg_live|=1ULL<<(2*(reg&7)+1);
		return(1);
	}
#endif

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_live|=1ULL<<(reg&15);
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
		{
#if 0
//			if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
			if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
					!(sctx->is_leaftiny&8))
				printf("Reject LeafTiny RegHold %s\n",
					ctx->cur_func->qname);
#endif

			sctx->is_leaf&=(~4);
			sctx->is_leaftiny|=8;
			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
			ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;
		}

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1ULL<<(reg&15);
		return(1);
	}

#if 0
//	if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
	if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
			!(sctx->is_leaftiny&8))
		printf("Reject LeafTiny RegHold2 %s\n",
			ctx->cur_func->qname);
#endif

	sctx->is_leaf&=(~4);
	sctx->is_leaftiny|=8;
	ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16))
	{
		if(!sctx->has_bjx1egpr)
			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1ULL<<(reg&63);

		sctx->is_leaf&=(~4);
		sctx->is_leaftiny|=8;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

		return(1);
	}

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R32 ) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48 ) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48))
	{
		if(!sctx->has_xgpr)
			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1ULL<<(reg&63);

		sctx->is_leaf&=(~4);
		sctx->is_leaftiny|=8;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;

		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&8)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1ULL<<(reg&15);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
		if((reg&8)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1ULL<<(reg&15);
		return(1);
	}

#if 0
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1ULL<<((reg&15)+0);
		sctx->sreg_held|=1ULL<<((reg&15)+1);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1ULL<<((reg&31)+0);
		sctx->sreg_held|=1ULL<<((reg&31)+1);
		return(1);
	}
#endif

#if 1
	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16) )
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }

		if(reg&1)
		{
			sctx->sreg_held|=3ULL<<((reg&30)+32);
		}else
		{
			sctx->sreg_held|=3ULL<<((reg&30)+0);
//			sctx->sreg_held|=1ULL<<((reg&31)+0);
//			sctx->sreg_held|=1ULL<<((reg&31)+1);
		}
		return(1);
	}
#endif

#if 0
//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_LR0)
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
//		if((reg&7)>=6)
//			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
//		sctx->sfreg_held|=1ULL<<(2*(reg&7)+0);
//		sctx->sfreg_held|=1ULL<<(2*(reg&7)+1);
		sctx->sfreg_held|=1ULL<<((reg&15)+0);
		sctx->sfreg_held|=1ULL<<((reg&15)+1);
		return(1);
	}
#endif

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held|=1ULL<<(reg&15);
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
		sctx->sreg_held&=~(1ULL<<(reg&15));
		sctx->sreg_live&=~(1ULL<<(reg&15));
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
		sctx->sreg_held&=~(1ULL<<(reg&63));
		sctx->sreg_live&=~(1ULL<<(reg&63));
		return(1);
	}

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R32 ) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ32) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R48 ) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD48) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ48))
	{
		if(!sctx->has_xgpr)
			{ BGBCC_DBGBREAK }
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1ULL<<(reg&63));
		sctx->sreg_live&=~(1ULL<<(reg&63));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1ULL<<(reg&15));
		sctx->sfreg_live&=~(1ULL<<(reg&15));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1ULL<<(reg&15));
		sctx->sfreg_live&=~(1ULL<<(reg&15));
		return(1);
	}

#if 0
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(3ULL<<((reg&15)+0));
//		sctx->sreg_held&=~(1ULL<<((reg&15)+1));
		sctx->sreg_live&=~(3ULL<<((reg&15)+0));
//		sctx->sreg_live&=~(1ULL<<((reg&15)+1));
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(3ULL<<((reg&30)+0));
		sctx->sreg_live&=~(3ULL<<((reg&30)+0));
		return(1);
	}
#endif

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16) )
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }

		if(reg&1)
		{
			sctx->sreg_held&=~(3ULL<<((reg&30)+32));
			sctx->sreg_live&=~(3ULL<<((reg&30)+32));
		}else
		{
			sctx->sreg_held&=~(3ULL<<((reg&30)+0));
			sctx->sreg_live&=~(3ULL<<((reg&30)+0));
		}
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

		sctx->sfreg_held&=~(3ULL<<(reg&15));
		sctx->sfreg_live&=~(3ULL<<(reg&15));

//		sctx->sfreg_held&=~(3ULL<<(2*(reg&7)+0));
//		sctx->sfreg_live&=~(3ULL<<(2*(reg&7)+0));
		return(1);
	}
#endif

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sreg_held&=~(1ULL<<(reg&15));
		sctx->sreg_live&=~(1ULL<<(reg&15));
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchAllocTsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int cls)
{
	static const byte tslist[4]={1, 16, 17};
	int i, j;

	for(j=0; j<3; j++)
	{
//		i=1;
		i=tslist[j];
		if(!(sctx->sreg_live&(1ULL<<i)))
		{
			sctx->sreg_live|=(1ULL<<i);
			if(sctx->is_addr64)
				return(BGBCC_SH_REG_RQ0+i);
			return(BGBCC_SH_REG_R0+i);
		}
	}	
	i=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, cls);
	return(i);
}

int BGBCC_JX2C_ScratchAllocReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int cls)
{
	int i, j;

#if 0
//	if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
	if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
			!(sctx->is_leaftiny&8))
		printf("Reject LeafTiny ScratchAlloc %s\n",
			ctx->cur_func->qname);
#endif

	sctx->is_leaf&=(~4);
	sctx->is_leaftiny|=8;
//	ctx->cur_vtr->trfl&=~1;
	ctx->cur_vtr->trfl&=~CCXL_TRFL_LEAF;

	ctx->cur_vtr->trfl|=CCXL_TRFL_USES_SCRATCH;

	ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	ctx->cur_func->regflags|=BGBCC_REGFL_NOSCRATCHDYN;
	
	if((sctx->is_tr_leaf&1) && !sctx->is_simpass)
		{ BGBCC_DBGBREAK }

	if(sctx->is_simpass && ((sctx->is_simpass&63)!=1) &&
		!(sctx->is_simpass&64))
	{
//		if(sctx->is_tr_leaf&1)
//			{ BGBCC_DBGBREAK }
	}

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
//		for(i=7; i>=2; i--)
		for(j=0; j<6; j++)
		{
//			i=7-((j+sctx->rov_sreg)%6);
			i=7-j;
			if(!(sctx->sreg_live&(1ULL<<i)))
			{
				sctx->rov_sreg+=j+1;
				sctx->sreg_live|=(1ULL<<i);
				if(sctx->is_addr64)
					return(BGBCC_SH_REG_RQ0+i);
				return(BGBCC_SH_REG_R0+i);
			}
		}

		if(sctx->has_bjx1egpr)
		{
//			for(i=23; i>=16; i--)
			for(j=0; j<8; j++)
			{
//				i=23-((j+sctx->rov_sreg)&7);
				i=23-j;
				if(!(sctx->sreg_live&(1ULL<<i)))
				{
					sctx->rov_sreg+=j+1;
					sctx->sreg_live|=(1ULL<<i);
					if(sctx->is_addr64)
						return(BGBCC_SH_REG_RQ0+i);
					return(BGBCC_SH_REG_R0+i);
				}
			}
		}

		if(sctx->has_xgpr)
		{
//			for(i=23; i>=16; i--)
			for(j=0; j<8; j++)
			{
				i=39-j;
				if(!(sctx->sreg_live&(1ULL<<i)))
				{
					sctx->rov_sreg+=j+1;
					sctx->sreg_live|=(1ULL<<i);
					if(sctx->is_addr64)
						return(BGBCC_SH_REG_RQ0+i);
					return(BGBCC_SH_REG_R0+i);
				}
			}

//			for(i=23; i>=16; i--)
			for(j=0; j<8; j++)
			{
				i=55-j;
				if(!(sctx->sreg_live&(1ULL<<i)))
				{
					sctx->rov_sreg+=j+1;
					sctx->sreg_live|=(1ULL<<i);
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
			if(!(sctx->sreg_live&(1ULL<<i)))
			{
				sctx->sreg_live|=(1ULL<<i);
				if(sctx->is_addr64)
					return(BGBCC_SH_REG_RD0+i);
				return(BGBCC_SH_REG_R0+i);
			}
		}
		if(sctx->has_bjx1egpr)
		{
			for(i=23; i>=16; i--)
			{
				if(!(sctx->sreg_live&(1ULL<<i)))
				{
					sctx->sreg_live|=(1ULL<<i);
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
			if(!(sctx->sfreg_live&(1ULL<<i)))
			{
				sctx->sfreg_live|=(1ULL<<i);
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
			if(!(sctx->sfreg_live&(1ULL<<i)))
			{
				sctx->sfreg_live|=(1ULL<<i);
				return(BGBCC_SH_REG_DR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	if((cls==BGBCC_SH_REGCLS_GR2) ||
//		(cls==BGBCC_SH_REGCLS_QGR2) ||
		(cls==BGBCC_SH_REGCLS_VO_GR2) ||
		(cls==BGBCC_SH_REGCLS_VO_QGR2))
	{
//		for(i=1; i<7; i++)
		for(i=6; i>=2; i-=2)
		{
			if(!(sctx->sreg_live&(3ULL<<i)))
			{
				sctx->sreg_live|=(3ULL<<i);
				return(BGBCC_SH_REG_LR0+i);
			}
		}

		if(sctx->has_bjx1egpr)
		{
			for(i=22; i>=16; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
				{
					sctx->sreg_live|=(3ULL<<i);
					return(BGBCC_SH_REG_LR0+i);
				}
			}
		}

		if(sctx->has_xgpr)
		{
			for(i=38; i>=34; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
				{
					sctx->sreg_live|=(3ULL<<i);
					return(BGBCC_SH_REG_LR0+((i-32)+1));
				}
			}

			for(i=54; i>=48; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
				{
					sctx->sreg_live|=(3ULL<<i);
					return(BGBCC_SH_REG_LR0+((i-32)+1));
				}
			}
		}

		BGBCC_DBGBREAK
		return(-1);
	}

	if(cls==BGBCC_SH_REGCLS_QGR2)
	{
		for(i=6; i>=2; i-=2)
		{
			if(!(sctx->sreg_live&(3ULL<<i)))
			{
				sctx->sreg_live|=(3ULL<<i);
				return(BGBCC_SH_REG_LR0+i);
			}
		}

		if(sctx->has_bjx1egpr)
		{
			for(i=22; i>=16; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
				{
					sctx->sreg_live|=(3ULL<<i);
					return(BGBCC_SH_REG_LR0+i);
				}
			}
		}

		if(sctx->has_xgpr)
		{
			for(i=38; i>=34; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
				{
					sctx->sreg_live|=(3ULL<<i);
					return(BGBCC_SH_REG_LR0+((i-32)+1));
				}
			}

			for(i=54; i>=48; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
				{
					sctx->sreg_live|=(3ULL<<i);
					return(BGBCC_SH_REG_LR0+((i-32)+1));
				}
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
			if(!(sctx->sfreg_live&(3ULL<<i)))
			{
				sctx->sfreg_live|=(3ULL<<i);
				return(BGBCC_SH_REG_DR0+i);
			}
		}
		BGBCC_DBGBREAK
		return(-1);
	}

	BGBCC_DBGBREAK
	return(-1);
}

int BGBCC_JX2C_ScratchQueryReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int cls)
{
	int i, n;

//	sctx->is_leaf&=(~4);
//	ctx->cur_vtr->trfl&=~1;
	
	if(!cls ||
		(cls==BGBCC_SH_REGCLS_QGR) ||
		(cls==BGBCC_SH_REGCLS_VO_QGR) ||
		(cls==BGBCC_SH_REGCLS_VO_REF) ||
		(cls==BGBCC_SH_REGCLS_AR_REF) ||
		(cls==BGBCC_SH_REGCLS_WGR))
	{
		n=0;
		for(i=7; i>=2; i--)
		{
			if(!(sctx->sreg_live&(1ULL<<i)))
				{ n++; }
		}

		if(sctx->has_bjx1egpr)
		{
			for(i=23; i>=16; i--)
			{
				if(!(sctx->sreg_live&(1ULL<<i)))
					{ n++; }
			}
		}
		return(n);
	}

	if(!cls ||
		(cls==BGBCC_SH_REGCLS_GR) ||
		(cls==BGBCC_SH_REGCLS_VO_GR) ||
		(cls==BGBCC_SH_REGCLS_WGR))
	{
		n=0;
		for(i=7; i>=2; i--)
		{
			if(!(sctx->sreg_live&(1ULL<<i)))
				{ n++; }
		}
		if(sctx->has_bjx1egpr)
		{
			for(i=23; i>=16; i--)
			{
				if(!(sctx->sreg_live&(1ULL<<i)))
					{ n++; }
			}
		}
		return(n);
	}

	if((cls==BGBCC_SH_REGCLS_GR2) ||
		(cls==BGBCC_SH_REGCLS_QGR2) ||
		(cls==BGBCC_SH_REGCLS_VO_GR2) ||
		(cls==BGBCC_SH_REGCLS_VO_QGR2))
	{
		n=0;
		
		for(i=6; i>=2; i-=2)
		{
			if(!(sctx->sreg_live&(3ULL<<i)))
				{ n++; }
		}

		if(sctx->has_bjx1egpr)
		{
			for(i=22; i>=16; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
					{ n++; }
			}
		}

		if(sctx->has_xgpr)
		{
			for(i=38; i>=34; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
					{ n++; }
			}

			for(i=54; i>=48; i-=2)
			{
				if(!(sctx->sreg_live&(3ULL<<i)))
					{ n++; }
			}
		}

		return(n);
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
//	switch(reg&31)
	switch(reg&63)
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

	case BGBCC_SH_REG_R47: ofs=-68; break;
	case BGBCC_SH_REG_R46: ofs=-72; break;
	case BGBCC_SH_REG_R45: ofs=-76; break;
	case BGBCC_SH_REG_R44: ofs=-80; break;
	case BGBCC_SH_REG_R43: ofs=-84; break;
	case BGBCC_SH_REG_R42: ofs=-88; break;
	case BGBCC_SH_REG_R41: ofs=-92; break;
	case BGBCC_SH_REG_R40: ofs=-96; break;

	case BGBCC_SH_REG_R63: ofs=-100; break;
	case BGBCC_SH_REG_R62: ofs=-104; break;
	case BGBCC_SH_REG_R61: ofs=-108; break;
	case BGBCC_SH_REG_R60: ofs=-112; break;
	case BGBCC_SH_REG_R59: ofs=-116; break;
	case BGBCC_SH_REG_R58: ofs=-120; break;
	case BGBCC_SH_REG_R57: ofs=-124; break;
	case BGBCC_SH_REG_R56: ofs=-128; break;

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

	case BGBCC_SH_REG_RQ47: ofs=-68; break;
	case BGBCC_SH_REG_RQ46: ofs=-72; break;
	case BGBCC_SH_REG_RQ45: ofs=-76; break;
	case BGBCC_SH_REG_RQ44: ofs=-80; break;
	case BGBCC_SH_REG_RQ43: ofs=-84; break;
	case BGBCC_SH_REG_RQ42: ofs=-88; break;
	case BGBCC_SH_REG_RQ41: ofs=-92; break;
	case BGBCC_SH_REG_RQ40: ofs=-96; break;

	case BGBCC_SH_REG_RQ63: ofs=-100; break;
	case BGBCC_SH_REG_RQ62: ofs=-104; break;
	case BGBCC_SH_REG_RQ61: ofs=-108; break;
	case BGBCC_SH_REG_RQ60: ofs=-112; break;
	case BGBCC_SH_REG_RQ59: ofs=-116; break;
	case BGBCC_SH_REG_RQ58: ofs=-120; break;
	case BGBCC_SH_REG_RQ57: ofs=-124; break;
	case BGBCC_SH_REG_RQ56: ofs=-128; break;

	case BGBCC_SH_REG_RD47: ofs=-68; break;
	case BGBCC_SH_REG_RD46: ofs=-72; break;
	case BGBCC_SH_REG_RD45: ofs=-76; break;
	case BGBCC_SH_REG_RD44: ofs=-80; break;
	case BGBCC_SH_REG_RD43: ofs=-84; break;
	case BGBCC_SH_REG_RD42: ofs=-88; break;
	case BGBCC_SH_REG_RD41: ofs=-92; break;
	case BGBCC_SH_REG_RD40: ofs=-96; break;

	case BGBCC_SH_REG_RD63: ofs=-100; break;
	case BGBCC_SH_REG_RD62: ofs=-104; break;
	case BGBCC_SH_REG_RD61: ofs=-108; break;
	case BGBCC_SH_REG_RD60: ofs=-112; break;
	case BGBCC_SH_REG_RD59: ofs=-116; break;
	case BGBCC_SH_REG_RD58: ofs=-120; break;
	case BGBCC_SH_REG_RD57: ofs=-124; break;
	case BGBCC_SH_REG_RD56: ofs=-128; break;
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

u64 BGBCC_JX2C_GetFrameVrsaveMask(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	u64 vrsave;
	
	if(sctx->emit_riscv&0x33)
	{
		vrsave=0xF003FCFFF003FCE0ULL;
		return(vrsave);
	}
	
	vrsave=0x00FF00FF00FF00FCULL;
	return(vrsave);
}

u64 BGBCC_JX2C_GetFrameArgRegsMask(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	u64 vrsave;
	
	if(sctx->emit_riscv&0x33)
	{
		vrsave=0x0003FC000003FC00ULL;
		return(vrsave);
	}
	
	vrsave=0x00F000F000F000F0ULL;
	return(vrsave);
}

u64 BGBCC_JX2C_GetFrameArgRegsMaskB(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	u64 vrsave;
	
	if(sctx->emit_riscv&0x33)
	{
		vrsave=0x0003FC000003FC00ULL;
		return(vrsave);
	}
	
	vrsave=0x00F000F000FC00F8ULL;
	return(vrsave);
}

u64 BGBCC_JX2C_GetFrameArgFpRegsMask(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	u64 vrsave;
	
	if(sctx->emit_riscv&0x33)
	{
		vrsave=0x0003FC000003FC00ULL;
		return(vrsave);
	}
	
	vrsave=0x00F0;
	return(vrsave);
}

int BGBCC_JX2C_GetBaseRegForArgumentIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	if(sctx->emit_riscv&0x33)
	{
		if((idx>=0) && (idx<8))
		{
			return(BGBCC_SH_REG_R10+idx);
		}
	
		return(-1);
	}
	
	if((idx>=0) && (idx<4))
	{
		return(BGBCC_SH_REG_R4+idx);
	}

	if((idx>=4) && (idx<8))
	{
		return(BGBCC_SH_REG_R20+(idx-4));
	}
	
	if(sctx->has_xgpr&2)
	{
		if((idx>=8) && (idx<12))
		{
			return(BGBCC_SH_REG_R36+(idx-8));
		}

		if((idx>=12) && (idx<16))
		{
			return(BGBCC_SH_REG_R52+(idx-12));
		}
	}
	
	return(-1);
}

int BGBCC_JX2C_GetBaseRegForArgTempIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	return(BGBCC_SH_REG_R16+(idx&1));
}

int BGBCC_JX2C_GetRdRegForArgumentIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	int reg;
	
	reg=BGBCC_JX2C_GetBaseRegForArgumentIndex(ctx, sctx, idx);
	if(reg<0)
		return(reg);
	reg=BGBCC_SH_REG_RD0+(reg&63);
	return(reg);
}

int BGBCC_JX2C_GetRqRegForArgumentIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	int reg;
	
	reg=BGBCC_JX2C_GetBaseRegForArgumentIndex(ctx, sctx, idx);
	if(reg<0)
		return(reg);
	reg=BGBCC_SH_REG_RQ0+(reg&63);
	return(reg);
}

int BGBCC_JX2C_GetLrRegForArgumentIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	int reg;
	
	if(idx&1)
		{ BGBCC_DBGBREAK }
	
	reg=BGBCC_JX2C_GetBaseRegForArgumentIndex(ctx, sctx, idx);
	if(reg<0)
		return(reg);

	if(reg&1)
		{ BGBCC_DBGBREAK }

	if(reg&0x20)
		reg=BGBCC_SH_REG_LR0+(reg&0x1E)+1;
	else
		reg=BGBCC_SH_REG_LR0+(reg&0x1E);

	return(reg);
}

int BGBCC_JX2C_GetRdRegForArgTempIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	int reg;
	
	reg=BGBCC_JX2C_GetBaseRegForArgTempIndex(ctx, sctx, idx);
	if(reg<0)
		return(reg);
	reg=BGBCC_SH_REG_RD0+(reg&63);
	return(reg);
}

int BGBCC_JX2C_GetRqRegForArgTempIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int idx)
{
	int reg;
	
	reg=BGBCC_JX2C_GetBaseRegForArgTempIndex(ctx, sctx, idx);
	if(reg<0)
		return(reg);
	reg=BGBCC_SH_REG_RQ0+(reg&63);
	return(reg);
}

int BGBCC_JX2C_GetRbRegReturn(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->emit_riscv&0x33)
		return(BGBCC_SH_REG_R10);

	return(BGBCC_SH_REG_R2);
}

int BGBCC_JX2C_GetRdRegReturn(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->emit_riscv&0x33)
		return(BGBCC_SH_REG_RD10);

	return(BGBCC_SH_REG_RD2);
}

int BGBCC_JX2C_GetRqRegReturn(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->emit_riscv&0x33)
		return(BGBCC_SH_REG_RQ10);

	return(BGBCC_SH_REG_RQ2);
}

int BGBCC_JX2C_GetLrRegReturn(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->emit_riscv&0x33)
		return(BGBCC_SH_REG_LR10);

	return(BGBCC_SH_REG_LR2);
}

int BGBCC_JX2C_GetRqRegThis(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->emit_riscv&0x33)
		return(BGBCC_SH_REG_RQ10);

	return(BGBCC_SH_REG_RQ3);
}

int BGBCC_JX2C_GetLrRegThis(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->emit_riscv&0x33)
		return(BGBCC_SH_REG_LR10);

	return(BGBCC_SH_REG_LR18);
}

int BGBCC_JX2C_CheckFrameNeedSaveReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	u64 vrsave;
	int rb;

	if(!BGBCC_JX2_EmitCheckRegExtGPR(sctx, reg))
		return(0);

	vrsave=BGBCC_JX2C_GetFrameVrsaveMask(ctx, sctx);
	
	rb=reg&63;
	if(!((vrsave>>rb)&1))
		return(1);
	return(0);

#if 0
	rb=reg&63;
	if((rb>=8) && (rb<15))
		return(1);
	if((rb>=24) && (rb<=31))
		return(1);
	if((rb>=24) && (rb<=31))
		return(1);
	if((rb>=40) && (rb<=47))
		return(1);
	if((rb>=56) && (rb<=63))
		return(1);
	return(0);
#endif

#if 0
	int ofs;
	ofs=BGBCC_JX2C_SaveFrameOfsForReg(ctx, sctx, reg);

	if(ofs!=0)
		return(1);
	return(0);
#endif
}

int BGBCC_JX2C_CheckFrameNeedSaveFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	return(0);
}

int BGBCC_JX2C_CheckFrameSavedReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int ofs, rb;

	if(BGBCC_JX2C_CheckFrameNeedSaveReg(ctx, sctx, reg))
	{
		if(sctx->reg_save&(1ULL<<(reg&63)))
			return(1);
		return(0);
	}
	return(1);

#if 0
	ofs=BGBCC_JX2C_SaveFrameOfsForReg(ctx, sctx, reg);

	if(ofs!=0)
	{
//		if(sctx->reg_save&(1ULL<<(reg&31)))
		if(sctx->reg_save&(1ULL<<(reg&63)))
			return(1);
		return(0);
	}
	return(1);
#endif
}

int BGBCC_JX2C_CheckFrameSavedLpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int tr0, tr1;

	if((reg&BGBCC_SH_REG_RTMASK5)!=BGBCC_SH_REG_LR0)
	{
		BGBCC_DBGBREAK
		return(0);
	}
	
	tr0=BGBCC_SH_REG_R0+(reg&0x1E);
	tr1=tr0+1;

	if(reg&1)
	{
		tr0+=32;
		tr1+=32;
	}

//	if(!BGBCC_JX2C_CheckFrameSavedReg(ctx, sctx, (reg&31)+0))
	if(!BGBCC_JX2C_CheckFrameSavedReg(ctx, sctx, tr0))
		return(0);
//	if(!BGBCC_JX2C_CheckFrameSavedReg(ctx, sctx, (reg&31)+1))
	if(!BGBCC_JX2C_CheckFrameSavedReg(ctx, sctx, tr1))
		return(0);
	return(1);
}

int BGBCC_JX2C_CheckFrameSavedFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	return(0);
}

int BGBCC_JX2C_EmitSaveFrameReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int tr0, tr1;

	if(!reg || (reg==BGBCC_SH_REG_ZZR))
		{ BGBCC_DBGBREAK }

	if((reg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_LR0)
	{
		tr0=BGBCC_SH_REG_R0+(reg&0x1E);
		tr1=tr0+1;

		if(reg&1)
		{
			tr0+=32;
			tr1+=32;
		}
		
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, tr0);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, tr1);
		return(0);
	}
	
//	if(sctx->has_xgpr&2)
	if(sctx->has_xgpr)
	{
		if(	((reg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0) ||
			((reg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0) )
		{
			tr0=BGBCC_SH_REG_R0+(reg&0x3E);
			tr1=tr0+1;

			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, tr0);
			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, tr1);
			return(0);
		}
	}

#if 1
	int ofs;
//	ofs=BGBCC_JX2C_SaveFrameOfsForReg(ctx, sctx, reg);
//	if(ofs!=0)
//	if(BGBCC_JX2C_CheckFrameSavedReg(ctx, sctx, reg))
	if(BGBCC_JX2C_CheckFrameNeedSaveReg(ctx, sctx, reg))
	{
//		if(sctx->reg_save&(1ULL<<(reg&15)))
//		if(sctx->reg_save&(1ULL<<(reg&31)))
		if(sctx->reg_save&(1ULL<<(reg&63)))
			return(0);

//		if(!sctx->is_simpass && !(sctx->reg_vsave&(1ULL<<(reg&15))))
//		if(!sctx->is_simpass && !(sctx->reg_vsave&(1ULL<<(reg&31))))
		if(!sctx->is_simpass && !(sctx->reg_vsave&(1ULL<<(reg&63))))
			{ BGBCC_DBGBREAK }

//		sctx->reg_save|=1ULL<<(reg&15);
//		sctx->reg_save|=1ULL<<(reg&31);
		sctx->reg_save|=1ULL<<(reg&63);
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, ofs, reg);
		return(1);
	}

//	ofs=BGBCC_JX2C_SaveFrameOfsForFpReg(ctx, sctx, reg);
//	if(ofs!=0)
//	if(BGBCC_JX2C_CheckFrameSavedFpReg(ctx, sctx, reg))
	if(BGBCC_JX2C_CheckFrameNeedSaveFpReg(ctx, sctx, reg))
	{
//		if(sctx->freg_save&(1ULL<<(reg&15)))
		if(sctx->freg_save&(1ULL<<(reg&31)))
			return(0);

//		if(!sctx->is_simpass && !(sctx->freg_vsave&(1ULL<<(reg&15))))
		if(!sctx->is_simpass && !(sctx->freg_vsave&(1ULL<<(reg&31))))
			{ BGBCC_DBGBREAK }

//		sctx->freg_save|=1ULL<<(reg&15);
		sctx->freg_save|=1ULL<<(reg&31);
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
//		if(!(sctx->reg_save&(1ULL<<(reg&15))))
		if(!(sctx->reg_save&(1ULL<<(reg&63))))
			return(0);

//		sctx->reg_save|=1ULL<<(reg&15);
		BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, ofs, reg);
		return(1);
	}

	ofs=BGBCC_JX2C_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
//		if(!(sctx->freg_save&(1ULL<<(reg&15))))
		if(!(sctx->freg_save&(1ULL<<(reg&31))))
			return(0);

		BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, ofs, reg);
		return(1);
	}
#endif

	return(0);
}

#if 0
// const short bgbcc_jx2_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
// const short bgbcc_jx2_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const short bgbcc_jx2_cachereg[8]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8, BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_jcachereg[8]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8, BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_qcachereg[8]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8, BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_maxreg=7;
#endif

#if 0
// const short bgbcc_jx2_cachereg[6]={ 13, 11, 10, 9, 8, 255 };
// const short bgbcc_jx2_cachereg[8]={ 14, 13, 12, 11, 10, 9, 8, 255 };
const short bgbcc_jx2_cachereg[16]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8, 
	BGBCC_SH_REG_R31, BGBCC_SH_REG_R30,
	BGBCC_SH_REG_R29, BGBCC_SH_REG_R28,
	BGBCC_SH_REG_R27, BGBCC_SH_REG_R26,
	BGBCC_SH_REG_R25, BGBCC_SH_REG_R24, 
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_jcachereg[16]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD31, BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29, BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD27, BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25, BGBCC_SH_REG_RD24, 
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_qcachereg[16]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ31, BGBCC_SH_REG_RQ30,
	BGBCC_SH_REG_RQ29, BGBCC_SH_REG_RQ28,
	BGBCC_SH_REG_RQ27, BGBCC_SH_REG_RQ26,
	BGBCC_SH_REG_RQ25, BGBCC_SH_REG_RQ24, 
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_maxreg=7;
const short bgbcc_jx2_maxreg_egpr=15;
#endif


#if 1
const short bgbcc_jx2_jcachereg[16]={
	BGBCC_SH_REG_RD14, BGBCC_SH_REG_RD13,
	BGBCC_SH_REG_RD12, BGBCC_SH_REG_RD11,
	BGBCC_SH_REG_RD10, BGBCC_SH_REG_RD9,
	BGBCC_SH_REG_RD8,
	BGBCC_SH_REG_RD7, BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5, BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3, BGBCC_SH_REG_RD2,
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_qcachereg[16]={
	BGBCC_SH_REG_RQ14, BGBCC_SH_REG_RQ13,
	BGBCC_SH_REG_RQ12, BGBCC_SH_REG_RQ11,
	BGBCC_SH_REG_RQ10, BGBCC_SH_REG_RQ9,
	BGBCC_SH_REG_RQ8,
	BGBCC_SH_REG_RQ7, BGBCC_SH_REG_RQ6,
	BGBCC_SH_REG_RQ5, BGBCC_SH_REG_RQ4,
	BGBCC_SH_REG_RQ3, BGBCC_SH_REG_RQ2,
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_pcachereg[16]={
	BGBCC_SH_REG_R14, BGBCC_SH_REG_R13,
	BGBCC_SH_REG_R12, BGBCC_SH_REG_R11,
	BGBCC_SH_REG_R10, BGBCC_SH_REG_R9,
	BGBCC_SH_REG_R8,
	BGBCC_SH_REG_R7, BGBCC_SH_REG_R6,
	BGBCC_SH_REG_R5, BGBCC_SH_REG_R4,
	BGBCC_SH_REG_R3, BGBCC_SH_REG_R2,
	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_lcachereg[32]={
	BGBCC_SH_REG_LR14, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR12, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR10, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR8, BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_LR6,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR4,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR2,	BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_ZZR
	};


const short bgbcc_jx2_jcachereg_egpr[32]={
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
const short bgbcc_jx2_qcachereg_egpr[32]={
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
const short bgbcc_jx2_pcachereg_egpr[32]={
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

const short bgbcc_jx2_lcachereg_egpr[32]={
	BGBCC_SH_REG_LR14, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR12, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR10, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR8, BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_LR30,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR28,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR26,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR24,	BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_LR6,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR4,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR2,	BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_LR22,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR20,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR18,	BGBCC_SH_REG_ZZR,
	
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_ZZR
	};

const short bgbcc_jx2_shufreg0_egpr[32]={
	 0,  1,	 2,  3,
	 4,  5,	 6,
	
	 7,  8,	 9, 10,
	11, 12,	13, 14,

	15, 16,	17, 18,
	19, 20,

	21, 22,	23, 24,
	25, 26,
	27, 28,
};

const short bgbcc_jx2_shufreg1_egpr[32]={
	 2,  3,	 0,  1,
	 6,  5,	 4,
	
	 9, 10,	 7,  8,
	13, 14,	11, 12,

	17, 18,	15, 16,
	19, 20,

	23, 24,	21, 22,
	25, 26,
	27, 28,
};

const short bgbcc_jx2_shufreg2_egpr[32]={
	 6,  5,	 0,  1,	
	 2,  3,	
	 4,
	
	11, 12,	 7,  8,
	 9, 10,	13, 14,

	19, 20,
	15, 16,
	17, 18,	

	25, 26,	21, 22,
	23, 24,

	27, 28,
};

const short bgbcc_jx2_shufreg3_egpr[32]={
	 2,  3,	 6,  5,
	 0,  1,	 4,
	
	11, 12,	13, 14,
	 7,  8,	 9, 10,	

	17, 18,	19, 20,
	15, 16,

	23, 24,	25, 26,
	21, 22,

	27, 28,
};

#if 0
const short bgbcc_jx2_jcachereg_egt[24]={
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
const short bgbcc_jx2_qcachereg_egt[24]={
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
const short bgbcc_jx2_pcachereg_egt[24]={
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

const short bgbcc_jx2_lcachereg_egt[32]={
	BGBCC_SH_REG_LR14, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR12, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR10, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR8, BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_LR6,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR4,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR2,	BGBCC_SH_REG_ZZR,

	BGBCC_SH_REG_LR22,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR20,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_LR18,	BGBCC_SH_REG_ZZR,
	
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_ZZR
	};
#endif

const short bgbcc_jx2_maxreg=7;
const short bgbcc_jx2_maxreg_lf=13;

const short bgbcc_jx2_maxreg_egpr=15;
// const short bgbcc_jx2_maxreg_egpr_lf=29;
const short bgbcc_jx2_maxreg_egpr_lf=27;
const short bgbcc_jx2_maxreg_egt=7;
const short bgbcc_jx2_maxreg_egt_lf=19;

byte bgbcc_jx2_lmaxreg=15;
byte bgbcc_jx2_lmaxreg_lf=27;

#endif

#if 0
const short bgbcc_jx2_jcachereg_ltiny[32]={
	BGBCC_SH_REG_RD7,  BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5,  BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3,  BGBCC_SH_REG_RD2,
	BGBCC_SH_REG_RD23, BGBCC_SH_REG_RD22,
	BGBCC_SH_REG_RD21, BGBCC_SH_REG_RD20,
	BGBCC_SH_REG_RD19, BGBCC_SH_REG_RD18,
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_qcachereg_ltiny[32]={
	BGBCC_SH_REG_RQ7,  BGBCC_SH_REG_RQ6,
	BGBCC_SH_REG_RQ5,  BGBCC_SH_REG_RQ4,
	BGBCC_SH_REG_RQ3,  BGBCC_SH_REG_RQ2,
	BGBCC_SH_REG_RQ23, BGBCC_SH_REG_RQ22,
	BGBCC_SH_REG_RQ21, BGBCC_SH_REG_RQ20,
	BGBCC_SH_REG_RQ19, BGBCC_SH_REG_RQ18,
	BGBCC_SH_REG_ZZR };
const short bgbcc_jx2_pcachereg_ltiny[32]={
	BGBCC_SH_REG_R7,  BGBCC_SH_REG_R6,
	BGBCC_SH_REG_R5,  BGBCC_SH_REG_R4,
	BGBCC_SH_REG_R3,  BGBCC_SH_REG_R2,
	BGBCC_SH_REG_R23, BGBCC_SH_REG_R22,
	BGBCC_SH_REG_R21, BGBCC_SH_REG_R20,
	BGBCC_SH_REG_R19, BGBCC_SH_REG_R18,
	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_maxreg_ltiny=12;
#endif



#if 0
const short bgbcc_jx2_jcachereg_xgpr[64]={
	BGBCC_SH_REG_RD14,
	BGBCC_SH_REG_RD13,	BGBCC_SH_REG_RD12,
	BGBCC_SH_REG_RD11,	BGBCC_SH_REG_RD10,
	BGBCC_SH_REG_RD9,	BGBCC_SH_REG_RD8,

	BGBCC_SH_REG_RD31,	BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29,	BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD27,	BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25,	BGBCC_SH_REG_RD24, 

	BGBCC_SH_REG_RD7,	BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5,	BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3,	BGBCC_SH_REG_RD2,

	BGBCC_SH_REG_RD23,	BGBCC_SH_REG_RD22,
	BGBCC_SH_REG_RD21,	BGBCC_SH_REG_RD20,
	BGBCC_SH_REG_RD19,	BGBCC_SH_REG_RD18,

	BGBCC_SH_REG_RD39, BGBCC_SH_REG_RD38,
	BGBCC_SH_REG_RD37, BGBCC_SH_REG_RD36,
	BGBCC_SH_REG_RD35, BGBCC_SH_REG_RD34,
	BGBCC_SH_REG_RD33, BGBCC_SH_REG_RD32,

	BGBCC_SH_REG_RD55, BGBCC_SH_REG_RD54,
	BGBCC_SH_REG_RD53, BGBCC_SH_REG_RD52,
	BGBCC_SH_REG_RD51, BGBCC_SH_REG_RD50,
	BGBCC_SH_REG_RD49, BGBCC_SH_REG_RD48,

	BGBCC_SH_REG_RD47, BGBCC_SH_REG_RD46,
	BGBCC_SH_REG_RD45, BGBCC_SH_REG_RD44,
	BGBCC_SH_REG_RD43, BGBCC_SH_REG_RD42,
	BGBCC_SH_REG_RD41, BGBCC_SH_REG_RD40,

	BGBCC_SH_REG_RD63, BGBCC_SH_REG_RD62,
	BGBCC_SH_REG_RD61, BGBCC_SH_REG_RD60,
	BGBCC_SH_REG_RD59, BGBCC_SH_REG_RD58,
	BGBCC_SH_REG_RD57, BGBCC_SH_REG_RD56,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_qcachereg_xgpr[64]={
	BGBCC_SH_REG_RQ14,
	BGBCC_SH_REG_RQ13,	BGBCC_SH_REG_RQ12,
	BGBCC_SH_REG_RQ11,	BGBCC_SH_REG_RQ10,
	BGBCC_SH_REG_RQ9,	BGBCC_SH_REG_RQ8,

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

	BGBCC_SH_REG_RQ39, BGBCC_SH_REG_RQ38,
	BGBCC_SH_REG_RQ37, BGBCC_SH_REG_RQ36,
	BGBCC_SH_REG_RQ35, BGBCC_SH_REG_RQ34,
	BGBCC_SH_REG_RQ33, BGBCC_SH_REG_RQ32,

	BGBCC_SH_REG_RQ55, BGBCC_SH_REG_RQ54,
	BGBCC_SH_REG_RQ53, BGBCC_SH_REG_RQ52,
	BGBCC_SH_REG_RQ51, BGBCC_SH_REG_RQ50,
	BGBCC_SH_REG_RQ49, BGBCC_SH_REG_RQ48,

	BGBCC_SH_REG_RQ47, BGBCC_SH_REG_RQ46,
	BGBCC_SH_REG_RQ45, BGBCC_SH_REG_RQ44,
	BGBCC_SH_REG_RQ43, BGBCC_SH_REG_RQ42,
	BGBCC_SH_REG_RQ41, BGBCC_SH_REG_RQ40,

	BGBCC_SH_REG_RQ63, BGBCC_SH_REG_RQ62,
	BGBCC_SH_REG_RQ61, BGBCC_SH_REG_RQ60,
	BGBCC_SH_REG_RQ59, BGBCC_SH_REG_RQ58,
	BGBCC_SH_REG_RQ57, BGBCC_SH_REG_RQ56,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_pcachereg_xgpr[64]={
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

	BGBCC_SH_REG_R39, BGBCC_SH_REG_R38,
	BGBCC_SH_REG_R37, BGBCC_SH_REG_R36,
	BGBCC_SH_REG_R35, BGBCC_SH_REG_R34,
	BGBCC_SH_REG_R33, BGBCC_SH_REG_R32,

	BGBCC_SH_REG_R55, BGBCC_SH_REG_R54,
	BGBCC_SH_REG_R53, BGBCC_SH_REG_R52,
	BGBCC_SH_REG_R51, BGBCC_SH_REG_R50,
	BGBCC_SH_REG_R49, BGBCC_SH_REG_R48,

	BGBCC_SH_REG_R47, BGBCC_SH_REG_R46,
	BGBCC_SH_REG_R45, BGBCC_SH_REG_R44,
	BGBCC_SH_REG_R43, BGBCC_SH_REG_R42,
	BGBCC_SH_REG_R41, BGBCC_SH_REG_R40,

	BGBCC_SH_REG_R63, BGBCC_SH_REG_R62,
	BGBCC_SH_REG_R61, BGBCC_SH_REG_R60,
	BGBCC_SH_REG_R59, BGBCC_SH_REG_R58,
	BGBCC_SH_REG_R57, BGBCC_SH_REG_R56,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_lcachereg_xgpr[64]={
	BGBCC_SH_REG_LR14,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR12,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR10,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR8,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR30,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR28,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR26,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR24,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR6,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR4,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR2,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR22,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR20,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR18,

	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR38,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR36,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR34,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR32,

	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR54,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR52,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR50,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR48,

	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR46,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR44,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR42,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR40,

	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR62,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR60,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR58,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR56,

	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_ZZR
	};

const short bgbcc_jx2_shufreg0_xgpr[64]={
	 0,  1,	 2,  3,
	 4,  5,	 6,
	
	 7,  8,	 9, 10,
	11, 12,	13, 14,

	15, 16,	17, 18,
	19, 20,

	21, 22,	23, 24,
	25, 26,
	27, 28,

	29, 30, 31, 32,
	33, 34, 35, 36,
	37, 38, 39, 40,
	41, 42, 43, 44,
	45, 46, 47, 48,
	49, 50, 51, 52,
	53, 54, 55, 56,
	57, 58, 59, 60,
	61, 62, 63
};

const short bgbcc_jx2_shufreg1_xgpr[64]={
	 2,  3,	 0,  1,
	 6,  5,	 4,
	
	 9, 10,	 7,  8,
	13, 14,	11, 12,

	17, 18,	15, 16,
	19, 20,

	23, 24,	21, 22,
	25, 26,
	27, 28,

	29, 30, 31, 32,
	33, 34, 35, 36,
	37, 38, 39, 40,
	41, 42, 43, 44,
	45, 46, 47, 48,
	49, 50, 51, 52,
	53, 54, 55, 56,
	57, 58, 59, 60,
	61, 62, 63
};

const short bgbcc_jx2_shufreg2_xgpr[64]={
	 6,  5,	 0,  1,	
	 2,  3,	
	 4,
	
	11, 12,	 7,  8,
	 9, 10,	13, 14,

	19, 20,
	15, 16,
	17, 18,	

	25, 26,	21, 22,
	23, 24,

	27, 28,

	29, 30, 31, 32,
	33, 34, 35, 36,
	37, 38, 39, 40,
	41, 42, 43, 44,
	45, 46, 47, 48,
	49, 50, 51, 52,
	53, 54, 55, 56,
	57, 58, 59, 60,
	61, 62, 63
};

const short bgbcc_jx2_shufreg3_xgpr[64]={
	 2,  3,	 6,  5,
	 0,  1,	 4,
	
	11, 12,	13, 14,
	 7,  8,	 9, 10,	

	17, 18,	19, 20,
	15, 16,

	23, 24,	25, 26,
	21, 22,

	27, 28,

	29, 30, 31, 32,
	33, 34, 35, 36,
	37, 38, 39, 40,
	41, 42, 43, 44,
	45, 46, 47, 48,
	49, 50, 51, 52,
	53, 54, 55, 56,
	57, 58, 59, 60,
	61, 62, 63
};

// const short bgbcc_jx2_maxreg_xgpr=31;
// const short bgbcc_jx2_maxreg_xgpr=59;
// const short bgbcc_jx2_maxreg_xgpr_lf=59;

const short bgbcc_jx2_maxreg_xgpr=15;
// const short bgbcc_jx2_maxreg_xgpr_lf=27;
const short bgbcc_jx2_maxreg_xgpr_lf=43;

#endif

#if 1
const short bgbcc_jx2_jcachereg_xgpr[64]={
	BGBCC_SH_REG_RD14,
	BGBCC_SH_REG_RD13, BGBCC_SH_REG_RD12,
	BGBCC_SH_REG_RD11, BGBCC_SH_REG_RD10,
	BGBCC_SH_REG_RD9 , BGBCC_SH_REG_RD8 ,

	BGBCC_SH_REG_RD31, BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29, BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD27, BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25, BGBCC_SH_REG_RD24,

	BGBCC_SH_REG_RD47, BGBCC_SH_REG_RD46,
	BGBCC_SH_REG_RD45, BGBCC_SH_REG_RD44,
	BGBCC_SH_REG_RD43, BGBCC_SH_REG_RD42,
	BGBCC_SH_REG_RD41, BGBCC_SH_REG_RD40,

	BGBCC_SH_REG_RD63, BGBCC_SH_REG_RD62,
	BGBCC_SH_REG_RD61, BGBCC_SH_REG_RD60,
	BGBCC_SH_REG_RD59, BGBCC_SH_REG_RD58,
	BGBCC_SH_REG_RD57, BGBCC_SH_REG_RD56,

	BGBCC_SH_REG_RD7,  BGBCC_SH_REG_RD6,
	BGBCC_SH_REG_RD5,  BGBCC_SH_REG_RD4,
	BGBCC_SH_REG_RD3,  BGBCC_SH_REG_RD2,

	BGBCC_SH_REG_RD23, BGBCC_SH_REG_RD22,
	BGBCC_SH_REG_RD21, BGBCC_SH_REG_RD20,
	BGBCC_SH_REG_RD19, BGBCC_SH_REG_RD18,

	BGBCC_SH_REG_RD39, BGBCC_SH_REG_RD38,
	BGBCC_SH_REG_RD37, BGBCC_SH_REG_RD36,
	BGBCC_SH_REG_RD35, BGBCC_SH_REG_RD34,
	BGBCC_SH_REG_RD33, BGBCC_SH_REG_RD32,

	BGBCC_SH_REG_RD55, BGBCC_SH_REG_RD54,
	BGBCC_SH_REG_RD53, BGBCC_SH_REG_RD52,
	BGBCC_SH_REG_RD51, BGBCC_SH_REG_RD50,
	BGBCC_SH_REG_RD49, BGBCC_SH_REG_RD48,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_qcachereg_xgpr[64]={
	BGBCC_SH_REG_RQ14,
	BGBCC_SH_REG_RQ13, BGBCC_SH_REG_RQ12,
	BGBCC_SH_REG_RQ11, BGBCC_SH_REG_RQ10,
	BGBCC_SH_REG_RQ9,  BGBCC_SH_REG_RQ8,

	BGBCC_SH_REG_RQ31, BGBCC_SH_REG_RQ30,
	BGBCC_SH_REG_RQ29, BGBCC_SH_REG_RQ28,
	BGBCC_SH_REG_RQ27, BGBCC_SH_REG_RQ26,
	BGBCC_SH_REG_RQ25, BGBCC_SH_REG_RQ24, 

	BGBCC_SH_REG_RQ47, BGBCC_SH_REG_RQ46,
	BGBCC_SH_REG_RQ45, BGBCC_SH_REG_RQ44,
	BGBCC_SH_REG_RQ43, BGBCC_SH_REG_RQ42,
	BGBCC_SH_REG_RQ41, BGBCC_SH_REG_RQ40,
	
	BGBCC_SH_REG_RQ63, BGBCC_SH_REG_RQ62,
	BGBCC_SH_REG_RQ61, BGBCC_SH_REG_RQ60,
	BGBCC_SH_REG_RQ59, BGBCC_SH_REG_RQ58,
	BGBCC_SH_REG_RQ57, BGBCC_SH_REG_RQ56,

	BGBCC_SH_REG_RQ7,  BGBCC_SH_REG_RQ6,
	BGBCC_SH_REG_RQ5,  BGBCC_SH_REG_RQ4,
	BGBCC_SH_REG_RQ3,  BGBCC_SH_REG_RQ2,

	BGBCC_SH_REG_RQ23, BGBCC_SH_REG_RQ22,
	BGBCC_SH_REG_RQ21, BGBCC_SH_REG_RQ20,
	BGBCC_SH_REG_RQ19, BGBCC_SH_REG_RQ18,

	BGBCC_SH_REG_RQ39, BGBCC_SH_REG_RQ38,
	BGBCC_SH_REG_RQ37, BGBCC_SH_REG_RQ36,
	BGBCC_SH_REG_RQ35, BGBCC_SH_REG_RQ34,
	BGBCC_SH_REG_RQ33, BGBCC_SH_REG_RQ32,

	BGBCC_SH_REG_RQ55, BGBCC_SH_REG_RQ54,
	BGBCC_SH_REG_RQ53, BGBCC_SH_REG_RQ52,
	BGBCC_SH_REG_RQ51, BGBCC_SH_REG_RQ50,
	BGBCC_SH_REG_RQ49, BGBCC_SH_REG_RQ48,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_pcachereg_xgpr[64]={
	BGBCC_SH_REG_R14,
	BGBCC_SH_REG_R13, BGBCC_SH_REG_R12,
	BGBCC_SH_REG_R11, BGBCC_SH_REG_R10,
	BGBCC_SH_REG_R9,  BGBCC_SH_REG_R8,

	BGBCC_SH_REG_R31, BGBCC_SH_REG_R30,
	BGBCC_SH_REG_R29, BGBCC_SH_REG_R28,
	BGBCC_SH_REG_R27, BGBCC_SH_REG_R26,
	BGBCC_SH_REG_R25, BGBCC_SH_REG_R24, 

	BGBCC_SH_REG_R47, BGBCC_SH_REG_R46,
	BGBCC_SH_REG_R45, BGBCC_SH_REG_R44,
	BGBCC_SH_REG_R43, BGBCC_SH_REG_R42,
	BGBCC_SH_REG_R41, BGBCC_SH_REG_R40,
	
	BGBCC_SH_REG_R63, BGBCC_SH_REG_R62,
	BGBCC_SH_REG_R61, BGBCC_SH_REG_R60,
	BGBCC_SH_REG_R59, BGBCC_SH_REG_R58,
	BGBCC_SH_REG_R57, BGBCC_SH_REG_R56,

	BGBCC_SH_REG_R7,  BGBCC_SH_REG_R6,
	BGBCC_SH_REG_R5,  BGBCC_SH_REG_R4,
	BGBCC_SH_REG_R3,  BGBCC_SH_REG_R2,

	BGBCC_SH_REG_R23, BGBCC_SH_REG_R22,
	BGBCC_SH_REG_R21, BGBCC_SH_REG_R20,
	BGBCC_SH_REG_R19, BGBCC_SH_REG_R18,

	BGBCC_SH_REG_R39, BGBCC_SH_REG_R38,
	BGBCC_SH_REG_R37, BGBCC_SH_REG_R36,
	BGBCC_SH_REG_R35, BGBCC_SH_REG_R34,
	BGBCC_SH_REG_R33, BGBCC_SH_REG_R32,

	BGBCC_SH_REG_R55, BGBCC_SH_REG_R54,
	BGBCC_SH_REG_R53, BGBCC_SH_REG_R52,
	BGBCC_SH_REG_R51, BGBCC_SH_REG_R50,
	BGBCC_SH_REG_R49, BGBCC_SH_REG_R48,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_lcachereg_xgpr[64]={
	BGBCC_SH_REG_LR14,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR12,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR10,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR8,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR30,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR28,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR26,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR24,
	
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR46,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR44,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR42,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR40,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR62,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR60,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR58,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR56,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR6,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR4,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR2,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR22,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR20,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR18,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR38,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR36,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR34,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR32,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR54,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR52,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR50,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR48,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_ZZR
	};

const short bgbcc_jx2_shufreg0_xgpr[64]={
	 0,  1,	 2,  3,
	 4,  5,	 6,
	
	 7,  8,	 9, 10,
	11, 12,	13, 14,

	15, 16,	17, 18,
	19, 20,

	21, 22,	23, 24,
	25, 26,	27, 28,
	
	29, 31, 32, 33,
	34, 35, 36, 37,
	38, 39, 40, 41,
	42, 43, 44, 45,

	46, 47, 48, 49,
	50, 51, 52, 53,
	54, 55, 56, 57,
	58, 59, 60, 62,
};

const short bgbcc_jx2_shufreg1_xgpr[64]={
	 2,  3,	 0,  1,
	 6,  5,	 4,
	
	 9, 10,	 7,  8,
	13, 14,	11, 12,

	17, 18,	15, 16,
	19, 20,

	23, 24,	21, 22,
	25, 26,	27, 28,
	
	29, 31, 32, 33,
	34, 35, 36, 37,
	38, 39, 40, 41,
	42, 43, 44, 45,

	46, 47, 48, 49,
	50, 51, 52, 53,
	54, 55, 56, 57,
	58, 59, 60, 62,
};

const short bgbcc_jx2_shufreg2_xgpr[64]={
	 6,  5,	 0,  1,	
	 2,  3,	
	 4,
	
	11, 12,	 7,  8,
	 9, 10,	13, 14,

	19, 20,
	15, 16,
	17, 18,	

	25, 26,	21, 22,
	23, 24,

	27, 28,
	
	29, 31, 32, 33,
	34, 35, 36, 37,
	38, 39, 40, 41,
	42, 43, 44, 45,

	46, 47, 48, 49,
	50, 51, 52, 53,
	54, 55, 56, 57,
	58, 59, 60, 62,
};

const short bgbcc_jx2_shufreg3_xgpr[64]={
	 2,  3,	 6,  5,
	 0,  1,	 4,
	
	11, 12,	13, 14,
	 7,  8,	 9, 10,	

	17, 18,	19, 20,
	15, 16,

	23, 24,	25, 26,
	21, 22,

	27, 28,
	
	29, 31, 32, 33,
	34, 35, 36, 37,
	38, 39, 40, 41,
	42, 43, 44, 45,

	46, 47, 48, 49,
	50, 51, 52, 53,
	54, 55, 56, 57,
	58, 59, 60, 62,
};

const short bgbcc_jx2_maxreg_xgpr=31;
const short bgbcc_jx2_maxreg_xgpr_lf=59;

#endif



#if 1
const short bgbcc_jx2_jcachereg_xg2rv[64]={
	BGBCC_SH_REG_RD26,
	BGBCC_SH_REG_RD25, BGBCC_SH_REG_RD24,
	BGBCC_SH_REG_RD23, BGBCC_SH_REG_RD22,
	BGBCC_SH_REG_RD21, BGBCC_SH_REG_RD20,

	BGBCC_SH_REG_RD19, BGBCC_SH_REG_RD18,
	BGBCC_SH_REG_RD9, BGBCC_SH_REG_RD8,

	BGBCC_SH_REG_RD59, BGBCC_SH_REG_RD58,
	BGBCC_SH_REG_RD57, BGBCC_SH_REG_RD56,

	BGBCC_SH_REG_RD55, BGBCC_SH_REG_RD54,
	BGBCC_SH_REG_RD53, BGBCC_SH_REG_RD52,
	BGBCC_SH_REG_RD51, BGBCC_SH_REG_RD50,
	BGBCC_SH_REG_RD41, BGBCC_SH_REG_RD40,


	BGBCC_SH_REG_RD31, BGBCC_SH_REG_RD30,
	BGBCC_SH_REG_RD29, BGBCC_SH_REG_RD28,
	BGBCC_SH_REG_RD17, BGBCC_SH_REG_RD16,
	BGBCC_SH_REG_RD15, BGBCC_SH_REG_RD14,

	BGBCC_SH_REG_RD13,  BGBCC_SH_REG_RD12,
	BGBCC_SH_REG_RD11,  BGBCC_SH_REG_RD10,
	BGBCC_SH_REG_RD7,  BGBCC_SH_REG_RD6,


	BGBCC_SH_REG_RD63, BGBCC_SH_REG_RD62,
	BGBCC_SH_REG_RD61, BGBCC_SH_REG_RD60,

	BGBCC_SH_REG_RD59, BGBCC_SH_REG_RD58,
	BGBCC_SH_REG_RD57, BGBCC_SH_REG_RD56,
	BGBCC_SH_REG_RD55, BGBCC_SH_REG_RD54,
	BGBCC_SH_REG_RD53, BGBCC_SH_REG_RD52,
	BGBCC_SH_REG_RD51, BGBCC_SH_REG_RD50,

	BGBCC_SH_REG_RD49, BGBCC_SH_REG_RD48,
	BGBCC_SH_REG_RD47, BGBCC_SH_REG_RD46,
	BGBCC_SH_REG_RD45, BGBCC_SH_REG_RD44,
	BGBCC_SH_REG_RD43, BGBCC_SH_REG_RD42,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_qcachereg_xg2rv[64]={
	BGBCC_SH_REG_RQ26,
	BGBCC_SH_REG_RQ25, BGBCC_SH_REG_RQ24,
	BGBCC_SH_REG_RQ23, BGBCC_SH_REG_RQ22,
	BGBCC_SH_REG_RQ21, BGBCC_SH_REG_RQ20,

	BGBCC_SH_REG_RQ19, BGBCC_SH_REG_RQ18,
	BGBCC_SH_REG_RQ9, BGBCC_SH_REG_RQ8,

	BGBCC_SH_REG_RQ59, BGBCC_SH_REG_RQ58,
	BGBCC_SH_REG_RQ57, BGBCC_SH_REG_RQ56,

	BGBCC_SH_REG_RQ55, BGBCC_SH_REG_RQ54,
	BGBCC_SH_REG_RQ53, BGBCC_SH_REG_RQ52,
	BGBCC_SH_REG_RQ51, BGBCC_SH_REG_RQ50,
	BGBCC_SH_REG_RQ41, BGBCC_SH_REG_RQ40,

	BGBCC_SH_REG_RQ31, BGBCC_SH_REG_RQ30,
	BGBCC_SH_REG_RQ29, BGBCC_SH_REG_RQ28,
	BGBCC_SH_REG_RQ17, BGBCC_SH_REG_RQ16,
	BGBCC_SH_REG_RQ15, BGBCC_SH_REG_RQ14,

	BGBCC_SH_REG_RQ13,  BGBCC_SH_REG_RQ12,
	BGBCC_SH_REG_RQ11,  BGBCC_SH_REG_RQ10,
	BGBCC_SH_REG_RQ7 ,  BGBCC_SH_REG_RQ6,

	BGBCC_SH_REG_RQ63, BGBCC_SH_REG_RQ62,
	BGBCC_SH_REG_RQ61, BGBCC_SH_REG_RQ60,

	BGBCC_SH_REG_RQ59, BGBCC_SH_REG_RQ58,
	BGBCC_SH_REG_RQ57, BGBCC_SH_REG_RQ56,
	BGBCC_SH_REG_RQ55, BGBCC_SH_REG_RQ54,
	BGBCC_SH_REG_RQ53, BGBCC_SH_REG_RQ52,
	BGBCC_SH_REG_RQ51, BGBCC_SH_REG_RQ50,

	BGBCC_SH_REG_RQ49, BGBCC_SH_REG_RQ48,
	BGBCC_SH_REG_RQ47, BGBCC_SH_REG_RQ46,
	BGBCC_SH_REG_RQ45, BGBCC_SH_REG_RQ44,
	BGBCC_SH_REG_RQ43, BGBCC_SH_REG_RQ42,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_pcachereg_xg2rv[64]={
	BGBCC_SH_REG_R26,
	BGBCC_SH_REG_R25, BGBCC_SH_REG_R24,
	BGBCC_SH_REG_R23, BGBCC_SH_REG_R22,
	BGBCC_SH_REG_R21, BGBCC_SH_REG_R20,

	BGBCC_SH_REG_R19, BGBCC_SH_REG_R18,
	BGBCC_SH_REG_R9, BGBCC_SH_REG_R8,

	BGBCC_SH_REG_R59, BGBCC_SH_REG_R58,
	BGBCC_SH_REG_R57, BGBCC_SH_REG_R56,

	BGBCC_SH_REG_R55, BGBCC_SH_REG_R54,
	BGBCC_SH_REG_R53, BGBCC_SH_REG_R52,
	BGBCC_SH_REG_R51, BGBCC_SH_REG_R50,
	BGBCC_SH_REG_R41, BGBCC_SH_REG_R40,

	BGBCC_SH_REG_R31, BGBCC_SH_REG_R30,
	BGBCC_SH_REG_R29, BGBCC_SH_REG_R28,
	BGBCC_SH_REG_R17, BGBCC_SH_REG_R16,
	BGBCC_SH_REG_R15, BGBCC_SH_REG_R14,

	BGBCC_SH_REG_R13,  BGBCC_SH_REG_R12,
	BGBCC_SH_REG_R11,  BGBCC_SH_REG_R10,
	BGBCC_SH_REG_R7,  BGBCC_SH_REG_R6,

	BGBCC_SH_REG_R63, BGBCC_SH_REG_R62,
	BGBCC_SH_REG_R61, BGBCC_SH_REG_R60,

	BGBCC_SH_REG_R59, BGBCC_SH_REG_R58,
	BGBCC_SH_REG_R57, BGBCC_SH_REG_R56,
	BGBCC_SH_REG_R55, BGBCC_SH_REG_R54,
	BGBCC_SH_REG_R53, BGBCC_SH_REG_R52,
	BGBCC_SH_REG_R51, BGBCC_SH_REG_R50,

	BGBCC_SH_REG_R49, BGBCC_SH_REG_R48,
	BGBCC_SH_REG_R47, BGBCC_SH_REG_R46,
	BGBCC_SH_REG_R45, BGBCC_SH_REG_R44,
	BGBCC_SH_REG_R43, BGBCC_SH_REG_R42,

	BGBCC_SH_REG_ZZR };

const short bgbcc_jx2_lcachereg_xg2rv[64]={
	BGBCC_SH_REG_LR26,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR24,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR22,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR20,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR18,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR8,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR58,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR56,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR54,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR52,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR50,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR40,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR30,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR28,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR16,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR14,

	BGBCC_SH_REG_ZZR,  BGBCC_SH_REG_LR12,
	BGBCC_SH_REG_ZZR,  BGBCC_SH_REG_LR10,
	BGBCC_SH_REG_ZZR,  BGBCC_SH_REG_LR6,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR62,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR60,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR58,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR56,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR54,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR52,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR50,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR48,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR46,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR44,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_LR42,

	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_ZZR,
	BGBCC_SH_REG_ZZR, BGBCC_SH_REG_ZZR
	};

const short bgbcc_jx2_maxreg_xg2rv=23;
const short bgbcc_jx2_maxreg_xg2rv_lf=59;

#endif



/* For leaf traces, rotate index to prefer scratch registers.
 */
int BGBCC_JX2C_EmitRotateRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx, int flag)
{
	const short *shuf;
	int m1, m2, m3;
	int i1, i2, h;

	if(sctx->is_leaftiny&1)
		return(idx);

//	h=((sctx->tr_opnum*251)>>8)&7;
	h=((sctx->tr_opnum*251)>>8)&3;

#if 0
//	switch(sctx->rov_rshuf&3)
	switch(h)
	{
//		case 0: shuf=bgbcc_jx2_shufreg0_egpr; break;
//		case 1: shuf=bgbcc_jx2_shufreg1_egpr; break;
//		case 2: shuf=bgbcc_jx2_shufreg2_egpr; break;
//		case 3: shuf=bgbcc_jx2_shufreg3_egpr; break;

		case 0: shuf=sctx->shufreg0; break;
		case 1: shuf=sctx->shufreg1; break;
		case 2: shuf=sctx->shufreg2; break;
		case 3: shuf=sctx->shufreg3; break;
	}
#endif

	if(!(sctx->is_tr_leaf&1))
	{
		m1=sctx->maxreg_gpr;

//		if(((idx&(~7))+8)>m1)
		if(((idx&(~3))+4)>m1)
			h=0;

		i1=idx;
//		if(flag&1)
//			i1=shuf[i1];

#if 0
		if(flag&1)
		{
			i2=i1^h;
			if(i2<m1)
				i1=i2;
		}
#endif

//		if((((i1&(~3))+4)<=m1) && (flag&1))
//			i1=i1^(sctx->rov_rshuf&3);

//		return(idx);
		return(i1);
	}

	m1=sctx->maxreg_gpr;
	m2=sctx->maxreg_gpr_lf;
	m3=m2-m1;
	
	if(idx<m3)
		i1=(m1+idx);
	else
		i1=(idx-m3);
	if((i1<0) || (i1>=m2))
		{ BGBCC_DBGBREAK }

//	if((i1+4)<=m2)
//	if(((i1+4)<=m2) && (flag&1))
//	if((((i1&(~3))+4)<=m2) && (flag&1))
//		i1=i1^(sctx->rov_rshuf&3);
//	if(flag&1)
//		i1=shuf[i1];

//	if(((idx&(~7))+8)>m2)
	if(((idx&(~3))+4)>m2)
		h=0;

	if(((i1&(~3))<m3) && (((i1&(~3))+4)>=m3))
		h=0;

#if 0
	if(flag&1)
	{
		i2=i1^h;
		if(i2<m2)
//		if((i2<m2) && (((i1<m3) && (i2<m3)) || ((i1>=m3) && (i2>=m3))))
			i1=i2;
	}
#endif

	return(i1);
}

int BGBCC_JX2C_EmitRotateLpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx, int flag)
{
	int m1, m2, m3;
	int i1;

	if(sctx->is_leaftiny&1)
		return(idx);

	if(!(sctx->is_tr_leaf&1))
	{
		i1=idx;
//		if(idx<sctx->maxreg_gpr)
//			{ i1=sctx->maxreg_gpr-idx-1; }
		return(i1);
	}
	
	m1=sctx->maxreg_gpr;
	m2=sctx->maxreg_gpr_lf;
	m3=m2-m1;
	
	if(idx<m3)
		i1=(m1+(m3-idx-1));
	else
		i1=m1-(idx-m3)-1;
	if((i1<0) || (i1>=m2))
		{ BGBCC_DBGBREAK }

	i1=idx;

	return(i1);
}

int BGBCC_JX2C_CheckRegisterIndexScratchP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx)
{
	int creg, tr0;
	
	creg=sctx->qcachereg[idx];
	
	tr0=creg&63;
	if((tr0>=2) && (tr0<8))
		return(1);
	if((tr0>=16) && (tr0<24))
		return(1);
	if((tr0>=32) && (tr0<40))
		return(1);
	if((tr0>=48) && (tr0<56))
		return(1);
	return(0);
}

int BGBCC_JX2C_CheckRegisterIndexArgIdxP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx, int argid)
{
	static int regargs[16]={
		BGBCC_SH_REG_RQ4, BGBCC_SH_REG_RQ5,
		BGBCC_SH_REG_RQ6, BGBCC_SH_REG_RQ7,
		BGBCC_SH_REG_RQ20, BGBCC_SH_REG_RQ21,
		BGBCC_SH_REG_RQ22, BGBCC_SH_REG_RQ23,
		BGBCC_SH_REG_RQ36, BGBCC_SH_REG_RQ37,
		BGBCC_SH_REG_RQ38, BGBCC_SH_REG_RQ39,
		BGBCC_SH_REG_RQ52, BGBCC_SH_REG_RQ53,
		BGBCC_SH_REG_RQ54, BGBCC_SH_REG_RQ55
		};
	int creg, tr0, tr1;
	
	if(argid<0)
		return(0);
	if(argid>=16)
		return(0);
	
	if(idx<0)
		return(0);
	if(idx>=sctx->maxreg_gpr_lf)
		return(0);
		
//	if(!(sctx->has_xgpr&2))
	if(1)
	{
		if(argid>=8)
			return(0);
	}
	
	creg=sctx->qcachereg[idx];
	tr1=regargs[argid];
	
	if(creg==BGBCC_SH_REG_ZZR)
		return(0);

	return(creg==tr1);

//	tr0=creg&63;

#if 0
	if((tr0>=4) && (tr0<8))
	{
		if((tr0-4)==argid)
			return(1);
		return(0);
	}

	if((tr0>=20) && (tr0<24))
	{
		if(((tr0-20)+4)==argid)
			return(1);
		return(0);
	}

	if((tr0>=36) && (tr0<40))
	{
		if(((tr0-36)+8)==argid)
			return(1);
		return(0);
	}

	if((tr0>=52) && (tr0<56))
	{
		if(((tr0-52)+12)==argid)
			return(1);
		return(0);
	}
#endif

	return(0);
}



int BGBCC_JX2C_CheckRegisterIndexExcludeP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx)
{
	int creg, tr0;
	
	if(idx>=sctx->maxreg_gpr_lf)
		return(1);
	
	creg=sctx->qcachereg[idx];
	
	if(creg==BGBCC_SH_REG_ZZR)
		return(1);
	
	tr0=creg&63;

	if(sctx->is_leaftiny&1)
	{
		if(sctx->regs_excl_tiny&(1ULL<<tr0))
			return(1);
	}else
	{
		if(sctx->regs_excl&(1ULL<<tr0))
			return(1);
	}

	return(0);
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
	int maxreg;
	u64 vrsave, uli, ulj;
	int creg, excl, nsv, userq, usepq, rcls, vspfl;
	int pr0, pr1, pr0b, pr1b, i1;
	int i, j, bi;

	if(reg.val==CCXL_REGID_REG_Z)
		return(BGBCC_SH_REG_ZZR);
	if(reg.val==CCXL_REGID_REG_DZ)
		return(BGBCC_SH_REG_ZZR);

//	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	tty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	vspfl=BGBCC_JX2C_GetFrameVRegVspanFlags(ctx, sctx, reg);

	if(ctx->arch_sizeof_ptr==16)
	{
		if(	(rcls==BGBCC_SH_REGCLS_VO_REF)	||
			(rcls==BGBCC_SH_REGCLS_AR_REF)	)
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
		
		if(	(rcls==BGBCC_SH_REGCLS_VO_GR)	||
			(rcls==BGBCC_SH_REGCLS_VO_QGR)	||
			(rcls==BGBCC_SH_REGCLS_VO_QGR2)	)
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
	}

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
		sctx->is_leaftiny|=8;
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
//	if(rcls==BGBCC_SH_REGCLS_GR2)
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_QGR2))
	if(	(rcls==BGBCC_SH_REGCLS_GR2) ||
		(rcls==BGBCC_SH_REGCLS_QGR2) ||
		(rcls==BGBCC_SH_REGCLS_VO_REF2) ||
		(rcls==BGBCC_SH_REGCLS_AR_REF2) )
	{
		sctx->is_leaftiny|=8;

		creg=BGBCC_JX2C_EmitTryGetLpRegister(ctx, sctx, reg, fl);
		if((creg&1) && (creg!=BGBCC_SH_REG_ZZR))
		{
//			BGBCC_DBGBREAK
		}
		return(creg);
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
//		sctx->is_leaftiny|=8;
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
//	vrsave=0x00FF00FF00FF00FCULL;
	vrsave=BGBCC_JX2C_GetFrameVrsaveMask(ctx, sctx);

	if(sctx->is_leaftiny&1)
	{
//		excl=0;
//		vrsave=0xFFFFFF;
//		excl=0x00007FFF;
	}

#if 1
	/* value already in a register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
//	for(i=0; i<maxreg; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
//		if(excl&(1ULL<<i))
		if((excl>>i)&1)
			continue;

		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i))
			continue;

//		if(!((sctx->regalc_live)&(1ULL<<i)))
//			continue;

//		if(!((sctx->regalc_save)&(1ULL<<i)))
//			continue;

#if 0
		ulj=(sctx->regalc_save)>>i;
		if(!(ulj&1))
//		if(!(j&1) && !sctx->is_leaftiny)
		{
			if(!ulj)
				break;
			if(!(ulj&15))
				{ i+=3; continue; }
			continue;
		}
#endif

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=1ULL<<i;
			if(fl&1)
			{
				sctx->regalc_dirty|=1ULL<<i;
				sctx->regalc_gbldirty|=1ULL<<i;
				sctx->regalc_map[i]=reg;
			}
			
			if((sctx->regalc_noval&(1ULL<<i)) && (fl&2))
			{
				BGBCC_DBGBREAK
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

//	if(sctx->is_leaftiny && ((fl&3)==1))
	if(sctx->is_leaftiny&1)
	{
		bi=-1;
		for(i=0; i<maxreg; i++)
		{
			if((excl>>i)&1)
				continue;

			i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i, 1);

			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1))
				continue;

			if(!BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
				continue;

			reg1=sctx->regalc_map[i1];
			if(!((sctx->regalc_save)&(1ULL<<i1)))
				{ bi=i1; break; }
			if(reg1.val==CCXL_REGID_REG_DZ)
				continue;
			if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i1; break; }
		}

		if(bi>=0)
		{
			i=bi;

			BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
			BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=1ULL<<i;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_map[i]=reg;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_live|=1ULL<<i;
			if(fl&1)
			{
				sctx->regalc_dirty|=1ULL<<i;
				sctx->regalc_gbldirty|=1ULL<<i;
				if(i>=sctx->vsp_rsv)
					sctx->regalc_noval|=1ULL<<i;
			}
			if(userq)
				creg=sctx->qcachereg[i];
			else if(usepq)
				creg=sctx->pcachereg[i];
			else
				creg=sctx->jcachereg[i];
//			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
			if(fl&2)
			{
				sctx->regalc_noval&=~(1ULL<<i);
				BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, reg);
				BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg);
			}else
			{
				BGBCC_JX2C_EmitMarkFrameVReg(ctx, sctx, reg);
			}
			return(creg);
		}

		return(BGBCC_SH_REG_ZZR);
	}

#if 1
	if(vspfl&BGBCC_RSPFL_ISCALLARG)
//	if((vspfl&BGBCC_RSPFL_ISCALLARG) &&
//		!(ctx->cur_func->regflags&BGBCC_REGFL_NOSCRATCHDYN))
	{
		j=(vspfl>>16)&0x3F;
		i1=-1;
		if((j>=0) && (j<4))
			{ i1=BGBCC_SH_REG_RQ4+j; }
		if((j>=4) && (j<8))
			{ i1=BGBCC_SH_REG_RQ20+(j-4); }

		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	
		bi=-1;
//		for(i=0; i<maxreg; i++)
		for(i=0; i<sctx->maxreg_gpr_lf; i++)
		{
			if(BGBCC_JX2C_CheckRegisterIndexArgIdxP(ctx, sctx, i, j)>0)
			{
				bi=i;
				break;
			}

#if 0
			creg=sctx->qcachereg[i];
			if(creg==i1)
			{
				bi=i;
				break;
			}
#endif
		}
		
//		if(!(fl&1))
		if((fl&3)!=1)
//		if((fl&3)==3)
		{
			bi=-1;
		}

//		if(sctx->regalc_utcnt[bi]>0)
		if(sctx->regalc_live&(1ULL<<bi))
		{
			bi=-1;
		}
		
		if(ctx->cur_vtr->trfl&CCXL_TRFL_USES_SCRATCH)
		{
			bi=-1;
		}
		
		if(((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv)))
		{
//			bi=-1;
		}

//		bi=-1;

		if(bi>=0)
		{
			for(i=0; i<sctx->maxreg_gpr_lf; i++)
			{
				reg1=sctx->regalc_map[i];

				if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg1, reg) &&
					(i!=bi))
				{
//					if(sctx->regalc_utcnt[i]>0)
//					if(sctx->regalc_live&(1ULL<<i))
					if((sctx->regalc_live&(1ULL<<i)) || ((fl&3)!=1))
					{
						bi=-1;
//						break;
					}else
					{
						BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
//						BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);
						reg1.val=CCXL_REGID_REG_Z;
						sctx->regalc_map[i]=reg1;
					}

				}

#if 0
				if(reg1.val==reg.val)
//				if(BGBCC_CCXL_RegisterEqualP(ctx, reg1, reg))
				{
					bi=-1;
				}else
					if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg1, reg) &&
						(i!=bi))
				{
					if(sctx->regalc_dirty)
					{
						bi=-1;
					}else
					{
						BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
						BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);
					}
				}
#endif
			}
		}
		
#if 1
		if(bi>=0)
		{
			i=bi;

			BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
//			BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=1ULL<<i;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_map[i]=reg;
//			sctx->regalc_utcnt[i]=1;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_live|=1ULL<<i;
			sctx->regalc_isarg|=1ULL<<i;
			
			if(sctx->regalc_utcnt[i]<1)
				sctx->regalc_utcnt[i]=1;

			if(fl&1)
			{
				sctx->regalc_dirty|=1ULL<<i;
				sctx->regalc_gbldirty|=1ULL<<i;
				if(i>=sctx->vsp_rsv)
					sctx->regalc_noval|=1ULL<<i;
			}

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
#endif

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

	pr0b=pr0;
	if(!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, reg))
		pr0b+=9999;

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
			i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i, 1);

			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1))
				continue;

			if(i1>=maxreg)
				BGBCC_CCXL_SanityError(ctx);

//			if(excl&(1ULL<<i1))
			if((excl>>i1)&1)
				continue;

			if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

//			if(!(sctx->is_tr_leaf&1) &&
			if((!(sctx->is_tr_leaf&1) ||
				(ctx->cur_vtr->trfl&CCXL_TRFL_USES_SCRATCH) ||
				(ctx->cur_vtr->trfl&CCXL_TRFL_MEMCOPY)) &&
				BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
					continue;

			if(!((sctx->regalc_save)&(1ULL<<i1)))
			{
//				creg=bgbcc_jx2_cachereg[i1];
				creg=sctx->jcachereg[i1];
//				if(sctx->reg_save&(1ULL<<(creg&15)))
//				if(sctx->reg_save&(1ULL<<(creg&31)))
//				if((sctx->reg_save|vrsave)&(1ULL<<(creg&31)))
				if((sctx->reg_save|vrsave)&(1ULL<<(creg&63)))
					{ bi=i1; break; }

				nsv++;
				continue;
			}

//			if(!((sctx->regalc_live)&(1ULL<<i1)))
			if(!((sctx->regalc_live)&(1ULL<<i1)) &&
				!((sctx->regalc_dirty)&(1ULL<<i1)))
			{
				reg1=sctx->regalc_map[i1];
				pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);
				
				pr1b=pr1;
				if(!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, reg1))
					pr1b+=9999;
				
				if(reg1.val==CCXL_REGID_REG_DZ)
					continue;

				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i1; break; }
				else
				{
//					if((bi<0) || 
//						(sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
//					if(pr0<=pr1)
					if((bi<0) ||
						((sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]) &&
//						(pr0<=pr1)))
						(pr0b<=pr1b)))
					{
//						if((pr1>=0) && (pr0>=0) && (pr0>=pr1))
							bi=i1;
					}
				}
			}
		}

		/* Check if we are better off allocating another register */
		if(bi>=0)
		{
			reg1=sctx->regalc_map[bi];
			if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
//				sctx->is_simpass &&
				(sctx->is_simpass&32) &&
				!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
			{
				bi=-1;
			}

			if(reg1.val==CCXL_REGID_REG_DZ)
				{ BGBCC_DBGBREAK }
		}

		if((pr0>=0) && (pr0<sctx->vsp_rsv))
			bi=pr0;
		
#if 1
		if(bi>=0)
		{
			i=bi;

			BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
			BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=1ULL<<i;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_map[i]=reg;
//			sctx->regalc_utcnt[i]=1;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_live|=1ULL<<i;
			if(fl&1)
			{
				sctx->regalc_dirty|=1ULL<<i;
				sctx->regalc_gbldirty|=1ULL<<i;
				if(i>=sctx->vsp_rsv)
					sctx->regalc_noval|=1ULL<<i;
			}
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
	int maxreg;
	u64 vrsave;
	int creg, lng, excl, bi, nsv, userq, usepq, rcls;
	int pr0, pr1, pr0b, pr1b;
	int i, i1;

	if(reg.val==CCXL_REGID_REG_Z)
		return(BGBCC_SH_REG_ZZR);
	if(reg.val==CCXL_REGID_REG_DZ)
		return(BGBCC_SH_REG_ZZR);

	creg=BGBCC_JX2C_EmitTryGetRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
		if(sctx->is_leaftiny&1)
		{
			if(BGBCC_JX2C_SaveFrameOfsForReg(ctx, sctx, creg)!=0)
				{ BGBCC_DBGBREAK }
		}

		return(creg);
	}

	if(sctx->is_leaftiny&1)
	{
#if 0
//		if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
		if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY) &&
				!(sctx->is_leaftiny&8))
			printf("Reject LeafTiny RegAllocFail %s\n",
				ctx->cur_func->qname);
#endif

		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		sctx->is_leaftiny|=8;
		sctx->is_leaftiny|=4;
		sctx->is_leaftiny&=~1;
//		return(BGBCC_SH_REG_ZZR);
	}


//	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	tty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
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
	
	if(ctx->arch_sizeof_ptr==16)
	{
		if(	(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF))
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
		
		if(		(rcls==BGBCC_SH_REGCLS_VO_GR)	||
				(rcls==BGBCC_SH_REGCLS_VO_QGR)	||
				(rcls==BGBCC_SH_REGCLS_VO_QGR2)
				)
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
	}
	
//	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) && !(sctx->is_addr64))
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2))
//	if(rcls==BGBCC_SH_REGCLS_GR2)
//	if((rcls==BGBCC_SH_REGCLS_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2) ||
//		(rcls==BGBCC_SH_REGCLS_VO_QGR2))
	if(	(rcls==BGBCC_SH_REGCLS_GR2) ||
		(rcls==BGBCC_SH_REGCLS_QGR2) ||
		(rcls==BGBCC_SH_REGCLS_VO_REF2) ||
		(rcls==BGBCC_SH_REGCLS_AR_REF2)	)
	{
		creg=BGBCC_JX2C_EmitGetLpRegister(ctx, sctx, reg, fl);
		if((creg&1) && (creg!=BGBCC_SH_REG_ZZR))
		{
//			BGBCC_DBGBREAK
		}
		return(creg);
	}

	usepq=0;
	switch(rcls)
	{
	case BGBCC_SH_REGCLS_VO_QGR:
	case BGBCC_SH_REGCLS_QGR:
	case BGBCC_SH_REGCLS_QGR2:
	case BGBCC_SH_REGCLS_VO_QGR2:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_VO_REF:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_GR2:
	case BGBCC_SH_REGCLS_GR2:
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
//	vrsave=0x00FF00FF00FF00FCULL;
	vrsave=BGBCC_JX2C_GetFrameVrsaveMask(ctx, sctx);

//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

	pr0b=pr0;
	if(!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, reg))
		pr0b+=9999;

	bi=-1; nsv=0;
	/* Check for registers not holding a live value. */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
	{
		i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i, 1);
//		i1=i;

		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1))
			continue;

		if(i1>=maxreg)
			BGBCC_CCXL_SanityError(ctx);


		if(excl&(1ULL<<i1))
			continue;

		if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

//		if(!(sctx->is_tr_leaf&1) &&
		if((!(sctx->is_tr_leaf&1) ||
			(ctx->cur_vtr->trfl&CCXL_TRFL_USES_SCRATCH) ||
			(ctx->cur_vtr->trfl&CCXL_TRFL_MEMCOPY)) &&
			BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
				continue;

		if(!((sctx->regalc_save)&(1ULL<<i1)))
		{
//			creg=bgbcc_jx2_cachereg[i1];
			creg=sctx->jcachereg[i1];
//			if(sctx->reg_save&(1ULL<<(creg&15)))
//			if(sctx->reg_save&(1ULL<<(creg&31)))
//			if((sctx->reg_save|vrsave)&(1ULL<<(creg&31)))
			if((sctx->reg_save|vrsave)&(1ULL<<(creg&63)))
				{ bi=i1; break; }

			nsv++;
			continue;
		}
//		if(!((sctx->regalc_live)&(1ULL<<i1)))
		if(	!((sctx->regalc_live)&(1ULL<<i1)) &&
			!((sctx->regalc_dirty)&(1ULL<<i1)))
		{
			reg1=sctx->regalc_map[i1];
			pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

			pr1b=pr1;
			if(!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, reg1))
				pr1b+=9999;

			if(reg1.val==CCXL_REGID_REG_DZ)
				continue;

			if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i1; break; }
			else
			{
	//			if((bi<0) || (sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]))
//				if((bi<0) || (sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]))
//				if(pr0<=pr1)
				if((bi<0) ||
					((sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]) &&
//					(pr0<=pr1)))
					(pr0b<=pr1b)))
				{
//					if((pr1>=0) && (pr0>=0) && (pr0>=pr1))
						bi=i1;
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
			i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i, 1);
//			i1=i;

			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1))
				continue;

			if(i1>=maxreg)
				BGBCC_CCXL_SanityError(ctx);

			if(excl&(1ULL<<i1))
				continue;

			if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

//			if(!(sctx->is_tr_leaf&1) &&
			if((!(sctx->is_tr_leaf&1) ||
				(ctx->cur_vtr->trfl&CCXL_TRFL_USES_SCRATCH) ||
				(ctx->cur_vtr->trfl&CCXL_TRFL_MEMCOPY)) &&
				BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
					continue;

			if(!((sctx->regalc_save)&(1ULL<<i1)))
				{ continue; }
			if(!((sctx->regalc_live)&(1ULL<<i1)))
			{
				reg1=sctx->regalc_map[i1];
				if(reg1.val==CCXL_REGID_REG_DZ)
					continue;
				if((bi<0) || (sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]))
					bi=i1;
				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i1; break; }
			}
		}
	}

//	if((pr0>=0) && (pr0<2))
//		bi=pr0;

	/* Check if we are better off allocating another register */
	if(bi>=0)
	{
		reg1=sctx->regalc_map[bi];
//		if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
		if((nsv>=1) && (sctx->regalc_ltcnt[bi]<=2) &&
//			sctx->is_simpass &&
			(sctx->is_simpass&32) &&
			!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
		{
			bi=-1;
		}
		if(reg1.val==CCXL_REGID_REG_DZ)
			{ BGBCC_DBGBREAK }
	}

//	if((sctx->vsp_rsv>0) && (bi<sctx->vsp_rsv) && (pr0>=sctx->vsp_rsv))
//		bi=-1;

	if((pr0>=0) && (pr0<sctx->vsp_rsv))
		bi=pr0;

#if 1
	if(bi>=0)
	{
		i=bi;

		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
		BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

//		if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
		if(BGBCC_CCXL_IsRegZzP(ctx, reg))
			{ BGBCC_DBGBREAK }

		sctx->regalc_save|=1ULL<<i;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_live|=1ULL<<i;
		if(fl&1)
		{
			sctx->regalc_dirty|=1ULL<<i;
			sctx->regalc_gbldirty|=1ULL<<i;
			if(i>=sctx->vsp_rsv)
				sctx->regalc_noval|=1ULL<<i;
		}
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
		{
			sctx->regalc_noval&=~(1ULL<<i);
			BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, reg);
			BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg);
		}else
		{
			BGBCC_JX2C_EmitMarkFrameVReg(ctx, sctx, reg);
		}

		sctx->rov_rshuf++;
		return(creg);
	}
#endif

	/* Check for unallocated registers. */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<maxreg; i++)
	{
//		i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i, 0);
		i1=BGBCC_JX2C_EmitRotateRegisterIndex(ctx, sctx, i, 3);

		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1))
			continue;

		if(i1>=maxreg)
			BGBCC_CCXL_SanityError(ctx);


		if(excl&(1ULL<<i1))
			continue;

		if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

//		if(!(sctx->is_tr_leaf&1) &&
		if((!(sctx->is_tr_leaf&1) ||
			(ctx->cur_vtr->trfl&CCXL_TRFL_USES_SCRATCH) ||
			(ctx->cur_vtr->trfl&CCXL_TRFL_MEMCOPY)) &&
			BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
				continue;

		if((sctx->regalc_save)&(1ULL<<i1))
			continue;

		BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i1);
		
		sctx->regalc_ltcnt[i1]=0;
		sctx->regalc_map[i1]=reg;
		sctx->regalc_utcnt[i1]=1;
		sctx->regalc_save|=1ULL<<i1;
		sctx->regalc_live|=1ULL<<i1;
		if(fl&1)
		{
			sctx->regalc_dirty|=1ULL<<i1;
			sctx->regalc_gbldirty|=1ULL<<i1;
			if(i>=sctx->vsp_rsv)
				sctx->regalc_noval|=1ULL<<i1;
		}

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
			sctx->regalc_noval&=~(1ULL<<i1);
//			printf("load from uninitialized register\n");
		}
		
		sctx->rov_rshuf++;
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

int BGBCC_JX2C_EmitBindVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int dreg)
{
	int i, bi, creg, maxreg;

	maxreg=sctx->maxreg_gpr_lf;

	bi=-1;
	for(i=0; i<maxreg; i++)
	{
		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i))
			continue;

		creg=sctx->qcachereg[i];
//		if((creg&31)==(dreg&31))
		if((creg&63)==(dreg&63))
			{ bi=i; break; }
	}

	if(bi>=0)
	{
		i=bi;

//		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
//		BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

		if(BGBCC_CCXL_IsRegZzP(ctx, reg))
			{ BGBCC_DBGBREAK }

		sctx->regalc_save|=1ULL<<i;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_live|=1ULL<<i;
		creg=sctx->qcachereg[i];

//		if(fl&1)
//			sctx->regalc_dirty|=1ULL<<i;
//		if(userq)
//			creg=sctx->qcachereg[i];
//		else if(usepq)
//			creg=sctx->pcachereg[i];
//		else
//			creg=sctx->jcachereg[i];
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//		if((fl&2) && (i>=sctx->vsp_rsv))
//			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }

		sctx->rov_rshuf++;
		return(creg);
	}
	
	return(-1);
}

int BGBCC_JX2C_EmitBindVRegRegDirty(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int dreg)
{
	int i, bi, creg, maxreg;

	maxreg=sctx->maxreg_gpr_lf;

	bi=-1;
	for(i=0; i<maxreg; i++)
	{
		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i))
			continue;

		creg=sctx->qcachereg[i];
		if((creg&63)==(dreg&63))
			{ bi=i; break; }
	}

	if(bi>=0)
	{
		i=bi;

		if(BGBCC_CCXL_IsRegZzP(ctx, reg))
			{ BGBCC_DBGBREAK }

		sctx->regalc_save|=1ULL<<i;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_map[i]=reg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_live|=1ULL<<i;

		sctx->regalc_dirty|=1ULL<<i;
		sctx->regalc_gbldirty|=1ULL<<i;
		if(i>=sctx->vsp_rsv)
			sctx->regalc_noval|=1ULL<<i;

		creg=sctx->qcachereg[i];

//		sctx->rov_rshuf++;
		return(creg);
	}
	
	return(-1);
}

int BGBCC_JX2C_GetRegisterIndexForReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int dreg)
{
	int i, bi, creg, maxreg;

	maxreg=sctx->maxreg_gpr_lf;

	bi=-1;
	for(i=0; i<maxreg; i++)
	{
		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i))
			continue;

		creg=sctx->qcachereg[i];
//		if((creg&31)==(dreg&31))
		if((creg&63)==(dreg&63))
			{ bi=i; break; }
	}

	if(bi>=0)
	{
		return(bi);
	}
	
	return(-1);
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

int BGBCC_JX2C_CheckVRegMoreUsesInTraceStepP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int step)
{
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_VirtTr *tr;
	BGBCC_CCXL_VirtOp *vop, *vop1, *vop2;
	int cvi, cvs, cve;
//	int ps0, ps1, tr1, tr2, usewex;
	int i, j, k;

	obj=ctx->cur_func;
	tr=ctx->cur_vtr;
	cvi=sctx->tr_opnum;
	
	if(!tr)
		return(0);
	
	if(reg.val==CCXL_REGID_REG_Z)
		return(0);
	if(reg.val==CCXL_REGID_REG_DZ)
		return(0);
	
	cvs=tr->b_ops;
	cve=cvs+tr->n_ops;
	
//	for(i=cvi+1; i<cve; i++)
	for(i=cvi+step; i<cve; i++)
	{
		vop=obj->vop[i];
		
		if(
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->dst, reg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->srca, reg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->srcb, reg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->srcc, reg) ||
			BGBCC_CCXL_RegisterIdentEqualP(ctx, vop->srcd, reg)	)
		{
			return(1);
		}
	}
	
	return(0);
}

int BGBCC_JX2C_CheckVRegMoreUsesInTraceP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_CheckVRegMoreUsesInTraceStepP(ctx, sctx, reg, 1));
}

int BGBCC_JX2C_CheckVRegConsumeNextOpP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	if(	BGBCC_JX2C_CheckVRegMoreUsesInTraceStepP(ctx, sctx, reg, 1) &&
		!BGBCC_JX2C_CheckVRegMoreUsesInTraceStepP(ctx, sctx, reg, 2))
	{
		return(1);
	}
	return(0);
}

int BGBCC_JX2C_CheckVRegCrossTraceP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	int vspfl;
	vspfl=BGBCC_JX2C_GetFrameVRegVspanFlags(ctx, sctx, reg);
	if(vspfl&BGBCC_RSPFL_CROSSTRACE)
		return(1);
	return(0);
}

int BGBCC_JX2C_ProbeVRegInRegisterP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	ccxl_register reg1;
	int pr0;
	int i;

#if 1
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
			return(1);
	}
#endif

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	if(pr0<sctx->vsp_rsv)
		return(1);

	return(0);
}

int BGBCC_JX2C_ProbeVRegInRegisterActiveP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	ccxl_register reg1;
	int pr0, maxreg;
	int i;

	maxreg=sctx->maxreg_gpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_gpr_lf;

#if 1
	for(i=0; i<maxreg; i++)
	{
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
			return(1);
	}
#endif

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	if(pr0<sctx->vsp_rsv)
		return(1);

	return(0);
}

int BGBCC_JX2C_ProbeVRegIsCallArgP(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	u64 vspfl;

	vspfl=BGBCC_JX2C_GetFrameVRegVspanFlags(ctx, sctx, reg);
	if(vspfl&BGBCC_RSPFL_ISCALLARG)
		return(1);
	return(0);
}

/** Try to figure out if it would be better to skip using an immediate,
  * and instead load the constant into a register.
  */
int BGBCC_JX2C_ProbeVRegRejectImm3P(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register dreg, ccxl_register sreg, ccxl_register treg, int nm)
{
	s64 li;
	s32 k;
	int nm1;

	if(!BGBCC_CCXL_IsRegImmSmallSIntP(ctx, treg))
		return(1);
	li=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
	k=li;
	
	if(k!=li)
		return(1);

	nm1=nm;

#if 1
	if(	BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, treg) ||
		BGBCC_JX2C_ProbeVRegInRegisterActiveP(ctx, sctx, treg))
	{
		switch(nm1)
		{
		case BGBCC_SH_NMID_ADD:
		case BGBCC_SH_NMID_SUB:
#if 1
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			{
				if(((k&65535)!=k) && ((k|(~65535))!=k))
					{ nm1=-1; }
				break;
			}
#endif

			if(((k&511)!=k) && ((k|(~511))!=k))
				{ nm1=-1; }
			break;

		case BGBCC_SH_NMID_ADDSL:
		case BGBCC_SH_NMID_SUBSL:
		case BGBCC_SH_NMID_ADDUL:
		case BGBCC_SH_NMID_SUBUL:
			if(((k&511)!=k) && ((k|(~511))!=k))
				{ nm1=-1; }
			break;
		case BGBCC_SH_NMID_AND:
		case BGBCC_SH_NMID_OR:
		case BGBCC_SH_NMID_XOR:
		case BGBCC_SH_NMID_DMULS:
		case BGBCC_SH_NMID_DMULU:
			if((k&511)!=k)
				{ nm1=-1; }
			break;

		case BGBCC_SH_NMID_DIVSL:
		case BGBCC_SH_NMID_DIVUL:
		case BGBCC_SH_NMID_MODSL:
		case BGBCC_SH_NMID_MODUL:
			nm1=-1;
			break;

		case BGBCC_SH_NMID_MULSQ:
		case BGBCC_SH_NMID_MULUQ:
		case BGBCC_SH_NMID_DIVSQ:
		case BGBCC_SH_NMID_DIVUQ:
		case BGBCC_SH_NMID_MODSQ:
		case BGBCC_SH_NMID_MODUQ:
			nm1=-1;
			break;

		case BGBCC_SH_NMID_RSUBSL:
		case BGBCC_SH_NMID_RSUBUL:
			nm1=-1;
			break;

		default:
			break;
		}
	}
#endif

	return(nm1!=nm);
}

int BGBCC_JX2C_EmitReleaseRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	char *s0, *s1;
	ccxl_register reg1;
	ccxl_type tty;
	int creg, regfl, rcls;
	int i;

	if(sctx->is_leaftiny&1)
	{
#if 1
		for(i=0; i<sctx->maxreg_gpr_lf; i++)
		{
			if(!((sctx->regalc_save)&(1ULL<<i)))
				continue;
				
			reg1=sctx->regalc_map[i];
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
			{
				sctx->regalc_noval&=~(1ULL<<i);
			}
		}
#endif
		return(0);
	}

	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
//		sig=ctx->reg_globals[i]->sig;
		s0=ctx->reg_globals[i]->name;
		if(!strncmp(s0, "__arch_", 7))
		{
			i=-1;
		}
	}

//	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	tty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(ctx->arch_sizeof_ptr==16)
	{
		if(	(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF))
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
		
		if(		(rcls==BGBCC_SH_REGCLS_VO_GR)	||
				(rcls==BGBCC_SH_REGCLS_VO_QGR)	||
				(rcls==BGBCC_SH_REGCLS_VO_QGR2)
				)
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
	}
	
	if(!sctx->no_fpu && !sctx->fpu_soft && !sctx->fpu_gfp)
	{
		if(BGBCC_CCXL_IsRegFloatP(ctx, reg) ||
			BGBCC_CCXL_IsRegFloat16P(ctx, reg) ||
			BGBCC_CCXL_IsRegBFloat16P(ctx, reg) ||
			BGBCC_CCXL_IsRegDoubleP(ctx, reg))
		{
			return(BGBCC_JX2C_EmitReleaseFpRegister(ctx, sctx, reg));
		}
	}

//	if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) && !sctx->is_addr64)
//	if(	(rcls==BGBCC_SH_REGCLS_GR2)		||
//		(rcls==BGBCC_SH_REGCLS_VO_GR2)	||
//		(rcls==BGBCC_SH_REGCLS_VO_QGR2)	)
	if(	(rcls==BGBCC_SH_REGCLS_GR2)		||
		(rcls==BGBCC_SH_REGCLS_QGR2) ||
		(rcls==BGBCC_SH_REGCLS_VO_REF2) ||
		(rcls==BGBCC_SH_REGCLS_AR_REF2)	)
	{
		return(BGBCC_JX2C_EmitReleaseLpRegister(ctx, sctx, reg));
	}

	/* value in register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
//		if(!((sctx->regalc_save)&(1ULL<<i)))
//			continue;
			
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
//		if(reg1.val==reg.val)
		{
			if(sctx->regalc_map[i+1].val==CCXL_REGID_REG_DZ)
			{
				BGBCC_DBGBREAK
			}

			sctx->regalc_live|=1ULL<<i;
//			if(sctx->regalc_utcnt[i]>0)
//				sctx->regalc_utcnt[i]--;

			if(!(sctx->is_leaftiny&1))
			{
				if(sctx->regalc_utcnt[i]<=0)
				{
//					__debugbreak();
					BGBCC_DBGBREAK
				}

				sctx->regalc_utcnt[i]--;
			}

			regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

			if(!sctx->regalc_utcnt[i])
			{
//				BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i);

//				if((regfl&BGBCC_REGFL_ALIASPTR))
//				if((regfl&BGBCC_REGFL_ALIASPTR) ||
				if(
//					BGBCC_CCXL_IsRegVolatileP(ctx, reg) ||
					(BGBCC_CCXL_IsRegVolatileP(ctx, reg) &&
						!BGBCC_CCXL_IsRegLocalP(ctx, reg)) ||
					BGBCC_JX2C_CheckVRegLastUseInBlock(ctx, sctx, reg))
				{
					BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
				}
#if 1
				else if(regfl&BGBCC_REGFL_ALIASPTR)
				{
					if(	BGBCC_CCXL_IsRegArgP(ctx, reg) ||
						BGBCC_CCXL_IsRegLocalP(ctx, reg))
					{
//						BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 3);
						BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 1);
					}else
					{
						BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
					}
				}
#endif

#if 1
				sctx->regalc_live&=~(1ULL<<i);
				sctx->regalc_noval&=~(1ULL<<i);
#endif

//				if((sctx->regalc_dirty)&(1ULL<<i))
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
	int wasdirty;
	int creg, regfl, userq, usepq, rcls, vspfl;
	int i, j, k;

	if(sctx->is_leaftiny&1)	//no mem sync in tiny leaves
	{
		if(!(ctx->cur_func->regflags&BGBCC_REGFL_HYBLEAFTINY))
			return(0);
	}

	i=rgix;

	if(!((sctx->regalc_save)&(1ULL<<i)))
		return(0);

//	if(i<sctx->vsp_rsv)
//		return(0);

	if(i<sctx->vsp_rsv)
	{
		reg=sctx->regalc_map[i];
		regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

		if(regfl&BGBCC_REGFL_ALIASPTR)
		{
			BGBCC_DBGBREAK
		}

		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
		{
			/* Register-Reserved globals sync in epilog. */
			return(0);
		}

		if(BGBCC_CCXL_IsRegImmP(ctx, reg))
		{
			/* Register-Reserved immediates are sticky. */
			return(0);
		}
	}

	reg=sctx->regalc_map[i];
	regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

//	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	tty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(ctx->arch_sizeof_ptr==16)
	{
		if(	(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF))
		{
			rcls=BGBCC_SH_REGCLS_QGR2;
		}
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
	case BGBCC_SH_REGCLS_VO_REF2:
	case BGBCC_SH_REGCLS_AR_REF2:
		if(sctx->is_addr64)
			userq=1;
		else
			userq=0;
		break;
	default:
		userq=0;
		break;
	}


	wasdirty=0;
	if(((sctx->regalc_dirty)&(1ULL<<i)) && (sfl&1))
	{
		wasdirty=1;
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

		if(BGBCC_JX2C_EmitCheckRegIndexIsLp(ctx, sctx, i))
		{
			creg=sctx->lcachereg[i+1];
		}

#if 1
		if(
//			(BGBCC_CCXL_IsRegTempP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//			BGBCC_CCXL_IsRegLocalP(ctx, reg)) &&
			!(regfl&BGBCC_REGFL_ALIASPTR) &&
			!BGBCC_CCXL_IsRegThisIdxP(ctx, reg) &&
			!BGBCC_CCXL_IsRegGlobalP(ctx, reg) &&
			!BGBCC_CCXL_IsRegVolatileP(ctx, reg))
//		if(BGBCC_CCXL_IsRegTempP(ctx, reg) &&
//			!(regfl&BGBCC_REGFL_ALIASPTR))
//		if(1)
		{
			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				/* Register-Reserved Vars don't need write back */
				sctx->regalc_dirty&=~(1ULL<<i);
				return(0);
			}
		}
#endif

#if 1
		if(BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!(regfl&BGBCC_REGFL_ALIASPTR) &&
			(sctx->regalc_dirty&(1ULL<<i)))
//		if((BGBCC_CCXL_IsRegTempP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//			BGBCC_CCXL_IsRegLocalP(ctx, reg)) &&
//			!(regfl&BGBCC_REGFL_ALIASPTR))
		{
#if 1
			if(!(regfl&BGBCC_REGFL_TEMPLOAD))
				sctx->regalc_dirty&=~(1ULL<<i);
#endif

#if 1
			if(sctx->is_leaftiny&1)
				sctx->regalc_dirty&=~(1ULL<<i);
#endif

#if 1
			if(	(sctx->regalc_dirty&(1ULL<<i)) &&
				!BGBCC_JX2C_CheckVRegLiveRange(ctx, sctx, reg))
			{
				//discard
				sctx->regalc_dirty&=~(1ULL<<i);
//				return(0);
			}
#endif

#if 0
			j=reg.val&CCXL_REGID_BASEMASK;
			if(	(sctx->regalc_dirty&(1ULL<<i)) &&
				!(sctx->reg_stflag[j]&1) &&
				!(sfl&4))
			{
				if(regfl&BGBCC_REGFL_TEMPLOAD)
					printf("TempLoad but not Phi\n");

				sctx->regalc_dirty&=~(1ULL<<i);
			}
#endif
		}
#endif

		if(
			(	BGBCC_CCXL_IsRegTempP(ctx, reg)		||
				BGBCC_CCXL_IsRegArgP(ctx, reg)		||
				BGBCC_CCXL_IsRegLocalP(ctx, reg)	)	&&
			!(regfl&BGBCC_REGFL_ALIASPTR) &&
			(sctx->is_leaftiny&1))
		{
			sctx->regalc_dirty&=~(1ULL<<i);
		}

#if 1
		if(!(sfl&4))
		{
			vspfl=BGBCC_JX2C_GetFrameVRegVspanFlags(ctx, sctx, reg);
			if((vspfl&BGBCC_RSPFL_NONOVTRACE) &&
				!(vspfl&BGBCC_RSPFL_NONBASIC) &&
				!(regfl&BGBCC_REGFL_ALIASPTR))
			{
				//discard
				sctx->regalc_dirty&=~(1ULL<<i);
	//			return(0);
			}
			
			if((vspfl&BGBCC_RSPFL_ISCALLARG) &&
				BGBCC_JX2C_CheckRegisterIndexArgIdxP(
					ctx, sctx, i, (vspfl>>16)&63))
			{
				/* don't store call arguments... */
				sctx->regalc_dirty&=~(1ULL<<i);
			}
		}
#endif

#if 0
		if(!sctx->is_addr64)
		{
			if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
				BGBCC_CCXL_IsRegVariantP(ctx, reg))
//					creg=bgbcc_jx2_lcachereg[i+1];
					creg=sctx->lcachereg[i+1];
		}else
		{
			if(BGBCC_CCXL_IsRegVec128P(ctx, reg))
//				creg=bgbcc_jx2_lcachereg[i+1];
				creg=sctx->lcachereg[i+1];
		}
#endif

//		if(!rchk)
//		if(!rchk && ((sctx->regalc_dirty)&(1ULL<<i)))
		if(!rchk && ((sctx->regalc_dirty)&(1ULL<<i)) &&
//			!(sctx->is_leaftiny&1) &&
			(
				!(sctx->is_leaftiny&1) ||
				(regfl&BGBCC_REGFL_ALIASPTR) ||
				BGBCC_CCXL_IsRegThisIdxP(ctx, reg) ||
				BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
				BGBCC_CCXL_IsRegVolatileP(ctx, reg)))
		{
			rchk=1;
			BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
			rchk=0;
		}

		sctx->regalc_dirty&=~(1ULL<<i);
	}

	if(sfl&2)
	{
		if(	BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
			BGBCC_CCXL_IsRegThisIdxP(ctx, reg) ||
			BGBCC_CCXL_IsRegVolatileP(ctx, reg) ||
			(regfl&BGBCC_REGFL_ALIASPTR))
		{
			sctx->regalc_map[i].val=-1;
			sctx->regalc_utcnt[i]=0;
			sctx->regalc_ltcnt[i]=0;
			sctx->regalc_dirty&=~(1ULL<<i);
			sctx->regalc_live&=~(1ULL<<i);
			sctx->regalc_noval&=~(1ULL<<i);
			sctx->regalc_pair&=~(1ULL<<i);

			if(sctx->regalc_map[i+1].val==CCXL_REGID_REG_DZ)
			{
				sctx->regalc_dirty&=~(3ULL<<i);
				sctx->regalc_live&=~(3ULL<<i);
				sctx->regalc_noval&=~(3ULL<<i);
				sctx->regalc_pair&=~(3ULL<<i);
				sctx->regalc_map[i+1].val=CCXL_REGID_REG_Z;
			}
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
		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 3);

#if 0
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_live&=~(1ULL<<i);
		sctx->regalc_noval&=~(1ULL<<i);
		sctx->regalc_pair&=~(1ULL<<i);
#endif
	}

	return(0);
}

int BGBCC_JX2C_EmitSyncDirtyRegisters(
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
		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 1);

#if 0
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_live&=~(1ULL<<i);
		sctx->regalc_noval&=~(1ULL<<i);
		sctx->regalc_pair&=~(1ULL<<i);
#endif
	}

	return(0);
}

int BGBCC_JX2C_EmitSyncLeafRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	ccxl_register reg;
	int i, isv, regfl;

//	if(sctx->is_leaftiny&1)
//	{
//		return(0);
//	}

	BGBCC_JX2C_EmitSyncFpRegisters(ctx, sctx);

	/* value in register? */
//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
//	for(i=sctx->maxreg_gpr; i<sctx->maxreg_gpr_lf; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		isv=0;
		reg=sctx->regalc_map[i];
		regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

		if(	BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
			BGBCC_CCXL_IsRegThisIdxP(ctx, reg) ||
			BGBCC_CCXL_IsRegVolatileP(ctx, reg) ||
			(regfl&BGBCC_REGFL_ALIASPTR))
		{
			isv=1;
		}
		
		if(i<sctx->vsp_rsv)
			isv=0;

		if(!BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i) && !isv)
			continue;

		if(!(sctx->is_leaftiny&1))
		{
			if((sctx->regalc_live&(1ULL<<i)) && !sctx->is_simpass)
				{ BGBCC_DBGBREAK }
		}

		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 3);

#if 0
		if(!(sctx->is_leaftiny&1))
		{
			sctx->regalc_utcnt[i]=0;
			sctx->regalc_live&=~(1ULL<<i);
			sctx->regalc_noval&=~(1ULL<<i);
			sctx->regalc_pair&=~(1ULL<<i);
		}
#endif
	}

	return(0);
}


int BGBCC_JX2C_EmitSyncAliasRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	ccxl_register reg;
	int i, isv, regfl;

//	BGBCC_JX2C_EmitSyncFpRegisters(ctx, sctx);

	/* value in register? */
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		isv=0;
		reg=sctx->regalc_map[i];
		regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

		if(
//			BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
//			BGBCC_CCXL_IsRegThisIdxP(ctx, reg) ||
			BGBCC_CCXL_IsRegVolatileP(ctx, reg) ||
			(regfl&BGBCC_REGFL_ALIASPTR))
		{
			isv=1;
		}
		
		if(	!BGBCC_CCXL_IsRegArgP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				continue;
		
		if(i<sctx->vsp_rsv)
			isv=0;

		if(!isv)
			continue;

		if(!(sctx->is_leaftiny&1))
		{
			if((sctx->regalc_live&(1ULL<<i)) && !sctx->is_simpass)
				{ BGBCC_DBGBREAK }
		}

		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 3);
	}

	return(0);
}

//extern byte bgbcc_jx2_lminreg;
//extern byte bgbcc_jx2_lmaxreg;
//extern byte bgbcc_jx2_lmaxreg_lf;

int BGBCC_JX2C_EmitLabelFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	ccxl_register reg;
	ccxl_type tty;

	int i;

	if((sctx->is_leaftiny&3)==1)
	{
#if 1
		if(ctx->cur_func->regflags&BGBCC_REGFL_HYBLEAFTINY)
		{
			BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);

			for(i=0; i<sctx->maxreg_gpr_lf; i++)
			{
				if(!((sctx->regalc_save)&(1ULL<<i)))
					continue;

				reg=sctx->regalc_map[i];

				if(		BGBCC_CCXL_IsRegLocalP(ctx, reg)	||
						BGBCC_CCXL_IsRegArgP(ctx, reg)		||
						BGBCC_CCXL_IsRegTempP(ctx, reg)		)
							continue;
				if(i<sctx->vsp_rsv)
					continue;

				sctx->regalc_map[i].val=-1;
				sctx->regalc_utcnt[i]=0;
				sctx->regalc_ltcnt[i]=0;
				sctx->regalc_dirty&=~(1ULL<<i);
				sctx->regalc_live&=~(1ULL<<i);
				sctx->regalc_noval&=~(1ULL<<i);
				sctx->regalc_pair&=~(1ULL<<i);
			}
		}
#endif

		return(0);
	}

	BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);

	sctx->jcachereg=bgbcc_jx2_jcachereg;
	sctx->qcachereg=bgbcc_jx2_qcachereg;
	sctx->pcachereg=bgbcc_jx2_pcachereg;
	sctx->lcachereg=bgbcc_jx2_lcachereg;
	sctx->maxreg_gpr=bgbcc_jx2_maxreg;
	sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_lf;

//	sctx->lmaxreg_gpr=bgbcc_jx2_lmaxreg;
//	sctx->lmaxreg_gpr_lf=bgbcc_jx2_lmaxreg_lf;

	sctx->lmaxreg_gpr=bgbcc_jx2_maxreg;
	sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_lf;


	sctx->shufreg0=bgbcc_jx2_shufreg0_egpr;
	sctx->shufreg1=bgbcc_jx2_shufreg1_egpr;
	sctx->shufreg2=bgbcc_jx2_shufreg2_egpr;
	sctx->shufreg3=bgbcc_jx2_shufreg3_egpr;
	
	sctx->regs_excl			=0xFFFFFFFFFFFF8000ULL;
	sctx->regs_excl_tiny	=0xFFFFFFFFFFFFFF00ULL;

	if(sctx->has_bjx1egpr)
	{
#if 0
		sctx->jcachereg=bgbcc_jx2_jcachereg_egt;
		sctx->qcachereg=bgbcc_jx2_qcachereg_egt;
		sctx->pcachereg=bgbcc_jx2_pcachereg_egt;
		sctx->lcachereg=bgbcc_jx2_lcachereg_egt;

//		sctx->jcachereg=bgbcc_jx2_jcachereg_epr;
//		sctx->qcachereg=bgbcc_jx2_qcachereg_epr;
//		sctx->pcachereg=bgbcc_jx2_pcachereg_epr;
//		sctx->lcachereg=bgbcc_jx2_lcachereg_epr;

		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egt;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_egt_lf;
		sctx->lmaxreg_gpr=bgbcc_jx2_maxreg_egt;
		sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_egt_lf;
#endif

#if 1
		sctx->jcachereg=bgbcc_jx2_jcachereg_egpr;
		sctx->qcachereg=bgbcc_jx2_qcachereg_egpr;
		sctx->pcachereg=bgbcc_jx2_pcachereg_egpr;
		sctx->lcachereg=bgbcc_jx2_lcachereg_egpr;
		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;
		sctx->lmaxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;
#endif

		sctx->regs_excl			=0xFFFFFFFFFF008000ULL;
		sctx->regs_excl_tiny	=0xFFFFFFFFFF00FF00ULL;
	}

	if(sctx->has_bjx1egpr && sctx->use_egpr)
	{
		sctx->jcachereg=bgbcc_jx2_jcachereg_egpr;
		sctx->qcachereg=bgbcc_jx2_qcachereg_egpr;
		sctx->pcachereg=bgbcc_jx2_pcachereg_egpr;
		sctx->lcachereg=bgbcc_jx2_lcachereg_egpr;
		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;
		sctx->lmaxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;

		sctx->regs_excl			=0xFFFFFFFF00008000ULL;
		sctx->regs_excl_tiny	=0xFFFFFFFFFF00FF00ULL;
	}
	
	if(sctx->is_leaftiny&1)
	{
//		sctx->jcachereg=bgbcc_jx2_jcachereg_ltiny;
//		sctx->qcachereg=bgbcc_jx2_qcachereg_ltiny;
//		sctx->pcachereg=bgbcc_jx2_pcachereg_ltiny;
//		sctx->maxreg_gpr=bgbcc_jx2_maxreg_ltiny;
//		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_ltiny;

		sctx->jcachereg=bgbcc_jx2_jcachereg_egpr;
		sctx->qcachereg=bgbcc_jx2_qcachereg_egpr;
		sctx->pcachereg=bgbcc_jx2_pcachereg_egpr;
		sctx->lcachereg=bgbcc_jx2_lcachereg_egpr;
		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;
		sctx->lmaxreg_gpr=bgbcc_jx2_maxreg_egpr;
		sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_egpr_lf;

		sctx->regs_excl			=0xFFFFFFFF00008000ULL;
		sctx->regs_excl_tiny	=0xFFFFFFFFFF00FF00ULL;
	}

#if 1
//	if(sctx->has_xgpr&2)
//	if((sctx->has_xgpr&2) ||
//		((sctx->has_xgpr&1) && (sctx->use_egpr&2)))
	if(sctx->has_xgpr&1)
	{
		sctx->jcachereg=bgbcc_jx2_jcachereg_xgpr;
		sctx->qcachereg=bgbcc_jx2_qcachereg_xgpr;
		sctx->pcachereg=bgbcc_jx2_pcachereg_xgpr;
		sctx->lcachereg=bgbcc_jx2_lcachereg_xgpr;

		sctx->maxreg_gpr=bgbcc_jx2_maxreg_xgpr;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_xgpr_lf;
		sctx->lmaxreg_gpr=bgbcc_jx2_maxreg_xgpr;
		sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_xgpr_lf;

		sctx->shufreg0=bgbcc_jx2_shufreg0_xgpr;
		sctx->shufreg1=bgbcc_jx2_shufreg1_xgpr;
		sctx->shufreg2=bgbcc_jx2_shufreg2_xgpr;
		sctx->shufreg3=bgbcc_jx2_shufreg3_xgpr;

		sctx->regs_excl			=0xFFFFFFFF00008000ULL;
		sctx->regs_excl_tiny	=0xFFFFFFFFFF00FF00ULL;

		if(sctx->has_xgpr&6)
		{
			sctx->regs_excl			=0xFF00FF0000008000ULL;
			sctx->regs_excl_tiny	=0xFF00FF00FF00FF00ULL;
		}

		if(sctx->use_egpr&2)
		{
			sctx->regs_excl			=0x0000000000008000ULL;
			sctx->regs_excl_tiny	=0xFF00FF00FF00FF00ULL;
		}

	}
#endif

#if 1
	if(sctx->emit_riscv&3)
	{
		sctx->jcachereg=bgbcc_jx2_jcachereg_xg2rv;
		sctx->qcachereg=bgbcc_jx2_qcachereg_xg2rv;
		sctx->pcachereg=bgbcc_jx2_pcachereg_xg2rv;
		sctx->lcachereg=bgbcc_jx2_lcachereg_xg2rv;

		sctx->maxreg_gpr=bgbcc_jx2_maxreg_xg2rv;
		sctx->maxreg_gpr_lf=bgbcc_jx2_maxreg_xg2rv_lf;
		sctx->lmaxreg_gpr=bgbcc_jx2_maxreg_xg2rv;
		sctx->lmaxreg_gpr_lf=bgbcc_jx2_maxreg_xg2rv_lf;
	}
#endif

	if(sctx->is_addr64)
	{
		if(!sctx->is_addr_x32)
		{
			sctx->pcachereg=sctx->qcachereg;
		}
	}

//	for(i=0; i<5; i++)
//	for(i=0; i<bgbcc_jx2_maxreg; i++)
//	for(i=0; i<sctx->maxreg_gpr; i++)
	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
//		if(!((sctx->regalc_save)&(1ULL<<i)))
//			continue;

//		if(i<sctx->vsp_rsv)
//			continue;

		sctx->regalc_map[i].val=-1;
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_dirty&=~(1ULL<<i);
		sctx->regalc_live&=~(1ULL<<i);
		sctx->regalc_noval&=~(1ULL<<i);
		sctx->regalc_pair&=~(1ULL<<i);
	}

	for(i=0; i<ctx->cur_func->n_regs; i++)
		{ sctx->reg_stflag[i]=0; }
	for(i=0; i<ctx->cur_func->n_args; i++)
		{ sctx->arg_stflag[i]=0; }
	for(i=0; i<ctx->cur_func->n_locals; i++)
		{ sctx->lcl_stflag[i]=0; }
	
	return(0);
}

int BGBCC_JX2C_EmitScratchFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	ccxl_register reg;
	ccxl_type tty;

	int i;

	if((sctx->is_leaftiny&3)==1)
	{
		return(0);
	}

	BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);

	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
//		if(!((sctx->regalc_save)&(1ULL<<i)))
//			continue;

//		if(i<sctx->vsp_rsv)
//			continue;

		if(BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i)<=0)
			continue;

		sctx->regalc_map[i].val=-1;
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_dirty&=~(1ULL<<i);
		sctx->regalc_live&=~(1ULL<<i);
		sctx->regalc_noval&=~(1ULL<<i);
		sctx->regalc_pair&=~(1ULL<<i);
	}
	
	return(0);
}

int BGBCC_JX2C_EmitEpilogFlushRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	ccxl_register reg;
	ccxl_type tty;

	int i;

	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		sctx->regalc_map[i].val=-1;
		sctx->regalc_utcnt[i]=0;
		sctx->regalc_ltcnt[i]=0;
		sctx->regalc_dirty&=~(1ULL<<i);
		sctx->regalc_live&=~(1ULL<<i);
		sctx->regalc_noval&=~(1ULL<<i);
		sctx->regalc_pair&=~(1ULL<<i);
	}
	
	return(0);
}

int BGBCC_JX2C_EmitScratchSyncRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	ccxl_register reg;
	ccxl_type tty;
	int i;

	if(sctx->sreg_live)
		{ BGBCC_DBGBREAK }

	if((sctx->is_leaftiny&3)==1)
	{
		return(0);
	}

//	BGBCC_JX2C_EmitLabelFlushFpRegisters(ctx, sctx);

	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		if(BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i)<=0)
			continue;
		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 3);
	}

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

int BGBCC_JX2C_EmitTempPhiRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	int i, j, k;

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
//		rcls=ctx->cur_func->regs[j]->regcls;
		sctx->reg_stflag[j]|=1;
	}

	return(0);
}

int BGBCC_JX2C_EmitTempLoadReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	int i, j, k;

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
//		rcls=ctx->cur_func->regs[j]->regcls;
		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_TEMPLOAD;
//		sctx->reg_stflag[j]|=1;
	}

	return(0);
}

int BGBCC_JX2C_GetVRegRegClass(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	ccxl_type tty;
	int rcls;

	tty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);
	return(rcls);
}

int BGBCC_JX2C_GetVRegRegForIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg,
	int idx)
{
	ccxl_type tty;
	int rcls, creg, userq;

	tty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	userq=0;
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
		userq=1;
		break;
	default:
		userq=0;
		break;
	}

	if(userq)
		creg=sctx->qcachereg[idx];
	else
		creg=sctx->jcachereg[idx];

	return(creg);
}

int BGBCC_JX2C_EmitSyncProlog(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	char *s0, *s1;
	ccxl_register reg1;
	ccxl_type tty;
	int creg, regfl, rcls;
	u64 cregmask;
	int i;

	sctx->regalc_gbldirty=0;
	cregmask=0;

	if(sctx->is_leaftiny&1)
	{
		for(i=0; i<sctx->maxreg_gpr_lf; i++)
		{
			if(!((sctx->regalc_save)&(1ULL<<i)))
				continue;
				
			reg1=sctx->regalc_map[i];
//			creg=sctx->qcachereg[i];
			creg=BGBCC_JX2C_GetVRegRegForIndex(ctx, sctx, reg1, i);
			
			if(	BGBCC_CCXL_IsRegGlobalP(ctx, reg1) ||
				BGBCC_CCXL_IsRegImmP(ctx, reg1))
			{
				BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg1, creg);

				if(BGBCC_JX2_EmitCheckRegExtGPR(sctx, creg))
				{
					cregmask|=1ULL<<(creg&63);
				}
			}
		}
	}

	for(i=0; i<sctx->vsp_rsv; i++)
	{
		reg1=sctx->vspan[i]->reg;
//		creg=sctx->qcachereg[i];
		creg=BGBCC_JX2C_GetVRegRegForIndex(ctx, sctx, reg1, i);

#if 1
		if(BGBCC_JX2_EmitCheckRegExtGPR(sctx, creg))
		{
			if(cregmask&(1ULL<<(creg&63)))
				continue;
		}
#endif

		if(	BGBCC_CCXL_IsRegGlobalP(ctx, reg1) ||
			BGBCC_CCXL_IsRegImmP(ctx, reg1))
		{
			BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg1, creg);
		}
	}

	return(0);
}

int BGBCC_JX2C_EmitSyncEpilog(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	char *s0, *s1;
	ccxl_register reg1;
	ccxl_type tty;
	int creg, regfl, rcls;
	int i;

	if(sctx->is_leaftiny&1)
	{
		for(i=0; i<sctx->maxreg_gpr_lf; i++)
		{
			if(!((sctx->regalc_save)&(1ULL<<i)))
				continue;
			if(!((sctx->regalc_dirty)&(1ULL<<i)))
				continue;

			reg1=sctx->regalc_map[i];
//			creg=sctx->qcachereg[i];
			creg=BGBCC_JX2C_GetVRegRegForIndex(ctx, sctx, reg1, i);

			if(	BGBCC_CCXL_IsRegGlobalP(ctx, reg1))
			{
				BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, reg1, creg);
			}
		}
	}

	for(i=0; i<sctx->vsp_rsv; i++)
	{
//		if(!((sctx->regalc_dirty)&(1ULL<<i)))
		if(!((sctx->regalc_gbldirty)&(1ULL<<i)))
			continue;

		reg1=sctx->vspan[i]->reg;
//		creg=sctx->qcachereg[i];
		creg=BGBCC_JX2C_GetVRegRegForIndex(ctx, sctx, reg1, i);

		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg1))
		{
			BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, reg1, creg);
		}
	}

	return(0);
}
