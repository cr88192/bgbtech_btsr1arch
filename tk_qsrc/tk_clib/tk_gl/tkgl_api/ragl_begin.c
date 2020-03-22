#include <tkgl/bgbrasw.h>

RASWGL_Context *RASWGL_CreateContext(int xs, int ys, int flags)
{
	RASWGL_Context *tmp;

	tmp=tkgl_malloc(sizeof(RASWGL_Context));
	tmp->ractx=BGBRASW_AllocContext();

	tmp->depth_near=0.0;
	tmp->depth_far=1.0;

#if 0
	tmp->ractx_thread[0]=BGBRASW_CreateChildThreadContext(tmp->ractx);
	tmp->ractx_thread[1]=BGBRASW_CreateChildThreadContext(tmp->ractx);
//	tmp->ractx_thread[2]=BGBRASW_CreateChildThreadContext(tmp->ractx);
//	tmp->ractx_thread[3]=BGBRASW_CreateChildThreadContext(tmp->ractx);
//	tmp->ractx_n_thread=3;
	tmp->ractx_n_thread=2;
//	tmp->ractx_n_thread=4;
#endif

	BGBRASW_SetupRenderBasic(tmp->ractx, xs, ys, flags);
	RASWGL_Init(tmp);
	
	return(tmp);
}

void RASWGL_Finish(RASWGL_Context *ctx)
{
	BGBRASW_Finish(ctx->ractx);
}

void RASWGL_Flush(RASWGL_Context *ctx)
{
	BGBRASW_Flush(ctx->ractx);
}

void RASWGL_GetFloatv(RASWGL_Context *ctx,
	int pname, float *params)
{
	switch(pname)
	{
	case GL_MODELVIEW:
	case GL_MODELVIEW_MATRIX:
		Mat4F_Copy(ctx->modelview_matrix, params);
		break;
	case GL_PROJECTION:
	case GL_PROJECTION_MATRIX:
		Mat4F_Copy(ctx->projection_matrix, params);
		break;
	default:
		*(int *)-1=-1;
		break;
	}
}

void RASWGL_GetIntegerv(RASWGL_Context *ctx,
	int pname, int *params)
{
	switch(pname)
	{
	case GL_MAX_TEXTURE_SIZE:
		*params=4096;
		break;
	default:
		*(int *)-1=-1;
		break;
	}
}

void RASWGL_MatrixMode(RASWGL_Context *ctx, int mode)
{
	switch(mode)
	{
	case GL_MODELVIEW:
	case GL_MODELVIEW_MATRIX:
		ctx->matrix=ctx->modelview_matrix;
		ctx->matrixstack=ctx->modelview_stack;
		ctx->matrixstackpos=&(ctx->modelview_stackpos);
		break;
	case GL_PROJECTION:
	case GL_PROJECTION_MATRIX:
		ctx->matrix=ctx->projection_matrix;
		ctx->matrixstack=ctx->projection_stack;
		ctx->matrixstackpos=&(ctx->projection_stackpos);
		break;
	default:
		*(int *)-1=-1;
		break;
	}
}

void RASWGL_LoadIdentityMatrix(RASWGL_Context *ctx)
{
	RASWGL_Init(ctx);
	Mat4F_Identity(ctx->matrix);
//	glMatrixMode(GL_MODELVIEW_MATRIX);
//	pdglModelviewMatrix();
//	pdglLoadIdentity();
}

void RASWGL_PushMatrix(RASWGL_Context *ctx)
{
	int i, j;

	RASWGL_Init(ctx);

	if((*ctx->matrixstackpos)<0)
	{
		Mat4F_Identity(ctx->matrix);
		(*ctx->matrixstackpos)=0;
		return;
	}

	if((*ctx->matrixstackpos)>=16)
	{
		*(int *)-1=-1;
	}

	i=(*ctx->matrixstackpos)++;
	for(j=0; j<16; j++)
		ctx->matrixstack[i*16+j]=ctx->matrix[j];
}

void RASWGL_PopMatrix(RASWGL_Context *ctx)
{
	int i, j;

//	RASWGL_Init();

	i=--(*ctx->matrixstackpos);
	if(i<0)
	{
		Mat4F_Identity(ctx->matrix);
		ctx->matrixstackpos=0;
		return;
	}

	for(j=0; j<16; j++)
		ctx->matrix[j]=ctx->matrixstack[i*16+j];
}

void RASWGL_LoadMatrix(RASWGL_Context *ctx, float *mat)
{
	Mat4F_Copy(mat, ctx->matrix);
}

void RASWGL_MultMatrix(RASWGL_Context *ctx, float *mat)
{
	float tmat[16];

//	Mat4F_MatMult(ctx->matrix, mat, tmat);
	Mat4F_MatMultT(ctx->matrix, mat, tmat);
	Mat4F_Copy(tmat, ctx->matrix);
}

void RASWGL_MultMatrixT(RASWGL_Context *ctx, float *mat)
{
	float tmat[16];

	Mat4F_MatMult(ctx->matrix, mat, tmat);
//	Mat4F_MatMultT(ctx->matrix, mat, tmat);
	Mat4F_Copy(tmat, ctx->matrix);
}

void RASWGL_PushMultMatrix(RASWGL_Context *ctx, float *mat)
{
	RASWGL_PushMatrix(ctx);
	RASWGL_MultMatrix(ctx, mat);
}

void RASWGL_Translatef(
	RASWGL_Context *ctx, float x, float y, float z)
{
	float tmat[16];

	Mat4F_Identity(tmat);
	tmat[12]=x; tmat[13]=y; tmat[14]=z;
	RASWGL_MultMatrix(ctx, tmat);
//	pdglTranslatef(x, y, z);
}

void RASWGL_Scalef(
	RASWGL_Context *ctx, float x, float y, float z)
{
	float tmat[16];

	Mat4F_Identity(tmat);
	tmat[0]=x; tmat[5]=y; tmat[10]=z;
	RASWGL_MultMatrix(ctx, tmat);
//	pdglTranslatef(x, y, z);
}

void RASWGL_Rotatef(
	RASWGL_Context *ctx, float th, float x, float y, float z)
{
	float tv[4];
	float tmat[16];

//	Mat4F_Identity(tmat);
//	tmat[0]=x; tmat[5]=y; tmat[10]=z;
	tv[0]=x; tv[1]=y; tv[2]=z; tv[3]=0;
	Mat4F_AxisTo4MatrixT(tv, -th*(M_PI/180), tmat);
	RASWGL_MultMatrix(ctx, tmat);
//	pdglTranslatef(x, y, z);
}

void RASWGL_SetupOrtho(
	RASWGL_Context *ctx,
	float left, float right, float bottom, float top, 
	float nearval, float farval)
{
	float tmat[16];

	Mat4F_SetupOrtho(tmat, left, right, bottom, top, nearval, farval);
	RASWGL_MultMatrix(ctx, tmat);
}

