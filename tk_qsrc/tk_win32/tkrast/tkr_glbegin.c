TKRA_Context *tkra_current_context;

TKRA_Context *TKRA_GetCurrentContext(void)
{
	return(tkra_current_context);
}

void TKRA_SetCurrentContext(TKRA_Context *ctx)
{
	tkra_current_context=ctx;
}

int TKRA_SetupContextBasic(int xs, int ys)
{
	TKRA_Context *ractx;

	ractx=TKRA_AllocContext();
	TKRA_SetupScreen(ractx, xs, ys);
	TKRA_SetCurrentContext(ractx);
	
	ractx->clear_zbuf=0x7FFF;
	ractx->clear_rgb5=0x0000;
	ractx->clear_rgba=0x00000000;
	ractx->zat_alfunc=TKRA_ZAT_AL;
	ractx->zat_zfunc=TKRA_ZAT_LE;
}

void *TKRA_GetCurrentScreenBuffer_RGB(void)
{
	TKRA_Context *ractx;
	ractx=TKRA_GetCurrentContext();
	if(!ractx)
		return(NULL);
	return(ractx->screen_rgb);
}

int TKRA_CalcDefaultStride(int size, int type)
{
	int str;
	
	str=0;
	switch(type)
	{
		case TKRA_BYTE:		case TKRA_UBYTE:
			str=size;	break;
		case TKRA_SHORT:	case TKRA_USHORT:
		case TKRA_HFLOAT:	case TKRA_2_BYTES:
			str=size*2;	break;
		case TKRA_INT:		case TKRA_UINT:	
		case TKRA_FLOAT:	case TKRA_4_BYTES:
			str=size*4;	break;
		case TKRA_DOUBLE:
			str=size*8;	break;
	}
	return(str);
}

void tkra_glVertexPointer(int size, int type, int stride,
	const void *pointer)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	if(!stride)
		stride=TKRA_CalcDefaultStride(size, type);
	
//	ctx->tkgl_vptr_xyz_nsz=size;
//	ctx->tkgl_vptr_xyz_ty=type;
//	ctx->tkgl_vptr_xyz_str=stride;
//	ctx->tkgl_vptr_xyz_ptr=(void *)pointer;

	ctx->vptr->vptr_xyz_fmt=TKRA_PrimitiveGetFmt(size, type);
	ctx->vptr->vptr_xyz_str=stride;
	ctx->vptr->vptr_xyz_ptr=(void *)pointer;

	ctx->VaGetPtr_xyz=TKRA_PrimitiveGetGetPtrXYZ(size, type);

//	getptr_xyz=TKRA_PrimitiveGetGetPtrXYZ(xyz_n, xyz_ty);
//	getptr_st =TKRA_PrimitiveGetGetPtrST(st_n, st_ty);
//	getptr_rgb=TKRA_PrimitiveGetGetPtrRGB(rgb_n, rgb_ty);
//	getptr_idx=TKRA_PrimitiveGetGetPtrIndex(idx_ty, &idx_str);

}

void tkra_glTexCoordPointer(int size, int type, int stride,
	const void *pointer)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();

	if(!stride)
		stride=TKRA_CalcDefaultStride(size, type);

//	ctx->tkgl_vptr_st_nsz=size;
//	ctx->tkgl_vptr_st_ty=type;
//	ctx->tkgl_vptr_st_str=stride;
//	ctx->tkgl_vptr_st_ptr=(void *)pointer;

	ctx->vptr->vptr_st_fmt=TKRA_PrimitiveGetFmt(size, type);
	ctx->vptr->vptr_st_str=stride;
	ctx->vptr->vptr_st_ptr=(void *)pointer;

	ctx->VaGetPtr_st =TKRA_PrimitiveGetGetPtrST(size, type);
}

void tkra_glColorPointer(int size, int type, int stride,
	const void *pointer)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();

	if(!stride)
		stride=TKRA_CalcDefaultStride(size, type);

