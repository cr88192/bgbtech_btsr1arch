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

int BGBCC_JX2_EmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegStReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_EmitRemapPseudoOp(BGBCC_JX2_Context *ctx, int nmid)
{
	if(nmid==BGBCC_SH_NMID_MOVP)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_MOVUL);
		return(BGBCC_SH_NMID_MOVQ);
	}
	
	if(nmid==BGBCC_SH_NMID_LEAP)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_LEAL);
		return(BGBCC_SH_NMID_LEAQ);
	}
	
	if(nmid==BGBCC_SH_NMID_XMOVP)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_XMOVUL);
		return(BGBCC_SH_NMID_XMOVQ);
	}

	if(nmid==BGBCC_SH_NMID_CMPPEQ)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_CMPEQ);
		if(ctx->has_qmul&16)
			return(BGBCC_SH_NMID_CMPPEQ);
		return(BGBCC_SH_NMID_CMPQEQ);
	}

	if(nmid==BGBCC_SH_NMID_CMPPGT)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_CMPGT);
		if(ctx->has_qmul&16)
			return(BGBCC_SH_NMID_CMPPGT);
		return(BGBCC_SH_NMID_CMPQGT);
	}

	if(nmid==BGBCC_SH_NMID_CMPPGE)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_CMPGE);
		return(BGBCC_SH_NMID_CMPQGE);
	}

	if(nmid==BGBCC_SH_NMID_SUBP)
	{
		if(ctx->is_addr_x32)
			return(BGBCC_SH_NMID_SUBSL);
		if(ctx->has_qmul&16)
			return(BGBCC_SH_NMID_SUBP);
		return(BGBCC_SH_NMID_SUB);
	}

	return(nmid);
}

int BGBCC_JX2_TryEmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, opw3, opw4, ex, ex2, exw;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpRegStReg(ctx, nmid, rm, rn));

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }
	}

	if(	(nmid==BGBCC_SH_NMID_XMOVB)		||
		(nmid==BGBCC_SH_NMID_XMOVW)		||
		(nmid==BGBCC_SH_NMID_XMOVL)		||
		(nmid==BGBCC_SH_NMID_XMOVQ)		||
		(nmid==BGBCC_SH_NMID_XMOVUB)	||
		(nmid==BGBCC_SH_NMID_XMOVUW)	||
		(nmid==BGBCC_SH_NMID_XMOVUL)	||
		(nmid==BGBCC_SH_NMID_XMOVX2)	||
		(nmid==BGBCC_SH_NMID_XLEAB)		||
		(nmid==BGBCC_SH_NMID_XLEAW)		||
		(nmid==BGBCC_SH_NMID_XLEAL)		||
		(nmid==BGBCC_SH_NMID_XLEAQ)		)
	{
		return(BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, 0));

//		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
//		if(rn<0)
//			{ BGBCC_DBGBREAK }
	}


	if(	(nmid==BGBCC_SH_NMID_MOVC)		||
		(nmid==BGBCC_SH_NMID_MOVTW)		||
		(nmid==BGBCC_SH_NMID_MOVUTW)	||
		(nmid==BGBCC_SH_NMID_MOVHTW)	)
	{
		return(BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, 0));
	}


	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_2r++;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
//	case BGBCC_SH_NMID_FMO//VS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x9000|((rn&15)<<4)|((rm&15)<<0);			break;
//	case BGBCC_SH_NMID_FMO//VD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x9100|((rn&15)<<4)|((rm&15)<<0);			break;
#endif
	}
	
//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		opw1=-1;
	
//	opw1=-1;
	
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x0000|ex2;
				break;
			}
//			opw1=0xF000|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:	
		case BGBCC_SH_NMID_MOVUW:
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x1000|ex2;
				break;
			}
//			opw1=0xF001|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:	
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0x2000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2000|ex2;
				break;
			}
//			opw1=0xF002|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:	
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0x3000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x3000|ex2;
				break;
			}
//			opw1=0xF003|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;
//			if(rm&1)
//				{ BGBCC_DBGBREAK }
//			rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
//			if(rm<0)
//				{ BGBCC_DBGBREAK }

			if(1)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x5800|ex2;
				break;
			}

			opw1=0xF000|ex|0;
			opw2=0x4000|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_FMOVS:
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x4000|ex2;
			break;
		case BGBCC_SH_NMID_FMOVH:
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x4800|ex2;
			break;

#if 0
		case BGBCC_SH_NMID_CMOVTB:
		case BGBCC_SH_NMID_CMOVFB:
		case BGBCC_SH_NMID_CMOVUTB:
		case BGBCC_SH_NMID_CMOVUFB:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF000|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTW:
		case BGBCC_SH_NMID_CMOVFW:
		case BGBCC_SH_NMID_CMOVUTW:
		case BGBCC_SH_NMID_CMOVUFW:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF001|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTL:
		case BGBCC_SH_NMID_CMOVFL:
		case BGBCC_SH_NMID_CMOVUTL:
		case BGBCC_SH_NMID_CMOVUFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF002|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTQ:
		case BGBCC_SH_NMID_CMOVFQ:
		case BGBCC_SH_NMID_CMOVDTL:
		case BGBCC_SH_NMID_CMOVDFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF003|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		}
	}

#if 0
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		switch(nmid)
		{
//		case BGBCC_SH_NMID_FMO//VS:
//			opw1=0xF000|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
//			opw1=0xF000|ex;
//			opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x4000|ex2;			break;

		case BGBCC_SH_NMID_FMOVD:	
//			opw1=0xF001|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
//			opw1=0xF000|ex;
//			opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x5000|ex2;			break;
		}
	}
#endif

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
//	if(nmid==BGBCC_SH_NMID_MOVX2)			exw&=~0x0200;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpRegStReg(ctx, nmid, rm, rn);
	
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, opw3, opw4, ex, ex2, exw;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpLdRegReg(ctx, nmid, rm, rn));

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }
	}

	if(	(nmid==BGBCC_SH_NMID_XMOVB)		||
		(nmid==BGBCC_SH_NMID_XMOVW)		||
		(nmid==BGBCC_SH_NMID_XMOVL)		||
		(nmid==BGBCC_SH_NMID_XMOVQ)		||
		(nmid==BGBCC_SH_NMID_XMOVUB)	||
		(nmid==BGBCC_SH_NMID_XMOVUW)	||
		(nmid==BGBCC_SH_NMID_XMOVUL)	||
		(nmid==BGBCC_SH_NMID_XMOVX2)	||
		(nmid==BGBCC_SH_NMID_XLEAB)		||
		(nmid==BGBCC_SH_NMID_XLEAW)		||
		(nmid==BGBCC_SH_NMID_XLEAL)		||
		(nmid==BGBCC_SH_NMID_XLEAQ)		)
	{
//		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
//		if(rn<0)
//			{ BGBCC_DBGBREAK }

		return(BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, nmid, rm, 0, rn));
	}

	if(	(nmid==BGBCC_SH_NMID_MOVC)		||
		(nmid==BGBCC_SH_NMID_MOVTW)		||
		(nmid==BGBCC_SH_NMID_MOVUTW)	||
		(nmid==BGBCC_SH_NMID_MOVHTW)	)
	{
		return(BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, nmid, rm, 0, rn));
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_2r++;
	}

	opw1=-1; opw2=-1;
	opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
//	case BGBCC_SH_NMID_FMO..VS:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMOVD:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9500|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x8000|((rn&15)<<4)|((rm&15)<<0);			break;
	}

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		opw1=-1;

//	opw1=-1;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x8000|ex2;
				break;
			}
//			opw1=0xF008|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:	
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x9000|ex2;
				break;
			}
//			opw1=0xF009|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:	
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0xA000|ex2;
				break;
			}
//			opw1=0xF00A|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:	
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF100|ex;
//				opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0xB000|ex2;
				break;
			}
//			opw1=0xF00B|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF180|ex;
//				opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x8800|ex2;
				break;
			}
//			opw1=0xF088|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;
			opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:	
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF180|ex;
//				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x9800|ex2;
				break;
			}
//			opw1=0xF089|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;
			opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:	
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF180|ex;
//				opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0xA800|ex2;
				break;
			}
//			opw1=0xF08A|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;
			opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVDL:
//			if(ctx->is_fixed32)
			if(1)
			{
//				opw1=0xF180|ex;
//				opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
//				opw2=0xB800|ex2;
				opw2=0xA800|ex2;
				break;
			}
			opw1=0xF080|ex;
			opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;
//			if(rn&1)
//				{ BGBCC_DBGBREAK }
//			rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
//			if(rn<0)
//				{ BGBCC_DBGBREAK }

			if(1)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x7800|ex2;
				break;
			}

			opw1=0xF000|ex|0;
			opw2=0x4800|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_FMOVS:
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x6000|ex2;
			break;
		case BGBCC_SH_NMID_FMOVH:
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x6800|ex2;
			break;

#if 0
		case BGBCC_SH_NMID_CMOVTB:
		case BGBCC_SH_NMID_CMOVFB:
		case BGBCC_SH_NMID_CMOVUTB:
		case BGBCC_SH_NMID_CMOVUFB:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF008|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTW:
		case BGBCC_SH_NMID_CMOVFW:
		case BGBCC_SH_NMID_CMOVUTW:
		case BGBCC_SH_NMID_CMOVUFW:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF009|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTL:
		case BGBCC_SH_NMID_CMOVFL:
		case BGBCC_SH_NMID_CMOVUTL:
		case BGBCC_SH_NMID_CMOVUFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00A|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTQ:
		case BGBCC_SH_NMID_CMOVFQ:
		case BGBCC_SH_NMID_CMOVDTL:
		case BGBCC_SH_NMID_CMOVDFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00B|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		}
	}

#if 0
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		switch(nmid)
		{
//		case BGBCC_SH_NMID_FM..OVS:
//			opw1=0xF004|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
//			opw1=0xF000|ex;
//			opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x6000|ex2;			break;

		case BGBCC_SH_NMID_FMOVD:	
//			opw1=0xF005|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
//			opw1=0xF000|ex;
//			opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0x7000|ex2;			break;
		}
	}
#endif

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
//	if(nmid==BGBCC_SH_NMID_MOVX2)			exw&=~0x0400;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpLdRegReg(ctx, nmid, rm, rn);

		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2, 0);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegStDecReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStDecReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, opw3, opw4, ex2, exw;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(0);

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_2rpi++;
	}

	ex2=0;
	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FF|((rn&15)<<8);
		opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FE|((rn&15)<<8);
		opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FC|((rn&15)<<8);
		opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FC|((rn&15)<<8);
		opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;
	}

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		opw1=-1;

	if(opw1<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-1);
//		opw3=0xF000;
//		opw4=0x0000|((rn&15)<<4)|((rm&15)<<0);
//		opw3=0xF100;
//		opw4=0x0000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw4=0x0000|ex2;
		break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-2);
//		opw3=0xF000;
//		opw4=0x0100|((rn&15)<<4)|((rm&15)<<0);
//		opw3=0xF100;
//		opw4=0x1000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw4=0x1000|ex2;
		break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-4);
//		opw3=0xF000;
//		opw4=0x0200|((rn&15)<<4)|((rm&15)<<0);
//		opw3=0xF100;
//		opw4=0x2000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw4=0x2000|ex2;
		break;
	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-8);
//		opw3=0xF000;
//		opw4=0x0300|((rn&15)<<4)|((rm&15)<<0);
//		opw3=0xF100;
//		opw4=0x3000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw4=0x3000|ex2;
		break;
	}

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpRegStDecReg(ctx, nmid, rm, rn);
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, opw3, opw4, exw;
	int ex2;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(0);

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_2rpi++;
	}

	ex2=0;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC001|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0900|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0A00|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC004|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0B00|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC008|((rm&15)<<8);
		break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5000|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC001|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x8000|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	}

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		opw1=-1;

	if(opw1<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF100;
//		opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x8000|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)1;
		break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF100;
//		opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x9000|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)2;
		break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF100;
//		opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xA000|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)4;
		break;
	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF100;
//		opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xB000|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)8;
		break;

	case BGBCC_SH_NMID_MOVUB:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF180;
//		opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x8800|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)1;
		break;
	case BGBCC_SH_NMID_MOVUW:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF180;
//		opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0x9800|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)2;
		break;
	case BGBCC_SH_NMID_MOVUL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF180;
//		opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xA800|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)4;
		break;
	case BGBCC_SH_NMID_MOVDL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
//		opw1=0xF180;
//		opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
//		opw2=0xB800|ex2;
		opw2=0xA800|ex2;
		opw3=0xF840|(rm&31);
		opw4=(u16)4;
		break;
	}


	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpLdIncRegReg(ctx, nmid, rm, rn);
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpStatDisp(BGBCC_JX2_Context *ctx, int nmid, int disp)
{
	int disp1;

	if(ctx->is_simpass)
		return(0);
	
	ctx->stat_ldst_disptot++;

	disp1=disp;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
	case BGBCC_SH_NMID_LEAB:
		disp1=disp>>0; break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
	case BGBCC_SH_NMID_FMOVH:
	case BGBCC_SH_NMID_LEAW:
	case BGBCC_SH_NMID_MOVTW:
	case BGBCC_SH_NMID_MOVUTW:
	case BGBCC_SH_NMID_MOVHTW:
		disp1=disp>>1; break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_FMOVS:
	case BGBCC_SH_NMID_LEAL:
		disp1=disp>>2; break;
	case BGBCC_SH_NMID_MOVQ:
	case BGBCC_SH_NMID_MOVX2:
	case BGBCC_SH_NMID_LEAQ:
		disp1=disp>>3; break;
	}

	if((disp1>=0) && (disp1<32))
		ctx->stat_ldst_disp5u++;

	if((disp1>=0) && (disp1<512))
		ctx->stat_ldst_disp9u++;

	if((disp1>=0) && (disp1<64))
		ctx->stat_ldst_disp6u++;

	if((disp1>=0) && (disp1<1024))
		ctx->stat_ldst_disp10u++;

