int BGBCC_JX2C_ScratchCheckFpRegFree(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(sctx->no_fpu || sctx->fpu_gfp)
		return(0);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
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

int BGBCC_JX2C_ScratchSafeStompFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(sctx->no_fpu || sctx->fpu_gfp)
		return(0);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=2)
			sctx->is_leaf&=(~4);

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		if(sctx->sfreg_live&(1<<(reg&15)))
			{ BGBCC_DBGBREAK }
		return(BGBCC_JX2C_ScratchStompFpReg(ctx, sctx, reg));
	}

	return(BGBCC_JX2C_ScratchStompFpReg(ctx, sctx, reg));
}

int BGBCC_JX2C_ScratchStompFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(sctx->no_fpu || sctx->fpu_gfp)
		return(0);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=2)
			sctx->is_leaf&=(~4);

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_live|=1<<(reg&15);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchHoldFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(sctx->no_fpu || sctx->fpu_gfp)
		return(0);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=2)
			sctx->is_leaf&=(~4);

		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held|=1<<(reg&15);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchReleaseFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	if(sctx->no_fpu || sctx->fpu_gfp)
		return(0);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0))
	{
		if((reg&15)>=8)
			{ BGBCC_DBGBREAK }
		sctx->sfreg_held&=~(1<<(reg&15));
		sctx->sfreg_live&=~(1<<(reg&15));
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_ScratchAllocFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int cls)
{
	int i;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	sctx->is_leaf&=(~4);

	if(cls==BGBCC_SH_REGCLS_FR)
	{
		for(i=0; i<8; i++)
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

#if 0
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
#endif

	BGBCC_DBGBREAK
	return(-1);
}


int BGBCC_JX2C_EmitSaveFrameFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
#if 1
	int ofs;
#if 0
	ofs=BGBCC_JX2C_SaveFrameOfsForFpReg(ctx, sctx, reg);
	if(ofs!=0)
	{
		if(sctx->freg_save&(1<<(reg&31)))
			return(0);

		if(!sctx->is_simpass && !(sctx->freg_vsave&(1<<(reg&31))))
			{ BGBCC_DBGBREAK }

		sctx->freg_save|=1<<(reg&31);
		return(1);
	}
#endif

//	ofs=BGBCC_JX2C_SaveFrameOfsForFpReg(ctx, sctx, reg);
//	if(ofs!=0)
	if((	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, reg) ||
			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, reg)) &&
		((reg&15)>=8))
	{
		if(sctx->freg_save&(1<<(reg&31)))
			return(0);

		if(!sctx->is_simpass && !(sctx->freg_vsave&(1<<(reg&31))))
			{ BGBCC_DBGBREAK }

		sctx->freg_save|=1<<(reg&31);
		return(1);
	}
#endif

	return(0);
}

int BGBCC_JX2C_EmitReloadSavedFrameFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int reg)
{
	int ofs;

	return(0);
}

#if 1
const short bgbcc_jx2_fcachereg[16]={
	BGBCC_SH_REG_FR15, BGBCC_SH_REG_FR14,
	BGBCC_SH_REG_FR13, BGBCC_SH_REG_FR12,
	BGBCC_SH_REG_FR11, BGBCC_SH_REG_FR10,
	BGBCC_SH_REG_FR9, BGBCC_SH_REG_FR8,
	BGBCC_SH_REG_FR7, BGBCC_SH_REG_FR6,
	BGBCC_SH_REG_FR5, BGBCC_SH_REG_FR4	};
const short bgbcc_jx2_dcachereg[16]={
	BGBCC_SH_REG_DR15, BGBCC_SH_REG_DR14,
	BGBCC_SH_REG_DR13, BGBCC_SH_REG_DR12,
	BGBCC_SH_REG_DR11, BGBCC_SH_REG_DR10,
	BGBCC_SH_REG_DR9, BGBCC_SH_REG_DR8,
	BGBCC_SH_REG_DR7, BGBCC_SH_REG_DR6,
	BGBCC_SH_REG_DR5, BGBCC_SH_REG_DR4 };
const short bgbcc_jx2_fmaxreg=8;
const short bgbcc_jx2_fmaxreg_lf=12;

