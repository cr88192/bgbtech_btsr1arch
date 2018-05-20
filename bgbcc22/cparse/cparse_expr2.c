#include <bgbccc.h>

//Lit:		Literal values/builtin expressions
//PE:		(<expr>) <expr>[<expr>] <expr>.<name>
//IncDec:	++<name> --<name> + - ! ~ ... <name>++ <name>-- <expr>(<args>)       
//E:		<expr>**<expr>
//MD:		* / % \ &	*. /. %. \. &.
//AS:		+ - | ^		+. -. |. ^.
//SHLR:		<< >>
//RCmp:		< > <= >= == === != <=> <<== >>==
//Lop:		&&
//Lop2:		|| ^^
//TCond:	<expr>?<expr>:<expr>
//Attr:		:= :!= :< :> :<= :>= :<< :>> :<<= :>>=
//Equals:	= += -= *= /= \= %= &= |= ^= >>= <<=

BGBCP_ParseItem *bgbcp_exprs=NULL;

int BGBCP_AddExpression(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_exprs;
	bgbcp_exprs=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseExpressionName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_exprs;
	while(cur)
	{
		if(!bgbcp_strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BGBCP_Number(BGBCP_ParseState *ctx, char *str)
{
	BCCX_Node *n;
	char *s;
	s64 li, lj;
	int i;

	s=str;
//	while(*s && (*s!='.') && (*s!='e') && (*s!='E')) s++;
	while(*s && (*s!='.')) s++;

	if(!*s)
	{
//		li=bgbcc_atoi(str);

		bgbcc_atoxl(str, (u64 *)(&li), (u64 *)(&lj));
		
		if((lj!=0) && (lj!=(-1)))
		{
			n=BCCX_NewCst(&bgbcc_rcst_int128, "int128");
			BCCX_SetIntCst(n, &bgbcc_rcst_value_lo, "value_lo", li);
			BCCX_SetIntCst(n, &bgbcc_rcst_value_hi, "value_hi", lj);
			return(n);
		}

		n=BCCX_NewCst(&bgbcc_rcst_int, "int");
//		BCCX_SetCst(n, &bgbcc_rcst_value, "value", str);
		BCCX_SetIntCst(n, &bgbcc_rcst_value, "value", li);
		return(n);
	}

	n=BCCX_NewCst(&bgbcc_rcst_real, "real");
//	BCCX_SetCst(n, &bgbcc_rcst_value, "value", str);
	BCCX_SetFloatCst(n, &bgbcc_rcst_value, "value", BGBCC_ParseNumber(str));
	return(n);
}

BCCX_Node *BGBCP_ExpressionLit(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	char *suf;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;
	int i;

	s=*str;

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
	{
		s=BGBCP_Token(s, b, &ty);	//(
		n=BGBCP_Expression2(ctx, &s);
		s=BGBCP_Token(s, b, &ty);	//)

		if(bgbcp_strcmp1(b, ")"))
		{
			BGBCP_ErrorCtx(ctx, s, "parser, token '%s'\n", b);
//			printf("parser, token '%s'\n", b);
		}

//		n=BGBCP_FunArgs(ctx, &s);
//		if(CDR(n))n=CONS(SYM("begin"), n);

		*str=s;
		return(n);
	}

	s=*str;

	s=BGBCP_Token(s, b, &ty);
	if(!s)return(NULL);

	if(!bgbcp_strcmp1(b, "{") && (ty==BTK_BRACE))
	{
		n1=BGBCP_FunArgs(ctx, &s);
		n=BCCX_NewCst1(&bgbcc_rcst_list, "list", n1);
		*str=s;
		return(n);
	}

	if(ty==BTK_NAME)
	{
		BGBCP_Token(s, b2, &ty2);

		if((b[0]=='_') && !bgbcp_strcmp10(b, "__offsetof"))
		{
			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, "("))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

//			n1=BGBCP_DefType(ctx, &s);
//			n2=BGBCP_DefName2(ctx, &s);
//			n1=BGBCP_ExpandDefinition(n1, n2);
//			n1=CDR(n1);

			n1=BGBCP_ArgDefinition(ctx, &s);

			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, ","))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

			s=BGBCP_Token(s, b, &ty);
//			n2=BCCX_NewCst(&bgbcc_rcst_symbol, "symbol"); BCCX_Set(n2, "name", b2);

			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, ")"))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

			n=BCCX_NewCst1(&bgbcc_rcst_offsetof, "offsetof",
				BCCX_NewCst1V(&bgbcc_rcst_tyexpr, "tyexpr", n1));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);

			*str=s;
			return(n);
		}

		if(!bgbcp_strcmp3(b, "new") && (ctx->lang!=BGBCC_LANG_C))
		{
			n=BCCX_NewCst(&bgbcc_rcst_new, "new");

			n1=BGBCP_DefType(ctx, &s);

			if(!n1)
			{
				BGBCP_Error(s, "invalid type for new\n");
				*str=NULL;
				return(NULL);
			}

			BCCX_Add(n, n1);

			BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, "("))
			{
				s=BGBCP_Token(s, b, &ty); //'('
				n1=BGBCP_FunArgs(ctx, &s);
				BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", n1));
			}

			*str=s;
			return(n);
		}

		if(!bgbcp_strcmp1(b, "L") && (ty2==BTK_STRING))
		{
			n=BGBCP_ExpressionLit(ctx, &s);
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", "L");
			*str=s;
			return(n);
		}

		n=BGBCP_ParseExpressionName(ctx, b, &s);
		if(n)
		{
			*str=s;
			return(n);
		}

		n=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
		*str=s;
		return(n);
	}

	BGBCP_Token(s, b2, &ty2);

	if(ty==BTK_NUMBER)
	{
		BGBCP_Token(s, b2, &ty2);
		if((!bgbcp_strcmp1(b2, "i") || !bgbcp_strcmp2(b2, "iF") ||
			!bgbcp_strcmp2(b2, "if")) && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			if(!bgbcp_strcmp1(b2, "i"))
			{
				n=BCCX_NewCst(&bgbcc_rcst_complex, "complex");
				BCCX_SetCst(n, &bgbcc_rcst_imag, "imag", b);
			}
			*str=s;
			return(n);

		}

		suf=NULL;
		if(!bgbcp_strcmp1(b2, "F") || !bgbcp_strcmp1(b2, "f"))suf="F";
		if(!bgbcp_strcmp1(b2, "L") || !bgbcp_strcmp1(b2, "l"))suf="L";
		if(!bgbcp_strcmp2(b2, "LL") || !bgbcp_strcmp2(b2, "ll"))suf="LL";
		if(!bgbcp_strcmp1(b2, "U") || !bgbcp_strcmp1(b2, "u"))suf="U";
		if(!bgbcp_strcmp2(b2, "UL") || !bgbcp_strcmp2(b2, "ul"))suf="UL";
		if(!bgbcp_strcmp3(b2, "ULL") || !bgbcp_strcmp3(b2, "ull"))suf="ULL";

		if(!bgbcp_strcmp2(b2, "XL") || !bgbcp_strcmp2(b2, "xl"))suf="XL";
		if(!bgbcp_strcmp3(b2, "UXL") || !bgbcp_strcmp3(b2, "uXl"))suf="UXL";

		if(suf && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			*str=s;
			n=BGBCP_Number(ctx, b);
			BCCX_SetCst(n, &bgbcc_rcst_tysuf, "tysuf", suf);
			return(n);
		}

		n=BGBCP_Number(ctx, b);
		*str=s;
		return(n);
	}

	if(ty==BTK_STRING)
	{
		BGBCP_Token(s, b2, &ty2);
		while(ty2==BTK_STRING)
		{
			s=BGBCP_Token(s, b2, &ty2);
			strcat(b, b2);
			BGBCP_Token(s, b2, &ty2);
		}

		n=BCCX_NewCst(&bgbcc_rcst_string, "string");
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);

		*str=s;
		return(n);
	}

	if(ty==BTK_CHARSTRING)
	{
		BGBCP_Token(s, b2, &ty2);
		while(ty2==BTK_CHARSTRING)
		{
			s=BGBCP_Token(s, b2, &ty2);
			strcat(b, b2);
			BGBCP_Token(s, b2, &ty2);
		}

		n=BCCX_NewCst(&bgbcc_rcst_charstring, "charstring");
		BCCX_SetCst(n, &bgbcc_rcst_value, "value", b);

		*str=s;
		return(n);
	}

	BGBCP_Error(s, "parse error before '%s'\n", b);

	*str=NULL;
	return(NULL);
}

