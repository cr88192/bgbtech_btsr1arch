#include <bgbccc.h>

BGBDYC_MetaNode *dyll_metapath_root=NULL;

char *dyll_metapath_hash_name[4096];
BGBDYC_MetaNode *dyll_metapath_hash_node[4096];

char *dyll_metapath_hash2_name[4096];
BGBDYC_MetaNode *dyll_metapath_hash2_snode[4096];
BGBDYC_MetaNode *dyll_metapath_hash2_dnode[4096];

BGBDYC_MetaNode *DYLL_MetaPath_LookupNodeR(BGBDYC_MetaNode *node, char *path)
{
	char tb[256];
	BGBDYC_MetaNode *cur;
	char *s, *t, *s1;
	int i;

	s=path; t=tb;
	while(*s=='/')s++;
	if(!*s)return(node);

	while(*s && (*s!='/'))*t++=*s++;
	*t++=0;

	s1=tb; i=0;
	while(*s1)i=(i*251)+(*s1++);
	i&=4095;

	if((dyll_metapath_hash2_snode[i]==node) &&
		dyll_metapath_hash2_name[i] &&
		!strcmp(dyll_metapath_hash2_name[i], tb))
	{
		cur=dyll_metapath_hash2_dnode[i];
		if(!cur)return(NULL);
		return(DYLL_MetaPath_LookupNodeR(cur, s));
	}

	t=bgbcc_strdup(tb);
	cur=node->first;
	while(cur)
	{
		if(cur->name==t)
			break;
		cur=cur->next;
	}

	dyll_metapath_hash2_name[i]=t;
	dyll_metapath_hash2_snode[i]=node;
	dyll_metapath_hash2_dnode[i]=cur;

	if(!cur)return(NULL);
	return(DYLL_MetaPath_LookupNodeR(cur, s));
}

#if 0
BGBDYC_MetaNode *DYLL_MetaPath_GetNodeR(BGBDYC_MetaNode *node, char *path)
{
	char tb[256];
	BGBDYC_MetaNode *cur, *prv, *tmp;
	char *s, *t, *s1;
	int i;

	s=path; t=tb;
	while(*s=='/')s++;
	if(!*s)return(node);

	while(*s && (*s!='/'))*t++=*s++;
	*t++=0;

	s1=tb; i=0;
	while(*s1)i=(i*251)+(*s1++);
	i&=4095;

	if((dyll_metapath_hash2_snode[i]==node) &&
		dyll_metapath_hash2_name[i] &&
		!strcmp(dyll_metapath_hash2_name[i], tb))
	{
		cur=dyll_metapath_hash2_dnode[i];
		if(cur)return(DYLL_MetaPath_GetNodeR(cur, s));
	}

	t=bgbcc_strdup(tb);
	cur=node->first;
	while(cur)
	{
		if(cur->name==t)
			break;
		cur=cur->next;
	}

	if(!cur)
	{
		tmp=bgbcc_tmalloc2("_dyc_metanode_t", sizeof(BGBDYC_MetaNode));
		tmp->name=t;

		if(node->first)
		{
			cur=node->first; prv=NULL;
			while(cur)
			{
				if(strcmp(cur->name, t)>=0)
					break;
				prv=cur; cur=cur->next;
			}
			if(prv)
			{
				tmp->next=cur;
				prv->next=tmp;
			}else
			{
				tmp->next=cur;
				node->first=tmp;
			}
		}else
		{
			tmp->next=node->first;
			node->first=tmp;
		}

		cur=tmp;
	}

	dyll_metapath_hash2_name[i]=t;
	dyll_metapath_hash2_snode[i]=node;
	dyll_metapath_hash2_dnode[i]=cur;

	return(DYLL_MetaPath_GetNodeR(cur, s));
}
#endif

#if 1
int DYLL_MetaPath_AdjustBiDepth(BGBDYC_MetaBiNode *node)
{
	int i, j;

	i=node->lnode?node->lnode->depth:-1;
	j=node->rnode?node->rnode->depth:-1;
	node->depth=((i>j)?i:j)+1;
	return(node->depth);
}

int DYLL_MetaPath_BalanceBiNode(BGBDYC_MetaBiNode *node)
{
	BGBDYC_MetaNode *tmp;
	BGBDYC_MetaBiNode *btmp;
	int i, j;

	i=node->lnode?node->lnode->depth:-1;
	j=node->rnode?node->rnode->depth:-1;
	node->depth=((i>j)?i:j)+1;

	if(i>(j+1))
	{
		btmp=node->lnode;
		tmp=node->mid;
		node->lnode=btmp->lnode;
		node->mid=btmp->mid;
		btmp->lnode=btmp->rnode;
		btmp->rnode=node->rnode;
		btmp->mid=tmp;
		node->rnode=btmp;

		DYLL_MetaPath_AdjustBiDepth(btmp);
		DYLL_MetaPath_AdjustBiDepth(node);
	}

	if(j>(i+1))
	{
		btmp=node->rnode;
		tmp=node->mid;
		node->rnode=btmp->rnode;
		node->mid=btmp->mid;
		btmp->rnode=btmp->lnode;
		btmp->lnode=node->lnode;
		btmp->mid=tmp;
		node->lnode=btmp;

		DYLL_MetaPath_AdjustBiDepth(btmp);
		DYLL_MetaPath_AdjustBiDepth(node);
	}
	return(0);
}

