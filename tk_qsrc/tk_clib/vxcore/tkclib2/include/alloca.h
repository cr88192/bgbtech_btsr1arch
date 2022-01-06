#ifndef STD_ALLOCA_H
#define STD_ALLOCA_H

void *__alloca(int size);

#define alloca(size)	__alloca(size);

#endif
