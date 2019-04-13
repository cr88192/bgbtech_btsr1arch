#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif

//#ifdef __linux
#ifndef _MSC_VER
#define __debugbreak()	(*((int *)-1)=-1)
#endif

// #ifdef _M_X64
// typedef long long nlint;
// #else
// typedef long nlint;
// #endif

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifndef __EMSCRIPTEN__
#ifdef __GNUC__
//#define force_inline inline
//#define default_inline inline
#endif
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

#if defined(X86) || defined(X86_64) || defined(ARM)
// #if 0

#define bgbcc_getu16le(ptr)		(*(u16 *)(ptr))
#define bgbcc_getu32le(ptr)		(*(u32 *)(ptr))
#define bgbcc_getu64le(ptr)		(*(u64 *)(ptr))
#define bgbcc_gets16le(ptr)		(*(s16 *)(ptr))
#define bgbcc_gets32le(ptr)		(*(s32 *)(ptr))
#define bgbcc_gets64le(ptr)		(*(s64 *)(ptr))
#define bgbcc_setu16le(ptr, val)	((*(u16 *)(ptr))=(val))
#define bgbcc_setu32le(ptr, val)	((*(u32 *)(ptr))=(val))
#define bgbcc_setu64le(ptr, val)	((*(u64 *)(ptr))=(val))
#define bgbcc_sets16le(ptr, val)	((*(s16 *)(ptr))=(val))
#define bgbcc_sets32le(ptr, val)	((*(s32 *)(ptr))=(val))
#define bgbcc_sets64le(ptr, val)	((*(s64 *)(ptr))=(val))

#define bgbcc_getu8le(ptr)			(*(byte *)(ptr))
#define bgbcc_gets8le(ptr)			(*(sbyte *)(ptr))

#define bgbcc_setu8le(ptr, val)	((*(byte *)(ptr))=(val))
#define bgbcc_sets8le(ptr, val)	((*(sbyte *)(ptr))=(val))

default_inline u16 bgbcc_bswapu16(u16 val)
	{ return((val>>8)|(val<<8)); }
default_inline u32 bgbcc_bswapu32(u32 val)
	{ return((val>>24)|(val<<24)|
		((val>>8)&0x0000FF00)|((val<<8)&0x00FF0000)); }
default_inline u64 bgbcc_bswapu64(u64 val)
	{ return(bgbcc_bswapu32(val>>32)|(((u64)bgbcc_bswapu32(val))<<32)); }

#define bgbcc_getu16be(ptr)		bgbcc_bswapu16(*(u16 *)(ptr))
#define bgbcc_getu32be(ptr)		bgbcc_bswapu32(*(u32 *)(ptr))
#define bgbcc_getu64be(ptr)		bgbcc_bswapu64(*(u64 *)(ptr))
#define bgbcc_gets16be(ptr)		((s16)bgbcc_bswapu16(*(u16 *)(ptr)))
#define bgbcc_gets32be(ptr)		((s32)bgbcc_bswapu32(*(u32 *)(ptr)))
#define bgbcc_gets64be(ptr)		((s64)bgbcc_bswapu64(*(u64 *)(ptr)))

default_inline void bgbcc_setu16be(byte *ptr, u16 val)
	{ ptr[1]=val; ptr[0]=val>>8; }
default_inline void bgbcc_setu32be(byte *ptr, u32 val)
	{ ptr[3]=val; ptr[2]=val>>8; ptr[1]=val>>16; ptr[0]=val>>24; }
default_inline void bgbcc_setu64be(byte *ptr, u64 val)
	{ bgbcc_setu32be(ptr+4, val); bgbcc_setu32be(ptr+0, val>>32); }
default_inline void bgbcc_sets16be(byte *ptr, s16 val)
	{ bgbcc_setu16be(ptr, (u16)val); }
default_inline void bgbcc_sets32be(byte *ptr, s32 val)
	{ bgbcc_setu32be(ptr, (u32)val); }
default_inline void bgbcc_sets64be(byte *ptr, s64 val)
	{ bgbcc_setu64be(ptr, (u64)val); }

#define bgbcc_getf32le(ptr)		(*(f32 *)(ptr))
#define bgbcc_getf64le(ptr)		(*(f64 *)(ptr))
#define bgbcc_setf32le(ptr, val)	((*(f32 *)(ptr))=(val))
#define bgbcc_setf64le(ptr, val)	((*(f64 *)(ptr))=(val))

default_inline f32 bgbcc_bswapf32a(u32 val)
	{ u32 t; t=bgbcc_bswapu32(val); return(*(f32 *)(&t)); }
