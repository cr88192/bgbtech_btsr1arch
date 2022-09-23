#include <bgbccc.h>

int BGBCP_IsUnaryP(BGBCP_ParseState *ctx, BCCX_Node *l, char *op)
{
	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
		return(0);
	return(BCCX_AttrIsCstP(l, &bgbcc_rcst_op, "op", op));
}

int BGBCP_IsBinaryP(BGBCP_ParseState *ctx, BCCX_Node *l, char *op)
{
	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
		return(0);
	return(BCCX_AttrIsCstP(l, &bgbcc_rcst_op, "op", op));
}

int BGBCP_BoolExpr(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	double f;
	int i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		i=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		if(!i)return(0);
		return(1);
	}
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
	{
		f=BCCX_GetFloatCst(l, &bgbcc_rcst_value, "value");
		if(f==0)return(0);
		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		t=BGBCP_ReduceExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		i=BGBCP_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_boolify, "boolify"))
	{
		t=BGBCP_ReduceExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		i=BGBCP_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_string, "string"))
		return(1);

	return(-1);
}

BCCX_Node *BGBCP_WrapIntSuf(s64 i, char *suf)
{
	BCCX_Node *t;
	
	if(suf)
	{
		if(!bgbcp_strcmp1(suf, "U"))
			i=(u32)i;
	}
	
	t=BCCX_NewCst(&bgbcc_rcst_int, "int");
	BCCX_SetIntCst(t, &bgbcc_rcst_value, "value", i);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCP_WrapRealSuf(double f, char *suf)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_real, "real");
	BCCX_SetFloatCst(t, &bgbcc_rcst_value, "value", f);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCP_WrapInt(s64 i)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_int, "int");
	BCCX_SetIntCst(t, &bgbcc_rcst_value, "value", i);
	return(t);
}

BCCX_Node *BGBCP_WrapReal(double f)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_real, "real");
	BCCX_SetFloatCst(t, &bgbcc_rcst_value, "value", f);
	return(t);
}

BCCX_Node *BGBCP_WrapString(char *str)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_string, "string");
	BCCX_SetCst(t, &bgbcc_rcst_value, "value", str);
	return(t);
}

BCCX_Node *BGBCP_WrapRef(char *str)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
	BCCX_SetCst(t, &bgbcc_rcst_name, "name", str);
	return(t);
}

int BGBCP_IsIntP(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))return(1);
	return(0);
}

int BGBCP_IsRealP(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))return(1);
	return(0);
}

int BGBCP_IsReal2P(BGBCP_ParseState *ctx, BCCX_Node *l)
{
//	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))return(1);
	return(0);
}

int BGBCP_IsCharP(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		return(1);
	return(0);
}

BCCX_Node *BGBCP_Boolify(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_boolify, "boolify"))
		return(l);
	t=BCCX_NewCst1(&bgbcc_rcst_boolify, "boolify",
		BCCX_NewCst1(&bgbcc_rcst_value, "value", l));
	return(t);
}