void RASWGL_SetupFrustum(
	RASWGL_Context *ctx,
	float left, float right, float bottom, float top, 
	float nearval, float farval)
{
	float tmat[16];

	Mat4F_SetupFrustum(tmat, left, right, bottom, top, nearval, farval);
	RASWGL_MultMatrix(ctx, tmat);
}

void RASWGL_TransformPoint(
	RASWGL_Context *ctx, float *src, float *dst)
{
//	Mat4F_TransformPoint(src, ctx->modelview_matrix, dst);
	Mat4F_TransformPointT(src, ctx->modelview_matrix, dst);
//	*(int *)-1=-1;
}

void RASWGL_TransformPointLocal(
	RASWGL_Context *ctx, float *src, float *dst)
{
//	Mat4F_TransformPointLocal(src, ctx->modelview_matrix, dst);
	Mat4F_TransformPointLocalT(src, ctx->modelview_matrix, dst);
//	*(int *)-1=-1;
}

void RASWGL_TransformVector(
	RASWGL_Context *ctx, float *src, float *dst)
{
//	Mat4F_TransformVector(src, ctx->modelview_matrix, dst);
	Mat4F_TransformVectorT(src, ctx->modelview_matrix, dst);
}

void RASWGL_TransformVectorLocal(
	RASWGL_Context *ctx, float *src, float *dst)
{
//	Mat4F_TransformVectorT(src, ctx->modelview_matrix, dst);
	Mat4F_TransformVector(src, ctx->modelview_matrix, dst);
}

void RASWGL_Init(RASWGL_Context *ctx)
{
	int i;

	if(ctx->init)
		return;
	ctx->init=1;

	BGBRASW_InitCore();

	i=256;
	ctx->xyzbuf=tkgl_malloc(i*4*sizeof(float));
	ctx->stbuf=tkgl_malloc(i*2*sizeof(float));
	ctx->nbuf=tkgl_malloc(i*4*sizeof(float));
	ctx->clrbuf=tkgl_malloc(i*4*sizeof(float));
	ctx->mvec=i;

	ctx->modelview_stackpos=-1;
	ctx->projection_stackpos=-1;

	RASWGL_MatrixMode(ctx, GL_PROJECTION_MATRIX);
	RASWGL_PushMatrix(ctx);
	RASWGL_LoadIdentityMatrix(ctx);

	RASWGL_MatrixMode(ctx, GL_MODELVIEW_MATRIX);
	RASWGL_PushMatrix(ctx);
	RASWGL_LoadIdentityMatrix(ctx);

	ctx->blend_src=GL_SRC_ALPHA;
	ctx->blend_dst=GL_ONE_MINUS_SRC_ALPHA;
	ctx->alpha_func=GL_ALWAYS;
	ctx->depth_func=GL_LEQUAL;
	ctx->mask_clr=0xFFFFFFFFUL;
	ctx->mask_z=0xFFFFFFFFUL;
	
	ctx->clear_clr=0x00000000UL;
//	ctx->clear_z=0xFFFFFF00UL;
	ctx->clear_z=0x7FFFFF00UL;
	ctx->depth_clear=1.0;
	
	ctx->tabs=BGBRASW_AllocTestBlendData(ctx->ractx);
}

void RASWGL_FlushState(RASWGL_Context *ctx)
{
}

void RASWGL_FlushTexture(RASWGL_Context *ctx)
{
	int i;
}

void RASWGL_SetupTexture(RASWGL_Context *ctx)
{
	float tv[4];
	int txn, tyn;
	int i, j, k, l;

	if(!ctx->texture)
		return;
}

/** Bind current shader texture */
void RASWGL_BindTexture(RASWGL_Context *ctx, int target, int num)
{
//	BGBRASW_Texture *cur;
//	short *pcm;
//	char *s;
//	float f, g, dt;
	int i, j;

	if(num<=0)
	{
		if(!ctx->texture)
			return;
		RASWGL_FlushTexture(ctx);
		ctx->texture=NULL;
		ctx->texid=0;
		return;
	}

	if(!ctx->texture || (ctx->texture->texid!=num))
	{
		RASWGL_FlushTexture(ctx);
		ctx->texture=BGBRASW_LookupTexture(ctx->ractx, num);
		ctx->texid=num;
	}
}

void RASWGL_DeleteTextures(RASWGL_Context *ctx, int cnt, int *tex)
{
	int i, j;
	
	BGBRASW_Finish(ctx->ractx);
	for(i=0; i<cnt; i++)
	{
		BGBRASW_DeleteTexture(ctx->ractx, tex[i]);
	}
}

void RASWGL_GenTextures(RASWGL_Context *ctx, int cnt, int *tex)
{
	int i, j;
	
	BGBRASW_Finish(ctx->ractx);
	for(i=0; i<cnt; i++)
	{
		tex[i]=BGBRASW_AllocTextureID(ctx->ractx);
	}
}

/** Get currently bound texture */
int RASWGL_GetCurrentTexture(RASWGL_Context *ctx)
{
	if(ctx->texture)
		return(ctx->texture->texid);
	return(0);
}

/** Get texture image for currently bound texture. */
int RASWGL_GetCurrentTextureImage(RASWGL_Context *ctx)
{
	if(ctx->texture)
	{
		return(ctx->texture->texid);
	}
	return(0);
}

int RASWGL_GetCurrentTextureFlags(RASWGL_Context *ctx)
{
	if(ctx->texture)
		return(ctx->texture->flags);
	return(0);
}

int RASWGL_GetTextureFlags(RASWGL_Context *ctx, int num)
{
	BGBRASW_Texture *cur;
	if(num<=0)
		return(0);
	cur=BGBRASW_LookupTexture(ctx->ractx, num);
	if(cur)return(cur->flags);
	return(0);
}

//void RASWGL_SetCurrentMaterial(void)
//{
//	LBXGL_Texture_SetImageMaterialImg(ctx->texture);
//}

//void RASWGL_SetCurrentLight(LBXGL_Light *light)
//{
//	ctx->light=light;
//}

//void RASWGL_SetCurrentMixerChannel(int num)
//{
//	ctx->sndmixchan=num;
//}

/** Set current blending functions */
void RASWGL_BlendFunc(RASWGL_Context *ctx, int src, int dst)
{
	ctx->blend_src=src;
	ctx->blend_dst=dst;

	ctx->lastblend_src=src;
	ctx->lastblend_dst=dst;

//	glBlendFunc(src, dst);
}

void RASWGL_AlphaFunc(RASWGL_Context *ctx, int func, float ref)
{
	int i;

	ctx->alpha_func=func;
	i=ref*255;
	i=(i<0)?0:((i>255)?255:i);
	ctx->ref_clr=(ctx->ref_clr&0x00FFFFFF)|(i<<24);
}

