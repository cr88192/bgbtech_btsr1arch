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

int BJX2_DecodeOpcode_DecF2(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int rn_rvo, rm_rvo, ro_rvo;
	int disp5, eq, eo, disp8s, disp11s, isxg3;
//	int imm8u, imm8n;
//	int imm10u, imm10n;
//	int imm9u, imm9n;
	s64 imm10u, imm10n;
	s64 imm9u, imm9n, imm9us;

	int ret;

	if(!(op->fl&BJX2_OPFL_RV64))
	{
		op->fl|=BJX2_OPFL_TWOWORD;
		op->opn=opw1;
		op->opn2=opw2;
	}

	op->pc=addr;

	isxg3=0;
	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
		(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_WXE) )
			isxg3=1;

	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1   )&15;
//	ro_dfl=(opw1   )&15;
	if(opw2&0x0400)rn_dfl+=16;
	if(opw2&0x0200)rm_dfl+=16;
//	if(opw2&0x0010)ro_dfl+=16;

	if(jbits&0x40000000U)rn_dfl+=32;
	if(jbits&0x20000000U)rm_dfl+=32;

	rn_rvo=rn_dfl;
	rm_rvo=rm_dfl;

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
//	imm9n=(opw2&511)|(~((1<<9)-1));
	imm9n=(opw2&511)|(~511);

	imm9us=imm9u;

	if(jbits&0x10000000U)
		{ imm9us=imm9n; }

	imm10u=(opw2&1023);
//	imm10n=(opw2&1023)|((-1)<<10);
	imm10n=(opw2&1023)|(~1023);

	if(jbits&0x10000000U)
	{
		imm9u|= 512;
		imm9n&=~512;
		imm10u|= 1024;
		imm10n&=~1024;
	}

	if(jbits&0x20000000U)
	{
		imm10u|= 2048;
		imm10n&=~2048;
	}
	
	if(isxg3)
	{
		imm9n&=~512;
		imm10n&=~1024;
		imm10n&=~2048;

		if(jbits&0x10000000U)
		{
			imm9n|= 512;
			imm10n|= 1024;
		}
		if(jbits&0x20000000U)
		{
			imm10n|= 2048;
		}
	}
	
//	disp11s=(opw1&1)?imm10n:imm10u;
//	if(jbits&0x20000000U)
//		{ disp11s^=2048; }
	
	disp8s=(signed char)opw2;
	
