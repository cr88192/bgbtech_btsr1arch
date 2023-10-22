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

void BJX2_Op_MOVB_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	BJX2_MemSetByte(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	BJX2_MemSetByteW(ctx,
		ctx->regs[op->rn], ctx->regs[op->rq],
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByteW(ctx,
		ctx->regs[op->rm], ctx->regs[op->rq]);
}

void BJX2_Op_MOVW_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	BJX2_MemSetWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	BJX2_MemSetWordW(ctx,
		ctx->regs[op->rn], ctx->regs[op->rq],
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
	ctx->regs[op->rn]=(s16)BJX2_MemGetWordW(ctx,
		ctx->regs[op->rm], ctx->regs[op->rq]);
}

void BJX2_Op_MOVL_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	BJX2_MemSetDWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
	BJX2_MemSetDWordW(ctx,
		ctx->regs[op->rn], ctx->regs[op->rq],
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWordW(ctx,
		ctx->regs[op->rm], ctx->regs[op->rq]);
}

void BJX2_Op_MOVQ_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
}

void BJX2_Op_MOVQ_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx, ctx->regs[op->rm]);
}

void BJX2_Op_MOVUB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
}

void BJX2_Op_MOVUW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
}

void BJX2_Op_MOVUL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
}


void BJX2_Op_SETTRIP_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetTrip(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
}


void BJX2_Op_MOVB_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[BJX2_REG_DR]),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVW_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2));
}

void BJX2_Op_MOVL_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*4),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*4));
}

void BJX2_Op_MOVQ_RegStDrReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*8),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVQ_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*8));
}

void BJX2_Op_MOVUB_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVUW_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2));
}

void BJX2_Op_MOVUL_LdDrRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[BJX2_REG_DR])*4));
}

void BJX2_Op_MOVB_RegStDrPc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVB_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVW_RegStDrPc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVW_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVL_RegStDrPc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVL_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVQ_RegStDrPc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVQ_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVUB_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVUW_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}

void BJX2_Op_MOVUL_LdDrPcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[BJX2_REG_DR]));
}


#if 0
void BJX2_Op_MOVB_RegStDr4Pc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVB_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm));
}

void BJX2_Op_MOVW_RegStDr4Pc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVW_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm));
}

void BJX2_Op_MOVL_RegStDr4Pc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVL_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm));
}

void BJX2_Op_MOVQ_RegStDr4Pc(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVQ_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetQWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm));
}

void BJX2_Op_MOVUB_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm));
}

void BJX2_Op_MOVUW_LdDr4PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(op->pc2)+(ctx->regs[BJX2_REG_DR]*16+op->imm));
}
#endif

void BJX2_Op_MOVB_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm), ctx->regs[op->rq],
		ctx->regs[op->rm]);
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm), 1);
}

void BJX2_Op_MOVB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), 1);
	ctx->regs[op->rn]=(s32)BJX2_MemGetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVW_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2), ctx->regs[op->rq],
		ctx->regs[op->rm]);
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2), 2);
}

void BJX2_Op_MOVW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 2);
	ctx->regs[op->rn]=(s32)BJX2_MemGetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), ctx->regs[op->rq]);
}

void BJX2_Op_MOVL_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*4), ctx->regs[op->rq],
		ctx->regs[op->rm]);
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*4), 4);
}

void BJX2_Op_MOVL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), 4);
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), ctx->regs[op->rq]);
}

void BJX2_Op_MOVQ_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*8), ctx->regs[op->rq],
		ctx->regs[op->rm]);
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*8), 8);
}

void BJX2_Op_MOVQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;

	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8);

#if 1
	if(addr==0xF000E038)
	{
		lv=-1;
	}
#endif

	ctx->trapc=op->pc;

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8), 8);

	lv=BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);
	ctx->regs[op->rn]=lv;
}



void BJX2_Op_MOVTW_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;

	BJX2_MemSetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2)+0, ctx->regs[op->rq],
		ctx->regs[op->rm]);
	BJX2_MemSetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2)+4, ctx->regs[op->rq],
		ctx->regs[op->rm]>>32);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2), 8);
}

void BJX2_Op_MOVHTW_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;

	BJX2_MemSetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2)+2, ctx->regs[op->rq],
		ctx->regs[op->rm]>>32);
	BJX2_MemSetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2)+0, ctx->regs[op->rq],
		ctx->regs[op->rm]>>16);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2), 8);
}

