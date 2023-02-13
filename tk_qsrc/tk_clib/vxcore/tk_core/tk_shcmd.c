#ifndef FOURCC
#define FOURCC(a, b, c, d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))
#endif

#ifndef TWOCC
#define TWOCC(a, b)				((a)|((b)<<8))
#endif

#define TCC_HASHBANG	TWOCC('#', '!')

#define TCC_PE			TWOCC('P', 'E')

#define FCC_PLFW		FOURCC('P', 'L', 'F', 'W')

#define FCC_TKPE		FOURCC('T', 'K', 'P', 'E')
#define FCC_TKTX		FOURCC('T', 'K', 'T', 'X')

#define FCC_B2DA		FOURCC('B', '2', 'D', 'A')
#define FCC_BJX2		FOURCC('B', 'J', 'X', '2')
#define FCC_SCMD		FOURCC('S', 'C', 'M', 'D')

#ifdef __BJX2__
#define PLF_FCC_CURARCH		FCC_B2DA
#define PLF_FCC_GENARCH		FCC_BJX2
#endif

int TKSH_TryLoad(char *img, char **args);
int TKSH_TryLoad_n(char *img, char **args);

extern volatile u64 __arch_gbr;

extern TKPE_TaskInfo *tk_task_syscall;
extern u64	tk_vmem_pageglobal;


TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2);

TKSH_CommandInfo *TKSH_LookupCommand(char *name);
TKSH_CommandInfo *TKSH_CreateCommand(char *name);


TKSH_CommandInfo *tksh_commands=NULL;
TKSH_CommandInfo *tksh_command_hash[64];
int tksh_commands_init;

int tksh_cmdentry;

#include "tk_shcmd_ed.c"
#include "tk_shcmd_hex.c"

TKSH_EditCtx *tksh_editctx=NULL;
	

int TKSH_HashFast(char *name)
{
	int i;
	i=*(u16 *)name;
	i=i*251+1;
	i=i*251+1;
	i=(i>>16)&63;
	return(i);
}

TKSH_CommandInfo *TKSH_LookupCommand(char *name)
{
	TKSH_CommandInfo *cur;
	int h;
	
	if(!name)
		return(NULL);
	if(!(name[0]))
		return(NULL);
	
	h=TKSH_HashFast(name);
	
//	cur=tksh_commands;
	cur=tksh_command_hash[h];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
//		cur=cur->next;
		cur=cur->hnext;
	}
	return(NULL);
}

TKSH_CommandInfo *TKSH_CreateCommand(char *name)
{
	TKSH_CommandInfo *cur;
	int h;
	
	if(!name)
		return(NULL);
	if(!(name[0]))
		return(NULL);
	
	cur=TKSH_LookupCommand(name);
	if(cur)
		return(cur);

	h=TKSH_HashFast(name);

	cur=tk_malloc(sizeof(TKSH_CommandInfo));
	cur->name=tk_strdup_in(name);
	
	cur->next=tksh_commands;
	tksh_commands=cur;

	cur->hnext=tksh_command_hash[h];
	tksh_command_hash[h]=cur;
	
	return(cur);
}