const short bgbcc_jx2_fcachereg_efpr[32]={
	BGBCC_SH_REG_FR15, BGBCC_SH_REG_FR14,
	BGBCC_SH_REG_FR13, BGBCC_SH_REG_FR12,
	BGBCC_SH_REG_FR11, BGBCC_SH_REG_FR10,
	BGBCC_SH_REG_FR9, BGBCC_SH_REG_FR8,
	BGBCC_SH_REG_FR31, BGBCC_SH_REG_FR30,
	BGBCC_SH_REG_FR29, BGBCC_SH_REG_FR28,
	BGBCC_SH_REG_FR27, BGBCC_SH_REG_FR26,
	BGBCC_SH_REG_FR25, BGBCC_SH_REG_FR24,
	BGBCC_SH_REG_FR7, BGBCC_SH_REG_FR6,
	BGBCC_SH_REG_FR5, BGBCC_SH_REG_FR4,
	BGBCC_SH_REG_FR23, BGBCC_SH_REG_FR22,
	BGBCC_SH_REG_FR21, BGBCC_SH_REG_FR20,
	};
const short bgbcc_jx2_dcachereg_efpr[32]={
	BGBCC_SH_REG_DR15, BGBCC_SH_REG_DR14,
	BGBCC_SH_REG_DR13, BGBCC_SH_REG_DR12,
	BGBCC_SH_REG_DR11, BGBCC_SH_REG_DR10,
	BGBCC_SH_REG_DR9, BGBCC_SH_REG_DR8,
	BGBCC_SH_REG_DR31, BGBCC_SH_REG_DR30,
	BGBCC_SH_REG_DR29, BGBCC_SH_REG_DR28,
	BGBCC_SH_REG_DR27, BGBCC_SH_REG_DR26,
	BGBCC_SH_REG_DR25, BGBCC_SH_REG_DR24,
	BGBCC_SH_REG_DR7, BGBCC_SH_REG_DR6,
	BGBCC_SH_REG_DR5, BGBCC_SH_REG_DR4,
	BGBCC_SH_REG_DR23, BGBCC_SH_REG_DR22,
	BGBCC_SH_REG_DR21, BGBCC_SH_REG_DR20,
	};
const short bgbcc_jx2_fmaxreg_efpr=16;
const short bgbcc_jx2_fmaxreg_lf_efpr=24;
#endif

/* For leaf traces, rotate index to prefer scratch registers.
 */
