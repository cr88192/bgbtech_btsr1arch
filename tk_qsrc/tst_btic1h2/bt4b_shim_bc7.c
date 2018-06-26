/*
 * TBD: Fast Shim decoder to BC7.
 * Goal will be to quickly shim MetaBlock2 blocks to analogous BC7 blocks.
 * Conversion Speed will be the priority.
 */

u16 btic4b_bc7_4x1x2to4x1x3[256];
u64 btic4b_bc7_2x2to4x4x3[256];
u32 btic4b_bc7_2x2to4x4x2[256];
u16 btic4b_bc7_4x1x1to4x1x3[16];
byte btic4b_bc7_4x1x1to4x1x2[16];
byte btic4b_bc7_4x1x3to4x1x2[4096];
byte btic4b_bc7_4x1x3to4x1x2a[4096];
byte btic4b_bc7_4x1x3to4x1x2b[4096];

byte btic4b_bc7_swap4x1x2[256];


void BTIC4B_ConvBlockBC7_Init()
{
	static int init=0;
	int p0, p1, p2, p3;
	int q0, q1, q2, q3;
	int r0, r1, r2, r3;
	int i, j, k;
	
	if(init)return;
	init=1;
	
	for(i=0; i<256; i++)
	{
		p0=(i>>6)&3;	p1=(i>>4)&3;
		p2=(i>>2)&3;	p3=(i>>0)&3;
		q0=(p0<<1)|(p0>>1);
		q1=(p1<<1)|(p1>>1);
		q2=(p2<<1)|(p2>>1);
		q3=(p3<<1)|(p3>>1);
		
//		r0=(q0<<9)|(q1<<6)|(q2<<3)|q3;
		r0=(q3<<9)|(q2<<6)|(q1<<3)|q0;
		btic4b_bc7_4x1x2to4x1x3[i]=r0;

		r0=p0|(p1<<2)|(p2<<4)|(p3<<6);
		btic4b_bc7_swap4x1x2[i]=r0;
	}

	for(i=0; i<256; i++)
	{
//		p0=(i>>6)&3;	p1=(i>>4)&3;
//		p2=(i>>2)&3;	p3=(i>>0)&3;

		p3=(i>>6)&3;	p2=(i>>4)&3;
		p1=(i>>2)&3;	p0=(i>>0)&3;

		q0=(p0<<1)|(p0>>1);
		q1=(p1<<1)|(p1>>1);
		q2=(p2<<1)|(p2>>1);
		q3=(p3<<1)|(p3>>1);
		
//		r0=(q0<<9)|(q0<<6)|(q1<<3)|q1;
//		r1=(q2<<9)|(q2<<6)|(q3<<3)|q3;
//		r2=(r0<<12)|r0;
//		r3=(r1<<12)|r1;

		r0=(q1<<9)|(q1<<6)|(q0<<3)|q0;
		r1=(q3<<9)|(q3<<6)|(q2<<3)|q2;
		r2=(r0<<12)|r0;
		r3=(r1<<12)|r1;
		btic4b_bc7_2x2to4x4x3[i]=(((u64)r3)<<24)|r2;

//		r0=(p0<<6)|(p0<<4)|(p1<<2)|p1;
//		r1=(p2<<6)|(p2<<4)|(p3<<2)|p3;
//		r2=(r0<<24)|(r0<<16)|(r1<<8)|r1;

		r0=(p1<<6)|(p1<<4)|(p0<<2)|p0;
		r1=(p3<<6)|(p3<<4)|(p2<<2)|p2;
		r2=(r1<<24)|(r1<<16)|(r0<<8)|r0;
		btic4b_bc7_2x2to4x4x2[i]=r2;
	}

	for(i=0; i<16; i++)
	{
//		p0=(i>>3)&1;	p1=(i>>2)&1;
//		p2=(i>>1)&1;	p3=(i   )&1;

		p3=(i>>3)&1;	p2=(i>>2)&1;
		p1=(i>>1)&1;	p0=(i   )&1;

		q0=p0?7:0;		q1=p1?7:0;
		q2=p2?7:0;		q3=p3?7:0;
//		r0=(q0<<9)|(q1<<6)|(q2<<3)|q3;
		r0=(q3<<9)|(q2<<6)|(q1<<3)|q0;
		btic4b_bc7_4x1x1to4x1x3[i]=r0;

		q0=p0?3:0;		q1=p1?3:0;
		q2=p2?3:0;		q3=p3?3:0;
//		r0=(q0<<6)|(q1<<4)|(q2<<2)|q3;
		r0=(q3<<6)|(q2<<4)|(q1<<2)|q0;
		btic4b_bc7_4x1x1to4x1x2[i]=r0;
	}


	for(i=0; i<4096; i++)
	{
		p3=(i>>9)&7;	p2=(i>>6)&7;
		p1=(i>>3)&7;	p0=(i>>0)&7;

		q0=(p0+1)>>1;	q1=(p1-1)>>1;
		q2=(p2+1)>>1;	q3=(p3-1)>>1;
		q0=(q0<0)?0:((q0>3)?3:q0);	q1=(q1<0)?0:((q1>3)?3:q1);
		q2=(q2<0)?0:((q2>3)?3:q2);	q3=(q3<0)?0:((q3>3)?3:q3);
		r0=q0|(q1<<2)|(q2<<4)|(q3<<6);

		q0=(p0-1)>>1;	q1=(p1+1)>>1;
		q2=(p2-1)>>1;	q3=(p3+1)>>1;
		q0=(q0<0)?0:((q0>3)?3:q0);	q1=(q1<0)?0:((q1>3)?3:q1);
		q2=(q2<0)?0:((q2>3)?3:q2);	q3=(q3<0)?0:((q3>3)?3:q3);
		r1=q0|(q1<<2)|(q2<<4)|(q3<<6);

		q0=p0>>1;	q1=p1>>1;
		q2=p2>>1;	q3=p3>>1;
		r2=q0|(q1<<2)|(q2<<4)|(q3<<6);

		btic4b_bc7_4x1x3to4x1x2a[i]=r0;
		btic4b_bc7_4x1x3to4x1x2b[i]=r1;
		btic4b_bc7_4x1x3to4x1x2[i]=r2;
	}
}

void BTIC4B_BitsLE_Clear(BTIC4B_BitStream *ctx)
{
	memset(ctx, 0, sizeof(BTIC4B_BitStream));
}

void BTIC4B_BitsLE_SetupWrite(BTIC4B_BitStream *ctx, byte *ct, int sz)
{
	ctx->ct=ct;
	ctx->cte=ct+sz;
	ctx->win=0;
	ctx->pos=0;
}

void BTIC4B_BitsLE_ClearSetupWrite(BTIC4B_BitStream *ctx, byte *ct, int sz)
{
	BTIC4B_BitsLE_Clear(ctx);
	BTIC4B_BitsLE_SetupWrite(ctx, ct, sz);
}

force_inline void BTIC4B_BitsLE_WriteBits(
	BTIC4B_BitStream *ctx, int v, int n)
{
#if defined(X86)||defined(X86_64)
//	static const mask[5]={
//		0x00000000, 0x000000FF, 0x0000FFFF, 0x00FFFFFF, 0xFFFFFFFF};
// #if 0
	u32 bw, m;
	int bp;
	int i;
	
	v&=(1<<n)-1;
	bp=ctx->pos;
	bw=ctx->win;
	bw=bw|(v<<bp);
	bp=bp+n;

	i=bp>>3;
	*(u32 *)ctx->ct=bw;
//	m=mask[i];
//	*(u32 *)ctx->ct=(bw&m)|((*(u32 *)ctx->ct)&(~m));
	ctx->ct+=i;
	ctx->win=(bw>>(i<<3));
	ctx->pos=bp&7;
#else
	v&=(1<<n)-1;
	ctx->win|=v<<(ctx->pos);
	ctx->pos+=n;
	while(ctx->pos>=8)
	{
		*ctx->ct++=(ctx->win)&255;
		ctx->win>>=8;
		ctx->pos-=8;
	}
#endif
}

#if 0
void BTIC4B_BitsLE_WriteLBits(BTIC4B_BitStream *ctx, int v, int n)
{
	v&=(1<<n)-1;
	ctx->win|=v<<(ctx->pos);
	ctx->pos+=n;
	if(ctx->pos>=8)
	{
		*ctx->ct++=(ctx->win)&255;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}
#endif

#if 0
void BTIC4B_BitsLE_WriteBit(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 1); }
void BTIC4B_BitsLE_Write2Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 2); }
void BTIC4B_BitsLE_Write3Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 3); }
void BTIC4B_BitsLE_Write4Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 4); }
void BTIC4B_BitsLE_Write5Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 5); }
void BTIC4B_BitsLE_Write6Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 6); }
void BTIC4B_BitsLE_Write7Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteLBits(ctx, v, 7); }
// void BTIC4B_BitsLE_Write8Bits(BTIC4B_BitStream *ctx, int v)
//	{ BTIC4B_BitsLE_WriteBits(ctx, v, 8); }
#endif

#if 1
default_inline void BTIC4B_BitsLE_WriteBit(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 1); }
default_inline void BTIC4B_BitsLE_Write2Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 2); }
default_inline void BTIC4B_BitsLE_Write3Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 3); }
default_inline void BTIC4B_BitsLE_Write4Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 4); }
default_inline void BTIC4B_BitsLE_Write5Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 5); }
default_inline void BTIC4B_BitsLE_Write6Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 6); }
default_inline void BTIC4B_BitsLE_Write7Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 7); }
#endif

void BTIC4B_BitsLE_Write10Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 10); }
void BTIC4B_BitsLE_Write11Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 11); }
void BTIC4B_BitsLE_Write12Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 12); }
void BTIC4B_BitsLE_Write14Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 14); }
void BTIC4B_BitsLE_Write15Bits(BTIC4B_BitStream *ctx, int v)
	{ BTIC4B_BitsLE_WriteBits(ctx, v, 15); }
// void BTIC4B_BitsLE_Write16Bits(BTIC4B_BitStream *ctx, int v)
//	{ BTIC4B_BitsLE_WriteBits(ctx, v, 16); }

void BTIC4B_BitsLE_Write8Bits(BTIC4B_BitStream *ctx, int v)
{
#if defined(X86)||defined(X86_64)
// #if 0
	u32 bw;
	bw=ctx->win|(((byte)v)<<ctx->pos);
	*ctx->ct++=bw;
	ctx->win=(bw>>8);
#else
	ctx->win|=(v&255)<<(ctx->pos);
	*ctx->ct++=ctx->win; ctx->win>>=8;
#endif
}

void BTIC4B_BitsLE_Write16Bits(BTIC4B_BitStream *ctx, int v)
{
#if defined(X86)||defined(X86_64)
//#if 0
	u32 bw;
//	bw=ctx->win|((v&65535)<<ctx->pos);
	bw=ctx->win|(((u16)v)<<ctx->pos);
	*(u32 *)ctx->ct=bw;
	ctx->ct+=2;
	ctx->win=(bw>>16);
#else
	ctx->win|=(v&65535)<<(ctx->pos);
	*ctx->ct++=ctx->win; ctx->win>>=8;
	*ctx->ct++=ctx->win; ctx->win>>=8;
#endif
}

void BTIC4B_BitsLE_Write24Bits(BTIC4B_BitStream *ctx, int v)
{
#if defined(X86)||defined(X86_64)
//#if 0
	u32 bw;
	bw=ctx->win|((v&16777215)<<ctx->pos);
	*(u32 *)ctx->ct=bw;
	ctx->ct+=3;
	ctx->win=(bw>>24);
#else
	ctx->win|=(v&65535)<<(ctx->pos);
	*ctx->ct++=ctx->win; ctx->win>>=8;
	*ctx->ct++=ctx->win; ctx->win>>=8;
#endif
}

void BTIC4B_BitsLE_FlushBits(BTIC4B_BitStream *ctx)
{
	while(ctx->pos>0)
	{
		*ctx->ct++=(ctx->win)&255;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void BTIC4B_BC7_EncodeBlock_VecSwapRGB(int *clra, int *clrb)
{
	int t;
	t=clra[0]; clra[0]=clrb[0]; clrb[0]=t;
	t=clra[1]; clra[1]=clrb[1]; clrb[1]=t;
	t=clra[2]; clra[2]=clrb[2]; clrb[2]=t;
}

void BTIC4B_BC7_EncodeBlock_VecSwapRGBA(int *clra, int *clrb)
{
	int t;
	t=clra[0]; clra[0]=clrb[0]; clrb[0]=t;
	t=clra[1]; clra[1]=clrb[1]; clrb[1]=t;
	t=clra[2]; clra[2]=clrb[2]; clrb[2]=t;
	t=clra[3]; clra[3]=clrb[3]; clrb[3]=t;
}

void BTIC4B_BC7_EncodeBlock_VecSwapA(int *clra, int *clrb)
	{ int t; t=clra[3]; clra[3]=clrb[3]; clrb[3]=t; }

void BTIC4B_BC7_EncodeBlock_VecInvertPixels(
	s16 *pxv, int n, int min, int max)
{
	int i, j, k;
	
	for(i=0; i<n; i++)
		{ pxv[i]=max-(pxv[i]-min); }
}

void BTIC4B_BC7_EncodeBlock_Mode4(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	static const char idxtab2[16]=
		{ 0,0,0,0, 0,1,2,3, 4,5,6,7, 7,7,7,7 };
	
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3a, l3b;
	int avg[4], acy, aca;
	int acr, acg, acb, ixb;
	int cr, cg, cb, ca, cy;
	int i, j, k, l;

	avg[0]=(min[0]+max[0])>>1;
	avg[1]=(min[1]+max[1])>>1;
	avg[2]=(min[2]+max[2])>>1;
	avg[3]=(min[3]+max[3])>>1;	
	acy=(mcy+ncy)>>1;
	aca=(mca+nca)>>1;
	
	ixb=1;

	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 16, 5);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);
	BTIC4B_BitsLE_WriteBits(&bits, ixb, 1);

	if(pxy[0]>=acy)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGB(min, max);
		BTIC4B_BC7_EncodeBlock_VecInvertPixels(pxy, 16, mcy, ncy);
	}

	if(pxa[0]>=aca)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapA(min, max);
		BTIC4B_BC7_EncodeBlock_VecInvertPixels(pxa, 16, mca, nca);
	}

	BTIC4B_BitsLE_WriteBits(&bits, (min[0])>>3, 5);
	BTIC4B_BitsLE_WriteBits(&bits, (max[0])>>3, 5);
	BTIC4B_BitsLE_WriteBits(&bits, (min[1])>>3, 5);
	BTIC4B_BitsLE_WriteBits(&bits, (max[1])>>3, 5);
	BTIC4B_BitsLE_WriteBits(&bits, (min[2])>>3, 5);
	BTIC4B_BitsLE_WriteBits(&bits, (max[2])>>3, 5);
	BTIC4B_BitsLE_WriteBits(&bits, (min[3])>>2, 6);
	BTIC4B_BitsLE_WriteBits(&bits, (max[3])>>2, 6);

//	l0=49152/(nca-aca+1);		//Fix-Point Scale (Alpha)
	l0=32768/(nca-aca+1);		//Fix-Point Scale (Alpha)
	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l2=65536-2048;
	l3a=65536;

	if(ixb)
	{
		p0=idxtab[((pxa[0]-aca)*l0+l3a)>>13];
		BTIC4B_BitsLE_WriteBit(&bits, p0);
		for(i=1; i<16; i++)
		{
			p0=idxtab[((pxa[i]-aca)*l0+l3a)>>13];
			BTIC4B_BitsLE_Write2Bits(&bits, p0);
		}

		p0=idxtab2[((pxy[0]-acy)*l1+l3a)>>13];
		BTIC4B_BitsLE_Write2Bits(&bits, p0);
		for(i=1; i<16; i++)
		{
			p0=idxtab2[((pxy[i]-acy)*l1+l3a)>>13];
			BTIC4B_BitsLE_Write3Bits(&bits, p0);
		}
	}else
	{
		p0=idxtab[((pxy[0]-acy)*l1+l3a)>>13];
		BTIC4B_BitsLE_WriteBit(&bits, p0);
		for(i=1; i<16; i++)
		{
			p0=idxtab[((pxy[i]-acy)*l1+l3a)>>13];
			BTIC4B_BitsLE_Write2Bits(&bits, p0);
		}

		p0=idxtab2[((pxa[0]-aca)*l0+l3a)>>13];
		BTIC4B_BitsLE_Write2Bits(&bits, p0);
		for(i=1; i<16; i++)
		{
			p0=idxtab2[((pxa[i]-aca)*l0+l3a)>>13];
			BTIC4B_BitsLE_Write3Bits(&bits, p0);
		}
	}

	BTIC4B_BitsLE_FlushBits(&bits);
	memcpy(block, tblock, 16);
}

