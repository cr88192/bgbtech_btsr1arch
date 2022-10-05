/*
BtRP2 is a yet another byte-based LZ77 variant.

Its design takes some inspiration from the EA RefPack format, but changes were made in an effort to improve compression ratio and decoder performance. Another goal is to try to avoid becomming excessively complicated.


BtRP2 (Transposed, LE):
*                   dddddddd-dlllrrr0	(l=3..10, d=0..511, r=0..7)
*          dddddddd-dddddlll-lllrrr01	(l=4..67, d=0..8191)
* dddddddd-dddddddd-dlllllll-llrrr011	(l=4..515, d=0..131071)
*                            rrrr0111	(Raw Bytes, r=(r+1)*8, 8..128)
*                          * rrr01111	(Long Match)
*                            rr011111	(r=1..3 bytes, 0=EOB)
*                   rrrrrrrr-r0111111 	(Long Raw, r=(r+1)*8, 8..4096)
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

#define HAVE_STDINT_H	/* Probably safe to assume at this point... */

#ifdef _MSC_VER
#if (_MSC_VER<1800)
#undef HAVE_STDINT_H	/* Older MSVC */
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>

typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

#else

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#endif


typedef struct TgvLz_Context_s TgvLz_Context;

//#define TKELZ_HASH_SZ	1024
#define TKELZ_HASH_SZ	4096
#define TKELZ_HASH_DN	64
#define TKELZ_HASH_DN1	63

#define TKELZ_CHHASH_SZ	16384
#define TKELZ_CHAIN_SZ	65536


/* Portability Cruft... */

#ifdef _MSC_VER
#define force_inline	__forceinline
#define	debug_break		__debugbreak();
#ifdef _M_IX86
#define unaligned
#define vol_unaligned	volatile
#else
#define unaligned		__unaligned
#define vol_unaligned	volatile __unaligned
#endif
#define HAVE_MISAL
#else

#if defined(__BGBCC__) || defined(__BJX2__)
#define force_inline	
#define	debug_break		__debugbreak();
#define unaligned		__unaligned
#define vol_unaligned	__unaligned
#define HAVE_MISAL
#else
#define force_inline
#define	debug_break	
#define unaligned
#define vol_unaligned
#endif

#endif

#if defined(__i386__) || defined(__x86_64__) || defined(__arm__)
#define HAVE_MISAL_MEMCPY
#endif

#ifdef HAVE_MISAL

#define	memcpy_4B(dst, src)		\
	(*(vol_unaligned u32 *)(dst)=*(vol_unaligned u32 *)(src))
#define	memcpy_8B(dst, src)		\
	(*(vol_unaligned u64 *)(dst)=*(vol_unaligned u64 *)(src))
#define	memcpy_16B(dst, src)	\
	(((vol_unaligned u64 *)(dst))[0]=((vol_unaligned u64 *)(src))[0]);	\
	(((vol_unaligned u64 *)(dst))[1]=((vol_unaligned u64 *)(src))[1])

#define	get_u16le(ptr)			(*(vol_unaligned u16 *)(ptr))
#define	get_u32le(ptr)			(*(vol_unaligned u32 *)(ptr))
#define	get_u64le(ptr)			(*(vol_unaligned u64 *)(ptr))
#define	set_u16le(ptr, val)		(*(vol_unaligned u16 *)(ptr))=(val)
#define	set_u32le(ptr, val)		(*(vol_unaligned u32 *)(ptr))=(val)
#define	set_u64le(ptr, val)		(*(vol_unaligned u64 *)(ptr))=(val)

#else

#define	memcpy_4B(dst, src)		memcpy(dst, src, 4)
#define	memcpy_8B(dst, src)		memcpy(dst, src, 8)
// #define	memcpy_16B(dst, src)	memcpy(dst, src, 16)
#define	memcpy_16B(dst, src)	\
	memcpy(((byte *)(dst))+0, ((byte *)(src))+0, 8);		\
	memcpy(((byte *)(dst))+8, ((byte *)(src))+8, 8);

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

struct TgvLz_Context_s {
byte *hash[TKELZ_HASH_SZ*TKELZ_HASH_DN];
byte hrov[TKELZ_HASH_SZ];
byte *cs;
byte *ct;
int maxlen;
int maxdist;
int maxdepth;
u32	csum;
byte cmp;

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
int pred_l1, pred_d1;
int pred_l2, pred_d2;
};

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

int TgvLz_CalcHash(byte *cs)
{
	int h;
	
//	h=*(u32 *)cs;
	h=get_u32le(cs);
	h=((h*4093)>>12)&(TKELZ_HASH_SZ-1);
	return(h);
}

int TgvLz_CalcHashB(byte *cs)
{
	int h;
	
//	h=*(u32 *)cs;
	h=get_u32le(cs);
	h^=h>>16;
	h=((h*4093)>>12)&(TKELZ_CHHASH_SZ-1);
	return(h);
}