void RASWGL_StencilFunc(RASWGL_Context *ctx, int func, int ref, int mask)
{
	int i;

	ctx->stencil_func=func;
	ctx->ref_sten=ref;
	ctx->mask_sten=mask;
}

void RASWGL_StencilMask(RASWGL_Context *ctx, int mask)
{
	ctx->mask_sten=mask;
}

void RASWGL_StencilOp(RASWGL_Context *ctx,
	int sfail, int dpfail, int dppass)
{
	int i;

	ctx->stencil_op_sfail=sfail;
	ctx->stencil_op_dpfail=dpfail;
	ctx->stencil_op_dppass=dppass;
}

void RASWGL_ClipPlane(RASWGL_Context *ctx, int pnum, double *plane)
{
	float tv1[4], tv2[4];
	int i, j, n;
	
	n=pnum-GL_CLIP_PLANE0;
	if((n<0) || (n>=16))
	{
		return;
	}

//	Vec4D2F_Copy(plane, tv1);
//	VEC4F_COPY(plane, tv1);
	tv1[0]=plane[0]; tv1[1]=plane[1];
	tv1[2]=plane[2]; tv1[3]=plane[3];

	Mat4F_TransformVector4T(tv1,
		ctx->modelview_matrix, tv2);
	Vec4F_Copy(tv2, ctx->clip_plane+n*4);
}

void RASWGL_Clear(RASWGL_Context *ctx, int mask)
{
	float z, zs;
	int i;
	
	BGBRASW_Finish(ctx->ractx);
	
	if(mask&GL_COLOR_BUFFER_BIT)
	{
		BGBRASW_ClearScreenColor(ctx->ractx,
			ctx->clear_clr);
	}

	if((mask&GL_DEPTH_BUFFER_BIT) ||
		(mask&GL_STENCIL_BUFFER_BIT))
	{
		z=ctx->depth_clear;
		if(z<0)z=0;
		if(z>1.0)z=1.0;
//		i=z*4294967039.0+0.5;
		i=z*2147483647.0+0.5;
		ctx->clear_z=(i&BGBRASW_MASK_DEPTH)|
			(ctx->clear_z&(~BGBRASW_MASK_DEPTH));
	}

	if((mask&GL_DEPTH_BUFFER_BIT) &&
		(mask&GL_STENCIL_BUFFER_BIT))
	{
		BGBRASW_ClearDepthStencil(ctx->ractx,
			0xFFFFFF00);
//		BGBRASW_ClearDepthStencil(ctx->ractx,
//			ctx->clear_z);
	}else if(mask&GL_DEPTH_BUFFER_BIT)
	{
		BGBRASW_ClearDepthOnly(ctx->ractx,
			0xFFFFFF00);
//		BGBRASW_ClearDepthOnly(ctx->ractx,
//			ctx->clear_z);
	}else if(mask&GL_STENCIL_BUFFER_BIT)
	{
		BGBRASW_ClearStencilOnly(ctx->ractx,
			0xFFFFFF00);
//		BGBRASW_ClearStencilOnly(ctx->ractx,
//			ctx->clear_z);
	}

//	ctx->texture=NULL;
}

void RASWGL_Enable(RASWGL_Context *ctx, int cap)
{
	int i, j;

	switch(cap)
	{
	case GL_ALPHA_TEST:
		ctx->caps_enabled|=BGBRASW_ENABLE_ALPHA_TEST;
		break;
	case GL_DEPTH_TEST:
		ctx->caps_enabled|=BGBRASW_ENABLE_DEPTH_TEST;
		break;
	case GL_TEXTURE_2D:
		ctx->caps_enabled|=BGBRASW_ENABLE_TEXTURE_2D;
		break;
	case GL_BLEND:
		ctx->caps_enabled|=BGBRASW_ENABLE_BLEND;
		break;
	case GL_STENCIL_TEST:
		ctx->caps_enabled|=BGBRASW_ENABLE_STENCIL_TEST;
		break;
	case GL_SCISSOR_TEST:
		ctx->caps_enabled|=BGBRASW_ENABLE_SCISSOR_TEST;
		break;
	case GL_CULL_FACE:
		ctx->caps_enabled|=BGBRASW_ENABLE_CULL_FACE;
		break;
	case GL_DEPTH_WRITEMASK:
		ctx->caps_enabled&=~BGBRASW_ENABLE_DEPTH_NOWRITE;
		ctx->mask_z|=BGBRASW_MASK_DEPTH;
		break;

	case GL_FOG:
		ctx->caps_enabled|=BGBRASW_ENABLE_FOG;
		break;
	case GL_LIGHTING:
		ctx->caps_enabled|=BGBRASW_ENABLE_LIGHTING;
		break;
	case GL_NORMALIZE:
		ctx->caps_enabled|=BGBRASW_ENABLE_NORMALIZE;
		break;
	case GL_COLOR_MATERIAL:
		ctx->caps_enabled|=BGBRASW_ENABLE_COLOR_MATERIAL;
		break;
	case GL_TEXTURE_1D:
		ctx->caps_enabled|=BGBRASW_ENABLE_TEXTURE_1D;
		break;
	case GL_POLYGON_OFFSET_FILL:
		ctx->caps_enabled|=BGBRASW_ENABLE_OFFSET_FILL;
		break;
	
	case (GL_CLIP_PLANE0+ 0):	case (GL_CLIP_PLANE0+ 1):
	case (GL_CLIP_PLANE0+ 2):	case (GL_CLIP_PLANE0+ 3):
	case (GL_CLIP_PLANE0+ 4):	case (GL_CLIP_PLANE0+ 5):
	case (GL_CLIP_PLANE0+ 6):	case (GL_CLIP_PLANE0+ 7):
	case (GL_CLIP_PLANE0+ 8):	case (GL_CLIP_PLANE0+ 9):
	case (GL_CLIP_PLANE0+10):	case (GL_CLIP_PLANE0+11):
	case (GL_CLIP_PLANE0+12):	case (GL_CLIP_PLANE0+13):
	case (GL_CLIP_PLANE0+14):	case (GL_CLIP_PLANE0+15):
		i=cap-GL_CLIP_PLANE0;
		ctx->clip_enabled|=1<<i;
		break;

	case (GL_LIGHT0+ 0):	case (GL_LIGHT0+ 1):
	case (GL_LIGHT0+ 2):	case (GL_LIGHT0+ 3):
	case (GL_LIGHT0+ 4):	case (GL_LIGHT0+ 5):
	case (GL_LIGHT0+ 6):	case (GL_LIGHT0+ 7):
	case (GL_LIGHT0+ 8):	case (GL_LIGHT0+ 9):
	case (GL_LIGHT0+10):	case (GL_LIGHT0+11):
	case (GL_LIGHT0+12):	case (GL_LIGHT0+13):
	case (GL_LIGHT0+14):	case (GL_LIGHT0+15):
		i=cap-GL_LIGHT0;
		ctx->lights_enabled|=1<<i;
		break;

	default:
		break;
	}
}

