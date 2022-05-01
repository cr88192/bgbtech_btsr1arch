__m64 __bcd64_add(__m64 a, __m64 b);
__m64 __bcd64_sub(__m64 a, __m64 b);
__m64 __bcd64_mul(__m64 a, __m64 b);
__m64 __bcd64_div(__m64 a, __m64 b);
__m64 __bcd64_mod(__m64 a, __m64 b);

__m64 __bcd64_fromint(long long a);
long long __bcd64_toint(__m64 a);

#if 1
u64 tk_bcd64_fromint_asm(long long val);

__asm {
tk_bcd64_fromint_asm:
.ifarch has_bcd
	MOV 0, R2
	MOV 64, R7
	.L0:
	ADD -8, R7

	ROTCL	R4
	BCDADC	R2, R2
	ROTCL	R4
	BCDADC	R2, R2
	ROTCL	R4
	BCDADC	R2, R2
	ROTCL	R4
	BCDADC	R2, R2

	ROTCL	R4
	BCDADC	R2, R2
	ROTCL	R4
	BCDADC	R2, R2
	ROTCL	R4
	BCDADC	R2, R2
	ROTCL	R4
	BCDADC	R2, R2

	TEST R7, R7
	BF .L0
	RTS
.else
	MOV	-1, R2
	RTS	
.endif
}
#endif

#if 1
__m64 __bcd64_fromint(long long a)
{
	__m64 c;
	long long li;
	u64 tc, t0;
	int i, j;
	
	li=tk_bcd64_fromint_asm(a);
	if(li!=-1)
	{
		c=(__m64)li;
		return(c);
	}
	
	tc=0;
	li=a;
	for(i=0; i<16; i++)
	{
		t0=li%10;
		li=li/10;
		tc|=t0<<(i*4);
	}
	c=(__m64)tc;
	return(c);
}

long long __bcd64_toint(__m64 a)
{
	long long li;
	u64 tc, t0;
	int i, j;

	li=0;
	tc=(u64)a;
	for(i=15; i>=0; i--)
	{
		li=(li<<3)+(li<<1);
		t0=(tc>>(i*4))&15;
//		li=(li*10)+t0;
		li=li+t0;
	}
	return(li);
}

__m64 __bcd64_add(__m64 a, __m64 b)
{
	__m64 c;
	u64 ta, tb, tc, t0;
	int i, j, k;
	
	ta=(u64)a;
	tb=(u64)b;
	tc=0;

//	__debugbreak();

	for(i=0; i<16; i++)
	{
		t0=((ta>>(i*4))&15)+((tb>>(i*4))&15);
		if(t0>9)t0+=6;
		tc+=t0<<(i*4);
	}
	c=(__m64)tc;
//	__debugbreak();
	return(c);
}

__m64 __bcd64_sub(__m64 a, __m64 b)
{
	__m64 b1;
	u64 tb;

	tb=(u64)b;
	tb=(0x9999999999999999ULL-tb)+1;
	b1=(__m64)tb;
	return(__bcd64_add(a, b1));
}

__m64 __bcd64_mul(__m64 a, __m64 b)
{
	long long la, lb, lc;
	la=__bcd64_toint(a);
	lb=__bcd64_toint(b);
	lc=la*lb;
	return(__bcd64_fromint(lc));
}

__m64 __bcd64_div(__m64 a, __m64 b)
{
	long long la, lb, lc;
	la=__bcd64_toint(a);
	lb=__bcd64_toint(b);
	lc=la/lb;
	return(__bcd64_fromint(lc));
}

__m64 __bcd64_mod(__m64 a, __m64 b)
{
	long long la, lb, lc;
	la=__bcd64_toint(a);
	lb=__bcd64_toint(b);
	lc=la%lb;
	return(__bcd64_fromint(lc));
}
#endif


__m128 __bcd128_add(__m128 a, __m128 b);
__m128 __bcd128_sub(__m128 a, __m128 b);
__m128 __bcd128_mul(__m128 a, __m128 b);
__m128 __bcd128_div(__m128 a, __m128 b);
__m128 __bcd128_mod(__m128 a, __m128 b);

__m128 __bcd128_fromint(long long a);
long long __bcd128_toint(__m128 a);

__m128 __bcd128_fromint128(__int128 a);
__int128 __bcd128_toint128(__m128 a);


#if 1
__m128 __bcd128_fromint(long long a)
{
	__m128 c;
	long long li;
	u128 tc, t0;
	int i, j;
	
	tc=0;
	li=a;
	for(i=0; i<32; i++)
	{
		t0=li%10;
		li=li/10;
		tc|=t0<<(i*4);
	}
	c=(__m128)tc;
	return(c);
}

long long __bcd128_toint(__m128 a)
{
	long long li;
	u128 tc, t0;
	int i, j;

	li=0;
	tc=(u128)a;
	for(i=31; i>=0; i--)
	{
		t0=(tc>>(i*4))&15;
		li=(li*10)+t0;
	}
	return(li);
}

__m128 __bcd128_fromint128(__int128 a)
{
	__m128 c;
	__int128 li;
	u128 tc, t0;
	int i, j;
	
	tc=0;
	li=a;
	for(i=0; i<32; i++)
	{
		t0=li%10;
		li=li/10;
		tc|=t0<<(i*4);
	}
	c=(__m128)tc;
	return(c);
}

__int128 __bcd128_toint128(__m128 a)
{
	__int128 li;
	u128 tc, t0;
	int i, j;

	li=0;
	tc=(u128)a;
	for(i=31; i>=0; i--)
	{
		t0=(tc>>(i*4))&15;
		li=(li*10)+t0;
	}
	return(li);
}

__m128 __bcd128_add(__m128 a, __m128 b)
{
	__m128 c;
	u128 ta, tb, tc, t0;
	int i, j, k;
	
	ta=(u128)a;
	tb=(u128)b;
	tc=0;
	for(i=0; i<32; i++)
	{
		t0=((ta>>(i*4))&15)+((tb>>(i*4))&15);
		if(t0>9)t0+=6;
		tc+=t0<<(i*4);
	}
	c=(__m128)tc;
	return(c);
}

__m128 __bcd128_sub(__m128 a, __m128 b)
{
	__m128 b1;
	u128 tb;

	tb=(u128)b;
	tb=(0x99999999999999999999999999999999UI128-tb)+1;
	b1=(__m128)tb;
	return(__bcd128_add(a, b1));
}

__m128 __bcd128_mul(__m128 a, __m128 b)
{
	__int128 la, lb, lc;
	la=__bcd128_toint128(a);
	lb=__bcd128_toint128(b);
	lc=la*lb;
	return(__bcd128_fromint128(lc));
}

__m128 __bcd128_div(__m128 a, __m128 b)
{
	__int128 la, lb, lc;
	la=__bcd128_toint128(a);
	lb=__bcd128_toint128(b);
	lc=la/lb;
	return(__bcd128_fromint128(lc));
}

__m128 __bcd128_mod(__m128 a, __m128 b)
{
	__int128 la, lb, lc;
	la=__bcd128_toint128(a);
	lb=__bcd128_toint128(b);
	lc=la%lb;
	return(__bcd128_fromint128(lc));
}

#endif
