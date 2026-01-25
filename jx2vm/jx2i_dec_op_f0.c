/*
 MIT No Attribution

 Copyright (c) 2018-2024 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person 
 obtaining a copy of this software and associated documentation 
 files (the "Software"), to deal in the Software without 
 restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell 
 copies of the Software, and to permit persons to whom the 
 Software is furnished to do so.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE.
*/

int BJX2_RemapGPR(BJX2_Context *ctx, int reg)
{
	int reg2;

//	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
//		(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)	)
	if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE)
	{
#if 1
		switch(reg)
		{
			case 0:		reg2=BJX2_REG_ZZR;	break;
			case 1:		reg2=BJX2_REG_LR;	break;
			case 2:		reg2=BJX2_REG_SP;	break;
			case 3:		reg2=BJX2_REG_GBR;	break;
//			case 4:		reg2=BJX2_REG_TBR;	break;
//			case 5:		reg2=BJX2_REG_DHR;	break;
			case 14:	reg2=BJX2_REG_R2;	break;
			case 15:	reg2=BJX2_REG_R3;	break;
			default:	reg2=reg;			break;
		}
#endif
		
#if 0
		switch(reg)
		{
			case 0:		reg2=BJX2_REG_ZZR;	break;
			case 1:		reg2=BJX2_REG_LR;	break;
			case 14:	reg2=BJX2_REG_GBR;	break;
			case 15:	reg2=BJX2_REG_SP;	break;
			default:	reg2=reg;			break;
		}
#endif

		return(reg2);
	}

	if(reg==0)
		return(BJX2_REG_DLR);
	if(reg==1)
		return(BJX2_REG_DHR);
	if(reg==15)
		return(BJX2_REG_SP);
	return(reg);
}

int BJX2_RemapCR(BJX2_Context *ctx, int reg)
{
//	if(reg==15)
//		return(BJX2_REG_R15);
//	return(BJX2_REG_PC+(reg&31));
	return(BJX2_REG_PC+(reg&63));
}

int BJX2_DecodeOpcode_DecF0(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl, rp_dfl;
	int cm_dfl, cn_dfl;
	int rm_x, rn_x, ro_x, rp_x;
	int rn_rvo, rm_rvo, ro_rvo;
	int opw3, opw4, op1r_opsel;
	s64 disp5, imm5, imm6;
	int eq, eo, wq, wo;
	int disp11as, dispasc, disp_ldop;
	int imm8u, imm8n;
	int imm10u, imm10n;
	int imm20s;
	int ret, fnm, is4r, isjimm, isjau, isxg3, isuser;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;

	isxg3=0;
	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
		(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_WXE) )
			isxg3=1;

	isuser=0;
	if(!(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_MD))
		isuser=1;

	if(1)
	{
#ifdef BJX2_FLIPSTNM
		fnm=0;
		if((opw2&0xF808)==0x0000)
			fnm=1;
		if((opw2&0xF008)==0x8000)
		{
			if((opw2&0xF808)==0x8000)
				fnm=1;
			if((opw2&0xF80B)==0x8803)
				fnm=1;
		}

//		if((opw2&0xF80C)==0x9000)
//			fnm=1;
		if((opw2&0xF808)==0x9000)
			fnm=1;

		if(fnm)
		{
			opw1=(opw1&0xFF00)|
				((opw1<<4)&0x00F0)|
				((opw1>>4)&0x000F);
			opw2=(opw2&0xF9FF)|
				((opw2<<1)&0x0400)|
				((opw2>>1)&0x0200);
			jbits=
				( jbits    &0x9FFFFFFFU) |
				((jbits<<1)&0x40000000U) |
				((jbits>>1)&0x20000000U) ;
		}
#endif

		ret=1;
		if(((opw2&0xF000)>=0xC000) && !isxg3)
			ret=0;

		if(ret)
		{
			opw3=(opw1&0xFF00)|((opw2>>4)&0x00FF);
			opw4=(opw2&0xF000)|((opw2<<8)&0x0F00)|
				(opw1&0x00FF);
			opw1=opw3;
			opw2=opw4;
		}
	}

	rn_dfl=(opw2>>4)&15;
	rm_dfl=(opw2   )&15;
	ro_dfl=(opw1   )&15;
	if(opw1&0x0040)rn_dfl+=16;
	if(opw1&0x0020)rm_dfl+=16;
	if(opw1&0x0010)ro_dfl+=16;

	if(jbits&0x40000000U)rn_dfl+=32;
	if(jbits&0x20000000U)rm_dfl+=32;
	if(jbits&0x10000000U)ro_dfl+=32;

	is4r=0;
//	rp_dfl=BJX2_REG_ZZR;
	rp_dfl=rn_dfl;
	if(jbits&0x02000000U)
	{
		if(jbits&0x00000800U)
		{
			rp_dfl=jbits&63;
			is4r=1;
		}
	}
	
	rm_rvo=rm_dfl;
	rn_rvo=rn_dfl;
	ro_rvo=ro_dfl;

	rm_x=(rm_dfl&0x1E)+((rm_dfl&1)?32:0);
	rn_x=(rn_dfl&0x1E)+((rn_dfl&1)?32:0);
	ro_x=(ro_dfl&0x1E)+((ro_dfl&1)?32:0);
	rp_x=(rp_dfl&0x1E)+((rp_dfl&1)?32:0);

//	cm_dfl=BJX2_REG_PC+(rm_dfl&31);
//	cn_dfl=BJX2_REG_PC+(rn_dfl&31);

	cn_dfl=BJX2_RemapCR(ctx, rn_dfl);
	cm_dfl=BJX2_RemapCR(ctx, rm_dfl);

	rn_dfl=BJX2_RemapGPR(ctx, rn_dfl);
	rm_dfl=BJX2_RemapGPR(ctx, rm_dfl);
	ro_dfl=BJX2_RemapGPR(ctx, ro_dfl);
	rp_dfl=BJX2_RemapGPR(ctx, rp_dfl);

	rn_x=BJX2_RemapGPR(ctx, rn_x);
	rm_x=BJX2_RemapGPR(ctx, rm_x);
	ro_x=BJX2_RemapGPR(ctx, ro_x);
	rp_x=BJX2_RemapGPR(ctx, rp_x);

	imm6=((opw2   )&15)|(opw1&0x30);

	disp5=(opw1   )&31;
//	if(opw1&0x0010)disp5|=~31;

	if(jbits&0x10000000U)
		disp5|=~31;

	imm5=disp5;

	eq=(opw1&0x0080)?1:0;
	eo=(opw1&0x0010)?1:0;
	wq=0;
	wo=0;
	isjimm=0;
	isjau=0;

	imm8u=opw2&255;
	imm8n=opw2|(~255);
	imm10u=(opw2&255)|((opw1&0x0060)<<3);
	imm10n=(opw2&255)|((opw1&0x0060)<<3)|(~1023);

	imm20s=((opw1&255)<<12)|(opw2&4095);
	imm20s=((s32)(imm20s<<12))>>12;

//	if(jbits&0x40000000U)imm20s^=(1<<22);
//	if(jbits&0x20000000U)imm20s^=(1<<21);
//	if(jbits&0x10000000U)imm20s^=(1<<20);
	
	if(jbits&0x40000000U)imm20s^=(1<<21);
	if(jbits&0x20000000U)imm20s^=(1<<20);
	if(jbits&0x10000000U)imm20s^=(1<<19);
	
	if(jbits&0x80000000U)
		wq=1;
	if(jbits&0x10000000U)
		wo=1;
	
	disp11as=0;
	dispasc=0;
	disp_ldop=0;

	if(jbits&0x02000000U)
	{
		imm20s=
			((jbits&4095)<<20) |
			((opw1 & 255)<<12) |
			 (opw2 &4095);
		imm20s=(s32)imm20s;
		isjau=1;

//		disp5|=((jbits&255)<<5);
//		if(jbits&0x80)

		disp5=((jbits&2047)<<4)|((opw1   )&15);
//		if(opw1&0x0010)disp5|=4096;
		if(opw1&0x0010)disp5|=0x8000;
		if(jbits&0x10000000U)
//			disp5|=0xFFFFE000;
			disp5|=0xFFFF0000;
			
		disp5=(s32)disp5;
		imm5=disp5;
		
		disp11as=(jbits&2047);
//		if(jbits&0x00000400U)
//			disp11as|=0xFFFFFC00;

		if(jbits&0x00000800U)
		{
			imm5=(jbits&2047);
			if(jbits&0x00000400U)
				imm5|=0xFFFFC000;

//			disp11as=(jbits&255);
//			if(jbits&0x00000400U)
//				disp11as|=0xFFFFFF00;
//			dispasc=4|((jbits>>8)&3);

			disp11as=(jbits&511);
			dispasc=4|((jbits>>9)&3);
		}

		disp_ldop=(jbits>>12)&15;
	}

	if(jbits&0x01000000U)
	{
		disp5=((jbits&0x000FFFFFF)<<4)|((opw1   )&15);

		if(isxg3)
		{
			if(jbits&0x10000000U)
				disp5|=~0xFFFFFFFFULL;
			if(jbits&0x80000000U)
				disp5|=0x80000000U;
			if(jbits&0x08000000U)
				disp5|=0x40000000U;
			if(jbits&0x04000000U)
				disp5|=0x20000000U;
			if(opw1&0x0010)
				disp5|=0x10000000U;
		}else
		{
			if(opw1&0x0010)
				disp5|=0xF0000000;

#if 0
			if(jbits&0x80000000U)
				disp5^=0x80000000U;
			if(jbits&0x08000000U)
				disp5^=0x40000000U;
			if(jbits&0x04000000U)
				disp5^=0x20000000U;
			if(jbits&0x10000000U)
				disp5^=0x10000000U;
#endif

			disp5=(s32)disp5;
		}
		imm5=disp5;
		isjimm=1;
	}
	
	if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
	{
		imm20s&=~1;
	}
	
	if(isxg3)
	{
		imm20s=
			((rm_rvo   &63)<< 0) |
			((ro_rvo   &63)<< 6) |
			(((opw2>>8)&15)<<12) |
			((rn_rvo   &63)<<16) ;
		if(eq)
			imm20s|=(~0U)<<22;
		
		imm20s=(imm20s<<1)-2;
	}

	op->rn=rn_dfl;
	op->rm=rm_dfl;
	op->ro=ro_dfl;
//	op->imm=disp5;
	op->imm=imm5;

//	op->rp=BJX2_REG_ZZR;
	op->rp=rp_dfl;
	op->rq=BJX2_REG_GBR_HI;

	if((rn_dfl==BJX2_REG_ZZR) && (opw2>>12)<12)
	{
		ret=-1;
	}

	ret=0;
	switch((opw2>>12)&15)
	{
	case 0x0:	/* F0ez_0zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		op->ldsc=dispasc;
		op->ldop=disp_ldop;

		op->fl|=BJX2_OPFL_NOWEX;

		switch((opw2>>8)&15)
		{
#if 1
		case 0x0:	/* F0ez_00zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
				{
					op->rq=BJX2_REG_PC_HI;
					op->rm=BJX2_REG_PC;
					op->nmid=BJX2_NMID_LEAB;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEAB_LdPcDispReg;
					break;
				}

				if(op->rm==BJX2_REG_DHR)
					op->rm=BJX2_REG_GBR;

				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAB_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVB_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;

			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPB;
				op->Run=BJX2_Op_LDOPB_RegStRegDisp;
			}
			
			break;
		case 0x1:	/* F0ez_01zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					op->rm=BJX2_REG_GBR;
				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAW_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVW_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;

			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPW;
				op->Run=BJX2_Op_LDOPW_RegStRegDisp;
			}
			break;
		case 0x2:	/* F0ez_02zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					op->rm=BJX2_REG_GBR;
				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAD_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPL;
				op->Run=BJX2_Op_LDOPL_RegStRegDisp;
			}
			break;
		case 0x3:	/* F0ez_03zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					op->rm=BJX2_REG_GBR;
				op->nmid=BJX2_NMID_LEAQ;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAQ_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPQ;
				op->Run=BJX2_Op_LDOPQ_RegStRegDisp;
			}
			break;
#endif

		case 0x4:	/* F0ez_04zz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
				{
					op->rq=BJX2_REG_PC_HI;
					op->rm=BJX2_REG_PC;
					op->nmid=BJX2_NMID_LEAB;
					op->fmid=BJX2_FMID_LDREG2REG;
					op->Run=BJX2_Op_LEAB_LdPcIdxReg;
					break;
				}

				if(op->rm==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_GBR; }

				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAB_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEXSFX3W;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVB;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVB_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStReg2B;
				break;
			}

			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVB_RegStReg2;
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPB;
				op->Run=BJX2_Op_LDOPB_RegStReg2;
			}
			break;
		case 0x5:	/* F0ez_05zz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAW_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEXSFX3W;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVW;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVW_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStReg2B;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVW_RegStReg2;
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPW;
				op->Run=BJX2_Op_LDOPW_RegStReg2;
			}
			break;
		case 0x6:	/* F0ez_06zz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAD_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEXSFX3W;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVL;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVL_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStReg2B;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVL_RegStReg2;
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPL;
				op->Run=BJX2_Op_LDOPL_RegStReg2;
			}
			break;
		case 0x7:	/* F0ez_07zz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEAQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAQ_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEXSFX3W;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVQ;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVQ_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStReg2B;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVQ_RegStReg2;
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPQ;
				op->Run=BJX2_Op_LDOPQ_RegStReg2;
			}
			break;

#if 1
		case 0x8:	/* F0ez_08zz */
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVB_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdRegDispReg;
			}
			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPB;
				op->Run=BJX2_Op_LDOPB_LdRegDispReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_LDOPUB;
					op->Run=BJX2_Op_LDOPUB_LdRegDispReg;
				}
			}
			break;
		case 0x9:	/* F0ez_09zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVW_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdRegDispReg;
			}
			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPW;
				op->Run=BJX2_Op_LDOPW_LdRegDispReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_LDOPUW;
					op->Run=BJX2_Op_LDOPUW_LdRegDispReg;
				}
			}
			break;
		case 0xA:	/* F0ez_0Azz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			}
			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPL;
				op->Run=BJX2_Op_LDOPL_LdRegDispReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_LDOPUL;
					op->Run=BJX2_Op_LDOPUL_LdRegDispReg;
				}
			}
			break;
		case 0xB:	/* F0ez_0Bzz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(disp_ldop)
			{
				op->nmid=BJX2_NMID_LDOPQ;
				op->Run=BJX2_Op_LDOPQ_LdRegDispReg;
			}

			if(eq)
			{
				op->imm=disp11as;

				op->fl|=BJX2_OPFL_NOWEX;
//				op->fl|=BJX2_OPFL_NOWEXSFX;

//				op->nmid=BJX2_NMID_MOVDL;
				op->nmid=BJX2_NMID_LDTEX;
				op->fmid=BJX2_FMID_LDREG2REG;
//				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
				op->Run=BJX2_Op_LDTEX_LdReg2Reg;
			}
			break;
#endif

		case 0xC:	/* F0ez_0Czz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVB;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVB_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVUB;
						op->Run=BJX2_Op_MOVUB_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVB_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVB_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVB_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdReg2Reg;
			}
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPB;
				op->Run=BJX2_Op_LDOPB_LdReg2Reg;
				if(eq)
				{
					op->nmid=BJX2_NMID_LDOPUB;
					op->Run=BJX2_Op_LDOPUB_LdReg2Reg;
				}
			}
			break;
		case 0xD:	/* F0ez_0Dzz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVW;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVW_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVUW;
						op->Run=BJX2_Op_MOVUW_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVW_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVW_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVW_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdReg2Reg;
			}
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPW;
				op->Run=BJX2_Op_LDOPW_LdReg2Reg;
				if(eq)
				{
					op->nmid=BJX2_NMID_LDOPUW;
					op->Run=BJX2_Op_LDOPUW_LdReg2Reg;
				}
			}
			break;
		case 0xE:	/* F0ez_0Ezz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVL;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVL_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVUL;
						op->Run=BJX2_Op_MOVUL_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVL_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVL_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVL_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdReg2Reg;
			}
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPL;
				op->Run=BJX2_Op_LDOPL_LdReg2Reg;
				if(eq)
				{
					op->nmid=BJX2_NMID_LDOPUL;
					op->Run=BJX2_Op_LDOPUL_LdReg2Reg;
				}
			}
			break;
		case 0xF:	/* F0ez_0Fzz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;

			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVQ;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVQ_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVDL;
						op->Run=BJX2_Op_MOVUL_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVDL;
					op->Run=BJX2_Op_MOVUL_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVQ_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdReg2Reg;
			}
			if(disp_ldop || dispasc)
			{
				op->nmid=BJX2_NMID_LDOPQ;
				op->Run=BJX2_Op_LDOPQ_LdReg2Reg;
			}
			break;
		}
		break;

	case 0x1:	/* F0ez_1zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_10zz */
//			if(eq)
//				break;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADD_RegRegReg;
			
			if(ro_dfl==BJX2_REG_ZZR)
			{
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
			}
			
			if(eq)
			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_ADD_RegImmReg;
				op->nmid=BJX2_NMID_ADDX;
				op->Run=BJX2_Op_ADDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;

				if(ro_dfl==BJX2_REG_ZZR)
				{
					op->nmid=BJX2_NMID_MOVX2;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_MOVX_RegReg;
				}				
			}
			break;
		case 0x1:	/* F0ez_11zz */
//			if(eq)
//				break;
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUB_RegRegReg;
			if(eq)
			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_SUB_RegImmReg;
				op->nmid=BJX2_NMID_SUBX;
				op->Run=BJX2_Op_SUBX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x2:	/* F0ez_12zz */
//			if(eq)
//				break;

			op->nmid=BJX2_NMID_MULS;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULS_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_MULQ))
				{
					op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_MULSQ;
				op->Run=BJX2_Op_MULSQ_RegRegReg;
			}

//			if(eq)
//			{
//				op->nmid=BJX2_NMID_MULU;
//				op->Run=BJX2_Op_MULU_RegRegReg;
//			}

//			if(eq)
//			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_MULS_RegImmReg;
//			}
			break;
		case 0x3:	/* F0ez_13zz */
//			if(eq)
//				break;

			op->nmid=BJX2_NMID_MULU;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULU_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_MULQ))
				{
					op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_MULUQ;
				op->Run=BJX2_Op_MULUQ_RegRegReg;
			}

//			if(eq)
//			{
//				op->nmid=BJX2_NMID_MULU;
//				op->Run=BJX2_Op_MULU_RegRegReg;
//			}

//			if(eq)
//			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_MULS_RegImmReg;
//			}
			break;

		case 0x5:	/* F0ez_15zz */
//			if(eq)
//				break;
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_AND_RegRegReg;
			if(eq)
			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_AND_RegImmReg;
				op->nmid=BJX2_NMID_ANDX;
				op->Run=BJX2_Op_ANDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x6:	/* F0ez_16zz */
//			if(eq)
//				break;
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_OR_RegRegReg;
			if(eq)
			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_OR_RegImmReg;
				op->nmid=BJX2_NMID_ORX;
				op->Run=BJX2_Op_ORX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x7:	/* F0ez_17zz */
//			if(eq)
//				break;
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_XOR_RegRegReg;
			if(eq)
			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_XOR_RegImmReg;
				op->nmid=BJX2_NMID_XORX;
				op->Run=BJX2_Op_XORX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;

		case 0x8:	/* F0ez_18zz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_18nm */
				op->nmid=BJX2_NMID_BNDCHKB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCHKB_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCHKB_ImmReg;
				}
				break;
			case 0x1:	/* F0e1_18nm */
				op->nmid=BJX2_NMID_BNDCHKW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCHKW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCHKW_ImmReg;
				}
				break;
			case 0x2:	/* F0e2_18nm */
				op->nmid=BJX2_NMID_BNDCHKL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCHKL_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCHKL_ImmReg;
				}
				break;
			case 0x3:	/* F0e3_18nm */
				op->nmid=BJX2_NMID_BNDCHKQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCHKQ_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCHKQ_ImmReg;
				}
				break;

#if 1
			case 0x4:	/* F0e4_18nm */
				op->imm=imm6;
				op->nmid=BJX2_NMID_BNDCHKB;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_BNDCHKB_ImmReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
				}
				break;
			case 0x5:	/* F0e5_18nm */
				op->imm=imm6;
				op->nmid=BJX2_NMID_BNDCMPB;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_BNDCMPB_ImmReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
				}
				break;
#endif

#if 0
			case 0x4:	/* F0e4_18nm */
//				op->nmid=BJX2_NMID_BNDCHKB;
//				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_BNDCHKB_RegReg;
//				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
				}
				break;
			case 0x5:	/* F0e5_18nm */
				op->nmid=BJX2_NMID_BNDCMP;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCMP_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_CTRLF;
				if(eq)
				{
				}
				break;
#endif

			case 0x6:	/* F0e6_18nm */
				op->nmid=BJX2_NMID_VSKG;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_VSKG_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
				}
				break;
			case 0x7:	/* F0e7_18nm */
				op->nmid=BJX2_NMID_VSKC;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_VSKC_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_CTRLF;
				if(eq)
				{
				}
				break;


			case 0x8:	/* F0e8_18nm */
				op->nmid=BJX2_NMID_BCDADC;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BCDADC_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
				}
				break;
			case 0x9:	/* F0e9_18nm */
				op->nmid=BJX2_NMID_BCDSBB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BCDSBB_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
				}
				break;

			case 0xA:	/* F0eA_18zz */
