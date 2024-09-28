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

#ifndef __TK_CLIB_ONLY__
extern TKPE_TaskInfo *tk_task_syscall;
extern u64	tk_vmem_pageglobal;
#endif

TKPE_TaskInfo *TK_GetSyscallUserTask();

TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2);

TKSH_CommandInfo *TKSH_LookupCommand(char *name);
TKSH_CommandInfo *TKSH_CreateCommand(char *name);

TK_EnvContext *TK_GetTaskEnvContext(TKPE_TaskInfo *task);


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
	memset(cur, 0, sizeof(TKSH_CommandInfo));
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

TKSH_CommandInfo *TKSH_RegisterCommandDesc(char *name, void *fcn, char *desc)
{
	TKSH_CommandInfo *cmdi;

	cmdi=TKSH_CreateCommand(name);
	cmdi->Cmd=fcn;
	cmdi->desc=tk_strdup_in(desc);
	return(cmdi);
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
	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

	if(tk_get_ttyid())
	{
		tk_puts("\x1B[3J");
		return(0);
	}

	tk_con_reset();
	return(0);
}

int TKSH_Cmds_Chdir(char **args)
{
	char tb_cwd[256];
	char tb[256];
	TK_DIR *dir;
	char *darg;
	int dohelp;
	int i;

	TK_Env_GetCwd(tb_cwd, 256);
	
	darg=NULL;	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] [dir]\n", args[0]);
		return(0);
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
	int i, j;

	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

	for(i=1; args[i]; i++)
	{
		s0=args[i];
		
		if(*s0=='$')
		{
			j=TK_Env_GetEnvVarI(s0+1, tb1, 256);
			if(j>0)
			{
				tk_puts(tb1);
				tk_putc(' ');
			}
			continue;
		}
		
		tk_puts(s0);
		tk_putc(' ');
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
	if(*s1==':')
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
	int dohelp;
	int i;

	TK_Env_GetCwd(tb_cwd, 256);
	
	darg=NULL;	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
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
	int dohelp;
	int i;

	darg=NULL;	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
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
	int dohelp;
	int i;

	darg=NULL;	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
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
	int dohelp;
	int i;

	darg=NULL;
	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
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

	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

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
	
	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

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
	int dohelp;
	int i;

	darg=NULL; dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
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
	int dohelp;
	int i;

	darg=NULL;	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
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

	TKSH_Cmds_Cls(NULL);
//	tk_con_reset();

	return(0);
}

int TKSH_Cmds_HexEdit(char **args)
{
	char tb[256];
	char *darg;
	int dohelp;
	int i;

	darg=NULL;	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		darg=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
	}

//	tksh_cmdentry=1;
	
	if(darg)
	{
		THSH_QualifyPathArg(tb, darg);
		TKSH_HexLoadFile(tb);
	}
	
	TKSH_HexUpdateLoop();

//	tk_con_reset();
	TKSH_Cmds_Cls(NULL);

	return(0);
}

int TKSH_Cmds_Cat(char **args)
{
	char tb[256];
	char *fname[64];
	char *darg;
	TK_FILE *fd;
	int dohelp;
	int i, j, k, nfn;

	nfn=0;
	dohelp=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "--help"))
				{ dohelp=1; continue; }

			continue;
		}
		fname[nfn++]=args[i];
	}

	if(dohelp)
	{
		tk_printf("Usage: %s [options] file\n", args[0]);
		return(0);
	}

	for(i=0; i<nfn; i++)
	{
		darg=fname[i];
		THSH_QualifyPathArg(tb, darg);
		fd=tk_fopen(tb, "rb");
		if(!fd)
			continue;
		j=0;
//		while(!tk_feof(fd))
		while(1)
		{
			tk_fgets(tb, 255, fd);
			tk_printf("%s\n", tb);
			k=tk_ftell(fd);
			if(k==j)
				break;
			j=k;
		}
		tk_fclose(fd);
	}

	return(0);
}

