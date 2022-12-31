char *BGBCC_CCXL_StackGetSigBinary(BGBCC_TransState *ctx,
	ccxl_type lty, ccxl_type rty)
{
	char tb[256];
	char *s0, *ct;

	ct=tb;
	*ct++='(';

	s0=BGBCC_CCXL_TypeGetSig(ctx, lty);
	strcpy(ct, s0);
	ct+=strlen(ct);

	s0=BGBCC_CCXL_TypeGetSig(ctx, rty);
	strcpy(ct, s0);
	ct+=strlen(ct);

	*ct++=')';
	*ct++=0;

	return(bgbcc_strdup(tb));
}

char *BGBCC_CCXL_StackGetNameBinaryOverload(
	BGBCC_TransState *ctx,
	char *op)
{
	char *top;

	if(!strcmp(op, "+"))
		top="__operator_add";
	if(!strcmp(op, "-"))
		top="__operator_sub";
	if(!strcmp(op, "*"))
		top="__operator_mul";
	if(!strcmp(op, "/"))
		top="__operator_div";
	if(!strcmp(op, "%"))
		top="__operator_mod";

	if(!strcmp(op, "&"))
		top="__operator_and";
	if(!strcmp(op, "|"))
		top="__operator_or";
	if(!strcmp(op, "^"))
		top="__operator_xor";

	if(!strcmp(op, "=="))
		top="__operator_eq";
	if(!strcmp(op, "!="))
		top="__operator_ne";
	if(!strcmp(op, "<"))
		top="__operator_lt";
	if(!strcmp(op, ">"))
		top="__operator_gt";
	if(!strcmp(op, "<="))
		top="__operator_le";
	if(!strcmp(op, ">="))
		top="__operator_ge";
	
	return(top);
}

char *BGBCC_CCXL_StackGetMangleNameBinaryOverload(
	BGBCC_TransState *ctx,
	char *op,
	ccxl_type lty, ccxl_type rty)
{
	char tb[256];
	char *top;
	char *tsig;
	
//	top=NULL;
	top=BGBCC_CCXL_StackGetNameBinaryOverload(ctx, op);
	
	if(!top)
		return(NULL);
		
	tsig=BGBCC_CCXL_StackGetSigBinary(ctx, lty, rty);
	
	sprintf(tb, "%s%s", top, tsig);

	return(bgbcc_strdup(tb));
}

int BGBCC_CCXL_BinaryOpIdForName(BGBCC_TransState *ctx, char *op)
{
	int opr;

	if(!op)
		return(-1);

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_BINOP_ADD;
	if(!strcmp(op, "-"))opr=CCXL_BINOP_SUB;
	if(!strcmp(op, "*"))opr=CCXL_BINOP_MUL;
	if(!strcmp(op, "/"))opr=CCXL_BINOP_DIV;
	if(!strcmp(op, "%"))opr=CCXL_BINOP_MOD;
	if(!strcmp(op, "&"))opr=CCXL_BINOP_AND;
	if(!strcmp(op, "|"))opr=CCXL_BINOP_OR;
	if(!strcmp(op, "^"))opr=CCXL_BINOP_XOR;
	if(!strcmp(op, "<<"))opr=CCXL_BINOP_SHL;
	if(!strcmp(op, ">>"))opr=CCXL_BINOP_SHR;
	if(!strcmp(op, ">>>"))opr=CCXL_BINOP_SHRR;

	if(!strcmp(op, "CONS"))
		opr=CCXL_BINOP_CONS;

#if 0
	if(!strcmp(op, "=="))opr=CCXL_BINOP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_BINOP_NE;
	if(!strcmp(op, "<"))opr=CCXL_BINOP_LT;
	if(!strcmp(op, ">"))opr=CCXL_BINOP_GT;
	if(!strcmp(op, "<="))opr=CCXL_BINOP_LE;
	if(!strcmp(op, ">="))opr=CCXL_BINOP_GE;
#endif

	return(opr);
}

