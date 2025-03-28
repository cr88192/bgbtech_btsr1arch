#include <bgbccc.h>

#if 0
ccxl_label BGBCC_CCXL_LabelFromName(BGBCC_TransState *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->n_goto; i++)
	{
		if(!strcmp(ctx->goto_name[i], name))
			return(ctx->goto_lbl[i]);
	}
	
	if(!ctx->goto_name)
	{
		i=256;
		ctx->goto_name=bgbcc_malloc(i*sizeof(char *));
		ctx->goto_lbl=bgbcc_malloc(i*sizeof(ccxl_label));
		ctx->m_goto=i;
	}
	
	if((ctx->n_goto+1)>=ctx->m_goto)
	{
		i=ctx->m_goto+(ctx->m_goto>>1);
		ctx->goto_name=bgbcc_realloc(ctx->goto_name, i*sizeof(char *));
		ctx->goto_lbl=bgbcc_realloc(ctx->goto_lbl, i*sizeof(ccxl_label));
		ctx->m_goto=i;
	}
	
	i=ctx->n_goto++;
	ctx->goto_name[i]=bgbcc_strdup(name);
	ctx->goto_lbl[i]=BGBCC_CCXL_GenSym(ctx);
	return(ctx->goto_lbl[i]);
}
#endif

#if 1
ccxl_label BGBCC_CCXL_LabelFromName(BGBCC_TransState *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->cur_func->n_goto; i++)
	{
		if(!strcmp(ctx->cur_func->goto_name[i], name))
			return(ctx->cur_func->goto_lbl[i]);
	}
	
	if(!ctx->cur_func->goto_name)
	{
//		i=256;
		i=16;
		ctx->cur_func->goto_name=bgbcc_malloc(i*sizeof(char *));
		ctx->cur_func->goto_lbl=bgbcc_malloc(i*sizeof(ccxl_label));
		ctx->cur_func->n_goto=0;
		ctx->cur_func->m_goto=i;
	}
	
	if((ctx->cur_func->n_goto+1)>=ctx->cur_func->m_goto)
	{
		i=ctx->cur_func->m_goto+(ctx->cur_func->m_goto>>1);
		ctx->cur_func->goto_name=bgbcc_realloc(
			ctx->cur_func->goto_name, i*sizeof(char *));
		ctx->cur_func->goto_lbl=bgbcc_realloc(
			ctx->cur_func->goto_lbl, i*sizeof(ccxl_label));
		ctx->cur_func->m_goto=i;
	}
	
	i=ctx->cur_func->n_goto++;
	ctx->cur_func->goto_name[i]=bgbcc_strdup(name);
	ctx->cur_func->goto_lbl[i]=BGBCC_CCXL_GenSym(ctx);
	return(ctx->cur_func->goto_lbl[i]);
}
#endif

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetLiteralRawSig(
	BGBCC_TransState *ctx, char *sig)
{
	char tb0[256], tb1[256];
	BGBCC_CCXL_LiteralInfo *obj;
	BGBCC_CCXL_LiteralInfo *obj1;
	ccxl_type bty;
	char *s0, *s1;
	int i, j, k, h;
	
	h=BGBCC_CCXL_HashName(sig)&255;

	i=ctx->hash_literals_sig[h];
	while(i>0)
	{
		obj=ctx->literals[i];
		i=obj->hnext_sig;
		if(obj->littype!=CCXL_LITID_RAWSIG)
			continue;
		if(!strcmp(obj->sig, sig))
			return(obj);
	}

#if 0
	for(i=1; i<ctx->n_literals; i++)
	{
		obj=ctx->literals[i];
		if(obj->littype!=CCXL_LITID_RAWSIG)
			continue;
		if(!strcmp(obj->sig, sig))
			return(obj);
	}
#endif

#if 0
	if(*sig=='(')
	{
		obj=BGBCC_CCXL_AllocLiteral(ctx);
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_FUNCTION;
		obj->littype=CCXL_LITID_FUNCTION;
//		obj->littype=CCXL_LITID_RAWSIG;
		obj->sig=bgbcc_strdup(sig);

		s0=sig+1; i=1;
		while(*s0 && (*s0!=')'))
		{
			sprintf(tb1, "_a%d", i++);

			s1=BGBCC_CCXL_SigNext(s0);
			j=s1-s0;
			memset(tb0, 0, 256);
			memcpy(tb0, s0, j);

			obj1=BGBCC_CCXL_AllocLiteral(ctx);
			obj1->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
			obj1->decl->regtype=CCXL_LITID_VAR;
			obj1->littype=CCXL_LITID_VAR;

			obj1->decl->name=bgbcc_strdup(tb1);

			obj1->decl->sig=bgbcc_strdup(tb0);
			BGBCC_CCXL_TypeFromSig(ctx, &bty, tb0);
			obj1->decl->type=bty;

			BGBCC_CCXL_AddFrameArg(ctx,
				obj->decl,
				obj1->decl);

			BGBCC_CCXL_CheckFreeLiteral(ctx, obj1);

			s0=s1;
		}
		
		if(*s0==')')
		{
			s0++;
//			BGBCC_CCXL_TypeFromSig(ctx, &bty, s0);
//			obj1->decl->type=bty;
		}

		BGBCC_CCXL_AddLiteral(ctx, obj);
		return(obj);

//		BGBCC_CCXL_Begin(ctx, CCXL_CMD_S_PROTOTYPE);
//		BGBCC_CCXL_End(ctx);
	}
#endif
	
	obj=BGBCC_CCXL_AllocLiteral(ctx);
	obj->littype=CCXL_LITID_RAWSIG;
	obj->sig=bgbcc_strdup(sig);
	i=BGBCC_CCXL_AddLiteral(ctx, obj);
	
	obj->hnext_sig=ctx->hash_literals_sig[h];
	ctx->hash_literals_sig[h]=i;

	return(obj);
}

