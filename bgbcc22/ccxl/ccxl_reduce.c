#include <bgbccc.h>


int BGBCC_CCXL_IsUnaryP(BGBCC_TransState *ctx, BCCX_Node *l, char *op)
{
	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
		return(0);
	return(BCCX_AttrIsCstP(l, &bgbcc_rcst_op, "op", op));

//	return(BCCX_TagAttrIsP(l, "unary", "op", op));
}

int BGBCC_CCXL_IsBinaryP(BGBCC_TransState *ctx, BCCX_Node *l, char *op)
{
	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
		return(0);
	return(BCCX_AttrIsCstP(l, &bgbcc_rcst_op, "op", op));

//	return(BCCX_TagAttrIsP(l, "binary", "op", op));
}

int BGBCC_CCXL_IsBinaryCompareP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char *s0;

	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
		return(0);

	s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
	if(!s0)
		return(0);
	
	if(!strcmp(s0, "=="))	return(1);
	if(!strcmp(s0, "!="))	return(1);
	if(!strcmp(s0, "<"))	return(1);
	if(!strcmp(s0, ">"))	return(1);
	if(!strcmp(s0, "<="))	return(1);
	if(!strcmp(s0, ">="))	return(1);
	if(!strcmp(s0, "&"))	return(1);
	return(0);
}

int BGBCC_CCXL_BoolExpr(BGBCC_TransState *ctx, BCCX_Node *l)
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
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_boolify, "boolify"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_string, "string"))
		return(1);

	return(-1);
}

int BGBCC_CCXL_IsFixIntAssignRVP(
	BGBCC_TransState *ctx, BCCX_Node *l,
	char **rname, int *rval)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	ccxl_label l0, l1, l2;
	char *name;
	char *s0, *s1, *s2;
	s64 li;
	int i, j, k;

	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
		return(0);

	s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
	if(s0)
		return(0);

	ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
	rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	name=NULL;
	if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref"))
		{ name=BCCX_GetCst(ln, &bgbcc_rcst_name, "name"); }

	i=0;
	if(BCCX_TagIsCstP(rn, &bgbcc_rcst_int, "int"))
	{
		li=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
		if(((int)li)==li)
			i=1;
	}
	
	if(name && i)
	{
		*rname=name;
		*rval=li;
		return(1);
	}

	return(0);
}

int BGBCC_CCXL_IsFixIntCompareRVP(
	BGBCC_TransState *ctx, BCCX_Node *l,
	char **rname, char **rcmp, int *rval)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	ccxl_label l0, l1, l2;
	char *name, *op;
	char *s0, *s1, *s2;
	s64 li;
	int i, j, k;

	if(!BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
		return(0);

	s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
	if(!s0)
		return(0);
	
	op=NULL;
	if(!strcmp(s0, "=="))op=s0;
	if(!strcmp(s0, "!="))op=s0;
	if(!strcmp(s0, "<" ))op=s0;
	if(!strcmp(s0, ">" ))op=s0;
	if(!strcmp(s0, "<="))op=s0;
	if(!strcmp(s0, ">="))op=s0;

	ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
	rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	name=NULL;
	if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref"))
		{ name=BCCX_GetCst(ln, &bgbcc_rcst_name, "name"); }

	i=0;
	if(BCCX_TagIsCstP(rn, &bgbcc_rcst_int, "int"))
	{
		li=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
		if(((int)li)==li)
			i=1;
	}
	
	if(name && op && i)
	{
		*rname=name;
		*rcmp=op;
		*rval=li;
		return(1);
	}

	return(0);
}

int BGBCC_CCXL_IsTagVarRVP(
	BGBCC_TransState *ctx, BCCX_Node *l,
	char *tag, char *name)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	ccxl_label l0, l1, l2;
	char *s0, *s1, *s2;
	s64 li;
	int i, j, k;

	if(!BCCX_TagIsP(l, tag))
		return(0);

	v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
	v=BGBCC_CCXL_ReduceExpr(ctx, v);

	s0=NULL;
	if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
		{ s0=BCCX_GetCst(v, &bgbcc_rcst_name, "name"); }
	
	if(s0 && name && !strcmp(s0, name))
		return(1);

	return(0);
}

BCCX_Node *BGBCC_CCXL_WrapIntSuf(s64 i, char *suf)
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

BCCX_Node *BGBCC_CCXL_WrapRealSuf(double f, char *suf)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_real, "real");
	BCCX_SetFloatCst(t, &bgbcc_rcst_value, "value", f);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapInt(s64 i)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_int, "int");
	BCCX_SetIntCst(t, &bgbcc_rcst_value, "value", i);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapReal(double f)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_real, "real");
	BCCX_SetFloatCst(t, &bgbcc_rcst_value, "value", f);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapComplex(double fr, double fi)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_complex, "complex");
	BCCX_SetFloatCst(t, &bgbcc_rcst_real, "real", fr);
	BCCX_SetFloatCst(t, &bgbcc_rcst_imag, "imag", fi);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapVec2(double x0, double x1, char *suf)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_vec2, "vec2");
	BCCX_SetFloatCst(t, &bgbcc_rcst_value_x0, "value_x0", x0);
	BCCX_SetFloatCst(t, &bgbcc_rcst_value_x1, "value_x1", x1);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapVec4(
	double x0, double x1,
	double x2, double x3,
	char *suf)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_vec4, "vec4");
	BCCX_SetFloatCst(t, &bgbcc_rcst_value_x0, "value_x0", x0);
	BCCX_SetFloatCst(t, &bgbcc_rcst_value_x1, "value_x1", x1);
	BCCX_SetFloatCst(t, &bgbcc_rcst_value_x2, "value_x2", x2);
	BCCX_SetFloatCst(t, &bgbcc_rcst_value_x3, "value_x3", x3);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}


BCCX_Node *BGBCC_CCXL_WrapInt128(bgbcc_vint128 i)
{
	return(BGBCC_CCXL_WrapInt128Suf(i, NULL));
}

BCCX_Node *BGBCC_CCXL_WrapInt128Suf(bgbcc_vint128 i, char *suf)
{
	BCCX_Node *t;
	
#if 0
	if(BGBCC_Int128_CheckSx64P(i))
	{
		t=BGBCC_CCXL_WrapIntSuf(i.lo, suf);
		return(t);
	}
#endif
	
	t=BCCX_NewCst(&bgbcc_rcst_int128, "int128");
	BCCX_SetIntCst(t, &bgbcc_rcst_value_lo, "value_lo", i.lo);
	BCCX_SetIntCst(t, &bgbcc_rcst_value_hi, "value_hi", i.hi);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapFloat128(bgbcc_vfloat128 f)
{
	return(BGBCC_CCXL_WrapFloat128Suf(f, NULL));
}

BCCX_Node *BGBCC_CCXL_WrapFloat128Suf(bgbcc_vfloat128 f, char *suf)
{
	BCCX_Node *t;
	t=BCCX_NewCst(&bgbcc_rcst_float128, "float128");
	BCCX_SetIntCst(t, &bgbcc_rcst_value_lo, "value_lo", f.lo);
	BCCX_SetIntCst(t, &bgbcc_rcst_value_hi, "value_hi", f.hi);
	if(suf)BCCX_SetCst(t, &bgbcc_rcst_tysuf, "tysuf", suf);
	return(t);
}

bgbcc_vint128 BGBCC_CCXL_UnwrapInt128(BGBCC_TransState *ctx, BCCX_Node *l)
{
	bgbcc_vint128 c;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
	{
		c.lo=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		c.hi=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		return(c);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		c.lo=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		c.hi=((s64)c.lo)>>63;
		return(c);
	}

	BGBCC_DBGBREAK
	
	c.lo=0;
	c.hi=0;
	return(c);
}

bgbcc_vfloat128 BGBCC_CCXL_UnwrapFloat128(BGBCC_TransState *ctx, BCCX_Node *l)
{
	double f;
	bgbcc_vfloat128 c;
	bgbcc_vint128 xi;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_float128, "float128"))
	{
		c.lo=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		c.hi=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		return(c);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
	{
		f=BCCX_GetFloatCst(l, &bgbcc_rcst_value, "value");
		c=BGBCC_Float128_FromDouble(f);
		return(c);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		f=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		c=BGBCC_Float128_FromDouble(f);
		return(c);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
	{
		xi.lo=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		xi.hi=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		c=BGBCC_Float128_FromInt128(xi);
		return(c);
	}

	BGBCC_DBGBREAK
	
	c.lo=0;
	c.hi=0;
	return(c);
}

int BGBCC_CCXL_IsIntP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsRealP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsReal2P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsComplexP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_complex, "complex"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsSmallComplexP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_complex, "complex"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsCharP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsInt128P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsSmallInt128P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsFloat128P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_float128, "float128"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsReal128P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_float128, "float128"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
		return(1);
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsVec2P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vec2, "vec2"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsVec4P(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vec4, "vec4"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsQuatP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char *suf;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vec4, "vec4"))
	{
		suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		if(suf && !strcmp(suf, "QF"))
			return(1);
		return(0);
	}
	return(0);
}

