#include <bgbccc.h>

BCCX_Node		*bccx_free_node=NULL;
BCCX_NodeList	*bccx_free_nodelist=NULL;

void			*bccx_chunk_node=NULL;
void			*bccx_chunk_nodelist=NULL;

int				bccx_stat_sznode;
int				bccx_stat_sznodelist;

char *bccx_stridx_strs[4096];
short bccx_stridx_next[4096];
short bccx_stridx_hash[256];
int bccx_stridx_num;

int bccx_init=0;

#if 1
int bccx_strcmp(char *s1, char *s2)
{
#if 1
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
	if(*s1 && (*s1==*s2))	{ s1++; s2++; }
	else					{ return(*s1-*s2); }
#endif

	while(*s1 && (*s1==*s2))
		{ s1++; s2++; }
	return(*s1-*s2);
}
#endif

#if 1
int BCCX_StringToStridx(char *str)
{
	char *s;
	int i, h;

	if(!bccx_init)
		BCCX_Init();

	s=str; h=0;
	while(*s)
		h=(h*127)+(*s++);
	h=((h*127)>>8)&255;
	
	i=bccx_stridx_hash[h];
	while(i>=0)
	{
		if(!strcmp(bccx_stridx_strs[i], str))
//		if(!bccx_strcmp(bccx_stridx_strs[i], str))
			return(i);
		i=bccx_stridx_next[i];
	}
	
	i=bccx_stridx_num++;
	bccx_stridx_strs[i]=bgbcc_strdup(str);
	bccx_stridx_next[i]=bccx_stridx_hash[h];
	bccx_stridx_hash[h]=i;
	return(i);
}

static char *bccx_stridx_nums[8]={
	"0","1","2","3","4","5","6","7"};

char *BCCX_StridxToString(int idx)
{
	if((idx&4095)>4088)
		return(bccx_stridx_nums[(4095-(idx&4095))]);

	return(bccx_stridx_strs[idx&4095]);
}

int BCCX_Init()
{
	int i;
	
	if(bccx_init)
		return(0);
	bccx_init=1;
	
	for(i=0; i<256; i++)
		bccx_stridx_hash[i]=-1;
	bccx_stridx_num=0;
	BCCX_StringToStridx("");
	
	return(1);
}
#endif


BCCX_Node *BCCX_AllocNode(void)
{
	void *chk;
	BCCX_Node *cur, *tblk;
	int i;

	if(bccx_free_node)
	{
		cur=bccx_free_node;
		bccx_free_node=*(BCCX_Node **)cur;
		*(BCCX_Node **)cur=NULL;
//		bccx_memset(cur, 0, sizeof(BCCX_Node));

		cur->magic=BCCX_NODE_MAGIC;
		return(cur);
	}

	BCCX_Init();
//	tblk=bgbcc_tmalloc("_bccx_node_t", 256*sizeof(BCCX_Node)+sizeof(void *));

	chk=bgbcc_tmalloc("_bccx_node_t", 256*sizeof(BCCX_Node)+sizeof(void *));
	*(void **)chk=bccx_chunk_node;
	bccx_chunk_node=chk;
	tblk=(BCCX_Node *)(((void **)chk)+1);
	bccx_stat_sznode+=256*sizeof(BCCX_Node)+sizeof(void *);
	
	memset(tblk, 0, 256*sizeof(BCCX_Node));
	for(i=0; i<256; i++)
	{
		cur=tblk+i;
		*(BCCX_Node **)cur=bccx_free_node;
		bccx_free_node=cur;
	}

	cur=bccx_free_node;
	bccx_free_node=*(BCCX_Node **)cur;
	*(BCCX_Node **)cur=NULL;
	cur->magic=BCCX_NODE_MAGIC;
	return(cur);
}

BCCX_NodeList *BCCX_AllocNodeList(void)
{
	BCCX_NodeList *cur, *tblk;
	void *chk;
	int i;

	if(bccx_free_nodelist)
	{
		cur=bccx_free_nodelist;
		bccx_free_nodelist=*(BCCX_NodeList **)cur;
		*(BCCX_NodeList **)cur=NULL;
//		bccx_memset(cur, 0, sizeof(BCCX_Node));
		return(cur);
	}

	BCCX_Init();
//	tblk=bgbcc_tmalloc("_bccx_nodelist_t", 256*sizeof(BCCX_NodeList));

	chk=bgbcc_tmalloc("_bccx_nodelist_t",
		256*sizeof(BCCX_NodeList)+sizeof(void *));
	*(void **)chk=bccx_chunk_nodelist;
	bccx_chunk_nodelist=chk;
	tblk=(BCCX_NodeList *)(((void **)chk)+1);
	bccx_stat_sznodelist+=256*sizeof(BCCX_NodeList)+sizeof(void *);

	memset(tblk, 0, 256*sizeof(BCCX_NodeList));
	for(i=0; i<256; i++)
	{
		cur=tblk+i;
		*(BCCX_NodeList **)cur=bccx_free_nodelist;
		bccx_free_nodelist=cur;
	}

	cur=bccx_free_nodelist;
	bccx_free_nodelist=*(BCCX_NodeList **)cur;
	*(BCCX_NodeList **)cur=NULL;
	return(cur);
}

void BCCX_FreeNode(BCCX_Node *n)
{
	if(!n)return;
	
	memset(n, 0, sizeof(BCCX_Node));

	*(BCCX_Node **)n=bccx_free_node;
	bccx_free_node=n;
}