void BJX2_Op_MOVTW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;

	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2);

	ctx->trapc=op->pc;

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 8);

	lv=BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);
	lv=((s64)(lv<<16))>>16;
	ctx->regs[op->rn]=lv;
}

void BJX2_Op_MOVUTW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;

	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2);

	ctx->trapc=op->pc;

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 8);

	lv=BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);
	lv=lv&0x0000FFFFFFFFFFFFLL;
	ctx->regs[op->rn]=lv;
}

void BJX2_Op_MOVHTW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;

	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2);

	ctx->trapc=op->pc;

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 8);

	lv=BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);
	lv=lv<<16;
	ctx->regs[op->rn]=lv;
}

#if 1
void BJX2_Op_MOVB_RegStRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm), ctx->regs[op->rq],
		ctx->regs[op->rm]);
//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+(op->imm));
}

void BJX2_Op_MOVB_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rm]),
//		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), 1);
	ctx->regs[op->rn]=(s32)BJX2_MemGetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVUB_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rm]),
//		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), 1);
	ctx->regs[op->rn]=(byte)BJX2_MemGetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}


void BJX2_Op_MOVW_RegStRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm), ctx->regs[op->rq],
		ctx->regs[op->rm]);
//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+(op->imm));
}

void BJX2_Op_MOVW_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVUW_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVL_RegStRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm), ctx->regs[op->rq],
		ctx->regs[op->rm]);
//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+(op->imm));
}

void BJX2_Op_MOVL_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVUL_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVQ_RegStRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm), ctx->regs[op->rq],
		ctx->regs[op->rm]);
//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+(op->imm));
}

void BJX2_Op_MOVQ_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}
#endif

void BJX2_Op_MOVUB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), 1);
	ctx->regs[op->rn]=(byte)BJX2_MemGetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);
}

void BJX2_Op_MOVUW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 2);
	ctx->regs[op->rn]=(u16)BJX2_MemGetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), ctx->regs[op->rq]);
}

void BJX2_Op_MOVUL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), 4);
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), ctx->regs[op->rq]);
}

void BJX2_Op_MOVX2_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(ctx->regs[op->rn])+(op->imm*8);
	ctx->trapc=op->pc;
	if(addr&7)
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
		return;
	}

#if 1
	if((addr&(~4095))!=((addr+16)&(~4095)))
	{
		BJX2_MemTranslateTlb(ctx, addr+0, 2);
		BJX2_MemTranslateTlb(ctx, addr+15, 2);
		if(ctx->status)
			return;
	}
#endif

//	BJX2_MemSetQWordW(ctx, addr+0, ctx->regs[op->rq], ctx->regs[op->rm+0]);
//	BJX2_MemSetQWordW(ctx, addr+8, ctx->regs[op->rq], ctx->regs[op->rm+1]);
	BJX2_MemSetXWordW(ctx, addr, ctx->regs[op->rq],
		ctx->regs[op->rm+0], ctx->regs[op->rm+1]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*8), 16);
}

void BJX2_Op_MOVX2_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	u64 vlo, vhi;
	
	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);

	if((addr&(~4095))!=((addr+16)&(~4095)))
	{
		BJX2_MemTranslateTlb(ctx, addr+0, 1);
		BJX2_MemTranslateTlb(ctx, addr+16, 1);
	}
//	ctx->regs[op->rn+0]=BJX2_MemGetQWordW(ctx, addr+0, ctx->regs[op->rq]);
//	ctx->regs[op->rn+1]=BJX2_MemGetQWordW(ctx, addr+8, ctx->regs[op->rq]);
	
	BJX2_MemGetXWordW(ctx, addr, ctx->regs[op->rq], &vlo, &vhi);
	ctx->regs[op->rn+0]=vlo;
	ctx->regs[op->rn+1]=vhi;
}

void BJX2_Op_MOVX2_RegStRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(ctx->regs[op->rn])+(op->imm*1);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);

//	if((addr&(~4095))!=((addr+16)&(~4095)))
//	{
//		BJX2_MemTranslateTlb(ctx, addr+0, 2);
//		BJX2_MemTranslateTlb(ctx, addr+16, 2);
//	}

