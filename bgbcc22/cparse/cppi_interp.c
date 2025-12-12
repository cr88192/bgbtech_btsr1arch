BGBPP_PpiDef *bgbpp_ppiglobal;

BGBPP_PpiDef *BGBPP_PpiGetGlobalDef(char *name, u32 lang)
{
	BGBPP_PpiDef *cur;

	cur=bgbpp_ppiglobal;
	while(cur)
	{
		if(lang && (cur->lang!=lang))
			{ cur=cur->next; continue; }
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=bgbcc_malloc(sizeof(BGBPP_PpiDef));
	memset(cur, 0, sizeof(BGBPP_PpiDef));

	cur->name=bgbcc_strdup(name);
	cur->next=bgbpp_ppiglobal;
	cur->lang=lang;
	bgbpp_ppiglobal=cur;

	return(cur);
}

BGBPP_PpiDef *BGBPP_PpiAllocDef(BGBCP_ParseState *ctx)
{
	BGBPP_PpiDef *tmp;

	if(ctx)
	{
		tmp=ctx->ppi_freedef;
		if(tmp)
		{
			ctx->ppi_freedef=tmp->next;
			memset(tmp, 0, sizeof(BGBPP_PpiDef));
			return(tmp);
		}
	}

	tmp=bgbcc_malloc(sizeof(BGBPP_PpiDef));
	memset(tmp, 0, sizeof(BGBPP_PpiDef));
	return(tmp);
}

void BGBPP_PpiFreeDef(BGBCP_ParseState *ctx, BGBPP_PpiDef *tmp)
{
	tmp->next=ctx->ppi_freedef;
	ctx->ppi_freedef=tmp;
}

BGBPP_PpiDef *BGBPP_PpiLookupTopDef(BGBCP_ParseState *ctx, char *name)
{
	BGBPP_PpiDef *cur;
	
	cur=ctx->ppi_ctx->ppi_toplevel;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	
	cur=bgbpp_ppiglobal;
	while(cur)
	{
		if(cur->lang && (cur->lang!=ctx->lang))
			{ cur=cur->next; continue; }
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	
	return(NULL);
}

BGBPP_PpiDef *BGBPP_PpiGetTopDef(BGBCP_ParseState *ctx, char *name)
{
	BGBPP_PpiDef *cur;
	
	cur=ctx->ppi_ctx->ppi_toplevel;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	
	cur=bgbcc_malloc(sizeof(BGBPP_PpiDef));
	memset(cur, 0, sizeof(BGBPP_PpiDef));
	
	cur->name=bgbcc_strdup(name);
	cur->next=ctx->ppi_ctx->ppi_toplevel;
	ctx->ppi_ctx->ppi_toplevel=cur;
	
	return(cur);
}

BGBPP_PpiFrame *BGBPP_PpiEnterFrame(BGBCP_ParseState *ctx)
{
	BGBPP_PpiFrame *tmp;

	tmp=ctx->ppi_freeframe;
	if(tmp)
	{
		ctx->ppi_freeframe=tmp->next;
		
		memset(tmp, 0, sizeof(BGBPP_PpiFrame));
		tmp->next=ctx->ppi_ctx->ppi_callframe;
		ctx->ppi_ctx->ppi_callframe=tmp;
		return(tmp);
	}

	tmp=bgbcc_malloc(sizeof(BGBPP_PpiFrame));
	memset(tmp, 0, sizeof(BGBPP_PpiFrame));
	tmp->next=ctx->ppi_ctx->ppi_callframe;
	ctx->ppi_ctx->ppi_callframe=tmp;
	return(tmp);
}	

void BGBPP_PpiLeaveFrame(BGBCP_ParseState *ctx)
{
	BGBPP_PpiFrame *tmp;
	BGBPP_PpiDef *dcur, *dnxt;

	tmp=ctx->ppi_ctx->ppi_callframe;
	if(!tmp)
		return;
	ctx->ppi_ctx->ppi_callframe=tmp->next;
	
	dcur=tmp->vars;
	while(dcur)
	{
		dnxt=dcur->next;
		BGBPP_PpiFreeDef(ctx, dcur);
		dcur=dnxt;
	}
	
	tmp->next=ctx->ppi_freeframe;
	ctx->ppi_freeframe=tmp;
}

BCCX_Node *BGBPP_PpiFetchNameReduce(BGBCP_ParseState *ctx, char *name)
{
	BGBPP_PpiDef *dcur;

	if(ctx->ppi_callframe)
	{
		dcur=ctx->ppi_callframe->vars;
		while(dcur)
		{
			if(!strcmp(dcur->name, name))
				return(dcur->value);
			dcur=dcur->next;
		}
	}

	dcur=ctx->ppi_toplevel;
	while(dcur)
	{
		if(!strcmp(dcur->name, name))
			return(dcur->value);
		dcur=dcur->next;
	}

	if(ctx->lang==BGBCC_LANG_SCAD)
	{
		if(!strcmp(name, "true"))
			return(BGBCP_WrapInt(1));
		if(!strcmp(name, "false"))
			return(BGBCP_WrapInt(0));
	}

	return(NULL);
}

BGBPP_PpiDef *BGBPP_PpiLookupNameDef(BGBCP_ParseState *ctx, char *name)
{
	BGBPP_PpiDef *dcur;

	if(ctx->ppi_ctx->ppi_callframe)
	{
		dcur=ctx->ppi_ctx->ppi_callframe->vars;
		while(dcur)
		{
			if(!strcmp(dcur->name, name))
				return(dcur);
			dcur=dcur->next;
		}
	}

	dcur=ctx->ppi_ctx->ppi_toplevel;
	while(dcur)
	{
		if(!strcmp(dcur->name, name))
			return(dcur);
		dcur=dcur->next;
	}

	return(NULL);
}


BCCX_Node *BGBPP_PpiInterpBodyStatement(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *targs[32];
	BCCX_Node *c, *n, *v, *t, *ln, *rn, *ni, *nc, *nb, *ns;
	BGBPP_PpiFrame *frm;
	BGBPP_PpiDef *def;
	char *s, *s0, *s1;
	int i, ci, na, na1;

	frm=ctx->ppi_ctx->ppi_callframe;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
//				c=BCCX_Next(c);
				continue;
			}

			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
//			t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

			def=BGBPP_PpiAllocDef(ctx);
			def->name=s;
			def->value=v;

			def->next=frm->vars;
			frm->vars=def;

//			c=BCCX_Next(c);
		}
		return(NULL);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
		nb=v;
		
		def=BGBPP_PpiLookupTopDef(ctx, s);
		if(def && !(def->PpiDoFuncall2))
		{
			v=BGBPP_PpiInterpDoFuncall(ctx, def, n, v);
			return(v);
		}
		
//		na=0;
//		c=n;
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);
			v=BGBCP_ReduceExpr(ctx->ppi_ctx, c);
