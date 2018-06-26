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

/*
  Some constants - at present these are only defined for IBM
*/
#if defined(__MVS__) || defined (__CMS__)
/*
 This is about the nearest thing we can get to inf wthout
*/
#define HUGE_VAL 9.999999999999999999999E72

#else

#define HUGE_VAL 9.999999999999999999999E72

#endif

double ceil(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);

/* unimplemented: */


double atan2(double y, double x);
double frexp(double value, int *exp);
double ldexp(double x, int exp);
double modf(double value, double *iptr);

/*
  Implemented by d.wade - April 2006
*/

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


#ifdef __WATCOMC__
#pragma intrinsic(cos, sin, tan, exp, log, log10, sqrt)
#endif

#ifdef __IBMC__
double _Builtin __fabs(double x);
#define fabs(x) (__fabs((x)))
double _Builtin __fsin(double x);
#define sin(x) (__fsin((x)))
double _Builtin __fcos(double x);
#define cos(x) (__fcos((x)))
double _Builtin __fptan(double x);
#define tan(x) (__fptan((x)))
double _Builtin __fpatan(double x);
#define atan(x) (__fpatan((x)))
double _Builtin __facos(double x);
#define acos(x) (__facos((x)))
double _Builtin __fasin(double x);
#define asin(x) (__fasin((x)))
double _Builtin __fsqrt(double x);
#define sqrt(x) (__fsqrt((x)))
#endif

#ifdef __BORLANDC__
double __sin__(double x);
#define sin(x) (__sin__((x)))
double __cos__(double x);
#define cos(x) (__cos__((x)))
#endif

#endif