//	BJX2_MemSetQWordW(ctx, addr+0, ctx->regs[op->rq], ctx->regs[op->rm+0]);
//	BJX2_MemSetQWordW(ctx, addr+8, ctx->regs[op->rq], ctx->regs[op->rm+1]);
	BJX2_MemSetXWordW(ctx, addr, ctx->regs[op->rq],
		ctx->regs[op->rm+0], ctx->regs[op->rm+1]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*1), 16);
}

void BJX2_Op_MOVX2_LdRegDisp1Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	u64 vlo, vhi;
	
	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*1);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);

//	if((addr&(~4095))!=((addr+16)&(~4095)))
//	{
//		BJX2_MemTranslateTlb(ctx, addr+0, 1);
//		BJX2_MemTranslateTlb(ctx, addr+16, 1);
//	}

//	ctx->regs[op->rn+0]=BJX2_MemGetQWordW(ctx, addr+0, ctx->regs[op->rq]);
//	ctx->regs[op->rn+1]=BJX2_MemGetQWordW(ctx, addr+8, ctx->regs[op->rq]);

	BJX2_MemGetXWordW(ctx, addr, ctx->regs[op->rq], &vlo, &vhi);
	ctx->regs[op->rn+0]=vlo;
	ctx->regs[op->rn+1]=vhi;
}

void BJX2_Op_MOVX2_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*8);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
//	if((addr&(~4095))!=((addr+16)&(~4095)))
//	{
//		BJX2_MemTranslateTlb(ctx, addr+0, 2);
//		BJX2_MemTranslateTlb(ctx, addr+16, 2);
//	}

//	BJX2_MemSetQWordW(ctx, addr+0, ctx->regs[op->rq], ctx->regs[op->rm+0]);
//	BJX2_MemSetQWordW(ctx, addr+8, ctx->regs[op->rq], ctx->regs[op->rm+1]);
	BJX2_MemSetXWordW(ctx, addr, ctx->regs[op->rq],
		ctx->regs[op->rm+0], ctx->regs[op->rm+1]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*8), 16);
}

void BJX2_Op_MOVX2_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	u64 vlo, vhi;

	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*8);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);

//	if((addr&(~4095))!=((addr+16)&(~4095)))
//	{
//		BJX2_MemTranslateTlb(ctx, addr+0, 1);
//		BJX2_MemTranslateTlb(ctx, addr+16, 1);
//	}

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*8), 16);

//	ctx->regs[op->rn+0]=BJX2_MemGetQWordW(ctx, addr+0, ctx->regs[op->rq]);
//	ctx->regs[op->rn+1]=BJX2_MemGetQWordW(ctx, addr+8, ctx->regs[op->rq]);

	BJX2_MemGetXWordW(ctx, addr, ctx->regs[op->rq], &vlo, &vhi);
	ctx->regs[op->rn+0]=vlo;
	ctx->regs[op->rn+1]=vhi;
}

void BJX2_Op_MOVX2_RegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
	if((addr&(~4095))!=((addr+16)&(~4095)))
	{
		BJX2_MemTranslateTlb(ctx, addr+0, 2);
		BJX2_MemTranslateTlb(ctx, addr+16, 2);
	}
	BJX2_MemSetQWordW(ctx, addr+0, ctx->regs[op->rq], ctx->regs[op->rm+0]);
	BJX2_MemSetQWordW(ctx, addr+8, ctx->regs[op->rq], ctx->regs[op->rm+1]);

//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1), 1);
}

void BJX2_Op_MOVX2_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*1);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
	if((addr&(~4095))!=((addr+16)&(~4095)))
	{
		BJX2_MemTranslateTlb(ctx, addr+0, 1);
		BJX2_MemTranslateTlb(ctx, addr+16, 1);
	}
	ctx->regs[op->rn+0]=BJX2_MemGetQWordW(ctx, addr+0, ctx->regs[op->rq]);
	ctx->regs[op->rn+1]=BJX2_MemGetQWordW(ctx, addr+8, ctx->regs[op->rq]);
}

void BJX2_Op_MOVX2_RegStPcIdx(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
	if((addr&(~4095))!=((addr+16)&(~4095)))
	{
		BJX2_MemTranslateTlb(ctx, addr+0, 2);
		BJX2_MemTranslateTlb(ctx, addr+16, 2);
	}
	BJX2_MemSetQWordW(ctx, addr+0, ctx->regs[op->rq], ctx->regs[op->rm+0]);
	BJX2_MemSetQWordW(ctx, addr+8, ctx->regs[op->rq], ctx->regs[op->rm+1]);
}

