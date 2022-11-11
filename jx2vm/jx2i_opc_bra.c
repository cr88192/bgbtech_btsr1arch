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

void BJX2_Op_BRA_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}


void BJX2_Op_BSR_RegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
//	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[op->rn]=lr;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

static byte bjx2_bpred_adjt[8]={ 7, 0, 1, 2, 4, 4, 5, 6 };
static byte bjx2_bpred_adjf[8]={ 1, 2, 3, 3, 5, 6, 7, 0 };

// static byte bjx2_bpred_adjt[8]={ 6, 6, 0, 0,  4, 4, 4, 4 };
// static byte bjx2_bpred_adjf[8]={ 2, 2, 2, 2,  6, 6, 0, 0 };

// static byte bjx2_bpred_adjt[8]={ 6, 6, 6, 6,  6, 6, 6, 6 };
// static byte bjx2_bpred_adjf[8]={ 0, 0, 0, 0,  0, 0, 0, 0 };

u16 bjx2_bpred_srbtab[256];

/*
Experiment with genetic algo generation of branch-state tables?...
*/

void BJX2_Op_BpredInitGA(BJX2_Context *ctx)
{
	int i, j, k;

	if(ctx->bpr_gatab_buf)
		return;
//	ctx->bpr_gatab_buf=malloc(256*256);
//	ctx->bpr_ga_adj_buf=malloc(256*(32+32));

	ctx->bpr_gatab_buf=malloc(256*64 + 256*64);
	ctx->bpr_ga_adj_buf=ctx->bpr_gatab_buf+(256*64);
	
	for(i=0; i<256; i++)
	{
		ctx->bpr_gatab[i]=ctx->bpr_gatab_buf+(i*64);
		ctx->bpr_ga_adjt[i]=ctx->bpr_ga_adj_buf+(i*64)+0;
		ctx->bpr_ga_adjf[i]=ctx->bpr_ga_adj_buf+(i*64)+32;
		ctx->bpr_ga_hit[i]=0;
	}

	for(i=0; i<256; i++)
	{
		for(j=0; j<32; j++)
		{
			ctx->bpr_ga_adjt[i][j]=1;
			ctx->bpr_ga_adjf[i][j]=0;
		}

		ctx->bpr_ga_adjt[i][0]=1;
		ctx->bpr_ga_adjt[i][1]=3;
		ctx->bpr_ga_adjt[i][2]=0;
		ctx->bpr_ga_adjt[i][3]=3;

		ctx->bpr_ga_adjf[i][0]=2;
		ctx->bpr_ga_adjf[i][1]=0;
		ctx->bpr_ga_adjf[i][2]=2;
		ctx->bpr_ga_adjf[i][3]=1;
	}

	for(i=8; i<256; i++)
	{
		for(j=0; j<32; j++)
		{
			ctx->bpr_ga_adjt[i][j]=rand()&15;
			ctx->bpr_ga_adjf[i][j]=rand()&15;
		}
	}
}

void BJX2_Op_BpredGaSwap(BJX2_Context *ctx, int i, int j)
{
	byte *p0, *p1, *p2, *p3;
	int i0, i1;
	
	p0=ctx->bpr_ga_adjt[i];
	p1=ctx->bpr_ga_adjt[j];
	p2=ctx->bpr_ga_adjf[i];
	p3=ctx->bpr_ga_adjf[j];
	i0=ctx->bpr_ga_hit[i];
	i1=ctx->bpr_ga_hit[j];
	ctx->bpr_ga_adjt[i]=p1;
	ctx->bpr_ga_adjt[j]=p0;
	ctx->bpr_ga_adjf[i]=p3;
	ctx->bpr_ga_adjf[j]=p2;
	ctx->bpr_ga_hit[i]=i1;
	ctx->bpr_ga_hit[j]=i0;
}

void BJX2_Op_BpredGaBreed(BJX2_Context *ctx, int ia, int ib, int ic)
{
	byte *p0, *p1, *p2, *p3, *p4, *p5;
	int sd;
	int i, j, k;

	p0=ctx->bpr_ga_adjt[ia];
	p1=ctx->bpr_ga_adjf[ia];
	p2=ctx->bpr_ga_adjt[ib];
	p3=ctx->bpr_ga_adjf[ib];
	p4=ctx->bpr_ga_adjt[ic];
	p5=ctx->bpr_ga_adjf[ic];
	
	sd=rand(); sd=sd*65521+1;
	for(i=0; i<32; i++)
	{
		p4[i]=((sd>>16)&1)?p2[i]:p0[i];
		p5[i]=((sd>>16)&2)?p3[i]:p1[i];
		sd=sd*65521+1;
//		if(!((sd>>16)&15))
		if(!((sd>>16)&63))
			{ p4[i]^=1<<((sd>>24)&7); }
		sd=sd*65521+1;
//		if(!((sd>>16)&15))
		if(!((sd>>16)&63))
			{ p5[i]^=1<<((sd>>24)&7); }

		sd=sd*65521+1;
		if(!((sd>>16)&255) && (i>0))
				p4[i]=p4[i-1];
		sd=sd*65521+1;
		if(!((sd>>16)&255) && (i>0))
				p4[i-1]=p4[i];
		sd=sd*65521+1;
		if(!((sd>>16)&255) && (i>0))
			{ k=p4[i]; p4[i]=p4[i-1]; p4[i-1]=k; }

		sd=sd*65521+1;
		if(!((sd>>16)&255) && (i>0))
				p5[i]=p5[i-1];
		sd=sd*65521+1;
		if(!((sd>>16)&255) && (i>0))
				p5[i-1]=p5[i];
		sd=sd*65521+1;
		if(!((sd>>16)&255) && (i>0))
			{ k=p5[i]; p5[i]=p5[i-1]; p5[i-1]=k; }
	}
}

