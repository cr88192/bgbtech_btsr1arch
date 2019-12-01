/*
Software Floating Point
To be used if the core lacks a native FPU.
 */

#ifdef __BGBCC__
u32 __float32_getbits(float f);
float __float32_frombits(u32 f);
u64 __float64_getbits(double f);
double __float64_frombits(u64 f);

s64 __int32_dmuls(s32 x, s32 y);
u64 __int32_dmulu(u32 x, u32 y);

#else
u32 __float32_getbits(float f)
	{ return(*(u32 *)(&f)); }
float __float32_frombits(u32 f)
	{ return(*(float *)(&f)); }
u64 __float64_getbits(double f)
	{ return(*(u64 *)(&f)); }
double __float64_frombits(u64 f)
	{ return(*(double *)(&f)); }

s64 __int32_dmuls(s32 x, s32 y)
	{ return(((s64)x)*y); }
u64 __int32_dmulu(u32 x, u32 y)
	{ return(((u64)x)*y); }

#endif

u16 sfp_rcp_adj[256]={
0x0000, 0x03F9, 0x07E1, 0x0BB9, 0x0F82, 0x133C, 0x16E7, 0x1A83,
0x1E10, 0x218E, 0x24FF, 0x2860, 0x2BB4, 0x2EFA, 0x3232, 0x355C,
0x3879, 0x3B88, 0x3E8B, 0x4180, 0x4468, 0x4744, 0x4A13, 0x4CD5,
0x4F8B, 0x5235, 0x54D3, 0x5765, 0x59EB, 0x5C65, 0x5ED4, 0x6137,
0x638F, 0x65DB, 0x681D, 0x6A53, 0x6C7F, 0x6EA0, 0x70B6, 0x72C1,
0x74C2, 0x76B9, 0x78A5, 0x7A88, 0x7C60, 0x7E2E, 0x7FF3, 0x81AE,
0x835F, 0x8506, 0x86A4, 0x8839, 0x89C5, 0x8B47, 0x8CC0, 0x8E30,
0x8F97, 0x90F6, 0x924B, 0x9398, 0x94DD, 0x9619, 0x974C, 0x9877,
0x999A, 0x9AB5, 0x9BC7, 0x9CD2, 0x9DD4, 0x9ECF, 0x9FC2, 0xA0AD,
0xA190, 0xA26C, 0xA340, 0xA40D, 0xA4D2, 0xA590, 0xA647, 0xA6F7,
0xA79F, 0xA840, 0xA8DB, 0xA96E, 0xA9FA, 0xAA80, 0xAAFF, 0xAB77,
0xABE9, 0xAC54, 0xACB8, 0xAD16, 0xAD6D, 0xADBE, 0xAE09, 0xAE4E,
0xAE8C, 0xAEC4, 0xAEF6, 0xAF22, 0xAF48, 0xAF68, 0xAF83, 0xAF97,
0xAFA5, 0xAFAE, 0xAFB1, 0xAFAF, 0xAFA6, 0xAF99, 0xAF85, 0xAF6D,
0xAF4E, 0xAF2B, 0xAF02, 0xAED4, 0xAEA0, 0xAE67, 0xAE2A, 0xADE7,
0xAD9E, 0xAD51, 0xACFF, 0xACA8, 0xAC4C, 0xABEB, 0xAB85, 0xAB1B,
0xAAAB, 0xAA37, 0xA9BE, 0xA941, 0xA8BF, 0xA838, 0xA7AC, 0xA71D,
0xA688, 0xA5F0, 0xA552, 0xA4B1, 0xA40B, 0xA361, 0xA2B2, 0xA1FF,
0xA148, 0xA08D, 0x9FCE, 0x9F0A, 0x9E42, 0x9D77, 0x9CA7, 0x9BD3,
0x9AFB, 0x9A20, 0x9940, 0x985D, 0x9775, 0x968A, 0x959B, 0x94A8,
0x93B2, 0x92B7, 0x91B9, 0x90B8, 0x8FB2, 0x8EAA, 0x8D9D, 0x8C8D,
0x8B79, 0x8A62, 0x8947, 0x8829, 0x8708, 0x85E3, 0x84BA, 0x838E,
0x825F, 0x812D, 0x7FF7, 0x7EBE, 0x7D82, 0x7C42, 0x7AFF, 0x79B9,
0x7870, 0x7724, 0x75D4, 0x7482, 0x732C, 0x71D4, 0x7078, 0x6F19,
0x6DB7, 0x6C53, 0x6AEB, 0x6980, 0x6813, 0x66A2, 0x652F, 0x63B8,
0x623F, 0x60C3, 0x5F45, 0x5DC3, 0x5C3F, 0x5AB8, 0x592E, 0x57A1,
0x5612, 0x5480, 0x52EC, 0x5154, 0x4FBA, 0x4E1E, 0x4C7F, 0x4ADD,
0x4939, 0x4792, 0x45E9, 0x443D, 0x428E, 0x40DE, 0x3F2A, 0x3D74,
0x3BBC, 0x3A02, 0x3844, 0x3685, 0x34C3, 0x32FF, 0x3138, 0x2F6F,
0x2DA4, 0x2BD7, 0x2A07, 0x2835, 0x2660, 0x248A, 0x22B1, 0x20D6,
0x1EF8, 0x1D19, 0x1B37, 0x1953, 0x176D, 0x1585, 0x139A, 0x11AE,
0x0FBF, 0x0DCF, 0x0BDC, 0x09E7, 0x07F0, 0x05F7, 0x03FC, 0x01FF
};

