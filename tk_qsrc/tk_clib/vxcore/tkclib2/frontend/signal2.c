#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <tkclib2/ci_stdio.h>

void __sigdfl(int sig)
{
	abort();
}

void __sigerr(int sig)
{
	abort();
}

void __sigign(int sig)
{
}

signal_t signal(int sig, signal_t func)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->signal(ctx, sig, func));
}

int raise(int sig)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->raise(ctx, sig));
}
