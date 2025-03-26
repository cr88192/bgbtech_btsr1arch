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
	BCCX_Node *an[64];
	int an_bo[64], an_bsz[64];
	BCCX_Node *c, *ct, *cv, *t, *v, *ln, *rn, *ln1, *rn1, *ln2, *rn2;
	BCCX_Node *ct2, *cv2;
	ccxl_type dty, tty;
	char *s;
	int i0, i1, i2, i3;
	int ci, na, sz, tbo;
	int i, j, k;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		s=BGBCC_CCXL_CompileRemapName(ctx, s);
		BGBCC_CCXL_PopStore(ctx, s);
		BGBCC_CCXL_VlMarkRefOut(ctx, s);
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

		if(BCCX_TagIsCstP(cv, &bgbcc_rcst_colon, "colon"))
		{
			ln1=BCCX_FetchCst(cv, &bgbcc_rcst_left, "left");
			rn1=BCCX_FetchCst(cv, &bgbcc_rcst_right, "right");
			ln1=BGBCC_CCXL_ReduceExpr(ctx, ln1);
			rn1=BGBCC_CCXL_ReduceExpr(ctx, rn1);

			if(BGBCC_CCXL_IsIntP(ctx, ln1) && BGBCC_CCXL_IsIntP(ctx, rn1))
			{
				i0=BCCX_GetIntCst(ln1, &bgbcc_rcst_value, "value");
				i1=BCCX_GetIntCst(rn1, &bgbcc_rcst_value, "value");
				if(i0<i1)
					{ k=i0; i0=i1; i1=k; }
				
				if(BCCX_TagIsCstP(ct, &bgbcc_rcst_ref, "ref"))
				{
					s=BCCX_GetCst(ct, &bgbcc_rcst_name, "name");
					s=BGBCC_CCXL_CompileRemapName(ctx, s);
					BGBCC_CCXL_StackBitStoreRef(ctx, i1, i0, s);
					BGBCC_CCXL_VlMarkRefOut(ctx, s);
					return;
				}else
				{
					BGBCC_CCXL_CompileExpr(ctx, ct);
					BGBCC_CCXL_StackBitStore(ctx, i1, i0);
					BGBCC_CCXL_CompileAssign(ctx, ct);
					return;
				}
			}
		}
		
		BGBCC_CCXL_CompileExpr(ctx, ct);
		BGBCC_CCXL_CompileExpr(ctx, cv);
		BGBCC_CCXL_StackStoreIndex(ctx);
		return;
	}
	
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_object, "object"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		s=BGBCC_CCXL_VarTypeString(ctx, t);
		BGBCC_CCXL_TypeFromSig(ctx, &dty, s);

//		c=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		c=BCCX_FindTagCst(l, &bgbcc_rcst_list, "list");
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_list, "list") &&
			BGBCC_CCXL_TypeBitIntP(ctx, dty))
		{
			tbo=0;
			na=BCCX_GetNodeChildCount(c);
//			for(ci=0; ci<na; ci++)
			for(ci=na-1; ci>=0; ci--)
			{
				v=BCCX_GetNodeIndex(c, ci);

				v=BGBCC_CCXL_ReduceExpr(ctx, v);
				BGBCC_CCXL_InferExpr(ctx, v, &tty);
				sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, tty);
				if(sz<=0)
					{ BGBCC_CCXL_TagError(ctx, CCXL_TERR_BADTYPEDEST); }

				an[ci]=v;
				an_bo[ci]=tbo;
				an_bsz[ci]=sz;
				tbo+=sz;
			}

			for(ci=0; ci<na; ci++)
			{
				if(BGBCC_CCXL_IsIntP(ctx, an[ci]))
					continue;

				if(BCCX_TagIsCstP(an[ci], &bgbcc_rcst_ref, "ref"))
				{
					s=BCCX_GetCst(an[ci], &bgbcc_rcst_name, "name");
					s=BGBCC_CCXL_CompileRemapName(ctx, s);
					BGBCC_CCXL_StackDupClean(ctx);
					BGBCC_CCXL_StackBitLoadStRef(ctx,
						an_bo[ci], an_bo[ci]+an_bsz[ci]-1,
						s);
					BGBCC_CCXL_VlMarkRefOut(ctx, s);
					continue;
				}

				BGBCC_CCXL_StackDupClean(ctx);
				BGBCC_CCXL_StackBitLoad(ctx,
					an_bo[ci], an_bo[ci]+an_bsz[ci]-1);
				BGBCC_CCXL_CompileAssign(ctx, an[ci]);
			}
			BGBCC_CCXL_StackPop(ctx);

//				BGBCC_CCXL_CompileExpr(ctx, v);
			return;
		}
	}

	BGBCC_CCXL_Error(ctx, "Bad lvalue in expression\n");
}

#if 0
void BGBCC_CCXL_CompileExprList(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
//		cur=BCCX_Next(cur);
	}
}

void BGBCC_CCXL_CompileExprListReverse(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i;

	i=0; cur=lst;
//	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
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
//	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)
	{
//		BGBCC_CCXL_CompileExpr(ctx, stk[i]);
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
		BGBCC_CCXL_CompileExpr(ctx, n);
	}
}
#endif

#if 1
void BGBCC_CCXL_CompileExprListB(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;
	int ci, na;

	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		cur=BCCX_GetNodeIndex(lst, ci);
		BGBCC_CCXL_CompileExpr(ctx, cur);
	}
}

void BGBCC_CCXL_CompileExprListReverseB(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur, *n;
	int ci, na;

	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		cur=BCCX_GetNodeIndex(lst, na-ci-1);
		n=BGBCC_CCXL_ReduceExpr(ctx, cur);
		BGBCC_CCXL_CompileExpr(ctx, n);
	}
}

