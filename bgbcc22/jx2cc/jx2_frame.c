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


int BGBCC_JX2C_EmitLoadFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int nmid, int ofs, int dreg)
{
	int dreg2, treg;
	int p0, p1;
	int ofs1;
	int i, j, k;

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_JX2C_EmitLoadFrameOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&31);
		
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }
	
		dreg2=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, dreg);

//		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
//			BGBCC_SH_REG_SP, ofs1+4, dreg2+1);
//		BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
//			BGBCC_SH_REG_SP, ofs1+0, dreg2+0);

		if(ofs1&7)
			{ BGBCC_DBGBREAK }

//		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs1&7))
		if(sctx->has_pushx2)
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

	if(nmid<=0)
	{
		if(BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
			nmid=BGBCC_SH_NMID_MOVQ;
		else
			nmid=BGBCC_SH_NMID_MOVL;
	}

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

#if 0
	if(BGBCC_JX2_EmitCheckRegQuad(sctx, dreg))
	{
		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		nmid=BGBCC_SH_NMID_MOVQ;
	}
#endif

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		nmid=BGBCC_SH_NMID_MOVX2;
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

	if((ofs>=sctx->frm_offs_fix) && (ofs<0) && !(sctx->is_prolog))
		{ BGBCC_DBGBREAK }
		
	if(ofs>(24*8))
		{ BGBCC_DBGBREAK }

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_JX2C_EmitStoreFrameOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&31);
		
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, dreg);

		if(ofs1&7)
			{ BGBCC_DBGBREAK }
	
//		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs1&7))
		if(sctx->has_pushx2)
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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		BGBCC_JX2C_CheckSetModeDqSet(ctx, sctx);
		nmid=BGBCC_SH_NMID_MOVX2;
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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&31);
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, dreg);

		nm1=BGBCC_SH_NMID_LEAB;

		BGBCC_JX2_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_GBH, dreg2+1);

		BGBCC_JX2_EmitOpLdRegDispReg(sctx, nm1,
			BGBCC_SH_REG_SP, ofs1, dreg2);
		return(1);
	}

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
	int ofs1, ofs0;
	int i, j, k;

#if 1
	ofs0=ofs-(sctx->frm_size);
	if((ofs0>=sctx->frm_offs_fix) && (ofs0<0) && !(sctx->is_prolog))
		{ BGBCC_DBGBREAK }
		
	if(ofs0>(24*8))
		{ BGBCC_DBGBREAK }
#endif

	if(BGBCC_JX2C_EmitRegIsFpReg(ctx, sctx, dreg) ||
		BGBCC_JX2C_EmitRegIsDpReg(ctx, sctx, dreg))
		{ return(BGBCC_JX2C_EmitStoreStackOfsFpReg(ctx, sctx, ofs, dreg)); }

	ofs1=ofs;

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&31);	
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, dreg);

		if(ofs1&7)
			{ BGBCC_DBGBREAK }
		
//		if(sctx->has_pushx2 && !(dreg2&1) && !(ofs1&7))
		if(sctx->has_pushx2)
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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg))
	{
		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
		{
			BGBCC_JX2_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_XMOVX2,
				breg, ofs, dreg);
			if(nmid2>=0)
				{ BGBCC_JX2_EmitOpRegReg(sctx, nmid2, dreg, dreg); }
			return(1);
		}

		switch(nmid)
		{
			case BGBCC_SH_NMID_MOVB: nmid=BGBCC_SH_NMID_XMOVB; break;
			case BGBCC_SH_NMID_MOVUB: nmid=BGBCC_SH_NMID_XMOVUB; break;
			case BGBCC_SH_NMID_MOVW: nmid=BGBCC_SH_NMID_XMOVW; break;
			case BGBCC_SH_NMID_MOVUW: nmid=BGBCC_SH_NMID_XMOVUW; break;
			case BGBCC_SH_NMID_MOVL: nmid=BGBCC_SH_NMID_XMOVL; break;
			case BGBCC_SH_NMID_MOVUL: nmid=BGBCC_SH_NMID_XMOVUL; break;
			case BGBCC_SH_NMID_MOVQ: nmid=BGBCC_SH_NMID_XMOVQ; break;
			case BGBCC_SH_NMID_MOVX2: nmid=BGBCC_SH_NMID_XMOVX2; break;
		}

		BGBCC_JX2_TryEmitOpLdRegDispReg(sctx, nmid,
			breg, ofs, dreg);
		if(nmid2>=0)
			{ BGBCC_JX2_EmitOpRegReg(sctx, nmid2, dreg, dreg); }
		return(1);
	}


	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&31);		
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, dreg);

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