//	if((disp1>=-512) && (disp1<512))
	if((disp1>=-32) && (disp1<512))
		ctx->stat_ldst_disp10s++;

	if((disp1>=-2048) && (disp1<2048))
		ctx->stat_ldst_disp12s++;


	if((disp>=0) && (disp<32))
		ctx->stat_ldst_disp5ub++;

	if((disp>=0) && (disp<512))
		ctx->stat_ldst_disp9ub++;

	if((disp>=-512) && (disp<512))
		ctx->stat_ldst_disp10sb++;

	if((disp>=-2048) && (disp<2048))
		ctx->stat_ldst_disp12sb++;
	
	return(0);
}

int BGBCC_JX2_EmitOpStatImmed3RI(BGBCC_JX2_Context *ctx, int nmid, int disp)
{
	int disp1u, disp1n;
	int aox;

	if(ctx->is_simpass)
		return(0);
	
	disp1u=disp;
	disp1n=-disp;
	
	ctx->stat_imm3ri_immtot++;

	if((disp1u>=0) && (disp1u<32))
		ctx->stat_imm3ri_imm5u++;

	if((disp1u>=0) && (disp1u<512))
		ctx->stat_imm3ri_imm9u++;

	if((disp1n>=0) && (disp1n<32))
		ctx->stat_imm3ri_imm5n++;

	if((disp1n>=0) && (disp1n<512))
		ctx->stat_imm3ri_imm9n++;

	if((disp1u>=-512) && (disp1u<512))
		ctx->stat_imm3ri_imm9un++;


	if((disp1u>=0) && (disp1u<1024))
		ctx->stat_imm3ri_imm10u++;

	if((disp1n>=0) && (disp1n<1024))
		ctx->stat_imm3ri_imm10n++;

	if((disp1u>=-1024) && (disp1u<1024))
		ctx->stat_imm3ri_imm10un++;

	if(((disp1u>>12)>=0) && ((disp1u>>12)<512) && !(disp1u&4095))
		ctx->stat_imm3ri_imm10u_sh12++;
	if(((disp1u>>16)>=0) && ((disp1u>>16)<512) && !(disp1u&65535))
		ctx->stat_imm3ri_imm10u_sh12++;

	aox=-1;

	if(	(nmid==BGBCC_SH_NMID_ADD) ||
		(nmid==BGBCC_SH_NMID_SUB) ||
		(nmid==BGBCC_SH_NMID_ADDSL) ||
		(nmid==BGBCC_SH_NMID_SUBSL) ||
		(nmid==BGBCC_SH_NMID_ADDUL) ||
		(nmid==BGBCC_SH_NMID_SUBUL) )
			aox=0;
	
	if(nmid==BGBCC_SH_NMID_AND)
		aox=5;
	if(nmid==BGBCC_SH_NMID_OR)
		aox=6;
	if(nmid==BGBCC_SH_NMID_XOR)
		aox=7;
	
	if(aox>=0)
	{
		ctx->stat_imm3ri_immtot_aox[aox]++;

		if((disp1u>=0) && (disp1u<32))
			ctx->stat_imm3ri_imm5u_aox[aox]++;

		if((disp1n>=0) && (disp1n<32))
			ctx->stat_imm3ri_imm5n_aox[aox]++;

		if((disp1u>=-32) && (disp1u<32))
			ctx->stat_imm3ri_imm5un_aox[aox]++;

		if((disp1u>=0) && (disp1u<512))
			ctx->stat_imm3ri_imm9u_aox[aox]++;

		if((disp1n>=0) && (disp1n<512))
			ctx->stat_imm3ri_imm9n_aox[aox]++;

		if((disp1u>=-512) && (disp1u<512))
			ctx->stat_imm3ri_imm9un_aox[aox]++;

		if((disp1u>=0) && (disp1u<1024))
			ctx->stat_imm3ri_imm10u_aox[aox]++;

		if((disp1n>=0) && (disp1n<1024))
			ctx->stat_imm3ri_imm10n_aox[aox]++;

		if((disp1u>=-1024) && (disp1u<1024))
			ctx->stat_imm3ri_imm10un_aox[aox]++;
	}

	return(0);
}

int BGBCC_JX2_EmitOpStatImmed2RI(BGBCC_JX2_Context *ctx, int nmid, int disp)
{
	int disp1u, disp1n;

	if(ctx->is_simpass)
		return(0);
	
	disp1u=disp;
	disp1n=-disp;
	
	ctx->stat_imm2ri_immtot++;

	if((disp1u>=0) && (disp1u<64))
		ctx->stat_imm2ri_imm6u++;

	if((disp1u>=0) && (disp1u<1024))
		ctx->stat_imm2ri_imm10u++;

	if((disp1n>=0) && (disp1n<64))
		ctx->stat_imm2ri_imm6n++;

	if((disp1n>=0) && (disp1n<1024))
		ctx->stat_imm2ri_imm10n++;

	if((disp1u>=-1024) && (disp1u<1024))
		ctx->stat_imm2ri_imm10un++;
	
	if(((disp1u>>12)>=0) && ((disp1u>>12)<512) && !(disp1u&4095))
		ctx->stat_imm3ri_imm10u_sh12++;
	if(((disp1u>>16)>=0) && ((disp1u>>16)<512) && !(disp1u&65535))
		ctx->stat_imm3ri_imm10u_sh12++;

	return(0);
}

int BGBCC_JX2_EmitOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	int nox2reg;

	nox2reg=BGBCC_JX2_EmitCheckRegNoX2(ctx, rm);

	if((nmid==BGBCC_SH_NMID_MOVX2) && (!(ctx->has_pushx2&1) || nox2reg))
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		rm=BGBCC_JX2_TryNormalizeXRegToReg(ctx, nmid, rm);
		BGBCC_JX2_EmitOpRegStRegDisp(ctx, BGBCC_SH_NMID_MOVQ,
			rm+0, rn, disp+0);
		BGBCC_JX2_EmitOpRegStRegDisp(ctx, BGBCC_SH_NMID_MOVQ,
			rm+1, rn, disp+8);
		return(1);
	}

	if(!BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw1, opw2, opw3, opw4, odr, ex, ex2, nowxi, exw, rm3;
	int rm0, rn0;
	int i, j, k;

	rm0=rm; rn0=rn;

//	if(!disp)
	if(!disp &&
		(nmid!=BGBCC_SH_NMID_MOVX2)		&&
		(nmid!=BGBCC_SH_NMID_MOVC)		&&
		(nmid!=BGBCC_SH_NMID_XMOVB)		&&
		(nmid!=BGBCC_SH_NMID_XMOVW)		&&
		(nmid!=BGBCC_SH_NMID_XMOVL)		&&
		(nmid!=BGBCC_SH_NMID_XMOVQ)		&&
		(nmid!=BGBCC_SH_NMID_XMOVUB)	&&
		(nmid!=BGBCC_SH_NMID_XMOVUW)	&&
		(nmid!=BGBCC_SH_NMID_XMOVUL)	&&
		(nmid!=BGBCC_SH_NMID_XMOVX2)	&&
		(nmid!=BGBCC_SH_NMID_XLEAB)		&&
		(nmid!=BGBCC_SH_NMID_XLEAW)		&&
		(nmid!=BGBCC_SH_NMID_XLEAL)		&&
		(nmid!=BGBCC_SH_NMID_XLEAQ)		&&
		(nmid!=BGBCC_SH_NMID_MOVTW)		&&
		(nmid!=BGBCC_SH_NMID_MOVUTW)	&&
		(nmid!=BGBCC_SH_NMID_MOVHTW)	)
	{
		return(BGBCC_JX2_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

	if(	(nmid==BGBCC_SH_NMID_XMOVB)		||
		(nmid==BGBCC_SH_NMID_XMOVW)		||
		(nmid==BGBCC_SH_NMID_XMOVL)		||
		(nmid==BGBCC_SH_NMID_XMOVQ)		||
		(nmid==BGBCC_SH_NMID_XMOVUB)	||
		(nmid==BGBCC_SH_NMID_XMOVUW)	||
		(nmid==BGBCC_SH_NMID_XMOVUL)	||
		(nmid==BGBCC_SH_NMID_XMOVX2)	||
		(nmid==BGBCC_SH_NMID_XLEAB)		||
		(nmid==BGBCC_SH_NMID_XLEAW)		||
		(nmid==BGBCC_SH_NMID_XLEAL)		||
		(nmid==BGBCC_SH_NMID_XLEAQ)	)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }

		if(nmid==BGBCC_SH_NMID_XMOVX2)
		{
			rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
			if(rm<0)
				{ BGBCC_DBGBREAK }
		}
	}

	BGBCC_JX2_EmitOpStatDisp(ctx, nmid, disp);

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp));

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }

		if((!(ctx->has_pushx2&1) || BGBCC_JX2_EmitCheckRegNoX2(ctx, rm)) &&
			BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
			BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
		{
			rm=BGBCC_JX2_TryNormalizeXRegToReg(ctx, nmid, rm);
			j=BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, BGBCC_SH_NMID_MOVQ,
				rm+1, rn, disp+8);
			if(j<=0)
				return(j);
			k=BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, BGBCC_SH_NMID_MOVQ,
				rm+0, rn, disp+0);
			return(1);
		}
	}

	if(!(ctx->has_fmovs&32)	&& !(ctx->emit_isprobe) &&
		(	(nmid==BGBCC_SH_NMID_MOVTW) ||
			(nmid==BGBCC_SH_NMID_MOVUTW) ||
			(nmid==BGBCC_SH_NMID_MOVHTW) ))
	{
		if(disp&3)
		{
			if(nmid==BGBCC_SH_NMID_MOVHTW)
			{
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_SHLR16, rm, BGBCC_SH_REG_R0);
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_SHLR32, rm, BGBCC_SH_REG_R1);
			}else
			{
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_MOV, rm, BGBCC_SH_REG_R0);
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_SHLR16, rm, BGBCC_SH_REG_R1);
			}
			BGBCC_JX2_EmitOpRegStRegDisp(ctx,
				BGBCC_SH_NMID_MOVW, BGBCC_SH_REG_R0, rn, disp+0);
			BGBCC_JX2_EmitOpRegStRegDisp(ctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, rn, disp+4);
		}else
		{
			if(nmid==BGBCC_SH_NMID_MOVHTW)
			{
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_SHLR16, rm, BGBCC_SH_REG_R0);
				BGBCC_JX2_EmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHLD, rm, -48, BGBCC_SH_REG_R1);
			}else
			{
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_MOV, rm, BGBCC_SH_REG_R0);
				BGBCC_JX2_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_SHLR32, rm, BGBCC_SH_REG_R1);
			}
			BGBCC_JX2_EmitOpRegStRegDisp(ctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, rn, disp+0);
			BGBCC_JX2_EmitOpRegStRegDisp(ctx,
				BGBCC_SH_NMID_MOVW, BGBCC_SH_REG_R1, rn, disp+4);
		}
		return(1);
	}
	
	if(nmid==BGBCC_SH_NMID_MOVTW_V)
		nmid=BGBCC_SH_NMID_MOVTW;
	if(nmid==BGBCC_SH_NMID_MOVUTW_V)
		nmid=BGBCC_SH_NMID_MOVUTW;
	if(nmid==BGBCC_SH_NMID_MOVHTW_V)
		nmid=BGBCC_SH_NMID_MOVHTW;

	if(((rn&63)==(rn0&63)) &&
		((rn0&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_LR0))
	{
//		if((rn&31)==0)
		if((rn&63)==0)
			{ BGBCC_DBGBREAK }
	//	if((rn&31)==1)
//		if(((rn&31)==1) && (rn!=BGBCC_SH_REG_GBR))
		if(((rn&63)==1) && (rn!=BGBCC_SH_REG_GBR))
			{ BGBCC_DBGBREAK }
	}

	if(rn==BGBCC_SH_REG_PC)
		rn=BGBCC_SH_REG_R0;
	if(rn==BGBCC_SH_REG_GBR)
		rn=BGBCC_SH_REG_R1;

	if(rn==BGBCC_SH_REG_TBR)
	{
//		BGBCC_JX2_EmitLoadDrImm(ctx, disp);
		BGBCC_JX2_EmitLoadRegImm(ctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, disp);
		return(BGBCC_JX2_TryEmitOpRegStReg2(ctx, nmid,
			rm, BGBCC_SH_REG_DLR, BGBCC_SH_REG_TBR));
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_disp++;
	}

//	if((rn&0x1E)==0x00)
	if((rn&0x3E)==0x00)
	{
		/* HACK: Scale displacement for Byte-Scaled cases. */
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_FMOVH:
		case BGBCC_SH_NMID_MOVTW:
		case BGBCC_SH_NMID_MOVUTW:
		case BGBCC_SH_NMID_MOVHTW:
			disp<<=1;
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_FMOVS:
			disp<<=2;
			break;
		case BGBCC_SH_NMID_MOVQ:
		case BGBCC_SH_NMID_MOVX2:
			disp<<=3;
			break;
		}
	}

	ex=0; ex2=0;
	opw1=-1; opw2=-1; opw3=-1; opw4=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
	case BGBCC_SH_NMID_MOVUB:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((disp&0x1F)==disp)
//			{	opw1=0xF000|(disp&31);
//				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
//				break;	}

			if((disp&0x1FF)==disp)
				break;
			if(disp<0)
				break;

