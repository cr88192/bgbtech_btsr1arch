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

int BJX2_DecodeOpcode_DecF1(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl, cm_dfl, cn_dfl;
	int disp5, eq, eo;
//	int imm8u, imm8n;
//	int imm10u, imm10n;
//	int imm9u, imm9n, imm9us;
	s64 imm9u, imm9n, imm9us;
	int ret, fnm;

	op->fl|=BJX2_OPFL_TWOWORD;
//	op->fl|=BJX2_OPFL_NOWEX;
	op->fl|=BJX2_OPFL_NOWEX_IO2;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;

	if(1)
	{
#ifdef BJX2_FLIPSTNM
		fnm=0;
		if(	((opw2&0xC800)==0x0000)||
//			((opw2&0xE800)==0x4000))
			((opw2&0xE000)==0x4000))
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
	}


	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1   )&15;
//	ro_dfl=(opw1   )&15;
	if(opw2&0x0400)rn_dfl+=16;
	if(opw2&0x0200)rm_dfl+=16;
//	if(opw2&0x0010)ro_dfl+=16;

	if(jbits&0x40000000U)rn_dfl+=32;
	if(jbits&0x20000000U)rm_dfl+=32;

	cm_dfl=BJX2_RemapCR(ctx, rm_dfl);
	cn_dfl=BJX2_RemapCR(ctx, rn_dfl);

	rn_dfl=BJX2_RemapGPR(ctx, rn_dfl);
	rm_dfl=BJX2_RemapGPR(ctx, rm_dfl);
//	ro_dfl=BJX2_RemapGPR(ctx, ro_dfl);

//	disp5=(opw1   )&31;
	eq=(opw2&0x0800)?1:0;
	eo=(opw2&0x0100)?1:0;

//	imm8u=opw2&255;
//	imm8n=opw2|(~255);
//	imm10u=(opw2&255)|((opw1&0x0060)<<3);
//	imm10n=(opw2&255)|((opw1&0x0060)<<3)|(~1023);

//	imm9u=((opw1&31)<<4)|((opw2>>8)&15);
//	imm9n=((opw1&31)<<4)|((opw2>>8)&15)|((-1)<<9);

	imm9u=(opw2&511);
//	imm9n=(opw2&511)|((-1)<<9);
	imm9n=(opw2&511)|(~511);
	imm9us=imm9u;

//	if(jbits)
	if(jbits&0x01000000U)
	{
//		imm9u=(opw2&511)|(jbits<<9);
//		imm9n=(opw2&511)|(jbits<<9)|((~0U)<<31);
//		imm9us=((s32)(imm9u<<1))>>1;

//		imm9u=(opw2&255)|(jbits<<8);
//		imm9n=(opw2&255)|(jbits<<8)|((~0U)<<31);
//		imm9us=(s32)imm9u;

		imm9u=(u32)((opw2&255)|(jbits<<8));
		if(eo)imm9u|=0xFFFFFFFF00000000LL;
		imm9n=imm9u;
		imm9us=imm9u;
	}

//	if(jbits)
	if(jbits&0x02000000U)
//	if(0)
	{
		imm9u=(u32)((opw2&255)|((jbits&255)<<8));
		if(eo)imm9u|=0xFFFFFFFFFFFF0000LL;
		imm9n=imm9u;
		imm9us=imm9u;
	}

	op->rn=rn_dfl;
	op->rm=rm_dfl;
	op->imm=imm9us;

	op->rp=BJX2_REG_ZZR;
	op->rq=BJX2_REG_GBR_HI;