//		dreg2=BGBCC_SH_REG_FR0+(dreg&31);		
		if(dreg2&1)
			{ dreg2=(dreg2&(~1))+32; }

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

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg))
	{
		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
		{
			BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_XMOVX2,
				dreg, breg, ofs);
			return(1);
		}

		switch(nmid)
		{
			case BGBCC_SH_NMID_MOVB: nmid=BGBCC_SH_NMID_XMOVB; break;
			case BGBCC_SH_NMID_MOVUB: nmid=BGBCC_SH_NMID_XMOVUB; break;
			case BGBCC_SH_NMID_MOVW: nmid=BGBCC_SH_NMID_XMOVW; break;
			case BGBCC_SH_NMID_MOVUW: nmid=BGBCC_SH_NMID_XMOVUW; break;
			case BGBCC_SH_NMID_MOVL: nmid=BGBCC_SH_NMID_XMOVL; break;
			case BGBCC_SH_NMID_MOVUL: nmid=BGBCC_SH_NMID_XMOVUL; break;
			case BGBCC_SH_NMID_MOVQ: nmid=BGBCC_SH_NMID_XMOVQ; break;
			case BGBCC_SH_NMID_MOVX2: nmid=BGBCC_SH_NMID_XMOVX2; break;
		}

		BGBCC_JX2_TryEmitOpRegStRegDisp(sctx, nmid,
			dreg, breg, ofs);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
//		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		dreg2=BGBCC_SH_REG_R0+(dreg&31);
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToGr(ctx, sctx, dreg);

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

	if(	BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) ||
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
	{
		if(	!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) ||
			!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
		{
			BGBCC_DBGBREAK
		}

		if(!ofs)
		{
			BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_XMOV, breg, dreg);
			return(1);
		}

		opw1=BGBCC_SH_NMID_XLEAB;
		BGBCC_JX2_EmitOpLdRegDispReg(sctx,
			opw1, breg, ofs, dreg);
		return(1);
	}

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
//		if((((sbyte)ofs)==ofs) && (breg==dreg) && ((breg&31)<16))
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

		if(sctx->abi_spillpad&4)
//		if(0)
		{
			opw1=BGBCC_SH_NMID_LEATB;
//			if(ofs>=512)
			if(ofs>=32)
			{
				if(!(ofs&1))
					opw1=BGBCC_SH_NMID_LEATW;
				if(!(ofs&3))
					opw1=BGBCC_SH_NMID_LEATL;
				if(!(ofs&7))
					opw1=BGBCC_SH_NMID_LEATQ;
			}
		}

		if(sctx->abi_spillpad&4)
		{
//			BGBCC_JX2_EmitOpInnReg(sctx,
//				BGBCC_SH_NMID_BNDCHK, ofs, breg);
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

		if(sctx->abi_spillpad&4)
//		if(0)
		{
			opw1=BGBCC_SH_NMID_LEATB;
//			if(ofs>=512)
			if(ofs>=32)
			{
				if(!(ofs&1))
					opw1=BGBCC_SH_NMID_LEATW;
				if(!(ofs&3))
					opw1=BGBCC_SH_NMID_LEATL;
				if(!(ofs&7))
					opw1=BGBCC_SH_NMID_LEATQ;
			}
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

	case BGBCC_SH_NMID_FMOVH: rt=(sc==2); break;
	case BGBCC_SH_NMID_FMOVS: rt=(sc==4); break;
	case BGBCC_SH_NMID_FMOVD: rt=(sc==8); break;


	case BGBCC_SH_NMID_XMOVB: rt=(sc==1); break;
	case BGBCC_SH_NMID_XMOVW: rt=(sc==2); break;
	case BGBCC_SH_NMID_XMOVL: rt=(sc==4); break;
	case BGBCC_SH_NMID_XMOVQ: rt=(sc==8); break;
	case BGBCC_SH_NMID_XLEAB: rt=(sc==1); break;
	case BGBCC_SH_NMID_XLEAW: rt=(sc==2); break;
	case BGBCC_SH_NMID_XLEAL: rt=(sc==4); break;
	case BGBCC_SH_NMID_XLEAQ: rt=(sc==8); break;

	case BGBCC_SH_NMID_XMOVUB: rt=(sc==1); break;
	case BGBCC_SH_NMID_XMOVUW: rt=(sc==2); break;
	case BGBCC_SH_NMID_XMOVUL: rt=(sc==4); break;

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

#if 1
	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_LR0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) &&
		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, dreg) &&
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

		BGBCC_DBGBREAK
	}
