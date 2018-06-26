/*
 * TBD: Fast Shim decoder to BC7.
 * Goal will be to quickly shim MetaBlock2 blocks to analogous BC7 blocks.
 * Conversion Speed will be the priority.
 */

typedef struct {
byte *cs, *cse;
byte *ct, *cte;
u32 win;
int pos;
}BGBBTJ_BitStream;

u16 btic1h_bc7_4x1x2to4x1x3[256];
u64 btic1h_bc7_2x2to4x4x3[256];
u32 btic1h_bc7_2x2to4x4x2[256];
u16 btic1h_bc7_4x1x1to4x1x3[16];
byte btic1h_bc7_4x1x1to4x1x2[16];
byte btic1h_bc7_4x1x3to4x1x2[4096];

byte btic1h_bc7_swap4x1x2[256];


void BTIC1H_ConvBlockBC7_Init()
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
		
		r0=(q0<<9)|(q1<<6)|(q2<<3)|q3;
		btic1h_bc7_4x1x2to4x1x3[i]=r0;

		r0=p0|(p1<<2)|(p2<<4)|(p3<<6);
		btic1h_bc7_swap4x1x2[i]=r0;
	}

	for(i=0; i<256; i++)
	{
		p0=(i>>6)&3;	p1=(i>>4)&3;
		p2=(i>>2)&3;	p3=(i>>0)&3;
		q0=(p0<<1)|(p0>>1);
		q1=(p1<<1)|(p1>>1);
		q2=(p2<<1)|(p2>>1);
		q3=(p3<<1)|(p3>>1);
		
		r0=(q0<<9)|(q0<<6)|(q1<<3)|q1;
		r1=(q2<<9)|(q2<<6)|(q3<<3)|q3;
		r2=(r0<<12)|r0;
		r3=(r1<<12)|r1;
		btic1h_bc7_2x2to4x4x3[i]=(((u64)r2)<<24)|r3;

		r0=(p0<<6)|(p0<<4)|(p1<<2)|p1;
		r1=(p2<<6)|(p2<<4)|(p3<<2)|p3;
		r2=(r0<<24)|(r0<<16)|(r1<<8)|r1;
		btic1h_bc7_2x2to4x4x2[i]=r2;
	}

	for(i=0; i<16; i++)
	{
		p0=(i>>3)&1;	p1=(i>>2)&1;
		p2=(i>>1)&1;	p3=(i   )&1;
		q0=p0?7:0;		q1=p1?7:0;
		q2=p2?7:0;		q3=p3?7:0;
		r0=(q0<<9)|(q1<<6)|(q2<<3)|q3;
		btic1h_bc7_4x1x1to4x1x3[i]=r0;

		q0=p0?3:0;		q1=p1?3:0;
		q2=p2?3:0;		q3=p3?3:0;
		r0=(q0<<6)|(q1<<4)|(q2<<2)|q3;
		btic1h_bc7_4x1x1to4x1x2[i]=r0;
	}
}

void BGBBTJ_BitsLE_Clear(BGBBTJ_BitStream *ctx)
{
	memset(ctx, 0, sizeof(BGBBTJ_BitStream));
}

void BGBBTJ_BitsLE_SetupWrite(BGBBTJ_BitStream *ctx, byte *ct, int sz)
{
	ctx->ct=ct;
	ctx->cte=ct+sz;
	ctx->win=0;
	ctx->pos=0;
}

void BGBBTJ_BitsLE_ClearSetupWrite(BGBBTJ_BitStream *ctx, byte *ct, int sz)
{
	BGBBTJ_BitsLE_Clear(ctx);
	BGBBTJ_BitsLE_SetupWrite(ctx, ct, sz);
}

void BGBBTJ_BitsLE_WriteBits(BGBBTJ_BitStream *ctx, int v, int n)
{
	v&=(1<<n)-1;
	ctx->win|=v<<(ctx->pos);
	ctx->pos+=n;
	while(ctx->pos>=8)
	{
		*ctx->ct++=(ctx->win)&255;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void BGBBTJ_BitsLE_WriteLBits(BGBBTJ_BitStream *ctx, int v, int n)
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

void BGBBTJ_BitsLE_WriteBit(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 1); }
void BGBBTJ_BitsLE_Write2Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 2); }
void BGBBTJ_BitsLE_Write3Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 3); }
void BGBBTJ_BitsLE_Write4Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 4); }
void BGBBTJ_BitsLE_Write5Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 5); }
void BGBBTJ_BitsLE_Write6Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 6); }
void BGBBTJ_BitsLE_Write7Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteLBits(ctx, v, 7); }
// void BGBBTJ_BitsLE_Write8Bits(BGBBTJ_BitStream *ctx, int v)
//	{ BGBBTJ_BitsLE_WriteBits(ctx, v, 8); }