void BJX2_Op_BpredGaDump(BJX2_Context *ctx, int ia)
{
	byte *p0, *p1, *p2, *p3, *p4, *p5;
	int sd, n;
	int i, j, k;

	p0=ctx->bpr_ga_adjt[ia];
	p1=ctx->bpr_ga_adjf[ia];
//	n=32;
	n=16;

	printf("AdjT: ");
	for(i=0; i<n; i++)
		printf("%X ", p0[i]&(n-1));
	printf("\n");

	printf("AdjF: ");
	for(i=0; i<n; i++)
		printf("%X ", p1[i]&(n-1));
	printf("\n");
}

void BJX2_Op_BpredUpdateBranchGA(BJX2_Context *ctx, BJX2_Opcode *op, int take)
{
	int pr, prb, pri, rov;
	int i0, i1;
	int i, j, k;

	BJX2_Op_BpredInitGA(ctx);

	rov=ctx->bpr_ga_rov;
	ctx->bpr_ga_rov=(rov+1)&255;

	pri=((op->pc)>>1)&63;
//	pri=((op->pc)>>1)&255;
	pr=ctx->bpr_gatab[rov][pri];

	ctx->bpr_ga_cnt++;
	
	if(take)
	{
//		if(prb&4)
		if(pr&1)
			ctx->bpr_ga_hit[rov]++;
		pr=ctx->bpr_ga_adjt[rov][pr];
	}else
	{
		if(!(pr&1))
			ctx->bpr_ga_hit[rov]++;
		pr=ctx->bpr_ga_adjf[rov][pr];
	}
//	pr=pr&31;
	pr=pr&15;
	ctx->bpr_gatab[rov][pri]=pr;
	
	if(ctx->bpr_ga_cnt>=(256*16384))
	{
		printf("Bpred Update GA, Gen=%d\n", ctx->bpr_ga_gencnt);
		for(i=0; i<256; i++)
			for(j=i+1; j<256; j++)
		{
			if(ctx->bpr_ga_hit[j] > ctx->bpr_ga_hit[i])
				BJX2_Op_BpredGaSwap(ctx, i, j);
		}
		
		printf("Bpred Top %.2f%%\n",
			(100.0*ctx->bpr_ga_hit[0])/16384.0);
		BJX2_Op_BpredGaDump(ctx, 0);
		
		k=rand(); k=k*65521+1;
		for(i=16; i<256; i++)
		{
			i0=(k>>16)&15; k=k*65521+1;
			i1=(k>>16)&15; k=k*65521+1;
			BJX2_Op_BpredGaBreed(ctx, i0, i1, i);
		}

		for(i=0; i<256; i++)
		{
			memset(ctx->bpr_gatab[i], 0, 64);
			ctx->bpr_ga_hit[i]=0;
		}

		ctx->bpr_ga_cnt=0;
		ctx->bpr_ga_gencnt++;
		
#if 1
//		for(i=0; i<64; i++)
		for(i=0; i<256; i++)
//			printf("%d", bjx2_bpred_srbtab[i]>=0x80);
			printf("%X", (bjx2_bpred_srbtab[i]>>12)&15);
		printf("\n");
#endif
	}
}

#if 0
void BJX2_Op_BpredUpdateBranch(BJX2_Context *ctx, BJX2_Opcode *op, int take)
{
	static u64 srbmsk = 0x513713671B677F7FULL;
	int pr, prb, pri;
	int sc, sc1, srb, prc;

	BJX2_Op_BpredUpdateBranchGA(ctx, op, take);

	pri=((op->pc)>>1)&63;
//	pri=((op->pc)>>1)&255;
	pr=ctx->bpr_tab[pri];

	sc=ctx->bpr_sctab[pri];	
	sc1=(sc<<1)|(take!=0);
	srb=bjx2_bpred_srbtab[sc];
	if(!srb)srb=0x7FFF;

//	srb=((srbmsk>>(63-sc))&1)? 0xFFFF : 0x0001;

	ctx->bpr_cnt++;
	
//	prb=pr;
//	if(op->imm<0)
//		prb=bjx2_bpred_adjt[prb];
//	else
//		prb=bjx2_bpred_adjf[prb];

	if(take)
	{
//		if(prb&4)
		if(srb>=0x8000)
			ctx->bpr_hit++;
		pr=bjx2_bpred_adjt[pr];
		
		if(srb<0xFFFF)srb++;
	}else
	{
//		if(!(prb&4))
		if(srb<0x8000)
			ctx->bpr_hit++;
		pr=bjx2_bpred_adjf[pr];
		if(srb>0x0001)srb--;
	}
	
	bjx2_bpred_srbtab[sc]=srb;
	ctx->bpr_tab[pri]=pr;
//	ctx->bpr_sctab[pri]=sc1&63;
	ctx->bpr_sctab[pri]=sc1&255;
}
#endif

#if 0
void BJX2_Op_BpredUpdateBranch(BJX2_Context *ctx, BJX2_Opcode *op, int take)
{
	int pr, prb, pri, prj;
	int sc, sc1, srb, prc;

	pri=((op->pc)>>1)&63;
//	pri=((op->pc)>>1)&255;

	sc=ctx->bpr_hist;
	sc1=(sc<<1)|(take!=0);
	ctx->bpr_hist=sc1;
	
	pr=ctx->bpr_tab[pri];

	ctx->bpr_cnt++;
	
	if(take)
	{
		if(pr&4)
			ctx->bpr_hit++;
		pr=bjx2_bpred_adjt[pr];
	}else
	{
		if(!(pr&4))
			ctx->bpr_hit++;
		pr=bjx2_bpred_adjf[pr];
	}
	
	ctx->bpr_tab[pri]=pr;
}
#endif

