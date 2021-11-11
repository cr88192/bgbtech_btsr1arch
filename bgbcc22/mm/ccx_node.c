#include <bgbccc.h>

// BCCX_Attr *bccx_free_attr=NULL;
BCCX_Node *bccx_free_node=NULL;

char *bccx_stridx_strs[4096];
short bccx_stridx_next[4096];
short bccx_stridx_hash[256];
int bccx_stridx_num;

int bccx_init=0;

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

int bccx_memset(void *buf, int val, int sz)
{
	byte *t, *te;
	u32 v;
	
	t=buf; te=t+sz;
	v=(byte)val; v=v|(v<<8); v=v|(v<<16);
#if 1
	while((t+16)<=te)
	{	((u32 *)t)[0]=v; ((u32 *)t)[1]=v;
		((u32 *)t)[2]=v; ((u32 *)t)[3]=v;
		t+=16;	}
#endif
	while((t+4)<=te)
		{ *((u32 *)t)=v; t+=4; }
	while(t<te)
		*t++=val;
	return(0);
}


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
//		if(!strcmp(bccx_stridx_strs[i], str))
		if(!bccx_strcmp(bccx_stridx_strs[i], str))
			return(i);
		i=bccx_stridx_next[i];
	}
	
	i=bccx_stridx_num++;
	bccx_stridx_strs[i]=bgbcc_strdup(str);
	bccx_stridx_next[i]=bccx_stridx_hash[h];
	bccx_stridx_hash[h]=i;
	return(i);
}

