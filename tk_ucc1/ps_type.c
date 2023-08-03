TKUCC_AstNode *TKUCC_TryParseType(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
	char *tyn;
	char *t0, *t1;
	u64 tyfl, fl;
	int ix, kwi, ix0;

	ix=*rvi;
	ix0=ix;
//	t0=TKUCC_TokStrmGetToken(ctx, ix+0);
//	t1=TKUCC_TokStrmGetToken(ctx, ix+1);

	tyn=NULL;
	tyfl=0;
	while(1)
	{
		t0=TKUCC_TokStrmGetToken(ctx, ix);
		kwi=TKUCC_TokIndexKeyword(t0, 0);
		if(kwi<=0)
			break;
		switch(kwi)
		{
			case TKUCC_KW_AUTO:			fl=TKUCC_TYFL_AUTO;			break;
			case TKUCC_KW_CHAR:			fl=TKUCC_TYFL_CHAR;			break;
			case TKUCC_KW_CONST:		fl=TKUCC_TYFL_CONST;		break;
			case TKUCC_KW_DOUBLE:		fl=TKUCC_TYFL_DOUBLE;		break;
			case TKUCC_KW_EXTERN:		fl=TKUCC_TYFL_EXTERN;		break;
			case TKUCC_KW_INT:			fl=TKUCC_TYFL_INT;			break;
			case TKUCC_KW_LONG:
				if(tyfl&TKUCC_TYFL_LONG)
				{
					fl=TKUCC_TYFL_VOID;
					break;
				}
				fl=TKUCC_TYFL_LONG;
				break;
			case TKUCC_KW_REGISTER:		fl=TKUCC_TYFL_REGISTER;		break;
			case TKUCC_KW_SHORT:		fl=TKUCC_TYFL_SHORT;		break;
			case TKUCC_KW_SIGNED:		fl=TKUCC_TYFL_SIGNED;		break;
			case TKUCC_KW_STATIC:		fl=TKUCC_TYFL_STATIC;		break;
			case TKUCC_KW_TYPEDEF:		fl=TKUCC_TYFL_TYPEDEF;		break;
			case TKUCC_KW_UNSIGNED:		fl=TKUCC_TYFL_UNSIGNED;		break;
			case TKUCC_KW_VOID:			fl=TKUCC_TYFL_VOID;			break;
			case TKUCC_KW_VOLATILE:		fl=TKUCC_TYFL_VOLATILE;		break;
			default:					fl=0;						break;
		}
		if(!fl)
			break;
		tyfl|=fl;
		ix++;
	}
	
	if(!tyfl && !tyn)
	{
		return(NULL);
	}

	if(!tyn)
	{
		switch(tyfl&TKUCC_TYFL_TYMASK)
		{
		case TKUCC_TYFL_TY_CHAR:
		case TKUCC_TYFL_TY_SCHAR:
			tyn="__sint_8";
			break;
		case TKUCC_TYFL_TY_UCHAR:
			tyn="__uint_8";
			break;

		case TKUCC_TYFL_TY_SHORT:
		case TKUCC_TYFL_TY_SHORTI:
		case TKUCC_TYFL_TY_SSHORT:
		case TKUCC_TYFL_TY_SSHORTI:
			tyn="__sint_16";
			break;
		case TKUCC_TYFL_TY_USHORT:
		case TKUCC_TYFL_TY_USHORTI:
			tyn="__uint_16";
			break;

		case TKUCC_TYFL_TY_INT:
		case TKUCC_TYFL_TY_SINT:
			tyn="__sint_32";
			break;
		case TKUCC_TYFL_TY_UINT:
			tyn="__uint_32";
			break;

		case TKUCC_TYFL_TY_LONG:
		case TKUCC_TYFL_TY_LONGI:
		case TKUCC_TYFL_TY_SLONG:
		case TKUCC_TYFL_TY_SLONGI:
			tyn="__sint_nl";
			break;
		case TKUCC_TYFL_TY_ULONG:
		case TKUCC_TYFL_TY_ULONGI:
			tyn="__uint_nl";
			break;

		case TKUCC_TYFL_TY_LLONG:
		case TKUCC_TYFL_TY_LLONGI:
		case TKUCC_TYFL_TY_SLLONG:
		case TKUCC_TYFL_TY_SLLONGI:
			tyn="__sint_64";
			break;
		case TKUCC_TYFL_TY_ULLONG:
		case TKUCC_TYFL_TY_ULLONGI:
			tyn="__uint_64";
			break;

		case TKUCC_TYFL_TY_FLOAT:
			tyn="__float_32";
			break;
		case TKUCC_TYFL_TY_DOUBLE:
			tyn="__float_64";
			break;
		case TKUCC_TYFL_TY_SFLOAT:
			tyn="__float_16";
			break;
		case TKUCC_TYFL_TY_LDOUBLE:
			tyn="__float_128";
			break;

		case TKUCC_TYFL_TY_VOID:
			tyn="__void";
			break;
		case TKUCC_TYFL_TY_AUTO:
			tyn="__auto";
			break;
		default:
			break;
		}
	}

	v0=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_TYPE, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrInt(ctx, v0, TKUCC_ATAG_TYFL, tyfl);
	TKUCC_AstSetAttrStr(ctx, v0, TKUCC_ATAG_TYN, tyn);
	return(v0);
}

