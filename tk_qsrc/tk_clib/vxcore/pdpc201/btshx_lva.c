/*
64-bit tag value, consisting of a low and high part.

High 4 bits:
  0  : Object Pointer
  1  : Reduced tag space
  2-3: Resv
  4-7: Fixlong
  8-B: Flonum
  C  : TagArray
  D  : Vec2f
  E  : TT Pointer (48 bit)
  F  : Raw Pointer (60 bit)
  
  Reduced Tag Space:
  10-17: 56-bit spaces
  1800-1BFF: 48 bit spaces
  1C000000-1DFFFFFF: 32-bit spaces

  10: RotLong
  11: FComplex

Object Pointer:
  Low 48 bits: Address
  Bits 48-59: Type ID

TagArray
  Low 32 bits: Array Object ID
  Bits 32-59: Base Offset

RotLong
   0-47: Value Bits
  48-53: Rot
  54/55: Fill Bits
Stores a long value, where 48 bits are represented directly.
The fill bits are repeated to create a fill pattern for the remaining 16 bits.
The value is then rotated left N bits to get the desired value.

Vec2f
   0-29: X
  30-59: Y
Consists of a pair of floating-point values (X and Y).
These are stored at 30 bits each.


TT Pointer:
  Low 48 bits: Base Address
  Bits 48-59: Type Tag

The TT Pointer will represent a raw "C style" pointer.

Type Tag 0 will be special, and refer to an untagged "void *" pointer.
	Other type-tags will identify types.
	Part of the range will be for fixed/primitive types.
	The rest will be for registered types.

The type-tag for a TT Pointer will primarily serve as a hint for cases where a pointer is cast to variant. A raw pointer which has not neen cast to variant will not necessarily be required to have a valid type-tag.

*/

#if 1
int __lva_fixnump(u64 val)
{
	if((val>>62)==1)
		return(1);
	return(0);
}

int __lva_flonump(u64 val)
{
	if((val>>62)==2)
		return(1);
	return(0);
}

s32 __lva_conv_toi32(u64 val)
{
	double f;

	if((val>>62)==1)
	{
		return((s32)val);
	}

	if((val>>62)==2)
	{
		*(u64 *)(&f)=(val<<2);
		return((s32)(f));
	}
	
	return(-1);
}

s64 __lva_conv_toi64(u64 val)
{
	s64 iv;
	double f;

	if((val>>62)==1)
	{
		iv=val<<2;
		return(iv>>2);
	}

	if((val>>62)==2)
	{
		*(u64 *)(&f)=(val<<2);
		return((s64)(f));
	}
	
	return(-1);
}

float __lva_conv_tof32(u64 val)
{
	double f;

	if((val>>62)==1)
	{
		return((float)((s32)val));
	}

	if((val>>62)==2)
	{
		*(u64 *)(&f)=(val<<2);
		return(f);
	}
	
	return(-1);
}

double __lva_conv_tof64(u64 val)
{
	double f;

	if((val>>62)==1)
	{
		return((double)((s32)val));
	}

	if((val>>62)==2)
	{
		*(u64 *)(&f)=(val<<2);
		return(f);
	}
	
	return(-1);
}

void *__lva_conv_toptr(u64 val)
{
	s64 iv;
	double f;

	if((val>>60)==0)
	{
// #ifdef __BJX1_64__
#ifdef __ADDR_64__
		iv=val<<16;
		return((void *)(iv>>16));
#else
		return((void *)((u32)iv));
#endif
	}

	if((val>>60)==15)
	{
//#ifdef __BJX1_64__
#ifdef __ADDR_64__
		iv=val<<4;
		return((void *)(iv>>4));
#else
		return((void *)((u32)iv));
#endif
	}

	if((val>>62)==1)
	{
// #ifdef __BJX1_64__
#ifdef __ADDR_64__
		iv=val<<2;
		return((void *)(iv>>2));
#else
		return((void *)((u32)iv));
#endif
	}

	if((val>>62)==2)
	{
		return(NULL);
	}
	
	return(NULL);
}

u64 __lva_conv_fromi32(s32 val)
{
	u64 v;
	
	tk_printf("__lva_conv_fromi32 A %X\n", val);
	
	v=((s64)val);
	v&=0x3FFFFFFFFFFFFFFFULL;
	v|=0x4000000000000000ULL;

	tk_printf("__lva_conv_fromi32 B %8X_%8X\n",
		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromi64(s64 val)
{
	u64 v;

	tk_printf("__lva_conv_fromi64 %X_%X\n", (int)(val>>32), (int)val);
	
	v=((s64)val);
	v&=0x3FFFFFFFFFFFFFFFULL;
	v|=0x4000000000000000ULL;

	tk_printf("__lva_conv_fromi64 B %8X_%8X\n",
		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromf32(float val)
{
	double f;
	u64 v;

	tk_printf("__lva_conv_fromf32 A %f\n", val);
	
	f=val;
	v=(*(u64 *)(&f));
	v=v>>2;
	v|=0x8000000000000000ULL;

	tk_printf("__lva_conv_fromf32 B %8X_%8X\n",
		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromf64(double val)
{
	double f;
	u64 v;

	tk_printf("__lva_conv_fromf64 A %f\n", val);

	f=val;
	v=(*(u64 *)(&f));

//	*((int *)-1)=-1;

	tk_printf("__lva_conv_fromf64 B %8X_%8X\n",
		(int)(v>>32), (int)v);

	v=v>>2;
	v|=0x8000000000000000ULL;

	tk_printf("__lva_conv_fromf64 C %8X_%8X\n",
		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromptr(void *val)
{
	u64 v;	

// #ifdef __BJX1_64__
#ifdef __ADDR_64__
	v=(u64)val;
	v=v<<16;
	v=v>>16;
	v|=TKMM_LVA_GetPtrTypeTag(val);
	return(v);
#else
	v=(u64)((u32)val);
	v|=TKMM_LVA_GetPtrTypeTag(val);
	return(v);
#endif
}

int tkmm_lvatyi_string;

u64 __lva_conv_fromstr(char *val)
{
	return(TKMM_LVA_WrapString(val));
}

u64 __lva_add(u64 arga, u64 argb)
{
	u64 v;
	s64 li, lj, lk;
	double lf, lg, lh;

	if(__lva_fixnump(arga))
	{
		if(__lva_fixnump(argb))
		{
			li=__lva_conv_toi64(arga);
			lj=__lva_conv_toi64(argb);
			lk=li+lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
		
		if(__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf+lg;
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}

	if(__lva_flonump(arga))
	{
		if(__lva_fixnump(argb) ||
			__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf+lg;
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}
}

u64 __lva_sub(u64 arga, u64 argb)
{
	u64 v;
	s64 li, lj, lk;
	double lf, lg, lh;

	if(__lva_fixnump(arga))
	{
		if(__lva_fixnump(argb))
		{
			li=__lva_conv_toi64(arga);
			lj=__lva_conv_toi64(argb);
			lk=li-lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
		
		if(__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf-lg;
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}

	if(__lva_flonump(arga))
	{
		if(__lva_fixnump(argb) ||
			__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf-lg;
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}
}

#endif
