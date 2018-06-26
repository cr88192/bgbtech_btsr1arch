/*
static inlines for BGBRASW.
*/

extern u32 bgbrasw_divtab_s32_u16[65536];

static inline s32 bgbrasw_fixdiv_s32_u16(s32 a, u16 b)
{
#ifdef _M_IX86
//	u32 c, d;
//	c=bgbrasw_divtab_s32_u16[b];
	__asm {
		mov eax, a
//		cmp eax, 2
//		jb lbl_out
		movzx edx, b
		mov ecx, [bgbrasw_divtab_s32_u16+edx*4]
		imul ecx
		mov eax, edx
//		lbl_out:
	}
//	return(d);
#else
	return((((s64)a)*bgbrasw_divtab_s32_u16[b])>>32);
#endif
}

static inline u32 bgbrasw_colormul(u32 a, u32 b)
{
// #if defined(_M_IX86) && (_M_IX86_FP>=2)
#if 0
//	byte mask0[16]={
//		255, 255, 255, 255, 255, 255, 255, 255,
//		255,   3, 255,   2, 255,   1, 255,   0};
//	byte mask1[16]={
//		255, 255, 255, 255, 255, 255, 255, 255,
//		255, 255, 255, 255,   7,   5,   3,   1};
	u64 mask0[2]={0xFF03FF02FF01FF00ULL, 0xFFFFFFFFFFFFFFFFULL};
	u64 mask1[2]={0xFFFFFFFF07050301ULL, 0xFFFFFFFFFFFFFFFFULL};
	u64 mask2[2]={0x0000000000000000ULL, 0x0000000000000000ULL};

	__asm {
		movd xmm0, [a]
		movd xmm1, [b]
	    pshufb xmm0, [mask0]
		pshufb xmm1, [mask0]
//		punpcklbw xmm0, [mask2]
//		punpcklbw xmm1, [mask2]
		pmullw xmm0, xmm1
		pshufb xmm0, [mask1]
		movd eax, xmm0
	}
#endif

#if defined(_M_IX86)
// #if 1

#if 1
	__asm {
		mov esi, a
		mov edi, b
		ror esi, 16
		ror edi, 16
	
		mov edx, esi
		mov ecx, edi
		movzx edx, dh
		movzx ecx, ch
		imul edx, ecx
		mov ah, dh

		mov edx, esi
		mov ecx, edi
		movzx edx, dl
		movzx ecx, cl
		imul edx, ecx
		mov al, dh

		ror esi, 16
		ror edi, 16
		shl eax, 16

		mov edx, esi
		mov ecx, edi
		movzx edx, dh
		movzx ecx, ch
		imul edx, ecx
		mov ah, dh

		mov edx, esi
		mov ecx, edi
		movzx edx, dl
		movzx ecx, cl
		imul edx, ecx
		mov al, dh
	}
#endif

#if 0
	__asm {
		mov esi, [a]
		mov edi, [b]
	
//		movzx eax, byte ptr [a+3]
//		movzx ecx, byte ptr [b+3]
		mov eax, esi
		mov ecx, edi
		shr eax, 24
		shr ecx, 24
		imul eax, ecx
		mov dh, ah
//		mov byte ptr [clr+3], ah
		
//		movzx eax, byte ptr [a+2]
//		movzx ecx, byte ptr [b+2]
		mov eax, esi
		mov ecx, edi
		shr eax, 16
		shr ecx, 16
		movzx eax, al
		movzx ecx, cl
		imul eax, ecx
		mov dl, ah
//		mov byte ptr [clr+2], ah

//		movzx eax, byte ptr [a+1]
//		movzx ecx, byte ptr [b+1]
		mov eax, esi
		mov ecx, edi
//		shr eax, 8
//		shr ecx, 8
//		movzx eax, al
//		movzx ecx, cl
		movzx eax, ah
		movzx ecx, ch
		imul eax, ecx
		shl edx, 16
		mov dh, ah
//		mov byte ptr [clr+1], ah
		
//		movzx eax, byte ptr [a]
//		movzx ecx, byte ptr [b]
		mov eax, esi
		mov ecx, edi
		movzx eax, al
		movzx ecx, cl
		imul eax, ecx
		mov dl, ah
//		mov byte ptr [clr+0], ah
		
		mov eax, edx
//		mov eax, [clr]
	}
#endif

#else
	u32 cat, crt, cgt, cbt;
	cat=((a>>16)*(b>>16))&0xFF000000UL;
	crt=(((a>>12)&0x0FF0)*((b>>12)&0x0FF0))&0x00FF0000UL;
	cgt=(((a>>8)&255)*((b>>8)&255))&0xFF00;
	cbt=(a&255)*(b&255)>>8;
	return(cat|crt|cgt|cbt);
#endif
}


static inline void Vec2F_Copy(float *a, float *b)
{
#if defined(_M_IX86) && (_M_IX86_FP>=2)
	__asm {
		mov eax, a
		mov edx, b
		movlps xmm0, [eax]
		movlps [edx], xmm0
	}
#else
//	*(double *)b=*(double *)a;
	b[0]=a[0]; b[1]=a[1];
#endif
}

static inline void Vec4F_Copy(float *a, float *b)
{
#if defined(_M_IX86) && (_M_IX86_FP>=2)
	__asm {
		mov eax, a
		mov edx, b
		movups xmm0, [eax]
		movups [edx], xmm0
	}
#else
//	((double *)b)[0]=((double *)a)[0];
//	((double *)b)[1]=((double *)a)[1];
	b[0]=a[0]; b[1]=a[1]; b[2]=a[2]; b[3]=a[3];
#endif
}

