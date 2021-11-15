BGBCC_CCXL_BackendFuncs_vt *bgbcc_ccxl_backends=NULL;

char *bgbcc_ccxl_basepath;

void BGBCC_CCXL_SetBasePath(char *path)
{
	bgbcc_ccxl_basepath=bgbcc_strdup(path);
}

char *BGBCC_CCXL_GetBasePath(void)
{
	return(bgbcc_ccxl_basepath);
}

ccxl_status BGBCC_CCXL_RegisterBackend(BGBCC_CCXL_BackendFuncs_vt *ivt)
{
	ivt->next=bgbcc_ccxl_backends;
	bgbcc_ccxl_backends=ivt;
	return(0);
}

#if 1
ccxl_status BGBCC_CCXL_StackFn(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	char *stn;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STKFN);
	BGBCC_CCXLR3_EmitArgString(ctx, name);

	if(ctx->cgif_no3ac)
		return(0);

	stn=bgbcc_strdup(name);
	ctx->lfn=stn;

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DBGFN;
	op->prd=ctx->curprd;
//	op->imm.str=bgbcc_strdup(name);
	op->imm.str=stn;
	op->immty=CCXL_VOPITY_STR;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_StackLn(BGBCC_TransState *ctx, int line)
{
	BGBCC_CCXL_VirtOp *op;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STKLN);
	BGBCC_CCXLR3_EmitArgInt(ctx, line);

	if(ctx->cgif_no3ac)
		return(0);

	ctx->lln=line;

#if 1
	if(ctx->n_vop>ctx->s_vop)
	{
		op=ctx->vop[ctx->n_vop-1];
		if(op->opn==CCXL_VOP_DBGLN)
		{
			op->imm.si=line;
			return(0);
		}
	}
#endif

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DBGLN;
	op->prd=ctx->curprd;
	op->imm.si=line;
	op->immty=CCXL_VOPITY_SI;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_StackDisable3AC(BGBCC_TransState *ctx)
{
	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_DI_3AC);
	ctx->cgif_no3ac++;
	return(0);
}

ccxl_status BGBCC_CCXL_StackEnable3AC(BGBCC_TransState *ctx)
{
	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_EN_3AC);
	if(ctx->cgif_no3ac>0)
		ctx->cgif_no3ac--;
	return(0);
}

ccxl_status BGBCC_CCXL_StackSetPred(BGBCC_TransState *ctx, int prd)
{
	BGBCC_CCXL_VirtOp *op;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SETPRED);
	BGBCC_CCXLR3_EmitArgInt(ctx, prd);

	if(ctx->curprd!=prd)
	{
		op=BGBCC_CCXL_AllocVirtOp(ctx);
		op->opn=CCXL_VOP_PREDSYNC;
		op->prd=ctx->curprd;
		BGBCC_CCXL_AddVirtOp(ctx, op);
	}

	ctx->curprd=prd;
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLabel(BGBCC_TransState *ctx, ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LABEL);
	BGBCC_CCXLR3_EmitArgLabel(ctx, lbl);

	if(ctx->cgif_no3ac)
		return(0);

	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LABEL;
	op->prd=ctx->curprd;
	op->imm.ui=lbl.id;
	op->immty=CCXL_VOPITY_LBL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJump(BGBCC_TransState *ctx, ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JMP;
	op->prd=ctx->curprd;
	op->imm.ui=lbl.id;
	op->immty=CCXL_VOPITY_LBL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegZero(BGBCC_TransState *ctx,
	ccxl_type type, int cmpop, ccxl_register reg, ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);
		
	if(cmpop<0)
		{ BGBCC_DBGBREAK }

	if(BGBCC_CCXL_IsRegZzP(ctx, reg))
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, reg);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JCMP_ZERO;
	op->prd=ctx->curprd;
	op->opr=cmpop;
	op->type=type;
	op->srca=reg;
	op->imm.ui=lbl.id;
	op->immty=CCXL_VOPITY_LBL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegCmpI(BGBCC_TransState *ctx,
	ccxl_type type, int cmpop, ccxl_register sreg, ccxl_register treg,
	ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;

	if(cmpop<0)
		{ BGBCC_DBGBREAK }

	if(BGBCC_CCXL_IsRegZzP(ctx, sreg))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, treg))
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, sreg);
	BGBCC_CCXL_EmitTempPhi(ctx, treg);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JCMP;
	op->prd=ctx->curprd;
	op->opr=cmpop;
	op->type=type;
	op->srca=sreg;
	op->srcb=treg;
	op->imm.ui=lbl.id;
	op->immty=CCXL_VOPITY_LBL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegCmp(BGBCC_TransState *ctx,
	ccxl_type type, int cmpop, ccxl_register sreg, ccxl_register treg,
	ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;
	int cmp1;
	s32 imm;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, sreg))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, treg))
		{ BGBCC_DBGBREAK }