//			if((disp&0x1FF)==disp)
//			{	opw1=0xF100|((disp>>4)&31);
//				opw2=0x0000|((disp&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
//				break;	}
		}

#if 0
		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0xFFFF)==disp)
			if(((disp&0xFFFF)==disp) &&
				(((disp&0x0FFF)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
			{	opw1=0xFC00;
				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)disp;
				break;	}
		}
#endif

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1; opw1=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:	
	case BGBCC_SH_NMID_MOVUW:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;

//		if((rn==BGBCC_SH_REG_SP) && ((disp&0x1E)==disp) && !ctx->is_addr64)
//			{ odr=0; opw1=0x2900|((rm&15)<<4)|((disp>>1)&15); break; }

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((disp&0x3E)==disp)
//			{	opw1=0xF000|((disp>>1)&31);
//				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
//				break;	}

			if((disp&0x3FE)==disp)
				break;
			if(disp<0)
				break;

//			if((disp&0x3FE)==disp)
//			{	opw1=0xF100|((disp>>5)&31);
//				opw2=0x1000|(((disp>>1)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
//				break;	}
		}

#if 0
		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0x1FFFE)==disp)
			if(((disp&0x1FFFE)==disp) && 
				(((disp&0x1FFE)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
			{	opw1=0xFC00;
				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>1);
				break;	}
		}
#endif

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		if(disp&1)break;
		odr=2; opw1=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))
		{
//			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm))
			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rm) || !(rm&16))
				break;

//			if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
//				{ odr=0; opw1=0x3C00|((rm&15)<<4)|((disp>>2)&15); break; }
//			if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
//				{ odr=0; opw1=0x4100|((rm&15)<<4)|((disp>>2)&15); break; }

			if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
				{ odr=0; opw1=0x4200|((rm&15)<<4)|((disp>>2)&15); break; }
			break;
		}

		if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x4000|((rm&15)<<4)|((disp>>2)&15); break; }		

//		if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
//			{ odr=0; opw1=0x2800|((rm&15)<<4)|((disp>>2)&15); break; }
//		if((rn==BGBCC_SH_REG_SP) && (((disp&0x3C)|0x40)==disp))
//			{ odr=0; opw1=0x2900|((rm&15)<<4)|((disp>>2)&15); break; }

#if 0
		if((rn==BGBCC_SH_REG_SP) && ((disp&0xFFC)==disp))
		{
			odr=0;
			opw1=0xF000|(rm&31)|((disp>>5)&0x0060);
			opw2=0x2800|((disp>>2)&255);
			break;
		}
#endif

		if(disp && ((disp&0x1C)==disp))
			{ odr=0; opw1=0x8000|((rn&15)<<4)|
				((rm&15)<<0)|((disp<<6)&0x0700); break; }

#if 0
//		if(disp==4)
//			{ odr=0; opw1=0x4100|((rn&15)<<4)|((rm&15)<<0); break; }
//		if(disp==8)
//			{ odr=0; opw1=0x4200|((rn&15)<<4)|((rm&15)<<0); break; }
//		if(disp==12)
//			{ odr=0; opw1=0x4300|((rn&15)<<4)|((rm&15)<<0); break; }
//		if(disp==16)
//			{ odr=0; opw1=0x4000|((rn&15)<<4)|((rm&15)<<0); break; }
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((disp&0x7C)==disp)
//			{	opw1=0xF000|((disp>>2)&31);
//				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

			if((disp&0x7FC)==disp)
				break;

			if(disp<0)
				break;

//			if((disp&0x7FC)==disp)
//			{	opw1=0xF100|((disp>>6)&31);
//				opw2=0x2000|(((disp>>2)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
//				break;	}
		}

#if 0
		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0x3FFFC)==disp)
			if(((disp&0x3FFFC)==disp) &&
				(((disp&0x3FFC)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
			{	opw1=0xFC00;
				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>2);
				break;	}
		}
#endif

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		if(disp&3)		break;
		odr=4; opw1=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVQ:	
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))
		{
			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rm))
				break;

//			if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
//				{ odr=0; opw1=0x3D00|((rm&15)<<4)|((disp>>3)&15); break; }
			if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
				{ odr=0; opw1=0x4300|((rm&15)<<4)|((disp>>3)&15); break; }
			break;
		}

//		if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
//			{ odr=0; opw1=0x2900|((rm&15)<<4)|((disp>>3)&15); break; }
//		if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
//			{ odr=0; opw1=0x4200|((rm&15)<<4)|((disp>>3)&15); break; }
		if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
			{ odr=0; opw1=0x4100|((rm&15)<<4)|((disp>>3)&15); break; }

#if 0
		if((rn==BGBCC_SH_REG_SP) && ((disp&0x1FF8)==disp))
		{
			odr=0;
			opw1=0xF000|(rm&31)|((disp>>6)&0x0060);
			opw2=0x2900|((disp>>3)&255);
			break;
		}
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			if((disp&0xF8)==disp)
//			{	opw1=0xF000|((disp>>3)&31);
//				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

			if((disp&0xFF8)==disp)
				break;

			if(disp<0)
				break;

//			if((disp&0xFF8)==disp)
//			{	opw1=0xF100|((disp>>7)&31);
//				opw2=0x3000|(((disp>>3)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
//				break;	}
		}

#if 0
		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0x7FFF8)==disp)
			if(((disp&0x7FFF8)==disp) &&
				(((disp&0x7FF8)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
			{	opw1=0xFC00;
				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>3);
				break;	}
		}
#endif

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

//		if(disp&7)		break;
		odr=8; opw1=0x0700|((rn&15)<<4)|((rm&15)<<0);	break;

#if 1
	case BGBCC_SH_NMID_MOVX2:
		if(!(ctx->has_pushx2&1))
			break;

		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rm))	break;

		if(rm&1)
			break;
//		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
//		if(rm<0)
//			break;

		if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
		{
			odr=0;
			opw1=0x2900|((rm&15)<<4)|((disp>>3)&15);
			if(rm&16)opw1|=0x0010;
			break;
		}
		break;
#endif

#if 0
//	case BGBCC_SH_NMID_FMO..VS:	
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x7FC)==disp)
				break;

//			if((disp&0x7C)==disp)
//			{	opw1=0xF000|((disp>>2)&31);
//				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
		}
//		if(disp&3)		break;
		odr=4; opw1=0x9200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMOVD:	
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0xFF8)==disp)
				break;
//			if((disp&0xF8)==disp)
//			{	opw1=0xF000|((disp>>3)&31);
//				opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
		}
//		if(disp&7)		break;
		odr=8; opw1=0x9300|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

	}

	if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{ opw1=-1; odr=0; }

	if((opw1<0) && (nmid==BGBCC_SH_NMID_MOVC))
	{		
		if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
			BGBCC_JX2_EmitCheckRegBsrCn(ctx, rm))
		{
			rm3=BGBCC_JX2_TryMapRegCn(ctx, rm);

			ex=0;
			if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn )) ex|=0x0040;
			if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm3)) ex|=0x0020;
			ex2=ex<<4;

			if((disp&0xFF8)==disp)
			{
				opw1=0xF100|((rn&15)<<4)|((rm3&15)<<0);
				opw2=0x5000|ex2|((disp>>3)&511);
			}
		}
	}

//	opw1=-1; odr=0;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
		
		nowxi=0;
//		if(ctx->op_is_wex2&4)
//			nowxi=disp>>8;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
		case BGBCC_SH_NMID_MOVB:
//			if(!ctx->is_fixed32)
//				if((disp&0x1F)==disp)
//			{	opw1=0xF000|ex|((disp>>0)&31);
//				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

			if(((disp&0x1FF)==disp) && !nowxi)
			{
//				opw1=0xF100|ex|((disp>>4)&31);
//				opw2=0x0000|(((disp>>0)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x0000|ex2|((disp>>0)&511);
				break;
			}

#if 1
			if(((disp|(~0x1FF))==disp) && !nowxi && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x0000|ex2|((disp>>0)&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0xFFFF)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>0);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~31))==disp))
			{
				opw1=0xF000|ex|(disp&31);
				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x0000,
					rm, disp, rn);
				if(i>0)break;
			}

//			opw1=0xF004|ex; odr=1;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=1;
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_MOVW:
//			if(!ctx->is_fixed32)
//				if((disp&0x3E)==disp)
//			{	opw1=0xF000|ex|((disp>>1)&31);
//				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			if(disp&1)		break;
//			opw1=0xF005|ex; odr=2;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if(((disp&0x3FE)==disp) && !(nowxi>>1))
			{
//				opw1=0xF100|ex|((disp>>5)&31);
//				opw2=0x1000|(((disp>>1)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x1000|ex2|((disp>>1)&511);
				break;	}

#if 1
			if((((disp&(~1))|(~0x3FF))==disp) &&
				!(nowxi>>1) && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x1000|ex2|((disp>>1)&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0x1FFFE)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>1);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~63))==disp) && !(disp&1))
			{
				opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&1))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x1000,
					rm, disp>>1, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex;	odr=2;
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_MOVL:
//			if(!ctx->is_fixed32)
//				if((disp&0x7C)==disp)
//			{	opw1=0xF000|ex|((disp>>2)&31);
//				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			if(disp&3)		break;
//			opw1=0xF006|ex; odr=4;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if(((disp&0x7FC)==disp) && !(nowxi>>2))
			{
//				opw1=0xF100|ex|((disp>>6)&31);
//				opw2=0x2000|(((disp>>2)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2000|ex2|((disp>>2)&511);
				break;	}


#if 1
			if((((disp&(~3))|(~0x7FF))==disp) &&
				!(nowxi>>2) && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2000|ex2|((disp>>2)&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0x3FFFC)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>2);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~127))==disp) && !(disp&3))
			{
				opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&3))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x2000,
					rm, disp>>2, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex;	odr=4;
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!ctx->is_fixed32)
//				if((disp&0xF8)==disp)
//			{	opw1=0xF000|ex|((disp>>3)&31);
//				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			if(disp&7)		break;
//			opw1=0xF007|ex; odr=8;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if(((disp&0xFF8)==disp) && !(nowxi>>3))
			{
//				opw1=0xF100|ex|((disp>>7)&31);
//				opw2=0x3000|(((disp>>3)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x3000|ex2|((disp>>3)&511);
				break;	}


#if 1
			if((((disp&(~7))|(~0xFFF))==disp) &&
				!(nowxi>>3) && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x3000|ex2|((disp>>3)&511);
				break;
			}
#endif

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0x7FFF8)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>3);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~255))==disp) && !(disp&7))
			{
				opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&7))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x3000,
					rm, disp>>3, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex;	odr=8;
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;

#if 1
		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;

//			if(rm&1)
//				{ BGBCC_DBGBREAK }

//			rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
//			if(rm<0)
//				{ BGBCC_DBGBREAK }

#if 0
			if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x4000|((rn&15)<<4)|((rm&15)<<0);
				break;		}
#endif

#if 1
			if(((disp&0xFF8)==disp) && !(nowxi>>3))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x5800|ex2|((disp>>3)&511);
				break;
			}
#endif


#if 1
			if((((disp&(~7))|(~0xFFF))==disp) &&
				!(nowxi>>3) && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x5800|ex2|((disp>>3)&511);
				break;
			}
#endif

			if(ctx->has_xgpr && ((disp|(~255))==disp) && !(disp&7))
			{
				opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x4000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&7))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x5800,
					rm, disp>>3, rn);
				if(i>0)break;
			}
#endif

			opw1=0xF000|ex;	odr=8;
			opw2=0x4400|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		case BGBCC_SH_NMID_FMOVS:
			if(((disp&0x7FC)==disp) && !(nowxi>>2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x4000|ex2|((disp>>2)&511);
				break;
			}

#if 1
			if((((disp&(~3))|(~0x7FF))==disp) &&
				!(nowxi>>2) && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x4000|ex2|((disp>>2)&511);
				break;
			}
#endif

			opw1=0xF000|ex;	odr=4;
			opw2=0x4600|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_FMOVH:
			if(((disp&0x3FE)==disp) && !(nowxi>>1))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x4800|ex2|((disp>>1)&511);
				break;
			}