TKSH_CommandInfo *TKSH_RegisterCommand(char *name, void *fcn)
{
	TKSH_CommandInfo *cmdi;

	cmdi=TKSH_CreateCommand(name);
	cmdi->Cmd=fcn;
	return(cmdi);
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

void TKSH_ListDir(char *path, char **args)
{
	char tb[256];
	TK_DIR *dir;
	TK_DIRENT **tdearr;
	TK_DIRENT *tde, *tde2;
	char *s0, *cs, *ct, *cte, *altn;
	int flag, num, max, mw, mwc, nrow;
	int i, j, k;
	
	flag=0;
	for(i=0; args[i]; i++)
	{
		s0=args[i];
		if(s0[0]=='-')
		{
			if(s0[1]=='-')
			{
				continue;
			}
			
			s0++;
			while(*s0)
			{
				if(*s0=='a')flag|=1;
				if(*s0=='l')flag|=2;
				if(*s0=='t')flag|=4;
				s0++;
			}
		}
	}
	
	dir=tk_opendir(path);
	if(!dir)
	{
		return;
	}

#if 0
	tde=tk_readdir(dir);
	while(tde)
	{
//		memset(tb, 0x20, 128);
//		memcpy(tb, tde->d_name, strlen(tde->d_name));
//		tb[32]=0;
//		tk_printf("%032s %8d\n", tde->d_name, tde->st_size);
//		printf("%032s %9d\n", tb, tde->st_size);
		printf("%-32s %9d %08X\n", tde->d_name, tde->st_size, tde->st_mtime);
		tde=tk_readdir(dir);
	}
#endif

#if 1
	tdearr=tk_malloc(128*sizeof(TK_DIRENT *));
	max=128;
	mw=1;

	num=0;
	tde=tk_readdir(dir);
	while(tde)
	{
		if((num+1)>=max)
		{
			max=max+(max>>1);
			tdearr=tk_realloc(tdearr, 128*sizeof(TK_DIRENT *));
		}
		
		if(!(flag&1))
		{
			if(tde->d_name[0]=='.')
			{
				tde=tk_readdir(dir);
				continue;
			}
		}
		
		k=strlen(tde->d_name);
		if(k>=mw)
			mw=k+1;
		
		tde2=tk_malloc(sizeof(TK_DIRENT));
		memcpy(tde2, tde, sizeof(TK_DIRENT));
		tdearr[num++]=tde2;
	
		tde=tk_readdir(dir);
	}
	
	mwc=72/mw;
	nrow=(num+(mwc-1))/mwc;
#endif
	
	if(flag&2)
	{
		for(i=0; i<num; i++)
		{
			tde=tdearr[i];
			
			altn="";
			if((tde->st_mode&TKFAT_EMODE_IFMT)==TKFAT_EMODE_LINK)
				altn=tde->st_link;
			
			printf("%-32s %9d %08X %s\n",
				tde->d_name,
				tde->st_size,
				tde->st_mtime, altn);
		}
	}else
	{
		for(i=0; i<nrow; i++)
		{
			for(j=0; j<mwc; j++)
			{
				k=i*mwc+j;
				if(k>=num)
					break;

				tde=tdearr[k];
				
				cs=tde->d_name;
				ct=tb; cte=tb+mw;
				
				while(*cs && ct<cte)
					{ *ct++=*cs++; }
				while(ct<cte)
					{ *ct++=' '; }
				*ct++=0;
				
//				if(tde->st_mode&TKFAT_EMODE_DIR)
				if((tde->st_mode&TKFAT_EMODE_IFMT)==TKFAT_EMODE_DIR)
//					tk_puts("\x1B[34m");
					tk_puts("\x1B[36m");

//				if(tde->st_mode&TKFAT_EMODE_LINK)
				if((tde->st_mode&TKFAT_EMODE_IFMT)==TKFAT_EMODE_LINK)
					tk_puts("\x1B[35m");

//				if(tde->st_mode&TKFAT_EMODE_ACC_XO)
//					tk_puts("\x1B[32m");
				
				tk_printf("%s", tb);
				tk_puts("\x1B[39m");
			}
			tk_printf("\n");
		}
	}
}

int TKSH_Cmds_Cls(char **args)
{
	tk_con_reset();
	return(0);
}

int TKSH_Cmds_Chdir(char **args)
{
	char tb_cwd[256];
	char tb[256];
	TK_DIR *dir;
	char *darg;
	int i;

	TK_Env_GetCwd(tb_cwd, 256);
	
	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

	if(darg)
	{
		if(darg[0]=='/')
		{
			strcpy(tb, darg);
		}else
		{
			strcpy(tb, tb_cwd);
			if(tb[strlen(tb)-1]!='/')
				strcat(tb, "/");
			strcat(tb, darg);
		}
		TKSH_NormalizePath(tb_cwd, tb);

		dir=tk_opendir(tb_cwd);
		if(!dir)
		{
			tk_printf("No such directory %s\n", tb_cwd);
			return(-1);
		}
		tk_closedir(dir);

		TK_Env_SetCwd(tb_cwd);
	}else
	{
		strcpy(tb_cwd, "/");
		TK_Env_SetCwd(tb_cwd);
	}
	return(0);
}

int TKSH_Cmds_Echo(char **args)
{
	char tb1[256];
	char *s0, *s1;
	int i;

	for(i=1; args[i]; i++)
	{
		s0=args[i];
		
		if(*s0=='$')
		{
			s1=TK_Env_GetEnvVarI(s0+1, tb1, 256);
			if(s1)
				{ tk_puts(tb1); }
			continue;
		}
		
		tk_puts(s0);
	}
	tk_putc('\n');
	return(0);
}

int THSH_CheckIsURI(char *src)
{
	char *s1;
	int isuri;

	isuri=0;
	s1=src;
	while(*s1 && (((*s1>='a') && (*s1<='z')) || ((*s1>='A') && (*s1<='Z'))))
		s1++;
	if(s1==':')
		isuri=1;
	return(isuri);
}

int THSH_QualifyPathArg(char *dst, char *src)
{
	char tb_cwd[256];
	int isuri;

	if(src)
	{
		if((src[0]=='.') && (src[1]=='/'))
			src+=2;
		
		isuri=THSH_CheckIsURI(src);
		
		if((src[0]=='/') || isuri)
		{
			strcpy(dst, src);
		}else
		{
			TK_Env_GetCwd(tb_cwd, 256);
			if(!tb_cwd[0] || !strcmp(tb_cwd, "/"))
			{
				strcpy(dst, "/");
				strcat(dst, src);
			}else
			{
				strcpy(dst, tb_cwd);
				strcat(dst, "/");
				strcat(dst, src);
			}
		}
	}else
	{
		TK_Env_GetCwd(dst, 256);
	}
	
	TKSH_NormalizePath(dst, dst);
	
	return(0);
}

int TKSH_Cmds_Ls(char **args)
{
	char tb[256];
	char *darg, *s;
	int i, dohelp;

	darg=NULL;
	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			if(args[i][1]!='-')
			{
				s=args[i]+1;
				while(*s)
				{
					if(*s=='h')
						{ dohelp=1; }
					s++;
				}
			}
			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] [target]\n", args[0]);
		tk_printf("List information about a file or directory\n");
		tk_printf("-a		Show all entries\n");
		tk_printf("-l		Use long listing format\n");
		tk_printf("-t		Sort by modification time (newest first)\n");
		return(0);
	}

	THSH_QualifyPathArg(tb, darg);
	TKSH_ListDir(tb, args+1);

//	if(darg)
//	{
//		THSH_QualifyPathArg(tb, darg);
//		TKSH_ListDir(tb, args+1);
//	}else
//	{
//		TKSH_ListDir(tb_cwd, args+1);
//	}
	return(0);
}

int TKSH_Cmds_Mv(char **args)
{
	char tb1[256];
	char tb2[256];
	char *darg, *sarg, *s;
	int i, dohelp;

	dohelp=0;
	darg=NULL;
	sarg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			if(args[i][1]!='-')
			{
				s=args[i]+1;
				while(*s)
				{
					if(*s=='h')
						{ dohelp=1; }
					s++;
				}
			}
			continue;
		}
		if(!sarg)
			{ sarg=args[i]; continue; }
		if(!darg)
			{ darg=args[i]; continue; }
	}
	
	if(dohelp)
	{
		tk_printf("Usage: %s [options] source destination\n", args[0]);
		tk_printf("Move file from source to destination\n");
		return(0);
	}

	if(sarg && darg)
	{
		THSH_QualifyPathArg(tb1, sarg);
		THSH_QualifyPathArg(tb2, darg);
		tk_rename(tb1, tb2, "r");
//		tk_unlink(tb);
	}else
	{
		tk_printf("%s: invalid arguments\n", args[0]);
	}
	return(0);
}

