#include <bgbccc.h>

BCAST_Node		*bcast_free_node=NULL;
BCAST_NodeList	*bcast_free_nodelist=NULL;

char *bcast_stridx_strs[4096];
short bcast_stridx_next[4096];
short bcast_stridx_hash[256];
int bcast_stridx_num;

int bcast_init=0;

#if 0
int bcast_strcmp(char *s1, char *s2)
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

#if 0
int BCAST_StringToStridx(char *str)
{
	char *s;
	int i, h;

	if(!bcast_init)
		BCAST_Init();

	s=str; h=0;
	while(*s)
		h=(h*127)+(*s++);
	h=((h*127)>>8)&255;
	
	i=bcast_stridx_hash[h];
	while(i>=0)
	{
		if(!strcmp(bcast_stridx_strs[i], str))
//		if(!bcast_strcmp(bcast_stridx_strs[i], str))
			return(i);
		i=bcast_stridx_next[i];
	}
	
	i=bcast_stridx_num++;
	bcast_stridx_strs[i]=bgbcc_strdup(str);
	bcast_stridx_next[i]=bcast_stridx_hash[h];
	bcast_stridx_hash[h]=i;
	return(i);
}

char *BCAST_StridxToString(int idx)
{
	return(bcast_stridx_strs[idx&4095]);
}

int BCAST_Init()
{
	int i;
	
	if(bcast_init)
		return(0);
	bcast_init=1;
	
	for(i=0; i<256; i++)
		bcast_stridx_hash[i]=-1;
	bcast_stridx_num=0;
	BCAST_StringToStridx("");
	
	return(1);
}
#endif

int BCAST_StringToStridx(char *str)
{
	return(BCCX_StringToStridx(str));
}

char *BCAST_StridxToString(int idx)
{
	return(BCCX_StridxToString(idx));
//	return(bcast_stridx_strs[idx&4095]);
}

int BCAST_Init()
{
	BCCX_Init();
	return(0);
}


BCAST_Node *BCAST_AllocNode(void)
{
	BCAST_Node *cur, *tblk;
	int i;

	if(bcast_free_node)
	{
		cur=bcast_free_node;
		bcast_free_node=*(BCAST_Node **)cur;
		*(BCAST_Node **)cur=NULL;
//		bcast_memset(cur, 0, sizeof(BCAST_Node));
		return(cur);
	}

	BCAST_Init();
	tblk=bgbcc_tmalloc("_bcast_node_t", 256*sizeof(BCAST_Node));
	memset(tblk, 0, 256*sizeof(BCAST_Node));
	for(i=0; i<256; i++)
	{
		cur=tblk+i;
		*(BCAST_Node **)cur=bcast_free_node;
		bcast_free_node=cur;
	}

	cur=bcast_free_node;
	bcast_free_node=*(BCAST_Node **)cur;
	*(BCAST_Node **)cur=NULL;
	return(cur);
}

BCAST_NodeList *BCAST_AllocNodeList(void)
{
	BCAST_NodeList *cur, *tblk;
	int i;

	if(bcast_free_nodelist)
	{
		cur=bcast_free_nodelist;
		bcast_free_nodelist=*(BCAST_NodeList **)cur;
		*(BCAST_NodeList **)cur=NULL;
//		bcast_memset(cur, 0, sizeof(BCAST_Node));
		return(cur);
	}

	BCAST_Init();
	tblk=bgbcc_tmalloc("_bcast_nodelist_t", 256*sizeof(BCAST_NodeList));
	memset(tblk, 0, 256*sizeof(BCAST_NodeList));
	for(i=0; i<256; i++)
	{
		cur=tblk+i;
		*(BCAST_NodeList **)cur=bcast_free_nodelist;
		bcast_free_nodelist=cur;
	}

	cur=bcast_free_nodelist;
	bcast_free_nodelist=*(BCAST_NodeList **)cur;
	*(BCAST_NodeList **)cur=NULL;
	return(cur);
}

void BCAST_FreeNode(BCAST_Node *n)
{
	if(!n)return;
	
	memset(n, 0, sizeof(BCAST_Node));

	*(BCAST_Node **)n=bcast_free_node;
	bcast_free_node=n;
}

void BCAST_FreeNodeList(BCAST_NodeList *n)
{
	if(!n)return;
	
	memset(n, 0, sizeof(BCAST_NodeList));

	*(BCAST_NodeList **)n=bcast_free_nodelist;
	bcast_free_nodelist=n;
}

