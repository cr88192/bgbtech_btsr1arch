#ifndef __BJX2__

const u16 pmorttab[256] = {
0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,
0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,
0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,
0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,
0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,
0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015,
0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115,
0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415,
0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515,
0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015,
0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115,
0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415,
0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515,
0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015,
0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115,
0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415,
0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515,
0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555 };


int tkra_morton8(int x, int y)
{
//	return(pmorttab[x]|(pmorttab[y]<<1));
	return(pmorttab[x&255]|(pmorttab[y&255]<<1));
}

int tkra_morton16(int x, int y)
{
//	return(pmorttab[x]|(pmorttab[y]<<1));
	return(
		(pmorttab[(x   )&255]    )|(pmorttab[(y   )&255]<< 1)|
		(pmorttab[(x>>8)&255]<<16)|(pmorttab[(y>>8)&255]<<17));
}


u64 tkra_paddhw(u64 a, u64 b)
{
	return((a&0xFFFEFFFEFFFEFFFEULL)+(b&0xFFFEFFFEFFFEFFFEULL));
}

u64 tkra_pmuluhw(u64 a, u64 b)
{
	int ax, ay, az, aw;
	int bx, by, bz, bw;
	int cx, cy, cz, cw;
	u64 c;
	
	ax=(u16)(a>> 0);	ay=(u16)(a>>16);
	az=(u16)(a>>32);	aw=(u16)(a>>48);
	bx=(u16)(b>> 0);	by=(u16)(b>>16);
	bz=(u16)(b>>32);	bw=(u16)(b>>48);
	cx=(ax*bx)>>16;		cy=(ay*by)>>16;
	cz=(az*bz)>>16;		cw=(aw*bw)>>16;
	cx=(u16)cx;			cy=(u16)cy;
	cz=(u16)cz;			cw=(u16)cw;
	c=	(((u64)cx)<< 0) | (((u64)cy)<<16) |
		(((u64)cz)<<32) | (((u64)cw)<<48) ;
	return(c);
}

u64 tkra_padduhw_sat(u64 a, u64 b)
{
	u64 c;
	
//	c=	((a&0xFFFEFFFEFFFEFFFEULL)>>1) +
//		((b&0xFFFEFFFEFFFEFFFEULL)>>1) ;
	c=	((a>>1)&0x7FFE7FFE7FFE7FFEULL) +
		((b>>1)&0x7FFE7FFE7FFE7FFEULL) ;

	if(c&0x8000800080008000ULL)
	{
		if(	c& 0x8000000000000000ULL)
			c|=0xFFFF000000000000ULL;
		if(	c& 0x0000800000000000ULL)
			c|=0x0000FFFF00000000ULL;
		if(	c& 0x0000000080000000ULL)
			c|=0x00000000FFFF0000ULL;
		if(	c& 0x0000000000008000ULL)
			c|=0x000000000000FFFFULL;
	}

	c<<=1;
	
	return(c);
}

u64 tkra_rgbupck64(u16 a)
{
	int ax, ay, az, aw, av;
	u64 c;

//	av=(a&0x0001)|((a&0x0020)>>5)|((a&0x0400)>>10);
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
	av=av<<13;
	ax=(a&0x001F)<<11;
	ay=(a&0x03E0)<< 6;
	az=(a&0x7C00)<< 1;
	
//	av|=av>>3;
	ax|=ax>>5;
	ay|=ay>>5;
	az|=az>>5;
	
	aw=(a&0x8000)?av:0xFFFF;
	c=	(((u64)ax)<< 0) | (((u64)ay)<<16) |
		(((u64)az)<<32) | (((u64)aw)<<48) ;
	return(c);
	
}

u16 tkra_rgbpck64(u64 a)
{
	int av;
	u16 c;

	av=(a>>60)&15;
	if(av==15)
	{
		c=	((a>>33)&0x7C00) |
			((a>>22)&0x03E0) |
			((a>>11)&0x001F);
	}else
	{
		c=	((a>>33)&0x7800) |
			((a>>22)&0x03C0) |
			((a>>11)&0x001E) |
			0x8000;
		if(av&8)c|=0x0400;
		if(av&4)c|=0x0020;
		if(av&2)c|=0x0001;
	}
	return(c);
}

