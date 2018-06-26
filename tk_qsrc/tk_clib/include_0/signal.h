/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  signal.h - signal header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __SIGNAL_INCLUDED
#define __SIGNAL_INCLUDED

typedef int sig_atomic_t;

void __sigdfl(int sig);
void __sigerr(int sig);
void __sigign(int sig);

#define SIG_DFL __sigdfl
#define SIG_ERR __sigerr
#define SIG_IGN __sigign

#define SIGABRT 1
#define SIGFPE 2
#define SIGILL 3
#define SIGINT 4
#define SIGSEGV 5
#define SIGTERM 6

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);

#endif
