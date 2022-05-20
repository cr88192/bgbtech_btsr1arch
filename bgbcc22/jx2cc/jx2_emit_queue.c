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

BGBCC_JX2_EmitQueueOp *BGBCC_JX2_AllocQueueOp(
	BGBCC_JX2_Context *ctx)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	int i;
	
	tmp=ctx->eqfree;
	if(tmp)
	{
		ctx->eqfree=tmp->next;
		memset(tmp, 0, sizeof(BGBCC_JX2_EmitQueueOp));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(64*sizeof(BGBCC_JX2_EmitQueueOp));
	for(i=0; i<63; i++)
	{
		tmp->next=ctx->eqfree;
		ctx->eqfree=tmp;
		tmp++;
	}
	return(tmp);
}

int BGBCC_JX2_FreeQueueOp(
	BGBCC_JX2_Context *ctx, BGBCC_JX2_EmitQueueOp *tmp)
{
	tmp->next=ctx->eqfree;
	ctx->eqfree=tmp;
	return(0);
}

int BGBCC_JX2_AddQueueOp(
	BGBCC_JX2_Context *ctx, BGBCC_JX2_EmitQueueOp *tmp)
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

int BGBCC_JX2_EmitQueueOpBasic(
	BGBCC_JX2_Context *ctx,
	BGBCC_JX2_EmitQueueOp *op)
{
	switch(op->fmid)
	{
	case BGBCC_SH_FMID_NONE:
		BGBCC_JX2_EmitOpNone(ctx, op->nmid);
		break;

	case BGBCC_SH_FMID_REGRM:
		BGBCC_JX2_EmitOpReg(ctx,
			op->nmid, op->rm);
		break;
	case BGBCC_SH_FMID_REGRN:
		BGBCC_JX2_EmitOpReg(ctx,
			op->nmid, op->rn);
		break;

	case BGBCC_SH_FMID_REGREG:
		BGBCC_JX2_EmitOpRegReg(ctx,
			op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGIMM:
		BGBCC_JX2_EmitOpImmReg(ctx,
			op->nmid, op->imm, op->rn);
		break;

	case BGBCC_SH_FMID_REGST:
		BGBCC_JX2_EmitOpRegStReg(ctx, op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGLD:
		BGBCC_JX2_EmitOpLdRegReg(ctx, op->nmid, op->rm, op->rn);
		break;

	case BGBCC_SH_FMID_REGDECST:
		BGBCC_JX2_EmitOpRegStDecReg(ctx, op->nmid, op->rm, op->rn);
		break;
	case BGBCC_SH_FMID_REGINCLD:
		BGBCC_JX2_EmitOpLdIncRegReg(ctx, op->nmid, op->rm, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTR0N:
		BGBCC_JX2_EmitOpRegStReg2(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;
	case BGBCC_SH_FMID_REGLDR0M:
		BGBCC_JX2_EmitOpLdReg2Reg(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTDISP:
		BGBCC_JX2_EmitOpRegStRegDisp(ctx, op->nmid, op->rm, op->rn, op->imm);
		break;
	case BGBCC_SH_FMID_REGLDDISP:
		BGBCC_JX2_EmitOpLdRegDispReg(ctx, op->nmid, op->rm, op->imm, op->rn);
		break;

	case BGBCC_SH_FMID_REGSTRODISP:
		BGBCC_JX2_EmitOpRegStReg2Disp(ctx, op->nmid,
			op->rm, op->rn, op->ro, op->imm);
		break;
	case BGBCC_SH_FMID_REGLDRODISP:
		BGBCC_JX2_EmitOpLdReg2DispReg(ctx, op->nmid,
			op->rm, op->ro, op->imm, op->rn);
		break;


	case BGBCC_SH_FMID_IMM:
		BGBCC_JX2_EmitOpImm(ctx, op->nmid, op->imm);
		break;
	
	case BGBCC_SH_FMID_REGREGREG:
		BGBCC_JX2_EmitOpRegRegReg(ctx, op->nmid, op->rm, op->ro, op->rn);
		break;
	case BGBCC_SH_FMID_REGIMMREG:
		BGBCC_JX2_EmitOpRegImmReg(ctx, op->nmid, op->rm, op->imm, op->rn);
		break;
	}
	return(0);
}

int BGBCC_JX2_EmitQueueOps(BGBCC_JX2_Context *ctx)
{
	BGBCC_JX2_EmitQueueOp *cur, *nxt;
	
	cur=ctx->eqstrt;
	ctx->eqstrt=NULL;
	ctx->eqend=NULL;
	while(cur)
	{
		nxt=cur->next;
		BGBCC_JX2_EmitQueueOpBasic(ctx, cur);
		BGBCC_JX2_FreeQueueOp(ctx, cur);
		cur=nxt;
	}
	return(0);
}

int BGBCC_JX2_QueueOpNone(BGBCC_JX2_Context *ctx, int nmid)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_NONE;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGRN;
	tmp->rn=reg;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpImm(BGBCC_JX2_Context *ctx, int nmid, int imm)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_IMM;
	tmp->imm=imm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpMReg(BGBCC_JX2_Context *ctx, int nmid, int reg)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_STREG;
	tmp->rn=reg;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegReg(BGBCC_JX2_Context *ctx, int nmid, int rm, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGREG;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGST;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLD;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGDECST;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGINCLD;
	tmp->rn=rn;
	tmp->rm=rm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTDISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=disp;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDDISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=disp;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTR0N;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDR0M;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGSTRODISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	tmp->imm=disp;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpLdReg2DispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGLDRODISP;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->ro=ro;
	tmp->imm=disp;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGREGREG;
	tmp->rn=rn;
	tmp->rm=rs;
	tmp->ro=rt;
//	tmp->imm=disp;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int imm, int reg)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGIMM;
	tmp->rn=reg;
	tmp->imm=imm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}

int BGBCC_JX2_QueueOpRegImmReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	BGBCC_JX2_EmitQueueOp *tmp;
	tmp=BGBCC_JX2_AllocQueueOp(ctx);
	tmp->nmid=nmid;
	tmp->fmid=BGBCC_SH_FMID_REGIMMREG;
	tmp->rn=rn;
	tmp->rm=rm;
	tmp->imm=imm;
	BGBCC_JX2_AddQueueOp(ctx, tmp);
	return(0);
}