#if 1
			if((((disp&(~1))|(~0x3FF))==disp) &&
				!(nowxi>>1) && (ctx->is_fixed32&2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x4800|ex2|((disp>>1)&511);
				break;
			}
#endif

			opw1=0xF000|ex;	odr=2;
			opw2=0x4700|((rn&15)<<4)|((rm&15)<<0);
			break;


		case BGBCC_SH_NMID_MOVTW:
		case BGBCC_SH_NMID_MOVUTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			if(((disp&0x3E)==disp) && !(nowxi>>1))
			{
				opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF000|ex;	odr=2;
			opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_MOVHTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			if(((disp&0x3E)==disp) && !(nowxi>>1))
			{
				opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF000|ex;	odr=2;
			opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);
			break;

#if 0
		case BGBCC_SH_NMID_CMOVTB:
		case BGBCC_SH_NMID_CMOVFB:
		case BGBCC_SH_NMID_CMOVUTB:
		case BGBCC_SH_NMID_CMOVUFB:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF004|ex;	odr=1;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTW:
		case BGBCC_SH_NMID_CMOVFW:
		case BGBCC_SH_NMID_CMOVUTW:
		case BGBCC_SH_NMID_CMOVUFW:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF005|ex;	odr=2;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTL:
		case BGBCC_SH_NMID_CMOVFL:
		case BGBCC_SH_NMID_CMOVUTL:
		case BGBCC_SH_NMID_CMOVUFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF006|ex;	odr=4;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTQ:
		case BGBCC_SH_NMID_CMOVFQ:
		case BGBCC_SH_NMID_CMOVDTL:
		case BGBCC_SH_NMID_CMOVDFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF007|ex;	odr=(nmid&8)?4:8;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_XMOVB:
		case BGBCC_SH_NMID_XMOVUB:
			if((disp&0x1F)==disp)
			{	opw1=0xF000|ex|((disp>>0)&31);
				opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=1;
			opw2=0x8400|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVW:
		case BGBCC_SH_NMID_XMOVUW:
			if((disp&0x3E)==disp)
			{	opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x8100|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=2;
			opw2=0x8500|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVL:
		case BGBCC_SH_NMID_XMOVUL:
			if((disp&0x7C)==disp)
			{	opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x8200|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=4;
			opw2=0x8600|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVQ:
			if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x8300|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=8;
			opw2=0x8700|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVX2:
			if(!(ctx->has_pushx2&1))
				break;

			if((disp&0xF8)==disp)
			{	opw1=0xF080|ex|((disp>>3)&31);
				opw2=0x8300|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF080|ex;	odr=8;
			opw2=0x8700|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		case BGBCC_SH_NMID_ADDSL:
		case BGBCC_SH_NMID_ADDUL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_SUBSL:
		case BGBCC_SH_NMID_SUBUL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_RSUBSL:
		case BGBCC_SH_NMID_RSUBUL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_ANDL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_ORL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x6000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_XORL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x7000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;

		case BGBCC_SH_NMID_ADD:
		case BGBCC_SH_NMID_ADDQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_SUB:
		case BGBCC_SH_NMID_SUBQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_RSUB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_AND:
		case BGBCC_SH_NMID_ANDQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_OR:
		case BGBCC_SH_NMID_ORQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x6000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_XOR:
		case BGBCC_SH_NMID_XORQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x7000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;

#if 0
		case BGBCC_SH_NMID_XCHGL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_XCHGQ:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_XCHGVL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x9000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_XCHGVQ:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x9000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;
#endif

		case BGBCC_SH_NMID_MOVVL:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x8000, 0xF000, 0x0002,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_MOVVQ:
			if(!ctx->has_ldop)
				break;
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x8000, 0xF000, 0x0003,
				rm, disp>>3, rn);
			break;

		}
	}

#if 0
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		nowxi=0;
//		if(ctx->op_is_wex2&4)
//			nowxi=disp>>8;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			if(((disp&0x7FC)==disp) && !(nowxi>>2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x4000|ex2|((disp>>2)&511);
				break;
			}

//			if((disp&0x7C)==disp)
//			{	opw1=0xF000|ex|((disp>>2)&31);
//				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			if(disp&3)		break;
//			opw1=0xF002|ex; odr=4;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=4;
			opw2=0x9200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			if(((disp&0xFF8)==disp) && !(nowxi>>3))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x5000|ex2|((disp>>3)&511);
				break;
			}
//			if((disp&0xF8)==disp)
//			{	opw1=0xF000|ex|((disp>>3)&31);
//				opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			if(disp&7)		break;
//			opw1=0xF003|ex; odr=8;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=8;
			opw2=0x9300|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}
#endif

	if((ctx->op_is_wex2&2) && (odr>0))
		{ opw1=-1; odr=0; }

	exw=0;
	if((disp<0) && !odr && ((opw1&0xFF00)==0xF000))
		{ exw|=0x0100; }
	if((disp<0) && !odr && ((opw1&0xFF00)==0xF100) &&
			((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22)))
		{ exw|=0x0100; }
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
//	if(nmid==BGBCC_SH_NMID_MOVX2)			exw&=~0x0200;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	
		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegNoX2(BGBCC_JX2_Context *ctx, int rn)
{
	int nox2reg;

//	return(0);

	nox2reg=0;
	if(
		(rn==BGBCC_SH_REG_R0) ||
		(rn==BGBCC_SH_REG_RD0) ||
		(rn==BGBCC_SH_REG_RQ0) ||
		(rn==BGBCC_SH_REG_R14) ||
		(rn==BGBCC_SH_REG_RD14) ||
		(rn==BGBCC_SH_REG_RQ14) ||
		(rn==BGBCC_SH_REG_R32) ||
		(rn==BGBCC_SH_REG_RD32) ||
		(rn==BGBCC_SH_REG_RQ32) ||
		(rn==BGBCC_SH_REG_R46) ||
		(rn==BGBCC_SH_REG_RD46) ||
		(rn==BGBCC_SH_REG_RQ46))
	{
		nox2reg=1;
	}

	if(
		(rn==BGBCC_SH_REG_R1) ||
		(rn==BGBCC_SH_REG_R15) ||
		(rn==BGBCC_SH_REG_LR14) ||
		(rn==BGBCC_SH_REG_LR32) ||
		(rn==BGBCC_SH_REG_LR46) )
	{
		nox2reg=1;
	}

	return(nox2reg);
}

int BGBCC_JX2_EmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int nox2reg;

	nox2reg=BGBCC_JX2_EmitCheckRegNoX2(ctx, rn);

	if((nmid==BGBCC_SH_NMID_MOVX2) && (!(ctx->has_pushx2&1) || nox2reg))
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		rn=BGBCC_JX2_TryNormalizeXRegToReg(ctx, nmid, rn);
		BGBCC_JX2_EmitOpLdRegDispReg(ctx, BGBCC_SH_NMID_MOVQ,
			rm, disp+0, rn+0);
		BGBCC_JX2_EmitOpLdRegDispReg(ctx, BGBCC_SH_NMID_MOVQ,
			rm, disp+8, rn+1);
		return(1);
	}

	if(!BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw1, opw2, opw3, opw4, odr, ex, ex2, nowxi, exw, rn3;
	int chkp_disp9b, chkp_disp9w, chkp_disp9l, chkp_disp9q;
	int rm0, rn0;
	int i, j, k;

//	if(!disp)
//	if(!disp && (nmid!=BGBCC_SH_NMID_MOVX2))
	if(!disp &&
		(nmid!=BGBCC_SH_NMID_MOVX2) &&
		(nmid!=BGBCC_SH_NMID_MOVQ_DISP) &&
		(nmid!=BGBCC_SH_NMID_MOVQ_DISP24) &&
		(nmid!=BGBCC_SH_NMID_MOVC_DISP24) &&
		(nmid!=BGBCC_SH_NMID_MOVC) 		&&
		(nmid!=BGBCC_SH_NMID_XMOVB)		&&
		(nmid!=BGBCC_SH_NMID_XMOVW)		&&
		(nmid!=BGBCC_SH_NMID_XMOVL)		&&
		(nmid!=BGBCC_SH_NMID_XMOVQ)		&&
		(nmid!=BGBCC_SH_NMID_XMOVUB)	&&
		(nmid!=BGBCC_SH_NMID_XMOVUW)	&&
		(nmid!=BGBCC_SH_NMID_XMOVUL)	&&
		(nmid!=BGBCC_SH_NMID_XMOVX2)	&&
		(nmid!=BGBCC_SH_NMID_XLEAB)		&&
		(nmid!=BGBCC_SH_NMID_XLEAW)		&&
		(nmid!=BGBCC_SH_NMID_XLEAL)		&&
		(nmid!=BGBCC_SH_NMID_XLEAQ)		&&
		(nmid!=BGBCC_SH_NMID_MOVTW)		&&
		(nmid!=BGBCC_SH_NMID_MOVUTW)	&&
		(nmid!=BGBCC_SH_NMID_MOVHTW)	&&
		(rm!=BGBCC_SH_REG_PC) &&
		(rm!=BGBCC_SH_REG_GBR) &&
		(rm!=BGBCC_SH_REG_TBR))
	{
		return(BGBCC_JX2_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

	BGBCC_JX2_EmitOpStatDisp(ctx, nmid, disp);

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn));

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);

	rm0=rm;
	rn0=rn;

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }


		if((!(ctx->has_pushx2&1) || BGBCC_JX2_EmitCheckRegNoX2(ctx, rn)) &&
			BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
			BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
		{
			rn=BGBCC_JX2_TryNormalizeXRegToReg(ctx, nmid, rn);
			j=BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, BGBCC_SH_NMID_MOVQ,
				rm, disp+8, rn+1);
			k=BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, BGBCC_SH_NMID_MOVQ,
				rm, disp+0, rn+0);
			return(1);
		}
	}

	if(!(ctx->has_fmovs&32)	&& !(ctx->emit_isprobe) &&
		(	(nmid==BGBCC_SH_NMID_MOVTW) ||
			(nmid==BGBCC_SH_NMID_MOVUTW) ||
			(nmid==BGBCC_SH_NMID_MOVHTW) ))
	{
		/* We don't have these ops, fake them. */
		if(!(disp&7))
		{
			BGBCC_JX2_EmitOpLdRegDispReg(ctx,
				BGBCC_SH_NMID_MOVQ, rm, disp, rn);
			BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHLL16, rn);
			if(nmid==BGBCC_SH_NMID_MOVTW)
				BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHAR16, rn);
			if(nmid==BGBCC_SH_NMID_MOVUTW)
				BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHLR16, rn);
			return(1);
		}
		
		if(disp&3)
		{
			opw4=BGBCC_SH_NMID_MOVUL;
			if(nmid==BGBCC_SH_NMID_MOVTW)
				opw4=BGBCC_SH_NMID_MOVL;
	
			BGBCC_JX2_EmitOpLdRegDispReg(ctx,
				opw4, rm, disp+4, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpLdRegDispReg(ctx,
				BGBCC_SH_NMID_MOVUW, rm, disp+0, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHLL16, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegRegReg(ctx, BGBCC_SH_NMID_OR,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R1, rn);
			if(nmid==BGBCC_SH_NMID_MOVHTW)
				BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHLL16, rn);
		}else
		{
			opw4=BGBCC_SH_NMID_MOVUW;
			if(nmid==BGBCC_SH_NMID_MOVTW)
				opw4=BGBCC_SH_NMID_MOVW;
	
			BGBCC_JX2_EmitOpLdRegDispReg(ctx,
				opw4, rm, disp+4, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpLdRegDispReg(ctx,
				BGBCC_SH_NMID_MOVUL, rm, disp+0, BGBCC_SH_REG_R0);
			BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHLL32, BGBCC_SH_REG_R1);
			BGBCC_JX2_EmitOpRegRegReg(ctx, BGBCC_SH_NMID_OR,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R1, rn);
			if(nmid==BGBCC_SH_NMID_MOVHTW)
				BGBCC_JX2_EmitOpReg(ctx, BGBCC_SH_NMID_SHLL16, rn);
		}
		return(1);
	}
	
	if(nmid==BGBCC_SH_NMID_MOVTW_V)
		nmid=BGBCC_SH_NMID_MOVTW;
	if(nmid==BGBCC_SH_NMID_MOVUTW_V)
		nmid=BGBCC_SH_NMID_MOVUTW;
	if(nmid==BGBCC_SH_NMID_MOVHTW_V)
		nmid=BGBCC_SH_NMID_MOVHTW;

#if 0
	if(
		(nmid==BGBCC_SH_NMID_LEATB)		||
		(nmid==BGBCC_SH_NMID_LEATW)		||
		(nmid==BGBCC_SH_NMID_LEATL)		||
		(nmid==BGBCC_SH_NMID_LEATQ)		)
	{
		if((rm==rn) && (disp>=(-1023)) && (disp<=( 1023)))
		{
			i=BGBCC_JX2_TryEmitOpImmReg(ctx, nmid, disp, rn);
			return(i);
		}
	}