#if 0
static const u32 tst_mandrill0_bmp[] = {
0x14364D42,0x00000000,0x04360000,0x00280000,0x00400000,0x00400000,0x00010000,
0x00000008,0x10000000,0x0EC30000,0x0EC30000,0x01000000,0x01000000,0x1C240000,
0x2320001E,0x2D1E0028,0x3136003F,0x332D0031,0x3547003A,0x3624002E,0x37310039,
0x372A0033,0x3D200037,0x35190042,0x3C20006B,0x44340059,0x442E0044,0x473F0047,
0x4836003D,0x4537003D,0x495E0051,0x4843003B,0x3926004A,0x4E2C0089,0x49510042,
0x4C2C004B,0x4E520050,0x4D1F0040,0x4F350055,0x52400047,0x524A0046,0x503E0046,
0x45250051,0x563800A5,0x5C430072,0x6055005B,0x604C004A,0x5D4B004E,0x5F610059,
0x5E56004B,0x534C0053,0x5F2F0080,0x5F370065,0x5F3F0063,0x608C0061,0x5D3D0046,
0x643F006D,0x5D790058,0x5156005F,0x57600094,0x637D007D,0x4521004B,0x5F5C00D0,
0x64740065,0x664A004E,0x5F4F0057,0x6169006D,0x5738005F,0x50460094,0x6B5600AF,
0x4A3E0057,0x685600D1,0x69610064,0x6231005D,0x684F0085,0x6C600067,0x6B6D0054,
0x450A0055,0x451B00F8,0x462600F6,0x661C00F5,0x482D0092,0x483500F4,0x684A00F3,
0x6B4D0085,0x6E31007B,0x5625007D,0x743E00D3,0x703F0069,0x73330078,0x744D0072,
0x714F0068,0x71470072,0x621A0076,0x784E00BB,0x50350060,0x522600F0,0x531A00EF,
0x4F4000F0,0x775900F1,0x737F0064,0x775F0065,0x75720063,0x777E0064,0x7768005A,
0x78710062,0x7467005C,0x7460006D,0x7B680071,0x6B760057,0x514C008A,0x574000F1,
0x799C00E9,0x6E6A005E,0x7BAE0095,0x73680054,0x795F0087,0x75830076,0x5A390079,
0x584E00EE,0x797800EE,0x575C0071,0x7E9D00ED,0x7C910055,0x61610060,0x666B00CC,
0x767900B8,0x6F380082,0x685C00B3,0x605C00BF,0x604A00E5,0x615200EC,0x616900E9,
0x7A4700E2,0x674D009A,0x817300D8,0x846C0077,0x816B0070,0x818C007B,0x847E0071,
0x8566006C,0x85740071,0x858B006D,0x62790066,0x685B00E2,0x7EA400D9,0x73730078,
0x846000AE,0x814B007E,0x815D0090,0x7C5F008A,0x8861009D,0x854A0074,0x85580086,
0x82530083,0x8553008F,0x75860085,0x895900AA,0x894C0077,0x823B007C,0x8A53009C,
0x85400079,0x893F0092,0x7D1A0085,0x780600BB,0x813B00D6,0x7A6600A9,0x6F7F00B6,
0x716F00D9,0x8E9700E0,0x9180007B,0x8EC10079,0x8E8C006F,0x91780081,0x8E7D007E,
0x8FB40088,0x94B00074,0x97D10065,0x958F0056,0x96C50077,0x7D720062,0x979D00D4,
0x96770071,0x8E9B0084,0x8D790093,0x7E8700A4,0x8E8500D3,0x8C0C009F,0x947800D0,
0x89850091,0x7B9B00B2,0x8A9400DC,0x913A00AC,0x944D00B6,0x909300A7,0x975C009D,
0x986A0099,0x9B5C0094,0x9864008F,0x95650097,0x9B4F00A1,0x9B650095,0x955E008E,
0x9C6A00A5,0x9570008A,0x9C7300A0,0x99740088,0x829D0093,0x8A8A00D3,0x9BA300C6,
0x9A8A0087,0x9E890094,0x9B990087,0x984E008F,0x973A00BF,0x94BD00CF,0x92AA00B9,
0xB0E800C7,0xB1DC005E,0xACDA0066,0xA78B0078,0xA65D00A5,0xAB8300B8,0xA781009B,
0xAC7B00AB,0xB1D1009F,0x9F9A0072,0xA7A700C3,0xA9CF00A4,0xAD970091,0xAA990097,
0xACA700A1,0xAF6F0096,0xAEB500A3,0xAE8F008F,0xA79C009C,0xAB6F00B0,0xB06200B2,
0xA84900AB,0xA6AA00CF,0xB17900B8,0xA93000A4,0xACB600D8,0xA6BA00A2,0xAD7A00B5,
0xB2D700B4,0xAD590086,0xA9A200C7,0xAD4100C0,0xAF9600D8,0xBFF100B8,0xB3B30070,
0xBA8C00AF,0xB66A00AB,0xB68F00C8,0xB84F00BC,0xC1EB00D3,0xB6AB0080,0xBBA600BE,
0xC2E700B8,0xBFE3008B,0xC3E30099,0xBEDC0093,0xBE4100A8,0xBFBD00DD,0xC4E000B8,
0xC7DE009E,0xCAD800A7,0xC9DA00B2,0x6E7E00BC,0x9CC69C9F,0x6B9FDCC4,0x284E7CC5,
0x343C2A2A,0xECE7B4D2,0xF4FBF5F4,0xD5EAF4F4,0xEAEAEAD5,0xF4EAEAD5,0xF1ECECEC,
0xD0ECE7F1,0x4B879494,0x27182727,0x5E3A224E,0x123A5D3A,0xA5A53B31,0xDAC6C79F,
0xA1A0DAA5,0x474B7CA1,0x8D472A67,0xF5F1F178,0xF4F4F5F5,0xE2EAD5E2,0xD5B0B0C3,
0xEAEAC3C3,0xF5EADEEA,0xE0F5F5F5,0x95878CB6,0x4D8C4F4A,0x313A3D28,0x7A35345D,
0x9FDC1231,0xD9DDC59C,0xAFC5C7DA,0x478E7C88,0xD2881666,0xECECF1DF,0xE2F4FBF4,
0x859999B0,0xC3997399,0x85B0D599,0x71D6B5AD,0xCFFBEEE5,0x4BBAD2AF,0x4D8E8B4F,
0x3A1F674E,0x5D357A5D,0xC5C71535,0xBFBAC5C7,0xBFCFD9D9,0x478C4FBA,0xB4784688,
0xEEFBF5FB,0xD5B0EEFB,0x83397370,0x6F83C376,0xB099B073,0xB5E6B525,0xC5D66871,
0x4F4788D9,0x4E8B8EBA,0x343D8886,0x6B9C5D5E,0xA1D97D35,0xBDD2C1CF,0xBAC1CFD2,
0x664B87BA,0xECBD4689,0xEEEEEEFB,0x85B2B5D6,0x74393970,0x7683A776,0x9983A79B,
0x2ED6602D,0x9C7EAA71,0xD2C5A1C7,0x4F4B4BB8,0x5E3D4E88,0x7A6B7A3D,0xC5C57D7E,
0xBDD2D8D2,0xBDB6AFB4,0x664878BD,0xD2DEBF89,0xE5D6FBEE,0xACD5FBB5,0xACAC8370,
0xACC3C3C3,0x74ACA7C3,0xB564B0D5,0xE535D62E,0xD9DA6B7D,0xD1B9DDD9,0x5E3D8888,
0x9F6B7A3D,0xC5CF6BA5,0xD0B7D2D2,0xB4B4BFC8,0x667892D0,0xCFADABAB,0x34A1EED6,
0xB085EAF4,0xE2E6B0E2,0xE2E2E2E6,0xC3E2B2E2,0xE2E2E2EA,0x5DAA2EE6,0xC5DA57E5,
0x8888C1C5,0x675E678D,0x7E7A7A4E,0xD2CF819D,0xBDB6D2E7,0x92B498F2,0xC5929294,
0xC5DEDE78,0xFBEEFB6B,0xE2E2EEFB,0xD6AAAAE6,0x70EAB0B0,0xEE85856F,0xEAD5E2E2,
0x6BAA60EA,0xC7C4C46B,0xD2D9D8D9,0x5E7CCFC1,0x7A7A7A7F,0xF1DF7E7E,0xD0D0D0E7,
0xB4C8B3E1,0xBDBA92BA,0xD6ADADF4,0xFBD6EED6,0x85ADB5D6,0x7339AC39,0x44747452,
0x73749B45,0xEEDED6AD,0x31AAE2EA,0xC79C6B11,0x8D87CFAF,0x7C8C87BF,0x9D7A5D7C,
0xE7DF9F9F,0xC9E1F0E7,0x92D0F2E1,0xEC3C43BF,0xB5E2ABBF,0xC7FBFBC7,0x459A70D6,
0x74614455,0x41424461,0x6A454445,0xAA31B270,0x64F4E5D6,0xE59C15E5,0xD2D8DADA,
0xA0B7B9EF,0x9DA05D7C,0xF0DFA59D,0xC9E9F0DF,0xB4B398C9,0xBDC887B7,0xE5B5EEAB,
0xADC4EEFE,0x42455539,0x9A454141,0x41414245,0x42554442,0xC7B53745,0xE268E557,
0x9CE5C4B5,0xCFD9DADC,0x5EAFDFE0,0x9D7C5E5E,0xE9F0C69D,0xC9F0F0E9,0x78C898C9,
0x72E7B478,0xEEE5DEDE,0x6F71C4FB,0x42414277,0x76444142,0x55455545,0x44455555,
0x68606142,0xE55757C4,0x129FE6B2,0xD8D9DAD6,0x678DB9D3,0xA07C5D3D,0xE9DFC6A0,
0xE1E9F2E1,0x8788C9C9,0xF5F29494,0x7D68D6AD,0x552D9F6B,0x44444455,0x76455555,
0x6C555576,0x44556155,0xAA734240,0x57353557,0x9CB5B511,0xD9C5D69F,0x5D7AC5D8,
0x7C7A5D5D,0xE1F0A0C6,0xEBEBE1E1,0x941698E1,0xC8B4B487,0x9FE5E5EE,0x45706803,
0x55555561,0x776F5555,0x6C61749A,0x55455561,0x85744144,0x357D5735,0x57AA157D,
0xD8DADADA,0x7D9FC6C6,0xA09F5D5D,0xF0E9A0A1,0xEBEBEBFA,0x8D4B98B3,0xBABFBF92,
0xC46BC7DE,0x45452E17,0x77778261,0x79833061,0x61627476,0x42456161,0x37444155,
0x326B326E,0x156B7D35,0xA8C4A8C4,0x9F3AA19F,0x7A7A9F6B,0xE9E9A97C,0xE4FAE4FA,
0x4E2A98B3,0xBDD987BB,0x071BC7D9,0x42452D32,0x6C775561,0x7655791D,0x3075769B,
0x74767474,0x6F414545,0x3F7D6E2C,0x9C7D6E32,0xA580A85A,0x223A589F,0x5D9F5959,
0xE1E9A97C,0xFAFAFAEB,0x4F274898,0xF5CFE7A1,0x156B9FD9,0x42452D57,0x0A767455,
0xC29A8362,0x759BC29A,0x62771313,0x37426962,0x2357A2A2,0x6EC41781,0x80A5819C,
0x9F583A7A,0x7A3B6B23,0xE9E1867A,0xC9EBFAF2,0x5E4E8796,0xD9C5D2C1,0x3231C5D6,
0x41692E32,0x741D1D52,0x779AB1AC,0xC2C28277,0x697579AC,0xB2527669,0x6332A284,
0x576E8157,0x7A7EA55A,0x313B6B59,0x5D3A7C22,0xF2FA67A0,0xC9EBFAF2,0x1C287898,
0xD8C1D3A9,0x230380DA,0x75392C63,0x9B9B7662,0x7761616A,0x4455746A,0x9B9B836A,
0x84605376,0x6363639E,0x57326E6E,0x7A59A56E,0x223BA56B,0x5D3A221F,0xF2F2867C,
0xE1FAFAFA,0x084E2A98,0xDAD8DA57,0x23325DC5,0x62602F11,0x44556A77,0x6C6A4541,
0x45626A6C,0x44525544,0x6365CA75,0x2F6D6365,0xC49C5A3F,0x7E597E9F,0x1F3A5931,
0x671F1F3A,0xFAEB4E5E,0xE1F2F2EB,0x7A884B98,0xDADA9F9F,0x32175EC5,0x84632F32,
0x4452A78F,0x55524141,0x55616A77,0xD7745252,0xCE63CE9E,0x326D2F63,0x23245A7D,
0x5B59593F,0x2222223B,0x5E343A22,0xF2F2674E,0xC9F2E1E1,0x3A478DC8,0xA1193822,
0x2F051B3A,0x6D11A632,0xACF76329,0x45524555,0x55557474,0xD7F99B44,0xA2CECE2C,
0x6365CE2C,0x1B235A2F,0x1A3B243E,0x225B2222,0x3D281C3B,0xEBE44E4E,0xC8C9C9C9,
0x19093CC8,0x3E0C0804,0x17071A24,0x65A4CD29,0xD7D76ECE,0x42415274,0x69696A74,
0xF79ECB52,0xA6CECE63,0x636D29A6,0x6E5A3F6E,0x2238233F,0x3A221F22,0x5E3D3D22,
0xE9F2284E,0xB398C8C8,0x1A0D9492,0x1A080F03,0x17170707,0x2FF36329,0xD7F735CE,
0x424262B2,0x52527474,0xFF63F955,0xCE9E2FE8,0x6DA66DA6,0x1B232332,0x211C3F1B,
0x28381C2B,0x1F565B5B,0xC8E11F2B,0x4898C8E9,0x01279294,0x5D00240C,0x1707070E,
0xCEE8CCA6,0xD7D763E8,0x41417684,0x69529A6A,0xF72CF955,0xEDF763F9,0x6D65CEA4,
0x1B0F1A3F,0x3F580E20,0x4D1C2B9D,0x1F4D5622,0xC8C92719,0x98B4C8C8,0x1A4B4848,
0x01040807,0x29003F0E,0xF79EED65,0xF9FFCEA2,0x424176D7,0x52699A74,0xF7A2F974,
0xEDE8A2FF,0x6DEDA6CD,0x353F1717,0x215B7DA5,0x33563319,0x1C1F3356,0xE9B32827,
0x98B3C8D0,0x0606BFB9,0x24080107,0x2F00000E,0xF9CCF8CC,0xF9FFF9D7,0x4144C3F9,
0x41429A77,0xE8FDD761,0xFED7E8F7,0x6DF3A6ED,0x21232307,0x335B5C5C,0x3D331A21,
0x22212238,0xB3B32B2B,0x87BDD0E9,0x144A4B92,0x07010804,0x65050004,0xFFCEE8ED,
0xD7F9D7D7,0x4141ACF9,0x41559A77,0xCEFCF945,0xFFD7FDFF,0xCDE8F3F3,0x3E200F0E,
0x2B213821,0x2B228A67,0x3333222B,0xD0B3332B,0x87BDBDD0,0x0C0F194E,0x0C060803,
0x6505080C,0xFEF7A6CE,0xD7FFFFD7,0x414276F9,0x41749A6A,0xE8F7FF44,0xFFD7F9FD,
0xF3CDF3FC,0x210E0F0F,0x33339D5B,0x2B56334D,0x4D2B3E21,0xB4C8191C,0xD0D0E7D0,
0x0C2BB6B6,0x0C04160C,0xCC11010E,0xFDFDCEFC,0xF9FDFEE8,0x414455FC,0x539AC26C,
0xF9F7CA52,0xF9E8F9F7,0xCEEDF3FF,0x5F381A23,0x382B2120,0x7B2B5133,0x21563356,
0xF0E92128,0xB68794B4,0x06278B8E,0x08190804,0xED29000F,0xF9FDCEE8,0xF9E8FDE8,
0x444141F9,0x69CBE69A,0xFFF37741,0xD7FFF9F7,0xCCF3F6FD,0x0E380FD4,0x7B7B5C9D,
0x51225856,0x335121A0,0xBAB42133,0x8E87BABF,0x0F4A8C8E,0x04190D0D,0xED630008,
0xD7FCF363,0xFFE8F8F8,0x414140E6,0x52E6CA77,0xF9CE4541,0xE8FFFCE8,0xEDEDF7F9,
0x5F1A21CD,0x512B5880,0x2B564D33,0x382B5133,0x88882121,0x1C3D4EBF,0x0D162B4F,
0x040C0822,0xF3CC000D,0xE8F7EDA6,0xFCE8FEFD,0x424040C2,0x419ACA82,0xFCF75553,
0xFEFCFCF9,0xEDF3F8D7,0x213838CD,0x33515138,0x4D28563D,0x2B332B58,0x5D3A4D2B,
0x1C0D1F67,0x08280D1F,0x22161010,0xCECC0004,0xE8F8F8CC,0xF7FEFCFE,0x414141AC,
0x5477CB6C,0xF8F74552,0xFCFFF8FF,0xF3F6CECE,0x801B21CD,0xC08A582B,0x2BC08A51,
0x2B4D4D22,0x108D3319,0x2B104E1F,0x1F2B2719,0x10081F86,0x65ED0510,0xF8E8F6ED,
0xF7FFF8FD,0x444062B1,0x539BC26C,0xF8F75554,0xFCFFF9FF,0xF3F8CEF8,0x211A20F3,
0x9DA07B56,0x4D568A9D,0x7B224D33,0x5E347B51,0x043D1088,0x22124D04,0x3A670C5E,
0xA4ED0512,0xFCCEF6ED,0xF7FFF8FD,0x414174C2,0x549BCA74,0xF6F75254,0xFCF8F7FF,
0xF3F7CEF3,0x380F5CED,0xD8A558A9,0x8A569DC6,0x51902B8A,0x8D88587F,0x4F22283A,
0x4EBC1C2A,0x1F3A674B,0xEDED1122,0xFCF6E8F6,0xF6FFFCFC,0x414162CB,0x5476CA74,
0xF3F75253,0xF8F8F9F9,0xF3CEEDF3,0x3321D4ED,0xC6D89D20,0xDDA09DA9,0x338A8A58,
0x1C474D51,0x4E4E4E2B,0x4D8B861F,0x227A67A9,0xEDA4051C,0xF8F3A6F8,0xF6FEFDFC,
0x40416ACB,0x539B9A6A,0xF8F76954,0xF8F8FDF8,0xF6A6F3F3,0x5838D4ED,0x9DC6A038,
0x7FC6C6A0,0x512B7BA9,0x2A865193,0x4B27168E,0xB72BB64E,0xA9C14ED2,0xEDA40322,
0xF8F6CDF6,0xF6FDFDF8,0x414176E6,0x699BCB6A,0xF8FC6954,0xF8FCFDF6,0xCDF3F6FC,
0x3838D4ED,0xD8C6A580,0x5ED8C6C6,0x8AC08AD2,0x1C1C5151,0xB8284B4F,0xD2884F3D,
0x887C86B7,0xF3651712,0xF8F3EDCD,0xF6F8F9F8,0x414275E5,0x54A79A62,0xF8F77654,
0xF8F8FCFC,0xA6F3F3F8,0x815CD4ED,0xDD80A5A5,0x80D89DC6,0x4DA97FC0,0xBA2A514A,
0x8E8D281F,0x86DFAF28,0x244FB7AF,0xED65227A,0xF6F3EDCE,0xF6F8FDFC,0x545353FE,
0x41626253,0xFEF77541,0xE8F8F6FC,0xEDF3F3F6,0x9D5FCDED,0xDD80A09D,0x9FD1D8D8,
0x8A8CA0C6,0x4F4F2B93,0xB9672A8E,0xD1878EB6,0x7C7A8CB7,0xED2F1231,0xF8F6EDA6,
0xF8F6FCE8,0x417455FE,0x41697452,0xFECE7554,0xF6F6F8F8,0xEDEDF3F3,0x5F7ED4CD,
0xD9D8C67E,0x9DC1A9D8,0x4DA07FA9,0x341C932B,0x94108728,0x28944E3D,0x7CB767EF,
0xCD111B7A,0xD4F8F3CC,0xF8F8F8F6,0x547552FE,0x54696954,0xFDCE8352,0xF6F6F8FE,
0xEDEDF3CD,0xA87EA3D4,0xD8A5A8DC,0xD1C6A9DD,0xC0A9D1C5,0x164B4A8A,0x8C48262A,
0xB62A4B4B,0x7AA986B6,0x9E239DAF,0xCEF8F3CC,0xFDF8F8F6,0x537653FE,0x54537454,
0xFCD77969,0xF3F8FCFD,0xEDEDCDF3,0x9CA581ED,0xC69DA5DC,0xD37CDDF5,0x86B8B7C0,
0x4E4BA951,0x864B4F4B,0xE08C4BB6,0x7CC1C1A9,0x17576B80,0xEDF6F3ED,0xFEFCF8F6,
0x547430FE,0x54537553,0xF8AA7975,0xF3F6FCFE,0xEDCDCDED,0x7EA5A8CD,0xDD7E9DA5,
0xA9D1D1A9,0xBC4DBEC0,0x8C4B90DB,0x4B4F4EB6,0xDBEF8688,0x7C674F88,0x11243BA0,
0xEDA3CDED,0xFEFCF8F6,0x538330FE,0x41536A49,0xFCD77975,0xF3D4FDFC,0xD4A3EDED,
0xA8815AA8,0xA07BDAA8,0xD8D8A9DD,0xAFC0C1A0,0x272691BB,0xB98C8C88,0x678C4FE0,
0x0994B68D,0x323F671F,0xEDEDA3CC,0xFCFCD4F3,0x69A730FD,0x5469831D,0xFDD77979,
0xD4F8F8FC,0xD4EDEDCD,0x807E3F3F,0xA9C658A9,0xD1DDD3D1,0x90BEE3C6,0x4B4E33BE,
0xB68B878B,0xC18CBBB6,0x3A6788B9,0x1A1A0C88,0xEDCC6E0E,0xF8FCE8D4,0x69791DFC,
0x49538336,0xFDD7369B,0xEDF6F8F8,0xA4D4A8A3,0x677A5A5A,0xD1A98622,0xA9D3D1C6,
0xBED1DDE3,0x4B4BBC8A,0x4C8BB68B,0xDBBBE0D0,0x88AF34E0,0x3E221CA0,0x57A4291B,
0xF6F6F3CD,0xC36436F8,0x49547313,0xFE3114A7,0xEDF6F8FD,0x3F6DCCED,0x4D1B2423,
0xD3A07F7C,0xA9BEBC7F,0x908AE3D3,0x8B26284D,0xBBE3938E,0xDFB6B6BB,0x7CBCD38C,
0x219D7E21,0xA4290E1B,0xCDEDEDCC,0xB06B04F8,0x5036250A,0xF6041A99,0xA8D4FCFE,
0x17116EA8,0x337C5B23,0xD1C6C6C1,0xC0EFD18A,0x3DE3D1EF,0x944CB8A9,0xB7DFB68B,
0xE3C1BCB8,0x7C86C093,0x3F356B7C,0x1B171B59,0x6D63151B,0x66240811,0x7273250B,
0xF3035934,0xEDEDEDED,0x236E9EED,0x7E5D247E,0xDADD7F7F,0xDDDDC0F5,0x90DBE3EF,
0x48168AE3,0xDBB948B8,0x4EB890DB,0x7A868CBE,0x5C7A7E7A,0xC5A1190E,0x246BA1A1,
0x3A04813F,0x72363636,0x2320A531,0x68595C23,0x810E1031,0x3B7A7A5C,0xC1C13E5D,
0xEFEFD3B7,0xBEEFEF8A,0x4A09C08A,0xE08BB8B6,0xBB4DDBB7,0x7AAF4DEF,0x5F387A7F,
0x217B385B,0xA53B3E38,0x0B0F1A3B,0x89363C02,0x59075F9F,0x5B7AA19F,0x31C5C5A1,
0x3B5B3E7E,0xA9DD7E7C,0xEFDDEFCF,0xD3BEDBD1,0x95949386,0x8B944E4C,0x91B9DBDF,
0x7AC6C1BE,0xA9806B3B,0x01075156,0x3A000101,0x02A0585B,0x471E1E0B,0x9D381431,
0x5A20819D,0xA1C61B81,0x801A247A,0xD8A15B3E,0xD1EFEFD8,0xBC93BCE3,0x4C944CB8,
0x8B948B95,0xC1E3B6BC,0xA19F6786,0x56A55C3B,0x9621010F,0x060136AE,0x020D3826,
0x27164F09,0xBD21A01E,0x02010907,0x805C0309,0x9D245DCF,0xB7DD3B80,0xEF86BEC1,
0xB793C0D1,0x482628C0,0xE04AB64A,0x8DE3BCDB,0x3B317AC1,0xA9A03E21,0x97975F06,
0x01885097,0x06095807,0x14142709,0x01582A16,0xAE960706,0x0F5A9696,0x3F7EC67A,
0xD85B9D3F,0xE0B9518A,0x8BD18ADB,0x8B914A8E,0xE0BC8D94,0xC0BEE3B8,0x1B0C1C4D,
0x1A1A0E04,0x0A508001,0x0372508D,0x06010F80,0x19091414,0x2186460C,0x4396A904,
0x0494960A,0x5C38C6A0,0xC5123B5B,0xA9C1C0A0,0x86D3B8BC,0x2616674A,0xBBE0BB28,
0xB8B84DBC,0x151204BC,0x0C120C04,0x0A1E0304,0x07070B0B,0x08040407,0x33805B0C,
0x8066060F,0x43965B07,0x018E9643,0x0C1A193E,0x7A5B1C21,0x8EB986EF,0x90E0DBE3,
0x264C2B8C,0x91BB4895,0x4DB8B8BB,0x0C1C3386,0x06091010,0x06060606,0x060D0C06,
0x5C122210,0x1A21191A,0x0D060919,0x0C071504,0x0C040210,0x330C0C0C,0x5D3B100C,
0xD3DF4ED9,0xB8A0EFA9,0x2626BC91,0xE0B8E091,0xBEDBBBDB,0x2B8A0626,0x0F12561C,
0x211B0F0C,0x0C0D0D1C,0x579C5912,0x59231AA8,0x19145B81,0x1A1A3E19,0x191C1C21,
0x1B0C0D19,0x273D225C,0xDFDBB786,0x4BB7B7B8,0x952627B8,0xBB95954C,0xB891BB8B,
0x2B4A93BE,0x7EA59D1A,0x5B205CA5,0x353F2159,0x81A35981,0xA3A8595C,0x387EDCDC,
0x593F7E0F,0x5A1B205A,0x3A242421,0x1F9D1C3D,0xD1C14E1F,0xC1DFD3D1,0x18184A4E,
0xB8934C2B,0xDB4AB8BB,0x8A8AE393,0x7EDA5BA0,0x240E2424,0x5C120E1B,0x23071B38,
0x1B141B0F,0xA8DCA85A,0x5A5C3F81,0xDC9FDAA5,0x59A89C81,0xC1C6124D,0x8BD1D17C,
0x4BDBD3B7,0x9402904F,0x954CBB4A,0x934ABBBB,0x9390938A,0xA5D88AD1,0x1C7A5BA5,
0x21805959,0x5C385B21,0x1A212419,0x1B203E3E,0x3E242324,0xA8813F23,0x7A599F9C,
0xA97CA99D,0xD2D37CA0,0xDFC0DFD3,0x18182B8E,0x4C8BB826,0x2695BB4C,0x33519193,
0x38A9A9DD,0x2138587F,0xA59D7F4D,0x5C803B59,0x7E20A55C,0x81818181,0x237E7A59,
0x3B3880A5,0x9FDAA5A8,0x5E805D9D,0xDFD3C6DD,0xDFC0BEB8,0xFFFF4E27,0xFFFFFFFF,
};
#endif

