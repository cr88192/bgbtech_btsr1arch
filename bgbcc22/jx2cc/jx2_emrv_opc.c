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

int BGBCC_JX2RV_NormalizeReg(BGBCC_JX2_Context *ctx, int reg)
{
	if(reg==BGBCC_SH_REG_ZZR)
		return(BGBCC_SH_REG_R0);
	if(reg==BGBCC_SH_REG_LR)
		return(BGBCC_SH_REG_R1);
	if(reg==BGBCC_SH_REG_SP)
		return(BGBCC_SH_REG_R2);
	if(reg==BGBCC_SH_REG_GBR)
		return(BGBCC_SH_REG_R3);
//	if(reg==BGBCC_SH_REG_TBR)
//		return(BGBCC_SH_REG_R4);
	if(reg==BGBCC_SH_REG_DHR)
		return(BGBCC_SH_REG_R5);
	return(reg);
}

int BGBCC_JX2RV_CheckRegIsGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		return(0);
	if((reg&63)>=32)
		return(0);
	return(1);
}

int BGBCC_JX2RV_CheckRegIsFPR(BGBCC_JX2_Context *ctx, int reg)
{
	if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		return(0);
	if((reg&63)<32)
		return(0);
	return(1);
}

int BGBCC_JX2RV_EmitOpRegRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	if(!BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int rn)
{
	s64 opw1, opw2, opw3, opw4;
	int rs1, rt1, rn1;
	int i, ex, exw, tr0;

	if(nmid==BGBCC_SH_NMID_LEAB)
		nmid=BGBCC_SH_NMID_ADD;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	rs=BGBCC_JX2RV_NormalizeReg(ctx, rs);
	rt=BGBCC_JX2RV_NormalizeReg(ctx, rt);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	if(	(nmid==BGBCC_SH_NMID_FADD) ||
		(nmid==BGBCC_SH_NMID_FSUB) ||
		(nmid==BGBCC_SH_NMID_FMUL) ||
		(nmid==BGBCC_SH_NMID_FDIV))
	{
		rs1=rs;
		rt1=rt;
		rn1=rn;

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))
			rs1=BGBCC_SH_REG_R32;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))
			rt1=BGBCC_SH_REG_R33;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))
			rn1=BGBCC_SH_REG_R35;
		
		if((rs1!=rs) || (rt1!=rt) || (rn1!=rn))
		{
			if(ctx->emit_isprobe)
				return(0);

			if(rs1!=rs)
			{
				BGBCC_JX2RV_TryEmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					rs, rs1);
			}
			if(rt1!=rt)
			{
				BGBCC_JX2RV_TryEmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					rt, rt1);
			}
			
			BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nmid, rs1, rt1, rn1);

			if(rn1!=rn)
			{
				BGBCC_JX2RV_TryEmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					rn1, rn);
			}
			
			return(1);
		}
	}

	if(	(nmid==BGBCC_SH_NMID_FCMPEQ) ||
		(nmid==BGBCC_SH_NMID_FCMPGT) ||
		(nmid==BGBCC_SH_NMID_FCMPGE)
		)
	{
		rs1=rs;
		rt1=rt;
		rn1=rn;

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))
			rs1=BGBCC_SH_REG_R32;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))
			rt1=BGBCC_SH_REG_R33;
		
		if((rs1!=rs) || (rt1!=rt) || (rn1!=rn))
		{
			if(ctx->emit_isprobe)
				return(0);

			if(rs1!=rs)
			{
				BGBCC_JX2RV_TryEmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					rs, rs1);
			}
			if(rt1!=rt)
			{
				BGBCC_JX2RV_TryEmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					rt, rt1);
			}
			
			BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nmid, rs1, rt1, rn);
			
			return(1);
		}
	}

	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x40000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00001033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00004033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_SHARQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x40005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);

	case BGBCC_SH_NMID_SHLRQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_OR:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00006033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_AND:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00007033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;


	case BGBCC_SH_NMID_ADDSL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SUBSL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_CMPQLT:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_CMPQGT:
	case BGBCC_SH_NMID_CMPGT:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		break;

	case BGBCC_SH_NMID_CMPGE:
	case BGBCC_SH_NMID_CMPQGE:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_CMPQLE:
//	case BGBCC_SH_NMID_CMPLE:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		opw2=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_CMPQEQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x40000033|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		break;
	case BGBCC_SH_NMID_CMPEQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x4000003B|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		break;

	case BGBCC_SH_NMID_CMPQNE:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x40000033|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		opw3=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;
#if 0
	case BGBCC_SH_NMID_CMPNE:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x4000003B|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		opw3=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;
#endif

#if 1
	case BGBCC_SH_NMID_TST:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x00007033|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		opw3=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_NTST:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x00007033|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
