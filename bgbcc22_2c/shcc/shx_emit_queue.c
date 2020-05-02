BGBCC_SHX_EmitQueueOp *BGBCC_SHX_AllocQueueOp(
	BGBCC_SHX_Context *ctx)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	int i;
	
	tmp=ctx->eqfree;
	if(tmp)
	{
		ctx->eqfree=tmp->next;
		memset(tmp, 0, sizeof(BGBCC_SHX_EmitQueueOp));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(64*sizeof(BGBCC_SHX_EmitQueueOp));
	for(i=0; i<63; i++)
	{
		tmp->next=ctx->eqfree;
		ctx->eqfree=tmp;
		tmp++;
	}
	return(tmp);
}

int BGBCC_SHX_FreeQueueOp(
	BGBCC_SHX_Context *ctx, BGBCC_SHX_EmitQueueOp *tmp)
{
	tmp->next=ctx->eqfree;
	ctx->eqfree=tmp;
	return(0);
}

int BGBCC_SHX_AddQueueOp(
	BGBCC_SHX_Context *ctx, BGBCC_SHX_EmitQueueOp *tmp)
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

int BGBCC_SHX_EmitQueueOpBasic(
	BGBCC_SHX_Context *ctx,
	BGBCC_SHX_EmitQueueOp *op)
{
	switch(op->fmid)
	{
	case BGBCC_SH_FMID_NONE:
		BGBCC_SHX_EmitOpNone(ctx, op->nmid);
		break;

	case BGBCC_SH_FMID_REGRM:
		BGBCC_SHX_EmitOpReg(ctx,
			op->nmid, op->rm);
		break;
	case BGBCC_SH_FMID_REGRN:
		BGBCC_SHX_EmitOpReg(ctx,
			op->nmid, op->rn);
		break;

	case BGBCC_SH_FMID_REGREG:
		BGBCC_SHX_EmitOpRegReg(ctx,
			op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGIMM:
		BGBCC_SHX_EmitOpRegImm(ctx,
			op->nmid, op->rn, op->imm);
		break;

	case BGBCC_SH_FMID_REGST:
		BGBCC_SHX_EmitOpRegStReg(ctx, op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGLD:
		BGBCC_SHX_EmitOpLdRegReg(ctx, op->nmid, op->rm, op->rn);
		break;

	case BGBCC_SH_FMID_REGDECST:
		BGBCC_SHX_EmitOpRegStDecReg(ctx, op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGINCLD:
		BGBCC_SHX_EmitOpLdIncRegReg(ctx, op->nmid, op->rm, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTR0N:
		BGBCC_SHX_EmitOpRegStReg2(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;
	case BGBCC_SH_FMID_REGLDR0M:
		BGBCC_SHX_EmitOpLdReg2Reg(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTDISP:
		BGBCC_SHX_EmitOpRegStRegDisp(ctx, op->nmid, op->rm, op->rn, op->imm);
		break;
	case BGBCC_SH_FMID_REGLDDISP:
		BGBCC_SHX_EmitOpLdRegDispReg(ctx, op->nmid, op->rm, op->imm, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTRODISP:
		BGBCC_SHX_EmitOpRegStReg2Disp(ctx, op->nmid,
			op->rm, op->rn, op->ro, op->imm);
		break;
	case BGBCC_SH_FMID_REGLDRODISP:
		BGBCC_SHX_EmitOpLdReg2DispReg(ctx, op->nmid,
			op->rm, op->ro, op->imm, op->rn);
		break;


	case BGBCC_SH_FMID_IMM:
		BGBCC_SHX_EmitOpImm(ctx, op->nmid, op->imm);
		break;
	
	case BGBCC_SH_FMID_REGREGREG:
		BGBCC_SHX_EmitOpRegRegReg(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;
	case BGBCC_SH_FMID_REGIMMREG:
		BGBCC_SHX_EmitOpRegImmReg(ctx, op->nmid, op->rm, op->imm, op->rn);
		break;
	}
	return(0);
}

int BGBCC_SHX_EmitQueueOps(BGBCC_SHX_Context *ctx)
{
	BGBCC_SHX_EmitQueueOp *cur, *nxt;
	
	cur=ctx->eqstrt;
	ctx->eqstrt=NULL;
	ctx->eqend=NULL;
	while(cur)
	{
		nxt=cur->next;
		BGBCC_SHX_EmitQueueOpBasic(ctx, cur);
		BGBCC_SHX_FreeQueueOp(ctx, cur);
		cur=nxt;
	}
	return(0);
}

int BGBCC_SHX_QueueOpNone(BGBCC_SHX_Context *ctx, int nmid)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_NONE;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGRN;
	tmp->rn=reg;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpImm(BGBCC_SHX_Context *ctx, int nmid, int imm)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_IMM;
	tmp->imm=imm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpMReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_STREG;
	tmp->rn=reg;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGREG;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGST;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLD;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGDECST;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGINCLD;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegStRegDisp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTDISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=disp;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDDISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=disp;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegStReg2(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTR0N;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpLdReg2Reg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDR0M;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegStReg2Disp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTRODISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	tmp->imm=disp;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpLdReg2DispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDRODISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	tmp->imm=disp;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGREGREG;
	tmp->rn=rn;
	tmp->rm=rs;
	tmp->ro=rt;
//	tmp->imm=disp;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegImm(BGBCC_SHX_Context *ctx,
	int nmid, int reg, int imm)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGIMM;
	tmp->rn=reg;
	tmp->imm=imm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_SHX_QueueOpRegImmReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	BGBCC_SHX_EmitQueueOp *tmp;
	tmp=BGBCC_SHX_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGIMMREG;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=imm;
	BGBCC_SHX_AddQueueOp(ctx, tmp);
	return(0);
}
