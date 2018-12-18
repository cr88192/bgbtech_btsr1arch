/*
BJX2 FPU Support
Implements some functionality which may be absent in the native HW FPU.
 */

#if 1
u64 __sfp_frcpa_f64(u64 f0)
{
	u64 f1, f2;
	int i;
	
	i=(f0>>52)&2047;
	if((i<2) || (i>2044))
		return(0);
	
//	f1=(f0&0x8000000000000000ULL)|
//		(0x7FF0000000000000ULL-
//			(f0&0x7FFFFFFFFFFFFFFFULL));
	f1=0x7FF0000000000000ULL-f0;
	f1-=((u64)(sfp_rcp_adj[(byte)(f0>>44)]))<<34;

	return(f1);
}
#endif

double __fpu_frcpa(double a)
{
	u64 t;
	t=__sfp_frcpa_f64(__float64_getbits(a));
	return(__float64_frombits(t));
}

#if 0
double __fpu_frcp_b1(double a);
double __fpu_frcp_b2(double a);
double __fpu_frcp_b3(double a);
double __fpu_frcp_b4(double a);

double __fpu_frcp(double a)
{
	double rv, rp;
	u64 rpb, rpa, rvb;
	int i;
	
//	if(a<0)
//		{ return(-__fpu_frcp(-a)); }
	return(__fpu_frcp_b4(a));

#if 0
	rv=__fpu_frcpa(a);

//	rv=__fpu_frcp_b1(rv, a);

	rv=__fpu_frcp_b3(rv, a);
	
#if 0
//	rpb=__float64_getbits(rp);
	rvb=__float64_getbits(rv);
	for(i=0; i<12; i++)
	{
		rv=__float64_frombits(rvb);
		rp=rv*a;
		rpb=__float64_getbits(rp);
		rpa=0x3FF0000000000000ULL-rpb;
		rvb=rvb+rpa;
	}
	rv=__float64_frombits(rvb);
#endif

#if 0
	rv=__fpu_frcp_b3(rv, a);
//	rv=rv*rv;
	rv = rv*(2.0 - a*rv);
	rv = rv*(2.0 - a*rv);
	rv = rv*(2.0 - a*rv);
	rv = rv*(2.0 - a*rv);
	rv = rv*(2.0 - a*rv);
#endif

	return(rv);
#endif
}
#endif

double __fpu_frcp(double a);

double __fpu_fdiv(double x, double y)
{
	return(x*__fpu_frcp(y));
}
