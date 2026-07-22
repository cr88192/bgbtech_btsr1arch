/*
Basic RP2/RP2B encoder/decoder.

RP2 (Transposed, LE):
*                        dddddddd-dlllrrr0	(l=3..10, d=0..511, r=0..7)
*               dddddddd-dddddlll-lllrrr01	(l=4..67, d=0..8191)
*      dddddddd-dddddddd-dlllllll-llrrr011	(l=4..515, d=0..131071)
*                                 rrrr0111	(Raw Bytes, r=(r+1)*8, 8..128)
*                               * rrr01111	(Long Match)
*                                 rr011111	(r=1..3 bytes, 0=EOB)
*                        rrrrrrrr-r0111111 	(Long Raw, r=(r+1)*8, 8..4096)
*                        lllllll0-01111111  (RP2B, l=4..131, d=1, r=0)
* 2x D-ddddddll-llllllll-llllrrr1-01111111  (RP2B, l=4..16K, d=0..4M, r=0..7)
*
** d: Distance
** l: Match Length
** r: Literal Length

The original long match is excluded.


Portability:
Assumes a target that allows fast unaligned memory dereference and little-endian word ordering. While not strictly portable, casting to a volatile pointer is usually enough to stop compilers from getting stupid, without the speed penalty of more "approved" methods ("like memcpy()") on some other compilers, otherwise one is either fighting with performance or needing to have a bunch of alternate-case boilerplate to deal with different compilers (some which deal well with "memcpy()", others where "memcpy()" performs like hot garbage).

Granted, there are some chips around which support misaligned access, sorta, but where doing so carries a significant speed penalty (due to internal traps and emulation). Will ignore these for now.

*/


#if 1
#include <stdint.h>

#ifndef GFXEDIT_BASETYPES
#define GFXEDIT_BASETYPES
typedef uint8_t	byte;
typedef int8_t		sbyte;
typedef uint16_t	u16;
typedef int16_t	s16;
typedef uint32_t	u32;
typedef int32_t	s32;
typedef uint64_t	u64;
typedef int64_t	s64;
typedef volatile uint16_t	vol_u16;
typedef volatile uint32_t	vol_u32;
typedef volatile uint64_t	vol_u64;
typedef volatile uint16_t	*vol_u16p;
typedef volatile uint32_t	*vol_u32p;
typedef volatile uint64_t	*vol_u64p;
#endif

#ifndef GFXEDIT_MISAL
#define GFXEDIT_MISAL
/* portability boilerplate goes here... */
#define gfxedit_getu16(ptr)			(*(vol_u16p)(ptr))
#define gfxedit_getu32(ptr)			(*(vol_u32p)(ptr))
#define gfxedit_getu64(ptr)			(*(vol_u64p)(ptr))
#define gfxedit_setu16(ptr,val)		(*(vol_u16p)(ptr)=(val))
#define gfxedit_setu32(ptr,val)		(*(vol_u32p)(ptr)=(val))
#define gfxedit_setu64(ptr,val)		(*(vol_u64p)(ptr)=(val))
#endif

#endif

