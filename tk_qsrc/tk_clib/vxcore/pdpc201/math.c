/*********************************************************************/
/*																	*/
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																	*/
/*  9-April-2006 D.Wade												*/
/*		Moved definitions for HUGE_VAL to math.h					*/
/*		Inserted argument rang checks in :-							*/
/*		acos														*/
/*																	*/
/*																	*/
/*																	*/
/*  2-April-2006 D.Wade added code for the :-						*/
/*																	*/
/*		acos(double x);												*/
/*		asin(double x);												*/
/*		atan(double x);												*/
/*		cos(double x);												*/
/*		sin(double x);												*/
/*		tan(double x);												*/
/*		cosh(double x);												*/
/*		sinh(double x);												*/
/*		tanh(double x);												*/
/*		exp(double x);												*/
/*		frexp(double value, int *exp);								*/
/*		ldexp(double x, int exp);									*/
/*		log(double x);												*/
/*		log10(double x);											*/
/*		modf(double value, double *iptr);							*/
/*		pow(double x, double y);									*/
/*		sqrt(double x);												*/
/*																	*/
/* Note:-															*/
/*  In order to avoide Copyright these functions are generally		*/
/*  implemented using Taylor Series. As a result they are a little   */
/*  slower that the equivalents in many maths packages.				*/
/*																	*/
/*********************************************************************/
/*********************************************************************/
/*																	*/
/*  math.c - implementation of stuff in math.h						*/
/*																	*/
/*********************************************************************/

#include "math.h"
#include "float.h"
#include "errno.h"
#include "stddef.h"

/*

  Some constants to make life easier elsewhere
  (These should I guess be in math.h)

*/
// static const double pi   = 3.1415926535897932384626433832795;
// static const double ln10 = 2.3025850929940456840179914546844;
// static const double ln2 = 0.69314718055994530941723212145818 ;


__PDPCLIB_API__ double ceil(double x)
{
	int y;

	y = (int)x;
	if ((double)y < x)
	{
		y++;
	}
	return ((double)y);
}

#ifdef fabs
#undef fabs
#endif
__PDPCLIB_API__ double fabs(double x)
{
	if (x < 0.0)
		{ x = -x; }
	return (x);
}

__PDPCLIB_API__ double floor(double x)
{
	int y;

	if (x < 0.0)
	{
		y = (int)x;
		if ((double)y != x)
		{
			y--;
		}
	}
	else
	{
		y = (int)x;
	}
	return ((double)y);
}

__PDPCLIB_API__ double fmod(double x, double y)
{
	int imod;
	if(y == 0.0) return (0.0);
	imod = x / y;
	return ((double)x-((double)imod*y));
}

#ifdef acos
#undef acos
#endif
/*

  For cos just use (sin(x)**2 + cos(x)**2)=1
  Note:- asin(x) decides which taylor series
  to use to ensure quickest convergence.

*/
__PDPCLIB_API__ double acos(double x)
{

/*
*/

	if ( fabs(x) > 1.0 ) /* is argument out of range */
	{
		errno=EDOM;
		return (HUGE_VAL);
	}
	if ( x < 0.0 ) return ( M_PI - acos(-x) ) ;

	return ( asin ( sqrt(1.0 - x*x) ) );

}

#ifdef asin
#undef asin
#endif
/*

   This routines Calculate arcsin(x) & arccos(x).

   Note if "x" is close to "1" the series converges slowly.
   To avoid this we use (sin(x)**2 + cos(x)**2)=1
   and fact cos(x)=sin(x+M_PI/2)

*/