//	op->fl|=BJX2_OPFL_NOWEX;

	ret=0;
	switch((opw2>>12)&15)
	{
	case 0x0:	/* F1ed_0dzz */
		if(eq)
		{
			if(op->rm==BJX2_REG_DLR)
			{
				break;
			}
			if(op->rm==BJX2_REG_DHR)
				op->rm=BJX2_REG_GBR;
			op->nmid=BJX2_NMID_LEAB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAB_LdRegDispReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			break;
		}
		if(op->rn==BJX2_REG_DHR)
		{
			op->rn=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVB_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		}
		op->nmid=BJX2_NMID_MOVB;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVB_RegStRegDisp;
		op->fl|=BJX2_OPFL_NOWEX;
		if(op->rn==BJX2_REG_DLR)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVB_RegStPcDisp;
		}
		break;
	case 0x1:	/* F1ed_1dzz */
		if(eq)
		{
			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
			{
				if(op->imm&1)break;
				op->imm=(op->imm>>1);
				op->rm=BJX2_REG_GBR;
			}
			op->nmid=BJX2_NMID_LEAW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAW_LdRegDispReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			break;
		}
		if(op->rn==BJX2_REG_DHR)
		{
//			if(op->imm&1)break;
//			op->imm=(op->imm>>1);
			op->rn=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVW_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		}
		op->nmid=BJX2_NMID_MOVW;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVW_RegStRegDisp;
		op->fl|=BJX2_OPFL_NOWEX;
		if(op->rn==BJX2_REG_DLR)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVW_RegStPcDisp;
		}
		break;
	case 0x2:	/* F1ed_2dzz */
		if(eq)
		{
			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
			{
				if(op->imm&3)break;
				op->imm=(op->imm>>2);
				op->rm=BJX2_REG_GBR;
			}
			op->nmid=BJX2_NMID_LEAD;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAD_LdRegDispReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			break;
		}
		if(op->rn==BJX2_REG_DHR)
		{
//			if(op->imm&3)break;
//			op->imm=(op->imm>>2);
			op->rn=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		}
		op->nmid=BJX2_NMID_MOVL;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVL_RegStRegDisp;
		op->fl|=BJX2_OPFL_NOWEX;
		if(op->rn==BJX2_REG_DLR)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVL_RegStPcDisp;
		}
		break;
	case 0x3:	/* F1ed_3dzz */
		if(eq)
		{
			if(op->rm==BJX2_REG_DLR)
				break;
			if(op->rm==BJX2_REG_DHR)
			{
				if(op->imm&7)break;
				op->imm=(op->imm>>3);
				op->rm=BJX2_REG_GBR;
			}
			op->nmid=BJX2_NMID_LEAQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAQ_LdRegDispReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			break;
		}
		if(op->rn==BJX2_REG_DHR)
		{
//			if(op->imm&7)break;
//			op->imm=(op->imm>>3);
			op->rn=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		}
		op->nmid=BJX2_NMID_MOVQ;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVQ_RegStRegDisp;
		op->fl|=BJX2_OPFL_NOWEX;
		if(op->rn==BJX2_REG_DLR)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVQ_RegStPcDisp;
		}
		break;

	case 0x4:	/* F1nm_4gnm */
		if(	(op->rm==BJX2_REG_DLR) ||
			(op->rm==BJX2_REG_DHR) ||
			(op->rm==BJX2_REG_SP))
			break;

		if(op->rn==BJX2_REG_DHR)
		{
			op->rn=BJX2_REG_GBR;

			if(eq)
				break;

			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_FMOVS_GRegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;

			break;
		}
		if(op->rn==BJX2_REG_DLR)
			break;

//		if(eq)
//			break;

#if 1
		op->nmid=BJX2_NMID_FMOVS;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_FMOVS_GRegStRegDisp;
		op->fl|=BJX2_OPFL_NOWEX;
		if(eq)
		{
			op->nmid=BJX2_NMID_FMOVH;
			op->Run=BJX2_Op_FMOVH_GRegStRegDisp;
//			op->Run=BJX2_Op_BREAK_None;
		}
#endif

#if 0
		op->nmid=BJX2_NMID_FMOVS;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_FMOVS_RegStRegDisp;
		if(eq)
		{
			op->nmid=BJX2_NMID_FMOVUS;
			op->Run=BJX2_Op_FMOVS_GRegStRegDisp;
		}
#endif
		break;
	case 0x5:	/* F1nm_5gnm */
//		if(op->rn==BJX2_REG_DHR)
//			op->rn=BJX2_REG_GBR;			
		if(op->rn==BJX2_REG_DLR)
			break;
#if 0
		op->nmid=BJX2_NMID_FMOVD;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_FMOVD_RegStRegDisp;
