BGBCC_VlSenseList *BGBCC_CCXL_AllocVlSenseList(BGBCC_TransState *ctx)
{
	BGBCC_VlSenseList *cur;

	cur=ctx->free_slist;
	if(cur)
	{
		ctx->free_slist=cur->next;
		memset(cur, 0, sizeof(BGBCC_VlSenseList));
	}else
	{
		cur=bgbcc_malloc(sizeof(BGBCC_VlSenseList));
		memset(cur, 0, sizeof(BGBCC_VlSenseList));
	}

	cur->pedep=cur->t_pedep;
	cur->nedep=cur->t_nedep;
	cur->refin=cur->t_refin;
	cur->refout=cur->t_refout;

	cur->m_pedep=4;
	cur->m_nedep=4;
	cur->m_refin=64;
	cur->m_refout=64;

	return(cur);
}

void BGBCC_CCXL_FreeVlSenseList(BGBCC_TransState *ctx, BGBCC_VlSenseList *cur)
{
	cur->next=ctx->free_slist;
	ctx->free_slist=cur;
}

void BGBCC_CCXL_VlMarkRefIn(BGBCC_TransState *ctx, char *name)
{
	BGBCC_VlSenseList *sn;
	int i, j, k;

	sn=ctx->cur_slist;
	if(!sn)
		return;
	
	for(i=0; i<sn->n_refin; i++)
	{
		if(!strcmp(name, sn->refin[i]))
			return;
	}
	
	for(i=0; i<sn->n_refout; i++)
	{
		if(!strcmp(name, sn->refout[i]))
			return;
	}
	
	if(sn->n_refin>=sn->m_refin)
	{
		if(sn->refin==sn->t_refin)
		{
			k=sn->m_refin;
			k=k+(k>>1);
			sn->refin=bgbcc_malloc(k*sizeof(char *));
			memcpy(sn->refin, sn->t_refin, sn->m_refin*sizeof(char *));
			sn->m_refin=k;
		}else
		{
			k=sn->m_refin;
			k=k+(k>>1);
			sn->refin=bgbcc_realloc(sn->refin, k*sizeof(char *));
			sn->m_refin=k;
		}
	}
	
	i=sn->n_refin++;
	sn->refin[i]=bgbcc_strdup(name);
}

void BGBCC_CCXL_VlMarkRefOut(BGBCC_TransState *ctx, char *name)
{
	BGBCC_VlSenseList *sn;
	int i, j, k;

	sn=ctx->cur_slist;
	if(!sn)
		return;
	
	for(i=0; i<sn->n_refout; i++)
	{
		if(!strcmp(name, sn->refout[i]))
			return;
	}
	
	if(sn->n_refout>=sn->m_refout)
	{
		if(sn->refout==sn->t_refout)
		{
			k=sn->m_refout;
			k=k+(k>>1);
			sn->refout=bgbcc_malloc(k*sizeof(char *));
			memcpy(sn->refout, sn->t_refout, sn->m_refout*sizeof(char *));
			sn->m_refout=k;
		}else
		{
			k=sn->m_refout;
			k=k+(k>>1);
			sn->refout=bgbcc_realloc(sn->refout, k*sizeof(char *));
			sn->m_refout=k;
		}
	}
	
	i=sn->n_refout++;
	sn->refout[i]=bgbcc_strdup(name);
}

void BGBCC_CCXL_VlMarkPosedge(BGBCC_TransState *ctx, char *name)
{
	BGBCC_VlSenseList *sn;
	int i, j, k;

	sn=ctx->cur_slist;
	if(!sn)
		return;
	
	for(i=0; i<sn->n_pedep; i++)
	{
		if(!strcmp(name, sn->pedep[i]))
			return;
	}	

	if(sn->n_pedep>=sn->m_pedep)
		return;

	i=sn->n_pedep++;
	sn->pedep[i]=bgbcc_strdup(name);
}

void BGBCC_CCXL_VlMarkNegedge(BGBCC_TransState *ctx, char *name)
{
	BGBCC_VlSenseList *sn;
	int i, j, k;

	sn=ctx->cur_slist;
	if(!sn)
		return;
	
	for(i=0; i<sn->n_nedep; i++)
	{
		if(!strcmp(name, sn->nedep[i]))
			return;
	}	

	if(sn->n_nedep>=sn->m_nedep)
		return;

	i=sn->n_nedep++;
	sn->nedep[i]=bgbcc_strdup(name);
}

