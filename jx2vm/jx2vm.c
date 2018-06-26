#include "jx2i_multi.c"

int BJX2_MainPollKeyboard(BJX2_Context *ctx)
{
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

	return(0);
}

BJX2_Context *ctx;

void btesh_main_iterate()
{
	int i;

	JX2I_GfxCon_Update();

	GfxDrv_BeginDrawing();
	BJX2_MainPollKeyboard(ctx);
	i=BJX2_RunLimit(ctx, 999999);
	
	if(i || ctx->req_kill)
		gfxdrv_kill=1;
	
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
	int rd_n_add;
	int rd_n_exp;

	char *ifn;
	double tsec;
	int t0, t1, tt;
	int ifmd, rdsz;
	int i;
	
	rd_n_add=0;
	rd_n_exp=0;
	ifn=NULL;
	ifmd=0; rdsz=128;
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

			continue;
		}
		
		if(!ifn)
			{ ifn=argv[i]; continue; }

		if(ifmd==1)
			{ rd_add[rd_n_add++]=argv[i]; continue; }
		if(ifmd==2)
			{ rd_exp[rd_n_exp++]=argv[i]; continue; }
	}
	
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
	BJX2_MemDefineGfxCon(ctx,	"CGFX",	0xA00A0000, 0xA00AFFFF);
	
	if(ifn)
	{
		BJX2_ContextLoadRom(ctx, ifn);
	}
	
//	ctx->ttick_hk=3052;
//	ctx->ttick_rst=3052;
	
	ctx->tgt_mhz=100;
	ctx->rcp_mhz=((1048576/ctx->tgt_mhz)+7)>>4;
	ctx->ttick_rst=(ctx->tgt_mhz*1000000)/1024;
//	ctx->ttick_rst=100000000/1024;
	ctx->ttick_hk=ctx->ttick_rst;

	GfxDrv_Start();
//	SoundDev_Init();
	JX2I_GfxCon_Startup();

	GfxDrv_MainLoop(btesh_main_iterate);

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

#if 0
	if(1)
	{
		tsec=tt/((double)CLOCKS_PER_SEC);
	
		BJX2_DbgDump(ctx);
		printf("%.2fs %.2fMIPS %.2fMHz\n",
			tsec,
			ctx->tot_ops/(tsec*1000000.0),
			ctx->tot_cyc/(tsec*1000000.0));
		printf("Exit Status %04X\n", i);
	}
#endif
}
