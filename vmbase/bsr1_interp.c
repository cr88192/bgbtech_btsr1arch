BTSR1_Context *BTSR1_AllocContext(void)
{
	BTSR1_Context *tmp;
	
	tmp=malloc(sizeof(BTSR1_Context));
	memset(tmp, 0, sizeof(BTSR1_Context));
	return(tmp);
}

BTSR1_Opcode *BTSR1_ContextAllocOpcode(BTSR1_Context *ctx)
{
	BTSR1_Opcode *tmp;
	int i;
	
	tmp=ctx->free_op;
	if(tmp)
	{
		ctx->free_op=*(BTSR1_Opcode **)tmp;
		memset(tmp, 0, sizeof(BTSR1_Opcode));
		return(tmp);
	}
	
	tmp=malloc(256*sizeof(BTSR1_Opcode));
	for(i=0; i<255; i++)
	{
		*(BTSR1_Opcode **)tmp=ctx->free_op;
		ctx->free_op=tmp;
		tmp++;
	}

	memset(tmp, 0, sizeof(BTSR1_Opcode));
	return(tmp);
}

BTSR1_Trace *BTSR1_ContextAllocTrace(BTSR1_Context *ctx)
{
	BTSR1_Trace *tmp;
	int i;
	
	tmp=ctx->free_tr;
	if(tmp)
	{
		ctx->free_tr=*(BTSR1_Trace **)tmp;
		memset(tmp, 0, sizeof(BTSR1_Trace));
		return(tmp);
	}
	
	tmp=malloc(256*sizeof(BTSR1_Trace));
	for(i=0; i<255; i++)
	{
		*(BTSR1_Trace **)tmp=ctx->free_tr;
		ctx->free_tr=tmp;
		tmp++;
	}

	memset(tmp, 0, sizeof(BTSR1_Trace));
	return(tmp);
}

void BTSR1_ContextFreeOpcode(BTSR1_Context *ctx, BTSR1_Opcode *tmp)
{
	*(BTSR1_Opcode **)tmp=ctx->free_op;
	ctx->free_op=tmp;
}

void BTSR1_ContextFreeTrace(BTSR1_Context *ctx, BTSR1_Trace *tmp)
{
	*(BTSR1_Trace **)tmp=ctx->free_tr;
	ctx->free_tr=tmp;
}

BTSR1_Trace *BTSR1_GetTraceForAddr(BTSR1_Context *ctx, btsr1_addr addr)
{
	BTSR1_Trace *cur, *prv;
	int h;

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
	h=((addr*65521)>>16)&1023;
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
	
	cur=BTSR1_ContextAllocTrace(ctx);
	cur->addr=addr;
	cur->hnext=ctx->trhash[h];
	ctx->trhash[h]=cur;

	BTSR1_DecodeTraceForAddr(ctx, cur, addr);

	return(cur);
}

int BTSR1_ThrowFaultStatus(BTSR1_Context *ctx, int status)
{
	ctx->regs[BTSR1_REG_EXSR]=status;
	ctx->tr_rnxt=NULL;
	ctx->tr_rjmp=NULL;
	ctx->status=status;
	return(0);
}

int BTSR1_FaultSwapRegs(BTSR1_Context *ctx)
{
	u32 va, vb;
	int i;

	for(i=0; i<8; i++)
	{
		va=ctx->regs[BTSR1_REG_R0+i];
		vb=ctx->regs[BTSR1_REG_R0B+i];
		ctx->regs[BTSR1_REG_R0+i]=vb;
		ctx->regs[BTSR1_REG_R0B+i]=va;
	}

	for(i=0; i<8; i++)
	{
		if(i==3)
			continue;
		va=ctx->regs[BTSR1_REG_PC+i];
		vb=ctx->regs[BTSR1_REG_SPC+i];
		ctx->regs[BTSR1_REG_PC+i]=vb;
		ctx->regs[BTSR1_REG_SPC+i]=va;
	}

	va=ctx->regs[BTSR1_REG_SP];
	vb=ctx->regs[BTSR1_REG_SSP];
	ctx->regs[BTSR1_REG_SP]=vb;
	ctx->regs[BTSR1_REG_SSP]=va;
	
	return(0);
}