#endif

	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, breg) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) &&
		(sc==16)
		)
	{
//		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
//		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
//		}

//		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);

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


	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) &&
		(sc==16)
		)
	{
//		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);

		tr0=dreg2;
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHAD, ireg, 1, tr0);
		BGBCC_JX2_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_XMOVX2,
			breg, tr0, dreg);
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
	int dreg2, sw, shl;
	int nm1, nm2;
	int tr0, tr1;
	int p0, p1;
	int i, j, k;

#if 1
	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, breg) &&
//		!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) &&
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

#if 1
	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_LR0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) &&
		BGBCC_JX2C_EmitRegIsExtGpReg(ctx, sctx, dreg) &&
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

		BGBCC_DBGBREAK
	}
#endif

#if 1
	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_LR0) &&
		(ireg!=BGBCC_SH_REG_LR0) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, breg) &&
		BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg) &&
		(sc>=16) && !(sc&(sc-1)))
	{
//		if(BGBCC_JX2_EmitCheckRegNeedSzx(sctx, ireg))
//		{
//			BGBCC_JX2_EmitOpReg(sctx, BGBCC_SH_NMID_EXTSL, ireg);
//		}

		k=sc; shl=0;
		while(k>1)
			{ k=k>>1; shl++; }

		tr0=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_JX2_EmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_SHAD, ireg, shl-3, tr0);

		i=BGBCC_JX2_TryEmitOpRegStReg2(sctx, BGBCC_SH_NMID_XMOVX2,
			dreg, tr0, breg);
		if(i>0)
		{
			BGBCC_JX2C_ScratchReleaseReg(ctx, sctx, tr0);
			return(1);
		}

		BGBCC_DBGBREAK
	}
#endif

	if(sctx->has_bjx1mov &&
		(breg!=BGBCC_SH_REG_R0) &&
		(ireg!=BGBCC_SH_REG_R0) &&
		(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg)) &&
		(sc==16)
		)
	{
//		dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
//		if(dreg2&1)
//			{ dreg2=(dreg2&(~1))+32; }

		dreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);

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

int BGBCC_JX2C_EmitLoadFrameNmidForVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	ccxl_type sty;
	int nmid;
	
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

#if 1
	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
		nmid=BGBCC_SH_NMID_MOVL;
//		if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
		if(BGBCC_CCXL_TypeUnsignedIntP(ctx, sty))
			nmid=BGBCC_SH_NMID_MOVUL;
		return(nmid);
	}
#endif

#if 0
	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
		nmid=BGBCC_SH_NMID_MOVQ;
//		if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
//			nmid=BGBCC_SH_NMID_MOVUL;
		return(nmid);
	}
#endif

//	nmid=BGBCC_SH_NMID_MOVL;

	return(0);
}

int BGBCC_JX2C_EmitMarkFrameVReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg)
{
	int p0, tr0, rcls, nm1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->locals[j]->regcls;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;
		return(1);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->regs[j]->regcls;
		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;
		return(1);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->args[j]->regcls;
		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;
		return(1);
	}

	return(0);
}

