TKUCC_AstNode *TKUCC_AstAllocNode(TKUCC_MainContext *ctx,
	int tag, int zid)
{
	TKUCC_AstNode *tmp;
	
	tmp=TKUCC_ZMalloc(ctx, sizeof(TKUCC_AstNode), zid);
	tmp->tag=tag;
	return(tmp);
}

//TKUCC_MainContext *TKUCC_AstGetNodeCtx(TKUCC_AstNode *n)
//{
//	return(TKUCC_ZGetContext(n));
//}

int TKUCC_AstGetNodeZid(TKUCC_MainContext *ctx, TKUCC_AstNode *n)
{
	int zid;
	zid=TKUCC_ZGetZoneId(n);
	return(zid);
}

int TKUCC_AstGetAttrBase(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, u64 *rval)
{
	int i, j, k, l;
	
	if(n->n_lvl==0)
	{
		l=n->n_keys;
		for(i=0; i<l; i++)
		{
			if(n->keys[i]==id)
			{
				*rval=n->vals[i];
				return(1);
			}
		}

		*rval=0;
		return(0);
	}

	l=n->n_keys;
	for(i=0; i<l; i++)
	{
		if(n->keys[i+1]>id)
		{
			return(TKUCC_AstGetAttrBase(
				ctx, (TKUCC_AstNode *)(n->vals[i]), id, rval));
		}
	}
	*rval=0;
	return(0);
}

void TKUCC_AstSetAttrBase(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, u64 val)
{
	TKUCC_AstNode *n1, *n2;
	int i, j, k, l, l1, zid;
	
	if(id&TKUCC_ASTNODE_KTAG_NODEIDX)
	{
		j=id&TKUCC_ASTNODE_KTAG_IMASK;
		if(j>n->maxidx)
			n->maxidx=j;
	}
	
	if(n->n_lvl==0)
	{
		l=n->n_keys;
		for(i=0; i<l; i++)
		{
			if(n->keys[i]==id)
			{
				n->vals[i]=val;
				return;
			}
		}
		
		if(l<TKUCC_ASTNODE_NKEYS)
		{
//			i=n->n_keys++;
//			n->keys[i]=id;
//			n->vals[i]=val;

			l=n->n_keys++;
			for(i=l; i>0; i--)
			{
				n->keys[i]=n->keys[i-1];
				n->vals[i]=n->vals[i-1];
				if(n->keys[i]<id)
					break;
			}
			n->keys[i]=id;
			n->vals[i]=val;

			return;
		}
	}

	l=n->n_keys;
	if((l+2)>=TKUCC_ASTNODE_NKEYS)
	{
		zid=TKUCC_AstGetNodeZid(ctx, n);
		n1=TKUCC_AstAllocNode(ctx, 0, zid);
		n2=TKUCC_AstAllocNode(ctx, 0, zid);
		n1->n_lvl=n->n_lvl;
		n2->n_lvl=n->n_lvl;
		
		k=l>>1;
		for(i=0; i<k; i++)
		{
			n1->keys[i]=n->keys[i];
			n1->vals[i]=n->vals[i];
		}
		n1->keys[i]=0xFFFF;

		for(i=0; i<(l-k); i++)
		{
			n2->keys[i]=n->keys[k+i];
			n2->vals[i]=n->vals[k+i];
		}
		n2->keys[i]=0xFFFF;
		
		n1->n_keys=k;
		n2->n_keys=l-k;
		
		n->keys[0]=n1->keys[0];
		n->vals[0]=(u64)n1;
		n->keys[1]=n2->keys[0];
		n->vals[1]=(u64)n2;
		n->keys[2]=0xFFFF;
		n->n_keys=2;
		n->n_lvl++;
	}

	l=n->n_keys;
	for(i=0; i<l; i++)
	{
		if(n->keys[i+1]>id)
			break;
	}
	
	if(i<l)
	{
		zid=TKUCC_AstGetNodeZid(ctx, n);
		n1=(TKUCC_AstNode *)(n->vals[i]);
		l1=n1->n_keys;

		if((l1+2)>=TKUCC_ASTNODE_NKEYS)
		{
			n2=TKUCC_AstAllocNode(ctx, 0, zid);
			n2->n_lvl=n1->n_lvl;

			k=l1>>1;
			for(i=0; i<(l1-k); i++)
			{
				n2->keys[i]=n1->keys[k+i];
				n2->vals[i]=n1->vals[k+i];
			}
			n1->keys[k]=0xFFFF;
			n2->keys[i]=0xFFFF;
			
			n1->n_keys=k;
			n2->n_keys=l1-k;

			for(j=l; j>(i+1); j--)
			{
				n->keys[j]=n->keys[j-1];
				n->vals[j]=n->vals[j-1];
			}
			
			n->keys[i+1]=n2->keys[0];
			n->vals[i+1]=(u64)n2;
			n->n_keys=l+1;
			n->keys[l+1]=0xFFFF;
			
			if(n2->keys[0]<=id)
			{
				i++;
				n1=(TKUCC_AstNode *)(n->vals[i]);
				l1=n1->n_keys;
			}
		}
		
		TKUCC_AstSetAttrBase(ctx, n1, id, val);
	}
	
	TKUCC_DBGBREAK
}

