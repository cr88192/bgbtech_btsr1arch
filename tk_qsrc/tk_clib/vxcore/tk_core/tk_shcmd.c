int TKSH_TryLoad(char *img, char **args);
int TKSH_TryLoad_n(char *img, char **args);

extern u64 __arch_gbr;

TK_APIEXPORT
int tk_isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2);

TKSH_CommandInfo *TKSH_LookupCommand(char *name);
TKSH_CommandInfo *TKSH_CreateCommand(char *name);


TKSH_CommandInfo *tksh_commands=NULL;
TKSH_CommandInfo *tksh_command_hash[64];
int tksh_commands_init;

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
	char *s0, *cs, *ct, *cte;
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
			printf("%-32s %9d %08X\n",
				tde->d_name,
				tde->st_size,
				tde->st_mtime);
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
				
				if(tde->st_mode&TKFAT_EMODE_DIR)
					tk_puts("\x1B[34m");
				
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

int THSH_QualifyPathArg(char *dst, char *src)
{
	char tb_cwd[256];

	if(src)
	{
		if(src[0]=='/')
		{
			strcpy(dst, src);
		}else
		{
			TK_Env_GetCwd(tb_cwd, 256);
			strcpy(dst, tb_cwd);
			strcat(dst, "/");
			strcat(dst, src);
		}
	}else
	{
		TK_Env_GetCwd(dst, 256);
	}
	return(0);
}

int TKSH_Cmds_Ls(char **args)
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
	char *darg, *sarg;
	int i;

	darg=NULL;
	sarg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!sarg)
			{ sarg=args[i]; continue; }
		if(!darg)
			{ darg=args[i]; continue; }
	}

	if(sarg && darg)
	{
		THSH_QualifyPathArg(tb1, sarg);
		THSH_QualifyPathArg(tb2, darg);
		tk_rename(tb1, tb2);
//		tk_unlink(tb);
	}else
	{
	}
	return(0);
}

int TKSH_Cmds_Cp(char **args)
{
	char tb1[256];
	char tb2[256];
	char *darg, *sarg;
	int i;

	darg=NULL;
	sarg=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!sarg)
			{ sarg=args[i]; continue; }
		if(!darg)
			{ darg=args[i]; continue; }
	}

	if(sarg && darg)
	{
		THSH_QualifyPathArg(tb1, sarg);
		THSH_QualifyPathArg(tb2, darg);
		tk_fcopy(tb1, tb2);
	}else
	{
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
	TKSH_RegisterCommand("echo",	TKSH_Cmds_Echo);
	TKSH_RegisterCommand("ls",		TKSH_Cmds_Ls);
	TKSH_RegisterCommand("mv",		TKSH_Cmds_Mv);
	TKSH_RegisterCommand("recvxm",	TKSH_Cmds_RecvXm);
	TKSH_RegisterCommand("rm",		TKSH_Cmds_Rm);
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

void __tk_farcall(void *fptr, void *gbr, void *newstack, void *tbr);
int TK_DestroyTaskInfo(void *tkptr);

int TKSH_TryLoad(char *img, char **args)
{
	byte tb[256];
	TK_FILE *fd;
	char **a1;
	char *cs, *ct;
	char *buf;
//	u64 bootgbr;
	u64	pb_gbr;
	TKPE_TaskInfo *task;
	TK_EnvContext *env0, *env1;
	void *bootgbr;
	byte *boot_newspb, *boot_newsp;
	void *boottbr;
	int (*bootptr)();
	int sig_is_pe, sig_is_asc;
	int rv, nl, sz;
	int i, j, k;

	fd=tk_fopen(img, "rb");
	
	if(!fd)
		return(-1);
	
	while(fd)
	{
		tk_fseek(fd, 0, 0);
		memset(tb, 0, 256);
		tk_fread(tb, 1, 255, fd);
		
		if((tb[0]!='#') || (tb[1]!='!'))
			break;

		cs=tb+2; ct=tb;
		while(*cs>=' ')
			*ct++=*cs++;
		*ct++=0;
		
		tk_fclose(fd);
		fd=tk_fopen(tb, "rb");
	}
	
	sig_is_pe=0;
	if((tb[0]=='M') && (tb[1]=='Z'))
		sig_is_pe=1;
	if((tb[0]=='P') && (tb[1]=='E'))
		sig_is_pe=1;
	
//	if(fd)
	if(fd && sig_is_pe)
	{
		bootgbr=0;
		TKPE_LoadStaticPE(fd, &bootptr, &bootgbr);
		tk_printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
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
		
		if(bootptr)
		{
			boot_newspb=TKMM_PageAlloc(1<<18);
			boot_newsp=boot_newspb+((1<<18)-1024);

			env0=TK_GetCurrentEnvContext();
			env1=TK_EnvCtx_CloneContext(env0);

			boottbr=TK_AllocNewTask();
			task=boottbr;
			task->bootptr=bootptr;
			task->basegbr=bootgbr;
			task->boottbr=boottbr;

			task->boot_sps=boot_newspb;
			task->boot_spe=boot_newsp;
			
			task->envctx=env1;
			task->SysCall=tk_isr_syscall;
			
			tk_printf("TKSH_TryLoad: task=%p, env=%p\n", task, env1);
			
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

			rv=tk_sysc_exitpt();
			if(rv)
			{
				TK_DestroyTaskInfo(task);
				tk_con_chkreset();
//				__debugbreak();
				return(rv);
			}
		
//			__arch_gbr=bootgbr;
//			bootptr();
			__tk_farcall(bootptr, bootgbr, boot_newsp, boottbr);
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
			tk_fseek(fd, 0, 2);
			sz=tk_ftell(fd);
			tk_fseek(fd, 0, 0);
			buf=tk_malloc(sz+16);
			memset(buf, 0, sz+8);
			tk_fread(buf, 1, sz, fd);
			tk_fclose(fd);

			TKSH_ExecCmdBuf(buf);
			tk_free(buf);
			return(1);
		}
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
	strcat(tb, ".pf");
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