#if 1
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		if(imm==0)
		{
			return(BGBCC_CCXL_EmitJumpRegZero(ctx,
				type, cmpop, sreg, lbl));
		}

		return(BGBCC_CCXL_EmitJumpRegCmpI(ctx,
			type, cmpop, sreg, treg, lbl));
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		switch(cmpop)
		{
		case CCXL_CMP_EQ:	cmp1=CCXL_CMP_EQ;	break;
		case CCXL_CMP_NE:	cmp1=CCXL_CMP_NE;	break;
		case CCXL_CMP_LT:	cmp1=CCXL_CMP_GT;	break;
		case CCXL_CMP_GT:	cmp1=CCXL_CMP_LT;	break;
		case CCXL_CMP_LE:	cmp1=CCXL_CMP_GE;	break;
		case CCXL_CMP_GE:	cmp1=CCXL_CMP_LE;	break;
		default:			cmp1=-1;			break;
		}

		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		if(imm==0)
		{
			return(BGBCC_CCXL_EmitJumpRegZero(ctx,
				type, cmp1, treg, lbl));
		}

		return(BGBCC_CCXL_EmitJumpRegCmpI(ctx,
			type, cmp1, treg, sreg, lbl));
	}
#endif

	return(BGBCC_CCXL_EmitJumpRegCmpI(ctx,
		type, cmpop, sreg, treg, lbl));
}

ccxl_status BGBCC_CCXL_EmitMov(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty, dty;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, sreg))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dreg))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

	BGBCC_CCXL_TypeArrayP(ctx, dty);
	BGBCC_CCXL_TypeArrayP(ctx, sty);
	BGBCC_CCXL_TypeArrayP(ctx, type);
	
	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) &&
		!(BGBCC_CCXL_TypePointerP(ctx, dty) &&
			BGBCC_CCXL_TypeValueObjectP(ctx, sty)))
		__debugbreak();

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_MOV;
	op->prd=ctx->curprd;
	op->type=type;
	op->srca=sreg;
	op->dst=dreg;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}


ccxl_status BGBCC_CCXL_EmitCallOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int na)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }

//	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_StackPhiTemporariesCall(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, src);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CALL;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->srcb.val=CCXL_REGID_REG_Z;
	op->imm.call.na=na;
	op->imm.call.ca=0;
	op->imm.call.args=bgbcc_malloc(na*sizeof(ccxl_register));
	op->immty=CCXL_VOPITY_CALL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallIntrinOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int na)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_EmitTempPhi(ctx, src);
	BGBCC_CCXL_EmitTempPhi(ctx, dst);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CALL_INTRIN;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->srcb.val=CCXL_REGID_REG_Z;
	op->imm.call.na=na;
	op->imm.call.ca=0;
	op->imm.call.args=bgbcc_malloc(na*sizeof(ccxl_register));
	op->immty=CCXL_VOPITY_CALL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitObjCallOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register src, ccxl_register obj, int na)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }

