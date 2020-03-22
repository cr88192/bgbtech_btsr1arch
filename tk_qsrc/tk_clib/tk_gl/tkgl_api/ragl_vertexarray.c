#include <tkgl/bgbrasw.h>

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
		case GL_HALF_FLOAT:			isz=sizeof(u16); break;
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

void RASWGL_ArrayElements2(RASWGL_Context *ctx, int si, int di, int cnt)
{
	void *ptr;

	if(ctx->csda_enabled&BGBRASW_CSENABLE_COORD_ARRAY)
	{
		ptr=(byte *)ctx->csda_st+si*ctx->csda_ststep;
		RASWGL_TexCoord_ArrPtrv(ctx,
			ctx->csda_stsize,
			ctx->csda_sttype,
			ctx->csda_ststep,
			ptr, di, cnt);
	}

	if(ctx->csda_enabled&BGBRASW_CSENABLE_NORMAL_ARRAY)
	{
		if(ctx->caps_enabled&BGBRASW_ENABLE_LIGHTING)
		{
			ptr=(byte *)ctx->csda_norm+si*ctx->csda_normstep;
			RASWGL_Normal_ArrPtrv(ctx,
				ctx->csda_normsize,
				ctx->csda_normtype,
				ctx->csda_normstep,
				ptr, di, cnt);
		}
	}

	if(ctx->csda_enabled&BGBRASW_CSENABLE_COLOR_ARRAY)
	{
		ptr=(byte *)ctx->csda_rgb+si*ctx->csda_rgbstep;
		RASWGL_Color_ArrPtrv(ctx,
			ctx->csda_rgbsize,
			ctx->csda_rgbtype,
			ctx->csda_rgbstep,
			ptr, di, cnt);
	}

	if(ctx->csda_enabled&BGBRASW_CSENABLE_VERTEX_ARRAY)
	{
		ptr=(byte *)ctx->csda_xyz+si*ctx->csda_xyzstep;
		RASWGL_Vertex_ArrPtrv(ctx,
			ctx->csda_xyzsize,
			ctx->csda_xyztype,
			ctx->csda_xyzstep,
			ptr, di, cnt);
	}

	ctx->nvec+=cnt;

//	RaGlSetErrorNopStub();
}