//	ctx->tkgl_vptr_rgb_nsz=size;
//	ctx->tkgl_vptr_rgb_ty=type;
//	ctx->tkgl_vptr_rgb_str=stride;
//	ctx->tkgl_vptr_rgb_ptr=(void *)pointer;

	ctx->vptr->vptr_rgb_fmt=TKRA_PrimitiveGetFmt(size, type);
	ctx->vptr->vptr_rgb_str=stride;
	ctx->vptr->vptr_rgb_ptr=(void *)pointer;

	ctx->VaGetPtr_rgb=TKRA_PrimitiveGetGetPtrRGB(size, type);
}

void tkra_glNormalPointer(int type, int stride,
	const void *pointer)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();

	if(!stride)
		stride=TKRA_CalcDefaultStride(3, type);

//	ctx->tkgl_vptr_nv_nsz=3;
//	ctx->tkgl_vptr_nv_ty=type;
//	ctx->tkgl_vptr_nv_str=stride;
//	ctx->tkgl_vptr_nv_ptr=(void *)pointer;

	ctx->vptr->vptr_nv_fmt=TKRA_PrimitiveGetFmt(3, type);
	ctx->vptr->vptr_nv_str=stride;
	ctx->vptr->vptr_nv_ptr=(void *)pointer;
}

void tkra_glInterleavedArrays(int format, int stride, const void *pointer)
{
	__debugbreak();
}

void tkra_glDrawArrays(int mode, int first, int count)
{
	TKRA_Context *ctx;
	void *ixp;

	ctx=TKRA_GetCurrentContext();
	
	ixp=(void *)((nlint)first);

#if 0
	TKRA_DrawPrimitiveIndexArrayBasic(
		ctx,
		ctx->tkgl_vptr_xyz_ptr,	ctx->tkgl_vptr_xyz_nsz,
		ctx->tkgl_vptr_xyz_ty,	ctx->tkgl_vptr_xyz_str,
		ctx->tkgl_vptr_st_ptr,	ctx->tkgl_vptr_st_nsz,
		ctx->tkgl_vptr_st_ty,	ctx->tkgl_vptr_st_str,
		ctx->tkgl_vptr_rgb_ptr,	ctx->tkgl_vptr_rgb_nsz,
		ctx->tkgl_vptr_rgb_ty,	ctx->tkgl_vptr_rgb_str,
		ctx->tkgl_vptr_nv_ptr,	ctx->tkgl_vptr_nv_nsz,
		ctx->tkgl_vptr_nv_ty,	ctx->tkgl_vptr_nv_str,
		ixp,	0,
		mode,	count);
#endif

	ctx->vptr->vptr_ix_fmt=0;
	ctx->vptr->vptr_ix_str=1;
	ctx->vptr->vptr_ix_ptr=ixp;

	TKRA_DrawPrimitiveIndexArrayObj(ctx,
		ctx->vptr, mode, count);
}

void tkra_glDrawElements(int mode, int count,
	int type, const void *indices)
{
	TKRA_Context *ctx;
	void *ixp;

	ctx=TKRA_GetCurrentContext();
	
	ixp=(void *)indices;

#if 0
	TKRA_DrawPrimitiveIndexArrayBasic(
		ctx,
		ctx->tkgl_vptr_xyz_ptr,	ctx->tkgl_vptr_xyz_nsz,
		ctx->tkgl_vptr_xyz_ty,	ctx->tkgl_vptr_xyz_str,
		ctx->tkgl_vptr_st_ptr,	ctx->tkgl_vptr_st_nsz,
		ctx->tkgl_vptr_st_ty,	ctx->tkgl_vptr_st_str,
		ctx->tkgl_vptr_rgb_ptr,	ctx->tkgl_vptr_rgb_nsz,
		ctx->tkgl_vptr_rgb_ty,	ctx->tkgl_vptr_rgb_str,
		ctx->tkgl_vptr_nv_ptr,	ctx->tkgl_vptr_nv_nsz,
		ctx->tkgl_vptr_nv_ty,	ctx->tkgl_vptr_nv_str,
		ixp,	type,
		mode,	count);
#endif

	ctx->vptr->vptr_ix_fmt=TKRA_PrimitiveGetFmt(1, type);
	ctx->vptr->vptr_ix_str=1;
	ctx->vptr->vptr_ix_ptr=indices;

	TKRA_DrawPrimitiveIndexArrayObj(ctx,
		ctx->vptr, mode, count);
}

