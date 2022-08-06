#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_PI_2 /* (M_PI/2) */
#define M_PI_2 1.5707963267948966192313216916398
#endif

#ifndef M_TAU	/* (2.0*M_PI) */
#define M_TAU 6.283185307179586476925286766559
#endif

#ifndef M_TAU_R	/* (1.0/(2*M_PI)) */
#define M_TAU_R 0.15915494309189533576888376337251
#endif

static const double sintab_c00= 1;
static const double sintab_c01=-0.16666666666666665741;
static const double sintab_c02= 0.0083333333333333332177;
static const double sintab_c03=-0.00019841269841269841253;
static const double sintab_c04= 2.7557319223985892511e-06;
static const double sintab_c05=-2.5052108385441720224e-08;
static const double sintab_c06= 1.6059043836821613341e-10;
static const double sintab_c07=-7.6471637318198164055e-13;
static const double sintab_c08= 2.8114572543455205981e-15;
static const double sintab_c09=-8.2206352466243294955e-18;
static const double sintab_c10= 1.9572941063391262595e-20;
static const double sintab_c11=-3.8681701706306835384e-23;
static const double sintab_c12= 6.4469502843844735895e-26;
static const double sintab_c13=-9.1836898637955460054e-29;
static const double sintab_c14= 1.1309962886447718071e-31;
static const double sintab_c15=-1.2161250415535181076e-34;
static const double sintab_c16= 1.1516335620771950891e-37;
static const double sintab_c17=-9.6775929586318906719e-41;
static const double sintab_c18= 7.2654601791530723547e-44;
static const double sintab_c19=-4.9024697565135435190e-47;
static const double sintab_c20= 2.9893108271424050896e-50;

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

double sin(double ang)
{
	double t, x, th, th2;
	int i;

    i=ang*M_TAU_R;
    th=ang-(i*M_TAU);

	x=th;
	th2=th*th;
	t =x*sintab_c00; x*=th2;
	t+=x*sintab_c01; x*=th2;
	t+=x*sintab_c02; x*=th2;
	t+=x*sintab_c03; x*=th2;
	t+=x*sintab_c04; x*=th2;
	t+=x*sintab_c05; x*=th2;
	t+=x*sintab_c06; x*=th2;
	t+=x*sintab_c07; x*=th2;
	t+=x*sintab_c08; x*=th2;
	t+=x*sintab_c09; x*=th2;
	t+=x*sintab_c10; x*=th2;
	t+=x*sintab_c11; x*=th2;
	t+=x*sintab_c12; x*=th2;
	t+=x*sintab_c13; x*=th2;
	t+=x*sintab_c14; x*=th2;
	t+=x*sintab_c15; x*=th2;
	t+=x*sintab_c16; x*=th2;
	t+=x*sintab_c17; x*=th2;
	t+=x*sintab_c18; x*=th2;
	t+=x*sintab_c19; x*=th2;
	t+=x*sintab_c20; x*=th2;

//	__debugbreak();

	return(t);
}

double cos(double ang)
{
	return(sin(ang+M_PI_2));
}

double fsqrt_approx(double x)
{
	unsigned long long t0, t1;
	t0=*(unsigned long long *)(&x);
	t1=(t0>>1)+0x1FF8000000000000ULL;
	return(*(double *)(&t1));
}

double sqrt(double x)
{
	unsigned long long t0, t1;
	double xs, yn, xsr, ynn;

	if(x<0.0)
		{ errno=EDOM; return(0.0); }
	if(x==0.0)
		return(0.0);
	if(x!=x)
		return(0.0);

#ifdef __BJX2__
	t0=__float64_getbits(x);
	t1=(t0>>1)+0x1FF8000000000000ULL;
	xs=__float64_frombits(t1);
#else
	t0=*(unsigned long long *)(&x);
	t1=(t0>>1)+0x1FF8000000000000ULL;
	xs=*(double *)(&t1);
#endif
	xsr=0.5f/((float)xs);
	yn=xs*xs;	xs-=(yn-x)*xsr;
	yn=xs*xs;	xs-=(yn-x)*xsr;
	yn=xs*xs;	xs-=(yn-x)*xsr;
	yn=xs*xs;	xs-=(yn-x)*xsr;
	yn=xs*xs;	xs-=(yn-x)*xsr;
	yn=xs*xs;	xs-=(yn-x)*xsr;
	return(xs);
}

double fmod(double x, double y)
{
	long imod;
	if(y == 0.0)
		return (0.0);
	imod = x / y;
	return(x-(imod*y));
}

double atan2(double y, double x)
{
	if(x >= y)
	{
		if(x >= -y)
			return(atan(y/x));
		return(-M_PI_2 - atan(x/y));
	}
	if(x >= -y)
		return(M_PI_2 - atan(x/y));
	if(y>=0)
		return(M_PI + atan(y/x));
	return(-M_PI + atan(y/x));
}

