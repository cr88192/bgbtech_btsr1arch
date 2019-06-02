uint32_t __sfp_float_f32(int32_t v);
uint32_t __sfp_fmul_f32(uint32_t va, uint32_t vb);

uint32_t __sfp_neg_f32(uint32_t v)
{
	return(v^0x80000000U);
}

uint32_t tkcl_sfp_ufloat_f32(uint32_t v)
{
	uint32_t tv, e;

	if(v&0xFF000000U)
	{
		e=127+23; tv=v;
		while(tv&0xFF000000U)
			{ e++; tv=tv>>1; }
		tv=(tv&0x007FFFFFU)|(e<<23);
		return(tv);
	}else if(v)
	{
		e=127+23; tv=v;
		while(!(tv&0x00800000U))
			{ e--; tv=tv<<1; }
		tv=(tv&0x007FFFFFU)|(e<<23);
		return(tv);
	}
	return(0);
}

uint32_t tkcl_sfp_rcpa_f32(uint32_t v)
{
	uint32_t tv;

	tv=0x7F800000U-v;
	return(tv);
}

uint32_t tkcl_sfp_rcp_f32(uint32_t v)
{
	uint32_t tv, tv1;
	int i;

	if(v&0x80000000U)
	{
//		tv=__sfp_neg_f32(v);
		tv=v^0x80000000U;
		tv=tkcl_sfp_rcp_f32(tv);
//		tv=__sfp_neg_f32(tv);
		tv=tv^0x80000000U;
		return(tv);
	}

	tv=tkcl_sfp_rcpa_f32(v);
	
//	for(i=0; i<16; i++)
	for(i=0; i<12; i++)
	{
		tv1=__sfp_fmul_f32(v, tv);
//		tv-=(tv1-0x3F800000U)>>1;
		tv-=(tv1-0x3F800000U);
	}
	
	return(tv);
}

uint32_t __sfp_float_f32(int32_t v)
{
	uint32_t tv;

	if(v<0)
	{
		tv=tkcl_sfp_ufloat_f32(-v);
		return(__sfp_neg_f32(tv));
	}else if(v)
	{
		tv=tkcl_sfp_ufloat_f32(v);
		return(tv);
	}else
	{
		return(0);
	}
}

uint32_t __sfp_fadd_f32(uint32_t va, uint32_t vb)
{
	uint32_t fa, fb, fc, tv, tm;
	int ea, eb, ec;
	
	if((va^vb)&0x80000000U)
	{
		tm=0x7FFFFFFFU;
		if((vb&tm)>(va&tm))
			{ fc=va; va=vb; vb=fc; }
		fa=(va&0x007FFFFF)|0x00800000;
		fb=(vb&0x007FFFFF)|0x00800000;
		ea=(va>>23)&255;
		eb=(vb>>23)&255;
		fb=fb>>(ea-eb);
		fc=fa-fb;
		ec=ea;
		
//		if(!fc)
		if(!(fc&0x00FFFFFFU))
			return(0);

//		while(fc && !(fc&0x00800000U))
		while(!(fc&0x00800000U))
			{ ec--; fc=fc<<1; }
		tv=(fc&0x007FFFFFU)|(ec<<23)|
			(va&0x80000000U);
		return(tv);
	}else
	{
		if(vb>va)
			{ fc=va; va=vb; vb=fc; }
		fa=(va&0x007FFFFF)|0x00800000;
		fb=(vb&0x007FFFFF)|0x00800000;
		ea=(va>>23)&255;
		eb=(vb>>23)&255;
		fb=fb>>(ea-eb);
		fc=fa+fb;
		ec=ea;

		while(fc&0xFF000000U)
			{ ec++; fc=fc>>1; }
		tv=(fc&0x007FFFFFU)|(ec<<23)|
			(va&0x80000000U);
		return(tv);
	}
}

uint32_t __sfp_fsub_f32(uint32_t va, uint32_t vb)
{
	return(__sfp_fadd_f32(va, __sfp_neg_f32(vb)));
}

uint32_t __sfp_fmul_f32(uint32_t va, uint32_t vb)
{
	uint32_t fa, fb, fc, tv, tm;
	uint64_t fca;
	int ea, eb, ec;
	
	fa=(va&0x007FFFFF)|0x00800000;
	fb=(vb&0x007FFFFF)|0x00800000;
	ea=(va>>23)&255;
	eb=(vb>>23)&255;
	
	ec=ea+eb-127;
//	__debugbreak();

	fca=((uint64_t)fa)*((uint64_t)fb);
//	__debugbreak();
	fc=(fca+0x003FFFFF)>>23;
//	__debugbreak();
//	fc=(fa>>12)*(fb>>11);
	while(fc&0xFF000000U)
		{ ec++; fc=fc>>1; }
	tv=(fc&0x007FFFFFU)|(ec<<23)|
		((va^vb)&0x80000000U);
//	__debugbreak();
	return(tv);
}

