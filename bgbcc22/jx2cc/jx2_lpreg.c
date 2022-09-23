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

#if 0
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
#endif


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

int BGBCC_JX2C_EmitCheckValidLpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx, int algn)
{
	if(!algn)
		return(1);
//	if(bgbcc_jx2_lcachereg[idx+1]&1)
	if(sctx->qcachereg[idx+1]&1)
		return(0);
//	if((bgbcc_jx2_lcachereg[idx+1]+1) != bgbcc_jx2_lcachereg[idx+0])
	if((sctx->qcachereg[idx+1]+1) != sctx->qcachereg[idx+0])
		return(0);
	return(1);
}

int BGBCC_JX2C_EmitCheckSavedLpRegIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx)
{
	int creg;
//	creg=bgbcc_jx2_lcachereg[idx+1];
//	creg=sctx->qcachereg[idx+1];
	creg=sctx->lcachereg[idx+1];
//	creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);
	return(BGBCC_JX2C_CheckFrameSavedLpReg(ctx, sctx, creg));
}

int BGBCC_JX2C_EmitCheckRegIndexIsLp(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int idx)
{
	if(!BGBCC_JX2C_EmitCheckValidLpRegisterIndex(ctx, sctx, idx, 1))
		return(0);
	if(sctx->regalc_map[idx+1].val==CCXL_REGID_REG_DZ)
		return(1);
	return(0);
}

int BGBCC_JX2C_EmitTryGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg, reg1;
	int pr0, pr1, bi;
	u64 vrsave, excl, uli, ulj;
	int creg, nsv, algn, maxreg;
	int i, j, k, i1;

	zreg.val=CCXL_REGID_REG_DZ;
	algn=BGBCC_JX2C_CheckLpRegisterNeedAlign(ctx, sctx, reg);

	excl=0;
//	if(sctx->use_bp)
		excl|=1;
	if(sctx->is_pic)
		excl|=4;

	maxreg=sctx->lmaxreg_gpr;
	if(sctx->is_tr_leaf&1)
		maxreg=sctx->lmaxreg_gpr_lf;
	vrsave=0x00FF00FF00FF00FCULL;

	if(sctx->is_leaftiny&1)
	{
//		excl=0;
//		vrsave=0xFFFFFF;
//		excl=0x00007FFF;
	}

	/* value already in a register? */
//	for(i=1; i<4; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
		if((excl>>i)&3)
			continue;

		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i+0))
			continue;
		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i+1))
			continue;

#if 1
		ulj=(sctx->regalc_save)>>i;
//		if(!(j&3))
		if((ulj&3)!=3)
		{
			if(!ulj)
				break;
			if(!(ulj&15))
				{ i+=3; continue; }
			continue;
		}
#endif

//		if(!((sctx->regalc_save)&(3ULL<<i)))
//			continue;
//		if(((sctx->regalc_save)&(3ULL<<i))!=(3ULL<<i))
//			continue;
//		if(algn && (bgbcc_jx2_lcachereg[i+1]&1))
//			continue;

		if(!BGBCC_JX2C_EmitCheckValidLpRegisterIndex(ctx, sctx, i, algn))
			continue;

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=3ULL<<i;
			if(fl&1)
			{
				sctx->regalc_dirty|=1ULL<<i;
				sctx->regalc_map[i]=reg;
			}

			if(sctx->regalc_ltcnt[i]>0)
				sctx->regalc_ltcnt[i]--;
			sctx->regalc_utcnt[i]++;

			creg=sctx->lcachereg[i+1];
//			creg=sctx->qcachereg[i+1];
//			creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);
			return(creg);
