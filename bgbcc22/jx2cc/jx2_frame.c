
int BGBCC_JX2C_EmitLoadFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1, nmid;
	int ofs1;
	int i, j, k;

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_JX2C_EmitLoadFrameOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, ofs1+4, dreg2+1);
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, ofs1+0, dreg2+0);
		return(1);
	}

	nmid=BGBCC_SH_NMID_MOVL;

	if(((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) && !sctx->is_addr64)
		nmid=BGBCC_SH_NMID_MOVW;

	if(BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		nmid=BGBCC_SH_NMID_MOVQ;
	}

	BGBCC_JX2_EmitOpLdRegDispReg(sctx, nmid,
		BGBCC_SH_REG_SP, ofs1, dreg);
	return(1);
}

int BGBCC_JX2C_EmitStoreFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, treg, nmid;
	int p0, p1;
	int ofs1;
	int i, j, k;

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_JX2C_EmitStoreFrameOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, ofs1+4);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
		return(1);
	}

	nmid=BGBCC_SH_NMID_MOVL;

	if(((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) && !sctx->is_addr64)
		nmid=BGBCC_SH_NMID_MOVW;

	if(BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		nmid=BGBCC_SH_NMID_MOVQ;
	}
	if(sctx->is_addr64)
	{
		if(dreg<32)
		{
			BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
			nmid=BGBCC_SH_NMID_MOVQ;
		}
	}

	BGBCC_JX2_EmitOpRegStRegDisp(sctx, nmid,
		dreg, BGBCC_SH_REG_SP, ofs1);
	return(1);
}

int BGBCC_JX2C_EmitLdaFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int dreg2;
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);
	BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		dreg, ofs1);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
		BGBCC_SH_REG_SP, dreg);
	return(1);
}


int BGBCC_JX2C_EmitStoreStackOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int dreg)
{
	int dreg2, nmid;
	int p0, p1;
	int ofs1;
	int i, j, k;

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_JX2C_EmitStoreStackOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs;

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, ofs1+4);
		if(p0>0)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			return(1);
		}

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs1);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+0, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	nmid=BGBCC_SH_NMID_MOVL;
	if(BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		nmid=BGBCC_SH_NMID_MOVQ;
	}

	BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, nmid,
		dreg, BGBCC_SH_REG_SP, ofs1);
	return(1);
}


int BGBCC_JX2C_EmitLoadBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	return(BGBCC_JX2C_EmitLoadOp2BRegOfsReg(
		ctx, sctx, nmid, -1, breg, ofs, dreg));
}

int BGBCC_JX2C_EmitLoadOp2BRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int nmid2, int breg, int ofs, int dreg)
{
	int dreg2;
	int p0, p1;
	int i, j, k;

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{	return(BGBCC_JX2C_EmitLoadBRegOfsFpReg(ctx, sctx,
			nmid, breg, ofs, dreg));	}

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
//		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
//			dreg2+1, breg, ofs+4);
		p0=BGBCC_JX2_TryEmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
			breg, ofs+4, dreg2+1);
		if(p0>0)
		{
//			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
//				dreg2+0, breg, ofs+0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				breg, ofs+0, dreg2+0);
			return(1);
		}

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			breg, BGBCC_SH_REG_R0, dreg2+0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			breg, BGBCC_SH_REG_R0, dreg2+1);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

	BGBCC_JX2_EmitOpLdRegDispReg(sctx, nmid,
		breg, ofs, dreg);
	if(nmid2>=0)
		{ BGBCC_JX2_EmitOpRegReg(sctx, nmid2, dreg, dreg); }
	return(1);
}

int BGBCC_JX2C_EmitStoreBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int dreg2, tr0;
	int p0, p1;
	int i, j, k;

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{	return(BGBCC_JX2C_EmitStoreBRegOfsFpReg(ctx, sctx,
			nmid, breg, ofs, dreg));	}

#if 0
	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		if(ofs==0)
		{
			BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg, breg);
			return(1);
		}

		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, nmid,
			dreg, breg, ofs);
		if(p0>0)
			return(1);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg, breg, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}
#endif

#if 0
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_FR0+(dreg&7)*2;
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
	
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+1, breg, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+0, breg, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}
#endif

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
	
		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, breg, ofs+4);
		if(p0>0)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, breg, ofs+0);
			return(1);
		}

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+0, breg, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, breg, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

	BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, nmid,
		dreg, breg, ofs);
	return(1);
}

int BGBCC_JX2C_EmitLeaBRegOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int ctreg;
	int opw1;
	int p0, p1;
	int i, j, k;

//	p0=BGBCC_JX2_EmitOpLdRegDispReg(sctx, nmid,
//		breg, ofs, dreg);
//	if(p0>0)
//		return(1);

	if(!ofs)
	{
		if(breg!=dreg)
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, breg, dreg);
		return(1);
	}

//	if((ctx->arch_sizeof_ptr==8) || (breg!=dreg))
	if(sctx->is_addr64 || (breg!=dreg))
	{
		opw1=BGBCC_SH_NMID_LEAB;	k=ofs;
		if(!(ofs&1))
			{ opw1=BGBCC_SH_NMID_LEAW;	k=ofs>>1; }
		if(!(ofs&3))
			{ opw1=BGBCC_SH_NMID_LEAL;	k=ofs>>2; }
		if(!(ofs&7))
			{ opw1=BGBCC_SH_NMID_LEAQ;	k=ofs>>3; }
		if(!(ofs&15))
			{ opw1=BGBCC_SH_NMID_LEAO;	k=ofs>>4; }

		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			opw1, breg, k, dreg);
		if(i>0)
			return(1);

//		BGBCC_DBGBREAK
	}

	if(breg!=dreg)
	{
		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, breg, ofs, dreg);
		if(i>0)
			return(1);

		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, ofs);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
	}else
	{	
		i=BGBCC_JX2_TryEmitOpImmReg(sctx, BGBCC_SH_NMID_ADD, ofs, dreg);
		if(i>0)
			return(1);
	
		ctreg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, ofs);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, ctreg, dreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, ctreg);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R3);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R3, ofs);
//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, BGBCC_SH_REG_R3, dreg);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R3);
	}

	return(1);
}


int BGBCC_JX2C_CheckNmidScaleMatch(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int sc)
{
	int rt;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB: rt=(sc==1); break;
	case BGBCC_SH_NMID_MOVW: rt=(sc==2); break;
	case BGBCC_SH_NMID_MOVL: rt=(sc==4); break;
	case BGBCC_SH_NMID_MOVQ: rt=(sc==8); break;
	case BGBCC_SH_NMID_LEAB: rt=(sc==1); break;
	case BGBCC_SH_NMID_LEAW: rt=(sc==2); break;
	case BGBCC_SH_NMID_LEAL: rt=(sc==4); break;
	case BGBCC_SH_NMID_LEAQ: rt=(sc==8); break;

	case BGBCC_SH_NMID_MOVUB: rt=(sc==1); break;
	case BGBCC_SH_NMID_MOVUW: rt=(sc==2); break;
	case BGBCC_SH_NMID_MOVUL: rt=(sc==4); break;

	case BGBCC_SH_NMID_FMOVS: rt=(sc==4); break;
	case BGBCC_SH_NMID_FMOVD: rt=(sc==8); break;

	default:	rt=0; break;
	}
	return(rt);
}



int BGBCC_JX2C_EmitLoadBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int dreg2, sw;
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
//		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg) &&
		(BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg)) &&
//		BGBCC_JX2_EmitCheckRegExtGPR(sctx, dreg) &&
		BGBCC_JX2C_CheckNmidScaleMatch(ctx, sctx, nmid, sc))
	{
		i=BGBCC_JX2_TryEmitOpLdReg2Reg(sctx, nmid,
			breg, ireg, dreg);
		if(i>0)
			return(1);
	}

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL1; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	case 8: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
	default:
		BGBCC_DBGBREAK
		break;
	}

	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	if(nm2>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg2+(sw^0));
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg2+(sw^1));
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

	BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
		breg, BGBCC_SH_REG_R0, dreg);
	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	return(1);
}

int BGBCC_JX2C_EmitStoreBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int dreg2, sw;
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

#if 1
	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
//		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg) &&
		(BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg)) &&
//		BGBCC_JX2_EmitCheckRegExtGPR(sctx, dreg) &&
		BGBCC_JX2C_CheckNmidScaleMatch(ctx, sctx, nmid, sc))
	{
		i=BGBCC_JX2_TryEmitOpRegStReg2(sctx, nmid,
			dreg, ireg, breg);
		if(i>0)
			return(1);
	}
#endif

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL1; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	case 8: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
	default:
		BGBCC_DBGBREAK
		break;
	}

	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	if(nm2>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^0), breg, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^1), breg, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

	BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
		dreg, BGBCC_SH_REG_R0, breg);
	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	return(1);
}