void BTIC4B_BC7_EncodeBlock_Mode5(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	
	byte tblock[24];
	BTIC4B_BitStream bits;

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int q0, q1, q2, q3;
	int l0, l1, l2, l3a, l3b;
	int avg[4], acy, aca;
	int acr, acg, acb;
	int cr, cg, cb, ca, cy;
	int i, j, k, l;

	avg[0]=(min[0]+max[0])>>1;
	avg[1]=(min[1]+max[1])>>1;
	avg[2]=(min[2]+max[2])>>1;
	avg[3]=(min[3]+max[3])>>1;	
	acy=(mcy+ncy)>>1;
	aca=(mca+nca)>>1;
	
	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
//	BTIC4B_BitsLE_ClearSetupWrite(&bits, block, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 32, 6);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);

	if((pxy[0]>=acy) && (ncy>mcy))
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGB(min, max);
		BTIC4B_BC7_EncodeBlock_VecInvertPixels(pxy, 16, mcy, ncy);
	}

	if((pxa[0]>=aca) && (nca>mca))
	{
		BTIC4B_BC7_EncodeBlock_VecSwapA(min, max);
		BTIC4B_BC7_EncodeBlock_VecInvertPixels(pxa, 16, mca, nca);
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=min[3]|(max[3]<<8);

	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write16Bits(&bits, p3);

//	BTIC4B_BitsLE_WriteBits(&bits, (min[0])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[0])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[1])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[1])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[2])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[2])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, min[3], 8);
//	BTIC4B_BitsLE_WriteBits(&bits, max[3], 8);

//	l0=32768/(nca-aca+1);		//Fix-Point Scale (Alpha)
//	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
//	l2=65536-2048;
//	l3a=65536;

	l3a=65536-2048;
	l3b=65536+2048;

	if(ncy>mcy)
	{
		l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)

#if 0
		p0=idxtab[((pxy[0]-acy)*l1+l3a)>>13];
		BTIC4B_BitsLE_WriteBit(&bits, p0);
		for(i=1; i<16; i++)
		{
			p0=idxtab[((pxy[i]-acy)*l1+l3a)>>13];
			BTIC4B_BitsLE_Write2Bits(&bits, p0);
		}
#endif

		p0=idxtab[((pxy[ 0]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 1]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[ 2]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[ 3]-acy)*l1+l3b)>>13];
		q0=p0|(p1<<1)|(p2<<3)|(p3<<5);
		p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>13];
		q1=p0|(p1<<2)|(p2<<4)|(p3<<6);
		p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>13];
		p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>13];
		p2=idxtab[((pxy[10]-acy)*l1+l3a)>>13];
		p3=idxtab[((pxy[11]-acy)*l1+l3b)>>13];
		q2=p0|(p1<<2)|(p2<<4)|(p3<<6);
		p0=idxtab[((pxy[12]-acy)*l1+l3b)>>13];
		p1=idxtab[((pxy[13]-acy)*l1+l3a)>>13];
		p2=idxtab[((pxy[14]-acy)*l1+l3b)>>13];
		p3=idxtab[((pxy[15]-acy)*l1+l3a)>>13];
		q3=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write15Bits(&bits, q0|(q1<<7));
		BTIC4B_BitsLE_Write16Bits(&bits, q2|(q3<<8));
	}else
	{
		BTIC4B_BitsLE_Write15Bits(&bits, 0);
		BTIC4B_BitsLE_Write16Bits(&bits, 0);
	}

	if(nca>mca)
	{
		l0=32768/(nca-aca+1);		//Fix-Point Scale (Alpha)

#if 0
		p0=idxtab[((pxa[0]-aca)*l0+l3a)>>13];
		BTIC4B_BitsLE_WriteBit(&bits, p0);
		for(i=1; i<16; i++)
		{
			p0=idxtab[((pxa[i]-aca)*l0+l3a)>>13];
			BTIC4B_BitsLE_Write2Bits(&bits, p0);
		}
#endif

		p0=idxtab[((pxa[ 0]-aca)*l0+l3a)>>13];
		p1=idxtab[((pxa[ 1]-aca)*l0+l3b)>>13];
		p2=idxtab[((pxa[ 2]-aca)*l0+l3a)>>13];
		p3=idxtab[((pxa[ 3]-aca)*l0+l3b)>>13];
		q0=p0|(p1<<1)|(p2<<3)|(p3<<5);
		p0=idxtab[((pxa[ 4]-aca)*l0+l3b)>>13];
		p1=idxtab[((pxa[ 5]-aca)*l0+l3a)>>13];
		p2=idxtab[((pxa[ 6]-aca)*l0+l3b)>>13];
		p3=idxtab[((pxa[ 7]-aca)*l0+l3a)>>13];
		q1=p0|(p1<<2)|(p2<<4)|(p3<<6);
		p0=idxtab[((pxa[ 8]-aca)*l0+l3a)>>13];
		p1=idxtab[((pxa[ 9]-aca)*l0+l3b)>>13];
		p2=idxtab[((pxa[10]-aca)*l0+l3a)>>13];
		p3=idxtab[((pxa[11]-aca)*l0+l3b)>>13];
		q2=p0|(p1<<2)|(p2<<4)|(p3<<6);
		p0=idxtab[((pxa[12]-aca)*l0+l3b)>>13];
		p1=idxtab[((pxa[13]-aca)*l0+l3a)>>13];
		p2=idxtab[((pxa[14]-aca)*l0+l3b)>>13];
		p3=idxtab[((pxa[15]-aca)*l0+l3a)>>13];
		q3=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write15Bits(&bits, q0|(q1<<7));
		BTIC4B_BitsLE_Write16Bits(&bits, q2|(q3<<8));
	}else
	{
		BTIC4B_BitsLE_Write15Bits(&bits, 0);
		BTIC4B_BitsLE_Write16Bits(&bits, 0);
	}

	BTIC4B_BitsLE_FlushBits(&bits);

#if 1
	memcpy(block, tblock, 16);
#endif
}

void BTIC4B_BC7_EncodeBlock_Mode6(byte *block,
	s16 *pxy,
	int *min, int *max,
	int mcy, int ncy)
{
	static const char idxtab[32]=
		{  0,  0,  0,  0,  0,  0,  0,  0,
		   0,  1,  2,  3,  4,  5,  6,  7,
		   8,  9, 10, 11, 12, 13, 14, 15,
		  15, 15, 15, 15, 15, 15, 15, 15};
	
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3a, l3b;
	int acy, aca;
	int acr, acg, acb;
	int cr, cg, cb, ca, cy;
	int i, j, k, l;

	acy=(mcy+ncy)>>1;

	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 64, 7);

	if(pxy[0]>=acy)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGBA(min, max);
		BTIC4B_BC7_EncodeBlock_VecInvertPixels(pxy, 16, mcy, ncy);
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=((min[3])>>1)|(((max[3])>>1)<<7);

	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write14Bits(&bits, p3);

//	BTIC4B_BitsLE_WriteBits(&bits, (min[0])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[0])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[1])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[1])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[2])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[2])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[3])>>1, 7);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[3])>>1, 7);
	BTIC4B_BitsLE_WriteBit(&bits, min[1]);
	BTIC4B_BitsLE_WriteBit(&bits, max[1]);

//	l0=49152/(nca-aca+1);		//Fix-Point Scale (Alpha)
	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
//	l1=(32768-512)/(ncy-acy+1);		//Fix-Point Scale (Luma)
//	l2=65536-2048;
	l3a=65536-1024;
	l3b=65536-1024+512;

	p0=idxtab[((pxy[0]-acy)*l1+l3a)>>12];
	p1=idxtab[((pxy[1]-acy)*l1+l3b)>>12];
	p2=idxtab[((pxy[2]-acy)*l1+l3a)>>12];
	p3=idxtab[((pxy[3]-acy)*l1+l3b)>>12];
	if(p0>7)p0=7;
	BTIC4B_BitsLE_Write3Bits(&bits, p0);
	BTIC4B_BitsLE_Write12Bits(&bits, p1|(p2<<4)|(p3<<8));

	p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>12];
	p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>12];
	p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>12];
	p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>12];
	BTIC4B_BitsLE_Write16Bits(&bits, p0|(p1<<4)|(p2<<8)|(p3<<12));
	p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>12];
	p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>12];
	p2=idxtab[((pxy[10]-acy)*l1+l3a)>>12];
	p3=idxtab[((pxy[11]-acy)*l1+l3b)>>12];
	BTIC4B_BitsLE_Write16Bits(&bits, p0|(p1<<4)|(p2<<8)|(p3<<12));
	p0=idxtab[((pxy[12]-acy)*l1+l3b)>>12];
	p1=idxtab[((pxy[13]-acy)*l1+l3a)>>12];
	p2=idxtab[((pxy[14]-acy)*l1+l3b)>>12];
	p3=idxtab[((pxy[15]-acy)*l1+l3a)>>12];
	BTIC4B_BitsLE_Write16Bits(&bits, p0|(p1<<4)|(p2<<8)|(p3<<12));

	BTIC4B_BitsLE_FlushBits(&bits);

	memcpy(block, tblock, 16);
}

void BTIC4B_BC7_EncodeBlock_AutoMode(byte *block,
	s16 *pxy, s16 *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
//	if(mca==nca)
	if((nca-mca)<8)
	{
		BTIC4B_BC7_EncodeBlock_Mode6(block,
			pxy, min, max, mcy, ncy);
		return;
	}
	
	if((ncy-mcy)>64)
	{
		BTIC4B_BC7_EncodeBlock_Mode4(block, pxy, pxa,
			min, max, mcy, ncy, mca, nca);
		return;
	}
	
	BTIC4B_BC7_EncodeBlock_Mode5(block, pxy, pxa,
		min, max, mcy, ncy, mca, nca);
}

void BTIC4B_BC7_EncodeBlockBits48_Mode5(byte *block,
	u64 pxy, u64 pxa,
	int *min, int *max)
{
	static const char idxtab[8]=
		{ 0,0,1,1, 2,2,3,3 };
	
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i, j, k, l;

//	return;

//	BTIC4B_BitsLE_ClearSetupWrite(&bits, block, 16);
	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 32, 6);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);

//	if(((pxy>>45)&7)>=4)
	if(pxy&4)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGB(min, max);
		pxy=~pxy;
	}

//	if(((pxa>>45)&7)>=4)
	if(pxa&4)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapA(min, max);
		pxa=~pxa;
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=min[3]|(max[3]<<8);

	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write16Bits(&bits, p3);

#if 0
	BTIC4B_BitsLE_WriteBits(&bits, (min[0])>>1, 7);
	BTIC4B_BitsLE_WriteBits(&bits, (max[0])>>1, 7);
	BTIC4B_BitsLE_WriteBits(&bits, (min[1])>>1, 7);
	BTIC4B_BitsLE_WriteBits(&bits, (max[1])>>1, 7);
	BTIC4B_BitsLE_WriteBits(&bits, (min[2])>>1, 7);
	BTIC4B_BitsLE_WriteBits(&bits, (max[2])>>1, 7);
	BTIC4B_BitsLE_WriteBits(&bits, min[3], 8);
	BTIC4B_BitsLE_WriteBits(&bits, max[3], 8);
#endif

#if 0
//	p0=idxtab[(pxy>>45)&7];
	p0=idxtab[pxy&7];
	BTIC4B_BitsLE_WriteBit(&bits, p0);
	for(i=1; i<16; i++)
	{
//		p0=idxtab[(pxy>>(45-i*3))&7];
		p0=idxtab[(pxy>>(i*3))&7];
		BTIC4B_BitsLE_Write2Bits(&bits, p0);
	}
#endif

#if 1
	p0=idxtab[(pxy   )&7];	p1=idxtab[(pxy>>3)&7];
	p2=idxtab[(pxy>>6)&7];	p3=idxtab[(pxy>>9)&7];
	p4=p0|(p1<<1)|(p2<<3)|(p3<<5);
	BTIC4B_BitsLE_Write7Bits(&bits, p4);
	p0=idxtab[(pxy>>12)&7];	p1=idxtab[(pxy>>15)&7];
	p2=idxtab[(pxy>>18)&7];	p3=idxtab[(pxy>>21)&7];
	p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
	BTIC4B_BitsLE_Write8Bits(&bits, p4);
	p0=idxtab[(pxy>>24)&7];	p1=idxtab[(pxy>>27)&7];
	p2=idxtab[(pxy>>30)&7];	p3=idxtab[(pxy>>33)&7];
	p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
	BTIC4B_BitsLE_Write8Bits(&bits, p4);
	p0=idxtab[(pxy>>36)&7];	p1=idxtab[(pxy>>39)&7];
	p2=idxtab[(pxy>>42)&7];	p3=idxtab[(pxy>>45)&7];
	p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
	BTIC4B_BitsLE_Write8Bits(&bits, p4);
#endif

	if(min[3]!=max[3])
	{
#if 0
	//	p0=idxtab[(pxa>>45)&7];
		p0=idxtab[pxa&7];
		BTIC4B_BitsLE_WriteBit(&bits, p0);
		for(i=1; i<16; i++)
		{
	//		p0=idxtab[(pxa>>(45-i*3))&7];
			p0=idxtab[(pxa>>(i*3))&7];
			BTIC4B_BitsLE_Write2Bits(&bits, p0);
		}
#endif

#if 1
		p0=idxtab[(pxa   )&7];	p1=idxtab[(pxa>>3)&7];
		p2=idxtab[(pxa>>6)&7];	p3=idxtab[(pxa>>9)&7];
		p4=p0|(p1<<1)|(p2<<3)|(p3<<5);
		BTIC4B_BitsLE_Write7Bits(&bits, p4);
		p0=idxtab[(pxa>>12)&7];	p1=idxtab[(pxa>>15)&7];
		p2=idxtab[(pxa>>18)&7];	p3=idxtab[(pxa>>21)&7];
		p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write8Bits(&bits, p4);
		p0=idxtab[(pxa>>24)&7];	p1=idxtab[(pxa>>27)&7];
		p2=idxtab[(pxa>>30)&7];	p3=idxtab[(pxa>>33)&7];
		p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write8Bits(&bits, p4);
		p0=idxtab[(pxa>>36)&7];	p1=idxtab[(pxa>>39)&7];
		p2=idxtab[(pxa>>42)&7];	p3=idxtab[(pxa>>45)&7];
		p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write8Bits(&bits, p4);
#endif
	}else
	{
		BTIC4B_BitsLE_Write15Bits(&bits, 0);
		BTIC4B_BitsLE_Write16Bits(&bits, 0);
	}

	BTIC4B_BitsLE_FlushBits(&bits);

#if 1
	memcpy(block, tblock, 16);
#endif
}

void BTIC4B_BC7_EncodeBlockBits64_Mode5(byte *block,
	u64 pxy, int *min, int *max)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 1,1,1,1, 2,2,2,2, 3,3,3,3 };
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i, j, k, l;

	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 32, 6);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);

	if(pxy&4)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGB(min, max);
		pxy=~pxy;
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=min[3]|(max[3]<<8);

	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write16Bits(&bits, p3);

	p0=idxtab[pxy&15];
	BTIC4B_BitsLE_WriteBit(&bits, p0);
	for(i=1; i<16; i++)
	{
		p0=idxtab[(pxy>>(i*4))&15];
		BTIC4B_BitsLE_Write2Bits(&bits, p0);
	}

	BTIC4B_BitsLE_Write15Bits(&bits, 0);
	BTIC4B_BitsLE_Write16Bits(&bits, 0);

	BTIC4B_BitsLE_FlushBits(&bits);

#if 1
	memcpy(block, tblock, 16);
#endif
}

void BTIC4B_BC7_EncodeBlockBits64_Mode6(byte *block,
	u64 pxy, int *min, int *max)
{
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3a, l3b;
	int acy, aca;
	int acr, acg, acb;
	int cr, cg, cb, ca, cy;
	int i, j, k, l;

	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 64, 7);

	if(pxy&8)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGBA(min, max);
		pxy=~pxy;
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=((min[3])>>1)|(((max[3])>>1)<<7);
	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write14Bits(&bits, p3);
	BTIC4B_BitsLE_WriteBit(&bits, min[1]);
	BTIC4B_BitsLE_WriteBit(&bits, max[1]);

	BTIC4B_BitsLE_Write3Bits(&bits, pxy);
	BTIC4B_BitsLE_Write12Bits(&bits, pxy>>4);
	BTIC4B_BitsLE_Write16Bits(&bits, pxy>>16);
	BTIC4B_BitsLE_Write16Bits(&bits, pxy>>32);
	BTIC4B_BitsLE_Write16Bits(&bits, pxy>>48);

	BTIC4B_BitsLE_FlushBits(&bits);

	memcpy(block, tblock, 16);
}


void BTIC4B_BC7_EncodeBlockBits48_Mode4(byte *block,
	u64 pxy, u64 pxa, int *min, int *max)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	static const char idxtab2[16]=
		{ 0,0,0,0, 0,1,2,3, 4,5,6,7, 7,7,7,7 };
	
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
//	int l0, l1, l2, l3a, l3b;
//	int avg[4], acy, aca;
//	int acr, acg, acb;
	int ixb;
	int cr, cg, cb, ca, cy;
	int i, j, k, l;

//	avg[0]=(min[0]+max[0])>>1;
//	avg[1]=(min[1]+max[1])>>1;
//	avg[2]=(min[2]+max[2])>>1;
//	avg[3]=(min[3]+max[3])>>1;	
//	acy=(mcy+ncy)>>1;
//	aca=(mca+nca)>>1;
	
	ixb=1;

	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 16, 5);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);
	BTIC4B_BitsLE_WriteBits(&bits, ixb, 1);

	if(pxy&4)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGB(min, max);
		pxy=~pxy;
	}

	if(pxa&4)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapA(min, max);
		pxa=~pxa;
	}

	p0=((min[0])>>3)|(((max[0])>>3)<<5);
	p1=((min[1])>>3)|(((max[1])>>3)<<5);
	p2=((min[2])>>3)|(((max[2])>>3)<<5);
	p3=((min[3])>>2)|(((max[3])>>2)<<6);
	BTIC4B_BitsLE_Write10Bits(&bits, p0);
	BTIC4B_BitsLE_Write10Bits(&bits, p1);
	BTIC4B_BitsLE_Write10Bits(&bits, p2);
	BTIC4B_BitsLE_Write12Bits(&bits, p3);