u32 __sfp_fadd_f32i(u32 f0, u32 f1)
{
	u32 f2;
	int e0, e1, e2;
	int m0, m1, m2;
	
	if(!f1)
		return(f0);
	
	e0=(f0>>23)&255;
	e1=(f1>>23)&255;
	if((e0-e1)>=24)
		return(f0);
//	e2=(e1>=e0)?e1:e0;
	e2=e0;
//	m0=(0x00800000|(f0&0x00FFFFFF))>>(e2-e0);
	m0=0x00800000|(f0&0x00FFFFFF);
	m1=(0x00800000|(f1&0x00FFFFFF))>>(e2-e1);
	m2=m0+m1;
	if((m2&0xFF000000))
	{
		m2=m2>>1;
		e2++;
		
		if(e2>=255)
		{
			e2=255;
			m2=0;
		}
	}
	
	f2=(f0&0x80000000)|(e2<<23)|(m2&0x007FFFFF);
	return(f2);
}

u32 __sfp_fsub_f32i(u32 f0, u32 f1)
{
	u32 f2;
	int e0, e1, e2;
	int m0, m1, m2;

	if(!f1)
		return(f0);

	e0=(f0>>23)&255;
	e1=(f1>>23)&255;
	if((e0-e1)>=24)
		return(f0);
	e2=e0;
//	m0=(0x00800000|(f0&0x00FFFFFF))>>(e2-e0);
	m0=0x00800000U|(f0&0x00FFFFFFU);
	m1=(0x00800000U|(f1&0x00FFFFFFU))>>(e2-e1);
	m2=m0-m1;
	if(!(m2&0x00800000U))
	{
		if(m2)
		{
			if(!(m2&0x00FFFF00U))
				{ m2=m2<<16; e2-=16; }
			if(!(m2&0x00FF0000U))
				{ m2=m2<<8; e2-=8; }
			if(!(m2&0x00F00000U))
				{ m2=m2<<4; e2-=4; }
			if(!(m2&0x00C00000U))
				{ m2=m2<<2; e2-=2; }
			if(!(m2&0x00800000U))
				{ m2=m2<<1; e2-=1; }

#if 0
			while(!(m2&0x00800000U))
			{
				m2=m2<<1;
				e2--;
			}
#endif
			if(e2<=0)
			{
				e2=0; m2=0;
			}
		}
		else
		{
			e2=0; m2=0;
		}
	}

	f2=(f0&0x80000000U)|(e2<<23)|(m2&0x007FFFFFU);
	return(f2);
}

u32 __sfp_fadd_f32(u32 f0, u32 f1)
{
	u32 f2;

	if((f0^f1)&0x80000000U)
	{
		if((f0&0x7FFFFFFFU)>=(f1&0x7FFFFFFFU))
		{
			f2=__sfp_fsub_f32i(f0, f1);
			return(f2);
		}else
		{
			f2=__sfp_fsub_f32i(f1, f0);
			return(f2);
		}
	}else
	{
		if((f0&0x7FFFFFFFU)>=(f1&0x7FFFFFFFU))
		{
			f2=__sfp_fadd_f32i(f0, f1);
			return(f2);
		}else
		{
			f2=__sfp_fadd_f32i(f1, f0);
			return(f2);
		}
	}
}