void BGBCC_CCXL_CompileBreak(BGBCC_TransState *ctx)
{
	ccxl_label l;

	ctx->loop_localstate|=BGBCC_LOOPFL_BREAK;
	l=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMP);
	BGBCC_CCXLR3_EmitArgLabel(ctx, l);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	BGBCC_CCXL_EmitJump(ctx, l);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinue(BGBCC_TransState *ctx)
{
	ccxl_label l;

	ctx->loop_localstate|=BGBCC_LOOPFL_CONTINUE;
	l=ctx->contstack[ctx->contstackpos-1];

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMP);
	BGBCC_CCXLR3_EmitArgLabel(ctx, l);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	BGBCC_CCXL_EmitJump(ctx, l);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileBreakFalse(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label l;

	l=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPF);
	BGBCC_CCXLR3_EmitArgLabel(ctx, l);


	BGBCC_CCXL_PopRegister(ctx, &reg);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_EQ, reg, l);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileBreakTrue(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label l;

	l=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPT);
	BGBCC_CCXLR3_EmitArgLabel(ctx, l);

	BGBCC_CCXL_PopRegister(ctx, &reg);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_NE, reg, l);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinueFalse(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label l;

	l=ctx->contstack[ctx->contstackpos-1];

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPF);
	BGBCC_CCXLR3_EmitArgLabel(ctx, l);

	BGBCC_CCXL_PopRegister(ctx, &reg);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_EQ, reg, l);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinueTrue(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label lbl;

	lbl=ctx->contstack[ctx->contstackpos-1];

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPT);
	BGBCC_CCXLR3_EmitArgLabel(ctx, lbl);

	BGBCC_CCXL_PopRegister(ctx, &reg);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_NE, reg, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}


void BGBCC_CCXL_CompileJmp(BGBCC_TransState *ctx, ccxl_label lbl)
{
	ccxl_register reg;
	ccxl_type ty;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMP);
	BGBCC_CCXLR3_EmitArgLabel(ctx, lbl);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	BGBCC_CCXL_EmitJump(ctx, lbl);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpFalse(BGBCC_TransState *ctx, ccxl_label lbl)
{
	ccxl_register reg;
	ccxl_type ty;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPF);
	BGBCC_CCXLR3_EmitArgLabel(ctx, lbl);

	BGBCC_CCXL_PopRegister(ctx, &reg);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_EQ, reg, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpTrue(BGBCC_TransState *ctx, ccxl_label lbl)
{
	ccxl_register reg;
	ccxl_type ty;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPT);
	BGBCC_CCXLR3_EmitArgLabel(ctx, lbl);

	BGBCC_CCXL_PopRegister(ctx, &reg);

	BGBCC_CCXL_StackPhiTemporaries(ctx);

	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_NE, reg, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
//	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_ConvImm(BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty, ccxl_register sreg, ccxl_register *rdreg)
{
	ccxl_register treg2;
	double f;
	float bf;
	s64 li, lj, li0, li1;
	u32 ui;
	int bty, stv, sg;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	if(BGBCC_CCXL_TypeSmallSIntP(ctx, dty))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		BGBCC_CCXL_GetRegForIntValue(ctx, rdreg, i);
		return;
	}
	
	if(BGBCC_CCXL_TypeSmallUIntP(ctx, dty))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);

		li0=li;
		li=(u32)li;

//		if(li0!=li)
		if((li0!=li) && (li0!=((s32)li)))
		{
			BGBCC_CCXL_Warn(ctx, "Constant Conversion, Loss of Range\n");
		}
			

//		BGBCC_CCXL_GetRegForLongValue(ctx, rdreg, li);
		BGBCC_CCXL_GetRegForUIntValue(ctx, rdreg, li);
		return;
	}