int TKSH_Cmds_Ln(char **args)
{
	char tb1[256];
	char tb2[256];
	char *darg, *sarg, *mode;
	char *s;
	int i, dohelp;

	darg=NULL;
	sarg=NULL;
	mode="l";
	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			if(!strcmp(args[i], "--symbolic"))
				{ mode="S"; continue; }

			if(args[i][1]!='-')
			{
				s=args[i]+1;
				while(*s)
				{
					if(*s=='s')
						{ mode="S"; }
					if(*s=='h')
						{ dohelp=1; }
					s++;
				}
			}

			continue;
		}
		if(!sarg)
			{ sarg=args[i]; continue; }
		if(!darg)
			{ darg=args[i]; continue; }
	}
	
	if(dohelp)
	{
		tk_printf("Usage: %s [options] source destination\n", args[0]);
		tk_printf("Create link at destination pointing at source\n");
		return(0);
	}

	if(sarg && darg)
	{
		if(*mode=='S')
		{
			strcpy(tb1, sarg);
		}else
		{
			THSH_QualifyPathArg(tb1, sarg);
		}
		THSH_QualifyPathArg(tb2, darg);
		tk_rename(tb1, tb2, mode);
//		tk_unlink(tb);
	}else
	{
		tk_printf("%s: invalid arguments\n", args[0]);
	}
	return(0);
}

int TKSH_Cmds_Cp(char **args)
{
	char tb1[256];
	char tb2[256];
	char *darg, *sarg, *s;
	int i, dohelp;

	darg=NULL;
	sarg=NULL;
	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			if(args[i][1]!='-')
			{
				s=args[i]+1;
				while(*s)
				{
					if(*s=='h')
						{ dohelp=1; }
					s++;
				}
			}

			continue;
		}
		if(!sarg)
			{ sarg=args[i]; continue; }
		if(!darg)
			{ darg=args[i]; continue; }
	}
	
	if(dohelp)
	{
		tk_printf("Usage: %s [options] source destination\n", args[0]);
		tk_printf("Copies file from source to destination\n");
		return(0);
	}

	if(sarg && darg)
	{
		THSH_QualifyPathArg(tb1, sarg);
		THSH_QualifyPathArg(tb2, darg);
		tk_fcopy(tb1, tb2);
	}else
	{
		tk_printf("%s: invalid arguments\n", args[0]);
	}
	return(0);
}

int TKSH_Cmds_RecvXm(char **args)
{
	char tb_cwd[256];
	char tb[256];
	char *darg;
	int i;

	TK_Env_GetCwd(tb_cwd, 256);
	
	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		TK_Dbg_RecvFileXM(tb);
	}else
	{
		TK_Dbg_RecvFileXM(tb);
	}
	return(0);
}

int TKSH_Cmds_Rm(char **args)
{
	char tb[256];
	char *darg;
	int i;

	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		tk_unlink(tb);
	}else
	{
	}
	return(0);
}

int TKSH_Cmds_Rmdir(char **args)
{
	char tb[256];
	char *darg;
	int i;

	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		tk_rmdir(tb);
	}else
	{
	}
	return(0);
}

int TKSH_Cmds_Mkdir(char **args)
{
	char tb[256];
	char *darg;
	int i;

	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		tk_mkdir(tb, "");
	}else
	{
	}
	return(0);
}

int TKSH_Cmds_Set(char **args)
{
	char tb1[256];
	char tb2[256];
	int i, j;

	if(args[1])
	{
		TK_Env_UpdateForSet(args[1]);
	}else
	{
		for(i=0;; i++)
		{
			j=TK_Env_GetEnvVarIdx(i, tb1, tb2, 256, 256);
			if(j<=0)break;
			tk_printf("%s=%s\n", tb1, tb2);
		}
	}
	return(0);
}

int TKSH_Cmds_Mount(char **args)
{
	TK_MOUNT *mcur;
	char *devfn, *mntfn, *fsty, *mode, *opts;
	char *s0, *s1, *s2;
	char **opta;
	int i;
	
	devfn=NULL;
	mntfn=NULL;
	fsty=NULL;
	mode=NULL;
	opts=NULL;
	
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(args[i][1]=='t')
			{
				if(args[i][2])
				{
					fsty=args[i]+2;
				}else
				{
					fsty=args[i+1];
					i++;
				}
				continue;
			}

			if(args[i][1]=='o')
			{
				if(args[i][2])
				{
					opts=args[i]+2;
				}else
				{
					opts=args[i+1];
					i++;
				}
				continue;
			}
			
			if(args[i][1]=='m')
			{
				if(args[i][2])
				{
					mode=args[i]+2;
				}else
				{
					mode=args[i+1];
					i++;
				}
				continue;
			}
			
			continue;
		}
		if(!devfn)
		{
			devfn=args[i];
			continue;
		}
		if(!mntfn)
		{
			mntfn=args[i];
			continue;
		}
	}
	
	if(!devfn)
	{
		mcur=tk_vf_firstmount();
		while(mcur)
		{
			s0=mcur->tgt;
			s1=mcur->src;
			s2=mcur->vt->fsname;
			if(!s0)s0="(none)";
			if(!s1)s1="/";
			if(!s2)s2="-";
			tk_printf("%s on %s type %s\n", s0, s1, s2);
			mcur=tk_vf_nextmount(mcur);
		}
		return(0);
	}
	
	opta=NULL;
	if(opts)
		opta=tk_rsplit_sep(opts, ',');
	
	if(devfn && mntfn && fsty)
	{
		tk_fmount(devfn, mntfn, fsty, mode, opta);
	}

	return(0);
}

