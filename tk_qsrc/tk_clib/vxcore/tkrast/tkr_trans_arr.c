

int TKRA_DrawTriangleArrayBasic(
	TKRA_Context *ctx,
	void *xyz,		int xyz_str,
	void *st,		int st_str,
	void *rgb,		int rgb_str,
	int cnt)
{
	tkra_trivertex	v0, v1, v2;
	byte *cxyz, *cst, *crgb;
	float *xyz0, *st0;
	u32 *rgb0;
	int i, j, k;
	
	cxyz=xyz;
	cst=st;
	crgb=rgb;
	for(i=0; i<cnt; i++)
	{
		xyz0=(float *)cxyz;
		st0=(float *)cst;
		rgb0=(u32 *)crgb;
		v0.xyz=tkra_mkvec4f(xyz0[0], xyz0[1], xyz0[2], 1);
		v0.st=tkra_mkvec2f(st0[0], st0[1]);
		v0.rgb=*rgb0;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		xyz0=(float *)cxyz;
		st0=(float *)cst;
		rgb0=(u32 *)crgb;
		v1.xyz=tkra_mkvec4f(xyz0[0], xyz0[1], xyz0[2], 1);
		v1.st=tkra_mkvec2f(st0[0], st0[1]);
		v1.rgb=*rgb0;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		xyz0=(float *)cxyz;
		st0=(float *)cst;
		rgb0=(u32 *)crgb;
		v2.xyz=tkra_mkvec4f(xyz0[0], xyz0[1], xyz0[2], 1);
		v2.st=tkra_mkvec2f(st0[0], st0[1]);
		v2.rgb=*rgb0;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;
		
		TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
//		TKRA_TransformProjectTriangle(ctx, v0, v1, v2,
//			ctx->mat_xform, ctx->mat_xproj);
	}
	return(0);
}

int tkra_clamp255(int v)
{
	if(v<0)v=0;
	if(v>255)v=255;
//	__debugbreak();
	return(v);
}

int tkra_clamp127s(int v)
{
	if(v<(-127))v=(-127);
	if(v>( 127))v=( 127);
	return(v);
}

tkra_vec4f tkra_getptr_v4f_2fv01(void *ptr)
{
	float *pf;		pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], 0, 1));
}
tkra_vec4f tkra_getptr_v4f_2sfv01(void *ptr)
{
	tkra_vec2f xy;
	xy=tkra_mkvec2sfv(ptr);
	return(tkra_mkvec4f(tkra_v2f_x(xy), tkra_v2f_y(xy), 0, 1));
}
tkra_vec4f tkra_getptr_v4f_2dv01(void *ptr)
{
	double *pf;	pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], 0, 1));
}

tkra_vec4f tkra_getptr_v4f_2ssv01(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0), 0.0, 1.0));
}

tkra_vec4f tkra_getptr_v4f_2siv01(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0), 0.0, 1.0));
}

#ifdef __BJX2__
tkra_vec4f tkra_getptr_v4f_3fv1(void *ptr);
tkra_vec4f tkra_getptr_v4f_4fv(void *ptr);
__asm {
tkra_getptr_v4f_3fv1:
	MOV.Q	(R4, 0), R2
	MOV.Q	(R4, 8), R3
	MOV		0x3F800000, R5
	MOVLD	R5, R3, R3
	RTS
tkra_getptr_v4f_4fv:
	MOV.Q	(R4, 0), R2
	MOV.Q	(R4, 8), R3
	RTS
};
#else
tkra_vec4f tkra_getptr_v4f_3fv1(void *ptr)
{
	float *pf;		pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], pf[2], 1));
}
tkra_vec4f tkra_getptr_v4f_4fv(void *ptr)
	{ return(tkra_mkvec4fv(ptr)); }
#endif

tkra_vec4f tkra_getptr_v4f_3sfv1(void *ptr)
	{ return(tkra_mkvec3sfv1(ptr)); }
tkra_vec4f tkra_getptr_v4f_3dv1(void *ptr)
{
	double *pf;	pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], pf[2], 1));
}

tkra_vec4f tkra_getptr_v4f_3ssv1(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0),
		pf[2]*(1.0/32767.0), 1.0));
}

tkra_vec4f tkra_getptr_v4f_3siv1(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0),
		pf[2]*(1.0/2147483648.0), 1.0));
}

tkra_vec4f tkra_getptr_v4f_4sfv(void *ptr)
	{ return(tkra_mkvec4sfv(ptr)); }

tkra_vec4f tkra_getptr_v4f_4dv(void *ptr)
{
	double *pf;	pf=ptr;
	return(tkra_mkvec4f(pf[0], pf[1], pf[2], pf[3]));
}

tkra_vec4f tkra_getptr_v4f_4ssv(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0),
		pf[2]*(1.0/32767.0), pf[3]*(1.0/32767.0)));
}

tkra_vec4f tkra_getptr_v4f_4siv(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec4f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0),
		pf[2]*(1.0/2147483648.0), pf[3]*(1.0/2147483648.0)));
}

tkra_vec4f tkra_getptr_v4f_null(void *ptr)
{
	return(tkra_mkvec4f(0.0, 0.0, 0.0, 1.0));
}


#ifdef __BJX2__
tkra_vec2f tkra_getptr_v2f_2fv(void *ptr);
__asm {
tkra_getptr_v2f_2fv:
	MOV.Q	(R4), R2
	RTS
};
#else
tkra_vec2f tkra_getptr_v2f_2fv(void *ptr)
	{ return(tkra_mkvec2fv(ptr)); }
#endif

tkra_vec2f tkra_getptr_v2f_2sfv(void *ptr)
	{ return(tkra_mkvec2sfv(ptr)); }
tkra_vec2f tkra_getptr_v2f_2dv(void *ptr)
{
	double *pf;
	pf=ptr;
	return(tkra_mkvec2f(pf[0], pf[1]));
}

tkra_vec2f tkra_getptr_v2f_2ssv(void *ptr)
{
	s16 *pf;	pf=ptr;
	return(tkra_mkvec2f(
		pf[0]*(1.0/32767.0), pf[1]*(1.0/32767.0)));
}

