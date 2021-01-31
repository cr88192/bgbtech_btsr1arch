/*
 Copyright (c) 2018-2020 Brendan G Bohannon

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

int BJX2_DecodeOpcode_DecF0(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int opw3, opw4;
	int disp5, eq, eo;
	int imm8u, imm8n;
	int imm10u, imm10n;
	int imm20s;
	int ret, fnm;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;
	
	if(1)
	{
#ifdef BJX2_FLIPSTNM
		fnm=0;
		if((opw2&0xF808)==0x0000)
			fnm=1;
		if((opw2&0xF80C)==0x9000)
			fnm=1;
		if(fnm)
		{
			opw1=(opw1&0xFF00)|
				((opw1<<4)&0x00F0)|
				((opw1>>4)&0x000F);
			opw2=(opw2&0xF9FF)|
				((opw2<<1)&0x0400)|
				((opw2>>1)&0x0200);
		}
#endif

		ret=1;
		if(((opw2&0xF000)>=0xC000))
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

	disp5=(opw1   )&31;
	eq=(opw1&0x0080)?1:0;
	eo=(opw1&0x0010)?1:0;

	imm8u=opw2&255;
	imm8n=opw2|(~255);
	imm10u=(opw2&255)|((opw1&0x0060)<<3);
	imm10n=(opw2&255)|((opw1&0x0060)<<3)|(~1023);

	imm20s=((opw1&255)<<12)|(opw2&4095);
	imm20s=((s32)(imm20s<<12))>>12;

	ret=0;
	switch((opw2>>12)&15)
	{
	case 0x0:	/* F0ez_0zzz */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=disp5;

		op->fl|=BJX2_OPFL_NOWEX;

		switch((opw2>>8)&15)
		{
#if 0
		case 0x0:	/* F0ez_00zz */
			if(eq)
			{
				if(op->rm==0)
				{
					op->rm=BJX2_REG_PC;
					op->nmid=BJX2_NMID_LEAB;
					op->fmid=BJX2_FMID_LDREGDISPREG;
					op->Run=BJX2_Op_LEAB_LdPcDispReg;
					break;
				}

				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAB_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVB_RegStRegDisp;
			break;
		case 0x1:	/* F0ez_01zz */
			if(eq)
			{
				if(op->rm==0)
					break;
				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAW_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVW_RegStRegDisp;
			break;
		case 0x2:	/* F0ez_02zz */
			if(eq)
			{
				if(op->rm==0)
					break;
				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAD_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp;
			break;
		case 0x3:	/* F0ez_03zz */
			if(eq)
			{
				if(op->rm==0)
					break;
				op->nmid=BJX2_NMID_LEAQ;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_LEAQ_LdRegDispReg;
				break;
			}
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			break;
#endif

		case 0x4:	/* F0ez_04zz */
			if(eq)
			{
				if(op->rm==0)
				{
					op->rm=BJX2_REG_PC;
					op->nmid=BJX2_NMID_LEAB;
					op->fmid=BJX2_FMID_LDREG2REG;
					op->Run=BJX2_Op_LEAB_LdPcIdxReg;
					break;
				}

				if(op->rm==1)
					{ op->rm=BJX2_REG_GBR; }

				op->nmid=BJX2_NMID_LEAB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAB_LdReg2Reg;
				break;
			}

			if(op->rn==0)
			{
				if(op->ro==1)
				{
					op->nmid=BJX2_NMID_MOVB;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVB_RegStReg;
					break;
				}

				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStPcIdx;
				break;
			}
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==1)
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
		case 0x5:	/* F0ez_05zz */
			if(eq)
			{
				if(op->rm==0)
					break;
				if(op->rm==1)
					break;

				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAW_LdReg2Reg;
				break;
			}

			if(op->rn==0)
			{
				if(op->ro==1)
				{
					op->nmid=BJX2_NMID_MOVW;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVW_RegStReg;
					break;
				}

				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStPcIdx;
				break;
			}
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==1)
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
		case 0x6:	/* F0ez_06zz */
			if(eq)
			{
				if(op->rm==0)
					break;
				if(op->rm==1)
					break;

				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAD_LdReg2Reg;
				break;
			}

			if(op->rn==0)
			{
				if(op->ro==1)
				{
					op->nmid=BJX2_NMID_MOVL;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVL_RegStReg;
					break;
				}

				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStPcIdx;
				break;
			}
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==1)
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
		case 0x7:	/* F0ez_07zz */
			if(eq)
			{
				if(op->rm==0)
					break;
				if(op->rm==1)
					break;

				op->nmid=BJX2_NMID_LEAQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAQ_LdReg2Reg;
				break;
			}

			if(op->rn==0)
			{
				if(op->ro==1)
				{
					op->nmid=BJX2_NMID_MOVQ;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVQ_RegStReg;
					break;
				}

				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStPcIdx;
				break;
			}
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==1)
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