//				op->rn=rn_dfl;
//				op->rm=rm_dfl;
//				op->ro=ro_dfl;
				op->ro=BJX2_REG_ZZR;
				op->imm=0;

//				op->nmid=BJX2_NMID_MOVTT;
				op->nmid=BJX2_NMID_MOVZT;
				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_MOVTT_RegRegReg;
				op->Run=BJX2_Op_MOVZT_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->nmid=BJX2_NMID_XMOVTT;
					op->nmid=BJX2_NMID_XMOVZT;
					op->Run=BJX2_Op_XMOVTT_RegImmReg;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
				break;

			case 0xB:	/* F0eB_18nm */
				op->nmid=BJX2_NMID_SNIPEDC;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SNIPEDC_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_SNIPEIC;
					op->Run=BJX2_Op_SNIPEIC_RegReg;
				}
				break;

#if 1
			case 0xC:	/* F0eC_18nm */
				op->nmid=BJX2_NMID_CMPTAEQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPTAEQ_RegReg;

//				op->nmid=BJX2_NMID_CMPEQ;
//				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_CMPEQ_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPXEQ;
					op->Run=BJX2_Op_CMPXEQ_RegReg;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
				break;
			case 0xD:	/* F0eD_18nm */
				op->nmid=BJX2_NMID_CMPTAHI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPTAHI_RegReg;

//				op->nmid=BJX2_NMID_CMPHI;
//				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_CMPHI_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPXHI;
					op->Run=BJX2_Op_CMPXHI_RegReg;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
				break;
			case 0xE:	/* F0eE_18nm */
				op->nmid=BJX2_NMID_CMPTAHS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPTAHS_RegReg;

//				op->nmid=BJX2_NMID_CMPGT;
//				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_CMPGT_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPXGT;
					op->Run=BJX2_Op_CMPXGT_RegReg;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
				break;
#endif

#if 1
			case 0xF:	/* F0eF_18nm */
				op->nmid=BJX2_NMID_CONVFXI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CONVFXI_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CONVFLI;
					op->Run=BJX2_Op_CONVFLI_RegReg;
				}
				break;
#endif

#if 0
			case 0x0:	/* F0e0_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVB_RegStReg;
				break;
			case 0x1:	/* F0e1_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVW_RegStReg;
				break;
			case 0x2:	/* F0e2_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVL_RegStReg;
				break;
			case 0x3:	/* F0e3_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_MOVQ_RegStReg;
				break;
			case 0x4:	/* F0e4_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVB_RegStDrReg;
				break;
			case 0x5:	/* F0e5_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVW_RegStDrReg;
				break;
			case 0x6:	/* F0e6_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVL_RegStDrReg;
				break;
			case 0x7:	/* F0e7_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_MOVQ_RegStDrReg;
				break;
			case 0x8:	/* F0e8_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVB_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdRegReg;		}
				break;
			case 0x9:	/* F0e9_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVW_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdRegReg;		}
				break;
			case 0xA:	/* F0eA_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVL_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdRegReg;		}
				break;
			case 0xB:	/* F0eB_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_MOVQ_LdRegReg;
				break;
			case 0xC:	/* F0eC_18zz */
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVB_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdDrRegReg;		}
				break;
			case 0xD:	/* F0eD_18zz */
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVW_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdDrRegReg;		}
				break;
			case 0xE:	/* F0eE_18zz */
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVL_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdDrRegReg;		}
				break;
			case 0xF:	/* F0eF_18zz */
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_MOVQ_LdDrRegReg;
				break;
#endif
			}
			break;
		case 0x9:	/* F0ez_19zz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_19nm */
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADD_RegReg;
				break;
			case 0x1:	/* F0e1_19nm */
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SUB_RegReg;
				break;
			case 0x2:	/* F0e2_19nm */
				op->nmid=BJX2_NMID_ADC;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADC_RegReg;
//				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x3:	/* F0e3_19nm */
				op->nmid=BJX2_NMID_SBB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SBB_RegReg;
//				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x4:	/* F0e4_19nm */
				op->nmid=BJX2_NMID_TST;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_TST_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_TSTQ;
					op->Run=BJX2_Op_TSTQ_RegReg;
				}
				break;
			case 0x5:	/* F0e5_19nm */
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_AND_RegReg;
				break;
			case 0x6:	/* F0e6_19nm */
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_OR_RegReg;
				break;
			case 0x7:	/* F0e7_19nm */
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_XOR_RegReg;
				break;
			case 0x8:	/* F0e8_19nm */
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				if(eq)
				{
					if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
							BJX2_EXTID_MOVX2))
					{
						op->Run=NULL;
						break;
					}

//					op->rm=rm_x;
//					op->rn=rn_x;
				
//					op->nmid=BJX2_NMID_MOVD;
//					op->Run=BJX2_Op_MOVD_RegReg;
					op->nmid=BJX2_NMID_MOVX2;
					op->Run=BJX2_Op_MOVX_RegReg;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
				break;
#if 0
			case 0x9:	/* F0e9_19nm */
				op->nmid=BJX2_NMID_MULS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MULS_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#endif
			case 0xA:	/* F0eA_19nm */
				if(eq)
				{
					op->rm=rn_dfl;
					op->rn=rm_dfl;
//					op->nmid=BJX2_NMID_MOVD;
//					op->Run=BJX2_Op_MOV_RegReg;
					op->nmid=BJX2_NMID_INVDC;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_SETTRIP_RegStReg;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				}

//				op->rn+=BJX2_REG_PC;
//				op->rn=BJX2_REG_PC+(op->rn&15);
				op->rn=cn_dfl;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOVC_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_CTRLF;

				if(isuser)
				{
					if((0xFFFFFF3C>>(cn_dfl&31))&1)
					{
						op->Run=BJX2_Op_EMUTRAP_RegRegReg;
						op->fl|=BJX2_OPFL_CTRLF;
					}
				}
				break;
			case 0xB:	/* F0eB_19nm */
				if(eq)
				{
					op->imm=imm6;
					op->nmid=BJX2_NMID_CMPTTEQ;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPTTEQ_ImmReg;
					break;
				}

//				op->rm+=BJX2_REG_PC;
//				op->rm=BJX2_REG_PC+(op->rm&15);
				op->rm=cm_dfl;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOVC_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isuser)
				{
					if((0xFFFFFF38>>(cm_dfl&31))&1)
					{
						op->Run=BJX2_Op_EMUTRAP_RegRegReg;
						op->fl|=BJX2_OPFL_CTRLF;
					}
				}

//				if(eq)
//				{
//					op->nmid=BJX2_NMID_MOVD;
//					op->Run=BJX2_Op_MOV_RegReg;
//				}
				break;
			case 0xC:	/* F0eC_19nm */
				op->nmid=BJX2_NMID_CMPEQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPEQ_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQEQ;
					op->Run=BJX2_Op_CMPQEQ_RegReg;
				}
				break;
			case 0xD:	/* F0eD_19nm */
				op->nmid=BJX2_NMID_CMPHI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHI_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQHI;
					op->Run=BJX2_Op_CMPQHI_RegReg;
				}
				break;
			case 0xE:	/* F0eE_19nm */
				op->nmid=BJX2_NMID_CMPGT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGT_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQGT;
					op->Run=BJX2_Op_CMPQGT_RegReg;
				}
				break;
#if 0
			case 0xF:	/* F0eF_19nm */
				op->nmid=BJX2_NMID_MULU;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MULU_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#endif
			}
			break;
		case 0xA:	/* F0ez_1Azz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{

			case 0x0:	/* F0e0_1Anm */
				if(eq)
				{
					break;
				}
				op->nmid=BJX2_NMID_PCVTHTOF8;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTHTOF8_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x1:	/* F0e1_1Anm */
				if(eq)
				{
					break;
				}
				op->nmid=BJX2_NMID_PCVTF8TOH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTF8TOH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x2:	/* F0e2_1Anm */
				if(eq)
				{
					break;
				}
				op->nmid=BJX2_NMID_PCVTH2F8A;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTH2F8A_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x3:	/* F0e3_1Anm */
				if(eq)
				{
					break;
				}
				op->nmid=BJX2_NMID_PCVTF8A2H;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTF8A2H_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

#if 0
			case 0x0:	/* F0e0_1Anm */
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_ADD_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_PADDL;
					op->Run=BJX2_Op_PADDL_RegReg;
				}else
				{
					op->nmid=BJX2_NMID_PADDW;
					op->Run=BJX2_Op_PADDW_RegReg;
				}
				break;
			case 0x1:	/* F0e1_1Anm */
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SUB_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_PSUBL;
					op->Run=BJX2_Op_PSUBL_RegReg;
				}else
				{
					op->nmid=BJX2_NMID_PSUBW;
					op->Run=BJX2_Op_PSUBW_RegReg;
				}
				break;
			case 0x2:	/* F0e2_1Anm */
				break;
			case 0x3:	/* F0e3_1Anm */
				break;
			case 0x4:	/* F0e4_1Anm */
				break;
			case 0x5:	/* F0e5_1Anm */
				op->nmid=BJX2_NMID_PADDH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PADDH_RegReg;
				break;
			case 0x6:	/* F0e6_1Anm */
				op->nmid=BJX2_NMID_PSUBH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSUBH_RegReg;
				break;
			case 0x7:	/* F0e7_1Anm */
				op->nmid=BJX2_NMID_PMULH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PMULH_RegReg;
				break;
			case 0x8:	/* F0e8_1Anm */
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVLD;
					op->Run=BJX2_Op_MOVLD_RegReg;
				}else
				{
					op->nmid=BJX2_NMID_MOVHD;
					op->Run=BJX2_Op_MOVHD_RegReg;
				}
				break;
			case 0xA:	/* F0eA_1Anm */
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVD;
					op->Run=BJX2_Op_MOV_RegReg;
				}
				break;
#endif

			case 0x9:	/* F0e9_1Anm */
				break;
			case 0xA:	/* F0eA_1Anm */
				op->nmid=BJX2_NMID_PCMPEQH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCMPEQW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCMPEQF;
					op->Run=BJX2_Op_PCMPEQL_RegReg;
				}
				break;
			case 0xB:	/* F0eB_1Anm */
				op->nmid=BJX2_NMID_PCMPGTH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCMPGTH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCMPGTF;
					op->Run=BJX2_Op_PCMPGTF_RegReg;
				}
				break;
			case 0xC:	/* F0eC_1Anm */
				op->nmid=BJX2_NMID_PCMPEQW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCMPEQW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCMPEQL;
					op->Run=BJX2_Op_PCMPEQL_RegReg;
				}
				break;
			case 0xD:	/* F0eD_1Anm */
				op->nmid=BJX2_NMID_PCMPHIW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCMPHIW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCMPHIL;
					op->Run=BJX2_Op_PCMPHIL_RegReg;
				}
				break;
			case 0xE:	/* F0eE_1Anm */
				op->nmid=BJX2_NMID_PCMPGTW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCMPGTW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCMPGTL;
					op->Run=BJX2_Op_PCMPGTL_RegReg;
				}
				break;
			case 0xF:	/* F0eF_1Anm */
				op->nmid=BJX2_NMID_BSWAPUL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BSWAPUL_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_BSWAPQ;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_BSWAPQ_RegReg;
				}
				break;

//				op->fmid=BJX2_FMID_REGREG;
//				op->nmid=BJX2_NMID_PMULUW;
//				op->Run=BJX2_Op_PMULUW_RegReg;
				break;
			}
			break;
		case 0xB:	/* F0ez_1Bzz */
			op->ro=BJX2_REG_ZZR;
#if 0
			op->fl|=BJX2_OPFL_NOWEX;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Bzz */
				op->nmid=BJX2_NMID_CMOVTB;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_CMOVTB_RegStReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFB;
					op->Run=BJX2_Op_CMOVFB_RegStReg;
				}
				break;
			case 0x1:	/* F0e1_1Bzz */
				op->nmid=BJX2_NMID_CMOVTW;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_CMOVTW_RegStReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFW;
					op->Run=BJX2_Op_CMOVFW_RegStReg;
				}
				break;
			case 0x2:	/* F0e2_1Bzz */
				op->nmid=BJX2_NMID_CMOVTL;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_CMOVTL_RegStReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFL;
					op->Run=BJX2_Op_CMOVFL_RegStReg;
				}
				break;
			case 0x3:	/* F0e3_1Bzz */
				op->nmid=BJX2_NMID_CMOVTQ;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_CMOVTQ_RegStReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFQ;
					op->Run=BJX2_Op_CMOVFQ_RegStReg;
				}
				break;
			case 0x4:	/* F0e4_1Bzz */
				op->nmid=BJX2_NMID_CMOVTB;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_CMOVTB_RegStDrReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFB;
					op->Run=BJX2_Op_CMOVFB_RegStDrReg;
				}
				break;
			case 0x5:	/* F0e5_1Bzz */
				op->nmid=BJX2_NMID_CMOVTW;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_CMOVTW_RegStDrReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFW;
					op->Run=BJX2_Op_CMOVFW_RegStDrReg;
				}
				break;
			case 0x6:	/* F0e6_1Bzz */
				op->nmid=BJX2_NMID_CMOVTL;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_CMOVTL_RegStDrReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFL;
					op->Run=BJX2_Op_CMOVFL_RegStDrReg;
				}
				break;
			case 0x7:	/* F0e7_1Bzz */
				op->nmid=BJX2_NMID_CMOVTQ;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_CMOVTQ_RegStDrReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_CMOVFQ;
					op->Run=BJX2_Op_CMOVFQ_RegStDrReg;
				}
				break;
			case 0x8:	/* F0e8_1Bzz */
				op->nmid=BJX2_NMID_CMOVTB;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_CMOVTB_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_CMOVUTB;
					op->Run=BJX2_Op_CMOVUTB_LdRegReg;		}
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFB;
					op->Run=BJX2_Op_CMOVFB_LdRegReg;	}
				if(eo && eq)
				{	op->nmid=BJX2_NMID_CMOVUFB;
					op->Run=BJX2_Op_CMOVUFB_LdRegReg;	}
				break;
			case 0x9:	/* F0e9_1Bzz */
				op->nmid=BJX2_NMID_CMOVTW;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_CMOVTW_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_CMOVUTW;
					op->Run=BJX2_Op_CMOVUTW_LdRegReg;		}
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFW;
					op->Run=BJX2_Op_CMOVFW_LdRegReg;	}
				if(eo && eq)
				{	op->nmid=BJX2_NMID_CMOVUFW;
					op->Run=BJX2_Op_CMOVUFW_LdRegReg;	}
				break;
			case 0xA:	/* F0eA_1Bzz */
				op->nmid=BJX2_NMID_CMOVTL;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_CMOVTL_LdRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_CMOVUTL;
					op->Run=BJX2_Op_CMOVUTL_LdRegReg;		}
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFL;
					op->Run=BJX2_Op_CMOVFL_LdRegReg;	}
				if(eo && eq)
				{	op->nmid=BJX2_NMID_CMOVUFL;
					op->Run=BJX2_Op_CMOVUFL_LdRegReg;	}
				break;
			case 0xB:	/* F0eB_1Bzz */
				op->nmid=BJX2_NMID_CMOVTQ;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_CMOVTQ_LdRegReg;
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFQ;
					op->Run=BJX2_Op_CMOVFQ_LdRegReg;	}
				break;
			case 0xC:	/* F0eC_1Bzz */
				op->nmid=BJX2_NMID_CMOVTB;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_CMOVTB_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_CMOVUTB;
					op->Run=BJX2_Op_CMOVUTB_LdDrRegReg;		}
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFB;
					op->Run=BJX2_Op_CMOVFB_LdDrRegReg;	}
				if(eo && eq)
				{	op->nmid=BJX2_NMID_CMOVUFB;
					op->Run=BJX2_Op_CMOVUFB_LdDrRegReg;	}
				break;
			case 0xD:	/* F0eD_1Bzz */
				op->nmid=BJX2_NMID_CMOVTW;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_CMOVTW_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_CMOVUTW;
					op->Run=BJX2_Op_CMOVUTW_LdDrRegReg;		}
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFW;
					op->Run=BJX2_Op_CMOVFW_LdDrRegReg;	}
				if(eo && eq)
				{	op->nmid=BJX2_NMID_CMOVUFW;
					op->Run=BJX2_Op_CMOVUFW_LdDrRegReg;	}
				break;
			case 0xE:	/* F0eE_1Bzz */
				op->nmid=BJX2_NMID_CMOVTL;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_CMOVTL_LdDrRegReg;
				if(eq)
				{	op->nmid=BJX2_NMID_CMOVUTL;
					op->Run=BJX2_Op_CMOVUTL_LdDrRegReg;		}
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFL;
					op->Run=BJX2_Op_CMOVFL_LdDrRegReg;	}
				if(eo && eq)
				{	op->nmid=BJX2_NMID_CMOVUFL;
					op->Run=BJX2_Op_CMOVUFL_LdDrRegReg;	}
				break;
			case 0xF:	/* F0eF_1Bzz */
				op->nmid=BJX2_NMID_CMOVTQ;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_CMOVTQ_LdDrRegReg;
				if(eo)
				{	op->nmid=BJX2_NMID_CMOVFQ;
					op->Run=BJX2_Op_CMOVFQ_LdDrRegReg;	}
				break;
			}
#endif
			break;
		case 0xC:	/* F0ez_1Czz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Cnm */
				op->nmid=BJX2_NMID_NOT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_NOT_RegReg;
				break;
			case 0x1:	/* F0e1_1Cnm */
				op->nmid=BJX2_NMID_NEG;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_NEG_RegReg;
				break;

			case 0x2:	/* F0e2_1Cnm */
				op->nmid=BJX2_NMID_CLZ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CLZ_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CLZQ;
					op->Run=BJX2_Op_CLZQ_RegReg;
				}
				break;

			case 0x5:	/* F0e5_1Cnm */
				op->nmid=BJX2_NMID_EXTSL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_EXTSL_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_EXTUL;
					op->Run=BJX2_Op_EXTUL_RegReg;
				}
				break;
			case 0x6:	/* F0e6_1Cnm */
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHAD_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHADQ;
					op->Run=BJX2_Op_SHADQ_RegReg;
				}
				break;
			case 0x7:	/* F0e7_1Cnm */
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SHLD_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHLDQ;
					op->Run=BJX2_Op_SHLDQ_RegReg;
				}
				break;

#if 1
			case 0x8:	/* F0e8_1Cnm */
				op->nmid=BJX2_NMID_EXTSB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_EXTSB_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_EXTUB;
					op->Run=BJX2_Op_EXTUB_RegReg;
				}
				break;
			case 0x9:	/* F0e9_1Cnm */
				if(eo)
					break;
				op->nmid=BJX2_NMID_EXTSW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_EXTSW_RegReg;
				if(eo)
				{
					op->nmid=BJX2_NMID_EXTSL;
					op->Run=BJX2_Op_EXTSL_RegReg;
				}
				if(eq)
				{
					op->nmid=BJX2_NMID_EXTUW;
					op->Run=BJX2_Op_EXTUW_RegReg;

					if(eo)
					{
						op->nmid=BJX2_NMID_EXTUL;
						op->Run=BJX2_Op_EXTUL_RegReg;
					}
				}
				break;

			case 0xA:	/* F0eA_1Cnm */
//				op->rn+=BJX2_REG_R0B;
//				op->rn+=BJX2_REG_R32;
				op->rn=BJX2_REG_R32+(op->rn&31);

				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0xB:	/* F0eB_1Cnm */
//				op->rm+=BJX2_REG_R0B;
//				op->rm+=BJX2_REG_R32;
				op->rm=BJX2_REG_R32+(op->rm&31);
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#endif

#if 1
			case 0xE:	/* F0eE_1Cnm */
				op->nmid=BJX2_NMID_CMPGE;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGE_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQGE;
					op->Run=BJX2_Op_CMPQGE_RegReg;
				}
				break;
			case 0xF:	/* F0eF_1Cnm */
				op->nmid=BJX2_NMID_CMPHS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHS_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQHS;
					op->Run=BJX2_Op_CMPQHS_RegReg;
				}
				break;
#endif
			}
			break;
		case 0xD:	/* F0ez_1Dzz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
//			case 0x0:	/* F0e0_1Dnm */
//				break;

#if 0
			case 0x6:	/* F0e6_1Dnm */
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_SHAD_RegDrReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHADQ;
					op->Run=BJX2_Op_SHADQ_RegDrReg;
				}
				break;
			case 0x7:	/* F0e7_1Dnm */
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_SHLD_RegDrReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_SHLDQ;
					op->Run=BJX2_Op_SHLDQ_RegDrReg;
				}
				break;
#endif

#if 0
			case 0x8:	/* F0e8_1Dnm */
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_ADD_RegDrReg;
				break;
			case 0x9:	/* F0e9_1Dnm */
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_SUB_RegDrReg;
				break;
			case 0xA:	/* F0eA_1Dnm */
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_AND_RegDrReg;
				break;
			case 0xB:	/* F0eB_1Dnm */
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_OR_RegDrReg;
				break;
			case 0xC:	/* F0eC_1Dnm */
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGDRREG;
				op->Run=BJX2_Op_XOR_RegDrReg;
				break;
