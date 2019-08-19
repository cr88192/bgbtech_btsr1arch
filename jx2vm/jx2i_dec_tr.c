void BJX2_MemSimTraceIS(BJX2_Context *ctx, BJX2_Trace *tr)
{
	int i;
	
	for(i=0; i<tr->n_ops; i++)
		BJX2_MemSimAddrL1I(ctx, tr->ops[i]->pc);
}

force_inline void BJX2_DecTraceCb_SetupForTrace(
	BJX2_Context *ctx, BJX2_Trace *tr)
{
	tr->runcnt++;
	ctx->tr_cur=tr;
	ctx->tr_rnxt=tr->lnknext;
	ctx->tr_rjmp=tr->jmpnext;
	ctx->regs[BJX2_REG_PC]=tr->addr_nxt;

#ifdef X86_64
	if(!(ctx->use_jit))
		{ BJX2_MemSimTraceIS(ctx, tr); }
#endif
}

BJX2_Trace *BJX2_DecTraceCb_Run1(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run2(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run3(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run4(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run5(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run6(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run7(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run8(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}


BJX2_Trace *BJX2_DecTraceCb_Run9(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run10(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run11(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run12(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run13(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run14(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run15(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run16(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run17(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run18(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
	ops=tr->ops;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run19(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run20(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run21(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run22(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run23(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run24(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run25(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run26(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run27(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run28(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run29(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run30(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run31(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run32(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode **ops;
	BJX2_DecTraceCb_SetupForTrace(ctx, tr);
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Bad(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MEX);
	return(NULL);
}

int BJX2_CheckWexSanity3W(BJX2_Context *ctx,
	BJX2_Opcode *op1, BJX2_Opcode *op2, BJX2_Opcode *op3)
{
	return(1);
}

int BJX2_CheckWexSanity2W(BJX2_Context *ctx,
	BJX2_Opcode *op1, BJX2_Opcode *op2)
{
	return(1);
}

int BJX2_DecodeTraceForAddr(BJX2_Context *ctx,
	BJX2_Trace *tr, bjx2_addr addr)
{
	static int rec=0;
	BJX2_Opcode *op, *op1, *op2;
	int ldrl, vdrl, brk, wexmd;
	int pc, nc, ncyc, npc, jpc;
	int i, j;
	
//	if((addr&1) || (addr&(~65535)))
	if(addr&1)
	{
		tr->n_ops=0;
		tr->n_cyc=0;
		tr->Run=BJX2_DecTraceCb_Bad;
		return(-1);
	}
	
	rec++;
	
//	ctx->v_wexmd=0xFF;
	
	ldrl=0; vdrl=0;
	nc=0; ncyc=0;
	pc=addr; npc=0; jpc=0; brk=0;
	while(nc<BJX2_TR_MAXOP)
	{
		op=BJX2_ContextAllocOpcode(ctx);
		BJX2_DecodeOpcodeForAddr(ctx, op, pc);
		tr->ops[nc++]=op;
//		ncyc++;
		ncyc+=op->cyc;
		pc+=2;
		if(op->fl&BJX2_OPFL_TWOWORD)
		{
//			ncyc++;
			pc+=2;
		}
		if(op->fl&BJX2_OPFL_TRIWORD)
		{
//			ncyc+=2;
			pc+=4;
		}
		
		op->pc2=pc;
		if(op->fmid==BJX2_FMID_CHAIN)
		{
			op1=op->data;
			op1->pc2=pc;
		}

		if(op->fl&BJX2_OPFL_CTRLF)
		{
#if 0
			if(ctx->use_jit)
			{
				if(op->nmid==BJX2_NMID_BRA)
				{
					if(op->fmid==BJX2_FMID_PCDISP)
					{
						nc--;
						pc=op->pc2+(op->imm*2);
						continue;
					}
				}
			}
#endif

//			if(op->nmid!=BJX2_NMID_BRA)
			if((op->nmid!=BJX2_NMID_BRA) &&
				(op->nmid!=BJX2_NMID_RTS) &&
				(op->nmid!=BJX2_NMID_RTE) &&
				(op->nmid!=BJX2_NMID_RET))
				{ npc=pc; }

			if(op->nmid==BJX2_NMID_BREAK)
				brk=1;

			if(	(op->nmid==BJX2_NMID_BRA)	||
				(op->nmid==BJX2_NMID_BSR)	||
				(op->nmid==BJX2_NMID_BT)	||
				(op->nmid==BJX2_NMID_BF)	)
			{
				if(op->fmid==BJX2_FMID_PCDISP)
				{ 
					jpc=op->pc2+(op->imm*2);
				}
				if(vdrl && (op->fmid==BJX2_FMID_PCDR4))
				{
					jpc=op->pc2+(ldrl*32)+(op->imm*2);
				}

				if(vdrl && (op->fmid==BJX2_FMID_PCDR))
				{
					jpc=op->pc2+(ldrl*2);
				}
			}
		
			break;
		}else
		{
			if((op->nmid==BJX2_NMID_LDIZ) ||
				(op->nmid==BJX2_NMID_LDIN))
			{
				ldrl=op->imm; vdrl=1;
			}else if(op->nmid==BJX2_NMID_LDISH8)
			{
				ldrl=(ldrl<<8)|(op->imm&255);
			}else
			{
				vdrl=0;
			}
		}
	}
	
	if(nc>BJX2_TR_MAXOP)
		{ JX2_DBGBREAK }
	
#if 1
	for(i=0; i<nc; i++)
	{
		wexmd=ctx->wexmd;
		if(ctx->v_wexmd!=0xFF)
			wexmd=ctx->v_wexmd;

//		if((i+1)>=BJX2_TR_MAXOP)
//			continue;
	
		op1=NULL;
		op2=NULL;
	
		op=tr->ops[i];
		if((i+1)<BJX2_TR_MAXOP)
			op1=tr->ops[i+1];
		if((i+2)<BJX2_TR_MAXOP)
			op2=tr->ops[i+2];

#if 1
		if((op->fl&BJX2_OPFL_WEX) && op1)
		{
			if((i+2)>nc)
				continue;
			
			if(!wexmd)
			{
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
			}

			if((op1->fl&BJX2_OPFL_WEX) && op2)
			{
				if(BJX2_CheckWexSanity3W(ctx, op, op1, op2)<=0)
				{
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
				}
			}else
			{
				if(BJX2_CheckWexSanity2W(ctx, op, op1)<=0)
				{
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
				}
			}

			
			j=op->cyc;
			if(op1->cyc>j)
				j=op1->cyc;
			op->cyc=0;
			op1->cyc=j;
			continue;
		}
#endif

#if 1
		if((op->nmid==BJX2_NMID_MOVB) ||
			(op->nmid==BJX2_NMID_MOVW) ||
			(op->nmid==BJX2_NMID_MOVL) ||
			(op->nmid==BJX2_NMID_MOVQ) ||
			(op->nmid==BJX2_NMID_MOVUB) ||
			(op->nmid==BJX2_NMID_MOVUW) ||
			(op->nmid==BJX2_NMID_MOVUL) ||
			(op->nmid==BJX2_NMID_MOVDL) ||
			(op->nmid==BJX2_NMID_FMOVS) ||
			(op->nmid==BJX2_NMID_FMOVD))
		{		
			if((op->fmid==BJX2_FMID_LDREGREG) ||
				(op->fmid==BJX2_FMID_LDDRREGREG) ||
				(op->fmid==BJX2_FMID_LDREGDISPREG) ||
				(op->fmid==BJX2_FMID_LDREG2REG))
			{
				if((i+3)>nc)
					continue;

				if(	(op->rn!=op1->rm) &&
					(op->rn!=op1->rn) &&
					(op->rn!=op1->ro))
				{
					if(	(op->rn!=op2->rm) &&
						(op->rn!=op2->rn) &&
						(op->rn!=op2->ro))
					{
						op->cyc=1;
					}else
					{
						op->cyc=2;
					}
				}
			}

			if((op->fmid==BJX2_FMID_REGSTREG) ||
				(op->fmid==BJX2_FMID_REGSTDRREG) ||
				(op->fmid==BJX2_FMID_REGSTREGDISP) ||
				(op->fmid==BJX2_FMID_REGSTREG2))
			{
				op->cyc=1;
			}
			
		}

		if((op->nmid==BJX2_NMID_PUSH) ||
			(op->nmid==BJX2_NMID_FPUSH))
		{
			op->cyc=1;
		}
#endif

#if 1
		if((op->nmid==BJX2_NMID_POP) ||
			(op->nmid==BJX2_NMID_FPOP) ||
			(op->nmid==BJX2_NMID_ADD) ||
			(op->nmid==BJX2_NMID_SUB))
		{
			if((i+3)>nc)
				continue;

			if(	(op->rn!=op1->rm) &&
				(op->rn!=op1->rn) &&
				(op->rn!=op1->ro))
			{
				if(	(op->rn!=op2->rm) &&
					(op->rn!=op2->rn) &&
					(op->rn!=op2->ro))
				{
					op->cyc=1;
				}else
				{
					op->cyc=2;
				}
			}
		}
#endif
	}
#endif

	ncyc=0;
	for(i=0; i<nc; i++)
	{
		op=tr->ops[i];
		ncyc+=op->cyc;
	}

	if(nc>=BJX2_TR_MAXOP)
		{ npc=pc; }

	tr->n_ops=nc;
	tr->n_cyc=ncyc;
	tr->addr_nxt=npc;
	tr->addr_jmp=jpc;
	
	if(rec<64)
	{
		if(npc && !brk)
			{ tr->lnknext=BJX2_GetTraceForAddr(ctx, pc); }
		if(jpc)
			{ tr->jmpnext=BJX2_GetTraceForAddr(ctx, jpc); }
	}
	
	switch(nc)
	{
	case  1: tr->Run=BJX2_DecTraceCb_Run1; break;
	case  2: tr->Run=BJX2_DecTraceCb_Run2; break;
	case  3: tr->Run=BJX2_DecTraceCb_Run3; break;
	case  4: tr->Run=BJX2_DecTraceCb_Run4; break;
	case  5: tr->Run=BJX2_DecTraceCb_Run5; break;
	case  6: tr->Run=BJX2_DecTraceCb_Run6; break;
	case  7: tr->Run=BJX2_DecTraceCb_Run7; break;
	case  8: tr->Run=BJX2_DecTraceCb_Run8; break;
	case  9: tr->Run=BJX2_DecTraceCb_Run9; break;
	case 10: tr->Run=BJX2_DecTraceCb_Run10; break;
	case 11: tr->Run=BJX2_DecTraceCb_Run11; break;
	case 12: tr->Run=BJX2_DecTraceCb_Run12; break;
	case 13: tr->Run=BJX2_DecTraceCb_Run13; break;
	case 14: tr->Run=BJX2_DecTraceCb_Run14; break;
	case 15: tr->Run=BJX2_DecTraceCb_Run15; break;
	case 16: tr->Run=BJX2_DecTraceCb_Run16; break;
	case 17: tr->Run=BJX2_DecTraceCb_Run17; break;
	case 18: tr->Run=BJX2_DecTraceCb_Run18; break;
	case 19: tr->Run=BJX2_DecTraceCb_Run19; break;
	case 20: tr->Run=BJX2_DecTraceCb_Run20; break;
	case 21: tr->Run=BJX2_DecTraceCb_Run21; break;
	case 22: tr->Run=BJX2_DecTraceCb_Run22; break;
	case 23: tr->Run=BJX2_DecTraceCb_Run23; break;
	case 24: tr->Run=BJX2_DecTraceCb_Run24; break;
	case 25: tr->Run=BJX2_DecTraceCb_Run25; break;
	case 26: tr->Run=BJX2_DecTraceCb_Run26; break;
	case 27: tr->Run=BJX2_DecTraceCb_Run27; break;
	case 28: tr->Run=BJX2_DecTraceCb_Run28; break;
	case 29: tr->Run=BJX2_DecTraceCb_Run29; break;
	case 30: tr->Run=BJX2_DecTraceCb_Run30; break;
	case 31: tr->Run=BJX2_DecTraceCb_Run31; break;
	case 32: tr->Run=BJX2_DecTraceCb_Run32; break;
	}
	
	rec--;
	return(0);
}