int BGBCC_CCXL_IsSmallQuatP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BGBCC_CCXL_IsQuatP(ctx, l))
		return(1);
	if(BGBCC_CCXL_IsSmallComplexP(ctx, l))
		return(1);
	return(0);
}

BCCX_Node *BGBCC_CCXL_Boolify(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_boolify, "boolify"))
		return(l);
	t=BCCX_NewCst1(&bgbcc_rcst_boolify, "boolify",
		BCCX_NewCst1(&bgbcc_rcst_value, "value", l));
	return(t);
}

//deal with number type suffixes
char *BGBCC_CCXL_BinaryTySuf(BGBCC_TransState *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn)
{
	char *s0, *s1;

//	return(NULL);

	if(!ln || !rn)
		return(NULL);

	s0=BCCX_GetCst(ln, &bgbcc_rcst_tysuf, "tysuf");
	s1=BCCX_GetCst(rn, &bgbcc_rcst_tysuf, "tysuf");

	if(!s0 && !s1)
		return(NULL);

	//throw out bad suffixes early
	if(s0 && BGBCC_CCXL_IsIntP(ctx, ln))
	{
		if(	!stricmp(s0, "F") ||
			!stricmp(s0, "D") ||
			!stricmp(s0, "M"))s0=NULL;
	}
	if(s1 && BGBCC_CCXL_IsIntP(ctx, rn))
	{
		if(	!stricmp(s1, "F") ||
			!stricmp(s1, "D") ||
			!stricmp(s1, "M"))s1=NULL;
	}

	if(s0 && BGBCC_CCXL_IsReal2P(ctx, ln))
	{
		if(	!stricmp(s0, "L") ||
			!stricmp(s0, "LL") ||
			!stricmp(s0, "UL") ||
			!stricmp(s0, "ULL"))s0=NULL;
	}
	if(s1 && BGBCC_CCXL_IsReal2P(ctx, rn))
	{
		if(	!stricmp(s1, "L") ||
			!stricmp(s1, "LL") ||
			!stricmp(s1, "UL") ||
			!stricmp(s1, "ULL"))s1=NULL;
	}

	//int+real, throw out int suffix
	if(BGBCC_CCXL_IsIntP(ctx, ln) && BGBCC_CCXL_IsReal2P(ctx, rn)) s0=NULL;
	if(BGBCC_CCXL_IsReal2P(ctx, ln) && BGBCC_CCXL_IsIntP(ctx, rn)) s1=NULL;
	
	if(!s0 && !s1) return(NULL);
	if(!s0) return(s1);
	if(!s1) return(s0);
	
	if(!strcmp(s0, s1))
		return(s0);

	//throw away 'F', as it is lesser
	if(!strcmp(s0, "F"))s0=NULL;
	if(!strcmp(s1, "F"))s1=NULL;
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

BCCX_Node *BGBCC_CCXL_ReduceForm(BGBCC_TransState *ctx,
	BCCX_Node *l, int flag)
{
	char tb1[256], tb2[256];
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	BGBCC_CCXL_RegisterInfo *ri;
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type bty;
	bgbcc_vint128 xi, xj, xk;
	bgbcc_vfloat128 xf, xg, xh;
	char *s, *s0, *s1, *suf;
	double f0, f1, f2, f3;
	double g0, g1, g2, g3;
	double h0, h1, h2, h3;
	double f, g;
	int sqn, tkn;
	int i0, i1;
	s64 i, j;
//	int i, j;

	if(!l)return(l);

#if 0
	if(	(BCCX_TagIsCstP(l, &bgbcc_rcst_value, "value")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_left, "left")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_right, "right")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_cond, "cond")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_then, "then")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_else, "else")) ||
		(BCCX_TagIsCstP(l, &bgbcc_rcst_body, "body")))
#endif

#if 1
	if(	(l->itag==bgbcc_rcst_value) ||
		(l->itag==bgbcc_rcst_left) ||
		(l->itag==bgbcc_rcst_right) ||
		(l->itag==bgbcc_rcst_cond) ||
		(l->itag==bgbcc_rcst_then) ||
		(l->itag==bgbcc_rcst_else) ||
		(l->itag==bgbcc_rcst_body))
#endif
	{
		return(BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Child(l), flag));
//		return(BCCX_Child(l));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		if(!ctx)	//preprocessor
		{
			t=BGBCC_CCXL_WrapInt(0);
			return(t);
		}

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		if(	(ctx->lang==BGBCC_LANG_CS)	||
			(ctx->lang==BGBCC_LANG_BS2)	||
			(ctx->lang==BGBCC_LANG_JAVA)	)
		{
			if(!bgbcp_strcmp(s, "true"))
			{
				t=BGBCC_CCXL_WrapIntSuf(1, "SB");
				return(t);
			}
			if(!bgbcp_strcmp(s, "false"))
			{
				t=BGBCC_CCXL_WrapIntSuf(0, "SB");
				return(t);
			}
		}
		
		ri=NULL;
		
		for(j=ctx->vlcl_stackpos-1; j>=0; j--)
		{
			sprintf(tb1, "%s!%d", s, ctx->vlcl_stack[j]);
			i=BGBCC_CCXL_LookupLocalIndex(ctx, tb1);
			if(i>=0)
				{ ri=ctx->cur_func->locals[i]; break; }
			if(!ri)
			{
				i=BGBCC_CCXL_LookupStaticIndex(ctx, tb1);
				if(i>=0)
					{ ri=ctx->cur_func->statics[i]; break; }
			}
		}
		
		if(!ri)
		{
			i=BGBCC_CCXL_LookupLocalIndex(ctx, s);
			if(i>=0)
				{ ri=ctx->cur_func->locals[i]; }
		}

		if(!ri)
		{
			i=BGBCC_CCXL_LookupStaticIndex(ctx, s);
			if(i>=0)
				{ ri=ctx->cur_func->statics[i]; }
		}

		if(!ri)
		{
			i=BGBCC_CCXL_LookupArgIndex(ctx, s);
			if(i>=0)
				{ ri=ctx->cur_func->args[i]; }
		}

		if(!ri)
		{
			if(!ri && (ctx->cur_struct || ctx->cur_ns))
			{
				s1=BGBCC_CCXL_QualifyNameNS(ctx, s);
				ri=BGBCC_CCXL_LookupGlobal(ctx, s1);
			}

			if(!ri)
			{
				s1=BGBCC_CCXL_QualifyNameNSFl(ctx, s, BGBCC_TYFL_STATIC);
				ri=BGBCC_CCXL_LookupGlobal(ctx, s1);
			}
			
			if(!ri && ctx->cur_struct)
			{
				ri=BGBCC_CCXL_LookupStructureStaticMember(ctx,
					ctx->cur_struct, s);
			}

			if(!ri && ctx->cur_struct)
			{
				ri=BGBCC_CCXL_LookupStructureMember(ctx,
					ctx->cur_struct, s);
			}
			
			if(!ri)
			{
	//			ri=BGBCC_CCXL_LookupGlobal(ctx, s);
				ri=BGBCC_CCXL_LookupGlobalRns(ctx, s);
			}
		}
		