int TKSH_Cmds_Ed(char **args)
{
	char tb[256];
	char *darg;
	int i;

	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

	tksh_cmdentry=1;
	
	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		if(!tksh_editctx)
			tksh_editctx=TKSH_EdAllocContext();
		TKSH_EdLoadFile(tksh_editctx, tb);
	}

	return(0);
}

int TKSH_Cmds_Edit(char **args)
{
	char tb[256];
	char *darg;
	int i;

	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

//	tksh_cmdentry=1;
	
	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		if(!tksh_editctx)
			tksh_editctx=TKSH_EdAllocContext();
		TKSH_EdLoadFile(tksh_editctx, tb);
	}
	
	if(!tksh_editctx)
		tksh_editctx=TKSH_EdAllocContext();
	TKSH_EditUpdateLoop(tksh_editctx);

	tk_con_reset();

	return(0);
}

int TKSH_Cmds_HexEdit(char **args)
{
	char tb[256];
	char *darg;
	int i;

	darg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		darg=args[i];
	}

//	tksh_cmdentry=1;
	
	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		TKSH_HexLoadFile(tb);
	}
	
	TKSH_HexUpdateLoop();

	tk_con_reset();

	return(0);
}

int TKSH_InitCmds(void)
{
	TKSH_CommandInfo *cmdi;
	
	if(tksh_commands_init)
		return(0);
	tksh_commands_init=1;
	
//	cmdi=TKSH_CreateCommand("echo");
//	cmdi->Cmd=TKSH_Cmds_Echo;

//	cmdi=TKSH_CreateCommand("set");
//	cmdi->Cmd=TKSH_Cmds_Set;

	TKSH_RegisterCommand("cd",		TKSH_Cmds_Chdir);
	TKSH_RegisterCommand("chdir",	TKSH_Cmds_Chdir);
	TKSH_RegisterCommand("cls",		TKSH_Cmds_Cls);
	TKSH_RegisterCommand("cp",		TKSH_Cmds_Cp);

	TKSH_RegisterCommand("dir",		TKSH_Cmds_Ls);

	TKSH_RegisterCommand("echo",	TKSH_Cmds_Echo);
	TKSH_RegisterCommand("ed",		TKSH_Cmds_Ed);
	TKSH_RegisterCommand("edit",	TKSH_Cmds_Edit);

	TKSH_RegisterCommand("hexedit",	TKSH_Cmds_HexEdit);

	TKSH_RegisterCommand("ln",		TKSH_Cmds_Ln);
	TKSH_RegisterCommand("ls",		TKSH_Cmds_Ls);

	TKSH_RegisterCommand("md",		TKSH_Cmds_Mkdir);
	TKSH_RegisterCommand("mkdir",	TKSH_Cmds_Mkdir);
	TKSH_RegisterCommand("mount",	TKSH_Cmds_Mount);
	TKSH_RegisterCommand("mv",		TKSH_Cmds_Mv);

	TKSH_RegisterCommand("recvxm",	TKSH_Cmds_RecvXm);
	TKSH_RegisterCommand("rm",		TKSH_Cmds_Rm);
	TKSH_RegisterCommand("rmdir",	TKSH_Cmds_Rmdir);

	TKSH_RegisterCommand("set",		TKSH_Cmds_Set);

	return(1);
}


#if 0

#define TK_CMD_RUN		0x01
#define TK_CMD_LS		0x02
#define TK_CMD_CD		0x03
#define TK_CMD_CLS		0x04
#define TK_CMD_RECVXM	0x05
#define TK_CMD_RM		0x06
#define TK_CMD_MV		0x07
#define TK_CMD_CP		0x08
#define TK_CMD_ECHO		0x09
#define TK_CMD_SET		0x0A

int tk_cmd2idx(char *s)
{
	if(*s=='c')
	{
		if(!strcmp(s, "cd"))
			return(TK_CMD_CD);

		if(!strcmp(s, "cp"))
			return(TK_CMD_CP);

		if(!strcmp(s, "cls"))
			return(TK_CMD_CLS);
	}

	if(*s=='d')
	{
		if(!strcmp(s, "dir"))
			return(TK_CMD_LS);
	}

	if(*s=='e')
	{
		if(!strcmp(s, "echo"))
			return(TK_CMD_ECHO);
	}

	if(*s=='l')
	{
		if(!strcmp(s, "load"))
			return(TK_CMD_RUN);
		if(!strcmp(s, "ls"))
			return(TK_CMD_LS);
	}

	if(*s=='m')
	{
		if(!strcmp(s, "mv"))
			return(TK_CMD_MV);
	}

	if(*s=='r')
	{
		if(!strcmp(s, "run"))
			return(TK_CMD_RUN);

		if(!strcmp(s, "recvxm"))
			return(TK_CMD_RECVXM);

		if(!strcmp(s, "rm"))
			return(TK_CMD_RM);
	}

	if(*s=='s')
	{
		if(!strcmp(s, "set"))
			return(TK_CMD_SET);
	}

	if(*s=='t')
	{
		if(!strcmp(s, "test"))
			return(0);
	}

	return(-1);
}
#endif

int TKSH_IsCmdEntryMode()
{
	return(tksh_cmdentry);
}