//	if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
	if(BGBCC_CCXL_TypeSgLongP(ctx, dty))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
		if(BGBCC_CCXL_TypeUnsignedP(ctx, dty))
			BGBCC_CCXL_GetRegForULongValue(ctx, rdreg, li);
		else
			BGBCC_CCXL_GetRegForLongValue(ctx, rdreg, li);
		return;
	}

	if(BGBCC_CCXL_TypeSgNLongP(ctx, dty))
	{
		if(ctx->arch_sizeof_long==8)
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			if(BGBCC_CCXL_TypeUnsignedP(ctx, dty))
				BGBCC_CCXL_GetRegForULongValue(ctx, rdreg, li);
			else
				BGBCC_CCXL_GetRegForLongValue(ctx, rdreg, li);
			return;
		}else
			if(ctx->arch_sizeof_long==4)
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			if(BGBCC_CCXL_TypeUnsignedP(ctx, dty))
				BGBCC_CCXL_GetRegForUIntValue(ctx, rdreg, li);
			else
				BGBCC_CCXL_GetRegForIntValue(ctx, rdreg, li);
			return;
		}
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, dty))
	{
		BGBCC_CCXL_GetRegImmInt128Value(ctx, sreg, &li, &lj);
//		if(BGBCC_CCXL_TypeUnsignedP(ctx, dty))
//			BGBCC_CCXL_GetRegForUInt128Value(ctx, rdreg, li, lj);
//		else
			BGBCC_CCXL_GetRegForInt128Value(ctx, rdreg, li, lj);
		return;
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmUIntP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmLongP(ctx, sreg) ||
		BGBCC_CCXL_IsRegImmULongP(ctx, sreg))
	{
		if(BGBCC_CCXL_TypeBcd64P(ctx, dty))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			
			sg=0;
			if(li<0)
				{ li=-li; sg=1; }
			
			li0=0;
			for(i=0; i<16; i++)
			{
				j=li%10;
				li=li/10;
				li0|=((u64)j)<<(i*4);
			}
			
			if(sg)
				{ li0=0x9999999999999999ULL-li0; }

			BGBCC_CCXL_GetRegForX128Value(ctx,
				rdreg, li0, 0, CCXL_REGVEC_TY_BCD64);
			return;
		}

		if(BGBCC_CCXL_TypeBcd128P(ctx, dty))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			
			sg=0;
			if(li<0)
				{ li=-li; sg=1; }
			
			li0=0;
			li1=0;
			for(i=0; i<16; i++)
			{
				j=li%10;
				li=li/10;
				li0|=((u64)j)<<(i*4);
			}
			for(i=0; i<16; i++)
			{
				j=li%10;
				li=li/10;
				li1|=((u64)j)<<(i*4);
			}
			
			if(sg)
			{
				li0=0x9999999999999999ULL-li0;
				li1=0x9999999999999999ULL-li1;
			}

			BGBCC_CCXL_GetRegForX128Value(ctx,
				rdreg, li0, li1, CCXL_REGVEC_TY_BCD128);
			return;
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, dty))
	{
		bf=BGBCC_CCXL_GetRegImmFloatValue(ctx, sreg);

//		memcpy(&ui, &bf, 4);
//		ui=((ui+4)>>3)<<3;
//		memcpy(&bf, &ui, 4);

		BGBCC_CCXL_GetRegForFloatValue(ctx, rdreg, bf);

//		f=BGBCC_CCXL_GetRegImmFloatValue(ctx, sreg);
//		BGBCC_CCXL_GetRegForFloatValue(ctx, rdreg, f);
		return;
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
	{
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, sreg);
		BGBCC_CCXL_GetRegForDoubleValue(ctx, rdreg, f);
		return;
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, dty))
	{
		BGBCC_CCXL_GetRegImmFloat128Value(ctx, sreg, &li, &lj);
		BGBCC_CCXL_GetRegForFloat128Value(ctx, rdreg, li, lj);
		return;
	}

	if(	BGBCC_CCXL_TypeFloat16P(ctx, dty)	||
		BGBCC_CCXL_TypeBFloat16P(ctx, dty)	)
	{
		bf=BGBCC_CCXL_GetRegImmFloatValue(ctx, sreg);
//		ui=*(u32 *)(&bf);
		memcpy(&ui, &bf, 4);
		ui=((ui+4096)>>13)<<13;
//		*(u32 *)(&bf)=ui;
		memcpy(&bf, &ui, 4);
		BGBCC_CCXL_GetRegForFloatValue(ctx, rdreg, bf);
		return;
	}

	if(	BGBCC_CCXL_TypePointerP(ctx, dty) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, dty) &&
		BGBCC_CCXL_IsRegImmILP(ctx, sreg))
	{
		if(ctx->arch_sizeof_ptr==8)
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_CCXL_GetRegForLongValue(ctx, &treg2, li);
			*rdreg=treg2;
			return;
		}

		if(ctx->arch_sizeof_ptr==4)
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_CCXL_GetRegForUIntValue(ctx, &treg2, li);
			*rdreg=treg2;
			return;
		}
	}
	
	if(BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
	{
		if(BGBCC_CCXL_IsRegImmInt128P(ctx, sreg))
		{
			*rdreg=sreg;
			return;
		}

		if(BGBCC_CCXL_IsRegImmILP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			lj=0;
			BGBCC_CCXL_GetRegForInt128Value(ctx, &treg2, li, lj);
			*rdreg=treg2;
			return;
		}
	}

	*rdreg=sreg;
}

void BGBCC_CCXL_CompileJmpCond(BGBCC_TransState *ctx,
	char *op, ccxl_label lbl)
{
	ccxl_register rega, regb;
	ccxl_register rega2, regb2;
	ccxl_type sty, tty, dty;
//	ccxl_type ty;
	int opr;

	if(!op)return;
	
	opr=-1;
	if(!strcmp(op, "=="))	{ opr=CCXL_CMP_EQ; }
	if(!strcmp(op, "!="))	{ opr=CCXL_CMP_NE; }
	if(!strcmp(op, "<"))	{ opr=CCXL_CMP_LT; }
	if(!strcmp(op, ">"))	{ opr=CCXL_CMP_GT; }
	if(!strcmp(op, "<="))	{ opr=CCXL_CMP_LE; }
	if(!strcmp(op, ">="))	{ opr=CCXL_CMP_GE; }

	if(!strcmp(op, "&"))	{ opr=CCXL_CMP_TST; }
	if(!strcmp(op, "!&"))	{ opr=CCXL_CMP_NTST; }
	if(!strcmp(op, "!!&"))	{ opr=CCXL_CMP_TST; }

//	if(!strcmp(op, "&&&"))	{ opr=CCXL_CMP_TST; }

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JCMP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);
	BGBCC_CCXLR3_EmitArgLabel(ctx, lbl);

	BGBCC_CCXL_PopRegister(ctx, &regb);
	BGBCC_CCXL_PopRegister(ctx, &rega);

