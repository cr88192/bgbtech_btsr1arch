#include <bgbrasw.h>

int raswgl_barf_arrayStepSize(
	int vsize, int vtype, int ivstep)
{
	int isz, vstep;
	
	vstep=ivstep;
	if(!vstep)
	{
		switch(vtype)
		{
		case GL_BYTE:				isz=sizeof(byte); break;
		case GL_UNSIGNED_BYTE:		isz=sizeof(byte); break;
		case GL_SHORT:				isz=sizeof(short); break;
		case GL_UNSIGNED_SHORT:		isz=sizeof(short); break;
		case GL_INT:				isz=sizeof(int); break;
		case GL_UNSIGNED_INT:		isz=sizeof(int); break;
		case GL_FLOAT:				isz=sizeof(float); break;
		case GL_DOUBLE:				isz=sizeof(double); break;
//		case GL_HALF_FLOAT:			isz=sizeof(u16); break;
		default:					isz=sizeof(float); break;
		}
		vstep=vsize*isz;
	}
	return(vstep);
}

void RASWGL_EnableClientState(RASWGL_Context *ctx, int cap)
{
	switch(cap)
	{
	case GL_VERTEX_ARRAY:
		ctx->csda_enabled|=BGBRASW_CSENABLE_VERTEX_ARRAY;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		ctx->csda_enabled|=BGBRASW_CSENABLE_COORD_ARRAY;
		break;
	case GL_NORMAL_ARRAY:
		ctx->csda_enabled|=BGBRASW_CSENABLE_NORMAL_ARRAY;
		break;
	case GL_COLOR_ARRAY:
		ctx->csda_enabled|=BGBRASW_CSENABLE_COLOR_ARRAY;
		break;
	case GL_INDEX_ARRAY:
		ctx->csda_enabled|=BGBRASW_CSENABLE_INDEX_ARRAY;
		break;
	case GL_EDGE_FLAG_ARRAY:
		ctx->csda_enabled|=BGBRASW_CSENABLE_EDGEFL_ARRAY;
		break;
	default:
		break;
	}
}

void RASWGL_DisableClientState(RASWGL_Context *ctx, int cap)
{
	switch(cap)
	{
	case GL_VERTEX_ARRAY:
		ctx->csda_enabled&=~BGBRASW_CSENABLE_VERTEX_ARRAY;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		ctx->csda_enabled&=~BGBRASW_CSENABLE_COORD_ARRAY;
		break;
	case GL_NORMAL_ARRAY:
		ctx->csda_enabled&=~BGBRASW_CSENABLE_NORMAL_ARRAY;
		break;
	case GL_COLOR_ARRAY:
		ctx->csda_enabled&=~BGBRASW_CSENABLE_COLOR_ARRAY;
		break;
	case GL_INDEX_ARRAY:
		ctx->csda_enabled&=~BGBRASW_CSENABLE_INDEX_ARRAY;
		break;
	case GL_EDGE_FLAG_ARRAY:
		ctx->csda_enabled&=~BGBRASW_CSENABLE_EDGEFL_ARRAY;
		break;
	default:
		break;
	}
}

void RASWGL_VertexPointer(RASWGL_Context *ctx, 
		int size, int type, int stride, const void *ptr)
{
	ctx->csda_xyzsize=size;
	ctx->csda_xyztype=type;
	ctx->csda_xyzstep=raswgl_barf_arrayStepSize(size, type, stride);
	ctx->csda_xyz=(void *)ptr;
//	RaGlSetErrorNopStub();
}

void RASWGL_TexCoordPointer(RASWGL_Context *ctx, 
		int size, int type, int stride, const void *ptr)
{
	ctx->csda_stsize=size;
	ctx->csda_sttype=type;
	ctx->csda_ststep=raswgl_barf_arrayStepSize(size, type, stride);
	ctx->csda_st=(void *)ptr;
//	RaGlSetErrorNopStub();
}

