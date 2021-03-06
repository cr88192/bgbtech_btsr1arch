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

BJX2_Context *BJX2_AllocContext(void)
{
	BJX2_Context *tmp;
	
	tmp=malloc(sizeof(BJX2_Context));
	memset(tmp, 0, sizeof(BJX2_Context));
	return(tmp);
}

BJX2_Context *BJX2_CreateSubContext(BJX2_Context *pctx)
{
	BJX2_Context *tmp;
	int i, j, k;
	
	tmp=BJX2_AllocContext();
	
	tmp->ctx_parent=pctx;
	tmp->ctx_next=pctx->ctx_child;
	pctx->ctx_child=tmp;
	
	for(i=0; i<32; i++)
	{
		tmp->span[i]=pctx->span[i];
	}
	
	tmp->n_span=pctx->n_span;
	
	return(tmp);
}

BJX2_Opcode *BJX2_ContextAllocOpcode(BJX2_Context *ctx)
{
	BJX2_Opcode *tmp;
	int i;
	
	tmp=ctx->free_op;
	if(tmp)
	{
		ctx->free_op=*(BJX2_Opcode **)tmp;
		memset(tmp, 0, sizeof(BJX2_Opcode));
		return(tmp);
	}
	
	tmp=malloc(256*sizeof(BJX2_Opcode));
	for(i=0; i<255; i++)
	{
		*(BJX2_Opcode **)tmp=ctx->free_op;
		ctx->free_op=tmp;
		tmp++;
	}

	memset(tmp, 0, sizeof(BJX2_Opcode));
	return(tmp);
}

BJX2_Trace *BJX2_ContextAllocTrace(BJX2_Context *ctx)
{
	BJX2_Trace *tmp;
	int i;
	
	tmp=ctx->free_tr;
	if(tmp)
	{
		ctx->free_tr=*(BJX2_Trace **)tmp;
		memset(tmp, 0, sizeof(BJX2_Trace));
		return(tmp);
	}
	
	tmp=malloc(256*sizeof(BJX2_Trace));
	for(i=0; i<255; i++)
	{
		*(BJX2_Trace **)tmp=ctx->free_tr;
		ctx->free_tr=tmp;
		tmp++;
	}

	memset(tmp, 0, sizeof(BJX2_Trace));
	return(tmp);
}

void BJX2_ContextFreeOpcode(BJX2_Context *ctx, BJX2_Opcode *tmp)
{
	tmp->Run=NULL;
	*(BJX2_Opcode **)tmp=ctx->free_op;
	ctx->free_op=tmp;
}

void BJX2_ContextFreeTrace(BJX2_Context *ctx, BJX2_Trace *tmp)
{
	tmp->Run=NULL;
	*(BJX2_Trace **)tmp=ctx->free_tr;
	ctx->free_tr=tmp;
}

BJX2_Trace *BJX2_GetTraceForAddr(BJX2_Context *ctx, bjx2_addr addr)
{
	BJX2_Trace *cur, *prv, *tmp;
	int i, h;

#if 0
	h=((addr*65521)>>16)&1023;
	cur=ctx->trhash[h];
	while(cur)
	{
		if(cur->addr==addr)
			return(cur);
		cur=cur->hnext;
	}
#endif

#if 1
	cur=ctx->prttr;
	if(cur && (cur->addr==addr))
		return(cur);
#endif

#if 1
	h=((addr*65521)>>16)&1023;

#if 1
	cur=ctx->rttr[h&63];
	if(cur && (cur->addr==addr))
		return(cur);
#endif

	cur=ctx->trhash[h];
	if(cur)
	{
		if(cur->addr==addr)
			return(cur);

		if(cur->hnext)
		{
			cur=cur->hnext;
			if(cur->addr==addr)
				return(cur);
		}

		cur=ctx->trhash[h];
			
		prv=cur;
		cur=cur->hnext;
		while(cur)
		{
			if(cur->addr==addr)
			{
				prv->hnext=cur->hnext;
				cur->hnext=ctx->trhash[h];
				ctx->trhash[h]=cur;
				return(cur);
			}
			
			prv=cur;
			cur=cur->hnext;
		}
	}
#endif

	if(!(ctx->status))
	{
		BJX2_MemTranslateTlb(ctx, addr+ 0);
		BJX2_MemTranslateTlb(ctx, addr+(32*8));
	}

	if(ctx->status)
		return(NULL);


	cur=BJX2_ContextAllocTrace(ctx);
	cur->addr=addr;

	ctx->rttr[h&63]=cur;

	i=BJX2_DecodeTraceForAddr(ctx, cur, addr);
	if(i<0)
	{
		ctx->rttr[h&63]=NULL;
		BJX2_ContextFreeTrace(ctx, cur);
		return(NULL);
	}

#if 0
	if(ctx->status)
	{
		ctx->rttr[h&63]=NULL;
		BJX2_ContextFreeTrace(ctx, cur);
		return(NULL);
	}
#endif

	cur->hnext=ctx->trhash[h];
	ctx->trhash[h]=cur;
	
	BJX2_CheckJitTrace(ctx, cur);

	return(cur);
}

int BJX2_DecodeTraceFlushCache(BJX2_Context *ctx)
{
	BJX2_Trace *cur, *nxt;
	int i;

	ctx->prttr=NULL;

	ctx->tr_rnxt=NULL;
	ctx->tr_rjmp=NULL;
	
	for(i=0; i<64; i++)
		ctx->rttr[i]=NULL;
	
	for(i=0; i<1024; i++)
	{
		cur=ctx->trhash[i];
		ctx->trhash[i]=NULL;
		while(cur)
		{
			nxt=cur->hnext;
			BJX2_ContextFreeTrace(ctx, cur);
			cur=nxt;
		}
	}
	return(0);
}

int BJX2_ThrowFaultStatus(BJX2_Context *ctx, int status)
{
	u64 exc, sr0;
	int i;

	if(!ctx->regs[BJX2_REG_PC])
		{ JX2_DBGBREAK }

	if(status==BJX2_FLT_IOPOKE)
	{
//		sr0=ctx->regs[BJX2_REG_SR];
//		if(sr0&(1<<28))
//			return(0);

		ctx->tr_rnxt=NULL;
		ctx->tr_rjmp=NULL;
		return(0);
	}

	if(ctx->status)
		return(0);

	if(status==BJX2_FLT_SCRPOKE)
	{
//		sr0=ctx->regs[BJX2_REG_SR];
//		if(sr0&(1<<28))
//			return(0);

//		return(0);

		ctx->tr_rnxt=NULL;
		ctx->tr_rjmp=NULL;
		ctx->status=status;
		return(0);
	}

	if(	(status==BJX2_FLT_BREAK)	||
		(status==BJX2_FLT_CCFLUSH))
	{
		ctx->tr_rnxt=NULL;
		ctx->tr_rjmp=NULL;
		ctx->status=status;
		return(0);
	}

	if((status&0xF000)==0xC000)
	{
		i=-1;
		sr0=ctx->regs[BJX2_REG_SR];
		if(sr0&(1<<28))
			return(0);

//		return(0);
	}

#if 1
//	if((status&0xF000)==0x8000)
	if(	((status&0xF000)==0x8000) ||
		((status&0xF000)==0xA000)	)
	{
		sr0=ctx->regs[BJX2_REG_SR];
		exc=(sr0<<32)|(status&65535);
		ctx->regs[BJX2_REG_EXSR]=exc;

		if(sr0&(1<<29))
		{
			JX2_DBGBREAK
		}

//		ctx->regs[BJX2_REG_EXSR]=status;
		for(i=0; i<128; i++)
			ctx->ex_regs[i]=ctx->regs[i];
		for(i=0; i<32; i++)
			ctx->ex_fpreg[i]=ctx->fpreg[i];
	}
#endif

	if((status&0xF000)==0xE000)
	{
//		printf("Syscall\n");
	}

	if((status&0xF000)==0xA000)
	{
		i=-1;
//		printf("Syscall\n");

		exc=ctx->regs[BJX2_REG_SR];
		exc=(exc<<32)|(status&65535);
		ctx->regs[BJX2_REG_EXSR]=exc;

//		printf("Throw\n");
//		BJX2_DbgPrintRegs(ctx);
	}

//	ctx->regs[BJX2_REG_EXSR]=status;
//	ctx->regs[BJX2_REG_EXSR]=
//		(ctx->regs[BJX2_REG_EXSR]&(~65535))|
//		(status&65535);

	sr0=ctx->regs[BJX2_REG_SR];
	exc=(sr0<<32)|(status&65535);

	if(sr0&(1<<29))
	{
		JX2_DBGBREAK
	}

//	exc=(ctx->regs[BJX2_REG_TEA]<<16)|(status&65535);

	ctx->regs[BJX2_REG_EXSR]=exc;
	ctx->tr_rnxt=NULL;
	ctx->tr_rjmp=NULL;
	ctx->status=status;
	return(0);
}

#if 0
int BJX2_FaultSwapRegs(BJX2_Context *ctx)
{
	u64 va, vb;
	int i;

#if 1
	for(i=0; i<8; i++)
	{
		va=ctx->regs[BJX2_REG_R0+i];
		vb=ctx->regs[BJX2_REG_R0B+i];
		ctx->regs[BJX2_REG_R0+i]=vb;
		ctx->regs[BJX2_REG_R0B+i]=va;

		va=ctx->regs[BJX2_REG_R16+i];
		vb=ctx->regs[BJX2_REG_R16B+i];
		ctx->regs[BJX2_REG_R16+i]=vb;
		ctx->regs[BJX2_REG_R16B+i]=va;
	}
#endif

#if 0
	for(i=0; i<8; i++)
	{
		if(i==3)
			continue;
		va=ctx->regs[BJX2_REG_PC+i];
		vb=ctx->regs[BJX2_REG_SPC+i];
		ctx->regs[BJX2_REG_PC+i]=vb;
		ctx->regs[BJX2_REG_SPC+i]=va;
	}
#endif

//	va=ctx->regs[BJX2_REG_PC];
//	vb=ctx->regs[BJX2_REG_SPC];
//	ctx->regs[BJX2_REG_PC]=vb;
//	ctx->regs[BJX2_REG_SPC]=va;

	va=ctx->regs[BJX2_REG_SP];
	vb=ctx->regs[BJX2_REG_SSP];
	ctx->regs[BJX2_REG_SP]=vb;
	ctx->regs[BJX2_REG_SSP]=va;
	
	return(0);
}
#endif