//deal with number type suffixes
char *BGBCP_BinaryTySuf(BGBCP_ParseState *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn)
{
	char *s0, *s1;

//	return(NULL);

//	if(!ln && !rn)
//		return(NULL);

	if(!ln || !rn)
		return(NULL);

	s0=BCCX_GetCst(ln, &bgbcc_rcst_tysuf, "tysuf");
	s1=BCCX_GetCst(rn, &bgbcc_rcst_tysuf, "tysuf");

	if(!s0 && !s1)
		return(NULL);

	//throw out bad suffixes early
	if(s0 && BGBCP_IsIntP(ctx, ln))
	{
		if(	!stricmp(s0, "F") ||
			!stricmp(s0, "D") ||
			!stricmp(s0, "M"))s0=NULL;
	}
	if(s1 && BGBCP_IsIntP(ctx, rn))
	{
		if(	!stricmp(s1, "F") ||
			!stricmp(s1, "D") ||
			!stricmp(s1, "M"))s1=NULL;
	}

	if(s0 && BGBCP_IsReal2P(ctx, ln))
	{
		if(	!stricmp(s0, "L") ||
			!stricmp(s0, "LL") ||
			!stricmp(s0, "UL") ||
			!stricmp(s0, "ULL"))s0=NULL;
	}
	if(s1 && BGBCP_IsReal2P(ctx, rn))
	{
		if(	!stricmp(s1, "L") ||
			!stricmp(s1, "LL") ||
			!stricmp(s1, "UL") ||
			!stricmp(s1, "ULL"))s1=NULL;
	}

	//int+real, throw out int suffix
	if(BGBCP_IsIntP(ctx, ln) && BGBCP_IsReal2P(ctx, rn))
		s0=NULL;
	if(BGBCP_IsReal2P(ctx, ln) && BGBCP_IsIntP(ctx, rn))
		s1=NULL;
	
	if(!s0 && !s1)
		return(NULL);
	if(!s0)
		return(s1);
	if(!s1)
		return(s0);
	
	if(!bgbcp_strcmp(s0, s1))
		return(s0);

	//throw away 'F', as it is lesser
	if(!bgbcp_strcmp1(s0, "F"))s0=NULL;
	if(!bgbcp_strcmp1(s1, "F"))s1=NULL;
	if(!s0 && !s1) return(NULL);
	if(!s0) return(s1);
	if(!s1) return(s0);
	
	if(*s0=='U')
	{
		if(!bgbcp_strcmp2(s0, "UL"))
		{
			if(!bgbcp_strcmp1(s1, "U"))
				return(s0);
			if(!bgbcp_strcmp1(s1, "L"))
				return(s0);
			return(NULL);
		}

		if(!bgbcp_strcmp3(s0, "ULL"))
		{
			if(!bgbcp_strcmp1(s1, "U"))
				return(s0);
			if(!bgbcp_strcmp1(s1, "L"))
				return(s0);
			if(!bgbcp_strcmp2(s1, "UL"))
				return(s0);
			if(!bgbcp_strcmp2(s1, "LL"))
				return(s0);
			return(NULL);
		}
	}

	if(*s1=='U')
	{
		if(!bgbcp_strcmp2(s1, "UL"))
		{
			if(!bgbcp_strcmp1(s0, "U"))
				return(s1);
			if(!bgbcp_strcmp1(s0, "L"))
				return(s1);
			return(NULL);
		}

		if(!bgbcp_strcmp3(s1, "ULL"))
		{
			if(!bgbcp_strcmp1(s0, "U"))
				return(s1);
			if(!bgbcp_strcmp1(s0, "L"))
				return(s1);
			if(!bgbcp_strcmp2(s0, "UL"))
				return(s1);
			if(!bgbcp_strcmp2(s0, "LL"))
				return(s1);
			return(NULL);
		}
	}
	
	//ok, whatever for now...
	
	return(NULL);
}

BCCX_Node *BGBCP_ReduceForm(BGBCP_ParseState *ctx,
	BCCX_Node *l, int flag)
{
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *suf;
	double f, g;
	int i0, i1, na, ci;
	s64 i, j;
//	int i, j;

	if(!l)return(l);

	if(	(BCCX_TagIsCstP(l, &bgbcc_rcst_value, "value")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_left, "left")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_right, "right")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_cond, "cond")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_then, "then")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_else, "else")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_body, "body")))
	{
		return(BGBCP_ReduceForm(ctx,
			BCCX_Child(l), flag));
//		return(BCCX_Child(l));
	}

	if(BGBCP_IsUnaryP(ctx, l, "!"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), flag);

		if(BGBCP_BoolExpr(ctx, t)==0)
		{
			BCCX_CheckDeleteUnlinked(t);
			return(BGBCP_Number(NULL, "1"));
		}
		if(BGBCP_BoolExpr(ctx, t)==1)
		{
			BCCX_CheckDeleteUnlinked(t);
			return(BGBCP_Number(NULL, "0"));
		}

		ln=BCCX_FetchCst(t, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(t, &bgbcc_rcst_right, "right");

		ln=BGBCP_ReduceForm(ctx, ln, flag);
		rn=BGBCP_ReduceForm(ctx, rn, flag);

#if 0
		x=NULL;
		if(BGBCP_IsBinaryP(ctx, t, "=="))
			x=BGBCP_BinaryExpr("!=", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, "!="))
			x=BGBCP_BinaryExpr("==", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, "==="))
			x=BGBCP_BinaryExpr("!==", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, "!=="))
			x=BGBCP_BinaryExpr("===", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, "<"))
			x=BGBCP_BinaryExpr(">=", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, ">"))
			x=BGBCP_BinaryExpr("<=", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, "<="))
			x=BGBCP_BinaryExpr(">", ln, rn);
		if(BGBCP_IsBinaryP(ctx, t, ">="))
			x=BGBCP_BinaryExpr("<", ln, rn);