void BJX2_Op_MOVX2_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr addr;
	addr=(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]);
	ctx->trapc=op->pc;
	if(addr&7)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_MISAL);
	if((addr&(~4095))!=((addr+16)&(~4095)))
	{
		BJX2_MemTranslateTlb(ctx, addr+0, 1);
		BJX2_MemTranslateTlb(ctx, addr+16, 1);
	}
	ctx->regs[op->rn+0]=BJX2_MemGetQWordW(ctx, addr+0, ctx->regs[op->rq]);
	ctx->regs[op->rn+1]=BJX2_MemGetQWordW(ctx, addr+8, ctx->regs[op->rq]);
}


void BJX2_Op_MOVB_RegStDrAbs(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx, ctx->regs[BJX2_REG_DR], ctx->regs[op->rn]);
}

void BJX2_Op_MOVB_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx, ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_MOVW_RegStDrAbs(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx, ctx->regs[BJX2_REG_DR], ctx->regs[op->rn]);
}

void BJX2_Op_MOVW_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx, ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_MOVL_RegStDrAbs(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx, ctx->regs[BJX2_REG_DR], ctx->regs[op->rn]);
}

void BJX2_Op_MOVL_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx, ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_MOVQ_RegStDrAbs(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx, ctx->regs[BJX2_REG_DR], ctx->regs[op->rn]);
}

void BJX2_Op_MOVQ_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx, ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_MOVUB_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx, ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_MOVUW_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx, ctx->regs[BJX2_REG_DR]);
}

void BJX2_Op_MOVUL_LdDrAbsReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx, ctx->regs[BJX2_REG_DR]);
}


#if 1
void BJX2_Op_MOVB_RegStPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(op->pc2)+(op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVB_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}

void BJX2_Op_MOVW_RegStPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVW_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}

void BJX2_Op_MOVL_RegStPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVL_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}

void BJX2_Op_MOVQ_RegStPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm),
		ctx->regs[op->rn]);
}

void BJX2_Op_MOVQ_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}

void BJX2_Op_MOVUB_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}

void BJX2_Op_MOVUW_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}

void BJX2_Op_MOVUL_LdPcDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(op->pc2)+(op->imm));
}
#endif


#if 1
void BJX2_Op_MOVB_RegStPcIdx(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVB_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVW_RegStPcIdx(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVL_RegStPcIdx(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVQ_RegStPcIdx(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVQ_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVUB_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVUW_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVUL_LdPcIdxReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(op->pc2)+(bjx2_addr)(ctx->regs[op->ro]));
}
#endif


#if 0
void BJX2_Op_PUSH_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_SP]-=4;
	BJX2_MemSetDWord(ctx,
		ctx->regs[BJX2_REG_SP],
		ctx->regs[op->rn]);
}

void BJX2_Op_POP_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetDWord(ctx,
		ctx->regs[BJX2_REG_SP]);
	ctx->regs[BJX2_REG_SP]+=4;
}
#endif

#if 0
void BJX2_Op_PUSH_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_SP]-=8;
//	printf("PUSH %d, %X %llX\n", op->rn,
//		(u32)(ctx->regs[BJX2_REG_SP]),
//		ctx->regs[op->rn]);
	BJX2_MemSetQWord(ctx,
		ctx->regs[BJX2_REG_SP],
		ctx->regs[op->rn]);
}

void BJX2_Op_POP_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		ctx->regs[BJX2_REG_SP]);
//	printf("POP %d, %X %llX\n", op->rn,
//		(u32)(ctx->regs[BJX2_REG_SP]),
//		ctx->regs[op->rn]);
	ctx->regs[BJX2_REG_SP]+=8;
}
#endif

#if 0
void BJX2_Op_PUSH_FpReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_SP]-=8;
	BJX2_MemSetQWord(ctx,
		ctx->regs[BJX2_REG_SP],
		ctx->fpreg[op->rn]);
}

void BJX2_Op_POP_FpReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->fpreg[op->rn]=BJX2_MemGetQWord(ctx,
		ctx->regs[BJX2_REG_SP]);
	ctx->regs[BJX2_REG_SP]+=8;
}
#endif

#if 0
void BJX2_Op_PUSHX2_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_SP]-=16;

	BJX2_MemSetQWord(ctx,
		ctx->regs[BJX2_REG_SP]+8,
		ctx->regs[op->rn+1]);
	BJX2_MemSetQWord(ctx,
		ctx->regs[BJX2_REG_SP]+0,
		ctx->regs[op->rn+0]);
}

