/* written by Paul Edwards */
/* released to the public domain */
/* pdossupc - support routines for pdos */

#include "stddef.h"

#include <pos.h>
#include <support.h>

int __open(const char *filename, int mode, int *errind)
{
    int handle;
    int ret;

    ret = PosOpenFile(filename, mode, &handle);
    *errind = ret;
    return (handle);
}

int __read(int handle, void *buf, size_t len, int *errind)
{
    long readBytes;

    PosReadFile(handle, buf, len, &readBytes);
    *errind = 0;
    return (readBytes);
}

int __write(int handle, const void *buf, size_t len, int *errind)
{
    int ret;

    ret = PosWriteFile(handle, buf, len);
    *errind = 0;
    return (ret);
}

void __seek(int handle, long offset, int whence)
{
    PosMoveFilePointer(handle, offset, whence);
    return;
}

void __close(int handle)
{
    PosCloseFile(handle);
    return;
}

void __remove(const char *filename)
{
    PosDeleteFile(filename);
    return;
}

void __rename(const char *old, const char *new)
{
    PosRenameFile(old, new);
    return;
}

void __allocmem(size_t size, void **ptr)
{
    *ptr = PosAllocMem(size);
    return;
}

void __freemem(void *ptr)
{
    PosFreeMem(ptr);
    return;
}

void __exec(char *cmd, void *env)
{
    PosExec(cmd, env);
    return;
}

void __datetime(void *ptr)
{
    int year, month, day, dow;
    int hour, minute, second, hundredths;
    int *iptr = ptr;

    PosGetSystemDate(&year, &month, &day, &dow);
    iptr[0] = year;
    iptr[1] = month;
    iptr[2] = day;
    iptr[3] = dow;
    PosGetSystemTime(&hour, &minute, &second, &hundredths);
    iptr[4] = hour;
    iptr[5] = minute;
    iptr[6] = second;
    iptr[7] = hundredths;
    return;
}

int __pstart(int *i1, int *i2, int *i3, POS_EPARMS *exep)
{
    return (__start(i1, i2, i3, exep));
}

#ifdef PDOS_MAIN_ENTRY
void __callback(void)
{
    __exit(0);
    return;
}
#endif

#ifdef PDOS_MAIN_ENTRY
void __main(int ebp, int retaddr, int i1, int i2, int i3, POS_EPARMS *exep)
{
    __start(&i1, &i2, &i3, exep);
    exep->callback = __callback;
    return;
}
#else
void __main(void)
{
    return;
}
#endif

void __exita(int retcode)
{
#ifndef PDOS_RET_EXIT
     PosTerminate(retcode);
#endif
    return;
}

void __displayc(void)
{
    *__vidptr = 'C';
    return;
}
