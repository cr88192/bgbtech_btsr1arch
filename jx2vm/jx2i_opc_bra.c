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
	int sc, sc1, srb, prc;

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

	ctx->bpr_cnt++;
	if(take)
	{
		if(pr&2)
		{
//			if(srb)
//				ctx->bpr_hit++;
			if(!(pr&4))
				ctx->bpr_hit++;
		}else
		{
			if(pr&4)
				ctx->bpr_hit++;
		}
	}else
	{
		if(pr&2)
		{
//			if(!srb)
//				ctx->bpr_hit++;
			if(pr&4)
				ctx->bpr_hit++;
		}else
		{
			if(!(pr&4))
				ctx->bpr_hit++;
		}
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
#ifdef BJX2_EM_BPRED
	BJX2_Op_BpredUpdateBranch(ctx, op, !(ctx->regs[BJX2_REG_SR]&1));
#endif

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
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
}

void BJX2_Op_BT_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(ctx->regs[BJX2_REG_SR]&1)
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
	}
}

void BJX2_Op_BF_PcReg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	if(!(ctx->regs[BJX2_REG_SR]&1))
	{
		ctx->regs[BJX2_REG_PC]=(op->pc2)+(ctx->regs[op->rn]*2);
		ctx->tr_rnxt=ctx->tr_rjmp;

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
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

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
}

void BJX2_Op_BSR_PcDr(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_LR]=op->pc2;
	ctx->regs[BJX2_REG_PC]=(op->pc2)+
		((bjx2_addr)(ctx->regs[BJX2_REG_DR])*2);
	ctx->tr_rnxt=ctx->tr_rjmp;
//	ctx->tr_rnxt=NULL;
//	BJX2_JumpUpdatePredicted(ctx);

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
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

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
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

		if(!ctx->regs[BJX2_REG_PC])
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
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
#endif

void BJX2_Op_RTS_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->regs[BJX2_REG_PC]=
		ctx->regs[BJX2_REG_LR];
	ctx->tr_rnxt=NULL;

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);

	if(ctx->regs[BJX2_REG_PC]==ctx->tr_cur->addr)
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
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

	if(!ctx->regs[BJX2_REG_PC])
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_INVOP);
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
	if(!ctx->regs[BJX2_REG_LR])
		{ JX2_DBGBREAK }
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

void BJX2_Op_INVIC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->cc_flush|=1;
	ctx->tr_rnxt=NULL;
	BJX2_ThrowFaultStatus(ctx, BJX2_FLT_CCFLUSH);
}

void BJX2_Op_INVDC_Reg(BJX2_Context *ctx, BJX2_Opcode *op)
{
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
	ctx->trapc=op->pc2;
	BJX2_ThrowFaultStatus(ctx, ctx->regs[op->rn]);
}

void BJX2_Op_SYSCALL_None(BJX2_Context *ctx, BJX2_Opcode *op)
{
	ctx->trapc=op->pc2;
	BJX2_ThrowFaultStatus(ctx, 0xE000|(ctx->regs[0]&0xFFF));
}

void BJX2_Op_WEXMD_Imm(BJX2_Context *ctx, BJX2_Opcode *op)
{
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

	default:
		ctx->regs[BJX2_REG_SR]&=~(1<<27);
		ctx->wexmd=0;
		break;
	}
}