void tkra_glBegin(int mode)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_mode=mode;
	ctx->tkgl_begin_nvtx=0;

	if(!ctx->tkgl_begin_vtxa)
	{
		ctx->tkgl_begin_vtxa=malloc(4096);
		ctx->tkgl_begin_mvtx=4096/(8*4);
	}
}

void tkra_glEnd(void)
{
	TKRA_Context *ctx;
	float *vtxa;

	ctx=TKRA_GetCurrentContext();
	
//	return;
	
	vtxa=ctx->tkgl_begin_vtxa;
	TKRA_DrawPrimitiveIndexArrayBasic(
		ctx,
		vtxa+0,	4,	TKRA_FLOAT,	8*4,
		vtxa+4,	2,	TKRA_FLOAT,	8*4,
		vtxa+6,	4,	TKRA_UBYTE,	8*4,
		vtxa+7,	3,	TKRA_BYTE,	8*4,
		NULL,		0,
		ctx->tkgl_begin_mode,
		ctx->tkgl_begin_nvtx);
}

void tkra_glColor3ubI(int red, int green, int blue)
{
	TKRA_Context *ctx;
	int cr, cg, cb, ca;
	u32 px;

	cr=tkra_clamp255(red);
	cg=tkra_clamp255(green);
	cb=tkra_clamp255(blue);
	ca=255;
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_rgba=px;
//	ctx->tkgl_begin_nvtx=0;
	
//	if(!ctx->tkgl_begin_vtxa)
//	{
//		ctx->tkgl_begin_vtxa=malloc(4096);
//		ctx->tkgl_begin_mvtx=4096/(8*4);
//	}
}

void tkra_glColor4ubI(int red, int green, int blue, int alpha)
{
	TKRA_Context *ctx;
	int cr, cg, cb, ca;
	u32 px;

	cr=tkra_clamp255(red);
	cg=tkra_clamp255(green);
	cb=tkra_clamp255(blue);
	ca=tkra_clamp255(alpha);
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_rgba=px;
}

void tkra_glTexCoord2fI(float s, float t)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_st[0]=s;
	ctx->tkgl_begin_st[1]=t;
}

void tkra_glNormal3fI(float x, float y, float z)
{
	TKRA_Context *ctx;
	int cr, cg, cb, ca;
	u32 px;

	cr=(tkra_clamp127s(x*127.0))&255;
	cg=(tkra_clamp127s(y*127.0))&255;
	cb=(tkra_clamp127s(z*127.0))&255;
	ca=0;
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_norm=px;
}


void tkra_glVertex4fI(float x, float y, float z, float w)
{
	TKRA_Context *ctx;
	float *ct;
	int i, j, k, n;

	ctx=TKRA_GetCurrentContext();

	if((ctx->tkgl_begin_nvtx+1)>=ctx->tkgl_begin_mvtx)
	{
		i=ctx->tkgl_begin_mvtx;
		i=i+(i>>1);
		ctx->tkgl_begin_vtxa=realloc(
			ctx->tkgl_begin_vtxa, i*(8*4));
		ctx->tkgl_begin_mvtx=i;
	}

	n=ctx->tkgl_begin_nvtx++;
	ct=(ctx->tkgl_begin_vtxa)+(n*8);
	
	ct[0]=x;	ct[1]=y;
	ct[2]=z;	ct[3]=w;
	ct[4]=ctx->tkgl_begin_st[0];
	ct[5]=ctx->tkgl_begin_st[1];
	((u32 *)ct)[6]=ctx->tkgl_begin_rgba;
	((u32 *)ct)[7]=ctx->tkgl_begin_norm;

//	ctx->tkgl_begin_rgba=mode;
//	ctx->tkgl_begin_nv[0]=x;
//	ctx->tkgl_begin_nv[1]=y;
//	ctx->tkgl_begin_nv[2]=z;
}


void tkra_glColor3b(tkra_s8 red, tkra_s8 green, tkra_s8 blue)
	{ tkra_glColor3ubI(red<<1, green<<1, blue<<1); }
void tkra_glColor3s(tkra_s16 red, tkra_s16 green, tkra_s16 blue)
	{ tkra_glColor3ubI(red>>7, green>>7, blue>>7); }
