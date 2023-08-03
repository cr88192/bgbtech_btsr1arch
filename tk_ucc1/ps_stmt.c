int TKUCC_TokenExpectOperatorP(TKUCC_MainContext *ctx, char *tok, char *sref)
{
	if(TKUCC_TokenCheckOperatorP(tok, sref))
		return(1);

	return(0);
}

int TKUCC_TokenExpectNameP(TKUCC_MainContext *ctx, char *tok, char *sref)
{
	if(TKUCC_TokenCheckNameP(tok, sref))
		return(1);

	return(0);
}

TKUCC_AstNode *TKUCC_ParseStmtBlock(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *n, *v0, *v1;
	char *t0, *t1;
	int ix, kwi;

	ix=*rvi;

	t0=TKUCC_TokStrmGetToken(ctx, ix);
	if(!TKUCC_TokenExpectOperatorP(ctx, t0, "{"))
		return(NULL);

	ix++;

	n=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_BEGIN, TKUCC_ZID_PARSE);

	while(1)
	{
		v1=TKUCC_ParseBlockStmt(ctx, &ix);
		if(!v1)
			return(NULL);

		TKUCC_AstAddNodeEnd(ctx, n, v1);

		t0=TKUCC_TokStrmGetToken(ctx, ix);
		if(TKUCC_TokenCheckOperatorP(t0, "}"))
		{
			ix++;
			break;
		}
	}
	
	*rvi=ix;
	return(n);
}

TKUCC_AstNode *TKUCC_ParseBlockStmt(TKUCC_MainContext *ctx, int *rvi)
{
	TKUCC_AstNode *n, *v0, *v1, *v2, *v3;
	char *t0, *t1;
	int ix, kwi;

	ix=*rvi;

	v0=TKUCC_TryParseDefinition(ctx, &ix);
	if(v0)
	{
		*rvi=ix;
		return(v0);
	}

	t0=TKUCC_TokStrmGetToken(ctx, ix+0);
	t1=TKUCC_TokStrmGetToken(ctx, ix+1);

	kwi=TKUCC_TokIndexKeyword(t0, 0);

	if(kwi>0)
	{
		if(kwi==TKUCC_KW_IF)
		{
			if(!TKUCC_TokenExpectOperatorP(ctx, t1, "("))
				return(NULL);
			ix+=2;

			v0=TKUCC_ParseExpr(ctx, &ix);

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(!TKUCC_TokenExpectOperatorP(ctx, t0, ")"))
				return(NULL);
			ix++;
			
			v1=TKUCC_ParseBlockStmt(ctx, &ix);
			v2=NULL;

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(TKUCC_TokenCheckNameP(t0, "else"))
			{
				ix++;
				v2=TKUCC_ParseBlockStmt(ctx, &ix);
			}

			n=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_IF, TKUCC_ZID_PARSE);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_CN, v0);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_LN, v1);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_RN, v2);
			return(n);
		}

		if(kwi==TKUCC_KW_WHILE)
		{
			if(!TKUCC_TokenExpectOperatorP(ctx, t1, "("))
				return(NULL);
			ix+=2;

			v0=TKUCC_ParseExpr(ctx, &ix);

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(!TKUCC_TokenExpectOperatorP(ctx, t0, ")"))
				return(NULL);
			ix++;
			
			v1=TKUCC_ParseBlockStmt(ctx, &ix);
			v2=NULL;

			n=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_WHILE, TKUCC_ZID_PARSE);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_CN, v0);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_VN, v1);
			return(n);
		}

		if(kwi==TKUCC_KW_FOR)
		{
			if(!TKUCC_TokenExpectOperatorP(ctx, t1, "("))
				return(NULL);
			ix+=2;

			v0=TKUCC_ParseBlockStmt(ctx, &ix);
			v1=TKUCC_ParseBlockStmt(ctx, &ix);
			v2=TKUCC_ParseExpr(ctx, &ix);

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(!TKUCC_TokenExpectOperatorP(ctx, t0, ")"))
				return(NULL);
			ix++;
			
			v3=TKUCC_ParseBlockStmt(ctx, &ix);

			n=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_FOR, TKUCC_ZID_PARSE);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_LN, v0);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_CN, v1);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_RN, v2);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_VN, v3);
			return(n);
		}

		if(kwi==TKUCC_KW_DO)
		{
			ix++;
			
			v1=TKUCC_ParseBlockStmt(ctx, &ix);

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			t1=TKUCC_TokStrmGetToken(ctx, ix+1);
			if(!TKUCC_TokenExpectNameP(ctx, t0, "while"))
				return(NULL);
			if(!TKUCC_TokenExpectOperatorP(ctx, t1, "("))
				return(NULL);
			ix+=2;

			v0=TKUCC_ParseExpr(ctx, &ix);

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(!TKUCC_TokenExpectOperatorP(ctx, t0, ")"))
				return(NULL);
			ix++;

			t0=TKUCC_TokStrmGetToken(ctx, ix);
			if(TKUCC_TokenExpectOperatorP(ctx, t0, ";"))
				ix++;

			n=TKUCC_AstAllocNode(ctx, TKUCC_ATAG_DOWHILE, TKUCC_ZID_PARSE);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_CN, v0);
			TKUCC_AstSetAttrNode(ctx, n, TKUCC_ATAG_VN, v1);
			return(n);
		}

		return(NULL);
	}
	
	if(TKUCC_TokenCheckOperatorP(t0, "{"))
	{
		v0=TKUCC_ParseStmtBlock(ctx, &ix);
		*rvi=ix;
		return(v0);
	}
	
	v0=TKUCC_ParseExpr(ctx, &ix);

	t0=TKUCC_TokStrmGetToken(ctx, ix);
	if(TKUCC_TokenExpectOperatorP(ctx, t0, ";"))
		ix++;

	*rvi=ix;
	return(v0);
}