//	BTIC4B_BitsLE_WriteBits(&bits, (min[0])>>3, 5);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[0])>>3, 5);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[1])>>3, 5);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[1])>>3, 5);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[2])>>3, 5);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[2])>>3, 5);
//	BTIC4B_BitsLE_WriteBits(&bits, (min[3])>>2, 6);
//	BTIC4B_BitsLE_WriteBits(&bits, (max[3])>>2, 6);

	if(ixb)
	{
		if((max[3]-min[3])>=8)
		{
			p0=(pxa>>1)&1;			p1=(pxa>>(1*3+1))&3;
			p2=(pxa>>(2*3+1))&3;	p3=(pxa>>(3*3+1))&3;
			p4=p0|(p1<<1)|(p2<<3)|(p3<<5);
			BTIC4B_BitsLE_Write7Bits(&bits, p4);
			p0=(pxa>>(4*3+1))&3;	p1=(pxa>>(5*3+1))&3;
			p2=(pxa>>(6*3+1))&3;	p3=(pxa>>(7*3+1))&3;
			p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
			BTIC4B_BitsLE_Write8Bits(&bits, p4);
			p0=(pxa>>( 8*3+1))&3;	p1=(pxa>>( 9*3+1))&3;
			p2=(pxa>>(10*3+1))&3;	p3=(pxa>>(11*3+1))&3;
			p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
			BTIC4B_BitsLE_Write8Bits(&bits, p4);
			p0=(pxa>>(12*3+1))&3;	p1=(pxa>>(13*3+1))&3;
			p2=(pxa>>(14*3+1))&3;	p3=(pxa>>(15*3+1))&3;
			p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
			BTIC4B_BitsLE_Write8Bits(&bits, p4);
		}else
		{
			BTIC4B_BitsLE_Write15Bits(&bits, 0);
			BTIC4B_BitsLE_Write16Bits(&bits, 0);
		}

		p0=(pxy   )&3;
		p1=pxy>>3;
		p2=p0|(p1<<2);
		BTIC4B_BitsLE_Write11Bits(&bits, p2);
		BTIC4B_BitsLE_Write12Bits(&bits, pxy>>12);
		BTIC4B_BitsLE_Write12Bits(&bits, pxy>>24);
		BTIC4B_BitsLE_Write12Bits(&bits, pxy>>36);

	}else
	{
		p0=(pxy>>1)&1;			p1=(pxy>>(1*3+1))&3;
		p2=(pxy>>(2*3+1))&3;	p3=(pxy>>(3*3+1))&3;
		p4=p0|(p1<<1)|(p2<<3)|(p3<<5);
		BTIC4B_BitsLE_Write7Bits(&bits, p4);
		p0=(pxy>>(4*3+1))&3;	p1=(pxy>>(5*3+1))&3;
		p2=(pxy>>(6*3+1))&3;	p3=(pxy>>(7*3+1))&3;
		p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write8Bits(&bits, p4);
		p0=(pxy>>( 8*3+1))&3;	p1=(pxy>>( 9*3+1))&3;
		p2=(pxy>>(10*3+1))&3;	p3=(pxy>>(11*3+1))&3;
		p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write8Bits(&bits, p4);
		p0=(pxy>>(12*3+1))&3;	p1=(pxy>>(13*3+1))&3;
		p2=(pxy>>(14*3+1))&3;	p3=(pxy>>(15*3+1))&3;
		p4=p0|(p1<<2)|(p2<<4)|(p3<<6);
		BTIC4B_BitsLE_Write8Bits(&bits, p4);

		if((max[3]-min[3])>=8)
		{
			p0=(pxa   )&3; p1=pxa>>3; p2=p0|(p1<<2);
			BTIC4B_BitsLE_Write11Bits(&bits, p2);
			BTIC4B_BitsLE_Write12Bits(&bits, pxa>>12);
			BTIC4B_BitsLE_Write12Bits(&bits, pxa>>24);
			BTIC4B_BitsLE_Write12Bits(&bits, pxa>>36);
		}else
		{
			BTIC4B_BitsLE_Write11Bits(&bits, 0);
			BTIC4B_BitsLE_Write12Bits(&bits, 0);
			BTIC4B_BitsLE_Write12Bits(&bits, 0);
			BTIC4B_BitsLE_Write12Bits(&bits, 0);
		}
	}

	BTIC4B_BitsLE_FlushBits(&bits);
	memcpy(block, tblock, 16);
}

void BTIC4B_BC7_EncodeBlockBits48_Mode6(byte *block,
	u64 pxy, int *min, int *max)
{
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3a, l3b;
	int acy, aca;
	int acr, acg, acb;
	int cr, cg, cb, ca, cy;
	int i, j, k, l;

	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 64, 7);

	if(pxy&4)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGBA(min, max);
		pxy=~pxy;
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=((min[3])>>1)|(((max[3])>>1)<<7);
	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write14Bits(&bits, p3);
	BTIC4B_BitsLE_WriteBit(&bits, min[1]);
	BTIC4B_BitsLE_WriteBit(&bits, max[1]);

	p5=pxy; p6=pxy>>24;
	p7=p5;
//	p7=pxy;
	p0=(p7   )&7;	p1=(p7>>3)&7;
	p2=(p7>>6)&7;	p3=(p7>>9)&7;
//	p0=(pxy   )&7;	p1=(pxy>>3)&7;
//	p2=(pxy>>6)&7;	p3=(pxy>>9)&7;
	p0=(p0<<1)|(p0>>2);	p1=(p1<<1)|(p1>>2);
	p2=(p2<<1)|(p2>>2);	p3=(p3<<1)|(p3>>2);
	BTIC4B_BitsLE_Write3Bits(&bits, p0);
	BTIC4B_BitsLE_Write12Bits(&bits, p1|(p2<<4)|(p3<<8));

	p7=p5>>12;
//	p7=pxy>>12;
	p0=(p7   )&7;	p1=(p7>>3)&7;
	p2=(p7>>6)&7;	p3=(p7>>9)&7;
//	p0=(pxy>>12)&7;	p1=(pxy>>15)&7;
//	p2=(pxy>>18)&7;	p3=(pxy>>21)&7;
	p0=(p0<<1)|(p0>>2);	p1=(p1<<1)|(p1>>2);
	p2=(p2<<1)|(p2>>2);	p3=(p3<<1)|(p3>>2);
	BTIC4B_BitsLE_Write16Bits(&bits, p0|(p1<<4)|(p2<<8)|(p3<<12));

	p7=p6;
//	p7=pxy>>24;
	p0=(p7   )&7;	p1=(p7>>3)&7;
	p2=(p7>>6)&7;	p3=(p7>>9)&7;
//	p0=(pxy>>24)&7;	p1=(pxy>>27)&7;
//	p2=(pxy>>30)&7;	p3=(pxy>>33)&7;
	p0=(p0<<1)|(p0>>2);	p1=(p1<<1)|(p1>>2);
	p2=(p2<<1)|(p2>>2);	p3=(p3<<1)|(p3>>2);
	BTIC4B_BitsLE_Write16Bits(&bits, p0|(p1<<4)|(p2<<8)|(p3<<12));

	p7=p6>>12;
//	p7=pxy>>36;
	p0=(p7   )&7;	p1=(p7>>3)&7;
	p2=(p7>>6)&7;	p3=(p7>>9)&7;
//	p0=(pxy>>36)&7;	p1=(pxy>>39)&7;
//	p2=(pxy>>42)&7;	p3=(pxy>>45)&7;
	p0=(p0<<1)|(p0>>2);	p1=(p1<<1)|(p1>>2);
	p2=(p2<<1)|(p2>>2);	p3=(p3<<1)|(p3>>2);
	BTIC4B_BitsLE_Write16Bits(&bits, p0|(p1<<4)|(p2<<8)|(p3<<12));

	BTIC4B_BitsLE_FlushBits(&bits);

	memcpy(block, tblock, 16);
}

void BTIC4B_BC7_EncodeBlockBits48_AutoMode(byte *block,
	u64 pxy, u64 pxa, int *min, int *max)
{
	if((max[3]-min[3])<8)
	{
		BTIC4B_BC7_EncodeBlockBits48_Mode6(block, pxy, min, max);
		return;
	}

	if((max[1]-min[1])>32)
	{
		BTIC4B_BC7_EncodeBlockBits48_Mode4(block, pxy, pxa, min, max);
		return;
	}
	
	BTIC4B_BC7_EncodeBlockBits48_Mode5(block, pxy, pxa, min, max);
}

void BTIC4B_BC7_EncodeBlockBits32_Mode5(byte *block,
	u32 pxy, u32 pxa,
	int *min, int *max)
{
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i, j, k, l;

//	BTIC4B_BitsLE_ClearSetupWrite(&bits, block, 16);
	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 32, 6);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);

//	if(((pxy>>30)&3)>1)
//	if((pxy&3)>1)
//	if(pxy&1)
	if(pxy&2)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapRGB(min, max);
		pxy=~pxy;
	}

//	if(((pxa>>30)&3)>1)
//	if((pxa&3)>1)
//	if(pxa&1)
	if(pxa&2)
	{
		BTIC4B_BC7_EncodeBlock_VecSwapA(min, max);
		pxa=~pxa;
	}

	p0=((min[0])>>1)|(((max[0])>>1)<<7);
	p1=((min[1])>>1)|(((max[1])>>1)<<7);
	p2=((min[2])>>1)|(((max[2])>>1)<<7);
	p3=min[3]|(max[3]<<8);

	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write16Bits(&bits, p3);

#if 0
	BTIC4B_BitsLE_Write7Bits(&bits, (min[0])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (max[0])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (min[1])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (max[1])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (min[2])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (max[2])>>1);
	BTIC4B_BitsLE_Write8Bits(&bits, min[3]);
	BTIC4B_BitsLE_Write8Bits(&bits, max[3]);
#endif

	BTIC4B_BitsLE_WriteBit(&bits, pxy);
	BTIC4B_BitsLE_Write6Bits(&bits, (pxy>> 2));
//	BTIC4B_BitsLE_Write8Bits(&bits, (pxy>> 8));
//	BTIC4B_BitsLE_Write16Bits(&bits, (pxy>>16));
	BTIC4B_BitsLE_Write24Bits(&bits, (pxy>> 8));

	BTIC4B_BitsLE_WriteBit(&bits, pxa);
	BTIC4B_BitsLE_Write6Bits(&bits, (pxa>> 2));
//	BTIC4B_BitsLE_Write8Bits(&bits, (pxa>> 8));
//	BTIC4B_BitsLE_Write16Bits(&bits, (pxa>>16));
	BTIC4B_BitsLE_Write24Bits(&bits, (pxa>> 8));

	BTIC4B_BitsLE_FlushBits(&bits);

	memcpy(block, tblock, 16);
}

void BTIC4B_BC7_EncodeBlockFlat_Mode5(byte *block, int *avg)
{
#if 1
	u64 blk;
	int p0, p1, p2, p3, p4, p5, p6, p7;

	p0=((avg[0])>>1)|(((avg[0])>>1)<<7);
	p1=((avg[1])>>1)|(((avg[1])>>1)<<7);
	p2=((avg[2])>>1)|(((avg[2])>>1)<<7);
	p3=avg[3]|(avg[3]<<8);

	p4=p0|(p1<<14);
	p5=p2|(p3<<14);

	blk=32|(((u64)p4)<<8)|(((u64)p5)<<36);
	((u64 *)block)[0]=blk;
	((u64 *)block)[1]=0;
#endif

#if 0
	BTIC4B_BitStream bits;
	byte tblock[24];

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i, j, k, l;

//	BTIC4B_BitsLE_ClearSetupWrite(&bits, block, 16);
	BTIC4B_BitsLE_ClearSetupWrite(&bits, tblock, 16);
	BTIC4B_BitsLE_WriteBits(&bits, 32, 6);
	BTIC4B_BitsLE_WriteBits(&bits, 0, 2);

	p0=((avg[0])>>1)|(((avg[0])>>1)<<7);
	p1=((avg[1])>>1)|(((avg[1])>>1)<<7);
	p2=((avg[2])>>1)|(((avg[2])>>1)<<7);
	p3=avg[3]|(avg[3]<<8);

	BTIC4B_BitsLE_Write14Bits(&bits, p0);
	BTIC4B_BitsLE_Write14Bits(&bits, p1);
	BTIC4B_BitsLE_Write14Bits(&bits, p2);
	BTIC4B_BitsLE_Write16Bits(&bits, p3);

#if 0
	BTIC4B_BitsLE_Write7Bits(&bits, (avg[0])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (avg[0])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (avg[1])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (avg[1])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (avg[2])>>1);
	BTIC4B_BitsLE_Write7Bits(&bits, (avg[2])>>1);
	BTIC4B_BitsLE_Write8Bits(&bits, avg[3]);
	BTIC4B_BitsLE_Write8Bits(&bits, avg[3]);
#endif

	BTIC4B_BitsLE_Write14Bits(&bits, 0);
	BTIC4B_BitsLE_Write16Bits(&bits, 0);
	BTIC4B_BitsLE_Write16Bits(&bits, 0);
	BTIC4B_BitsLE_Write16Bits(&bits, 0);

	BTIC4B_BitsLE_FlushBits(&bits);

	memcpy(block, tblock, 16);
#endif
}

void BTIC4B_BC7_EncodeBlockFlatPx(BTIC4B_Context *ctx,
	byte *block, u32 px)
{
	int avg[4];
	avg[2]=(px    )&255; avg[1]=(px>> 8)&255;
	avg[0]=(px>>16)&255; avg[3]=(px>>24)&255;
//	BTIC4B_BC7_EncodeBlockFlat_Mode5(block, avg);
	ctx->BCnEncodeBlockFlat(block, avg);
}

void BTIC4B_BC7_EncodeBlockBits32Px(BTIC4B_Context *ctx,
	byte *block,
	u32 pxy, u32 pxa, u32 pxmin, u32 pxmax)
{
	int min[4], max[4];
	min[2]=(pxmin    )&255; min[1]=(pxmin>> 8)&255;
	min[0]=(pxmin>>16)&255; min[3]=(pxmin>>24)&255;
	max[2]=(pxmax    )&255; max[1]=(pxmax>> 8)&255;
	max[0]=(pxmax>>16)&255; max[3]=(pxmax>>24)&255;
//	BTIC4B_BC7_EncodeBlockBits32_Mode5(
	ctx->BCnEncodeBlockBits32(
		block, pxy, pxa, min, max);
}

void BTIC4B_BC7_EncodeBlockBits48Px(BTIC4B_Context *ctx,
	byte *block,
	u64 pxy, u64 pxa, u32 pxmin, u32 pxmax)
{
	int min[4], max[4];
	min[2]=(pxmin    )&255; min[1]=(pxmin>> 8)&255;
	min[0]=(pxmin>>16)&255; min[3]=(pxmin>>24)&255;
	max[2]=(pxmax    )&255; max[1]=(pxmax>> 8)&255;
	max[0]=(pxmax>>16)&255; max[3]=(pxmax>>24)&255;
//	BTIC4B_BC7_EncodeBlockBits32_Mode5(
	ctx->BCnEncodeBlockBits48(
		block, pxy, pxa, min, max);
}

void BTIC4B_BC7_EncodeBlockBits64Px(BTIC4B_Context *ctx,
	byte *block, u64 pxy, u32 pxmin, u32 pxmax)
{
	int min[4], max[4];
	min[2]=(pxmin    )&255; min[1]=(pxmin>> 8)&255;
	min[0]=(pxmin>>16)&255; min[3]=(pxmin>>24)&255;
	max[2]=(pxmax    )&255; max[1]=(pxmax>> 8)&255;
	max[0]=(pxmax>>16)&255; max[3]=(pxmax>>24)&255;
//	BTIC4B_BC7_EncodeBlockBits32_Mode5(
	ctx->BCnEncodeBlockBits64(
		block, pxy, min, max);
}

