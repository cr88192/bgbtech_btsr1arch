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

#define BGBCC_USE_RELW23_XG3

int BGBCC_JX2RV_NormalizeReg(BGBCC_JX2_Context *ctx, int reg)
{
	int tr0;

	if(reg==BGBCC_SH_REG_ZZR)
		return(BGBCC_SH_REG_R0);
	if(reg==BGBCC_SH_REG_LR)
		return(BGBCC_SH_REG_R1);
	if(reg==BGBCC_SH_REG_SP)
		return(BGBCC_SH_REG_R2);
	if(reg==BGBCC_SH_REG_GBR)
		return(BGBCC_SH_REG_R3);
	if(reg==BGBCC_SH_REG_TBR)
		return(BGBCC_SH_REG_R4);
	if(reg==BGBCC_SH_REG_DHR)
		return(BGBCC_SH_REG_R5);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR16) )
	{
		tr0=BGBCC_SH_REG_RQ0+(reg&0x1E);
		if(reg&1)
			tr0+=32;
		return(tr0);
	}

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

int BGBCC_JX2RV_NormalizeRegAsCsr(BGBCC_JX2_Context *ctx, int reg)
{
	int creg, csr;

	csr=-1;

	creg=BGBCC_JX2_TryMapRegCn(ctx, reg);
	if(creg>0)
	{
		csr=0x7C0+(creg&0x3F);
	}

	return(csr);
}

int BGBCC_JX2RV_CheckRepackRVC(
	BGBCC_JX2_Context *ctx,
	s64 *ropw1, s64 *ropw2)
{
	s64 opw1, opw2, opw3;
	int rm, rn, ro, rm3, rn3, ro3, imm12, fn7, m_i6alu;
	int disp12ld, disp12st, m_i6lq, m_i6sq, m_i5lq, m_i5sq;
	int m_i6lw, m_i6sw, m_i5lw, m_i5sw;

	if(ctx->is_fixed32&1)
		return(0);

	if(!(ctx->emit_riscv&0x01))
		return(0);
//	if(ctx->emit_riscv&0x02)
//		return(0);

	opw1=*ropw1;
	opw2=*ropw2;
	
	if(opw2>0)
		return(0);
	
	rn=(opw1>> 7)&31;
	rm=(opw1>>15)&31;
	ro=(opw1>>20)&31;
	imm12=(((s32)opw1)>>20);
	fn7=(opw1>>25)&127;

	disp12ld=imm12;
	disp12st=(imm12&(~31))|rn;

	m_i6lq=-1;	m_i6sq=-1;
	m_i5lq=-1;	m_i5sq=-1;
	m_i6lw=-1;	m_i6sw=-1;
	m_i5lw=-1;	m_i5sw=-1;

	if((disp12ld>=0) && (disp12ld<512) && !(disp12ld&7))
	{
		m_i6lq=
			(((disp12ld>>6)&7)<< 2) |
			(((disp12ld>>3)&3)<< 5) |
			(((disp12ld>>5)&1)<<12) ;
	}
	if((disp12st>=0) && (disp12st<512) && !(disp12st&7))
	{
		m_i6sq=
			(((disp12st>>6)&7)<< 7) |
			(((disp12st>>3)&7)<<10) ;
	}

	if((disp12ld>=0) && (disp12ld<256) && !(disp12ld&7))
	{
		m_i5lq=
			(((disp12ld>>6)&3)<< 5) |
			(((disp12ld>>3)&7)<<10) ;
	}
	if((disp12st>=0) && (disp12st<256) && !(disp12st&7))
	{
		m_i5sq=
			(((disp12st>>6)&3)<< 5) |
			(((disp12st>>3)&7)<<10) ;
	}


	if((disp12ld>=0) && (disp12ld<256) && !(disp12ld&3))
	{
		m_i6lw=
			(((disp12ld>>6)&3)<< 2) |
			(((disp12ld>>2)&7)<< 4) |
			(((disp12ld>>5)&1)<<12) ;
	}
	if((disp12st>=0) && (disp12st<256) && !(disp12st&3))
	{
		m_i6sw=
			(((disp12st>>6)& 3)<<7) |
			(((disp12st>>2)&15)<<9) ;
	}

	if((disp12ld>=0) && (disp12ld<128) && !(disp12ld&3))
	{
		m_i5lw=
			(((disp12ld>>2)&1)<< 6) |
			(((disp12ld>>6)&1)<< 5) |
			(((disp12ld>>3)&7)<<10) ;
	}
	if((disp12st>=0) && (disp12st<128) && !(disp12st&3))
	{
		m_i5sw=
			(((disp12st>>2)&1)<< 6) |
			(((disp12st>>6)&1)<< 5) |
			(((disp12st>>3)&7)<<10) ;
	}


	rn3=-1;
	rm3=-1;
	ro3=-1;
	if((rn>=8) && (rn<=15))
		rn3=rn-8;
	if((rm>=8) && (rm<=15))
		rm3=rm-8;
	if((ro>=8) && (ro<=15))
		ro3=ro-8;

	m_i6alu=-1;
	if((imm12>0) && (imm12<=31))
		m_i6alu=imm12<<2;
	if((imm12<0) && (imm12>=(-32)))
		m_i6alu=0x1000|((imm12&0x1F)<<2);

	opw3=-1;

	if((opw1&0x707F)==0x0013)	//ADDI 3RI
	{
		if((imm12==0) && (rn>0) && (rm>0))
		{	/* C.MV Rm5, Rn5 */
			opw3=0x8002|(rn<<7)|(rm<<2)|(0x0CULL<<56);
		}

#if 1
		if((opw3<0) && (rm==rn) && (rn!=2) && (m_i6alu>=0))
		{	/* C.ADDI Imm, Rn */
			opw3=0x0001|(rn<<7)|m_i6alu|(0x0CULL<<56);
		}

		if((opw3<0) && (rm==0) && (rn!=0) && (m_i6alu>=0))
		{	/* C.LI Imm, Rn */
			opw3=0x4001|(rn<<7)|m_i6alu|(0x0CULL<<56);
		}
#endif
	}

#if 1
	if((opw1&0x707F)==0x001B)
	{
		if((rm==rn) && (rn!=0) && (rn!=2) && (m_i6alu>=0))
		{	/* C.ADDW Imm. Rn */
			opw3=0x2001|(rn<<7)|m_i6alu|(0x0CULL<<56);
		}
	}
#endif

#if 1
	if((opw1&0x707F)==0x0033)
	{
		if((fn7==0x00) && (rm==rn))
		{	/* C.ADD Rm, Rn */
			opw3=0x9002|(rn<<7)|(ro<<2)|(0x0CULL<<56);
		}

		if((fn7==0x20) && (rm==rn) && (rn3>=0) && (ro3>=0))
		{	/* C.SUB Rm, Rn */
			opw3=0x8C01|(rn3<<7)|(ro3<<2)|(0x0CULL<<56);
		}
	}
#endif

	if((opw1&0x707F)==0x7013)	/* ANDI 3RI */
	{
		if((opw3<0) && (rm==rn) && (rn3>=0) && (m_i6alu>=0))
		{	/* C.ANDI Imm, Rn */
			opw3=0x8801|(rn3<<7)|m_i6alu|(0x0CULL<<56);
		}
	}

#if 1
	if((opw1&0x707F)==0x003B)	/* ADDW 3R */
	{
		if((fn7==0x00) && (rm==rn) && (rn3>=0) && (ro3>=0))
		{	/* C.ADDW Rm, Rn */
			opw3=0x9C21|(rn3<<7)|(ro3<<2)|(0x0CULL<<56);
		}

		if((fn7==0x20) && (rm==rn) && (rn3>=0) && (ro3>=0))
		{	/* C.SUBW Rm3, Rn3 */
			opw3=0x9C01|(rn3<<7)|(ro3<<2)|(0x0CULL<<56);
		}
	}
#endif

#if 1
	if((opw1&0x707F)==0x4033)	/* XOR 3R */
	{
		if((fn7==0x00) && (rm==rn) && (rn3>=0) && (ro3>=0))
		{	/* C.XOR Rm, Rn */
			opw3=0x8C21|(rn3<<7)|(ro3<<2)|(0x0CULL<<56);
		}
	}

	if((opw1&0x707F)==0x6033)	/* OR 3R */
	{
		if((fn7==0x00) && (rm==rn) && (rn3>=0) && (ro3>=0))
		{	/* C.OR Rm, Rn */
			opw3=0x8C41|(rn3<<7)|(ro3<<2)|(0x0CULL<<56);
		}
	}

	if((opw1&0x707F)==0x7033)	/* AND 3R */
	{
		if((fn7==0x00) && (rm==rn) && (rn3>=0) && (ro3>=0))
		{	/* C.AND Rm, Rn */
			opw3=0x8C61|(rn3<<7)|(ro3<<2)|(0x0CULL<<56);
		}
	}
#endif

	if((opw1&0x707F)==0x3003)	/* LD */
	{
		if((rm==2) && (rn>0) && (m_i6lq>=0))
		{	/* C.LDSP */
			opw3=0x6002|(rn<<7)|m_i6lq|(0x0CULL<<56);
		}

		if((rm3>=0) && (rn3>=0) && (m_i5lq>=0))
		{	/* C.LD */
			opw3=0x6000|(rn3<<2)|(rm3<<7)|m_i5lq|(0x0CULL<<56);
		}
	}

#if 1
	if((opw1&0x707F)==0x3023)	/* SD */
	{
		if((rm==2) && (ro>0) && (m_i6sq>=0))
		{	/* C.SDSP */
			opw3=0xE002|(ro<<2)|m_i6sq|(0x0CULL<<56);
		}

		if((rm3>=0) && (ro3>=0) && (m_i5sq>=0))
		{	/* C.SD */
			opw3=0xE000|(ro3<<2)|(rm3<<7)|m_i5sq|(0x0CULL<<56);
		}
	}
#endif


#if 1
	if((opw1&0x707F)==0x2003)	/* LW */
	{
		if((rm==2) && (rn>0) && (m_i6lw>=0))
		{	/* C.LWSP */
			opw3=0x4002|(rn<<7)|m_i6lw|(0x0CULL<<56);
		}

		if((rm3>=0) && (rn3>=0) && (m_i5lw>=0))
		{	/* C.LW */
			opw3=0x4000|(rn3<<2)|(rm3<<7)|m_i5lw|(0x0CULL<<56);
		}
	}
#endif

#if 1
	if((opw1&0x707F)==0x2023)	/* SW */
	{
		if((rm==2) && (ro>0) && (m_i6sw>=0))
		{	/* C.SWSP */
			opw3=0xC002|(ro<<2)|m_i6sw|(0x0CULL<<56);
		}

		if((rm3>=0) && (ro3>=0) && (m_i5sw>=0))
		{	/* C.SW */
			opw3=0xC000|(ro3<<2)|(rm3<<7)|m_i5sw|(0x0CULL<<56);
		}
	}
#endif

	if(opw3>0)
	{
		*ropw1=opw3;
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
	BGBCC_JX2_Context *ctx,
	int rm, s64 imm, int rn,
	s64 opwb, s64 *ropw1, s64 *ropw2)
{
	s64 imm12, imm12b, imm11j, imm5jj, imm5jk;
	s64 vimm12, vimm23, vimm33;
	int isimm12, isimm23, isimm33;

	imm12=imm&4095;
	imm12b=(imm&2047)|((imm>>63)&2048);
	vimm12=((s32)(imm<<20))>>20;
	isimm12=(vimm12==imm);

	vimm23=((s32)(imm<<9))>>9;
	isimm23=(vimm23==imm);

	vimm33=((s64)(imm<<31))>>31;
	isimm33=(vimm33==imm);

	imm11j=(imm>>11)&2047;
	imm5jj=(imm>>22)&31;
	imm5jk=(imm>>27)&31;

//	return(0);

	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isimm12)
	{
		*ropw1=opwb|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		return(1);
	}

	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isimm33)
	{
//		*ropw1=0x0000401BULL |
		*ropw1=0x0000403FULL |
			(imm5jk<< 7) |
			(imm5jj<<15) |
			(imm11j<<20) ;
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|(imm12b<<20);
		return(1);
	}

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		isimm23 )
	{
//		*ropw1=0x8000401BULL |
		*ropw1=0x8000403FULL |
			(((rn>>5)&1)<< 9) |
			(((rm>>5)&1)<<10) |
			(imm11j<<20) ;
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|(imm12b<<20);
		return(1);
	}


	return(0);
}

int BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg_J33Only(
	BGBCC_JX2_Context *ctx,
	int rm, s64 imm, int rn,
	s64 opwb, s64 *ropw1, s64 *ropw2)
{
	s64 imm12, imm12b, imm11j, imm5jj, imm5jk;
	s64 vimm12, vimm23, vimm33;
	int isimm12, isimm23, isimm33;

	imm12=imm&4095;
	imm12b=(imm&2047)|((imm>>63)&2048);
	vimm12=((s32)(imm<<20))>>20;
	isimm12=(vimm12==imm);

	vimm23=((s32)(imm<<9))>>9;
	isimm23=(vimm23==imm);

	vimm33=((s64)(imm<<31))>>31;
	isimm33=(vimm33==imm);

	imm11j=(imm>>11)&2047;
	imm5jj=(imm>>22)&31;
	imm5jk=(imm>>27)&31;

	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isimm33)
	{
//		*ropw1=0x0000401BULL |
		*ropw1=0x0000403FULL |
			(imm5jk<< 7) |
			(imm5jj<<15) |
			(imm11j<<20) ;
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|(imm12b<<20);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
	BGBCC_JX2_Context *ctx,
	int rm, int ro, int rn,
	s64 opwb, s64 *ropw1, s64 *ropw2)
{
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, ro) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
	{
		*ropw1=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) )
	{
//		*ropw1=0x8000401BULL |
		*ropw1=0x8000403FULL |
			(((rn>>5)&1)<< 9) |
			(((rm>>5)&1)<<10) |
			(((ro>>5)&1)<<11) ;
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEncJumboOpFRegFRegFReg(
	BGBCC_JX2_Context *ctx,
	int rm, int ro, int rn,
	s64 opwb, s64 *ropw1, s64 *ropw2)
{
	if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsFPR(ctx, ro) &&
		BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
	{
		*ropw1=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) )
	{
//		*ropw1=0x8000401BULL |
		*ropw1=0x8000403FULL |
			(((rn>>5)&1)<< 9) |
			(((rm>>5)&1)<<10) |
			(((ro>>5)&1)<<11) ;
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEncJumboOpFRegFRegXReg(
	BGBCC_JX2_Context *ctx,
	int rm, int ro, int rn,
	s64 opwb, s64 *ropw1, s64 *ropw2)
{
	if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsFPR(ctx, ro) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
	{
		*ropw1=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) )
	{
//		*ropw1=0x8000401BULL |
		*ropw1=0x8000403FULL |
			(((rn>>5)&1)<< 9) |
			(((rm>>5)&1)<<10) |
			(((ro>>5)&1)<<11) ;
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg(
	BGBCC_JX2_Context *ctx,
	int rm, s64 imm, int rn,
	s64 opwb, s64 *ropw1, s64 *ropw2)
{
	s64 imm12, imm12b, imm11j, imm5jj, imm5jk;
	s64 vimm12, vimm23, vimm33;
	int isimm12, isimm23, isimm33;

	imm12=imm&4095;
	imm12b=(imm&2047)|((imm>>63)&2048);
	vimm12=((s32)(imm<<20))>>20;
	isimm12=(vimm12==imm);

	vimm23=((s32)(imm<<9))>>9;
	isimm23=(vimm23==imm);

	vimm33=((s64)(imm<<31))>>31;
	isimm33=(vimm33==imm);

	imm11j=(imm>>11)&2047;
	imm5jj=(imm>>22)&31;
	imm5jk=(imm>>27)&31;

//	return(0);

	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isimm12)
	{
		*ropw1=opwb|((rn&31)<<20)|((rm&31)<<15)|
			((imm12&0xFE0)<<20)|
			((imm12&0x01F)<< 7);
		return(1);
	}

	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
		isimm33)
	{
//		*ropw1=0x0000401BULL |
		*ropw1=0x0000403FULL |
			(imm5jk<< 7) |
			(imm5jj<<15) |
			(imm11j<<20) ;
		*ropw2=opwb|((rn&31)<<20)|((rm&31)<<15)|
			((imm12&0xFE0)<<20)|
			((imm12&0x01F)<< 7);
		return(1);
	}

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		isimm23 )
	{
//		*ropw1=0x8000401BULL |
		*ropw1=0x8000403FULL |
			(((rn>>5)&1)<< 9) |
			(((rm>>5)&1)<<10) |
			(imm11j<<20) ;
		*ropw2=opwb|((rn&31)<<20)|((rm&31)<<15)|
			((imm12&0xFE0)<<20)|
			((imm12&0x01F)<< 7);
		return(1);
	}


	return(0);
}

int BGBCC_JX2RV_TryEncJumboSubOpXRegXRegXReg(
	BGBCC_JX2_Context *ctx,
	int rm, int ro, int rn,
	s64 opwb, int sub, s64 *ropw1, s64 *ropw2)
{
	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) )
	{
//		*ropw1=0x8000401BULL |
		*ropw1=0x8000403FULL |
			(((rn>>5)&1)<< 9) |
			(((rm>>5)&1)<<10) |
			(((ro>>5)&1)<<11) |
			((sub&15)<<16);
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|((ro&31)<<20);
		return(1);
	}

	return(0);
}


int BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
	BGBCC_JX2_Context *ctx,
	int rm, s64 iro, int rn,
	s64 opwb, int sub, s64 *ropw1, s64 *ropw2)
{
	if(!(ctx->has_jumbo&2))
		return(0);

	if(	BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		(((s16)iro)==iro) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) )
	{
		*ropw1=0x8000C03FULL |
			(((iro>> 5)&2047)<<20) |
			(((rn >> 5)&   1)<< 9) |
			(((rm >> 5)&   1)<<10) |
			(((iro>>16)&   1)<<11) |
			((sub&15)<<16);
		*ropw2=opwb|((rn&31)<<7)|((rm&31)<<15)|((iro&31)<<20);
		return(1);
	}

	return(0);
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

	if(nmid==BGBCC_SH_NMID_SUBP)
		nmid=BGBCC_SH_NMID_SUB;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	rs=BGBCC_JX2RV_NormalizeReg(ctx, rs);
	rt=BGBCC_JX2RV_NormalizeReg(ctx, rt);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	i=BGBCC_JX2X3_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn);
	if(i>0)
		return(i);

	if(nmid==BGBCC_SH_NMID_SHLD)
	{
		BGBCC_JX2RV_EmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_SHAD, rs, rt, rn);
		BGBCC_JX2RV_EmitOpRegReg(ctx,
			BGBCC_SH_NMID_EXTUL, rn, rn);
		return(1);
	}

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

		if(ctx->has_jumbo&2)
		{
			rs1=rs;
			rt1=rt;
			rn1=rn;
		}
		
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
		
		if(ctx->has_jumbo&2)
		{
			rs1=rs;
			rt1=rt;
			rn1=rn;
		}
		
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
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00000033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SUB:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x40000033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x40000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00001033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00001033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_XOR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00004033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00004033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_SHARQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x40005033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x40005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_SHLRQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00005033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_OR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00006033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00006033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_AND:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00007033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00007033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;


	case BGBCC_SH_NMID_ADDSL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0000003B, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x0000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_SUBSL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x4000003B, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

#if 0
	case BGBCC_SH_NMID_SUBUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;
#endif

	case BGBCC_SH_NMID_CMPQLT:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00002033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00002033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_CMPQGT:
	case BGBCC_SH_NMID_CMPGT:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rt, rs, rn, 0x00002033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00002033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		break;

	case BGBCC_SH_NMID_CMPGE:
	case BGBCC_SH_NMID_CMPQGE:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x40002033, &opw1, &opw2)>0)
					break;
		}
	
		if(BGBCC_JX2RV_TryEncJumboSubOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00002033, 2, &opw1, &opw2)>0)
				break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_CMPQLE:
//	case BGBCC_SH_NMID_CMPLE:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rt, rs, rn, 0x40002033, &opw1, &opw2)>0)
					break;
		}

		if(BGBCC_JX2RV_TryEncJumboSubOpXRegXRegXReg(
			ctx, rt, rs, rn, 0x00002033, 2, &opw1, &opw2)>0)
				break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		opw2=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_CMPQEQ:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x80002033, &opw1, &opw2)>0)
					break;
		}
		if(BGBCC_JX2RV_TryEncJumboSubOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00002033, 1, &opw1, &opw2)>0)
				break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x40000033|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		break;

#if 0
	case BGBCC_SH_NMID_CMPEQ:
//		if(BGBCC_JX2RV_TryEncJumboSubOpXRegXRegXReg(
//			ctx, rs, rt, rn, 0x00002033, 1, &opw1, &opw2)>0)
//				break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x4000003B|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		break;
#endif

	case BGBCC_SH_NMID_CMPQNE:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0xC0002033, &opw1, &opw2)>0)
					break;
		}
		if(BGBCC_JX2RV_TryEncJumboSubOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x00002033, 3, &opw1, &opw2)>0)
				break;
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

	case BGBCC_SH_NMID_CMPQHI:
	case BGBCC_SH_NMID_CMPHI:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rt, rs, rn, 0x00003033, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00003033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		break;

#if 1
	case BGBCC_SH_NMID_TST:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x80003033, &opw1, &opw2)>0)
					break;
		}
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		tr0=BGBCC_SH_REG_R5;
		opw1=0x00007033|((tr0&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x00103013|((rn&31)<<7)|((tr0&31)<<15);
		opw3=0x00104013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_NTST:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0xC0003033, &opw1, &opw2)>0)
					break;
		}
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
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegXReg(
			ctx, rs, rt, rn, 0xA2002053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0xA2002053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0xA2002053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_FCMPGT:
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegXReg(
			ctx, rs, rt, rn, 0xA2001053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0xA2001053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0xA2001053|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		opw1=0xA2001053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_FCMPGE:
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegXReg(
			ctx, rs, rt, rn, 0xA2000053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0xA2000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0xA2000053|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
		opw1=0xA2000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_ADDUL:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x0A00003B, &opw1, &opw2)>0)
					break;
		}

		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(ctx->has_rvzba&2)
		{
			opw1=0x0A00003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
			break;
		}

		if(ctx->has_rvzba&1)
//		if(0)
		{
			opw1=0x0000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
			opw2=0x0800003B|((rn&31)<<7)|((rn&31)<<15);
			break;
		}

		opw1=0x0000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x02001013|((rn&31)<<7)|((rn&31)<<15);
		opw3=0x02005013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_SUBUL:
		if(ctx->has_rvzba&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x4A00003B, &opw1, &opw2)>0)
					break;
		}

		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(ctx->has_rvzba&1)
		{
			opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
			opw2=0x0800003B|((rn&31)<<7)|((rn&31)<<15);
			break;
		}
		opw1=0x4000003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		opw2=0x02001013|((rn&31)<<7)|((rn&31)<<15);
		opw3=0x02005013|((rn&31)<<7)|((rn&31)<<15);
		break;

	case BGBCC_SH_NMID_SHAD:
//	case BGBCC_SH_NMID_SHLD:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0000103B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000103B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHAR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x4000503B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHLR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0000503B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MULSL:
		if(ctx->has_jumbo&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x0200203B, &opw1, &opw2)>0)
					break;
			if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
			if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
			if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
			opw1=0x0200203B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
			break;
		}

		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MULUL:
		if(ctx->has_jumbo&2)
		{
			if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
				ctx, rs, rt, rn, 0x0200303B, &opw1, &opw2)>0)
					break;
		}

		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(ctx->has_jumbo&2)
		{
			opw1=0x0200303B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
			break;
		}
		opw1=0x02000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
		break;

	case BGBCC_SH_NMID_MULSW:
	case BGBCC_SH_NMID_MULUW:
	case BGBCC_SH_NMID_DMULS:
	case BGBCC_SH_NMID_MULL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0200003B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200003B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MULQ:
	case BGBCC_SH_NMID_MULSQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x02000033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MULUQ:
	case BGBCC_SH_NMID_DMULU:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x02000033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		/* ??? */
		opw1=0x02000033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_DIVSL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0200403B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200403B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_DIVSQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x02004033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02004033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_DIVUL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0200503B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200503B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_DIVUQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x02005033, &opw1, &opw2)>0)
				break;

#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02005033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MODSL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0200603B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200603B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MODSQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x02006033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02006033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MODUL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x0200703B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0200703B|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_MODUQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rs, rt, rn, 0x02007033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x02007033|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_FADD:
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegFReg(
			ctx, rs, rt, rn, 0x02000053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0x02000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x02000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_FSUB:
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegFReg(
			ctx, rs, rt, rn, 0x0A000053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401B|
				opw1=0x8000403F|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0x0A000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x0A000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_FMUL:
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegFReg(
			ctx, rs, rt, rn, 0x12000053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401B|
				opw1=0x8000403F|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0x12000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x12000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;

	case BGBCC_SH_NMID_FDIV:
		if(BGBCC_JX2RV_TryEncJumboOpFRegFRegFReg(
			ctx, rs, rt, rn, 0x1A000053, &opw1, &opw2)>0)
				break;

#if 0
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401B|
				opw1=0x8000403F|
					(((rn>>5)&1)<< 9)|
					(((rs>>5)&1)<<10)|
					(((rt>>5)&1)<<11);
				opw2=0x1A000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
				break;
			}
		}

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x1A000053|((rn&31)<<7)|((rs&31)<<15)|((rt&31)<<20);
#endif
		break;


	case BGBCC_SH_NMID_LEAW:
		if(!(ctx->has_rvzba&1))
			break;
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rt, rs, rn, 0x20002033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x20002033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
#endif
		break;	

	case BGBCC_SH_NMID_LEAL:
		if(!(ctx->has_rvzba&1))
			break;
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rt, rs, rn, 0x20004033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x20004033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
#endif
		break;	

	case BGBCC_SH_NMID_LEAQ:
		if(!(ctx->has_rvzba&1))
			break;
		if(BGBCC_JX2RV_TryEncJumboOpXRegXRegXReg(
			ctx, rt, rs, rn, 0x20006033, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rs))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rt))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x20006033|((rn&31)<<7)|((rt&31)<<15)|((rs&31)<<20);