//		ri=NULL;

//		if(ri && ri->flagstr && !strcmp(ri->flagstr, "k"))
//		if(ri && BGBCC_CCXL_CheckFlagstrFlag(ri->flagstr, "k"))
		if(ri && BGBCC_CCXL_TypeSmallLongP(ctx, ri->type) &&
			BGBCC_CCXL_CheckFlagstrFlag(ri->flagstr, "k") &&
			!(flag&16))
		{
			if(BGBCC_CCXL_IsRegImmIntP(ctx, ri->value))
			{
				i=BGBCC_CCXL_GetRegImmIntValue(ctx, ri->value);
				t=BGBCC_CCXL_WrapInt(i);
				return(t);
			}

			if(BGBCC_CCXL_IsRegImmLongP(ctx, ri->value))
			{
				i=BGBCC_CCXL_GetRegImmLongValue(ctx, ri->value);
				t=BGBCC_CCXL_WrapIntSuf(i, "L");
				return(t);
			}

#if 1
			if(BGBCC_CCXL_IsRegImmFloatP(ctx, ri->value))
			{
				f=BGBCC_CCXL_GetRegImmFloatValue(ctx, ri->value);
				t=BGBCC_CCXL_WrapRealSuf(f, "F");
				return(t);
			}

			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, ri->value))
			{
				f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, ri->value);
				t=BGBCC_CCXL_WrapReal(f);
				return(t);
			}
#endif
		}

#if 0
		if((flag&2) && (ctx->lang==BGBCC_LANG_CS))
		{
			BGBCC_DBGBREAK

			if(!ri && (ctx->cur_struct || ctx->cur_ns))
			{
				s1=BGBCC_CCXL_QualifyNameNS(ctx, s);
				ri=BGBCC_CCXL_LookupGlobal(ctx, s1);
			}
		}
#endif
		
//		if(ctx->lang!=BGBCC_LANG_C)
		if(1)
		{
//			if(ri && strcmp(ri->name, s))
			if(ri && strcmp(ri->name, s) &&
				!(ri->qname && !strcmp(ri->qname, s)))
			{
				t=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
				BCCX_SetCst(t, &bgbcc_rcst_name, "name", ri->name);
				return(t);
			}

			if(ri && ri->qname && strcmp(ri->qname, s))
			{
				t=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
				BCCX_SetCst(t, &bgbcc_rcst_name, "name", ri->qname);
				return(t);
			}
		}
		
		if(ctx->lang==BGBCC_LANG_CS)
		{
			if(ctx->cur_struct)
			{
				if(BGBCC_CCXL_CheckForCNameGetter(ctx, ctx->cur_struct, s) ||
					BGBCC_CCXL_CheckForCNameSetter(ctx, ctx->cur_struct, s) ||
					BGBCC_CCXL_LookupStructureMember(ctx, ctx->cur_struct, s) ||
					BGBCC_CCXL_LookupStructureVirtualMethod(ctx,
						ctx->cur_struct, s, NULL))
				{
					v=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
					BCCX_SetCst(v, &bgbcc_rcst_name, "name", "this");
					
					t=BCCX_NewCst(&bgbcc_rcst_objref, "objref");
					BCCX_AddV(t, BCCX_NewCst1V(&bgbcc_rcst_value, "value", v));
					BCCX_SetCst(t, &bgbcc_rcst_name, "name", s);
					return(t);
				}
			}
		}

		return(BCCX_Clone(l));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		if(!ctx)
			return(BCCX_Clone(l));
	
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s1=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			ri=BGBCC_CCXL_LookupStructureStaticMemberRns(ctx, s1, s);
			
			if(ri && BGBCC_CCXL_TypeSmallLongP(ctx, ri->type) &&
				BGBCC_CCXL_CheckFlagstrFlag(ri->flagstr, "k") &&
				!(flag&16))
			{
				if(BGBCC_CCXL_IsRegImmIntP(ctx, ri->value))
				{
					i=BGBCC_CCXL_GetRegImmIntValue(ctx, ri->value);
					t=BGBCC_CCXL_WrapInt(i);
					return(t);
				}

				if(BGBCC_CCXL_IsRegImmLongP(ctx, ri->value))
				{
					i=BGBCC_CCXL_GetRegImmLongValue(ctx, ri->value);
					t=BGBCC_CCXL_WrapIntSuf(i, "L");
					return(t);
				}

#if 1
				if(BGBCC_CCXL_IsRegImmFloatP(ctx, ri->value))
				{
					f=BGBCC_CCXL_GetRegImmFloatValue(ctx, ri->value);
					t=BGBCC_CCXL_WrapRealSuf(f, "F");
					return(t);
				}

				if(BGBCC_CCXL_IsRegImmDoubleP(ctx, ri->value))
				{
					f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, ri->value);
					t=BGBCC_CCXL_WrapReal(f);
					return(t);
				}
#endif
			}
			
			if(ri)
			{
				if(ri->qname)
				{
					t=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
					BCCX_SetCst(t, &bgbcc_rcst_name, "name", ri->qname);
					return(t);
				}
			}

			obj=BGBCC_CCXL_LookupNamespace(ctx, s1);
			if(obj)
			{
				sprintf(tb1, "%s/%s", s1, s);
				s0=bgbcc_strdup(tb1);
				t=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
				BCCX_SetCst(t, &bgbcc_rcst_name, "name", s0);
				return(t);
			}
		}

		v=t;
		t=BCCX_NewCst(&bgbcc_rcst_objref, "objref");
		BCCX_AddV(t, BCCX_NewCst1V(&bgbcc_rcst_value, "value", v));
		BCCX_SetCst(t, &bgbcc_rcst_name, "name", s);
		return(t);

