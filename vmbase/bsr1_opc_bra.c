void BTSR1_Op_BRA_PcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
}

void BTSR1_Op_BSR_PcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_LR]=op->pc+2;
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
}

void BTSR1_Op_BT_PcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[BTSR1_REG_SR]&1)
	{
		ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
	}
}

void BTSR1_Op_BSR_PcDisp2(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_LR]=op->pc+4;
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
}

void BTSR1_Op_BF_PcDisp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(!(ctx->regs[BTSR1_REG_SR]&1))
	{
		ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
	}
}

void BTSR1_Op_BRA_PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(ctx->regs[op->rn]*2);
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_BSR_PcReg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_LR]=op->pc+2;
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(ctx->regs[op->rn]*2);
	ctx->tr_rnxt=NULL;
}

void BTSR1_JumpUpdatePredicted(BTSR1_Context *ctx)
{
	BTSR1_Trace *tr;
	u32 pc;

#if 1
	tr=ctx->tr_rjmp;
	pc=ctx->regs[BTSR1_REG_PC];
	if(tr)
	{
		if(tr->addr==pc)
		{
			ctx->tr_rnxt=tr;
			return;
		}
	}
#endif

#if 0
	if(!ctx->status)
	{
		tr=BTSR1_GetTraceForAddr(ctx, pc);
		if(ctx->tr_rnxt)
		{
			ctx->tr_rnxt=tr;
			ctx->tr_cur->jmpnext=tr;
			return;
		}
	}
#endif

//	ctx->tr_rnxt=ctx->tr_rjmp;
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_BRA_PcDr(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BTSR1_JumpUpdatePredicted(ctx);
}

void BTSR1_Op_BSR_PcDr(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_LR]=op->pc+2;
	ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BTSR1_JumpUpdatePredicted(ctx);
}

void BTSR1_Op_BT_PcDr(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[BTSR1_REG_SR]&1)
	{
		ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=NULL;
//		BTSR1_JumpUpdatePredicted(ctx);
	}
}

void BTSR1_Op_BF_PcDr(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(!(ctx->regs[BTSR1_REG_SR]&1))
	{
		ctx->regs[BTSR1_REG_PC]=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=NULL;
//		BTSR1_JumpUpdatePredicted(ctx);
	}
}

void BTSR1_Op_BRA_PcDr4(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	btsr1_addr pc;
	
	pc=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*32+op->imm*2);
	ctx->regs[BTSR1_REG_PC]=pc;
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=BTSR1_GetTraceForAddr(ctx, pc);
//	BTSR1_JumpUpdatePredicted(ctx);
}

void BTSR1_Op_BSR_PcDr4(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	btsr1_addr pc;
	
	pc=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*32+op->imm*2);
	ctx->regs[BTSR1_REG_LR]=op->pc+2;
	ctx->regs[BTSR1_REG_PC]=pc;
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=BTSR1_GetTraceForAddr(ctx, pc);
//	BTSR1_JumpUpdatePredicted(ctx);
}

void BTSR1_Op_BT_PcDr4(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	btsr1_addr pc;
	if(ctx->regs[BTSR1_REG_SR]&1)
	{
		pc=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*32+op->imm*2);
		ctx->regs[BTSR1_REG_PC]=pc;
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=BTSR1_GetTraceForAddr(ctx, pc);
//		BTSR1_JumpUpdatePredicted(ctx);
	}
}

void BTSR1_Op_BF_PcDr4(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	btsr1_addr pc;
	if(!(ctx->regs[BTSR1_REG_SR]&1))
	{
		pc=(op->pc+2)+(ctx->regs[BTSR1_REG_DR]*32+op->imm*2);
		ctx->regs[BTSR1_REG_PC]=pc;
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=BTSR1_GetTraceForAddr(ctx, pc);
//		BTSR1_JumpUpdatePredicted(ctx);
	}
}

void BTSR1_Op_RTS_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_PC]=
		ctx->regs[BTSR1_REG_LR];
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_RET_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_PC]=
		BTSR1_MemGetDWord(ctx,
			ctx->regs[BTSR1_REG_SP]);
	ctx->regs[BTSR1_REG_SP]+=4;
//	ctx->regs[BTSR1_REG_PC]=
//		ctx->regs[BTSR1_REG_LR];
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_BREAK_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_BREAK);
}

void BTSR1_Op_SLEEP_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_SLEEP);
}


void BTSR1_Op_RTE_None(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	BTSR1_FaultLeaveInterrupt(ctx);
//	ctx->regs[BTSR1_REG_PC]=
//		ctx->regs[BTSR1_REG_LR];
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_BRA_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_PC]=ctx->regs[op->rn];
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_BSR_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	ctx->regs[BTSR1_REG_LR]=op->pc+2;
	ctx->regs[BTSR1_REG_PC]=ctx->regs[op->rn];
	ctx->tr_rnxt=NULL;
}

void BTSR1_Op_BT_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(ctx->regs[BTSR1_REG_SR]&1)
	{
		ctx->regs[BTSR1_REG_PC]=ctx->regs[op->rn];
		ctx->tr_rnxt=NULL;
	}
}

void BTSR1_Op_BF_Reg(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	if(!(ctx->regs[BTSR1_REG_SR]&1))
	{
		ctx->regs[BTSR1_REG_PC]=ctx->regs[op->rn];
		ctx->tr_rnxt=NULL;
	}
}