#endif
		break;	
	}

	if((opw1>0) && (opw3<0))
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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
	s64 opw1, opw2, opw3, opw4, imm12, imm12s;
	int i, ex, exw, nm1, not12, not12io, has12;
	int imm1, imm2;

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

	if(nmid==BGBCC_SH_NMID_SHLD)
	{
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SHAD, rm, imm, rn);
		BGBCC_JX2RV_EmitOpRegReg(ctx,
			BGBCC_SH_NMID_EXTUL, rn, rn);
		return(1);
	}


	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	i=BGBCC_JX2X3_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn);
	if(i>0)
		return(i);

	not12=0;
	has12=0;
	not12io=0;

	imm12s=((s32)(imm<<20))>>20;
	imm12=imm&4095;
	if(imm12s!=imm)
	{
		not12=1;
		not12io=1;
	}

	if(ctx->has_jumbo&2)
		not12=0;

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
		(nmid==BGBCC_SH_NMID_NTST) ||
		(nmid==BGBCC_SH_NMID_LDSH32)
		)
	{
		not12=1;
	}

	if(	(nmid==BGBCC_SH_NMID_ADD)	||
		(nmid==BGBCC_SH_NMID_SUB)	||
		(nmid==BGBCC_SH_NMID_ADDSL)	||
		(nmid==BGBCC_SH_NMID_SUBSL)	||
		(nmid==BGBCC_SH_NMID_AND)	||
		(nmid==BGBCC_SH_NMID_OR)	||
		(nmid==BGBCC_SH_NMID_XOR)
	)
	{
		has12=1;
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
#if 0
			if((ctx->has_jumbo&2) && has12 && (imm12s!=imm) &&
				BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
				BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
			{
				if((imm==((s32)imm)) || (imm==((u32)imm)))
				{
					imm12=(imm&2047)|((imm>>63)&(~2047));
					BGBCC_JX2RV_EmitOpImm(ctx,
						BGBCC_SH_NMID_RVJ21I, imm>>11);
					BGBCC_JX2RV_EmitOpRegImmReg(ctx,
						nmid, rm, imm12, rn);
					return(1);
				}
			}
#endif
		
			if(ctx->emit_isprobe)
				return(0);

			if(imm==0)
			{
				return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
					nmid, rm, BGBCC_SH_REG_ZZR, rn));
			}else
			{
				BGBCC_JX2RV_EmitOpImmReg(ctx,
					BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
				BGBCC_JX2RV_EmitOpRegRegReg(ctx,
					nmid, rm, BGBCC_SH_REG_R5, rn);
				return(1);
			}
		}

#if 0
		if(nmid==BGBCC_SH_NMID_LDSH32)
		{
			imm12=(imm&2047)|((imm>>63)&(~2047));
			BGBCC_JX2RV_EmitOpImm(ctx,
				BGBCC_SH_NMID_RVJ21I, imm>>11);
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_LDSH16, rm, imm12, rn);
			return(1);
		}
#endif
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

		if(imm==0)
		{
			return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				nm1, BGBCC_SH_REG_ZZR, rm, rn));
		}else
		{
			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
			BGBCC_JX2RV_EmitOpRegRegReg(ctx,
				nm1, BGBCC_SH_REG_R5, rm, rn);
			return(1);
		}
	}

	if(nmid==BGBCC_SH_NMID_ADDUL)
	{
		if(ctx->emit_isprobe)
			return(0);

		if(ctx->has_rvzba&1)
		{
			if(not12io)
			{
				BGBCC_JX2RV_EmitOpImmReg(ctx,
					BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
				BGBCC_JX2RV_EmitOpRegRegReg(ctx,
					nmid, BGBCC_SH_REG_R5, rm, rn);
			}else
			{
				if(imm==0)
				{
					BGBCC_JX2RV_EmitOpRegRegReg(ctx,
						BGBCC_SH_NMID_ADDUL, rm, BGBCC_SH_REG_ZZR, rn);
				}else
#if 0
					if((ctx->has_jumbo&2) &&
						(ctx->has_rvzba&2) &&
						(((s16)imm)==imm) &&
						BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
						BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
				{
					imm1=((imm>>5)&2047)|(1<<11)|
						(((imm>>31)&1)<<(11+5+4));
					BGBCC_JX2RV_EmitOpImm(ctx,
						BGBCC_SH_NMID_RVJ21O, imm1);
					BGBCC_JX2RV_EmitOpRegRegReg(ctx,
						BGBCC_SH_NMID_ADDUL, rm, (imm&31), rn);
				}else
#endif
				{
					BGBCC_JX2RV_EmitOpRegImmReg(ctx,
						BGBCC_SH_NMID_ADDSL, rm, imm, rn);
					BGBCC_JX2RV_EmitOpRegReg(ctx,
						BGBCC_SH_NMID_EXTUL, rn, rn);
//					BGBCC_JX2RV_EmitOpRegRegReg(ctx,
//						BGBCC_SH_NMID_ADDUL, rn, BGBCC_SH_REG_ZZR, rn);
				}
			}
		}else
		{
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_ADDSL, rm, imm, rn);
			BGBCC_JX2RV_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_EXTUL, rn, rn);
		}
		return(1);
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00000013, &opw1, &opw2)>0)
				break;
#if 0
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHADQ:
	case BGBCC_SH_NMID_SHLDQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00001013, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00001013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_XOR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00004013, &opw1, &opw2)>0)
				break;
#if 0
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00004013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHARQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x40005013, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x40005013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHLRQ:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00005013, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00005013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_OR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00006013, &opw1, &opw2)>0)
				break;
#if 0
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00006013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_AND:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00007013, &opw1, &opw2)>0)
				break;
#if 0
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00007013|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;


	case BGBCC_SH_NMID_ADDSL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x0000001B, &opw1, &opw2)>0)
				break;
#if 0
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000001B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

#if 0
	case BGBCC_SH_NMID_ADDUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!(ctx->has_rvzba&1))
			break;
		opw1=0x0800001B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
		break;
#endif

	case BGBCC_SH_NMID_SHAD:
//	case BGBCC_SH_NMID_SHLD:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x0000101B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000101B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHAR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x4000501B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x4000501B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_SHLR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x0000501B, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000501B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_JSR:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x00000067, &opw1, &opw2)>0)
				break;
#if 0
		if(not12)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000067|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;
		
	case BGBCC_SH_NMID_LDSH16:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm, rn, 0x0000601B, &opw1, &opw2)>0)
				break;
#if 0
		if(not12io)
			break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x0000601B|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_LDSH32:
		BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg_J33Only(
			ctx, rm, imm, rn, 0x0000601B, &opw1, &opw2);
		break;

#if 0
		if(nmid==BGBCC_SH_NMID_LDSH32)
		{
			imm12=(imm&2047)|((imm>>63)&(~2047));
			BGBCC_JX2RV_EmitOpImm(ctx,
				BGBCC_SH_NMID_RVJ21I, imm>>11);
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_LDSH16, rm, imm12, rn);
			return(1);
		}
#endif

	case BGBCC_SH_NMID_CSRRW:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm12s, rn, 0x00001073, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CSRRS:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm12s, rn, 0x00002073, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CSRRC:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm12s, rn, 0x00003073, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CSRRWI:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm12s, rn, 0x00005073, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CSRRSI:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm12s, rn, 0x00006073, &opw1, &opw2)>0)
				break;
		break;
	case BGBCC_SH_NMID_CSRRCI:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, imm12s, rn, 0x00007073, &opw1, &opw2)>0)
				break;
		break;
	}

	if((opw1>0) && (opw3<0))
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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

int BGBCC_JX2RV_EmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, s64 disp)
{
	if(!BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, s64 disp)
{
	s64 opw1, opw2, opw3, opw4;
	u32 dispm;
	int odr, ex, ex2, nowxi, exw, rt, imm12, sc;
	int i, j, k;

	i=BGBCC_JX2X3_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	if(i>0)
		return(i);

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		if(((rm&63)<4) || ((rm&63)>=32) ||
			!(ctx->has_pushx2&1) || (rm&1))
		{
			if(ctx->emit_isprobe)
				return(0);

			i=BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
				BGBCC_SH_NMID_MOVQ, rm+0, rn, disp+0);
			if(i<=0)
				return(i);
			i=BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
				BGBCC_SH_NMID_MOVQ, rm+1, rn, disp+8);
			return(i);
		}
	}

	if((((s32)(disp<<20))>>20)!=disp)
	{
#if 0
//		if(ctx->has_jumbo&2)
		if((ctx->has_jumbo&2) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			if((disp==((s32)disp)) || (disp==((u32)disp)))
			{
				imm12=(disp&2047)|((disp>>63)&(~2047));
				BGBCC_JX2RV_EmitOpImm(ctx,
					BGBCC_SH_NMID_RVJ21I, disp>>11);
				BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
					nmid, rm, rn, imm12);
				return(1);
			}
		}
#endif
	
		if(ctx->emit_isprobe)
			return(0);


#if 1
		sc=-1;
		if(	(nmid==BGBCC_SH_NMID_MOVB) ||
			(nmid==BGBCC_SH_NMID_MOVUB) ||
			(nmid==BGBCC_SH_NMID_LEAB))
				sc=0;
		if(	(nmid==BGBCC_SH_NMID_MOVW) ||
			(nmid==BGBCC_SH_NMID_MOVUW) ||
			(nmid==BGBCC_SH_NMID_LEAW))
				sc=1;
		if(	(nmid==BGBCC_SH_NMID_MOVL) ||
			(nmid==BGBCC_SH_NMID_MOVUL) ||
			(nmid==BGBCC_SH_NMID_MOVDL) ||
			(nmid==BGBCC_SH_NMID_LEAL))
				sc=2;
		if(	(nmid==BGBCC_SH_NMID_MOVQ) ||
			(nmid==BGBCC_SH_NMID_LEAQ))
				sc=3;

		if((ctx->has_rvzba&16) && (sc>=0) &&
			((((s16)(disp>>sc))<<sc)==disp))
		{
			rt=BGBCC_SH_REG_R5;
			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, disp>>sc, rt);
			i=BGBCC_JX2RV_TryEmitOpRegStReg2(ctx, nmid, rm, rt, rn);
			return(i);
		}
#endif
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, disp, rt);
		i=BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rm, rt, 0);
		return(i);
	}

	if(nmid==BGBCC_SH_NMID_MOVC)
	{
		if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))
			nmid=BGBCC_SH_NMID_MOVQ;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;

	dispm=((disp&0xFE0)<<20)|((disp&31)<<7);

// BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
	case BGBCC_SH_NMID_MOVB_ST:
		if(BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg(
			ctx, rn, disp, rm, 0x00000023, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00000023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
	case BGBCC_SH_NMID_MOVW_ST:
		if(BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg(
			ctx, rn, disp, rm, 0x00001023, &opw1, &opw2)>0)
				break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00001023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
	case BGBCC_SH_NMID_MOVL_ST:
		if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
			(disp>-2047) && (disp<2047))
		{
			opw1=0x00002027|((rn&31)<<15)|((rm&31)<<20)|dispm;
			break;
		}

		if(BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg(
			ctx, rn, disp, rm, 0x00002023, &opw1, &opw2)>0)
				break;

#if 0
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			opw1=0x00002023|((rn&31)<<15)|((rm&31)<<20)|dispm;
			break;
		}

		if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			opw1=0x00002027|((rn&31)<<15)|((rm&31)<<20)|dispm;
			break;
		}
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00002023|((rn&31)<<15)|((rm&31)<<20)|dispm;
#endif
		break;

	case BGBCC_SH_NMID_MOVQ:
	case BGBCC_SH_NMID_MOVQ_ST:
		if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) &&
			(disp>-2047) && (disp<2047))
		{
			opw1=0x00003027|((rn&31)<<15)|((rm&31)<<20)|dispm;
			break;
		}

		if(BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg(
			ctx, rn, disp, rm, 0x00003023, &opw1, &opw2)>0)
				break;