#if 1
	if(	 BGBCC_CCXL_IsRegImmILFDP(ctx, rega) &&
		!BGBCC_CCXL_IsRegImmILFDP(ctx, regb))
	{
		switch(opr)
		{
			case CCXL_CMP_LT: opr=CCXL_CMP_GT; break;
			case CCXL_CMP_GT: opr=CCXL_CMP_LT; break;
			case CCXL_CMP_LE: opr=CCXL_CMP_GE; break;
			case CCXL_CMP_GE: opr=CCXL_CMP_LE; break;
		}
		
		rega2=rega;
		rega=regb;
		regb=rega2;
	}
#endif

	sty=BGBCC_CCXL_GetRegType(ctx, rega);
	tty=BGBCC_CCXL_GetRegType(ctx, regb);
	BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &dty);

#if 1
//	if(	!BGBCC_CCXL_TypeEqualP(ctx, dty, sty) ||
//		!BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
	if(	!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) ||
		!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, tty))
	{
//		if(BGBCC_CCXL_IsRegImmP(ctx, regb) &&
//			(BGBCC_CCXL_TypeDoubleP(ctx, dty) ||
//			 BGBCC_CCXL_TypeSgLongP(ctx, dty)))
		if(BGBCC_CCXL_IsRegImmILFDP(ctx, regb) &&
//			BGBCC_CCXL_TypeSmallDoubleP(ctx, dty))
			BGBCC_CCXL_TypeSmallFloat128P(ctx, dty) &&
			BGBCC_CCXL_TypeSmallTypeP(ctx, sty, tty))
		{
			dty=sty; tty=sty;
			BGBCC_CCXL_ConvImm(ctx, dty, tty, regb, &regb);
		}

		if(BGBCC_CCXL_IsRegImmILFDP(ctx, regb) &&
			BGBCC_CCXL_TypeRealP(ctx, dty) &&
			BGBCC_CCXL_TypeSmallTypeP(ctx, dty, tty))
		{
			BGBCC_CCXL_ConvImm(ctx, dty, tty, regb, &regb);
			tty=dty;
		}
	}
#endif

//	if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//		BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
//	if(	BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) &&
//		BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, tty))
	if(	BGBCC_CCXL_TypeCompatibleValueExtP(ctx, dty, sty) &&
		BGBCC_CCXL_TypeCompatibleValueExtP(ctx, dty, tty))
	{
		BGBCC_CCXL_StackPhiTemporaries(ctx);

//		ty=BGBCC_CCXL_GetRegType(ctx, rega);
		BGBCC_CCXL_EmitJumpRegCmp(ctx, dty, opr, rega, regb, lbl);
		BGBCC_CCXL_RegisterCheckRelease(ctx, rega);
		BGBCC_CCXL_RegisterCheckRelease(ctx, regb);
//		BGBCC_CCXL_EmitMarkEndTrace(ctx);
		return;
	}

//	if(!BGBCC_CCXL_TypeEqualP(ctx, dty, sty))
//	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty))
	if(!BGBCC_CCXL_TypeCompatibleValueExtP(ctx, dty, sty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &rega2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, rega2, rega);
		BGBCC_CCXL_RegisterCheckRelease(ctx, rega);
	}else
	{
		rega2=rega;
	}

//	if(!BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
//	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, tty))
	if(!BGBCC_CCXL_TypeCompatibleValueExtP(ctx, dty, tty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &regb2);
		BGBCC_CCXL_EmitConv(ctx, dty, tty, regb2, regb);
		BGBCC_CCXL_RegisterCheckRelease(ctx, regb);
	}else
	{
		regb2=regb;
	}

#if 1
	if(	(rega2.val == regb2.val) &&
		(	BGBCC_CCXL_TypeSmallInt128P(ctx, dty) ||
			(	BGBCC_CCXL_TypeSmallFloat128P(ctx, dty) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, regb) 	) ||
			(	BGBCC_CCXL_TypeSmallFloat128P(ctx, dty) &&
				(opr!=CCXL_CMP_EQ) &&
				(opr!=CCXL_CMP_NE) 	) ||
			BGBCC_CCXL_TypePointerP(ctx, dty)	)
		)
	{
		/*
		 If comparing a value to itself, it is either equal or not equal.
		 Though, this is not entirely true with floating point types,
		 due mostly to NaN and similar.
		 */
	
		if(	(opr==CCXL_CMP_EQ) ||
			(opr==CCXL_CMP_LE) ||
			(opr==CCXL_CMP_GE) )
		{
			BGBCC_CCXL_EmitJump(ctx, lbl);
			BGBCC_CCXL_EmitMarkEndTrace(ctx);
			BGBCC_CCXL_RegisterCheckRelease(ctx, rega2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, regb2);
			return;
		}

		if(	(opr==CCXL_CMP_NE) ||
			(opr==CCXL_CMP_GT) ||
			(opr==CCXL_CMP_LT) )
		{
//			BGBCC_CCXL_EmitJump(ctx, lbl);
//			BGBCC_CCXL_EmitMarkEndTrace(ctx);
			BGBCC_CCXL_RegisterCheckRelease(ctx, rega2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, regb2);
			return;
		}
	}
#endif

	BGBCC_CCXL_EmitJumpRegCmp(ctx, dty, opr, rega2, regb2, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, rega2);
	BGBCC_CCXL_RegisterCheckRelease(ctx, regb2);
}

void BGBCC_CCXL_CompileJCO(BGBCC_TransState *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn, ccxl_label lbl)
{
	BCCX_Node *lt, *rt;
	BCCX_Node *t0, *t1;
	int i, j, k;

	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	BGBCC_CCXL_CompileExpr(ctx, ln);
	BGBCC_CCXL_CompileExpr(ctx, rn);
	BGBCC_CCXL_CompileJmpCond(ctx, op, lbl);
}