/* Convert MetaBlock to 2x2 BC7 blocks */
void BTIC4B_ConvBlockSpecialBC7_A(
	BTIC4B_Context *ctx, byte *iblock,
	byte *oblock, int obxstr, int obystr, int tfl)
{
	u32 tblk[64];
	s16 yblk[64], ablk[64];
	u32 pxm, pxn, px, px0, px1, px2, px3;
	int min[4], max[4];
	byte *cs, *ct;
	int cy0, cy1, cy2, cy3;
	int ca0, ca1, ca2, ca3;
	int acr, acg, acb, acy;
	int cr, cg, cb, ca, cy;
	int mcy, ncy, mca, nca;
	int x, y;
	int i, j, k, xn;

	if(tfl&1)
		{ ctx->DecBlock(ctx, iblock, (byte *)(tblk+56), -8*4); }
	else
		{ ctx->DecBlock(ctx, iblock, (byte *)tblk, 8*4); }
	
	if(ctx->BCnEncodeBlockBGRA)
	{
		for(y=0; y<2; y++)
			for(x=0; x<2; x++)
		{
			ctx->BCnEncodeBlockBGRA(
				oblock+(y*obystr)+(x*obxstr),
				(byte *)(tblk+y*4*4+x*4), 8*4);
		}
		return;
	}
	
	for(y=0; y<2; y++)
		for(x=0; x<2; x++)
	{
		mcy=256; ncy=-1;
		mca=256; nca=-1;
#if 0
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=(y*4+i)*8+(x*4+j);
			px=tblk[k];
			cy=(byte)(px>>8);
			ca=(byte)(px>>24);

			k=i*4+j;
			yblk[k]=cy;
			ablk[k]=ca;
			if(cy<mcy)	{ mcy=cy; pxm=px; }
			if(cy>ncy)	{ ncy=cy; pxn=px; }
			if(ca<mca)	{ mca=ca; }
			if(ca>nca)	{ nca=ca; }
		}
#endif

#if 1
		acr=0; acg=0; acb=0;
		for(i=0; i<4; i++)
		{
			k=(y*4+i)*8+(x*4);
			px0=tblk[k+0];	px1=tblk[k+1];
			px2=tblk[k+2];	px3=tblk[k+3];
			cy0=(byte)(px0>>8);	cy1=(byte)(px1>>8);
			cy2=(byte)(px2>>8);	cy3=(byte)(px3>>8);

//			cy0=(((byte)(px0>>8))*2+((byte)(px0>>16))+((byte)px0))>>2;
//			cy1=(((byte)(px1>>8))*2+((byte)(px1>>16))+((byte)px1))>>2;
//			cy2=(((byte)(px2>>8))*2+((byte)(px2>>16))+((byte)px2))>>2;
//			cy3=(((byte)(px3>>8))*2+((byte)(px3>>16))+((byte)px3))>>2;

			acr+=((byte)(px0>>16))+((byte)(px3>>16));
			acg+=((byte)(px0>> 8))+((byte)(px3>> 8));
			acb+=((byte)(px0    ))+((byte)(px3    ));

			k=i*4;
			yblk[k+0]=cy0;	yblk[k+1]=cy1;
			yblk[k+2]=cy2;	yblk[k+3]=cy3;
			if(cy0<mcy)	{ mcy=cy0; pxm=px0; }
			if(cy0>ncy)	{ ncy=cy0; pxn=px0; }
			if(cy3<mcy)	{ mcy=cy3; pxm=px3; }
			if(cy3>ncy)	{ ncy=cy3; pxn=px3; }
			if(cy1<mcy)	{ mcy=cy1; pxm=px1; }
			if(cy1>ncy)	{ ncy=cy1; pxn=px1; }
			if(cy2<mcy)	{ mcy=cy2; pxm=px2; }
			if(cy2>ncy)	{ ncy=cy2; pxn=px2; }

			ca0=(byte)(px0>>24);	ca1=(byte)(px1>>24);
			ca2=(byte)(px2>>24);	ca3=(byte)(px3>>24);

//			if(((px0&px1&px2&px3)>>24)!=((px0|px1|px2|px3)>>24))
			if((ca0&ca1&ca2&ca3)!=(ca0|ca1|ca2|ca3))
			{
//				ca0=(byte)(px0>>24);	ca1=(byte)(px1>>24);
//				ca2=(byte)(px2>>24);	ca3=(byte)(px3>>24);
				ablk[k+0]=ca0;	ablk[k+1]=ca1;
				ablk[k+2]=ca2;	ablk[k+3]=ca3;
				if(ca0<mca)	{ mca=ca0; }
				if(ca0>nca)	{ nca=ca0; }
				if(ca3<mca)	{ mca=ca3; }
				if(ca3>nca)	{ nca=ca3; }
				if(ca1<mca)	{ mca=ca1; }
				if(ca1>nca)	{ nca=ca1; }
				if(ca2<mca)	{ mca=ca2; }
				if(ca2>nca)	{ nca=ca2; }
			}else
			{
				ca0=(byte)(px0>>24);
				ablk[k+0]=ca0; ablk[k+1]=ca0;
				ablk[k+2]=ca0; ablk[k+3]=ca0;
				if(ca0<mca)	{ mca=ca0; }
				if(ca0>nca)	{ nca=ca0; }
			}
		}
		acr=acr>>3;
		acg=acg>>3;
		acb=acb>>3;
#endif
		
		min[0]=(byte)(pxm>>16);
		min[1]=(byte)(pxm>> 8);
		min[2]=(byte)(pxm    );
		max[0]=(byte)(pxn>>16);
		max[1]=(byte)(pxn>> 8);
		max[2]=(byte)(pxn    );
		min[3]=mca;	max[3]=nca;

#if 1
//		acy=(2*acg+acr+acb)>>2;
		acy=acg;
		cy0=mcy-acy;
		cy1=ncy-acy;
		min[0]=lqtvq_clamp255((min[0]+3*(acr+cy0))>>2);
		min[1]=lqtvq_clamp255((min[1]+3*(acg+cy0))>>2);
		min[2]=lqtvq_clamp255((min[2]+3*(acb+cy0))>>2);
		max[0]=lqtvq_clamp255((max[0]+3*(acr+cy1))>>2);
		max[1]=lqtvq_clamp255((max[1]+3*(acg+cy1))>>2);
		max[2]=lqtvq_clamp255((max[2]+3*(acb+cy1))>>2);
#endif

//		BTIC4B_BC7_EncodeBlock_Mode5(
//			oblock+(y*obystr)+(x*16),
//			yblk, ablk,
//			min, max, mcy, ncy, mca, nca);
		ctx->BCnEncodeBlockGen(
			oblock+(y*obystr)+(x*obxstr), yblk, ablk,
			min, max, mcy, ncy, mca, nca);
	}
}

/* Convert metablock to a single BC7 block, 1/2 subsample */
void BTIC4B_ConvBlockSpecialBC7_B(
	BTIC4B_Context *ctx, byte *iblock,
	byte *oblock, byte *pblock, int tfl)
{
	u32 tblk[64];
	s16 yblk[64], ablk[64];
	int min[4], max[4];
	u32 px, pxm, pxn;
	u32 px0, px1, px2, px3;
	byte *cs, *ct;
	int cy0, cy1, cy2, cy3;
	int ca0, ca1, ca2, ca3;
	int cr, cg, cb, ca, cy;
	int acr, acg, acb;
	int mcy, ncy, mca, nca, acy, aca, dcy, dca;
	int mcu, ncu, mcv, ncv, acu, acv, dcu, dcv;
	int i, j, k, xn;

	if(tfl&1)
		{ ctx->DecBlockHalf(ctx, iblock, (byte *)(tblk+12), -4*4); }
	else
		{ ctx->DecBlockHalf(ctx, iblock, (byte *)tblk, 4*4); }
	
	mcy=256; ncy=-1;
	mca=256; nca=-1;
	acr=0; acg=0; acb=0;

#if 0
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		k=i*4+j;
		px=tblk[k];
		cy=(byte)(px>>8);
		ca=(byte)(px>>24);
		
		k=i*4+j;
		yblk[k]=cy;
		ablk[k]=ca;
		if(cy<mcy)	{ mcy=cy; pxm=px; }
		if(cy>ncy)	{ ncy=cy; pxn=px; }
		if(ca<mca)	{ mca=ca; }
		if(ca>nca)	{ nca=ca; }
	}
#endif

#if 1
	for(i=0; i<4; i++)
	{
		k=i*4;
		px0=tblk[k+0];	px1=tblk[k+1];
		px2=tblk[k+2];	px3=tblk[k+3];
//		cy0=(byte)(px0>>8);	cy1=(byte)(px1>>8);
//		cy2=(byte)(px2>>8);	cy3=(byte)(px3>>8);

		cy0=(((byte)(px0>>8))*2+((byte)(px0>>16))+((byte)px0))>>2;
		cy1=(((byte)(px1>>8))*2+((byte)(px1>>16))+((byte)px1))>>2;
		cy2=(((byte)(px2>>8))*2+((byte)(px2>>16))+((byte)px2))>>2;
		cy3=(((byte)(px3>>8))*2+((byte)(px3>>16))+((byte)px3))>>2;

//		acr+=(((byte)px0)>>16)+(((byte)px3)>>16);
//		acg+=(((byte)px0)>> 8)+(((byte)px3)>> 8);
//		acb+=(((byte)px0)    )+(((byte)px3)    );

		acr+=((byte)(px0>>16))+((byte)(px3>>16));
		acg+=((byte)(px0>> 8))+((byte)(px3>> 8));
		acb+=((byte)(px0    ))+((byte)(px3    ));

		yblk[k+0]=cy0;	yblk[k+1]=cy1;
		yblk[k+2]=cy2;	yblk[k+3]=cy3;
		if(cy0<mcy)	{ mcy=cy0; pxm=px0; }
		if(cy0>ncy)	{ ncy=cy0; pxn=px0; }
		if(cy3<mcy)	{ mcy=cy3; pxm=px3; }
		if(cy3>ncy)	{ ncy=cy3; pxn=px3; }
		if(cy1<mcy)	{ mcy=cy1; pxm=px1; }
		if(cy1>ncy)	{ ncy=cy1; pxn=px1; }
		if(cy2<mcy)	{ mcy=cy2; pxm=px2; }
		if(cy2>ncy)	{ ncy=cy2; pxn=px2; }

		ca0=(byte)(px0>>24);	ca1=(byte)(px1>>24);
		ca2=(byte)(px2>>24);	ca3=(byte)(px3>>24);
//		if(((px0&px1&px2&px3)>>24)!=((px0|px1|px2|px3)>>24))
		if((ca0&ca1&ca2&ca3)!=(ca0|ca1|ca2|ca3))
		{
//			ca0=(byte)(px0>>24);	ca1=(byte)(px1>>24);
//			ca2=(byte)(px2>>24);	ca3=(byte)(px3>>24);
			ablk[k+0]=ca0;	ablk[k+1]=ca1;
			ablk[k+2]=ca2;	ablk[k+3]=ca3;
			if(ca0<mca)	{ mca=ca0; }
			if(ca0>nca)	{ nca=ca0; }
			if(ca3<mca)	{ mca=ca3; }
			if(ca3>nca)	{ nca=ca3; }
			if(ca1<mca)	{ mca=ca1; }
			if(ca1>nca)	{ nca=ca1; }
			if(ca2<mca)	{ mca=ca2; }
			if(ca2>nca)	{ nca=ca2; }
		}else
		{
//			ca0=(byte)(px0>>24);
			ablk[k+0]=ca0; ablk[k+1]=ca0;
			ablk[k+2]=ca0; ablk[k+3]=ca0;
			if(ca0<mca)	{ mca=ca0; }
			if(ca0>nca)	{ nca=ca0; }
		}
	}
#endif


	min[0]=(byte)(pxm>>16);
	min[1]=(byte)(pxm>> 8);
	min[2]=(byte)(pxm    );
	max[0]=(byte)(pxn>>16);
	max[1]=(byte)(pxn>> 8);
	max[2]=(byte)(pxn    );

	acr=acr>>3;
	acg=acg>>3;
	acb=acb>>3;

//	min[0]=(3*min[0]+acr)>>2;
//	min[1]=(3*min[1]+acg)>>2;
//	min[2]=(3*min[2]+acb)>>2;
//	max[0]=(3*max[0]+acr)>>2;
//	max[1]=(3*max[1]+acg)>>2;
//	max[2]=(3*max[2]+acb)>>2;

	acy=(2*acg+acr+acb)>>2;
	cy0=mcy-acy;
	cy1=ncy-acy;
	min[0]=lqtvq_clamp255((min[0]+3*(acr+cy0))>>2);
	min[1]=lqtvq_clamp255((min[1]+3*(acg+cy0))>>2);
	min[2]=lqtvq_clamp255((min[2]+3*(acb+cy0))>>2);
	max[0]=lqtvq_clamp255((max[0]+3*(acr+cy1))>>2);
	max[1]=lqtvq_clamp255((max[1]+3*(acg+cy1))>>2);
	max[2]=lqtvq_clamp255((max[2]+3*(acb+cy1))>>2);

	if(pblock)
	{
		acy=(mcy+ncy)>>1;
		aca=(mca+nca)>>1;
		dcy=ncy-mcy;
		dca=nca-mca;
		
		mcu=((min[2]-min[1])>>1)+128;
		mcv=((min[0]-min[1])>>1)+128;
		ncu=((max[2]-max[1])>>1)+128;
		ncv=((max[0]-max[1])>>1)+128;
//		acu=(mcu+ncu)>>1;
//		acv=(mcv+ncv)>>1;
		dcu=ncu-mcu;
		dcv=ncv-mcv;

		acu=(mcu+3*ncu)>>2;
		acv=(mcv+3*ncv)>>2;

		pblock[ 0]=acy;	pblock[ 1]=dcy;
		pblock[ 2]=aca;	pblock[ 3]=dca;
		pblock[ 4]=acu;	pblock[ 5]=acv;
		pblock[ 6]=dcu;	pblock[ 7]=dcv;
		pblock[ 8]=(yblk[ 0]+yblk[ 1]+yblk[ 4]+yblk[ 5])>>2;
		pblock[ 9]=(yblk[ 2]+yblk[ 3]+yblk[ 6]+yblk[ 7])>>2;
		pblock[10]=(yblk[ 8]+yblk[ 9]+yblk[12]+yblk[13])>>2;
		pblock[11]=(yblk[10]+yblk[11]+yblk[14]+yblk[15])>>2;
		pblock[12]=(ablk[ 0]+ablk[ 1]+ablk[ 4]+ablk[ 5])>>2;
		pblock[13]=(ablk[ 2]+ablk[ 3]+ablk[ 6]+ablk[ 7])>>2;
		pblock[14]=(ablk[ 8]+ablk[ 9]+ablk[12]+ablk[13])>>2;
		pblock[15]=(ablk[10]+ablk[11]+ablk[14]+ablk[15])>>2;
	}

	min[3]=mca;	max[3]=nca;
//	BTIC4B_BC7_EncodeBlock_Mode5(oblock, yblk, ablk,
//		min, max, mcy, ncy, mca, nca);
	ctx->BCnEncodeBlockGen(oblock, yblk, ablk,
		min, max, mcy, ncy, mca, nca);
}

static u32 btic4b_bcn_4x2x1to4x4x2(int v)
{
	int p0, p1, p2, p3;
	p0=btic4b_bc7_4x1x1to4x1x2[v&15];
	p1=btic4b_bc7_4x1x1to4x1x2[(v>>4)&15];
	p2=p0|(p0<<8)|(p1<<16)|(p1<<24);
	return(p2);
}

static u32 btic4b_bcn_2x4x1to4x4x2(int v)
{
	static const u32 pbtab_2x1x1[4]={
		0x00, 0x0F, 0xF0, 0xFF};
	int p0, p1, p2, p3;
	p0=pbtab_2x1x1[(v   )&3];
	p1=pbtab_2x1x1[(v>>2)&3];
	p2=pbtab_2x1x1[(v>>4)&3];
	p3=pbtab_2x1x1[(v>>6)&3];
	p2=p0|(p1<<8)|(p2<<16)|(p3<<24);
	return(p2);
}

static u32 btic4b_bcn_4x4x1to4x4x2(int v)
{
	int p0, p1, p2, p3;
	p0=btic4b_bc7_4x1x1to4x1x2[(v    )&15];
	p1=btic4b_bc7_4x1x1to4x1x2[(v>> 4)&15];
	p2=btic4b_bc7_4x1x1to4x1x2[(v>> 8)&15];
	p3=btic4b_bc7_4x1x1to4x1x2[(v>>12)&15];
	p2=p0|(p0<<8)|(p1<<16)|(p1<<24);
	return(p2);
}

static byte btic4b_bcn_4x4x2to2x2x2(u32 px)
{
	u32 px1;
	int p0, p1, p2, p3;

	p0=(((px    )&3)+((px>> 2)&3)+((px>> 8)&3)+((px>>10)&3))>>2;
	p1=(((px>> 4)&3)+((px>> 6)&3)+((px>>12)&3)+((px>>14)&3))>>2;
	p2=(((px>>16)&3)+((px>>18)&3)+((px>>24)&3)+((px>>26)&3))>>2;
	p3=(((px>>20)&3)+((px>>22)&3)+((px>>28)&3)+((px>>30)&3))>>2;
	px1=p0|(p1<<2)|(p2<<4)|(p3<<6);
	return(px1);
}

static byte btic4b_bcn_4x8x2to2x2x2(u32 pxa, u32 pxb)
{
	u32 px1;
	int p0, p1, p2, p3, p4, p5, p6, p7;

	p0=(((pxa    )&3)+((pxa>> 2)&3)+((pxa>> 8)&3)+((pxa>>10)&3))>>2;
	p1=(((pxa>> 4)&3)+((pxa>> 6)&3)+((pxa>>12)&3)+((pxa>>14)&3))>>2;
	p2=(((pxa>>16)&3)+((pxa>>18)&3)+((pxa>>24)&3)+((pxa>>26)&3))>>2;
	p3=(((pxa>>20)&3)+((pxa>>22)&3)+((pxa>>28)&3)+((pxa>>30)&3))>>2;
	p4=(((pxb    )&3)+((pxb>> 2)&3)+((pxb>> 8)&3)+((pxb>>10)&3))>>2;
	p5=(((pxb>> 4)&3)+((pxb>> 6)&3)+((pxb>>12)&3)+((pxb>>14)&3))>>2;
	p6=(((pxb>>16)&3)+((pxb>>18)&3)+((pxb>>24)&3)+((pxb>>26)&3))>>2;
	p7=(((pxb>>20)&3)+((pxb>>22)&3)+((pxb>>28)&3)+((pxb>>30)&3))>>2;
	p0=(p0+p1)>>1;	p1=(p2+p3)>>1;
	p2=(p4+p5)>>1;	p3=(p6+p7)>>1;
	px1=p0|(p1<<2)|(p2<<4)|(p3<<6);
	return(px1);
}