void BGBBTJ_BitsLE_Write12Bits(BGBBTJ_BitStream *ctx, int v)
	{ BGBBTJ_BitsLE_WriteBits(ctx, v, 12); }
// void BGBBTJ_BitsLE_Write16Bits(BGBBTJ_BitStream *ctx, int v)
//	{ BGBBTJ_BitsLE_WriteBits(ctx, v, 16); }

void BGBBTJ_BitsLE_Write8Bits(BGBBTJ_BitStream *ctx, int v)
{
	ctx->win|=(v&255)<<(ctx->pos);
	*ctx->ct++=ctx->win; ctx->win>>=8;
}

void BGBBTJ_BitsLE_Write16Bits(BGBBTJ_BitStream *ctx, int v)
{
	ctx->win|=(v&65535)<<(ctx->pos);
	*ctx->ct++=ctx->win; ctx->win>>=8;
	*ctx->ct++=ctx->win; ctx->win>>=8;
}

void BGBBTJ_BitsLE_FlushBits(BGBBTJ_BitStream *ctx)
{
	while(ctx->pos>0)
	{
		*ctx->ct++=(ctx->win)&255;
		ctx->win>>=8;
		ctx->pos-=8;
	}
}

void BGBBTJ_BC7_EncodeBlock_VecSwapRGB(int *clra, int *clrb)
{
	int t;
	t=clra[0]; clra[0]=clrb[0]; clrb[0]=t;
	t=clra[1]; clra[1]=clrb[1]; clrb[1]=t;
	t=clra[2]; clra[2]=clrb[2]; clrb[2]=t;
}

void BGBBTJ_BC7_EncodeBlock_VecSwapRGBA(int *clra, int *clrb)
{
	int t;
	t=clra[0]; clra[0]=clrb[0]; clrb[0]=t;
	t=clra[1]; clra[1]=clrb[1]; clrb[1]=t;
	t=clra[2]; clra[2]=clrb[2]; clrb[2]=t;
	t=clra[3]; clra[3]=clrb[3]; clrb[3]=t;
}

void BGBBTJ_BC7_EncodeBlock_VecSwapA(int *clra, int *clrb)
	{ int t; t=clra[3]; clra[3]=clrb[3]; clrb[3]=t; }

void BGBBTJ_BC7_EncodeBlock_VecInvertPixels(
	byte *pxv, int n, int min, int max)
{
	int i, j, k;
	
	for(i=0; i<n; i++)
		{ pxv[i]=max-(pxv[i]-min); }
}

void BTIC1H_BC7_EncodeBlock_Mode5(byte *block,
	byte *pxy, byte *pxa,
	int *min, int *max,
	int mcy, int ncy,
	int mca, int nca)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	
	BGBBTJ_BitStream bits;

	int p0, p1, p2, p3, p4, p5, p6, p7;
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
	
	BGBBTJ_BitsLE_ClearSetupWrite(&bits, block, 16);
	BGBBTJ_BitsLE_WriteBits(&bits, 32, 6);
	BGBBTJ_BitsLE_WriteBits(&bits, 0, 2);

	if(pxy[0]>=acy)
	{
		BGBBTJ_BC7_EncodeBlock_VecSwapRGB(min, max);
		BGBBTJ_BC7_EncodeBlock_VecInvertPixels(pxy, 16, mcy, ncy);
	}

	if(pxa[0]>=aca)
	{
		BGBBTJ_BC7_EncodeBlock_VecSwapA(min, max);
		BGBBTJ_BC7_EncodeBlock_VecInvertPixels(pxa, 16, mca, nca);
	}

	BGBBTJ_BitsLE_WriteBits(&bits, (min[0])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (max[0])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (min[1])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (max[1])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (min[2])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (max[2])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, min[3], 8);
	BGBBTJ_BitsLE_WriteBits(&bits, max[3], 8);

//	l0=49152/(nca-aca+1);		//Fix-Point Scale (Alpha)
	l0=32768/(nca-aca+1);		//Fix-Point Scale (Alpha)
	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l2=65536-2048;
	l3a=65536;

	p0=idxtab[((pxy[0]-acy)*l1+l3a)>>13];
	BGBBTJ_BitsLE_WriteBit(&bits, p0);
	for(i=1; i<16; i++)
	{
		p0=idxtab[((pxy[i]-acy)*l1+l3a)>>13];
		BGBBTJ_BitsLE_Write2Bits(&bits, p0);
	}

	p0=idxtab[((pxa[0]-aca)*l0+l3a)>>13];
	BGBBTJ_BitsLE_WriteBit(&bits, p0);
	for(i=1; i<16; i++)
	{
		p0=idxtab[((pxa[i]-aca)*l0+l3a)>>13];
		BGBBTJ_BitsLE_Write2Bits(&bits, p0);
	}

	BGBBTJ_BitsLE_FlushBits(&bits);
}

