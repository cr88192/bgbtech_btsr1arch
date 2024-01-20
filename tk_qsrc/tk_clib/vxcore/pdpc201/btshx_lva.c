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

#ifdef __BJX2__

int __lva_fixnump(u64 val);
int __lva_flonump(u64 val);
s32 __lva_conv_toi32(u64 val);
s64 __lva_conv_toi64(u64 val);

float __lva_conv_tof32(u64 val);
double __lva_conv_tof64(u64 val);

__asm {

.ifarch seen_variant

__lva_fixnump:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	MOVT	R2
	RTS

__lva_flonump:
	SHLD.Q	R4, -62, R5
	CMPEQ	2, R5
	MOVT	R2
	RTS

__lva_conv_toi32:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
	CMPEQ	2, R5
	BT		.LF

	.LI:
	EXTS.L	R4, R2
	RTS

	.LF:
	SHLD.Q	R4, 2, R6
	FSTCI	R6, R2
	RTS

__lva_conv_toi64:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
	CMPEQ	2, R5
	BT		.LF

	.LI:
	SHAD.Q	R4, 2, R2
	SHAD.Q	R2, -2, R2
	RTS

	.LF:
	SHLD.Q	R4, 2, R6
	FSTCI	R6, R2
	RTS

__lva_conv_tof32:
__lva_conv_tof64:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
	CMPEQ	2, R5
	BT		.LF

	.LI:
	SHAD.Q	R4, 2, R6
	SHAD.Q	R6, -2, R6
	FLDCI	R6, R2
	RTS

	.LF:
	SHLD.Q	R4, 2, R2
	RTS

.endif

};

#else

int __lva_fixnump(u64 val)
{
	return((val>>62)==1);
//	if((val>>62)==1)
//		return(1);
//	return(0);
}

