#include "bsr1_multi.c"

int BTSR1_MainPollKeyboard(BTSR1_Context *ctx)
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
}

BTSR1_Context *ctx;

void btesh_main_iterate()
{
	int i;

	BTSR1_GfxCon_Update();

	GfxDrv_BeginDrawing();
	BTSR1_MainPollKeyboard(ctx);
	i=BTSR1_RunLimit(ctx, 999999);
	
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
	char *ifn;
	double tsec;
	int t0, t1, tt;
	int i;
	
	ifn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		
		if(!ifn)
			ifn=argv[i];
	}
	
	ctx=BTSR1_AllocContext();
	BTSR1_MemDefineROM(ctx, "ROM", 0x0000, 0x7FFF);
	BTSR1_MemDefineRAM(ctx, "RAM", 0xC000, 0xDFFF);
	BTSR1_MemDefineMmgp(ctx, "MMGP", 0xE000, 0xE3FF);

	BTSR1_MemDefineGfxCon(ctx, "CGFX", 0xA0000, 0xAFFFF);
	
	if(ifn)
	{
		BTSR1_ContextLoadRom(ctx, ifn);
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
	BTSR1_GfxCon_Startup();

	GfxDrv_MainLoop(btesh_main_iterate);

	BTSR1_DbgDump(ctx);


#if 0
	t0=clock();
	t1=t0;
	tt=0;
	while(1)
	{
		BTSR1_MainPollKeyboard(ctx);
		i=BTSR1_RunLimit(ctx, 999999);
		
		t1=clock();
		if(t1>t0)
			tt+=t1-t0;
		t0=t1;
//		ctx->ttick_32k-=999;
//		if(ctx->ttick_32k<=0)
//		{
//			BTSR1_ThrowFaultStatus(ctx, BTSR1_FLT_TIMER32K);
//			ctx->ttick_32k+=3052;
//		}
		
		if(i)
			break;
		if(ctx->req_kill)
			break;
	}
//	if(i)
	if(1)
	{
		tsec=tt/((double)CLOCKS_PER_SEC);
	
		BTSR1_DbgDump(ctx);
		printf("%.2fs %.2fMIPS %.2fMHz\n",
			tsec,
			ctx->tot_ops/(tsec*1000000.0),
			ctx->tot_cyc/(tsec*1000000.0));
		printf("Exit Status %04X\n", i);
	}
#endif

}