#endif

	if(disp<0)
	{
		k=1;
	}

	if(	(nmid==BGBCC_SH_NMID_XMOVB)		||
		(nmid==BGBCC_SH_NMID_XMOVW)		||
		(nmid==BGBCC_SH_NMID_XMOVL)		||
		(nmid==BGBCC_SH_NMID_XMOVQ)		||
		(nmid==BGBCC_SH_NMID_XMOVUB)	||
		(nmid==BGBCC_SH_NMID_XMOVUW)	||
		(nmid==BGBCC_SH_NMID_XMOVUL)	||
		(nmid==BGBCC_SH_NMID_XMOVX2)	||
		(nmid==BGBCC_SH_NMID_XLEAB)		||
		(nmid==BGBCC_SH_NMID_XLEAW)		||
		(nmid==BGBCC_SH_NMID_XLEAL)		||
		(nmid==BGBCC_SH_NMID_XLEAQ)		)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }

		if(	(nmid==BGBCC_SH_NMID_XMOVX2) ||
			(nmid==BGBCC_SH_NMID_XLEAB))
		{
			rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
			if(rn<0)
				{ BGBCC_DBGBREAK }
		}
	}

	if(((rm&63)==(rm0&63)) &&
		((rm0&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_LR0))
	{
//		if((rm&31)==0)
		if((rm&63)==0)
			{ BGBCC_DBGBREAK }
//		if((rm&31)==1)
//		if(((rm&31)==1) && (rm!=BGBCC_SH_REG_GBR))
		if(((rm&63)==1) && (rm!=BGBCC_SH_REG_GBR))
			{ BGBCC_DBGBREAK }
	}

	if(rm==BGBCC_SH_REG_PC)
		rm=BGBCC_SH_REG_R0;
	if(rm==BGBCC_SH_REG_GBR)
		rm=BGBCC_SH_REG_R1;

	if(rm==BGBCC_SH_REG_TBR)
	{
		BGBCC_JX2_EmitLoadDrImm(ctx, disp);
		return(BGBCC_JX2_TryEmitOpLdReg2Reg(ctx, nmid,
			BGBCC_SH_REG_TBR, BGBCC_SH_REG_DLR, rn));
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_disp++;
	}

//	if((rm&0x1E)==0x00)
	if((rm&0x3E)==0x00)
	{
		/* HACK: Scale displacement for Byte-Scaled cases. */
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_FMOVH:
		case BGBCC_SH_NMID_MOVTW:
		case BGBCC_SH_NMID_MOVUTW:
		case BGBCC_SH_NMID_MOVHTW:
			disp<<=1;
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_FMOVS:
			disp<<=2;
			break;
		case BGBCC_SH_NMID_MOVQ:
		case BGBCC_SH_NMID_MOVC:
		case BGBCC_SH_NMID_MOVX2:
		case BGBCC_SH_NMID_MOVQ_DISP:
		case BGBCC_SH_NMID_MOVQ_DISP24:
		case BGBCC_SH_NMID_MOVC_DISP24:
			disp<<=3;
			break;
		}
	}else
	{
		if((nmid==BGBCC_SH_NMID_LEAB) && !(disp&1))
			nmid=BGBCC_SH_NMID_LEAW;
		if((nmid==BGBCC_SH_NMID_LEAW) && !(disp&3))
			nmid=BGBCC_SH_NMID_LEAL;
		if((nmid==BGBCC_SH_NMID_LEAL) && !(disp&7))
			nmid=BGBCC_SH_NMID_LEAQ;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; odr=0; ex=0; ex2=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0x1F)==disp)
			{	opw1=0xF000|(disp&31);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x1FF)==disp)
			{	opw1=0xF100|ex|((disp>>4)&31);
				opw2=0x8000|(((disp>>0)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif
			if((disp&0x1F)==disp)
				break;
			if((disp&0x1FF)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0xFFFF)==disp)
			if(((disp&0xFFFF)==disp) && !((disp&0x0FFF)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>0);
				break;	}
#endif
//			if(((disp&0xFFFF)==disp) && !((disp&0x0FFF)==disp))
			if(((disp&0xFFFF)==disp) &&
				(((disp&0x0FFF)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1; opw1=0x0C00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;

//		if((rm==BGBCC_SH_REG_SP) && ((disp&0x1E)==disp) && !ctx->is_addr64)
//			{ odr=0; opw1=0x2B00|((rn&15)<<4)|((disp>>1)&15); break; }
		
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0x3E)==disp)
			{	opw1=0xF000|((disp>>1)&31);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x3FE)==disp)
			{	opw1=0xF100|ex|((disp>>5)&31);
				opw2=0x9000|(((disp>>1)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif

			if((disp&0x3E)==disp)
				break;
			if((disp&0x3FE)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0x1FFFE)==disp)
			if(((disp&0x1FFFE)==disp) && !((disp&0x1FFE)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>1);
				break;	}
#endif
//			if(((disp&0x1FFFE)==disp) && !((disp&0x1FFE)==disp))
			if(((disp&0x1FFFE)==disp) &&
				(((disp&0x1FFE)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=2; opw1=0x0D00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVDL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))
		{
			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rn))
				break;

//			if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
//				{ odr=0; opw1=0x3E00|((rn&15)<<4)|((disp>>2)&15); break; }
//			if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
//				{ odr=0; opw1=0x4500|((rn&15)<<4)|((disp>>2)&15); break; }
			if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
				{ odr=0; opw1=0x4600|((rn&15)<<4)|((disp>>2)&15); break; }
			break;
		}

		if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x4400|((rn&15)<<4)|((disp>>2)&15); break; }
//		if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
//			{ odr=0; opw1=0x2A00|((rn&15)<<4)|((disp>>2)&15); break; }
//		if((rm==BGBCC_SH_REG_SP) && (((disp&0x3C)|0x40)==disp))
//			{ odr=0; opw1=0x2B00|((rn&15)<<4)|((disp>>2)&15); break; }

#if 0
		if((rm==BGBCC_SH_REG_SP) && ((disp&0xFFC)==disp))
		{
			odr=0;
			opw1=0xF000|(rn&31)|((disp>>5)&0x0060);
			opw2=0x2A00|((disp>>2)&255);
			break;
		}
#endif

		if(disp && ((disp&0x1C)==disp))
			{ odr=0; opw1=0x8800|((rn&15)<<4)|
				((rm&15)<<0)|((disp<<6)&0x0700); break; }

#if 0
		if(disp==16)
			{ odr=0; opw1=0x4400|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==4)
			{ odr=0; opw1=0x4500|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==8)
			{ odr=0; opw1=0x4600|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==12)
			{ odr=0; opw1=0x4700|((rn&15)<<4)|((rm&15)<<0); break; }
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0x7C)==disp)
			{	opw1=0xF000|((disp>>2)&31);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x7FC)==disp)
			{	opw1=0xF100|ex|((disp>>6)&31);
				opw2=0xA000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif
			if((disp&0x7C)==disp)
				break;
			if((disp&0x7FC)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0x3FFFC)==disp)
			if(((disp&0x3FFFC)==disp) && !((disp&0x3FFC)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>2);
				break;	}
#endif

//			if(((disp&0x3FFFC)==disp) && !((disp&0x3FFC)==disp))
			if(((disp&0x3FFFC)==disp) &&
				(((disp&0x3FFC)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=4; opw1=0x0E00|((rn&15)<<4)|((rm&15)<<0);	break;

	case BGBCC_SH_NMID_MOVQ:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))
		{
			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rn))
				break;

//			if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
//				{ odr=0; opw1=0x3F00|((rn&15)<<4)|((disp>>3)&15); break; }
			if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
				{ odr=0; opw1=0x4700|((rn&15)<<4)|((disp>>3)&15); break; }
			break;
		}

//		if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
//			{ odr=0; opw1=0x2B00|((rn&15)<<4)|((disp>>3)&15); break; }
//		if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
//			{ odr=0; opw1=0x4600|((rn&15)<<4)|((disp>>3)&15); break; }
		if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
			{ odr=0; opw1=0x4500|((rn&15)<<4)|((disp>>3)&15); break; }

#if 0
		if((rm==BGBCC_SH_REG_SP) && ((disp&0x1FF8)==disp))
		{
			odr=0;
			opw1=0xF000|(rn&31)|((disp>>6)&0x0060);
			opw2=0x2B00|((disp>>3)&255);
			break;
		}
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0xF8)==disp)
			{	opw1=0xF000|((disp>>3)&31);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0xFF8)==disp)
			{	opw1=0xF100|ex|((disp>>7)&31);
				opw2=0xB000|(((disp>>3)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif
			if((disp&0xF8)==disp)
				break;
			if((disp&0xFF8)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0x7FFF8)==disp)
			if(((disp&0x7FFF8)==disp) && !((disp&0x7FF8)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>3);
				break;	}
#endif
//			if(((disp&0x7FFF8)==disp) && !((disp&0x7FF8)==disp))
			if(((disp&0x7FFF8)==disp) &&
				(((disp&0x7FF8)!=disp) ||
					(ctx->optmode!=BGBCC_OPT_SIZE)))
				break;
		}

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=8; opw1=0x0F00|((rn&15)<<4)|((rm&15)<<0);	break;

#if 1
	case BGBCC_SH_NMID_MOVX2:
		if(!(ctx->has_pushx2&1))
			break;

		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rn))	break;
		
		if(rn&1)
			break;
//		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
//		if(rn<0)
//			{ BGBCC_DBGBREAK }

		if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
		{
			odr=0;
			opw1=0x2B00|((rn&15)<<4)|((disp>>3)&15);
			if(rn&16)opw1|=0x0010;
			break;
		}
		break;
#endif

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
//		if((disp&0x1F)==disp)		break;
		if((disp&0x1FF)==disp)		break;

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=1; opw1=0x5200|((rn&15)<<4)|((rm&15)<<0);		break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
//		if((disp&0x3E)==disp)		break;
		if((disp&0x3FE)==disp)		break;

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=2; opw1=0x5300|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVUL:
//	case BGBCC_SH_NMID_MOVDL:
//		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;

#if 1
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))
		{
			if(!BGBCC_JX2_EmitCheckRegExt32GPR(ctx, rn))
				break;
			if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
				{ odr=0; opw1=0x2A00|((rn&15)<<4)|((disp>>2)&15); break; }
			break;
		}

		if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x2800|((rn&15)<<4)|((disp>>2)&15); break; }
#endif

//		if((disp&0x7C)==disp)		break;
		if((disp&0x7FC)==disp)		break;

		if(ctx->has_jumbo && !ctx->op_is_wex2)
			break;

		odr=4; opw1=0x8800|((rn&15)<<4)|((rm&15)<<0);	break;

#if 0
//	case BGBCC_SH_NMID_FMO..VS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x7FC)==disp)
				break;
//			if((disp&0x7C)==disp)
//			{	opw1=0xF000|((disp>>2)&31);
//				opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
		}
		odr=4; opw1=0x9600|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_FMOVD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0xFF8)==disp)
				break;
//			if((disp&0xF8)==disp)
//			{	opw1=0xF000|((disp>>3)&31);
//				opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
		}
		odr=8; opw1=0x9700|((rn&15)<<4)|((rm&15)<<0);	break;
#endif
	}

	if(ctx->is_fixed32 || ctx->op_is_wex2 || (ctx->emit_riscv&0x22))
		{ opw1=-1; odr=0; }

	if((opw1<0) &&
		((nmid==BGBCC_SH_NMID_MOVC) ||
		 (nmid==BGBCC_SH_NMID_MOVC_DISP24)))
	{	
		if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
			BGBCC_JX2_EmitCheckRegBsrCn(ctx, rn))
		{
			rn3=BGBCC_JX2_TryMapRegCn(ctx, rn);

			ex=0;
			if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn3)) ex|=0x0040;
			if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm )) ex|=0x0020;
			ex2=ex<<4;

			if(nmid==BGBCC_SH_NMID_MOVC_DISP24)
			{
				if(ctx->has_jumbo && !(ctx->op_is_wex2&12))
				{
					opw1=0xFE00|((disp>>27)&0x00FF);
					opw2=0x0000|((disp>>11)&0xFFFF);
					opw3=0xF100|((rn3&15)<<4)|((rm&15)<<0);
					opw4=0x7000|ex2|
						((disp>>3)&0x00FF)|
						((disp>>23)&0x0100);
				}else
				{
					opw1=0xFA00|((disp>>19)&0x01FF);
					opw2=0x0000|((disp>> 3)&0xFFFF);
	//				opw3=0xF080|ex;
	//				opw4=0x4C00|((rn3&15)<<4)|((rm&15)<<0);
					opw3=0xF000|ex;
					opw4=0x4D00|((rn3&15)<<4)|((rm&15)<<0);
				}
			}else
				if((disp&0xFF8)==disp)
			{
				opw1=0xF100|((rn3&15)<<4)|((rm&15)<<0);
				opw2=0x7000|ex2|((disp>>3)&511);
			}
		}
	}

//	opw1=-1; odr=0;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		nowxi=0;
//		if(ctx->op_is_wex2&4)
//			nowxi=disp>>8;

		chkp_disp9b=0;
		chkp_disp9w=0;
		chkp_disp9l=0;
		chkp_disp9q=0;
		
		if(((disp&0x1FF)==disp) && !(nowxi>>0))
			chkp_disp9b=1;
		if(((disp&0x3FE)==disp) && !(nowxi>>1))
			chkp_disp9w=1;
		if(((disp&0x7FC)==disp) && !(nowxi>>2))
			chkp_disp9l=1;
		if(((disp&0xFF8)==disp) && !(nowxi>>3))
			chkp_disp9q=1;

#if 1
		if(ctx->is_fixed32&2)
		{
			if(((disp|(~0x1FF))==disp) && !(nowxi>>0))
				chkp_disp9b=1;
			if(((disp|(~0x3FF))==disp) && !(disp&1) && !(nowxi>>1))
				chkp_disp9w=1;
			if(((disp|(~0x7FF))==disp) && !(disp&3) && !(nowxi>>2))
				chkp_disp9l=1;
			if(((disp|(~0xFFF))==disp) && !(disp&7) && !(nowxi>>3))
				chkp_disp9q=1;
		}
#endif

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
//			if(!ctx->is_fixed32)
//				if((disp&0x1F)==disp)
//			{	opw1=0xF000|ex|((disp>>0)&31);
//				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			opw1=0xF00C|ex; odr=1;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

//			if(((disp&0x1FF)==disp) && !(nowxi>>0))
//			if(	(((disp&0x1FF)==disp) ||
//				((ctx->is_fixed32&2) && ((disp|(~0x1FF))==disp))) &&
//				!(nowxi>>0))
			if(chkp_disp9b)
			{
//				opw1=0xF100|ex|((disp>>4)&31);
//				opw2=0x8000|(((disp>>0)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x8000|ex2|((disp>>0)&511);
				break;	}

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0xFFFF)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>0);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~31))==disp))
			{
				opw1=0xF000|ex|((disp>>0)&31);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x8000,
					rm, disp, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex;	odr=1;
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
//			if(!ctx->is_fixed32)
//				if((disp&0x3E)==disp)
//			{	opw1=0xF000|ex|((disp>>1)&31);
//				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			opw1=0xF00D|ex; odr=2;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

//			if(((disp&0x3FE)==disp) && !(nowxi>>1))
//			if(	(((disp&0x3FE)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&1) &&
//					((disp|(~0x3FF))==disp))) &&
//				!(nowxi>>1))
			if(chkp_disp9w)
			{
//				opw1=0xF100|ex|((disp>>5)&31);
//				opw2=0x9000|(((disp>>1)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x9000|ex2|((disp>>1)&511);
				break;	}

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0x1FFFE)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>1);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~63))==disp) && !(disp&1))
			{
				opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&1))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x9000,
					rm, disp>>1, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex;	odr=2;
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
//			if(!ctx->is_fixed32)
//				if((disp&0x7C)==disp)
//			{	opw1=0xF000|ex|((disp>>2)&31);
//				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			opw1=0xF00E|ex; odr=4;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