bool BGBCC_CCXL_VlSenseCheckRefIn(
	BGBCC_TransState *ctx, BGBCC_VlSenseList *sn, char *name)
{
	int i, j, k;

	for(i=0; i<sn->n_refin; i++)
	{
		if(!strcmp(name, sn->refin[i]))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_VlSenseCheckRefOut(
	BGBCC_TransState *ctx, BGBCC_VlSenseList *sn, char *name)
{
	int i, j, k;

	for(i=0; i<sn->n_refout; i++)
	{
		if(!strcmp(name, sn->refout[i]))
			return(true);
	}
	return(false);
}

/*
 Compare two sense lists.
 -1: Sn2 comes before Sn1.
  1: Sn1 comes before Sn2.
  0: No ordering dependency.
 */
int BGBCC_CCXL_VlSenseCompareRefOrder(
	BGBCC_TransState *ctx, BGBCC_VlSenseList *sn1, BGBCC_VlSenseList *sn2)
{
	int i, j, k;

	for(i=0; i<sn1->n_refin; i++)
	{
		if(BGBCC_CCXL_VlSenseCheckRefOut(ctx, sn2, sn1->refin[i]))
			return(-1);
	}

	for(i=0; i<sn2->n_refin; i++)
	{
		if(BGBCC_CCXL_VlSenseCheckRefOut(ctx, sn1, sn2->refin[i]))
			return(1);
	}
	return(0);
}

void BGBCC_CCXL_CompileVlModule(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	short t_srtlst[32];
	BGBCC_CCXL_LiteralInfo *cur;
	BGBCC_VlSenseList *oslist, *ocslist;
	BGBCC_VlSenseList *sncur, *sncxt, *snc1, *snc2, *snslst;
	BCCX_Node *c, *t, *n, *osn, *rcl, *icl;
	char *srtgrid;
	short *srtlst, *srttlst;
	char *s, *os0, *qn, *s0, *s1;
	int i, j, k, j1, j2, na, ci, nsn;

	if(ctx->cf_n)
		{ BGBCC_DBGBREAK }

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");

	qn=BGBCC_CCXL_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
//	if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP))
	if(1)
	{
		cur=BGBCC_CCXL_LookupStructure(ctx, qn);
		if(cur)
		{
			if(cur->decl && cur->decl->n_fields)
				return;
		}
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=qn;
	j=ctx->cur_idx;
	j1=ctx->cur_idx2;
	j2=ctx->cur_idx3;
	oslist=ctx->mod_slist;
	ocslist=ctx->cur_slist;

	ctx->cur_idx=0;
	ctx->cur_idx2=0;
	ctx->cur_idx3=0;
	ctx->mod_slist=NULL;
	ctx->cur_slist=NULL;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_CLASS, qn);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, qn);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_EXTENDS);

	s0=bgbcc_strdup("LVlModuleBase;");
	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s0);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s0);
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_End(ctx);

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileVarStatement(ctx, c);
	}

	BGBCC_CCXL_End(ctx);

	n=BCCX_FindTagCst(l, &bgbcc_rcst_body, "body");
	na=BCCX_GetNodeChildCount(n);
	for(ci=0; ci<na; ci++)
	{
		c=BCCX_GetNodeIndex(n, ci);
		BGBCC_CCXL_CompileVarStatement2(ctx, c);
	}

	nsn=0;
	sncur=ctx->mod_slist;
	while(sncur)
		{ nsn++; sncur=sncur->next; }
	if(nsn<=16)
		{ srtgrid=tb; srtlst=t_srtlst; srttlst=srtlst+nsn; }
	else
		{ srtgrid=malloc(nsn*nsn); srtlst=malloc(2*nsn*2); srttlst=srtlst+nsn; }
	
	snc1=ctx->mod_slist;
	while(snc1)
	{
		j=snc1->blkord;
		srtgrid[j*nsn+j]=0;
		snc2=snc1->next;
		while(snc2)
		{
			k=snc2->blkord;
			i=BGBCC_CCXL_VlSenseCompareRefOrder(ctx, snc1, snc2);
			snc2=snc2->next;
			srtgrid[j*nsn+k]= i;
			srtgrid[k*nsn+j]=-i;
		}
		snc1=snc1->next;
	}
	
	for(i=0; i<nsn; i++)
		srtlst[i]=i;

	for(i=0; i<nsn; i++)
	{
		for(j=i+1; j<nsn; j++)
		{
			j1=srtlst[i];
			j2=srtlst[j];
			if(srtgrid[j1*nsn+j2]<0)
			{
				srtlst[i]=j2;
				srtlst[j]=j1;
			}
		}
	}

	for(i=0; i<nsn; i++)
	{
		srttlst[srtlst[i]]=i;
	}

	snslst=NULL;
	while(sncur)
	{
		sncur->rankord=srttlst[sncur->blkord];

		if(snslst)
		{
			if(sncur->rankord>snslst->rankord)
			{
				snc1=snslst;
				while(snc1->chain && (snc1->chain->rankord<sncur->rankord))
					snc1=snc1->chain;
				sncur->chain=snc1->chain;
				snc1->chain=sncur;
			}else
			{
				sncur->chain=snslst;
				snslst=sncur;
			}
		}else
		{
			snslst=sncur;
			sncur->chain=NULL;
		}
	
		sncur=sncur->next;
	}

	rcl=NULL;
	icl=NULL;
	sncur=snslst;
	while(sncur)
	{
		n=BCCX_NewCst1(&bgbcc_rcst_funcall, "funcall",
			BCCX_NewCst1(&bgbcc_rcst_args, "args", NULL));
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", sncur->name);

		rcl=BCCX_AddEnd(rcl, n);
		sncur=sncur->chain;
	}

	n=BCCX_NewCst1(&bgbcc_rcst_begin, "begin", rcl);
	BGBCC_CCXL_CompileBlock2(ctx, NULL, "run", NULL, n, NULL);

	n=BCCX_NewCst1(&bgbcc_rcst_begin, "begin", icl);
	BGBCC_CCXL_CompileBlock2(ctx, NULL, "init", NULL, n, NULL);
	
	if(srtlst!=t_srtlst)
	{
		free(srtgrid);
		free(srtlst);
	}

	sncur=ctx->mod_slist;
	while(sncur)
	{
		sncxt=sncur->next;
		BGBCC_CCXL_FreeVlSenseList(ctx, sncur);
		sncur=sncxt;
	}

//	bgbcc_free(ctx->mod_slist);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
	ctx->mod_slist=oslist;
	ctx->cur_slist=ocslist;
}
