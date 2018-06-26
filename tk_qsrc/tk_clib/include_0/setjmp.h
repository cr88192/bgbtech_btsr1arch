/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  setjmp.h - setjmp header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __SETJMP_INCLUDED
#define __SETJMP_INCLUDED

typedef struct {
#if defined(__MVS__) || defined(__CMS__)
    int regs[15];
#elif defined(__WIN32__) || defined(__32BIT__) || defined(__OS2__) \
    || defined(__gnu_linux__)
    int ebx;
    int ecx;
    int edx;
    int edi;
    int esi;
    int esp;
    int ebp;
    int retaddr;
#elif defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC)
    int bx;
    int cx;
    int dx;
    int di;
    int si;
    int sp;
    int bp;
    void *retaddr;
    int ds;
    int es;
#endif
    int retval;
} jmp_buf[1];

int __setj(jmp_buf env);
#define setjmp(x) (__setj(x))
void longjmp(jmp_buf env, int val);

#endif