//	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_StackPhiTemporariesCall(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, src);
	BGBCC_CCXL_EmitTempPhi(ctx, obj);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_OBJCALL;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->srcb=obj;
	op->imm.call.na=na;
	op->imm.call.ca=0;
	op->imm.call.args=bgbcc_malloc(na*sizeof(ccxl_register));
	op->immty=CCXL_VOPITY_CALL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallCsrvOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CSRV;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallArg(BGBCC_TransState *ctx,
	ccxl_register reg)
{
	BGBCC_CCXL_VirtOp *op;
	int i;

	if(BGBCC_CCXL_IsRegZzP(ctx, reg))
		{ BGBCC_DBGBREAK }

	op=ctx->vop[ctx->n_vop-1];

//	if(op->opn!=CCXL_VOP_CALL)
	if((op->opn!=CCXL_VOP_CALL) &&
		(op->opn!=CCXL_VOP_OBJCALL) &&
		(op->opn!=CCXL_VOP_CALL_INTRIN))
			{ BGBCC_DBGBREAK }

	i=op->imm.call.ca++;
	op->imm.call.args[i]=reg;
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJmpTab(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register src,
	int clm, int cln, ccxl_label *clbl, s64 *clv,
	ccxl_label dfl, ccxl_label dfl2)
{
	BGBCC_CCXL_VirtOp *op;
	s64 vmin, vmax;
	int ncl, ncv;
	int i, j, k;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }

	ncl=cln-clm;

//	if((ncl<1) || (ncl>256))
	if((ncl<1) || (ncl>1024))
		{ BGBCC_DBGBREAK }

	vmin=clv[clm];
	vmax=clv[cln-1];
	ncv=(vmax-vmin)+1;

//	if((ncv<1) || (ncv>256))
	if((ncv<1) || (ncv>1024))
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, src);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JMPTAB;
	op->type=type;
//	op->dst=dst;
	op->srca=src;

//	op->srcc.val=CCXL_REGTY2_IMM_LONG|(vmin&CCXL_REGLONG2_MASK);
//	op->srcd.val=CCXL_REGTY2_IMM_LONG|(vmax&CCXL_REGLONG2_MASK);

	op->imm.jmptab.vmin=vmin;
	op->imm.jmptab.nlbl=ncv;
//	op->imm.jmptab.ca=0;
	op->imm.jmptab.lbls=bgbcc_malloc((ncv+2)*sizeof(ccxl_label));
	op->immty=CCXL_VOPITY_JMPTAB;

//	for(i=0; i<ncv; i++)
	for(i=0; i<(ncv+2); i++)
	{
		op->imm.jmptab.lbls[i]=dfl2;
	}
//	op->imm.jmptab.lbls[ncv]=dfl;
	op->imm.jmptab.lbls[ncv+1]=dfl;

	for(i=0; i<ncl; i++)
	{
		j=clv[clm+i]-vmin;
		
		if((j<0) || (j>=ncv))
			{ BGBCC_DBGBREAK }
		
		op->imm.jmptab.lbls[j]=clbl[clm+i];
	}

	BGBCC_CCXL_AddVirtOp(ctx, op);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetDefault(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

//	BGBCC_CCXL_StackPhiTemporaries(ctx);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_RETDFL;
	op->prd=ctx->curprd;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetV(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

//	BGBCC_CCXL_StackPhiTemporaries(ctx);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_RETV;
	op->prd=ctx->curprd;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }

//	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, src);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_RET;
	op->prd=ctx->curprd;
	op->type=type;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitTempPhi(BGBCC_TransState *ctx,
	ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;

	return(0);

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
	{
//		BGBCC_DBGBREAK
		return(0);
	}

	if(!BGBCC_CCXL_IsRegTempP(ctx, src))
		return(0);

	if(ctx->s_vop==ctx->n_vop)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_TEMP_PHI;
//	op->prd=ctx->curprd;
//	op->type=type;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitConv(BGBCC_TransState *ctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;
	
	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	if(dtype.val==CCXL_TY_V)
		{ BGBCC_DBGBREAK }

	if(dtype.val==stype.val)
	{
		op=NULL;
//		BGBCC_CCXL_EmitMov(ctx, dtype, dst, src);
//		return(0);
	}

	BGBCC_CCXL_TypeArrayP(ctx, dtype);
	BGBCC_CCXL_TypeArrayP(ctx, stype);

	if(BGBCC_CCXL_TypeSmallIntP(ctx, stype) &&
		BGBCC_CCXL_IsRegDoubleP(ctx, src))
			{ BGBCC_DBGBREAK }

	if(BGBCC_CCXL_TypeFloatP(ctx, stype) ||
		BGBCC_CCXL_TypeFloatP(ctx, dtype))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
	}

	if(BGBCC_CCXL_TypeVarObjP(ctx, dtype))
	{
		BGBCC_CCXL_MarkTypeVarConv(ctx, stype);
	}

	if(BGBCC_CCXL_TypeVarObjP(ctx, stype))
	{
		BGBCC_CCXL_MarkTypeVarConv(ctx, dtype);
	}

//	if((dtype.val==408) && (stype.val==0))
//		{ BGBCC_DBGBREAK }
	
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CONV;
	op->prd=ctx->curprd;
	op->type=dtype;
	op->stype=stype;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitUnaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr,
	ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_UNARY;
	op->prd=ctx->curprd;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitBinaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	if(BGBCC_CCXL_TypeSmallLongP(ctx, type) &&
		!BGBCC_CCXL_IsRegImmIntP(ctx, srcb))
	{
		if(opr==CCXL_BINOP_DIV)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_IDIV_VAR;
		if(opr==CCXL_BINOP_MOD)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_IMOD_VAR;
		if(opr==CCXL_BINOP_SHL)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISHL_VAR;
		if(opr==CCXL_BINOP_SHR)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISHR_VAR;
			else
				ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISAR_VAR;
		}
		if(opr==CCXL_BINOP_SHRR)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISHR_VAR;
	}
	
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
	}

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_BINARY;
	op->prd=ctx->curprd;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitTrinaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb, ccxl_register srcc)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcc))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