byte *gfxedit_memlzcpyf(byte *dst, byte *src, int len)
{
	byte *cs, *ct, *cte;
	u64 v0, v1;
	int d;
	int i;
	
	d=dst-src;

#if 0
	//simple/safe, but slow
	if(d>len)
		{ memcpy(dst, src, len); return(dst+len); }
	if(d<0)
	{
		memmove(dst, src, len);
		return(dst+len);
	}
	if(d==1)
		{ memset(dst, *src, len); return(dst+len); }

	for(i=0; i<len; i++)
		dst[i]=src[i];
	return(dst+len);
#endif

//	if((d>len) || (d<(-len)))
	if(d>len)
	{
		cs=src;
		ct=dst;
		cte=dst+len;

		v0=gfxedit_getu64(cs);
		v1=gfxedit_getu64(cs+8);
		gfxedit_setu64(ct, v0);
		gfxedit_setu64(ct+8, v1);
		cs+=16; ct+=16;

		if(ct<cte)
		{
			v0=gfxedit_getu64(cs);
			v1=gfxedit_getu64(cs+8);
			gfxedit_setu64(ct, v0);
			gfxedit_setu64(ct+8, v1);
			cs+=16; ct+=16;

			if(ct<cte)
			{
				v0=gfxedit_getu64(cs);
				v1=gfxedit_getu64(cs+8);
				gfxedit_setu64(ct, v0);
				gfxedit_setu64(ct+8, v1);
				cs+=16; ct+=16;

				while(ct<cte)
				{
					v0=gfxedit_getu64(cs);
					v1=gfxedit_getu64(cs+8);
					gfxedit_setu64(ct, v0);
					gfxedit_setu64(ct+8, v1);
					cs+=16; ct+=16;
				}
			}
		}
		return(cte);
	}

	if(d<=8)
	{
		if(d==1)
		{
			v0=*(byte *)src;
			v0|=(v0<<8); v0|=(v0<<16); v0|=(v0<<32);
			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); gfxedit_setu64(ct+8, v0); ct+=16; }
			return(cte);
		}

		if(d==2)
		{
			v0=gfxedit_getu16(src);
			v0|=(v0<<16); v0|=(v0<<32);
			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); gfxedit_setu64(ct+8, v0); ct+=16; }
			return(cte);
		}

		if(d==4)
		{
			v0=gfxedit_getu32(src);
			v0|=(v0<<32);
			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); gfxedit_setu64(ct+8, v0); ct+=16; }
			return(cte);
		}

		if(d==8)
		{
			v0=gfxedit_getu64(src);
			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); gfxedit_setu64(ct+8, v0); ct+=16; }
			return(cte);
		}

		if(d<0)
		{
			if(d<(-len))
			{
				cs=src;
				ct=dst;
				cte=dst+len;
				while(ct<cte)
				{
					v0=gfxedit_getu64(cs);
					v1=gfxedit_getu64(cs+8);
					gfxedit_setu64(ct, v0);
					gfxedit_setu64(ct+8, v1);
					cs+=16; ct+=16;
				}
				return(cte);
			}
		
			memmove(dst, src, len);
			return(dst+len);
		}

#if 0
		if(d==3)
		{
			v0=gfxedit_getu32(src);
			v0&=0x00FFFFFF;
			v0|=(v0<<24);
			v1=(v0>>16)|(v0<<32);
			v0|=v0<<48;

			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); gfxedit_setu64(ct+8, v1); ct+=15; }
			return(cte);
		}
#endif

#if 1
		if(d==3)
		{
			v0=gfxedit_getu32(src);
			v0=v0<<40;
			v0=(v0>>16)|(v0>>40);
			ct=dst; cte=dst+len;
			while(ct<cte)
			{
				gfxedit_setu64(ct+ 0, v0);
				gfxedit_setu64(ct+ 6, v0);
				gfxedit_setu64(ct+12, v0);
				ct+=18;
			}
			return(cte);
		}
#endif

#if 1
		if(d==5)
		{
			v0=gfxedit_getu64(src);
			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); ct+=5; }
			return(cte);
		}

		if(d==7)
		{
			v0=gfxedit_getu64(src);
			ct=dst; cte=dst+len;
			while(ct<cte)
				{ gfxedit_setu64(ct, v0); ct+=7; }
			return(cte);
		}
#endif

		for(i=0; i<len; i++)
			dst[i]=src[i];
		return(dst+len);
	}

	if(d>=16)
	{
		cs=src;
		ct=dst;
		cte=dst+len;
		while(ct<cte)
		{
			v0=gfxedit_getu64(cs);
			v1=gfxedit_getu64(cs+8);
			gfxedit_setu64(ct, v0);
			gfxedit_setu64(ct+8, v1);
			cs+=16; ct+=16;
		}
		return(cte);
	}
	else
	{
		cs=src;
		ct=dst;
		cte=dst+len;
		while(ct<cte)
		{
			v0=gfxedit_getu64(cs);
			gfxedit_setu64(ct, v0);
			cs+=8; ct+=8;
		}
		return(cte);
	}
}