void BCCX_FreeNodeList(BCCX_NodeList *n)
{
	if(!n)return;
	
	memset(n, 0, sizeof(BCCX_NodeList));

	*(BCCX_NodeList **)n=bccx_free_nodelist;
	bccx_free_nodelist=n;
}

int BCCX_LookupAttrArrIx(BCCX_Node *node, int iv,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	BCCX_Node *nl;
	int i, j;

	if(iv<0)
	{
		*rrn=NULL;
		*rrv=NULL;
		return(0);
	}

	if(!node->malvl)
	{
		if(iv>=8)
		{
			*rrn=NULL;
			*rrv=NULL;
			return(0);
		}

		*rrn=node->attr_n+iv;
		*rrv=(BCCX_AttrVal *)(node->attr_v+iv);
		return(1);
	}

	nl=node;
	i=nl->malvl;

	if(iv>=(1<<((i+1)*3)))
	{
		*rrn=NULL;
		*rrv=NULL;
		return(0);
	}

	while(i)
	{
		j=(iv>>(i*3))&7;
		nl=(BCCX_Node *)(nl->attr_v[j]);
		if(!nl)
		{
			*rrn=NULL;
			*rrv=NULL;
			return(0);
		}
		i=nl->malvl;
	}

	j=iv&7;
	*rrn=node->attr_n+j;
	*rrv=(BCCX_AttrVal *)(node->attr_v+j);
	return(1);
}

int BCCX_FetchAttrArrIx(BCCX_Node *node, int iv,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	BCCX_Node *nl, *nl1;
	int i, j;

	if(iv<0)
	{
		*rrn=NULL;
		*rrv=NULL;
		return(0);
	}

	i=node->malvl;
	while(iv>=(1<<((i+1)*3)))
	{
		nl=BCCX_AllocNode();
		memcpy(nl->attr_n, node->attr_n, 8*sizeof(u16));
		memcpy(nl->attr_v, node->attr_v, 8*sizeof(u64));
		nl->malvl=i;
		nl->ztag=node->ztag;

		i++;
		memset(node->attr_n, 0, 8*sizeof(u16));
		memset(node->attr_v, 0, 8*sizeof(u64));
		node->attr_v[0]=(u64)nl;
		node->malvl=i;
	}

	if(!node->malvl)
	{
		if(iv>=8)
		{
			*rrn=NULL;
			*rrv=NULL;
			return(0);
		}

		*rrn=node->attr_n+iv;
		*rrv=(BCCX_AttrVal *)(node->attr_v+iv);
		return(1);
	}

	nl=node;
	i=nl->malvl;

	while(i)
	{
		j=(iv>>(i*3))&7;
		nl1=(BCCX_Node *)(nl->attr_v[j]);
		if(!nl1)
		{
			nl1=BCCX_AllocNode();
			nl1->malvl=i-1;
			nl1->ztag=node->ztag;
			nl->attr_v[j]=(u64)nl1;
		}
		nl=nl1;
		i=nl->malvl;
	}

	j=iv&7;
	*rrn=node->attr_n+j;
	*rrv=(BCCX_AttrVal *)(node->attr_v+j);
	return(1);
}

int BCCX_LookupAttrVal(BCCX_Node *node, char *var,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	int iv;
	iv=BCCX_StringToStridx(var);
	return(BCCX_LookupAttrValIx(node, iv, rrn, rrv));
}

int BCCX_LookupAttrValCst(BCCX_Node *node,
	bccx_cxstate *rcst, char *var,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	int iv;

	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(var); *rcst=iv; }
	return(BCCX_LookupAttrValIx(node, iv, rrn, rrv));
}

int BCCX_LookupAttrValIx(BCCX_Node *node, int iv,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	u16 *attr_n, *an;
	BCCX_AttrVal *attr_v, *av;
	int m, n, c;
	int na, ma, mask, mask1;
	int i, j, k;
	
	*rrn=NULL;
	*rrv=NULL;
	
	if(!node)
		return(-1);
	
	if(node->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
		na=node->nattr;
		ma=BCCX_NODE_FIXATTR;
	}else
	{
//		attr_n=node->attr_v[0].p;
//		attr_v=node->attr_v[1].p;
		attr_n=NULL;
		attr_v=NULL;
		na=node->nattr;
//		ma=node->mattr;
		ma=1<<(3*node->malvl);
	}

	if(na>ma)
		{ BGBCC_DBGBREAK }

	mask1=4095;
	mask=4095;
//	if(iv&0x8000)
//		mask=65535;

	if(node->nattr<5)
	{
		for(i=0; i<node->nattr; i++)
		{
			j=attr_n[i];
			if((j&mask)==iv)
			{
				*rrn=attr_n+i;
				*rrv=attr_v+i;
				return(i);
			}
		}
	}else if(attr_n)
	{
		m=0; n=na;
		while(m!=(i=(m+n)>>1))
		{
			j=attr_n[i]&mask1;
//			if(iv>=j)
			if((iv&mask1)>=j)
				{ m=i; }
			else
				{ n=i; }
		}

		if((i<0) || (i>=na))
			{ BGBCC_DBGBREAK }

		j=attr_n[i]&mask;
		if(iv==j)
		{
			*rrn=attr_n+i;
			*rrv=attr_v+i;
			return(i);
		}

		j=attr_n[i+1]&mask;
		if(iv==j)
		{
			i++;
			*rrn=attr_n+i;
			*rrv=attr_v+i;
			return(i);
		}
	}else
	{
//		BCCX_FetchAttrArrIx()
		m=0; n=na;
		while(m!=(i=(m+n)>>1))
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
			j=(*an)&mask1;
			if((iv&mask1)>=j)
				{ m=i; }
			else
				{ n=i; }
		}

		if((i<0) || (i>=na))
			{ BGBCC_DBGBREAK }

		BCCX_FetchAttrArrIx(node, i, &an, &av);
		j=(*an)&mask;
		if(iv==j)
		{
			*rrn=an;
			*rrv=av;
			return(i);
		}

		BCCX_FetchAttrArrIx(node, i+1, &an, &av);
		j=(*an)&mask;
//		j=attr_n[i+1]&mask;
		if(iv==j)
		{
			i++;
			*rrn=an;
			*rrv=av;
			return(i);
		}
	}
	return(-1);
}

