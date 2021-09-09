void tkra_glClearDepth(double depth)
{
	TKRA_Context *ctx;
	int dr;
	ctx=TKRA_GetCurrentContext();
	
	dr=depth*32767;
//	dr=depth*16383;
//	if(dr<0)dr=0;
//	if(dr>65535)dr=65535;

	if(dr<-32767)dr=-32767;
	if(dr> 32767)dr= 32767;

//	if(dr<-16383)dr=-16383;
//	if(dr> 16383)dr= 16383;
	
	ctx->clear_zbuf=dr;
}

void tkra_glDepthFunc(int func)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();

	switch(func)
	{
	case GL_NEVER:		ctx->zat_zfunc=TKRA_ZAT_NV; break;
	case GL_ALWAYS:		ctx->zat_zfunc=TKRA_ZAT_AL; break;
	case GL_EQUAL:		ctx->zat_zfunc=TKRA_ZAT_EQ; break;
	case GL_NOTEQUAL:	ctx->zat_zfunc=TKRA_ZAT_NE; break;
	case GL_LESS:		ctx->zat_zfunc=TKRA_ZAT_LT; break;
	case GL_LEQUAL:		ctx->zat_zfunc=TKRA_ZAT_LE; break;
	case GL_GREATER:	ctx->zat_zfunc=TKRA_ZAT_GT; break;
	case GL_GEQUAL:		ctx->zat_zfunc=TKRA_ZAT_GE; break;
	default:
		__debugbreak();
		break;
	}
	
//	TKRA_SetupDrawBlend(ctx);
	ctx->blend_isready=0;
}

void tkra_glDepthMask(int flag)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	switch(flag)
	{
	case GL_TRUE:
		ctx->stateflag1&=~TKRA_STFL1_NODEPTHWRITE;
		break;
	case GL_FALSE:
		ctx->stateflag1|=TKRA_STFL1_NODEPTHWRITE;
		break;
	default:
		ctx->stateflag1&=~TKRA_STFL1_NODEPTHWRITE;
		break;
	}
//	TKRA_SetupDrawBlend(ctx);
	ctx->blend_isready=0;
}

void tkra_glDepthRange(double near_val, double far_val)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->trans_znear=near_val;
	ctx->trans_zfar=far_val;
	ctx->trans_zrange=far_val-near_val;
	TKRA_RecalcViewport(ctx);
}


void tkra_glClearIndex(float c)
{
	/* NO-OP, Index Color Not Implemented. */
}

void tkra_glClearColor(float red, float green, float blue, float alpha)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->clear_rgb5=tkra_teximg_packrgb555(
		red*255, green*255, blue*255, alpha*255);
	ctx->clear_rgba=tkra_teximg_packrgba(
		red*255, green*255, blue*255, alpha*255);
}

#ifdef __BJX2__
void tkra_memset_word(void *buf, u16 val, int cnt);

__asm {
tkra_memset_word:
	PSHUF.W	R5, 0x00, R18
	MOV		R18, R19
	
	LEA.W	(R4, R6), R7

	ADD		R4, 64, R2
	CMPQGT	R2, R7
	BF		.L1
	.L0:
	MOV.X	R18, (R4, 0)
	MOV.X	R18, (R4, 16)
	MOV.X	R18, (R4, 32)
	MOV.X	R18, (R4, 48)
	MOV		R2, R4
	ADD		R4, 64, R2
	CMPQGT	R2, R7
	BT		.L0
	.L1:

	ADD		R4, 8, R2
	CMPQGT	R2, R7
	BF		.L3
	.L2:
	MOV.Q	R18, (R4)
	MOV		R2, R4
	ADD		R4, 8, R2
	CMPQGT	R2, R7
	BT		.L2
	.L3:

	CMPQGT	R4, R7
	BF		.L5
	.L4:
	MOV.W	R18, (R4)
	ADD		2, R4
	CMPQGT	R4, R7
	BT		.L4
	.L5:

	RTS
}