//			if(((disp&0x7FC)==disp) && !(nowxi>>2))
//			if(	(((disp&0x7FC)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&3) &&
//					((disp|(~0x7FF))==disp))) &&
//				!(nowxi>>2))
			if(chkp_disp9l)
			{
//				opw1=0xF100|ex|((disp>>6)&31);
//				opw2=0xA000|(((disp>>2)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0xA000|ex2|((disp>>2)&511);
				break;	}

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0x3FFFC)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>2);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~127))==disp) && !(disp&3))
			{
				opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&3))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0xA000,
					rm, disp>>2, rn);
				if(i>0)break;
			}

			opw1=0xF000|ex;	odr=4;
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!ctx->is_fixed32)
//				if((disp&0xF8)==disp)
//			{	opw1=0xF000|ex|((disp>>3)&31);
//				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			opw1=0xF00F|ex; odr=8;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

//			if(((disp&0xFF8)==disp) && !(nowxi>>3))
//			if(	(((disp&0xFF8)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&7) &&
//					((disp|(~0xFFF))==disp))) &&
//				!(nowxi>>3))
			if(chkp_disp9q)
			{
//				opw1=0xF100|ex|((disp>>7)&31);
//				opw2=0xB000|(((disp>>3)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0xB000|ex2|((disp>>3)&511);
				break;	}

#if 0
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32 && !nowxi)
			{
				if((disp&0x7FFF8)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>3);
					break;	}
			}
#endif

			if(ctx->has_xgpr && ((disp|(~255))==disp) && !(disp&7))
			{
				opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&7))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0xB000,
					rm, disp>>3, rn);
				if(i>0)break;
			}


			opw1=0xF000|ex;	odr=8;
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVQ_DISP:
			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&7))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0xB000,
					rm, disp>>3, rn);
				if(i>0)break;
			}

//			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
//			opw2=0xB000|ex2|((disp>>3)&511);
//			break;

//			opw1=0xF000|ex;	odr=8;
//			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);
//			break;

			opw1=0xFA00|((disp>>19)&0x01FF);
			opw2=0x0000|((disp>> 3)&0xFFFF);
			opw3=0xF000|ex;
			opw4=0x0F00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_MOVQ_DISP24:
			if(ctx->has_jumbo && !(ctx->op_is_wex2&12))
			{
				opw1=0xFE00|((disp>>27)&0x00FF);
				opw2=0x0000|((disp>>11)&0xFFFF);
				opw3=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw4=0xB000|ex2|
					((disp>>3)&0x00FF)|
					((disp>>23)&0x0100);
			}else
			{
				opw1=0xFA00|((disp>>19)&0x01FF);
				opw2=0x0000|((disp>> 3)&0xFFFF);
				opw3=0xF000|ex;
				opw4=0x0F00|((rn&15)<<4)|((rm&15)<<0);
			}
			break;

		case BGBCC_SH_NMID_MOVUB:
//			if(!ctx->is_fixed32)
//				if((disp&0x1F)==disp)
//			{	opw1=0xF080|ex|((disp>>0)&31);
//				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

//			if(((disp&0x1FF)==disp) && !(nowxi>>0))
//			if(	(((disp&0x1FF)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&0) &&
//					((disp|(~0x1FF))==disp))) &&
//				!(nowxi>>0))
			if(chkp_disp9b)
			{
//				opw1=0xF180|ex|((disp>>4)&31);
//				opw2=0x8000|(((disp>>0)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x8800|ex2|((disp>>0)&511);
				break;	}

			if(ctx->has_xgpr && ((disp|(~31))==disp))
			{
				opw1=0xF080|ex|((disp>>0)&31);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x8800,
					rm, disp, rn);
				if(i>0)break;
			}

//			opw1=0xF08C|ex; odr=1;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;	odr=1;
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
//			if(!ctx->is_fixed32)
//				if((disp&0x3E)==disp)
//			{	opw1=0xF080|ex|((disp>>1)&31);
//				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

//			if(((disp&0x3FE)==disp) && !(nowxi>>1))
//			if(	(((disp&0x3FE)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&1) &&
//					((disp|(~0x3FF))==disp))) &&
//				!(nowxi>>1))
			if(chkp_disp9w)
			{
//				opw1=0xF180|ex|((disp>>5)&31);
//				opw2=0x9000|(((disp>>1)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x9800|ex2|((disp>>1)&511);
				break;	}

			if(ctx->has_xgpr && ((disp|(~63))==disp) && !(disp&1))
			{
				opw1=0xF080|ex|((disp>>1)&31);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&1))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x9800,
					rm, disp>>1, rn);
				if(i>0)break;
			}

//			opw1=0xF08D|ex; odr=2;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;	odr=2;
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
//			if(!ctx->is_fixed32)
//				if((disp&0x7C)==disp)
//			{	opw1=0xF080|ex|((disp>>2)&31);
//				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

//			if(((disp&0x7FC)==disp) && !(nowxi>>2))
//			if(	(((disp&0x7FC)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&3) &&
//					((disp|(~0x7FF))==disp))) &&
//				!(nowxi>>2))
			if(chkp_disp9l)
			{
//				opw1=0xF180|ex|((disp>>6)&31);
//				opw2=0xA000|(((disp>>2)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0xA800|ex2|((disp>>2)&511);
				break;	}

			if(ctx->has_xgpr && ((disp|(~127))==disp) && !(disp&3))
			{
				opw1=0xF080|ex|((disp>>2)&31);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				break;
			}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&3))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0xA800,
					rm, disp>>2, rn);
				if(i>0)break;
			}

//			opw1=0xF08E|ex; odr=4;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;	odr=4;
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVDL:
//			if(!ctx->is_fixed32)
//				if((disp&0x7C)==disp)
//			{	opw1=0xF080|ex|((disp>>2)&31);
//				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
//				break;		}

//			if(((disp&0x7FC)==disp) && !(nowxi>>2))
			if(chkp_disp9l)
			{
//				opw1=0xF180|ex|((disp>>6)&31);
//				opw2=0xB000|(((disp>>2)&15)<<8)|
//					((rn&15)<<4)|((rm&15)<<0);
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
//				opw2=0xB800|ex2|((disp>>2)&511);
				opw2=0xA800|ex2|((disp>>2)&511);
				break;	}
			opw1=0xF080|ex;	odr=4;
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;

//			if(rn&1)
//				{ BGBCC_DBGBREAK }
//			rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
//			if(rn<0)
//				{ BGBCC_DBGBREAK }

#if 0
			if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x4800|((rn&15)<<4)|((rm&15)<<0);
				break;		}
#endif

#if 1
//			if(((disp&0xFF8)==disp) && !(nowxi>>3))
//			if(	(((disp&0xFF8)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&7) &&
//					((disp|(~0xFFF))==disp))) &&
//				!(nowxi>>3))
			if(chkp_disp9q)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x7800|ex2|((disp>>3)&511);
				break;
			}
#endif

			if(ctx->has_xgpr && ((disp|(~255))==disp) && !(disp&7))
			{
				opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x4800|((rn&15)<<4)|((rm&15)<<0);
				break;
			}


#if 1
			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&7))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x7800,
					rm, disp>>3, rn);
				if(i>0)break;
			}
#endif

			opw1=0xF000|ex;	odr=8;
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_FMOVS:
//			if(((disp&0x7FC)==disp) && !(nowxi>>2))
//			if(	(((disp&0x7FC)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&3) &&
//					((disp|(~0x7FF))==disp))) &&
//				!(nowxi>>2))
			if(chkp_disp9l)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x6000|ex2|((disp>>2)&511);
				break;
			}
			opw1=0xF000|ex;	odr=4;
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_FMOVH:
//			if(((disp&0x3FE)==disp) && !(nowxi>>1))
			if(chkp_disp9w)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x6800|ex2|((disp>>1)&511);
				break;
			}
			opw1=0xF000|ex;	odr=2;
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			if(((disp&0x3E)==disp) && !(nowxi>>1))
			{
				opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x9800|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF000|ex;	odr=2;
			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_MOVUTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			if(((disp&0x3E)==disp) && !(nowxi>>1))
			{
				opw1=0xF080|ex|((disp>>1)&31);
				opw2=0x9800|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF080|ex;	odr=2;
			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_MOVHTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			if(((disp&0x3E)==disp) && !(nowxi>>1))
			{
				opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x9900|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF000|ex;	odr=2;
			opw2=0x9D00|((rn&15)<<4)|((rm&15)<<0);
			break;

#if 0
		case BGBCC_SH_NMID_CMOVTB:
		case BGBCC_SH_NMID_CMOVFB:
		case BGBCC_SH_NMID_CMOVUTB:
		case BGBCC_SH_NMID_CMOVUFB:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00C|ex;	odr=1;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTW:
		case BGBCC_SH_NMID_CMOVFW:
		case BGBCC_SH_NMID_CMOVUTW:
		case BGBCC_SH_NMID_CMOVUFW:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00D|ex;	odr=2;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTL:
		case BGBCC_SH_NMID_CMOVFL:
		case BGBCC_SH_NMID_CMOVUTL:
		case BGBCC_SH_NMID_CMOVUFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00E|ex;	odr=4;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTQ:
		case BGBCC_SH_NMID_CMOVFQ:
		case BGBCC_SH_NMID_CMOVDTL:
		case BGBCC_SH_NMID_CMOVDFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00F|ex;	odr=(nmid&8)?4:8;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_LEAB:
//			if(((disp&0x1FF)==disp) && !(nowxi>>0))
//			if(	(((disp&0x1FF)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&0) &&
//					((disp|(~0x1FF))==disp))) &&
//				!(nowxi>>0))
			if(chkp_disp9b)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x0800|ex2|((disp>>0)&511);
				break;	}

			if(ctx->has_jumbo && !ctx->op_is_wex2)
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x0800,
					rm, disp, rn);
				if(i>0)break;
			}

			opw1=0xF080|ex;	odr=1;
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_LEAW:
//			if(((disp&0x3FE)==disp) && !(nowxi>>1))
//			if(	(((disp&0x3FE)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&1) &&
//					((disp|(~0x3FF))==disp))) &&
//				!(nowxi>>1))
			if(chkp_disp9w)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x1800|ex2|((disp>>1)&511);
				break;	}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&1))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x1800,
					rm, disp>>1, rn);
				if(i>0)break;
			}

			opw1=0xF080|ex;	odr=2;
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
//			if(((disp&0x7FC)==disp) && !(nowxi>>2))
//			if(	(((disp&0x7F8)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&3) &&
//					((disp|(~0x7FF))==disp))) &&
//				!(nowxi>>2))
			if(chkp_disp9l)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x2800|ex2|((disp>>2)&511);
				break;	}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&3))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x2800,
					rm, disp>>2, rn);
				if(i>0)break;
			}

			opw1=0xF080|ex;	odr=4;
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
//			if(((disp&0xFF8)==disp) && !(nowxi>>3))
//			if(	(((disp&0xFF8)==disp) ||
//				((ctx->is_fixed32&2) && !(disp&7) &&
//					((disp|(~0xFFF))==disp))) &&
//				!(nowxi>>3))
			if(chkp_disp9q)
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x3800|ex2|((disp>>3)&511);
				break;	}

			if(ctx->has_jumbo && !ctx->op_is_wex2 && !(disp&7))
			{
				i=BGBCC_JX2_ComposeJumboRegImmRegF2A(ctx,
					&opw1, &opw2, &opw3, &opw4,
					0xF100, 0x3800,
					rm, disp>>3, rn);
				if(i>0)break;
			}

			opw1=0xF080|ex;	odr=8;
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_LEATB:
			if((disp&0x1F)==disp)
			{
				opw1=0xF080|ex|((disp>>0)&0x1F);
				opw2=0x4000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF080|ex;	odr=1;
			opw2=0x4400|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_LEATW:
			if((disp&0x3E)==disp)
			{
				opw1=0xF080|ex|((disp>>1)&0x1F);
				opw2=0x4100|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF080|ex;	odr=2;
			opw2=0x4500|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_LEATL:
			if((disp&0x7C)==disp)
			{
				opw1=0xF080|ex|((disp>>2)&0x1F);
				opw2=0x4200|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF080|ex;	odr=4;
			opw2=0x4600|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_LEATQ:
			if((disp&0xF8)==disp)
			{
				opw1=0xF080|ex|((disp>>3)&0x1F);
				opw2=0x4300|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF080|ex;	odr=8;
			opw2=0x4700|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_XLEAB:
			if((disp&0x1F)==disp)
			{	opw1=0xF080|ex|((disp>>0)&31);
				opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF080|ex;	odr=1;
			opw2=0x8400|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_XMOVB:
			if((disp&0x1F)==disp)
			{	opw1=0xF000|ex|((disp>>0)&31);
				opw2=0x8800|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=1;
			opw2=0x8C00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVUB:
			if((disp&0x1F)==disp)
			{	opw1=0xF080|ex|((disp>>0)&31);
				opw2=0x8800|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF080|ex;	odr=1;
			opw2=0x8C00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_XMOVW:
			if((disp&0x3E)==disp)
			{	opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x8900|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=2;
			opw2=0x8D00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVUW:
			if((disp&0x3E)==disp)
			{	opw1=0xF080|ex|((disp>>1)&31);
				opw2=0x8900|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF080|ex;	odr=2;
			opw2=0x8D00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_XMOVL:
			if((disp&0x7C)==disp)
			{	opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x8A00|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=4;
			opw2=0x8E00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVUL:
			if((disp&0x7C)==disp)
			{	opw1=0xF080|ex|((disp>>2)&31);
				opw2=0x8A00|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF080|ex;	odr=4;
			opw2=0x8E00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_XMOVQ:
			if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x8B00|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF000|ex;	odr=8;
			opw2=0x8F00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_XMOVX2:
			if(!(ctx->has_pushx2&1))
				break;

			if((disp&0xF8)==disp)
			{	opw1=0xF080|ex|((disp>>3)&31);
				opw2=0x8B00|((rn&15)<<4)|((rm&15)<<0);
				break;		}
			opw1=0xF080|ex;	odr=8;
			opw2=0x8F00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		case BGBCC_SH_NMID_ADDSL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x000A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_ADDUL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_SUBSL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x000A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_SUBUL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_RSUBSL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x000A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_RSUBUL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_ANDL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_ORL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x6000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_XORL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x7000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;


		case BGBCC_SH_NMID_ADD:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x000B,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_SUB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x000B,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_RSUB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x000B,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_AND:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x080B,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_OR:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x6000, 0xF000, 0x080B,
				rm, disp>>3, rn);
			break;
		case BGBCC_SH_NMID_XOR:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x7000, 0xF000, 0x080B,
				rm, disp>>3, rn);
			break;

#if 0
		case BGBCC_SH_NMID_XCHGL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_XCHGQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x080B,
				rm, disp>>3, rn);
			break;

		case BGBCC_SH_NMID_XCHGVL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x9000, 0xF000, 0x080A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_XCHGVQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x9000, 0xF000, 0x080B,
				rm, disp>>3, rn);
			break;
#endif

		case BGBCC_SH_NMID_ADDSB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x0008,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_ADDUB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x0808,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_SUBSB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x0008,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_SUBUB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x0808,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_RSUBSB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x0008,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_RSUBUB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x0808,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_ANDB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0808,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_ORB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0808,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_XORB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0808,
				rm, disp>>0, rn);
			break;

		case BGBCC_SH_NMID_ADDSW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x0009,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_ADDUW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x2000, 0xF000, 0x0809,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_SUBSW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x0009,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_SUBUW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x3000, 0xF000, 0x0809,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_RSUBSW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x0009,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_RSUBUW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x4000, 0xF000, 0x0809,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_ANDW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0809,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_ORW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0809,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_XORW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x5000, 0xF000, 0x0809,
				rm, disp>>1, rn);
			break;

		case BGBCC_SH_NMID_SWAPB:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x0008,
				rm, disp>>0, rn);
			break;
		case BGBCC_SH_NMID_SWAPW:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x0009,
				rm, disp>>1, rn);
			break;
		case BGBCC_SH_NMID_SWAPL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x000A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_SWAPQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x1000, 0xF000, 0x000B,
				rm, disp>>2, rn);
			break;

		case BGBCC_SH_NMID_SWAPVL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x9000, 0xF000, 0x000A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_SWAPVQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x9000, 0xF000, 0x000B,
				rm, disp>>2, rn);
			break;

		case BGBCC_SH_NMID_MOVVL:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x8000, 0xF000, 0x000A,
				rm, disp>>2, rn);
			break;
		case BGBCC_SH_NMID_MOVVQ:
			BGBCC_JX2_ComposeOp64RegImm17sRegF0(ctx,
				&opw1, &opw2, &opw3, &opw4,
				0xFF00, 0x8000, 0xF000, 0x000B,
				rm, disp>>2, rn);
			break;
		}
	}

