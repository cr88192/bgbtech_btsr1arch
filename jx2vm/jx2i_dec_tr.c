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

void BJX2_MemSimTraceIS(BJX2_Context *ctx, BJX2_Trace *tr)
{
	bjx2_addr pc, lpc;
	int i;

	if(ctx->no_memcost)
		return;
	
	lpc=~0;
	for(i=0; i<tr->n_ops; i++)
	{
		pc=tr->ops[i]->pc;
		if((pc>>4)==(lpc>>4))
			continue;
		BJX2_MemSimAddrL1I(ctx, pc);
		lpc=pc;

//		BJX2_MemSimAddrL1I(ctx, tr->ops[i]->pc);
	}
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

	if(tr->jit_inh>0)
	{
		tr->jit_inh--;
		if(tr->jit_inh<=0)
		{
			BJX2_CheckJitTrace(ctx, tr);
		}
	}

//	cur->jit_inh=1024;
//	BJX2_CheckJitTrace(ctx, cur);
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



BJX2_Trace *BJX2_DecTraceCb_Run33(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run34(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run35(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run36(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run37(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run38(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run39(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run40(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run41(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run42(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}



BJX2_Trace *BJX2_DecTraceCb_Run43(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run44(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); ops++;
	(*ops)->Run(ctx, *ops); ops++;		(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run45(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run46(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run47(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run48(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run49(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run50(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run51(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run52(BJX2_Context *ctx, BJX2_Trace *tr)
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




BJX2_Trace *BJX2_DecTraceCb_Run53(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run54(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run55(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run56(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run57(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run58(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run59(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run60(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run61(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run62(BJX2_Context *ctx, BJX2_Trace *tr)
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

BJX2_Trace *BJX2_DecTraceCb_Run63(BJX2_Context *ctx, BJX2_Trace *tr)
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
	(*ops)->Run(ctx, *ops); // ops++;
	return(ctx->tr_rnxt);
}

BJX2_Trace *BJX2_DecTraceCb_Run64(BJX2_Context *ctx, BJX2_Trace *tr)
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
	int sppfx;

	sppfx=0;
	
	if((op2->nmid==BJX2_NMID_PADDF) && (op3->nmid==BJX2_NMID_PADDF))
		sppfx=1;
	if((op2->nmid==BJX2_NMID_PSUBF) && (op3->nmid==BJX2_NMID_PSUBF))
		sppfx=1;
	if((op2->nmid==BJX2_NMID_PMULF) && (op3->nmid==BJX2_NMID_PMULF))
		sppfx=1;
		
	if(op1->fl&BJX2_OPFL_NOWEX)
		return(-1);
	if(op2->fl&BJX2_OPFL_NOWEX)
		return(-1);
	if(op3->fl&BJX2_OPFL_NOWEXSFX)
		return(-1);

	if(op3->fl&BJX2_OPFL_NOWEXSFX3W)
		return(-1);

	if(op1->fl&BJX2_OPFL_NOWEX_FP2)
		return(-1);
	if(op1->fl&BJX2_OPFL_NOWEX_IO2)
		return(-1);

	if(op2->fl&BJX2_OPFL_NOWEX_FP2)
	{
		if(op3->fl&BJX2_OPFL_NOWEX_FP2)
		{
			if(!sppfx)
			{
				return(-1);
			}
		}
	}

	if(ctx->v_wexmd==0xFF)
	{
		if(ctx->wexmd==1)
			return(-1);
	}else
	{
		if(ctx->v_wexmd==1)
			return(-1);
	}

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
	int sppfx;

	sppfx=0;
	
	if((op1->nmid==BJX2_NMID_PADDF) && (op2->nmid==BJX2_NMID_PADDF))
		sppfx=1;
	if((op1->nmid==BJX2_NMID_PSUBF) && (op2->nmid==BJX2_NMID_PSUBF))
		sppfx=1;
	if((op1->nmid==BJX2_NMID_PMULF) && (op2->nmid==BJX2_NMID_PMULF))
		sppfx=1;

	if((op1->nmid==BJX2_NMID_MOV) && (op2->nmid==BJX2_NMID_BSR))
		sppfx=1;
		
	if(op1->fl&BJX2_OPFL_NOWEX)
	{
		if(!sppfx)
		{
			printf("BJX2_CheckWexSanity2W: Invalid Prefix\n");
			return(-1);
		}
	}
	if(op2->fl&BJX2_OPFL_NOWEXSFX)
	{
		if(!sppfx)
		{
			printf("BJX2_CheckWexSanity2W: Invalid Suffix\n");
			return(-1);
		}
	}

	if(op1->fl&BJX2_OPFL_NOWEX_FP2)
	{
		if(op2->fl&BJX2_OPFL_NOWEX_FP2)
		{
			if(!sppfx)
			{
				printf("BJX2_CheckWexSanity2W: Invalid FP Pair\n");
				return(-1);
			}
		}
	}


	if((ctx->wexmd==1) || (ctx->v_wexmd==1))
	{
		if(	(op2->fmid==BJX2_FMID_REGSTREGDISP)	||
			(op2->fmid==BJX2_FMID_REGSTREG)		||
			(op2->fmid==BJX2_FMID_REGSTREG2)	)
		{
			return(-1);
		}
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
	int nc, ncyc, ilo, nbo;
	bjx2_addr pc, npc, jpc;
	bjx2_addr br_addr;
	byte lrseen, r1seen;
	int i, j;
	
//	if((addr&1) || (addr&(~65535)))
	if(addr&1)
	{
		tr->n_ops=0;
		tr->n_nbops=0;
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
		if(!ctx->status)
			{ BJX2_MemTranslateTlb(ctx, addr+(32*8), 4); }
	}

//	if(ctx->status)
	if((ctx->status) && !(tfl&2))
	{
//		__debugbreak();

		tr->n_ops=0;
		tr->n_nbops=0;
		tr->n_cyc=0;
		tr->jit_inh=-1;
//		if(ctx->status==BJX2_FLT_TLBMISS)
		if((ctx->status&0xF000)==0xA000)
			tr->Run=BJX2_DecTraceCb_RunUnpack;
		else
			tr->Run=BJX2_DecTraceCb_Bad;
		return(-1);
	}

//	if(rec>=16)
	if(rec>=32)
	{
		tr->n_ops=0;
		tr->n_nbops=0;
		tr->n_cyc=0;
		tr->jit_inh=-1;
		tr->Run=BJX2_DecTraceCb_RunUnpack;
		return(0);
	}

	rec++;
	
	br_addr=0;
	if(ctx->dbg_setbrk_nz)
	{
		for(i=0; i<256; i++)
		{
			pc=ctx->dbg_setbrk_pc[i];
			if(!pc)
				continue;
			if((pc>=addr) && (pc<(addr+256)))
			{
				if(!br_addr || (pc<br_addr))
					br_addr=pc;
			}
		}
	}
	
//	ctx->v_wexmd=0xFF;
	
	op=NULL;
	ldrl=0; vdrl=0;
	nc=0; ncyc=0;
	pc=addr; npc=0; jpc=0; brk=0;
	while(nc<BJX2_TR_MAXOP)
	{
		if(tfl&4)
		{
			if((nc>0) && !(op->fl&BJX2_OPFL_WEX))
				break;
		}
		
//		if(br_addr && (pc==br_addr))
//			break;

		if(br_addr && (pc==br_addr))
		{
			op=BJX2_ContextAllocOpcode(ctx);

			op->pc=pc;
			op->pc2=pc;
			op->nmid=BJX2_NMID_EMUBREAK;
			op->Run=BJX2_Op_EMUBREAK_None;

			tr->ops[nc++]=op;
		}

//		ctx->trapc=pc;
	
		op=BJX2_ContextAllocOpcode(ctx);
		BJX2_DecodeOpcodeForAddr(ctx, op, pc);
		if(ctx->status)
		{
//			__debugbreak();
			break;
		}

		if(	(op->nmid==BJX2_NMID_NONE) &&
			(op->fl&BJX2_OPFL_JUMBO96))
		{
			if((nc+1)>=BJX2_TR_MAXOP)
				break;

			op1=op->data;
			op2=op1->data;
			tr->ops[nc++]=op1;
			tr->ops[nc++]=op2;
		}else
		{
			tr->ops[nc++]=op;
		}

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

#if 0
		if(op->fl&BJX2_OPFL_OP24)
		{
//			ncyc++;
			pc++;
		}
#endif

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
				(op->nmid!=BJX2_NMID_RTSU) &&
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
		tr->n_nbops=0;
		tr->n_cyc=0;
		tr->jit_inh=-1;
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
	lrseen=0;
	r1seen=0;
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
				
#if 0
			if(!wexmd)
			{
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
			}
#endif

#if 0
			if(op->fl&BJX2_OPFL_NOWEX)
			{
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
			}
#endif

#if 0
			if(op->fl&BJX2_OPFL_NOWEX_FP2)
			{
				if((wexmd!=4) && (wexmd!=5))
				{
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
				}
			}
#endif

#if 1
			if(op->fl&BJX2_OPFL_NOWEX_IO2)
			{
				if((wexmd!=3) && (wexmd!=5))
				{
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
				}
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
			(op->nmid==BJX2_NMID_FMOVH) ||
//			(op->nmid==BJX2_NMID_FMOVD) ||
			(op->nmid==BJX2_NMID_FMOVH) ||
			(op->nmid==BJX2_NMID_MOVX2) ||
			(op->nmid==BJX2_NMID_LDTEX))
		{		
			if((op->fmid==BJX2_FMID_LDREGREG) ||
				(op->fmid==BJX2_FMID_LDDRREGREG) ||
				(op->fmid==BJX2_FMID_LDREGDISPREG) ||
				(op->fmid==BJX2_FMID_LDREGDISP1REG) ||
				(op->fmid==BJX2_FMID_LDREG2REG))
			{
//				if((i+3)>nc)
//					continue;

				op->fl|=BJX2_OPFL_OPPIPE;
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

#if 0
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

#endif

		if(
			(op->nmid==BJX2_NMID_MULS)		||
			(op->nmid==BJX2_NMID_MULU)		||
			(op->nmid==BJX2_NMID_MULSL)		||
			(op->nmid==BJX2_NMID_MULUL)		||
			(op->nmid==BJX2_NMID_MACSL)		||
			(op->nmid==BJX2_NMID_MACUL)		||
			(op->nmid==BJX2_NMID_DMACSL)	||
			(op->nmid==BJX2_NMID_DMACUL)	||
			(op->nmid==BJX2_NMID_RGB5MINMAX)	||
			(op->nmid==BJX2_NMID_RGB5CCENC)		||
			(op->nmid==BJX2_NMID_RGB5CCENC2)	)
		{
			op->fl|=BJX2_OPFL_OPPIPE;
			op->cyc=ilo;
		}

		if(
			(op->nmid==BJX2_NMID_FADD)		||
			(op->nmid==BJX2_NMID_FSUB)		||
			(op->nmid==BJX2_NMID_FMUL)		||
			(op->nmid==BJX2_NMID_PADDF)		||
			(op->nmid==BJX2_NMID_PSUBF)		||
			(op->nmid==BJX2_NMID_PMULF)		||
			(op->nmid==BJX2_NMID_PADDXF)	||
			(op->nmid==BJX2_NMID_PSUBXF)	||
			(op->nmid==BJX2_NMID_PMULXF)	)
		{
			if(op->imm&8)
			{
				op->fl|=BJX2_OPFL_OPPIPE;
				op->cyc=ilo;
			}
		}

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

			(op->nmid==BJX2_NMID_LEAB)		||
			(op->nmid==BJX2_NMID_LEAW)		||
			(op->nmid==BJX2_NMID_LEAD)		||
			(op->nmid==BJX2_NMID_LEAQ)		||

			(op->nmid==BJX2_NMID_FLDCF)		||
			(op->nmid==BJX2_NMID_FLDCH)		||
			(op->nmid==BJX2_NMID_FLDCFH)	||
			(op->nmid==BJX2_NMID_FSTCF)		||
			(op->nmid==BJX2_NMID_FSTCH)		||

			(op->nmid==BJX2_NMID_SHAD)		||
			(op->nmid==BJX2_NMID_SHLD)		||
			(op->nmid==BJX2_NMID_SHADQ)		||
			(op->nmid==BJX2_NMID_SHLDQ)		||
			(op->nmid==BJX2_NMID_SHAR)		||
			(op->nmid==BJX2_NMID_SHLR)		||
			(op->nmid==BJX2_NMID_SHARQ)		||
			(op->nmid==BJX2_NMID_SHLRQ)		||

			(op->nmid==BJX2_NMID_RGB32PCK64)	||
			(op->nmid==BJX2_NMID_RGB32UPCK64)	||
			(op->nmid==BJX2_NMID_RGB5PCK32)		||
			(op->nmid==BJX2_NMID_RGB5UPCK32)	||
			(op->nmid==BJX2_NMID_RGB5PCK64)		||
			(op->nmid==BJX2_NMID_RGB5UPCK64)	||
			(op->nmid==BJX2_NMID_RGBSHR1)		||
			(op->nmid==BJX2_NMID_RGBAVG)		||

#if 1
//			(op->nmid==BJX2_NMID_MOV)		||
			(op->nmid==BJX2_NMID_NOT)		||
			(op->nmid==BJX2_NMID_EXTSB)		||
			(op->nmid==BJX2_NMID_EXTUB)		||
			(op->nmid==BJX2_NMID_EXTSW)		||
			(op->nmid==BJX2_NMID_EXTUW)		||
//			(op->nmid==BJX2_NMID_EXTSL)		||
//			(op->nmid==BJX2_NMID_EXTUL)		||
#endif

			(op->nmid==BJX2_NMID_MULSW)		||
			(op->nmid==BJX2_NMID_MULUW)		||
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
			op->fl|=BJX2_OPFL_OPPIPE;
		
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
			op->fl|=BJX2_OPFL_OPPIPE;
		}

		if(
			(op->nmid==BJX2_NMID_BRA)		||
			(op->nmid==BJX2_NMID_BSR)		||
			(op->nmid==BJX2_NMID_BT)		||
			(op->nmid==BJX2_NMID_BF)		)
		{
			if(i<2)
			{
				op->cyc=8;
			}
		
			if(op->fmid==BJX2_FMID_REG)
			{
				op->cyc=8;

//				if(op->rn==BJX2_REG_R1)
				if(op->rn==BJX2_REG_DHR)
				{
					if(!r1seen)
					{
						op->cyc=2;
					}
				}
			}
		}

		if(
			(op->nmid==BJX2_NMID_CMPEQ)		||
			(op->nmid==BJX2_NMID_CMPGT)		||
			(op->nmid==BJX2_NMID_CMPGE)		||
			(op->nmid==BJX2_NMID_CMPQEQ)	||
			(op->nmid==BJX2_NMID_CMPQGT)	||
			(op->nmid==BJX2_NMID_CMPQGE)	||
			(op->nmid==BJX2_NMID_CMPPEQ)	||
			(op->nmid==BJX2_NMID_CMPPGT)	||
			(op->nmid==BJX2_NMID_CMPXEQ)	||
			(op->nmid==BJX2_NMID_CMPXGT)	||
			(op->nmid==BJX2_NMID_TST)		||
			(op->nmid==BJX2_NMID_TSTQ)		||
			(op->nmid==BJX2_NMID_FCMPEQ)	||
			(op->nmid==BJX2_NMID_FCMPGT)	||
			(op->nmid==BJX2_NMID_ADC)		||
			(op->nmid==BJX2_NMID_SBB)		||
			(op->nmid==BJX2_NMID_BCDADC)	||
			(op->nmid==BJX2_NMID_BCDSBB)	||
			(op->nmid==BJX2_NMID_BCDSBB)	||
			(op->nmid==BJX2_NMID_ROTCLQ)	||
			(op->nmid==BJX2_NMID_ROTCRQ)	)
		{
			if(op1)
			{
				if(
					(op1->nmid==BJX2_NMID_PRED_T)	||
					(op1->nmid==BJX2_NMID_PRED_F)	||
					(op1->nmid==BJX2_NMID_BT)		||
					(op1->nmid==BJX2_NMID_BF)		||
					(op1->nmid==BJX2_NMID_CSELT)	||
					(op1->nmid==BJX2_NMID_MOVT)		||
					(op1->nmid==BJX2_NMID_MOVNT)	||
					(op1->nmid==BJX2_NMID_ADC)		||
					(op1->nmid==BJX2_NMID_SBB)		||
					(op1->nmid==BJX2_NMID_BCDADC)	||
					(op1->nmid==BJX2_NMID_BCDSBB)	||
					(op1->nmid==BJX2_NMID_BCDSBB)	||
					(op1->nmid==BJX2_NMID_ROTCLQ)	||
					(op1->nmid==BJX2_NMID_ROTCRQ)	)
				{
					op->cyc=2;
				}
			}
		}
		

		if(lrseen>0)
			lrseen--;
		if(r1seen>0)
			r1seen--;
		
		if(op->rn==BJX2_REG_LR)
			lrseen=6;
//		if(op->rn==BJX2_REG_R1)
		if(op->rn==BJX2_REG_DHR)
			r1seen=6;
	}
#endif

	nbo=0;
	ncyc=0;
	ilo=0;
	for(i=0; i<nc; i++)
	{
		op=tr->ops[i];
		ncyc+=op->cyc;

		if(!(op->fl&BJX2_OPFL_WEX))
			nbo++;

		if(op->fl&BJX2_OPFL_OPPIPE)
		{
			j=(op->cyc-1);
			if(j<0)j=0;
			ilo+=j;
		}
	}

	if((nc>=BJX2_TR_MAXOP) || (tfl&4))
		{ npc=pc; }

	tr->n_ops=nc;
	tr->n_nbops=nbo;
	tr->n_cyc=ncyc;
	tr->ip_cyc=ilo;
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
		rec--;

//		__debugbreak();

		tr->n_ops=0;
		tr->n_cyc=0;
		tr->jit_inh=-1;
//		if(ctx->status==BJX2_FLT_TLBMISS)
		if((ctx->status&0xF000)==0xA000)
			tr->Run=BJX2_DecTraceCb_RunUnpack;
		else
			tr->Run=BJX2_DecTraceCb_Bad;
		return(-1);
	}

	tr->jit_inh=1024;

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

	case 33: tr->Run=BJX2_DecTraceCb_Run33; break;
	case 34: tr->Run=BJX2_DecTraceCb_Run34; break;
	case 35: tr->Run=BJX2_DecTraceCb_Run35; break;
	case 36: tr->Run=BJX2_DecTraceCb_Run36; break;
	case 37: tr->Run=BJX2_DecTraceCb_Run37; break;
	case 38: tr->Run=BJX2_DecTraceCb_Run38; break;
	case 39: tr->Run=BJX2_DecTraceCb_Run39; break;
	case 40: tr->Run=BJX2_DecTraceCb_Run40; break;
	case 41: tr->Run=BJX2_DecTraceCb_Run41; break;
	case 42: tr->Run=BJX2_DecTraceCb_Run42; break;

	case 43: tr->Run=BJX2_DecTraceCb_Run43; break;
	case 44: tr->Run=BJX2_DecTraceCb_Run44; break;
	case 45: tr->Run=BJX2_DecTraceCb_Run45; break;
	case 46: tr->Run=BJX2_DecTraceCb_Run46; break;
	case 47: tr->Run=BJX2_DecTraceCb_Run47; break;
	case 48: tr->Run=BJX2_DecTraceCb_Run48; break;
	case 49: tr->Run=BJX2_DecTraceCb_Run49; break;
	case 50: tr->Run=BJX2_DecTraceCb_Run50; break;
	case 51: tr->Run=BJX2_DecTraceCb_Run51; break;
	case 52: tr->Run=BJX2_DecTraceCb_Run52; break;

	case 53: tr->Run=BJX2_DecTraceCb_Run53; break;
	case 54: tr->Run=BJX2_DecTraceCb_Run54; break;
	case 55: tr->Run=BJX2_DecTraceCb_Run55; break;
	case 56: tr->Run=BJX2_DecTraceCb_Run56; break;
	case 57: tr->Run=BJX2_DecTraceCb_Run57; break;
	case 58: tr->Run=BJX2_DecTraceCb_Run58; break;
	case 59: tr->Run=BJX2_DecTraceCb_Run59; break;
	case 60: tr->Run=BJX2_DecTraceCb_Run60; break;
	case 61: tr->Run=BJX2_DecTraceCb_Run61; break;
	case 62: tr->Run=BJX2_DecTraceCb_Run62; break;

	case 63: tr->Run=BJX2_DecTraceCb_Run63; break;
	case 64: tr->Run=BJX2_DecTraceCb_Run64; break;
	}
	
	rec--;
	return(0);
}

BJX2_Trace *BJX2_DecTraceCb_RunUnpack(BJX2_Context *ctx, BJX2_Trace *tr)
{
	int i;

	ctx->regs[BJX2_REG_PC]=tr->addr;
	ctx->trapc=tr->addr;
	i=BJX2_DecodeTraceForAddr(ctx, tr, tr->addr, 0);
	if((i>=0) && (tr->Run!=BJX2_DecTraceCb_RunUnpack))
		return(tr->Run(ctx, tr));

	if((ctx->status&0xF000)==0xA000)
		ctx->regs[BJX2_REG_SPC]=tr->addr;

	return(NULL);
}