int BGBCC_JX2C_EmitLoadFrameVRegByValReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, tr0, rcls, nm1;
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

		if(	(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{ BGBCC_DBGBREAK }

		nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		rcls=ctx->cur_func->regs[j]->regcls;

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;

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

		if(	(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{ BGBCC_DBGBREAK }

		nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, dreg);
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

		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;

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
		
		if(	(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{ BGBCC_DBGBREAK }

		nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

#if 1
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, dreg);
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

		if(	(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{ BGBCC_DBGBREAK }

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

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;

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

		if(	(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{ BGBCC_DBGBREAK }

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

		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;

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

		if(	(rcls!=BGBCC_SH_REGCLS_VO_GR2) &&
			(rcls!=BGBCC_SH_REGCLS_VO_QGR2) &&
			BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{ BGBCC_DBGBREAK }

		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, dreg);
		return(1);
	}

	p0=BGBCC_JX2C_EmitStoreFrameVRegReg(ctx, sctx, sreg, dreg);
	return(p0);
}

int BGBCC_JX2C_SizeToFp8(int sz)
{
	int v, e, i;

	if(sz<0)
		return(0);
	
	if(sz<16)
		return(sz);
	
	v=sz; e=1;
	while(v>=16)
		{ v=(v+1)>>1; e++; }
	i=(e<<3)|(v&7);
	return(i);
}

int BGBCC_JX2C_Fp8ToSize(int v)
{
	int f, e, sz;
	if(v<16)
		return(v);

	f=8|(v&7); e=(v>>3)&31;
	sz=f<<(e-1);
	return(sz);
}

int BGBCC_JX2C_SizeToBndTag16(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int sz, ccxl_type tty)
{
	int i, tt, sc;
	
	tt=-1;
	switch(tty.val)
	{
	case CCXL_TY_SB:
		tt=0; sc=1;	break;
	case CCXL_TY_SS:
		tt=1; sc=2;	break;
	case CCXL_TY_I:
		tt=2; sc=4;	break;
	case CCXL_TY_L:
	case CCXL_TY_UL:
		tt=3; sc=8;	break;
	case CCXL_TY_NL:
	case CCXL_TY_UNL:
		tt=3; sc=8;	break;

	case CCXL_TY_UB:
		tt=4; sc=1;	break;
	case CCXL_TY_US:
		tt=5; sc=2;	break;
	case CCXL_TY_UI:
		tt=6; sc=4;	break;

	case CCXL_TY_F16:
		tt=9; sc=2;		break;
	case CCXL_TY_F:
		tt=10; sc=4;	break;
	case CCXL_TY_D:
		tt=11; sc=8;	break;

	case CCXL_TY_I128:
	case CCXL_TY_UI128:
		tt=15; sc=8;	break;
	
	default:
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, tty))
			{
				tt=14; sc=8;
			}else
			{
				tt=12; sc=8;
			}
		}
		break;
	}
	
	if(tt<0)
		return(0);
	
	i=BGBCC_JX2C_SizeToFp8(sz*sc);
//	i=BGBCC_JX2C_SizeToFp8(sz*sc+3);
	if(!i)
		return(0);
	
//	return(0x3000|(tt<<8)|i);
//	return(0x3000|i);
	return(0x3100|(i+2));		/* lax pad */
}

int BGBCC_JX2C_CheckPadToBndTag16(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int sz)
{
	int sz1, tg;
	
	if(!(sctx->abi_spillpad&4))
		return(sz);

//	tg=BGBCC_JX2C_SizeToFp8(sz+3);
	tg=BGBCC_JX2C_SizeToFp8(sz)+2;
	sz1=BGBCC_JX2C_Fp8ToSize(tg);
	return(sz1);
}

int BGBCC_JX2C_CheckPadOffsetToBndTag16(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	int ofs, int sz)
{
	int ofs1, tg;
	
	if(!(sctx->abi_spillpad&4))
		return(ofs);

	tg=BGBCC_JX2C_SizeToFp8(sz);
	tg+=2;

	if(tg<0x10)
		return(ofs);
	ofs1=ofs+(1<<((tg>>3)-1));
//	sz1=BGBCC_JX2C_Fp8ToSize(tg);
	return(ofs1);
//	return(ofs);
}

int BGBCC_JX2C_EmitLoadFrameVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register sreg, int dreg)
{
	ccxl_register tvreg;
	ccxl_type tty, bty;
	char *s0;
	double f, g;
	float sf;
	u16 usk;
	s64 li, lj;
	int dreg2, treg, rcls;
	int nm1, nm2, nm3, asz, tag;
	int p0, p1, tr0, tr1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
		{
//			BGBCC_DBGBREAK
			return(-1);
		}
	
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

			if(sctx->abi_spillpad&4)
			{
				BGBCC_CCXL_TypeDerefType(ctx, tty, &bty);
				asz=BGBCC_CCXL_TypeGetArrayDimSize(ctx, tty);
//				tag=BGBCC_JX2C_SizeToBndTag16(ctx, sctx, asz+3, bty);
				tag=BGBCC_JX2C_SizeToBndTag16(ctx, sctx, asz, bty);
				if(tag>0)
				{
					BGBCC_JX2_EmitOpRegImmReg(sctx,
						BGBCC_SH_NMID_MOVTT, dreg, tag, dreg);
				}
			}

			return(i);
		}

#if 1
//		if(BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if(	(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF) ||
			(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF2))
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
			if((ctx->arch_sizeof_ptr==16) &&
				!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
					{ BGBCC_DBGBREAK }
		
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
				
				nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, treg);
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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
//			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
//				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
				dreg=BGBCC_SH_REG_RQ0|(dreg&63);
//			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
//				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
				dreg=BGBCC_SH_REG_RQ0|(dreg&63);
		}

		nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