void BTIC1H_BC7_EncodeBlockBits48_Mode5(byte *block,
	u64 pxy, u64 pxa,
	int *min, int *max)
{
	static const char idxtab[8]=
		{ 0,0,1,1, 2,2,3,3 };
	
	BGBBTJ_BitStream bits;

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i, j, k, l;

//	return;

	BGBBTJ_BitsLE_ClearSetupWrite(&bits, block, 16);
	BGBBTJ_BitsLE_WriteBits(&bits, 32, 6);
	BGBBTJ_BitsLE_WriteBits(&bits, 0, 2);

	if(((pxy>>45)&7)>=4)
	{
		BGBBTJ_BC7_EncodeBlock_VecSwapRGB(min, max);
		pxy=~pxy;
	}

	if(((pxa>>45)&7)>=4)
	{
		BGBBTJ_BC7_EncodeBlock_VecSwapA(min, max);
		pxa=~pxa;
	}

	BGBBTJ_BitsLE_WriteBits(&bits, (min[0])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (max[0])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (min[1])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (max[1])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (min[2])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, (max[2])>>1, 7);
	BGBBTJ_BitsLE_WriteBits(&bits, min[3], 8);
	BGBBTJ_BitsLE_WriteBits(&bits, max[3], 8);

	p0=idxtab[(pxy>>45)&7];
	BGBBTJ_BitsLE_WriteBit(&bits, p0);
	for(i=1; i<16; i++)
	{
		p0=idxtab[(pxy>>(45-i*3))&7];
		BGBBTJ_BitsLE_Write2Bits(&bits, p0);
	}

	p0=idxtab[(pxa>>45)&7];
	BGBBTJ_BitsLE_WriteBit(&bits, p0);
	for(i=1; i<16; i++)
	{
		p0=idxtab[(pxa>>(45-i*3))&7];
		BGBBTJ_BitsLE_Write2Bits(&bits, p0);
	}

	BGBBTJ_BitsLE_FlushBits(&bits);
}

void BTIC1H_BC7_EncodeBlockBits32_Mode5(byte *block,
	u32 pxy, u32 pxa,
	int *min, int *max)
{
	BGBBTJ_BitStream bits;

	int p0, p1, p2, p3, p4, p5, p6, p7;
	int i, j, k, l;

	BGBBTJ_BitsLE_ClearSetupWrite(&bits, block, 16);
	BGBBTJ_BitsLE_WriteBits(&bits, 32, 6);
	BGBBTJ_BitsLE_WriteBits(&bits, 0, 2);

	if(((pxy>>30)&3)>1)
	{
		BGBBTJ_BC7_EncodeBlock_VecSwapRGB(min, max);
		pxy=~pxy;
	}

	if(((pxa>>30)&3)>1)
	{
		BGBBTJ_BC7_EncodeBlock_VecSwapA(min, max);
		pxa=~pxa;
	}

	BGBBTJ_BitsLE_Write7Bits(&bits, (min[0])>>1);
	BGBBTJ_BitsLE_Write7Bits(&bits, (max[0])>>1);
	BGBBTJ_BitsLE_Write7Bits(&bits, (min[1])>>1);
	BGBBTJ_BitsLE_Write7Bits(&bits, (max[1])>>1);
	BGBBTJ_BitsLE_Write7Bits(&bits, (min[2])>>1);
	BGBBTJ_BitsLE_Write7Bits(&bits, (max[2])>>1);
	BGBBTJ_BitsLE_Write8Bits(&bits, min[3]);
	BGBBTJ_BitsLE_Write8Bits(&bits, max[3]);

//	p0=(pxy>>30)&3;
//	BGBBTJ_BitsLE_WriteBit(&bits, p0);
//	for(i=1; i<16; i++)
//	{
//		p0=(pxy>>(30-i*2))&3;
//		BGBBTJ_BitsLE_Write2Bits(&bits, p0);
//	}

	BGBBTJ_BitsLE_Write7Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxy>>24)&255]>>1);
	BGBBTJ_BitsLE_Write8Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxy>>16)&255]);
	BGBBTJ_BitsLE_Write8Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxy>> 8)&255]);
	BGBBTJ_BitsLE_Write8Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxy    )&255]);