//		return(BCCX_Clone(l));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		if(BGBCC_CCXL_IsUnaryP(ctx, l, "!"))
		{
			t=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), flag);

			i0=BGBCC_CCXL_BoolExpr(ctx, t);
			if(i0==0)
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_Number(NULL, "1"));
			}
			if(i0==1)
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCP_Number(NULL, "0"));
			}

			ln=BCCX_FetchCst(t, &bgbcc_rcst_left, "left");
			rn=BCCX_FetchCst(t, &bgbcc_rcst_right, "right");

			ln=BGBCC_CCXL_ReduceForm(ctx, ln, flag);
			rn=BGBCC_CCXL_ReduceForm(ctx, rn, flag);

	#if 0
			x=NULL;
			if(BGBCC_CCXL_IsBinaryP(ctx, t, "=="))
				x=BGBCP_BinaryExpr("!=", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, "!="))
				x=BGBCP_BinaryExpr("==", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, "==="))
				x=BGBCP_BinaryExpr("!==", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, "!=="))
				x=BGBCP_BinaryExpr("===", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, "<"))
				x=BGBCP_BinaryExpr(">=", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, ">"))
				x=BGBCP_BinaryExpr("<=", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, "<="))
				x=BGBCP_BinaryExpr(">", ln, rn);
			if(BGBCC_CCXL_IsBinaryP(ctx, t, ">="))
				x=BGBCP_BinaryExpr("<", ln, rn);
	#endif

	#if 1
			x=NULL;
			if(BCCX_TagIsCstP(t, &bgbcc_rcst_binary, "binary"))
			{
				s=BCCX_GetCst(t, &bgbcc_rcst_op, "op");

	//			if(!strcmp(s, "=="))
				if(!bgbcp_strcmp2(s, "=="))
					x=BGBCP_BinaryExpr("!=", ln, rn);
	//			if(!strcmp(s, "!="))
				if(!bgbcp_strcmp2(s, "!="))
					x=BGBCP_BinaryExpr("==", ln, rn);
	//			if(!strcmp(s, "==="))
				if(!bgbcp_strcmp3(s, "==="))
					x=BGBCP_BinaryExpr("!==", ln, rn);
	//			if(!strcmp(s, "!=="))
				if(!bgbcp_strcmp3(s, "!=="))
					x=BGBCP_BinaryExpr("===", ln, rn);
	//			if(!strcmp(s, "<"))
				if(!bgbcp_strcmp1(s, "<"))
					x=BGBCP_BinaryExpr(">=", ln, rn);
	//			if(!strcmp(s, ">"))
				if(!bgbcp_strcmp1(s, ">"))
					x=BGBCP_BinaryExpr("<=", ln, rn);
	//			if(!strcmp(s, "<="))
				if(!bgbcp_strcmp2(s, "<="))
					x=BGBCP_BinaryExpr(">", ln, rn);
	//			if(!strcmp(s, ">="))
				if(!bgbcp_strcmp2(s, ">="))
					x=BGBCP_BinaryExpr("<", ln, rn);
			}
	#endif

			BCCX_CheckDeleteUnlinked(t);
			t=NULL;

			if(x)
			{
				v=BGBCC_CCXL_ReduceForm(ctx, x, flag);
				BCCX_CheckDeleteUnlinked(x);
				return(v);
			}
		}

		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");
		if(!bgbcp_strcmp1(s, "&"))
			flag|=16;

		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"), flag);

		if(BGBCC_CCXL_IsCharP(ctx, t))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_value, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(t);
			t=BGBCC_CCXL_WrapInt(i);
		}

//		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(BGBCC_CCXL_IsIntP(ctx, t))
		{
			i=BCCX_GetIntCst(t, &bgbcc_rcst_value, "value");
//			suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
			suf=BCCX_GetCst(t, &bgbcc_rcst_tysuf, "tysuf");

//			if(!strcmp(s, "+"))
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapIntSuf(i, suf));
			}
//			if(!strcmp(s, "-"))
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapIntSuf(-i, suf));
			}
//			if(!strcmp(s, "~"))
			if(!bgbcp_strcmp1(s, "~"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapIntSuf(~i, suf));
			}
//			if(!strcmp(s, "!"))
			if(!bgbcp_strcmp1(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapInt(!i));
			}
		}

		if(BGBCC_CCXL_IsRealP(ctx, t))
		{
			f=BCCX_GetFloatCst(t, &bgbcc_rcst_value, "value");
			suf=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
//			if(!strcmp(s, "+"))
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapRealSuf(f, suf));
			}
//			if(!strcmp(s, "-"))
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapRealSuf(-f, suf));
			}
//			if(!strcmp(s, "!"))
			if(!bgbcp_strcmp1(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapInt(f!=0));
			}
		}

		if(BGBCC_CCXL_IsComplexP(ctx, t))
		{
			f0=BCCX_GetFloatCst(t, &bgbcc_rcst_real, "real");
			f1=BCCX_GetFloatCst(t, &bgbcc_rcst_imag, "imag");
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapComplex(f0, f1));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapComplex(-f0, -f1));
			}
		}

		if(BGBCC_CCXL_IsInt128P(ctx, t))
		{
			xi=BGBCC_CCXL_UnwrapInt128(ctx, t);
			suf=BCCX_GetCst(t, &bgbcc_rcst_tysuf, "tysuf");

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapInt128Suf(xi, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				xk=BGBCC_Int128_Neg(xi);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp1(s, "~"))
			{
				BCCX_CheckDeleteUnlinked(t);
				xk=BGBCC_Int128_Not(xi);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp1(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				i=BGBCC_Int128_LNot(xi);
				return(BGBCC_CCXL_WrapInt(i));
			}
		}

		x=BGBCP_UnaryExpr(s, t);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&"))
		{
			ln=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_left, "left"), flag);
			rn=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_right, "right"), flag);

			i0=BGBCC_CCXL_BoolExpr(ctx, ln);
			i1=BGBCC_CCXL_BoolExpr(ctx, rn);

			if((i0==0) || (i1==0))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
	//			return(BGBCP_Number(NULL, "0"));
				return(BGBCC_CCXL_WrapInt(0));
			}

			if((i0==1) && (i1==1))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
	//			return(BGBCP_Number(NULL, "1"));
				return(BGBCC_CCXL_WrapInt(1));
			}

			if(i0==1)
			{
				BCCX_CheckDeleteUnlinked(ln);
				t=BGBCC_CCXL_Boolify(ctx, rn);
				return(t);
			}

			if(i1==1)
			{
				BCCX_CheckDeleteUnlinked(rn);
				t=BGBCC_CCXL_Boolify(ctx, ln);
				return(t);
			}

			x=BGBCP_BinaryExpr("&&", ln, rn);
			return(x);
		}

		if(BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
		{
			ln=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_left, "left"), flag);
			rn=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_right, "right"), flag);

			i0=BGBCC_CCXL_BoolExpr(ctx, ln);
			i1=BGBCC_CCXL_BoolExpr(ctx, rn);

			if((i0==1) || (i1==1))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
	//			return(BGBCP_Number(NULL, "1"));
				return(BGBCC_CCXL_WrapInt(1));
			}

			if((i0==0) && (i1==0))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
	//			return(BGBCP_Number(NULL, "0"));
				return(BGBCC_CCXL_WrapInt(0));
			}

			if(i0==0)
			{
				BCCX_CheckDeleteUnlinked(ln);
				t=BGBCC_CCXL_Boolify(ctx, rn);
				return(t);
			}

			if(i1==0)
			{
				BCCX_CheckDeleteUnlinked(rn);
				t=BGBCC_CCXL_Boolify(ctx, ln);
				return(t);
			}

			x=BGBCP_BinaryExpr("||", ln, rn);
			return(x);
		}


		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");

		ln=BGBCC_CCXL_ReduceForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, rn, flag);

		if(BGBCC_CCXL_IsCharP(ctx, ln))
		{
			s=BCCX_GetCst(ln, &bgbcc_rcst_value, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(ln);
			ln=BGBCC_CCXL_WrapInt(i);
		}

		if(BGBCC_CCXL_IsCharP(ctx, rn))
		{
			s=BCCX_GetCst(rn, &bgbcc_rcst_value, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(rn);
			rn=BGBCC_CCXL_WrapInt(i);
		}

		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(BGBCC_CCXL_IsIntP(ctx, ln) && BGBCC_CCXL_IsIntP(ctx, rn))
		{
			i=BCCX_GetIntCst(ln, &bgbcc_rcst_value, "value");
			j=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");

			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

//			if(!strcmp(s, "=="))
			if(!bgbcp_strcmp2(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i==j));
			}
//			if(!strcmp(s, "!="))
			if(!bgbcp_strcmp2(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i!=j));
			}
//			if(!strcmp(s, "==="))
			if(!bgbcp_strcmp3(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i==j));
			}