// extern void __rsrc__mandril;
extern byte __rsrc__mandril[];

int TKSH_Cmds_TestGfx(char **args)
{
	u64 *conbufa;
	u64 *conbufa_0;
	u64 *conbufa_1;
	u64 *conbufa_2;
	u64 *conbufa_3;
	int kk, md, lmd, mdb, clrm, brk;
	u16 mandril_pal16[256];
	byte mandril_pal8[256];
	u16 mandril_pal4[256];
//	byte mandril_pal8[256];
	byte mandril_pal2[256];
	byte mandril_pal1[256];
	u16 *mandril_pal;
	byte *mandril_pal8b;

	byte *mandril_bmp;
	byte *mandril_pix;
	byte *mandril_pal3;
	u64 v0, v1, v2, v3;
	int cr, cg, cb, cy, cr1, cg1, cb1;
	int i, j, k, k0, k1, xstr;

	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

	conbufa=(u64 *)0xFFFFF00A0000ULL;
//	((u32 *)0xFFFFF00BFF00UL)[0]=0x0095;	//320x200x16bpp, RGB555

	conbufa_0=(u64 *)0xFFFFF00A0000ULL;
	conbufa_1=(u64 *)0xFFFFF00A0001ULL;
	conbufa_2=(u64 *)0xFFFFF00A0002ULL;
	conbufa_3=(u64 *)0xFFFFF00A0003ULL;

	md=0;
	lmd=-1;
	clrm=0;
	brk=0;
	xstr=80;

//	mandril_bmp=(byte *)(tst_mandrill0_bmp);
	mandril_bmp=(byte *)(&__rsrc__mandril);
//	mandril_pix=mandril_bmp+0x0436;
//	mandril_pal3=mandril_bmp+0x0036;

	if(mandril_bmp[0]==' ')
	{
		j=*(u32 *)(mandril_bmp+0x0C);
		mandril_pix=mandril_bmp+j;
		mandril_pal3=mandril_bmp+0x0038;
	}else
		if(mandril_bmp[0]=='B')
	{
		j=*(u32 *)(mandril_bmp+0x0A);
		mandril_pix=mandril_bmp+j;
		mandril_pal3=mandril_bmp+0x0036;
	}else
	{
		tk_printf("Image sig not intact D=%08X A=%p\n",
			*(u32 *)mandril_bmp, mandril_bmp);
		return(0);
	}

	for(i=0; i<256; i++)
	{
		cr=mandril_pal3[i*4+2];
		cg=mandril_pal3[i*4+1];
		cb=mandril_pal3[i*4+0];
		cy=(2*cg+cr+cb)>>2;
		
		cr=cr>>3;
		cg=cg>>3;
		cb=cb>>3;
		k=(cr<<10)|(cg<<5)|cb;
		mandril_pal16[i]=k;
		
		cr1=cr>>3;
		cg1=cg>>2;
		cb1=cb>>3;
		k=(cr1<<5)|(cg1<<2)|cb1;
		mandril_pal8[i]=k;

//		if(cg>=16)
		if(cy>=128)
		{
			cr1=(cr>=24);
			cg1=(cg>=24);
			cb1=(cb>=24);
			k=0x8|(cr1<<2)|(cg1<<1)|cb1;
		}else
		{
			cr1=(cr>=12);
			cg1=(cg>=12);
			cb1=(cb>=12);
			k=(cr1<<2)|(cg1<<1)|cb1;
		}

		k=k|(k<<4)|(k<<8)|(k<<12);
		mandril_pal4[i]=k;
		
		cr1=cy+24;
		cb1=cy-24;
//		k=(cy>=128)?((cy>192)?3:2):((cy>64)?1:0);
//		k=k|(k<<2)|(k<<4)|(k<<6);

		k0=(cr1>=128)?((cr1>192)?3:2):((cr1>64)?1:0);
		k1=(cb1>=128)?((cb1>192)?3:2):((cb1>64)?1:0);
		k=k0|(k1<<2)|(k1<<4)|(k0<<6);

		mandril_pal2[i]=k;

		cr1=cy+48;
		cb1=cy-48;
		k0=(cr1>=128);
		k1=(cb1>=128);
		k=(k0<<0)|(k1<<1)|(k1<<2)|(k0<<3);
		mandril_pal1[i]=k;
	}
	
	tk_printf("Keys: 1..9 = Graphics Modes; 0=Text\n");
	tk_printf("  1=320x200 16bpp  , 2=320x200 8bpp , 3=640x400 RGBI\n");
	tk_printf("  4=640x400 4-color, 5=640x400 1-bpp, 6=640x400 16bpp\n");
	tk_printf("Keys: a..p = Palette Mode Selector (2bpp and 1bpp modes)\n");
	tk_printf("Keys: Space = Exit\n");

	while(!brk)
	{
		if(md!=lmd)
		{
			switch(md)
			{
			case 0:
				mdb=0x0081;	//80x25 color-cell
				xstr=80;
				break;
			case 1:
				mdb=0x0095;	//320x200x16bpp, RGB555
				xstr=80;
				break;
			case 2:
				mdb=0x0097;	//320x200x8bpp, RGB232
				xstr=80;
				break;
			case 3:
				mdb=0x0025;	//640x400x4bpp, RGBI
				xstr=80;
				break;
			case 4:
				mdb=0x0027;	//640x400x2bpp, 4 Color
				xstr=80;
				break;
			case 5:
				mdb=0x0425;	//640x400x1bpp, 2 Color
				xstr=80;
				break;
			case 6:
				mdb=0x000D0095;	//320x200x16bpp, RGB555
				xstr=160;
				break;

			case 7:
//				mdb=0x0020008F;	//800x600x8bpp, RGB232
				mdb=0x002DC09F;	//800x600x8bpp, RGB232
				xstr=200;
				break;

			default:
				mdb=0x0081;	//80x25 color-cell
				xstr=80;
				break;
			}
			
			mdb|=(clrm&15)<<12;
			((u32 *)0xFFFFF00BFF00UL)[0]=mdb;

			if((md==1) || (md==3))
			{
				mandril_pal=mandril_pal16;
				if(md==3)
					mandril_pal=mandril_pal4;
				
				for(i=0; i<16; i++)
					for(j=0; j<16; j++)
				{
					k=(63-(i*4+0))*64+(j*4);
					v0=         mandril_pal[mandril_pix[k+3]];
					v0=(v0<<16)|mandril_pal[mandril_pix[k+2]];
					v0=(v0<<16)|mandril_pal[mandril_pix[k+1]];
					v0=(v0<<16)|mandril_pal[mandril_pix[k+0]];
					k=(63-(i*4+1))*64+(j*4);
					v1=         mandril_pal[mandril_pix[k+3]];
					v1=(v1<<16)|mandril_pal[mandril_pix[k+2]];
					v1=(v1<<16)|mandril_pal[mandril_pix[k+1]];
					v1=(v1<<16)|mandril_pal[mandril_pix[k+0]];
					k=(63-(i*4+2))*64+(j*4);
					v2=         mandril_pal[mandril_pix[k+3]];
					v2=(v2<<16)|mandril_pal[mandril_pix[k+2]];
					v2=(v2<<16)|mandril_pal[mandril_pix[k+1]];
					v2=(v2<<16)|mandril_pal[mandril_pix[k+0]];
					k=(63-(i*4+3))*64+(j*4);
					v3=         mandril_pal[mandril_pix[k+3]];
					v3=(v3<<16)|mandril_pal[mandril_pix[k+2]];
					v3=(v3<<16)|mandril_pal[mandril_pix[k+1]];
					v3=(v3<<16)|mandril_pal[mandril_pix[k+0]];
					
					k=((i*xstr)+j)*4;
					conbufa[k+0]=v0;
					conbufa[k+1]=v1;
					conbufa[k+2]=v2;
					conbufa[k+3]=v3;
				}
			}

			if((md==2) || (md==4) || (md==7))
			{
				mandril_pal8b=mandril_pal8;
				if(md==4)
					mandril_pal8b=mandril_pal2;

				for(i=0; i<16; i++)
					for(j=0; j<16; j++)
				{
					k=(63-(i*4+1))*64+(j*4);
					v0=        mandril_pal8b[mandril_pix[k+3]];
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+2]];
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+1]];
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+0]];
					k=(63-(i*4+0))*64+(j*4);
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+3]];
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+2]];
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+1]];
					v0=(v0<<8)|mandril_pal8b[mandril_pix[k+0]];

					k=(63-(i*4+3))*64+(j*4);
					v1=        mandril_pal8b[mandril_pix[k+3]];
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+2]];
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+1]];
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+0]];
					k=(63-(i*4+2))*64+(j*4);
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+3]];
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+2]];
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+1]];
					v1=(v1<<8)|mandril_pal8b[mandril_pix[k+0]];
					
					k=((i*xstr)+j)*2;
					conbufa[k+0]=v0;
					conbufa[k+1]=v1;
				}
			}

			if(md==5)
			{
				mandril_pal8b=mandril_pal1;

				for(i=0; i<16; i++)
					for(j=0; j<16; j++)
				{
					k=(63-(i*4+3))*64+(j*4);
					v0=        mandril_pal8b[mandril_pix[k+3]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+2]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+1]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+0]];
					k=(63-(i*4+2))*64+(j*4);
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+3]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+2]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+1]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+0]];
					k=(63-(i*4+1))*64+(j*4);
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+3]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+2]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+1]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+0]];
					k=(63-(i*4+0))*64+(j*4);
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+3]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+2]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+1]];
					v0=(v0<<4)|mandril_pal8b[mandril_pix[k+0]];
					
					k=((i*xstr)+j)*1;
					conbufa[k]=v0;
				}
			}

			if(md==6)
			{
				mandril_pal=mandril_pal16;
				if(md==3)
					mandril_pal=mandril_pal4;
				
				for(i=0; i<16; i++)
					for(j=0; j<16; j++)
				{
					k=(63-(i*4+0))*64+(j*4);
					v0=         mandril_pal[mandril_pix[k+3]];
					v0=(v0<<16)|mandril_pal[mandril_pix[k+2]];
					v0=(v0<<16)|mandril_pal[mandril_pix[k+1]];
					v0=(v0<<16)|mandril_pal[mandril_pix[k+0]];
					k=(63-(i*4+1))*64+(j*4);
					v1=         mandril_pal[mandril_pix[k+3]];
					v1=(v1<<16)|mandril_pal[mandril_pix[k+2]];
					v1=(v1<<16)|mandril_pal[mandril_pix[k+1]];
					v1=(v1<<16)|mandril_pal[mandril_pix[k+0]];
					k=(63-(i*4+2))*64+(j*4);
					v2=         mandril_pal[mandril_pix[k+3]];
					v2=(v2<<16)|mandril_pal[mandril_pix[k+2]];
					v2=(v2<<16)|mandril_pal[mandril_pix[k+1]];
					v2=(v2<<16)|mandril_pal[mandril_pix[k+0]];
					k=(63-(i*4+3))*64+(j*4);
					v3=         mandril_pal[mandril_pix[k+3]];
					v3=(v3<<16)|mandril_pal[mandril_pix[k+2]];
					v3=(v3<<16)|mandril_pal[mandril_pix[k+1]];
					v3=(v3<<16)|mandril_pal[mandril_pix[k+0]];
					
					k=(i*xstr)+j;
					conbufa_0[k]=v0;
					conbufa_1[k]=v1;
					conbufa_2[k]=v2;
					conbufa_3[k]=v3;
				}
			}

			mdb|=(clrm&15)<<12;
			((u32 *)0xFFFFF00BFF00UL)[0]=mdb;
			lmd=md;
		}

		while(tk_kbhit())
		{		
			kk=tk_getch();
			if(kk==' ')
			{
				brk=1;
				continue;
			}
			if(kk==TK_K_ESC)
			{
				brk=1;
				continue;
			}
			
			if((kk>='0') && (kk<='9'))
			{
				md=kk-'0';
				continue;
			}

			if((kk>='a') && (kk<='p'))
			{
				clrm=kk-'a';
				lmd=-1;
				continue;
			}
		}
	}

	((u32 *)0xFFFFF00BFF00UL)[0]=0x0081;	//80x25 color-cell
	tk_con_reset();
	return(0);
}