tkra_vec2f tkra_getptr_v2f_2siv(void *ptr)
{
	s32 *pf;	pf=ptr;
	return(tkra_mkvec2f(
		pf[0]*(1.0/2147483648.0), pf[1]*(1.0/2147483648.0)));
}

tkra_vec2f tkra_getptr_v2f_null(void *ptr)
{
	return(tkra_mkvec2f(0, 0));
}

u32 tkra_getptr_rgba_3sbv(void *ptr)
	{ return(0xFF000000|(*(u32 *)ptr)); }
u32 tkra_getptr_rgba_4sbv(void *ptr)
	{ return(*(u32 *)ptr); }

u32 tkra_getptr_rgba_3ubv(void *ptr)
{
	u32 c;
	c=0xFF000000|(*(u32 *)ptr);
	c=	((c    )&0xFF00FF00) |
		((c>>16)&0x000000FF) |
		((c<<16)&0x00FF0000) ;
	return(c);
//	return(0xFF000000|(*(u32 *)ptr));
}

#ifdef __BJX2__
u32 tkra_getptr_rgba_4ubv(void *ptr);
__asm {
tkra_getptr_rgba_4ubv:
	MOVU.L	(R4), R2
	PSHUF.B R2, 0xC6, R2
	RTS
};
#else
u32 tkra_getptr_rgba_4ubv(void *ptr)
{
	u32 c;
	c=*(u32 *)ptr;
	c=	((c    )&0xFF00FF00) |
		((c>>16)&0x000000FF) |
		((c<<16)&0x00FF0000) ;
	return(c);
}

#endif