int __lva_flonump(u64 val)
{
	return((val>>62)==2);
//	if((val>>62)==2)
//		return(1);
//	return(0);
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

#endif

void *__lva_conv_toptr(u64 val)
{
	s64 iv;
	double f;

	if((val>>60)==0)
	{
		return((void *)(val&0x0000FFFFFFFFFFFFULL));
	
#if 0
// #ifdef __BJX1_64__
#ifdef __ADDR_64__
		iv=val<<16;
		return((void *)(iv>>16));
#else
		return((void *)((u32)iv));
#endif
#endif
	}

	if((val>>60)==15)
	{
		return((void *)(val&0x0000FFFFFFFFFFFFULL));

#if 0
//#ifdef __BJX1_64__
#ifdef __ADDR_64__
		iv=val<<4;
		return((void *)(iv>>4));
#else
		return((void *)((u32)iv));
#endif
#endif
	}

	if((val>>62)==1)
	{
// #ifdef __BJX1_64__
// #ifdef __ADDR_64__
#if 1
		iv=val<<2;
		return((void *)(iv>>2));
#else
		iv=val;
		return((void *)((u32)iv));
#endif
	}

	if((val>>62)==2)
	{
		return(NULL);
	}
	
	return(NULL);
}

#ifdef __BGBCC__
u64 __lva_conv_fromi128(__int128 val)
{
//	return(TKMM_LVA_NewInt128(val));
	return(TKMM_LVA_NewBigInt(val));

#if 0
	void *ptr;
	u64 vptr;
	ptr=TKMM_LVA_AllocInt128();
	*(__int128 *)ptr=val;
	vptr=(u64)ptr;
	vptr|=((u64)LVA_LVATY_INT128)<<48;
	return(vptr);
#endif
}

__int128 __lva_conv_toi128(u64 val)
{
	s64 iv;
	void *ptr;
	int tag;
	
	if((val>>62)==1)
	{
		iv=val<<2;
		return(iv>>2);
	}

	tag=val>>48;
	if(tag==LVA_LVATY_BIGINT)
	{
		ptr=(void *)val;
		return(*(__int128 *)ptr);
	}

	iv=__lva_conv_toi64(val);
	return(iv);

//	return(-1);
}
#endif

#ifdef __BJX2__

u64 __lva_conv_fromi32(s32 val);
u64 __lva_conv_fromi64(s64 val);
u64 __lva_conv_fromf32(float val);
u64 __lva_conv_fromf64(double val);

u64 __lva_conv_fromvec2f(u64 vec);

__asm {

.ifarch seen_variant

__lva_conv_fromi32:
	EXTS.L	R4, R2
	CONVFXI	R2, R2
//	MOV		0xC000_0000_0000_0000, R6
//	NOT		R6, R6
//	MOV		0x4000_0000_0000_0000, R7
//	AND		R6, R2
//	OR		R7, R2
	RTS

__lva_conv_fromi64:
	CONVFXI	R4, R2
	RTS

#if 0
	SHAD.Q	R4, -62, R6
	CONVFXI	R4, R2

	CMPEQ	0, R6
	BT		.OK
	CMPEQ	-1, R6
	BT		.OK
	
	SHAD.Q	R6, -1, R5
	BRA		__lva_conv_fromi128
//	MOV		0xC000_0000_0000_0000, R6
//	NOT		R6, R6
//	MOV		0x4000_0000_0000_0000, R7
//	AND		R6, R2
//	OR		R7, R2
	.OK:
	RTS
#endif

__lva_conv_fromf32:
__lva_conv_fromf64:
	CONVFLI	R2, R2
//	SHLD.Q		R4, -2, R2
//	MOV			0x8000_0000_0000_0000, R3
//	OR			R3, R2
	RTS

__lva_conv_fromvec2f:
	MOV			0x0FFF_FFFF_C000_0000, R16
	MOV			0x0000_0000_3FFF_FFFF, R17
	MOV			0xD000_0000_0000_0000, R18
	SHLD.Q	R4, -4, R6		|	SHLD.Q	R4, -2, R7
	AND		R6, R16, R6		|	AND		R7, R17, R7
	OR		R6, R7, R2
	OR		R18, R2
	RTS

.endif

};

#else

u64 __lva_conv_fromi32(s32 val)
{
	u64 v;
	
//	tk_printf("__lva_conv_fromi32 A %X\n", val);
	
	v=((s64)val);
	v&=0x3FFFFFFFFFFFFFFFULL;
	v|=0x4000000000000000ULL;

//	tk_printf("__lva_conv_fromi32 B %8X_%8X\n",
//		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromi64(s64 val)
{
	u64 v;

//	tk_printf("__lva_conv_fromi64 %X_%X\n", (int)(val>>32), (int)val);
	
	v=((s64)val);
	v&=0x3FFFFFFFFFFFFFFFULL;
	v|=0x4000000000000000ULL;

//	tk_printf("__lva_conv_fromi64 B %8X_%8X\n",
//		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromf32(float val)
{
	double f;
	u64 v;

//	tk_printf("__lva_conv_fromf32 A %f\n", val);
	
	f=val;
	v=(*(u64 *)(&f));
	v=v>>2;
	v|=0x8000000000000000ULL;

//	tk_printf("__lva_conv_fromf32 B %8X_%8X\n",
//		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromf64(double val)
{
	double f;
	u64 v;

//	tk_printf("__lva_conv_fromf64 A %f\n", val);

	f=val;
	v=(*(u64 *)(&f));

//	*((int *)-1)=-1;

//	tk_printf("__lva_conv_fromf64 B %8X_%8X\n",
//		(int)(v>>32), (int)v);

	v=v>>2;
	v|=0x8000000000000000ULL;

//	tk_printf("__lva_conv_fromf64 C %8X_%8X\n",
//		(int)(v>>32), (int)v);

	return(v);
}

u64 __lva_conv_fromvec2f(u64 vec)
{
	u64 v0, v1, v2;
	v0=(vec>>2)&0x000000003FFFFFFFULL;
	v1=(vec>>4)&0x0FFFFFFFC0000000ULL;
	v2=v0|v1|0xD000000000000000ULL;
	return(v2);
}

#endif

u64 __lva_conv_fromi64big(s64 val)
{
#ifdef __BGBCC__
//	u64 v;
	return(__lva_conv_fromi128(val));
#else
	return(__lva_conv_fromi64(val));
#endif
}

u64 __lva_conv_fromptr(void *val)
{
	u64 v;	

	v=((u64)val)&0x0000FFFFFFFFFFFFULL;
	v|=((u64)TKMM_LVA_GetPtrTypeTag(val))<<48;
	return(v);

#if 0
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
#endif

}

int tkmm_lvatyi_string;

u64 __lva_conv_fromstr(char *val)
{
	return(TKMM_LVA_WrapString(val));
}

u64 __lva_add_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_ADD));
}

u64 __lva_sub_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_SUB));
}