int BJX2_FaultSwapRegs2(BJX2_Context *ctx)
{
	u64 va, vb;
	int i;

#if 0
	for(i=0; i<8; i++)
	{
		va=ctx->regs[BJX2_REG_R0+i];
		vb=ctx->regs[BJX2_REG_R0B+i];
		ctx->regs[BJX2_REG_R0+i]=vb;
		ctx->regs[BJX2_REG_R0B+i]=va;

		va=ctx->regs[BJX2_REG_R16+i];
		vb=ctx->regs[BJX2_REG_R16B+i];
		ctx->regs[BJX2_REG_R16+i]=vb;
		ctx->regs[BJX2_REG_R16B+i]=va;
	}
#endif

	return(0);
}

int BJX2_FaultEnterRegs(BJX2_Context *ctx, int exsr)
{
	u64 va, vb, vc;
	int i;

//	BJX2_FaultSwapRegs2(ctx);

	va=ctx->regs[BJX2_REG_SR];
	vb=ctx->regs[BJX2_REG_EXSR];
	vc=(va<<32)|((u32)vb);
	ctx->regs[BJX2_REG_EXSR]=vc;

	if(va&(1<<29))
	{
		JX2_DBGBREAK
	}

	va=ctx->regs[BJX2_REG_PC];
//	vb=ctx->regs[BJX2_REG_SPC];
//	ctx->regs[BJX2_REG_PC]=vb;
	ctx->regs[BJX2_REG_SPC]=va;

//	if((exsr&0xF000)!=0xE000)
	if(1)
	{
		va=ctx->regs[BJX2_REG_SP];
		vb=ctx->regs[BJX2_REG_SSP];
		ctx->regs[BJX2_REG_SP]=vb;
		ctx->regs[BJX2_REG_SSP]=va;
	}

//	va=ctx->regs[BJX2_REG_LR];
//	vb=ctx->regs[BJX2_REG_SLR];
//	ctx->regs[BJX2_REG_LR]=vb;
//	ctx->regs[BJX2_REG_SLR]=va;

//	va=ctx->regs[BJX2_REG_LR];
//	ctx->regs[BJX2_REG_SLR]=va;
//	va=ctx->regs[BJX2_REG_SR];
//	ctx->regs[BJX2_REG_SSR]=va;

//	va=ctx->regs[BJX2_REG_DLR];
//	vb=ctx->regs[BJX2_REG_DHR];
//	ctx->regs[BJX2_REG_SDL]=va;
//	ctx->regs[BJX2_REG_SDH]=vb;
	
	return(0);
}

int BJX2_FaultExitRegs(BJX2_Context *ctx, int exsr)
{
	u64 va, vb, sr0;
	int i;

	sr0=ctx->regs[BJX2_REG_SR];

//	BJX2_FaultSwapRegs2(ctx);

	va=ctx->regs[BJX2_REG_SR];
	vb=ctx->regs[BJX2_REG_EXSR];
	va=(va&0xFFFFFFFF00000000ULL)|((u32)(vb>>32));
	ctx->regs[BJX2_REG_SR]=va;

	if(va&(1<<29))
	{
		JX2_DBGBREAK
	}

//	va=ctx->regs[BJX2_REG_PC];
	vb=ctx->regs[BJX2_REG_SPC];
	ctx->regs[BJX2_REG_PC]=vb;
//	ctx->regs[BJX2_REG_SPC]=va;

//	if((exsr&0xF000)!=0xE000)
//	if(1)
	if(sr0&(1<<29))
	{
		va=ctx->regs[BJX2_REG_SP];
		vb=ctx->regs[BJX2_REG_SSP];
		ctx->regs[BJX2_REG_SP]=vb;
		ctx->regs[BJX2_REG_SSP]=va;
	}else
	{
//		JX2_DBGBREAK
	}

//	va=ctx->regs[BJX2_REG_LR];
//	vb=ctx->regs[BJX2_REG_SLR];
//	ctx->regs[BJX2_REG_LR]=vb;
//	ctx->regs[BJX2_REG_SLR]=va;

//	va=ctx->regs[BJX2_REG_SLR];
//	ctx->regs[BJX2_REG_LR]=va;
//	va=ctx->regs[BJX2_REG_SSR];
//	ctx->regs[BJX2_REG_SR]=va;

//	va=ctx->regs[BJX2_REG_SDL];
//	vb=ctx->regs[BJX2_REG_SDH];
//	ctx->regs[BJX2_REG_DLR]=va;
//	ctx->regs[BJX2_REG_DHR]=vb;

	return(0);
}


int BJX2_FaultEnterInterrupt(BJX2_Context *ctx)
{
	u64 vbr, exsr, sr0, pc1;
	int i, j, k;

	sr0=ctx->regs[BJX2_REG_SR];
	if(sr0&(1<<28))
	{
		exsr=ctx->regs[BJX2_REG_EXSR];
		if((exsr&0xF000)==0xC000)
			return(0);

		JX2_DBGBREAK
		return(0);
	}

	exsr=ctx->regs[BJX2_REG_EXSR];
	vbr=ctx->regs[BJX2_REG_VBR];
	if(!vbr && ((exsr&0xF000)!=0xC000))
	{
		printf("Fault Into ROM %X\n", (int)exsr);
		return(0);
	}

	if(!(ctx->regs[BJX2_REG_SR]&(1<<29)))
	{
#if 1
		exsr=ctx->regs[BJX2_REG_EXSR];
//		exsr=ctx->status;
		if(	((exsr&0xF000)==0x8000) &&
			(exsr!=BJX2_FLT_BREAK) &&
//			(exsr==ctx->ex_regs[BJX2_REG_EXSR]))
			(((u16)exsr)==((u16)ctx->ex_regs[BJX2_REG_EXSR])))
		{
			for(i=0; i<128; i++)
			{
				if(i==BJX2_REG_EXSR)
					continue;
				ctx->regs[i]=ctx->ex_regs[i];
			}
			for(i=0; i<32; i++)
				ctx->fpreg[i]=ctx->ex_fpreg[i];
		}
//		ctx->ex_regs[BJX2_REG_EXSR]=0;

#if 1
		if(	((exsr&0xF000)==0xA000) &&
			(((u16)exsr)==((u16)ctx->ex_regs[BJX2_REG_EXSR]))	)
		{
			for(i=0; i<128; i++)
			{
				if(i==BJX2_REG_EXSR)
					continue;
				ctx->regs[i]=ctx->ex_regs[i];
			}
			for(i=0; i<32; i++)
				ctx->fpreg[i]=ctx->ex_fpreg[i];
		}
#endif

#endif

//		BJX2_FaultSwapRegs(ctx);
		BJX2_FaultEnterRegs(ctx, exsr);

//		if((exsr&0xF000)!=0xE000)
		ctx->regs[BJX2_REG_SR]|=(1<<29);	//Set RB
	}

//	if((exsr&0xF000)!=0xE000)
	ctx->regs[BJX2_REG_SR]|=(1<<28);	//BL
	ctx->regs[BJX2_REG_SR]|=(1<<30);	//MD
	
	vbr=ctx->regs[BJX2_REG_VBR];
	exsr=ctx->regs[BJX2_REG_EXSR];
	if((exsr&0xF000)==0xE000)
	{
		pc1=vbr+0x20;
	}else
		if((exsr&0xF000)==0xA000)
	{
		pc1=vbr+0x18;
	}else
		if((exsr&0xF000)==0xC000)
	{
		pc1=vbr+0x10;
	}else
	{
		pc1=vbr+0x08;
	}
	ctx->regs[BJX2_REG_PC]=pc1;
	ctx->trcur=NULL;

	ctx->status=0;
	return(1);
}

int BJX2_FaultLeaveInterrupt(BJX2_Context *ctx)
{
	u64 sr;

	sr=ctx->regs[BJX2_REG_SR];
//	if(sr&(1<<29))
	if(1)
	{
//		BJX2_FaultSwapRegs(ctx);
		BJX2_FaultExitRegs(ctx, ctx->regs[BJX2_REG_EXSR]);
//		ctx->regs[BJX2_REG_SR]&=~(1<<29);	//Clear RB

		ctx->trcur=NULL;
		ctx->tr_rnxt=NULL;
		ctx->tr_rjmp=NULL;
		return(0);
	}
	ctx->regs[BJX2_REG_SR]&=~(1<<29);	//Clear RB
	ctx->trcur=NULL;
	ctx->tr_rnxt=NULL;
	ctx->tr_rjmp=NULL;
	return(0);
}

char *BJX2_DbgNameForAddrSz(BJX2_Context *ctx,
	bjx2_addr addr, bjx2_addr *raddr2, int *rsz2)
{
	bjx2_addr ba, ba2;
	char *bn;
	int i, j, k, m, n, sz;

	m=0;
	n=ctx->map_n_ents;
	
	while(n>(m+3))
	{
		i=(m+n)/2;
		if(ctx->map_addr[i]>=addr)
		{
			n=i;
		}else
		{
			m=i;
		}
	}

	i=m;
	while(i<ctx->map_n_ents)
	{
		if(addr<ctx->map_addr[i])
			break;
		ba=ctx->map_addr[i];
		bn=ctx->map_name[i];
		ba2=ctx->map_addr[i+1];
		i++;
	}
	
	sz=ba2-ba;

#if 0
	ba=-999999999;
	bn=NULL;
	
	for(i=0; i<ctx->map_n_ents; i++)
	{
		if(addr<ctx->map_addr[i])
			continue;
		if(ctx->map_addr[i]>ba)
		{
			ba=ctx->map_addr[i];
			bn=ctx->map_name[i];
		}
	}
#endif
	
	if(raddr2)
		*raddr2=ba;
	if(rsz2)
		*rsz2=sz;
	return(bn);
}