void tkra_glColor3i(tkra_s32 red, tkra_s32 green, tkra_s32 blue)
	{ tkra_glColor3ubI(red>>23, green>>23, blue>>23); }

void tkra_glColor3f(tkra_f32 red, tkra_f32 green, tkra_f32 blue)
{
	TKRA_Context *ctx;
	int cr, cg, cb, ca;
	u32 px;

	cr=(int)(red*255);
	cg=(int)(green*255);
	cb=(int)(blue*255);
	cr=tkra_clamp255(cr);
	cg=tkra_clamp255(cg);
	cb=tkra_clamp255(cb);

//	cr=tkra_clamp255((int)(red*255));
//	cg=tkra_clamp255((int)(green*255));
//	cb=tkra_clamp255((int)(blue*255));
	ca=255;
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_rgba=px;

//	tkra_glColor3ubI(red*255.0, green*255.0, blue*255.0);
}

void tkra_glColor3d(tkra_f64 red, tkra_f64 green, tkra_f64 blue)
	{ tkra_glColor3ubI(red*255.0, green*255.0, blue*255.0); }
void tkra_glColor3ub(tkra_u8 red, tkra_u8 green, tkra_u8 blue)
	{ tkra_glColor3ubI(red, green, blue); }
void tkra_glColor3us(tkra_u16 red, tkra_u16 green, tkra_u16 blue)
	{ tkra_glColor3ubI(red>>8, green>>8, blue>>8); }
void tkra_glColor3ui(tkra_u32 red, tkra_u32 green, tkra_u32 blue)
	{ tkra_glColor3ubI(red>>24, green>>24, blue>>24); }
void tkra_glColor4b(tkra_s8 red, tkra_s8 green, tkra_s8 blue, tkra_s8 alpha)
	{ tkra_glColor4ubI(red<<1, green<<1, blue<<1, alpha<<1); }
void tkra_glColor4s(tkra_s16 red, tkra_s16 green, tkra_s16 blue, tkra_s16 alpha)
	{ tkra_glColor4ubI(red>>7, green>>7, blue>>7, alpha>>7); }
void tkra_glColor4i(tkra_s32 red, tkra_s32 green, tkra_s32 blue, tkra_s32 alpha)
	{ tkra_glColor4ubI(red>>23, green>>23, blue>>23, alpha>>23); }

void tkra_glColor4f(tkra_f32 red, tkra_f32 green, tkra_f32 blue, tkra_f32 alpha)
{
	TKRA_Context *ctx;
	int cr, cg, cb, ca;
	u32 px;

	cr=(int)(red*255);
	cg=(int)(green*255);
	cb=(int)(blue*255);
	ca=(int)(alpha*255);
	cr=tkra_clamp255(cr);
	cg=tkra_clamp255(cg);
	cb=tkra_clamp255(cb);
	ca=tkra_clamp255(ca);

//	cr=tkra_clamp255((int)(red*255));
//	cg=tkra_clamp255((int)(green*255));
//	cb=tkra_clamp255((int)(blue*255));
//	ca=tkra_clamp255((int)(alpha*255));
	px=(ca<<24)|(cr<<16)|(cg<<8)|cb;

	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_rgba=px;
//	tkra_glColor4ubI(red*255.0, green*255.0, blue*255.0, alpha*255.0);
}

void tkra_glColor4d(tkra_f64 red, tkra_f64 green, tkra_f64 blue, tkra_f64 alpha)
	{ tkra_glColor4ubI(red*255.0, green*255.0, blue*255.0, alpha*255.0); }
void tkra_glColor4ub(tkra_u8 red, tkra_u8 green, tkra_u8 blue, tkra_u8 alpha)
	{ tkra_glColor4ubI(red, green, blue, alpha); }
void tkra_glColor4us(tkra_u16 red, tkra_u16 green, tkra_u16 blue, tkra_u16 alpha)
	{ tkra_glColor4ubI(red>>8, green>>8, blue>>8, alpha>>8); }
void tkra_glColor4ui(tkra_u32 red, tkra_u32 green, tkra_u32 blue, tkra_u32 alpha)
	{ tkra_glColor4ubI(red>>24, green>>24, blue>>24, alpha>>24); }