//			targs[na++]=v;
			targs[ci]=v;
//			c=BCCX_Next(c);
		}

		if(def && def->PpiDoFuncall2)
		{
			v=def->PpiDoFuncall2(ctx, def, targs, na, nb);
			return(v);
		}
		
		v=BGBPP_PpiInterpDoFuncall2(ctx, s, targs, na, nb);
		return(v);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		n=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		v=BGBCP_ReduceExpr(ctx->ppi_ctx, n);

		def=BGBPP_PpiLookupNameDef(ctx, s);
		if(def)
		{
			def->value=v;
		}

		if(ctx->lang==BGBCC_LANG_SCAD)
		{
			def=BGBPP_PpiAllocDef(ctx);
			def->name=s;
			def->value=v;
			def->next=frm->vars;
			frm->vars=def;
		}

		return(NULL);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		ns=NULL;

		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);
			v=BGBPP_PpiInterpBodyStatement(ctx, c);
			if(ctx->ppi_break)
				break;
			if(v && (ctx->lang==BGBCC_LANG_SCAD))
				{ ns=BCCX_AddEnd(ns, v); }
		}
		return(ns);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		t=BGBCP_ReduceExpr(ctx->ppi_ctx, t);

		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		i=BGBCP_BoolExpr(ctx, t);
		if(i==1)
		{
			v=BGBPP_PpiInterpBodyStatement(ctx, ln);
		}else if(i==0)
		{
			v=BGBPP_PpiInterpBodyStatement(ctx, rn);
		}else
		{
			printf("PPI Failed Reduce %s\n", BCCX_Tag(t));
		}
		
		return(v);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		v=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		n=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");

		ns=NULL;
		t=BGBCP_ReduceExpr(ctx->ppi_ctx, v);
		i=BGBCP_BoolExpr(ctx, t);
		while(i==1)
		{
			v=BGBPP_PpiInterpBodyStatement(ctx, n);
			if(v && (ctx->lang==BGBCC_LANG_SCAD))
				{ ns=BCCX_AddEnd(ns, v); }

			if(ctx->ppi_break==3)
				ctx->ppi_break=0;
			if(ctx->ppi_break)
				break;

			t=BGBCP_ReduceExpr(ctx->ppi_ctx, v);
			i=BGBCP_BoolExpr(ctx, t);
		}
		if(ctx->ppi_break==2)
			ctx->ppi_break=0;
		return(ns);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for"))
	{
		ni=BCCX_FetchCst(l, &bgbcc_rcst_init, "init");
		nc=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		nb=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		ns=BCCX_FetchCst(l, &bgbcc_rcst_step, "step");

		BGBPP_PpiInterpBodyStatement(ctx, ni);

		ns=NULL;
		t=BGBCP_ReduceExpr(ctx->ppi_ctx, nc);
		i=BGBCP_BoolExpr(ctx, t);
		while(i==1)
		{
			BGBPP_PpiInterpBodyStatement(ctx, nb);

			if(ctx->ppi_break==3)
				ctx->ppi_break=0;
			if(ctx->ppi_break)
				break;

			v=BGBPP_PpiInterpBodyStatement(ctx, ns);
			if(v && (ctx->lang==BGBCC_LANG_SCAD))
				{ ns=BCCX_AddEnd(ns, v); }

			t=BGBCP_ReduceExpr(ctx->ppi_ctx, nc);
			i=BGBCP_BoolExpr(ctx, t);
		}
		if(ctx->ppi_break==2)
			ctx->ppi_break=0;
		return(ns);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		n=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		if(n)
		{
			v=BGBCP_ReduceExpr(ctx->ppi_ctx, n);
			ctx->ppi_retval=v;
		}

		ctx->ppi_break=1;
		return(NULL);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_break, "break"))
	{
		ctx->ppi_break=2;
		return(NULL);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_continue, "continue"))
	{
		ctx->ppi_break=3;
		return(NULL);
	}

