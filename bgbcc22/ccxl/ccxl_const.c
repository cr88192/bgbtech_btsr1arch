#include <bgbccc.h>

#if 1
int BGBCC_CCXL_CheckExpandLvt4(BGBCC_TransState *ctx)
{
	int i;

	if(!ctx->ctab_lvt4)
	{
		i=1024;
		ctx->ctab_lvt4=bgbcc_malloc(i*sizeof(u32));
		ctx->ctab_lvt4[0]=0;
		ctx->n_ctab_lvt4=1;
		ctx->m_ctab_lvt4=i;
	}
	
	if((ctx->n_ctab_lvt4+1)>=ctx->m_ctab_lvt4)
	{
		i=ctx->m_ctab_lvt4;
		i=i+(i>>1);
		ctx->ctab_lvt4=bgbcc_realloc(ctx->ctab_lvt4, i*sizeof(u32));
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
		ctx->ctab_lvt8=bgbcc_malloc(i*sizeof(u64));
		ctx->ctab_lvt8[0]=0;
		ctx->n_ctab_lvt8=1;
		ctx->m_ctab_lvt8=i;
	}
	
	if((ctx->n_ctab_lvt8+1)>=ctx->m_ctab_lvt8)
	{
		i=ctx->m_ctab_lvt8;
		i=i+(i>>1);
		ctx->ctab_lvt8=bgbcc_realloc(ctx->ctab_lvt8, i*sizeof(u64));
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
	BGBCC_CCXL_LiteralInfo *cur;
	int h, c;
	int i, j, k;

#if 1
	h=BGBCC_CCXL_HashName(str);
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
			(cur->littype!=CCXL_LITID_FUNCTION))
				continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
#endif

#if 0
	c=ctx->usort_literal;
	while(c>0)
	{
		cur=ctx->literals[c];
		c=cur->hnext_name;

		if((cur->littype!=CCXL_LITID_STRUCT) &&
			(cur->littype!=CCXL_LITID_UNION) &&
			(cur->littype!=CCXL_LITID_CLASS) &&
			(cur->littype!=CCXL_LITID_FUNCTION))
				continue;
		if(!cur->name)
			continue;
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
