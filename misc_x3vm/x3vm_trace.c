X3VM_Trace *X3VM_OpTrace_1(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_2(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_3(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_4(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_5(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_6(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_7(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_8(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_9(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_10(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_11(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_12(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_13(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_14(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_15(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_16(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_17(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_18(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_19(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_20(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_21(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_22(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	op=ops[21];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_23(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	op=ops[21];	op->Run(ctx, op);
	op=ops[22];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_24(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	ctx->tr_cur=tr;
	ctx->tr_next=tr->tr_next;
	ctx->tr_bra=tr->tr_bra;
	ctx->reg[X3VM_REG_PC]=tr->addr_next;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	op=ops[21];	op->Run(ctx, op);
	op=ops[22];	op->Run(ctx, op);
	op=ops[23];	op->Run(ctx, op);
	return(ctx->tr_next);
}

#if (X3VM_TRACE_MAXOPS>24)

X3VM_Trace *X3VM_OpTrace_25(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_26(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_27(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	op=ops[26];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_28(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	op=ops[26];	op->Run(ctx, op);
	op=ops[27];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_29(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	op=ops[26];	op->Run(ctx, op);
	op=ops[27];	op->Run(ctx, op);
	op=ops[28];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_30(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	op=ops[26];	op->Run(ctx, op);
	op=ops[27];	op->Run(ctx, op);
	op=ops[28];	op->Run(ctx, op);
	op=ops[29];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_31(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	op=ops[26];	op->Run(ctx, op);
	op=ops[27];	op->Run(ctx, op);
	op=ops[28];	op->Run(ctx, op);
	op=ops[29];	op->Run(ctx, op);
	op=ops[30];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_32(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	ops=tr->ops;
	X3VM_OpTrace_24(ctx, tr);
	op=ops[24];	op->Run(ctx, op);
	op=ops[25];	op->Run(ctx, op);
	op=ops[26];	op->Run(ctx, op);
	op=ops[27];	op->Run(ctx, op);
	op=ops[28];	op->Run(ctx, op);
	op=ops[29];	op->Run(ctx, op);
	op=ops[30];	op->Run(ctx, op);
	op=ops[31];	op->Run(ctx, op);
	return(ctx->tr_next);
}

#else

#if (X3VM_TRACE_MAXEXTOPS>X3VM_TRACE_MAXOPS)

X3VM_Trace *X3VM_OpTrace_25(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_26(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_27(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_28(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_29(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_30(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_31(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_32(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_33(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_34(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_35(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_36(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_37(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_38(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_39(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_40(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_32(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_41(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_42(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_43(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_44(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_45(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_46(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	op=ops[21];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_47(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_40(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	op=ops[21];	op->Run(ctx, op);
	op=ops[22];	op->Run(ctx, op);
	return(ctx->tr_next);
}

X3VM_Trace *X3VM_OpTrace_48(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_Opcode **ops;
	X3VM_Opcode *op;
	
	X3VM_OpTrace_24(ctx, tr);
	ops=tr->tr_doext->ops;
	op=ops[0];	op->Run(ctx, op);
	op=ops[1];	op->Run(ctx, op);
	op=ops[2];	op->Run(ctx, op);
	op=ops[3];	op->Run(ctx, op);
	op=ops[4];	op->Run(ctx, op);
	op=ops[5];	op->Run(ctx, op);
	op=ops[6];	op->Run(ctx, op);
	op=ops[7];	op->Run(ctx, op);
	op=ops[8];	op->Run(ctx, op);
	op=ops[9];	op->Run(ctx, op);
	op=ops[10];	op->Run(ctx, op);
	op=ops[11];	op->Run(ctx, op);
	op=ops[12];	op->Run(ctx, op);
	op=ops[13];	op->Run(ctx, op);
	op=ops[14];	op->Run(ctx, op);
	op=ops[15];	op->Run(ctx, op);
	op=ops[16];	op->Run(ctx, op);
	op=ops[17];	op->Run(ctx, op);
	op=ops[18];	op->Run(ctx, op);
	op=ops[19];	op->Run(ctx, op);
	op=ops[20];	op->Run(ctx, op);
	op=ops[21];	op->Run(ctx, op);
	op=ops[22];	op->Run(ctx, op);
	op=ops[23];	op->Run(ctx, op);
	return(ctx->tr_next);
}

#endif
#endif

X3VM_Opcode *X3VM_GetTraceOpIx(X3VM_Context *ctx, X3VM_Trace *tr, int ix)
{
	X3VM_Trace *tre;
	if(ix<X3VM_TRACE_MAXOPS)
		return(tr->ops[ix]);
	if(ix<(2*X3VM_TRACE_MAXOPS))
	{
		tre=tr->tr_doext;
		return(tre->ops[ix-X3VM_TRACE_MAXOPS]);
	}
	if(ix<(3*X3VM_TRACE_MAXOPS))
	{
		tre=tr->tr_doext;
		tre=tre->tr_doext;
		return(tre->ops[ix-2*X3VM_TRACE_MAXOPS]);
	}
	if(ix<(4*X3VM_TRACE_MAXOPS))
	{
		tre=tr->tr_doext;
		tre=tre->tr_doext;
		return(tre->ops[ix-3*X3VM_TRACE_MAXOPS]);
	}
	return(NULL);
}

int X3VM_TraceSetupRun(X3VM_Context *ctx, X3VM_Trace *tr)
{
	static int rec=0;
	X3VM_Opcode *opt;
	int isbra_nonext, usefl;

	ctx->stat_dec_traces++;
	ctx->stat_dec_traceops+=tr->n_ops;
	ctx->stat_dec_tracecnts[tr->n_ops]++;

//	opt=tr->ops[tr->n_ops-1];
	opt=X3VM_GetTraceOpIx(ctx, tr, tr->n_ops-1);

	switch(tr->n_ops)
	{
	case 1:		tr->Run=X3VM_OpTrace_1; break;
	case 2:		tr->Run=X3VM_OpTrace_2; break;
	case 3:		tr->Run=X3VM_OpTrace_3; break;
	case 4:		tr->Run=X3VM_OpTrace_4; break;
	case 5:		tr->Run=X3VM_OpTrace_5; break;
	case 6:		tr->Run=X3VM_OpTrace_6; break;
	case 7:		tr->Run=X3VM_OpTrace_7; break;
	case 8:		tr->Run=X3VM_OpTrace_8; break;
	case 9:		tr->Run=X3VM_OpTrace_9; break;
	case 10:	tr->Run=X3VM_OpTrace_10; break;
	case 11:	tr->Run=X3VM_OpTrace_11; break;
	case 12:	tr->Run=X3VM_OpTrace_12; break;
	case 13:	tr->Run=X3VM_OpTrace_13; break;
	case 14:	tr->Run=X3VM_OpTrace_14; break;
	case 15:	tr->Run=X3VM_OpTrace_15; break;
	case 16:	tr->Run=X3VM_OpTrace_16; break;
	case 17:	tr->Run=X3VM_OpTrace_17; break;
	case 18:	tr->Run=X3VM_OpTrace_18; break;
	case 19:	tr->Run=X3VM_OpTrace_19; break;
	case 20:	tr->Run=X3VM_OpTrace_20; break;
	case 21:	tr->Run=X3VM_OpTrace_21; break;
	case 22:	tr->Run=X3VM_OpTrace_22; break;
	case 23:	tr->Run=X3VM_OpTrace_23; break;
	case 24:	tr->Run=X3VM_OpTrace_24; break;

#if (X3VM_TRACE_MAXEXTOPS>24)
	case 25:	tr->Run=X3VM_OpTrace_25; break;
	case 26:	tr->Run=X3VM_OpTrace_26; break;
	case 27:	tr->Run=X3VM_OpTrace_27; break;
	case 28:	tr->Run=X3VM_OpTrace_28; break;
	case 29:	tr->Run=X3VM_OpTrace_29; break;
	case 30:	tr->Run=X3VM_OpTrace_30; break;
	case 31:	tr->Run=X3VM_OpTrace_31; break;
	case 32:	tr->Run=X3VM_OpTrace_32; break;
#endif

#if (X3VM_TRACE_MAXEXTOPS>32)
	case 33:	tr->Run=X3VM_OpTrace_33; break;
	case 34:	tr->Run=X3VM_OpTrace_34; break;
	case 35:	tr->Run=X3VM_OpTrace_35; break;
	case 36:	tr->Run=X3VM_OpTrace_36; break;
	case 37:	tr->Run=X3VM_OpTrace_37; break;
	case 38:	tr->Run=X3VM_OpTrace_38; break;
	case 39:	tr->Run=X3VM_OpTrace_39; break;
	case 40:	tr->Run=X3VM_OpTrace_40; break;
	case 41:	tr->Run=X3VM_OpTrace_41; break;
	case 42:	tr->Run=X3VM_OpTrace_42; break;
	case 43:	tr->Run=X3VM_OpTrace_43; break;
	case 44:	tr->Run=X3VM_OpTrace_44; break;
	case 45:	tr->Run=X3VM_OpTrace_45; break;
	case 46:	tr->Run=X3VM_OpTrace_46; break;
	case 47:	tr->Run=X3VM_OpTrace_47; break;
	case 48:	tr->Run=X3VM_OpTrace_48; break;
#endif
	}

	tr->tr_next=NULL;
	tr->tr_bra=NULL;
	tr->tr_nobra=NULL;
	
//	if(rec<64)
	if(rec<128)
	{
		rec++;
		
		usefl=0;
		if(rec>=8)
			usefl|=8;

		isbra_nonext=
			((opt->nmid==X3VM_NMID_JAL ) && (opt->rn==0)) ||
			((opt->nmid==X3VM_NMID_JALR) && (opt->rn==0)) ||
			(opt->nmid==X3VM_NMID_INVOP) ;

		if(!isbra_nonext)
		{
			tr->tr_next=X3VM_TraceGetForAddr(ctx, tr->addr_next, usefl);
		}

		if(tr->addr_nobra)
			tr->tr_nobra=X3VM_TraceGetForAddr(ctx, tr->addr_nobra, usefl);
		
		if(	(opt->nmid==X3VM_NMID_JAL ) ||
			(opt->nmid==X3VM_NMID_BTSTT) ||	(opt->nmid==X3VM_NMID_BTSTF) ||
			(opt->nmid==X3VM_NMID_BEQ  ) ||	(opt->nmid==X3VM_NMID_BNE  ) ||
			(opt->nmid==X3VM_NMID_BLT  ) ||	(opt->nmid==X3VM_NMID_BGE  ) ||
			(opt->nmid==X3VM_NMID_BLTU ) ||	(opt->nmid==X3VM_NMID_BGEU ) )
		{
			tr->tr_bra=X3VM_TraceGetForAddr(ctx, opt->addr+opt->imm, usefl);
		}
		
		rec--;
	}

	return(0);
}

int X3VM_DecodeOpcodeXG3PT(X3VM_Context *ctx,
	X3VM_Opcode *op, u64 addr, u32 opw)
{
	X3VM_Opcode *op1;

	op1=X3VM_AllocOpcode(ctx);
	op->subop=op1;
	X3VM_DecodeOpcodeXG3(ctx, op1, addr, opw, 0);
	op->Run=X3VM_Opc_PREDT;
	op->nmid=X3VM_NMID_PREDT;
	op->fmid=X3VM_FMID_NONE;
	return(0);
}

int X3VM_DecodeOpcodeXG3PF(X3VM_Context *ctx,
	X3VM_Opcode *op, u64 addr, u32 opw)
{
	X3VM_Opcode *op1;

	op1=X3VM_AllocOpcode(ctx);
	op->subop=op1;
	X3VM_DecodeOpcodeXG3(ctx, op1, addr, opw, 0);
	op->Run=X3VM_Opc_PREDF;
	op->nmid=X3VM_NMID_PREDF;
	op->fmid=X3VM_FMID_NONE;
	return(0);
}

int X3VM_DecodeOpcode(X3VM_Context *ctx,
	X3VM_Opcode *op, u64 addr)
{
	u32 opw;
	int hasrvc;

	hasrvc=(((ctx->pc_hibits>>2)&63)==2);

	opw=X3VM_MemLoadU32(ctx, addr);
//	op->opw=opw;
	
	if(!opw)
	{
		op->addr=addr;
		op->opw1=opw;
		op->nmid=0;
		op->fmid=0;
		op->Run=X3VM_Opc_INVOP_NONE;
		op->opfl|=X3VM_OPFL_CTRLF;
		return(0);
	}

	if(hasrvc && (opw&3)!=3)
	{
		X3VM_DecodeOpcodeRVC(ctx, op, addr, opw);
		return(0);
	}

	switch(opw&3)
	{
		case 0:
			X3VM_DecodeOpcodeXG3PT(ctx, op, addr, opw);
			break;
		case 1:
			X3VM_DecodeOpcodeXG3PF(ctx, op, addr, opw);
			break;
		case 2:
			X3VM_DecodeOpcodeXG3(ctx, op, addr, opw, 0);
			break;
		case 3:
			X3VM_DecodeOpcodeRV(ctx, op, addr, opw, 0);
			break;
	}
	return(0);
}

X3VM_Trace *X3VM_OpTrace_Lazy(X3VM_Context *ctx, X3VM_Trace *tr)
{
	X3VM_TraceDecodeForAddr(ctx, tr, tr->addr, 0);
	if(tr->n_ops)
		return(tr->Run(ctx, tr));
	return(NULL);
}

int X3VM_TraceDecodeForAddr(X3VM_Context *ctx,
	X3VM_Trace *tr, u64 addr, int usefl)
{
	X3VM_Opcode *t_ops[X3VM_TRACE_MAXEXTOPS+4];
	X3VM_Trace *tre;
	X3VM_Opcode **opa;
	X3VM_Opcode *op;
	u64 addr1, addr_nobra;
	int hasrvc;
	int n;
	int i, j, k;
	
	hasrvc=(((ctx->pc_hibits>>2)&63)==2);
//	hasrvc=0;

	if((addr&2) && !hasrvc)
	{
		__debugbreak();
	}
	
	if(usefl&16)
	{
		tr->Run=X3VM_OpTrace_Lazy;
		tr->n_ops=0;
		return(1);
	}
	
	addr_nobra=0;
//	opa=tr->ops;
	opa=t_ops;
	addr1=addr&0x0000FFFFFFFFFFFEULL;
//	for(n=0; n<X3VM_TRACE_MAXOPS; n++)
	for(n=0; n<X3VM_TRACE_MAXEXTOPS; n++)
	{
		op=X3VM_AllocOpcode(ctx);
		opa[n]=op;
		X3VM_DecodeOpcode(ctx, op, addr1);

#if 1
		if(	(op->nmid==X3VM_NMID_JAL) &&
			(op->fmid==X3VM_FMID_J2RI) &&
			(op->rn==X3VM_REG_ZR))
		{
			op->Run=X3VM_Opc_BRANB_2RI;
			addr1=op->addr+op->imm;
			continue;
		}

		if(	(op->nmid==X3VM_NMID_JAL) &&
			(op->fmid==X3VM_FMID_J2RI) &&
			(op->rn==X3VM_REG_LR) && !addr_nobra)
		{
			addr_nobra=op->addr+4;
			op->Run=X3VM_Opc_BSRNB_2RI;
			addr1=op->addr+op->imm;
			continue;
		}
#endif

#if 1
		if(	((ctx->opt_faststack&1) && (op->rs==X3VM_REG_SP)) ||
			((ctx->opt_faststack&2) && (op->rs==X3VM_REG_GBR)))
//		if((ctx->opt_faststack&1) && (op->rs==X3VM_REG_SP))
		{
			if(op->fmid==X3VM_FMID_LDRI)
			{
				if(op->nmid==X3VM_NMID_LDP)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_LD64P_3RI_FSP;
				}
				if(op->nmid==X3VM_NMID_LD)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_LD64_3RI_FSP;
				}
				if(op->nmid==X3VM_NMID_LW)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_LDS32_3RI_FSP;
				}
				if(op->nmid==X3VM_NMID_LWU)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_LDU32_3RI_FSP;
				}
			}

			if(op->fmid==X3VM_FMID_STRI)
			{
				if(op->nmid==X3VM_NMID_SDP)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_ST64P_3RI_FSP;
				}
				if(op->nmid==X3VM_NMID_SD)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_ST64_3RI_FSP;
				}
				if(op->nmid==X3VM_NMID_SW)
				{
					op->imm<<=op->sc;	op->sc=0;
					op->Run=X3VM_Opc_ST32_3RI_FSP;
				}
			}
		}
#endif

		if(op->opw2)
			{ addr1+=12; }
		else if(op->opw1)
			{ addr1+=8; }
		else
		{
//			if(hasrvc && ((op->opw0&3)!=3))
			if(op->opfl&X3VM_OPFL_OP16)
				{ addr1+=2; }
			else
				{ addr1+=4; }
		}
		
		if(op->opfl&X3VM_OPFL_RN_P)
		{
			if((op->rn&1) || (op->rn<4))
			{
				op->Run=X3VM_Opc_INVOP_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
			}
		}
		
		if(op->opfl&X3VM_OPFL_RS_P)
		{
			if((op->rs&1) || (op->rs<4))
			{
				op->Run=X3VM_Opc_INVOP_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
			}
		}

		if(op->opfl&X3VM_OPFL_RT_P)
		{
			if((op->rt&1) || (op->rt<4))
			{
				op->Run=X3VM_Opc_INVOP_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
			}
		}

		if(op->opfl&1)
		{
			tr->n_ops=n+1;
			tr->addr_next=addr1;
			break;
		}
	}
	if(n>=X3VM_TRACE_MAXEXTOPS)
	{
		tr->n_ops=n;
		tr->addr_next=addr1;
	}
	
	for(i=0; i<X3VM_TRACE_MAXOPS; i++)
		tr->ops[i]=t_ops[i];

	if((tr->n_ops)>X3VM_TRACE_MAXOPS)
	{
		tre=tr->tr_doext;
		if(!tre)
		{
			tre=X3VM_AllocTrace(ctx);
			tr->tr_doext=tre;
		}
		for(i=0; i<X3VM_TRACE_MAXOPS; i++)
			tre->ops[i]=t_ops[X3VM_TRACE_MAXOPS+i];
	}
	
	tr->addr_nobra=addr_nobra;
	
	X3VM_TraceSetupRun(ctx, tr);
	return(0);
}


X3VM_Trace *X3VM_TraceGetForAddr(X3VM_Context *ctx, u64 addr, int fl)
{
	X3VM_Trace *tr;
	u64 addr1;
	int h;
	
	if(!addr)
		return(NULL);

	if(ctx->vas_parent)
		return(X3VM_TraceGetForAddr(ctx->vas_parent, addr, fl));

	addr1=addr&0x0000FFFFFFFFFFFEULL;
	h=(addr>>2)&4095;
	
	tr=ctx->tr_hash[h];
	while(tr)
	{
		if(tr->addr==addr1)
			return(tr);
		tr=tr->h_next;
	}
	
	tr=X3VM_AllocTrace(ctx);
	tr->addr=addr1;
	tr->h_next=ctx->tr_hash[h];
	ctx->tr_hash[h]=tr;
	X3VM_TraceDecodeForAddr(ctx, tr, addr, fl);
	
	return(tr);
}