int BCCX_FetchAttrVal(BCCX_Node *node, char *var,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	int iv;
	iv=BCCX_StringToStridx(var);
	return(BCCX_FetchAttrValIx(node, iv, rrn, rrv));
}

int BCCX_FetchAttrValCst(BCCX_Node *node,
	bccx_cxstate *rcst, char *var,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	int iv;
	iv=*rcst;

	if(!iv)
		{ iv=BCCX_StringToStridx(var); *rcst=iv; }
	return(BCCX_FetchAttrValIx(node, iv, rrn, rrv));
}

int BCCX_FetchAttrValIx(BCCX_Node *node, int iv,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	u16 *attr_n, *an, *an1;
	BCCX_AttrVal *attr_v, *av, *av1;
	int na, ma, mask, mask1;
	int i, j, k;

	*rrn=NULL;
	*rrv=NULL;
	
	if(!node)
		return(-1);
	
	if(node->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
		na=node->nattr;
		ma=BCCX_NODE_FIXATTR;
	}else
	{
//		attr_n=node->attr_v[0].p;
//		attr_v=node->attr_v[1].p;
//		na=node->nattr;
//		ma=node->mattr;

		attr_n=NULL;
		attr_v=NULL;
		na=node->nattr;
		ma=1<<(3*node->malvl);
	}

	if(na>ma)
		{ BGBCC_DBGBREAK }

	mask=4095;
	mask1=4095;
//	if(iv&0x8000)
//		mask=65535;

	if(attr_n)
	{
		for(i=0; i<na; i++)
		{
			j=attr_n[i];
			if((j&mask)==iv)
//			if(((j&mask)==iv) && !((iv^j)&0x8000))
			{
				*rrn=attr_n+i;
				*rrv=attr_v+i;
				return(i);
			}
		}
		
	//	if(node->nattr<ma)
		if(na<ma)
		{
			i=node->nattr++;
			
	//		while((i>0) && attr_n[i-1]>iv)
//			while((i>0) && (attr_n[i-1]&mask)>iv)
//			while((i>0) && (attr_n[i-1]&mask)>(iv&mask))
			while((i>0) && (attr_n[i-1]&mask1)>(iv&mask1))
			{
				attr_n[i]=attr_n[i-1];
				attr_v[i]=attr_v[i-1];
				i--;
			}
			
			attr_n[i]=iv;
			*rrn=attr_n+i;
			*rrv=attr_v+i;
			return(i);
		}

		BCCX_FetchAttrArrIx(node, ma, &an, &av);
		return(BCCX_FetchAttrValIx(node, iv, rrn, rrv));
	}else
	{
		for(i=0; i<na; i++)
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
			j=*an;
			if((j&mask)==iv)
//			if(((j&mask)==iv) && !((iv^j)&0x8000))
			{
				*rrn=an;
				*rrv=av;
				return(i);
			}
		}
		
		i=node->nattr++;

//		while((i>0) && attr_n[i-1]>iv)
		while(i>0)
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
			BCCX_FetchAttrArrIx(node, i-1, &an1, &av1);
			if(((*an1)&mask1)<(iv&mask1))
				break;
			*an=*an1;
			*av=*av1;
			i--;
		}
		
		BCCX_FetchAttrArrIx(node, i, &an, &av);
		*an=iv;
		*rrn=an;
		*rrv=av;
		return(i);
	}

#if 0
	k=node->mattr;
	if(!k)
	{
		k=BCCX_NODE_FESCATTR;
		attr_n=bgbcc_malloc((k+1)*sizeof(u16));
		attr_v=bgbcc_malloc((k+1)*sizeof(u64));
		memcpy(attr_n, node->attr_n,
			BCCX_NODE_FIXATTR*sizeof(u16));
		memcpy(attr_v, node->attr_v,
			BCCX_NODE_FIXATTR*sizeof(u64));
		node->mattr=k;
		node->attr_v[0].p=attr_n;
		node->attr_v[1].p=attr_v;
		return(BCCX_FetchAttrValIx(node, iv, rrn, rrv));
	}
	
	k=k+(k>>1);
	attr_n=bgbcc_realloc(attr_n, (k+1)*sizeof(u16));
	attr_v=bgbcc_realloc(attr_v, (k+1)*sizeof(u64));
	node->attr_v[0].p=attr_n;
	node->attr_v[1].p=attr_v;
	node->mattr=k;	
	return(BCCX_FetchAttrValIx(node, iv, rrn, rrv));
#endif
}

char *BCCX_Get(BCCX_Node *n, char *var)
{
	bccx_cxstate iv;
	iv=0; return(BCCX_GetCst(n, &iv, var));
}