#endif

#if 0
			case 0xD:	/* F0eD_1Dnm */
				op->nmid=BJX2_NMID_CMPHS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPHS_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQHS;
					op->Run=BJX2_Op_CMPQHS_RegReg;
				}
				break;
			case 0xE:	/* F0eE_1Dnm */
				op->nmid=BJX2_NMID_CMPGE;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPGE_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPQGE;
					op->Run=BJX2_Op_CMPQGE_RegReg;
				}
				break;
#endif

			case 0x0:	/* F0e0_1Dnm */
				op->nmid=BJX2_NMID_FLDCF;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCF_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x1:	/* F0e1_1Dnm */
				op->nmid=BJX2_NMID_FLDCFH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCFH_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x2:	/* F0e2_1Dnm */
				op->nmid=BJX2_NMID_FLDCI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCI_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x3:	/* F0e3_1Dnm */
				op->nmid=BJX2_NMID_FLDCH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCH_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x4:	/* F0e4_1Dnm */
				op->nmid=BJX2_NMID_FSTCF;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSTCF_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#if 0
			case 0x5:	/* F0e5_1Dnm */
				op->nmid=BJX2_NMID_FSTCFH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSTCFH_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#endif
			case 0x6:	/* F0e6_1Dnm */
				op->nmid=BJX2_NMID_FSTCI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSTCI_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x7:	/* F0e7_1Dnm */
				op->nmid=BJX2_NMID_FSTCH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSTCH_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x8:	/* F0e8_1Dnm */
				op->nmid=BJX2_NMID_FNEG;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FNEG_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x9:	/* F0e9_1Dnm */
				op->nmid=BJX2_NMID_FABS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FABS_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0xA:	/* F0eA_1Dnm */
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FCMPEQ_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_FCMPXEQ;
					op->Run=BJX2_Op_FCMPXEQ_GRegReg;
				}
				break;
			case 0xB:	/* F0eB_1Dnm */
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FCMPGT_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_FCMPXGT;
					op->Run=BJX2_Op_FCMPXGT_GRegReg;
				}
				break;

			case 0xC:	/* F0eC_1Dnm */
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_FDIV))
				{
					op->nmid=BJX2_NMID_FSQRT;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_EMUTRAP_RegRegReg;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;

	//				op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_FSQRT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSQRT_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_FSQRTX;
					op->Run=BJX2_Op_FSQRTX_GRegReg;
				}
				break;

			case 0xD:	/* F0eD_1Dnm */
				op->nmid=BJX2_NMID_FSQRTA;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSQRTA_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_FSQRTXA;
					op->Run=BJX2_Op_FSQRTXA_RegReg;
				}
				break;

			case 0xE:	/* F0eE_1Dnm */
				op->nmid=BJX2_NMID_FCMPGE;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FCMPGE_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->nmid=BJX2_NMID_FCMPXGE;
//					op->Run=BJX2_Op_FCMPXGE_GRegReg;
				}
				break;

			case 0xF:	/* F0eF_1Dnm */
				op->nmid=BJX2_NMID_FLDCIU;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCIU_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		case 0xE:	/* F0ez_1Ezz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Enm */
				op->nmid=BJX2_NMID_RGBSHR1;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_RGBSHR1_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB5MINMAX;
					op->Run=BJX2_Op_RGB5MINMAX_RegReg;
					op->fl|=BJX2_OPFL_NOWEX;
				}
				break;

			case 0x1:	/* F0e1_1Enm */
				op->nmid=BJX2_NMID_PMORTL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PMORTL_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_PMORTQ;
					op->Run=BJX2_Op_PMORTQ_RegReg;
				}
				break;

			case 0x2:	/* F0e2_1Enm */
//				op->nmid=BJX2_NMID_RGBPCK;
				op->nmid=BJX2_NMID_RGB5PCK32;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_RGB5PCK32_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB5PCK64;
					op->Run=BJX2_Op_RGB5PCK64_RegReg;
				}
				break;
			case 0x3:	/* F0e3_1Enm */
//				op->nmid=BJX2_NMID_RGBPCK;
				op->nmid=BJX2_NMID_RGB5UPCK32;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_RGB5UPCK32_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB5UPCK64;
					op->Run=BJX2_Op_RGB5UPCK64_RegReg;
				}
				break;

			case 0x4:	/* F0e4_1Enm */
//				op->nmid=BJX2_NMID_RGBPCK;
				op->nmid=BJX2_NMID_RGB5SH3;
				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_RGB5PCK32_RegReg;
				op->Run=BJX2_Op_RGB5SH3_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB32PCK64;
					op->Run=BJX2_Op_RGB32PCK64_RegReg;
				}
				break;
			case 0x5:	/* F0e5_1Enm */
//				op->nmid=BJX2_NMID_RGBPCK;
				op->nmid=BJX2_NMID_RGB5USH3;
				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_RGB5UPCK32_RegReg;
				op->Run=BJX2_Op_RGB5USH3_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB32UPCK64;
					op->Run=BJX2_Op_RGB32UPCK64_RegReg;
				}
				break;

			case 0x6:	/* F0e6_1Enm */
				op->nmid=BJX2_NMID_PSHALW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSHALW_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_PSHLLW;
					op->Run=BJX2_Op_PSHLLW_RegReg;
				}
				break;
			case 0x7:	/* F0e7_1Enm */
				op->nmid=BJX2_NMID_PSHARW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSHARW_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_PSHLRW;
					op->Run=BJX2_Op_PSHLRW_RegReg;
				}
				break;

			case 0x8:	/* F0e8_1Enm */
				op->nmid=BJX2_NMID_PLDCM8SH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PLDCM8SH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PLDCM8UH;
					op->Run=BJX2_Op_PLDCM8UH_RegReg;
				}
				break;

			case 0x9:	/* F0e9_1Enm */
//				op->nmid=BJX2_NMID_PSTCM8SH;
//				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_PSTCM8SH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PLDCXH;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_PLDCXH_RegReg;
				}
				break;

			case 0xA:	/* F0eA_1Enm */
				op->nmid=BJX2_NMID_PSTCM8SH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSTCM8SH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PSTCM8UH;
					op->Run=BJX2_Op_PSTCM8UH_RegReg;
				}
				break;

			case 0xB:	/* F0eB_1Enm */
//				op->nmid=BJX2_NMID_PSTCM8SH;
//				op->fmid=BJX2_FMID_REGREG;
//				op->Run=BJX2_Op_PSTCM8SH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->fmid=BJX2_FMID_REGREG;
					op->nmid=BJX2_NMID_PSTCXH;
					op->Run=BJX2_Op_PSTCXH_RegReg;
				}
				break;

			case 0xC:	/* F0eC_1Enm */
				op->nmid=BJX2_NMID_PLDCH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PLDCH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PLDCHH;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_PLDCHH_RegReg;
//					op->nmid=BJX2_NMID_PLDCHX;
//					op->Run=BJX2_Op_PLDCHX_RegReg;
				}
				break;

			case 0xD:	/* F0eD_1Enm */
				op->nmid=BJX2_NMID_PLDCH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PLDCEHL_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PLDCHH;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_PLDCEHH_RegReg;
				}
				break;

#if 0
			case 0xD:	/* F0eD_1Enm */
				op->nmid=BJX2_NMID_PLDCHH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PLDCHH_RegReg;
				if(eq)
				{
//					op->nmid=BJX2_NMID_PLDCHX;
//					op->Run=BJX2_Op_PLDHX_RegReg;
				}
				break;
#endif
			case 0xE:	/* F0eE_1Enm */
				if(eq)
				{
					op->nmid=BJX2_NMID_BSWAPQ;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_BSWAPQ_RegReg;
					break;
				}
				op->nmid=BJX2_NMID_PSTCH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSTCH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
//				if(eq)
//				{
//					op->nmid=BJX2_NMID_PSTCHX;
//					op->Run=BJX2_Op_PSTCHX_RegReg;
//				}
				break;

#if 0
			case 0xF:	/* F0eF_1Enm */
				op->nmid=BJX2_NMID_PSTCHH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSTCHH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->nmid=BJX2_NMID_PLDCHX;
//					op->Run=BJX2_Op_PLDHX_RegReg;
				}
				break;
#endif

			case 0xF:	/* F0eF_1Enm */
				op->ro=BJX2_REG_ZZR;
				op->imm=0;

				op->nmid=BJX2_NMID_MOVST;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOVST_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_XMOVST;
					op->Run=BJX2_Op_XMOVST_RegReg;
				}
				break;
			}
			break;
		case 0xF:	/* F0ez_1Fzz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Fnm */
				op->nmid=BJX2_NMID_PCVTSB2HL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTSB2HL_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTSB2HL;
					op->Run=BJX2_Op_PCVTUB2HL_RegReg;
				}
				break;
			case 0x1:	/* F0e1_1Fnm */
				op->nmid=BJX2_NMID_PCVTSB2HH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTSB2HH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTSB2HH;
					op->Run=BJX2_Op_PCVTUB2HH_RegReg;
				}
				break;
			case 0x2:	/* F0e2_1Fnm */
				op->nmid=BJX2_NMID_PCVTSW2FL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTSW2FL_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTUW2FL;
					op->Run=BJX2_Op_PCVTUW2FL_RegReg;
				}
				break;
			case 0x3:	/* F0e3_1Fnm */
				op->nmid=BJX2_NMID_PCVTSW2FH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTSW2FH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTUW2FH;
					op->Run=BJX2_Op_PCVTUW2FH_RegReg;
				}
				break;

			case 0x4:	/* F0e4_1Fnm */
				op->nmid=BJX2_NMID_PCVTH2SB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTH2SB_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTH2UB;
					op->Run=BJX2_Op_PCVTH2UB_RegReg;
				}
				break;
			case 0x5:	/* F0e5_1Fnm */
				op->nmid=BJX2_NMID_PCVTSW2H;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTSW2H_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTUW2H;
					op->Run=BJX2_Op_PCVTUW2H_RegReg;
				}
				break;
			case 0x6:	/* F0e6_1Fnm */
				op->nmid=BJX2_NMID_PCVTF2SW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTF2SW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTF2UW;
					op->Run=BJX2_Op_PCVTF2UW_RegReg;
				}
				break;
			case 0x7:	/* F0e7_1Fnm */
				op->nmid=BJX2_NMID_PCVTH2SW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTH2SW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PCVTH2UW;
					op->Run=BJX2_Op_PCVTH2UW_RegReg;
				}
				break;

			case 0x8:	/* F0e8_1Fnm */
				op->nmid=BJX2_NMID_PSQRTAH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSQRTAH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PSQRTUAH;
					op->Run=BJX2_Op_PSQRTUAH_RegReg;
				}
				break;
			case 0x9:	/* F0e9_1Fnm */
				op->nmid=BJX2_NMID_PSQRTAF;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PSQRTAF_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PSQRTUAF;
					op->Run=BJX2_Op_PSQRTUAF_RegReg;
				}
				break;
			case 0xA:	/* F0eA_1Fnm */
				op->nmid=BJX2_NMID_PRCPAH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PRCPAH_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PRELUH;
					op->Run=BJX2_Op_PRELUH_RegReg;
				}
				break;
			case 0xB:	/* F0eB_1Fnm */
				op->nmid=BJX2_NMID_PRCPAF;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PRCPAF_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_PRELUF;
					op->Run=BJX2_Op_PRELUF_RegReg;
				}
				break;

			case 0xC:	/* F0eC_1Fnm */
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB5PCKI8;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_RGB5PCKI8_RegReg;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				}
				op->nmid=BJX2_NMID_PCVTH2AL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTH2AL_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0xD:	/* F0eD_1Fnm */
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB5UPCKI8;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_RGB5UPCKI8_RegReg;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				}
				op->nmid=BJX2_NMID_PCVTAL2H;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_PCVTAL2H_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0xE:	/* F0eE_1Fnm */
				op->nmid=BJX2_NMID_FLDCI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCIL_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0xF:	/* F0eF_1Fnm */
				op->nmid=BJX2_NMID_FLDCI;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FLDCIUL_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		}
		break;

	case 0x2:	/* F0zz_2zzz */
//		op->rn=ro_dfl;

		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{

		case 0x0:	/* F0ez_20zz */
			op->nmid=BJX2_NMID_PADDW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDW_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PADDL;
				op->Run=BJX2_Op_PADDL_RegRegReg;
			}
			break;
		case 0x1:	/* F0ez_21zz */
			op->nmid=BJX2_NMID_PSUBW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBW_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSUBL;
				op->Run=BJX2_Op_PSUBL_RegRegReg;
			}
			break;

		case 0x2:	/* F0eo_22nm */
			op->nmid=BJX2_NMID_SHAR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAR_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHARQ;
				op->Run=BJX2_Op_SHARQ_RegRegReg;
			}
			if(jbits&0x01000000U)
			{
				op->imm=jbits&0xFFFFFF;
				op->nmid=BJX2_NMID_BITMOVS;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_BITMOVS_RegRegImmReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_BITMOVSX;
					op->Run=BJX2_Op_BITMOVSX_RegRegImmReg;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
			}
			break;
		case 0x3:	/* F0eo_23nm */
			op->nmid=BJX2_NMID_SHLR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLR_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHLRQ;
				op->Run=BJX2_Op_SHLRQ_RegRegReg;
			}
			if(jbits&0x01000000U)
			{
				op->imm=jbits&0xFFFFFF;
				op->nmid=BJX2_NMID_BITMOV;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_BITMOV_RegRegImmReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_BITMOVX;
					op->Run=BJX2_Op_BITMOVX_RegRegImmReg;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
				}
			}
			break;
		case 0x4:	/* F0eo_24nm */
			op->nmid=BJX2_NMID_PCSELTW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PCSELTW_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PCSELTL;
				op->Run=BJX2_Op_PCSELTL_RegRegReg;
			}
			break;
		case 0x5:	/* F0ez_25zz */
//			if(dispasc)
			if((dispasc&3)!=0)
			{
				if(!eq)
					break;
				op->imm=(disp11as&511)|((dispasc&3)<<9);
				op->nmid=BJX2_NMID_PSHADDXF;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_PSHADDXF_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}

			op->imm=disp11as;

			op->nmid=BJX2_NMID_PADDF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PADDXF;
				op->Run=BJX2_Op_PADDXF_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
#ifdef BJX2_CFG_SIMDFXASS
				op->Run=BJX2_Op_PADDXFA_RegRegReg;
#endif
			}
			break;
		case 0x6:	/* F0ez_26zz */
//			if(dispasc)
			if((dispasc&3)!=0)
			{
				if(!eq)
					break;
				op->imm=(disp11as&511)|((dispasc&3)<<9);
				op->nmid=BJX2_NMID_PSHSUBXF;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_PSHSUBXF_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}

			op->imm=disp11as;

			op->nmid=BJX2_NMID_PSUBF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSUBXF;
				op->Run=BJX2_Op_PSUBXF_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
#ifdef BJX2_CFG_SIMDFXASS
				op->Run=BJX2_Op_PSUBXFA_RegRegReg;
#endif
			}
			break;
		case 0x7:	/* F0ez_27zz */
//			if(dispasc)
			if((dispasc&3)!=0)
			{
				if(!eq)
					break;
				op->imm=(disp11as&511)|((dispasc&3)<<9);
				op->nmid=BJX2_NMID_PSHMULXF;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_PSHMULXF_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
		
			op->imm=disp11as;

			op->nmid=BJX2_NMID_PMULF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PMULXF;
				op->Run=BJX2_Op_PMULXF_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
#ifdef BJX2_CFG_SIMDFXASS
				op->Run=BJX2_Op_PMULXFA_RegRegReg;
#endif
			}
			break;

		case 0x8:	/* F0ez_28zz */
			op->nmid=BJX2_NMID_MOVHD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MOVHD_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVLD;
				op->Run=BJX2_Op_MOVLD_RegRegReg;
			}
			break;
		case 0x9:	/* F0ez_29zz */
			op->nmid=BJX2_NMID_MOVHLD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MOVHLD_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVLHD;
				op->Run=BJX2_Op_MOVLHD_RegRegReg;
			}
			break;


		case 0xA:	/* F0ez_2Azz */
			op->nmid=BJX2_NMID_PSCHEQW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSCHEQW_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSCHEQB;
				op->Run=BJX2_Op_PSCHEQB_RegRegReg;
//				op->nmid=BJX2_NMID_PADDXD;
//				op->Run=BJX2_Op_PADDXD_RegRegReg;
			}
			break;
		case 0xB:	/* F0ez_2Bzz */
			op->nmid=BJX2_NMID_PSCHNEW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSCHNEW_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSCHNEB;
				op->Run=BJX2_Op_PSCHNEB_RegRegReg;
//				op->nmid=BJX2_NMID_PADDXD;
//				op->Run=BJX2_Op_PADDXD_RegRegReg;
			}
			break;

		case 0xC:	/* F0ez_2Czz */
			op->nmid=BJX2_NMID_BLKUTX1;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_BLKUTX1_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_BLKUTX2;
				op->Run=BJX2_Op_BLKUTX2_RegRegReg;
			}
			break;


		case 0xD:	/* F0ez_2Dzz */