int TgvLz_CheckMatch(byte *s0, byte *s1, byte *cse)
{
	byte *cs, *ct;
	
	if(s0==s1)
		return(0);
	
	cs=s0; ct=s1;
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

	h=TgvLz_CalcHashB(cs);

	ctx->chn_ptrs[rv]=cs;
	ctx->chn_next[rv]=ctx->chn_hash[h];
	ctx->chn_hash[h]=rv;

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
	byte *cs1, *cse;
	int h, hr, bl, bd;
	int i, j, k, l, d, n, ml, md, ld;

	if(str==ctx->pred_cs2)
	{
		bl=ctx->pred_l2;
		bd=ctx->pred_d2;

		*rl=bl;
		*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512)));
	}

	if(str==ctx->pred_cs1)
	{
		bl=ctx->pred_l1;
		bd=ctx->pred_d1;

		*rl=bl;
		*rd=bd;
		return((bl>=4) || ((bl>=3) && (bd<512)));
	}

	ml=ctx->maxlen;
	md=ctx->maxdist;
//	cse=str+259;
//	cse=str+1023;
	cse=str+ml;
	if(cse>stre)
		cse=stre;
	
	h=TgvLz_CalcHash(str);
	hr=ctx->hrov[h];
	
	bl=0; bd=0;
	
	for(i=0; i<TKELZ_HASH_DN; i++)
	{
		cs1=ctx->hash[(h*TKELZ_HASH_DN)+((hr+i)&TKELZ_HASH_DN1)];
		if(!cs1)continue;
		l=TgvLz_CheckMatch(str, cs1, cse);
		d=str-cs1;
		if(d<=0)
			continue;
//		if((l>bl) && (d<65536))
//		if((l>bl) && (d<131072))
		if((l>bl) && (d<md))
			{ bl=l; bd=d; }
	}

//	ctx->chn_ptrs[rv]=cs;
//	ctx->chn_next[rv]=ctx->chn_hash[h];
//	ctx->chn_hash[h]=rv;

	h=TgvLz_CalcHashB(str);

	n=ctx->maxdepth;
	i=ctx->chn_hash[h];
//	n=1024;
	ld=0;
	while(n--)
	{
		cs1=ctx->chn_ptrs[i];
		if(!cs1)continue;

		d=str-cs1;
		if(d<=ld)
			break;
		ld=d;
		if(d>md)
			break;

		l=TgvLz_CheckMatch(str, cs1, cse);		
		if((l>bl) && (d<md))
			{ bl=l; bd=d; }
		i=ctx->chn_next[i];
	}

	ctx->pred_cs2=ctx->pred_cs1;
	ctx->pred_l2=ctx->pred_l1;
	ctx->pred_d2=ctx->pred_d1;
	ctx->pred_cs1=str;
	ctx->pred_l1=bl;
	ctx->pred_d1=bd;

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
		c=rl+1;
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
	else if((bl<=67) && (bd<8192))
		{ c+=3; }
	else if((bl<=515) && (bd<131072))
		{ c+=4; }
	else if((bl<=16383) && (bd<(1<<22)))
	{
		c+=1+((bl<128)?1:2)+((bd<32768)?2:3);
	}
	return(c);
}

int TgvLz_LookupMatchB(TgvLz_Context *ctx,
	byte *cs, byte *cse, byte *lcs,
	int *rl, int *rd)
{
	int l, d, l1, d1;
	int rl0, rl1, rl2;
	int mc0, mc1;
	int i, j, k;

	l=0; d=0;
	l1=0; d1=0;

	i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
	rl0=cs-lcs;
	rl1=rl0+1;	rl2=rl0+2;
	
	mc0=TgvLz_EstMatchCost(ctx, rl0, l, d);
	mc1=TgvLz_EstRawCost(ctx, rl0+l);
	
	if(mc0>=mc1)
	{
		/* Skip match if it costs more than it saves. */
		i=0;
	}
	
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

		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);

			mc1=TgvLz_EstMatchCost(ctx, rl2, l1, d1);
//			if(j && (l1>(l+0)) && (mc1<=mc0))
			if(j && ((mc1-l1)<(mc0-l)))
//			if(j && ((mc1-l1)<=(mc0-l)))
				i=0;
		}
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
force_inline void TgvLz_MatchCopy2(byte *dst, int sz, int d)
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
//			v=*(u16 *)(dst-d);
			v=get_u16le(dst-d);
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
//			v=*(u32 *)(dst-d);
			v=get_u32le(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
//				*(u64 *)ct=v;
				set_u64le(ct, v);
				ct+=8;
			}
		}else if(d)
		{
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
		memcpy_16B(dst, cs);
	}else
	{
//		cs=src;
		cs=dst-d;
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
	}
}
#endif

force_inline void TgvLz_RawCopy(byte *dst, byte *src, int sz)
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