//		opw3=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;
#endif

	case BGBCC_SH_NMID_FCMPEQ:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0xA2002053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_FCMPGT:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0xA2001053|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		opw1=0xA2001053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_FCMPGE:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0xA2000053|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		opw1=0xA2000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_ADDUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x02001013|((rn&31)<<7)|((rn&31)<<15);
		opw3=0x02005013|((rn&31)<<7)|((rn&31)<<15);
		break;
	case BGBCC_SH_NMID_SUBUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x02001013|((rn&31)<<7)|((rn&31)<<15);
		opw3=0x02005013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_SHAD:
	case BGBCC_SH_NMID_SHLD:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000103B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_SHAR:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);

	case BGBCC_SH_NMID_SHLR:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MULSL:
	case BGBCC_SH_NMID_MULSW:
	case BGBCC_SH_NMID_MULUW:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MULQ:
	case BGBCC_SH_NMID_MULSQ:
	case BGBCC_SH_NMID_DMULS:
	case BGBCC_SH_NMID_MULL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MULUQ:
	case BGBCC_SH_NMID_DMULU:
	case BGBCC_SH_NMID_MULUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		/* ??? */
		opw1=0x02000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_DIVSL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200403B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_DIVSQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02004033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_DIVUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_DIVUQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MODSL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200603B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MODSQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02006033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MODUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200703B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MODUQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02007033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_FADD:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x02000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_FSUB:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x0A000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_FMUL:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x12000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_FDIV:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x1A000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegRegReg(ctx, nmid, rs, rt, rn);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		if(opw4>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_EmitOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 imm, int rn)
{
	if(!BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, s64 imm, int rn)
{
	s64 opw1, opw2, opw3, opw4, imm12;
	int i, ex, exw, nm1, not12;

	nm1=-1;
	if(nmid==BGBCC_SH_NMID_SUB)
		nm1=BGBCC_SH_NMID_ADD;
	if(nmid==BGBCC_SH_NMID_SUBSL)
		nm1=BGBCC_SH_NMID_ADDSL;
	if(nmid==BGBCC_SH_NMID_SUBUL)
		nm1=BGBCC_SH_NMID_ADDUL;

	if((nmid==BGBCC_SH_NMID_SHAD) && (imm<0))
		nm1=BGBCC_SH_NMID_SHAR;
	if((nmid==BGBCC_SH_NMID_SHADQ) && (imm<0))
		nm1=BGBCC_SH_NMID_SHARQ;

	if((nmid==BGBCC_SH_NMID_SHLD) && (imm<0))
		nm1=BGBCC_SH_NMID_SHLR;
	if((nmid==BGBCC_SH_NMID_SHLDQ) && (imm<0))
		nm1=BGBCC_SH_NMID_SHLRQ;

	if((nmid==BGBCC_SH_NMID_SHAR) && (imm<0))
		nm1=BGBCC_SH_NMID_SHAD;
	if((nmid==BGBCC_SH_NMID_SHARQ) && (imm<0))
		nm1=BGBCC_SH_NMID_SHADQ;

	if((nmid==BGBCC_SH_NMID_SHLR) && (imm<0))
		nm1=BGBCC_SH_NMID_SHLD;
	if((nmid==BGBCC_SH_NMID_SHLRQ) && (imm<0))
		nm1=BGBCC_SH_NMID_SHLDQ;

	if(nm1>0)
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			nm1, rm, -imm, rn));
	}

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	not12=0;
	opw4=((s32)(imm<<20))>>20;
	imm12=imm&4095;
	if(opw4!=imm)
		not12=1;

	if(	(nmid==BGBCC_SH_NMID_MULL)	||
		(nmid==BGBCC_SH_NMID_MULSL) ||
		(nmid==BGBCC_SH_NMID_MULSQ) ||
		(nmid==BGBCC_SH_NMID_MULUL) ||
		(nmid==BGBCC_SH_NMID_MULUQ)	||
		(nmid==BGBCC_SH_NMID_DIVSL) ||
		(nmid==BGBCC_SH_NMID_DIVSQ) ||
		(nmid==BGBCC_SH_NMID_DIVUL) ||
		(nmid==BGBCC_SH_NMID_DIVUQ)	||
		(nmid==BGBCC_SH_NMID_MODSL) ||
		(nmid==BGBCC_SH_NMID_MODSQ) ||
		(nmid==BGBCC_SH_NMID_MODUL) ||
		(nmid==BGBCC_SH_NMID_MODUQ) ||

		(nmid==BGBCC_SH_NMID_DMULS) ||
		(nmid==BGBCC_SH_NMID_DMULU) ||

		(nmid==BGBCC_SH_NMID_CMPEQ) ||
		(nmid==BGBCC_SH_NMID_CMPQEQ) ||
//		(nmid==BGBCC_SH_NMID_CMPNE) ||
		(nmid==BGBCC_SH_NMID_CMPQNE) ||
		(nmid==BGBCC_SH_NMID_CMPGT) ||
		(nmid==BGBCC_SH_NMID_CMPQGT) ||
		(nmid==BGBCC_SH_NMID_CMPGE) ||
		(nmid==BGBCC_SH_NMID_CMPQGE) ||
		(nmid==BGBCC_SH_NMID_CMPQLT) ||
		(nmid==BGBCC_SH_NMID_CMPQLE) ||
		(nmid==BGBCC_SH_NMID_TST) ||
		(nmid==BGBCC_SH_NMID_NTST)
		)
	{
		not12=1;
	}

	if(not12)
	{
		if(	(nmid==BGBCC_SH_NMID_ADD)	||
			(nmid==BGBCC_SH_NMID_SUB)	||
			(nmid==BGBCC_SH_NMID_ADDSL)	||
			(nmid==BGBCC_SH_NMID_SUBSL)	||
			(nmid==BGBCC_SH_NMID_AND)	||
			(nmid==BGBCC_SH_NMID_OR)	||
			(nmid==BGBCC_SH_NMID_XOR)	||
			(nmid==BGBCC_SH_NMID_MULL)	||
			(nmid==BGBCC_SH_NMID_MULSL)	||
			(nmid==BGBCC_SH_NMID_MULSQ)	||
			(nmid==BGBCC_SH_NMID_MULUL)	||
			(nmid==BGBCC_SH_NMID_MULUQ)	||
			(nmid==BGBCC_SH_NMID_DIVSL)	||
			(nmid==BGBCC_SH_NMID_DIVSQ)	||
			(nmid==BGBCC_SH_NMID_DIVUL)	||
			(nmid==BGBCC_SH_NMID_DIVUQ)	||
			(nmid==BGBCC_SH_NMID_MODSL)	||
			(nmid==BGBCC_SH_NMID_MODSQ)	||
			(nmid==BGBCC_SH_NMID_MODUL)	||
			(nmid==BGBCC_SH_NMID_MODUQ)	 ||
			(nmid==BGBCC_SH_NMID_DMULS) ||
			(nmid==BGBCC_SH_NMID_DMULU) ||

			(nmid==BGBCC_SH_NMID_CMPEQ) ||
			(nmid==BGBCC_SH_NMID_CMPQEQ) ||
	//		(nmid==BGBCC_SH_NMID_CMPNE) ||
			(nmid==BGBCC_SH_NMID_CMPQNE) ||
			(nmid==BGBCC_SH_NMID_CMPGT) ||
			(nmid==BGBCC_SH_NMID_CMPQGT) ||
			(nmid==BGBCC_SH_NMID_CMPGE) ||
			(nmid==BGBCC_SH_NMID_CMPQGE) ||
			(nmid==BGBCC_SH_NMID_CMPQLT) ||
			(nmid==BGBCC_SH_NMID_CMPQLE) ||
			(nmid==BGBCC_SH_NMID_TST) ||
			(nmid==BGBCC_SH_NMID_NTST)
			)
		{
			if(ctx->emit_isprobe)
				return(0);

			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
			BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				nmid, rm, BGBCC_SH_REG_R5, rn);
			return(1);
		}
	}


	nm1=-1;
	if(nmid==BGBCC_SH_NMID_RSUB)
		nm1=BGBCC_SH_NMID_SUB;
	if(nmid==BGBCC_SH_NMID_RSUBSL)
		nm1=BGBCC_SH_NMID_SUBSL;
	if(nmid==BGBCC_SH_NMID_RSUBUL)
		nm1=BGBCC_SH_NMID_SUBUL;

	if(nm1>0)
	{
		if(ctx->emit_isprobe)
			return(0);

		BGBCC_JX2RV_EmitOpImmReg(ctx,
			BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
		BGBCC_JX2RV_EmitOpRegRegReg(ctx,
			nm1, BGBCC_SH_REG_R5, rm, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_ADDUL)
	{
		if(ctx->emit_isprobe)
			return(0);

		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADDSL, rm, imm, rn);
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHLDQ, rn, 32, rn);
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHLDQ, rn, -32, rn);
		return(1);
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00001013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_XOR:
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00004013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_SHARQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x40005013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_SHLRQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00005013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_OR:
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00006013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_AND:
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00007013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;


	case BGBCC_SH_NMID_ADDSL:
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000001B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

