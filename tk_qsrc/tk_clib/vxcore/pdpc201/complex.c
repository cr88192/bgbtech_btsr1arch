#include <complex.h>

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

#ifdef __BGBCC__
#define CPX_REAL(x)		x.r
#define CPX_IMAG(x)		x.i
#define CPX_NEW(x, y)	((_Complex double) {x, y})
#else
#define CPX_REAL(x)		creal(x)
#define CPX_IMAG(x)		cimag(x)
#define CPX_NEW(x, y)	(1.0i*y+x)
#endif

#ifndef MATH2_SINTAB
#define MATH2_SINTAB
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
#endif


double				cabs(double _Complex a)
	{ return(sqrt((CPX_REAL(a)*CPX_REAL(a))+(CPX_IMAG(a)*CPX_IMAG(a)))); }
double				carg(double _Complex a)
	{ return(atan2(CPX_IMAG(a), CPX_REAL(a))); }

#ifdef __BGBCC__
double				cimag(double _Complex a)
	{ return(CPX_IMAG(a)); }
double				creal(double _Complex a)
	{ return(CPX_REAL(a)); }
#else
double				cimag(double _Complex a)
{
	double *fv;
	fv=(double *)(&a);
	return(fv[1]);
}
double				creal(double _Complex a)
{
	double *fv;
	fv=(double *)(&a);
	return(fv[0]);
}
#endif

double _Complex	casin(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(asin(CPX_REAL(a)));
	return(1.0i*clog(csqrt(1.0-a*a)-(a*1.0i)));
}

double _Complex	cacos(double _Complex a)
{
	return(M_PI_2-casin(a));
}

double _Complex	cacosh(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(acosh(CPX_REAL(a)));
	return(clog(a+csqrt(a*a-1.0)));
}

double _Complex	casinh(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(asinh(CPX_REAL(a)));
	return(clog(a+csqrt(a*a+1.0)));
}

double _Complex	catan(double _Complex a)
{
	double _Complex az;
	if(CPX_IMAG(a)==0.0)
		return(atan(CPX_REAL(a)));
	az=a*1.0i;
	return(0.5i*clog((1.0+az)/(1.0-az)));
}

double _Complex	catanh(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(atanh(CPX_REAL(a)));
	return(0.5*clog((1.0+a)/(1.0-a)));
}

double _Complex	ccosh(double _Complex a)
{
	if(CPX_IMAG(a)==0)
		return(cosh(CPX_REAL(a)));
	return((cexp(a)+cexp(-a))*0.5);
}

double _Complex	cexp(double _Complex a)
{
	double ex, cy, sy;
	ex=exp(CPX_REAL(a));
	cy=cos(CPX_IMAG(a));
	sy=sin(CPX_IMAG(a));
//	return((ex*cy)+((ex*sy)*1i));
	return( CPX_NEW(ex*cy, ex*sy) );
}

double _Complex	clog(double _Complex a)
//	{ return(log(CPX_REAL(a))+(carg(a)*1i)); }
	{ return( CPX_NEW(log(CPX_REAL(a)), atan2(CPX_IMAG(a), CPX_REAL(a))) ); }

double _Complex	conj(double _Complex a)
//	{ return(CPX_REAL(a)-(CPX_IMAG(a)*1i)); }
	{ return( CPX_NEW(CPX_REAL(a), -CPX_IMAG(a)) ); }

double _Complex	cpow(double _Complex a, double _Complex b)
	{ return(cexp(clog(a)*b)); }

double _Complex	cproj(double _Complex a)
{
	if(isinf(CPX_REAL(a)))
		return( CPX_NEW(0.0, CPX_IMAG(a)) );
	return(a);
}
	