#if 0
// int BGBCC_CCXL_BinaryCmpOpIdForName(BGBCC_TransState *ctx, char *op)
{
	int opr;

	if(!op)
		return(-1);

	opr=-1;
	if(!strcmp(op, "=="))opr=CCXL_BINOP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_BINOP_NE;
	if(!strcmp(op, "<"))opr=CCXL_BINOP_LT;
	if(!strcmp(op, ">"))opr=CCXL_BINOP_GT;
	if(!strcmp(op, "<="))opr=CCXL_BINOP_LE;
	if(!strcmp(op, ">="))opr=CCXL_BINOP_GE;

	return(opr);
}
#endif

int BGBCC_CCXL_CompareOpIdForName(BGBCC_TransState *ctx, char *op)
{
	int opr;

	if(!op)
		return(-1);

	opr=-1;
	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;

	return(opr);
}

ccxl_status BGBCC_CCXL_StackBinaryOp(BGBCC_TransState *ctx, char *op)
{
	ccxl_register sreg, treg, dreg, sreg2, treg2, dreg2;
	ccxl_type sty, tty, dty, dty2, bty, pty;
	double f, g;
	s64 li, lj, lk;
	int opr, is_shl;
	int i, j, k;

	opr=-1;
	if(!strcmp(op, "+"))
		opr=CCXL_BINOP_ADD;
	if(!strcmp(op, "-"))
		opr=CCXL_BINOP_SUB;
	if(!strcmp(op, "*"))
		opr=CCXL_BINOP_MUL;
	if(!strcmp(op, "/"))
		opr=CCXL_BINOP_DIV;
	if(!strcmp(op, "%"))
		opr=CCXL_BINOP_MOD;
	if(!strcmp(op, "&"))
		opr=CCXL_BINOP_AND;
	if(!strcmp(op, "|"))
		opr=CCXL_BINOP_OR;
	if(!strcmp(op, "^"))
		opr=CCXL_BINOP_XOR;
	if(!strcmp(op, "<<"))
		opr=CCXL_BINOP_SHL;
	if(!strcmp(op, ">>"))
		opr=CCXL_BINOP_SHR;
	if(!strcmp(op, ">>>"))
		opr=CCXL_BINOP_SHRR;

	if(!strcmp(op, "CONS"))
		opr=CCXL_BINOP_CONS;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx, "BinaryOp", __FILE__, __LINE__);

		switch(opr)
		{
		case CCXL_BINOP_ADD:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ADD); break;
		case CCXL_BINOP_SUB:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SUB); break;
		case CCXL_BINOP_MUL:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_MUL); break;
		case CCXL_BINOP_AND:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_AND); break;
		case CCXL_BINOP_OR:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_OR); break;
		case CCXL_BINOP_XOR:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_XOR); break;
		case CCXL_BINOP_SHL:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SHL); break;
		case CCXL_BINOP_SHR:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_SAR); break;
		default:
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_BINOP);
			BGBCC_CCXLR3_EmitArgInt(ctx, opr);
			break;
		}

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);

#if 1
		if(BGBCC_CCXL_IsRegImmILFDP(ctx, sreg) &&
			((opr==CCXL_BINOP_ADD) ||
			 (opr==CCXL_BINOP_MUL) ||
			 (opr==CCXL_BINOP_AND) ||
			 (opr==CCXL_BINOP_OR) ||
			 (opr==CCXL_BINOP_XOR)))
		{
			dreg2=sreg;
			sreg=treg;
			treg=dreg2;
		}
