#ifndef _ALLOCA_H_
#define _ALLOCA_H_

#ifdef __BGBCC__
void *__alloca(int size);
#define alloca(size)	__alloca(size);
#endif

#endif