int TKSH_ExecCmdEntry(char *cmd)
{
	if(tksh_cmdentry==1)
	{
		if(*cmd=='q')
		{
			tksh_cmdentry=0;
			return(0);
		}
		if(!tksh_editctx)
			tksh_editctx=TKSH_EdAllocContext();
		TKSH_EdParseCommand(tksh_editctx, cmd);
		return(0);
	}
}

int TKSH_ExecCmd(char *cmd)
{
	char tb[256], tb1[256], tb2[256];
	char tb_cwd[256];
	TKSH_CommandInfo *cmdi;
	char **a;
	char *s0, *s1;
	int ci, ri;
	int i, j, k;

	a=tk_rsplit(cmd);
	if(!a[0])
		return(0);
//	ci=tk_cmd2idx(a[0]);
//	TK_Env_GetCwd(tb_cwd, 256);

#if 1
	cmdi=TKSH_LookupCommand(a[0]);
	if(cmdi && (cmdi->Cmd))
	{
		i=cmdi->Cmd(a);
		return(i);
	}

	cmd[0]=0;

	ri=TKSH_TryLoad_n(tb, a);

	if(ri>0)
	{
		if(ri&65535)
			{ tk_printf("Return Status=%d\n", (ri&65535)); }
		return(ri);
	}
	
	tk_printf("Unhandled Command '%s'\n", a[0]);
	return(-1);
#endif

#if 0
	switch(ci)
	{
	case 0:
//			tk_printf("Test Command '%s'\n", a[0]);
		break;

	default:
		cmdi=TKSH_LookupCommand(a[0]);
		if(cmdi && (cmdi->Cmd))
		{
			cmdi->Cmd(a);
			break;
		}

		ri=TKSH_TryLoad_n(tb, a);

		if(ri>0)
		{
			if(ri&65535)
				{ tk_printf("Return Status=%d\n", (ri&65535)); }
			break;
		}
		
		tk_printf("Unhandled Command '%s'\n", a[0]);
		break;
	}
#endif

	return(1);
}

int TKSH_ExecCmdBuf(char *cmd)
{
	byte tb[256];
	char *cs, *ct;
	int i;
	
	cs=cmd;
	while(*cs)
	{
		ct=tb;
		while(*cs && (*cs!='\r') && (*cs!='\n'))
			{ *ct++=*cs++; }
		*ct++=0;
		
		if(tb[0])
		{
			tk_printf("cmd: %s\n", tb);
			TKSH_ExecCmd(tb);
		}

		i=*cs;
		if(i=='\r')i=*(++cs);
		if(i=='\n')cs++;
//		while(*cs && (*cs<=' '))cs++;
	}
}

void *tk_rovalloc(int sz, void **rov)
{
	byte *ptr;
	int sz1;
	
	sz1=(sz+8)&(~7);
	ptr=*rov;
	*rov=ptr+sz1;
	return(ptr);
}

char *tk_rovstrdup(char *str, void **rov)
{
	char *s1;

	s1=tk_rovalloc(strlen(str)+1, rov);
	strcpy(s1, str);
	return(s1);
}

char *tksh_cifyemit(char *dst, char *str)
{
	char *cs, *ct;
	
	ct=dst;		cs=str;
	*ct++='\"';
	while(*cs)
	{
		if(*cs=='\\')
			{ cs++; *ct++='\\';	*ct++='\\'; continue; }
		if(*cs=='\"')
			{ cs++; *ct++='\\';	*ct++='\"'; continue; }
		if(*cs=='\r')
			{ cs++; *ct++='\\';	*ct++='r'; continue; }
		if(*cs=='\n')
			{ cs++; *ct++='\\';	*ct++='n'; continue; }
		if(*cs=='\t')
			{ cs++; *ct++='\\';	*ct++='t'; continue; }
		*ct++=*cs++;
	}
	*ct++='\"';
	return(ct);
}

void __tk_farcall(void *fptr, void *gbr, void *newstack, void *tbr);
int TK_DestroyTaskInfo(void *tkptr);
void TK_FlushCacheL1D();
void TK_FlushCacheL1D_INVIC(void *ptr);

int tk_sysc_exitpt();

int tk_syscall_utxt(void *sobj, int umsg, void *pptr, void *args);
int tk_syscall_rv_utxt(void *sobj, int umsg, void *pptr, void *args);

void tk_isr_syscall_rv();

volatile int tksh_runstate;