#include <tkgdi/tkgdi.h>

int TKSH_Cmds_StartGui(char **args)
{
	TKGDI_BITMAPINFOHEADER t_info, t_info2;
	TKGDI_EVENT t_imsg;
	_tkgdi_context_t *ctx;
	TKGDI_BITMAPINFOHEADER *info, *info2;
	TKGDI_EVENT *imsg;
	TKGHDC hdcScrn, hdcWin;
	TKGDI_RECT tRect;
	char tb[256];
	int i, j, k;

	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

	info=&t_info;
	info2=&t_info2;
	imsg=&t_imsg;

	tk_con_disable();

	info->biWidth=640;
	info->biHeight=400;

//	info->biWidth=800;
//	info->biHeight=600;
	info->biBitCount=16;

//	ctx=TKGDI_GetHalContext(TK_FCC_GDI, TK_FCC_GDI);
	ctx=TKGDI_GetCurrentGdiContext();

	hdcScrn=ctx->vt->CreateDisplay(ctx, 0, TKGDI_FCC_crea, info);

//	info2->biWidth=640;
//	info2->biHeight=200;

	info2->biWidth=480;
	info2->biHeight=200;

//	info2->biWidth=320;
//	info2->biHeight=150;

	info2->biBitCount=16;
	info2->biCompression=TKGDI_FCC_text;

	hdcWin=ctx->vt->CreateDisplay(ctx, hdcScrn, TKGDI_FCC_crea, info2);

	tRect.left=80;
	tRect.top=100;
	tRect.right=80+640;
	tRect.bottom=100+200;
	ctx->vt->ModifyDisplay(ctx, hdcWin, TKGDI_FCC_move, &tRect, NULL);

	ctx->vt->DrawString(ctx, hdcWin, -1, -1,
		"Console test string\r\n", 0, 0);

	TK_SpawnShellTask((void *)TK_GET_TBR, 0x10000000+hdcWin+(0<<20));
	TK_SpawnShellTask((void *)TK_GET_TBR, 0x10000000+hdcWin+(1<<20));
	TK_SpawnShellTask((void *)TK_GET_TBR, 0x10000000+hdcWin+(2<<20));
	TK_SpawnShellTask((void *)TK_GET_TBR, 0x10000000+hdcWin+(3<<20));

	while(1)
	{
		TKGDI_UpdateWindowStack();
		TK_YieldCurrentThread();

#if 0
		while(1)
		{
			j=ctx->vt->QueryDisplay(ctx, hdcWin, TKGDI_FCC_poll, NULL, imsg);
			if(j<1)
				break;
			if(imsg->fccMsg==0)
				break;

			if(imsg->fccMsg==TKGDI_FCC_keyb)
			{
				tk_printf("Win keyb %02X\n", imsg->wParm1);
				tb[0]=imsg->wParm1;
				tb[1]=0;
				
				if(imsg->wParm1=='\r')
				{
					tb[1]='\n';
					tb[2]=0;
				}
				
				ctx->vt->DrawString(ctx, hdcWin, -1, -1, tb, 0, 0);
			}
		}
#endif

#if 0
		if(!tk_kbhit())
		{
			TK_YieldCurrentThread();
			continue;
		}

		i=tk_getch();
		if(i<=0)
		{
			TK_YieldCurrentThread();
			continue;
		}

		if(i==0x80)
		{
			j=tk_getch();
			k=(j<<8)|tk_getch();
		}else
			if((i==0x7F) || (i==0xFF))
		{
			j=tk_getch();
			k=((i&0x80)<<8)|j;
		}else
		{
			k=((i&0x80)<<8)|(i&0x7F);
		}

//		if(i>=0x7F)
//			continue;

		if(i==TK_K_ESC)
			break;
		
		imsg->evSize=sizeof(TKGDI_EVENT);
		imsg->dev=hdcScrn;
		imsg->fccMsg=TKGDI_FCC_keyb;
		imsg->ptMsec=0;
		imsg->wParm1=k;
		imsg->wParm2=0;
		imsg->lParm1=0;
		imsg->ptMsX=0;
		imsg->ptMsY=0;
		imsg->ptMsB=0;
		ctx->vt->QueryDisplay(ctx, hdcScrn, TKGDI_FCC_poll, imsg, NULL);
#endif

//		tk_dbg_printf("Push keyb %02X\n", imsg->wParm1);
	}

	info->biWidth=640;
	info->biHeight=200;
	hdcScrn=ctx->vt->CreateDisplay(ctx, 0, TKGDI_FCC_crea, info);
	
	tk_con_reset();
	return(0);
}