void RASWGL_Disable(RASWGL_Context *ctx, int cap)
{
	int i, j;

	switch(cap)
	{
	case GL_ALPHA_TEST:
		ctx->caps_enabled&=~BGBRASW_ENABLE_ALPHA_TEST;
		break;
	case GL_DEPTH_TEST:
		ctx->caps_enabled&=~BGBRASW_ENABLE_DEPTH_TEST;
		break;
	case GL_TEXTURE_2D:
		ctx->caps_enabled&=~BGBRASW_ENABLE_TEXTURE_2D;
		break;
	case GL_BLEND:
		ctx->caps_enabled&=~BGBRASW_ENABLE_BLEND;
		break;
	case GL_STENCIL_TEST:
		ctx->caps_enabled&=~BGBRASW_ENABLE_STENCIL_TEST;
		break;
	case GL_SCISSOR_TEST:
		ctx->caps_enabled&=~BGBRASW_ENABLE_SCISSOR_TEST;
		break;
	case GL_CULL_FACE:
		ctx->caps_enabled&=~BGBRASW_ENABLE_CULL_FACE;
		break;
	case GL_DEPTH_WRITEMASK:
		ctx->caps_enabled|=BGBRASW_ENABLE_DEPTH_NOWRITE;
		ctx->mask_z&=~BGBRASW_MASK_DEPTH;
		break;

	case GL_FOG:
		ctx->caps_enabled&=~BGBRASW_ENABLE_FOG;
		break;
	case GL_LIGHTING:
		ctx->caps_enabled&=~BGBRASW_ENABLE_LIGHTING;
		break;
	case GL_NORMALIZE:
		ctx->caps_enabled&=~BGBRASW_ENABLE_NORMALIZE;
		break;
	case GL_COLOR_MATERIAL:
		ctx->caps_enabled&=~BGBRASW_ENABLE_COLOR_MATERIAL;
		break;
	case GL_TEXTURE_1D:
		ctx->caps_enabled&=~BGBRASW_ENABLE_TEXTURE_1D;
		break;
	case GL_POLYGON_OFFSET_FILL:
		ctx->caps_enabled&=~BGBRASW_ENABLE_OFFSET_FILL;
		break;

	case (GL_CLIP_PLANE0+ 0):	case (GL_CLIP_PLANE0+ 1):
	case (GL_CLIP_PLANE0+ 2):	case (GL_CLIP_PLANE0+ 3):
	case (GL_CLIP_PLANE0+ 4):	case (GL_CLIP_PLANE0+ 5):
	case (GL_CLIP_PLANE0+ 6):	case (GL_CLIP_PLANE0+ 7):
	case (GL_CLIP_PLANE0+ 8):	case (GL_CLIP_PLANE0+ 9):
	case (GL_CLIP_PLANE0+10):	case (GL_CLIP_PLANE0+11):
	case (GL_CLIP_PLANE0+12):	case (GL_CLIP_PLANE0+13):
	case (GL_CLIP_PLANE0+14):	case (GL_CLIP_PLANE0+15):
		i=cap-GL_CLIP_PLANE0;
		ctx->clip_enabled&=~(1<<i);
		break;

	case (GL_LIGHT0+ 0):	case (GL_LIGHT0+ 1):
	case (GL_LIGHT0+ 2):	case (GL_LIGHT0+ 3):
	case (GL_LIGHT0+ 4):	case (GL_LIGHT0+ 5):
	case (GL_LIGHT0+ 6):	case (GL_LIGHT0+ 7):
	case (GL_LIGHT0+ 8):	case (GL_LIGHT0+ 9):
	case (GL_LIGHT0+10):	case (GL_LIGHT0+11):
	case (GL_LIGHT0+12):	case (GL_LIGHT0+13):
	case (GL_LIGHT0+14):	case (GL_LIGHT0+15):
		i=cap-GL_LIGHT0;
		ctx->lights_enabled&=~(1<<i);
		break;

	default:
		break;
	}
}

int RASWGL_IsEnabled(RASWGL_Context *ctx, int cap)
{
	int i, j;
	int ret;

	switch(cap)
	{
	case GL_ALPHA_TEST:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)!=0;
		break;
	case GL_DEPTH_TEST:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)!=0;
		break;
	case GL_TEXTURE_2D:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_2D)!=0;
		break;
	case GL_BLEND:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_BLEND)!=0;
		break;
	case GL_STENCIL_TEST:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_STENCIL_TEST)!=0;
		break;
	case GL_SCISSOR_TEST:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_SCISSOR_TEST)!=0;
		break;
	case GL_CULL_FACE:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_CULL_FACE)!=0;
		break;
	case GL_DEPTH_WRITEMASK:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_DEPTH_NOWRITE)==0;
		break;

	case GL_FOG:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_FOG)!=0;
		break;
	case GL_LIGHTING:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_LIGHTING)!=0;
		break;
	case GL_NORMALIZE:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_NORMALIZE)!=0;
		break;
	case GL_COLOR_MATERIAL:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_COLOR_MATERIAL)!=0;
		break;
	case GL_TEXTURE_1D:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_1D)!=0;
		break;
	case GL_POLYGON_OFFSET_FILL:
		ret=(ctx->caps_enabled&BGBRASW_ENABLE_OFFSET_FILL)!=0;
		break;

	case (GL_CLIP_PLANE0+ 0):	case (GL_CLIP_PLANE0+ 1):
	case (GL_CLIP_PLANE0+ 2):	case (GL_CLIP_PLANE0+ 3):
	case (GL_CLIP_PLANE0+ 4):	case (GL_CLIP_PLANE0+ 5):
	case (GL_CLIP_PLANE0+ 6):	case (GL_CLIP_PLANE0+ 7):
	case (GL_CLIP_PLANE0+ 8):	case (GL_CLIP_PLANE0+ 9):
	case (GL_CLIP_PLANE0+10):	case (GL_CLIP_PLANE0+11):
	case (GL_CLIP_PLANE0+12):	case (GL_CLIP_PLANE0+13):
	case (GL_CLIP_PLANE0+14):	case (GL_CLIP_PLANE0+15):
		i=cap-GL_CLIP_PLANE0;
		ret=(ctx->clip_enabled&(1<<i))!=0;
		break;

	default:
		ret=0;
		break;
	}
	return(ret);
}

void RASWGL_BeginQuery(RASWGL_Context *ctx, int cap, int id)
{
	BGBRASW_ClearQueryValue(ctx->ractx, id);
	ctx->query_id=id;
}

void RASWGL_EndQuery(RASWGL_Context *ctx, int cap)
{
	ctx->query_id=0;
}

void RASWGL_GetQueryObjectIV(RASWGL_Context *ctx,
	int id, int cap, u32 *rptr)
{
//	ctx->query_id=0;
	if(rptr)
	{
		*rptr=BGBRASW_GetQueryValue(ctx->ractx, id);
	}
}

