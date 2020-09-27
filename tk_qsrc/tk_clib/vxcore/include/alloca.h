#ifndef __ALLOCA_H
#define __ALLOCA_H

#ifdef __BGBCC__
void *__alloca(int size);
#define alloca(size)	__alloca(size);
#endif

#endif