__PDPCLIB_API__ double asin (double y)
{
	int i, n;
	double term,answer,work,x,powx,coef;

	x = y;

/*
  if arg is -ve then we want "-asin(-x)"
*/

	if (x <0.0 ) return ( -asin(-x) );

/*
	If arg is > 1.0 we can't calculate
	(note also < -1.0 but previous statement removes this case)
*/
	if ( x > 1.0 )
	{
		errno=EDOM;
		return(HUGE_VAL);
	}

/*
 now check for large(ish) x > 0.6
*/

	if( x > 0.75 )
	{
		x = ( sqrt(1.0 - (x*x) ) );
		return((M_PI/2.0)-asin(x));
	}

/*
	arcsin(x) = x + 1/2 (x^3/3) + (1/2)(3/4)(x^5/5) +
		(1/2)(3/4)(5/6)(x^7/7) + ...
*/
	i=1;
	answer=x;
	term = 1;
	coef = 1;
	powx = x;

	n=256;
	while ((n--)>0)
	{
		work = i;
		coef = (coef * work)/(work+1);
		powx = powx * x * x;
		term =  coef * powx / (work + 2.0);
		if ( answer == (answer + term) )break;
		answer = answer + (term);
		i+=2;
	}

	return(answer);
}


#ifdef atan
#undef atan
#endif
/*

	Because atan(x) is valid for large values of "x" &
	the taylor series converges more slowly for large "X"
	we use the following

	1. Reduce to the first octant by using :-

		atan(-x)=-atan(x),
		atan(1/x)=PI/2-atan(x)

	2. Reduce further so that |x| less than tan(PI/12)

		atan(x)=M_PI/6+atan((X*sqrt(3)-1)/(x+sqrt(3)))

	3. Then use the taylor series

		atan(x) = x - x**3 + x**5 - x**7
						----   ----   ----
						3		5		7

*/

__PDPCLIB_API__ double atan (double x)
{
	int i, n;
	double sqrt3;
	double term,answer,work,powx;

/*
  if arg is -ve then we want "-atan(-x)"
*/

	if ( x<0.0 ) return ( -atan(-x) );

/*
 If arg is large we can't calculate
 use atan(1/x)=PI/2-atan(x)
*/

	if ( x > 1.0 ) return ((M_PI/2.0) - atan(1.0/x));

/*
 now check for large(ish) x > tan(15) (0.26794919243112)
 if so use atan(x)=M_PI/6+atan((X*SQRT3-1)/(X+SQRT3))
*/

//	if( x > (2.0 - sqrt(3.0)))
//	return( (M_PI/6.0) + atan( ( x * sqrt(3.0)-1.0 ) / (x + sqrt(3.0) ) ) );

	sqrt3 = 1.7320508075688772935274463415059;

	if( x > (2.0 - sqrt3))
	return( (M_PI/6.0) + atan( ( x * sqrt3-1.0 ) / (x + sqrt3 ) ) );

/*
*		atan(x) = x - x**3 + x**5 - x**7
*					----   ----   ----
*						3		5		7
*/

	i=1;
	answer=x;
	term = x;
	powx = x;

	n=256;

	while ((n--)>0)
	{
		work = i;
		powx = 0.0 - powx * x * x;
		term = powx / (work + 2.0);
		if ( answer == (answer + term) )break;
		answer = answer + (term);
		i+=2;
	}

	return(answer);

}


/* atan2 was taken from libnix and modified slightly */

__PDPCLIB_API__ double atan2(double y,double x)
{
	return (x >= y) ?
				(x >= -y ? atan(y/x) : -M_PI/2 - atan(x/y))
				:
				(x >= -y ? M_PI/2 - atan(x/y)
						: (y >= 0) ? M_PI + atan(y/x)
									: -M_PI + atan(y/x));
}

#if 0

#ifdef cos
#undef cos
#endif
__PDPCLIB_API__ double cos(double x)
{
/*

   Calculate COS using Taylor series.

   sin(x) = 1 - x**2  +  x**4  - x**6 + x**8
				====	====	====   ====	.........
					2!		4!		6!	8!

   Note whilst this is accurate it can be slow for large
   values of "X" so we scale

*/

	int i, n;
	double term,answer,work,x1;

/*
	Scale arguments to be in range 1 => M_PI
*/

	i = x/(2*M_PI);
	x1 =  x - (i * (2.0 * M_PI));

	i=1;
	term=answer=1;

	n=256;
	while ((n--)>0)
	{
		work = i;
		term = -(term * x1 * x1)/(work * (work + 1.0));
		if ( answer == (answer + term) )break;
		answer = answer + term;
		i += 2;
	}

	return(answer);

}