#if 0
		case 0x8:	/* F0ez_08zz */
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVB_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdRegDispReg;
			}
			break;
		case 0x9:	/* F0ez_09zz */
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVW_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdRegDispReg;
			}
			break;
		case 0xA:	/* F0ez_0Azz */
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			}
			break;
		case 0xB:	/* F0ez_0Bzz */
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVDL;
				op->Run=BJX2_Op_MOVUL_LdRegDispReg;
			}
			break;
#endif

		case 0xC:	/* F0ez_0Czz */
			if(op->rm==0)
			{
				if(op->ro==1)
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
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==1)
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
		case 0xD:	/* F0ez_0Dzz */
			if(op->rm==0)
			{
				if(op->ro==1)
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
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==1)
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
		case 0xE:	/* F0ez_0Ezz */
			if(op->rm==0)
			{
				if(op->ro==1)
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
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==1)
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
		case 0xF:	/* F0ez_0Fzz */
			if(op->rm==0)
			{
				if(op->ro==1)
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
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==1)
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
			if(eq)
			{
//				op->fmid=BJX2_FMID_REGIMMREG;
//				op->Run=BJX2_Op_ADD_RegImmReg;
				op->nmid=BJX2_NMID_ADDX;
				op->Run=BJX2_Op_ADDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
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
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x2:	/* F0ez_12zz */
			if(eq)
				break;
			op->nmid=BJX2_NMID_MULS;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULS_RegRegReg;
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
			if(eq)
				break;
			op->nmid=BJX2_NMID_MULU;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULU_RegRegReg;
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
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;

		case 0x8:	/* F0ez_18zz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
#if 1
			case 0xC:	/* F0eC_18nm */
				op->nmid=BJX2_NMID_CMPEQ;
				op->fmid=BJX2_FMID_REGREG;
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
				op->nmid=BJX2_NMID_CMPHI;
				op->fmid=BJX2_FMID_REGREG;
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
				op->nmid=BJX2_NMID_CMPGT;
				op->fmid=BJX2_FMID_REGREG;
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
//					op->nmid=BJX2_NMID_MOVD;
//					op->Run=BJX2_Op_MOVD_RegReg;
					op->nmid=BJX2_NMID_MOVX2;
					op->Run=BJX2_Op_MOVX_RegReg;
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
				op->rn+=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(eq)
				{
					op->rm=rn_dfl;
					op->rn=rm_dfl;
//					op->nmid=BJX2_NMID_MOVD;
//					op->Run=BJX2_Op_MOV_RegReg;
					op->nmid=BJX2_NMID_INVDC;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_SETTRIP_RegStReg;
				}
				break;
			case 0xB:	/* F0eB_19nm */
				op->rm+=BJX2_REG_PC;
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
				op->rn+=BJX2_REG_R0B;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0xB:	/* F0eB_1Cnm */
				op->rm+=BJX2_REG_R0B;
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
			case 0x5:	/* F0e5_1Dnm */
				op->nmid=BJX2_NMID_FSTCFH;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSTCFH_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
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
				op->nmid=BJX2_NMID_RGBPCK;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_RGB5PCK32_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB32PCK64;
					op->Run=BJX2_Op_RGB32PCK64_RegReg;
				}
				break;
			case 0x5:	/* F0e5_1Enm */
				op->nmid=BJX2_NMID_RGBPCK;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_RGB5UPCK32_RegReg;
				if(eq)
				{
					op->nmid=BJX2_NMID_RGB32UPCK64;
					op->Run=BJX2_Op_RGB32UPCK64_RegReg;
				}
				break;
			}
			break;
		case 0xF:	/* F0ez_1Fzz */
			op->ro=BJX2_REG_ZZR;
			switch(opw1&15)
			{
			case 0x0:	/* F0e0_1Fnm */
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
			op->nmid=BJX2_NMID_PADDF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDF_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PADDXF;
				op->Run=BJX2_Op_PADDXF_RegRegReg;
			}
			break;
		case 0x6:	/* F0ez_26zz */
			op->nmid=BJX2_NMID_PSUBF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBF_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSUBXF;
				op->Run=BJX2_Op_PSUBXF_RegRegReg;
			}
			break;
		case 0x7:	/* F0ez_27zz */
			op->nmid=BJX2_NMID_PMULF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULF_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PMULXF;
				op->Run=BJX2_Op_PMULXF_RegRegReg;
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
			if(eq)
			{
//				op->nmid=BJX2_NMID_PADDXD;
//				op->Run=BJX2_Op_PADDXD_RegRegReg;
			}
			break;
		case 0xB:	/* F0ez_2Bzz */
			op->nmid=BJX2_NMID_PSCHNEW;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSCHNEW_RegRegReg;
			if(eq)
			{
//				op->nmid=BJX2_NMID_PADDXD;
//				op->Run=BJX2_Op_PADDXD_RegRegReg;
			}
			break;

		case 0xC:	/* F0ez_2Czz */
			op->nmid=BJX2_NMID_BLKUTX1;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_BLKUTX1_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_BLKUTX2;
				op->Run=BJX2_Op_BLKUTX2_RegRegReg;
			}
			break;


		case 0xD:	/* F0ez_2Dzz */
			op->nmid=BJX2_NMID_PADDH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDH_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PADDXD;
				op->Run=BJX2_Op_PADDXD_RegRegReg;
			}
			break;
		case 0xE:	/* F0ez_2Ezz */
			op->nmid=BJX2_NMID_PSUBH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBF_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSUBXD;
				op->Run=BJX2_Op_PSUBXD_RegRegReg;
			}
			break;
		case 0xF:	/* F0ez_2Fzz */
			op->nmid=BJX2_NMID_PMULH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULH_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PMULXD;
				op->Run=BJX2_Op_PMULXD_RegRegReg;
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
		op->imm=ro_dfl;
		switch((opw2>>8)&15)
		{
		case 0x0:	/* F0ez_30zz */
			switch(opw2&255)
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
					break;

				case 0x2:
					op->nmid=BJX2_NMID_SLEEP;
					op->Run=BJX2_Op_SLEEP_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					break;
				case 0x3:
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
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
				break;
			case 0x11:	/* F0en_3011 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BSR_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x12:	/* F0en_3012 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BT_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x13:	/* F0en_3013 */
				op->rm=BJX2_REG_DLR;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_PCREG;
				op->Run=BJX2_Op_BF_PcReg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
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
				op->nmid=BJX2_NMID_BRA;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BRA_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x21:	/* F0ez_3021 */
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BSR_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x22:	/* F0ez_3022 */
				op->nmid=BJX2_NMID_BT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BT_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x23:	/* F0ez_3023 */
				op->nmid=BJX2_NMID_BF;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_BF_Reg;
				op->fl|=BJX2_OPFL_CTRLF;
				op->fl|=BJX2_OPFL_NOWEX;
				break;


			case 0x33:	/* 33z3 */
				op->nmid=BJX2_NMID_MOVNT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVNT_Reg;
//				op->fl|=BJX2_OPFL_NOWEX;
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
				break;
			case 0x37:	/* 33z7 */
				op->nmid=BJX2_NMID_ROTCR;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_ROTCR_Reg;
				op->fl|=BJX2_OPFL_NOWEX;
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

			case 0x6F:	/* 36zF */
				op->nmid=BJX2_NMID_MOVT;
				op->fmid=BJX2_FMID_REG;
				op->Run=BJX2_Op_MOVT_Reg;
//				op->fl|=BJX2_OPFL_NOWEX;
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


		case 0x2:	/* F0ez_32zz */
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
			op->nmid=BJX2_NMID_ROTL;
			op->fmid=BJX2_FMID_REGREGREG;
//			op->Run=BJX2_Op_ROTL_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHADX;
				op->Run=BJX2_Op_SHADX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}
			break;
		case 0x5:	/* F0ez_35zz */
			op->nmid=BJX2_NMID_ROTR;
			op->fmid=BJX2_FMID_REGREGREG;