int TKSH_TryLoad(char *img, char **args0)
{
	byte tb[1024];
	byte cwd[256];
	char *args[64];
	u64 tlsix[8];
	TK_FILE *fd;
	char **a1;
	char *cs, *ct, *cs1, *ct1;
	char *buf;
//	u64 bootgbr;
	u64	pb_gbr;
	u64	pb_boot;
	u64	pb_sysc;
	TKPE_TaskInfo *task;
	TKPE_TaskInfo *ctask, *ptask;
	TKPE_TaskInfoKern *tkern;
	TK_EnvContext *env0, *env1;
	TKPE_ImageInfo *pimg;
	void *bootgbr;
	byte *boot_newspb, *boot_newsp;
	byte *boot_newspbk, *boot_newspk;
	void *boottbr, *boottls;
	void *sysc;
	int (*bootptr)();
	volatile int chk;
	volatile int *rchk;
	int plf_dofs, plf_dnum, plf_fdofs, plf_fdsz;
	int plf_lofs, plf_lsz, plf_lname1, plf_lname2, plf_lname3;
	int sig_is_pe, sig_is_asc;
	int rv, nl, sz, sza, ix;
	int i, j, k;

	pimg=NULL;
	fd=NULL;
	sig_is_pe=0;

	TK_Env_GetCwd(cwd, 256);
	
	ix=TKPE_TryLoadProgramImage(img, cwd, 2);
	if(ix>0)
	{
		pimg=TK_GetImageForIndex(ix);
//		tk_printf("TKSH_TryLoad: Got Image %d %p\n", ix, pimg);
	}else
	{
//		tk_printf("TKSH_TryLoad: Not Yet Loaded\n");
	}

	if(args0)
	{
		for(i=0; args0[i]; i++)
			args[i]=args0[i];
		args[i]=NULL;
		args[0]=img;
	}else
	{
		args[0]=img;
		args[1]=NULL;
	}

	if(!pimg)
	{
		fd=tk_fopen(img, "rb");
		
		if(!fd)
		{
			tk_printf("Failed to open %s\n", img);
			return(-1);
		}

		while(fd)
		{
			tk_fseek(fd, 0, 0);
	//		memset(tb, 0, 256);
	//		tk_fread(tb, 1, 255, fd);
			memset(tb, 0, 1024);
			tk_fread(tb, 1, 1023, fd);

			plf_lname1=tkfat_getWord(tb);
			plf_lname2=tkfat_getDWord(tb+128);
	//		plf_lname3=tkfat_getDWord(tb+256);

	//		if((tb[0]!='#') || (tb[1]!='!'))
	//			break;

			if(plf_lname1!=TCC_HASHBANG)
				break;
			if(plf_lname2==FCC_PLFW)
				break;
	//		if((plf_lname2==FCC_PLFW) ||
	//			(plf_lname3==FCC_PLFW))
	//				break;

			cs=tb+2; ct=tb;
			while(*cs>=' ')
				*ct++=*cs++;
			*ct++=0;
			
			tk_fclose(fd);
			fd=tk_fopen(tb, "rb");
		}

		plf_fdofs=0;
		plf_fdsz=0;
		plf_dnum=0;
		plf_dofs=0;

	#if 1
		plf_lname1=tkfat_getDWord(tb+0);
		plf_lname2=tkfat_getDWord(tb+128);
		plf_lname3=tkfat_getDWord(tb+256);

	//	if((tb[0]=='P') && (tb[1]=='L') && (tb[2]=='F') && (tb[3]=='W'))
		if(plf_lname1==FCC_PLFW)
		{
			plf_dnum=tkfat_getDWord(tb+4);
			plf_dofs=tkfat_getDWord(tb+8);
		}else if(plf_lname2==FCC_PLFW)
		{
			plf_dnum=tkfat_getDWord(tb+(128+4));
			plf_dofs=tkfat_getDWord(tb+(128+8));
		}
	#if 1
		else if(plf_lname3==FCC_PLFW)
		{
			plf_dnum=tkfat_getDWord(tb+(256+4));
			plf_dofs=tkfat_getDWord(tb+(256+8));
		}
	#endif
	#endif

	#if 0
		cs=(char *)tb;
		for(i=0; i<6; i++)
		{
			plf_lname1=tkfat_getDWord(cs);
			if(plf_lname1==FCC_PLFW)
			{
				plf_dnum=tkfat_getDWord(cs+4);
				plf_dofs=tkfat_getDWord(cs+8);
				break;
			}
			if(((u16)plf_lname1)==TCC_PE)
				break;
			cs+=128;
		}
	#endif

		
		if(plf_dnum>0)
		{
			for(i=plf_dnum-1; i>=0; i--)
			{
				cs=(char *)tb+plf_dofs+(i*16);
				plf_lofs	= tkfat_getDWord(cs+ 0);
				plf_lsz		= tkfat_getDWord(cs+ 4);
				plf_lname1	= tkfat_getDWord(cs+ 8);
				plf_lname2	= tkfat_getDWord(cs+12);
				
				if(plf_lname1==FCC_TKPE)
				{
					if(plf_lname2==PLF_FCC_CURARCH)
						{ plf_fdofs=plf_lofs; plf_fdsz=plf_lsz; break; }
					if(plf_lname2==PLF_FCC_GENARCH)
						{ plf_fdofs=plf_lofs; plf_fdsz=plf_lsz; break; }
				}

				if(plf_lname1==FCC_TKTX)
				{
					if(plf_lname2==FCC_SCMD)
						{ plf_fdofs=plf_lofs; plf_fdsz=plf_lsz; break; }
				}
			}
			
			if(i>=0)
			{
				tk_fseek(fd, plf_fdofs, 0);
	//			tk_fread(tb, 1, 255, fd);
				tk_fread(tb, 1, 1024, fd);
			}
		}
		
		sig_is_pe=0;
		if((tb[0]=='M') && (tb[1]=='Z'))
			sig_is_pe=1;
		if((tb[0]=='P') && (tb[1]=='E'))
			sig_is_pe=1;
	}

	chk=999;
	rchk=&chk;
	*rchk=0x1234567;

//	if(fd)
	if(pimg || (fd && sig_is_pe))
	{
		bootgbr=0;
//		TKPE_LoadStaticPE(fd, plf_fdofs, &bootptr, &bootgbr, tlsix);
//		tk_printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);

		if(!pimg)
			pimg=TKPE_LoadDynPE(fd, 0, img, NULL, 0);

		bootptr=pimg->bootptr;

		if(fd)
		{
			tk_fclose(fd);
			fd=NULL;
		}

#if 0
		if(bootgbr)
		{
			pb_gbr=*(u64 *)bootgbr;
			if(((u64)pb_gbr)!=((u64)bootgbr))
			{
				tk_printf("Boot GBR Mismatch, Got=%p Expect=%p\n",
					(void *)pb_gbr, (void *)bootgbr);
				__debugbreak();
			}
		}
#endif
		
		if(bootptr)
		{
		
			tksh_runstate = 2;
		
//			boot_newspb=TKMM_PageAlloc(1<<18);
//			boot_newsp=boot_newspb+((1<<18)-1024);

			i=TK_GetRandom16ASLR()&0x03F0;
			boot_newspb=TKMM_PageAlloc(1<<18);
//			boot_newspb=TKMM_PageAlloc(1<<19);
//			boot_newspb=TKMM_PageAlloc(1<<20);
//			boot_newspb=TKMM_PageAllocUsc(1<<19);
//			boot_newspb=TKMM_PageAllocUsc(1<<20);
//			boot_newsp=boot_newspb+((1<<19)-1024);
			boot_newsp=boot_newspb+(((1<<18)-1024)-i);
//			boot_newsp=boot_newspb+(((1<<19)-1024)-i);
//			boot_newsp=boot_newspb+(((1<<20)-1024)-i);

//			i=TK_GetRandom16ASLR()&0x00F0;
//			boot_newspbk=TKMM_PageAlloc(1<<16);
//			boot_newspk=boot_newspb+((1<<16)-1024);
//			boot_newspk=boot_newspbk+((1<<16)-1024);
//			boot_newspk=boot_newspbk+(((1<<16)-1024)-i);

			boot_newspbk=NULL;
			boot_newspk=NULL;

			env0=TK_GetCurrentEnvContext();
			env1=TK_EnvCtx_CloneContext(env0);

			ctask=TK_GetCurrentTask();
			ptask=NULL;

			if(ctask==tk_task_syscall)
			{
				ptask=TK_GetSyscallUserTask();
				if(ptask==tk_task_syscall)
					ptask=NULL;
			}

			boottbr=TK_AllocNewTask();
			task=boottbr;
			tkern=(TKPE_TaskInfoKern *)(task->krnlptr);

#if 1
//			task->baseptr=(tk_kptr)tlsix[6];
			task->baseptr=(tk_kptr)pimg->imgbase;
			task->bootptr=(tk_kptr)bootptr;
			task->basegbr=(tk_kptr)bootgbr;
			task->boottbr=(tk_kptr)boottbr;
#endif

			task->boot_sps=(tk_kptr)boot_newspb;
			task->boot_spe=(tk_kptr)boot_newsp;

			tkern->usr_boot_sps=(tk_kptr)boot_newspb;
			tkern->usr_boot_spe=(tk_kptr)boot_newsp;
			tkern->krn_boot_sps=(tk_kptr)boot_newspbk;
			tkern->krn_boot_spe=(tk_kptr)boot_newspk;
			
			task->envctx=(tk_kptr)env1;
//			task->SysCall=tk_isr_syscall;

//			sysc=tk_isr_syscall;
			sysc=tk_syscall_utxt;

			pb_sysc=(tk_kptr)sysc;
			pb_sysc&=0x0000FFFFFFFFFFFEULL;
			pb_sysc|=0x0000000000000001ULL;
			sysc=(void *)pb_sysc;

			pb_boot=(u64)bootptr;

//			if((bootptr>>50)&1)
//				{ sysc=tk_syscall_rv_utxt; }

			if((pb_boot>>50)&1)
				{ sysc=tk_syscall_rv_utxt; }

			task->SysCall=(tk_kptr)sysc;

#if 0
			if(tlsix[0])
			{
				boottls=TKMM_PageAlloc(tlsix[1]);
				if(tlsix[2])
					{ memcpy(boottls, (void *)(tlsix[2]), tlsix[3]); }
				TK_TlsSetB(task, tlsix[0], boottls);
			}
			
			task->imgbaseptrs=task->img_baseptrs;
			task->imggbrptrs=task->img_gbrptrs;
			task->imgtlsrvas=task->img_tlsrvas;
			
			task->img_baseptrs[0]=(tk_kptr)tlsix[6];
			task->img_gbrptrs[0]=(tk_kptr)bootgbr;
			task->img_tlsrvas[0]=tlsix[7];
			
			*(tk_kptr *)bootgbr=task->img_gbrptrs;
#endif

			TKPE_SetupTaskForImage(task, pimg);
			bootgbr=task->basegbr;
			
			TK_SchedAddTask(task);

			tk_printf("TKSH_TryLoad: task=%p, env=%p\n", task, env1);

			tk_printf("TKPE_SetupTaskForImage: GBR=%p\n", bootgbr);

			sza=0;
//			if(args)
			if(1)
			{
				for(i=0; args[i]; i++)
				{
					j=strlen(args[i]);
					sza+=j+(j>>2)+4;
				}

				ct=boot_newspb;
				cs1=tk_rovalloc(sza, (void **)(&ct));
				
				ct1=cs1;
				for(i=0; args[i]; i++)
				{
					ct1=tksh_cifyemit(ct1, args[i]);
					*ct1++=' ';
				}
				if(ct1>cs1)
				{
					*(ct1-1)=0;
				}
				task->argv=(tk_kptr)cs1;
			}else
			{
				j=strlen(img);
				sza=j+(j>>2)+4;

				ct=boot_newspb;
				cs1=tk_rovalloc(sza, &ct);
				ct1=tksh_cifyemit(ct1, img);
				*ct1++=0;
				task->argv=(tk_kptr)cs1;
			}

#if 0
			ct=boot_newspb;
			a1=tk_rovalloc(256, &ct);
			if(args)
			{
				for(i=0; args[i]; i++)
				{
					a1[i]=tk_rovstrdup(args[i], &ct);
				}
				a1[i]=NULL;
			}else
			{
				a1[0]=tk_rovstrdup(img, &ct);
				a1[1]=NULL;
			}
			task->argv=a1;
#endif

#if 0
			rv=tk_sysc_exitpt();
//			if(rv || (tksh_runstate != 2))
			if(rv)
			{
				if(rv<0)
					rv=-rv;
//				__debugbreak();
				tk_con_chkreset();
//				__debugbreak();
				TK_TaskFreeAllPageAlloc(task);
				TK_DestroyTaskInfo(task);
				return(rv);
			}

			if(chk!=0x1234567)
				__debugbreak();

			tksh_runstate = 3;
		
			TK_FlushCacheL1D();
			TK_FlushCacheL1D_INVIC(NULL);

//			__arch_gbr=bootgbr;
//			bootptr();
			__tk_farcall(bootptr, bootgbr, boot_newsp, boottbr);
#endif

#if 1
			tksh_runstate = 3;
		
			TK_FlushCacheL1D();
			TK_FlushCacheL1D_INVIC(NULL);

			tkern->task_join_ret=ptask;
			if(!ptask)
			{
				if(ctask==tk_task_syscall)
					__debugbreak();
				tkern->task_join_ret=ctask;
			}
			
			pb_boot=(u64)bootptr;

			tkern->ctx_regsave[TKPE_REGSAVE_TTB]=tk_vmem_pageglobal;
//			tkern->ctx_regsave[TKPE_REGSAVE_SPC]=bootptr;
			tkern->ctx_regsave[TKPE_REGSAVE_SPC]=
				pb_boot&0x0000FFFFFFFFFFFEULL;
			tkern->ctx_regsave[TKPE_REGSAVE_GBR]=bootgbr;
			tkern->ctx_regsave[TKPE_REGSAVE_SSP]=boot_newsp;
//			tkern->ctx_regsave[TKPE_REGSAVE_EXSR]|=0xC000000000000000ULL;
			tkern->ctx_regsave[TKPE_REGSAVE_EXSR]|=
				0xC000000000000000ULL|
				(((pb_boot>>48)&0x000C)<<56)|
				(((pb_boot>>48)&0x00F0)<<48);
//			tkern->ctx_regsave[TKPE_REGSAVE_EXSR]|=0x8000000000000000ULL;
			TK_Task_SyscallReturnToUser(task);

#if 1
			if(!ptask)
			{
				rv=TK_Task_JoinOnReturn(task);
				if(rv<0)
					rv=-rv;
				if(!rv)rv=1;
				tk_con_chkreset();
				TK_TaskFreeAllPageAlloc(task);
				TK_DestroyTaskInfo(task);
				return(rv);
			}
#endif

			return(1);

#endif

		}
	}
	
	if(fd)
	{
		sig_is_asc=0; nl=0;
		cs=tb;
		while(*cs)
		{
			i=*cs;
			if(i<' ')
			{
				if(i=='\r')
				{
					cs++;
					i=*cs;
					if(i=='\n')
						cs++;
					nl++;
					continue;
				}
				if(i=='\n')
				{
					cs++;
					nl++;
					continue;
				}
				if(i=='\t')
					{ cs++; continue; }
	//			if((i=='\r') || (i=='\n') || (i=='\t'))
	//				{ cs++; continue; }
				break;
			}
			
			if((i&255)>=127)
				break;
			cs++;
		}
		
		if(!(*cs) && (nl>0))
			sig_is_asc=1;
		
		if(sig_is_asc)
		{
	//		TKSH_ExecCmdFd(fd);
			if(plf_fdofs!=0)
			{
				tk_fseek(fd, plf_fdofs, 0);
				sz=plf_fdsz;
			}else
			{
				tk_fseek(fd, 0, 2);
				sz=tk_ftell(fd);
				tk_fseek(fd, 0, 0);
			}
			buf=tk_malloc(sz+16);
			memset(buf, 0, sz+8);
			tk_fread(buf, 1, sz, fd);
			tk_fclose(fd);
			fd=NULL;

			TKSH_ExecCmdBuf(buf);
			tk_free(buf);
			return(1);
		}
	}
	
	if(fd)
	{
		tk_printf("Failed to execute %s\n", img);
		tk_fclose(fd);
		fd=NULL;
	}
	
//	TK_DestroyTaskInfo(NULL);
	return(-1);
}

