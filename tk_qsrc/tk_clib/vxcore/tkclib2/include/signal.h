#ifndef STD_SIGNAL_H
#define STD_SIGNAL_H

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

typedef void (*signal_t)(int);

signal_t signal(int sig, signal_t func);
int raise(int sig);

#endif