#if 1
int BGBCC_JX2C_EmitLoadBRegIRegScDispReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ireg, int sc, int disp, int dreg)
{
	int dreg2, sw;
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
//		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg) &&
		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, dreg) &&
		BGBCC_JX2C_CheckNmidScaleMatch(ctx, sctx, nmid, sc))
	{
		i=BGBCC_JX2_TryEmitOpLdReg2DispReg(sctx, nmid,
			breg, ireg, disp, dreg);
		if(i>0)
			return(1);
	}

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL1; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	case 8: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
	default:
		BGBCC_DBGBREAK
		break;
	}

	if((sc&(sc-1)) || (disp%sc))
	{
		BGBCC_DBGBREAK
	}

	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		disp/sc, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	if(nm2>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg2+(sw^0));
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
			breg, BGBCC_SH_REG_R0, dreg2+(sw^1));
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

	BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
		breg, BGBCC_SH_REG_R0, dreg);
	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	return(1);
}

int BGBCC_JX2C_EmitStoreBRegIRegScDispReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ireg, int sc, int disp, int dreg)
{
	int dreg2, sw;
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

#if 1
	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
//		BGBCC_JX2C_EmitRegIsGpReg(ctx, sctx, dreg) &&
		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, dreg) &&
		BGBCC_JX2C_CheckNmidScaleMatch(ctx, sctx, nmid, sc))
	{
		i=BGBCC_JX2_TryEmitOpRegStReg2Disp(sctx, nmid,
			dreg, ireg, breg, disp);
		if(i>0)
			return(1);
	}
#endif

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL1; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	case 8: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
	case 16: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
	default:
		BGBCC_DBGBREAK
		break;
	}
	
	if((sc&(sc-1)) || (disp%sc))
	{
		BGBCC_DBGBREAK
	}

	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		disp/sc, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	if(nm2>=0)
		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^0), breg, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			4, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
			dreg2+(sw^1), breg, BGBCC_SH_REG_R0);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

	BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
		dreg, BGBCC_SH_REG_R0, breg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	return(1);
}
#endif

int BGBCC_JX2C_GetGblIndexGotIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int gblidx)
{
	int i, j, k;
	
	for(i=0; i<sctx->got_n_gblidx; i++)
		if(sctx->got_gblidx[i]==gblidx)
			return(i);

	if(!sctx->got_gblidx)
	{
		i=4096;
		sctx->got_gblidx=bgbcc_malloc(i*sizeof(u32));
		sctx->got_n_gblidx=0;
		sctx->got_m_gblidx=i;
	}
	
	if((sctx->got_n_gblidx+1)>=sctx->got_m_gblidx)
	{
		i=sctx->got_m_gblidx+(sctx->got_m_gblidx>>1);
		sctx->got_gblidx=bgbcc_realloc(sctx->got_gblidx, i*sizeof(u32));
		sctx->got_m_gblidx=i;
	}

	i=sctx->got_n_gblidx++;
	sctx->got_gblidx[i]=gblidx;
	return(i);
}

int BGBCC_JX2C_GetGblIndexLabel(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int gblidx)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	int i;

	gbl=ctx->reg_globals[gblidx];

	BGBCC_CCXL_GlobalMarkReachable(ctx, gbl);

	i=gbl->fxoffs;
	if(i<=0)
	{
//		i=BGBCC_JX2_GenLabel(sctx);
		i=BGBCC_JX2_GetNamedLabel(sctx, gbl->name);
		gbl->fxoffs=i;
	}
	return(i);
}

int BGBCC_JX2C_GetVRegGotIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	int i, j, k;
	j=BGBCC_CCXL_GetRegID(ctx, sreg);
	i=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);
	return(i);
}

int BGBCC_JX2C_GetGblNameGotIndex(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	int i, j, k;
	
	gbl=BGBCC_CCXL_GetGlobal(ctx, name);
	if(!gbl->regtype)
		gbl->regtype=CCXL_LITID_ASMFUNC;

	BGBCC_CCXL_GlobalMarkReachable(ctx, gbl);

	k=gbl->fxoffs;
	if(k<=0)
	{
//		k=BGBCC_JX2_GenLabel(sctx);
		k=BGBCC_JX2_GetNamedLabel(sctx, gbl->name);
		gbl->fxoffs=k;
	}

	j=gbl->gblid;
//	j=BGBCC_CCXL_GetRegID(ctx, sreg);
	i=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);
	return(i);
}

int BGBCC_JX2C_GetGblNameLabel(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name)
{
	BGBCC_CCXL_RegisterInfo *gbl;
	int i, j, k;
	
	gbl=BGBCC_CCXL_GetGlobal(ctx, name);
	if(!gbl->regtype)
		gbl->regtype=CCXL_LITID_ASMFUNC;
	
	i=gbl->fxoffs;
	if(i<=0)
	{
//		i=BGBCC_JX2_GenLabel(sctx);
		i=BGBCC_JX2_GetNamedLabel(sctx, gbl->name);
		gbl->fxoffs=i;
	}
	return(i);
}

int BGBCC_JX2C_EmitLoadGblNameReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name, int dreg)
{
	int p0, p1;
	int k;
	
	if(sctx->is_pic)
	{
		k=BGBCC_JX2C_GetGblNameGotIndex(ctx, sctx, name);
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, k*4);
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}else
	{
		k=BGBCC_JX2C_GetGblNameLabel(ctx, sctx, name);
		if(sctx->is_rom)
			BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
		else
			BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_EmitLoadFrameVRegByValReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, tr0, rcls;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->locals[j]->regcls;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR))
		{
			if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);

			p0=BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->regs[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR))
		{
			if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);

			p0=BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

//		k=ctx->cur_func->regs[j]->fxoffs;

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);

		BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->args[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR))
		{
			if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);

			p0=BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

#if 1
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
#endif

//		k=ctx->cur_func->regs[j]->fxoffs;

#if 0
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);
		BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
#endif
		return(1);
	}

	p0=BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
	return(p0);
}

int BGBCC_JX2C_EmitStoreFrameVRegByValReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, tr0, rcls;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->locals[j]->regcls;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR))
		{
			if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);

			p0=BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->regs[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR))
		{
			if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);

			p0=BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

//		k=ctx->cur_func->regs[j]->fxoffs;

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);

		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->args[j]->regcls;

		if((rcls!=BGBCC_SH_REGCLS_VO_GR) &&
			(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR))
		{
			if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);

			p0=BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
			return(p0);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, dreg);
		return(1);
	}

	p0=BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
	return(p0);
}

int BGBCC_JX2C_EmitLoadFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	ccxl_type tty;
	char *s0;
	double f, g;
	float sf;
	s64 li, lj;
	int dreg2, treg, rcls;
	int nm1, nm2, nm3;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;

		tty=ctx->cur_func->locals[j]->type;
//		if(BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if(BGBCC_CCXL_TypeArrayP(ctx, tty))
		{
//			if(!strcmp(ctx->cur_func->name, "Mod_LoadTextures"))
//				{ k=-1; }
		
			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

#if 1
//		if(BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
		{
			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

		rcls=ctx->cur_func->locals[j]->regcls;

		if((ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_GR) ||
			(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_GR2) ||
			(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_QGR2) ||
			(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_QGR))
		{
			k=ctx->cur_func->locals[j]->fxoffs;
			i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}
#endif

		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			k=1;
		}

#if 0
		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) &&
			!BGBCC_CCXL_TypeSgIntP(ctx, tty))
		{
			k=ctx->cur_func->locals[j]->fxoffs;
			k=k+(sctx->frm_size);

			BGBCC_JX2C_EmitLoadTypeBRegOfsReg(ctx, sctx,
				tty, BGBCC_SH_REG_SP, k, dreg);
			return(1);
		}
#endif

		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			dreg=BGBCC_SH_REG_RD0|(dreg&15);

//		k=sctx->frm_offs_lcl+j*4;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		tty=ctx->cur_func->regs[j]->type;
		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			k=1;
		}

		rcls=ctx->cur_func->regs[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			dreg=BGBCC_SH_REG_RD0|(dreg&15);

		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;

		if((ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_GR) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_GR2) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_QGR2) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_QGR))
		{
			k=ctx->cur_func->args[j]->fxoffs;
			i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

#if 0
		tty=ctx->cur_func->args[j]->type;
		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) &&
			!BGBCC_CCXL_TypeSgIntP(ctx, tty))
		{
			k=ctx->cur_func->args[j]->fxoffs;
			k=k+(sctx->frm_size);

			BGBCC_JX2C_EmitLoadTypeBRegOfsReg(ctx, sctx,
				tty, BGBCC_SH_REG_SP, k, dreg);
			return(1);
		}
#endif

		rcls=ctx->cur_func->args[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			dreg=BGBCC_SH_REG_RD0|(dreg&15);

//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			*(f32 *)(&k)=j;
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//			i=BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
//			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
			dreg2=BGBCC_SH_REG_FR0+(dreg&15);

			f=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg2, k, j);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, j);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R1, k);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg2+0);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R1, dreg2+1);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
			BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);

//			j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
//			k=j>>31;
//			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg, j, k);
			return(1);
		}
	
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
	{
		if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
			BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);
