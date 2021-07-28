int BGBCC_CCXL_DoFoldLookupLocalIndex(BGBCC_TransState *ctx, char *name)
{
	int i, j, k;

	for(i=(ctx->vlcn_npos-1); i>=0; i--)
		if(!strcmp(ctx->vlcn_names[i], name))
			return(i);
	return(-1);
}

void BGBCC_CCXL_DoExpressionLambdaFold(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb1[128], tb2[128];
	char *s, *s0;
	BCCX_Node *c, *t, *u, *v, *x, *n, *ln, *rn, *ls;
	int i, j, k, bn, sqn, sqon, obn, oncap, onpos;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		if(ctx->do_lambda_capref<=0)
			return;
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		s0=NULL;
		t=NULL;
		bn=999;

		for(j=ctx->vlcl_stackpos-1; j>=0; j--)
		{
			sprintf(tb1, "%s!%d", s, ctx->vlcl_stack[j]);
			i=BGBCC_CCXL_DoFoldLookupLocalIndex(ctx, tb1);
			if(i>=0)
				{ s0=bgbcc_strdup(tb1); t=ctx->vlcn_types[i]; bn=j; break; }
		}
		
		if(!s0)
		{
			i=BGBCC_CCXL_DoFoldLookupLocalIndex(ctx, s);
			if(i>=0)
			{
				for(j=ctx->vlcl_stackpos-1; j>=0; j--)
				{
					if(ctx->vlcl_stack_npos[j]<=i)
						break;
				}
				t=ctx->vlcn_types[i];
				s0=s; bn=j;
			}
		}
		
		if(s0 && (bn<ctx->do_lambda_capref))
		{
			k=ctx->n_lambda_capref++;
			ctx->lambda_capref[k]=s0;
			ctx->lambda_captype[k]=t;
		}
		
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, ln);
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, rn);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_binary, "binary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_left, "left");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_right, "right");
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, ln);
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, rn);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, ln);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		ln=BCCX_FetchCst(l, &bgbcc_rcst_value, "value");
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, ln);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_lambda, "lambda"))
	{
		t=BCCX_FindTagCst(l, &bgbcc_rcst_type, "type");
		u=BCCX_FetchCst(l, &bgbcc_rcst_args, "args");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		ls=BCCX_FetchCst(l, &bgbcc_rcst_list, "list");
		i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
		
		sqn=i;
		
		onpos=ctx->vlcn_npos;
		oncap=ctx->n_lambda_capref;
		obn=ctx->do_lambda_capref;
		ctx->do_lambda_capref=ctx->vlcl_stackpos;

		onpos=ctx->vlcn_npos;
		sqon=ctx->vlcl_curseq;
		ctx->vlcl_curseq=sqn;
		i=ctx->vlcl_stackpos++;
		ctx->vlcl_stack[i]=sqn;
		ctx->vlcl_stack_npos[i]=onpos;

		c=ls;
		while(c)
		{
			if(BCCX_TagIsCstP(c, &bgbcc_rcst_assign, "assign"))
			{
				ln=BCCX_FetchCst(c, &bgbcc_rcst_left, "left");
				rn=BCCX_FetchCst(c, &bgbcc_rcst_right, "right");
				s=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");

				x=BCCX_NewCst(&bgbcc_rcst_type, "type");
				BCCX_SetCst(x, &bgbcc_rcst_name, "name", "variant");

				k=ctx->n_lambda_capref++;
				ctx->lambda_capref[k]=s;
				ctx->lambda_captype[k]=x;
			}

			c=BCCX_Next(c);
		}

		BGBCC_CCXL_LambdaFoldAddArgs(ctx, u);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, v);

		ctx->vlcl_stackpos--;
		ctx->vlcl_curseq=sqon;
		ctx->vlcn_npos=onpos;


		if(ctx->n_lambda_capref>oncap)
		{
			/* Bindings were captured. */

			sprintf(tb1, "__lfn_%X", sqn);
			s0=bgbcc_strdup(tb1);

			n=BCCX_NewCst(&bgbcc_rcst_struct, "struct");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);
			
			ln=NULL;

			x=BCCX_NewCst(&bgbcc_rcst_int, "int");
			BCCX_SetIntCst(x, &bgbcc_rcst_value, "value", 16);
			rn=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(rn, &bgbcc_rcst_name, "name", "int");
			BCCX_AddV(rn,
				BCCX_NewCst1V(&bgbcc_rcst_size, "size", x));

			x=BCCX_NewCst(&bgbcc_rcst_var, "var");
			BCCX_SetCst(x, &bgbcc_rcst_name, "name", "__thunk");
