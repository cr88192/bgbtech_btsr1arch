//const byte bgbcc_bsr_lcachereg[6]={
//	BGBCC_SH_REG_LR13,
//	BGBCC_SH_REG_LR11,
//	BGBCC_SH_REG_LR10,
//	BGBCC_SH_REG_LR9,
//	BGBCC_SH_REG_LR8, 255 };

const byte bgbcc_bsr_lcachereg[8]={
	BGBCC_SH_REG_LR14, BGBCC_SH_REG_LR13,
	BGBCC_SH_REG_LR12, BGBCC_SH_REG_LR11,
	BGBCC_SH_REG_LR10, BGBCC_SH_REG_LR9,
	BGBCC_SH_REG_LR8, 255 };
const byte bgbcc_bsr_lminreg=0;
const byte bgbcc_bsr_lmaxreg=6;

int BGBCC_BSRC_EmitTryGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg, int fl)
{
	int creg;
	int i;

	/* value already in a register? */
//	for(i=1; i<4; i++)
	for(i=bgbcc_bsr_lminreg; i<bgbcc_bsr_lmaxreg; i++)
	{
//		if(!((sctx->regalc_save)&(3<<i)))
//			continue;
		if(((sctx->regalc_save)&(3<<i))!=(3<<i))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=3<<i;
			if(fl&1)
				sctx->regalc_dirty|=1<<i;

			if(sctx->regalc_ltcnt[i]>0)
				sctx->regalc_ltcnt[i]--;
			sctx->regalc_utcnt[i]++;
			return(bgbcc_bsr_lcachereg[i+1]);
		}
	}

	return(BGBCC_SH_REG_ZZR);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_BSRC_EmitGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg;
	int creg, excl;
	int i;

	creg=BGBCC_BSRC_EmitTryGetLpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

//	for(i=1; i<4; i++)
	for(i=bgbcc_bsr_lminreg; i<bgbcc_bsr_lmaxreg; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

//	zreg.val=CCXL_REGTY_TEMP|CCXL_REGID_REGMASK;
	zreg.val=CCXL_REGID_REG_DZ;

	/* Check for registers not holding a live value. */
//	for(i=2; i<5; i++)
	for(i=bgbcc_bsr_lminreg; i<bgbcc_bsr_lmaxreg; i++)
	{
		if(excl&(3<<i))
			continue;

//		if(!((sctx->regalc_save)&(3<<i)))
//			continue;
		if(((sctx->regalc_save)&(3<<i))!=(3<<i))
			continue;
		if(!((sctx->regalc_live)&(3<<i)))
//		if(	!((sctx->regalc_live)&(3<<i)) &&
//			!((sctx->regalc_dirty)&(3<<i)))
		{
			sctx->regalc_ltcnt[i+0]=0;
			sctx->regalc_ltcnt[i+1]=0;
			sctx->regalc_map[i+0]=reg;
			sctx->regalc_map[i+1]=zreg;
			sctx->regalc_utcnt[i]=1;
			sctx->regalc_live|=3<<i;
			if(fl&1)
				sctx->regalc_dirty|=1<<i;
			creg=bgbcc_bsr_lcachereg[i+1];
			if(fl&2)
				{ BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
	}

	/* Check for unallocated registers. */
//	for(i=1; i<4; i++)
	for(i=bgbcc_bsr_lminreg; i<bgbcc_bsr_lmaxreg; i++)
	{
		if(excl&(3<<i))
			continue;

		if((sctx->regalc_save)&(1<<i))
			continue;
		if((sctx->regalc_live)&(3<<i))
			continue;

//		if(	((sctx->regalc_live)&(3<<i)) ||
//			((sctx->regalc_dirty)&(3<<i)))
//				continue;

		if((sctx->regalc_dirty)&(1<<i))
			BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i+0);
		if((sctx->regalc_dirty)&(2<<i))
			BGBCC_BSRC_EmitSyncRegisterIndex(ctx, sctx, i+1);

		sctx->regalc_ltcnt[i+0]=0;
		sctx->regalc_ltcnt[i+1]=0;
		sctx->regalc_map[i+0]=reg;
		sctx->regalc_map[i+1]=zreg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_save|=3<<i;
		sctx->regalc_live|=3<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;

		creg=bgbcc_bsr_lcachereg[i+1];		
		BGBCC_BSRC_EmitSaveFrameReg(ctx, sctx, (creg+0)&15);
		BGBCC_BSRC_EmitSaveFrameReg(ctx, sctx, (creg+1)&15);
		if(fl&2)
			{ BGBCC_BSRC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSRC_EmitReleaseLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	int creg;
	int i;

	/* value in register? */
//	for(i=1; i<4; i++)
	for(i=bgbcc_bsr_lminreg; i<bgbcc_bsr_lmaxreg; i++)
	{
		if(!((sctx->regalc_save)&(3<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=3<<i;
			if(sctx->regalc_utcnt[i]>0)
				sctx->regalc_utcnt[i]--;

			if(!sctx->regalc_utcnt[i])
			{
				if((sctx->regalc_dirty)&(1<<i))
				{
					creg=bgbcc_bsr_lcachereg[i+1];
					if(!rchk)
					{
						rchk++;
						BGBCC_BSRC_EmitStoreFrameVRegReg(
							ctx, sctx, reg, creg);
						rchk--;
					}
					sctx->regalc_dirty&=~(1<<i);
				}

				sctx->regalc_live&=~(3<<i);
			}

			return(1);
		}
	}

	return(0);
}

int BGBCC_BSRC_StompLpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_BSR_Context *sctx, int rgidx)
{
	ccxl_register reg, zreg;
	int creg;
	int i;

	i=rgidx;

	if(!((sctx->regalc_save)&(1<<i)))
		return(0);

	reg=sctx->regalc_map[i];
	zreg.val=CCXL_REGID_REG_Z;

	if(reg.val==CCXL_REGID_REG_DZ)
	{
		sctx->regalc_map[i-1]=zreg;
		sctx->regalc_map[i+0]=zreg;
		return(1);
	}

	if(!sctx->is_addr64)
	{
		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg) ||
			BGBCC_CCXL_IsRegVariantP(ctx, reg))
		{
			sctx->regalc_map[i+0]=zreg;
			sctx->regalc_map[i+1]=zreg;
			return(1);
		}
	}

	sctx->regalc_map[i]=zreg;
	return(1);
}
