/*
BtRP2 is a yet another byte-based LZ77 variant.

Its design takes some inspiration from the EA RefPack format, but changes were made in an effort to improve compression ratio and decoder performance. Another goal is to try to avoid becomming excessively complicated.


BtRP2 (Transposed, LE):
*                        dddddddd-dlllrrr0	(l=3..10, d=0..511, r=0..7)
*               dddddddd-dddddlll-lllrrr01	(l=4..67, d=0..8191)
*      dddddddd-dddddddd-dlllllll-llrrr011	(l=4..515, d=0..131071)
*                                 rrrr0111	(Raw Bytes, r=(r+1)*8, 8..128)
*                               * rrr01111	(RP2-0, Long Match)
*                        dddllll0-rrr01111  (RP2C, l=11..26, d=1..8, r=0..7)
*               dddddddl-lllllll1-rrr01111  (RP2C, l=68..323, d=1..128, r=0..7)
*                                 rr011111	(r=1..3 bytes, 0=EOB)
*                        rrrrrrrr-r0111111 	(Long Raw, r=(r+1)*8, 8..4096)
*                        lllllll0-01111111  (RP2B, l=4..131, d=1, r=0)
* 2x D-ddddddll-llllllll-llllrrr1-01111111  (RP2B, l=4..16K, d=0..4M, r=0..7)
*   DD-dddddddd-ddddddll-llllrrr0-11111111  (RP2B?, l=4..67, d=0..4M, r=0..7)
** d: Distance
** l: Match Length
** r: Literal Length

Values are encoded in little-endian order, with tag bits located in the LSB. Bits will be contiguous within the value, with shift-and-mask being used to extract individual elements.

Long Match will encode length and distance using variable-length encodings directly following the initial tag byte.

Length VLN:
          lllllll0,   4..  131
 llllllll-llllll01, 132..16383

Distance VLN:
          dddddddd-ddddddd0, 32K (0..32767)
 dddddddd-dddddddd-dddddd01,  4M

While there are both more space efficient and faster ways to handle Length/Distance VLNs (such as via a combined encoding), this encoding is "reasonable" and the Long Match case appears to be relatively less common. The above encoding can be trivially extended to support larger values.

Note the lack of length/distance predictors:
While predictors can be useful, their effectiveness in byte-oriented encoders is limited, and supporting these cases tends to have a detrimental effect on performance (they make more sense in Entropy-coded designs).


Simple File Format:
*  0: 'RP2A'
*  4: Compressed Size
*  8: Uncompressed Size
* 12: Checksum
* Compressed data starts at 16.

This format will not attempt to deal with chunking or streaming.

*/

#ifndef TGVLZ1_C
#define TGVLZ1_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <time.h>

#ifndef TKELZ_MISAL_H

#define HAVE_STDINT_H	/* Probably safe to assume at this point... */

#ifdef _MSC_VER
#if (_MSC_VER<1800)
#undef HAVE_STDINT_H	/* Older MSVC */
#endif
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef U8_T
#define U8_T
typedef unsigned char u8;
#endif

#ifndef S8_T
#define S8_T
typedef signed char s8;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T

// typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

// typedef int8_t		sbyte;
typedef int16_t	s16;
typedef int32_t	s32;
typedef int64_t	s64;

#endif

#else

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T

typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;

#endif

#endif

/* Portability Cruft... */

#ifdef _MSC_VER
#define force_inline	__forceinline
#define	debug_break		__debugbreak();
#ifdef _M_IX86
#define unaligned
#else
#define unaligned		__unaligned
#endif
#define HAVE_MISAL
#else

#if defined(__BGBCC__) || defined(__BJX2__)
#define force_inline	
#define	debug_break		__debugbreak();
#define unaligned		__unaligned
#define HAVE_MISAL
#endif

#if defined(__GNUC__)
#define force_inline	inline
#define	debug_break		__builtin_trap();
#define unaligned		
#define HAVE_MISAL
#endif

#ifndef force_inline
#define force_inline
#endif

#ifndef debug_break
#define	debug_break
#endif
	
#ifndef unaligned
#define unaligned
#endif

#endif

#if defined(__i386__) || defined(__x86_64__) || defined(__arm__)
#define HAVE_MISAL_MEMCPY
#endif

#ifdef HAVE_MISAL

#define	memcpy_4B(dst, src)		\
	(*(unaligned u32 *)(dst)=*(unaligned u32 *)(src))
#define	memcpy_8B(dst, src)		\
	(*(unaligned u64 *)(dst)=*(unaligned u64 *)(src))
#define	memcpy_16B(dst, src)	\
	(((unaligned u64 *)(dst))[0]=((unaligned u64 *)(src))[0]);	\
	(((unaligned u64 *)(dst))[1]=((unaligned u64 *)(src))[1])

#define	get_u16le(ptr)			(*(unaligned u16 *)(ptr))
#define	get_u32le(ptr)			(*(unaligned u32 *)(ptr))
#define	get_u64le(ptr)			(*(unaligned u64 *)(ptr))
#define	set_u16le(ptr, val)		(*(unaligned u16 *)(ptr))=(val)
#define	set_u32le(ptr, val)		(*(unaligned u32 *)(ptr))=(val)
#define	set_u64le(ptr, val)		(*(unaligned u64 *)(ptr))=(val)

#else

#define	memcpy_4B(dst, src)		memcpy(dst, src, 4)
#define	memcpy_8B(dst, src)		memcpy(dst, src, 8)
#define	memcpy_16B(dst, src)	memcpy(dst, src, 16)
//#define	memcpy_16B(dst, src)	\
//	memcpy(((byte *)(dst))+0, ((byte *)(src))+0, 8);		\
//	memcpy(((byte *)(dst))+8, ((byte *)(src))+8, 8);

#ifdef HAVE_MISAL_MEMCPY

force_inline u16 get_u16le(byte *ptr)
	{ u16 v; memcpy(&v, ptr, 2); return(v); }
force_inline u32 get_u32le(byte *ptr)
	{ u32 v; memcpy(&v, ptr, 4); return(v); }
force_inline u64 get_u64le(byte *ptr)
	{ u64 v; memcpy(&v, ptr, 8); return(v); }
force_inline void set_u16le(byte *ptr, u16 val)
	{ memcpy(ptr, &val, 2); }
force_inline void set_u32le(byte *ptr, u32 val)
	{ memcpy(ptr, &val, 4); }
force_inline void set_u64le(byte *ptr, u64 val)
	{ memcpy(ptr, &val, 8); }

#else

force_inline u16 get_u16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
force_inline u32 get_u32le(byte *ptr)
{	return(
		 ((u32)(ptr[0]))     |(((u32)(ptr[1]))<< 8) |
		(((u32)(ptr[2]))<<16)|(((u32)(ptr[3]))<<24) );		}
force_inline u64 get_u64le(byte *ptr)
{	return(
		 ((u64)(ptr[0]))     |(((u64)(ptr[1]))<< 8) |
		(((u64)(ptr[2]))<<16)|(((u64)(ptr[3]))<<24) |
		(((u64)(ptr[4]))<<32)|(((u64)(ptr[5]))<<40) |
		(((u64)(ptr[6]))<<48)|(((u64)(ptr[7]))<<56) );		}
force_inline void set_u16le(byte *ptr, u16 val)
{	ptr[0]=val;		ptr[1]=val>>8;			}
force_inline void set_u32le(byte *ptr, u32 val)
{	ptr[0]=val;			ptr[1]=val>>8;
	ptr[2]=val>>16;		ptr[3]=val>>24;		}
force_inline void set_u64le(byte *ptr, u64 val)
{	ptr[0]=val;			ptr[1]=val>>8;
	ptr[2]=val>>16;		ptr[3]=val>>24;
	ptr[4]=val>>32;		ptr[5]=val>>40;
	ptr[6]=val>>48;		ptr[7]=val>>56;		}

#endif

#endif

#endif

#ifndef GFXEDIT_MISAL
#define GFXEDIT_MISAL
/* portability boilerplate goes here... */
#define gfxedit_getu16(ptr)			get_u16le(ptr)
#define gfxedit_getu32(ptr)			get_u32le(ptr)
#define gfxedit_getu64(ptr)			get_u64le(ptr)
#define gfxedit_setu16(ptr,val)		set_u16le(ptr, val)
#define gfxedit_setu32(ptr,val)		set_u32le(ptr, val)
#define gfxedit_setu64(ptr,val)		set_u64le(ptr, val)
#endif

typedef struct TgvLz_Context_s TgvLz_Context;

//#define TKELZ_HASH_SZ	1024
// #define TKELZ_HASH_SZ	4096
// #define TKELZ_HASH_DN	64

#define TKELZ_HASH_SZ	1024
#define TKELZ_HASH_DN	128

// #define TKELZ_HASH_DN1	63

#define TKELZ_HASHB_SZ	32768
// #define TKELZ_HASHB_DN	4
// #define TKELZ_HASHB_DN	8
#define TKELZ_HASHB_DN	16

// #define TKELZ_HASHB_SZ	8192
// #define TKELZ_HASHB_DN	16

//#define TKELZ_HASHB_SZ	16384
//#define TKELZ_HASHB_DN	8

// #define TKELZ_HASHB_SZ	16384
// #define TKELZ_HASHB_DN	16
// #define TKELZ_HASHB_DN1	15

#define TKELZ_HASH_DN1	(TKELZ_HASH_DN-1)
#define TKELZ_HASHB_DN1	(TKELZ_HASHB_DN-1)

#define TKELZ_CHHASH_SZ	16384
// #define TKELZ_CHAIN_SZ	65536
#define TKELZ_CHAIN_SZ	131072