double frexp(double value, int *rexp)
{
	double v;
	int ex;
	v=x; ex=0;
	while(fabs(v)>1.0)
		{ v=v*0.5; ex++; }
	while(fabs(v)<0.5)
		{ v=v*2.0; ex--; }
	*rexp=ex;
	return(v);
}

double ldexp(double x, int exp)
{
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
}

double modf(double value, double *iptr)
{
	int neg = 0;
	long li;
	if (value < 0)
		{ neg = 1; value = -value; }
	li = (long)value;
	value -= li;
	if (neg)
		{ value = -value; li = -li; }
	*iptr = li;
	return(value);
}

double pow(double x, double y)
{
	if (x < 0.0)
		{ errno=EDOM; return(0.0); }
	if (y == 0.0)
		return (1.0);
	return (exp(y*log(x)));
}

double acos(double x)
{
	if(fabs(x)>1.0)
		{ errno=EDOM; return (HUGE_VAL); }
	if(x<0.0)
		return(M_PI - acos(-x));
	return(asin(sqrt(1.0 - x*x)));
}

double tan(double ang)
{
	double sc, cc;
	
	sc=sin(x);	cc=cos(x);
	if(cc == 0.0)
		{ errno=EDOM; return (HUGE_VAL); }

	return(sc/cc);

#if 0
	double t, x, th, th2;
	long i;

    i=ang*M_TAU_R;
    th=ang-(i*M_TAU);

	x=th;
	th2=th*th;
	t =x*( 1.0      ); x*=th2;
	t+=x*( 1.0/  3.0); x*=th2;
	t+=x*( 2.0/ 15.0); x*=th2;
	t+=x*(17.0/315.0); x*=th2;
	return(t);
#endif
}

double asin(double x)
{
	double t, x, th, th2;
	long i;

    i=ang*M_TAU_R;
    th=ang-(i*M_TAU);

	x=th;
	th2=th*th;
	t =x*( 1.0      ); x*=th2;
	t+=x*( 1.0/  6.0); x*=th2;
	t+=x*( 3.0/ 40.0); x*=th2;
//	t+=x*(-1.0/  7.0); x*=th2;
//	t+=x*( 1.0/  9.0); x*=th2;
//	t+=x*(-1.0/ 11.0); x*=th2;
	return(t);
}

double atan(double ang)
{
	double t, x, th, th2;
	long i;

    i=ang*M_TAU_R;
    th=ang-(i*M_TAU);

	x=th;
	th2=th*th;
	t =x*( 1.0      ); x*=th2;
	t+=x*(-1.0/  3.0); x*=th2;
	t+=x*( 1.0/  5.0); x*=th2;
	t+=x*(-1.0/  7.0); x*=th2;
	t+=x*( 1.0/  9.0); x*=th2;
	t+=x*(-1.0/ 11.0); x*=th2;
	return(t);
}

double exp(double th)
{
	double t, x, th2;
	
	x=th;
	th2=th*th;
	t =    1.0;
	t+=x*( 1.0        ); x*=th;
	t+=x*( 1.0/    2.0); x*=th;
	t+=x*( 1.0/    6.0); x*=th;
	t+=x*( 1.0/   24.0); x*=th;
	t+=x*( 1.0/  120.0); x*=th;
	t+=x*( 1.0/  720.0); x*=th;
	t+=x*( 1.0/ 5040.0); x*=th;
	t+=x*( 1.0/40320.0); x*=th;
	return(t);
}

double log(double x)
{
	double t, xn1, rx, tn, td;
	
	xn1=x-1;	rx=1/x;
	
	tn=xn1; td=rx;
	t=tn*td;			tn*=xn1;	td*=rx;
	t+=tn*td*(1/2.0);	tn*=xn1;	td*=rx;
	t+=tn*td*(1/3.0);	tn*=xn1;	td*=rx;
	t+=tn*td*(1/4.0);	tn*=xn1;	td*=rx;
	t+=tn*td*(1/5.0);	tn*=xn1;	td*=rx;
	t+=tn*td*(1/6.0);	tn*=xn1;	td*=rx;
	t+=tn*td*(1/7.0);	tn*=xn1;	td*=rx;
	t+=tn*td*(1/8.0);	tn*=xn1;	td*=rx;
	
	return(t);
}

double log10(double x)
{
	return(log(x)/M_LN10);
}


double cosh(double x);
{
	double d;
	d=exp(x);
	return(0.5*(d+(1.0/d)));
}

double sinh(double x);
{
	double d;
	d=exp(x);
	return(0.5*(d-(1.0/d)));
}

double tanh(double x);
{
	double d;
	d=exp(-2.0 * x);
	return((1.0-d)/(1.0+d));
}

double acosh(double x)
{
	return(log(x+sqrt(x*x-1)));
}

double asinh(double x)
{
	return(log(x+sqrt(x*x+1)));
}

double atanh(double x)
{
	return(0.5*log((1+x)/(1-x)));
}