u32 tkra_getptr_rgba_3fv(void *ptr)
{
	int cr, cg, cb;
	float *pf;
	pf=ptr;
	cr=pf[0]*255;	cg=pf[1]*255;	cb=pf[2]*255;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4fv(void *ptr)
{
	int cr, cg, cb, ca;
	float *pf;
	pf=ptr;
	cr=pf[0]*255;	cg=pf[1]*255;	cb=pf[2]*255;	ca=pf[3]*255;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3ssv(void *ptr)
{
	int cr, cg, cb;		s16 *pf;	pf=ptr;
	cr=pf[0]/127;	cg=pf[1]/127;	cb=pf[2]/127;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4ssv(void *ptr)
{
	int cr, cg, cb, ca;		s16 *pf;	pf=ptr;
	cr=pf[0]/127;	cg=pf[1]/127;
	cb=pf[2]/127;	ca=pf[3]/127;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3usv(void *ptr)
{
	int cr, cg, cb;		u16 *pf;	pf=ptr;
	cr=pf[0]>>8;	cg=pf[1]>>8;	cb=pf[2]>>8;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4usv(void *ptr)
{
	int cr, cg, cb, ca;		u16 *pf;	pf=ptr;
	cr=pf[0]>>8;	cg=pf[1]>>8;	cb=pf[2]>>8;	ca=pf[3]>>8;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3siv(void *ptr)
{
	int cr, cg, cb;		s32 *pf;	pf=ptr;
	cr=pf[0]>>23;	cg=pf[1]>>23;	cb=pf[2]>>23;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4siv(void *ptr)
{
	int cr, cg, cb, ca;		s32 *pf;	pf=ptr;
	cr=pf[0]>>23;	cg=pf[1]>>23;	cb=pf[2]>>23;	ca=pf[3]>>23;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_3uiv(void *ptr)
{
	int cr, cg, cb;		u32 *pf;	pf=ptr;
	cr=pf[0]>>24;	cg=pf[1]>>24;	cb=pf[2]>>24;
	return(0xFF000000|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_4uiv(void *ptr)
{
	int cr, cg, cb, ca;		u32 *pf;	pf=ptr;
	cr=pf[0]>>24;	cg=pf[1]>>24;	cb=pf[2]>>24;	ca=pf[3]>>24;
	return((ca<<24)|(cr<<16)|(cg<<8)|(cb<<0));
}

u32 tkra_getptr_rgba_null(void *ptr)
{
	return(0xFFFFFFFFU);
}

u32 tkra_getptr_norm_3sbv(void *ptr)
{
	return(*(u32 *)ptr);
}

u32 tkra_getptr_norm_3ssv(void *ptr)
{
	int cr, cg, cb;		s16 *pf;	pf=ptr;
	cr=pf[0]/127;	cg=pf[1]/127;	cb=pf[2]/127;
	return((cb<<16)|(cg<<8)|(cr<<0));
}

u32 tkra_getptr_norm_3fv(void *ptr)
{
	int cr, cg, cb;
	float *pf;
	pf=ptr;
	cr=pf[0]*127;	cg=pf[1]*127;	cb=pf[2]*127;
	return((cb<<16)|(cg<<8)|(cr<<0));
}

#ifdef __BJX2__
int tkra_getptr_int_ubv(void *ptr);
int tkra_getptr_int_usv(void *ptr);
int tkra_getptr_int_uiv(void *ptr);
int tkra_getptr_int_null(void *ptr);
__asm {
tkra_getptr_int_ubv:
	MOVU.B	(R4), R2
	RTS
tkra_getptr_int_usv:
	MOVU.W	(R4), R2
	RTS
tkra_getptr_int_uiv:
	MOVU.L	(R4), R2
	RTS
tkra_getptr_int_null:
	MOV		R4, R2
	RTS
};
#else
int tkra_getptr_int_ubv(void *ptr)
	{ return(*(byte *)ptr); }
int tkra_getptr_int_usv(void *ptr)
	{ return(*(u16 *)ptr); }
int tkra_getptr_int_uiv(void *ptr)
	{ return(*(u32 *)ptr); }
int tkra_getptr_int_null(void *ptr)
	{ return((nlint)ptr); }
#endif

int TKRA_PrimitiveGetFmt(int xyz_n, int xyz_ty)
{
	int fmt;

	switch(xyz_n)
	{
	case 1:
		switch(xyz_ty)
		{
		case TKRA_BYTE:				fmt=TKRA_FMT_1SB;	break;
		case TKRA_UNSIGNED_BYTE:	fmt=TKRA_FMT_1UB;	break;
		case TKRA_SHORT:			fmt=TKRA_FMT_1SS;	break;
		case TKRA_UNSIGNED_SHORT:	fmt=TKRA_FMT_1US;	break;
		case TKRA_INT:				fmt=TKRA_FMT_1SI;	break;
		case TKRA_UNSIGNED_INT:		fmt=TKRA_FMT_1UI;	break;
		case TKRA_FLOAT:			fmt=TKRA_FMT_1F;	break;
		case TKRA_DOUBLE:			fmt=TKRA_FMT_1D;	break;
		case TKRA_HALF_FLOAT:		fmt=TKRA_FMT_1H;	break;
		default:					fmt=TKRA_FMT_1F;	break;
		}
		break;
	case 2:
		switch(xyz_ty)
		{
		case TKRA_BYTE:				fmt=TKRA_FMT_2SB;	break;
		case TKRA_UNSIGNED_BYTE:	fmt=TKRA_FMT_2UB;	break;
		case TKRA_SHORT:			fmt=TKRA_FMT_2SS;	break;
		case TKRA_UNSIGNED_SHORT:	fmt=TKRA_FMT_2US;	break;
		case TKRA_INT:				fmt=TKRA_FMT_2SI;	break;
		case TKRA_UNSIGNED_INT:		fmt=TKRA_FMT_2UI;	break;
		case TKRA_FLOAT:			fmt=TKRA_FMT_2F;	break;
		case TKRA_DOUBLE:			fmt=TKRA_FMT_2D;	break;
		case TKRA_HALF_FLOAT:		fmt=TKRA_FMT_2H;	break;
		default:					fmt=TKRA_FMT_2F;	break;
		}
		break;
	case 3:
		switch(xyz_ty)
		{
		case TKRA_BYTE:				fmt=TKRA_FMT_3SB;	break;
		case TKRA_UNSIGNED_BYTE:	fmt=TKRA_FMT_3UB;	break;
		case TKRA_SHORT:			fmt=TKRA_FMT_3SS;	break;
		case TKRA_UNSIGNED_SHORT:	fmt=TKRA_FMT_3US;	break;
		case TKRA_INT:				fmt=TKRA_FMT_3SI;	break;
		case TKRA_UNSIGNED_INT:		fmt=TKRA_FMT_3UI;	break;
		case TKRA_FLOAT:			fmt=TKRA_FMT_3F;	break;
		case TKRA_DOUBLE:			fmt=TKRA_FMT_3D;	break;
		case TKRA_HALF_FLOAT:		fmt=TKRA_FMT_3H;	break;
		default:					fmt=TKRA_FMT_3F;	break;
		}
		break;
	case 4:
	default:
		switch(xyz_ty)
		{
		case TKRA_BYTE:				fmt=TKRA_FMT_4SB;	break;
		case TKRA_UNSIGNED_BYTE:	fmt=TKRA_FMT_4UB;	break;
		case TKRA_SHORT:			fmt=TKRA_FMT_4SS;	break;
		case TKRA_UNSIGNED_SHORT:	fmt=TKRA_FMT_4US;	break;
		case TKRA_INT:				fmt=TKRA_FMT_4SI;	break;
		case TKRA_UNSIGNED_INT:		fmt=TKRA_FMT_4UI;	break;
		case TKRA_FLOAT:			fmt=TKRA_FMT_4F;	break;
		case TKRA_DOUBLE:			fmt=TKRA_FMT_4D;	break;
		case TKRA_HALF_FLOAT:		fmt=TKRA_FMT_4H;	break;
		default:					fmt=TKRA_FMT_4F;	break;
		}
		break;
	}
	
	return(fmt);
}

#if 1
void *TKRA_PrimitiveFmtGetPtrXYZ(int fmt)
{
	tkra_vec4f	(*getptr_xyz)(void *ptr);

	switch(fmt)
	{
		case TKRA_FMT_2F:	getptr_xyz=tkra_getptr_v4f_2fv01;	break;
		case TKRA_FMT_2D:	getptr_xyz=tkra_getptr_v4f_2dv01;	break;
		case TKRA_FMT_2H:	getptr_xyz=tkra_getptr_v4f_2sfv01;	break;
		case TKRA_FMT_2SS:	getptr_xyz=tkra_getptr_v4f_2ssv01;	break;
		case TKRA_FMT_2SI:	getptr_xyz=tkra_getptr_v4f_2siv01;	break;

		case TKRA_FMT_3F:	getptr_xyz=tkra_getptr_v4f_3fv1;	break;
		case TKRA_FMT_3D:	getptr_xyz=tkra_getptr_v4f_3dv1;	break;
		case TKRA_FMT_3H:	getptr_xyz=tkra_getptr_v4f_3sfv1;	break;
		case TKRA_FMT_3SS:	getptr_xyz=tkra_getptr_v4f_3ssv1;	break;
		case TKRA_FMT_3SI:	getptr_xyz=tkra_getptr_v4f_3siv1;	break;

		case TKRA_FMT_4F:	getptr_xyz=tkra_getptr_v4f_4fv;		break;
		case TKRA_FMT_4D:	getptr_xyz=tkra_getptr_v4f_4dv;		break;
		case TKRA_FMT_4H:	getptr_xyz=tkra_getptr_v4f_4sfv;	break;
		case TKRA_FMT_4SS:	getptr_xyz=tkra_getptr_v4f_4ssv;	break;
		case TKRA_FMT_4SI:	getptr_xyz=tkra_getptr_v4f_4siv;	break;

		default:			getptr_xyz=tkra_getptr_v4f_null;	break;
	}
	
	return(getptr_xyz);
}

void *TKRA_PrimitiveFmtGetPtrST(int fmt)
{
	tkra_vec2f	(*getptr_st)(void *ptr);

	switch(fmt)
	{
	case TKRA_FMT_2F:	getptr_st=tkra_getptr_v2f_2fv;		break;
	case TKRA_FMT_2D:	getptr_st=tkra_getptr_v2f_2dv;		break;
	case TKRA_FMT_2H:	getptr_st=tkra_getptr_v2f_2sfv;		break;
	case TKRA_FMT_2SS:	getptr_st=tkra_getptr_v2f_2ssv;		break;
	case TKRA_FMT_2SI:	getptr_st=tkra_getptr_v2f_2siv;		break;
	default:			getptr_st=tkra_getptr_v2f_null;		break;
	}
	
	return(getptr_st);
}

void *TKRA_PrimitiveFmtGetPtrRGB(int fmt)
{
	u32			(*getptr_rgb)(void *ptr);

	switch(fmt)
	{
	case TKRA_FMT_3SB:	getptr_rgb=tkra_getptr_rgba_3sbv;	break;
	case TKRA_FMT_3UB:	getptr_rgb=tkra_getptr_rgba_3ubv;	break;
	case TKRA_FMT_3SS:	getptr_rgb=tkra_getptr_rgba_3ssv;	break;
	case TKRA_FMT_3US:	getptr_rgb=tkra_getptr_rgba_3usv;	break;
	case TKRA_FMT_3F:	getptr_rgb=tkra_getptr_rgba_3fv;	break;

	case TKRA_FMT_4SB:	getptr_rgb=tkra_getptr_rgba_4sbv;	break;
	case TKRA_FMT_4UB:	getptr_rgb=tkra_getptr_rgba_4ubv;	break;
	case TKRA_FMT_4SS:	getptr_rgb=tkra_getptr_rgba_4ssv;	break;
	case TKRA_FMT_4US:	getptr_rgb=tkra_getptr_rgba_4usv;	break;
	case TKRA_FMT_4F:	getptr_rgb=tkra_getptr_rgba_4fv;	break;

	default:			getptr_rgb=tkra_getptr_rgba_null;	break;
	}
	
	return(getptr_rgb);
}

void *TKRA_PrimitiveFmtGetPtrIndex(int fmt, int *rstr)
{
	int			(*getptr_idx)(void *ptr);
	int idx_str;

	switch(fmt)
	{
	case TKRA_FMT_1UB:	getptr_idx=tkra_getptr_int_ubv; idx_str=1; break;
	case TKRA_FMT_1US:	getptr_idx=tkra_getptr_int_usv; idx_str=2; break;
	case TKRA_FMT_1UI:	getptr_idx=tkra_getptr_int_uiv; idx_str=4; break;
	default:			getptr_idx=tkra_getptr_int_null; idx_str=1; break;
	}
	
	*rstr=idx_str;
	return(getptr_idx);
}
#endif

#if 1
void *TKRA_PrimitiveGetGetPtrXYZ(int xyz_n, int xyz_ty)
{
	tkra_vec4f	(*getptr_xyz)(void *ptr);

	switch(xyz_n)
	{
	case 2:
		switch(xyz_ty)
		{
		case TKRA_FLOAT:	getptr_xyz=tkra_getptr_v4f_2fv01;	break;
		case TKRA_DOUBLE:	getptr_xyz=tkra_getptr_v4f_2dv01;	break;
		case TKRA_HFLOAT:	getptr_xyz=tkra_getptr_v4f_2sfv01;	break;
		case TKRA_SHORT:	getptr_xyz=tkra_getptr_v4f_2ssv01;	break;
		case TKRA_INT:		getptr_xyz=tkra_getptr_v4f_2siv01;	break;
		default:			getptr_xyz=tkra_getptr_v4f_null;	break;
		}
		break;
	case 3:
		switch(xyz_ty)
		{
		case TKRA_FLOAT:	getptr_xyz=tkra_getptr_v4f_3fv1;	break;
		case TKRA_DOUBLE:	getptr_xyz=tkra_getptr_v4f_3dv1;	break;
		case TKRA_HFLOAT:	getptr_xyz=tkra_getptr_v4f_3sfv1;	break;
		case TKRA_SHORT:	getptr_xyz=tkra_getptr_v4f_3ssv1;	break;
		case TKRA_INT:		getptr_xyz=tkra_getptr_v4f_3siv1;	break;
		default:			getptr_xyz=tkra_getptr_v4f_null;	break;
		}
		break;
	case 4:
	default:
		switch(xyz_ty)
		{
		case TKRA_FLOAT:	getptr_xyz=tkra_getptr_v4f_4fv;		break;
		case TKRA_DOUBLE:	getptr_xyz=tkra_getptr_v4f_4dv;		break;
		case TKRA_HFLOAT:	getptr_xyz=tkra_getptr_v4f_4sfv;	break;
		case TKRA_SHORT:	getptr_xyz=tkra_getptr_v4f_4ssv;	break;
		case TKRA_INT:		getptr_xyz=tkra_getptr_v4f_4siv;	break;
		default:			getptr_xyz=tkra_getptr_v4f_null;		break;
		}
		break;
	}
	
	return(getptr_xyz);
}

void *TKRA_PrimitiveGetGetPtrST(int st_n, int st_ty)
{
	tkra_vec2f	(*getptr_st)(void *ptr);

	switch(st_ty)
	{
	case TKRA_FLOAT:	getptr_st=tkra_getptr_v2f_2fv;		break;
	case TKRA_DOUBLE:	getptr_st=tkra_getptr_v2f_2dv;		break;
	case TKRA_HFLOAT:	getptr_st=tkra_getptr_v2f_2sfv;		break;
	case TKRA_SHORT:	getptr_st=tkra_getptr_v2f_2ssv;		break;
	case TKRA_INT:		getptr_st=tkra_getptr_v2f_2siv;		break;
	default:			getptr_st=tkra_getptr_v2f_null;		break;
	}
	
	return(getptr_st);
}

void *TKRA_PrimitiveGetGetPtrRGB(int rgb_n, int rgb_ty)
{
	u32			(*getptr_rgb)(void *ptr);

	switch(rgb_n)
	{
	case 3:
		switch(rgb_ty)
		{
		case TKRA_BYTE:		getptr_rgb=tkra_getptr_rgba_3sbv;	break;
		case TKRA_UBYTE:	getptr_rgb=tkra_getptr_rgba_3ubv;	break;
		case TKRA_SHORT:	getptr_rgb=tkra_getptr_rgba_3ssv;	break;
		case TKRA_USHORT:	getptr_rgb=tkra_getptr_rgba_3usv;	break;
		case TKRA_FLOAT:	getptr_rgb=tkra_getptr_rgba_3fv;	break;
		default:			getptr_rgb=tkra_getptr_rgba_null;	break;
		}
		break;
	case 4:
	default:
		switch(rgb_ty)
		{
		case TKRA_BYTE:		getptr_rgb=tkra_getptr_rgba_4sbv;	break;
		case TKRA_UBYTE:	getptr_rgb=tkra_getptr_rgba_4ubv;	break;
		case TKRA_SHORT:	getptr_rgb=tkra_getptr_rgba_4ssv;	break;
		case TKRA_USHORT:	getptr_rgb=tkra_getptr_rgba_4usv;	break;
		case TKRA_FLOAT:	getptr_rgb=tkra_getptr_rgba_4fv;	break;
		default:			getptr_rgb=tkra_getptr_rgba_null;	break;
		}
		break;
	}
	
	return(getptr_rgb);
}

void *TKRA_PrimitiveGetGetPtrNV(int nv_n, int nv_ty)
{
	u32			(*getptr_rgb)(void *ptr);
	switch(nv_ty)
	{
	case TKRA_BYTE:		case TKRA_UBYTE:
		getptr_rgb=tkra_getptr_norm_3sbv;	break;
	case TKRA_SHORT:	case TKRA_USHORT:
		getptr_rgb=tkra_getptr_norm_3ssv;	break;
	case TKRA_FLOAT:
		getptr_rgb=tkra_getptr_norm_3fv;	break;
	default:
		getptr_rgb=tkra_getptr_rgba_null;	break;
	}
	return(getptr_rgb);
}

void *TKRA_PrimitiveGetGetPtrIndex(int idx_ty, int *rstr)
{
	int			(*getptr_idx)(void *ptr);
	int idx_str;

	switch(idx_ty)
	{
	case	TKRA_BYTE:	case	TKRA_UBYTE:
		getptr_idx=tkra_getptr_int_ubv; idx_str=1; break;
	case	TKRA_SHORT:	case	TKRA_USHORT:
		getptr_idx=tkra_getptr_int_usv; idx_str=2; break;
	case	TKRA_INT:	case	TKRA_UINT:
		getptr_idx=tkra_getptr_int_uiv; idx_str=4; break;
	default:
		getptr_idx=tkra_getptr_int_null; idx_str=1; break;
	}
	
	*rstr=idx_str;
	return(getptr_idx);
}
#endif

int TKRA_DrawPrimitive_TriangleComm(
	TKRA_Context *ctx,
	void *xyz,		int xyz_str,
	void *st,		int st_str,
	void *rgb,		int rgb_str,
	int mode,		int count)
{
	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb;
	int idx_str;
	int i, j, k, cnt, ix;

	cxyz=xyz;
	cst=st;
	crgb=rgb;

	cnt=count/3;
	for(i=0; i<cnt; i++)
	{
		v0.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v0.st=tkra_getptr_v2f_2fv(cst);
		v0.st=*(tkra_vec2f *)cst;
		v0.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v0.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		v1.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v1.st=tkra_getptr_v2f_2fv(cst);
		v1.st=*(tkra_vec2f *)cst;
		v1.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v1.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v2.st=tkra_getptr_v2f_2fv(cst);
		v2.st=*(tkra_vec2f *)cst;
		v2.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v2.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
	}
	return(0);
}

int TKRA_DrawPrimitive_PolyComm(
	TKRA_Context *ctx,
	void *xyz,		int xyz_str,
	void *st,		int st_str,
	void *rgb,		int rgb_str,
	int mode,		int count)
{
	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb;
	int idx_str;
	int i, j, k, cnt, ix;

	cxyz=xyz;
	cst=st;
	crgb=rgb;

	v0.fl=0;
	v1.fl=0;
	v2.fl=0;
	v3.fl=0;

	v0.xyz=tkra_getptr_v4f_3fv1(cxyz);
//	v0.st=tkra_getptr_v2f_2fv(cst);
	v0.st=*(tkra_vec2f *)cst;
	v0.rgb=tkra_getptr_rgba_4ubv(crgb);
//	v0.rgb=*(u32 *)crgb;
	cxyz+=xyz_str;
	cst+=st_str;
	crgb+=rgb_str;

	v1.xyz=tkra_getptr_v4f_3fv1(cxyz);
//	v1.st=tkra_getptr_v2f_2fv(cst);
	v1.st=*(tkra_vec2f *)cst;
	v1.rgb=tkra_getptr_rgba_4ubv(crgb);
//	v1.rgb=*(u32 *)crgb;
	cxyz+=xyz_str;
	cst+=st_str;
	crgb+=rgb_str;

	if(mode==TKRA_POLYGON)
	{
#if 1
		if(count==4)
		{
			v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v2.st=*(tkra_vec2f *)cst;
			v2.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			v3.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v3.st=*(tkra_vec2f *)cst;
			v3.rgb=tkra_getptr_rgba_4ubv(crgb);
	//		cxyz+=xyz_str;
	//		cst+=st_str;
	//		crgb+=rgb_str;

			TKRA_TransformProjectQuad(ctx,
				v0, v1, v2, v3, ctx->mat_tproj);

			return(0);
		}
#endif

#if 1
		if(count==5)
		{
			v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v2.st=*(tkra_vec2f *)cst;
			v2.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			v3.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v3.st=*(tkra_vec2f *)cst;
			v3.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			TKRA_TransformProjectQuad(ctx,
				v0, v1, v2, v3, ctx->mat_tproj);

	//		v2=v3;
			memcpy(&v2, &v3, sizeof(tkra_trivertex));

			v3.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v3.st=*(tkra_vec2f *)cst;
			v3.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			TKRA_TransformProjectTriangle(ctx,
				v0, v2, v3, ctx->mat_tproj);

			return(0);
		}
#endif

#if 1
		if(count==6)
		{
			v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v2.st=*(tkra_vec2f *)cst;
			v2.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			v3.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v3.st=*(tkra_vec2f *)cst;
			v3.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			TKRA_TransformProjectQuad(ctx,
				v0, v1, v2, v3, ctx->mat_tproj);

	//		v1=v3;
			memcpy(&v1, &v3, sizeof(tkra_trivertex));

			v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v2.st=*(tkra_vec2f *)cst;
			v2.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			v3.xyz=tkra_getptr_v4f_3fv1(cxyz);
			v3.st=*(tkra_vec2f *)cst;
			v3.rgb=tkra_getptr_rgba_4ubv(crgb);
			cxyz+=xyz_str;
			cst+=st_str;
			crgb+=rgb_str;

			TKRA_TransformProjectQuad(ctx,
				v0, v1, v2, v3, ctx->mat_tproj);

			return(0);
		}
#endif
	}

	cnt=count-2;
	for(i=0; i<cnt; i++)
	{		
		v2.xyz=tkra_getptr_v4f_3fv1(cxyz);
//		v2.st=tkra_getptr_v2f_2fv(cst);
		v2.st=*(tkra_vec2f *)cst;
		v2.rgb=tkra_getptr_rgba_4ubv(crgb);
//		v2.rgb=*(u32 *)crgb;
		cxyz+=xyz_str;
		cst+=st_str;
		crgb+=rgb_str;

		TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
		v1=v2;
	}

	return(0);
}

#if 0
int TKRA_DrawPrimitiveIndexArrayBasic(
	TKRA_Context *ctx,
	void *xyz,		int xyz_n,		int xyz_ty,		int xyz_str,
	void *st,		int st_n,		int st_ty,		int st_str,
	void *rgb,		int rgb_n,		int rgb_ty,		int rgb_str,
	void *nv,		int nv_n,		int nv_ty,		int nv_str,
	void *idx,						int idx_ty,
	int mode,		int count)
{
	tkra_vec4f	(*getptr_xyz)(void *ptr);
	tkra_vec2f	(*getptr_st )(void *ptr);
	u32			(*getptr_rgb)(void *ptr);
	u32			(*getptr_nv )(void *ptr);
	int			(*getptr_idx)(void *ptr);

	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb, *cnv, *cix;
	int idx_str;
	int i, j, k, cnt, ix;
	
//	return;
//	return(0);

	if(!xyz)xyz_ty=0;
	if(!st )st_ty=0;
	if(!rgb)rgb_ty=0;
	if(!nv)nv_ty=0;
	if(!idx)
		{ idx_ty=0; idx_str=1; }

#if 1
	if(	(xyz_ty==TKRA_FLOAT) &&
		(st_ty==TKRA_FLOAT) &&
		(rgb_ty==TKRA_UBYTE) &&
		(idx_ty==0) &&
		(xyz_n==3) &&
		(st_n==2) &&
		(rgb_n==4))
	{
		if(mode==TKRA_TRIANGLES)
		{
			TKRA_DrawPrimitive_TriangleComm(ctx,
				xyz,	xyz_str,	st,		st_str,
				rgb,	rgb_str,	mode,	count);
			return;
		}

		if((mode==TKRA_TRIANGLE_FAN) || (mode==TKRA_POLYGON))
		{
			TKRA_DrawPrimitive_PolyComm(ctx,
				xyz,	xyz_str,	st,		st_str,
				rgb,	rgb_str,	mode,	count);
			return;
		}
	}
#endif
	
//	return(0);
	
	getptr_xyz=TKRA_PrimitiveGetGetPtrXYZ(xyz_n, xyz_ty);
	getptr_st =TKRA_PrimitiveGetGetPtrST(st_n, st_ty);
	getptr_rgb=TKRA_PrimitiveGetGetPtrRGB(rgb_n, rgb_ty);
	getptr_nv=TKRA_PrimitiveGetGetPtrNV(nv_n, nv_ty);
	getptr_idx=TKRA_PrimitiveGetGetPtrIndex(idx_ty, &idx_str);

	cxyz=xyz;
	cst=st;
	crgb=rgb;
	cnv=nv;
	cix=idx;

	v0.fl=0;
	v1.fl=0;
	v2.fl=0;
	v3.fl=0;

	if(mode==TKRA_TRIANGLES)
	{
		cnt=count/3;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*3+0)*idx_str));
			v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v0.st=getptr_st(cst+(ix*st_str));
			v0.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v0.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+((i*3+1)*idx_str));
			v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v1.st=getptr_st(cst+(ix*st_str));
			v1.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v1.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+((i*3+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v2.nv=getptr_nv(cnv+(ix*nv_str));

			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
		}
		return(0);
	}

	if((mode==TKRA_TRIANGLE_FAN) || (mode==TKRA_POLYGON))
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));
		v0.nv=getptr_nv(cnv+(ix*nv_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));
		v1.nv=getptr_nv(cnv+(ix*nv_str));

#if 1
		if(count==4)
		{
			ix=getptr_idx(cix+(2*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v2.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+(3*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v3.nv=getptr_nv(cnv+(ix*nv_str));

			TKRA_TransformProjectQuad(ctx, v0, v1, v2, v3, ctx->mat_tproj);
			return(0);
		}
#endif

		cnt=count-2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v2.nv=getptr_nv(cnv+(ix*nv_str));

			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
			v1=v2;
		}
		return(0);
	}

	if(mode==TKRA_TRIANGLE_STRIP)
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));
		v0.nv=getptr_nv(cnv+(ix*nv_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));
		v1.nv=getptr_nv(cnv+(ix*nv_str));

		cnt=count-2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v2.nv=getptr_nv(cnv+(ix*nv_str));

			if(i&1)
			{
				TKRA_TransformProjectTriangle(ctx,
					v2, v1, v0, ctx->mat_tproj);
			}else
			{
				TKRA_TransformProjectTriangle(ctx,
					v0, v1, v2, ctx->mat_tproj);
			}
			
			v0=v1;
			v1=v2;
		}
		return(0);
	}

	if(mode==TKRA_QUADS)
	{
		cnt=count>>2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*4+0)*idx_str));
			v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v0.st=getptr_st(cst+(ix*st_str));
			v0.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v0.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+((i*4+1)*idx_str));
			v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v1.st=getptr_st(cst+(ix*st_str));
			v1.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v1.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+((i*4+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v2.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+((i*4+3)*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v3.nv=getptr_nv(cnv+(ix*nv_str));

//			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
//			TKRA_TransformProjectTriangle(ctx, v0, v2, v3, ctx->mat_tproj);

			TKRA_TransformProjectQuad(ctx, v0, v1, v2, v3, ctx->mat_tproj);
		}
		return(0);
	}

	if(mode==TKRA_QUAD_STRIP)
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));
		v0.nv=getptr_nv(cnv+(ix*nv_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));
		v1.nv=getptr_nv(cnv+(ix*nv_str));

		cnt=(count-2)/2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*2+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v2.nv=getptr_nv(cnv+(ix*nv_str));

			ix=getptr_idx(cix+((i*2+3)*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			v3.nv=getptr_nv(cnv+(ix*nv_str));

//			TKRA_TransformProjectTriangle(ctx, v0, v2, v1, ctx->mat_tproj);
//			TKRA_TransformProjectTriangle(ctx, v1, v2, v3, ctx->mat_tproj);

			TKRA_TransformProjectQuad(ctx, v0, v1, v2, v3, ctx->mat_tproj);

			v0=v2;
			v1=v3;
		}
		return(0);
	}

	if(mode==TKRA_POINTS)
	{
		return(0);
	}

	if(	(mode==TKRA_LINES) ||
		(mode==TKRA_LINE_LOOP) ||
		(mode==TKRA_LINE_STRIP))
	{
		return(0);
	}

	return(-1);
}
#endif


