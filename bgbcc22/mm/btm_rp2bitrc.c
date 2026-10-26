/*
 * Bitwise Range Coder post-compressor for RP2.
 *
 * Escape Magic: 30 00
 *
 * Can be bolted on top of RP2 as an optional post-compressor, with some
 * gains seen for compression ratio. Doesn't support the
 * "Long Match" case.
 *
 * RP2 (Transposed, LE):
 *                        dddddddd-dlllrrr0  (l=3..10, d=0..511, r=0..7)
 *               dddddddd-dddddlll-lllrrr01  (l=4..67, d=0..8191)
 *      dddddddd-dddddddd-dlllllll-llrrr011  (l=4..515, d=0..131071)
 *                                 rrrr0111  (Raw Bytes, r=(r+1)*8, 8..128)
 *                               * rrr01111  (Long Match)
 *                        dddllll0-rrr01111  (RP2C, l=11..26, d=1..8, r=0..7)
 *               dddddddl-lllllll1-rrr01111  (RP2C, l=68..323, d=1..128, r=0..7)
 *                                 rr011111  (r=1..3 bytes, 0=EOB)
 *                        rrrrrrrr-r0111111  (Long Raw, r=(r+1)*8, 8..4096)
 *                        lllllll0-01111111  (RP2B, l=4..131, d=1, r=0)
 * 2x D-ddddddll-llllllll-llllrrr1-01111111  (RP2B, l=4..16K, d=0..4M, r=0..7)
 *   DD-dddddddd-ddddddll-llllrrr0-11111111  (RP2B?, l=4..67, d=0..4M, r=0..7)
 *
 ** d: Distance
 ** l: Match Length
 ** r: Literal Length
 *
 * Will use 3 contexts:
 * One for Tag Bytes (the first byte in the match pattern).
 * One for Length/Distance Bytes (any secondary bytes in the match).
 * One for raw/literal bytes.
 *
 * This will use a shared bit history between symbols,
 * with each symbol being encoded with 11 bits of context
 *   As history and bit position within the byte.
 * Symbols will be encoded in LSB first order in this case.
 *   LSB first is a slightly better fit for the data being encoded.
 *
 * This will use 30-00 as an escape pattern.
 *   This will otherwise be invalid at the start of an RP2 encoded blob.
 * Apart from this (and a few other escape patterns), could assume normal RP2.
 *   Here, entropy coding is regarded as an optional special case.
 *   Entropy coding is then only used if it brings sufficient benefit.
 *   Otherwise, the high CPU cost of Range Coding is undesirable.
 *
 * Here, it is assumed that this will be used on relatively short data blobs.
 *   Something more normal, like Deflate, making more sense for larger blobs.
 *   But, Deflate being mostly ineffective with small data blobs.
 *     Also, initializing Huffman tables is very expensive for small blobs.
 * Huffman quickly becomes faster than Range-Coding as symbol counts increase.
 *   STF+AdRice was another option for fast and OK at small blob sizes.
 *   But, for small blobs, STF+AdRice was seeing little compression vs plain RP2.
 *     So, Range Coding was the only option that "actually did something" here.
 *
 */
 
#ifndef RP2POST_RP2BITRC_C
#define RP2POST_RP2BITRC_C

#ifdef GFXEDIT_MINRP2_C
#ifndef HAS_RP2STF_DECODERP2
#define HAS_RP2STF_DECODERP2
#define PostRp2Stf_DecodeRP2(obuf, ibuf, osz, isz)		\
	GfxEdit_DecodeRP2(obuf, ibuf, osz, isz)
#endif
#endif

#ifdef TGVLZ1_C
#ifndef HAS_RP2STF_DECODERP2
#define HAS_RP2STF_DECODERP2
#define PostRp2Stf_DecodeRP2(obuf, ibuf, osz, isz)		\
	TgvLz_DecodeBufferRP2C(ibuf, obuf, isz, osz)
#endif
#endif

typedef struct BTM_BitRcCtx_s BTM_BitRcCtx;

struct BTM_BitRcCtx_s {
u32 w_lo;	//lower bound of range
u32 w_rn;	//range
u32 w_in;	//reference input
byte *cst;	//entropy-coded stream
};