//		k=sctx->frm_offs_lcl+j*4;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
		{
//			BGBCC_DBGBREAK
			return(-1);
		}
	
		j=sreg.val&CCXL_REGID_BASEMASK;
		tty=ctx->cur_func->regs[j]->type;
		if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{
			k=1;
		}

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;

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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
			if((ctx->arch_sizeof_ptr==16) &&
				!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
					{ BGBCC_DBGBREAK }

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
//			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
//				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
				dreg=BGBCC_SH_REG_RQ0|(dreg&63);
//			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
//				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
				dreg=BGBCC_SH_REG_RQ0|(dreg&63);
		}

		nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

		j=sreg.val&CCXL_REGID_BASEMASK;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		if(sctx->is_leaftiny&1)
		{
//			BGBCC_DBGBREAK
			return(-1);
		}
	
		j=sreg.val&CCXL_REGID_BASEMASK;
		tty=ctx->cur_func->args[j]->type;

		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;

		if((ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_GR) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_GR2) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_QGR2) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_QGR))
		{
			if((ctx->arch_sizeof_ptr==16) &&
				!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
					{ BGBCC_DBGBREAK }

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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
				i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, treg);
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
//			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
//				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
				dreg=BGBCC_SH_REG_RQ0|(dreg&63);
//			if((dreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			if((dreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
//				dreg=BGBCC_SH_REG_RQ0|(dreg&31);
				dreg=BGBCC_SH_REG_RQ0|(dreg&63);
		}
		
		nm1=BGBCC_JX2C_EmitLoadFrameNmidForVReg(ctx, sctx, sreg);

//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, nm1, k, dreg);
		return(i);
	}
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
#if 0
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
#endif

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
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
#if 0
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
#endif

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
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
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

#if 0
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
#endif

		j=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
		BGBCC_JX2_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, dreg, j);
		return(1);
	}

	if(BGBCC_CCXL_IsRegImmFloatP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmDoubleP(ctx, sreg))
	{
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
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
//			if(k && ((*(float *)(&j))==f) && sctx->fpu_gfp)
			if(k && ((*(float *)(&j))==f) && sctx->fpu_gfp &&
				BGBCC_JX2_EmitCheckRegExt32GPR(sctx, dreg))
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
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
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
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
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
		ctx->cur_func->regflags|=BGBCC_REGFL_IMMLOAD;
	
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

				if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
				{
//					tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);
//					tr1=tr0+1;

					nm1=BGBCC_SH_NMID_MOVX2;
					j=BGBCC_CCXL_GetRegID(ctx, sreg);
					k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						nm1, dreg, k);
					return(1);
				}
				else
				{
					nm1=BGBCC_SH_NMID_MOVP;
				
					j=BGBCC_CCXL_GetRegID(ctx, sreg);
					k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
					BGBCC_JX2_EmitLoadRegLabelVarRel24(sctx,
						nm1, dreg, k);
					return(1);
				}
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
				if(	(ctx->arch_sizeof_long==8) ||
					(ctx->arch_sizeof_long==16))
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
//			if(ctx->arch_sizeof_ptr==16)
			if(0)
			{
				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
					BGBCC_SH_REGCLS_QGR2);
			}
			else
			{
				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);
			}
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
				if(ctx->arch_sizeof_ptr==16)
				{
					tr1=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						BGBCC_SH_REG_PCH, tr1+1);
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						treg, tr1+0);

//					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_XMOV,
//						treg, dreg);
				}else
				{
					BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
						treg, dreg);
				}
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
//			dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
//			if(dreg2&1)
//				{ dreg2=(dreg2&(~1))+32; }

			dreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);

			BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