#if 0
	case BGBCC_SH_NMID_ADDUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000001B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		opw2=0x02001013|((rn&31)<<7)|((rn&31)<<15);
		opw3=0x02005013|((rn&31)<<7)|((rn&31)<<15);
		break;
#endif

	case BGBCC_SH_NMID_SHAD:
	case BGBCC_SH_NMID_SHLD:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000101B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_SHAR:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000501B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_SHLR:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000501B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_JSR:
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000067|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegImmReg(ctx, nmid, rm, imm, rn);
	
		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, int disp)
{
	s64 opw1, opw2, opw3, opw4;
	u32 dispm;
	int odr, ex, ex2, nowxi, exw, rt;
	int i, j, k;

	if((((s32)(disp<<20))>>20)!=disp)
	{
		if(ctx->emit_isprobe)
			return(0);

		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, disp, rt);
		i=BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rm, rt, 0);
		return(i);
	}

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	dispm=((disp&0xFE0)<<20)|((disp&31)<<7);

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00001023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00003023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;

	case BGBCC_SH_NMID_MOVX2:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00007023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	}



	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	
		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	s64 opw1, opw2, opw3, opw4, imm12;
	int odr, ex, ex2, nowxi, exw, rt;
	int i, j, k;

	if(	(nmid==BGBCC_SH_NMID_LEAB) ||
		(nmid==BGBCC_SH_NMID_LEAW) ||
		(nmid==BGBCC_SH_NMID_LEAL) ||
		(nmid==BGBCC_SH_NMID_LEAQ))
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, disp, rn));
	}

	if((((s32)(disp<<20))>>20)!=disp)
	{
		if(ctx->emit_isprobe)
			return(0);
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, disp, rt);
		i=BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx, nmid, rt, 0, rn);
		return(i);
	}

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	imm12=disp&4095;

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00001003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00003003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00004003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00005003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00006003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;

	case BGBCC_SH_NMID_MOVX2:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00007003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	return(BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, 0));
}