char *BJX2_DbgNameForAddr(BJX2_Context *ctx,
	bjx2_addr addr, bjx2_addr *raddr2)
{
	return(BJX2_DbgNameForAddrSz(ctx, addr, raddr2, NULL));
}

bjx2_addr BJX2_DbgAddrForName(BJX2_Context *ctx, char *name)
{
	int i;

	for(i=0; i<ctx->map_n_ents; i++)
	{
		if(ctx->map_addr[i] < 0x10000)
			continue;
	
		if(!strcmp(ctx->map_name[i], name))
			return(ctx->map_addr[i]);
	}

	return(0);
}

int BJX2_DbgAddrAccessTrap(BJX2_Context *ctx,
	bjx2_addr base, bjx2_addr tgt, int asz)
{
	s64 tvus;
	bjx2_addr ba1, ba2;
	int bsz;
	char *s0, *s1;

//	if(ctx->use_jit)
	if(ctx->use_jit || !(ctx->dbg_data_start))
		return(0);

#if 0
	if((base>=ctx->dbg_data_start) && (base<ctx->dbg_bss_end))
	{
		tvus=BJX2_Interp_GetVirtualUsec(ctx);
		if(tvus<5000000)
			return(0);
	
		s0=BJX2_DbgNameForAddrSz(ctx, base, &ba1, &bsz);
		ba2=ba1+bsz;
		if(s0)
		{
			if(((tgt+asz)>ba2) || (tgt<ba1))
			{
				BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
			}
		}
		
//		s1=BJX2_DbgNameForAddr(ctx, tgt, NULL);
//		if(s0!=s1)
//		{
//			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
//		}
	}
#endif
	
	if(BJX2_MemQueryTransit(ctx, base, tgt+asz, 1)&1)
	{
		BJX2_ThrowFaultStatus(ctx, BJX2_FLT_BOUNDCHK);
	}
	
	return(0);
}

char *BJX2_DbgPrintNameForNmid(BJX2_Context *ctx, int nmid)
{
	static char tb[128];
	char *s0;
	
	s0=NULL;
	switch(nmid)
	{
	case BJX2_NMID_NONE:		s0="None";		break;
	case BJX2_NMID_MOVB:		s0="MOV.B";		break;
	case BJX2_NMID_MOVW:		s0="MOV.W";		break;
	case BJX2_NMID_MOVL:		s0="MOV.L";		break;
	case BJX2_NMID_MOVQ:		s0="MOV.Q";		break;
	case BJX2_NMID_MOVUB:		s0="MOVU.B";	break;
	case BJX2_NMID_MOVUW:		s0="MOVU.W";	break;
	case BJX2_NMID_NOP:			s0="NOP";		break;
	case BJX2_NMID_BRA:			s0="BRA";		break;
	case BJX2_NMID_BSR:			s0="BSR";		break;
	case BJX2_NMID_BT:			s0="BT";		break;
	case BJX2_NMID_BF:			s0="BF";		break;
	case BJX2_NMID_LEAB:		s0="LEA.B";		break;
	case BJX2_NMID_LEAW:		s0="LEA.W";		break;
	case BJX2_NMID_LEAD:		s0="LEA.L";		break;
	case BJX2_NMID_LEAQ:		s0="LEA.Q";		break;
	case BJX2_NMID_ADD:			s0="ADD";		break;
	case BJX2_NMID_SUB:			s0="SUB";		break;
	case BJX2_NMID_ADC:			s0="ADC";		break;
	case BJX2_NMID_SBB:			s0="SBB";		break;
	case BJX2_NMID_TST:			s0="TST";		break;
	case BJX2_NMID_AND:			s0="AND";		break;
	case BJX2_NMID_OR:			s0="OR";		break;
	case BJX2_NMID_XOR:			s0="XOR";		break;
	case BJX2_NMID_MOV:			s0="MOV";		break;
	case BJX2_NMID_LDIZ:		s0="LDIZ";		break;
	case BJX2_NMID_LDIN:		s0="LDIN";		break;
	case BJX2_NMID_LDISH8:		s0="LDISH8";	break;
	case BJX2_NMID_CMPEQ:		s0="CMPEQ";		break;
	case BJX2_NMID_CMPHI:		s0="CMPHI";		break;
	case BJX2_NMID_CMPGT:		s0="CMPGT";		break;
	case BJX2_NMID_RTS:			s0="RTS";		break;
	case BJX2_NMID_RTSU:		s0="RTSU";		break;
	case BJX2_NMID_EXTUB:		s0="EXTU.B";	break;
	case BJX2_NMID_EXTUW:		s0="EXTU.W";	break;
	case BJX2_NMID_EXTSB:		s0="EXTS.B";	break;
	case BJX2_NMID_EXTSW:		s0="EXTS.W";	break;
	case BJX2_NMID_CLRT:		s0="CLRT";		break;
	case BJX2_NMID_SETT:		s0="SETT";		break;
	case BJX2_NMID_CLRS:		s0="CLRS";		break;
	case BJX2_NMID_SETS:		s0="SETS";		break;
	case BJX2_NMID_NOTT:		s0="NOTT";		break;
	case BJX2_NMID_NOTS:		s0="NOTS";		break;
	case BJX2_NMID_LDISH16:		s0="LDISH16";	break;
	case BJX2_NMID_CMPGE:		s0="CMPGE";		break;
	case BJX2_NMID_PUSH:		s0="PUSH";		break;
	case BJX2_NMID_POP:			s0="POP";		break;
	case BJX2_NMID_CMPPL:		s0="CMPPL";		break;
	case BJX2_NMID_CMPPZ:		s0="CMPPZ";		break;
	case BJX2_NMID_SHLL:		s0="SHLL";		break;
	case BJX2_NMID_SHLR:		s0="SHLR";		break;
	case BJX2_NMID_SHAR:		s0="SHAR";		break;
	case BJX2_NMID_CMPHS:		s0="CMPHS";		break;
	case BJX2_NMID_BREAK:		s0="BREAK";		break;
	case BJX2_NMID_SLEEP:		s0="SLEEP";		break;
	case BJX2_NMID_MULS:		s0="MULS";		break;
	case BJX2_NMID_MULU:		s0="MULU";		break;
	case BJX2_NMID_RTE:			s0="RTE";		break;
	case BJX2_NMID_NOT:			s0="NOT";		break;
	case BJX2_NMID_NEG:			s0="NEG";		break;
	case BJX2_NMID_NEGC:		s0="NEGC";		break;
	case BJX2_NMID_ROTL:		s0="ROTL";		break;
	case BJX2_NMID_ROTR:		s0="ROTR";		break;
	case BJX2_NMID_ROTCL:		s0="ROTCL";		break;
	case BJX2_NMID_ROTCR:		s0="ROTCR";		break;
	case BJX2_NMID_RET:			s0="RET";		break;

	case BJX2_NMID_MOVT:		s0="MOVT";		break;
	case BJX2_NMID_LDISH12:		s0="LDISH12";	break;
	case BJX2_NMID_LDISH20:		s0="LDISH20";	break;
	case BJX2_NMID_SHAD:		s0="SHAD";		break;
	case BJX2_NMID_SHLD:		s0="SHLD";		break;

	case BJX2_NMID_MULSL:		s0="DMULS.L";	break;
	case BJX2_NMID_MULUL:		s0="DMULU.L";	break;
	case BJX2_NMID_MULSW:		s0="DMULS.W";	break;
	case BJX2_NMID_MULUW:		s0="DMULU.W";	break;

	case BJX2_NMID_WEXMD:		s0="WEXMD";		break;
	case BJX2_NMID_SYSCALL:		s0="SYSCALL";	break;
	case BJX2_NMID_INVIC:		s0="INVIC";		break;
	case BJX2_NMID_INVDC:		s0="INVDC";		break;

	case BJX2_NMID_SWAPB:		s0="SWAP.B";	break;
	case BJX2_NMID_SWAPW:		s0="SWAP.W";	break;

	case BJX2_NMID_FADD:		s0="FADD";		break;
	case BJX2_NMID_FSUB:		s0="FSUB";		break;
	case BJX2_NMID_FMUL:		s0="FMUL";		break;
	case BJX2_NMID_FDIV:		s0="FDIV";		break;
	case BJX2_NMID_FCMPEQ:		s0="FCMPEQ";	break;
	case BJX2_NMID_FCMPGT:		s0="FCMPGT";	break;
	case BJX2_NMID_FMOVS:		s0="FMOV.S";	break;
	case BJX2_NMID_FMOVD:		s0="FMOV.D";	break;
	case BJX2_NMID_FMOV:		s0="FMOV";		break;
	case BJX2_NMID_FNEG:		s0="FNEG";		break;
	case BJX2_NMID_FLDCF:		s0="FLDCF";		break;
	case BJX2_NMID_FSTCF:		s0="FSTCF";		break;
	case BJX2_NMID_FLDCI:		s0="FLDCI";		break;
	case BJX2_NMID_FSTCI:		s0="FSTCI";		break;
	case BJX2_NMID_FLDCH:		s0="FLDCH";		break;
	case BJX2_NMID_FSTCH:		s0="FSTCH";		break;
	case BJX2_NMID_FLDCD:		s0="FLDCD";		break;
	case BJX2_NMID_FSTCD:		s0="FSTCD";		break;
	case BJX2_NMID_FABS:		s0="FABS";		break;
	case BJX2_NMID_FLDCFH:		s0="FLDCFH";	break;
	case BJX2_NMID_FSTCFH:		s0="FSTCFH";	break;

	case BJX2_NMID_INV:			s0="Invalid";	break;
	case BJX2_NMID_MOVUL:		s0="MOVU.L";	break;
	case BJX2_NMID_EXTUL:		s0="EXTU.L";	break;
	case BJX2_NMID_EXTSL:		s0="EXTS.L";	break;
	case BJX2_NMID_LDISH32:		s0="LDISH32";	break;

	case BJX2_NMID_SHADQ:		s0="SHAD.Q";	break;
	case BJX2_NMID_SHLDQ:		s0="SHLD.Q";	break;
	case BJX2_NMID_SHARQ:		s0="SHAR.Q";	break;
	case BJX2_NMID_SHLRQ:		s0="SHLR.Q";	break;

	case BJX2_NMID_TSTQ:		s0="TSTQ";	break;

	case BJX2_NMID_CMPQEQ:		s0="CMPQEQ";	break;
	case BJX2_NMID_CMPQGT:		s0="CMPQGT";	break;
	case BJX2_NMID_CMPQHI:		s0="CMPQHI";	break;
	case BJX2_NMID_CMPQGE:		s0="CMPQGE";	break;
	case BJX2_NMID_CMPQHS:		s0="CMPQHS";	break;

	case BJX2_NMID_FPUSH:		s0="FPUSH";		break;
	case BJX2_NMID_FPOP:		s0="FPOP";		break;

	case BJX2_NMID_CSELT:		s0="CSELT";		break;
	case BJX2_NMID_MOVNT:		s0="MOVNT";		break;
	case BJX2_NMID_CLZ:			s0="CLZ";		break;
	case BJX2_NMID_CLZQ:		s0="CLZQ";		break;
	case BJX2_NMID_MOVDL:		s0="MOVD.L";	break;
	case BJX2_NMID_LDI:			s0="LDI";		break;
	case BJX2_NMID_JLDI:		s0="JLDI";		break;
	case BJX2_NMID_LDIQ:		s0="LDIQ";		break;

	case BJX2_NMID_LDTLB:		s0="LDTLB";		break;

	case BJX2_NMID_MOVX2:		s0="MOV.X";		break;
	case BJX2_NMID_PUSHX2:		s0="PUSH.X";	break;
	case BJX2_NMID_POPX2:		s0="POP.X";		break;

	case BJX2_NMID_ADDSL:		s0="ADDS.L";	break;
	case BJX2_NMID_ADDUL:		s0="ADDU.L";	break;
	case BJX2_NMID_SUBSL:		s0="SUBS.L";	break;
	case BJX2_NMID_SUBUL:		s0="SUBU.L";	break;

	case BJX2_NMID_PADDW:		s0="PADD.W";	break;
	case BJX2_NMID_PADDL:		s0="PADD.L";	break;
	case BJX2_NMID_PSUBW:		s0="PSUB.W";	break;
	case BJX2_NMID_PSUBL:		s0="PSUB.L";	break;

	case BJX2_NMID_PMULUW:		s0="PMULU.W";	break;
//	case BJX2_NMID_PMULSW:		s0="PMULS.W";	break;

	case BJX2_NMID_PRED_T:		s0="PRED.T";	break;
	case BJX2_NMID_PRED_F:		s0="PRED.F";	break;

//	case BJX2_NMID_MULSL:		s0="MULS.L";	break;
//	case BJX2_NMID_MULUL:		s0="MULU.L";	break;
	case BJX2_NMID_LDIL:		s0="LDI.L";	break;

//	case BJX2_NMID_FLDCFH:		s0="FLDCFH";	break;
//	case BJX2_NMID_FSTCFH:		s0="FSTCFH";	break;

	case BJX2_NMID_MOVHD:		s0="MOVHD";		break;
	case BJX2_NMID_MOVLD:		s0="MOVLD";		break;
	case BJX2_NMID_MOVHLD:		s0="MOVHLD";	break;
	case BJX2_NMID_MOVLHD:		s0="MOVLHD";	break;

	case BJX2_NMID_PSHUFB:		s0="PSHUF.B";	break;
	case BJX2_NMID_RGBSHR1:		s0="RGB5SHR1";	break;
	case BJX2_NMID_RGBAVG:		s0="RGB5AVG";	break;
	case BJX2_NMID_RGBMOD:		s0="RGB5MOD";	break;
	case BJX2_NMID_RGBPCK:		s0="RGB5PCK";	break;
	case BJX2_NMID_PSHUFW:		s0="PSHUF.W";	break;
	case BJX2_NMID_PMULSLW:		s0="PMULS.LW";	break;
	case BJX2_NMID_PMULULW:		s0="PMULU.LW";	break;
	case BJX2_NMID_PMULSHW:		s0="PMULS.HW";	break;
	case BJX2_NMID_PMULUHW:		s0="PMULU.HW";	break;

	case BJX2_NMID_BLKUTX1:		s0="BLKUTX1";	break;
	case BJX2_NMID_BLKUTX2:		s0="BLKUTX2";	break;

	case BJX2_NMID_PMORTL:		s0="PMORT.L";	break;
	case BJX2_NMID_PMORTQ:		s0="PMORT.Q";	break;

	case BJX2_NMID_PADDF:		s0="PADD.F";	break;
	case BJX2_NMID_PADDXF:		s0="PADDX.F";	break;
	case BJX2_NMID_PSUBF:		s0="PSUB.F";	break;
	case BJX2_NMID_PSUBXF:		s0="PSUBX.F";	break;
	case BJX2_NMID_PMULF:		s0="PMUL.F";	break;
	case BJX2_NMID_PMULXF:		s0="PMULX.F";	break;

	case BJX2_NMID_PADDH:		s0="PADD.H";	break;
	case BJX2_NMID_PADDXD:		s0="PADDX.D";	break;
	case BJX2_NMID_PSUBH:		s0="PSUB.H";	break;
	case BJX2_NMID_PSUBXD:		s0="PSUBX.D";	break;
	case BJX2_NMID_PMULH:		s0="PMUL.H";	break;
	case BJX2_NMID_PMULXD:		s0="PMULX.D";	break;

	case BJX2_NMID_RGB32PCK64:		s0="RGB32PCK64";	break;
	case BJX2_NMID_RGB32UPCK64:		s0="RGB32UPCK64";	break;
	case BJX2_NMID_RGB5PCK32:		s0="RGB5PCK32";		break;
	case BJX2_NMID_RGB5UPCK32:		s0="RGB5UPCK32";	break;
	case BJX2_NMID_RGB5PCK64:		s0="RGB5PCK64";		break;
	case BJX2_NMID_RGB5UPCK64:		s0="RGB5UPCK64";	break;

	default:
		sprintf(tb, "?NM%02X", nmid);
		s0=tb;
		break;
	}
	
	return(s0);
}

