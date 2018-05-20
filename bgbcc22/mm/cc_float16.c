#include <bgbccc.h>

u32 bgbcc_float16_mantissatable[2048];
u32 bgbcc_float16_exponenttable[64];
short bgbcc_float16_offsettable[64];

u16 bgbcc_float16_basetable[512];
byte bgbcc_float16_shifttable[512];

u32 BGBCC_Float16_ConvertMantissa(u32 i)
{
	u32 m;
	u32 e;
	
	if(!i)return(0);
	
	e=0; m=i<<13;
	while(!(m&0x00800000U))
	{
		e-=0x00800000U;
		m<<=1;
	}
	m&=~0x00800000U;
	e+=0x38800000U;
	return(m|e);
}

int BGBCC_Float16_Init()
{
	static int init=0;
	int i, e;

	if(init)return(0);
	init=1;

	for(i=0; i<1024; i++)
		{ bgbcc_float16_mantissatable[i]=BGBCC_Float16_ConvertMantissa(i); }
	for(i=1024; i<2048; i++)
		{ bgbcc_float16_mantissatable[i]=0x38000000U+((i-1024)<<13); }
	for(i=0; i<31; i++)
		{ bgbcc_float16_exponenttable[i]=0x00000000U+(i<<23); }
	for(i=32; i<63; i++)
		{ bgbcc_float16_exponenttable[i]=0x80000000U+((i-32)<<23); }

//	bgbcc_float16_exponenttable[31]=0x47800000U;
//	bgbcc_float16_exponenttable[63]=0xC7800000U;

	bgbcc_float16_exponenttable[31]=0x7F800000U;	//+ NaN/Inf
	bgbcc_float16_exponenttable[63]=0xFF800000U;	//- NaN/Inf

	for(i=1; i<32; i++)
	{
		bgbcc_float16_offsettable[i]=1024;
		bgbcc_float16_offsettable[i+32]=1024;
	}

	bgbcc_float16_offsettable[0]=0;
	bgbcc_float16_offsettable[32]=0;

	for(i=0; i<256; i++)
	{
		e=i-127;
		if(e<-24)	//zeroes
		{
			bgbcc_float16_basetable[i|0x000]=0x0000;	//-0
			bgbcc_float16_basetable[i|0x100]=0x8000;	//+0
			bgbcc_float16_shifttable[i|0x000]=24;
			bgbcc_float16_shifttable[i|0x100]=24;
		}
		else if(e<-14)	//denormals
		{
			bgbcc_float16_basetable[i|0x000]=(0x0400>>(-e-14))|0x0000;
			bgbcc_float16_basetable[i|0x100]=(0x0400>>(-e-14))|0x8000;
			bgbcc_float16_shifttable[i|0x000]=-e-1;
			bgbcc_float16_shifttable[i|0x100]=-e-1;
		}
		else if(e<=15) //normal range
		{
			bgbcc_float16_basetable[i|0x000]=((e+15)<<10)|0x0000;
			bgbcc_float16_basetable[i|0x100]=((e+15)<<10)|0x8000;
			bgbcc_float16_shifttable[i|0x000]=13;
			bgbcc_float16_shifttable[i|0x100]=13;
		}
		else if(e<128)	//overflow to Inf
		{
			bgbcc_float16_basetable[i|0x000]=0x7C00;
			bgbcc_float16_basetable[i|0x100]=0xFC00;
			bgbcc_float16_shifttable[i|0x000]=24;
			bgbcc_float16_shifttable[i|0x100]=24;
		}
		else	//Inf and NaN
		{
			bgbcc_float16_basetable[i|0x000]=0x7C00;
			bgbcc_float16_basetable[i|0x100]=0xFC00;
			bgbcc_float16_shifttable[i|0x000]=13;
			bgbcc_float16_shifttable[i|0x100]=13;
		}
	}
	
	return(1);
}

double BGBCC_DecodeFloat16(u16 val)
{
	u32 f, h;
	h=val;
	f=bgbcc_float16_mantissatable[
		bgbcc_float16_offsettable[h>>10]+(h&0x3FF)]+
		bgbcc_float16_exponenttable[h>>10];
	return(*(float *)(&f));
}

u16 BGBCC_EncodeFloat16(double val)
{
	u16 t;
	u32 f;
	int i;
	
	*(float *)(&f)=val;
	f=f+4095+((f>>13)&1);
	i=(f>>23)&0x1FF;

	t=bgbcc_float16_basetable[i]+
		((f&0x007FFFFF)>>bgbcc_float16_shifttable[i]);
	return(t);
}

double BGBCC_DecodeFloat16F(u16 val)
{
	u32 f;
	f=bgbcc_float16_exponenttable[val>>10]|((val&0x3FF)<<13);
	return(*(float *)(&f));
}

double BGBCC_DecodeFloat12(u16 val)
{
	return(BGBCC_DecodeFloat16(val<<4));
}

u16 BGBCC_EncodeFloat12(double val)
{
	int i, j;
	
	i=BGBCC_EncodeFloat16(val);
	j=i+7+((i&15)>>1);
	return(j);
	
//	return((BGBCC_EncodeFloat16(val)+7)>>4);
}

u16 BGBCC_NegFloat16(u16 x)
{
	return(x^0x8000);
}

u16 BGBCC_RcpFloat16(u16 x)
{
	int z;
//	z=0x3C00-(x&0x7FFF);
	z=0x7800-(x&0x7FFF);
	if(z<0)return(0);
	z=(x&0x8000)|z;
	return(z);
}

u16 BGBCC_SqrtFloat16(u16 x)
{
	int z;
	z=((x-0x3C00)>>1)+0x3C00;
	return(z);
}