int BCAST_LookupAttrArrIx(BCAST_Node *node, int iv,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	BCAST_Node *nl;
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
		*rrv=(BCAST_AttrVal *)(node->attr_v+iv);
		return(1);
	}

	nl=node;
	i=nl->malvl;

	if(iv>=(1<<(i*3)))
	{
		*rrn=NULL;
		*rrv=NULL;
		return(0);
	}

	while(i)
	{
		j=(iv>>(i*3))&7;
		nl=(BCAST_Node *)(nl->attr_v[j]);
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
	*rrv=(BCAST_AttrVal *)(node->attr_v+j);
	return(1);
}

int BCAST_FetchAttrArrIx(BCAST_Node *node, int iv,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	BCAST_Node *nl, *nl1;
	int i, j;

	if(iv<0)
	{
		*rrn=NULL;
		*rrv=NULL;
		return(0);
	}

	i=node->malvl;
	while(iv>=(1<<(i*3)))
	{
		nl=BCAST_AllocNode();
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
		*rrv=(BCAST_AttrVal *)(node->attr_v+iv);
		return(1);
	}

	nl=node;
	i=nl->malvl;

	while(i)
	{
		j=(iv>>(i*3))&7;
		nl1=(BCAST_Node *)(nl->attr_v[j]);
		if(!nl1)
		{
			nl1=BCAST_AllocNode();
			nl1->malvl=i-1;
			nl1->ztag=node->ztag;
			nl->attr_v[j]=(u64)nl1;
		}
		nl=nl1;
		i=nl->malvl;
	}

	j=iv&7;
	*rrn=node->attr_n+j;
	*rrv=(BCAST_AttrVal *)(node->attr_v+j);
	return(1);
}

int BCAST_LookupAttrVal(BCAST_Node *node, char *var,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	int iv;
	iv=BCAST_StringToStridx(var);
	return(BCAST_LookupAttrValIx(node, iv, rrn, rrv));
}

int BCAST_LookupAttrValCst(BCAST_Node *node,
	bcast_cxstate *rcst, char *var,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	int iv;

	iv=*rcst;
	if(!iv)
		{ iv=BCAST_StringToStridx(var); *rcst=iv; }
	return(BCAST_LookupAttrValIx(node, iv, rrn, rrv));
}