int TKRA_DrawPrimitiveIndexArrayBasicB(
	TKRA_Context *ctx,
//	void *xyz,		int xyz_n,		int xyz_ty,		int xyz_str,
//	void *st,		int st_n,		int st_ty,		int st_str,
//	void *rgb,		int rgb_n,		int rgb_ty,		int rgb_str,

	void *xyz,		int xyz_fmt,	int xyz_str,
	void *st,		int st_fmt,		int st_str,
	void *rgb,		int rgb_fmt,	int rgb_str,

	void *idx,						int idx_fmt,
	int mode,		int count)
{
	tkra_vec4f	(*getptr_xyz)(void *ptr);
	tkra_vec2f	(*getptr_st )(void *ptr);
	u32			(*getptr_rgb)(void *ptr);
	int			(*getptr_idx)(void *ptr);

	tkra_trivertex	v0, v1, v2, v3;
	byte *cxyz, *cst, *crgb, *cix;
	int idx_str;
	int i, j, k, cnt, ix;
	
//	return;
	
	if(!xyz)xyz_fmt=0;
	if(!st )st_fmt=0;
	if(!rgb)rgb_fmt=0;
	if(!idx)
		{ idx_fmt=0; idx_str=1; }

	TKRA_CheckBoundTexImg(ctx);

	if(!ctx->svctx->tex_cur)
	{
		__debugbreak();
	}

#if 1
//	if(	(xyz_ty==TKRA_FLOAT) &&
//		(st_ty==TKRA_FLOAT) &&
//		(rgb_ty==TKRA_UBYTE) &&
//		(idx_ty==0) &&
//		(xyz_n==3) &&
//		(st_n==2) &&
//		(rgb_n==4))

	if(	(xyz_fmt==TKRA_FMT_3F) &&
		(st_fmt==TKRA_FMT_2F) &&
		(rgb_fmt==TKRA_FMT_4UB) &&
		(idx_fmt==0) )
	{
		if(mode==TKRA_TRIANGLES)
		{
			TKRA_DrawPrimitive_TriangleComm(ctx,
				xyz,	xyz_str,	st,		st_str,
				rgb,	rgb_str,	mode,	count);
			return;
		}

		if((mode==TKRA_TRIANGLE_FAN) || (mode==TKRA_POLYGON))
		{
			TKRA_DrawPrimitive_PolyComm(ctx,
				xyz,	xyz_str,	st,		st_str,
				rgb,	rgb_str,	mode,	count);
			return;
		}
	}
#endif
	
//	getptr_xyz=TKRA_PrimitiveGetGetPtrXYZ(xyz_n, xyz_ty);
//	getptr_st =TKRA_PrimitiveGetGetPtrST(st_n, st_ty);
//	getptr_rgb=TKRA_PrimitiveGetGetPtrRGB(rgb_n, rgb_ty);
//	getptr_idx=TKRA_PrimitiveGetGetPtrIndex(idx_ty, &idx_str);

	getptr_xyz=TKRA_PrimitiveFmtGetPtrXYZ(xyz_fmt);
	getptr_st =TKRA_PrimitiveFmtGetPtrST ( st_fmt);
	getptr_rgb=TKRA_PrimitiveFmtGetPtrRGB(rgb_fmt);
	getptr_idx=TKRA_PrimitiveFmtGetPtrIndex(idx_fmt, &idx_str);
	

	cxyz=xyz;
	cst=st;
	crgb=rgb;
	cix=idx;

	if(mode==TKRA_TRIANGLES)
	{
		cnt=count/3;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*3+0)*idx_str));
			v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v0.st=getptr_st(cst+(ix*st_str));
			v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*3+1)*idx_str));
			v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v1.st=getptr_st(cst+(ix*st_str));
			v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*3+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
		}
		return(0);
	}

	if((mode==TKRA_TRIANGLE_FAN) || (mode==TKRA_POLYGON))
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