//			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+0, li);
//			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+1, lj);
			BGBCC_JX2_EmitLoadRegImm128P(sctx, dreg2, li, lj);
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
//			dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
//			if(dreg2&1)
//				{ dreg2=(dreg2&(~1))+32; }

			dreg2=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);

			BGBCC_CCXL_GetRegImmX128Value(ctx, sreg, &li, &lj);
//			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+0, li);
//			BGBCC_JX2_EmitLoadRegImm64P(sctx, dreg2+1, lj);
			BGBCC_JX2_EmitLoadRegImm128P(sctx, dreg2, li, lj);
			return(1);
		}

		if(1)
		{
//			dreg2=BGBCC_SH_REG_RQ0+(dreg&31);
			dreg2=BGBCC_SH_REG_RQ0+(dreg&63);
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
			i=BGBCC_JX2C_EmitLoadFrameOfsReg(ctx, sctx, 0, k, dreg);
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
			if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, sreg))
				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
					BGBCC_SH_REGCLS_QGR2);
			else
				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 0);

			tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx,
				ctx->cur_func->locals[j]->type);

			k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);
			if((ctx->cur_func->locals[j]->regcls!=BGBCC_SH_REGCLS_VO_REF) &&
				(ctx->cur_func->locals[j]->regcls!=BGBCC_SH_REGCLS_VO_REF2))
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
					if(k>=sctx->frm_offs_fix)
						{ BGBCC_DBGBREAK }

					k=k+(sctx->frm_size);
					BGBCC_JX2_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
						sreg, BGBCC_SH_REG_SP, k);
					return(1);
				}
				
//				treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				treg=BGBCC_SH_REG_R1;
				k=ctx->cur_func->locals[j]->fxoffs;
				if(k>=sctx->frm_offs_fix)
					{ BGBCC_DBGBREAK }

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
				if(k>=sctx->frm_offs_fix)
					{ BGBCC_DBGBREAK }

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
				if(k>=sctx->frm_offs_fix)
					{ BGBCC_DBGBREAK }

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
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0)
//				sreg=BGBCC_SH_REG_RD0|(sreg&31);
				sreg=BGBCC_SH_REG_RD0|(sreg&63);
		}
		
		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
//				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
				sreg=BGBCC_SH_REG_RQ0|(sreg&63);
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
//				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
				sreg=BGBCC_SH_REG_RQ0|(sreg&63);
		}

		k=ctx->cur_func->locals[j]->fxoffs;
		if(k>=sctx->frm_offs_fix)
			{ BGBCC_DBGBREAK }

		i=BGBCC_JX2C_EmitStoreFrameOfsReg(ctx, sctx, k, sreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, dreg))
	{
		if(sctx->is_leaftiny&1)
			{ BGBCC_DBGBREAK }
	
		j=dreg.val&CCXL_REGID_BASEMASK;

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;

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
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RQ0)
//				sreg=BGBCC_SH_REG_RD0|(sreg&31);
				sreg=BGBCC_SH_REG_RD0|(sreg&63);
		}
		
		if(rcls==BGBCC_SH_REGCLS_QGR)
		{
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
//				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
				sreg=BGBCC_SH_REG_RQ0|(sreg&63);
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
//				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
				sreg=BGBCC_SH_REG_RQ0|(sreg&63);
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

		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;

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
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_R0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_R0)
//				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
				sreg=BGBCC_SH_REG_RQ0|(sreg&63);
//			if((sreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0)
			if((sreg&BGBCC_SH_REG_RTMASK6)==BGBCC_SH_REG_RD0)
//				sreg=BGBCC_SH_REG_RQ0|(sreg&31);
				sreg=BGBCC_SH_REG_RQ0|(sreg&63);
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

//				if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, sreg))
				if(BGBCC_CCXL_TypeQuadPointerP(ctx, tty))
				{
					if(!BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, sreg))
						{ BGBCC_DBGBREAK }