#else
void tkra_memset_word(void *buf, u16 px, int cnt)
{
	u64 px4;
	u16 *ct, *cte;
	
	ct=buf; cte=ct+cnt;
	px4=px|(px<<16); px4=px4|(px4<<32);
	while((ct+32)<=cte)
	{
		((u64 *)ct)[0]=px4;	((u64 *)ct)[1]=px4;
		((u64 *)ct)[2]=px4;	((u64 *)ct)[3]=px4;
		((u64 *)ct)[4]=px4;	((u64 *)ct)[5]=px4;
		((u64 *)ct)[6]=px4;	((u64 *)ct)[7]=px4;
		ct+=32;
	}
	while((ct+8)<=cte)
	{
		((u64 *)ct)[0]=px4;
		((u64 *)ct)[1]=px4;
		ct+=8;
	}
	while(ct<cte)
		{ *ct++=px; }
}

#endif


void tkra_glClear(unsigned int mask)
{
	TKRA_Context *ctx;
	tkra_rastpixel *rgb, *cct, *ccte;
	tkra_zbufpixel *zbuf, *zct, *zcte;
	byte *sten;
	u64 px4;
	u32 px;
	int xs, ys;
	int i, j, k, n;
	
	ctx=TKRA_GetCurrentContext();

	xs=ctx->screen_xsize;
	ys=ctx->screen_ysize;
	rgb=ctx->screen_rgb;
	zbuf=ctx->screen_zbuf;
	sten=ctx->screen_sten;

	if(mask&TKRA_GL_COLOR_BUFFER_BIT)
	{
		px=ctx->clear_rgb5;
		n=xs*ys;
//		for(i=0; i<n; i++)
//			{ rgb[i]=px; }

		tkra_memset_word(rgb, px, n);

#if 0
		px4=px|(px<<16); px4=px4|(px4<<32);
		cct=rgb;	ccte=rgb+n;
		while((cct+32)<=ccte)
		{
			((u64 *)cct)[0]=px4;	((u64 *)cct)[1]=px4;
			((u64 *)cct)[2]=px4;	((u64 *)cct)[3]=px4;
			((u64 *)cct)[4]=px4;	((u64 *)cct)[5]=px4;
			((u64 *)cct)[6]=px4;	((u64 *)cct)[7]=px4;
			cct+=32;
		}
		while((cct+8)<=ccte)
		{
			((u64 *)cct)[0]=px4;
			((u64 *)cct)[1]=px4;
			cct+=8;
		}
		while(cct<ccte)
			{ *cct++=px; }
#endif
	}

	if(mask&TKRA_GL_DEPTH_BUFFER_BIT)
	{
		px=ctx->clear_zbuf;
//		n=(xs>>1)*(ys>>1);
		n=xs*ys;

		tkra_memset_word(zbuf, px, n);

#if 0
//		for(i=0; i<n; i++)
//			{ zbuf[i]=px; }
		px4=px|(px<<16); px4=px4|(px4<<32);
		zct=zbuf;	zcte=zbuf+n;
		while((zct+32)<=zcte)
		{
			((u64 *)zct)[0]=px4;	((u64 *)zct)[1]=px4;
			((u64 *)zct)[2]=px4;	((u64 *)zct)[3]=px4;
			((u64 *)zct)[4]=px4;	((u64 *)zct)[5]=px4;
			((u64 *)zct)[6]=px4;	((u64 *)zct)[7]=px4;
			zct+=32;
		}
		while((zct+8)<=zcte)
		{
			((u64 *)zct)[0]=px4;
			((u64 *)zct)[1]=px4;
			zct+=8;
		}
		while(zct<zcte)
			{ *zct++=px; }
#endif
	}

	if(mask&TKRA_GL_STENCIL_BUFFER_BIT)
	{
		px=0;
		n=(xs>>1)*(ys>>1);
		for(i=0; i<n; i++)
			{ sten[i]=px; }
	}
}

void tkra_glIndexMask(unsigned int mask)
{
	__debugbreak();
}

void tkra_glColorMask(int red, int green, int blue, int alpha)
{
	__debugbreak();
}