#endif

#if 1
		x=NULL;
		if(BCCX_TagIsCstP(t, &bgbcc_rcst_binary, "binary"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_op, "op");

			if(!strcmp(s, "=="))
				x=BGBCP_BinaryExpr("!=", ln, rn);
			if(!strcmp(s, "!="))
				x=BGBCP_BinaryExpr("==", ln, rn);
			if(!strcmp(s, "==="))
				x=BGBCP_BinaryExpr("!==", ln, rn);
			if(!strcmp(s, "!=="))
				x=BGBCP_BinaryExpr("===", ln, rn);
			if(!strcmp(s, "<"))
				x=BGBCP_BinaryExpr(">=", ln, rn);
			if(!strcmp(s, ">"))
				x=BGBCP_BinaryExpr("<=", ln, rn);
			if(!strcmp(s, "<="))
				x=BGBCP_BinaryExpr(">", ln, rn);
			if(!strcmp(s, ">="))
				x=BGBCP_BinaryExpr("<", ln, rn);
		}
#endif

		BCCX_CheckDeleteUnlinked(t);
		t=NULL;

		if(x)
		{
			v=BGBCP_ReduceForm(ctx, x, flag);
			BCCX_CheckDeleteUnlinked(x);
			return(v);
		}
	}

	if(BGBCP_IsBinaryP(ctx, l, "&&"))
	{
		ln=BGBCP_ReduceForm(ctx, BCCX_FetchCst(l, &bgbcc_rcst_left, "left"), flag);
		rn=BGBCP_ReduceForm(ctx, BCCX_FetchCst(l, &bgbcc_rcst_right, "right"), flag);

#if 1
		i0=BGBCP_BoolExpr(ctx, ln);
		i1=BGBCP_BoolExpr(ctx, rn);
		if((i0==0) || (i1==0))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "0"));
		}

		if((i0==1) && (i1==1))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "1"));
		}

		if(i0==1)
		{
			BCCX_CheckDeleteUnlinked(ln);
			t=BGBCP_Boolify(ctx, rn);
			return(t);
		}

		if(i1==1)
		{
			BCCX_CheckDeleteUnlinked(rn);
			t=BGBCP_Boolify(ctx, ln);
			return(t);
		}
#endif

#if 0
		if((BGBCP_BoolExpr(ctx, ln)==0) ||
			(BGBCP_BoolExpr(ctx, rn)==0))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "0"));
		}

		if(BGBCP_BoolExpr(ctx, ln)==1)
		{
			BCCX_CheckDeleteUnlinked(ln);
			t=BGBCP_Boolify(ctx, rn);
			return(t);
		}

		if(BGBCP_BoolExpr(ctx, rn)==1)
		{
			BCCX_CheckDeleteUnlinked(rn);
			t=BGBCP_Boolify(ctx, ln);
			return(t);
		}
#endif

		x=BGBCP_BinaryExpr("&&", ln, rn);
		return(x);
	}

	if(BGBCP_IsBinaryP(ctx, l, "||"))
	{
		ln=BGBCP_ReduceForm(ctx, BCCX_FetchCst(l, &bgbcc_rcst_left, "left"), flag);
		rn=BGBCP_ReduceForm(ctx, BCCX_FetchCst(l, &bgbcc_rcst_right, "right"), flag);

#if 1
		i0=BGBCP_BoolExpr(ctx, ln);
		i1=BGBCP_BoolExpr(ctx, rn);
		if((i0==1) || (i1==1))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "1"));
		}

		if((i0==0) && (i1==0))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "0"));
		}

		if(i0==0)
		{
			BCCX_CheckDeleteUnlinked(ln);
			t=BGBCP_Boolify(ctx, rn);
			return(t);
		}

		if(i1==0)
		{
			BCCX_CheckDeleteUnlinked(rn);
			t=BGBCP_Boolify(ctx, ln);
			return(t);
		}
#endif