void BGBCC_CCXL_CompileJCT(BGBCC_TransState *ctx,
	BCCX_Node *l, ccxl_label lbl)
{
	BCCX_Node *t, *ln, *rn;
	ccxl_label lbl1;
	char *op;
	int i;

	if(!l)
	{
		BGBCC_DBGBREAK
		printf("BGBCC_CCXL_CompileJCT: No Expr\n");
		BGBCC_CCXL_CompileJmp(ctx, lbl);
		return;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);

	if(	BGBCC_CCXL_IsUnaryP(ctx, l, "!") ||
		BGBCC_CCXL_IsUnaryP(ctx, l, "!!"))
	{
		BGBCC_CCXL_CompileJCF(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), lbl);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
			BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, lbl, lbl1);
		BGBCC_CCXL_EmitLabel(ctx, lbl1);
		return;
	}

	op=NULL;
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "=="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "==="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!=="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<"))op="<";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">"))op=">";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<="))op="<=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">="))op=">=";

#if 1
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&"))
		op="&";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!&"))
		op="!&";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!!&"))
		op="!!&";

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&&"))
		op="&";
#endif

	if(!op)
	{
		if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
			i=-1;

		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_CompileJmpTrue(ctx, lbl);
		return;
	}

	ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
	rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
	BGBCC_CCXL_CompileJCO(ctx, op, ln, rn, lbl);
}

void BGBCC_CCXL_CompileJCF(BGBCC_TransState *ctx, BCCX_Node *l, ccxl_label lbl)
{
	BCCX_Node *t, *ln, *rn;
	ccxl_label lbl1;
	char *op;
	int i;

	if(!l)
	{
		BGBCC_DBGBREAK
		printf("BGBCC_CCXL_CompileJCF: No Expr\n");
		return;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);
//	t=BGBCC_CCXL_InferExpr(ctx, l);

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "!") ||
		BGBCC_CCXL_IsUnaryP(ctx, l, "!!"))
	{
		BGBCC_CCXL_CompileJCT(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), lbl);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
		BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, lbl1, lbl);
		BGBCC_CCXL_EmitLabel(ctx, lbl1);
		return;
	}

	op=NULL;
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "=="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "==="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!=="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<"))op=">=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">"))op="<=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<="))op=">";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">="))op="<";

#if 1
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&"))
		op="!&";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!&"))
		op="!!&";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!!&"))
		op="!&";

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&&"))
		op="!&";
#endif

	if(!op)
	{
		if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
			i=-1;

		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_CompileJmpFalse(ctx, lbl);
		return;
	}

	ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
	rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
	BGBCC_CCXL_CompileJCO(ctx, op, ln, rn, lbl);
}

int BGBCC_CCXL_TryGetSizeofType(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	static int rcp=0;
	int sza[2], ala[2];
	BCCX_Node *n0;
	char *s;
	int i, na;
	
	if(!ctx)return(-1);
	if(!ty)return(-1);

	n0=BCCX_FindTagCst(ty, &bgbcc_rcst_size, "size");
	if(n0)
	{
		rcp=1;
		s=BGBCC_CCXL_VarTypeString(ctx, ty);
		
		if(!strncmp(s, "A0", 2))
			return(-1);
		
		i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
		rcp=0;
		if(i>=0)
		{
			if(sza[0]==sza[1])
				return(sza[0]);
		}

		return(-1);

//		na=BCCX_GetNodeChildCount(n0);
//		if(na>0)
//			return(-1);
	}

	i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
	if(i)
	{
//		if(BGBCC_IsEmitRil(ctx))
//			return(-1);
	
		if(ctx->arch_sizeof_ptr)
			return(ctx->arch_sizeof_ptr);
		return(-1);
	}

	s=NULL;
	s=BCCX_Get(ty, "name");

	if(s)
	{
		if(!strcmp(s, "char"))return(1);
		if(!strcmp(s, "short"))return(2);
		if(!strcmp(s, "int"))return(4);
		if(!strcmp(s, "llong"))return(8);
		if(!strcmp(s, "ullong"))return(8);

		if(!strcmp(s, "float16"))return(2);
		if(!strcmp(s, "int128"))return(16);
		if(!strcmp(s, "float128"))return(16);

	//	if(ctx->arch==BGBCC_ARCH_X86)
		if(1)
		{
			if(!strcmp(s, "long") || !strcmp(s, "ulong"))
			{
	//			if(BGBCC_IsEmitRil(ctx))
	//				return(-1);
				if(ctx->arch_sizeof_long)
					return(ctx->arch_sizeof_long);
				return(-1);
			}
		}

	//	if(ctx->arch==BGBCC_ARCH_X64)
		if(0)
		{
			if(!strcmp(s, "long"))return(8);
		}
	}
	
	if(!rcp)
	{
		rcp=1;
		s=BGBCC_CCXL_VarTypeString(ctx, ty);
		i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
		rcp=0;
		if(i>=0)
		{
			if(sza[0]==sza[1])
				return(sza[0]);
		}

		return(-1);
	}
	
	return(-1);
}

int BGBCC_CCXL_GetMinMaxSizeofType(BGBCC_TransState *ctx, BCCX_Node *ty,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	char *s;
	int i;
	
	if(!rcp)
	{
		rcp=1;
		s=BGBCC_CCXL_VarTypeString(ctx, ty);
		i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
		rcp=0;
		if(i>=0)
		{
			if(rmsz)*rmsz=sza[0];
			if(rnsz)*rnsz=sza[1];
			if(rmal)*rmal=ala[0];
			if(rnal)*rnal=ala[1];
			return(0);
		}

		return(-1);
	}
	
	return(-1);
}


