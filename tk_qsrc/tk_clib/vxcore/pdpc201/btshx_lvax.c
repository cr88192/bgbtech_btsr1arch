/*
128-bit tag value, consisting of a low and high part.

ssss-vvvv-vvvv-vvvv-tttt-vvvv-vvvv-vvvv

tttt: Tag
ssss: Tag2

Tag, High 4 bits:
  0  : Object Pointer
  1  : Reduced tag space
  2-3: Bounded Pointer
  4  : Fixnum
  5  : Flonum
  6-7: Reserved
  8-B: Reserved
  C  : TagArray
  D  : Vec2f
  E  : Reserved
  F  : Reserved
  
Reduced Tag Space:
  10-17: 120-bit spaces
  1800-1BFF: 112-bit spaces
  1C000000-1DFFFFFF: 96-bit spaces

Flonum is either 

*/

s128 __lvax_conv_toi128_gen(u128 val);

#ifdef __BJX2__
// #if 0

int __lvax_fixnump(u128 val);
int __lvax_flonump(u128 val);

s128 __lvax_conv_toi128(u128 val);
u128 __lvax_conv_fromi128(s128 val);

s32 __lvax_conv_toi32(u128 val);
s64 __lvax_conv_toi64(u128 val);

u128 __lvax_conv_fromi32(s32 val);
u128 __lvax_conv_fromi64(s64 val);

__asm {
__lvax_fixnump:
	SHLD.Q	R4, -60, R7
	CMPEQ	4, R7
	MOVT	R2
	RTS

__lvax_flonump:
	SHLD.Q	R4, -60, R7
	CMPEQ	5, R7
	MOVT	R2
	RTS

__lvax_conv_toi128:
	SHLD.Q	R4, -60, R7
	CMPEQ	4, R7
	BF		.L0

	MOV		R5, R2
	SHLD.Q	R4, 4, R7
	SHAD.Q	R7, -4, R3
	RTS
	
	.L0:
	BRA		__lvax_conv_toi128_gen
	
__lvax_conv_fromi128:
	MOV		R4, R3
	MOVTT	R5, 8, R2
//	BREAK
	RTS

__lvax_conv_toi32:
	SHLD.Q	R4, -60, R7
	CMPEQ	4, R7
	BF		.L0
	EXTS.L	R5, R2
	RTS
	.L0:
	BRA		__lvax_conv_toi128_gen

__lvax_conv_toi64:
	SHLD.Q	R4, -60, R7
	CMPEQ	4, R7
	BF		.L0
	MOV		R5, R2
	RTS
	.L0:
	BRA		__lvax_conv_toi128_gen

__lvax_conv_fromi32:
__lvax_conv_fromi64:
	MOV		R4, R3
	SHAD.Q	R4, -63, R2
	MOVTT	R2, 8, R2
	RTS
};

#else

int __lvax_fixnump(u128 val)
{
	int tg;
	tg=(val>>60)&15;
	return(tg==4);
}

int __lvax_flonump(u128 val)
{
	int tg;
	tg=(val>>60)&15;
	return(tg==5);
}

s128 __lvax_conv_toi128(u128 val)
{
	u128 v1;
	int tg;

	tg=(val>>60)&15;
	if(tg==4)
	{
		v1=(val>>64)|(val<<64);
		v1=v1<<4;
		v1=((s128)v1)>>4;
		return(v1);
	}
	
	return(__lvax_conv_toi128_gen(val));
}

s32 __lvax_conv_toi32(u128 val)
{
	return(__lvax_conv_toi128(val));
}

s64 __lvax_conv_toi64(u128 val)
{
	return(__lvax_conv_toi128(val));
}

u128 __lvax_conv_fromi128(s128 val)
{
	u128 v0, v1;
	
	v0=(u128)val;
	v1=(v0<<64)|((v0>>64)&0x0FFFFFFFFFFFFFFFUI128);
	v1|=0x4000000000000000UI128;
	return(v1);
}

u128 __lvax_conv_fromi32(s32 val)
{
	return(__lvax_conv_fromi128(val));
}

u128 __lvax_conv_fromi64(s64 val)
{
	return(__lvax_conv_fromi128(val));
}
#endif

double __lvax_conv_tof64_gen(u128 val);

#if 1
double __lvax_conv_tof64(u128 val)
{
	u128 v1;
	__float128 fx;
	double fg;
	int tg;

	tg=(val>>60)&15;
	if(tg==5)
	{
		v1=(val>>64)|(val<<64);
		v1=v1<<4;
		*(u128 *)(&fx)=v1;
		fg=fx;
		return(fg);
	}
	
	return(__lvax_conv_tof64_gen(val));
}

float __lvax_conv_tof32(u128 val)
{
	return(__lvax_conv_tof64(val));
}


u128 __lvax_conv_fromf64(double val)
{
	__float128 fx;
	u128 v0, v1;
	fx=val;
	v0=*(u128 *)(&fx);
	v0=(v0>>4);
	v1=(v0<<64)|(v0>>64);
	v1|=0x5000000000000000UI128;
	return(v1);
}