#if 0
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			opw1=0x00003023|((rn&31)<<15)|((rm&31)<<20)|dispm;
			break;
		}

		if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			opw1=0x00003027|((rn&31)<<15)|((rm&31)<<20)|dispm;
			break;
		}

//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00003023|((rn&31)<<15)|((rm&31)<<20)|dispm;
#endif
		break;

	case BGBCC_SH_NMID_MOVX2:
	case BGBCC_SH_NMID_MOVX_ST:
		if(rm&1)
			break;
		if(BGBCC_JX2RV_TryEncJumboOpXRegDisp12StXReg(
			ctx, rn, disp, rm, 0x00007023, &opw1, &opw2)>0)
				break;

//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00007023|((rn&31)<<15)|((rm&31)<<20)|dispm;
		break;
	}



	if((opw1>0) && (opw3<0))
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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

int BGBCC_JX2RV_EmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 disp, int rn)
{
	if(!BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, s64 disp, int rn)
{
	s64 opw1, opw2, opw3, opw4, imm12;
	int odr, ex, ex2, nowxi, exw, rt, sc;
	int i, j, k;

	if(	(nmid==BGBCC_SH_NMID_LEAB) ||
		(nmid==BGBCC_SH_NMID_LEAW) ||
		(nmid==BGBCC_SH_NMID_LEAL) ||
		(nmid==BGBCC_SH_NMID_LEAQ))
	{
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, disp, rn));
	}

	if(	(nmid==BGBCC_SH_NMID_MOVB_ST) ||
		(nmid==BGBCC_SH_NMID_MOVW_ST) ||
		(nmid==BGBCC_SH_NMID_MOVL_ST) ||
		(nmid==BGBCC_SH_NMID_MOVQ_ST))
	{
		return(BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx, nmid, rn, rm, disp));
	}

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);


	if(nmid==BGBCC_SH_NMID_MOVX2)
	{
		if(((rn&63)<4) || ((rn&63)>=32) || !(ctx->has_pushx2&1) || (rn&1))
		{
			if(ctx->emit_isprobe)
				return(0);

			i=BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
				BGBCC_SH_NMID_MOVQ, rm, disp+0, rn+0);
			if(i<=0)
				return(i);
			i=BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
				BGBCC_SH_NMID_MOVQ, rm, disp+8, rn+1);
			return(i);
		}
	}

	i=BGBCC_JX2X3_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);
	if(i>0)
		return(i);

	if(((((s32)(disp<<20))>>20)!=disp) && !(ctx->has_jumbo&2))
	{
#if 0
		if((ctx->has_jumbo&2) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			if((disp==((s32)disp)) || (disp==((u32)disp)))
			{
				imm12=(disp&2047)|((disp>>63)&(~2047));
				BGBCC_JX2RV_EmitOpImm(ctx,
					BGBCC_SH_NMID_RVJ21I, disp>>11);
				BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
					nmid, rm, imm12, rn);
				return(1);
			}
		}
#endif
	
		if(ctx->emit_isprobe)
			return(0);

#if 1
		sc=-1;
		if(	(nmid==BGBCC_SH_NMID_MOVB) ||
			(nmid==BGBCC_SH_NMID_MOVUB) ||
			(nmid==BGBCC_SH_NMID_LEAB))
				sc=0;
		if(	(nmid==BGBCC_SH_NMID_MOVW) ||
			(nmid==BGBCC_SH_NMID_MOVUW) ||
			(nmid==BGBCC_SH_NMID_LEAW))
				sc=1;
		if(	(nmid==BGBCC_SH_NMID_MOVL) ||
			(nmid==BGBCC_SH_NMID_MOVUL) ||
			(nmid==BGBCC_SH_NMID_MOVDL) ||
			(nmid==BGBCC_SH_NMID_LEAL))
				sc=2;
		if(	(nmid==BGBCC_SH_NMID_MOVQ) ||
			(nmid==BGBCC_SH_NMID_LEAQ))
				sc=3;

		if((ctx->has_rvzba&16) && (sc>=0) &&
			((((s16)(disp>>sc))<<sc)==disp))
		{
			rt=BGBCC_SH_REG_R5;
			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, disp>>sc, rt);
			i=BGBCC_JX2RV_TryEmitOpLdReg2Reg(ctx, nmid, rm, rt, rn);
			return(i);
		}
#endif
	
		rt=BGBCC_SH_REG_R5;
		BGBCC_JX2RV_EmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, disp, rt);
		i=BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx, nmid, rt, 0, rn);
		return(i);
	}

	if(nmid==BGBCC_SH_NMID_MOVC)
	{
		if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
			nmid=BGBCC_SH_NMID_MOVQ;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	imm12=disp&4095;

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00000003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00000003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVW:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00001003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00001003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00002003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00002003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) &&
			(disp>=-2047) && (disp<=2047))
		{
			opw1=0x00003007|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
			break;
		}

		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00003003, &opw1, &opw2)>0)
				break;

#if 0
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
		{
			opw1=0x00003003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
			break;
		}

		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))
		{
			opw1=0x00003007|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
			break;
		}
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0x00003003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVUB:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00004003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00004003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVUW:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00005003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00005003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVUL:
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00006003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00006003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVDL:
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
			BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) &&
			(disp>=-2047) && (disp<=2047))
		{
			opw1=0x00002007|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
			break;
		}

		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00006003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00006003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;

	case BGBCC_SH_NMID_MOVX2:
		if(rn&1)
			break;
		if(BGBCC_JX2RV_TryEncJumboOpXRegImm12XReg(
			ctx, rm, disp, rn, 0x00007003, &opw1, &opw2)>0)
				break;
#if 0
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0x00007003|((rn&31)<<7)|((rm&31)<<15)|(imm12<<20);
#endif
		break;
	}

	if((opw1>0) && (opw3<0))
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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

int BGBCC_JX2RV_EmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int rn)
{
	if(!BGBCC_JX2RV_TryEmitOpLblReg(ctx, nmid, lbl, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	static int isrec=0;
	s64 opw1, opw2, opw3, opw4;
	s64 gpoffs;
	int odr, ex, ex2, nowxi, exw, rlty;
	int i, j, k;

	reg=BGBCC_JX2RV_NormalizeReg(ctx, reg);

	gpoffs=0;
	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
	{
		gpoffs=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BSR:
#ifdef BGBCC_USE_RELW23_XG3
		if(ctx->emit_riscv&0x22)
		{
			if((reg&63)==0)
			{
				rlty=BGBCC_SH_RLC_RELW23_XG3;
				opw1=0x0000C002U;
				break;
			}
			if((reg&63)==1)
			{
				rlty=BGBCC_SH_RLC_RELW23_XG3;
				opw1=0x0000D002U;
				break;
			}
		}
#endif

		if(BGBCC_JX2_EmitCheckAutoLabelNear20(ctx, lbl, nmid))
		{
			if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
				break;
	//		opw1=0x00000003|((reg&31)<<7)|((rm&31)<<15)|(disp<<20);
	//		opw1=0xFFDFF06F|((reg&31)<<7);
			opw1=0x0000006F|((reg&31)<<7);
			rlty=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00000017|(5<<7);
		opw2=0x00000067|((reg&31)<<7)|(5<<15);
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		break;

#ifdef BGBCC_USE_RELW23_XG3
	case BGBCC_SH_NMID_BT:
		if(!(ctx->emit_riscv&0x22))
			break;
		if(!(ctx->has_jcmp&16))
			break;
		opw1=0x0000F010;
		rlty=BGBCC_SH_RLC_RELW23_XG3;
		break;

	case BGBCC_SH_NMID_BF:
		if(!(ctx->emit_riscv&0x22))
			break;
		if(!(ctx->has_jcmp&16))
			break;
		opw1=0x0000F011;
		rlty=BGBCC_SH_RLC_RELW23_XG3;
		break;
#endif

	case BGBCC_SH_NMID_MOV:
	case BGBCC_SH_NMID_LEAB:	
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(	!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if((ctx->emit_riscv&0x22) &&
				BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((gpoffs>0) && (gpoffs<(1<<19)) && !(gpoffs&7))
				{
					opw1=0x00005012|((reg&63)<<6)|((gpoffs>>3)<<16);
					break;
				}

				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00000026U,
					BGBCC_SH_REG_RQ3, gpoffs, 0, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x00000026U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif

#if 1
				if(!BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_RELB33_XG3;
					opw1=0x0000001AU;
					opw2=0x00000026U|((reg&63)<<6);
					break;
				}
#endif
			}

			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							BGBCC_SH_NMID_MOV, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegReg(ctx,
							BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					}else
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							BGBCC_SH_NMID_MOV, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00000013U|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}
			
			if((ctx->emit_riscv&0x22) &&
				(k>0) && (k<(1<<19)) && !(k&7))
			{
				opw1=0x00005012U|((reg&63)<<6)|((k>>3)<<16);
				break;
			}

			if((ctx->emit_riscv&0x22) && (k>0) &&
				BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00000026U,
				BGBCC_SH_REG_RQ3, gpoffs, 0, reg, &opw1, &opw2)>0)
					break;

//			if(ctx->has_jumbo&2)
			if(0)	//DBG disabled (2025-03: Can't find cause of bug...)
			{
				rlty=BGBCC_SH_RLC_PBO32J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00000013U|((reg&31)<<7)|(3<<15);
				break;
			}

			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
//			opw3=0x00000033|((reg&31)<<7)|((reg&31)<<15)|(3<<20);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			break;
		}

#if 1
		if((ctx->emit_riscv&0x22) &&
			!BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			rlty=BGBCC_SH_RLC_RELB33_XG3;
			opw1=0x0000001AU;
			opw2=0x00000026U|((reg&63)<<6);
//			opw3=0x00100073;
			break;
		}
#endif

		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVB:
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(	!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00008006U,
					BGBCC_SH_REG_RQ3, gpoffs, 0, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x80008006U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif

				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegReg(ctx,
							BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					}else
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
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
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(	!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00008026U,
					BGBCC_SH_REG_RQ3, gpoffs, 0, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x80008026U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif

				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegReg(ctx,
							BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					}else
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
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
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00009006U,
					BGBCC_SH_REG_RQ3, gpoffs, 1, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x80009006U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif
				if(!ctx->emit_isprobe)
				{
					BGBCC_JX2RV_EmitOpLblReg(ctx,
						nmid, lbl, BGBCC_SH_REG_RQ5);
					BGBCC_JX2RV_EmitOpRegReg(ctx,
						BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					return(1);
				}
			}
			break;
		}
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00001003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

//			if(ctx->has_jumbo&2)
			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00001003|((reg&31)<<7)|(3<<15);
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
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00009026U,
					BGBCC_SH_REG_RQ3, gpoffs, 1, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x80009026U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif

				if(!ctx->emit_isprobe)
				{
					BGBCC_JX2RV_EmitOpLblReg(ctx,
						nmid, lbl, BGBCC_SH_REG_RQ5);
					BGBCC_JX2RV_EmitOpRegReg(ctx,
						BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					return(1);
				}
			}
			break;
		}
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00005003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

//			if(ctx->has_jumbo&2)
			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00005003U|((reg&31)<<7)|(3<<15);
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
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if((ctx->emit_riscv&0x22) &&
				BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if((gpoffs>0) && (gpoffs<(1<<18)) && !(gpoffs&3))
				{
					opw1=0x0000D032|((reg&63)<<6)|((gpoffs>>2)<<16);
					break;
				}
#endif

				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x0000A006U,
					BGBCC_SH_REG_RQ3, gpoffs, 2, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x8000A006U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif
			}
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegReg(ctx,
							BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					}else
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00002003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

#if 1
			if((ctx->emit_riscv&0x22) &&
				(k>0) && (k<(1<<18)) && !(k&3))
			{
				opw1=0x0000D032|((reg&63)<<6)|((k>>2)<<16);
				break;
			}
#endif

			if(ctx->has_jumbo&2)
//			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00002003U|((reg&31)<<7)|(3<<15);
				break;
			}

#if 1
			rlty=BGBCC_SH_RLC_PBO32LD_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw3=0x00002003|((reg&31)<<7)|((reg&31)<<15);
#endif

#if 0
			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00002003|((reg&31)<<7)|((reg&31)<<15);
#endif
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00002003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if((ctx->emit_riscv&0x22) &&
					(gpoffs>0) && (gpoffs<(1<<18)) && !(gpoffs&3))
				{
					opw1=0x00005032|((reg&63)<<6)|((gpoffs>>2)<<16);
					break;
				}
#endif

				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x0000A026U,
					BGBCC_SH_REG_RQ3, gpoffs, 2, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(	(ctx->emit_riscv&0x22) &&
					BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x8000A026U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif

#if 1
				if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, reg) &&
					BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl) &&
					(nmid==BGBCC_SH_NMID_MOVDL))
				{
					k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
					if((k>0) && (k<2048))
					{
						opw1=0x00002007|((reg&31)<<7)|(3<<15)|(k<<20);
						break;
					}
					rlty=BGBCC_SH_RLC_PBO32LD_RVI;
					opw1=0x00000037|(5<<7);
					opw2=0x00000033|(5<<7)|(3<<15)|(5<<20);
					opw3=0x00002007|((reg&31)<<7)|(5<<15);
					break;
				}