#if 1
void BJX2_Op_BpredUpdateBranch(BJX2_Context *ctx, BJX2_Opcode *op, int take)
{
//	static byte adjt[8]={ 6, 0, 7, 7, 5, 5, 5, 6 };
//	static byte adjf[8]={ 1, 1, 1, 2, 2, 4, 3, 3 };
	static byte adjt[8]={ 6, 0, 7, 7, 5, 5, 4, 6 };
	static byte adjf[8]={ 1, 1, 0, 2, 2, 4, 3, 3 };
	static u64 srbmsk = 0x513713671B677F7FULL;
	int pr, prb, pri, prj;
	int sc, sc1, srb, prc, p;

	pri=((op->pc)>>1)&63;
//	pri=((op->pc)>>1)&255;

	sc=ctx->bpr_hist;
	sc1=(sc<<1)|(take!=0);
	ctx->bpr_hist=sc1;
	
	prj=(pri^sc)&63;
	pr=ctx->bpr_tab[prj];
	prb=take ? adjt[pr] : adjf[pr];
	ctx->bpr_tab[prj]=prb;

	sc=ctx->bpr_sctab[pri];	
	sc1=(sc<<1)|(take!=0);
	ctx->bpr_sctab[pri]=sc1&63;
	srb=(srbmsk>>(63-sc))&1;

	p=0;
	ctx->bpr_cnt++;
	if(take)
	{
		if(pr&2)
		{
//			if(srb)
//				ctx->bpr_hit++;
			if(!(pr&4))
				ctx->bpr_hit++;
			else
				p=1;
		}else
		{
			if(pr&4)
				ctx->bpr_hit++;
			else
				p=1;
		}
	}else
	{
		if(pr&2)
		{
//			if(!srb)
//				ctx->bpr_hit++;
			if(pr&4)
				ctx->bpr_hit++;
			else
				p=1;
		}else
		{
			if(!(pr&4))
				ctx->bpr_hit++;
			else
				p=1;
		}
	}
	
	if(p)
	{
		ctx->miss_cyc_bra+=7;
		if(ctx->tr_cur)
			ctx->tr_cur->acc_pencyc+=7;
	}
}
#endif


void BJX2_Op_BT_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op, (ctx->regs[BJX2_REG_SR]&1));
#endif

	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BSR_PcDisp2(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=(op->pc+4)&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc+4;
	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
	ctx->tr_rnxt=ctx->tr_rjmp;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BF_PcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op, !(ctx->regs[BJX2_REG_SR]&1));
#endif

	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRA_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

void BJX2_Op_BT_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BF_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRAL_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*4);
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSRL_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*4);
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

void BJX2_Op_BTL_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*4);
		ctx->tr_rnxt=ctx->tr_rjmp;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BFL_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*4);
		ctx->tr_rnxt=ctx->tr_rjmp;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
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
	ctx->regs[BJX2_REG_PC]=(op->pc2)+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BJX2_JumpUpdatePredicted(ctx);

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BJX2_JumpUpdatePredicted(ctx);

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

void BJX2_Op_BT_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+
			((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=NULL;
//		BJX2_JumpUpdatePredicted(ctx);

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BF_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+
			((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
		ctx->tr_rnxt=ctx->tr_rjmp;
//		ctx->tr_rnxt=NULL;
//		BJX2_JumpUpdatePredicted(ctx);

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

#if 0
void BJX2_Op_BRA_PcDr4(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr pc;
	
	pc=(op->pc2)+(ctx->regs[BJX2_REG_DR]*32+op->imm*2);
	ctx->regs[BJX2_REG_PC]=pc;
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=BJX2_GetTraceForAddr(ctx, pc);
//	BJX2_JumpUpdatePredicted(ctx);

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_PcDr4(BJX2_Context *ctx, BJX2_Opcode *op)
{
	bjx2_addr pc;
	
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;

	pc=(op->pc2)+(ctx->regs[BJX2_REG_DR]*32+op->imm*2);
//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=pc;
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=BJX2_GetTraceForAddr(ctx, pc);
//	BJX2_JumpUpdatePredicted(ctx);

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
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

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
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

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}
#endif

void BJX2_Op_RTS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=ctx->regs[BJX2_REG_LR];
	sr&=0xFFFFFFFFF3FF000CULL;
	sr|=(lr>>48)&0xFFF3;
	sr|=((lr>>48)&0x000C)<<24;

	ctx->regs[BJX2_REG_PC]=
//		ctx->regs[BJX2_REG_LR];
		lr&0x0000FFFFFFFFFFFFULL;
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);

	if(ctx->regs[BJX2_REG_PC]==ctx->tr_cur->addr)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_RET_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->regs[BJX2_REG_PC]=
//		BJX2_MemGetDWord(ctx,
//			ctx->regs[BJX2_REG_SP]);
//	ctx->regs[BJX2_REG_SP]+=4;

	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_PC]=
		BJX2_MemGetQWord(ctx,
			ctx->regs[BJX2_REG_SP]);
	ctx->regs[BJX2_REG_SP]+=8;

//	ctx->regs[BJX2_REG_PC]=
//		ctx->regs[BJX2_REG_LR];
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BREAK_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=op->pc;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BREAK);
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_SLEEP_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
//	ctx->trapc=op->pc;
//	ctx->regs[BJX2_REG_PC]=op->pc;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_SLEEP);
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_INVOP_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc;
	ctx->regs[BJX2_REG_PC]=op->pc;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	ctx->tr_rnxt=NULL;
}

void BJX2_Op_VSKG_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int va, vb, vc;
	
	va=ctx->regs[op->rm];
	vb=ctx->regs[op->rn];

	vc=((va^ctx->vsk_rng)^(va>>16))&0xFFFF;
	ctx->regs[op->rn]=vc;
}

void BJX2_Op_VSKC_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int va, vb, vc;
	
	va=ctx->regs[op->rm];
	vb=ctx->regs[op->rn];
	vc=((va^ctx->vsk_rng)^(va>>16))&0xFFFF;

	if(vc!=vb)
	{
		ctx->trapc=op->pc;
		ctx->regs[BJX2_REG_PC]=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
		ctx->tr_rnxt=NULL;
	}
}


void BJX2_Op_RTE_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int exc;
	int i, j, k;

	exc=ctx->regs[BJX2_REG_EXSR];

	if((exc&0xF000)==0xA000)
	{
//		printf("RTE\n");
	}

	if(!(ctx->regs[BJX2_REG_SPC]))
	{
		ctx->status=BJX2_FLT_BADPC;
		ctx->tr_rnxt=NULL;
		ctx->regs[BJX2_REG_EXSR]=BJX2_FLT_BADPC;
//		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
//		JX2_DBGBREAK
		return;
	}

	BJX2_FaultLeaveInterrupt(ctx);
