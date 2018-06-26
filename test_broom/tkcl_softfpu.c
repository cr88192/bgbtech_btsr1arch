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