void BGBCC_CCXL_CompileArgsListB(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur, *n;
	int ci, na;

	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		cur=BCCX_GetNodeIndex(lst, na-ci-1);
		n=BGBCC_CCXL_ReduceExpr(ctx, cur);
		BGBCC_CCXL_CompileExpr(ctx, n);
	}
}
#endif


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
		(ctx->lang==BGBCC_LANG_BS)	||
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
		(ctx->lang==BGBCC_LANG_BS)	||
		(ctx->lang==BGBCC_LANG_JAVA)	)
	{
		if(!bgbcp_strcmp(name, "null"))
		{
			return(bgbcc_strdup("__object_null"));
		}
	}
	
	if(	(ctx->lang==BGBCC_LANG_BS2)	||
		(ctx->lang==BGBCC_LANG_BS)	||
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
//			BGBCC_CCXL_CompileExprListReverse(ctx,
//				BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_CompileExprListReverseB(ctx,
				BCCX_FindTagCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_StackVaEnd(ctx);
			if(!(flag&1) && !dname)
				BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		if(!strcmp(s0, "__builtin_va_start"))
		{
//			BGBCC_CCXL_CompileExprListReverse(ctx,
//				BCCX_FetchCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_CompileExprListReverseB(ctx,
				BCCX_FindTagCst(l, &bgbcc_rcst_args, "args"));
			BGBCC_CCXL_StackVaStart(ctx);
			if(!(flag&1) && !dname)
				BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		if(!strcmp(s0, "__builtin_va_arg"))
		{
			BGBCC_CCXL_CompileExprListReverseB(ctx,
				BCCX_FindTagCst(l, &bgbcc_rcst_args, "args"));
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
		BGBCC_CCXL_CompileExprListReverseB(ctx,
			BCCX_FindTagCst(l, &bgbcc_rcst_args, "args"));
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
	BGBCC_CCXL_CompileExprListReverseB(ctx,
		BCCX_FindTagCst(l, &bgbcc_rcst_args, "args"));
	BGBCC_CCXL_CompileExpr(ctx, c);
//	BGBCC_CCXL_StackPopCall(ctx, flag);
	BGBCC_CCXL_StackPopCall2(ctx, dname, flag);
	return;
}

void BGBCC_CCXL_CompileStkAsVarList(BGBCC_TransState *ctx,
	BCCX_Node **stk, int na)
{
	BCCX_Node *n;
	int i;

	BGBCC_CCXL_PushLoad(ctx, "__object_null");
	i=na;
	while(i--)
	{
		n=BGBCC_CCXL_ReduceExpr(ctx, stk[i]);
		BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", n);
		BGBCC_CCXL_StackBinaryOp(ctx, "CONS");
	}
}

void BGBCC_CCXL_CompileArgsAsVarListB(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *stk[512];
	BCCX_Node *cur, *n;
	int i, na, ci;

	i=0; cur=l;
//	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }

	na=BCCX_GetNodeChildCount(l);
	for(ci=0; ci<na; ci++)
	{
		cur=BCCX_GetNodeIndex(l, ci);
		stk[i++]=cur;
	}

	BGBCC_CCXL_CompileStkAsVarList(ctx, stk, i);
}

void BGBCC_CCXL_CompileArgsAsVarArrayB(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i, na, ci;

//	cur=l; na=0;
//	while(cur) { na++; cur=BCCX_Next(cur); }
	na=BCCX_GetNodeChildCount(l);

	if(na>8)
	{
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileArgsAsVarListB(ctx, l);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_newvararray_nlist", 0);
	}else
	{
		i=0; cur=l;
//		while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
//		na=i;

		for(ci=0; ci<na; ci++)
			{ stk[i++]=BCCX_GetNodeIndex(l, ci); }

		BGBCC_CCXL_PushMark(ctx);

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

	if((*sig=='r') && (na>8))
	{
		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileStkAsVarList(ctx, stk, na);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_newvararray_nlist", 0);
		return;
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
//	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
//	na=i;
	na=BCCX_GetNodeChildCount(l);
	for(i=0; i<na; i++)
		{ stk[i]=BCCX_GetNodeIndex(l, i); }

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
//	u=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
	u=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");

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
			BGBCC_CCXL_CompileExprListReverseB(ctx, u);
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
		if(	(ctx->lang==BGBCC_LANG_BS2) ||
			(ctx->lang==BGBCC_LANG_BS)	)
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
		BGBCC_CCXL_CompileArgsAsVarArrayB(ctx, u);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		BGBCC_CCXL_CompileExpr(ctx, v);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_methodcall", 0);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);
	BGBCC_CCXL_CompileExprListReverseB(ctx, u);

	sig=BGBCC_CCXL_StackGetSigCallArgs(ctx);

	BGBCC_CCXL_CompileExpr(ctx, v);
	BGBCC_CCXL_StackDup(ctx);

//	BGBCC_CCXL_StackLoadSlot(ctx, s0);
	BGBCC_CCXL_StackLoadSlotSig(ctx, s0, sig);

	BGBCC_CCXL_StackPopCall(ctx, 4);
	return;
}

void BGBCC_CCXL_CompileArgsFuncallNameB(BGBCC_TransState *ctx,
	char *fname, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *stk[128];
	BCCX_Node *cur, *n;
	int i, na;

	BGBCC_CCXL_PushMark(ctx);

	BGBCC_CCXL_CompileExprListReverseB(ctx, l);

#if 0
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
#endif

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
		if(	(ctx->lang==BGBCC_LANG_BS2) ||
			(ctx->lang==BGBCC_LANG_BS)	)
		{
			if(!strcmp(name, "toString"))
			{
				BGBCC_CCXL_PushMark(ctx);
				BGBCC_CCXL_CompileExpr(ctx, l);
				BGBCC_CCXL_StackCallName(ctx, "__lvo_tostring", 0);
				return(1);
			}
		}

		sprintf(tb, "__temp_fidcache__%s", name);

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackLoadAddrVSig(ctx, tb, "t");
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackLoadSlotSig(ctx, tb, "()");
//		BGBCC_CCXL_StackPopCall(ctx, 0);
//		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot", 0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot_cst", 0);
		return(1);
	}
	
	if(BGBCC_CCXL_TypeRefArrayP(ctx, sty))
	{
		if(	(!strcmp(name, "length") && (ctx->lang==BGBCC_LANG_BS2)) ||
			(!strcmp(name, "length") && (ctx->lang==BGBCC_LANG_BS)) ||
			(!strcmp(name, "Length") && (ctx->lang==BGBCC_LANG_CS)))
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExpr(ctx, l);
			BGBCC_CCXL_StackCallName(ctx, "__lvo_get_length", 0);
			return(1);
		}

		sprintf(tb, "__temp_fidcache__%s", name);

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackLoadAddrVSig(ctx, tb, "t");
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot", 0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot_cst", 0);
		return(1);
	}

	if(BGBCC_CCXL_TypeRefStringP(ctx, sty))
	{
		if(	(!strcmp(name, "length") && (ctx->lang==BGBCC_LANG_BS2)) ||
			(!strcmp(name, "length") && (ctx->lang==BGBCC_LANG_BS)) ||
			(!strcmp(name, "Length") && (ctx->lang==BGBCC_LANG_CS)))
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExpr(ctx, l);
			BGBCC_CCXL_StackCallName(ctx, "__lvo_get_length", 0);
			return(1);
		}

		if(	(ctx->lang==BGBCC_LANG_BS2) ||
			(ctx->lang==BGBCC_LANG_BS))
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

		sprintf(tb, "__temp_fidcache__%s", name);

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackLoadAddrVSig(ctx, tb, "t");
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot", 0);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_getslot_cst", 0);
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
		sprintf(tb, "__temp_fidcache__%s", name);

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
		BGBCC_CCXL_StackLoadAddrVSig(ctx, tb, "t");
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackLoadSlotSig(ctx, tb, "()");
//		BGBCC_CCXL_StackPopCall(ctx, 0);
//		BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot", 1);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot_cst", 1);
		BGBCC_CCXL_StackPop(ctx);
		return(1);
	}
	
	if(BGBCC_CCXL_TypeRefArrayP(ctx, sty) ||
		BGBCC_CCXL_TypeRefStringP(ctx, sty))
	{
		sprintf(tb, "__temp_fidcache__%s", name);

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
		BGBCC_CCXL_StackLoadAddrVSig(ctx, tb, "t");
		BGBCC_CCXL_StackPushConstString(ctx, name);
		BGBCC_CCXL_CompileExpr(ctx, l);
//		BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot", 1);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot_cst", 1);
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

void BGBCC_CCXL_CompileNewArray(BGBCC_TransState *ctx,
	BCCX_Node *type, BCCX_Node *args, int isj)
{
	char tb[256];
	int asz[16];
	char *s0, *s1, *s2, *t0;
	BCCX_Node *c, *n, *ln, *rn;
	int nasz, na, ci;
	int i, j, k;

	c=args; nasz=0;
//	while(c)
//	{

	na=BCCX_GetNodeChildCount(args);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(args, ci);

		n=BGBCC_CCXL_ReduceExprConst(ctx, c);
		if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
			break;

		i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
		if(i<0)
			break;

		asz[nasz++]=i;
//		c=BCCX_Next(c);
	}

	if(!c)
	{
		s0=BGBCC_CCXL_VarTypeString(ctx, type);
		
		if((nasz==1) && (isj&1))
		{
			s1=NULL;
			switch(s0[0])
			{
			case 'a':	s1="__lvo_newarray_sb_sz1"; break;
			case 'b':	s1="__lvo_newarray_ub_sz1"; break;
			case 'c':	s1="__lvo_newarray_sb_sz1"; break;
			case 'd':	s1="__lvo_newarray_d_sz1"; break;
//			case 'e':	s1="__lvo_newarray_d_sz1"; break;
			case 'f':	s1="__lvo_newarray_f_sz1"; break;
//			case 'g':	s1="__lvo_newarray_g_sz1"; break;
			case 'h':	s1="__lvo_newarray_ub_sz1"; break;
			case 'i':	s1="__lvo_newarray_si_sz1"; break;
			case 'j':	s1="__lvo_newarray_ui_sz1"; break;
//			case 'k':	s1="__lvo_newarray_ui_sz1"; break;
			case 'l':	s1="__lvo_newarray_sl_sz1"; break;
			case 'm':	s1="__lvo_newarray_ul_sz1"; break;
//			case 'n':	s1="__lvo_newarray_ui_sz1"; break;
//			case 'o':	s1="__lvo_newarray_ui_sz1"; break;
//			case 'p':	s1="__lvo_newarray_ui_sz1"; break;
//			case 'q':	s1="__lvo_newarray_ui_sz1"; break;
			case 'r':	s1="__lvo_newarray_var_sz1"; break;
			case 's':	s1="__lvo_newarray_ss_sz1"; break;
			case 't':	s1="__lvo_newarray_us_sz1"; break;
//			case 'u':	s1="__lvo_newarray_us_sz1"; break;
//			case 'v':	s1="__lvo_newarray_us_sz1"; break;
			case 'w':	s1="__lvo_newarray_us_sz1"; break;
			case 'x':	s1="__lvo_newarray_sl_sz1"; break;
			case 'y':	s1="__lvo_newarray_ul_sz1"; break;
//			case 'z':	s1="__lvo_newarray_ul_sz1"; break;

			case 'P':	s1="__lvo_newarray_ptr_sz1"; break;
			case 'Q':	s1="__lvo_newarray_var_sz1"; break;
			}
			
			if(s1)
			{
				BGBCC_CCXL_PushMark(ctx);
				BGBCC_CCXL_StackPushConstInt(ctx, asz[0]);
				BGBCC_CCXL_StackCallName2(ctx, s1, NULL, 0);
				return;
			}

			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, asz[0]);
			BGBCC_CCXL_StackPushConstString(ctx, s0);
			BGBCC_CCXL_StackCallName2(ctx, "__lvo_newarray_sig_sz1", NULL, 0);
			return;
		}
		

#if 1
		s0=BGBCC_CCXL_VarTypeString(ctx, type);
		
		t0=tb;
		*t0++='A';
		for(j=0; j<nasz; j++)
		{
			i=asz[j];
			t0=BGBCC_CCXL_StrPrintInt(t0, i);
			if((j+1)<nasz)
				*t0++=',';
		}
		*t0++=';';
		strcpy(t0, s0);
		
		s0=bgbcc_strdup(tb);

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_StackSizeofSig(ctx, s0);
		BGBCC_CCXL_StackCastSig(ctx, "m");
		BGBCC_CCXL_StackCallName2(ctx, "__operator_new", NULL, 0);
		return;
#endif
	}


	BGBCC_CCXL_StubError(ctx);
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

