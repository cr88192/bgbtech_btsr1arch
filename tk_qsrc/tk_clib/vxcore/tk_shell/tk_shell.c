extern u64 __arch_gbr;

char tb_cwd[256];

int tk_cmd2idx(char *s)
{
	if(*s=='c')
	{
		if(!strcmp(s, "cd"))
			return(3);

		if(!strcmp(s, "cls"))
			return(4);
	}

	if(*s=='d')
	{
		if(!strcmp(s, "dir"))
			return(2);
	}

	if(*s=='l')
	{
		if(!strcmp(s, "load"))
			return(1);
		if(!strcmp(s, "ls"))
			return(2);
	}

	if(*s=='r')
	{
		if(!strcmp(s, "run"))
			return(1);
	}

	if(*s=='t')
	{
		if(!strcmp(s, "test"))
			return(0);
	}

	return(-1);
}

void __tk_farcall(void *fptr, void *gbr, void *newstack);

int tk_tryload(char *img, char **args)
{
	TK_FILE *fd;
//	u64 bootgbr;
	void *bootgbr;
	byte *boot_newspb, *boot_newsp;
	int (*bootptr)();
	int rv;

	fd=tk_fopen(img, "rb");	
	if(fd)
	{
		bootgbr=0;
		TKPE_LoadStaticPE(fd, &bootptr, &bootgbr);
		printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
		if(bootptr)
		{
			boot_newspb=TKMM_PageAlloc(1<<18);
			boot_newsp=boot_newspb+((1<<18)-1024);
		
			rv=tk_sysc_exitpt();
			if(rv)
			{
				tk_con_chkreset();
//				__debugbreak();
				return(rv);
			}
		
//			__arch_gbr=bootgbr;
//			bootptr();
			__tk_farcall(bootptr, bootgbr, boot_newsp);
		}
	}
	
	return(-1);
}

void tk_dir(char *path, char **args)
{
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
		tk_printf("%s\n", tde->d_name);
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

int main(int argc, char *argv[])
{
	char tbuf[256];
	char tb[256], tb1[256];
	char **a;
	int ci, ri;

	tk_con_reset();

	strcpy(tb_cwd, "/");

	while(1)
	{
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
			tk_printf("Test Command '%s'\n", a[0]);
			break;
		case 1:
			if(a[1])
			{
				tk_tryload(a[1], a+1);
			}else
			{
				tk_printf("usage: %s <image> [args*]\n", a[0]);
			}
			break;

		case 2:
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

		case 3:
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
			}
			break;

		case 4:
			tk_con_reset();
			break;

		default:
//			strcpy(tb, a[0]);
			strcpy(tb, tb_cwd);
			strcat(tb, "/");
			strcat(tb, a[0]);
			strcat(tb, ".exe");
			ri=tk_tryload(tb, a);
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