void TKRA_DrawSpan_DirClr(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u64	cpos, cstep;
	u32 xmask, ymask;
	int pix, idx;

//	cpos=ctx->ds_cpos;
//	cstep=ctx->ds_cstep;
	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		pix=src[idx];
		pix=
			((cpos>>33)&0x7C00) |
			((cpos>>22)&0x03E0) |
			((cpos>>11)&0x001F);
		*ct++=pix;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_DirTex(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u32 xmask, ymask;
	int pix, idx;

//	pos=ctx->ds_pos;
//	step=ctx->ds_step;
	pos=parm[TKRA_DS_TPOS];
	step=parm[TKRA_DS_TSTEP];

//	src=ctx->tex_img;
	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
//	xmask=ctx->tex_xmask;
//	ymask=ctx->tex_ymask;

	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8()
		pix=src[idx];
		*ct++=pix;
		pos+=step;
	}
}

void TKRA_DrawSpan_DirTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u32 xmask, ymask;
	int pix, idx;

//	pos=ctx->ds_pos;
//	step=ctx->ds_step;
	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

//	src=ctx->tex_img;
	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
//	xmask=ctx->tex_xmask;
//	ymask=ctx->tex_ymask;

	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=tkra_morton8(((pos>>16)&xmask), ((pos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		*ct++=pix;
		tpos+=tstep;
	}
}

void TKRA_DrawSpan_ModTex(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u64	cpos, cstep;
	u64 cval;
	u32 xmask, ymask;
	int pix, clr, idx;

	pos=parm[TKRA_DS_TPOS];
	step=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		pos+=step;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_ModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval;
	u32 xmask, ymask;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_AlphaModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
		dpix=ct[0];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);

		dval=tkra_rgbupck64(dpix);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;

		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_AtestModTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
//		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
//		idx=tkra_morton8(((tpos>>16)&xmask), ((tpos>>48)&xmask));
//		idx=tkra_morton8(tpos>>16, tpos>>48)&ymask;
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		pix=src[idx];
//		dpix=ct[0];

		cval=tkra_rgbupck64(pix);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

//		dval=tkra_rgbupck64(dpix);

		if((cval>>63)&1)
			*ct=pix;

		ct++;
		tpos+=tstep;
		cpos+=cstep;
	}
}

u64 TKRA_InterpBilinear(
	u16 px0, u16 px1, u16 px2, u16 px3,
	u16 xfrac, u16 yfrac);
u64 TKRA_InterpBilinear64(
	u64 px0, u64 px1, u64 px2, u64 px3,
	u16 xfrac, u16 yfrac);

u64 TKRA_InterpLinear(u16 px0, u16 px1, u16 frac);
u64 TKRA_InterpLinear64(u64 px0, u64 px1, u16 frac);

u64 TKRA_InterpBilinear3Pt_64(
	u64 px0, u64 px1, u64 px2,
	u16 xfrac, u16 yfrac);


void TKRA_DrawSpan_ModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
//		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		pix=tkra_rgbpck64(cval);

		*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
//		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AtestModBlTexMort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int pix0, pix1, pix2, pix3;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXIMG]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		pix0=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask];
		pix1=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask];
		pix2=src[tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask];
		pix3=src[tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask];
		cval=TKRA_InterpBilinear(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		if((cval>>63)&1)
			*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
//		zpos+=zstep;
	}
}


u64 tkra_blkutx2(u64 blk, int ix)
{
	int pxa, pxb, pxv;
	int axa, axb, axc;
	u64 clra, clrb, clrc;
	
	pxa=(u16)(blk>> 0);
	pxb=(u16)(blk>>16);
	pxv=blk>>32;
	clra=tkra_rgbupck64(pxa);
	clrb=tkra_rgbupck64(pxb);

	if(pxb&0x8000)
	{
		axa=TKRA_GetPixel555Alpha(pxa);
		axb=TKRA_GetPixel555Alpha(pxb);
		axa=axa<<8;
		axb=axb<<8;

		clrc= ((pxv>>(ix*2+1))&1)?clra:clrb;
		axc = ((pxv>>(ix*2+0))&1)?axa:axb;
		clrc&=0x0000FFFFFFFFFFFFULL;
		clrc|=((u64)axc)<<48;
	}else
	{
		clrc=(pxv&(1<<(ix*2+1)))?clra:clrb;
	}

	return(clrc);
}

int		 tkra_utx2_cachedindx[64];
void	*tkra_utx2_cachedblka[64];
u64		 tkra_utx2_cachedpels[64][16];