u64 __lva_mul_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_MUL));
}

u64 __lva_div_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_DIV));
}

u64 __lva_mod_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_MOD));
}

u64 __lva_and_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_AND));
}

u64 __lva_or_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_OR));
}

u64 __lva_xor_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_XOR));
}

u64 __lva_shl_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_SHL));
}

u64 __lva_shr_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_SHR));
}

u64 __lva_neg_gen(u64 arga)
{
	return(TKMM_LVA_UnaryOpGeneric(arga, LVA_OPR_NEG));
}

u64 __lva_not_gen(u64 arga)
{
	return(TKMM_LVA_UnaryOpGeneric(arga, LVA_OPR_NOT));
}

int __lva_cmpeq_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_CMPEQ));
}

int __lva_cmpgt_gen(u64 arga, u64 argb)
{
	return(TKMM_LVA_BinaryOpGeneric(arga, argb, LVA_OPR_CMPGT));
}

u64 __lva_inc_gen(u64 arga)
{
	return(TKMM_LVA_UnaryOpGeneric(arga, LVA_OPR_INC));
}

u64 __lva_dec_gen(u64 arga)
{
	return(TKMM_LVA_UnaryOpGeneric(arga, LVA_OPR_DEC));
}



#ifdef __BJX2__
u64 __lva_add(u64 arga, u64 argb);
u64 __lva_sub(u64 arga, u64 argb);

u64 __lva_and(u64 arga, u64 argb);
u64 __lva_or (u64 arga, u64 argb);
u64 __lva_xor(u64 arga, u64 argb);

u64 __lva_shl(u64 arga, u64 argb);
u64 __lva_shr(u64 arga, u64 argb);

u64 __lva_neg(u64 arga);
u64 __lva_not(u64 arga);

int __lva_cmp_eq(u64 arga, u64 argb);
int __lva_cmp_gt(u64 arga, u64 argb);

u64 __lva_inc(u64 arga);
u64 __lva_dec(u64 arga);