int BTSR1_FaultEnterInterrupt(BTSR1_Context *ctx)
{
	u32 vbr, exsr, pc1;
	if(ctx->regs[BTSR1_REG_SR]&(1<<28))
	{
		return(0);
	}

	if(!(ctx->regs[BTSR1_REG_SR]&(1<<29)))
	{
		BTSR1_FaultSwapRegs(ctx);
		ctx->regs[BTSR1_REG_SR]|=(1<<29);	//Set RB
	}

	ctx->regs[BTSR1_REG_SR]|=(1<<28);	//BL
	ctx->regs[BTSR1_REG_SR]|=(1<<30);	//MD
	
	vbr=ctx->regs[BTSR1_REG_VBR];
	exsr=ctx->regs[BTSR1_REG_EXSR];
	if((exsr&0xF000)==0xC000)
	{
		pc1=vbr+0x10;
	}else
	{
		pc1=vbr+0x08;
	}
	ctx->regs[BTSR1_REG_PC]=pc1;
	ctx->trcur=NULL;

	ctx->status=0;
	return(1);
}

int BTSR1_FaultLeaveInterrupt(BTSR1_Context *ctx)
{
	if(ctx->regs[BTSR1_REG_SR]&(1<<29))
	{
		BTSR1_FaultSwapRegs(ctx);
		ctx->regs[BTSR1_REG_SR]&=~(1<<29);	//Clear RB
	}
	ctx->regs[BTSR1_REG_SR]&=~(1<<29);	//Clear RB
	ctx->trcur=NULL;
	ctx->tr_rnxt=NULL;
	ctx->tr_rjmp=NULL;
	return(0);
}