u32 __sfp_fsub_f32(u32 f0, u32 f1)
{
	u32 f2;
	f2=__sfp_fadd_f32(f0, f1^0x80000000U);
	return(f2);
}

#if 0
u32 __sfp_fmul_f32_asm(u32 f0, u32 f1);

u32 __sfp_fmul_f32(u32 f0, u32 f1)
{
	u32 f2, sg;
	int e0, e1, e2;
	int m0, m1, m2;

//	return(__sfp_fmul_f32_asm(f0, f1));

//	if(!f0 || !f1)
//		return(0);
//	if(f0==0)		return(0);
//	if(f1==0)		return(0);

	sg=(f0^f1)&0x80000000;

//	if(!f0)
//	{
//		__debugbreak();
//	}

	if(!f0 || !f1)
//	if(!f1)
//		return(sg);
		return(0);
	
	e0=(f0>>23)&255;
	e1=(f1>>23)&255;
//	e2=(e1>=e0)?e1:e0;
	e2=(e0+e1)-127;
	m0=0x00800000|(f0&0x00FFFFFF);
	m1=0x00800000|(f1&0x00FFFFFF);
//	m2=m0+m1;
//	m2=(m0>>9)*(m1>>9);
//	m2=(((u64)m0)*m1)>>18;
	m2=__int32_dmulu(m0, m1)>>18;
//	m2=(m2+31)>>6;
	m2=(m2+15)>>5;
//	while((m2&0xFF000000))
	if((m2&0xFF000000))
	{
		m2=m2>>1;
//		m2=(m2+1)>>1;
		e2++;
	}
	if(e2>=255)
	{
		e2=255;
		m2=0;
	}else if(e2<=0)
	{
		return(0);
//		return(sg);
	}

//	f2=(f0&0x80000000)|(e2<<23)|(m2&0x007FFFFF);
	f2=sg|(e2<<23)|(m2&0x007FFFFF);
	return(f2);
}
#endif

#if 0
u32 __sfp_frcp_f32(u32 f0)
{
	u32 f1, f2;
	int i;

	i=(f0>>23)&255;
	if((i<2) || (i>252))
		return(0);
	
	f1=(f0&0x80000000)|(0x7F000000-(f0&0x7FFFFFFF));

//	for(i=0; i<10; i++)
	for(i=0; i<6; i++)
	{
		f2=__sfp_fmul_f32(f0, f1);
		f1-=(f2-0x3F800000);
	}

	return(f1);
}
#endif

#if 1
u32 __sfp_frcp_f32(u32 f0)
{
	u32 f1, f2;
	int i;

	i=(f0>>23)&255;
//	if((i<2) || (i>252))
//		return(0);
//	if(i>252)
//		return(0);
	if(i>254)
	{
//		f1=(f0&0x80000000)|0x7F800000;
//		f1=(f0&0x80000000);
		f1=0;
		return(f1);
	}
	if(i<1)
	{
		f1=(f0&0x80000000)|0x7FC00000;
		return(f1);
	}

	f1=(f0&0x80000000)|(0x7F000000-(f0&0x7FFFFFFF));
	f1-=(sfp_rcp_adj[(f0>>15)&255])<<5;

#if 1
//	for(i=0; i<10; i++)
//	for(i=0; i<6; i++)
	for(i=0; i<3; i++)
	{
		f2=__sfp_fmul_f32(f0, f1);
		f1-=(f2-0x3F800000);
	}
#endif

	return(f1);
}
#endif

u32 __sfp_fdiv_f32(u32 f0, u32 f1)
{
	return(__sfp_fmul_f32(f0, __sfp_frcp_f32(f1)));
}

u32 __sfp_fsqrt_f32(u32 f0)
{
	u32 f1, f2;
	int i;

	f1=(f0>>1)+0x1FC00000;

//	for(i=0; i<12; i++)
	for(i=0; i<6; i++)
	{
		f2=__sfp_fmul_f32(f1, f1);
		f1-=((s32)(f2-f0))>>1;
	}

	return(f1);
}