void RASWGL_ClearTexture(RASWGL_Context *ctx)
{
	ctx->texture=NULL;
}

void RASWGL_BeginPoly(RASWGL_Context *ctx)
{
	RASWGL_Init(ctx);
	ctx->nvec=0;
	ctx->prim=GL_POLYGON;
}

void RASWGL_EndPoly(RASWGL_Context *ctx)
{
	RASWGL_EndPrim(ctx, GL_POLYGON);
}

void RASWGL_Begin(RASWGL_Context *ctx, int prim)
{
	RASWGL_Init(ctx);
	ctx->nvec=0;
	ctx->prim=prim;
}

void RASWGL_End(RASWGL_Context *ctx)
	{ RASWGL_EndPrim(ctx, ctx->prim); }

void RASWGL_EndPrim_DrawArrays(
	RASWGL_Context *ctx, int prim,
	float *xyz, float *st, float *norm, float *rgba,
	int nxyz)
{
	int i;

	RASWGL_TransformDrawArrays(ctx,
		prim, 0, nxyz,
		4, xyz,		2, st,
		4, norm,	4, rgba);

//	i=4*sizeof(float);
//	PDGL_DrawPrim_DrawArraysNormalTexRGB(
//		prim, 0, nxyz,
//		3, GL_FLOAT, i, xyz,
//		2, GL_FLOAT, 0, st,
//		3, GL_FLOAT, i, norm,
//		4, GL_FLOAT, 0, rgba);
}

// int ctx->mtex_layers;

void RASWGL_GetWindowOrigin(
	RASWGL_Context *ctx, int *rxs, int *rys)
{
//	*rxs=lbxgl_cam->v_xo;
//	*rys=lbxgl_cam->v_yo;
//	GfxDrv_GetWindowSize(rxs, rys);
}

void RASWGL_GetWindowSize(
	RASWGL_Context *ctx, int *rxs, int *rys)
{
//	*rxs=lbxgl_cam->v_xs;
//	*rys=lbxgl_cam->v_ys;
//	GfxDrv_GetWindowSize(rxs, rys);
}

void RASWGL_GetWindowMaxSize(
	RASWGL_Context *ctx, int *rxs, int *rys)
{
//	*rxs=lbxgl_cam->v_xs;
//	*rys=lbxgl_cam->v_ys;
//	GfxDrv_GetWindowMaxSize(rxs, rys);
}

void RASWGL_GetWindowTexSize(
	RASWGL_Context *ctx, int *rxs, int *rys)
{
//	*rxs=lbxgl_cam->v_xs;
//	*rys=lbxgl_cam->v_ys;
//	GfxDrv_GetWindowTexSize(rxs, rys);
}

void RASWGL_GetWindowMaxTexSize(
	RASWGL_Context *ctx, int *rxs, int *rys)
{
//	*rxs=lbxgl_cam->v_xs;
//	*rys=lbxgl_cam->v_ys;
//	GfxDrv_GetWindowMaxTexSize(rxs, rys);
}

void RASWGL_EndPrim_DrawTexture(RASWGL_Context *ctx, int prim)
{
	int i, j, k, l;

	RASWGL_EndPrim_DrawArrays(ctx, prim,
		ctx->xyzbuf,
		ctx->stbuf,
		ctx->nbuf,
		ctx->clrbuf,
		ctx->nvec);
}

void RASWGL_EndPrim(RASWGL_Context *ctx, int prim)
{
	RASWGL_EndPrim_DrawTexture(ctx, prim);
}

void RASWGL_VertexDownProject(RASWGL_Context *ctx)
{
	float *s, *t;
	int i, j, k;
}

void RASWGL_VertexCheckExpand(RASWGL_Context *ctx, int pad)
{
	int i;

	if((ctx->nvec+pad)>=ctx->mvec)
	{
		i=ctx->mvec+(ctx->mvec>>1);
		while((ctx->nvec+pad)>=i)
			i=i+(i>>1);

		ctx->xyzbuf=
			bgbrasw_realloc(ctx->xyzbuf, i*4*sizeof(float));
		ctx->stbuf=
			bgbrasw_realloc(ctx->stbuf, i*2*sizeof(float));
		ctx->nbuf=
			bgbrasw_realloc(ctx->nbuf, i*4*sizeof(float));
		ctx->clrbuf=
			bgbrasw_realloc(ctx->clrbuf, i*4*sizeof(float));
		ctx->mvec=i;
	}
}

void RASWGL_Vertex3fv(RASWGL_Context *ctx, float *v)
{
	float *t;
	int i;

	RASWGL_VertexCheckExpand(ctx, 1);

	t=ctx->stbuf+ctx->nvec*2;
//	V2F_COPY(ctx->sttmp, t);
	Vec2F_Copy(ctx->sttmp, t);

	t=ctx->nbuf+ctx->nvec*4;
//	V4F_COPY(ctx->ntmp, t);
	Vec4F_Copy(ctx->ntmp, t);

	t=ctx->xyzbuf+ctx->nvec*4;
//	V3F_COPY(v, t);
	Vec4F_Copy(v, t);
	t[3]=1.0;

	t=ctx->clrbuf+ctx->nvec*4;
//	V4F_COPY(ctx->clr, t);
	Vec4F_Copy(ctx->clr, t);

	ctx->nvec++;
}

void RASWGL_Vertex4fv(RASWGL_Context *ctx, float *v)
{
	float *t;
	int i;

	RASWGL_VertexCheckExpand(ctx, 1);

	t=ctx->stbuf+ctx->nvec*2;
//	V2F_COPY(ctx->sttmp, t);
	Vec2F_Copy(ctx->sttmp, t);

	t=ctx->clrbuf+ctx->nvec*4;
//	V4F_COPY(ctx->clr, t);
	Vec4F_Copy(ctx->clr, t);

	t=ctx->nbuf+ctx->nvec*4;
//	V4F_COPY(ctx->ntmp, t);
	Vec4F_Copy(ctx->ntmp, t);

	t=ctx->xyzbuf+ctx->nvec*4;
//	V4F_COPY(v, t);
	Vec4F_Copy(v, t);

	ctx->nvec++;
}

void RASWGL_TexCoord2fv(RASWGL_Context *ctx, float *v)
{
	float *t;
//	V2F_COPY(v, ctx->sttmp);
	Vec2F_Copy(v, ctx->sttmp);
}

void RASWGL_Normal3fv(RASWGL_Context *ctx, float *v)
{
	float *t;

//	V3F_COPY(v, ctx->ntmp);
	Vec4F_Copy(v, ctx->ntmp);
	ctx->ntmp[3]=0;
}

void RASWGL_Normal4fv(RASWGL_Context *ctx, float *v)
{
	float *t;
//	V4F_COPY(v, ctx->ntmp);
	Vec4F_Copy(v, ctx->ntmp);
}