#if 0
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		ex2=ex<<4;
	
		nowxi=0;
//		if(ctx->op_is_wex2&4)
//			nowxi=disp>>8;
	
		switch(nmid)
		{
//		case BGBCC_SH_NMID_FMO..VS:
			if(((disp&0x7FC)==disp) && !(nowxi>>2))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x6000|ex2|((disp>>2)&511);
				break;
			}
//			if((disp&0x7C)==disp)
//			{	opw1=0xF000|ex|((disp>>2)&31);
//				opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			opw1=0xF006|ex; odr=4;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=4;
			opw2=0x9600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			if(((disp&0xFF8)==disp) && !(nowxi>>3))
			{
				opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
				opw2=0x7000|ex2|((disp>>3)&511);
				break;
			}
//			if((disp&0xF8)==disp)
//			{	opw1=0xF000|ex|((disp>>3)&31);
//				opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);
//				break;		}
//			opw1=0xF007|ex; odr=8;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=8;
			opw2=0x9700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}
#endif

	if((ctx->op_is_wex2&2) && (odr>0))
		{ opw1=-1; odr=0; }

	exw=0;
	if((disp<0) && !odr && ((opw1&0xFF00)==0xF000))
		{ exw|=0x0100; }
	if((disp<0) && !odr && ((opw1&0xFF00)==0xF100) &&
		((ctx->is_fixed32&2) || (ctx->emit_riscv&0x22)))
		{ exw|=0x0100; }
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
//	if(nmid==BGBCC_SH_NMID_MOVX2)			exw&=~0x0400;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);

		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStReg2(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3, opw4, ex, exw, rt;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn));

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);
	ro=BGBCC_JX2_NormalizeReg(ctx, ro);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }
	}

	if(	(nmid==BGBCC_SH_NMID_XMOVB)		||
		(nmid==BGBCC_SH_NMID_XMOVW)		||
		(nmid==BGBCC_SH_NMID_XMOVL)		||
		(nmid==BGBCC_SH_NMID_XMOVQ)		||
		(nmid==BGBCC_SH_NMID_XMOVUB)	||
		(nmid==BGBCC_SH_NMID_XMOVUW)	||
		(nmid==BGBCC_SH_NMID_XMOVUL)	||
		(nmid==BGBCC_SH_NMID_XMOVX2)	||
		(nmid==BGBCC_SH_NMID_XLEAB)		||
		(nmid==BGBCC_SH_NMID_XLEAW)		||
		(nmid==BGBCC_SH_NMID_XLEAL)		||
		(nmid==BGBCC_SH_NMID_XLEAQ)		)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }

		if(nmid==BGBCC_SH_NMID_XMOVX2)
		{
			rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
			if(rm<0)
				{ BGBCC_DBGBREAK }
		}
	}

	if(	(nmid==BGBCC_SH_NMID_MOVTW_V) ||
		(nmid==BGBCC_SH_NMID_MOVUTW_V) ||
		(nmid==BGBCC_SH_NMID_MOVHTW_V) )
	{
		if(ctx->emit_isprobe)
			return(0);
	
		opw4=BGBCC_SH_NMID_MOVTW;
		if(nmid==BGBCC_SH_NMID_MOVHTW_V)
			opw4=BGBCC_SH_NMID_MOVHTW;

		/* Synthesize the scale-6 index. */
		BGBCC_JX2_TryEmitOpLdReg2Reg(ctx,
			BGBCC_SH_NMID_LEAW, ro, ro, BGBCC_SH_REG_R0);
		rt=BGBCC_JX2_TryEmitOpRegStReg2(ctx,
			opw4, rm, BGBCC_SH_REG_R0, rn);
		return(rt);
	}

	if(!(ctx->has_fmovs&32)	&& !(ctx->emit_isprobe) &&
		(	(nmid==BGBCC_SH_NMID_MOVTW) ||
			(nmid==BGBCC_SH_NMID_MOVUTW) ||
			(nmid==BGBCC_SH_NMID_MOVHTW) ))
	{
		/* We don't have these ops, fake them. */
		BGBCC_JX2_EmitOpLdReg2Reg(ctx,
			BGBCC_SH_NMID_LEAW, rn, ro, BGBCC_SH_REG_R16);
		BGBCC_JX2_EmitOpRegStRegDisp(ctx,
			nmid, rm, BGBCC_SH_REG_R16, 0);
		return(1);
	}

	if(((rn&63)>=2) && (ro==BGBCC_SH_REG_R0))
		ro=BGBCC_SH_REG_R1;

	if(rn==BGBCC_SH_REG_PC)
		rn=BGBCC_SH_REG_R0;

	if((rn==BGBCC_SH_REG_GBR) && (ro!=BGBCC_SH_REG_DLR))
		rn=BGBCC_SH_REG_R1;

	if((rn==BGBCC_SH_REG_TBR) && (ro==BGBCC_SH_REG_DLR))
	{
		rn=BGBCC_SH_REG_R1;
		ro=BGBCC_SH_REG_R1;
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_indx++;
	}

	if(ro==BGBCC_SH_REG_DLR)
	{
		opw1=-1; opw2=-1; opw3=-1; opw4=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else if(ro!=BGBCC_SH_REG_R0)
	{
		opw1=-1; opw2=-1; opw3=-1; opw4=-1;

#if 1
		if(BGBCC_JX2_CheckPadCross32(ctx))
		{
			switch(nmid)
			{
			case BGBCC_SH_NMID_MOVB:
			case BGBCC_SH_NMID_MOVUB:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVW:
			case BGBCC_SH_NMID_MOVUW:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVL:
			case BGBCC_SH_NMID_MOVUL:
			case BGBCC_SH_NMID_MOVDL:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVQ:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
			}
		}
#endif
	}else
	{
		opw1=-1; opw2=-1; opw3=-1; opw4=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3500|((rm&15)<<4);						break;
			opw3=0x0000|((rm&15)<<0);						break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3501|((rm&15)<<4);						break;
			opw3=0x0100|((rm&15)<<0);						break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3502|((rm&15)<<4);						break;
			opw3=0x0200|((rm&15)<<0);						break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3503|((rm&15)<<4);						break;
			opw3=0x0300|((rm&15)<<0);						break;
		}
	}

	if(ctx->is_fixed32 || (ctx->emit_riscv&0x22))
		{ opw1=-1; }

//	opw1=-1;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		(ro==BGBCC_SH_REG_DLR))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
		case BGBCC_SH_NMID_MOVB:
			opw1=0xF000|ex;
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_MOVW:
			opw1=0xF000|ex;
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_MOVL:
			opw1=0xF000|ex;
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			opw1=0xF000|ex;
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;

//			if(rm&1)
//				{ BGBCC_DBGBREAK }
//			rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
//			if(rm<0)
//				{ BGBCC_DBGBREAK }

			opw1=0xF000|ex;
			opw2=0x4400|((rn&15)<<4)|((rm&15)<<0);
			break;

#if 0
		case BGBCC_SH_NMID_CMOVTB:
		case BGBCC_SH_NMID_CMOVFB:
		case BGBCC_SH_NMID_CMOVUTB:
		case BGBCC_SH_NMID_CMOVUFB:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF004|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTW:
		case BGBCC_SH_NMID_CMOVFW:
		case BGBCC_SH_NMID_CMOVUTW:
		case BGBCC_SH_NMID_CMOVUFW:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF005|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTL:
		case BGBCC_SH_NMID_CMOVFL:
		case BGBCC_SH_NMID_CMOVUTL:
		case BGBCC_SH_NMID_CMOVUFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF006|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTQ:
		case BGBCC_SH_NMID_CMOVFQ:
		case BGBCC_SH_NMID_CMOVDTL:
		case BGBCC_SH_NMID_CMOVDFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF007|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		case BGBCC_SH_NMID_XMOVUB:
		case BGBCC_SH_NMID_XMOVB:
			opw1=0xF000|ex;
			opw2=0x8400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUW:
		case BGBCC_SH_NMID_XMOVW:
			opw1=0xF000|ex;
			opw2=0x8500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUL:
		case BGBCC_SH_NMID_XMOVL:
			opw1=0xF000|ex;
			opw2=0x8600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVQ:
			opw1=0xF000|ex;
			opw2=0x8700|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVX2:
			if(!(ctx->has_pushx2&1))
				break;

			opw1=0xF080|ex;
			opw2=0x8700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
		case BGBCC_SH_NMID_MOVB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_MOVW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_MOVL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;

//			if(rm&1)
//				{ BGBCC_DBGBREAK }
//			rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
//			if(rm<0)
//				{ BGBCC_DBGBREAK }

			opw1=0xF000|ex|(ro&15);
			opw2=0x4400|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_FMOVS:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVH:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4700|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVTW:
		case BGBCC_SH_NMID_MOVUTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			opw1=0xF000|ex|(ro&15);
			opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVHTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			opw1=0xF000|ex|(ro&15);
			opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);			break;


		case BGBCC_SH_NMID_XMOVUB:
		case BGBCC_SH_NMID_XMOVB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUW:
		case BGBCC_SH_NMID_XMOVW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUL:
		case BGBCC_SH_NMID_XMOVL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8700|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVX2:
			if(!(ctx->has_pushx2&1))
				break;

			opw1=0xF080|ex|(ro&15);
			opw2=0x8700|((rn&15)<<4)|((rm&15)<<0);
			break;
		}
	}

#if 0
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;

		switch(nmid)
		{
//		case BGBCC_SH_NMID_FMO,,VS:
			opw1=0xF000|ex|(ro&15);
			opw2=0x9200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			opw1=0xF000|ex|(ro&15);
			opw2=0x9300|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}