char *BCCX_StridxToString(int idx)
{
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

BCCX_Node *BCCX_AllocNode(void)
{
	BCCX_Node *cur, *tblk;
	int i;

	if(bccx_free_node)
	{
		cur=bccx_free_node;
		bccx_free_node=cur->next;
//		cur->hnext=NULL;
		cur->next=NULL;
		cur->magic=BCCX_NODE_MAGIC;
//		bccx_memset(cur, 0, sizeof(BCCX_Node));
		return(cur);
	}

	BCCX_Init();
	tblk=bgbcc_tmalloc("_bccx_node_t", 256*sizeof(BCCX_Node));
	memset(tblk, 0, 256*sizeof(BCCX_Node));
	for(i=0; i<256; i++)
	{
		cur=tblk+i;
		cur->next=bccx_free_node;
		bccx_free_node=cur;
	}

	cur=bccx_free_node;
	bccx_free_node=cur->next;
//	cur->hnext=NULL;
	cur->next=NULL;
	cur->magic=BCCX_NODE_MAGIC;

//	bccx_memset(cur, 0, sizeof(BCCX_Node));
//	cur=bgbcc_tmalloc("_bccx_node_t", sizeof(BCCX_Node));
	return(cur);
}

#if 0
// BCCX_Attr *BCCX_NewAttr(void)
{
	BCCX_Attr *cur, *tblk;
	int i;

	if(bccx_free_attr)
	{
		cur=bccx_free_attr;
		bccx_free_attr=cur->next;
		bccx_memset(cur, 0, sizeof(BCCX_Attr));
		return(cur);
	}

	BCCX_Init();
	tblk=bgbcc_tmalloc("_bccx_attr_t", 256*sizeof(BCCX_Attr));
	for(i=0; i<256; i++)
	{
		cur=tblk+i;
		cur->next=bccx_free_attr;
		bccx_free_attr=cur;
	}

//	cur=bgbcc_tmalloc("_bccx_attr_t", sizeof(BCCX_Attr));
	cur=bccx_free_attr;
	bccx_free_attr=cur->next;
	bccx_memset(cur, 0, sizeof(BCCX_Attr));
	return(cur);
}
#endif

void BCCX_FreeNode(BCCX_Node *n)
{
	if(!n)return;

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	
	bccx_memset(n, 0, sizeof(BCCX_Node));

//	if(n->tag==BGBCC_UNDEFINED)
//		{ *(int *)-1=-1; }

//	n->tag=BGBCC_UNDEFINED;
	n->next=bccx_free_node;
	bccx_free_node=n;
}

#if 0
// void BCCX_FreeAttr(BCCX_Attr *a)
{
	if(!a)return;

//	if(a->var==BGBCC_UNDEFINED)
//		{ *(int *)-1=-1; }
//	a->var=BGBCC_UNDEFINED;
	a->next=bccx_free_attr;
	bccx_free_attr=a;
}

// BCCX_Attr *BCCX_LookupAttr(BCCX_Node *n, char *var)
{
	BCCX_Attr *cur;
	int iv;
	
	iv=BCCX_StringToStridx(var);

	cur=n->attr;
	while(cur)
	{
		if((cur->ivar&4095)==iv)
			return(cur);
//		if(!strcmp(cur->var, var))
//			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

// BCCX_Attr *BCCX_FetchAttr(BCCX_Node *n, char *var)
{
	BCCX_Attr *cur, *lst;
	int iv;
	
	iv=BCCX_StringToStridx(var);

	cur=n->attr; lst=NULL;
	while(cur)
	{
		if((cur->ivar&4095)==iv)
			return(cur);
		lst=cur; cur=cur->next;
	}

	cur=BCCX_NewAttr();
	cur->ivar=iv;
	if(lst)lst->next=cur; else n->attr=cur;

	return(cur);
}
#endif

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
//	else
//		{ if(bccx_strcmp(BCCX_StridxToString(iv), var))
//			 { BGBCC_DBGBREAK } }

	return(BCCX_LookupAttrValIx(node, iv, rrn, rrv));
}

int BCCX_LookupAttrValIx(BCCX_Node *node, int iv,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	u16 *attr_n;
	BCCX_AttrVal *attr_v;
	int m, n, c;
	int na, ma;
	int i, j, k;
	
	*rrn=NULL;
	*rrv=NULL;
	
	if(node->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	
	if(!node->mattr)
	{
		attr_n=node->attr_n;
		attr_v=node->attr_v;
		na=node->nattr;
		ma=BCCX_NODE_FIXATTR;
	}else
	{
		attr_n=node->attr_v[0].p;
		attr_v=node->attr_v[1].p;
		na=node->nattr;
		ma=node->mattr;
	}

	if(na>ma)
		{ BGBCC_DBGBREAK }

	if(node->nattr<5)
	{
		for(i=0; i<node->nattr; i++)
		{
			j=attr_n[i];
			if((j&4095)==iv)
			{
				*rrn=attr_n+i;
				*rrv=attr_v+i;
				return(i);
			}
		}
	}else
	{
//		m=0; n=node->nattr;
		m=0; n=na;
//		while(m!=n)
//		while((m+1)<n)
		while(m!=(i=(m+n)>>1))
		{
//			i=(m+n)>>1;
			j=attr_n[i]&4095;
			if(iv>=j)
				{ m=i; }
//				{ if(m==i)break; m=i; }
			else
				{ n=i; }
		}

		if((i<0) || (i>=na))
			{ BGBCC_DBGBREAK }

		j=attr_n[i]&4095;
		if(iv==j)
		{
			*rrn=attr_n+i;
			*rrv=attr_v+i;
			return(i);
		}

		j=attr_n[i+1]&4095;
		if(iv==j)
		{
			i++;
			*rrn=attr_n+i;
			*rrv=attr_v+i;
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
//	else
//		{ if(bccx_strcmp(BCCX_StridxToString(iv), var))
//			 { BGBCC_DBGBREAK } }

	return(BCCX_FetchAttrValIx(node, iv, rrn, rrv));
}

int BCCX_FetchAttrValIx(BCCX_Node *node, int iv,
	u16 **rrn, BCCX_AttrVal **rrv)
{
	u16 *attr_n;
	BCCX_AttrVal *attr_v;
	int na, ma;
	int i, j, k;

	*rrn=NULL;
	*rrv=NULL;
	
	if(node->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	
	if(!node->mattr)
	{
		attr_n=node->attr_n;
		attr_v=node->attr_v;
		na=node->nattr;
		ma=BCCX_NODE_FIXATTR;
	}else
	{
		attr_n=node->attr_v[0].p;
		attr_v=node->attr_v[1].p;
		na=node->nattr;
		ma=node->mattr;
	}

	if(na>ma)
		{ BGBCC_DBGBREAK }

//	iv=BCCX_StringToStridx(var);
	
	for(i=0; i<na; i++)
	{
		j=attr_n[i];
		if((j&4095)==iv)
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
		while((i>0) && (attr_n[i-1]&4095)>iv)
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

//	i=BCCX_LookupAttrVal(n, var, &an, &av);
	i=BCCX_LookupAttrValCst(n, rcst, var, &an, &av);
	if(i>=0)
	{
//		if(!(*an>>12))
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(av->s); }

		if((*an>>12)==BCCX_IVTY_INT)
		{
			tb=bgbcc_ralloc(64);
			sprintf(tb, "%lld", av->i);
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

char *BCCX_Get_SI(BCCX_Node *n, char *var)
{
	return(BCCX_Get(n, var));
}

#if 0
char *BCCX_Get_SI(BCCX_Node *n, char *var)
{
	BCCX_Attr *cur;

	cur=BCCX_LookupAttr(n, var);
	if(cur)return(cur->val);

//	cur=n->attr;
//	while(cur)
//	{
//		if((cur->var==var))
//			return(cur->val);
//		cur=cur->next;
//	}

	return(NULL);
}
#endif

s64 BCCX_GetInt(BCCX_Node *n, char *var)
{
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	i=BCCX_LookupAttrVal(n, var, &an, &av);
	if(i>=0)
	{
//		if(!(*an>>12))
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		if((*an>>12)==BCCX_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCCX_IVTY_REAL)
			{ return(av->f); }
		return(-1);
	}
	return(0);
}

double BCCX_GetFloat(BCCX_Node *n, char *var)
{
	u16 *an;
	BCCX_AttrVal *av;
	char *tb;
	int i;

	i=BCCX_LookupAttrVal(n, var, &an, &av);
	if(i>=0)
	{
//		if(!(*an>>12))
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		if((*an>>12)==BCCX_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCCX_IVTY_REAL)
			{ return(av->f); }
		return(-1);
	}
	return(0);
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
//		if(!(*an>>12))
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		if((*an>>12)==BCCX_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCCX_IVTY_REAL)
			{ return(av->f); }
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
//		if(!(*an>>12))
		if((*an>>12)==BCCX_IVTY_STRING)
			{ return(BGBCC_ParseNumber(av->s)); }
		if((*an>>12)==BCCX_IVTY_INT)
			{ return(av->i); }
		if((*an>>12)==BCCX_IVTY_REAL)
			{ return(av->f); }
		return(-1);
	}
	return(0);
}
#endif

void BCCX_Set(BCCX_Node *n, char *var, char *val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	i=BCCX_FetchAttrVal(n, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_STRING<<12);
	av->s=bgbcc_strdup(val);
}

void BCCX_SetInt(BCCX_Node *n, char *var, s64 val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	i=BCCX_FetchAttrVal(n, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_INT<<12);
	av->i=val;
}

void BCCX_SetFloat(BCCX_Node *n, char *var, double val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	i=BCCX_FetchAttrVal(n, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_REAL<<12);
	av->f=val;
}

#if 1
void BCCX_SetCst(BCCX_Node *n, bccx_cxstate *rcst, char *var, char *val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_STRING<<12);
	av->s=bgbcc_strdup(val);
}

void BCCX_SetIntCst(BCCX_Node *n, bccx_cxstate *rcst, char *var, s64 val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_INT<<12);
	av->i=val;
}

void BCCX_SetFloatCst(BCCX_Node *n, bccx_cxstate *rcst, char *var, double val)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	i=BCCX_FetchAttrValCst(n, rcst, var, &an, &av);
	*an=(*an&4095)|(BCCX_IVTY_REAL<<12);
	av->f=val;
}
#endif

BCCX_Node *BCCX_New(char *tag)
{
	BCCX_Node *n;
	n=BCCX_AllocNode();
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
//	else
//		{ if(strcmp(BCCX_StridxToString(iv), tag))
//			 { BGBCC_DBGBREAK } }
	n=BCCX_AllocNode();
	n->itag=iv;
	return(n);
}

#if 0
BCCX_Node *BCCX_NewText(char *text)
{
	BCCX_Node *n;
	n=BCCX_AllocNode();
	n->text=bgbcc_strdup(text);
	return(n);
}
#endif

#if 1
BCCX_Node *BCCX_NewText(char *text)
{
	BCCX_Node *n;
	n=BCCX_AllocNode();
//	n->text=bgbcc_strdup(text);
	BCCX_Set(n, "$text", text);
	return(n);
}
#endif

#if 1
BCCX_Node *BCCX_NewCData(char *text)
{
	BCCX_Node *n;
	n=BCCX_AllocNode();
	n->itag=BCCX_StringToStridx("!CDATA");
	BCCX_Set(n, "$text", text);
	return(n);
}
#endif

void BCCX_AddV(BCCX_Node *parent, BCCX_Node *child)
{
	u16 *an;
	BCCX_AttrVal *av;
	int i;

	if(!child)return;

	if(parent->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	if(child->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

#if 1
//	if(!child->nattr && child->down && !child->down->next)
	if((child->itag>>12)==BCCX_NTY_TRANS)
	{
//		i=BCCX_FetchAttrVal(parent, BCCX_Tag(child), &an, &av);
//		i=BCCX_FetchAttrValIx(parent, child->itag, &an, &av);
		i=BCCX_FetchAttrValIx(parent, child->itag&4095, &an, &av);
//		if(((*an>>12)==BCCX_IVTY_STRING) && !(av->s))
//		if(((*an>>12)==BCCX_IVTY_DEFAULT) && !(av->s))
		if((*an>>12)==BCCX_IVTY_DEFAULT)
		{
			*an=(*an&4095)|(BCCX_IVTY_NODE<<12);
			av->p=child->down;
			if(child->down)
				child->down->up=parent;
//			BCCX_FreeNode(child);
			return;
		}
	}
#endif

	BCCX_Add(parent, child);
}

int BCCX_Add(BCCX_Node *parent, BCCX_Node *child)
{
	BCCX_Node *cur, *lst;

	if(!child)
		return(-1);

	if(parent->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	if(child->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	if(child->prev || child->up)
	{
		child=BCCX_CloneList(child);
	}

	if(parent->down)
	{
		cur=parent->down_end;
		cur->next=child;
		child->prev=cur;
		
	}else
	{
		parent->down=child;
	}

	cur=child; lst=NULL;
	while(cur)
	{
		cur->up=parent;
		lst=cur; cur=cur->next;
	}
	parent->down_end=lst;
	return(0);
}

BCCX_Node *BCCX_AddEnd(BCCX_Node *lst, BCCX_Node *n)
{
	BCCX_Node *cur;

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	if(n->prev || n->up)
	{
		n=BCCX_CloneList(n);
	}

	if(!lst)return(n);

	if(lst->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	if(lst->up)
	{
		BCCX_Add(lst->up, n);
		return(lst);
	}

	cur=lst;
	while(cur->next)cur=cur->next;
	cur->next=n;
	cur->next->prev=cur;

	return(lst);
}

BCCX_Node *BCCX_AddEnd2(BCCX_Node *fst, BCCX_Node **rlst, BCCX_Node *n)
{
	BCCX_Node *c, *t;

	if(!rlst)return(BCCX_AddEnd(fst, n));

	if(!fst)
	{
		c=n;
		while(c && c->next)
			c=c->next;
		*rlst=c;
		return(n);
	}

	if(fst->up || !(*rlst))
	{
		t=BCCX_AddEnd(fst, n);
		c=t; while(c && c->next)c=c->next;
		*rlst=c;
		return(t);
	}

	if(!n)return(fst);

	c=*rlst;
	n->prev=c; c->next=n;

	while(c && c->next)c=c->next;
	*rlst=c;
	return(fst);
}

BCCX_Node *BCCX_New1V(char *tag, BCCX_Node *a)
{
	BCCX_Node *n;
	n=BCCX_New(tag); BCCX_Add(n, a);
	n->itag=(n->itag&4095)|(BCCX_NTY_TRANS<<12);
	return(n);
}

BCCX_Node *BCCX_NewCst1V(bccx_cxstate *rcst, char *tag, BCCX_Node *a)
{
	BCCX_Node *n;
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

#if 0
int BCCX_NodeP(BCCX_Node *n)
{
	if(n->text)return(0);
	return(1);
}

int BCCX_TextP(BCCX_Node *n)
{
	if(n->text)return(1);
	return(0);
}
#endif

int BCCX_LeafP(BCCX_Node *n)
{
//	if(n->text)return(0);
	if(n->down)return(0);
	return(1);
}

int BCCX_ChildP(BCCX_Node *n)
{
//	if(n->text)return(0);
	if(n->down)return(1);
	return(0);
}

BCCX_Node *BCCX_Next(BCCX_Node *n)	{ return(n->next); }
BCCX_Node *BCCX_Prev(BCCX_Node *n)	{ return(n->prev); }
BCCX_Node *BCCX_Parent(BCCX_Node *n)	{ return(n->up); }
BCCX_Node *BCCX_Child(BCCX_Node *n)	{ return(n->down); }

// char *BCCX_Tag(BCCX_Node *n)	{ return(n->tag); }
char *BCCX_Tag(BCCX_Node *n)
	{ return(BCCX_StridxToString(n->itag)); }

// char *BCCX_Text(BCCX_Node *n)	{ return(n->text); }

char *BCCX_Text(BCCX_Node *n)
	{ return(BCCX_Get(n, "$text")); }

void BCCX_SetTag(BCCX_Node *n, char *s)
{
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
//	n->tag=bgbcc_strdup(s);
	n->itag=BCCX_StringToStridx(s);
}

void BCCX_SetTagCst(BCCX_Node *n, bccx_cxstate *rcst, char *str)
{
	int iv;
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(str); *rcst=iv; }
//	else
//		{ if(strcmp(BCCX_StridxToString(iv), str))
//			 { BGBCC_DBGBREAK } }
	n->itag=iv;
}

int BCCX_TagIsP(BCCX_Node *n, char *str)
{
	if(!n)return(0);
//	if(!n->tag)return(0);
	if(!n->itag)return(0);
//	if(!strcmp(n->tag, str))
	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }
	if(!bccx_strcmp(BCCX_StridxToString(n->itag), str))
		return(1);
	return(0);
}

int BCCX_TagIsCstP(BCCX_Node *n, bccx_cxstate *rcst, char *str)
{
	int ix;
	if(!n)return(0);
	if(!n->itag)return(0);
	
	ix=*rcst;
	if(!ix)
		{ ix=BCCX_StringToStridx(str); *rcst=ix; }
//	else
//		{ if(bccx_strcmp(BCCX_StridxToString(ix), str))
//			 { BGBCC_DBGBREAK } }

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	return(n->itag==ix);
	
//	if(!strcmp(BCCX_StridxToString(n->itag), str))
//		return(1);
	return(0);
}

int BCCX_AttrIsP(BCCX_Node *n, char *var, char *val)
{
	char *s;

	if(!n)return(0);
//	if(!n->tag)return(0);
	if(!n->itag)return(0);

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	s=BCCX_Get(n, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
	if(!bccx_strcmp(s, val))
		return(1);
	return(0);
}

int BCCX_AttrIsCstP(BCCX_Node *n,
	bccx_cxstate *rcst, char *var, char *val)
{
	char *s;

	if(!n)return(0);
//	if(!n->tag)return(0);
	if(!n->itag)return(0);

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	s=BCCX_GetCst(n, rcst, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
	if(!bccx_strcmp(s, val))
		return(1);
	return(0);
}

int BCCX_TagAttrIsP(BCCX_Node *n, char *tag, char *var, char *val)
{
	char *s;

	if(!n)return(0);
//	if(!n->tag)return(0);
//	if(strcmp(n->tag, tag))
//		return(0);
	if(!n->itag)return(0);

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	if(bccx_strcmp(BCCX_StridxToString(n->itag), tag))
		return(0);

	s=BCCX_Get(n, var);
	if(!s || !*s)
	{
		if(!val)return(1);
		if(!*val)return(1);
		return(0);
	}
	if(!val || !*val)return(0);
	if(!bccx_strcmp(s, val))
		return(1);
	return(0);
}


BCCX_Node *BCCX_FindNextTag(BCCX_Node *last, char *tag)
{
	BCCX_Node *cur;
	int iv;

	iv=BCCX_StringToStridx(tag);

	cur=last;
	while(cur)
	{
//		if(!strcmp(cur->tag, tag))
//			return(cur);
		if((cur->itag&4095)==iv)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BCCX_FindNextTagIx(BCCX_Node *last, int iv)
{
	BCCX_Node *cur;

	cur=last;
	while(cur)
	{
		if((cur->itag&4095)==iv)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BCCX_FindNextAttr(BCCX_Node *last, char *var, char *val)
{
	BCCX_Node *cur;

	cur=last;
	while(cur)
	{
		if(BCCX_AttrIsP(cur, var, val))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BCCX_FindNextTagAttr(BCCX_Node *last,
	char *tag, char *var, char *val)
{
	BCCX_Node *cur;
	int iv;

	iv=BCCX_StringToStridx(tag);

	cur=last;
	while(cur)
	{
//		if(!strcmp(cur->tag, tag))
		if((cur->itag&4095)==iv)
		{
			if(BCCX_AttrIsP(cur, var, val))
				return(cur);
		}
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BCCX_FindTag(BCCX_Node *parent, char *tag)
	{ return(BCCX_FindNextTag(parent->down, tag)); }
BCCX_Node *BCCX_FindAttr(BCCX_Node *parent, char *var, char *val)
	{ return(BCCX_FindNextAttr(parent->down, var, val)); }
BCCX_Node *BCCX_FindTagAttr(BCCX_Node *parent,
		char *tag, char *var, char *val)
	{ return(BCCX_FindNextTagAttr(parent->down, tag, var, val)); }

BCCX_Node *BCCX_FindTagIx(BCCX_Node *parent, int iv)
	{ return(BCCX_FindNextTagIx(parent->down, iv)); }

BCCX_Node *BCCX_FindTagCst(BCCX_Node *parent, bccx_cxstate *rcst, char *tag)
{
	BCCX_Node *tmp;
	int iv;

	iv=*rcst;
	if(!iv)
		{ iv=BCCX_StringToStridx(tag); *rcst=iv; }
	return(BCCX_FindNextTagIx(parent->down, iv));
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

//	if(parent->magic!=BCCX_NODE_MAGIC)
//		{ BGBCC_DBGBREAK }

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

BCCX_Node *BCCX_CloneS(BCCX_Node *n)
{
	return(BCCX_Clone(n));
}

BCCX_Node *BCCX_Clone(BCCX_Node *n)
{
	BCCX_Node *t, *t1, *c, *l;
//	BCCX_Attr *ac, *at, *al;

	if(!n)return(NULL);

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	t=BCCX_AllocNode();

//	t->tag=n->tag;
	t->itag=n->itag;
//	t->text=n->text;

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

#if 0
	ac=n->attr; al=NULL;
	while(ac)
	{
//		BCCX_SetNS(t, ac->ns, ac->var, ac->val);

		at=BCCX_NewAttr();
//		at->ns=ac->ns;
//		at->var=ac->var;
		at->ivar=ac->ivar;
		at->val=ac->val;
		if(al)al->next=at; else t->attr=at;
		al=at;

		ac=ac->next;
	}
#endif

	c=n->down; l=NULL;
	while(c)
	{
		t1=BCCX_Clone(c);
//		BCCX_Add(t, t1);

		if(l)
		{
			t1->up=t;
			l->next=t1;
			t1->prev=l;
			l=t1;
		}else
		{
			t1->up=t;
			t->down=t1;
			l=t1;
		}

		c=c->next;
	}
	t->down_end=l;

	return(t);	
}

BCCX_Node *BCCX_CloneList(BCCX_Node *n)
{
	BCCX_Node *t, *c, *ls, *le;

	if(!n)return(NULL);

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	c=n; ls=NULL; le=NULL;
	while(c)
	{
		t=BCCX_Clone(c);
//		l=BCCX_AddEnd(l, t);

		if(le)
		{
			t->prev=le;
			le->next=t;
			le=t;
		}else
		{
			ls=t; le=t;
		}

		c=c->next;
	}

	return(ls);
}

void BCCX_Unlink(BCCX_Node *n)
{
	BCCX_Node *nu, *np, *nn;

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	nu=n->up;
	np=n->prev;
	nn=n->next;

	if(!nu)return;
	if(nu->down==n)
	{
		nu->down=nn;
		if(nn)nn->prev=NULL;
	}else
	{
		if(np)np->next=nn;
		if(nn)nn->prev=np;
	}

	n->up=NULL;
	n->next=NULL;
	n->prev=NULL;
}

void BCCX_DeleteTree(BCCX_Node *n)
{
	BCCX_Node *cur, *nxt;
//	BCCX_Attr *acur, *anxt;

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	cur=n->down;
	while(cur)
	{
		nxt=cur->next;
		if(cur->up!=n)
			{ cur=nxt; continue; }
		
		BCCX_DeleteTree(cur);
		cur=nxt;
	}

#if 0
	acur=n->attr;
	while(acur)
	{
		anxt=acur->next;
		BCCX_FreeAttr(acur);
		acur=anxt;
	}
#endif

	if(n->mattr)
	{
		bgbcc_free(n->attr_v[0].p);
		bgbcc_free(n->attr_v[1].p);
	}

	BCCX_FreeNode(n);
}

void BCCX_CheckDeleteUnlinked(BCCX_Node *n)
{
//	return;

	if(!n)return;

	if(n->magic!=BCCX_NODE_MAGIC)
		{ BGBCC_DBGBREAK }

	if(!n->up)
	{
		BCCX_DeleteTree(n);
		return;
	}
}

int BCCX_GetNodeChildCount(BCCX_Node *node)
{
	BCCX_Node *n1;
	int i;
	
	if(!node)
		return(0);
	
	n1=BCCX_Child(node);
	i=0;
	while(n1)
		{ n1=BCCX_Next(n1); i++; }
	return(i);
}

BCCX_Node *BCCX_GetNodeIndex(BCCX_Node *n, int idx)
{
	BCCX_Node *n1;
	int i;
	
	if(!n)
		return(NULL);
	
	n1=BCCX_Child(n);
	i=idx;
	while(n1 && (i>0))
	{
		n1=BCCX_Next(n1);
		i--;
	}
	return(n1);
}