void tkra_glAlphaFunc(int func, float ref)
{
	TKRA_Context *ctx;
	int i;

	ctx=TKRA_GetCurrentContext();

	i=ref*255;
	ctx->zat_cref=(ctx->zat_cref&0x00FFFFFFU)|(i<<24);
	
	switch(func)
	{
	case GL_NEVER:		ctx->zat_alfunc=TKRA_ZAT_NV; break;
	case GL_ALWAYS:		ctx->zat_alfunc=TKRA_ZAT_AL; break;
	case GL_EQUAL:		ctx->zat_alfunc=TKRA_ZAT_EQ; break;
	case GL_NOTEQUAL:	ctx->zat_alfunc=TKRA_ZAT_NE; break;
	case GL_LESS:		ctx->zat_alfunc=TKRA_ZAT_LT; break;
	case GL_LEQUAL:		ctx->zat_alfunc=TKRA_ZAT_LE; break;
	case GL_GREATER:	ctx->zat_alfunc=TKRA_ZAT_GT; break;
	case GL_GEQUAL:		ctx->zat_alfunc=TKRA_ZAT_GE; break;
	default:
		__debugbreak();
		break;
	}
	
//	TKRA_SetupDrawBlend(ctx);
	ctx->blend_isready=0;
}

void tkra_glBlendFunc(int sfactor, int dfactor)
{
	TKRA_Context *ctx;
	int sfn, dfn;
	
	ctx=TKRA_GetCurrentContext();

	sfn=TKRA_BLEND_ONE;
	switch(sfactor)
	{
		case TKRA_GL_ZERO:			sfn=TKRA_BLEND_ZERO;		break;
		case TKRA_GL_ONE:			sfn=TKRA_BLEND_ONE;			break;
		case TKRA_GL_SRC_COLOR:		sfn=TKRA_BLEND_SRC_COLOR;	break;
		case TKRA_GL_SRC_ALPHA:		sfn=TKRA_BLEND_SRC_ALPHA;	break;
		case TKRA_GL_DST_ALPHA:		sfn=TKRA_BLEND_DST_ALPHA;	break;
		case TKRA_GL_DST_COLOR:		sfn=TKRA_BLEND_DST_COLOR;	break;
		case TKRA_GL_ONE_MINUS_SRC_COLOR:
			sfn=TKRA_BLEND_ONE_MINUS_SRC_COLOR;	break;
		case TKRA_GL_ONE_MINUS_SRC_ALPHA:
			sfn=TKRA_BLEND_ONE_MINUS_SRC_ALPHA;	break;
		case TKRA_GL_ONE_MINUS_DST_ALPHA:
			sfn=TKRA_BLEND_ONE_MINUS_DST_ALPHA;	break;
		case TKRA_GL_ONE_MINUS_DST_COLOR:
			sfn=TKRA_BLEND_ONE_MINUS_DST_COLOR;	break;
		default:
			__debugbreak();
			break;
	}

	dfn=TKRA_BLEND_ZERO;
	switch(dfactor)
	{
		case TKRA_GL_ZERO:			dfn=TKRA_BLEND_ZERO;		break;
		case TKRA_GL_ONE:			dfn=TKRA_BLEND_ONE;			break;
		case TKRA_GL_SRC_COLOR:		dfn=TKRA_BLEND_SRC_COLOR;	break;
		case TKRA_GL_SRC_ALPHA:		dfn=TKRA_BLEND_SRC_ALPHA;	break;
		case TKRA_GL_DST_ALPHA:		dfn=TKRA_BLEND_DST_ALPHA;	break;
		case TKRA_GL_DST_COLOR:		dfn=TKRA_BLEND_DST_COLOR;	break;
		case TKRA_GL_ONE_MINUS_SRC_COLOR:
			dfn=TKRA_BLEND_ONE_MINUS_SRC_COLOR;	break;
		case TKRA_GL_ONE_MINUS_SRC_ALPHA:
			dfn=TKRA_BLEND_ONE_MINUS_SRC_ALPHA;	break;
		case TKRA_GL_ONE_MINUS_DST_ALPHA:
			dfn=TKRA_BLEND_ONE_MINUS_DST_ALPHA;	break;
		case TKRA_GL_ONE_MINUS_DST_COLOR:
			dfn=TKRA_BLEND_ONE_MINUS_DST_COLOR;	break;
		default:
			__debugbreak();
			break;
	}
	
	ctx->blend_sfunc=sfn;
	ctx->blend_dfunc=dfn;
	
//	TKRA_SetupDrawBlend(ctx);
	ctx->blend_isready=0;
}