//	if(jbits)
	if(jbits&0x01000000U)
	{
//		imm9u=(opw2&511)|(jbits<<9);
//		imm9n=(opw2&511)|(jbits<<9)|((~0U)<<31);
//		imm10u=(opw2&1023)|(jbits<<10);
//		imm10n=imm10u;

//		imm9u=(opw2&255)|(jbits<<8);
//		imm9n=(opw2&255)|(jbits<<8);
//		imm10u=(opw2&255)|(jbits<<8);
//		imm10n=imm10u;

		imm9u=(u32)((opw2&255)|((jbits&0x00FFFFFF)<<8));
		if(eo)imm9u|=0xFFFFFFFF00000000LL;
		imm9n=imm9u;
		imm9us=imm9u;

		imm10u=imm9u;
		imm10n=imm9u;

		disp8s=(int)imm9u;
	}

	if(jbits&0x02000000U)
	{
//		imm9u=(opw2&511)|(jbits<<9);
//		imm9n=(opw2&511)|(jbits<<9)|((~0U)<<31);
//		imm10u=(opw2&1023)|(jbits<<10);
//		imm10n=imm10u;

//		imm9u=(opw2&255)|(jbits<<8);
//		imm9n=(opw2&255)|(jbits<<8);
//		imm10u=(opw2&255)|(jbits<<8);
//		imm10n=imm10u;

		imm9u=(u32)((opw2&255)|((jbits&255)<<8));
		if(eo)imm9u|=	0x0000000000010000LL;
		imm9n=imm9u|	0xFFFFFFFFFFFE0000LL;
		
		if(jbits&0x10000000U)
			imm9u=imm9n;

		imm9us=imm9u;

		imm10u=(u32)((opw2&255)|((jbits&255)<<8));
		imm10u|=((opw2&0x0300)<<8);
		imm10n=imm10u|0xFFFFFFFFFFFC0000LL;

#if 1
		if(isxg3)
//		if(1)
		{
			imm10u=imm9u;
			imm10n=imm9n;
		}
#endif
	}

	if(isxg3)
	{
		/* Tweak: Make XG3 interpret 2RI using same immed as 3RI */
		imm10u=imm9u;
		imm10n=imm9n;
	}

	disp11s=(opw1&1)?imm10n:imm10u;

	op->rn=rn_dfl;
	op->rm=rm_dfl;
	op->imm=imm9u;

	op->rp=BJX2_REG_ZZR;
	op->rq=BJX2_REG_GBR_HI;

	ret=0;
	switch((opw2>>12)&15)
	{
#if 1
	case 0x0:	/* F2nm_0ejj */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
	case 0x1:	/* F2nm_1ejj */
#if 1
		if(jbits&0x01000000U)
		{
			if(eq)
			{
#if 1
				op->imm=imm9u<<32;
				op->nmid=BJX2_NMID_FADD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_FADD_RegImm56Reg;
				if(eo)
					{ op->Run=BJX2_Op_FADDA_RegImm56Reg; }
#endif

				break;
			}
//			break;
		}
#endif
		op->imm=imm9n;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
#endif

#if 1
	case 0x2:	/* F2nm_2ejj */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_MULS;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_MULS_RegImmReg;
		op->fl|=BJX2_OPFL_NOWEX;
		if(eq)
		{
			op->nmid=BJX2_NMID_MULU;
			op->Run=BJX2_Op_MULU_RegImmReg;
		}
		break;
#endif

#if 1
	case 0x3:	/* F2nm_3ejj */
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
	case 0x4:	/* F2nm_4ejj */
#if 1
		if(jbits&0x01000000U)
		{
			if(eq)
			{
				op->imm=imm9u<<32;
				op->nmid=BJX2_NMID_FMUL;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_FMUL_RegImm56Reg;
				if(eo)
					{ op->Run=BJX2_Op_FMULA_RegImm56Reg; }
				break;
			}
			break;
		}
#endif

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

	case 0x5:	/* F2nm_5ejj */
//		op->imm=imm9u;
		op->imm=imm9us;
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
		op->imm=imm9u;
//		op->imm=imm9us;
		op->nmid=BJX2_NMID_OR;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_OR_RegImmReg;
#if 1
		if(eq)
		{
			op->imm=(sbyte)imm9u;
			op->nmid=BJX2_NMID_SHADX;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHADX_RegImmReg;
		}
#endif

		if(eq && (rn_rvo<2))
		{
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_LDREGDISP1;
			op->Run=BJX2_Op_BRA_RegDisp1;
			op->rn=rm_dfl;
			op->rm=rm_dfl;
			op->imm=imm9us*4;

			if(rn_rvo==1)
			{
				op->rn=BJX2_REG_LR;
				op->nmid=BJX2_NMID_BSR;
				op->fmid=BJX2_FMID_LDREGDISP1REG;
				op->Run=BJX2_Op_BSR_RegRegDisp1;
			}
			
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;
		}

		break;
	case 0x7:	/* F2nm_7ejj */
		op->imm=imm9u;
//		op->imm=imm9us;
		op->nmid=BJX2_NMID_XOR;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_XOR_RegImmReg;
#if 1
		if(eq)
		{
			op->imm=(sbyte)imm9u;
			op->nmid=BJX2_NMID_SHLDX;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDX_RegImmReg;
		}
#endif
		break;

	case 0x8:	/* F2nm_8ejj */
		if(eo)
		{
//			op->imm=(sbyte)imm9u;
			op->imm=(byte)imm9u;
			op->nmid=BJX2_NMID_PSHUFB;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_PSHUFB_RegImmReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSHUFW;
				op->Run=BJX2_Op_PSHUFW_RegImmReg;
				if((imm9u>>8)==3)
				{
					op->nmid=BJX2_NMID_PMULTW;
					op->Run=BJX2_Op_PMULTW_RegImmReg;
				}
				if(imm9u>>16)
				{
					op->imm=imm9u;
				}
			}
		}else
		{
			op->imm=(sbyte)imm9u;
			op->nmid=BJX2_NMID_SHAD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHAD_RegImmReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_SHADQ;
				op->Run=BJX2_Op_SHADQ_RegImmReg;
			}
		}
		break;
	case 0x9:	/* F2nm_9ejj */
		op->imm=(sbyte)imm9u;
		op->nmid=BJX2_NMID_SHLD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_SHLD_RegImmReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_SHLDQ;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
		}
		break;

	case 0xC:	/* F2nz_Cejj */
		op->rn=rn_dfl;
		op->rm=rn_dfl;

		op->imm=imm10u;
		if(opw1&1)
			op->imm=imm10n;

		switch(opw1&0xF)
		{
		case 0x0:
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMMREG;
//			op->Run=BJX2_Op_MOV_ImmuReg;
			op->Run=BJX2_Op_MOV_ImmReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_LDI;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;
				op->imm=imm10u<<16;
			}
			break;

		case 0x1:
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMMREG;
//			op->Run=BJX2_Op_MOV_ImmnReg;
			op->Run=BJX2_Op_MOV_ImmReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_LDI;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;
				op->imm=imm10n<<16;
			}
			break;

		case 0x2:
			if(eq)
				break;
			op->nmid=BJX2_NMID_LDISH8;
			op->fmid=BJX2_FMID_IMMREG;