static const u16 btm_bitrc_padjusttab[256]={
0xA05F, 0x0301, 0x0501, 0x0702, 0x0803, 0x0A04, 0x0B05, 0x0C06,
0x0E07, 0x0F08, 0x1109, 0x120A, 0x130B, 0x140C, 0x160D, 0x170E,
0x180F, 0x1910, 0x1A11, 0x1C12, 0x1D13, 0x1E14, 0x1F15, 0x2016,
0x2117, 0x2318, 0x2419, 0x251A, 0x261A, 0x271B, 0x281C, 0x291D,
0x2A1E, 0x2B1F, 0x2C20, 0x2E21, 0x2F22, 0x3023, 0x3124, 0x3225,
0x3326, 0x3426, 0x3527, 0x3628, 0x3729, 0x382A, 0x392B, 0x3A2C,
0x3B2D, 0x3C2E, 0x3D2F, 0x3E30, 0x3F31, 0x4031, 0x4132, 0x4233,
0x4334, 0x4435, 0x4536, 0x4637, 0x4738, 0x4839, 0x493A, 0x4A3B,
0x4B3C, 0x4C3D, 0x4D3D, 0x4E3E, 0x4F3F, 0x5040, 0x5141, 0x5242,
0x5343, 0x5444, 0x5545, 0x5646, 0x5747, 0x5848, 0x5949, 0x5A49,
0x5B4A, 0x5C4B, 0x5D4C, 0x5E4D, 0x5F4E, 0x604F, 0x6150, 0x6251,
0x6352, 0x6453, 0x6554, 0x6655, 0x6755, 0x6856, 0x6957, 0x6A58,
0x6B59, 0x6C5A, 0x6D5B, 0x6E5C, 0x6F5D, 0x705E, 0x705F, 0x7160,
0x7261, 0x7362, 0x7462, 0x7563, 0x7664, 0x7765, 0x7866, 0x7967,
0x7A68, 0x7B69, 0x7C6A, 0x7D6B, 0x7E6C, 0x7F6D, 0x806E, 0x816F,
0x816F, 0x8270, 0x8371, 0x8472, 0x8573, 0x8674, 0x8775, 0x8876,
0x8977, 0x8A78, 0x8B79, 0x8C7A, 0x8D7B, 0x8E7C, 0x8F7D, 0x907E,
0x907E, 0x917F, 0x9280, 0x9381, 0x9482, 0x9583, 0x9684, 0x9785,
0x9886, 0x9987, 0x9A88, 0x9B89, 0x9C8A, 0x9D8B, 0x9D8C, 0x9E8D,
0x9F8E, 0xA08F, 0xA18F, 0xA290, 0xA391, 0xA492, 0xA593, 0xA694,
0xA795, 0xA896, 0xA997, 0xAA98, 0xAA99, 0xAB9A, 0xAC9B, 0xAD9C,
0xAE9D, 0xAF9E, 0xB09F, 0xB1A0, 0xB2A1, 0xB3A2, 0xB4A3, 0xB5A4,
0xB6A5, 0xB6A6, 0xB7A7, 0xB8A8, 0xB9A9, 0xBAAA, 0xBBAB, 0xBCAC,
0xBDAD, 0xBEAE, 0xBFAF, 0xC0B0, 0xC1B1, 0xC2B2, 0xC2B3, 0xC3B4,
0xC4B5, 0xC5B6, 0xC6B7, 0xC7B8, 0xC8B9, 0xC9BA, 0xCABB, 0xCBBC,
0xCCBD, 0xCDBE, 0xCEBF, 0xCEC0, 0xCFC1, 0xD0C2, 0xD1C3, 0xD2C4,
0xD3C5, 0xD4C6, 0xD5C7, 0xD6C8, 0xD7C9, 0xD8CA, 0xD9CB, 0xD9CC,
0xDACD, 0xDBCE, 0xDCCF, 0xDDD0, 0xDED1, 0xDFD3, 0xE0D4, 0xE1D5,
0xE2D6, 0xE3D7, 0xE4D8, 0xE5D9, 0xE5DA, 0xE6DB, 0xE7DC, 0xE8DE,
0xE9DF, 0xEAE0, 0xEBE1, 0xECE2, 0xEDE3, 0xEEE5, 0xEFE6, 0xF0E7,
0xF1E8, 0xF2E9, 0xF3EB, 0xF4EC, 0xF5ED, 0xF6EE, 0xF7F0, 0xF8F1,
0xF9F3, 0xFAF4, 0xFBF5, 0xFCF7, 0xFDF8, 0xFEFA, 0xFEFC, 0xA05F,
};

