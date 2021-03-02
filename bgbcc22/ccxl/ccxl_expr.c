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
		s=BGBCC_CCXL_CompileRemapName(ctx, s);
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
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");		
		if(BGBCC_CCXL_CheckCompileObjSet(ctx, v, s))
		{
			return;
		}
		
//		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_CompileExpr(ctx, v);
//		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StackStoreSlot(ctx, s);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_getindex, "getindex") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_vector_ref, "vector-ref"))
	{
		ct=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		cv=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");
		ct=BGBCC_CCXL_ReduceExpr(ctx, ct);
		cv=BGBCC_CCXL_ReduceExpr(ctx, cv);
		
		if(BGBCC_CCXL_CheckCompileStoreIndex(ctx, ct, cv)>0)
			return;
		
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

int BGBCC_CCXL_NameIsQName(BGBCC_TransState *ctx,
	char *name)
{
	char *s, *s0;
	int i, j, k;
	
	s=name;
	while(*s)
	{
		if((*s=='!') || (*s=='/'))
			return(1);
		s++;
	}

	return(0);
}

char *BGBCC_CCXL_CompileRemapName(BGBCC_TransState *ctx,
	char *name)
{
	char tb1[256];
	BGBCC_CCXL_RegisterInfo *ri;
	BGBCC_CCXL_RegisterInfo *gbl;
	char *s, *s0;
	int i, j, k;
	
	s=name;
	while(*s)
	{
		if((*s=='!') || (*s=='/'))
			return(name);
		s++;
	}

	if(	(ctx->lang==BGBCC_LANG_CPP)	||
		(ctx->lang==BGBCC_LANG_CS)	||
		(ctx->lang==BGBCC_LANG_BS2)	||
		(ctx->lang==BGBCC_LANG_JAVA)	)
	{
		if(!bgbcp_strcmp(name, "this"))
		{
//			return(name);
			return(bgbcc_strdup("__object_this"));
		}
	}

	if(	(ctx->lang==BGBCC_LANG_CS)	||
		(ctx->lang==BGBCC_LANG_BS2)	||
		(ctx->lang==BGBCC_LANG_JAVA)	)
	{
		if(!bgbcp_strcmp(name, "null"))
		{
			return(bgbcc_strdup("__object_null"));
		}
	}
	
	if(	(ctx->lang==BGBCC_LANG_BS2)	||
		(ctx->lang==BGBCC_LANG_JAVA)	)
	{
		if(!bgbcp_strcmp(name, "super"))
		{
			return(bgbcc_strdup("__object_super"));
		}
	}

	s0=name;

	ri=NULL;
	for(j=ctx->vlcl_stackpos-1; j>=0; j--)
	{
		sprintf(tb1, "%s!%d", s0, ctx->vlcl_stack[j]);
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
		for(i=(ctx->n_imp-1); i>=0; i--)
		{
			sprintf(tb1, "%s/%s", ctx->imp_ns[i], name);
			gbl=BGBCC_CCXL_LookupGlobal(ctx, tb1);
			if(gbl)
				{ ri=gbl; break; }
		}
	}

	if(ri)
	{
		s0=ri->name;
	}
	
	return(s0);
}

void BGBCC_CCXL_CompileFuncallI(BGBCC_TransState *ctx,
	BCCX_Node *l, char *dname, int flag)
{
//	char tb1[256];
//	BGBCC_CCXL_RegisterInfo *ri;
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j, k;

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

		s0=BGBCC_CCXL_CompileRemapName(ctx, s0);

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

void BGBCC_CCXL_CompileArgsAsVarArray(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i, na;

	BGBCC_CCXL_PushMark(ctx);

	i=0; cur=l;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	na=i;
	while(i--)
	{
//		BGBCC_CCXL_CompileExpr(ctx, stk[i]);
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
		BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", n);
	}

	if(na>8)
	{
		BGBCC_CCXL_StackPushConstInt(ctx, na);
		sprintf(tb, "__lvo_newvararray_n");
	}else
	{
		sprintf(tb, "__lvo_newvararray_%d", na);
	}
	BGBCC_CCXL_StackCallName(ctx, tb, 0);
}

void BGBCC_CCXL_CompileArgsAsVarArraySig_A1(
	BGBCC_TransState *ctx, BCCX_Node **stk, int na, char *sig)
{
	char tb[256];
	BCCX_Node *cur, *n;
	char *sfn;
	int i;

	sfn=NULL;
	
	switch(*sig)
	{
	case 'a':	sfn="__lvo_newarray_sb";	break;
	case 'b':	sfn="__lvo_newarray_sb";	break;
	case 'c':	sfn="__lvo_newarray_sb";	break;
	case 'd':	sfn="__lvo_newarray_d";		break;
	case 'e':	sfn="__lvo_newarray_d";		break;
	case 'f':	sfn="__lvo_newarray_f";		break;
	case 'g':	break;
	case 'h':	sfn="__lvo_newarray_ub";	break;
	case 'i':	sfn="__lvo_newarray_si";	break;
	case 'j':	sfn="__lvo_newarray_ui";	break;
	case 'k':	sfn="__lvo_newarray_hf";	sig="f";	break;
	case 'l':	sfn="__lvo_newarray_sl";	break;
	case 'm':	sfn="__lvo_newarray_ul";	break;
	case 'n':	break;
	case 'o':	break;
	case 'p':	break;
	case 'q':	break;
	case 'r':	sfn="__lvo_newvararray";	break;
	case 's':	sfn="__lvo_newarray_ss";	break;
	case 't':	sfn="__lvo_newarray_us";	break;
	case 'u':	break;
	case 'v':	break;
	case 'w':	sfn="__lvo_newarray_us";	break;
	case 'x':	sfn="__lvo_newarray_l";		break;
	case 'y':	sfn="__lvo_newarray_ul";	break;
	case 'z':	break;

	default:
		sig="r";
		sfn="__lvo_newvararray";
		break;
	}

	BGBCC_CCXL_PushMark(ctx);

	i=na;
	while(i--)
	{
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
		BGBCC_CCXL_CompileExprAsTypeSig(ctx, sig, n);
	}

	if(na>8)
	{
		BGBCC_CCXL_StackPushConstInt(ctx, na);
		sprintf(tb, "%s_n", sfn);
	}else
	{
		sprintf(tb, "%s_%d", sfn, na);
	}
	BGBCC_CCXL_StackCallName(ctx, tb, 0);
}

void BGBCC_CCXL_CompileArgsAsVarArraySig_A2(
	BGBCC_TransState *ctx, BCCX_Node **stk, int na, char *sig)
{
	char tb[256];
	int i;

	if(na<=8)
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A1(ctx, stk, na, sig);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);

	i=na;
	while(i>8)
	{
		i-=8;
		BGBCC_CCXL_CompileArgsAsVarArraySig_A1(ctx, stk+i, 8, sig);
	}
	BGBCC_CCXL_CompileArgsAsVarArraySig_A1(ctx, stk, i, sig);

	sprintf(tb, "__lvo_array_append_%d", (na+7)/8);
	BGBCC_CCXL_StackCallName(ctx, tb, 0);
}

void BGBCC_CCXL_CompileArgsAsVarArraySig_A3(
	BGBCC_TransState *ctx, BCCX_Node **stk, int na, char *sig)
{
	char tb[256];
	int i;

	if(na<=64)
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A2(ctx, stk, na, sig);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);

	i=na;
	while(i>64)
	{
		i-=64;
		BGBCC_CCXL_CompileArgsAsVarArraySig_A2(ctx, stk+i, 64, sig);
	}
	BGBCC_CCXL_CompileArgsAsVarArraySig_A2(ctx, stk, i, sig);

	sprintf(tb, "__lvo_array_append_%d", (na+63)/64);
	BGBCC_CCXL_StackCallName(ctx, tb, 0);
}