//	BGBCC_CCXL_PushMark(ctx);
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
			BGBCC_CCXL_CompileExprListReverseB(ctx, args);
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
		BGBCC_CCXL_VlMarkRefIn(ctx, s0);
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
		ln=BCCX_FetchCst(t, &bgbcc_rcst_array, "array");
		rn=BCCX_FetchCst(t, &bgbcc_rcst_index, "index");
		BGBCC_CCXL_CompileExpr(ctx, ln);
		BGBCC_CCXL_CompileExpr(ctx, rn);
		BGBCC_CCXL_StackLoadIndexAddr(ctx);
		return;
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "*"))
	{
		v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_CompileExpr(ctx, v);
//		BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
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
	char tb[128], tb1[128], tb2[128];
	byte *ips[64];
	ccxl_label l0, l1, l2, l3;
	ccxl_register sreg, treg;
	ccxl_type bty, dty, sty, tty, lty, rty, lty2, rty2;
	BGBCC_CCXL_LiteralInfo *obj;
	char *s, *s0, *s1, *s2, *s3;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn, *ln1, *rn1, *ln2, *rn2, *n;
	BCCX_Node *ct1, *cv1, *ct2, *cv2;
	int i0, i1, i2, i3, na, ci, opr;
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

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_instanceof, "instanceof"))
	{
		s0=BGBCC_CCXL_VarTypeString(ctx,
			BCCX_FindTagCst(l, &bgbcc_rcst_type, "type"));
		t=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		s1=NULL;
		if(!strcmp(s0, "Gx"))	s1="__lvo_fixnump";
		if(!strcmp(s0, "Gy"))	s1="__lvo_flonump";
		if(!strcmp(s0, "Cs"))	s1="__lvo_stringp";

		if(s1)
		{
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", t);
			BGBCC_CCXL_StackCallName(ctx, s1, 0);
			return;
		}

		BGBCC_CCXL_PushMark(ctx);
//		BGBCC_CCXL_CompileExprListReverse(ctx, nl);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", t);
		BGBCC_CCXL_StackCallName(ctx, "__lvo_instanceof_sig", 0);
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

#if 0
		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_getindex, "getindex"))
		{
			k=-1;
		}

		if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_getindex, "getindex") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_getindex, "getindex"))
		{
			ct1=BCCX_FetchCst(ln, &bgbcc_rcst_array, "array");
			cv1=BCCX_FetchCst(ln, &bgbcc_rcst_index, "index");
			ct1=BGBCC_CCXL_ReduceExpr(ctx, ct1);
			cv1=BGBCC_CCXL_ReduceExpr(ctx, cv1);
			
			ct2=BCCX_FetchCst(rn, &bgbcc_rcst_array, "array");
			cv2=BCCX_FetchCst(rn, &bgbcc_rcst_index, "index");
			ct2=BGBCC_CCXL_ReduceExpr(ctx, ct2);
			cv2=BGBCC_CCXL_ReduceExpr(ctx, cv2);
			
			if(	BCCX_TagIsCstP(cv1, &bgbcc_rcst_colon, "colon") &&
				BCCX_TagIsCstP(cv2, &bgbcc_rcst_colon, "colon")	)
			{
				ln1=BCCX_FetchCst(cv1, &bgbcc_rcst_left, "left");
				rn1=BCCX_FetchCst(cv1, &bgbcc_rcst_right, "right");
				ln1=BGBCC_CCXL_ReduceExpr(ctx, ln1);
				rn1=BGBCC_CCXL_ReduceExpr(ctx, rn1);

				ln2=BCCX_FetchCst(cv2, &bgbcc_rcst_left, "left");
				rn2=BCCX_FetchCst(cv2, &bgbcc_rcst_right, "right");
				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);

				if(	BGBCC_CCXL_IsIntP(ctx, ln1) &&
					BGBCC_CCXL_IsIntP(ctx, rn1) &&
					BGBCC_CCXL_IsIntP(ctx, ln2) &&
					BGBCC_CCXL_IsIntP(ctx, rn2))
				{
					i0=BCCX_GetIntCst(ln1, &bgbcc_rcst_value, "value");
					i1=BCCX_GetIntCst(rn1, &bgbcc_rcst_value, "value");
					i2=BCCX_GetIntCst(ln2, &bgbcc_rcst_value, "value");
					i3=BCCX_GetIntCst(rn2, &bgbcc_rcst_value, "value");
					if(i0<i1)
						{ k=i0; i0=i1; i1=k; }
					if(i2<i3)
						{ k=i2; i2=i3; i3=k; }
					
					if(((i0-i1)==(i2-i3)) && ((i0-i1)>0))
					{
						BGBCC_CCXL_CompileExpr(ctx, ct2);
						BGBCC_CCXL_CompileExpr(ctx, ct1);
						BGBCC_CCXL_StackBitMove(ctx, i1, i0, i3, i2);
						BGBCC_CCXL_CompileAssign(ctx, ct1);
						return;
					}
				}
			}
		}