char *BCCX_GetCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var)
{
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	i=BCCX_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(av->s); }

		if((*an>>12)==BCCX_IVTY_INT)
		{
			tb=bgbcc_ralloc(64);
			sprintf(tb, "%lld", (long long)(av->i));
			return(tb);
		}

		if((*an>>12)==BCCX_IVTY_REAL)
		{
			tb=bgbcc_ralloc(64);
			sprintf(tb, "%f", av->f);
			return(tb);
		}
		return(NULL);
	}

	return(NULL);
}

#if 1
s64 BCCX_GetIntCst(BCCX_Node *n, bccx_cxstate *rcst, char *var)
{
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	i=BCCX_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCCX_IVTY_REAL)
			{ return(av->f); }
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		return(-1);
	}
	return(0);
}

double BCCX_GetFloatCst(BCCX_Node *n, bccx_cxstate *rcst, char *var)
{
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	i=BCCX_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCCX_IVTY_REAL)
			{ return(av->f); }
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		return(-1);
	}
	return(0);
}
#endif

#if 1
void BCCX_SetCst(BCCX_Node *n, bccx_cxstate *rcst, char *var, char *val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;

	if(!n)
		return;
	
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_STRING<<12);
	av->s=bgbcc_strdup(val);
}

void BCCX_SetIntCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, s64 val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;
	
	if(!n)
		return;
	
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_INT<<12);
	av->i=val;
}

void BCCX_SetFloatCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, double val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;

	if(!n)
		return;
	
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_REAL<<12);
	av->f=val;
}
#endif

BCCX_Node *BCCX_GetNodeCst(BCCX_Node *n, bccx_cxstate *rcst, char *var)
{
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	i=BCCX_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_NODE)
			{ return(av->p); }
		return(NULL);
	}
	return(NULL);
}

void BCCX_SetNodeCst(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, BCCX_Node *val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;

	BCCX_MarkTreeZone(val, n->ztag);

	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_NODE<<12);
	av->p=val;
}


BCCX_Node *BCCX_GetNodeIndex(BCCX_Node *n, int idx0)
{
	BCCX_NodeList *nl;
	BCCX_AttrVal *av;
	u16 *an;
	int i, j, k, ix, idx;

	if(idx0<0)
		return(NULL);

	idx=idx0-4;
	if(idx<0)
	{
//		ix=0x8000+idx0;
//		ix=(BCCX_IVTY_NODE<<12)+(4095-idx0);
		ix=4095-idx0;
		i=BCCX_LookupAttrValIx(n, ix, &an, &av);
		if(i>=0)
		{
//			return(av->p);

			if((*an>>12)==BCCX_IVTY_NODE)
				{ return(av->p); }
			return(NULL);
		}

		return(NULL);
	}

	nl=n->list;
	if(!nl)
		return(NULL);

	if((idx<0) || (idx>=nl->lisz))
		return(NULL);
	if(!nl->lilvl)
	{
		if(idx&(~15))
			{ BGBCC_DBGBREAK }
	
		return(nl->lidat[idx]);
	}

	i=nl->lilvl;
	if((i<0) || (i>6))
		{ BGBCC_DBGBREAK }
	
	while(i)
	{
		j=(idx>>(i*4))&15;
		nl=nl->lidat[j];
//		i=nl->lilvl;

		k=nl->lilvl;

		if(k!=(i-1))
			{ BGBCC_DBGBREAK }

		i=k;
	}
	return(nl->lidat[idx&15]);

#if 0
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i, ix;

	if((idx<0) || (idx>=32768))
		return(NULL);
	ix=0x8000+idx;
	i=BCCX_LookupAttrValIx(n, ix, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_NODE)
			{ return(av->p); }
		return(NULL);
	}
	return(NULL);
#endif
}

void BCCX_SetNodeIndex(BCCX_Node *n,
	int idx0, BCCX_Node *val)
{
	BCCX_NodeList *nl, *nl1;
	BCCX_AttrVal *av;
	u16 *an;
	int i, j, k, ix, idx;

	if(idx0<0)
	{
		BGBCC_DBGBREAK
		return;
	}

	BCCX_MarkTreeZone(val, n->ztag);

	idx=idx0-4;
	if(idx<0)
	{
//		ix=0x8000+idx0;
//		ix=(BCCX_IVTY_NODE<<12)+(4095-idx0);
		ix=4095-idx0;
		i=BCCX_FetchAttrValIx(n, ix, &an, &av);
		*an=(*an&4095)|(BCCX_IVTY_NODE<<12);
		av->p=val;
		return;
	}

	nl=n->list;
	if(!nl)
	{
		nl=BCCX_AllocNodeList();
		nl->ztag=n->ztag;
		n->list=nl;
	}
	
	while(idx>=(1<<((nl->lilvl+1)*4)))
	{
		nl1=BCCX_AllocNodeList();
		nl1->lisz=nl->lisz;
		nl1->lilvl=nl->lilvl+1;
		nl1->lidat[0]=nl;
		nl1->ztag=n->ztag;
		n->list=nl1;
		nl=nl1;
	}

	i=nl->lilvl;
	while(i)
	{
		j=(idx>>(i*4))&15;
		nl1=nl->lidat[j];
		if(!nl1)
		{
			nl1=BCCX_AllocNodeList();
			nl1->lilvl=i-1;
			nl1->ztag=n->ztag;
			nl->lidat[j]=nl1;
		}
		nl=nl1;
		i=nl->lilvl;
	}
	nl->lidat[idx&15]=val;
	return;


#if 0
	if(!n->list)
	{
		if(!val)
			return;
	
		k=16;
		while(idx>=k)
			k=k+(k>>1);
		n->list=bgbcc_malloc(k*sizeof(BCCX_Node *));
		n->limax=k;
		n->lisz=idx+1;
		n->list[idx]=val;
		return;
	}

	if((idx<0) || (idx>=65536))
	{
		BGBCC_DBGBREAK
		return;
	}
	
	if(idx>=n->limax)
	{
		k=n->limax;
		while(idx>=k)
			k=k+(k>>1);
		n->list=bgbcc_realloc(n->list, k*sizeof(BCCX_Node *));
		n->limax=k;
	}
	
	if(idx>=n->lisz)
		n->lisz=idx+1;

	n->list[idx]=val;
#endif

#if 0
	u16 *an;
	BCCX_AttrVal *av;
	int i, ix;

	if((idx<0) || (idx>=32768))
	{
		BGBCC_DBGBREAK
		return;
	}
	
	ix=0x8000+idx;
	i=BCCX_FetchAttrValIx(n, ix, &an, &av);
//	*an=(*an&4095)|(BCCX_IVTY_NODE<<12);
	av->p=val;
#endif
}