//			j=li;
//			k=li>>32;
//			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg, j, k);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
//			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
			dreg2=BGBCC_SH_REG_FR0+(dreg&15);

			f=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg2, k, j);
			return(1);
		}

		j=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg))
	{
		if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			*(f32 *)(&k)=f;

//			if(ctx->has_bjx1mov)
//			{
//				if(!(sctx->sreg_live&1) && !(k&255))
//				{
//				}
//			}

//			i=BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, k);
//			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, k);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
//			dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
			dreg2=BGBCC_SH_REG_DR0+(dreg&15);

			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));

			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg2, k, j);

#if 0
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, j);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, k);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R0, dreg2+0);
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx, BGBCC_SH_REG_R1, dreg2+1);
#endif
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg) ||
			BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
		{
//			li=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			li=*(u64 *)(&f);

			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);
//			j=li;
//			k=li>>32;
//			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg, j, k);
			return(1);
		}
		
		if(!(dreg>>4))
		{
			BGBCC_DBGBREAK
		}
	
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
		*(float *)(&j)=f;
//		j=f;
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmStringP(ctx, sreg))
	{
		s0=BGBCC_CCXL_GetRegImmStringValue(ctx, sreg);
//		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
		k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);

		if(sctx->is_pic)
		{
			BGBCC_CCXL_StubError(ctx);
		}else
		{
			if(sctx->is_rom)
			{
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
			}
			else
			{
//				if(!BGBCC_JX2_EmitCheckRegBaseGPR(sctx, dreg))
				if(0)
				{
					treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
					BGBCC_JX2_EmitLoadRegLabelRel24(sctx, treg, k);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						treg, dreg);
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				}else
				{
					BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
				}
			}
		}
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmWStringP(ctx, sreg))
	{
		s0=BGBCC_CCXL_GetRegImmStringValue(ctx, sreg);
//		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
		k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS2(sctx, s0);

		if(sctx->is_pic)
		{
			BGBCC_CCXL_StubError(ctx);
		}else
		{
			if(sctx->is_rom)
			{
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
			}
			else
			{
				BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
			}
		}
		return(1);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg))
	{
#if 0
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) &&
			BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, treg);
			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, tty) &&
			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, treg);
			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}
#endif

		tty=BGBCC_CCXL_GetRegType(ctx, sreg);
//		if(sctx->has_bjx1mov && !sctx->is_pic &&
//			!sctx->is_rom &&
//			BGBCC_JX2C_ScratchCheckRegFree(ctx, sctx,
//				BGBCC_SH_REG_R0) &&
//			BGBCC_JX2_EmitCheckRegBaseGPR(sctx, dreg))
		if(1)
		{
			if(BGBCC_CCXL_TypeSgIntP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVL, dreg, k);
				return(1);
			}

			if(BGBCC_CCXL_TypeSmallIntP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(BGBCC_CCXL_TypeSignedByteP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVB;
				if(BGBCC_CCXL_TypeSignedShortP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVW;
				if(BGBCC_CCXL_TypeUnsignedByteP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUB;
				if(BGBCC_CCXL_TypeUnsignedShortP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUW;

				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, dreg, k);
				return(1);
			}

			if((BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
				BGBCC_CCXL_TypePointerP(ctx, tty)) &&
				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(sctx->is_addr64)
				{
					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
					nm1=BGBCC_SH_NMID_MOVQ;
				}
			
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, dreg, k);
				return(1);
			}

#if 0
			if(BGBCC_CCXL_TypeFloatP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_FMOVS, dreg, k);
				return(1);
			}
#endif
		}
		
		treg=dreg;
//		if(	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
//			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg) ||
//			BGBCC_JX2C_EmitRegIsLpReg(ctx, sctx, dreg)	)
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(sctx, dreg))
		{
			treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		}

//		if(sctx->is_pic)
		if(0)
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
//			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, treg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
//			BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
			if(sctx->is_rom)
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, treg, k);
			else
				BGBCC_JX2_EmitLoadRegLabelRel24(sctx, treg, k);
		}

		tty=BGBCC_CCXL_GetRegType(ctx, sreg);

		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
//			BGBCC_CCXL_TypeFunctionP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty))
		{
//			if(treg!=dreg)
//				{ BGBCC_DBGBREAK }

			if(treg!=dreg)
			{
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			}
		
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
			(BGBCC_CCXL_TypePointerP(ctx, tty) && !sctx->is_addr64))
		{
//			p1=BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
//				dreg, dreg);

//			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
//				BGBCC_SH_NMID_MOVL, dreg, 0, dreg);

//			nm1=BGBCC_SH_NMID_MOVL;
//			nm2=-1;

//			BGBCC_JX2C_EmitLoadOp2BRegOfsReg(ctx, sctx,
//				nm1, nm2, dreg, 0, dreg);

			BGBCC_JX2C_EmitLoadTypeBRegOfsReg(ctx, sctx,
				tty, treg, 0, dreg);

			if(treg!=dreg)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVariantP(ctx, tty) ||
			(BGBCC_CCXL_TypePointerP(ctx, tty) && sctx->is_addr64))
		{
			nm1=BGBCC_SH_NMID_MOVL;
			if(sctx->is_addr64)
			{
				BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
				nm1=BGBCC_SH_NMID_MOVQ;
			}

			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
				nm1, treg, 0, dreg);

			if(treg!=dreg)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
		{
			if(treg!=dreg)
			{
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}
		
//			if(treg!=dreg)
//				{ BGBCC_DBGBREAK }
			return(1);
		}

#if 1
		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			if(sctx->fpu_soft)
			{
				if(BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
				{
					BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
						BGBCC_SH_NMID_MOVQ, treg, 0, dreg);
				}else
				{
					BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
						BGBCC_SH_NMID_MOVL, treg, 0, dreg);
				}

				if(treg!=dreg)
					BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}
		
//			p1=BGBCC_JX2_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, dreg);

//			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
//				BGBCC_SH_NMID_FMOVS, dreg, 0, dreg);

			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);

			if(treg!=dreg)
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}
#endif

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	if(BGBCC_CCXL_IsRegImmInt128P(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmFloat128P(ctx, sreg))
	{
		BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
		k=BGBCC_JX2C_IndexLitInt128(ctx, sctx, li, lj);
	
//		if(sctx->is_pic)
		if(0)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		}else
		{
			if(sctx->is_rom)
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
			else
				BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
		}

		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitStoreFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register dreg, int sreg)
{
	ccxl_type tty;
	int cdreg, treg, tsz, nm1, rcls;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, ctx->cur_func->locals[j]->type))
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
		BGBCC_JX2C_EmitDebugCheckReg(ctx, sctx,
			ctx->cur_func->locals[j]->type, sreg);
#endif

		if(BGBCC_CCXL_TypeValueObjectP(ctx, ctx->cur_func->locals[j]->type))
		{
//			cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
			treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx,
				ctx->cur_func->locals[j]->type);

			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			if(ctx->cur_func->locals[j]->regcls!=BGBCC_SH_REGCLS_VO_REF)
				k=ctx->cur_func->locals[j]->fxoffs;

			i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, treg);

			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, treg, sreg, tsz, 4);

//			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(i);
		}

		rcls=ctx->cur_func->locals[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			sreg=BGBCC_SH_REG_RD0|(sreg&15);

		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, ctx->cur_func->regs[j]->type))
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
#endif

		rcls=ctx->cur_func->regs[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			sreg=BGBCC_SH_REG_RD0|(sreg&15);

		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, dreg))
	{
		j=dreg.val&CCXL_REGID_BASEMASK;

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, ctx->cur_func->args[j]->type))
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
		BGBCC_JX2C_EmitDebugCheckReg(ctx, sctx,
			ctx->cur_func->args[j]->type, sreg);
#endif

		rcls=ctx->cur_func->args[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			sreg=BGBCC_SH_REG_RD0|(sreg&15);

		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, dreg))
	{
#if 1
		tty=BGBCC_CCXL_GetRegType(ctx, dreg);
//		if(sctx->has_bjx1mov && !sctx->is_pic &&
//			!sctx->is_rom &&
//			BGBCC_JX2C_ScratchCheckRegFree(ctx, sctx,
//				BGBCC_SH_REG_R0) &&
//			BGBCC_JX2_EmitCheckRegBaseGPR(sctx, sreg))
		if(1)
		{
#if 0
			if((BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
//			if((BGBCC_CCXL_TypeSgIntP(ctx, tty) ||
				BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
				BGBCC_CCXL_TypePointerP(ctx, tty)) &&
				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVL, sreg, k);
				return(1);
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeSmallIntP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(BGBCC_CCXL_TypeSgByteP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVB;
				if(BGBCC_CCXL_TypeSgShortP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVW;

				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					nm1, sreg, k);
				return(1);
			}

			if((BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
				BGBCC_CCXL_TypePointerP(ctx, tty)) &&
				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(sctx->is_addr64)
				{
					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
					nm1=BGBCC_SH_NMID_MOVQ;
				}
			
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					nm1, sreg, k);
				return(1);
			}
#endif

#if 0
			if(BGBCC_CCXL_TypeFloatP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_FMOVS, sreg, k);
				return(1);
			}
#endif
		}
#endif

		treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(sctx->is_pic)
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, treg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
			if(sctx->is_rom)
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, treg, k);
			else
				BGBCC_JX2_EmitLoadRegLabelRel24(sctx, treg, k);
		}

		tty=BGBCC_CCXL_GetRegType(ctx, dreg);

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, tty))
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
#endif

		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{
			tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx, treg, sreg, tsz, 4);