#endif

		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		
		if(	BGBCC_CCXL_IsRegImmILFDP(ctx, sreg) &&
			BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
		{
			if(	BGBCC_CCXL_IsRegImmILP(ctx, sreg) &&
				BGBCC_CCXL_IsRegImmILP(ctx, treg))
			{
				li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
				lj=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
				lk=-999999;
				
				switch(opr)
				{
				case CCXL_BINOP_ADD:	lk=li+lj; break;
				case CCXL_BINOP_SUB:	lk=li-lj; break;
				case CCXL_BINOP_MUL:	lk=li*lj; break;
				case CCXL_BINOP_DIV:	lk=li/lj; break;
				case CCXL_BINOP_MOD:	lk=li%lj; break;
				case CCXL_BINOP_AND:	lk=li&lj; break;
				case CCXL_BINOP_OR:		lk=li|lj; break;
				case CCXL_BINOP_XOR:	lk=li^lj; break;

				case CCXL_BINOP_SHL:	lk=li<<((int)lj); break;
				case CCXL_BINOP_SHR:	lk=li>>((int)lj); break;
				}
				
				if(lk!=-999999)
				{
					if(((int)lk)==lk)
						BGBCC_CCXL_GetRegForIntValue(ctx, &dreg, lk);
					else
						BGBCC_CCXL_GetRegForLongValue(ctx, &dreg, lk);
					BGBCC_CCXL_PushRegister(ctx, dreg);
					return(CCXL_STATUS_YES);
				}
			}

			/* Errm... */
		}
		else if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
		{
//			if(	BGBCC_CCXL_TypeFloatP(ctx, sty) ||
//				BGBCC_CCXL_TypeDoubleP(ctx, sty))
			if(BGBCC_CCXL_TypeRealP(ctx, sty) ||
				BGBCC_CCXL_TypeBcdP(ctx, sty))
			{
				BGBCC_CCXL_ConvImm(ctx, sty, tty, treg, &treg);
				tty=sty;
			}
		}else
			if(BGBCC_CCXL_IsRegImmILFDP(ctx, sreg))
		{
//			if(	BGBCC_CCXL_TypeFloatP(ctx, tty) ||
//				BGBCC_CCXL_TypeDoubleP(ctx, tty))
			if(BGBCC_CCXL_TypeRealP(ctx, tty) ||
				BGBCC_CCXL_TypeBcdP(ctx, tty))
			{
				BGBCC_CCXL_ConvImm(ctx, tty, sty, sreg, &sreg);
				sty=tty;
			}
		}

		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);
//		BGBCC_CCXL_TypeAutoPromoteType(ctx, dty, &dty);
		dty2=dty;
		
		if(BGBCC_CCXL_TypeFloatP(ctx, dty2))
		{
			i=-1;
		}

		if(	BGBCC_CCXL_TypeVecP(ctx, sty) &&
			BGBCC_CCXL_TypeVecP(ctx, tty) &&
			!BGBCC_CCXL_TypeVecP(ctx, dty)	)
		{
			BGBCC_CCXL_GetTypeBinaryDest(ctx,
				CCXL_BINOP_ADD, sty, tty, &dty2);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty) &&
			(opr==CCXL_BINOP_SUB))
		{
//			pty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			pty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_NL);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);

			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			BGBCC_CCXL_EmitDiffPtr(ctx, bty, dreg, sreg, treg);

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

			BGBCC_CCXL_PushRegister(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, tty))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				((opr==CCXL_BINOP_ADD) ||
				 (opr==CCXL_BINOP_SUB)))
			{
				k=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if(opr==CCXL_BINOP_SUB)
					k=-k;
				if(k==0)
				{
					j=-1;
				}

				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }
				
				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}
			
			if(opr==CCXL_BINOP_ADD)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}

			BGBCC_DBGBREAK
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				((opr==CCXL_BINOP_ADD) ||
				 (opr==CCXL_BINOP_SUB)))
			{
				k=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if(opr==CCXL_BINOP_SUB)
					k=-k;
				if(k==0)
				{
					j=-1;
				}

				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }
				
				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}
			
			if(opr==CCXL_BINOP_ADD)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				if(BGBCC_CCXL_TypeArrayP(ctx, sty))
					{ BGBCC_CCXL_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);

				BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				return(CCXL_STATUS_YES);
			}
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

#if 1
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				BGBCC_CCXL_TypeSmallLongP(ctx, dty) &&
				BGBCC_CCXL_TypeUnsignedP(ctx, dty))
			{
				j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if((j>1) && !(j&(j-1)))
				{
					k=0;
					while(j>1)	{ j>>=1; k++; }
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
					opr=CCXL_BINOP_SHR;
				}
			}
#endif
		}