//			if(!strcmp(s, "!=="))
			if(!bgbcp_strcmp3(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i!=j));
			}
//			if(!strcmp(s, "<="))
			if(!bgbcp_strcmp2(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCC_CCXL_WrapInt(((u64)i)<=((u64)j))); }
				return(BGBCC_CCXL_WrapInt(i<=j));
			}
//			if(!strcmp(s, ">="))
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCC_CCXL_WrapInt(((u64)i)>=((u64)j))); }
				return(BGBCC_CCXL_WrapInt(i>=j));
			}
//			if(!strcmp(s, "<"))
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCC_CCXL_WrapInt(((u64)i)<((u64)j))); }
				return(BGBCC_CCXL_WrapInt(i<j));
			}
//			if(!strcmp(s, ">"))
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					{ return(BGBCC_CCXL_WrapInt(((u64)i)>((u64)j))); }
				return(BGBCC_CCXL_WrapInt(i>j));
			}

//			if(!strcmp(s, "+"))
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i+j, suf));
			}
//			if(!strcmp(s, "-"))
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i-j, suf));
			}
//			if(!strcmp(s, "*"))
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i*j, suf));
			}
//			if(!strcmp(s, "/"))
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);

				if(!j)
					return(BGBCP_WrapIntSuf(0, suf));				
				if(suf && (*suf=='U'))
				{
					return(BGBCC_CCXL_WrapIntSuf(((u64)i)/((u64)j), suf));
				}

				return(BGBCC_CCXL_WrapIntSuf(i/j, suf));
			}
//			if(!strcmp(s, "%"))
			if(!bgbcp_strcmp1(s, "%"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);

				if(!j)
					return(BGBCP_WrapIntSuf(0, suf));				
				if(suf && (*suf=='U'))
				{
					return(BGBCC_CCXL_WrapIntSuf(((u64)i)%((u64)j), suf));
				}

				return(BGBCC_CCXL_WrapIntSuf(i%j, suf));
			}

//			if(!strcmp(s, "&"))
			if(!bgbcp_strcmp1(s, "&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i&j, suf));
			}
//			if(!strcmp(s, "|"))
			if(!bgbcp_strcmp1(s, "|"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i|j, suf));
			}
//			if(!strcmp(s, "^"))
			if(!bgbcp_strcmp1(s, "^"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i^j, suf));
			}
//			if(!strcmp(s, "\\"))
			if(!bgbcp_strcmp1(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i/j, suf));
			}

//			if(!strcmp(s, "&&"))
			if(!bgbcp_strcmp2(s, "&&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i&&j));
			}
//			if(!strcmp(s, "||"))
			if(!bgbcp_strcmp2(s, "||"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i||j));
			}

//			if(!strcmp(s, "<<"))
			if(!bgbcp_strcmp2(s, "<<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i<<j, suf));
			}
//			if(!strcmp(s, ">>"))
			if(!bgbcp_strcmp2(s, ">>"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);

				if(suf && (*suf=='U'))
				{
					return(BGBCC_CCXL_WrapIntSuf(((u64)i)>>j, suf));
				}

				return(BGBCC_CCXL_WrapIntSuf(i>>j, suf));
			}
		}

		if(BGBCC_CCXL_IsRealP(ctx, ln) && BGBCC_CCXL_IsRealP(ctx, rn))
		{
			f=BCCX_GetFloatCst(ln, &bgbcc_rcst_value, "value");
			g=BCCX_GetFloatCst(rn, &bgbcc_rcst_value, "value");

//			if(!strcmp(s, "=="))
			if(!bgbcp_strcmp2(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f==g));
			}
//			if(!strcmp(s, "!="))
			if(!bgbcp_strcmp2(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f!=g));
			}
//			if(!strcmp(s, "==="))
			if(!bgbcp_strcmp3(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f==g));
			}
//			if(!strcmp(s, "!=="))
			if(!bgbcp_strcmp3(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f!=g));
			}
//			if(!strcmp(s, "<="))
			if(!bgbcp_strcmp2(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f<=g));
			}
//			if(!strcmp(s, ">="))
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f>=g));
			}
//			if(!strcmp(s, "<"))
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f<g));
			}
//			if(!strcmp(s, ">"))
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f>g));
			}

			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

//			if(!strcmp(s, "+"))
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f+g, suf));
			}
//			if(!strcmp(s, "-"))
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f-g, suf));
			}
//			if(!strcmp(s, "*"))
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f*g, suf));
			}
//			if(!strcmp(s, "/"))
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f/g, suf));
			}
