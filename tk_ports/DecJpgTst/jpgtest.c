#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#ifdef __BJX2__
#include <tkgdi/tkgdi.h>
#endif

typedef signed char			sbyte;
typedef unsigned char			byte;
typedef signed short			s16;
typedef unsigned short		u16;
typedef signed int			s32;
typedef unsigned int			u32;
typedef signed long long		s64;
typedef unsigned long long	u64;

typedef unsigned int			uint;

#include "bcn_decjpg.c"


u32 jpegarr[] = {
#include "gen_jpg2c.c"
};

#ifdef __BJX2__
int GetTimeMs()
{
	return(TK_GetTimeMs());
}
#else
int GetTimeMs()
{
	return(clock());
}
#endif

#ifdef __BJX2__
TKGHDC hDc;
u16 *pImgBuf;

void TestTkGdi()
{
	TKGDI_BITMAPINFOHEADER t_info;
	TKGDI_BITMAPINFOHEADER *info;
	int x, y, cr, cg, cb, cp;

	tk_printf("TestTkGdi:\n");

	info=&t_info;
	memset(info, 0, sizeof(TKGDI_BITMAPINFOHEADER));

	info->biWidth=320;
	info->biHeight=200;

	info->biWidth=640;
	info->biHeight=400;

	info->biBitCount=16;

	tk_printf("  1\n", hDc);

	hDc=1;
	hDc=tkgCreateDisplay(info);

	tk_printf("  hDc = %X\n", hDc);
	if(hDc<=0)
		return;

	info->biWidth=320;
	info->biHeight=200;
	pImgBuf=malloc(322*202*2);
	
	for(y=0; y<200; y++)
		for(x=0; x<320; x++)
	{
		cr=(x*204)>>8;
		cb=(y*327)>>8;
		cg=255-((cr+cb)>>1);
		
		cp=((cr>>3)<<10)|((cg>>3)<<5)|(cb>>3);
		pImgBuf[y*320+x]=cp;
	}
	
	tkgBlitImage(hDc, 0, 0, info, pImgBuf);
}
#endif

int main()
{
	PDJPG_Context *ctx;
	int xs, ys;
	int t0, t1, t0e, cnt, secs;
	s64 totpix;

#ifdef __BJX2__
	TestTkGdi();
#endif

//	return(0);

	ctx=PDJPG_AllocContext();

	printf("Test Dec 0:\n");
	PDJPG_DecodeBasic(ctx, jpegarr, JPG2STR_SIZE, &xs, &ys);
	printf("%d x %d\n", xs, ys);
	
	secs=30;
//	t0=clock();
	t0=GetTimeMs();
	t0e=t0+(secs*CLOCKS_PER_SEC);
	
	t1=t0; cnt=0;
	while(t1<t0e)
	{
		PDJPG_DecodeBasic(ctx, jpegarr, JPG2STR_SIZE, &xs, &ys);
		cnt++;
//		t1=clock();
		t1=GetTimeMs();
	}
	
	totpix=((s64)xs)*((s64)ys)*((s64)cnt);
	
	printf("Count=%d %lld pix/sec\n", cnt, totpix/secs);
}