void tkra_glColor3bv(const tkra_s8 *v)
	{ tkra_glColor3ubI(v[0]<<1, v[1]<<1, v[2]<<1); }
void tkra_glColor3sv(const tkra_s16 *v)
	{ tkra_glColor3ubI(v[0]>>7, v[1]>>7, v[2]>>7); }
void tkra_glColor3iv(const tkra_s32 *v)
	{ tkra_glColor3ubI(v[0]>>23, v[1]>>23, v[2]>>23); }
void tkra_glColor3fv(const tkra_f32 *v)
	{ tkra_glColor3ubI(v[0]*255.0, v[1]*255.0, v[2]*255.0); }
void tkra_glColor3dv(const tkra_f64 *v)
	{ tkra_glColor3ubI(v[0]*255.0, v[1]*255.0, v[2]*255.0); }
void tkra_glColor3ubv(const tkra_u8 *v)
	{ tkra_glColor3ubI(v[0], v[1], v[2]); }
void tkra_glColor3usv(const tkra_u16 *v)
	{ tkra_glColor3ubI(v[0]>>8, v[1]>>8, v[2]>>8); }
void tkra_glColor3uiv(const tkra_u32 *v)
	{ tkra_glColor3ubI(v[0]>>24, v[1]>>24, v[2]>>24); }

void tkra_glColor4bv(const tkra_s8 *v)
	{ tkra_glColor4ubI(v[0]<<1, v[1]<<1, v[2]<<1, v[3]<<1); }
void tkra_glColor4sv(const tkra_s16 *v)
	{ tkra_glColor4ubI(v[0]>>7, v[1]>>7, v[2]>>7, v[3]>>7); }
void tkra_glColor4iv(const tkra_s32 *v)
	{ tkra_glColor4ubI(v[0]>>23, v[1]>>23, v[2]>>23, v[3]>>23); }
void tkra_glColor4fv(const tkra_f32 *v)
	{ tkra_glColor4ubI(v[0]*255.0, v[1]*255.0, v[2]*255.0, v[3]*255.0); }
void tkra_glColor4dv(const tkra_f64 *v)
	{ tkra_glColor4ubI(v[0]*255.0, v[1]*255.0, v[2]*255.0, v[3]*255.0); }
void tkra_glColor4ubv(const tkra_u8 *v)
	{ tkra_glColor4ubI(v[0], v[1], v[2], v[3]); }
void tkra_glColor4usv(const tkra_u16 *v)
	{ tkra_glColor4ubI(v[0]>>8, v[1]>>8, v[2]>>8, v[3]>>8); }
void tkra_glColor4uiv(const tkra_u32 *v)
	{ tkra_glColor4ubI(v[0]>>24, v[1]>>24, v[2]>>24, v[3]>>24); }

void tkra_glTexCoord1s(tkra_s16 s)
	{ tkra_glTexCoord2fI(s/32767.0, 0.0); } 
void tkra_glTexCoord1i(tkra_s32 s)
	{ tkra_glTexCoord2fI(s/2147483647.0, 0.0); } 
void tkra_glTexCoord1f(tkra_f32 s)
	{ tkra_glTexCoord2fI(s, 0.0); } 
void tkra_glTexCoord1d(tkra_f64 s)
	{ tkra_glTexCoord2fI(s, 0.0); } 

void tkra_glTexCoord2s(tkra_s16 s, tkra_s16 t)
	{ tkra_glTexCoord2fI(s/32767.0, t/32767.0); } 
void tkra_glTexCoord2i(tkra_s32 s, tkra_s32 t)
	{ tkra_glTexCoord2fI(s/2147483647.0, t/2147483647.0); } 

void tkra_glTexCoord2f(tkra_f32 s, tkra_f32 t)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_st[0]=s;
	ctx->tkgl_begin_st[1]=t;
//	tkra_glTexCoord2fI(s, t);
} 

void tkra_glTexCoord2d(tkra_f64 s, tkra_f64 t)
	{ tkra_glTexCoord2fI(s, t); } 
void tkra_glTexCoord3s(tkra_s16 s, tkra_s16 t, tkra_s16 r)
	{ tkra_glTexCoord2fI(s/32767.0, t/32767.0); } 