void RASWGL_Normal3sbv(RASWGL_Context *ctx, sbyte *v)
{
	ctx->ntmp[0]=v[0]*(1.0/127.0);
	ctx->ntmp[1]=v[1]*(1.0/127.0);
	ctx->ntmp[2]=v[2]*(1.0/127.0);
	ctx->ntmp[3]=0;
}

void RASWGL_Color3fv(RASWGL_Context *ctx, float *v)
{
//	V3F_COPY(v, ctx->clr);
	Vec4F_Copy(v, ctx->clr);
	ctx->clr[3]=1.0;
}

void RASWGL_Color4fv(RASWGL_Context *ctx, float *v)
{
//	V4F_COPY(v, ctx->clr);
	Vec4F_Copy(v, ctx->clr);
}

void RASWGL_Color4ubv(RASWGL_Context *ctx, byte *v)
{
	ctx->clr[0]=v[0]*(1.0/255.0);
	ctx->clr[1]=v[1]*(1.0/255.0);
	ctx->clr[2]=v[2]*(1.0/255.0);
	ctx->clr[3]=v[3]*(1.0/255.0);
}

void RASWGL_Color3f(
	RASWGL_Context *ctx, float r, float g, float b)
{
	ctx->clr[0]=r;
	ctx->clr[1]=g;
	ctx->clr[2]=b;
	ctx->clr[3]=1.0;
}

void RASWGL_Color4f(
	RASWGL_Context *ctx, float r, float g, float b, float a)
{
	ctx->clr[0]=r;
	ctx->clr[1]=g;
	ctx->clr[2]=b;
	ctx->clr[3]=a;
}

void RASWGL_TexCoord2f(
	RASWGL_Context *ctx, float s, float t)
{
	ctx->sttmp[0]=s;
	ctx->sttmp[1]=t;
}

void RASWGL_Vertex2f(
	RASWGL_Context *ctx, float x, float y)
{
	float tv[3];
	tv[0]=x; tv[1]=y; tv[2]=0.0; //tv[3]=1.0;
	RASWGL_Vertex3fv(ctx, tv);
}

void RASWGL_Vertex3f(
	RASWGL_Context *ctx, float x, float y, float z)
{
	float tv[3];
	tv[0]=x; tv[1]=y; tv[2]=z; //tv[3]=1.0;
	RASWGL_Vertex3fv(ctx, tv);
}

void RASWGL_Normal3f(
	RASWGL_Context *ctx, float x, float y, float z)
{
	ctx->ntmp[0]=x;
	ctx->ntmp[1]=y;
	ctx->ntmp[2]=z;
	ctx->ntmp[3]=0;
}

void RASWGL_Normal4f(
	RASWGL_Context *ctx, float x, float y, float z, float w)
{
	ctx->ntmp[0]=x;
	ctx->ntmp[1]=y;
	ctx->ntmp[2]=z;
	ctx->ntmp[3]=w;
}

void RASWGL_Vertex4f(
	RASWGL_Context *ctx, float x, float y, float z, float w)
{
	float tv[4];
	tv[0]=x; tv[1]=y; tv[2]=z; tv[3]=w;
	RASWGL_Vertex4fv(ctx, tv);
}

void RASWGL_TexCoord_Ptrv(
	RASWGL_Context *ctx, int size, int type, void *ptr)
{
	const float sc127=1.0/127.0;
	const float sc255=1.0/255.0;
	const float sc32767=1.0/32767.0;
	const float sc65535=1.0/65535.0;

	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	switch(type)
	{
	case GL_BYTE:
		RASWGL_TexCoord2f(ctx,
			((sbyte *)ptr)[0]*sc127,
			((sbyte *)ptr)[1]*sc127);
		break;
	case GL_UNSIGNED_BYTE:
		RASWGL_TexCoord2f(ctx,
			((byte *)ptr)[0]*sc255,
			((byte *)ptr)[1]*sc255);
		break;
	case GL_SHORT:
		RASWGL_TexCoord2f(ctx,
			((s16 *)ptr)[0]*sc32767,
			((s16 *)ptr)[1]*sc32767);
		break;
	case GL_UNSIGNED_SHORT:
		RASWGL_TexCoord2f(ctx,
			((u16 *)ptr)[0]*sc65535,
			((u16 *)ptr)[1]*sc65535);
		break;
	case GL_INT:
		RASWGL_TexCoord2f(ctx,
			((s32 *)ptr)[0]*sc32767,
			((s32 *)ptr)[1]*sc32767);
		break;
	case GL_UNSIGNED_INT:
		RASWGL_TexCoord2f(ctx,
			((s32 *)ptr)[0]*sc32767,
			((s32 *)ptr)[1]*sc32767);
		break;
	case GL_FLOAT:
		RASWGL_TexCoord2fv(ctx, ptr);
		break;
	case GL_DOUBLE:
		RASWGL_TexCoord2f(ctx,
			((double *)ptr)[0],
			((double *)ptr)[1]);
		break;
	case GL_HALF_FLOAT:
		RASWGL_TexCoord2f(ctx,
			RASWGL_HalfToFloat(((u16 *)ptr)[0]),
			RASWGL_HalfToFloat(((u16 *)ptr)[1]));
		break;
	default:
		break;
	}
}

void RASWGL_Normal_Ptrv(
	RASWGL_Context *ctx, int size, int type, void *ptr)
{
	const float sc127=1.0/127.0;
	const float sc255=1.0/255.0;
	const float sc32767=1.0/32767.0;
	const float sc65535=1.0/65535.0;
	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	switch(type)
	{
	case GL_BYTE:
		RASWGL_Normal3f(ctx,
			((sbyte *)ptr)[0]*sc127,
			((sbyte *)ptr)[1]*sc127,
			((sbyte *)ptr)[2]*sc127);
		break;
	case GL_UNSIGNED_BYTE:
		RASWGL_Normal3f(ctx,
			((byte *)ptr)[0]*sc255,
			((byte *)ptr)[1]*sc255,
			((byte *)ptr)[2]*sc255);
		break;
	case GL_SHORT:
		RASWGL_Normal3f(ctx,
			((s16 *)ptr)[0]*sc32767,
			((s16 *)ptr)[1]*sc32767,
			((s16 *)ptr)[2]*sc32767);
		break;
	case GL_UNSIGNED_SHORT:
		RASWGL_Normal3f(ctx,
			((u16 *)ptr)[0]*sc65535,
			((u16 *)ptr)[1]*sc65535,
			((u16 *)ptr)[2]*sc65535);
		break;
	case GL_INT:
		RASWGL_Normal3f(ctx,
			((s32 *)ptr)[0]*sc32767,
			((s32 *)ptr)[1]*sc32767,
			((s32 *)ptr)[2]*sc32767);
		break;
	case GL_UNSIGNED_INT:
		RASWGL_Normal3f(ctx,
			((s32 *)ptr)[0]*sc32767,
			((s32 *)ptr)[1]*sc32767,
			((s32 *)ptr)[2]*sc32767);
		break;
	case GL_FLOAT:
		RASWGL_Normal3fv(ctx, ptr);
		break;
	case GL_DOUBLE:
		RASWGL_Normal3f(ctx,
			((double *)ptr)[0],
			((double *)ptr)[1],
			((double *)ptr)[2]);
		break;
	case GL_HALF_FLOAT:
		RASWGL_Normal3f(ctx,
			RASWGL_HalfToFloat(((u16 *)ptr)[0]),
			RASWGL_HalfToFloat(((u16 *)ptr)[1]),
			RASWGL_HalfToFloat(((u16 *)ptr)[2]));
		break;
	default:
		break;
	}
}

