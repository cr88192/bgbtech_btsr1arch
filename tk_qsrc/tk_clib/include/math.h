/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  math.h - math header file.                                       */
/*                                                                   */
/*********************************************************************/

#ifndef __MATH_INCLUDED
#define __MATH_INCLUDED

#ifndef HUGE_VAL
#define HUGE_VAL 9.999999999999999999999E72
#endif

#ifndef M_E
#define M_E			2.7182818284590452353602874713527
#endif
#ifndef M_LOG2E
#define M_LOG2E		1.4426950408889634073599246810019
#endif
#ifndef M_LOG10E
#define M_LOG10E	0.43429448190325182765112891891661
#endif
#ifndef M_LN2
#define M_LN2		0.69314718055994530941723212145818
#endif
#ifndef M_LN10
#define M_LN10		2.3025850929940456840179914546844
#endif
#ifndef M_PI
#define M_PI		3.1415926535897932384626433832795
#endif
#ifndef M_PI_2
#define M_PI_2		1.5707963267948966192313216916398
#endif
#ifndef M_PI_4
#define M_PI_4		0.78539816339744830961566084581988
#endif
#ifndef M_1_PI
#define M_1_PI		0.31830988618379067153776752674503
#endif
#ifndef M_2_PI
#define M_2_PI		0.63661977236758134307553505349006
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI	1.1283791670955125738961589031215
#endif
#ifndef M_SQRT2
#define M_SQRT2		1.4142135623730950488016887242097
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2	0.70710678118654752440084436210485
#endif

int		isinf(double x);
int		isnan(double x);
int		isnormal(double x);
int		isfinite(double x);
int		fpclassify(double x);

double ceil(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);
double atan2(double y, double x);
double frexp(double value, int *rexp);
double ldexp(double x, int exp);
double modf(double value, double *iptr);
double pow(double x, double y);
double sqrt(double x);
double acos(double x);
double asin(double x);
double atan(double x);
double cos(double x);
double sin(double x);
double tan(double x);
double cosh(double x);
double sinh(double x);
double tanh(double x);
double exp(double x);
double log(double x);
double log10(double x);

double acosh(double x);
double asinh(double x);
double atanh(double x);

float ceilf(float x);
float fabsf(float x);
float floorf(float x);
float fmodf(float x, float y);
float atan2f(float y, float x);
float frexpf(float value, int *exp);
float ldexpf(float x, int exp);
float modff(float value, float *iptr);
float powf(float x, float y);
float sqrtf(float x);
float acosf(float x);
float asinf(float x);
float atanf(float x);
float cosf(float x);
float sinf(float x);
float tanf(float x);
float coshf(float x);
float sinhf(float x);
float tanhf(float x);
float expf(float x);
float logf(float x);
float log10f(float x);

double _sin_fast(double ang);
double _cos_fast(double ang);
double _sqrt_fast(double x);

long double ceill(long double x);
long double fabsl(long double x);
long double floorl(long double x);
long double fmodl(long double x, long double y);
long double atan2l(long double y, long double x);
long double frexpl(long double value, int *exp);
long double ldexpl(long double x, int exp);
long double modfl(long double value, long double *iptr);
long double powl(long double x, long double y);
long double sqrtl(long double x);
long double acosl(long double x);
long double asinl(long double x);
long double atanl(long double x);
long double cosl(long double x);
long double sinl(long double x);
long double tanl(long double x);
long double coshl(long double x);
long double sinhl(long double x);
long double tanhl(long double x);
long double expl(long double x);
long double logl(long double x);
long double log10l(long double x);

#endif