BGBDYC_MetaNode *DYLL_MetaPath_GetNodeR2(BGBDYC_MetaBiNode *node, char *name)
{
	BGBDYC_MetaNode *tmp, *tmp1, *mid;
	BGBDYC_MetaBiNode *btmp, *btmp1;
	int i, j;

	mid=node->mid;

	i=strcmp(name, mid->name);
	if(i<0)
	{
		if(!node->lnode)
		{
			tmp=bgbcc_tmalloc2("_dyc_metanode_t",
				sizeof(BGBDYC_MetaNode));
			tmp->name=name;
			tmp->owner=mid->owner;

			if(mid->prev)
			{
				mid->prev->next=tmp;
				tmp->next=mid;
				tmp->prev=mid->prev;
				mid->prev=tmp;
			}else
			{
				tmp->next=mid;
				mid->prev=tmp;
				mid->owner->first=tmp;
			}

			btmp=bgbcc_tmalloc2("_dyc_metabinode_t",
				sizeof(BGBDYC_MetaBiNode));
			btmp->mid=tmp;
			node->lnode=btmp;

			return(tmp);
		}

		tmp=DYLL_MetaPath_GetNodeR2(node->lnode, name);
		DYLL_MetaPath_BalanceBiNode(node);

		return(tmp);
	}else if(i>0)
	{
		if(!node->rnode)
		{
			tmp=bgbcc_tmalloc2("_dyc_metanode_t",
				sizeof(BGBDYC_MetaNode));
			tmp->name=name;
			tmp->owner=mid->owner;

			if(mid->next)
			{
				mid->next->prev=tmp;
				tmp->prev=mid;
				tmp->next=mid->next;
				mid->next=tmp;
			}else
			{
				tmp->prev=mid;
				mid->next=tmp;
			}

			btmp=bgbcc_tmalloc2("_dyc_metabinode_t",
				sizeof(BGBDYC_MetaBiNode));
			btmp->mid=tmp;
			node->rnode=btmp;

			return(tmp);
		}

		tmp=DYLL_MetaPath_GetNodeR2(node->rnode, name);
		DYLL_MetaPath_BalanceBiNode(node);

		return(tmp);
	}else
	{
		return(mid);
	}

	return(NULL);
}

BGBDYC_MetaNode *DYLL_MetaPath_GetNodeR(BGBDYC_MetaNode *node, char *path)
{
	char tb[256];
	BGBDYC_MetaNode *cur, *prv, *tmp;
	BGBDYC_MetaBiNode *btmp;
	char *s, *t, *s1;
	int i;

	s=path; t=tb;
	while(*s=='/')s++;
	if(!*s)return(node);

	while(*s && (*s!='/'))*t++=*s++;
	*t++=0;

	s1=tb; i=0;
	while(*s1)i=(i*251)+(*s1++);
	i&=4095;

	if((dyll_metapath_hash2_snode[i]==node) &&
		dyll_metapath_hash2_name[i] &&
		!strcmp(dyll_metapath_hash2_name[i], tb))
	{
		cur=dyll_metapath_hash2_dnode[i];
		if(cur)return(DYLL_MetaPath_GetNodeR(cur, s));
	}

	t=bgbcc_strdup(tb);
	if(node->biroot)
	{
		cur=DYLL_MetaPath_GetNodeR2(node->biroot, t);
	}else
	{
		cur=bgbcc_tmalloc2("_dyc_metanode_t", sizeof(BGBDYC_MetaNode));
		cur->name=t;
		cur->owner=node;
		node->first=cur;

		btmp=bgbcc_tmalloc2("_dyc_metabinode_t",
			sizeof(BGBDYC_MetaBiNode));
		btmp->mid=cur;

		node->biroot=btmp;
	}

	dyll_metapath_hash2_name[i]=t;
	dyll_metapath_hash2_snode[i]=node;
	dyll_metapath_hash2_dnode[i]=cur;

	return(DYLL_MetaPath_GetNodeR(cur, s));
}
#endif


