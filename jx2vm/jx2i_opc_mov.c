void BJX2_Op_MOVB_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
}

void BJX2_Op_MOVB_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx, ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx, ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_RegStReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_LdRegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx, ctx->regs[op->rm]);
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
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm));
}

void BJX2_Op_MOVW_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*2),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2));
}

void BJX2_Op_MOVL_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*4),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4));
}

void BJX2_Op_MOVQ_RegStRegDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(op->imm*8),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVQ_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*8));
}

void BJX2_Op_MOVUB_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm));
}

void BJX2_Op_MOVUW_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*2));
}

void BJX2_Op_MOVUL_LdRegDispReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(op->imm*4));
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

#if 1
void BJX2_Op_PUSH_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_SP]-=8;
	BJX2_MemSetQWord(ctx,
		ctx->regs[BJX2_REG_SP],
		ctx->regs[op->rn]);
}

void BJX2_Op_POP_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		ctx->regs[BJX2_REG_SP]);
	ctx->regs[BJX2_REG_SP]+=8;
}
#endif

#if 1
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


void BJX2_Op_MOVB_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(sbyte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+(bjx2_addr)(ctx->regs[op->ro]));
}

void BJX2_Op_MOVW_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*2),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
}

void BJX2_Op_MOVL_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*4),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(s32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*4));
}

void BJX2_Op_MOVQ_RegStReg2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+((bjx2_addr)(ctx->regs[op->ro])*8),
		ctx->regs[op->rm]);
}

void BJX2_Op_MOVQ_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=BJX2_MemGetQWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*8));
}

void BJX2_Op_MOVUB_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(byte)BJX2_MemGetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])));
}

void BJX2_Op_MOVUW_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u16)BJX2_MemGetWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*2));
}

void BJX2_Op_MOVUL_LdReg2Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[op->rn]=(u32)BJX2_MemGetDWord(ctx,
		(bjx2_addr)(ctx->regs[op->rm])+((bjx2_addr)(ctx->regs[op->ro])*4));
}



void BJX2_Op_MOVB_RegStReg2B(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	BJX2_MemSetByte(ctx,
		(bjx2_addr)(ctx->regs[op->rn])+(bjx2_addr)(ctx->regs[op->ro]),
		ctx->regs[op->rm]);
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