#if 0
		if((BGBCP_BoolExpr(ctx, ln)==1) ||
			(BGBCP_BoolExpr(ctx, rn)==1))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "1"));
		}

		if(BGBCP_BoolExpr(ctx, ln)==0)
		{
			BCCX_CheckDeleteUnlinked(ln);
			t=BGBCP_Boolify(ctx, rn);
			return(t);
		}

		if(BGBCP_BoolExpr(ctx, rn)==0)
		{
			BCCX_CheckDeleteUnlinked(rn);
			t=BGBCP_Boolify(ctx, ln);
			return(t);
		}
#endif

		x=BGBCP_BinaryExpr("||", ln, rn);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		t=BGBCP_ReduceForm(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), flag);

		if(BGBCP_IsCharP(ctx, t))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_value, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(t);
			t=BGBCP_WrapInt(i);
		}

		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(BGBCP_IsIntP(ctx, t))
		{
			i=BCCX_GetIntCst(t, &bgbcc_rcst_value, "value");
			suf=BCCX_GetCst(t, &bgbcc_rcst_tysuf, "tysuf");

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapIntSuf(i, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapIntSuf(-i, suf));
			}
			if(!bgbcp_strcmp1(s, "~"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapIntSuf(~i, suf));
			}
			if(!bgbcp_strcmp1(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapInt(!i));
			}
		}

		if(BGBCP_IsRealP(ctx, t))
		{
			f=BCCX_GetFloatCst(t, &bgbcc_rcst_value, "value");
			suf=BCCX_GetCst(t, &bgbcc_rcst_tysuf, "tysuf");
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapRealSuf(f, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapRealSuf(-f, suf));
			}
			if(!bgbcp_strcmp1(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_WrapInt(f!=0));
			}
		}

		x=BGBCP_UnaryExpr(s, t);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");

		ln=BGBCP_ReduceForm(ctx, ln, flag);
		rn=BGBCP_ReduceForm(ctx, rn, flag);

		if(BGBCP_IsCharP(ctx, ln))
		{
			s=BCCX_GetCst(ln, &bgbcc_rcst_value, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(ln);
			ln=BGBCP_WrapInt(i);
		}

		if(BGBCP_IsCharP(ctx, rn))
		{
			s=BCCX_GetCst(rn, &bgbcc_rcst_value, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(rn);
			rn=BGBCP_WrapInt(i);
		}

		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(BGBCP_IsIntP(ctx, ln) && BGBCP_IsIntP(ctx, rn))
		{
			i=BCCX_GetIntCst(ln, &bgbcc_rcst_value, "value");
			j=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");

			suf=BGBCP_BinaryTySuf(ctx, s, ln, rn);

			if(!bgbcp_strcmp2(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i==j));
			}
			if(!bgbcp_strcmp2(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i!=j));
			}
			if(!bgbcp_strcmp3(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i==j));
			}
			if(!bgbcp_strcmp3(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i!=j));
			}
			if(!bgbcp_strcmp2(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCP_WrapInt(((u64)i)<=((u64)j))); }
				return(BGBCP_WrapInt(i<=j));
			}
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCP_WrapInt(((u64)i)>=((u64)j))); }
				return(BGBCP_WrapInt(i>=j));
			}
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCP_WrapInt(((u64)i)<((u64)j))); }
				return(BGBCP_WrapInt(i<j));
			}
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCP_WrapInt(((u64)i)>((u64)j))); }
				return(BGBCP_WrapInt(i>j));
			}

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i+j, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i-j, suf));
			}
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i*j, suf));
			}
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(!j)
					return(BGBCP_WrapIntSuf(0, suf));				
				if(suf && (*suf=='U'))
				{
					return(BGBCP_WrapIntSuf(((u64)i)/((u64)j), suf));
				}
				return(BGBCP_WrapIntSuf(i/j, suf));
			}
			if(!bgbcp_strcmp1(s, "%"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(!j)
					return(BGBCP_WrapIntSuf(0, suf));				
				if(suf && (*suf=='U'))
				{
					return(BGBCP_WrapIntSuf(((u64)i)%((u64)j), suf));
				}
				return(BGBCP_WrapIntSuf(i%j, suf));
			}

			if(!bgbcp_strcmp1(s, "&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i&j, suf));
			}
			if(!bgbcp_strcmp1(s, "|"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i|j, suf));
			}
			if(!bgbcp_strcmp1(s, "^"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i^j, suf));
			}
			if(!bgbcp_strcmp1(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i/j, suf));
			}

			if(!bgbcp_strcmp2(s, "&&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i&&j));
			}
			if(!bgbcp_strcmp2(s, "||"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i||j));
			}

			if(!bgbcp_strcmp2(s, "<<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapIntSuf(i<<j, suf));
			}
			if(!bgbcp_strcmp2(s, ">>"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				
				if(suf && (*suf=='U'))
				{
					return(BGBCP_WrapIntSuf(((u64)i)>>j, suf));
				}
				
				return(BGBCP_WrapIntSuf(i>>j, suf));
			}
		}

		if(BGBCP_IsRealP(ctx, ln) && BGBCP_IsRealP(ctx, rn))
		{
			f=BCCX_GetFloatCst(ln, &bgbcc_rcst_value, "value");
			g=BCCX_GetFloatCst(rn, &bgbcc_rcst_value, "value");

			if(!bgbcp_strcmp2(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f==g));
			}
			if(!bgbcp_strcmp2(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f!=g));
			}
			if(!bgbcp_strcmp3(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f==g));
			}
			if(!bgbcp_strcmp3(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f!=g));
			}
			if(!bgbcp_strcmp2(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f<=g));
			}
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f>=g));
			}
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f<g));
			}
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f>g));
			}

			suf=BGBCP_BinaryTySuf(ctx, s, ln, rn);

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapRealSuf(f+g, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapRealSuf(f-g, suf));
			}
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapRealSuf(f*g, suf));
			}
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapRealSuf(f/g, suf));
			}
			if(!bgbcp_strcmp1(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(f/g));
			}
		}

		x=BGBCP_BinaryExpr(s, ln, rn);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==1)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BGBCP_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_then, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
			if(t)t=BGBCP_ReduceForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		ln=BGBCP_ReduceForm(ctx, ln, flag);
		rn=BGBCP_ReduceForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