BGBDYC_MetaNode *DYLL_MetaPath_LookupNode(char *path)
{
	BGBDYC_MetaNode *tmp;
	char *s;
	int i;

	s=path; i=0;
	while(*s)i=(i*251)+(*s++);
	i&=4095;

	if(dyll_metapath_hash_name[i] &&
		!strcmp(dyll_metapath_hash_name[i], path))
			return(dyll_metapath_hash_node[i]);

	if(!dyll_metapath_root)return(NULL);
	tmp=DYLL_MetaPath_LookupNodeR(dyll_metapath_root, path);

	if(dyll_metapath_hash_name[i])
		free(dyll_metapath_hash_name[i]);
	dyll_metapath_hash_name[i]=strdup(path);
	dyll_metapath_hash_node[i]=tmp;

	return(tmp);
}

BGBDYC_MetaNode *DYLL_MetaPath_GetNode(char *path)
{
	BGBDYC_MetaNode *tmp;
	char *s;
	int i;

	if(!dyll_metapath_root)
	{
		tmp=bgbcc_tmalloc2("_dyc_metanode_t", sizeof(BGBDYC_MetaNode));
		dyll_metapath_root=tmp;
	}

	s=path; i=0;
	while(*s)i=(i*251)+(*s++);
	i&=4095;

	if(dyll_metapath_hash_name[i] &&
		!strcmp(dyll_metapath_hash_name[i], path))
	{
		tmp=dyll_metapath_hash_node[i];
		if(tmp)return(tmp);
	}

	tmp=DYLL_MetaPath_GetNodeR(dyll_metapath_root, path);

	if(dyll_metapath_hash_name[i])
		free(dyll_metapath_hash_name[i]);
	dyll_metapath_hash_name[i]=strdup(path);
	dyll_metapath_hash_node[i]=tmp;

	return(tmp);
}

BGBDYC_MetaLeaf *DYLL_MetaPath_LookupNodeLeaf(BGBDYC_MetaNode *node, char *key)
{
	BGBDYC_MetaLeaf *cur;
	char *s, *t;

	if(!key || !(*key))key="_";

	t=bgbcc_strdup(key);
	cur=node->leaf;
	while(cur)
	{
		if(cur->key==t)
			break;
		cur=cur->next;
	}

	if(!cur)return(NULL);
	return(cur);
}

BGBDYC_MetaLeaf *DYLL_MetaPath_GetNodeLeaf(BGBDYC_MetaNode *node, char *key)
{
	BGBDYC_MetaLeaf *cur, *prv, *tmp;
	char *s, *t;

	if(!key || !(*key))key="_";

	t=bgbcc_strdup(key);
	cur=node->leaf;
	while(cur)
	{
		if(cur->key==t)
			break;
		cur=cur->next;
	}

	if(!cur)
	{
		tmp=bgbcc_tmalloc2("_dyc_metaleaf_t", sizeof(BGBDYC_MetaLeaf));
		tmp->key=t;

		if(node->leaf)
		{
			cur=node->leaf; prv=NULL;
			while(cur)
			{
				if(strcmp(cur->key, t)>=0)
					break;
				prv=cur; cur=cur->next;
			}
			if(prv)
			{
				tmp->next=cur;
				prv->next=tmp;
			}else
			{
				tmp->next=cur;
				node->leaf=tmp;
			}
		}else
		{
			tmp->next=node->leaf;
			node->leaf=tmp;
		}

		cur=tmp;
	}

	return(cur);
}

void DYLL_MetaPath_SplitKey(char *path, char *nb, char *kb)
{
	char *s, *t;
	int i;

	s=path;

	t=nb;
	while(*s && (*s!=':'))
		*t++=*s++;
	*t++=0;
	if(*s==':')s++;

	t=kb;
	while(*s)
		*t++=*s++;
	*t++=0;
}

char *DYLL_MetaPath_LookupKey(char *path)
{
	char nb[1024], kb[256];
	BGBDYC_MetaNode *node;
	BGBDYC_MetaLeaf *leaf;
	int i, j;

	DYLL_MetaPath_SplitKey(path, nb, kb);

	node=DYLL_MetaPath_LookupNode(nb);
	if(!node)return(NULL);
	leaf=DYLL_MetaPath_LookupNodeLeaf(node, kb);
	if(!leaf)return(NULL);

	return(leaf->val);
}

void DYLL_MetaPath_BindKey(char *path, char *val)
{
	char nb[1024], kb[256];
	BGBDYC_MetaNode *node;
	BGBDYC_MetaLeaf *leaf;
	int i, j;

	DYLL_MetaPath_SplitKey(path, nb, kb);

	node=DYLL_MetaPath_GetNode(nb);
	leaf=DYLL_MetaPath_GetNodeLeaf(node, kb);
	leaf->val=bgbcc_strdup(val);
}