//			rn=ctx->lambda_captype[j];
			BCCX_AddV(x, rn);
			ln=BCCX_AddEnd(ln, x);

			for(j=oncap; j<ctx->n_lambda_capref; j++)
			{
				strcpy(tb2, ctx->lambda_capref[j]);
				s=tb2;
				while(*s && (*s!='!'))
					s++;
				*s=0;
			
				x=BCCX_NewCst(&bgbcc_rcst_var, "var");
				BCCX_SetCst(x, &bgbcc_rcst_name, "name",
					bgbcc_strdup(tb2));
				rn=ctx->lambda_captype[j];
				BCCX_AddV(x, BCCX_Clone(rn));
				ln=BCCX_AddEnd(ln, x);
			}

			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", ln));

			ctx->dynobj=BCCX_AddEnd2(ctx->dynobj, &(ctx->dynobj_e), n);

			sprintf(tb1, "__lfn_%X/fn", sqn);
			s0=bgbcc_strdup(tb1);
		}else
		{
			/* Primitive Function */
		
			sprintf(tb1, "__fn_%X", sqn);
			s0=bgbcc_strdup(tb1);
		}
		
		n=BCCX_NewCst(&bgbcc_rcst_defun, "defun");
		BCCX_AddV(n, BCCX_Clone(t));
		BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", u));
		BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", v));
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s0);

		ctx->dynobj=BCCX_AddEnd2(ctx->dynobj, &(ctx->dynobj_e), n);


		ctx->do_lambda_capref=obn;
		return;
	}

}

void BGBCC_CCXL_DoStatementLambdaFold(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb1[128];
	BCCX_Node *c, *t, *u, *v, *x, *n, *ln, *rn;
	char *s, *fnam;
	double f, g;
	int sqn, tkn, sqon, onpos;
	int i, j, lnum;

	if(!l)return;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		ln=BCCX_FetchCst(l, &bgbcc_rcst_then, "then");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_else, "else");

		BGBCC_CCXL_DoExpressionLambdaFold(ctx, t);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, ln);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, rn);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		if(!BCCX_Child(l))
			return;

		sqn=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
		tkn=BCCX_GetIntCst(l, &bgbcc_rcst_tokens, "tokens");

		onpos=ctx->vlcn_npos;
		sqon=ctx->vlcl_curseq;
		ctx->vlcl_curseq=sqn;
		i=ctx->vlcl_stackpos++;
		ctx->vlcl_stack[i]=sqn;
		ctx->vlcl_stack_npos[i]=onpos;

		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_CCXL_DoStatementLambdaFold(ctx, c);
			c=BCCX_Next(c);
		}

		ctx->vlcl_stackpos--;
		ctx->vlcl_curseq=sqon;
		ctx->vlcn_npos=onpos;

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, t);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, t);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for"))
	{
		t=BCCX_FetchCst(l, &bgbcc_rcst_cond, "cond");
		v=BCCX_FetchCst(l, &bgbcc_rcst_body, "body");
		ln=BCCX_FetchCst(l, &bgbcc_rcst_init, "init");
		rn=BCCX_FetchCst(l, &bgbcc_rcst_step, "step");

		BGBCC_CCXL_DoStatementLambdaFold(ctx, ln);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, rn);
		BGBCC_CCXL_DoExpressionLambdaFold(ctx, t);
		BGBCC_CCXL_DoStatementLambdaFold(ctx, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(BCCX_TagIsCstP(c, &bgbcc_rcst_proto, "proto"))
			{
				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
				n=BCCX_FetchCst(c, &bgbcc_rcst_args, "args");

//				BGBCC_CCXL_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
				v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

				sprintf(tb1, "%s!%d", s, ctx->vlcl_curseq);

				i=ctx->vlcn_npos++;
				ctx->vlcn_names[i]=bgbcc_strdup(tb1);
				ctx->vlcn_types[i]=t;

				BGBCC_CCXL_DoExpressionLambdaFold(ctx, v);

//				BGBCC_CCXL_EmitTopVar(ctx, s, t, v);
			}

			c=BCCX_Next(c);
		}
		return;
	}

	BGBCC_CCXL_DoExpressionLambdaFold(ctx, l);
}

void BGBCC_CCXL_LambdaFoldAddArgs(
	BGBCC_TransState *ctx, BCCX_Node *args)
{
	char tb1[128];
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *fnam;
	double f, g;
	int sqn, tkn, sqon, onpos;
	int i, j, lnum;

	c=args;
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
			v=BCCX_FetchCst(c, &bgbcc_rcst_value, "value");

			i=ctx->vlcn_npos++;
			ctx->vlcn_names[i]=s;
			ctx->vlcn_types[i]=t;
		}

		c=BCCX_Next(c);
	}

	return;
}

void BGBCC_CCXL_SetupLambdaFold(
	BGBCC_TransState *ctx, BCCX_Node *args)
{
	ctx->vlcn_npos=0;
	ctx->do_lambda_capref=0;
	ctx->vlcl_stackpos=0;
	ctx->vlcl_stack_npos[0]=0;
	BGBCC_CCXL_LambdaFoldAddArgs(ctx, args);
	return;
}