//			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
			BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
			BGBCC_CCXL_TypePointerP(ctx, tty))
		{
//			p1=BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVL,
//				dreg, treg);

			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, treg, 0, sreg);

//			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			if(sctx->fpu_soft)
			{
				if(BGBCC_JX2_EmitCheckRegQuad(sctx, sreg))
				{
					BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
						BGBCC_SH_NMID_MOVQ, treg, 0, sreg);
				}else
				{
					BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
						BGBCC_SH_NMID_MOVL, treg, 0, sreg);
				}

				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}

//			p1=BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, treg);

			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_FMOVS, treg, 0, sreg);

//			BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
		{
			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_EmitLdaValSyncFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;
		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ALIASPTR;

		if(ctx->cur_func->regs[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
		{
			k=(ctx->cur_func->regs[j]->fxmoffs)+(sctx->frm_offs_fix);
			BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, sreg, dreg);
			return(1);
		}

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ALIASPTR;

		if(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
		{
			k=(ctx->cur_func->args[j]->fxmoffs)+(sctx->frm_offs_fix);
			BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx, sreg, dreg);
			return(1);
		}

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	i=BGBCC_JX2C_EmitLdaFrameVRegReg(ctx, sctx, sreg, dreg);
	return(i);
}

int BGBCC_JX2C_EmitLdaFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR;

		if(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;

//		if(ctx->cur_func->regs[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
//			k=(ctx->cur_func->regs[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;

//		if(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
//			k=(ctx->cur_func->args[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg))
	{
//		if(sctx->is_pic)
		if(0)
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, k*4);
			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		}else
		{
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
			if(sctx->is_rom)
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
			else
				BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
		}
		
		return(0);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_GetFrameVRegFlags(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	int p0, p1;
	int i, j, k;

	if((sreg.val&4095)==4095)
		return(0);

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		if(!ctx->cur_func->regs && !j)
			return(0);
		return(ctx->cur_func->regs[j]->regflags);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		return(ctx->cur_func->args[j]->regflags);
	}

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=ctx->cur_func->locals[j]->fxoffs;
//		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;
//		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR;
		return(ctx->cur_func->locals[j]->regflags);
	}
	
	return(0);
}

int BGBCC_JX2C_SetupFrameVRegSpan(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	BGBCC_JX2_VarSpan2 *vspb;
	ccxl_type tty;
	int fl;
	int i, j, k;

	if(!sreg.val)
		return(0);
	if((sreg.val&4095)==4095)
		return(0);

	sctx->vsp_tcnt++;

	if(!sctx->vspan)
	{
		sctx->vspan=bgbcc_malloc(1024*sizeof(BGBCC_JX2_VarSpan *));
		sctx->vspan_num=0;
		sctx->vspan_max=1024;
	}
	
	for(i=0; i<sctx->vspan_num; i++)
	{
		vsp=sctx->vspan[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vsp->reg, sreg))
		{
			if(sctx->tr_opnum < vsp->bbeg)
				vsp->bbeg=sctx->tr_opnum;
			if(sctx->tr_opnum > vsp->bend)
				vsp->bend=sctx->tr_opnum;

			if(sctx->tr_trnum < vsp->tbeg)
				vsp->tbeg=sctx->tr_trnum;
			if(sctx->tr_trnum > vsp->tend)
				vsp->tend=sctx->tr_trnum;

			vsp->cnt++;
			
			while(i>0)
			{
				vsp1=sctx->vspan[i-1];
				if(vsp->cnt<=vsp1->cnt)
					break;
				if((vsp->flag&3) && !(vsp1->flag&3))
					break;
				sctx->vspan[i-1]=vsp;
				sctx->vspan[i  ]=vsp1;
				i--;
			}
			
			j=(sreg.val>>12)&4093;
			if(j<0)j=0;
			if(j>63)j=63;
			vspb=vsp->seq+j;

			if(sctx->tr_opnum < vspb->bbeg)
				vspb->bbeg=sctx->tr_opnum;
			if(sctx->tr_opnum > vspb->bend)
				vspb->bend=sctx->tr_opnum;
			if(sctx->tr_trnum < vspb->tbeg)
				vspb->tbeg=sctx->tr_trnum;
			if(sctx->tr_trnum > vspb->tend)
				vspb->tend=sctx->tr_trnum;
			vspb->cnt++;
			
			return(1);
		}
	}

	tty=BGBCC_CCXL_GetRegType(ctx, sreg);
	if(BGBCC_CCXL_TypeRealP(ctx, tty))
		{ sctx->use_fpr=1; }
	if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{ sctx->use_dbr=1; }

	fl=0;
	if(	!BGBCC_CCXL_IsRegTempP(ctx, sreg) &&
		!BGBCC_CCXL_IsRegLocalP(ctx, sreg))
			fl|=1;

	if(	BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
		BGBCC_CCXL_TypeArrayP(ctx, tty) ||
		BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
		BGBCC_CCXL_TypeRealP(ctx, tty))
		fl|=2;

	i=sctx->vspan_num++;
	vsp=sctx->vspan[i];
	if(!vsp)
	{
		vsp=bgbcc_malloc(sizeof(BGBCC_JX2_VarSpan));
		sctx->vspan[i]=vsp;
	}

	vsp->reg=sreg;
	vsp->bbeg=sctx->tr_opnum;
	vsp->bend=sctx->tr_opnum;
	vsp->tbeg=sctx->tr_trnum;
	vsp->tend=sctx->tr_trnum;
	vsp->cnt=1;
	vsp->flag=fl;
	
	for(j=0; j<64; j++)
	{
		vspb=vsp->seq+j;
		vspb->bbeg=999999;
		vspb->tbeg=999999;
		vspb->bend=-999999;
		vspb->tend=-999999;
		vspb->cnt=0;
	}
	
	j=(sreg.val>>12)&4093;
	if(j<0)j=0;
	if(j>63)j=63;
	vspb=vsp->seq+j;

	vspb->bbeg=sctx->tr_opnum;
	vspb->bend=sctx->tr_opnum;
	vspb->tbeg=sctx->tr_trnum;
	vspb->tend=sctx->tr_trnum;
	vspb->cnt=1;

	return(1);
}

int BGBCC_JX2C_SetupFrameLayout(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_CCXL_VirtOp *vop;
	BGBCC_CCXL_VirtTr *vtr;
	ccxl_register reg;
	ccxl_type tty;
	int trn;
	int ni, nf, rcls;
	int i, j, k, ka, kf;

	sctx->vspan_num=0;

	ctx->cur_func=obj;
	sctx->use_bp=0;
	sctx->use_fpr=0;
	sctx->use_dbr=0;
	sctx->is_vararg=0;
	sctx->frm_offs_retstr=0;

	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];
		if(vop->opn==CCXL_VOP_LDAVAR)
		{
			reg=vop->srca;
			j=reg.val&4095;
			if(BGBCC_CCXL_IsRegTempP(ctx, reg))
				{ obj->regs[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
			if(BGBCC_CCXL_IsRegArgP(ctx, reg))
				{ obj->args[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
			if(BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ obj->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
		}
	}

	sctx->vsp_tcnt=0;
	trn=0;
	for(i=0; i<obj->n_vop; i++)
	{
		vtr=obj->vtr[trn];
		vop=obj->vop[i];

		while(i>=(vtr->b_ops+vtr->n_ops) && (trn<obj->n_vtr))
		{
			trn++;
			vtr=obj->vtr[trn];
		}
		
		sctx->tr_trnum=trn;
		sctx->tr_opnum=i;
		
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->dst);
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->srca);
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->srcb);


		if(vop->opn==CCXL_VOP_CALL)
		{
			for(j=0; j<vop->imm.call.na; j++)
			{
				BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
					vop->imm.call.args[j]);
			}
		}
	}
	
	sctx->use_egpr=0;
	if(sctx->has_bjx1egpr)
	{
#if 1
//		if(sctx->vspan_num>32)
//		if(sctx->vspan_num>48)
//		if(sctx->vspan_num>56)
//		if(sctx->vspan_num>64)
//		if(sctx->vspan_num>68)

		if(sctx->vspan_num>72)

//		if(sctx->vspan_num>80)
//		if(sctx->vspan_num>88)
//		if(sctx->vspan_num>96)
//		if(sctx->vspan_num>128)
		{
			sctx->use_egpr=1;
		}
		
//		if(sctx->is_mergece)
//			sctx->use_egpr=0;
#endif

//		sctx->use_egpr=1;
	}

	ni=0; nf=0;
	k=0; ka=0; kf=0;
	k-=2*4;		//saved PR, R14
//	k-=6*4;		//saved R8/9/10/11/13, R2
	k-=8*4;		//saved R8/9/10/11/13, R2

//	if(sctx->has_bjx1egpr)
	if(sctx->has_bjx1egpr && sctx->use_egpr)
		k-=8*4;		//saved R24..R31

	if(sctx->is_addr64)
		k*=2;

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;
	
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);
		obj->locals[i]->regcls=rcls;
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
			obj->locals[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->locals[i]->type);
			if(!j)
				{ BGBCC_DBGBREAK }
			j=(j+3)&(~3);
			kf+=j;
			break;

		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_QGR:
		case BGBCC_SH_REGCLS_WGR:
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}