//					tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);
//					tr1=tr0+1;

					nm1=BGBCC_SH_NMID_MOVX2;
					j=BGBCC_CCXL_GetRegID(ctx, dreg);
					k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
					BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
						nm1, sreg, k);
					return(1);
				}
				else
				{
					nm1=BGBCC_SH_NMID_MOVP;
				
					j=BGBCC_CCXL_GetRegID(ctx, dreg);
					k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
					BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
						nm1, sreg, k);
					return(1);
				}
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

		if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, sreg))
			treg=BGBCC_JX2C_ScratchAllocReg(ctx, sctx, 
				BGBCC_SH_REGCLS_QGR2);
		else
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
	
	if(BGBCC_CCXL_IsRegImmP(ctx, dreg))
	{
		return(0);
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
		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;
		
		if(!(ctx->cur_func->regs[j]->regflags&BGBCC_REGFL_INITIALIZED))
			{ BGBCC_DBGBREAK }

		if(	(ctx->cur_func->regs[j]->regcls==BGBCC_SH_REGCLS_VO_REF) ||
			(ctx->cur_func->regs[j]->regcls==BGBCC_SH_REGCLS_VO_REF2))
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
		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;

		if(	(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_REF) ||
			(ctx->cur_func->args[j]->regcls==BGBCC_SH_REGCLS_VO_REF2))
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
	int tr0, tr1;
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->locals[j]->fxoffs;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ACCESSED;
		ctx->cur_func->locals[j]->regflags|=BGBCC_REGFL_ALIASPTR;

		if(	(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF) ||
			(ctx->cur_func->locals[j]->regcls==BGBCC_SH_REGCLS_VO_REF2))
				k=(ctx->cur_func->locals[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->regs[j]->fxoffs;

		ctx->cur_func->regs[j]->regflags|=BGBCC_REGFL_ACCESSED;

//		if(ctx->cur_func->regs[j]->regcls==BGBCC_SH_REGCLS_VO_REF)
//			k=(ctx->cur_func->regs[j]->fxmoffs)+(sctx->frm_offs_fix);

		i=BGBCC_JX2C_EmitLdaFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_BASEMASK;
		k=ctx->cur_func->args[j]->fxoffs;

		ctx->cur_func->args[j]->regflags|=BGBCC_REGFL_ACCESSED;

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
			if(BGBCC_JX2C_EmitRegIsExtLpReg(ctx, sctx, dreg))
			{
				tr0=BGBCC_JX2C_MapLpRegToQgr(ctx, sctx, dreg);
				tr1=tr0+1;

				BGBCC_JX2_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_GBH, tr1);

				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				if(sctx->is_rom)
					BGBCC_JX2_EmitLoadRegLabelAbs(sctx, tr0, k);
				else
					BGBCC_JX2_EmitLoadRegLabelRel24(sctx, tr0, k);

//				nm1=BGBCC_SH_NMID_MOVX2;
//				j=BGBCC_CCXL_GetRegID(ctx, sreg);
//				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
//				BGBCC_JX2_EmitStoreRegLabelVarRel24(sctx,
//					nm1, dreg, k);
				return(1);
			}
			else
			{
				j=BGBCC_CCXL_GetRegID(ctx, sreg);
				k=BGBCC_JX2C_GetGblIndexLabel(ctx, sctx, j);
				if(sctx->is_rom)
					BGBCC_JX2_EmitLoadRegLabelAbs(sctx, dreg, k);
				else
					BGBCC_JX2_EmitLoadRegLabelRel24(sctx, dreg, k);
			}
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
	ccxl_register sreg, int dstfl, int mult0)
{
	BGBCC_JX2_VarSpan *vsp, *vsp1;
	BGBCC_JX2_VarSpan2 *vspb;
	ccxl_type tty;
	int fl, regfl, mult, acnt;
	int i, j, k;

	if(!sreg.val)
		return(0);
	if((sreg.val&4095)==4095)
		return(0);

//	sctx->vsp_tcnt++;

	if(!sctx->vspan)
	{
		sctx->vspan=bgbcc_malloc(1024*sizeof(BGBCC_JX2_VarSpan *));
		sctx->vspan_chn=bgbcc_malloc(1024*sizeof(short));
		sctx->vspan_num=0;
		sctx->vspan_max=1024;
	}
	
	mult=mult0;
	for(i=0; i<sctx->vspan_num; i++)
	{
		vsp=sctx->vspan[i];
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, vsp->reg, sreg))
		{
			if(!vsp->flag)
				mult=mult0+1;
		
			if(vsp->flag&BGBCC_RSPFL_NONLOCAL)
			{
				if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg) &&
					!(ctx->cur_func->regflags&BGBCC_REGFL_NOTLEAF))
//				if(0)
				{
//					mult=mult0-1;
//					if(mult<0)
//						mult=0;
				}
				else
					if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
				{
					mult=-1;
				}else
				{
					mult=0;
				}
			}
			if(vsp->flag&BGBCC_RSPFL_NONBASIC)
				mult=0;
			if(vsp->flag&BGBCC_RSPFL_ALIASPTR)
				mult=0;
