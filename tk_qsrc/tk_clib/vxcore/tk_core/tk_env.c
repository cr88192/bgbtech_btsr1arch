int TK_Env_GetEnvVarI(char *varn, char *buf, int sz)
{
	TK_EnvContext *env;
	env=TK_GetCurrentEnvContext();
	return(TK_EnvCtx_GetEnvVarI(env, varn, buf, sz));
}

int TK_Env_SetEnvVarI(char *varn, char *varv)
{
	TK_EnvContext *env;
	env=TK_GetCurrentEnvContext();
	return(TK_EnvCtx_SetEnvVarI(env, varn, varv));
}

int TK_Env_SetCwd(char *cwd)
{
	TK_EnvContext *env;
	env=TK_GetCurrentEnvContext();
	return(TK_EnvCtx_SetCwd(env, cwd));
}

char *TK_Env_GetCwd(char *buf, int sz)
{
	TK_EnvContext *env;
	env=TK_GetCurrentEnvContext();
	return(TK_EnvCtx_GetCwd(env, buf, sz));
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
	env=TK_GetCurrentEnvContext();
	return(TK_EnvCtx_GetPathList(env, rlst, rnlst));
}

int TK_Env_SetPath(char *cwd)
{
	TK_EnvContext *env;
	env=TK_GetCurrentEnvContext();
	return(TK_EnvCtx_SetPath(env, cwd));
}

int TK_Env_SetEnvVar(char *varn, char *varv)
{
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
	
	TK_Env_SetEnvVarI(varn, varv);

	return(0);
}

int TK_Env_UpdateForSet(char *estr)
{
	char tbn[64];
	char *tbv;

	TK_Env_SplitVar(estr, tbn, &tbv);
	TK_Env_SetEnvVar(tbn, tbv);
}
