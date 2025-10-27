/*
Generic Quad-FP.
Should also support generic C (not depending entirely on __int128).

 */

#ifndef _BGBCC

// #if 1

typedef struct tk_sqfp_val128_s tk_sqfp_val128;

struct tk_sqfp_val128_s {
u64 lo;
u64 hi;
};

tk_sqfp_val128 tk_sqfp_get_f128(u64 *rv)
{
	tk_sqfp_val128 vc;
	vc.lo=rv[0];
	vc.hi=rv[1];
	return(vc);
}

void tk_sqfp_set_f128(u64 *rv, tk_sqfp_val128 va)
{
	rv[0]=va.lo;
	rv[1]=va.hi;
}

tk_sqfp_val128 tk_sqfp_get_frac(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.lo=va.lo;
	vc.hi=(va.hi&0x0000FFFFFFFFFFFFULL);
	if((va.hi>>48)&32767)
		vc.hi|=0x0001000000000000ULL;
	return(vc);
}

int tk_sqfp_get_exp(tk_sqfp_val128 va)
{
	return((va.hi>>48)&32767);
}

int tk_sqfp_get_sgn(tk_sqfp_val128 va)
{
	return((va.hi>>63)&1);
}

tk_sqfp_val128 tk_sqfp_shlr(tk_sqfp_val128 va, int shr)
{
	tk_sqfp_val128 vc;
	
	if(!shr)
		return(va);
	
	if(shr>=64)
	{
		if(shr==64)
		{
			vc.lo=va.hi;
			vc.hi=0;
			return(vc);
		}
		vc.lo=va.hi>>(shr-64);
		vc.hi=0;
		return(vc);
	}
	
	vc.lo=(va.lo>>shr)|(va.hi<<(64-shr));
	vc.hi=va.hi>>shr;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_shll(tk_sqfp_val128 va, int shr)
{
	tk_sqfp_val128 vc;
	
	if(!shr)
		return(va);
	
	if(shr>=64)
	{
		if(shr==64)
		{
			vc.hi=va.lo;
			vc.lo=0;
			return(vc);
		}
		vc.hi=va.lo<<(shr-64);
		vc.lo=0;
		return(vc);
	}
	
	vc.hi=(va.hi<<shr)|(va.lo>>(64-shr));
	vc.lo=va.lo<<shr;
	return(vc);
}


tk_sqfp_val128 tk_sqfp_shlr1(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.lo=(va.lo>>1)|(va.hi<<63);
	vc.hi=va.hi>>1;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_shll1(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.hi=(va.hi<<1)|(va.lo>>63);
	vc.lo=va.lo<<1;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_shlr8(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.lo=(va.lo>>8)|(va.hi<<56);
	vc.hi=va.hi>>8;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_shll8(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.hi=(va.hi<<8)|(va.lo>>56);
	vc.lo=va.lo<<8;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_shlr32(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.lo=(va.lo>>32)|(va.hi<<32);
	vc.hi=va.hi>>32;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_shll32(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc.hi=(va.hi<<32)|(va.lo>>32);
	vc.lo=va.lo<<32;
	return(vc);
}


tk_sqfp_val128 tk_sqfp_add_i128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vc;
	int cf;
	
	vc.lo=va.lo+vb.lo;
	cf=vc.lo<va.lo;
	vc.hi=va.hi+vb.hi+cf;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_neg_i128(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	u64 vlo, vlop1;
	int cf;

	vlo=(~va.lo);
	vlop1=vlo+1;
	cf=vlo>vlop1;
	vc.lo=vlop1;
	vc.hi=(~va.hi)+cf;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_wrapu64lo_i128(u64 va)
{
	tk_sqfp_val128 vc;
	vc.lo=va;
	vc.hi=0;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_wrapu64hi_i128(u64 va)
{
	tk_sqfp_val128 vc;
	vc.lo=0;
	vc.hi=va;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_wrapu64mi_i128(u64 va)
{
	tk_sqfp_val128 vc;
	vc.lo=va<<32;
	vc.hi=va>>32;
	return(vc);
}

tk_sqfp_val128 tk_sqfp_mulhi_i128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vc;
	u64 v_aa, v_ab, v_ac, v_ad;
	u64 v_ba, v_bb, v_bc;
	u64 v_ca, v_cb, v_da;
	u64 va_a, va_b, va_c, va_d;
	u64 vb_a, vb_b, vb_c, vb_d;

	va_a=(u32)(va.hi>>32);	va_b=(u32)(va.hi>> 0);
	va_c=(u32)(va.lo>>32);	va_d=(u32)(va.lo>> 0);
	vb_a=(u32)(vb.hi>>32);	vb_b=(u32)(vb.hi>> 0);
	vb_c=(u32)(vb.lo>>32);	vb_d=(u32)(vb.lo>> 0);

	v_aa=va_a*vb_a;
	v_ab=va_a*vb_b;
	v_ac=va_a*vb_c;
	v_ad=va_a*vb_d;

	v_ba=va_b*vb_a;
	v_bb=va_b*vb_b;
	v_bc=va_b*vb_c;

	v_ca=va_c*vb_a;
	v_cb=va_c*vb_b;

	v_da=va_d*vb_a;

	vc=
		tk_sqfp_add_i128(
			tk_sqfp_add_i128(
				tk_sqfp_add_i128(
					tk_sqfp_wrapu64hi_i128(v_aa),
					tk_sqfp_wrapu64lo_i128(v_bb)),
				tk_sqfp_add_i128(
					tk_sqfp_add_i128(
						tk_sqfp_wrapu64mi_i128(v_ab),
						tk_sqfp_wrapu64mi_i128(v_ba)),
					tk_sqfp_add_i128(
						tk_sqfp_wrapu64lo_i128(v_ac),
						tk_sqfp_wrapu64lo_i128(v_ca)))),
			tk_sqfp_add_i128(
				tk_sqfp_add_i128(
					tk_sqfp_wrapu64lo_i128(v_ad>>32),
					tk_sqfp_wrapu64lo_i128(v_da>>32)),
				tk_sqfp_add_i128(
					tk_sqfp_wrapu64lo_i128(v_bc>>32),
					tk_sqfp_wrapu64lo_i128(v_cb>>32))));
	
	return(vc);
}

static const tk_sqfp_val128 tk_sqfp_f128_const_0 =
	{ 0x0000000000000000ULL, 0x0000000000000000ULL };
static const tk_sqfp_val128 tk_sqfp_f128_const_1 =
	{ 0x0000000000000000ULL, 0x3FFF000000000000ULL };
static const tk_sqfp_val128 tk_sqfp_f128_const_2 =
	{ 0x0000000000000000ULL, 0x4000000000000000ULL };

#define tk_sqfp_i128_iszero(va)	(((va.hi)==0) && ((va.lo)==0))

#define	tk_sqfp_hi16(va)		((int)((va.hi)>>48))
#define	tk_sqfp_hi24(va)		((int)((va.hi)>>40))
#define	tk_sqfp_hi32(va)		((u32)((va.hi)>>32))
#define	tk_sqfp_hi48(va)		((u32)((va.hi)>>16))

int tk_sqfp_i128_isgt(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	if(va.hi>vb.hi)
		return(1);
	if(va.hi!=vb.hi)
		return(0);
	if(va.lo>vb.lo)
		return(1);
	return(0);
}

int tk_sqfp_i128_iseq(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	if(va.hi!=vb.hi)
		return(0);
	if(va.lo!=vb.lo)
		return(0);
	return(1);
}

#else

#define SQFP_ISINT128

typedef unsigned __int128 tk_sqfp_val128;

// #define tk_sqfp_get_f128(rv)		(*(unsigned __int128 **)(rv))
// #define tk_sqfp_set_f128(rv, va)	(*(unsigned __int128 **)(rv)=(va))

tk_sqfp_val128 tk_sqfp_get_f128(u64 *rv)
{
	tk_sqfp_val128 vc;
	vc=0;
	memcpy(&vc, rv, 16);
	return(vc);
}

void tk_sqfp_set_f128(u64 *rv, tk_sqfp_val128 va)
	{ memcpy(rv, &va, 16); }

#define	tk_sqfp_get_sgn(va)		(((int)((va)>>127))&1)
#define	tk_sqfp_get_exp(va)		(((int)((va)>>112))&32767)

#define	tk_sqfp_shlr(va, shr)	((va)>>(shr))
#define	tk_sqfp_shll(va, shr)	((va)<<(shr))
#define	tk_sqfp_shlr1(va)		((va)>>1)
#define	tk_sqfp_shll1(va)		((va)<<1)
#define	tk_sqfp_shlr8(va)		((va)>>8)
#define	tk_sqfp_shll8(va)		((va)<<8)
#define	tk_sqfp_shlr32(va)		((va)>>32)
#define	tk_sqfp_shll32(va)		((va)<<32)

tk_sqfp_val128 tk_sqfp_get_frac(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	vc=(va&0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFUI128);
	if(tk_sqfp_get_exp(va))
		vc|=0x00010000000000000000000000000000UI128;
	return(vc);
}

#define tk_sqfp_add_i128(va, vb)	((va)+(vb))
#define tk_sqfp_sub_i128(va, vb)	((va)-(vb))
#define tk_sqfp_neg_i128(va)		(0UI128-(va))

#define tk_sqfp_wrapu64lo_i128(va)	((tk_sqfp_val128)(va))
#define tk_sqfp_wrapu64hi_i128(va)	(((tk_sqfp_val128)(va))<<64)
#define tk_sqfp_wrapu64mi_i128(va)	(((tk_sqfp_val128)(va))<<32)

tk_sqfp_val128 tk_sqfp_mulhi_i128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vc;
	u64 v_aa, v_ab, v_ac, v_ad;
	u64 v_ba, v_bb, v_bc;
	u64 v_ca, v_cb, v_da;
	u64 va_a, va_b, va_c, va_d;
	u64 vb_a, vb_b, vb_c, vb_d;
	
	va_a=(u32)(va>>96);	va_b=(u32)(va>>64);
	va_c=(u32)(va>>32);	va_d=(u32)(va>> 0);
	vb_a=(u32)(vb>>96);	vb_b=(u32)(vb>>64);
	vb_c=(u32)(vb>>32);	vb_d=(u32)(vb>> 0);

#ifdef __BJX2__
	v_aa=__int32_dmulu(va_a, vb_a);
	v_ab=__int32_dmulu(va_a, vb_b);
	v_ac=__int32_dmulu(va_a, vb_c);
	v_ad=__int32_dmulu(va_a, vb_d);

	v_ba=__int32_dmulu(va_b, vb_a);
	v_bb=__int32_dmulu(va_b, vb_b);
	v_bc=__int32_dmulu(va_b, vb_c);

	v_ca=__int32_dmulu(va_c, vb_a);
	v_cb=__int32_dmulu(va_c, vb_b);

	v_da=__int32_dmulu(va_d, vb_a);
#else
	v_aa=va_a*vb_a;
	v_ab=va_a*vb_b;
	v_ac=va_a*vb_c;
	v_ad=va_a*vb_d;

	v_ba=va_b*vb_a;
	v_bb=va_b*vb_b;
	v_bc=va_b*vb_c;

	v_ca=va_c*vb_a;
	v_cb=va_c*vb_b;

	v_da=va_d*vb_a;
#endif

	vc=
		(((tk_sqfp_val128)v_aa)<<64)+
		(((tk_sqfp_val128)v_ab)<<32)+
		(((tk_sqfp_val128)v_ba)<<32)+
		v_bb+
		v_ac+
		v_ca+
		(v_ad>>32)+
		(v_da>>32)+
		(v_bc>>32)+
		(v_cb>>32);
	
	return(vc);
}


#define tk_sqfp_f128_const_0		0x00000000000000000000000000000000UI128
#define tk_sqfp_f128_const_1		0x3FFF0000000000000000000000000000UI128
#define tk_sqfp_f128_const_2		0x40000000000000000000000000000000UI128
#define tk_sqfp_f128_const_inf		0x7FFF0000000000000000000000000000UI128

#define tk_sqfp_i128_iszero(va)	((va)==0)

#define	tk_sqfp_hi16(va)		((int)((va)>>112))
#define	tk_sqfp_hi24(va)		((int)((va)>>104))
#define	tk_sqfp_hi32(va)		((u32)((va)>> 96))
#define	tk_sqfp_hi48(va)		((u64)((va)>> 80))

#define tk_sqfp_i128_isgt(va, vb)	((va)>(vb))
#define tk_sqfp_i128_iseq(va, vb)	((va)==(vb))


#endif

tk_sqfp_val128 tk_sqfp_repack_f128(tk_sqfp_val128 frc, int exc, int sgc)
{
	tk_sqfp_val128 vc;

	if(tk_sqfp_i128_iszero(frc))
		return(tk_sqfp_f128_const_0);

	if(tk_sqfp_get_sgn(frc))
	{
		sgc=!sgc;
		frc=tk_sqfp_neg_i128(frc);
	}
	
	while(tk_sqfp_hi16(frc)>1)
		{ exc++; frc=tk_sqfp_shlr1(frc); }

	while(!tk_sqfp_hi48(frc))
		{ exc-=32; frc=tk_sqfp_shll32(frc); }
	while(!tk_sqfp_hi24(frc))
		{ exc-=8; frc=tk_sqfp_shll8(frc); }
	while(!tk_sqfp_hi16(frc))
		{ exc--; frc=tk_sqfp_shll1(frc); }
	
	if(exc>=32767)
	{
#ifdef SQFP_ISINT128
		vc=tk_sqfp_f128_const_inf|(((tk_sqfp_val128)sgc)<<127);
#else
		vc.hi=(32767ULL<<48)|(((u64)sgc)<<63);
		vc.lo=0;
#endif
		return(vc);
	}

	if(exc<=0)
	{
		if(exc<=(-111))
			return(tk_sqfp_f128_const_0);
		
		frc=tk_sqfp_shlr(frc, -exc);

#ifdef SQFP_ISINT128
		vc=frc|(((tk_sqfp_val128)sgc)<<127);
#else
		vc.lo=frc.lo;
		vc.hi=(frc.hi&0x0000FFFFFFFFFFFFULL)|
			(((u64)sgc)<<63);
#endif
		return(vc);
	}
	
#ifdef SQFP_ISINT128
	vc=frc |
		(((tk_sqfp_val128)exc)<<112) |
		(((tk_sqfp_val128)sgc)<<127) ;
#else
	vc.lo=frc.lo;
	vc.hi=(frc.hi&0x0000FFFFFFFFFFFFULL)|
		(((u64)exc)<<48)|(((u64)sgc)<<63);
#endif
	return(vc);
}

tk_sqfp_val128 tk_sqfp_fadd_f128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vc, fra, frb, frc;
	int exa, exb, exc, sga, sgb, sgc;
	int exd;

	fra=tk_sqfp_get_frac(va);
	frb=tk_sqfp_get_frac(vb);
	exa=tk_sqfp_get_exp(va);
	exb=tk_sqfp_get_exp(vb);
	sga=tk_sqfp_get_sgn(va);
	sgb=tk_sqfp_get_sgn(vb);
	
	if((exb>exa) || ((exb==exa) && tk_sqfp_i128_isgt(frb, fra)))
	{
		vc=va; va=vb; vb=vc;
		frc=fra; fra=frb; frb=frc;
		exc=exa; exa=exb; exb=exc;
		sgc=sga; sga=sgb; sgb=sgc;
	}
	
	exd=exa-exb;
	if(exd>=112)
		return(va);
	
	frb=tk_sqfp_shlr(frb, exd);
	
	if(sga!=sgb)
		{ frb=tk_sqfp_neg_i128(frb); }
	
	exc=exa; sgc=sga;
	frc=tk_sqfp_add_i128(fra, frb);

	vc=tk_sqfp_repack_f128(frc, exc, sgc);
	return(vc);
}

tk_sqfp_val128 tk_sqfp_fsub_f128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vc, vb1;
	vb1=vb;
#ifdef SQFP_ISINT128
	vb1^=1UI128<<127;
#else
	vb1.hi^=1ULL<<63;
#endif
	vc=tk_sqfp_fadd_f128(va, vb1);
}

tk_sqfp_val128 tk_sqfp_fmul_f128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vc, fra, frb, frc;
	int exa, exb, exc, sga, sgb, sgc;
	int exd;

	fra=tk_sqfp_get_frac(va);
	frb=tk_sqfp_get_frac(vb);
	exa=tk_sqfp_get_exp(va);
	exb=tk_sqfp_get_exp(vb);
	sga=tk_sqfp_get_sgn(va);
	sgb=tk_sqfp_get_sgn(vb);

	sgc=sga^sgb;
	exc=(exa+exb)-16383;

	fra=tk_sqfp_shll8(fra);
	frb=tk_sqfp_shll8(frb);
	frc=tk_sqfp_mulhi_i128(fra, frb);

	vc=tk_sqfp_repack_f128(frc, exc, sgc);
	return(vc);
}

tk_sqfp_val128 tk_sqfp_frcpa_f128(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	int sga;

	sga=tk_sqfp_get_sgn(va);

#ifdef SQFP_ISINT128
	vc=~va;
	vc+=0x7FFE0000000000000000000000000000UI128;
	vc&=0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFUI128;
	vc|=((tk_sqfp_val128)sga)<<127;
#else
	vc.hi=~va.hi;
	vc.lo=~va.lo;
	vc.hi+=0x7FFE000000000000ULL;
	vc.hi&=0x7FFFFFFFFFFFFFFFULL;
	vc.hi|=((u64)sga)<<63;
#endif

	return(vc);
}

tk_sqfp_val128 tk_sqfp_fsqrta_f128(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vc;
	int sga;

	sga=tk_sqfp_get_sgn(va);
	
#ifdef SQFP_ISINT128
	vc=va&0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFUI128;
	vc=vc>>1;
	vc+=0x1FFF8000000000000000000000000000UI128;
	vc|=((tk_sqfp_val128)sga)<<127;
#else
	vc.hi=va.hi&0x7FFFFFFFFFFFFFFFULL;
	vc.lo=va.lo;
//	vc=tk_sqfp_shlr(vc, 1);
	vc=tk_sqfp_shlr1(vc);
	vc.hi+=0x1FFF800000000000ULL;
	vc.hi|=((u64)sga)<<63;
#endif

	return(vc);
}

tk_sqfp_val128 tk_sqfp_frcp_f128(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vq, vt, vd, vk1, vk2;
	int i;
	
	vk2=tk_sqfp_f128_const_2;

	vq=tk_sqfp_frcpa_f128(va);
	for(i=0; i<8; i++)
	{
		vt=tk_sqfp_fmul_f128(vq, va);
		vd=tk_sqfp_fsub_f128(vk2, vt);
		vq=tk_sqfp_fmul_f128(vq, vd);
	}
	return(vq);
}

tk_sqfp_val128 tk_sqfp_fdiv_f128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	tk_sqfp_val128 vq, vr;
	vr=tk_sqfp_frcp_f128(vb);
	vq=tk_sqfp_fmul_f128(va, vr);
	return(vq);
}

tk_sqfp_val128 tk_sqfp_fsqrt_f128(tk_sqfp_val128 va)
{
	tk_sqfp_val128 vq, vt, vd, vk;
	int i;

	vk=tk_sqfp_fadd_f128(va, tk_sqfp_f128_const_1);
	vq=tk_sqfp_fsqrta_f128(va);
	for(i=0; i<8; i++)
	{
		vt=tk_sqfp_fmul_f128(vq, vq);
		vd=tk_sqfp_fsub_f128(vk, vt);
		vq=tk_sqfp_fmul_f128(vq, vd);
	}
	return(vq);
}

int tk_sqfp_fcmp_f128(tk_sqfp_val128 va, tk_sqfp_val128 vb)
{
	int gt, lt, eq, sga, sgb, rv;

	sga=tk_sqfp_get_sgn(va);
	sgb=tk_sqfp_get_sgn(vb);
	
	if(sga==sgb)
	{
		gt=tk_sqfp_i128_isgt(va, vb);
		eq=tk_sqfp_i128_iseq(va, vb);
		lt=!gt && !eq;
		
		if(eq)
		{
			if(tk_sqfp_get_exp(va)==32767)
				return(2);
		}
	}else
	{
		gt=0; eq=0; lt=0;
	}
	
	rv=0;
	if(sga)
	{
		if(sgb)
		{
			if(gt)	rv=-1;
			if(lt)	rv= 1;
		}else
			{ rv=-1; }
	}else
	{
		if(!sgb)
		{
			if(gt)	rv=-1;
			if(lt)	rv= 1;
		}
		else
			{ rv=1; }
	}
	return(rv);
}

tk_sqfp_val128 tk_sqfp_fcvt_f64_to_f128(u64 va)
{
	tk_sqfp_val128 vc;
	int sga, exa;

	sga=va>>63;
	exa=(va>>52)&2047;
	if(!exa)
		return(tk_sqfp_f128_const_0);

#ifdef SQFP_ISINT128
	vc=	((va&0x7FFFFFFFFFFFFFFFUI128)<<60)+
		(((tk_sqfp_val128)(16383-1023))<<112);
	vc|=((tk_sqfp_val128)sga)<<127;
#else
	vc.hi=(va>>4)+((16383LL-1023LL)<<48)+(((u64)sga)<<63);
	vc.lo=va<<60;
#endif
	return(vc);
}

u64 tk_sqfp_fcvt_f128_to_f64(tk_sqfp_val128 va)
{
	u64 vc;
	int sga, exa, exa1;

	sga=tk_sqfp_get_sgn(va);
	exa=tk_sqfp_get_exp(va);
	exa1=exa-(16383-1023);
	if(exa1<=0)
		return(0);
	if(exa1>=2047)
	{
		vc=0x7FF0000000000000ULL|(((u64)sga)<<63);
		return(vc);
	}

#ifdef SQFP_ISINT128
	vc=(va>>60)-((16383UI128-1023UI128)<<52);
	vc|=((u64)sga)<<63;
#else
	vc=(va.hi-((16383ULL-1023ULL)<<48))<<4;
	vc|=va.lo>>60;
#endif
	return(vc);
}

tk_sqfp_val128 tk_sqfp_fcvt_i64_to_f128(u64 va, int issg)
{
	tk_sqfp_val128 vc, frc;
	int sga, exa;

	if(!va)
		return(tk_sqfp_f128_const_0);

#ifdef SQFP_ISINT128
	exa=16383+112;
	frc=va; sga=0;
	if(issg && (va>>63))
		{ frc=(~va)+1; sga=1; }
#else
	sga=0; exa=16383+112;
	frc.hi=0;	frc.lo=va;
	if(issg && (va>>63))
		{ frc.lo=(~va)+1; sga=1; }
#endif

	vc=tk_sqfp_repack_f128(frc, exa, sga);
	return(vc);
}

u64 tk_sqfp_fcvt_f128_to_i64(tk_sqfp_val128 va)
{
	tk_sqfp_val128 fra;
	u64 vc;
	int sga, exa, exa1;

	sga=tk_sqfp_get_sgn(va);
	exa=tk_sqfp_get_exp(va);
	fra=tk_sqfp_get_frac(va);
	exa1=exa-16383;

	if(exa1<=0)
	{
		vc=1;
		if(exa1<0)	vc=0;
		if(sga)		vc=(~vc)+1;
		return(vc);
	}

	if(exa1>=64)
	{
		vc=0x8000000000000000ULL;
		return(vc);
	}

#ifdef SQFP_ISINT128
	vc=fra>>(112-exa1);
#else
	fra=tk_sqfp_shlr(fra, 112-exa1);
	vc=fra.lo;
#endif

	if(sga)
		vc=(~vc)+1;
	return(vc);
}



tk_sqfp_val128 tk_sqfp_fcvt_i128_to_f128(tk_sqfp_val128 va, int issg)
{
	tk_sqfp_val128 vc, frc;
	int sga, exa;

	if(tk_sqfp_int128_iszero(va))
		return(tk_sqfp_f128_const_0);

#ifdef SQFP_ISINT128
	exa=16383+112;
	frc=va; sga=0;
	if(issg && ((va>>127)!=0))
		{ frc=(~va)+1; sga=1; }
#else
	sga=0; exa=16383+112;
	frc.hi=va.hi;	frc.lo=va.lo;
	if(issg && (va.hi>>63))
		{ frc=tk_sqfp_neg_i128(frc); sga=1; }
#endif

	while(tk_sqfp_hi16(frc)>1)
		{ exa++; frc=tk_sqfp_shlr1(frc); }

	vc=tk_sqfp_repack_f128(frc, exa, sga);
	return(vc);
}

tk_sqfp_val128 tk_sqfp_fcvt_f128_to_i128(tk_sqfp_val128 va)
{
	tk_sqfp_val128 fra;
	tk_sqfp_val128 vc;
	int sga, exa, exa1;

	sga=tk_sqfp_get_sgn(va);
	exa=tk_sqfp_get_exp(va);
	fra=tk_sqfp_get_frac(va);
	exa1=exa-16383;

	if(exa1<=0)
	{
		vc=tk_sqfp_wrapu64lo_i128(1);
		if(exa1<0)	vc=tk_sqfp_wrapu64lo_i128(0);
		if(sga)		vc=tk_sqfp_neg_i128(vc);
		return(vc);
	}

	if(exa1>=128)
	{
		vc=tk_sqfp_wrapu64hi_i128(0x8000000000000000ULL);
		return(vc);
	}

#ifdef SQFP_ISINT128
	vc=fra>>(112-exa1);
#else
	fra=tk_sqfp_shlr(fra, 112-exa1);
	vc=fra.lo;
#endif

	if(sga)		vc=tk_sqfp_neg_i128(vc);
	return(vc);
}


void __sqfp_fadd_f128fv(u64 *rf0, u64 *rf1, u64 *rf2)
{
	tk_sqfp_val128 va, vb, vc;
	va=tk_sqfp_get_f128(rf0);
	vb=tk_sqfp_get_f128(rf1);
	vc=tk_sqfp_fadd_f128(va, vb);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fsub_f128fv(u64 *rf0, u64 *rf1, u64 *rf2)
{
	tk_sqfp_val128 va, vb, vc;
	va=tk_sqfp_get_f128(rf0);
	vb=tk_sqfp_get_f128(rf1);
	vc=tk_sqfp_fsub_f128(va, vb);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fmul_f128fv(u64 *rf0, u64 *rf1, u64 *rf2)
{
	tk_sqfp_val128 va, vb, vc;
	va=tk_sqfp_get_f128(rf0);
	vb=tk_sqfp_get_f128(rf1);
	vc=tk_sqfp_fmul_f128(va, vb);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fdiv_f128fv(u64 *rf0, u64 *rf1, u64 *rf2)
{
	tk_sqfp_val128 va, vb, vc;
	va=tk_sqfp_get_f128(rf0);
	vb=tk_sqfp_get_f128(rf1);
	vc=tk_sqfp_fdiv_f128(va, vb);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fsqrt_f128fv(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va, vb, vc;
	va=tk_sqfp_get_f128(rf0);
	vc=tk_sqfp_fsqrt_f128(va);
	tk_sqfp_set_f128(rf2, vc);
}


int __sqfp_fcmp_f128fv(u64 *rf0, u64 *rf1)
{
	tk_sqfp_val128 va, vb, vc;
	int vi;
	va=tk_sqfp_get_f128(rf0);
	vb=tk_sqfp_get_f128(rf1);
	vi=tk_sqfp_fcmp_f128(va, vb);
	return(vi);
}

void __sqfp_fcvt_d2q(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 vc;
	vc=tk_sqfp_fcvt_f64_to_f128(rf0[0]);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fcvt_q2d(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va;
	va=tk_sqfp_get_f128(rf0);
	rf2[0]=tk_sqfp_fcvt_f128_to_f64(va);
}


void __sqfp_fcvt_s2q(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 vc;
	u64 va;
	
	va=__sfp_fcnvsd(rf0[0]);
	vc=tk_sqfp_fcvt_f64_to_f128(va);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fcvt_q2s(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va;
	u64 vc;
	va=tk_sqfp_get_f128(rf0);
	vc=tk_sqfp_fcvt_f128_to_f64(va);
	vc=__sfp_fcnvds(vc);
	rf2[0]=vc;
}


void __sqfp_fcvt_i2q(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 vc;
	vc=tk_sqfp_fcvt_i64_to_f128(rf0[0], 1);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fcvt_ui2q(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 vc;
	vc=tk_sqfp_fcvt_i64_to_f128(rf0[0], 0);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fcvt_q2i(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va;
	va=tk_sqfp_get_f128(rf0);
	rf2[0]=tk_sqfp_fcvt_f128_to_i64(va);
}


void __sqfp_fcvt_xi2q(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va, vc;
	va=tk_sqfp_get_f128(rf0);
	vc=tk_sqfp_fcvt_i128_to_f128(vc, 1);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fcvt_xiu2q(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va, vc;
	va=tk_sqfp_get_f128(rf0);
	vc=tk_sqfp_fcvt_i128_to_f128(vc, 0);
	tk_sqfp_set_f128(rf2, vc);
}

void __sqfp_fcvt_q2xi(u64 *rf0, u64 *rf2)
{
	tk_sqfp_val128 va, vc;
	va=tk_sqfp_get_f128(rf0);
	vc=tk_sqfp_fcvt_f128_to_i128(va);
	tk_sqfp_set_f128(rf2, vc);
}
