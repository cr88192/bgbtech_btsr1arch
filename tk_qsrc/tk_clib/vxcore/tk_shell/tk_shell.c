int tk_tryload(char *img, char **args);
int tk_tryload_n(char *img, char **args);

extern u64 __arch_gbr;

char tb_cwd[256];

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

int TkSh_ExecCmd(char *cmd)
{
	char tb[256], tb1[256], tb2[256];
	char **a;
	char *s0, *s1;
	int ci, ri;
	int i, j, k;

	a=tk_rsplit(cmd);
	if(!a[0])
		return(0);
	ci=tk_cmd2idx(a[0]);

#if 1
	switch(ci)
	{
	case 0:
//			tk_printf("Test Command '%s'\n", a[0]);
		break;
	case TK_CMD_RUN:
		if(a[1])
		{
			tk_tryload(a[1], a+1);
		}else
		{
			tk_printf("usage: %s <image> [args*]\n", a[0]);
		}
		break;

	case TK_CMD_LS:
		if(a[1])
		{
			if(a[1][0]=='/')
			{
				strcpy(tb, a[1]);
			}else
			{
				strcpy(tb, tb_cwd);
				strcat(tb, "/");
				strcat(tb, a[1]);
			}
//				tk_dir(a[1], a+1);
			tk_dir(tb, a+1);
		}else
		{
			tk_dir(tb_cwd, a+1);
//				printf("usage: %s <path> [args*]\n", a[0]);
		}
		break;

	case TK_CMD_CD:
		if(a[1])
		{
			if(a[1][0]=='/')
			{
				strcpy(tb, a[1]);
			}else
			{
				strcpy(tb, tb_cwd);
				if(tb[strlen(tb)-1]!='/')
					strcat(tb, "/");
				strcat(tb, a[1]);
			}
//				tk_dir(a[1], a+1);
//				tk_dir(tb, a+1);
//				strcpy(tb_cwd, tb);
			tk_normalize_path(tb_cwd, tb);
			TK_Env_SetCwd(tb_cwd);
		}else
		{
//				tk_dir(tb_cwd, a+1);
//				printf("usage: %s <path> [args*]\n", a[0]);
			strcpy(tb_cwd, "/");
			TK_Env_SetCwd(tb_cwd);
		}
		break;

	case TK_CMD_CLS:
		tk_con_reset();
		break;

	case TK_CMD_ECHO:
		for(i=1; a[i]; i++)
		{
			s0=a[i];
			
			if(*s0=='$')
			{
				s1=TK_Env_GetEnvVarI(s0+1, tb1, 256);
//				if(*s1)
//					{ tk_puts(s1); }
				if(s1)
					{ tk_puts(tb1); }
				continue;
			}
			
			tk_puts(s0);
		}
		tk_putc('\n');
		break;

	case TK_CMD_RECVXM:
		if(a[1])
		{
			if(a[1][0]=='/')
			{
				strcpy(tb, a[1]);
			}else
			{
				strcpy(tb, tb_cwd);
				strcat(tb, "/");
				strcat(tb, a[1]);
			}
			TK_Dbg_RecvFileXM(tb);
		}
		break;

	case TK_CMD_RM:
		if(a[1])
		{
			if(a[1][0]=='/')
			{
				strcpy(tb, a[1]);
			}else
			{
				strcpy(tb, tb_cwd);
				strcat(tb, "/");
				strcat(tb, a[1]);
			}
			tk_unlink(tb);
		}
		break;

	case TK_CMD_MV:
		if(a[1])
		{
			if(a[1][0]=='/')
			{
				strcpy(tb, a[1]);
			}else
			{
				strcpy(tb, tb_cwd);
				strcat(tb, "/");
				strcat(tb, a[1]);
			}

			if(a[2][0]=='/')
			{
				strcpy(tb1, a[2]);
			}else
			{
				strcpy(tb1, tb_cwd);
				strcat(tb1, "/");
				strcat(tb1, a[2]);
			}
			tk_rename(tb, tb1);
		}
		break;

	case TK_CMD_CP:
		if(a[1])
		{
			if(a[1][0]=='/')
			{
				strcpy(tb, a[1]);
			}else
			{
				strcpy(tb, tb_cwd);
				strcat(tb, "/");
				strcat(tb, a[1]);
			}

			if(a[2][0]=='/')
			{
				strcpy(tb1, a[2]);
			}else
			{
				strcpy(tb1, tb_cwd);
				strcat(tb1, "/");
				strcat(tb1, a[2]);
			}
			tk_fcopy(tb, tb1);
		}
		break;

	case TK_CMD_SET:
		if(a[1])
		{
			TK_Env_UpdateForSet(a[1]);
		}else
		{
			for(i=0;; i++)
			{
				j=TK_Env_GetEnvVarIdx(i, tb1, tb2, 256, 256);
				if(j<=0)break;
				tk_printf("%s=%s\n", tb1, tb2);
			}
		}
		break;

	default:
#if 0
		TK_Env_GetCwd(tb_cwd, 256);
//			strcpy(tb, a[0]);
		strcpy(tb, tb_cwd);
		strcat(tb, "/");
		strcat(tb, a[0]);
		strcat(tb, ".exe");
		ri=tk_tryload(tb, a);
#endif

		ri=tk_tryload_n(tb, a);

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

int TkSh_ExecCmdBuf(char *cmd)
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
			TkSh_ExecCmd(tb);
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

int tk_tryload(char *img, char **args)
{
	byte tb[256];
	TK_FILE *fd;
	char **a1;
	char *cs, *ct;
	char *buf;
//	u64 bootgbr;
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
		printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
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
			
			tk_printf("tk_tryload: task=%p, env=%p\n", task, env1);
			
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
	//		TkSh_ExecCmdFd(fd);
			tk_fseek(fd, 0, 2);
			sz=tk_ftell(fd);
			tk_fseek(fd, 0, 0);
			buf=tk_malloc(sz+16);
			memset(buf, 0, sz+8);
			tk_fread(buf, 1, sz, fd);
			tk_fclose(fd);

			TkSh_ExecCmdBuf(buf);
			tk_free(buf);
			return(1);
		}
	}
	
//	TK_DestroyTaskInfo(NULL);
	return(-1);
}

