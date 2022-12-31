char *BGBCC_CCXL_StackGetSigUnary(BGBCC_TransState *ctx,
	ccxl_type lty)
{
	char tb[256];
	char *s0, *ct;

	ct=tb;
	*ct++='(';

	s0=BGBCC_CCXL_TypeGetSig(ctx, lty);
	strcpy(ct, s0);
	ct+=strlen(ct);

	*ct++=')';
	*ct++=0;

	return(bgbcc_strdup(tb));
}

ccxl_status BGBCC_CCXL_StackUnaryOp(BGBCC_TransState *ctx, char *op)
{
	ccxl_register sreg, dreg;
	ccxl_type sty, dty, bty;
	int opr;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOp", __FILE__, __LINE__);

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_UNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	BGBCC_CCXL_TypeAutoPromoteType(ctx, sty, &sty);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
			
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
				__debugbreak();
			
			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
	}

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		__debugbreak();

	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
	BGBCC_CCXL_PushRegister(ctx, dreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackUnaryOpName(BGBCC_TransState *ctx,
	char *op, char *name)
{
	ccxl_register sreg;
	ccxl_type sty, dty, bty;
	int opr;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOpName", __FILE__, __LINE__);

	j=BGBCC_CCXL_LookupAsRegister(ctx, name, &sreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDUNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);
	BGBCC_CCXLR3_EmitArgSymbol(ctx, name);

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	if(BGBCC_CCXL_TypePointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_CCXL_PushRegister(ctx, dreg);
			BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			return(CCXL_STATUS_YES);
		}
	}

//	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
//		__debugbreak();

	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	return(CCXL_STATUS_YES);

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
//	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
//	BGBCC_CCXL_PushRegister(ctx, dreg);
//	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
//	return(CCXL_STATUS_YES);
}


ccxl_status BGBCC_CCXL_StackUnaryOpNameB(BGBCC_TransState *ctx,
	char *op, char *name, int mod)
{
	ccxl_register dreg, sreg;
	ccxl_type sty, dty, bty;
	int opr;
	int i, j, k;

	if(mod==0)
	{
		return(BGBCC_CCXL_StackUnaryOpName(ctx, op, name));
	}

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOpNameB", __FILE__, __LINE__);

	j=BGBCC_CCXL_LookupAsRegister(ctx, name, &sreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LDUNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr+(mod*16));
	BGBCC_CCXLR3_EmitArgSymbol(ctx, name);

	sty=BGBCC_CCXL_GetRegType(ctx, sreg);

	if(BGBCC_CCXL_TypePointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_CCXL_PushRegister(ctx, dreg);

			if(mod==0)
			{
				BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				return(CCXL_STATUS_YES);
			}

			if(mod==1)
			{
				BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
//				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_PushRegister(ctx, sreg);
				return(CCXL_STATUS_YES);
			}

			if(mod==2)
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
				BGBCC_CCXL_EmitMov(ctx, sty, dreg, sreg);
				BGBCC_CCXL_EmitLeaImm(ctx, bty, sreg, sreg, k);
				BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
				BGBCC_CCXL_PushRegister(ctx, dreg);
				return(CCXL_STATUS_YES);
			}

			BGBCC_DBGBREAK
			return(CCXL_STATUS_ERR_GENERIC);
		}

//		BGBCC_DBGBREAK
	}

	if(mod==0)
	{
		BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		return(CCXL_STATUS_YES);
	}

	if(mod==1)
	{
		BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
//		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_PushRegister(ctx, sreg);
		return(CCXL_STATUS_YES);
	}

	if(mod==2)
	{
		BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
		BGBCC_CCXL_EmitMov(ctx, sty, dreg, sreg);
		BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_PushRegister(ctx, dreg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_DBGBREAK
	return(CCXL_STATUS_ERR_GENERIC);

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);
//	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
//	BGBCC_CCXL_PushRegister(ctx, dreg);
//	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
//	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackUnaryOpStore(BGBCC_TransState *ctx,
	char *op, char *dname)
{
	ccxl_register sreg, dreg, dreg2;
	ccxl_type sty, dty, bty;
	int opr, cnv;
	int i, j, k;

	BGBCC_CCXL_DebugPrintStackLLn(ctx, "UnaryOpStore", __FILE__, __LINE__);

	opr=-1;
	if(!strcmp(op, "+"))opr=CCXL_UNOP_MOV;
	if(!strcmp(op, "-"))opr=CCXL_UNOP_NEG;
	if(!strcmp(op, "~"))opr=CCXL_UNOP_NOT;
	if(!strcmp(op, "!"))opr=CCXL_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=CCXL_UNOP_INC;
	if(!strcmp(op, "--"))opr=CCXL_UNOP_DEC;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STUNOP);
	BGBCC_CCXLR3_EmitArgInt(ctx, opr);
	BGBCC_CCXLR3_EmitArgSymbol(ctx, dname);

	j=BGBCC_CCXL_LookupAsRegisterStore(ctx, dname, &dreg);
	if(j<=0)
	{
		if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(CCXL_STATUS_ERR_GENERIC);
	}

	i=BGBCC_CCXL_PopRegister(ctx, &sreg);
	sty=BGBCC_CCXL_GetRegType(ctx, sreg);
	dty=BGBCC_CCXL_GetRegType(ctx, dreg);