static byte btic4b_bcn_4x8x3to2x2x2(u32 pxa, u32 pxb, u32 pxc, u32 pxd)
{
	u32 px1;
	int p0, p1, p2, p3, p4, p5, p6, p7;
	p0=(((pxa    )&7)+((pxa>> 3)&7)+((pxa>>12)&7)+((pxa>>15)&7))>>2;
	p1=(((pxa>> 6)&7)+((pxa>> 9)&7)+((pxa>>18)&7)+((pxa>>21)&7))>>2;
	p2=(((pxb    )&7)+((pxb>> 3)&7)+((pxb>>12)&7)+((pxb>>15)&7))>>2;
	p3=(((pxb>> 6)&7)+((pxb>> 9)&7)+((pxb>>18)&7)+((pxb>>21)&7))>>2;
	p4=(((pxc    )&7)+((pxc>> 3)&7)+((pxc>>12)&7)+((pxc>>15)&7))>>2;
	p5=(((pxc>> 6)&7)+((pxc>> 9)&7)+((pxc>>18)&7)+((pxc>>21)&7))>>2;
	p6=(((pxd    )&7)+((pxd>> 3)&7)+((pxd>>12)&7)+((pxd>>15)&7))>>2;
	p7=(((pxd>> 6)&7)+((pxd>> 9)&7)+((pxd>>18)&7)+((pxd>>21)&7))>>2;
	p0=(p0+p1)>>2;	p1=(p2+p3)>>2;
	p2=(p4+p5)>>2;	p3=(p6+p7)>>2;
	px1=p0|(p1<<2)|(p2<<4)|(p3<<6);
	return(px1);
}

void BTIC4B_DecBlockTab1b(BTIC4B_Context *ctx,
	byte *blkbuf, u32 *tab)
{
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int ca;

	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	ca=blkbuf[2];

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	tab[0]=ctx->ClrDec1A(mcy, acu, acv, ca);
	tab[1]=ctx->ClrDec1A(ncy, acu, acv, ca);
}

void BTIC4B_DecBlockTab2b(BTIC4B_Context *ctx,
	byte *blkbuf, u32 *tab)
{
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int ca;

	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	ca=blkbuf[2];
	
	ctx->ClrDec2T(blkbuf[0],
		acy, acu, acv, dcy, dcu, dcv,
		&cr0, &cg0, &cb0, &cr3, &cg3, &cb3);

	cr1=(11*cr0+ 5*cr3)>>4;
	cg1=(11*cg0+ 5*cg3)>>4;
	cb1=(11*cb0+ 5*cb3)>>4;
	cr2=( 5*cr0+11*cr3)>>4;
	cg2=( 5*cg0+11*cg3)>>4;
	cb2=( 5*cb0+11*cb3)>>4;

	tab[0]=(ca<<24)|(cr0<<16)|(cg0<<8)|cb0;
	tab[1]=(ca<<24)|(cr1<<16)|(cg1<<8)|cb1;
	tab[2]=(ca<<24)|(cr2<<16)|(cg2<<8)|cb2;
	tab[3]=(ca<<24)|(cr3<<16)|(cg3<<8)|cb3;
}

void BTIC4B_DecBlockTab1b4(BTIC4B_Context *ctx,
	byte *blkbuf, u32 *tab, byte pxu, byte pxv)
{
	int utab[4], vtab[4];
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int ca;

	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
	ca=blkbuf[2];

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	utab[0]=mcu;
	utab[1]=(11*mcu+ 5*ncu)>>4;
	utab[2]=( 5*mcu+11*ncu)>>4;
	utab[3]=ncu;

	vtab[0]=mcv;
	vtab[1]=(11*mcv+ 5*ncv)>>4;
	vtab[2]=( 5*mcv+11*ncv)>>4;
	vtab[3]=ncv;

	tab[0]=ctx->ClrDec1A(mcy, utab[(pxu   )&3], vtab[(pxv   )&3], ca);
	tab[1]=ctx->ClrDec1A(ncy, utab[(pxu   )&3], vtab[(pxv   )&3], ca);
	tab[2]=ctx->ClrDec1A(mcy, utab[(pxu>>2)&3], vtab[(pxv>>2)&3], ca);
	tab[3]=ctx->ClrDec1A(ncy, utab[(pxu>>2)&3], vtab[(pxv>>2)&3], ca);
	tab[4]=ctx->ClrDec1A(mcy, utab[(pxu>>4)&3], vtab[(pxv>>4)&3], ca);
	tab[5]=ctx->ClrDec1A(ncy, utab[(pxu>>4)&3], vtab[(pxv>>4)&3], ca);
	tab[6]=ctx->ClrDec1A(mcy, utab[(pxu>>6)&3], vtab[(pxv>>6)&3], ca);
	tab[7]=ctx->ClrDec1A(ncy, utab[(pxu>>6)&3], vtab[(pxv>>6)&3], ca);
}

void BTIC4B_DecBlockYuvTab2b(BTIC4B_Context *ctx,
	byte *blkbuf, int *ytab, int *utab, int *vtab)
{
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int ca;

	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
//	ca=blkbuf[2];

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(11*mcy+ 5*ncy)>>4;
	ytab[2]=( 5*mcy+11*ncy)>>4;
	ytab[3]=ncy;
	
	utab[0]=mcu;
	utab[1]=(11*mcu+ 5*ncu)>>4;
	utab[2]=( 5*mcu+11*ncu)>>4;
	utab[3]=ncu;

	vtab[0]=mcv;
	vtab[1]=(11*mcv+ 5*ncv)>>4;
	vtab[2]=( 5*mcv+11*ncv)>>4;
	vtab[3]=ncv;
}

void BTIC4B_DecBlockYuvTab2b1b(BTIC4B_Context *ctx,
	byte *blkbuf, int *ytab, int *utab, int *vtab)
{
	int acy, acu, acv, dcy, dcu, dcv;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int ca;

	acy=*(s16 *)(blkbuf+ 4);
	acu=*(s16 *)(blkbuf+ 6);
	acv=*(s16 *)(blkbuf+ 8);
	dcy=*(s16 *)(blkbuf+10);
	dcu=*(s16 *)(blkbuf+12);
	dcv=*(s16 *)(blkbuf+14);
//	ca=blkbuf[2];

	mcy=acy-(dcy>>1); ncy=mcy+dcy;
	mcu=acu-(dcu>>1); ncu=mcu+dcu;
	mcv=acv-(dcv>>1); ncv=mcv+dcv;

	ytab[0]=mcy;
	ytab[1]=(11*mcy+ 5*ncy)>>4;
	ytab[2]=( 5*mcy+11*ncy)>>4;
	ytab[3]=ncy;
	utab[0]=mcu;	utab[1]=ncu;
	vtab[0]=mcv;	vtab[1]=ncv;
}