#if 1
		if(count==4)
		{
			ix=getptr_idx(cix+(2*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+(3*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));

			TKRA_TransformProjectQuad(ctx, v0, v1, v2, v3, ctx->mat_tproj);
			return(0);
		}
#endif

		cnt=count-2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
			v1=v2;
		}
		return(0);
	}

	if(mode==TKRA_TRIANGLE_STRIP)
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

		cnt=count-2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
			if(i&1)
			{
				TKRA_TransformProjectTriangle(ctx,
					v2, v1, v0, ctx->mat_tproj);
			}else
			{
				TKRA_TransformProjectTriangle(ctx,
					v0, v1, v2, ctx->mat_tproj);
			}
			
			v0=v1;
			v1=v2;
		}
		return(0);
	}

	if(mode==TKRA_QUADS)
	{
		cnt=count>>2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*4+0)*idx_str));
			v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v0.st=getptr_st(cst+(ix*st_str));
			v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*4+1)*idx_str));
			v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v1.st=getptr_st(cst+(ix*st_str));
			v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*4+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*4+3)*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
//			TKRA_TransformProjectTriangle(ctx, v0, v1, v2, ctx->mat_tproj);
//			TKRA_TransformProjectTriangle(ctx, v0, v2, v3, ctx->mat_tproj);

			TKRA_TransformProjectQuad(ctx, v0, v1, v2, v3, ctx->mat_tproj);
		}
		return(0);
	}

	if(mode==TKRA_QUAD_STRIP)
	{
		ix=getptr_idx(cix+(0*idx_str));
		v0.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v0.st=getptr_st(cst+(ix*st_str));
		v0.rgb=getptr_rgb(crgb+(ix*rgb_str));

		ix=getptr_idx(cix+(1*idx_str));
		v1.xyz=getptr_xyz(cxyz+(ix*xyz_str));
		v1.st=getptr_st(cst+(ix*st_str));
		v1.rgb=getptr_rgb(crgb+(ix*rgb_str));

		cnt=(count-2)/2;
		for(i=0; i<cnt; i++)
		{
			ix=getptr_idx(cix+((i*2+2)*idx_str));
			v2.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v2.st=getptr_st(cst+(ix*st_str));
			v2.rgb=getptr_rgb(crgb+(ix*rgb_str));

			ix=getptr_idx(cix+((i*2+3)*idx_str));
			v3.xyz=getptr_xyz(cxyz+(ix*xyz_str));
			v3.st=getptr_st(cst+(ix*st_str));
			v3.rgb=getptr_rgb(crgb+(ix*rgb_str));
			
//			TKRA_TransformProjectTriangle(ctx, v0, v2, v1, ctx->mat_tproj);
//			TKRA_TransformProjectTriangle(ctx, v1, v2, v3, ctx->mat_tproj);

			TKRA_TransformProjectQuad(ctx, v0, v1, v2, v3, ctx->mat_tproj);

			v0=v2;
			v1=v3;
		}
		return(0);
	}

	if(mode==TKRA_POINTS)
	{
		return(0);
	}

	if(	(mode==TKRA_LINES) ||
		(mode==TKRA_LINE_LOOP) ||
		(mode==TKRA_LINE_STRIP))
	{
		return(0);
	}

	return(-1);
}

