extern u64 __arch_gbr;

void sanity_a()
{
	int *pi, *pj;
	int i, j, k, l;

	pi=&i;
	pj=&j;

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

	*pi=123456;
	*pj=10;
	k=i/j;
	
	if(k!=12345)
		__debugbreak();

	l=i%j;

	if(l!=6)
		__debugbreak();

	puts("A4\n");
}

void sanity_b()
{
	int *pi, *pj;
	int i, j, k, l;
	double a, b;

#if 1
	for(i=0; i<32; i++)
	{
		a=(i*(1.0/31.0))*(2*M_PI);
		printf("a=%f", a);

		b=sin(a);	
		printf(" b=%f\n", b);
	}
#endif
}

int tk_cmd2idx(char *s)
{
	if(*s=='t')
	{
		if(!strcmp(s, "test"))
			return(0);
	}

	if(*s=='l')
	{
		if(!strcmp(s, "load"))
			return(1);
	}

	if(*s=='r')
	{
		if(!strcmp(s, "run"))
			return(1);
	}

	return(-1);
}

void tk_tryload(char *img, char **args)
{
	TK_FILE *fd;
	u64 bootgbr;
	int (*bootptr)();

	fd=tk_fopen(img, "rb");	
	if(fd)
	{
		bootgbr=0;
		TKPE_LoadStaticPE(fd, &bootptr, &bootgbr);
		printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
		if(bootptr)
		{
			__arch_gbr=bootgbr;
			bootptr();
		}
	}
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
	sanity_b();
	
	TK_RamChk();
	
//	TKSPI_InitDevice();
	tk_vfile_init();

	puts("Boot 1\n");

	tk_tryload("bootload.sys", NULL);

#if 0
	fd=tk_fopen("bootload.sys", "rb");	
	if(fd)
	{
		TKPE_LoadStaticPE(fd, &bootptr, &bootgbr);
		printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
		if(bootptr)
		{
			__arch_gbr=bootgbr;
			bootptr();
		}
	}else
//	if(!fd)
	{
		puts("Fail Open BOOTLOAD.SYS\n");
//		return;
	}
#endif

//	puts("Boot Failed\n");
	puts("No BOOTLOAD.SYS found\n");
	while(1)
	{
		puts("$ ");
		gets(tbuf);
		a=tk_rsplit(tbuf);
		if(!a[0])
			continue;
		ci=tk_cmd2idx(a[0]);

#if 1
		switch(ci)
		{
		case 0:
			printf("Test Command '%s'\n", a[0]);
			break;
		case 1:
			if(a[1])
			{
				tk_tryload(a[1], a+1);
			}else
			{
				printf("usage: %s <image> [args*]\n", a[0]);
			}
			break;

		default:
			printf("Unhandled Command '%s'\n", a[0]);
			break;
		}
#endif
	}

	puts("Loop Terminate\n");
}