char *BTSR1_DbgPrintNameForNmid(BTSR1_Context *ctx, int nmid)
{
	static char tb[128];
	char *s0;
	
	s0=NULL;
	switch(nmid)
	{
	case BTSR1_NMID_NONE:		s0="none";		break;
	case BTSR1_NMID_MOVB:		s0="MOV.B";		break;
	case BTSR1_NMID_MOVW:		s0="MOV.W";		break;
	case BTSR1_NMID_MOVD:		s0="MOV.L";		break;
	case BTSR1_NMID_MOVQ:		s0="MOV.Q";		break;
	case BTSR1_NMID_MOVUB:		s0="MOVU.B";	break;
	case BTSR1_NMID_MOVUW:		s0="MOVU.W";	break;
	case BTSR1_NMID_NOP:		s0="NOP";		break;
	case BTSR1_NMID_BRA:		s0="BRA";		break;
	case BTSR1_NMID_BSR:		s0="BSR";		break;
	case BTSR1_NMID_BT:			s0="BT";		break;
	case BTSR1_NMID_BF:			s0="BF";		break;
	case BTSR1_NMID_LEAB:		s0="LEA.B";		break;
	case BTSR1_NMID_LEAW:		s0="LEA.W";		break;
	case BTSR1_NMID_LEAD:		s0="LEA.L";		break;
	case BTSR1_NMID_LEAQ:		s0="LEA.Q";		break;
	case BTSR1_NMID_ADD:		s0="ADD";		break;
	case BTSR1_NMID_SUB:		s0="SUB";		break;
	case BTSR1_NMID_ADC:		s0="ADC";		break;
	case BTSR1_NMID_SBB:		s0="SBB";		break;
	case BTSR1_NMID_TST:		s0="TST";		break;
	case BTSR1_NMID_AND:		s0="AND";		break;
	case BTSR1_NMID_OR:			s0="OR";		break;
	case BTSR1_NMID_XOR:		s0="XOR";		break;
	case BTSR1_NMID_MOV:		s0="MOV";		break;
	case BTSR1_NMID_LDIZ:		s0="LDIZ";		break;
	case BTSR1_NMID_LDIN:		s0="LDIN";		break;
	case BTSR1_NMID_LDISH:		s0="LDISH";		break;
	case BTSR1_NMID_CMPEQ:		s0="CMPEQ";		break;
	case BTSR1_NMID_CMPHI:		s0="CMPHI";		break;
	case BTSR1_NMID_CMPGT:		s0="CMPGT";		break;
	case BTSR1_NMID_RTS:		s0="RTS";		break;
	case BTSR1_NMID_EXTUB:		s0="EXTU.B";	break;
	case BTSR1_NMID_EXTUW:		s0="EXTU.W";	break;
	case BTSR1_NMID_EXTSB:		s0="EXTS.B";	break;
	case BTSR1_NMID_EXTSW:		s0="EXTS.W";	break;
	case BTSR1_NMID_CLRT:		s0="CLRT";		break;
	case BTSR1_NMID_SETT:		s0="SETT";		break;
	case BTSR1_NMID_CLRS:		s0="CLRS";		break;
	case BTSR1_NMID_SETS:		s0="SETS";		break;
	case BTSR1_NMID_NOTT:		s0="NOTT";		break;
	case BTSR1_NMID_NOTS:		s0="NOTS";		break;
	case BTSR1_NMID_LDISH16:	s0="LDISH16";	break;
	case BTSR1_NMID_CMPGE:		s0="CMPGE";		break;
	case BTSR1_NMID_PUSH:		s0="PUSH";		break;
	case BTSR1_NMID_POP:		s0="POP";		break;
	case BTSR1_NMID_CMPPL:		s0="CMPPL";		break;
	case BTSR1_NMID_CMPPZ:		s0="CMPPZ";		break;
	case BTSR1_NMID_SHLL:		s0="SHLL";		break;
	case BTSR1_NMID_SHLR:		s0="SHLR";		break;
	case BTSR1_NMID_SHAR:		s0="SHAR";		break;
	case BTSR1_NMID_CMPHS:		s0="CMPHS";		break;
	case BTSR1_NMID_BREAK:		s0="BREAK";		break;
	case BTSR1_NMID_SLEEP:		s0="SLEEP";		break;
	case BTSR1_NMID_MULS:		s0="MULS";		break;
	case BTSR1_NMID_MULU:		s0="MULU";		break;
	case BTSR1_NMID_RTE:		s0="RTE";		break;
	case BTSR1_NMID_NOT:		s0="NOT";		break;
	case BTSR1_NMID_NEG:		s0="NEG";		break;
	case BTSR1_NMID_NEGC:		s0="NEGC";		break;
	case BTSR1_NMID_ROTL:		s0="ROTL";		break;
	case BTSR1_NMID_ROTR:		s0="ROTR";		break;
	case BTSR1_NMID_ROTCL:		s0="ROTCL";		break;
	case BTSR1_NMID_ROTCR:		s0="ROTCR";		break;
	case BTSR1_NMID_RET:		s0="RET";		break;

	default:
		sprintf(tb, "?NM%02X", nmid);
		s0=tb;
		break;
	}
	
	return(s0);
}