int BGBCC_JX2C_EmitRotateFpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx)
{
	int m1, m2, m3;
	int i1;

	if(!(sctx->is_tr_leaf&1))
		return(idx);

	m1=sctx->maxreg_fpr;
	m2=sctx->maxreg_fpr_lf;
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
int BGBCC_JX2C_EmitTryGetFpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int maxreg, vrsave;
	int creg, excl, nsv, userq, rcls;
	int pr0, pr1;
	int i, bi;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	switch(rcls)
	{
	case BGBCC_SH_REGCLS_DR:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_FR:
		userq=0;
		break;
	default:
		userq=0;
		break;
	}

	excl=0;

	maxreg=sctx->maxreg_fpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_fpr_lf;
	vrsave=0x00FF00FC;

#if 1
	/* value already in a register? */
//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
		if(excl&(1<<i))
			continue;

		if(!((sctx->fregalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->fregalc_map[i]))
		{
			sctx->fregalc_live|=1<<i;
			if(fl&1)
			{
				sctx->fregalc_dirty|=1<<i;
				sctx->fregalc_map[i]=reg;
			}

			if(sctx->fregalc_ltcnt[i]>0)
				sctx->fregalc_ltcnt[i]--;
			sctx->fregalc_utcnt[i]++;
//			creg=bgbcc_jx2_fcachereg[i];
			creg=sctx->fcachereg[i];
			if(userq)
//				creg=bgbcc_jx2_dcachereg[i];
				creg=sctx->dcachereg[i];
			return(creg);
		}
	}
#endif

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);

	if(((fl&3)==1) || ((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv)))
	{
		bi=-1; nsv=0;
	
		/* Check for registers not holding a live value. */
//		for(i=0; i<bgbcc_jx2_fmaxreg; i++)
		for(i=0; i<maxreg; i++)
		{
			if(excl&(1<<i))
				continue;

#if 0
			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}
#endif

			if(!((sctx->fregalc_save)&(1<<i)))
			{
//				creg=bgbcc_jx2_fcachereg[i];
				creg=sctx->fcachereg[i];
//				if(sctx->freg_save&(1<<(creg&31)))
				if((sctx->freg_save|vrsave)&(1<<(creg&31)))
					{ bi=i; break; }

				nsv++;
				continue;
			}

			if(!((sctx->fregalc_live)&(1<<i)) &&
				!((sctx->fregalc_dirty)&(1<<i)))
			{
				reg1=sctx->fregalc_map[i];
				pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i; break; }
				else
				{
					if((bi<0) ||
						((sctx->fregalc_ltcnt[i]>=sctx->fregalc_ltcnt[bi]) &&
						(pr0<=pr1)))
					{
						bi=i;
					}
				}
			}
		}

		/* Check if we are better off allocating another register */
		if(bi>=0)
		{
			reg1=sctx->fregalc_map[bi];
			if((nsv>=2) && (sctx->fregalc_ltcnt[bi]<=2) &&
				sctx->is_simpass &&
				!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
			{
				bi=-1;
			}
		}

#if 0
		if((pr0>=0) && (pr0<sctx->vsp_rsv))
			bi=pr0;
#endif
		
#if 1
		if(bi>=0)
		{
			i=bi;

			BGBCC_JX2C_EmitSyncFpRegisterIndex(ctx, sctx, i);
//			BGBCC_JX2C_StompLpFpRegisterIndex(ctx, sctx, i);

			sctx->fregalc_save|=1<<i;
			sctx->fregalc_ltcnt[i]=0;
			sctx->fregalc_map[i]=reg;
//			sctx->fregalc_utcnt[i]=1;
			sctx->fregalc_utcnt[i]++;
			sctx->fregalc_live|=1<<i;
			if(fl&1)
				{ sctx->fregalc_dirty|=1<<i; }
//			creg=bgbcc_jx2_fcachereg[i];
			creg=sctx->fcachereg[i];
			if(userq)
//				creg=bgbcc_jx2_dcachereg[i];
				creg=sctx->dcachereg[i];
			BGBCC_JX2C_EmitSaveFrameFpReg(ctx, sctx, creg);
//			if(fl&2)
//				{ BGBCC_JX2C_EmitLoadFrameVFpRegFpReg(ctx, sctx, reg, creg); }
			return(creg);
		}
#endif
	}

	return(BGBCC_SH_REG_ZZR);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_JX2C_EmitGetFpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int maxreg, vrsave;
	int creg, lng, excl, bi, nsv, userq, rcls;
	int pr0, pr1;
	int i, i1;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	creg=BGBCC_JX2C_EmitTryGetFpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	switch(rcls)
	{
	case BGBCC_SH_REGCLS_DR:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_FR:
		userq=0;
		break;
	default:
		userq=0;
		break;
	}

	excl=0;

	maxreg=sctx->maxreg_fpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_fpr_lf;
	vrsave=0x00FF00FC;

//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
	for(i=0; i<maxreg; i++)
		if(sctx->fregalc_ltcnt[i]<255)
			sctx->fregalc_ltcnt[i]++;

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	bi=-1; nsv=0;
	/* Check for registers not holding a live value. */
//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
		if(excl&(1<<i))
			continue;

#if 0
		if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}