void RASWGL_Color_Ptrv(
	RASWGL_Context *ctx, int size, int type, void *ptr)
{
	const float sc127=1.0/127.0;
	const float sc255=1.0/255.0;
	const float sc32767=1.0/32767.0;
	const float sc65535=1.0/65535.0;

	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	if(size==3)
	{
		switch(type)
		{
		case GL_BYTE:
			RASWGL_Color3f(ctx,
				((sbyte *)ptr)[0]*sc127,
				((sbyte *)ptr)[1]*sc127,
				((sbyte *)ptr)[2]*sc127);
			break;
		case GL_UNSIGNED_BYTE:
			RASWGL_Color3f(ctx,
				((byte *)ptr)[0]*sc255,
				((byte *)ptr)[1]*sc255,
				((byte *)ptr)[2]*sc255);
			break;
		case GL_SHORT:
			RASWGL_Color3f(ctx,
				((s16 *)ptr)[0]*sc32767,
				((s16 *)ptr)[1]*sc32767,
				((s16 *)ptr)[2]*sc32767);
			break;
		case GL_UNSIGNED_SHORT:
			RASWGL_Color3f(ctx,
				((u16 *)ptr)[0]*sc65535,
				((u16 *)ptr)[1]*sc65535,
				((u16 *)ptr)[2]*sc65535);
			break;
		case GL_INT:
			RASWGL_Color3f(ctx,
				((s32 *)ptr)[0]*sc32767,
				((s32 *)ptr)[1]*sc32767,
				((s32 *)ptr)[2]*sc32767);
			break;
		case GL_UNSIGNED_INT:
			RASWGL_Color3f(ctx,
				((s32 *)ptr)[0]*sc32767,
				((s32 *)ptr)[1]*sc32767,
				((s32 *)ptr)[2]*sc32767);
			break;
		case GL_FLOAT:
			RASWGL_Color3fv(ctx, ptr);
			break;
		case GL_DOUBLE:
			RASWGL_Color3f(ctx,
				((double *)ptr)[0],
				((double *)ptr)[1],
				((double *)ptr)[2]);
			break;
		case GL_HALF_FLOAT:
			RASWGL_Color3f(ctx,
				RASWGL_HalfToFloat(((u16 *)ptr)[0]),
				RASWGL_HalfToFloat(((u16 *)ptr)[1]),
				RASWGL_HalfToFloat(((u16 *)ptr)[2]));
			break;
		default:
			break;
		}
	}else if(size==4)
	{
		switch(type)
		{
		case GL_BYTE:
			RASWGL_Color4f(ctx,
				((sbyte *)ptr)[0]*sc127,
				((sbyte *)ptr)[1]*sc127,
				((sbyte *)ptr)[2]*sc127,
				((sbyte *)ptr)[3]*sc127);
			break;
		case GL_UNSIGNED_BYTE:
			RASWGL_Color4f(ctx,
				((byte *)ptr)[0]*sc255,
				((byte *)ptr)[1]*sc255,
				((byte *)ptr)[2]*sc255,
				((byte *)ptr)[3]*sc255);
			break;
		case GL_SHORT:
			RASWGL_Color4f(ctx,
				((s16 *)ptr)[0]*sc32767,
				((s16 *)ptr)[1]*sc32767,
				((s16 *)ptr)[2]*sc32767,
				((s16 *)ptr)[3]*sc32767);
			break;
		case GL_UNSIGNED_SHORT:
			RASWGL_Color4f(ctx,
				((u16 *)ptr)[0]*sc65535,
				((u16 *)ptr)[1]*sc65535,
				((u16 *)ptr)[2]*sc65535,
				((u16 *)ptr)[3]*sc65535);
			break;
		case GL_INT:
			RASWGL_Color4f(ctx,
				((s32 *)ptr)[0]*sc32767,
				((s32 *)ptr)[1]*sc32767,
				((s32 *)ptr)[2]*sc32767,
				((s32 *)ptr)[3]*sc32767);
			break;
		case GL_UNSIGNED_INT:
			RASWGL_Color4f(ctx,
				((s32 *)ptr)[0]*sc32767,
				((s32 *)ptr)[1]*sc32767,
				((s32 *)ptr)[2]*sc32767,
				((s32 *)ptr)[3]*sc32767);
			break;
		case GL_FLOAT:
			RASWGL_Color4fv(ctx, ptr);
			break;
		case GL_DOUBLE:
			RASWGL_Color4f(ctx,
				((double *)ptr)[0],
				((double *)ptr)[1],
				((double *)ptr)[2],
				((double *)ptr)[3]);
			break;
		case GL_HALF_FLOAT:
			RASWGL_Color4f(ctx,
				RASWGL_HalfToFloat(((u16 *)ptr)[0]),
				RASWGL_HalfToFloat(((u16 *)ptr)[1]),
				RASWGL_HalfToFloat(((u16 *)ptr)[2]),
				RASWGL_HalfToFloat(((u16 *)ptr)[3]));
			break;
		default:
			break;
		}
	}
}