void BGBCC_CCXL_CompileArgsAsVarArraySig_A4(
	BGBCC_TransState *ctx, BCCX_Node **stk, int na, char *sig)
{
	char tb[256];
	int i;

	if(na<=512)
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A3(ctx, stk, na, sig);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);

	i=na;
	while(i>512)
	{
		i-=512;
		BGBCC_CCXL_CompileArgsAsVarArraySig_A3(ctx, stk+i, 512, sig);
	}
	BGBCC_CCXL_CompileArgsAsVarArraySig_A3(ctx, stk, i, sig);
	sprintf(tb, "__lvo_array_append_%d", (na+511)/512);
	BGBCC_CCXL_StackCallName(ctx, tb, 0);
}

void BGBCC_CCXL_CompileArgsAsVarArraySig(
	BGBCC_TransState *ctx, BCCX_Node *l, char *sig)
{
	char tb[256];
	BCCX_Node *stk[8192];
	BCCX_Node *cur, *n;
	char *sfn;
	int i, j, k, na;

	
	i=0; cur=l;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	na=i;

	if(na>512)
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A4(ctx, stk, na, sig);
	}else
		if(na>64)
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A3(ctx, stk, na, sig);
	}else
		if(na>8)
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A2(ctx, stk, na, sig);
	}else
	{
		BGBCC_CCXL_CompileArgsAsVarArraySig_A1(ctx, stk, na, sig);
	}
}