#if 0
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->locals[i]->type) ||
			(BGBCC_CCXL_TypeValueObjectP(ctx, obj->locals[i]->type) &&
			(rcls==BGBCC_SH_REGCLS_VO_REF))	)
		{
//			k-=4; obj->locals[i]->fxoffs=k;
			obj->locals[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->locals[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}
#endif
	}

	for(i=0; i<obj->n_regs; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
		obj->regs[i]->regcls=rcls;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
			j=(j+3)&(~3);
			kf+=j;
			break;

		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_QGR:
		case BGBCC_SH_REGCLS_WGR:
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}

#if 0
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->regs[i]->type))
		{
//			k-=4; obj->regs[i]->fxoffs=k;
			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}

		if(rcls==BGBCC_SH_REGCLS_VO_REF)
		{
//			k-=4; obj->regs[i]->fxoffs=k;
			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}
#endif
	}

	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ sctx->is_vararg=1; }

		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		if(rcls==BGBCC_SH_REGCLS_WGR)
			rcls=BGBCC_SH_REGCLS_GR;

		obj->args[i]->regcls=rcls;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_FR:
		case BGBCC_SH_REGCLS_FR2:
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_WGR:
			break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			break;
		default:
			BGBCC_DBGBREAK
			break;
		}

//		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->args[i]->type) &&
//			(rcls==BGBCC_SH_REGCLS_VO_REF))
		if(rcls==BGBCC_SH_REGCLS_VO_REF)
		{
//			k-=4; obj->args[i]->fxoffs=k;
			obj->args[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->args[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}
	}

	if(sctx->use_fpr)
		{ k-=4*8; }		//saved FR12/13/14/15

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
	if(rcls==BGBCC_SH_REGCLS_VO_REF)
	{
		k-=4;
		sctx->frm_offs_retstr=k;
	}

	k-=kf;
	sctx->frm_offs_fix=k;

#if 1
	for(i=0; i<obj->n_args; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		if(rcls==BGBCC_SH_REGCLS_WGR)
			rcls=BGBCC_SH_REGCLS_GR;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_WGR:
//			if(sctx->is_addr64)
			if(sctx->has_bjx1egpr)
			{
				if(ni<8)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++; }
				else		{ obj->args[i]->fxoffs=ka; ka+=4; }
				break;
			}
		
			if(ni<4)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++; }
			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
			{
				if((ni<4) || ((ni<8) && sctx->has_bjx1egpr))
				{
					k&=(~7); k-=8;
					obj->args[i]->fxoffs=k; ni++;
				}
				else
				{
					ka=(ka+7)&(~7);
					obj->args[i]->fxoffs=ka;
					ka+=8;
				}
				break;
			}

			if(ni<4)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; ni++; }
			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->is_addr64)
			{
//				if(ni<7)
				if((ni<3) || ((ni<7) && sctx->has_bjx1egpr))
				{
					k&=~7; k-=16;
					obj->args[i]->fxoffs=k; ni+=2;
				}
				else
				{
					ka=(ka+7)&(~7);
					obj->args[i]->fxoffs=ka; ka+=16;
				}
				break;
			}
			if(ni<3)	{ k&=~3; k-=8; obj->args[i]->fxoffs=k; ni+=2; }
			else		{ obj->args[i]->fxoffs=ka; ka+=8; }
			break;

		case BGBCC_SH_REGCLS_FR:
			if(nf<4)	{ k&=~3; k-=4; obj->args[i]->fxoffs=k; nf++; }
			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			sctx->use_fpr=1;
			break;
		case BGBCC_SH_REGCLS_DR:
			if(nf<4)	{ k&=~7; k-=8; obj->args[i]->fxoffs=k; nf++; }
			else		{ ka=(ka+7)&(~7); obj->args[i]->fxoffs=ka; ka+=8; }
			sctx->use_fpr=1;
			break;

#if 0
		case BGBCC_SH_REGCLS_FR2:
			if(sctx->is_addr64)
			{
				nf+=nf&1;
				if(nf<7)
				{
					k&=~7; k-=8;
					obj->args[i]->fxoffs=k; nf+=2;
				}
				else
				{
					ka=(ka+7)&(~7);
					obj->args[i]->fxoffs=ka; ka+=8;
				}
				sctx->use_fpr=1;
				sctx->use_dbr=1;
				break;
			}

			nf+=nf&1;
			if(nf<7)	{ k&=~3; k-=8; obj->args[i]->fxoffs=k; nf+=2; }
			else		{ obj->args[i]->fxoffs=ka; ka+=8; }
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			break;
#endif

		default:
			BGBCC_DBGBREAK
			break;
		}
	}
#endif

#if 0
	for(i=0; i<obj->n_locals; i++)
		{ obj->locals[i]->fxoffs=0; }
	for(i=0; i<obj->n_regs; i++)
		{ obj->regs[i]->fxoffs=0; }

	for(i=sctx->vspan_num-1; i>=0; i--)
	{
		reg=sctx->vspan[i]->reg;

//		tty=BGBCC_CCXL_GetRegType(ctx, reg);

		if(	!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				continue;

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_GR:
			k&=~1; k-=2;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
				{ k-=8;	break; }
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->is_addr64)
				{ k-=16;	break; }
			k&=~3; k-=8;	break;
		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k&=~7; k-=8;	break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k&=~3; k-=8;	break;
		default:
			BGBCC_DBGBREAK
			break;
		}
		
		j=reg.val&4095;
		if(BGBCC_CCXL_IsRegTempP(ctx, reg))
			{ obj->regs[j]->fxoffs=k; }
		if(BGBCC_CCXL_IsRegLocalP(ctx, reg))
			{ obj->locals[j]->fxoffs=k; }
	}

	sctx->frm_offs_lcl=k;
	sctx->frm_offs_tmp=k;
#endif

