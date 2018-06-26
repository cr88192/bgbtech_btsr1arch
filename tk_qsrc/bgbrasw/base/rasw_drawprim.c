#include <bgbrasw.h>

void BGBRASW_DrawPrimitive(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	switch(prim->type)
	{
	case BGBRASW_PTYPE_LINE:
		BGBRASW_DrawPrimitive_Line(ctx, prim);
		break;
	case BGBRASW_PTYPE_BOX:
		BGBRASW_DrawPrimitive_Box(ctx, prim);
		break;
	case BGBRASW_PTYPE_TRIANGLE:
		BGBRASW_DrawPrimitive_Triangle(ctx, prim);
		break;
	case BGBRASW_PTYPE_QUAD:
		BGBRASW_DrawPrimitive_Quad(ctx, prim);
		break;
	default:
		break;
	}
}

BGBRASW_API void BGBRASW_DrawPrimitiveList(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *list)
{
	BGBRASW_Primitive *cur;
	
	cur=list;
	while(cur)
	{
		BGBRASW_DrawPrimitive(ctx, cur);
		cur=cur->next;
	}
}