BCCX_Node *BGBCP_ExpressionPostfix(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s0;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;
	int i;

	s=*str;

	n=BGBCP_ExpressionLit(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(!bgbcp_strcmp1(b, "[") && (ty==BTK_BRACE))
		{
			s=BGBCP_Token(s, b, &ty); //[
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty); //]
			n=BCCX_NewCst2(&bgbcc_rcst_getindex, "getindex",
				BCCX_NewCst1V(&bgbcc_rcst_array, "array", n),
				BCCX_NewCst1V(&bgbcc_rcst_index, "index", n1));
			continue;
		}

		if(!bgbcp_strcmp1(b, ".") && (ty==BTK_SEPERATOR))
		{
			s=BGBCP_Token(s, b, &ty); //.
			s=BGBCP_Token(s, b, &ty); //name
			n=BCCX_NewCst1(&bgbcc_rcst_objref, "objref",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
			continue;
		}

		if(!bgbcp_strcmp2(b, "->") && (ty==BTK_OPERATOR))
		{
			s=BGBCP_Token(s, b, &ty); //.
			s=BGBCP_Token(s, b, &ty); //name
			n=BCCX_NewCst1(&bgbcc_rcst_objref, "objref",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
			continue;
		}

#if 1
		if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
		{
			if(BCCX_TagIsP(n, "objref"))
			{
				s=BGBCP_Token(s, b, &ty); //'('
				n1=BGBCP_FunArgs(ctx, &s);
//				n=CONS3(SYM("methodcall"),
//					CADR(n), CADDR(n), n1);

				BCCX_SetTag(n, "methodcall");
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", n1));
				continue;
			}

			s=BGBCP_Token(s, b, &ty); //'('
			n1=BGBCP_FunArgs(ctx, &s);
			if(!s)
			{
				*str=s;
				return(n);
			}

//			n=CONS2(SYM("funcall"), n, n1);
//			n->tag=bgbcc_strdup("funcall");
//			BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_args, "args", n1));

			if(BCCX_TagIsP(n, "ref"))
			{
				s0=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
				n=BCCX_NewCst1(&bgbcc_rcst_funcall, "funcall",
					BCCX_NewCst1V(&bgbcc_rcst_args, "args", n1));
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);
				continue;
			}

			n=BCCX_NewCst2(&bgbcc_rcst_funcall, "funcall",
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n),
				BCCX_NewCst1V(&bgbcc_rcst_args, "args", n1));
			continue;
		}

		if(!bgbcp_strcmp2(b, "++") && (ty==BTK_OPERATOR))
		{
			s=BGBCP_Token(s, b, &ty);
			n=BCCX_NewCst1(&bgbcc_rcst_postinc, "postinc",
				BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n));
			continue;
		}
		if(!bgbcp_strcmp2(b, "--") && (ty==BTK_OPERATOR))
		{
			s=BGBCP_Token(s, b, &ty);
			n=BCCX_NewCst1(&bgbcc_rcst_postdec, "postdec",
				BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n));
			continue;
		}