#endif

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, ro)) exw|=0x0100;
//	if(nmid==BGBCC_SH_NMID_MOVX2)			exw&=~0x0200;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpRegStReg2(ctx, nmid, rm, ro, rn);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdReg2Reg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3, opw4, ex, exw, rt;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn));

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);
	ro=BGBCC_JX2_NormalizeReg(ctx, ro);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }
	}

	if(	(nmid==BGBCC_SH_NMID_XMOVB)		||
		(nmid==BGBCC_SH_NMID_XMOVW)		||
		(nmid==BGBCC_SH_NMID_XMOVL)		||
		(nmid==BGBCC_SH_NMID_XMOVQ)		||
		(nmid==BGBCC_SH_NMID_XMOVUB)	||
		(nmid==BGBCC_SH_NMID_XMOVUW)	||
		(nmid==BGBCC_SH_NMID_XMOVUL)	||
		(nmid==BGBCC_SH_NMID_XMOVX2)	||
		(nmid==BGBCC_SH_NMID_XLEAB)		||
		(nmid==BGBCC_SH_NMID_XLEAW)		||
		(nmid==BGBCC_SH_NMID_XLEAL)		||
		(nmid==BGBCC_SH_NMID_XLEAQ)		)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }

		if(	(nmid==BGBCC_SH_NMID_XMOVX2) ||
			(nmid==BGBCC_SH_NMID_XLEAB))
		{
			rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
			if(rn<0)
				{ BGBCC_DBGBREAK }
		}
	}

	if(	(nmid==BGBCC_SH_NMID_MOVTW_V) ||
		(nmid==BGBCC_SH_NMID_MOVUTW_V) ||
		(nmid==BGBCC_SH_NMID_MOVHTW_V) )
	{
		if(ctx->emit_isprobe)
			return(0);
	
		opw4=BGBCC_SH_NMID_MOVTW;
		if(nmid==BGBCC_SH_NMID_MOVUTW_V)
			opw4=BGBCC_SH_NMID_MOVUTW;
		if(nmid==BGBCC_SH_NMID_MOVHTW_V)
			opw4=BGBCC_SH_NMID_MOVHTW;

		/* Synthesize the scale-6 index. */
		BGBCC_JX2_TryEmitOpLdReg2Reg(ctx,
			BGBCC_SH_NMID_LEAW, ro, ro, BGBCC_SH_REG_R0);
		rt=BGBCC_JX2_TryEmitOpLdReg2Reg(ctx,
			opw4, rm, BGBCC_SH_REG_R0, rn);
		return(rt);
	}

	if(!(ctx->has_fmovs&32)	&& !(ctx->emit_isprobe) &&
		(	(nmid==BGBCC_SH_NMID_MOVTW) ||
			(nmid==BGBCC_SH_NMID_MOVUTW) ||
			(nmid==BGBCC_SH_NMID_MOVHTW) ))
	{
		/* We don't have these ops, fake them. */
		BGBCC_JX2_EmitOpLdReg2Reg(ctx,
			BGBCC_SH_NMID_LEAW, rm, ro, BGBCC_SH_REG_R16);
		BGBCC_JX2_EmitOpLdRegDispReg(ctx,
			nmid, BGBCC_SH_REG_R16, 0, rn);
		return(1);
	}

	if(((rm&63)>=2) && (ro==BGBCC_SH_REG_R0))
		ro=BGBCC_SH_REG_R1;

	if(rm==BGBCC_SH_REG_PC)
		rm=BGBCC_SH_REG_R0;

	if((rm==BGBCC_SH_REG_GBR) && (ro!=BGBCC_SH_REG_DLR))
		rm=BGBCC_SH_REG_R1;

	if((rm==BGBCC_SH_REG_TBR) && (ro==BGBCC_SH_REG_DLR))
	{
		rm=BGBCC_SH_REG_R1;
		ro=BGBCC_SH_REG_R1;
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_indx++;
	}

	opw4=-1;

	if(ro==BGBCC_SH_REG_DLR)
	{
		opw1=-1; opw2=-1; opw3=-1; opw4=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x5200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x5300|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x8800|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else if(ro!=BGBCC_SH_REG_R0)
	{
		opw1=-1; opw2=-1; opw3=-1; opw4=-1;

#if 1
		if(BGBCC_JX2_CheckPadCross32(ctx))
		{
			switch(nmid)
			{
#if 1
			case BGBCC_SH_NMID_MOVB:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVW:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVL:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVQ:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVUB:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x5200|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVUW:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x5300|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVUL:
			case BGBCC_SH_NMID_MOVDL:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x8800|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
			case BGBCC_SH_NMID_LEAB:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_LEAW:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_LEAL:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_LEAQ:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif
			}
		}
#endif
	}else
	{
		opw1=-1; opw2=-1; opw3=-1; opw4=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x3508|((rn&15)<<4);						break;
			opw3=0x0800|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x3509|((rn&15)<<4);						break;
			opw3=0x0900|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x350A|((rn&15)<<4);						break;
			opw3=0x0A00|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x350B|((rn&15)<<4);						break;
			opw3=0x0B00|((rn&15)<<4);						break;

		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1800|(ro&15);
			opw2=0x1000|(rm&15);
			opw3=0x5200|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1800|(ro&15);
			opw2=0x1000|(rm&15);
			opw3=0x5300|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1800|(ro&15);
			opw2=0x1000|(rm&15);
			opw3=0x8800|((rn&15)<<4);						break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(ctx->is_fixed32 || (ctx->emit_riscv&0x22))
		{ opw1=-1; }

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		(ro==BGBCC_SH_REG_DLR))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw1=0xF000|ex;
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			opw1=0xF000|ex;
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			opw1=0xF000|ex;
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			opw1=0xF000|ex;
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			opw1=0xF080|ex;
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			opw1=0xF080|ex;
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			opw1=0xF080|ex;
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
//		case BGBCC_SH_NMID_MOVDL:
//			opw1=0xF080|ex;
//			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;
//			if(rn&1)
//				{ BGBCC_DBGBREAK }
//			rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
//			if(rn<0)
//				{ BGBCC_DBGBREAK }

			opw1=0xF000|ex;
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);
			break;

#if 0
		case BGBCC_SH_NMID_CMOVTB:
		case BGBCC_SH_NMID_CMOVFB:
		case BGBCC_SH_NMID_CMOVUTB:
		case BGBCC_SH_NMID_CMOVUFB:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00C|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTW:
		case BGBCC_SH_NMID_CMOVFW:
		case BGBCC_SH_NMID_CMOVUTW:
		case BGBCC_SH_NMID_CMOVUFW:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00D|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTL:
		case BGBCC_SH_NMID_CMOVFL:
		case BGBCC_SH_NMID_CMOVUTL:
		case BGBCC_SH_NMID_CMOVUFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00E|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
		case BGBCC_SH_NMID_CMOVTQ:
		case BGBCC_SH_NMID_CMOVFQ:
		case BGBCC_SH_NMID_CMOVDTL:
		case BGBCC_SH_NMID_CMOVDFL:
			if(nmid&4)ex|=0x0010;
			if(nmid&8)ex|=0x0080;
			opw1=0xF00F|ex;
			opw2=0x1B00|((rn&15)<<4)|((rm&15)<<0);
			break;
#endif

		case BGBCC_SH_NMID_XLEAB:
			if(!(ctx->has_pushx2&1))
				break;
			opw1=0xF080|ex;
			opw2=0x8400|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_XMOVB:
			opw1=0xF000|ex;
			opw2=0x8C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVW:
			opw1=0xF000|ex;
			opw2=0x8D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVL:
			opw1=0xF000|ex;
			opw2=0x8E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVQ:
			opw1=0xF000|ex;
			opw2=0x8F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_XMOVUB:
			opw1=0xF080|ex;
			opw2=0x8C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUW:
			opw1=0xF080|ex;
			opw2=0x8D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUL:
			opw1=0xF080|ex;
			opw2=0x8E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVX2:
			if(!(ctx->has_pushx2&1))
				break;
			opw1=0xF080|ex;
			opw2=0x8F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
//		case BGBCC_SH_NMID_MOVDL:
//			opw1=0xF080|ex|(ro&15);
//			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_LDTEX:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVX2:
			if(!(ctx->has_pushx2&1))
				break;
//			if(rn&1)
//				{ BGBCC_DBGBREAK }
//			rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
//			if(rn<0)
//				{ BGBCC_DBGBREAK }

			opw1=0xF000|ex|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);
			break;

		case BGBCC_SH_NMID_FMOVS:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVH:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			opw1=0xF000|ex|(ro&15);
			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			opw1=0xF080|ex|(ro&15);
			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVHTW:
			if(!(ctx->has_fmovs&32) && ctx->emit_isprobe)
				break;
			opw1=0xF000|ex|(ro&15);
			opw2=0x9D00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
		case BGBCC_SH_NMID_LEAB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_LEAB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_LEATB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x4400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEATW:
			opw1=0xF080|ex|(ro&15);
			opw2=0x4500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEATL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x4600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEATQ:
			opw1=0xF080|ex|(ro&15);
			opw2=0x4700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif


		case BGBCC_SH_NMID_XLEAB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x8400|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_XMOVB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x8F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_XMOVUB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x8C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUW:
			opw1=0xF080|ex|(ro&15);
			opw2=0x8D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVUL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x8E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_XMOVX2:
			if(!(ctx->has_pushx2&1))
				break;
			opw1=0xF080|ex|(ro&15);
			opw2=0x8F00|((rn&15)<<4)|((rm&15)<<0);
			break;
		}
	}

#if 0
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;

		switch(nmid)
		{
//		case BGBCC_SH_NMID_FM//OVS:
			opw1=0xF000|ex|(ro&15);
			opw2=0x9600|((rn&15)<<4)|((rm&15)<<0);			break;
//		case BGBCC_SH_NMID_FM//OVD:	
			opw1=0xF000|ex|(ro&15);
			opw2=0x9700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}
#endif

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, ro)) exw|=0x0100;
//	if(nmid==BGBCC_SH_NMID_MOVX2)			exw&=~0x0400;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(!BGBCC_JX2_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_EmitOpLdReg2DispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdReg2DispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStReg2Disp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw1, opw2, opw3, opw4, exw;
	int odr;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpRegStReg2Disp(ctx,
			nmid, rm, ro, rn, disp));

	if(ro==BGBCC_SH_REG_DLR)
	{
		BGBCC_DBGBREAK
	}

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);
	ro=BGBCC_JX2_NormalizeReg(ctx, ro);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rm=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		if(rm<0)
			{ BGBCC_DBGBREAK }
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_idxdisp++;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=1;
		opw1=0x1000|(ro&15); odr=1;
		opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=2;
		opw1=0x1000|(ro&15); odr=2;
		opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=4;
		opw1=0x1000|(ro&15); odr=4;
		opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=8;
		opw1=0x1000|(ro&15); odr=8;
		opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32 || (ctx->emit_riscv&0x22))
		{ opw1=-1; odr=0; }

	if((ctx->op_is_wex2&2) && (odr>0))
		{ opw1=-1; odr=0; }

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, ro)) exw|=0x0100;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1<0)
	{
		BGBCC_JX2_EmitOpLdRegDispReg(ctx,
			BGBCC_SH_NMID_LEAB, rn, disp, BGBCC_SH_REG_R16);
		BGBCC_JX2_EmitOpRegStReg2(ctx, nmid, rm, ro, BGBCC_SH_REG_R16);
		return(1);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp);
	
		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_TryEmitOpLdReg2DispReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	int opw1, opw2, opw3, opw4, exw;
	int odr;

	if((ctx->emit_riscv&0x11) && !(ctx->emit_isprobe&2))
		return(BGBCC_JX2RV_TryEmitOpLdReg2DispReg(ctx,
			nmid, rm, ro, disp, rn));

	if(ro==BGBCC_SH_REG_DLR)
	{
		BGBCC_DBGBREAK
	}

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);
	rm=BGBCC_JX2_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2_NormalizeReg(ctx, rn);
	ro=BGBCC_JX2_NormalizeReg(ctx, ro);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		rn=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);
		if(rn<0)
			{ BGBCC_DBGBREAK }
	}

	if(!ctx->is_simpass)
	{
		ctx->stat_ldst_tot++;
		ctx->stat_ldst_idxdisp++;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=1;
		opw1=0x1000|(ro&15); odr=1;
		opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=2;
		opw1=0x1000|(ro&15); odr=2;
		opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=4;
		opw1=0x1000|(ro&15); odr=4;
		opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=8;
		opw1=0x1000|(ro&15); odr=8;
		opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=1;
		opw1=0x1000|(ro&15); odr=1;
		opw2=0x5200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=2;
		opw1=0x1000|(ro&15); odr=2;
		opw2=0x5300|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=4;
		opw1=0x1000|(ro&15); odr=4;
		opw2=0x8800|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32 || (ctx->emit_riscv&0x22))
		{ opw1=-1; odr=0; }

	if((ctx->op_is_wex2&2) && (odr>0))
		{ opw1=-1; odr=0; }

	exw=0;
	if(ctx->op_is_wex2&8) exw|=0x1000;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rn)) exw|=0x0400;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, rm)) exw|=0x0200;
	if(BGBCC_JX2_EmitCheckRegExt5(ctx, ro)) exw|=0x0100;
	if(exw)
	{
		BGBCC_JX2_EmitOpCheckRepack4Exw(ctx, exw,
			&opw1, &opw2, &opw3, &opw4);
	}

	if(opw1<0)
	{
		BGBCC_JX2_EmitOpLdRegDispReg(ctx,
			BGBCC_SH_NMID_LEAB, rm, disp, BGBCC_SH_REG_R16);
		BGBCC_JX2_EmitOpLdReg2Reg(ctx, nmid, BGBCC_SH_REG_R16, ro, rn);
		return(1);
	}

	if(opw1>=0)
	{
		if(ctx->emit_isprobe)
		{
			if(ctx->emit_isprobe&2)
			{
				BGBCC_JX2_EmitOpCheckRepack4(ctx,
					&opw1, &opw2, &opw3, &opw4);
				ctx->probe_opw[0]=opw1;
				ctx->probe_opw[1]=opw2;
				ctx->probe_opw[2]=opw3;
				ctx->probe_opw[3]=opw4;
			}
			return(1);
		}

		BGBCC_JX2DA_EmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn);

		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}

//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
//		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		BGBCC_JX2_EmitPadCheckAligned(ctx);
		return(1);
	}

	return(0);
}