#if 0
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type) &&
		!BGBCC_CCXL_IsRegImmIntP(ctx, srcb))
	{
		if(opr==CCXL_BINOP_DIV)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_IDIV_VAR;
		if(opr==CCXL_BINOP_MOD)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_IMOD_VAR;
		if(opr==CCXL_BINOP_SHL)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISHL_VAR;
		if(opr==CCXL_BINOP_SHR)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISHR_VAR;
			else
				ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISAR_VAR;
		}
		if(opr==CCXL_BINOP_SHRR)
			ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_ISHR_VAR;
	}
	
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
	}
#endif

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_TRINARY;
	op->prd=ctx->curprd;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	op->srcc=srcc;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCompareOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_COMPARE;
	op->prd=ctx->curprd;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitPredCmpOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
//	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
//		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_StackPhiTemporaries(ctx);
	BGBCC_CCXL_EmitTempPhi(ctx, srca);
	BGBCC_CCXL_EmitTempPhi(ctx, srcb);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_PREDCMP;
	op->prd=ctx->curprd;
	op->opr=opr;
	op->type=type;
//	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, src);

	if(BGBCC_CCXL_TypeSquareArrayP(ctx, sty))
		__debugbreak();

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LDIXIMM;
	op->prd=ctx->curprd;
	op->type=type;
	op->stype=sty;
	op->dst=dst;
	op->srca=src;
	op->imm.si=idx;
	op->immty=CCXL_VOPITY_SI;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, dst);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_STIXIMM;
	op->prd=ctx->curprd;
	op->type=type;
	op->stype=sty;
	op->dst=dst;
	op->srca=src;
	op->imm.si=idx;
	op->immty=CCXL_VOPITY_SI;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, srca);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LDIX;
	op->prd=ctx->curprd;
	op->type=type;
	op->stype=sty;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, dst);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_STIX;
	op->prd=ctx->curprd;
	op->type=type;
	op->stype=sty;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLeaImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty;

//	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dst, src))
//		__debugbreak();

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, src);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LEAIMM;
	op->prd=ctx->curprd;
	op->type=type;
	op->stype=sty;
	op->dst=dst;
	op->srca=src;
	op->imm.si=idx;
	op->immty=CCXL_VOPITY_SI;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLea(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	ccxl_type sty;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	sty=BGBCC_CCXL_GetRegType(ctx, srca);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LEA;
	op->prd=ctx->curprd;
	op->type=type;
	op->stype=sty;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLdaVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LDAVAR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitSizeofVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_SIZEOFVAR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitDiffPtr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, srca))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, srcb))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DIFFPTR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}