char *BTSR1_DbgPrintNameForReg(BTSR1_Context *ctx, int reg)
{
	char *s;

	switch(reg)
	{
	case BTSR1_REG_R0:		s="R0";		break;
	case BTSR1_REG_R1:		s="R1";		break;
	case BTSR1_REG_R2:		s="R2";		break;
	case BTSR1_REG_R3:		s="R3";		break;
	case BTSR1_REG_R4:		s="R4";		break;
	case BTSR1_REG_R5:		s="R5";		break;
	case BTSR1_REG_R6:		s="R6";		break;
	case BTSR1_REG_R7:		s="R7";		break;
	case BTSR1_REG_R8:		s="R8";		break;
	case BTSR1_REG_R9:		s="R9";		break;
	case BTSR1_REG_R10:		s="R10";	break;
	case BTSR1_REG_R11:		s="R11";	break;
	case BTSR1_REG_R12:		s="R12";	break;
	case BTSR1_REG_R13:		s="R13";	break;
	case BTSR1_REG_R14:		s="R14";	break;
	case BTSR1_REG_R15:		s="SP";		break;

	case BTSR1_REG_PC:		s="PC";		break;
	case BTSR1_REG_LR:		s="LR";		break;
	case BTSR1_REG_SR:		s="SR";		break;
	case BTSR1_REG_VBR:		s="VBR";	break;
	case BTSR1_REG_DLR:		s="DLR";	break;
	case BTSR1_REG_DHR:		s="DHR";	break;
	case BTSR1_REG_GBR:		s="GBR";	break;
	case BTSR1_REG_TBR:		s="TBR";	break;

	case BTSR1_REG_TTB:		s="TTB";	break;
	case BTSR1_REG_TEA:		s="TEA";	break;
	case BTSR1_REG_MMCR:	s="MMCR";	break;
	case BTSR1_REG_EXSR:	s="EXSR";	break;


	case BTSR1_REG_R0B:		s="R0B";	break;
	case BTSR1_REG_R1B:		s="R1B";	break;
	case BTSR1_REG_R2B:		s="R2B";	break;
	case BTSR1_REG_R3B:		s="R3B";	break;
	case BTSR1_REG_R4B:		s="R4B";	break;
	case BTSR1_REG_R5B:		s="R5B";	break;
	case BTSR1_REG_R6B:		s="R6B";	break;
	case BTSR1_REG_R7B:		s="R7B";	break;

	case BTSR1_REG_SPC:		s="SPC";	break;
	case BTSR1_REG_SLR:		s="SLR";	break;
	case BTSR1_REG_SSR:		s="SSR";	break;
	case BTSR1_REG_SSP:		s="SSP";	break;
	case BTSR1_REG_SDL:		s="SDL";	break;
	case BTSR1_REG_SDH:		s="SDH";	break;
	case BTSR1_REG_SGB:		s="SGB";	break;
	case BTSR1_REG_STB:		s="STB";	break;

	default:				s="?";		break;
	}
	
	return(s);
}

