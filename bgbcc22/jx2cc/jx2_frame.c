
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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&31);
	
//		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
//			BGBCC_SH_REG_SP, ofs1+4, dreg2+1);
//		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
//			BGBCC_SH_REG_SP, ofs1+0, dreg2+0);

		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs1&7))
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
				BGBCC_SH_REG_SP, ofs1, dreg2);
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, ofs1+0, dreg2+0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, ofs1+8, dreg2+1);
		}

		return(1);
	}

	nmid=BGBCC_SH_NMID_MOVL;

//	if(sctx->is_addr64 && sctx->is_addr_x32)
	if(0)
	{
		if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			nmid=BGBCC_SH_NMID_MOVUL;
		if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			nmid=BGBCC_SH_NMID_MOVDL;
	}

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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&31);
	
		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs1&7))
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
				dreg2, BGBCC_SH_REG_SP, ofs1);
		}else
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+1, BGBCC_SH_REG_SP, ofs1+8);
		}
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

#if 0
	if(sctx->is_addr64)
	{
		if(dreg<32)
		{
			BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
			nmid=BGBCC_SH_NMID_MOVQ;
		}
	}
#endif

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
	int ofs1, nm1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	if(!ofs1)
	{
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_SP, dreg);
		return(1);
	}

	nm1=BGBCC_SH_NMID_LEAB;
	if(ofs1>=512)
	{
		if(!(ofs1&1))
			{ nm1=BGBCC_SH_NMID_LEAW; }
		if(!(ofs1&3))
			{ nm1=BGBCC_SH_NMID_LEAL; }
		if(!(ofs1&7))
			{ nm1=BGBCC_SH_NMID_LEAQ; }
	}

#if 0
	if((ofs1<512) && (ofs1>(-512)))
	{
//		nm1=BGBCC_SH_NMID_ADD;
		BGBCC_JX2_EmitOpRegImmReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_SP, ofs1, dreg);

//		BGBCC_JX2_EmitOpLdRegDispReg(sctx,
//			BGBCC_SH_NMID_LEAB, BGBCC_SH_REG_SP, ofs1, dreg);
		return(1);
	}
#endif

//	BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//		dreg, ofs1);
//	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
//		BGBCC_SH_REG_SP, dreg);
	BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1,
		BGBCC_SH_REG_SP, ofs1, dreg);
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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&31);
	
		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs1&7))
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
				dreg2, BGBCC_SH_REG_SP, ofs1);
			return(1);
		}else
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+1, BGBCC_SH_REG_SP, ofs1+8);
			return(1);
		}

		BGBCC_DBGBREAK

#if 0
		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			dreg2+1, BGBCC_SH_REG_SP, ofs1+8);
		if(p0>0)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+0, BGBCC_SH_REG_SP, ofs1+0);
			return(1);
		}

		BGBCC_DBGBREAK
#endif

#if 0
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, ofs1);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
//			dreg2+0, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
//			dreg2+1, BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
//		return(1);
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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&31);

		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs&7) &&
			(nmid!=BGBCC_SH_NMID_MOVQ))
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
				breg, ofs, dreg2);
			return(1);
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				breg, ofs+0, dreg2+0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				breg, ofs+8, dreg2+1);
			return(1);
		}


#if 0
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
#endif

		BGBCC_DBGBREAK

#if 0
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, ofs);
//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//			breg, BGBCC_SH_REG_R0, dreg2+0);
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//			breg, BGBCC_SH_REG_R0, dreg2+1);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
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
//			BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg, breg);
			return(1);
		}

		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, nmid,
			dreg, breg, ofs);
		if(p0>0)
			return(1);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, ofs);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
//			dreg, breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//		return(1);
	}
#endif

#if 0
	if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_FR0+(dreg&7)*2;
//		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
		dreg2=BGBCC_SH_REG_FR0+(dreg&31);
	
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, ofs);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
//			dreg2+1, breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
//			dreg2+0, breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//		return(1);
	}
#endif

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&31);

//		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs&7))
		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs&7) &&
			(nmid!=BGBCC_SH_NMID_MOVQ))
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
				dreg2, breg, ofs);
			return(1);
		}else
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+0, breg, ofs+0);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				dreg2+1, breg, ofs+8);
			return(1);
		}

#if 0
		p0=BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
			dreg2+1, breg, ofs+4);
		if(p0>0)
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				dreg2+0, breg, ofs+0);
			return(1);
		}

		BGBCC_DBGBREAK
#endif

#if 0
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, ofs);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
//			dreg2+0, breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
//			dreg2+1, breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
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

#if 0
//	if((ctx->arch_sizeof_ptr==8) || (breg!=dreg))
	if(sctx->is_addr64 || (breg!=dreg))
	{
		if((((sbyte)ofs)==ofs) && (breg==dreg) && ((breg&31)<16))
		{
			i=BGBCC_JX2_TryEmitOpImmReg(sctx,
				BGBCC_SH_NMID_ADD, ofs, dreg);
			if(i>0)
				return(1);
		}

#if 0
		opw1=BGBCC_SH_NMID_LEAB;	k=ofs;
		if(!(ofs&1))
			{ opw1=BGBCC_SH_NMID_LEAW;	k=ofs>>1; }
		if(!(ofs&3))
			{ opw1=BGBCC_SH_NMID_LEAL;	k=ofs>>2; }
		if(!(ofs&7))
			{ opw1=BGBCC_SH_NMID_LEAQ;	k=ofs>>3; }
//		if(!(ofs&15))
//			{ opw1=BGBCC_SH_NMID_LEAO;	k=ofs>>4; }
#endif

		opw1=BGBCC_SH_NMID_ADD;	k=ofs;

		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			opw1, breg, k, dreg);
//			opw1, breg, ofs, dreg);
		if(i>0)
			return(1);

//		BGBCC_DBGBREAK
	}
#endif

	if(breg!=dreg)
	{
		opw1=BGBCC_SH_NMID_LEAB;
		if(ofs>=512)
		{
			if(!(ofs&1))
				opw1=BGBCC_SH_NMID_LEAW;
			if(!(ofs&3))
				opw1=BGBCC_SH_NMID_LEAL;
			if(!(ofs&7))
				opw1=BGBCC_SH_NMID_LEAQ;
		}

		BGBCC_JX2_EmitOpLdRegDispReg(sctx,
			opw1, breg, ofs, dreg);
		return(1);

#if 0
		i=BGBCC_JX2_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, breg, ofs, dreg);
		if(i>0)
			return(1);

		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, ofs);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, dreg);
#endif
	}else
	{	
		opw1=BGBCC_SH_NMID_LEAB;
		if(ofs>=512)
		{
			if(!(ofs&1))
				opw1=BGBCC_SH_NMID_LEAW;
			if(!(ofs&3))
				opw1=BGBCC_SH_NMID_LEAL;
			if(!(ofs&7))
				opw1=BGBCC_SH_NMID_LEAQ;
		}

		BGBCC_JX2_EmitOpLdRegDispReg(sctx,
			opw1, breg, ofs, dreg);
		return(1);

#if 0
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
#endif
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
	case BGBCC_SH_NMID_MOVDL: rt=(sc==4); break;

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
	int tr0, tr1;
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
		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
		}
	
		i=BGBCC_JX2_TryEmitOpLdReg2Reg(sctx, nmid,
			breg, ireg, dreg);
		if(i>0)
			return(1);
	}

	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg)) &&
		(sc==16)
		)
	{
//		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
//		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
//		}

		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);

#if 1
		if((nmid==BGBCC_SH_NMID_MOVX2) && !(dreg2&1))
		{
			tr0=dreg2;
			BGBCC_JX2_EmitOpRegImmReg(sctx,
				BGBCC_SH_NMID_SHAD, ireg, 1, tr0);
			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVX2,
				breg, tr0, dreg2);
			return(1);
		}
#endif


		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHAD, ireg, 4, tr0);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, tr0);
		
		if((nmid==BGBCC_SH_NMID_MOVX2) && !(dreg2&1))
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
				tr0, 0, dreg2);
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				tr0, 0, dreg2+0);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				tr0, 8, dreg2+1);
		}
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

		return(1);

//		i=BGBCC_JX2_TryEmitOpLdReg2Reg(sctx, nmid,
//			breg, ireg, dreg);
//		if(i>0)
//			return(1);
	}

	BGBCC_DBGBREAK

#if 0
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

//	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//		ireg, BGBCC_SH_REG_R0);
//	if(nm1>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
//	if(nm2>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
//			breg, BGBCC_SH_REG_R0, dreg2+(sw^0));
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
//			breg, BGBCC_SH_REG_R0, dreg2+(sw^1));
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

//	BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
//		breg, BGBCC_SH_REG_R0, dreg);
//	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif

	return(1);
}

int BGBCC_JX2C_EmitStoreBRegIRegScReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int dreg2, sw;
	int nm1, nm2;
	int tr0, tr1;
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
		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
		}
	
		i=BGBCC_JX2_TryEmitOpRegStReg2(sctx, nmid,
			dreg, ireg, breg);
		if(i>0)
			return(1);
	}
#endif

	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg)) &&
		(sc==16)
		)
	{
		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHAD, ireg, 4, tr0);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD, breg, tr0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			dreg2+0, tr0, 0);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			dreg2+1, tr0, 8);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);

		return(1);
	}

	BGBCC_DBGBREAK

#if 0
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

//	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//		ireg, BGBCC_SH_REG_R0);
//	if(nm1>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
//	if(nm2>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

//		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
//			dreg2+(sw^0), breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
//			dreg2+(sw^1), breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

//	BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
//		dreg, BGBCC_SH_REG_R0, breg);
//	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif

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
		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
		}
	
		i=BGBCC_JX2_TryEmitOpLdReg2DispReg(sctx, nmid,
			breg, ireg, disp, dreg);
		if(i>0)
			return(1);
	}

	BGBCC_DBGBREAK

#if 0
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

//	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//		ireg, BGBCC_SH_REG_R0);
//	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//		disp/sc, BGBCC_SH_REG_R0);
//	if(nm1>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
//	if(nm2>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
//			breg, BGBCC_SH_REG_R0, dreg2+(sw^0));
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
//			breg, BGBCC_SH_REG_R0, dreg2+(sw^1));
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

//	BGBCC_JX2_EmitOpLdReg2Reg(sctx, nmid,
//		breg, BGBCC_SH_REG_R0, dreg);
//	BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif

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
		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
		}
	
		i=BGBCC_JX2_TryEmitOpRegStReg2Disp(sctx, nmid,
			dreg, ireg, breg, disp);
		if(i>0)
			return(1);
	}
#endif

	BGBCC_DBGBREAK

#if 0
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

//	BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//	BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//		ireg, BGBCC_SH_REG_R0);
//	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//		disp/sc, BGBCC_SH_REG_R0);
//	if(nm1>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
//	if(nm2>=0)
//		BGBCC_JX2_EmitOpReg(sctx, nm2, BGBCC_SH_REG_R0);

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		dreg2=BGBCC_JX2C_RegNormalizeReg(ctx, sctx, dreg);
//		sw=BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2);
//		sw=(sctx->is_le!=0);
		sw=(sctx->is_le!=0)^(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg2)==0);

//		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
//			dreg2+(sw^0), breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			4, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
//			dreg2+(sw^1), breg, BGBCC_SH_REG_R0);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
//		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
		{ BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx); }
	if((nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
		{ BGBCC_JX2C_CheckSetModeDqClear(ctx, sctx); }

//	BGBCC_JX2_EmitOpRegStReg2(sctx, nmid,
//		dreg, BGBCC_SH_REG_R0, breg);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif

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
	int i, j;

	gbl=ctx->reg_globals[gblidx];

	BGBCC_CCXL_GlobalMarkReachable(ctx, gbl);

	i=gbl->fxoffs;
	if(i<=0)
	{
#if 0
		if(!bgbcc_strncmp2(gbl->name, "__"))
		{
			if(!bgbcc_strncmp7(gbl->name, "__arch_"))
			{
				j=BGBCC_JX2A_GetRegId(gbl->name+7);
				if(j>=0)
					{ i=BGBCC_SH_LBL_ARCHREG(j); }
			}
		}
#endif

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
	
//	if(sctx->is_pic)
	if(0)
	{
		BGBCC_DBGBREAK

#if 0
//		k=BGBCC_JX2C_GetGblNameGotIndex(ctx, sctx, name);
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, k*4);
//		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//			BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
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

		if(!(ctx->cur_func->regs[j]->regflags&BGBCC_REGFL_TEMPLOAD))
		{
			printf("Mark Tempload\n");
		}

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_TEMPLOAD;

		k=ctx->cur_func->regs[j]->fxoffs;

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
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);

#if 0
//		k=ctx->cur_func->regs[j]->fxoffs;

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);

		BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
		return(1);
#endif
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
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
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
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
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

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, dreg);
		return(i);