__asm {

.ifarch seen_variant

__lva_add:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF
	
	BRA		__lva_add_gen
	
	.II:
//	MOV		0xC000_0000_0000_0000, R3
//	MOV		0x4000_0000_0000_0000, R17
//	NOT		R3, R16
	ADD		R4, R5, R2
	CONVFXI	R2, R2
//	AND		R16, R2
//	OR		R17, R2
	RTS
	
	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FLDCI	R5, R5
	BRA		.FF2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FLDCI	R4, R4
	BRA		.FF2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
//	BRA		.FF2
	
	.FF2:
//	MOV		0x8000_0000_0000_0000, R3
	FADD	R4, R5, R2
	CONVFLI	R2, R2
//	SHLD.Q	R2, -2, R2
//	OR		R3, R2
	RTS

__lva_sub:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF
	
	BRA		__lva_sub_gen
	
	.II:
//	MOV		0xC000_0000_0000_0000, R3
//	MOV		0x4000_0000_0000_0000, R17
//	NOT		R3, R16
	SUB		R4, R5, R2
	CONVFXI	R2, R2
//	AND		R16, R2
//	OR		R17, R2
	RTS
	
	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FLDCI	R5, R5
	BRA		.FF2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FLDCI	R4, R4
	BRA		.FF2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
//	BRA		.FF2
	
	.FF2:
//	MOV		0x8000_0000_0000_0000, R3
	FSUB	R4, R5, R2
//	SHLD.Q	R2, -2, R2
//	OR		R3, R2
	CONVFLI	R2, R2
	RTS

__lva_and:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF

	BRA		__lva_and_gen

	.II:
	AND		R4, R5, R2
	RTS

	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FSTCI	R4, R4
	BRA		.II2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R5, R5
	BRA		.II2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R4, R4
	FSTCI	R5, R5

	.II2:
	AND		R4, R5, R2
	CONVFXI	R2, R2
	RTS

__lva_or:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF

	BRA		__lva_or_gen
	
	.II:
	OR		R4, R5, R2
	RTS

	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FSTCI	R4, R4
	BRA		.II2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R5, R5
	BRA		.II2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R4, R4
	FSTCI	R5, R5

	.II2:
	OR		R4, R5, R2
	CONVFXI	R2, R2
	RTS

__lva_xor:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF

	BRA		__lva_xor_gen
	
	.II:
//	MOV		0x4000_0000_0000_0000, R17
	XOR		R4, R5, R2
	CONVFXI	R2, R2
//	OR		R17, R2
	RTS

	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FSTCI	R4, R4
	BRA		.II2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R5, R5
	BRA		.II2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R4, R4
	FSTCI	R5, R5

	.II2:
	XOR		R4, R5, R2
	CONVFXI	R2, R2
	RTS

__lva_shl:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF

	BRA		__lva_shl_gen
	
	.II:
//	MOV		0xC000_0000_0000_0000, R3
//	MOV		0x4000_0000_0000_0000, R17
//	NOT		R3, R16
	SHAD.Q	R4, R5, R2
	CONVFXI	R2, R2
//	AND		R16, R2
//	OR		R17, R2
	RTS
	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FSTCI	R4, R4
	BRA		.II2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R5, R5
	BRA		.II2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R4, R4
	FSTCI	R5, R5

	.II2:
	SHAD.Q	R4, R5, R2
	CONVFXI	R2, R2
	RTS

__lva_shr:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF

	BRA		__lva_shr_gen
	
	.II:
//	MOV		0xC000_0000_0000_0000, R3
//	MOV		0x4000_0000_0000_0000, R17
//	NOT		R3, R16
	NEG		R5, R5
	SHAD.Q	R4, R5, R2
	CONVFXI	R2, R2
//	AND		R16, R2
//	OR		R17, R2
	RTS

	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FSTCI	R4, R4
	BRA		.II2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R5, R5
	BRA		.II2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	FSTCI	R4, R4
	FSTCI	R5, R5

	.II2:
	NEG		R5, R5
	SHAD.Q	R4, R5, R2
	CONVFXI	R2, R2
	RTS

__lva_neg:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
	CMPEQ	2, R5
	BT		.LF

	BRA		__lva_neg_gen

	.LI:
//	MOV		0xC000_0000_0000_0000, R3
//	MOV		0x4000_0000_0000_0000, R17
//	NOT		R3, R16
	NEG		R4, R2
	CONVFXI	R2, R2
//	AND		R16, R2
//	OR		R17, R2
	RTS

	.LF:
	MOV		0x2000_0000_0000_0000, R3
	XOR		R4, R3, R2
	RTS

__lva_not:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
//	CMPEQ	2, R5
//	BT		.LF

	BRA		__lva_not_gen

	.LI:
	MOV		0xC000_0000_0000_0000, R3
	NOT		R4, R2
	XOR		R3, R2
	RTS

//	.LF:
//	MOV		0x2000_0000_0000_0000, R3
//	XOR		R4, R3, R2
//	RTS

__lva_cmp_eq:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF
	
	BRA		__lva_cmpeq_gen
	
	.II:
	CMPQEQ	R4, R5
	MOVT	R2
	RTS
	
	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FLDCI	R5, R5
	BRA		.FF2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FLDCI	R4, R4
	BRA		.FF2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
//	BRA		.FF2
	
	.FF2:
	FCMPEQ	R4, R5
	MOVT	R2
	RTS

__lva_cmp_gt:
	MOVHHD		R4, R5, R6
	PMORT.Q		R6, R6
	SHLD.Q		R6, -60, R7
	CMPEQ		0x3, R7
	BT			.II
	CMPEQ		0x6, R7
	BT			.IF
	CMPEQ		0x9, R7
	BT			.FI
	CMPEQ		0xC, R7
	BT			.FF
	
	BRA		__lva_cmpgt_gen
	
	.II:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	CMPQGT	R5, R4
	MOVT	R2
	RTS
	
	.IF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
	SHAD.Q	R5, -2, R5
	FLDCI	R5, R5
	BRA		.FF2

	.FI:
	SHLD.Q	R4, 2, R4
	SHAD.Q	R4, -2, R4
	SHLD.Q	R5, 2, R5
	FLDCI	R4, R4
	BRA		.FF2

	.FF:
	SHLD.Q	R4, 2, R4
	SHLD.Q	R5, 2, R5
//	BRA		.FF2
	
	.FF2:
	FCMPGT	R5, R4
	MOVT	R2
	RTS


__lva_inc:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
	CMPEQ	2, R5
	BT		.LF

	BRA		__lva_inc_gen

	.LI:
	ADD		R4, 1, R2
	CONVFXI	R2, R2
	RTS

	.LF:
	SHLD.Q	R4, 2, R4
	MOV		0x3FF0_0000_0000_0000, R5
	FADD	R4, R5, R2
	CONVFLI	R2, R2
	RTS

__lva_dec:
	SHLD.Q	R4, -62, R5
	CMPEQ	1, R5
	BT		.LI
	CMPEQ	2, R5
	BT		.LF

	BRA		__lva_dec_gen

	.LI:
	ADD		R4, -1, R2
	CONVFXI	R2, R2
	RTS

	.LF:
	SHLD.Q	R4, 2, R4
	MOV		0x3FF0_0000_0000_0000, R5
	FSUB	R4, R5, R2
	CONVFLI	R2, R2
	RTS

.endif

};

