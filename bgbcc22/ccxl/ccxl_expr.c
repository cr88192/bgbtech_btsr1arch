#include <bgbccc.h>


char *BGBCC_CCXL_CIfy(char *s)
{
	static char buf[1024];
	char *t;
	int i;

	t=buf;
	*t++='\"';

	while(*s)
	{
		i=BGBCP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(buf);
}

void BGBCC_CCXL_CompileAssign(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *v;
	char *s;
	int i, j;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_PopStore(ctx, s);
		return;
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "*"))
	{
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_CompileExpr(ctx, v);
		BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_CompileExpr(ctx, v);
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StackStoreSlot(ctx, s);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_getindex, "getindex") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_vector_ref, "vector-ref"))
	{
		ct=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		cv=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");
		BGBCC_CCXL_CompileExpr(ctx, ct);
		BGBCC_CCXL_CompileExpr(ctx, cv);
		BGBCC_CCXL_StackStoreIndex(ctx);
		return;
	}

	BGBCC_CCXL_Error(ctx, "Bad lvalue in expression\n");
}

void BGBCC_CCXL_CompileExprList(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
		cur=BCCX_Next(cur);
	}
}

void BGBCC_CCXL_CompileExprListReverse(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)
	{
//		BGBCC_CCXL_CompileExpr(ctx, stk[i]);
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
		BGBCC_CCXL_CompileExpr(ctx, n);
	}
}

void BGBCC_CCXL_CompileArgsList(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)
	{
//		BGBCC_CCXL_CompileExpr(ctx, stk[i]);
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
		BGBCC_CCXL_CompileExpr(ctx, n);
	}
}

void BGBCC_CCXL_CompileFuncall(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BGBCC_CCXL_CompileFuncallI(ctx, l, NULL, 0);
}

void BGBCC_CCXL_CompileFuncallStmt(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BGBCC_CCXL_CompileFuncallI(ctx, l, NULL, 1);
}

void BGBCC_CCXL_CompileFuncallStore(BGBCC_TransState *ctx,
	BCCX_Node *l, char *dname)
{
	BGBCC_CCXL_CompileFuncallI(ctx, l, dname, 0);
}