//			if(!strcmp(s, "\\"))
			if(!bgbcp_strcmp1(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f/g));
			}
		}

		if(	BGBCC_CCXL_IsSmallComplexP(ctx, ln) &&
			BGBCC_CCXL_IsSmallComplexP(ctx, rn))
		{
			if(BGBCC_CCXL_IsComplexP(ctx, ln))
			{
				f0=BCCX_GetFloatCst(ln, &bgbcc_rcst_real, "real");
				f1=BCCX_GetFloatCst(ln, &bgbcc_rcst_imag, "imag");
			}else
			{
				f0=BCCX_GetFloatCst(ln, &bgbcc_rcst_value, "value");
				f1=0;
			}

			if(BGBCC_CCXL_IsComplexP(ctx, rn))
			{
				g0=BCCX_GetFloatCst(rn, &bgbcc_rcst_real, "real");
				g1=BCCX_GetFloatCst(rn, &bgbcc_rcst_imag, "imag");
			}else
			{
				g0=BCCX_GetFloatCst(rn, &bgbcc_rcst_value, "value");
				g1=0;
			}
			
			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapComplex(f0+g0, f1+g1));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapComplex(f0-g0, f1-g1));
			}

			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapComplex(f0*g0-f1*g1, f1*g0+f0*g1));
			}

			if(!bgbcp_strcmp1(s, "/"))
			{
				f=g0*g0+g1*g1;
				if(f>0)
				{
					g2=g0/f;
					g3=-g1/f;
				}else
				{
					g2=0;
					g3=0;
				}
			
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapComplex(f0*g2-f1*g3, f1*g2+f0*g3));
			}
		}

		if(BGBCC_CCXL_IsVec2P(ctx, ln) && BGBCC_CCXL_IsVec2P(ctx, rn))
		{
			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

			f0=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x0, "value_x0");
			f1=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x1, "value_x1");
			g0=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x0, "value_x0");
			g1=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x1, "value_x1");

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec2(f0+g0, f1+g1, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec2(f0-g0, f1-g1, suf));
			}
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec2(f0*g0, f1*g1, suf));
			}
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec2(f0/g0, f1/g1, suf));
			}
		}

		if(	BGBCC_CCXL_IsSmallQuatP(ctx, ln) &&
			BGBCC_CCXL_IsSmallQuatP(ctx, rn))
		{
			suf="QF";
		
			if(BGBCC_CCXL_IsQuatP(ctx, ln))
			{
				f0=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x0, "value_x0");
				f1=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x1, "value_x1");
				f2=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x2, "value_x2");
				f3=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x3, "value_x3");
			}else if(BGBCC_CCXL_IsComplexP(ctx, ln))
			{
				f0=BCCX_GetFloatCst(ln, &bgbcc_rcst_imag, "imag");
				f1=0;
				f2=0;
				f3=BCCX_GetFloatCst(ln, &bgbcc_rcst_real, "real");
			}else
			{
				f0=0;
				f1=0;
				f2=0;
				f3=BCCX_GetFloatCst(ln, &bgbcc_rcst_value, "value");
			}

			if(BGBCC_CCXL_IsQuatP(ctx, rn))
			{
				g0=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x0, "value_x0");
				g1=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x1, "value_x1");
				g2=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x2, "value_x2");
				g3=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x3, "value_x3");
			}else if(BGBCC_CCXL_IsComplexP(ctx, rn))
			{
				g0=BCCX_GetFloatCst(rn, &bgbcc_rcst_imag, "imag");
				g1=0;
				g2=0;
				g3=BCCX_GetFloatCst(rn, &bgbcc_rcst_real, "real");
			}else
			{
				g0=0;
				g1=0;
				g2=0;
				g3=BCCX_GetFloatCst(rn, &bgbcc_rcst_value, "value");
			}

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec4(f0+g0, f1+g1, f2+g2, f3+g3, suf));
			}

			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec4(f0-g0, f1-g1, f2-g2, f3-g3, suf));
			}

			if(!bgbcp_strcmp1(s, "*"))
			{
				h0=f3*g0+f0*g3+f1*g2-f2*g1;
				h1=f3*g1+f1*g3+f2*g0-f0*g2;
				h2=f3*g2+f2*g3+f0*g1-f1*g0;
				h3=f3*g3-f0*g0-f1*g1-f2*g2;
			
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec4(
					h0, h1, h2, h3, suf));
			}

			if(!bgbcp_strcmp1(s, "/"))
			{
				h0=f3*g0+f0*g3+f1*g2-f2*g1;
				h1=f3*g1+f1*g3+f2*g0-f0*g2;
				h2=f3*g2+f2*g3+f0*g1-f1*g0;
				h3=f3*g3-f0*g0-f1*g1-f2*g2;
			
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec4(
					h0, h1, h2, h3, suf));
			}
		}

		if(BGBCC_CCXL_IsVec4P(ctx, ln) && BGBCC_CCXL_IsVec4P(ctx, rn))
		{
			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

			f0=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x0, "value_x0");
			f1=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x1, "value_x1");
			f2=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x2, "value_x2");
			f3=BCCX_GetFloatCst(ln, &bgbcc_rcst_value_x3, "value_x3");
			g0=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x0, "value_x0");
			g1=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x1, "value_x1");
			g2=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x2, "value_x2");
			g3=BCCX_GetFloatCst(rn, &bgbcc_rcst_value_x3, "value_x3");

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec4(f0+g0, f1+g1, f2+g2, f3+g3, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapVec4(f0-g0, f1-g1, f2-g2, f3-g3, suf));
			}

			if(!bgbcp_strcmp2(s, "QF"))
			{
				if(!bgbcp_strcmp1(s, "*"))
				{
					h3=f3*g3-f0*g0-f1*g1-f2*g2;
					h0=f3*g0+f0*g3+f1*g2-f2*g1;
					h1=f3*g1+f1*g3+f2*g0-f0*g2;
					h2=f3*g2+f2*g3+f0*g1-f1*g0;
				
					BCCX_CheckDeleteUnlinked(ln);
					BCCX_CheckDeleteUnlinked(rn);
					return(BGBCC_CCXL_WrapVec4(
						h0, h1, h2, h3, suf));
				}
			}else
			{
				if(!bgbcp_strcmp1(s, "*"))
				{
					BCCX_CheckDeleteUnlinked(ln);
					BCCX_CheckDeleteUnlinked(rn);
					return(BGBCC_CCXL_WrapVec4(
						f0*g0, f1*g1, f2*g2, f3*g3, suf));
				}
				if(!bgbcp_strcmp1(s, "/"))
				{
					BCCX_CheckDeleteUnlinked(ln);
					BCCX_CheckDeleteUnlinked(rn);
					return(BGBCC_CCXL_WrapVec4(
						f0/g0, f1/g1, f2/g2, f3/g3, suf));
				}
			}
		}

#if 1
		if(BGBCC_CCXL_IsSmallInt128P(ctx, ln) &&
			BGBCC_CCXL_IsSmallInt128P(ctx, rn))
		{
//			i=BCCX_GetIntCst(ln, &bgbcc_rcst_value, "value");
//			j=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");

			xi=BGBCC_CCXL_UnwrapInt128(ctx, ln);
			xj=BGBCC_CCXL_UnwrapInt128(ctx, rn);

			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

			if(!bgbcp_strcmp2(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpEq(xi, xj)));
			}
			if(!bgbcp_strcmp2(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpNe(xi, xj)));
			}
			if(!bgbcp_strcmp3(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpEq(xi, xj)));
			}
			if(!bgbcp_strcmp3(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpNe(xi, xj)));
			}
			if(!bgbcp_strcmp2(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpBe(xi, xj)));
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpLe(xi, xj)));
			}
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpAe(xi, xj)));
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpGe(xi, xj)));
			}
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpB(xi, xj)));
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpLt(xi, xj)));
			}
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				if(suf && (*suf=='U'))
					return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpA(xi, xj)));
				return(BGBCC_CCXL_WrapInt(BGBCC_Int128_CmpGt(xi, xj)));
			}

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xk=BGBCC_Int128_Add(xi, xj);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xk=BGBCC_Int128_Sub(xi, xj);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xk=BGBCC_Int128_Mul(xi, xj);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}

#if 0
//			if(!strcmp(s, "/"))
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);

				if(!j)
					return(BGBCP_WrapIntSuf(0, suf));				
				if(suf && (*suf=='U'))
				{
					return(BGBCC_CCXL_WrapIntSuf(((u64)i)/((u64)j), suf));
				}

				return(BGBCC_CCXL_WrapIntSuf(i/j, suf));
			}
//			if(!strcmp(s, "%"))
			if(!bgbcp_strcmp1(s, "%"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);

				if(!j)
					return(BGBCP_WrapIntSuf(0, suf));				
				if(suf && (*suf=='U'))
				{
					return(BGBCC_CCXL_WrapIntSuf(((u64)i)%((u64)j), suf));
				}

				return(BGBCC_CCXL_WrapIntSuf(i%j, suf));
			}
#endif

			if(!bgbcp_strcmp1(s, "&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xk=BGBCC_Int128_And(xi, xj);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp1(s, "|"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xk=BGBCC_Int128_Or(xi, xj);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp1(s, "^"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xk=BGBCC_Int128_Xor(xi, xj);
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}

#if 0
			if(!bgbcp_strcmp1(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i/j, suf));
			}
#endif

#if 0
			if(!bgbcp_strcmp2(s, "&&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i&&j));
			}
			if(!bgbcp_strcmp2(s, "||"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i||j));
			}