#else

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

	return(__lva_add_gen(arga, argb));
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

	return(__lva_sub_gen(arga, argb));
}

int __lva_cmp_eq(u64 arga, u64 argb)
{
	s64 li, lj;
	double lf, lg;

	if(__lva_fixnump(arga))
	{
		if(__lva_fixnump(argb))
		{
			li=__lva_conv_toi64(arga);
			lj=__lva_conv_toi64(argb);
			return(li==lj);
		}
	}
		
	if(__lva_fixnump(arga) ||
		__lva_flonump(arga))
	{
		if(__lva_fixnump(argb) ||
			__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			return(lf==lg);
		}
	}
	return(__lva_cmpeq_gen(arga, argb));
}

int __lva_cmp_gt(u64 arga, u64 argb)
{
	s64 li, lj;
	double lf, lg;

	if(__lva_fixnump(arga))
	{
		if(__lva_fixnump(argb))
		{
			li=__lva_conv_toi64(arga);
			lj=__lva_conv_toi64(argb);
			return(li>lj);
		}
	}
		
	if(__lva_fixnump(arga) ||
		__lva_flonump(arga))
	{
		if(__lva_fixnump(argb) ||
			__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			return(lf>lg);
		}
	}
	return(__lva_cmpgt_gen(arga, argb));
}


#endif


u64 __lva_mul(u64 arga, u64 argb)
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
			lk=li*lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
		
		if(__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf*lg;
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
			lh=lf*lg;
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}

	return(__lva_mul_gen(arga, argb));
}

u64 __lva_div(u64 arga, u64 argb)
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
			lk=li/lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
		
		if(__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf/lg;
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
			lh=lf/lg;
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}

	return(__lva_div_gen(arga, argb));
}

u64 __lva_mod(u64 arga, u64 argb)
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
			lk=li%lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
	}

	if(__lva_fixnump(arga) ||
		__lva_flonump(arga))
	{
		if(__lva_fixnump(argb) ||
			__lva_flonump(argb))
		{
			lf=__lva_conv_tof64(arga);
			lg=__lva_conv_tof64(argb);
			lh=lf/lg;
			lh=(double)((long)lh);
			lh=lf-(lh*lg);
			v=__lva_conv_fromf64(lh);
			return(v);
		}
	}

	return(__lva_mod_gen(arga, argb));
}

#ifndef __BJX2__

u64 __lva_and(u64 arga, u64 argb)
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
			lk=li&lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
	}

	return(__lva_and_gen(arga, argb));
}

u64 __lva_or(u64 arga, u64 argb)
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
			lk=li|lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
	}

	return(__lva_or_gen(arga, argb));
}

u64 __lva_xor(u64 arga, u64 argb)
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
			lk=li^lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
	}

	return(__lva_xor_gen(arga, argb));
}

u64 __lva_shl(u64 arga, u64 argb)
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
			lk=li<<lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
	}

	return(__lva_shl_gen(arga, argb));
}

u64 __lva_shr(u64 arga, u64 argb)
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
			lk=li>>lj;
			v=__lva_conv_fromi64(lk);
			return(v);
		}
	}

	return(__lva_shr_gen(arga, argb));
}

u64 __lva_neg(u64 arga)
{
	u64 v;
	s64 li, lj, lk;
	double lf, lg, lh;

	if(__lva_fixnump(arga))
	{
		li=__lva_conv_toi64(arga);
		lk=-li;
		v=__lva_conv_fromi64(lk);
		return(v);
	}

	if(__lva_flonump(arga))
	{
		lf=__lva_conv_tof64(arga);
		lh=-lf;
		v=__lva_conv_fromf64(lh);
		return(v);
	}

	return(__lva_neg_gen(arga));
}