int TKSH_Cmds_Help(char **args)
{
	char *t_args[16];
	char tb[256];
	TKSH_CommandInfo *cmdi;
	int i, n;
	
	if(args[1])
	{
		cmdi=TKSH_LookupCommand(args[1]);
	}else
	{
		cmdi=NULL;
	}
	
//	if(!args[1])
	if(!cmdi)
	{
		while(tk_kbhit())
			tk_getch();
		tk_printf("Builtin Commands:\n", cmdi->name);
		cmdi=tksh_commands;
		while(cmdi)
		{
			n=22;
			while(cmdi && ((n--)>0))
			{
				if(cmdi->desc)
				{
					tk_printf("  %-12s %s\n", cmdi->name, cmdi->desc);
				}else
				{
					tk_printf("  %12s\n", cmdi->name);
				}
				cmdi=cmdi->next;
			}
			if(cmdi)
			{
				tk_printf("... ? ");
				tk_gets(tb);
//				tk_getch();
			}
		}
		return(0);
	}

	t_args[0]=args[1];
	t_args[1]="--help";
	for(i=2; args[i]; i++)
	{
		t_args[i]=args[i];
	}
	t_args[i]=NULL;
	
	cmdi->Cmd(t_args);
	
//	if(cmdi->desc)
//	{
//		tk_printf("  %-12s %s\n", cmdi->name, cmdi->desc);
//	}else
//	{
//		tk_printf("  %12s\n", cmdi->name);
//	}
	return(0);
}