void BGBCC_CCXL_CompileFuncallI(BGBCC_TransState *ctx,
	BCCX_Node *l, char *dname, int flag)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	if(s0)
	{
		if(!strcmp(s0, "__BGBCC_DBG"))
		{
			printf("BGBCC Debug\n");
		}

		if(!strcmp(s0, "__builtin_va_end"))
		{
			BGBCC_CCXL_CompileExprListReverse(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_StackVaEnd(ctx);
			if(!(flag&1) && !dname)
				BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		if(!strcmp(s0, "__builtin_va_start"))
		{
			BGBCC_CCXL_CompileExprListReverse(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_StackVaStart(ctx);
			if(!(flag&1) && !dname)
				BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		if(!strcmp(s0, "__builtin_va_arg"))
		{
			BGBCC_CCXL_CompileExprListReverse(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_StackVaArg(ctx);

			if(dname)
			{
				BGBCC_CCXL_PopStore(ctx, dname);
			}

//			BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileExprListReverse(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
//		BGBCC_CCXL_StackCallName(ctx, s0, flag);
		BGBCC_CCXL_StackCallName2(ctx, s0, dname, flag);
		return;
	}


	c=BCCX_FetchCst(l, &bgbcc_rcst_func, "func");
	if(!c)c=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

	if(!c)
	{
		BGBCC_CCXL_Error(ctx, "funcall missing function\n");
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);
	BGBCC_CCXL_CompileExprListReverse(ctx,
		BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
	BGBCC_CCXL_CompileExpr(ctx, c);
//	BGBCC_CCXL_StackPopCall(ctx, flag);
	BGBCC_CCXL_StackPopCall2(ctx, dname, flag);
	return;
}


void BGBCC_CCXL_CompileMethodcall(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	BGBCC_CCXL_PushMark(ctx);
	BGBCC_CCXL_CompileExprListReverse(ctx,
		BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));

	BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	BGBCC_CCXL_StackLoadSlot(ctx, s0);

	BGBCC_CCXL_StackPopCall(ctx, 0);
	return;
}

void BGBCC_CCXL_CompileNewBasic(BGBCC_TransState *ctx,
	BCCX_Node *type, BCCX_Node *args)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *obj;
//	ccxl_register treg;
	char *sig_size_t;
	char *s0;
	int i;

//	sig_size_t="l";		//Lazy
	sig_size_t="m";		//Lazy

	BGBCC_CCXL_PushMark(ctx);
	s0=BGBCC_CCXL_VarTypeString(ctx, type);
	obj=BGBCC_CCXL_GetStructureForSig(ctx, s0);

	if(!obj)
	{
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackSizeofSig(ctx, s0);
		BGBCC_CCXL_StackCastSig(ctx, sig_size_t);
		BGBCC_CCXL_StackCallName2(ctx, "__operator_new", NULL, 0);
		return;
	}

	if(obj)
	{
		sprintf(tb, "%s/%s", obj->decl->name, "__operator_new");
		i=BGBCC_CCXL_LookupGlobalIndex(ctx, tb);
		if(i>0)
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_StackSizeofSig(ctx, s0);
			BGBCC_CCXL_StackCastSig(ctx, sig_size_t);
			BGBCC_CCXL_StackCallName2(ctx, tb, NULL, 0);
		}else
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_StackSizeofSig(ctx, s0);
			BGBCC_CCXL_StackCallName2(ctx, "__operator_new", NULL, 0);
		}

		if(obj->decl->flagsint&BGBCC_TYFL_NONPOD)
		{
			sprintf(tb, "%s/%s", obj->decl->name, "__init");

			//Call ObjType::__init
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
//			BGBCC_CCXL_StackCallName(ctx, tb, flag);
			BGBCC_CCXL_StackCallName2(ctx, tb, NULL, 1);
	//		return;
		}

		sprintf(tb, "%s/%s", obj->decl->name, "__ctor");
		i=BGBCC_CCXL_LookupGlobalIndex(ctx, tb);
		if(i>0)
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExprListReverse(ctx, args);
			BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
			BGBCC_CCXL_StackLoadSlot(ctx, "__ctor");
			BGBCC_CCXL_StackPopCall(ctx, 1);
			return;
		}
	}

//	s0=BCCX_GetCst(type, &bgbcc_rcst_name, "name");
}

void BGBCC_CCXL_CompileFormJmpTF(BGBCC_TransState *ctx, BCCX_Node *l,
	ccxl_label brt, ccxl_label brf)
{
//	char *s0, *s1, *s2;
	ccxl_label lbl0, lbl1, lbl2;
	BCCX_Node *ln, *rn;
	int i, j, k;

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&"))
	{
		lbl0=BGBCC_CCXL_GenSym(ctx);

		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, lbl0, brf); j=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, ln, brf); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, lbl0); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, brt, brf); k=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, rn, brf); }
		if(!k) { BGBCC_CCXL_CompileJmp(ctx, brt); }
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl0=BGBCC_CCXL_GenSym(ctx);

		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, brt, lbl0); j=1; }
		else
		{
//			BGBCC_CCXL_CompileJCF(ctx, ln, brf);
			BGBCC_CCXL_CompileJCT(ctx, ln, brt);
		}

		if(j) { BGBCC_CCXL_EmitLabel(ctx, lbl0); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, brt, brf); k=1; }
		else
		{
//			BGBCC_CCXL_CompileJCF(ctx, rn, brf);
			BGBCC_CCXL_CompileJCT(ctx, rn, brt);
		}

		if(!k) { BGBCC_CCXL_CompileJmp(ctx, brf); }
		return;
	}
	
	BGBCC_CCXL_CompileForm(ctx, l);
}