#endif

			if(!bgbcp_strcmp2(s, "<<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
//				return(BGBCC_CCXL_WrapIntSuf(i<<j, suf));
				xk=BGBCC_Int128_Shl(xi, (int)(xj.lo));
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
			if(!bgbcp_strcmp2(s, ">>"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);

				if(suf && (*suf=='U'))
				{
					xk=BGBCC_Int128_Shr(xi, (int)(xj.lo));
					return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
				}

				xk=BGBCC_Int128_Sar(xi, (int)(xj.lo));
				return(BGBCC_CCXL_WrapInt128Suf(xk, suf));
			}
		}
#endif

#if 1
		if(BGBCC_CCXL_IsReal128P(ctx, ln) && BGBCC_CCXL_IsReal128P(ctx, rn))
		{
//			f=BCCX_GetFloatCst(ln, &bgbcc_rcst_value, "value");
//			g=BCCX_GetFloatCst(rn, &bgbcc_rcst_value, "value");

			xf=BGBCC_CCXL_UnwrapFloat128(ctx, ln);
			xg=BGBCC_CCXL_UnwrapFloat128(ctx, rn);
			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

			if(!bgbcp_strcmp2(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpEq(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp2(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpNe(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp3(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpEq(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp3(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpNe(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp2(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpLe(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp2(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpGe(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp1(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpLt(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}
			if(!bgbcp_strcmp1(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				i=BGBCC_Float128_CmpGt(xf, xg);
				return(BGBCC_CCXL_WrapInt(i));
			}

			if(!bgbcp_strcmp1(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xh=BGBCC_Float128_Add(xf, xg);
				return(BGBCC_CCXL_WrapFloat128Suf(xh, suf));
			}
			if(!bgbcp_strcmp1(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xh=BGBCC_Float128_Sub(xf, xg);
				return(BGBCC_CCXL_WrapFloat128Suf(xh, suf));
			}
			if(!bgbcp_strcmp1(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xh=BGBCC_Float128_Mul(xf, xg);
				return(BGBCC_CCXL_WrapFloat128Suf(xh, suf));
			}
			if(!bgbcp_strcmp1(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				xh=BGBCC_Float128_Div(xf, xg);
				return(BGBCC_CCXL_WrapFloat128Suf(xh, suf));
			}

#if 0
			if(!bgbcp_strcmp1(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f/g));
			}
#endif
		}
#endif

#if 0
		if(BGBCC_CCXL_IsRealP(ctx, rn))
		{
			g=BCCX_GetFloatCst(rn, &bgbcc_rcst_value, "value");
			suf=BCCX_GetCst(rn, &bgbcc_rcst_tysuf, "tysuf");

			if(!bgbcp_strcmp1(s, "/") && (g!=0))
			{
				BCCX_CheckDeleteUnlinked(rn);
				rn=BGBCC_CCXL_WrapRealSuf(1.0/g, suf);
				x=BGBCP_BinaryExpr("*", ln, rn);
				return(x);
			}
		}
#endif

#if 1
		if(BGBCC_CCXL_IsIntP(ctx, rn))
		{
			i=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
			if(i==0)
			{
				if(	!bgbcp_strcmp1(s, "+") ||
					!bgbcp_strcmp1(s, "-") ||
					!bgbcp_strcmp1(s, "|") ||
					!bgbcp_strcmp1(s, "^") ||
					!bgbcp_strcmp1(s, "<<") ||
					!bgbcp_strcmp1(s, ">>")	)
				{
					return(ln);
				}
			}
		}
#endif

		x=BGBCP_BinaryExpr(s, ln, rn);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_then, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
			if(t)t=BGBCC_CCXL_ReduceForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		ln=BGBCC_CCXL_ReduceForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		sqn=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
		tkn=BCCX_GetIntCst(l, &bgbcc_rcst_tokens, "tokens");

		if(!BCCX_Child(l))
		{
			return(NULL);
		}

		if(!BCCX_Next(BCCX_Child(l)))
		{
			t=BGBCC_CCXL_ReduceForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_NewCst(&bgbcc_rcst_begin, "begin");
		c=BCCX_Child(l);
		while(c)
		{
			t=BGBCC_CCXL_ReduceForm(ctx, c, flag);
			BCCX_Add(x, t);
			c=BCCX_Next(c);
		}

		BCCX_SetIntCst(x, &bgbcc_rcst_tokens, "tokens", tkn);
		BCCX_SetIntCst(x, &bgbcc_rcst_index, "index", sqn);

		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof, "sizeof"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		if(!t)
		{
			t=BCCX_FetchCst(l, &bgbcc_rcst_tyexpr, "tyexpr");
			if(t)t=BCCX_FindTagCst(t, &bgbcc_rcst_type, "type");
		}

		if(ctx && t)
		{
			i=BGBCC_CCXL_TryGetSizeofType(ctx, t);
			if(i>0)return(BGBCC_CCXL_WrapInt(i));
			
			if(flag&1)
			{
				i=BGBCC_CCXL_GetMinMaxSizeofType(ctx, t,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCC_CCXL_WrapInt(i0));
				BGBCC_DBGBREAK
			}
		}
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_offsetof, "offsetof"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		if(!t)
		{
			t=BCCX_FetchCst(l, &bgbcc_rcst_tyexpr, "tyexpr");
			if(t)t=BCCX_FindTagCst(t, &bgbcc_rcst_type, "type");
		}

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		if(ctx && t && s)
		{
			i=BGBCC_CCXL_TryGetOffsetofType(ctx, t, s);
			if(i>=0)
				return(BGBCC_CCXL_WrapInt(i));

#if 0
			if(flag&1)
			{
				i=BGBCC_CCXL_GetMinMaxSizeofType(ctx, t,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCC_CCXL_WrapInt(i0));
				BGBCC_DBGBREAK
			}
#endif
		}

		if(ctx && t && v)
		{
			s=BGBCC_CCXL_FlattenDottedName(ctx, v);
			if(s)
			{
				i=BGBCC_CCXL_TryGetOffsetofType(ctx, t, s);
				if(i>=0)
					return(BGBCC_CCXL_WrapInt(i));
			}
		}
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof_expr, "sizeof_expr"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		if(BGBCC_CCXL_IsUnaryP(ctx, t, "&"))
		{
			if(ctx->arch_sizeof_ptr)
				return(BGBCC_CCXL_WrapInt(ctx->arch_sizeof_ptr));

			if(flag&1)
			{
				return(BGBCC_CCXL_WrapInt(8));
			}
		}
		
		i=BGBCC_CCXL_InferExpr(ctx, t, &bty);
		if(i>0)
		{
			i=BGBCC_CCXL_TypeGetLogicalSize(ctx, bty);
			if(i>0)
				return(BGBCC_CCXL_WrapInt(i));
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");

			i=BGBCC_CCXL_TryGetSizeofName(ctx, s);
			if(i>0)return(BGBCC_CCXL_WrapInt(i));
			
			if(flag&1)
			{
				i=BGBCC_CCXL_GetMinMaxSizeofName(ctx, s,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCC_CCXL_WrapInt(i0));
				BGBCC_DBGBREAK
			}
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_loadindex, "loadindex"))
		{
			t=BCCX_FetchCst(t, &bgbcc_rcst_array, "array");
			if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
			{
				s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");

				i=BGBCC_CCXL_GetMinMaxSizeofDerefName(ctx, s,
					&i1, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
				{
					if((i0==i1) || (flag&1))
						return(BGBCC_CCXL_WrapInt(i0));
				}
			}
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_string, "string"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_value, "value");
			if(s)
			{
				i=strlen(s);
				return(BGBCC_CCXL_WrapInt(i));
			}
		}

		if(flag&1)
			{ BGBCC_DBGBREAK }

		return(BCCX_Clone(l));
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		t=BGBCC_CCXL_ReduceExpr(ctx, t);

		if(BGBCC_CCXL_IsIntP(ctx, t))
		{
			s0=BGBCC_CCXL_VarTypeString(ctx, BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
			i=BCCX_GetIntCst(t, &bgbcc_rcst_value, "value");
			f=i;

			switch(*s0)
			{
			case 'a':	return(BGBCC_CCXL_WrapInt((sbyte)i));
			case 'b':	return(BGBCC_CCXL_WrapInt(i!=0));
			case 'c':	return(BGBCC_CCXL_WrapInt((sbyte)i));
			case 'd':	return(BGBCC_CCXL_WrapRealSuf(f, "D"));
			case 'f':	return(BGBCC_CCXL_WrapRealSuf(f, "F"));
			case 'h':	return(BGBCC_CCXL_WrapInt((byte)i));
			case 'i':	return(BGBCC_CCXL_WrapInt((s32)i));
			case 'j':	return(BGBCC_CCXL_WrapInt((u32)i));
			case 'l':	return(BGBCC_CCXL_WrapInt(i));
			case 'm':	return(BGBCC_CCXL_WrapInt(i));
			case 's':	return(BGBCC_CCXL_WrapInt((s16)i));
			case 't':	return(BGBCC_CCXL_WrapInt((u16)i));
			case 'w':	return(BGBCC_CCXL_WrapInt((u16)i));
			}
		}

		if(BGBCC_CCXL_IsRealP(ctx, t))
		{
			s0=BGBCC_CCXL_VarTypeString(ctx, BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
			f=BCCX_GetFloatCst(t, &bgbcc_rcst_value, "value");
			i=f;

			switch(*s0)
			{
			case 'a':	return(BGBCC_CCXL_WrapInt((sbyte)i));
			case 'b':	return(BGBCC_CCXL_WrapInt(i!=0));
			case 'c':	return(BGBCC_CCXL_WrapInt((sbyte)i));
			case 'd':	return(BGBCC_CCXL_WrapRealSuf(f, "D"));
			case 'f':	return(BGBCC_CCXL_WrapRealSuf(f, "F"));
			case 'h':	return(BGBCC_CCXL_WrapInt((byte)i));
			case 'i':	return(BGBCC_CCXL_WrapInt((s32)i));
			case 'j':	return(BGBCC_CCXL_WrapInt((u32)i));
			case 'l':	return(BGBCC_CCXL_WrapInt(i));
			case 'm':	return(BGBCC_CCXL_WrapInt(i));
			case 's':	return(BGBCC_CCXL_WrapInt((s16)i));
			case 't':	return(BGBCC_CCXL_WrapInt((u16)i));
			case 'w':	return(BGBCC_CCXL_WrapInt((u16)i));
			}
		}

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_list, "list"))
		{
			x=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
			s0=BGBCC_CCXL_VarTypeString(ctx, x);

			x=BGBCC_CCXL_TryReduceExprAsTypeSig(ctx, s0, t);
			if(x)return(x);
		}

//		i=BGBCC_CCXL_BoolExpr(ctx, t);
//		return(i);
		return(BCCX_Clone(l));
	}

	return(BCCX_Clone(l));
}

int BGBCC_CCXL_SetLine(BGBCC_TransState *ctx, BCCX_Node *l,
	char *fn, int ln)
{
	if(fn)BCCX_SetCst(l, &bgbcc_rcst_fn, "fn", fn);
	if(ln>0)BCCX_SetIntCst(l, &bgbcc_rcst_ln, "ln", ln);
	return(0);
}

BCCX_Node *BGBCC_CCXL_ReduceStatementForm(BGBCC_TransState *ctx,
	BCCX_Node *l, int flag)
{
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *fnam;
	double f, g;
	int sqn, tkn;
	int i, j, lnum;

	if(!l)return(l);

	fnam=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	lnum=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			t=BGBCC_CCXL_ReduceStatementForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_then, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			t=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
			if(t)t=BGBCC_CCXL_ReduceStatementForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		ln=BGBCC_CCXL_ReduceStatementForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceStatementForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		if(!BCCX_Child(l))
		{
			return(NULL);
		}

		if(!BCCX_Next(BCCX_Child(l)))
		{
			t=BGBCC_CCXL_ReduceStatementForm(ctx, BCCX_Child(l), flag);
			return(t);
		}

		sqn=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
		tkn=BCCX_GetIntCst(l, &bgbcc_rcst_tokens, "tokens");
		
		x=BCCX_NewCst(&bgbcc_rcst_begin, "begin");
		c=BCCX_Child(l);
		while(c)
		{
			t=BGBCC_CCXL_ReduceStatementForm(ctx, c, flag);
			BCCX_Add(x, t);
			c=BCCX_Next(c);
		}

		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);
		BCCX_SetIntCst(x, &bgbcc_rcst_tokens, "tokens", tkn);
		BCCX_SetIntCst(x, &bgbcc_rcst_index, "index", sqn);

		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)
		{
			return(NULL);
		}

		v=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_New2("while",
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));

		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)
		{
			v=BGBCC_CCXL_ReduceStatementForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);
			return(v);
		}

		v=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_New2("do_while",
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx, BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)
		{
			ln=BGBCC_CCXL_ReduceStatementForm(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_init, "init"), flag);
			return(ln);
		}

		ln=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_init, "init"), flag);
		rn=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_step, "step"), flag);

		v=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_body, "body"), flag);

		x=BCCX_New4("for",
			BCCX_NewCst1(&bgbcc_rcst_init, "init", ln),
			BCCX_NewCst1(&bgbcc_rcst_cond, "cond", t),
			BCCX_NewCst1(&bgbcc_rcst_step, "step", rn),
			BCCX_NewCst1(&bgbcc_rcst_body, "body", v));
		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	t=BGBCC_CCXL_ReduceForm(ctx, l, flag);
	return(t);
}

BCCX_Node *BGBCC_CCXL_ReduceExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceForm(ctx, l, 0);

	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}

BCCX_Node *BGBCC_CCXL_ReduceStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceStatementForm(ctx, l, 0);

	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}


BCCX_Node *BGBCC_CCXL_ReduceExprConst(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceForm(ctx, l, 1);
	
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}

BCCX_Node *BGBCC_CCXL_ReduceExprConst2(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceForm(ctx, l, 3);
	
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}

BCCX_Node *BGBCC_CCXL_TryReduceExprAsTypeSig(BGBCC_TransState *ctx,
	char *sig, BCCX_Node *l)
{
	BCCX_Node *an[64];
	double afv[64];
	BCCX_Node *c, *t, *x;
	char *vsfx;
	ccxl_type bty;
	int i, j, k, na, vty;
	
	BGBCC_CCXL_TypeFromSig(ctx, &bty, sig);

	if(BGBCC_CCXL_TypeVecP(ctx, bty))
	{
		vsfx=NULL;
		switch(bty.val)
		{
			case CCXL_TY_VEC2F:		vsfx="V2F";		break;
			case CCXL_TY_VEC3F:		vsfx="V3F";		break;
			case CCXL_TY_VEC4F:		vsfx="V4F";		break;
			case CCXL_TY_QUATF:		vsfx="QF";		break;
			case CCXL_TY_VEC2D:		vsfx="V2D";		break;
			case CCXL_TY_FCOMPLEX:	vsfx="FCPX";	break;
			case CCXL_TY_DCOMPLEX:	vsfx="DCPX";	break;
			case CCXL_TY_VEC2SI:	vsfx="V2SI";	break;
			case CCXL_TY_VEC2UI:	vsfx="V2UI";	break;
			case CCXL_TY_VEC4SW:	vsfx="V4SW";	break;
			case CCXL_TY_VEC4UW:	vsfx="V4UW";	break;
			case CCXL_TY_VEC4SI:	vsfx="V4SI";	break;
			case CCXL_TY_VEC4UI:	vsfx="V4UI";	break;
		}
	
		na=0;
	
		if(BCCX_TagIsCstP(l, &bgbcc_rcst_list, "list"))
		{
			c=BCCX_Child(l);
			while(c)
			{
				t=BGBCC_CCXL_ReduceExpr(ctx, c);
				an[na++]=t;
				c=BCCX_Next(c);
			}
			
			for(i=0; i<na; i++)
			{
				if(!BGBCC_CCXL_IsRealP(ctx, an[i]))
					break;
				afv[i]=BCCX_GetFloatCst(an[i], &bgbcc_rcst_value, "value");
			}
			
			if((i>=na) && (na>1) && (na<=4) && vsfx)
			{
				if(na==2)
				{
					x=BGBCC_CCXL_WrapVec2(
						afv[0], afv[1], vsfx);
				}else
				{
					x=BGBCC_CCXL_WrapVec4(
						afv[0], afv[1], afv[2], afv[3], vsfx);
				}
				
				return(x);
			}
		}
	}
	
	return(NULL);
}
