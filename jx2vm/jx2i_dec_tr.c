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
//	ctx->regs[BJX2_REG_PC]=tr->addr_nxt;

//	ctx->tr_rnxt=NULL;
//	ctx->tr_rjmp=NULL;

	if(tr->addr_nxt)
	{
		ctx->regs[BJX2_REG_PC]=tr->addr_nxt;
	}else
	{
		ctx->regs[BJX2_REG_PC]=-1;
		ctx->tr_rnxt=NULL;
	}

//	if(tr->addr_jmp<0x10000)
//	{
//		ctx->tr_rjmp=NULL;
//	}

//	if(tr->addr && !ctx->regs[BJX2_REG_PC])
//		{ JX2_DBGBREAK }

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

BJX2_Trace *BJX2_DecTraceCb_RunUnpack(BJX2_Context *ctx, BJX2_Trace *tr);

BJX2_Trace *BJX2_DecTraceCb_Bad(BJX2_Context *ctx, BJX2_Trace *tr)
{
//	ctx->trapc=op->pc;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INV_MEX);
	return(NULL);
}

int BJX2_CheckWexSanity3W(BJX2_Context *ctx,
	BJX2_Opcode *op1, BJX2_Opcode *op2, BJX2_Opcode *op3)
{
	if(op1->fl&BJX2_OPFL_NOWEX)
		return(-1);
	if(op2->fl&BJX2_OPFL_NOWEX)
		return(-1);
	if(op3->fl&BJX2_OPFL_NOWEXSFX)
		return(-1);

	if(op1->rn!=BJX2_REG_ZZR)
	{
		if((op1->rn==op2->rm) || (op1->rn==op2->ro))
			return(-1);
		if((op1->rn==op3->rm) || (op1->rn==op3->ro))
			return(-1);
	}

	if(op2->rn!=BJX2_REG_ZZR)
	{
//		if((op2->rn==op1->rm) || (op2->rn==op1->ro))
//			return(-1);
		if((op2->rn==op3->rm) || (op2->rn==op3->ro))
			return(-1);
	}

	if(op3->rn!=BJX2_REG_ZZR)
	{
//		if((op3->rn==op1->rm) || (op3->rn==op1->ro))
//			return(-1);
//		if((op3->rn==op2->rm) || (op3->rn==op2->ro))
//			return(-1);
	}

	return(1);
}

int BJX2_CheckWexSanity2W(BJX2_Context *ctx,
	BJX2_Opcode *op1, BJX2_Opcode *op2)
{
	if(op1->fl&BJX2_OPFL_NOWEX)
	{
		printf("BJX2_CheckWexSanity2W: Invalid Prefix\n");
		return(-1);
	}
	if(op2->fl&BJX2_OPFL_NOWEXSFX)
	{
		printf("BJX2_CheckWexSanity2W: Invalid Suffix\n");
		return(-1);
	}

	if(op1->rn!=BJX2_REG_ZZR)
	{
		if((op1->rn==op2->rm) || (op1->rn==op2->ro))
		{
			printf("BJX2_CheckWexSanity2W: Bundle Dependency\n");
			return(-1);
		}
	}

	if(op2->rn!=BJX2_REG_ZZR)
	{
//		if((op2->rn==op1->rm) || (op2->rn==op1->ro))
//			return(-1);
	}

	return(1);
}