#if 1
		if(opr==CCXL_BINOP_DIV)
		{
			if(	BGBCC_CCXL_TypeFloatP(ctx, dty) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(	BGBCC_CCXL_TypeDoubleP(ctx, dty) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}
		}
#endif

		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) &&
			BGBCC_CCXL_TypeSgIntP(ctx, dty2) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, sty) )
		{
			if(		(opr==CCXL_BINOP_ADD) ||
					(opr==CCXL_BINOP_SUB) ||
					(opr==CCXL_BINOP_MUL) )
			{
				BGBCC_CCXL_GetRegAsType(ctx, sreg, dty2, &sreg);
				sty=BGBCC_CCXL_GetRegType(ctx, sreg);
			}
		}

		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) &&
			BGBCC_CCXL_TypeSgIntP(ctx, dty2) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, tty) )
		{
			if(		(opr==CCXL_BINOP_ADD) ||
					(opr==CCXL_BINOP_SUB) ||
					(opr==CCXL_BINOP_MUL) )
			{
				BGBCC_CCXL_GetRegAsType(ctx, treg, dty2, &treg);
				tty=BGBCC_CCXL_GetRegType(ctx, treg);
			}
		}

#if 1
//		if(	!BGBCC_CCXL_TypeEqualP(ctx, dty, sty) ||
//			!BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) ||
			!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty))
		{
//			if(BGBCC_CCXL_IsRegImmP(ctx, treg) &&
//				(BGBCC_CCXL_TypeDoubleP(ctx, dty) ||
//				 BGBCC_CCXL_TypeSmallLongP(ctx, dty)))

			if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg) &&
				!BGBCC_CCXL_IsRegImmUnsignedP(ctx, treg) &&
//				BGBCC_CCXL_TypeSmallDoubleP(ctx, dty))
				BGBCC_CCXL_TypeSmallFloat128P(ctx, dty2) &&
				BGBCC_CCXL_TypeSmallTypeP(ctx, sty, tty))
			{
				dty=sty;
				dty2=sty;
				tty=sty;

				if(	(opr!=CCXL_BINOP_SHL) &&
					(opr!=CCXL_BINOP_SHR) &&
					(opr!=CCXL_BINOP_SHRR))
				{
					BGBCC_CCXL_ConvImm(ctx, dty, tty, treg, &treg);
//					tty=BGBCC_CCXL_GetRegType(ctx, treg);
				}
			}

			if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg) &&
				BGBCC_CCXL_TypeRealP(ctx, dty2) &&
				BGBCC_CCXL_TypeSmallTypeP(ctx, dty2, tty))
			{
				BGBCC_CCXL_ConvImm(ctx, dty2, tty, treg, &treg);
				tty=dty2;
			}
		}
#endif

		is_shl=0;
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		{
			if(	(opr==CCXL_BINOP_SHL) ||
				(opr==CCXL_BINOP_SHR) ||
				(opr==CCXL_BINOP_SHRR))
					is_shl=1;
		}


//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) &&
			(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) || is_shl))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg);
			BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg, sreg, treg);
			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
//		if((dty.val!=sty.val) && (dty.val!=tty.val))
//		if(1)
		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) &&
			!(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) || is_shl))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &sreg2);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty2, sty, sreg2, sreg);
			if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
				{ BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg); }
			else
				{ BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg); }
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		}else
		{
//			if(dty.val!=sty.val)
//			if(1)
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty2, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}
			else
				{ sreg2=sreg; }

//			if(dty.val!=tty.val)
//			if(1)
			if(!(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) || is_shl))
			{
//				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
				if((opr!=CCXL_BINOP_SHL) &&
					(opr!=CCXL_BINOP_SHR) &&
					(opr!=CCXL_BINOP_SHRR))
				{
					BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
					BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
				}
				else
				{
					BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
					BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg);
				}
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}
			else
				{ treg2=treg; }
		}

		BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		return(CCXL_STATUS_YES);
	}

	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<"))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">"))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;
	if(!strcmp(op, "==="))opr=CCXL_CMP_EQQ;
	if(!strcmp(op, "!=="))opr=CCXL_CMP_NEQQ;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx, "CompareOp", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_CMPOP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &dty);

//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
		{
			BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
			BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);

#if 0
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
#endif

#if 1
//		if((dty.val!=sty.val) && (dty.val!=tty.val))
//		if(1)
		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			!BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
			if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
				{ BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg); }
			else
				{ BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg); }
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		}else
		{
//			if(dty.val!=sty.val)
//			if(1)
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}
			else
				{ sreg2=sreg; }