void BGBCC_CCXL_CompileMethodcall(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_RegisterInfo *ri;
	ccxl_type sty;
	char *s0, *s1, *s2, *sig;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
	u=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");

	v=BGBCC_CCXL_ReduceExpr(ctx, v);
	
	if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
	{
		s1=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
		ri=BGBCC_CCXL_LookupStructureMethodRns(ctx, s1, s0, NULL);
		if(ri && !(ri->flagsint&BGBCC_TYFL_FINAL))
//		if(ri && (ri->flagsint&BGBCC_TYFL_VIRTUAL))
		{
			bgbcc_strcpy_nosig(tb, ri->name);
			s2=bgbcc_strdup(tb);
			sprintf(tb, "%s/%s", ri->defp->name, s2);

			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExprListReverse(ctx, u);
			BGBCC_CCXL_CompileExpr(ctx, v);
	//		BGBCC_CCXL_StackCallName(ctx, s0, flag);
			BGBCC_CCXL_StackCallName2(ctx, tb, NULL, 4);
			return;
		}
	}

	if(BGBCC_CCXL_InferExpr(ctx, v, &sty)<=0)
	{
		BGBCC_DBGBREAK
		return;
	}

	if(BGBCC_CCXL_TypeVarObjP(ctx, sty))
	{
		if(ctx->lang==BGBCC_LANG_BS2)
		{
			if(!strcmp(s0, "toString"))
			{
				BGBCC_CCXL_PushMark(ctx);
				BGBCC_CCXL_CompileExpr(ctx, l);
				BGBCC_CCXL_StackCallName(ctx, "__lvo_tostring", 0);
				return;
			}
		}
	
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileArgsAsVarArray(ctx, u);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		BGBCC_CCXL_CompileExpr(ctx, v);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_methodcall", 0);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);
	BGBCC_CCXL_CompileExprListReverse(ctx, u);

	sig=BGBCC_CCXL_StackGetSigCallArgs(ctx);

	BGBCC_CCXL_CompileExpr(ctx, v);
	BGBCC_CCXL_StackDup(ctx);

//	BGBCC_CCXL_StackLoadSlot(ctx, s0);
	BGBCC_CCXL_StackLoadSlotSig(ctx, s0, sig);

	BGBCC_CCXL_StackPopCall(ctx, 4);
	return;
}

void BGBCC_CCXL_CompileArgsFuncallName(BGBCC_TransState *ctx,
	char *fname, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i, na;

	BGBCC_CCXL_PushMark(ctx);

	i=0; cur=l;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	na=i;
	while(i--)
	{
//		BGBCC_CCXL_CompileExpr(ctx, stk[i]);
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
//		BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", n);
		BGBCC_CCXL_CompileExpr(ctx, n);
	}

	BGBCC_CCXL_StackCallName(ctx, fname, 0);
}

int BGBCC_CCXL_CheckCompileObjGet(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	char tb[256];
	ccxl_type sty;
	char *s0;
//	char *s0, *s1, *s2, *sig;
//	BCCX_Node *c, *d, *t, *u, *v;
//	int i, j;

	l=BGBCC_CCXL_ReduceExpr(ctx, l);
	if(BGBCC_CCXL_InferExpr(ctx, l, &sty)<=0)
		return(0);
	if(BGBCC_CCXL_CheckForTypeGetter(ctx, sty, name)>0)
	{
		sprintf(tb, "get_%s", name);
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_StackLoadSlotSig(ctx, tb, "()");
		BGBCC_CCXL_StackPopCall(ctx, 0);
		return(1);
	}

	if(BGBCC_CCXL_TypeVarObjP(ctx, sty))
	{
		if(ctx->lang==BGBCC_LANG_BS2)
		{
			if(!strcmp(name, "toString"))
			{
				BGBCC_CCXL_PushMark(ctx);
				BGBCC_CCXL_CompileExpr(ctx, l);
				BGBCC_CCXL_StackCallName(ctx, "__lvo_tostring", 0);
				return(1);
			}
		}
	
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackLoadSlotSig(ctx, tb, "()");
//		BGBCC_CCXL_StackPopCall(ctx, 0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot", 0);
		return(1);
	}
	
	if(BGBCC_CCXL_TypeRefArrayP(ctx, sty))
	{
		if((!strcmp(name, "length") && (ctx->lang==BGBCC_LANG_BS2)) ||
			(!strcmp(name, "Length") && (ctx->lang==BGBCC_LANG_CS)))
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExpr(ctx, l);
			BGBCC_CCXL_StackCallName(ctx, "__lvo_get_length", 0);
			return(1);
		}

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot", 0);
		return(1);
	}

	if(BGBCC_CCXL_TypeRefStringP(ctx, sty))
	{
		if((!strcmp(name, "length") && (ctx->lang==BGBCC_LANG_BS2)) ||
			(!strcmp(name, "Length") && (ctx->lang==BGBCC_LANG_CS)))
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExpr(ctx, l);
			BGBCC_CCXL_StackCallName(ctx, "__lvo_get_length", 0);
			return(1);
		}

		if(ctx->lang==BGBCC_LANG_BS2)
		{
			s0=NULL;
			if(!strcmp(name, "toInt"))
				s0="__lvo_stringtoint";
			if(!strcmp(name, "toUInt"))
				s0="__lvo_stringtouint";
			if(!strcmp(name, "toLong"))
				s0="__lvo_stringtolong";
			if(!strcmp(name, "toULong"))
				s0="__lvo_stringtoulong";

			if(!strcmp(name, "toFloat"))
				s0="__lvo_stringtofloat";
			if(!strcmp(name, "toDouble"))
				s0="__lvo_stringtodouble";

			if(s0)
			{
				BGBCC_CCXL_PushMark(ctx);
				BGBCC_CCXL_CompileExpr(ctx, l);
				BGBCC_CCXL_StackCallName(ctx, s0, 0);
				return(1);
			}
		}

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot", 0);
		return(1);
	}

	return(0);
}

int BGBCC_CCXL_CheckCompileObjSet(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	char tb[256];
	ccxl_type sty;
	char *sig;
//	char *s0, *s1, *s2, *sig;
//	BCCX_Node *c, *d, *t, *u, *v;
//	int i, j;

	l=BGBCC_CCXL_ReduceExpr(ctx, l);
	if(BGBCC_CCXL_InferExpr(ctx, l, &sty)<=0)
		return(0);

	if(BGBCC_CCXL_CheckForTypeSetter(ctx, sty, name)>0)
	{
		sprintf(tb, "set_%s", name);
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
		sig=BGBCC_CCXL_StackGetSigCallArgs(ctx);
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_StackLoadSlotSig(ctx, tb, sig);
		BGBCC_CCXL_StackPopCall(ctx, 1);
		BGBCC_CCXL_StackPop(ctx);
		return(1);
	}

	if(BGBCC_CCXL_TypeVarObjP(ctx, sty))
	{
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackLoadSlotSig(ctx, tb, "()");
//		BGBCC_CCXL_StackPopCall(ctx, 0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot", 1);
		BGBCC_CCXL_StackPop(ctx);
		return(1);
	}
	
	if(BGBCC_CCXL_TypeRefArrayP(ctx, sty) ||
		BGBCC_CCXL_TypeRefStringP(ctx, sty))
	{
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot", 1);
		BGBCC_CCXL_StackPop(ctx);
		return(1);
	}

	return(0);
}

