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
		t=BGBCP_ReduceExpr(ctx, BCCX_Fetch(l, "value"));
		i=BGBCP_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_boolify, "boolify"))
	{
		t=BGBCP_ReduceExpr(ctx, BCCX_Fetch(l, "value"));
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

int BGBCP_IsIntP(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "int"))return(1);
	return(0);
}

int BGBCP_IsRealP(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "int"))return(1);
	if(BCCX_TagIsP(l, "real"))return(1);
	return(0);
}

int BGBCP_IsCharP(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "charstring"))
		return(1);
	return(0);
}

BCCX_Node *BGBCP_Boolify(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(BCCX_TagIsP(l, "boolify"))
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

	if(!ln || !rn)
		return(NULL);

	s0=BCCX_GetCst(ln, &bgbcc_rcst_tysuf, "tysuf");
	s1=BCCX_GetCst(rn, &bgbcc_rcst_tysuf, "tysuf");

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

	if(s0 && BGBCP_IsRealP(ctx, ln))
	{
		if(	!stricmp(s0, "L") ||
			!stricmp(s0, "LL") ||
			!stricmp(s0, "UL") ||
			!stricmp(s0, "ULL"))s0=NULL;
	}
	if(s1 && BGBCP_IsRealP(ctx, rn))
	{
		if(	!stricmp(s1, "L") ||
			!stricmp(s1, "LL") ||
			!stricmp(s1, "UL") ||
			!stricmp(s1, "ULL"))s1=NULL;
	}

	//int+real, throw out int suffix
	if(BGBCP_IsIntP(ctx, ln) && BGBCP_IsRealP(ctx, rn)) s0=NULL;
	if(BGBCP_IsRealP(ctx, ln) && BGBCP_IsIntP(ctx, rn)) s1=NULL;
	
	if(!s0 && !s1) return(NULL);
	if(!s0) return(s1);
	if(!s1) return(s0);
	
	if(!bgbcp_strcmp(s0, s1))
		return(s0);

	//throw away 'F', as it is lesser
	if(!bgbcp_strcmp1(s0, "F"))s0=NULL;
	if(!bgbcp_strcmp1(s1, "F"))s1=NULL;
	if(!s0 && !s1) return(NULL);
	if(!s0) return(s1);
	if(!s1) return(s0);
	
	//ok, whatever for now...
	
	return(NULL);
}