#if 1
	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;

		j=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
			k&=~3; k-=4; obj->locals[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_WGR:
			k&=~1; k-=2; obj->locals[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
				{ k&=~7; k-=8; obj->locals[i]->fxoffs=k; break; }
			k&=~3; k-=4; obj->locals[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->is_addr64)
				{ k&=~7; k-=16; obj->locals[i]->fxoffs=k; break; }
			k&=~3; k-=8; obj->locals[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k&=~3; k-=4; obj->locals[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			if(sctx->is_addr64)
				{ k&=~7; }
			k&=~3; k-=8; obj->locals[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k&=~7; k-=8; obj->locals[i]->fxoffs=k;
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	sctx->frm_offs_lcl=k;

	for(i=0; i<obj->n_regs; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_VO_GR:
			k&=~3; k-=4; obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_WGR:
			k&=~1; k-=2; obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
				{ k&=~7; k-=8; obj->regs[i]->fxoffs=k; break; }
			k&=~3; k-=4; obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->is_addr64)
				{ k&=~7; k-=16; obj->regs[i]->fxoffs=k; break; }
			k&=~3; k-=8; obj->regs[i]->fxoffs=k;
			break;

		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k&=~3; k-=4; obj->regs[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			if(sctx->is_addr64)
				{ k&=~7; }
			k&=~3; k-=8; obj->regs[i]->fxoffs=k;
			break;
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;
			k&=~7; k-=8; obj->regs[i]->fxoffs=k;
			break;
		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	sctx->frm_offs_tmp=k;
#endif

	if(sctx->is_addr64)
		{ k&=~7; k-=obj->n_cargs*8; }
	else
		k-=obj->n_cargs*4;

//	k-=obj->n_cargs*4;
//	k-=obj->n_cargs*8;
//	k&=~15;
//	k&=~3;

//	if(sctx->is_addr64)
	if(1)
		{ k&=~15; }

	if((-k)>=60)
	{
		j=-1;
	}
	
	if(sctx->has_movi20)
	{
		if((-k)>=3840)
			{ k&=~255; sctx->use_bp=1; }
	}else
	{
//		if((-k)>=960)
		if((-k)>=1012)
			{ k&=~255; sctx->use_bp=1; }
	}
	
	sctx->frm_size=-k;
	
	return(0);
}

int BGBCC_JX2C_CalcFrameEpiKey(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj,
	int rqt, int *repik, int *repix)
{
	u64 uli;
	int epik, epix, epilbl;

	*repik=0;
	*repix=0;

	if(sctx->is_simpass)
		return(0);

	if(!sctx->has_bjx1jmp)
		return(0);

	if((sctx->is_leaf&1) && (rqt&1))
//	if(1)
	{
		return(0);
	}
	
	if(rqt&1)
	{
		if(sctx->is_leaf)
//		if(sctx->is_leaf&1)
			return(0);
			
//		return(0);
	}

	epik=	((sctx->reg_save >> 8)&0x000000FF)|
			((sctx->reg_save >>16)&0x0000FF00)|
			((sctx->freg_save<< 8)&0x00FF0000);
	if(sctx->use_fpr)
		epik|=0x01000000;
	if(!(sctx->is_leaf&1))
		epik|=0x02000000;
		
	uli=epik;
	uli=(uli+1)*65521;
	uli=(uli+1)*65521;
	uli=(uli+1)*251;
	epix=(uli>>32)&1023;

	/* Unusual Registers Saved */
	if(sctx->reg_save&0x00FF00FF)		{ epilbl=0; epik=0; epix=0; }
	if(sctx->freg_save&0xFFFF00FF)		{ epilbl=0; epik=0; epix=0; }

	/* Not enough registers saved */
	if((epik&0x70)!=0x70)
		{ epilbl=0; epik=0; epix=0; }
//	if((epik&0x60)!=0x60)
//		{ epilbl=0; epik=0; epix=0; }

	if(rqt&1)
	{
//		if((epik&0x78)!=0x78)
//			{ epilbl=0; epik=0; epix=0; }

//		if((epik&0x7C)!=0x7C)
//		if(epik && ((epik&0x7C)!=0x7C))
//		{
//			epilbl=0;
//			epik=0; epix=0;
//		}
	}

	*repik=epik;
	*repix=epix;
	return(0);
}

int BGBCC_JX2C_EmitFrameProlog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	ccxl_register reg, treg;
	ccxl_type tty;
	int bo, co, pr0;
	int p0, vaix, cnt;
	int ni, nf, rcls, ob, ov;
	int epik, epix, epilbl, epij;
	int i, j, k;

#if 1
	sctx->reg_save=sctx->reg_vsave;
	sctx->freg_save=sctx->freg_vsave;

	//	if((sctx->is_leaf&1) && ((sctx->reg_save&0x7E00)==0x7E00))
	//		sctx->is_leaf=0;

	if(sctx->is_leaf&1)
	{
		if((sctx->reg_save&0x7E00)==0x7E00)
			sctx->is_leaf=0;

		if(((sctx->reg_save&0x7800)==0x7800) &&
			((sctx->freg_save&0xC000)==0xC000))
				sctx->is_leaf=0;
		if(((sctx->reg_save&0x7000)==0x7000) &&
			((sctx->freg_save&0xE000)==0xE000))
				sctx->is_leaf=0;
		if(((sctx->reg_save&0x6000)==0x6000) &&
			((sctx->freg_save&0xF000)==0xF000))
				sctx->is_leaf=0;
	}

	BGBCC_JX2C_CalcFrameEpiKey(ctx, sctx, obj, 1, &epik, &epix);
//	epix=(((((u64)epik)+1)*2147483647LL)>>32)&1023;
	epilbl=sctx->eprhash_lbl[epix];
	
	if(sctx->eprhash_key[epix]!=epik)
		epilbl=0;
	
	epij=0;
	if(epilbl>0)
	{
		epij=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
		if(epij<=0)epilbl=0;
	}

	if(!sctx->is_simpass && epik && (epilbl<=0))
	{
		epij=3;

		epilbl=BGBCC_JX2_GenLabel(sctx);
		sctx->eprhash_key[epix]=epik;
		sctx->eprhash_lbl[epix]=epilbl;

		BGBCC_JX2_EmitLabel(sctx, epilbl);

		k=0;

#if 1
	//	for(i=14; i>0; i--)
		for(i=31; i>0; i--)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

			if(!(sctx->reg_vsave&((2LL<<i)-1)))
				break;
	//		if(!(sctx->reg_vsave&((2<<i)-1)))
	//			break;

			if(sctx->reg_vsave&(1<<i))
			{
				sctx->reg_save|=(1<<i);
				if(sctx->is_addr64)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
						BGBCC_SH_REG_R0+i);
				}else
				{
					BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_MOVL,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP);
				}
	//			k++;
				k+=sctx->is_addr64?2:1;
			}
		}

		if(sctx->use_fpr && !sctx->no_fpu)
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_FPSCR, BGBCC_SH_REG_SP);
//			k+=sctx->is_addr64?2:1;
		}

		for(i=15; i>0; i--)
		{
			if(sctx->freg_vsave&(1<<i))
			{
				sctx->freg_save|=(1<<i);
//				BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//					BGBCC_SH_REG_FR0+i, BGBCC_SH_REG_SP);
//				k++;
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
					BGBCC_SH_REG_FR0+i);
				k+=2;
			}
		}

//		if(sctx->reg_save || sctx->freg_save)
//		{
//			/* Hack prior op into RTS delay slot */
//			ob=BGBCC_JX2_EmitGetOffs(sctx);
//			ov=BGBCC_JX2_EmitGetOffsWord(sctx, ob-2);
//			BGBCC_JX2_EmitSetOffsWord(sctx, ob-2, 0x000B);
//			BGBCC_JX2_EmitWord(sctx, ov);
//		}else
//		if(sctx->has_bjx1jmp)
		if(1)
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
#endif
	}
#endif

	BGBCC_JX2_EmitLabel(sctx, fcnlbl);

	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	sctx->cur_fpscr=0;
	sctx->dfl_fpscr=0;

	sctx->reg_save=0;
	sctx->freg_save=0;
	
	sctx->sreg_held=0x00F4;
	sctx->sfreg_held=0x0FF0;

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	bo=BGBCC_JX2_EmitGetOffs(sctx);

#if 1
	sctx->regalc_save=0;
	sctx->regalc_live=0;
	sctx->regalc_dirty=0;

	sctx->fregalc_save=0;
	sctx->fregalc_live=0;
	sctx->fregalc_dirty=0;

	sctx->sreg_held=0x0000;
	sctx->sfreg_held=0x0000;
	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
	
	if(sctx->cnt_set_fp64>sctx->cnt_set_fp32)
//	if(0)
	{
		sctx->dfl_fpscr=BGBCC_SH_FPSCR_PR;
	}else
	{
		sctx->dfl_fpscr=0;
	}

//	sctx->cur_fpscr=0x1000;	
//	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

//	sctx->cur_fpscr|=0x1000;	
//	sctx->cur_fpscr=1;
//	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
	
//	sctx->vsp_rsv=0;
//	sctx->vsp_rsv=2;
//	sctx->vsp_rsv=3;
	sctx->vsp_rsv=4;
	
	if(sctx->use_egpr)
		sctx->vsp_rsv+=3;
	
	if(sctx->use_bp)sctx->vsp_rsv--;
	if(sctx->is_pic)sctx->vsp_rsv--;
	if(sctx->vsp_rsv<0)
		sctx->vsp_rsv=0;

	if(sctx->vspan_num<sctx->vsp_rsv)
		sctx->vsp_rsv=sctx->vspan_num;

	cnt=0;
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		cnt+=sctx->vspan[i]->cnt;
		if(cnt<(sctx->vsp_tcnt/16))
			{ sctx->vsp_rsv=i; break; }
		
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
//		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//				(k&BGBCC_REGFL_ALIASPTR))
		if(k&BGBCC_REGFL_ALIASPTR)
			{ sctx->vsp_rsv=i; break; }

		if(
			!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegArgP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ sctx->vsp_rsv=i; break; }

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
			BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVoidP(ctx, tty) ||
			BGBCC_CCXL_TypeRealP(ctx, tty))
			{ sctx->vsp_rsv=i; break; }

//		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegFloatP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
	}
#endif

	k=0;

#if 0
	if(!(sctx->is_leaf&1))
	{
		BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
			BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
//		k++;
		k+=sctx->is_addr64?2:1;
	}
#endif

//	j=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
//	epij=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
	if(epik && (epilbl>0) && (epij>0))
	{
#if 1
//		BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//			BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
		k+=sctx->is_addr64?2:1;
#endif

//		if((j&3)==3)
		if(0)
//		if((epij&3)>1)
		{
			BGBCC_JX2_EmitOpLabel(sctx,
				BGBCC_SH_NMID_BSR, epilbl);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

		}else
		{
			BGBCC_JX2_EmitOpFar16Label(sctx,
				BGBCC_SH_NMID_BSRN, epilbl);
		}

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

#if 1
	//	for(i=14; i>0; i--)
		for(i=31; i>0; i--)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;
			if(!(sctx->reg_vsave&((2LL<<i)-1)))
				break;
			if(sctx->reg_vsave&(1<<i))
				{ sctx->reg_save|=(1<<i); k+=sctx->is_addr64?2:1; }
		}
		if(sctx->use_fpr)
			{ k+=sctx->is_addr64?2:1; }
		for(i=15; i>0; i--)
		{
			if(sctx->freg_vsave&(1<<i))
				{ sctx->freg_save|=(1<<i); k++; }
		}
#endif
	}
	else
	{
#if 1
		if(!(sctx->is_leaf&1))
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=sctx->is_addr64?2:1;
		}
