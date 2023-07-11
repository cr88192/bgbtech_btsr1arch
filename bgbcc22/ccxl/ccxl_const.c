#include <bgbccc.h>

#if 1
int BGBCC_CCXL_CheckExpandLvt4(BGBCC_TransState *ctx)
{
	int i;

	if(!ctx->ctab_lvt4)
	{
		i=1024;
		ctx->ctab_lvt4=bgbcc_malloc((i+5)*sizeof(u32));
		ctx->ctab_lvt4[0]=0;
		ctx->n_ctab_lvt4=1;
		ctx->m_ctab_lvt4=i;
	}
	
	if((ctx->n_ctab_lvt4+1)>=ctx->m_ctab_lvt4)
	{
		i=ctx->m_ctab_lvt4;
		i=i+(i>>1);
		ctx->ctab_lvt4=bgbcc_realloc(ctx->ctab_lvt4, (i+5)*sizeof(u32));
		ctx->m_ctab_lvt4=i;
	}
	
	return(0);
}

int BGBCC_CCXL_CheckExpandLvt8(BGBCC_TransState *ctx)
{
	int i;

	if(!ctx->ctab_lvt8)
	{
		i=1024;
		ctx->ctab_lvt8=bgbcc_malloc((i+5)*sizeof(u64));
//		ctx->ctab_lvt8=malloc((i+5)*sizeof(u64));
		ctx->ctab_lvt8[0]=0;
		ctx->n_ctab_lvt8=1;
		ctx->m_ctab_lvt8=i;
	}
	
	if((ctx->n_ctab_lvt8+1)>=ctx->m_ctab_lvt8)
	{
		i=ctx->m_ctab_lvt8;
		i=i+(i>>1);
		ctx->ctab_lvt8=bgbcc_realloc(ctx->ctab_lvt8, (i+5)*sizeof(u64));
//		ctx->ctab_lvt8=realloc(ctx->ctab_lvt8, (i+5)*sizeof(u64));
		ctx->m_ctab_lvt8=i;
	}

	return(0);
}

int BGBCC_CCXL_IndexLitU32(BGBCC_TransState *ctx, u32 val)
{
	int i;

	for(i=0; i<ctx->n_ctab_lvt4; i++)
	{
		if(ctx->ctab_lvt4[i]==val)
			return(i);
	}

	BGBCC_CCXL_CheckExpandLvt4(ctx);
	
	i=ctx->n_ctab_lvt4++;
	if(i>=ctx->m_ctab_lvt8)
		{ BGBCC_DBGBREAK }
	ctx->ctab_lvt4[i]=val;
	return(i);
}

int BGBCC_CCXL_IndexLitU64(BGBCC_TransState *ctx, u64 val)
{
	int i;

	for(i=0; i<ctx->n_ctab_lvt8; i++)
	{
		if(ctx->ctab_lvt8[i]==val)
			return(i);
	}

	BGBCC_CCXL_CheckExpandLvt8(ctx);
	
	i=ctx->n_ctab_lvt8++;
	ctx->ctab_lvt8[i]=val;
	return(i);
}

int BGBCC_CCXL_IndexLitS32(BGBCC_TransState *ctx, s32 val)
{
	return(BGBCC_CCXL_IndexLitU32(ctx, (u32)val));
}

int BGBCC_CCXL_IndexLitF32(BGBCC_TransState *ctx, float val)
{
	return(BGBCC_CCXL_IndexLitU32(ctx, *(u32 *)(&val)));
}

int BGBCC_CCXL_IndexLitS64(BGBCC_TransState *ctx, s64 val)
{
	return(BGBCC_CCXL_IndexLitU64(ctx, (u64)val));
}

int BGBCC_CCXL_IndexLitF64(BGBCC_TransState *ctx, double val)
{
	return(BGBCC_CCXL_IndexLitU64(ctx, *(u64 *)(&val)));
}
#endif

int BGBCC_CCXL_LookupString(BGBCC_TransState *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!ctx->strtab)
		return(-1);

	if(!str)return(0);
	if(!(*str))return(1);

	s=ctx->strtab; se=s+ctx->sz_strtab; se2=s+ctx->msz_strtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->strtab);
		s+=strlen(s)+1;
	}

	return(-1);
}

int BGBCC_CCXL_LookupWString(BGBCC_TransState *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!ctx->wstrtab)
		return(-1);

	if(!str)return(0);
	if(!(*str))return(1);

	s=ctx->wstrtab; se=s+ctx->sz_wstrtab; se2=s+ctx->msz_wstrtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->wstrtab);
		s+=strlen(s)+1;
	}

	return(-1);
}