#endif

				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegReg(ctx,
							BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					}else
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							nmid, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00006003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

#if 1
			if((ctx->emit_riscv&0x22) &&
				(k>0) && (k<(1<<18)) && !(k&3))
			{
				opw1=0x00005032|((reg&63)<<6)|((k>>2)<<16);
				break;
			}
#endif

			if(ctx->has_jumbo&2)
//			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00006003U|((reg&31)<<7)|(3<<15);
				break;
			}

#if 1
			rlty=BGBCC_SH_RLC_PBO32LD_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw3=0x00006003|((reg&31)<<7)|((reg&31)<<15);
#endif

#if 0
			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00006003|((reg&31)<<7)|((reg&31)<<15);
#endif
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00006003|((reg&31)<<7)|((reg&31)<<15);
		break;

	case BGBCC_SH_NMID_MOVQ:
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if((ctx->emit_riscv&0x22) &&
				BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if((gpoffs>0) && (gpoffs<(1<<19)) && !(gpoffs&7))
				{
					opw1=0x0000D012|((reg&63)<<6)|((gpoffs>>3)<<16);
					break;
				}
#endif

				if((ctx->emit_riscv&0x22) && (gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x0000B006U,
					BGBCC_SH_REG_RQ3, gpoffs, 3, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x8000B006U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif
			}
#if 1
			if(	BGBCC_JX2RV_CheckRegIsFPR(ctx, reg) &&
				BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
				k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
				if((k>0) && (k<2048))
				{
					opw1=0x00003007|((reg&31)<<7)|(3<<15)|(k<<20);
					break;
				}
				rlty=BGBCC_SH_RLC_PBO32LD_RVI;
				opw1=0x00000037|(5<<7);
				opw2=0x00000033|(5<<7)|(3<<15)|(5<<20);
				opw3=0x00003007|((reg&31)<<7)|(5<<15);
				break;
			}
#endif
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							BGBCC_SH_NMID_MOVQ, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegReg(ctx,
							BGBCC_SH_NMID_MOV, BGBCC_SH_REG_RQ5, reg);
					}else
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							BGBCC_SH_NMID_MOVQ, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
			k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
			if((k>0) && (k<2048))
			{
				opw1=0x00003003|((reg&31)<<7)|(3<<15)|(k<<20);
				break;
			}

#if 1
			if((ctx->emit_riscv&0x22) &&
				(k>0) && (k<(1<<19)) && !(k&7))
			{
				opw1=0x0000D012|((reg&63)<<6)|((k>>3)<<16);
				break;
			}
#endif

			if(ctx->has_jumbo&2)
//			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00003003U|((reg&31)<<7)|(3<<15);
				break;
			}

#if 1
			rlty=BGBCC_SH_RLC_PBO32LD_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw3=0x00003003|((reg&31)<<7)|((reg&31)<<15);
#endif

#if 0
			rlty=BGBCC_SH_RLC_PBO32_RVI;
			opw1=0x00000037|((reg&31)<<7);
			opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
			opw3=0x00000033|((reg&31)<<7)|(3<<15)|((reg&31)<<20);
			opw4=0x00003003|((reg&31)<<7)|((reg&31)<<15);
#endif
			break;
		}
		rlty=BGBCC_SH_RLC_REL32UI_RVI;
		opw1=0x00000017|((reg&31)<<7);
		opw2=0x00000013|((reg&31)<<7)|((reg&31)<<15);
		opw3=0x00003003|((reg&31)<<7)|((reg&31)<<15);
		break;

#if 1
	case BGBCC_SH_NMID_FMOVH:
		if((ctx->emit_riscv&0x22) && BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
			((reg&63)>4))
		{
			if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
				rlty=BGBCC_SH_RLC_PBOJ33_XG3;
				opw1=0x0000001AU;
				opw2=0x80006026U|((reg&63)<<6)|(3<<16);
				break;
			}
		}
		break;

	case BGBCC_SH_NMID_FMOVS:
		if((ctx->emit_riscv&0x22) && BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
			((reg&63)>4))
		{
			if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
				rlty=BGBCC_SH_RLC_PBOJ33_XG3;
				opw1=0x0000001AU;
				opw2=0x80006006U|((reg&63)<<6)|(3<<16);
				break;
			}
		}
		break;
#endif

	}

//	if((opw1>0) && (opw3<0))
//		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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

int BGBCC_JX2RV_EmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int rn)
{
	if(!BGBCC_JX2RV_TryEmitOpRegLbl(ctx, nmid, lbl, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	static int isrec=0;
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty, rt, gpoffs;
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

	gpoffs=0;
	if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
	{
		gpoffs=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
	}
	
	if((nmid==BGBCC_SH_NMID_FMOVS) && ((reg&63)==0))
		nmid=BGBCC_SH_NMID_MOVL;
	if((nmid==BGBCC_SH_NMID_FMOVH) && ((reg&63)==0))
		nmid=BGBCC_SH_NMID_MOVW;

	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((ctx->emit_riscv&0x22) &&(gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00000006U,
					BGBCC_SH_REG_RQ3, gpoffs, 0, reg, &opw1, &opw2)>0)
						break;

				if(!ctx->emit_isprobe)
				{
					BGBCC_JX2RV_EmitOpLblReg(ctx,
						BGBCC_SH_NMID_MOV, lbl, BGBCC_SH_REG_RQ5);
					BGBCC_JX2RV_EmitOpRegStRegDisp(ctx,
						BGBCC_SH_NMID_MOVW, reg, BGBCC_SH_REG_RQ5, 0);
					return(1);
				}
			}
			break;
		}

		if((reg&31)==5)
			break;

		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
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
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				if((ctx->emit_riscv&0x22) &&(gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00001006U,
					BGBCC_SH_REG_RQ3, gpoffs, 1, reg, &opw1, &opw2)>0)
						break;

				if(!ctx->emit_isprobe)
				{
					BGBCC_JX2RV_EmitOpLblReg(ctx,
						BGBCC_SH_NMID_MOV, lbl, BGBCC_SH_REG_RQ5);
					BGBCC_JX2RV_EmitOpRegStRegDisp(ctx,
						BGBCC_SH_NMID_MOVW, reg, BGBCC_SH_REG_RQ5, 0);
					return(1);
				}
			}
			break;
		}

		if((reg&31)==5)
			break;

		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
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
	case BGBCC_SH_NMID_MOVDL:
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if((ctx->emit_riscv&0x22) &&
				BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if((gpoffs>0) && (gpoffs<(1<<18)) && !(gpoffs&3))
				{
					opw1=0x0000C032|((reg&63)<<6)|((gpoffs>>2)<<16);
					break;
				}
#endif

				if((ctx->emit_riscv&0x22) &&(gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00002006U,
					BGBCC_SH_REG_RQ3, gpoffs, 2, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x80002006U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif
			}
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
					if((k>0) && (k<2048))
					{
						opw1=0x00002027|(3<<15)|((reg&31)<<20)|
							((k&0xFE0)<<20)|((k&0x01F)<<7);
						break;
					}
					rlty=BGBCC_SH_RLC_PBO32ST_RVI;
					rt=BGBCC_SH_REG_RQ5;
					opw1=0x00000037|((rt&31)<<7);
					opw2=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
					opw3=0x00002027|((rt&31)<<15)|((reg&31)<<20);
					break;
				}
#endif
				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							BGBCC_SH_NMID_MOV, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegStRegDisp(ctx,
							BGBCC_SH_NMID_MOVL, reg, BGBCC_SH_REG_RQ5, 0);
					}else
					{
						BGBCC_JX2RV_EmitOpRegLbl(ctx,
							BGBCC_SH_NMID_MOVL, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
		if((reg&31)==5)
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

#if 1
			if((ctx->emit_riscv&0x22) &&
				(gpoffs>0) && (gpoffs<(1<<18)) && !(gpoffs&3))
			{
				opw1=0x0000C032|((reg&63)<<6)|((gpoffs>>2)<<16);
				break;
			}
#endif

#if 1
			if(ctx->has_jumbo&2)
//			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32STJ_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00002023U|((reg&31)<<20)|(3<<15);
				break;
			}
#endif

#if 1
			rlty=BGBCC_SH_RLC_PBO32ST_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw3=0x00002023|((rt&31)<<15)|((reg&31)<<20);
#endif

#if 0
			rlty=BGBCC_SH_RLC_PBO32_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
			opw3=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw4=0x00002023|((rt&31)<<15)|((reg&31)<<20);
#endif
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
//		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg) ||
			((ctx->emit_riscv&0x22) && !isrec))
		{
			if((ctx->emit_riscv&0x22) &&
				BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if((gpoffs>0) && (gpoffs<(1<<19)) && !(gpoffs&7))
				{
					opw1=0x0000C012|((reg&63)<<6)|((gpoffs>>3)<<16);
					break;
				}
#endif
				
				if((ctx->emit_riscv&0x22) &&(gpoffs>0) &&
					BGBCC_JX2X3_CheckEncodeRIRJ_Disp10s(ctx, 0x00003006U,
						BGBCC_SH_REG_RQ3, gpoffs, 3, reg, &opw1, &opw2)>0)
						break;

#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					rlty=BGBCC_SH_RLC_PBOJ33_XG3;
					opw1=0x0000001AU;
					opw2=0x80003006U|((reg&63)<<6)|(3<<16);
					break;
				}
#endif
			}
			if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
#if 1
				if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
				{
					k=BGBCC_JX2_CheckGetLabelGpOffs(ctx, lbl);
					if((k>0) && (k<2048))
					{
						opw1=0x00003027|(3<<15)|((reg&31)<<20)|
							((k&0xFE0)<<20)|((k&0x01F)<<7);
						break;
					}
					rlty=BGBCC_SH_RLC_PBO32ST_RVI;
					rt=BGBCC_SH_REG_RQ5;
					opw1=0x00000037|((rt&31)<<7);
					opw2=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
					opw3=0x00003027|((rt&31)<<15)|((reg&31)<<20);
					break;
				}
#endif
				if(!ctx->emit_isprobe)
				{
					isrec++;
					if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
					{
						BGBCC_JX2RV_EmitOpLblReg(ctx,
							BGBCC_SH_NMID_MOV, lbl, BGBCC_SH_REG_RQ5);
						BGBCC_JX2RV_EmitOpRegStRegDisp(ctx,
							BGBCC_SH_NMID_MOVQ, reg, BGBCC_SH_REG_RQ5, 0);
					}else
					{
						BGBCC_JX2RV_EmitOpRegLbl(ctx,
							BGBCC_SH_NMID_MOVQ, lbl, reg);
					}
					isrec--;
					return(1);
				}
			}
			break;
		}
		if((reg&31)==5)
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

#if 1
			if((ctx->emit_riscv&0x22) && (k>0) && (k<(1<<19)) && !(k&7))
			{
				opw1=0x0000C012|((reg&63)<<6)|((k>>3)<<16);
				break;
			}
#endif

#if 0
			if((ctx->emit_riscv&0x22) &&
				BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
				rlty=BGBCC_SH_RLC_PBOJ33_XG3;
				opw1=0x0000001AU;
				opw2=0x80003006U|((reg&63)<<6)|(3<<16);
				break;
			}
#endif

#if 1
			if(ctx->has_jumbo&2)
//			if(0)	//DBG disabled
			{
				rlty=BGBCC_SH_RLC_PBO32STJ_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00003023U|((reg&31)<<20)|(3<<15);
				break;
			}
#endif

#if 1
			rlty=BGBCC_SH_RLC_PBO32ST_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw3=0x00003023|((rt&31)<<15)|((reg&31)<<20);
#endif

#if 0
			rlty=BGBCC_SH_RLC_PBO32_RVI;
			rt=BGBCC_SH_REG_RQ5;
			opw1=0x00000037|((rt&31)<<7);
			opw2=0x00000013|((rt&31)<<7)|((rt&31)<<15);
			opw3=0x00000033|((rt&31)<<7)|(3<<15)|((rt&31)<<20);
			opw4=0x00003023|((rt&31)<<15)|((reg&31)<<20);
#endif
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


#if 1
	case BGBCC_SH_NMID_FMOVH:
		if((ctx->emit_riscv&0x22) && BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
			((reg&63)>4))
		{
			if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
				rlty=BGBCC_SH_RLC_PBOJ33_XG3;
				opw1=0x0000001AU;
				opw2=0x80004026U|((reg&63)<<6)|(3<<16);
				break;
			}
		}
		break;

	case BGBCC_SH_NMID_FMOVS:
		if((ctx->emit_riscv&0x22) && BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg) &&
			((reg&63)>4))
		{
			if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
				rlty=BGBCC_SH_RLC_PBOJ33_XG3;
				opw1=0x0000001AU;
				opw2=0x80004006U|((reg&63)<<6)|(3<<16);
				break;
			}
		}
		break;
