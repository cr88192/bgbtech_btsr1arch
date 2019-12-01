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


double sin(double ang)
{
	double t, x, th, th2;
	int i;

    i=ang*M_TAU_R;
    th=ang-(i*M_TAU);

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

double cos(double ang)
{
	return(sin(ang+M_PI_2));
}