u128 __lvax_conv_fromf32(float val)
{
	return(__lvax_conv_fromf64(val));
}

#endif

s128 __lvax_conv_toi128_gen(u128 val)
{
	s64 li;

	if(__lvax_fixnump(val))
		return(__lvax_conv_toi128(val));
	if(__lvax_flonump(val))
	{
		li=__lvax_conv_tof64(val);
		return(li);
	}
	return(0);
}

double __lvax_conv_tof64_gen(u128 val)
{
	s64 li;
	u128 v1;
	__float128 fx;
	double fg;
	int tg;

	tg=(val>>60)&15;
	if(tg==5)
	{
		v1=(val>>64)|(val<<64);
		v1=v1<<4;
		*(u128 *)(&fx)=v1;
		fg=fx;
		return(fg);
	}
	
	if(__lvax_fixnump(val))
	{
		li=__lvax_conv_toi128(val);
		return(li);
	}
	return(0);
}


u128 __lvax_add(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva+lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	if(__lvax_flonump(arga) || __lvax_fixnump(arga))
	{
		if(__lvax_flonump(argb) || __lvax_fixnump(argb))
		{
			fa=__lvax_conv_tof64(arga);
			fb=__lvax_conv_tof64(argb);
			fc=fa+fb;
			return(__lvax_conv_fromf64(fc));
		}
	}
	
	return(0);
}

u128 __lvax_sub(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva-lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	if(__lvax_flonump(arga) || __lvax_fixnump(arga))
	{
		if(__lvax_flonump(argb) || __lvax_fixnump(argb))
		{
			fa=__lvax_conv_tof64(arga);
			fb=__lvax_conv_tof64(argb);
			fc=fa-fb;
			return(__lvax_conv_fromf64(fc));
		}
	}
	
	return(0);
}

u128 __lvax_mul(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva*lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	if(__lvax_flonump(arga) || __lvax_fixnump(arga))
	{
		if(__lvax_flonump(argb) || __lvax_fixnump(argb))
		{
			fa=__lvax_conv_tof64(arga);
			fb=__lvax_conv_tof64(argb);
			fc=fa*fb;
			return(__lvax_conv_fromf64(fc));
		}
	}
	
	return(0);
}

u128 __lvax_div(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva/lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	if(__lvax_flonump(arga) || __lvax_fixnump(arga))
	{
		if(__lvax_flonump(argb) || __lvax_fixnump(argb))
		{
			fa=__lvax_conv_tof64(arga);
			fb=__lvax_conv_tof64(argb);
			fc=fa/fb;
			return(__lvax_conv_fromf64(fc));
		}
	}
	
	return(0);
}

u128 __lvax_mod(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva%lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	return(0);
}

u128 __lvax_and(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva&lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	return(0);
}

u128 __lvax_or(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva|lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	return(0);
}

u128 __lvax_xor(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			lvc=lva^lvb;
			return(__lvax_conv_fromi128(lvc));
		}
	}

	return(0);
}


int __lvax_cmp_eq(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			return(lva==lvb);
		}
	}

	if(__lvax_flonump(arga))
	{
		if(__lvax_flonump(argb))
		{
			fa=__lvax_conv_tof64(arga);
			fb=__lvax_conv_tof64(argb);
			return(fa==fb);
		}
	}
	
	return(0);
}

int __lvax_cmp_gt(u128 arga, u128 argb)
{
	s128 lva, lvb, lvc;
	double fa, fb, fc;

	if(__lvax_fixnump(arga))
	{
		if(__lvax_fixnump(argb))
		{
			lva=__lvax_conv_toi128(arga);
			lvb=__lvax_conv_toi128(argb);
			return(lva>lvb);
		}
	}

	if(__lvax_flonump(arga))
	{
		if(__lvax_flonump(argb))
		{
			fa=__lvax_conv_tof64(arga);
			fb=__lvax_conv_tof64(argb);
			return(fa>fb);
		}
	}
	
	return(0);
}

u128 __lvax_conv_fromptr(void *val)
{
	u128 v;	

	v=((u128)val);
	
	if(((v>>60)&15)==0)
	{
		if((v>>48)&4095)
			return(v);

		v=v&0x0000FFFF_FFFFFFFF_0000FFFF_FFFFFFFFUI128;
		v|=((u128)TKMM_LVA_GetPtrTypeTag(val))<<48;
		return(v);
	}

	if(	(((v>>60)&15)==2)	||
		(((v>>60)&15)==3)	)
	{
		v=v&0x0000FFFF_FFFFFFFF_0000FFFF_FFFFFFFFUI128;
		v|=((u128)TKMM_LVA_GetPtrTypeTag(val))<<48;
		return(v);
	}
	
	return(0);
}

void *__lvax_conv_toptr(u128 val)
{
	void *ptr;
	u128 v;	

}