int GfxEdit_DecodeRP2(byte *dst, byte *src, int dsz, int ssz)
{
	byte *cs, *ct, *cse, *cte;
	u64 v0, v1;
	u32 tag;
	int ml, md, mr, no;
	int i;
	
	cs=src; ct=dst; cse=src+ssz; cte=dst+dsz;
	no=0;
	while((cs<cse) && (ct<cte))
	{
		tag=gfxedit_getu32(cs);
		if(!(tag&0x01))
		{
			mr=(tag>>1)&7;
			ml=((tag>>4)&7)+3;
			md=(tag>>7)&511;
			cs+=2;
		}
		else if(!(tag&0x02))
		{
			mr=(tag>>2)&7;
			ml=((tag>>5)&63)+4;
			md=(tag>>11)&8191;
			cs+=3;
		}
		else if(!(tag&0x04))
		{
			mr=(tag>>3)&7;
			ml=((tag>>6)&511)+4;
			md=(tag>>15)&131071;
			cs+=4;
		}else if(!(tag&0x08))
		{
			mr=(((tag>>4)&15)+1)*8;
			ml=0;
			md=0;
			cs+=1;
#if 1
			while(mr>=16)
			{
				v0=gfxedit_getu64(cs);
				v1=gfxedit_getu64(cs+8);
				gfxedit_setu64(ct, v0);
				gfxedit_setu64(ct+8, v1);
				cs+=16; ct+=16; mr-=16;
			}
			while(mr>=8)
			{
				v0=gfxedit_getu64(cs);
				gfxedit_setu64(ct, v0);
				cs+=8; ct+=8; mr-=8;
			}
			continue;
#endif
		}else if(!(tag&0x10))
		{
			__debugbreak();
			break;
		}else if(!(tag&0x20))
		{
			mr=(tag>>6)&3;
			ml=0;
			md=0;
			cs+=1;
			if(!mr)
				break;
#if 1
			/* copy an exact number of bytes. */
			while(mr--)
				{ *ct++=*cs++; }
			continue;
#endif
		}else if(!(tag&0x40))
		{
			mr=(((tag>>7)&511)+1)*8;
			ml=0;
			md=0;
			cs+=2;
		}else if(!(tag&0x80))
		{
			if(!(tag&0x100))
			{
				mr=0;
				ml=((tag>>9)&127)+4;
				md=1;
				cs+=2;
			}else
			{
				mr=(tag>>9)&7;
				ml=((tag>>12)&16383)+4;
				md=(gfxedit_getu32(cs+3)>>2)&0x3FFFFF;
				cs+=6;
			}
		}else
		{
			__debugbreak();
			break;
		}

		if(mr)
		{
			if(mr>8)
			{
				if(mr>16)
				{
					gfxedit_memlzcpyf(ct, cs, mr);
				}else
				{
					gfxedit_setu64(ct, gfxedit_getu64(cs));
					gfxedit_setu64(ct+8, gfxedit_getu64(cs+8));
				}
			}else
				{ gfxedit_setu64(ct, gfxedit_getu64(cs)); }
			ct+=mr;
			cs+=mr;
		}
		if(ml)
			{ ct=gfxedit_memlzcpyf(ct, ct-md, ml); }
	}
	return(ct-dst);
}


#define GFXEDIT_MINRP2_HASHSZ		1024
#define GFXEDIT_MINRP2_HASHDEPTH	16

// #define GFXEDIT_MINRP2_MAX_ML		515
// #define GFXEDIT_MINRP2_MAX_MD		131071

#define GFXEDIT_MINRP2_MAX_ML		16387
#define GFXEDIT_MINRP2_MAX_MD		0x3FFFFF


#define GFXEDIT_MINRP2_HASH2SZ		4096
#define GFXEDIT_MINRP2_HASH2DEPTH	4

// #define GFXEDIT_MINRP2_HASH2SZ		128
// #define GFXEDIT_MINRP2_HASH2DEPTH	128

//#define GFXEDIT_MINRP2_HASH2SZ		256
//#define GFXEDIT_MINRP2_HASH2DEPTH	64

