#define FOURCC(a, b, c, d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))
#define BOOTPARM_MAGIC1		FOURCC('B', 'O', 'O', 'T')
#define BOOTPARM_MAGIC2		FOURCC('P', 'A', 'R', 'M')
// #define BOOTPARM_MAGIC3		FOURCC('P', 'a', 'r', 'm')

#define BOOTPARM_MAGIC3		FOURCC('M', 'R', 'A', 'P')
#define BOOTPARM_MAGIC4		FOURCC('T', 'O', 'O', 'B')


extern u64 __arch_gbr;
extern int tk_ram_kib;

struct BootParm_s {
u32 magic1;		//00
u32 magic2;		//04
u32 ramkib;		//08
u32 conctx;		//0C

u32 resv1;		//10
u32 resv2;		//14
u32 resv3;		//18
u32 resv4;		//1C

u32 resv5;		//20
u32 resv6;		//24
u32 resv7;		//28
u32 resv8;		//2C

u32 resv9;		//30
u32 resv10;		//34
u32 magic3;		//38
u32 magic4;		//3C
};

struct BootParm_s bootparm;

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

void TK_FlushCacheL1D_INVDCA(void);
void TK_FlushCacheL1D_INVL2(void);
void TK_FlushCacheL1D_INVDC(void *ptr);
void TK_FlushCacheL1D_INVIC(void *ptr);
void TK_FlushCacheL1D_ReadBuf(void *ptr, int sz);
__asm {
TK_FlushCacheL1D_INVDCA:
	MOV		-1, R4
	INVDC	R4
	NOP
	NOP
	RTS

TK_FlushCacheL1D_INVL2:
	MOV		-2, R4
	INVDC	R4
	NOP
	NOP
	RTS

TK_FlushCacheL1D_INVDC:
	INVDC	R4
	NOP
	NOP
	RTS
TK_FlushCacheL1D_INVIC:
	INVIC	R4
	NOP
	NOP
	RTS

TK_FlushCacheL1D_ReadBuf:
	.L0:
	MOV.Q	(R4), R7
	ADD		-16, R5
	ADD		16, R4
	CMP/GT	0, R5
	BT		.L0
	RTS
};

void TK_FlushCacheL1D(void *ptr)
{
	void *pptr;
	pptr=(void *)(((u64)ptr)&(~16777215));
	
	TK_FlushCacheL1D_INVDC(NULL);
//	TK_FlushCacheL1D_ReadBuf(pptr, 65536);
	TK_FlushCacheL1D_ReadBuf(pptr, 262144);
	TK_FlushCacheL1D_INVDC(NULL);
//	TK_FlushCacheL1D_ReadBuf(pptr, 65536);
	TK_FlushCacheL1D_ReadBuf(pptr, 262144);

	TK_FlushCacheL1D_INVIC(NULL);
}

void tk_tryload(char *img, char **args)
{
	TK_FILE *fd;
	u64 bootgbr, pb_gbr;
	int (*bootptr)();

	fd=tk_fopen(img, "rb");	
	if(fd)
	{
		bootgbr=0;
		TKPE_LoadStaticPE(fd, &bootptr, &bootgbr);
		printf("Boot Pointer %p, GBR=%p\n", bootptr, (void *)bootgbr);
		
		pb_gbr=*(u64 *)bootgbr;
		if(pb_gbr!=bootgbr)
		{
			printf("Boot GBR Mismatch, Got=%p Expect=%p\n", pb_gbr, bootgbr);
		}
		
		if(bootptr)
		{
			TK_FlushCacheL1D(bootptr);
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
	int ci, cj, ck;

	ci = TK_GetTimeMs();
//	__debugbreak();

	tk_con_init();
	puts("Boot 0\n");
	printf("Print 0 %0X %0X %0X %0X\n",
		0x12345678, 0x9ABCDEF0, 0x1234CDEF, 0x9ABC5678);
		
	sanity_a();
//	sanity_b();

#ifndef JX2UC

//	ci=I_GetTime();
	ci = TK_GetTimeMs();
	printf("%d\n", ci);
	
	TK_RamChk();
	
//	ci=I_GetTime();
	ci = TK_GetTimeMs();
	printf("%d\n", ci);

//	ci = TK_GetTimeMs();
	TK_RamBench();
//	cj = TK_GetTimeMs();
//	ck = cj-ci;
//	if(ck>0)
//	{
//		printf("memcpy: %d MB/s\n", 1000/ck);
//	}
	
	while(tk_dbg_kbhit())
		{ tk_dbg_getch(); }
	while(kbhit())
		{ getch(); }

//	ci=I_GetTime();
	ci = TK_GetTimeMs();

	printf("%d\n", ci);
	
//	TKSPI_InitDevice();
	tk_vfile_init();

	bootparm.magic1=BOOTPARM_MAGIC1;
	bootparm.magic2=BOOTPARM_MAGIC2;
	bootparm.ramkib=tk_ram_kib;
	bootparm.conctx=(u32)(tk_con_getctx());
	bootparm.magic3=BOOTPARM_MAGIC3;
	bootparm.magic4=BOOTPARM_MAGIC4;

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
//				tk_tryload(a[1], a+1);
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
