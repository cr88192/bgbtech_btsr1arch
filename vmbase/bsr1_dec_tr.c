void BTSR1_DecTraceCb_SetupForTrace(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	ctx->tr_cur=tr;
	ctx->tr_rnxt=tr->lnknext;
	ctx->tr_rjmp=tr->jmpnext;
	ctx->regs[BTSR1_REG_PC]=tr->addr_nxt;
}

BTSR1_Trace *BTSR1_DecTraceCb_Run1(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run2(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run3(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run4(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run5(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run6(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run7(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run8(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}


BTSR1_Trace *BTSR1_DecTraceCb_Run9(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run10(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run11(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run12(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run13(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run14(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run15(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run16(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run17(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run18(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run19(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run20(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run21(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run22(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run23(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run24(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run25(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run26(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run27(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run28(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run29(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run30(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run31(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Run32(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_Opcode **ops;
	BTSR1_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	return(ctx->tr_rnxt);
}

BTSR1_Trace *BTSR1_DecTraceCb_Bad(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_INV_MEX);
	return(NULL);
}

int BTSR1_DecodeTraceForAddr(BTSR1_Context *ctx,
	BTSR1_Trace *tr, btsr1_addr addr)
{
	BTSR1_Opcode *op;
	int ldrl, vdrl;
	int pc, nc, ncyc, npc, jpc;
	
	if((addr&1) || (addr&(~65535)))
	{
		tr->n_ops=0;
		tr->n_cyc=0;
		tr->Run=BTSR1_DecTraceCb_Bad;
		return(-1);
	}
	
	ldrl=0; vdrl=0;
	nc=0; ncyc=0;
	pc=addr; npc=0; jpc=0;
	while(nc<BTSR1_TR_MAXOP)
	{
		op=BTSR1_ContextAllocOpcode(ctx);
		BTSR1_DecodeOpcodeForAddr(ctx, op, pc);
		tr->ops[nc++]=op;
//		ncyc++;
		ncyc+=op->cyc;
		pc+=2;
		if(op->fl&BTSR1_OPFL_TWOWORD)
		{
			ncyc++;
			pc+=2;
		}

		if(op->fl&BTSR1_OPFL_CTRLF)
		{
			if(op->nmid!=BTSR1_NMID_BRA)
				{ npc=pc; }

			if(	(op->nmid==BTSR1_NMID_BRA)	||
				(op->nmid==BTSR1_NMID_BSR)	||
				(op->nmid==BTSR1_NMID_BT)	||
				(op->nmid==BTSR1_NMID_BF)	)
			{
				if(op->fmid==BTSR1_FMID_PCDISP)
				{ 
					jpc=(op->pc+2)+(op->imm*2);
				}
				if(vdrl && (op->fmid==BTSR1_FMID_PCDR4))
				{
					jpc=(op->pc+2)+(ldrl*32)+(op->imm*2);
				}

				if(vdrl && (op->fmid==BTSR1_FMID_PCDR))
				{
					jpc=(op->pc+2)+(ldrl*2);
				}
			}
		
			break;
		}else
		{
			if((op->nmid==BTSR1_NMID_LDIZ) ||
				(op->nmid==BTSR1_NMID_LDIN))
			{
				ldrl=op->imm; vdrl=1;
			}else if(op->nmid==BTSR1_NMID_LDISH)
			{
				ldrl=(ldrl<<8)|(op->imm&255);
			}else
			{
				vdrl=0;
			}
		}
	}

	if(nc>=BTSR1_TR_MAXOP)
		{ npc=pc; }

	tr->n_ops=nc;
	tr->n_cyc=ncyc;
	tr->addr_nxt=npc;
	tr->addr_jmp=jpc;
	
	if(npc)
		{ tr->lnknext=BTSR1_GetTraceForAddr(ctx, pc); }
	if(jpc)
		{ tr->jmpnext=BTSR1_GetTraceForAddr(ctx, jpc); }
	
	switch(nc)
	{
	case  1: tr->Run=BTSR1_DecTraceCb_Run1; break;
	case  2: tr->Run=BTSR1_DecTraceCb_Run2; break;
	case  3: tr->Run=BTSR1_DecTraceCb_Run3; break;
	case  4: tr->Run=BTSR1_DecTraceCb_Run4; break;
	case  5: tr->Run=BTSR1_DecTraceCb_Run5; break;
	case  6: tr->Run=BTSR1_DecTraceCb_Run6; break;
	case  7: tr->Run=BTSR1_DecTraceCb_Run7; break;
	case  8: tr->Run=BTSR1_DecTraceCb_Run8; break;
	case  9: tr->Run=BTSR1_DecTraceCb_Run9; break;
	case 10: tr->Run=BTSR1_DecTraceCb_Run10; break;
	case 11: tr->Run=BTSR1_DecTraceCb_Run11; break;
	case 12: tr->Run=BTSR1_DecTraceCb_Run12; break;
	case 13: tr->Run=BTSR1_DecTraceCb_Run13; break;
	case 14: tr->Run=BTSR1_DecTraceCb_Run14; break;
	case 15: tr->Run=BTSR1_DecTraceCb_Run15; break;
	case 16: tr->Run=BTSR1_DecTraceCb_Run16; break;
	case 17: tr->Run=BTSR1_DecTraceCb_Run17; break;
	case 18: tr->Run=BTSR1_DecTraceCb_Run18; break;
	case 19: tr->Run=BTSR1_DecTraceCb_Run19; break;
	case 20: tr->Run=BTSR1_DecTraceCb_Run20; break;
	case 21: tr->Run=BTSR1_DecTraceCb_Run21; break;
	case 22: tr->Run=BTSR1_DecTraceCb_Run22; break;
	case 23: tr->Run=BTSR1_DecTraceCb_Run23; break;
	case 24: tr->Run=BTSR1_DecTraceCb_Run24; break;
	case 25: tr->Run=BTSR1_DecTraceCb_Run25; break;
	case 26: tr->Run=BTSR1_DecTraceCb_Run26; break;
	case 27: tr->Run=BTSR1_DecTraceCb_Run27; break;
	case 28: tr->Run=BTSR1_DecTraceCb_Run28; break;
	case 29: tr->Run=BTSR1_DecTraceCb_Run29; break;
	case 30: tr->Run=BTSR1_DecTraceCb_Run30; break;
	case 31: tr->Run=BTSR1_DecTraceCb_Run31; break;
	case 32: tr->Run=BTSR1_DecTraceCb_Run32; break;
	}
	
	return(0);
}