int BGBCC_JX2RV_TryEmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	return(BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx, nmid, rm, 0, rn));
}

int BGBCC_JX2RV_TryEmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty;
	int i, j, k;

	reg=BGBCC_JX2RV_NormalizeReg(ctx, reg);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BSR:
//		opw1=0x00000003|((reg&31)<<7)|((rm&31)<<15)|(disp<<20);
//		opw1=0xFFDFF06F|((reg&31)<<7);
		opw1=0x0000006F|((reg&31)<<7);
		rlty=BGBCC_SH_RLC_RELW20_RVI;
		break;

	case BGBCC_SH_NMID_MOV:
	case BGBCC_SH_NMID_LEAB:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00000013|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}
		
			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
//			opw3=0x00000033|((reg&31)<<7)|((reg&31)<<15)|(3<<20);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00000003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00000003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00000003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00004003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00004003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00004003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00001003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00001003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00001003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00005003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00005003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00005003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00002003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00002003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00002003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVUL:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00006003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00006003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00006003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00003003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00003003|((reg&31)<<7)|((reg&31)<<15);
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00003003|((reg&31)<<7)|((reg&31)<<15);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpLblReg(ctx, nmid, lbl, reg);

		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		if(opw4>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty, rt;
	int i, j, k;

	reg=BGBCC_JX2RV_NormalizeReg(ctx, reg);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;

	if(	(nmid==BGBCC_SH_NMID_BREQ) ||
		(nmid==BGBCC_SH_NMID_BRNE) ||
		(nmid==BGBCC_SH_NMID_BRLT) ||
		(nmid==BGBCC_SH_NMID_BRGE) ||
		(nmid==BGBCC_SH_NMID_BRGT) ||
		(nmid==BGBCC_SH_NMID_BRLE) ||
		(nmid==BGBCC_SH_NMID_BRLTU) ||
		(nmid==BGBCC_SH_NMID_BRGEU) ||
		(nmid==BGBCC_SH_NMID_BRGTU) ||
		(nmid==BGBCC_SH_NMID_BRLEU) ||
		(nmid==BGBCC_SH_NMID_BREQL) ||
		(nmid==BGBCC_SH_NMID_BRNEL) ||
		(nmid==BGBCC_SH_NMID_BRLTL) ||
		(nmid==BGBCC_SH_NMID_BRGEL) ||
		(nmid==BGBCC_SH_NMID_BRGTL) ||
		(nmid==BGBCC_SH_NMID_BRLEL) ||
		(nmid==BGBCC_SH_NMID_BRLTUL) ||
		(nmid==BGBCC_SH_NMID_BRGEUL) ||
		(nmid==BGBCC_SH_NMID_BRGTUL) ||
		(nmid==BGBCC_SH_NMID_BRLEUL))
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegLbl(ctx,
			nmid, BGBCC_SH_REG_ZZR, reg, lbl));
	}

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if((reg&31)==5)
			break;

		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00000023|(3<<15)|((reg&31)<<20)|
					((k&0xFE0)<<20)|((k&0x01F)<<7);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
			opw3=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw4=0x00000023|((rt&31)<<15)|((reg&31)<<20);
			break;
		}
		if(ctx->is_pbo)
			break;
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		rt=BGBCC_SH_REG_RQ5;
		opw1=0x00000017|((rt&31)<<7);
		opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
		opw3=0x00000023|((rt&31)<<15)|((reg&31)<<20);
		break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if((reg&31)==5)
			break;

		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00001023|(3<<15)|((reg&31)<<20)|
					((k&0xFE0)<<20)|((k&0x01F)<<7);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
			opw3=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw4=0x00001023|((rt&31)<<15)|((reg&31)<<20);
			break;
		}
		if(ctx->is_pbo)
			break;
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		rt=BGBCC_SH_REG_RQ5;
		opw1=0x00000017|((rt&31)<<7);
		opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
		opw3=0x00001023|((rt&31)<<15)|((reg&31)<<20);
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if((reg&31)==5)
			break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00002023|(3<<15)|((reg&31)<<20)|
					((k&0xFE0)<<20)|((k&0x01F)<<7);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
			opw3=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw4=0x00002023|((rt&31)<<15)|((reg&31)<<20);
			break;
		}
		if(ctx->is_pbo)
			break;
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		rt=BGBCC_SH_REG_RQ5;
		opw1=0x00000017|((rt&31)<<7);
		opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
		opw3=0x00002023|((rt&31)<<15)|((reg&31)<<20);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if((reg&31)==5)
			break;
		if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			break;
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00003023|(3<<15)|((reg&31)<<20)|
					((k&0xFE0)<<20)|((k&0x01F)<<7);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
			opw3=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw4=0x00003023|((rt&31)<<15)|((reg&31)<<20);
			break;
		}
		if(ctx->is_pbo)
			break;
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		rt=BGBCC_SH_REG_RQ5;
		opw1=0x00000017|((rt&31)<<7);
		opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
		opw3=0x00003023|((rt&31)<<15)|((reg&31)<<20);
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegLbl(ctx, nmid, lbl, reg);

		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);
		if(opw4>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int lbl)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty, rlty2, rlty3, rt;
	int i, j, k;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;
	rlty2=-1;
	rlty3=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BREQ:
	case BGBCC_SH_NMID_BREQL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00001463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