struct TgvLz_Context_s {
byte *hash[TKELZ_HASH_SZ*TKELZ_HASH_DN];
byte hrov[TKELZ_HASH_SZ];

#ifdef TKELZ_HASHB_SZ
byte *hashb[TKELZ_HASHB_SZ*TKELZ_HASHB_DN];
byte hrovb[TKELZ_HASHB_SZ];
#endif

byte *cs;
byte *ct;
int maxlen;
int maxdist;
int maxdepth;
u32	csum;
byte cmp;
byte cmpver;

byte *css;
byte *cse;

byte	*chn_base;
byte	*chn_ptrs[TKELZ_CHAIN_SZ];
u16		chn_next[TKELZ_CHAIN_SZ];
u16		chn_hash[TKELZ_CHHASH_SZ];

char *tstName;
int (*EncodeBuffer)(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz);
int (*DecodeBuffer)(
	byte *ibuf, byte *obuf, int ibsz, int obsz);

int stat_len[24];
int stat_dist[24];
int stat_rlen[24];

byte *pred_cs1;
byte *pred_cs2;
byte *pred_cs3;
byte *pred_cs4;
int pred_l1, pred_d1, pred_h1;
int pred_l2, pred_d2, pred_h2;
int pred_l3, pred_d3, pred_h3;
int pred_l4, pred_d4, pred_h4;
};

TgvLz_Context *TgvLz_CreateContextRP2C();

#ifndef TGVLZ_LOADFILE
#define TGVLZ_LOADFILE
byte *TgvLz_LoadFile(char *name, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		if(i>0)
		{
			sz=i;
		}else
		{
			free(buf);
			return(NULL);
		}
	}
	
//	*(u64 *)(buf+sz+0)=0;
//	*(u64 *)(buf+sz+8)=0;
	memset(buf+sz, 0, 16);
	
	*rsz=sz;
	return(buf);
}

int TgvLz_StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}
#endif

int TgvLz_CalcHash(byte *cs)
{
	int h;
	
//	h=*(u32 *)cs;
	h=get_u32le(cs);
	h&=0x00FFFFFF;
	h=((h*4093LL)>>12)&(TKELZ_HASH_SZ-1);
	return(h);
}

#ifdef TKELZ_HASHB_SZ
int TgvLz_CalcHashB(byte *cs)
{
#if 0
	int h;
	
//	h=*(u32 *)cs;
	h=get_u32le(cs);
	h=((h*65521LL)>>16)&(TKELZ_HASHB_SZ-1);
#endif

#if 1
	u64 h;
	/* for wide/shallow */
	h=gfxedit_getu64(cs);
	
	h&=0x0000FFFFFFFFFFFFULL;
	
	h^=(h>>29);
	h^=(h<<7)^(h>>13);
	h^=(h>>17);
	
	h&=(TKELZ_HASHB_SZ-1);
#endif

	return(h);
}
#endif

int TgvLz_CalcHashC(byte *cs)
{
	int h;
	
//	h=*(u32 *)cs;
	h=get_u32le(cs);
//	h^=h>>16;
	h^=h>>19;
	h^=h<<11;
	h^=h>>17;
//	h=((h*4093LL)>>12)&(TKELZ_CHHASH_SZ-1);
	h=((h*65521LL)>>16)&(TKELZ_CHHASH_SZ-1);
	return(h);
}

int TgvLz_CheckMatch(byte *s0, byte *s1, byte *cse)
{
	byte *cs, *ct;
	u32 v0, v1, m;
	int d;

	v0=get_u32le(s0);
	v1=get_u32le(s1);
	if(v0!=v1)
	{
		d=s0-s1;
		if(d>=512)
			return(0);
		m=0x00FFFFFF;
		if((v0&m)==(v1&m))
			return(3);
		return(0);
	}
	
	cs=s0; ct=s1;
	cs+=4; ct+=4;

#if 1
	while((cs+8)<=cse)
	{
		if(get_u64le(cs)!=get_u64le(ct))
			break;
		cs+=8; ct+=8;
	}
#endif

	while((cs+4)<=cse)
	{
//		if(*(u32 *)cs!=*(u32 *)ct)
		if(get_u32le(cs)!=get_u32le(ct))
			break;
		cs+=4; ct+=4;
	}
	while(cs<cse)
	{
		if(*cs!=*ct)
			break;
		cs++; ct++;
	}
	
	return(cs-s0);
}

int TgvLz_CalcMatchGain(TgvLz_Context *ctx, int ml, int md)
{
	int mg;
	mg=ml;
	if(md>=512)			mg--;
	if(md>=8192)		mg--;
//	if(md>=8192)		mg-=2;
//	if(md>=131072)		mg--;
	if(md>=131072)		mg-=5;
	return(mg);
}

int TgvLz_HashByte(TgvLz_Context *ctx, byte *cs)
{
	int h, hr, rv;
	int i;

	rv=(cs-ctx->chn_base)&(TKELZ_CHAIN_SZ-1);

	h=TgvLz_CalcHash(cs);
	
	hr=ctx->hrov[h];
	hr=(hr-1)&TKELZ_HASH_DN1;
	ctx->hrov[h]=hr;
	ctx->hash[(h*TKELZ_HASH_DN)+hr]=cs;

#ifdef TKELZ_HASHB_SZ
	h=TgvLz_CalcHashB(cs);
	hr=ctx->hrovb[h];
	hr=(hr-1)&TKELZ_HASHB_DN1;
	ctx->hrovb[h]=hr;
	ctx->hashb[(h*TKELZ_HASHB_DN)+hr]=cs;
#endif

	h=TgvLz_CalcHashC(cs);

	ctx->chn_ptrs[rv]=cs;
	ctx->chn_next[rv]=ctx->chn_hash[h];
	ctx->chn_hash[h]=rv;

#if 1
	if((h==ctx->pred_h1) && (ctx->pred_l1<8))
		ctx->pred_cs1=NULL;
	if((h==ctx->pred_h2) && (ctx->pred_l2<8))
		ctx->pred_cs2=NULL;
	if((h==ctx->pred_h3) && (ctx->pred_l3<8))
		ctx->pred_cs3=NULL;
	if((h==ctx->pred_h4) && (ctx->pred_l4<8))
		ctx->pred_cs4=NULL;
#endif

	return(0);
}

int TgvLz_HashStr(TgvLz_Context *ctx, byte *cs, int l)
{
	byte *cse;
	
	cse=cs+l;
	while(cs<cse)
	{
		TgvLz_HashByte(ctx, cs);
		cs++;
	}
	return(0);
}

int TgvLz_LookupMatch(TgvLz_Context *ctx,
	byte *str, byte *stre,
	int *rl, int *rd)
{
	byte *cs1, *cs2, *cse;
	int h, hr, bl, bd, bmc, tmc;
	int i, j, k, l, d, n, ml, md, ld;

#if 1
	if(str==ctx->pred_cs4)
	{
		bl=ctx->pred_l4;	bd=ctx->pred_d4;
		*rl=bl;				*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512) && (ctx->cmp==3)));
	}
	if(str==ctx->pred_cs3)
	{
		bl=ctx->pred_l3;	bd=ctx->pred_d3;
		*rl=bl;				*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512) && (ctx->cmp==3)));
	}
	if(str==ctx->pred_cs2)
	{
		bl=ctx->pred_l2;	bd=ctx->pred_d2;
		*rl=bl;				*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512) && (ctx->cmp==3)));
	}
	if(str==ctx->pred_cs1)
	{
		bl=ctx->pred_l1;	bd=ctx->pred_d1;
		*rl=bl;				*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512) && (ctx->cmp==3)));
	}
#endif

	ml=ctx->maxlen;
	md=ctx->maxdist;
//	cse=str+259;
//	cse=str+1023;
	cse=str+ml;
	if(cse>stre)
		cse=stre;
	
	if((cse-str)<48)
	{
		*rl=0;
		*rd=0;
		return(0);
	}
	
	h=TgvLz_CalcHash(str);
	hr=ctx->hrov[h];
	
	bl=0; bd=0; bmc=0;
	tmc=0;

	for(i=1; i<16; i++)
	{
		cs1=str-i;

		if(!cs1 || (cs1<(ctx->css)) || (cs1>=ctx->cse))
			break;
		l=TgvLz_CheckMatch(str, cs1, cse);
		d=str-cs1;
		if(d<=0)
			continue;
		tmc=TgvLz_CalcMatchGain(ctx, l, d);
		tmc=l;
//		if((l>bl) && (d<md))
//		if(((l>bl) && (d<md)) || ((l==bl) && (d<bd)))
//		if((tmc>bmc) && (d<md))
		if((tmc>bmc) && (d<md) || ((l==bl) && (d<bd)))
			{ bl=l; bd=d; bmc=tmc; }
	}
	
	for(i=0; i<TKELZ_HASH_DN; i++)
	{
		cs1=ctx->hash[(h*TKELZ_HASH_DN)+((hr+i)&TKELZ_HASH_DN1)];
//		if(!cs1)continue;
		if(!cs1 || (cs1<(ctx->css)) || (cs1>=ctx->cse))
			break;
		l=TgvLz_CheckMatch(str, cs1, cse);
		d=str-cs1;
		if(d<=0)
			break;
//		if((l>bl) && (d<65536))
//		if((l>bl) && (d<131072))
		tmc=TgvLz_CalcMatchGain(ctx, l, d);
//		tmc=l;
//		if((l>bl) && (d<md))
//		if(((l>bl) && (d<md)) || ((l==bl) && (d<bd)))
//		if((tmc>bmc) && (d<md))
		if((tmc>bmc) && (d<md) || ((l==bl) && (d<bd)))
			{ bl=l; bd=d; bmc=tmc; }
	}

#ifdef TKELZ_HASHB_SZ
	h=TgvLz_CalcHashB(str);
	hr=ctx->hrovb[h];
	
//	bl=0; bd=0;
	
	for(i=0; i<TKELZ_HASHB_DN; i++)
	{
		cs1=ctx->hashb[(h*TKELZ_HASHB_DN)+((hr+i)&TKELZ_HASHB_DN1)];
//		if(!cs1)continue;
		if(!cs1 || (cs1<(ctx->css)) || (cs1>=ctx->cse))
			break;
		l=TgvLz_CheckMatch(str, cs1, cse);
		d=str-cs1;
		if(d<=0)
			break;
		tmc=TgvLz_CalcMatchGain(ctx, l, d);
//		tmc=l;
//		if((l>bl) && (d<md))
//		if(((l>bl) && (d<md)) || ((l==bl) && (d<bd)))
//		if((tmc>bmc) && (d<md))
		if((tmc>bmc) && (d<md) || ((l==bl) && (d<bd)))
			{ bl=l; bd=d; bmc=tmc; }
	}