#if 1
u64 __sfp_fadd_f64i(u64 f0, u64 f1)
{
	u64 f2;
	int e0, e1, e2;
	u64 m0, m1, m2;
	
	if(!f1)
		return(f0);
	
	e0=(f0>>52)&2047;
	e1=(f1>>52)&2047;
	if((e0-e1)>=52)
		return(f0);
	e2=e0;
	m0=0x0010000000000000ULL|(f0&0x000FFFFFFFFFFFFFULL);
	m1=(0x0010000000000000ULL|(f1&0x000FFFFFFFFFFFFFULL))>>(e2-e1);
	m2=m0+m1;
	if((m2&0xFFE0000000000000ULL))
	{
		m2=m2>>1;
		e2++;
		
		if(e2>=2047)
		{
			e2=2047;
			m2=0;
		}
	}
	
	f2=(f0&0x8000000000000000ULL)|(((u64)e2)<<52)|(m2&0x000FFFFFFFFFFFFFULL);
	return(f2);
}

u64 __sfp_fsub_f64i(u64 f0, u64 f1)
{
	u64 f2;
	int e0, e1, e2;
	u64 m0, m1, m2;

	if(!f1)
		return(f0);

	e0=(f0>>52)&2047;
	e1=(f1>>52)&2047;
	if((e0-e1)>=52)
		return(f0);
	e2=e0;
	m0=0x0010000000000000ULL|(f0&0x000FFFFFFFFFFFFFULL);
	m1=(0x0010000000000000ULL|(f1&0x000FFFFFFFFFFFFFULL))>>(e2-e1);
	m2=m0-m1;
	if(!(m2&0x0010000000000000ULL))
	{
		if(m2)
		{
			while(!(m2&0x0010000000000000ULL))
			{
				m2=m2<<1;
				e2--;
			}
			if(e2<=0)
			{
				e2=0; m2=0;
			}
		}
		else
		{
			e2=0; m2=0;
		}
	}

	f2=(f0&0x8000000000000000ULL)|(((u64)e2)<<52)|(m2&0x000FFFFFFFFFFFFFULL);
	return(f2);
}

u64 __sfp_fadd_f64(u64 f0, u64 f1)
{
	u64 f2;

	if((f0^f1)&0x8000000000000000ULL)
	{
		if((f0&0x7FFFFFFFFFFFFFFFULL)>=(f1&0x7FFFFFFFFFFFFFFFULL))
		{
			f2=__sfp_fsub_f64i(f0, f1);
			return(f2);
		}else
		{
			f2=__sfp_fsub_f64i(f1, f0);
			return(f2);
		}
	}else
	{
		if((f0&0x7FFFFFFFFFFFFFFFULL)>=(f1&0x7FFFFFFFFFFFFFFFULL))
		{
			f2=__sfp_fadd_f64i(f0, f1);
			return(f2);
		}else
		{
			f2=__sfp_fadd_f64i(f1, f0);
			return(f2);
		}
	}
}

u64 __sfp_fsub_f64(u64 f0, u64 f1)
{
	u64 f2;
	f2=__sfp_fadd_f64(f0, f1^0x8000000000000000ULL);
	return(f2);
}


u64 __sfp_fmul_f64(u64 f0, u64 f1)
{
	u64 f2, sg;
	u64 m0, m1, m2;
	u64 m3, m4, m5, m6;
	int e0, e1, e2;
	
	sg=(f0^f1)&0x8000000000000000ULL;
	
	if(!f0 || !f1)
//		return(sg);
		return(0);
	
	e0=(f0>>52)&2047;
	e1=(f1>>52)&2047;
	e2=(e0+e1)-1023;
	m0=0x0010000000000000ULL|(f0&0x000FFFFFFFFFFFFFULL);
	m1=0x0010000000000000ULL|(f1&0x000FFFFFFFFFFFFFULL);
//	m2=m0+m1;
//	m2=(m0>>9)*(m1>>9);
//	m2=(m0>>21)*(m1>>21);

	m3=__int32_dmulu(m0, m1);
	m4=__int32_dmulu(m0>>32, m1);
	m5=__int32_dmulu(m0, m1>>32);
	m6=__int32_dmulu(m0>>32, m1>>32);
	m2=(m6<<22)+(m5>>10)+(m4>>10)+(m3>>42);

//	m2=(((u64)m0)*m1)>>18;
	m2=(m2+511)>>10;
//	m2=(m2+15)>>5;

//	__debugbreak();

	while((m2&0xFFE0000000000000ULL))
	{
//		m2=m2>>1;
		m2=(m2+1)>>1;
		e2++;
	}
	if(e2>=2047)
	{
		e2=2047;
		m2=0;
	}else if(e2<=0)
	{
//		return(sg);
		return(0);
	}

	f2=sg|(((u64)e2)<<52)|(m2&0x000FFFFFFFFFFFFFULL);
	return(f2);
}