int tk_tryload_ext(char *img, char **args)
{
	char **path;
	char tb[256];
	int npath, ri;
	int i;

	ri=tk_tryload(img, args);
	if(ri>0)
		return(ri);
	
	strcpy(tb, img);
	strcat(tb, ".exe");
	ri=tk_tryload(tb, args);
	if(ri>0)
		return(ri);

	strcpy(tb, img);
	strcat(tb, ".pf");
	ri=tk_tryload(tb, args);
	if(ri>0)
		return(ri);
	
	return(-1);
}


int tk_tryload_n(char *img, char **args)
{
	char **path;
	char tb[256];
	int npath, ri;
	int i;

#if 1
	TK_Env_GetCwd(tb_cwd, 256);
//	strcpy(tb, a[0]);
	strcpy(tb, tb_cwd);
	strcat(tb, "/");
	strcat(tb, args[0]);
//	strcat(tb, ".exe");
//	ri=tk_tryload(tb, args);
	ri=tk_tryload_ext(tb, args);
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
//		ri=tk_tryload(tb, args);
		ri=tk_tryload_ext(tb, args);
		if(ri>0)
			return(ri);
	}

	return(ri);
}

void tk_dir(char *path, char **args)
{
	char tb[128];
	TK_DIR *dir;
	TK_DIRENT *tde;
	
	dir=tk_opendir(path);
	if(!dir)
	{
		return;
	}
	
	tde=tk_readdir(dir);
	while(tde)
	{
//		if((tde->d_name[0]=='!') && (tde->d_name[1]=='!'))
//		{
//			tde=tk_readdir(dir);
//			continue;
//		}
	
//		memset(tb, 0x20, 128);
//		memcpy(tb, tde->d_name, strlen(tde->d_name));
//		tb[32]=0;
//		tk_printf("%032s %8d\n", tde->d_name, tde->st_size);
//		printf("%032s %9d\n", tb, tde->st_size);
		printf("%-32s %9d %08X\n", tde->d_name, tde->st_size, tde->st_mtime);
		tde=tk_readdir(dir);
	}
}

void tk_normalize_path(char *dst, char *src)
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

int tk_shell_chksane()
{
	char tb[64];

	int *pi, *pj;
	int i, j, k, l;
	
	pi=&i;
	pj=&j;
	
	*pi=123456;
	*pj=10;
	k=i/j;
	l=i%j;
	
	if(k!=12345)
		__debugbreak();
	if(l!=6)
		__debugbreak();
		
	strcpy(tb, "0123456789ABCDEF");
	*pi=1;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "G1234567", 8))
		__debugbreak();
	*pi=3;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHI34567", 8))
		__debugbreak();
	*pi=5;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHIJK567", 8))
		__debugbreak();
	*pi=7;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHIJKLM7", 8))
		__debugbreak();

	strcpy(tb, "0123456789ABCDEF");
	*pi=1;
	memset(tb, ' ', *pi);
	if(memcmp(tb, " 1234567", 8))
		__debugbreak();
	*pi=3;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "   34567", 8))
		__debugbreak();
	*pi=5;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "     567", 8))
		__debugbreak();
	*pi=7;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "       7", 8))
		__debugbreak();

}

int main(int argc, char *argv[])
{
	TKPE_TaskInfo *task;
	char tbuf[256];

	tk_shell_chksane();

	tk_con_reset();

	task=TK_AllocNewTask();
	TK_SetCurrentTask(task);

	strcpy(tb_cwd, "/");
	TK_Env_SetCwd(tb_cwd);

	tk_tryload("autoexec.pf", NULL);

	while(1)
	{
//		tk_con_chkreset();
		tk_puts(tb_cwd);
		tk_puts("$ ");
		tk_gets(tbuf);
		TkSh_ExecCmd(tbuf);
	}

	puts("Loop Terminate\n");
}
