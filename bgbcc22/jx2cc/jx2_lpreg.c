//const byte bgbcc_jx2_lcachereg[6]={
//	BGBCC_SH_REG_LR13,
//	BGBCC_SH_REG_LR11,
//	BGBCC_SH_REG_LR10,
//	BGBCC_SH_REG_LR9,
//	BGBCC_SH_REG_LR8, 255 };

//const byte bgbcc_jx2_lcachereg[8]={
//	BGBCC_SH_REG_LR14, BGBCC_SH_REG_LR13,
//	BGBCC_SH_REG_LR12, BGBCC_SH_REG_LR11,
//	BGBCC_SH_REG_LR10, BGBCC_SH_REG_LR9,
//	BGBCC_SH_REG_LR8, 255 };

const byte bgbcc_jx2_lcachereg[32]={
	BGBCC_SH_REG_LR14, BGBCC_SH_REG_LR13,
	BGBCC_SH_REG_LR12, BGBCC_SH_REG_LR11,
	BGBCC_SH_REG_LR10, BGBCC_SH_REG_LR9,
	BGBCC_SH_REG_LR8,
	BGBCC_SH_REG_LR31, BGBCC_SH_REG_LR30,
	BGBCC_SH_REG_LR29, BGBCC_SH_REG_LR28,
	BGBCC_SH_REG_LR27, BGBCC_SH_REG_LR26,
	BGBCC_SH_REG_LR25, BGBCC_SH_REG_LR24, 
	BGBCC_SH_REG_LR7,  BGBCC_SH_REG_LR6,
	BGBCC_SH_REG_LR5,  BGBCC_SH_REG_LR4,
	BGBCC_SH_REG_LR3,  BGBCC_SH_REG_LR2,
	BGBCC_SH_REG_LR23, BGBCC_SH_REG_LR22,
	BGBCC_SH_REG_LR21, BGBCC_SH_REG_LR20,
	BGBCC_SH_REG_LR19, BGBCC_SH_REG_LR18,
	BGBCC_SH_REG_ZZR };
byte bgbcc_jx2_lminreg=0;
// const byte bgbcc_jx2_lmaxreg=6;
byte bgbcc_jx2_lmaxreg=15;
byte bgbcc_jx2_lmaxreg_lf=27;


int BGBCC_JX2C_CheckLpRegisterNeedAlign(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	ccxl_type tty;
	int tyb;

	return(1);

#if 0
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	tyb=BGBCC_CCXL_GetTypeBaseType(ctx, tty);

	if(tyb==CCXL_TY_M128)
		return(1);

	if(	(tyb==CCXL_TY_VEC3F)	||
		(tyb==CCXL_TY_VEC4F)	||
		(tyb==CCXL_TY_QUATF)	||
		(tyb==CCXL_TY_VEC2D)	||
		(tyb==CCXL_TY_DCOMPLEX)	||
		(tyb==CCXL_TY_VEC4SI)	||
		(tyb==CCXL_TY_VEC4UI)	)
//		(tyb==CCXL_TY_VEC4UI)	||
//		(tyb==CCXL_TY_I128)		||
//		(tyb==CCXL_TY_UI128)	||
//		(tyb==CCXL_TY_F128)		)
			return(1);

	return(0);
#endif
}

int BGBCC_JX2C_EmitTryGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg;
	int pr0, bi;
	int creg, algn, maxreg;
	int i;

	zreg.val=CCXL_REGID_REG_DZ;
	algn=BGBCC_JX2C_CheckLpRegisterNeedAlign(ctx, sctx, reg);

	maxreg=sctx->lmaxreg_gpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->lmaxreg_gpr_lf;

	/* value already in a register? */
//	for(i=1; i<4; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
//		if(!((sctx->regalc_save)&(3<<i)))
//			continue;
		if(((sctx->regalc_save)&(3<<i))!=(3<<i))
			continue;
		if(algn && (bgbcc_jx2_lcachereg[i+1]&1))
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
			BGBCC_DBGBREAK
		
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
	ccxl_register reg1;
	int creg, excl, pr0, pr1, algn, bi, nsv, maxreg;
	int i;

	creg=BGBCC_JX2C_EmitTryGetLpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	algn=BGBCC_JX2C_CheckLpRegisterNeedAlign(ctx, sctx, reg);

	excl=0;
	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

	maxreg=sctx->lmaxreg_gpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->lmaxreg_gpr_lf;

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

//	for(i=1; i<4; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<maxreg; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

//	zreg.val=CCXL_REGTY_TEMP|CCXL_REGID_REGMASK;
	zreg.val=CCXL_REGID_REG_DZ;

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	bi=-1; nsv=0;

	/* Check for registers not holding a live value. */
//	for(i=2; i<5; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<maxreg; i++)
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

		if(algn && (bgbcc_jx2_lcachereg[i+1]&1))
			continue;

//		if(!((sctx->regalc_live)&(3<<i)))
		if(	!((sctx->regalc_live)&(3<<i)) &&
			!((sctx->regalc_dirty)&(3<<i)))
		{
			reg1=sctx->regalc_map[i];
			pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

			if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i; break; }
			else
			{
				if((bi<0) ||
					((sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]) &&
					(pr0<=pr1)))
				{
					bi=i;
				}
			}

#if 0
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
#endif
		}
	}
	
	if(bi<0)
	{
		for(i=0; i<maxreg; i++)
		{
			if(excl&(3<<i))
				continue;

			if(algn && (bgbcc_jx2_lcachereg[i+1]&1))
				continue;

			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

			if(!((sctx->regalc_save)&(3<<i)))
				{ continue; }
			if(!((sctx->regalc_live)&(3<<i)))
			{
				reg1=sctx->regalc_map[i];
				if((bi<0) || (sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
					bi=i;
				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i; break; }
			}
		}
	}

	/* Check if we are better off allocating another register */
	if(bi>=0)
	{
		reg1=sctx->regalc_map[bi];
//		if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
		if((nsv>=1) && (sctx->regalc_ltcnt[bi]<=2) &&
			sctx->is_simpass &&
			!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
		{
			bi=-1;
		}
	}

	if(bi>=0)
	{
		i=bi;

		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i+0, 7);
		BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i+1, 7);
		BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

		sctx->regalc_ltcnt[i+0]=0;
		sctx->regalc_ltcnt[i+1]=0;
		sctx->regalc_map[i+0]=reg;
		sctx->regalc_map[i+1]=zreg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_live|=3<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;
		creg=bgbcc_jx2_lcachereg[i+1];
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&31);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&31);
		if(fl&2)
			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		return(creg);
	}

	/* Check for unallocated registers. */
//	for(i=1; i<4; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<maxreg; i++)
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

		if(algn && (bgbcc_jx2_lcachereg[i+1]&1))
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
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&15);
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&15);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&31);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&31);
//		if(fl&2)
//			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
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
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<bgbcc_jx2_lmaxreg_lf; i++)
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
