#include <bgbccc.h>

// BCCX_Node *bgbcp/_structs=NULL;
// BCCX_Node *bgbcp/_types=NULL;

#if 0
BCCX_Node *BGBCP_LookupStructI(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *c;
	char *s;

	c=ctx->structs;
	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
		c=BCCX_Next(c);
	}
	return(NULL);
}
#endif

BCCX_Node *BGBCP_LookupStructI(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c;
	int hi, na, ci;
	char *s;

//	s=name; hi=0;
//	while(*s)hi=(hi*251)+(*s++);
//	hi&=255;

	hi=BGBCC_CCXL_HashName(name)&255;

#ifdef BGBCC_BCCX2

	if(!ctx->structs)
		return(NULL);

	ci=ctx->struct_hash_ix[hi];
	while(ci>=0)
	{
		c=BCCX_GetNodeIndex(ctx->structs, ci);
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
		ci=BCCX_GetIntCst(c, &bgbcc_rcst_Shnext, "$hnext");
	}

	na=BCCX_GetNodeChildCount(ctx->structs);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->structs, ci);

		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
	}

#else

	c=ctx->struct_hash[hi];
	if(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
	}

	c=ctx->structs;
	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
		{
			ctx->struct_hash[hi]=c;
			return(c);
		}
		c=BCCX_Next(c);
	}
#endif

	return(NULL);
}

BCCX_Node *BGBCP_LookupTemplateI(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c;
	int hi, na, ci;
	char *s;

//	hi=BGBCC_CCXL_HashName(name)&255;
	hi=BGBCP_HashTypeNameI(ctx, name);

	if(!ctx->types)
		return(NULL);

	ci=ctx->type_hash_ix[hi];
	while(ci>=0)
	{
		c=BCCX_GetNodeIndex(ctx->types, ci);

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_template, "template"))
		{
			ci=BCCX_GetIntCst(c, &bgbcc_rcst_Shnext, "$hnext");
			continue;
		}
		
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
		ci=BCCX_GetIntCst(c, &bgbcc_rcst_Shnext, "$hnext");
	}

#if 0
	na=BCCX_GetNodeChildCount(ctx->types);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->types, ci);

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_template, "template"))
			continue;

		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
	}
#endif

	return(NULL);
}

BCCX_Node *BGBCP_LookupToplevelI(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *c;
	int hi, na, ci;
	char *s;

	na=BCCX_GetNodeChildCount(ctx->toplevel);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(ctx->toplevel, ci);

//		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_template, "template"))
//			continue;

		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
	}

	return(NULL);
}

BCCX_Node *BGBCP_LookupTemplate(BGBCP_ParseState *ctx, char *name)
{
	return(BGBCP_LookupTemplateI(ctx, name));
}

BCCX_Node *BGBCP_LookupToplevel(BGBCP_ParseState *ctx, char *name)
{
	return(BGBCP_LookupToplevelI(ctx, name));
}

BCCX_Node *BGBCP_LookupTemplateType(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n, *t;

	n=BGBCP_LookupTemplateI(ctx, name);
	if(!n)
		return(NULL);

	t=BCCX_FindTagCst(n, &bgbcc_rcst_type, "type");
	if(!t)
		return(NULL);

	return(n);
}

BCCX_Node *BGBCP_LookupTemplateFunc(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n, *t;

	n=BGBCP_LookupTemplateI(ctx, name);
	if(!n)
		return(NULL);

	t=BCCX_FindTagCst(n, &bgbcc_rcst_defun, "defun");
	if(t)
		return(n);

	t=BCCX_FindTagCst(n, &bgbcc_rcst_proto, "proto");
	if(t)
		return(n);

	return(NULL);
}

int BGBCP_HashTypeNameI(BGBCP_ParseState *ctx, char *name)
{
	int hi;
	char *s;

	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
//	hi=(hi>>8)&1023;
	hi=hi&1023;
	
	return(hi);
}

BCCX_Node *BGBCP_LookupTypeI(BGBCP_ParseState *ctx, char *name)
{
	static char *tk_name=NULL;
	char tb[256];
	BCCX_Node *c;
	int hi, ci;
	char *s, *sn;

#if 0
	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
//	hi=(hi>>8)&1023;
	hi=hi&1023;
#endif

	hi=BGBCP_HashTypeNameI(ctx, name);

	if(!tk_name)tk_name=bgbcc_strdup("name");

#ifdef BGBCC_BCCX2

	ci=ctx->type_hash_ix[hi];
	while(ci>=0)
	{
		c=BCCX_GetNodeIndex(ctx->types, ci);
		
		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			ci=BCCX_GetIntCst(c, &bgbcc_rcst_Shnext, "$hnext");
			continue;
		}
		
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
		ci=BCCX_GetIntCst(c, &bgbcc_rcst_Shnext, "$hnext");
	}

#else

#if 0
	c=ctx->type_hash[hi];
	if(c)
	{
		//s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		s=BCCX_Get_SI(c, tk_name);
		if(s && !bgbcp_strcmp(s, name))
			return(c);
	}
#endif

//	sn=bgbcc_strdup(name);

#if 1
	c=ctx->type_hash[hi];
	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
//		s=BCCX_Get_SI(c, tk_name);
		if(s && !bgbcp_strcmp(s, name))
//		if(s==sn)
			return(c);
		c=c->hnext;
	}
#endif

#if 0
//	c=ctx->types;
	while(c)
	{
//		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		s=BCCX_Get_SI(c, tk_name);
//		if(s && !bgbcp_strcmp(s, name))
		if(s==sn)
		{
//			c->hnext=ctx->type_hash[hi];
			ctx->type_hash[hi]=c;
			return(c);
		}
		c=BCCX_Next(c);
	}
#endif

#endif

	return(NULL);
}

char *BGBCP_GetNamespacePath(BGBCP_ParseState *ctx)
{
	char tbuf[4096];
	char *t;
	int i;
	
	t=tbuf;

#if 0
	if(ctx->cur_ns)
	{
		strcpy(t, ctx->cur_ns);
		t+=strlen(ctx->cur_ns);
		*t++=';';
		*t=0;
	}

	if(ctx->cur_nsi)
	{
		for(i=(ctx->n_cur_nsi-1); i>=0; i--)
		{
			strcpy(t, ctx->cur_nsi[i]);
			t+=strlen(ctx->cur_nsi[i]);
			*t++=';';
			*t=0;
		}
	}
#endif

#if 1
	if(ctx->cur_nsi)
	{
		for(i=0; i<ctx->n_cur_nsi; i++)
		{
			strcpy(t, ctx->cur_nsi[i]);
			t+=strlen(ctx->cur_nsi[i]);
			*t++=';';
			*t=0;
		}
	}

	if(ctx->cur_ns)
	{
		strcpy(t, ctx->cur_ns);
		t+=strlen(ctx->cur_ns);
		*t++=';';
		*t=0;
	}
#endif
	
	*t=0;
	return(bgbcc_strdup(tbuf));
}

BCCX_Node *BGBCP_LookupStruct(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c;
	char *s;
	int i;

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		c=BGBCP_LookupStructI(ctx, tb);
		if(c)return(c);
	}

	if(ctx->cur_nsi)
	{
//		for(i=0; ctx->cur_nsi[i]; i++)
		for(i=(ctx->n_cur_nsi-1); i>=0; i--)
		{
			sprintf(tb, "%s/%s", ctx->cur_nsi[i], name);
			c=BGBCP_LookupStructI(ctx, tb);
			if(c)return(c);
		}
	}

	c=BGBCP_LookupStructI(ctx, name);
	if(c)return(c);

	return(NULL);
}

#if 0
BCCX_Node *BGBCP_LookupTypeI(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *c;
	char *s;

//	c=ctx->types;
	while(c)
	{
		s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		if(s && !bgbcp_strcmp(s, name))
			return(c);
		c=BCCX_Next(c);
	}
	return(NULL);
}
#endif

BCCX_Node *BGBCP_LookupType(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c, *a, *ty, *n;
	char *s, *tn;
	int i, na, ci;

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		c=BGBCP_LookupTypeI(ctx, tb);
		if(c)return(c);
	}

	if(ctx->cur_nsi)
	{
//		for(i=0; ctx->cur_nsi[i]; i++)
		for(i=(ctx->n_cur_nsi-1); i>=0; i--)
		{
			sprintf(tb, "%s/%s", ctx->cur_nsi[i], name);
			c=BGBCP_LookupTypeI(ctx, tb);
			if(c)return(c);
		}
	}

	c=BGBCP_LookupTypeI(ctx, name);
	if(c)return(c);

	if(ctx->template_stackpos>0)
	{
		i=ctx->template_stackpos;
		while((i--)>0)
		{
			a=ctx->template_stack[i];

			na=BCCX_GetNodeChildCount(a);
			for(ci=0; ci<na; ci++)
			{
				c=BCCX_GetNodeIndex(a, ci);

				if(BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
				{
					ty=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
					s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
					if(!ty || !s)
						continue;

					tn=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");
					
					if(!strcmp(s, name) && tn && !strcmp(tn, "class"))
					{
						sprintf(tb, "$.%s", name);
					
						n=BCCX_NewCst(&bgbcc_rcst_type, "type");
						BCCX_SetCst(n, &bgbcc_rcst_name, "name", tb);
						BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", 0);
						BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

						n=BCCX_NewCst1(&bgbcc_rcst_var, "var", n);

						return(n);
					}
				}
			}
		}
	}

	return(NULL);
}