void tkra_glTexCoord3i(tkra_s32 s, tkra_s32 t, tkra_s32 r)
	{ tkra_glTexCoord2fI(s/2147483647.0, t/2147483647.0); } 
void tkra_glTexCoord3f(tkra_f32 s, tkra_f32 t, tkra_f32 r)
	{ tkra_glTexCoord2fI(s, t); } 
void tkra_glTexCoord3d(tkra_f64 s, tkra_f64 t, tkra_f64 r)
	{ tkra_glTexCoord2fI(s, t); } 
void tkra_glTexCoord4s(tkra_s16 s, tkra_s16 t, tkra_s16 r, tkra_s16 q)
	{ tkra_glTexCoord2fI(s/32767.0, t/32767.0); } 
void tkra_glTexCoord4i(tkra_s32 s, tkra_s32 t, tkra_s32 r, tkra_s32 q)
	{ tkra_glTexCoord2fI(s/2147483647.0, t/2147483647.0); } 
void tkra_glTexCoord4f(tkra_f32 s, tkra_f32 t, tkra_f32 r, tkra_f32 q)
	{ tkra_glTexCoord2fI(s, t); } 
void tkra_glTexCoord4d(tkra_f64 s, tkra_f64 t, tkra_f64 r, tkra_f64 q)
	{ tkra_glTexCoord2fI(s, t); } 

void tkra_glTexCoord1sv(const tkra_s16 *v)
	{ tkra_glTexCoord2fI(v[0]/32767.0, 0.0); } 
void tkra_glTexCoord1iv(const tkra_s32 *v)
	{ tkra_glTexCoord2fI(v[0]/2147483647.0, 0.0); } 
void tkra_glTexCoord1fv(const tkra_f32 *v)
	{ tkra_glTexCoord2fI(v[0], 0.0); } 
void tkra_glTexCoord1dv(const tkra_f64 *v)
	{ tkra_glTexCoord2fI(v[0], 0.0); } 

void tkra_glTexCoord2sv(const tkra_s16 *v)
	{ tkra_glTexCoord2fI(v[0]/32767.0, v[1]/32767.0); } 
void tkra_glTexCoord2iv(const tkra_s32 *v)
	{ tkra_glTexCoord2fI(v[0]/2147483647.0, v[1]/2147483647.0); } 

void tkra_glTexCoord2fv(const tkra_f32 *v)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->tkgl_begin_st[0]=v[0];
	ctx->tkgl_begin_st[1]=v[1];

//	tkra_glTexCoord2fI(v[0], v[1]);
} 

void tkra_glTexCoord2dv(const tkra_f64 *v)
	{ tkra_glTexCoord2fI(v[0], v[1]); } 
void tkra_glTexCoord3sv(const tkra_s16 *v)
	{ tkra_glTexCoord2fI(v[0]/32767.0, v[1]/32767.0); } 
void tkra_glTexCoord3iv(const tkra_s32 *v)
	{ tkra_glTexCoord2fI(v[0]/2147483647.0, v[1]/2147483647.0); } 
void tkra_glTexCoord3fv(const tkra_f32 *v)
	{ tkra_glTexCoord2fI(v[0], v[1]); } 
void tkra_glTexCoord3dv(const tkra_f64 *v)
	{ tkra_glTexCoord2fI(v[0], v[1]); } 
void tkra_glTexCoord4sv(const tkra_s16 *v)
	{ tkra_glTexCoord2fI(v[0]/32767.0, v[1]/32767.0); } 
void tkra_glTexCoord4iv(const tkra_s32 *v)
	{ tkra_glTexCoord2fI(v[0]/2147483647.0, v[1]/2147483647.0); } 
void tkra_glTexCoord4fv(const tkra_f32 *v)
	{ tkra_glTexCoord2fI(v[0], v[1]); } 
void tkra_glTexCoord4dv(const tkra_f64 *v)
	{ tkra_glTexCoord2fI(v[0], v[1]); } 


void tkra_glVertex2s(tkra_s16 x, tkra_s16 y)
	{ tkra_glVertex4fI(x/32767.0, y/32767.0, 0.0, 1.0); }
void tkra_glVertex2i(tkra_s32 x, tkra_s32 y)
	{ tkra_glVertex4fI(x/2147483647.0, y/2147483647.0, 0.0, 1.0); }