//			if(dty.val!=tty.val)
//			if(1)
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
				if((opr!=CCXL_BINOP_SHL) && (opr!=CCXL_BINOP_SHR))
					{ BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg); }
				else
					{ BGBCC_CCXL_EmitMov(ctx, tty, treg2, treg); }
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}
			else
				{ treg2=treg; }
		}
#endif

		BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_NO);
}


ccxl_status BGBCC_CCXL_StackBinaryOpStore(BGBCC_TransState *ctx,
	char *op, char *name)
{
	ccxl_register sreg, treg, dreg, sreg2, treg2, dreg2;
	ccxl_type sty, tty, dty, dty2, bty;
	double f, g;
	s64 li, lj, lk;
	int opr, is_shl;
	int i, j, k;

//	BGBCC_CCXL_DebugPrintStackLLn(ctx, "BinaryOpStore", __FILE__, __LINE__);

//	if(name && !strcmp(name, "dc_iscale"))
//		__debugbreak();

//	i=BGBCC_CCXL_PopRegister(ctx, &treg);
	j=BGBCC_CCXL_LookupAsRegisterStore(ctx, name, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}
	
	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_BINOP_ADD;
	if(!strcmp(op, "-"))opr=CCXL_BINOP_SUB;
	if(!strcmp(op, "*"))opr=CCXL_BINOP_MUL;
	if(!strcmp(op, "/"))opr=CCXL_BINOP_DIV;
	if(!strcmp(op, "%"))opr=CCXL_BINOP_MOD;
	if(!strcmp(op, "&"))opr=CCXL_BINOP_AND;
	if(!strcmp(op, "|"))opr=CCXL_BINOP_OR;
	if(!strcmp(op, "^"))opr=CCXL_BINOP_XOR;
	if(!strcmp(op, "<<"))opr=CCXL_BINOP_SHL;
	if(!strcmp(op, ">>"))opr=CCXL_BINOP_SHR;
	if(!strcmp(op, ">>>"))opr=CCXL_BINOP_SHRR;

	if(!strcmp(op, "CONS"))
		opr=CCXL_BINOP_CONS;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx,
			"BinaryOpStore", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STBINOP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr);
		BGBCC_CCXLR3_EmitArgSymbol(ctx, name);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);

		if(	BGBCC_CCXL_TypeVecP(ctx, sty) &&
			BGBCC_CCXL_TypeVecP(ctx, tty))
		{
			k=-1;
		}

#if 1
		if(BGBCC_CCXL_IsRegImmILFDP(ctx, sreg) &&
//			BGBCC_CCXL_TypeSmallFloat128P(ctx, sty) &&
//			BGBCC_CCXL_TypeSmallFloat128P(ctx, tty) &&
			((opr==CCXL_BINOP_ADD) ||
			 (opr==CCXL_BINOP_MUL) ||
			 (opr==CCXL_BINOP_AND) ||
			 (opr==CCXL_BINOP_OR) ||
			 (opr==CCXL_BINOP_XOR)))
		{
			dreg2=sreg;
			sreg=treg;
			treg=dreg2;
		}