void BTIC4B_ConvBlockBC7_A(
	BTIC4B_Context *ctx, byte *iblock,
	byte *oblock, int obxstr, int obystr, int tfl)
{
	static const u32 pbtab_2x2x1[16]={
		0x00000000, 0x00000F0F, 0x0000F0F0, 0x0000FFFF,
		0x0F0F0000, 0x0F0F0F0F, 0x0F0FF0F0, 0x0F0FFFFF,
		0xF0F00000, 0xF0F00F0F, 0xF0F0F0F0, 0xF0F0FFFF,
		0xFFFF0000, 0xFFFF0F0F, 0xFFFFF0F0, 0xFFFFFFFF};
	static const u32 pbtab_1x2x1[4]={
		0x00000000, 0x0000FFFF, 0xFFFF0000, 0xFFFFFFFF};
	static const byte pbtab_2x1x2to4x1x2[16]={
		0x00, 0x05, 0x0A, 0x0F,	0x50, 0x55, 0x5A, 0x5F,
		0xA0, 0xA5, 0xAA, 0xAF,	0xF0, 0xF5, 0xFA, 0xFF};
	static const u32 pbtab_2x1x2to4x4x2[16]={
		0x00000000, 0x05050505, 0x0A0A0A0A, 0x0F0F0F0F,
		0x50505050, 0x55555555, 0x5A5A5A5A, 0x5F5F5F5F,
		0xA0A0A0A0, 0xA5A5A5A5, 0xAAAAAAAA, 0xAFAFAFAF,
		0xF0F0F0F0, 0xF5F5F5F5, 0xFAFAFAFA, 0xFFFFFFFF};
	static const u32 pbtab_1x2x2to4x4x2[16]={
		0x00000000, 0x00005555, 0x0000AAAA, 0x0000FFFF,
		0x55550000, 0x55555555, 0x5555AAAA, 0x5555FFFF,
		0xAAAA0000, 0xAAAA5555, 0xAAAAAAAA, 0xAAAAFFFF,
		0xFFFF0000, 0xFFFF5555, 0xFFFFAAAA, 0xFFFFFFFF};

	byte *ct0, *ct1, *ct2, *ct3;
	u32 pxtab[16];
	int avg[4], ytab[4], utab[4], vtab[4];
	int p0, p1, p2, p3, p4, p5, p6, p7;
	u32 pxy0, pxy1, pxy2, pxy3;
	u32 px, pxy, pxu, pxv;
	u64 li;
	int ca;
	int i, j, k;

	if(!(*(u16 *)iblock))
	{
		px=ctx->ClrDec1A(*(s16 *)(iblock+4),
			*(s16 *)(iblock+6), *(s16 *)(iblock+8),
			iblock[2]);
		avg[2]=(px    )&255;	avg[1]=(px>> 8)&255;
		avg[0]=(px>>16)&255;	avg[3]=(px>>24)&255;
//		BTIC4B_BC7_EncodeBlockFlat_Mode5(oblock   , avg);
//		BTIC4B_BC7_EncodeBlockFlat_Mode5(oblock+16, avg);
//		BTIC4B_BC7_EncodeBlockFlat_Mode5(oblock+obystr   , avg);
//		BTIC4B_BC7_EncodeBlockFlat_Mode5(oblock+obystr+16, avg);

		ctx->BCnEncodeBlockFlat(oblock   , avg);
		ctx->BCnEncodeBlockFlat(oblock+obxstr, avg);
		ctx->BCnEncodeBlockFlat(oblock+obystr   , avg);
		ctx->BCnEncodeBlockFlat(oblock+obystr+obxstr, avg);
		return;
	}
	
	if(iblock[1]&0x1F)
	{
		BTIC4B_ConvBlockSpecialBC7_A(ctx, iblock, oblock,
			obxstr, obystr, tfl);
		return;
	}

	if(!(ctx->BCnEncodeBlockBits32))
	{
		BTIC4B_ConvBlockSpecialBC7_A(ctx, iblock, oblock,
			obxstr, obystr, tfl);
		return;
	}

	ct0=oblock;
	ct1=oblock+obxstr;
	ct2=oblock+obystr;
	ct3=oblock+obystr+obxstr;
	switch(iblock[0]&0x1F)
	{
	case 0x01:
		pxy=iblock[16];
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct0, pxtab[(pxy   )&1]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct1, pxtab[(pxy>>1)&1]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct2, pxtab[(pxy>>2)&1]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct3, pxtab[(pxy>>3)&1]);
		break;
	case 0x02:
		pxy=*(byte *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=((pxy   )&0x3)|((pxy<< 2)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>2)&0x3)|((pxy    )&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>4)&0x3)|((pxy>> 2)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>6)&0x3)|((pxy>> 4)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		break;
	case 0x03:
		pxy=*(byte *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=((pxy>>0)&1)|((pxy>>1)&2);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			pbtab_1x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>1)&1)|((pxy>>2)&2);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			pbtab_1x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>4)&1)|((pxy>>5)&2);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			pbtab_1x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>5)&1)|((pxy>>6)&2);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			pbtab_1x2x1[i], 0, pxtab[0], pxtab[1]);
		break;
	case 0x04:
		pxy=*(u16 *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=((pxy    )&0x3)|((pxy>> 2)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 2)&0x3)|((pxy>> 4)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 8)&0x3)|((pxy>>10)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>10)&0x3)|((pxy>>12)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			pbtab_2x2x1[i], 0, pxtab[0], pxtab[1]);
		break;
	case 0x05:
		pxy=*(u32 *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=((pxy    )&0x0F)|((pxy>> 4)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			btic4b_bcn_4x2x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=((pxy>> 4)&0x0F)|((pxy>> 8)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			btic4b_bcn_4x2x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=((pxy>>16)&0x0F)|((pxy>>20)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			btic4b_bcn_4x2x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=((pxy>>20)&0x0F)|((pxy>>24)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			btic4b_bcn_4x2x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		break;
	case 0x06:
		pxy=*(u32 *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=	((pxy    )&0x03)|((pxy>> 2)&0x0C)|
			((pxy>> 4)&0x30)|((pxy>> 6)&0xC0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			btic4b_bcn_2x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=	((pxy>> 2)&0x03)|((pxy>> 4)&0x0C)|
			((pxy>> 6)&0x30)|((pxy>> 8)&0xC0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			btic4b_bcn_2x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=	((pxy>>16)&0x03)|((pxy>>18)&0x0C)|
			((pxy>>20)&0x30)|((pxy>>22)&0xC0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			btic4b_bcn_2x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=	((pxy>>18)&0x03)|((pxy>>20)&0x0C)|
			((pxy>>22)&0x30)|((pxy>>24)&0xC0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			btic4b_bcn_2x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		break;
	case 0x07:
		pxy0=*(u32 *)(iblock+16);
		pxy1=*(u32 *)(iblock+20);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=	((pxy0    )&0x000F)|((pxy0>> 4)&0x00F0)|
			((pxy0>> 8)&0x0F00)|((pxy0>>12)&0xF000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			btic4b_bcn_4x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=	((pxy0>> 4)&0x000F)|((pxy0>> 8)&0x00F0)|
			((pxy0>>12)&0x0F00)|((pxy0>>16)&0xF000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			btic4b_bcn_4x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=	((pxy1    )&0x000F)|((pxy1>> 4)&0x00F0)|
			((pxy1>> 8)&0x0F00)|((pxy1>>12)&0xF000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			btic4b_bcn_4x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		i=	((pxy1>> 4)&0x000F)|((pxy1>> 8)&0x00F0)|
			((pxy1>>12)&0x0F00)|((pxy1>>16)&0xF000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			btic4b_bcn_4x4x1to4x4x2(i), 0, pxtab[0], pxtab[1]);
		break;

	case 0x09:
		pxy=iblock[16];
		BTIC4B_DecBlockTab2b(ctx, iblock, pxtab);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct0, pxtab[(pxy   )&3]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct1, pxtab[(pxy>>2)&3]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct2, pxtab[(pxy>>4)&3]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct3, pxtab[(pxy>>6)&3]);
		break;

#if 1
	case 0x0A:
		pxy=*(u16 *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			pbtab_2x1x2to4x4x2[(pxy    )&15], 0, pxtab[0], pxtab[1]);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			pbtab_2x1x2to4x4x2[(pxy>> 4)&15], 0, pxtab[0], pxtab[1]);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			pbtab_2x1x2to4x4x2[(pxy>> 8)&15], 0, pxtab[0], pxtab[1]);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			pbtab_2x1x2to4x4x2[(pxy>>12)&15], 0, pxtab[0], pxtab[1]);
		break;
	case 0x0B:
		pxy=*(u16 *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=((pxy    )&0x3)|((pxy>> 2)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			pbtab_1x2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 2)&0x3)|((pxy>> 4)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			pbtab_1x2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 8)&0x3)|((pxy>>10)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			pbtab_1x2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>10)&0x3)|((pxy>>12)&0xC);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			pbtab_1x2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		break;
#endif
	case 0x0C:
		pxy=*(u32 *)(iblock+16);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		i=((pxy    )&0x0F)|((pxy>> 4)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 4)&0x0F)|((pxy>> 8)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>16)&0x0F)|((pxy>>20)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>>20)&0x0F)|((pxy>>24)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		break;
#if 1
	case 0x0D:
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		px=	((pxy0   )&0x000000FF)|((pxy0<<8)&0x0000FF00)|
			((pxy0   )&0x00FF0000)|((pxy0<<8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy0>>8)&0x000000FF)|((pxy0   )&0x0000FF00)|
			((pxy0>>8)&0x00FF0000)|((pxy0   )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy1   )&0x000000FF)|((pxy1<<8)&0x0000FF00)|
			((pxy1   )&0x00FF0000)|((pxy1<<8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy1>>8)&0x000000FF)|((pxy1   )&0x0000FF00)|
			((pxy1>>8)&0x00FF0000)|((pxy1   )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[0], pxtab[1]);
		break;
	case 0x0E:
		pxy0=*(u32 *)(iblock+16);
		pxy1=*(u32 *)(iblock+20);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		px=	(pbtab_2x1x2to4x1x2[(pxy0    )&15]    )|
			(pbtab_2x1x2to4x1x2[(pxy0>> 8)&15]<< 8)|
			(pbtab_2x1x2to4x1x2[(pxy0>>16)&15]<<16)|
			(pbtab_2x1x2to4x1x2[(pxy0>>24)&15]<<24);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	(pbtab_2x1x2to4x1x2[(pxy0>> 4)&15]    )|
			(pbtab_2x1x2to4x1x2[(pxy0>>12)&15]<< 8)|
			(pbtab_2x1x2to4x1x2[(pxy0>>20)&15]<<16)|
			(pbtab_2x1x2to4x1x2[(pxy0>>28)&15]<<24);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[0], pxtab[1]);
		px=	(pbtab_2x1x2to4x1x2[(pxy1    )&15]    )|
			(pbtab_2x1x2to4x1x2[(pxy1>> 8)&15]<< 8)|
			(pbtab_2x1x2to4x1x2[(pxy1>>16)&15]<<16)|
			(pbtab_2x1x2to4x1x2[(pxy1>>24)&15]<<24);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[0], pxtab[1]);
		px=	(pbtab_2x1x2to4x1x2[(pxy1>> 4)&15]    )|
			(pbtab_2x1x2to4x1x2[(pxy1>>12)&15]<< 8)|
			(pbtab_2x1x2to4x1x2[(pxy1>>20)&15]<<16)|
			(pbtab_2x1x2to4x1x2[(pxy1>>28)&15]<<24);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[0], pxtab[1]);
		break;
#endif
	case 0x0F:
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		pxy2=*(u32 *)(iblock+24);	pxy3=*(u32 *)(iblock+28);
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		px=	((pxy0    )&0x000000FF)|((pxy0>> 8)&0x0000FF00)|
			((pxy1<<16)&0x00FF0000)|((pxy1<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy0>> 8)&0x000000FF)|((pxy0>>16)&0x0000FF00)|
			((pxy1<< 8)&0x00FF0000)|((pxy1    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy2    )&0x000000FF)|((pxy2>> 8)&0x0000FF00)|
			((pxy3<<16)&0x00FF0000)|((pxy3<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy2>> 8)&0x000000FF)|((pxy2>>16)&0x0000FF00)|
			((pxy3<< 8)&0x00FF0000)|((pxy3    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[0], pxtab[1]);
		break;
	case 0x10:
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+19);
		pxy2=*(u32 *)(iblock+22);	pxy3=*(u32 *)(iblock+25);
		li=	((((u64)pxy0)    )&0x000000000FFF)|
			((((u64)pxy1)<<12)&0x000000FFF000)|
			((((u64)pxy2)<<24)&0x000FFF000000)|
			((((u64)pxy3)<<36)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct0,
			li, 0, pxtab[0], pxtab[1]);
		li=	((((u64)pxy0)>>12)&0x000000000FFF)|
			((((u64)pxy1)    )&0x000000FFF000)|
			((((u64)pxy2)<<12)&0x000FFF000000)|
			((((u64)pxy3)<<24)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct1,
			li, 0, pxtab[0], pxtab[1]);
		pxy0=*(u32 *)(iblock+28);	pxy1=*(u32 *)(iblock+31);
		pxy2=*(u32 *)(iblock+34);	pxy3=*(u32 *)(iblock+37);
		li=	((((u64)pxy0)    )&0x000000000FFF)|
			((((u64)pxy1)<<12)&0x000000FFF000)|
			((((u64)pxy2)<<24)&0x000FFF000000)|
			((((u64)pxy3)<<36)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct2,
			li, 0, pxtab[0], pxtab[1]);
		li=	((((u64)pxy0)>>12)&0x000000000FFF)|
			((((u64)pxy1)    )&0x000000FFF000)|
			((((u64)pxy2)<<12)&0x000FFF000000)|
			((((u64)pxy3)<<24)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct3,
			li, 0, pxtab[0], pxtab[1]);
		break;
	case 0x11:
		BTIC4B_DecBlockTab1b(ctx, iblock, pxtab);
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		pxy2=*(u32 *)(iblock+24);	pxy3=*(u32 *)(iblock+28);
		li=	((((u64)pxy0)    )&0x000000000000FFFF)|
			((((u64)pxy1)<<16)&0x00000000FFFF0000)|
			((((u64)pxy2)<<32)&0x0000FFFF00000000)|
			((((u64)pxy3)<<48)&0xFFFF000000000000);
		BTIC4B_BC7_EncodeBlockBits64Px(ctx, ct0,
			li, pxtab[0], pxtab[1]);
		li=	((((u64)pxy0)>>16)&0x000000000000FFFF)|
			((((u64)pxy1)    )&0x00000000FFFF0000)|
			((((u64)pxy2)<<16)&0x0000FFFF00000000)|
			((((u64)pxy3)<<32)&0xFFFF000000000000);
		BTIC4B_BC7_EncodeBlockBits64Px(ctx, ct1,
			li, pxtab[0], pxtab[1]);
		pxy0=*(u32 *)(iblock+32);	pxy1=*(u32 *)(iblock+36);
		pxy2=*(u32 *)(iblock+40);	pxy3=*(u32 *)(iblock+44);
		li=	((((u64)pxy0)    )&0x000000000000FFFF)|
			((((u64)pxy1)<<16)&0x00000000FFFF0000)|
			((((u64)pxy2)<<32)&0x0000FFFF00000000)|
			((((u64)pxy3)<<48)&0xFFFF000000000000);
		BTIC4B_BC7_EncodeBlockBits64Px(ctx, ct2,
			li, pxtab[0], pxtab[1]);
		li=	((((u64)pxy0)>>16)&0x000000000000FFFF)|
			((((u64)pxy1)    )&0x00000000FFFF0000)|
			((((u64)pxy2)<<16)&0x0000FFFF00000000)|
			((((u64)pxy3)<<32)&0xFFFF000000000000);
		BTIC4B_BC7_EncodeBlockBits64Px(ctx, ct3,
			li, pxtab[0], pxtab[1]);
		break;

#if 1
	case 0x14:
		pxy=iblock[16]; i=iblock[17]; ca=iblock[2];
		pxu=i&15; pxv=(i>>4);
//		BTIC4B_DecBlockTab2b(ctx, iblock, pxtab);
		BTIC4B_DecBlockYuvTab2b1b(ctx, iblock, ytab, utab, vtab);
		pxtab[0]=ctx->ClrDec1A(ytab[(pxy   )&3],
			utab[(pxu   )&1], vtab[(pxv   )&1], ca);
		pxtab[1]=ctx->ClrDec1A(ytab[(pxy>>2)&3],
			utab[(pxu>>1)&1], vtab[(pxv>>1)&1], ca);
		pxtab[2]=ctx->ClrDec1A(ytab[(pxy>>4)&3],
			utab[(pxu>>2)&1], vtab[(pxv>>2)&1], ca);
		pxtab[3]=ctx->ClrDec1A(ytab[(pxy>>6)&3],
			utab[(pxu>>3)&1], vtab[(pxv>>3)&1], ca);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct0, pxtab[0]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct1, pxtab[1]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct2, pxtab[2]);
		BTIC4B_BC7_EncodeBlockFlatPx(ctx, ct3, pxtab[3]);
		break;
	case 0x15:
		pxy=*(u32 *)(iblock+16);
		pxu=iblock[20];		pxv=iblock[21];
		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		i=((pxy    )&0x0F)|((pxy>> 4)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 4)&0x0F)|((pxy>> 8)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[2], pxtab[3]);
		i=((pxy>>16)&0x0F)|((pxy>>20)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[4], pxtab[5]);
		i=((pxy>>20)&0x0F)|((pxy>>24)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x16:
		pxy=*(u32 *)(iblock+16);
		pxu=*(u32 *)(iblock+20);
		pxv=*(u32 *)(iblock+24);
		pxu=btic4b_bcn_4x4x2to2x2x2(pxu);
		pxv=btic4b_bcn_4x4x2to2x2x2(pxv);
		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		i=((pxy    )&0x0F)|((pxy>> 4)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[0], pxtab[1]);
		i=((pxy>> 4)&0x0F)|((pxy>> 8)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[2], pxtab[3]);
		i=((pxy>>16)&0x0F)|((pxy>>20)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[4], pxtab[5]);
		i=((pxy>>20)&0x0F)|((pxy>>24)&0xF0);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			btic4b_bc7_2x2to4x4x2[i], 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x17:
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		pxy2=*(u32 *)(iblock+24);	pxy3=*(u32 *)(iblock+28);
		pxu=iblock[32];		pxv=iblock[33];
		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		px=	((pxy0    )&0x000000FF)|((pxy0>> 8)&0x0000FF00)|
			((pxy1<<16)&0x00FF0000)|((pxy1<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy0>> 8)&0x000000FF)|((pxy0>>16)&0x0000FF00)|
			((pxy1<< 8)&0x00FF0000)|((pxy1    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[2], pxtab[3]);
		px=	((pxy2    )&0x000000FF)|((pxy2>> 8)&0x0000FF00)|
			((pxy3<<16)&0x00FF0000)|((pxy3<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[4], pxtab[5]);
		px=	((pxy2>> 8)&0x000000FF)|((pxy2>>16)&0x0000FF00)|
			((pxy3<< 8)&0x00FF0000)|((pxy3    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x18:
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		pxy2=*(u32 *)(iblock+24);	pxy3=*(u32 *)(iblock+28);
		pxu=*(u32 *)(iblock+32);	pxv=*(u32 *)(iblock+36);
		pxu=btic4b_bcn_4x4x2to2x2x2(pxu);
		pxv=btic4b_bcn_4x4x2to2x2x2(pxv);
		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		px=	((pxy0    )&0x000000FF)|((pxy0>> 8)&0x0000FF00)|
			((pxy1<<16)&0x00FF0000)|((pxy1<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy0>> 8)&0x000000FF)|((pxy0>>16)&0x0000FF00)|
			((pxy1<< 8)&0x00FF0000)|((pxy1    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[2], pxtab[3]);
		px=	((pxy2    )&0x000000FF)|((pxy2>> 8)&0x0000FF00)|
			((pxy3<<16)&0x00FF0000)|((pxy3<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[4], pxtab[5]);
		px=	((pxy2>> 8)&0x000000FF)|((pxy2>>16)&0x0000FF00)|
			((pxy3<< 8)&0x00FF0000)|((pxy3    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x1A:
		pxy0=*(u32 *)(iblock+32);	pxy1=*(u32 *)(iblock+36);
		pxu=btic4b_bcn_4x8x2to2x2x2(pxy0, pxy1);
		pxy0=*(u32 *)(iblock+40);	pxy1=*(u32 *)(iblock+44);
		pxv=btic4b_bcn_4x8x2to2x2x2(pxy0, pxy1);

		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		pxy2=*(u32 *)(iblock+24);	pxy3=*(u32 *)(iblock+28);

		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		px=	((pxy0    )&0x000000FF)|((pxy0>> 8)&0x0000FF00)|
			((pxy1<<16)&0x00FF0000)|((pxy1<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy0>> 8)&0x000000FF)|((pxy0>>16)&0x0000FF00)|
			((pxy1<< 8)&0x00FF0000)|((pxy1    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[2], pxtab[3]);
		px=	((pxy2    )&0x000000FF)|((pxy2>> 8)&0x0000FF00)|
			((pxy3<<16)&0x00FF0000)|((pxy3<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[4], pxtab[5]);
		px=	((pxy2>> 8)&0x000000FF)|((pxy2>>16)&0x0000FF00)|
			((pxy3<< 8)&0x00FF0000)|((pxy3    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x1B:
		pxy0=*(u32 *)(iblock+32);	pxy1=*(u32 *)(iblock+36);
		pxy2=*(u32 *)(iblock+40);	pxy3=*(u32 *)(iblock+44);
		pxy0=btic4b_bcn_4x4x2to2x2x2(pxy0);
		pxy1=btic4b_bcn_4x4x2to2x2x2(pxy1);
		pxy2=btic4b_bcn_4x4x2to2x2x2(pxy2);
		pxy3=btic4b_bcn_4x4x2to2x2x2(pxy3);
		pxu=((pxy0    )&0x0000000F)|((pxy1<< 4)&0x000000F0)|
			((pxy0<< 4)&0x00000F00)|((pxy1<< 8)&0x0000F000)|
			((pxy2<<16)&0x000F0000)|((pxy2<<20)&0x00F00000)|
			((pxy3<<20)&0x0F000000)|((pxy3<<24)&0xF0000000);
		pxu=btic4b_bcn_4x4x2to2x2x2(pxu);

		pxy0=*(u32 *)(iblock+48);	pxy1=*(u32 *)(iblock+52);
		pxy2=*(u32 *)(iblock+56);	pxy3=*(u32 *)(iblock+60);
		pxy0=btic4b_bcn_4x4x2to2x2x2(pxy0);
		pxy1=btic4b_bcn_4x4x2to2x2x2(pxy1);
		pxy2=btic4b_bcn_4x4x2to2x2x2(pxy2);
		pxy3=btic4b_bcn_4x4x2to2x2x2(pxy3);
		pxv=((pxy0    )&0x0000000F)|((pxy1<< 4)&0x000000F0)|
			((pxy0<< 4)&0x00000F00)|((pxy1<< 8)&0x0000F000)|
			((pxy2<<16)&0x000F0000)|((pxy2<<20)&0x00F00000)|
			((pxy3<<20)&0x0F000000)|((pxy3<<24)&0xF0000000);
		pxv=btic4b_bcn_4x4x2to2x2x2(pxv);

		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+20);
		pxy2=*(u32 *)(iblock+24);	pxy3=*(u32 *)(iblock+28);
//		pxu=*(u32 *)(iblock+32);	pxv=*(u32 *)(iblock+36);
//		pxu=btic4b_bcn_4x4x2to2x2x2(pxu);
//		pxv=btic4b_bcn_4x4x2to2x2x2(pxv);
		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		px=	((pxy0    )&0x000000FF)|((pxy0>> 8)&0x0000FF00)|
			((pxy1<<16)&0x00FF0000)|((pxy1<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct0,
			px, 0, pxtab[0], pxtab[1]);
		px=	((pxy0>> 8)&0x000000FF)|((pxy0>>16)&0x0000FF00)|
			((pxy1<< 8)&0x00FF0000)|((pxy1    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct1,
			px, 0, pxtab[2], pxtab[3]);
		px=	((pxy2    )&0x000000FF)|((pxy2>> 8)&0x0000FF00)|
			((pxy3<<16)&0x00FF0000)|((pxy3<< 8)&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct2,
			px, 0, pxtab[4], pxtab[5]);
		px=	((pxy2>> 8)&0x000000FF)|((pxy2>>16)&0x0000FF00)|
			((pxy3<< 8)&0x00FF0000)|((pxy3    )&0xFF000000);
		BTIC4B_BC7_EncodeBlockBits32Px(ctx, ct3,
			px, 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x1C:
		pxu=*(u32 *)(iblock+40);	pxv=*(u32 *)(iblock+44);
		pxu=btic4b_bcn_4x4x2to2x2x2(pxu);
		pxv=btic4b_bcn_4x4x2to2x2x2(pxv);
		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);

		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+19);
		pxy2=*(u32 *)(iblock+22);	pxy3=*(u32 *)(iblock+25);
		li=	((((u64)pxy0)    )&0x000000000FFF)|
			((((u64)pxy1)<<12)&0x000000FFF000)|
			((((u64)pxy2)<<24)&0x000FFF000000)|
			((((u64)pxy3)<<36)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct0,
			li, 0, pxtab[0], pxtab[1]);
		li=	((((u64)pxy0)>>12)&0x000000000FFF)|
			((((u64)pxy1)    )&0x000000FFF000)|
			((((u64)pxy2)<<12)&0x000FFF000000)|
			((((u64)pxy3)<<24)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct1,
			li, 0, pxtab[2], pxtab[3]);
		pxy0=*(u32 *)(iblock+28);	pxy1=*(u32 *)(iblock+31);
		pxy2=*(u32 *)(iblock+34);	pxy3=*(u32 *)(iblock+37);
		li=	((((u64)pxy0)    )&0x000000000FFF)|
			((((u64)pxy1)<<12)&0x000000FFF000)|
			((((u64)pxy2)<<24)&0x000FFF000000)|
			((((u64)pxy3)<<36)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct2,
			li, 0, pxtab[4], pxtab[5]);
		li=	((((u64)pxy0)>>12)&0x000000000FFF)|
			((((u64)pxy1)    )&0x000000FFF000)|
			((((u64)pxy2)<<12)&0x000FFF000000)|
			((((u64)pxy3)<<24)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct3,
			li, 0, pxtab[6], pxtab[7]);
		break;
#endif

#if 1
	case 0x1E:
		pxy0=*(u32 *)(iblock+40);		pxy1=*(u32 *)(iblock+43);
		pxy2=*(u32 *)(iblock+46);		pxy3=*(u32 *)(iblock+49);
		pxu=btic4b_bcn_4x8x3to2x2x2(pxy0, pxy1, pxy2, pxy3);
		pxy0=*(u32 *)(iblock+52);		pxy1=*(u32 *)(iblock+55);
		pxy2=*(u32 *)(iblock+58);		pxy3=*(u32 *)(iblock+61);
		pxv=btic4b_bcn_4x8x3to2x2x2(pxy0, pxy1, pxy2, pxy3);

		BTIC4B_DecBlockTab1b4(ctx, iblock, pxtab, pxu, pxv);
		pxy0=*(u32 *)(iblock+16);	pxy1=*(u32 *)(iblock+19);
		pxy2=*(u32 *)(iblock+22);	pxy3=*(u32 *)(iblock+25);
		li=	((((u64)pxy0)    )&0x000000000FFF)|
			((((u64)pxy1)<<12)&0x000000FFF000)|
			((((u64)pxy2)<<24)&0x000FFF000000)|
			((((u64)pxy3)<<36)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct0,
			li, 0, pxtab[0], pxtab[1]);
		li=	((((u64)pxy0)>>12)&0x000000000FFF)|
			((((u64)pxy1)    )&0x000000FFF000)|
			((((u64)pxy2)<<12)&0x000FFF000000)|
			((((u64)pxy3)<<24)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct1,
			li, 0, pxtab[2], pxtab[3]);
		pxy0=*(u32 *)(iblock+28);	pxy1=*(u32 *)(iblock+31);
		pxy2=*(u32 *)(iblock+34);	pxy3=*(u32 *)(iblock+37);
		li=	((((u64)pxy0)    )&0x000000000FFF)|
			((((u64)pxy1)<<12)&0x000000FFF000)|
			((((u64)pxy2)<<24)&0x000FFF000000)|
			((((u64)pxy3)<<36)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct2,
			li, 0, pxtab[4], pxtab[5]);
		li=	((((u64)pxy0)>>12)&0x000000000FFF)|
			((((u64)pxy1)    )&0x000000FFF000)|
			((((u64)pxy2)<<12)&0x000FFF000000)|
			((((u64)pxy3)<<24)&0xFFF000000000);
		BTIC4B_BC7_EncodeBlockBits48Px(ctx, ct3,
			li, 0, pxtab[6], pxtab[7]);
		break;
#endif

	default:
		BTIC4B_ConvBlockSpecialBC7_A(ctx,
			iblock, oblock, obxstr, obystr, tfl);
		break;
	}
}

void BTIC4B_ConvBlockBC7_B(
	BTIC4B_Context *ctx, byte *iblock,
	byte *oblock, byte *pblock, int tfl)
{
	int avg[4];
	int cy, cu, cv, ca;
	u32 px;

	if(!(*(u16 *)iblock))
	{
		cy=*(s16 *)(iblock+4);
		cu=*(s16 *)(iblock+6);
		cv=*(s16 *)(iblock+8);
		ca=iblock[2];
		px=ctx->ClrDec1A(cy, cu, cv, ca);
		avg[2]=(px    )&255;	avg[1]=(px>> 8)&255;
		avg[0]=(px>>16)&255;	avg[3]=(px>>24)&255;
		
		if(pblock)
		{
			cy=lqtvq_clamp255(cy);
			cu=(cu>>1)+128;
			cv=(cv>>1)+128;
			cu=lqtvq_clamp255(cu);
			cv=lqtvq_clamp255(cv);
		
			pblock[ 0]=cy;	pblock[ 1]=0;
			pblock[ 2]=ca;	pblock[ 3]=0;
			pblock[ 4]=cu;	pblock[ 5]=cv;
			pblock[ 6]=0;	pblock[ 7]=0;
			pblock[ 8]=cy;	pblock[ 9]=cy;
			pblock[10]=cy;	pblock[11]=cy;
			pblock[12]=ca;	pblock[13]=ca;
			pblock[14]=ca;	pblock[15]=ca;
		}
		
//		BTIC4B_BC7_EncodeBlockFlat_Mode5(oblock, avg);
		ctx->BCnEncodeBlockFlat(oblock, avg);
		return;
	}

	if(iblock[1]&0x1F)
	{
		BTIC4B_ConvBlockSpecialBC7_B(ctx,
			iblock, oblock, pblock, tfl);
		return;
	}

	switch(iblock[0])
	{
	default:
		BTIC4B_ConvBlockSpecialBC7_B(ctx,
			iblock, oblock, pblock, tfl);
		break;
	}
}

void BTIC4B_ConvBlockBC7_C(
	BTIC4B_Context *ctx,
	byte *ipblock, int ipystr,
	byte *oblock, byte *pblock, int tfl)
{
	byte tblk[64*4];
	s16 yblk[64], ablk[64];
	int min[4], max[4];
	byte *cs, *ct;
	int cr, cg, cb, ca, cy, cu, cv, cu1, cv1;
	int acr, acg, acb;
	int cy0, cy1;
	int mcy, ncy, mca, nca, acy, aca, dcy, dca;
	int mcu, ncu, mcv, ncv, acu, acv, dcu, dcv;
	int i, j, k, l, xn;

	mcy=256; ncy=-1;
	mca=256; nca=-1;
	acr=0; acg=0; acb=0;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		k=((i>>1)*ipystr)+(j>>1)*16;
		l=(i&1)*2+(j&1);
		
		cy=ipblock[k+ 8+l];
		ca=ipblock[k+12+l];
		cu=ipblock[k+ 4];
		cv=ipblock[k+ 5];
		
		cu1=(cu-128)<<1; cv1=(cv-128)<<1;
		cg=cy-((cu1+cv1)>>2);
		cb=cg+cu1;
		cr=cg+cv1;

//		cr=lqtvq_clamp255(cr);
//		cg=lqtvq_clamp255(cg);
//		cb=lqtvq_clamp255(cb);
		
		acr+=cr;
		acg+=cg;
		acb+=cb;

		k=i*4+j;
		yblk[k]=cy;
		ablk[k]=ca;
		if(cy<mcy)	{ mcy=cy; min[0]=cr; min[1]=cg; min[2]=cb; }
		if(cy>ncy)	{ ncy=cy; max[0]=cr; max[1]=cg; max[2]=cb; }
		if(ca<mca)	{ mca=ca; }
		if(ca>nca)	{ nca=ca; }
	}

	acr=acr>>4;
	acg=acg>>4;
	acb=acb>>4;
	
//	min[0]=(3*min[0]+acr)>>2;
//	min[1]=(3*min[1]+acg)>>2;
//	min[2]=(3*min[2]+acb)>>2;
//	max[0]=(3*max[0]+acr)>>2;
//	max[1]=(3*max[1]+acg)>>2;
//	max[2]=(3*max[2]+acb)>>2;

	acy=(2*acg+acr+acb)>>2;
	cy0=mcy-acy;
	cy1=ncy-acy;
	min[0]=lqtvq_clamp255((min[0]+3*(acr+cy0))>>2);
	min[1]=lqtvq_clamp255((min[1]+3*(acg+cy0))>>2);
	min[2]=lqtvq_clamp255((min[2]+3*(acb+cy0))>>2);
	max[0]=lqtvq_clamp255((max[0]+3*(acr+cy1))>>2);
	max[1]=lqtvq_clamp255((max[1]+3*(acg+cy1))>>2);
	max[2]=lqtvq_clamp255((max[2]+3*(acb+cy1))>>2);

	if(pblock)
	{
		acy=(mcy+ncy)>>1;
		aca=(mca+nca)>>1;
		dcy=ncy-mcy;
		dca=nca-mca;
		
		mcu=((min[2]-min[1])>>1)+128;
		mcv=((min[0]-min[1])>>1)+128;
		ncu=((max[2]-max[1])>>1)+128;
		ncv=((max[0]-max[1])>>1)+128;
//		acu=(mcu+ncu)>>1;
//		acv=(mcv+ncv)>>1;
		dcu=ncu-mcu;
		dcv=ncv-mcv;

		acu=(mcu+3*ncu)>>2;
		acv=(mcv+3*ncv)>>2;
	
		pblock[ 0]=acy;	pblock[ 1]=dcy;
		pblock[ 2]=aca;	pblock[ 3]=dca;
		pblock[ 4]=acu;	pblock[ 5]=acv;
		pblock[ 6]=dcu;	pblock[ 7]=dcv;

		pblock[ 8]=(yblk[ 0]+yblk[ 1]+yblk[ 4]+yblk[ 5])>>2;
		pblock[ 9]=(yblk[ 2]+yblk[ 3]+yblk[ 6]+yblk[ 7])>>2;
		pblock[10]=(yblk[ 8]+yblk[ 9]+yblk[12]+yblk[13])>>2;
		pblock[11]=(yblk[10]+yblk[11]+yblk[14]+yblk[15])>>2;

		pblock[12]=(ablk[ 0]+ablk[ 1]+ablk[ 4]+ablk[ 5])>>2;
		pblock[13]=(ablk[ 2]+ablk[ 3]+ablk[ 6]+ablk[ 7])>>2;
		pblock[14]=(ablk[ 8]+ablk[ 9]+ablk[12]+ablk[13])>>2;
		pblock[15]=(ablk[10]+ablk[11]+ablk[14]+ablk[15])>>2;
	}

	min[3]=mca;	max[3]=nca;
//	BTIC4B_BC7_EncodeBlock_Mode5(oblock, yblk, ablk,
//		min, max, mcy, ncy, mca, nca);
	ctx->BCnEncodeBlockGen(oblock, yblk, ablk,
		min, max, mcy, ncy, mca, nca);
}

void BTIC4B_ConvImageBC7_AI(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int oblkstr, int xs, int ys)
{
	int xs1, ys1, obystr;
	int i, j, k0, k1, tfl;

	BTIC4B_ConvBlockBC7_Init();
	BTIC4B_DecImageSetupClrsI(ctx, BTIC4B_CLRS_BGRA);
	
	tfl=0;
	if(ys<0)
	{
		ys=-ys; tfl|=1;
	}
	
	xs1=(xs+7)>>3; ys1=(ys+7)>>3;
	obystr=(tfl&1)?(-2*xs1*oblkstr):(2*xs1*oblkstr);

	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		k0=i*xs1+j;
		k1=(tfl&1)?((2*ys1-i*2-1)*2*xs1+j*2):(i*2*2*xs1+j*2);
		BTIC4B_ConvBlockBC7_A(ctx,
			iblock+k0*iblkstr,
			oblock+k1*oblkstr,
			oblkstr,
			obystr,
			tfl);
	}
}

void BTIC4B_ConvImageBC7_BI(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int oblkstr, byte *pblock,
	int xs, int ys)
{
	int xs1, ys1;
	int i, j, k0, k1, k2, tfl;

	BTIC4B_ConvBlockBC7_Init();
	BTIC4B_DecImageSetupClrsI(ctx, BTIC4B_CLRS_BGRA);
	
	tfl=0;
	if(ys<0)
	{
		ys=-ys; tfl|=1;
	}
	
	xs1=(xs+3)>>2; ys1=(ys+3)>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		k0=i*xs1+j;
		k1=(tfl&1)?((ys1-i-1)*xs1+j):(i*xs1+j);
//		k2=(tfl&1)?((2*ys1-i*2-1)*2*xs1+j*2):(i*2*2*xs1+j*2);
		k2=k1;
		BTIC4B_ConvBlockBC7_B(ctx,
			iblock+k0*iblkstr,
			oblock+k1*oblkstr,
			pblock+k2*16,
			tfl);
	}
}

void BTIC4B_ConvImageBC7_CI(BTIC4B_Context *ctx,
	byte *ipblock, byte *oblock, int oblkstr, byte *pblock,
	int xs, int ys)
{
	int xs1, ys1;
	int i, j, k0, k1, tfl;

	tfl=0;
	if(ys<0)
		{ ys=-ys; tfl|=1; }
	
	xs1=(xs+3)>>2; ys1=(ys+3)>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		k0=i*2*2*xs1+j*2;
		k1=i*xs1+j;
		BTIC4B_ConvBlockBC7_C(ctx,
			ipblock+k0*16, xs1*2*16,
			oblock+k1*oblkstr,
			pblock+k1*16,
			tfl);
	}
}

void BTIC4B_ConvImageBC7n(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
//	ctx->BCnEncodeBlockGen=BTIC4B_BC7_EncodeBlock_Mode5;
	ctx->BCnEncodeBlockGen=BTIC4B_BC7_EncodeBlock_AutoMode;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC7_EncodeBlockBits32_Mode5;
//	ctx->BCnEncodeBlockBits48=BTIC4B_BC7_EncodeBlockBits48_Mode5;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC7_EncodeBlockBits48_AutoMode;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC7_EncodeBlockBits64_Mode6;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC7_EncodeBlockFlat_Mode5;

	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, oblock, 16, xs, ys);
}

void BTIC4B_ConvImageBC7nMip(BTIC4B_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int xs, int ys)
{
//	byte *pblk;
	byte *ct;
	int xs1, ys1, xs2, ys2;
	
//	ctx->BCnEncodeBlockGen=BTIC4B_BC7_EncodeBlock_Mode5;
	ctx->BCnEncodeBlockGen=BTIC4B_BC7_EncodeBlock_AutoMode;
	ctx->BCnEncodeBlockBits32=BTIC4B_BC7_EncodeBlockBits32_Mode5;
//	ctx->BCnEncodeBlockBits48=BTIC4B_BC7_EncodeBlockBits48_Mode5;
	ctx->BCnEncodeBlockBits48=BTIC4B_BC7_EncodeBlockBits48_AutoMode;
	ctx->BCnEncodeBlockBits64=BTIC4B_BC7_EncodeBlockBits64_Mode6;
	ctx->BCnEncodeBlockFlat=BTIC4B_BC7_EncodeBlockFlat_Mode5;

	ct=oblock; xs1=xs; ys1=ys;
	BTIC4B_ConvImageBC7_AI(ctx,
		iblock, iblkstr, ct, 16, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	if(!ctx->pblk)
		ctx->pblk=malloc(xs1*ys1*16);

	BTIC4B_ConvImageBC7_BI(ctx,
		iblock, iblkstr, ct, 16, ctx->pblk, xs1, ys1);
	ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
	xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	
	while((xs1>=4) && (ys1>=4))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 16, ctx->pblk, xs1, ys1);
		ct+=((xs1+3)>>2)*((ys1+3)>>2)*16;
		xs1=(xs1+1)>>1;		ys1=(ys1+1)>>1;
	}

	xs2=xs1; ys2=ys1;
	while((xs2>1) || (ys2>1))
	{
		BTIC4B_ConvImageBC7_CI(ctx,
			ctx->pblk, ct, 16, ctx->pblk, xs1, ys1);
		ct+=((xs2+3)>>2)*((ys2+3)>>2)*16;
		xs2=(xs2+1)>>1;		ys2=(ys2+1)>>1;
	}
}


#if 1
void BTIC4B_BitsLE_SetupRead(BTIC4B_BitStream *ctx, byte *cs, int sz)
{
	ctx->cs=cs;
	ctx->cse=cs+sz;
//	ctx->win=0;
	ctx->pos=0;
	
	ctx->win=ctx->cs[0] | (ctx->cs[1]<<8) |
		(ctx->cs[2]<<16) | (ctx->cs[3]<<24);
	ctx->cs+=4;
}

void BTIC4B_BitsLE_ClearSetupRead(BTIC4B_BitStream *ctx, byte *ct, int sz)
{
	BTIC4B_BitsLE_Clear(ctx);
	BTIC4B_BitsLE_SetupRead(ctx, ct, sz);
}

int BTIC4B_BitsLE_ReadBits(BTIC4B_BitStream *ctx, int n)
{
	int v;
	
	v=(ctx->win>>ctx->pos)&((1<<n)-1);
	ctx->pos+=n;
	while(ctx->pos>=8)
	{
		ctx->win=(ctx->win>>8)|((*ctx->cs++)<<24);
		ctx->pos-=8;
	}
	return(v);
}

int BTIC4B_BitsLE_ReadBit(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 1)); }
int BTIC4B_BitsLE_Read2Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 2)); }
int BTIC4B_BitsLE_Read3Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 3)); }
int BTIC4B_BitsLE_Read4Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 4)); }
int BTIC4B_BitsLE_Read5Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 5)); }
int BTIC4B_BitsLE_Read6Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 6)); }
int BTIC4B_BitsLE_Read7Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 7)); }
int BTIC4B_BitsLE_Read8Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 8)); }