//			return(bgbcc_jx2_lcachereg[i+1]);
		}
	}

	pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	if(pr0<sctx->vsp_rsv)
	{
		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg);
	}

	if(((fl&3)==1) || ((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv)))
	{
		bi=-1; nsv=0;

#if 1
		for(i=0; i<maxreg; i++)
		{
			i1=BGBCC_JX2C_EmitRotateLpRegisterIndex(ctx, sctx, i, 1);

//			if(excl&(1ULL<<i1))
			if((excl>>i1)&3)
				continue;

			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+0))
				continue;
			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+1))
				continue;

			if(!BGBCC_JX2C_EmitCheckValidLpRegisterIndex(ctx, sctx,
					i1, algn))
				continue;

			if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

			if(!(sctx->is_tr_leaf&1) &&
				BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
					continue;

//			if(!((sctx->regalc_save)&(3ULL<<i)))
//			if(!((sctx->regalc_save)&(3ULL<<i1)) && sctx->is_simpass)
			if(!((sctx->regalc_save)&(3ULL<<i1)) && (sctx->is_simpass&32))
			{
//				creg=bgbcc_jx2_lcachereg[i1+1];
				creg=sctx->qcachereg[i1+1];
//				creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);

//				if((sctx->reg_save|vrsave)&(3ULL<<(creg&31)))
				if((sctx->reg_save|vrsave)&(3ULL<<(creg&63)))
					{ bi=i1; break; }

				creg=sctx->lcachereg[i1+1];

				nsv++;
				continue;
			}

//			if(!sctx->is_simpass &&
			if(!(sctx->is_simpass&32) &&
				!BGBCC_JX2C_EmitCheckSavedLpRegIndex(ctx, sctx, i1))
					continue;

//			if(((sctx->regalc_save)>>i1)!=3)
//			{
//				if(!sctx->is_simpass)
//					continue;
//			}

			if(!((sctx->regalc_live)&(3ULL<<i1)) &&
				!((sctx->regalc_dirty)&(1ULL<<i1)))
			{
				reg1=sctx->regalc_map[i1];
				pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);
				
				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i1; break; }
				else
				{
					if((bi<0) ||
						((sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]) &&
						(pr0<=pr1)))
					{
						bi=i1;
					}
				}
			}
		}

		/* Check if we are better off allocating another register */
		if(bi>=0)
		{
			reg1=sctx->regalc_map[bi];
//			if((nsv>=2) && (sctx->regalc_ltcnt[bi]<=2) &&
//			if((nsv>=1) && (sctx->regalc_ltcnt[bi]<=2) &&
			if((nsv>=1) && (sctx->regalc_ltcnt[bi]<=3) &&
//				sctx->is_simpass &&
				(sctx->is_simpass&32) &&
				!BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
			{
				bi=-1;
			}

			if(reg1.val==CCXL_REGID_REG_DZ)
				{ BGBCC_DBGBREAK }

			if((((sctx->regalc_save)>>bi)&3)!=3)
			{
//				if(!sctx->is_simpass)
				if(!sctx->is_simpass &&
					!BGBCC_JX2C_EmitCheckSavedLpRegIndex(ctx, sctx, bi))
						{ BGBCC_DBGBREAK }
			}
		}
#endif

		if((pr0>=0) && (pr0<sctx->vsp_rsv))
			bi=pr0;

#if 1
		if(bi>=0)
		{
//			BGBCC_DBGBREAK
		
			i=bi;

			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i+0);
			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i+1);
			BGBCC_JX2C_StompLpRegisterIndex(ctx, sctx, i);

			sctx->regalc_save|=3ULL<<i;
			sctx->regalc_ltcnt[i+0]=0;
			sctx->regalc_ltcnt[i+1]=0;
			sctx->regalc_map[i+0]=reg;
			sctx->regalc_map[i+1]=zreg;
//			sctx->regalc_utcnt[i]=1;
			sctx->regalc_utcnt[i]++;
			sctx->regalc_save|=3ULL<<i;
			sctx->regalc_live|=3ULL<<i;
			sctx->regalc_pair|=1ULL<<i;
			if(fl&1)
				{ sctx->regalc_dirty|=1ULL<<i; }

//			creg=bgbcc_jx2_lcachereg[i+1];
			creg=sctx->lcachereg[i+1];
//			creg=sctx->qcachereg[i+1];
//			creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);

			if(!creg || (creg==BGBCC_SH_REG_ZZR))
				{ BGBCC_DBGBREAK }
			
			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&63);
//			BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&63);
//			if(fl&2)
//				{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
#endif
	}

	return(BGBCC_SH_REG_ZZR);
}