#if 1
//		ln=BCCX_Child(l);

		na=BCCX_GetNodeChildCount(l);
		if(na<=0)
			return(NULL);
		
		if(na==1)
		{
			t=BGBCP_ReduceForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_NewCst(&bgbcc_rcst_begin, "begin");
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			t=BGBCP_ReduceForm(ctx, c, flag);
			BCCX_Add(x, t);
		}
		return(x);
#endif

#if 0
		if(!BCCX_Child(l))
		{
			return(NULL);
		}

		if(!BCCX_Next(BCCX_Child(l)))
		{
			t=BGBCP_ReduceForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_NewCst(&bgbcc_rcst_begin, "begin");
		c=BCCX_Child(l);
		while(c)
		{
			t=BGBCP_ReduceForm(ctx, c, flag);
			BCCX_Add(x, t);
			c=BCCX_Next(c);
		}
		return(x);
#endif
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		if(!ctx)	//preprocessor
		{
			t=BGBCP_WrapInt(0);
			return(t);
		}
		
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BGBPP_PpiFetchNameReduce(ctx, s);
		if(t)
			return(t);

		j=BGBCP_CheckNameEnum(ctx, s);
		if(j>=0)
		{
			t=BGBCP_WrapInt(j);
			return(t);
		}
		
		if((s[0]=='$') && (s[1]=='.'))
		{
			t=BGBCP_CheckTemplateParam(ctx, s+2);
			if(t)
			{
				return(BCCX_CloneS(t));
			}
		}else
		{
			t=BGBCP_CheckTemplateParam(ctx, s);
			if(t)
			{
				return(BCCX_CloneS(t));
			}
		}

		return(BCCX_CloneS(l));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof, "sizeof"))
	{
		t=BCCX_FindTag(l, "type");
		if(!t)
		{
			t=BCCX_FetchCst(l, &bgbcc_rcst_tyexpr, "tyexpr");
			if(t)t=BCCX_FindTag(t, "type");
		}

#if 0
		if(ctx && t)
		{
			i=BGBCP_TryGetSizeofType(ctx, t);
			if(i>0)return(BGBCP_WrapInt(i));
			
			if(flag&1)
			{
				i=BGBCP_GetMinMaxSizeofType(ctx, t,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCP_WrapInt(i0));
			}
		}
#endif

		if(ctx && ctx->template_inst_stackpos && t)
		{
			t=BGBCP_ReduceType(ctx, t);

			x=BCCX_NewCst(&bgbcc_rcst_sizeof, "sizeof");
			BCCX_AddV(x,
				BCCX_NewCst1V(&bgbcc_rcst_tyexpr, "tyexpr",
					BCCX_NewCst1(&bgbcc_rcst_var, "var", t)));
			return(x);
		}

	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof_expr, "sizeof_expr"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		if(BGBCP_IsUnaryP(ctx, t, "&"))
		{
			if(flag&1)
				return(BGBCP_WrapInt(8));
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");

#if 0
			i=BGBCP_TryGetSizeofName(ctx, s);
			if(i>0)return(BGBCP_WrapInt(i));
			
			if(flag&1)
			{
				i=BGBCP_GetMinMaxSizeofName(ctx, s,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCP_WrapInt(i0));
			}
#endif
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_loadindex, "loadindex"))
		{
			t=BCCX_FetchCst(t, &bgbcc_rcst_array, "array");
			if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
			{
				s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");

#if 0
				i=BGBCP_GetMinMaxSizeofDerefName(ctx, s,
					&i1, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
				{
					if((i0==i1) || (flag&1))
						return(BGBCP_WrapInt(i0));
				}
#endif
			}
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_string, "string"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_value, "value");
			if(s)
			{
				i=strlen(s);
				return(BGBCP_WrapInt(i));
			}
		}
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_list, "list"))
		{
		}