//	p0=(pxa>>30)&3;
//	BGBBTJ_BitsLE_WriteBit(&bits, p0);
//	for(i=1; i<16; i++)
//	{
//		p0=(pxa>>(30-i*2))&3;
//		BGBBTJ_BitsLE_Write2Bits(&bits, p0);
//	}

	BGBBTJ_BitsLE_Write7Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxa>>24)&255]>>1);
	BGBBTJ_BitsLE_Write8Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxa>>16)&255]);
	BGBBTJ_BitsLE_Write8Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxa>> 8)&255]);
	BGBBTJ_BitsLE_Write8Bits(&bits,
		btic1h_bc7_swap4x1x2[(pxa    )&255]);

	BGBBTJ_BitsLE_FlushBits(&bits);
}


void BTIC1H_DecodeBlockMB2B_RGBI(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip);

void BTIC1H_ConvBlockSpecialBC7(
	BTIC1H_Context *ctx,
	byte *iblock,
	byte *oblock, int tfl)
{
	byte tblk[16*4];
	byte yblk[16], ablk[16];
	int min[4], max[4];
	byte *cs, *ct;
	int cr, cg, cb, ca, cy;
	int mcy, ncy, mca, nca;
	int i, j, xn;

	if(tfl&1)
	{
		BTIC1H_DecodeBlockMB2B_RGBI(ctx, iblock, tblk+12*4, 4, -4*4, 0);
	}else
	{
		BTIC1H_DecodeBlockMB2B_RGBI(ctx, iblock, tblk, 4, 4*4, 0);
	}
	
	mcy=256; ncy=-1;
	mca=256; nca=-1;
	for(i=0; i<16; i++)
	{
		cr=tblk[i*4+0];
		cg=tblk[i*4+1];
		cb=tblk[i*4+2];
		ca=tblk[i*4+3];
		cy=(cr+2*cg+cb)>>2;
		yblk[i]=cy;
		ablk[i]=ca;
		if(cy<mcy)	{ mcy=cy; min[0]=cr; min[1]=cg; min[2]=cb; }
		if(cy>ncy)	{ ncy=cy; max[0]=cr; max[1]=cg; max[2]=cb; }
		if(ca<mca)	{ mca=ca; }
		if(ca>nca)	{ nca=ca; }
	}
	
	min[3]=mca;	max[3]=nca;
	BTIC1H_BC7_EncodeBlock_Mode5(oblock, yblk, ablk,
		min, max, mcy, ncy, mca, nca);
}

