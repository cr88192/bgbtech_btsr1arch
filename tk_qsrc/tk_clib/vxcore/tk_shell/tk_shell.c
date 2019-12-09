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

	if(*s=='t')
	{
		if(!strcmp(s, "test"))
			return(0);
	}

	return(-1);
}

void __tk_farcall(void *fptr, void *gbr, void *newstack, void *tbr);
int TK_DestroyTaskInfo(void *tkptr);

int tk_tryload(char *img, char **args)
{
	byte tb[256];
	TK_FILE *fd;
	char *cs, *ct;
//	u64 bootgbr;
	TKPE_TaskInfo *task;
	void *bootgbr;
	byte *boot_newspb, *boot_newsp;
	void *boottbr;
	int (*bootptr)();
	int sig_is_pe;
	int rv;

	fd=tk_fopen(img, "rb");
	
	while(fd)
	{
		tk_fseek(fd, 0, 0);
		tk_fread(tb, 1, 256, fd);
		
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

			boottbr=TK_AllocNewTask();
			task=boottbr;
			task->bootptr=bootptr;
			task->basegbr=bootgbr;
			task->boottbr=boottbr;
		
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
	
//	TK_DestroyTaskInfo(NULL);
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
	strcat(tb, ".exe");
	ri=tk_tryload(tb, args);
	if(ri>0)
		return(ri);
#endif

	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		strcpy(tb, path[i]);
		strcat(tb, "/");
		strcat(tb, args[0]);
		strcat(tb, ".exe");
		ri=tk_tryload(tb, args);
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
	char tbuf[256];
	char tb[256], tb1[256];
	char **a;
	int ci, ri;

	tk_shell_chksane();

	tk_con_reset();

	strcpy(tb_cwd, "/");

	while(1)
	{
//		tk_con_chkreset();
		tk_puts(tb_cwd);
		tk_puts("$ ");
		tk_gets(tbuf);
		a=tk_rsplit(tbuf);
		if(!a[0])
			continue;
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
	}

	puts("Loop Terminate\n");
}