#endif

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
	int odr, ex, ex2, nowxi, exw, rlty, rlty2, rlty3, rt, nm1;
	int i, j, k;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	i=BGBCC_JX2X3_TryEmitOpRegRegLbl(ctx, nmid, rm, rn, lbl);
	if(i>0)
		return(i);

	nm1=-1;

#if 0
	if(nmid==BGBCC_SH_NMID_BRLE)
		nm1=BGBCC_SH_NMID_BRGE;
	if(nmid==BGBCC_SH_NMID_BRLEL)
		nm1=BGBCC_SH_NMID_BRGEL;

	if(nmid==BGBCC_SH_NMID_BRGT)
		nm1=BGBCC_SH_NMID_BRLT;
	if(nmid==BGBCC_SH_NMID_BRGTL)
		nm1=BGBCC_SH_NMID_BRLTL;

	if(nmid==BGBCC_SH_NMID_BRLEU)
		nm1=BGBCC_SH_NMID_BRGEU;
	if(nmid==BGBCC_SH_NMID_BRLEUL)
		nm1=BGBCC_SH_NMID_BRGEUL;

	if(nmid==BGBCC_SH_NMID_BRGTU)
		nm1=BGBCC_SH_NMID_BRLTU;
	if(nmid==BGBCC_SH_NMID_BRGTUL)
		nm1=BGBCC_SH_NMID_BRLTL;
#endif

#if 0
	if(nmid==BGBCC_SH_NMID_BRGE)
		nm1=BGBCC_SH_NMID_BRLE;
	if(nmid==BGBCC_SH_NMID_BRGEL)
		nm1=BGBCC_SH_NMID_BRLEL;

	if(nmid==BGBCC_SH_NMID_BRLT)
		nm1=BGBCC_SH_NMID_BRGT;
	if(nmid==BGBCC_SH_NMID_BRLTL)
		nm1=BGBCC_SH_NMID_BRGTL;

	if(nmid==BGBCC_SH_NMID_BRGEU)
		nm1=BGBCC_SH_NMID_BRLEU;
	if(nmid==BGBCC_SH_NMID_BRGEUL)
		nm1=BGBCC_SH_NMID_BRLEUL;

	if(nmid==BGBCC_SH_NMID_BRLTU)
		nm1=BGBCC_SH_NMID_BRGTU;
	if(nmid==BGBCC_SH_NMID_BRLTUL)
		nm1=BGBCC_SH_NMID_BRGTL;
#endif

	if(nm1>=0)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegLbl(ctx, nm1, rn, rm, lbl));
	}

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
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00000063U|((rn&31)<<15)|((rm&31)<<20);
				break;
			}
			
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
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00001063U|((rn&31)<<15)|((rm&31)<<20);
				break;
			}
			
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
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00004063U|((rn&31)<<15)|((rm&31)<<20);
				break;
			}
			
			opw1=0x00005463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00004063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

#if 1
	case BGBCC_SH_NMID_BRGT:
	case BGBCC_SH_NMID_BRGTL:
//	case BGBCC_SH_NMID_BRLT:
//	case BGBCC_SH_NMID_BRLTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00004063U|((rm&31)<<15)|((rn&31)<<20);
				break;
			}
			
			opw1=0x00005463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00004063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
#endif

#if 1
	case BGBCC_SH_NMID_BRLE:
	case BGBCC_SH_NMID_BRLEL:
//	case BGBCC_SH_NMID_BRGE:
//	case BGBCC_SH_NMID_BRGEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00005063U|((rm&31)<<15)|((rn&31)<<20);
				break;
			}
			
			opw1=0x00004463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00005063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
#endif

	case BGBCC_SH_NMID_BRGE:
	case BGBCC_SH_NMID_BRGEL:
//	case BGBCC_SH_NMID_BRLE:
//	case BGBCC_SH_NMID_BRLEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00005063U|((rn&31)<<15)|((rm&31)<<20);
				break;
			}
			
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
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00006063U|((rn&31)<<15)|((rm&31)<<20);
				break;
			}
			
			opw1=0x00007463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00006063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

#if 1
	case BGBCC_SH_NMID_BRGTU:
	case BGBCC_SH_NMID_BRGTUL:
//	case BGBCC_SH_NMID_BRLTU:
//	case BGBCC_SH_NMID_BRLTUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00006063U|((rm&31)<<15)|((rn&31)<<20);
				break;
			}
			
			opw1=0x00007463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00006063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
#endif

	case BGBCC_SH_NMID_BRGEU:
	case BGBCC_SH_NMID_BRGEUL:
//	case BGBCC_SH_NMID_BRLEU:
//	case BGBCC_SH_NMID_BRLEUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00007063U|((rn&31)<<15)|((rm&31)<<20);
				break;
			}
			
			opw1=0x00006463|((rn&31)<<15)|((rm&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00007063|((rn&31)<<15)|((rm&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

#if 1
	case BGBCC_SH_NMID_BRLEU:
	case BGBCC_SH_NMID_BRLEUL:
//	case BGBCC_SH_NMID_BRGEU:
//	case BGBCC_SH_NMID_BRGEUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty=BGBCC_SH_RLC_RELW33J_RVI;
//				opw1=0x0000401BU;
				opw1=0x0000403FU;
				opw2=0x00007063U|((rm&31)<<15)|((rn&31)<<20);
				break;
			}
			
			opw1=0x00006463|((rm&31)<<15)|((rn&31)<<20);
			opw2=0x0000006F;
			rlty2=BGBCC_SH_RLC_RELW20_RVI;
			break;
		}

		opw1=0x00007063|((rm&31)<<15)|((rn&31)<<20);
		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
#endif

	case BGBCC_SH_NMID_BRTSTQ:
	case BGBCC_SH_NMID_BRTSTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;

		rt=BGBCC_SH_REG_R5;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
		{
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty2=BGBCC_SH_RLC_RELW33J_RVI;
				opw1=0x00007033U|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
//				opw2=0x0000401BU;
				opw2=0x0000403FU;
				opw3=0x00000063U|((rt&31)<<15)|(0<<20);
				break;
			}

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
//			if(ctx->has_jumbo&2)
			if(0)
			{
				rlty2=BGBCC_SH_RLC_RELW33J_RVI;
				opw1=0x00007033U|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
//				opw2=0x0000401BU;
				opw2=0x0000403FU;
				opw3=0x00001063U|((rt&31)<<15)|(0<<20);
				break;
			}

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



#if 1
int BGBCC_JX2RV_TryEmitOpImmRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm, int rn, int lbl)
{
	s64 opw1, opw2, opw3, opw4;
	int odr, ex, ex2, nowxi, exw, rlty, rlty2, rlty3, rt;
	int i, j, k;

	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	i=BGBCC_JX2X3_TryEmitOpImmRegLbl(ctx, nmid, imm, rn, lbl);
	if(i>0)
		return(i);

	if(!(ctx->has_rvzba&32))
		return(0);

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	rlty=-1;
	rlty2=-1;
	rlty3=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_BREQ:
	case BGBCC_SH_NMID_BREQL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm, 4,
			0x00000063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;
	
//		opw1=0x00000063|((rn&31)<<15)|((rm&31)<<20);
//		opw1=0x00000063|((rn&31)<<15)|((rm&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRNE:
	case BGBCC_SH_NMID_BRNEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm, 4,
			0x00001063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;
	
//		opw1=0x00001063|((rn&31)<<15)|((rm&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;


	case BGBCC_SH_NMID_BRLT:
	case BGBCC_SH_NMID_BRLTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm, 4,
			0x00004063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00004063|((rn&31)<<15)|((rm&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGT:
	case BGBCC_SH_NMID_BRGTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm+1, 4,
			0x00005063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00004063|((rm&31)<<15)|((rn&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;


	case BGBCC_SH_NMID_BRLE:
	case BGBCC_SH_NMID_BRLEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm+1, 4,
			0x00004063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00005063|((rm&31)<<15)|((rn&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGE:
	case BGBCC_SH_NMID_BRGEL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm, 4,
			0x00005063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00005063|((rn&31)<<15)|((rm&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRLTU:
	case BGBCC_SH_NMID_BRLTUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm, 4,
			0x00006063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00006063|((rn&31)<<15)|((rm&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGTU:
	case BGBCC_SH_NMID_BRGTUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm+1, 4,
			0x00007063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00006063|((rm&31)<<15)|((rn&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRGEU:
	case BGBCC_SH_NMID_BRGEUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm, 4,
			0x00007063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00007063|((rn&31)<<15)|((rm&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRLEU:
	case BGBCC_SH_NMID_BRLEUL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

		BGBCC_JX2RV_TryEncJumboSubOpXRegImmRoXReg(
			ctx, rn, imm+1, 4,
			0x00006063, 0, &opw1, &opw2);
		rlty2=BGBCC_SH_RLC_RELW12_RVI;

//		opw1=0x00007063|((rm&31)<<15)|((rn&31)<<20);
//		rlty=BGBCC_SH_RLC_RELW12_RVI;
		break;
		
	case BGBCC_SH_NMID_BRTSTQ:
	case BGBCC_SH_NMID_BRTSTL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

//		opw1=0x00007033|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
//		opw2=0x00000063|((rt&31)<<15)|(0<<20);
//		rlty2=BGBCC_SH_RLC_RELW12_RVI;
		break;

	case BGBCC_SH_NMID_BRTSTNQ:
	case BGBCC_SH_NMID_BRTSTNL:
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid))
			break;

//		opw1=0x00007033|((rn&31)<<15)|((rm&31)<<20)|((rt&31)<<7);
//		opw2=0x00001063|((rt&31)<<15)|(0<<20);
//		rlty2=BGBCC_SH_RLC_RELW12_RVI;
		break;
	}

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpImmRegLbl(ctx, nmid, imm, rn, lbl);

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
#endif

int BGBCC_JX2RV_TryEmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int ret;
	
	ret=BGBCC_JX2X3_TryEmitOpLabel(ctx, nmid, lbl);
	if(ret>0)
		return(ret);

	if(nmid==BGBCC_SH_NMID_BRA)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_ZZR));

	if(nmid==BGBCC_SH_NMID_BSR)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_LR));

	if(nmid==BGBCC_SH_NMID_BRAN)
	{
#if 0
		if(BGBCC_JX2_CheckPadLastOpWasBRA(ctx))
		{
			/* Eliminate branch directly after a branch. */
			return(1);
		}
#endif

		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_ZZR));
	}

	if(nmid==BGBCC_SH_NMID_BSRN)
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			BGBCC_SH_NMID_BSR, lbl, BGBCC_SH_REG_LR));

	if((nmid==BGBCC_SH_NMID_BT) || (nmid==BGBCC_SH_NMID_BF))
	{
		if(!(ctx->emit_riscv&0x22))
			return(0);
		return(BGBCC_JX2RV_TryEmitOpLblReg(ctx,
			nmid, lbl, BGBCC_SH_REG_ZZR));
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	s64 opw1, opw2, opw3, reg2, ex, ex2, exw;

	if(nmid==BGBCC_SH_NMID_RTS)
		return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_JSR, BGBCC_SH_REG_LR, 0, BGBCC_SH_REG_ZZR));
	if(nmid==BGBCC_SH_NMID_RTSU)
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

	if(nmid==BGBCC_SH_NMID_RTE)
	{
		opw1=0x30200073;
	}

	if(nmid==BGBCC_SH_NMID_SLEEP)
	{
		opw1=0x00500073;
	}

//	if((opw1>0) && (opw3<0))
	if(opw1>0)
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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

int BGBCC_JX2RV_EmitOpImm(BGBCC_JX2_Context *ctx,
	int nmid, s64 imm)
{
	int i;
	if(!BGBCC_JX2RV_TryEmitOpImm(ctx, nmid, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2RV_TryEmitOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	s64 opw1, opw2, odr;

	opw1=-1;
	opw2=-1;

	if(nmid==BGBCC_SH_NMID_BRK)
	{
		if(ctx->emit_riscv&0x22)
		{
			opw1=0x00004092|((imm&0xFFFF)<<16);
		}else
		{
//			opw1=0x00100073;

			opw1=0x0000601B|
				((imm&2047)<<20) |
				(((imm>>11)&31)<<15);
//			opw2=0x00100073;
		}
	}

#if 0
	if(nmid==BGBCC_SH_NMID_RVJ21I)
	{
//		opw1=0x0000401BU|
		opw1=0x0000403FU|
			((imm&2047)<<20)|
			(((imm>>11)&31)<<15)|
			(((imm>>16)&31)<< 7);
	}

	if(nmid==BGBCC_SH_NMID_RVJ21O)
	{
//		opw1=0x8000401BU|
		opw1=0x8000403FU|
			((imm&2047)<<20)|
			(((imm>>11)&31)<<15)|
			(((imm>>16)&31)<< 7);
	}
#endif

	if(opw1>0)
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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
	int rm3, rn3, rm1, rn1, nm1, cm1, cn1;
	int i;

	rm=BGBCC_JX2RV_NormalizeReg(ctx, rm);
	rn=BGBCC_JX2RV_NormalizeReg(ctx, rn);

	i=BGBCC_JX2X3_TryEmitOpRegReg(ctx, nmid, rm, rn);
	if(i>0)
		return(i);

	if(nmid==BGBCC_SH_NMID_FLDCF)
	{
		if((rm&63)==6)
		{
//			BGBCC_DBGBREAK
		}
	}

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
		cm1=BGBCC_JX2RV_NormalizeRegAsCsr(ctx, rm);
		cn1=BGBCC_JX2RV_NormalizeRegAsCsr(ctx, rn);
		
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

		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) && (cn1>0))
		{	return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_CSRRW, rm, cn1, BGBCC_SH_REG_ZZR));	}
		if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) && (cm1>0))
		{	return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_CSRRW, BGBCC_SH_REG_ZZR, cm1, rn));	}
	}

	if(nmid==BGBCC_SH_NMID_XMOV)
	{
		rm3=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rm);
		rn3=BGBCC_JX2_TryNormalizeXReg(ctx, nmid, rn);

		BGBCC_JX2RV_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, rm3+0, rn3+0);
		BGBCC_JX2RV_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV, rm3+1, rn3+1);
		return(1);
	}

	if(
		(nmid==BGBCC_SH_NMID_FABS) ||
		(nmid==BGBCC_SH_NMID_FNEG) ||

		(nmid==BGBCC_SH_NMID_FLDCF) ||
		(nmid==BGBCC_SH_NMID_FSTCF) ||

		(nmid==BGBCC_SH_NMID_FLDCI) ||
		(nmid==BGBCC_SH_NMID_FLDCIU) ||
		(nmid==BGBCC_SH_NMID_FLDCISL) ||
		(nmid==BGBCC_SH_NMID_FLDCIUL) ||
		(nmid==BGBCC_SH_NMID_FSTCI) )
	{
		rm1=rm;
		rn1=rn;

		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))
			rm1=BGBCC_SH_REG_R32;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))
			rn1=BGBCC_SH_REG_R33;

		if(	(nmid==BGBCC_SH_NMID_FLDCI)		||
			(nmid==BGBCC_SH_NMID_FLDCIU)	||
			(nmid==BGBCC_SH_NMID_FLDCISL)	||
			(nmid==BGBCC_SH_NMID_FLDCIUL)	)
		{
			if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))
				rm1=rm;
		}
		if(nmid==BGBCC_SH_NMID_FSTCI)
		{
			if(BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
				rn1=rn;
		}

		if(ctx->has_jumbo&2)
		{
			rm1=rm;
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
		if(ctx->has_rvzba&1)
		{
			return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_ADDUL, rm, BGBCC_SH_REG_ZZR, rn));
		}
	
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
		if(ctx->has_jumbo&2)
		{
			BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_AND,
				rm, 65535, rn);
			return(1);
		}
	
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
		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_AND,
			rm, 255, rn);