#endif

		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		dty=BGBCC_CCXL_GetRegType(ctx, dreg);

		if(	BGBCC_CCXL_IsRegImmILFDP(ctx, sreg) &&
			BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
		{
			if(	BGBCC_CCXL_IsRegImmILP(ctx, sreg) &&
				BGBCC_CCXL_IsRegImmILP(ctx, treg))
			{
				li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
				lj=BGBCC_CCXL_GetRegImmLongValue(ctx, treg);
				lk=-999999;
				
				switch(opr)
				{
				case CCXL_BINOP_ADD:	lk=li+lj; break;
				case CCXL_BINOP_SUB:	lk=li-lj; break;
				case CCXL_BINOP_MUL:	lk=li*lj; break;
				case CCXL_BINOP_DIV:	lk=li/lj; break;
				case CCXL_BINOP_MOD:	lk=li%lj; break;
				case CCXL_BINOP_AND:	lk=li&lj; break;
				case CCXL_BINOP_OR:		lk=li|lj; break;
				case CCXL_BINOP_XOR:	lk=li^lj; break;

				case CCXL_BINOP_SHL:	lk=li<<((int)lj); break;
				case CCXL_BINOP_SHR:	lk=li>>((int)lj); break;
				}
				
				if(lk!=-999999)
				{
					BGBCC_CCXL_GetRegForLongValue(ctx, &treg2, lk);
					BGBCC_CCXL_ConvImm(ctx, dty, tty, treg2, &treg2);
					BGBCC_CCXL_EmitMov(ctx, dty, dreg, treg2);
					return(CCXL_STATUS_YES);
				}
			}

			/* Errm... */
		}
		else if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
		{
			if(BGBCC_CCXL_TypeRealP(ctx, sty))
			{
				if(BGBCC_CCXL_TypeRealP(ctx, dty) ||
					BGBCC_CCXL_TypeBcdP(ctx, dty))
				{
					BGBCC_CCXL_ConvImm(ctx, dty, tty, treg, &treg);
					tty=dty;
				}else
				{
					BGBCC_CCXL_ConvImm(ctx, sty, tty, treg, &treg);
					tty=sty;
				}
			}
		}else
			if(BGBCC_CCXL_IsRegImmILFDP(ctx, sreg))
		{
			if(BGBCC_CCXL_TypeRealP(ctx, tty))
			{
				if(BGBCC_CCXL_TypeRealP(ctx, dty) ||
					BGBCC_CCXL_TypeBcdP(ctx, dty))
				{
					BGBCC_CCXL_ConvImm(ctx, dty, sty, sreg, &sreg);
					sty=dty;
				}else
				{
					BGBCC_CCXL_ConvImm(ctx, tty, sty, sreg, &sreg);
					sty=tty;
				}
			}
		}

		dty2=dty;
//		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &dty2);
		BGBCC_CCXL_GetTypeBinaryDestB(ctx, opr, sty, tty, &dty2, 2);
//		BGBCC_CCXL_TypeAutoPromoteType(ctx, dty2, &dty2);

		if(	BGBCC_CCXL_TypeRealP(ctx, dty) &&
			BGBCC_CCXL_TypeRealP(ctx, dty2))
		{
			dty2=dty;
		}

#if 1
		if(	BGBCC_CCXL_TypeSmallLongP(ctx, dty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, dty2) &&
			(opr!=CCXL_BINOP_MUL) &&
			(opr!=CCXL_BINOP_DIV) &&
			(opr!=CCXL_BINOP_MOD) &&
			(opr!=CCXL_BINOP_SHL) &&
			(opr!=CCXL_BINOP_SHR) &&
			(opr!=CCXL_BINOP_SHRR) &&
			!BGBCC_CCXL_TypeSmallShortP(ctx, dty))
		{
			if(	BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
				BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty))
			{
				dty2=dty;
			}else
				if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) ||
					!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty))
			{
//				dty2=dty;
			}
		}
#endif
		
		if(	BGBCC_CCXL_TypeVecP(ctx, sty) &&
			BGBCC_CCXL_TypeVecP(ctx, tty) &&
			!BGBCC_CCXL_TypeVecP(ctx, dty2)	)
		{
			BGBCC_CCXL_GetTypeBinaryDest(ctx, CCXL_BINOP_ADD, sty, tty, &dty2);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty) &&
			(opr==CCXL_BINOP_SUB))
		{
//			pty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, pty, &dreg);

			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);

			if(	!BGBCC_CCXL_TypeSgNLongP(ctx, dty) &&
				!BGBCC_CCXL_TypeSgLongP(ctx, dty))
			{
				dty2=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_NL);
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg2);
				BGBCC_CCXL_EmitDiffPtr(ctx, bty, dreg2, sreg, treg);
				BGBCC_CCXL_EmitConv(ctx, dty, dty2, dreg, dreg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}else
			{
				BGBCC_CCXL_EmitDiffPtr(ctx, bty, dreg, sreg, treg);
			}

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);