int BGBCC_CCXL_CheckCompileLoadIndex(BGBCC_TransState *ctx,
	BCCX_Node *arr, BCCX_Node *idx)
{
	char tb[256];
	ccxl_type sty, tty;
	char *sig, *fn;
	int bt;
//	char *s0, *s1, *s2, *sig;
//	BCCX_Node *c, *d, *t, *u, *v;
//	int i, j;

//	return(0);

//	l=BGBCC_CCXL_ReduceExpr(ctx, l);
	if(BGBCC_CCXL_InferExpr(ctx, arr, &sty)<=0)
		return(0);

	if(BGBCC_CCXL_TypeSquareArrayP(ctx, sty))
	{
		if(	(ctx->lang==BGBCC_LANG_C) ||
			(ctx->lang==BGBCC_LANG_CPP)	)
		{
			/* C mode: Always use non-bounds-checked logic. */
			return(0);
		}
	}

	if(BGBCC_CCXL_TypeRefArrayP(ctx, sty) ||
		BGBCC_CCXL_TypeVariantP(ctx, sty) ||
		BGBCC_CCXL_TypeRefStringP(ctx, sty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, sty, &tty);
		
		fn="__lvo_loadindex_va";

		if(BGBCC_CCXL_TypeVariantP(ctx, sty))
//			{ fn="__lvo_loadindex_var"; }
			{ fn="__lva_loadindex"; }
		else if(BGBCC_CCXL_TypeRefStringP(ctx, sty))
			{ fn="__lvo_loadindex_str"; }
		else if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty))
			{ fn="__lvo_loadindex_p"; }
		else if(BGBCC_CCXL_TypeVarRefP(ctx, tty))
			{ fn="__lvo_loadindex_v"; }
		else
		{
//			bt=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
			bt=BGBCC_CCXL_GetTypeOperationZ(ctx, tty);
			switch(bt)
			{
			case CCXL_TY_I:		fn="__lvo_loadindex_i"; break;
			case CCXL_TY_L:		fn="__lvo_loadindex_l"; break;
			case CCXL_TY_F:		fn="__lvo_loadindex_f"; break;
			case CCXL_TY_D:		fn="__lvo_loadindex_d"; break;
			case CCXL_TY_P:		fn="__lvo_loadindex_p"; break;
			case CCXL_TY_V:		fn="__lvo_loadindex_v"; break;
			case CCXL_TY_NL:	fn="__lvo_loadindex_l"; break;
			case CCXL_TY_SB:	fn="__lvo_loadindex_sb"; break;
			case CCXL_TY_UB:	fn="__lvo_loadindex_ub"; break;
			case CCXL_TY_SS:	fn="__lvo_loadindex_ss"; break;
			case CCXL_TY_US:	fn="__lvo_loadindex_us"; break;
			case CCXL_TY_UI:	fn="__lvo_loadindex_ui"; break;
			case CCXL_TY_UL:	fn="__lvo_loadindex_ul"; break;
			case CCXL_TY_UNL:	fn="__lvo_loadindex_ul"; break;

			case CCXL_TY_M64:	fn="__lvo_loadindex_ul"; break;
			case CCXL_TY_M128:	fn="__lvo_loadindex_x"; break;
			default:
				break;
			}
		}

		if(fn)
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExpr(ctx, idx);
			BGBCC_CCXL_CompileExpr(ctx, arr);
			BGBCC_CCXL_StackCallName(ctx, fn, 0);
	//		BGBCC_CCXL_StackPop(ctx);

			if(BGBCC_CCXL_TypeVarRefP(ctx, tty))
			{
				sig=BGBCC_CCXL_TypeGetSig(ctx, tty);
				BGBCC_CCXL_StackCastSig(ctx, sig);
			}

			return(1);
		}
	}

	return(0);
}

int BGBCC_CCXL_CheckCompileStoreIndex(BGBCC_TransState *ctx,
	BCCX_Node *arr, BCCX_Node *idx)
{
	char tb[256];
	ccxl_type sty, tty;
	char *sig, *fn;
	int bt;
//	char *s0, *s1, *s2, *sig;
//	BCCX_Node *c, *d, *t, *u, *v;
//	int i, j;

//	l=BGBCC_CCXL_ReduceExpr(ctx, l);
	if(BGBCC_CCXL_InferExpr(ctx, arr, &sty)<=0)
		return(0);

	if(BGBCC_CCXL_TypeSquareArrayP(ctx, sty))
	{
		if(	(ctx->lang==BGBCC_LANG_C) ||
			(ctx->lang==BGBCC_LANG_CPP)	)
		{
			/* C mode: Always use non-bounds-checked logic. */
			return(0);
		}
	}

	if(BGBCC_CCXL_TypeRefArrayP(ctx, sty) ||
		BGBCC_CCXL_TypeVariantP(ctx, sty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, sty, &tty);
		
		fn="__lvo_storeindex_va";

		if(BGBCC_CCXL_TypeVariantP(ctx, sty))
//			{ fn="__lvo_storeindex_var"; }
			{ fn="__lva_storeindex"; }
		else if(BGBCC_CCXL_TypeRefStringP(ctx, sty))
			{ fn="__lvo_storeindex_str"; }
		else if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, tty))
			{ fn="__lvo_storeindex_p"; }
		else if(BGBCC_CCXL_TypeVarRefP(ctx, tty))
			{ fn="__lvo_storeindex_v"; }
		else
		{
//			bt=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
			bt=BGBCC_CCXL_GetTypeOperationZ(ctx, tty);
			switch(bt)
			{
			case CCXL_TY_I:		fn="__lvo_storeindex_i"; break;
			case CCXL_TY_L:		fn="__lvo_storeindex_l"; break;
			case CCXL_TY_F:		fn="__lvo_storeindex_f"; break;
			case CCXL_TY_D:		fn="__lvo_storeindex_d"; break;
			case CCXL_TY_P:		fn="__lvo_storeindex_p"; break;
			case CCXL_TY_V:		fn="__lvo_storeindex_v"; break;
			case CCXL_TY_NL:	fn="__lvo_storeindex_l"; break;
			case CCXL_TY_SB:	fn="__lvo_storeindex_b"; break;
			case CCXL_TY_UB:	fn="__lvo_storeindex_b"; break;
			case CCXL_TY_SS:	fn="__lvo_storeindex_s"; break;
			case CCXL_TY_US:	fn="__lvo_storeindex_s"; break;
			case CCXL_TY_UI:	fn="__lvo_storeindex_i"; break;
			case CCXL_TY_UL:	fn="__lvo_storeindex_l"; break;
			case CCXL_TY_UNL:	fn="__lvo_storeindex_l"; break;

			case CCXL_TY_M64:	fn="__lvo_storeindex_l"; break;
			case CCXL_TY_M128:	fn="__lvo_storeindex_x"; break;

			default:
				break;
			}
		}

		if(fn)
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
	//		BGBCC_CCXL_StackPushConstString(ctx, name);
			BGBCC_CCXL_CompileExpr(ctx, idx);
			BGBCC_CCXL_CompileExpr(ctx, arr);
			BGBCC_CCXL_StackCallName(ctx, fn, 1);
			BGBCC_CCXL_StackPop(ctx);
			return(1);
		}
	}

	return(0);
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

