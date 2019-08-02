extern u64 __arch_gbr;

void sanity_a()
{
	int i, j, k;
	
	*(int *)(&i)=0x12345678;
	*(int *)(&j)=0x1234CDEF;

	puts("A0\n");
	
	k=i+j;	if(k!=0x24692467)
		__debugbreak();

	puts("A0-0\n");

	k=i&j;	if(k!=0x12344468)
		__debugbreak();

	puts("A0-1\n");

	k=i|j;	if(k!=0x1234DFFF)
		__debugbreak();
	k=i^j;	if(k!=0x00009B97)
		__debugbreak();

	puts("A1\n");

	k=i<<9;	if(k!=0x68ACF000)
		__debugbreak();

	puts("A2\n");

	k=i>>9;	if(k!=0x00091A2B)
		__debugbreak();

	puts("A3\n");
}

int tk_cmd2idx(char *s)
{
	if(*s=='t')
	{
		if(!strcmp(s, "test"))
			return(0);
	}
	return(-1);
}

void __start()
{
	char tbuf[256];
	char **a;
	TK_FILE *fd;
	u64 bootgbr;
	int (*bootptr)();
	int ci;

	tk_con_init();
	puts("Boot 0\n");
	printf("Print 0 %0X %0X %0X %0X\n",
		0x12345678, 0x9ABCDEF0, 0x1234CDEF, 0x9ABC5678);
		
	sanity_a();
	
	tk_vfile_init();

	puts("Boot 1\n");
	
	fd=tk_fopen("bootload.sys", "rb");
	if(!fd)
	{
		puts("Fail Open BOOTLOAD.SYS\n");
//		return;
	}
	
	if(fd)
	{
		TKPE_LoadStaticPE(fd, &bootptr, &bootgbr);
		printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
		if(bootptr)
		{
			__arch_gbr=bootgbr;
			bootptr();
		}
	}

	puts("Boot Failed\n");
	while(1)
	{
		puts("$ ");
		gets(tbuf);
		a=tk_rsplit(tbuf);
		if(!a[0])
			continue;
		ci=tk_cmd2idx(a[0]);
#if 0
		switch(ci)
		{
		case 0:
			printf("Test Command '%s'\n", a[0]);
			break;
		default:
			printf("Unhandled Command '%s'\n", a[0]);
			break;
		}
#endif
	}

	puts("Loop Terminate\n");
}