#if 0
//		k=ctx->cur_func->regs[j]->fxoffs;

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2C_EmitLoadFrameVRegReg(ctx, sctx, sreg, tr0);

		BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
			BGBCC_SH_NMID_MOVL, tr0, 0, dreg);

		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
#endif
		return(1);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
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
	ccxl_register tvreg;
	ccxl_type tty;
	char *s0;
	double f, g;
	float sf;
	u16 usk;
	s64 li, lj;
	int dreg2, treg, rcls;
	int nm1, nm2, nm3;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
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

//		if(ctx->cur_func->locals[j]->regflags&BGBCC_REGFL_ALIASPTR)
		if(1)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
			{
				if(sctx->has_fmovs)
//				if(0)
				{
					k=ctx->cur_func->locals[j]->fxoffs;
					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_FMOVS,
						BGBCC_SH_REG_SP, k, dreg);
					return(1);
				}

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FLDCF, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_LDHF16, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_LDBF16, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}
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

//		if((rcls==BGBCC_SH_REGCLS_GR) && sctx->is_addr64)
		if(((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR)) && sctx->is_addr64)
		{
			if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);
			if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
				dreg=BGBCC_SH_REG_RD16|(dreg&15);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
		}

//		k=sctx->frm_offs_lcl+j*4;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
		j=sreg.val&CCXL_REGID_BASEMASK;
		tty=ctx->cur_func->regs[j]->type;
		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			k=1;
		}

		if(!(ctx->cur_func->regs[j]->regflags&BGBCC_REGFL_TEMPLOAD))
		{
			printf("Mark Tempload\n");
		}

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_TEMPLOAD;

//		if(ctx->cur_func->regs[j]->regflags&BGBCC_REGFL_ALIASPTR)
		if(1)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
			{
				if(sctx->has_fmovs)
//				if(0)
				{
					k=ctx->cur_func->regs[j]->fxoffs;
					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_FMOVS,
						BGBCC_SH_REG_SP, k, dreg);
					return(1);
				}

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->regs[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FLDCF, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->regs[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_LDHF16, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->regs[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_LDBF16, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}
		}

		rcls=ctx->cur_func->regs[j]->regcls;

		if((rcls==BGBCC_SH_REGCLS_VO_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR2) ||
			(rcls==BGBCC_SH_REGCLS_VO_QGR2) ||
			(rcls==BGBCC_SH_REGCLS_VO_QGR))
		{
			k=ctx->cur_func->regs[j]->fxoffs;
			i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

		rcls=ctx->cur_func->regs[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			dreg=BGBCC_SH_REG_RD0|(dreg&15);

//		if((rcls==BGBCC_SH_REGCLS_GR) && sctx->is_addr64)
		if(((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR)) && sctx->is_addr64)
		{
			if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);
			if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
				dreg=BGBCC_SH_REG_RD16|(dreg&15);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
		}

		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
		j=sreg.val&CCXL_REGID_BASEMASK;
		tty=ctx->cur_func->args[j]->type;

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

//		if(ctx->cur_func->args[j]->regflags&BGBCC_REGFL_ALIASPTR)
		if(1)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
			{
				if(sctx->has_fmovs)
//				if(0)
				{
					k=ctx->cur_func->args[j]->fxoffs;
					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_FMOVS,
						BGBCC_SH_REG_SP, k, dreg);
					return(1);
				}

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->args[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FLDCF, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->args[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_LDHF16, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->args[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_LDBF16, treg, dreg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}
		}

		rcls=ctx->cur_func->args[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			dreg=BGBCC_SH_REG_RD0|(dreg&15);

//		if((rcls==BGBCC_SH_REGCLS_GR) && sctx->is_addr64)
		if(((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR)) && sctx->is_addr64)
		{
			if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
				dreg=BGBCC_SH_REG_RD0|(dreg&15);
			if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
				dreg=BGBCC_SH_REG_RD16|(dreg&15);
		}

		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
		}

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
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_FMOV, dreg, k);

			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			f=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg, k, j);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
			BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);
			return(1);
		}
	
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmUIntP(ctx, sreg))
	{
		if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			*(f32 *)(&k)=li;
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_FMOV, dreg, k);

			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			f=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg, k, j);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
			BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);
			return(1);
		}
	
//		j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
		if(sctx->is_addr64)
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);
		else
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, (s32)li);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
	{
		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
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
//			dreg2=BGBCC_SH_REG_FR0+(dreg&15);
			dreg2=BGBCC_SH_REG_FR0+(dreg&31);

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
			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, k);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_DR0+(dreg&31);

			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
			j=(*(u64 *)(&f))>>32;
			k=(*(u64 *)(&f));
			BGBCC_JX2_EmitLoadRegImmPair32(sctx, dreg2, k, j);
			return(1);
		}

		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) ||
			BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);

			li=*(u64 *)(&f);
			*(float *)(&j)=f;
			
			k=1;
			if(li==0)k=0;

			if(!sctx->is_fixed32 && !sctx->op_is_wex2)
			{
				if(((li>>52)<<52)==li)k=0;
			}

			if(sctx->is_fixed32 || sctx->op_is_wex2)
			{
				if(((li>>48)<<48)==li)k=0;
			}

//			if((li!=0) && ((*(float *)(&j))==f) && sctx->fpu_gfp)
			if(k && ((*(float *)(&j))==f) && sctx->fpu_gfp)
			{
				i=BGBCC_JX2_ConstConvFloatToHalf(j, &usk);
				if(i>0)
				{
					BGBCC_JX2C_EmitOpImmReg(ctx, sctx,
						BGBCC_SH_NMID_FLDCH,
						usk, dreg);
					return(1);

#if 0
					BGBCC_JX2_EmitLoadDrImm(sctx, usk);
					BGBCC_JX2C_EmitOpRegReg(ctx, sctx,
						BGBCC_SH_NMID_FLDCH,
						BGBCC_SH_REG_R0, dreg);
					return(1);
#endif
				}
			}

//			li=*(u64 *)(&f);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg, li);
			return(1);
		}
		
		if(!(dreg>>4) || sctx->fpu_gfp)
		{
			BGBCC_DBGBREAK
		}
	
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
		*(float *)(&j)=f;
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmStringP(ctx, sreg))
	{
		s0=BGBCC_CCXL_GetRegImmStringValue(ctx, sreg);
//		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
//		k=BGBCC_JX2_EmitGetStrtabLabelUTF2ASCII(sctx, s0);

		if(BGBCC_CCXL_IsRegImmU8StringP(ctx, sreg))
			k=BGBCC_JX2_EmitGetStrtabLabelUTF8(sctx, s0);
		else
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

	if(BGBCC_CCXL_IsRegImmW4StringP(ctx, sreg))
	{
		s0=BGBCC_CCXL_GetRegImmStringValue(ctx, sreg);
//		k=BGBCC_JX2_EmitGetStrtabLabel(sctx, s0);
		k=BGBCC_JX2_EmitGetStrtabLabelUTF2UCS4(sctx, s0);

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
//		sctx->is_leaftiny|=8;
//		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
//		if(ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR)
		if(sctx->is_pbo && (ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR))
//		if(1)
		{
#if 0
//			if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
			if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY))
				printf("Reject LeafTiny PBO+Alias %s\n",
					ctx->cur_func->qname);
#endif

			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}

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
//			BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
//				BGBCC_SH_NMID_FMOVS, treg, 0, dreg);
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
				nm1=BGBCC_SH_NMID_MOVL;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUL;
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, dreg, k);
				return(1);
			}

			if(BGBCC_CCXL_TypeSgLongP(ctx, tty) && sctx->is_addr64)
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVQ, dreg, k);
				return(1);
			}

			if(BGBCC_CCXL_TypeSmallIntP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUL;

				if(BGBCC_CCXL_TypeSignedByteP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVB;
				if(BGBCC_CCXL_TypeSignedShortP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVW;
				if(BGBCC_CCXL_TypeUnsignedByteP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUB;
				if(BGBCC_CCXL_TypeUnsignedShortP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUW;
				if(BGBCC_CCXL_TypeUnsignedIntP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUL;

				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, dreg, k);
				return(1);
			}

//			if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
//			{
//				k=-1;
//			}

#if 1
//			if((BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
//				BGBCC_CCXL_TypePointerP(ctx, tty)) &&
//				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			if( BGBCC_CCXL_TypePointerP(ctx, tty) &&
				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			{
//				nm1=BGBCC_SH_NMID_MOVL;
//				if(sctx->is_addr64)
//				if(ctx->arch_sizeof_ptr==8)
//				{
//					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//					nm1=BGBCC_SH_NMID_MOVQ;
//				}
//				else if(sctx->is_addr64)
//				{
//					nm1=BGBCC_SH_NMID_MOVUL;
//				}
				nm1=BGBCC_SH_NMID_MOVP;
			
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					nm1, dreg, k);
				return(1);
			}

			if(BGBCC_CCXL_TypeFunctionP(ctx, tty))
			{
				k=-1;
			}
			
			if(BGBCC_CCXL_TypeSgNLongP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUL;
				if(ctx->arch_sizeof_long==8)
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
#endif

#if 1
			if(BGBCC_CCXL_TypeFloatP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				if(sctx->fpu_soft || sctx->fpu_gfp)
				{
					if(sctx->has_fmovs)
					{
						BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
							BGBCC_SH_NMID_FMOVS, dreg, k);
						return(1);
					}

					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_MOVL, dreg, k);
					if(sctx->fpu_gfp)
					{
						BGBCC_JX2_EmitOpRegReg(sctx,
							BGBCC_SH_NMID_FLDCF, dreg, dreg);
					}
				}else
				{
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_FMOVS, dreg, k);
				}
				return(1);
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				if(sctx->fpu_soft || sctx->fpu_gfp)
				{
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_MOVQ, dreg, k);
				}else
				{
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_FMOVD, dreg, k);
				}
				return(1);
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeVec64P(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVQ, dreg, k);
				return(1);
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeVec128P(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVX2, dreg, k);
				return(1);
			}
#endif
		}
		
		treg=dreg;
//		if(	BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
//			BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg) ||
//			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg)	)
//		if(!BGBCC_JX2_EmitCheckRegAddrGPR(sctx, dreg))
		if(!BGBCC_JX2_EmitCheckRegExtGPR(sctx, dreg))
		{
			treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		}

//		if(sctx->is_pic)
		if(0)
		{
#if 0
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);

//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, k*4);
//			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
//			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, treg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
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

#if 0
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
#endif

//		if(BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
//			BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
//			BGBCC_CCXL_TypeVariantP(ctx, tty) ||
//			(BGBCC_CCXL_TypePointerP(ctx, tty) && sctx->is_addr64))

		if(BGBCC_CCXL_TypeSmallLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVariantP(ctx, tty))
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

		if(BGBCC_CCXL_TypeSgNLongP(ctx, tty))
		{
			nm1=BGBCC_SH_NMID_MOVL;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, tty))
				nm1=BGBCC_SH_NMID_MOVUL;

			if(ctx->arch_sizeof_long==8)
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

//		if(	BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
//			(BGBCC_CCXL_TypePointerP(ctx, tty) && sctx->is_addr64))
		if(BGBCC_CCXL_TypePointerP(ctx, tty))
		{
//			nm1=BGBCC_SH_NMID_MOVL;
//			nm1=BGBCC_SH_NMID_MOVUL;

//			if(sctx->is_addr64)
//			if(ctx->arch_sizeof_ptr==8)
//			{
//				BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//				nm1=BGBCC_SH_NMID_MOVQ;
//			}
			nm1=BGBCC_SH_NMID_MOVP;

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
			if(sctx->fpu_soft || sctx->fpu_gfp)
			{
				if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
				{
					BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
						BGBCC_SH_NMID_MOVQ, treg, 0, dreg);
				}else
				{
					if(sctx->has_fmovs)
					{
						BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
							BGBCC_SH_NMID_FMOVS, treg, 0, dreg);
					}else
					{
						BGBCC_JX2C_EmitLoadBRegOfsReg(ctx, sctx,
							BGBCC_SH_NMID_MOVL, treg, 0, dreg);
						if(sctx->fpu_gfp)
						{
							BGBCC_JX2_EmitOpRegReg(sctx,
								BGBCC_SH_NMID_FLDCF, dreg, dreg);
						}
					}
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

		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
			BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+0, li);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+1, lj);
			return(1);
		}

#if 1
		BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
		k=BGBCC_JX2C_IndexLitInt128(ctx, sctx, li, lj);
	
//		if(sctx->is_pic)
		if(0)
		{
#if 0
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, k*4);
//			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
		}else
		{
			if(sctx->is_rom)
				BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
			else
				BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
		}