void BJX2_Op_POPX2_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn+0]=BJX2_MemGetQWord(ctx,
		ctx->regs[BJX2_REG_SP]+0);
	ctx->regs[op->rn+1]=BJX2_MemGetQWord(ctx,
		ctx->regs[BJX2_REG_SP]+8);
	ctx->regs[BJX2_REG_SP]+=16;
}
#endif


void BJX2_Op_MOVB_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1), 1);
}

void BJX2_Op_MOVB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*1), 1);
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVW_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*2),
		ctx->regs[op->rm]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*2), 2);
}

void BJX2_Op_MOVW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*2), 2);
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
}

void BJX2_Op_MOVL_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*4),
		ctx->regs[op->rm]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*4), 4);
}

void BJX2_Op_MOVL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*4), 4);
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*4));
}

void BJX2_Op_MOVQ_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*8),
		ctx->regs[op->rm]);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*8), 8);
}

void BJX2_Op_MOVQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*8), 8);
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*8));
}

void BJX2_Op_MOVUB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*1), 1);
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])));

}

void BJX2_Op_MOVUW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*2), 2);
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
}

void BJX2_Op_MOVUL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*4), 4);
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*4));
}


void BJX2_Op_MOVTW_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*2)+0,
		ctx->regs[op->rm]);
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*2)+2,
		ctx->regs[op->rm]>>16);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*2), 8);
}

void BJX2_Op_MOVHTW_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*2)+0,
		ctx->regs[op->rm]>>16);
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*2)+2,
		ctx->regs[op->rm]>>32);

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rn]),
		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*2), 8);
}

void BJX2_Op_MOVTW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*2), 2);

	lv=(s16)BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
	lv=((s64)(lv<<16))>>16;
	ctx->regs[op->rn]=lv;
}

void BJX2_Op_MOVUTW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*2), 2);

	lv=(s16)BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
	lv=((u64)(lv<<16))>>16;
	ctx->regs[op->rn]=lv;
}

void BJX2_Op_MOVHTW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*2), 2);

	lv=(s16)BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
	lv=lv<<16;
	ctx->regs[op->rn]=lv;
}



void BJX2_Op_MOVB_RegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);

//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1));
}

void BJX2_Op_MOVB_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVW_RegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);

//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1));
}

void BJX2_Op_MOVW_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVL_RegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);

//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1));
}

void BJX2_Op_MOVL_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVQ_RegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);

//	BJX2_DbgAddrAccessTrap(ctx,
//		(bjx2_addr)(ctx->regs[op->rn]),
//		(bjx2_addr)(ctx->regs[op->rn])+((ctx->regs[op->ro])*1));
}

void BJX2_Op_MOVQ_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVUB_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVUW_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVUL_LdReg2RegB(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}


s64 BJX2_OpI_LDOP_DoStOp(
	BJX2_Context *ctx, BJX2_Opcode *op, s64 val)
{
	s64 v1, sv;

	sv=ctx->regs[op->rm];
	if(op->ldop&8)
	{
		sv=op->rm&63;
		if(op->rm==BJX2_REG_DLR)	sv=0;
		if(op->rm==BJX2_REG_DHR)	sv=1;
	}

	switch((op->ldop)&7)
	{
	case 0:		v1=val;		break;
	case 1:		v1=val;		break;
	case 2:		v1=sv+val;	break;
	case 3:		v1=val-sv;	break;
	case 4:		v1=sv-val;	break;
	case 5:		v1=sv&val;	break;
	case 6:		v1=sv|val;	break;
	case 7:		v1=sv^val;	break;
	}
	return(v1);
}

void BJX2_Op_LDOPB_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;

	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+(op->imm);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 1);
	lv=(s32)BJX2_MemGetByteW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetByteW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPW_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+((op->imm)*2);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 2);
	lv=(s32)BJX2_MemGetWordW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetWordW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPL_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+((op->imm)*4);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 4);
	lv=(s32)BJX2_MemGetDWordW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetDWordW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPQ_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+((op->imm)*8);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 8);
	lv=(s32)BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetQWordW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPB_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	int sc;
	
	sc=1;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+
		(bjx2_addr)(ctx->regs[op->ro]*sc)+
		(op->imm);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 1);
	lv=(s32)BJX2_MemGetByteW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetByteW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPW_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	int sc;
	
	sc=2;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+
		(bjx2_addr)(ctx->regs[op->ro]*sc)+
		(op->imm);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 2);
	lv=(s32)BJX2_MemGetWordW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetWordW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPL_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	int sc;
	
	sc=4;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+
		(bjx2_addr)(ctx->regs[op->ro]*sc)+
		(op->imm);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 4);
	lv=(s32)BJX2_MemGetDWordW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetDWordW(ctx, addr, ctx->regs[op->rq], lv);
}

