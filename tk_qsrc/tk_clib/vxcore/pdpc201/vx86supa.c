#include <stdio.h>
#include <stddef.h>

#include <vxcore.h>

int __read(int handle, void *buf, size_t len, int *errind)
{
    if(errind)*errind=0;
    read(handle, buf, len);
}

int __write(int handle, const void *buf, size_t len, int *errind)
{
    if(errind)*errind=0;
    write(handle, buf, len);
}

void __seek(int handle, long offset, int whence)
    { lseek(handle, offset, whence); }

int __open(const char *a, int b, int c)
    { return(open(a, b, c)); }

void __close(int handle)
    { close(handle); }

void __remove(const char *filename)
    { vx_remove(filename); }

void __rename(const char *old, const char *new)
    { vx_rename(old, new); }

void __exita(int status)
    { vx_exit(status); }

void __datetime(void *ptr)
{
    struct {
        int year;
        int month;
        int day;
        int hours;
        int minutes;
        int seconds;
        int hundredths;
    } dt;

    vx_datetime(ptr);
}

__PDPCLIB_API__ void *_alloca(int sz)
    { return((void *)vx_malloc(sz)); }

void __allocmem(size_t size, void **ptr)
    { *ptr=(void *)vx_malloc(size); }

void __freemem(void *ptr)
    { vx_free(ptr); }

unsigned char *__envptr;

void __exec(char *cmd, void *env)
{
	vx_system(cmd);
}
