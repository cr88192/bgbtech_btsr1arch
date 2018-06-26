#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef unsigned long long u64;
typedef signed long long s64;

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifdef __GNUC__
#define force_inline inline
#define default_inline inline
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

void *bgbrpi_talloc(char *str, int sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void *bgbrpi_malloc(int sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

#include "bgbrpi_dummyavi.h"
#include "w32_dummyavi.c"

int main(int argc, char *argv[])
{
	BGBBTJ_AVI_Context *ctx;
	char *ifn;
	double dt, fsz;
	int fc, xs, ys;
	int t0, t1, t2;
	int i, j, k;

	ifn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		ifn=argv[i];
	}

	if(!ifn)
	{
		printf("No Input\n");
		return(-1);
	}
	
	bipro_thread_init();

	ctx=BGBBTJ_AVI_LoadAVI(ifn);
	if(!ctx)
	{
		printf("Failed load AVI %s\n", ifn);
		return(-1);
	}
	
	xs=ctx->bmihead->biWidth;
	ys=ctx->bmihead->biHeight;
	fsz=xs*ys/1000000.0;
	printf("%d x %d, fsz=%f Mpix\n", xs, ys, fsz);
	
	fc=0; t0=clock(); t1=t0;
	while(t1<(t0+(30*CLOCKS_PER_SEC)))
	{
		BGBBTJ_AVI_DecodeFrameBGRA(ctx);
		fc++;
		
		t1=clock();
		t2=t1-t0;
		dt=t2/((double)CLOCKS_PER_SEC);
		if(dt==0)dt=0.01;
		
		printf("%.2fs %.2f fps, %.2f Mpix/sec  \r",
			dt, fc/dt, fsz*(fc/dt));
	}
	printf("\n");
}