#endif

		if(!((sctx->fregalc_save)&(1<<i)))
		{
//			creg=bgbcc_jx2_fcachereg[i];
			creg=sctx->fcachereg[i];
//			if(sctx->freg_save&(1<<(creg&31)))
			if((sctx->freg_save|vrsave)&(1<<(creg&31)))
				{ bi=i; break; }

			nsv++;
			continue;
		}
		if(!((sctx->fregalc_live)&(1<<i)) &&
			!((sctx->fregalc_dirty)&(1<<i)))
		{
			reg1=sctx->fregalc_map[i];
			pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

			if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i; break; }
			else
			{
				if((bi<0) ||
					((sctx->fregalc_ltcnt[i]>=sctx->fregalc_ltcnt[bi]) &&
					(pr0<=pr1)))
				{
					bi=i;
				}
			}
		}
	}
	
	if(bi<0)
	{
//		for(i=0; i<bgbcc_jx2_fmaxreg; i++)
		for(i=0; i<maxreg; i++)
		{
			if(excl&(1<<i))
				continue;

#if 0
			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}
#endif

			if(!((sctx->fregalc_save)&(1<<i)))
				{ continue; }
			if(!((sctx->fregalc_live)&(1<<i)))
			{
				reg1=sctx->fregalc_map[i];
				if((bi<0) || (sctx->fregalc_ltcnt[i]>=sctx->fregalc_ltcnt[bi]))
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
		reg1=sctx->fregalc_map[bi];
		if((nsv>=2) && (sctx->fregalc_ltcnt[bi]<=2) &&
			sctx->is_simpass &&
			!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
		{
			bi=-1;
		}
	}

#if 0
	if((pr0>=0) && (pr0<sctx->vsp_rsv))
		bi=pr0;
#endif

#if 1
	if(bi>=0)
	{
		i=bi;

		BGBCC_JX2C_EmitSyncFpRegisterIndex(ctx, sctx, i);
//		BGBCC_JX2C_StompLpFpRegisterIndex(ctx, sctx, i);

		if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
			{ BGBCC_DBGBREAK }

		sctx->fregalc_save|=1<<i;
		sctx->fregalc_ltcnt[i]=0;
		sctx->fregalc_map[i]=reg;
		sctx->fregalc_utcnt[i]=1;
		sctx->fregalc_live|=1<<i;
		if(fl&1)
			sctx->fregalc_dirty|=1<<i;
//		creg=bgbcc_jx2_fcachereg[i];
		creg=sctx->fcachereg[i];
		if(userq)
//			creg=bgbcc_jx2_dcachereg[i];
			creg=sctx->dcachereg[i];
		BGBCC_JX2C_EmitSaveFrameFpReg(ctx, sctx, creg);
		if(fl&2)
//		if((fl&2) && (i>=sctx->vsp_rsv))
			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		return(creg);
	}
#endif

	/* Check for unallocated registers. */
//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
		i1=BGBCC_JX2C_EmitRotateFpRegisterIndex(ctx, sctx, i);

		if(excl&(1<<i1))
			continue;

#if 0
		if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}
#endif

		if((sctx->fregalc_save)&(1<<i1))
			continue;
		
		sctx->fregalc_ltcnt[i1]=0;
		sctx->fregalc_map[i1]=reg;
		sctx->fregalc_utcnt[i1]=1;
		sctx->fregalc_save|=1<<i1;
		sctx->fregalc_live|=1<<i1;
		if(fl&1)
			sctx->fregalc_dirty|=1<<i1;

//		creg=bgbcc_jx2_fcachereg[i1];		
		creg=sctx->fcachereg[i1];
		if(userq)
//			creg=bgbcc_jx2_dcachereg[i1];
			creg=sctx->dcachereg[i1];
		BGBCC_JX2C_EmitSaveFrameFpReg(ctx, sctx, creg);

		if(fl&2)
		{
//			printf("load from uninitialized register\n");
		}
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

/* Dirty: FpRegister is being modified. Needs old value and writeback. */
int BGBCC_JX2C_EmitGetFpRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitGetFpRegister(ctx, sctx, reg, 3));
}

/* Write: FpRegister is being written. Old value is not needed. */
int BGBCC_JX2C_EmitGetFpRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitGetFpRegister(ctx, sctx, reg, 1));
}

/* Read: FpRegister is being read. No writeback needed. */
int BGBCC_JX2C_EmitGetFpRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitGetFpRegister(ctx, sctx, reg, 2));
}

/* Dirty: FpRegister is being modified. Needs old value and writeback. */
int BGBCC_JX2C_EmitTryGetFpRegisterDirty(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitTryGetFpRegister(ctx, sctx, reg, 3));
}

/* Write: FpRegister is being written. Old value is not needed. */
int BGBCC_JX2C_EmitTryGetFpRegisterWrite(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitTryGetFpRegister(ctx, sctx, reg, 1));
}