int BGBCC_JX2C_SmashRegisterNonLive(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	ccxl_register reg1;
	ccxl_type tty;
	int i;

	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		if(!((sctx->regalc_save)&(1ULL<<i)))
			continue;
			
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
		{
			if(!(sctx->is_leaftiny&1))
			{
				if((sctx->regalc_utcnt[i]>0) ||
					(sctx->regalc_live&(1ULL<<i)))
				{
					BGBCC_DBGBREAK
					return(0);
				}
				
				if(sctx->regalc_dirty&(1ULL<<i))
				{
					BGBCC_DBGBREAK
					return(0);
				}

				sctx->regalc_map[i].val=-1;
				return(1);
			}

//			sctx->regalc_noval&=~(1ULL<<i);
		}
	}
	return(0);
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
	int i, i1;

	creg=BGBCC_JX2C_EmitTryGetLpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

//	BGBCC_JX2C_SmashRegisterNonLive(ctx, sctx, reg);

	algn=BGBCC_JX2C_CheckLpRegisterNeedAlign(ctx, sctx, reg);

	excl=0;
//	if(sctx->use_bp)
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
//		i1=i;
//		i1=maxreg-i-1;
		i1=BGBCC_JX2C_EmitRotateLpRegisterIndex(ctx, sctx, i, 1);

		if(excl&(3ULL<<i1))
			continue;

		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+0))
			continue;
		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+1))
			continue;

		if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

		if(!(sctx->is_tr_leaf&1) &&
			BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
				continue;

		if(!((sctx->regalc_save)&(3ULL<<i1)))
		{
			if(BGBCC_JX2C_EmitCheckValidLpRegisterIndex(ctx, sctx, i1, algn))
				nsv++;
		}

//		if(!((sctx->regalc_save)&(3ULL<<i)))
//			continue;
		if(((sctx->regalc_save)&(3ULL<<i1))!=(3ULL<<i1))
		{
//			nsv++;
			continue;
		}

//		if(algn && (bgbcc_jx2_lcachereg[i1+1]&1))
//			continue;
		if(!BGBCC_JX2C_EmitCheckValidLpRegisterIndex(ctx, sctx, i1, algn))
			continue;
		
		reg1=sctx->regalc_map[i1];
		if(	BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1) &&
			BGBCC_JX2C_EmitCheckSavedLpRegIndex(ctx, sctx, i1) &&
//			!sctx->is_simpass)
			!(sctx->is_simpass&32))
		{
			bi=i1;
			break;
		}

//		if(!((sctx->regalc_live)&(3ULL<<i1)))
		if(	!((sctx->regalc_live)&(3ULL<<i1)) &&
			!((sctx->regalc_dirty)&(3ULL<<i1)))
		{
			reg1=sctx->regalc_map[i1];
			pr1=BGBCC_JX2C_GetVRegPriority(ctx, sctx, reg1);

			if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i1; break; }
			else
			{
				if((bi<0) ||
					((sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]) &&
					(pr0<=pr1)))
				{
					bi=i1;
				}
			}
		}
	}
	
	if(bi<0)
	{
		for(i=0; i<maxreg; i++)
		{
//			i1=i;
//			i1=maxreg-i-1;
			i1=BGBCC_JX2C_EmitRotateLpRegisterIndex(ctx, sctx, i, 1);
	
			if(excl&(3ULL<<i1))
				continue;

			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+0))
				continue;
			if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+1))
				continue;

//			if(!(sctx->regalc_save>>i1))
//				break;

			if(!(sctx->is_tr_leaf&1) &&
				BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
					continue;

//			if(algn && (bgbcc_jx2_lcachereg[i+1]&1))
//				continue;
			if(!BGBCC_JX2C_EmitCheckValidLpRegisterIndex(
					ctx, sctx, i1, algn))
				continue;

			if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
			{
				if(pr0>=sctx->vsp_rsv)
					continue;
			}

			if((((sctx->regalc_save)>>i1)&3)!=3)
			{
//				if((((sctx->regalc_vrsave)>>i1)&3)!=3)
				continue;
			}

//			if(!((sctx->regalc_save)&(3ULL<<i1)))
//				{ continue; }
			if(!((sctx->regalc_live)&(3ULL<<i1)))
			{
				reg1=sctx->regalc_map[i1];
				if((bi<0) ||
						(sctx->regalc_ltcnt[i1]>=sctx->regalc_ltcnt[bi]))
					bi=i1;
				if(BGBCC_JX2C_CheckVRegIsZRegP(ctx, sctx, reg1))
					{ bi=i1; break; }
			}
		}
	}

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
		
		if(!sctx->is_simpass &&
			!BGBCC_JX2C_EmitCheckSavedLpRegIndex(ctx, sctx, bi))
				{ BGBCC_DBGBREAK }
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
//		sctx->regalc_utcnt[i]=1;
		sctx->regalc_utcnt[i]++;
		sctx->regalc_live|=3ULL<<i;
		sctx->regalc_save|=3ULL<<i;
		sctx->regalc_pair|=1ULL<<i;
		if(fl&1)
			sctx->regalc_dirty|=1ULL<<i;

