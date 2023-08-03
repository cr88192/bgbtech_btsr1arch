TKUCC_AstNode *TKUCC_ParseExpr(TKUCC_MainContext *ctx, int *rvi);
TKUCC_AstNode *TKUCC_TryParseCastDecl(TKUCC_MainContext *ctx, int *rvi);
TKUCC_AstNode *TKUCC_ParseBlockStmt(TKUCC_MainContext *ctx, int *rvi);

TKUCC_AstNode *TKUCC_ParseWrapFuncall(TKUCC_MainContext *ctx,
	char *fn, TKUCC_AstNode *args)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_CALL, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_NAME, fn);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_ARGS, args);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapBinary(TKUCC_MainContext *ctx,
	char *op, TKUCC_AstNode *ln, TKUCC_AstNode *rn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_BINARY, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_OP, op);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_LN, ln);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_RN, rn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapCompare(TKUCC_MainContext *ctx,
	char *op, TKUCC_AstNode *ln, TKUCC_AstNode *rn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_COMPARE, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_OP, op);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_LN, ln);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_RN, rn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapUnary(TKUCC_MainContext *ctx,
	char *op, TKUCC_AstNode *vn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_UNARY, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_OP, op);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_VN, vn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapPostfix(TKUCC_MainContext *ctx,
	char *op, TKUCC_AstNode *vn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_POSTFIX, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_OP, op);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_VN, vn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapTern(TKUCC_MainContext *ctx,
	TKUCC_AstNode *cn, TKUCC_AstNode *ln, TKUCC_AstNode *rn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_COMPARE, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_CN, cn);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_LN, ln);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_RN, rn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapAssign(TKUCC_MainContext *ctx,
	char *op, TKUCC_AstNode *ln, TKUCC_AstNode *rn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_ASSIGN, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_OP, op);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_LN, ln);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_RN, rn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapRef(TKUCC_MainContext *ctx,
	char *name)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_REF, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_VALUE, name);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapString(TKUCC_MainContext *ctx,
	char *name)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_STRING, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrStr(ctx, tmp, TKUCC_ATAG_VALUE, name);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapInt(TKUCC_MainContext *ctx, s64 val)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_INT, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrInt(ctx, tmp, TKUCC_ATAG_VALUE, val);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapReal(TKUCC_MainContext *ctx, double val)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_REAL, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrReal(ctx, tmp, TKUCC_ATAG_VALUE, val);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseWrapCast(TKUCC_MainContext *ctx,
	TKUCC_AstNode *cn, TKUCC_AstNode *vn)
{
	TKUCC_AstNode *tmp;
	tmp=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_CAST, TKUCC_ZID_PARSE);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_CN, cn);
	TKUCC_AstSetAttrNode(ctx, tmp, TKUCC_ATAG_VN, vn);
	return(tmp);
}

TKUCC_AstNode *TKUCC_ParseFunArgs(TKUCC_MainContext *ctx, int *rvi)
{
//	TKUCC_AstNode *va[16];
	TKUCC_AstNode *v0, *v1, *n;
	char *s0, *s1;
	int na, ix;

	ix=*rvi;

	n=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_LIST, TKUCC_ZID_PARSE);

	na=0;
	s0=TKUCC_TokStrmGetToken(ctx, ix);
	while(s0)
	{
		if(!strcmp(s0, ")"))
		{
			ix++;
			break;
		}
		
		v0=TKUCC_ParseExpr(ctx, &ix);
//		va[na++]=v0;

		TKUCC_AstAddNodeEnd(ctx, n, v0);

		s0=TKUCC_TokStrmGetToken(ctx, ix);
		if(!strcmp(s0, ","))
		{
			ix++;
			s0=TKUCC_TokStrmGetToken(ctx, ix);
		}
	}
	
	*rvi=ix;
	return(n);
}


TKUCC_AstNode *TKUCC_ParseExprI(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
	char *t0, *t1;
	long li;
	double f;
	int ix;

	ix=*rvi;
	t0=TKUCC_TokStrmGetToken(ctx, ix+0);
	t1=TKUCC_TokStrmGetToken(ctx, ix+1);

	if(TKUCC_TokenNameP(t0))
	{
		if(t1 && !strcmp(t1, "("))
		{
			ix+=2;
			v1=TKUCC_ParseFunArgs(ctx, &ix);
			v0=TKUCC_ParseWrapFuncall(ctx, t0, v1);
			*rvi=ix+1;
			return(v1);
		}
		
		*rvi=ix+1;
		return(TKUCC_ParseWrapRef(ctx, t0));
	}

	if(TKUCC_TokenNumberP(t0))
	{
		*rvi=ix+1;
		
		if(t0[0]=='0')
		{
			if((t0[1]=='x') || (t0[1]=='X'))
			{
				li=tkucc_atoll(t0);
				return(TKUCC_ParseWrapInt(ctx, li));
			}
		}
		
		f=tkucc_atof(t0);
		li=f;
		if(li==f)
			return(TKUCC_ParseWrapInt(ctx, li));
		return(TKUCC_ParseWrapReal(ctx, f));
	}

	if(TKUCC_TokenStringP(t0))
	{
		v1=TKUCC_ParseWrapString(ctx, t0);
		*rvi=ix+1;
		return(v1);
	}

	if(!strcmp(t0, "("))
	{
		ix++;
		v0=TKUCC_ParseExpr(ctx, &ix);

		t1=TKUCC_TokStrmGetToken(ctx, ix);
		if(!strcmp(t1, ")"))
		{
			*rvi=ix+1;
			return(v0);
		}
	}

//	tk_printf("?%s?\n", a[0]);
//	__debugbreak();

	*rvi=ix;
	return(0);
}