void BGBCC_CCXL_CompileExprRef(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2, *s3;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	t=l;

	if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
	{
		s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
		s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
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

	s1=BCCX_Tag(t);
	BGBCC_CCXL_Error(ctx, "Bad expression type for '&', %s\n", s1);
	return;
}

void BGBCC_CCXL_CompileExprDeref(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BGBCC_CCXL_CompileExpr(ctx, l);
	BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
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
		s0=BGBCC_CCXL_VarTypeString(ctx,
			BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
		if(!strcmp(s0, "v"))
		{
			BGBCC_CCXL_StackPushVoid(ctx);
			return;
		}

		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		if(BCCX_TagIsCstP(t, &bgbcc_rcst_list, "list"))
		{
			BGBCC_CCXL_CompileExprAsTypeSig(ctx, s0, t);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
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
//		t=BCCX_FindTagCst(l, &bgbcc_rcst_tyexpr, "tyexpr");
//		t=BCCX_FindTagCst(t, &bgbcc_rcst_type, "type");
		t=BCCX_FetchCst(l, &bgbcc_rcst_tyexpr, "tyexpr");
		t=BCCX_FindTagCst(t, &bgbcc_rcst_type, "type");
		s0=BGBCC_CCXL_VarTypeString(ctx, t);

		s1=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		if(s1)
		{
			BGBCC_CCXL_StackOffsetof(ctx, s0, s1);
			return;
		}

		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		
		if(v)
		{
			s1=BGBCC_CCXL_FlattenDottedName(ctx, v);
			if(s1)
			{
				BGBCC_CCXL_StackOffsetof(ctx, s0, s1);
				return;
			}
		}

		BGBCC_CCXL_StubError(ctx);
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
			if(BGBCC_CCXL_InferExprCleanP(ctx, ln))
			{
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOp(ctx, s0);
	//			BGBCC_CCXL_StackDup(ctx);
				BGBCC_CCXL_StackDupClean(ctx);
				BGBCC_CCXL_CompileAssign(ctx, ln);
			}else
			{
				BGBCC_CCXL_CompileExprRef(ctx, ln);
				BGBCC_CCXL_StackDupClean(ctx);
				BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOp(ctx, s0);
				BGBCC_CCXL_StackDupClean(ctx);
				BGBCC_CCXL_StackDupIdx(ctx, 2);
				BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
				BGBCC_CCXL_StackExch(ctx);
				BGBCC_CCXL_StackPop(ctx);
			}
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
//		if((ctx->lln==0x1CB) && (ctx->lang==BGBCC_LANG_CS))
//			{ BGBCC_DBGBREAK }
	
		ln=BCCX_FetchCst(l, &bgbcc_rcst_array, "array");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_index, "index");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		if(BGBCC_CCXL_CheckCompileLoadIndex(ctx, ln, rn))
			return;

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
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");		
		if(BGBCC_CCXL_CheckCompileObjGet(ctx, t, s0))
		{
			return;
		}
		
		BGBCC_CCXL_CompileExpr(ctx, t);
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
			s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "++", s0, 1);
			return;
		}

#if 0
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_CompileAssign(ctx, t);
#endif

		if(BGBCC_CCXL_InferExprCleanP(ctx, t))
//		if(0)
		{
			BGBCC_CCXL_CompileExpr(ctx, t);
			BGBCC_CCXL_StackUnaryOp(ctx, "++");
	//		BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_CompileAssign(ctx, t);
		}else
		{
			BGBCC_CCXL_CompileExprRef(ctx, t);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			BGBCC_CCXL_StackUnaryOp(ctx, "++");
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackDupIdx(ctx, 2);
			BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
			BGBCC_CCXL_StackExch(ctx);
			BGBCC_CCXL_StackPop(ctx);
		}

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_predec, "predec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "--", s0, 1);
			return;
		}

#if 0
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_CompileAssign(ctx, t);
#endif

		if(BGBCC_CCXL_InferExprCleanP(ctx, t))
//		if(0)
		{
			BGBCC_CCXL_CompileExpr(ctx, t);
			BGBCC_CCXL_StackUnaryOp(ctx, "--");
	//		BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_CompileAssign(ctx, t);
		}else
		{
			BGBCC_CCXL_CompileExprRef(ctx, t);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			BGBCC_CCXL_StackUnaryOp(ctx, "--");
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackDupIdx(ctx, 2);
			BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
			BGBCC_CCXL_StackExch(ctx);
			BGBCC_CCXL_StackPop(ctx);
		}

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "++", s0, 2);
			return;
		}