//		opw1=0x00000063|((rn&31)<<15)|((rm&31)<<20);
		opw1=0x00000063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRNE:
	case BGBCC_SH_NMID_BRNEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00000463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

//		opw1=0xFE001EE3|((rn&31)<<15)|((rm&31)<<20);
		opw1=0x00001063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;


	case BGBCC_SH_NMID_BRLT:
	case BGBCC_SH_NMID_BRLTL:
//	case BGBCC_SH_NMID_BRGT:
//	case BGBCC_SH_NMID_BRGTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00005463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00004063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGT:
	case BGBCC_SH_NMID_BRGTL:
//	case BGBCC_SH_NMID_BRLT:
//	case BGBCC_SH_NMID_BRLTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00005463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00004063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;


	case BGBCC_SH_NMID_BRLE:
	case BGBCC_SH_NMID_BRLEL:
//	case BGBCC_SH_NMID_BRGE:
//	case BGBCC_SH_NMID_BRGEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00004463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00005063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGE:
	case BGBCC_SH_NMID_BRGEL:
//	case BGBCC_SH_NMID_BRLE:
//	case BGBCC_SH_NMID_BRLEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00004463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00005063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRLTU:
	case BGBCC_SH_NMID_BRLTUL:
//	case BGBCC_SH_NMID_BRGTU:
//	case BGBCC_SH_NMID_BRGTUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00007463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00006063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGTU:
	case BGBCC_SH_NMID_BRGTUL:
//	case BGBCC_SH_NMID_BRLTU:
//	case BGBCC_SH_NMID_BRLTUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00007463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00006063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGEU:
	case BGBCC_SH_NMID_BRGEUL:
//	case BGBCC_SH_NMID_BRLEU:
//	case BGBCC_SH_NMID_BRLEUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00006463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00007063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRLEU:
	case BGBCC_SH_NMID_BRLEUL:
//	case BGBCC_SH_NMID_BRGEU:
//	case BGBCC_SH_NMID_BRGEUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00006463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00007063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
		
	case BGBCC_SH_NMID_BRTSTQ:
	case BGBCC_SH_NMID_BRTSTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		rt=BGBCC_SH_REG_R5;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00007033|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
			opw2=0x00001463|((rt&31)<<15)|(0<<20);
			opw3=0x0000006F;
			rlty3=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00007033|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
		opw2=0x00000063|((rt&31)<<15)|(0<<20);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRTSTNQ:
	case BGBCC_SH_NMID_BRTSTNL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		rt=BGBCC_SH_REG_R5;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
			opw1=0x00007033|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
			opw2=0x00000463|((rt&31)<<15)|(0<<20);
			opw3=0x0000006F;
			rlty3=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00007033|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
		opw2=0x00001063|((rt&31)<<15)|(0<<20);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegRegLbl(ctx, nmid, rm, rn, lbl);

		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }

		BGBCC_JX2_EmitOpDWord(ctx, opw1);

		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);

		if(rlty3>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty3); }
		if(opw3>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw3);

		if(opw4>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	if(nmid==BGBCC_SH_NMID_BRA)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_ZZR));

	if(nmid==BGBCC_SH_NMID_BSR)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_LR));

	if(nmid==BGBCC_SH_NMID_BRAN)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_ZZR));

	if(nmid==BGBCC_SH_NMID_BSRN)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_LR));

	return(0);
}

int BGBCC_JX2RV_TryEmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	s64 opw1, opw2, opw3, reg2, ex, ex2, exw;

	if(nmid==BGBCC_SH_NMID_RTS)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, BGBCC_SH_REG_LR, 0, BGBCC_SH_REG_ZZR));

	if(nmid==BGBCC_SH_NMID_NOP)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, BGBCC_SH_REG_ZZR, 0, BGBCC_SH_REG_ZZR));

	opw1=-1; opw2=-1;

	if(nmid==BGBCC_SH_NMID_SYSCALL)
	{
		opw1=0x00000073;
	}

	if(nmid==BGBCC_SH_NMID_BRK)
	{
		opw1=0x00100073;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpNone(ctx, nmid);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	s64 opw1, opw2, opw3, reg2, ex, ex2, exw;
	int i;

	if(nmid==BGBCC_SH_NMID_JSR)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, 0, BGBCC_SH_REG_LR));
	if(nmid==BGBCC_SH_NMID_JMP)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, 0, BGBCC_SH_REG_ZZR));

	return(0);
}