int BGBCP_HandleTypedef(BGBCP_ParseState *ctx, BCCX_Node *n)
{
	BCCX_Node *c, *t, *n0, *n1, *n2, *n3;
	char *s, *s1, *s2;
	int i, j, k, ci, na;

	if(BCCX_TagIsCstP(n, &bgbcc_rcst_vars, "vars"))
	{
// #ifndef BGBCC_BCCX2
#if 0
		c=BCCX_Child(n);
		while(c)
		{
#else
		na=BCCX_GetNodeChildCount(n);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(n, ci);
#endif

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_proto, "proto"))
			{
				n2=BCCX_FetchCst(c, &bgbcc_rcst_args, "args");
				n3=BCCX_FindTagCst(c, &bgbcc_rcst_type, "type");
				s2=BGBCC_GenSym();

				n0=BCCX_NewCst(&bgbcc_rcst_func, "func");
				BCCX_SetCst(n0, &bgbcc_rcst_name, "name", s2);

				BCCX_Add(n0, BCCX_Clone(n3));
				BCCX_AddV(n0, BCCX_NewCst1(&bgbcc_rcst_args, "args", n2));
				ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n0);

				n1=BCCX_NewCst(&bgbcc_rcst_type, "type");
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", s2);
				BCCX_SetIntCst(n1, &bgbcc_rcst_flags, "flags", 0);
				BCCX_SetIntCst(n1, &bgbcc_rcst_ind, "ind", 0);

				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_NewCst(&bgbcc_rcst_var, "var");
				BCCX_SetCst(t, &bgbcc_rcst_name, "name", s);
				BCCX_Add(t, n1);
			}else
			{
				t=BCCX_Clone(c);
			}

#if 1
			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			if(!s)
			{
//				c=BCCX_Next(c);
				continue;
			}
			i=BGBCP_HashTypeNameI(ctx, s);

#ifdef BGBCC_BCCX2
			if(!ctx->types)
				ctx->types=BCCX_NewList();
			j=ctx->type_hash_ix[i];
			k=BCCX_Add(ctx->types, t);
			if(k>=0)
			{
				ctx->type_hash_ix[i]=k;
				BCCX_SetIntCst(t, &bgbcc_rcst_Shnext, "$hnext", j);	
			}
#else
			t->hnext=ctx->type_hash[i];
			ctx->type_hash[i]=t;
			ctx->types=BCCX_AddEnd2(ctx->types, &ctx->e_types, t);
//			c=BCCX_Next(c);
#endif

#endif		

		}
		return(0);
	}

#ifndef BGBCC_BCCX2
	BCCX_Print(n);
#endif

	BGBCP_Error(NULL, "BGBCP_HandleTypedef: "
			"Bad typedef definition\n");
	return(-1);
}

int BGBCP_AddDefToplevel(BGBCP_ParseState *ctx, BCCX_Node *n)
{
	if(!ctx->toplevel)
		ctx->toplevel=BCCX_NewList();
	BCCX_Add(ctx->toplevel, n);
	return(0);
}

int BGBCP_HandleTemplate(BGBCP_ParseState *ctx,
	BCCX_Node *def, BCCX_Node *args)
{
	BCCX_Node *n, *c, *ty;
	char *s, *tag;
	int na, ci;
	int i, j, k;

	tag=BCCX_Tag(def);

	if(	BCCX_TagIsCstP(def, &bgbcc_rcst_proto, "proto") ||
		BCCX_TagIsCstP(def, &bgbcc_rcst_defun, "defun"))
	{
		s=BCCX_GetCst(def, &bgbcc_rcst_name, "name");

		n=BCCX_NewCst(&bgbcc_rcst_template, "template");

		BCCX_Add(n, BCCX_Clone(def));
	//	BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_body, "body", def));
		BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_args, "args", args));
	}else
		if(BCCX_TagIsCstP(def, &bgbcc_rcst_var, "var"))
	{
		ty=BCCX_FindTagCst(def, &bgbcc_rcst_type, "type");
		if(!ty)
			{ return(-1); }

		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		n=BCCX_NewCst(&bgbcc_rcst_template, "template");

		BCCX_Add(n, BCCX_Clone(ty));
	//	BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_body, "body", def));
		BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_args, "args", args));
	}else
		if(BCCX_TagIsCstP(def, &bgbcc_rcst_vars, "vars"))
	{
		na=BCCX_GetNodeChildCount(def);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(def, ci);
			BGBCP_HandleTemplate(ctx, c, args);
		}
		
		return(0);
//		BGBCC_DBGBREAK
	}else
	{
		BGBCC_DBGBREAK
	}

	if(!s)
		{ return(-1); }

	if(*s=='!')
		s++;

	BCCX_SetCst(n, &bgbcc_rcst_name, "name", s);

//	s=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
	i=BGBCP_HashTypeNameI(ctx, s);

	if(!ctx->types)
		ctx->types=BCCX_NewList();
	j=ctx->type_hash_ix[i];
	k=BCCX_Add(ctx->types, n);
	if(k>=0)
	{
		ctx->type_hash_ix[i]=k;
		BCCX_SetIntCst(n, &bgbcc_rcst_Shnext, "$hnext", j);	
	}

	return(0);
}

int BGBCP_PushTemplateArgs(BGBCP_ParseState *ctx,
	BCCX_Node *args)
{
	int i;
	
	i=ctx->template_stackpos++;
	ctx->template_stack[i]=args;
	return(0);
}

int BGBCP_PopTemplateArgs(BGBCP_ParseState *ctx)
{
	if(ctx->template_stackpos>0)
		ctx->template_stackpos--;
	return(0);
}

BCCX_Node *BGBCP_GetStruct(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;
	int i, j;

	if(!strcmp(name, "bot_matchvariable_s"))
		{ j=-1; }

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_NewCst(&bgbcc_rcst_struct, "struct");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", name);

#ifdef BGBCC_BCCX2
	BCCX_Add(ctx->structs, n);
#else
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
#endif
	return(n);
}

BCCX_Node *BGBCP_GetUnion(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_NewCst(&bgbcc_rcst_union, "union");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", name);

#ifdef BGBCC_BCCX2
	BCCX_Add(ctx->structs, n);
#else
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
#endif

	return(n);
}

BCCX_Node *BGBCP_GetEnum(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", name);

#ifdef BGBCC_BCCX2
	BCCX_Add(ctx->structs, n);
#else
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
#endif

	return(n);
}

BCCX_Node *BGBCP_GetStructJ(BGBCP_ParseState *ctx, char *name, int ty)
{
	char tb[256];
	BCCX_Node *n, *n1, *nt;
	char *nsp;
	char *s, *s1;
	int i, j, k;

	if(!strcmp(name, "bot_matchvariable_s"))
		{ i=-1; }

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	s1=name;
	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		s1=bgbcc_strdup(tb);
	}

	nsp=BGBCP_GetNamespacePath(ctx);

	n=NULL;
	if(ty==1)
		{ n=BCCX_NewCst(&bgbcc_rcst_struct, "struct"); }
	else if(ty==2)
		{ n=BCCX_NewCst(&bgbcc_rcst_union, "union"); }
	else if(ty==3)
		{ n=BCCX_NewCst(&bgbcc_rcst_class, "class"); }
	else if(ty==4)
		{ n=BCCX_NewCst(&bgbcc_rcst_interface, "interface"); }
	else if(ty==5)
		{ n=BCCX_NewCst(&bgbcc_rcst_enum, "enum"); }

	if(!n)
		return(NULL);

	BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
	if(nsp && (*nsp))
		{ BCCX_SetCst(n, &bgbcc_rcst_nspath, "nspath", nsp); }

// #ifdef BGBCC_BCCX2
#if 0
	BCCX_Add(ctx->structs, n);
#else
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
#endif

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		n1=n;
//		s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
		nt=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(nt, &bgbcc_rcst_name, "name", s1);
//		BCCX_SetIntCst(nt, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(nt, &bgbcc_rcst_flags, "flags", 0);
		BCCX_SetIntCst(nt, &bgbcc_rcst_ind, "ind", 0);

		n1=BCCX_NewCst1(&bgbcc_rcst_var, "var", nt);
		BCCX_SetCst(n1, &bgbcc_rcst_name, "name", s1);

#ifdef BGBCC_BCCX2
		if(!ctx->types)
			ctx->types=BCCX_NewList();

		i=BGBCP_HashTypeNameI(ctx, s1);
		j=ctx->type_hash_ix[i];
		k=BCCX_Add(ctx->types, n1);
		ctx->type_hash_ix[i]=k;
		BCCX_SetIntCst(n1, &bgbcc_rcst_Shnext, "$hnext", j);
#else
		ctx->types=BCCX_AddEnd2(ctx->types, &ctx->e_types, n1);

		i=BGBCP_HashTypeNameI(ctx, s1);
		n1->hnext=ctx->type_hash[i];
		ctx->type_hash[i]=n1;
#endif

	}

	return(n);
}

int BGBCP_CheckTypeName(BGBCP_ParseState *ctx, char *name)
{
	char *s;
	s=BGBCP_LookupTypeSig(ctx, name);
	if(s)return(1);
	if(ctx->expect_type)
		return(1);
	return(0);
}


static char *bgbcp_basetypes_c[]={
"char", "short", "int", "long", "float", "double", "void",
"__int8",	"__int16",	"__int32",	"__int64",
"__uint8",	"__uint16",	"__uint32",	"__uint64",
"__int128",	"__uint128",

"__intptr",	"__uintptr",
"__ssize_t",	"__usize_t",
"__ptrdiff",

"__float32", "__float64",
"__float128", "__float16",
"__m64", "__m128", "__m128p",
// "__vec2", "__vec3", "__vec4", "__quat", 
"__vec2f", "__vec3f", "__vec4f", "__quatf", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
// "__mat2", "__mat3", "__mat4",
"__mat2f", "__mat3f", "__mat4f",

 "__vec4w", "__vec4sw", "__vec4uw",
 "__vec4i", "__vec4si", "__vec4ui",
 "__vec4h", "__vec4sf",
 "__vec3h", "__vec3sf",
 
"__bfloat16",

"__fcomplex",
"__dcomplex",
"__hcomplex",

"__vec3fx",
"__vec3xf",
"__vec3fq",

// "_Float128",	"_Float64",
// "_Float32",	"_Float16",

// "__v2f", "__v3f", "__v4f",
// "__m2f", "__m3f", "__m4f",
"__variant", "__variantf",
"__variant_va", "__variant_this", "__variant_cls",
"__gnuc_va_list", "__builtin_va_list",
"_Bool",
"__auto",	"__var",

"_BitInt",

"__object",
"__string",
"__number",

"__fixnum",
"__flonum",
"__boxint",

"__bcd64",
"__bcd128",

"__float8s",
"__float8u",

"__vec4fp8s",
"__vec4fp8u",

NULL
};