//			BGBCC_CCXL_PushRegister(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
//			BGBCC_CCXL_TypeIntP(ctx, tty))
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, tty))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				((opr==CCXL_BINOP_ADD) ||
				 (opr==CCXL_BINOP_SUB)))
			{
				k=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if(opr==CCXL_BINOP_SUB)
					k=-k;
				
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
			
			if(opr==CCXL_BINOP_ADD)
			{
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
			
			BGBCC_DBGBREAK
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		{
			if(opr==CCXL_BINOP_ADD)
			{
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_SUB)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_CCXL_EmitUnaryOp(ctx, tty,
					CCXL_UNOP_NEG, treg2, treg);
				BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
				BGBCC_CCXL_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
		}

		if(opr==CCXL_BINOP_DIV)
		{
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
				opr=CCXL_BINOP_MUL;
			}

			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
				opr=CCXL_BINOP_MUL;
			}

			if(BGBCC_CCXL_IsRegImmIntP(ctx, treg) &&
				BGBCC_CCXL_TypeSmallLongP(ctx, dty2) &&
				BGBCC_CCXL_TypeUnsignedP(ctx, dty2))
			{
				j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
				if((j>1) && !(j&(j-1)))
				{
					k=0;
					while(j>1)	{ j>>=1; k++; }
					BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
					opr=CCXL_BINOP_SHR;
				}
			}
		}

#if 1
		if(opr==CCXL_BINOP_DIV)
		{
			if(	BGBCC_CCXL_TypeFloatP(ctx, dty2) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}

			if(	BGBCC_CCXL_TypeDoubleP(ctx, dty2) &&
				BGBCC_CCXL_IsRegImmILFDP(ctx, treg))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, treg);
				if((f!=0) && (f==f))
				{
					BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0/f);
					opr=CCXL_BINOP_MUL;
				}
			}
		}
#endif

		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) &&
			BGBCC_CCXL_TypeSgIntP(ctx, dty2) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, sty) )
		{
			if(		(opr==CCXL_BINOP_ADD) ||
					(opr==CCXL_BINOP_SUB) ||
					(opr==CCXL_BINOP_MUL) )
			{
				BGBCC_CCXL_GetRegAsType(ctx, sreg, dty2, &sreg);
				sty=BGBCC_CCXL_GetRegType(ctx, sreg);
			}
		}

		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) &&
			BGBCC_CCXL_TypeSgIntP(ctx, dty2) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, tty) )
		{
			if(		(opr==CCXL_BINOP_ADD) ||
					(opr==CCXL_BINOP_SUB) ||
					(opr==CCXL_BINOP_MUL) )
			{
				BGBCC_CCXL_GetRegAsType(ctx, treg, dty2, &treg);
				tty=BGBCC_CCXL_GetRegType(ctx, treg);
			}
		}


#if 1
//		if(	!BGBCC_CCXL_TypeEqualP(ctx, dty2, sty) ||
//			!BGBCC_CCXL_TypeEqualP(ctx, dty2, tty))
		if(	!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) ||
			!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty))
		{
			if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg) &&
				!BGBCC_CCXL_IsRegImmUnsignedP(ctx, treg) &&
				BGBCC_CCXL_TypeSmallFloat128P(ctx, dty2) &&
				BGBCC_CCXL_TypeSmallTypeP(ctx, sty, tty))
			{
				dty2=sty; tty=sty;

				if(	(opr!=CCXL_BINOP_SHL) &&
					(opr!=CCXL_BINOP_SHR) &&
					(opr!=CCXL_BINOP_SHRR))
				{
					BGBCC_CCXL_ConvImm(ctx, dty2, tty, treg, &treg);
				}
			}

			if(BGBCC_CCXL_IsRegImmILFDP(ctx, treg) &&
				BGBCC_CCXL_TypeRealP(ctx, dty2) &&
				BGBCC_CCXL_TypeSmallTypeP(ctx, dty2, tty))
			{
				BGBCC_CCXL_ConvImm(ctx, dty2, tty, treg, &treg);
				tty=dty2;
			}
		}
#endif

		is_shl=0;
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		{
			if(	(opr==CCXL_BINOP_SHL) ||
				(opr==CCXL_BINOP_SHR) ||
				(opr==CCXL_BINOP_SHRR))
			{
				is_shl=1;
			}
		}

		if(opr==CCXL_BINOP_SHR)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, sty))
				opr=CCXL_BINOP_SHRR;
		}

		if(!BGBCC_CCXL_TypeSmallTypeP(ctx, dty, dty2) ||
			( BGBCC_CCXL_TypeRealP(ctx, dty) &&
			 !BGBCC_CCXL_TypeRealP(ctx, dty2)) ||
			(!BGBCC_CCXL_TypeRealP(ctx, dty) &&
			  BGBCC_CCXL_TypeRealP(ctx, dty2)))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg2);

			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty2, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}else
			{
				sreg2=sreg;
			}