ccxl_status BGBCC_CCXL_EmitOffsetOf(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	int fn;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_OFFSETOF;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
//	op->srca=srca;
//	op->srcb=srcb;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name, char *sig)
{
	BGBCC_CCXL_VirtOp *op;
	BGBCC_CCXL_LiteralInfo *st1;
	int fn;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOT;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
//	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	fn=BGBCC_CCXL_LookupStructFieldIDSig(ctx, st, name, sig);
	if(fn<0)
	{
		st1=BGBCC_CCXL_GetStructSuperclass(ctx, st);
		while(st1)
		{
			fn=BGBCC_CCXL_LookupStructFieldIDSig(ctx, st1, name, sig);
			if(fn>=0)
				{ st=st1; break; }
			st1=BGBCC_CCXL_GetStructSuperclass(ctx, st1);
		}
		
		if(fn<0)
			{ BGBCC_DBGBREAK }
	}
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	BGBCC_CCXL_LiteralInfo *st1;
	int fn;

	if(ctx->cgif_no3ac)
		return(0);

	if(BGBCC_CCXL_IsRegZzP(ctx, src))
		{ BGBCC_DBGBREAK }
	if(BGBCC_CCXL_IsRegZzP(ctx, dst))
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_STORESLOT;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	if(fn<0)
	{
		st1=BGBCC_CCXL_GetStructSuperclass(ctx, st);
		while(st1)
		{
			fn=BGBCC_CCXL_LookupStructFieldID(ctx, st1, name);
			if(fn>=0)
				{ st=st1; break; }
			st1=BGBCC_CCXL_GetStructSuperclass(ctx, st1);
		}
		
		if(fn<0)
			{ BGBCC_DBGBREAK }
	}
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlotAddr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	BGBCC_CCXL_LiteralInfo *st1;
	int fn;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOTADDR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	if(fn<0)
	{
		st1=BGBCC_CCXL_GetStructSuperclass(ctx, st);
		while(st1)
		{
			fn=BGBCC_CCXL_LookupStructFieldID(ctx, st1, name);
			if(fn>=0)
				{ st=st1; break; }
			st1=BGBCC_CCXL_GetStructSuperclass(ctx, st1);
		}
		
		if(fn<0)
			{ BGBCC_DBGBREAK }
	}
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlotID(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, int fn)
{
	BGBCC_CCXL_VirtOp *op;
	BGBCC_CCXL_LiteralInfo *st1;

	if(ctx->cgif_no3ac)
		return(0);

	if(fn<0)
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOT;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlotAddrID(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, int fn)
{
	BGBCC_CCXL_VirtOp *op;
	BGBCC_CCXL_LiteralInfo *st1;

	if(ctx->cgif_no3ac)
		return(0);

	if(fn<0)
		{ BGBCC_DBGBREAK }

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOTADDR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_INITOBJ;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->imm.obj.gid=st->litid;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitDropObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DROPOBJ;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->imm.si=st->litid;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, int sz)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_INITARR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->imm.si=sz;
	op->immty=CCXL_VOPITY_SI;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, int sz)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_INITOBJARR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=sz;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register val, int sz)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADINITARR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=val;
	op->imm.si=sz;
	op->immty=CCXL_VOPITY_SI;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, ccxl_register val, int sz)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADINITOBJARR;
	op->prd=ctx->curprd;
	op->type=type;
	op->dst=dst;
	op->srca=val;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=sz;
	op->immty=CCXL_VOPITY_GFID;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitVaStart(BGBCC_TransState *ctx,
	ccxl_register dreg, ccxl_register sreg)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_VA_START;
	op->prd=ctx->curprd;
//	op->type=type;
	op->srca=sreg;
	op->dst=dreg;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitVaEnd(BGBCC_TransState *ctx,
	ccxl_register sreg)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_VA_END;
	op->prd=ctx->curprd;
//	op->type=type;
	op->srca=sreg;