//	if(!BGBCC_CCXL_TypeEqualP(ctx, dty, sty))
	if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, sty))
	{
		dreg2=dreg; cnv=1;
		BGBCC_CCXL_TypeAutoPromoteType(ctx, sty, &sty);
		BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
	}else
	{
		dreg2=dreg; cnv=0;
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if((opr==CCXL_UNOP_INC) ||
			(opr==CCXL_UNOP_DEC))
		{
			BGBCC_CCXL_TypeDerefType(ctx, sty, &bty);
			k=(opr==CCXL_UNOP_INC)?(1):(-1);
//			BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
			
			if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
				__debugbreak();
			
			BGBCC_CCXL_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_CCXL_PushRegister(ctx, dreg);

			if(cnv)
			{
				BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg2, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
		
		if(opr==CCXL_UNOP_LNOT)
		{
			BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);

			if(cnv)
			{
				BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg2, dreg);
				BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
			}

			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
			return(CCXL_STATUS_YES);
		}
		
		BGBCC_DBGBREAK
	}

//	BGBCC_CCXL_TypeFromSig(ctx, &dty, sig);
//	BGBCC_CCXL_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg, sreg);

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{ BGBCC_DBGBREAK }

	BGBCC_CCXL_EmitUnaryOp(ctx, sty, opr, dreg, sreg);

	if(cnv)
	{
		BGBCC_CCXL_EmitConv(ctx, dty, sty, dreg2, dreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
	}

	BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
	BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_StackTrinaryOp(BGBCC_TransState *ctx, char *op)
{
	return(BGBCC_CCXL_StackTrinaryOpStore(ctx, op, NULL));
}

ccxl_status BGBCC_CCXL_StackTrinaryOpStore(BGBCC_TransState *ctx,
	char *op, char *name)
{
	ccxl_register sreg, treg, ureg, dreg, sreg2, treg2, ureg2, dreg2;
	ccxl_type sty, tty, uty, dty, dty2, bty;
	double f, g;
	int opr, is_shl;
	int i, j, k;

	if(name)
	{
	//	i=BGBCC_CCXL_PopRegister(ctx, &treg);
		j=BGBCC_CCXL_LookupAsRegisterStore(ctx, name, &dreg);
		if(j<=0)
		{
			if(!j)return(CCXL_STATUS_ERR_LOOKUPFAIL);
	//		if(i<0)return(i);
			if(j<0)return(j);
			return(CCXL_STATUS_ERR_GENERIC);
		}
	}
	
	opr=-1;
	if(!strcmp(op, "MAC"))
		opr=CCXL_BINOP_MAC;

	if(opr>=0)
	{
		BGBCC_CCXL_DebugPrintStackLLn(ctx,
			"TrinaryOpStore", __FILE__, __LINE__);

		if(name)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_STTRINOP);
			BGBCC_CCXLR3_EmitArgInt(ctx, opr);
			BGBCC_CCXLR3_EmitArgSymbol(ctx, name);
		}else
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_TRINOP);
			BGBCC_CCXLR3_EmitArgInt(ctx, opr);
		}

		k=BGBCC_CCXL_PopRegister(ctx, &ureg);
		j=BGBCC_CCXL_PopRegister(ctx, &treg);
		i=BGBCC_CCXL_PopRegister(ctx, &sreg);
		sty=BGBCC_CCXL_GetRegType(ctx, sreg);
		tty=BGBCC_CCXL_GetRegType(ctx, treg);
		uty=BGBCC_CCXL_GetRegType(ctx, ureg);

		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, sty, tty, &bty);
		BGBCC_CCXL_GetTypeBinaryDest(ctx, opr, uty, bty, &dty2);

		if(name)
		{
			dty=BGBCC_CCXL_GetRegType(ctx, dreg);

			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2))
//			if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty, dty2) ||
//				BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, ureg))
			{
				BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &dreg2);
			}else
			{
				dreg2=dreg;
			}

		}else
		{
			dty=dty2;
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty, &dreg);
			dreg2=dreg;
		}

		sreg2=sreg;
		treg2=treg;
		ureg2=ureg;

		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &sreg2);
			BGBCC_CCXL_EmitConv(ctx, dty2, sty, sreg2, sreg);
		}

		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, tty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &treg2);
			BGBCC_CCXL_EmitConv(ctx, dty2, tty, treg2, treg);
		}

		if(!BGBCC_CCXL_TypeCompatibleP(ctx, dty2, uty))
		{
			BGBCC_CCXL_RegisterAllocTemporary(ctx, dty2, &ureg2);
			BGBCC_CCXL_EmitConv(ctx, dty2, uty, ureg2, ureg);
		}

		BGBCC_CCXL_EmitTrinaryOp(ctx, dty2, opr, dreg2, sreg2, treg2, ureg2);

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, dreg2))
		{
			BGBCC_CCXL_EmitConv(ctx, dty, dty2, dreg, dreg2);
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg2);
		}
		
		if(!name)
		{
			BGBCC_CCXL_PushRegister(ctx, dreg);
		}else
		{
			BGBCC_CCXL_RegisterCheckRelease(ctx, dreg);
		}

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, sreg, sreg2))
			BGBCC_CCXL_RegisterCheckRelease(ctx, sreg2);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, treg, treg2))
			BGBCC_CCXL_RegisterCheckRelease(ctx, treg2);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, ureg, ureg2))
			BGBCC_CCXL_RegisterCheckRelease(ctx, ureg2);
		
		BGBCC_CCXL_RegisterCheckRelease(ctx, sreg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, treg);
		BGBCC_CCXL_RegisterCheckRelease(ctx, ureg);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);
}