//		creg=bgbcc_jx2_lcachereg[i+1];
		creg=sctx->lcachereg[i+1];
//		creg=sctx->qcachereg[i+1];
//		creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);

		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&31);
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&31);
		if(fl&2)
		{
			BGBCC_JX2C_EmitTempLoadReg(ctx, sctx, reg);
			BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg);
		}
		return(creg);
	}

//	if(!sctx->is_simpass)
//		{ BGBCC_DBGBREAK }

	/* Check for unallocated registers. */
//	for(i=1; i<4; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
	for(i=0; i<maxreg; i++)
	{
//		i1=i;
//		i1=maxreg-i-1;
		i1=BGBCC_JX2C_EmitRotateLpRegisterIndex(ctx, sctx, i, 3);
	
		if(excl&(3ULL<<i1))
			continue;

		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+0))
			continue;
		if(BGBCC_JX2C_CheckRegisterIndexExcludeP(ctx, sctx, i1+1))
			continue;

		if((sctx->vsp_rsv>0) && (i1<sctx->vsp_rsv))
		{
			if(pr0>=sctx->vsp_rsv)
				continue;
		}

		if((sctx->regalc_save)&(3ULL<<i1))
			continue;
		if((sctx->regalc_live)&(3ULL<<i1))
			continue;

//		if(algn && (bgbcc_jx2_lcachereg[i1+1]&1))
//			continue;
		if(!BGBCC_JX2C_EmitCheckValidLpRegisterIndex(ctx, sctx, i1, algn))
			continue;

		if(!(sctx->is_tr_leaf&1) &&
			BGBCC_JX2C_CheckRegisterIndexScratchP(ctx, sctx, i1))
				continue;

//		if(	((sctx->regalc_live)&(3ULL<<i1)) ||
//			((sctx->regalc_dirty)&(3ULL<<i1)))
//				continue;

		if((sctx->regalc_dirty)&(1ULL<<i1))
			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i1+0);
		if((sctx->regalc_dirty)&(2ULL<<i1))
			BGBCC_JX2C_EmitSyncRegisterIndex(ctx, sctx, i1+1);

		sctx->regalc_ltcnt[i1+0]=0;
		sctx->regalc_ltcnt[i1+1]=0;
		sctx->regalc_map[i1+0]=reg;
		sctx->regalc_map[i1+1]=zreg;
//		sctx->regalc_utcnt[i1]=1;
		sctx->regalc_utcnt[i1]++;
		sctx->regalc_save|=3ULL<<i1;
		sctx->regalc_live|=3ULL<<i1;
		sctx->regalc_pair|=1ULL<<i1;
		if(fl&1)
			sctx->regalc_dirty|=1ULL<<i1;

//		creg=bgbcc_jx2_lcachereg[i1+1];
		creg=sctx->lcachereg[i1+1];
//		creg=sctx->qcachereg[i1+1];
//		creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);