BCCX_Node *BCCX_New(char *tag)
{
	BCCX_Node *n;
	n=BCCX_AllocNode();
	n->ztag=BCCX_ZTY_REDUCE;
	n->itag=BCCX_StringToStridx(tag);
	return(n);
}

BCCX_Node *BCCX_NewCst(bccx_cxstate *rcst, char *tag)
{
	BCCX_Node *n;
	int iv;
	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(tag); *rcst=iv; }
	n=BCCX_AllocNode();
	n->ztag=BCCX_ZTY_REDUCE;
	n->itag=iv;
	return(n);
}

#if 1

bccx_cxstate	bccx_text;

bccx_cxstate	bccx_Stext;
bccx_cxstate	bccx_Scdata;
bccx_cxstate	bccx_Slist;

BCCX_Node *BCCX_NewText(char *text)
{
	BCCX_Node *n;
	n=BCCX_NewCst(&bccx_Stext, "$text");
	BCCX_SetCst(n, &bccx_Stext, "$text", text);
	return(n);
}

BCCX_Node *BCCX_NewCData(char *text)
{
	BCCX_Node *n;
	n=BCCX_NewCst(&bccx_Scdata, "$cdata");
	BCCX_SetCst(n, &bccx_Stext, "$text", text);
	return(n);
}

BCCX_Node *BCCX_NewList(void)
{
	return(BCCX_NewCst(&bccx_Slist, "$list"));
}
#endif

int BCCX_GetNodeChildCount(BCCX_Node *node)
{
	int ix;
	if(!node)
		return(0);
	if(!node->list)
		return(node->lifx);
	ix=node->list->lisz;
	ix+=node->lifx;
	return(ix);
}

BCCX_Node *BCCX_AddEnd(BCCX_Node *node, BCCX_Node *child)
{
	if(!node)
	{
		node=BCCX_NewList();
		node->itag=(node->itag&4095)|(BCCX_NTY_TRANS<<12);
	}else
	{
		if((node->itag&4095)!=bccx_Slist)
		{
			BGBCC_DBGBREAK
		}
	}
	
	if(child)
	{
		BCCX_Add(node, child);
	}
	
	return(node);
}

BCCX_Node *BCCX_AddEnd2(BCCX_Node *node, BCCX_Node **rne, BCCX_Node *child)
{
	return(BCCX_AddEnd(node, child));
}

int BCCX_Add(BCCX_Node *node, BCCX_Node *child)
{
	BCCX_Node *cur, *lst;
	int ix, na, ci;

	if(!child)
		return(-1);

	if((child->itag&4095)==bccx_Slist)
	{
		na=BCCX_GetNodeChildCount(child);
		for(ci=0; ci<na; ci++)
		{
			cur=BCCX_GetNodeIndex(child, ci);
			BCCX_Add(node, cur);
		}
		return(0);
	}

	if(node->list)
	{
		ix=node->list->lisz;
		ix+=node->lifx;
		BCCX_SetNodeIndex(node, ix, child);
		node->list->lisz++;
	}else
	{
		ix=node->lifx;
		BCCX_SetNodeIndex(node, ix, child);
		if(node->list)
			node->list->lisz++;
		else
			node->lifx++;
	}
	
	return(ix);
}

