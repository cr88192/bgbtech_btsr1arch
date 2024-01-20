#include <tk_core.h>
#include <tkgdi/tkgdi.h>

int TK_Env_GetEnvVarIdx(int idx, char *bufn, char *bufv, int szn, int szv)
{
	TK_EnvContext *env;
	TK_SysArg ar[8];
	void *p;
	int i;

	if(tk_iskernel())
	{
		env=TK_GetCurrentEnvContext();
		if(!env)
		{
			*bufn=0;
			*bufv=0;
			return(-1);
		}

		return(TK_EnvCtx_GetEnvVarIdx(env, idx, bufn, bufv, szn, szv));
	}else
	{
		p=NULL;
		i=0;
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
		if(!env)
		{
			tk_printf("TK_Env_GetEnvVarI: No Env\n");
			*buf=0;
			return(-1);
		}

		return(TK_EnvCtx_GetEnvVar(env, varn, buf, sz));
	}else
	{
		i=-1;
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
		i=-1;
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

//	tk_printf("TK_Env_GetCwdQualifyName: cwd=%s\n", tcwd);
	
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

	tbv=NULL;
	TK_EnvCtx_SplitVar(estr, tbn, &tbv);
	TK_Env_SetEnvVar(tbn, tbv);
}


void TKSH_NormalizePath(char *dst, char *src)
{
	char pbuf[512];
	char tb[256];
	char *stk[64];
	char *cs, *ct, *cp;
	int i, n;
	
	cs=src;

	while(*cs=='/')
		cs++;

	n=0;
	ct=pbuf;
	stk[n]=ct;
	cp=ct;
	while(1)
	{
		if((*cs=='/') || !(*cs))
		{
			*ct++=0;
			while(*cs=='/')
				cs++;
			
			if(!strcmp(cp, "."))
			{
				ct=stk[n];
			}else if(!strcmp(cp, ".."))
			{
				if(n>0)n--;
				ct=stk[n];
			}else
			{
				n++;
			}

			stk[n]=ct;
			cp=ct;
			if(!(*cs))
				break;
			continue;
		}
		
		*ct++=*cs++;
	}

	ct=dst;
	*ct++='/';
	for(i=0; i<n; i++)
	{
		cs=stk[i];
		while(*cs)
			*ct++=*cs++;
		if((i+1)<n)
			*ct++='/';
	}
	*ct++=0;
}


void *TK_DlGetApiContextB(TKPE_TaskInfo *task, u64 apiname, u64 subname);

void *TK_DlGetApiContextA(u64 apiname, u64 subname)
{
	TK_SysArg ar[4];
	void *p;
	int tid;
	
//	return(NULL);
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		p=TK_DlGetApiContextB((TKPE_TaskInfo *)TK_GET_TBR, apiname, subname);
		return(p);
	}
#endif

	p=0;
	ar[0].l=apiname;
	ar[1].l=subname;
	tk_syscall(NULL, TK_UMSG_GETAPICTX, &p, ar);
	return(p);
}

_tkgdi_context_t *TKGDI_GetCurrentGdiContext()
{
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *tusr;
	_tkgdi_context_t *ctx;

//	task=TK_GetCurrentTask();
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	if(!task)
		return(NULL);
	
	tusr=(TKPE_TaskInfoUser *)(task->usrptr);
	if(!tusr)
		return(NULL);
	
	ctx=(_tkgdi_context_t *)(tusr->gdictx);
	if(ctx)
		{ return(ctx); }

	ctx=TK_DlGetApiContextA(TK_FCC_GDI, TK_FCC_GDI);
	tusr->gdictx=(tk_kptr)ctx;
	return(ctx);
}