int BTM_BItRc_EncodeFlush(BTM_BitRcCtx *ctx)
{
	int prb, mid, rng, low;
	int i;
	
	rng=ctx->w_rn;
	low=ctx->w_lo;
	
	while(low)
	{
		*ctx->cst++=low>>24;
		low=low<<8;
		rng=(rng<<8);
	}
	
	ctx->w_lo=low;
	ctx->w_rn=rng;
	return(0);
}


int BTM_BItRc_EncodeBit(BTM_BitRcCtx *ctx, byte *rprb, int bit)
{
	u16 prbi;
	u32 prb, mid, rng, low, low0;
	int i, j, k;
	
	prb=*rprb;

	rng=ctx->w_rn;
	low=ctx->w_lo;
	prbi=btm_bitrc_padjusttab[prb];
	low0=low;
	
	mid=(rng>>8)*prb;
	if(bit)
	{
		low=low+mid;
		rng=rng-mid;
		prb=prbi&255;
	}else
	{
		rng=mid;
		prb=prbi>>8;
	}
	
	/* if value has overflowed, we need to adjust prior bytes... */
	if(low<low0)
	{
		i=-1;
		while(1)
		{
			j=ctx->cst[i];
			k=j+1;
			ctx->cst[i]=k;
			if((k&255)>j)
				break;
			i--;
		}
	}
	
	if(rng<0x01000000)
	{
		*ctx->cst++=low>>24;
		low=low<<8;
		rng=(rng<<8);
	}
	
	ctx->w_lo=low;
	ctx->w_rn=rng;
	*rprb=prb;
	return(0);
}

int BTM_BItRc_DecodeBit(BTM_BitRcCtx *ctx, byte *rprb)
{
	u16 prbi;
	u32 prb, mid, rng, bit, win;
	
	prb=*rprb;

	rng=ctx->w_rn;
	win=ctx->w_in;
	prbi=btm_bitrc_padjusttab[prb];
	mid=(rng>>8)*prb;

	bit=win>=mid;

	if(bit)
	{
		win-=mid;
		rng-=mid;
		prb=prbi&255;
	}else
	{
		rng=mid;
		prb=prbi>>8;
	}
	
	if(rng<0x01000000)
	{
		win=(win<<8)|(*ctx->cst++);
		rng=(rng<<8);
	}
	
	ctx->w_rn=rng;
	ctx->w_in=win;
	*rprb=prb;
	
	return(bit);
}

int BTM_BItRc_DecodeInit(BTM_BitRcCtx *ctx, byte *ibuf)
{
	ctx->w_lo=0x00000000;
	ctx->w_rn=0xFFFFFFFF;
	ctx->cst=ibuf;

	ctx->w_in=(ctx->w_in<<8)|(*ctx->cst++);
	ctx->w_in=(ctx->w_in<<8)|(*ctx->cst++);
	ctx->w_in=(ctx->w_in<<8)|(*ctx->cst++);
	ctx->w_in=(ctx->w_in<<8)|(*ctx->cst++);

	return(0);
}

int BTM_BItRc_EncodeInit(BTM_BitRcCtx *ctx, byte *ibuf)
{
	ctx->w_lo=0x00000000;
	ctx->w_rn=0xFFFFFFFF;
	ctx->cst=ibuf;
	return(0);
}

int BTM_BItRc_DecodeByteLeCtx8B(BTM_BitRcCtx *ctx,
	byte *probtab, int *rhist)
{
	int b, v, h, ix;
	int i, j, k;
	
	v=0;
	h=*rhist;
	for(i=0; i<8; i++)
	{
		j=((h&255)<<3)|(i&7);
		b=BTM_BItRc_DecodeBit(ctx, probtab+j);
		h=(h<<1)|b;
		v=(v>>1)|(b<<7);
	}
	*rhist=h;
	return(v);
}

int BTM_BItRc_EncodeByteLeCtx8B(BTM_BitRcCtx *ctx,
	byte *probtab, int *rhist, int val)
{
	int b, v, h, ix;
	int i, j, k;
	
	h=*rhist;
	for(i=0; i<8; i++)
	{
		b=(val>>i)&1;
		j=((h&255)<<3)|(i&7);
		BTM_BItRc_EncodeBit(ctx, probtab+j, b);
		h=(h<<1)|b;
	}
	*rhist=h;
	return(0);
}