#if 0
u64 __sfp_frcp_f64(u64 f0)
{
	u64 f1, f2;
	int i;
	
	i=(f0>>52)&2047;
	if((i<2) || (i>2044))
		return(0);
	
	f1=(f0&0x8000000000000000ULL)|
		(0x7FF0000000000000ULL-
			(f0&0x7FFFFFFFFFFFFFFFULL));

//	for(i=0; i<12; i++)
	for(i=0; i<8; i++)
	{
		f2=__sfp_fmul_f64(f0, f1);
		f1-=(f2-0x3FF0000000000000ULL);
	}

	return(f1);
}
#endif

#if 1
u64 __sfp_frcp_f64(u64 f0)
{
	u64 f1, f2;
	int i;
	
	i=(f0>>52)&2047;
//	if((i<2) || (i>2044))
//		return(0);
	if(i<1)
	{
		f1=(f0&0x8000000000000000ULL)|
			0x7FF8000000000000ULL;
		return(f1);
	}
	if(i>2046)
	{
//		f1=(f0&0x8000000000000000ULL);
		f1=0;
		return(f1);
	}
	
	f1=(f0&0x8000000000000000ULL)|
		(0x7FF0000000000000ULL-
			(f0&0x7FFFFFFFFFFFFFFFULL));
	f1-=((u64)(sfp_rcp_adj[(byte)(f0>>44)]))<<34;

#if 1
//	for(i=0; i<12; i++)
//	for(i=0; i<8; i++)
//	for(i=0; i<4; i++)
	for(i=0; i<2; i++)
	{
		f2=__sfp_fmul_f64(f0, f1);
		f1-=(f2-0x3FF0000000000000ULL);
	}
#endif

	return(f1);
}
#endif

u64 __sfp_fdiv_f64(u64 f0, u64 f1)
{
	return(__sfp_fmul_f64(f0, __sfp_frcp_f64(f1)));
}


u64 __sfp_fsqrt_f64(u64 f0)
{
	u64 f1, f2;
	int i;

	f1=(f0>>1)+0x1FF8000000000000ULL;
//	for(i=0; i<12; i++)
	for(i=0; i<8; i++)
	{
		f2=__sfp_fmul_f64(f1, f1);
		f1-=((s64)(f2-f0))>>1;
	}

	return(f1);
}
#endif


u64 __sfp_fcnvsd(u32 f0)
{
	u64 f1, sg;
	
	if(!f0)
		return(0);
	
	sg=(f0&0x80000000);
	
	f1=f0&0x7FFFFFFF;
	f1+=0x1C0000000ULL;
	f1=f1<<29;
	f1|=sg<<32;
	
	return(f1);
}

u32 __sfp_fcnvds(u64 f0)
{
	u32 f1, sg;

	if(!f0)
		return(0);

	sg=(f0>>32)&0x80000000;
//	f1=(f0>>29)-0x1C0000000ULL;	
	f1=((f0+0x10000000)>>29)-0x1C0000000ULL;	
	if(f1&0x80000000)
		return(0);
	return(f1|sg);
}

s32 __sfp_ftrc_f32(u32 f0)
{
	s32 t;
	u32 f2;
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

u32 __sfp_ftrc_f64(u64 f0)
{
	s32 t;
	u32 f2;
	int e0, e1, e2;
//	u32 m0, m1, m2;
	s64 m0, m1, m2;
	
	if(!f0)
		return(0);
	
	e0=(f0>>52)&2047;
	m0=0x80000000ULL|((u32)(f0>>21));

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
//			t=((s32)m0)>>e1;
			t=m0>>e1;
		}
		if(f0&0x8000000000000000ULL)
			t=-t;
	}
	return(t);
}

u32 __sfp_float_f32(s32 iv)
{
	u32 sg;
	u32 i0, t0;
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

#if 0
		while(!(i0&0x00800000))
		{
			i0=i0<<1;
			e0--;
		}
#endif
	}

	t0=sg|(e0<<23)|(i0&0x007FFFFF);
	return(t0);
}