int TKSH_Cmds_Uuidgen(char **args)
{
	u64 val[2];
	char tb[128];
	int ver;
	int i;

	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}
	
	ver=0;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			if(!strcmp(args[i], "-r"))
				ver=1;
			if(!strcmp(args[i], "-t"))
				ver=2;
		}
	}

	if(!ver)
		ver=1;

	if(ver==2)
		TK_GenerateTimeUuid(val);
	else
		TK_GenerateUuid(val);

#if 0
	tk_printf(
		"%02x%02x%02x%02x-%02x%02x-%02x%02x-"
		"%02x%02x-%02x%02x%02x%02x%02x%02x\n",
		((val[0]>> 0)&255),	((val[0]>> 8)&255),
		((val[0]>>16)&255),	((val[0]>>24)&255),
		((val[0]>>32)&255),	((val[0]>>40)&255),
		((val[0]>>48)&255),	((val[0]>>56)&255),
		((val[1]>> 0)&255),	((val[1]>> 8)&255),
		((val[1]>>16)&255),	((val[1]>>24)&255),
		((val[1]>>32)&255),	((val[1]>>40)&255),
		((val[1]>>48)&255),	((val[1]>>56)&255)
		);
#endif

	TK_FormatUuidAsString(tb, val);
	tk_printf("%s\n", tb);
//	TK_FormatGuidAsString(tb, val);
//	tk_printf("%s\n", tb);
	return(0);
}

