/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  float.h - float header file.                                     */
/*                                                                   */
/*********************************************************************/

#ifndef __FLOAT_INCLUDED
#define __FLOAT_INCLUDED

#if defined(__CMS__) || defined(__MVS__)
/*
  IBM 360 & 370 use "HEX" floating point
  float is 32 bits, double & long double are 64 bit.
  Although some models can do 128 bit (actually 120 bit)
  GCC does not implement this and "long double" is same as "double"
*/

/* rounding direction is unpredictable */
#define FLT_ROUNDS 0

/* Floating point is HEX so RADIX is base 16 */
#define FLT_RADIX 16

/* Note FLT_RADIX is 16 these are smaller than normal*/
#define FLT_MANT_DIG 6
#define DBL_MANT_DIG 14
/* don't use 128 bit floats so this is still 14 */
#define LDBL_MANT_DIG 14


/* As IBM uses hex float with "wobbling precision" these are approximate */
#define FLT_DIG 7
#define DBL_DIG 15
#define LDBL_DIG 15


#define FLT_MIN_EXP (-64)
#define DBL_MIN_EXP (-64)
#define LDBL_MIN_EXP (-64)

#define FLT_MIN_10_EXP (-78)
#define DBL_MIN_10_EXP (-78)
#define LDBL_MIN_10_EXP (-78)

#define FLT_MAX_EXP 63
#define DBL_MAX_EXP 63
#define LDBL_MAX_EXP 63

#define FLT_MAX_10_EXP 75
#define DBL_MAX_10_EXP 75
#define LDBL_MAX_10_EXP 75

/*
  This is a temporary fiddle to get round bug in GCC
  scanning of ASCII to Floats.
*/
typedef union {
    unsigned short _HexVal[4];
    double _Dval;
    float  _Fval;
    long _Lval;
} _HexFloat;

static _HexFloat _FltMax =  {{0x7fff, 0xffff ,0xffff ,0xffff}};
static _HexFloat _FltMin =  {{0x0010, 0x0000 ,0x0000 ,0x0000}};
static _HexFloat _DblMax =  {{0x7fff ,0xffff ,0xffff ,0xffff}};
static _HexFloat _DblMin =  {{0x0010 ,0x0000 ,0x0000 ,0x0000}};
static _HexFloat _FltEps =  {{0x3C10 ,0x0000 ,0x0000 ,0x0000}};
static _HexFloat _DblEps =  {{0x3410 ,0x0000 ,0x0000 ,0x0000}};

#define FLT_MAX _FltMax._Fval
#define DBL_MAX _DblMax._Dval
#define LDBL_MAX _DblMax._Lval

#define FLT_MIN _FltMin._Fval
#define DBL_MIN _DblMin._Fval
#define LDBL_MIN _DblMin._Fval

#define FLT_EPSILON _FltEps._Fval
#define DBL_EPSILON _DblEps._Fval
#define LDBL_EPSILON _DblEps._Fval



#else
/*
 original stuff from Paul for IEEE maths
*/

#define FLT_ROUNDS 1
#define FLT_RADIX 2
#define FLT_MANT_DIG 24
#define DBL_MANT_DIG 53
#define LDBL_MANT_DIG 53
#define FLT_DIG 6
#define DBL_DIG 10
#define LDBL_DIG 10
#define FLT_MIN_EXP -125
#define DBL_MIN_EXP -1021
#define LDBL_MIN_EXP -1021
#define FLT_MIN_10_EXP -37
#define DBL_MIN_10_EXP -37
#define LDBL_MIN_10_EXP -37
#define FLT_MAX_EXP +128
#define DBL_MAX_EXP +1024
#define LDBL_MAX_EXP +1024
#define FLT_MAX_10_EXP +37
#define DBL_MAX_10_EXP +37
#define LDBL_MAX_10_EXP +37
#define FLT_MAX 1E+37
#define DBL_MAX 1E+37
#define LDBL_MAX 1E+37
#define FLT_EPSILON 1E-5
#define DBL_EPSILON 1E-9
#define LDBL_EPSILON 1E-9
#define FLT_MIN 1E-37
#define DBL_MIN 1E-37
#define LDBL_MIN 1E-37
#endif

#endif