//			op->Run=BJX2_Op_MOV_ImmnReg;
			op->Run=BJX2_Op_LDISH_Imm;
			if(eq)
			{
				op->nmid=BJX2_NMID_LDI;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;
				op->imm=imm10u<<16;
			}
			break;

		case 0x3:
			op->nmid=BJX2_NMID_LDI;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			op->imm=imm10u<<22;
			if(eq)
			{
				op->nmid=BJX2_NMID_LDIQ;
				op->Run=BJX2_Op_MOV_ImmHiReg;
				
				if(jbits)
				{
					op->imm=imm10u;
				}
			}else
				if(jbits)
			{
//				op->nmid=BJX2_NMID_LDIL;
//				op->Run=BJX2_Op_MOV_ImmMidReg;
//				op->imm=imm10u;

				op->imm=imm10u<<16;
			}

			break;

		case 0x4:
		case 0x5:
			op->nmid=BJX2_NMID_TST;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_TST_ImmReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_TSTQ;
				op->Run=BJX2_Op_TSTQ_ImmReg;
			}
			break;

		case 0x6:
		case 0x7:
			op->nmid=BJX2_NMID_CMPHS;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPHS_ImmReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQHS;
				op->Run=BJX2_Op_CMPQHS_ImmReg;
			}
			break;
		case 0x8:
		case 0x9:
			op->nmid=BJX2_NMID_CMPHI;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPHI_ImmReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQHI;
				op->Run=BJX2_Op_CMPQHI_ImmReg;
			}
			break;
		case 0xA:
		case 0xB:
			op->nmid=BJX2_NMID_CMPGE;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGE_ImmReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQGE;
				op->Run=BJX2_Op_CMPQGE_ImmReg;
			}
			break;
		case 0xC:
		case 0xD:
			op->nmid=BJX2_NMID_CMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPEQ_ImmReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQEQ;
				op->Run=BJX2_Op_CMPQEQ_ImmReg;
			}
			break;
		case 0xE:
		case 0xF:
			op->nmid=BJX2_NMID_CMPGT;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_CMPGT_ImmReg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				op->nmid=BJX2_NMID_CMPQGT;
				op->Run=BJX2_Op_CMPQGT_ImmReg;
			}
			break;
			
		default:
			break;
		}
		break;

	case 0xD:	/* F2nz_Dejj */
		op->rn=rn_dfl;
		op->rm=rn_dfl;

		op->imm=imm10u;
		if(opw1&1)
			op->imm=imm10n;

		switch(opw1&0xF)
		{
		case 0x0:	case 0x1:
//			op->imm=imm16s;
//			op->rn=rn_i16;
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_ADD_ImmReg;
			break;

		case 0x2:	case 0x3:
//			op->imm=imm16s;
//			op->rn=rn_i16;
			op->nmid=BJX2_NMID_MULSW;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MULSW_ImmReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MULUW;
				op->Run=BJX2_Op_MULUW_ImmReg;
			}
			break;

		case 0x4:
			op->nmid=BJX2_NMID_LDIZ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmuReg;
			op->imm=(imm10u&255)<<((imm10u>>8)*8);
			if(eq)
			{
				op->nmid=BJX2_NMID_LDIQ;
				op->Run=BJX2_Op_MOV_ImmHiReg;
			}
			break;
		case 0x5:
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmnReg;
			op->imm=(imm10u|(~255))<<((imm10u>>8)*8);