default_inline f64 bgbcc_bswapf64a(u64 val)
	{ u64 t; t=bgbcc_bswapu64(val); return(*(f64 *)(&t)); }

default_inline u32 bgbcc_bswapf32b(f32 val)
	{ return(bgbcc_bswapu32(*(u32 *)(&val))); }
default_inline u64 bgbcc_bswapf64b(f64 val)
	{ return(bgbcc_bswapu64(*(u64 *)(&val))); }

#define bgbcc_getf32be(ptr)		bgbcc_bswapf32a(*(u32 *)(ptr))
#define bgbcc_getf64be(ptr)		bgbcc_bswapf64a(*(u64 *)(ptr))
#define bgbcc_setf32be(ptr, val)	((*(u32 *)(ptr))=bgbcc_bswapf32b(val))
#define bgbcc_setf64be(ptr, val)	((*(u64 *)(ptr))=bgbcc_bswapf64b(val))

#else
default_inline u16 bgbcc_getu16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline u32 bgbcc_getu32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline u64 bgbcc_getu64le(byte *ptr)
	{ return(bgbcc_getu32le(ptr)|(((u64)bgbcc_getu32le(ptr+4))<<32)); }
default_inline s16 bgbcc_gets16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline s32 bgbcc_gets32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline s64 bgbcc_gets64le(byte *ptr)
	{ return(bgbcc_getu32le(ptr)|(((s64)bgbcc_gets32le(ptr+4))<<32)); }
default_inline void bgbcc_setu16le(byte *ptr, u16 val)
	{ ptr[0]=val; ptr[1]=val>>8; }
default_inline void bgbcc_setu32le(byte *ptr, u32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; ptr[3]=val>>24; }
default_inline void bgbcc_setu64le(byte *ptr, u64 val)
	{ bgbcc_setu32le(ptr, val); bgbcc_setu32le(ptr+4, val>>32); }
default_inline void bgbcc_sets16le(byte *ptr, s16 val)
	{ bgbcc_setu16le(ptr, (u16)val); }
default_inline void bgbcc_sets32le(byte *ptr, s32 val)
	{ bgbcc_setu32le(ptr, (u32)val); }
default_inline void bgbcc_sets64le(byte *ptr, s64 val)
	{ bgbcc_setu64le(ptr, (u64)val); }

#define bgbcc_getu8le(ptr)			(*(byte *)(ptr))
#define bgbcc_gets8le(ptr)			(*(sbyte *)(ptr))

#define bgbcc_setu8le(ptr, val)	((*(byte *)(ptr))=(val))
#define bgbcc_sets8le(ptr, val)	((*(sbyte *)(ptr))=(val))

default_inline u16 bgbcc_getu16be(byte *ptr)
	{ return(ptr[1]|(ptr[0]<<8)); }
default_inline u32 bgbcc_getu32be(byte *ptr)
	{ return(ptr[3]|(ptr[2]<<8)|(ptr[1]<<16)|(ptr[0]<<24)); }
default_inline u64 bgbcc_getu64be(byte *ptr)
	{ return(bgbcc_getu32le(ptr+4)|(((u64)bgbcc_getu32le(ptr))<<32)); }

default_inline void bgbcc_setu16be(byte *ptr, u16 val)
	{ ptr[1]=val; ptr[0]=val>>8; }
default_inline void bgbcc_setu32be(byte *ptr, u32 val)
	{ ptr[3]=val; ptr[2]=val>>8; ptr[1]=val>>16; ptr[0]=val>>24; }
default_inline void bgbcc_setu64be(byte *ptr, u64 val)
	{ bgbcc_setu32be(ptr+4, val); bgbcc_setu32be(ptr+0, val>>32); }
default_inline void bgbcc_sets16be(byte *ptr, s16 val)
	{ bgbcc_setu16be(ptr, (u16)val); }
default_inline void bgbcc_sets32be(byte *ptr, s32 val)
	{ bgbcc_setu32be(ptr, (u32)val); }
default_inline void bgbcc_sets64be(byte *ptr, s64 val)
	{ bgbcc_setu64be(ptr, (u64)val); }

#define bgbcc_gets16be(ptr)		((s16)bgbcc_getu16be(ptr))
#define bgbcc_gets32be(ptr)		((s32)bgbcc_getu32be(ptr))
#define bgbcc_gets64be(ptr)		((s64)bgbcc_getu64be(ptr))


default_inline f32 bgbcc_f32fromu32bits(u32 val)
	{ return(*(f32 *)(&val)); }
default_inline f64 bgbcc_f64fromu64bits(u64 val)
	{ return(*(f64 *)(&val)); }

default_inline u32 bgbcc_u32fromf32bits(f32 val)
	{ return(*(u32 *)(&val)); }