#if 0
u64 __sfp_float_f64(s32 iv)
{
	u64 t;
	/* FIXME: Hack, do proper double */
	t=__sfp_fcnvsd(__sfp_float_f32(iv));
	return(t);
}
#endif

u64 __sfp_float_f64(s32 iv)
{
	u64 sg;
	u64 i0, t0;
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

#if 0
	while(!(i0&0x0010000000000000ULL))
	{
		i0=i0<<1;
		e0--;
	}
#endif

	t0=sg|(((u64)e0)<<52)|(i0&0x000FFFFFFFFFFFFFULL);
	return(t0);
}

#if 0
u64 __sfp_fadd_f64(u64 f0, u64 f1)
{
	u32 t0, t1, t2;
	u64 t;
	/* FIXME: Hack, do proper double */
	t0=__sfp_fcnvds(f0);
	t1=__sfp_fcnvds(f1);
	t2=__sfp_fadd_f32(t0, t1);
	t=__sfp_fcnvsd(t2);
	return(t);	
}

u64 __sfp_fsub_f64(u64 f0, u64 f1)
{
	u32 t0, t1, t2;
	u64 t;
	/* FIXME: Hack, do proper double */
	t0=__sfp_fcnvds(f0);
	t1=__sfp_fcnvds(f1);
	t2=__sfp_fsub_f32(t0, t1);
	t=__sfp_fcnvsd(t2);
	return(t);	
}

u64 __sfp_fmul_f64(u64 f0, u64 f1)
{
	u32 t0, t1, t2;
	u64 t;
	/* FIXME: Hack, do proper double */
	t0=__sfp_fcnvds(f0);
	t1=__sfp_fcnvds(f1);
	t2=__sfp_fmul_f32(t0, t1);
	t=__sfp_fcnvsd(t2);
	return(t);	
}

u64 __sfp_fdiv_f64(u64 f0, u64 f1)
{
	u32 t0, t1, t2;
	u64 t;
	/* FIXME: Hack, do proper double */
	t0=__sfp_fcnvds(f0);
	t1=__sfp_fcnvds(f1);
	t2=__sfp_fdiv_f32(t0, t1);
	t=__sfp_fcnvsd(t2);
	return(t);	
}
#endif

int __sfp_cmpeq_f32(u32 f0, u32 f1)
{
	if((((byte)(f0>>23))==0xFF) && (f0&0x007FFFFF))
		return(0);
	if((((byte)(f1>>23))==0xFF) && (f1&0x007FFFFF))
		return(0);
	return(f0==f1);
}

int __sfp_cmpne_f32(u32 f0, u32 f1)
{
//	if((((byte)(f0>>23))==0xFF) && (f0&0x007FFFFF))
//		return(1);
//	if((((byte)(f1>>23))==0xFF) && (f1&0x007FFFFF))
//		return(1);
//	return(f0!=f1);
	return(!__sfp_cmpeq_f32(f0, f1));
}

int __sfp_cmpgt_f32(u32 f0, u32 f1)
{
	s32 f2, f3;
	
	f2=f0;
	f3=f1;
	if(f0&0x80000000U)f2^=0x7FFFFFFFU;
	if(f1&0x80000000U)f3^=0x7FFFFFFFU;
	return(f2>f3);
}

int __sfp_cmplt_f32(u32 f0, u32 f1)
{
	return(__sfp_cmpgt_f32(f1, f0));
}

int __sfp_cmpge_f32(u32 f0, u32 f1)
{
	return(!__sfp_cmpgt_f32(f1, f0));
}

int __sfp_cmple_f32(u32 f0, u32 f1)
{
	return(!__sfp_cmpgt_f32(f0, f1));
}


int __sfp_cmpeq_f64(u64 f0, u64 f1)
{
	if((((f0>>52)&2047)==2047) && (f0&0x000FFFFFFFFFFFFFULL))
		return(0);
	if((((f1>>52)&2047)==2047) && (f1&0x000FFFFFFFFFFFFFULL))
		return(0);
//	return(!__sfp_cmpeq_f64(f0, f1));
	return(f0==f1);
}

int __sfp_cmpne_f64(u64 f0, u64 f1)
{
	return(!__sfp_cmpeq_f64(f0, f1));
//	return(f0!=f1);
}