//			op->imm=(imm10u|(-256))<<((imm10u>>8)*8);
			if(eq)
			{
				op->nmid=BJX2_NMID_LDIQ;
				op->Run=BJX2_Op_MOV_ImmHiReg;
			}
			break;

		case 0x6:	/* F2n6_Dgdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp8s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				switch((opw2>>8)&3)
				{
				case 0:
					op->nmid=BJX2_NMID_BREQ;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BREQ_RegRegPcDisp;
					break;
				case 1:
					op->nmid=BJX2_NMID_BRNE;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRNE_RegRegPcDisp;
					break;
				case 2:
					op->nmid=BJX2_NMID_BRLE;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRLE_RegRegPcDisp;
					break;
				case 3:
					op->nmid=BJX2_NMID_BRGT;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRGT_RegRegPcDisp;
					break;
				}
			}else
			{
				switch((opw2>>8)&3)
				{
				case 0:
					op->nmid=BJX2_NMID_BREQL;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BREQL_RegRegPcDisp;
					break;
				case 1:
					op->nmid=BJX2_NMID_BRNEL;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRNEL_RegRegPcDisp;
					break;
				case 2:
					op->nmid=BJX2_NMID_BRLEL;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRLEL_RegRegPcDisp;
					break;
				case 3:
					op->nmid=BJX2_NMID_BRGTL;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRGTL_RegRegPcDisp;
					break;
				}
			}
			break;

		case 0x7:	/* F2n7_Dgdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp8s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				switch((opw2>>8)&3)
				{
				case 0:
					op->nmid=BJX2_NMID_BRLT;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRLT_RegRegPcDisp;
					break;
				case 1:
					op->nmid=BJX2_NMID_BRGE;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRGE_RegRegPcDisp;
					break;
				}
			}else
			{
				switch((opw2>>8)&3)
				{
				case 0:
					op->nmid=BJX2_NMID_BRLTL;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRLTL_RegRegPcDisp;
					break;
				case 1:
					op->nmid=BJX2_NMID_BRGEL;
					op->fmid=BJX2_FMID_REGPCDISP;
					op->Run=BJX2_Op_BRGEL_RegRegPcDisp;
					break;
				}
			}
			break;

		case 0x8:
			op->imm=imm10u*4;
			op->rn=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 0x9:
			op->imm=imm10u*8;
			op->rn=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			if(eq)
			{
				if(op->rm&1)
					op->rm=0x20|((op->rm)&0x1E);

				op->nmid=BJX2_NMID_MOVX2;
				op->Run=BJX2_Op_MOVX2_RegStRegDisp1;
				op->fl|=BJX2_OPFL_NOWEXSFX;
			}
			break;

		case 0xA:
			op->imm=imm10u*4;
			op->rm=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
//			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEX_IO2;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdRegDisp1Reg;
			}
			break;
		case 0xB:
			op->imm=imm10u*8;
			op->rm=BJX2_REG_GBR;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			if(eq)
			{
				if(op->rn&1)
					op->rn=0x20|((op->rn)&0x1E);

				op->nmid=BJX2_NMID_MOVX2;
				op->Run=BJX2_Op_MOVX2_LdRegDisp1Reg;
				op->fl|=BJX2_OPFL_NOWEXSFX;
			}
			break;

		case 0xC:
			if(jbits&0x01000000U)
			{
				if(eq)
					break;

				op->imm=imm10u<<32;
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_IMMREG;
				op->fl|=BJX2_OPFL_NOWEX;
				op->Run=BJX2_Op_FCMPEQ_Imm56Reg;
				break;
			}

//			if(eq)
//				break;

			op->imm=imm10u;
			op->nmid=BJX2_NMID_FCMPEQ;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_FCMPEQ_ImmReg;
			op->imm=imm10u<<6;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				op->nmid=BJX2_NMID_FLDCH;
				op->Run=BJX2_Op_FLDCH_ImmGReg;
			}
			break;

		case 0xE:
			if(jbits&0x01000000U)
			{
				if(eq)
					break;

				op->imm=imm10u<<32;
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_IMMREG;
				op->fl|=BJX2_OPFL_NOWEX;
				op->Run=BJX2_Op_FCMPGT_Imm56Reg;
				break;
			}

//			if(eq)
//				break;

			op->imm=imm10u;
			op->nmid=BJX2_NMID_FCMPGT;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_FCMPGT_ImmReg;
			op->imm=imm10u<<6;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				op->rm=op->rn;
				op->nmid=BJX2_NMID_FADD;
				op->Run=BJX2_Op_FADD_RegImmReg;
			}
			break;
		case 0xF:
			if(jbits&0x01000000U)
			{
				if(eq)
					break;

				op->imm=imm10u<<32;
				op->nmid=BJX2_NMID_FCMPGE;
				op->fmid=BJX2_FMID_IMMREG;
				op->fl|=BJX2_OPFL_NOWEX;
				op->Run=BJX2_Op_FCMPGE_Imm56Reg;
				break;
			}

//			if(eq)
//				break;

			op->imm=imm10u;
			op->nmid=BJX2_NMID_FCMPGE;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_FCMPGE_ImmReg;
			op->imm=imm10u<<6;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				op->rm=op->rn;
				op->nmid=BJX2_NMID_FMUL;
				op->Run=BJX2_Op_FMUL_RegImmReg;
			}
			break;


		default:
			break;
		}
		break;

	case 0xE:	/* F2nz_Eejj */
		op->rn=rn_dfl;
		op->rm=rn_dfl;

		op->imm=imm10u;
		if(opw1&1)
			op->imm=imm10n;

		switch(opw1&0xF)
		{
		case 0x0:	case 0x1:
//			op->imm=imm16s;
//			op->rn=rn_i16;
			op->nmid=BJX2_NMID_LEATB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEATB_LdRegDispReg;
			op->fl|=BJX2_OPFL_NOWEX;

			if(eq)
			{
				if(op->rn&1)
					{ op->rn=(op->rn&0x1E)+0x20;	}
				op->rq=op->rn+1;
				op->fl|=BJX2_OPFL_REGX2R;
				op->fl|=BJX2_OPFL_NOWEXSFX;

				op->nmid=BJX2_NMID_XLEAB;
				op->fmid=BJX2_FMID_LDREGDISPREG;
				op->Run=BJX2_Op_XLEAB_LdRegDispReg;
			}
			break;

		case 0x4:	/* F2n4_Egdd */
		case 0x5:	/* F2n5_Egdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp11s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				op->nmid=BJX2_NMID_BREQ;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BREQ_RegRegPcDisp;
			}else
			{
				op->nmid=BJX2_NMID_BREQL;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BREQL_RegRegPcDisp;
			}
			break;

		case 0x6:	/* F2n6_Egdd */
		case 0x7:	/* F2n7_Egdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp11s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				op->nmid=BJX2_NMID_BRNE;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRNE_RegRegPcDisp;
			}else
			{
				op->nmid=BJX2_NMID_BRNEL;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRNEL_RegRegPcDisp;
			}
			break;

		case 0x8:	/* F2n8_Egdd */
		case 0x9:	/* F2n9_Egdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp11s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				op->nmid=BJX2_NMID_BRLT;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRLT_RegRegPcDisp;
			}else
			{
				op->nmid=BJX2_NMID_BRLTL;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRLTL_RegRegPcDisp;
			}
			break;

		case 0xA:	/* F2nA_Egdd */
		case 0xB:	/* F2nB_Egdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp11s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				op->nmid=BJX2_NMID_BRGE;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRGE_RegRegPcDisp;
			}else
			{
				op->nmid=BJX2_NMID_BRGEL;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRGEL_RegRegPcDisp;
			}
			break;

		case 0xC:	/* F2nC_Egdd */
		case 0xD:	/* F2nD_Egdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp11s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				op->nmid=BJX2_NMID_BRLE;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRLE_RegRegPcDisp;
			}else
			{
				op->nmid=BJX2_NMID_BRLEL;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRLEL_RegRegPcDisp;
			}
			break;

		case 0xE:	/* F2nE_Egdd */
		case 0xF:	/* F2nF_Egdd */
			op->rn=rn_dfl;
			op->rm=BJX2_REG_ZZR;
			op->imm=disp11s;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			if(eq)
			{
				op->nmid=BJX2_NMID_BRGT;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRGT_RegRegPcDisp;
			}else
			{
				op->nmid=BJX2_NMID_BRGTL;
				op->fmid=BJX2_FMID_REGPCDISP;
				op->Run=BJX2_Op_BRGTL_RegRegPcDisp;
			}
			break;
		}
		break;


	default:
		ret=-1;
		break;
	}
	
	return(ret);
}

int BJX2_DecodeOpcode_DecF6(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw2b;
	int ret, pr;
	
	if(!(ctx->regs[BJX2_REG_SR]&(1<<27)))
	{
		op1=BJX2_ContextAllocOpcode(ctx);

		if(opw2&0x8000)
		{
			opw2b=(opw2&0xFCFF)|
				((opw2>>1)&0x0100)|
				((opw1<<9)&0x0200);
//			opw2b=opw2&0xFDFF;
			pr=opw2&0x0100;
		}
		else
		{
			opw2b=opw2&0xFEFF;
			pr=opw2&0x0100;
		}
		ret=BJX2_DecodeOpcode_DecF2(ctx, op1, addr, opw1, opw2b, 0);

//		if(opw2&0x0100)
		if(pr)
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
	
	ret=BJX2_DecodeOpcode_DecF2(ctx, op, addr, opw1, opw2, 0);
	return(ret);
}

int BJX2_DecodeOpcode_DecD6(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	BJX2_Opcode *op1;
	int ret;
	
	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

	op1=op->data;
	if(!op1)
		op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF2(ctx, op1, addr, opw1, opw2, jbits);

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