default_inline u64 bgbcc_u64fromf64bits(f64 val)
	{ return(*(u64 *)(&val)); }

#define bgbcc_getf32le(ptr)		bgbcc_f32fromu32bits(bgbcc_getu32le(ptr))
#define bgbcc_getf64le(ptr)		bgbcc_f64fromu64bits(bgbcc_getu64le(ptr))
#define bgbcc_setf32le(ptr, val)	bgbcc_setu32le((ptr), \
										bgbcc_u32fromf32bits(val))
#define bgbcc_setf64le(ptr, val)	bgbcc_setu32le((ptr), \
										bgbcc_u64fromf64bits(val))

#define bgbcc_getf32be(ptr)		bgbcc_f32fromu32bits(bgbcc_getu32be(ptr))
#define bgbcc_getf64be(ptr)		bgbcc_f64fromu64bits(bgbcc_getu64be(ptr))
#define bgbcc_setf32be(ptr, val)	bgbcc_setu32be((ptr), \
										bgbcc_u32fromf32bits(val))
#define bgbcc_setf64be(ptr, val)	bgbcc_setu32be((ptr), \
										bgbcc_u64fromf64bits(val))

#endif

#define bgbcc_getu16en(ptr, en)			\
	((en)?bgbcc_getu16be(ptr):bgbcc_getu16le(ptr))
#define bgbcc_getu32en(ptr, en)			\
	((en)?bgbcc_getu32be(ptr):bgbcc_getu32le(ptr))
#define bgbcc_getu64en(ptr, en)			\
	((en)?bgbcc_getu32be(ptr):bgbcc_getu32le(ptr))
#define bgbcc_gets16en(ptr, en)			\
	((en)?bgbcc_gets16be(ptr):bgbcc_gets16le(ptr))
#define bgbcc_gets32en(ptr, en)			\
	((en)?bgbcc_gets32be(ptr):bgbcc_gets32le(ptr))
#define bgbcc_gets64en(ptr, en)			\
	((en)?bgbcc_gets32be(ptr):bgbcc_gets32le(ptr))

#if 0
#define bgbcc_setu16en(ptr, en, val)			\
	((en)?bgbcc_setu16be(ptr, val):bgbcc_setu16le(ptr, val))
#define bgbcc_setu32en(ptr, en, val)			\
	((en)?bgbcc_setu32be(ptr, val):bgbcc_setu32le(ptr, val))
#define bgbcc_setu64en(ptr, en, val)			\
	((en)?bgbcc_setu64be(ptr, val):bgbcc_setu64le(ptr, val))
#define bgbcc_sets16en(ptr, en, val)			\
	((en)?bgbcc_sets16be(ptr, val):bgbcc_sets16le(ptr, val))
#define bgbcc_sets32en(ptr, en, val)			\
	((en)?bgbcc_sets32be(ptr, val):bgbcc_sets32le(ptr, val))
#define bgbcc_sets64en(ptr, en, val)			\
	((en)?bgbcc_sets64be(ptr, val):bgbcc_sets64le(ptr, val))
#endif

default_inline void bgbcc_setu16en(byte *ptr, int en, u16 val)
{
	if(en)
		bgbcc_setu16be(ptr, val);
	else
		bgbcc_setu16le(ptr, val);
}
default_inline void bgbcc_setu32en(byte *ptr, int en, u32 val)
{
	if(en)
		bgbcc_setu32be(ptr, val);
	else
		bgbcc_setu32le(ptr, val);
}

default_inline void bgbcc_setu64en(byte *ptr, int en, u64 val)
{
	if(en)
		bgbcc_setu64be(ptr, val);
	else
		bgbcc_setu64le(ptr, val);
}
default_inline void bgbcc_sets16en(byte *ptr, int en, s16 val)
{
	if(en)
		bgbcc_sets16be(ptr, val);
	else
		bgbcc_sets16le(ptr, val);
}
default_inline void bgbcc_sets32en(byte *ptr, int en, s32 val)
{
	if(en)
		bgbcc_sets32be(ptr, val);
	else
		bgbcc_sets32le(ptr, val);
}
default_inline void bgbcc_sets64en(byte *ptr, int en, s64 val)
{
	if(en)
		bgbcc_sets64be(ptr, val);
	else
		bgbcc_sets64le(ptr, val);
}

#define bgbcc_getu8en(ptr, en)			(*(byte *)(ptr))
#define bgbcc_gets8en(ptr, en)			(*(sbyte *)(ptr))
#define bgbcc_setu8en(ptr, en, val)		(*(byte *)(ptr))=(val)
#define bgbcc_sets8en(ptr, en, val)		(*(sbyte *)(ptr))=(val)
