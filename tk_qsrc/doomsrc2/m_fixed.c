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


static const char
rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#include "stdlib.h"

#include "doomtype.h"
#include "i_system.h"

#ifdef __GNUG__
#pragma implementation "m_fixed.h"
#endif
#include "m_fixed.h"




long long __smullq(int a, int b);
long long __int32_dmuls(int a, int b);
u64 __int32_dmulu(u32 a, u32 b);

// Fixme. __USE_C_FIXED__ or something.

#ifdef __BJX2__
// #if 0

fixed_t FixedMul(fixed_t a, fixed_t b);

__asm {
FixedMul:
	DMULS.L		R4, R5, R6
	SHAD.Q		R6, -16, R7
	EXTS.L		R7, R2
	RTS
};

#else

fixed_t FixedMul(fixed_t a, fixed_t b)
{
	int c;
	c = ((long long) a * (long long) b) >> FRACBITS;
//	c = __smullq(a, b) >> FRACBITS;
//	c = __int32_dmuls(a, b) >> FRACBITS;
	c = (int)c;
	return(c);
//	return ((long long) a * (long long) b) >> FRACBITS;
}

#endif


//
// FixedDiv, C version.
//

fixed_t
FixedDiv
( fixed_t	a,
  fixed_t	b )
{
	if ( (abs(a)>>14) >= abs(b))
		return ((a^b)<0) ? MININT : MAXINT;
	return FixedDiv2 (a,b);
}

fixed_t
FixedDivSoft
( fixed_t	a,
  fixed_t	b )
{
	long long c;
	int r;

	if ( (abs(a)>>14) >= abs(b))
		return ((a^b)<0) ? MININT : MAXINT;
//	return FixedDiv2 (a,b);

	r=M_SoftDivRcpS(b);
#ifdef __BJX2__
//	c=__smullq(a, r);
	c = __int32_dmuls(a, r);
#else
	c = ((long long)a) * ((long long)r);
#endif
	return (fixed_t)(c>>16);
}

s32 M_SoftDivRcpS(s32 b);

fixed_t
FixedDiv2
( fixed_t	a,
  fixed_t	b )
{
#if 1
	long long c;
	c = ((long long)a<<16) / ((long long)b);
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

//	c = ((double)a) / ((double)b) * FRACUNIT;
	c = (((double)a) * FRACUNIT) / ((double)b);

	if (c >= 2147483648.0 || c < -2147483648.0)
		I_Error("FixedDiv: divide by zero");
	return (fixed_t) c;
#endif
}


// static u32 m_softdiv_rcptab[4096];
static u32 m_softdiv_rcptab[1024];
static int m_softdiv_init=0;

int M_InitSoftDiv()
{
	unsigned int uj;
	int i, j;

	if(!m_softdiv_init)
	{
		m_softdiv_init=1;
//		for(i=1; i<4096; i++)
		for(i=1; i<1024; i++)
		{
//			j=0x7fffffff / i;
//			j=(0x7fffffffULL + (i-1)) / i;
//			m_softdiv_rcptab[i] = j<<1;

			uj=(0x100000000ULL + (i-1)) / i;
//			uj=0x100000000ULL / i;
			m_softdiv_rcptab[i] = uj;
		}
	}
	
	return(0);
}

u32 M_SoftDivU(u32 a, u32 b)
{
	u64 ta, tb, tc;
	u32 c;

//	if(!b)return(0);
	
//	if(b<4096)
	if(b<1024)
	{
		ta=(u64)a;
		tb=m_softdiv_rcptab[b];
//		tc=ta*tb;
		tc = __int32_dmulu(a, tb);
		c=(u32)(tc>>32);
//		c=(((u64)a)*(m_softdiv_rcptab[b]))>>32;
		return(c);
	}

	tb=M_SoftDivRcp(b);
	tc = __int32_dmulu(a, tb);
	c=(u32)(tc>>32);
	return(c);

#if 0
	if(b<0x10000)
	{
		tb=m_softdiv_rcptab[b>>4]>>4;
		ta=(u64)a;
		tc=ta*tb;
		c=(u32)(tc>>32);
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

	if(b<0x1000000)
	{
		tb=m_softdiv_rcptab[b>>12]>>12;
		ta=(u64)a;
		tc=ta*tb;
		c=(u32)(tc>>32);
		return(c);
	}

	if(b<0x10000000)
	{
		tb=m_softdiv_rcptab[b>>16]>>16;
		ta=(u64)a;
		tc=ta*tb;
		c=(u32)(tc>>32);
		return(c);
	}

	tb=m_softdiv_rcptab[b>>20]>>20;
	ta=(u64)a;
//	tc=ta*tb;
	tc = __int32_dmulu(a, tb);
	c=(u32)(tc>>32);
	return(c);
#endif

//	c=a/b;
//	return(c);
}

u32 M_SoftDivU2(u32 a, u32 b)
{
	u32 c, d, e;
	
	c=M_SoftDivU(a, b);

#if 0
//	L0:
	e=a-(c*b);
	if(((s32)e)<0)
		c--;
//		{ c--; goto L0; }
	if(e>=b)
		c++;
//		{ c++; goto L0; }
#endif

	e=a-(c*b);
	while(((s32)e)<0)
		{ c--; e=a-(c*b); }
	while(e>=b)
		{ c++; e=a-(c*b); }

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
	u32 ix, shr;
	u32 c;

#if 0
	if(b<0x100)
	{
		c=m_softdiv_rcptab[b];
		return(c);
	}

	if(b<0x1000)
	{
		c=m_softdiv_rcptab[b>>4]>>4;
		return(c);
	}

	if(b<0x10000)
	{
		c=m_softdiv_rcptab[b>>8]>>8;
		return(c);
	}

	if(b<0x100000)
	{
		c=m_softdiv_rcptab[b>>12]>>12;
		return(c);
	}

	if(b<0x1000000)
	{
		c=m_softdiv_rcptab[b>>16]>>16;
		return(c);
	}

	if(b<0x10000000)
	{
		c=m_softdiv_rcptab[b>>20]>>20;
		return(c);
	}

	c=m_softdiv_rcptab[b>>24]>>24;
	return(c);
#endif

#if 1
//	if(b<4096)
	if(b<1024)
	{
		c=m_softdiv_rcptab[b];
		return(c);
	}

	ix=b; shr=0;
//	while(ix>=65536)
	while(ix>=16384)
		{ ix>>=4; shr+=4; }
//	while(ix>=4096)
	while(ix>=1024)
		{ ix>>=1; shr++; }
	c=m_softdiv_rcptab[ix]>>shr;
	return(c);
#endif

#if 0
	if(b<4096)
	{
		c=m_softdiv_rcptab[b];
		return(c);
	}

	if(b<0x10000)
	{
		c=m_softdiv_rcptab[b>>4]>>4;
		return(c);
	}

	if(b<0x100000)
	{
		c=m_softdiv_rcptab[b>>8]>>8;
		return(c);
	}

	if(b<0x1000000)
	{
		c=m_softdiv_rcptab[b>>12]>>12;
		return(c);
	}

	if(b<0x10000000)
	{
		c=m_softdiv_rcptab[b>>16]>>16;
		return(c);
	}

	c=m_softdiv_rcptab[b>>20]>>20;
	return(c);
#endif

//    c = (0x7fffffffu / b)<<1;
//	return(c);
}

s32 M_SoftDivRcpS(s32 b)
{
	if(b<0)
	{
		return(-M_SoftDivRcp(-b));
	}
	return(M_SoftDivRcp(b));
}
