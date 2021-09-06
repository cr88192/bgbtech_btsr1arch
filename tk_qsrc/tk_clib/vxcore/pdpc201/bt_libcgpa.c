/*
DLL Stub Library, GetProcAddress interface.
 */

#include <tk_core.h>
#include <tk_libcgpa.h>

#ifdef __TK_CLIB_DLLSTUB__

extern int __arch_exsr;
extern void *__arch_tbr;

void *TkClGetProcAddress(char *name)
{
	void *(*GpaFn)(char *name);
	TKPE_TaskInfo *task;
	TKPE_TaskInfoUser *tusr;
	
	task=(TKPE_TaskInfo *)__arch_tbr;
	tusr=(TKPE_TaskInfoUser *)task->usrptr;
	
	GpaFn=tusr->clib_gpa;
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
	task=__arch_tbr;
	return((void **)(task->allocaptr));
}

void __exit()
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