#endif

		return(1);
	}

	if(BGBCC_CCXL_IsRegImmX128P(ctx, sreg))
	{

		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
		{
			dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
			BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+0, li);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+1, lj);
			return(1);
		}

		if(1)
		{
			dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
			BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+0, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegThisIdxP(ctx, sreg))
	{
		if(BGBCC_CCXL_IsRegThisP(ctx, sreg))
		{
			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;

			k=sctx->frm_offs_thisptr;
			i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
			return(i);
		}

		tty=BGBCC_CCXL_GetRegType(ctx, sreg);
		BGBCC_JX2C_EmitLoadThisIdxVRegReg(ctx, sctx, tty, sreg, dreg);
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
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
		j=dreg.val&CCXL_REGID_BASEMASK;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;

		rcls=ctx->cur_func->locals[j]->regcls;
		tty=ctx->cur_func->locals[j]->type;

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

//		if(ctx->cur_func->locals[j]->regflags&BGBCC_REGFL_ALIASPTR)
		if(1)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
			{
				if(sctx->has_fmovs)
//				if(0)
				{
					k=ctx->cur_func->locals[j]->fxoffs;
					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
						sreg, BGBCC_SH_REG_SP, k);
					return(1);
				}

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FSTCF, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_STHF16, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_STBF16, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}
		}

		rcls=ctx->cur_func->locals[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			sreg=BGBCC_SH_REG_RD0|(sreg&15);

//		if((rcls==BGBCC_SH_REGCLS_GR) && sctx->is_addr64)
		if(((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR)) && sctx->is_addr64)
		{
//			if((sreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
//				sreg=BGBCC_SH_REG_RD0|(sreg&15);
//			if((sreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
//				sreg=BGBCC_SH_REG_RD16|(sreg&15);
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0)
				sreg=BGBCC_SH_REG_RD0|(sreg&31);
		}
		
		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
		}

		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, dreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
		j=dreg.val&CCXL_REGID_BASEMASK;

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, ctx->cur_func->regs[j]->type))
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
#endif

		rcls=ctx->cur_func->regs[j]->regcls;
		tty=ctx->cur_func->regs[j]->type;

//		if(ctx->cur_func->regs[j]->regflags&BGBCC_REGFL_ALIASPTR)
		if(1)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
			{
				if(sctx->has_fmovs)
//				if(0)
				{
					k=ctx->cur_func->regs[j]->fxoffs;
					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
						sreg, BGBCC_SH_REG_SP, k);
					return(1);
				}

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->regs[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FSTCF, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->regs[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_STHF16, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->regs[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_STBF16, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}
		}

		rcls=ctx->cur_func->regs[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			sreg=BGBCC_SH_REG_RD0|(sreg&15);

//		if((rcls==BGBCC_SH_REGCLS_GR) && sctx->is_addr64)
		if(((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR)) && sctx->is_addr64)
		{
//			if((sreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
//				sreg=BGBCC_SH_REG_RD0|(sreg&15);
//			if((sreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
//				sreg=BGBCC_SH_REG_RD16|(sreg&15);
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0)
				sreg=BGBCC_SH_REG_RD0|(sreg&31);
		}
		
		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
		}

		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, dreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
		j=dreg.val&CCXL_REGID_BASEMASK;

#if 1	//Debug: Deref pointers to check validity
//		if(BGBCC_CCXL_TypePointerP(ctx, ctx->cur_func->args[j]->type))
//			BGBCC_JX2_EmitOpMReg(sctx, BGBCC_SH_NMID_PREF, sreg);
		BGBCC_JX2C_EmitDebugCheckReg(ctx, sctx,
			ctx->cur_func->args[j]->type, sreg);
#endif

		rcls=ctx->cur_func->args[j]->regcls;
		tty=ctx->cur_func->args[j]->type;

//		if(ctx->cur_func->args[j]->regflags&BGBCC_REGFL_ALIASPTR)
		if(1)
		{
			if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
			{
				if(sctx->has_fmovs)
//				if(0)
				{
					k=ctx->cur_func->args[j]->fxoffs;
					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
						sreg, BGBCC_SH_REG_SP, k);
					return(1);
				}

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->args[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_FSTCF, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->args[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_STHF16, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}

			if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
			{
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->args[j]->fxoffs;
				BGBCC_JX2C_EmitConvOpRegReg(ctx, sctx,
					BGBCC_SH_NMID_STBF16, sreg, treg);
				i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, treg);
				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(i);
			}
		}

		rcls=ctx->cur_func->args[j]->regcls;
		if((rcls==BGBCC_SH_REGCLS_WGR) && !sctx->is_addr64)
			sreg=BGBCC_SH_REG_RD0|(sreg&15);

//		if((rcls==BGBCC_SH_REGCLS_GR) && sctx->is_addr64)
		if(((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_GR)) && sctx->is_addr64)
		{
			if((sreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
				sreg=BGBCC_SH_REG_RD0|(sreg&15);
			if((sreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
				sreg=BGBCC_SH_REG_RD16|(sreg&15);
		}
		
		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
		}

		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegGlobalP(ctx, dreg))
	{
//		sctx->is_leaftiny|=8;
//		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;

//		if(ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR)
		if(sctx->is_pbo && (ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR))
//		if(1)
		{
#if 0
//			if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
			if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY))
				printf("Reject LeafTiny PBO+Alias %s\n",
					ctx->cur_func->qname);
#endif
			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}

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

			if(BGBCC_CCXL_TypeSgLongP(ctx, tty) && sctx->is_addr64)
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVQ, sreg, k);
				return(1);
			}

//			if((BGBCC_CCXL_TypeSgNLongP(ctx, tty) ||
//				BGBCC_CCXL_TypePointerP(ctx, tty)) &&
//				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			if(BGBCC_CCXL_TypePointerP(ctx, tty) &&
				!BGBCC_CCXL_TypeArrayP(ctx, tty))
			{
//				nm1=BGBCC_SH_NMID_MOVL;
//				if(sctx->is_addr64)
//				if(ctx->arch_sizeof_ptr==8)
//				{
//					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//					nm1=BGBCC_SH_NMID_MOVQ;
//				}
				nm1=BGBCC_SH_NMID_MOVP;
			
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					nm1, sreg, k);
				return(1);
			}

			if(BGBCC_CCXL_TypeSgNLongP(ctx, tty))
			{
				nm1=BGBCC_SH_NMID_MOVL;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, tty))
					nm1=BGBCC_SH_NMID_MOVUL;
				if(ctx->arch_sizeof_long==8)
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

#if 1
			if(BGBCC_CCXL_TypeFloatP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);

				if(sctx->fpu_gfp)
				{
					if(sctx->has_fmovs)
					{
						BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
							BGBCC_SH_NMID_FMOVS, sreg, k);
					}else
					{
						BGBCC_JX2_EmitOpRegReg(sctx,
							BGBCC_SH_NMID_FSTCF, sreg, BGBCC_SH_REG_R1);
						BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, k);
					}
				}else
					if(sctx->fpu_soft)
				{
					BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_MOVL, sreg, k);
				}else
				{
					BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_FMOVS, sreg, k);
				}
				return(1);
			}

			if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				if(sctx->fpu_soft || sctx->fpu_gfp)
				{
					BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_MOVQ, sreg, k);
				}else
				{
					BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
						BGBCC_SH_NMID_FMOVD, sreg, k);
				}
				return(1);
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeVec64P(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVQ, sreg, k);
				return(1);
			}
#endif

#if 1
			if(BGBCC_CCXL_TypeVec128P(ctx, tty))
			{
				j=BGBCC_CCXL_GetRegID(ctx, dreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
					BGBCC_SH_NMID_MOVX2, sreg, k);
				return(1);
			}
#endif

		}
#endif

		treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
//		treg=BGBCC_SH_REG_R1;
//		cdreg=BGBCC_JX2C_EmitGetRegisterRead(ctx, sctx, dreg);
//		cdreg=BGBCC_JX2C_EmitGetRegisterWrite(ctx, sctx, dreg);

//		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
//			treg=BGBCC_SH_REG_R2;

		if(sctx->is_pic)
		{
#if 0
			j=BGBCC_CCXL_GetRegID(ctx, dreg);
			k=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);

//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, k*4);
//			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, treg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
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

#if 0
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
#endif

#if 1
		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty))
		{
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVL, treg, 0, sreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypePointerP(ctx, tty))
		{
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVP, treg, 0, sreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		{
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVQ, treg, 0, sreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}
#endif


		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			if(sctx->fpu_gfp)
			{
				if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
				{
					BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
						BGBCC_SH_NMID_MOVQ, treg, 0, sreg);
				}else
				{
					if(sctx->has_fmovs)
					{
						BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
							BGBCC_SH_NMID_FMOVS, treg, 0, sreg);
					}else
					{
						BGBCC_JX2_EmitOpRegReg(sctx,
							BGBCC_SH_NMID_FSTCF, sreg, BGBCC_SH_REG_R1);
						BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
							BGBCC_SH_NMID_MOVL, treg, 0, BGBCC_SH_REG_R1);
					}
				}

				BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
				return(1);
			}else
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
		
		if(BGBCC_CCXL_TypeVec64P(ctx, tty))
		{
			BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
				BGBCC_SH_NMID_MOVQ, treg, 0, sreg);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		if(BGBCC_CCXL_TypeVec128P(ctx, tty))
		{
			if(sctx->has_pushx2 && !(sreg&1))
			{
				BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVX2, treg, 0, sreg);
			}else
			{
				BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, treg, 0, sreg+0);
				BGBCC_JX2C_EmitStoreBRegOfsReg(ctx, sctx,
					BGBCC_SH_NMID_MOVQ, treg, 8, sreg+1);
			}
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
	
	if(BGBCC_CCXL_IsRegThisIdxP(ctx, dreg))
	{
		if(BGBCC_CCXL_IsRegThisP(ctx, dreg))
		{
			BGBCC_DBGBREAK
			return(0);
		}

		tty=BGBCC_CCXL_GetRegType(ctx, dreg);
		BGBCC_JX2C_EmitStoreThisIdxVRegReg(ctx, sctx, tty, dreg, sreg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2C_CheckEmitLookupVRegForName(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name, ccxl_register *rreg)
{
	ccxl_register treg;
	int i, j, k;

	if(!ctx->cur_func)
		return(0);

	for(i=0; i<ctx->cur_func->n_locals; i++)
	{
		if(!strcmp(ctx->cur_func->locals[i]->name, name))
		{
			treg.val=CCXL_REGTY_LOCAL|
//				(((u64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				i;
			*rreg=treg;
			return(1);
		}
	}

	for(i=0; i<ctx->cur_func->n_args; i++)
	{
		if(!strcmp(ctx->cur_func->args[i]->name, name))
		{
			treg.val=CCXL_REGTY_ARG|
//				(((u64)bty.val)<<CCXL_REGID_TYPESHIFT)|
				i;
			*rreg=treg;
			return(1);
		}
	}
	
	return(0);
}

int BGBCC_JX2C_CheckEmitLoadFrameNameReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name, int dreg)
{
	ccxl_register sreg;
	int i, j, k;
	
	i=BGBCC_JX2C_CheckEmitLookupVRegForName(ctx, sctx, name, &sreg);
	if(i<=0)
		return(i);

	BGBCC_JX2C_EmitLoadFrameVRegByValReg(ctx, sctx,
		sreg, dreg);

	return(0);
}

int BGBCC_JX2C_CheckEmitStoreFrameNameReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	char *name, int dreg)
{
	ccxl_register sreg;
	int i, j, k;
	
	i=BGBCC_JX2C_CheckEmitLookupVRegForName(ctx, sctx, name, &sreg);
	if(i<=0)
		return(i);

	BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
		sreg, dreg);

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
		
		if(!(ctx->cur_func->regs[j]->regflags&BGBCC_REGFL_INITIALIZED))
			{ BGBCC_DBGBREAK }

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
//		sctx->is_leaftiny|=8;
//		ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
//		if(ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR)
		if(sctx->is_pbo && (ctx->cur_func->regflags&BGBCC_REGFL_ALIASPTR))
//		if(1)
		{
#if 0
//			if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
			if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY))
				printf("Reject LeafTiny PBO+Alias %s\n",
					ctx->cur_func->qname);
#endif
			ctx->cur_func->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}

//		if(sctx->is_pic)
		if(0)
		{
#if 0
			j=BGBCC_CCXL_GetRegID(ctx, sreg);
			k=BGBCC_JX2C_GetGblIndexGotIndex(ctx, sctx, j);

//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//			BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R0, k*4);
//			BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R12, BGBCC_SH_REG_R0, dreg);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif
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

int BGBCC_JX2C_GetFrameVRegVspanFlags(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	BGBCC_JX2_VarSpan2 *vspb;
	int fl;
	int i, j, k;

	if(!sreg.val)
		return(0);
	if((sreg.val&4095)==4095)
		return(0);
	
	for(i=0; i<sctx->vspan_num; i++)
	{
		vsp=sctx->vspan[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vsp->reg, sreg))
		{
			j=(sreg.val>>12)&4095;
			if(j<0)j=0;
			if(j>63)j=63;
			vspb=vsp->seq+j;
			
			if(vspb->cnt>0)
				return(vspb->flag | vsp->flag);
		}
	}
	
	return(0);
}