//			if(dispasc)
			if((dispasc&3)!=0)
			{
				if(eq)
					break;
				op->imm=(disp11as&511)|((dispasc&3)<<9);
				op->nmid=BJX2_NMID_PSHADDH;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_PSHADDH_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
		
			op->nmid=BJX2_NMID_PADDH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDH_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PADDXD;
				op->Run=BJX2_Op_PADDXD_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0xE:	/* F0ez_2Ezz */
//			if(dispasc)
			if((dispasc&3)!=0)
			{
				if(eq)
					break;
				op->imm=(disp11as&511)|((dispasc&3)<<9);
				op->nmid=BJX2_NMID_PSHSUBH;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_PSHSUBH_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
		
			op->nmid=BJX2_NMID_PSUBH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSUBXD;
				op->Run=BJX2_Op_PSUBXD_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0xF:	/* F0ez_2Fzz */
//			if(dispasc)
			if((dispasc&3)!=0)
			{
				if(eq)
					break;
				op->imm=(disp11as&511)|((dispasc&3)<<9);
				op->nmid=BJX2_NMID_PSHMULH;
				op->fmid=BJX2_FMID_REGREGIMMREG;
				op->Run=BJX2_Op_PSHMULH_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
		
			op->nmid=BJX2_NMID_PMULH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULH_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_PMULXD;
				op->Run=BJX2_Op_PMULXD_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;

#if 0
		case 0x0:	/* 20zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BRA_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x1:	/* 21zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BSR;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BSR_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x2:	/* 22zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BT;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BT_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
		case 0x3:	/* 23zz */
			op->imm=(s16)(((opw1&255)<<8)|(opw2&255));
			op->nmid=BJX2_NMID_BF;
			op->fmid=BJX2_FMID_PCDISP;
			op->Run=BJX2_Op_BF_PcDisp;
			op->fl|=BJX2_OPFL_CTRLF;
			break;
#endif

#if 0
		case 0x4:
		case 0x5:
			if(opw2&0x0100)
			{
				op->imm=((opw1&255)<<8)|(opw2&255)|(~65535);
				op->nmid=BJX2_NMID_LDIN;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIN_Imm12;
			}else
			{
				op->imm=((opw1&255)<<8)|(opw2&255);
				op->nmid=BJX2_NMID_LDIZ;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIZ_Imm12;
			}
			break;

		case 0x6:	/* F0zz_26zz */
			op->imm=((opw1&255)<<8)|(opw2&255);
			op->nmid=BJX2_NMID_LDISH16;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDISH16_Imm;
			break;
#endif

#if 0
		case 0x8:	/* 28zz */
			op->imm=imm10u;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x9:	/* 29zz */
			op->imm=imm10u;
			op->rm=op->rn;
			op->rn=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;
		case 0xA:	/* 2Azz */
			op->imm=imm10u;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 0xB:	/* 2Bzz */
			op->imm=imm10u;
			op->rm=BJX2_REG_SP;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;
#endif

#if 0
		case 0xC:
//			op->imm=imm8u;
			op->imm=imm10u;
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xD:
//			op->imm=imm8n;
			op->imm=imm10n;
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			break;
		case 0xE:
//			op->imm=imm8u;
			op->imm=imm10u;
			op->nmid=BJX2_NMID_CMPGT;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGT_ImmReg;
			break;
		case 0xF:
//			op->imm=imm8u;
			op->imm=imm10u;
			op->nmid=BJX2_NMID_CMPGE;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGE_ImmReg;
			break;
#endif
		}
		break;

	case 0x3:	/* F0ez_3zzz */
		op->rm=ro_dfl;
		op->ro=BJX2_REG_ZZR;
		op->rn=ro_dfl;
//		op->imm=ro_dfl;
		op->imm=ro_rvo;
		op1r_opsel=opw2&255;
		
		if(isxg3)
		{
			op->rm=rn_dfl;
			op->ro=BJX2_REG_ZZR;
			op->rn=rn_dfl;
			op->imm=rn_rvo;
			op1r_opsel=(opw2&15)|((ro_rvo&15)<<4);
		}
		
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_30zz */
//			switch(opw2&255)
			switch(op1r_opsel)
			{
			case 0x00:	/* F0ez_3000 */
				op->fmid=BJX2_FMID_Z;
				switch(opw1&15)
				{
				case 0x0:
					op->nmid=BJX2_NMID_NOP;
					op->Run=BJX2_Op_NOP_None;
					break;
				case 0x1:
					op->nmid=BJX2_NMID_RTS;
					op->Run=BJX2_Op_RTS_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					break;

				case 0x2:
					op->nmid=BJX2_NMID_SLEEP;
					op->Run=BJX2_Op_SLEEP_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					break;
				case 0x3:
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					break;

				case 0x4:
					op->nmid=BJX2_NMID_CLRT;
					op->Run=BJX2_Op_CLRT_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x5:
					op->nmid=BJX2_NMID_SETT;
					op->Run=BJX2_Op_SETT_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x6:
					op->nmid=BJX2_NMID_CLRS;
					op->Run=BJX2_Op_CLRS_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x7:
					op->nmid=BJX2_NMID_SETS;
					op->Run=BJX2_Op_SETS_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x8:
					op->nmid=BJX2_NMID_NOTT;
					op->Run=BJX2_Op_NOTT_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x9:
					op->nmid=BJX2_NMID_NOTS;
					op->Run=BJX2_Op_NOTS_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;

/*
				case 0xA:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0x0000;
					op->nmid=BJX2_NMID_LDISH16;
					op->Run=BJX2_Op_LDISH16_Imm;
					break;
				case 0xB:
					op->fmid=BJX2_FMID_IMM;
					op->imm=0xFFFF;
					op->nmid=BJX2_NMID_LDISH16;
					op->Run=BJX2_Op_LDISH16_Imm;
					break;
*/

				case 0xC:
					op->nmid=BJX2_NMID_RTE;
					op->Run=BJX2_Op_RTE_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					break;

				case 0xF:
					op->nmid=BJX2_NMID_LDTLB;
					op->Run=BJX2_Op_LDTLB_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					break;
				}
				break;

			case 0x02:	/* F0ez_3002 */
				op->fmid=BJX2_FMID_Z;
				switch(opw1&15)
				{

				case 0x1:
					op->nmid=BJX2_NMID_RTSU;
					op->Run=BJX2_Op_RTS_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					break;

				case 0x2:
					op->nmid=BJX2_NMID_SYSCALL;
					op->fmid=BJX2_FMID_Z;
					op->Run=BJX2_Op_SYSCALL_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;

				case 0x4:
					op->nmid=BJX2_NMID_LDACL;
					op->Run=BJX2_Op_LDACL_None;
					op->fl|=BJX2_OPFL_NOWEX;
					break;

				case 0x8:
					op->nmid=BJX2_NMID_SXENTR;
					op->Run=BJX2_Op_SXENTR_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0x9:
					op->nmid=BJX2_NMID_SUENTR;
					op->Run=BJX2_Op_SUENTR_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0xA:
					op->nmid=BJX2_NMID_SVEKRR;
					op->Run=BJX2_Op_SVEKRR_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0xB:
					op->nmid=BJX2_NMID_SVENTR;
					op->Run=BJX2_Op_SVENTR_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0xC:
					op->nmid=BJX2_NMID_LDEKRR;
					op->Run=BJX2_Op_LDEKRR_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0xD:
					op->nmid=BJX2_NMID_LDEKEY;
					op->Run=BJX2_Op_SVENTR_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0xE:
					op->nmid=BJX2_NMID_LDEENC;
					op->Run=BJX2_Op_LDEENC_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;
				case 0xF:
					op->nmid=BJX2_NMID_INVTLB;
					op->Run=BJX2_Op_INVTLB_None;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_CTRLF;
					break;

#if 0
				case 0x0:	/* F0e0_3002 */
					op->nmid=BJX2_NMID_BRA;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BRA_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x1:	/* F0e1_3002 */
					op->nmid=BJX2_NMID_BSR;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BSR_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x2:	/* F0e2_3002 */
					op->nmid=BJX2_NMID_BT;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BT_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x3:	/* F0e3_3002 */
					op->nmid=BJX2_NMID_BF;
					op->fmid=BJX2_FMID_PCDR;
					op->Run=BJX2_Op_BF_PcDr;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
#endif
				}
				break;

#if 1
			case 0x10:	/* F0en_3010 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BRA_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
			case 0x11:	/* F0en_3011 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BSR_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
#if 0
			case 0x12:	/* F0en_3012 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BT_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
			case 0x13:	/* F0en_3013 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BF_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
#endif

#endif

#if 0
			case 0x18:	/* F0ez_3018 */
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x19:	/* F0ez_3019 */
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_PUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x1A:	/* F0ez_301A */
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x1B:	/* F0ez_301B */
				if((opw1&0x00F0)==0x0000)
				{
					op->nmid=BJX2_NMID_RET;
					op->fmid=BJX2_FMID_Z;
					op->Run=BJX2_Op_RET_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				}
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_POP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#endif

			case 0x1C:	/* F0ez_301C */
				op->nmid=BJX2_NMID_CMPPL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPL_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x1D:	/* F0ez_301D */
				op->nmid=BJX2_NMID_CMPPZ;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_CMPPZ_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x20:	/* F0ez_3020 */
				if(eq)
				{
					op->nmid=BJX2_NMID_BRAX;
					op->fmid=BJX2_FMID_REG;
					op->Run=BJX2_Op_BRAX_Reg;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
					break;
				}
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BRA_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
			case 0x21:	/* F0ez_3021 */
				if(eq)
				{
					op->nmid=BJX2_NMID_BSRX;
					op->fmid=BJX2_FMID_REG;
					op->Run=BJX2_Op_BSRX_Reg;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
					op->fl|=BJX2_OPFL_REGX2R;
					break;
				}
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BSR_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;

#if 0
			case 0x22:	/* F0ez_3022 */
				if(eq)
					break;
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BT_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
			case 0x23:	/* F0ez_3023 */
				if(eq)
					break;
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BF_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
#endif

			case 0x33:	/* 33z3 */
				op->nmid=BJX2_NMID_MOVNT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVNT_Reg;
//				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_REGCHKG;
					op->Run=BJX2_Op_REGCHKG_Reg;
				}
				break;
			case 0x34:	/* 33z4 */
				op->nmid=BJX2_NMID_ROTL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTL_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x35:	/* 33z5 */
				op->nmid=BJX2_NMID_ROTR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTR_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x36:	/* 33z6 */
				op->nmid=BJX2_NMID_ROTCL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCL_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_ROTCLQ;
					op->Run=BJX2_Op_ROTCLQ_Reg;
				}
				break;
			case 0x37:	/* 33z7 */
				op->nmid=BJX2_NMID_ROTCR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCR_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_ROTCRQ;
					op->Run=BJX2_Op_ROTCRQ_Reg;
				}
				break;

			case 0x38:	/* 33z8 */
				op->nmid=BJX2_NMID_SHLL;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHLL_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x39:	/* 33z9 */
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHLR_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x3A:	/* 33zA */
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SHAR_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

#if 0
			case 0x68:	/* 36z8 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BRA_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
			case 0x69:	/* 36z9 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BSR_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				break;
#endif

#if 0
			case 0x6A:	/* 36zA */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_SWAPB;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPB_Reg;
				break;
			case 0x6B:	/* 36zB */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_SWAPW;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_SWAPW_Reg;
				break;
#endif

			case 0x69:	/* 36z9 */
				op->nmid=BJX2_NMID_WEXMD;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_WEXMD_Imm;
				op->fl|=BJX2_OPFL_NOWEX;
				op->imm=op->rn;
				if(op->imm==BJX2_REG_DLR)
					op->imm=0;
				if(op->imm==BJX2_REG_DHR)
					op->imm=1;
				ctx->v_wexmd=op->imm;
				break;

			case 0x6A:	/* 36zA */
				op->nmid=BJX2_NMID_CPUID;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_CPUID_Imm;
				op->fl|=BJX2_OPFL_NOWEX;
				op->imm=op->rn;
				if(op->rn==BJX2_REG_DLR)
					op->imm=0;
				if(op->rn==BJX2_REG_DHR)
					op->imm=1;
				if(op->rn==BJX2_REG_SP)
					op->imm=15;
				break;

			case 0x6F:	/* 36zF */
				op->nmid=BJX2_NMID_MOVT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVT_Reg;
//				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_REGCHKC;
					op->Run=BJX2_Op_REGCHKC_Reg;
				}
				break;

#if 0
			case 0x82:	/* 38z2 */
				op->nmid=BJX2_NMID_FPUSH;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_PUSH_FpReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x83:	/* 38z3 */
				op->nmid=BJX2_NMID_FPOP;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_POP_FpReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
#endif
			}
			break;
		case 0x7:
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_37nm */
				op->nmid=BJX2_NMID_SWAPB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPB_Reg;
				break;
			case 0x1:	/* F0e1_37nm */
				op->nmid=BJX2_NMID_SWAPW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPW_Reg;
				break;
			case 0x2:	/* F0e2_37nm */
				op->nmid=BJX2_NMID_SWAP8B;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAP8B_Reg;
				break;
			case 0x3:	/* F0e3_37nm */
				op->nmid=BJX2_NMID_SWAPL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPL_Reg;
				break;
			case 0x4:	/* F0e4_37nm */
				op->nmid=BJX2_NMID_SWAPLB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPLB_Reg;
				break;
			case 0x5:	/* F0e5_37nm */
				op->nmid=BJX2_NMID_SWAPLW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPLW_Reg;
				break;
			case 0x6:	/* F0e6_37nm */
				op->nmid=BJX2_NMID_SWCPLB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWCPLB_Reg;
				break;
			case 0x7:	/* F0e7_37nm */
				op->nmid=BJX2_NMID_SWCPLW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWCPLW_Reg;
				break;
			case 0x8:	/* F0e8_37nm */
				op->nmid=BJX2_NMID_SWAPMB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPMB_Reg;
				break;
			case 0x9:	/* F0e9_37nm */
				op->nmid=BJX2_NMID_SWAPMW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPMW_Reg;
				break;
			case 0xA:	/* F0eA_37nm */
				op->nmid=BJX2_NMID_SWCPMB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWCPMB_Reg;
				break;
			case 0xB:	/* F0eB_37nm */
				op->nmid=BJX2_NMID_SWCPMW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWCPMW_Reg;
				break;
			case 0xC:	/* F0eC_37nm */
				op->nmid=BJX2_NMID_SWAPHB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPHB_Reg;
				break;
			case 0xD:	/* F0eD_37nm */
				op->nmid=BJX2_NMID_SWAPHW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWAPHW_Reg;
				break;
			case 0xE:	/* F0eE_37nm */
				op->nmid=BJX2_NMID_SWCPHB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWCPHB_Reg;
				break;
			case 0xF:	/* F0eF_37nm */
				op->nmid=BJX2_NMID_SWCPHW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_SWCPHW_Reg;
				break;
			}
			break;

		case 0x1:	/* F0ez_31zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=disp5;

			op->nmid=BJX2_NMID_MOVTT;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MOVTT_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MOVTT_RegImmReg;
				
				if(jbits&0x02000000U)
				{
					op->Run=BJX2_Op_MOVTT_RegImm17Reg;
				}
			}
			break;

		case 0x2:	/* F0ez_32zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=disp5;

			op->nmid=BJX2_NMID_ROTL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ROTL_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHARX;
				op->Run=BJX2_Op_SHARX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}
			break;
		case 0x3:	/* F0ez_33zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=disp5;

			op->nmid=BJX2_NMID_ROTR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ROTR_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHLRX;
				op->Run=BJX2_Op_SHLRX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}
			break;
		case 0x4:	/* F0ez_34zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=disp5;

//			op->nmid=BJX2_NMID_ROTL;
			op->fmid=BJX2_FMID_REGREGREG;
//			op->Run=BJX2_Op_ROTL_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHADX;
				op->Run=BJX2_Op_SHADX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}else
			{
				op->nmid=BJX2_NMID_MULSHQ;
				op->Run=BJX2_Op_MULSHQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
			}
			break;
		case 0x5:	/* F0ez_35zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=disp5;

//			op->nmid=BJX2_NMID_ROTR;
			op->fmid=BJX2_FMID_REGREGREG;
//			op->Run=BJX2_Op_ROTR_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHLDX;
				op->Run=BJX2_Op_SHLDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}else
			{
				op->nmid=BJX2_NMID_MULUHQ;
				op->Run=BJX2_Op_MULUHQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
			}
			break;
		case 0x6:	/* F0ez_36zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=disp5;

			op->nmid=BJX2_NMID_ROTL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ROTL_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_ROTLX;
				op->Run=BJX2_Op_ROTLX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}
			break;

		case 0x8:	/* F0ez_38zz */
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=BJX2_REG_ZZR;
			op->imm=disp5;

			switch(opw1&15)
			{
			case 0x0:	/* F0e0_38nm */
				op->nmid=BJX2_NMID_BNDCMPB;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCMPB_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCMPB_ImmReg;
				}
				break;
			case 0x1:	/* F0e1_38nm */
				op->nmid=BJX2_NMID_BNDCMPW;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCMPW_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCMPW_ImmReg;
				}
				break;
			case 0x2:	/* F0e2_38nm */
				op->nmid=BJX2_NMID_BNDCMPL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCMPL_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCMPL_ImmReg;
				}
				break;
			case 0x3:	/* F0e3_38nm */
				op->nmid=BJX2_NMID_BNDCMPQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_BNDCMPQ_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
//					op->imm=imm6;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_BNDCMPQ_ImmReg;
				}
				break;

			case 0x6:	/* F0e6_38nm */
				op->nmid=BJX2_NMID_CMPPEQ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPPEQ_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPPEQX;
					op->Run=BJX2_Op_CMPPEQX_RegReg;
					op->fl|=BJX2_OPFL_NOWEXSFX;
				}
				break;
			case 0x7:	/* F0e7_38nm */
				op->nmid=BJX2_NMID_CMPPGT;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_CMPPGT_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->nmid=BJX2_NMID_CMPPGTX;
					op->Run=BJX2_Op_CMPPGTX_RegReg;
					op->fl|=BJX2_OPFL_NOWEXSFX;
				}
				break;
			}
			break;

		default:
			break;
		}
		break;

	case 0x4:	/* F0ez_4zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_40zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
				{
					op->rq=BJX2_REG_PC_HI;
					op->rm=BJX2_REG_PC;
					op->nmid=BJX2_NMID_LEATB;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEATB_LdPcDispReg;
					op->fl|=BJX2_OPFL_NOWEX_IO2;
					break;
				}
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEATB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEATB_LdRegDispReg;
				break;
			}

			op->rn=rm_dfl;
			op->rm=rn_dfl;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVX2_RegStRegDisp;
			break;
		case 0x1:	/* F0ez_41zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEATW;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEATW_LdRegDispReg;
				break;
			}
			break;
		case 0x2:	/* F0ez_42zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEATL;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEATL_LdRegDispReg;
				break;
			}
			break;
		case 0x3:	/* F0ez_43zz */
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;

				op->nmid=BJX2_NMID_LEATQ;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEATQ_LdRegDispReg;
				break;
			}
			break;

#if 0
		case 0x0:
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXM;
			op->rn=rm_dfl;
			op->rm=rn_dfl;
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVX2_RegStRegDisp;
			break;
#endif

		case 0x4:	/* F0ez_44zz */
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
				{
					op->rq=BJX2_REG_PC_HI;
					op->rm=BJX2_REG_PC;
					op->nmid=BJX2_NMID_LEATB;
					op->fmid=BJX2_FMID_LDREG2REG;
					op->Run=BJX2_Op_LEATB_LdPcIdxReg;
					op->fl|=BJX2_OPFL_NOWEX_IO2;
					break;
				}
				if(op->rm==BJX2_REG_DHR)
					break;
				op->fl|=BJX2_OPFL_NOWEX;
				op->nmid=BJX2_NMID_LEATB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEATB_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXM;
			op->rn=rm_dfl;
			op->rm=rn_dfl;

			if(	(op->rm==BJX2_REG_DLR) ||
				(op->rm==BJX2_REG_DHR) ||
				(op->rm==BJX2_REG_SP))
				break;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
							BJX2_EXTID_MOVX2))
					{
						op->Run=NULL;
						break;
					}

					op->imm=0;
					op->nmid=BJX2_NMID_MOVX2;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVX2_RegStRegDisp;
					break;
				}

				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
						BJX2_EXTID_MOVX2))
				{
					op->Run=NULL;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVX2_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
						BJX2_EXTID_MOVX2))
				{
					op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVX2_RegStReg2B;
				break;
			}

			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_MOVX2))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVX2_RegStReg2;
			break;

		case 0x5:	/* F0ez_45zz */
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;
				op->fl|=BJX2_OPFL_NOWEX;
				op->nmid=BJX2_NMID_LEATW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEATW_LdReg2Reg;
				break;
			}

			op->rn=rm_dfl;
			op->rm=cn_dfl;

			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
				break;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVQ_RegStReg2;
			op->fl|=BJX2_OPFL_NOWEX;
			break;


		case 0x6:	/* F0ez_46zz */
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;
				op->fl|=BJX2_OPFL_NOWEX;
				op->nmid=BJX2_NMID_LEATL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEATL_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEX;
			op->rn=rm_dfl;
			op->rm=rn_dfl;
			if(op->rn==BJX2_REG_DLR)
				break;

			if(	(op->rm==BJX2_REG_DLR) ||
				(op->rm==BJX2_REG_DHR) ||
				(op->rm==BJX2_REG_SP))
				break;

			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_FMOVS_GRegStReg2B;

				break;
			}

			if(eq)
				break;

			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_FMOVS_GRegStReg2;
//			op->Run=BJX2_Op_BREAK_None;
			break;

		case 0x7:	/* F0ez_47zz */
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;
				op->fl|=BJX2_OPFL_NOWEX;
				op->nmid=BJX2_NMID_LEATQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEATQ_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEX;
			op->rn=rm_dfl;
			op->rm=rn_dfl;
			if(op->rn==BJX2_REG_DLR)
				break;
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_FMOVH;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_FMOVH_GRegStReg2B;

				break;
			}

			if(eq)
				break;

			op->nmid=BJX2_NMID_FMOVH;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_FMOVH_GRegStReg2;
//			op->Run=BJX2_Op_BREAK_None;
			break;

#if 1
		case 0x8:
			if(eq)
			{
				op->imm=disp5*4;
				op->fl|=BJX2_OPFL_NOWEX;
				op->nmid=BJX2_NMID_MOVNL;
				op->fmid=BJX2_FMID_LDREGDISP1REG;
				op->fl|=BJX2_OPFL_NOWEX;
				op->Run=BJX2_Op_MOVNL_LdRegDisp1Reg;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXN;
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVX2_LdRegDispReg;
			break;
#endif

		case 0xA:	/* F0ez_4Azz */
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;
				op->nmid=BJX2_NMID_PMOVF8;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_PMOVF8_LdRegDispReg;
				break;
			}
			break;

		case 0xB:	/* F0ez_4Bzz */
			if(eq)
			{
				break;
			}
			break;

		case 0xC:	/* F0ez_4Czz */
			if(eq)
			{
				op->fl|=BJX2_OPFL_NOWEX;
				op->nmid=BJX2_NMID_MOVNL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->fl|=BJX2_OPFL_NOWEX;
				op->Run=BJX2_Op_MOVNL_LdReg2Reg;
				break;
			}

#if 0
			if(eq)
			{
				op->rn=cn_dfl;

				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdReg2Reg;
				break;
			}
#endif

			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_MOVX2))
			{
				op->Run=NULL;
				break;
			}

			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXN;
			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->imm=0;
					op->nmid=BJX2_NMID_MOVX2;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVX2_LdRegDispReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVX2_LdPcIdxReg;
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVX2_LdReg2RegB;
				break;
			}
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVX2_LdReg2Reg;
			break;

		case 0xD:	/* F0ez_4Dzz */
			if(eq)
			{
				break;

#if 0			
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_GFP))
				{
					op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_FSUB;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FSUBA_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->imm=8;
				break;
#endif
			}

			op->rn=cn_dfl;

			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
				break;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVQ_LdReg2Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0xE:	/* F0ez_4Ezz */
			if(eq)
			{
				if(op->rm==BJX2_REG_DLR)
					break;
				if(op->rm==BJX2_REG_DHR)
					break;
				op->nmid=BJX2_NMID_PMOVF8;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_PMOVF8_LdReg2Reg;
				break;

#if 0
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_GFP))
				{
					op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_FMUL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FMULA_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->imm=8;
				break;
#endif
			}

			if(	(op->rn==BJX2_REG_DLR) ||
				(op->rn==BJX2_REG_DHR) ||
				(op->rn==BJX2_REG_SP))
				break;

			op->fl|=BJX2_OPFL_NOWEX;
			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_FMOVS_LdReg2GRegB;

				break;
			}

			if(eq)
				break;

			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_FMOVS_LdReg2GReg;
//			op->Run=BJX2_Op_BREAK_None;
			break;

		case 0xF:	/* F0ez_4Fzz */
			if(	(op->rn==BJX2_REG_DLR) ||
				(op->rn==BJX2_REG_DHR) ||
				(op->rn==BJX2_REG_SP))
				break;

			op->fl|=BJX2_OPFL_NOWEX;
			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_FMOVH;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_FMOVH_LdReg2GRegB;

				break;
			}

			if(eq)
				break;

			op->nmid=BJX2_NMID_FMOVH;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_FMOVH_LdReg2GReg;