// #define GFXEDIT_MINRP2_HASH2SZ		64
// #define GFXEDIT_MINRP2_HASH2DEPTH	256

int GfxEdit_EncodeRP2_HashMatch(byte *cs)
{
	u32 h;
	h=gfxedit_getu32(cs);
	h^=(h<<7)^(h>>13);
	h^=(h>>17);
	return(h&(GFXEDIT_MINRP2_HASHSZ-1));
}

#ifdef GFXEDIT_MINRP2_HASH2SZ

#if (GFXEDIT_MINRP2_HASH2SZ>1024)
int GfxEdit_EncodeRP2_Hash2Match(byte *cs)
{
	u64 h;
	/* for wide/shallow */
	h=gfxedit_getu64(cs);
	h^=(h>>29);
	h^=(h<<7)^(h>>13);
	h^=(h>>17);
	return(h&(GFXEDIT_MINRP2_HASH2SZ-1));
}
#else
int GfxEdit_EncodeRP2_Hash2Match(byte *cs)
{
	u32 h;
	/* for narrow/deep */
	h=gfxedit_getu64(cs);
//	h&=0xFFFFFF;
	h<<=8;
	h^=(h<<7)^(h>>13);
	h^=(h>>17);
	return(h&(GFXEDIT_MINRP2_HASH2SZ-1));
}
#endif

#endif

void GfxEdit_EncodeRP2_HashUpdateByte(
	byte *cs, byte *src, int *hash, byte *hrov)
{
	int *hashz, *hash2;
	byte *hrov2;
	int c0, c1, c2, c3;
	int h, hr, i;

	h=GfxEdit_EncodeRP2_HashMatch(cs);
	hr=hrov[h];
	hash[(h*GFXEDIT_MINRP2_HASHDEPTH)+hr]=cs-src;
	hrov[h]=(hr+1)&(GFXEDIT_MINRP2_HASHDEPTH-1);

#ifdef GFXEDIT_MINRP2_HASH2SZ
	hash2=hash+(GFXEDIT_MINRP2_HASHSZ*GFXEDIT_MINRP2_HASHDEPTH);
	hrov2=hrov+GFXEDIT_MINRP2_HASHSZ;

	h=GfxEdit_EncodeRP2_Hash2Match(cs);
	hr=hrov2[h];
	hash2[(h*GFXEDIT_MINRP2_HASH2DEPTH)+hr]=cs-src;
	hrov2[h]=(hr+1)&(GFXEDIT_MINRP2_HASH2DEPTH-1);
#endif
}

int GfxEdit_EncodeRP2_CheckMatchLength(byte *s0, byte *s1, byte *cse)
{
	char *cs0, *cs1;
	u64 v0, v1;
	int ml, i;

#if 0
	for(i=0; i<GFXEDIT_MINRP2_MAX_ML; i++)
		if(s0[i]!=s1[i])
			break;
	return(i);
#endif

	v0=gfxedit_getu64(s0);
	v1=gfxedit_getu64(s1);
	if(v0!=v1)
	{
		if(((u32)v0)!=((u32)v1))
		{
			if((v0&0xFFFFFF)==(v1&0xFFFFFF))
				return(3);
			return(0);
		}
		v0>>=32; v1>>=32;
		if(((u16)v0)!=((u16)v1))
		{
			if(((byte)v0)!=((byte)v1))
				return(4);
			return(5);
		}
		v0>>=16; v1>>=16;
		if(((byte)v0)!=((byte)v1))
			return(6);
		return(7);
	}

	cs0=s0+8;
	cs1=s1+8;
	ml=8;

	v0=gfxedit_getu64(cs0);
	v1=gfxedit_getu64(cs1);
	while((v0==v1) && (cs0<cse))
	{
		cs0+=8; cs1+=8; ml+=8;
		v0=gfxedit_getu64(cs0); v1=gfxedit_getu64(cs1);
	}

	if(v0!=v1)
	{
		for(i=0; i<64; i+=8)
			if(((v0>>i)&255)!=((v1>>i)&255))
				break;
		ml+=i>>3;
	}
	return(ml);
}