int BCAST_LookupAttrValIx(BCAST_Node *node, int iv,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	u16 *attr_n, *an;
	BCAST_AttrVal *attr_v, *av;
	int m, n, c;
	int na, ma, mask, mask1;
	int i, j, k;
	
	*rrn=NULL;
	*rrv=NULL;
	
//	if(node->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCAST_AttrVal *)(node->attr_v);
		na=node->nattr;
		ma=BCAST_NODE_FIXATTR;
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
//		BCAST_FetchAttrArrIx()
		m=0; n=na;
		while(m!=(i=(m+n)>>1))
		{
			BCAST_FetchAttrArrIx(node, i, &an, &av);
			j=(*an)&mask1;
			if((iv&mask1)>=j)
				{ m=i; }
			else
				{ n=i; }
		}

		if((i<0) || (i>=na))
			{ BGBCC_DBGBREAK }

		BCAST_FetchAttrArrIx(node, i, &an, &av);
		j=(*an)&mask;
		if(iv==j)
		{
			*rrn=an;
			*rrv=av;
			return(i);
		}

		BCAST_FetchAttrArrIx(node, i+1, &an, &av);
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

int BCAST_FetchAttrVal(BCAST_Node *node, char *var,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	int iv;
	iv=BCAST_StringToStridx(var);
	return(BCAST_FetchAttrValIx(node, iv, rrn, rrv));
}

int BCAST_FetchAttrValCst(BCAST_Node *node,
	bcast_cxstate *rcst, char *var,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	int iv;
	iv=*rcst;

	if(!iv)
		{ iv=BCAST_StringToStridx(var); *rcst=iv; }
	return(BCAST_FetchAttrValIx(node, iv, rrn, rrv));
}

int BCAST_FetchAttrValIx(BCAST_Node *node, int iv,
	u16 **rrn, BCAST_AttrVal **rrv)
{
	u16 *attr_n, *an, *an1;
	BCAST_AttrVal *attr_v, *av, *av1;
	int na, ma, mask, mask1;
	int i, j, k;

	*rrn=NULL;
	*rrv=NULL;
	
//	if(node->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCAST_AttrVal *)(node->attr_v);
		na=node->nattr;
		ma=BCAST_NODE_FIXATTR;
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

		BCAST_FetchAttrArrIx(node, ma, &an, &av);
		return(BCAST_FetchAttrValIx(node, iv, rrn, rrv));
	}else
	{
		for(i=0; i<na; i++)
		{
			BCAST_FetchAttrArrIx(node, i, &an, &av);
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
			BCAST_FetchAttrArrIx(node, i, &an, &av);
			BCAST_FetchAttrArrIx(node, i-1, &an1, &av1);
			if(((*an1)&mask1)<(iv&mask1))
				break;
			*an=*an1;
			*av=*av1;
			i--;
		}
		
		BCAST_FetchAttrArrIx(node, i, &an, &av);
		*an=iv;
		*rrn=an;
		*rrv=av;
		return(i);
	}

#if 0
	k=node->mattr;
	if(!k)
	{
		k=BCAST_NODE_FESCATTR;
		attr_n=bgbcc_malloc((k+1)*sizeof(u16));
		attr_v=bgbcc_malloc((k+1)*sizeof(u64));
		memcpy(attr_n, node->attr_n,
			BCAST_NODE_FIXATTR*sizeof(u16));
		memcpy(attr_v, node->attr_v,
			BCAST_NODE_FIXATTR*sizeof(u64));
		node->mattr=k;
		node->attr_v[0].p=attr_n;
		node->attr_v[1].p=attr_v;
		return(BCAST_FetchAttrValIx(node, iv, rrn, rrv));
	}
	
	k=k+(k>>1);
	attr_n=bgbcc_realloc(attr_n, (k+1)*sizeof(u16));
	attr_v=bgbcc_realloc(attr_v, (k+1)*sizeof(u64));
	node->attr_v[0].p=attr_n;
	node->attr_v[1].p=attr_v;
	node->mattr=k;	
	return(BCAST_FetchAttrValIx(node, iv, rrn, rrv));
#endif
}

char *BCAST_Get(BCAST_Node *n, char *var)
{
	bcast_cxstate iv;
	iv=0; return(BCAST_GetCst(n, &iv, var));
}

char *BCAST_GetCst(BCAST_Node *n,
	bcast_cxstate *rcst, char *var)
{
	u16 *an;
	BCAST_AttrVal *av;
	char *tb;
	int i;

	i=BCAST_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_STRING)
			{ return(av->s); }

		if((*an>>12)==BCAST_IVTY_INT)
		{
			tb=bgbcc_ralloc(64);
			sprintf(tb, "%lld", av->i);
			return(tb);
		}

		if((*an>>12)==BCAST_IVTY_REAL)
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
s64 BCAST_GetIntCst(BCAST_Node *n, bcast_cxstate *rcst, char *var)
{
	u16 *an;
	BCAST_AttrVal *av;
	char *tb;
	int i;

	i=BCAST_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCAST_IVTY_REAL)
			{ return(av->f); }
		if((*an>>12)==BCAST_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		return(-1);
	}
	return(0);
}

double BCAST_GetFloatCst(BCAST_Node *n, bcast_cxstate *rcst, char *var)
{
	u16 *an;
	BCAST_AttrVal *av;
	char *tb;
	int i;

	i=BCAST_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCAST_IVTY_REAL)
			{ return(av->f); }
		if((*an>>12)==BCAST_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		return(-1);
	}
	return(0);
}
#endif

#if 1
void BCAST_SetCst(BCAST_Node *n, bcast_cxstate *rcst, char *var, char *val)
{
	u16 *an;
	BCAST_AttrVal *av;
	int i;
//	if(n->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }
	i=BCAST_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCAST_IVTY_STRING<<12);
	av->s=bgbcc_strdup(val);
}

void BCAST_SetIntCst(BCAST_Node *n,
	bcast_cxstate *rcst, char *var, s64 val)
{
	u16 *an;
	BCAST_AttrVal *av;
	int i;
//	if(n->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }
	i=BCAST_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCAST_IVTY_INT<<12);
	av->i=val;
}