/* Read: FpRegister is being read. No writeback needed. */
int BGBCC_JX2C_EmitTryGetFpRegisterRead(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	return(BGBCC_JX2C_EmitTryGetFpRegister(ctx, sctx, reg, 2));
}


int BGBCC_JX2C_EmitReleaseFpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	ccxl_register reg1;
	int maxreg, vrsave;
	int creg, regfl;
	int i;

	maxreg=sctx->maxreg_fpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_fpr_lf;
	vrsave=0x00FF00FC;

	/* value in register? */
//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
		if(!((sctx->fregalc_save)&(1<<i)))
			continue;
			
		reg1=sctx->fregalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
		{
			sctx->fregalc_live|=1<<i;

			if(sctx->fregalc_utcnt[i]<=0)
				__debugbreak();

			sctx->fregalc_utcnt[i]--;

			regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

			if(!sctx->fregalc_utcnt[i])
			{
				if((regfl&BGBCC_REGFL_ALIASPTR))
				{
					BGBCC_JX2C_EmitSyncFpRegisterIndex(ctx, sctx, i);
				}

				sctx->fregalc_live&=~(1<<i);
			}

			return(1);
		}
	}

	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_JX2C_EmitSyncFpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int rgix)
{
	return(BGBCC_JX2C_EmitSyncFpRegisterIndex2(ctx, sctx, rgix, 3));
}

int BGBCC_JX2C_EmitSyncFpRegisterIndex2(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int rgix, int sfl)
{
	static int rchk=0;
	ccxl_register reg;
	ccxl_type tty;
	int creg, regfl, userq, rcls;
	int i;

	i=rgix;

	if(!((sctx->fregalc_save)&(1<<i)))
		return(0);

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	reg=sctx->fregalc_map[i];
	regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	switch(rcls)
	{
	case BGBCC_SH_REGCLS_DR:
		userq=1;
		break;
	case BGBCC_SH_REGCLS_FR:
		userq=0;
		break;
	default:
		userq=0;
		break;
	}


	if(((sctx->fregalc_dirty)&(1<<i)) && (sfl&1))
	{
		reg=sctx->fregalc_map[i];
//		creg=bgbcc_jx2_fcachereg[i];		
		creg=sctx->fcachereg[i];
		if(userq)
//			creg=bgbcc_jx2_dcachereg[i];
			creg=sctx->dcachereg[i];

		if((reg.val&CCXL_REGID_REGMASK)==CCXL_REGID_REGMASK)
			return(0);

#if 0
		if(1)
		{
			if((sctx->vsp_rsv>0) && (i<sctx->vsp_rsv))
			{
				/* Register-Reserved Vars don't need write back */
				sctx->fregalc_dirty&=~(1<<i);
				return(0);
			}
		}
#endif

#if 1
		if(BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!(regfl&BGBCC_REGFL_ALIASPTR))
		{
			if(!BGBCC_JX2C_CheckVRegLiveRange(ctx, sctx, reg))
			{
				//discard
				sctx->fregalc_dirty&=~(1<<i);
				return(0);
			}
		}
#endif

		if(!rchk)
		{
			rchk=1;
			BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
			rchk=0;
		}

		sctx->fregalc_dirty&=~(1<<i);
	}

	if(sfl&2)
	{
		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
			(regfl&BGBCC_REGFL_ALIASPTR))
		{
			sctx->fregalc_map[i].val=-1;
			sctx->fregalc_utcnt[i]=0;
			sctx->fregalc_ltcnt[i]=0;
			sctx->fregalc_dirty&=~(1<<i);
			sctx->fregalc_live&=~(1<<i);
		}
	}

	return(1);
}

int BGBCC_JX2C_EmitSyncFpRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	int i;

	/* value in register? */
//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
//	for(i=0; i<bgbcc_jx2_fmaxreg_lf; i++)
	for(i=0; i<bgbcc_jx2_fmaxreg_lf_efpr; i++)
	{
		BGBCC_JX2C_EmitSyncFpRegisterIndex(ctx, sctx, i);
		sctx->fregalc_utcnt[i]=0;
		sctx->fregalc_live&=~(1<<i);
	}

	return(0);
}

int BGBCC_JX2C_EmitLabelFlushFpRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	int i;