int BGBCC_JX2C_BeginSetupFrameVRegSpan(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	return(0);
}

int BGBCC_JX2C_EndSetupFrameVRegSpan(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx)
{
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	BGBCC_JX2_VarSpan2 *vspb, *vspb1;
	ccxl_register vreg;
	int v, h;
	int i, j, k;
	
	for(i=0; i<64; i++)
		sctx->vspan_hash[i]=(-1);
	
//	for(i=0; i<sctx->vspan_num; i++)
	for(i=sctx->vspan_num-1; i>=0; i--)
	{
		vsp=sctx->vspan[i];
		vreg=vsp->reg;

//		v=vreg.val&CCXL_REGTY_REGMASK;
		v=(vreg.val&CCXL_REGID_BASEMASK)^
			((vreg.val&CCXL_REGTY_REGMASK)>>56);
		h=v*251;
		h=(h>>8)&63;

		sctx->vspan_chn[i]=sctx->vspan_hash[h];
		sctx->vspan_hash[h]=i;

		if((vsp->tbeg)==(vsp->tend))
		{
			vsp->flag|=BGBCC_RSPFL_SINGLETRACE;
			
//			if(vsp->nseq==1)
//				{ vsp->flag|=BGBCC_RSPFL_NONOVTRACE; }
		}

		for(j=0; j<vsp->nseq; j++)
		{
			vspb=vsp->seq+j;
			if(!(vspb->cnt))
				continue;

			if(vspb->flag&BGBCC_RSPFL_CROSSTRACE)
				{ vsp->flag|=BGBCC_RSPFL_CROSSTRACE; }
			
			if((vspb->tbeg)!=(vspb->tend))
				{ vsp->flag|=BGBCC_RSPFL_CROSSTRACE; }
		}

		for(j=0; j<vsp->nseq; j++)
		{
			vspb=vsp->seq+j;
			if(!(vspb->cnt))
				continue;
			
			if((vspb->tbeg)==(vspb->tend))
			{
				vspb->flag|=BGBCC_RSPFL_SINGLETRACE;
				
				if(vsp->flag&BGBCC_RSPFL_CROSSTRACE)
					continue;

#if 1
				for(k=0; k<vsp->nseq; k++)
				{
					if(k==j)
						continue;
					vspb1=vsp->seq+k;
					if(!(vspb1->cnt))
						continue;
					if(((vspb->tbeg)>=(vspb1->tbeg)) &&
						((vspb->tbeg)<=(vspb1->tend)))
							break;
				}
				
				if(k>=vsp->nseq)
				{
//					if(BGBCC_CCXL_IsRegTempP(ctx, vreg))
//					if(BGBCC_CCXL_IsRegTempP(ctx, vreg) ||
//						BGBCC_CCXL_IsRegLocalP(ctx, vreg))
//					if(vsp->flag&BGBCC_RSPFL_LOCAL)
//						vspb->flag|=BGBCC_RSPFL_NONOVTRACE;

					if(BGBCC_CCXL_IsRegTempP(ctx, vreg))
						vspb->flag|=BGBCC_RSPFL_NONOVTRACE;

//					vspb->flag|=BGBCC_RSPFL_NONOVTRACE;
				}
#endif
			}
		}

//		if((vreg.val&CCXL_REGTY_REGMASK)!=(reg.val&CCXL_REGTY_REGMASK))
//			continue;
		
//		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vreg, reg))
//			return(i);
	}

	return(0);
}

int BGBCC_JX2C_SetupFrameVRegSpan(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dstfl)
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
		sctx->vspan_chn=bgbcc_malloc(1024*sizeof(short));
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
			
//			j=(sreg.val>>12)&4093;
			j=(sreg.val>>12)&4095;
			if(j<0)j=0;
			if(j>63)j=63;
			vspb=vsp->seq+j;
			if(j>=vsp->nseq)
				vsp->nseq=j+1;

			if(vspb->cnt)
			{
				if(sctx->tr_opnum < vspb->bbeg)
					vspb->bbeg=sctx->tr_opnum;
				if(sctx->tr_opnum > vspb->bend)
					vspb->bend=sctx->tr_opnum;
				if(sctx->tr_trnum < vspb->tbeg)
					vspb->tbeg=sctx->tr_trnum;
				if(sctx->tr_trnum > vspb->tend)
					vspb->tend=sctx->tr_trnum;
				vspb->cnt++;
			}else
			{
				vspb->bbeg=sctx->tr_opnum;
				vspb->bend=sctx->tr_opnum;
				vspb->tbeg=sctx->tr_trnum;
				vspb->tend=sctx->tr_trnum;
				vspb->cnt=1;
				vspb->flag=0;
				if(!(dstfl&1))
					{ vspb->flag|=BGBCC_RSPFL_CROSSTRACE; }
			}
			
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
		!BGBCC_CCXL_IsRegArgP(ctx, sreg) &&
		!BGBCC_CCXL_IsRegLocalP(ctx, sreg))
			fl|=BGBCC_RSPFL_NONLOCAL;

	if(BGBCC_CCXL_IsRegVolatileP(ctx, sreg))
		fl|=BGBCC_RSPFL_NONLOCAL;

//	if(	BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
//		BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//		BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
//		BGBCC_CCXL_TypeRealP(ctx, tty) ||
//		BGBCC_CCXL_TypeSgInt128P(ctx, tty))

	if(	BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
		BGBCC_CCXL_TypeArrayP(ctx, tty) ||
		BGBCC_CCXL_TypeSgInt128P(ctx, tty) ||
		BGBCC_CCXL_TypeFloat128P(ctx, tty) ||
		BGBCC_CCXL_TypeVec128P(ctx, tty))
			fl|=BGBCC_RSPFL_NONBASIC;

	i=sctx->vspan_num++;
	vsp=sctx->vspan[i];
	if(!vsp)
	{
		vsp=bgbcc_malloc(sizeof(BGBCC_JX2_VarSpan));
		sctx->vspan[i]=vsp;
	}else
	{
		memset(vsp, 0, sizeof(BGBCC_JX2_VarSpan));
	}

	vsp->reg=sreg;
	vsp->bbeg=sctx->tr_opnum;
	vsp->bend=sctx->tr_opnum;
	vsp->tbeg=sctx->tr_trnum;
	vsp->tend=sctx->tr_trnum;
	vsp->cnt=1;
	vsp->flag=fl;
	vsp->nseq=0;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&4095;
		if(ctx->cur_func->locals[j]->flagsint&BGBCC_TYFL_INLINE)
			{ vsp->cnt+=100; }
	}

//	if((obj->flagsint&BGBCC_TYFL_CONST) &&

#if 0
	for(j=0; j<64; j++)
	{
		vspb=vsp->seq+j;
		vspb->bbeg=999999;
		vspb->tbeg=999999;
		vspb->bend=-999999;
		vspb->tend=-999999;
		vspb->cnt=0;
	}
#endif
	
	j=(sreg.val>>12)&4093;
	if(j<0)j=0;
	if(j>63)j=63;
	vspb=vsp->seq+j;
	if(j>=vsp->nseq)
		vsp->nseq=j+1;

	vspb->bbeg=sctx->tr_opnum;
	vspb->bend=sctx->tr_opnum;
	vspb->tbeg=sctx->tr_trnum;
	vspb->tend=sctx->tr_trnum;
	vspb->cnt=1;
	vspb->flag=fl;
	if(!(dstfl&1))
		{ vspb->flag|=BGBCC_RSPFL_CROSSTRACE; }

	return(1);
}

int BGBCC_JX2C_SetupFrameLayout(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_CCXL_VirtOp *vop;
	BGBCC_CCXL_VirtTr *vtr;
	char *fname;
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
	sctx->frm_offs_thisptr=0;

	for(i=0; i<obj->n_vop; i++)
	{
		vop=obj->vop[i];
		if(vop->opn==CCXL_VOP_LDAVAR)
		{
			reg=vop->srca;
			j=reg.val&4095;
			if(j==4095)
				continue;

			if(BGBCC_CCXL_IsRegTempP(ctx, reg))
			{
				obj->regs[j]->regflags|=BGBCC_REGFL_ALIASPTR;
				obj->regs[j]->regflags|=BGBCC_REGFL_INITIALIZED;
			}
			if(BGBCC_CCXL_IsRegArgP(ctx, reg))
				{ obj->args[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
			if(BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ obj->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR; }
		}

		if((vop->opn==CCXL_VOP_INITOBJ) ||
			(vop->opn==CCXL_VOP_DROPOBJ) ||
			(vop->opn==CCXL_VOP_INITARR) ||
			(vop->opn==CCXL_VOP_INITOBJARR) ||
			(vop->opn==CCXL_VOP_CALL) ||
			(vop->opn==CCXL_VOP_OBJCALL) ||
			(vop->opn==CCXL_VOP_CSRV))
		{
			reg=vop->dst;
			j=reg.val&4095;
			if(j==4095)
				continue;

			if(BGBCC_CCXL_IsRegTempP(ctx, reg))
				{ obj->regs[j]->regflags|=BGBCC_REGFL_INITIALIZED; }
		}
	}

#if 1

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
			continue;
		if(obj->locals[i]->regflags&BGBCC_REGFL_ALIASPTR)
			continue;
			
		tty=obj->locals[i]->type;

		if(BGBCC_CCXL_TypeFloat16P(ctx, tty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			obj->locals[i]->type=tty;
		}
		
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			obj->locals[i]->type=tty;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, tty))
		{
			obj->regflags|=BGBCC_REGFL_HASARRAY;
		}

		if(obj->locals[i]->flagsint&BGBCC_TYFL_REGISTER)
		{
			sctx->use_egpr=1;
			obj->regflags|=BGBCC_REGFL_GOFAST;
		}
	}

#if 1
	for(i=0; i<obj->n_regs; i++)
	{
		if(obj->regs[i]->regflags&BGBCC_REGFL_CULL)
			continue;
		if(obj->regs[i]->regflags&BGBCC_REGFL_ALIASPTR)
			continue;
			
		tty=obj->regs[i]->type;

		if(BGBCC_CCXL_TypeFloat16P(ctx, tty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			obj->regs[i]->type=tty;
		}
		
#if 1
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			obj->regs[i]->type=tty;
		}
#endif

//		if(BGBCC_CCXL_TypeArrayP(ctx, tty))
//		{
//			obj->regflags|=BGBCC_REGFL_HASARRAY;
//		}
	}
#endif

#if 1
	for(i=0; i<obj->n_args; i++)
	{
		if(obj->args[i]->regflags&BGBCC_REGFL_CULL)
			continue;
		if(obj->args[i]->regflags&BGBCC_REGFL_ALIASPTR)
			continue;
			
		tty=obj->args[i]->type;

		if(BGBCC_CCXL_TypeFloat16P(ctx, tty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			obj->args[i]->type=tty;
		}
		
		if(BGBCC_CCXL_TypeFloatP(ctx, tty) && sctx->fpu_gfp)
		{
			tty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			obj->args[i]->type=tty;
		}
	}
#endif

#endif

	sctx->vsp_tcnt=0;
	trn=0;
	BGBCC_JX2C_BeginSetupFrameVRegSpan(ctx, sctx);

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
		
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->dst, 1);
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->srca, 0);
		BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx, vop->srcb, 0);


		if(vop->opn==CCXL_VOP_CALL)
		{
			fname=NULL;
			if((vop->srca.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
			{
				j=(int)(vop->srca.val&CCXL_REGID_REGMASK);
				fname=ctx->reg_globals[j]->name;
				
				if(fname && !strcmp(fname, "__alloca"))
					{ obj->regflags|=BGBCC_REGFL_ALLOCA; }
			}

			for(j=0; j<vop->imm.call.na; j++)
			{
				BGBCC_JX2C_SetupFrameVRegSpan(ctx, sctx,
					vop->imm.call.args[j], 0);
			}
		}
	}
	BGBCC_JX2C_EndSetupFrameVRegSpan(ctx, sctx);

	if(!strcmp(obj->name, "TKRA_ProjectVertex"))
	{
//		sctx->frm_offs_retstr=k-8;
		k=-1;
//		BGBCC_DBGBREAK
	}

	
	if(!(sctx->use_egpr&4))
		sctx->use_egpr=0;
	if(sctx->has_bjx1egpr)
	{
		k=64;
		if(ctx->optmode==BGBCC_OPT_SPEED)
//			k=32;
//			k=24;
			k=20;
//			k=16;
//			k=4;
		if(ctx->optmode==BGBCC_OPT_SIZE)
			k=96;

#if 1
//		if(sctx->vspan_num>32)
//		if(sctx->vspan_num>48)
//		if(sctx->vspan_num>56)
//		if(sctx->vspan_num>64)
//		if(sctx->vspan_num>68)

//		if(sctx->vspan_num>72)

//		if(sctx->vspan_num>80)
//		if(sctx->vspan_num>88)
//		if(sctx->vspan_num>96)
//		if(sctx->vspan_num>128)

		/* High register pressure. */
		if(sctx->vspan_num>k)
		{
			sctx->use_egpr=1;
		}

		/* Try for full static assignment. */
		if(sctx->vspan_num<12)
			sctx->use_egpr=1;
		
//		if(sctx->is_mergece)
//			sctx->use_egpr=0;
#endif

//		sctx->use_egpr=1;
	}

	if(ctx->optmode==BGBCC_OPT_SPEED2)
		sctx->use_egpr=1;

	if(sctx->is_fixed32)
		sctx->use_egpr=1;

	if(!sctx->use_egpr)
	{
		for(i=0; i<obj->n_locals; i++)
		{
			if(obj->locals[i]->regflags&BGBCC_REGFL_CULL)
				continue;

			if(obj->locals[i]->flagsint&BGBCC_TYFL_REGISTER)
			{
				sctx->use_egpr=1;
				obj->regflags|=BGBCC_REGFL_GOFAST;
//				continue;
			}
		
			rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->locals[i]->type);
			switch(rcls)
			{
			case BGBCC_SH_REGCLS_VO_QGR:
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_WGR:
				sctx->use_egpr=1;
				break;
			}
		}
		
		for(i=0; i<obj->n_args; i++)
		{
			rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
			switch(rcls)
			{
			case BGBCC_SH_REGCLS_VO_QGR:
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_WGR:
				sctx->use_egpr=1;
				break;
			}
		}
		
		for(i=0; i<obj->n_regs; i++)
		{
			rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
			switch(rcls)
			{
			case BGBCC_SH_REGCLS_VO_QGR:
			case BGBCC_SH_REGCLS_VO_QGR2:
			case BGBCC_SH_REGCLS_QGR:
			case BGBCC_SH_REGCLS_QGR2:
			case BGBCC_SH_REGCLS_WGR:
				sctx->use_egpr=1;
				break;
			}
		}
	}

	if(!strcmp(obj->name, "TKRA_MatrixIdentify"))
	{
		sctx->frm_offs_retstr=k-8;
//		BGBCC_DBGBREAK
	}

