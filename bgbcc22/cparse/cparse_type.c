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
	int hi;
	char *s;

	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi&=255;

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
	int hi;
	char *s, *sn;

	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
//	hi=(hi>>8)&1023;
	hi=hi&1023;

	if(!tk_name)tk_name=bgbcc_strdup("name");

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
	c=ctx->types;
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

	return(NULL);
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
		for(i=0; ctx->cur_nsi[i]; i++)
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

	c=ctx->types;
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
	BCCX_Node *c;
	char *s;
	int i;

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		c=BGBCP_LookupTypeI(ctx, tb);
		if(c)return(c);
	}

	if(ctx->cur_nsi)
	{
		for(i=0; ctx->cur_nsi[i]; i++)
		{
			sprintf(tb, "%s/%s", ctx->cur_nsi[i], name);
			c=BGBCP_LookupTypeI(ctx, tb);
			if(c)return(c);
		}
	}

	c=BGBCP_LookupTypeI(ctx, name);
	if(c)return(c);

	return(NULL);
}


int BGBCP_HandleTypedef(BGBCP_ParseState *ctx, BCCX_Node *n)
{
	BCCX_Node *c, *t;
	char *s;
	int i;

	if(BCCX_TagIsP(n, "vars"))
	{
		c=BCCX_Child(n);
		while(c)
		{
			t=BCCX_Clone(c);
			
#if 1
			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			if(!s) { c=BCCX_Next(c); continue; }
			i=BGBCP_HashTypeNameI(ctx, s);

			t->hnext=ctx->type_hash[i];
			ctx->type_hash[i]=t;
#endif		

			ctx->types=BCCX_AddEnd2(ctx->types, &ctx->e_types, t);
			c=BCCX_Next(c);
		}
		return(0);
	}

	BCCX_Print(n);

	BGBCP_Error(NULL, "BGBCP_HandleTypedef: "
			"Bad typedef definition\n");
	return(-1);
}