#endif
		if(eq)
		{
			if(op->rm&1)
				op->rm=0x20|((op->rm)&0x1E);
		
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVX2_RegStRegDisp;

			if(op->rn==BJX2_REG_DHR)
			{
				op->rn=BJX2_REG_GBR;
				op->Run=BJX2_Op_MOVX2_RegStRegDisp1;
				break;
			}
			break;
		}else
		{
			op->rm=cm_dfl;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		}
		break;
	case 0x6:	/* F1nm_6gdd */
		if(	(op->rn==BJX2_REG_DLR) ||
			(op->rn==BJX2_REG_DHR) ||
			(op->rn==BJX2_REG_SP))
			break;

		if(op->rm==BJX2_REG_DLR)
			break;
		if(op->rm==BJX2_REG_DHR)
		{
			op->rm=BJX2_REG_GBR;

			if(eq)
				break;

			op->nmid=BJX2_NMID_FMOVS;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_FMOVS_LdRegDisp1GReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;

			break;
		}
		
//		if(eq)
//			break;

#if 1
		op->nmid=BJX2_NMID_FMOVS;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_FMOVS_LdRegDispGReg;
//		op->Run=BJX2_Op_BREAK_None;
//		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEX_IO2;
		if(eq)
		{
			op->nmid=BJX2_NMID_FMOVH;
			op->Run=BJX2_Op_FMOVH_LdRegDispGReg;
//			op->Run=BJX2_Op_BREAK_None;
		}
#endif

#if 0
		op->nmid=BJX2_NMID_FMOVS;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_FMOVS_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_FMOVUS;
			op->Run=BJX2_Op_FMOVS_LdRegDispGReg;
		}
#endif
		break;
	case 0x7:	/* F1nm_7gdd */
		if(op->rm==BJX2_REG_DLR)
			break;
//		if(op->rm==BJX2_REG_DHR)
//			op->rm=BJX2_REG_GBR;
#if 0
		op->nmid=BJX2_NMID_FMOVD;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_FMOVD_LdRegDispReg;
#endif
		if(eq)
		{
			if(op->rn&1)
				op->rn=0x20|((op->rn)&0x1E);

			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVX2_LdRegDispReg;
			if(op->rm==BJX2_REG_DHR)
			{
				op->rm=BJX2_REG_GBR;
				op->Run=BJX2_Op_MOVX2_LdRegDisp1Reg;
				break;
			}
			break;
		}else
		{
			op->rn=cn_dfl;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			break;
		}
		break;

#if 0
	case 0x4:	/* F1ed_4dzz */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
	case 0x5:	/* F1ed_5dzz */
		op->imm=imm9n;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
#endif

	case 0x8:	/* F1ed_8dzz */
//		if(op->rm==BJX2_REG_DHR)
//			op->rm=BJX2_REG_GBR;			
		if(op->rm==BJX2_REG_DHR)
		{
//			op->imm=(op->imm>>1);
			op->rm=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVB_LdRegDisp1Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDREGDISP1REG;
				op->Run=BJX2_Op_MOVUB_LdRegDisp1Reg;
			}
			break;
		}

		op->nmid=BJX2_NMID_MOVB;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVB_LdRegDispReg;
//		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEX_IO2;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUB;
			op->Run=BJX2_Op_MOVUB_LdRegDispReg;
		}
		if(op->rm==BJX2_REG_DLR)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVB_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdPcDispReg;
			}
		}
		break;
	case 0x9:	/* F1ed_9dzz */
		if(op->rm==BJX2_REG_DHR)
		{
//			if(op->imm&1)break;
//			op->imm=(op->imm>>1);
//			op->rm=BJX2_REG_GBR;

			op->rm=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVW_LdRegDisp1Reg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdRegDisp1Reg;
			}
			break;
		}
		op->nmid=BJX2_NMID_MOVW;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVW_LdRegDispReg;
//		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEX_IO2;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUW;
			op->Run=BJX2_Op_MOVUW_LdRegDispReg;
		}
		if(op->rm==BJX2_REG_DLR)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVW_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdPcDispReg;
			}
		}
		break;
	case 0xA:	/* F1ed_Adzz */
		if(op->rm==BJX2_REG_DHR)
		{
//			if(op->imm&3)break;
//			op->imm=(op->imm>>2);
			op->rm=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDisp1Reg;
			}
			break;
		}
		op->nmid=BJX2_NMID_MOVL;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVL_LdRegDispReg;