#if 0
		BGBCC_CCXL_CompileExpr(ctx, t);
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupB(ctx);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
		BGBCC_CCXL_CompileAssign(ctx, t);
#endif

		if(BGBCC_CCXL_InferExprCleanP(ctx, t))
//		if(0)
		{
			BGBCC_CCXL_CompileExpr(ctx, t);
	//		BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackUnaryOp(ctx, "++");
			BGBCC_CCXL_CompileAssign(ctx, t);
		}else
		{
			BGBCC_CCXL_CompileExprRef(ctx, t);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackUnaryOp(ctx, "++");
			BGBCC_CCXL_StackDupIdx(ctx, 2);
			BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
			BGBCC_CCXL_StackExch(ctx);
			BGBCC_CCXL_StackPop(ctx);
		}

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx,
			BCCX_FetchCst(l, &bgbcc_rcst_expr, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
			BGBCC_CCXL_StackUnaryOpNameB(ctx, "--", s0, 2);
			return;
		}

#if 0
		BGBCC_CCXL_CompileExpr(ctx, t);
//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupB(ctx);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
		BGBCC_CCXL_CompileAssign(ctx, t);
#endif

		if(BGBCC_CCXL_InferExprCleanP(ctx, t))
//		if(0)
		{
			BGBCC_CCXL_CompileExpr(ctx, t);
	//		BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackUnaryOp(ctx, "--");
			BGBCC_CCXL_CompileAssign(ctx, t);
		}else
		{
			BGBCC_CCXL_CompileExprRef(ctx, t);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			BGBCC_CCXL_StackDupB(ctx);
			BGBCC_CCXL_StackUnaryOp(ctx, "--");
			BGBCC_CCXL_StackDupIdx(ctx, 2);
			BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
			BGBCC_CCXL_StackExch(ctx);
			BGBCC_CCXL_StackPop(ctx);
		}

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

#if 0
		if(	ctx->arch_has_predops &&
			!BGBCC_CCXL_CheckIsStaticLib(ctx) &&
			BGBCC_CCXL_InferExprCleanP(ctx, ln) &&
			BGBCC_CCXL_InferExprCleanP(ctx, rn) &&
			BGBCC_CCXL_IsBinaryCompareP(ctx, t))
		{
//			BGBCC_CCXL_CompilePredExpr(ctx, t);
//			BGBCC_CCXL_StackSetPred(ctx, 2);
//			BGBCC_CCXL_CompileStatement(ctx, ln);
//			if(rn)
//			{
//				BGBCC_CCXL_StackSetPred(ctx, 3);
//				BGBCC_CCXL_CompileStatement(ctx, rn);
//			}
//			BGBCC_CCXL_StackSetPred(ctx, 0);
//			return;

			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_CompileCSel(ctx, t);
			return;
		}
#endif
		
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

			BGBCC_CCXL_CompileExprRef(ctx, t);
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

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall_intrin, "funcall_intrin"))
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

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_symbol, "symbol"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_symbol", 0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_keyword, "keyword"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_keyword", 0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_array, "array"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		ln=BCCX_FetchCst(l, &bgbcc_rcst_list, "list");

		if(!s0)
		{
//			BGBCC_CCXL_CompileArgsAsVarArray(ctx, ln);
			BGBCC_CCXL_CompileArgsAsVarArraySig(ctx, ln, "r");
			return;
		}

		s1=NULL;
		
		if(!bgbcc_stricmp(s0, "SB"))	s1="a";
		if(!bgbcc_stricmp(s0, "B"))		s1="a";
		if(!bgbcc_stricmp(s0, "SC"))	s1="c";
		if(!bgbcc_stricmp(s0, "D"))		s1="d";
		if(!bgbcc_stricmp(s0, "F"))		s1="f";
		if(!bgbcc_stricmp(s0, "G"))		s1="g";
		if(!bgbcc_stricmp(s0, "UB"))	s1="h";
		if(!bgbcc_stricmp(s0, "SI"))	s1="i";
		if(!bgbcc_stricmp(s0, "UI"))	s1="j";
		if(!bgbcc_stricmp(s0, "SF"))	s1="k";
		if(!bgbcc_stricmp(s0, "HF"))	s1="k";
		if(!bgbcc_stricmp(s0, "L"))		s1="x";
		if(!bgbcc_stricmp(s0, "UL"))	s1="y";
		if(!bgbcc_stricmp(s0, "LX"))	s1="n";
		if(!bgbcc_stricmp(s0, "ULX"))	s1="o";
		if(!bgbcc_stricmp(s0, "V"))		s1="r";
		if(!bgbcc_stricmp(s0, "SS"))	s1="s";
		if(!bgbcc_stricmp(s0, "US"))	s1="t";
		if(!bgbcc_stricmp(s0, "W"))		s1="w";
		
		if(s1)
		{
			BGBCC_CCXL_CompileArgsAsVarArraySig(ctx, ln, s1);
			return;
		}

		BGBCC_CCXL_CompileArgsAsVarArraySig(ctx, ln, "r");
		return;

//		BGBCC_CCXL_PushMark(ctx);
//		BGBCC_CCXL_StackPushConstString(ctx, s0);
//		BGBCC_CCXL_StackCallName(ctx, "__lvo_keyword", 0);
//		return(1);
	}

	BGBCC_CCXL_Error(ctx, "Unhandled expr %s\n", BCCX_Tag(l));
	BGBCC_CCXL_StackPushConstInt(ctx, 0);
}

