/*
Environment Variable related State.
 */

char *tk_env_cwd;

char *tk_env_pathbuf;
char **tk_env_pathlst;
int tk_env_npathlst;


char *tk_env_envbufs;
char *tk_env_envbufe;
char *tk_env_envbufc;
char **tk_env_envlst_var;
char **tk_env_envlst_val;
int tk_env_nenvlst;
int tk_env_menvlst;

char *TK_Env_SetEnvVarI_StrDup(char *val)
{
	char *ct;
	int l;
	
	l=strlen(val);
	if((tk_env_envbufc+l)>=tk_env_envbufe)
		return(NULL);

	ct=tk_env_envbufc;
	tk_env_envbufc=ct+l+1;
	strcpy(ct, val);
	return(ct);
}

int TK_Env_RepackEnvbuf()
{
	char *oldenv;
	char *cn0, *cv0, *cn1, *cv1;
	int i, j, k;

	oldenv=tk_env_envbufs;
	
	k=tk_env_envbufe-tk_env_envbufs;
	k=k+(k>>1);
	tk_env_envbufs=tk_malloc(k);
	tk_env_envbufe=tk_env_envbufs+k;
	tk_env_envbufc=tk_env_envbufs;
	
	for(i=0; i<tk_env_nenvlst; i++)
	{
		cn0=tk_env_envlst_var[i];
		cv0=tk_env_envlst_val[i];
		cn1=TK_Env_SetEnvVarI_StrDup(cn0);
		cv1=TK_Env_SetEnvVarI_StrDup(cv0);
		tk_env_envlst_var[i]=cn1;
		tk_env_envlst_val[i]=cv1;
	}
	
	tk_free(oldenv);

	return(0);
}

int TK_Env_SetEnvVarI(char *varn, char *varv)
{
	char *cn1, *cv1;
	int i, j, k;

	if(!tk_env_envbufs)
	{
		tk_env_envbufs=tk_malloc(65536);
		tk_env_envbufe=tk_env_envbufs+65536;
		tk_env_envbufc=tk_env_envbufs;

		tk_env_envlst_var=tk_malloc(256*sizeof(char *));
		tk_env_envlst_val=tk_malloc(256*sizeof(char *));
		tk_env_nenvlst=0;
		tk_env_menvlst=256;
	}
	
	for(i=0; i<tk_env_nenvlst; i++)
	{
		if(!strcmp(tk_env_envlst_var[i], varn))
		{
			cv1=TK_Env_SetEnvVarI_StrDup(varv);
			
			if(cv1)
			{
				tk_env_envlst_val[i]=cv1;
				return(1);
			}
			break;
		}
	}
	
	if(i>=tk_env_nenvlst)
	{
		if((tk_env_nenvlst+1)>=tk_env_menvlst)
		{
			k=tk_env_menvlst; k=k+(k>>1);
			tk_env_envlst_var=tk_realloc(
				tk_env_envlst_var, k*sizeof(char *));
			tk_env_envlst_val=tk_realloc(
				tk_env_envlst_val, k*sizeof(char *));
			tk_env_menvlst=k;
		}

		cn1=TK_Env_SetEnvVarI_StrDup(varn);
		cv1=TK_Env_SetEnvVarI_StrDup(varv);

		if(cn1 && cv1)
		{
			i=tk_env_nenvlst++;
			tk_env_envlst_var[i]=cn1;
			tk_env_envlst_val[i]=cv1;
			return(1);
		}
	}

	TK_Env_RepackEnvbuf();
	TK_Env_SetEnvVarI(varn, varv);
	return(1);
}

int TK_Env_SetCwd(char *cwd)
{
	if(cwd)
		tk_env_cwd=TKMM_LVA_Strdup(cwd);
	else
		tk_env_cwd=NULL;
}

char *TK_Env_GetCwd(char *buf, int sz)
{
	char *cwd;
	int l;
	
	cwd=tk_env_cwd;
	if(!cwd)cwd="/";
	
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

int TK_Env_GetPathList(char ***rlst, int *rnlst)
{
	*rlst=tk_env_pathlst;
	*rnlst=tk_env_npathlst;
	return(1);
}

int TK_Env_SetPath(char *path)
{
//	char tb[1024];
	char **tba;
	int ntb;
	
	char *cs, *ct, *csb, *ctb;
	
	if(!tk_env_pathbuf)
		tk_env_pathbuf=malloc(65536);
	if(!tk_env_pathlst)
		tk_env_pathlst=malloc(256*sizeof(char *));
	
	tba=tk_env_pathlst;
	ctb=tk_env_pathbuf;
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
		continue;
	}
	
	tk_env_npathlst=ntb;
	return(0);
}

int TK_Env_SplitVar(char *str, char *bvar, char **rval)
{
	char *cs, *ct;
	
	cs=str;
	ct=bvar;
	
	while(*cs && (*cs!='='))
		{ *ct++=*cs++; }
	*ct=0;
	
	if(*cs=='=')
		cs++;
	rval=cs;
	return(0);
}

int TK_Env_UpdateForSet(char *estr)
{
	char tbn[64];
	char *tbv;

	TK_Env_SplitVar(estr, tbn, &tbv);
	
	if(!strcmp(tbn, "CWD"))
	{
		TK_Env_SetCwd(tbv);
		return(1);
	}
	
	if(!strcmp(tbn, "PATH"))
	{
		TK_Env_SetPath(tbv);
		TK_Env_SetEnvVarI(tbn, tbv);
		return(1);
	}
	
	TK_Env_SetEnvVarI(tbn, tbv);

	return(0);
}

int TK_Env_GetEnvListBuffer(void *buf, int szbuf)
{
	char **env;
	char *ct;
	int menv;
	
	env=buf;
	menv=tk_env_nenvlst;
	ct=((char *)buf)+(menv+2)*sizeof(char *);

	return(0);
}

int TK_Env_InitForEnv(char **envv)
{
	char **csa;
	
	csa=envv;
	while(*csa)
	{
		TK_Env_UpdateForSet(*csa++);
	}
	
	return(0);
}