double _Complex	csin(double _Complex a)
{
	double _Complex t, x, th, th2;
	double f;
	long li;

	if(CPX_IMAG(a)==0.0)
		return(sin(CPX_REAL(a)));

	li=CPX_REAL(a)*M_TAU_R;
	f=(li*M_TAU);
	th=a-f;

	th2=th*th;
	t =th*sintab_c00; x=th*th2;
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

double _Complex	ccos(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(cos(CPX_REAL(a)));
	return(csin(a+M_PI_2));
}

double _Complex	csinh(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(sinh(CPX_REAL(a)));
	return((cexp(a)-cexp(-a))*0.5);
}


double _Complex	csqrt(double _Complex a)
{
	double r, i, g, d, n;
	
	r=CPX_REAL(a);
	i=CPX_IMAG(a);
	if(i!=0.0)
	{
		n=sqrt(r*r+i*i);
		g=sqrt((n+r)/2.0);
		d=sqrt((n-r)/2.0);
		if(i<0)d=-d;
	}else
	{
		g=sqrt(r);
		d=0.0;
	}
//	return(g+(d*1i));
	return( CPX_NEW(g, d) );
}

double _Complex	ctan(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(tan(CPX_REAL(a)));
	return(csin(a)/ccos(a));
}

double _Complex	ctanh(double _Complex a)
{
	if(CPX_IMAG(a)==0.0)
		return(tanh(CPX_REAL(a)));
	return(csinh(a)/ccosh(a));
}


float				cabsf(float _Complex a)
	{ return(sqrt((CPX_REAL(a)*CPX_REAL(a))+(CPX_IMAG(a)*CPX_IMAG(a)))); }
float				cargf(float _Complex a)
	{ return(atan2(CPX_IMAG(a), CPX_REAL(a))); }
float				cimagf(float _Complex a)
	{ return(CPX_IMAG(a)); }
float				crealf(float _Complex a)
	{ return(CPX_REAL(a)); }

float _Complex		cacosf(float _Complex a)
	{ return(cacos(a)); }
float _Complex		cacoshf(float _Complex a)
	{ return(cacosh(a)); }
float _Complex		casinf(float _Complex a)
	{ return(casin(a)); }
float _Complex		casinhf(float _Complex a)
	{ return(casinh(a)); }
float _Complex		catanf(float _Complex a)
	{ return(catan(a)); }
float _Complex		catanhf(float _Complex a)
	{ return(catanh(a)); }
float _Complex		ccosf(float _Complex a)
	{ return(ccos(a)); }
float _Complex		ccoshf(float _Complex a)
	{ return(ccosh(a)); }
float _Complex		cexpf(float _Complex a)
	{ return(cexp(a)); }
float _Complex		clogf(float _Complex a)
	{ return(clog(a)); }
float _Complex		conjf(float _Complex a)
	{ return(conj(a)); }
float _Complex		cpowf(float _Complex a, float _Complex b)
	{ return(cpow(a, b)); }
float _Complex		cprojf(float _Complex a)
	{ return(cproj(a)); }
float _Complex		csinf(float _Complex a)
	{ return(csin(a)); }
float _Complex		csinhf(float _Complex a)
	{ return(csinh(a)); }
float _Complex		csqrtf(float _Complex a)
	{ return(csqrt(a)); }
float _Complex		ctanf(float _Complex a)
	{ return(ctan(a)); }
float _Complex		ctanhf(float _Complex a)
	{ return(ctanh(a)); }


#if 0

long double		cabsl(long double _Complex a);
long double		cargl(long double _Complex a);
long double		cimagl(long double _Complex a);
long double		creall(long double _Complex a);

long double _Complex	cacoshl(long double _Complex a);
long double _Complex	cacosl(long double _Complex a);
long double _Complex	casinhl(long double _Complex a);
long double _Complex	casinl(long double _Complex a);
long double _Complex	catanhl(long double _Complex a);
long double _Complex	catanl(long double _Complex a);
long double _Complex	ccoshl(long double _Complex a);
long double _Complex	ccosl(long double _Complex a);
long double _Complex	cexpl(long double _Complex a);
long double _Complex	clogl(long double _Complex a);
long double _Complex	conjl(long double _Complex a);
long double _Complex	cpowl(long double _Complex a, long double _Complex b);
long double _Complex	cprojl(long double _Complex a);

long double _Complex	csinhl(long double _Complex a);
long double _Complex	csinl(long double _Complex a);
long double _Complex	csqrtl(long double _Complex a);
long double _Complex	ctanhl(long double _Complex a);
long double _Complex	ctanl(long double _Complex a);

#endif