int BGBCC_JX2RV_TryEmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	s64 opw1, opw2, odr;

	opw1=-1;
	opw2=-1;

	if(nmid==BGBCC_SH_NMID_BRK)
	{
		opw1=0x00100073;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpImm(ctx, nmid, imm);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_EmitOpRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i;
	if(rm==0xFF)
		{ i=-1; }

	if(!BGBCC_JX2RV_TryEmitOpRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	s64 opw1, opw2, ex, ex3r, exw;
	int rm3, rn3, rm1, rn1;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	if(
		(nmid==BGBCC_SH_NMID_ADD)	||
		(nmid==BGBCC_SH_NMID_SUB)	||
		(nmid==BGBCC_SH_NMID_ADDSL)	||
		(nmid==BGBCC_SH_NMID_SUBSL)	||
		(nmid==BGBCC_SH_NMID_ADDUL)	||
		(nmid==BGBCC_SH_NMID_SUBUL)	||
		(nmid==BGBCC_SH_NMID_AND)	||
		(nmid==BGBCC_SH_NMID_OR)	||
		(nmid==BGBCC_SH_NMID_XOR)	)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nmid, rn, rm, rn));
	}

	if(
		(nmid==BGBCC_SH_NMID_MULSL) ||
		(nmid==BGBCC_SH_NMID_MULUL) ||
		(nmid==BGBCC_SH_NMID_MULSQ) ||
		(nmid==BGBCC_SH_NMID_MULUQ) ||
		(nmid==BGBCC_SH_NMID_DIVSL) ||
		(nmid==BGBCC_SH_NMID_DIVUL) ||
		(nmid==BGBCC_SH_NMID_DIVSQ) ||
		(nmid==BGBCC_SH_NMID_DIVUQ) ||
		(nmid==BGBCC_SH_NMID_MODSL) ||
		(nmid==BGBCC_SH_NMID_MODUL) ||
		(nmid==BGBCC_SH_NMID_MODSQ) ||
		(nmid==BGBCC_SH_NMID_MODUQ) ||

		(nmid==BGBCC_SH_NMID_FADD) ||
		(nmid==BGBCC_SH_NMID_FSUB) ||
		(nmid==BGBCC_SH_NMID_FMUL) ||
		(nmid==BGBCC_SH_NMID_FDIV)
		)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nmid,
			rn, rm, rn));
	}

	if(nmid==BGBCC_SH_NMID_MOV)
	{
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_ADD, rm, 0, rn));
		}

		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))
		{
			return(BGBCC_JX2RV_TryEmitOpRegReg(ctx,
				BGBCC_SH_NMID_FMOVID, rm, rn));
		}

		if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			return(BGBCC_JX2RV_TryEmitOpRegReg(ctx,
				BGBCC_SH_NMID_FMOVDI, rm, rn));
		}

		if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))
		{
			return(BGBCC_JX2RV_TryEmitOpRegReg(ctx,
				BGBCC_SH_NMID_FMOV, rm, rn));
		}
	}

	if(nmid==BGBCC_SH_NMID_XMOV)
	{
		rm3=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		rn3=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);

		BGBCC_JX2RV_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, rm3+0, rn3+0);
		BGBCC_JX2RV_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, rm3+1, rn3+1);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_FABS) ||
		(nmid==BGBCC_SH_NMID_FNEG) ||
		(nmid==BGBCC_SH_NMID_FLDCI) ||
		(nmid==BGBCC_SH_NMID_FSTCI) )
	{
		rm1=rm;
		rn1=rn;

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))
			rm1=BGBCC_SH_REG_R32;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))
			rn1=BGBCC_SH_REG_R33;
		
		if(nmid==BGBCC_SH_NMID_FLDCI)
		{
			if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))
				rm1=rm;
		}
		if(nmid==BGBCC_SH_NMID_FSTCI)
		{
			if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
				rn1=rn;
		}

		if((rm!=rm1) || (rn!=rn1))
		{
			if(rm!=rm1)
			{
				BGBCC_JX2RV_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_MOV, rm, rm1);
			}

			BGBCC_JX2RV_EmitOpRegReg(ctx,
				nmid, rm1, rn1);

			if(rn!=rn1)
			{
				BGBCC_JX2RV_EmitOpRegReg(ctx,
					BGBCC_SH_NMID_MOV, rn1, rn);
			}

			return(1);
		}
	}


	if(nmid==BGBCC_SH_NMID_EXTSL)
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_ADDSL,
			rm, 0, rn));
	}

	if(nmid==BGBCC_SH_NMID_EXTUL)
	{
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
			rm, 32, rn);
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
			rn, -32, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_EXTSW)
	{
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHADQ,
			rm, 48, rn);
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHADQ,
			rn, -48, rn);
		return(1);
	}
	if(nmid==BGBCC_SH_NMID_EXTUW)
	{
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
			rm, 48, rn);
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
			rn, -48, rn);
		return(1);
	}


	if(nmid==BGBCC_SH_NMID_EXTSB)
	{
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHADQ,
			rm, 56, rn);
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHADQ,
			rn, -56, rn);
		return(1);
	}
	if(nmid==BGBCC_SH_NMID_EXTUB)
	{
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
			rm, 56, rn);
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
			rn, -56, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_TRAPA)
	{
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_NEG)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, BGBCC_SH_NMID_SUB,
			BGBCC_SH_REG_R0, rm, rn));
	}

	if(nmid==BGBCC_SH_NMID_NOT)
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_XOR,
			rm, -1, rn));
	}

	opw1=-1;
	opw2=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_FLDCI:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0xC2200053U|((rm&31)<<15)|((rn&31)<<7);
		break;
	case BGBCC_SH_NMID_FSTCI:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0xD2200053U|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FLDCF:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x40100053U|((rm&31)<<15)|((rn&31)<<7);
		break;
	case BGBCC_SH_NMID_FSTCF:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x42000053U|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FNEG:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x22001053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
		break;
	case BGBCC_SH_NMID_FABS:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x22002053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
		break;
	
	case BGBCC_SH_NMID_FMOV:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x22000053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FMOVID:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0xF2000053U|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FMOVDI:
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0xE2000053U|((rm&31)<<15)|((rn&31)<<7);
		break;
	
	
	default:
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegReg(ctx, nmid, rm, rn);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_EmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	if(!BGBCC_JX2RV_TryEmitOpImmReg(ctx, nmid, imm, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int reg)
{
	s64 imm1, imm2, imm1a, imm2a;
	s64 opw1, opw2;

	if(
		(nmid==BGBCC_SH_NMID_ADD)	||
		(nmid==BGBCC_SH_NMID_SUB)	||
		(nmid==BGBCC_SH_NMID_ADDSL)	||
		(nmid==BGBCC_SH_NMID_SUBSL)	||
		(nmid==BGBCC_SH_NMID_ADDUL)	||
		(nmid==BGBCC_SH_NMID_SUBUL)	||
		(nmid==BGBCC_SH_NMID_AND)	||
		(nmid==BGBCC_SH_NMID_OR)	||
		(nmid==BGBCC_SH_NMID_XOR)	||
		(nmid==BGBCC_SH_NMID_SHAD)	||
		(nmid==BGBCC_SH_NMID_SHLD)	||
		(nmid==BGBCC_SH_NMID_SHADQ)	||
		(nmid==BGBCC_SH_NMID_SHLDQ)	||
		(nmid==BGBCC_SH_NMID_SHAR)	||
		(nmid==BGBCC_SH_NMID_SHLR)	||
		(nmid==BGBCC_SH_NMID_SHARQ)	||
		(nmid==BGBCC_SH_NMID_SHLRQ)	)
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, nmid, reg, imm, reg));
	}

	if(nmid==BGBCC_SH_NMID_MOV)
	{
		if(imm==(((s32)(imm<<20))>>20))
		{
			return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_ZZR, imm, reg));
		}
		