//		t=BGBCP_ReduceExpr(ctx, t);
//		i=BGBCP_BoolExpr(ctx, t);
//		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		t=BGBPP_PpiInterpTryReduce(ctx, l);
		if(t)
			return(t);
	}

	return(BCCX_CloneS(l));
}

int BGBCP_SetLine(BGBCP_ParseState *ctx, BCCX_Node *l,
	char *fn, int ln)
{
	if(fn)BCCX_SetCst(l, &bgbcc_rcst_fn, "fn", fn);
	if(ln>0)BCCX_SetIntCst(l, &bgbcc_rcst_ln, "ln", ln);
	return(0);
}

BCCX_Node *BGBCP_ReduceStatementForm(BGBCP_ParseState *ctx,
	BCCX_Node *l, int flag)
{
	BCCX_Node *c, *t, *v, *x, *a, *ln, *rn, *ty;
	char *s, *fnam;
	double f, g;
	int i, j, lnum, na, ci;

	if(!l)return(l);

	fnam=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	lnum=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==1)
		{
			t=BGBCP_ReduceStatementForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_then, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			t=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
			if(t)t=BGBCP_ReduceStatementForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		ln=BGBCP_ReduceStatementForm(ctx, ln, flag);
		rn=BGBCP_ReduceStatementForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		BGBCP_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
#if 1
//		ln=BCCX_Child(l);

		na=BCCX_GetNodeChildCount(l);
		if(na<=0)
			return(NULL);
		
		if(na==1)
		{
			t=BGBCP_ReduceStatementForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_NewCst(&bgbcc_rcst_begin, "begin");
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			t=BGBCP_ReduceStatementForm(ctx, c, flag);
			BCCX_Add(x, t);
		}
#endif

#if 0
		if(!BCCX_Child(l))
		{
			return(NULL);
		}

		if(!BCCX_Next(BCCX_Child(l)))
		{
			t=BGBCP_ReduceStatementForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_NewCst(&bgbcc_rcst_begin, "begin");
		c=BCCX_Child(l);
		while(c)
		{
			t=BGBCP_ReduceStatementForm(ctx, c, flag);
			BCCX_Add(x, t);
			c=BCCX_Next(c);
		}
#endif

		BGBCP_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==0)
		{
			return(NULL);
		}

		v=BGBCP_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_NewCst2(&bgbcc_rcst_while, "while",
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));

		BGBCP_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==0)
		{
			v=BGBCP_ReduceStatementForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);
			return(v);
		}

		v=BGBCP_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_NewCst2(&bgbcc_rcst_do_while, "do_while",
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		BGBCP_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==0)
		{
			ln=BGBCP_ReduceStatementForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_init, "init"), flag);
			return(ln);
		}

		ln=BGBCP_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_init, "init"), flag);
		rn=BGBCP_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_step, "step"), flag);

		v=BGBCP_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);

		x=BCCX_NewCst4(&bgbcc_rcst_for, "for",
			BCCX_NewCst1(&bgbcc_rcst_init, "init", ln),
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_step, "step", rn),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		BGBCP_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
#if 1
		na=BCCX_GetNodeChildCount(l);
		if(na<=0)
			return(NULL);
		