int BTIC4B_BitsLE_Read14Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 14)); }
int BTIC4B_BitsLE_Read16Bits(BTIC4B_BitStream *ctx)
	{ return(BTIC4B_BitsLE_ReadBits(ctx, 16)); }

int BTIC4B_BitsLE_Peek8Bits(BTIC4B_BitStream *ctx)
{
	int v;
	v=(ctx->win>>ctx->pos)&255;
	return(v);
}

int btic4b_bc7_weights2[4]=
	{ 0, 21, 43, 64};
int btic4b_bc7_weights3[8]=
	{ 0,  9, 18, 27, 37, 46, 55, 64};
int btic4b_bc7_weights4[16]= 
	{ 0,  4,  9, 13, 17, 21, 26, 30,
	 34, 38, 43, 47, 51, 55, 60, 64};

int btic4b_bc7_interpolate(int e0, int e1, int idx, int prec)
{
	int i, w;

	switch(prec)
	{
	case 2:
		w=btic4b_bc7_weights2[idx];
		i=(((64-w)*e0+w*e1+32)>>6);
		break;
	case 3:
		w=btic4b_bc7_weights3[idx];
		i=(((64-w)*e0+w*e1+32)>>6);
		break;
	case 4:
		w=btic4b_bc7_weights4[idx];
		i=(((64-w)*e0+w*e1+32)>>6);
		break;
	default:
		i=0; break;
	}
	return(i);
}