#endif

//	ctx->chn_ptrs[rv]=cs;
//	ctx->chn_next[rv]=ctx->chn_hash[h];
//	ctx->chn_hash[h]=rv;

	h=TgvLz_CalcHashC(str);

	n=ctx->maxdepth;
	i=ctx->chn_hash[h];
	cs2=str;
//	n=1024;
	ld=0;
	while(n--)
	{
		cs1=ctx->chn_ptrs[i];
//		if(!cs1)continue;
		if(!cs1 || (cs1<(ctx->css)) || (cs1>=ctx->cse))
			break;

		d=str-cs1;
		if(d<=ld)
			break;
		ld=d;
		if(d>md)
			break;

		if(cs1>=cs2)
			break;
		cs2=cs1;

		l=TgvLz_CheckMatch(str, cs1, cse);		
//		if((l>bl) && (d<md))
//			{ bl=l; bd=d; }

		tmc=TgvLz_CalcMatchGain(ctx, l, d);
//		tmc=l;
//		if((l>bl) && (d<md))
//		if(((l>bl) && (d<md)) || ((l==bl) && (d<bd)))
//		if((tmc>bmc) && (d<md))
		if((tmc>bmc) && (d<md) || ((l==bl) && (d<bd)))
			{ bl=l; bd=d; bmc=tmc; }

		j=i;
		i=ctx->chn_next[i];
		if(i==j)
			break;
//		cs2=ctx->chn_ptrs[i];
//		if(cs2>=cs1)
//			break;
	}

	ctx->pred_cs4=ctx->pred_cs3;
	ctx->pred_l4=ctx->pred_l3;
	ctx->pred_d4=ctx->pred_d3;
	ctx->pred_h4=ctx->pred_h3;

	ctx->pred_cs3=ctx->pred_cs2;
	ctx->pred_l3=ctx->pred_l2;
	ctx->pred_d3=ctx->pred_d2;
	ctx->pred_h3=ctx->pred_h2;

	ctx->pred_cs2=ctx->pred_cs1;
	ctx->pred_l2=ctx->pred_l1;
	ctx->pred_d2=ctx->pred_d1;
	ctx->pred_h2=ctx->pred_h1;

	ctx->pred_cs1=str;
	ctx->pred_l1=bl;
	ctx->pred_d1=bd;
	ctx->pred_h1=h;

	*rl=bl;
	*rd=bd;
	return((bl>=4) ||
		((bl>=3) && (bd<512) && (ctx->cmp==3)));
}

int tgvlz_log2u(int val)
{
	int i, v;
	
	v=val; i=0;
	while(v)
		{ i++; v=v>>1; }
	return(i);
}

int TgvLz_EstRawCost(TgvLz_Context *ctx, int rl)
{
	int i, j, c;

	if(ctx->cmp==4)
	{
		c=rl+1;
		if(rl>=15)
		{
			i=rl-15;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		c+=3;
		return(c);
	}

	c=rl;
	i=rl;
	while(i>=4096)
		{ c+=2; i-=4096; }
	if(i>(128+7))
		{ c+=2; i=i&7; }
	if(i>=8)
		{ c+=1; i=i&7; }

//	c=(rl>>3)+rl;
//	c=(rl>>7)+rl+(rl>=8);
	return(c);
}

int TgvLz_EstMatchCost(TgvLz_Context *ctx, int rl, int bl, int bd)
{
	int c, i;

	if(ctx->cmp==4)
	{
//		c=rl+1;
		c=rl+3;
		if(rl>=15)
		{
			i=rl-15;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		if(bl>=19)
		{
			i=bl-19;
			while(i>=255)
				{ c++; i-=255; }
			c++;
		}
		
		return(c);
	}

//	c=(rl>>3)+rl;
//	c=(rl>>7)+rl+(rl>=8);
	c=TgvLz_EstRawCost(ctx, rl);
	if((bl<=10) && (bd<512))
		{ c+=2; }
	else if((bl<=26) && (bd<8) && (ctx->cmpver==2))
		{ c+=2; }
	else if((bl<=67) && (bd<8192))
		{ c+=3; }
	else if((bl<=323) && (bd<128) && (ctx->cmpver==2))
		{ c+=3; }
	else if((bl<=515) && (bd<131072))
		{ c+=4; }
	else if((bl<=16383) && (bd<(1<<22)))
	{
		if(ctx->cmpver==0)
		{
			c+=1+((bl<128)?1:2)+((bd<32768)?2:3);
		}else
		{
			if(bl<=67)
				c+=5;
			else
				c+=6;
		}
	}
	return(c);
}

int TgvLz_LookupMatchB(TgvLz_Context *ctx,
	byte *cs, byte *cse, byte *lcs,
	int *rl, int *rd)
{
	int l, d, l1, d1;
	int rl0, rl1, rl2, rl3;
	int mc0, mc1;
	int i, j, k;

	i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
	
	if(i<=0)
	{
		*rl=0;
		*rd=0;
		return(0);
	}
	
	rl0=cs-lcs;
	rl1=rl0+1;
	rl2=rl0+2;
	rl3=rl0+3;
	
	mc0=TgvLz_EstMatchCost(ctx, rl0, l, d);
	mc1=TgvLz_EstRawCost(ctx, rl0+l);
	
	if(mc0>=mc1)
	{
		/* Skip match if it costs more than it saves. */
		i=0;
	}
	
	if(i && (d>=8192) && (l<6))
		i=0;
	if(i && (d>=131072) && (l<12))
		i=0;
	
	if(i)
	{
		/* Check if a better match is just around the corner. */

		j=TgvLz_LookupMatch(ctx, cs+1, cse, &l1, &d1);
		if(j && (l1>(l+1)) && ((rl0>>3)==(rl1>>3)))
			i=0;

		mc1=TgvLz_EstMatchCost(ctx, rl1, l1, d1);
//		if(j && (l1>(l+1)) && (mc1<=mc0))
//		if(j && (l1>(l+0)) && (mc1<=mc0))
		if(j && ((mc1-l1)<=(mc0-l)))
			i=0;

//		if(j && (l1>(l+1)))
//		if(j && (l1>(l+2)))
//			i=0;

#if 1
		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);

			mc1=TgvLz_EstMatchCost(ctx, rl2, l1, d1);
//			if(j && (l1>(l+0)) && (mc1<=mc0))
			if(j && ((mc1-l1)<(mc0-l)))
//			if(j && ((mc1-l1)<=(mc0-l)))
				i=0;
		}
#endif

#if 1
		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+3, cse, &l1, &d1);

			mc1=TgvLz_EstMatchCost(ctx, rl3, l1, d1);
			if(j && ((mc1-l1)<(mc0-l)))
				i=0;
		}
#endif
	}
	
	*rl=l;
	*rd=d;
	return(i);
}

void TgvLz_MatchCopy(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;
	u64 v;
	int d;
	
	d=dst-src;
	if(d<8)
	{
		if(d==1)
		{
			v=*src;
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				ct+=8;
			}
		}else
			if(d==2)
		{
//			v=*(u16 *)src;
			v=get_u16le(src);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				ct+=8;
			}
		}else
			if(d==4)
		{
//			v=*(u32 *)src;
			v=get_u32le(src);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				ct+=8;
			}
		}else
		{
//			v=*(u64 *)src;
			v=get_u64le(src);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				ct+=d;
			}

#if 0
			cs=src;
			ct=dst; cte=dst+sz;
			while(ct<cte)
				{ *ct++=*cs++; }
#endif
		}
	}else
	{
		cs=src;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
//			*(u64 *)ct=*(u64 *)cs;
			memcpy_8B(ct, cs);
			ct+=8; cs+=8;
		}
	}
}

#if 1
// force_inline
void TgvLz_MatchCopy2(byte *dst, int sz, int d)
{
	byte *cs, *ct, *cte;
	u64 v, v1;
	
	if(d<8)
	{
		if(d==1)
		{
//			v=*src;
			v=*(dst-d);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);

			if(sz<=16)
			{
				set_u64le(dst, v);
				set_u64le(dst+8, v);
				return;
			}

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				set_u64le(ct+8, v);
				ct+=16;
			}
		}else
			if(d==2)
		{
//			v=*(u16 *)src;
//			v=*(u16 *)(dst-d);
			v=get_u16le(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				set_u64le(ct+8, v);
				ct+=16;
			}
		}else
			if(d==4)
		{
//			v=*(u32 *)src;
//			v=*(u32 *)(dst-d);
			v=get_u32le(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				set_u64le(ct+8, v);
				ct+=16;
			}
		}else
			if(d==3)
		{
//			v=*(u32 *)src;
//			v=*(u32 *)(dst-d);
			v=get_u32le(dst-d);
			v&=0x00FFFFFF;
			v=v|(v<<24);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				set_u64le(ct+6, v);
				ct+=12;
			}
		}else
		{
			if(!d)
			{
				/* OK, this isn't ideal... */
				return;
			}
//			v=*(u64 *)src;
//			v=*(u64 *)(dst-d);
			v=get_u64le(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
//		((u64 *)dst)[0]=((u64 *)cs)[0];
//		((u64 *)dst)[1]=((u64 *)cs)[1];
//		memcpy_16B(dst, cs);
		ct=dst;
		memcpy_8B(ct+0, cs+0);
		memcpy_8B(ct+8, cs+8);
	}else
		if(sz<=32)
	{
		cs=dst-d;
		ct=dst;
		memcpy_8B(ct+ 0, cs+ 0);
		memcpy_8B(ct+ 8, cs+ 8);
		memcpy_8B(ct+16, cs+16);
		memcpy_8B(ct+24, cs+24);
	}else
		if(d<16)
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
			memcpy_8B(ct+0, cs+0);
			memcpy_8B(ct+8, cs+8);
			ct+=16; cs+=16;
		}
	}else
//		if((d<sz) || (sz<256))
		if(1)
	{
//		cs=src;
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
#if 1
//			((u64 *)ct)[0]=((u64 *)cs)[0];
//			((u64 *)ct)[1]=((u64 *)cs)[1];
			memcpy_16B(ct, cs);
			ct+=16; cs+=16;
#endif
		}
	}else
	{
		memcpy(dst, dst-d, sz);
	}
}
#endif