#endif

	//	for(i=14; i>0; i--)
		for(i=31; i>0; i--)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

			if(!(sctx->reg_vsave&((2LL<<i)-1)))
				break;
	//		if(!(sctx->reg_vsave&((2<<i)-1)))
	//			break;

			if(sctx->reg_vsave&(1<<i))
			{
				sctx->reg_save|=(1<<i);
//				if(sctx->is_addr64)
				if(1)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
						BGBCC_SH_REG_R0+i);
				}else
				{
					BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_MOVL,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP);
				}
	//			k++;
				k+=sctx->is_addr64?2:1;
			}
		}

		if(sctx->use_fpr && !sctx->no_fpu)
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_FPSCR, BGBCC_SH_REG_SP);
//			k+=sctx->is_addr64?2:1;
		}

		for(i=15; i>0; i--)
		{
			if(sctx->freg_vsave&(1<<i))
			{
				sctx->freg_save|=(1<<i);
//				BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//					BGBCC_SH_REG_FR0+i, BGBCC_SH_REG_SP);
//				k++;
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
					BGBCC_SH_REG_FR0+i);
				k+=2;
			}
		}
	}

	BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
//	BGBCC_JX2C_SetModeDqSet(ctx, sctx);
//	BGBCC_JX2C_ResetModeDqLocal(ctx, sctx);

//	BGBCC_JX2_EmitWord(sctx, 0x0858);	//Debug, Set Token
	
	sctx->frm_offs_save=sctx->frm_size-(k*4);

	p0=BGBCC_JX2_TryEmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		-sctx->frm_offs_save, BGBCC_SH_REG_SP);
	if(p0<=0)
	{
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, sctx->frm_offs_save);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	}

#if 0
	if(sctx->is_pic)
	{
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, -16, BGBCC_SH_REG_R12);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, BGBCC_SH_REG_R12);

		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitWord(sctx, 0xC700);
		BGBCC_JX2_EmitLoadRegLabelRel(sctx, BGBCC_SH_REG_R12, sctx->lbl_got, 0);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R12);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	}
#endif

//	BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);

	k=sctx->frm_offs_retstr;
	if(k)
	{
		if(sctx->is_addr64)
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
		else
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);
	}

	sctx->sreg_live|=0x00F000F0;

	ni=0; nf=0; vaix=-1;
	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ vaix=i; continue; }

		tty=obj->args[i]->type;
		treg.val=CCXL_REGTY_ARG|
			(((u64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, treg);
	
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
//		case BGBCC_SH_REGCLS_VO_GR:
			if(sctx->is_addr64)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
				if(ni>=8)
					break;
				k=obj->args[i]->fxoffs;
				if(ni>=4)
				{
					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_RD20+(ni-4));
					else
						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
							k, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					break;
				}
				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
				else
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k, BGBCC_SH_REG_RD4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
			else
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_GR:
			if(sctx->is_addr64)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
				if(ni>=8)
					break;
				k=obj->args[i]->fxoffs;
				if(ni>=4)
				{
					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
						BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
							treg, BGBCC_SH_REG_RD20+(ni-4));
					else
						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
							k, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					break;
				}
				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
				else
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k, BGBCC_SH_REG_RD4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
				BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
			else
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
//		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
				if(ni>=8)
					break;
				k=obj->args[i]->fxoffs;
				if(ni>=4)
				{
					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_RQ20+(ni-4));
					else
						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
							k, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					break;
				}
				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
				else
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k, BGBCC_SH_REG_RQ4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
			else
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_QGR:
//		case BGBCC_SH_REGCLS_QGR:
			if(sctx->is_addr64)
			{
				if((ni>=4) && !sctx->has_bjx1egpr)
					break;
				if(ni>=8)
					break;
				k=obj->args[i]->fxoffs;
				if(ni>=4)
				{
					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
						BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
							treg, BGBCC_SH_REG_RQ20+(ni-4));
					else
						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
							k, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					break;
				}
				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
				else
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k, BGBCC_SH_REG_RQ4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
				BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
			else
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->is_addr64)
			{
				if((ni>=3) && !sctx->has_bjx1egpr)
					break;
				if(ni>=7)
					break;
				k=obj->args[i]->fxoffs;
				if(ni>=4)
				{
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+0, BGBCC_SH_REG_RQ20+(ni-4)+0);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+8, BGBCC_SH_REG_RQ20+(ni-4)+1);
					ni+=2;
					break;
				}
				if(ni==3)
				{
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+0, BGBCC_SH_REG_RQ7);
					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
						k+8, BGBCC_SH_REG_RQ20);
					ni+=2;
					break;
				}
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k+0, BGBCC_SH_REG_RQ4+ni+0);
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
					k+8, BGBCC_SH_REG_RQ4+ni+1);
				ni+=2;
				break;
			}

			if(ni>=3)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k, BGBCC_SH_REG_R4+ni);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_R4+ni+1);
			ni+=2;
			break;
		case BGBCC_SH_REGCLS_FR:
			if(nf>=4)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_FR4+nf);
			nf++;
			break;
		case BGBCC_SH_REGCLS_FR2:
			nf+=nf&1;
			if(nf>=7)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_FR4+nf+0);
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
				k+0, BGBCC_SH_REG_FR4+nf+1);
			nf+=2;
			break;
		case BGBCC_SH_REGCLS_DR:
			if(nf>=4)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_DR4+nf);
			nf++;
			break;
		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	if(vaix<0)
		vaix=obj->n_args;

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;

		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);

		if((rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF))
		{
			j=obj->locals[i]->fxmoffs+(sctx->frm_offs_fix);
			k=obj->locals[i]->fxoffs;

			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R3);
			if(sctx->is_addr64)
			{
				BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, j, BGBCC_SH_REG_RQ3);
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ3);
			}else
			{
				BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, j, BGBCC_SH_REG_R3);
				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R3);
			}
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R3);
		}
		
		if(BGBCC_CCXL_TypeVaListP(ctx, obj->locals[i]->type) && (vaix>0))
		{
			if(sctx->is_addr64)
			{
				for(j=0; j<4; j++)
				{
					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//					if(sctx->has_bjx1mov && !sctx->no_ext32)
//					if(0)
					if(1)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_RQ4+j, BGBCC_SH_REG_RQ3, (j+0)*8);
					}else
					{
						BGBCC_JX2C_ScratchSafeStompReg(
							ctx, sctx, BGBCC_SH_REG_R0);
						BGBCC_JX2_EmitLoadRegImm(sctx,
							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, j*8);
						BGBCC_JX2_EmitOpRegStReg2(sctx,
							BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ4+j,
							BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
						BGBCC_JX2C_ScratchReleaseReg(
							ctx, sctx, BGBCC_SH_REG_R0);
					}
				}

				if(sctx->has_bjx1egpr)
				{
					for(j=0; j<4; j++)
					{
						BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//						if(sctx->has_bjx1mov && !sctx->no_ext32)
//						if(0)
						if(1)
						{
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_MOVQ,
								BGBCC_SH_REG_RQ20+j,
								BGBCC_SH_REG_RQ3, (j+4)*8);
						}else
						{
							BGBCC_JX2C_ScratchSafeStompReg(
								ctx, sctx, BGBCC_SH_REG_R0);
							BGBCC_JX2_EmitLoadRegImm(sctx,
								BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, (j+4)*8);
							BGBCC_JX2_EmitOpRegStReg2(sctx,
								BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ20+j,
								BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
							BGBCC_JX2C_ScratchReleaseReg(
								ctx, sctx, BGBCC_SH_REG_R0);
						}
					}
				}else
				{
#if 0
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, 0x55AA55AA);
					for(j=0; j<4; j++)
					{
						BGBCC_JX2_EmitLoadRegImm(sctx,
							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, (j+4)*8);
						BGBCC_JX2_EmitOpRegStReg2(sctx,
							BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ1,
							BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
					}
#endif
				}

				if(!sctx->no_fpu)
				{
					for(j=0; j<4; j++)
					{
						BGBCC_JX2_EmitOpRegStRegDisp(sctx,
							BGBCC_SH_NMID_FMOVD,
							BGBCC_SH_REG_DR4+j, BGBCC_SH_REG_RQ3, 64+(j*8));
					}
					
					
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);

#if 0
					for(j=0; j<8; j++)
					{
						if(sctx->has_bjx1mov && !sctx->no_ext32)
	//					if(0)
						{
							BGBCC_JX2_EmitOpRegStRegDisp(sctx,
								BGBCC_SH_NMID_FMOVS,
								BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_RQ3, 64+(j*4));
						}else
						{
							BGBCC_JX2C_ScratchSafeStompReg(
								ctx, sctx, BGBCC_SH_REG_R0);
							BGBCC_JX2_EmitLoadRegImm(sctx,
								BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 64+(j*4));
							BGBCC_JX2_EmitOpRegStReg2(sctx,
								BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR4+j,
								BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
							BGBCC_JX2C_ScratchReleaseReg(
								ctx, sctx, BGBCC_SH_REG_R0);
						}
					}
#endif
				}

//				BGBCC_JX2_EmitLoadRegImm(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 96);
//				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
//					BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

//				for(j=7; j>=0; j--)
//				{
//					BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//						BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_R0);
//				}

//				if(sctx->has_bjx1mov && !sctx->no_ext32)
//				if(0)
				if(1)
				{
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, ni*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0,
						BGBCC_SH_REG_R3, 96);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 64+nf*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0,
						BGBCC_SH_REG_R3, 104);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_R15, BGBCC_SH_REG_R1);

					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 112);
				}else
				{
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7, ni*8);
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 96);
					BGBCC_JX2_EmitOpRegStReg2(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7,
						BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7, 64+nf*4);
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 104);
					BGBCC_JX2_EmitOpRegStReg2(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7,
						BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_R15, BGBCC_SH_REG_R1);

					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 112);
					BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R1, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
				}