static inline void Vec2F_Copy3Gather(
	float *a0, float *a1, float *a2, float *b)
{
#if defined(_M_IX86) && (_M_IX86_FP>=2)
	__asm {
		mov eax, a0
		mov ecx, a1
		mov edx, a2
		mov ebx, b
		movlps xmm0, [eax]
		movlps xmm1, [ecx]
		movlps xmm2, [edx]
		movlps [ebx], xmm0
		movlps [ebx+8], xmm1
		movlps [ebx+16], xmm2
	}
#else
//	*(double *)b=*(double *)a;
	b[0]=a0[0]; b[1]=a0[1];
	b[2]=a1[0]; b[3]=a1[1];
	b[4]=a2[0]; b[5]=a2[1];
#endif
}

static inline void Vec4F_Copy3Gather(
	float *a0, float *a1, float *a2, float *b)
{
#if defined(_M_IX86) && (_M_IX86_FP>=2)
	__asm {
		mov eax, a0
		mov ecx, a1
		mov edx, a2
		mov ebx, b
		movups xmm0, [eax]
		movups xmm1, [ecx]
		movups xmm2, [edx]
		movups [ebx], xmm0
		movups [ebx+16], xmm1
		movups [ebx+32], xmm2
	}
#else
//	((double *)b)[0]=((double *)a)[0];
//	((double *)b)[1]=((double *)a)[1];
	b[0]=a0[0]; b[1]=a0[1]; b[ 2]=a0[2]; b[ 3]=a0[3];
	b[4]=a1[0]; b[5]=a1[1]; b[ 6]=a1[2]; b[ 7]=a1[3];
	b[8]=a2[0]; b[9]=a2[1]; b[10]=a2[2]; b[11]=a2[3];
#endif
}

static inline bgbrasw_yypixel bgbrasw_pixel2yypixel(
	bgbrasw_pixel pix)
{
	bgbrasw_yypixel ypix;
	int cr, cg, cb, ca;
	int cy, cu, cv, cx;
	
	cr=BGBRASW_PIXEL_R(pix);
	cg=BGBRASW_PIXEL_G(pix);
	cb=BGBRASW_PIXEL_B(pix);
	ca=BGBRASW_PIXEL_A(pix);
	
	cy=(cr+2*cg+cb)>>2;
//	cu=128; cv=128;
//	cu=((cb-cg)>>1)+128;
//	cv=((cr-cg)>>1)+128;
	cu=(cb-cg)+128;
	cv=(cr-cg)+128;
	cu+=8; cv+=8;
	cu=(cu<0)?0:((cu>255)?255:cu);
	cv=(cv<0)?0:((cv>255)?255:cv);
	cx=cy&15;

	ypix=BGBRASW_MAKEYYPIXEL(cy, cy, cy, cy, cu, cv, ca, cx);
	return(ypix);
}

static inline u32 bgbrasw_yycolormul(
	bgbrasw_yypixel a, bgbrasw_yypixel b)
{
	int y0a, y1a, y2a, y3a, ua, va, aa, xa;
	int y0b, y1b, y2b, y3b, ub, vb, ab, xb;
	int y0c, y1c, y2c, y3c, uc, vc, ac, xc;
	bgbrasw_yypixel c;
	
	y0a=BGBRASW_YYPIXEL_Y0(a);	y1a=BGBRASW_YYPIXEL_Y1(a);
	y2a=BGBRASW_YYPIXEL_Y2(a);	y3a=BGBRASW_YYPIXEL_Y3(a);
	ua=BGBRASW_YYPIXEL_U(a);	va=BGBRASW_YYPIXEL_V(a);
	aa=BGBRASW_YYPIXEL_A(a);	xa=BGBRASW_YYPIXEL_X2(a);

	y0b=BGBRASW_YYPIXEL_Y0(b);	y1b=BGBRASW_YYPIXEL_Y1(b);
	y2b=BGBRASW_YYPIXEL_Y2(b);	y3b=BGBRASW_YYPIXEL_Y3(b);
	ub=BGBRASW_YYPIXEL_U(b);	vb=BGBRASW_YYPIXEL_V(b);
	ab=BGBRASW_YYPIXEL_A(b);	xb=BGBRASW_YYPIXEL_X2(b);
	
	y0a|=xa;	y1a|=xa;	y2a|=xa;	y3a|=xa;
	y0b|=xb;	y1b|=xb;	y2b|=xb;	y3b|=xb;
	
	y0c=(y0a*y0b)>>8;	y1c=(y1a*y1b)>>8;
	y2c=(y2a*y2b)>>8;	y3c=(y3a*y3b)>>8;
	uc=(ua+ub)>>1;	vc=(va+vb)>>1;
	ac=(aa*ab)>>8;
	
//	xc=0;
	xc=((y0c+y1c+y2c+y3c)>>2)&15;
	
	c=BGBRASW_MAKEYYPIXEL(y0c, y1c, y2c, y3c, uc, vc, ac, xc);
	return(c);
}

static inline bgbrasw_yypixel bgbrasw_colormul_yymod(
	bgbrasw_yypixel a, bgbrasw_pixel b)
{
	return(bgbrasw_yycolormul(a,
		bgbrasw_pixel2yypixel(b)));
//	return(a);
}