/* Match Copy, but only for lengths under 16 bytes. */
// force_inline
void TgvLz_MatchCopyShort(byte *dst, int sz, int d)
{
	byte *src, *ct;
	u64 v0, v1;
	
	src=dst-d;

#if 0
	if(d>=16)
	{
		memcpy_16B(dst, src);
		return;
	}
#endif

	if(d>=8)
	{
		memcpy_8B(dst, src);
		memcpy_8B(dst+8, src+8);
		return;
	}
	
	v0=get_u64le(src);

	if(d>=4)
	{
		if(d>4)
		{
			ct=dst;
			set_u64le(ct, v0); ct+=d;
			set_u64le(ct, v0); ct+=d;
			set_u64le(ct, v0); ct+=d;
			set_u64le(ct, v0);
			return;
		}
		v1=((u32)v0)|(v0<<32);
		set_u64le(dst, v1);
		set_u64le(dst+8, v1);
		return;
	}
	if(d>=2)
	{
		if(d==3)
		{
			v1=v0&0x00FFFFFF;
			v1|=v1<<24;
			set_u64le(dst, v1);
			set_u64le(dst+ 6, v1);
			set_u64le(dst+12, v1);
			return;
		}
		v1=(u16)v0;
		v1|=v1<<16;
		v1|=v1<<32;
		set_u64le(dst, v1);
		set_u64le(dst+8, v1);
		return;
	}

	v1=(byte)v0;
	v1|=v1<< 8;
	v1|=v1<<16;
	v1|=v1<<32;
	set_u64le(dst, v1);
	set_u64le(dst+8, v1);
	return;
}

// force_inline
void TgvLz_RawCopy(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	if(sz>8)
	{
		cs=src;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
#if 0
//			*(u64 *)ct=*(u64 *)cs;
			memcpy_8B(ct, cs);
			ct+=8; cs+=8;
#endif
#if 1
//			((u64 *)ct)[0]=((u64 *)cs)[0];
//			((u64 *)ct)[1]=((u64 *)cs)[1];
			memcpy_16B(ct, cs);
			ct+=16; cs+=16;
#endif
		}
	}else
		if(sz)
	{
//		*(u64 *)dst=*(u64 *)src;
		memcpy_8B(dst, src);
	}
}

// force_inline
void TgvLz_RawCopyC(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte, *cte_m16;

	cs=src;
	ct=dst; cte=dst+sz;

#if 0
	if(sz<=32)
	{
		if(ct<cte)
		{
			memcpy_8B(ct, cs);
			ct+=8; cs+=8;
			if(ct<cte)
			{
				memcpy_8B(ct, cs);
				ct+=8; cs+=8;
				if(ct<cte)
				{
					memcpy_8B(ct, cs);
					ct+=8; cs+=8;
					if(ct<cte)
						memcpy_8B(ct, cs);
				}
			}
		}
		return;
	}
#endif

//	if(sz>=40)
//	if(1)
	if(sz&15)
	{
		cte_m16=cte-16;
//		cte_m16=cte-((~((sz&15)-1))&16);
//		while(ct<cte)
		while(ct<=cte_m16)
		{
#if 0
			memcpy_8B(ct, cs);
			ct+=8; cs+=8;
#endif
#if 1
			memcpy_16B(ct, cs);
			ct+=16; cs+=16;
#endif
		}
		if(ct<cte)
		{
			memcpy_8B(ct, cs);
			ct+=8; cs+=8;
			if(ct<cte)
				memcpy_8B(ct, cs);
		}
	}else
	{
#if 1
		while(ct<cte)
		{
//			memcpy_8B(ct, cs);
//			ct+=8; cs+=8;
			memcpy_16B(ct, cs);
			ct+=16; cs+=16;
		}
#endif
	}
}

void TgvLz_RawCopyB(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte, *cte_m16;

	cs=src;
	ct=dst; cte=dst+sz;
	while(ct<cte)
	{
//		memcpy_8B(ct, cs);
//		ct+=8; cs+=8;
		memcpy_16B(ct, cs);
		ct+=16; cs+=16;
	}	
}

