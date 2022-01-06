#ifndef STD_ASSERT_H
#define STD_ASSERT_H

int __assert(char *x, char *y, int z);

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(x) (x) ? ((void)0) : \
    __assert(#x, __FILE__, __LINE__)
#endif

#endif