static char *bgbcp_basetypes_j[]={
"char", "byte",
"short", "ushort",
"int", "uint",
"long", "ulong",
"float", "double",
"void", "string",
"variant",
"hfloat",
"boolean",

"int128", "uint128",
"float128",

"__int128", "__uint128",
"__float128", "__float16", "__m64", "__m128",
"__vec2", "__vec3", "__vec4", "__quat", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
"__mat2", "__mat3", "__mat4",

"__v2f", "__v3f", "__v4f",
"__m2f", "__m3f", "__m4f",
"__variantf",
NULL
};

static char *bgbcp_basetypes_cs[]={
"char", "byte", "sbyte", "ubyte",
"short", "ushort",
"int", "uint",
"long", "ulong",
"float", "double",
"void", "string",
"variant",
"decimal",
"bool",
"hfloat",

"int128", "uint128",
"float128",

"object",

"__int128", "__uint128",
"__float128", "__float16", "__m64", "__m128",
"__vec2", "__vec3", "__vec4", "__quat", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
"__mat2", "__mat3", "__mat4",

"__v2f", "__v3f", "__v4f",
"__m2f", "__m3f", "__m4f",
"__variantf",
NULL
};

static char *bgbcp_basetypes_bs2[]={
"char", "byte", "sbyte", "ubyte",
"short", "ushort",
"int", "uint",
"long", "ulong",
"float", "double",
"void", "string",
"var", "variant",
"hfloat",

"int128", "uint128",
"float128",
"bool",
"object",
"auto",
"cstring",
"wstring",
"ustring",

"__int128", "__uint128",
"__float128", "__float16", "__m64", "__m128",
"__vec2", "__vec3", "__vec4", "__quat", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
"__mat2", "__mat3", "__mat4",

"__v2f", "__v3f", "__v4f",
"__m2f", "__m3f", "__m4f",
"__variantf",
NULL
};

static char *bgbcp_nontypes_bs2[]={
"break",		"case",
"catch",		"class",
"continue",		"default",
"do",			"else",
"enum",			"for",
"goto",			"if",
"import",		"interface",
"namespace",	"new",
"package",		"return",
"string",		"struct",
"switch",		"throw",
"try",			"union",
"using",		"while",
NULL
};

static char *bgbcp_nontypes_cs[]={
"break",		"case",
"catch",		"class",
"continue",		"default",
"do",			"else",
"enum",			"for",
"goto",			"if",
"interface",	"namespace",
"new",			"return",
"string",		"struct",
"switch",		"throw",
"try",			"union",
"using",		"while",
NULL
};

static char *bgbcp_nontypes_c[]={
"break",		"case",
"catch",		"continue",
"default",		"do",
"else",			"enum",
"for",			"goto",
"if",			"return",
"struct",		"switch",
"union",		"while",
NULL
};

static short bgbcp_basetypes_c_hix[512];

static int bgbcp_chktoklst(char *str, char **lst)
{
	char *s;
	int i;

	for(i=0; lst[i]; i++)
		if(!bgbcp_strcmp(lst[i], str))
			return(1);

	s="__type_";
	if(!strncmp(str, s, strlen(s)))
		return(1);

	return(0);
}

static int bgbcp_chktoklst_hix(char *str, char **lst, short *hix)
{
	char *s;
	int i, h;
	
	if(!hix[0])
	{
		for(i=0; i<256; i++)
			hix[i]=-1;

		for(i=0; lst[i]; i++)
		{
			s=lst[i];
			h=*(u32 *)s;
			h=((h*65521)>>16)&255;
			
			hix[256+i]=hix[h];
			hix[h]=256+i;
		}
	}

	h=*(u32 *)str;
	h=((h*65521)>>16)&255;
	i=hix[h];
	while(i>0)
	{
		if(!bgbcp_strcmp(lst[i-256], str))
			return(1);
		i=hix[i];
	}

	if(str[0]=='_')
	{
		s="__type_";
//		if(!strncmp(str, s, strlen(s)))
		if(!strncmp(str, s, 7))
			return(1);
	}

	return(0);

#if 0
	for(i=0; lst[i]; i++)
		if(!bgbcp_strcmp(lst[i], str))
			return(1);

	s="__type_";
	if(!strncmp(str, s, strlen(s)))
		return(1);

	return(0);
#endif
}

int BGBCP_CheckTokenKeyword(BGBCP_ParseState *ctx, char *tag)
{
	if(BGBCP_DefTypeFlag(ctx, tag))
		return(1);

	if(ctx->lang==BGBCC_LANG_C)
	{
		if(bgbcp_chktoklst(tag, bgbcp_basetypes_c))
			return(1);
		if(bgbcp_chktoklst(tag, bgbcp_nontypes_c))
			return(1);
	}

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		if(bgbcp_chktoklst(tag, bgbcp_basetypes_c))
			return(1);
		if(bgbcp_chktoklst(tag, bgbcp_nontypes_cs))
			return(1);
	}

	if(ctx->lang==BGBCC_LANG_CS)
	{
		if(bgbcp_chktoklst(tag, bgbcp_basetypes_cs))
			return(1);
		if(bgbcp_chktoklst(tag, bgbcp_nontypes_cs))
			return(1);
	}

	if(ctx->lang==BGBCC_LANG_BS2)
	{
		if(bgbcp_chktoklst(tag, bgbcp_basetypes_bs2))
			return(1);
		if(bgbcp_chktoklst(tag, bgbcp_nontypes_bs2))
			return(1);
	}

	return(0);
}

