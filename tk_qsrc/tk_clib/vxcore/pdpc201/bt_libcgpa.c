/*
DLL Stub Library, GetProcAddress interface.
 */

#include <tk_core.h>
#include <tk_libcgpa.h>

#include <stdint.h>

#ifdef __TK_CLIB_DLLSTUB__

// extern int __arch_exsr;
// extern void *__arch_tbr;

// intptr_t _arch_gettbr(void);

void *TkClGetProcAddress(char *name)
{
	void *(*GpaFn)(char *name);
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *tusr;
	
//	task=(TKPE_TaskInfo *)__arch_tbr;
//	task=(TKPE_TaskInfo *)_arch_gettbr();
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	tusr=(TKPE_TaskInfoUser *)task->usrptr;
	
	GpaFn=(void *)(tusr->clib_gpa);
	if(GpaFn)
		return(GpaFn(name));
	return(NULL);
}

void *TkClGetProcAddressCn(void **rfptr, char *name)
{
	void *p;
	p=*rfptr;
	if(p)return(p);
	p=TkClGetProcAddress(name);
	*rfptr=p;
	return(p);
}

void **TK_GetAllocaMark()
{
	TKPE_TaskInfo *task;
//	task=__arch_tbr;
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	return((void **)(task->allocaptr));
}

void __exit(int status)
{
}

u32 TK_GetTimeMs(void)
{
	return(0);
}

char *TK_Env_GetCwd(char *buf, int sz)
{
}

#endif


#ifndef __TK_CLIB_DLLSTUB__

struct tk_stdio_vt tk_stdio_vti =
{
NULL,			NULL,				NULL,			NULL,			//00
fopen,			fclose,				fread,			fwrite,			//04
fgetc,			fputc,				fgets,			fputs,			//08
ungetc,			fseek,				ftell,			fflush,			//0C
feof,			freopen,			remove,			__rename2,		//10
vfprintf,		vsnprintf,			vfscanf,		vsscanf,		//14
clearerr,		perror,				setvbuf,		tmpnam,			//18
ferror,			_TK_STDIO_KEY1A,	_mgetbase,		_mfreezone,		//1C
_malloc_cat,	free,				realloc,		_msize,			//20
_mgettag,		_msettag,			_mgetzone,		_msetzone,		//24
opendir,		closedir,			readdir,		__get_errno,	//28
mmap,			munmap,				msync,			mprotect,		//2C

setlocale,		localeconv,			signal,			raise,			//30
dlopen,			dlclose,			dlsym,			dlerror,		//34
__open,			__close,			__read,			__write,		//38
__seek,			__ioctl,			__sendto,		__recvfrom,		//3C
__bind,			__accept,			__connect,		__multicall,	//40
__exita,		TK_GetTimeUs,

NULL
};

struct tkclgpa_symbol_s {
char *name;
void *ptr;
}tkclgpa_symbols[]={
{"stdio_vt", &tk_stdio_vti},
{NULL, NULL}
};

void *TkClGetProcAddressSv(char *name)
{
	struct tkclgpa_symbol_s *sym;
	char *s;
	int i;
	
	for(i=0; ; i++)
	{
		sym=tkclgpa_symbols+i;
		s=sym->name;
		if(!s)break;
		if(!strcmp(s, name))
			return(sym->ptr);
	}
	return(NULL);
}

void __init_getprocstub()
{
	void *(*GpaFn)(char *name);
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *tusr;
	
	task=(TKPE_TaskInfo *)TK_GET_TBR;
	tusr=(TKPE_TaskInfoUser *)task->usrptr;
	
	GpaFn=(void *)(tusr->clib_gpa);
	if(GpaFn)
		return;
	GpaFn=TkClGetProcAddressSv;
	tusr->clib_gpa=GpaFn;
}

#endif