int TKSH_TryLoad_ext(char *img, char **args)
{
	char **path;
	char tb[256];
	int npath, ri;
	int i;

	ri=TKSH_TryLoad(img, args);
	if(ri>0)
		return(ri);
	
	strcpy(tb, img);
	strcat(tb, ".exe");
	ri=TKSH_TryLoad(tb, args);
	if(ri>0)
		return(ri);
	
	strcpy(tb, img);
	strcat(tb, ".sh");
	ri=TKSH_TryLoad(tb, args);
	if(ri>0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".com");
	ri=TKSH_TryLoad(tb, args);
	if(ri>0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".pf");
	ri=TKSH_TryLoad(tb, args);
	if(ri>0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".bxw");
	ri=TKSH_TryLoad(tb, args);
	if(ri>0)
		return(ri);

	return(-1);
}


int TKSH_TryLoad_n(char *img, char **args)
{
	char **path;
	char tb[256];
	char tb_cwd[256];
	int npath, ri;
	int i;

#if 1
	TK_Env_GetCwd(tb_cwd, 256);
//	strcpy(tb, a[0]);
	strcpy(tb, tb_cwd);
	strcat(tb, "/");
	strcat(tb, args[0]);
//	strcat(tb, ".exe");
//	ri=TKSH_TryLoad(tb, args);
	ri=TKSH_TryLoad_ext(tb, args);
	if(ri>0)
		return(ri);
#endif

	path=NULL;	npath=0;
	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		strcpy(tb, path[i]);
		strcat(tb, "/");
		strcat(tb, args[0]);
//		strcat(tb, ".exe");
//		ri=TKSH_TryLoad(tb, args);
		ri=TKSH_TryLoad_ext(tb, args);
		if(ri>0)
			return(ri);
	}

	return(ri);
}