//	ctx->regs[BJX2_REG_PC]=
//		ctx->regs[BJX2_REG_LR];
	ctx->tr_rnxt=NULL;

	if(!(ctx->regs[BJX2_REG_PC]))
	{
		ctx->status=9999;
		ctx->tr_rnxt=NULL;
//		BJX2_ThrowFaultStatus(ctx, 0x9999);
//		JX2_DBGBREAK
		return;
	}
	
	if((exc&0xF000)==0xA000)
	{
		for(i=0; i<128; i++)
		{
//			if(i==BJX2_REG_EXSR)
//				continue;
			if(i==BJX2_REG_SPC)
				continue;
			if(ctx->regs[i]!=ctx->ex_regs[i])
				break;
		}

		if(i<128)
		{
			printf("RTE\n");
			BJX2_DbgPrintRegs(ctx);
		}
	}

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BRA_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr, rn, pc1;
	
	rn=ctx->regs[op->rn];
	pc1=rn&0x0000FFFFFFFFFFFFULL;
	
	if(op->rn==BJX2_REG_DHR)
	{
		sr=ctx->regs[BJX2_REG_SR];
		lr=ctx->regs[BJX2_REG_DHR];
		sr&=0xFFFFFFFFF3FF000CULL;
		sr|=(lr>>48)&0xFFF3;
		sr|=((lr>>48)&0x000C)<<24;
		ctx->regs[BJX2_REG_SR]=sr;
	}

	if(pc1&1)
	{
		sr&=0xFFFFFFFFF3FF000CULL;
		sr|=(rn>>48)&0xFFF3;
		sr|=((rn>>48)&0x000C)<<24;
		ctx->regs[BJX2_REG_SR]=sr;
		pc1&=~3;
	}


//	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
	ctx->regs[BJX2_REG_PC]=pc1;
	ctx->tr_rnxt=NULL;
	
//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr, rn, pc1;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

	rn=ctx->regs[op->rn];
	pc1=rn&0x0000FFFFFFFFFFFFULL;

	if(pc1&1)
	{
		sr&=0xFFFFFFFFF3FF000CULL;
		sr|=(rn>>48)&0xFFF3;
		sr|=((rn>>48)&0x000C)<<24;

//		sr^=BJX2_FLAG_SR_RVE;
//		sr&=~BJX2_FLAG_SR_WXE;
//		if(pc1&2)
//			sr|=BJX2_FLAG_SR_WXE;
		ctx->regs[BJX2_REG_SR]=sr;
		pc1&=~3;
	}

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
//	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
	ctx->regs[BJX2_REG_PC]=pc1;
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

void BJX2_Op_BT_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
		ctx->tr_rnxt=NULL;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BF_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
		ctx->tr_rnxt=NULL;

//		if(!ctx->regs[BJX2_REG_PC])
		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}


#if 1
void BJX2_Op_BRAX_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
//	if(op->rn==BJX2_REG_DHR)
	if(0)
	{
		sr=ctx->regs[BJX2_REG_SR];
		lr=ctx->regs[BJX2_REG_DHR];
		sr&=0xFFFFFFFFF3FF000CULL;
		sr|=(lr>>48)&0xFFF3;
		sr|=((lr>>48)&0x000C)<<24;
		ctx->regs[BJX2_REG_SR]=sr;
	}


//	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn+0]&0x0000FFFFFFFFFFFFULL;
	ctx->regs[BJX2_REG_PC_HI]=ctx->regs[op->rn+1]&0x0000FFFFFFFFFFFFULL;
	ctx->tr_rnxt=NULL;
	
//	if(!ctx->regs[BJX2_REG_PC])
//	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
//		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSRX_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr, pc1, pc1h;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

	pc1=ctx->regs[op->rn+0]&0x0000FFFFFFFFFFFFULL;
	pc1h=ctx->regs[op->rn+1]&0x0000FFFFFFFFFFFFULL;

	if(pc1&1)
	{
		sr^=BJX2_FLAG_SR_RVE;
		sr&=~BJX2_FLAG_SR_WXE;
		if(pc1&2)
			sr|=BJX2_FLAG_SR_WXE;
		ctx->regs[BJX2_REG_SR]=sr;
		pc1&=~3;
	}

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
//	ctx->regs[BJX2_REG_PC]=ctx->regs[op->rn];
	ctx->regs[BJX2_REG_PC]=pc1;
	ctx->regs[BJX2_REG_PC_HI]=pc1h;
	ctx->tr_rnxt=NULL;

//	if(!ctx->regs[BJX2_REG_PC])
//	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
//		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
//	if(!ctx->regs[BJX2_REG_LR])
//		{ JX2_DBGBREAK }
}
#endif


void BJX2_Op_BRA_RegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr, pc1;

	sr=ctx->regs[BJX2_REG_SR];
	
	if(op->rn==BJX2_REG_DHR)
	{
		lr=ctx->regs[BJX2_REG_DHR];
		sr&=0xFFFFFFFFF3FF000CULL;
		sr|=(lr>>48)&0xFFF3;
		sr|=((lr>>48)&0x000C)<<24;
		ctx->regs[BJX2_REG_SR]=sr;
	}

	pc1=(ctx->regs[op->rn]+op->imm)&0x0000FFFFFFFFFFFFULL;

	if(pc1&1)
	{
		sr^=BJX2_FLAG_SR_RVE;
		sr&=~BJX2_FLAG_SR_WXE;
		if(pc1&2)
			sr|=BJX2_FLAG_SR_WXE;
		ctx->regs[BJX2_REG_SR]=sr;
		pc1&=~3;
	}

	ctx->regs[BJX2_REG_PC]=pc1;
	ctx->tr_rnxt=NULL;
	
