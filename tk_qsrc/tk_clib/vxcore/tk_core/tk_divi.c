#if 0
static int _fcn_clz32(u32 v)
{
	const u32 m1=0x80000000U;
	const u32 m8=0xFF000000U;
	const u32 m16=0xFFFF0000U;
	u32 c;
	int n;
	c=v; n=0;
	if(!(c&m16))
		{ n+=16; c<<=16; }
	while(!(c&m8))
		{ n+=8; c<<=8; }
	while(!(c&m1))
		{ n++; c+=c; }
	return(n);
}
#endif

#if 0
u32 __udivsi3(u32 n, u32 d)
{
	u32 q, r;
	int s, c;
//	int sr;
	byte sr;

	if(!d || !n)
		return(0);
//	sr=_fcn_clz(d)-_fcn_clz(n);
	sr=(byte)(_fcn_clz32(d)-_fcn_clz32(n));

	if(sr>=31)
	{
		if(sr==31)
			return(n);
		return(0);
	}

	sr++;
	q=n<<(32-sr); r=n>>sr; c=0;
	while(sr--)
	{
		r=(r<<1)|(q>>31);
		q=(q<<1)|c;
		s=((int)(d-r-1))>>31;
		c=s&1;
		r-=d&s;
	}
	q=(q<<1)|c;
	q=(u32)q;
	return(q);
}
#endif

#if 0
s32 __sdivsi3(s32 a, s32 b)
{
	s32 sga, sgb;
	sga=a>>31;		sgb=b>>31;
	a=(a^sga)-sga;	b=(b^sgb)-sgb;
	sga^=sgb;
	return((__udivsi3(a, b)^sga)-sga);
}
#endif

#if 1
static int _fcn_clz64(u64 v)
{
	const u32 m1=0x8000000000000000ULL;
	const u32 m8=0xFF00000000000000ULL;
	const u32 m16=0xFFFF000000000000ULL;
	const u64 m32=0xFFFFFFFF00000000ULL;
	u64 c;
	int n;
	c=v; n=0;
	if(!(c&m32))
		{ n+=32; c<<=32; }
	if(!(c&m16))
		{ n+=16; c<<=16; }
	while(!(c&m8))
		{ n+=8; c<<=8; }
	while(!(c&m1))
		{ n++; c+=c; }
	return(n);
}

u64 __udivdi3(u64 n, u64 d)
{
	u64 q, r;
	int s, c;
//	int sr;
	byte sr;

	if(!d || !n)
		return(0);
//	sr=_fcn_clz(d)-_fcn_clz(n);
	sr=(byte)(_fcn_clz64(d)-_fcn_clz64(n));

	if(sr>=63)
	{
		if(sr==63)
			return(n);
		return(0);
	}

	sr++;
	q=n<<(64-sr); r=n>>sr; c=0;
	while(sr--)
	{
		r=(r<<1)|(q>>63);
		q=(q<<1)|c;
		s=((s64)(d-r-1))>>63;
		c=s&1;
		r-=d&s;
	}
	q=(q<<1)|c;
	return(q);
}

s64 __sdivdi3(s64 a, s64 b)
{
	s64 sga, sgb;
	sga=a>>63;		sgb=b>>63;
	a=(a^sga)-sga;	b=(b^sgb)-sgb;
	sga^=sgb;
	return((__udivdi3(a, b)^sga)-sga);
}
#endif