//		if(imm==((s32)imm))
		if((imm==((s32)imm)) || (imm==((u32)imm)))
		{
			if(ctx->emit_isprobe)
				return(0);

//			imm1=imm>>12;

//			imm1=((s32)(imm+2048))>>12;
//			imm2=imm-(imm1<<12);
//			imm2=((s32)(imm2<<20))>>20;

			imm2=((s32)(imm<<20))>>20;
			imm1=((s32)(imm-imm2))>>12;

			if(((imm1<<12)+imm2)==imm)
			{
				BGBCC_JX2RV_EmitOpImmReg(ctx,
					BGBCC_SH_NMID_LUI, imm1, reg);
				if(imm2!=0)
				{
					BGBCC_JX2RV_EmitOpRegImmReg(ctx,
						BGBCC_SH_NMID_ADD, reg, imm2, reg);
				}
				return(1);
			}

			imm2=((s32)(imm<<20))>>20;
			imm1=((s32)(imm^imm2))>>12;

			if(((imm1<<12)^imm2)==imm)
			{
				BGBCC_JX2RV_EmitOpImmReg(ctx,
					BGBCC_SH_NMID_LUI, imm1, reg);
				if(imm2!=0)
				{
					BGBCC_JX2RV_EmitOpRegImmReg(ctx,
						BGBCC_SH_NMID_XOR, reg, imm2, reg);
				}
				return(1);
			}
			
			imm1=-1;
		}