void RASWGL_NormalPointer(RASWGL_Context *ctx, 
		int type, int stride, const void *ptr)
{
	ctx->csda_normsize=3;
	ctx->csda_normtype=type;
	ctx->csda_normstep=raswgl_barf_arrayStepSize(3, type, stride);
	ctx->csda_norm=(void *)ptr;

//	RaGlSetErrorNopStub();
}
void RASWGL_ColorPointer(RASWGL_Context *ctx, 
		int size, int type, int stride, const void *ptr)
{
	ctx->csda_rgbsize=size;
	ctx->csda_rgbtype=type;
	ctx->csda_rgbstep=raswgl_barf_arrayStepSize(size, type, stride);
	ctx->csda_rgb=(void *)ptr;

//	RaGlSetErrorNopStub();
}

void RASWGL_IndexPointer(RASWGL_Context *ctx, 
		int type, int stride, const void *ptr)
	{ RaGlSetErrorNopStub(); }
void RASWGL_EdgeFlagPointer(RASWGL_Context *ctx, 
		int stride, const void *ptr)
	{ RaGlSetErrorNopStub(); }

void RASWGL_GetPointerv(RASWGL_Context *ctx, 
		int pname, void **params)
	{ RaGlSetErrorNopStub(); }

void RASWGL_ArrayElement(RASWGL_Context *ctx, int i)
{
	void *ptr;

	if(ctx->csda_enabled&BGBRASW_CSENABLE_COORD_ARRAY)
	{
		ptr=(byte *)ctx->csda_st+i*ctx->csda_ststep;
		RASWGL_TexCoord_Ptrv(ctx,
			ctx->csda_stsize,
			ctx->csda_sttype,
			ptr);
	}

	if(ctx->csda_enabled&BGBRASW_CSENABLE_NORMAL_ARRAY)
	{
		ptr=(byte *)ctx->csda_norm+i*ctx->csda_normstep;
		RASWGL_Normal_Ptrv(ctx,
			ctx->csda_normsize,
			ctx->csda_normtype,
			ptr);
	}

	if(ctx->csda_enabled&BGBRASW_CSENABLE_COLOR_ARRAY)
	{
		ptr=(byte *)ctx->csda_rgb+i*ctx->csda_rgbstep;
		RASWGL_Color_Ptrv(ctx,
			ctx->csda_rgbsize,
			ctx->csda_rgbtype,
			ptr);
	}

	if(ctx->csda_enabled&BGBRASW_CSENABLE_VERTEX_ARRAY)
	{
		ptr=(byte *)ctx->csda_xyz+i*ctx->csda_xyzstep;
		RASWGL_Vertex_Ptrv(ctx,
			ctx->csda_xyzsize,
			ctx->csda_xyztype,
			ptr);
	}

//	RaGlSetErrorNopStub();
}

void RASWGL_DrawArrays(RASWGL_Context *ctx, 
		int mode, int first, int count)
{
	int i;

	RASWGL_Begin(ctx, mode);

	for(i=0; i<count; i++)
		{ RASWGL_ArrayElement(ctx, first+i); }

	RASWGL_End(ctx);
//	RaGlSetErrorNopStub();
}

void RASWGL_DrawElements(RASWGL_Context *ctx, 
		int mode, int count, int type, const void *indices)
{
	int i;

	if(count<=0)
		return;
	if((indices>=NULL) && (indices<(void *)0x00100000))
	{
		return;
	}

	RASWGL_Begin(ctx, mode);

	switch(type)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		for(i=0; i<count; i++)
			{ RASWGL_ArrayElement(ctx, ((byte *)indices)[i]); }
		break;
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		for(i=0; i<count; i++)
			{ RASWGL_ArrayElement(ctx, ((u16 *)indices)[i]); }
		break;
	case GL_INT:
	case GL_UNSIGNED_INT:
		for(i=0; i<count; i++)
			{ RASWGL_ArrayElement(ctx, ((int *)indices)[i]); }
		break;
	}

	RASWGL_End(ctx);
//	RaGlSetErrorNopStub();
}

void RASWGL_InterleavedArrays(RASWGL_Context *ctx, 
		int format, int stride, const void *pointer)
	{ RaGlSetErrorNopStub(); }