s64 BGBCP_DefTypeFlag(BGBCP_ParseState *ctx, char *tag)
{
	s64 i, n, c0;

	i=0;

#if 0
	n=BGBCC_GetOrdinalLang(tag, ctx->lang);
	if(n<0)return(0);

	switch(n)
	{
	case BOTK_STATIC:			i=BGBCC_TYFL_STATIC; break;
	case BOTK_VOLATILE:			i=BGBCC_TYFL_VOLATILE; break;

	case BOTK_SIGNED:			i=BGBCC_TYFL_SIGNED; break;
	case BOTK_UNSIGNED:			i=BGBCC_TYFL_UNSIGNED; break;
	case BOTK_EXTERN:			i=BGBCC_TYFL_EXTERN; break;
	case BOTK_CONST:			i=BGBCC_TYFL_CONST; break;

	case BOTK_VIRTUAL:			i=BGBCC_TYFL_VIRTUAL; break;

	case BOTK_PUBLIC:			i=BGBCC_TYFL_PUBLIC; break;
	case BOTK_PRIVATE:			i=BGBCC_TYFL_PRIVATE; break;
	case BOTK_PROTECTED:		i=BGBCC_TYFL_PROTECTED; break;

	case BOTK_FINAL:			i=BGBCC_TYFL_FINAL; break;
	case BOTK_NATIVE:			i=BGBCC_TYFL_NATIVE; break;
	case BOTK_ABSTRACT:			i=BGBCC_TYFL_ABSTRACT; break;

	case BOTK__COMPLEX:			i=BGBCC_TYFL_UNSIGNED; break;
	case BOTK__IMAGINARY:		i=BGBCC_TYFL_UNSIGNED; break;

	case BOTK_INLINE:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___INLINE:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___FORCEINLINE:	i=BGBCC_TYFL_INLINE; break;

	case BOTK___STDCALL:		i=BGBCC_TYFL_STDCALL; break;
	case BOTK___CDECL:			i=BGBCC_TYFL_CDECL; break;
	case BOTK___PROXY:			i=BGBCC_TYFL_PROXY; break;

	case BOTK___W64:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___PTR32:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___PTR64:			i=BGBCC_TYFL_INLINE; break;

	case BOTK___WIN64:			i=BGBCC_TYFL_WIN64; break;
	case BOTK___AMD64:			i=BGBCC_TYFL_AMD64; break;
	case BOTK___XCALL:			i=BGBCC_TYFL_XCALL; break;

	case BOTK___PACKED:			i=BGBCC_TYFL_PACKED; break;
//	case BOTK___GC:				i=BGBCC_TYFL_GC; break;
//	case BOTK___NOGC:			i=BGBCC_TYFL_NOGC; break;

	case BOTK___WIDE:			i=BGBCC_TYFL_PERSISTENT; break;
		
	case BOTK___PUBLIC:			i=BGBCC_TYFL_PUBLIC; break;
	case BOTK___PRIVATE:		i=BGBCC_TYFL_PRIVATE; break;
	case BOTK___PROTECTED:		i=BGBCC_TYFL_PROTECTED; break;

	case BOTK___FINAL:			i=BGBCC_TYFL_FINAL; break;

	case BOTK___VIRTUAL:		i=BGBCC_TYFL_VIRTUAL; break;
	case BOTK___NATIVE:			i=BGBCC_TYFL_NATIVE; break;
	case BOTK___ABSTRACT:		i=BGBCC_TYFL_ABSTRACT; break;

	default: break;
	}

	if(i)return(i);

	return(0);
#endif

	c0=*tag;

//	if(!bgbcp_strcmp(tag, "const"))i=BGBCC_TYFL_STATIC;

//	if(n==BOTK_STATIC)i=BGBCC_TYFL_STATIC;
//	if(n==BOTK_VOLATILE)i=BGBCC_TYFL_VOLATILE;

	if(	(ctx->lang==BGBCC_LANG_C) ||
		(ctx->lang==BGBCC_LANG_CPP))
	{
#if 1
		if(c0=='s')
		{
			if(!bgbcp_strcmp(tag, "static"))i=BGBCC_TYFL_STATIC;
			if(!bgbcp_strcmp(tag, "signed"))i=BGBCC_TYFL_SIGNED;
		}else if(c0=='v')
		{
			if(!bgbcp_strcmp(tag, "volatile"))i=BGBCC_TYFL_VOLATILE;
		}else if(c0=='u')
		{
			if(!bgbcp_strcmp(tag, "unsigned"))i=BGBCC_TYFL_UNSIGNED;
		}else if(c0=='e')
		{
			if(!bgbcp_strcmp(tag, "extern"))i=BGBCC_TYFL_EXTERN;
		}else if(c0=='c')
		{
			if(!bgbcp_strcmp(tag, "const"))i=BGBCC_TYFL_CONST;
		}else if(c0=='i')
		{
			if(!bgbcp_strcmp(tag, "inline"))i=BGBCC_TYFL_INLINE;
		}else if(c0=='r')
		{
			if(!bgbcp_strcmp(tag, "register"))i=BGBCC_TYFL_INLINE;
			if(!bgbcp_strcmp(tag, "restrict"))i=BGBCC_TYFL_RESTRICT;
		}else if(c0=='a')
		{
			if(!bgbcp_strcmp(tag, "auto"))i=BGBCC_TYFL_AUTO;
		}else if(c0=='_')
		{
			if(tag[1]=='_')
			{
				if(!bgbcp_strcmp(tag, "__const"))i=BGBCC_TYFL_CONST;

				if(!bgbcp_strcmp(tag, "__inline"))i=BGBCC_TYFL_INLINE;
				if(!bgbcp_strcmp(tag, "__forceinline"))i=BGBCC_TYFL_INLINE;

				if(!bgbcp_strcmp(tag, "__stdcall"))i=BGBCC_TYFL_STDCALL;
				if(!bgbcp_strcmp(tag, "__cdecl"))i=BGBCC_TYFL_CDECL;
				if(!bgbcp_strcmp(tag, "__proxy"))i=BGBCC_TYFL_PROXY;

				if(!bgbcp_strcmp(tag, "__interrupt"))i=BGBCC_TYFL_INTERRUPT;
				if(!bgbcp_strcmp(tag, "__syscall"))i=BGBCC_TYFL_SYSCALL;

				if(!bgbcp_strcmp(tag, "__w64"))i=BGBCC_TYFL_INLINE;
				if(!bgbcp_strcmp(tag, "__ptr64"))i=BGBCC_TYFL_INLINE;
				if(!bgbcp_strcmp(tag, "__ptr32"))i=BGBCC_TYFL_INLINE;

				if(!bgbcp_strcmp(tag, "__win64"))i=BGBCC_TYFL_WIN64;
				if(!bgbcp_strcmp(tag, "__amd64"))i=BGBCC_TYFL_AMD64;
				if(!bgbcp_strcmp(tag, "__xcall"))i=BGBCC_TYFL_XCALL;

				if(!bgbcp_strcmp(tag, "__packed"))i=BGBCC_TYFL_PACKED;
//				if(!bgbcp_strcmp(tag, "__gc"))i=BGBCC_TYFL_GC;
//				if(!bgbcp_strcmp(tag, "__nogc"))i=BGBCC_TYFL_NOGC;
				if(!bgbcp_strcmp(tag, "__unaligned"))i=BGBCC_TYFL_PACKED;
				if(!bgbcp_strcmp(tag, "__restrict"))i=BGBCC_TYFL_RESTRICT;

				if(!bgbcp_strcmp(tag, "__wide"))i=BGBCC_TYFL_WIDE;

				if(!bgbcp_strcmp(tag, "__ltlendian"))i=BGBCC_TYFL_LTLENDIAN;
				if(!bgbcp_strcmp(tag, "__bigendian"))i=BGBCC_TYFL_BIGENDIAN;

				if(!bgbcp_strcmp(tag, "__transient"))i=BGBCC_TYFL_ABSTRACT;
				if(!bgbcp_strcmp(tag, "__thread"))i=BGBCC_TYFL_THREAD;
				if(!bgbcp_strcmp(tag, "__dynamic"))i=BGBCC_TYFL_DYNAMIC;

				if(!bgbcp_strcmp(tag, "__near"))i=BGBCC_TYFL_NEAR;
				if(!bgbcp_strcmp(tag, "__far"))i=BGBCC_TYFL_FAR;
				if(!bgbcp_strcmp(tag, "__huge"))i=BGBCC_TYFL_HUGE;

//				if(!bgbcp_strcmp(tag, "__getter"))i=BGBCC_TYFL_GETTER;
//				if(!bgbcp_strcmp(tag, "__setter"))i=BGBCC_TYFL_SETTER;
			}else
			{
				if(!bgbcp_strcmp(tag, "_Complex"))i=BGBCC_TYFL_UNSIGNED;
//				if(!bgbcp_strcmp(tag, "_Imaginary"))i=BGBCC_TYFL_UNSIGNED;
				if(!bgbcp_strcmp(tag, "_Imaginary"))i=BGBCC_TYFL_SIGNED;

				if(!bgbcp_strcmp(tag, "_Near"))i=BGBCC_TYFL_NEAR;
				if(!bgbcp_strcmp(tag, "_Far"))i=BGBCC_TYFL_FAR;
				if(!bgbcp_strcmp(tag, "_Huge"))i=BGBCC_TYFL_HUGE;

				if(!bgbcp_strcmp(tag, "_Thread_local"))
					i=BGBCC_TYFL_THREAD;
				if(!bgbcp_strcmp(tag, "_Noreturn"))
					i=BGBCC_TYFL_ABSTRACT;
			}
		}
#endif
	}else
	{
		if((c0=='s') && !bgbcp_strcmp(tag, "static"))
			i=BGBCC_TYFL_STATIC;
	//	if(!bgbcp_strcmp(tag, "const"))i=BGBCC_TYFL_STATIC;
		if((c0=='v') && !bgbcp_strcmp(tag, "volatile"))
			i=BGBCC_TYFL_VOLATILE;
	}

	if(ctx->lang==BGBCC_LANG_C)
	{
		if(c0=='_')
		{
			if(!bgbcp_strcmp(tag, "__public"))i=BGBCC_TYFL_PUBLIC;
			if(!bgbcp_strcmp(tag, "__private"))i=BGBCC_TYFL_PRIVATE;
			if(!bgbcp_strcmp(tag, "__protected"))i=BGBCC_TYFL_PROTECTED;
			if(!bgbcp_strcmp(tag, "__final"))i=BGBCC_TYFL_FINAL;

			if(!bgbcp_strcmp(tag, "__virtual"))i=BGBCC_TYFL_VIRTUAL;
			if(!bgbcp_strcmp(tag, "__native"))i=BGBCC_TYFL_NATIVE;
			if(!bgbcp_strcmp(tag, "__abstract"))i=BGBCC_TYFL_ABSTRACT;

			if(!bgbcp_strcmp(tag, "__byref"))i=BGBCC_TYFL_BYREF;
			if(!bgbcp_strcmp(tag, "__delegate"))i=BGBCC_TYFL_DELEGATE;
		}

//		if(i)
		return(i);
	}

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		if((c0=='v') && !bgbcp_strcmp(tag, "virtual"))
			i=BGBCC_TYFL_VIRTUAL;
	}

	if(	(ctx->lang==BGBCC_LANG_CPP) ||
		(ctx->lang==BGBCC_LANG_JAVA) ||
		(ctx->lang==BGBCC_LANG_CS))
	{
		if(!bgbcp_strcmp(tag, "public"))i=BGBCC_TYFL_PUBLIC;
		if(!bgbcp_strcmp(tag, "private"))i=BGBCC_TYFL_PRIVATE;
		if(!bgbcp_strcmp(tag, "protected"))i=BGBCC_TYFL_PROTECTED;

		if(!bgbcp_strcmp(tag, "abstract"))i=BGBCC_TYFL_ABSTRACT;
	}

	if(ctx->lang==BGBCC_LANG_CS)
	{
		if(!bgbcp_strcmp(tag, "virtual"))i=BGBCC_TYFL_VIRTUAL;
		if(!bgbcp_strcmp(tag, "new"))i=BGBCC_TYFL_VIRTUAL;
		if(!bgbcp_strcmp(tag, "override"))i=BGBCC_TYFL_VIRTUAL;
		
		if(!bgbcp_strcmp(tag, "internal"))i=BGBCC_TYFL_INLINE;
		if(!bgbcp_strcmp(tag, "const"))i=BGBCC_TYFL_CONST;

		if(!bgbcp_strcmp(tag, "sealed"))i=BGBCC_TYFL_FINAL;
		if(!bgbcp_strcmp(tag, "readonly"))i=BGBCC_TYFL_FINAL;
	}

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS))

	if(ctx->lang==BGBCC_LANG_JAVA)
	{
		if(!bgbcp_strcmp(tag, "final"))i=BGBCC_TYFL_FINAL;

		if(!bgbcp_strcmp(tag, "transient"))i=BGBCC_TYFL_TRANSIENT;
		if(!bgbcp_strcmp(tag, "native"))i=BGBCC_TYFL_NATIVE;

		if(!bgbcp_strcmp(tag, "abstract"))i=BGBCC_TYFL_ABSTRACT;
	}

	if(ctx->lang==BGBCC_LANG_BS2)
	{
		if(!bgbcp_strcmp(tag, "public"))
			i=BGBCC_TYFL_PUBLIC;
		if(!bgbcp_strcmp(tag, "private"))
			i=BGBCC_TYFL_PRIVATE;
		if(!bgbcp_strcmp(tag, "protected"))
			i=BGBCC_TYFL_PROTECTED;

		if(!bgbcp_strcmp(tag, "abstract"))i=BGBCC_TYFL_ABSTRACT;
		if(!bgbcp_strcmp(tag, "virtual"))i=BGBCC_TYFL_VIRTUAL;

		if(!bgbcp_strcmp(tag, "final"))i=BGBCC_TYFL_FINAL;
		if(!bgbcp_strcmp(tag, "const"))i=BGBCC_TYFL_CONST;

		if(!bgbcp_strcmp(tag, "transient"))i=BGBCC_TYFL_TRANSIENT;
		if(!bgbcp_strcmp(tag, "native"))i=BGBCC_TYFL_NATIVE;

		if(!bgbcp_strcmp(tag, "typedef"))i=BGBCC_TYFL_TYPEDEF;

		if(!bgbcp_strcmp(tag, "byref"))i=BGBCC_TYFL_BYREF;
		if(!bgbcp_strcmp(tag, "delegate"))i=BGBCC_TYFL_DELEGATE;
	}

	return(i);
}

