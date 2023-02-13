/*
Environment Variables via Context.
 */

TK_EnvContext *tk_envctx_free;

TK_EnvContext *TK_EnvCtx_AllocContext(void)
{
	TK_EnvContext *tmp;
	
	tmp=tk_envctx_free;
	if(tmp)
	{
		tk_envctx_free=tmp->next;
		memset(tmp, 0, sizeof(TK_EnvContext));
		return(tmp);
	}
	
	tmp=tk_malloc(sizeof(TK_EnvContext));
	memset(tmp, 0, sizeof(TK_EnvContext));
	return(tmp);
}

void TK_EnvCtx_FreeContext(TK_EnvContext *ctx)
{
	ctx->next=tk_envctx_free;
	tk_envctx_free=ctx;
}

TK_EnvContext *TK_EnvCtx_CloneContext(TK_EnvContext *ctx)
{
	TK_EnvContext *tmp;
	int i, j, k;

	tmp=TK_EnvCtx_AllocContext();	
	if(!ctx)
	{
		tk_puts("TK_EnvCtx_CloneContext: Clone Null Context\n");
		return(tmp);
	}

	for(i=0; i<ctx->nenvlst; i++)
	{
		TK_EnvCtx_SetEnvVar(tmp,
			ctx->envlst_var[i],
			ctx->envlst_val[i]);
	}

	tk_printf("TK_EnvCtx_CloneContext: Cwd=%s\n", ctx->cwd);

	TK_EnvCtx_SetCwd(tmp, ctx->cwd);
	
	return(tmp);
}

char *TK_EnvCtx_SetEnvVarI_StrDup(TK_EnvContext *ctx, char *val)
{
	char *ct;
	int l;
	
	if(!(ctx->envbufs))
	{
		ctx->envbufs=tk_malloc(4096);
		ctx->envbufe=ctx->envbufs+4096;
		ctx->envbufc=ctx->envbufs;
	}
	
	l=strlen(val);
	if((ctx->envbufc+l)>=ctx->envbufe)
		return(NULL);

	ct=ctx->envbufc;
	ctx->envbufc=ct+l+1;
	strcpy(ct, val);
	return(ct);
}

int TK_EnvCtx_RepackEnvbuf(TK_EnvContext *ctx)
{
	char *oldenv;
	char *cn0, *cv0, *cn1, *cv1;
	int i, j, k;

	oldenv=ctx->envbufs;
	
	k=ctx->envbufe-ctx->envbufs;
	k=k+(k>>1);
	ctx->envbufs=tk_malloc(k);
	ctx->envbufe=ctx->envbufs+k;
	ctx->envbufc=ctx->envbufs;
	
	for(i=0; i<ctx->nenvlst; i++)
	{
		cn0=ctx->envlst_var[i];
		cv0=ctx->envlst_val[i];
		cn1=TK_EnvCtx_SetEnvVarI_StrDup(ctx, cn0);
		cv1=TK_EnvCtx_SetEnvVarI_StrDup(ctx, cv0);
		ctx->envlst_var[i]=cn1;
		ctx->envlst_val[i]=cv1;
	}
	
	tk_free(oldenv);

	return(0);
}

int TK_EnvCtx_GetEnvVarIdx(TK_EnvContext *ctx, int idx,
	char *bufn, char *bufv, int szn, int szv)
{
	int i, j, k;

	if(idx<0)
		return(-1);
	if(idx>=ctx->nenvlst)
		return(-1);

	strcpy(bufn, ctx->envlst_var[idx]);
	strcpy(bufv, ctx->envlst_val[idx]);
	return(1);

#if 0
	for(i=0; i<ctx->nenvlst; i++)
	{
		if(!strcmp(ctx->envlst_var[i], varn))
		{
			strncpy(bufv, ctx->envlst_val[i], sz);
			return(1);
		}
	}
#endif
	return(-1);
}

int TK_EnvCtx_GetEnvVarI(TK_EnvContext *ctx, char *varn, char *bufv, int sz)
{
	int i, j, k;

	for(i=0; i<ctx->nenvlst; i++)
	{
		if(!strcmp(ctx->envlst_var[i], varn))
		{
			strncpy(bufv, ctx->envlst_val[i], sz);
			return(1);
		}
	}
	return(-1);
}

int TK_EnvCtx_SetEnvVarI(TK_EnvContext *ctx, char *varn, char *varv)
{
	char *cn1, *cv1;
	int i, j, k;

	tk_printf("TK_EnvCtx_SetEnvVarI: %s=%s\n", varn, varv);
	
	if(!ctx->envbufs)
	{
		ctx->envbufs=tk_malloc(65536);
		ctx->envbufe=ctx->envbufs+65536;
		ctx->envbufc=ctx->envbufs;

		ctx->envlst_var=tk_malloc(256*sizeof(char *));
		ctx->envlst_val=tk_malloc(256*sizeof(char *));
		ctx->nenvlst=0;
		ctx->menvlst=256;
	}
	
	for(i=0; i<ctx->nenvlst; i++)
	{
		if(!strcmp(ctx->envlst_var[i], varn))
		{
			cv1=TK_EnvCtx_SetEnvVarI_StrDup(ctx, varv);
			
			if(cv1)
			{
				ctx->envlst_val[i]=cv1;
				return(1);
			}
			break;
		}
	}
	
	if(i>=ctx->nenvlst)
	{
		if((ctx->nenvlst+1)>=ctx->menvlst)
		{
			k=ctx->menvlst; k=k+(k>>1);
			ctx->envlst_var=tk_realloc(
				ctx->envlst_var, k*sizeof(char *));
			ctx->envlst_val=tk_realloc(
				ctx->envlst_val, k*sizeof(char *));
			ctx->menvlst=k;
		}

		cn1=TK_EnvCtx_SetEnvVarI_StrDup(ctx, varn);
		cv1=TK_EnvCtx_SetEnvVarI_StrDup(ctx, varv);

		if(cn1 && cv1)
		{
			i=ctx->nenvlst++;
			ctx->envlst_var[i]=cn1;
			ctx->envlst_val[i]=cv1;
			return(1);
		}
	}

	TK_EnvCtx_RepackEnvbuf();
	TK_EnvCtx_SetEnvVarI(ctx, varn, varv);
	return(1);
}