//		if(imm==((s32)imm))
//		if(1)
		if((reg&31)!=5)
		{
			if(ctx->emit_isprobe)
				return(0);

//			imm1a=imm>>32;
//			imm1a=(imm+(1LL<<31))>>32;
//			imm2a=imm-(imm1a<<32);

			imm2a=(s32)imm;
			imm1a=(imm-imm2a)>>32;

			if(((imm1a<<32)+imm2a)!=imm)
				{ BGBCC_DBGBREAK }

			if(!imm2a)
			{
				BGBCC_JX2RV_EmitOpImmReg(ctx,
					BGBCC_SH_NMID_MOV, imm1a, reg);
				BGBCC_JX2RV_EmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, reg, 32, reg);
				return(1);
			}

			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, imm1a, BGBCC_SH_REG_R5);
			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, imm2a, reg);
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_SHADQ, BGBCC_SH_REG_R5, 32, BGBCC_SH_REG_R5);
			BGBCC_JX2RV_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_ADD, reg, BGBCC_SH_REG_R5, reg);
				
//			imm1=(imm2a+2048)>>12;
//			imm2=imm2a-(imm1<<12);
		
//			BGBCC_JX2RV_TryEmitOpImmReg(ctx, BGBCC_SH_NMID_LUI, imm1, reg);
//			BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
//				BGBCC_SH_NMID_ADD, reg, imm2, reg);
			return(1);
		}
		
		if(1)
		{
			if(ctx->emit_isprobe)
				return(0);

			imm2=((s32)(imm<<20))>>20;
			imm1=(imm-imm2)>>12;

			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, imm1, reg);
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_SHADQ, reg, 12, reg);
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_ADD, reg, imm2, reg);
			return(1);
		}
	
	}

	if(nmid==BGBCC_SH_NMID_FLDCH)
	{
		imm1=BGBCC_ConstConvHalfToDouble(imm);
		return(BGBCC_JX2RV_TryEmitOpImmReg(ctx,
			BGBCC_SH_NMID_MOV, imm1, reg));
	}

	if(nmid==BGBCC_SH_NMID_JSR)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, imm, BGBCC_SH_REG_LR));
	if(nmid==BGBCC_SH_NMID_JMP)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, reg, imm, BGBCC_SH_REG_ZZR));

	opw1=-1; opw2=-1;
	imm1=imm&0xFFFFF;

	if(nmid==BGBCC_SH_NMID_LUI)
	{
		opw1=0x00000037|((reg&31)<<7)|(imm1<<12);
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpImmReg(ctx, nmid, imm, reg);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegRegRegReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int rp, int rn)
{
	s64 opw1, opw2, opw3, opw4;
	int i, ex, exw;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegRegImmReg(
	BGBCC_JX2_Context *ctx, int nmid, int rs, int rt, int imm, int rn)
{
	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStReg2(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3, opw4, ex, exw, rt;

	if(	(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVUB))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, ro, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, 0);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 1, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, rt, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, 0);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVUL))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 2, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, rt, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, 0);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 3, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, rt, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, 0);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpLdReg2Reg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3, opw4, ex, rt, exw;
	
	if(nmid==BGBCC_SH_NMID_LEAB)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, ro, rn));
	}

	if(nmid==BGBCC_SH_NMID_LEAW)
	{
		if((rm&31)==5)
			return(0);
	
//		rt=BGBCC_SH_REG_R5;
		rt=rn;
		if(rm==rn)
			rt=BGBCC_SH_REG_R5;

		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 1, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_LEAL)
	{
		if((rm&31)==5)
			return(0);
	
//		rt=BGBCC_SH_REG_R5;
		rt=rn;
		if(rm==rn)
			rt=BGBCC_SH_REG_R5;

		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 2, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_LEAQ)
	{
		if((rm&31)==5)
			return(0);
	
//		rt=BGBCC_SH_REG_R5;
		rt=rn;
		if(rm==rn)
			rt=BGBCC_SH_REG_R5;

		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 3, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVUB))
	{
		if((rm&31)==5)
			return(0);
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, ro, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, 0, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
	{
		if((rm&31)==5)
			return(0);
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 1, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, 0, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVUL))
	{
		if((rm&31)==5)
			return(0);
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 2, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, 0, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVQ))
	{
		if((rm&31)==5)
			return(0);
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 3, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, 0, rn);
		return(1);
	}

	return(0);
}


int BGBCC_JX2RV_TryEmitOpLdReg2DispReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	s64 opw1, opw2, opw3, opw4, ex, rt, exw;
	
	if(nmid==BGBCC_SH_NMID_LEAB)
	{
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, ro, rn);
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, disp, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_LEAW)
	{
		if((rm&31)==5)
			return(0);
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 1, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rn);
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, disp, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_LEAL)
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 2, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rn);
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, disp, rn);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_LEAQ)
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 3, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rn);
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, disp, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVUB))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, ro, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, disp, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 1, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, disp, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVUL))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 2, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, disp, rn);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVQ))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 3, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, rt, rt);
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rt, disp, rn);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStReg2Disp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	s64 opw1, opw2, opw3, opw4, ex, exw, rt;

	if(	(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVUB))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, ro, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, disp);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 1, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, rt, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, disp);
		return(1);
	}

	if(	(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVUL))
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 2, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, rt, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, disp);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
	{
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHADQ, ro, 3, rt);
		BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rn, rt, rt);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rt, disp);
		return(1);
	}

	return(0);
}