BCCX_Node *BGBCP_DefClassC(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1, *s2;
	s64 dfl_fl, fl, li;
	int i, j, ty, ty2, cty;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

//	bty=NULL;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		li=BGBCP_DefTypeFlag(ctx, b);
		if(li)
		{
			fl|=li;
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			j++;
			continue;
		}

		break;
	}

	if(	!bgbcp_strcmp6(b, "struct") ||
		!bgbcp_strcmp5(b, "union") ||
		!bgbcp_strcmp5(b, "class") ||
		!bgbcp_strcmp9(b, "interface") ||
		!bgbcp_strcmp7(b, "__class") ||
		!bgbcp_strcmp11(b, "__interface"))
	{
//		j=!bgbcp_strcmp(b, "struct");

		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//struct
		
		dfl_fl=ctx->dfl_flags;

		ctx->dfl_flags=BGBCC_TYFL_PUBLIC;

		cty=0;
		if(!bgbcp_strcmp6(b, "struct"))cty=1;
		if(!bgbcp_strcmp5(b, "union"))cty=2;

		if(!bgbcp_strcmp(b, "class"))
		{
			cty=3;	//C++, class==struct
			ctx->dfl_flags=BGBCC_TYFL_PROTECTED;

			if(ctx->lang==BGBCC_LANG_CPP)
			{
				cty=1;	//C++, class==struct
				ctx->dfl_flags=BGBCC_TYFL_PRIVATE;
			}
//			if(fl&BGBCC_TYFL_GC)cty=3;
//				else fl|=BGBCC_TYFL_CLASS;
//			if(!(fl&BGBCC_TYFL_GC))
//				fl|=BGBCC_TYFL_NOGC;

		}

//		if(!bgbcp_strcmp(b, "interface"))cty=4;

		if(!bgbcp_strcmp(b, "__class"))
		{
			ctx->dfl_flags=BGBCC_TYFL_PROTECTED;
			cty=3;
		}
		if(!bgbcp_strcmp(b, "__interface"))cty=4;

		nl=NULL;
		while(1)
		{
			n1=BGBCP_DeclAttributeC(ctx, &s);
			if(!n1)break;
			nl=BCCX_AddEnd(nl, n1);
		}

		s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
		BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...

		n=NULL;
		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name

			if(ctx->template_stackpos)
			{
				sprintf(b2, "!%s", b);
				n=BGBCP_GetStructJ(ctx, b2, cty);
			}
			else
			{
				n=BGBCP_GetStructJ(ctx, b, cty);
			}

			s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
			BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...
		}

		if(n && !bgbcp_strcmp1(b, ":"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//':'

			n1=NULL;
			if(!n1)n1=BCCX_NewCst(&bgbcc_rcst_super, "super");
			while(1)
			{
				n2=BGBCP_DefType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token2(s, b, &ty, ctx->lang);
				if(bgbcp_strcmp1(b, ","))break;
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
			}
			BCCX_Add(n, n1);
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(n && (
			!bgbcp_strcmp(b, "extends") ||
			!bgbcp_strcmp(b, "__extends")))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'extends'

//			n1=BGBCP_DefType(ctx, &s);
			n1=BGBCP_DefExpectType(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_super, "super", n1));
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(n && (
			!bgbcp_strcmp(b, "implements") ||
			!bgbcp_strcmp(b, "__implements")))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'implements'

			n1=BCCX_NewCst(&bgbcc_rcst_impl, "impl");

			while(1)
			{
//				n2=BGBCP_DefType(ctx, &s);
				n2=BGBCP_DefExpectType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token2(s, b, &ty, ctx->lang);
				if(bgbcp_strcmp1(b, ","))
					break;
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//','
			}
			BCCX_Add(n, n1);
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(!n)
		{
			s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
			BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...

			if(!bgbcp_strcmp1(b, "{"))
			{
//				s1=s;
				i=0; j=1;
				while(*s1 && j)
				{
					s1=BGBCP_Token2(s1, b2, &ty2, ctx->lang);
					if(!bgbcp_strcmp1(b2, "{"))j++;
					if(!bgbcp_strcmp1(b2, "}"))j--;
					s2=b2;
					while(*s2)i=i*127+(*s2++);
					i=i*127+' ';
				}
				sprintf(b2, "HTX%08X", i);
				s1=bgbcc_strdup(b2);
			}else
			{
				s1=BGBCP_GetLastFileName();
				i=BGBCP_GetLastLineNumber();
				sprintf(b2, "%s:%d", s1, i);
				s1=b2; i=0; while(*s1)i=i*127+(*s1++);

				sprintf(b2, "HFN%08X", i);
				s1=bgbcc_strdup(b2);
			}


//			s1=BGBCC_GenSym2();
			n=BGBCP_GetStructJ(ctx, s1, cty);
		}

		if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_Block(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_body, "body", n1));

//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}

		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);

		n1=n;
		s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
		n=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

		if(nl)BCCX_AddV(n,
			BCCX_NewCst1(&bgbcc_rcst_attributes, "attributes", nl));

		ctx->dfl_flags=dfl_fl;

		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp4(b, "enum"))
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//enum
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name|'{'
		BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name|'{'|...

		n=NULL;
		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			n=BGBCP_GetEnum(ctx, b);
//			n=BGBCP_GetStructJ(ctx, b, 5);

			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'{', ...
			if(!bgbcp_strcmp1(b2, "{"))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_defs, "defs", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_defs, "defs", n1));
			ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
		}

		s1=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
		n=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_DeclAttributeC(BGBCP_ParseState *ctx, char **str)
{
	char b[4096], b2[4096];
	char *s, *s1, *s2, *bty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

	s1=BGBCP_Token2(s, b, &ty, ctx->lang);

	if((ty==BTK_BRACE) && (b[0]=='[') && (*s1=='['))
	{
		s2=BGBCP_Token2(s1, b2, &ty2, ctx->lang);

		/* "[[attribute]]" syntax */

		if(!bgbcp_strcmp(b, "[") && !bgbcp_strcmp(b2, "["))
		{
			nl=NULL;
			s=s2;

			while(1)
			{
				BGBCP_Token2(s, b, &ty, ctx->lang);
				if(!bgbcp_strcmp1(b, "]"))
				{
					s=BGBCP_Token2(s, b, &ty, ctx->lang);	//']'
					s=BGBCP_Token2(s, b, &ty, ctx->lang);	//']'
					break;
				}
				if(!bgbcp_strcmp1(b, ","))
				{
					s=BGBCP_Token2(s, b, &ty, ctx->lang);
					continue;
				}
				if(ty!=BTK_NAME)break;

				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
				BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
				if(!bgbcp_strcmp1(b2, "("))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
					n2=BGBCP_FunArgs(ctx, &s);
					n1=BCCX_NewCst1(&bgbcc_rcst_attr, "attr",
						BCCX_NewCst1(&bgbcc_rcst_args, "args", n2));
					BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
				}else
				{
					n1=BCCX_NewCst(&bgbcc_rcst_attr, "attr");
					BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
				}

				nl=BCCX_AddEnd(nl, n1);
			}

//			n=BCCX_NewCst1(&bgbcc_rcst_attribute, "attribute", nl);
			n=BCCX_NewCst1(&bgbcc_rcst_declspec, "declspec", nl);
			*str=s;
			return(n);
		}
		
		return(NULL);
	}
	
	if(ty!=BTK_NAME)return(NULL);

	if(!bgbcp_strcmp(b, "__declspec"))
	{
		nl=NULL;
	
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//__declspec
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'('
		while(1)
		{
			BGBCP_Token2(s, b, &ty, ctx->lang);
			if(!bgbcp_strcmp1(b, ")"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
				break;
			}
			if(!bgbcp_strcmp1(b, ","))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
				continue;
			}
			if(ty!=BTK_NAME)break;

			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
			if(!bgbcp_strcmp1(b2, "("))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
				n2=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_NewCst1(&bgbcc_rcst_attr, "attr",
					BCCX_NewCst1(&bgbcc_rcst_args, "args", n2));
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
			}else
			{
				n1=BCCX_NewCst(&bgbcc_rcst_attr, "attr");
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
			}

			nl=BCCX_AddEnd(nl, n1);
		}
			
		n=BCCX_NewCst1(&bgbcc_rcst_declspec, "declspec", nl);
		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp(b, "__attribute__"))
	{
		nl=NULL;
	
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//__attribute__
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'('
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'('
		while(1)
		{
			BGBCP_Token2(s, b, &ty, ctx->lang);
			if(!bgbcp_strcmp1(b, ")"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//')'
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//')'
				break;
			}
			if(!bgbcp_strcmp1(b, ","))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
				continue;
			}
			if(ty!=BTK_NAME)break;

			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
			if(!bgbcp_strcmp1(b2, "("))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
				n2=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_NewCst1(&bgbcc_rcst_attr, "attr",
					BCCX_NewCst1(&bgbcc_rcst_args, "args", n2));
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
			}else
			{
				n1=BCCX_NewCst(&bgbcc_rcst_attr, "attr");
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
			}

			nl=BCCX_AddEnd(nl, n1);
		}

		n=BCCX_NewCst1(&bgbcc_rcst_attribute, "attribute", nl);
		*str=s;
		return(n);
	}

	if(	!bgbcp_strcmp(b, "__pragma") ||
		!bgbcp_strcmp(b, "_Pragma"))
	{
		nl=NULL;
	
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//__declspec
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'('
		while(1)
		{
			BGBCP_Token2(s, b, &ty, ctx->lang);
			if(!bgbcp_strcmp1(b, ")"))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
				break;
			}
			if(!bgbcp_strcmp1(b, ","))
			{
				s=BGBCP_Token2(s, b, &ty, ctx->lang);
				continue;
			}
			if(ty!=BTK_NAME)break;

			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
			if(!bgbcp_strcmp1(b2, "("))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
				n2=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_NewCst1(&bgbcc_rcst_attr, "attr",
					BCCX_NewCst1(&bgbcc_rcst_args, "args", n2));
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
			}else
			{
				n1=BCCX_NewCst(&bgbcc_rcst_attr, "attr");
				BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
			}

			nl=BCCX_AddEnd(nl, n1);
		}
			
		n=BCCX_NewCst1(&bgbcc_rcst_pragma, "pragma", nl);
		*str=s;
		return(n);
	}
	
	return(NULL);
}