void BTIC4B_BC7_DecodeBlock_Mode4(
	BTIC4B_BitStream *bits,
	byte *rgba, int xstride, int ystride, int flags)
{
	byte blkb[16*4];
	byte clrb[8*4];
	int rot, idxm;
	int r0, r1, g0, g1, b0, b1, a0, a1;
	int p;
	int i, j, k, l;
	
	rot=BTIC4B_BitsLE_Read2Bits(bits);
	idxm=BTIC4B_BitsLE_ReadBit(bits);

	r0=BTIC4B_BitsLE_Read5Bits(bits);
	r1=BTIC4B_BitsLE_Read5Bits(bits);
	g0=BTIC4B_BitsLE_Read5Bits(bits);
	g1=BTIC4B_BitsLE_Read5Bits(bits);
	b0=BTIC4B_BitsLE_Read5Bits(bits);
	b1=BTIC4B_BitsLE_Read5Bits(bits);
	a0=BTIC4B_BitsLE_Read6Bits(bits);
	a1=BTIC4B_BitsLE_Read6Bits(bits);
	
	r0=(r0<<3)|(r0>>2); g0=(g0<<3)|(g0>>2);
	b0=(b0<<3)|(b0>>2); a0=(a0<<2)|(a0>>4);
	r1=(r1<<3)|(r1>>2); g1=(g1<<3)|(g1>>2);
	b1=(b1<<3)|(b1>>2); a1=(a1<<2)|(a1>>4);
	
	if(idxm==0)
	{
		for(i=0; i<4; i++)
		{
			clrb[i*4+0]=btic4b_bc7_interpolate(r0, r1, i, 2);
			clrb[i*4+1]=btic4b_bc7_interpolate(g0, g1, i, 2);
			clrb[i*4+2]=btic4b_bc7_interpolate(b0, b1, i, 2);
		}

		for(i=0; i<8; i++)
		{
			clrb[i*4+3]=btic4b_bc7_interpolate(a0, a1, i, 3);
		}
		
		p=BTIC4B_BitsLE_ReadBit(bits);
		blkb[0*4+0]=clrb[p*4+0];
		blkb[0*4+1]=clrb[p*4+1];
		blkb[0*4+2]=clrb[p*4+2];
		for(i=1; i<16; i++)
		{
			p=BTIC4B_BitsLE_Read2Bits(bits);
			blkb[i*4+0]=clrb[p*4+0];
			blkb[i*4+1]=clrb[p*4+1];
			blkb[i*4+2]=clrb[p*4+2];
		}

		p=BTIC4B_BitsLE_Read2Bits(bits);
		blkb[0*4+3]=clrb[p*4+3];
		for(i=1; i<16; i++)
		{
			p=BTIC4B_BitsLE_Read3Bits(bits);
			blkb[i*4+3]=clrb[p*4+3];
		}
	}else
	{
		for(i=0; i<8; i++)
		{
			clrb[i*4+0]=btic4b_bc7_interpolate(r0, r1, i, 3);
			clrb[i*4+1]=btic4b_bc7_interpolate(g0, g1, i, 3);
			clrb[i*4+2]=btic4b_bc7_interpolate(b0, b1, i, 3);
		}

		for(i=0; i<4; i++)
		{
			clrb[i*4+3]=btic4b_bc7_interpolate(a0, a1, i, 2);
		}
		
		p=BTIC4B_BitsLE_ReadBit(bits);
		blkb[0*4+3]=clrb[p*4+3];
		for(i=1; i<16; i++)
		{
			p=BTIC4B_BitsLE_Read2Bits(bits);
			blkb[i*4+3]=clrb[p*4+3];
		}

		p=BTIC4B_BitsLE_Read2Bits(bits);
		blkb[0*4+0]=clrb[p*4+0];
		blkb[0*4+1]=clrb[p*4+1];
		blkb[0*4+2]=clrb[p*4+2];
		for(i=1; i<16; i++)
		{
			p=BTIC4B_BitsLE_Read3Bits(bits);
			blkb[i*4+0]=clrb[p*4+0];
			blkb[i*4+1]=clrb[p*4+1];
			blkb[i*4+2]=clrb[p*4+2];
		}
	}
	
	switch(rot)
	{
	case 0: break;
	case 1: case 2: case 3:
		k=rot-1;
		for(i=0; i<16; i++)
			{ j=blkb[i*4+k]; blkb[i*4+k]=blkb[i*4+3]; blkb[i*4+3]=j; }
		break;
	default:
		break;
	}

	if(flags&1)
	{
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride; l=(i*4+j)*4;
			rgba[k+2]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+0]=blkb[l+2]; rgba[k+3]=blkb[l+3];
		}
	}else
	{
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride; l=(i*4+j)*4;
			rgba[k+0]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+2]=blkb[l+2]; rgba[k+3]=blkb[l+3];
		}
	}
}

void BTIC4B_BC7_DecodeBlock_Mode5(
	BTIC4B_BitStream *bits,
	byte *rgba, int xstride, int ystride, int flags)
{
	byte blkb[16*4];
	byte clrb[8*4];
	byte *bp0, *bp1;
	int rot, idxm;
	int r0, r1, g0, g1, b0, b1, a0, a1;
	int p, p0, p1, p2, p3;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	rot=BTIC4B_BitsLE_Read2Bits(bits);
	p0=BTIC4B_BitsLE_Read14Bits(bits);
	p1=BTIC4B_BitsLE_Read14Bits(bits);
	p2=BTIC4B_BitsLE_Read14Bits(bits);
	p3=BTIC4B_BitsLE_Read16Bits(bits);
	r0=p0&127; r1=(p0>>7);	g0=p1&127; g1=(p1>>7);
	b0=p2&127; b1=(p2>>7);	a0=p3&255; a1=(p3>>8);

	r0=(r0<<1)|(r0>>6); r1=(r1<<1)|(r1>>6);
	g0=(g0<<1)|(g0>>6); g1=(g1<<1)|(g1>>6);
	b0=(b0<<1)|(b0>>6); b1=(b1<<1)|(b1>>6);

	if((r0==r1) && (g0==g1) && (b0==b1) && (a0==a1))
	{
		if(flags&1)
		{
			for(i=0; i<4; i++)
			{
				j=i*ystride;
				i0=j+0*xstride;		i1=j+1*xstride;
				i2=j+2*xstride;		i3=j+3*xstride;
				rgba[i0+2]=r0; rgba[i0+1]=g0;
				rgba[i0+0]=b0; rgba[i0+3]=a0;
				rgba[i1+2]=r0; rgba[i1+1]=g0;
				rgba[i1+0]=b0; rgba[i1+3]=a0;
				rgba[i2+2]=r0; rgba[i2+1]=g0;
				rgba[i2+0]=b0; rgba[i2+3]=a0;
				rgba[i3+2]=r0; rgba[i3+1]=g0;
				rgba[i3+0]=b0; rgba[i3+3]=a0;
			}
		}else
		{
			for(i=0; i<4; i++)
			{
				j=i*ystride;
				i0=j+0*xstride;		i1=j+1*xstride;
				i2=j+2*xstride;		i3=j+3*xstride;
				rgba[i0+0]=r0; rgba[i0+1]=g0;
				rgba[i0+2]=b0; rgba[i0+3]=a0;
				rgba[i1+0]=r0; rgba[i1+1]=g0;
				rgba[i1+2]=b0; rgba[i1+3]=a0;
				rgba[i2+0]=r0; rgba[i2+1]=g0;
				rgba[i2+2]=b0; rgba[i2+3]=a0;
				rgba[i3+0]=r0; rgba[i3+1]=g0;
				rgba[i3+2]=b0; rgba[i3+3]=a0;
			}
		}
		return;
	}

	for(i=0; i<4; i++)
	{
		clrb[i*4+0]=btic4b_bc7_interpolate(r0, r1, i, 2);
		clrb[i*4+1]=btic4b_bc7_interpolate(g0, g1, i, 2);
		clrb[i*4+2]=btic4b_bc7_interpolate(b0, b1, i, 2);
		clrb[i*4+3]=btic4b_bc7_interpolate(a0, a1, i, 2);
	}
	
	if((r0==r1) && (g0==g1) && (b0==b1))
	{
		for(i=0; i<16; i++)
		{
			blkb[i*4+0]=r0;
			blkb[i*4+1]=g0;
			blkb[i*4+2]=b0;
		}
		if(a0!=a1)
			{ p=BTIC4B_BitsLE_ReadBits(bits, 31); }
	}else
	{
		p=BTIC4B_BitsLE_Read7Bits(bits);
		p0=(p&1)*4;			p1=((p>>1)&3)*4;
		p2=((p>>3)&3)*4;	p3=((p>>5)&3)*4;
		i0=0*4;		i1=1*4;
		i2=2*4;		i3=3*4;
		blkb[i0+0]=clrb[p0+0];	blkb[i0+1]=clrb[p0+1];
		blkb[i0+2]=clrb[p0+2];	blkb[i1+0]=clrb[p1+0];
		blkb[i1+1]=clrb[p1+1];	blkb[i1+2]=clrb[p1+2];
		blkb[i2+0]=clrb[p2+0];	blkb[i2+1]=clrb[p2+1];
		blkb[i2+2]=clrb[p2+2];	blkb[i3+0]=clrb[p3+0];
		blkb[i3+1]=clrb[p3+1];	blkb[i3+2]=clrb[p3+2];
		for(i=1; i<4; i++)
		{
			p=BTIC4B_BitsLE_Read8Bits(bits);
			p0=(p&3)*4;			p1=((p>>2)&3)*4;
			p2=((p>>4)&3)*4;	p3=((p>>6)&3)*4;
			i0=(i*4+0)*4;		i1=(i*4+1)*4;
			i2=(i*4+2)*4;		i3=(i*4+3)*4;
			blkb[i0+0]=clrb[p0+0];	blkb[i0+1]=clrb[p0+1];
			blkb[i0+2]=clrb[p0+2];	blkb[i1+0]=clrb[p1+0];
			blkb[i1+1]=clrb[p1+1];	blkb[i1+2]=clrb[p1+2];
			blkb[i2+0]=clrb[p2+0];	blkb[i2+1]=clrb[p2+1];
			blkb[i2+2]=clrb[p2+2];	blkb[i3+0]=clrb[p3+0];
			blkb[i3+1]=clrb[p3+1];	blkb[i3+2]=clrb[p3+2];
		}
	}

	if(a0==a1)
	{
		for(i=0; i<4; i++)
		{
			blkb[i*16+0*4+3]=a0;	blkb[i*16+1*4+3]=a0;
			blkb[i*16+2*4+3]=a0;	blkb[i*16+3*4+3]=a0;
		}
	}else
	{
		p=BTIC4B_BitsLE_Read7Bits(bits);
		p0=(p&1)*4;			p1=((p>>1)&3)*4;
		p2=((p>>3)&3)*4;	p3=((p>>5)&3)*4;
		i0=0*4;		i1=1*4;
		i2=2*4;		i3=3*4;
		blkb[i0+3]=clrb[p0+3];	blkb[i1+3]=clrb[p1+3];
		blkb[i2+3]=clrb[p2+3];	blkb[i3+3]=clrb[p3+3];
		for(i=1; i<4; i++)
		{
			p=BTIC4B_BitsLE_Read8Bits(bits);
			p0=(p&3)*4;			p1=((p>>2)&3)*4;
			p2=((p>>4)&3)*4;	p3=((p>>6)&3)*4;
			i0=(i*4+0)*4;		i1=(i*4+1)*4;
			i2=(i*4+2)*4;		i3=(i*4+3)*4;
			blkb[i0+3]=clrb[p0+3];	blkb[i1+3]=clrb[p1+3];
			blkb[i2+3]=clrb[p2+3];	blkb[i3+3]=clrb[p3+3];
		}
	}
	
	switch(rot)
	{
	case 0: break;
	case 1: case 2: case 3:
		k=rot-1;
		for(i=0; i<16; i++)
			{ j=blkb[i*4+k]; blkb[i*4+k]=blkb[i*4+3]; blkb[i*4+3]=j; }
		break;
	default:
		break;
	}

	if(flags&1)
	{
		for(i=0; i<4; i++)
		{
			k=i*ystride+0*xstride; l=(i*4+0)*4;
			rgba[k+2]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+0]=blkb[l+2]; rgba[k+3]=blkb[l+3];
			k=i*ystride+1*xstride; l=(i*4+1)*4;
			rgba[k+2]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+0]=blkb[l+2]; rgba[k+3]=blkb[l+3];
			k=i*ystride+2*xstride; l=(i*4+2)*4;
			rgba[k+2]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+0]=blkb[l+2]; rgba[k+3]=blkb[l+3];
			k=i*ystride+3*xstride; l=(i*4+3)*4;
			rgba[k+2]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+0]=blkb[l+2]; rgba[k+3]=blkb[l+3];
		}
	}else
	{
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride; l=(i*4+j)*4;
			rgba[k+0]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+2]=blkb[l+2]; rgba[k+3]=blkb[l+3];
		}
	}
}

void BTIC4B_BC7_DecodeBlock_Mode6(
	BTIC4B_BitStream *bits,
	byte *rgba, int xstride, int ystride, int flags)
{
	byte blkb[16*4];
	byte clrb[16*4];
	int rot, idxm;
	int r0, r1, g0, g1, b0, b1, a0, a1;
	int p;
	int i, j, k, l;
	
	r0=BTIC4B_BitsLE_Read7Bits(bits);
	r1=BTIC4B_BitsLE_Read7Bits(bits);
	g0=BTIC4B_BitsLE_Read7Bits(bits);
	g1=BTIC4B_BitsLE_Read7Bits(bits);
	b0=BTIC4B_BitsLE_Read7Bits(bits);
	b1=BTIC4B_BitsLE_Read7Bits(bits);
	a0=BTIC4B_BitsLE_Read7Bits(bits);
	a1=BTIC4B_BitsLE_Read7Bits(bits);

	j=BTIC4B_BitsLE_ReadBit(bits);
	k=BTIC4B_BitsLE_ReadBit(bits);
	r0=(r0<<1)|j; g0=(g0<<1)|j; b0=(b0<<1)|j; a0=(a0<<1)|j;
	r1=(r1<<1)|k; g1=(g1<<1)|k; b1=(b1<<1)|k; a1=(a1<<1)|k;
	
	for(i=0; i<16; i++)
	{
		clrb[i*4+0]=btic4b_bc7_interpolate(r0, r1, i, 4);
		clrb[i*4+1]=btic4b_bc7_interpolate(g0, g1, i, 4);
		clrb[i*4+2]=btic4b_bc7_interpolate(b0, b1, i, 4);
		clrb[i*4+3]=btic4b_bc7_interpolate(a0, a1, i, 4);
	}
		
	p=BTIC4B_BitsLE_Read3Bits(bits);
	blkb[0*4+0]=clrb[p*4+0]; blkb[0*4+1]=clrb[p*4+1];
	blkb[0*4+2]=clrb[p*4+2]; blkb[0*4+3]=clrb[p*4+3];
	for(i=1; i<16; i++)
	{
		p=BTIC4B_BitsLE_Read4Bits(bits);
		blkb[i*4+0]=clrb[p*4+0]; blkb[i*4+1]=clrb[p*4+1];
		blkb[i*4+2]=clrb[p*4+2]; blkb[i*4+3]=clrb[p*4+3];
	}
	
	if(flags&1)
	{
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride; l=(i*4+j)*4;
			rgba[k+2]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+0]=blkb[l+2]; rgba[k+3]=blkb[l+3];
		}
	}else
	{
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride; l=(i*4+j)*4;
//			k=i*ystride+j*xstride; l=(j*4+i)*4;
			rgba[k+0]=blkb[l+0]; rgba[k+1]=blkb[l+1];
			rgba[k+2]=blkb[l+2]; rgba[k+3]=blkb[l+3];
		}
	}
}

int BTIC4B_BC7_GetBlockMode(byte *block)
{
	int mode;
	int i;
	
	i=block[0];
	if((i&1)==1)			{ mode=0; }
	else if((i&  3)==  2)	{ mode=1; }
	else if((i&  7)==  4)	{ mode=2; }
	else if((i& 15)==  8)	{ mode=3; }
	else if((i& 31)== 16)	{ mode=4; }
	else if((i& 63)== 32)	{ mode=5; }
	else if((i&127)== 64)	{ mode=6; }
	else if((i&255)==128)	{ mode=7; }
	else					{ mode=8; }
	return(mode);
}

int BTIC4B_BC7_DecodeBlock_ReadMode(BTIC4B_BitStream *bits)
{
	int i, j;

	i=BTIC4B_BitsLE_Peek8Bits(bits);
	if(i)
	{
		if(i&15)
		{
			if(i&3)		{ j=(i&1)?0:1; }
			else		{ j=(i&4)?2:3; }
		}else
		{
			if(i&63)	{ j=(i&16)?4:5; }
			else		{ j=(i&64)?6:7; }
		}
		BTIC4B_BitsLE_ReadBits(bits, j+1);
		return(j);
	}
	return(8);
}

void BTIC4B_BC7_DecodeBlock(byte *block,
	byte *rgba, int xstride, int ystride, int flags)
{
	BTIC4B_BitStream bits;
	int mode;

	BTIC4B_BitsLE_ClearSetupRead(&bits, block, 16);
	mode=BTIC4B_BC7_DecodeBlock_ReadMode(&bits);
	switch(mode)
	{
	case 0:
//		BTIC4B_BC7_DecodeBlock_Mode0(&bits, rgba, xstride, ystride, flags);
		break;
	case 1:
//		BTIC4B_BC7_DecodeBlock_Mode1(&bits, rgba, xstride, ystride, flags);
		break;
	case 2:
//		BTIC4B_BC7_DecodeBlock_Mode2(&bits, rgba, xstride, ystride, flags);
		break;
	case 3:
//		BTIC4B_BC7_DecodeBlock_Mode3(&bits, rgba, xstride, ystride, flags);
		break;
	case 4:
		BTIC4B_BC7_DecodeBlock_Mode4(&bits, rgba, xstride, ystride, flags);
		break;
	case 5:
		BTIC4B_BC7_DecodeBlock_Mode5(&bits, rgba, xstride, ystride, flags);
		break;
	case 6:
		BTIC4B_BC7_DecodeBlock_Mode6(&bits, rgba, xstride, ystride, flags);
		break;
	case 7:
//		BTIC4B_BC7_DecodeBlock_Mode7(&bits, rgba, xstride, ystride, flags);
		break;
	default:
//		BTIC4B_BC7_DecodeBlock_Default(&bits, rgba, xstride, ystride, flags);
		break;
	}
}

void BTIC4B_BC7_DecodeImage(byte *block,
	byte *rgba, int xs, int ys, int stride, int pfb)
{
	byte *rgba1;
	int xstr, ystr;
	int xs1, ys1, xs2, ys2;
	int i, j;
	
	xstr=stride; ystr=xs*stride; rgba1=rgba;
	if(ys<0)
	{
		ys=-ys;
		xstr=stride; ystr=-xs*stride;
		rgba1=rgba+(ys-1)*xs*stride;
	}
	
	xs1=xs>>2; ys1=ys>>2;
	xs2=(xs+3)>>2; ys2=(ys+3)>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC4B_BC7_DecodeBlock(
			block+(i*xs2+j)*16,
			rgba1+i*4*ystr+j*4*xstr,
			xstr, ystr, pfb);
	}
}
#endif