int TKSH_Cmds_Date(char **args)
{
	time_t tv;
	char *ts;
	int i;

	if(args[1] && !strcmp(args[1], "--help"))
	{
		tk_printf("Usage: %s [options]\n", args[0]);
		return(0);
	}

	tv=time(NULL);
	tk_printf("%llX\n", tv);
	ts=ctime(&tv);
	tk_printf("%s\n", ts);
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

	TKSH_RegisterCommandDesc("cd",		TKSH_Cmds_Chdir,	"Change Directory");
	TKSH_RegisterCommandDesc("chdir",	TKSH_Cmds_Chdir,	"Change Directory");
	TKSH_RegisterCommandDesc("cls",		TKSH_Cmds_Cls,		"Clear Screen");
	TKSH_RegisterCommandDesc("cp",		TKSH_Cmds_Cp,		"Copy file");

	TKSH_RegisterCommandDesc("dir",		TKSH_Cmds_Ls,		"List Directory");

	TKSH_RegisterCommandDesc("echo",	TKSH_Cmds_Echo,		"Print Message");
	TKSH_RegisterCommandDesc("ed",		TKSH_Cmds_Ed,		"Line Editor");
	TKSH_RegisterCommandDesc("edit",	TKSH_Cmds_Edit,		"Text Editor");

	TKSH_RegisterCommandDesc("hexedit",	TKSH_Cmds_HexEdit,	"Hex Editor");
	TKSH_RegisterCommandDesc("cat",		TKSH_Cmds_Cat,	
		"Dump file to terminal");

	TKSH_RegisterCommandDesc("ln",		TKSH_Cmds_Ln,		"Create Symlink");
	TKSH_RegisterCommandDesc("ls",		TKSH_Cmds_Ls,		"List Directory");

	TKSH_RegisterCommandDesc("md",		TKSH_Cmds_Mkdir,	"Make Directory");
	TKSH_RegisterCommandDesc("mkdir",	TKSH_Cmds_Mkdir,	"Make Directory");
	TKSH_RegisterCommandDesc("mount",	TKSH_Cmds_Mount,	"Mount Volume");
	TKSH_RegisterCommandDesc("mv",		TKSH_Cmds_Mv,		"Move File");

	TKSH_RegisterCommandDesc("recvxm",	TKSH_Cmds_RecvXm,
		"Recieve file over serial/terminal link");
	TKSH_RegisterCommandDesc("rm",		TKSH_Cmds_Rm,		"Remove File");
	TKSH_RegisterCommandDesc("rmdir",	TKSH_Cmds_Rmdir,	"Remove Directory");

	TKSH_RegisterCommandDesc("set",		TKSH_Cmds_Set,		"Set Env Variable");
	TKSH_RegisterCommandDesc("uuidgen",	TKSH_Cmds_Uuidgen,	"Generate UUID");
	TKSH_RegisterCommandDesc("date",	TKSH_Cmds_Date,	"Print date");

	TKSH_RegisterCommandDesc("testgfx",	TKSH_Cmds_TestGfx,
		"Test Graphics Modes");

	TKSH_RegisterCommandDesc("startgui",	TKSH_Cmds_StartGui,
		"Start GUI");
	TKSH_RegisterCommandDesc("help",		TKSH_Cmds_Help,
		"Info about commands (help [command])");

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
	char *rdin, *rdout;
	int fdin, fdout, ofdin, ofdout, rdappend;
	int ci, ri;
	int i, j, k;

	a=tk_rsplit(cmd);
	if(!a[0])
		return(0);
//	ci=tk_cmd2idx(a[0]);
//	TK_Env_GetCwd(tb_cwd, 256);

	rdin=NULL;
	rdout=NULL; j=0;
	rdappend=0;
	for(i=0; a[i]; i++)
	{
		k=a[i][0];
		if(k=='$')
		{
			TK_Env_GetEnvVar(a[i]+1, tb1, 255);
			a[j++]=tk_rstrdup(tb1);
			continue;
		}else
			if(k=='<')
		{
			if(a[i][1])
			{
				rdin=a[i]+1;
			}else
			{
				rdin=a[i+1];
				i++;
			}
			continue;
		}else
			if(k=='>')
		{
			if(a[i][1]=='>')
			{
				rdappend=1;
				if(a[i][2])
				{
					rdout=a[i]+2;
				}else
				{
					rdout=a[i+1];
					i++;
				}
			}else
			{
				rdappend=0;
				if(a[i][1])
				{
					rdout=a[i]+1;
				}else
				{
					rdout=a[i+1];
					i++;
				}
			}
			continue;
		}else
		{
			tk_dbg_printf("TKSH_ExecCmd: A=%d Ch=%02X\n", i, k);
			a[j++]=a[i];
		}
	}
	
	a[j]=NULL;

	if(rdin)
	{
		THSH_QualifyPathArg(tb1, rdin);
		rdin=tk_rstrdup(tb1);
	}

	if(rdout)
	{
		THSH_QualifyPathArg(tb1, rdout);
		rdout=tk_rstrdup(tb1);
	}

#if 1
	cmdi=TKSH_LookupCommand(a[0]);
	if(cmdi && (cmdi->Cmd))
	{
		fdin=-1; fdout=-1;
		if(rdin || rdout)
		{
			ofdin=tk_get_redir_stdin();
			ofdout=tk_get_redir_stdout();
			if(rdin)
			{
				fdin=tk_hfopen(TK_GetCurrentTask(), rdin, "rb");
				if(fdin>0)
					tk_set_redir_stdin(fdin);
			}
			if(rdout)
			{
				if(rdappend)
					fdout=tk_hfopen(TK_GetCurrentTask(), rdout, "ab");
				else
					fdout=tk_hfopen(TK_GetCurrentTask(), rdout, "wb");
				if(fdout>0)
					tk_set_redir_stdout(fdout);
			}
			i=cmdi->Cmd(a);
			if(fdin>0)
			{
				tk_set_redir_stdin(ofdin);
				tk_hclose(TK_GetCurrentTask(), fdin);
			}
			if(fdout>0)
			{
				tk_set_redir_stdout(ofdout);
				tk_hclose(TK_GetCurrentTask(), fdout);
			}
		}else
		{
			i=cmdi->Cmd(a);
		}
		return(i);
	}

	cmd[0]=0;

	if(rdin)
	{
		fdin=tk_hfopen(TK_GetCurrentTask(), rdin, "rb");
		if(fdin>0)
			tk_set_next_redir_stdin(fdin);
	}
	if(rdout)
	{
		if(rdappend)
			fdout=tk_hfopen(TK_GetCurrentTask(), rdout, "ab");
		else
			fdout=tk_hfopen(TK_GetCurrentTask(), rdout, "wb");
		if(fdout>0)
			tk_set_next_redir_stdout(fdout);
	}
	
	strcpy(tb, a[0]);
	
	ri=TKSH_TryLoad_n(tb, a);

	tk_set_next_redir_stdin(0);
	tk_set_next_redir_stdout(0);

	if(ri>=0)
	{
		if(ri&65535)
			{ tk_dbg_printf("Return Status=%d\n", (ri&65535)); }
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

int TKSH_ExecCmdBuf(char *cmd, char *ext)
{
	byte tb[256];
	char *cs, *ct;
	int i;
	
	if(ext)
	{
#ifdef __BGBCC__
		if(!strcmp(ext, "bas"))
		{
			TKSH_BasRunBasicBuffer(cmd, strlen(cmd));
			return(0);
		}
#endif
	}
	
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
	return(0);
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

int TKSH_TryLoadA0(char *img, char **args0)
{
	byte tb[1024];
	byte cwd[256];
	char *args[64];
	u64 tlsix[8];
	TK_FILE *fd;
	char **a1;
	char *cs, *ct, *cs1, *ct1;
	char *buf, *ext;
//	u64 bootgbr;
	u64	pb_gbr;
	u64	pb_boot;
	u64	pb_sysc;
	int plf_dofs, plf_dnum, plf_fdofs, plf_fdsz;
	int plf_lofs, plf_lsz, plf_lname1, plf_lname2, plf_lname3;
	int sig_is_pe, sig_is_asc, sig_is_elf;
	int rv, nl, sz, sza, ix;
	int i, j, k;

	tk_dbg_printf("TKSH_TryLoadA0: %s\n", img);

//	pimg=NULL;
	fd=NULL;
	sig_is_pe=0;
	sig_is_elf=0;
	ext=NULL;
	
	cs=img+strlen(img);
	while((cs>img) && (*cs!='.'))
		cs--;
	if((cs>img) && (*cs=='.') && ((*(cs-1))!='/'))
		ext=cs+1;

	TK_Env_GetCwd(cwd, 256);
	
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

//	if(!pimg)
	if(1)
	{
		fd=tk_fopen(img, "rb");
		
		if(!fd)
		{
			tk_dbg_printf("Failed to open %s\n", img);
			return(-1);
		}

		while(fd)
		{
			tk_fseek(fd, 0, 0);
			memset(tb, 0, 1024);
			tk_fread(tb, 1, 1023, fd);

			plf_lname1=tkfat_getWord(tb);
			plf_lname2=tkfat_getDWord(tb+128);

			if(plf_lname1!=TCC_HASHBANG)
				break;
			if(plf_lname2==FCC_PLFW)
				break;

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
		
		sig_is_pe=0;
		if((tb[0]=='M') && (tb[1]=='Z'))
			sig_is_pe=1;
		if((tb[0]=='P') && (tb[1]=='E'))
			sig_is_pe=1;

		if((tb[0]==0x7F) && (tb[1]=='E') && (tb[2]=='L') && (tb[3]=='F'))
			sig_is_elf=1;
	}

	if(sig_is_pe || sig_is_elf)
		return(-2);

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

			TKSH_ExecCmdBuf(buf, ext);
			tk_free(buf);
			return(1);
		}
	}

	return(-1);
}

int TK_CreateProcess(
	char *imgname,
	char *cmdline,
	char *envmod,
	char *cwd,
	u64 taskflags,
	TKPE_CreateTaskInfo *info);

int TKSH_TryLoadA(char *img, char **args0)
{
	char tbuf[512];
	byte cwd[256];
	TKPE_CreateTaskInfo t_tinf;
	TKPE_CreateTaskInfo *tinf;
	char *ct;
	int rdfdi, rdfdo;
	int i, rt, tid;
	
	rt=TKSH_TryLoadA0(img, args0);
	if(rt>0)
	{
		tk_dbg_printf("TKSH_TryLoadA: A0-OK %s\n", img);
		return(rt);
	}
	
	if(rt==-2)
	{
		tk_dbg_printf("TKSH_TryLoadA(-2): %s\n", img);

		rdfdi=tk_get_next_redir_stdin();
		rdfdo=tk_get_next_redir_stdout();
		if(rdfdi<=0)
			rdfdi=tk_get_redir_stdin();
		if(rdfdo<=0)
			rdfdo=tk_get_redir_stdout();

		tinf=&t_tinf;
		tinf->szInfo=sizeof(TKPE_CreateTaskInfo);
		tinf->idTty=tk_get_ttyid();
		tinf->rdStdin=rdfdi;
		tinf->rdStdout=rdfdo;
		tinf->szStack=0;	//use default
		tinf->flSched=1;

		if(args0)
		{
			ct=tbuf;
			for(i=0; args0[i]; i++)
			{
				strcpy(ct, args0[i]);
				ct+=strlen(ct);
				*ct++=' ';
				*ct=0;
			}
		}else
		{
			strcpy(tbuf, img);
		}
	
		TK_Env_GetCwd(cwd, 256);
	
		tid=TK_CreateProcess(img, tbuf, NULL, cwd, 0, tinf);
		tk_dbg_printf("TKSH_TryLoadA(-2): pid=%d\n", tid);
		rt=TK_Task_PidJoinOnReturn(tid);

//		__debugbreak();
		return(rt);
//		return(tid);
	}

	return(-1);
}

TKPE_TaskInfo *tksh_createproc_ptask=NULL;
TKPE_CreateTaskInfo *tksh_createproc_info=NULL;

int TKSH_TryLoadB(char *img, char **args0)
{
	byte tb[1024];
	byte cwd[256];
	byte atb[512];
	char *args[64];
	u64 tlsix[8];
	TK_FILE *fd;
	char **a1;
	char *cs, *ct, *cs1, *ct1;
	char *buf, *ext;
//	u64 bootgbr;
	u64	pb_gbr;
	u64	pb_boot;
	u64	pb_sysc;
	TKPE_TaskInfo *task;
	TKPE_TaskInfo *ctask, *ptask;
	TKPE_TaskInfoKern *tkern;
	TKPE_CreateTaskInfo *ctinfo;
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
	int sig_is_pe, sig_is_asc, sig_is_elf;
	int rv, nl, sz, sza, ix, pid;
	int argc;
	int i, j, k;

	pimg=NULL;
	fd=NULL;
	sig_is_pe=0;
	sig_is_elf=0;
	ext=NULL;
	
	if(args0)
	{
		ct=atb;

		j=strlen(img);
		memcpy(ct, img, j+1);
		args[0]=ct;
		ct+=j+1;

//		for(i=0; args0[i]; i++)
		for(i=1; args0[i]; i++)
		{
//			args[i]=args0[i];
			j=strlen(args0[i]);
			memcpy(ct, args0[i], j+1);
			args[i]=ct;
			ct+=j+1;
		}
		args[i]=NULL;
//		args[0]=img;
		argc=i;
	}else
	{
		ct=atb;

		j=strlen(img);
		memcpy(ct, img, j+1);
		args[0]=ct;
		ct+=j+1;

//		args[0]=img;
		args[1]=NULL;
		argc=1;
	}

	cs=img+strlen(img);
	while((cs>img) && (*cs!='.'))
		cs--;
	if((cs>img) && (*cs=='.') && ((*(cs-1))!='/'))
		ext=cs+1;

	TK_Env_GetCwd(cwd, 256);
	
	ix=TKPE_TryLoadProgramImage(img, cwd, 2);
	if(ix>0)
	{
		pimg=TK_GetImageForIndex(ix);
//		tk_dbg_printf("TKSH_TryLoad: Got Image %d %p\n", ix, pimg);
	}else
	{
//		tk_dbg_printf("TKSH_TryLoad: Not Yet Loaded\n");
	}

	if(!pimg)
	{
		fd=tk_fopen(img, "rb");
		
		if(!fd)
		{
			tk_dbg_printf("Failed to open %s\n", img);
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

#ifdef __BGBCC__
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
#endif
		
		sig_is_pe=0;
		if((tb[0]=='M') && (tb[1]=='Z'))
			sig_is_pe=1;
		if((tb[0]=='P') && (tb[1]=='E'))
			sig_is_pe=1;

		if((tb[0]==0x7F) && (tb[1]=='E') && (tb[2]=='L') && (tb[3]=='F'))
			sig_is_elf=1;
	}

	chk=999;
	rchk=&chk;
	*rchk=0x1234567;

	j=strlen(args[0]);	//BGB: Debug

//	if(fd)
	if(pimg || (fd && sig_is_pe) || (fd && sig_is_elf))
	{
		bootgbr=0;
//		TKPE_LoadStaticPE(fd, plf_fdofs, &bootptr, &bootgbr, tlsix);
//		tk_printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);

		if(!pimg)
//		if(!pimg && sig_is_pe)
			pimg=TKPE_LoadDynPE(fd, 0, img, NULL, 0);

		if(fd)
		{
			tk_fclose(fd);
			fd=NULL;
		}

		if(!pimg)
		{
			return(-1);
		}

		bootptr=(void *)(pimg->bootptr);

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
		
		j=strlen(args[0]);	//BGB: Debug

		if(bootptr)
		{
		
			tksh_runstate = 2;
		
//			boot_newspb=TKMM_PageAlloc(1<<18);
//			boot_newsp=boot_newspb+((1<<18)-1024);

			i=TK_GetRandom16ASLR()&0x03F0;
			i+=1024;
			boot_newspb=TKMM_PageAlloc(1<<18);
//			boot_newspb=TKMM_PageAlloc(1<<19);
//			boot_newspb=TKMM_PageAlloc(1<<20);
//			boot_newspb=TKMM_PageAllocUsc(1<<19);
//			boot_newspb=TKMM_PageAllocUsc(1<<20);
//			boot_newsp=boot_newspb+((1<<19)-1024);
			boot_newsp=boot_newspb+(((1<<18)-1024)-i);
//			boot_newsp=boot_newspb+(((1<<19)-1024)-i);
//			boot_newsp=boot_newspb+(((1<<20)-1024)-i);

//			*(byte **)boot_newsp=((byte *)boot_newsp)+8;
//			strncpy(((byte *)boot_newsp)+8, atb, 512);

//			ct=((byte *)boot_newsp)+512;
			ct=((byte *)boot_newspb);

#if 1
//			a1=(char **)(((byte *)boot_newsp)+16);
			*(long *)(boot_newsp+0)=argc;
//			*(byte **)(boot_newsp+8)=((byte *)a1);
			a1=(char **)(((byte *)boot_newsp)+8);
			strncpy(ct, atb, 512);
			
			for(j=0; j<argc; j++)
			{
				k=((char *)(args[j]))-((char *)atb);
				a1[j]=ct+k;
			}
			a1[j]=NULL;	//end eof argc list
			a1+=(argc+1);

			j=0;
			a1[j]=NULL;	//end eof env list
			a1+=(j+1);

			j=0;

			a1[j++]=TKPE_ELF_AT_BASE;		//interpreter base
			a1[j++]=pimg->elf_interpbase;

			a1[j++]=TKPE_ELF_AT_PHDR;		//program header
			a1[j++]=pimg->elf_phdr_ptr;

			a1[j++]=TKPE_ELF_AT_PHENT;
			a1[j++]=pimg->elf_phdr_phentsz;

			a1[j++]=TKPE_ELF_AT_PHNUM;
			a1[j++]=pimg->elf_phdr_phnum;

			a1[j++]=TKPE_ELF_AT_ENTRY;		//program entry point
			a1[j++]=pimg->realentry;

			a1[j++]=TKPE_ELF_AT_EXECFN;		//program entry point
			a1[j++]=img;

			a1[j++]=TKPE_ELF_AT_PAGESZ;	//page size
			a1[j++]=1<<TKMM_PAGEBITS;	//page size

			a1[j++]=TKPE_ELF_AT_HWCAP;	//capability flags
			a1[j++]=0;					//capability flags

			a1[j++]=TKPE_ELF_AT_CLKTCK;		//clock tick frequency
			a1[j++]=1000;					//

			a1[j++]=NULL;	//end eof aux list
			a1+=(j+1);

			a1[0]=NULL;
			a1[1]=NULL;
			a1[2]=NULL;
			a1[3]=NULL;
#endif

//			i=TK_GetRandom16ASLR()&0x00F0;
//			boot_newspbk=TKMM_PageAlloc(1<<16);
//			boot_newspk=boot_newspb+((1<<16)-1024);
//			boot_newspk=boot_newspbk+((1<<16)-1024);
//			boot_newspk=boot_newspbk+(((1<<16)-1024)-i);

			boot_newspbk=NULL;
			boot_newspk=NULL;

			ctinfo=tksh_createproc_info;

			ctask=TK_GetCurrentTask();
			ptask=NULL;

			if(tksh_createproc_ptask)
			{
				ptask=tksh_createproc_ptask;
				if(ptask==tk_task_syscall)
					ptask=NULL;
			}

			if((ctask==tk_task_syscall) && !ptask)
			{
				ptask=TK_GetSyscallUserTask();
				if(ptask==tk_task_syscall)
					ptask=NULL;
			}

			if(ptask)
			{
				env0=TK_GetTaskEnvContext(ptask);
			}else
			{
				env0=TK_GetCurrentEnvContext();
			}

			env1=TK_EnvCtx_CloneContext(env0);

			boottbr=TK_AllocNewTask();
			task=boottbr;
			tkern=(TKPE_TaskInfoKern *)(task->krnlptr);

			if(ptask)
			{
				task->ttyid=ptask->ttyid;
			}
			
			if(ctinfo)
			{
				task->redir_stdin=ctinfo->rdStdin;
				task->redir_stdout=ctinfo->rdStdout;
			}

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

#ifdef __BGBCC__
//			sysc=tk_isr_syscall;
			sysc=tk_syscall_utxt;
#endif

			pb_sysc=(tk_kptr)sysc;
//			pb_sysc&=0x0000FFFFFFFFFFFEULL;
//			pb_sysc|=0x0000000000000001ULL;
			sysc=(void *)pb_sysc;
//			__debugbreak();

#ifdef __BJX2__
			if(!(pb_sysc&1))
			{
//				__debugbreak();

				__ifarch(bjx2_xg2)
				{
					pb_sysc&=0x0000FFFFFFFFFFFEULL;
					pb_sysc|=0x0080000000000001ULL;
				}else
				{
					pb_sysc&=0x0000FFFFFFFFFFFEULL;
					pb_sysc|=0x0000000000000001ULL;
				}
				sysc=(void *)pb_sysc;
			}
#endif

			pb_boot=(u64)bootptr;

#ifdef __BGBCC__
//			if((bootptr>>50)&1)
//				{ sysc=tk_syscall_rv_utxt; }

			if((pb_boot>>50)&1)
			{
				sysc=tk_syscall_rv_utxt;

				pb_sysc=(tk_kptr)sysc;
				pb_sysc&=0x0000FFFFFFFFFFFEULL;
				pb_sysc|=0x0004000000000001ULL;
				sysc=(void *)pb_sysc;				
			}
#endif

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

			j=strlen(args[0]);	//BGB: Debug

			TKPE_SetupTaskForImage(task, pimg);
			bootgbr=(void *)(task->basegbr);

//			j=strlen(args[0]);	//BGB: Debug
			
			TK_SchedAddTask(task);

//			__debugbreak();
			j=strlen(args[0]);	//BGB: Debug

			tk_dbg_printf("TKSH_TryLoad: task=%p, env=%p\n", task, env1);

			tk_dbg_printf("TKPE_SetupTaskForImage: GBR=%p\n", bootgbr);
			tk_dbg_printf("TKPE_SetupTaskForImage: Entry=%p\n", bootptr);

			sza=0;
//			if(args)
			if(1)
			{
				for(i=0; args[i]; i++)
				{
					j=strlen(args[i]);
					sza+=j+(j>>2)+4;
				}

				ct=boot_newspb+1024;
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
				cs1=tk_rovalloc(sza, (void **)(&ct));
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

			tkern->task_join_ret=(tk_kptr)ptask;
			if(!ptask)
			{
				if(ctask==tk_task_syscall)
					__debugbreak();
				tkern->task_join_ret=(tk_kptr)ctask;
			}
			
			pid=task->pid;
			
			i+=*(u64 *)bootptr;
			
			pb_boot=(u64)bootptr;

			tkern->ctx_regsave[TKPE_REGSAVE_R4]=(tk_kptr)boottbr;
			if(pb_boot&0x0001000000000000ULL)
			{
				/* If Linux binary, set TP=0 */
				tkern->ctx_regsave[TKPE_REGSAVE_R4]=0;
//				tkern->ctx_regsave[TKPE_REGSAVE_R4]=
//					((tk_kptr)boot_newsp)+1024;
			}

			tkern->ctx_regsave[TKPE_REGSAVE_TTB]=tk_vmem_pageglobal;
//			tkern->ctx_regsave[TKPE_REGSAVE_SPC]=bootptr;
			tkern->ctx_regsave[TKPE_REGSAVE_SPC]=
				pb_boot&0x0000FFFFFFFFFFFEULL;
			tkern->ctx_regsave[TKPE_REGSAVE_GBR]=(tk_kptr)bootgbr;
			tkern->ctx_regsave[TKPE_REGSAVE_SSP]=(tk_kptr)boot_newsp;
//			tkern->ctx_regsave[TKPE_REGSAVE_EXSR]|=0xC000000000000000ULL;
			tkern->ctx_regsave[TKPE_REGSAVE_EXSR]|=
				0xC000000000000000ULL|
				(((pb_boot>>48)&0x000C)<<56)|
				(((pb_boot>>48)&0x00F0)<<48);
//			tkern->ctx_regsave[TKPE_REGSAVE_EXSR]|=0x8000000000000000ULL;

			if(ctask!=tk_task_syscall)
			{
				TK_Task_SyscallReturnToUser(task);
			}

#if 0
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

//			return(1);
			return(task->pid);

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

			TKSH_ExecCmdBuf(buf, ext);
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

int TKSH_TryLoad(char *img, char **args0)
{
//	if(tk_iskernel() || tk_issyscall())
	if(tk_issyscall())
	{
		return(TKSH_TryLoadB(img, args0));
	}else
	{
		return(TKSH_TryLoadA(img, args0));
	}
}

int TKSH_TryLoad_ext(char *img, char **args)
{
	char **path;
	char *s;
	char tb[256];
	int npath, ri;
	int i;

	ri=TKSH_TryLoad(img, args);
	if(ri>=0)
		return(ri);
	
	s=img+strlen(img);
	while((s>img) && (*s!='.') && (*s!='/'))
		s--;
	if(*s=='.')
		return(-1);
	
	strcpy(tb, img);
	strcat(tb, ".exe");
	ri=TKSH_TryLoad(tb, args);
	if(ri>=0)
		return(ri);
	
	strcpy(tb, img);
	strcat(tb, ".elf");
	ri=TKSH_TryLoad(tb, args);
	if(ri>=0)
		return(ri);
	
	strcpy(tb, img);
	strcat(tb, ".sh");
	ri=TKSH_TryLoad(tb, args);
	if(ri>=0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".com");
	ri=TKSH_TryLoad(tb, args);
	if(ri>=0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".pf");
	ri=TKSH_TryLoad(tb, args);
	if(ri>=0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".bxw");
	ri=TKSH_TryLoad(tb, args);
	if(ri>=0)
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

	if(*img=='/')
	{
		ri=TKSH_TryLoad_ext(tb, args);
		if(ri>0)
			return(ri);
		return(-1);
	}

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
	TK_Env_FreePathList(path);

	return(ri);
}


int TK_CreateProcessB(
	TKPE_TaskInfo *task,
	char *imgname,
	char *cmdline,
	char *envmod,
	char *cwd,
	u64 taskflags,
	TKPE_CreateTaskInfo *info)
{
	char **path;
	char **args;
	char tb[256];
	char tb_cwd[256];
	int npath, ri;
	int i;

	tk_dbg_printf("TK_CreateProcessB: img=%s cmd=%s env=%s cwd=%s\n",
		imgname, cmdline, envmod, cwd);

	tksh_createproc_ptask=task;
	tksh_createproc_info=info;

	args=tk_rsplit(cmdline);

	if((*imgname)=='/')
	{
		ri=TKSH_TryLoad_ext(imgname, args);
		if(ri>0)
		{
			tksh_createproc_ptask=NULL;
			tksh_createproc_info=NULL;
			tk_dbg_printf("TK_CreateProcessB: pid=%d\n", ri);
			return(ri);
		}
	}

#if 1
//	if(cwd)
	if(cwd && ((*imgname)!='/'))
	{
		strcpy(tb, cwd);
		strcat(tb, "/");
		strcat(tb, imgname);
		ri=TKSH_TryLoad_ext(tb, args);
		if(ri>0)
		{
			tksh_createproc_ptask=NULL;
			tksh_createproc_info=NULL;
			tk_dbg_printf("TK_CreateProcessB: pid=%d\n", ri);
			return(ri);
		}
	}
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
		{
			tksh_createproc_ptask=NULL;
			tksh_createproc_info=NULL;
			tk_dbg_printf("TK_CreateProcessB: pid=%d\n", ri);
			return(ri);
		}
	}
	TK_Env_FreePathList(path);

	ri=0;
	tksh_createproc_ptask=NULL;
	tksh_createproc_info=NULL;
	tk_dbg_printf("TK_CreateProcessB: Fail, pid=%d\n", ri);
	return(ri);
}

int TK_CreateProcess(
	char *imgname,
	char *cmdline,
	char *envmod,
	char *cwd,
	u64 taskflags,
	TKPE_CreateTaskInfo *info)
{
	TK_SysArg ar[8];
	void *p;
	int tid;
	
#ifndef __TK_CLIB_ONLY__
//	if(tk_iskernel())
	if(tk_iskerneltask())
	{
		tid=TK_CreateProcessB((void *)TK_GET_TBR,
			imgname, cmdline, envmod, cwd, taskflags, info);
		return(tid);
	}
#endif

	tid=0;
//	p=NULL;
	ar[0].p=imgname;
	ar[1].p=cmdline;
	ar[2].p=envmod;
	ar[3].p=cwd;
	ar[4].i=taskflags;
	ar[5].p=info;
	tk_syscall(NULL, TK_UMSG_CREATEPROCESS, &tid, ar);
	return(tid);
}

