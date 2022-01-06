#ifndef STD_STDARG_H
#define STD_STDARG_H

#ifndef STD_VALIST_T
#define STD_VALIST_T
typedef __builtin_va_list va_list;
#endif

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)
#define __va_copy(d,s)  __builtin_va_copy(d,s)

#endif