BCCX_Node *BGBCP_DeclAttributeJ(BGBCP_ParseState *ctx, char **str)
{
	char b[4096], b2[4096];
	char *s, *s1, *s2, *bty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

	s1=BGBCP_Token2(s, b, &ty, ctx->lang);

	if(ctx->lang==BGBCC_LANG_CS)
	{
		/* "[attribute]" syntax */

		if(!bgbcp_strcmp(b, "["))
		{
			nl=NULL;
			s=s1;

			while(1)
			{
				BGBCP_Token2(s, b, &ty, ctx->lang);
				if(!bgbcp_strcmp1(b, "]"))
				{
					s=BGBCP_Token2(s, b, &ty, ctx->lang);	//']'
					break;
				}
				if(!bgbcp_strcmp1(b, ","))
				{
					s=BGBCP_Token2(s, b, &ty, ctx->lang);
					continue;
				}
				if(ty!=BTK_NAME)break;

				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
				BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
				if(!bgbcp_strcmp1(b2, "("))
				{
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'('
					n2=BGBCP_FunArgs(ctx, &s);
					n1=BCCX_NewCst1(&bgbcc_rcst_attr, "attr",
						BCCX_NewCst1(&bgbcc_rcst_args, "args", n2));
					BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
				}else
				{
					n1=BCCX_NewCst(&bgbcc_rcst_attr, "attr");
					BCCX_SetCst(n1, &bgbcc_rcst_name, "name", b);
				}

				nl=BCCX_AddEnd(nl, n1);
			}

//			n=BCCX_NewCst1(&bgbcc_rcst_attribute, "attribute", nl);
			n=BCCX_NewCst1(&bgbcc_rcst_declspec, "declspec", nl);
			*str=s;
			return(n);
		}
	}
	
	return(NULL);
}


BCCX_Node *BGBCP_DefTypeC(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256], b3[256];
	char *s, *s1, *s2, *s3, *bty;
	s64 fl, fl2, li;
	int i, j, k, ty, ty2, ty3;
	BCCX_Node *n, *n1, *n2, *attrl, *attrle;

	s=*str;

//	BGBCP_Token2(s, b, &ty, ctx->lang);
//	BGBCP_Warn(s, "BGBCP_DefTypeC: token=%s\n", b);

	bty=NULL;

	fl=0; j=0; attrl=NULL; attrle=NULL;
	while(1)
	{
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);
		s2=BGBCP_Token2(s1, b2, &ty2, ctx->lang);
		if(ty!=BTK_NAME)break;

		if(!strcmp(b, "vec3_t"))
			{ ty3=-1; }

		if(ctx->lang==BGBCC_LANG_CPP)
		{
//			s2=BGBCP_Token2(s1, b2, &ty2, ctx->lang);

			if(!bgbcp_strcmp6(b, "extern"))
			{
				if(!bgbcp_strcmp1(b2, "C"))
				{
					fl|=BGBCC_TYFL_NATIVE;
					s=s2;
					j++;
					continue;
				}
			}

			while(!bgbcp_strcmp2(b2, "::"))
			{
				s1=BGBCP_Token2(s1, b2, &ty2, ctx->lang);
				s1=BGBCP_Token2(s1, b3, &ty3, ctx->lang);
				strcat(b, "/");
				strcat(b, b3);

				BGBCP_Token2(s1, b2, &ty2, ctx->lang);
			}

		}

		li=BGBCP_DefTypeFlag(ctx, b);
		if(li)
		{
			fl|=li;
//			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			s=s1;
			j++;
			continue;
		}

//		if(bgbcp_chktoklst(b, bgbcp_basetypes_c))
		if(bgbcp_chktoklst_hix(b, bgbcp_basetypes_c, bgbcp_basetypes_c_hix))
		{
//			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			s=s1;

			if(!bgbcp_strcmp3(b, "int") && bty)
			{
				if(!bty)bty=bgbcc_strdup(b);
				j++;
				continue;	//short int, ...
			}else if(!bgbcp_strcmp4(b, "long"))
			{
				if(bty && !bgbcp_strcmp4(bty, "long"))
				{
//					bty=bgbcc_strdup("llong");
					bty=bgbcc_strdup("long_long");
				}else
				{
					bty=bgbcc_strdup("long");
				}
			}else if(!bgbcp_strcmp5(b, "float"))
			{
				if(bty && !bgbcp_strcmp5(bty, "short"))
				{
					bty=bgbcc_strdup("float16");
				}else
				{
					bty=bgbcc_strdup("float");
				}
			}else if(!bgbcp_strcmp5(b, "double"))
			{
				if(bty && !bgbcp_strcmp4(bty, "long"))
				{
//					bty=bgbcc_strdup("float128");
					bty=bgbcc_strdup("long_double");
				}else
				{
					bty=bgbcc_strdup("double");
				}
			}else
			{
				s3=BGBCP_Token2(s1, b3, &ty3, ctx->lang);

				bty=bgbcc_strdup(b);
				if(bty[0]=='_')
				{
					if(bty[1]=='_')
					{
//						if(!strncmp(bty, "__type_", 7))bty+=7;
//						if(!strncmp(bty, "__", 2))bty+=2;

						if(!strncmp(bty, "__type_", 7))
							bty+=7;
						else
							bty+=2;
					}if(!strcmp(bty, "_BitInt"))
					{
						if(!strcmp(b2, "(") && (ty3==BTK_NUMBER))
						{
							s=BGBCP_Token2(s3, b2, &ty2, ctx->lang);

							k=atoi(b3);
							if(k<=8)
								{ bty=bgbcc_strdup("char"); }
							else if(k<=16)
								{ bty=bgbcc_strdup("short"); }
							else if(k<=32)
								{ bty=bgbcc_strdup("int"); }
							else if(k<=64)
//								{ bty=bgbcc_strdup("llong"); }
								{ bty=bgbcc_strdup("long_long"); }
							else if(k<=128)
								{ bty=bgbcc_strdup("int128"); }
							else if(k<=256)
								{ bty=bgbcc_strdup("bitint_256"); }
							else if(k<=384)
								{ bty=bgbcc_strdup("bitint_384"); }
							else if(k<=512)
								{ bty=bgbcc_strdup("bitint_512"); }
							else
							{
								k=((k+127)/128)*128;
								sprintf(b2, "bitint_%d", k);
								bty=bgbcc_strdup(b2);
							}
						}
					}

#if 0
					else
						if(bty[1]=='I')
					{
						if(!strcmp(bty, "_Int8"))
							bty=bgbcc_strdup("char");
						if(!strcmp(bty, "_Int16"))
							bty=bgbcc_strdup("short");
						if(!strcmp(bty, "_Int32"))
							bty=bgbcc_strdup("int");
						if(!strcmp(bty, "_Int64"))
//							bty=bgbcc_strdup("llong");
							bty=bgbcc_strdup("long_long");
						if(!strcmp(bty, "_Int128"))
							bty=bgbcc_strdup("int128");
					}
					else
						if(bty[1]=='F')
					{
						if(!strcmp(bty, "_Float16"))
							bty=bgbcc_strdup("float16");
						if(!strcmp(bty, "_Float32"))
							bty=bgbcc_strdup("float");
						if(!strcmp(bty, "_Float64"))
							bty=bgbcc_strdup("double");
						if(!strcmp(bty, "_Float128"))
							bty=bgbcc_strdup("float128");
					}
#endif
				}
			}

			j++;
			continue;
		}

		n1=BGBCP_DeclAttributeC(ctx, &s);
		if(n1)
		{
			attrl=BCCX_AddEnd2(attrl, &attrle, n1);
			//j++;
			continue;
		}

		break;
	}
	
	if(!bty)
	{
		if(fl&BGBCC_TYFL_AUTO)
		{
			fl&=~BGBCC_TYFL_AUTO;
			bty="auto";
		}
	}

//	if(ty!=BTK_NAME)
	if((ty==BTK_STRING) || (ty==BTK_CHARSTRING))
		return(NULL);

	if(fl && !bgbcp_strcmp1(b, ":"))
	{
		ctx->dfl_flags=fl;
		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		*str=s;
		return(NULL);
	}

	if(!(fl&BGBCC_TYFL_DFL_MASK))
		fl|=(ctx->dfl_flags&BGBCC_TYFL_DFL_MASK);

	fl|=(ctx->dfl_flags&BGBCC_TYFL_DFL_COPY_MASK);

	if(!bgbcp_strcmp(b, "struct") || !bgbcp_strcmp(b, "union") ||
		!bgbcp_strcmp4(b, "enum") || !bgbcp_strcmp(b, "__class") ||
		!bgbcp_strcmp(b, "__interface") ||
		(!bgbcp_strcmp(b, "class") && (ctx->lang==BGBCC_LANG_CPP)))
	{
		n=BGBCP_DefClassC(ctx, &s);
		if(attrl)BCCX_Add(n, attrl);

		fl2=BCCX_GetIntCst(n, &bgbcc_rcst_flags, "flags");
		fl2&=~BGBCC_TYFL_DFL_MASK;

		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl2|fl);

		*str=s;
		return(n);	
	}

#if 0
	if(!bgbcp_strcmp(b, "struct"))return(BGBCP_DefClassC(ctx, str));
	if(!bgbcp_strcmp(b, "union"))return(BGBCP_DefClassC(ctx, str));
	if(!bgbcp_strcmp(b, "enum"))return(BGBCP_DefClassC(ctx, str));

	if(!bgbcp_strcmp(b, "__class"))return(BGBCP_DefClassC(ctx, str));
	if(!bgbcp_strcmp(b, "__interface"))return(BGBCP_DefClassC(ctx, str));

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		if(!bgbcp_strcmp(b, "class"))return(BGBCP_DefClassC(ctx, str));
//		if(!bgbcp_strcmp(b, "interface"))return(BGBCP_DefClassC(ctx, str));
	}
#endif

#if 0
	if(!bgbcp_strcmp(b, "struct") || !bgbcp_strcmp(b, "union"))
	{
		j=!bgbcp_strcmp(b, "struct");

		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//struct
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
		BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			if(j)n=BGBCP_GetStruct(ctx, b);
				else n=BGBCP_GetUnion(ctx, b);

			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'{', ...
			if(!bgbcp_strcmp1(b2, "{"))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang); //'{'
				n1=BGBCP_Block(ctx, &s);
				BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_body, "body", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_Block(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_New(j?"struct":"union");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_body, "body", n1));

//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
			ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
		}


		n1=n;
		s1=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
		n=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

//		printf("ESTR %s\n", s);
//		*(int *)-1=-1;

		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp4(b, "enum"))
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//enum
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name|'{'
		BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name|'{'|...

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			n=BGBCP_GetEnum(ctx, b);

			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'{', ...
			if(!bgbcp_strcmp1(b2, "{"))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_defs, "defs", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_defs, "defs", n1));