void RASWGL_Vertex_Ptrv(
	RASWGL_Context *ctx, int size, int type, void *ptr)
{
	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	if(size==2)
	{
		switch(type)
		{
		case GL_BYTE:
			RASWGL_Vertex2f(ctx,
				((sbyte *)ptr)[0],	((sbyte *)ptr)[1]);
			break;
		case GL_UNSIGNED_BYTE:
			RASWGL_Vertex2f(ctx,
				((byte *)ptr)[0],	((byte *)ptr)[1]);
			break;
		case GL_SHORT:
			RASWGL_Vertex2f(ctx,
				((s16 *)ptr)[0],	((s16 *)ptr)[1]);
			break;
		case GL_UNSIGNED_SHORT:
			RASWGL_Vertex2f(ctx,
				((u16 *)ptr)[0],	((u16 *)ptr)[1]);
			break;
		case GL_INT:
			RASWGL_Vertex2f(ctx,
				((s32 *)ptr)[0],	((s32 *)ptr)[1]);
			break;
		case GL_UNSIGNED_INT:
			RASWGL_Vertex2f(ctx,
				((u32 *)ptr)[0],	((u32 *)ptr)[1]);
			break;
		case GL_FLOAT:
			RASWGL_Vertex2f(ctx,
				((float *)ptr)[0],	((float *)ptr)[1]);
			break;
		case GL_DOUBLE:
			RASWGL_Vertex2f(ctx,
				((double *)ptr)[0],	((double *)ptr)[1]);
			break;
		case GL_HALF_FLOAT:
			RASWGL_Vertex2f(ctx,
				RASWGL_HalfToFloat(((u16 *)ptr)[0]),
				RASWGL_HalfToFloat(((u16 *)ptr)[1]));
			break;
		default:
			break;
		}
	}else if(size==3)
	{
		switch(type)
		{
		case GL_BYTE:
			RASWGL_Vertex3f(ctx,	((sbyte *)ptr)[0],
				((sbyte *)ptr)[1],	((sbyte *)ptr)[2]);
			break;
		case GL_UNSIGNED_BYTE:
			RASWGL_Vertex3f(ctx,	((byte *)ptr)[0],
				((byte *)ptr)[1],	((byte *)ptr)[2]);
			break;
		case GL_SHORT:
			RASWGL_Vertex3f(ctx,	((s16 *)ptr)[0],
				((s16 *)ptr)[1],	((s16 *)ptr)[2]);
			break;
		case GL_UNSIGNED_SHORT:
			RASWGL_Vertex3f(ctx,	((u16 *)ptr)[0],
				((u16 *)ptr)[1],	((u16 *)ptr)[2]);
			break;
		case GL_INT:
			RASWGL_Vertex3f(ctx,	((s32 *)ptr)[0],
				((s32 *)ptr)[1],	((s32 *)ptr)[2]);
			break;
		case GL_UNSIGNED_INT:
			RASWGL_Vertex3f(ctx,	((u32 *)ptr)[0],
				((s32 *)ptr)[1],	((u32 *)ptr)[2]);
			break;
		case GL_FLOAT:
			RASWGL_Vertex3fv(ctx, ptr);
			break;
		case GL_DOUBLE:
			RASWGL_Vertex3f(ctx,		((double *)ptr)[0],
				((double *)ptr)[1],	((double *)ptr)[2]);
			break;
		case GL_HALF_FLOAT:
			RASWGL_Vertex3f(ctx,
				RASWGL_HalfToFloat(((u16 *)ptr)[0]),
				RASWGL_HalfToFloat(((u16 *)ptr)[1]),
				RASWGL_HalfToFloat(((u16 *)ptr)[2]));
			break;
		default:
			break;
		}
	}else if(size==4)
	{
		switch(type)
		{
		case GL_BYTE:
			RASWGL_Vertex4f(ctx,
				((sbyte *)ptr)[0],	((sbyte *)ptr)[1],
				((sbyte *)ptr)[2],	((sbyte *)ptr)[3]);
			break;
		case GL_UNSIGNED_BYTE:
			RASWGL_Vertex4f(ctx,
				((byte *)ptr)[0],	((byte *)ptr)[1],
				((byte *)ptr)[2],	((byte *)ptr)[3]);
			break;
		case GL_SHORT:
			RASWGL_Vertex4f(ctx,
				((s16 *)ptr)[0],	((s16 *)ptr)[1],
				((s16 *)ptr)[2],	((s16 *)ptr)[3]);
			break;
		case GL_UNSIGNED_SHORT:
			RASWGL_Vertex4f(ctx,
				((u16 *)ptr)[0],	((u16 *)ptr)[1],
				((u16 *)ptr)[2],	((u16 *)ptr)[3]);
			break;
		case GL_INT:
			RASWGL_Vertex4f(ctx,
				((s32 *)ptr)[0],	((s32 *)ptr)[1],
				((s32 *)ptr)[2],	((s32 *)ptr)[3]);
			break;
		case GL_UNSIGNED_INT:
			RASWGL_Vertex4f(ctx,
				((s32 *)ptr)[0],	((u32 *)ptr)[1],
				((s32 *)ptr)[2],	((u32 *)ptr)[3]);
			break;
		case GL_FLOAT:
			RASWGL_Vertex4fv(ctx, ptr);
			break;
		case GL_DOUBLE:
			RASWGL_Vertex4f(ctx,
				((double *)ptr)[0],	((double *)ptr)[1],
				((double *)ptr)[2],	((double *)ptr)[3]);
			break;
		case GL_HALF_FLOAT:
			RASWGL_Vertex4f(ctx,
				RASWGL_HalfToFloat(((u16 *)ptr)[0]),
				RASWGL_HalfToFloat(((u16 *)ptr)[1]),
				RASWGL_HalfToFloat(((u16 *)ptr)[2]),
				RASWGL_HalfToFloat(((u16 *)ptr)[3]));
			break;
		default:
			break;
		}
	}
}

void RASWGL_DrawArraysSimpleFlat(
	RASWGL_Context *ctx,
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz)
{
//	PDGL_DrawPrim_DrawArraysSimpleFlat(prim, base, nxyz,
//		xyzsize, xyztype, xyzstep, xyz);
}

#if 0
void RASWGL_DrawArraysNormalFlat(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm)
{
	PDGL_DrawPrim_DrawArraysNormalFlat(prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,
		normsize, normtype, normstep, norm);
}

void RASWGL_DrawArraysNormalFlatRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	PDGL_DrawPrim_DrawArraysNormalFlatRGB(prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,
		normsize, normtype, normstep, norm,
		rgbsize, rgbtype, rgbstep, rgb);
}

void RASWGL_DrawArraysNormalTex(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm)
{
	PDGL_DrawPrim_DrawArraysNormalTex(prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,
		stsize, sttype, ststep, st,
		normsize, normtype, normstep, norm);
}

void RASWGL_DrawArraysNormalTexRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	PDGL_DrawPrim_DrawArraysNormalTexRGB(prim, base, nxyz,
		xyzsize, xyztype, xyzstep, xyz,
		stsize, sttype, ststep, st,
		normsize, normtype, normstep, norm,
		rgbsize, rgbtype, rgbstep, rgb);
}


void RASWGL_DrawElementsSimpleFlat(
	int prim, int nelem, int elemtype, void *elems,
	int xyzsize, int xyztype, int xyzstep, void *xyz)
{
//	PDGL_DrawPrim_DrawArraysSimpleFlat(prim, base, nxyz,
//		xyzsize, xyztype, xyzstep, xyz);
}


void RASWGL_DrawElementsNormalTexRGB(
	int prim, int nelem, int elemtype, void *elems,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	PDGL_DrawPrim_DrawElementsNormalTexRGB(
		prim, nelem, elemtype, elems,
		xyzsize, xyztype, xyzstep, xyz,
		stsize, sttype, ststep, st,
		normsize, normtype, normstep, norm,
		rgbsize, rgbtype, rgbstep, rgb);
}
#endif