//	op->dst=dreg;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitVaArg(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_VA_ARG;
	op->prd=ctx->curprd;
	op->type=type;
	op->srca=sreg;
	op->dst=dreg;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}


ccxl_status BGBCC_CCXL_EmitCSelCmp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb,
	ccxl_type ptype, int cmpop, ccxl_register srcc, ccxl_register srcd)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CSELCMP;
	op->prd=ctx->curprd;
	
	//select op
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;

	//compare op
	op->stype=ptype;
	op->opr=cmpop;
	op->srcc=srcc;
	op->srcd=srcd;

	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}
ccxl_status BGBCC_CCXL_EmitCSelCmpZero(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb,
	ccxl_type ptype, int cmpop, ccxl_register srcc)
{
	BGBCC_CCXL_VirtOp *op;

	if(ctx->cgif_no3ac)
		return(0);

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CSELCMP_Z;
	op->prd=ctx->curprd;

	//select op
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;

	//compare op
	op->stype=ptype;
	op->opr=cmpop;
	op->srcc=srcc;

	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_InlineAsmBlob(BGBCC_TransState *ctx, char *text)
{
	BGBCC_CCXL_VirtOp *op;
	char *stn;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_INLINEASM);
	BGBCC_CCXLR3_EmitArgString(ctx, text);

	if(ctx->cgif_no3ac)
		return(0);

	stn=bgbcc_strdup(text);
	ctx->lfn=stn;

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_ASMINLINE;
	op->prd=ctx->curprd;
	op->imm.str=stn;
	op->immty=CCXL_VOPITY_STR;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

#endif


ccxl_status BGBCC_CCXL_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	if(ctx->back_vt && ctx->back_vt->FlattenImage)
		return(ctx->back_vt->FlattenImage(ctx, obuf, rosz, imgfmt));
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}

ccxl_status BGBCC_CCXL_AddResourceData(BGBCC_TransState *ctx,
	char *name, byte *buf, int sz, fourcc imgfmt)
{
	if(ctx->back_vt && ctx->back_vt->AddResourceData)
		return(ctx->back_vt->AddResourceData(ctx, name, buf, sz, imgfmt));
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}


/** Setup context for the current target architecture.
  * Returns UNSUPPORTED error status if unsupported.
  */
ccxl_status BGBCC_CCXL_SetupContextForArch(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_BackendFuncs_vt *cur;
	int i;

	if(ctx->back_vt && ctx->back_vt->SetupContextForArch)
		return(ctx->back_vt->SetupContextForArch(ctx));
		
	cur=bgbcc_ccxl_backends;
	while(cur)
	{
		if(cur->SetupContextForArch)
		{
			i=cur->SetupContextForArch(ctx);
			if(i>=0)
			{
				ctx->back_vt=cur;
				return(i);
			}
		}
		cur=cur->next;
	}
	
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}


/** Setup parser context for the current target architecture.
  * Returns UNSUPPORTED error status if unsupported.
  */
ccxl_status BGBCC_CCXL_SetupParserForArch(BGBCP_ParseState *ctx)
{
	BGBCC_CCXL_BackendFuncs_vt *cur;
	int i;

	if(ctx->back_vt && ctx->back_vt->SetupParserForArch)
		return(ctx->back_vt->SetupParserForArch(ctx));
		
	cur=bgbcc_ccxl_backends;
	while(cur)
	{
		if(cur->SetupParserForArch)
		{
			i=cur->SetupParserForArch(ctx);
			if(i>=0)
			{
				ctx->back_vt=cur;
				return(i);
			}
		}
		cur=cur->next;
	}
	
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}

ccxl_status BGBCC_CCXL_LoadBufferDLL(BGBCC_TransState *ctx,
	byte *buf, int sz)
{
	if(ctx->back_vt && ctx->back_vt->LoadBufferDLL)
		return(ctx->back_vt->LoadBufferDLL(ctx, buf, sz));
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}

ccxl_status BGBCC_CCXL_InitTargets(void)
{
	static int init=0;
	
	if(init)
		return(0);
	init=1;

	return(1);
}