int GfxEdit_EncodeRP2_LookupMatch(byte *cs,
	byte *src, int *hash, byte *hrov, byte *cse,
	int *rml, int *rmd)
{
	int *hash2;
	byte *cs1, *hrov2, *cse1;
	int bl, bd, bi, d, hr;
	int h, i, j, k;

	cse1=cs+GFXEDIT_MINRP2_MAX_ML;
	if(cse<cse1)
		cse1=cse;

	h=GfxEdit_EncodeRP2_HashMatch(cs);
	hr=hrov[h]-1;
	bl=0; bd=0; bi=0;
	for(i=0; i<GFXEDIT_MINRP2_HASHDEPTH; i++)
	{
		cs1=src+hash[h*GFXEDIT_MINRP2_HASHDEPTH+
			((hr-i)&(GFXEDIT_MINRP2_HASHDEPTH-1))];
		j=GfxEdit_EncodeRP2_CheckMatchLength(cs, cs1, cse1);
		d=cs-cs1;
		if((j>bl) && (d>0) && (d<GFXEDIT_MINRP2_MAX_MD))
			{ bl=j; bd=d; bi=i; }
	}

#ifdef GFXEDIT_MINRP2_HASH2SZ
	if(bl<8)
	{
		hash2=hash+(GFXEDIT_MINRP2_HASHSZ*GFXEDIT_MINRP2_HASHDEPTH);
		hrov2=hrov+GFXEDIT_MINRP2_HASHSZ;

		h=GfxEdit_EncodeRP2_Hash2Match(cs);
		hr=hrov2[h]-1;
		for(i=0; i<GFXEDIT_MINRP2_HASH2DEPTH; i++)
		{
			cs1=src+hash2[h*GFXEDIT_MINRP2_HASH2DEPTH+
				((hr-i)&(GFXEDIT_MINRP2_HASH2DEPTH-1))];
			j=GfxEdit_EncodeRP2_CheckMatchLength(cs, cs1, cse1);
			d=cs-cs1;
			if((j>bl) && (d>0) && (d<GFXEDIT_MINRP2_MAX_MD))
			{
				bl=j; bd=d; bi=i;
				if(bl>=8)
					break;
			}
		}
	}
#endif

	if((cs+bl)>cse)
		bl=cse-cs;
	if(bl>GFXEDIT_MINRP2_MAX_ML)
		bl=GFXEDIT_MINRP2_MAX_ML;

	if(bl<3)					bl=0;
	if((bl<4) && (bd>   511))	bl=0;
	if((bl<6) && (bd>  8191))	bl=0;
	if((bl<8) && (bd>131071))	bl=0;
	
	if((bl>0) && (bd>=8192))
	{
		k=-1;
	}
	
	*rml=bl;	*rmd=bd;
	return(bl!=0);
}

int GfxEdit_EncodeRP2_GetRawM7Cost(int mr)
{
	int c, k;
	
	c=mr; k=mr;
	while(k>=4096)
		{ c+=2; k-=4096; }
	if(k>128)
	{
		c+=2;
		k&=7;
	}else
	{
		c+=1;
		k&=7;
	}
	return(c);
}

int GfxEdit_EncodeRP2_GetRawM0Cost(int mr)
{
	int c, k;
	
	c=mr; k=mr;
	while(k>=4096)
		{ c+=2; k-=4096; }
	if(k>128)
	{
		c+=2;
		k&=7;
	}else
	{
		c+=1;
		k&=7;
	}
	while(k>=3)
		{ c=+1; k-=3; }
	if(k)
		{ c+=1; }
	return(c);
}