int TgvLz_EncodeBufferRP2I(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz, int flag)
{
	byte *cs, *cse, *lcs;
	byte *ct;
	u32 v;
	int pl, pd;
	int l, d, rl, l1, d1;
	int i, j, k;
	
	ctx->chn_base=ibuf;
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	
	ctx->css=ibuf;
	ctx->cse=cse;

	pd=0; pl=0;
	
	lcs=cs;
	while(cs<cse)
	{
//		i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
		i=TgvLz_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

		if(!i)
		{
			TgvLz_HashByte(ctx, cs);
			cs++;
			continue;
		}
			
		rl=cs-lcs;

		ctx->stat_rlen[tgvlz_log2u(rl)]++;
		ctx->stat_len[tgvlz_log2u(l)]++;
		ctx->stat_dist[tgvlz_log2u(d)]++;

#if 1
//		while(rl>128)
		while(rl>(128+7))
		{
			j=(rl>>3)-1;
			if(j>511)j=511;
			
			k=(j+1)*8;

			v=0x3F+(j<<7);
			*ct++=v;
			*ct++=v>>8;

			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}
#endif

		while(rl>=8)
		{
			j=(rl>>3)-1;
			if(j>15)j=15;
			
			k=(j+1)*8;
			*ct++=0x07+(j<<4);
			memcpy(ct, lcs, k);
			ct+=k;	lcs+=k;	rl-=k;
		}

#if 1
		if((rl<8) && (l<=10) && (d<=511))
		{
			d1=d;
			l1=l-3;
			v=(d1<<7)|(l1<<4)|(rl<<1)|0;
			*ct++=v;
			*ct++=v>>8;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((flag&2) && (rl<8) && (l>=11) && (l<=26) && (d>=1) && (d<=8))
		{
			d1=d-1;
			l1=l-11;
			v=(d1<<13)|(l1<<9)|(rl<<5)|0x000F;
			*ct++=v;
			*ct++=v>>8;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
//		if((flag&1) && (rl==0) && (l>=4) && (l<=131) && (d==1))
//		if((flag&1) && (rl==0) && (l>=16) && (l<=131) && (d==1))
		if((flag&1) && (rl==0) && (l>=32) && (l<=131) && (d==1))
		{
			l1=l-4;
			v=(l1<<9)|0x007F;
			*ct++=v;
			*ct++=v>>8;
		}else
#endif
#if 1
		if((rl<8) && (l<=67) && (d<=8191))
		{
			d1=d;
			l1=l-4;

			v=(d1<<11)|(l1<<5)|(rl<<2)|1;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((flag&2) && (rl<8) && (l>=68) && (l<=323) && (d>=1) && (d<=128))
		{
			d1=d-1;
			l1=l-68;
			v=(d1<<17)|(l1<<9)|(rl<<5)|0x010F;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((rl<8) && (l<=515) && (d<=131071))
		{
			d1=d;
			l1=l-4;

			v=(d1<<15)|(l1<<6)|(rl<<3)|3;
			*ct++=v;
			*ct++=v>>8;
			*ct++=v>>16;
			*ct++=v>>24;

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((flag&1) && (rl<8) && (l<=67) && (d<=0x3FFFFF))
		{
			d1=d;
			l1=l-4;
			v=(d1<<18)|(l1<<12)|(rl<<9)|0x00FF;
			*ct++=v >> 0;
			*ct++=v >> 8;
			*ct++=v >>16;
			*ct++=v >>24;
			*ct++=d1>>14;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((flag&1) && (rl<8) && (l<=0x3FFF) && (d<=0x3FFFFF))
		{
			d1=d;
			l1=l-4;
			v=(d1<<26)|(l1<<12)|(rl<<9)|0x017F;
			*ct++=v >> 0;
			*ct++=v >> 8;
			*ct++=v >>16;
			*ct++=v >>24;
			*ct++=d1>> 6;
			*ct++=d1>>14;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if(!(flag&3) && (rl<8) && (l<=0x3FFF) && (d<=0x3FFFFF))
		{
			*ct++=0x0F|(rl<<5);
			d1=d;
			l1=l-4;

			if(l1<128)
			{
				*ct++=(l1<<1);
			}else
			{
				v=(l1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
			}

			if(d1<32768)
			{
				v=d1<<1;
				*ct++=v;
				*ct++=v>>8;
			}else
			{
				v=(d1<<2)|1;
				*ct++=v;
				*ct++=v>>8;
				*ct++=v>>16;
			}

			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
		{
			debug_break
		}
		
		pl=l; pd=d;
		
		TgvLz_HashStr(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

	l1=(cs-48)-lcs;
	while(l1>=128)
	{
		j=(l1>>3)-1;
		if(j>511)j=511;
		
		k=(j+1)*8;

		v=0x3F+(j<<7);
		*ct++=v;
		*ct++=v>>8;

		memcpy(ct, lcs, k);
		ct+=k;	lcs+=k;	rl-=k;

		l1=(cs-48)-lcs;
	}

	rl=cs-lcs;
	
	while(rl>8)
	{
		j=(rl>>3)-1;
		if(j>15)j=15;
		
		k=(j+1)*8;
		*ct++=0x07+(j<<4);
		memcpy(ct, lcs, k);
		ct+=k;	rl-=k;	lcs+=k;
	}

	while(rl)
	{
		j=rl;
		if(j>3)j=3;

		*ct++=0x1F+(j<<6);
		memcpy(ct, lcs, j);
		ct+=j;	rl-=j;	lcs+=j;
	}

	*ct++=0x1F;
	
	return(ct-obuf);
}

int TgvLz_EncodeBufferRP2(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_EncodeBufferRP2I(ctx, ibuf, obuf, ibsz, obsz, 0));
}

int TgvLz_EncodeBufferRP2B(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_EncodeBufferRP2I(ctx, ibuf, obuf, ibsz, obsz, 1));
}

int TgvLz_EncodeBufferRP2C(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_EncodeBufferRP2I(ctx, ibuf, obuf, ibsz, obsz, 3));
}

int TgvLz_EncodeBufferRP2C_NoCtx(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	static TgvLz_Context *st_ctx;
	if(!st_ctx)
		{ st_ctx=TgvLz_CreateContextRP2C(); }
	return(TgvLz_EncodeBufferRP2I(st_ctx, ibuf, obuf, ibsz, obsz, 3));
}

int TgvLz_DecodeBufferRP2I(
	byte *ibuf, byte *obuf, int ibsz, int obsz, int flag)
{
	u32 tag;
	byte *cs, *ct, *cse, *cte;
//	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf; cte=obuf+obsz;
	rl=0; l=0; d=0;
	
//	while(1)
	while((cs<cse) && (ct<cte))
	{
//		t0=*(u32 *)cs;
//		t0=*(u64 *)cs;
		t0=get_u64le(cs);
		if(!(t0&0x01))
		{
			cs+=2;
			rl=(t0>>1)&7;
			l=((t0>>4)&7)+3;
			d=(t0>>7)&511;
		}else
			if(!(t0&0x02))
		{
			cs+=3;
			rl=(t0>>2)&7;
			l=((t0>>5)&63)+4;
			d=(t0>>11)&8191;
		}else
			if(!(t0&0x04))
		{
			cs+=4;
			rl=(t0>>3)&7;
			l=((t0>>6)&511)+4;
			d=(t0>>15)&131071;
		}else
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			rl=(t1+1)*8;
			if(flag&4)
			{
				if((ct+rl)>cte)
					return(-1);
			}
//			TgvLz_RawCopyB(ct, cs, rl);
			TgvLz_RawCopyC(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x10))
		{
			if(flag&2)
			{
				/* RP2C */
				if(t0&0x100)
				{
					rl=(t0>> 5)&7;
					l=((t0>> 9)&255)+68;
					d=((t0>>17)&127)+1;
					cs+=3;
				}else
				{
					rl=(t0>> 5)&7;
					l=((t0>> 9)&15)+11;
					d=((t0>>13)& 7)+1;
					cs+=2;
				}
			}else
				if(!(flag&1) || !(flag&4))
			{
				/* RP2 Original, Long Match */
				cs++;
				rl=(t0>>5)&7;
				t1=t0>>8;
				if(!(t1&1))
					{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
				else if(!(t1&2))
					{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
				else
				{
					if(flag&4)
						return(-1);
					debug_break
				}

				if(!(t2&1))
					{ d=((t2>>1)&0x007FFF); cs+=2; }
				else if(!(t2&2))
					{ d=((t2>>2)&0x3FFFFF); cs+=3; }
				else
				{
					if(flag&4)
						return(-1);
					debug_break
				}
			}else
			{
				if(flag&4)
					return(-1);
				debug_break
			}
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
//			*(u32 *)ct=*(u32 *)cs;
//			memcpy_4B(ct, cs);
//			cs+=rl;
//			ct+=rl;

			/* Slower, but exact */
			while(rl--)
				*ct++=*cs++;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
			if(flag&4)
			{
				if((ct+rl)>cte)
					return(-1);
			}
			TgvLz_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x80))
		{
			if(t0&0x100)
			{
				rl=(t0>>9)&7;
				l=((t0>>12)&16383)+4;
				d=(t0>>26)&0x3FFFFF;
				cs+=6;
			}else
			{
				rl=0;
				l=((t0>> 9)&127)+4;
				d=1;
				cs+=2;
			}
		}else
			if(!(t0&0x0100))
		{
			rl=(t0>>9)&7;
			l=((t0>>12)&63)+4;
			d=(t0>>18)&0x3FFFFF;
			cs+=5;
		}else
		{
			debug_break
		}

#if 1
		if(flag&4)
		{
			if(!d || ((ct+(rl-d))<obuf))
				return(-1);
			if(ct+(rl+l)>cte)
				return(-1);
		}
#endif

//		*(u64 *)ct=*(u64 *)cs;
		memcpy_8B(ct, cs);
		cs+=rl;
		ct+=rl;
		TgvLz_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}

#if !defined(__BJX2__) && !defined(__XG3__) && !defined(__riscv__)
/* Use branch-free handling for top 3 cases.
 * Don't use on XG3 or RISC-V as it will probably be slower...
 */
// #define TGVLZ_DECRP2C_TOP3ALU
#endif

/* Faster case for RP2B/C, assumes well-formed input. */
int TgvLz_DecodeBufferRP2CI(
	byte *ibuf, byte *obuf, int ibsz, int obsz, int flag)
{
	u32 tag;
	byte *cs, *ct, *cse, *cte;
#ifdef TGVLZ_DECRP2C_TOP3ALU
	int teo, eo, te_x0, te_x1, te_x2;
	int sml, smd, smt, omr, oml, omt;
	int mml, mmd, bml;
	int mr, ml, md;
#endif
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf; cte=obuf+obsz;
	rl=0; l=0; d=0;
	
//	while(1)
//	while((cs<cse) && (ct<cte))
	while(cs<cse)
	{
		t0=get_u64le(cs);

#ifdef TGVLZ_DECRP2C_TOP3ALU
		teo=(t0&7);
		if(teo!=7)
		{
			/* tortured branch-free arithmetic... */
			te_x0=(teo&1);			te_x1=(teo>>1)&1;
			te_x2=te_x0&te_x1;
			eo=te_x0+te_x2;
			omr=eo+1;
			oml=omr+3;				smt=(eo<<3)+(12-eo);
			mr=(t0>>omr)&7;			ml=t0>>oml;
			bml=(3+te_x0);			sml=((5*smt)>>4)+te_x0;
			smd=smt-sml;			md=ml>>sml;
			mml=((1<<sml)-1);		mmd=((1<<smd)-1);
			ml=(ml&mml)+bml;		md=(md&mmd);

			cs+=eo+2;
			memcpy_8B(ct, cs);
			cs+=mr;	ct+=mr;
			TgvLz_MatchCopy2(ct, ml, md);
			ct+=ml;
			continue;
		}else
#else
		if((t0&7)!=7)
		{
			if(!(t0&0x01))
			{
				cs+=2;
				rl=(t0>>1)&7;
				l=((t0>>4)&7)+3;
				d=(t0>>7)&511;
			}else
				if(!(t0&0x02))
			{
				cs+=3;
				rl=(t0>>2)&7;
				l=((t0>>5)&63)+4;
				d=(t0>>11)&8191;
			}else
			{
				cs+=4;
				rl=(t0>>3)&7;
				l=((t0>>6)&511)+4;
				d=(t0>>15)&131071;
			}
		}else
#endif
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			if(t1)
			{
				if(t1>=2)
				{
					rl=(t1+1)*8;
		//			TgvLz_RawCopyB(ct, cs, rl);
					TgvLz_RawCopyC(ct, cs, rl);
					cs+=rl;
					ct+=rl;
				}else
				{
					memcpy_16B(ct, cs);
					cs+=16;	ct+=16;
				}
			}else
			{
				memcpy_8B(ct, cs);
				cs+=8;	ct+=8;
			}
			continue;
		}else
			if(!(t0&0x10))
		{
			if(t0&0x100)
			{
				rl=(t0>> 5)&7;
				l=((t0>> 9)&255)+68;
				d=((t0>>17)&127)+1;
				cs+=3;
			}else
			{
				rl=(t0>> 5)&7;
				l=((t0>> 9)&15)+11;
				d=((t0>>13)& 7)+1;
				cs+=2;
			}
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
			memcpy_4B(ct, cs);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
			TgvLz_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x80))
		{
			if(t0&0x100)
			{
				rl=(t0>>9)&7;
				l=((t0>>12)&16383)+4;
				d=(t0>>26)&0x3FFFFF;
				cs+=6;
			}else
			{
				rl=0;
				l=((t0>> 9)&127)+4;
				d=1;
				cs+=2;
			}
		}else
			if(!(t0&0x0100))
		{
			rl=(t0>>9)&7;
			l=((t0>>12)&63)+4;
			d=(t0>>18)&0x3FFFFF;
			cs+=5;
		}else
		{
			debug_break
		}

		memcpy_8B(ct, cs);
		cs+=rl;
		ct+=rl;
		TgvLz_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}

int TgvLz_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_DecodeBufferRP2I(ibuf, obuf, ibsz, obsz, 0));
}

int TgvLz_DecodeBufferRP2B(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
//	return(TgvLz_DecodeBufferRP2I(ibuf, obuf, ibsz, obsz, 1));
	return(TgvLz_DecodeBufferRP2CI(ibuf, obuf, ibsz, obsz, 1));
}

int TgvLz_DecodeBufferRP2C(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
//	return(TgvLz_DecodeBufferRP2I(ibuf, obuf, ibsz, obsz, 3));
	return(TgvLz_DecodeBufferRP2CI(ibuf, obuf, ibsz, obsz, 3));
}

/* Decode RP2 Original
 * Lint: Runs some sanity checks and errors out. */
int TgvLz_DecodeBufferLintRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_DecodeBufferRP2I(ibuf, obuf, ibsz, obsz, 4));
}

/* Decode RP2B
 * Lint: Runs some sanity checks and errors out. */
int TgvLz_DecodeBufferLintRP2B(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_DecodeBufferRP2I(ibuf, obuf, ibsz, obsz, 5));
}

/* Decode RP2C
 * Lint: Runs some sanity checks and errors out. */
int TgvLz_DecodeBufferLintRP2C(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_DecodeBufferRP2I(ibuf, obuf, ibsz, obsz, 7));
}

int TgvLz_EncodeBufferLZ4(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *cse, *lcs;
	byte *ct;
	int pl, pd;
	int l, d, rl, l1, d1;
	int i, j, k;
	
	ctx->chn_base=ibuf;
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	
	ctx->css=ibuf;
	ctx->cse=cse;

	pd=0; pl=0;
	
	lcs=cs;
	while(cs<cse)
	{
		i=TgvLz_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

		if(!i)
		{
			TgvLz_HashByte(ctx, cs);
			cs++;
			continue;
		}
			
		rl=cs-lcs;

		ctx->stat_rlen[tgvlz_log2u(rl)]++;
		ctx->stat_len[tgvlz_log2u(l)]++;
		ctx->stat_dist[tgvlz_log2u(d)]++;

		l1=l-4;
		j=((rl<<4)&0xF0)|(l1&0x0F);
		if(rl>=15)j|=0xF0;
		if(l1>=15)j|=0x0F;
		*ct++=j;
		
		if(rl>=15)
		{
			j=rl-15;
			while(j>=255)
				{ *ct++=0xFF; j-=255; }
			*ct++=j;
		}

		memcpy(ct, lcs, rl);
		ct+=rl;
		lcs+=rl;

		*ct++=d;
		*ct++=d>>8;
		
		if(l>=19)
		{
			j=l-19;
			while(j>=255)
				{ *ct++=0xFF; j-=255; }
			*ct++=j;
		}

		TgvLz_HashStr(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

	rl=cs-lcs;

	if(rl>0)
	{
		j=(rl<<4)&0xF0;
		if(rl>=15)j|=0xF0;
		*ct++=j;	
		if(rl>=15)
		{
			j=rl-15;
			while(j>=255)
				{ *ct++=0xFF; j-=255; }
			*ct++=j;
		}

		memcpy(ct, lcs, rl);
		ct+=rl;
		lcs+=rl;
	}

	return(ct-obuf);
}

int TgvLz_DecodeBufferLZ4(byte *ibuf, byte *obuf, int isz, int osz)
{
	byte *ct, *cs, *cse;
	byte *cs1, *cs1e, *ct1;
	u64 tv;
	int tg, lr, ll, ld;
	int i;
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	ct=obuf;
	cs=ibuf; cse=ibuf+isz;
	while(cs<cse)
	{
		tg=*cs++;
		lr=(tg>>4)&15;
		if(lr==15)
		{
			i=*cs++;
			while(i==255)
				{ lr+=255; i=*cs++; }
			lr+=i;
		}

		if(lr)
		{
			TgvLz_RawCopy(ct, cs, lr);
			cs+=lr;	ct+=lr;
		}

		if((cs+1)>=cse)
			{ break; }
		
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			printf("TKPE_UnpackL4: End Of Image\n");
			break;
		}
		ll=(tg&15)+4;
		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}

		TgvLz_MatchCopy2(ct, ll, ld);
		ct+=ll;
	}
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	return(ct-obuf);
}

int TgvLz_DecodeBufferLR3C(byte *ibuf, byte *obuf, int isz, int osz)
{
	byte *cs, *ct, *cse, *cte;
	u64 tg;
	int eo, mr, ml, md, smt, smd, sml;
	
	cs=ibuf; cse=ibuf+isz;
	ct=obuf; cte=obuf+osz;
	
	while(cs<cse)
	{
		tg=get_u64le(cs);
		eo=tg&3;
		if(eo!=3)
		{
			cs+=eo+2;
			mr=(tg>>2)&7;
			ml=tg>>5;
			smt=(eo<<3)+11;
			sml=(11*smt)>>5;
			smd=smt-sml;
			md=ml>>sml;
			ml=(ml&((1<<sml)-1))+4;
			md=(md&((1<<smd)-1))+1;

			memcpy_8B(ct, cs);
			cs+=mr;	ct+=mr;
			TgvLz_MatchCopy2(ct, ml, md);
			ct+=ml;
			continue;
		}

		eo=(tg>>2)&3;
		if(eo!=3)
		{
			if(!(tg&0x08))
			{
				cs+=eo+1;
				smt=(eo<<3)+4;
				mr=(tg>>4)&((1<<smt)-1);
				mr=(mr+1)<<3;
				TgvLz_RawCopy(ct, cs, mr);
				cs+=mr;	ct+=mr;
				continue;
			}
			
			cs++;
			mr=(tg>>4)&15;
			if(!mr)
				break;
//			memcpy_16B(ct, cs);
			memcpy(ct, cs, mr);
			cs+=mr;	ct+=mr;
			continue;
		}
	}
	return(ct-obuf);
}

int TgvLz_EncodeBufferLR3C(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *cse, *lcs;
	byte *ct;
	int pl, pd, tg;
	int ml, md, mr, l1, d1;
	int i, j, k;
	
	ctx->chn_base=ibuf;
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	
	ctx->css=ibuf;
	ctx->cse=cse;

	pd=0; pl=0;
	
	lcs=cs;
	while(cs<cse)
	{
		i=TgvLz_LookupMatchB(ctx, cs, cse, lcs, &ml, &md);

		if(i)
		{
			if(ml<4)
				i=0;
			if((ml>515) || (md>262144))
			{
				i=0;
			}
		}

		if(!i)
		{
			TgvLz_HashByte(ctx, cs);
			cs++;
			continue;
		}
			
		mr=cs-lcs;

		ctx->stat_rlen[tgvlz_log2u(mr)]++;
		ctx->stat_len[tgvlz_log2u(ml)]++;
		ctx->stat_dist[tgvlz_log2u(md)]++;

		while(mr>=128)
		{
			k=(mr>>3)-1;
			if(k>4095)
				k=4095;
			tg=0x07|(k<<4);
			*ct++=tg>>0;
			*ct++=tg>>8;
			memcpy(ct, lcs, (k+1)*8);
			ct+=(k+1)*8;
			lcs+=(k+1)*8;
			mr=cs-lcs;
		}

		while(mr>=8)
		{
			k=(mr>>3)-1;
			if(k>15)
				k=15;
			tg=0x03|(k<<4);
			*ct++=tg;
			memcpy(ct, lcs, (k+1)*8);
			ct+=(k+1)*8;
			lcs+=(k+1)*8;
			mr=cs-lcs;
		}

		if((ml<=11) && (md<=256))
		{
			tg=0x00|(mr<<2)|((ml-4)<<5)|((md-1)<<8);
			*ct++=tg>>0;
			*ct++=tg>>8;
		}else
			if((ml<=67) && (md<=8192))
		{
			tg=0x01|(mr<<2)|((ml-4)<<5)|((md-1)<<11);
			*ct++=tg>>0;
			*ct++=tg>>8;
			*ct++=tg>>16;
		}else
			if((ml<=515) && (md<=262144))
		{
			tg=0x02|(mr<<2)|((ml-4)<<5)|((md-1)<<14);
			*ct++=tg>>0;
			*ct++=tg>>8;
			*ct++=tg>>16;
			*ct++=tg>>24;
		}else
		{
			debug_break
		}

		memcpy(ct, lcs, mr);
		ct+=mr;
		lcs+=mr;

		TgvLz_HashStr(ctx, cs, ml);
		cs+=ml;
		lcs=cs;
	}

	mr=cs-lcs;

	while(mr>=128)
	{
		k=(mr>>3)-1;
		if(k>4095)
			k=4095;
		tg=0x07|(k<<4);
		*ct++=tg>>0;
		*ct++=tg>>8;
		memcpy(ct, lcs, (k+1)*8);
		ct+=(k+1)*8;
		lcs+=(k+1)*8;
		mr=cs-lcs;
	}

	while(mr>=48)
	{
		k=(mr>>3)-1;
		if(k>15)
			k=15;
		tg=0x03|(k<<4);
		*ct++=tg;
		memcpy(ct, lcs, (k+1)*8);
		ct+=(k+1)*8;
		lcs+=(k+1)*8;
		mr=cs-lcs;
	}

	while(mr>0)
	{
		k=mr;
		if(k>15)
			k=15;
		tg=0x0B|(k<<4);
		*ct++=tg;
		memcpy(ct, lcs, k);
		ct+=k;
		lcs+=k;
		mr=cs-lcs;
	}

	tg=0x0B;
	*ct++=tg;

	return(ct-obuf);
}


u32 TgvLz_CalculateImagePel4BChecksum(byte *buf, int size)
{
	byte *cs, *cse;
//	u32 v;
	u32 v0, v1, v2, v3;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;
#if 0
	while(cs<cse)
	{
		v=*(u32 *)cs;
		acc_lo=acc_lo+v;

		acc_hi=acc_hi+acc_lo;
		cs+=4;
	}
#endif
#if 1
	while(cs<cse)
	{
//		v0=((u32 *)cs)[0];	v1=((u32 *)cs)[1];
//		v2=((u32 *)cs)[2];	v3=((u32 *)cs)[3];
		v0=get_u32le(cs+0);	v1=get_u32le(cs+ 4);
		v2=get_u32le(cs+8);	v3=get_u32le(cs+12);
		acc_lo=acc_lo+v0;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v1;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v2;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v3;	acc_hi=acc_hi+acc_lo;
		cs+=16;
	}
#endif
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);
	return(csum);
}

u16 TgvLz_CalculateSmallByteCsum(byte *buf, int sz)
{
	byte *cs, *cse;
	u32 ac0, ac1, csum;
	
	if(!sz)
		return(0);

	cs=buf; cse=buf+sz;
	ac0=1; ac1=0;
	while(cs<cse)
	{
		ac0+=*cs;
		ac1+=ac0;
		cs++;
	}
	ac0=((u16)ac0)+(ac0>>16);
	ac1=((u16)ac1)+(ac1>>16);
	ac0=((u16)ac0)+(ac0>>16);
	ac1=((u16)ac1)+(ac1>>16);
	csum=(u16)(ac0^ac1);
	return(csum);
}

u32 TgvLz_CalculateImagePel4BChecksumAc(byte *buf, int sz)
{
	u32 csum0, csum1;

	if(!(sz&15))
	{
		csum0=TgvLz_CalculateImagePel4BChecksum(buf, sz);
		return(csum0);
	}

	csum0=TgvLz_CalculateImagePel4BChecksum(buf, sz&(~15));
	csum1=TgvLz_CalculateSmallByteCsum(buf+(sz&(~15)), sz&15);
	return(csum0^csum1);
}

int TgvLz_DoEncode(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int isz)
{
	long long ttsz;
	byte *obuf2;
	double f, g;
	int t0, t1, te;
	int csum1, csum2;
	int osz, osz2;
	int i, j, k;

//	obuf2=malloc(isz*2);

	osz=ctx->EncodeBuffer(ctx, ibuf, obuf, isz, 2*isz);
	
//	printf("%s: %d -> %d\n", ctx->tstName, isz, osz);

	csum1=TgvLz_CalculateImagePel4BChecksum(ibuf, isz);	
	ctx->csum=csum1;
	return(osz);
}

int TgvLz_DoEncodeSafe(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int isz)
{
	long long ttsz;
	byte *obuf2;
	double f, g;
	int t0, t1, te;
	int csum1, csum2;
	int osz, osz2;
	int i, j, k;

	obuf2=malloc(isz*2+1024);

	osz=ctx->EncodeBuffer(ctx, ibuf, obuf, isz, 2*isz);
	
#if 1
	osz2=ctx->DecodeBuffer(obuf, obuf2, osz, isz*2);
	
	if(osz2!=isz)
	{
		printf("%s: Size mismatch %d->%d\n", ctx->tstName, isz, osz2);
		return(-1);
	}else
	{
		if(memcmp(ibuf, obuf2, isz))
		{
			printf("%s: Data mismatch\n", ctx->tstName);
			return(-1);
		}else
		{
//			printf("%s: Data OK\n", ctx->tstName);
		}
	}

//	*(u64 *)(obuf2+isz+0)=0;
//	*(u64 *)(obuf2+isz+8)=0;
	memset(obuf2+isz, 0, 16);

	csum1=TgvLz_CalculateImagePel4BChecksum(ibuf, isz);
	csum2=TgvLz_CalculateImagePel4BChecksum(obuf2, isz);
	
	if(csum1!=csum2)
	{
		printf("%s: Checksum %08X->%08X\n", ctx->tstName, csum1, csum2);
	}
	
	ctx->csum=csum2;
#endif

	free(obuf2);
	return(osz);
}

static int tgvlz_benchsecs;

int TgvLz_DoTest(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int isz)
{
	long long ttsz;
	byte *obuf2;
	double f, g;
	int t0, t1, te;
	int csum1, csum2;
	int osz, osz2;
	int i, j, k;

	obuf2=malloc(isz*2);

	osz=ctx->EncodeBuffer(ctx, ibuf, obuf, isz, 2*isz);
	
	printf("%s: %d -> %d\n", ctx->tstName, isz, osz);

#if 0
	printf("Stat Len:\n");
	for(i=0; i<2; i++)
	{
		k=i*8;
		printf("%5d: ", k);
		for(j=0; j<8; j++)
		{
			printf("%5d ", ctx->stat_len[i*8+j]);
		}
		printf("\n");
	}

	printf("Stat Dist:\n");
	for(i=0; i<3; i++)
	{
		k=i*8;
		printf("%5d: ", k);
		for(j=0; j<8; j++)
		{
			printf("%5d ", ctx->stat_dist[i*8+j]);
		}
		printf("\n");
	}

	printf("Stat Raw:\n");
	for(i=0; i<2; i++)
	{
		k=i*8;
		printf("%5d: ", k);
		for(j=0; j<8; j++)
		{
			printf("%5d ", ctx->stat_rlen[i*8+j]);
		}
		printf("\n");
	}
#endif

#if 1
	osz2=ctx->DecodeBuffer(obuf, obuf2, osz, isz*2);
	
	if(osz2!=isz)
	{
		printf("%s: Size mismatch\n", ctx->tstName);
	}else
	{
		if(memcmp(ibuf, obuf2, isz))
		{
			printf("%s: Data mismatch\n", ctx->tstName);
		}else
		{
			printf("%s: Data OK\n", ctx->tstName);
		}
	}

//	*(u64 *)(obuf2+isz+0)=0;
//	*(u64 *)(obuf2+isz+8)=0;
	memset(obuf2+isz, 0, 16);

	csum1=TgvLz_CalculateImagePel4BChecksum(ibuf, isz);
	csum2=TgvLz_CalculateImagePel4BChecksum(obuf2, isz);
	
	if(csum1!=csum2)
	{
		printf("%s: Checksum %08X->%08X\n", ctx->tstName, csum1, csum2);
	}
	
	ctx->csum=csum2;
	
	t0=clock();
//	te=t0+(10*CLOCKS_PER_SEC);
	te=t0+(tgvlz_benchsecs*CLOCKS_PER_SEC);
	t1=t0;
	ttsz=0;
	while(t1<te)
	{
		ttsz+=ctx->DecodeBuffer(obuf, obuf2, osz, isz*2);
		t1=clock();
	}
	
	f=(t1-t0)/((double)CLOCKS_PER_SEC);
	printf("%s: %lldB %fs %.2f MB/s\n",
		ctx->tstName, ttsz,
		f, (ttsz/f)*(1.0/1048576));
#endif

	free(obuf2);
	return(osz);
}

TgvLz_Context *TgvLz_CreateContext()
{
	TgvLz_Context *ctx;

	ctx=malloc(sizeof(TgvLz_Context));
	
	memset(ctx, 0, sizeof(TgvLz_Context));
	ctx->maxlen=16383;
	ctx->maxdist=(1<<22)-1;
	ctx->maxdepth=1024;

	ctx->EncodeBuffer=TgvLz_EncodeBufferRP2;
	ctx->DecodeBuffer=TgvLz_DecodeBufferRP2;
	ctx->tstName="RP2";
	ctx->cmp=3;

	return(ctx);
}

TgvLz_Context *TgvLz_CreateContextRP2A()
{
	TgvLz_Context *ctx;

	ctx=malloc(sizeof(TgvLz_Context));
	
	memset(ctx, 0, sizeof(TgvLz_Context));
	ctx->maxlen=515;
	ctx->maxdist=131071;
	ctx->maxdepth=1024;

	ctx->EncodeBuffer=TgvLz_EncodeBufferRP2;
	ctx->DecodeBuffer=TgvLz_DecodeBufferRP2;
	ctx->tstName="RP2A";
	ctx->cmp=3;

	return(ctx);
}

TgvLz_Context *TgvLz_CreateContextRP2B()
{
	TgvLz_Context *ctx;

	ctx=malloc(sizeof(TgvLz_Context));
	
	memset(ctx, 0, sizeof(TgvLz_Context));
	ctx->maxlen=16383;
	ctx->maxdist=(1<<22)-1;
	ctx->maxdepth=1024;

	ctx->EncodeBuffer=TgvLz_EncodeBufferRP2B;
	ctx->DecodeBuffer=TgvLz_DecodeBufferRP2B;
	ctx->tstName="RP2B";
	ctx->cmp=3;

	return(ctx);
}

TgvLz_Context *TgvLz_CreateContextRP2C()
{
	TgvLz_Context *ctx;

	ctx=malloc(sizeof(TgvLz_Context));
	
	memset(ctx, 0, sizeof(TgvLz_Context));
	ctx->maxlen=16383;
	ctx->maxdist=(1<<22)-1;
	ctx->maxdepth=1024;

	ctx->EncodeBuffer=TgvLz_EncodeBufferRP2C;
	ctx->DecodeBuffer=TgvLz_DecodeBufferRP2C;
	ctx->tstName="RP2C";
	ctx->cmp=3;

	return(ctx);
}


TgvLz_Context *TgvLz_CreateContextLZ4()
{
	TgvLz_Context *ctx;

	ctx=malloc(sizeof(TgvLz_Context));
	
	memset(ctx, 0, sizeof(TgvLz_Context));
	ctx->maxlen=16383;
	ctx->maxdist=65535;
	ctx->maxdepth=1024;

	ctx->EncodeBuffer=TgvLz_EncodeBufferLZ4;
	ctx->DecodeBuffer=TgvLz_DecodeBufferLZ4;
	ctx->tstName="LZ4";
	ctx->cmp=4;

	return(ctx);
}

int TgvLz_SetLevel(TgvLz_Context *ctx, int lvl)
{
	ctx->maxdepth=1<<(lvl+3);
	if(lvl<4)
		ctx->maxdepth=0;
	return(0);
}

int TgvLz_DestroyContext(TgvLz_Context *ctx)
{
	free(ctx);
	return(0);
}

#ifndef TGVLZ_NOMAIN

#ifdef TGVLZ_ENA_POST
#define TGVLZ_ENA_POST_BENCH
#include "btm_rp2stf.c"
#include "btm_rp2bitrc.c"
#include "rp2post_h12d.c"
#include "rp2post_h12e.c"
#include "rp2post_auto.c"
#endif

int main(int argc, char *argv[])
{
	TgvLz_Context *ctx;
	long long ttsz;
	byte *ibuf;
	byte *obuf;
	byte *obuf2;
	char *ifn, *ofn;
	double f, g;
	int t0, t1, te;
	int isz, csz, osz, osz2, dsz, dsum, csum, mode, ver;
	int wsz, pflag;
	int i, j, k;
	
	ifn=NULL;
	ofn=NULL;

	wsz=(1<<22)-1;
	tgvlz_benchsecs=10;
#ifdef TGVLZ_ENA_POST
	pflag=RP2POST_FLAGS_FAVOR_SPEED;
#endif
	
	ver=0;
	mode=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-z"))
				mode=1;
			if(!strcmp(argv[i], "-zb"))
				{ mode=1; ver=1; }
			if(!strcmp(argv[i], "-zc"))
				{ mode=1; ver=2; }

#ifdef TGVLZ_ENA_POST
			if(!strcmp(argv[i], "-zp"))
				{ mode=1; ver=3; }

			if(!strcmp(argv[i], "-zp_sz"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_FAVOR_SIZE; }
			if(!strcmp(argv[i], "-zp_sz2"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_FAVOR_SIZE2; }
			if(!strcmp(argv[i], "-zp_sp"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_FAVOR_SPEED; }
			if(!strcmp(argv[i], "-zp_sp2"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_FAVOR_SPEED2; }

			if(!strcmp(argv[i], "-zp_stf"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_USE_ADRICE; }
			if(!strcmp(argv[i], "-zp_huff"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_USE_HUFF; }
			if(!strcmp(argv[i], "-zp_range"))
				{ mode=1; ver=3; pflag=RP2POST_FLAGS_USE_RANGE; }
#endif

			if(!strcmp(argv[i], "-d"))
				mode=2;
			if(!strcmp(argv[i], "-b"))
				mode=3;
			if(!strcmp(argv[i], "-t"))
				mode=4;

//			if(!strcmp(argv[i], "-w16"))
//				{ wsz=(1<<16)-1; }
//			if(!strcmp(argv[i], "-w17"))
//				{ wsz=(1<<17)-1; }

			if(!strcmp(argv[i], "-bb"))
				{ mode=3; ver=1; }
			if(!strcmp(argv[i], "-bc"))
				{ mode=3; ver=2; }
			if(!strcmp(argv[i], "-bp"))
				{ mode=3; ver=3; }

#ifdef TGVLZ_ENA_POST
			if(!strcmp(argv[i], "-bp_sz"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_FAVOR_SIZE; }
			if(!strcmp(argv[i], "-bp_sz2"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_FAVOR_SIZE2; }
			if(!strcmp(argv[i], "-bp_sp"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_FAVOR_SPEED; }
			if(!strcmp(argv[i], "-bp_sp2"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_FAVOR_SPEED2; }

			if(!strcmp(argv[i], "-bp_stf"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_USE_ADRICE; }
			if(!strcmp(argv[i], "-bp_huff"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_USE_HUFF; }
			if(!strcmp(argv[i], "-bp_range"))
				{ mode=3; ver=3; pflag=RP2POST_FLAGS_USE_RANGE; }
#endif

			if(!strcmp(argv[i], "-b4"))
				{ mode=3; ver=4; }

			if(!strcmp(argv[i], "-b3c"))
				{ mode=3; ver=5; }
			
			if((argv[i][1]=='w') && (argv[i][2]>='0') && (argv[i][2]<='9'))
				{ wsz=(1<<atoi(argv[i]+2))-1; continue; }
			if((argv[i][1]=='i') && (argv[i][2]>='0') && (argv[i][2]<='9'))
				{ tgvlz_benchsecs=atoi(argv[i]+2); continue; }
			
			continue;
		}
		
		if(!ifn)
		{
			ifn=argv[i];
			continue;
		}

		if(!ofn)
		{
			ofn=argv[i];
			continue;
		}
	}
	
	if((mode==0) && !ifn)
	{
		printf("usage: %s [opts*] infile [outfile]\n", argv[0]);
		printf("\t-z\t\tEncode (RP2, Original)\n");
		printf("\t-zb\t\tEncode (RP2B)\n");
		printf("\t-zc\t\tEncode (RP2C)\n");
		printf("\t-d\t\tDecode\n");
		printf("\t-b\t\tBenchmark\n");
		printf("\t-bb\t\tBenchmark (RP2B)\n");
		printf("\t-bc\t\tBenchmark (RP2C)\n");
		printf("\t-w<bits>\tSet Window Size in Bits\n");
		printf("\t-i<secs>\tSet Benchmark Seconds\n");
#ifdef TGVLZ_ENA_POST
		printf("\t-zp\t\tEncode (RP2C+Post)\n");
		printf("\t-bp\t\tBenchmark (RP2C+Post)\n");

		printf("\t-zp_<md>\tEncode (RP2C+Post) with Sub-Mode\n");
		printf("\t-bp_<md>\tBenchmark (RP2C+Post) with Sub-Mode\n");
		printf("\t  sp / sp2\tDecode Speed, More Speed\n");
		printf("\t  sz / sz2\tEncoded Size, More Size\n");
		printf("\t  stf\t\tUse STF Post Compressor\n");
		printf("\t  huff\t\tUse Huffman Post Compressor\n");
		printf("\t  range\t\tUse Range Post Compressor\n");
#endif
	}
	
	ibuf=NULL;

	if(ifn)
	{
		ibuf=TgvLz_LoadFile(ifn, &isz);
	}else
	{
		printf("%s: no input file\n", argv[0]);
		return(-1);
	}

	if((mode==2) && ibuf[3]=='B')	ver=1;
	if((mode==2) && ibuf[3]=='C')	ver=2;
	if((mode==2) && ibuf[3]=='P')	ver=3;

	if((mode==2) && ibuf[2]=='L' && ibuf[3]=='4')
		ver=4;

//	ctx1=malloc(sizeof(TgvLz_Context));
//	ctx2=malloc(sizeof(TgvLz_Context));
	ctx=malloc(sizeof(TgvLz_Context));
	
//	obuf=malloc(isz*2);

//	obuf2=malloc(isz*2);

	memset(ctx, 0, sizeof(TgvLz_Context));
//	ctx->ct=obuf;	
	ctx->maxlen=16383;
//	ctx->maxdist=(1<<22)-1;
	ctx->maxdist=wsz;
//	ctx->maxdepth=4096;
	ctx->maxdepth=1024;
	ctx->cmp=3;

	ctx->EncodeBuffer=TgvLz_EncodeBufferRP2;
	ctx->DecodeBuffer=TgvLz_DecodeBufferRP2;
	ctx->tstName="RP2";
	ctx->cmpver=ver;
	
	if(ver==1)
	{
		ctx->EncodeBuffer=TgvLz_EncodeBufferRP2B;
		ctx->DecodeBuffer=TgvLz_DecodeBufferRP2B;
		ctx->tstName="RP2B";
	}

	if(ver==2)
	{
		ctx->EncodeBuffer=TgvLz_EncodeBufferRP2C;
		ctx->DecodeBuffer=TgvLz_DecodeBufferRP2C;
		ctx->tstName="RP2C";
	}

	if(ver==4)
	{
		ctx->EncodeBuffer=TgvLz_EncodeBufferLZ4;
		ctx->DecodeBuffer=TgvLz_DecodeBufferLZ4;
		ctx->tstName="LZ4";

		ctx->maxlen=4095;
		ctx->maxdist=65535;
	}

	if(ver==5)
	{
		ctx->EncodeBuffer=TgvLz_EncodeBufferLR3C;
		ctx->DecodeBuffer=TgvLz_DecodeBufferLR3C;
		ctx->tstName="LR3C";

		ctx->maxlen=515;
		ctx->maxdist=262144;
	}

	if((mode==3) || ((mode==0) && !ofn))
	{
		obuf=malloc(isz*2);

		if(ver==3)
		{
#ifdef TGVLZ_ENA_POST
			osz2=PostRp2_EncodeRP2BenchAuto(obuf+16, ibuf, isz*2, isz, 
				pflag|RP2POST_FLAGS_VERBOSE);
#else
			osz2=-1;
#endif
		}else
		{
			osz=TgvLz_DoTest(ctx, ibuf, obuf+16, isz);
		}
		
	//	csum=TgvLz_CalculateImagePel4BChecksum(ibuf, isz);
		
		memcpy(obuf, "RP2A", 4);
		set_u32le(obuf+ 4, osz);
		set_u32le(obuf+ 8, isz);
		set_u32le(obuf+12, ctx->csum);

		if(ver==1)
			obuf[3]='B';
		if(ver==2)
			obuf[3]='C';
		if(ver==3)
			obuf[3]='P';

		if(ver==4)
		{
			memcpy(obuf, "RL4A", 4);
		}

//		*(u32 *)(obuf+4)=osz;
//		*(u32 *)(obuf+8)=isz;
//		*(u32 *)(obuf+12)=ctx->csum;

		if(ofn)
		{
			TgvLz_StoreFile(ofn, obuf, osz+16);
		}
		return(0);
	}
	
	if((mode==1) || ((mode==0) && ofn))
	{
		obuf=malloc(isz*2);

		if(ver==3)
		{
#ifdef TGVLZ_ENA_POST
			osz2=PostRp2_EncodeRP2Auto(obuf+16, ibuf, isz*2, isz, 
				pflag);
#else
			osz2=-1;
#endif
		}else
		{
			osz=TgvLz_DoEncode(ctx, ibuf, obuf+16, isz);
		}
		
		memcpy(obuf, "RP2A", 4);
//		*(u32 *)(obuf+4)=osz;
//		*(u32 *)(obuf+8)=isz;
//		*(u32 *)(obuf+12)=ctx->csum;
		set_u32le(obuf+ 4, osz);
		set_u32le(obuf+ 8, isz);
		set_u32le(obuf+12, ctx->csum);

		if(ver==1)
			obuf[3]='B';
		if(ver==2)
			obuf[3]='C';
		if(ver==3)
			obuf[3]='P';

		if(ver==4)
		{
			memcpy(obuf, "RL4A", 4);
		}

		if(ofn)
		{
			TgvLz_StoreFile(ofn, obuf, osz+16);
		}
		return(0);
	}

	if(((mode==2) && ofn) || (mode==4))
	{
		if(	memcmp(ibuf, "RP2A", 4) &&
			memcmp(ibuf, "RP2B", 4) &&
			memcmp(ibuf, "RP2C", 4) &&
			memcmp(ibuf, "RP2P", 4) &&
			memcmp(ibuf, "RL4A", 4))
		{
			printf("Magic Fail\n");
			return(0);
		}
		
//		osz=*(u32 *)(ibuf+4);
//		dsz=*(u32 *)(ibuf+8);
//		dsum=*(u32 *)(ibuf+12);

		osz=get_u32le(ibuf+4);
		dsz=get_u32le(ibuf+8);
		dsum=get_u32le(ibuf+12);

		obuf=malloc(dsz*2);
		if(ver==3)
		{
#ifdef TGVLZ_ENA_POST
			osz2=PostRp2_DecodeBufferRp2Full(obuf, ibuf+16, dsz*2, osz);
#else
			osz2=-1;
#endif
		}else
		{
			osz2=ctx->DecodeBuffer(ibuf+16, obuf, osz, dsz*2);
		}
//		*(u64 *)(obuf+osz2+0)=0;
//		*(u64 *)(obuf+osz2+8)=0;
		memset(obuf+osz2, 0, 16);
		
		csum=TgvLz_CalculateImagePel4BChecksum(obuf, osz2);

		if(osz2!=dsz)
		{
			printf("Size mismatch\n");
		}else
		{
			if(csum!=dsum)
			{
				printf("Checksum mismatch %08X->%08X\n", dsum, csum);
			}else
			{
				if(mode==4)
				{
					printf("OK\n");
				}
			}
		}

		if(ofn)
		{
			TgvLz_StoreFile(ofn, obuf, osz2);
		}
		return(0);
	}

	return(0);
}
#endif

#endif