//	sctx->maxreg_gpr=bgbcc_jx2_maxreg;
//	if(sctx->has_bjx1egpr && sctx->use_egpr)
//		sctx->maxreg_gpr=bgbcc_jx2_maxreg_egpr;

	sctx->maxreg_fpr=bgbcc_jx2_fmaxreg;
	sctx->maxreg_fpr_lf=bgbcc_jx2_fmaxreg_lf;
	sctx->fcachereg=bgbcc_jx2_fcachereg;
	sctx->dcachereg=bgbcc_jx2_dcachereg;

	if(sctx->use_efpr)
	{
		sctx->maxreg_fpr=bgbcc_jx2_fmaxreg_efpr;
		sctx->maxreg_fpr_lf=bgbcc_jx2_fmaxreg_lf_efpr;
		sctx->fcachereg=bgbcc_jx2_fcachereg_efpr;
		sctx->dcachereg=bgbcc_jx2_dcachereg_efpr;
	}

//	for(i=0; i<bgbcc_jx2_fmaxreg; i++)
//	for(i=0; i<bgbcc_jx2_fmaxreg_lf; i++)
	for(i=0; i<bgbcc_jx2_fmaxreg_lf_efpr; i++)
//	for(i=0; i<sctx->maxreg_fpr_lf; i++)
	{
		if(!((sctx->fregalc_save)&(1<<i)))
			continue;

		sctx->fregalc_map[i].val=-1;
		sctx->fregalc_utcnt[i]=0;
		sctx->fregalc_ltcnt[i]=0;
		sctx->fregalc_dirty&=~(1<<i);
		sctx->fregalc_live&=~(1<<i);
	}
	
	return(0);
}

int BGBCC_JX2C_EmitScratchSyncFpRegisters(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	if(sctx->sfreg_live)
		{ BGBCC_DBGBREAK }

	return(0);
}

int BGBCC_JX2C_EmitScratchSyncFpRegistersSafeMask(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int mask)
{
	if(sctx->sfreg_live & (~mask))
		{ BGBCC_DBGBREAK }

	return(0);
}


int BGBCC_JX2C_EmitLoadStackOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1, nmid;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	nmid=BGBCC_SH_NMID_FMOVS;
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		nmid=BGBCC_SH_NMID_FMOVD;

	BGBCC_JX2_EmitOpLdRegDispReg(sctx, nmid,
		BGBCC_SH_REG_SP, ofs, dreg);
	return(1);
}

int BGBCC_JX2C_EmitStoreStackOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1, nmid;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	nmid=BGBCC_SH_NMID_FMOVS;
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		nmid=BGBCC_SH_NMID_FMOVD;

	BGBCC_JX2_EmitOpRegStRegDisp(sctx, nmid,
		dreg, BGBCC_SH_REG_SP, ofs);
	return(1);
}



int BGBCC_JX2C_EmitLoadFrameOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int ofs1;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	ofs1=ofs+(sctx->frm_size);
	BGBCC_JX2C_EmitLoadStackOfsFpReg(ctx, sctx, ofs1, dreg);
	return(1);
}

int BGBCC_JX2C_EmitStoreFrameOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int ofs1;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	ofs1=ofs+(sctx->frm_size);
	BGBCC_JX2C_EmitStoreStackOfsFpReg(ctx, sctx, ofs1, dreg);
	return(1);
}


int BGBCC_JX2C_EmitLoadBRegOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid1, int breg, int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1, nmid;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	nmid=BGBCC_SH_NMID_FMOVS;
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		nmid=BGBCC_SH_NMID_FMOVD;

	BGBCC_JX2_EmitOpLdRegDispReg(sctx, nmid,
		breg, ofs, dreg);
	return(1);
}

int BGBCC_JX2C_EmitStoreBRegOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid1, int breg, int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1, nmid;
	int i, j, k;

	if(sctx->no_fpu || sctx->fpu_gfp)
	{
		BGBCC_DBGBREAK
		return(-1);
	}

	nmid=BGBCC_SH_NMID_FMOVS;
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		nmid=BGBCC_SH_NMID_FMOVD;

	BGBCC_JX2_EmitOpRegStRegDisp(sctx, nmid,
		dreg, breg, ofs);
	return(1);
}
