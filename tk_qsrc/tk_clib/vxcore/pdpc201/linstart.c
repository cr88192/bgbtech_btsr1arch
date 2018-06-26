/* Startup code for Linux */
/* written by Paul Edwards */
/* released to the public domain */

#include "errno.h"
#include "stddef.h"


extern int __start(int argc, char **argv);
extern int __exit(int rc);


/* We can get away with a minimal startup code, plus make it
   a C program. There is no return address. Instead, on the
   stack is a count, followed by all the parameters as pointers */

int _start(char *p)
{
    int rc;

    rc = __start(*(int *)(&p - 1), &p);
    __exit(rc);
    return (rc);
}

static char membuf[31000000];

void *__allocmem(size_t size)
{
    return (membuf);
}