void BGBCC_CCXL_CompileForm(BGBCC_TransState *ctx, BCCX_Node *l)
{
	byte *ips[64];
	ccxl_label l0, l1, l2, l3;
	ccxl_register sreg, treg;
	ccxl_type bty, dty, sty, tty, lty, rty;
	char *s0, *s1, *s2, *s3;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	int i0, i1, i2, i3;
	int i, j, k;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		BGBCC_CCXL_CompileExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		BGBCC_CCXL_StackRet(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		BGBCC_CCXL_CompileExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		s0=BGBCC_CCXL_VarTypeString(ctx,
			BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
		BGBCC_CCXL_StackCastSig(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_var, "var"))
	{
//		BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		s0=BGBCC_CCXL_VarTypeString(ctx,
			BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
		BGBCC_CCXL_StackLitTypeSig(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof, "sizeof"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		if(!t)
		{
			u=BCCX_FindTagCst(l, &bgbcc_rcst_tyexpr, "tyexpr");
			v=NULL;
			if(u)
			{
				v=BCCX_FindTagCst(u, &bgbcc_rcst_var, "var");
				if(!v)v=BCCX_FindTagCst(u, &bgbcc_rcst_proto, "proto");
			}
			if(v)t=BCCX_FindTagCst(v, &bgbcc_rcst_type, "type");
		}

		if(!t)
		{
			printf("BGBCC_CCXL_CompileForm: sizeof: bad AST\n");
			BGBCC_CCXL_StackSizeofSig(ctx, "Pv");
			return;
		}

		s0=BGBCC_CCXL_VarTypeString(ctx, t);
		BGBCC_CCXL_StackSizeofSig(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sizeof_expr, "sizeof_expr"))
	{
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
//		ctx->cgif_no3ac++;
		BGBCC_CCXL_StackDisable3AC(ctx);
		BGBCC_CCXL_CompileExpr(ctx, v);
		BGBCC_CCXL_StackEnable3AC(ctx);
//		ctx->cgif_no3ac--;
		BGBCC_CCXL_StackSizeofVal(ctx);
		return;
	}
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_offsetof, "offsetof"))
	{
		s0=BGBCC_CCXL_VarTypeString(ctx,
			BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
		s1=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StackOffsetof(ctx, s0, s1);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(s0)
		{
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_StackBinaryOp(ctx, s0);
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_CompileAssign(ctx, ln);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, rn);
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_CompileAssign(ctx, ln);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_getindex, "getindex"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		if(BGBCC_CCXL_IsIntP(ctx, rn))
		{
			i=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_StackLoadIndexConst(ctx, i);
		}else
		{
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_StackLoadIndex(ctx);
		}
		return;

//		BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_array, "array"));
//		BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_index, "index"));
//		BGBCC_CCXL_StackLoadIndex(ctx);
//		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_objref, "objref"))
	{
		BGBCC_CCXL_CompileExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StackLoadSlot(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_preinc, "preinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));
		
		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "++", s0, 1);
			return;
		}
		
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_predec, "predec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "--", s0, 1);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "++", s0, 2);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupB(ctx);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "--", s0, 2);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupB(ctx);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_CCXL_CompileExpr(ctx,
				BCCX_FetchCst(l, &bgbcc_rcst_then, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");
			if(t)BGBCC_CCXL_CompileExpr(ctx, t);
			return;
		}

		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		
		i0=BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		i1=BGBCC_CCXL_InferExpr(ctx, rn, &rty);
//		BGBCC_CCXL_GetTypeBinaryDest(ctx, CCXL_BINOP_ADD, lty, rty, &dty);
		BGBCC_CCXL_GetTypeCompareBinaryDest(ctx, CCXL_CMP_NV, lty, rty, &dty);
		s0=BGBCC_CCXL_TypeGetSig(ctx, dty);

#if 0
		i0=BGBCC_CCXL_InferExprCleanP(ctx, ln);
		i1=BGBCC_CCXL_InferExprCleanP(ctx, rn);
		if((i&3)==3)
		{
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_CompileCSel(ctx, t);
			return;
		}
#endif

//		BGBCC_CCXL_StackBeginU(ctx, "i");
		BGBCC_CCXL_StackBeginU(ctx, s0);
		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileJCF(ctx, t, l0);

		BGBCC_CCXL_CompileExprT(ctx, ln);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_CompileJmp(ctx, l1);
		BGBCC_CCXL_EmitLabel(ctx, l0);

		BGBCC_CCXL_CompileExprT(ctx, rn);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_StackEndU(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				BGBCC_CCXL_CompileStatement(ctx, c);
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CCXL_CompileExprT(ctx, c);
			break;
		}

		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&"))
	{
		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		l3=BGBCC_CCXL_GenSym(ctx);

		BGBCC_CCXL_StackBeginU(ctx, "i");

#if 1
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, l2, l0); j=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, ln, l0); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, l2); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, l3, l0); k=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, rn, l0); }
#endif

		if(k) { BGBCC_CCXL_EmitLabel(ctx, l3); }
		BGBCC_CCXL_StackPushConstInt(ctx, 1);
		BGBCC_CCXL_StackSetU(ctx);
		BGBCC_CCXL_CompileJmp(ctx, l1);

		BGBCC_CCXL_EmitLabel(ctx, l0);
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_StackEndU(ctx);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		l3=BGBCC_CCXL_GenSym(ctx);

		BGBCC_CCXL_StackBeginU(ctx, "i");