#if 0
int TKRA_DrawPrimitiveIndexArrayBasic(
	TKRA_Context *ctx,
	void *xyz,		int xyz_n,		int xyz_ty,		int xyz_str,
	void *st,		int st_n,		int st_ty,		int st_str,
	void *rgb,		int rgb_n,		int rgb_ty,		int rgb_str,
	void *idx,						int idx_ty,
	int mode,		int count)
{
	int xyz_fmt, st_fmt, rgb_fmt, idx_fmt;
	int rt;
	
	xyz_fmt=TKRA_PrimitiveGetFmt(xyz_n, xyz_ty);
	st_fmt=TKRA_PrimitiveGetFmt(st_n, st_ty);
	rgb_fmt=TKRA_PrimitiveGetFmt(rgb_n, rgb_ty);
	idx_fmt=TKRA_PrimitiveGetFmt(1, idx_ty);

	rt=TKRA_DrawPrimitiveIndexArrayBasicB(ctx,
		xyz, xyz_fmt, xyz_str,
		 st,  st_fmt,  st_str,
		rgb, rgb_fmt, rgb_str,
		idx, idx_fmt,
		mode, count);
	return(rt);
}
#endif

int TKRA_DrawPrimitiveIndexArrayObjI(
	TKRA_Context *ctx,
	TKRA_DrawPrimArrays	*parr,
	int mode,		int count)
{
	int rt;

	rt=TKRA_DrawPrimitiveIndexArrayBasicB(ctx,
		parr->vptr_xyz_ptr, parr->vptr_xyz_fmt, parr->vptr_xyz_str,
		parr->vptr_st_ptr,  parr->vptr_st_fmt,  parr->vptr_st_str,
		parr->vptr_rgb_ptr, parr->vptr_rgb_fmt, parr->vptr_rgb_str,
		parr->vptr_ix_ptr,  parr->vptr_ix_fmt,
		mode, count);
	return(rt);
}