int BJX2_DecodeTraceForAddr(BJX2_Context *ctx,
	BJX2_Trace *tr, bjx2_addr addr, int tfl)
{
	static int rec=0;
	BJX2_Opcode *op, *op1, *op2, *op3, *op4;
	int ldrl, vdrl, brk, wexmd;
	int pc, nc, ncyc, npc, jpc, ilo;
	int i, j;
	
//	if((addr&1) || (addr&(~65535)))
	if(addr&1)
	{
		tr->n_ops=0;
		tr->n_cyc=0;
		tr->Run=BJX2_DecTraceCb_Bad;
		return(-1);
	}
	
	if(!(ctx->status))
	{
//		ctx->trapc=addr;
		BJX2_MemTranslateTlb(ctx, addr+ 0, 4);
//		BJX2_MemTranslateTlb(ctx, addr+12);
//		BJX2_MemTranslateTlb(ctx, addr+4096);
		BJX2_MemTranslateTlb(ctx, addr+(32*8), 4);
	}

	if(ctx->status)
	{
//		__debugbreak();

		tr->n_ops=0;
		tr->n_cyc=0;
//		if(ctx->status==BJX2_FLT_TLBMISS)
		if((ctx->status&0xF000)==0xA000)
			tr->Run=BJX2_DecTraceCb_RunUnpack;
		else
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
//		ctx->trapc=pc;
	
		op=BJX2_ContextAllocOpcode(ctx);
		BJX2_DecodeOpcodeForAddr(ctx, op, pc);
		if(ctx->status)
		{
//			__debugbreak();
			break;
		}

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

		if(op->fl&BJX2_OPFL_OP24)
		{
//			ncyc++;
			pc++;
		}

		if(op->fl&BJX2_OPFL_JUMBO64)
		{
//			ncyc+=2;
			pc+=6;
		}

		if(op->fl&BJX2_OPFL_JUMBO96)
		{
//			ncyc+=2;
			pc+=10;
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
	
	if(nc<1)
	{
		tr->n_ops=0;
		tr->n_cyc=0;
		tr->Run=BJX2_DecTraceCb_Bad;
		return(-1);
	}
	
	if(nc>BJX2_TR_MAXOP)
		{ JX2_DBGBREAK }

	if(nc>=BJX2_TR_MAXOP)
	{
		/* Trace should not split a bundle. */
		while(nc>0)
		{
			op=tr->ops[nc-1];
			if(!(op->fl&BJX2_OPFL_WEX))
				break;
			nc--;
			pc=op->pc;
		}
		npc=pc;
	}
	
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
		op3=NULL;
		op4=NULL;
	
		op=tr->ops[i];
		if((i+1)<nc)
			op1=tr->ops[i+1];
		if((i+2)<nc)
			op2=tr->ops[i+2];
		if((i+3)<nc)
			op3=tr->ops[i+3];
		if((i+4)<nc)
			op4=tr->ops[i+4];

		if(op->pc&1)
		{
			if(	(op->fl&BJX2_OPFL_WEX)		||
				(op->fl&BJX2_OPFL_TWOWORD)	||
				(op->fl&BJX2_OPFL_TRIWORD)	||
				(op->fl&BJX2_OPFL_JUMBO64)	||
				(op->fl&BJX2_OPFL_JUMBO96)	)
			{
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
			}				
		}

#if 0
		if(op->nmid==BJX2_NMID_RTSU)
		{
			if((i<6) && !(tfl&1))
			{
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
			}
		}
#endif

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

#if 1
			if(op->fl&BJX2_OPFL_NOWEX)
			{
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
			}
#endif

#if 1
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
#endif
			
			if(op1->nmid==BJX2_NMID_BLINTA)
			{
				op1->data=op;
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
		if(	op1 &&
			(op->rn!=op1->rm) &&
			(op->rn!=op1->rn) &&
			(op->rn!=op1->ro))
		{
			if(	op2 &&
				(op->rn!=op2->rm) &&
				(op->rn!=op2->rn) &&
				(op->rn!=op2->ro))
			{
				if(	op3 &&
					(op->rn!=op3->rm) &&
					(op->rn!=op3->rn) &&
					(op->rn!=op3->ro))
				{
					if(	op4 &&
						(op->rn!=op4->rm) &&
						(op->rn!=op4->rn) &&
						(op->rn!=op4->ro))
					{
						ilo=1;
					}else
					{
						ilo=1;
						if(	(op1->fl&BJX2_OPFL_WEX)	&&
							(op2->fl&BJX2_OPFL_WEX)	)
								{ ilo++; }
						else if(	(op1->fl&BJX2_OPFL_WEX)	&&
									(op3->fl&BJX2_OPFL_WEX)	)
								{ ilo++; }
					}
				}else
				{
					ilo=1;
					if(op1->fl&BJX2_OPFL_WEX)
						ilo++;
					if(op2->fl&BJX2_OPFL_WEX)
						ilo++;
				}
			}else
			{
				ilo=2;
				if(op1->fl&BJX2_OPFL_WEX)
					ilo++;
			}
		}else
		{
			ilo=3;
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
			(op->nmid==BJX2_NMID_FMOVD) ||
			(op->nmid==BJX2_NMID_MOVX2))
		{		
			if((op->fmid==BJX2_FMID_LDREGREG) ||
				(op->fmid==BJX2_FMID_LDDRREGREG) ||
				(op->fmid==BJX2_FMID_LDREGDISPREG) ||
				(op->fmid==BJX2_FMID_LDREG2REG))
			{
//				if((i+3)>nc)
//					continue;

				op->cyc=ilo;

#if 0
				if(	op1 &&
					(op->rn!=op1->rm) &&
					(op->rn!=op1->rn) &&
					(op->rn!=op1->ro))
				{
					if(	op2 &&
						(op->rn!=op2->rm) &&
						(op->rn!=op2->rn) &&
						(op->rn!=op2->ro))
					{
						if(	op3 &&
							(op->rn!=op3->rm) &&
							(op->rn!=op3->rn) &&
							(op->rn!=op3->ro))
						{
							if(	op4 &&
								(op->rn!=op4->rm) &&
								(op->rn!=op4->rn) &&
								(op->rn!=op4->ro))
							{
								op->cyc=1;
							}else
							{
								op->cyc=1;
								if(	(op1->fl&BJX2_OPFL_WEX)	&&
									(op2->fl&BJX2_OPFL_WEX)	)
										{ op->cyc++; }
								else if(	(op1->fl&BJX2_OPFL_WEX)	&&
											(op3->fl&BJX2_OPFL_WEX)	)
										{ op->cyc++; }
							}
						}else
						{
							op->cyc=1;
							if(op1->fl&BJX2_OPFL_WEX)
								op->cyc++;
							if(op2->fl&BJX2_OPFL_WEX)
								op->cyc++;
						}
					}else
					{
						op->cyc=2;
						if(op1->fl&BJX2_OPFL_WEX)
							op->cyc++;
					}
				}else
				{
					op->cyc=3;
				}
#endif

#if 0
				if(	(op->rn!=op1->rm) &&
					(op->rn!=op1->rn) &&
					(op->rn!=op1->ro))
				{
//					op->cyc=1;
					op->cyc=2;
				}else
				{
//					op->cyc=2;
					op->cyc=3;
				}
#endif
			}

			if((op->fmid==BJX2_FMID_REGSTREG) ||
				(op->fmid==BJX2_FMID_REGSTDRREG) ||
				(op->fmid==BJX2_FMID_REGSTREGDISP) ||
				(op->fmid==BJX2_FMID_REGSTREG2))
			{
				op->cyc=1;
//				op->cyc=3;
			}
			
		}

		if((op->nmid==BJX2_NMID_POP) ||
			(op->nmid==BJX2_NMID_FPOP) ||
			(op->nmid==BJX2_NMID_POPX2))
		{
//			op->cyc=1;
			op->cyc=3;
		}

		if((op->nmid==BJX2_NMID_PUSH) ||
			(op->nmid==BJX2_NMID_FPUSH) ||
			(op->nmid==BJX2_NMID_PUSHX2))
		{
//			op->cyc=1;
			op->cyc=4;
		}
#endif

#if 1
		if(
			(op->nmid==BJX2_NMID_ADD)		||
			(op->nmid==BJX2_NMID_SUB)		||
			(op->nmid==BJX2_NMID_AND)		||
			(op->nmid==BJX2_NMID_OR)		||
			(op->nmid==BJX2_NMID_XOR)		||
			(op->nmid==BJX2_NMID_ADDSL)		||
			(op->nmid==BJX2_NMID_SUBSL)		||
			(op->nmid==BJX2_NMID_ADDUL)		||
			(op->nmid==BJX2_NMID_SUBUL)		||
			(op->nmid==BJX2_NMID_PADDW)		||
			(op->nmid==BJX2_NMID_PSUBW)		||
			(op->nmid==BJX2_NMID_PSHUFB)	||
			(op->nmid==BJX2_NMID_PSHUFW)	||
			(op->nmid==BJX2_NMID_PMORTQ)	||
			(op->nmid==BJX2_NMID_PMORTL)	||
			(op->nmid==BJX2_NMID_MOVLD)		||
			(op->nmid==BJX2_NMID_MOVHD)		||
			(op->nmid==BJX2_NMID_MOVHLD)	||
			(op->nmid==BJX2_NMID_MOVLHD))
		{
			j=ilo-1;
			if(j<1)j=1;
			if(j>2)j=2;
			op->cyc=j;
		
#if 0
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
//					op->cyc=3;
				}else
				{
					op->cyc=2;
//					op->cyc=4;
				}
			}
#endif
		}
#endif

		if(
			(op->nmid==BJX2_NMID_BLKUTX1)	||
			(op->nmid==BJX2_NMID_BLKUTX2)	||
			(op->nmid==BJX2_NMID_BLKUTX3H)	||
			(op->nmid==BJX2_NMID_BLKUTX3L)	)
		{
			j=ilo-1;
			if(j<1)j=1;
			if(j>2)j=2;
			op->cyc=j;
		}
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

#if 1
	if(rec<64)
	{
		if(npc && !brk)
			{ tr->lnknext=BJX2_GetTraceForAddr(ctx, pc, 1); }
		if(jpc)
			{ tr->jmpnext=BJX2_GetTraceForAddr(ctx, jpc, 0); }
	}
#endif

	if(ctx->status)
	{
//		__debugbreak();

		tr->n_ops=0;
		tr->n_cyc=0;
//		if(ctx->status==BJX2_FLT_TLBMISS)
		if((ctx->status&0xF000)==0xA000)
			tr->Run=BJX2_DecTraceCb_RunUnpack;
		else
			tr->Run=BJX2_DecTraceCb_Bad;
		return(-1);
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

BJX2_Trace *BJX2_DecTraceCb_RunUnpack(BJX2_Context *ctx, BJX2_Trace *tr)
{
	int i;
	i=BJX2_DecodeTraceForAddr(ctx, tr, tr->addr, 0);
	if(i>=0)
		return(tr->Run(ctx, tr));
	return(NULL);
}