#endif

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

		BGBCC_CCXL_InferExpr(ctx, ln, &lty);

		if(BGBCC_CCXL_CheckCompileLoadIndex(ctx, ln, rn))
			return;

		if(BCCX_TagIsCstP(rn, &bgbcc_rcst_colon, "colon"))
		{
			ln2=BCCX_FetchCst(rn, &bgbcc_rcst_left, "left");
			rn2=BCCX_FetchCst(rn, &bgbcc_rcst_right, "right");
			ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
			rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);

			if(BGBCC_CCXL_IsIntP(ctx, ln2) && BGBCC_CCXL_IsIntP(ctx, rn2))
			{
				i=BCCX_GetIntCst(ln2, &bgbcc_rcst_value, "value");
				j=BCCX_GetIntCst(rn2, &bgbcc_rcst_value, "value");
				if(i<j)
					{ k=i; i=j; j=k; }
				
				BGBCC_CCXL_CompileExpr(ctx, ln);
//				BGBCC_CCXL_StackLoadIndexConst(ctx, i);
				BGBCC_CCXL_StackBitLoad(ctx, j, i);
				return;
			}
		}
		
		if(BGBCC_CCXL_TypeBitIntP(ctx, lty))
		{
			if(BGBCC_CCXL_IsIntP(ctx, rn))
			{
				i=BCCX_GetIntCst(ln2, &bgbcc_rcst_value, "value");
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_StackBitLoad(ctx, i, i);
				return;
			}
			
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_StackBinaryOp(ctx, ">>");
			BGBCC_CCXL_StackPushConstInt(ctx, 1);
			BGBCC_CCXL_StackBinaryOp(ctx, "&");
			return;
		}

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
			BGBCC_CCXL_VlMarkRefIn(ctx, s0);
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
			BGBCC_CCXL_VlMarkRefIn(ctx, s0);
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
			BGBCC_CCXL_VlMarkRefIn(ctx, s0);
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
			BGBCC_CCXL_VlMarkRefIn(ctx, s0);
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

#if 1
		if(	BGBCC_CCXL_InferExprCleanP(ctx, ln) &&
			BGBCC_CCXL_InferExprCleanP(ctx, rn))
		{
			if(	BGBCC_CCXL_IsBinaryP(ctx, t, "<") ||
				BGBCC_CCXL_IsBinaryP(ctx, t, ">") ||
				BGBCC_CCXL_IsBinaryP(ctx, t, "<=") ||
				BGBCC_CCXL_IsBinaryP(ctx, t, ">="))
			{
				ln2=BCCX_FetchCst(t, &bgbcc_rcst_then, "then");
				rn2=BCCX_FetchCst(t, &bgbcc_rcst_else, "else");
				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
				s1=NULL;
				
				if(	BGBCC_CCXL_InferExprIsEquivP(ctx, ln, ln2) &&
					BGBCC_CCXL_InferExprIsEquivP(ctx, rn, rn2))
				{
					if(	BGBCC_CCXL_IsBinaryP(ctx, t, "<") ||
						BGBCC_CCXL_IsBinaryP(ctx, t, "<="))
					{
						if(	!strcmp(s0, "i") ||
							!strcmp(s0, "s") ||
							!strcmp(s0, "t") ||
							!strcmp(s0, "w") ||
							!strcmp(s0, "c") ||
							!strcmp(s0, "a") ||
							!strcmp(s0, "h"))
						{
							s1="__int_min";
						}

						if(	!strcmp(s0, "l") ||
							!strcmp(s0, "x") ||
							!strcmp(s0, "j") )
						{
							s1="__int64_min";
						}

						if(	!strcmp(s0, "m") ||
							!strcmp(s0, "y") )
						{
							s1="__uint64_min";
						}
					}

					if(	BGBCC_CCXL_IsBinaryP(ctx, t, ">") ||
						BGBCC_CCXL_IsBinaryP(ctx, t, ">="))
					{
						if(	!strcmp(s0, "i") ||
							!strcmp(s0, "s") ||
							!strcmp(s0, "t") ||
							!strcmp(s0, "w") ||
							!strcmp(s0, "c") ||
							!strcmp(s0, "a") ||
							!strcmp(s0, "h"))
						{
							s1="__int_max";
						}

						if(	!strcmp(s0, "l") ||
							!strcmp(s0, "x") ||
							!strcmp(s0, "j") )
						{
							s1="__int64_max";
						}

						if(	!strcmp(s0, "m") ||
							!strcmp(s0, "y") )
						{
							s1="__uint64_max";
						}
					}
				}

				if(s1)
				{
					BGBCC_CCXL_PushMark(ctx);
					BGBCC_CCXL_CompileExpr(ctx, ln);
					BGBCC_CCXL_CompileExpr(ctx, rn);
					BGBCC_CCXL_StackCallName(ctx, s1, 0);
	//				BGBCC_CCXL_StackCallName2(ctx, s1, dname, 0);
					return;
				}
			}
		}
#endif

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
//		c=BCCX_Child(l);
//		while(1)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

//			if(BCCX_Next(c))
			if((ci+1)<na)
			{
				BGBCC_CCXL_CompileStatement(ctx, c);
//				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CCXL_CompileExprT(ctx, c);
//			break;
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

			v=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

#if 1
			if(BCCX_TagIsCstP(v, &bgbcc_rcst_postinc, "postinc"))
			{
				t=BGBCC_CCXL_ReduceExpr(ctx,
					BCCX_FetchCst(v, &bgbcc_rcst_expr, "expr"));

#if 1
				if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
				{
					s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
					s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
//					BGBCC_CCXL_StackUnaryOpNameB(ctx, "++", s0, 2);
//					BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
					BGBCC_CCXL_VlMarkRefIn(ctx, s0);

					BGBCC_CCXL_PushLoad(ctx, s0);
//					BGBCC_CCXL_StackDupB(ctx);
					BGBCC_CCXL_StackDupClean(ctx);
					BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
					BGBCC_CCXL_StackExch(ctx);
					BGBCC_CCXL_StackUnaryOpStore(ctx, "++", s0);
					return;
				}
#endif

				if(BGBCC_CCXL_InferExprCleanP(ctx, t))
				{
					BGBCC_CCXL_CompileExpr(ctx, t);
//					BGBCC_CCXL_StackDupB(ctx);
					BGBCC_CCXL_StackDupClean(ctx);
					BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
					BGBCC_CCXL_StackExch(ctx);
					BGBCC_CCXL_StackUnaryOp(ctx, "++");
					BGBCC_CCXL_CompileAssign(ctx, t);
					return;
				}
			}
#endif

#if 1
			if(BCCX_TagIsCstP(v, &bgbcc_rcst_postdec, "postdec"))
			{
				t=BGBCC_CCXL_ReduceExpr(ctx,
					BCCX_FetchCst(v, &bgbcc_rcst_expr, "expr"));

#if 1
				if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
				{
					s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
					s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
//					BGBCC_CCXL_StackUnaryOpNameB(ctx, "++", s0, 2);
//					BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
					BGBCC_CCXL_VlMarkRefIn(ctx, s0);

					BGBCC_CCXL_PushLoad(ctx, s0);
//					BGBCC_CCXL_StackDupB(ctx);
					BGBCC_CCXL_StackDupClean(ctx);
					BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
					BGBCC_CCXL_StackExch(ctx);
					BGBCC_CCXL_StackUnaryOpStore(ctx, "--", s0);
					return;
				}
#endif

				if(BGBCC_CCXL_InferExprCleanP(ctx, t))
				{
					BGBCC_CCXL_CompileExpr(ctx, t);
//					BGBCC_CCXL_StackDupB(ctx);
					BGBCC_CCXL_StackDupClean(ctx);
					BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
					BGBCC_CCXL_StackExch(ctx);
					BGBCC_CCXL_StackUnaryOp(ctx, "--");
					BGBCC_CCXL_CompileAssign(ctx, t);
					return;
				}
			}
#endif

			BGBCC_CCXL_CompileExpr(ctx, v);
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
			(ctx->lang==BGBCC_LANG_BS2) ||
			(ctx->lang==BGBCC_LANG_BS)	)
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
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);
		
		if((ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_BS2) ||
			(ctx->lang==BGBCC_LANG_BS)	)
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