int BGBCC_CCXL_TryGetOffsetofSig(
	BGBCC_TransState *ctx,
	char *sig, char *name)
{
	char tname[256];
	BGBCC_CCXL_LiteralInfo *st;
	BGBCC_CCXL_RegisterInfo *fi;
	ccxl_register sreg, dreg;
	ccxl_type bty;
	char *cs, *ct;
	int fn, foffs, fsoffs;
	int i;

	if(!ctx)return(-1);
	if(!sig)return(-1);
	if(!name)return(-1);
	
	cs=name; ct=tname;
	while(*cs)
	{
		if(*cs=='/')
			break;
		*ct++=*cs++;
	}
	if(*cs=='/')
		cs++;
	*ct++=0;
	
	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
	st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
	if(st->decl && st->decl->fxmsize &&
		(st->decl->fxmsize==st->decl->fxnsize))
	{
		fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, tname);
		fi=st->decl->fields[fn];

		foffs=-1;
		fsoffs=0;

		if(fi->fxmoffs==fi->fxnoffs)
			foffs=fi->fxmoffs;

		if(*cs)
		{
			fsoffs=BGBCC_CCXL_TryGetOffsetofSig(ctx, fi->sig, cs);
		}

		if((foffs>=0) && (fsoffs>=0))
			return(foffs+fsoffs);
		return(-1);
		
//		if(fi->fxmoffs==fi->fxnoffs)
//		{
//			return(fi->fxmoffs);
//		}
	}
	return(-1);
}

int BGBCC_CCXL_TryGetOffsetofType(BGBCC_TransState *ctx,
	BCCX_Node *ty, char *name)
{
	int sza[2], ala[2];
	char *s;
	int i;
	
	if(!ctx)return(-1);
	if(!ty)return(-1);
	if(!name)return(-1);
	
	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	i=BGBCC_CCXL_TryGetOffsetofSig(ctx, s, name);
	return(i);
}


int BGBCC_CCXL_TryGetSizeofName(BGBCC_TransState *ctx, char *name)
{
	static int rcp=0;
	int sza[2], ala[2];
	BGBCC_CCXL_RegisterInfo *gbl;
	BGBCC_CCXL_LiteralInfo *litobj;
	ccxl_register treg;
	ccxl_type tty;
	ccxl_type ty;
	char *s;
	int sz, sz1, asz;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, &treg, false);
	if(i<=0)return(-1);

	ty=BGBCC_CCXL_GetRegType(ctx, treg);
	s=BGBCC_CCXL_TypeGetSig(ctx, ty);

	i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
	rcp=0;
	if(i>=0)
	{
		if((sza[0]==sza[1]) && (sza[0]>0))
			return(sza[0]);
	}

	gbl=NULL;

	if(BGBCC_CCXL_IsRegGlobalP(ctx, treg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, treg);
		gbl=ctx->reg_globals[i];
	}
	
	if(!gbl)
		return(-1);

	ty=gbl->type;

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, ty, &tty);
		sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
		if(sz1<=0)
			return(-1);

		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, gbl->value))
		{
			i=gbl->value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];

			asz=litobj->decl->n_listdata;

			sz=sz1*asz;
			if(sz>0)
				return(sz);
		}

//		asz=BGBCC_CCXL_TypeGetArraySize(ctx, ty);
//		sz=sz1*asz;
//		if(sz>0)
//			return(sz);
		return(-1);
	}

	return(-1);
}

int BGBCC_CCXL_GetMinMaxSizeofName(BGBCC_TransState *ctx, char *name,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	ccxl_register treg;
	ccxl_type ty;
	char *s;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, &treg, false);
	if(i<=0)return(-1);

	ty=BGBCC_CCXL_GetRegType(ctx, treg);
	s=BGBCC_CCXL_TypeGetSig(ctx, ty);

	i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
	rcp=0;
	if(i>=0)
	{
		if(rmsz)*rmsz=sza[0];
		if(rnsz)*rnsz=sza[1];
		if(rmal)*rmal=ala[0];
		if(rnal)*rnal=ala[1];
		return(0);
	}

	return(-1);
}

int BGBCC_CCXL_GetMinMaxSizeofDerefName(BGBCC_TransState *ctx, char *name,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	ccxl_register treg;
	ccxl_type ty;
	char *s;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, &treg, false);
	if(i<=0)return(-1);

	ty=BGBCC_CCXL_GetRegDerefType(ctx, treg);
	s=BGBCC_CCXL_TypeGetSig(ctx, ty);

	i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
	rcp=0;
	if(i>=0)
	{
		if(rmsz)*rmsz=sza[0];
		if(rnsz)*rnsz=sza[1];
		if(rmal)*rmal=ala[0];
		if(rnal)*rnal=ala[1];
		return(0);
	}

	return(-1);
}