void tkra_glVertex2f(tkra_f32 x, tkra_f32 y)
	{ tkra_glVertex4fI(x, y, 0.0, 1.0); }
void tkra_glVertex2d(tkra_f64 x, tkra_f64 y)
	{ tkra_glVertex4fI(x, y, 0.0, 1.0); }
void tkra_glVertex3s(tkra_s16 x, tkra_s16 y, tkra_s16 z)
	{ tkra_glVertex4fI(x/32767.0, y/32767.0, z/32767.0, 1.0); }
void tkra_glVertex3i(tkra_s32 x, tkra_s32 y, tkra_s32 z)
	{ tkra_glVertex4fI(x/2147483647.0, y/2147483647.0, z/2147483647.0, 1.0); }
void tkra_glVertex3f(tkra_f32 x, tkra_f32 y, tkra_f32 z)
	{ tkra_glVertex4fI(x, y, z, 1.0); }
void tkra_glVertex3d(tkra_f64 x, tkra_f64 y, tkra_f64 z)
	{ tkra_glVertex4fI(x, y, z, 1.0); }
void tkra_glVertex4s(tkra_s16 x, tkra_s16 y, tkra_s16 z, tkra_s16 w)
	{ tkra_glVertex4fI(x/32767.0, y/32767.0, z/32767.0, w/32767.0); }
void tkra_glVertex4i(tkra_s32 x, tkra_s32 y, tkra_s32 z, tkra_s32 w)
	{ tkra_glVertex4fI(		x/2147483647.0, y/2147483647.0,
							z/2147483647.0, w/2147483647.0); }
void tkra_glVertex4f(tkra_f32 x, tkra_f32 y, tkra_f32 z, tkra_f32 w)
	{ tkra_glVertex4fI(x, y, z, w); }
void tkra_glVertex4d(tkra_f64 x, tkra_f64 y, tkra_f64 z, tkra_f64 w)
	{ tkra_glVertex4fI(x, y, z, w); }

void tkra_glVertex2sv(const tkra_s16 *v)
	{ tkra_glVertex4fI(v[0]/32767.0, v[1]/32767.0, 0.0, 1.0); }
void tkra_glVertex2iv(const tkra_s32 *v)
	{ tkra_glVertex4fI(v[0]/2147483647.0, v[1]/2147483647.0, 0.0, 1.0); }
void tkra_glVertex2fv(const tkra_f32 *v)
	{ tkra_glVertex4fI(v[0], v[1], 0.0, 1.0); }
void tkra_glVertex2dv(const tkra_f64 *v)
	{ tkra_glVertex4fI(v[0], v[1], 0.0, 1.0); }

void tkra_glVertex3sv(const tkra_s16 *v)
	{ tkra_glVertex4fI(v[0]/32767.0, v[1]/32767.0, v[2]/32767.0, 1.0); }
void tkra_glVertex3iv(const tkra_s32 *v)
	{ tkra_glVertex4fI(		v[0]/2147483647.0, v[1]/2147483647.0,
							v[2]/2147483647.0, 1.0); }
void tkra_glVertex3fv(const tkra_f32 *v)
	{ tkra_glVertex4fI(v[0], v[1], v[2], 1.0); }
void tkra_glVertex3dv(const tkra_f64 *v)
	{ tkra_glVertex4fI(v[0], v[1], v[2], 1.0); }
void tkra_glVertex4sv(const tkra_s16 *v)
	{ tkra_glVertex4fI(		v[0]/32767.0, v[1]/32767.0,
							v[2]/32767.0, v[3]/32767.0); }
void tkra_glVertex4iv(const tkra_s32 *v)
	{ tkra_glVertex4fI(		v[0]/2147483647.0, v[1]/2147483647.0,
							v[2]/2147483647.0, v[3]/2147483647.0); }
void tkra_glVertex4fv(const tkra_f32 *v)
	{ tkra_glVertex4fI(v[0], v[1], v[2], v[3]); }
void tkra_glVertex4dv(const tkra_f64 *v)
	{ tkra_glVertex4fI(v[0], v[1], v[2], v[3]); }


