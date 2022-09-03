VMX86_Trace *VMX86_TraceOp_Run1(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	op=tr->ops[0];
	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run2(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run3(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run4(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run5(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}
VMX86_Trace *VMX86_TraceOp_Run6(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run7(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}


VMX86_Trace *VMX86_TraceOp_Run8(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run9(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}
VMX86_Trace *VMX86_TraceOp_Run10(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run11(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;

	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}


VMX86_Trace *VMX86_TraceOp_Run12(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run13(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}
VMX86_Trace *VMX86_TraceOp_Run14(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run15(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;

	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}

VMX86_Trace *VMX86_TraceOp_Run16(VMX86_Context *ctx, VMX86_Trace *tr)
{
	VMX86_Opcode **ops;
	VMX86_Opcode *op;
	ctx->tr_next=tr->tr_next;
	ops=tr->ops;
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	op=*ops++;	op->Run(ctx, op);	op=*ops++;	op->Run(ctx, op);
	return(ctx->tr_next);
}
