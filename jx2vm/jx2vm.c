#include "jx2i_multi.c"

FILE *bjx2_vmoutlog=NULL;

#ifdef linux
void SoundDev_Submit()
{
}

int SoundDev_WriteStereoSamples(short *buffer, int cnt)
{
}

int SoundDev_Init()
{
}

int SoundDev_DeInit()
{
}
#endif

#ifdef linux
int	gfxdrv_kill=0;

int GfxDrv_Start()
{
}

int GfxDrv_PrepareFramebuf()
{
}

void GfxDrv_BeginDrawing()
{
}

void GfxDrv_EndDrawing(void)
{
}

int GfxDrv_MainLoop(void (*fcn)())
{
	static int lt=0;
	int ct, dt;
	
	ct=FRGL_TimeMS();
	while(!gfxdrv_kill)
	{
		ct=FRGL_TimeMS();
		dt=ct-lt;
//		if((dt>=0) && (dt<10))
		if((dt>=0) && (dt<5))
		{
//			Sleep(1);
			usleep(0);
			continue;
		}
		
		fcn();
		lt=ct;
	}
	return(0);
}

#endif


int BJX2_MainAddKeyByte(BJX2_Context *ctx, int k)
{
	ctx->kbbuf[ctx->kbrov]=k;
	ctx->kbrov=(ctx->kbrov+1)&255;
	ctx->kbirq++;
	return(0);
}

int BJX2_MainPollKeyboard(BJX2_Context *ctx)
{
	u16 *kb;
	int i, j, k;

#ifdef _WIN32
	while(_kbhit())
	{
		j=_getch();
		
		if(j=='`')
			ctx->req_kill=1;

//		if(j=='\r')
//			j=0x0A0D;
		
		ctx->kbbuf[ctx->kbrov]=j;
		ctx->kbrov=(ctx->kbrov+1)&255;
		ctx->kbirq=1;
		
		if(j>>8)
		{
			ctx->kbbuf[ctx->kbrov]=j>>8;
			ctx->kbrov=(ctx->kbrov+1)&255;
			ctx->kbirq++;
		}
	}
#endif

#if 1
	kb=FRGL_GetKeybuf();
	while(*kb)
	{
		k=*kb++;
		if(k&0x8000)
		{
			j=k&4095;
			if(j<0x7F)
			{
				BJX2_MainAddKeyByte(ctx, j|0x80);
			}else if(j<0xFF)
			{
				BJX2_MainAddKeyByte(ctx, 0xFF);
				BJX2_MainAddKeyByte(ctx, j);
			}
		}else
		{
			j=k&4095;
			if(j<0x7F)
			{
				BJX2_MainAddKeyByte(ctx, j);
			}else if(j<0xFF)
			{
				BJX2_MainAddKeyByte(ctx, 0x7F);
				BJX2_MainAddKeyByte(ctx, j);
			}
		}
	}
#endif

	return(0);
}



BJX2_FILE *bjx2_fopen(char *name, char *mode)
{
	FILE *fd;
	fd=fopen(name, mode);
	return((BJX2_FILE *)fd);
}

int bjx2_fclose(BJX2_FILE *fd)
{
	fclose((FILE *)fd);
	return(0);
}

int bjx2_feof(BJX2_FILE *fd)
{
	return(feof((FILE *)fd));
}

s64 bjx2_ftell(BJX2_FILE *fd)
{
	return(ftell((FILE *)fd));
}

int bjx2_fseek(BJX2_FILE *fd, s64 pos, int md)
{
	return(fseek((FILE *)fd, pos, md));
}

int bjx2_fread(void *buf, int n, int m, BJX2_FILE *fd)
{
	return(fread(buf, n, m, (FILE *)fd));
}

int bjx2_fwrite(void *buf, int n, int m, BJX2_FILE *fd)
{
	return(fwrite(buf, n, m, (FILE *)fd));
}

int bjx2_fgetc(BJX2_FILE *fd)
{
	return(fgetc((FILE *)fd));
}

int bjx2_fputc(int val, BJX2_FILE *fd)
{
	return(fputc(val, (FILE *)fd));
}

char *bjx2_fgets(char *buf, int lim, BJX2_FILE *fd)
{
	char *t;
	int i;
	
	t=buf;
	while(!bjx2_feof(fd))
	{
		i=bjx2_fgetc(fd);
		if(i=='\n')
		{
			break;
		}
		if(i=='\r')
		{
			continue;
		}
		if(i<0)
		{
			break;
		}
		
		*t++=i;
	}
	
	*t=0;
	
	return(buf);
}