int GfxEdit_EncodeRP2_GetMatchCost(int mr, int ml, int md)
{
	if((ml>=3) && (ml<=10) && (md<=511))
		return(2+GfxEdit_EncodeRP2_GetRawM7Cost(mr));
	if((ml>=4) && (ml<=131) && (md==1) && (mr==0))
		return(2+GfxEdit_EncodeRP2_GetRawM0Cost(mr));
	if((ml>=4) && (ml<=67) && (md<=8191))
		return(3+GfxEdit_EncodeRP2_GetRawM7Cost(mr));
	if((ml>=4) && (ml<=515) && (md<=131071))
		return(4+GfxEdit_EncodeRP2_GetRawM7Cost(mr));
	return(6+GfxEdit_EncodeRP2_GetRawM7Cost(mr));
}

int GfxEdit_EncodeRP2(byte *dst, byte *src, int dsz, int ssz)
{
	static int *hash;
	static byte *hrov;

	byte *cs, *cs0, *ct, *cse, *cte;
	u32 tag;
	int ml, md, mr, h, mrt, ml1, md1, mrt1, mc1, mc2;
	int i, j, k, no;

	if(!hash)
	{
		hash=malloc((
			GFXEDIT_MINRP2_HASHSZ *GFXEDIT_MINRP2_HASHDEPTH +
#ifdef GFXEDIT_MINRP2_HASH2SZ
			GFXEDIT_MINRP2_HASH2SZ*GFXEDIT_MINRP2_HASH2DEPTH+
#endif
			0)*sizeof(int));
		hrov=malloc(
			GFXEDIT_MINRP2_HASHSZ+
#ifdef GFXEDIT_MINRP2_HASH2SZ
			GFXEDIT_MINRP2_HASH2SZ+
#endif
			0);
	}

	memset(hash, 0,
		(GFXEDIT_MINRP2_HASHSZ*GFXEDIT_MINRP2_HASHDEPTH+
#ifdef GFXEDIT_MINRP2_HASH2SZ
		GFXEDIT_MINRP2_HASH2SZ*GFXEDIT_MINRP2_HASH2DEPTH+
#endif
		0)*sizeof(int));
	memset(hrov, 0,
		(GFXEDIT_MINRP2_HASHSZ+
#ifdef GFXEDIT_MINRP2_HASH2SZ
		GFXEDIT_MINRP2_HASH2SZ+
#endif
		0)*sizeof(byte));
	
	cs=src; ct=dst; cse=src+ssz; cte=dst+dsz;
	cs0=cs; no=0;
	while(cs<cse)
	{
		mrt1=0;
		mrt=GfxEdit_EncodeRP2_LookupMatch(cs, src,
			hash, hrov, cse, &ml, &md);
		if(mrt)
		{
			mrt1=GfxEdit_EncodeRP2_LookupMatch(cs+1, src,
				hash, hrov, cse, &ml1, &md1);
			if(mrt1)
			{
				mr=cs-cs0;
				mc1=GfxEdit_EncodeRP2_GetMatchCost(mr, ml, md);
				mc2=GfxEdit_EncodeRP2_GetMatchCost(mr+1, ml1, md1);
				if(mrt1 && (ml1>(ml+1+(mc2-mc1))))
					mrt=0;
			}
			
			if((cs+ml+32)>=cse)
			{
				/* Don't allow LZ matches within final 32 bytes.
				 * Shorten match, or discard if needed.
				 */
				if(ml>48)
					{ ml-=32; }
				else
					{ mrt=0; }
			}
		}

		if(mrt)
		{
			mr=cs-cs0;
			while(mr>128)
			{
				k=(mr/8)-1;
				if(k>511)
					k=511;
				tag=0x3F|(k<<7);
				*ct++=(tag>>0)&255;
				*ct++=(tag>>8)&255;
				j=(k+1)*8;
				memcpy(ct, cs0, j);
				ct+=j; cs0+=j;
				mr=cs-cs0;
			}

			while(mr>=8)
			{
				k=(mr/8)-1;
				if(k>15)
					k=15;
				tag=0x07|(k<<4);
				*ct++=tag;
				j=(k+1)*8;
				memcpy(ct, cs0, j);
				ct+=j; cs0+=j;
				mr=cs-cs0;
			}
			
			if((ml<=10) && (md<=511))
			{
				tag=0|(mr<<1)|((ml-3)<<4)|(md<<7);
				*ct++=(tag>>0)&255;
				*ct++=(tag>>8)&255;
				if(mr)
				{
					memcpy(ct, cs0, mr);
					ct+=mr; cs0+=mr;
				}
			}else
				if((mr==0) && (ml>=4) && (ml<=131) && (md==1))
			{
				tag=0x7F|((ml-4)<<9);
				*ct++=(tag>>0)&255;
				*ct++=(tag>>8)&255;
			}else
				if((ml>=4) && (ml<=67) && (md<=8191))
			{
				tag=1|(mr<<2)|((ml-4)<<5)|(md<<11);
				*ct++=(tag>> 0)&255;
				*ct++=(tag>> 8)&255;
				*ct++=(tag>>16)&255;
				if(mr)
				{
					memcpy(ct, cs0, mr);
					ct+=mr; cs0+=mr;
				}
			}else
				if((ml>=4) && (ml<=515) && (md<=131071))
			{
				tag=3|(mr<<3)|((ml-4)<<6)|(md<<15);
				*ct++=(tag>> 0)&255;
				*ct++=(tag>> 8)&255;
				*ct++=(tag>>16)&255;
				*ct++=(tag>>24)&255;
				if(mr)
				{
					memcpy(ct, cs0, mr);
					ct+=mr; cs0+=mr;
				}
			}else
				if((ml<=16387) && (md<=0x3FFFFF))
			{
				tag=0x017F|(mr<<9)|((ml-4)<<12)|(md<<26);
				*ct++=(tag>> 0)&255;
				*ct++=(tag>> 8)&255;
				*ct++=(tag>>16)&255;
				*ct++=(tag>>24)&255;
				*ct++=(md >> 6)&255;
				*ct++=(md >>14)&255;
				if(mr)
				{
					memcpy(ct, cs0, mr);
					ct+=mr; cs0+=mr;
				}
			}else
			{
				__debugbreak();
			}
			
			for(i=0; i<ml; i++)
			{
				GfxEdit_EncodeRP2_HashUpdateByte(cs, src, hash, hrov);
				cs++;
			}
			cs0=cs;
		}else
		{
			GfxEdit_EncodeRP2_HashUpdateByte(cs, src, hash, hrov);
			cs++;
		}
	}

	mr=cs-cs0;
//	while(mr>128)
	while(mr>192)
	{
		k=(mr/8)-1;
		if(k>511)
			k=511;

		/* Don't allow this run type near end of buffer. */
		while((cs+(k+1)*8)>(cse-48))
			k--;
		tag=0x3F|(k<<7);
		*ct++=(tag>>0)&255;
		*ct++=(tag>>8)&255;
		j=(k+1)*8;
		memcpy(ct, cs0, j);
		ct+=j; cs0+=j;
		mr=cs-cs0;
	}

	while(mr>=8)
	{
		k=(mr/8)-1;
		if(k>15)
			k=15;
		*ct++=0x07|(k<<4);
		j=(k+1)*8;
		memcpy(ct, cs0, j);
		ct+=j; cs0+=j;
		mr=cs-cs0;
	}
	
	while(mr>0)
	{
		k=mr;
		if(k>3)
			k=3;
		*ct++=0x1F|(k<<6);
		memcpy(ct, cs0, k);
		ct+=k; cs0+=k;
		mr=cs-cs0;
	}
	
	*ct++=0x1F;
	return(ct-dst);
}

int GfxEdit_EncodeRP2Test(byte *dst, byte *src, int dsz, int ssz)
{
	byte *tptr;
	int csz, tsz;
	int i;
	
	tptr=malloc(ssz+1024);
	
	csz=GfxEdit_EncodeRP2(dst, src, dsz, ssz);
	tsz=GfxEdit_DecodeRP2(tptr, dst, ssz+256, csz);
	
	if(tsz!=ssz)
	{
		__debugbreak();
	}
	
	for(i=0; i<ssz; i++)
		if(tptr[i]!=src[i])
			break;
	
	if(i<ssz)
	{
		__debugbreak();
	}
	
	free(tptr);
	return(csz);
}