void BTIC1H_ConvBlockBC7(
	BTIC1H_Context *ctx,
	byte *iblock,
	byte *oblock, int flip)
{
//	byte *clr;
	int min[4], max[4];
	u64 pxb2, pxa;
	byte *ct;
	int cy, cu, cv, cd, cdu, cdv, cda;
	int cya, cyb, cua, cub, cva, cvb;
	int cy1, cu1, cv1;
	int cr, cg, cb, ca;
	int cr1, cg1, cb1, ca1;
	int cr2, cg2, cb2, ca2;
	int bt, pxb, pxb1, pxa1;
	int i0, i1, i2, i3;
	int i, j, k, l;

	i=iblock[3];
	cd=i; bt=0;
	cdu=0; cdv=0;

	ca=iblock[24];
	cda=iblock[25];
	if(cda==0)
	{
		if(iblock[27]==0)
		{
			ca1=255;	ca2=255;
			pxa=0; pxa1=0;
		}else
		{
			cda=iblock[26];
			ca1=ca-(cda>>1);	ca2=ca1+cda;
			if((ca1|ca2)>>8)
				{ ca1=clamp255(ca1); ca2=clamp255(ca2); }

			if(iblock[27]==1)
			{
				pxb=iblock[30];
				if(flip&1)
					{ pxb=((pxb>>4)&15)|((pxb<<4)&0xF0); }
				pxa1=btic1h_bc7_2x2to4x4x2[pxb];
			}else if(iblock[27]==2)
			{
				pxb=(iblock[30]<<8)|iblock[31];
				if(flip&1)
				{
					i0=btic1h_bc7_4x1x1to4x1x2[(pxb    )&15];
					i1=btic1h_bc7_4x1x1to4x1x2[(pxb>> 4)&15];
					i2=btic1h_bc7_4x1x1to4x1x2[(pxb>> 8)&15];
					i3=btic1h_bc7_4x1x1to4x1x2[(pxb>>12)&15];
				}else
				{
					i0=btic1h_bc7_4x1x1to4x1x2[(pxb>>12)&15];
					i1=btic1h_bc7_4x1x1to4x1x2[(pxb>> 8)&15];
					i2=btic1h_bc7_4x1x1to4x1x2[(pxb>> 4)&15];
					i3=btic1h_bc7_4x1x1to4x1x2[(pxb    )&15];
				}
				pxa1=(i0<<24)|(i1<<16)|(i2<<8)|(i3);
			}else if(iblock[27]==6)
			{
				if(flip&1)
				{
					pxa1=	(iblock[28]<<24)|(iblock[29]<<16)|
							(iblock[30]<< 8)|(iblock[31]    );
				}else
				{
					pxa1=	(iblock[28]<<24)|(iblock[29]<<16)|
							(iblock[30]<< 8)|(iblock[31]    );
				}
			}else
			{
				pxa1=0x33CC33CC;
			}

			i0=btic1h_bc7_4x1x2to4x1x3[(pxa1>>24)&255];
			i1=btic1h_bc7_4x1x2to4x1x3[(pxa1>>16)&255];
			i2=btic1h_bc7_4x1x2to4x1x3[(pxa1>> 8)&255];
			i3=btic1h_bc7_4x1x2to4x1x3[(pxa1    )&255];
			i=(i0<<12)|i1;		j=(i2<<12)|i3;
			pxa=(((u64)i)<<24)|j;
		}
	}else
	{
		ca1=ca-(cda>>1);	ca2=ca1+cda;
		if((ca1|ca2)>>8)
			{ ca1=clamp255(ca1); ca2=clamp255(ca2); }

		i=(iblock[26]<<16)|(iblock[27]<<8)|iblock[28];
		j=(iblock[29]<<16)|(iblock[30]<<8)|iblock[31];
		
		if(flip&1)
		{
			k=((i>>12)|(i<<12))&0xFFFFFF;
			i=((j>>12)|(j<<12))&0xFFFFFF;
			j=k;
		}
		
		pxa=(((u64)i)<<24)|j;

		i0=btic1h_bc7_4x1x3to4x1x2[(i>>12)&4095];
		i1=btic1h_bc7_4x1x3to4x1x2[(i    )&4095];
		i2=btic1h_bc7_4x1x3to4x1x2[(j>>12)&4095];
		i3=btic1h_bc7_4x1x3to4x1x2[(j    )&4095];
		pxa1=(i0<<24)|(i1<<16)|(i2<< 8)|i3;
	}
	
	if(cd==0)
	{
		if(iblock[4]==0)
		{
			cy=iblock[0];
			cu=iblock[1];
			cv=iblock[2];

			cu1=cu-128; cv1=cv-128;
			cr=(256*cy        +359*cv1+128)>>8;
			cg=(256*cy- 88*cu1-183*cv1+128)>>8;
			cb=(256*cy+454*cu1        +128)>>8;

			if((cr|cg|cb)>>8)
			{
				cr=clamp255(cr);
				cg=clamp255(cg);
				cb=clamp255(cb);
			}
			
			min[0]=cr;	min[1]=cg;	min[2]=cb;	min[3]=ca1;
			max[0]=cr;	max[1]=cg;	max[2]=cb;	max[3]=ca2;
			
			BTIC1H_BC7_EncodeBlockBits32_Mode5(oblock, 0, pxa1, min, max);
			return;
		}	

		if(iblock[4]==1)
			{ cd=iblock[5]; bt=1; }
		else if(iblock[4]==2)
			{ cd=iblock[5]; bt=2; }
		else if(iblock[4]==6)
			{ cd=iblock[5]; bt=6; }
		else if(iblock[4]==17)
		{
			BTIC1H_ConvBlockSpecialBC7(ctx, iblock, oblock, flip);
			return;
		}else if(iblock[4]==7)
		{
			BTIC1H_ConvBlockSpecialBC7(ctx, iblock, oblock, flip);
			return;
		}else if((iblock[4]==14) || (iblock[4]==15) ||
			(iblock[4]==19))
		{
			bt=iblock[4];
			cd=((iblock[5]-128)<<1);
			cdu=((iblock[8]-128)<<1);
			cdv=((iblock[9]-128)<<1);
			
			if(bt==15)bt=6;
			if(bt==19)bt=1;
		}else if((iblock[4]==20) || (iblock[4]==21) ||
			(iblock[4]==22))
		{
			BTIC1H_ConvBlockSpecialBC7(ctx, iblock, oblock, flip);
			return;
		}
		else
		{
		}
	}

	cy=iblock[0];
	cu=iblock[1];
	cv=iblock[2];
//	cd=iblock[3];

	cya=cy-(cd>>1);		cyb=cya+cd;
	cua=cu-(cdu>>1);	cub=cua+cdu;
	cva=cv-(cdv>>1);	cvb=cva+cdv;
	
	cy1=cya; cu1=cua-128; cv1=cva-128;
	cr1=(256*cy1        +359*cv1+128)>>8;
	cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
	cb1=(256*cy1+454*cu1        +128)>>8;

	cy1=cyb; cu1=cub-128; cv1=cvb-128;
	cr2=(256*cy1        +359*cv1+128)>>8;
	cg2=(256*cy1- 88*cu1-183*cv1+128)>>8;
	cb2=(256*cy1+454*cu1        +128)>>8;

	if((cr1|cg1|cb1|cr2|cg2|cb2)>>8)
	{
		cr1=clamp255(cr1);
		cg1=clamp255(cg1);
		cb1=clamp255(cb1);
		cr2=clamp255(cr2);
		cg2=clamp255(cg2);
		cb2=clamp255(cb2);
	}

	min[0]=cr1;	min[1]=cg1;	min[2]=cb1;	min[3]=ca1;
	max[0]=cr2;	max[1]=cg2;	max[2]=cb2;	max[3]=ca2;

	if(bt==0)
	{
//		i0=btic1h_bc7_4x1x2to4x1x3[iblock[4]];
//		i1=btic1h_bc7_4x1x2to4x1x3[iblock[5]];
//		i2=btic1h_bc7_4x1x2to4x1x3[iblock[6]];
//		i3=btic1h_bc7_4x1x2to4x1x3[iblock[7]];
//		i=(i0<<12)|i1;		j=(i2<<12)|i3;
//		pxb2=(((u64)i)<<24)|j;

		if(flip&1)
		{
			pxb1=	(iblock[7]<<24)|(iblock[6]<<16)|
					(iblock[5]<< 8)|(iblock[4]    );
		}else
		{
			pxb1=	(iblock[4]<<24)|(iblock[5]<<16)|
					(iblock[6]<< 8)|(iblock[7]    );
		}

		BTIC1H_BC7_EncodeBlockBits32_Mode5(oblock, pxb1, pxa1, min, max);
		return;
	}

	if(bt==1)
	{
		pxb=iblock[6];

		if(flip&1)
			{ pxb=((pxb>>4)&15)|((pxb<<4)&0xF0); }

		pxb1=btic1h_bc7_2x2to4x4x2[pxb];

		BTIC1H_BC7_EncodeBlockBits32_Mode5(oblock, pxb1, pxa1, min, max);
		return;
	}
	
	if(bt==2)
	{
		pxb=(iblock[6]<<8)|iblock[7];
//		i0=btic1h_bc7_4x1x1to4x1x3[(pxb>>12)&15];
//		i1=btic1h_bc7_4x1x1to4x1x3[(pxb>> 8)&15];
//		i2=btic1h_bc7_4x1x1to4x1x3[(pxb>> 4)&15];
//		i3=btic1h_bc7_4x1x1to4x1x3[(pxb    )&15];
//		i=(i0<<12)|i1;		j=(i2<<12)|i3;
//		pxb2=(((u64)i)<<24)|j;
//		BTIC1H_BC7_EncodeBlockBits_Mode5(oblock, pxb2, pxa, min, max);

		if(flip&1)
		{
			i0=btic1h_bc7_4x1x1to4x1x2[(pxb    )&15];
			i1=btic1h_bc7_4x1x1to4x1x2[(pxb>> 4)&15];
			i2=btic1h_bc7_4x1x1to4x1x2[(pxb>> 8)&15];
			i3=btic1h_bc7_4x1x1to4x1x2[(pxb>>12)&15];
		}else
		{
			i0=btic1h_bc7_4x1x1to4x1x2[(pxb>>12)&15];
			i1=btic1h_bc7_4x1x1to4x1x2[(pxb>> 8)&15];
			i2=btic1h_bc7_4x1x1to4x1x2[(pxb>> 4)&15];
			i3=btic1h_bc7_4x1x1to4x1x2[(pxb    )&15];
		}
		pxb1=(i0<<24)|(i1<<16)|(i2<<8)|(i3);
		BTIC1H_BC7_EncodeBlockBits32_Mode5(oblock, pxb1, pxa1, min, max);
		return;
	}
	
	if(bt==6)
	{
		i=(iblock[10]<<16)|(iblock[11]<<8)|iblock[12];
		j=(iblock[13]<<16)|(iblock[14]<<8)|iblock[15];
		
		if(flip&1)
		{
			k=((i>>12)|(i<<12))&0xFFFFFF;
			i=((j>>12)|(j<<12))&0xFFFFFF;
			j=k;
		}
		
		pxb2=(((s64)i)<<24)|j;
		BTIC1H_BC7_EncodeBlockBits48_Mode5(oblock, pxb2, pxa, min, max);
		return;
	}
	
	if(bt==14)
	{
//		i0=btic1h_bc7_4x1x2to4x1x3[iblock[12]];
//		i1=btic1h_bc7_4x1x2to4x1x3[iblock[13]];
//		i2=btic1h_bc7_4x1x2to4x1x3[iblock[14]];
//		i3=btic1h_bc7_4x1x2to4x1x3[iblock[15]];
//		i=(i0<<12)|i1;		j=(i2<<12)|i3;
//		pxb2=(((u64)i)<<24)|j;
//		BTIC1H_BC7_EncodeBlockBits_Mode5(oblock, pxb2, pxa, min, max);

		if(flip&1)
		{
			pxb1=	(iblock[15]<<24)|(iblock[14]<<16)|
					(iblock[13]<< 8)|(iblock[12]    );
		}else
		{
			pxb1=	(iblock[12]<<24)|(iblock[13]<<16)|
					(iblock[14]<< 8)|(iblock[15]    );
		}
		BTIC1H_BC7_EncodeBlockBits32_Mode5(oblock, pxb1, pxa1, min, max);
		return;
	}
}