//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_RegRegDisp1(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr, pc1;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

	pc1=(ctx->regs[op->rm]+op->imm)&0x0000FFFFFFFFFFFFULL;

	if(pc1&1)
	{
		sr^=BJX2_FLAG_SR_RVE;
		sr&=~BJX2_FLAG_SR_WXE;
		if(pc1&2)
			sr|=BJX2_FLAG_SR_WXE;
		ctx->regs[BJX2_REG_SR]=sr;
		pc1&=~3;
	}

	ctx->regs[op->rn]=lr;
	ctx->regs[BJX2_REG_PC]=pc1;
	ctx->tr_rnxt=NULL;

	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

void BJX2_Op_INVIC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->status)
		return;
	ctx->cc_flush|=1;
	ctx->tr_rnxt=NULL;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_CCFLUSH);
}

void BJX2_Op_INVDC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->status)
		return;
	ctx->cc_flush|=2;
	ctx->trapc=op->pc2;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_CCFLUSH);
}

void BJX2_Op_TRAP_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc2;
	BJX2_ThrowFaultStatus(ctx, op->imm);
}

void BJX2_Op_TRAP_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	BJX2_Context *pctx, *cctx;
	u64 exc;
	int cvn;
	ctx->trapc=op->pc2;
	
	exc=ctx->regs[op->rn];
	cvn=(exc>>8)&15;
	if(cvn || (ctx->core_id))
	{
		if(cvn==15)
		{
			pctx=ctx;
			if(pctx->ctx_parent)
				pctx=pctx->ctx_parent;
			cctx=pctx=pctx->ctx_child;
			while(cctx)
			{
//				cctx->regs[BJX2_REG_EXSR]=exc;
				cctx->regs[BJX2_REG_TEA]=exc>>16;
				BJX2_ThrowFaultStatus(cctx, exc&0xFFFF);
				cctx=cctx->ctx_next;
			}
//			pctx->regs[BJX2_REG_EXSR]=exc;
			pctx->regs[BJX2_REG_TEA]=exc>>16;
			BJX2_ThrowFaultStatus(pctx, exc&0xFFFF);
			return;
		}

		pctx=ctx;
		if(pctx->ctx_parent)
			pctx=pctx->ctx_parent;
		cctx=pctx=pctx->ctx_child;
		while(cctx)
		{
			if(cctx->core_id==cvn)
			{
//				cctx->regs[BJX2_REG_EXSR]=exc;
				cctx->regs[BJX2_REG_TEA]=exc>>16;
				BJX2_ThrowFaultStatus(cctx, exc&0xFFFF);
				return;
			}
			cctx=cctx->ctx_next;
		}
//		pctx->regs[BJX2_REG_EXSR]=exc;
		pctx->regs[BJX2_REG_TEA]=exc>>16;
		BJX2_ThrowFaultStatus(pctx, exc&0xFFFF);
		return;
	}
	
	if(exc>>16)
	{
//		ctx->regs[BJX2_REG_EXSR]=exc;
		ctx->regs[BJX2_REG_TEA]=exc>>16;
		BJX2_ThrowFaultStatus(ctx, exc&0xFFFF);
	}else
	{
		BJX2_ThrowFaultStatus(ctx, exc);
	}
}

void BJX2_Op_SYSCALL_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc2;
	BJX2_ThrowFaultStatus(ctx, 0xE000|(ctx->regs[BJX2_REG_DLR]&0xFFF));
}

void BJX2_AdvanceRng(BJX2_Context *ctx, u32 val)
{
	ctx->hw_rng[0] += val + 1;
	ctx->hw_rng[1] += ctx->hw_rng[0]>>48;
	ctx->hw_rng[2] += ctx->hw_rng[1]>>48;
	ctx->hw_rng[3] += ctx->hw_rng[2]>>48;
	ctx->hw_rng[0] += ctx->hw_rng[3]>>48;
	ctx->hw_rng[0] *= 0x0000FC4BA2F7ACABULL;
	ctx->hw_rng[1] *= 0x0000F7A83CF9E588ULL;
	ctx->hw_rng[2] *= 0x0000FC4BA2F7ACABULL;
	ctx->hw_rng[3] *= 0x0000F7A83CF9E588ULL;
}

void BJX2_AdvanceRngB(BJX2_Context *ctx, u32 val)
{
	BJX2_AdvanceRng(ctx, val);
	BJX2_AdvanceRng(ctx, 0);
	BJX2_AdvanceRng(ctx, 0);
	BJX2_AdvanceRng(ctx, 0);
}

void BJX2_Op_WEXMD_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;

	ctx->wexmd=op->imm;

	switch(ctx->wexmd)
	{
	case 0:
		ctx->regs[BJX2_REG_SR]&=~(1<<27);
		break;
	case 1:
	case 2:
		ctx->regs[BJX2_REG_SR]|=(1<<27);
		break;

	case 31:
		ctx->regs[BJX2_REG_SR]&=~(1<<27);
		BJX2_AdvanceRngB(ctx, ctx->tot_cyc);
		v=	((ctx->hw_rng[0]>>48)<<48)	|
			((ctx->hw_rng[1]>>48)<<32)	|
			((ctx->hw_rng[2]>>48)<<16)	|
			((ctx->hw_rng[3]>>48)<< 0)	;
		ctx->vsk_rng=(v+(v>>16)+(v>>32)+(v>>48))&0xFFFF;
		break;

	default:
		ctx->regs[BJX2_REG_SR]&=~(1<<27);
		ctx->wexmd=0;
		break;
	}
}

