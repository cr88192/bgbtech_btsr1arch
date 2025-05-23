/*********************************************************************/
/*																   */
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																   */
/*********************************************************************/
/*********************************************************************/
/*																   */
/*  start.c - startup/termination code							   */
/*																   */
/*********************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"

#if USE_MEMMGR
#include "__memmgr.h"
#endif

extern FILE *__userFiles[__NFILE];

#define MAXPARMS 50 /* maximum number of arguments we can handle */

#ifdef __OS2__
#define INCL_DOS
#include <os2.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#if (defined(__MSDOS__) && defined(__WATCOMC__))
#define CTYP __cdecl
#else
#define CTYP
#endif

#ifdef __MSDOS__
/* Must be unsigned as it is used for array index */
extern unsigned char *__envptr;
extern unsigned short __osver;
#endif

#ifdef __VSE__
#undef __CMS__
#endif

#ifdef __MVS__
int __tso = 0; /* is this a TSO environment? */
#endif

#ifdef __MAIN_FP__
#ifdef __VX86__
// int (*__main_fp)(int argc, char **argv, char **env);
#else
// int (*__main_fp)(int argc, char **argv);
#endif

// __PDPCLIB_API__ void **__get_main_fp()
//	{ return((void **)(&__main_fp)); }

#else
int main(int argc, char **argv);
#endif

void __exit(int status);
void CTYP __exita(int status);

char *__get_cmdline();
char **__get_cmdenv();

#if !defined(__MVS__) && !defined(__CMS__) && !defined(__VSE__)
static char buffer1[BUFSIZ + 8];
static char buffer2[BUFSIZ + 8];
static char buffer3[BUFSIZ + 8];
#endif

#if USE_MEMMGR
extern void *__lastsup; /* last thing supplied to memmgr */
#endif

char **__eplist;
char *__plist;

int __start_early();
int __start_late();
int __start_first();

void _ctype_init(void);
void __init_getprocstub();
void tk_putc(int val);

short *_ctype_tolow;
short *_ctype_toup;

#ifdef __riscv
extern char __bss_start;
// extern char __bss_end;
extern char __BSS_END__;
#endif