void BTIC1H_ConvImageBC7_I(
	BTIC1H_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int oblkstr, int xs, int ys)
{
	int xs1, ys1;
	int i, j, k0, k1, tfl;

	BTIC1H_ConvBlockBC7_Init();
	
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
		BTIC1H_ConvBlockBC7(
			ctx,
			iblock+k0*iblkstr,
			oblock+k1*oblkstr,
			tfl);
	}
}

void BTIC1H_ConvImageBC7n(
	BTIC1H_Context *ctx,
	byte *iblock, int iblkstr,
	byte *oblock, int bcn, int xs, int ys)
{
	BTIC1H_ConvImageBC7_I(ctx, iblock, iblkstr, oblock, 16, xs, ys);
}



void BGBBTJ_BitsLE_SetupRead(BGBBTJ_BitStream *ctx, byte *cs, int sz)
{
	ctx->cs=cs;
	ctx->cse=cs+sz;
//	ctx->win=0;
	ctx->pos=0;
	
	ctx->win=ctx->cs[0] | (ctx->cs[1]<<8) |
		(ctx->cs[2]<<16) | (ctx->cs[3]<<24);
	ctx->cs+=4;
}

void BGBBTJ_BitsLE_ClearSetupRead(BGBBTJ_BitStream *ctx, byte *ct, int sz)
{
	BGBBTJ_BitsLE_Clear(ctx);
	BGBBTJ_BitsLE_SetupRead(ctx, ct, sz);
}