#endif

		break;
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionUnary(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	t=BGBCP_Token(s, b, &ty);
	BGBCP_Token(t, b2, &ty2);

	if(!bgbcp_strcmp2(b, "++") && (ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionUnary(ctx, &s);
		n=BCCX_NewCst1(&bgbcc_rcst_preinc, "preinc",
			BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n1));
		*str=s;
		return(n);
	}
	if(!bgbcp_strcmp2(b, "--") && (ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionUnary(ctx, &s);
		n=BCCX_NewCst1(&bgbcc_rcst_predec, "predec",
			BCCX_NewCst1V(&bgbcc_rcst_expr, "expr", n1));
		*str=s;
		return(n);
	}

	if((!bgbcp_strcmp1(b, "+") ||	!bgbcp_strcmp1(b, "-") ||
		!bgbcp_strcmp1(b, "!") ||	!bgbcp_strcmp1(b, "~") ||
		!bgbcp_strcmp1(b, "&") ||	!bgbcp_strcmp1(b, "*")) &&
			(ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionCast(ctx, &s);
		n1=BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1);
		n=BCCX_NewCst1(&bgbcc_rcst_unary, "unary", n1);
		BCCX_SetCst(n, &bgbcc_rcst_op, "op", b);
		*str=s;
		return(n);
	}

#if 1
	if(!bgbcp_strcmp(b, "sizeof"))
	{
		s=BGBCP_Token(s, b, &ty);

		if(!bgbcp_strcmp1(b2, "(") && (ty2==BTK_BRACE))
		{
			s=BGBCP_Token(s, b2, &ty2);

//			n=BGBCP_DefType(ctx, &s);
//			n1=BGBCP_DefName2(ctx, &s);
//			n=BGBCP_ExpandDefinition(n, n1);
//			n=CDR(n);

			n=BGBCP_ArgDefinition2(ctx, &s);
			if(n)
			{
				n=BCCX_NewCst1(&bgbcc_rcst_sizeof, "sizeof",
					BCCX_NewCst1(&bgbcc_rcst_tyexpr, "tyexpr", n));
			}else
			{
				n1=BGBCP_Expression(ctx, &s);
				n=BCCX_NewCst1(&bgbcc_rcst_sizeof_expr, "sizeof_expr", 
					BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
			}

			s=BGBCP_Token(s, b2, &ty2);
			if(bgbcp_strcmp1(b2, ")"))
				BGBCP_Error(s,
					"sizeof token '%s'\n", b2);

			*str=s;
			return(n);
		}

		n=BGBCP_ArgDefinition2(ctx, &s);
		if(n)
		{
			n=BCCX_NewCst1(&bgbcc_rcst_sizeof, "sizeof",
				BCCX_NewCst1(&bgbcc_rcst_tyexpr, "tyexpr", n));
		}else
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_NewCst1(&bgbcc_rcst_sizeof_expr, "sizeof_expr",
				BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
		}

//		n1=BGBCP_Expression(ctx, &s);
//		n=BCCX_NewCst1(&bgbcc_rcst_sizeof_expr, "sizeof_expr", 
//			BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
		*str=s;
		return(n);
	}
#endif

	s=*str;
	n=BGBCP_ExpressionPostfix(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionCast(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;

	s=*str;
	s1=BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
	{
//		n=BGBCP_DefType(ctx, &s1);
		n=BGBCP_ArgDefinition2(ctx, &s1);
		if(n)
		{
//			n1=BGBCP_DefName2(ctx, &s1);
//			n=BGBCP_ExpandDefinition(n, n1);
//			n=CDR(n);

			n2=BCCX_FindTag(n, "type");
			n2=BCCX_Clone(n2);

			s=s1;
			s=BGBCP_Token(s, b, &ty);
			if(bgbcp_strcmp1(b, ")"))
			{
//				printf("parser, cast token '%s'\n", b);
				BGBCP_ErrorCtx(ctx, s, "parser, cast token '%s'\n", b);
			}

			n1=BGBCP_ExpressionCast(ctx, &s);

			BCCX_CheckDeleteUnlinked(n);

			n=BCCX_NewCst2(&bgbcc_rcst_cast, "cast", n2,
				BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));

			*str=s;
			return(n);
		}
	}

	n=BGBCP_ExpressionUnary(ctx, str);
	return(n);
}

BCCX_Node *BGBCP_BinaryExpr(char *op, BCCX_Node *l, BCCX_Node *r)
{
	BCCX_Node *n, *n1, *n2;

	n1=BCCX_NewCst1V(&bgbcc_rcst_left, "left", l);
	n2=BCCX_NewCst1V(&bgbcc_rcst_right, "right", r);
	n=BCCX_NewCst2(&bgbcc_rcst_binary, "binary", n1, n2);
	BCCX_SetCst(n, &bgbcc_rcst_op, "op", op);

	return(n);
}

BCCX_Node *BGBCP_UnaryExpr(char *op, BCCX_Node *v)
{
	BCCX_Node *n, *n1, *n2;

	n1=BCCX_NewCst1V(&bgbcc_rcst_value, "value", v);
	n=BCCX_NewCst1(&bgbcc_rcst_unary, "unary", n1);
	BCCX_SetCst(n, &bgbcc_rcst_op, "op", op);

	return(n);
}

BCCX_Node *BGBCP_IfExpr(BCCX_Node *cond, BCCX_Node *ln, BCCX_Node *rn)
{
	BCCX_Node *n, *n1, *n2, *n3;

	if(ln && rn)
	{
		n1=BCCX_NewCst1V(&bgbcc_rcst_then, "then", ln);
		n2=BCCX_NewCst1V(&bgbcc_rcst_else, "else", rn);
		n3=BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", cond);
		n=BCCX_NewCst3(&bgbcc_rcst_if, "if", n3, n1, n2);
	}else if(ln)
	{
		n1=BCCX_NewCst1V(&bgbcc_rcst_then, "then", ln);
		n3=BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", cond);
		n=BCCX_NewCst2(&bgbcc_rcst_if, "if", n3, n1);
	}else if(rn)
	{
		n1=BCCX_NewCst1V(&bgbcc_rcst_then, "then", rn);
		n2=BGBCP_UnaryExpr("!", cond);
		n3=BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n2);
		n=BCCX_NewCst2(&bgbcc_rcst_if, "if", n3, n1);
	}else
	{
		n=NULL;
	}

	return(n);
}

BCCX_Node *BGBCP_ExpressionExp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionCast(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "`") && bgbcp_strcmp2(b, "``") &&
			bgbcp_strcmp1(b, "&") && bgbcp_strcmp2(b, "&`") &&
			bgbcp_strcmp2(b, "`&") && bgbcp_strcmp1(b, "|") &&
			bgbcp_strcmp2(b, "|`") && bgbcp_strcmp2(b, "`|") &&
			bgbcp_strcmp1(b, "^") && bgbcp_strcmp2(b, "^`") &&
			bgbcp_strcmp2(b, "`^")	)
				break;

		s=BGBCP_Token(s, b, &ty);
		if(!bgbcp_strcmp1(b, "`"))strcpy(b, "**");

		n1=BGBCP_ExpressionCast(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionMD(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionExp(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "*") && bgbcp_strcmp1(b, "/") &&
			bgbcp_strcmp1(b, "%") && bgbcp_strcmp1(b, "*`") &&
			bgbcp_strcmp2(b, "/`") && bgbcp_strcmp2(b, "%`") &&
			bgbcp_strcmp2(b, "`*") && bgbcp_strcmp2(b, "`/") &&
			bgbcp_strcmp2(b, "`%") && bgbcp_strcmp1(b, "~") &&
			bgbcp_strcmp2(b, "~`") && bgbcp_strcmp2(b, "`~"))
				break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionExp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionAS(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionMD(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "+") && bgbcp_strcmp1(b, "-") &&
			bgbcp_strcmp2(b, "+`") && bgbcp_strcmp2(b, "-`") &&
			bgbcp_strcmp2(b, "`+") && bgbcp_strcmp2(b, "`-"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionMD(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionSHLR(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionAS(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;
		if(bgbcp_strcmp2(b, "<<") && bgbcp_strcmp2(b, ">>"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionAS(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionRCmp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionSHLR(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;

		if(bgbcp_strcmp1(b, "<") && bgbcp_strcmp1(b, ">") &&
			bgbcp_strcmp2(b, "<=") && bgbcp_strcmp2(b, ">="))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionSHLR(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionECmp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionRCmp(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;
		if(bgbcp_strcmp2(b, "==") && bgbcp_strcmp2(b, "!="))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionRCmp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitAnd(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionECmp(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp1(b, "&"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionECmp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitXor(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitAnd(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp1(b, "^"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitAnd(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitOr(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitXor(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp1(b, "|"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitXor(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLop(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitOr(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)break;
		if(bgbcp_strcmp2(b, "&&"))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitOr(ctx, &s);

		n=BGBCP_BinaryExpr("&&", n, n1);
//		n=BCCX_NewCst2(&bgbcc_rcst_l_and, "l_and",
//			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
//			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));

#if 0
		if(BCCX_TagIsP(n, "and"))
		{
			BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
			continue;
		}

		n=BCCX_NewCst2(&bgbcc_rcst_and, "and",
			BCCX_NewCst1V(&bgbcc_rcst_value, "value", n),
			BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
#endif
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLop2(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionLop(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;
		if(bgbcp_strcmp2(b, "||"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionLop(ctx, &s);

		n=BGBCP_BinaryExpr("||", n, n1);
//		n=BCCX_NewCst2(&bgbcc_rcst_l_or, "l_or",
//			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
//			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));

#if 0
		if(BCCX_TagIsP(n, "or"))
		{
			BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
			continue;
		}

		n=BCCX_NewCst2(&bgbcc_rcst_or, "or",
			BCCX_NewCst1(&bgbcc_rcst_value, "value", n),
			BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
#endif
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionTCond(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n1=BGBCP_ExpressionLop2(ctx, &s);
	BGBCP_Token(s, b, &ty);
	if(bgbcp_strcmp1(b, "?") || (ty!=BTK_OPERATOR))
	{
		*str=s;
		return(n1);
	}

//	s=BGBCP_Token(s, b, &ty); //?
	s=BGBCP_EatExpectToken(ctx, s, "?");
//	n2=BGBCP_ExpressionLop2(ctx, &s);
	n2=BGBCP_Expression(ctx, &s);
//	s=BGBCP_Token(s, b, &ty); //:
	s=BGBCP_EatExpectToken(ctx, s, ":");
//	n3=BGBCP_ExpressionTCond(ctx, &s);
	n3=BGBCP_Expression(ctx, &s);

	n=BCCX_NewCst3(&bgbcc_rcst_if, "if",
		BCCX_NewCst1V(&bgbcc_rcst_cond, "cond", n1),
		BCCX_NewCst1V(&bgbcc_rcst_then, "then", n2),
		BCCX_NewCst1V(&bgbcc_rcst_else, "else", n3));

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionEquals(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionTCond(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;

		if(	bgbcp_strcmp1(b, "=") &&	bgbcp_strcmp2(b, "+=") &&
			bgbcp_strcmp2(b, "-=") &&	bgbcp_strcmp2(b, "*=") &&
			bgbcp_strcmp2(b, "/=") &&	bgbcp_strcmp2(b, "%=")  &&
			bgbcp_strcmp2(b, "&=") &&	bgbcp_strcmp2(b, "|=") &&
			bgbcp_strcmp2(b, "^=") &&	bgbcp_strcmp3(b, "<<=") &&
			bgbcp_strcmp3(b, ">>="))
				break;

		t=NULL;
		if(!bgbcp_strcmp2(b, "+="))t="+";
		if(!bgbcp_strcmp2(b, "-="))t="-";
		if(!bgbcp_strcmp2(b, "*="))t="*";
		if(!bgbcp_strcmp2(b, "/="))t="/";
		if(!bgbcp_strcmp2(b, "%="))t="%";
		if(!bgbcp_strcmp2(b, "&="))t="&";
		if(!bgbcp_strcmp2(b, "|="))t="|";
		if(!bgbcp_strcmp2(b, "^="))t="^";
		if(!bgbcp_strcmp3(b, "<<="))t="<<";
		if(!bgbcp_strcmp3(b, ">>="))t=">>";

		s=BGBCP_Token(s, b, &ty);

#if 0
		if(BCCX_TagIsP(n, "ref"))
		{
			n1=BGBCP_ExpressionEquals(ctx, &s);
			n2=n;

			n=BCCX_NewCst1(&bgbcc_rcst_assign, "assign",
				BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
			if(t)BCCX_SetCst(n, &bgbcc_rcst_op, "op", t);

			t=BCCX_GetCst(n2, &bgbcc_rcst_name, "name");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", t);

			continue;
		}

		if(BCCX_TagIsP(n, "objref") || BCCX_TagIsP(n, "getindex"))
		{
			if(BCCX_TagIsP(n, "objref"))
			{
				n1=BGBCP_ExpressionEquals(ctx, &s);
				if(t)n1=BGBCP_BinaryExpr(t,
					BCCX_Clone(n), n1);

				n2=BCCX_Find(n, "value");
				BCCX_SetTag(n2, "object");

				BCCX_SetTag(n, "objset");
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));
				continue;
			}
			if(BCCX_TagIsP(n, "getindex"))
			{
				n1=BGBCP_ExpressionEquals(ctx, &s);
				if(t)n1=BGBCP_BinaryExpr(t,
					BCCX_Clone(n), n1);
				BCCX_SetTag(n, "setindex");
				BCCX_Add(n, BCCX_NewCst1(&bgbcc_rcst_value, "value", n1));
				continue;
			}
		}
#endif

		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_NewCst2(&bgbcc_rcst_assign, "assign",
			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));
		if(t)BCCX_SetCst(n, &bgbcc_rcst_op, "op", t);

		continue;
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionComma(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(ty!=BTK_SEPERATOR)break;
		if(bgbcp_strcmp1(b, ","))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_NewCst2(&bgbcc_rcst_comma, "comma",
			BCCX_NewCst1V(&bgbcc_rcst_left, "left", n),
			BCCX_NewCst1V(&bgbcc_rcst_right, "right", n1));
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_Expression(BGBCP_ParseState *ctx, char **str)
{
	return(BGBCP_ExpressionEquals(ctx, str));
}

BCCX_Node *BGBCP_Expression2(BGBCP_ParseState *ctx, char **str)
{
	return(BGBCP_ExpressionComma(ctx, str));
}

#if 0
// BCCX_Node *BGBCP_Expression3(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	int ty, ty2;
	char *s;
	BCCX_Node *n;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, ":") && (ty==BTK_SEPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		if(BS1_SYMBOLP(n))n=KEYSYM(TOSYM(n));
			else n=LIST2S("colon", n);
	}

	*str=s;
	return(n);
}
#endif