BCCX_Node *BGBCP_GetStruct(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_NewCst(&bgbcc_rcst_struct, "struct");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", name);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

BCCX_Node *BGBCP_GetUnion(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_NewCst(&bgbcc_rcst_union, "union");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", name);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

BCCX_Node *BGBCP_GetEnum(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", name);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

BCCX_Node *BGBCP_GetStructJ(BGBCP_ParseState *ctx, char *name, int ty)
{
	char tb[256];
	BCCX_Node *n, *n1, *nt;
	char *s, *s1;
	int i;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	s=NULL;
	if(ty==1)s="struct";
	if(ty==2)s="union";
	if(ty==3)s="class";
	if(ty==4)s="interface";
	if(!s)return(NULL);

	s1=name;
	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		s1=bgbcc_strdup(tb);
	}

//	n=BCCX_New(s);
	n=NULL;
	if(ty==1)n=BCCX_NewCst(&bgbcc_rcst_struct, "struct");
	if(ty==2)n=BCCX_NewCst(&bgbcc_rcst_union, "union");
	if(ty==3)n=BCCX_NewCst(&bgbcc_rcst_class, "class");
	if(ty==4)n=BCCX_NewCst(&bgbcc_rcst_interface, "interface");

	BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		n1=n;
//		s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
		nt=BCCX_NewCst(&bgbcc_rcst_type, "type");
		BCCX_SetCst(nt, &bgbcc_rcst_name, "name", s1);
//		BCCX_SetIntCst(nt, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(nt, &bgbcc_rcst_flags, "flags", 0);
		BCCX_SetIntCst(nt, &bgbcc_rcst_ind, "ind", 0);

		n1=BCCX_NewCst1V(&bgbcc_rcst_var, "var", nt);
		BCCX_SetCst(n1, &bgbcc_rcst_name, "name", s1);

		ctx->types=BCCX_AddEnd2(ctx->types, &ctx->e_types, n1);

		i=BGBCP_HashTypeNameI(ctx, s1);
		n1->hnext=ctx->type_hash[i];
		ctx->type_hash[i]=n1;
	}

	return(n);
}

int BGBCP_CheckTypeName(BGBCP_ParseState *ctx, char *name)
{
	char *s;
	s=BGBCP_LookupTypeSig(ctx, name);
	if(s)return(1);
	return(0);

//	return(1);
}


static char *bgbcp_basetypes_c[]={
"char", "short", "int", "long", "float", "double", "void",
"__int64",
"__int128", "__float128", "__float16", "__m64", "__m128",
"__vec2", "__vec3", "__vec4", "__quat", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
"__mat2", "__mat3", "__mat4",

"__v2f", "__v3f", "__v4f",
"__m2f", "__m3f", "__m4f",
"__variant", "__variantf",
"__variant_va", "__variant_this", "__variant_cls",
"__gnuc_va_list", "__builtin_va_list",
"_Bool",
NULL
};

static char *bgbcp_basetypes_j[]={
"char", "byte", "short", "ushort", "int", "uint", "long", "ulong",
"float", "double", "void", "string", "variant",
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

int BGBCP_DefTypeFlag(BGBCP_ParseState *ctx, char *tag)
{
	int i, n, c0;

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
	case BOTK___GC:				i=BGBCC_TYFL_GC; break;
	case BOTK___NOGC:			i=BGBCC_TYFL_NOGC; break;

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

				if(!bgbcp_strcmp(tag, "__w64"))i=BGBCC_TYFL_INLINE;
				if(!bgbcp_strcmp(tag, "__ptr64"))i=BGBCC_TYFL_INLINE;
				if(!bgbcp_strcmp(tag, "__ptr32"))i=BGBCC_TYFL_INLINE;

				if(!bgbcp_strcmp(tag, "__win64"))i=BGBCC_TYFL_WIN64;
				if(!bgbcp_strcmp(tag, "__amd64"))i=BGBCC_TYFL_AMD64;
				if(!bgbcp_strcmp(tag, "__xcall"))i=BGBCC_TYFL_XCALL;

				if(!bgbcp_strcmp(tag, "__packed"))i=BGBCC_TYFL_PACKED;
				if(!bgbcp_strcmp(tag, "__gc"))i=BGBCC_TYFL_GC;
				if(!bgbcp_strcmp(tag, "__nogc"))i=BGBCC_TYFL_NOGC;
				if(!bgbcp_strcmp(tag, "__unaligned"))i=BGBCC_TYFL_PACKED;
				if(!bgbcp_strcmp(tag, "__restrict"))i=BGBCC_TYFL_RESTRICT;

				if(!bgbcp_strcmp(tag, "__wide"))i=BGBCC_TYFL_WIDE;

				if(!bgbcp_strcmp(tag, "__ltlendian"))i=BGBCC_TYFL_LTLENDIAN;
				if(!bgbcp_strcmp(tag, "__bigendian"))i=BGBCC_TYFL_BIGENDIAN;

				if(!bgbcp_strcmp(tag, "__transient"))i=BGBCC_TYFL_ABSTRACT;
				if(!bgbcp_strcmp(tag, "__thread"))i=BGBCC_TYFL_THREAD;
				if(!bgbcp_strcmp(tag, "__dynamic"))i=BGBCC_TYFL_DYNAMIC;

//				if(!bgbcp_strcmp(tag, "__getter"))i=BGBCC_TYFL_GETTER;
//				if(!bgbcp_strcmp(tag, "__setter"))i=BGBCC_TYFL_SETTER;
			}else
			{
				if(!bgbcp_strcmp(tag, "_Complex"))i=BGBCC_TYFL_UNSIGNED;
				if(!bgbcp_strcmp(tag, "_Imaginary"))i=BGBCC_TYFL_UNSIGNED;
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
		}

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

		if(!bgbcp_strcmp(tag, "transient"))i=BGBCC_TYFL_TRANSIENT;
		if(!bgbcp_strcmp(tag, "native"))i=BGBCC_TYFL_NATIVE;

		if(!bgbcp_strcmp(tag, "typedef"))i=BGBCC_TYFL_TYPEDEF;
	}

	return(i);
}