void BGBCC_CCXL_CompileExprT(BGBCC_TransState *ctx, BCCX_Node *l)
{
	long long li, lj;
	double g0, g1, g2, g3;
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
		s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
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

		if(s0)
		{
			if(!strcmp(s0, "SB"))
				{ BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 4); return; }
			if(!strcmp(s0, "UB"))
				{ BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 5); return; }
			if(!strcmp(s0, "SS"))
				{ BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 6); return; }
			if(!strcmp(s0, "US"))
				{ BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 7); return; }
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
		s1=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		if(s1)
		{
			if(!strcmp(s1, "L") || !strcmp(s1, "u"))
			{
				BGBCC_CCXL_StackPushConstWString(ctx, s0);
				return;
			}

			if(!strcmp(s1, "u8"))
			{
				BGBCC_CCXL_StackPushConstU8String(ctx, s0);
				return;
			}

			if(!strcmp(s1, "U"))
			{
				BGBCC_CCXL_StackPushConstW4String(ctx, s0);
				return;
			}
		}

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
		f=BCCX_GetFloatCst(l, &bgbcc_rcst_real, "real");
		g=BCCX_GetFloatCst(l, &bgbcc_rcst_imag, "imag");
		BGBCC_CCXL_StackPushConstDouble(ctx, g);

		BGBCC_CCXL_StackPushConstComplex(ctx, f, g);

//		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vec2, "vec2"))
	{
		g0=BCCX_GetFloatCst(l, &bgbcc_rcst_value_x0, "value_x0");
		g1=BCCX_GetFloatCst(l, &bgbcc_rcst_value_x1, "value_x1");
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		i=0;
		if(!strcmp(s0, "V2F"))i=CCXL_REGVEC_TY_V2F;
		if(!strcmp(s0, "V2D"))i=CCXL_REGVEC_TY_V2D;
		if(!strcmp(s0, "V2SI"))i=CCXL_REGVEC_TY_V2SI;
		if(!strcmp(s0, "V2UI"))i=CCXL_REGVEC_TY_V2UI;
		if(!strcmp(s0, "FCPX"))i=CCXL_REGVEC_TY_FCPX;
		if(!strcmp(s0, "DCPX"))i=CCXL_REGVEC_TY_DCPX;
		BGBCC_CCXL_StackPushConstVec2F(ctx, g0, g1, i);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vec4, "vec4"))
	{
		g0=BCCX_GetFloatCst(l, &bgbcc_rcst_value_x0, "value_x0");
		g1=BCCX_GetFloatCst(l, &bgbcc_rcst_value_x1, "value_x1");
		g2=BCCX_GetFloatCst(l, &bgbcc_rcst_value_x2, "value_x2");
		g3=BCCX_GetFloatCst(l, &bgbcc_rcst_value_x3, "value_x3");
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");

		i=0;
		if(!strcmp(s0, "V3F"))i=CCXL_REGVEC_TY_V3F;
		if(!strcmp(s0, "V4F"))i=CCXL_REGVEC_TY_V4F;
		if(!strcmp(s0, "QF"))i=CCXL_REGVEC_TY_QUATF;

		if(!strcmp(s0, "V3FX"))i=CCXL_REGVEC_TY_V3FX;
		if(!strcmp(s0, "V3FQ"))i=CCXL_REGVEC_TY_V3FQ;

		if(!strcmp(s0, "V4SW"))i=CCXL_REGVEC_TY_V4SW;
		if(!strcmp(s0, "V4UW"))i=CCXL_REGVEC_TY_V4UW;
		if(!strcmp(s0, "V4SI"))i=CCXL_REGVEC_TY_V4SI;
		if(!strcmp(s0, "V4UI"))i=CCXL_REGVEC_TY_V4UI;

		BGBCC_CCXL_StackPushConstVec4F(ctx, g0, g1, g2, g3, i);
		return;
	}

	BGBCC_CCXL_CompileForm(ctx, l);
	return;
}

void BGBCC_CCXL_CompileExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	int i;

//	t=BGBCC_CCXL_ReduceExpr(ctx, l);

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_CCXL_CompileExprT(ctx, l);
//	BGBCC_CCXL_CompileExprT(ctx, t);
	ctx->tail=i;
}

void BGBCC_CCXL_CompileExprTail(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	int i;

//	t=BGBCC_CCXL_ReduceExpr(ctx, l);

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_CCXL_CompileExprT(ctx, l);
//	BGBCC_CCXL_CompileExprT(ctx, t);
	ctx->tail=i;
}

int BGBCC_CCXL_CompileExprValueObject(BGBCC_TransState *ctx,
	char *sig, ccxl_type ty, BCCX_Node *l)
{
	BGBCC_CCXL_LiteralInfo *st;
	BCCX_Node *c;
	char *sn, *ss;
	int idx;

//	BGBCC_CCXL_StubError(ctx);
	
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_list, "list"))
	{
		BGBCC_CCXL_StackPushTempObj(ctx, sig);

		st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);

		c=BCCX_Child(l); idx=0;
		while(c)
		{
			sn=st->decl->fields[idx]->name;
			ss=st->decl->fields[idx]->sig;
		
			BGBCC_CCXL_StackDupClean(ctx);

			BGBCC_CCXL_CompileExprAsTypeSig(ctx, ss, c);
			BGBCC_CCXL_StackExch(ctx);

			BGBCC_CCXL_StackStoreSlot(ctx, sn);

			idx++;

			c=BCCX_Next(c);
		}

		return(0);
	}

	BGBCC_CCXL_CompileExpr(ctx, l);
	return(1);
}

