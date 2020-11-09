/*

Experiment with RefPack:
* 0ddlllrr-dddddddd
** d+1, l+3
* 10llllll-rrdddddd-dddddddd
** d+1, l+4
* 110dllrr-dddddddd-dddddddd-llllllll
** d+1, l+0
* 111rrrrr, (r+1)*4
* 111111rr, r=r
** d: Distance
** l: Match Length
** r: Literal Length


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

Long Match will encode length and distance using variable-length encodings.

Length VLN:
          lllllll0,   4..  131
 llllllll-llllll01, 132..16383

Distance VLN:
          dddddddd-ddddddd0, 32K
 dddddddd-dddddddd-dddddd01,  4M

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef struct TgvLz_Context_s TgvLz_Context;

//#define TKELZ_HASH_SZ	1024
#define TKELZ_HASH_SZ	4096
#define TKELZ_HASH_DN	64
#define TKELZ_HASH_DN1	63

#define TKELZ_CHAIN_SZ	65536

struct TgvLz_Context_s {
byte *hash[TKELZ_HASH_SZ*TKELZ_HASH_DN];
byte hrov[TKELZ_HASH_SZ];
byte *cs;
byte *ct;
int maxlen;
int maxdist;

byte	*chn_base;
byte	*chn_ptrs[TKELZ_CHAIN_SZ];
u16		chn_next[TKELZ_CHAIN_SZ];
u16		chn_hash[TKELZ_HASH_SZ];

char *tstName;
int (*EncodeBuffer)(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int ibsz, int obsz);
int (*DecodeBuffer)(
	byte *ibuf, byte *obuf, int ibsz, int obsz);

int stat_len[24];
int stat_dist[24];
int stat_rlen[24];

};

byte *TkeLz0_LoadFile(char *name, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz;
	
	fd=fopen(name, "rb");
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

int TgvLz_CalcHash(byte *cs)
{
	int h;
	
	h=*(u32 *)cs;
	h=((h*4093)>>12)&(TKELZ_HASH_SZ-1);
	return(h);
}

int TgvLz_CheckMatch(byte *s0, byte *s1, byte *cse)
{
	byte *cs, *ct;
	
	cs=s0; ct=s1;
	while((cs+4)<=cse)
	{
		if(*(u32 *)cs!=*(u32 *)ct)
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
//		if((l>bl) && (d<65536))
//		if((l>bl) && (d<131072))
		if((l>bl) && (d<md))
			{ bl=l; bd=d; }
	}

//	ctx->chn_ptrs[rv]=cs;
//	ctx->chn_next[rv]=ctx->chn_hash[h];
//	ctx->chn_hash[h]=rv;

	i=ctx->chn_hash[h]; n=1024; ld=0;
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

	*rl=bl;
	*rd=bd;
	return(bl>=4);
}

// static int stat_len[256];
// static int stat_dist[256];
// static int stat_rlen[256];

int tgvlz_log2u(int val)
{
	int i, v;
	
	v=val; i=0;
	while(v)
		{ i++; v=v>>1; }
	return(i);
}

int TgvLz_LookupMatchB(TgvLz_Context *ctx,
	byte *cs, byte *cse,
	int *rl, int *rd)
{
	int l, d, l1, d1;
	int i, j, k;

	i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
	
	if(i)
	{
		j=TgvLz_LookupMatch(ctx, cs+1, cse, &l1, &d1);
//		if(j && (l1>(l+1)))
		if(j && (l1>(l+2)))
			i=0;

		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);
//			if(j && (l1>(l+2)))
//			if(j && (l1>(l+3)))
			if(j && (l1>(l+4)))
				i=0;
		}
		
		if(i)
		{
//			if((d>256) && (l<6))
//			if((d>128) && (l<4))
//				i=0;
			if((d>511) && (l<4))
//			if((d>511) && (l<5))
				i=0;
//			if((d>4096) && (l<10))
//			if((d>8191) && (l<8))
			if((d>8191) && (l<7))
//			if((d>8191) && (l<6))
				i=0;

//			if((d>32767) && (l<9))
			if((d>32767) && (l<8))
				i=0;

//			if((d>131071) && (l<9))
			if((d>131071) && (l<10))
//			if((d>131071) && (l<11))
				i=0;
		}
	}
	
	*rl=l;
	*rd=d;
	return(i);
}

int TgvLz_EncodeBuffer(TgvLz_Context *ctx,
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
//		i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
		i=TgvLz_LookupMatchB(ctx, cs, cse, &l, &d);

#if 0
		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+1, cse, &l1, &d1);
			if(j && (l1>(l+1)))
				i=0;

			if(i)
			{
				j=TgvLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);
				if(j && (l1>(l+2)))
					i=0;
			}
			
			if(i)
			{
				if((d>256) && (l<6))
					i=0;
				if((d>4096) && (l<10))
					i=0;
			}
		}
#endif

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

//		while(rl>=8)
		while(rl>=4)
		{
//			j=(rl>>2)-2;
			j=(rl>>2)-1;
			if(j>27)j=27;
			
//			k=(j+2)*4;
			k=(j+1)*4;
			*ct++=0xE0+j;
			memcpy(ct, lcs, k);
			ct+=k;	rl-=k;	lcs+=k;
		}

#if 0
		while(rl>=4)
		{
			*ct++=0xFF;
			*ct++=*lcs++;
			*ct++=*lcs++;
			*ct++=*lcs++;
			rl-=3;
		}
#endif

#if 1
		if((rl<4) && (l<=10) && (d<=1024))
		{
			d1=d-1;
			*ct++=0x00|((d1>>3)&0x60)|((l-3)<<2)|rl;
			*ct++=d1&0xFF;
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((rl<4) && (l<=67) && (d<=16384))
		{
			d1=d-1;
			*ct++=0x80|(l-4);
			*ct++=((d1>>8)&0x3F)|(rl<<6);
			*ct++=((d1   )&0xFF);
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
#if 1
		if((rl<4) && (l<=1023) && (d<=131072))
		{
			d1=d-1;
			*ct++=0xC0|((d1>>12)&0x10)|((l>>6)&0x0C)|rl;
			*ct++=((d1>>8)&0xFF);
			*ct++=((d1   )&0xFF);
			*ct++=((l    )&0xFF);
			memcpy(ct, lcs, rl);
			ct+=rl;
		}
		else
#endif
		{
			__debugbreak();
		}
		
		pl=l; pd=d;
		
		TgvLz_HashStr(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

	rl=cs-lcs;
	
//	while(rl>=8)
	while(rl>4)
	{
//		j=(rl>>2)-2;
		j=(rl>>2)-1;
		if(j>27)j=27;
		
//		k=(j+2)*4;
		k=(j+1)*4;
		*ct++=0xE0+j;
		memcpy(ct, lcs, k);
		ct+=k;	rl-=k;	lcs+=k;
	}

#if 0
	while(rl>=4)
	{
		*ct++=0xFF;
		*ct++=*lcs++;
		*ct++=*lcs++;
		*ct++=*lcs++;
		rl-=3;
	}
#endif
	
//	if(rl)
	if(1)
	{
		*ct++=0xFC+rl;
		ct[0]=lcs[0];
		ct[1]=lcs[1];
		ct[2]=lcs[2];
		ct+=rl;
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
//		i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
		i=TgvLz_LookupMatchB(ctx, cs, cse, &l, &d);

#if 0
		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+1, cse, &l1, &d1);
			if(j && (l1>(l+1)))
				i=0;

			if(i)
			{
				j=TgvLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);
				if(j && (l1>(l+2)))
					i=0;
			}
			
			if(i)
			{
				if((d>256) && (l<6))
					i=0;
				if((d>4096) && (l<10))
					i=0;
			}
		}
#endif

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
		if(rl>15)j|=0xF0;
		if(l1>15)j|=0x0F;
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

	j=rl<<4;
	if(rl>15)j|=0xF0;
	*ct++=j;	
	if(rl>15)
	{
		j=rl;
		while(j>=255)
			{ *ct++=0xFF; j-=255; }
		*ct++=j;
	}

	memcpy(ct, lcs, rl);
	ct+=rl;
	lcs+=rl;

	return(ct-obuf);
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
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
			v=*(u16 *)src;
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
			v=*(u32 *)src;
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
		{
			v=*(u64 *)src;
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
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
			*(u64 *)ct=*(u64 *)cs;
			ct+=8; cs+=8;
		}
	}
}

__forceinline void TgvLz_MatchCopy2(byte *dst, int sz, int d)
{
	byte *cs, *ct, *cte;
	u64 v;
	
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
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
//			v=*(u16 *)src;
			v=*(u16 *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
//			v=*(u32 *)src;
			v=*(u32 *)(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
		{
//			v=*(u64 *)src;
			v=*(u64 *)(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
		((u64 *)dst)[0]=((u64 *)cs)[0];
		((u64 *)dst)[1]=((u64 *)cs)[1];
	}else
	{
//		cs=src;
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
#if 0
			*(u64 *)ct=*(u64 *)cs;
			ct+=8; cs+=8;
#endif
#if 1
			((u64 *)ct)[0]=((u64 *)cs)[0];
			((u64 *)ct)[1]=((u64 *)cs)[1];
			ct+=16; cs+=16;
#endif
		}
	}
}

__forceinline void TgvLz_RawCopy(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	if(sz>8)
	{
		cs=src;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
#if 0
			*(u64 *)ct=*(u64 *)cs;
			ct+=8; cs+=8;
#endif
#if 1
			((u64 *)ct)[0]=((u64 *)cs)[0];
			((u64 *)ct)[1]=((u64 *)cs)[1];
			ct+=16; cs+=16;
#endif
		}
	}else
		if(sz)
	{
		*(u64 *)dst=*(u64 *)src;
	}
}

__forceinline void TgvLz_RawCopyB(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	cs=src;
	ct=dst; cte=dst+sz;
	while(ct<cte)
	{
#if 0
		*(u64 *)ct=*(u64 *)cs;
		ct+=8; cs+=8;
#endif
#if 1
		((u64 *)ct)[0]=((u64 *)cs)[0];
		((u64 *)ct)[1]=((u64 *)cs)[1];
		ct+=16; cs+=16;
#endif
	}
}

int TgvLz_DecodeBuffer(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse;
	int pl, pd;
	int rl, l, d;
	int t0, t1, t2, t3;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
//	while(cs<cse)
	while(1)
	{
		t0=*(u32 *)cs;
		if(!(t0&0x80))
		{
			cs+=2;
			rl=t0&3;
			l=((t0>>2)&7)+3;
			d=(((t0&0x60)<<3)|((t0>>8)&0x00FF))+1;
		}else
			if(!(t0&0x40))
		{
			cs+=3;
			l=(t0&63)+4;
			rl=(t0>>14)&3;
			d=((t0&0x3F00)|((t0>>16)&0x00FF))+1;
		}else
			if(!(t0&0x20))
		{
			cs+=4;
			rl=t0&3;
			d=(((t0<<12)&0x10000)|(t0&0xFF00)|((t0>>16)&0x00FF))+1;
			l=((t0<<6)&0x0300)|((t0>>24)&0x00FF);
		}
		else
		{
			cs++;
			t1=t0&31;
//			rl=(t1+1)*4;
			if(t1>=28)
			{
				l=t0&3;
//				rl=t1-28;
				*(u32 *)ct=*(u32 *)cs;
				cs+=l;
				ct+=l;
				break;
			}
			else
			{
				rl=(t1+1)*4;
				TgvLz_RawCopyB(ct, cs, rl);
				cs+=rl;
				ct+=rl;
				continue;
			}
		}

		*(u32 *)ct=*(u32 *)cs;
		cs+=rl;
		ct+=rl;
		TgvLz_MatchCopy2(ct, l, d);
		ct+=l;
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
	
//	__hint_use_egpr();
	
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
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	
	pd=0; pl=0;
	
	lcs=cs;
	while(cs<cse)
	{
//		i=TgvLz_LookupMatch(ctx, cs, cse, &l, &d);
		i=TgvLz_LookupMatchB(ctx, cs, cse, &l, &d);

#if 0
		if(i)
		{
			j=TgvLz_LookupMatch(ctx, cs+1, cse, &l1, &d1);
			if(j && (l1>(l+1)))
				i=0;

			if(i)
			{
				j=TgvLz_LookupMatch(ctx, cs+2, cse, &l1, &d1);
				if(j && (l1>(l+2)))
					i=0;
			}
			
			if(i)
			{
				if((d>256) && (l<6))
					i=0;
				if((d>4096) && (l<10))
					i=0;
			}
		}
#endif

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
//			if(j>23)j=23;
			if(j>15)j=15;
			
			k=(j+1)*8;
//			*ct++=0x07+(j<<3);
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
			__debugbreak();
		}
		
		pl=l; pd=d;
		
		TgvLz_HashStr(ctx, cs, l);
		cs+=l;
		lcs=cs;
	}

	rl=cs-lcs;
	
	while(rl>8)
	{
//		j=(rl>>2)-2;
		j=(rl>>3)-1;
//		if(j>23)j=23;
		if(j>15)j=15;
		
//		k=(j+2)*4;
		k=(j+1)*8;
//		*ct++=0x07+(j<<3);
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

#if 0
	if(1)
	{
		k=rl;
		*ct++=0xC7+(rl<<3);
		*ct++=0xC7+(rl<<3);
		memcpy(ct, lcs, k);
		ct+=k;	rl-=k;	lcs+=k;
	}
#endif
	
	return(ct-obuf);
}

int TgvLz_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse;
	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
//	int t0, t1, t2, t3;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
//	while(cs<cse)
	while(1)
	{
//		t0=*(u32 *)cs;
		t0=*(u64 *)cs;
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
			rl=(t0>>6)&3;
			if(!rl)break;
			*(u32 *)ct=*(u32 *)cs;
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
		{
			__debugbreak();
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
		TgvLz_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}

int TgvLz_DoTest(TgvLz_Context *ctx,
	byte *ibuf, byte *obuf, int isz)
{
	long long ttsz;
	byte *obuf2;
	double f, g;
	int t0, t1, te;
	int osz, osz2;
	int i, j, k;

	obuf2=malloc(isz*2);

	osz=ctx->EncodeBuffer(ctx, ibuf, obuf, isz, 2*isz);
	
	printf("%s: %d -> %d\n", ctx->tstName, isz, osz);

#if 1
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

int main(int argc, char *argv[])
{
	TgvLz_Context *ctx1;
	TgvLz_Context *ctx2;
	TgvLz_Context *ctx3;
	long long ttsz;
	byte *ibuf;
	byte *obuf;
	byte *obuf2;
	double f, g;
	int t0, t1, te;
	int isz, osz, osz2;
	int i, j, k;
	
	ibuf=TkeLz0_LoadFile(argv[1], &isz);
	
	ctx1=malloc(sizeof(TgvLz_Context));
	ctx2=malloc(sizeof(TgvLz_Context));
	ctx3=malloc(sizeof(TgvLz_Context));
	
	obuf=malloc(isz*2);

//	obuf2=malloc(isz*2);

	memset(ctx1, 0, sizeof(TgvLz_Context));
	ctx1->ct=obuf;
	ctx1->maxlen=16384;
	ctx1->maxdist=65535;
	ctx1->EncodeBuffer=TgvLz_EncodeBufferLZ4;
	ctx1->DecodeBuffer=TgvLz_DecodeBufferLZ4;
	ctx1->tstName="LZ4";

	memset(ctx2, 0, sizeof(TgvLz_Context));
	ctx2->ct=obuf;	
	ctx2->maxlen=1023;
	ctx2->maxdist=131072;
	ctx2->EncodeBuffer=TgvLz_EncodeBuffer;
	ctx2->DecodeBuffer=TgvLz_DecodeBuffer;
	ctx2->tstName="RefPack";

	memset(ctx3, 0, sizeof(TgvLz_Context));
	ctx3->ct=obuf;	
//	ctx3->maxlen=515;
//	ctx3->maxdist=131071;

	ctx3->maxlen=16383;
	ctx3->maxdist=(1<<22)-1;

	ctx3->EncodeBuffer=TgvLz_EncodeBufferRP2;
	ctx3->DecodeBuffer=TgvLz_DecodeBufferRP2;
	ctx3->tstName="RP2";

	TgvLz_DoTest(ctx1, ibuf, obuf, isz);
	TgvLz_DoTest(ctx2, ibuf, obuf, isz);
	TgvLz_DoTest(ctx3, ibuf, obuf, isz);

//	TgvLz_DoTest(ctx2, ibuf, obuf, isz);
//	TgvLz_DoTest(ctx1, ibuf, obuf, isz);

}
