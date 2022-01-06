#ifndef STD_STDDEF_H
#define STD_STDDEF_H

#ifndef STD_PTRDIFF_T
#define STD_PTRDIFF_T
typedef long ptrdiff_t;
#endif

#ifndef STD_SIZE_T
#define STD_SIZE_T
typedef unsigned long size_t;
#endif

#ifndef STD_RSIZE_T
#define STD_RSIZE_T
typedef size_t rsize_t;
#endif

#ifndef STD_WCHAR_T
#define STD_WCHAR_T
typedef unsigned short wchar_t;
#endif

#ifndef STD_ERRNO_T
#define STD_ERRNO_T
typedef int errno_t;
#endif

#define NULL			((void *)0)
#define offsetof(x, y)	__offsetof(x, y)

#endif