//		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
//			rm, 56, rn);
//		BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_SHLDQ,
//			rn, -56, rn);
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

	nm1=-1;
	if(nmid==BGBCC_SH_NMID_CMPQEQ)	nm1=BGBCC_SH_NMID_CMPQEQ;
	if(nmid==BGBCC_SH_NMID_CMPEQ)	nm1=BGBCC_SH_NMID_CMPEQ;
	if(nmid==BGBCC_SH_NMID_CMPQGT)	nm1=BGBCC_SH_NMID_CMPQGT;
	if(nmid==BGBCC_SH_NMID_CMPGT)	nm1=BGBCC_SH_NMID_CMPGT;
	if(nmid==BGBCC_SH_NMID_CMPQHI)	nm1=BGBCC_SH_NMID_CMPQHI;
	if(nmid==BGBCC_SH_NMID_CMPHI)	nm1=BGBCC_SH_NMID_CMPHI;

	if(nmid==BGBCC_SH_NMID_TSTQ)	nm1=BGBCC_SH_NMID_TSTQ;
	if(nmid==BGBCC_SH_NMID_TST)		nm1=BGBCC_SH_NMID_TSTQ;

//	if(!(ctx->emit_riscv&0x22))
	if(!(ctx->has_jcmp&16))
		nm1=-1;

	if(nm1>0)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx, nm1,
			rn, rm, BGBCC_SH_REG_ZZR));
	}

	opw1=-1;
	opw2=-1;

	switch(nmid)
	{
	case BGBCC_SH_NMID_FLDCI:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10);
				opw2=0xD2200053U|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
//		opw1=0xC2200053U|((rm&31)<<15)|((rn&31)<<7);
		opw1=0xD2200053U|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FSTCI:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10);
				opw2=0xC2200053U|((rm&31)<<15)|((rn&31)<<7)|(1<<12);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
//		opw1=0xD2200053U|((rm&31)<<15)|((rn&31)<<7);
		opw1=0xC2200053U|((rm&31)<<15)|((rn&31)<<7)|(1<<12);
		break;

//	case BGBCC_SH_NMID_FLDCF:
	case BGBCC_SH_NMID_FSTCF:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10);
				opw2=0x40100053U|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x40100053U|((rm&31)<<15)|((rn&31)<<7);
		break;
//	case BGBCC_SH_NMID_FSTCF:
	case BGBCC_SH_NMID_FLDCF:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10);
				opw2=0x42000053U|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x42000053U|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FNEG:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10)|
					(((rm>>5)&1)<<11);
				opw2=0x22001053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x22001053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FABS:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10)|
					(((rm>>5)&1)<<11);
				opw2=0x22002053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x22002053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
		break;
	
	case BGBCC_SH_NMID_FMOV:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10)|
					(((rm>>5)&1)<<11);
				opw2=0x22000053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0x22000053U|((rm&31)<<20)|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FMOVID:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10);
				opw2=0xF2000053U|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn))		break;
		opw1=0xF2000053U|((rm&31)<<15)|((rn&31)<<7);
		break;

	case BGBCC_SH_NMID_FMOVDI:
		if(ctx->has_jumbo&2)
		{
			if(	!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm) ||
				!BGBCC_JX2RV_CheckRegIsFPR(ctx, rn) )
			{
//				opw1=0x8000401BU|
				opw1=0x8000403FU|
					(((rn>>5)&1)<< 9)|
					(((rm>>5)&1)<<10);
				opw2=0xE2000053U|((rm&31)<<15)|((rn&31)<<7);
				break;
			}
		}
		if(!BGBCC_JX2RV_CheckRegIsFPR(ctx, rm))		break;
		if(!BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))		break;
		opw1=0xE2000053U|((rm&31)<<15)|((rn&31)<<7);
		break;
	
	
	default:
		break;
	}

	if(opw1>0)
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

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
	s64 opw1, opw2, opw3, opw4;
	int i, nm1;

	i=BGBCC_JX2X3_TryEmitOpImmReg(ctx, nmid, imm, reg);
	if(i>0)
		return(i);

	nm1=-1;
	if(nmid==BGBCC_SH_NMID_CMPQEQ)	nm1=BGBCC_SH_NMID_CMPQEQ;
	if(nmid==BGBCC_SH_NMID_CMPQGT)	nm1=BGBCC_SH_NMID_CMPQGT;
	if(nmid==BGBCC_SH_NMID_CMPEQ)	nm1=BGBCC_SH_NMID_CMPQEQ;
	if(nmid==BGBCC_SH_NMID_CMPGT)	nm1=BGBCC_SH_NMID_CMPQGT;
	if(nmid==BGBCC_SH_NMID_CMPQHI)	nm1=BGBCC_SH_NMID_CMPQHI;
	if(nmid==BGBCC_SH_NMID_CMPHI)	nm1=BGBCC_SH_NMID_CMPQHI;

//	if(!(ctx->emit_riscv&0x22))
	if(!(ctx->has_jcmp&16))
		nm1=-1;
	
	if(nm1>0)
	{
		if(ctx->emit_isprobe)
			return(0);
		if(!(ctx->has_jcmp&16))
			return(0);

		BGBCC_JX2RV_EmitOpImmReg(ctx,
			BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
		BGBCC_JX2RV_EmitOpRegRegReg(ctx,
			nm1, reg, BGBCC_SH_REG_R5, BGBCC_SH_REG_ZZR);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_CMPQGE)	nm1=BGBCC_SH_NMID_CMPQGT;
	if(nmid==BGBCC_SH_NMID_CMPGE)	nm1=BGBCC_SH_NMID_CMPGT;
	if(nmid==BGBCC_SH_NMID_CMPQHS)	nm1=BGBCC_SH_NMID_CMPQHI;
	if(nmid==BGBCC_SH_NMID_CMPHS)	nm1=BGBCC_SH_NMID_CMPHI;

//	if(!(ctx->emit_riscv&0x22))
	if(!(ctx->has_jcmp&16))
		nm1=-1;

	if(nm1>0)
	{
		if(ctx->emit_isprobe)
			return(0);
		if(!(ctx->has_jcmp&16))
			return(0);

		BGBCC_JX2RV_EmitOpImmReg(ctx,
			BGBCC_SH_NMID_MOV, imm-1, BGBCC_SH_REG_R5);
		BGBCC_JX2RV_EmitOpRegRegReg(ctx,
			nm1, reg, BGBCC_SH_REG_R5, BGBCC_SH_REG_ZZR);
		return(1);
	}

	if(nmid==BGBCC_SH_NMID_CSRR)
	{
		i=BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_CSRRS, BGBCC_SH_REG_ZZR, imm, reg);
		return(i);
	}

	if(nmid==BGBCC_SH_NMID_CSRW)
	{
		i=BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_CSRRW, reg, imm, BGBCC_SH_REG_ZZR);
		return(i);
	}

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
		if(BGBCC_JX2RV_CheckRegIsFPR(ctx, reg))
		{
			if(ctx->emit_isprobe)
				return(0);
			BGBCC_JX2RV_EmitOpImmReg(ctx,
				BGBCC_SH_NMID_MOV, imm, BGBCC_SH_REG_R5);
			BGBCC_JX2RV_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R5, reg);
			return(1);
		}

		if(imm==(((s32)(imm<<20))>>20))
		{
			return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_ZZR, imm, reg));
		}
		
		if(((s32)(imm&0xFFFFF000))==imm)
		{
			return(
				BGBCC_JX2RV_TryEmitOpImmReg(ctx,
					BGBCC_SH_NMID_LUI, imm>>12, reg));
		}

#if 1	//DBG disabled
		if(ctx->has_rvzba&16)
		{
			if((imm==((s16)imm)) || (imm==((u16)imm)))
			{
				return(
					BGBCC_JX2RV_TryEmitOpImmReg(ctx,
						BGBCC_SH_NMID_LDIZ, imm, reg));
			}
		}

		if(ctx->has_jumbo&2)
		{
			if((imm==((s16)imm)) || (imm==((u16)imm)))
			{
				return(
					BGBCC_JX2RV_TryEmitOpImmReg(ctx,
						BGBCC_SH_NMID_LDIZ, imm, reg));
			}

#if 1
			if((imm==((s32)imm)) || (imm==((u32)imm)))
			{
				return(BGBCC_JX2RV_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_ADD,
					BGBCC_SH_REG_ZZR, imm, reg));
			}
#endif

#if 1
			if(ctx->has_jumbo&4)
			{
				return(
					BGBCC_JX2RV_TryEmitOpImmReg(ctx,
						BGBCC_SH_NMID_LDIZ, imm, reg));
			}
#endif

#if 1
			BGBCC_JX2RV_EmitOpRegImmReg(ctx, BGBCC_SH_NMID_ADD,
					BGBCC_SH_REG_ZZR, imm>>32, reg);
			BGBCC_JX2RV_EmitOpRegImmReg(ctx,
				BGBCC_SH_NMID_LDSH32, reg, (u32)imm, reg);
			return(1);
#endif
		}
