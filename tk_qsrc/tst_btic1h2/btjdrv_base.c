/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include <bgbbtj.h>


// #if defined(BGBBTJ_DRV) || defined(BGBBTJ_NODY)
#if 1

#ifndef BGBBTJ_DRVLOGFILE
#ifdef _MSC_VER
#define BGBBTJ_DRVLOGFILE "F:\\bt1hdrv_log.txt"
#define BGBBTJ_DRVCFGFILE "F:\\bt1hdrv_conf.txt"

#define BGBBTJ_DRVLOGSFX "\\bt1hdrv_log.txt"
#define BGBBTJ_DRVCFGSFX "\\bt1hdrv_conf.txt"
#endif
#ifdef __linux__
#define BGBBTJ_DRVLOGFILE "~/.bt1hdrv_log.txt"
#define BGBBTJ_DRVCFGFILE "~/.bt1hdrv_conf.txt"
#endif
#endif

FILE *btjpg_log=NULL;
char *btjpg_home;
int btjpg_uselog=0;

extern u32 btjpg_drv_defaultCodecFcc;
extern int btjpg_drv_defaultCodecQfl;
extern int btjpg_drv_defaultCodecDbfl;

int BTJPG_DriverTryLoadConfig(char *name)
{
	char tb[256];
	FILE *fd;
	char **a;
	int i;

	fd=fopen(name, "rt");
	if(!fd)return(0);
	
	while(!feof(fd))
	{
		fgets(tb, 256, fd);
		
		a=bgbv86_rsplit(tb);
		if(!a[0])continue;
		if(a[0][0]=='#')continue;
		if(a[0][0]==';')continue;
		if(a[0][0]=='/')continue;
		
		if(!strcmp(a[0], "useLog"))
		{
			if(!strcmp(a[1], "true"))
				btjpg_uselog=1;
			continue;
		}

		if(!strcmp(a[0], "workerThreads"))
		{
			btic1h_workqueue_defaultworkers=atoi(a[1]);
			continue;
		}
		
		if(!strcmp(a[0], "defaultCodec"))
		{
			btjpg_drv_defaultCodecFcc=RIFF_MAKETAG(
				a[1][0], a[1][1], a[1][2], a[1][3]);
			continue;
		}

		if(!strcmp(a[0], "encoderFlags"))
		{
			for(i=1; a[i]; i++)
			{
				if(!strcmp(a[i], "rangecoder"))
					btjpg_drv_defaultCodecQfl|=BTIC1H_QFL_USERC;
				if(!strcmp(a[i], "rangecoder66"))
					btjpg_drv_defaultCodecQfl|=BTIC1H_QFL_USERC66;
				if(!strcmp(a[i], "slice"))
					btjpg_drv_defaultCodecQfl|=BTIC1H_QFL_USESLICE;
				if(!strcmp(a[i], "gdbdr"))
					btjpg_drv_defaultCodecQfl|=BTIC1H_QFL_USEGDBDR;
			}
		}

		if(!strcmp(a[0], "debugFlags"))
		{
			for(i=1; a[i]; i++)
			{
				if(!strcmp(a[i], "clearskip"))
					btjpg_drv_defaultCodecDbfl|=BTIC1H_DBFL_CLEARSKIP;
			}
		}
	}
	
	fclose(fd);
	
	return(1);
}

void BTJPG_DriverInit(void)
{
	static int init=0;
	char tb1[1024];
	char tb2[1024];

	if(init)return;
	init=1;

#ifdef _MSC_VER
	GetEnvironmentVariable("HOME", tb1, 512);

	strcpy(tb2, tb1);
	strcat(tb2, BGBBTJ_DRVCFGSFX);
	BTJPG_DriverTryLoadConfig(tb2);
	
	if(btjpg_uselog)
	{
		strcpy(tb2, tb1);
		strcat(tb2, BGBBTJ_DRVLOGSFX);
		btjpg_log=fopen(tb2, "at");
		if(btjpg_log)
			{ btjpg_puts("--------\n"); }
	}
#else

#ifdef BGBBTJ_DRVLOGFILE
	if(!btjpg_log)
		btjpg_log=fopen(BGBBTJ_DRVLOGFILE, "at");
	if(btjpg_log)
		{ btjpg_puts("--------\n"); }
#endif

#ifdef BGBBTJ_DRVCFGFILE
	BTJPG_DriverTryLoadConfig(BGBBTJ_DRVCFGFILE);
#endif

#endif
}

void BTJPG_DriverDeinit(void)
{
	if(btjpg_log)
	{
		fclose(btjpg_log);
		btjpg_log=NULL;
	}
}

void btjpg_puts(char *str)
{
	if(btjpg_log)
	{
		fputs(str, btjpg_log);
		fflush(btjpg_log);
	}
}

void btjpg_printf(char *str, ...)
{
	char buf[65536];
	va_list lst;

	if(!btjpg_log)
		return;
		
	va_start(lst, str);
//	vfprintf(btjpg_log, str, lst);
	vsprintf(buf, str, lst);
	va_end(lst);
	
	btjpg_puts(buf);
}

void *btjpg_malloc(size_t sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void *btjpg_gcalloc(size_t sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void *btjpg_gctalloc(char *str, size_t sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void btjpg_gcfree(void *ptr)
{
	free(ptr);
}

char *btjpg_strdup(char *str)
{
	char *s1;
	
	s1=strdup(str);
	return(s1);
}

#endif