void tkra_glNormal3b(tkra_s8 nx, tkra_s8 ny, tkra_s8 nz)
	{ tkra_glNormal3fI(nx/127.0, ny/127.0, nz/127.0); }
void tkra_glNormal3d(tkra_f64 nx, tkra_f64 ny, tkra_f64 nz)
	{ tkra_glNormal3fI(nx, ny, nz); }
void tkra_glNormal3f(tkra_f32 nx, tkra_f32 ny, tkra_f32 nz)
	{ tkra_glNormal3fI(nx, ny, nz); }
void tkra_glNormal3i(tkra_s32 nx, tkra_s32 ny, tkra_s32 nz)
	{ tkra_glNormal3fI(		nx/2147483647.0, ny/2147483647.0,
							nz/2147483647.0); }
void tkra_glNormal3s(tkra_s16 nx, tkra_s16 ny, tkra_s16 nz)
	{ tkra_glNormal3fI(nx/32767.0, ny/32767.0, nz/32767.0); }

void tkra_glNormal3bv(const tkra_s8 *v)
	{ tkra_glNormal3fI(v[0]/127.0, v[1]/127.0, v[2]/127.0); }
void tkra_glNormal3dv(const tkra_f64 *v)
	{ tkra_glNormal3fI(v[0], v[1], v[2]); }
void tkra_glNormal3fv(const tkra_f32 *v)
	{ tkra_glNormal3fI(v[0], v[1], v[2]); }
void tkra_glNormal3iv(const tkra_s32 *v)
	{ tkra_glNormal3fI(		v[0]/2147483647.0, v[1]/2147483647.0,
							v[2]/2147483647.0); }
void tkra_glNormal3sv(const tkra_s16 *v)
	{ tkra_glNormal3fI(v[0]/32767.0, v[1]/32767.0, v[2]/32767.0); }

void tkra_glArrayElement(int idx)
{
	TKRA_Context *ctx;
	tkra_vec4f	xyz;
	tkra_vec2f	st;
	u32			rgb;
	
	ctx=TKRA_GetCurrentContext();

	if(ctx->stateflag1&TKRA_STFL1_TEXCOORD_ARRAY)
	{
//		st=ctx->VaGetPtr_st(
//			((byte *)(ctx->tkgl_vptr_st_ptr))+
//			(idx*ctx->tkgl_vptr_st_str));

		st=ctx->VaGetPtr_st(
			((byte *)(ctx->vptr->vptr_st_ptr))+
			(idx*ctx->vptr->vptr_st_str));

		ctx->tkgl_begin_st[0]=tkra_v2f_x(st);
		ctx->tkgl_begin_st[1]=tkra_v2f_y(st);
//		tkra_glTexCoord2fv((float *)(&st));
	}

	if(ctx->stateflag1&TKRA_STFL1_COLOR_ARRAY)
	{
//		rgb=ctx->VaGetPtr_rgb(
//			((byte *)(ctx->tkgl_vptr_rgb_ptr))+
//			(idx*ctx->tkgl_vptr_rgb_str));
		rgb=ctx->VaGetPtr_rgb(
			((byte *)(ctx->vptr->vptr_rgb_ptr))+
			(idx*ctx->vptr->vptr_rgb_str));
		rgb=((rgb    )&0xFF00FF00U) |
			((rgb>>16)&0x000000FFU) |
			((rgb<<16)&0x00FF0000U) ;
		ctx->tkgl_begin_rgba=rgb;
//		tkra_glColor4ubv((byte *)(&rgb));
	}

	if(ctx->stateflag1&TKRA_STFL1_VERTEX_ARRAY)
	{
//		xyz=ctx->VaGetPtr_xyz(
//			((byte *)(ctx->tkgl_vptr_xyz_ptr))+
//			(idx*ctx->tkgl_vptr_xyz_str));
		xyz=ctx->VaGetPtr_xyz(
			((byte *)(ctx->vptr->vptr_xyz_ptr))+
			(idx*ctx->vptr->vptr_xyz_str));
//		tkra_glVertex3fv((float *)(&xyz));
		tkra_glVertex4fv((float *)(&xyz));
	}
}


void tkra_glLockArraysEXT (int first, int count)
{
}

void tkra_glUnlockArraysEXT (void)
{
}