//			op->Run=BJX2_Op_BREAK_None;
			break;

#if 0
		case 0xC:	/* F0eo_4Cnm */
			if(op->rm==BJX2_REG_DLR)
			{
				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAB_LdPcIdxReg;
				if(eq)
				{
					op->fmid=BJX2_FMID_LDPCDISPREG;
					op->Run=BJX2_Op_LEAB_LdPcDispReg;
				}
				break;
			}

			op->nmid=BJX2_NMID_LEAB;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAB_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAB_LdRegDispReg;
			}
			break;
		case 0xD:	/* F0eo_4Dnm */
			if(op->rm==BJX2_REG_DLR)
			{
				break;
			}

			op->nmid=BJX2_NMID_LEAW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAW_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAW_LdRegDispReg;
			}
			break;
		case 0xE:	/* F0eo_4Enm */
			if(op->rm==BJX2_REG_DLR)
			{
				break;
			}

			op->nmid=BJX2_NMID_LEAD;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAD_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAD_LdRegDispReg;
			}
			break;
		case 0xF:	/* F0eo_4Fnm */
			if(op->rm==BJX2_REG_DLR)
			{
				break;
			}

			op->nmid=BJX2_NMID_LEAQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_LEAQ_LdReg2Reg;
			if(eq)
			{
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAQ_LdRegDispReg;
			}
			break;
#endif
		}
		break;

	case 0x5:	/* F0ez_5zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0eo_50nm */
			op->nmid=BJX2_NMID_CSELT;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_CSELT_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_NOP;
				op->Run=BJX2_Op_NOP_None;
			}
			break;

		case 0x2:	/* F0ez_52zz */
			op->nmid=BJX2_NMID_MULSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULSL_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_MULQ))
				{
					op->Run=NULL;
					break;
				}

//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_MULSL_RegImmReg;

				op->nmid=BJX2_NMID_MULSQ;
				op->Run=BJX2_Op_MULSQ_RegRegReg;
			}
			break;
		case 0x3:	/* F0ez_53zz */
			op->nmid=BJX2_NMID_MULUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULUL_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
					BJX2_EXTID_MULQ))
				{
					op->Run=NULL;
					break;
				}

//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_MULUL_RegImmReg;

				op->nmid=BJX2_NMID_MULUQ;
				op->Run=BJX2_Op_MULUQ_RegRegReg;
			}
			break;

		case 0x4:	/* F0eo_54nm */
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAD_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHADQ;
				op->Run=BJX2_Op_SHADQ_RegRegReg;
			}
			break;
		case 0x5:	/* F0eo_55nm */
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLD_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHLDQ;
				op->Run=BJX2_Op_SHLDQ_RegRegReg;
			}
			break;

		case 0x6:	/* F0eo_56nm */
			op->nmid=BJX2_NMID_PMULSLW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULSLW_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX3L;
			if(eq)
			{
				op->nmid=BJX2_NMID_PMULULW;
				op->Run=BJX2_Op_PMULULW_RegRegReg;
			}
			break;
		case 0x7:	/* F0eo_57nm */
			op->nmid=BJX2_NMID_PMULSHW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULSHW_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX3L;
			if(eq)
			{
				op->nmid=BJX2_NMID_PMULUHW;
				op->Run=BJX2_Op_PMULUHW_RegRegReg;
			}
			break;

#if 0
		case 0x6:	/* F0eo_56nm */
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHAD_RegImmReg;
			if(eq)op->imm|=~31;
			break;
		case 0x7:	/* F0eo_57nm */
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLD_RegImmReg;
			if(eq)op->imm|=~31;
			break;
#endif

#if 0
		case 0x8:	/* F0eo_58nm */
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADQ_RegImmReg;
			if(eq)op->imm|=~31;
			break;
		case 0x9:	/* F0eo_59nm */
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			if(eq)op->imm|=~31;
			break;
		case 0xA:	/* F0eo_5Anm */
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADQ_RegImmReg;
			if(eq)
				op->imm|=~63;
			else
				op->imm|= 32;
			break;
		case 0xB:	/* F0eo_5Bnm */
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			if(eq)
				op->imm|=~63;
			else
				op->imm|= 32;
			break;
#endif

		case 0x8:	/* F0ez_58zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->imm=disp11as;

			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			
			if(disp11as&8)
				op->Run=BJX2_Op_FADDA_RegRegReg;
			
			if(eq)
			{
				op->nmid=BJX2_NMID_FADDX;
				op->Run=BJX2_Op_FADDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x9:	/* F0ez_59zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->imm=disp11as;

			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;

			if(disp11as&8)
				op->Run=BJX2_Op_FSUBA_RegRegReg;

			if(eq)
			{
				op->nmid=BJX2_NMID_FSUBX;
				op->Run=BJX2_Op_FSUBX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0xA:	/* F0ez_5Azz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->imm=disp11as;

			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;

			if(disp11as&8)
				op->Run=BJX2_Op_FMULA_RegRegReg;

			if(eq)
			{
				op->nmid=BJX2_NMID_FMULX;
				op->Run=BJX2_Op_FMULX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;

#if 0
		case 0xB:	/* F0eo_5Bnm */
			op->nmid=BJX2_NMID_RGBAVG;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_RGBAVG_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_RGBMOD;
				op->Run=BJX2_Op_RGBMOD_RegRegReg;
			}
			break;
#endif

		case 0xB:	/* F0ez_5Bzz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			if(eq)
			{
				break;
			}

//			op->imm=disp11as;

			op->nmid=BJX2_NMID_FMAC;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMADDD_RegRegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;

#if 0
			if(eq)
			{
				op->nmid=BJX2_NMID_FMACX;
				op->Run=BJX2_Op_FMULX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
#endif
			break;

		case 0xC:	/* F0ez_5Czz */
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDSL_RegRegReg;

			if(eq)
			{
				op->nmid=BJX2_NMID_ADDUL;
				op->Run=BJX2_Op_ADDUL_RegRegReg;
			}

			if(ro_dfl==BJX2_REG_ZZR)
			{
				op->nmid=BJX2_NMID_EXTSL;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_EXTSL_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_EXTUL;
					op->Run=BJX2_Op_EXTUL_RegReg;
				}
			}
			
			break;
		case 0xD:	/* F0ez_5Dzz */
			op->nmid=BJX2_NMID_SUBSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUBSL_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SUBUL;
				op->Run=BJX2_Op_SUBUL_RegRegReg;
			}
			break;

		case 0xE:	/* F0ez_5Ezz */
			op->nmid=BJX2_NMID_MULSW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULSW_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX3L;

			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MULSW_RegImmReg;
			}
			break;
		case 0xF:	/* F0ez_5Fzz */
			op->nmid=BJX2_NMID_MULUW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULUW_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX3L;

			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MULUW_RegImmReg;
			}
			break;
		}
		break;

	case 0x6:	/* F0ez_6zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=imm5;
		switch((opw2>>8)&15)
		{

		case 0x0:	/* F0ez_60zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_DMAC))
			{
				op->Run=NULL;
				break;
			}

//			if(eq)
//				break;

			op->nmid=BJX2_NMID_MACSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MACSL_RegRegReg;
			
//			if(op->rp!=BJX2_REG_ZZR)
			if(is4r)
			{
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_MACSL_RegRegRegReg;
			}
			
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MACSL_RegImmReg;

				if(is4r)
				{
					op->fmid=BJX2_FMID_REGIMMREGREG;
					op->Run=BJX2_Op_MACSL_RegImmRegReg;
				}
			}
			break;
		case 0x1:	/* F0ez_61zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_DMAC))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_MACUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MACUL_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

//			if(op->rp!=BJX2_REG_ZZR)
			if(is4r)
			{
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_MACUL_RegRegRegReg;
			}
			
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MACUL_RegImmReg;

				if(is4r)
				{
					op->fmid=BJX2_FMID_REGIMMREGREG;
					op->Run=BJX2_Op_MACUL_RegImmRegReg;
				}
			}
			break;

		case 0x2:	/* F0ez_62zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_DMAC))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_DMACSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_DMACSL_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

//			if(op->rp!=BJX2_REG_ZZR)
			if(is4r)
			{
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_DMACSL_RegRegRegReg;
			}
			
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_DMACSL_RegImmReg;

				if(is4r)
				{
					op->fmid=BJX2_FMID_REGIMMREGREG;
					op->Run=BJX2_Op_DMACSL_RegImmRegReg;
				}
			}
			break;
		case 0x3:	/* F0ez_63zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_DMAC))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_DMACUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_DMACUL_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

//			if(op->rp!=BJX2_REG_ZZR)
			if(is4r)
			{
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_DMACUL_RegRegRegReg;
			}
			
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_DMACUL_RegImmReg;

				if(is4r)
				{
					op->fmid=BJX2_FMID_REGIMMREGREG;
					op->Run=BJX2_Op_DMACUL_RegImmRegReg;
				}
			}
			break;

		case 0x4:	/* F0ez_64zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_MULQ))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_DIVSQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_DIVSQ_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_DIVUQ;
				op->Run=BJX2_Op_DIVUQ_RegRegReg;
			}
			break;
		case 0x5:	/* F0ez_65zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_MULQ))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_MODSQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MODSQ_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_MODUQ;
				op->Run=BJX2_Op_MODUQ_RegRegReg;
			}
			break;
		case 0x6:	/* F0ez_66zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_FDIV))
			{
				op->nmid=BJX2_NMID_FDIV;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_EMUTRAP_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_CTRLF;

//				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FDIV;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FDIVD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_FDIVX;
				op->Run=BJX2_Op_FDIVX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x7:	/* F0ez_67zz */
			op->nmid=BJX2_NMID_FDIVA;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FDIVAD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_FDIVXA;
				op->Run=BJX2_Op_FDIVXA_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;

		case 0x8:	/* F0ez_68zz */
			op->nmid=BJX2_NMID_BLKUTX2;
			op->fmid=BJX2_FMID_REGREGREG;
//			op->Run=BJX2_Op_BLKUTX2_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_BLKUTX3H;
				op->Run=BJX2_Op_BLKUTX3H_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x9:	/* F0ez_69zz */
			op->nmid=BJX2_NMID_BLERP;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_BLERP_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_BLKUTX3L;
				op->Run=BJX2_Op_BLKUTX3L_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0xA:	/* F0ez_6Azz */
			op->nmid=BJX2_NMID_BLINTA;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_BLINTA_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_BLINT;
				op->Run=BJX2_Op_BLINT_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;

		case 0xC:	/* F0ez_6Czz */
			op->nmid=BJX2_NMID_BLKUAB1;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_BLKUAB1_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_BLKUAB2;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_BLKUAB2_RegRegReg;
			}
			break;

		case 0xD:	/* F0ez_6Dzz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDG_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			if(eq)
			{
//				op->nmid=BJX2_NMID_FADDX;
//				op->Run=BJX2_Op_FADDX_RegRegReg;
//				op->fl|=BJX2_OPFL_NOWEXSFX;
//				op->fl|=BJX2_OPFL_REGX3R;

				op->imm=BJX2_OpI_ImmFp5ToFp16(ctx, imm5);
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_FADD_RegImmReg;
			}
			break;
		case 0xE:	/* F0ez_6Ezz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBG_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
//				op->nmid=BJX2_NMID_FSUBX;
//				op->Run=BJX2_Op_FSUBX_RegRegReg;
//				op->fl|=BJX2_OPFL_NOWEXSFX;
//				op->fl|=BJX2_OPFL_REGX3R;

				op->imm=BJX2_OpI_ImmFp5ToFp16(ctx, imm5);
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_FSUB_RegImmReg;
			}
			break;
		case 0xF:	/* F0ez_6Fzz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULG_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
//				op->nmid=BJX2_NMID_FMULX;
//				op->Run=BJX2_Op_FMULX_RegRegReg;
//				op->fl|=BJX2_OPFL_NOWEXSFX;
//				op->fl|=BJX2_OPFL_REGX3R;

				op->imm=BJX2_OpI_ImmFp5ToFp16(ctx, imm5);
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_FMUL_RegImmReg;
			}
			break;
		}
		break;


	case 0x7:	/* F0ez_7zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=imm5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_70zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDA_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			op->imm=8;
			break;

		case 0x1:	/* F0ez_71zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBA_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			op->imm=8;
			break;

		case 0x2:	/* F0ez_72zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_BITNN;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_BITNN_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
//				op->fl|=BJX2_OPFL_NOWEX_FP2;
//				op->imm=8;
				break;
			}

			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULA_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			op->imm=8;
			break;

		case 0x3:	/* F0ez_73zz */
			op->nmid=BJX2_NMID_MOVHW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MOVHW_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVLW;
				op->Run=BJX2_Op_MOVLW_RegRegReg;
			}
			break;

		case 0x4:	/* F0ez_74zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_MULQ))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_DIVSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_DIVSL_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_DIVUL;
				op->Run=BJX2_Op_DIVUL_RegRegReg;
			}
			break;
		case 0x5:	/* F0ez_75zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_MULQ))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_MODSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MODSQ_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_MODUL;
				op->Run=BJX2_Op_MODUQ_RegRegReg;
			}
			break;

		case 0x6:	/* F0ez_76zz */
			op->nmid=BJX2_NMID_RGB5CCENC;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_RGB5CCENC_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
//				op->nmid=BJX2_NMID_RGB5CCENC2;
				op->nmid=BJX2_NMID_PMULF8H;
				op->Run=BJX2_Op_PMULF8H_RegRegReg;
				break;
			}
			break;

		case 0x7:	/* F0ez_77zz */
			op->nmid=BJX2_NMID_SUBP;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUBP_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_SUBXP;
				op->Run=BJX2_Op_SUBXP_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
				break;
			}
			break;
		}
		break;



#if 1
	case 0x8:	/* F0ez_8zzz */
		if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_XMOV))
			break;

		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;

		if((opw2>>8)&8)
//			op->rq=rm_dfl+1;
			op->rq=rm_x+1;
		else
//			op->rq=rn_dfl+1;
			op->rq=rn_x+1;

		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_80zz */
			if(eq)
			{
				op->rq=rm_x+1;
				op->fl|=BJX2_OPFL_REGX2R;
				op->nmid=BJX2_NMID_XLEAB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_XLEAB_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVB_RegStRegDisp;
			op->fl|=BJX2_OPFL_REGXN;
			break;
		case 0x1:	/* F0ez_81zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_TSTQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_TSTQ_RegRegReg;

				if(isjimm)
				{
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_TSTQ_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_TSTQ_RegReg;

					if(isjimm)
					{
						op->fmid=BJX2_FMID_IMMREG;
						op->Run=BJX2_Op_TSTQ_ImmReg;
					}
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVW_RegStRegDisp;
			op->fl|=BJX2_OPFL_REGXN;
			break;
		case 0x2:	/* F0ez_82zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_TSTNQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_TSTNQ_RegRegReg;

				if(isjimm)
				{
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_TSTNQ_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_TSTNQ_RegReg;

					if(isjimm)
					{
						op->fmid=BJX2_FMID_IMMREG;
						op->Run=BJX2_Op_TSTNQ_ImmReg;
					}
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			op->fl|=BJX2_OPFL_REGXN;
			break;
		case 0x3:	/* F0ez_83zz */
			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_MOVX2))
					break;

				op->fl|=BJX2_OPFL_REGX2R;
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVX2_RegStRegDisp;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			op->fl|=BJX2_OPFL_REGXN;
			break;

		case 0x4:	/* F0ez_84zz */
			if(eq)
			{
				op->rq=rm_x+1;
				op->fl|=BJX2_OPFL_REGX2R;
				op->nmid=BJX2_NMID_XLEAB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_XLEAB_LdReg2Reg;
				break;
			}

			op->fl|=BJX2_OPFL_REGXN;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVB;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVB_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStReg2B;
				break;
			}

			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVB_RegStReg2;
			break;
		case 0x5:	/* F0ez_85zz */
			if(eq)
				break;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVW;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVW_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStReg2B;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVW_RegStReg2;
			break;
		case 0x6:	/* F0ez_86zz */
			if(eq)
				break;

			op->fl|=BJX2_OPFL_REGXN;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVL;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVL_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStReg2B;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVL_RegStReg2;
			break;
		case 0x7:	/* F0ez_87zz */
			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_MOVX2))
					break;

				op->fl|=BJX2_OPFL_REGX2R;
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVX2_RegStReg2;
				break;
			}

			op->fl|=BJX2_OPFL_REGXN;

			if(op->rn==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVQ;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVQ_RegStReg;
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStPcIdx;
				break;
			}
			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStReg2B;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVQ_RegStReg2;
			break;

		case 0x8:	/* F0ez_88zz */
			op->fl|=BJX2_OPFL_REGXM;
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVB_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdRegDispReg;
			}
			break;
		case 0x9:	/* F0ez_89zz */
			op->fl|=BJX2_OPFL_REGXM;
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVW_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdRegDispReg;
			}
			break;
		case 0xA:	/* F0ez_8Azz */
			op->fl|=BJX2_OPFL_REGXM;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			}
			break;
		case 0xB:	/* F0ez_8Bzz */
			op->fl|=BJX2_OPFL_REGXM;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
						BJX2_EXTID_MOVX2))
				{
					op->Run=NULL;
					break;
				}

				op->fl|=BJX2_OPFL_REGX2R;
				op->nmid=BJX2_NMID_MOVX2;
				op->Run=BJX2_Op_MOVX2_LdRegDispReg;
			}
			break;

		case 0xC:	/* F0ez_8Czz */
			op->fl|=BJX2_OPFL_REGXM;
			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVB;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVB_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVUB;
						op->Run=BJX2_Op_MOVUB_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVB_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVB_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUB;
					op->Run=BJX2_Op_MOVUB_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVB_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdReg2Reg;
			}
			break;
		case 0xD:	/* F0ez_8Dzz */
			op->fl|=BJX2_OPFL_REGXM;
			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVW;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVW_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVUW;
						op->Run=BJX2_Op_MOVUW_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVW_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVW_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUW;
					op->Run=BJX2_Op_MOVUW_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVW_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdReg2Reg;
			}
			break;
		case 0xE:	/* F0ez_8Ezz */
			op->fl|=BJX2_OPFL_REGXM;
			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVL;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVL_LdRegReg;
					if(eq)
					{
						op->nmid=BJX2_NMID_MOVUL;
						op->Run=BJX2_Op_MOVUL_LdRegReg;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVL_LdPcIdxReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVL_LdReg2RegB;
				if(eq)
				{
					op->nmid=BJX2_NMID_MOVUL;
					op->Run=BJX2_Op_MOVUL_LdReg2RegB;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVL_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdReg2Reg;
			}
			break;
		case 0xF:	/* F0ez_8Fzz */
			op->fl|=BJX2_OPFL_REGXM;
			if(op->rm==BJX2_REG_DLR)
			{
				if(op->ro==BJX2_REG_DHR)
				{
					op->nmid=BJX2_NMID_MOVQ;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVQ_LdRegReg;
					if(eq)
					{
						if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
								BJX2_EXTID_MOVX2))
						{
							op->Run=NULL;
							break;
						}

						op->imm=0;
						op->nmid=BJX2_NMID_MOVX2;
						op->Run=BJX2_Op_MOVX2_LdRegDispReg;
						op->fl|=BJX2_OPFL_REGX2R;
					}
					break;
				}

				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdPcIdxReg;
				if(eq)
				{
					if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
							BJX2_EXTID_MOVX2))
					{
						op->Run=NULL;
						break;
					}
					op->nmid=BJX2_NMID_MOVX2;
					op->Run=BJX2_Op_MOVX2_LdPcIdxReg;
				}
				break;
			}
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==BJX2_REG_DHR)
					{ op->rm=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdReg2RegB;
				if(eq)
				{
					if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
							BJX2_EXTID_MOVX2))
					{
						op->Run=NULL;
						break;
					}

					op->nmid=BJX2_NMID_MOVX2;
					op->Run=BJX2_Op_MOVX2_LdReg2RegB;
					op->fl|=BJX2_OPFL_REGX2R;
				}
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVQ_LdReg2Reg;
			if(eq)
			{
				if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
						BJX2_EXTID_MOVX2))
				{
					op->Run=NULL;
					break;
				}

				op->nmid=BJX2_NMID_MOVX2;
				op->Run=BJX2_Op_MOVX2_LdReg2Reg;
				op->fl|=BJX2_OPFL_REGX2R;
			}
			break;
		}
		break;