//		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEX_IO2;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUL;
			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
		}
		if(op->rm==BJX2_REG_DLR)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVL_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdPcDispReg;
			}
		}
		break;
	case 0xB:	/* F1ed_Bdzz */
		if(eq)
			break;

		if(op->rm==BJX2_REG_DHR)
		{
#if 0
			if(eq)
			{
				if(op->imm&3)break;
				op->imm=(op->imm>>2);
			}else
			{
				if(op->imm&7)break;
				op->imm=(op->imm>>3);
			}
#endif

			op->rm=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			break;
		}
		op->nmid=BJX2_NMID_MOVQ;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVQ_LdRegDispReg;
//		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEX_IO2;
		if(eq)
		{
//			op->nmid=BJX2_NMID_MOVDL;
//			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
		}
		if(op->rm==BJX2_REG_DLR)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVQ_LdPcDispReg;
			if(eq)
			{
//				op->nmid=BJX2_NMID_MOVDL;
//				op->Run=BJX2_Op_MOVUL_LdPcDispReg;
			}
		}
		break;

#if 0
	case 0xC:	/* F1ed_Cdzz */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_ADDSL;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADDSL_RegImmReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_ADDUL;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
		}
		break;
	case 0xD:	/* F1ed_Ddzz */
		op->imm=imm9n;
		op->nmid=BJX2_NMID_ADDSL;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADDSL_RegImmReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_ADDUL;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
		}
		break;
#endif

	case 0xC:	/* F1nm_Cgdd */
		op->rn=rm_dfl;
		op->rm=rn_dfl;
		op->imm=(signed char)(imm9u);

		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(eo)
		{
			op->nmid=BJX2_NMID_BRTSTF;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRTSTF_RegRegPcDisp;
		}else
		{
			op->nmid=BJX2_NMID_BRTSTT;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRTSTT_RegRegPcDisp;
		}
		break;

	case 0xD:	/* F1nm_Dgdd */
		op->rn=rm_dfl;
		op->rm=rn_dfl;
		op->imm=(signed char)(imm9u);

		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(eo)
		{
			op->nmid=BJX2_NMID_BRGE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRGE_RegRegPcDisp;
		}else
		{
			op->nmid=BJX2_NMID_BRLT;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRLT_RegRegPcDisp;
		}
		break;

	case 0xE:	/* F1nm_Egdd */
		op->rn=rm_dfl;
		op->rm=rn_dfl;
		op->imm=(signed char)(imm9u);

		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(eo)
		{
			op->nmid=BJX2_NMID_BRHE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRHE_RegRegPcDisp;
		}else
		{
			op->nmid=BJX2_NMID_BRBI;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRBI_RegRegPcDisp;
		}
		break;

	case 0xF:	/* F1nm_Fgdd */
		op->rn=rm_dfl;
		op->rm=rn_dfl;
		op->imm=(signed char)(imm9u);

		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(eo)
		{
			op->nmid=BJX2_NMID_BRNE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRNE_RegRegPcDisp;
		}else
		{
			op->nmid=BJX2_NMID_BREQ;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BREQ_RegRegPcDisp;
		}
		break;

	default:
		ret=-1;
		break;
	}
	
	return(ret);
}

int BJX2_DecodeOpcode_DecF5(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw2b;
	int ret;
	
	if(!(ctx->regs[BJX2_REG_SR]&(1<<27)))
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op1->pc=addr;

		opw2b=opw2&0xFEFF;
		ret=BJX2_DecodeOpcode_DecF1(ctx, op1, addr, opw1, opw2b, 0);

		if(opw2&0x0100)
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
	
	ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw1, opw2, 0);
	return(ret);
}

int BJX2_DecodeOpcode_DecD5(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	BJX2_Opcode *op1;
	int ret;
	
	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

	op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF1(ctx, op1, addr, opw1, opw2, jbits);

	if(!op1->Run || !op1->opn)
	{
		op->nmid=BJX2_NMID_INV;
		op->fmid=BJX2_FMID_Z;
//		op->Run=BJX2_Op_INVOP_None;
		op->nmid=BJX2_NMID_BREAK;
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