//	sctx->use_egpr=0;

	ni=0; nf=0;
	k=0; ka=0; kf=0;
	k-=2*4;		//saved PR, R14
//	k-=6*4;		//saved R8/9/10/11/13, R2
	k-=8*4;		//saved R8/9/10/11/13, R2

//	if(sctx->has_bjx1egpr)
	if(sctx->has_bjx1egpr && sctx->use_egpr)
		k-=8*4;		//saved R24..R31

//	if(sctx->is_pbo)
	if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		k-=4;		//saved GBR

	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		k-=8*4;		//saved R0..R7
		if(sctx->has_bjx1egpr)
			k-=8*4;		//saved R16..R23
	}

	if(sctx->is_addr64)
		k*=2;
		
	if((obj->flagsint&BGBCC_TYFL_VIRTUAL) ||
		obj->thisstr)
	{
		if(sctx->is_addr64)
			{ k-=8; k&=~7; }
		else
			{ k-=4; k&=~3; }
		sctx->frm_offs_thisptr=k;
	}
	
//	if(obj->regflags&BGBCC_REGFL_HASARRAY)
	if((obj->regflags&BGBCC_REGFL_HASARRAY) &&
		!(obj->regflags&BGBCC_REGFL_GOFAST))
	{
		i=(nlint)obj;
		i=(u16)((i*65521)>>16);
		k-=8; k&=~7;
		sctx->frm_offs_sectoken=k;
		sctx->frm_val_sectoken=i;
	}else
	{
		sctx->frm_offs_sectoken=0;
	}

	if((obj->regflags&BGBCC_REGFL_ALLOCA))
	{
		k-=8; k&=~7;
		sctx->frm_offs_allocamrk=k;
	}else
	{
		sctx->frm_offs_allocamrk=0;
	}

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

			if(j>BGBCC_MAXSTACKOBJ)
				{ BGBCC_DBGBREAK }

			if(sctx->is_addr64)
				j=(j+7)&(~7);
			else
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
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_WGR:
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}

	for(i=0; i<obj->n_regs; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
//		rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, 
//			BGBCC_CCXL_TypeWrapBasicType(obj->regs[i]->type_zb));
		obj->regs[i]->regcls=rcls;

		switch(rcls)
		{
		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
		case BGBCC_SH_REGCLS_DR:
			sctx->use_fpr=1;	break;
		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:

			if(!(obj->regs[i]->regflags&BGBCC_REGFL_INITIALIZED))
				break;

			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
			if(sctx->is_addr64)
				j=(j+7)&(~7);
			else
				j=(j+3)&(~3);

			if(j>BGBCC_MAXSTACKOBJ)
				{ BGBCC_DBGBREAK }

			kf+=j;
			break;

//		case BGBCC_SH_REGCLS_AR_REF:
//			break;

		case BGBCC_SH_REGCLS_GR:
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_VO_GR:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR:
		case BGBCC_SH_REGCLS_VO_QGR2:
		case BGBCC_SH_REGCLS_QGR:
		case BGBCC_SH_REGCLS_QGR2:
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

			if(j>1024)
				{ BGBCC_DBGBREAK }

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
		case BGBCC_SH_REGCLS_QGR2:
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
			
			if(j>BGBCC_MAXSTACKOBJ)
				{ BGBCC_DBGBREAK }
			
//			j=(j+3)&(~3);
			j=(j+7)&(~7);
			kf+=j;
		}
	}

//	if(sctx->use_fpr)
//		{ k-=4*8; }		//saved FR12/13/14/15
	if(sctx->use_fpr)
		{ k-=8*8; }		//saved FR12/13/14/15

//	if(!strcmp(obj->name, "GetWavinfo"))
	if(!strcmp(obj->name, "TKRA_MatrixIdentify"))
//	if(!strcmp(obj->qname, "TKRA_MatrixIdentify"))
	{
		sctx->frm_offs_retstr=k-8;
//		BGBCC_DBGBREAK
	}

	k-=kf;
	sctx->frm_offs_fix=k;

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

//	if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
	if(rcls==BGBCC_SH_REGCLS_VO_REF)
	{
//		k-=4;
		k-=8;
		sctx->frm_offs_retstr=k;
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

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
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->abi_evenonly)
				ni=(ni+1)&(~1);
		
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

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

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
				{ k&=~7; k-=8;	break; }
			k&=~3; k-=4;	break;
		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->is_addr64)
				{ k&=~7; k-=16;	break; }
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

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

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
		case BGBCC_SH_REGCLS_QGR2:
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

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

	for(i=0; i<obj->n_regs; i++)
	{
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->regs[i]->type);
//		rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, 
//			BGBCC_CCXL_TypeWrapBasicType(obj->regs[i]->type_zb));
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
		case BGBCC_SH_REGCLS_QGR2:
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

	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }

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
	
#if 0
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
#else
	if((-k)>=1012)
		{ k&=~63; }
#endif
	
	if((-k)>=BGBCC_MAXSTACKFRAME)
		{ BGBCC_DBGBREAK }
	
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

//	if(!sctx->has_bjx1jmp)
//		return(0);

	if(	(obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
		(obj->flagsint&BGBCC_TYFL_SYSCALL))
			return(0);

	/* Don't compress prologs/epilogs for hot-path functions. */
	if(obj->regflags&BGBCC_REGFL_GOFAST)
		return(0);


//	if(ctx->optmode==BGBCC_OPT_SPEED)
//		return(0);
//	if(ctx->optmode!=BGBCC_OPT_SIZE)
//		return(0);

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

	if((obj->regflags&BGBCC_REGFL_ALIASPTR) && sctx->is_pbo)
		epik|=0x04000000;
		
	uli=epik;
	uli=(uli+1)*65521;
	uli=(uli+1)*65521;
	uli=(uli+1)*251;
	epix=(uli>>32)&1023;

	/* Unusual Registers Saved */
	if(sctx->reg_save&0x00FF00FF)		{ epilbl=0; epik=0; epix=0; }
	if(sctx->freg_save&0xFFFF00FF)		{ epilbl=0; epik=0; epix=0; }

	/* Not enough registers saved */
	if((epik&0x60)!=0x60)
//	if((epik&0x70)!=0x70)
//	if((epik&0x7C)!=0x7C)
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

int BGBCC_JX2C_EmitFrameProlog_PushRegs(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx, int fl, int *rfl2)
{
	int i, j, k, n, fl2;

//	if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
//		(ctx->optmode==BGBCC_OPT_SPEED2))
//	if(0)
	if(1)
	{
		k=0;
		n=0;
		fl2=0;

		if(fl&1)
			k+=2;
		if(fl&2)
			k+=4;

		for(i=31; i>0; i--)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

			if(!(sctx->reg_vsave&((2LL<<i)-1)))
				break;

			if(sctx->has_pushx2 && (i&1))
//			if(0)
			{
				if((sctx->reg_vsave&(3<<(i-1)))==(3<<(i-1)))
				{
					sctx->reg_save|=(3<<(i-1));
					k+=4;
					i--;
					continue;
				}
			}

			if(sctx->reg_vsave&(1<<i))
			{
				sctx->reg_save|=(1<<i);
				k+=2;
			}
		}

		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
			-k*4, BGBCC_SH_REG_SP);
		n++;
			
		j=0;

		if(fl&1)
		{
			j+=2;
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R16, BGBCC_SH_REG_SP, (k-j)*4);
				BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, (k-j)*4);
			n++;
		}

		if(fl&2)
		{
			j+=4;
			if(sctx->has_pushx2)
//			if(0)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_R18, BGBCC_SH_REG_SP, (k-j)*4);
				n++;
			}else
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R19, BGBCC_SH_REG_SP, ((k-j)+2)*4);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R18, BGBCC_SH_REG_SP, ((k-j)+0)*4);
				n+=2;
			}
		}

		for(i=31; i>0; i--)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

			if(!(sctx->reg_vsave&((2LL<<i)-1)))
				break;

			if(sctx->has_pushx2 && (i&1))
//			if(0)
			{
				if((sctx->reg_vsave&(3<<(i-1)))==(3<<(i-1)))
				{
					sctx->reg_save|=(3<<(i-1));
					j+=4;

					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_R0+(i-1), BGBCC_SH_REG_SP, (k-j)*4);
					n++;
					i--;
					continue;
				}
			}

			if(sctx->reg_vsave&(1<<i))
			{
				j+=2;

				sctx->reg_save|=(1<<i);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, (k-j)*4);
				n++;
			}
		}
		
		if(n>=6)
			fl2|=1;

		if(rfl2)
			*rfl2=fl2;

		return(k);
	}

	BGBCC_DBGBREAK

#if 0
	k=0;

	if(fl&1)
	{
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
//			BGBCC_SH_REG_R16);
			BGBCC_SH_REG_R1);
		k+=2;
	}

	for(i=31; i>0; i--)
	{
		if(!(sctx->has_bjx1egpr) && (i>=16))
			continue;
		if(i==15)
			continue;

		if(!(sctx->reg_vsave&((2LL<<i)-1)))
			break;

#if 1
		if(sctx->has_pushx2 && (i&1))
		{
			if((sctx->reg_vsave&(3<<(i-1)))==(3<<(i-1)))
			{
				sctx->reg_save|=(3<<(i-1));
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSHX2,
					BGBCC_SH_REG_R0+(i-1));
				k+=sctx->is_addr64?4:2;
				i--;
				continue;
			}
		}
#endif

		if(sctx->reg_vsave&(1<<i))
		{
			sctx->reg_save|=(1<<i);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
				BGBCC_SH_REG_R0+i);
			k+=2;
		}
	}

#if 0
	for(i=31; i>0; i--)
	{
		if(sctx->freg_vsave&(1<<i))
		{
			sctx->freg_save|=(1<<i);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
				BGBCC_SH_REG_FR0+i);
			k+=2;
		}
	}
#endif

#endif

	return(k);
}

ccxl_status BGBCC_JX2C_TinyLeafProlog_ReserveReg(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, ccxl_register reg)
{
	int csreg;

	if(!reg.val)
		return(0);

	csreg=BGBCC_JX2C_EmitTryGetRegisterRead(ctx, sctx, reg);
	
	if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
	{
#if 0
		if(!(obj->regflags&BGBCC_REGFL_HYBLEAFTINY))
		{
			printf("BGBCC_JX2C_TinyLeafProlog_ReserveReg: "
				"Reject Alloc Fail %s %016llX\n",
				obj->qname, reg.val);
		}
#endif
		obj->regflags|=BGBCC_REGFL_HYBLEAFTINY;
	}else
	{
		BGBCC_JX2C_EmitReleaseRegister(ctx, sctx, reg);
	}
	
	return(0);
}