void BCCX_AddV(BCCX_Node *parent, BCCX_Node *child)
{
	BCCX_Node *ch1, *cur;
	u16 *an;
	BCCX_AttrVal *av;
	int i, na, ci;

	if(!parent)
		return;
	if(!child)
		return;

	if((child->itag&4095)==bccx_Slist)
	{
		na=BCCX_GetNodeChildCount(child);
		for(ci=0; ci<na; ci++)
		{
			cur=BCCX_GetNodeIndex(child, ci);
			BCCX_Add(parent, cur);
		}
		return;
	}

	if(parent->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	if(child->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

#if 1
	if((child->itag>>12)==BCCX_NTY_TRANS)
	{
		i=BCCX_FetchAttrValIx(parent, child->itag&4095, &an, &av);
		if((*an>>12)==BCCX_IVTY_DEFAULT)
		{
			*an=(*an&4095)|(BCCX_IVTY_NODE<<12);
			ch1=BCCX_GetNodeIndex(child, 0);
			av->p=ch1;

//			av->p=child->down;
//			if(child->down)
//				child->down->up=parent;
			return;
		}
	}
#endif

	BCCX_Add(parent, child);
}


BCCX_Node *BCCX_New1V(char *tag, BCCX_Node *a)
{
	BCCX_Node *n;

	if((a->itag&4095)==bccx_Slist)
	{
		BCCX_SetTag(a, tag);
		return(a);
	}
	
	n=BCCX_New(tag); BCCX_Add(n, a);
	n->itag=(n->itag&4095)|(BCCX_NTY_TRANS<<12);
	return(n);
}

BCCX_Node *BCCX_NewCst1V(bccx_cxstate *rcst, char *tag, BCCX_Node *a)
{
	BCCX_Node *n;
	
	if(a && ((a->itag&4095)==bccx_Slist))
	{
		BCCX_SetTagCst(a, rcst, tag);
		return(a);
	}
	
	n=BCCX_NewCst(rcst, tag); BCCX_Add(n, a);
	n->itag=(n->itag&4095)|(BCCX_NTY_TRANS<<12);
	return(n);
}

BCCX_Node *BCCX_New1(char *tag, BCCX_Node *a)
{
	BCCX_Node *n;
	n=BCCX_New(tag); BCCX_AddV(n, a);
	return(n);
}

BCCX_Node *BCCX_New2(char *tag, BCCX_Node *a, BCCX_Node *b)
{
	BCCX_Node *n;
	n=BCCX_New1(tag, a); BCCX_AddV(n, b);
	return(n);
}

BCCX_Node *BCCX_New3(char *tag, BCCX_Node *a,
	BCCX_Node *b, BCCX_Node *c)
{
	BCCX_Node *n;
	n=BCCX_New2(tag, a, b); BCCX_AddV(n, c);
	return(n);
}

BCCX_Node *BCCX_New4(char *tag, BCCX_Node *a,
	BCCX_Node *b, BCCX_Node *c, BCCX_Node *d)
{
	BCCX_Node *n;
	n=BCCX_New3(tag, a, b, c); BCCX_AddV(n, d);
	return(n);
}

BCCX_Node *BCCX_NewCst1(bccx_cxstate *rcst, char *tag, BCCX_Node *a)
{
	BCCX_Node *n;

	if(a && ((a->itag&4095)==bccx_Slist))
	{
		BCCX_SetTagCst(a, rcst, tag);
		return(a);
	}
	
	n=BCCX_NewCst(rcst, tag); BCCX_AddV(n, a);
	return(n);
}

BCCX_Node *BCCX_NewCst2(bccx_cxstate *rcst, char *tag,
	BCCX_Node *a, BCCX_Node *b)
{
	BCCX_Node *n;
	n=BCCX_NewCst1(rcst, tag, a); BCCX_AddV(n, b);
	return(n);
}

BCCX_Node *BCCX_NewCst3(bccx_cxstate *rcst, char *tag, BCCX_Node *a,
	BCCX_Node *b, BCCX_Node *c)
{
	BCCX_Node *n;
	n=BCCX_NewCst2(rcst, tag, a, b); BCCX_AddV(n, c);
	return(n);
}

BCCX_Node *BCCX_NewCst4(bccx_cxstate *rcst, char *tag, BCCX_Node *a,
	BCCX_Node *b, BCCX_Node *c, BCCX_Node *d)
{
	BCCX_Node *n;
	n=BCCX_NewCst3(rcst, tag, a, b, c); BCCX_AddV(n, d);
	return(n);
}

char *BCCX_Tag(BCCX_Node *n)
	{ return(BCCX_StridxToString(n->itag)); }

char *BCCX_Text(BCCX_Node *n)
	{ return(BCCX_GetCst(n, &bccx_Stext, "$text")); }

void BCCX_SetTagCst(BCCX_Node *n, bccx_cxstate *rcst, char *str)
{
	int iv;
	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(str); *rcst=iv; }
	n->itag=iv;
}

void BCCX_SetTag(BCCX_Node *n, char *str)
{
	int iv;
	iv=BCCX_StringToStridx(str);
	n->itag=iv;
}

int BCCX_TagIsCstP(BCCX_Node *n, bccx_cxstate *rcst, char *str)
{
	int ix;
	if(!n)return(0);
	if(!n->itag)return(0);
	
	ix=*rcst;
	if(!ix)
		{ ix=BCCX_StringToStridx(str); *rcst=ix; }
	return(n->itag==ix);
}

int BCCX_TagIsP(BCCX_Node *n, char *str)
{
	int ix;

	if(!n)return(0);
	if(!n->itag)return(0);
	
	ix=BCCX_StringToStridx(str);
	return(n->itag==ix);
}

int BCCX_AttrIsCstP(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, char *val)
{
	char *s;

	if(!n)return(0);
	if(!n->itag)return(0);

	s=BCCX_GetCst(n, rcst, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
	if(!strcmp(s, val))
//	if(!bccx_strcmp(s, val))
		return(1);
	return(0);
}

int BCCX_TagAttrIsP(BCCX_Node *n, char *tag, char *var, char *val)
{
	char *s;

	if(!n)return(0);
	if(!n->itag)return(0);

//	if(bccx_strcmp(BCCX_StridxToString(n->itag), tag))
	if(strcmp(BCCX_StridxToString(n->itag), tag))
		return(0);

	s=BCCX_Get(n, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
//	if(!bccx_strcmp(s, val))
	if(!strcmp(s, val))
		return(1);
	return(0);
}

// BCCX_Node *BCCX_FindTag(BCCX_Node *parent, char *tag)
//	{ return(BCCX_FindNextTag(parent->down, tag)); }
//BCCX_Node *BCCX_FindAttr(BCCX_Node *parent, char *var, char *val)
//	{ return(BCCX_FindNextAttr(parent->down, var, val)); }
//BCCX_Node *BCCX_FindTagAttr(BCCX_Node *parent,
//		char *tag, char *var, char *val)
//	{ return(BCCX_FindNextTagAttr(parent->down, tag, var, val)); }

BCCX_Node *BCCX_Child(BCCX_Node *parent)
{
	BCCX_Node *tmp;
	tmp=BCCX_GetNodeIndex(parent, 0);
	return(tmp);
}

BCCX_Node *BCCX_FindTagIx(BCCX_Node *parent, int iv)
{
	BCCX_Node *tmp;
	int i, j, k, na;
	
	na=BCCX_GetNodeChildCount(parent);
	for(i=0; i<na; i++)
	{
		tmp=BCCX_GetNodeIndex(parent, i);
		if(!tmp)
			continue;
		if((tmp->itag&4095)==(iv&4095))
			return(tmp);
	}
	
	return(NULL);

//	return(BCCX_FindNextTagIx(parent->down, iv));
}

int BCCX_FindTagIndexCst(BCCX_Node *parent,
	bccx_cxstate *rcst, char *tag)
{
	BCCX_Node *tmp;
	int i, j, k, na, iv;

	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(tag); *rcst=iv; }

	na=BCCX_GetNodeChildCount(parent);
	for(i=0; i<na; i++)
	{
		tmp=BCCX_GetNodeIndex(parent, i);
		if(!tmp)
			continue;
		if((tmp->itag&4095)==(iv&4095))
			return(i);
	}
	
	return(-1);
}

BCCX_Node *BCCX_FindTagCst(BCCX_Node *parent, bccx_cxstate *rcst, char *tag)
{
	BCCX_Node *tmp;
	int iv;

	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(tag); *rcst=iv; }
	return(BCCX_FindTagIx(parent, iv));

//	return(BCCX_FindNextTagIx(parent->down, iv));
}

BCCX_Node *BCCX_FindTag(BCCX_Node *parent, char *tag)
{
	BCCX_Node *tmp;
	int iv;

	iv=BCCX_StringToStridx(tag);
	return(BCCX_FindTagIx(parent, iv));

//	return(BCCX_FindNextTagIx(parent->down, iv));
}

BCCX_Node *BCCX_Fetch(BCCX_Node *parent, char *tag)
{
	u16 *an;
	BCCX_Node *tmp;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	if(parent->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	i=BCCX_LookupAttrVal(parent, tag, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_NODE)
			{ return(av->p); }
	}

	tmp=BCCX_FindTag(parent, tag);
	if(tmp)return(BCCX_Child(tmp));
	return(NULL);
}


BCCX_Node *BCCX_FetchCst(BCCX_Node *parent, bccx_cxstate *rcst, char *tag)
{
	u16 *an;
	BCCX_Node *tmp;
	BCCX_AttrVal *av;
	char *tb;
	int iv;
	int i;

	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(tag); *rcst=iv; }

	if(parent->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

//	i=BCCX_LookupAttrVal(parent, tag, &an, &av);
	i=BCCX_LookupAttrValIx(parent, iv, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCCX_IVTY_NODE)
			{ return(av->p); }
	}

	tmp=BCCX_FindTagIx(parent, iv);
	if(tmp)return(BCCX_Child(tmp));
	return(NULL);
}