int BGBCC_CCXL_IndexString(BGBCC_TransState *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!str)return(0);
	if(!(*str))return(1);

	if(!ctx->strtab)
	{
		ctx->strtab=bgbcc_malloc(1<<18);
		ctx->sz_strtab=2;
		ctx->msz_strtab=1<<18;
		ctx->strtab[0]=0;
		ctx->strtab[1]=0;
	}
	
	s=ctx->strtab; se=s+ctx->sz_strtab; se2=s+ctx->msz_strtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->strtab);
		s+=strlen(s)+1;
	}
	
	if((s+strlen(str)+1)>=se2)
	{
		i=ctx->msz_strtab;
		j=(s+strlen(str)+1)-ctx->strtab;
		k=s-ctx->strtab;
		
		while(i<=j)i=i+(i>>1);
		ctx->strtab=bgbcc_realloc(ctx->strtab, i);
		ctx->msz_strtab=i;
		s=ctx->strtab+k;
		se=ctx->strtab+ctx->sz_strtab;
		se2=ctx->strtab+ctx->msz_strtab;
	}

	i=s-ctx->strtab;
	strcpy(s, str);
	s+=strlen(s)+1;
	ctx->sz_strtab=s-ctx->strtab;
	return(i);
}

int BGBCC_CCXL_IndexWString(BGBCC_TransState *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!str)return(0);
	if(!(*str))return(1);

	if(!ctx->wstrtab)
	{
		ctx->wstrtab=bgbcc_malloc(1<<18);
		ctx->sz_wstrtab=2;
		ctx->msz_wstrtab=1<<18;
		ctx->wstrtab[0]=0;
		ctx->wstrtab[1]=0;
	}
	
	s=ctx->wstrtab; se=s+ctx->sz_wstrtab; se2=s+ctx->msz_wstrtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->wstrtab);
		s+=strlen(s)+1;
	}
	
	if((s+strlen(str)+1)>=se2)
	{
		i=ctx->msz_wstrtab;
		j=(s+strlen(str)+1)-ctx->wstrtab;
		k=s-ctx->wstrtab;
		
		while(i<=j)i=i+(i>>1);
		ctx->wstrtab=bgbcc_realloc(ctx->wstrtab, i);
		ctx->msz_wstrtab=i;
		s=ctx->wstrtab+k;
		se=ctx->wstrtab+ctx->sz_wstrtab;
		se2=ctx->wstrtab+ctx->msz_wstrtab;
	}

	i=s-ctx->wstrtab;
	strcpy(s, str);
	s+=strlen(s)+1;
	ctx->sz_wstrtab=s-ctx->wstrtab;
	return(i);
}


int BGBCC_CCXL_IndexCountString(BGBCC_TransState *ctx, char *str)
{
	char *s, *se, *se2, *s0, *s1;
	int n0, n1;
	int i, j, k;

	if(!str)return(0);
	if(!(*str))return(1);

	if(!ctx->cntstrs)
	{
		i=4096;
		ctx->cntstrn=bgbcc_malloc(i*sizeof(int));
		ctx->cntstrs=bgbcc_malloc(i*sizeof(char *));
		ctx->n_cntstrs=0;
		ctx->m_cntstrs=i;
	}

	for(i=0; i<ctx->n_cntstrs; i++)
	{
		if(!strcmp(ctx->cntstrs[i], str))
			break;
	}
	
	if(i<ctx->n_cntstrs)
	{
		ctx->cntstrn[i]++;
		while((i>0) && (ctx->cntstrn[i]>ctx->cntstrn[i-1]))
		{
			s0=ctx->cntstrs[i  ];	n0=ctx->cntstrn[i  ];
			s1=ctx->cntstrs[i-1];	n1=ctx->cntstrn[i-1];
			ctx->cntstrs[i  ]=s1;	ctx->cntstrn[i  ]=n1;
			ctx->cntstrs[i-1]=s0;	ctx->cntstrn[i-1]=n0;
			i--;
		}
		return(i);
	}
	
	if((ctx->n_cntstrs+1)>=ctx->m_cntstrs)
	{
		i=ctx->m_cntstrs;
		i=i+(i>>1);
		ctx->cntstrn=bgbcc_realloc(ctx->cntstrn, i*sizeof(int));
		ctx->cntstrs=bgbcc_realloc(ctx->cntstrs, i*sizeof(char *));
		ctx->m_cntstrs=i;
	}
	
	i=ctx->n_cntstrs++;
	ctx->cntstrn[i]=1;
	ctx->cntstrs[i]=bgbcc_strdup(str);
	return(i);
}