void BCAST_SetFloatCst(BCAST_Node *n,
	bcast_cxstate *rcst, char *var, double val)
{
	u16 *an;
	BCAST_AttrVal *av;
	int i;
//	if(n->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }
	i=BCAST_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCAST_IVTY_REAL<<12);
	av->f=val;
}
#endif

BCAST_Node *BCAST_GetNodeCst(BCAST_Node *n, bcast_cxstate *rcst, char *var)
{
	u16 *an;
	BCAST_AttrVal *av;
	char *tb;
	int i;

	i=BCAST_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_NODE)
			{ return(av->p); }
		return(NULL);
	}
	return(NULL);
}

void BCAST_SetNodeCst(BCAST_Node *n,
	bcast_cxstate *rcst, char *var, BCAST_Node *val)
{
	u16 *an;
	BCAST_AttrVal *av;
	int i;

	BCAST_MarkTreeZone(val, n->ztag);

	i=BCAST_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCAST_IVTY_NODE<<12);
	av->p=val;
}


BCAST_Node *BCAST_GetNodeIndex(BCAST_Node *n, int idx0)
{
	BCAST_NodeList *nl;
	BCAST_AttrVal *av;
	u16 *an;
	int i, j, ix, idx;

	if(idx0<0)
		return(NULL);

	idx=idx0-4;
	if(idx<0)
	{
//		ix=0x8000+idx0;
		ix=(BCAST_IVTY_NODE<<12)+(4095-idx0);
		i=BCAST_LookupAttrValIx(n, ix, &an, &av);
		if(i>=0)
		{
			return(av->p);
		}

		return(NULL);
	}

	nl=n->list;
	if(!nl)
		return(NULL);

	if((idx<0) || (idx>=nl->lisz))
		return(NULL);
	if(!nl->lilvl)
		return(nl->lidat[idx]);

	i=nl->lilvl;
	while(i)
	{
		j=(idx>>(i*4))&15;
		nl=nl->lidat[j];
		i=nl->lilvl;
	}
	return(nl->lidat[idx&15]);

#if 0
	u16 *an;
	BCAST_AttrVal *av;
	char *tb;
	int i, ix;

	if((idx<0) || (idx>=32768))
		return(NULL);
	ix=0x8000+idx;
	i=BCAST_LookupAttrValIx(n, ix, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_NODE)
			{ return(av->p); }
		return(NULL);
	}
	return(NULL);
#endif
}

void BCAST_SetNodeIndex(BCAST_Node *n,
	int idx0, BCAST_Node *val)
{
	BCAST_NodeList *nl, *nl1;
	BCAST_AttrVal *av;
	u16 *an;
	int i, j, k, ix, idx;

	if(idx0<0)
	{
		BGBCC_DBGBREAK
		return;
	}

	BCAST_MarkTreeZone(val, n->ztag);

	idx=idx0-4;
	if(idx<0)
	{
//		ix=0x8000+idx0;
		ix=(BCAST_IVTY_NODE<<12)+(4095-idx0);
		i=BCAST_FetchAttrValIx(n, ix, &an, &av);
		av->p=val;
		return;
	}

	nl=n->list;
	if(!nl)
	{
		nl=BCAST_AllocNodeList();
		n->list=nl;
	}
	
	while(idx>=(1<<(nl->lilvl*4)))
	{
		nl1=BCAST_AllocNodeList();
		nl1->lisz=nl->lisz;
		nl1->lilvl=nl->lilvl+1;
		nl1->lidat[0]=nl;
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
			nl1=BCAST_AllocNodeList();
			nl1->lilvl=i-1;
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
		n->list=bgbcc_malloc(k*sizeof(BCAST_Node *));
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
		n->list=bgbcc_realloc(n->list, k*sizeof(BCAST_Node *));
		n->limax=k;
	}
	
	if(idx>=n->lisz)
		n->lisz=idx+1;

	n->list[idx]=val;
#endif

#if 0
	u16 *an;
	BCAST_AttrVal *av;
	int i, ix;

	if((idx<0) || (idx>=32768))
	{
		BGBCC_DBGBREAK
		return;
	}
	
	ix=0x8000+idx;
	i=BCAST_FetchAttrValIx(n, ix, &an, &av);
//	*an=(*an&4095)|(BCAST_IVTY_NODE<<12);
	av->p=val;
#endif
}

BCAST_Node *BCAST_New(char *tag)
{
	BCAST_Node *n;
	n=BCAST_AllocNode();
	n->itag=BCAST_StringToStridx(tag);
	return(n);
}