#if 1
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, l0, l2); j=1; }
		else
			{ BGBCC_CCXL_CompileJCT(ctx, ln, l0); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, l2); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, l0, l3); k=1; }
		else
			{ BGBCC_CCXL_CompileJCT(ctx, rn, l0); }
#endif

		if(k) { BGBCC_CCXL_EmitLabel(ctx, l3); }
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		BGBCC_CCXL_StackSetU(ctx);
		BGBCC_CCXL_CompileJmp(ctx, l1);
		BGBCC_CCXL_EmitLabel(ctx, l0);
		BGBCC_CCXL_StackPushConstInt(ctx, 1);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_StackEndU(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_boolify, "boolify"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
		BGBCC_CCXL_StackCastBool(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

//		if(BCCX_AttrIsCstP(l, &bgbcc_rcst_op, "op", "*"))
		if(!strcmp(s0, "*"))
		{
			BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));
//			BGBCC_CCXL_StackPushConstInt(ctx, 0);
//			BGBCC_CCXL_StackLoadIndex(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			return;
		}

//		if(BCCX_AttrIsCstP(l, &bgbcc_rcst_op, "op", "&"))
		if(!strcmp(s0, "&"))
		{
			t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_value, "value"));

			if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
			{
				s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_StackLoadAddr(ctx, s0);
				return;
			}

			if(BCCX_TagIsCstP(t, &bgbcc_rcst_objref, "objref"))
			{
				BGBCC_CCXL_CompileExpr(ctx,
					BCCX_FetchCst(t, &bgbcc_rcst_value, "value"));
				s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_StackLoadSlotAddr(ctx, s0);
				return;
			}

			if(BCCX_TagIsCstP(t, &bgbcc_rcst_getindex, "getindex") ||
				BCCX_TagIsCstP(t, &bgbcc_rcst_vector_ref, "vector-ref"))
			{
				BGBCC_CCXL_CompileExpr(ctx,
					BCCX_FetchCst(t, &bgbcc_rcst_array, "array"));
				BGBCC_CCXL_CompileExpr(ctx,
					BCCX_FetchCst(t, &bgbcc_rcst_index, "index"));
				BGBCC_CCXL_StackLoadIndexAddr(ctx);
				return;
			}

			BGBCC_CCXL_Error(ctx, "Bad expression type for '&'\n");
			return;
		}

		ln=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if((ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_BS2))
		{
			i0=BGBCC_CCXL_InferExpr(ctx, ln, &lty);
			
			if(i0>0)
			{
				s1=BGBCC_CCXL_StackGetNameBinaryOverload(ctx, s0);
				s2=BGBCC_CCXL_StackGetSigUnary(ctx, lty);

				i2=BGBCC_CCXL_LookupGlobalIndexSig(ctx, s1, s2);
				if(i2>=0)
				{
					BGBCC_CCXL_PushMark(ctx);
					BGBCC_CCXL_CompileExpr(ctx, ln);
					BGBCC_CCXL_StackCallName(ctx, s1, 0);
					return;
				}
			}
		}

		BGBCC_CCXL_CompileExpr(ctx, ln);
		BGBCC_CCXL_StackUnaryOp(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		
		if((ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_BS2))
		{
			i0=BGBCC_CCXL_InferExpr(ctx, ln, &lty);
			i1=BGBCC_CCXL_InferExpr(ctx, rn, &rty);
			
			if((i0>0) && (i1>0))
			{
				s1=BGBCC_CCXL_StackGetNameBinaryOverload(ctx, s0);
				s2=BGBCC_CCXL_StackGetSigBinary(ctx, lty, rty);

				i2=BGBCC_CCXL_LookupGlobalIndexSig(ctx, s1, s2);
				if(i2>=0)
				{
					BGBCC_CCXL_PushMark(ctx);
					BGBCC_CCXL_CompileExpr(ctx, rn);
					BGBCC_CCXL_CompileExpr(ctx, ln);
					BGBCC_CCXL_StackCallName(ctx, s1, 0);
					return;
				}
			}
		}

		BGBCC_CCXL_CompileExpr(ctx, ln);
		BGBCC_CCXL_CompileExpr(ctx, rn);

//		BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_left, "left"));
//		BGBCC_CCXL_CompileExpr(ctx, BCCX_FetchCst(l, &bgbcc_rcst_right, "right"));

		BGBCC_CCXL_StackBinaryOp(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_comma, "comma"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		i=BGBCC_CCXL_InferExprCleanP(ctx, ln);
		if(i&1)
		{
			BGBCC_CCXL_CompileExpr(ctx, rn);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, ln);
		BGBCC_CCXL_StackPop(ctx);
		BGBCC_CCXL_CompileExpr(ctx, rn);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		BGBCC_CCXL_CompileFuncall(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_methodcall, "methodcall"))
	{
		BGBCC_CCXL_CompileMethodcall(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_new, "new"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_type, "type");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		BGBCC_CCXL_CompileNewBasic(ctx, ln, rn);
//		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_throw, "throw"))
	{
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_list, "list"))
	{
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	BGBCC_CCXL_Error(ctx, "Unhandled expr %s\n", BCCX_Tag(l));
	BGBCC_CCXL_StackPushConstInt(ctx, 0);
}

void BGBCC_CCXL_CompileExprT(BGBCC_TransState *ctx, BCCX_Node *l)
{
	long long li, lj;
	double g;
	float f;
	BCCX_Node *t, *c, *ct, *cv;
	char *s0, *s1, *s2;
//	u32 ui, uj;
//	u16 usi, usj;
	int i, j;

	l=BGBCC_CCXL_ReduceExpr(ctx, l);

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");	
		BGBCC_CCXL_PushLoad(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		
		if(s0 && !strcmp(s0, "U"))
		{
			BGBCC_CCXL_StackPushConstUInt(ctx, li);
			return;
		}
		
		if(s0 && (!strcmp(s0, "L") || !strcmp(s0, "LL") ||
			!strcmp(s0, "ULL") ||
			(((s32)li)!=li)))
		{
			BGBCC_CCXL_StackPushConstLong(ctx, li);
			return;
		}

		BGBCC_CCXL_StackPushConstInt(ctx, li);
		return;
	}
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_real, "real"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
//		if(s0 && !stricmp(s0, "F"))
		if(s0)
		{
			if(!strcmp(s0, "F"))
			{
				g=BCCX_GetFloatCst(l, &bgbcc_rcst_value, "value");
				BGBCC_CCXL_StackPushConstFloat(ctx, g);
				return;
			}

			if(!strcmp(s0, "HF"))
			{
				g=BCCX_GetFloatCst(l, &bgbcc_rcst_value, "value");
				
				BGBCC_Float16_Init();
				
//				*(float *)(&uj)=g;
//				BGBCC_JX2_ConstConvFloatToHalf(uj, &usj);
				j=BGBCC_EncodeFloat16(g);
				g=BGBCC_DecodeFloat16(j);
				
				BGBCC_CCXL_StackPushConstFloat(ctx, g);
				return;
			}
		}

		g=BCCX_GetFloatCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_StackPushConstDouble(ctx, g);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int128, "int128"))
	{
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		lj=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		BGBCC_CCXL_StackPushConstInt128(ctx, li, lj);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_float128, "float128"))
	{
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		lj=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		BGBCC_CCXL_StackPushConstFloat128(ctx, li, lj);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_string, "string"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_charstring, "charstring"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_value, "value");
		if(s0) { i=BGBCP_ParseChar(&s0); }
			else i=0;
		BGBCC_CCXL_StackPushConstInt(ctx, i);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_complex, "complex"))
	{
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	BGBCC_CCXL_CompileForm(ctx, l);
	return;
}

void BGBCC_CCXL_CompileExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_CCXL_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_CCXL_CompileExprTail(BGBCC_TransState *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_CCXL_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_CCXL_CompileExprAsType(BGBCC_TransState *ctx,
	BCCX_Node *ty, BCCX_Node *l)
{
	char *s;
	int i;

	BGBCC_CCXL_CompileExpr(ctx, l);
	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	BGBCC_CCXL_StackCastSig(ctx, s);
}