#if 1
		if(	!strcmp(s0, "+") &&
			!BGBCC_CCXL_CheckIsStaticLib(ctx) &&
			BGBCC_CCXL_IsBinaryP(ctx, rn, "*"))
		{
			ln2=BCCX_FetchCst(rn, &bgbcc_rcst_left, "left");
			rn2=BCCX_FetchCst(rn, &bgbcc_rcst_right, "right");
			ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
			rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
			BGBCC_CCXL_InferExpr(ctx, ln2, &lty2);
			BGBCC_CCXL_InferExpr(ctx, rn2, &rty2);

			BGBCC_CCXL_GetTypeBinaryDest(ctx,
				CCXL_BINOP_MUL, lty2, rty2, &bty);
			BGBCC_CCXL_GetTypeBinaryDest(ctx,
				CCXL_BINOP_ADD, lty, bty, &dty);

			if(	(	BGBCC_CCXL_TypeSmallIntP(ctx, dty) &&
					BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) &&
					ctx->arch_has_imac	) ||
				(	BGBCC_CCXL_TypeBaseRealP(ctx, dty) &&
					BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) &&
					ctx->arch_has_fmac	)	)
			{
				BGBCC_CCXL_CompileExpr(ctx, ln2);
				BGBCC_CCXL_CompileExpr(ctx, rn2);
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_StackTrinaryOp(ctx, "MAC");
				return;
			}
		}
#endif

#if 1
		if(	!strcmp(s0, "+") &&
			!BGBCC_CCXL_CheckIsStaticLib(ctx) &&
			BGBCC_CCXL_IsBinaryP(ctx, ln, "*"))
		{
			ln2=BCCX_FetchCst(ln, &bgbcc_rcst_left, "left");
			rn2=BCCX_FetchCst(ln, &bgbcc_rcst_right, "right");
			ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
			rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
			BGBCC_CCXL_InferExpr(ctx, ln2, &lty2);
			BGBCC_CCXL_InferExpr(ctx, rn2, &rty2);

			BGBCC_CCXL_GetTypeBinaryDest(ctx,
				CCXL_BINOP_MUL, lty2, rty2, &bty);
			BGBCC_CCXL_GetTypeBinaryDest(ctx,
				CCXL_BINOP_ADD, rty, bty, &dty);

			if(	(	BGBCC_CCXL_TypeSmallIntP(ctx, dty) &&
					BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) &&
					ctx->arch_has_imac	) ||
				(	BGBCC_CCXL_TypeBaseRealP(ctx, dty) &&
					BGBCC_CCXL_TypeCompatibleP(ctx, dty, bty) &&
					ctx->arch_has_fmac	)	)
			{
				BGBCC_CCXL_CompileExpr(ctx, ln2);
				BGBCC_CCXL_CompileExpr(ctx, rn2);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackTrinaryOp(ctx, "MAC");
				return;
			}
		}
#endif

		if(BGBCC_CCXL_IsBinaryP(ctx, l, "&") &&
			BGBCC_CCXL_IsBinaryP(ctx, ln, ">>"))
		{
			ln2=BCCX_FetchCst(ln, &bgbcc_rcst_left, "left");
			rn2=BCCX_FetchCst(ln, &bgbcc_rcst_right, "right");
			ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
			rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
			BGBCC_CCXL_InferExpr(ctx, ln2, &lty2);
			BGBCC_CCXL_InferExpr(ctx, rn2, &rty2);
			
			if(	BGBCC_CCXL_IsIntP(ctx, rn2) &&
				BGBCC_CCXL_IsIntP(ctx, rn) &&
				BGBCC_CCXL_TypeSmallLongP(ctx, lty2))
			{
				i0=BCCX_GetIntCst(rn2, &bgbcc_rcst_value, "value");
				i1=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
				i2=-1;
				for(k=1; k<30; k++)
					if(((1<<k)-1)==i1)
						{ i2=k; break; }
				if((i2>0) && ((i0+i2)<=64))
				{
					BGBCC_CCXL_CompileExpr(ctx, ln2);
					BGBCC_CCXL_StackBitLoadInt(ctx, i0, i0+i2-1);
					return;
				}
			}
		}

		s1=NULL;
		opr=BGBCC_CCXL_BinaryOpIdForName(ctx, s0);
//		if(opr>=0)
		if((opr>=0) &&
			(lty.val!=CCXL_TY_UNDEF_I) &&
			(rty.val!=CCXL_TY_UNDEF_I))
		{
			BGBCC_CCXL_GetTypeBinaryDest(ctx,
				opr, lty, rty, &dty);
			s1=BGBCC_CCXL_TypeGetSig(ctx, dty);
		}

//		if(BGBCC_CCXL_TypeSmallFloat128P(ctx, dty) &&
//			BGBCC_CCXL_TypeSmallFloat128P(ctx, lty) &&
//			BGBCC_CCXL_TypeSmallFloat128P(ctx, rty))
//		if(BGBCC_CCXL_TypeSmallInt128P(ctx, dty) &&
//			BGBCC_CCXL_TypeSmallInt128P(ctx, lty) &&
//			BGBCC_CCXL_TypeSmallInt128P(ctx, rty) &&
//			(opr!=CCXL_BINOP_SHL) &&
//			(opr!=CCXL_BINOP_SHR) &&
//			(opr!=CCXL_BINOP_SHRR))

#if 0
		if(
			(opr>=0) &&
			s1 &&
			BGBCC_CCXL_TypeSgLongP(ctx, dty) &&
//			BGBCC_CCXL_TypeSmallLongP(ctx, dty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, lty) &&
			BGBCC_CCXL_TypeSmallLongP(ctx, rty) &&
			((lty.val!=dty.val) || (rty.val!=dty.val)) &&
			(opr==CCXL_BINOP_ADD) &&
			(opr!=CCXL_BINOP_SHL) &&
			(opr!=CCXL_BINOP_SHR) &&
			(opr!=CCXL_BINOP_SHRR) &&
			(opr!=CCXL_BINOP_MUL) &&
			(opr!=CCXL_BINOP_DIV))
#endif
		if(0)
		{
			BGBCC_CCXL_CompileExprAsTypeSig(ctx, s1, ln);
			BGBCC_CCXL_CompileExprAsTypeSig(ctx, s1, rn);
			BGBCC_CCXL_StackBinaryOp(ctx, s0);
			return;
		}else
		{
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
		}

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

	if(	BCCX_TagIsCstP(l, &bgbcc_rcst_new, "new") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_jnew, "jnew"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_type, "type");