int BTM_BItRc_CheckPostRp2Blob(byte *ibuf)
{
	if((ibuf[0]==0x30) && (ibuf[1]==0x00))
		return(1);
	return(0);
}

int BTM_BItRc_EncodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[2048];
	byte prbtab_d[2048];
	byte prbtab_l[2048];
	BTM_BitRcCtx t_ctx;
	BTM_BitRcCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	u32 tag;
	int lc, tsz, nr;
	int i, j, k;

	ctx=&t_ctx;

	ct=obuf;
	*ct++=0x30;
	*ct++=0x00;

	BTM_BItRc_EncodeInit(ctx, ct);

	memset(prbtab_t, 0x80, 2048);
	memset(prbtab_d, 0x80, 2048);
	memset(prbtab_l, 0x80, 2048);
	lc=0;
	
	cs=ibuf;
	cse=ibuf+ibsz;
	while(cs<cse)
	{
		tag=*(u32 *)cs;
		if(!(tag&0x01))			{ tsz=2; nr=(tag>>1)&7; }
		else if(!(tag&0x02))	{ tsz=3; nr=(tag>>2)&7; }
		else if(!(tag&0x04))	{ tsz=4; nr=(tag>>3)&7; }
		else if(!(tag&0x08))	{ tsz=1; nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x10))
		{
			nr=(tag>>5)&7;
			if(tag&0x100)		{ tsz=3; }
			else				{ tsz=2; }
		}
		else if(!(tag&0x20))	{ tsz=1; nr=(tag>>6)&3; }
		else if(!(tag&0x40))	{ tsz=2; nr=(((tag>>7)&511)+1)*8; }
		else if(!(tag&0x80))
		{
			if(tag&0x100)
				{ tsz=6; nr=(tag>>9)&7; }
			else
				{ tsz=2; nr=0; }
		}
		else
		{
			if(!(tag&0x100))
				{ tsz=5; nr=(tag>>9)&7; }
			else
				{ tsz=-1; }
		}
		
		if(tsz<1)
			return(-1);
		
		if((cs+(tsz+nr))>cse)
			return(-1);
		
		BTM_BItRc_EncodeByteLeCtx8B(ctx, prbtab_t, &lc, *cs++);
		for(i=1; i<tsz; i++)
			BTM_BItRc_EncodeByteLeCtx8B(ctx, prbtab_d, &lc, *cs++);
		for(i=0; i<nr; i++)
			BTM_BItRc_EncodeByteLeCtx8B(ctx, prbtab_l, &lc, *cs++);
	}
	BTM_BItRc_EncodeFlush(ctx);
	return(ctx->cst-obuf);
}