int BTSR1_DbgPrintOp(BTSR1_Context *ctx, BTSR1_Opcode *op)
{
	int msc;

	printf("%05X  %04X %-8s ", op->pc, op->opn,
		BTSR1_DbgPrintNameForNmid(ctx, op->nmid));
	msc=1;

	switch(op->fmid)
	{
	case BTSR1_FMID_NONE:
		printf("?");
		break;
	case BTSR1_FMID_REG:
		printf("%s", BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGREG:
		printf("%s, %s",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_Z:
		break;
	case BTSR1_FMID_IMM:
		printf("#%d", op->imm);
		break;
	case BTSR1_FMID_PCDISP:
		printf("(PC, %d)", op->imm);
		break;
	case BTSR1_FMID_LDREGREG:
		printf("(%s), %s",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGSTREG:
		printf("%s, (%s)",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_LDDRREGREG:
		printf("(%s, DLR), %s",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGSTDRREG:
		printf("%s, (%s, DLR)",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_LDREGDISPREG:
		printf("(%s, %d), %s",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			(op->imm*msc),
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGSTREGDISP:
		printf("%s, (%s, %d)",
			BTSR1_DbgPrintNameForReg(ctx, op->rm),
			BTSR1_DbgPrintNameForReg(ctx, op->rn),
			(op->imm*msc));
		break;

	case BTSR1_FMID_PCDR:
		printf("(PC, DLR)");
		break;

	case BTSR1_FMID_LDDRPCREG:
		printf("(PC, DLR), %s",
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGSTDRPC:
		printf("%s, (PC, DLR)",
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_IMMREG:
		printf("#%d, %s", op->imm,
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;

	case BTSR1_FMID_LDDRABSREG:
		printf("(DLR), %s",
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGSTDRABS:
		printf("%s, (DLR)",
			BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;

	case BTSR1_FMID_PCDR4:
		printf("(PC, DLR:%d)", op->imm);
		break;
	case BTSR1_FMID_LDDR4PCREG:
		printf("(PC, DLR:%d), %s",
			op->imm, BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;
	case BTSR1_FMID_REGSTDR4PC:
		printf("%s, (PC, DLR:%d)",
			BTSR1_DbgPrintNameForReg(ctx, op->rn), op->imm);
		break;
	case BTSR1_FMID_DR4REG:
		printf("DLR:%d, %s",
			op->imm, BTSR1_DbgPrintNameForReg(ctx, op->rn));
		break;

	default:
		printf("?");
		break;
	}

	printf("\n");
	return(0);
}

int BTSR1_DbgPrintTrace(BTSR1_Context *ctx, BTSR1_Trace *tr)
{
	int i;
	printf("PC @ %05X\n", tr->addr);
	
	for(i=0; i<tr->n_ops; i++)
	{
		BTSR1_DbgPrintOp(ctx, tr->ops[i]);
	}
	return(0);
}

int BTSR1_DbgPrintRegs(BTSR1_Context *ctx)
{
	int i, j;

	for(i=0; i<(48/4); i++)
	{
		for(j=0; j<4; j++)
		{
			if(j)printf("  ");
			printf("%-4s=%08X",
				BTSR1_DbgPrintNameForReg(ctx, i*4+j),
				ctx->regs[i*4+j]);
		}
		printf("\n");
	}
	return(0);
}

int BTSR1_DbgDump(BTSR1_Context *ctx)
{
	BTSR1_Trace *cur;
	btsr1_addr pc;
	int i;

	for(i=0; i<32; i++)
	{
		pc=ctx->pclog[(ctx->pclogrov-32+i)&63];
		cur=BTSR1_GetTraceForAddr(ctx, pc);
		BTSR1_DbgPrintTrace(ctx, cur);
	}

	BTSR1_DbgPrintRegs(ctx);
	return(0);
}

int BTSR1_UpdateForStatus(BTSR1_Context *ctx)
{
	if(ctx->status==BTSR1_FLT_SLEEP)
	{
		ctx->status=0;
		return(0);
	}
	if(ctx->status==BTSR1_FLT_BREAK)
		return(0);

	if(ctx->status==BTSR1_FLT_PCMISH)
		return(0);

	if(BTSR1_FaultEnterInterrupt(ctx)>0)
		return(1);
		
	if((ctx->status&0xF000)==0xC000)
	{
		printf("BTSR1_UpdateForStatus: Missed IRQ %04X\n", ctx->status);
		ctx->status=0;
		return(1);
	}

	return(0);
}

int BTSR1_RunLimit(BTSR1_Context *ctx, int lim)
{
	BTSR1_Trace *cur;
	btsr1_addr pc;
	int cn, cn0, cn1, nc, no;
	int i, j, k;

	cn=lim;

	if(ctx->status)
	{
		if(!BTSR1_UpdateForStatus(ctx))
			return(ctx->status);
	}

	while(cn>0)
	{
		cur=ctx->trcur;
		if(!cur)
		{
			pc=ctx->regs[BTSR1_REG_PC];
			cur=BTSR1_GetTraceForAddr(ctx, pc);
		}

//		if(cur && (cur->addr!=ctx->regs[BTSR1_REG_PC]))
//			__debugbreak();
		
		cn1=cn;
		if(cn1>=ctx->ttick_hk)
			cn1=ctx->ttick_hk;
		cn0=cn1;
		
		no=0;
		while(cur && (cn1>0))
		{
#if 0
			if(cur && (cur->addr!=ctx->regs[BTSR1_REG_PC]))
			{
				ctx->status=BTSR1_FLT_PCMISH;
//				__debugbreak();
				break;
			}
#endif

			ctx->pclog[ctx->pclogrov]=cur->addr;
			ctx->pclogrov=
				(ctx->pclogrov+1)&63;
	//		BTSR1_DbgPrintTrace(ctx, cur);
			nc=cur->n_cyc;
			cn1-=nc;
			no+=cur->n_ops;

			cur=cur->Run(ctx, cur);
		}

#if 0
		if(cur && (cur->addr!=ctx->regs[BTSR1_REG_PC]))
		{
			ctx->status=BTSR1_FLT_PCMISH;
			ctx->trcur=cur;
//			__debugbreak();
			break;
		}
#endif


		nc=(cn0-cn1);
		ctx->tot_cyc+=nc;
		ctx->tot_ops+=no;
		ctx->ttick_hk-=nc;
		cn-=nc;

		ctx->trcur=cur;

		if(ctx->ttick_hk<=0)
		{
			BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_TIMER);
			while(ctx->ttick_hk<=0)
				ctx->ttick_hk+=ctx->ttick_rst;
		}

		if(ctx->status)
		{
			if(BTSR1_UpdateForStatus(ctx))
				continue;
			break;
		}
	}

	return(ctx->status);
}