//		u=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		u=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
//		v=BCCX_FetchCst(l, &bgbcc_rcst_size, "size");
		v=BCCX_FindTagCst(l, &bgbcc_rcst_size, "size");
		i=BCCX_TagIsCstP(l, &bgbcc_rcst_jnew, "jnew");
		
		if(!t || (u && v))
		{
			BGBCC_CCXL_StubError(ctx);
			return;
		}

		if(v)
		{
			BGBCC_CCXL_CompileNewArray(ctx, t, v, i);
		}else
		{
			BGBCC_CCXL_CompileNewBasic(ctx, t, u);
		}

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

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_object, "object"))
	{
//		ln=BCCX_FetchCst(l, &bgbcc_rcst_list, "list");
		ln=BCCX_FindTagCst(l, &bgbcc_rcst_list, "list");
//		t=BCCX_FetchCst(l, &bgbcc_rcst_type, "type");
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");

		if(t)
		{
			ln=BCCX_FindTagCst(l, &bgbcc_rcst_list, "list");

			s1=BGBCC_CCXL_VarTypeString(ctx, t);
			BGBCC_CCXL_CompileExprAsTypeSig(ctx, s1, ln);

			return;

//			BGBCC_CCXL_CompileExprAsTypeSig()

//			BGBCC_CCXL_StackPushInitObj(ctx, s1);
//			BGBCC_CCXL_StackPushTempObj(ctx, s1);
		}else
		{
			BGBCC_CCXL_PushMark(ctx);
	//		BGBCC_CCXL_StackPushConstString(ctx, s0);
			BGBCC_CCXL_StackCallName(ctx, "__lvo_emptyobject", 0);
		}

//		c=ln;
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(ln);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(ln, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_attr, "attr"))
			{
				s0=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

				sprintf(tb, "__temp_fidcache__%s", s0);

#if 1
				BGBCC_CCXL_PushMark(ctx);

				BGBCC_CCXL_StackDupMarkIdx(ctx, 0);
//				BGBCC_CCXL_CompileExpr(ctx, v);

				BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", v);
				BGBCC_CCXL_StackExch(ctx);

				BGBCC_CCXL_StackLoadAddrVSig(ctx, tb, "t");
				BGBCC_CCXL_StackExch(ctx);

				BGBCC_CCXL_StackPushConstString(ctx, s0);
				BGBCC_CCXL_StackExch(ctx);

//				BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot", 1);
				BGBCC_CCXL_StackCallName(ctx, "__lvo_setslot_cst", 1);
//				BGBCC_CCXL_StackPop(ctx);
#endif

#if 0
				BGBCC_CCXL_StackDupB(ctx);
				BGBCC_CCXL_CompileExpr(ctx, v);
				BGBCC_CCXL_StackExch(ctx);
				BGBCC_CCXL_StackStoreSlot(ctx, s0);
#endif
			}
//			c=BCCX_Next(c);
		}

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_lambda, "lambda"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
//		u=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		u=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
//		v=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
//		d=BCCX_FetchCst(l, &bgbcc_rcst_list, "list");
		d=BCCX_FindTagCst(l, &bgbcc_rcst_list, "list");
		i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");

//		j=BGBCC_CCXL_InferLambdaCapRef(ctx, v);

		sprintf(tb, "__lfn_%X", i);
//		j=BGBCC_CCXL_LookupStructureID(ctx, tb);
		obj=BGBCC_CCXL_LookupStructure(ctx, tb);
		
//		if(j>=0)
		if(obj)
		{
			BGBCC_CCXL_PushMark(ctx);

			sprintf(tb, "__lfn_%X/fn", i);
			BGBCC_CCXL_StackLoadAddr(ctx, bgbcc_strdup(tb));

			sprintf(tb, "X__lfn_%X;", i);
			sprintf(tb1, "PX__lfn_%X;", i);
			BGBCC_CCXL_PushMark(ctx);
			BGBCC_CCXL_StackSizeofSig(ctx, tb);
			
//			if(i&1)
			if(i&0x10000000U)
			{
				BGBCC_CCXL_StackCallName(ctx, "__alloca_wxe", 0);
			}else
			{
//				BGBCC_CCXL_StackCallName(ctx, "tk_malloc_wxe", 0);
				BGBCC_CCXL_StackCallName(ctx, "__lvo_alloc_wxe", 0);
			}

			BGBCC_CCXL_StackCastSig(ctx, tb1);

#if 1
			for(j=0; j<obj->decl->n_fields; j++)
			{
				s=obj->decl->fields[j]->name;
				if(!s)
					continue;
				if((s[0]=='_') && (s[1]=='_'))
					continue;

				n=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s);
				n=BGBCC_CCXL_ReduceExpr(ctx, n);

				s0=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
				k=BGBCC_CCXL_LookupLocalIndex(ctx, s0);
				if(k<0)
					continue;

				BGBCC_CCXL_StackDupB(ctx);
				BGBCC_CCXL_CompileExpr(ctx, n);
				BGBCC_CCXL_StackExch(ctx);
				BGBCC_CCXL_StackStoreSlot(ctx, s);
			}
			
//			c=d;
//			while(c)
//			{
			na=BCCX_GetNodeChildCount(d);
			for(ci=0; ci<na; ci++)
			{
				c=BCCX_GetNodeIndex(d, ci);

				if(BCCX_TagIsCstP(c, &bgbcc_rcst_assign, "assign"))
				{
					ln=BCCX_FetchCst(c, &bgbcc_rcst_left, "left");
					rn=BCCX_FetchCst(c, &bgbcc_rcst_right, "right");
					s=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");

					BGBCC_CCXL_StackDupB(ctx);
//					BGBCC_CCXL_CompileExpr(ctx, rn);
					BGBCC_CCXL_CompileExprAsTypeSig(ctx, "r", rn);
					BGBCC_CCXL_StackExch(ctx);
					BGBCC_CCXL_StackStoreSlot(ctx, s);
				}

//				c=BCCX_Next(c);
			}
#endif

#if 0
			for(j=0; j<ctx->n_lambda_capref; j++)
			{
				strcpy(tb2, ctx->lambda_capref[j]);
				s=tb2;
				while(*s && (*s!='!'))
					s++;
				*s=0;
				
				s=bgbcc_strdup(tb2);
				
				n=BCCX_NewCst(&bgbcc_rcst_ref, "ref");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s);
				n=BGBCC_CCXL_ReduceExpr(ctx, n);
			
				BGBCC_CCXL_StackDupB(ctx);
//				BGBCC_CCXL_PushLoad(ctx, ctx->lambda_capref[j]);
				BGBCC_CCXL_CompileExpr(ctx, n);
				BGBCC_CCXL_StackExch(ctx);
//				BGBCC_CCXL_StackStoreSlot(ctx, ctx->lambda_capref[j]);
//				BGBCC_CCXL_StackStoreSlot(ctx, bgbcc_strdup(tb2));
				BGBCC_CCXL_StackStoreSlot(ctx, s);
			}
#endif

			BGBCC_CCXL_StackCallName(ctx, "__lvo_makelambda", 0);

			sprintf(tb1, "PX__lfn_%X/fn;", i);
			BGBCC_CCXL_StackCastSig(ctx, tb1);

			return;

#if 0
			sprintf(tb, "__lfn_%X", i);
			s0=bgbcc_strdup(tb);

			n=BCCX_NewCst(&bgbcc_rcst_struct, "struct");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);
			ctx->dynobj=BCCX_AddEnd2(ctx->dynobj, &(ctx->dynobj_e), n);

			sprintf(tb, "__lfn_%X/fn", i);
			s0=bgbcc_strdup(tb);

			n=BCCX_NewCst(&bgbcc_rcst_defun, "defun");
			BCCX_AddV(n, BCCX_Clone(t));
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", u));
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", v));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);

			ctx->dynobj=BCCX_AddEnd2(ctx->dynobj, &(ctx->dynobj_e), n);