BCAST_Node *BCAST_NewCst(bcast_cxstate *rcst, char *tag)
{
	BCAST_Node *n;
	int iv;
	iv=*rcst;
	if(!iv)
		{ iv=BCAST_StringToStridx(tag); *rcst=iv; }
	n=BCAST_AllocNode();
	n->itag=iv;
	return(n);
}

#if 1

bcast_cxstate	bcast_text;

bcast_cxstate	bcast_Stext;
bcast_cxstate	bcast_Slist;

BCAST_Node *BCAST_NewText(char *text)
{
	BCAST_Node *n;
	n=BCAST_NewCst(&bcast_Stext, "$text");
	BCAST_SetCst(n, &bcast_Stext, "$text", text);
	return(n);
}

BCAST_Node *BCAST_NewList(void)
{
	return(BCAST_NewCst(&bcast_Slist, "$list"));
}
#endif

int BCAST_GetNodeChildCount(BCAST_Node *node)
{
	int ix;
	ix=node->list->lisz;
	ix+=node->lifx;
	return(ix);
}

BCAST_Node *BCAST_AddEnd(BCAST_Node *node, BCAST_Node *child)
{
	if(!node)
	{
		node=BCAST_NewList();
		node->itag=(node->itag&4095)|(BCAST_NTY_TRANS<<12);
	}else
	{
		if((node->itag&4095)!=bcast_Slist)
		{
			BGBCC_DBGBREAK
		}
	}
	
	if(child)
	{
		BCAST_Add(node, child);
	}
	
	return(node);
}

void BCAST_Add(BCAST_Node *node, BCAST_Node *child)
{
	BCAST_Node *cur, *lst;
	int ix;

	if(!child)
		return;

	if(node->list)
	{
		ix=node->list->lisz++;
		ix+=node->lifx;
		BCAST_SetNodeIndex(node, ix, child);
	}else
	{
		ix=node->lifx++;
		BCAST_SetNodeIndex(node, ix, child);
	}
}

void BCAST_AddV(BCAST_Node *parent, BCAST_Node *child)
{
	BCAST_Node *ch1;
	u16 *an;
	BCAST_AttrVal *av;
	int i;

	if(!parent)
		return;
	if(!child)
		return;

//	if(parent->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }
//	if(child->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }

#if 1
	if((child->itag>>12)==BCAST_NTY_TRANS)
	{
		i=BCAST_FetchAttrValIx(parent, child->itag&4095, &an, &av);
		if((*an>>12)==BCAST_IVTY_DEFAULT)
		{
			*an=(*an&4095)|(BCAST_IVTY_NODE<<12);
			ch1=BCAST_GetNodeIndex(child, 0);
			av->p=ch1;

//			av->p=child->down;
//			if(child->down)
//				child->down->up=parent;
			return;
		}
	}
#endif

	BCAST_Add(parent, child);
}


BCAST_Node *BCAST_New1V(char *tag, BCAST_Node *a)
{
	BCAST_Node *n;
	n=BCAST_New(tag); BCAST_Add(n, a);
	n->itag=(n->itag&4095)|(BCAST_NTY_TRANS<<12);
	return(n);
}

BCAST_Node *BCAST_NewCst1V(bccx_cxstate *rcst, char *tag, BCAST_Node *a)
{
	BCAST_Node *n;
	n=BCAST_NewCst(rcst, tag); BCAST_Add(n, a);
	n->itag=(n->itag&4095)|(BCAST_NTY_TRANS<<12);
	return(n);
}

BCAST_Node *BCAST_New1(char *tag, BCAST_Node *a)
{
	BCAST_Node *n;
	n=BCAST_New(tag); BCAST_AddV(n, a);
	return(n);
}

BCAST_Node *BCAST_New2(char *tag, BCAST_Node *a, BCAST_Node *b)
{
	BCAST_Node *n;
	n=BCAST_New1(tag, a); BCAST_AddV(n, b);
	return(n);
}

BCAST_Node *BCAST_New3(char *tag, BCAST_Node *a,
	BCAST_Node *b, BCAST_Node *c)
{
	BCAST_Node *n;
	n=BCAST_New2(tag, a, b); BCAST_AddV(n, c);
	return(n);
}

