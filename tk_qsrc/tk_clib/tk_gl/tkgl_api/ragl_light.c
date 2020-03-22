#include <tkgl/bgbrasw.h>

void RASWGL_Lightf(RASWGL_Context *ctx, 
		int target, int pname, float param)
{
	if((target<0) || (target>=16))
	{
		return;
	}

	switch(pname)
	{
	case GL_SPOT_EXPONENT:
		ctx->lightf[target][0]=param;
		break;
	case GL_SPOT_CUTOFF:
		ctx->lightf[target][1]=param;
		break;
	case GL_CONSTANT_ATTENUATION:
		ctx->lightf[target][2]=param;
		break;
	case GL_LINEAR_ATTENUATION:
		ctx->lightf[target][3]=param;
		break;
	case GL_QUADRATIC_ATTENUATION:
		ctx->lightf[target][4]=param;
		break;
	default:
		break;
	}
}

void RASWGL_Lighti(RASWGL_Context *ctx, 
		int target, int pname, int param)
{
//	RaGlSetErrorNopStub();
	RASWGL_Lightf(ctx, target, pname, param);
}

void RASWGL_Lightfv(RASWGL_Context *ctx, 
		int target, int pname, const float *params)
{
	if((target<0) || (target>=16))
	{
		return;
	}

	switch(pname)
	{
	case GL_AMBIENT:
		V4F_COPY(params, ctx->lightfv[target][0]);
		break;
	case GL_DIFFUSE:
		V4F_COPY(params, ctx->lightfv[target][1]);
		break;
	case GL_SPECULAR:
		V4F_COPY(params, ctx->lightfv[target][2]);
		break;
	case GL_POSITION:
		V4F_COPY(params, ctx->lightfv[target][3]);
		break;
	case GL_SPOT_CUTOFF:
		V4F_COPY(params, ctx->lightfv[target][4]);
		break;
	case GL_SPOT_DIRECTION:
		V4F_COPY(params, ctx->lightfv[target][5]);
		break;
	case GL_SPOT_EXPONENT:
		V4F_COPY(params, ctx->lightfv[target][6]);
		break;
	case GL_CONSTANT_ATTENUATION:
		V4F_COPY(params, ctx->lightfv[target][7]);
		break;
	case GL_LINEAR_ATTENUATION:
		V4F_COPY(params, ctx->lightfv[target][8]);
		break;
	case GL_QUADRATIC_ATTENUATION:
		V4F_COPY(params, ctx->lightfv[target][9]);
		break;
	default:
		break;
	}
//	RaGlSetErrorNopStub();
}

void RASWGL_Lightiv(RASWGL_Context *ctx, 
		int target, int pname, const int *params)
{
	float tv[4];
	
	tv[0]=params[0];	tv[1]=params[1];
	tv[2]=params[2];	tv[3]=params[3];
	
	RASWGL_Lightfv(ctx, target, pname, tv);
	
//	RaGlSetErrorNopStub();
}


void RASWGL_LightModelf(RASWGL_Context *ctx, 
	int pname, float param)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_LightModeli(RASWGL_Context *ctx, 
	int pname, int param)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_LightModelfv(RASWGL_Context *ctx, 
	int pname, const float *params)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_LightModeliv(RASWGL_Context *ctx, 
	int pname, const int *params)
{
//	RaGlSetErrorNopStub();
}


void RASWGL_Materialf(RASWGL_Context *ctx, 
		int target, int pname, float param)
{
	switch(pname)
	{
	case GL_SHININESS:
		break;
	default:
		break;
	}
//	RaGlSetErrorNopStub();
}

void RASWGL_Materiali(RASWGL_Context *ctx, 
		int target, int pname, int param)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_Materialfv(RASWGL_Context *ctx, 
		int target, int pname, const float *params)
{
	switch(pname)
	{
	case GL_AMBIENT:
		V4F_COPY(params, ctx->materialfv[0]);
		break;
	case GL_DIFFUSE:
		V4F_COPY(params, ctx->materialfv[1]);
		break;
	case GL_SPECULAR:
		V4F_COPY(params, ctx->materialfv[2]);
		break;
	case GL_EMISSION:
		V4F_COPY(params, ctx->materialfv[3]);
		break;
	case GL_SHININESS:
		V4F_COPY(params, ctx->materialfv[4]);
		break;
	case GL_AMBIENT_AND_DIFFUSE:
		V4F_COPY(params, ctx->materialfv[5]);
		break;
	case GL_COLOR_INDEXES:
		V4F_COPY(params, ctx->materialfv[6]);
		break;
	default:
		break;
	}
//	RaGlSetErrorNopStub();
}

void RASWGL_Materialiv(RASWGL_Context *ctx, 
		int target, int pname, const int *params)
{
//	RaGlSetErrorNopStub();
}

void RASWGL_CalcLightVertexColor(RASWGL_Context *ctx, 
	float *xyzw, float *srgb, float *drgb)
{
	float tv[16];
	float *pos;
	float f, g, h;
	int i, j, k, l;
	
	V4F_ZERO(tv);
	
	for(i=0; i<16; i++)
	{
		if(!(ctx->lights_enabled&(1<<i)))
			continue;
		pos=ctx->lightfv[i][3];
		f=	xyzw[0]*pos[0]+xyzw[1]*pos[1]+
			xyzw[2]*pos[2]+xyzw[3]*pos[3];
			
		V4F_ADDSCALE(tv, srgb, f, tv);
	}
	
	V4F_COPY(tv, drgb);
}