#ifdef sin
#undef sin
#endif
__PDPCLIB_API__ double sin(double x)
{
/*

   Calculate SIN using Taylor series.

   sin(x) = x - x**3  +  x**5  - x**7 + x**9
				====	====	====   ====
					3!		5!		7!	9!

   Note whilst this is accurate it can be slow for large values
   of "X" so we scale

*/

	int i, n;
	double term,answer,work,x1;
	double f0, f1;

/*
  scale so series converges pretty quickly
*/
	i = x/(2.0*M_PI);
	x1 =  x - (i * (2.0 * M_PI));

//	__debugbreak();
/*
 set up initial term
*/
	i=1;
	term=answer=x1;
/*
 loop until no more changes
*/

#if 1
	n=256;
	while ((n--)>0)
	{
		work = i+1;
		f0 = -(term * x1 * x1);
		f1 = (work * (work + 1.0));
		term = f0 / f1;
//		__debugbreak();
//		term = -(term * x1 * x1)/(work * (work + 1.0));
		if ( answer == (answer + term) )break;
		answer = answer + term;
		i = i+2;
	}
#endif

	return(answer);
}

#endif

#ifdef tan
#undef tan
#endif
__PDPCLIB_API__ double tan (double x)
{
/*

  use tan = sin(x)/cos(x)
  if cos(x) is 0 then return HUGE_VAL else return sin/cos

  *** need to set ERROR for overflow ***

*/
	double temp;

	temp=cos(x);
	if (temp == 0.0 )
	{
		/* errno=EDOM; don't seem to return an error here */
		return (HUGE_VAL); /* need to set error here */
	}
	return ( sin(x)/cos(x) );
}

/*

  Hyperbolic functions

  SINH(X) = (E**X-E**(-1))/2
  COSH(X) = (E**X+E**(-1))/2

*/
__PDPCLIB_API__ double cosh(double x)
{
	double dexpx;

	dexpx = exp(x);

	return( 0.5 * (dexpx + (1.0/dexpx) ) );

}

__PDPCLIB_API__ double sinh(double x)
{
	double dexpx;

	dexpx = exp(x);

	return( 0.5 * (dexpx - (1.0/dexpx) ) );
}

/*
	tanh returns the hyperbolic area tangent of floating point argument x.
*/

__PDPCLIB_API__ double tanh(double x)
{
	double dexp2;

	dexp2 = exp( -2.0 * x);
	return ( (1.0  - dexp2) /  (1.0 + dexp2) );
}

/*

exp(x) = 1 + x + x2/2 + x3/6 + x4/24 + x5/120 + ... + xn/n! + ...

*/
__PDPCLIB_API__ double exp (double x)
{
	int i, n;
	double term,answer,work;

	i=2;
	term=x;
	answer=x;

//	n=256;
//   while ((n--)>0)
	for(n=0; n<12; n++)
	{
		work = i;
		term =  (term * x)/work;
//		if ( answer == (answer + term) )break;
		answer = answer + (term);
		i++;
	}

	answer=answer+1.0;
	return(answer);
}