int BGBCC_CCXL_StackCSelCmp(BGBCC_TransState *ctx, char *op)
{
	ccxl_register sreg, treg, sreg2, treg2;
	ccxl_register ureg, vreg, ureg2, vreg2;
	ccxl_register dreg, dreg2;
	ccxl_type sty, tty, dty, bty, pty, uty, vty;
	double f, g;
	int opr;
	int i, j, k;

	opr=-1;

	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx, "CSelCmpOp", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CSELCMP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr|0x00);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &pty);

		j=BGBCC_CCXL_PopRegister(ctx, &vreg);
		i=BGBCC_CCXL_PopRegister(ctx, &ureg);
		uty=BGBCC_CCXL_GetRegType(ctx, ureg);
		vty=BGBCC_CCXL_GetRegType(ctx, vreg);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, uty, vty, &dty);

		if(BGBCC_CCXL_TypeCompatibleP(ctx, pty, sty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &sreg2);
			BGBCC_CCXL_EmitConv(ctx, pty, sty, sreg2, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			sreg=sreg2;
		}

		if(BGBCC_CCXL_TypeCompatibleP(ctx, pty, tty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &treg2);
			BGBCC_CCXL_EmitConv(ctx, pty, tty, treg2, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			treg=treg2;
		}


		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, uty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &ureg2);
			BGBCC_CCXL_EmitConv(ctx, dty, uty, ureg2, ureg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, ureg);
			ureg=ureg2;
		}

		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, vty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &vreg2);
			BGBCC_CCXL_EmitConv(ctx, dty, vty, vreg2, vreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, vreg);
			vreg=vreg2;
		}

		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);

		BGBCC_CCXL_EmitCSelCmp(ctx,
			dty, dreg, ureg, vreg,
			pty, opr, sreg, treg);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, ureg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, vreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_NO);
}
	
int BGBCC_CCXL_StackCSelCmpZero(BGBCC_TransState *ctx, char *op)
{
	ccxl_register sreg, treg, sreg2, treg2;
	ccxl_register ureg, vreg, ureg2, vreg2;
	ccxl_register dreg, dreg2;
	ccxl_type sty, tty, dty, bty, pty, uty, vty;
	double f, g;
	int opr;
	int i, j, k;

	opr=-1;

	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx, "CSelCmpOpZero", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CSELCMP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr|0x10);

		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);

		j=BGBCC_CCXL_PopRegister(ctx, &vreg);
		i=BGBCC_CCXL_PopRegister(ctx, &ureg);
		uty=BGBCC_CCXL_GetRegType(ctx, ureg);
		vty=BGBCC_CCXL_GetRegType(ctx, vreg);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, uty, vty, &dty);


		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, uty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &ureg2);
			BGBCC_CCXL_EmitConv(ctx, dty, uty, ureg2, ureg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, ureg);
			ureg=ureg2;
		}

		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, vty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &vreg2);
			BGBCC_CCXL_EmitConv(ctx, dty, vty, vreg2, vreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, vreg);
			vreg=vreg2;
		}

		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
		BGBCC_CCXL_EmitCSelCmpZero(ctx,
			dty, dreg, ureg, vreg,
			sty, opr, sreg);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, ureg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, vreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_NO);
}
	
void BGBCC_CCXL_CompileCSelCmp(BGBCC_TransState *ctx,
	char *op, BCCX_Node *ln, BCCX_Node *rn)
{
	BGBCC_CCXL_CompileExpr(ctx, ln);
	BGBCC_CCXL_CompileExpr(ctx, rn);
	BGBCC_CCXL_StackCSelCmp(ctx, op);
}

void BGBCC_CCXL_CompileCSel(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	ccxl_label lbl1;
	char *op;

	if(!l)
	{
		BGBCC_DBGBREAK
		printf("BGBCC_CCXL_CompileJCF: No Expr\n");
		return;
	}

//	l=BGBCC_CCXL_ReduceExpr(ctx, l);
//	t=BGBCC_CCXL_InferExpr(ctx, l);

#if 0
	if(BGBCC_CCXL_IsUnaryP(ctx, l, "!") ||
		BGBCC_CCXL_IsUnaryP(ctx, l, "!!"))
	{
		BGBCC_CCXL_CompileJCT(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), lbl);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
		BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, lbl1, lbl);
		BGBCC_CCXL_EmitLabel(ctx, lbl1);
		return;
	}
#endif

	op=NULL;
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "=="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "==="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!=="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<"))op=">=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">"))op="<=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<="))op=">";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">="))op="<";

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&"))op="!&";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!&"))op="!!&";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!!&"))op="!&";

	if(!op)
	{
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_CompileJmpFalse(ctx, lbl);
		return;
	}

	BGBCC_CCXL_CompileCSelCmp(ctx, op,
		BCCX_FetchCst(l, &bgbcc_rcst_left, "left"), BCCX_FetchCst(l, &bgbcc_rcst_right, "right"));
	return;
}


ccxl_status BGBCC_CCXL_StackCompileJmpTab(BGBCC_TransState *ctx,
	int clm, int cln, int clmw, ccxl_label *clbl, s64 *clv, s64 *clmv,
	ccxl_label dfl, ccxl_label dfl2)
{
	ccxl_register sreg;
	ccxl_type bty;
	s64 clmm;
	int ncl, isz;
	int i, j, k;
	
	ncl=cln-clm;
//	if((ncl<1) || (ncl>256))
	if((ncl<1) || (ncl>1024))
		{ BGBCC_DBGBREAK }

	if(clmw<64)
	{
		clmm=(1ULL<<clmw)-1;
	}else
	{
		clmm=-1;
	}

	isz=0;
	for(i=0; i<ncl; i++)
	{
		if(clmv[clm+i]&clmm)
			isz=1;
	}

	if(isz)
	{
		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPTABZ);
		BGBCC_CCXLR3_EmitArgInt(ctx, dfl.id?ncl:(-ncl));
		BGBCC_CCXLR3_EmitArgInt(ctx, clmw);
		BGBCC_CCXLR3_EmitArgLabel(ctx, dfl2);

		for(i=0; i<ncl; i++)
		{
			BGBCC_CCXLR3_EmitArgInt(ctx, clv[clm+i]);
			BGBCC_CCXLR3_EmitArgInt(ctx, clmv[clm+i]);
			BGBCC_CCXLR3_EmitArgLabel(ctx, clbl[clm+i]);
		}
	}else
	{
		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_JMPTAB);
		BGBCC_CCXLR3_EmitArgInt(ctx, dfl.id?ncl:(-ncl));
		BGBCC_CCXLR3_EmitArgLabel(ctx, dfl2);

		for(i=0; i<ncl; i++)
		{
			BGBCC_CCXLR3_EmitArgInt(ctx, clv[clm+i]);
			BGBCC_CCXLR3_EmitArgLabel(ctx, clbl[clm+i]);
		}
	}

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	bty=BGBCC_CCXL_GetRegType(ctx, sreg);

	BGBCC_CCXL_EmitJmpTab(ctx, bty, sreg,
		clm, cln, clmw,
		clbl, clv, clmv,
		dfl, dfl2);
	return(CCXL_STATUS_YES);
}



