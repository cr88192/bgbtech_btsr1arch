#ifndef STD_FENV_H
#define STD_FENV_H

#define FE_DIVBYZERO	0x1000
#define FE_INEXACT		0x0100
#define FE_INVALID		0x0800
#define FE_OVERFLOW		0x0400
#define FE_UNDERFLOW	0x0200

#define FE_ALL_EXCEPT	0x1F00

#define FE_TONEAREST	0x0000
#define FE_TOWARDZERO	0x0001
#define FE_DOWNWARD		0x0002
#define FE_UPWARD		0x0003

#define	FE_DFL_ENV		0x0000

typedef int fexcept_t;
typedef int fenv_t;

int  feclearexcept(int v);
int  fegetexceptflag(fexcept_t *ex, int v);
int  feraiseexcept(int v);
int  fesetexceptflag(const fexcept_t *ex, int v);
int  fetestexcept(int v);
int  fegetround(void);
int  fesetround(int v);
int  fegetenv(fenv_t *env);
int  feholdexcept(fenv_t *env);
int  fesetenv(const fenv_t *env);
int  feupdateenv(const fenv_t *env);

#endif