//		if(na==1)
//		{
//			t=BGBCP_ReduceStatementForm(ctx, BCCX_Child(l), flag);
//			return(t);
//		}
		
		x=BCCX_NewCst(&bgbcc_rcst_vars, "vars");
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			t=BGBCP_ReduceStatementForm(ctx, c, flag);
			BCCX_Add(x, t);
		}
#endif

		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_var, "var"))
	{
		ty=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");

		ty=BGBCP_ReduceType(ctx, ty);

		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		v=BGBCP_ReduceExpr(ctx, v);

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		x=BCCX_NewCst(&bgbcc_rcst_var, "var");
		BCCX_SetCst(x, &bgbcc_rcst_name, "name", s);
		BCCX_Add(x, ty);
		if(v)
		{
			BCCX_AddV(x, BCCX_NewCst1(&bgbcc_rcst_value, "value", v));
		}
		
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_args, "args"))
	{
		na=BCCX_GetNodeChildCount(l);
//		if(na<=0)
//			return(NULL);
		
		x=BCCX_NewCst(&bgbcc_rcst_args, "args");
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			t=BGBCP_ReduceStatementForm(ctx, c, flag);
			BCCX_Add(x, t);
		}
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		a=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		t=BGBCP_ReduceType(ctx, t);
		a=BGBCP_ReduceStatementForm(ctx, a, 0);
		v=BGBCP_ReduceStatementForm(ctx, v, 0);

		x=BCCX_NewCst(&bgbcc_rcst_defun, "defun");
		BCCX_SetCst(x, &bgbcc_rcst_name, "name", s);
		BCCX_Add(x, t);
		BCCX_Add(x, a);
		BCCX_AddV(x, BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_proto, "proto"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		a=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
//		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		t=BGBCP_ReduceType(ctx, t);
		a=BGBCP_ReduceStatementForm(ctx, a, 0);
//		v=BGBCP_ReduceStatementForm(ctx, v, 0);

		x=BCCX_NewCst(&bgbcc_rcst_proto, "proto");
		BCCX_SetCst(x, &bgbcc_rcst_name, "name", s);
		BCCX_Add(x, t);
		BCCX_Add(x, a);
//		BCCX_AddV(x, BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		
		return(x);
	}

	t=BGBCP_ReduceForm(ctx, l, flag);
	return(t);
}

BCCX_Node *BGBCP_ReduceType(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *n, *t, *v;
	char *s;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	if((s[0]=='$') && (s[1]=='.'))
	{
		t=BGBCP_CheckTemplateParam(ctx, s+2);
		if(t)
		{
			if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
			{
				n=BCCX_Clone(l);
				s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s);
			}else
				if(BCCX_TagIsCstP(t, &bgbcc_rcst_type, "type"))
			{
				n=BCCX_CloneS(t);
			}else
			{
				n=BCCX_CloneS(l);
			}
		}else
		{
			n=BCCX_CloneS(l);
		}
	}else
	{
		n=BCCX_CloneS(l);
	}
	
	return(n);
}

BCCX_Node *BGBCP_ReduceExpr(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCP_ReduceForm(ctx, l, 0);

#ifndef BGBCC_BCCX2
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}
#endif

	return(t);
}

BCCX_Node *BGBCP_ReduceStatement(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCP_ReduceStatementForm(ctx, l, 0);

#ifndef BGBCC_BCCX2
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}
#endif

	return(t);
}

BCCX_Node *BGBCP_ReduceStatementList(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t, *a, *c, *n;
	int na, ci;

	if(!l)
		return(NULL);
//	t=BGBCP_ReduceStatementForm(ctx, l, 0);

	n=BCCX_New(BCCX_Tag(l));

	na=BCCX_GetNodeChildCount(l);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(l, ci);
		t=BGBCP_ReduceStatement(ctx, c);
		if(!t)
			continue;
		BCCX_Add(n, t);
	}
	return(n);
}

BCCX_Node *BGBCP_ReduceExprConst(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCP_ReduceForm(ctx, l, 1);
	
#ifndef BGBCC_BCCX2
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}
#endif

	return(t);
}