#endif


	case 0x9:	/* F0ez_9zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_90zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQEQ;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SEQI_RegImmReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm && wo && !isxg3)
				{
					op->nmid=BJX2_NMID_CMPQHI;
					op->Run=BJX2_Op_SGTIU_RegImmReg;
				}
				
				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPQEQ_ImmReg;
				}
				
				break;
			}
			op->nmid=BJX2_NMID_MOVTW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVTW_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;			
			break;
		case 0x1:	/* F0ez_91zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQGT;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SGTI_RegImmReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm && wo && !isxg3)
				{
					op->nmid=BJX2_NMID_CMPQHI;
					op->Run=BJX2_Op_SGTIU_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPQGT_ImmReg;
				}

				break;
			}
			op->nmid=BJX2_NMID_MOVHTW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVTW_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;			
			break;

		case 0x2:	/* F0ez_92zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQNE;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SNEI_RegImmReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm && wo && !isxg3)
				{
					op->nmid=BJX2_NMID_SLTU;
					op->Run=BJX2_Op_SLTIU_RegImmReg;
				}
				
				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPQNE_ImmReg;
				}

				break;
			}
			break;

		case 0x3:	/* F0ez_93zz */
			if(eq)
			{
//				op->nmid=BJX2_NMID_CMPQLT;
				op->nmid=BJX2_NMID_SLT;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SLTI_RegImmReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm && wo && !isxg3)
				{
					op->nmid=BJX2_NMID_SLTU;
					op->Run=BJX2_Op_SLTIU_RegImmReg;
				}
				
				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->fmid=BJX2_FMID_IMMREG;
					op->Run=BJX2_Op_CMPQLT_ImmReg;
				}
				break;
			}
			break;

		case 0x4:	/* F0ez_94zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQEQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SEQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm)
				{
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_SEQ_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_CMPQEQ_RegReg;
					if(isjimm)
					{
						op->fmid=BJX2_FMID_IMMREG;
						op->Run=BJX2_Op_CMPQEQ_ImmReg;
					}
				}
				break;
			}
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			op->nmid=BJX2_NMID_MOVTW;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVTW_RegStReg2;
			break;
		case 0x5:	/* F0ez_95zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQGT;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SGT_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm)
				{
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_SGT_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_CMPQGT_RegReg;
					if(isjimm)
					{
						op->fmid=BJX2_FMID_IMMREG;
						op->Run=BJX2_Op_CMPQGT_ImmReg;
					}
				}
				break;
			}
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			op->nmid=BJX2_NMID_MOVTW;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVHTW_RegStReg2;
			break;

		case 0x6:	/* F0ez_96zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQNE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SNE_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm)
				{
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_SNE_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_CMPQNE_RegReg;
					if(isjimm)
					{
						op->fmid=BJX2_FMID_IMMREG;
						op->Run=BJX2_Op_CMPQNE_ImmReg;
					}
				}
				break;
			}
			break;
		case 0x7:	/* F0ez_97zz */
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQGE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SGE_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(isjimm)
				{
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_SGE_RegImmReg;
				}

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_CMPQGE_RegReg;
					if(isjimm)
					{
						op->fmid=BJX2_FMID_IMMREG;
						op->Run=BJX2_Op_CMPQGE_ImmReg;
					}
				}
				break;
			}
			break;

		case 0x8:	/* F0ez_98zz */
			op->nmid=BJX2_NMID_MOVTW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVTW_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUTW;
				op->Run=BJX2_Op_MOVUTW_LdRegDispReg;
			}
			break;
		case 0x9:	/* F0ez_99zz */
			if(eq)
				break;
			op->nmid=BJX2_NMID_MOVHTW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVTW_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0xA:
			break;

		case 0xB:
			break;

		case 0xC:	/* F0ez_9Czz */
			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;
			op->nmid=BJX2_NMID_MOVTW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVTW_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUTW;
				op->Run=BJX2_Op_MOVUTW_LdReg2Reg;
			}
			break;

		case 0xD:	/* F0ez_9Dzz */
			if(eq)
			{
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPEQ_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_FCMPEQ_GRegReg;
				}
				break;
			}

			op->imm=disp11as;
			op->fl|=BJX2_OPFL_NOWEX;
			op->nmid=BJX2_NMID_MOVHTW;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_MOVHTW_LdReg2Reg;
			break;

		case 0xE:
			if(eq)
			{
//				op->rm=ro_dfl;
//				op->ro=rm_dfl;
//				op->rn=rn_dfl;

				op->rm=rm_dfl;
				op->ro=ro_dfl;
				op->rn=rn_dfl;

				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPGT_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;

				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=rm_dfl;
					op->rm=ro_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_FCMPGT_GRegReg;
				}
				break;
			}
			break;

		case 0xF:
			if(eq)
			{
//				op->rm=rm_dfl;
//				op->ro=ro_dfl;
//				op->rn=rn_dfl;

				op->rm=ro_dfl;
				op->ro=rm_dfl;
				op->rn=rn_dfl;

				op->nmid=BJX2_NMID_FCMPGE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPGE_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;

		}
		break;


#if 0
	case 0x9:	/* F0ez_9zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;
		op->fl|=BJX2_OPFL_NOWEX;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0eo_90nm */
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_FMOVS_RegStRegDisp;
			if(eq)
			{
				op->nmid=BJX2_NMID_FMOVUS;
				op->Run=BJX2_Op_FMOVS_GRegStRegDisp;
			}
			break;
		case 0x1:	/* F0eo_91nm */
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_FMOVD_RegStRegDisp;
			break;
		case 0x2:	/* F0eo_92nm */
			if(rn_dfl==BJX2_REG_DLR)
			{
				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_FMOVS_RegStPcReg;
				break;
			}
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_FMOVS_RegStReg2;
			if(eq)
			{
				op->nmid=BJX2_NMID_FMOVUS;
				op->Run=BJX2_Op_FMOVS_GRegStReg2;
			}
			break;
		case 0x3:	/* F0eo_93nm */
			if(rn_dfl==BJX2_REG_DLR)
			{
				op->rq=BJX2_REG_PC_HI;
				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_FMOVD_RegStPcReg;
				break;
			}
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_FMOVD_RegStReg2;
			break;
		case 0x4:	/* F0eo_94nm */
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_FMOVS_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_FMOVUS;
				op->Run=BJX2_Op_FMOVS_LdRegDispGReg;
			}
			break;
		case 0x5:	/* F0eo_95nm */
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_FMOVD_LdRegDispReg;
			break;
		case 0x6:	/* F0eo_96nm */
			if(rm_dfl==BJX2_REG_DLR)
			{
				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_FMOVS_LdPcRegReg;
				break;
			}
			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_FMOVS_LdReg2Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_FMOVUS;
				op->Run=BJX2_Op_FMOVS_LdReg2GReg;
			}
			break;
		case 0x7:	/* F0eo_97nm */
			if(rm_dfl==BJX2_REG_DLR)
			{
				op->rq=BJX2_REG_PC_HI;
				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_FMOVD_LdPcRegReg;
				break;
			}
			op->nmid=BJX2_NMID_FMOVD;
			op->fmid=BJX2_FMID_LDREG2REG;
			op->Run=BJX2_Op_FMOVD_LdReg2Reg;
			break;

		case 0x8:	/* 98zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FADD_RegRegReg;
			break;
		case 0x9:	/* 99zz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FSUB_RegRegReg;
			break;
		case 0xA:	/* 9Azz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_GFP))
			{
				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FMUL_RegRegReg;
			break;
		case 0xB:	/* 9Bzz */
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
				BJX2_EXTID_FDIV))
			{
				op->nmid=BJX2_NMID_FDIV;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_EMUTRAP_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_CTRLF;

//				op->Run=NULL;
				break;
			}

			op->nmid=BJX2_NMID_FDIV;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FDIV_RegRegReg;
			break;

		case 0xC:	/* F0ez_9Cnm */
			switch(opw1&15)
			{
#if 0
			case 0x0:	/* F0e0_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_FMOVS_RegStReg;
				break;
			case 0x1:	/* F0e1_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_REGSTREG;
				op->Run=BJX2_Op_FMOVD_RegStReg;
				break;
			case 0x2:	/* F0e2_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_FMOVS_RegStDrReg;
				break;
			case 0x3:	/* F0e3_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_REGSTDRREG;
				op->Run=BJX2_Op_FMOVD_RegStDrReg;
				break;
			case 0x4:	/* F0e4_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_FMOVS_LdRegReg;
				break;
			case 0x5:	/* F0e5_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_LDREGREG;
				op->Run=BJX2_Op_FMOVD_LdRegReg;
				break;
			case 0x6:	/* F0e6_9Cnm */
				op->nmid=BJX2_NMID_FMOVS;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_FMOVS_LdDrRegReg;
				break;
			case 0x7:	/* F0e7_9Cnm */
				op->nmid=BJX2_NMID_FMOVD;
				op->fmid=BJX2_FMID_LDDRREGREG;
				op->Run=BJX2_Op_FMOVD_LdDrRegReg;
				break;
#endif

			case 0xC:	/* F0eC_9Czz */
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FCMPEQ_RegReg;
				break;
			case 0xD:	/* F0eD_9Czz */
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FCMPGT_RegReg;
				break;
			case 0xE:	/* F0eE_9Czz */
				op->nmid=BJX2_NMID_FMOV;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FMOV_RegReg;
				if(eo)
				{
					if(eq)
					{
						op->fmid=BJX2_FMID_GREGFREG;
						op->Run=BJX2_Op_FMOV_GRegFReg;
					}else
					{
						op->fmid=BJX2_FMID_FREGGREG;
						op->Run=BJX2_Op_FMOV_FRegGReg;
					}
				}
				break;
			}
			break;

		case 0xD:	/* F0ez_9Dnm */
			switch(opw1&15)
			{
			default:
				break;
			}
			break;
		case 0xE:	/* F0ez_9Enm */
			switch(opw1&15)
			{
			default:
				break;
			}
			break;
		case 0xF:	/* F0ez_9Fnm */
			switch(opw1&15)
			{
			case 0x0:	/* 9Fz0 */
				op->nmid=BJX2_NMID_FLDCF;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCF_RegReg;
				break;
			case 0x1:	/* 9Fz1 */
				op->nmid=BJX2_NMID_FLDCD;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCD_RegReg;
				break;
			case 0x2:	/* 9Fz2 */
				op->nmid=BJX2_NMID_FLDCI;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCI_RegReg;
				break;
			case 0x3:	/* 9Fz3 */
				op->nmid=BJX2_NMID_FLDCH;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FLDCH_RegReg;
				break;
			case 0x4:	/* 9Fz4 */
				op->nmid=BJX2_NMID_FSTCF;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCF_RegReg;
				break;
			case 0x5:	/* 9Fz5 */
				op->nmid=BJX2_NMID_FSTCD;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCD_RegReg;
				break;
			case 0x6:	/* 9Fz6 */
				op->nmid=BJX2_NMID_FSTCI;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCI_RegReg;
				break;
			case 0x7:	/* 9Fz7 */
				op->nmid=BJX2_NMID_FSTCH;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FSTCH_RegReg;
				break;
			case 0x8:	/* 9Fz8 */
				op->nmid=BJX2_NMID_FNEG;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FNEG_RegReg;
				break;
			case 0x9:	/* 9Fz9 */
				op->nmid=BJX2_NMID_FABS;
				op->fmid=BJX2_FMID_FREGREG;
				op->Run=BJX2_Op_FABS_RegReg;
				break;

			default:
				break;
			}
			break;
			
		}
		break;
#endif

	case 0xC:	/* F0dd_Cddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BRA;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BRA_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;
		break;
	case 0xD:	/* F0dd_Dddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BSR;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BSR_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;
		break;

#if 0
	case 0xE:	/* F0dd_Eddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BT;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BT_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;
		break;
	case 0xF:	/* F0dd_Fddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BF;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BF_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;
		break;
#endif

	default:
		ret=-1;
		break;
	}

	if(is4r)
	{
		if((op->fmid==BJX2_FMID_REGREGREG) && (op->imm!=disp11as))
		{
			op->nmid=BJX2_NMID_BREAK;
			op->Run=BJX2_Op_BREAK_None;
		}
	}

	return(ret);
}

void BJX2_Op_PREDT_Chn(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_Opcode *op1;
	op1=op->data;
	if(ctx->regs[BJX2_REG_SR]&1)
		{ op1->Run(ctx, op1); }
}

void BJX2_Op_PREDF_Chn(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_Opcode *op1;
	op1=op->data;
	if(!(ctx->regs[BJX2_REG_SR]&1))
		{ op1->Run(ctx, op1); }
}

void BJX2_Op_PAIR_Chn(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_Opcode *op1, *op2;
	op1=op->data;
	op2=op1->data;
	op1->Run(ctx, op1);
	op2->Run(ctx, op2);
}

int BJX2_DecodeOpcode_DecFJ(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1a, int opw2a)
{
	BJX2_Opcode *op1, *op2;
	int opw3a, opw4a, opw5a, opw6a;
	int opw1, opw2, opw3, opw4, opw5, opw6, opw3v, opw5v;
	u32 immb;
	u64 imm, imm57s, imm56fv;
	int ret, isxg3, isxg2or3;
	byte rm, rn, eq, eo;

	opw3a=BJX2_MemGetWord(ctx, addr+ 4);
	opw4a=BJX2_MemGetWord(ctx, addr+ 6);
	opw5a=BJX2_MemGetWord(ctx, addr+ 8);
	opw6a=BJX2_MemGetWord(ctx, addr+10);

	opw1=opw1a;
	opw2=opw2a;
	opw3=opw3a;
	opw4=opw4a;
	opw5=opw5a;
	opw6=opw6a;

	isxg3=0;
	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
		!(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2))
	{
		BJX2_RepackOpcode_DecXG3toXG2(opw1, opw2, &opw1, &opw2);
		BJX2_RepackOpcode_DecXG3toXG2(opw3, opw4, &opw3, &opw4);
		BJX2_RepackOpcode_DecXG3toXG2(opw5, opw6, &opw5, &opw6);
		isxg3=1;
	}

	isxg2or3=isxg3;
	if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
		isxg2or3=1;

	opw3v=opw3;
	opw5v=opw5;

//	if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
	if(isxg2or3)
	{
		opw3v|=0xE000;
		opw5v|=0xE000;
	}

//	if(opw3&0x0400)
//	if((opw3&0xFE00)==0xFE00)
	if((opw3v&0xFE00)==0xFE00)
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;

		op ->pc=addr;
		op1->pc=addr+6;
		
		op ->opn =opw1a;
		op ->opn2=opw2a;
		op ->opn3=opw3a;
		op1->opn =opw4a;
		op1->opn2=opw5a;
		op1->opn3=opw6a;
		
		op->fl&=~BJX2_OPFL_TWOWORD;
		op->fl|=BJX2_OPFL_JUMBO96;
		op->fl|=BJX2_OPFL_NOWEX;

#if 0
		if((opw5&0xFE00)==0xFA00)
		{
			rn	=
				(((opw2>>12)&3)<<2)|
				(((opw4>>12)&3)<<0)|
				(((opw5>> 8)&1)<<4);
			imm	=
				((opw1&255ULL)<<56) | ((opw2&4095ULL)<<44) |
				((opw3&255ULL)<<36) | ((opw4&4095ULL)<<24) |
				((opw5&255ULL)<<16) | ((opw6&65535  )<< 0) ;
			
			op ->rn=rn;
			op ->imm=imm>>32;
			op1->imm=imm;

			op->nmid=BJX2_NMID_JLDI;
			op->fmid=BJX2_FMID_IMMJREG;
			op->Run=BJX2_Op_MOV_JImmReg;

			return(1);
		}
#endif

#if 0
		if((opw5&0xFFE0)==0xF800)
		{
			rn	= (opw5&31);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;

			op ->rn=rn;
			op ->imm=imm>>32;
			op1->imm=imm;

			op->nmid=BJX2_NMID_JLDI;
			op->fmid=BJX2_FMID_IMMJREG;
			op->Run=BJX2_Op_MOV_JImmReg;

			return(1);
		}
#endif

#if 1
//		if((opw5&0xFFE0)==0xF800)
//		if((opw5&0xFFC0)==0xF800)
		if((opw5v&0xFFC0)==0xF800)
		{
//			rn	= (opw5&31);
			rn	= (opw5&63);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;

			if(isxg3)
			{
				rn	= (opw5&31);
				if(!(opw5&0x8000))
					rn += 32;
			}

			rn=BJX2_RemapGPR(ctx, rn);

			op ->rn=rn;
			op ->imm=imm;
//			op ->imm=imm>>32;
//			op1->imm=imm;

//			op->nmid=BJX2_NMID_LDI;
			op->nmid=BJX2_NMID_JLDI;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;

			return(1);
		}
#endif

#if 1
//		if((opw5&0xFFE0)==0xF840)
		if((opw5&0xFFC0)==0xF840)
		{
//			rn	= (opw5&31);
			rn	= (opw5&63);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;

			rn=BJX2_RemapGPR(ctx, rn);

			op ->rn=rn;
			op ->imm=imm;
//			op ->imm=imm>>32;
//			op1->imm=imm;

//			op->nmid=BJX2_NMID_ADD;
			op->nmid=BJX2_NMID_JADD;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_ADD_ImmReg;

			return(1);
		}
#endif

#if 1
//		if((opw5&0xFFE0)==0xF840)
		if((opw5&0xFFE0)==0xF880)
		{
//			rn	= (opw5&31);
			rn	= (opw5&63);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;
			rn=BJX2_RemapGPR(ctx, rn);

			op ->rn=rn;
			op ->imm=imm;
//			op ->imm=imm>>32;
//			op1->imm=imm;

//			op->nmid=BJX2_NMID_ADD;
			op->nmid=BJX2_NMID_PLDCXH;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_PLDCXH_ImmReg;

			op->fl|=BJX2_OPFL_REGXN;

			return(1);
		}
#endif

#if 1
//		if((opw5&0xFFE0)==0xF800)
		if((opw5&0xFBC0)==0xE800)
		{
//			op2=BJX2_ContextAllocOpcode(ctx);

//			rn	= (opw5&31);
			rn	= (opw5&63);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;
			
			rn=BJX2_RemapGPR(ctx, rn);

			op ->rn=rn;
			op ->imm=imm;
//			op ->imm=imm>>32;
//			op1->imm=imm;

//			op->nmid=BJX2_NMID_LDI;
			op->nmid=BJX2_NMID_JLDI;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			
			if(opw5&0x0400)
			{
				op->nmid=BJX2_NMID_JLDIF;
				op->Run=BJX2_Op_MOVPF_ImmReg;
			}else
			{
				op->nmid=BJX2_NMID_JLDIT;
				op->Run=BJX2_Op_MOVPT_ImmReg;
			}

			return(1);
		}
#endif

#if 1
		opw5v=opw5;
		immb=0x01000000U|((opw1&255)<<16)|(opw2&65535);

//		if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
		if(isxg2or3)
		{
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_XMOV))
			{
				return(0);
			}

			if(!(opw5&0x8000))
				immb|=0x40000000U;
			if(!(opw5&0x4000))
				immb|=0x20000000U;
			if(!(opw5&0x2000))
				immb|=0x10000000U;
			opw5v|=0xE000;
		}

		imm57s	=
			((opw1&255ULL)<<48) |	((opw2&65535ULL)<<32) |
			((opw3&255ULL)<<24) |	((opw4&65535ULL)<< 8) |
									((opw6&  255ULL)<< 0) ;
		if(opw6&0x0100)
			imm57s|=(-1ULL)<<56;
		imm56fv=
			((imm57s&0x00FFFC0000000000ULL)<<8)	|
			((imm57s&0x000003FFF0000000ULL)<<6)	|
			((imm57s&0x000000000FFFC000ULL)<<4)	|
			((imm57s&0x0000000000003FFFULL)<<2)	;

//		if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
		if(isxg2or3)
		{
			if(!(opw1&0x8000))
				{ imm57s^=(1ULL<<62); }
			if(!(opw1&0x4000))
				{ imm57s^=(1ULL<<61); }
			if(!(opw1&0x2000))
				{ imm57s^=(1ULL<<60); }
			if(!(opw3&0x8000))
				{ imm57s^=(1ULL<<59); }
			if(!(opw3&0x4000))
				{ imm57s^=(1ULL<<58); }
			if(!(opw3&0x2000))
				{ imm57s^=(1ULL<<57); }
			if(!(opw5&0x2000))
				{ imm57s^=(1ULL<<56); }
		}

		eq=((opw6&0x0800)!=0);
		eo=((opw6&0x0100)!=0);

		if((opw5v&0xFF00)==0xF200)
		{
			rn=(opw5>>4)&15;
			rm=(opw5   )&15;
			if(opw6&0x0400)rn+=16;
			if(opw6&0x0200)rm+=16;
			if(immb&0x40000000U)rn+=32;
			if(immb&0x20000000U)rm+=32;

			op ->rm=rm;
			op ->rn=rn;
			op ->imm=imm57s;

			ret=0;
			switch((opw6>>12)&15)
			{
			case 0x0:	/* F2ed_0dzz */
				op->imm=imm57s;
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_ADD_RegImmReg;
				break;

			case 0x1:	/* F2ed_1dzz */
				if(eq)
				{
					op->imm=(imm57s<<8)|
						((imm57s>>55)&0xFE)|
						((imm57s>>63)&0x01);
					op->nmid=BJX2_NMID_FADD;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_FADD_RegImm56Reg;
					break;
				}
				break;

			case 0x2:	/* F2ed_2dzz */
#if 1
				if(eq)
				{
					/* Deprecated */
					op->imm=(imm57s<<8)|
						((imm57s>>55)&0xFE)|
						((imm57s>>63)&0x01);
					op->nmid=BJX2_NMID_FMUL;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_FMUL_RegImm56Reg;
					break;
				}
#endif
				break;

			case 0x3:	/* F2ed_3dzz */
#if 1
				if(eq)
				{
					/* Deprecated */
					op->imm=(imm57s<<8)|
						((imm57s>>55)&0xFE)|
						((imm57s>>63)&0x01);
					op->nmid=BJX2_NMID_FADD;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_FADD_RegImm56Reg;
					break;
				}