BCCX_Node *BGBCP_DefClassC(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1, *s2;
	s64 dfl_fl;
	int i, j, ty, ty2, fl, cty;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

//	bty=NULL;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			fl|=i;
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
			if(!(fl&BGBCC_TYFL_GC))
				fl|=BGBCC_TYFL_NOGC;

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
			n=BGBCP_GetStructJ(ctx, b, cty);

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
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));

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
			BCCX_NewCst1V(&bgbcc_rcst_attributes, "attributes", nl));

		ctx->dfl_flags=dfl_fl;

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
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_defs, "defs", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_defs, "defs", n1));
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
	char *s, *s1, *bty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

	BGBCP_Token2(s, b, &ty, ctx->lang);
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
					BCCX_NewCst1V(&bgbcc_rcst_args, "args", n2));
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
					BCCX_NewCst1V(&bgbcc_rcst_args, "args", n2));
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
	
	return(NULL);
}

BCCX_Node *BGBCP_DefTypeC(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256], b3[256];
	char *s, *s1, *s2, *bty;
	int i, j, ty, ty2, ty3, fl;
	BCCX_Node *n, *n1, *n2, *attrl, *attrle;

	s=*str;

//	BGBCP_Token2(s, b, &ty, ctx->lang);
//	BGBCP_Warn(s, "BGBCP_DefTypeC: token=%s\n", b);

	bty=NULL;

	fl=0; j=0; attrl=NULL; attrle=NULL;
	while(1)
	{
		s1=BGBCP_Token2(s, b, &ty, ctx->lang);
		if(ty!=BTK_NAME)break;

		if(ctx->lang==BGBCC_LANG_CPP)
		{
			s2=BGBCP_Token2(s1, b2, &ty2, ctx->lang);

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

		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			fl|=i;
//			s=BGBCP_Token2(s, b, &ty, ctx->lang);
			s=s1;
			j++;
			continue;
		}

		if(bgbcp_chktoklst(b, bgbcp_basetypes_c))
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
					bty=bgbcc_strdup("llong");
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
			}else
			{
				bty=bgbcc_strdup(b);
				if(!strncmp(bty, "__type_", 7))bty+=7;
				if(!strncmp(bty, "__", 2))bty+=2;
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
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_Block(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_New(j?"struct":"union");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));

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
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_defs, "defs", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_defs, "defs", n1));
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
	if(!bty)n=BGBCP_LookupType(ctx, b);

//	n=BGBCP_LookupType(ctx, b);
	if(n)
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);

		while(1)
		{
			BGBCP_Token2(s, b2, &ty2, ctx->lang);
			i=BGBCP_DefTypeFlag(ctx, b2);
			if(i)
			{
				fl|=i;
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

			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

			if(attrl)BCCX_Add(n, attrl);

			*str=s;
			return(n);
		}

		n=BCCX_Clone(n1);
		i=BCCX_GetIntCst(n, &bgbcc_rcst_flags, "flags");
		BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", i|fl);

		if(attrl)BCCX_Add(n, attrl);

		*str=s;
		return(n);
	}

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		n=BGBCP_LookupStruct(ctx, b);
		if(n)
		{
			n1=n;
			s1=BCCX_GetCst(n1, &bgbcc_rcst_name, "name");
			i=BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags");

			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", i|fl);
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
				i=BGBCP_DefTypeFlag(ctx, b2);
				if(i)
				{
					fl|=i;
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
		if(!bty)bty="int";

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
	char *s, *s1;
	s64 dfl_fl;
	int i, j, ty, ty2, fl, cty;
	BCCX_Node *n, *n1, *n2;

	s=*str;

//	bty=NULL;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			fl|=i;
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

			s1=BGBCP_Token2(s, b, &ty, ctx->lang);	//name, '{', ...
			BGBCP_Token2(s1, b2, &ty2, ctx->lang);	//name, '{', ...
		}

		if(n && !bgbcp_strcmp1(b, ":"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//':'

			n1=BGBCP_DefType(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_super, "super", n1));

			BGBCP_Token2(s, b, &ty, ctx->lang);
			n1=NULL;
			while(!bgbcp_strcmp1(b, ","))
			{
				if(!n1)n1=BCCX_NewCst(&bgbcc_rcst_impl, "impl");
				s=BGBCP_Token2(s, b, &ty, ctx->lang);	//','
				n2=BGBCP_DefType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token2(s, b, &ty, ctx->lang);
			}
			BCCX_Add(n, n1);
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(n && !bgbcp_strcmp(b, "extends"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'extends'

			n1=BGBCP_DefType(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_super, "super", n1));
			BGBCP_Token2(s, b, &ty, ctx->lang);
		}

		if(n && !bgbcp_strcmp(b, "implements"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//'implements'

			n1=BCCX_NewCst(&bgbcc_rcst_impl, "impl");

			while(1)
			{
				n2=BGBCP_DefType(ctx, &s);
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
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_Block(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));

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

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);	//name
			n=BGBCP_GetEnum(ctx, b);

			BGBCP_Token2(s, b2, &ty2, ctx->lang);	//'{', ...
			if(!bgbcp_strcmp1(b2, "{"))
			{
				s=BGBCP_Token2(s, b2, &ty2, ctx->lang); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_defs, "defs", n1));
			}
		}else if(!bgbcp_strcmp1(b, "{"))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_NewCst(&bgbcc_rcst_enum, "enum");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s1);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_defs, "defs", n1));
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
	char b[256], b2[256];
	char *s, *s1, *bty, *vty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2;

	s=*str;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token2(s, b, &ty, ctx->lang);
		i=BGBCP_DefTypeFlag(ctx, b);
		if(!i)break;

		fl|=i;
		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		j++;
		continue;
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
		*str=s;
		return(NULL);
	}

