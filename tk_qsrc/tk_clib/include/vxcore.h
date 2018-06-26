#ifndef __VXCORE_H__
#define __VXCORE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#include <errno.h>
#include <signal.h>

#include <fcntl.h>
#include <dlfcn.h>

// #include <vxsys/vxsyscall.h>

#include <dirent.h>
#include <sys/mman.h>

#include <sys/socket.h>

#ifdef VXCORE_DLL
#define VXCORE_API __declspec(dllexport)
#else
#define VXCORE_API
#endif

// int open(char *name, int mode, int flag);

#endif
