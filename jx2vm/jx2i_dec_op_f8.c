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

int BJX2_DecodeOpcode_DecF8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	s64 imm16u, imm16n, imm16s;
	int rn_i16;
	int ret;
	
	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;

//	rn_i16=opw1&15;
//	if(opw1&0x0100)
//		rn_i16+=16;
	rn_i16=opw1&31;
	if(jbits&0x40000000U)rn_i16+=32;
	
	rn_i16=BJX2_RemapGPR(ctx, rn_i16);

	imm16u=(u16)opw2;
	imm16n=opw2|(~65535);
	imm16s=(s16)opw2;

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
		op->fl|=BJX2_OPFL_NOWEX;

		if(jbits&0x02000000U)
		{
			op->nmid=BJX2_NMID_FLDCF;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_FLDCF_ImmGReg;
		}

		break;

	default:
		ret=-1;
		break;
	}
	return(ret);
}

int BJX2_DecodeOpcode_DecD8(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int ret;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;
//	op->opn3=opw3;
	
	op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF8(ctx, op1, addr, opw1, opw2, 0);

	if(!op1->Run || !op1->opn)
	{
		op->nmid=BJX2_NMID_INV;
		op->fmid=BJX2_FMID_Z;
		op->Run=BJX2_Op_INVOP_None;
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
