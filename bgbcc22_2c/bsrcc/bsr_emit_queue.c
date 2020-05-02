BGBCC_BSR_EmitQueueOp *BGBCC_BSR_AllocQueueOp(
	BGBCC_BSR_Context *ctx)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	int i;
	
	tmp=ctx->eqfree;
	if(tmp)
	{
		ctx->eqfree=tmp->next;
		memset(tmp, 0, sizeof(BGBCC_BSR_EmitQueueOp));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(64*sizeof(BGBCC_BSR_EmitQueueOp));
	for(i=0; i<63; i++)
	{
		tmp->next=ctx->eqfree;
		ctx->eqfree=tmp;
		tmp++;
	}
	return(tmp);
}

int BGBCC_BSR_FreeQueueOp(
	BGBCC_BSR_Context *ctx, BGBCC_BSR_EmitQueueOp *tmp)
{
	tmp->next=ctx->eqfree;
	ctx->eqfree=tmp;
	return(0);
}

int BGBCC_BSR_AddQueueOp(
	BGBCC_BSR_Context *ctx, BGBCC_BSR_EmitQueueOp *tmp)
{
	if(ctx->eqstrt && ctx->eqend)
	{
		tmp->next=NULL;
		ctx->eqend->next=tmp;
		ctx->eqend=tmp;
	}else
	{
		tmp->next=NULL;
		ctx->eqstrt=tmp;
		ctx->eqend=tmp;
	}
	return(0);
}

int BGBCC_BSR_EmitQueueOpBasic(
	BGBCC_BSR_Context *ctx,
	BGBCC_BSR_EmitQueueOp *op)
{
	switch(op->fmid)
	{
	case BGBCC_SH_FMID_NONE:
		BGBCC_BSR_EmitOpNone(ctx, op->nmid);
		break;

	case BGBCC_SH_FMID_REGRM:
		BGBCC_BSR_EmitOpReg(ctx,
			op->nmid, op->rm);
		break;
	case BGBCC_SH_FMID_REGRN:
		BGBCC_BSR_EmitOpReg(ctx,
			op->nmid, op->rn);
		break;

	case BGBCC_SH_FMID_REGREG:
		BGBCC_BSR_EmitOpRegReg(ctx,
			op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGIMM:
		BGBCC_BSR_EmitOpImmReg(ctx,
			op->nmid, op->imm, op->rn);
		break;

	case BGBCC_SH_FMID_REGST:
		BGBCC_BSR_EmitOpRegStReg(ctx, op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGLD:
		BGBCC_BSR_EmitOpLdRegReg(ctx, op->nmid, op->rm, op->rn);
		break;

	case BGBCC_SH_FMID_REGDECST:
		BGBCC_BSR_EmitOpRegStDecReg(ctx, op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGINCLD:
		BGBCC_BSR_EmitOpLdIncRegReg(ctx, op->nmid, op->rm, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTR0N:
		BGBCC_BSR_EmitOpRegStReg2(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;
	case BGBCC_SH_FMID_REGLDR0M:
		BGBCC_BSR_EmitOpLdReg2Reg(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTDISP:
		BGBCC_BSR_EmitOpRegStRegDisp(ctx, op->nmid, op->rm, op->rn, op->imm);
		break;
	case BGBCC_SH_FMID_REGLDDISP:
		BGBCC_BSR_EmitOpLdRegDispReg(ctx, op->nmid, op->rm, op->imm, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTRODISP:
		BGBCC_BSR_EmitOpRegStReg2Disp(ctx, op->nmid,
			op->rm, op->rn, op->ro, op->imm);
		break;
	case BGBCC_SH_FMID_REGLDRODISP:
		BGBCC_BSR_EmitOpLdReg2DispReg(ctx, op->nmid,
			op->rm, op->ro, op->imm, op->rn);
		break;


	case BGBCC_SH_FMID_IMM:
		BGBCC_BSR_EmitOpImm(ctx, op->nmid, op->imm);
		break;
	
	case BGBCC_SH_FMID_REGREGREG:
		BGBCC_BSR_EmitOpRegRegReg(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;
	case BGBCC_SH_FMID_REGIMMREG:
		BGBCC_BSR_EmitOpRegImmReg(ctx, op->nmid, op->rm, op->imm, op->rn);
		break;
	}
	return(0);
}

int BGBCC_BSR_EmitQueueOps(BGBCC_BSR_Context *ctx)
{
	BGBCC_BSR_EmitQueueOp *cur, *nxt;
	
	cur=ctx->eqstrt;
	ctx->eqstrt=NULL;
	ctx->eqend=NULL;
	while(cur)
	{
		nxt=cur->next;
		BGBCC_BSR_EmitQueueOpBasic(ctx, cur);
		BGBCC_BSR_FreeQueueOp(ctx, cur);
		cur=nxt;
	}
	return(0);
}

int BGBCC_BSR_QueueOpNone(BGBCC_BSR_Context *ctx, int nmid)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_NONE;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGRN;
	tmp->rn=reg;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpImm(BGBCC_BSR_Context *ctx, int nmid, int imm)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_IMM;
	tmp->imm=imm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpMReg(BGBCC_BSR_Context *ctx, int nmid, int reg)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_STREG;
	tmp->rn=reg;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegReg(BGBCC_BSR_Context *ctx, int nmid, int rm, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGREG;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegStReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGST;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpLdRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLD;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegStDecReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGDECST;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpLdIncRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGINCLD;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegStRegDisp(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTDISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=disp;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpLdRegDispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDDISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=disp;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegStReg2(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTR0N;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpLdReg2Reg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDR0M;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegStReg2Disp(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTRODISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	tmp->imm=disp;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpLdReg2DispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDRODISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	tmp->imm=disp;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGREGREG;
	tmp->rn=rn;
	tmp->rm=rs;
	tmp->ro=rt;
//	tmp->imm=disp;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int imm, int reg)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGIMM;
	tmp->rn=reg;
	tmp->imm=imm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_BSR_QueueOpRegImmReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	BGBCC_BSR_EmitQueueOp *tmp;
	tmp=BGBCC_BSR_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGIMMREG;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=imm;
	BGBCC_BSR_AddQueueOp(ctx, tmp);
	return(0);
}
