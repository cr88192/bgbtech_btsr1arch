#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <bgbrasw.h>

int main(int argc, char *argv[])
{
	BGBRASW_Context *ctx;
	BGBRASW_Primitive *prim, *plst, *pcur;
	BGBRASW_Texture *tex;
	double dt0;
	int t0, t1, t2, t3;
	int t4, t5, t6, t7;
	int i, j, k, l, n;
	
	ctx=BGBRASW_AllocContext();
//	BGBRASW_SetupRenderBasic(ctx, 1024, 768, 0);
	BGBRASW_SetupRenderBasic(ctx, 1440, 900, 0);

	BGBRASW_ClearScreenColor(ctx,
		BGBRASW_MAKEPIXEL(0,0,0,255));
	BGBRASW_ClearDepthStencil(ctx,
		0xFFFFFF00);

#if 0
	printf("Test Clear:\n");
	t0=clock(); n=0;
	while(1)
	{
		t1=clock();
		if(((t1-t0)<0) || ((t1-t0)>(10*CLOCKS_PER_SEC)))
			break;
		dt0=(t1-t0)/((double)CLOCKS_PER_SEC);
//		dt0=dt0+0.000001;
		BGBRASW_ClearScreenColor(ctx,
			BGBRASW_MAKEPIXEL(0,0,0,255));
		printf("%d, %.2fsec, %.2f/sec\r", n, dt0, n/dt0);
		n++;
	}
	printf("\n");
#endif

#if 1
	tex=BGBRASW_AllocTexture(ctx);
//	BGBRASW_SetTextureSize(ctx, tex, 256, 256);
	tex->rgba[0]=BGBRASW_Img_LoadTGAPixel("fixed_16px.tga",
		&(tex->xs), &(tex->ys));

	prim=BGBRASW_AllocPrimitive(ctx);
	prim->next=NULL;
	prim->tex=tex;

	prim->type=BGBRASW_PTYPE_BOX;
//	prim->mode=BGBRASW_PMODE_FLATBASIC;
	prim->mode=BGBRASW_PMODE_FLATINTERP;
//	prim->mode=BGBRASW_PMODE_TEXBASIC;
//	prim->mode=BGBRASW_PMODE_TEXINTERP;
//	prim->mode=BGBRASW_PMODE_TEXBASICZTEST;
//	prim->mode=BGBRASW_PMODE_TEXINTERPZTEST;
	
//	prim->xy[0][0]=   0; prim->xy[0][1]=0;
//	prim->xy[1][0]=1024; prim->xy[1][1]=0;
//	prim->xy[2][0]=   0; prim->xy[2][1]=768;
//	prim->xy[3][0]=1024; prim->xy[3][1]=768;

	prim->xy[0][0]=		0;		prim->xy[0][1]=0;
	prim->xy[1][0]=ctx->tgt_xs;	prim->xy[1][1]=0;
	prim->xy[2][0]=		0;		prim->xy[2][1]=ctx->tgt_ys;
	prim->xy[3][0]=ctx->tgt_xs;	prim->xy[3][1]=ctx->tgt_ys;

	prim->st[0][0]=0;			prim->st[0][1]=0;
	prim->st[1][0]=tex->xs;		prim->st[1][1]=0;
	prim->st[2][0]=0;			prim->st[2][1]=tex->ys;
	prim->st[3][0]=tex->xs;		prim->st[3][1]=tex->ys;

	prim->rgba[0]=BGBRASW_MAKEPIXEL(255,0,0,255);
	prim->rgba[1]=BGBRASW_MAKEPIXEL(0,255,0,255);
	prim->rgba[2]=BGBRASW_MAKEPIXEL(0,0,255,255);
	prim->rgba[3]=BGBRASW_MAKEPIXEL(0,0,0,255);
	
	printf("Test DrawPrim:\n");
	t0=clock(); n=0;
	while(1)
	{
		t1=clock();
		if(((t1-t0)<0) || ((t1-t0)>(10*CLOCKS_PER_SEC)))
			break;
		dt0=(t1-t0)/((double)CLOCKS_PER_SEC);
//		dt0=dt0+0.000001;
//		BGBRASW_ClearScreenColor(ctx,
//			BGBRASW_MAKEPIXEL(0,0,0,255));
		BGBRASW_DrawPrimitiveList(ctx, prim);
		printf("%d, %.2fsec, %.2f/sec\r", n, dt0, n/dt0);
		n++;
	}
	printf("\n");
#endif

#if 0
	tex=BGBRASW_AllocTexture(ctx);
//	BGBRASW_SetTextureSize(ctx, tex, 256, 256);
//	BGBRASW_SetTextureAllocMip(ctx, tex);

	tex->rgba[0]=BGBRASW_Img_LoadTGAPixel("fixed_16px.tga",
		&(tex->xs), &(tex->ys));
	BGBRASW_SetTextureBuildMip(ctx, tex);

	plst=NULL;
	for(i=0; i<(ctx->tgt_ys/8); i++)
		for(j=0; j<(ctx->tgt_xs/8); j++)
	{
		prim=BGBRASW_AllocPrimitive(ctx);
		prim->next=plst;
		plst=prim;

		prim->tex=tex;

		prim->type=BGBRASW_PTYPE_BOX;
//		prim->mode=BGBRASW_PMODE_FLATBASIC;
//		prim->mode=BGBRASW_PMODE_FLATINTERP;
		prim->mode=BGBRASW_PMODE_TEXBASIC;
//		prim->mode=BGBRASW_PMODE_TEXINTERP;
//		prim->mode=BGBRASW_PMODE_TEXBASICZTEST;
//		prim->mode=BGBRASW_PMODE_TEXINTERPZTEST;
	
		prim->xy[0][0]=j*8;	prim->xy[0][1]=i*8;
		prim->xy[1][0]=j*8+8;	prim->xy[1][1]=i*8;
		prim->xy[2][0]=j*8;	prim->xy[2][1]=i*8+8;
		prim->xy[3][0]=j*8+8;	prim->xy[3][1]=i*8+8;

		prim->rgba[0]=BGBRASW_MAKEPIXEL(255,0,0,255);
		prim->rgba[1]=BGBRASW_MAKEPIXEL(0,255,0,255);
		prim->rgba[2]=BGBRASW_MAKEPIXEL(0,0,255,255);
		prim->rgba[3]=BGBRASW_MAKEPIXEL(0,0,0,255);

		k=rand();
		t0=k&15; t1=(k>>4)&15;

//		prim->st[0][0]=t0*8;		prim->st[0][1]=t1*8;
//		prim->st[1][0]=t0*8+8;		prim->st[1][1]=t1*8;
//		prim->st[2][0]=t0*8;		prim->st[2][1]=t1*8+8;
//		prim->st[3][0]=t0*8+8;		prim->st[3][1]=t1*8+8;

		prim->st[0][0]=t0*16;		prim->st[0][1]=t1*16;
		prim->st[1][0]=t0*16+16;	prim->st[1][1]=t1*16;
		prim->st[2][0]=t0*16;		prim->st[2][1]=t1*16+16;
		prim->st[3][0]=t0*16+16;	prim->st[3][1]=t1*16+16;
	}

	printf("Test DrawConsole:\n");
	t0=clock(); n=0; prim=plst;
	while(1)
	{
		t1=clock();
		if(((t1-t0)<0) || ((t1-t0)>(10*CLOCKS_PER_SEC)))
			break;
		dt0=(t1-t0)/((double)CLOCKS_PER_SEC);
//		dt0=dt0+0.000001;
//		BGBRASW_ClearScreenColor(ctx,
//			BGBRASW_MAKEPIXEL(0,0,0,255));
		BGBRASW_DrawPrimitiveList(ctx, prim);
		printf("%d, %.2fsec, %.2f/sec\r", n, dt0, n/dt0);
		n++;
	}
	printf("\n");
#endif

#if 0
	tex=BGBRASW_AllocTexture(ctx);
//	BGBRASW_SetTextureSize(ctx, tex, 256, 256);
//	BGBRASW_SetTextureAllocMip(ctx, tex);

	tex->rgba[0]=BGBRASW_Img_LoadTGAPixel("fixed_16px.tga",
		&(tex->xs), &(tex->ys));

	plst=NULL;
//	for(i=0; i<(ctx->tgt_ys/8); i++)
//		for(j=0; j<(ctx->tgt_xs/8); j++)
	for(i=0; i<4096; i++)
	{
		prim=BGBRASW_AllocPrimitive(ctx);
		prim->next=plst;
		plst=prim;

		prim->tex=tex;

		prim->type=BGBRASW_PTYPE_BOX;
//		prim->mode=BGBRASW_PMODE_FLATBASIC;
//		prim->mode=BGBRASW_PMODE_FLATINTERP;
		prim->mode=BGBRASW_PMODE_TEXBASIC;
//		prim->mode=BGBRASW_PMODE_TEXINTERP;
//		prim->mode=BGBRASW_PMODE_TEXBASICZTEST;
//		prim->mode=BGBRASW_PMODE_TEXINTERPZTEST;
	
		t0=rand()%ctx->tgt_xs;		t1=rand()%ctx->tgt_xs;
		t2=rand()%ctx->tgt_ys;		t3=rand()%ctx->tgt_ys;
		if(t1==t0)t1=rand()%ctx->tgt_xs;
		if(t3==t2)t3=rand()%ctx->tgt_ys;
		if(t1<t0)	{ k=t0; t0=t1; t1=k; }
		if(t3<t2)	{ k=t2; t2=t3; t3=k; }
		if(t1>(t0+64))t1=t0+64;
		if(t3>(t2+64))t3=t2+64;
	
		prim->xy[0][0]=t0;	prim->xy[0][1]=t2;
		prim->xy[1][0]=t1;	prim->xy[1][1]=t2;
		prim->xy[2][0]=t0;	prim->xy[2][1]=t3;
		prim->xy[3][0]=t1;	prim->xy[3][1]=t3;

		prim->rgba[0]=BGBRASW_MAKEPIXEL(255,0,0,255);
		prim->rgba[1]=BGBRASW_MAKEPIXEL(0,255,0,255);
		prim->rgba[2]=BGBRASW_MAKEPIXEL(0,0,255,255);
		prim->rgba[3]=BGBRASW_MAKEPIXEL(0,0,0,255);

		k=rand();
		t0=k&15; t1=(k>>4)&15;

//		prim->st[0][0]=t0*8;		prim->st[0][1]=t1*8;
//		prim->st[1][0]=t0*8+8;		prim->st[1][1]=t1*8;
//		prim->st[2][0]=t0*8;		prim->st[2][1]=t1*8+8;
//		prim->st[3][0]=t0*8+8;		prim->st[3][1]=t1*8+8;

		prim->st[0][0]=t0*16;		prim->st[0][1]=t1*16;
		prim->st[1][0]=t0*16+16;	prim->st[1][1]=t1*16;
		prim->st[2][0]=t0*16;		prim->st[2][1]=t1*16+16;
		prim->st[3][0]=t0*16+16;	prim->st[3][1]=t1*16+16;
	}

	printf("Test DrawRandBoxes:\n");
	t0=clock(); n=0; prim=plst;
	while(1)
	{
		t1=clock();
		if(((t1-t0)<0) || ((t1-t0)>(10*CLOCKS_PER_SEC)))
			break;
		dt0=(t1-t0)/((double)CLOCKS_PER_SEC);
//		dt0=dt0+0.000001;
//		BGBRASW_ClearScreenColor(ctx,
//			BGBRASW_MAKEPIXEL(0,0,0,255));
		BGBRASW_DrawPrimitiveList(ctx, prim);
		printf("%d, %.2fsec, %.2f/sec\r", n, dt0, n/dt0);
		n++;
	}
	printf("\n");
#endif

#if 0
	tex=BGBRASW_AllocTexture(ctx);
//	BGBRASW_SetTextureSize(ctx, tex, 256, 256);
//	BGBRASW_SetTextureAllocMip(ctx, tex);

	tex->rgba[0]=BGBRASW_Img_LoadTGAPixel("fixed_16px.tga",
		&(tex->xs), &(tex->ys));
	BGBRASW_SetTextureBuildMip(ctx, tex);

	plst=NULL;
	for(i=0; i<(ctx->tgt_ys/8); i++)
		for(j=0; j<(ctx->tgt_xs/8); j++)
	{
		prim=BGBRASW_AllocPrimitive(ctx);
		prim->next=plst;
		plst=prim;

		prim->tex=tex;

//		prim->type=BGBRASW_PTYPE_BOX;
		prim->type=BGBRASW_PTYPE_TRIANGLE;
//		prim->mode=BGBRASW_PMODE_FLATBASIC;
//		prim->mode=BGBRASW_PMODE_FLATINTERP;
		prim->mode=BGBRASW_PMODE_TEXBASIC;
//		prim->mode=BGBRASW_PMODE_TEXINTERP;
//		prim->mode=BGBRASW_PMODE_TEXBASICZTEST;
//		prim->mode=BGBRASW_PMODE_TEXINTERPZTEST;
	
		prim->xy[0][0]=j*8;	prim->xy[0][1]=i*8;
		prim->xy[1][0]=j*8+8;	prim->xy[1][1]=i*8;
//		prim->xy[2][0]=j*8+8;	prim->xy[2][1]=i*8+8;
		prim->xy[2][0]=j*8;	prim->xy[2][1]=i*8+8;
//		prim->xy[3][0]=j*8+8;	prim->xy[3][1]=i*8+8;

		prim->rgba[0]=BGBRASW_MAKEPIXEL(255,0,0,255);
		prim->rgba[1]=BGBRASW_MAKEPIXEL(0,255,0,255);
		prim->rgba[2]=BGBRASW_MAKEPIXEL(0,0,255,255);
		prim->rgba[3]=BGBRASW_MAKEPIXEL(0,0,0,255);

		k=rand();
		t0=k&15; t1=(k>>4)&15;

//		prim->st[0][0]=t0*8;		prim->st[0][1]=t1*8;
//		prim->st[1][0]=t0*8+8;		prim->st[1][1]=t1*8;
//		prim->st[2][0]=t0*8;		prim->st[2][1]=t1*8+8;
//		prim->st[3][0]=t0*8+8;		prim->st[3][1]=t1*8+8;

		prim->st[0][0]=t0*16;		prim->st[0][1]=t1*16;
		prim->st[1][0]=t0*16+16;	prim->st[1][1]=t1*16;
		prim->st[2][0]=t0*16;		prim->st[2][1]=t1*16+16;
//		prim->st[3][0]=t0*16+16;	prim->st[3][1]=t1*16+16;
	}

	printf("Test DrawConTris:\n");
	t0=clock(); n=0; prim=plst;
	while(1)
	{
		t1=clock();
		if(((t1-t0)<0) || ((t1-t0)>(10*CLOCKS_PER_SEC)))
			break;
		dt0=(t1-t0)/((double)CLOCKS_PER_SEC);
//		dt0=dt0+0.000001;
//		BGBRASW_ClearScreenColor(ctx,
//			BGBRASW_MAKEPIXEL(0,0,0,255));
		BGBRASW_DrawPrimitiveList(ctx, prim);
		printf("%d, %.2fsec, %.2f/sec\r", n, dt0, n/dt0);
		n++;
	}
	printf("\n");
#endif

#if 0
	tex=BGBRASW_AllocTexture(ctx);
//	BGBRASW_SetTextureSize(ctx, tex, 256, 256);
//	BGBRASW_SetTextureAllocMip(ctx, tex);

	tex->rgba[0]=BGBRASW_Img_LoadTGAPixel("fixed_16px.tga",
		&(tex->xs), &(tex->ys));

	plst=NULL;
//	for(i=0; i<(ctx->tgt_ys/8); i++)
//		for(j=0; j<(ctx->tgt_xs/8); j++)
	for(i=0; i<4096; i++)
	{
		prim=BGBRASW_AllocPrimitive(ctx);
		prim->next=plst;
		plst=prim;

		prim->tex=tex;

		prim->type=BGBRASW_PTYPE_TRIANGLE;
//		prim->mode=BGBRASW_PMODE_FLATBASIC;
		prim->mode=BGBRASW_PMODE_FLATINTERP;
//		prim->mode=BGBRASW_PMODE_TEXBASIC;
//		prim->mode=BGBRASW_PMODE_TEXINTERP;
//		prim->mode=BGBRASW_PMODE_TEXBASICZTEST;
//		prim->mode=BGBRASW_PMODE_TEXINTERPZTEST;

		t6=rand()%(ctx->tgt_xs-64)+32;
		t7=rand()%(ctx->tgt_ys-64)+32;
	
		t0=t6+(rand()&63)-32;
		t1=t7+(rand()&63)-32;
		t2=t6+(rand()&63)-32;
		t3=t7+(rand()&63)-32;
		t4=t6+(rand()&63)-32;
		t5=t7+(rand()&63)-32;

//		t0=rand()%ctx->tgt_xs;		t1=rand()%ctx->tgt_xs;
//		t2=rand()%ctx->tgt_ys;		t3=rand()%ctx->tgt_ys;
//		if(t1==t0)t1=rand()%ctx->tgt_xs;
//		if(t3==t2)t3=rand()%ctx->tgt_ys;
//		if(t1<t0)	{ k=t0; t0=t1; t1=k; }
//		if(t3<t2)	{ k=t2; t2=t3; t3=k; }
//		if(t1>(t0+64))t1=t0+64;
//		if(t3>(t2+64))t3=t2+64;
	
		prim->xy[0][0]=t0;	prim->xy[0][1]=t1;
		prim->xy[1][0]=t2;	prim->xy[1][1]=t3;
		prim->xy[2][0]=t4;	prim->xy[2][1]=t5;
//		prim->xy[3][0]=t1;	prim->xy[3][1]=t3;

		prim->rgba[0]=BGBRASW_MAKEPIXEL(255,0,0,255);
		prim->rgba[1]=BGBRASW_MAKEPIXEL(0,255,0,255);
		prim->rgba[2]=BGBRASW_MAKEPIXEL(0,0,255,255);
		prim->rgba[3]=BGBRASW_MAKEPIXEL(0,0,0,255);

		k=rand();
		t0=k&15; t1=(k>>4)&15;

//		prim->st[0][0]=t0*8;		prim->st[0][1]=t1*8;
//		prim->st[1][0]=t0*8+8;		prim->st[1][1]=t1*8;
//		prim->st[2][0]=t0*8;		prim->st[2][1]=t1*8+8;
//		prim->st[3][0]=t0*8+8;		prim->st[3][1]=t1*8+8;

		prim->st[0][0]=t0*16;		prim->st[0][1]=t1*16;
		prim->st[1][0]=t0*16+16;	prim->st[1][1]=t1*16;
		prim->st[2][0]=t0*16;		prim->st[2][1]=t1*16+16;
		prim->st[3][0]=t0*16+16;	prim->st[3][1]=t1*16+16;
	}

	printf("Test DrawRandTriangles:\n");
	t0=clock(); n=0; prim=plst;
	while(1)
	{
		t1=clock();
		if(((t1-t0)<0) || ((t1-t0)>(10*CLOCKS_PER_SEC)))
			break;
		dt0=(t1-t0)/((double)CLOCKS_PER_SEC);
//		dt0=dt0+0.000001;
//		BGBRASW_ClearScreenColor(ctx,
//			BGBRASW_MAKEPIXEL(0,0,0,255));
		BGBRASW_DrawPrimitiveList(ctx, prim);
		printf("%d, %.2fsec, %.2f/sec\r", n, dt0, n/dt0);
		n++;
	}
	printf("\n");
#endif

	BGBRASW_Img_SaveTGAPixel("bgbraswtst0.tga",
		ctx->tgt_rgba, ctx->tgt_xs, ctx->tgt_ys);
}
