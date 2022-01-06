#ifndef STD_UNISTD_H
#define STD_UNISTD_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int read(int handle, void *buf, size_t len);
int write(int handle, const void *buf, size_t len);
long lseek(int handle, long offset, int whence);
int close(int handle);

#endif