char *BJX2_DbgPrintNameForReg(BJX2_Context *ctx, int reg)
{
	char *s;

	switch(reg)
	{
#ifdef BJX2_REG_REMAP
	case BJX2_REG_R0:		s="R0";		break;
	case BJX2_REG_R1:		s="R1";		break;
#endif
	case BJX2_REG_R2:		s="R2";		break;
	case BJX2_REG_R3:		s="R3";		break;
	case BJX2_REG_R4:		s="R4";		break;
	case BJX2_REG_R5:		s="R5";		break;
	case BJX2_REG_R6:		s="R6";		break;
	case BJX2_REG_R7:		s="R7";		break;
	case BJX2_REG_R8:		s="R8";		break;
	case BJX2_REG_R9:		s="R9";		break;
	case BJX2_REG_R10:		s="R10";	break;
	case BJX2_REG_R11:		s="R11";	break;
	case BJX2_REG_R12:		s="R12";	break;
	case BJX2_REG_R13:		s="R13";	break;
	case BJX2_REG_R14:		s="R14";	break;

#ifdef BJX2_REG_REMAP
	case BJX2_REG_R15:		s="R15";	break;
#endif

	case BJX2_REG_R16:		s="R16";	break;
	case BJX2_REG_R17:		s="R17";	break;
	case BJX2_REG_R18:		s="R18";	break;
	case BJX2_REG_R19:		s="R19";	break;
	case BJX2_REG_R20:		s="R20";	break;
	case BJX2_REG_R21:		s="R21";	break;
	case BJX2_REG_R22:		s="R22";	break;
	case BJX2_REG_R23:		s="R23";	break;
	case BJX2_REG_R24:		s="R24";	break;
	case BJX2_REG_R25:		s="R25";	break;
	case BJX2_REG_R26:		s="R26";	break;
	case BJX2_REG_R27:		s="R27";	break;
	case BJX2_REG_R28:		s="R28";	break;
	case BJX2_REG_R29:		s="R29";	break;
	case BJX2_REG_R30:		s="R30";	break;
	case BJX2_REG_R31:		s="R31";	break;

#ifndef BJX2_REG_REMAP
	case BJX2_REG_R32:		s="R32";	break;
	case BJX2_REG_R33:		s="R33";	break;
#endif
	case BJX2_REG_R34:		s="R34";	break;
	case BJX2_REG_R35:		s="R35";	break;
	case BJX2_REG_R36:		s="R36";	break;
	case BJX2_REG_R37:		s="R37";	break;
	case BJX2_REG_R38:		s="R38";	break;
	case BJX2_REG_R39:		s="R39";	break;
	case BJX2_REG_R40:		s="R40";	break;
	case BJX2_REG_R41:		s="R41";	break;
	case BJX2_REG_R42:		s="R42";	break;
	case BJX2_REG_R43:		s="R43";	break;
	case BJX2_REG_R44:		s="R44";	break;
	case BJX2_REG_R45:		s="R45";	break;
	case BJX2_REG_R46:		s="R46";	break;

#ifndef BJX2_REG_REMAP
	case BJX2_REG_R47:		s="R47";	break;
#endif

#if 1
	case BJX2_REG_R48:		s="R48";	break;
	case BJX2_REG_R49:		s="R49";	break;
	case BJX2_REG_R50:		s="R50";	break;
	case BJX2_REG_R51:		s="R51";	break;
	case BJX2_REG_R52:		s="R52";	break;
	case BJX2_REG_R53:		s="R53";	break;
	case BJX2_REG_R54:		s="R54";	break;
	case BJX2_REG_R55:		s="R55";	break;
	case BJX2_REG_R56:		s="R56";	break;
	case BJX2_REG_R57:		s="R57";	break;
	case BJX2_REG_R58:		s="R58";	break;
	case BJX2_REG_R59:		s="R59";	break;
	case BJX2_REG_R60:		s="R60";	break;
	case BJX2_REG_R61:		s="R61";	break;
	case BJX2_REG_R62:		s="R62";	break;
	case BJX2_REG_R63:		s="R63";	break;
#endif

	case BJX2_REG_DLR:		s="DLR";	break;
	case BJX2_REG_DHR:		s="DHR";	break;
	case BJX2_REG_SP:		s="SP";		break;

	case BJX2_REG_PC:		s="PC";		break;
	case BJX2_REG_LR:		s="LR";		break;
	case BJX2_REG_SR:		s="SR";		break;
	case BJX2_REG_VBR:		s="VBR";	break;
	case BJX2_REG_SPC:		s="SPC";	break;
	case BJX2_REG_SSP:		s="SSP";	break;
	case BJX2_REG_GBR:		s="GBR";	break;
	case BJX2_REG_TBR:		s="TBR";	break;
	case BJX2_REG_TTB:		s="TTB";	break;
	case BJX2_REG_TEA:		s="TEA";	break;
	case BJX2_REG_MMCR:		s="MMCR";	break;
	case BJX2_REG_EXSR:		s="EXSR";	break;
	case BJX2_REG_KRR:		s="KRR";	break;


#if 0
	case BJX2_REG_R0B:		s="R0B";	break;
	case BJX2_REG_R1B:		s="R1B";	break;
	case BJX2_REG_R2B:		s="R2B";	break;
	case BJX2_REG_R3B:		s="R3B";	break;
	case BJX2_REG_R4B:		s="R4B";	break;
	case BJX2_REG_R5B:		s="R5B";	break;
	case BJX2_REG_R6B:		s="R6B";	break;
	case BJX2_REG_R7B:		s="R7B";	break;

	case BJX2_REG_SPC:		s="SPC";	break;
	case BJX2_REG_SLR:		s="SLR";	break;
	case BJX2_REG_SSR:		s="SSR";	break;
	case BJX2_REG_SSP:		s="SSP";	break;
	case BJX2_REG_SDL:		s="SDL";	break;
	case BJX2_REG_SDH:		s="SDH";	break;
	case BJX2_REG_SGB:		s="SGB";	break;
	case BJX2_REG_STB:		s="STB";	break;

	case BJX2_REG_R16B:		s="R16B";	break;
	case BJX2_REG_R17B:		s="R17B";	break;
	case BJX2_REG_R18B:		s="R18B";	break;
	case BJX2_REG_R19B:		s="R19B";	break;
	case BJX2_REG_R20B:		s="R20B";	break;
	case BJX2_REG_R21B:		s="R21B";	break;
	case BJX2_REG_R22B:		s="R22B";	break;
	case BJX2_REG_R23B:		s="R23B";	break;
#endif

	default:				s="?";		break;
	}
	
	return(s);
}