//	BGBCC_CCXL_Error(ctx, "PPI Unhandled Body Statement %s\n", BCCX_Tag(l));
	printf("PPI Unhandled Body Statement %s\n", BCCX_Tag(l));
	return(NULL);
}

BCCX_Node *BGBPP_PpiInterpDoFuncall2(BGBCP_ParseState *ctx,
	char *name, BCCX_Node **args, int nargs, BCCX_Node *extbody)
{
	char tbuf[4096];
	BCCX_Node *v;
	BGBPP_Def *def;
	char *str, *s, *t, *s0, *s1;
	s64 li;
	double f;
	int i, j, k, an;

	if(!strcmp(name, "isDefined"))
	{
		str=BCCX_GetCst(args[0], &bgbcc_rcst_value, "value");
		def=BGBPP_LookupDefine2(ctx, str);
		return(BGBCP_WrapInt(def!=NULL));
	}

	if(!strcmp(name, "getDefineStr"))
	{
		str=BCCX_GetCst(args[0], &bgbcc_rcst_value, "value");
		def=BGBPP_LookupDefine2(ctx, str);
		if(def)
		{
			return(BGBCP_WrapString(def->value));
		}
		return(NULL);
//		return(BGBCP_WrapRef("null"));
//		return(BGBCP_WrapIntSuf(def!=NULL, NULL));
	}

	if(!strcmp(name, "addDefineStr"))
	{
		s0=BCCX_GetCst(args[0], &bgbcc_rcst_value, "value");
		s1=BCCX_GetCst(args[1], &bgbcc_rcst_value, "value");
		BGBPP_AddDefine(ctx, s0, s1);
		return(NULL);
	}

	if(!strcmp(name, "echo"))
	{
		t=tbuf;
 		for(i=0; i<nargs; i++)
		{
			v=args[i];
			
			if(BCCX_TagIsCstP(v, &bgbcc_rcst_string, "string"))
			{
				s0=BCCX_GetCst(v, &bgbcc_rcst_value, "value");
				strcpy(t, s0);
				t+=strlen(t);
				continue;
			}

			if(BCCX_TagIsCstP(v, &bgbcc_rcst_int, "int"))
			{
				li=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
				sprintf(t, "%lld", li);
				t+=strlen(t);
				continue;
			}

			if(BCCX_TagIsCstP(v, &bgbcc_rcst_real, "real"))
			{
				f=BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value");
				sprintf(t, "%f", f);
				t+=strlen(t);
				continue;
			}
		}
		*t=0;

		BGBPP_ScriptPuts(ctx, tbuf);
		return(NULL);
	}

	if(
//		!strcmp(name, "printf") ||
//		!strcmp(name, "echo") ||
		!strcmp(name, "print"))
	{
		str=BCCX_GetCst(args[0], &bgbcc_rcst_value, "value");

		s=str; t=tbuf; an=1;
		while(*s)
		{
			if(*s=='%')
			{
				if(s[1]=='%')
				{
					s+=2;
					*t++='%';
					continue;
				}
				
				v=args[an];
				if((s[1]=='s') || (s[1]=='d') || (s[1]=='f'))
				{
					s+=2;
					an++;

					if(BCCX_TagIsCstP(v, &bgbcc_rcst_string, "string"))
					{
						s0=BCCX_GetCst(v, &bgbcc_rcst_value, "value");
						strcpy(t, s0);
						t+=strlen(t);
						continue;
					}

					if(BCCX_TagIsCstP(v, &bgbcc_rcst_int, "int"))
					{
						li=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
						sprintf(t, "%lld", li);
//						strcpy(t, s0);
						t+=strlen(t);
						continue;
					}

					if(BCCX_TagIsCstP(v, &bgbcc_rcst_real, "real"))
					{
						f=BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value");
						sprintf(t, "%f", f);
						t+=strlen(t);
						continue;
					}

					continue;
				}
			}
		
			*t++=*s++;
		}
		*t=0;

		BGBPP_ScriptPuts(ctx, tbuf);
		return(NULL);
	}

	return(NULL);
}

