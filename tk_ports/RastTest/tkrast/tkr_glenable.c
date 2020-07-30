void tkra_glClearDepth(double depth)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->clear_zbuf=depth*65535;
}

void tkra_glDepthFunc(int func)
{
}

void tkra_glDepthMask(int flag)
{
}

void tkra_glDepthRange(double near_val, double far_val)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->trans_znear=near_val;
	ctx->trans_zfar=far_val;
	ctx->trans_zrange=far_val-near_val;
}


void tkra_glClearIndex(float c)
{
	/* NO-OP, Index Color Not Implemented. */
}

void tkra_glClearColor(float red, float green, float blue, float alpha)
{
	TKRA_Context *ctx;
	ctx=TKRA_GetCurrentContext();
	ctx->clear_rgb5=tkra_teximg_packrgb555(red, green, blue, alpha);
	ctx->clear_rgba=tkra_teximg_packrgba(red, green, blue, alpha);
}

void tkra_glClear(unsigned int mask)
{
	TKRA_Context *ctx;
	tkra_rastpixel *rgb;
	tkra_zbufpixel *zbuf;
	byte *sten;
	int xs, ys, px;
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
		for(i=0; i<n; i++)
			{ rgb[i]=px; }
	}

	if(mask&TKRA_GL_DEPTH_BUFFER_BIT)
	{
		px=ctx->clear_zbuf;
		n=(xs>>1)*(ys>>1);
		for(i=0; i<n; i++)
			{ zbuf[i]=px; }
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
}

void tkra_glColorMask(int red, int green, int blue, int alpha)
{
}

void tkra_glAlphaFunc(int func, float ref)
{
	/* NO-OP, Alpha Test not implemented. */
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
	}
	
	ctx->blend_sfunc=sfn;
	ctx->blend_dfunc=dfn;
}

void tkra_glLogicOp(int opcode)
{
	/* NO-OP, Exclude Feature */
}

void tkra_glCullFace(int mode)
{
}

void tkra_glFrontFace(int mode)
{
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
}

void tkra_glEdgeFlagv(const int *flag)
{
}

void tkra_glScissor(int x, int y, int width, int height)
{
}

void tkra_glClipPlane(int plane, const double *equation)
{
}

void tkra_glGetClipPlane(int plane, double *equation)
{
}

void tkra_glDrawBuffer(int mode)
{
}

void tkra_glReadBuffer(int mode)
{
}

void tkra_glEnable(int cap)
{
}

void tkra_glDisable(int cap)
{
}

int tkra_glIsEnabled(int cap)
{
	return(0);
}

void tkra_glEnableClientState(int cap)
{
}

void tkra_glDisableClientState(int cap)
{
}

void tkra_glGetBooleanv(int pname, int *params)
{
}

void tkra_glGetDoublev(int pname, double *params)
{
}

void tkra_glGetFloatv(int pname, float *params)
{
}

void tkra_glGetIntegerv(int pname, int *params)
{
}

void tkra_glPushAttrib(unsigned int mask)
{
}

void tkra_glPopAttrib(void)
{
}

void tkra_glPushClientAttrib(unsigned int mask)
{
}

void tkra_glPopClientAttrib(void)
{
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