ccxl_status BGBCC_JX2C_TinyLeafProlog_ReserveVopRegs(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srca);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srcb);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srcc);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->srcd);
	BGBCC_JX2C_TinyLeafProlog_ReserveReg(ctx, sctx, obj, op->dst);
	return(0);
}

int BGBCC_JX2C_EmitFrameProlog_TinyLeaf(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	BGBCC_CCXL_VirtOp	*vop;
	ccxl_register reg, treg;
	ccxl_type tty;
	int i, j, k, bo, ni, nf, vaix, rcls;

	sctx->is_leaftiny=3;

	BGBCC_JX2_EmitLabel(sctx, fcnlbl);

	sctx->cur_fcnbase=BGBCC_JX2_EmitGetOffs(sctx);

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

	sctx->vsp_rsv=0;

	sctx->lbl_ret=BGBCC_JX2_GenLabelTemp(sctx);
	

	ni=0; nf=0; vaix=-1;
	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ vaix=i; continue; }

		tty=obj->args[i]->type;
		treg.val=CCXL_REGTY_ARG|
			(((u64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
//		pr0=BGBCC_JX2C_GetVRegPriority(ctx, sctx, treg);
	
		rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(rcls)
		{
		case BGBCC_SH_REGCLS_GR:
			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
			if(ni>=8)
				break;
			k=obj->args[i]->fxoffs;
			if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD20+(ni-4));
				ni++;
				break;
			}
			BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
				treg, BGBCC_SH_REG_RD4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_GR:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
			if(ni>=8)
				break;
			k=obj->args[i]->fxoffs;
			if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RD20+(ni-4));
				ni++;
				break;
			}
			BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
				treg, BGBCC_SH_REG_RD4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_REF:
		case BGBCC_SH_REGCLS_AR_REF:
//			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		case BGBCC_SH_REGCLS_QGR:
			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
			if(ni>=8)
				break;
			k=obj->args[i]->fxoffs;
			if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ20+(ni-4));
				ni++;
				break;
			}
			BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
				treg, BGBCC_SH_REG_RQ4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_VO_QGR:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

			if((ni>=4) && !sctx->has_bjx1egpr)
				break;
			if(ni>=8)
				break;
			k=obj->args[i]->fxoffs;
			if(ni>=4)
			{
				BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_RQ20+(ni-4));
				ni++;
				break;
			}
			BGBCC_JX2C_EmitBindVRegReg(ctx, sctx,
				treg, BGBCC_SH_REG_RQ4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
	}

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);
	rcls=BGBCC_JX2C_TypeGetRegClassP(ctx, tty);

	if(	(rcls==BGBCC_SH_REGCLS_VO_REF)	||
		(rcls==BGBCC_SH_REGCLS_VO_QGR2)	||
		(rcls==BGBCC_SH_REGCLS_VO_GR2)	||
		(rcls==BGBCC_SH_REGCLS_GR2)		||
		(rcls==BGBCC_SH_REGCLS_QGR2)	)
		{ obj->regflags|=BGBCC_REGFL_NOTLEAFTINY; }

	if(sctx->frm_offs_retstr)
		{ obj->regflags|=BGBCC_REGFL_NOTLEAFTINY; }
	if(sctx->frm_offs_thisptr)
		{ obj->regflags|=BGBCC_REGFL_NOTLEAFTINY; }

	sctx->is_leaftiny=1;

//	obj->regflags|=BGBCC_REGFL_HYBLEAFTINY;

	if(!(obj->regflags&BGBCC_REGFL_HYBLEAFTINY))
	{
		for(i=0; i<obj->n_vop; i++)
		{
			vop=obj->vop[i];
			BGBCC_JX2C_TinyLeafProlog_ReserveVopRegs(
				ctx, sctx, obj, vop);
		}

		if(obj->regflags&BGBCC_REGFL_HYBLEAFTINY)
		{
//			sctx->is_leaftiny=0;
//			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

//			sctx->is_leaftiny=3;
			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);
		}
	}
	
	sctx->is_leaftiny=1;
	
	return(0);
}

int BGBCC_JX2C_EmitFrameEpilog_TinyLeaf(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	int i;

	if(sctx->is_leaftiny&8)
	{
#if 0
//		if(ctx->cur_func->regflags&BGBCC_REGFL_LEAFTINY)
		if(!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAFTINY))
			printf("Reject LeafTiny, Epilog&8 %s\n",
				ctx->cur_func->qname);
#endif
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if(sctx->frm_offs_thisptr)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

//	bo=BGBCC_JX2_EmitGetOffs(sctx);

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
		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret);
	}

	sctx->is_leaftiny=3;

	BGBCC_JX2C_EmitSyncRegisters(ctx, sctx);

	BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

	BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);

	return(0);
}

int BGBCC_JX2C_EmitFrameProlog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, int fcnlbl)
{
	ccxl_register reg, treg;
	ccxl_type tty;
	int bo, co, pr0;
	int p0, vaix, cnt, maxrsv, ismaxrsv, maxvalidrsv;
	int ni, nf, rcls, ob, ov;
	int epik, epix, epilbl, epij;
	int i, j, k, fl, fl2;

	ctx->cur_func=obj;
	ctx->cur_vtr=NULL;

	if(sctx->frm_offs_thisptr)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
	}

	if(obj->regflags&BGBCC_REGFL_NOTLEAFTINY)
		obj->regflags&=~BGBCC_REGFL_LEAFTINY;
	sctx->is_leaftiny=0;

	if(obj->regflags&BGBCC_REGFL_LEAFTINY)
	{
		i=BGBCC_JX2C_EmitFrameProlog_TinyLeaf(ctx, sctx, obj, fcnlbl);
		return(i);
	}

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
//		epij=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl, 
		epij=BGBCC_JX2_EmitCheckAutoLabelNear20B(sctx, epilbl, 
			BGBCC_SH_NMID_BSR);
		if(epij<=0)epilbl=0;
	}

	if(!sctx->is_simpass && epik && (epilbl<=0))
	{
		epij=3;

		epilbl=BGBCC_JX2_GenLabel(sctx);
		sctx->eprhash_key[epix]=epik;
		sctx->eprhash_lbl[epix]=epilbl;

		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

		BGBCC_JX2_EmitLabel(sctx, epilbl);

		fl=1;
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			fl=0;
			fl=2;
		}

#if 1
		fl2=0;
		k=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, fl, &fl2);

//		if(1)
		if(fl2&1)
		{
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
#endif

#if 0
		k=0;

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

#if 1
			if(sctx->has_pushx2 && (i&1))
			{
				if((sctx->reg_vsave&(3<<(i-1)))==(3<<(i-1)))
				{
					sctx->reg_save|=(3<<(i-1));
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSHX2,
						BGBCC_SH_REG_R0+(i-1));
					k+=sctx->is_addr64?4:2;
					i--;
					continue;
				}
			}
#endif

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

		if(sctx->use_fpr && !sctx->no_fpu && !sctx->fpu_gfp)
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_FPSCR, BGBCC_SH_REG_SP);
//			k+=sctx->is_addr64?2:1;
		}

//		for(i=15; i>0; i--)
		for(i=31; i>0; i--)
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
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSN);
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		}
#endif
	}
#endif

	if(sctx->is_align_wexj&2)
		{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

	BGBCC_JX2_EmitLabel(sctx, fcnlbl);

	sctx->cur_fcnbase=BGBCC_JX2_EmitGetOffs(sctx);

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
	
	if(!(sctx->is_simpass&64))
	{
		if(	!(ctx->cur_func->regflags&BGBCC_REGFL_NOSCRATCHDYN) &&
			!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF))
		{
			if(sctx->use_egpr)
				{ sctx->vsp_rsv+=8; }
			else
				{ sctx->vsp_rsv+=3; }
		}
	}


#if 1
	maxvalidrsv=sctx->vspan_num;
	for(i=0; i<sctx->vspan_num; i++)
	{
		reg=sctx->vspan[i]->reg;
		k=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, reg);
		if(k&BGBCC_REGFL_ALIASPTR)
			{ maxvalidrsv=i; break; }

		if(
			!BGBCC_CCXL_IsRegTempP(ctx, reg) &&
			!BGBCC_CCXL_IsRegArgP(ctx, reg) &&
			!BGBCC_CCXL_IsRegLocalP(ctx, reg))
				{ maxvalidrsv=i; break; }

//		if(BGBCC_CCXL_IsRegVolatileP(ctx, reg))
//			{ maxvalidrsv=i; break; }

		tty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
			BGBCC_CCXL_TypeVoidP(ctx, tty) ||
//			BGBCC_CCXL_TypeRealP(ctx, tty) ||
			BGBCC_CCXL_TypeVec128P(ctx, tty))
				{ maxvalidrsv=i; break; }
	}
#endif

	/*
		If number of registers needed is less than what is available,
		Try to static assign everything.
	 */
	maxrsv=7;
	if(sctx->use_bp)maxrsv--;
	if(sctx->is_pic)maxrsv--;
	if(sctx->use_egpr)
		maxrsv+=8;

	ismaxrsv=0;

	if(	(sctx->vspan_num<=maxrsv) &&
		(sctx->vspan_num==maxvalidrsv))
	{
		sctx->vsp_rsv=sctx->vspan_num;
		ismaxrsv=1;
	}
	
	if(sctx->use_bp)sctx->vsp_rsv--;
	if(sctx->is_pic)sctx->vsp_rsv--;
	if(sctx->vsp_rsv<0)
		sctx->vsp_rsv=0;

	if(sctx->vspan_num<sctx->vsp_rsv)
		sctx->vsp_rsv=sctx->vspan_num;

	if(maxvalidrsv<sctx->vsp_rsv)
		sctx->vsp_rsv=maxvalidrsv;

	cnt=0;
	for(i=0; i<sctx->vsp_rsv; i++)
	{
		cnt+=sctx->vspan[i]->cnt;
		if((cnt<(sctx->vsp_tcnt/16)) && !(ismaxrsv>0))
			{ sctx->vsp_rsv=i; break; }

#if 0
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
			BGBCC_CCXL_TypeRealP(ctx, tty) ||
			BGBCC_CCXL_TypeVec128P(ctx, tty))
			{ sctx->vsp_rsv=i; break; }

//		if(BGBCC_CCXL_IsRegSgLongP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegFloatP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
//		if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
//			{ sctx->vsp_rsv=i; break; }
#endif
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
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=4;

			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R19);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_GBR, BGBCC_SH_REG_R18);

		}else
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_R16);
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
//			k+=sctx->is_addr64?2:1;
			k+=2;
		}
#endif

#if 0
		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1, j);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}
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
//			BGBCC_JX2_EmitOpFar16Label(sctx,
			BGBCC_JX2_EmitOpFar20Label(sctx,
				BGBCC_SH_NMID_BSRN, epilbl);
		}

#if 1
		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
//			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
//			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
//				BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R1, j);
//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);

//			BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_GBR, 0, BGBCC_SH_REG_R18);

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx,
					BGBCC_SH_NMID_MOVC_DISP24,
					BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_GBR);
			}else
			{
				BGBCC_JX2_EmitRelocTy(sctx, j, BGBCC_SH_RLC_TBR12_BJX);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx,
					BGBCC_SH_NMID_MOVQ_DISP24,
					BGBCC_SH_REG_R18, 0, BGBCC_SH_REG_R18);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_R18, BGBCC_SH_REG_GBR);
			}
		}
#endif

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
//		if(sctx->use_fpr)
//			{ k+=sctx->is_addr64?2:1; }
		for(i=15; i>0; i--)
		{
			if(sctx->freg_vsave&(1<<i))
				{ sctx->freg_save|=(1<<i); k+=2; }
		}
#endif
	}
	else
		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
			!(obj->flagsint&BGBCC_TYFL_SYSCALL))
//		if(0)
	{
#if 0
//		if(!(sctx->is_leaf&1) && !(obj->flagsint&BGBCC_TYFL_INTERRUPT))
		if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
			(obj->flagsint&BGBCC_TYFL_SYSCALL))
//		if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
//			(obj->flagsint&BGBCC_TYFL_SYSCALL) ||
//			(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR)))
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=sctx->is_addr64?2:1;
		}

		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=sctx->is_addr64?2:1;
		}
#endif

		fl=0;

#if 1
		if(sctx->is_leaf&1)
		{
			if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
			{
				fl=1;
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
			}else
			{
				fl=0;
			}
		}else
			if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			fl=2;
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R19);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_GBR, BGBCC_SH_REG_R18);
		}else
		{
			fl=1;
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
		}
#endif

		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

		fl2=0;
		k+=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, fl, &fl2);