int __sfp_cmpgt_f64(u64 f0, u64 f1)
{
	s64 f2, f3;
	
	f2=f0;
	f3=f1;
	if(f0&0x8000000000000000ULL)f2^=0x7FFFFFFFFFFFFFFFULL;
	if(f1&0x8000000000000000ULL)f3^=0x7FFFFFFFFFFFFFFFULL;
	return(f2>f3);
}

int __sfp_cmplt_f64(u64 f0, u64 f1)
{
	return(__sfp_cmpgt_f64(f1, f0));
}

int __sfp_cmpge_f64(u64 f0, u64 f1)
{
	return(!__sfp_cmpgt_f64(f1, f0));
}

int __sfp_cmple_f64(u64 f0, u64 f1)
{
	return(!__sfp_cmpgt_f64(f0, f1));
}

u32 __sfp_lnot_f32(u32 f0)
{
	return(f0?0x00000000:0x3F800000U);
}

u64 __sfp_lnot_f64(u64 f0)
{
	return(f0?0x0000000000000000ULL:0x3FF0000000000000ULL);
}

u32 __sfp_neg_f32(u32 f0)
{
	return(f0^0x80000000U);
}

u64 __sfp_neg_f64(u64 f0)
{
	return(f0^0x8000000000000000ULL);
}

u32 __sfp_ldhf16(u16 f0)
{
	u32 t0;
	
	if(!f0)
		return(0x00000000);

	if((f0&0x7C00)==0x7C00)
	{
		t0=((f0&0x8000)<<16) |
			0x7F800000U |
			((f0&0x03FF)<<13);
		return(t0);
	}
	
	t0=((f0&0x8000)<<16) |
		(((f0&0x7FFF)<<13)+0x38000000);
	return(t0);
}

u16 __sfp_sthf16(u32 f0)
{
	int i0;
	
	i0=(s32)(f0&0x7FFFFFFF);
	i0=(i0-0x38000000)>>13;
	if(i0<=0)
		return(0x0000);
	if(i0>=0x7C00)
		return(((f0>>16)&0x8000)|0x7C00);
	return(((f0>>16)&0x8000)|i0);
}


double __lfp_fcnvsd(float f0)
{
	u64 t;
	t=__sfp_fcnvsd(*(u32 *)(&f0));
	return(*(double *)(&t));
}

float __lfp_fcnvds(double f0)
{
	u32 t;
	t=__sfp_fcnvds(*(u64 *)(&f0));
	return(*(float *)(&t));
}

float __lfp_fadd_f32(float f0, float f1)
{
	u32 t0, t1, t2;
	
	t0=*(u32 *)(&f0);
	t1=*(u32 *)(&f1);
	t2=__sfp_fadd_f32(t0, t1);
	return(*(float *)(&t2));
}

float __lfp_fsub_f32(float f0, float f1)
{
	u32 t0, t1, t2;
	
	t0=*(u32 *)(&f0);
	t1=*(u32 *)(&f1);
	t2=__sfp_fsub_f32(t0, t1);
	return(*(float *)(&t2));
}

float __lfp_fmul_f32(float f0, float f1)
{
	u32 t0, t1, t2;
	
	t0=*(u32 *)(&f0);
	t1=*(u32 *)(&f1);
	t2=__sfp_fmul_f32(t0, t1);
	return(*(float *)(&t2));
}

#if 0
float __lfp_fdiv_f32(float f0, float f1)
{
	u32 t0, t1, t2;
	
	t0=*(u32 *)(&f0);
	t1=*(u32 *)(&f1);
	t2=__sfp_fdiv_f32(t0, t1);
	return(*(float *)(&t2));
}
#endif

double __lfp_fadd_f64(double f0, double f1)
{
	u64 t0, t1, t2;
	t0=*(u64 *)(&f0);
	t1=*(u64 *)(&f1);
	t2=__sfp_fadd_f64(t0, t1);
	return(*(double *)(&t2));
}

double __lfp_fsub_f64(double f0, double f1)
{
	u64 t0, t1, t2;
	t0=*(u64 *)(&f0);
	t1=*(u64 *)(&f1);
	t2=__sfp_fsub_f64(t0, t1);
	return(*(double *)(&t2));
}