uint32_t __sfp_fdiv_f32(uint32_t va, uint32_t vb)
{
	return(__sfp_fmul_f32(va, tkcl_sfp_rcp_f32(vb)));
}




int32_t __sfp_ftrc_f32(uint32_t f0)
{
	int32_t t;
	uint32_t f2;
	int e0, e1, e2;
	int m0, m1, m2;
	
	if(!f0)
		return(0);
	
	e0=(f0>>23)&255;
	m0=0x00800000|(f0&0x00FFFFFF);

//	if(f0&0x80000000)
//		m0=~m0;
	
	e1=(e0-127)-23;
	if(e1>=0)
	{
		t=m0<<e1;
	}else
	{
		e1=-e1;
		if(e1>24)
		{
			t=0;
		}else
		{
			t=m0>>e1;
		}
	}
	if(f0&0x80000000)
		t=-t;
	return(t);
}

uint32_t __sfp_ftrc_f64(uint64_t f0)
{
	int32_t t;
	uint32_t f2;
	int e0, e1, e2;
//	uint32_t m0, m1, m2;
	int64_t m0, m1, m2;
	
	if(!f0)
		return(0);
	
	e0=(f0>>52)&2047;
	m0=0x80000000ULL|((uint32_t)(f0>>21));

//	if(f0&0x8000000000000000ULL)
//		m0=-m0;
	
	e1=(e0-1023)-31;
	if(e1>=0)
	{
		t=0x80000000U;
	}else
	{
		e1=-e1;
		if(e1>31)
		{
			t=0;
		}else
		{
			t=m0>>e1;
		}
		if(f0&0x8000000000000000ULL)
			t=-t;
	}
	return(t);
}

#if 0
uint32_t __sfp_float_f32(int32_t iv)
{
	uint32_t sg;
	uint32_t i0, t0;
	int e0;

	if(!iv)
		return(0);

	if(iv<0)
	{
		i0=-iv;
		sg=0x80000000U;
	}else
	{
		i0=iv;
		sg=0;
	}
	e0=127+23;
	if(i0>>24)
	{
		while(i0>>24)
		{
			i0=i0>>1;
//			i0=(i0+1)>>1;
			e0++;
		}
	}else
	{
#if 1
		if(!(i0&0x00FFFF00))
		{
			i0=i0<<16;
			e0-=16;
		}

		if(!(i0&0x00FF0000))
		{
			i0=i0<<8;
			e0-=8;
		}

		if(!(i0&0x00F00000))
		{
			i0=i0<<4;
			e0-=4;
		}

		if(!(i0&0x00C00000))
		{
			i0=i0<<2;
			e0-=2;
		}

		if(!(i0&0x00800000))
		{
			i0=i0<<1;
			e0-=1;
		}
#endif
	}

	t0=sg|(e0<<23)|(i0&0x007FFFFF);
	return(t0);
}

uint64_t __sfp_float_f64(int32_t iv)
{
	uint64_t sg;
	uint64_t i0, t0;
	int e0;

	if(!iv)
		return(0);

	if(iv<0)
	{
		i0=-iv;
		sg=0x8000000000000000ULL;
	}else
	{
		i0=iv;
		sg=0;
	}
	e0=1023+52;

#if 1
	if(!(i0&0x001FFFFFFFE00000ULL))
		{ i0=i0<<32; e0-=32; }
	if(!(i0&0x001FFFE000000000ULL))
		{ i0=i0<<16; e0-=16; }
	if(!(i0&0x001FE00000000000ULL))
		{ i0=i0<<8; e0-=8; }
	if(!(i0&0x001E000000000000ULL))
		{ i0=i0<<4; e0-=4; }
	if(!(i0&0x0018000000000000ULL))
		{ i0=i0<<2; e0-=2; }
	if(!(i0&0x0010000000000000ULL))
		{ i0=i0<<1; e0--; }
#endif

	t0=sg|(((uint64_t)e0)<<52)|(i0&0x000FFFFFFFFFFFFFULL);
	return(t0);
}
#endif