int bjx2_fflush(BJX2_FILE *fd)
{
	return(fflush((FILE *)fd));
}


int bjx2_vmputc(BJX2_Context *ctx, int val)
{
	if(bjx2_vmoutlog)
		fputc(val, bjx2_vmoutlog);

	fputc(val, stdout);
	return(0);

#if 0
	static char pbuf[1024];
	static int pbpos=0;
	
	if((val>=' '))
	{
		pbuf[pbpos++]=val;
		return(0);
	}
	
	if(val=='\n')
	{
		pbuf[pbpos++]=val;
		pbuf[pbpos++]=0;
//		BGBRPI_AddConMessage(pbuf);
		puts(pbuf);
		pbpos=0;
		return(0);
	}
#endif

	return(0);
}

int BJX2_VmMsgRxUpdate(BJX2_Context *ctx)
{
	return(0);
}

int BJX2_VmMsgTxUpdate(BJX2_Context *ctx)
{
//	BGBDT_Jx2Vm_UpdateMessages();
	return(0);
}



BJX2_Context *ctx;
int ms0, lms1;

void btesh_main_iterate()
{
	s64 cyc;
	int ms1, ms, dtms;
	int i, rcy, rsc;

	ms1=FRGL_TimeMS();
	ms=ms1-ms0;
//	cyc=100000LL*ms;
	cyc=ctx->tgt_mhz*1000LL*ms;
	dtms=ms1-lms1;
	lms1=ms1;

	if(ctx->tot_cyc>cyc)
	{
		BJX2_MainPollKeyboard(ctx);
		BJX2_SndSblk_Update(ctx, dtms);
#ifdef linux
		usleep(0);
#else
//		Sleep(1);
		Sleep(0);
#endif
		return;
	}

	BJX2_SndSblk_Update(ctx, dtms);
	JX2I_GfxCon_Update();

//	rcy=ctx->tgt_mhz*1000000/45;
	rcy=(ctx->tgt_mhz*1000000LL)/72;
//	rcy=(ctx->tgt_mhz*1000000LL)/45;
//	rcy=(ctx->tgt_mhz*1000000LL)/30;
//	rcy=(ctx->tgt_mhz*1000000LL)/15;
	
//	rsc=256+((ctx->tgt_mhz-100)>>3);
	rsc=256+((ctx->tgt_mhz-100)>>2);
//	rsc=256+((ctx->tgt_mhz-100)>>1);
//	rsc=(256*ctx->tgt_mhz)/100;

//	if(rsc>256)
//		rcy=(((s64)rcy)*rsc)>>8;

	GfxDrv_BeginDrawing();
	BJX2_MainPollKeyboard(ctx);
//	i=BJX2_RunLimit(ctx, 999999);
//	i=BJX2_RunLimit(ctx, 9999999);
//	i=BJX2_RunLimit(ctx, 5000000);
//	i=BJX2_RunLimit(ctx, 2500000);
//	i=BJX2_RunLimit(ctx, rcy);

	do {
		i=BJX2_RunLimit(ctx, rcy);

		if(i || ctx->req_kill)
			break;

		ms1=FRGL_TimeMS();
		ms=ms1-lms1;
	} while((ctx->tot_cyc<cyc) && (ms<28));
	
#ifndef linux
	if(i || ctx->req_kill)
		gfxdrv_kill=1;
#endif
	
//	t1=clock();
//	if(t1>t0)
//		tt+=t1-t0;
//	t0=t1;

	GfxDrv_EndDrawing();
}