//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&15);
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&15);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, creg);
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+0)&31);
//		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, (creg+1)&31);
		if(fl&2)
			{ BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_ChecRegisterLive(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	ccxl_register reg1;
	ccxl_type tty;
	int i;

	for(i=0; i<sctx->maxreg_gpr_lf; i++)
	{
		if(!((sctx->regalc_save)&(1ULL<<i)))
			continue;
			
		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
		{
			if(!(sctx->is_leaftiny&1))
			{
				if(sctx->regalc_utcnt[i]<=0)
				{
//					__debugbreak();
					return(0);
				}
//				sctx->regalc_utcnt[i]--;
				return(1);
			}

//			sctx->regalc_noval&=~(1ULL<<i);
		}
	}
	return(0);
}

int BGBCC_JX2C_EmitReleaseLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	ccxl_register reg1;
	ccxl_type tty;
	int creg, regfl, rcls, maxreg;
	int i;

	if(sctx->is_leaftiny&1)
	{
#if 1
		for(i=0; i<sctx->maxreg_gpr_lf; i++)
		{
			if(!((sctx->regalc_save)&(3ULL<<i)))
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
	
//	maxreg=sctx->maxreg_gpr;
//	if(sctx->is_tr_leaf&1)
		maxreg=sctx->maxreg_gpr_lf;

	/* value in register? */
//	for(i=1; i<4; i++)
//	for(i=bgbcc_jx2_lminreg; i<bgbcc_jx2_lmaxreg; i++)
//	for(i=0; i<bgbcc_jx2_lmaxreg_lf; i++)
	for(i=0; i<maxreg; i++)
	{
		if(!((sctx->regalc_save)&(3ULL<<i)))
			continue;

		reg1=sctx->regalc_map[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, reg, reg1))
		{
			if(sctx->regalc_map[i+1].val!=CCXL_REGID_REG_DZ)
			{
				BGBCC_DBGBREAK
			}

			sctx->regalc_live|=3ULL<<i;
//			if(sctx->regalc_utcnt[i]>0)
//				sctx->regalc_utcnt[i]--;

			if(!(sctx->is_leaftiny&1))
			{
				if(sctx->regalc_utcnt[i]<=0)
					__debugbreak();
				sctx->regalc_utcnt[i]--;
			}

			regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);

			if(!sctx->regalc_utcnt[i])
			{
#if 0
				if((sctx->regalc_dirty)&(1ULL<<i))
				{
//					creg=bgbcc_jx2_lcachereg[i+1];
					creg=sctx->lcachereg[i+1];
//					creg=sctx->qcachereg[i+1];
//					creg=BGBCC_SH_REG_LR0+(creg&0x1E)+((creg>>5)&1);

					if(!rchk)
					{
						rchk++;
						BGBCC_JX2C_EmitStoreFrameVRegReg(
							ctx, sctx, reg, creg);
						rchk--;
					}
					sctx->regalc_dirty&=~(1ULL<<i);
				}
#endif

				if((regfl&BGBCC_REGFL_ALIASPTR))
				{
					BGBCC_JX2C_EmitSyncRegisterIndex2(ctx, sctx, i, 7);
				}

				sctx->regalc_live&=~(3ULL<<i);
				sctx->regalc_pair&=~(3ULL<<i);
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
	ccxl_register reg, reg1, zreg;
	int creg;
	int i;

	i=rgidx;

	if(!((sctx->regalc_save)&(1ULL<<i)))
		return(0);

	reg=sctx->regalc_map[i+0];
	reg1=sctx->regalc_map[i+1];
	zreg.val=CCXL_REGID_REG_Z;

	if(reg.val==CCXL_REGID_REG_DZ)
	{
		if((sctx->regalc_live)&(1ULL<<(i-1)))
			{ BGBCC_DBGBREAK }
		if((sctx->regalc_dirty)&(1ULL<<(i-1)))
			{ BGBCC_DBGBREAK }

		sctx->regalc_map[i-1]=zreg;
		sctx->regalc_map[i+0]=zreg;
		return(1);
	}

	if(reg1.val==CCXL_REGID_REG_DZ)
	{
		if((sctx->regalc_live)&(1ULL<<i))
			{ BGBCC_DBGBREAK }
		if((sctx->regalc_dirty)&(1ULL<<i))
			{ BGBCC_DBGBREAK }

		sctx->regalc_map[i+1]=zreg;
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
	int tr0, tr1;
	int csreg, cdreg, nm1;

//	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, sreg);
//	cdreg=BGBCC_JX2C_EmitTryGetRegisterWrite(ctx, sctx, dreg);

	csreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

	if(	(csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR) &&
		(cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		tr0=cdreg&0x1E;
		tr1=tr0+1;
		
		if(cdreg&1)
		{
			tr0+=32;
			tr1+=32;
		}
	
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			csreg, tr0);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, stype))
		{
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr1, 0);
		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, tr1);
			BGBCC_JX2C_EmitLeaSarRegImm(ctx, sctx, tr1, 63);
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
				BGBCC_SH_REG_R5, 63);
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
			BGBCC_SH_REG_R5, 63);
	}

	BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);

	return(1);
}