u64 BJX2_GetCpuidVal(BJX2_Context *ctx, int ix)
{
	char tb[16];
	int fflags;
	u64 v;

//	fflags=0x01D9FF00;
	fflags=0x77D9FF00;

	switch(ix)
	{
	case 0:
		tb[0]='B';	tb[1]='J';
		tb[2]='X';	tb[3]='2';
		tb[4]='F';	tb[5]='0';
		tb[6]='0';	tb[7]='0';
		
		tb[4]='E';
		if(fflags&(1<<15))
		{
			tb[4]='F';
			if(fflags&(1<<11))
			{
				tb[4]='C';
				if((fflags&(1<<12)) && (fflags&(1<<9)))
				{
					tb[4]='A';
					if(fflags&(1<<26))
					{
						tb[4]='G';
					}
				}
			}
		}
		
		v=BJX2_PtrGetSQWordOfsLe(tb, 0);
		break;
	case 1:
		v=(ctx->core_id)|fflags;
		break;
	
	case 31:
		BJX2_AdvanceRngB(ctx, ctx->tot_cyc);
		v=	((ctx->hw_rng[0]>>48)<<48)	|
			((ctx->hw_rng[1]>>48)<<32)	|
			((ctx->hw_rng[2]>>48)<<16)	|
			((ctx->hw_rng[3]>>48)<< 0)	;
		break;
	default:
		v=0;
		break;
	}
	
	return(v);
}

void BJX2_Op_CPUID_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v;
	v=BJX2_GetCpuidVal(ctx, op->imm);
	ctx->regs[BJX2_REG_DLR]=v;
}


void BJX2_Op_LDTLB_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	s64 addr;
	u64	r0, r1, r2, r3;
	int i, j, h;

	BJX2_MemFlushPPA(ctx);

	ctx->mem_tlb_pr0_hi=0;
	ctx->mem_tlb_pr0_hx=0;
	ctx->mem_tlb_pr0_lo=0;
	ctx->mem_tlb_pr1_hi=0;
	ctx->mem_tlb_pr1_hx=0;
	ctx->mem_tlb_pr1_lo=0;

	r0=ctx->regs[BJX2_REG_DLR];
	r1=ctx->regs[BJX2_REG_DHR];
	addr=r1&0x0000FFFFFFFFF000ULL;

	if((r0&3)==2)
	{
		ctx->mem_ldtlb_lox=r0;
		ctx->mem_ldtlb_hix=r1;
		return;
	}

	if(ctx->regs[BJX2_REG_MMCR]&32)
	{
//		h=((addr>>24)&0x3F)^((addr>>16)&0xFF)^((addr>>8)&0xC0);
		h=(addr>>14)&0xFF;
	}else
		if(ctx->regs[BJX2_REG_MMCR]&16)
	{
//		h=((addr>>22)&1)^((addr>>16)&1);
//		h=((addr>>22)&3)^((addr>>16)&3);
//		h=((addr>>22)&7)^((addr>>16)&7);
//		h=((addr>>22)&15)^((addr>>16)&15);
//		h=((addr>>22)&31)^((addr>>16)&31);
//		h=((addr>>22)&63)^((addr>>16)&63);
//		h=((addr>>24)&127)^((addr>>16)&127);
//		h=((addr>>24)&255)^((addr>>16)&255);
		h=(addr>>16)&0xFF;
	}else
	{
//		h=((addr>>12)&63)^((addr>>16)&63);
//		h=((addr>>12)&255)^((addr>>16)&255);
//		h=((addr>>12)&127)^((addr>>16)&127);
//		h=((addr>>12)&63)^((addr>>16)&63);
//		h=((addr>>12)&31)^((addr>>16)&31);
//		h=((addr>>12)&15)^((addr>>16)&15);
//		h=((addr>>12)& 7)^((addr>>16)& 7);

//		h=((addr>>24)&0x0F)^((addr>>16)&0xFF)^((addr>>8)&0xF0);
		h=(addr>>12)&0xFF;
	}
	
	if(ctx->mem_ldtlb_lox)
//	if(0)
	{
		r0=ctx->mem_tlb_lo[h*4+0];
		r1=ctx->mem_tlb_hi[h*4+0];
		r2=ctx->mem_tlb_lo[h*4+1];
		r3=ctx->mem_tlb_hi[h*4+1];
		ctx->mem_tlb_lo[h*4+2]=r0;
		ctx->mem_tlb_hi[h*4+2]=r1;
		ctx->mem_tlb_lo[h*4+3]=r2;
		ctx->mem_tlb_hi[h*4+3]=r3;

		ctx->mem_tlb_lo[h*4+0]=r0;
		ctx->mem_tlb_hi[h*4+0]=r1;
		ctx->mem_tlb_lo[h*4+1]=ctx->mem_ldtlb_lox;
		ctx->mem_tlb_hi[h*4+1]=ctx->mem_ldtlb_hix;

		ctx->mem_ldtlb_lox=0;
		ctx->mem_ldtlb_hix=0;
		return;
	}

	for(i=3; i>0; i--)
	{
		j=i-1;
		r2=ctx->mem_tlb_lo[h*4+j];
		r3=ctx->mem_tlb_hi[h*4+j];
		ctx->mem_tlb_lo[h*4+i]=r2;
		ctx->mem_tlb_hi[h*4+i]=r3;
	}
	ctx->mem_tlb_lo[h*4+0]=r0;
	ctx->mem_tlb_hi[h*4+0]=r1;
	
//	printf("LDTLB H=%016llX L=%016llX\n", r1, r0);
}

void BJX2_Op_LDACL_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->mem_tlb_acl[3]=ctx->mem_tlb_acl[2];
	ctx->mem_tlb_acl[2]=ctx->mem_tlb_acl[1];
	ctx->mem_tlb_acl[1]=ctx->mem_tlb_acl[0];
	ctx->mem_tlb_acl[0]=ctx->regs[BJX2_REG_DLR];
}

void BJX2_Op_INVTLB_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	int i, j, k;

	ctx->mem_tlb_pr0_hi=0;
	ctx->mem_tlb_pr0_hx=0;
	ctx->mem_tlb_pr0_lo=0;
	ctx->mem_tlb_pr1_hi=0;
	ctx->mem_tlb_pr1_hx=0;
	ctx->mem_tlb_pr1_lo=0;

	ctx->mem_tlb_acl[3]=0;
	ctx->mem_tlb_acl[2]=0;
	ctx->mem_tlb_acl[1]=0;
	ctx->mem_tlb_acl[0]=0;
	
	for(i=0; i<256; i++)
	{
		for(j=0; j<4; j++)
		{
			ctx->mem_tlb_lo[i*4+j]=0;
			ctx->mem_tlb_hi[i*4+j]=0;
		}
	}
}