/*

   Calculate LOG using Taylor series.

   log(1+ x) = x - x**2  +  x**3  - x**4 + x**5
					====	====	====   ====	.........
					2		3		4	8

   Note this only works for small x so we scale....

*/
__PDPCLIB_API__ double log (double x0)
{
	int i, n, scale;
	double term,answer,work,xs;

	if (x0 <= 0 )
	{
		/* need to set signal */
		errno=EDOM;
		return (HUGE_VAL);
	}
	if(x0 == 1.0)
		return(0.0);

#if 1
/*
  Scale arguments to be in range 1 < x <= 10
*/

/*
	scale = 0;
	xs = x0;
	while ( xs > 10.0 ) { scale ++; xs=xs/10.0;}
	while ( xs < 1.0 ) { scale --; xs=xs*10.0;}
*/

//	scale = -1; xs = x;
//	while ( xs >= 1.0 ) { scale ++; xs=xs*0.5;}
//	while ( xs <  0.5 ) { scale --; xs=xs*2.0;}

	scale = 0; xs = x0;
//	while ( xs >= 2.0 ) { scale ++; xs=xs*0.5; }
//	while ( xs <  1.0 ) { scale --; xs=xs*2.0; }
//	xs = xs - 1.0;

	xs = frexp(x0,&scale);
	xs = xs - 1.0;

//	xs = (1.0 * xs) - 1.0;
	scale = scale - 0;

//	__debugbreak();

	i=2;
	term=xs;
	answer=xs;

//	n=256;
//	while ((n--)>0)
//	for(n=0; n<12; n++)
//	for(n=0; n<6; n++)
	for(i=2; i<10; i++)
	{
//		work = i;
		work = 1.0 / i;
		term = - (term * xs);
//		if ( answer == (answer + (term/work)) )break;
//		answer = answer + (term/work);
		answer = answer + (term*work);
//		i++;
//		__debugbreak();
	}

	work = (double)scale;
	answer = answer + (work * M_LN2);
//	answer = answer + (double)scale * M_LN2;
	return(answer);
#endif
}


__PDPCLIB_API__ double log10(double x)
{
	return ( log(x) / M_LN10 );
}


/*

   This code uses log and exp to calculate x to the power y.
   If

*/

__PDPCLIB_API__ double pow(double x, double y)
{
#if 0
	int j, n, neg;
	double yy,xx;
	j=y;
	yy=j;
	if( yy == y)
	{
		neg=0;
		xx = x;
		if ( y < 0 ){neg = 1; j = -j;}
		if ( y == 0) return (1.0);
		--j;
		n=256;
		while(j>0 && ((n--)>0))
			{ xx=xx * x; j--;}
		if(neg)xx=1.0/xx;
		return (xx);
	}
#endif

	if (x < 0.0)
	{
		errno=EDOM;
		return(0.0);
	}
	if (y == 0.0)
		return (1.0);

	return (exp(y*log(x)));
}

#ifdef sqrt
#undef sqrt
#endif
/*

   pretty tivial code here.

	1) Scale x such that 1 <= x <= 4.0

	2) Use newton Raphson to calculate root.

	4) multiply back up.

   Because we only scale by "4" this is pretty slow....

*/

extern double __sqrt_d(double x);

double fsqrt_approx(double x)
{
	unsigned long long t0, t1;
	t0=*(unsigned long long *)(&x);
	t1=(t0>>1)+0x1FF8000000000000ULL;
	return(*(double *)(&t1));
}

__PDPCLIB_API__ double sqrt(double x)
{
	double xs, yn, xsr, ynn;
	double pow1;
	int i, n;

	if (x < 0.0)
	{
		errno=EDOM;
		return(0.0);
	}

	if (x == 0.0)
		return (0.0);

	if (x != x)
		return (0.0);

//	xs=__sqrt_d(x);
//	*(int *)-1=-1;
//	return(xs);

/* BGB: just doing this, original doesn't really work. */
	xs=fsqrt_approx(x);
//	__debugbreak();
//	xsr=0.5/xs;
	xsr=0.5f/((float)xs);		//faster approximate

#if 1
	for(i=0; i<6; i++)
	{
		yn=xs*xs;
//		xs-=(yn-x)*0.125;
		xs-=(yn-x)*xsr;
	}
#endif

	return(xs);

/*

  Scale argument 1 <= x <= 4

*/

#if 0
	xs=x;
	pow1=1;
	n=256;

	while((xs<1.0) && ((n--)>0))
		{ xs=xs*4.0; pow1=pow1/2.0; }
	while((xs>=4.0) && ((n--)>0))
		{ xs=xs/4.0; pow1=pow1*2.0; }

	if(n<=0)
	{
//		tk_puts("sqrt fail\n");
//		tk_printf("sqrt fail %f\n", x);
		return(0);
	}

/*
  calculate using Newton raphson
  use x0 = x/2.0
*/

	i=0;
	yn = xs/2.0;
	ynn = 0;
	while(1)
//	while((n--)>0)
	{
		ynn = (yn + xs/yn)*0.5;
		if ( fabs(ynn-yn) <= (10.0 * DBL_MIN) )
			break;
		yn=ynn;
		if ( i > 16 )
			break;
		i++;
	}
	return (ynn*pow1);
#endif
}