//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
//			k+=sctx->is_addr64?2:1;

			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1, j);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}

	}
	else
	{
#if 0
//		if(!(sctx->is_leaf&1) && !(obj->flagsint&BGBCC_TYFL_INTERRUPT))
//		if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
//			(obj->flagsint&BGBCC_TYFL_SYSCALL))
		if(1)
		{
//			BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
//				BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);

			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
			k+=sctx->is_addr64?2:1;
		}
#endif

#if 0
//		if(sctx->is_pbo)
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=sctx->is_addr64?2:1;

//			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
//			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
//				BGBCC_SH_NMID_MOVQ,
//				BGBCC_SH_REG_R1, j);
//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}
#endif

#if 1
		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=4;

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-16, BGBCC_SH_REG_SP);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_SP, 8);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_SP, 0);
			}

#if 1
			else
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-24, BGBCC_SH_REG_SP);

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 16);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);

	//			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
	//				BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
	//			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
	//				-8, BGBCC_SH_REG_SP);
	//			BGBCC_JX2_EmitOpRegStReg(sctx, BGBCC_SH_NMID_MOVQ,
	//				BGBCC_SH_REG_R1, BGBCC_SH_REG_SP);
			}
#endif

//			k+=sctx->is_addr64?2:1;
		}else
		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH, BGBCC_SH_REG_GBR);
			k+=2;

			if(sctx->has_fmovc)
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-8, BGBCC_SH_REG_SP);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVC,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_SP, 0);
			}else
			{
				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					-16, BGBCC_SH_REG_SP);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 0);

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_SP, 8);

				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, 0, BGBCC_SH_REG_R1);

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					8, BGBCC_SH_REG_SP);
			}
		}
#endif

		if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
		{
#if 1
			j=k;
			if(sctx->has_bjx1egpr)
				k+=16;
			k+=16;

			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
				-(k-j)*4, BGBCC_SH_REG_SP);

			if(sctx->has_bjx1egpr)
			{
				for(i=23; i>=16; i--)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, ((i-16)+8)*8);
				}
			}

			for(i=7; i>=0; i--)
			{
				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_R0+i, BGBCC_SH_REG_SP, i*8);
			}
#endif

#if 0
			if(sctx->has_bjx1egpr)
			{
				for(i=23; i>=16; i--)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
						BGBCC_SH_REG_R0+i);
					k+=sctx->is_addr64?2:1;
				}
			}

			for(i=7; i>=0; i--)
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_PUSH,
					BGBCC_SH_REG_R0+i);
				k+=sctx->is_addr64?2:1;
			}
#endif
		}

		fl2=0;
		k+=BGBCC_JX2C_EmitFrameProlog_PushRegs(ctx, sctx, 0, &fl2);

		if(obj->regflags&BGBCC_REGFL_ALIASPTR)
		{
			if(sctx->use_wexmd)
			{
				BGBCC_JX2_EmitOpImm(sctx,
					BGBCC_SH_NMID_WEXMD, sctx->use_wexmd);
			}
		}

		if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
		{
			j=BGBCC_JX2_GetNamedLabel(sctx, "__global_ptr");
			BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
				BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1, j);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}
	}


	BGBCC_JX2C_ResetModeDqUnknown(ctx, sctx);
//	BGBCC_JX2C_SetModeDqSet(ctx, sctx);
//	BGBCC_JX2C_ResetModeDqLocal(ctx, sctx);

//	BGBCC_JX2_EmitWord(sctx, 0x0858);	//Debug, Set Token
	
	sctx->frm_offs_save=sctx->frm_size-(k*4);

	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		-sctx->frm_offs_save, BGBCC_SH_REG_SP);

#if 0
	p0=BGBCC_JX2_TryEmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		-sctx->frm_offs_save, BGBCC_SH_REG_SP);
	if(p0<=0)
	{
#if 0
//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
//		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R0, sctx->frm_offs_save);
//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
#endif

#if 1
		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R2, sctx->frm_offs_save);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
			BGBCC_SH_REG_R2, BGBCC_SH_REG_SP);
		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
#endif

	}
#endif

#if 0
	if(sctx->is_pic)
	{
//		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, -16, BGBCC_SH_REG_R12);
		BGBCC_JX2C_EmitSaveFrameReg(ctx, sctx, BGBCC_SH_REG_R12);

//		BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_JX2_EmitWord(sctx, 0xC700);
		BGBCC_JX2_EmitLoadRegLabelRel(sctx, BGBCC_SH_REG_R12, sctx->lbl_got, 0);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R12);
//		BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	}
#endif

//	BGBCC_JX2C_ResetFpscrUnknown(ctx, sctx);

	k=sctx->frm_offs_retstr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		if(sctx->is_addr64)
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
		else
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);

//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
	}
	
	k=sctx->frm_offs_thisptr;
	if(k)
	{
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		if(sctx->is_addr64)
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ3);
		else
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R3);
	}

	
	k=sctx->frm_offs_sectoken;
	if(k)
	{
		BGBCC_JX2_EmitLoadRegImm64P(sctx, BGBCC_SH_REG_RQ0,
//			0x1234567890ABCDEFLL);
			sctx->frm_val_sectoken);
		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ0);
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
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_RD20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RD4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
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
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
							treg, BGBCC_SH_REG_RD20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RD20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RD4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RD4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
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
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
							treg, BGBCC_SH_REG_RQ20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RQ4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
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
//					if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//					if(1)
						BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
							treg, BGBCC_SH_REG_RQ20+(ni-4));
//					else
//						BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//							k, BGBCC_SH_REG_RQ20+(ni-4));
					ni++;
					break;
				}
//				if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//				if(1)
					BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
						treg, BGBCC_SH_REG_RQ4+ni);
//				else
//					BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//						k, BGBCC_SH_REG_RQ4+ni);
				ni++;
				break;
			}

			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
//			if((sctx->vsp_rsv>0) && (pr0<sctx->vsp_rsv))
//			if(1)
				BGBCC_JX2C_EmitStoreFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
//			else
//				BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx,
//					k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;

		case BGBCC_SH_REGCLS_GR2:
		case BGBCC_SH_REGCLS_QGR2:
		case BGBCC_SH_REGCLS_VO_GR2:
		case BGBCC_SH_REGCLS_VO_QGR2:
			if(sctx->abi_evenonly)
				ni=(ni+1)&(~1);
		
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
#if 0
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
#endif
		default:
			BGBCC_DBGBREAK
			break;
		}
	}
	if(vaix<0)
		vaix=obj->n_args;

	if(!strcmp(obj->name, "TKRA_MatrixIdentify"))
	{
		i=-1;
//		sctx->frm_offs_retstr=k-8;
//		BGBCC_DBGBREAK
	}

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
		
		if(obj->locals[i]->flagsint&BGBCC_TYFL_DYNAMIC)
		{
			j=BGBCC_CCXL_LookupGlobalIndex(ctx, obj->locals[i]->name);
			if(j<=0)
				continue;
			tty=ctx->reg_globals[j]->type;
			reg.val=CCXL_REGTY_GLOBAL|
				(((s64)tty.val)<<CCXL_REGID_TYPESHIFT)|j;
			treg.val=CCXL_REGTY_LOCAL|
				(((s64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, tty, treg, reg);
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
#if 0
//						BGBCC_JX2C_ScratchSafeStompReg(
//							ctx, sctx, BGBCC_SH_REG_R0);
//						BGBCC_JX2_EmitLoadRegImm(sctx,
//							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, j*8);
//						BGBCC_JX2_EmitOpRegStReg2(sctx,
//							BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ4+j,
//							BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//						BGBCC_JX2C_ScratchReleaseReg(
//							ctx, sctx, BGBCC_SH_REG_R0);
#endif
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
#if 0
//							BGBCC_JX2C_ScratchSafeStompReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
//							BGBCC_JX2_EmitLoadRegImm(sctx,
//								BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, (j+4)*8);
//							BGBCC_JX2_EmitOpRegStReg2(sctx,
//								BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ20+j,
//								BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//							BGBCC_JX2C_ScratchReleaseReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
#endif
						}
					}
				}else
				{
#if 0
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, 0x55AA55AA);
					for(j=0; j<4; j++)
					{
//						BGBCC_JX2_EmitLoadRegImm(sctx,
//							BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, (j+4)*8);
//						BGBCC_JX2_EmitOpRegStReg2(sctx,
//							BGBCC_SH_NMID_MOVQ, BGBCC_SH_REG_RQ1,
//							BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
					}
#endif
				}

				if(!sctx->no_fpu && !sctx->fpu_gfp)
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
//							BGBCC_JX2C_ScratchSafeStompReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
//							BGBCC_JX2_EmitLoadRegImm(sctx,
//								BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 64+(j*4));
//							BGBCC_JX2_EmitOpRegStReg2(sctx,
//								BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR4+j,
//								BGBCC_SH_REG_RQ0, BGBCC_SH_REG_RQ3);
//							BGBCC_JX2C_ScratchReleaseReg(
//								ctx, sctx, BGBCC_SH_REG_R0);
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
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, ni*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1,
						BGBCC_SH_REG_R3, 96);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, 64+nf*8);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1,
						BGBCC_SH_REG_R3, 104);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);

					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 112);
				}else
				{
#if 0
					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7, ni*8);
//					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 96);
					BGBCC_JX2_EmitOpRegStReg2(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7,
						BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7, 64+nf*4);
//					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 104);
//					BGBCC_JX2_EmitOpRegStReg2(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R7,
//						BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);

					BGBCC_JX2_EmitLoadRegImm(sctx,
						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
						BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);

					BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
//					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 112);
//					BGBCC_JX2_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVQ,
//						BGBCC_SH_REG_R1, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
#endif
				}

//				BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
			}else
			{
			
				BGBCC_DBGBREAK

#if 0
				for(j=0; j<4; j++)
				{
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
						BGBCC_SH_REG_R4+j, BGBCC_SH_REG_R3, j*4);
				}

				if(!sctx->no_fpu && !sctx->fpu_gfp)
				{
//					BGBCC_JX2_EmitLoadRegImm(sctx,
//						BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 48);
//					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
//						BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

//					for(j=7; j>=0; j--)
					for(j=3; j>=0; j--)
					{
//						BGBCC_JX2_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVD,
//							BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_R0);
					}
				}

//				BGBCC_JX2_EmitLoadRegImm(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, ni*4);
//				BGBCC_JX2_EmitOpRegStRegDisp(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 48);

//				BGBCC_JX2_EmitLoadRegImm(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 16+nf*4);
//				BGBCC_JX2_EmitOpRegStRegDisp(sctx,
//					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 52);

#if 1
				BGBCC_JX2_EmitLoadRegImm(sctx,
					BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, sctx->frm_size);
				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
					BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);
#endif

				BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 56);
#endif
			}
		}
	}

	k=sctx->frm_offs_allocamrk;
	if(k)
	{
		BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_begin");
		BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
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

int BGBCC_JX2C_EmitFrameEpilogUnwind(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	if(	(ctx->imgfmt==BGBCC_IMGFMT_ROM) ||
		(ctx->imgfmt==BGBCC_IMGFMT_SYS)	)
			return(0);

	if(sctx->is_leaf&1)
	{
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_PR, BGBCC_SH_REG_R1);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_TBR, 0x40);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R4, BGBCC_SH_REG_PR);
	}else
	{
		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_SP, sctx->frm_size-8,
			BGBCC_SH_REG_R1);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_R1, BGBCC_SH_REG_TBR, 0x40);
		BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
			BGBCC_SH_REG_R4,
			BGBCC_SH_REG_SP, sctx->frm_size-8);
	}

	if(sctx->lbl_ret)
	{
		BGBCC_JX2_EmitOpAutoLabel(sctx,
			BGBCC_SH_NMID_BRAN, sctx->lbl_ret);
	}else
	{
		BGBCC_DBGBREAK
	}

//	sctx->lbl_ret
	return(0);
}

int BGBCC_JX2C_EmitFrameEpilog(BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	ccxl_register reg, treg;
	ccxl_type tty;
	u64 uli;
	int epik, epix, epilbl;
	int bo, co;
	int ob, ov;
	int tr0, tr1;
	int p0, tsz;
	int i, j, k;

	if(sctx->is_leaftiny&8)
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

	if(obj->regflags&BGBCC_REGFL_LEAFTINY)
	{
		i=BGBCC_JX2C_EmitFrameEpilog_TinyLeaf(ctx, sctx, obj);
		return(i);
	}

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
		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

//		BGBCC_JX2C_ResetFpscrLocal(ctx, sctx);
		BGBCC_JX2_EmitLabel(sctx, sctx->lbl_ret);
	}

	for(i=0; i<obj->n_locals; i++)
	{
		if(obj->locals[i]->flagsint&BGBCC_TYFL_DYNAMIC)
		{
			j=BGBCC_CCXL_LookupGlobalIndex(ctx, obj->locals[i]->name);
			if(j<=0)
				continue;
			tty=ctx->reg_globals[j]->type;
			reg.val=CCXL_REGTY_GLOBAL|
				(((s64)tty.val)<<CCXL_REGID_TYPESHIFT)|j;
			treg.val=CCXL_REGTY_LOCAL|
				(((s64)tty.val)<<CCXL_REGID_TYPESHIFT)|i;
			BGBCC_JX2C_EmitMovVRegVReg(ctx, sctx, tty, reg, treg);
			obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;
		}
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
		obj->regflags|=BGBCC_REGFL_NOTLEAFTINY;

		tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);

		if(sctx->is_addr64)
		{
#if 0
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ2);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_RQ2, BGBCC_SH_REG_RQ0, tsz, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ2);
#endif