void BJX2_Op_SXENTR_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	sr&=~(0x0000000070000000ULL);
	sr|= (0x0000000010000000ULL);
	ctx->regs[BJX2_REG_SR]=sr;
}

void BJX2_Op_SUENTR_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	sr&=~(0x0000000070000000ULL);
	ctx->regs[BJX2_REG_SR]=sr;
}

void BJX2_Op_SVENTR_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	sr&=~(0x0000000070000000ULL);
	sr|= (0x0000000040000000ULL);
	ctx->regs[BJX2_REG_SR]=sr;
}

void BJX2_Op_SVEKRR_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sk0, sk1, krr0;
	u64 sk2, sk3;

	sk0=ctx->krr_key[0];
	sk1=ctx->krr_key[1];
	krr0=ctx->regs[BJX2_REG_KRR];
	
	if((sk0&15) || (sk1&15))
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVKRR);
		return;
	}

	sk2=krr0^sk0;
	sk3=krr0^sk1;
	ctx->regs[BJX2_REG_DLR]=sk2;
	ctx->regs[BJX2_REG_DHR]=sk3;
}

void BJX2_Op_LDEKRR_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sk0, sk1, krr0;
	u64 sk2, sk3;
	u64 sk4, sk5;

	sk0=ctx->krr_key[0];
	sk1=ctx->krr_key[1];
	sk2=ctx->regs[BJX2_REG_DLR];
	sk3=ctx->regs[BJX2_REG_DHR];

	if((sk0&15) || (sk1&15))
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVKRR);
		return;
	}

	sk4=sk2^sk0;
	sk5=sk3^sk1;

	if(sk4!=sk5)
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVKRR);
		return;
	}

	ctx->regs[BJX2_REG_KRR]=sk4;
}

void BJX2_Op_LDEENC_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 sk0, sk1, krr0;
	u64 sk2, sk3;

	sk0=ctx->krr_key[0];
	sk1=ctx->krr_key[1];
	krr0=ctx->regs[BJX2_REG_DLR];
	
	if((sk0&15) || (sk1&15))
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVKRR);
		return;
	}

	sk2=krr0^sk0;
	sk3=krr0^sk1;
	ctx->regs[BJX2_REG_DLR]=sk2;
	ctx->regs[BJX2_REG_DHR]=sk3;
}

void BJX2_Op_LDEKEY_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->krr_key[0]=ctx->regs[BJX2_REG_DLR];
	ctx->krr_key[1]=ctx->regs[BJX2_REG_DHR];
}

void BJX2_Op_BRA_Abs(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=op->imm;
	ctx->tr_rnxt=ctx->tr_rjmp;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
}

void BJX2_Op_BSR_Abs(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 lr, sr;
	
	sr=ctx->regs[BJX2_REG_SR];
	lr=op->pc2&0x0000FFFFFFFFFFFFULL;
	lr|=((u64)((sr&0xFFF3)|((sr>>24)&0x000C)))<<48;

//	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_LR]=lr;
	ctx->regs[BJX2_REG_PC]=op->imm;
	ctx->tr_rnxt=ctx->tr_rjmp;

//	if(!ctx->regs[BJX2_REG_PC])
	if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}


void BJX2_Op_BREQ_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(ctx->regs[op->rn]==ctx->regs[op->rm]));
#endif

	if(ctx->regs[op->rn]==ctx->regs[op->rm])
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRNE_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(ctx->regs[op->rn]!=ctx->regs[op->rm]));
#endif

	if(ctx->regs[op->rn]!=ctx->regs[op->rm])
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRLT_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s64)ctx->regs[op->rn])<((s64)ctx->regs[op->rm])));
#endif

	if(((s64)ctx->regs[op->rn])<((s64)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRGE_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s64)ctx->regs[op->rn])>=((s64)ctx->regs[op->rm])));
#endif

	if(((s64)ctx->regs[op->rn])>=((s64)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRGT_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s64)ctx->regs[op->rn])>((s64)ctx->regs[op->rm])));
#endif

	if(((s64)ctx->regs[op->rn])>((s64)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRLE_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s64)ctx->regs[op->rn])<=((s64)ctx->regs[op->rm])));
#endif

	if(((s64)ctx->regs[op->rn])<=((s64)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRBI_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((u64)ctx->regs[op->rn])<((u64)ctx->regs[op->rm])));
#endif

	if(((u64)ctx->regs[op->rn])<((u64)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRHE_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((u64)ctx->regs[op->rn])>=((u64)ctx->regs[op->rm])));
#endif

	if(((u64)ctx->regs[op->rn])>=((u64)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRTSTT_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[op->rm]&ctx->regs[op->rn]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRTSTF_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if( (ctx->regs[op->rm]&ctx->regs[op->rn]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}


void BJX2_Op_BREQL_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s32)ctx->regs[op->rn])==((s32)ctx->regs[op->rm])));
#endif

	if(((s32)ctx->regs[op->rn])==((s32)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRNEL_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s32)ctx->regs[op->rn])!=((s32)ctx->regs[op->rm])));
#endif

	if(((s32)ctx->regs[op->rn])!=((s32)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRLTL_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s32)ctx->regs[op->rn])<((s32)ctx->regs[op->rm])));
#endif

	if(((s32)ctx->regs[op->rn])<((s32)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRGEL_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s32)ctx->regs[op->rn])>=((s32)ctx->regs[op->rm])));
#endif

	if(((s32)ctx->regs[op->rn])>=((s32)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRGTL_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s32)ctx->regs[op->rn])>((s32)ctx->regs[op->rm])));
