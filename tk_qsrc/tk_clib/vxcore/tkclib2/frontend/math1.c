#include <math.h>
#include <stdint.h>

int		fpclassify(double x)
{
	uint64_t xv, fr;
	int ex;
	
	xv=__float64_getbits(x);
	ex=(xv>>52)&2047;
	if(ex==2047)
	{
		fr=xv&0x000FFFFFFFFFFFFFULL;
		if(fr!=0)
			return(FP_NAN);
		return(FP_INFINITE);
	}else if(ex==0)
	{
		fr=xv&0x000FFFFFFFFFFFFFULL;
		if(fr!=0)
			return(FP_SUBNORMAL);
		return(FP_ZERO);
	}
	return(FP_NORMAL);
}

int		isinf(double x);
{
	uint64_t xv, fr;
	int ex;
	
	xv=__float64_getbits(x);
	ex=(xv>>52)&2047;

	if(ex==2047)
	{
		fr=xv&0x000FFFFFFFFFFFFFULL;
		if(fr==0)
			return(((xv>>63)&1)?(-1):1);
		return(0);
	}
	return(0);
}

int		isnan(double x)
	{ return(fpclassify(x)==FP_NAN); }
int		isnormal(double x)
	{ return(fpclassify(x)==FP_NORMAL); }

int		isfinite(double x)
{
	int i;
	i=fpclassify(x);
	return((i!=FP_NAN) && (i!=FP_INFINITE));
}

double ceil(double x)
{
	long long ix;
	double x1;
	ix=x;
	x1=ix;
	if(x1!=x)
		{ ix++; x1=ix; }
	return(x1);
}

double fabs(double x)
{
	if(x<0)
		return(-x);
	return(x);
}

double floor(double x)
{
	long long ix;
	double x1;
	ix=x;
	x1=ix;
	return(x1);
}

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