void tkra_glLogicOp(int opcode)
{
	/* NO-OP, Exclude Feature */
	__debugbreak();
}

void tkra_glCullFace(int mode)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	ctx->stateflag1&=~(TKRA_STFL1_CULL_FT|TKRA_STFL1_CULL_BK);
	switch(mode)
	{
	case GL_FRONT:
		ctx->stateflag1|=TKRA_STFL1_CULL_FT;
		break;
	case GL_BACK:
		ctx->stateflag1|=TKRA_STFL1_CULL_BK;
		break;
	case GL_FRONT_AND_BACK:
		ctx->stateflag1|=TKRA_STFL1_CULL_FT|TKRA_STFL1_CULL_BK;
		break;
	}
}

void tkra_glFrontFace(int mode)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	switch(mode)
	{
	case GL_CW:
		ctx->stateflag1|=TKRA_STFL1_CULL_CW;
		break;
	case GL_CCW:
		ctx->stateflag1&=~TKRA_STFL1_CULL_CW;
		break;
	}
}

void tkra_glPointSize(float size)
{
	/* NO-OP */
}

void tkra_glLineWidth(float width)
{
	/* NO-OP */
}

void tkra_glLineStipple(int factor, unsigned short pattern)
{
	/* NO-OP */
}

void tkra_glPolygonMode(int face, int mode)
{
}

void tkra_glPolygonOffset(float factor, float units)
{
	/* NO-OP */
}

void tkra_glPolygonStipple(const byte *mask)
{
	/* NO-OP */
}

void tkra_glGetPolygonStipple(byte *mask)
{
	/* NO-OP */
}

void tkra_glEdgeFlag(int flag)
{
	__debugbreak();
}

void tkra_glEdgeFlagv(const int *flag)
{
	__debugbreak();
}

void tkra_glScissor(int x, int y, int width, int height)
{
}

void tkra_glClipPlane(int plane, const double *equation)
{
//	__debugbreak();
}

void tkra_glGetClipPlane(int plane, double *equation)
{
	__debugbreak();
}

void tkra_glDrawBuffer(int mode)
{
}

void tkra_glReadBuffer(int mode)
{
}

void tkra_glEnable(int cap)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	switch(cap)
	{
	case TKRA_GL_DEPTH_TEST:
		ctx->stateflag1|=TKRA_STFL1_DEPTHTEST;
//		TKRA_SetupDrawBlend(ctx);
		break;
	case TKRA_GL_ALPHA_TEST:
		ctx->stateflag1|=TKRA_STFL1_ALPHATEST;
//		TKRA_SetupDrawBlend(ctx);
		break;
	case TKRA_GL_BLEND:
		ctx->stateflag1|=TKRA_STFL1_BLEND;
//		TKRA_SetupDrawBlend(ctx);
		break;
	case GL_CULL_FACE:
		ctx->stateflag1|=TKRA_STFL1_CULLFACE;
		break;
	case GL_TEXTURE_2D:
		break;
	case GL_SCISSOR_TEST:
		break;
	case GL_STENCIL_TEST:
		break;
	case GL_CLIP_PLANE0:
		break;
	case GL_POLYGON_OFFSET_FILL:
		break;
	default:
		__debugbreak();
		break;
	}

	ctx->blend_isready=0;
}

