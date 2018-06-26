void BJX2_Op_BRA_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BSR_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BT_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BSR_PcDisp2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_LR]=op->pc+4;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BF_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BRA_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
	ctx->tr_rnxt=NULL;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BSR_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
	ctx->tr_rnxt=NULL;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_JumpUpdatePredicted(BJX2_Context *ctx)
{
	BJX2_Trace *tr;
	u32 pc;

#if 1
	tr=ctx->tr_rjmp;
	pc=ctx->regs[BJX2_REG_PC];
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
		tr=BJX2_GetTraceForAddr(ctx, pc);
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

void BJX2_Op_BRA_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[BJX2_REG_DR]*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BJX2_JumpUpdatePredicted(ctx);

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BSR_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[BJX2_REG_DR]*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BJX2_JumpUpdatePredicted(ctx);

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BT_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[BJX2_REG_DR]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=NULL;
//		BJX2_JumpUpdatePredicted(ctx);

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BF_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[BJX2_REG_DR]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=NULL;
//		BJX2_JumpUpdatePredicted(ctx);

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BRA_PcDr4(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr pc;
	
	pc=(op->pc2)+(ctx->regs[BJX2_REG_DR]*32+op->imm*2);
	ctx->regs[BJX2_REG_PC]=pc;
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=BJX2_GetTraceForAddr(ctx, pc);
//	BJX2_JumpUpdatePredicted(ctx);

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BSR_PcDr4(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr pc;
	
	pc=(op->pc2)+(ctx->regs[BJX2_REG_DR]*32+op->imm*2);
	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=pc;
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=BJX2_GetTraceForAddr(ctx, pc);
//	BJX2_JumpUpdatePredicted(ctx);

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BT_PcDr4(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr pc;
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		pc=(op->pc2)+(ctx->regs[BJX2_REG_DR]*32+op->imm*2);
		ctx->regs[BJX2_REG_PC]=pc;
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=BJX2_GetTraceForAddr(ctx, pc);
//		BJX2_JumpUpdatePredicted(ctx);

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BF_PcDr4(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr pc;
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		pc=(op->pc2)+(ctx->regs[BJX2_REG_DR]*32+op->imm*2);
		ctx->regs[BJX2_REG_PC]=pc;
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=BJX2_GetTraceForAddr(ctx, pc);
//		BJX2_JumpUpdatePredicted(ctx);
	}
}

void BJX2_Op_RTS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=
		ctx->regs[BJX2_REG_LR];
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_RET_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[BJX2_REG_PC]=
//		BJX2_MemGetDWord(ctx,
//			ctx->regs[BJX2_REG_SP]);
//	ctx->regs[BJX2_REG_SP]+=4;

	ctx->regs[BJX2_REG_PC]=
		BJX2_MemGetQWord(ctx,
			ctx->regs[BJX2_REG_SP]);
	ctx->regs[BJX2_REG_SP]+=8;

//	ctx->regs[BJX2_REG_PC]=
//		ctx->regs[BJX2_REG_LR];
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_BREAK_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BREAK);
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_SLEEP_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_SLEEP);
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_INVOP_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	ctx->tr_rnxt=NULL;
}


void BJX2_Op_RTE_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_FaultLeaveInterrupt(ctx);
//	ctx->regs[BJX2_REG_PC]=
//		ctx->regs[BJX2_REG_LR];
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_BRA_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
	ctx->tr_rnxt=NULL;
	
	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BSR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
	ctx->tr_rnxt=NULL;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
		ctx->tr_rnxt=NULL;

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BF_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
		ctx->tr_rnxt=NULL;

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}