TKUCC_AstNode *TKUCC_TryParseDecl(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n_ty, int *rvi)
{
	TKUCC_AstNode *n_ty1, *n_def, *n_val;
	char *t0, *t1, *nam;
	int ix, ind, i0, i1;
	
	ix=*rvi;
	ind=0;

	while(1)
	{
		t0=TKUCC_TokStrmGetToken(ctx, ix);
//		kwi=TKUCC_TokIndexKeyword(t0, 0);
//		if(kwi<=0)
//			break;

		if(TKUCC_TokenCheckOperatorP(t0, "*"))
		{
			ind++;
			ix++;
			continue;
		}
		
		break;
	}
	
	if(TKUCC_TokenOperatorP(t0))
	{
		if(	!strcmp(t0, ",") &&
			!strcmp(t0, ")") &&
			!strcmp(t0, ";") )
		{
			i0=TKUCC_AstGetAttrInt(ctx, n_ty, TKUCC_ATAG_IND);
			if(i0<=0)
				i0=0;

			n_ty1=TKUCC_AstCloneNode(ctx, n_ty);
			TKUCC_AstSetAttrInt(ctx, n_ty1, TKUCC_ATAG_IND, i0+ind);

			n_def=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_VAR, TKUCC_ZID_PARSE);
//			TKUCC_AstSetAttrStr(ctx, n_def, TKUCC_ATAG_NAME, nam);

			TKUCC_AstSetAttrNode(ctx, n_def, TKUCC_ATAG_TN, n_ty1);
//			if(n_val)
//				TKUCC_AstSetAttrNode(ctx, n_def, TKUCC_ATAG_VN, n_val);

			*rvi=ix;
			return(n_def);
		}
	}

	if(TKUCC_TokenNameP(t0))
	{
		nam=TKUCC_StrdupInternParse(ctx, t0);
	
		ix++;
		t1=TKUCC_TokStrmGetToken(ctx, ix);
		n_val=NULL;

		if(TKUCC_TokenCheckOperatorP(t1, "="))
		{
			ix++;
			n_val=TKUCC_ParseExpr(ctx, &ix);
			t1=TKUCC_TokStrmGetToken(ctx, ix);
		}

		i0=TKUCC_AstGetAttrInt(ctx, n_ty, TKUCC_ATAG_IND);
		if(i0<=0)
			i0=0;

		n_ty1=TKUCC_AstCloneNode(ctx, n_ty);
		TKUCC_AstSetAttrInt(ctx, n_ty1, TKUCC_ATAG_IND, i0+ind);

		n_def=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_VAR, TKUCC_ZID_PARSE);
//		TKUCC_AstSetAttrInt(ctx, v0, TKUCC_ATAG_TYFL, tyfl);

		TKUCC_AstSetAttrStr(ctx, n_def, TKUCC_ATAG_NAME, nam);

		TKUCC_AstSetAttrNode(ctx, n_def, TKUCC_ATAG_TN, n_ty1);
		if(n_val)
			TKUCC_AstSetAttrNode(ctx, n_def, TKUCC_ATAG_VN, n_val);

		*rvi=ix;
		return(n_def);
	}

	return(NULL);
}

TKUCC_AstNode *TKUCC_TryParseDeclList(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n_ty, int *rvi)
{
	TKUCC_AstNode *n_var, *n_vars, *n_def;
	char *t0, *t1;
	int ix;
	
	if(!n_ty)
		return(NULL);

	ix=*rvi;
	n_var=TKUCC_TryParseDecl(ctx, n_ty, &ix);
	if(!n_var)
		return(NULL);

	t0=TKUCC_TokStrmGetToken(ctx, ix);

	if(TKUCC_TokenCheckOperatorP(t0, ","))
	{
		n_vars=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_VARS, TKUCC_ZID_PARSE);
		TKUCC_AstAddNodeEnd(ctx, n_vars, n_var);
		ix++;
		
		while(1)
		{
			n_var=TKUCC_TryParseDecl(ctx, n_ty, &ix);
			if(n_var)
				TKUCC_AstAddNodeEnd(ctx, n_vars, n_var);

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(TKUCC_TokenCheckOperatorP(t0, ","))
			{
				ix++;
				continue;
			}
			break;
		}

		t0=TKUCC_TokStrmGetToken(ctx, ix);
		if(TKUCC_TokenCheckOperatorP(t0, ";"))
			ix++;
		
		*rvi=ix;
		return(n_vars);
	}

	if(TKUCC_TokenCheckOperatorP(t0, ";"))
		ix++;
	*rvi=ix;
	return(n_var);
}

TKUCC_AstNode *TKUCC_TryParseDefinition(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *n_ty, *n_def;
	int ix;

	ix=*rvi;
	n_ty=TKUCC_TryParseType(ctx, &ix);
	if(!n_ty)
		return(NULL);
	n_def=TKUCC_TryParseDeclList(ctx, n_ty, &ix);
	if(!n_def)
		return(NULL);
	*rvi=ix;
	return(n_def);
}

TKUCC_AstNode *TKUCC_TryParseCastDecl(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *n_ty, *n_def;
	int ix;

	ix=*rvi;
	n_ty=TKUCC_TryParseType(ctx, &ix);
	if(!n_ty)
		return(NULL);
	n_def=TKUCC_TryParseDecl(ctx, n_ty, &ix);
	if(!n_def)
		return(NULL);
	*rvi=ix;
	return(n_def);
}