void tkra_glDisable(int cap)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	switch(cap)
	{
//	case TKRA_GL_DEPTH_TEST:
	case GL_DEPTH_TEST:
		ctx->stateflag1&=~TKRA_STFL1_DEPTHTEST;
		break;
//	case TKRA_GL_ALPHA_TEST:
	case GL_ALPHA_TEST:
		ctx->stateflag1&=~TKRA_STFL1_ALPHATEST;
		break;
//	case TKRA_GL_BLEND:
	case GL_BLEND:
		ctx->stateflag1&=~TKRA_STFL1_BLEND;
//		TKRA_SetupDrawBlend(ctx);
		break;
	case GL_CULL_FACE:
		ctx->stateflag1&=~TKRA_STFL1_CULLFACE;
		break;
	case GL_TEXTURE_2D:
		break;
	case GL_SCISSOR_TEST:
		break;
	case GL_STENCIL_TEST:
		break;
	case GL_CLIP_PLANE0:
		break;
	case GL_POLYGON_OFFSET_FILL:
		break;
	default:
		__debugbreak();
		break;
	}

	ctx->blend_isready=0;
}

int tkra_glIsEnabled(int cap)
{
	return(0);
}

void tkra_glEnableClientState(int cap)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	switch(cap)
	{
	case GL_VERTEX_ARRAY:
		ctx->stateflag1|=TKRA_STFL1_VERTEX_ARRAY;
		break;
	case GL_NORMAL_ARRAY:
		ctx->stateflag1|=TKRA_STFL1_NORMAL_ARRAY;
		break;
	case GL_COLOR_ARRAY:
		ctx->stateflag1|=TKRA_STFL1_COLOR_ARRAY;
		break;
	case GL_INDEX_ARRAY:
		ctx->stateflag1|=TKRA_STFL1_INDEX_ARRAY;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		ctx->stateflag1|=TKRA_STFL1_TEXCOORD_ARRAY;
		break;
	case GL_EDGE_FLAG_ARRAY:
		ctx->stateflag1|=TKRA_STFL1_EDGEFLAG_ARRAY;
		break;
	}
}

void tkra_glDisableClientState(int cap)
{
	TKRA_Context *ctx;

	ctx=TKRA_GetCurrentContext();
	
	switch(cap)
	{
	case GL_VERTEX_ARRAY:
		ctx->stateflag1&=~TKRA_STFL1_VERTEX_ARRAY;
		break;
	case GL_NORMAL_ARRAY:
		ctx->stateflag1&=~TKRA_STFL1_NORMAL_ARRAY;
		break;
	case GL_COLOR_ARRAY:
		ctx->stateflag1&=~TKRA_STFL1_COLOR_ARRAY;
		break;
	case GL_INDEX_ARRAY:
		ctx->stateflag1&=~TKRA_STFL1_INDEX_ARRAY;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		ctx->stateflag1&=~TKRA_STFL1_TEXCOORD_ARRAY;
		break;
	case GL_EDGE_FLAG_ARRAY:
		ctx->stateflag1&=~TKRA_STFL1_EDGEFLAG_ARRAY;
		break;
	}
}

void tkra_glGetBooleanv(int pname, int *params)
{
	*params = 0;
}

void tkra_glGetDoublev(int pname, double *params)
{
	*params = 0;
}

void tkra_glGetFloatv(int pname, float *params)
{
	*params = 0;
}

void tkra_glGetIntegerv(int pname, int *params)
{
	int val;
	
	val=0;

	switch(pname)
	{
	case GL_MAX_TEXTURE_SIZE:
		val=256; break;
	default:
		__debugbreak();
		break;
	}

	*params = val;
}

void tkra_glPushAttrib(unsigned int mask)
{
	__debugbreak();
}

void tkra_glPopAttrib(void)
{
	__debugbreak();
}

void tkra_glPushClientAttrib(unsigned int mask)
{
	__debugbreak();
}

void tkra_glPopClientAttrib(void)
{
	__debugbreak();
}

int tkra_glRenderMode(int mode)
{
	return(0);
}

int tkra_glGetError(void)
{
	return(0);
}

char *tkra_glGetString(int name)
{
	if(name==TKRA_GL_VENDOR)
		return("BGBTech");
	if(name==TKRA_GL_RENDERER)
		return("TKRastGL");
	if(name==TKRA_GL_VERSION)
		return("1.1");
	if(name==TKRA_GL_EXTENSIONS)
		return("");
	return(NULL);
}

void tkra_glFinish(void)
{
}

void tkra_glFlush(void)
{
}

void tkra_glHint(int target, int mode)
{
}