//			op->Run=BJX2_Op_ROTR_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHLDX;
				op->Run=BJX2_Op_SHLDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX2R;
			}
			break;
		case 0x6:	/* F0ez_36zz */
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

		case 0x4:	/* F0ez_44zz */
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXM;
			op->rn=rm_dfl;
			op->rm=rn_dfl;
			if(op->rn==0)
			{
				if(op->ro==1)
				{
					op->imm=0;
					op->nmid=BJX2_NMID_MOVX2;
					op->fmid=BJX2_FMID_REGSTREG;
					op->Run=BJX2_Op_MOVX2_RegStRegDisp;
					break;
				}

				op->rn=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVX2_RegStPcIdx;
				break;
			}
			if(op->rn==1)
			{
				op->rn=BJX2_REG_GBR;
				if(op->ro==1)
					{ op->rn=BJX2_REG_TBR; op->ro=BJX2_REG_DLR; }

				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVX2_RegStReg2B;
				break;
			}

			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREG2;
			op->Run=BJX2_Op_MOVX2_RegStReg2;
			break;
	
		case 0x8:
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXN;
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVX2_LdRegDispReg;
			break;

		case 0xC:	/* F0ez_4Czz */
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->fl|=BJX2_OPFL_REGXN;
			if(op->rm==0)
			{
				if(op->ro==1)
				{
					op->imm=0;
					op->nmid=BJX2_NMID_MOVX2;
					op->fmid=BJX2_FMID_LDREGREG;
					op->Run=BJX2_Op_MOVX2_LdRegDispReg;
					break;
				}

				op->rm=BJX2_REG_PC;
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVX2_LdPcIdxReg;
				break;
			}
			if(op->rm==1)
			{
				op->rm=BJX2_REG_GBR;
				if(op->ro==1)
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

#if 0
		case 0xC:	/* F0eo_4Cnm */
			if(op->rm==0)
			{
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
			if(op->rm==0)
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
			if(op->rm==0)
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
			if(op->rm==0)
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
			break;

		case 0x2:	/* F0ez_52zz */
			op->nmid=BJX2_NMID_MULSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULSL_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MULSL_RegImmReg;
			}
			break;
		case 0x3:	/* F0ez_53zz */
			op->nmid=BJX2_NMID_MULUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_MULUL_RegRegReg;
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MULUL_RegImmReg;
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
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_FADDX;
				op->Run=BJX2_Op_FADDX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0x9:	/* F0ez_59zz */
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_FSUBX;
				op->Run=BJX2_Op_FSUBX_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				op->fl|=BJX2_OPFL_REGX3R;
			}
			break;
		case 0xA:	/* F0ez_5Azz */
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_FMULX;
				op->Run=BJX2_Op_FMULX_RegRegReg;
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

		case 0xC:	/* F0ez_5Czz */
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_ADDSL_RegRegReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_ADDUL;
				op->Run=BJX2_Op_ADDUL_RegRegReg;
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
			if(eq)
			{
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_MULUW_RegImmReg;
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
			if(rn_dfl==0)
			{
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
			if(rn_dfl==0)
			{
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
			if(rm_dfl==0)
			{
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
			if(rm_dfl==0)
			{
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
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FADD_RegRegReg;
			break;
		case 0x9:	/* 99zz */
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FSUB_RegRegReg;
			break;
		case 0xA:	/* 9Azz */
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_FREGREGREG;
			op->Run=BJX2_Op_FMUL_RegRegReg;
			break;
		case 0xB:	/* 9Bzz */
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
		break;
	case 0xD:	/* F0dd_Dddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BSR;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BSR_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		break;
	case 0xE:	/* F0dd_Eddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BT;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BT_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		break;
	case 0xF:	/* F0dd_Fddd */
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BF;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BF_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		break;

	default:
		ret=-1;
		break;
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

int BJX2_DecodeOpcode_DecFJ(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw3, opw4, opw5, opw6;
	u32 immb;
	u64 imm;
	int ret;
	byte rn;

	opw3=BJX2_MemGetWord(ctx, addr+4);
	opw4=BJX2_MemGetWord(ctx, addr+6);
	opw5=BJX2_MemGetWord(ctx, addr+8);
	opw6=BJX2_MemGetWord(ctx, addr+10);
	
//	if(opw3&0x0400)
	if((opw3&0xFE00)==0xFE00)
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;

		op ->pc=addr;
		op1->pc=addr+6;
		
		op ->opn =opw1;
		op ->opn2=opw2;
		op ->opn3=opw3;
		op1->opn =opw4;
		op1->opn2=opw5;
		op1->opn3=opw6;
		
		op->fl|=BJX2_OPFL_JUMBO96;

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
		if((opw5&0xFFE0)==0xF800)
		{
			rn	= (opw5&31);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;

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
		if((opw5&0xFFE0)==0xF840)
		{
			rn	= (opw5&31);
			imm	=
				((opw1&255ULL)<<56) |	((opw2&65535ULL)<<40) |
				((opw3&255ULL)<<32) |	((opw4&65535ULL)<<16) |
										((opw6&65535ULL)<< 0) ;

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

	}else
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;

//		immb=((opw1&255)<<14)|(opw2&16383);
		immb=((opw1&255)<<16)|(opw2&65535);
//		switch((opw3>>8)&15)
		switch((opw3>>8)&255)
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
		default:
			ret=-1;
			break;
		}

		op ->opn =opw1;
		op ->opn2=opw2;
		op1->opn =opw3;
		op1->opn2=opw4;

		op ->pc=addr;
		op1->pc=addr+4;

		op->fl&=~BJX2_OPFL_TWOWORD;
		op->fl|=BJX2_OPFL_JUMBO64;

		return(ret);
	}

	return(0);
}

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

int BJX2_DecodeOpcode_DecD4(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int ret;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

	op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF0(ctx, op1, addr, opw1, opw2, 0);
	op->fl|=op1->fl&BJX2_OPFL_PREDMSK;

	if(!op1->Run || !op1->opn)
	{
		op->nmid=BJX2_NMID_INV;
		op->fmid=BJX2_FMID_Z;
		op->Run=BJX2_Op_INVOP_None;
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
