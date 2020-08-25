int TK_Env_GetEnvVarIdx(int idx, char *bufn, char *bufv, int szn, int szv)
{
	TK_EnvContext *env;
	TK_SysArg ar[8];
	void *p;
	int i;

	if(tk_iskernel())
	{
		env=TK_GetCurrentEnvContext();
		return(TK_EnvCtx_GetEnvVarIdx(env, idx, bufn, bufv, szn, szv));
	}else
	{
		p=NULL;
		ar[0].i=idx;
		ar[1].p=bufn;
		ar[2].p=bufv;
		ar[3].i=szn;
		ar[4].i=szv;
		tk_syscall(NULL, TK_UMSG_GETENVVARIDX, &i, ar);
		return(i);
	}
}

int TK_Env_GetEnvVarI(char *varn, char *buf, int sz)
{
	TK_EnvContext *env;
	TK_SysArg ar[8];
	void *p;
	int i;

	if(tk_iskernel())
	{
		env=TK_GetCurrentEnvContext();
		return(TK_EnvCtx_GetEnvVar(env, varn, buf, sz));
	}else
	{
		ar[0].p=varn;
		ar[1].p=buf;
		ar[2].i=sz;
		tk_syscall(NULL, TK_UMSG_GETENVVAR, &i, ar);
		return(i);
	}
}

int TK_Env_SetEnvVarI(char *varn, char *varv)
{
	TK_EnvContext *env;
	TK_SysArg ar[8];
	void *p;
	int i;

	if(tk_iskernel())
	{
		env=TK_GetCurrentEnvContext();
		return(TK_EnvCtx_SetEnvVar(env, varn, varv));
	}else
	{
		ar[0].p=varn;
		ar[1].p=varv;
		tk_syscall(NULL, TK_UMSG_SETENVVAR, &i, ar);
		return(i);
	}
}

int TK_Env_SetCwd(char *cwd)
{
//	TK_EnvContext *env;
//	env=TK_GetCurrentEnvContext();
//	return(TK_EnvCtx_SetCwd(env, cwd));
	TK_Env_SetEnvVarI("CWD", cwd);
}

char *TK_Env_GetCwd(char *buf, int sz)
{
//	TK_EnvContext *env;
//	env=TK_GetCurrentEnvContext();
//	if(!env)
//		return("/");
//	return(TK_EnvCtx_GetCwd(env, buf, sz));
	TK_Env_GetEnvVarI("CWD", buf, sz);
}

int TK_Env_GetCwdQualifyName(char *buf, int sz, char *fn)
{
	char tcwd[256];
	char tnam[256];

	if(*fn=='/')
	{
		strcpy(buf, fn);
		return(1);
	}
	
	TK_Env_GetCwd(tcwd, 256);
	strcpy(tnam, tcwd);
	strcat(tnam, "/");
	strcat(tnam, fn);
	
	strcpy(buf, tnam);
	return(1);
}

int TK_Env_GetPathList(char ***rlst, int *rnlst)
{
	TK_EnvContext *env;

	if(tk_iskernel())
	{
		env=TK_GetCurrentEnvContext();
		return(TK_EnvCtx_GetPathList(env, rlst, rnlst));
	}
	else
	{
		return(-1);
	}
}

int TK_Env_SetPath(char *cwd)
{
//	TK_EnvContext *env;
//	env=TK_GetCurrentEnvContext();
//	return(TK_EnvCtx_SetPath(env, cwd));
	TK_Env_SetEnvVarI("PATH", cwd);
}

int TK_Env_GetEnvVar(char *varn, char *buf, int sz)
{
	return(TK_Env_GetEnvVarI(varn, buf, sz));
}

int TK_Env_SetEnvVar(char *varn, char *varv)
{
#if 0
	if(!strcmp(varn, "CWD"))
	{
		TK_Env_SetCwd(varv);
		return(1);
	}
	
	if(!strcmp(varn, "PATH"))
	{
		TK_Env_SetPath(varv);
		TK_Env_SetEnvVarI(varn, varv);
		return(1);
	}
#endif
	
	TK_Env_SetEnvVarI(varn, varv);

	return(0);
}

int TK_Env_UpdateForSet(char *estr)
{
	char tbn[64];
	char *tbv;

	TK_EnvCtx_SplitVar(estr, tbn, &tbv);
	TK_Env_SetEnvVar(tbn, tbv);
}