TKUCC_AstNode *TKUCC_AstCloneNode(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n)
{
	TKUCC_AstNode *n1, *n2;
	int i, j, k, l, l1, zid;
	
	if(n->n_lvl==0)
	{
		zid=TKUCC_AstGetNodeZid(ctx, n);
		l=n->n_keys;

		n1=TKUCC_AstAllocNode(ctx, n->tag, zid);
		for(i=0; i<l; i++)
		{
			n1->keys[i]=n->keys[i];
			n1->vals[i]=n->vals[i];
		}
		
		n1->n_keys=n->n_keys;
		n1->n_lvl=n->n_lvl;
		n1->maxidx=n->maxidx;
		return(n1);
	}

	zid=TKUCC_AstGetNodeZid(ctx, n);
	l=n->n_keys;

	n1=TKUCC_AstAllocNode(ctx, n->tag, zid);
	for(i=0; i<l; i++)
	{
		n2=(TKUCC_AstNode *)(n->vals[i]);
		n2=TKUCC_AstCloneNode(ctx, n2);
		n1->keys[i]=n->keys[i];
		n1->vals[i]=(u64)(n2);
	}
	
	n1->n_keys=n->n_keys;
	n1->n_lvl=n->n_lvl;
	n1->maxidx=n->maxidx;
	return(n1);
}

s64 TKUCC_AstGetAttrInt(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id)
{
	u64 v;
	TKUCC_AstGetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_INT, &v);
	return((s64)v);
}

int TKUCC_AstSetAttrInt(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, s64 val)
{
	u64 v;
	TKUCC_AstSetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_INT, (u64)val);
	return(0);
}

double TKUCC_AstGetAttrReal(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id)
{
	u64 v;
	double f;
	TKUCC_AstGetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_REAL, &v);
	memcpy(&f, &v, sizeof(double));
	return(f);
}

int TKUCC_AstSetAttrReal(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, double val)
{
	u64 v;
	memcpy(&v, &val, sizeof(double));
	TKUCC_AstSetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_REAL, v);
	return(0);
}

char *TKUCC_AstGetAttrStr(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id)
{
	u64 v;
	TKUCC_AstGetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_STR, &v);
	if(!v)
		return(NULL);
	return(TKUCC_InternIndexToString(ctx, v));
}

int TKUCC_AstSetAttrStr(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, char *str)
{
	int zid;
	u64 v;
	zid=TKUCC_AstGetNodeZid(ctx, n);
	v=TKUCC_InternStringToIndex(ctx, str, zid);
	TKUCC_AstSetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_STR, v);
	return((s64)v);
}

TKUCC_AstNode *TKUCC_AstGetAttrNode(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id)
{
	u64 v;
	TKUCC_AstGetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_NODE, &v);
	if(!v)
		return(NULL);
	return((TKUCC_AstNode *)v);
}

int TKUCC_AstSetAttrNode(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, TKUCC_AstNode *nv)
{
	u64 v;
	v=(u64)nv;
	TKUCC_AstSetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_NODE, v);
	return((s64)v);
}


TKUCC_AstNode *TKUCC_AstGetAttrNodeIndex(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id)
{
	u64 v;
	TKUCC_AstGetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_NODEIDX, &v);
	if(!v)
		return(NULL);
	return((TKUCC_AstNode *)v);
}

int TKUCC_AstSetAttrNodeIndex(TKUCC_MainContext *ctx,
	TKUCC_AstNode *n, int id, TKUCC_AstNode *nv)
{
	u64 v;
	v=(u64)nv;
	TKUCC_AstSetAttrBase(ctx, n, id|TKUCC_ASTNODE_KTAG_NODEIDX, v);
	return((s64)v);
}

int TKUCC_AstAddNodeEnd(TKUCC_MainContext *ctx,
	TKUCC_AstNode *np, TKUCC_AstNode *nv)
{
	if(!nv)
		return(0);
	TKUCC_AstSetAttrNodeIndex(ctx, np, np->maxidx+1, nv);
	return(0);
}