#endif
		}else
		{
			sprintf(tb, "__fn_%X", i);
			s0=bgbcc_strdup(tb);

#if 0
			n=BCCX_NewCst(&bgbcc_rcst_defun, "defun");
			BCCX_AddV(n, BCCX_Clone(t));
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", u));
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", v));
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);

			ctx->dynobj=BCCX_AddEnd2(ctx->dynobj, &(ctx->dynobj_e), n);
#endif

			BGBCC_CCXL_StackLoadAddr(ctx, s0);
			return;
		}

//		BGBCC_CCXL_StackPushConstInt(ctx, 0);
//		BGBCC_CCXL_StubError(ctx);
		return;
	}

	BGBCC_CCXL_Error(ctx, "Unhandled expr %s\n", BCCX_Tag(l));
	BGBCC_CCXL_StackPushConstInt(ctx, 0);
}

s64 BGBCC_CCXL_ConstRangeMaskU(BGBCC_TransState *ctx, s64 imm, int sz)
{
	s64 li, lk;
	
	li=imm;
	lk=li&((1ULL<<sz)-1);
	if(lk!=li)
	{
		BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONSTBITLOSS);
		li=lk;
	}
	return(li);
}

s64 BGBCC_CCXL_ConstRangeMaskS(BGBCC_TransState *ctx, s64 imm, int sz)
{
	s64 li, lk;
	int j;
	
	li=imm;
	if(sz>0)
	{
		j=64-sz;
		lk=(li<<j)>>j;
	}else
	{
		lk=li>>63;
	}
	if(lk!=li)
	{
		BGBCC_CCXL_TagWarn(ctx, CCXL_TERR_CONSTBITLOSS);
		li=lk;
	}
	return(li);
}

void BGBCC_CCXL_CompileExprT(BGBCC_TransState *ctx, BCCX_Node *l)
{
	long long li, lj, lk;
	double g0, g1, g2, g3;
	double g;
	float f;
	BCCX_Node *t, *c, *ct, *cv;
	char *s0, *s1, *s2;
//	u32 ui, uj;
//	u16 usi, usj;
	int sz;
	int i, j;

	l=BGBCC_CCXL_ReduceExpr(ctx, l);

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");	
		s0=BGBCC_CCXL_CompileRemapName(ctx, s0);
		BGBCC_CCXL_PushLoad(ctx, s0);
		BGBCC_CCXL_VlMarkRefIn(ctx, s0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		sz=BCCX_GetIntCst(l, &bgbcc_rcst_value_sz, "value_sz");
		
		if((sz>0) && (sz&(sz-1)))
		{
			if(s0 && (*s0=='U'))
				{ li=BGBCC_CCXL_ConstRangeMaskU(ctx, li, sz); }
			if(s0 && (*s0=='I'))
				{ li=BGBCC_CCXL_ConstRangeMaskS(ctx, li, sz); }
		
			BGBCC_CCXL_StackPushConstBitInt64(ctx, li, sz);
			return;
		}
		
		if(s0 && !strcmp(s0, "U"))
		{
			BGBCC_CCXL_StackPushConstUInt(ctx, li);
			return;
		}
		
//		if(s0 && (!strcmp(s0, "L") || !strcmp(s0, "LL") ||
//			!strcmp(s0, "ULL") ||
//			(((s32)li)!=li)))
		if(s0 && (!strcmp(s0, "L") || !strcmp(s0, "LL") ||
			(((s32)li)!=li)))
		{
			BGBCC_CCXL_StackPushConstLong(ctx, li);
			return;
		}
		
		if(s0 && (!strcmp(s0, "UL") || !strcmp(s0, "ULL")))
		{
			BGBCC_CCXL_StackPushConstULong(ctx, li);
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
		sz=BCCX_GetIntCst(l, &bgbcc_rcst_value_sz, "value_sz");
		s0=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");

		if((sz>0) && (sz&(sz-1)))
		{
			if(s0 && (*s0=='U'))
			{
				if(sz>=64)
					{ lj=BGBCC_CCXL_ConstRangeMaskU(ctx, lj, sz-64); }
				else
					{ li=BGBCC_CCXL_ConstRangeMaskU(ctx, li, sz); }
			}
		
			if(s0 && (*s0=='I'))
			{
				if(sz>=64)
					{ lj=BGBCC_CCXL_ConstRangeMaskS(ctx, lj, sz-64); }
				else
					{ li=BGBCC_CCXL_ConstRangeMaskS(ctx, li, sz); }
			}

			BGBCC_CCXL_StackPushConstBitInt128(ctx, li, lj, sz);
			return;
		}
		
		BGBCC_CCXL_StackPushConstInt128(ctx, li, lj);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int_ts, "int_ts"))
	{
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		lj=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		j=BCCX_GetIntCst(l, &bgbcc_rcst_value_sz, "value_sz");
		BGBCC_CCXL_StackPushConstTristate64(ctx, li, lj, j);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int_ts, "int_bi"))
	{
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		lj=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		j=BCCX_GetIntCst(l, &bgbcc_rcst_value_sz, "value_sz");
		BGBCC_CCXL_StackPushConstTristate64(ctx, li, lj, j);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_float128, "float128"))
	{
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value_lo, "value_lo");
		lj=BCCX_GetIntCst(l, &bgbcc_rcst_value_hi, "value_hi");
		BGBCC_CCXL_StackPushConstFloat128(ctx, li, lj);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_bigint, "bigint"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_value, "value");
		s1=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		BGBCC_CCXL_StackPushConstBigInt(ctx, s0, 0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_bigtsi, "bigtsi"))
	{
		s0=BCCX_GetCst(l, &bgbcc_rcst_value, "value");
//		s1=BCCX_GetCst(l, &bgbcc_rcst_tysuf, "tysuf");
		lj=BCCX_GetIntCst(l, &bgbcc_rcst_value_sz, "value_sz");
		BGBCC_CCXL_StackPushConstBigTristate(ctx, s0, lj);
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

		if(!strcmp(s0, "V3SF"))i=CCXL_REGVEC_TY_V3H;
		if(!strcmp(s0, "V4SF"))i=CCXL_REGVEC_TY_V4H;

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
	BCCX_Node *c, *v;
	char *sn, *ss;
	int idx, ix1, ci, na;

//	BGBCC_CCXL_StubError(ctx);
	
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_list, "list"))
	{
		BGBCC_CCXL_StackPushTempObj(ctx, sig);

		st=BGBCC_CCXL_LookupStructureForSig(ctx, sig);

//		c=BCCX_Child(l);
		idx=0;
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_attr, "attr"))
			{
				sn=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

				ix1=BGBCC_CCXL_LookupStructFieldID(ctx, st, sn);
				ss=st->decl->fields[ix1]->sig;
			
				BGBCC_CCXL_StackDupClean(ctx);

				BGBCC_CCXL_CompileExprAsTypeSig(ctx, ss, c);
				BGBCC_CCXL_StackExch(ctx);

				BGBCC_CCXL_StackStoreSlot(ctx, sn);
			}
			else
			{
				sn=st->decl->fields[idx]->name;
				ss=st->decl->fields[idx]->sig;
			
				BGBCC_CCXL_StackDupClean(ctx);

				BGBCC_CCXL_CompileExprAsTypeSig(ctx, ss, c);
				BGBCC_CCXL_StackExch(ctx);

				BGBCC_CCXL_StackStoreSlot(ctx, sn);

				idx++;
			}