#if 0
BCCX_Node *BCCX_Clone(BCCX_Node *n)
{
	BCCX_Node *t, *t1, *c, *l;
//	BCCX_Attr *ac, *at, *al;

	if(!n)return(NULL);

	t=BCCX_AllocNode();
	t->ztag=BCCX_ZTY_REDUCE;

	t->itag=n->itag;

	if(!n->mattr)
	{
		t->nattr=n->nattr;
		t->mattr=0;
		
		memcpy(t->attr_n, n->attr_n, n->nattr*sizeof(u16));
		memcpy(t->attr_v, n->attr_v, n->nattr*sizeof(u64));
	}else
	{
		t->attr_v[0].p=bgbcc_malloc(n->mattr*sizeof(u16));
		t->attr_v[1].p=bgbcc_malloc(n->mattr*sizeof(u64));
		memcpy(t->attr_n, n->attr_v[0].p, n->nattr*sizeof(u16));
		memcpy(t->attr_v, n->attr_v[1].p, n->nattr*sizeof(u64));
		t->nattr=n->nattr;
		t->mattr=n->mattr;
	}

	return(t);	
}
#endif

int BCCX_MarkNodeListZone(BCCX_NodeList *nl, int zone)
{
	int i, j, k;

	if(!nl)
		return(0);

	nl->ztag=zone;

	if(nl->lilvl)
	{
		for(i=0; i<16; i++)
		{
			BCCX_MarkNodeListZone(nl->lidat[i], zone);
		}
		return(1);
	}

	for(i=0; i<16; i++)
	{
		BCCX_MarkTreeZone(nl->lidat[i], zone);
	}

	return(1);
}

int BCCX_MarkAttrListZone(BCCX_Node *n, int zone)
{
	BCCX_Node *tn;
	BCCX_AttrVal *av;
	u16 *an;
	int i, j, k;

	if(!n)
		return(0);

	n->ztag=zone;

	an=n->attr_n;
	av=(BCCX_AttrVal *)(n->attr_v);

	if(!n->malvl)
	{
		for(i=0; i<BCCX_NODE_FIXATTR; i++)
		{
			if((an[i]>>12)==BCCX_IVTY_NODE)
			{
				tn=av[i].p;
				BCCX_MarkTreeZone(tn, zone);
			}
		}
	}else
	{
		for(i=0; i<BCCX_NODE_FIXATTR; i++)
		{
			if((an[i]>>12)==BCCX_IVTY_NODE)
			{
				tn=av[i].p;
				BCCX_MarkAttrListZone(tn, zone);
			}
		}
	}
	return(1);
}

