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
fopen,		fclose,
fread,		fwrite,
fgetc,		fputc,
fgets,		fputs,
ungetc,		fseek,
ftell,		fflush,
feof,		freopen,
remove,		rename,
vfprintf,	vsnprintf,
vfscanf,	vsscanf,
clearerr,	perror,
setvbuf,	tmpnam,
ferror,

malloc,		free,
realloc,	_msize
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

#endif