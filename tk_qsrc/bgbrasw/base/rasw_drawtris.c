#include <bgbrasw.h>

int bgbrasw_log2dn(int val)
{
	int i, j;
	
	i=val; j=0;
	while(i>1)
		{ i=i>>1; j++; }
	return(j);
}

int bgbrasw_log2f8(int val)
{
	int i, j;
	
	i=val<<8; j=0;
	while(i>=512)
		{ i=i>>1; j++; }
	return((j<<8)|(i&255));
}

void BGBRASW_DrawPrimitive_TriangleAuto(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	if(prim->tex && prim->tex->rgba[0])
	{
		BGBRASW_DrawPrimitive_TriangleTexInterp(ctx, prim, 0);
	}else
	{
		BGBRASW_DrawPrimitive_TriangleFlatInterp(ctx, prim, 0);
	}
}

void BGBRASW_DrawPrimitive_QuadAuto(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	if(prim->tex && prim->tex->rgba[0])
	{
		BGBRASW_DrawPrimitive_TriangleTexInterp(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleTexInterp(ctx, prim, 1);
	}else
	{
		BGBRASW_DrawPrimitive_TriangleFlatInterp(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleFlatInterp(ctx, prim, 1);
	}
}

void BGBRASW_DrawPrimitive_Triangle(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	switch(prim->mode)
	{
	case BGBRASW_PMODE_NONE:
		BGBRASW_DrawPrimitive_TriangleAuto(ctx, prim);
		break;
	case BGBRASW_PMODE_FLATBASIC:
		BGBRASW_DrawPrimitive_TriangleFlatBasic(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_FLATINTERP:
		BGBRASW_DrawPrimitive_TriangleFlatInterp(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_FLATBASICZTEST:
		BGBRASW_DrawPrimitive_TriangleFlatBasicZTest(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_FLATINTERPZTEST:
		BGBRASW_DrawPrimitive_TriangleFlatInterpZTest(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_FLATINTERPTABS:
		BGBRASW_DrawPrimitive_TriangleFlatTestBlend(ctx, prim, 0);
		break;

	case BGBRASW_PMODE_TEXBASIC:
		BGBRASW_DrawPrimitive_TriangleTexBasic(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_TEXINTERP:
		BGBRASW_DrawPrimitive_TriangleTexInterp(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_TEXBASICZTEST:
		BGBRASW_DrawPrimitive_TriangleTexBasicZTest(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_TEXINTERPZTEST:
		BGBRASW_DrawPrimitive_TriangleTexInterpZTest(ctx, prim, 0);
		break;
	case BGBRASW_PMODE_TEXINTERPTABS:
		BGBRASW_DrawPrimitive_TriangleTexTestBlend(ctx, prim, 0);
		break;
	default:
		break;
	}
}

void BGBRASW_DrawPrimitive_Quad(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	switch(prim->mode)
	{
	case BGBRASW_PMODE_NONE:
		BGBRASW_DrawPrimitive_QuadAuto(ctx, prim);
		break;
	case BGBRASW_PMODE_FLATBASIC:
		BGBRASW_DrawPrimitive_TriangleFlatBasic(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleFlatBasic(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_FLATINTERP:
		BGBRASW_DrawPrimitive_TriangleFlatInterp(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleFlatInterp(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_FLATBASICZTEST:
		BGBRASW_DrawPrimitive_TriangleFlatBasicZTest(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleFlatBasicZTest(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_FLATINTERPZTEST:
		BGBRASW_DrawPrimitive_TriangleFlatInterpZTest(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleFlatInterpZTest(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_FLATINTERPTABS:
		BGBRASW_DrawPrimitive_TriangleFlatTestBlend(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleFlatTestBlend(ctx, prim, 1);
		break;

	case BGBRASW_PMODE_TEXBASIC:
		BGBRASW_DrawPrimitive_TriangleTexBasic(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleTexBasic(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_TEXINTERP:
		BGBRASW_DrawPrimitive_TriangleTexInterp(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleTexInterp(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_TEXBASICZTEST:
		BGBRASW_DrawPrimitive_TriangleTexBasicZTest(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleTexBasicZTest(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_TEXINTERPZTEST:
		BGBRASW_DrawPrimitive_TriangleTexInterpZTest(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleTexInterpZTest(ctx, prim, 1);
		break;
	case BGBRASW_PMODE_TEXINTERPTABS:
		BGBRASW_DrawPrimitive_TriangleTexTestBlend(ctx, prim, 0);
		BGBRASW_DrawPrimitive_TriangleTexTestBlend(ctx, prim, 1);
		break;
	default:
		break;
	}
}