int BCCX_MarkTreeZone(BCCX_Node *n, int zone)
{
	BCCX_Node *tn;
	BCCX_NodeList *nl;

	int i, j, k;

	if(!n)
		return(0);

	if(n->ztag<=zone)
		return(0);

	n->ztag=zone;

	BCCX_MarkAttrListZone(n, zone);
	BCCX_MarkNodeListZone(n->list, zone);
	return(1);
}


BCCX_NodeList *BCCX_CloneNodeListZone(BCCX_NodeList *nl, int zone)
{
	BCCX_NodeList *nlt;
	int i, j, k;

	if(!nl)
		return(NULL);

	if(nl->lilvl)
	{
		nlt=BCCX_AllocNodeList();
		nlt->lisz=nl->lisz;
		nlt->lilvl=nl->lilvl;
		for(i=0; i<16; i++)
			{ nlt->lidat[i]=BCCX_CloneNodeListZone(nl->lidat[i], zone); }
		return(nlt);
	}

	nlt=BCCX_AllocNodeList();
	nlt->lisz=nl->lisz;
	nlt->lilvl=nl->lilvl;
	for(i=0; i<16; i++)
		{ nlt->lidat[i]=BCCX_CloneNodeZone(nl->lidat[i], zone); }
	return(nlt);
}

BCCX_Node *BCCX_CloneAttrListZone(BCCX_Node *n, int zone)
{
	BCCX_Node *tn, *xn;
	BCCX_AttrVal *av, *xav;
	u16 *an, *xan;
	int i, j, k;

	if(!n)
		return(NULL);

	xn=BCCX_AllocNode();
	xn->itag=n->itag;
	xn->nattr=n->nattr;
	xn->malvl=n->malvl;
	xn->ztag=zone;
	xn->lifx=n->lifx;

	an=n->attr_n;
	av=(BCCX_AttrVal *)(n->attr_v);

	xan=xn->attr_n;
	xav=(BCCX_AttrVal *)(xn->attr_v);

	if(!n->malvl)
	{
		for(i=0; i<BCCX_NODE_FIXATTR; i++)
		{
			if((an[i]>>12)==BCCX_IVTY_NODE)
			{
				tn=av[i].p;
				tn=BCCX_CloneNodeZone(tn, zone);
				xan[i]=an[i];
				xav[i].p=tn;
			}else
			{
				xan[i]=an[i];
				xav[i]=av[i];
			}
		}
	}else
	{
		for(i=0; i<BCCX_NODE_FIXATTR; i++)
		{
			xav[i].p=BCCX_CloneAttrListZone(av[i].p, zone);
		}
	}
	return(xn);
}

BCCX_Node *BCCX_CloneNodeZone(BCCX_Node *n, int zone)
{
	BCCX_Node *tn;
	
	if(!n)
		return(NULL);
	
	tn=BCCX_CloneAttrListZone(n, zone);
	tn->list=BCCX_CloneNodeListZone(n->list, zone);
	return(tn);
}

BCCX_Node *BCCX_Clone(BCCX_Node *n)
{
	return(BCCX_CloneNodeZone(n, BCCX_ZTY_REDUCE));
}

BCCX_Node *BCCX_CloneS(BCCX_Node *n)
{
	return(n);
}

int BCCX_Unlink2(BCCX_Node *node, BCCX_Node *child)
{
	BCCX_Node *c;
	int na, ci, cj;

	na=BCCX_GetNodeChildCount(node);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(node, ci);
		if(c==child)
			break;
	}
	
	if(ci>=na)
		return(0);

	for(; ci<(na-1); ci++)
	{
		c=BCCX_GetNodeIndex(node, ci+1);
		BCCX_SetNodeIndex(node, ci, c);
	}

	if(node->list && (node->list->lisz>0))
		{ node->list->lisz--; }
	else if(node->lifx>0)
		node->lifx--;

	return(1);
}

void BCCX_CheckDeleteUnlinked(BCCX_Node *n)
{
}

void BCCX_ClearZoneLevel(int zone)
{
	BCCX_Node *tn, *tne;
	BCCX_NodeList *tnl, *tnle;
	void *ptr;

	ptr=bccx_chunk_node;
	while(ptr)
	{
		tn=(BCCX_Node *)(((void **)ptr)+1);
		tne=tn+256;
		while(tn<tne)
		{
			if(tn->ztag>=zone)
			{
				memset(tn, 0, sizeof(BCCX_Node));
				*(void **)tn=bccx_free_node;
				bccx_free_node=tn;
			}
		
			tn++;
		}
	
		ptr=*(void **)ptr;
	}

	ptr=bccx_chunk_nodelist;
	while(ptr)
	{
		tnl=(BCCX_NodeList *)(((void **)ptr)+1);
		tnle=tnl+256;
		while(tnl<tnle)
		{
			if(tnl->ztag>=zone)
			{
				memset(tnl, 0, sizeof(BCCX_NodeList));
				*(void **)tnl=bccx_free_nodelist;
				bccx_free_nodelist=tnl;
			}
		
			tnl++;
		}
	
		ptr=*(void **)ptr;
	}
}