#endif
				break;

			case 0x4:	/* F2ed_4dzz */
				if(eq)
				{
					op->imm=(imm57s<<8)|
						((imm57s>>55)&0xFE)|
						((imm57s>>63)&0x01);
					op->nmid=BJX2_NMID_FMUL;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_FMUL_RegImm56Reg;
					break;
				}
				break;

			case 0x5:	/* F2nm_5ejj */
				op->imm=imm57s;
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_AND_RegImmReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RSUB;
					op->Run=BJX2_Op_RSUB_RegImmReg;
				}
				break;

			case 0x6:	/* F2nm_6ejj */
				if(eq)
				{
#if 0
					op->imm=(imm57s<<8)|
//						((imm57s>>55)&0xFE);
						(((imm57s>>55)^(((s64)imm57s)>>63))&0xFE);
//						((imm57s>>55)&0xFE)|
//						((imm57s>>63)&0x01);
					
					if(eo)
					{
						op->nmid=BJX2_NMID_FMUL;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_FMUL_RegImm56Reg;
					}else
					{
						op->nmid=BJX2_NMID_FADD;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_FADD_RegImm56Reg;
					}
					break;
#endif

					break;
				}

				op->imm=imm57s;
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_OR_RegImmReg;
				break;
			case 0x7:	/* F2nm_7ejj */
				if(eq)
				{
					op->imm=imm57s;
//					op->nmid=BJX2_NMID_SHADX;
//					op->fmid=BJX2_FMID_REGIMMREG;
//					op->Run=BJX2_Op_SHADX_RegImmReg;
					break;
				}

				op->imm=imm57s;
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_XOR_RegImmReg;
				break;

			case 0x8:	/* F2nm_8ejj */
				op->imm=imm56fv;
				if(eq)
				{
					if(eo)
					{
						op->nmid=BJX2_NMID_PMULXF;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PMULXF_RegImmReg;
					}else
					{
						op->nmid=BJX2_NMID_PADDXF;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PADDXF_RegImmReg;
					}
				}else
				{
					if(eo)
					{
						op->nmid=BJX2_NMID_PMULH;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PMULH_RegImmReg;
					}else
					{
						op->nmid=BJX2_NMID_PADDH;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PADDH_RegImmReg;
					}
				}
				break;

			case 0x9:	/* F2nm_9ejj */
				op->imm=imm56fv;
				if(eq)
				{
					if(eo)
					{
						op->nmid=BJX2_NMID_PSHMULXF;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PSHMULXF_RegImmReg;
					}else
					{
						op->nmid=BJX2_NMID_PSHADDXF;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PSHADDXF_RegImmReg;
					}
				}else
				{
					if(eo)
					{
						op->nmid=BJX2_NMID_PSHMULH;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PSHMULH_RegImmReg;
					}else
					{
						op->nmid=BJX2_NMID_PSHADDH;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_PSHADDH_RegImmReg;
					}
				}
				break;
			}
		}
#endif

	}else
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;

		opw3v=opw3;

//		immb=((opw1&255)<<14)|(opw2&16383);
//		immb=((opw1&255)<<16)|(opw2&65535);
		immb=0x01000000U|((opw1&255)<<16)|(opw2&65535);

//		if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
		if(isxg2or3)
		{
			if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_XMOV))
				return(0);

			if(!(opw3&0x8000))
				immb|=0x40000000U;
			if(!(opw3&0x4000))
				immb|=0x20000000U;
			if(!(opw3&0x2000))
				immb|=0x10000000U;
			opw3v|=0xE000;


			if(!(opw1&0x8000))
				immb|=0x80000000U;
			if(!(opw1&0x4000))
				immb|=0x08000000U;
			if(!(opw1&0x2000))
				immb|=0x04000000U;
		}

//		switch((opw3>>8)&15)
//		switch((opw3>>8)&255)
		switch((opw3v>>8)&255)
		{
		case 0xE0:		case 0xE4:
			ret=BJX2_DecodeOpcode_DecD4(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xE1:		case 0xE5:
			ret=BJX2_DecodeOpcode_DecD5(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xE2:		case 0xE6:
			ret=BJX2_DecodeOpcode_DecD6(ctx, op, addr, opw3, opw4, immb);
			break;			
			
		case 0xF0:		case 0xF4:
			ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xF1:		case 0xF5:
			ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xF2:		case 0xF6:
			ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw3, opw4, immb);
			break;

		case 0x71:
		case 0x72:	case 0x73:
		case 0x74:	case 0x75:
		case 0x76:	case 0x77:
//			if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
			if(isxg2or3)
				break;
		
			opw5=0xF000|(opw3&0x00FF);
			opw6=opw4;
			immb|=(opw3&0x0700)<<20;
//			if(opw3&0x0800)
//				opw5|=0x0400;

			ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw5, opw6, immb);
			break;

		case 0x91:
		case 0x92:	case 0x93:
		case 0x94:	case 0x95:
		case 0x96:	case 0x97:
//			if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
			if(isxg2or3)
				break;

			opw5=0xF000|(opw3&0x00FF);
			opw6=opw4;
			immb|=(opw3&0x0600)<<20;

//			if(opw3&0x0800)
//				opw5|=0x0400;
			if(opw3&0x0100)
				opw5|=0x0100;
			else
				opw5|=0x0200;
			
			if(opw3&0x0100)
				ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw5, opw6, immb);
			else
				ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw5, opw6, immb);
			break;

		case 0xFA:
		case 0xFB:
//			op->fl|=BJX2_OPFL_TWOWORD;
//			op->opn=opw1;
//			op->opn2=opw2;
			if(opw3&0x0100)
			{
				op->imm=
					0xFFFF000000000000ULL|
//					(((u64)immb)<<24)|
					((immb&0x00FFFFFFULL)<<24)|
					((opw3&255)<<16)|
					((u16)opw4);
				op->nmid=BJX2_NMID_LDIN;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIN_Imm48;
				
				if(opw1&0x0100)
					{ op->Run=BJX2_Op_BSR_Abs; }
			}else
			{
				op->imm=
//					(((u64)immb)<<24)|
					((immb&0x00FFFFFFULL)<<24)|
					((opw3&255)<<16)|
					((u16)opw4);
				op->nmid=BJX2_NMID_LDIZ;
				op->fmid=BJX2_FMID_IMM;
				op->Run=BJX2_Op_LDIZ_Imm48;
				
				if(opw1&0x0100)
					{ op->Run=BJX2_Op_BRA_Abs; }
			}
			break;
			
			
		default:
			ret=-1;
			break;
		}

		op ->opn =opw1a;
		op ->opn2=opw2a;
		op1->opn =opw3a;
		op1->opn2=opw4a;

		op ->pc=addr;
		op1->pc=addr+4;

		op->fl&=~BJX2_OPFL_TWOWORD;
		op->fl|=BJX2_OPFL_JUMBO64;

		if(isxg3)
		{
			op->fl|=BJX2_OPFL_RV64;
		}

		return(ret);
	}

	return(0);
}

int BJX2_DecodeOpcode_DecFK(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1a, int opw2a)
{
	BJX2_Opcode *op1, *op2;
	int opw3a, opw4a, opw5a, opw6a;
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int opw3v;
	u32 imma, immb, immc;
	u64 imm;
	int ret, isxg3, isxg2or3;
	byte rn;

	opw3a=BJX2_MemGetWord(ctx, addr+ 4);
	opw4a=BJX2_MemGetWord(ctx, addr+ 6);
	opw5a=BJX2_MemGetWord(ctx, addr+ 8);
	opw6a=BJX2_MemGetWord(ctx, addr+10);

	opw1=opw1a;
	opw2=opw2a;
	opw3=opw3a;
	opw4=opw4a;
	opw5=opw5a;
	opw6=opw6a;

	isxg3=0;
	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
		!(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2))
	{
		BJX2_RepackOpcode_DecXG3toXG2(opw1, opw2, &opw1, &opw2);
		BJX2_RepackOpcode_DecXG3toXG2(opw3, opw4, &opw3, &opw4);
		BJX2_RepackOpcode_DecXG3toXG2(opw5, opw6, &opw5, &opw6);
		isxg3=1;
	}
	
	isxg2or3=isxg3;
	if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
		isxg2or3=1;

	if((opw3&0xFE00)==0xFA00)
	{
		ret=BJX2_DecodeOpcode_DecFJ(ctx,
			op, addr, opw1a, opw2a);
		return(ret);
	}

	if(	((opw1&0xFF00)==0x7800)	&&
		((opw3&0xFC00)==0xF400)	&&
//		!(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2))
		!isxg2or3)
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op2=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;
		op1->data=op2;

		imma=0x02000000U|((opw1&255)<<16)|(opw2&65535);
//		immb=0x02000000U|((imma<<0)&0xF00000)|((imma>>4)&0x0F0F);
//		immc=0x02000000U|((imma<<4)&0xF00000)|((imma>>0)&0x0F0F);
//		if(immb&0x100000)immb|=0x0F0;
//		if(immc&0x100000)immc|=0x0F0;

		immb=0x02000000U|((imma<<0)&0xF00000)|
			((imma>>4)&0x080F)|((imma>>8)&0x0070);
		immc=0x02000000U|((imma<<4)&0xF00000)|
			((imma>>0)&0x080F)|((imma>>4)&0x0070);

//		if(immb&0x100000)immb|=0x780;
//		if(immc&0x100000)immc|=0x780;
		if(immb&0x0040)immb|=0x0780;
		if(immc&0x0040)immc|=0x0780;
		if(	(((opw3>>8)&3)==0) &&
			(((opw4>>12)&3)==0) &&
			(opw4&4) && (imma&0x0800))
		{
			immc&=~0x0600;
			immc|=((imma<<1)&0x0600);
		}

		switch((opw3>>8)&255)
		{
		case 0xF0:		case 0xF4:
			ret=BJX2_DecodeOpcode_DecF0(ctx, op1, addr, opw3, opw4, immb);
			break;
		case 0xF1:		case 0xF5:
			ret=BJX2_DecodeOpcode_DecF1(ctx, op1, addr, opw3, opw4, immb);
			break;
		case 0xF2:		case 0xF6:
			ret=BJX2_DecodeOpcode_DecF2(ctx, op1, addr, opw3, opw4, immb);
			break;			
		case 0xF8:		case 0xFC:
			ret=BJX2_DecodeOpcode_DecF8(ctx, op1, addr, opw3, opw4, immb);
			break;			
		case 0xE0:		case 0xE4:
			ret=BJX2_DecodeOpcode_DecD4(ctx, op1, addr, opw3, opw4, immb);
			break;
		case 0xE1:		case 0xE5:
			ret=BJX2_DecodeOpcode_DecD5(ctx, op1, addr, opw3, opw4, immb);
			break;
		case 0xE2:		case 0xE6:
			ret=BJX2_DecodeOpcode_DecD6(ctx, op1, addr, opw3, opw4, immb);
			break;			
		case 0xE8:		case 0xEC:
			ret=BJX2_DecodeOpcode_DecD8(ctx, op1, addr, opw3, opw4, immb);
			break;			
			
		default:
			ret=-1;
			break;
		}

		switch((opw5>>8)&255)
		{
		case 0xF0:		case 0xF4:
			ret=BJX2_DecodeOpcode_DecF0(ctx, op2, addr, opw5, opw6, immc);
			break;
		case 0xF1:		case 0xF5:
			ret=BJX2_DecodeOpcode_DecF1(ctx, op2, addr, opw5, opw6, immc);
			break;
		case 0xF2:		case 0xF6:
			ret=BJX2_DecodeOpcode_DecF2(ctx, op2, addr, opw5, opw6, immc);
			break;			
		case 0xF8:		case 0xFC:
			ret=BJX2_DecodeOpcode_DecF8(ctx, op2, addr, opw5, opw6, immc);
			break;			
		case 0xE0:		case 0xE4:
			ret=BJX2_DecodeOpcode_DecD4(ctx, op2, addr, opw5, opw6, immc);
			break;
		case 0xE1:		case 0xE5:
			ret=BJX2_DecodeOpcode_DecD5(ctx, op2, addr, opw5, opw6, immc);
			break;
		case 0xE2:		case 0xE6:
			ret=BJX2_DecodeOpcode_DecD6(ctx, op2, addr, opw5, opw6, immc);
			break;			
		case 0xE8:		case 0xEC:
			ret=BJX2_DecodeOpcode_DecD8(ctx, op2, addr, opw5, opw6, immc);
			break;			
			
		default:
			ret=-1;
			break;
		}

		op ->opn =opw1a;
		op ->opn2=opw2a;

		op1->opn=0xD000|
			((imma>>12)&0x0F00)|
			((imma>> 8)&0x00F0)|
			((imma>> 4)&0x000F);
		op1->opn2=opw3;
		op1->opn3=opw4;

		op2->opn=0xD000|
			((imma>> 8)&0x0F00)|
			((imma>> 4)&0x00F0)|
			((imma>> 0)&0x000F);
		op2->opn2=opw5;
		op2->opn3=opw6;

		op ->pc=addr;
		op1->pc=addr+3;
		op2->pc=addr+5;

		op->fl&=~BJX2_OPFL_TWOWORD;
		op1->fl&=~BJX2_OPFL_TWOWORD;
		op2->fl&=~BJX2_OPFL_TWOWORD;

		op->fl|=BJX2_OPFL_JUMBO96;
		op1->fl|=BJX2_OPFL_TRIWORD;
		op2->fl|=BJX2_OPFL_TRIWORD;
		
		op->nmid=BJX2_NMID_NONE;

		return(ret);
	}
	
	if(1)
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;

//		immb=((opw1&255)<<14)|(opw2&16383);
//		immb=((opw1&255)<<16)|(opw2&65535);
		immb=0x02000000U|((opw1&255)<<16)|(opw2&65535);
		
		immb|=(opw1&0x00F0)<<24;
		
		opw3v=opw3;
		
		if(isxg2or3)
		{
			if(!(opw3&0x8000))
				immb|=1<<30;
			if(!(opw3&0x4000))
				immb|=1<<29;
			if(!(opw3&0x2000))
				immb|=1<<28;
			opw3v|=0xE000;
		}
		
//		switch((opw3>>8)&15)
//		switch((opw3>>8)&255)
		switch((opw3v>>8)&255)
		{
		case 0xF0:		case 0xF4:
			ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xF1:		case 0xF5:
			ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xF2:		case 0xF6:
			ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw3, opw4, immb);
			break;			

		case 0xF8:		case 0xFC:
			ret=BJX2_DecodeOpcode_DecF8(ctx, op, addr, opw3, opw4, immb);
			break;			

		case 0xE0:		case 0xE4:
			ret=BJX2_DecodeOpcode_DecD4(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xE1:		case 0xE5:
			ret=BJX2_DecodeOpcode_DecD5(ctx, op, addr, opw3, opw4, immb);
			break;
		case 0xE2:		case 0xE6:
			ret=BJX2_DecodeOpcode_DecD6(ctx, op, addr, opw3, opw4, immb);
			break;			

		case 0xE8:		case 0xEC:
			ret=BJX2_DecodeOpcode_DecD8(ctx, op, addr, opw3, opw4, immb);
			break;			

		default:
			ret=-1;
			break;
		}

		op ->opn =opw1a;
		op ->opn2=opw2a;
		op1->opn =opw3a;
		op1->opn2=opw4a;

		op ->pc=addr;
		op1->pc=addr+4;

		op->fl&=~BJX2_OPFL_TWOWORD;
		op->fl|=BJX2_OPFL_JUMBO64;

		if(isxg3)
		{
			op->fl|=BJX2_OPFL_RV64;
		}

		return(ret);
	}

	return(0);
}

#if 0
int BJX2_DecodeOpcode_DecF4(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw1b;
	int ret;
		
	if(!(ctx->regs[BJX2_REG_SR]&(1<<27)))
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op1->pc=addr;

		opw1b=opw1&0xFFEF;
		ret=BJX2_DecodeOpcode_DecF0(ctx, op1, addr, opw1b, opw2, 0);

		if(opw1&0x0010)
		{
			op->nmid=BJX2_NMID_PRED_F;
			op->fmid=BJX2_FMID_CHAIN;
			op->Run=BJX2_Op_PREDF_Chn;
			op->data=op1;
		}else
		{
			op->nmid=BJX2_NMID_PRED_T;
			op->fmid=BJX2_FMID_CHAIN;
			op->Run=BJX2_Op_PREDT_Chn;
			op->data=op1;
		}

		return(ret);
	}
	
	ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw1, opw2, 0);
	return(ret);
}
#endif

int BJX2_DecodeOpcode_DecD4(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	BJX2_Opcode *op1;
	int ret;

	if(!(op->fl&BJX2_OPFL_RV64))
	{
		op->fl|=BJX2_OPFL_TWOWORD;
		op->opn=opw1;
		op->opn2=opw2;
	}

	/* Pred+BRA is Equivalent to BT/BF. */
	if(((opw1&0xFB00)==0xE000) && (opw2&0xF000)==0xC000)
	{
		ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw1, opw2, jbits);

		if(opw1&0x0400)
		{
			op->nmid=BJX2_NMID_BF;
			op->Run=BJX2_Op_BF_PcDisp;
		}else
		{
			op->nmid=BJX2_NMID_BT;
			op->Run=BJX2_Op_BT_PcDisp;
		}

		return(ret);
	}

	op1=op->data;
	if(!op1)
		op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;


	ret=BJX2_DecodeOpcode_DecF0(ctx, op1, addr, opw1, opw2, jbits);
	op->fl|=op1->fl&BJX2_OPFL_PREDMSK;

	if(!op1->Run || !op1->opn)
	{
		op->nmid=BJX2_NMID_INV;
		op->fmid=BJX2_FMID_Z;
//		op->Run=BJX2_Op_INVOP_None;
		op->Run=BJX2_Op_BREAK_None;
		op->fl|=BJX2_OPFL_CTRLF;
		return(ret);
	}

	if(opw1&0x0400)
	{
		op->nmid=BJX2_NMID_PRED_F;
		op->fmid=BJX2_FMID_CHAIN;
		op->Run=BJX2_Op_PREDF_Chn;
		op->data=op1;
	}else
	{
		op->nmid=BJX2_NMID_PRED_T;
		op->fmid=BJX2_FMID_CHAIN;
		op->Run=BJX2_Op_PREDT_Chn;
		op->data=op1;
	}

	return(ret);
}

#if 0
int BJX2_DecodeOpcode_DecHz(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw3, opw4;
	int ret, z, imm;

	op->fl|=BJX2_OPFL_OP24;
	op->opn=opw1;
	op->opn2=opw2;

	if((opw1&0xF000)==0x9000)
	{
		opw3=0xF000|(opw1&0x00FF);
		opw4=0x0000|
			((opw2<<4)&0x0FF0) |
			((opw1>>8)&0x000F) ;
		ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, 0);
		op->opn=opw1;
		op->opn2=opw2&0x00FF;

		return(ret);
	}

	if((opw1&0xF800)==0x7800)
	{
		opw3=0xF000|(opw1&0x00FF);
		opw4=0x1000|
			((opw2<<4)&0x0FF0) |
			((opw1>>8)&0x000F) ;
		ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, 0);

		op->opn=opw1;
		op->opn2=opw2&0x00FF;
		return(ret);
	}

	if(((opw1&0xF800)==0x7000) && ((opw2&0x0080)==0x0000))
	{
		opw3=0xF000|(opw1&0x00FF);
		opw4=0x1000|
			((opw2<<4)&0x0FF0) |
			((opw1>>8)&0x000F) ;
		ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, 0);

		op->opn=opw1;
		op->opn2=opw2&0x00FF;
		return(ret);
	}

	if(((opw1&0xF800)==0x7000) && ((opw2&0x0080)==0x0080))
	{
		z=-1;
		imm=opw2&0x1F;
		
		if((opw1&0x0F00)==0x0000)	z=0x0000;
		if((opw1&0x0F00)==0x0100)	{ z=0x1000; imm=-imm; }
		if((opw1&0x0F00)==0x0200)	z=0x2000;
		if((opw1&0x0F00)==0x0300)	z=0x2800;
		if((opw1&0x0F00)==0x0400)	z=0x8000;
		if((opw1&0x0F00)==0x0500)	z=0x81E0;
		if((opw1&0x0F00)==0x0600)	z=0x9000;
		if((opw1&0x0F00)==0x0700)	z=0x91E0;

		if(z>=0)
		{
			opw3=0xF200|(opw1&0x00FF);
			opw4=	(z                )	|
					((opw2<< 4)&0x0E00) |
					((imm     )&0x01FF) ;
			ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw3, opw4, 0);

			op->opn=opw1;
			op->opn2=opw2&0x00FF;
			return(ret);
		}
	}

	return(0);
}
#endif