int TKRA_DrawPrimitiveIndexArrayObj(
	TKRA_Context *ctx,
	TKRA_DrawPrimArrays	*parr,
	int mode,		int count)
{
	int rt;
	return(ctx->vt->DrawPrimitiveIndexArrayObj(
		ctx, parr,
		mode, count));
}

#if 1
int TKRA_DrawPrimitiveIndexArrayBasic(
	TKRA_Context *ctx,
	void *xyz,		int xyz_n,		int xyz_ty,		int xyz_str,
	void *st,		int st_n,		int st_ty,		int st_str,
	void *rgb,		int rgb_n,		int rgb_ty,		int rgb_str,
	void *nv,		int nv_n,		int nv_ty,		int nv_str,
	void *idx,						int idx_ty,
	int mode,		int count)
{
	TKRA_DrawPrimArrays	t_parr;
	TKRA_DrawPrimArrays	*parr;
	int xyz_fmt, st_fmt, rgb_fmt, idx_fmt, nv_fmt;
	int rt;
	
	xyz_fmt=TKRA_PrimitiveGetFmt(xyz_n, xyz_ty);
	st_fmt=TKRA_PrimitiveGetFmt(st_n, st_ty);
	rgb_fmt=TKRA_PrimitiveGetFmt(rgb_n, rgb_ty);
	nv_fmt=TKRA_PrimitiveGetFmt(nv_n, nv_ty);
	idx_fmt=TKRA_PrimitiveGetFmt(1, idx_ty);

	parr=&t_parr;
	parr->vptr_xyz_fmt=xyz_fmt;
	parr->vptr_xyz_str=xyz_str;
	parr->vptr_xyz_ptr=xyz;

	parr->vptr_st_fmt=st_fmt;
	parr->vptr_st_str=st_str;
	parr->vptr_st_ptr=st;

	parr->vptr_rgb_fmt=rgb_fmt;
	parr->vptr_rgb_str=rgb_str;
	parr->vptr_rgb_ptr=rgb;

	parr->vptr_nv_fmt=nv_fmt;
	parr->vptr_nv_str=nv_str;
	parr->vptr_nv_ptr=nv;

	parr->vptr_ix_fmt=idx_fmt;
	parr->vptr_ix_str=1;
	parr->vptr_ix_ptr=idx;

	rt=TKRA_DrawPrimitiveIndexArrayObj(ctx,
		parr, mode, count);
	return(rt);
}
#endif