BCAST_Node *BCAST_New4(char *tag, BCAST_Node *a,
	BCAST_Node *b, BCAST_Node *c, BCAST_Node *d)
{
	BCAST_Node *n;
	n=BCAST_New3(tag, a, b, c); BCAST_AddV(n, d);
	return(n);
}

BCAST_Node *BCAST_NewCst1(bccx_cxstate *rcst, char *tag, BCAST_Node *a)
{
	BCAST_Node *n;
	n=BCAST_NewCst(rcst, tag); BCAST_AddV(n, a);
	return(n);
}

BCAST_Node *BCAST_NewCst2(bccx_cxstate *rcst, char *tag,
	BCAST_Node *a, BCAST_Node *b)
{
	BCAST_Node *n;
	n=BCAST_NewCst1(rcst, tag, a); BCAST_AddV(n, b);
	return(n);
}

BCAST_Node *BCAST_NewCst3(bccx_cxstate *rcst, char *tag, BCAST_Node *a,
	BCAST_Node *b, BCAST_Node *c)
{
	BCAST_Node *n;
	n=BCAST_NewCst2(rcst, tag, a, b); BCAST_AddV(n, c);
	return(n);
}

BCAST_Node *BCAST_NewCst4(bccx_cxstate *rcst, char *tag, BCAST_Node *a,
	BCAST_Node *b, BCAST_Node *c, BCAST_Node *d)
{
	BCAST_Node *n;
	n=BCAST_NewCst3(rcst, tag, a, b, c); BCAST_AddV(n, d);
	return(n);
}

char *BCAST_Tag(BCAST_Node *n)
	{ return(BCAST_StridxToString(n->itag)); }

char *BCAST_Text(BCAST_Node *n)
	{ return(BCAST_GetCst(n, &bcast_Stext, "$text")); }

void BCAST_SetTagCst(BCAST_Node *n, bcast_cxstate *rcst, char *str)
{
	int iv;
	iv=*rcst;
	if(!iv)
		{ iv=BCAST_StringToStridx(str); *rcst=iv; }
	n->itag=iv;
}

int BCAST_TagIsCstP(BCAST_Node *n, bcast_cxstate *rcst, char *str)
{
	int ix;
	if(!n)return(0);
	if(!n->itag)return(0);
	
	ix=*rcst;
	if(!ix)
		{ ix=BCAST_StringToStridx(str); *rcst=ix; }
	return(n->itag==ix);
}