//	if(!bgbcp_strcmp(b, "class"))return(NULL);
//	if(!bgbcp_strcmp(b, "struct"))return(NULL);
//	if(!bgbcp_strcmp(b, "interface"))return(NULL);
//	if(!bgbcp_strcmp(b, "enum"))return(NULL);

	if(!bgbcp_strcmp(b, "class"))return(BGBCP_DefClassJ(ctx, str));
	if(!bgbcp_strcmp(b, "struct"))return(BGBCP_DefClassJ(ctx, str));
	if(!bgbcp_strcmp(b, "union"))return(BGBCP_DefClassJ(ctx, str));
	if(!bgbcp_strcmp(b, "interface"))return(BGBCP_DefClassJ(ctx, str));
	if(!bgbcp_strcmp4(b, "enum"))return(BGBCP_DefClassJ(ctx, str));

	if(ctx->lang==BGBCC_LANG_BS2)
	{
		if(bgbcp_chktoklst(b, bgbcp_basetypes_bs2))
		{
			s=BGBCP_Token2(s, b, &ty, ctx->lang);

			bty=bgbcc_strdup(b);

			if(!strncmp(bty, "__type_", 7))bty+=7;
			if(!strncmp(bty, "__", 2))bty+=2;
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

	if(!bty && (ty==BTK_NAME))
	{
		s=BGBCP_Token2(s, b, &ty, ctx->lang);
		BGBCP_Token2(s, b2, &ty2, ctx->lang);
		while(!bgbcp_strcmp1(b2, "."))
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
	if(fl)bty=vty;

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
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", i|fl);
		}else
		{
			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", bty);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);
		}
	}else if(vty)
	{
		n=BGBCP_LookupType(ctx, vty);
		if(n)
		{
			n=BCCX_FindTagCst(n, &bgbcc_rcst_type, "type");

			n=BCCX_Clone(n);
			i=BCCX_GetIntCst(n, &bgbcc_rcst_flags, "flags");
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", i|fl);
		}else if(BGBCP_CheckTypeName(ctx, vty))
		{
			n=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", vty);
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", fl);
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);
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
			BCCX_Add(n, BCCX_NewCst(&bgbcc_rcst_array, "array"));
			BGBCP_Token2(s, b, &ty, ctx->lang);
			continue;
		}

		n1=BGBCP_FunArgs(ctx, &s);
		BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_array, "array", n1));

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

	return(NULL);
}

BCCX_Node *BGBCP_EnumVarsList(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *lst, *lste;
	int i;

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