int BGBCC_CCXL_FlattenCountStrings(BGBCC_TransState *ctx)
{
	int i;

	if(!ctx->cntstrs)
		return(0);

	for(i=0; i<ctx->n_cntstrs; i++)
	{
		BGBCC_CCXL_IndexString(ctx, ctx->cntstrs[i]);
	}
	
	ctx->n_cntstrs=0;
	return(1);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStruct(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h, c;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=CCXL_LITID_STRUCT)
			continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
	return(NULL);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupUnion(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=CCXL_LITID_UNION)
			continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
	return(NULL);
}


BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupExportList(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=CCXL_LITID_EXPLIST)
			continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
	return(NULL);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructure(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur, *lcur;
	int h, c, lc;
	int i, j, k;

	if(!str)
		return(NULL);

	cur=NULL;

#if 1
//	h=BGBCC_CCXL_HashName(str);
	h=BGBCC_CCXL_HashNameNoSig(str);
	c=ctx->hash_literals[h];
	while(c>0)
	{
		cur=ctx->literals[c];
		c=cur->hnext_name;

//		if(cur->littype!=CCXL_LITID_STRUCT)
//			continue;
		if((cur->littype!=CCXL_LITID_STRUCT) &&
			(cur->littype!=CCXL_LITID_UNION) &&
			(cur->littype!=CCXL_LITID_CLASS) &&
			(cur->littype!=CCXL_LITID_ENUMDEF) &&
			(cur->littype!=CCXL_LITID_FUNCTION))
				continue;
//		if(!cur->name)
//			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
#endif

#if 1
	c=ctx->usort_literal; cur=NULL; lcur=NULL;
	while(c>0)
	{
		lcur=cur;
		cur=ctx->literals[c];
		lc=c;
		c=cur->hnext_name;

		if((cur->littype!=CCXL_LITID_STRUCT) &&
			(cur->littype!=CCXL_LITID_UNION) &&
			(cur->littype!=CCXL_LITID_CLASS) &&
			(cur->littype!=CCXL_LITID_FUNCTION))
				continue;
		if(!cur->name)
			continue;

		h=BGBCC_CCXL_HashNameNoSig(cur->name);
		if(lcur)
			{ lcur->hnext_name=c; }
		else
			{ ctx->usort_literal=c;	}
		cur->hnext_name=ctx->hash_literals[h];
		ctx->hash_literals[h]=lc;

		if(!strcmp(cur->name, str))
			return(cur);
	}
#endif

#if 0
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if((cur->littype!=CCXL_LITID_STRUCT) &&
			(cur->littype!=CCXL_LITID_UNION) &&
			(cur->littype!=CCXL_LITID_CLASS) &&
			(cur->littype!=CCXL_LITID_FUNCTION))
				continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
		{
			BGBCC_DBGBREAK
			return(cur);
		}
	}
#endif

	return(NULL);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureRns(
	BGBCC_TransState *ctx, char *name)
{
	char tb1[256];
	BGBCC_CCXL_LiteralInfo *cur, *ri;
	int i;

	ri=NULL;

	for(i=(ctx->n_imp-1); i>=0; i--)
	{
		sprintf(tb1, "%s/%s", ctx->imp_ns[i], name);
		cur=BGBCC_CCXL_LookupStructure(ctx, tb1);
		if(cur)
			{ ri=cur; break; }
	}
	
	if(!ri)
	{
		cur=BGBCC_CCXL_LookupStructure(ctx, name);
		if(cur)
			{ ri=cur; }
	}
	
	return(ri);
}

int BGBCC_CCXL_LookupStructureID(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	cur=BGBCC_CCXL_LookupStructure(ctx, str);
	if(cur)
		return(cur->litid);
	return(-1);
}

void BGBCC_CCXL_CheckExpandLiterals(
	BGBCC_TransState *ctx)
{
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetStruct(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h;
	int i, j, k;
	
	cur=BGBCC_CCXL_LookupStruct(ctx, str);
	if(cur)return(cur);
	
	BGBCC_CCXL_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	cur->littype=CCXL_LITID_STRUCT;
	cur->name=bgbcc_strdup(str);

	if(!ctx->n_literals)
		ctx->n_literals++;

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;

	h=BGBCC_CCXL_HashName(str);
	cur->hnext_name=ctx->hash_literals[h];
	ctx->hash_literals[h]=i;

	return(cur);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetUnion(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h;
	int i, j, k;
	
	cur=BGBCC_CCXL_LookupUnion(ctx, str);
	if(cur)return(cur);
	
	BGBCC_CCXL_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	cur->littype=CCXL_LITID_UNION;
	cur->name=bgbcc_strdup(str);

	if(!ctx->n_literals)
		ctx->n_literals++;

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;

	h=BGBCC_CCXL_HashName(str);
	cur->hnext_name=ctx->hash_literals[h];
	ctx->hash_literals[h]=i;
	
	return(cur);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetExportList(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h;
	int i, j, k;
	
	cur=BGBCC_CCXL_LookupExportList(ctx, str);
	if(cur)return(cur);
	
	BGBCC_CCXL_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	cur->littype=CCXL_LITID_EXPLIST;
	cur->name=bgbcc_strdup(str);

	if(!ctx->n_literals)
		ctx->n_literals++;

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;

	h=BGBCC_CCXL_HashName(str);
	cur->hnext_name=ctx->hash_literals[h];
	ctx->hash_literals[h]=i;
	
	return(cur);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupExportListForName(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h, c;
	int i, j, k;

#if 1
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=CCXL_LITID_EXPLIST)
			continue;
		if(!cur->name)
			continue;
//		if(!strcmp(cur->name, str))
//		{
//			BGBCC_DBGBREAK
//			return(cur);
//		}

		if(!cur->decl)
			continue;
		if(!cur->decl->goto_name)
			continue;
		for(j=0; j<cur->decl->n_goto; j++)
		{
			if(!strcmp(cur->decl->goto_name[j], str))
			{
				cur->decl->fxmoffs=j;
				return(cur);
			}
		}
	}
#endif

	return(NULL);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetTypedef2(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h;
	int i, j, k;
	
	cur=BGBCC_CCXL_LookupTypedef(ctx, str, NULL);
	if(cur)return(cur);
	
	BGBCC_CCXL_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	cur->littype=CCXL_LITID_TYPEDEF;
	cur->name=bgbcc_strdup(str);

	if(!ctx->n_literals)
		ctx->n_literals++;

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;

	h=BGBCC_CCXL_HashName(str);
	cur->hnext_name=ctx->hash_literals[h];
	ctx->hash_literals[h]=i;

	return(cur);
}

int BGBCC_CCXL_GetStructID(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	cur=BGBCC_CCXL_GetStruct(ctx, str);
	if(cur)
		return(cur->litid);
	return(-1);
}

int BGBCC_CCXL_GetUnionID(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	cur=BGBCC_CCXL_GetUnion(ctx, str);
	if(cur)
		return(cur->litid);
	return(-1);
}


BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupTypedef(
	BGBCC_TransState *ctx, char *name, char *sig)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=CCXL_LITID_TYPEDEF)
			continue;

		if(name)
		{
			if(!cur->name)
				continue;
			if(strcmp(cur->name, name))
				continue;
		}else
		{
			if(cur->name)
				continue;
		}

		if(sig)
		{
			if(!cur->sig)
				continue;
			if(strcmp(cur->sig, sig))
				continue;
		}
		
		return(cur);
	}
	return(NULL);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetTypedef(
	BGBCC_TransState *ctx, char *name, char *sig)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h;
	int i, j, k;
	
	cur=BGBCC_CCXL_LookupTypedef(ctx, name, sig);
	if(cur)return(cur);
	
	BGBCC_CCXL_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	cur->littype=CCXL_LITID_TYPEDEF;
	cur->name=bgbcc_strdup(name);
	cur->sig=bgbcc_strdup(sig);

	if(!ctx->n_literals)
		ctx->n_literals++;

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;

	h=BGBCC_CCXL_HashName(name);
	cur->hnext_name=ctx->hash_literals[h];
	ctx->hash_literals[h]=i;

	return(cur);
}


BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupNamespace(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h, c;
	int i, j, k;

	if(!str)
		return(NULL);

	h=BGBCC_CCXL_HashName(str);
	c=ctx->hash_literals[h];
	while(c>0)
	{
		cur=ctx->literals[c];
		c=cur->hnext_name;

		if(cur->littype!=CCXL_LITID_NAMESPACE)
			continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}

	return(NULL);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetNamespace(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXL_LiteralInfo *cur;
	int h;
	int i, j, k;
	
	cur=BGBCC_CCXL_LookupNamespace(ctx, str);
	if(cur)return(cur);
	
	BGBCC_CCXL_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	cur->littype=CCXL_LITID_NAMESPACE;
	cur->name=bgbcc_strdup(str);

	if(!ctx->n_literals)
		ctx->n_literals++;

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;

	h=BGBCC_CCXL_HashName(str);
	cur->hnext_name=ctx->hash_literals[h];
	ctx->hash_literals[h]=i;

	return(cur);
}

void BGBCC_CCXL_GetNamespaceBuildPath(
	BGBCC_TransState *ctx, char *str)
{
	char tb[256];
	char *s, *t;
	
	s=str; t=tb;
	while(*s)
	{
		if(*s=='/')
		{
			*t=0;
			BGBCC_CCXL_GetNamespace(ctx, tb);
		}
		*t++=*s++;
	}
	*t=0;
	BGBCC_CCXL_GetNamespace(ctx, tb);
}


int BGBCC_CCXL_CheckForOptStr(
	BGBCC_TransState *ctx, char *str)
{
	int i;
	
	for(i=0; i<ctx->noptstrs; i++)
	{
		if(!strcmp(ctx->optstrs[i], str))
			return(i+1);
	}
	return(0);
}

char *BGBCC_CCXL_CheckForOptParmStr(
	BGBCC_TransState *ctx, char *str)
{
	char *s;
	int l;
	int i;
	
	l=strlen(str);
	
	for(i=0; i<ctx->noptstrs; i++)
	{
		s=ctx->optstrs[i];
		if(!strncmp(s, str, l) && (s[l]=='='))
			return(s+l+1);
	}
	return(NULL);
}

int BGBCC_CCXL_CheckForOptParmInt(
	BGBCC_TransState *ctx, char *str)
{
	char *s;
	int l;
	int i;
	
	l=strlen(str);
	
	for(i=0; i<ctx->noptstrs; i++)
	{
		s=ctx->optstrs[i];
		if(!strncmp(s, str, l) && (s[l]=='='))
			return(atoi(s+l+1));
	}
	return(0);
}

int BGBCC_CCXL_ConstFloatAsHalf(float f, u16 *rv, u16 *rve)
{
	int exp, sgn;
	u32 fra, fra1, v;
	u16 v1, ve;
	int ret;
	
	v=*(u32 *)(&f);
	
	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;

	fra=v&0x007FFFFF;
	exp=(v>>23)&255;
	sgn=(v>>31)&1;
	
	exp=(exp-127)+15;
	fra1=fra>>13;
	ve=(fra>>8)&31;
	
	if((fra1<<13)!=fra)
		ret=0;
	
	if(exp<=0)
	{
		ret=0;
		exp=0;
		fra1=0;
	}
	
	if(exp>=31)
	{
		ret=-1;
		exp=31;
		fra1=0;
	}

	v1=(sgn<<15)|(exp<<10)|fra1;
	*rv=v1;
	*rve=ve;
	return(ret);
}

int BGBCC_CCXL_LookupArchVar(BGBCC_TransState *ctx, char *str)
{
	int i, h;
	
	if(!ctx->archvar_num)
		return(-1);
	
	h=BGBCC_CCXL_HashName(str)&255;
	i=ctx->archvar_hash[h];
	while(i>=0)
	{
		if(!strcmp(ctx->archvar_name[i], str))
			return(i);
		i=ctx->archvar_chn[i];
	}
	return(-1);
}

s64 BGBCC_CCXL_GetArchVarValForIndex(BGBCC_TransState *ctx, int idx)
{
	if(idx<0)
		return(-1);
	if(idx>=ctx->archvar_num)
		return(-1);

	return(ctx->archvar_val[idx]);
}

/* Define a constant that should not be expanded until
 * specialization for the backend. 
 */
int BGBCC_CCXL_DefineArchVar(BGBCC_TransState *ctx, char *str, s64 val)
{
	int i, h;
	
	if(!ctx->archvar_num)
	{
		for(i=0; i<256; i++)
			ctx->archvar_hash[i]=-1;
	}
	
	i=BGBCC_CCXL_LookupArchVar(ctx, str);
	if(i>=0)
	{
		ctx->archvar_val[i]=val;
		return(i);
	}
	
	h=BGBCC_CCXL_HashName(str)&255;
	i=ctx->archvar_num++;
	ctx->archvar_name[i]=bgbcc_strdup(str);
	ctx->archvar_val[i]=val;
	ctx->archvar_chn[i]=ctx->archvar_hash[h];
	ctx->archvar_hash[h]=i;
	return(i);
}
