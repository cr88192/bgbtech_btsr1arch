//const byte bgbcc_jx2_lcachereg[6]={
//	BGBCC_SH_REG_LR13,
//	BGBCC_SH_REG_LR11,
//	BGBCC_SH_REG_LR10,
//	BGBCC_SH_REG_LR9,
//	BGBCC_SH_REG_LR8, 255 };

const byte bgbcc_jx2_lcachereg[8]={
	BGBCC_SH_REG_LR14, BGBCC_SH_REG_LR13,
	BGBCC_SH_REG_LR12, BGBCC_SH_REG_LR11,
	BGBCC_SH_REG_LR10, BGBCC_SH_REG_LR9,
	BGBCC_SH_REG_LR8, 255 };
const byte bgbcc_jx2_lminreg=0;
const byte bgbcc_jx2_lmaxreg=6;

int BGBCC_JX2C_EmitTryGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg;
	int pr0, bi;
	int creg;
	int i;

	zreg.val=CCXL_REGID_REG_DZ;

	/* value already in a register? */
//	for(i=1; i<4; i++)
	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
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
			return(bgbcc_jx2_lcachereg[i+1]);
		}
	}

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

	if(((fl&3)==1) || ((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv)))
	{
		bi=-1;

		if((pr0>=0) && (pr0<sctx->vsp_rsv))
			bi=pr0;

#if 1
		if(bi>=0)
		{
			i=bi;

			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i+0);
			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i+1);
			BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=3<<i;
			sctx->regalc_ltcnt[i+0]=0;
			sctx->regalc_ltcnt[i+1]=0;
			sctx->regalc_map[i+0]=reg;
			sctx->regalc_map[i+1]=zreg;
//			sctx->regalc_utcnt[i]=1;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_live|=3<<i;
			if(fl&1)
				{ sctx->regalc_dirty|=1<<i; }
			creg=bgbcc_jx2_lcachereg[i+1];
			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&15);
			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&15);
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
int BGBCC_JX2C_EmitGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg;
	int creg, excl, pr0;
	int i;

	creg=BGBCC_JX2C_EmitTryGetLpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

//	for(i=1; i<4; i++)
	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

//	zreg.val=CCXL_REGTY_TEMP|CCXL_REGID_REGMASK;
	zreg.val=CCXL_REGID_REG_DZ;

	/* Check for registers not holding a live value. */
//	for(i=2; i<5; i++)
	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	{
		if(excl&(3<<i))
			continue;

		if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

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
			creg=bgbcc_jx2_lcachereg[i+1];
			if(fl&2)
				{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
	}

	/* Check for unallocated registers. */
//	for(i=1; i<4; i++)
	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	{
		if(excl&(3<<i))
			continue;

		if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

		if((sctx->regalc_save)&(1<<i))
			continue;
		if((sctx->regalc_live)&(3<<i))
			continue;

//		if(	((sctx->regalc_live)&(3<<i)) ||
//			((sctx->regalc_dirty)&(3<<i)))
//				continue;

		if((sctx->regalc_dirty)&(1<<i))
			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i+0);
		if((sctx->regalc_dirty)&(2<<i))
			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i+1);

		sctx->regalc_ltcnt[i+0]=0;
		sctx->regalc_ltcnt[i+1]=0;
		sctx->regalc_map[i+0]=reg;
		sctx->regalc_map[i+1]=zreg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_save|=3<<i;
		sctx->regalc_live|=3<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;

		creg=bgbcc_jx2_lcachereg[i+1];		
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&15);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&15);
		if(fl&2)
			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitReleaseLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	int creg;
	int i;

	/* value in register? */
//	for(i=1; i<4; i++)
	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
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
					creg=bgbcc_jx2_lcachereg[i+1];
					if(!rchk)
					{
						rchk++;
						BGBCC_JX2C_EmitStoreFrameVRegReg(
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

	BGBCC_DBGBREAK

	return(0);
}

int BGBCC_JX2C_StompLpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int rgidx)
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

int BGBCC_JX2C_ConvLpRegisterNarrowing(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid,
	ccxl_register sreg,
	ccxl_register dreg)
{
	int csreg, cdreg, nm1;

	nm1=nmid;
//		nm1=BGBCC_SH_NMID_MOV;

#if 1
	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);

//	if((csreg&15)==0xD)
//		{ __debugbreak(); }

	cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	
//	if((csreg&15)==0xD)
//		{ __debugbreak(); }
	
//	if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
//		csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
//	if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR) &&
		(cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
//		csreg=BGBCC_SH_REG_R0+(csreg&15);

		BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, cdreg);

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}else if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
//		nm1=BGBCC_SH_NMID_MOV;
		BGBCC_JX2_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
//		nm1=BGBCC_SH_NMID_MOV;
		BGBCC_JX2_EmitOpRegReg(sctx, nm1, csreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}else
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

		if(nm1!=BGBCC_SH_NMID_MOV)
			BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R4);

		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);
		return(1);
	}
#endif

	return(0);
}


int BGBCC_JX2C_ConvLpRegisterWiden(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_type stype,
	ccxl_register sreg,
	ccxl_register dreg)
{
	int csreg, cdreg, nm1;

	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

	if(	(csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR) &&
		(cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			csreg, cdreg&15);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
		{
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				(cdreg&15)+1, 0);
		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				csreg, (cdreg&15)+1);
			BGBCC_JX2C_EmitLeaSarRegImm(ctx, sctx,
				(cdreg&15)+1, 31);
		}

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);

		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			csreg, BGBCC_SH_REG_R4);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
		{
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R5, 0);
		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				csreg, BGBCC_SH_REG_R5);
			BGBCC_JX2C_EmitLeaSarRegImm(ctx, sctx,
				BGBCC_SH_REG_R5, 31);
		}

		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);

		BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);

		return(1);
	}

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
	if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, sreg);


	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
	BGBCC_JX2C_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);

//	if(	(st==CCXL_TY_UB) || (st==CCXL_TY_US) ||
//		(st==CCXL_TY_UI) || (st==CCXL_TY_UNL))
	if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
	{
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R5, 0);
	}else
	{
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_R5);
		BGBCC_JX2C_EmitLeaSarRegImm(ctx, sctx,
			BGBCC_SH_REG_R5, 31);
	}

	BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);

	return(1);
}