int BGBBTJ_BitsLE_ReadBits(BGBBTJ_BitStream *ctx, int n)
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

int BGBBTJ_BitsLE_ReadBit(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 1)); }
int BGBBTJ_BitsLE_Read2Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 2)); }
int BGBBTJ_BitsLE_Read3Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 3)); }
int BGBBTJ_BitsLE_Read4Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 4)); }
int BGBBTJ_BitsLE_Read5Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 5)); }
int BGBBTJ_BitsLE_Read6Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 6)); }
int BGBBTJ_BitsLE_Read7Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 7)); }
int BGBBTJ_BitsLE_Read8Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 8)); }

int BGBBTJ_BitsLE_Read14Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 14)); }
int BGBBTJ_BitsLE_Read16Bits(BGBBTJ_BitStream *ctx)
	{ return(BGBBTJ_BitsLE_ReadBits(ctx, 16)); }

int BGBBTJ_BitsLE_Peek8Bits(BGBBTJ_BitStream *ctx)
{
	int v;
	v=(ctx->win>>ctx->pos)&255;
	return(v);
}

int bgbbtj_bc7_weights2[4]=
	{ 0, 21, 43, 64};
int bgbbtj_bc7_weights3[8]=
	{ 0,  9, 18, 27, 37, 46, 55, 64};
int bgbbtj_bc7_weights4[16]= 
	{ 0,  4,  9, 13, 17, 21, 26, 30,
	 34, 38, 43, 47, 51, 55, 60, 64};