BCCX_Node *BGBPP_PpiInterpDoFuncall(BGBCP_ParseState *ctx,
	BGBPP_PpiDef *func, BCCX_Node *args, BCCX_Node *extbody)
{
	BGBPP_PpiFrame *frm;
	BGBPP_PpiDef *def;	
	BCCX_Node *ca0, *ca1, *c, *v, *ns;
	char *s0;
	int na0, na1, na, ci;
	
	frm=BGBPP_PpiEnterFrame(ctx);

// #ifdef BGBCC_BCCX2
#if 1
	na0=BCCX_GetNodeChildCount(func->args);
	na1=BCCX_GetNodeChildCount(args);
	na=(na0<na1)?na0:na1;
	for(ci=0; ci<na; ci++)
	{
		ca0=BCCX_GetNodeIndex(func->args, ci);
		ca1=BCCX_GetNodeIndex(args, ci);

		def=BGBPP_PpiAllocDef(ctx);
		s0=BCCX_GetCst(ca0, &bgbcc_rcst_name, "name");

		v=BGBCP_ReduceExpr(ctx->ppi_ctx, ca1);

		def->name=s0;
		def->value=v;

		def->next=frm->vars;
		frm->vars=def;
	}
#endif

// #ifndef BGBCC_BCCX2
#if 0
	ca0=func->args;
	ca1=args;
	while(ca0 && ca1)
	{
		def=BGBPP_PpiAllocDef(ctx);
		s0=BCCX_GetCst(ca0, &bgbcc_rcst_name, "name");

		v=BGBCP_ReduceExpr(ctx->ppi_ctx, ca1);

		def->name=s0;
		def->value=v;

		def->next=frm->vars;
		frm->vars=def;

		ca0=BCCX_Next(ca0);
		ca1=BCCX_Next(ca1);
	}
#endif
	
	ctx->ppi_retval=NULL;
	ns=NULL;
	
//	c=func->body;
//	while(c)
//	{
	na=BCCX_GetNodeChildCount(func->body);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(func->body, ci);
		v=BGBPP_PpiInterpBodyStatement(ctx, c);
		if(ctx->ppi_break==1)
			break;
//		c=BCCX_Next(c);

		if(v && (ctx->lang==BGBCC_LANG_SCAD))
			ns=BCCX_AddEnd(ns, v);
	}
	
	ctx->ppi_break=0;
	
	if(ns && !ctx->ppi_retval)
		ctx->ppi_retval=ns;
	
	BGBPP_PpiLeaveFrame(ctx);
	return(ctx->ppi_retval);
}