char *BJX2_DbgPrintNameForFReg(BJX2_Context *ctx, int reg)
{
	char *s;

	switch(reg)
	{
	case BJX2_REG_R0:		s="FR0";	break;
	case BJX2_REG_R1:		s="FR1";	break;
	case BJX2_REG_R2:		s="FR2";	break;
	case BJX2_REG_R3:		s="FR3";	break;
	case BJX2_REG_R4:		s="FR4";	break;
	case BJX2_REG_R5:		s="FR5";	break;
	case BJX2_REG_R6:		s="FR6";	break;
	case BJX2_REG_R7:		s="FR7";	break;
	case BJX2_REG_R8:		s="FR8";	break;
	case BJX2_REG_R9:		s="FR9";	break;
	case BJX2_REG_R10:		s="FR10";	break;
	case BJX2_REG_R11:		s="FR11";	break;
	case BJX2_REG_R12:		s="FR12";	break;
	case BJX2_REG_R13:		s="FR13";	break;
	case BJX2_REG_R14:		s="FR14";	break;
	case BJX2_REG_R15:		s="FR15";	break;

	default:				s="?";		break;
	}
	
	return(s);
}

int BJX2_DbgPrintOp(BJX2_Context *ctx, BJX2_Opcode *op, int fl)
{
	char tb1[64];
	BJX2_Opcode *op1;
	s64 li;
	int msc, psc, brpc, nonl, nosc;

//	printf("%05X  %04X %-8s ", op->pc, op->opn,
//		BJX2_DbgPrintNameForNmid(ctx, op->nmid));

	if(fl&4)
	{
		brpc=op->pc2;
	}else
	{
		if(op->fl&BJX2_OPFL_JUMBO96)
		{
			op1=op->data;

//			printf("%08X  (%2d) %04X_%04X_%04X -\n",
//				(u32)op->pc, op->cyc,
//				op->opn, op->opn2, op->opn3);
//			printf("%08X       %04X_%04X_%04X   %-8s ",
//				(u32)op1->pc,
//				op1->opn, op1->opn2, op1->opn3,
//				BJX2_DbgPrintNameForNmid(ctx, op->nmid));

			printf("%08X  (%2d) %04X_%04X   -\n",
				(u32)op->pc+0, op->cyc,
				op->opn, op->opn2);
			printf("%08X       %04X_%04X   -\n",
				(u32)op->pc+4,
				op->opn3, op->opn);
			printf("%08X       %04X_%04X   %-8s ",
				(u32)op->pc+8,
				op1->opn2, op1->opn3,
				BJX2_DbgPrintNameForNmid(ctx, op->nmid));

			brpc=op->pc+12;
		}else
		if(op->fl&BJX2_OPFL_JUMBO64)
		{
			op1=op->data;

			printf("%08X  (%2d) %04X_%04X   -\n",
				(u32)op->pc, op->cyc,
				op->opn, op->opn2);
			printf("%08X       %04X_%04X   %-8s ",
				(u32)op1->pc,
				op1->opn, op1->opn2,
				BJX2_DbgPrintNameForNmid(ctx, op->nmid));
			brpc=op->pc+8;
		}else
		if(op->fl&BJX2_OPFL_TRIWORD)
		{
			printf("%08X  (%2d) %04X_%04X_%04X %c %-8s ",
				(u32)op->pc, op->cyc,
				op->opn, op->opn2, op->opn3,
				((op->fl&BJX2_OPFL_WEX)?'|':' '),
				BJX2_DbgPrintNameForNmid(ctx, op->nmid));
			brpc=op->pc+6;
		}else
		if(op->fl&BJX2_OPFL_TWOWORD)
		{
			if((op->nmid==BJX2_NMID_PRED_T) || (op->nmid==BJX2_NMID_PRED_F))
			{
				op1=op->data;

				sprintf(tb1, "%s?%c",
					BJX2_DbgPrintNameForNmid(ctx, op1->nmid),
						(op->nmid==BJX2_NMID_PRED_T)?'T':'F');

				if(fl&2)
				{
					printf("%08X  (%2d) %04X_%04X     %c %-8s ",
						(u32)op->pc, op->cyc,
						op->opn, op->opn2,
						((op->fl&BJX2_OPFL_WEX)?'|':' '),
						tb1);
				}else
				{
					printf("%08X  (%2d) %04X_%04X %c %-8s ",
						(u32)op->pc, op->cyc,
						op->opn, op->opn2,
					((op->fl&BJX2_OPFL_WEX)?'|':' '),
						tb1);
				}
			}else
			{
				if(fl&2)
				{
					printf("%08X  (%2d) %04X_%04X      %c %-8s ",
						(u32)op->pc, op->cyc,
						op->opn, op->opn2,
						((op->fl&BJX2_OPFL_WEX)?'|':' '),
						BJX2_DbgPrintNameForNmid(ctx, op->nmid));
				}else
				{
					printf("%08X  (%2d) %04X_%04X %c %-8s ",
						(u32)op->pc, op->cyc,
						op->opn, op->opn2,
						((op->fl&BJX2_OPFL_WEX)?'|':' '),
						BJX2_DbgPrintNameForNmid(ctx, op->nmid));
				}
			}
			brpc=op->pc+4;
		}else
		{
			if(fl&2)
			{
				printf("%08X  (%2d) %04X           %c %-8s ",
					(u32)op->pc, op->cyc,
					op->opn,
					((op->fl&BJX2_OPFL_WEX)?'|':' '),
					BJX2_DbgPrintNameForNmid(ctx, op->nmid));
			}else
			{
				printf("%08X  (%2d) %04X      %c %-8s ",
					(u32)op->pc, op->cyc,
					op->opn,
					((op->fl&BJX2_OPFL_WEX)?'|':' '),
					BJX2_DbgPrintNameForNmid(ctx, op->nmid));
			}
			brpc=op->pc+2;
		}
	}

	msc=1;
	psc=1;
	switch(op->nmid)
	{
	case BJX2_NMID_BRA:
	case BJX2_NMID_BSR:
	case BJX2_NMID_BT:
	case BJX2_NMID_BF:
		psc=2;	break;
	case BJX2_NMID_MOVB:
	case BJX2_NMID_MOVUB:
		msc=1;	break;
	case BJX2_NMID_MOVW:
	case BJX2_NMID_MOVUW:
		msc=2;
		break;
	case BJX2_NMID_MOVL:
	case BJX2_NMID_MOVUL:
	case BJX2_NMID_MOVDL:
		msc=4;	break;
	case BJX2_NMID_MOVQ:
	case BJX2_NMID_MOVX2:
		msc=8;	break;
	case BJX2_NMID_LEAB:
		msc=1;	psc=1;	break;
	case BJX2_NMID_LEAW:
		msc=2;	psc=2;	break;
	case BJX2_NMID_LEAD:
		msc=4;	psc=4;	break;
	case BJX2_NMID_LEAQ:
		msc=8;	psc=8;	break;

	case BJX2_NMID_FMOVS:
		msc=4;	break;
	case BJX2_NMID_FMOVD:
		msc=8;	break;
	}

	op1=NULL;
	nonl=0;

	switch(op->fmid)
	{
	case BJX2_FMID_NONE:
		printf("?");
		break;
	case BJX2_FMID_REG:
		printf("%s", BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGREG:
		printf("%s, %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGREGREG:
		printf("%s, %s, %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->ro),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_Z:
		break;
	case BJX2_FMID_IMM:
		if(	(((sbyte)op->imm)!=op->imm) &&
			(((byte)op->imm)!=op->imm)	)
		{
			printf("#0x%llX", op->imm);
		}
		else
		{
			printf("#%lld", op->imm);
		}
		break;
	case BJX2_FMID_PCREG:
		printf("(PC, %s)", BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_PCDISP:
		if(
			(((sbyte)op->imm)!=op->imm) &&
			(((byte)op->imm)!=op->imm)	)
		{
//			printf("(PC, 0x%X)", op->imm*psc);
			printf("(0x%llX)", brpc+op->imm*psc);
		}else
		{
			printf("(PC, %lld)", op->imm*psc);
		}
		break;
	case BJX2_FMID_LDREGREG:
		printf("(%s), %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGSTREG:
		printf("%s, (%s)",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_LDDRREGREG:
		printf("(%s, DLR), %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGSTDRREG:
		printf("%s, (%s, DLR)",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_LDREGDISPREG:
		if((op->rm==BJX2_REG_PC) || (op->rm==BJX2_REG_GBR))
			msc=1;
	
		if(	(((sbyte)op->imm)!=op->imm) &&
			(((byte)op->imm)!=op->imm)	)
		{
			printf("(%s, 0x%llX), %s",
				BJX2_DbgPrintNameForReg(ctx, op->rm),
				(op->imm*msc),
				BJX2_DbgPrintNameForReg(ctx, op->rn));
		}else
		{
			printf("(%s, %lld), %s",
				BJX2_DbgPrintNameForReg(ctx, op->rm),
				(op->imm*msc),
				BJX2_DbgPrintNameForReg(ctx, op->rn));
		}
		break;
	case BJX2_FMID_REGSTREGDISP:
		if((op->rn==BJX2_REG_PC) || (op->rn==BJX2_REG_GBR))
			msc=1;

		if(	(((sbyte)op->imm)!=op->imm) &&
			(((byte)op->imm)!=op->imm)	)
		{
			printf("%s, (%s, 0x%llX)",
				BJX2_DbgPrintNameForReg(ctx, op->rm),
				BJX2_DbgPrintNameForReg(ctx, op->rn),
				(op->imm*msc));
		}else
		{
			printf("%s, (%s, %lld)",
				BJX2_DbgPrintNameForReg(ctx, op->rm),
				BJX2_DbgPrintNameForReg(ctx, op->rn),
				(op->imm*msc));
		}
		break;

	case BJX2_FMID_REGDRREG:
		printf("%s, DLR, %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;

	case BJX2_FMID_PCDR:
		printf("(PC, DLR)");
		break;

	case BJX2_FMID_LDDRPCREG:
		printf("(PC, DLR), %s",
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGSTDRPC:
		printf("%s, (PC, DLR)",
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_IMMREG:
//		if((((sbyte)op->imm)!=op->imm) &&
//			(((byte)op->imm)!=op->imm))
		if((((s16)op->imm)!=op->imm) &&
			(((u16)op->imm)!=op->imm))
		{
			printf("#0x%llX, %s", op->imm,
				BJX2_DbgPrintNameForReg(ctx, op->rn));
		}else
		{
			printf("#%lld, %s", op->imm,
				BJX2_DbgPrintNameForReg(ctx, op->rn));
		}
		break;

	case BJX2_FMID_IMMJREG:
		op1=op->data;
		li=((u32)op1->imm)|
			(((u64)op->imm)<<32);
		printf("#0x%llX, %s", li,
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;

#if 0
	case BJX2_FMID_IMMXREG:
		li=((u32)op->imm)|
			(((((u32)op->imm)>>31)-1LL)<<32);
		printf("#0x%llX, %s", li,
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
#endif

#if 1
	case BJX2_FMID_IMMZREG:
		li=((u32)op->imm);
		printf("#0x%llX, %s", li,
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_IMMNREG:
//		li=((u32)op->imm)|((-1LL)<<32);
		li=((u32)op->imm)|(~((1LL<<32)-1));
		printf("#0x%llX, %s", li,
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
#endif

	case BJX2_FMID_LDDRABSREG:
		printf("(DLR), %s",
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGSTDRABS:
		printf("%s, (DLR)",
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;

	case BJX2_FMID_PCDR4:
		printf("(PC, DLR:%lld)", op->imm);
		break;
	case BJX2_FMID_LDDR4PCREG:
		printf("(PC, DLR:%lld), %s",
			op->imm, BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BJX2_FMID_REGSTDR4PC:
		printf("%s, (PC, DLR:%lld)",
			BJX2_DbgPrintNameForReg(ctx, op->rn), op->imm);
		break;
	case BJX2_FMID_DR4REG:
		printf("DLR:%lld, %s",
			op->imm, BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;

	case BJX2_FMID_LDPCDISPREG:
		if(op->imm>256)
		{
//			printf("(PC, 0x%X), %s",
//				(op->imm*psc),
//				BJX2_DbgPrintNameForReg(ctx, op->rn));
			printf("(0x%llX), %s",
				brpc+(op->imm*psc),
				BJX2_DbgPrintNameForReg(ctx, op->rn));
		}else
		{
			printf("(PC, %lld), %s",
				(op->imm*psc),
				BJX2_DbgPrintNameForReg(ctx, op->rn));
		}
		break;
	case BJX2_FMID_REGSTPCDISP:
		if(op->imm>256)
		{
//			printf("%s, (PC, 0x%X)",
//				BJX2_DbgPrintNameForReg(ctx, op->rn),
//				(op->imm*psc));
			printf("%s, (0x%llX)",
				BJX2_DbgPrintNameForReg(ctx, op->rn),
				brpc+(op->imm*psc));
		}else
		{
			printf("%s, (PC, %lld)",
				BJX2_DbgPrintNameForReg(ctx, op->rn),
				(op->imm*psc));
		}
		break;

	case BJX2_FMID_LDREG2REG:
		printf("(%s, %s), %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->ro),
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;
		
	case BJX2_FMID_REGSTREG2:
		printf("%s, (%s, %s)",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			BJX2_DbgPrintNameForReg(ctx, op->rn),
			BJX2_DbgPrintNameForReg(ctx, op->ro));
		break;

//	case BJX2_FMID_REGREGREG:
//		printf("%s, %s, %s",
//			BJX2_DbgPrintNameForReg(ctx, op->rm),
//			BJX2_DbgPrintNameForReg(ctx, op->ro),
//			BJX2_DbgPrintNameForReg(ctx, op->rn));
//		break;

	case BJX2_FMID_REGIMMREG:
		printf("%s, #%lld, %s",
			BJX2_DbgPrintNameForReg(ctx, op->rm),
			op->imm,
			BJX2_DbgPrintNameForReg(ctx, op->rn));
		break;

	case BJX2_FMID_FREG:
		printf("%s", BJX2_DbgPrintNameForFReg(ctx, op->rn));
		break;
	case BJX2_FMID_FREGREG:
		printf("%s, %s",
			BJX2_DbgPrintNameForFReg(ctx, op->rm),
			BJX2_DbgPrintNameForFReg(ctx, op->rn));
		break;
	case BJX2_FMID_FREGREGREG:
		printf("%s, %s, %s",
			BJX2_DbgPrintNameForFReg(ctx, op->rm),
			BJX2_DbgPrintNameForFReg(ctx, op->ro),
			BJX2_DbgPrintNameForFReg(ctx, op->rn));
		break;
		
	case BJX2_FMID_CHAIN:
		op1=op->data;
		BJX2_DbgPrintOp(ctx, op1, 4);
		nonl=1;
		break;

	default:
		printf("?");
		break;
	}

//	if(op->fl&BJX2_OPFL_WEX)
//		printf("\t|");

//	if(!op1)
	if(!nonl)
		printf("\n");
	return(0);
}

int BJX2_DbgPrintTraceOps(BJX2_Context *ctx, BJX2_Trace *tr)
{
	BJX2_Opcode *op;
	bjx2_addr ba2;
	char *bn2;
	int i, fl;

	fl=0;
	for(i=0; i<tr->n_ops; i++)
	{
		op=tr->ops[i];

//		if(op->fl&BJX2_OPFL_JUMBO96) fl|=2;
		if(op->fl&BJX2_OPFL_JUMBO96) fl|=1;
		if(op->fl&BJX2_OPFL_JUMBO64) fl|=1;

		if(op->fl&BJX2_OPFL_TRIWORD) fl|=2;
		if(op->fl&BJX2_OPFL_TWOWORD) fl|=1;
	}

	for(i=0; i<tr->n_ops; i++)
	{
		BJX2_DbgPrintOp(ctx, tr->ops[i], fl);
	}
	return(0);
}

int BJX2_DbgPrintTrace(BJX2_Context *ctx, BJX2_Trace *tr)
{
	bjx2_addr ba2;
	char *bn2;
	int i;
	
	if(!tr)
	{
		printf("PC @ ...  NULL Trace\n");
		return(0);
	}
	
	bn2=BJX2_DbgNameForAddr(ctx, tr->addr, &ba2);

	if(bn2)
	{
		printf("PC @ %08X (%s+%d)\n", (u32)(tr->addr),
			bn2, (int)(tr->addr-ba2));
	}else
	{
		printf("PC @ %08X\n", (u32)(tr->addr));
	}
	
	BJX2_DbgPrintTraceOps(ctx, tr);
//	for(i=0; i<tr->n_ops; i++)
//	{
//		BJX2_DbgPrintOp(ctx, tr->ops[i]);
//	}
	return(0);
}


int BJX2_DbgPrintRegs_KnownReg(BJX2_Context *ctx, int ix)
{
	char *s0;

	s0=BJX2_DbgPrintNameForReg(ctx, ix);
	if(!strcmp(s0, "?"))
		return(0);
	return(1);
}

int BJX2_DbgPrintRegs_KnownReg4(BJX2_Context *ctx, int ix)
{
	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+0))
		return(1);
	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+1))
		return(1);
	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+2))
		return(1);
	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+3))
		return(1);
	return(0);
}

int BJX2_DbgPrintRegs_KnownReg2(BJX2_Context *ctx, int ix)
{
	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+0))
		return(1);
	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+1))
		return(1);
//	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+2))
//		return(1);
//	if(BJX2_DbgPrintRegs_KnownReg(ctx, ix+3))
//		return(1);
	return(0);
}

int BJX2_DbgPrintRegs(BJX2_Context *ctx)
{
	int i, j;

	for(i=0; i<(128/2); i++)
	{
		if(!BJX2_DbgPrintRegs_KnownReg2(ctx, i*2))
			continue;
		
		if((i>=16) && !(ctx->regs[i*2+0]) && !(ctx->regs[i*2+1]))
			continue;
	
		if(((i*2)>=BJX2_REG_R32) && ((i*2)<BJX2_REG_R63))
		{
			printf("X%2d=%08X_%08X_%08X_%08X",
				i*2,
				(u32)(ctx->regs[i*2+1]>>32),
				(u32)(ctx->regs[i*2+1]    ),
				(u32)(ctx->regs[i*2+0]>>32),
				(u32)(ctx->regs[i*2+0]    )
				);
			printf("\n");
			continue;
		}
	
		for(j=0; j<2; j++)
		{
			if(j)printf("  ");
			printf("%-4s=%08X_%08X",
				BJX2_DbgPrintNameForReg(ctx, i*2+j),
				(u32)(ctx->regs[i*2+j]>>32),
				(u32)(ctx->regs[i*2+j]    ));
		}
		printf("\n");
	}
	return(0);
}

int BJX2_DbgPrintFpRegs(BJX2_Context *ctx)
{
	int i, j;

	for(i=0; i<16; i++)
	{
		if(ctx->fpreg[i]!=0)
			break;
	}
	if(i>=16)
		return(0);

	for(i=0; i<(16/2); i++)
	{
//		if(!BJX2_DbgPrintRegs_KnownReg2(ctx, i*2))
//			continue;
	
		for(j=0; j<2; j++)
		{
			if(j)printf("  ");
			printf("%-4s=%08X_%08X",
				BJX2_DbgPrintNameForFReg(ctx, i*2+j),
				(u32)(ctx->fpreg[i*2+j]>>32),
				(u32)(ctx->fpreg[i*2+j]    ));
		}
		printf("\n");
	}
	return(0);
}

int BJX2_DbgTopTraces(BJX2_Context *ctx)
{
	BJX2_Trace *tra[65536];
	s64 cyc_nmid[256];
	int idx_nmid[256];
	BJX2_Trace *trcur;
	bjx2_addr ba2;
	s64 cyc;
	double pcnt;
	char *bn2;
	int trn, trtops;
	int i, j, k;

	for(i=0; i<256; i++)
	{
		cyc_nmid[i]=0;
	}

	trn=0; trtops=0;
	for(i=0; i<1024; i++)
	{
		trcur=ctx->trhash[i];
		while(trcur && (trn<65536))
		{
			tra[trn++]=trcur;
			trcur=trcur->hnext;
		}
	}

	for(i=0; i<trn; i++)
	{
		trcur=tra[i];
		trtops+=trcur->n_ops;
		for(j=0; j<trcur->n_ops; j++)
		{
			cyc_nmid[trcur->ops[j]->nmid]+=
				trcur->ops[j]->cyc*tra[i]->runcnt;
		}
	}
	
	for(i=0; i<trn; i++)
		for(j=i+1; j<trn; j++)
	{
//		if(tra[j]->runcnt>tra[i]->runcnt)
		if(	(tra[j]->runcnt*tra[j]->n_cyc+tra[j]->acc_pencyc) >
			(tra[i]->runcnt*tra[i]->n_cyc+tra[i]->acc_pencyc))
		{
			trcur=tra[i];
			tra[i]=tra[j];
			tra[j]=trcur;
		}
	}

	trcur=tra[0];
	if(!trcur->runcnt)
	{
		/* If we don't have a run count here; skip further analysis. */
		return(0);
	}
	
	printf("Top Traces:\n");

	for(i=0; i<48; i++)
	{
		trcur=tra[63-i];
		bn2=BJX2_DbgNameForAddr(ctx, trcur->addr, &ba2);

		cyc=trcur->runcnt*trcur->n_cyc+trcur->acc_pencyc;
		pcnt=(100.0*cyc)/(ctx->tot_cyc);

		if(bn2)
		{
			printf("PC @ %04X_%08X (%-20s+%04X) "
				"Cyc=%4lldM(p=%lldM %.2f%%) %.2f%%\n",
				(u32)(trcur->addr>>32), 
				(u32)(trcur->addr), bn2,
				(int)(trcur->addr-ba2),
				cyc>>20, trcur->acc_pencyc>>20,
				(100.0*trcur->acc_pencyc)/cyc,
				pcnt);
		}else
		{
			printf("PC @ %04X_%08X Cyc=%5lldM %.2f%%\n",
				(u32)(trcur->addr>>32), 
				(u32)(trcur->addr), cyc>>20, pcnt);
		}
	}

	for(i=0; i<16; i++)
	{
		trcur=tra[15-i];
//		trcur=tra[i];
		bn2=BJX2_DbgNameForAddr(ctx, trcur->addr, &ba2);

		cyc=trcur->runcnt*trcur->n_cyc+trcur->acc_pencyc;
		pcnt=(100.0*cyc)/(ctx->tot_cyc);

		if(bn2)
		{
			printf("PC @ %04X_%08X (%-20s+%04X) "
				"Cyc=%4lldM(p=%lldM %.2f%%) %.2f%%\n",
				(u32)(trcur->addr>>32), 
				(u32)(trcur->addr), bn2,
				(int)(trcur->addr-ba2),
				cyc>>20, trcur->acc_pencyc>>20,
				(100.0*trcur->acc_pencyc)/cyc,
				pcnt);
		}else
		{
			printf("PC @ %04X_%08X Cyc=%5lldM %.2f%%\n",
				(u32)(trcur->addr>>32), 
				(u32)(trcur->addr), cyc>>20, pcnt);
		}

		BJX2_DbgPrintTraceOps(ctx, trcur);

#if 0
		for(j=0; j<trcur->n_ops; j++)
		{
			BJX2_DbgPrintOp(ctx, trcur->ops[j]);
		}
#endif
	}

	for(i=0; i<256; i++)
		idx_nmid[i]=i;

	for(i=0; i<256; i++)
		for(j=i+1; j<256; j++)
	{
		if(cyc_nmid[idx_nmid[j]]>cyc_nmid[idx_nmid[i]])
		{
			k=idx_nmid[i];
			idx_nmid[i]=idx_nmid[j];
			idx_nmid[j]=k;
		}
	}

#if 0
	printf("Cyc/Nmid:\n");
	for(i=0; i<64; i++)
	{
		printf("%2X ", i*2);
		for(j=0; j<2; j++)
		{
			printf("%8s(%10lld) ",
				BJX2_DbgPrintNameForNmid(ctx, i*2+j),
				cyc_nmid[i*2+j]);
		}
		printf("\n");
	}
#endif

#if 1
	printf("Cyc/Nmid:\n");
//	for(i=0; i<64; i++)
	for(i=0; i<128; i++)
	{
		k=idx_nmid[i*2];
		if(!cyc_nmid[k])
			break;

//		printf("%2X ", i*2);
		for(j=0; j<2; j++)
		{
			k=idx_nmid[i*2+j];
			pcnt=(100.0*cyc_nmid[k])/(ctx->tot_cyc);

			printf("%12s(%11lld) %2d.%02d%%  ",
				BJX2_DbgPrintNameForNmid(ctx, k),
				cyc_nmid[k], (int)pcnt, ((int)(pcnt*100))%100);
		}
		printf("\n");
	}
#endif

	pcnt=((double)trtops)/trn;
	printf("Average Trace Length: %.2f\n", pcnt);

	if(ctx->tot_cyc_mem>0)
	{
		pcnt=(100.0*ctx->tot_cyc_mem)/(ctx->tot_cyc);
		printf("Cycles Spent, Mem Op: %.2f%%\n", pcnt);
		pcnt=(100.0*(ctx->tot_cyc_mem+ctx->tot_cyc_miss))/(ctx->tot_cyc);
		printf("Cycles Spent, Mem Tot: %.2f%%\n", pcnt);


		pcnt=(100.0*ctx->tot_cyc_miss)/(ctx->tot_cyc);
		printf("Cycles Spent, Cache Miss: %.2f%%\n", pcnt);
		pcnt=(100.0*ctx->tot_cyc_miss_l1)/(ctx->tot_cyc);
		printf("Cycles Spent, Cache Miss L1: %.2f%%\n", pcnt);
		pcnt=(100.0*ctx->tot_cyc_miss_l2)/(ctx->tot_cyc);
		printf("Cycles Spent, Cache Miss L2: %.2f%%\n", pcnt);

		pcnt=(100.0*ctx->tot_cyc_mmio)/(ctx->tot_cyc);
		printf("Cycles Spent, MMIO: %.2f%%\n", pcnt);

		pcnt=(100.0*(ctx->tot_cnt_mem_l1i-ctx->tot_cnt_miss_l1i))/
			(ctx->tot_cnt_mem_l1i);
		printf("Total Count, Cache Hit  L1 I$: %.2f%%\n", pcnt);
		pcnt=(100.0*ctx->tot_cnt_miss_l1i)/(ctx->tot_cnt_mem_l1i);
		printf("Total Count, Cache Miss L1 I$: %.2f%%\n", pcnt);

		pcnt=(100.0*(ctx->tot_cnt_mem_l1-ctx->tot_cnt_miss_l1))/
			(ctx->tot_cnt_mem_l1);
		printf("Total Count, Cache Hit  L1 D$: %.2f%%\n", pcnt);
		pcnt=(100.0*ctx->tot_cnt_miss_l1)/(ctx->tot_cnt_mem_l1);
		printf("Total Count, Cache Miss L1 D$: %.2f%%\n", pcnt);

		pcnt=(100.0*(ctx->tot_cnt_mem_l2-ctx->tot_cnt_miss_l2))/
			(ctx->tot_cnt_mem_l2);
		printf("Total Count, Cache Hit  L2   : %.2f%%\n", pcnt);
		pcnt=(100.0*ctx->tot_cnt_miss_l2)/(ctx->tot_cnt_mem_l2);
		printf("Total Count, Cache Miss L2   : %.2f%%\n", pcnt);

		pcnt=(100.0*(ctx->tot_cnt_mem_l1i-ctx->tot_cnt_mem_dri))/
			(ctx->tot_cnt_mem_l1i);
		printf("Total Count, Cache Hit Combined I$: %.2f%%\n", pcnt);
		pcnt=(100.0*(ctx->tot_cnt_mem_l1-ctx->tot_cnt_mem_drd))/
			(ctx->tot_cnt_mem_l1);
		printf("Total Count, Cache Hit Combined D$: %.2f%%\n", pcnt);
	}

	return(0);
}


int BJX2_DbgDump(BJX2_Context *ctx)
{
	BJX2_Trace *cur;
	bjx2_addr pc;
	int i;

//	if(!ctx->use_jit)
	if(ctx->pclogrov<128)
	{
		for(i=0; i<32; i++)
		{
			pc=ctx->pclog[(ctx->pclogrov-32+i)&63];
			ctx->trapc=pc;
			cur=BJX2_GetTraceForAddr(ctx, pc);
			BJX2_DbgPrintTrace(ctx, cur);
		}
	}

	BJX2_DbgPrintRegs(ctx);
	BJX2_DbgPrintFpRegs(ctx);
	
//		if(ctx->use_jit)

	if(!ctx->status && !ctx->use_jit)
	{
		BJX2_DbgTopTraces(ctx);
	}
	
	if(ctx->bpr_cnt>0)
	{
		printf("Branch Pred Hit %.2f%%\n",
			(100.0*ctx->bpr_hit)/ctx->bpr_cnt);
	}
	
	return(0);
}

int BJX2_UpdateForStatus(BJX2_Context *ctx)
{
	if(ctx->status==BJX2_FLT_SLEEP)
	{
		ctx->status=0;
		return(0);
	}
	if(ctx->status==BJX2_FLT_BREAK)
		return(0);

	if(ctx->status==BJX2_FLT_PCMISH)
		return(0);

	if(ctx->status==BJX2_FLT_INVOP)
		return(0);

	if(ctx->status==BJX2_FLT_SCRPOKE)
	{
		ctx->status=0;
		return(0);
	}

	if(ctx->status==BJX2_FLT_CCFLUSH)
	{
		if(ctx->cc_flush&1)
		{
			BJX2_DecodeTraceFlushCache(ctx);
		}

		ctx->status=0;
		return(0);
	}

	if(BJX2_FaultEnterInterrupt(ctx)>0)
		return(1);
		
	if((ctx->status&0xF000)==0xC000)
	{
		printf("BJX2_UpdateForStatus: Missed IRQ %04X\n", ctx->status);
		ctx->status=0;
		return(1);
	}

	return(0);
}

int BJX2_RunLimit(BJX2_Context *ctx, int lim)
{
	BJX2_Trace *cur;
	bjx2_addr pc;
	int cn, cn0, cn1, nc, no;
	int i, j, k;

	cn=lim;

	BJX2_MemSetupState(ctx);

	if(ctx->status)
	{
		if(!BJX2_UpdateForStatus(ctx))
			return(ctx->status);
	}

	while(cn>0)
	{
		cur=ctx->trcur;
		if(!cur)
		{
			pc=ctx->regs[BJX2_REG_PC];
			
			if(!pc && ctx->tot_cyc>1000)
				break;
			if(pc!=(pc&0x0000FFFFFFFFFFFFULL))
			{
				ctx->status=0x9999;
				break;
			}
			
			ctx->trapc=pc;
			cur=BJX2_GetTraceForAddr(ctx, pc);

			if(ctx->status)
			{
				if(BJX2_UpdateForStatus(ctx))
					continue;
			}
		}

//		if(cur && (cur->addr!=ctx->regs[BJX2_REG_PC]))
//			JX2_DBGBREAK
		
		cn1=cn;
		if(cn1>=ctx->ttick_hk)
			cn1=ctx->ttick_hk;
//		if(cn1>=2048)
//			cn1=2048;
		if((ctx->iodel_cyc>0) && (cn1>=ctx->iodel_cyc))
			cn1=ctx->iodel_cyc;
		cn0=cn1;
		
		no=0;
		
//		if(ctx->tgt_mhz>300)
//		if(ctx->use_jit)
		if(0)
		{
			ctx->pclogrov=128;
			while(cur && (cn1>0))
			{
				nc=cur->n_cyc;
				cn1-=nc;
				no+=cur->n_ops;
				cur=cur->Run(ctx, cur);
			}
		}
		else
		{
			while(cur && (cn1>0))
			{
				ctx->pclog[ctx->pclogrov]=cur->addr;
				ctx->pclogrov=
					(ctx->pclogrov+1)&63;
		//		BJX2_DbgPrintTrace(ctx, cur);
				nc=cur->n_cyc;
				cn1-=nc;
				no+=cur->n_ops;

				cur=cur->Run(ctx, cur);
			}
		}

#if 0
		if(cur && (cur->addr!=ctx->regs[BJX2_REG_PC]))
		{
			ctx->status=BJX2_FLT_PCMISH;
			ctx->trcur=cur;
//			JX2_DBGBREAK
			break;
		}
#endif


		nc=(cn0-cn1);

//		nc+=ctx->iodel_cyc;
//		ctx->iodel_cyc=0;

#ifdef X86_64
		ctx->tot_cyc_mem+=ctx->mem_cyc;
		ctx->tot_cyc_miss+=ctx->miss_cyc;
		ctx->tot_cyc_miss_l1+=ctx->miss_cyc_l1;
		ctx->tot_cyc_miss_l2+=ctx->miss_cyc_l2;
		ctx->tot_cyc_mmio+=ctx->mem_cyc_mmio;

		ctx->tot_cnt_mem_l1+=ctx->mem_cnt_l1;
		ctx->tot_cnt_mem_l1i+=ctx->mem_cnt_l1i;
		ctx->tot_cnt_mem_l2+=ctx->mem_cnt_l2;
		ctx->tot_cnt_miss_l1+=ctx->miss_cnt_l1;
		ctx->tot_cnt_miss_l1i+=ctx->miss_cnt_l1i;
		ctx->tot_cnt_miss_l2+=ctx->miss_cnt_l2;
		ctx->tot_cnt_mem_dri+=ctx->mem_cnt_dri;
		ctx->tot_cnt_mem_drd+=ctx->mem_cnt_drd;

		nc+=ctx->miss_cyc;
		ctx->mem_cyc=0;
		ctx->miss_cyc=0;
		ctx->miss_cyc_l1=0;
		ctx->miss_cyc_l2=0;
		ctx->mem_cyc_mmio=0;

		ctx->mem_cnt_l1=0;
		ctx->mem_cnt_l2=0;
		ctx->mem_cnt_l1i=0;
		ctx->miss_cnt_l1=0;
		ctx->miss_cnt_l2=0;
		ctx->miss_cnt_l1i=0;
		ctx->mem_cnt_dri=0;
		ctx->mem_cnt_drd=0;
#endif

		ctx->tot_cyc+=nc;
		ctx->tot_ops+=no;
		ctx->ttick_hk-=nc;
		cn-=nc;

		ctx->trcur=cur;

		if(ctx->ttick_hk<=0)
		{
			pc=ctx->regs[BJX2_REG_PC];
			ctx->trapc=pc;

			ctx->nttick_irq++;
			BJX2_ThrowFaultStatus(ctx, BJX2_FLT_TIMER);
			while(ctx->ttick_hk<=0)
				ctx->ttick_hk+=ctx->ttick_rst;
		}

		if(ctx->status)
		{
			if(BJX2_UpdateForStatus(ctx))
				continue;
			break;
		}
	}

	return(ctx->status);
}