int bgbbtj_bc7_interpolate(int e0, int e1, int idx, int prec)
{
	int i, w;

	switch(prec)
	{
	case 2:
		w=bgbbtj_bc7_weights2[idx];
		i=(((64-w)*e0+w*e1+32)>>6);
		break;
	case 3:
		w=bgbbtj_bc7_weights3[idx];
		i=(((64-w)*e0+w*e1+32)>>6);
		break;
	case 4:
		w=bgbbtj_bc7_weights4[idx];
		i=(((64-w)*e0+w*e1+32)>>6);
		break;
	default:
		i=0; break;
	}
	return(i);
}

void BGBBTJ_BC7_DecodeBlock_Mode5(
	BGBBTJ_BitStream *bits,
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
	
	rot=BGBBTJ_BitsLE_Read2Bits(bits);
	p0=BGBBTJ_BitsLE_Read14Bits(bits);
	p1=BGBBTJ_BitsLE_Read14Bits(bits);
	p2=BGBBTJ_BitsLE_Read14Bits(bits);
	p3=BGBBTJ_BitsLE_Read16Bits(bits);
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
		clrb[i*4+0]=bgbbtj_bc7_interpolate(r0, r1, i, 2);
		clrb[i*4+1]=bgbbtj_bc7_interpolate(g0, g1, i, 2);
		clrb[i*4+2]=bgbbtj_bc7_interpolate(b0, b1, i, 2);
		clrb[i*4+3]=bgbbtj_bc7_interpolate(a0, a1, i, 2);
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
			{ p=BGBBTJ_BitsLE_ReadBits(bits, 31); }
	}else
	{
		p=BGBBTJ_BitsLE_Read7Bits(bits);
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
			p=BGBBTJ_BitsLE_Read8Bits(bits);
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
		p=BGBBTJ_BitsLE_Read7Bits(bits);
		p0=(p&1)*4;			p1=((p>>1)&3)*4;
		p2=((p>>3)&3)*4;	p3=((p>>5)&3)*4;
		i0=0*4;		i1=1*4;
		i2=2*4;		i3=3*4;
		blkb[i0+3]=clrb[p0+3];	blkb[i1+3]=clrb[p1+3];
		blkb[i2+3]=clrb[p2+3];	blkb[i3+3]=clrb[p3+3];
		for(i=1; i<4; i++)
		{
			p=BGBBTJ_BitsLE_Read8Bits(bits);
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

int BGBBTJ_BC7_GetBlockMode(byte *block)
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

int BGBBTJ_BC7_DecodeBlock_ReadMode(BGBBTJ_BitStream *bits)
{
	int i, j;

	i=BGBBTJ_BitsLE_Peek8Bits(bits);
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
		BGBBTJ_BitsLE_ReadBits(bits, j+1);
		return(j);
	}
	return(8);
}

void BGBBTJ_BC7_DecodeBlock(byte *block,
	byte *rgba, int xstride, int ystride, int flags)
{
	BGBBTJ_BitStream bits;
	int mode;

	BGBBTJ_BitsLE_ClearSetupRead(&bits, block, 16);
	mode=BGBBTJ_BC7_DecodeBlock_ReadMode(&bits);
	switch(mode)
	{
	case 0:
//		BGBBTJ_BC7_DecodeBlock_Mode0(&bits, rgba, xstride, ystride, flags);
		break;
	case 1:
//		BGBBTJ_BC7_DecodeBlock_Mode1(&bits, rgba, xstride, ystride, flags);
		break;
	case 2:
//		BGBBTJ_BC7_DecodeBlock_Mode2(&bits, rgba, xstride, ystride, flags);
		break;
	case 3:
//		BGBBTJ_BC7_DecodeBlock_Mode3(&bits, rgba, xstride, ystride, flags);
		break;
	case 4:
//		BGBBTJ_BC7_DecodeBlock_Mode4(&bits, rgba, xstride, ystride, flags);
		break;
	case 5:
		BGBBTJ_BC7_DecodeBlock_Mode5(&bits, rgba, xstride, ystride, flags);
		break;
	case 6:
//		BGBBTJ_BC7_DecodeBlock_Mode6(&bits, rgba, xstride, ystride, flags);
		break;
	case 7:
//		BGBBTJ_BC7_DecodeBlock_Mode7(&bits, rgba, xstride, ystride, flags);
		break;
	default:
//		BGBBTJ_BC7_DecodeBlock_Default(&bits, rgba, xstride, ystride, flags);
		break;
	}
}

void BTIC1H_BC7_DecodeImage(byte *block,
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
		BGBBTJ_BC7_DecodeBlock(
			block+(i*xs2+j)*16,
			rgba1+i*4*ystr+j*4*xstr,
			xstr, ystr, pfb);
	}
}