void BGBCC_CCXL_CompilePredCmp(BGBCC_TransState *ctx, char *op)
{
	ccxl_register rega, regb;
	ccxl_register rega2, regb2;
	ccxl_type sty, tty, dty;
//	ccxl_type ty;
	int opr;

	if(!op)return;

#if 0	
	opr=-1;
	if(!strcmp(op, "=="))	{ opr=CCXL_CMP_EQ; }
	if(!strcmp(op, "!="))	{ opr=CCXL_CMP_NE; }
	if(!strcmp(op, "<"))	{ opr=CCXL_CMP_LT; }
	if(!strcmp(op, ">"))	{ opr=CCXL_CMP_GT; }
	if(!strcmp(op, "<="))	{ opr=CCXL_CMP_LE; }
	if(!strcmp(op, ">="))	{ opr=CCXL_CMP_GE; }

	if(!strcmp(op, "&"))	{ opr=CCXL_CMP_TST; }
	if(!strcmp(op, "!&"))	{ opr=CCXL_CMP_NTST; }
	if(!strcmp(op, "!!&"))	{ opr=CCXL_CMP_TST; }
#endif

	opr=BGBCC_CCXL_CompareOpIdForName(ctx, op);

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_PRCMP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);

	BGBCC_CCXL_PopRegister(ctx, &regb);
	BGBCC_CCXL_PopRegister(ctx, &rega);

	sty=BGBCC_CCXL_GetRegType(ctx, rega);
	tty=BGBCC_CCXL_GetRegType(ctx, regb);
	BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &dty);

// BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty)

//	if(	!BGBCC_CCXL_TypeEqualP(ctx, dty, sty) ||
//		!BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
	if(	!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) ||
		!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, tty))
	{
		if(BGBCC_CCXL_IsRegImmILFDP(ctx, regb) &&
			BGBCC_CCXL_TypeSmallFloat128P(ctx, dty) &&
			BGBCC_CCXL_TypeSmallTypeP(ctx, sty, tty))
		{
			dty=sty; tty=sty;
			BGBCC_CCXL_ConvImm(ctx, dty, tty, regb, &regb);
		}
	}

//	if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//		BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
	if(	BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty) &&
		BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, tty))
	{
		BGBCC_CCXL_StackPhiTemporaries(ctx);

		BGBCC_CCXL_EmitPredCmpOp(ctx, dty, opr, rega, regb);
		BGBCC_CCXL_RegisterCheckRelease(ctx, rega);
		BGBCC_CCXL_RegisterCheckRelease(ctx, regb);
		BGBCC_CCXL_EmitMarkEndTrace(ctx);
		return;
	}

//	if(!BGBCC_CCXL_TypeEqualP(ctx, dty, sty))
	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, sty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &rega2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, rega2, rega);
		BGBCC_CCXL_RegisterCheckRelease(ctx, rega);
	}else
	{
		rega2=rega;
	}

//	if(!BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
	if(!BGBCC_CCXL_TypeCompatibleArchP(ctx, dty, tty))
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &regb2);
		BGBCC_CCXL_EmitConv(ctx, dty, tty, regb2, regb);
		BGBCC_CCXL_RegisterCheckRelease(ctx, regb);
	}else
	{
		regb2=regb;
	}

	BGBCC_CCXL_EmitPredCmpOp(ctx, dty, opr, rega2, regb2);
	BGBCC_CCXL_RegisterCheckRelease(ctx, rega2);
	BGBCC_CCXL_RegisterCheckRelease(ctx, regb2);
}

void BGBCC_CCXL_CompilePredCmpZero(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_StackPushConstInt(ctx, 0);
	BGBCC_CCXL_CompilePredCmp(ctx, "==");
}

void BGBCC_CCXL_CompilePredCmpNonZero(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_StackPushConstInt(ctx, 0);
	BGBCC_CCXL_CompilePredCmp(ctx, "!=");
}

int BGBCC_CCXL_CompilePredExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *ln, *rn;
	ccxl_type bty, dty, lty, rty;
	char *s0;
	int opr;
	int i0, i1, i2, i3;
	int i, j, k;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		opr=BGBCC_CCXL_CompareOpIdForName(ctx, s0);
		
		if(opr>=0)
		{
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_CompilePredCmp(ctx, s0);
			return(1);
		}

		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_CompilePredCmpNonZero(ctx);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		
		if(	!strcmp(s0, "!") ||
			!strcmp(s0, "!!"))
		{
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompilePredCmpZero(ctx);
			return(1);
		}
		
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_CompilePredCmpNonZero(ctx);
		return(0);
	}

	BGBCC_CCXL_CompileExpr(ctx, l);
	BGBCC_CCXL_CompilePredCmpNonZero(ctx);
	return(1);
}