u64 TKRA_CachedBlkUtx2(void *src, int ix)
{
	u64 *blka;
	u64 tca[4];
	int pxa, pxb, pxv;
	int axa, axb, axc;
	u64 blk;
	u64 clra, clrb, clrc, clrd, clrp, clrq;
	
	int ix0, ix1, hxi;
	int i;

	if(!src)
	{
//		__debugbreak();
		return(0);
	}

	ix0=ix&15;
	ix1=ix>>4;
//	hxi=((ix1*251)>>8)&255;
	hxi=(ix1^(ix1>>6))&63;
	
	if(	(tkra_utx2_cachedindx[hxi]==ix1) &&
		(tkra_utx2_cachedblka[hxi]==src))
	{
		return(tkra_utx2_cachedpels[hxi][ix0]);
	}

	blka=src;
	blk=blka[ix1];

	pxa=(u16)(blk>> 0);
	pxb=(u16)(blk>>16);
	pxv=blk>>32;
	clra=tkra_rgbupck64(pxa);
	clrb=tkra_rgbupck64(pxb);

	clrc=tkra_pmuluhw(clra, 0xAAAAAAAAAAAAAAAAULL);
	clrd=tkra_pmuluhw(clrb, 0xAAAAAAAAAAAAAAAAULL);
	clrp=tkra_pmuluhw(clra, 0x5555555555555555ULL);
	clrq=tkra_pmuluhw(clrb, 0x5555555555555555ULL);
	clrc+=clrq;
	clrd+=clrp;

	tca[0]=clrb;
	tca[1]=clrd;
	tca[2]=clrc;
	tca[3]=clra;

	if((pxa&0x8000) && (pxb&0x8000))
	{
		for(i=0; i<16; i++)
		{
//			clrp=((pxv>>(i*2+1))&1)?clra:clrb;
			clrp=tca[(pxv>>(i*2))&3];
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}else
		if(pxb&0x8000)
	{
		axa=TKRA_GetPixel444A3_Alpha(pxa);
		axb=TKRA_GetPixel444A3_Alpha(pxb);
		axa=axa<<8;
		axb=axb<<8;

		for(i=0; i<16; i++)
		{
			clrp= ((pxv>>(i*2+1))&1)?clra:clrb;
			axc = ((pxv>>(i*2+0))&1)?axa:axb;
			clrp&=0x0000FFFFFFFFFFFFULL;
			clrp|=((u64)axc)<<48;
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}else
	{
		for(i=0; i<16; i++)
		{
//			clrp=((pxv>>(i*2+1))&1)?clra:clrb;
			clrp=tca[(pxv>>(i*2))&3];
			tkra_utx2_cachedpels[hxi][i]=clrp;
		}
	}

	tkra_utx2_cachedindx[hxi]=ix1;
	tkra_utx2_cachedblka[hxi]=src;
	return(tkra_utx2_cachedpels[hxi][ix0]);
}

int		 tkra_utx3_cachedindx[64];
void	*tkra_utx3_cachedblka[64];
u64		 tkra_utx3_cachedpels[64][16];

u64 TKRA_CachedBlkUtx3(void *src, int ix)
{
	u64 *blka;
	u64 tca[4];
	int pxa, pxb, pxvy, pxva;
	int axa, axb, axc;
	u64 blk0, blk1;
	u64 clra, clrb, clrc, clrd, clrp, clrq;
	
	int ix0, ix1, hxi;
	int i;

	if(!src)
	{
		return(0);
	}

	ix0=ix&15;
	ix1=ix>>4;
	hxi=(ix1^(ix1>>6))&63;
	
	if(	(tkra_utx3_cachedindx[hxi]==ix1) &&
		(tkra_utx3_cachedblka[hxi]==src))
	{
		return(tkra_utx3_cachedpels[hxi][ix0]);
	}

	blka=src;
	blk0=blka[ix1*2+0];
	blk1=blka[ix1*2+1];

	pxa=(u32)(blk0>> 0);
	pxb=(u32)(blk0>>32);
	pxvy=blk1>> 0;
	pxva=blk1>>32;
	clra=tkra_rgba32upck64(pxa);
	clrb=tkra_rgba32upck64(pxb);
	
//	clra|=0xFFFF000000000000ULL;
//	clrb|=0xFFFF000000000000ULL;

	clrc=tkra_pmuluhw(clra, 0xAAAAAAAAAAAAAAAAULL);
	clrd=tkra_pmuluhw(clrb, 0xAAAAAAAAAAAAAAAAULL);
	clrp=tkra_pmuluhw(clra, 0x5555555555555555ULL);
	clrq=tkra_pmuluhw(clrb, 0x5555555555555555ULL);
	clrc+=clrq;
	clrd+=clrp;

	tca[0]=clrb;
	tca[1]=clrd;
	tca[2]=clrc;
	tca[3]=clra;

	for(i=0; i<16; i++)
	{
		clrp=tca[(pxvy>>(i*2))&3];
		clrq=tca[(pxva>>(i*2))&3];
		clrp=	(clrp&0x0000FFFFFFFFFFFFULL) |
				(clrq&0xFFFF000000000000ULL) ;
		tkra_utx3_cachedpels[hxi][i]=clrp;
	}

	tkra_utx3_cachedindx[hxi]=ix1;
	tkra_utx3_cachedblka[hxi]=src;
	return(tkra_utx3_cachedpels[hxi][ix0]);
}

void TKRA_DrawSpan_ModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, blk;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;

//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_AlphaModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 cval, dval, amod, anmod;
	u64 blk;
	u32 xmask, ymask;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;
		dpix=ct[0];

//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);

		cval=tkra_pmuluhw(cval, cpos);

		dval=tkra_rgbupck64(dpix);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;

		pix=tkra_rgbpck64(cval);

		*ct++=pix;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_AtestModUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	u64 *src;
	tkra_rastpixel *ct, *cte;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, blk;
	u32 xmask, ymask;
	u32 z;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	src=(u64 *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		idx=tkra_morton16(tpos>>16, tpos>>48)&ymask;

//		cval=tkra_blkutx2(blk, idx&15);
		cval=TKRA_CachedBlkUtx2(src, idx);
		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		if((cval>>63)&1)
			*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
	}
}

void TKRA_DrawSpan_ModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		ix0=tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask;
		ix1=tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask;
		ix2=tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask;
		ix3=tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask;
		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		pix3=TKRA_CachedBlkUtx2(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
//		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AlphaModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval, dval, amod, anmod;
	u32 xmask, ymask;
	u32 z;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, dpix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		ix0=tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask;
		ix1=tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask;
		ix2=tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask;
		ix3=tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask;
		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		pix3=TKRA_CachedBlkUtx2(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		dpix=*ct;

		dval=tkra_rgbupck64(dpix);
		cval=tkra_pmuluhw(cval, cpos);
		
		amod=(cval>>48)&65535;
		amod|=amod<<16;
		amod|=amod<<32;
		anmod=~amod;

		cval=tkra_pmuluhw(cval, amod);
		dval=tkra_pmuluhw(dval, anmod);
		cval+=dval;
		
		pix=tkra_rgbpck64(cval);

		*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
//		zpos+=zstep;
	}
}

void TKRA_DrawSpan_AtestModBlUtx2Mort(u64 *parm,
	tkra_rastpixel *dstc, tkra_zbufpixel *dstz, int cnt)
{
	tkra_rastpixel *ct, *cte, *src;
	tkra_zbufpixel *ctz;
	u64	tpos, tstep;
	u64	cpos, cstep;
	u64 zpos, zstep;
	u64 cval;
	u32 xmask, ymask;
	u32 z;
	int ix0, ix1, ix2, ix3;
	u64 pix0, pix1, pix2, pix3;
	int pix, clr, idx;

	tpos=parm[TKRA_DS_TPOS];
	tstep=parm[TKRA_DS_TSTEP];

	cpos=parm[TKRA_DS_CPOS];
	cstep=parm[TKRA_DS_CSTEP];

	zpos=parm[TKRA_DS_ZPOS];
	zstep=parm[TKRA_DS_ZSTEP];

	src=(tkra_rastpixel *)(parm[TKRA_DS_TEXBCN]);
	xmask=parm[TKRA_DS_XMASK];
	ymask=parm[TKRA_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	ctz=dstz;
	while(ct<cte)
	{
		ix0=tkra_morton16((tpos>>16)+0, (tpos>>48)+0)&ymask;
		ix1=tkra_morton16((tpos>>16)+1, (tpos>>48)+0)&ymask;
		ix2=tkra_morton16((tpos>>16)+0, (tpos>>48)+1)&ymask;
		ix3=tkra_morton16((tpos>>16)+1, (tpos>>48)+1)&ymask;
		pix0=TKRA_CachedBlkUtx2(src, ix0);
		pix1=TKRA_CachedBlkUtx2(src, ix1);
		pix2=TKRA_CachedBlkUtx2(src, ix2);
		pix3=TKRA_CachedBlkUtx2(src, ix3);
		cval=TKRA_InterpBilinear64(pix0, pix1, pix2, pix3,
			(u16)tpos, (u16)(tpos>>32));

		cval=tkra_pmuluhw(cval, cpos);
		pix=tkra_rgbpck64(cval);

		if((cval>>63)&1)
			*ct=pix;
		ct++;
		tpos+=tstep;
		cpos+=cstep;
//		zpos+=zstep;
	}
}
#endif