//			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
//			BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
//			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			
			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) && !is_shl)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
				BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}else
			{
				treg2=treg;
			}

			BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg2, sreg2, treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, dty2, dreg, dreg2);

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

#if 0
//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2) &&
			!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
//			BGBCC_CCXL_EmitBinaryOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg, sreg, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
#endif

#if 1
		if(	BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty))
		{
			if(BGBCC_CCXL_TypeVecP(ctx, dty2) &&
				!BGBCC_CCXL_TypeVecP(ctx, dty))
			{
				BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg, sreg, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}
		
			if(	BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2) &&
				!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
			{
				BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg, sreg, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				return(CCXL_STATUS_YES);
			}else
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg2);
				BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg2, sreg, treg);
				if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2))
					BGBCC_CCXL_EmitMov(ctx, dty, dreg, dreg2);
				else
					BGBCC_CCXL_EmitConv(ctx, dty, dty2, dreg, dreg2);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
				return(CCXL_STATUS_YES);
			}
		}
#endif
		
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2)	||
			(	 BGBCC_CCXL_TypeVecP(ctx, dty2) &&
				!BGBCC_CCXL_TypeVecP(ctx, dty )	)	)
		{
	//		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
	//		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
	//		BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
	//		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

//			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
//			BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
//			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);

			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}else
			{
				sreg2=sreg;
			}

			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) && !is_shl)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
				BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}else
			{
				treg2=treg;
			}

			BGBCC_CCXL_EmitBinaryOp(ctx, dty, opr, dreg, sreg2, treg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}else
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg2);

			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &sreg2);
				BGBCC_CCXL_EmitConv(ctx, dty2, sty, sreg2, sreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			}else
			{
				sreg2=sreg;
			}

			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty) && !is_shl)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
				BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			}else
			{
				treg2=treg;
			}

			BGBCC_CCXL_EmitBinaryOp(ctx, dty2, opr, dreg2, sreg2, treg2);
			BGBCC_CCXL_EmitConv(ctx, dty, dty2, dreg, dreg2);

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
	}

	if(!strcmp(op, "=="))opr=CCXL_CMP_EQ;
	if(!strcmp(op, "!="))opr=CCXL_CMP_NE;
	if(!strcmp(op, "<" ))opr=CCXL_CMP_LT;
	if(!strcmp(op, ">" ))opr=CCXL_CMP_GT;
	if(!strcmp(op, "<="))opr=CCXL_CMP_LE;
	if(!strcmp(op, ">="))opr=CCXL_CMP_GE;
	if(!strcmp(op, "==="))opr=CCXL_CMP_EQQ;
	if(!strcmp(op, "!=="))opr=CCXL_CMP_NEQQ;

	if(opr>=0)
	{
//		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
		if(!BGBCC_CCXL_IsRegSmallIntP(ctx, dreg))
		{
			BGBCC_CCXL_StackBinaryOp(ctx, op);
			BGBCC_CCXL_PopStore(ctx, name);
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_DebugPrintStackLLn(ctx,
			"CompareOpStore", __FILE__, __LINE__);

		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STCMPOP);
		BGBCC_CCXLR3_EmitArgInt(ctx, opr);
		BGBCC_CCXLR3_EmitArgSymbol(ctx, name);

		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
//		dty=BGBCC_CCXL_GetRegType(ctx, dreg);
//		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &dty);

//		if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty) &&
//			BGBCC_CCXL_TypeEqualP(ctx, dty, tty))
		if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty) &&
			BGBCC_CCXL_TypeCompatibleP(ctx, dty, tty) &&
			!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
//			BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
			BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg, treg);
//			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}

//		BGBCC_CCXL_RegisterAllocTemporaryInt(ctx, &dreg);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_CCXL_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_CCXL_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);

		BGBCC_CCXL_EmitCompareOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_NO);
}