int BTM_BItRc_DecodeBufferPostRp2(byte *obuf, byte *ibuf, int ibsz)
{
	byte prbtab_t[2048];
	byte prbtab_d[2048];
	byte prbtab_l[2048];
	BTM_BitRcCtx t_ctx;
	BTM_BitRcCtx *ctx;
	byte *cs, *cse, *cs1, *csr, *ct;
	u64 tag;
	int lc, tsz, tsi, nr, tg;
	int i, j, k;

	if((ibuf[0]!=0x30) || (ibuf[1]!=0x00))
		return(-1);

	ctx=&t_ctx;
	BTM_BItRc_DecodeInit(ctx, ibuf+2);
	
	ct=obuf;

	memset(prbtab_t, 0x80, 2048);
	memset(prbtab_d, 0x80, 2048);
	memset(prbtab_l, 0x80, 2048);
	lc=0;
	
	while(1)
	{
		tg=BTM_BItRc_DecodeByteLeCtx8B(ctx, prbtab_t, &lc);

		*ct++=tg;
		if(tg==0x1F)
			break;

		tag=tg;
		tsi=1;

		if(!(tg&0x01))
			{ tsz=2; }
		else if(!(tg&0x02))
			{ tsz=3; }
		else if(!(tg&0x04))
			{ tsz=4; }
		else if(!(tg&0x08))
			{ tsz=1; }
		else if(!(tg&0x10))
		{
			j=BTM_BItRc_DecodeByteLeCtx8B(ctx, prbtab_d, &lc);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			tsz=2;
			if(tag&0x0100)
				tsz=3;
		}
		else if(!(tg&0x20))
			{ tsz=1; }
		else if(!(tg&0x40))
			{ tsz=2; }
		else if(!(tg&0x80))
		{
			tsz=2;
			j=BTM_BItRc_DecodeByteLeCtx8B(ctx, prbtab_d, &lc);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			if(tag&0x0100)
				tsz=6;
		}else
		{
			tsz=2;
			j=BTM_BItRc_DecodeByteLeCtx8B(ctx, prbtab_d, &lc);
			*ct++=j;
			tag|=j<<8;
			tsi=2;
			if(!(tag&0x0100))
				tsz=5;
		}
		
		for(i=tsi; i<tsz; i++)
		{
			j=BTM_BItRc_DecodeByteLeCtx8B(ctx, prbtab_d, &lc);
			*ct++=j;
			tag|=((u64)j)<<(i*8);
		}
		
		if(!tag)
			break;
		
		if(!(tag&0x0001))		{ nr=(tag>>1)&7; }
		else if(!(tag&0x0002))	{ nr=(tag>>2)&7; }
		else if(!(tag&0x0004))	{ nr=(tag>>3)&7; }
		else if(!(tag&0x0008))	{ nr=(((tag>>4)&15)+1)*8; }
		else if(!(tag&0x0010))	{ nr=(tag>>5)&7; }
		else if(!(tag&0x0020))	{ nr=(tag>>6)&3; }
		else if(!(tag&0x0040))	{ nr=(((tag>>7)&511)+1)*8; }	
		else if(!(tag&0x0080))	{ nr=(tag&0x0100)?((tag>>9)&7):0; }	
		else if(!(tag&0x0100))	{ nr=(tag>>9)&7; }	
		for(i=0; i<nr; i++)
			{ *ct++=BTM_BItRc_DecodeByteLeCtx8B(ctx, prbtab_l, &lc); }
	}
	return(ct-obuf);
}

int BTM_BItRc_EncodeBufferPostRp2Test(byte *obuf, byte *ibuf, int ibsz)
{
	byte *i2buf;
	int osz, i2sz;
	
	osz=BTM_BItRc_EncodeBufferPostRp2(obuf, ibuf, ibsz);
	if(osz<0)
		return(-1);

	i2buf=malloc(ibsz*2);
	i2sz=BTM_BItRc_DecodeBufferPostRp2(i2buf, obuf, osz);

	if(i2sz!=ibsz)
	{
		free(i2buf);
		return(-1);
	}

	if(memcmp(i2buf, ibuf, ibsz))
	{
		free(i2buf);
		return(-1);
	}

	free(i2buf);

	return(osz);
}

int PostRp2BitRc_DecodeBufferPostRp2(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(BTM_BItRc_DecodeBufferPostRp2(obuf, ibuf, ibsz));
}

int PostRp2BitRc_DecodeBufferRp2Full(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
#ifdef HAS_RP2STF_DECODERP2
	static byte *ts_buf;
	byte *tbuf;
	int tbsz, osz;
	
	if(ibsz<8192)
	{
		if(!ts_buf)
			ts_buf=malloc(32768);
		tbuf=ts_buf;
	}else
	{
		tbuf=malloc(ibsz*4);
	}
	
	tbsz=BTM_BItRc_DecodeBufferPostRp2(tbuf, ibuf, ibsz);
	if(tbsz<=0)
	{
		if(tbuf!=ts_buf)
			free(tbuf);
		return(tbsz);
	}
	
	osz=PostRp2Stf_DecodeRP2(obuf, tbuf, obsz, tbsz);
	if(tbuf!=ts_buf)
		free(tbuf);
	return(osz);
#else
	return(-1);
#endif
}

int PostRp2BitRc_EncodeBufferPostRp2(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(BTM_BItRc_EncodeBufferPostRp2(obuf, ibuf, ibsz));
}

int PostRp2BitRc_EncodeBufferPostRp2Test(
	byte *obuf, byte *ibuf, int obsz, int ibsz)
{
	return(BTM_BItRc_EncodeBufferPostRp2Test(obuf, ibuf, ibsz));
}

#endif
