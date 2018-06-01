void BTSR1_Op_MOVB_RegStReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ BTSR1_MemSetByte(ctx, ctx->regs[op->rn], ctx->regs[op->rm]); }
void BTSR1_Op_MOVB_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(sbyte)BTSR1_MemGetByte(ctx, ctx->regs[op->rm]); }

void BTSR1_Op_MOVW_RegStReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ BTSR1_MemSetWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]); }
void BTSR1_Op_MOVW_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(s16)BTSR1_MemGetWord(ctx, ctx->regs[op->rm]); }

void BTSR1_Op_MOVD_RegStReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ BTSR1_MemSetDWord(ctx, ctx->regs[op->rn], ctx->regs[op->rm]); }
void BTSR1_Op_MOVD_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx, ctx->regs[op->rm]); }

void BTSR1_Op_MOVUB_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx, ctx->regs[op->rm]); }
void BTSR1_Op_MOVUW_LdRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx, ctx->regs[op->rm]); }


void BTSR1_Op_MOVB_RegStDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetByte(ctx,
		ctx->regs[op->rn]+(ctx->regs[BTSR1_REG_DR]*1),
		ctx->regs[op->rm]);
}

void BTSR1_Op_MOVB_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)BTSR1_MemGetByte(ctx,
		ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*1));
}

void BTSR1_Op_MOVW_RegStDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetWord(ctx,
		ctx->regs[op->rn]+(ctx->regs[BTSR1_REG_DR]*2),
		ctx->regs[op->rm]);
}

void BTSR1_Op_MOVW_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s16)BTSR1_MemGetWord(ctx,
		ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*2));
}

void BTSR1_Op_MOVD_RegStDrReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetDWord(ctx,
		ctx->regs[op->rn]+(ctx->regs[BTSR1_REG_DR]*4),
		ctx->regs[op->rm]);
}

void BTSR1_Op_MOVD_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx,
		ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*4));
}

void BTSR1_Op_MOVUB_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx,
		ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*1));
}

void BTSR1_Op_MOVUW_LdDrRegReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx,
		ctx->regs[op->rm]+(ctx->regs[BTSR1_REG_DR]*2));
}


void BTSR1_Op_MOVB_RegStDrPc(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetByte(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVB_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)BTSR1_MemGetByte(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1));
}

void BTSR1_Op_MOVW_RegStDrPc(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVW_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s16)BTSR1_MemGetWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1));
}

void BTSR1_Op_MOVD_RegStDrPc(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetDWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVD_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1));
}

void BTSR1_Op_MOVUB_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1));
}

void BTSR1_Op_MOVUW_LdDrPcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*1));
}


#if 1
void BTSR1_Op_MOVB_RegStDr4Pc(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetByte(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVB_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)BTSR1_MemGetByte(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm));
}

void BTSR1_Op_MOVW_RegStDr4Pc(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVW_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s16)BTSR1_MemGetWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm));
}

void BTSR1_Op_MOVD_RegStDr4Pc(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetDWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVD_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm));
}

void BTSR1_Op_MOVUB_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm));
}

void BTSR1_Op_MOVUW_LdDr4PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx,
		(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*16+op->imm));
}
#endif

void BTSR1_Op_MOVB_RegStRegDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetByte(ctx,
		ctx->regs[op->rn]+(op->imm*1),
		ctx->regs[op->rm]);
}

void BTSR1_Op_MOVB_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetByte(ctx,
		ctx->regs[op->rm]+(op->imm*1));
}

void BTSR1_Op_MOVW_RegStRegDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetWord(ctx,
		ctx->regs[op->rn]+(op->imm*2),
		ctx->regs[op->rm]);
}

void BTSR1_Op_MOVW_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetWord(ctx,
		ctx->regs[op->rm]+(op->imm*2));
}

void BTSR1_Op_MOVD_RegStRegDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetDWord(ctx,
		ctx->regs[op->rn]+(op->imm*4),
		ctx->regs[op->rm]);
}

void BTSR1_Op_MOVD_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx,
		ctx->regs[op->rm]+(op->imm*4));
}

void BTSR1_Op_MOVUB_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx,
		ctx->regs[op->rm]+(op->imm*1));
}

void BTSR1_Op_MOVUW_LdRegDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx,
		ctx->regs[op->rm]+(op->imm*2));
}


void BTSR1_Op_MOVB_RegStDrAbs(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ BTSR1_MemSetByte(ctx, ctx->regs[BTSR1_REG_DR], ctx->regs[op->rn]); }
void BTSR1_Op_MOVB_LdDrAbsReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(sbyte)BTSR1_MemGetByte(ctx, ctx->regs[BTSR1_REG_DR]); }
void BTSR1_Op_MOVW_RegStDrAbs(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ BTSR1_MemSetWord(ctx, ctx->regs[BTSR1_REG_DR], ctx->regs[op->rn]); }
void BTSR1_Op_MOVW_LdDrAbsReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(s16)BTSR1_MemGetWord(ctx, ctx->regs[BTSR1_REG_DR]); }
void BTSR1_Op_MOVD_RegStDrAbs(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ BTSR1_MemSetDWord(ctx, ctx->regs[BTSR1_REG_DR], ctx->regs[op->rn]); }
void BTSR1_Op_MOVD_LdDrAbsReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx, ctx->regs[BTSR1_REG_DR]); }

void BTSR1_Op_MOVUB_LdDrAbsReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx, ctx->regs[BTSR1_REG_DR]); }
void BTSR1_Op_MOVUW_LdDrAbsReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
	{ ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx, ctx->regs[BTSR1_REG_DR]); }


#if 1
void BTSR1_Op_MOVB_RegStPcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetByte(ctx,
		(op->pc+2)+(op->imm),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVB_LdPcDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(sbyte)BTSR1_MemGetByte(ctx,
		(op->pc+2)+(op->imm));
}

void BTSR1_Op_MOVW_RegStPcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetWord(ctx,
		(op->pc+2)+(op->imm),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVW_LdPcDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s16)BTSR1_MemGetWord(ctx,
		(op->pc+2)+(op->imm));
}

void BTSR1_Op_MOVD_RegStPcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_MemSetDWord(ctx,
		(op->pc+2)+(op->imm),
		ctx->regs[op->rn]);
}

void BTSR1_Op_MOVD_LdPcDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(s32)BTSR1_MemGetDWord(ctx,
		(op->pc+2)+(op->imm));
}

void BTSR1_Op_MOVUB_LdPcDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(byte)BTSR1_MemGetByte(ctx,
		(op->pc+2)+(op->imm));
}

void BTSR1_Op_MOVUW_LdPcDispReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=(u16)BTSR1_MemGetWord(ctx,
		(op->pc+2)+(op->imm));
}
#endif


void BTSR1_Op_PUSH_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_SP]-=4;
	BTSR1_MemSetDWord(ctx,
		ctx->regs[BTSR1_REG_SP],
		ctx->regs[op->rn]);
}

void BTSR1_Op_POP_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[op->rn]=BTSR1_MemGetDWord(ctx,
		ctx->regs[BTSR1_REG_SP]);
	ctx->regs[BTSR1_REG_SP]+=4;
}