void RASWGL_DrawArrays(RASWGL_Context *ctx, 
		int mode, int first, int count)
{
	int i;

	RASWGL_Begin(ctx, mode);

	RASWGL_VertexCheckExpand(ctx, count);

//	for(i=0; i<count; i++)
//		{ RASWGL_ArrayElement(ctx, first+i); }

	RASWGL_ArrayElements2(ctx, first, 0, count);

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



void RASWGL_GenCoord_ArrPtrv(
	RASWGL_Context *ctx, int size, int type, int stri, byte *sptr,
	int dstri, float *dptr, int cnt, float pad, int normp)
{
	const float sc127=1.0/127.0;
	const float sc255=1.0/255.0;
	const float sc32767=1.0/32767.0;
	const float sc65535=1.0/65535.0;
	const float sc2G=1.0/2147483647.0;
	const float sc4G=1.0/4294967295.0;
	static float cvttab_sb[256];
	static float cvttab_ub[256];
	static float cvttab_sbn[256];
	static float cvttab_ubn[256];
	static int init=0;
	float sc;
	float *ct, *cvt;
	byte *cs;
	int i;
	
	if(!init)
	{
		init=1;
		
		for(i=0; i<256; i++)
		{
			cvttab_sb[i]=((sbyte)i);
			cvttab_ub[i]=((byte)i);
			cvttab_sbn[i]=((sbyte)i)*sc127;
			cvttab_ubn[i]=((byte)i)*sc255;
		}
	}

	cs=sptr;
	ct=dptr;

	if(size==2)
	{
		switch(type)
		{
		case GL_BYTE:
//			sc=normp?sc127:1.0;
			cvt=normp?cvttab_sbn:cvttab_sb;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{
//				ct[0]=((sbyte *)cs)[0]*sc;
//				ct[1]=((sbyte *)cs)[1]*sc;
				ct[0]=cvt[cs[0]];
				ct[1]=cvt[cs[1]];
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_UNSIGNED_BYTE:
//			sc=normp?sc255:1.0;
			cvt=normp?cvttab_ubn:cvttab_ub;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{
//				ct[0]=((byte *)cs)[0]*sc;
//				ct[1]=((byte *)cs)[1]*sc;
				ct[0]=cvt[cs[0]];
				ct[1]=cvt[cs[1]];
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_SHORT:
			sc=normp?sc32767:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((s16 *)cs)[0]*sc;
				ct[1]=((s16 *)cs)[1]*sc;
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_UNSIGNED_SHORT:
			sc=normp?sc65535:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((u16 *)cs)[0]*sc;
				ct[1]=((u16 *)cs)[1]*sc;
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_INT:
			sc=normp?sc2G:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((s32 *)cs)[0]*sc;
				ct[1]=((s32 *)cs)[1]*sc;
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_UNSIGNED_INT:
			sc=normp?sc4G:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((u32 *)cs)[0]*sc;
				ct[1]=((u32 *)cs)[1]*sc;
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_FLOAT:
			for(i=0; i<cnt; i++)
			{	ct[0]=((float *)cs)[0];
				ct[1]=((float *)cs)[1];
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_DOUBLE:
			for(i=0; i<cnt; i++)
			{	ct[0]=((double *)cs)[0];
				ct[1]=((double *)cs)[1];
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		case GL_HALF_FLOAT:
			for(i=0; i<cnt; i++)
			{	ct[0]=RASWGL_HalfToFloat(((u16 *)cs)[0]);
				ct[1]=RASWGL_HalfToFloat(((u16 *)cs)[1]);
				ct[2]=pad; ct[3]=pad;
				ct+=dstri; cs+=stri;	}
			break;
		default:
			break;
		}
	}else if(size==3)
	{
		switch(type)
		{
		case GL_BYTE:
//			sc=normp?sc127:1.0;
			cvt=normp?cvttab_sbn:cvttab_sb;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{
//				ct[0]=((sbyte *)cs)[0]*sc;
//				ct[1]=((sbyte *)cs)[1]*sc;
//				ct[2]=((sbyte *)cs)[2]*sc;
				ct[0]=cvt[cs[0]];
				ct[1]=cvt[cs[1]];
				ct[2]=cvt[cs[2]];
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		case GL_UNSIGNED_BYTE:
//			sc=normp?sc255:1.0;
			cvt=normp?cvttab_ubn:cvttab_ub;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{
//				ct[0]=((byte *)cs)[0]*sc;
//				ct[1]=((byte *)cs)[1]*sc;
//				ct[2]=((byte *)cs)[2]*sc;
				ct[0]=cvt[cs[0]];
				ct[1]=cvt[cs[1]];
				ct[2]=cvt[cs[2]];
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		case GL_SHORT:
			if(normp)
			{
//				sc=normp?sc32767:1.0;
				BGBRASW_NOVECTOR
				for(i=0; i<cnt; i++)
				{	ct[0]=((s16 *)cs)[0]*sc32767;
					ct[1]=((s16 *)cs)[1]*sc32767;
					ct[2]=((s16 *)cs)[2]*sc32767;
					ct[3]=pad;
					ct+=4; cs+=stri;	}
			}else
			{
				BGBRASW_NOVECTOR
				for(i=0; i<cnt; i++)
				{	ct[0]=((s16 *)cs)[0];
					ct[1]=((s16 *)cs)[1];
					ct[2]=((s16 *)cs)[2];
					ct[3]=pad;
					ct+=4; cs+=stri;	}
			}
			break;
		case GL_UNSIGNED_SHORT:
			if(normp)
			{
//				sc=normp?sc65535:1.0;
				BGBRASW_NOVECTOR
				for(i=0; i<cnt; i++)
				{	ct[0]=((u16 *)cs)[0]*sc65535;
					ct[1]=((u16 *)cs)[1]*sc65535;
					ct[2]=((u16 *)cs)[2]*sc65535;
					ct[3]=pad;
					ct+=4; cs+=stri;	}
			}else
			{
				BGBRASW_NOVECTOR
				for(i=0; i<cnt; i++)
				{	ct[0]=((u16 *)cs)[0];
					ct[1]=((u16 *)cs)[1];
					ct[2]=((u16 *)cs)[2];
					ct[3]=pad;
					ct+=4; cs+=stri;	}
			}
			break;
		case GL_INT:
			sc=normp?sc2G:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((s32 *)cs)[0]*sc;
				ct[1]=((s32 *)cs)[1]*sc;
				ct[2]=((s32 *)cs)[2]*sc;
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		case GL_UNSIGNED_INT:
			sc=normp?sc4G:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((u32 *)cs)[0]*sc;
				ct[1]=((u32 *)cs)[1]*sc;
				ct[2]=((u32 *)cs)[2]*sc;
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		case GL_FLOAT:
			for(i=0; i<cnt; i++)
			{	ct[0]=((float *)cs)[0];
				ct[1]=((float *)cs)[1];
				ct[2]=((float *)cs)[2];
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		case GL_DOUBLE:
			for(i=0; i<cnt; i++)
			{	ct[0]=((double *)cs)[0];
				ct[1]=((double *)cs)[1];
				ct[2]=((double *)cs)[2];
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		case GL_HALF_FLOAT:
			for(i=0; i<cnt; i++)
			{	ct[0]=RASWGL_HalfToFloat(((u16 *)cs)[0]);
				ct[1]=RASWGL_HalfToFloat(((u16 *)cs)[1]);
				ct[2]=RASWGL_HalfToFloat(((u16 *)cs)[2]);
				ct[3]=pad;
				ct+=4; cs+=stri;	}
			break;
		default:
			break;
		}
	}else if(size==4)
	{
		switch(type)
		{
		case GL_BYTE:
//			sc=normp?sc127:1.0;
			cvt=normp?cvttab_sbn:cvttab_sb;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{
//				ct[0]=((sbyte *)cs)[0]*sc;
//				ct[1]=((sbyte *)cs)[1]*sc;
//				ct[2]=((sbyte *)cs)[2]*sc;
//				ct[3]=((sbyte *)cs)[3]*sc;
				ct[0]=cvt[cs[0]];
				ct[1]=cvt[cs[1]];
				ct[2]=cvt[cs[2]];
				ct[3]=cvt[cs[3]];
				ct+=4; cs+=stri;	}
			break;
		case GL_UNSIGNED_BYTE:
//			sc=normp?sc255:1.0;
			cvt=normp?cvttab_ubn:cvttab_ub;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{
//				ct[0]=((byte *)cs)[0]*sc;
//				ct[1]=((byte *)cs)[1]*sc;
//				ct[2]=((byte *)cs)[2]*sc;
//				ct[3]=((byte *)cs)[3]*sc;
				ct[0]=cvt[cs[0]];
				ct[1]=cvt[cs[1]];
				ct[2]=cvt[cs[2]];
				ct[3]=cvt[cs[3]];
				ct+=4; cs+=stri;	}
			break;
		case GL_SHORT:
			sc=normp?sc32767:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((s16 *)cs)[0]*sc;
				ct[1]=((s16 *)cs)[1]*sc;
				ct[2]=((s16 *)cs)[2]*sc;
				ct[3]=((s16 *)cs)[3]*sc;
				ct+=4; cs+=stri;	}
			break;
		case GL_UNSIGNED_SHORT:
			sc=normp?sc65535:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((u16 *)cs)[0]*sc;
				ct[1]=((u16 *)cs)[1]*sc;
				ct[2]=((u16 *)cs)[2]*sc;
				ct[3]=((u16 *)cs)[3]*sc;
				ct+=4; cs+=stri;	}
			break;
		case GL_INT:
			sc=normp?sc2G:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((s32 *)cs)[0]*sc;
				ct[1]=((s32 *)cs)[1]*sc;
				ct[2]=((s32 *)cs)[2]*sc;
				ct[3]=((s32 *)cs)[3]*sc;
				ct+=4; cs+=stri;	}
			break;
		case GL_UNSIGNED_INT:
			sc=normp?sc4G:1.0;
			BGBRASW_NOVECTOR
			for(i=0; i<cnt; i++)
			{	ct[0]=((u32 *)cs)[0]*sc;
				ct[1]=((u32 *)cs)[1]*sc;
				ct[2]=((u32 *)cs)[2]*sc;
				ct[3]=((u32 *)cs)[3]*sc;
				ct+=4; cs+=stri;	}
			break;
		case GL_FLOAT:
			for(i=0; i<cnt; i++)
			{	ct[0]=((float *)cs)[0];
				ct[1]=((float *)cs)[1];
				ct[2]=((float *)cs)[2];
				ct[3]=((float *)cs)[3];
				ct+=4; cs+=stri;	}
			break;
		case GL_DOUBLE:
			for(i=0; i<cnt; i++)
			{	ct[0]=((double *)cs)[0];
				ct[1]=((double *)cs)[1];
				ct[2]=((double *)cs)[2];
				ct[3]=((double *)cs)[3];
				ct+=4; cs+=stri;	}
			break;
		case GL_HALF_FLOAT:
			for(i=0; i<cnt; i++)
			{	ct[0]=RASWGL_HalfToFloat(((u16 *)cs)[0]);
				ct[1]=RASWGL_HalfToFloat(((u16 *)cs)[1]);
				ct[2]=RASWGL_HalfToFloat(((u16 *)cs)[2]);
				ct[3]=RASWGL_HalfToFloat(((u16 *)cs)[3]);
				ct+=4; cs+=stri;	}
			break;
		default:
			break;
		}
	}
}

void RASWGL_TexCoord_ArrPtrv(
	RASWGL_Context *ctx, int size, int type, int stri, void *ptr,
	int di, int cnt)
{
	float *ct;

	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	ct=ctx->stbuf+(ctx->nvec+di)*2;
	RASWGL_GenCoord_ArrPtrv(ctx, size, type, stri, ptr, 2, ct, cnt, 0.0, 0);
}

void RASWGL_Normal_ArrPtrv(
	RASWGL_Context *ctx, int size, int type, int stri, void *ptr,
	int di, int cnt)
{
	float *ct;

	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	ct=ctx->nbuf+(ctx->nvec+di)*4;
	RASWGL_GenCoord_ArrPtrv(ctx, size, type, stri, ptr, 4, ct, cnt, 0.0, 1);
}

void RASWGL_Color_ArrPtrv(
	RASWGL_Context *ctx, int size, int type, int stri, void *ptr,
	int di, int cnt)
{
	float *ct;

	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	ct=ctx->clrbuf+(ctx->nvec+di)*4;
	RASWGL_GenCoord_ArrPtrv(ctx, size, type, stri, ptr, 4, ct, cnt, 1.0, 1);
}

void RASWGL_Vertex_ArrPtrv(
	RASWGL_Context *ctx, int size, int type, int stri, void *ptr,
	int di, int cnt)
{
	float *ct;

	if((ptr>=NULL) && (ptr<(void *)0x00100000))
		{ return; }

	ct=ctx->xyzbuf+(ctx->nvec+di)*4;
	RASWGL_GenCoord_ArrPtrv(ctx, size, type, stri, ptr, 4, ct, cnt, 1.0, 0);
}