BCCX_Node *BGBPP_PpiInterpTopStatement(BGBCP_ParseState *ctx, BCCX_Node *l)
{
	BCCX_Node *targs[32];
	BCCX_Node *c, *n, *v, *t, *nb;
	BGBPP_PpiDef *def;	
	char *s, *s0, *s1;
	int i, na, ci;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
//		c=BCCX_Child(l);
//		while(c)
//		{
		na=BCCX_GetNodeChildCount(l);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(l, ci);

			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
//				c=BCCX_Next(c);
				continue;
			}

			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
//			t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

//			BGBCC_CCXL_EmitVar2(ctx, s, t, v);
			def=BGBPP_PpiGetTopDef(ctx, s);
			def->value=v;

//			c=BCCX_Next(c);
		}
		return(NULL);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");

//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");

		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
//		v=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");

		def=BGBPP_PpiGetTopDef(ctx, s);
		def->args=n;
		def->body=v;
		return(NULL);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
		nb=v;
		
		def=BGBPP_PpiLookupTopDef(ctx, s);
		if(def && !(def->PpiDoFuncall2))
		{
			v=BGBPP_PpiInterpDoFuncall(ctx, def, n, v);
			return(v);
		}

		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);
			v=BGBCP_ReduceExpr(ctx->ppi_ctx, c);
//			targs[na++]=v;
			targs[ci]=v;
//			c=BCCX_Next(c);
		}

		if(def && def->PpiDoFuncall2)
		{
			v=def->PpiDoFuncall2(ctx, def, targs, na, nb);
			return(v);
		}
		
		v=BGBPP_PpiInterpDoFuncall2(ctx, s, targs, na, nb);
		return(v);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		n=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");

		v=BGBCP_ReduceExpr(ctx->ppi_ctx, n);

		def=BGBPP_PpiLookupNameDef(ctx, s);
		if(def)
		{
			def->value=v;
		}

		if(ctx->lang==BGBCC_LANG_SCAD)
		{
			def=BGBPP_PpiGetTopDef(ctx, s);
			def->value=v;
		}

		return(NULL);
	}

//	BGBCC_CCXL_Error(ctx, "PPI Unhandled Top Statement %s\n", BCCX_Tag(l));
	printf("PPI Unhandled Top Statement %s\n", BCCX_Tag(l));
	return(NULL);
}

BCCX_Node *BGBPP_PpiInterpTryReduce(BGBCP_ParseState *ppctx, BCCX_Node *l)
{
	BCCX_Node *targs[32];
	BGBCP_ParseState *ctx;
	BCCX_Node *c, *n, *v, *t, *nb;
	BGBPP_PpiDef *def;	
	char *s, *s0, *s1;
	int i, na, ci;

	ctx=ppctx;
	if(ctx->ppi_upctx)
		ctx=ctx->ppi_upctx;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
//		n=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		n=BCCX_FindTagCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
		nb=v;
		
		def=BGBPP_PpiLookupTopDef(ctx, s);
		if(def && !(def->PpiDoFuncall2))
		{
			v=BGBPP_PpiInterpDoFuncall(ctx, def, n, v);
			return(v);
		}

#if 1
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);
			v=BGBCP_ReduceExpr(ctx->ppi_ctx, c);
			targs[ci]=v;
		}
#endif

#if 0
		na=0;
		c=n;
		while(c)
		{
			v=BGBCP_ReduceExpr(ctx->ppi_ctx, c);
			targs[na++]=v;
			c=BCCX_Next(c);
		}
#endif

		if(def && def->PpiDoFuncall2)
		{
			v=def->PpiDoFuncall2(ctx, def, targs, na, nb);
			return(v);
		}

		v=BGBPP_PpiInterpDoFuncall2(ctx, s, targs, na, nb);
		return(v);
	}
	
	return(NULL);
}

void BGBPP_InterpMacroed(BGBCP_ParseState *ctx, char *str)
{
	BCCX_Node *c, *n, *lst;
	char *s;
	int ci, na;
	
	s=str;
	lst=BGBCP_Toplevel(ctx->ppi_ctx, &s);
	
#ifdef BGBCC_BCCX2
	na=BCCX_GetNodeChildCount(lst);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(lst, ci);
		BGBPP_PpiInterpTopStatement(ctx, c);
	}
#else
	c=lst;
	while(c)
	{
		BGBPP_PpiInterpTopStatement(ctx, c);
		c=BCCX_Next(c);
	}
#endif
}
