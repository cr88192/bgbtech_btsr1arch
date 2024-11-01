// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Fixed point implementation.
//
//-----------------------------------------------------------------------------


#include <stdlib.h>
#include <math.h>
#include "rt_def.h"

// #include "doomtype.h"
// #include "i_system.h"

// #include "m_fixed.h"

fixed_t FixedMul	(fixed_t a, fixed_t b);
fixed_t FixedDiv	(fixed_t a, fixed_t b);
fixed_t FixedDiv2	(fixed_t a, fixed_t b);



long long __smullq(int a, int b);
long long __int32_dmuls(int a, int b);

#ifdef _WIN32
// #if 0
long long __int32_dmuls(int a, int b)
{
	return(((long long)a) * ((long long)b));
}
#endif

// Fixme. __USE_C_FIXED__ or something.

fixed_t
FixedMul
( fixed_t	a,
  fixed_t	b )
{
	int c;
//	c = ((long long) a * (long long) b) >> SFRACBITS;
//	c = __smullq(a, b) >> SFRACBITS;
	c = __int32_dmuls(a, b) >> SFRACBITS;
	c = (fixed_t)c;
	return(c);
//	return ((long long) a * (long long) b) >> SFRACBITS;
}

fixed_t
FixedMulShift
( fixed_t	a,
  fixed_t	b,
  int shr )
{
	int c;
//	c = ((long long) a * (long long) b) >> shr;
//	c = __int32_dmuls(a, b) >> shr;
	c = __int32_dmuls(a, b) >> (shr&63);
	c = (fixed_t)c;
//	__debugbreak();
	return(c);
}


//
// FixedDiv, C version.
//

fixed_t
FixedDiv
( fixed_t	a,
  fixed_t	b )
{
	if ( (rt_abs(a)>>14) >= rt_abs(b))
		return ((a^b)<0) ? MININT : MAXINT;
	return FixedDiv2 (a,b);
}

fixed FixedScale(fixed orig, fixed factor, fixed divisor)
{
	return(FixedDiv(FixedMul(orig, factor), divisor));
}

double sqrt_appx(double val)
{
	double f, g;
	u64 v;

	if(val==0)
		return(0.0);

	v=0; f=0;
	memcpy(&v, &val, 8);
	v=0x1FF8000000000000ULL+(v>>1);
	memcpy(&f, &v, 8);
	
	g=f*f;	f=f*(1.0+(val-g)*0.5);
	g=f*f;	f=f*(1.0+(val-g)*0.5);
	g=f*f;	f=f*(1.0+(val-g)*0.5);
	g=f*f;	f=f*(1.0+(val-g)*0.5);
	g=f*f;	f=f*(1.0+(val-g)*0.5);
	
	return(f);
}

fixed_t FixedSqrtHP(fixed_t val)
{
	fixed v;
	
	v=sqrt(val/65536.0)*65536.0;
//	v=sqrt_appx(val/65536.0)*65536.0;
	return(v);
}

fixed_t FixedSqrtLP(fixed_t val)
{
	fixed v;
	
	v=sqrt(val/256.0)*256.0;
//	v=sqrt_appx(val/256.0)*256.0;
	return(v);
}


s32 M_SoftDivRcpS(s32 b);

fixed_t
FixedDiv2
( fixed_t	a,
  fixed_t	b )
{
#if 1
	long long ta, tb, tc, c;
	int sg;
	
	sg=0;
	ta=a;
	tb=b;
	if(ta<0)
		{ ta=-ta; sg=!sg; }
	if(tb<0)
		{ tb=-tb; sg=!sg; }
	
	tc = (ta<<16) / (tb);
	
	c=tc;
	if(sg)
		c=-c;
	
	return (fixed_t) c;
#endif

#if 0
	long long c;
	c = (((long long)a)<<16) / ((long long)b);
	return (fixed_t) c;
#endif

#if 0
	long long c;
	int r;
	r=M_SoftDivRcpS(b);
	c=__smullq(a, r>>16);
	return (fixed_t)(c>>32);
#endif

#if 0
	double c;

//	c = ((double)a) / ((double)b) * SFRACUNIT;
	c = (((double)a) * SFRACUNIT) / ((double)b);

	if (c >= 2147483648.0 || c < -2147483648.0)
	I_Error("FixedDiv: divide by zero");
	return (fixed_t) c;
#endif
}


static u32 m_softdiv_rcptab[4096];
static int m_softdiv_init=0;

int M_InitSoftDiv()
{
	int i, j;

	if(!m_softdiv_init)
	{
		m_softdiv_init=1;
		for(i=1; i<4096; i++)
		{
			j=0x7fffffff / i;
			m_softdiv_rcptab[i] = j<<1;
		}
	}
	
	return(0);
}

u32 M_SoftDivU(u32 a, u32 b)
{
	u64 ta, tb, tc;
	u32 c;

//	if(!b)return(0);
	
	if(b<4096)
	{
		ta=(u64)a;
		tb=m_softdiv_rcptab[b];
		tc=ta*tb;
		c=(u32)(tc>>32);
//		c=(((u64)a)*(m_softdiv_rcptab[b]))>>32;
		return(c);
	}

	if(b<0x100000)
	{
		tb=m_softdiv_rcptab[b>>8]>>8;
		ta=(u64)a;
		tc=ta*tb;
		c=(u32)(tc>>32);
		return(c);
	}

	c=a/b;
	return(c);
}

s32 M_SoftDivS(s32 a, s32 b)
{
	if((a|b)<0)
	{
		if(a<0)
		{
			if(b<0)
				{ return( M_SoftDivU(-a, -b)); }
			else
				{ return(-M_SoftDivU(-a,  b)); }
		}else
		{
			if(b<0)
				{ return(-M_SoftDivU(a, -b)); }
			else
				{ return( M_SoftDivU(a,  b)); }
		}
	}

	return(M_SoftDivU(a, b));
}

u32 M_SoftDivRcp(u32 b)
{
	u32 c;

	if(b<4096)
	{
		c=m_softdiv_rcptab[b];
		return(c);
	}

	if(b<0x100000)
	{
		c=m_softdiv_rcptab[b>>8]>>8;
		return(c);
	}

    c = (0x7fffffffu / b)<<1;
	return(c);
}

s32 M_SoftDivRcpS(s32 b)
{
	if(b<0)
	{
		return(-M_SoftDivRcp(-b));
	}
	return(M_SoftDivRcp(b));
}