u16 BGBCC_AddFloat16(u16 x, u16 y)
{
	int ux, uy, z, k, d;
	if((x^y)&0x8000)
	{
		ux=x&0x7FFF;
		uy=y&0x7FFF;
		if(ux>=uy)
		{
			d=(ux>>10)-(uy>>10);
			k=((y&0x03FF)|0x0400)>>d;
			z=x-k;
			if((x^z)&0x8000)
				{ z^=0x7FFF; }
		}else
		{
			d=(uy>>10)-(ux>>10);
			k=((x&0x03FF)|0x0400)>>d;
			z=y-k;
			if((y^z)&0x8000)
				{ z^=0x7FFF; }
		}
	}else
	{
		if(x>=y)
		{
			d=(x>>10)-(y>>10);
			k=((y&0x03FF)|0x0400)>>d;
			z=x+k;
			if((x^z)&0x8000)
				{ z=(x&0x8000)|0x7C00; }
		}else
		{
			d=(y>>10)-(x>>10);
			k=((x&0x03FF)|0x0400)>>d;
			z=y+k;
			if((y^z)&0x8000)
				{ z=(x&0x8000)|0x7C00; }
		}
	}
	return(z);
}

#if 0
u16 BGBCC_AddFloat16F(u16 x, u16 y)
{
	int ux, uy, z, z0, z1, z2, z3;
	int k0, k1, k2, d0, d1;
	int m0, m1, m2, m3;
	int n0, n1, n2, n3;

	ux=x&0x7FFF;	uy=y&0x7FFF;
	m0=(((int)(x^y))<<16)>>31;
	m1=(uy-ux)>>31;
	n0=~m0;	n1=~m1;
	d0=(ux>>10)-(uy>>10); d1=-d0;
	k0=((y&0x03FF)|0x0400)>>d0;
	k1=((x&0x03FF)|0x0400)>>d1;
	k2=((x&0x8000)|0x7C00);
	z0=x-k0; z1=y-k1;
	z2=x+k0; z3=y+k1;

	m2=(((z2^x)<<16)>>31);
	m3=(((z3^y)<<16)>>31);
	n2=~m2;	n3=~m3;

	z0=z0^((z0>>31)&0x7FFF);
	z1=z1^((z1>>31)&0x7FFF);
	z2=(z2&n2)|(m2&k2);
	z3=(z3&n3)|(m3&k2);

	z0=(z0&m1)|(z1&n1);
	z1=(z2&m1)|(z3&n1);
	z=(z0&m0)|(z1&n0);
	return(z);
}
#endif

#if 1
u16 BGBCC_AddFloat16F(u16 x, u16 y)
{
	int ux, uy, z, z0, z1, z2, z3;
	int k0, k1, k2, d0, d1;
	int m0, m1, m2, m3;
	int n0, n1, n2, n3;

	ux=x&0x7FFF;	uy=y&0x7FFF;
	m0=(((int)(x^y))<<16)>>31;
	m1=(uy-ux)>>31;
	n0=~m0;	n1=~m1;
	d0=(ux>>10)-(uy>>10); d1=-d0;
	k0=((y&0x03FF)|0x0400)>>d0;
	k1=((x&0x03FF)|0x0400)>>d1;
	z0=x-k0; z1=y-k1;
	z2=x+k0; z3=y+k1;

	z0=z0^((z0>>31)&0x7FFF);
	z1=z1^((z1>>31)&0x7FFF);

	z0=(z0&m1)|(z1&n1);
	z1=(z2&m1)|(z3&n1);
	z=(z0&m0)|(z1&n0);
	return(z);
}
#endif

u16 BGBCC_SubFloat16(u16 x, u16 y)
{
	return(BGBCC_AddFloat16(x, y^0x8000));
}

u16 BGBCC_MulFloat16(u16 x, u16 y)
{
	int ux, uy, z, k, d;

	ux=x&0x7FFF;
	uy=y&0x7FFF;
	z=ux+uy-0x3C00;
	if(z&0x8000)
	{
		if(z<0)return(0);
		z=((x^y)&0x8000)|0x7C00;
	}else
	{
		z=((x^y)&0x8000)|z;
	}
	return(z);
}

u16 BGBCC_MulFloat16F(u16 x, u16 y)
{
	int ux, uy, z;

	ux=x&0x7FFF;
	uy=y&0x7FFF;
	z=ux+uy-0x3C00;
	z=z&(~((z<<16)>>31));
	z=((x^y)&0x8000)|z;
	return(z);
}

u32 BGBCC_MulFloat16F2P(u32 x, u32 y)
{
	int ux, uy, z;

	ux=x&0x7FFF7FFF;
	uy=y&0x7FFF7FFF;
	z=ux+uy-0x3C003C00;
	z=	(z&((~( z     >>31))&0x7FFF0000))|
		(z&((~((z<<16)>>31))&0x00007FFF)); 
	z=((x^y)&0x80008000)|z;
	return(z);
}

u64 BGBCC_MulFloat16F4P(u64 x, u64 y)
{
	s64 ux, uy, z;

	ux=x&0x7FFF7FFF7FFF7FFFLL;
	uy=y&0x7FFF7FFF7FFF7FFFLL;
	z=ux+uy-0x3C003C003C003C00LL;
	z=	(z&((~((z    )>>63))&0x7FFF000000000000LL))|
		(z&((~((z<<16)>>63))&0x00007FFF00000000LL))|
		(z&((~((z<<32)>>63))&0x000000007FFF0000LL))|
		(z&((~((z<<48)>>63))&0x0000000000007FFFLL)); 
	z=((x^y)&0x8000800080008000LL)|z;
	return(z);
}

u16 BGBCC_DivFloat16(u16 x, u16 y)
{
	return(BGBCC_MulFloat16(x, BGBCC_RcpFloat16(y)));
}