void BJX2_Op_LDOPQ_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;
	int sc;
	
	sc=8;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	ctx->trapc=op->pc;
	addr=(bjx2_addr)(ctx->regs[op->rn])+
		(bjx2_addr)(ctx->regs[op->ro]*sc)+
		(op->imm);
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rn]), addr, 8);
	lv=(s32)BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);
	lv=BJX2_OpI_LDOP_DoStOp(ctx, op, lv);
	BJX2_MemSetQWordW(ctx, addr, ctx->regs[op->rq], lv);
}


u64 BJX2_OpI_LDOP_DoLdOp(
	BJX2_Context *ctx, BJX2_Opcode *op, s64 val)
{
	s64 v1;

	switch((op->ldop)&7)
	{
	case 0:		v1=val;						break;
	case 1:		v1=val;						break;
	case 2:		v1=ctx->regs[op->rn]+val;	break;
	case 3:		v1=val-ctx->regs[op->rn];	break;
	case 4:		v1=ctx->regs[op->rn]-val;	break;
	case 5:		v1=ctx->regs[op->rn]&val;	break;
	case 6:		v1=ctx->regs[op->rn]|val;	break;
	case 7:		v1=ctx->regs[op->rn]^val;	break;
	}
	return(v1);
}

void BJX2_Op_LDOPB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), 1);
	lv=(s32)BJX2_MemGetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetByteW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*1),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 2);
	lv=(s32)BJX2_MemGetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetWordW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), 4);
	lv=(s32)BJX2_MemGetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetDWordW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}


void BJX2_Op_LDOPQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr, lv;

	addr=(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8);

	ctx->trapc=op->pc;

	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8), 8);

	lv=BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetQWordW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPUB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), 1);
	lv=(byte)BJX2_MemGetByteW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm), ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetByteW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*1),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPUW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), 2);
	lv=(u16)BJX2_MemGetWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2), ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetWordW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPUL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv;
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), 4);
	lv=(u32)BJX2_MemGetDWordW(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4), ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetDWordW(ctx,
			(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4),
			ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}



void BJX2_Op_LDOPB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=1;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;
	
	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		addr, 1);
	lv=(sbyte)BJX2_MemGetByteW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetByteW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=2;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;

	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]),
		addr, 2);
	lv=(s16)BJX2_MemGetWordW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetWordW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=4;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;

	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]), addr, 4);
	lv=(s32)BJX2_MemGetDWordW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetDWordW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=8;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;

	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rm]), addr, 8);
	lv=BJX2_MemGetQWordW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetQWordW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPUB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=1;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;

	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rm]), addr, 1);
	lv=(byte)BJX2_MemGetByteW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetByteW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPUW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=2;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;

	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx,
		(bjx2_addr)(ctx->regs[op->rm]), addr, 2);
	lv=(u16)BJX2_MemGetWordW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetWordW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}

void BJX2_Op_LDOPUL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 lv, addr;
	int sc;

	sc=4;
	if(op->ldsc&4)
		sc=1<<(op->ldsc&3);
	addr=(bjx2_addr)(ctx->regs[op->rm])+((ctx->regs[op->ro])*sc)+op->imm;

	ctx->trapc=op->pc;
	BJX2_DbgAddrAccessTrap(ctx, (bjx2_addr)(ctx->regs[op->rm]), addr, 4);
	lv=(u32)BJX2_MemGetDWordW(ctx, addr, ctx->regs[op->rq]);

	if(op->ldop==1)
	{
		BJX2_MemSetDWordW(ctx,
			addr, ctx->regs[op->rq], ctx->regs[op->rn]);
		ctx->regs[op->rn]=lv;
		return;
	}
		
	ctx->regs[op->rn]=BJX2_OpI_LDOP_DoLdOp(ctx, op, lv);
}