//			if(vsp->flag&BGBCC_RSPFL_GBLSTORE)
//				mult=0;
//			if(dstfl&1)
//				mult=0;
			
			if(BGBCC_CCXL_IsRegGlobalP(ctx, sreg) && (dstfl&1))
				vsp->flag|=BGBCC_RSPFL_GBLSTORE;

			if(sctx->tr_opnum < vsp->bbeg)
				vsp->bbeg=sctx->tr_opnum;
			if(sctx->tr_opnum > vsp->bend)
				vsp->bend=sctx->tr_opnum;

			if(sctx->tr_trnum < vsp->tbeg)
				vsp->tbeg=sctx->tr_trnum;
			if(sctx->tr_trnum > vsp->tend)
				vsp->tend=sctx->tr_trnum;

			acnt=vsp->cnt;

			if(mult>=0)
				vsp->cnt++;

#if 1
//			if(mult>0)
//				vsp->cnt++;

			if(mult>0)
			{
//				vsp->cnt+=mult;
//				vsp->cnt+=mult*mult;
				vsp->cnt+=8*mult*mult;
//				vsp->cnt+=4*mult*mult;
			}
#endif

#if 0
			if(mult>1)
				vsp->cnt+=vsp->cnt>>2;
			if(mult>2)
				vsp->cnt+=vsp->cnt>>3;
			if(mult>4)
				vsp->cnt+=vsp->cnt>>4;
#endif

			sctx->vsp_tcnt+=vsp->cnt-acnt;

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

	regfl=BGBCC_JX2C_GetFrameVRegFlags(ctx, sctx, sreg);

	fl=0;
	if(	!BGBCC_CCXL_IsRegTempP(ctx, sreg) &&
		!BGBCC_CCXL_IsRegArgP(ctx, sreg) &&
		!BGBCC_CCXL_IsRegLocalP(ctx, sreg))
			fl|=BGBCC_RSPFL_NONLOCAL;

	if(BGBCC_CCXL_IsRegVolatileP(ctx, sreg))
		fl|=BGBCC_RSPFL_NONLOCAL;

//	if(BGBCC_CCXL_IsRegArgP(ctx, sreg) && (ctx->cur_func->n_args>8))
//	if(regfl&BGBCC_REGFL_TEMPLOAD)
	if(BGBCC_CCXL_IsRegArgP(ctx, sreg) && (regfl&BGBCC_REGFL_TEMPLOAD))
		fl|=BGBCC_RSPFL_NONLOCAL;

	if(regfl&BGBCC_REGFL_ALIASPTR)
	{
		fl|=BGBCC_RSPFL_NONLOCAL;
		fl|=BGBCC_RSPFL_ALIASPTR;
	}

//	if(	BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
//		BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//		BGBCC_CCXL_TypeSgLongP(ctx, tty) ||
//		BGBCC_CCXL_TypeRealP(ctx, tty) ||
//		BGBCC_CCXL_TypeSgInt128P(ctx, tty))

	if(	BGBCC_CCXL_TypeValueObjectP(ctx, tty) ||
		BGBCC_CCXL_TypeArrayP(ctx, tty) ||
		BGBCC_CCXL_TypeSgInt128P(ctx, tty) ||
		BGBCC_CCXL_TypeFloat128P(ctx, tty) ||
		BGBCC_CCXL_TypeVec128P(ctx, tty) ||
		BGBCC_CCXL_TypeQuadPointerP(ctx, tty) ||
		BGBCC_CCXL_TypeQuadVariantP(ctx, tty) ||
		BGBCC_CCXL_TypeQuadVarObjP(ctx, tty) ||
		BGBCC_CCXL_TypeQuadVarRefP(ctx, tty))
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

	if(!fl)
		mult=mult0+1;

	if((mult>0) && !fl)
	{
		vsp->cnt+=8*mult*mult;
	}

	sctx->vsp_tcnt+=vsp->cnt;

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


int BGBCC_JX2C_CheckVRegLastUseInBlock(
	BGBCC_TransState *ctx,
	BGBCC_JX2_Context *sctx,
	ccxl_register reg)
{
	if(!BGBCC_JX2C_CheckVRegMoreUsesInTraceP(ctx, sctx, reg))
		return(1);
	return(0);
}