double __lfp_fmul_f64(double f0, double f1)
{
	u64 t0, t1, t2;
	t0=*(u64 *)(&f0);
	t1=*(u64 *)(&f1);
	t2=__sfp_fmul_f64(t0, t1);
	return(*(double *)(&t2));
}

double __lfp_fdiv_f64(double f0, double f1)
{
	u64 t0, t1, t2;
	t0=*(u64 *)(&f0);
	t1=*(u64 *)(&f1);
	t2=__sfp_fdiv_f64(t0, t1);
	return(*(double *)(&t2));
}

s32 __lfp_ftrc_f32(float f0)
{
	s32 t;
	t=__sfp_ftrc_f32(*(u32 *)(&f0));
	return(t);
}

s32 __lfp_ftrc_f64(double f0)
{
	s32 t;
	t=__sfp_ftrc_f64(*(u64 *)(&f0));
	return(t);
}

float __lfp_float_f32(s32 i0)
{
	u32 t;
	t=__sfp_float_f32(i0);
	return(*(float *)(&t));
}

double __lfp_float_f64(s32 i0)
{
	u64 t;
	t=__sfp_float_f64(i0);
	return(*(double *)(&t));
}

float __lfp_frcpa_f32(float f0)
{
	u32 i0, i1, i2;
	float f1, f2;
	int i;
		
//	i0=*(u32 *)(&f0);
	i0=__float32_getbits(f0);

	i=(i0>>23)&255;
	if((i<2) || (i>252))
		return(0);

	i1=(i0&0x80000000)|(0x7F000000-(i0&0x7FFFFFFF));
//	f1=*(float *)(&i1);

#if 1
	for(i=0; i<12; i++)
	{
//		f1=*(float *)(&i1);
		f1=__float32_frombits(i1);
		f2=f0*f1;
//		i2=*(u32 *)(&f2);
		i2=__float32_getbits(f2);
		i1-=(i2-0x3F800000);
	}
//	f1=*(float *)(&i1);
	f1=__float32_frombits(i1);
#endif

#if 0
	for(i=0; i<10; i++)
	{
		f2=f0*f1;
//		f1-=(f2-1.0)*0.125;
//		f1-=(f2-1.0)*0.0625;
//		f1+=(1.0-f2)*0.125;

		i1=*(u32 *)(&f1);
		i2=*(u32 *)(&f2);
		i1-=(i2-0x3F800000);
		f1=*(float *)(&i1);
	}
#endif

	return(f1);
}

#if 1
float __lfp_frcp_f32(float f0)
{
	u32 t;
	t=__sfp_frcp_f32(*(u32 *)(&f0));
	return(*(float *)(&t));
}
#endif

double __lfp_frcp_f64(double f0)
{
	u64 t;
	t=__sfp_frcp_f64(*(u64 *)(&f0));
	return(*(double *)(&t));
}

#if 1
float __lfp_fdiv_f32(float f0, float f1)
{
//	return(f0*__lfp_frcp_f32(f1));
	return(f0*__lfp_frcpa_f32(f1));
}
#endif


float __lfp_fsqrt_f32(float f0)
{
	u32 t;
	t=__sfp_fsqrt_f32(*(u32 *)(&f0));
	return(*(float *)(&t));
}

#if 1
double __lfp_fsqrt_f64(double f0)
{
	u64 t;
	t=__sfp_fsqrt_f64(*(u64 *)(&f0));
	return(*(double *)(&t));
}
#endif

int __lfp_cmpeq_f32(float f0, float f1)
{
	u32 t0, t1;
	t0=*(u32 *)(&f0);
	t1=*(u32 *)(&f1);
	return(__sfp_cmpeq_f32(t0, t1));
}

int __lfp_cmpgt_f32(float f0, float f1)
{
	u32 t0, t1;
	t0=*(u32 *)(&f0);
	t1=*(u32 *)(&f1);
	return(__sfp_cmpgt_f32(t0, t1));
}

int __lfp_cmpeq_f64(double f0, double f1)
{
	u64 t0, t1;
	t0=*(u64 *)(&f0);
	t1=*(u64 *)(&f1);
	return(__sfp_cmpeq_f64(t0, t1));
}

int __lfp_cmpgt_f64(double f0, double f1)
{
	u64 t0, t1;
	t0=*(u64 *)(&f0);
	t1=*(u64 *)(&f1);
	return(__sfp_cmpgt_f64(t0, t1));
}