void BGBCC_CCXL_CompileExprAsTypeSig(BGBCC_TransState *ctx,
	char *sig, BCCX_Node *l)
{
	BCCX_Node *an[64];
	double afv[64];
	BCCX_Node *c, *t;
	char *vfn;
	ccxl_type bty;
	int i, j, k, na, vty;
	
	BGBCC_CCXL_TypeFromSig(ctx, &bty, sig);

//	printf("BGBCC_CCXL_CompileExprAsTypeSig: %s\n", sig);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
	{
		BGBCC_CCXL_CompileExprValueObject(ctx, sig, bty, l);
		return;
	}

	if(BGBCC_CCXL_TypeVecP(ctx, bty))
	{
		vty=0;
		switch(bty.val)
		{
			case CCXL_TY_VEC2F:		vty=CCXL_REGVEC_TY_V2F;		break;
			case CCXL_TY_VEC3F:		vty=CCXL_REGVEC_TY_V3F;		break;
			case CCXL_TY_VEC3FQ:	vty=CCXL_REGVEC_TY_V3FQ;	break;
			case CCXL_TY_VEC3FX:	vty=CCXL_REGVEC_TY_V3FX;	break;
			case CCXL_TY_VEC4F:		vty=CCXL_REGVEC_TY_V4F;		break;
			case CCXL_TY_QUATF:		vty=CCXL_REGVEC_TY_QUATF;	break;
			case CCXL_TY_VEC2D:		vty=CCXL_REGVEC_TY_V2D;		break;
			case CCXL_TY_FCOMPLEX:	vty=CCXL_REGVEC_TY_FCPX;	break;
			case CCXL_TY_DCOMPLEX:	vty=CCXL_REGVEC_TY_DCPX;	break;
			case CCXL_TY_VEC2SI:	vty=CCXL_REGVEC_TY_V2SI;	break;
			case CCXL_TY_VEC2UI:	vty=CCXL_REGVEC_TY_V2UI;	break;
			case CCXL_TY_VEC4SW:	vty=CCXL_REGVEC_TY_V4SW;	break;
			case CCXL_TY_VEC4UW:	vty=CCXL_REGVEC_TY_V4UW;	break;
			case CCXL_TY_VEC4SI:	vty=CCXL_REGVEC_TY_V4SI;	break;
			case CCXL_TY_VEC4UI:	vty=CCXL_REGVEC_TY_V4UI;	break;
		}

		vfn=NULL;
		switch(bty.val)
		{
			case CCXL_TY_VEC2F:		vfn="__v2f_float2";			break;
			case CCXL_TY_VEC3F:		vfn="__v3f_float3";			break;
			case CCXL_TY_VEC3FQ:	vfn="__v3fq_float3";		break;
			case CCXL_TY_VEC3FX:	vfn="__v3fx_float3";		break;
			case CCXL_TY_VEC4F:		vfn="__v4f_float4";			break;
			case CCXL_TY_QUATF:		vfn="__vqf_float4";			break;
			case CCXL_TY_VEC2D:		vfn="__v2d_double2";		break;
			case CCXL_TY_FCOMPLEX:	vfn="__c2f_float2";			break;
			case CCXL_TY_DCOMPLEX:	vfn="__c2d_double2";		break;
			case CCXL_TY_VEC2SI:	vfn="__m64_int2";			break;
			case CCXL_TY_VEC2UI:	vfn="__m64_int2";			break;
			case CCXL_TY_VEC4SW:	vfn="__m64_word4";			break;
			case CCXL_TY_VEC4UW:	vfn="__m64_word4";			break;
			case CCXL_TY_VEC4SI:	vfn="__m128_int4";			break;
			case CCXL_TY_VEC4UI:	vfn="__m128_int4";			break;
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
			
			if(na>=64)
			{
				BGBCC_DBGBREAK
			}
			
			for(i=0; i<na; i++)
			{
				if(!BGBCC_CCXL_IsRealP(ctx, an[i]))
					break;
				afv[i]=BCCX_GetFloatCst(an[i], &bgbcc_rcst_value, "value");
			}
			
			if((i>=na) && (na>1) && (na<=4))
			{
				if(na==2)
				{
					BGBCC_CCXL_StackPushConstVec2F(ctx,
						afv[0], afv[1], vty);
				}else
				{
					BGBCC_CCXL_StackPushConstVec4F(ctx,
						afv[0], afv[1], afv[2], afv[3], vty);
				}
				
				return;
			}

			if(vfn)
			{
				BGBCC_CCXL_CompileArgsFuncallName(ctx,
					vfn, BCCX_Child(l));
				return;
			}

			BGBCC_CCXL_StubError(ctx);
			return;
		}
	}

	BGBCC_CCXL_CompileExpr(ctx, l);
	BGBCC_CCXL_StackCastSig(ctx, sig);
}

void BGBCC_CCXL_CompileExprAsType(BGBCC_TransState *ctx,
	BCCX_Node *ty, BCCX_Node *l)
{
	BGBCC_CCXL_LiteralInfo *sti;
	char *s;
	int i;

	s=BGBCC_CCXL_VarTypeString(ctx, ty);

	if(*s=='U')
	{
		sti=BGBCC_CCXL_LookupTypedefForSig2(ctx, s);
		if(sti)
			{ s=BGBCC_CCXL_TypeGetSig(ctx, sti->decl->type); }
	}

	BGBCC_CCXL_CompileExprAsTypeSig(ctx, s, l);

//	BGBCC_CCXL_CompileExpr(ctx, l);
//	s=BGBCC_CCXL_VarTypeString(ctx, ty);
//	BGBCC_CCXL_StackCastSig(ctx, s);
}