//			c=BCCX_Next(c);
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
	int an_bo[64], an_bsz[64];
	double afv[64];
	BCCX_Node *c, *t, *v, *ln, *rn;
	char *vfn;
	ccxl_type bty, sty, tty;
	s64 li;
	int btysz, sz, tbsz;
	int i0, i1, i2, i3;
	int i, j, k, na, ci, vty;
	
	BGBCC_CCXL_TypeFromSig(ctx, &bty, sig);

//	printf("BGBCC_CCXL_CompileExprAsTypeSig: %s\n", sig);

	if(BGBCC_CCXL_TypeBitIntP(ctx, bty))
	{
		btysz=BGBCC_CCXL_TypeGetBitIntSize(ctx, bty);

		if(BCCX_TagIsCstP(l, &bgbcc_rcst_list, "list"))
		{
			tbsz=0;
			na=BCCX_GetNodeChildCount(l);
			if(na>=64)
				{ BGBCC_DBGBREAK }
//			for(ci=0; ci<na; ci++)
			for(ci=na-1; ci>=0; ci--)
			{
				c=BCCX_GetNodeIndex(l, ci);
				t=BGBCC_CCXL_ReduceExpr(ctx, c);
				BGBCC_CCXL_InferExpr(ctx, t, &tty);
				sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, tty);
				if(!sz)
					BGBCC_CCXL_TagError(ctx, CCXL_TERR_BADTYPEARG);
				an[ci]=t;
				an_bo[ci]=tbsz;
				an_bsz[ci]=sz;
				tbsz+=sz;
			}
			if((btysz>1) && (btysz!=tbsz))
				BGBCC_CCXL_TagError(ctx, CCXL_TERR_BITSIZENEQ);

			if(btysz!=tbsz)
			{
				btysz=tbsz;
				bty=BGBCC_CCXL_MakeTypeID_Arr(ctx, CCXL_TY_UBITINT, btysz);
			}

			BGBCC_CCXL_StackPushConstInt(ctx, 0);
			BGBCC_CCXL_StackCastType(ctx, bty);
			for(ci=0; ci<na; ci++)
			{
				c=an[ci];
				if(BCCX_TagIsCstP(c, &bgbcc_rcst_getindex, "getindex"))
				{
					t=BCCX_FetchCst(c, &bgbcc_rcst_index, "index");
					v=BCCX_FetchCst(c, &bgbcc_rcst_array, "array");

					if(BCCX_TagIsCstP(t, &bgbcc_rcst_colon, "colon"))
					{
						ln=BCCX_FetchCst(t, &bgbcc_rcst_left, "left");
						rn=BCCX_FetchCst(t, &bgbcc_rcst_right, "right");

						if(	BGBCC_CCXL_IsIntP(ctx, ln) &&
							BGBCC_CCXL_IsIntP(ctx, rn) )
						{
							i0=BCCX_GetIntCst(ln, &bgbcc_rcst_value, "value");
							i1=BCCX_GetIntCst(rn, &bgbcc_rcst_value, "value");
							if(i0<i1)
								{ k=i0; i0=i1; i1=k; }

							BGBCC_CCXL_CompileExpr(ctx, v);
							BGBCC_CCXL_StackExch(ctx);
							BGBCC_CCXL_StackBitMove(ctx,
								an_bo[ci], an_bo[ci]+an_bsz[ci]-1,
								i1, i0);
							continue;
						}
					}

					if(BCCX_TagIsCstP(t, &bgbcc_rcst_int, "int"))
					{
						i0=BCCX_GetIntCst(t, &bgbcc_rcst_value, "value");
						BGBCC_CCXL_CompileExpr(ctx, v);
						BGBCC_CCXL_StackExch(ctx);
						BGBCC_CCXL_StackBitMove(ctx,
							an_bo[ci], an_bo[ci], i0, i0);
						continue;
					}
				}

				BGBCC_CCXL_CompileExpr(ctx, an[ci]);
				BGBCC_CCXL_StackExch(ctx);
				BGBCC_CCXL_StackBitStore(ctx,
					an_bo[ci], an_bo[ci]+an_bsz[ci]-1);
			}
			
			return;
		}
	}


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
			case CCXL_TY_VEC4H:		vty=CCXL_REGVEC_TY_V4H;		break;
			case CCXL_TY_VEC3H:		vty=CCXL_REGVEC_TY_V3H;		break;
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
			case CCXL_TY_VEC4H:		vfn="__v4h_float4";			break;
			case CCXL_TY_VEC3H:		vfn="__v4h_float4";			break;
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
//			c=BCCX_Child(l);
//			while(c)
//			{
//				t=BGBCC_CCXL_ReduceExpr(ctx, c);
//				an[na++]=t;
//				c=BCCX_Next(c);
//			}

			na=BCCX_GetNodeChildCount(l);
			for(ci=0; ci<na; ci++)
			{
				c=BCCX_GetNodeIndex(l, ci);
				t=BGBCC_CCXL_ReduceExpr(ctx, c);
				an[ci]=t;
			}

			if(na>=64)
			{
				BGBCC_DBGBREAK
			}

			for(i=0; i<4; i++)
				afv[i]=0;
			
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
//				BGBCC_CCXL_CompileArgsFuncallName(ctx,
//					vfn, BCCX_Child(l));
				BGBCC_CCXL_CompileArgsFuncallNameB(ctx,
					vfn, l);
				return;
			}

			BGBCC_CCXL_StubError(ctx);
			return;
		}
	}

	BGBCC_CCXL_InferExpr(ctx, l, &sty);

#if 1
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_int, "int"))
	{
		li=BCCX_GetIntCst(l, &bgbcc_rcst_value, "value");
		
		if((*sig=='i') || ((*sig=='l') && (ctx->arch_sizeof_long==4)))
		{
			li=(s32)li;
			BGBCC_CCXL_StackPushConstInt(ctx, li);
			return;
		}

		if((*sig=='j') || ((*sig=='m') && (ctx->arch_sizeof_long==4)))
		{
			li=(u32)li;
			BGBCC_CCXL_StackPushConstUInt(ctx, li);
			return;
		}

		if((*sig=='x') || ((*sig=='l') && (ctx->arch_sizeof_long==8)))
		{
			BGBCC_CCXL_StackPushConstLong(ctx, li);
			return;
		}

#if 1
		if((*sig=='y') || ((*sig=='m') && (ctx->arch_sizeof_long==8)))
		{
			BGBCC_CCXL_StackPushConstULong(ctx, li);
			return;
		}
#endif

		if((*sig=='c') || (*sig=='a'))
		{
			li=(signed char)li;
			BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 4);
			return;
		}
		if((*sig=='h') || (*sig=='b'))
		{
			li=(unsigned char)li;
			BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 5);
			return;
		}

		if(*sig=='s')
		{
			li=(signed short)li;
			BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 6);
			return;
		}
		if((*sig=='t') || (*sig=='w'))
		{
			li=(unsigned short)li;
			BGBCC_CCXL_StackPushConstSmallInt(ctx, li, 7);
			return;
		}

		if(*sig=='f')
		{
			BGBCC_CCXL_StackPushConstFloat(ctx, li);
			return;
		}

		if(*sig=='d')
		{
			BGBCC_CCXL_StackPushConstDouble(ctx, li);
			return;
		}
	}
#endif

	BGBCC_CCXL_CompileExpr(ctx, l);
//	BGBCC_CCXL_StackCastSig(ctx, sig);

	if(!BGBCC_CCXL_TypeCompatibleP(ctx, bty, sty))
		BGBCC_CCXL_StackCastSig(ctx, sig);

//			if(BGBCC_CCXL_TypeCompatibleP(ctx, dty, rty))
//				BGBCC_CCXL_CompileExpr(ctx, rn);
//			else
//				BGBCC_CCXL_CompileExprAsTypeSig(ctx, s1, rn);

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