TKUCC_AstNode *TKUCC_ParseExprPostfix(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
//	tok0=TKUCC_TokStrmGetToken(ctx, ix+0);
//	tok1=TKUCC_TokStrmGetToken(ctx, ix+1);

	v0=TKUCC_ParseExprI(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);

		if(	!strcmp(s0, "++") ||
			!strcmp(s0, "--"))
		{
			ix++;
			v1=TKUCC_ParseExprI(ctx, &ix);
			v0=TKUCC_ParseWrapPostfix(ctx, s0, v0);
			continue;
		}

#if 0
		if(	!strcmp(s0, "*") ||
			!strcmp(s0, "/") ||
			!strcmp(s0, "%") )
		{
			ix++;
			v1=TKUCC_ParseExprI(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}
#endif
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprPrefix(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0, *s1;
//	s64 v0, v1;
	int ix, ix1;

	ix=*rvi;
	s0=TKUCC_TokStrmGetToken(ctx, ix+0);

	if(	!strcmp(s0, "++") ||
		!strcmp(s0, "--") ||
		!strcmp(s0, "+") ||
		!strcmp(s0, "-") ||
		!strcmp(s0, "!") ||
		!strcmp(s0, "~") ||
		!strcmp(s0, "*") ||
		!strcmp(s0, "&"))
	{
		ix++;
		v1=TKUCC_ParseExprPrefix(ctx, &ix);
		v0=TKUCC_ParseWrapUnary(ctx, s0, v1);
		*rvi=ix;
		return(v0);
	}
	
	if(!strcmp(s0, "("))
	{
		ix1=ix+1;
		v0=TKUCC_TryParseCastDecl(ctx, &ix1);
		s1=TKUCC_TokStrmGetToken(ctx, ix1);

		if(v0 && !strcmp(s1, ")"))
		{
			ix1++;
			v1=TKUCC_ParseExprPrefix(ctx, &ix1);
			v0=TKUCC_ParseWrapCast(ctx, v0, v1);
			*rvi=ix1;
			return(v0);
		}
	}
	
	v0=TKUCC_ParseExprI(ctx, &ix);
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprMD(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
//	tok0=TKUCC_TokStrmGetToken(ctx, ix+0);
//	tok1=TKUCC_TokStrmGetToken(ctx, ix+1);

	v0=TKUCC_ParseExprPrefix(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "*") ||
			!strcmp(s0, "/") ||
			!strcmp(s0, "%") )
		{
			ix++;
			v1=TKUCC_ParseExprPrefix(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprAS(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprMD(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "+") ||
			!strcmp(s0, "-"))
		{
			ix++;
			v1=TKUCC_ParseExprMD(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprSHLR(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprAS(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "<<") ||
			!strcmp(s0, ">>"))
		{
			ix++;
			v1=TKUCC_ParseExprAS(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprRelCmp(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprSHLR(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "<" ) ||
			!strcmp(s0, ">" ) ||
			!strcmp(s0, "<=") ||
			!strcmp(s0, ">=") )
		{
			ix++;
			v1=TKUCC_ParseExprSHLR(ctx, &ix);
			v0=TKUCC_ParseWrapCompare(ctx, s0, v0, v1);
			continue;
		}
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprEqCmp(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprRelCmp(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "==") ||
			!strcmp(s0, "!=") )
		{
			ix++;
			v1=TKUCC_ParseExprRelCmp(ctx, &ix);
			v0=TKUCC_ParseWrapCompare(ctx, s0, v0, v1);
			continue;
		}
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprBit(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprEqCmp(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "&") ||
			!strcmp(s0, "|") ||
			!strcmp(s0, "^"))
		{
			ix++;
			v1=TKUCC_ParseExprEqCmp(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}
		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprLogic(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprBit(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "&&") ||
			!strcmp(s0, "||"))
		{
			ix++;
			v1=TKUCC_ParseExprBit(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}

		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprTern(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1, *v2;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprLogic(ctx, &ix);

	s0=TKUCC_TokStrmGetToken(ctx, ix+0);
	if(!strcmp(s0, "?"))
	{
		ix++;
		v1=TKUCC_ParseExprTern(ctx, &ix);
		ix++;
		v2=TKUCC_ParseExprTern(ctx, &ix);
		
		v0=TKUCC_ParseWrapTern(ctx, v0, v1, v2);
	}

	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprAssign(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprTern(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(	!strcmp(s0, "=") ||
			!strcmp(s0, "+=") ||
			!strcmp(s0, "-=") ||
			!strcmp(s0, "*=") ||
			!strcmp(s0, "/=") ||
			!strcmp(s0, "%=") ||
			!strcmp(s0, "<<=") ||
			!strcmp(s0, ">>=") ||
			!strcmp(s0, "&=") ||
			!strcmp(s0, "|=") ||
			!strcmp(s0, "^="))
		{
			ix++;
			v1=TKUCC_ParseExprAssign(ctx, &ix);
			v0=TKUCC_ParseWrapAssign(ctx, s0, v0, v1);
			continue;
		}

		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExprComma(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *v0, *v1;
//	char **a;
	char *s0;
//	s64 v0, v1;
	int ix;

	ix=*rvi;
	v0=TKUCC_ParseExprAssign(ctx, &ix);
	while(1)
	{
		s0=TKUCC_TokStrmGetToken(ctx, ix+0);
		if(!strcmp(s0, ","))
		{
			ix++;
			v1=TKUCC_ParseExprAssign(ctx, &ix);
			v0=TKUCC_ParseWrapBinary(ctx, s0, v0, v1);
			continue;
		}

		break;
	}
	*rvi=ix;
	return(v0);
}

TKUCC_AstNode *TKUCC_ParseExpr(TKUCC_MainContext *ctx, int *rvi)
{
	return(TKUCC_ParseExprComma(ctx, rvi));
}