#endif
	
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

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	imm1=imm&0xFFFFF;

	if(nmid==BGBCC_SH_NMID_LUI)
	{
		if(BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
			opw1=0x00000037|((reg&31)<<7)|(imm1<<12);
		}
	}

	if(nmid==BGBCC_SH_NMID_LDIZ)
	{
		if(BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
	//		if((((s16)imm)==imm) || (((u16)imm)==imm))
			if((((s32)(imm<<15))>>15)==imm)
			{
				opw1=0x0000701B|((reg&31)<<7)|
					(((imm>> 0)&2047)<<20)|
					(((imm>>11)&  31)<<15)|
					(((imm>>31)&   1)<<31);
			}else
				if((((s32)imm)==imm) || (((u32)imm)==imm))
			{
//				opw1=0x0000401BU|((imm&0xFFF)<<20);
				opw1=0x0000403FU|((imm&0xFFF)<<20);
				opw2=0x00000037U|((reg&31)<<7)|(imm&0xFFFFF000);
			}else
			{
				imm1a=imm>>32;
				imm2a=(s32)imm;

				imm2=imm1a>>11;
//				opw1=0x0000401BU |
				opw1=0x0000403FU |
					(((imm2 >> 0)&2047)<<20) |
					(((imm2 >>11)&  31)<<15) |
					(((imm2 >>16)&  31)<< 7) |
					(((imm1a>>10)&   1)<<31) ;
//				opw2=0x0000401BU |
				opw2=0x0000403FU |
					(((imm2a>>0)&0xFFF)<<20) |
					(((imm1a>>0)&   31)<<15) |
					(((imm1a>>5)&   31)<< 7) ;
				opw3=0x00000037|((reg&31)<<7)|(imm2a&0xFFFFF000);
			}
		}
	}

	if(nmid==BGBCC_SH_NMID_LDSH16)
	{
		if(BGBCC_JX2RV_CheckRegIsGPR(ctx, reg))
		{
			opw1=0x0000601B|((reg&31)<<7)|
				(((imm>> 0)&2047)<<20)|
				(((imm>>11)&  31)<<15);
		}
	}

	if((opw1>0) && (opw3<0))
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpImmReg(ctx, nmid, imm, reg);

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
	int ret;

	ret=BGBCC_JX2X3_TryEmitOpRegRegImmReg(ctx, nmid, rs, rt, imm, rn);
	if(ret>0)
		return(ret);

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStReg2(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3, opw4, ex, exw, rt, i0;
	int isrvgpr;

	opw1=-1;
	opw2=-1;
	
	if(nmid==BGBCC_SH_NMID_MOVB_ST)
		nmid=BGBCC_SH_NMID_MOVB;
	if(nmid==BGBCC_SH_NMID_MOVW_ST)
		nmid=BGBCC_SH_NMID_MOVW;
	if(nmid==BGBCC_SH_NMID_MOVL_ST)
		nmid=BGBCC_SH_NMID_MOVL;
	if(nmid==BGBCC_SH_NMID_MOVQ_ST)
		nmid=BGBCC_SH_NMID_MOVQ;
	if(nmid==BGBCC_SH_NMID_MOVX_ST)
		nmid=BGBCC_SH_NMID_MOVX2;

	rt=BGBCC_JX2X3_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn);
	if(rt>0)
		return(rt);

	isrvgpr=0;
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, ro) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
			isrvgpr=1;


	if(	(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVUB))
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			opw1=0x3800002F|((rn&31)<<15)|((ro&31)<<20)|((rm&31)<<7)|(0<<25);
		}else
		{
			rt=BGBCC_SH_REG_R5;
			BGBCC_JX2RV_EmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_ADD, rn, ro, rt);
			BGBCC_JX2RV_EmitOpRegStRegDisp(ctx,
				nmid, rm, rt, 0);
			return(1);
		}
	}

	if(	(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			opw1=0x3800102F|((rn&31)<<15)|((ro&31)<<20)|((rm&31)<<7)|(1<<25);
		}else
		{
			rt=BGBCC_SH_REG_R5;

			i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_LEAW, rn, ro, rt);
			if(i0<=0)
			{
				BGBCC_JX2RV_EmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, ro, 1, rt);
				BGBCC_JX2RV_EmitOpRegRegReg(ctx,
					BGBCC_SH_NMID_ADD, rn, rt, rt);
			}

			BGBCC_JX2RV_EmitOpRegStRegDisp(ctx,
				nmid, rm, rt, 0);
			return(1);
		}
	}

	if(	(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVUL) ||
		(nmid==BGBCC_SH_NMID_MOVDL))
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			opw1=0x3800202F|((rn&31)<<15)|((ro&31)<<20)|((rm&31)<<7)|(2<<25);
		}else
		{
			rt=BGBCC_SH_REG_R5;

			i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_LEAL, rn, ro, rt);
			if(i0<=0)
			{
				BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, ro, 2, rt);
				BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
					BGBCC_SH_NMID_ADD, rn, rt, rt);
			}
			BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
				nmid, rm, rt, 0);
			return(1);
		}
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			opw1=0x3800302F|((rn&31)<<15)|((ro&31)<<20)|((rm&31)<<7)|(3<<25);
		}else
		{
			rt=BGBCC_SH_REG_R5;

			i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_LEAQ, rn, ro, rt);
			if(i0<=0)
			{
				BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, ro, 3, rt);
				BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
					BGBCC_SH_NMID_ADD, rn, rt, rt);
			}
			BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
				nmid, rm, rt, 0);
			return(1);
		}
	}
	
//	if((opw1>0) && (opw3<0))
	if(opw1>0)
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpRegStReg2(ctx, nmid, rm, ro, rn);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpLdReg2Reg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	s64 opw1, opw2, opw3, opw4, ex, rt, exw, i0;
	int isrvgpr;
	
	opw1=-1;	opw2=-1;
	opw3=-1;	opw4=-1;
	
	if(	(nmid==BGBCC_SH_NMID_MOVB_ST) ||
		(nmid==BGBCC_SH_NMID_MOVW_ST) ||
		(nmid==BGBCC_SH_NMID_MOVL_ST) ||
		(nmid==BGBCC_SH_NMID_MOVQ_ST))
	{
		return(BGBCC_JX2RV_TryEmitOpRegStReg2(ctx, nmid, rn, ro, rm));
	}

	rt=BGBCC_JX2X3_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);
	if(rt>0)
		return(rt);

	isrvgpr=0;
	if(	BGBCC_JX2RV_CheckRegIsGPR(ctx, rm) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, ro) &&
		BGBCC_JX2RV_CheckRegIsGPR(ctx, rn))
			isrvgpr=1;

	if(nmid==BGBCC_SH_NMID_LEAB)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_ADD, rm, ro, rn));
	}

	if(nmid==BGBCC_SH_NMID_LEAW)
	{
		i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_LEAW, rm, ro, rn);
		if(i0>0)
			return(i0);
	
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
		i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_LEAL, rm, ro, rn);
		if(i0>0)
			return(i0);
	
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
		i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
			BGBCC_SH_NMID_LEAQ, rm, ro, rn);
		if(i0>0)
			return(i0);
	
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
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			if(nmid==BGBCC_SH_NMID_MOVUB)
				{ opw1=0x3000402F|((rm&31)<<15)|((ro&31)<<20)|
					((rn&31)<<7)|(0<<25); }
			else
				{ opw1=0x3000002F|((rm&31)<<15)|((ro&31)<<20)|
					((rn&31)<<7)|(0<<25); }
		}else
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
	}

	if(	(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVUW))
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			if(nmid==BGBCC_SH_NMID_MOVUW)
				{ opw1=0x3000502F|((rm&31)<<15)|((ro&31)<<20)|
					((rn&31)<<7)|(1<<25); }
			else
				{ opw1=0x3000102F|((rm&31)<<15)|((ro&31)<<20)|
					((rn&31)<<7)|(1<<25); }
		}else
		{
			if((rm&31)==5)
				return(0);
		
			rt=BGBCC_SH_REG_R5;

			i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_LEAW, rm, ro, rt);
			if(i0<=0)
			{
				BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, ro, 1, rt);
				BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
					BGBCC_SH_NMID_ADD, rm, rt, rt);
			}
			BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
				nmid, rt, 0, rn);
			return(1);
		}
	}

	if(	(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVUL) ||
		(nmid==BGBCC_SH_NMID_MOVDL))
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			if(nmid==BGBCC_SH_NMID_MOVUL)
				{ opw1=0x3000602F|((rm&31)<<15)|((ro&31)<<20)|
					((rn&31)<<7)|(2<<25); }
			else
				{ opw1=0x3000202F|((rm&31)<<15)|((ro&31)<<20)|
					((rn&31)<<7)|(2<<25); }
		}else
		{
			if((rm&31)==5)
				return(0);
		
			rt=BGBCC_SH_REG_R5;
			i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_LEAL, rm, ro, rt);
			if(i0<=0)
			{
				BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, ro, 2, rt);
				BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
					BGBCC_SH_NMID_ADD, rm, rt, rt);
			}
			BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
				nmid, rt, 0, rn);
			return(1);
		}
	}

	if(nmid==BGBCC_SH_NMID_MOVQ)
	{
		if((ctx->has_rvzba&16) && isrvgpr)
		{
			opw1=0x3000302F|((rm&31)<<15)|((ro&31)<<20)|
				((rn&31)<<7)|(3<<25);
		}else
		{
			if((rm&31)==5)
				return(0);
		
			rt=BGBCC_SH_REG_R5;

			i0=BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
				BGBCC_SH_NMID_LEAQ, rm, ro, rt);
			if(i0<=0)
			{
				BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
					BGBCC_SH_NMID_SHADQ, ro, 3, rt);
				BGBCC_JX2RV_TryEmitOpRegRegReg(ctx,
					BGBCC_SH_NMID_ADD, rm, rt, rt);
			}
			BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
				nmid, rt, 0, rn);
			return(1);
		}
	}

	if((opw1>0) && (opw3<0))
		{ BGBCC_JX2RV_CheckRepackRVC(ctx, &opw1, &opw2); }

	if(opw1>0)
	{
		if(ctx->emit_isprobe)
			return(1);

		BGBCC_JX2DA_EmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);

		BGBCC_JX2_EmitOpDWord(ctx, opw1);
		if(opw2>0)
			BGBCC_JX2_EmitOpDWord(ctx, opw2);
		return(1);
	}

	return(0);
}


int BGBCC_JX2RV_TryEmitOpLdReg2DispReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	s64 opw1, opw2, opw3, opw4, ex, rt, exw;

	if(	((rm&63)==5) ||
		((ro&63)==5) )
	{
		BGBCC_DBGBREAK
	}

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
		(nmid==BGBCC_SH_NMID_MOVUL) ||
		(nmid==BGBCC_SH_NMID_MOVDL))
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

	if(nmid==BGBCC_SH_NMID_MOVQ)
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

	if(	((rm&63)==5) ||
		((ro&63)==5) ||
		((rn&63)==5))
	{
		BGBCC_DBGBREAK
	}

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
		(nmid==BGBCC_SH_NMID_MOVUL) ||
		(nmid==BGBCC_SH_NMID_MOVDL))
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


int BGBCC_JX2RV_TryEmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int stp;

	if(ctx->emit_isprobe)
		return(0);

	stp=0;
	if(nmid==BGBCC_SH_NMID_MOVB)	stp=1;
	if(nmid==BGBCC_SH_NMID_MOVUB)	stp=1;
	if(nmid==BGBCC_SH_NMID_MOVW)	stp=2;
	if(nmid==BGBCC_SH_NMID_MOVUW)	stp=2;
	if(nmid==BGBCC_SH_NMID_MOVL)	stp=4;
	if(nmid==BGBCC_SH_NMID_MOVUL)	stp=4;
	if(nmid==BGBCC_SH_NMID_MOVDL)	stp=4;
	if(nmid==BGBCC_SH_NMID_MOVQ)	stp=8;
	if(nmid==BGBCC_SH_NMID_MOVX2)	stp=16;
	
	if(stp>0)
	{
		BGBCC_JX2RV_TryEmitOpLdRegDispReg(ctx,
			nmid, rm, 0, rn);
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, stp, rm);
		return(1);
	}

	return(0);
}

int BGBCC_JX2RV_TryEmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int stp;

	if(ctx->emit_isprobe)
		return(0);

	stp=0;
	if(nmid==BGBCC_SH_NMID_MOVB)	stp=1;
	if(nmid==BGBCC_SH_NMID_MOVUB)	stp=1;
	if(nmid==BGBCC_SH_NMID_MOVW)	stp=2;
	if(nmid==BGBCC_SH_NMID_MOVUW)	stp=2;
	if(nmid==BGBCC_SH_NMID_MOVL)	stp=4;
	if(nmid==BGBCC_SH_NMID_MOVUL)	stp=4;
	if(nmid==BGBCC_SH_NMID_MOVDL)	stp=4;
	if(nmid==BGBCC_SH_NMID_MOVQ)	stp=8;
	if(nmid==BGBCC_SH_NMID_MOVX2)	stp=16;
	
	if(stp>0)
	{
		BGBCC_JX2RV_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rn, -stp, rn);
		BGBCC_JX2RV_TryEmitOpRegStRegDisp(ctx,
			nmid, rm, rn, 0);
		return(1);
	}

	return(0);
}