//				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
			}else
			{
				for(j=0; j<4; j++)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
						BGBCC_SH_REG_R4+j, BGBCC_SH_REG_R3, j*4);
				}

				if(!sctx->no_fpu)
				{
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 48);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

//					for(j=7; j>=0; j--)
					for(j=3; j>=0; j--)
					{
						BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVD,
							BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_R0);
					}
				}

				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, ni*4);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 48);

				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 16+nf*4);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 52);

#if 1
				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
					BGBCC_SH_REG_R15, BGBCC_SH_REG_R1);
#endif

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 56);
			}
		}
	}

	sctx->sreg_live&=~0x00F000F0;

#if 0
	sctx->regalc_save=0;
	sctx->regalc_live=0;
	sctx->regalc_dirty=0;

	sctx->fregalc_save=0;
	sctx->fregalc_live=0;
	sctx->fregalc_dirty=0;

	sctx->sreg_held=0x0000;
	sctx->sfreg_held=0x0000;
	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
	
	if(sctx->cnt_set_fp64>sctx->cnt_set_fp32)
//	if(0)
	{
		sctx->dfl_fpscr=BGBCC_SH_FPSCR_PR;
	}else
	{
		sctx->dfl_fpscr=0;
	}

//	sctx->cur_fpscr=0x1000;	
//	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);

//	sctx->cur_fpscr|=0x1000;	
//	sctx->cur_fpscr=1;
//	BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
	
//	sctx->vsp_rsv=0;
//	sctx->vsp_rsv=2;
//	sctx->vsp_rsv=3;
	sctx->vsp_rsv=4;
	if(sctx->use_bp)sctx->vsp_rsv--;
	if(sctx->is_pic)sctx->vsp_rsv--;
	if(sctx->vsp_rsv<0)
		sctx->vsp_rsv=0;

	if(sctx->vspan_num<sctx->vsp_rsv)
		sctx->vsp_rsv=sctx->vspan_num;

	cnt=0;
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		cnt+=sctx->vspan[i]->cnt;
		if(cnt<(sctx->vsp_tcnt/16))
			{ sctx->vsp_rsv=i; break; }
		
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
//		if(BGBCC_CCXL_IsRegGlobalP(ctx, reg) ||
//			BGBCC_CCXL_IsRegArgP(ctx, reg) ||
//				(k&BGBCC_REGFL_ALIASPTR))
		if(k&BGBCC_REGFL_ALIASPTR)
			{ sctx->vsp_rsv=i; break; }

		if(
			!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ sctx->vsp_rsv=i; break; }

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
			BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVoidP(ctx, tty) ||
			BGBCC_CCXL_TypeRealP(ctx, tty))
			{ sctx->vsp_rsv=i; break; }

//		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegFloatP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
	}
#endif


#if 0
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		reg=sctx->vspan[i].reg;
		BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, reg);
//		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, reg);
	}
#endif

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_pro=co-bo;
	
	return(0);
}

int BGBCC_JX2C_EmitFrameEpilog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	ccxl_type tty;
	u64 uli;
	int epik, epix, epilbl;
	int bo, co;
	int ob, ov;
	int tr0, tr1;
	int p0, tsz;
	int i, j, k;

	bo=BGBCC_JX2_EmitGetOffs(sctx);

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_ACCESSED)
			{ obj->locals[i]->regflags&=~BGBCC_REGFL_CULL; }
		else
			{ obj->locals[i]->regflags|=BGBCC_REGFL_CULL; }
	}

	BGBCC_JX2_EmitCheckFlushIndexImm(sctx);

	if(sctx->lbl_ret)
	{
//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret);
	}

	sctx->sreg_held=0x0003;
	sctx->sfreg_held=0x0003;

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);

	if(BGBCC_CCXL_TypeVoidP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
		BGBCC_CCXL_TypePointerP(ctx, tty) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{ sctx->sreg_held=0x0001; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
		BGBCC_CCXL_TypeVariantP(ctx, tty))
		{ sctx->sreg_held=0x0003; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0001; }
	if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0003; }

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	k=sctx->frm_offs_retstr;
	if(k)
	{
		tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);

		if(sctx->is_addr64)
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ0, tsz, 4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
		}else
		{
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_R2, BGBCC_SH_REG_R0, tsz, 4);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
		}
	}

//	BGBCC_JX2C_ResetFpscrDefaults(ctx, sctx);

	BGBCC_JX2C_CalcFrameEpiKey(ctx, sctx, obj, 2, &epik, &epix);
//	epix=(((((u64)epik)+1)*2147483647LL)>>32)&1023;
	epilbl=sctx->epihash_lbl[epix];

#if 1
	k=sctx->frm_offs_save;
	p0=BGBCC_JX2_TryEmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		k, BGBCC_SH_REG_SP);
	if(p0<=0)
	{
		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, tr0, k);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, tr0, BGBCC_SH_REG_SP);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
	}

	j=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl);
	if(epik && (sctx->epihash_key[epix]==epik) && (epilbl>0) &&
		!sctx->is_simpass && j)
	{
		if((j&3)==3)
		{
			BGBCC_JX2_EmitOpLabel(sctx,
				BGBCC_SH_NMID_BRAN, epilbl);
		}else
		{
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, epilbl);
			BGBCC_JX2_EmitOpFar16Label(sctx,
				BGBCC_SH_NMID_BRAN, epilbl);
		}

		BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
		BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

		if(sctx->is_leaf==2)
			sctx->is_leaf=1;

		co=BGBCC_JX2_EmitGetOffs(sctx);
		sctx->fnsz_epi=co-bo;

		return(0);
	}

#if 1
	if(!sctx->is_simpass && epik)
	{
		epilbl=BGBCC_JX2_GenLabel(sctx);
		BGBCC_JX2_EmitLabel(sctx, epilbl);

		sctx->epihash_key[epix]=epik;
		sctx->epihash_lbl[epix]=epilbl;
	}
#endif

//	BGBCC_JX2_EmitWord(sctx, 0x0848);	//Debug, Clear Token

#if 1
	for(i=0; i<16; i++)
	{
		if(sctx->freg_save&(1<<i))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
				BGBCC_SH_REG_FR0+i);
			k+=8;

//			BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
//				BGBCC_SH_REG_SP, BGBCC_SH_REG_FR0+i);
//			k+=4;
		}
	}

	if(sctx->use_fpr && !sctx->no_fpu)
	{
//		BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_LDSL,
//			BGBCC_SH_REG_SP, BGBCC_SH_REG_FPSCR);
//		k+=sctx->is_addr64?8:4;
	}
#endif

//	for(i=0; i<15; i++)
	for(i=0; i<32; i++)
	{
		if(!(sctx->has_bjx1egpr) && (i>=16))
			continue;
		if(i==15)
			continue;

		if(sctx->reg_save&(1<<i))
		{
			if(sctx->is_addr64)
//			if(1)
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
					BGBCC_SH_REG_RQ0+i);
				k+=8;
			}else
			{
//				BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_MOVL,
//					BGBCC_SH_REG_SP, BGBCC_SH_REG_R0+i);
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
					BGBCC_SH_REG_R0+i);
				k+=4;
			}
		}
	}

	if(!(sctx->is_leaf&1))
	{
		if(sctx->is_addr64)
			k+=8;
		else
			k+=4;
	}
	
	if((k!=sctx->frm_size) && !sctx->is_simpass)
		{ BGBCC_DBGBREAK; }
#endif


	if(!(sctx->is_leaf&1))
	{

		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PC);

#if 0
//		BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_LDSL,
//			BGBCC_SH_REG_SP, BGBCC_SH_REG_PR);
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PR);

//		if(sctx->has_movi20)
		if(1)
//		if(sctx->has_bjx1jmp)
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
#endif

	}else
	{
//		if(sctx->reg_save || sctx->freg_save)
		if(0)
		{
			/* Hack prior op into RTS delay slot */
			ob=BGBCC_JX2_EmitGetOffs(sctx);
			ov=BGBCC_JX2_EmitGetOffsWord(sctx, ob-2);
			BGBCC_JX2_EmitSetOffsWord(sctx, ob-2, 0x000B);
			BGBCC_JX2_EmitWord(sctx, ov);
		}else
//			if(sctx->has_movi20)
//			if(0)
		if(1)
//		if(sctx->has_bjx1jmp)
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
	}

//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

#if 0
	ob=BGBCC_JX2_EmitGetOffs(ctx);
	ov=BGBCC_JX2_EmitGetOffsWord(ctx, ob);
	BGBCC_JX2_EmitSetOffsWord(ctx, ob, ov);
#endif

	BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
//	BGBCC_JX2_EmitFlushIndexImm16(sctx);
//	BGBCC_JX2_EmitFlushIndexImm32(sctx);
	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	if(sctx->is_leaf==2)
		sctx->is_leaf=1;
//	sctx->is_leaf=0;

	co=BGBCC_JX2_EmitGetOffs(sctx);
	sctx->fnsz_epi=co-bo;

	return(0);
}