#endif

	if(((s32)ctx->regs[op->rn])>((s32)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}

void BJX2_Op_BRLEL_RegRegPcDisp(BJX2_Context *ctx, BJX2_Opcode *op)
{
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op,
		(((s32)ctx->regs[op->rn])<=((s32)ctx->regs[op->rm])));
#endif

	if(((s32)ctx->regs[op->rn])<=((s32)ctx->regs[op->rm]))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(op->imm*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if((op->pc2>0x10000) && (ctx->regs[BJX2_REG_PC]<0x10000))
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BADPC);
	}
}



u64 BJX2_RVI_GetCSR(BJX2_Context *ctx, int csr)
{
	u64 v;

	v=0;

	if((csr>=0x7C0) && (csr<=0x7FF))
	{
		v=ctx->regs[BJX2_REG_PC+(csr-0x7C0)];
	}

	if((csr>=0xBC0) && (csr<=0xBFF))
	{
		v=ctx->regs[BJX2_REG_R0+(csr-0xBC0)];
	}

	if((csr>=0xFC0) && (csr<=0xFFF))
	{
		v=BJX2_GetCpuidVal(ctx, (csr-0xFC0));
	}
	
	return(v);
}

void BJX2_RVI_SetCSR(BJX2_Context *ctx, int csr, u64 val)
{
	if((csr>=0x7C0) && (csr<=0x7FF))
	{
		ctx->regs[BJX2_REG_PC+(csr-0x7C0)]=val;
	}

	if((csr>=0xBC0) && (csr<=0xBFF))
	{
		ctx->regs[BJX2_REG_R0+(csr-0xBC0)]=val;
	}

	if((csr>=0xFC0) && (csr<=0xFFF))
	{
//		v=BJX2_GetCpuidVal(ctx, (csr-0xFC0));
	}
}

void BJX2_Op_CSRRW_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=ctx->regs[op->rm];
	v1=BJX2_RVI_GetCSR(ctx, op->imm);
	if(op->rm!=BJX2_REG_ZZR)
		BJX2_RVI_SetCSR(ctx, op->imm, v0);
	if(op->rn!=BJX2_REG_ZZR)
		ctx->regs[op->rn]=v1;
}

void BJX2_Op_CSRRS_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=ctx->regs[op->rm];
	v1=BJX2_RVI_GetCSR(ctx, op->imm);
	v2=v0|v1;
	if(op->rm!=BJX2_REG_ZZR)
		BJX2_RVI_SetCSR(ctx, op->imm, v2);
	if(op->rn!=BJX2_REG_ZZR)
		ctx->regs[op->rn]=v1;
}

void BJX2_Op_CSRRC_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=ctx->regs[op->rm];
	v1=BJX2_RVI_GetCSR(ctx, op->imm);
	v2=v0&(~v1);
	if(op->rm!=BJX2_REG_ZZR)
		BJX2_RVI_SetCSR(ctx, op->imm, v2);
	if(op->rn!=BJX2_REG_ZZR)
		ctx->regs[op->rn]=v1;
}


void BJX2_Op_CSRRWI_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=op->rm;
	v1=BJX2_RVI_GetCSR(ctx, op->imm);
	BJX2_RVI_SetCSR(ctx, op->imm, v0);
	if(op->rn!=BJX2_REG_ZZR)
		ctx->regs[op->rn]=v1;
}

void BJX2_Op_CSRRSI_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=op->rm;
	v1=BJX2_RVI_GetCSR(ctx, op->imm);
	v2=v0|v1;
	if(op->rm!=0)
		BJX2_RVI_SetCSR(ctx, op->imm, v2);
	if(op->rn!=BJX2_REG_ZZR)
		ctx->regs[op->rn]=v1;
}

void BJX2_Op_CSRRCI_RegImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	v0=op->rm;
	v1=BJX2_RVI_GetCSR(ctx, op->imm);
	v2=v0&(~v1);
	if(op->rm!=0)
		BJX2_RVI_SetCSR(ctx, op->imm, v2);
	if(op->rn!=BJX2_REG_ZZR)
		ctx->regs[op->rn]=v1;
}


int BJX2_OpI_BNDCHK(u64 v0, u64 v1)
{
	u64 v2, v2bi;
	byte bnd;

	if((v0>>60)==0)
	{
		if(v0>>48)
			return(1);
		return(0);
	}

	if((v0>>60)==3)
	{
		bnd=(v0>>48)&255;
		if(bnd<0x10)
		{
			v2=bnd;
			v2bi=((v0>>56)&15);
		}
		else
		{
			v2=(8|(bnd&7))<<((bnd>>3)-1);
			v2bi=((v0>>56)&15)<<((bnd>>3)-1);
		}
		
//		return(v1>=v2);
		return((v1+v2bi)>=v2);
	}
	
	if((v0>>60)==15)
	{
		return(0);
	}

	if((v0>>60)==2)
	{
		bnd=(v0>>48)&255;
		v2=bnd;
		return(v1>=v2);
	}

	return(1);
}

void BJX2_Op_BNDCHKB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*1);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCHKW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*2);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCHKL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*4);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCHKQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*8);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCMPB_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}

void BJX2_Op_BNDCMPW_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*2);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}

void BJX2_Op_BNDCMPL_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*4);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}

void BJX2_Op_BNDCMPQ_RegReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=ctx->regs[op->rm];
	
	i=BJX2_OpI_BNDCHK(v0, v1*8);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}


void BJX2_Op_BNDCHKB_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*1);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCHKW_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*2);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCHKL_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*4);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCHKQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*8);
	if(i)
	{
		ctx->trapc=op->pc;
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
}

void BJX2_Op_BNDCMPB_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}

void BJX2_Op_BNDCMPW_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*2);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}

void BJX2_Op_BNDCMPL_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*4);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}

void BJX2_Op_BNDCMPQ_ImmReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	u64 v0, v1, v2;
	int i;

	v0=ctx->regs[op->rn];
	v1=op->imm;
	
	i=BJX2_OpI_BNDCHK(v0, v1*8);

	if(i)
		{ ctx->regs[BJX2_REG_SR]&=~1; }
	else
		{ ctx->regs[BJX2_REG_SR]|=1; }
}
