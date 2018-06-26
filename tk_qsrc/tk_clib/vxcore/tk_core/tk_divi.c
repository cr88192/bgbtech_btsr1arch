static int _fcn_clz(u32 v)
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

u32 __udivsi3(u32 n, u32 d)
{
    u32 q, r;
    int s, c;
    int sr;

    if(!d || !n)
		return(0);
    sr=_fcn_clz(d)-_fcn_clz(n);

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
		s=(int)(d-r-1)>>31;
		c=s&1;
		r-=d&s;
	}
	q=(q<<1)|c;
	return(q);
}

s32 __sdivsi3(s32 a, s32 b)
{
	s32 sga, sgb;
	sga=a>>31;		sgb=b>>31;
	a=(a^sga)-sga;	b=(b^sgb)-sgb;
	sga^=sgb;
	return((__udivsi3(a, b)^sga)-sga);
}