//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
			ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
		}

		s1=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
		n=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

		*str=s;
		return(n);
	}
#endif

	n=NULL;
//	if(!bty)
	if(!bty && (ty==BTK_NAME))
		n=BGBCP_LookupType(ctx, b);

//	n=BGBCP_LookupType(ctx, b);
	if(n)
	{
//		if(BCCX_TagIsCstP(n, &bgbcc_rcst_proto, "proto"))
//		{
//		}

		s=BGBCP_Token2(s, b, &ty, ctx->lang);

		while(1)
		{
			BGBCP_Token2(s, b2, &ty2, ctx->lang);
			li=BGBCP_DefTypeFlag(ctx, b2);
			if(li)
			{
				fl|=li;
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
				j++;
				continue;
			}
			break;
		}

		n1=BCCX_FindTagCst(n, &bgbcc_rcst_type, "type");

		n2=BCCX_FindTagCst(n1, &bgbcc_rcst_size, "size");
		if(n2)
		{
			s1=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
//			s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");

			/* Type uses typedef name. */
			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

			if(attrl)BCCX_Add(n, attrl);

			*str=s;
			return(n);
		}

		n=BCCX_Clone(n1);
		fl2=BCCX_GetIntCst(n, &bgbcc_rcst_flags, "flags");
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl2|fl);

		if(attrl)BCCX_Add(n, attrl);

		*str=s;
		return(n);
	}
	
	if(ctx->lang==BGBCC_LANG_CPP)
	{
		n=BGBCP_LookupTemplateType(ctx, b);
		if(n)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			s1=BGBCP_Token2(s, b2, &ty2, ctx->lang);
			
			n2=NULL;
			if(!strcmp(b2, "<"))
			{
				s=s1;
				n2=BGBCP_GenArgs(ctx, &s);
			}
			
			BGBCP_GenMangledTemplateName(ctx, b3, b, n2);

			n1=BGBCP_LookupStruct(ctx, b3);
			if(n1)
			{
				/* Already exists. */
			
				s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
				fl2=BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags");

				n=BCCX_NewCst(&bgbcc_rcst_type, "type");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
				BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl2|fl);
				BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

				if(attrl)BCCX_Add(n, attrl);

				*str=s;
				return(n);
			}

			BGBCP_InstanceTemplate(ctx, b3, n, n2);

			n1=BGBCP_LookupStruct(ctx, b3);
			if(n1)
			{
				s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
				fl2=BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags");

				n=BCCX_NewCst(&bgbcc_rcst_type, "type");
				BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
				BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl2|fl);
				BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

				if(attrl)BCCX_Add(n, attrl);

				*str=s;
				return(n);
			}
			
			BGBCC_DBGBREAK
		}
	}

	if(	(ctx->lang==BGBCC_LANG_CPP) ||
		(ctx->lang==BGBCC_LANG_BS2) ||
		(ctx->expect_type&1))
	{
		n=BGBCP_LookupStruct(ctx, b);
		if(n)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);

			n1=n;
			s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
			fl2=BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags");

			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl2|fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

			if(attrl)BCCX_Add(n, attrl);

			*str=s;
			return(n);
		}
	}

	if(ctx->lang==BGBCC_LANG_CX)
	{
		if(BGBCP_CheckTypeName(ctx, b))
		{
			while(1)
			{
				BGBCP_Token2(s, b2, &ty2, ctx->lang);
				li=BGBCP_DefTypeFlag(ctx, b2);
				if(li)
				{
					fl|=li;
					s=BGBCP_Token2(s, b2, &ty2, ctx->lang);
					j++;
					continue;
				}
				break;
			}

			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

			if(attrl)BCCX_Add(n, attrl);

			*str=s;
			return(n);
		}
	}

	if(j)
	{
		if(!bty)
			bty="int";

		if(!strcmp(bty, "vec3_t"))
			{ __debugbreak(); }

		n=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", bty);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

		if(attrl)BCCX_Add(n, attrl);

#if 0
		BGBCP_Token2(s, b, &ty, ctx->lang);
		if(!bgbcp_strcmp1(b, ":"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//':'
			n1=BGBCP_Expression(ctx, &s);
			n1=BCCX_NewCst1(&bgbcc_rcst_bits, "bits", n1);
			BCCX_Add(n, n1);
		}
#endif

		*str=s;
		return(n);
	}

	return(NULL);
}


BCCX_Node *BGBCP_DefClassJ(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *cname, *ocname;
	char *s, *s1;
	s64 dfl_fl, fl, li;
	int ocnsi;
	int i, j, ty, ty2, cty;
	BCCX_Node *n, *n1, *n2;

	s=*str;

//	bty=NULL;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		li=BGBCP_DefTypeFlag(ctx, b);
		if(li)
		{
			fl|=li;
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			j++;
			continue;
		}

		break;
	}

	if(!bgbcp_strcmp(b, "struct") || !bgbcp_strcmp(b, "union") ||
		!bgbcp_strcmp(b, "class") || !bgbcp_strcmp(b, "interface"))
	{
//		j=!bgbcp_strcmp(b, "struct");

		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//struct

		dfl_fl=ctx->dfl_flags;
		
		ctx->dfl_flags=BGBCC_TYFL_PUBLIC;

		cname=NULL;

		cty=0;
		if(!bgbcp_strcmp(b, "struct"))cty=1;
		if(!bgbcp_strcmp(b, "union"))cty=2;
		if(!bgbcp_strcmp(b, "class"))
		{
			ctx->dfl_flags=BGBCC_TYFL_PROTECTED;
			cty=3;
		}
		if(!bgbcp_strcmp(b, "interface"))cty=4;

		s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
		BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...

		n=NULL;
		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			n=BGBCP_GetStructJ(ctx, b, cty);
			
			cname=bgbcc_strdup(b);

			s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
			BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...
		}

		if(n && !bgbcp_strcmp1(b, ":"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//':'

//			n1=BGBCP_DefType(ctx, &s);
			n1=BGBCP_DefExpectType(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_super, "super", n1));

			BGBCP_Token2(s, b, &ty, ctx->lang);
			n1=NULL;
			while(!bgbcp_strcmp1(b, ","))
			{
				if(!n1)n1=BCCX_NewCst(&bgbcc_rcst_impl, "impl");
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//','
//				n2=BGBCP_DefType(ctx, &s);
				n2=BGBCP_DefExpectType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token2(s, b, &ty, ctx->lang);
			}
			if(n1)
				BCCX_Add(n, n1);
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(n && (
			!bgbcp_strcmp(b, "extends") ||
			!bgbcp_strcmp(b, "__extends")))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'extends'

//			n1=BGBCP_DefType(ctx, &s);
			n1=BGBCP_DefExpectType(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_super, "super", n1));
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(n && (
			!bgbcp_strcmp(b, "implements") ||
			!bgbcp_strcmp(b, "__implements")))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'implements'

			n1=BCCX_NewCst(&bgbcc_rcst_impl, "impl");

			while(1)
			{
//				n2=BGBCP_DefType(ctx, &s);
				n2=BGBCP_DefExpectType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token2(s, b, &ty, ctx->lang);
				if(bgbcp_strcmp1(b, ","))
					break;
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//','
			}
			BCCX_Add(n, n1);
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(!n)
		{
			s1=BGBCC_GenSym2();
			n=BGBCP_GetStructJ(ctx, s1, cty);
		}

		if(!bgbcp_strcmp1(b, "{"))
		{
			ocname=ctx->cur_class;
			ocnsi=ctx->n_cur_nsi;
			ctx->cur_class=cname;

			ctx->cur_nsi[ctx->n_cur_nsi++]=cname;
		
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_Block(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_body, "body", n1));

			ctx->cur_class=ocname;
			ctx->n_cur_nsi=ocnsi;

//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}

		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);

		n1=n;
		s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
		n=BCCX_NewCst(&bgbcc_rcst_classdef, "classdef");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

		ctx->dfl_flags=dfl_fl;

		*str=s;
		return(n);
	}

	if(!bgbcp_strcmp4(b, "enum"))
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);	//enum
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name|'{'
		BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name|'{'|...

		n=NULL;
		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
//			n=BGBCP_GetEnum(ctx, b);
			n=BGBCP_GetStructJ(ctx, b, 5);

			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'{', ...
			if(!bgbcp_strcmp1(b2, "{"))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_defs, "defs", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_defs, "defs", n1));
//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
			ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
		}

		s1=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
		n=BCCX_NewCst(&bgbcc_rcst_enumdef, "enumdef");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_DefTypeJ(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256], b3[256];
	char *s, *s1, *s2, *bty, *vty;
	s64 fl, li;
	int i, j, ty, ty2, ty3, ind;
	BCCX_Node *n, *n1, *n2, *attrl, *attrle;

	s=*str;

	fl=0; j=0; attrl=NULL; attrle=NULL;
	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		li=BGBCP_DefTypeFlag(ctx, b);
		if(li)
		{
			fl|=li;
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			j++;
			continue;
		}

		n1=BGBCP_DeclAttributeJ(ctx, &s);
		if(n1)
		{
			attrl=BCCX_AddEnd2(attrl, &attrle, n1);
			continue;
		}
		
		break;
	}

	bty=NULL;
	vty=NULL;

//	if(ty!=BTK_NAME)
	if((ty==BTK_STRING) || (ty==BTK_CHARSTRING))
		return(NULL);

	if(fl && !bgbcp_strcmp(b, ":"))
	{
		ctx->dfl_flags=fl;
		s=BGBCP_Token2(s, b, &ty, ctx->lang);

		n=BCCX_NewCst(&bgbcc_rcst_null_statement, "null_statement");

		*str=s;
		return(n);
	}

//	if(!bgbcp_strcmp(b, "class"))return(NULL);
//	if(!bgbcp_strcmp(b, "struct"))return(NULL);
//	if(!bgbcp_strcmp(b, "interface"))return(NULL);
//	if(!bgbcp_strcmp(b, "enum"))return(NULL);