u64 __lva_not(u64 arga)
{
	u64 v;
	s64 li, lj, lk;
	double lf, lg, lh;

	if(__lva_fixnump(arga))
	{
		li=__lva_conv_toi64(arga);
		lk=~li;
		v=__lva_conv_fromi64(lk);
		return(v);
	}

#if 0
	if(__lva_flonump(arga))
	{
		lf=__lva_conv_tof64(arga);
		lh=-lf;
		v=__lva_conv_fromf64(lh);
		return(v);
	}
#endif

	return(__lva_not_gen(arga));
}

#endif

#if 1
tk_lva_variant __lva_loadindex(tk_lva_variant obj, int idx)
{
	u16 *pw;
	byte *pb;
	u64 objv;
	int tg4, tg12, tgc;
	
	objv=__object_getbits(obj);

	tg4=(objv>>60)&15;
	tg12=(objv>>48)&4095;
	if(tg4==0)
	{
		if(tg12==tkmm_lvatyi_tagarray)
		{
			return(__lvo_loadindex_var(obj, idx));
		}
	
		if(tg12==tkmm_lvatyi_string)
		{
			pb=(byte *)objv;
			return(pb[idx]);
		}

		if(tg12==tkmm_lvatyi_wstring)
		{
			pw=(u16 *)objv;
			return(pw[idx]);
		}
	}

	if(tg4==12)
	{
		return(__lvo_loadindex_var(obj, idx));
	}

//	return(*(tk_lva_variant *)TKMM_LVA_GetArrayIndexPtr(obj, idx, 3));
}

void __lva_storeindex(tk_lva_variant obj, int idx, tk_lva_variant val)
{
	u64 objv;
	int tg4, tg12, tgc;
	
	objv=__object_getbits(obj);

	tg4=(objv>>60)&15;
	if(tg4==12)
	{
		__lvo_storeindex_var(obj, idx, val);
		return;
	}

	tg12=(objv>>48)&4095;
	if(tg4==0)
	{
		if(tg12==tkmm_lvatyi_tagarray)
		{
			__lvo_storeindex_var(obj, idx, val);
			return;
		}
	}
}

#endif

tk_lva_variant __lva_rcons(tk_lva_variant cdr, tk_lva_variant car)
{
	LVA_Cons *tmp;
	u64 val;
	tmp=TKMM_LVA_NewCons(car, cdr);
#ifdef __ADDR_X96__
	val=__lvax_conv_fromptr(tmp);
#else
	val=__lva_conv_fromptr(tmp);
#endif
	return(val);
}

tk_lva_variant __lva_cons(tk_lva_variant car, tk_lva_variant cdr)
{
	LVA_Cons *tmp;
	u64 val;
	tmp=TKMM_LVA_NewCons(car, cdr);
#ifdef __ADDR_X96__
	val=__lvax_conv_fromptr(tmp);
#else
	val=__lva_conv_fromptr(tmp);
#endif
	return(val);
}

char *__lva_conv_tostring(u64 val)
{
	char tb[128];
	char *ct;
	s64 li, lj, lk, ll;
	double lf, lg;
	int i, j, k, sg;
	
	if(__lva_fixnump(val))
	{
		li=__lva_conv_toi64(val);
		sg=0;
		if(li<0)
			{ li=-li; sg=1; }
		
		ct=tb+127;
		*(--ct)=0;
		do {
			lj=li/10;
			j=li-(lj*10);
			*(--ct)='0'+j;
			li=lj;
		}while(li!=0);
		
		if(sg)
			*(--ct)='-';
		
		return(tk_rstrdup(ct));
	}

	if(__lva_flonump(val))
	{
		lf=__lva_conv_tof64(val);
		li=(long)lf;
		lg=lf-((double)li);
		lj=lg*1000000+0.5;
		
		ct=tb+127;
		*(--ct)=0;

		lk=lj;
//		while(lk!=0)
		for(i=0; i<6; i++)
		{
			ll=lk/10;
			j=lk-(ll*10);
			*(--ct)='0'+j;
			lk=ll;
		}

		*(--ct)='.';

		lk=li;
		do{
			ll=lk/10;
			j=lk-(ll*10);
			*(--ct)='0'+j;
			lk=ll;
		}while(lk!=0);

		if(sg)
			*(--ct)='-';
		
		return(tk_rstrdup(ct));
	}
	
	return("?");
}
#endif