force_inline void TgvLz_RawCopyB(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	cs=src;
	ct=dst; cte=dst+sz;
	while(ct<cte)
	{
#if 0
//		*(u64 *)ct=*(u64 *)cs;
		memcpy_8B(ct, cs);
		ct+=8; cs+=8;
#endif
#if 1
//		((u64 *)ct)[0]=((u64 *)cs)[0];
//		((u64 *)ct)[1]=((u64 *)cs)[1];
		memcpy_16B(ct, cs);
		ct+=16; cs+=16;
#endif
	}
}

int TgvLz_EncodeBufferRP2(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *cse, *lcs;
	byte *ct;
	u32 v;
	int pl, pd;
	int l, d, rl, l1, d1;
	int i, j, k;
	
	ctx->chn_base=ibuf;
	ctx->pred_cs1=NULL;
	ctx->pred_cs2=NULL;
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	
	pd=0; pl=0;
	d=0; l=0;
	
	lcs=cs;
	while(cs<cse)
	{
//		i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
		i=TgvLz_LookupMatchB(ctx, cs, cse, lcs, &l, &d);

		if((d<=0) || ((cs-d)<ibuf))
			i=0;

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
		if((rl<8) && (l<=0x3FFF) && (d<=0x3FFFFF))
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

int TgvLz_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse, *cte;
//	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf; cte=ct+obsz;
	rl=0; l=0; d=0;
	
	while(1)
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
			TgvLz_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x10))
		{
			/* Long Match */
			cs++;
			rl=(t0>>5)&7;
			t1=t0>>8;
			if(!(t1&1))
				{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
			else
				{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
			if(!(t2&1))
				{ d=((t2>>1)&0x007FFF); cs+=2; }
			else
				{ d=((t2>>2)&0x3FFFFF); cs+=3; }
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
//			*(u32 *)ct=*(u32 *)cs;
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

			if((ct+rl)>cte)
				break;

			TgvLz_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
		{
			debug_break
		}

		if((ct+rl)>cte)
			break;

//		*(u64 *)ct=*(u64 *)cs;
		memcpy_8B(ct, cs);
		cs+=rl;
		ct+=rl;

		if((ct-d)<obuf)
			break;
		if((ct+l)>cte)
			break;

		TgvLz_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
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
		return(-1);
	}
	
	ctx->csum=csum2;
#endif

	free(obuf2);
	return(osz);
}

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
	te=t0+(10*CLOCKS_PER_SEC);
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
	int isz, csz, osz, osz2, dsz, dsum, csum, mode;
	int i, j, k;
	
	ifn=NULL;
	ofn=NULL;
	
	mode=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-z"))
				mode=1;
			if(!strcmp(argv[i], "-d"))
				mode=2;
			if(!strcmp(argv[i], "-b"))
				mode=3;
			if(!strcmp(argv[i], "-t"))
				mode=4;
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
		printf("\t-z\t\tEncode\n");
		printf("\t-d\t\tDecode\n");
		printf("\t-b\t\tBenchmark\n");
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
	
//	ctx1=malloc(sizeof(TgvLz_Context));
//	ctx2=malloc(sizeof(TgvLz_Context));
	ctx=malloc(sizeof(TgvLz_Context));
	
//	obuf=malloc(isz*2);

//	obuf2=malloc(isz*2);

	memset(ctx, 0, sizeof(TgvLz_Context));
//	ctx->ct=obuf;	
	ctx->maxlen=16383;
	ctx->maxdist=(1<<22)-1;

	ctx->EncodeBuffer=TgvLz_EncodeBufferRP2;
	ctx->DecodeBuffer=TgvLz_DecodeBufferRP2;
	ctx->tstName="RP2";

	if((mode==3) || ((mode==0) && !ofn))
	{
		obuf=malloc(isz*2);
		osz=TgvLz_DoTest(ctx, ibuf, obuf+16, isz);
		
	//	csum=TgvLz_CalculateImagePel4BChecksum(ibuf, isz);
		
		memcpy(obuf, "RP2A", 4);
		set_u32le(obuf+ 4, osz);
		set_u32le(obuf+ 8, isz);
		set_u32le(obuf+12, ctx->csum);

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
		osz=TgvLz_DoEncode(ctx, ibuf, obuf+16, isz);
		
		memcpy(obuf, "RP2A", 4);
//		*(u32 *)(obuf+4)=osz;
//		*(u32 *)(obuf+8)=isz;
//		*(u32 *)(obuf+12)=ctx->csum;
		set_u32le(obuf+ 4, osz);
		set_u32le(obuf+ 8, isz);
		set_u32le(obuf+12, ctx->csum);

		if(ofn)
		{
			TgvLz_StoreFile(ofn, obuf, osz+16);
		}
		return(0);
	}

	if(((mode==2) && ofn) || (mode==4))
	{
		if(memcmp(ibuf, "RP2A", 4))
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
		osz2=ctx->DecodeBuffer(ibuf+16, obuf, osz, dsz*2);
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