//	if(!bgbcp_strcmp(b, "class"))return(BGBCP_DefClassJ(ctx, str));
//	if(!bgbcp_strcmp(b, "struct"))return(BGBCP_DefClassJ(ctx, str));
//	if(!bgbcp_strcmp(b, "union"))return(BGBCP_DefClassJ(ctx, str));
//	if(!bgbcp_strcmp(b, "interface"))return(BGBCP_DefClassJ(ctx, str));
//	if(!bgbcp_strcmp4(b, "enum"))return(BGBCP_DefClassJ(ctx, str));

	if(!bgbcp_strcmp(b, "class") ||
		!bgbcp_strcmp(b, "struct") ||
		!bgbcp_strcmp(b, "union") ||
		!bgbcp_strcmp(b, "interface") ||
		!bgbcp_strcmp4(b, "enum"))
	{
		n=BGBCP_DefClassJ(ctx, str);
		if(attrl)BCCX_Add(n, attrl);
		return(n);
	}

	ind=0;

	if(ctx->lang==BGBCC_LANG_BS2)
	{
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);
		while(!bgbcp_strcmp(b, "*"))
		{
			ind++;
			s=s1;
			s1=BGBCP_Token2(s, b, &ty, ctx->lang);
		}
	}

	if(	(ctx->lang==BGBCC_LANG_BS)	||
		(ctx->lang==BGBCC_LANG_BS2)	)
	{
		if(	!bgbcc_strcmp(b, "var")			||
			!bgbcc_strcmp(b, "function")	)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			bty=bgbcc_strdup("var");
		}else
			if(bgbcp_chktoklst(b, bgbcp_basetypes_bs2))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);

			bty=bgbcc_strdup(b);

			if(!strncmp(bty, "__type_", 7))bty+=7;
			if(!strncmp(bty, "__", 2))bty+=2;
		}else if(bgbcp_chktoklst(b, bgbcp_nontypes_bs2))
		{
			return(NULL);
		}
	}else
		if(ctx->lang==BGBCC_LANG_CS)
	{
		if(bgbcp_chktoklst(b, bgbcp_basetypes_cs))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);

			bty=bgbcc_strdup(b);

			if(!strncmp(bty, "__type_", 7))bty+=7;
			if(!strncmp(bty, "__", 2))bty+=2;
		}else if(bgbcp_chktoklst(b, bgbcp_nontypes_cs))
		{
			return(NULL);
		}
	}else
	{
		if(bgbcp_chktoklst(b, bgbcp_basetypes_j))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);

			bty=bgbcc_strdup(b);

			if(!strncmp(bty, "__type_", 7))bty+=7;
			if(!strncmp(bty, "__", 2))bty+=2;
		}
	}

	s1=BGBCP_Token2(s, b, &ty, ctx->lang);
	s2=BGBCP_Token2(s1, b2, &ty2, ctx->lang);

	if(!bty && (ty==BTK_NAME))
	{
		if(!ctx->in_func_body &&
			ctx->cur_class &&
			!bgbcp_strcmp(b, ctx->cur_class) &&
			!bgbcp_strcmp(b2, "("))
		{
			bty="void_ctor";
		}
	}

	if(!bty && (ty==BTK_NAME))
	{
		s=s1;
		while(!bgbcp_strcmp1(b2, ".") ||
			!bgbcp_strcmp2(b2, "::"))
		{
			s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'.'
			strcat(b, "/");
			s=BGBCP_Token2(s, b2, &ty2, ctx->lang);	//name

			if(ty2!=BTK_NAME)return(NULL);

			strcat(b, b2);
			BGBCP_Token2(s, b2, &ty2, ctx->lang);
		}

		vty=bgbcc_strdup(b);
	}

	if(fl && !bty)
		bty=vty;

	if(!bty && vty)
	{
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);
		s2=BGBCP_Token2(s1, b2, &ty2, ctx->lang);
		BGBCP_Token2(s2, b3, &ty3, ctx->lang);

		if((ty==BTK_NAME) && bgbcp_chktoklst(b, bgbcp_nontypes_bs2))
			ty=-1;
		if((ty2==BTK_NAME) && bgbcp_chktoklst(b2, bgbcp_nontypes_bs2))
			ty=-1;
		if((ty3==BTK_NAME) && bgbcp_chktoklst(b3, bgbcp_nontypes_bs2))
			ty=-1;

		if(ty==BTK_NAME)
			bty=vty;
		if(!bgbcp_strcmp(b, "*") && (ty2==BTK_NAME))
			bty=vty;
		if(!bgbcp_strcmp(b, "[") &&
			!bgbcp_strcmp(b2, "]") &&
			(ty3==BTK_NAME))
				bty=vty;
	}

	//bty: known valid type name
	//vty: expected valid

	if(!(fl&BGBCC_TYFL_DFL_MASK))
		fl|=(ctx->dfl_flags&BGBCC_TYFL_DFL_MASK);

	fl|=(ctx->dfl_flags&BGBCC_TYFL_DFL_COPY_MASK);

	if(bty)
	{
		n=BGBCP_LookupType(ctx, bty);
		if(n)
		{
			n=BCCX_FindTagCst(n, &bgbcc_rcst_type, "type");
			n=BCCX_Clone(n);
			i=BCCX_GetIntCst(n, &bgbcc_rcst_flags, "flags");
			j=BCCX_GetIntCst(n, &bgbcc_rcst_ind, "ind");
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", i|fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", j+ind);
			if(attrl)BCCX_Add(n, attrl);
		}else
		{
			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", bty);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", ind);
			if(attrl)BCCX_Add(n, attrl);
		}
	}else if(vty)
	{
		n=BGBCP_LookupType(ctx, vty);
		if(n)
		{
			n=BCCX_FindTagCst(n, &bgbcc_rcst_type, "type");

			n=BCCX_Clone(n);
			i=BCCX_GetIntCst(n, &bgbcc_rcst_flags, "flags");
			j=BCCX_GetIntCst(n, &bgbcc_rcst_ind, "ind");
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", i|fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", j+ind);
			if(attrl)BCCX_Add(n, attrl);
		}else if(BGBCP_CheckTypeName(ctx, vty))
		{
			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", vty);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", ind);
			if(attrl)BCCX_Add(n, attrl);
		}else
		{
			return(NULL);
		}
	}else
	{
		return(NULL);
	}

	BGBCP_Token2(s, b, &ty, ctx->lang);
	while(!bgbcp_strcmp1(b, "["))
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		BGBCP_Token2(s, b, &ty, ctx->lang);
		if(!bgbcp_strcmp1(b, "]"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			BCCX_Add(n, BCCX_NewCst(&bgbcc_rcst_arrayq, "arrayq"));
			BGBCP_Token2(s, b, &ty, ctx->lang);
			continue;
		}

		n1=BGBCP_FunArgs(ctx, &s);
		BCCX_AddV(n, BCCX_NewCst1(&bgbcc_rcst_arrayq, "arrayq", n1));

		BGBCP_Token2(s, b, &ty, ctx->lang);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_DefType(BGBCP_ParseState *ctx, char **str)
{
	if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_CX))
		return(BGBCP_DefTypeC(ctx, str));

	if((ctx->lang==BGBCC_LANG_JAVA) || (ctx->lang==BGBCC_LANG_CS))
		return(BGBCP_DefTypeJ(ctx, str));

	if(ctx->lang==BGBCC_LANG_BS2)
		return(BGBCP_DefTypeJ(ctx, str));
	if(ctx->lang==BGBCC_LANG_BS)
		return(BGBCP_DefTypeJ(ctx, str));

	return(NULL);
}

BCCX_Node *BGBCP_DefExpectType(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	int oet;
	
	oet=ctx->expect_type;
	ctx->expect_type=1;
	n=BGBCP_DefType(ctx, str);
	ctx->expect_type=oet;
	return(n);
}

s64 BGBCP_CheckNameEnum(BGBCP_ParseState *ctx, char *name)
{
	int i, j, k, h;
	
	if(!ctx)
		return(-1);
	
	if(!name)
		return(-1);
	
	h=BGBCC_CCXL_HashName(name)&255;
	i=ctx->enum_hash[h];
	while(i>=0)
	{
		if(!strcmp(ctx->enum_names[i], name))
			return(ctx->enum_vals[i]);
		i=ctx->enum_chain[i];
	}
	
//	for(i=0; i<ctx->n_enum_vars; i++)
//		if(!strcmp(ctx->enum_names[i], name))
//			return(ctx->enum_vals[i]);
	
	return(-1);
}

BCCX_Node *BGBCP_EnumVarsList(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *lst, *lste;
	int i, j, k, h;

	s=*str;
	lst=NULL;
	lste=NULL;
	i=0;

	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		if(!bgbcp_strcmp1(b, ";") || !bgbcp_strcmp1(b, "}"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			break;
		}

		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		BGBCP_Token2(s, b2, &ty2, ctx->lang);
		if(!bgbcp_strcmp1(b2, "="))
		{
			s=BGBCP_Token(s, b2, &ty);	//'='
//			s=BGBCP_Token(s, b2, &ty);	//<val>
//			i=atoi(b2);
			n1=BGBCP_Expression(ctx, &s);
			n1=BGBCP_ReduceExpr(ctx, n1);
			i=BCCX_GetIntCst(n1, &bgbcc_rcst_value, "value");
		}

		h=BGBCC_CCXL_HashName(b)&255;
		for(k=0; k<ctx->n_enum_vars; k++)
			if(!strcmp(ctx->enum_names[k], b))
				break;
		if(k<ctx->n_enum_vars)
		{
			ctx->enum_vals[k]=i;
		}else
		{
			if(ctx->n_enum_vars<4096)
			{
				k=ctx->n_enum_vars++;
				ctx->enum_names[k]=bgbcc_strdup(b);
				ctx->enum_vals[k]=i;
				ctx->enum_chain[k]=ctx->enum_hash[h];
				ctx->enum_hash[h]=k;
			}else
			{
				k=-1;
			}
		}

		n=BCCX_NewCst(&bgbcc_rcst_def, "def");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
		BCCX_SetIntCst(n, &bgbcc_rcst_value, "value", i++);
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		if(!bgbcp_strcmp1(b, ";"))break;
		if(!bgbcp_strcmp1(b, "}"))break;

		if(bgbcp_strcmp1(b, ","))
		{
			BGBCP_Error(s, "PDSCR_CParse_EnumVarsList: "
				"Invalid token %s in vars list\n", b);
			return(NULL);
		}
	}

	*str=s;
	return(lst);
}