int BJX2_DecodeOpcode_DecXz(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw3, opw4;
	int ret, z, imm;

	if(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2)
	{
		return(-1);
	}

	if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_XGPR))
		return(-1);

	if((opw1&0xFF00) == 0x7800)
	{
		ret=BJX2_DecodeOpcode_DecFK(ctx, op, addr, opw1, opw2);
		return(ret);
	}

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

	if((opw1&0xF000)==0x7000)
	{
		opw3=0xF000|(opw1&0x00FF);
		opw4=opw2;
		imm=(opw1&0x0700)<<20;
		if(opw1&0x0800)
			opw3|=0x0400;
		
		ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, imm);
		op->opn=opw1;
		op->opn2=opw2;

		return(ret);
	}

	if((opw1&0xF000)==0x9000)
	{
		opw3=0xF000|(opw1&0x00FF);
		opw4=opw2;
		imm=(opw1&0x0600)<<20;
		if(opw1&0x0800)
			opw3|=0x0400;
		if(opw1&0x0100)
			opw3|=0x0100;
		else
			opw3|=0x0200;
		
		if(((opw4>>12)&15)==4)
			{ ret=-1; }
		
		if(opw1&0x0100)
			ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw3, opw4, imm);
		else
			ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw3, opw4, imm);
		op->opn=opw1;
		op->opn2=opw2;

		return(ret);
	}

	return(0);
}

int BJX2_RepackOpcode_DecXG3toXG2(
	int opw1, int opw2,
	int *ropw3, int *ropw4)
{
	u32 opw;
	int opw3, opw4;
	int imm;
	int ret;

	opw=((u16)opw1)|(opw2<<16);
	
	if((opw&3)==3)
	{
		*ropw3=0xF000;
		*ropw4=0x3030;
		return(0);
	}
	
	ret=0;
	switch((opw>>2)&7)
	{
	case 0:
	case 3:
	case 5:
#if 0
		if(((opw1>>12)&0xE)==0xC)
		{
			imm=(opw2&0xFFFF)|(((opw1>>6)&63)<<16);
			if(opw1&0x20)
				imm|=~0x003FFFFFULL;
			opw3=0xF000|((imm>>12)&0xFF);
			opw4=(opw1&0xF000)|(imm&0x0FFF);
			if(((imm>>20)^(imm>>31))&1)
				opw3^=0x2000;
			if(((imm>>21)^(imm>>31))&1)
				opw3^=0x4000;
			if(((imm>>22)^(imm>>31))&1)
				opw3^=0x8000;
			break;
		}
#endif

		if(((opw1>>12)&0xE)==0xC)
		{
			imm=(opw2&0xFFFF)|(((opw1>>6)&63)<<16);
		}
	
		opw3=
			((((opw>>11)^1)& 1)<<15) |
			((((opw>>21)^1)& 1)<<14) |
			((((opw>>27)^1)& 1)<<13) |
			((((opw>> 1)^0)& 1)<<12) |
			((((opw>> 4)^0)& 1)<<11) |
			((((opw>> 2)^0)& 3)<< 8) |
			((((opw>> 6)^0)&15)<< 4) |
			((((opw>>16)^0)&15)<< 0) ;
		opw4=
			(((opw>>12)&15)<<12) |
			(((opw>> 5)& 1)<<11) |
			(((opw>>10)& 1)<<10) |
			(((opw>>20)& 1)<< 9) |
			(((opw>>26)& 1)<< 8) |
			(((opw>>22)&15)<< 4) |
			(((opw>>28)&15)<< 0) ;
		break;
	case 1:
	case 2:
		opw3=
			((((opw>>11)^1)& 1)<<15) |
			((((opw>>21)^1)& 1)<<14) |
			((((opw>>31)^1)& 1)<<13) |
			((((opw>> 1)^0)& 1)<<12) |
			((((opw>> 4)^0)& 1)<<11) |
			((((opw>> 2)^0)& 3)<< 8) |
			((((opw>> 6)^0)&15)<< 4) |
			((((opw>>16)^0)&15)<< 0) ;
		opw4=
			(((opw>>12)&15)<<12) |
			(((opw>> 5)& 1)<<11) |
			(((opw>>10)& 1)<<10) |
			(((opw>>20)& 1)<< 9) |
			(((opw>>26)&31)<< 4) |
			(((opw>>22)&15)<< 0) ;
		break;
	case 4:
		opw4=opw2;
		opw3=
			((((opw>>11)^1)& 1)<<15) |
			((((opw>>15)^1)& 1)<<14) |
			((((opw>> 5)^1)& 1)<<13) |
			((((opw>> 1)^0)& 1)<<12) |
			((((opw>> 4)^0)& 1)<<11) |
			((((opw>> 2)^0)& 3)<< 8) |
			(((opw>> 6)&31)<<0) |
			(((opw>>12)& 7)<<5) ;
		break;
	case 6:
	case 7:
		opw3=0x1E00|
			((((opw>>5)^7)& 7)<<13) |
			((((opw>>2)^0)& 1)<< 8) |
			( ((opw>>8)  &255)<< 0) ;
		opw4=opw2;
		break;
	}
	
	if(!(opw&2))
	{
		opw3&=~0x1400;
		if(opw&1)
			opw3|=0x0400;
	}
	
	*ropw3=opw3;
	*ropw4=opw4;
	return(0);
}

int BJX2_DecodeOpcode_DecPair14(BJX2_Context *ctx,
	BJX2_Opcode *op, u16 opw)
{
//	static byte rm3remap[8]={18,19,26,27,20,21,10,11};
	static byte rm3remap[8]={8,9,10,11,20,21,22,23};
	int rm5, rn5, rm3, rn3, rm5_o, rn5_o, rm3_o, rn3_o, rm3b, rn3b;
	int i5s;

	rn5_o=(opw>>3)&31;
	rm5_o=(opw>>8)&31;
	rn3_o=8+((opw>>3)&7);
	rm3_o=8+((opw>>8)&7);
	
	i5s=rm5_o;
	if(rm5_o&16)
		i5s|=~31;

	rn5=BJX2_RemapGPR(ctx, rn5_o);
	rm5=BJX2_RemapGPR(ctx, rm5_o);

	rn3=BJX2_RemapGPR(ctx, rn3_o);
	rm3=BJX2_RemapGPR(ctx, rm3_o);

	rn3b=BJX2_RemapGPR(ctx, rm3remap[rn3_o&7]);
	rm3b=BJX2_RemapGPR(ctx, rm3remap[rm3_o&7]);

	op->rn=rn5;
	op->rm=rm5;
	op->imm=i5s;

	op->opn=opw;

	op->rp=BJX2_REG_ZZR;
	op->rq=BJX2_REG_GBR_HI;


	if(opw&0x2000)
	{
		switch(opw&7)
		{
		case 0:
			op->rm=BJX2_REG_SP;
			op->rn=rn5;
			op->imm=rm5_o;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			break;
		case 1:
			op->rm=BJX2_REG_SP;
			op->rn=rn5;
			op->imm=rm5_o;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;
		case 2:
			op->rn=BJX2_REG_SP;
			op->rm=rn5;
			op->imm=rm5_o;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 3:
			op->rn=BJX2_REG_SP;
			op->rm=rn5;
			op->imm=rm5_o;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;

		case 4:
			op->rm=rm3;
			op->rn=rn3;
			op->imm=(opw>>11)&3;
			switch((opw>>6)&0x3)
			{
			case 0:
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVL_LdRegDispReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVQ_LdRegDispReg;
				break;
			case 2:
				op->rn=rm3;
				op->rm=rn3;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVL_RegStRegDisp;
				break;
			case 3:
				op->rn=rm3;
				op->rm=rn3;
				op->imm=(opw>>11)&3;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVQ_RegStRegDisp;
				break;
			}
			break;
		case 5:
			op->rn=rn3;
			op->rm=rm3;
			op->imm=0;
			switch(((opw>>6)&0x3)|((opw>>9)&0xC))
			{
			case 0:
				op->rm=rm3;
				op->rn=rn3;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVB_LdRegDispReg;
				break;
			case 1:
				op->rm=rm3;
				op->rn=rn3;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVW_LdRegDispReg;
				break;
			case 2:
				op->rn=rm3;
				op->rm=rn5;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVB_RegStRegDisp;
				break;
			case 3:
				op->rn=rm3;
				op->rm=rn5;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVW_RegStRegDisp;
				break;
			case 4:
				op->rm=rm3;
				op->rn=rn3;
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVUB_LdRegDispReg;
				break;
			case 5:
				op->rm=rm3;
				op->rn=rn3;
				op->nmid=BJX2_NMID_MOVUW;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVUW_LdRegDispReg;
				break;
			case 6:
				op->rm=rm3;
				op->rn=rn3;
				op->nmid=BJX2_NMID_MOVUL;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
				break;
			}
			break;


		case 7:
			op->rm=rm3b;
			op->rn=rn3b;
			op->imm=(opw>>11)&3;
			switch((opw>>6)&0x3)
			{
			case 0:
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVL_LdRegDispReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_MOVQ_LdRegDispReg;
				break;
			case 2:
				op->rn=rm3b; op->rm=rn3b;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVL_RegStRegDisp;
				break;
			case 3:
				op->rn=rm3b; op->rm=rn3b;
				op->imm=(opw>>11)&3;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREGDISP;
				op->Run=BJX2_Op_MOVQ_RegStRegDisp;
				break;
			}
			break;
		}

		return(0);
	}

	switch(opw&7)
	{
	case 0:
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_REGREG;
		op->Run=BJX2_Op_MOV_RegReg;
		break;
	case 1:
//		op->nmid=BJX2_NMID_ADD;
//		op->fmid=BJX2_FMID_REGREG;
//		op->Run=BJX2_Op_ADD_RegReg;

		op->nmid=BJX2_NMID_EXTSL;
		op->fmid=BJX2_FMID_REGREG;
		op->Run=BJX2_Op_EXTSL_RegReg;
		break;
	case 2:
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;
	case 3:
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_ADD_ImmReg;
		break;
	case 4:
		op->rm=rn5;
		op->ro=rm5;
		op->rn=rn5;
		op->nmid=BJX2_NMID_ADDSL;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADDSL_RegImmReg;
		break;
	case 5:
		op->rn=rn3;
		op->rm=rm3;
		switch(((opw>>6)&0x3)|((opw>>9)&0xC))
		{
		case 0x0:
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_SUB_RegReg;
			break;
		case 0x1:
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_XOR_RegReg;
			break;
		case 0x2:
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_AND_RegReg;
			break;
		case 0x3:
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_OR_RegReg;
			break;

		case 0x4:
			op->rm=rn3;
			op->ro=rm3;
			op->rn=rn3;
			op->nmid=BJX2_NMID_SUBSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUBSL_RegRegReg;
			break;
		case 0x5:
			op->rm=rn3;
			op->ro=rm3;
			op->rn=rn3;
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDSL_RegRegReg;
			break;
		case 0x6:
			op->rm=rn3;
			op->ro=rm3;
			op->rn=rn3;
			op->nmid=BJX2_NMID_SUBUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUBUL_RegRegReg;
			break;
		case 0x7:
			op->rm=rn3;
			op->ro=rm3;
			op->rn=rn3;
			op->nmid=BJX2_NMID_ADDUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDUL_RegRegReg;
			break;

		case 0x8:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLDQ_RegRegReg;
			break;
		case 0x9:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHADQ_RegRegReg;
			break;
		case 0xA:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLRQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLRQ_RegRegReg;
			break;
		case 0xB:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHARQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHARQ_RegRegReg;
			break;

		case 0xC:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLD_RegRegReg;
			break;
		case 0xD:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAD_RegRegReg;
			break;
		case 0xE:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLR_RegRegReg;
			break;
		case 0xF:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHAR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAR_RegRegReg;
			break;
		}
		break;
	case 6:
		op->rn=rn3;
		op->rm=rn3;
		op->imm=rm3_o&7;
		switch(((opw>>6)&0x3)|((opw>>9)&0xC))
		{
		case 0:
			op->imm=rm3_o&7;
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
			break;
		case 1:
			op->imm=rm3_o&7;
			op->nmid=BJX2_NMID_ADDUL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDUL_RegImmReg;
			break;

		case 2:
//			op->rn=rn3;
//			op->rm=rm3;
//			op->imm=0;
//			op->nmid=BJX2_NMID_ADDSL;
//			op->fmid=BJX2_FMID_REGIMMREG;
//			op->Run=BJX2_Op_ADDSL_RegImmReg;
			op->rn=rn3; op->rm=rn3; op->ro=rm3;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADD_RegRegReg;
			break;
		case 3:
			op->rn=rn3;
			op->rm=rm3;
			op->imm=0;
			op->nmid=BJX2_NMID_ADDUL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDUL_RegImmReg;
			break;

		case 4:
			op->imm=rm3_o|(~7);
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
			break;

			break;
		case 5:
			op->imm=rm3_o|(~7);
			op->nmid=BJX2_NMID_ADDUL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDUL_RegImmReg;
			break;

		case 6:
			op->rn=rn3b; op->rm=rn3b; op->ro=rm3b;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADD_RegRegReg;
			break;
		case 7:
			op->rn=rn3b;
			op->rm=rm3b;
			op->imm=0;
//			op->nmid=BJX2_NMID_ADDUL;
//			op->fmid=BJX2_FMID_REGIMMREG;
//			op->Run=BJX2_Op_ADDUL_RegImmReg;

			op->nmid=BJX2_NMID_EXTUL;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_EXTUL_RegReg;
			break;

		case 8:
			op->imm=rm3_o&7;
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			break;
		case 9:
			op->imm=rm3_o&7;
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADQ_RegImmReg;
			break;
		case 10:
			op->imm=rm3_o&7;
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLD_RegImmReg;
			break;
		case 11:
			op->imm=rm3_o&7;
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHAD_RegImmReg;
			break;

		case 12:
			op->imm=rm3_o|(~7);
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			break;
		case 13:
			op->imm=rm3_o|(~7);
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADQ_RegImmReg;
			break;
		case 14:
			op->imm=rm3_o|(~7);
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLD_RegImmReg;
			break;
		case 15:
			op->imm=rm3_o|(~7);
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHAD_RegImmReg;
			break;
		}
		break;


	case 7:
		op->rn=rn3b;
		op->rm=rm3b;
		switch(((opw>>6)&0x3)|((opw>>9)&0xC))
		{
		case 0x0:
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_SUB_RegReg;
			break;
		case 0x1:
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_XOR_RegReg;
			break;
		case 0x2:
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_AND_RegReg;
			break;
		case 0x3:
			op->nmid=BJX2_NMID_SUB;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_OR_RegReg;
			break;

		case 0x4:
			op->rm=rn3b;
			op->ro=rm3b;
			op->rn=rn3b;
			op->nmid=BJX2_NMID_SUBSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUBSL_RegRegReg;
			break;
		case 0x5:
			op->rm=rn3b;
			op->ro=rm3b;
			op->rn=rn3b;
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDSL_RegRegReg;
			break;
		case 0x6:
			op->rm=rn3b;
			op->ro=rm3b;
			op->rn=rn3b;
			op->nmid=BJX2_NMID_SUBUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SUBUL_RegRegReg;
			break;
		case 0x7:
			op->rm=rn3b;
			op->ro=rm3b;
			op->rn=rn3b;
			op->nmid=BJX2_NMID_ADDUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDUL_RegRegReg;
			break;

#if 0
		case 0x8:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLDQ_RegRegReg;
			break;
		case 0x9:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHADQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHADQ_RegRegReg;
			break;
		case 0xA:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLRQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLRQ_RegRegReg;
			break;
		case 0xB:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHARQ;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHARQ_RegRegReg;
			break;

		case 0xC:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLD_RegRegReg;
			break;
		case 0xD:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAD_RegRegReg;
			break;
		case 0xE:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHLR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHLR_RegRegReg;
			break;
		case 0xF:
			op->rm=rn3; op->ro=rm3; op->rn=rn3;
			op->nmid=BJX2_NMID_SHAR;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_SHAR_RegRegReg;
			break;
#endif
		}
		break;
	}
	
	return(0);
}

int BJX2_DecodeOpcode_DecXG2(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1a, int opw2a)
{
	BJX2_Opcode *op1, *op2;
	u32 opwb;
	int opw1, opw2, opw3, opw4;
	int ret, z, imm;

	opw1=opw1a;
	opw2=opw2a;

	if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx, BJX2_EXTID_XG2))
		return(0);

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1a;
	op->opn2=opw2a;
	opwb=0;

	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
		!(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_XG2))
	{
		BJX2_RepackOpcode_DecXG3toXG2(opw1, opw2, &opw1, &opw2);
		opwb=(opw1a&0xFFFF)|(opw2a<<16);
		
		if(!opw1a)
		{
			BJX2_DbgPrintf(ctx,
				"BJX2_DecodeOpcode_DecXG2: %04X.%04X -> %04X_%04X\n",
				opw2a, opw1a, opw1, opw2);
				
		}
		
		if((opw1&0x1A00)==0x0A00)
		{
			op1=BJX2_ContextAllocOpcode(ctx);
			op1->pc=addr;

			op2=BJX2_ContextAllocOpcode(ctx);
			op2->pc=addr;

			opwb=(opw1a&0xFFFF)|(opw2a<<16);
			
			opw3=	((opwb>>19)&0x1FFF);
			opw4=	((opwb>> 6)&0x1FFF)|
					(((opwb>>5)&1)<<13);
			BJX2_DecodeOpcode_DecPair14(ctx, op1, opw3);
			BJX2_DecodeOpcode_DecPair14(ctx, op2, opw4);

			if(!op1->Run || !op2->Run)
			{
				printf("BJX2_DecodeOpcode_DecXG2: Fail X3C %04X %04X\n",
					opw3, opw4);
				if(!op1->Run)
				{
					printf("  %04X %d %d %d\n",
						opw3, opw3&7, (opw3>>6)&3, (opw3>>11)&3);
				}
				if(!op2->Run)
				{
					printf("  %04X %d %d %d\n",
						opw4, opw4&7, (opw4>>6)&3, (opw4>>11)&3);
				}
				return(0);
			}

			op->nmid=BJX2_NMID_PAIR;
			op->fmid=BJX2_FMID_PAIR;
			op->Run=BJX2_Op_PAIR_Chn;

			op1->data=op2;
			op->data=op1;

			op->fl|=BJX2_OPFL_RV64;
			op1->fl|=BJX2_OPFL_RV64;
			op2->fl|=BJX2_OPFL_RV64;
	
			op->fl|=BJX2_OPFL_NOWEX;
			return(1);
		}
	}

	opw3=0xE000|(opw1&0x1FFF);
	opw4=opw2;
	imm=((opw1&0xE000)^0xE000)<<15;
	ret=0;

	if((opw1&0x1E00)==0x1A00)
	{
		ret=1;
		if(opw1&0x0100)
		{
			op->imm=((opw1|(~255))<<16)|((u16)opw2);
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIN_Imm24;
		}else
		{
			op->imm=((opw1&255)<<16)|((u16)opw2);
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMM;
			op->Run=BJX2_Op_LDIZ_Imm24;
		}
		op->fl|=BJX2_OPFL_NOWEX;
		return(ret);
	}

	if((opw1&0x1F00)==0x1E00)
	{
		op->fl&=~BJX2_OPFL_TWOWORD;
		ret=BJX2_DecodeOpcode_DecFJ(ctx, op, addr, opw1a, opw2a);
		return(ret);
	}

	if((opw1&0x1F00)==0x1F00)
	{
		op->fl&=~BJX2_OPFL_TWOWORD;
		ret=BJX2_DecodeOpcode_DecFK(ctx, op, addr, opw1a, opw2a);
		return(ret);
	}

	if((opw1&0x1000)==0x0000)
	{
		if((opw1&0x1B00)==0x0000)
			{ ret=BJX2_DecodeOpcode_DecD4(ctx, op, addr, opw3, opw4, imm); }
		if((opw1&0x1B00)==0x0100)
			{ ret=BJX2_DecodeOpcode_DecD5(ctx, op, addr, opw3, opw4, imm); }
		if((opw1&0x1B00)==0x0200)
			{ ret=BJX2_DecodeOpcode_DecD6(ctx, op, addr, opw3, opw4, imm); }

		if((opw1&0x1B00)==0x0800)
			{ ret=BJX2_DecodeOpcode_DecD8(ctx, op, addr, opw3, opw4, imm); }

		if((opw1&0x1B00)==0x0A00)
		{
			ret=BJX2_DecodeOpcode_DecD4(ctx, op, addr, opw3, opw4, imm);
			op->fl|=BJX2_OPFL_WEX;
		}

		if((opw1&0x1B00)==0x0B00)
		{
			ret=BJX2_DecodeOpcode_DecD6(ctx, op, addr, opw3, opw4, imm);
			op->fl|=BJX2_OPFL_WEX;
		}

		op->opn=opw1a;
		op->opn2=opw2a;
		if((opw1a!=opw1) || (opw2a!=opw2))
			{ op->fl|=BJX2_OPFL_RV64; }
		return(ret);
	}


	if((opw1&0x1000)==0x1000)
	{
		if((opw1&0x1B00)==0x1000)
			{ ret=BJX2_DecodeOpcode_DecF0(ctx, op, addr, opw3, opw4, imm); }
		if((opw1&0x1B00)==0x1100)
			{ ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw3, opw4, imm); }
		if((opw1&0x1B00)==0x1200)
			{ ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw3, opw4, imm); }

		if((opw1&0x1B00)==0x1800)
			{ ret=BJX2_DecodeOpcode_DecF8(ctx, op, addr, opw3, opw4, imm); }

		if(opw1&0x0400)
			op->fl|=BJX2_OPFL_WEX;

		op->opn=opw1a;
		op->opn2=opw2a;
		if((opw1a!=opw1) || (opw2a!=opw2))
			{ op->fl|=BJX2_OPFL_RV64; }
		return(ret);
	}

	return(0);
}