int main(int argc, char *argv[])
{
	char *rd_add[64];
	char *rd_exp[64];
	char *rd_map[64];
	int rd_n_add;
	int rd_n_exp;
	int rd_n_map;

	char *ifn;
	double tsec;
	int t0, t1, tt, fbtt, tvus;
	int ifmd, rdsz, mhz;
	int i;
	
	rd_n_add=0;
	rd_n_exp=0;
	rd_n_map=0;
	ifn=NULL;
	ifmd=0; rdsz=128;
	mhz=100;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--rd_sz"))
				{ rdsz=atoi(argv[i+1]); i++; continue; }
			if(!strcmp(argv[i], "--rd_add"))
				{ ifmd=1; continue; }
			if(!strcmp(argv[i], "--rd_exp"))
				{ ifmd=2; continue; }
			if(!strcmp(argv[i], "--rd_map"))
				{ ifmd=3; continue; }

			if(!strcmp(argv[i], "--mhz"))
				{ mhz=atoi(argv[i+1]); i++; continue; }

			continue;
		}
		
		if(!ifn)
			{ ifn=argv[i]; continue; }

		if(ifmd==1)
			{ rd_add[rd_n_add++]=argv[i]; continue; }
		if(ifmd==2)
			{ rd_exp[rd_n_exp++]=argv[i]; continue; }
		if(ifmd==3)
			{ rd_map[rd_n_map++]=argv[i]; continue; }
	}
	
	bjx2_vmoutlog=fopen("bjx2_vmoutlog.txt", "wb");
	
	JX2R_UseImageCreateRamdisk(rdsz*1024);
	
	for(i=0; i<rd_n_add; i++)
		JX2R_UseImageAddFile(rd_add[i], NULL);
	for(i=0; i<rd_n_exp; i++)
		JX2R_UseImageAddFile(rd_exp[i], NULL);
	
	ctx=BJX2_AllocContext();
	BJX2_MemDefineROM(ctx,		"ROM",	0x00000000, 0x00007FFF);
	BJX2_MemDefineRAM(ctx,		"SRAM",	0x0000C000, 0x0000DFFF);
	BJX2_MemDefineMmgp(ctx,		"MMGP",	0x0000E000, 0x0000E3FF);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0x000A0000, 0x000AFFFF);
	BJX2_MemDefineRAM(ctx,		"DRAM",	0x01000000, 0x18000000);

	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xA000E000, 0xA000E3FF);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xA0080000, 0xA0081FFF);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xA00A0000, 0xA00AFFFF);

	BJX2_MemDefineMmgp(ctx,		"MMGP",	0xFFFFFFFFA000E000, 0xFFFFFFFFA000E3FF);
	BJX2_MemDefineSndSblk(ctx,	"SBAU",	0xFFFFFFFFA0080000, 0xFFFFFFFFA0081FFF);
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xFFFFFFFFA00A0000, 0xFFFFFFFFA00AFFFF);
	
	if(ifn)
	{
		BJX2_ContextLoadRom(ctx, ifn);
	}

	for(i=0; i<rd_n_map; i++)
	{
		BJX2_ContextLoadMap(ctx, rd_map[i]);
	}

	ctx->use_jit=0;
	if(mhz>100)
		ctx->use_jit=1;

//	ctx->ttick_hk=3052;
//	ctx->ttick_rst=3052;
	
	ctx->tgt_mhz=mhz;
//	ctx->tgt_mhz=100;
//	ctx->tgt_mhz=200;
//	ctx->tgt_mhz=300;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
//	ctx->ttick_rst=100000000/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	GfxDrv_Start();
	SoundDev_Init();
	JX2I_GfxCon_Startup();

//	t0=clock();
	t0=FRGL_TimeMS();

	ms0=FRGL_TimeMS();
	GfxDrv_MainLoop(btesh_main_iterate);

//	t1=clock();
	t1=FRGL_TimeMS();
	tt=t1-t0;
	
	fbtt=t1-jx2i_gfxcon_cbffms;
	
	tvus=BJX2_Interp_GetVirtualUsec(ctx);

	SoundDev_DeInit();

//	t0=clock();
//	t1=t0;
//	tt=0;
//	while(1)
//	{
//		
//		if(i)
//			break;
//		if(ctx->req_kill)
//			break;
//	}
//	if(i)

	BJX2_DbgDump(ctx);
	
	if(bjx2_vmoutlog)
		fclose(bjx2_vmoutlog);

	i=ctx->status;

#if 1
	if(1)
	{
		if(fbtt>0)
		{
			tsec=fbtt/1000.0;
			printf("%.2f fps\n", jx2i_gfxcon_cbfrnum/tsec);
		}


//		tsec=tt/((double)CLOCKS_PER_SEC);
		tsec=tt/1000.0;
		
//		BJX2_DbgDump(ctx);
		printf("%.2fs %.2fMIPS %.2fMHz usec=%3.2f%% timer_irq=%4.2fHz\n",
			tsec,
			ctx->tot_ops/(tsec*1000000.0),
			ctx->tot_cyc/(tsec*1000000.0),
			(tvus/10000.0)/tsec,
			ctx->nttick_irq/tsec);
		printf("Exit Status %04X\n", i);
	}
#endif
}