BCCX_Node *BGBCP_ReduceForm(BGBCP_ParseState *ctx,
	BCCX_Node *l, int flag)
{
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *suf;
	double f, g;
	int i0, i1;
	s64 i, j;
//	int i, j;

	if(!l)return(l);

	if(	(BCCX_TagIsP(l, "value")) ||
		(BCCX_TagIsP(l, "left")) ||
		(BCCX_TagIsP(l, "right")) ||
		(BCCX_TagIsP(l, "cond")) ||
		(BCCX_TagIsP(l, "then")) ||
		(BCCX_TagIsP(l, "else")) ||
		(BCCX_TagIsP(l, "body")))
	{
		return(BGBCP_ReduceForm(ctx,
			BCCX_Child(l), flag));
//		return(BCCX_Child(l));
	}

	if(BGBCP_IsUnaryP(ctx, l, "!"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_Fetch(l, "value"), flag);

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

		ln=BCCX_Fetch(t, "left");
		rn=BCCX_Fetch(t, "right");

		ln=BGBCP_ReduceForm(ctx, ln, flag);
		rn=BGBCP_ReduceForm(ctx, rn, flag);

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
		ln=BGBCP_ReduceForm(ctx, BCCX_Fetch(l, "left"), flag);
		rn=BGBCP_ReduceForm(ctx, BCCX_Fetch(l, "right"), flag);

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

		x=BGBCP_BinaryExpr("&&", ln, rn);
		return(x);
	}

	if(BGBCP_IsBinaryP(ctx, l, "||"))
	{
		ln=BGBCP_ReduceForm(ctx, BCCX_Fetch(l, "left"), flag);
		rn=BGBCP_ReduceForm(ctx, BCCX_Fetch(l, "right"), flag);

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

		x=BGBCP_BinaryExpr("||", ln, rn);
		return(x);
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		t=BGBCP_ReduceForm(ctx, BCCX_Fetch(l, "value"), flag);

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
			suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");

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
			suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
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

	if(BCCX_TagIsP(l, "binary"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");

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
				return(BGBCP_WrapInt(i<=j));
			}
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i>=j));
			}
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i<j));
			}
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCP_WrapInt(i>j));
			}

			suf=BGBCP_BinaryTySuf(ctx, s, ln, rn);
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
				return(BGBCP_WrapIntSuf(i/j, suf));
			}
			if(!bgbcp_strcmp1(s, "%"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
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
				return(BGBCP_WrapIntSuf(i>>j, suf));
			}
		}

		if(BGBCP_IsRealP(ctx, ln) && BGBCP_IsRealP(ctx, rn))
		{
			f=BCCX_GetFloat(ln, "value");
			g=BCCX_GetFloat(rn, "value");

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

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==1)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BGBCP_ReduceForm(ctx,
				BCCX_Fetch(l, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BCCX_Fetch(l, "else");
			if(t)t=BGBCP_ReduceForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_Fetch(l, "then");
		rn=BCCX_Fetch(l, "else");

		ln=BGBCP_ReduceForm(ctx, ln, flag);
		rn=BGBCP_ReduceForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		return(x);
	}

	if(BCCX_TagIsP(l, "begin"))
	{
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
	}

	if(BCCX_TagIsP(l, "ref"))
	{
		if(!ctx)	//preprocessor
		{
			t=BGBCP_WrapInt(0);
			return(t);
		}
	}

	if(BCCX_TagIsP(l, "sizeof"))
	{
		t=BCCX_FindTag(l, "type");
		if(!t)
		{
			t=BCCX_Fetch(l, "tyexpr");
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
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		t=BCCX_Fetch(l, "value");

		if(BGBCP_IsUnaryP(ctx, t, "&"))
		{
			if(flag&1)
				return(BGBCP_WrapInt(8));
		}

		if(BCCX_TagIsP(t, "ref"))
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

		if(BCCX_TagIsP(t, "loadindex"))
		{
			t=BCCX_Fetch(t, "array");
			if(BCCX_TagIsP(t, "ref"))
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

		if(BCCX_TagIsP(t, "string"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_value, "value");
			if(s)
			{
				i=strlen(s);
				return(BGBCP_WrapInt(i));
			}
		}
	}

	return(BCCX_Clone(l));
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
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *fnam;
	double f, g;
	int i, j, lnum;

	if(!l)return(l);

	fnam=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	lnum=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==1)
		{
			t=BGBCP_ReduceStatementForm(ctx,
				BCCX_Fetch(l, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)t=BGBCP_ReduceStatementForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_Fetch(l, "then");
		rn=BCCX_Fetch(l, "else");

		ln=BGBCP_ReduceStatementForm(ctx, ln, flag);
		rn=BGBCP_ReduceStatementForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		BGBCP_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsP(l, "begin"))
	{
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

		BGBCP_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsP(l, "while"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==0)
		{
			return(NULL);
		}

		v=BGBCP_ReduceStatementForm(ctx,
			BCCX_Fetch(l, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_NewCst2(&bgbcc_rcst_while, "while",
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));

		BGBCP_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsP(l, "do_while"))
	{
		t=BGBCP_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==0)
		{
			v=BGBCP_ReduceStatementForm(ctx,
				BCCX_Fetch(l, "body"), flag);
			return(v);
		}

		v=BGBCP_ReduceStatementForm(ctx,
			BCCX_Fetch(l, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_NewCst2(&bgbcc_rcst_do_while, "do_while",
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		BGBCP_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	if(BCCX_TagIsP(l, "for"))
	{
		t=BGBCP_ReduceForm(ctx, BCCX_Fetch(l, "cond"), flag);
		i=BGBCP_BoolExpr(ctx, t);
		if(i==0)
		{
			ln=BGBCP_ReduceStatementForm(ctx,
				BCCX_Fetch(l, "init"), flag);
			return(ln);
		}

		ln=BGBCP_ReduceStatementForm(ctx, BCCX_Fetch(l, "init"), flag);
		rn=BGBCP_ReduceStatementForm(ctx, BCCX_Fetch(l, "step"), flag);

		v=BGBCP_ReduceStatementForm(ctx, BCCX_Fetch(l, "body"), flag);

		x=BCCX_NewCst4(&bgbcc_rcst_for, "for",
			BCCX_NewCst1(&bgbcc_rcst_init, "init", ln),
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_step, "step", rn),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		BGBCP_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	t=BGBCP_ReduceForm(ctx, l, flag);
	return(t);
}

BCCX_Node *BGBCP_ReduceExpr(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCP_ReduceForm(ctx, l, 0);

	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}

BCCX_Node *BGBCP_ReduceStatement(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCP_ReduceStatementForm(ctx, l, 0);

	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}


BCCX_Node *BGBCP_ReduceExprConst(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCP_ReduceForm(ctx, l, 1);
	
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}