__PDPCLIB_API__ double frexp(double x, int *rexp)
{
#if 1
	double v;
	int ex;
	
	v=x; ex=0;
	while(fabs(v)>1.0)
		{ v=v*0.5; ex++; }
	while(fabs(v)<0.5)
		{ v=v*2.0; ex--; }
	*rexp=ex;
	return(v);
#endif

#if 0
	unsigned long long xb;
	int ex;
	
	xb = __float64_getbits(x);
	ex=((xb>>52)&2047)-1022;
	xb-=((long long)ex)<<52;
	*rexp=ex;
	return(__float64_frombits(xb));
#endif

#if 0
/* BGB: WTF is this? */

/*
  split float into fraction and mantissa
  note this is not so easy for IBM as it uses HEX float
*/
	union dblhex
	{
		double d;
		unsigned short s[4];
	};
	union dblhex split;

	if ( x == 0.0)
	{
		*exp = 0;
		return (0.0);
	}

	split.d = x;
	*exp = (((split.s[0] >> 8) & 0x007f)-64) * 4;
	split.s[0] = split.s[0] & 0x80ff;
	split.s[0] = split.s[0] | 0x4000;
	/* following code adjust for fact IBM has hex float */
	while ( (fabs(split.d) < 0.5) && (split.d != 0) )
	{
		split.d = split.d * 2;
		*exp =( *exp ) - 1;
	}
	/*	*/
	return(split.d);
#endif
}

__PDPCLIB_API__ double ldexp(double x, int exp)
{
#if 1
	double v;
	int i;
	
	v=x;
	if(exp>0)
	{
		i=exp;
		while(i--)
			v=v*2.0;
	}
	else if(exp<0)
	{
		i=-exp;
		while(i--)
			v=v*0.5;
	}
	return(v);
#endif

#if 0
	unsigned long long xb;
	int ex;
	xb = __float64_getbits(x);
	xb += ((long long)exp)<<52;
	return(__float64_frombits(xb));
#endif

#if 0
/* BGB: WTF is this? */

/*
  note this is not so easy for IBM as it uses HEX float
*/
	int bin_exp,hex_exp,adj_exp;
	union dblhex
	{
		double d;
		unsigned short s[4];
	};
	union dblhex split;
/*
	note "X" mauy already have an exponent => extract it
*/
	split.d = frexp(x,&bin_exp);
	bin_exp = bin_exp + exp;  /* add in from caller */
/* need to test for sensible value here */
	hex_exp =  (bin_exp / 4); /* convert back to HEX */
	adj_exp =  bin_exp - (hex_exp * 4);
	if (adj_exp < 0){ hex_exp=hex_exp -1; adj_exp = 4 + adj_exp;}
	split.s[0] = split.s[0] & 0x80ff;
	split.s[0] = split.s[0] | (((hex_exp+64)  << 8) & 0x7f00);
	/* following code adjust for fact IBM has hex float */
	/* well it will I have done */
	while ( adj_exp > 0 )
	{
		split.d = split.d * 2;
		--adj_exp;
	}
	/**/
	return(split.d);
#endif
}

__PDPCLIB_API__ double modf(double value, double *iptr)
{
	int neg = 0;
	long i;

	if (value < 0)
	{
		neg = 1;
		value = -value;
	}
	i = (long)value;
	value -= i;
	if (neg)
	{
		value = -value;
		i = -i;
	}
	*iptr = i;
	return (value);
}