__PDPCLIB_API__ int CTYP __start()
{
	FILE *t_stdin, *t_stdout, *t_stderr;
	int x;
	int argc;
	static char *argv[MAXPARMS + 1];
	int rc;
	char *p;
	char **penv;

#ifdef __riscv
//	memset(&__bss_start, 0, &__BSS_END__-&__bss_start);
#endif

	__start_first();

//	__debugbreak();

#ifdef __riscv
	tk_putc('T');
	tk_putc('e');
	tk_putc('s');
	tk_putc('t');
	tk_putc('\n');
	
//	__debugbreak();

	tk_puts("TK: Start A0\n");
#endif

	__start_early();

	p=__get_cmdline();
	penv=__get_cmdenv();

//	if(p && !(((long)p)>>14))
//	{
//		__debugbreak();
//	}

//	tk_puts("A1\n");

	_ctype_tolow=NULL;
	_ctype_toup=NULL;

	__init_stdin();
	_ctype_init();
	__init_getprocstub();

	t_stdin=stdin;
	t_stdout=stdout;
	t_stderr=stderr;

	t_stdin->hfile = 0;
	t_stdout->hfile = 1;
	t_stderr->hfile = 2;

	t_stdin->quickBin = 0;
	t_stdin->quickText = 0;
	t_stdin->textMode = 1;
	t_stdin->intFno = 0;
	t_stdin->bufStartR = 0;
	t_stdin->bufTech = _IOLBF;
	t_stdin->intBuffer = buffer1;
	*(u16 *)t_stdin->intBuffer=0x1234;
	*(u16 *)(t_stdin->intBuffer+(BUFSIZ + 6))=0x4321;
	t_stdin->fbuf = t_stdin->intBuffer + 2;
	*t_stdin->fbuf++ = '\0';
	*t_stdin->fbuf++ = '\0';
	t_stdin->szfbuf = BUFSIZ;
	t_stdin->endbuf = t_stdin->fbuf + t_stdin->szfbuf;
	*t_stdin->endbuf = '\n';
	t_stdin->noNl = 0;
	t_stdin->upto = t_stdin->endbuf;
	t_stdin->bufStartR = -t_stdin->szfbuf;
	t_stdin->mode = __READ_MODE;
	t_stdin->ungetCh = -1;
	t_stdin->update = 0;
	t_stdin->theirBuffer = 0;
	t_stdin->permfile = 1;
	t_stdin->isopen = 1;

	t_stdout->quickBin = 0;
	t_stdout->quickText = 0;
	t_stdout->textMode = 1;
	t_stdout->bufTech = _IOLBF;
	t_stdout->intBuffer = buffer2;
	*(u16 *)t_stdout->intBuffer=0x1234;
	*(u16 *)(t_stdout->intBuffer+(BUFSIZ + 6))=0x4321;
	t_stdout->fbuf = t_stdout->intBuffer + 2;
	*t_stdout->fbuf++ = '\0';
	*t_stdout->fbuf++ = '\0';
	t_stdout->szfbuf = BUFSIZ;
	t_stdout->endbuf = t_stdout->fbuf + t_stdout->szfbuf;
	*t_stdout->endbuf = '\n';
	t_stdout->noNl = 0;
	t_stdout->upto = t_stdout->fbuf;
	t_stdout->bufStartR = 0;
	t_stdout->mode = __WRITE_MODE;
	t_stdout->update = 0;
	t_stdout->theirBuffer = 0;
	t_stdout->permfile = 1;
	t_stdout->isopen = 1;

	t_stderr->quickBin = 0;
	t_stderr->quickText = 0;
	t_stderr->textMode = 1;
	t_stderr->bufTech = _IOLBF;
	t_stderr->intBuffer = buffer3;
	*(u16 *)t_stderr->intBuffer=0x1234;
	*(u16 *)(t_stderr->intBuffer+(BUFSIZ + 6))=0x4321;
	t_stderr->fbuf = t_stderr->intBuffer + 2;
	*t_stderr->fbuf++ = '\0';
	*t_stderr->fbuf++ = '\0';
	t_stderr->szfbuf = BUFSIZ;
	t_stderr->endbuf = t_stderr->fbuf + t_stderr->szfbuf;
	*t_stderr->endbuf = '\n';
	t_stderr->noNl = 0;
	t_stderr->upto = t_stderr->fbuf;
	t_stderr->bufStartR = 0;
	t_stderr->mode = __WRITE_MODE;
	t_stderr->update = 0;
	t_stderr->theirBuffer = 0;
	t_stderr->permfile = 1;
	t_stderr->isopen = 1;

	for (x=0; x < __NFILE; x++)
	{
		__userFiles[x] = NULL;
	}

//	tk_puts("A2\n");

	if(p)
	{
		if(*p=='"')
		{
			p++;
			argv[0] = p;
			while(p && ((*p)!='"'))p++;
//			*p++ = '\0';
			*p++ = 0;
		}else
		{
			argv[0] = p;
//			p = strchr(p, ' ');
			while(*p)
			{
				if(*p==' ')
					break;
				p++;
			}
//			if (p == NULL)
//			if(!p)
			if(!(*p))
			{
				p = "";
			}
			else
			{
//				*p = '\0';
				*p = 0;
				p++;
			}
		}
	}else
	{
		argv[0] = "";
	}

//	tk_puts("A3\n");

	if(p)
	{
		while (*p == ' ')
		{
			p++;
		}
//		if (*p == '\0')
		if (!(*p))
		{
			argv[1] = NULL;
			argc = 1;
		}
		else
		{
			for (x = 1; x < MAXPARMS; )
			{
				char srch = ' ';

				if (*p == '"')
				{
					p++;
					srch = '"';
				}
				argv[x] = p;
				x++;

				if(x>=MAXPARMS)
					break;

				p = strchr(p, srch);
//				if (p == NULL)
				if(!p)
				{
					break;
				}
				else
				{
//					*p = '\0';
					*p = 0;
					p++;
					while (*p == ' ') p++;
					if (*p == '\0') break; /* strip trailing blanks */
				}
			}
			argv[x] = NULL;
			argc = x;
		}
	}else
	{
		argv[1] = NULL;
		argc = 1;
	}

//	tk_puts("A4\n");

	__start_late();

//	tk_puts("TK: Start Main\n");

	rc = main(argc, argv);
	__exit(rc);
	return (rc);
}

void __exit(int status)
{
	int x;

#if 0
	for (x = 0; x < __NFILE; x++)
	{
		if (__userFiles[x] != NULL)
		{
			fclose(__userFiles[x]);
		}
	}
	if (stdout != NULL) fflush(stdout);
	if (stderr != NULL) fflush(stderr);
#endif

#if USE_MEMMGR
	memmgrTerm(&__memmgr);
#endif /* USE_MEMMGR */

	__exita(status);
	while(1);
}