void DYLL_MetaPath_LoadDB(char *name)
{
	char tb[1024], nb[1024], kb[256];
	FILE *fd;
	char **a;
	char *s, *t;

	fd=fopen(name, "rb");
	if(!fd)return;

	nb[0]=0;

	while(!feof(fd))
	{
		fgets(tb, 255, fd);

		s=tb;
		while(*s && (*s<=' '))s++;
		if(!(*s))continue;
		if((*s)==';')continue;
		if((*s)=='#')continue;
		if((*s)=='/')continue;

		if((*s)=='[')
		{
			s++;
			t=nb;
			while(*s && (*s!=']'))*t++=*s++;
			*t++=0;
			continue;
		}

		t=kb;
		while(*s && (*s!='='))*t++=*s++;
		*t++=0;
		if(*s=='=')s++;

		s=bgbcc_strdup(s);
		sprintf(tb, "%s:%s", nb, kb);

		DYLL_MetaPath_BindKey(tb, s);
	}
	fclose(fd);
}

void DYLL_MetaPath_SaveNode(FILE *fd,
	BGBDYC_MetaNode *node, char *path, int ind)
{
	char tb[1024];
	BGBDYC_MetaLeaf *lcur;
	BGBDYC_MetaNode *ncur;
	char *s0;
	int i;

	if(node->leaf)
	{
		for(i=0; i<ind; i++)
			fprintf(fd, "  ");

		fprintf(fd, "[%s]\n", path);
		lcur=node->leaf;

		while(lcur)
		{
			if(!lcur->val || (lcur->val==NULL))
			{
				lcur=lcur->next;
				continue;
			}

			for(i=0; i<ind; i++)
				fprintf(fd, "  ");
			fprintf(fd, "  %s=%s\n", lcur->key, lcur->val);
			lcur=lcur->next;
		}
	}

	ncur=node->first;
	while(ncur)
	{
		if(*path)sprintf(tb, "%s/%s", path, ncur->name);
			else strcpy(tb, ncur->name);
		DYLL_MetaPath_SaveNode(fd, ncur, tb, ind+1);
		ncur=ncur->next;
	}
}

void DYLL_MetaPath_SaveDB(char *name)
{
	FILE *fd;
	int i;

	if(!dyll_metapath_root)return;

	fd=fopen(name, "wt");
	if(!fd)return;
	DYLL_MetaPath_SaveNode(fd, dyll_metapath_root, "", -1);
	fclose(fd);
}

char *DYLL_MetaPath_SaveNodeBuffer(char *obuf,
	BGBDYC_MetaNode *node, char *path, int ind)
{
	char tb[1024];
	BGBDYC_MetaLeaf *lcur;
	BGBDYC_MetaNode *ncur;
	char *s0;
	int i;

	if(node->leaf)
	{
		for(i=0; i<ind; i++)
		{
			sprintf(obuf, "  ");
			obuf+=strlen(obuf);
		}

		sprintf(obuf, "[%s]\n", path);
		obuf+=strlen(obuf);
		lcur=node->leaf;

		while(lcur)
		{
			if(!lcur->val || (lcur->val==NULL))
			{
				lcur=lcur->next;
				continue;
			}

			for(i=0; i<ind; i++)
			{
				sprintf(obuf, "  ");
				obuf+=strlen(obuf);
			}
			sprintf(obuf, "  %s=%s\n", lcur->key, lcur->val);
			obuf+=strlen(obuf);
			lcur=lcur->next;
		}
	}

	ncur=node->first;
	while(ncur)
	{
		if(*path)sprintf(tb, "%s/%s", path, ncur->name);
			else strcpy(tb, ncur->name);
		obuf=DYLL_MetaPath_SaveNodeBuffer(obuf, ncur, tb, ind+1);
		ncur=ncur->next;
	}
	return(obuf);
}

int DYLL_MetaPath_SaveDBBuffer(char *obuf, int szbuf)
{
	char *ct;
	int i;

	if(!dyll_metapath_root)return(0);

//	fd=fopen(name, "wt");
//	if(!fd)return;
	ct=DYLL_MetaPath_SaveNodeBuffer(obuf, dyll_metapath_root, "", -1);
//	fclose(fd);
	return(ct-obuf);
}

void dyllMetaInit()
{
	static int init=0;
	char tb[1024];
	char **a;

	if(init)return;
	init=1;

//	DYLL_MetaPath_LoadAll();
}

void dyllMetaCommit()
{
//	DYLL_MetaPath_SaveAll();
}


void dyllMetaBindKey(char *key, char *val)
{
	dyllMetaInit();
	DYLL_MetaPath_BindKey(key, val);
}

char *dyllMetaLookupKey(char *key)
{
	char *s;
	dyllMetaInit();
	s=DYLL_MetaPath_LookupKey(key);
//	if(s==UNDEFINED)return(NULL);
	return(s);
}