int TK_EnvCtx_SetCwd(TK_EnvContext *ctx, char *cwd)
{
	if(cwd)
		ctx->cwd=TKMM_LVA_Strdup(cwd);
	else
		ctx->cwd=NULL;
}

char *TK_EnvCtx_GetCwd(TK_EnvContext *ctx, char *buf, int sz)
{
	char *cwd;
	int l;
	
//	tk_printf("TK_EnvCtx_GetCwd env=%p\n", ctx);
	
	cwd=NULL;
	if(ctx)	
		{ cwd=ctx->cwd; }
	if(!cwd)
	{
//		tk_printf("TK_EnvCtx_GetCwd env=%p, CWD was NULL\n", ctx);
		cwd="/";
	}else
	{
//		tk_printf("TK_EnvCtx_GetCwd env=%p, CWD=%s (%p)\n", ctx, cwd, cwd);
	}
	
	l=strlen(cwd);
	
	if(!buf)
	{
		if(!sz)
			sz=l+1;
		if(l>=sz)
			return(NULL);
		buf=malloc(sz);
		strcpy(buf, cwd);
		return(buf);
	}
	
	if(strlen(cwd)>=sz)
		return(NULL);
	strcpy(buf, cwd);
	
	return(buf);
}

int TK_EnvCtx_GetPathList(TK_EnvContext *ctx, char ***rlst, int *rnlst)
{
	*rlst=ctx->pathlst;
	*rnlst=ctx->npathlst;
	return(1);
}

int TK_EnvCtx_SetPath(TK_EnvContext *ctx, char *path)
{
//	char tb[1024];
	char **tba;
	int ntb;
	
	char *cs, *ct, *csb, *ctb;
	
	if(!ctx->pathbuf)
		ctx->pathbuf=malloc(65536);
	if(!ctx->pathlst)
		ctx->pathlst=malloc(256*sizeof(char *));
	
	tba=ctx->pathlst;
	ctb=ctx->pathbuf;
	cs=path; ct=ctb;
	ntb=0;
	while(*cs)
	{
		if(*cs==':')
		{
			cs++;

			*ct++=0;
			tba[ntb]=ctb;
			ntb++;
			ctb=ct;
			continue;
		}
		
		*ct++=*cs++;
	}
	
	*ct=0;
	if(*ctb)
	{
		tba[ntb]=ctb;
		ntb++;
		ctb=ct;
	}
	
	ctx->npathlst=ntb;
	return(0);
}

int TK_EnvCtx_SplitVar(char *str, char *bvar, char **rval)
{
	char *cs, *ct;
	
	cs=str;
	ct=bvar;
	
	while(*cs && (*cs!='='))
		{ *ct++=*cs++; }
	*ct=0;
	
	if(*cs=='=')
		cs++;
	*rval=cs;
	return(0);
}

int TK_EnvCtx_GetEnvVar(TK_EnvContext *ctx, char *varn, char *bufv, int sz)
{
	if(!strcmp(varn, "CWD"))
	{
		TK_EnvCtx_GetCwd(ctx, bufv, sz);
		return(1);
	}

	if(!strcmp(varn, "PATH"))
	{
		TK_EnvCtx_GetEnvVarI(ctx, varn, bufv, sz);
		return(1);
	}

	TK_EnvCtx_GetEnvVarI(ctx, varn, bufv, sz);
	return(1);
}

int TK_EnvCtx_SetEnvVar(TK_EnvContext *ctx, char *varn, char *varv)
{
	if(!strcmp(varn, "CWD"))
	{
		TK_EnvCtx_SetCwd(ctx, varv);
		return(1);
	}
	
	if(!strcmp(varn, "PATH"))
	{
		TK_EnvCtx_SetPath(ctx, varv);
		TK_EnvCtx_SetEnvVarI(ctx, varn, varv);
		return(1);
	}
	
	TK_EnvCtx_SetEnvVarI(ctx, varn, varv);
}

int TK_EnvCtx_UpdateForSet(TK_EnvContext *ctx, char *estr)
{
	char tbn[64];
	char *tbv;

	tbv=NULL;
	TK_EnvCtx_SplitVar(estr, tbn, &tbv);
	TK_EnvCtx_SetEnvVar(ctx, tbn, tbv);

	return(0);
}

int TK_EnvCtx_GetEnvListBuffer(TK_EnvContext *ctx, void *buf, int szbuf)
{
	char **env;
	char *ct;
	int menv;
	
	env=buf;
	menv=ctx->nenvlst;
	ct=((char *)buf)+(menv+2)*sizeof(char *);

	return(0);
}

int TK_EnvCtx_InitForEnv(TK_EnvContext *ctx, char **envv)
{
	char **csa;
	
	csa=envv;
	while(*csa)
	{
		TK_EnvCtx_UpdateForSet(ctx, *csa++);
	}
	
	return(0);
}