int BCAST_AttrIsCstP(BCAST_Node *n,
	bcast_cxstate *rcst, char *var, char *val)
{
	char *s;

	if(!n)return(0);
	if(!n->itag)return(0);

	s=BCAST_GetCst(n, rcst, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
	if(!strcmp(s, val))
//	if(!bcast_strcmp(s, val))
		return(1);
	return(0);
}

int BCAST_TagAttrIsP(BCAST_Node *n, char *tag, char *var, char *val)
{
	char *s;

	if(!n)return(0);
	if(!n->itag)return(0);

//	if(bcast_strcmp(BCAST_StridxToString(n->itag), tag))
	if(strcmp(BCAST_StridxToString(n->itag), tag))
		return(0);

	s=BCAST_Get(n, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
//	if(!bcast_strcmp(s, val))
	if(!strcmp(s, val))
		return(1);
	return(0);
}

// BCAST_Node *BCAST_FindTag(BCAST_Node *parent, char *tag)
//	{ return(BCAST_FindNextTag(parent->down, tag)); }
//BCAST_Node *BCAST_FindAttr(BCAST_Node *parent, char *var, char *val)
//	{ return(BCAST_FindNextAttr(parent->down, var, val)); }
//BCAST_Node *BCAST_FindTagAttr(BCAST_Node *parent,
//		char *tag, char *var, char *val)
//	{ return(BCAST_FindNextTagAttr(parent->down, tag, var, val)); }

BCAST_Node *BCAST_Child(BCAST_Node *parent)
{
	BCAST_Node *tmp;
	tmp=BCAST_GetNodeIndex(parent, 0);
	return(tmp);
}

BCAST_Node *BCAST_FindTagIx(BCAST_Node *parent, int iv)
{
	BCAST_Node *tmp;
	int i, j, k, na;
	
	na=BCAST_GetNodeChildCount(parent);
	for(i=0; i<na; i++)
	{
		tmp=BCAST_GetNodeIndex(parent, i);
		if(!tmp)
			continue;
		if((tmp->itag&4095)==(iv&4095))
			return(tmp);
	}
	
	return(NULL);

//	return(BCAST_FindNextTagIx(parent->down, iv));
}

BCAST_Node *BCAST_FindTagCst(BCAST_Node *parent, bcast_cxstate *rcst, char *tag)
{
	BCAST_Node *tmp;
	int iv;

	iv=*rcst;
	if(!iv)
		{ iv=BCAST_StringToStridx(tag); *rcst=iv; }
	return(BCAST_FindTagIx(parent, iv));

//	return(BCAST_FindNextTagIx(parent->down, iv));
}

BCAST_Node *BCAST_FindTag(BCAST_Node *parent, char *tag)
{
	BCAST_Node *tmp;
	int iv;

	iv=BCAST_StringToStridx(tag);
	return(BCAST_FindTagIx(parent, iv));

//	return(BCAST_FindNextTagIx(parent->down, iv));
}

BCAST_Node *BCAST_Fetch(BCAST_Node *parent, char *tag)
{
	u16 *an;
	BCAST_Node *tmp;
	BCAST_AttrVal *av;
	char *tb;
	int i;

//	if(parent->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }

	i=BCAST_LookupAttrVal(parent, tag, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_NODE)
			{ return(av->p); }
	}

	tmp=BCAST_FindTag(parent, tag);
	if(tmp)return(BCAST_Child(tmp));
	return(NULL);
}


BCAST_Node *BCAST_FetchCst(BCAST_Node *parent, bcast_cxstate *rcst, char *tag)
{
	u16 *an;
	BCAST_Node *tmp;
	BCAST_AttrVal *av;
	char *tb;
	int iv;
	int i;

	iv=*rcst;
	if(!iv)
		{ iv=BCAST_StringToStridx(tag); *rcst=iv; }

//	if(parent->magic!=BCAST_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }

//	i=BCAST_LookupAttrVal(parent, tag, &an, &av);
	i=BCAST_LookupAttrValIx(parent, iv, &an, &av);
	if(i>=0)
	{
		if((*an>>12)==BCAST_IVTY_NODE)
			{ return(av->p); }
	}

	tmp=BCAST_FindTagIx(parent, iv);
	if(tmp)return(BCAST_Child(tmp));
	return(NULL);
}

#if 0
BCAST_Node *BCAST_Clone(BCAST_Node *n)
{
	BCAST_Node *t, *t1, *c, *l;
//	BCAST_Attr *ac, *at, *al;

	if(!n)return(NULL);

	t=BCAST_AllocNode();

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

int BCAST_MarkNodeListZone(BCAST_NodeList *nl, int zone)
{
	int i, j, k;

	if(!nl)
		return(0);

	if(nl->lilvl)
	{
		for(i=0; i<16; i++)
		{
			BCAST_MarkNodeListZone(nl->lidat[i], zone);
		}
		return(1);
	}

	for(i=0; i<16; i++)
	{
		BCAST_MarkTreeZone(nl->lidat[i], zone);
	}

	return(1);
}

int BCAST_MarkAttrListZone(BCAST_Node *n, int zone)
{
	BCAST_Node *tn;
	BCAST_AttrVal *av;
	u16 *an;
	int i, j, k;

	if(!n)
		return(0);

	n->ztag=zone;

	an=n->attr_n;
	av=(BCAST_AttrVal *)(n->attr_v);

	if(!n->malvl)
	{
		for(i=0; i<BCAST_NODE_FIXATTR; i++)
		{
			if((an[i]>>12)==BCAST_IVTY_NODE)
			{
				tn=av[i].p;
				BCAST_MarkTreeZone(tn, zone);
			}
		}
	}else
	{
		for(i=0; i<BCAST_NODE_FIXATTR; i++)
		{
			if((an[i]>>12)==BCAST_IVTY_NODE)
			{
				tn=av[i].p;
				BCAST_MarkAttrListZone(tn, zone);
			}
		}
	}
	return(1);
}

int BCAST_MarkTreeZone(BCAST_Node *n, int zone)
{
	BCAST_Node *tn;
	BCAST_NodeList *nl;

	int i, j, k;

	if(!n)
		return(0);

	if(n->ztag<=zone)
		return(0);

	n->ztag=zone;

	BCAST_MarkAttrListZone(n, zone);
	BCAST_MarkNodeListZone(n->list, zone);
	return(1);
}
