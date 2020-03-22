#ifdef _BGBCC
__asm {
//R4=Parm
//R5=dstc
//R6=dstz
//R7=cnt
TKGL_DrawSpan_DirClr:
	MOV.Q	(R4, TKGL_DS_CPOS*8), R22
	MOV.Q	(R4, TKGL_DS_CSTEP*8), R23
	LEA.W	(R5, R7), R20
	.L0:
	RGB5PCK64	R22, R2
	ADD			R23, R22
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPGT		R5, R20
	BT			.L0
	RTSU
};
#else
void TKGL_DrawSpan_DirClr(u64 *parm,
	tkgl_rastpixel *dstc, tkgl_zbufpixel *dstz, int cnt)
{
	tkgl_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u64	cpos, cstep;
	u32 xmask, ymask;
	int pix, idx;

//	cpos=ctx->ds_cpos;
//	cstep=ctx->ds_cstep;
	cpos=parm[TKGL_DS_CPOS];
	cstep=parm[TKGL_DS_CSTEP];

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
#endif

#ifdef _BGBCC
__asm {
TKGL_DrawSpan_DirTex:
	PUSH.X	R30
	PUSH.X	R28
	MOV.Q	(R4, TKGL_DS_TEXIMG*8), R20
	MOV.Q	(R4, TKGL_DS_TPOS*8), R28
	MOV.Q	(R4, TKGL_DS_TSTEP*8), R29
	MOV.Q	(R4, TKGL_DS_XMASK*8), R30
	MOV.Q	(R4, TKGL_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	SHLD.Q	R28, -16, R3	|	SHLD.Q		R28, -32, R2
	AND		R3, R30, R3		|	AND			R2, R31, R2
	ADD		R29, R28		|	OR			R2, R3
	MOV.W	(R20, R3), R2

	MOV.W	R2, (R5)
	ADD		2, R5
	CMPGT	R5, R21
	BT		.L0

	POP.X	R28
	POP.X	R30
	RTSU
};
#else
void TKGL_DrawSpan_DirTex(u64 *parm,
	tkgl_rastpixel *dstc, tkgl_zbufpixel *dstz, int cnt)
{
	tkgl_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u32 xmask, ymask;
	int pix, idx;

//	pos=ctx->ds_pos;
//	step=ctx->ds_step;
	pos=parm[TKGL_DS_TPOS];
	step=parm[TKGL_DS_TSTEP];

//	src=ctx->tex_img;
	src=(tkgl_rastpixel *)(parm[TKGL_DS_TEXIMG]);
//	xmask=ctx->tex_xmask;
//	ymask=ctx->tex_ymask;

	xmask=parm[TKGL_DS_XMASK];
	ymask=parm[TKGL_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
		pix=src[idx];
		*ct++=pix;
		pos+=step;
	}
}
#endif

/*
TKGL_DrawSpan_ModTex: Color Modulated Texture
*/

#ifdef _BGBCC
__asm {
TKGL_DrawSpan_ModTex:
	PUSH.X	R30
	PUSH.X	R28
	PUSH.X	R26
	MOV.Q	(R4, TKGL_DS_TEXIMG*8), R20

	MOV.Q	(R4, TKGL_DS_CPOS*8), R26
	MOV.Q	(R4, TKGL_DS_CSTEP*8), R27
	MOV.Q	(R4, TKGL_DS_TPOS*8), R28
	MOV.Q	(R4, TKGL_DS_TSTEP*8), R29
	MOV.Q	(R4, TKGL_DS_XMASK*8), R30
	MOV.Q	(R4, TKGL_DS_YMASK*8), R31
	LEA.W	(R5, R7), R21

	.L0:	
	SHLD.Q		R28, -16, R3	|	SHLD.Q		R28, -32, R2
	AND			R3, R30, R3		|	AND			R2, R31, R2
	ADD			R29, R28		|	OR			R2, R3
	MOV.W		(R20, R3), R2
	RGB5UPCK64	R2, R2
	PMULU.HW	R2, R26, R2
	ADD			R27, R26		| RGB5PCK64		R2, R2
	MOV.W		R2, (R5)
	ADD			2, R5
	CMPGT		R5, R21
	BT			.L0

	POP.X	R26
	POP.X	R28
	POP.X	R30
	RTSU
};
#else

u64 tkgl_pmuluhw(u64 a, u64 b)
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
	
	c=	(((u64)cx)<< 0) | (((u64)cy)<<16) |
		(((u64)cz)<<32) | (((u64)cw)<<48) ;
	return(c);
}

u64 tkgl_rgbupck64(u16 a)
{
	int ax, ay, az, aw, av;
	u64 c;

	av=(a&0x0001)|((a&0x0020)>>5)|((a&0x0400)>>10);
	av=av<<13;
	ax=(a&0x001F)<<11;
	ay=(a&0x03E0)<< 6;
	az=(a&0x7C00)<< 1;
	aw=(a&0x8000)?av:0xFFFF;
	c=	(((u64)ax)<< 0) | (((u64)ay)<<16) |
		(((u64)az)<<32) | (((u64)aw)<<48) ;
	return(c);
	
}

u16 tkgl_rgbpck64(u64 a)
{
	int av;
	u16 c;

	av=(a>>60)&16;
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

void TKGL_DrawSpan_ModTex(u64 *parm,
	tkgl_rastpixel *dstc, tkgl_zbufpixel *dstz, int cnt)
{
	tkgl_rastpixel *ct, *cte, *src;
	u64	pos, step;
	u64	cpos, cstep;
	u64 cval;
	u32 xmask, ymask;
	int pix, clr, idx;

	pos=parm[TKGL_DS_TPOS];
	step=parm[TKGL_DS_TSTEP];

	cpos=parm[TKGL_DS_CPOS];
	cstep=parm[TKGL_DS_CSTEP];

	src=(tkgl_rastpixel *)(parm[TKGL_DS_TEXIMG]);
	xmask=parm[TKGL_DS_XMASK];
	ymask=parm[TKGL_DS_YMASK];

	ct=dstc; cte=ct+cnt;
	while(ct<cte)
	{
		idx=((pos>>32)&ymask)|((pos>>16)&xmask);
		pix=src[idx];

		cval=tkgl_rgbupck64(pix);
		cval=tkgl_pmuluhw(cval, cpos);
		pix=tkgl_rgbpck64(cval);

		*ct++=pix;
		pos+=step;
		cpos+=cstep;
	}
}
#endif