#if 1
			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ3);
			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
				BGBCC_SH_REG_RQ3, BGBCC_SH_REG_RQ2, tsz, 8);
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ3);
#endif

		}else
		{
#if 0
//			BGBCC_JX2C_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);
//			BGBCC_JX2C_EmitValueCopyRegRegSz(ctx, sctx,
//				BGBCC_SH_REG_R2, BGBCC_SH_REG_R0, tsz, 4);
//			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
#endif
		}
	}

	k=sctx->frm_offs_allocamrk;
	if(k)
	{
		if(BGBCC_JX2C_CheckFrameSavedLpReg(ctx, sctx, BGBCC_SH_REG_LR12))
		{
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_LR2, BGBCC_SH_REG_LR12);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_end");
			BGBCC_JX2C_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_LR12, BGBCC_SH_REG_LR2);
		}else
		{
			BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, 0, BGBCC_SH_REG_LR2);
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ4);
			BGBCC_JX2C_EmitCallName(ctx, sctx, "__alloca_end");
			BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, BGBCC_SH_REG_LR2);
		}
	}

	k=sctx->frm_offs_sectoken;
	if(k)
	{
		BGBCC_JX2_EmitLoadRegImm64P(sctx, BGBCC_SH_REG_RQ17,
//			0x1234567890ABCDEFLL);
			sctx->frm_val_sectoken);
		BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_RQ16);
		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_CMPQEQ,
			BGBCC_SH_REG_RQ16, BGBCC_SH_REG_RQ17);
		sctx->op_is_wex2=5;
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
		sctx->op_is_wex2=0;
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

//	j=BGBCC_JX2_EmitCheckAutoLabelNear16B(sctx, epilbl, 
	j=BGBCC_JX2_EmitCheckAutoLabelNear20B(sctx, epilbl, 
		BGBCC_SH_NMID_BSR);
	if(epik && (sctx->epihash_key[epix]==epik) && (epilbl>0) &&
		!sctx->is_simpass && j)
	{
//		if((j&3)==3)
		if((j&15)==1)
		{
			BGBCC_JX2_EmitOpLabel(sctx,
				BGBCC_SH_NMID_BRAN, epilbl);
		}else
		{
//			BGBCC_JX2_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, epilbl);
//			BGBCC_JX2_EmitOpFar16Label(sctx,
			BGBCC_JX2_EmitOpFar20Label(sctx,
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
		if(sctx->is_align_wexj&2)
			{ BGBCC_JX2_EmitPad32AlignLastOp(sctx); }

		epilbl=BGBCC_JX2_GenLabel(sctx);
		BGBCC_JX2_EmitLabel(sctx, epilbl);

		sctx->epihash_key[epix]=epik;
		sctx->epihash_lbl[epix]=epilbl;
	}
#endif

//	BGBCC_JX2_EmitWord(sctx, 0x0848);	//Debug, Clear Token

#if 0
//	for(i=0; i<16; i++)
	for(i=0; i<32; i++)
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

	if(sctx->use_fpr && !sctx->no_fpu && !sctx->fpu_gfp)
	{
//		BGBCC_JX2_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_LDSL,
//			BGBCC_SH_REG_SP, BGBCC_SH_REG_FPSCR);
//		k+=sctx->is_addr64?8:4;
	}
#endif

//	if(	(ctx->optmode==BGBCC_OPT_SPEED) ||
//		(ctx->optmode==BGBCC_OPT_SPEED2) )
	if(1)
	{
		j=k;
	
	//	for(i=0; i<15; i++)
		for(i=0; i<32; i++)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

#if 1
			if(sctx->has_pushx2 && !(i&1))
			{
				if(((sctx->reg_save&(3<<i))==(3<<i)) && (i!=14) && (i!=31))
				{
//					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POPX2,
//						BGBCC_SH_REG_R0+i);
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_R0+i);
					k+=16;
					i++;
					continue;
				}
			}
#endif

			if(sctx->reg_save&(1<<i))
			{
//				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
//					BGBCC_SH_REG_RQ0+i);
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_R0+i);
				k+=8;
			}
		}

//		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
//			!(obj->flagsint&BGBCC_TYFL_SYSCALL) &&
//			!(sctx->is_leaf&1)	)
//		if(0)
		if(	!(obj->flagsint&BGBCC_TYFL_INTERRUPT) &&
			!(obj->flagsint&BGBCC_TYFL_SYSCALL))
		{
#if 1
			if(sctx->is_leaf&1)
			{
				if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
				{
					if(((k+8)!=sctx->frm_size) && !sctx->is_simpass)
						{ BGBCC_DBGBREAK; }

					if(sctx->has_fmovc)
					{
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
							BGBCC_SH_REG_SP, (k-j),
							BGBCC_SH_REG_GBR);
					}else
					{
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j),
							BGBCC_SH_REG_R1);
						BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
							BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
					}
					k+=8;

					BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);
					if(BGBCC_JX2_CheckPipelineMin(sctx, 6))
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU); }
					else
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS); }
				}else
				{
					BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);
					if(BGBCC_JX2_CheckPipelineMin(sctx, 6))
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU); }
					else
						{ BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS); }
				}
			}
			else
#endif
				if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
			{
				if(((k+16)!=sctx->frm_size) && !sctx->is_simpass)
					{ BGBCC_DBGBREAK; }

//				if(sctx->has_pushx2)
				if(0)
				{
//					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
//						BGBCC_SH_REG_SP, (k-j),
//						BGBCC_SH_REG_R18);
				}else
				{
					if(sctx->has_fmovc)
					{
						BGBCC_JX2_EmitOpLdRegDispReg(sctx,
							BGBCC_SH_NMID_MOVC,
							BGBCC_SH_REG_SP, (k-j)+0,
							BGBCC_SH_REG_GBR);
//						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
//							BGBCC_SH_REG_SP, (k-j)+8,
//							BGBCC_SH_REG_PR);
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j)+8,
							BGBCC_SH_REG_R1);
					}else
					{
						BGBCC_JX2_EmitOpLdRegDispReg(sctx,
							BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j)+0,
							BGBCC_SH_REG_R18);
						BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
							BGBCC_SH_REG_SP, (k-j)+8,
	//						BGBCC_SH_REG_R19);
							BGBCC_SH_REG_R1);
					}
				}
				k+=16;

				BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
					k-j, BGBCC_SH_REG_SP);

				if(sctx->has_fmovc)
				{
//					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
					BGBCC_JX2_EmitOpReg(sctx,
						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);
				}else
				{
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						BGBCC_SH_REG_R18, BGBCC_SH_REG_GBR);
//					BGBCC_JX2_EmitOpReg(sctx,
//						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R19);
					BGBCC_JX2_EmitOpReg(sctx,
						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);
				}
			}else
			{
				if(((k+8)!=sctx->frm_size) && !sctx->is_simpass)
					{ BGBCC_DBGBREAK; }

//				if(sctx->has_fmovc)
				if(0)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_PR);
					k+=8;

					BGBCC_JX2_EmitOpImmReg(sctx,
						BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);

					BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
				}else
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_R1);
					k+=8;

					BGBCC_JX2_EmitOpImmReg(sctx,
						BGBCC_SH_NMID_ADD,
						k-j, BGBCC_SH_REG_SP);

					BGBCC_JX2_EmitOpReg(sctx,
						BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);
				}
			}


//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PR);
//			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTE);

			BGBCC_JX2_EmitFlushIndexImmBasic(sctx);
			BGBCC_JX2C_EmitLabelFlushRegisters(ctx, sctx);

			if(sctx->is_leaf==2)
				sctx->is_leaf=1;

			co=BGBCC_JX2_EmitGetOffs(sctx);
			sctx->fnsz_epi=co-bo;

			return(0);

		}
		else
		{
			BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
				k-j, BGBCC_SH_REG_SP);
		}
	}else
	{

		BGBCC_DBGBREAK

#if 0
	//	for(i=0; i<15; i++)
		for(i=0; i<32; i++)
		{
			if(!(sctx->has_bjx1egpr) && (i>=16))
				continue;
			if(i==15)
				continue;

	#if 1
			if(sctx->has_pushx2 && !(i&1))
	//		if(0)
			{
				if(((sctx->reg_save&(3<<i))==(3<<i)) && (i!=14) && (i!=31))
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POPX2,
						BGBCC_SH_REG_R0+i);
					k+=sctx->is_addr64?16:8;
					i++;
					continue;
				}
			}
	#endif

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
#endif

	}

	j=k;

	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{

#if 1
		if(sctx->has_pushx2)
		{
#if 1
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_R0);
			k+=8;
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_R1);
			k+=8;
#endif

//			for(i=0; i<8; i+=2)
			for(i=2; i<8; i+=2)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_R0+i);
				k+=16;
			}

			for(i=16; i<24; i+=2)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVX2,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_R0+i);
				k+=16;
			}
		}else
		{
			for(i=0; i<8; i++)
			{
				BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
					BGBCC_SH_REG_SP, (k-j),
					BGBCC_SH_REG_R0+i);
				k+=8;
			}

			if(sctx->has_bjx1egpr)
			{
				for(i=16; i<24; i++)
				{
					BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
						BGBCC_SH_REG_SP, (k-j),
						BGBCC_SH_REG_R0+i);
					k+=8;
				}
			}
		}

//		BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//			k-j, BGBCC_SH_REG_SP);
#endif


#if 0
		for(i=0; i<8; i++)
		{
			if(sctx->is_addr64)
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
					BGBCC_SH_REG_RQ0+i);
				k+=8;
			}else
			{
				BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
					BGBCC_SH_REG_R0+i);
				k+=4;
			}
		}

//		if(sctx->has_bjx1egpr)
		if(1)
		{
			for(i=16; i<24; i++)
			{
				if(sctx->is_addr64)
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
						BGBCC_SH_REG_RQ0+i);
					k+=8;
				}else
				{
					BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP,
						BGBCC_SH_REG_R0+i);
					k+=4;
				}
			}
		}
#endif
	}

#if 1
	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		if(!sctx->has_fmovc)
		{
	//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
	//			BGBCC_SH_REG_R1, BGBCC_SH_REG_TEA);
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_R1,
				BGBCC_SH_REG_SP, 0);
		}
	}
#endif

//	if(sctx->is_pbo)
	if(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR))
	{
		if(sctx->has_fmovc)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_GBR);
		}
		else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_GBR);
		}
		k+=8;

//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_GBR);
//		k+=sctx->is_addr64?8:4;
	}

//	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
//		k-j, BGBCC_SH_REG_SP);

//	if(!(sctx->is_leaf&1) && !(obj->flagsint&BGBCC_TYFL_INTERRUPT))
	if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
		(obj->flagsint&BGBCC_TYFL_SYSCALL))
//	if(!(sctx->is_leaf&1) || (obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
//		(obj->flagsint&BGBCC_TYFL_SYSCALL) ||
//		(sctx->is_pbo && (obj->regflags&BGBCC_REGFL_ALIASPTR)))
	{
		if(sctx->has_fmovc)
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVC,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_PR);
		}else
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, (k-j),
				BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_PR);
		}

		k+=8;

//		if(sctx->is_addr64)
//			k+=8;
//		else
//			k+=4;
	}

#if 1
	if(obj->flagsint&BGBCC_TYFL_INTERRUPT)
	{
		if(!sctx->has_fmovc)
		{
	//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
	//			BGBCC_SH_REG_TEA, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVQ,
				BGBCC_SH_REG_SP, 0,
				BGBCC_SH_REG_R1);
		}
	}
#endif

	BGBCC_JX2_EmitOpImmReg(sctx, BGBCC_SH_NMID_ADD,
		k-j, BGBCC_SH_REG_SP);

	if((k!=sctx->frm_size) && !sctx->is_simpass)
		{ BGBCC_DBGBREAK; }
#endif

	if((obj->flagsint&BGBCC_TYFL_INTERRUPT) ||
		(obj->flagsint&BGBCC_TYFL_SYSCALL))
	{
//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PR);
//		BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
//			BGBCC_SH_REG_R1, BGBCC_SH_REG_PR);

		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);

		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTE);

		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	}
	else
		if(!(sctx->is_leaf&1))
	{

		BGBCC_DBGBREAK

//		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_POP, BGBCC_SH_REG_PC);
//		BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RET);
		BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_JMP, BGBCC_SH_REG_R1);

	}else
	{
//		if(BGBCC_JX2_CheckPipelineMin(sctx, 4))
		if(BGBCC_JX2_CheckPipelineMin(sctx, 6))
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTSU);
		}else
		{
			BGBCC_JX2_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
		}
	}

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
