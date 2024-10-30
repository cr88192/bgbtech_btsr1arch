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

int BJX2_DecodeOpcode_DecF8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	s64 imm16u, imm16n, imm16s, imm20s;
	int rn_i16, rn_i16_org, wm, wi, isxg3;
	int ret;
	
	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;

	wm=0;	wi=0;

//	rn_i16=opw1&15;
//	if(opw1&0x0100)
//		rn_i16+=16;
	rn_i16=opw1&31;
	if(jbits&0x40000000U)	rn_i16+=32;	
	if(jbits&0x20000000U)	wm=1;
	if(jbits&0x10000000U)	wi=1;
	
	isxg3=0;
	if(	(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_RVE) &&
		(ctx->regs[BJX2_REG_SR]&BJX2_FLAG_SR_WXE) )
			isxg3=1;

	rn_i16_org=rn_i16;
	rn_i16=BJX2_RemapGPR(ctx, rn_i16);

	imm16u=(u16)opw2;
	imm16n=opw2|(~65535);
	imm16s=(s16)opw2;

	imm20s=imm16u|((opw1&15)<<16);
	if(opw1&8)
		imm20s|=(~0)<<20;
	imm20s^=((jbits>>28)&7)<<20;

	if(isxg3)
	{
		imm20s=imm16u|(rn_i16<<16);
		if(wi)
			imm20s|=(~0)<<22;
		imm20s=(imm20s<<1)-2;
	}

	if(jbits&0x02000000U)
	{
		imm16s=(u16)opw2;
		imm16s|=((s64)(jbits&0xFFFF))<<16;
		if(jbits&0x10000000U)
			imm16s|=0xFFFFFFFF00000000LL;
		
		imm16u=imm16s;
		imm16n=imm16s;
	}

	ret=0;
	switch((opw1>>4)&15)
	{
	case 0x0:	case 0x1:
		op->imm=imm16u;
		op->rn=rn_i16;
//		op->nmid=BJX2_NMID_MOV;
		op->nmid=BJX2_NMID_LDI;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;
	case 0x2:	case 0x3:
		op->imm=imm16n;
		op->rn=rn_i16;
//		op->nmid=BJX2_NMID_MOV;
		op->nmid=BJX2_NMID_LDI;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;
	case 0x4:	case 0x5:
		op->imm=imm16s;
		op->rn=rn_i16;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_ADD_ImmReg;

		if(jbits&0x02000000U)
		{
			switch((jbits>>20)&3)
			{
			case 0: break;
			case 3:
				op->nmid=BJX2_NMID_XMOVTT;
				op->Run=BJX2_Op_XMOVTT_ImmReg;
				break;
			}
		}
		break;

	case 0x6:	case 0x7:
		op->imm=imm16u;
		op->rn=rn_i16;
		op->nmid=BJX2_NMID_LDISH16;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_LDISH16_ImmReg;
		break;

	case 0x8:	case 0x9:
		op->imm=imm16u;
		op->rn=rn_i16;
		op->nmid=BJX2_NMID_FLDCH;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_FLDCH_ImmGReg;
//		op->fl|=BJX2_OPFL_NOWEX;

//		if((opw1&31)==15)
		if(((opw1&31)==15) && !(jbits&0x40000000U))
		{
			op->nmid=BJX2_NMID_BREAK;
			op->Run=BJX2_Op_BREAK_None;
		}

		if(jbits&0x02000000U)
		{
			op->fl|=BJX2_OPFL_NOWEX;

			switch((jbits>>20)&3)
			{
			case 0:
				op->nmid=BJX2_NMID_FLDCF;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_FLDCF_ImmGReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_PLDCH;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_PLDCH_ImmReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_PLDCM8SH;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_PLDCM8SH_ImmReg;

//				op->nmid=BJX2_NMID_BREAK;
//				op->Run=BJX2_Op_BREAK_None;
				break;
			case 3:
				op->nmid=BJX2_NMID_PLDCM8UH;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_PLDCM8UH_ImmReg;
				break;
			default:
				break;
			}
		}else
			if(wm)
		{
			op->imm=imm16u;
			op->rn=BJX2_REG_GBR;
			op->rm=rn_i16;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			
			if(wi)
			{
				op->nmid=BJX2_NMID_MOVL;
				op->Run=BJX2_Op_MOVQ_RegStRegDisp;
			}
		}

		break;

	case 0xA:	case 0xB:
		op->imm=imm16u;
		op->rm=BJX2_REG_GBR;
		op->rn=rn_i16;
		op->nmid=BJX2_NMID_LEAQ;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_LEAQ_LdRegDispReg;
		op->fl|=BJX2_OPFL_NOWEX_IO2;
		
		if(wm)
		{
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_MOVQ_LdRegDispReg;
			
			if(wi)
			{
				op->nmid=BJX2_NMID_MOVL;
				op->Run=BJX2_Op_MOVL_LdRegDispReg;
			}
		}
		break;

	case 0xE:	case 0xF:
		op->imm=imm20s;
		op->nmid=BJX2_NMID_BRA;
		op->fmid=BJX2_FMID_PCDISP;
		op->Run=BJX2_Op_BRA_PcDisp;
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;
		
		if(isxg3 && wm)
		{
			op->nmid=BJX2_NMID_BSR;
			op->Run=BJX2_Op_BSR_PcDisp;
		}
		if(!isxg3 && (opw1&0x0010))
		{
			op->nmid=BJX2_NMID_BSR;
			op->Run=BJX2_Op_BSR_PcDisp;
		}
		break;

	default:
		ret=-1;
		break;
	}
	return(ret);
}

int BJX2_DecodeOpcode_DecD8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	BJX2_Opcode *op1;
	int ret;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
//	op->opn3=opw3;
	
	op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF8(ctx, op1, addr, opw1, opw2, jbits);

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

//	if(opw1&0x0200)
//	if(opw1&0x0100)
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
