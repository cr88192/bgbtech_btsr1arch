int TKUCC_CgenEmitOp_AsmOp(TKUCC_MainContext *ctx,
	TKUCC_AsmOp *op)
{
	if(ctx->cblk_asm_first)
	{
		ctx->cblk_asm_last->next=op;
		ctx->cblk_asm_last=op;
	}else
	{
		ctx->cblk_asm_first=op;
		ctx->cblk_asm_last=op;
	}
}

int TKUCC_CgenEmitOp_RegRegReg(TKUCC_MainContext *ctx,
	int nmid, int rs, int rt, int rd)
{
	TKUCC_AsmOp *op;
	
	op=TKUCC_AsmAllocOp(ctx);
	op->nmid=nmid;
	op->arg[0]=op->t_arg+0;
	op->arg[1]=op->t_arg+1;
	op->arg[2]=op->t_arg+2;
	op->n_arg=3;
	
	op->arg[0]->aty=TKUCC_ARGTY_REG;
	op->arg[1]->aty=TKUCC_ARGTY_REG;
	op->arg[2]->aty=TKUCC_ARGTY_REG;

	op->arg[0]->rb=rs;
	op->arg[1]->rb=rt;
	op->arg[2]->rb=rd;
	
	return(TKUCC_CgenEmitOp_AsmOp(ctx, op));
}

int TKUCC_CgenEmitOp_RegImmReg(TKUCC_MainContext *ctx,
	int nmid, int rs, s64 imm, int rd)
{
	TKUCC_AsmOp *op;
	
	op=TKUCC_AsmAllocOp(ctx);
	op->nmid=nmid;
	op->arg[0]=op->t_arg+0;
	op->arg[1]=op->t_arg+1;
	op->arg[2]=op->t_arg+2;
	op->n_arg=3;
	
	op->arg[0]->aty=TKUCC_ARGTY_REG;
	op->arg[1]->aty=TKUCC_ARGTY_IMM;
	op->arg[2]->aty=TKUCC_ARGTY_REG;

	op->arg[0]->rb=rs;
	op->arg[1]->imm=imm;
	op->arg[2]->rb=rd;
	
	return(TKUCC_CgenEmitOp_AsmOp(ctx, op));
}

int TKUCC_CgenEmitOp_LblReg(TKUCC_MainContext *ctx,
	int nmid, int lbl, int rd)
{
	TKUCC_AsmOp *op;
	
	op=TKUCC_AsmAllocOp(ctx);
	op->nmid=nmid;
	op->arg[0]=op->t_arg+0;
	op->arg[1]=op->t_arg+1;
	op->n_arg=2;
	
	op->arg[0]->aty=TKUCC_ARGTY_LABEL;
	op->arg[1]->aty=TKUCC_ARGTY_REG;

	op->arg[0]->lbl=lbl;
	op->arg[1]->rb=rd;
	
	return(TKUCC_CgenEmitOp_AsmOp(ctx, op));
}

int TKUCC_CgenEmitOp_ImmReg(TKUCC_MainContext *ctx,
	int nmid, s64 imm, int rd)
{
	TKUCC_AsmOp *op;
	
	op=TKUCC_AsmAllocOp(ctx);
	op->nmid=nmid;
	op->arg[0]=op->t_arg+0;
	op->arg[1]=op->t_arg+1;
	op->n_arg=2;
	
	op->arg[0]->aty=TKUCC_ARGTY_IMM;
	op->arg[1]->aty=TKUCC_ARGTY_REG;

	op->arg[0]->imm=imm;
	op->arg[1]->rb=rd;
	
	return(TKUCC_CgenEmitOp_AsmOp(ctx, op));
}

int TKUCC_CgenEmitOp_LdDispReg(TKUCC_MainContext *ctx,
	int nmid, int rb, s64 disp, int rd)
{
	TKUCC_AsmOp *op;
	
	op=TKUCC_AsmAllocOp(ctx);
	op->nmid=nmid;
	op->arg[0]=op->t_arg+0;
	op->arg[1]=op->t_arg+1;
	op->n_arg=2;
	
	op->arg[0]->aty=TKUCC_ARGTY_MEM_RD;
	op->arg[1]->aty=TKUCC_ARGTY_REG;

	op->arg[0]->rb=rb;
	op->arg[0]->imm=disp;
	op->arg[1]->rb=rd;
	
	return(TKUCC_CgenEmitOp_AsmOp(ctx, op));
}

int TKUCC_CgenEmitOp_RegStDisp(TKUCC_MainContext *ctx,
	int nmid, int rd, int rb, s64 disp)
{
	TKUCC_AsmOp *op;
	
	op=TKUCC_AsmAllocOp(ctx);
	op->nmid=nmid;
	op->arg[0]=op->t_arg+0;
	op->arg[1]=op->t_arg+1;
	op->n_arg=2;
	
	op->arg[0]->aty=TKUCC_ARGTY_REG;
	op->arg[1]->aty=TKUCC_ARGTY_MEM_RD;

	op->arg[0]->rb=rd;
	op->arg[1]->rb=rb;
	op->arg[1]->imm=disp;
	
	return(TKUCC_CgenEmitOp_AsmOp(ctx, op));
}



int TKUCC_CgenEmitBinopInt(TKUCC_MainContext *ctx,
	TKUCC_IRDECL *op)
{
	int nmid, isuns;
	int rs, rt, rd;
	
	nmid=-1;
	isuns=TKUCC_TypeUnsignedP(ctx, op->dty);

	if(op->opr==TKUCC_BINOP_ADD)
		{ nmid=isuns?TKUCC_NMID_ADDUL:TKUCC_NMID_ADDSL; }
	if(op->opr==TKUCC_BINOP_SUB)
		{ nmid=isuns?TKUCC_NMID_SUBUL:TKUCC_NMID_SUBSL; }
	if(op->opr==TKUCC_BINOP_MUL)
		{ nmid=isuns?TKUCC_NMID_MULUL:TKUCC_NMID_MULSL; }

	if(op->opr==TKUCC_BINOP_AND)
		{ nmid=isuns?TKUCC_NMID_MULUL:TKUCC_NMID_AND; }
	if(op->opr==TKUCC_BINOP_OR)
		{ nmid=isuns?TKUCC_NMID_MULUL:TKUCC_NMID_OR; }
	if(op->opr==TKUCC_BINOP_XOR)
		{ nmid=isuns?TKUCC_NMID_MULUL:TKUCC_NMID_XOR; }
		
	if(nmid>=0)
	{
		rd=TKUCC_CgenGetValueAsRegisterDst(ctx, op->vd);
		rs=TKUCC_CgenGetValueAsRegisterSrc(ctx, op->va);
		rt=TKUCC_CgenGetValueAsRegisterSrc(ctx, op->vb);
		
		TKUCC_CgenEmitOp_RegRegReg(ctx, nmid, rs, rt, rd);
		return(1);
	}
}
