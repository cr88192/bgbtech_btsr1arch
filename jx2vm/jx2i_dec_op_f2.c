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

int BJX2_DecodeOpcode_DecF2(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int disp5, eq, eo;
//	int imm8u, imm8n;
//	int imm10u, imm10n;
//	int imm9u, imm9n;
	s64 imm10u, imm10n;
	s64 imm9u, imm9n;

	int ret;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;

	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1   )&15;
//	ro_dfl=(opw1   )&15;
	if(opw2&0x0400)rn_dfl+=16;
	if(opw2&0x0200)rm_dfl+=16;
//	if(opw2&0x0010)ro_dfl+=16;

	if(jbits&0x40000000U)rn_dfl+=32;
	if(jbits&0x20000000U)rm_dfl+=32;

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

	imm10u=(opw2&1023);
//	imm10n=(opw2&1023)|((-1)<<10);
	imm10n=(opw2&1023)|(~1023);
	
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

		imm9u=(u32)((opw2&255)|(jbits<<8));
		if(eo)imm9u|=0xFFFFFFFF00000000LL;
		imm9n=imm9u;

		imm10u=imm9u;
		imm10n=imm9u;
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
		if(eo)imm9u|=0x0000000000010000LL;
		imm9n=imm9u|0xFFFFFFFFFFFE0000LL;

		imm10u=(u32)((opw2&255)|((jbits&255)<<8));
		imm10u=((opw2&0x0300)<<8);
		imm10n=imm10u|0xFFFFFFFFFFFC0000LL;
	}

	op->rn=rn_dfl;
	op->rm=rm_dfl;
	op->imm=imm9u;

	ret=0;
	switch((opw2>>12)&15)
	{
#if 1
	case 0x0:	/* F2ed_0dzz */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
	case 0x1:	/* F2ed_1dzz */
		op->imm=imm9n;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
#endif

#if 1
	case 0x2:	/* F1ed_2dzz */
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
	case 0x3:	/* F1ed_3dzz */
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
	case 0x4:	/* F1ed_4dzz */
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
		op->imm=imm9u;
		op->nmid=BJX2_NMID_AND;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_AND_RegImmReg;
		break;
	case 0x6:	/* F2nm_6ejj */
		op->imm=imm9u;
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
		break;
	case 0x7:	/* F2nm_7ejj */
		op->imm=imm9u;
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
			op->imm=(sbyte)imm9u;
			op->nmid=BJX2_NMID_PSHUFB;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_PSHUFB_RegImmReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_PSHUFW;
				op->Run=BJX2_Op_PSHUFW_RegImmReg;
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
			break;

		case 0x1:
			op->nmid=BJX2_NMID_LDIN;
			op->fmid=BJX2_FMID_IMMREG;
//			op->Run=BJX2_Op_MOV_ImmnReg;
			op->Run=BJX2_Op_MOV_ImmReg;
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
		default:
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

	op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF2(ctx, op1, addr, opw1, opw2, jbits);

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
