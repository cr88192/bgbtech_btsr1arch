//AHSRC:base/rasw_api.c
//AHSRC:base/rasw_context.c
BGBRASW_API void *bgbrasw_malloc(size_t sz);
BGBRASW_API void *bgbrasw_tmalloc(char *ty, size_t sz);
BGBRASW_API void bgbrasw_free(void *ptr);
BGBRASW_API void *bgbrasw_realloc(void *ptr, size_t sz);
BGBRASW_API int BGBRASW_InitCore();
BGBRASW_API BGBRASW_Context *BGBRASW_AllocContext();
BGBRASW_API void BGBRASW_SetupRenderBasic(BGBRASW_Context *ctx, int xs, int ys, int flags);
BGBRASW_API void BGBRASW_ClearScreenColor(BGBRASW_Context *ctx, bgbrasw_pixel clr);
BGBRASW_API void BGBRASW_ClearScreenColor(BGBRASW_Context *ctx, bgbrasw_pixel clr);
BGBRASW_API void BGBRASW_ClearDepthStencil(BGBRASW_Context *ctx, bgbrasw_zbuf clr);
BGBRASW_API void BGBRASW_ClearDepthOnly(BGBRASW_Context *ctx, bgbrasw_zbuf clr);
BGBRASW_API void BGBRASW_ClearStencilOnly(BGBRASW_Context *ctx, bgbrasw_zbuf clr);
BGBRASW_API void BGBRASW_FreeContext(BGBRASW_Context *ctx);
BGBRASW_API BGBRASW_Primitive *BGBRASW_AllocPrimitive(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_FreePrimitive(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
BGBRASW_API void BGBRASW_FreePrimitiveList(BGBRASW_Context *ctx, BGBRASW_Primitive *plst);
BGBRASW_API BGBRASW_Texture *BGBRASW_AllocTexture(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_FreeTexture(BGBRASW_Context *ctx, BGBRASW_Texture *tex);
BGBRASW_API BGBRASW_TestBlendData *BGBRASW_AllocTestBlendData(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_FreeTestBlendData(BGBRASW_Context *ctx, BGBRASW_TestBlendData *tabs);
BGBRASW_API void BGBRASW_SetTextureSize(BGBRASW_Context *ctx, BGBRASW_Texture *tex, int xs, int ys);
BGBRASW_API void BGBRASW_SetTextureAllocMip(BGBRASW_Context *ctx, BGBRASW_Texture *tex);
int Tex_HalfSampleB(byte *src, byte *dst, int w, int h);
BGBRASW_API void BGBRASW_SetTextureBuildMip(BGBRASW_Context *ctx, BGBRASW_Texture *tex);
//AHSRC:base/rasw_drawbox.c
void BGBRASW_DrawPrimitive_BoxFlatBasic(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_BoxFlatInterp(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_BoxTexBasic(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_BoxTexInterp(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_BoxTexBasicZTest(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_BoxTexInterpZTest(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_BoxTestBlend(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_Box(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
//AHSRC:base/rasw_drawprim.c
void BGBRASW_DrawPrimitive(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
BGBRASW_API void BGBRASW_DrawPrimitiveList(BGBRASW_Context *ctx, BGBRASW_Primitive *list);
//AHSRC:base/rasw_drawline.c
void BGBRASW_DrawPrimitive_Line(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
//AHSRC:base/rasw_drawspan.c
void BGBRASW_DrawSpanFlatBasic(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, int npix, bgbrasw_pixel clr);
void BGBRASW_DrawSpanFlatInterp(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, int npix, bgbrasw_pixel clr0, bgbrasw_pixel clr1);
void BGBRASW_DrawSpanFlatBasicZTest(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanFlatInterpZTest(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanTextureBasic(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t);
void BGBRASW_DrawSpanTextureInterp(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t, bgbrasw_pixel clr0, bgbrasw_pixel clr1);
void BGBRASW_DrawSpanTextureBasicZTest(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanTextureInterpZTest(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t, bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanFlatBasicTestBlend(BGBRASW_TestBlendData *testData, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanFlatInterpTestBlend(BGBRASW_TestBlendData *testData, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanTextureBasicTestBlend(BGBRASW_TestBlendData *testData, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
void BGBRASW_DrawSpanTextureInterpTestBlend(BGBRASW_TestBlendData *testData, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t, bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
bgbrasw_pixel bgbrasw_interp_linear(bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_pixel clr2, bgbrasw_pixel clr3, int fx, int fy);
void BGBRASW_DrawSpanTextureLinearInterpTestBlend(BGBRASW_TestBlendData *testData, bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, bgbrasw_pixel *tex, int txs, int tys, int st0s, int st0t, int st1s, int st1t, bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1);
//AHSRC:base/rasw_drawtris.c
int bgbrasw_log2dn(int val);
int bgbrasw_log2f8(int val);
void BGBRASW_DrawPrimitive_TriangleAuto(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_QuadAuto(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_Triangle(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
void BGBRASW_DrawPrimitive_Quad(BGBRASW_Context *ctx, BGBRASW_Primitive *prim);
//AHSRC:base/rasw_drawtris_flat.c
void BGBRASW_DrawPrimitive_TriangleFlatBasic(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleFlatInterp(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleFlatBasicZTest(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleFlatInterpZTest(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleFlatTestBlend(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
//AHSRC:base/rasw_drawtris_tex.c
void BGBRASW_DrawPrimitive_TriangleTexBasic(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleTexInterp(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleTexBasicZTest(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleTexInterpZTest(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
void BGBRASW_DrawPrimitive_TriangleTexTestBlend(BGBRASW_Context *ctx, BGBRASW_Primitive *prim, int flag);
//AHSRC:base/rasw_targa.c
BGBRASW_API byte *BGBRASW_Img_ReadTGA(FILE *fd, int *w, int *h);
BGBRASW_API int BGBRASW_Img_StoreTGA(FILE *fd, byte *buf, int w, int h);
BGBRASW_API int BGBRASW_Img_StoreTGAPixel(FILE *fd, bgbrasw_pixel *buf, int w, int h);
BGBRASW_API int BGBRASW_Img_SaveTGAPixel(char *name, bgbrasw_pixel *buf, int w, int h);
BGBRASW_API bgbrasw_pixel *BGBRASW_Img_ReadTGAPixel(FILE *fd, int *w, int *h);
BGBRASW_API bgbrasw_pixel *BGBRASW_Img_LoadTGAPixel(char *name, int *w, int *h);
//AHSRC:base/rasw_testblend.c
int BGBRASW_TestAndBlend_AlphaTest_Never(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_Always(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_Equal(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_NotEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_Less(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_Greater(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_LessEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaTest_GreaterEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_Never(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_Always(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_Less(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_LessEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_Greater(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_GreaterEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_Equal(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthTest_NotEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_Never(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_Always(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_Less(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_LessEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_Greater(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_GreaterEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_Equal(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_StencilTest_NotEqual(BGBRASW_TestBlendData *data, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_Keep(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_Zero(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_Replace(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_Incr(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_Decr(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_IncrWrap(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_DecrWrap(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_TestAndBlend_StencilOp_Invert(BGBRASW_TestBlendData *tabs, bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz);
void BGBRASW_BlendFunc_Zero(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_One(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_SrcColor(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_OneMinusSrcColor(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_DstColor(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_OneMinusDstColor(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_SrcAlpha(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_OneMinusSrcAlpha(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_DstAlpha(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_BlendFunc_OneMinusDstAlpha(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rfr, int *rfg, int *rfb, int *rfa);
void BGBRASW_Blend_Generic(BGBRASW_TestBlendData *data, int sr, int sg, int sb, int sa, int dr, int dg, int db, int da, int *rr, int *rg, int *rb, int *ra);
bgbrasw_pixel BGBRASW_DoBlend_None(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_Generic(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_Zero_SrcColor(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_Zero_OneMinusSrcColor(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_One_One(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_DstColor_Zero(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_DstColor_One(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_DstColor_SrcColor(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_DstColor_SrcAlpha(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_DstColor_OneMinusSrcAlpha(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_DstColor_OneMinusDstAlpha(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
bgbrasw_pixel BGBRASW_DoBlend_SrcAlpha_OneMinusSrcAlpha(BGBRASW_TestBlendData *data, bgbrasw_pixel src, bgbrasw_pixel dst);
int BGBRASW_TestAndBlend_Generic(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthBlend(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthBlend_LessEqual(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthBlend_LessEqualNoMask(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthBlend_LessEqual_NoClrMask_NoDepthWrite(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaDepth(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_Depth(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_DepthStencil(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_Stencil(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_AlphaBlend(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_Blend(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_Alpha(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
int BGBRASW_TestAndBlend_None(BGBRASW_TestBlendData *tabs, bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz, bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz);
BGBRASW_StencilOpFunc_ft BGBRASW_TestAndBlend_GetStencilOpFunc(BGBRASW_Context *ctx, BGBRASW_TestBlendData *tabs, int op);
int BGBRASW_SetupTestBlend(BGBRASW_Context *ctx,BGBRASW_TestBlendData *tabs);
void BGBRASW_CopyTestBlend(BGBRASW_Context *ctx,BGBRASW_TestBlendData *srctabs, BGBRASW_TestBlendData *dsttabs);
//AHSRC:base/rasw_texture.c
BGBRASW_Texture *BGBRASW_LookupTexture(BGBRASW_Context *ctx, int texid);
BGBRASW_Texture *BGBRASW_GetTexture(BGBRASW_Context *ctx, int texid);
int BGBRASW_DeleteTexture(BGBRASW_Context *ctx, int texid);
int BGBRASW_AllocTextureID(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_SetTexturePixelsSize(BGBRASW_Context *ctx, BGBRASW_Texture *tex, bgbrasw_pixel *pix, int xs, int ys, int lvl);
BGBRASW_API void BGBRASW_SetTextureSizeMip(BGBRASW_Context *ctx, BGBRASW_Texture *tex, int xs, int ys, int lvl);
//AHSRC:base/rasw_thread.c
BGBRASW_API BGBRASW_Context *BGBRASW_AllocChildContext(BGBRASW_Context *parent);
BGBRASW_API BGBRASW_Context *BGBRASW_CreateChildThreadContext(BGBRASW_Context *parent);
BGBRASW_API void BGBRASW_LockContext(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_UnlockContext(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_Finish(BGBRASW_Context *ctx);
BGBRASW_API void BGBRASW_Flush(BGBRASW_Context *ctx);
//AHSRC:raswgl/ragl_begin.c
BGBRASW_API RASWGL_Context *RASWGL_CreateContext(int xs, int ys, int flags);
void RASWGL_Finish(RASWGL_Context *ctx);
void RASWGL_Flush(RASWGL_Context *ctx);
void RASWGL_GetFloatv(RASWGL_Context *ctx,int pname, float *params);
void RASWGL_GetIntegerv(RASWGL_Context *ctx,int pname, int *params);
void RASWGL_MatrixMode(RASWGL_Context *ctx, int mode);
void RASWGL_LoadIdentityMatrix(RASWGL_Context *ctx);
void RASWGL_PushMatrix(RASWGL_Context *ctx);
void RASWGL_PopMatrix(RASWGL_Context *ctx);
void RASWGL_LoadMatrix(RASWGL_Context *ctx, float *mat);
void RASWGL_MultMatrix(RASWGL_Context *ctx, float *mat);
void RASWGL_MultMatrixT(RASWGL_Context *ctx, float *mat);
void RASWGL_PushMultMatrix(RASWGL_Context *ctx, float *mat);
void RASWGL_Translatef(RASWGL_Context *ctx, float x, float y, float z);
void RASWGL_Scalef(RASWGL_Context *ctx, float x, float y, float z);
void RASWGL_Rotatef(RASWGL_Context *ctx, float th, float x, float y, float z);
void RASWGL_SetupOrtho(RASWGL_Context *ctx, float left, float right, float bottom, float top,  float nearval, float farval);
void RASWGL_SetupFrustum(RASWGL_Context *ctx, float left, float right, float bottom, float top,  float nearval, float farval);
void RASWGL_TransformPoint(RASWGL_Context *ctx, float *src, float *dst);
void RASWGL_TransformPointLocal(RASWGL_Context *ctx, float *src, float *dst);
void RASWGL_TransformVector(RASWGL_Context *ctx, float *src, float *dst);
void RASWGL_TransformVectorLocal(RASWGL_Context *ctx, float *src, float *dst);
void RASWGL_Init(RASWGL_Context *ctx);
void RASWGL_FlushState(RASWGL_Context *ctx);
void RASWGL_FlushTexture(RASWGL_Context *ctx);
void RASWGL_SetupTexture(RASWGL_Context *ctx);
void RASWGL_BindTexture(RASWGL_Context *ctx, int target, int num);
void RASWGL_DeleteTextures(RASWGL_Context *ctx, int cnt, int *tex);
void RASWGL_GenTextures(RASWGL_Context *ctx, int cnt, int *tex);
int RASWGL_GetCurrentTexture(RASWGL_Context *ctx);
int RASWGL_GetCurrentTextureImage(RASWGL_Context *ctx);
int RASWGL_GetCurrentTextureFlags(RASWGL_Context *ctx);
int RASWGL_GetTextureFlags(RASWGL_Context *ctx, int num);
void RASWGL_BlendFunc(RASWGL_Context *ctx, int src, int dst);
void RASWGL_AlphaFunc(RASWGL_Context *ctx, int func, float ref);
void RASWGL_StencilFunc(RASWGL_Context *ctx, int func, int ref, int mask);
void RASWGL_StencilMask(RASWGL_Context *ctx, int mask);
void RASWGL_StencilOp(RASWGL_Context *ctx,int sfail, int dpfail, int dppass);
void RASWGL_ClipPlane(RASWGL_Context *ctx, int pnum, double *plane);
void RASWGL_Clear(RASWGL_Context *ctx, int mask);
void RASWGL_Enable(RASWGL_Context *ctx, int cap);
void RASWGL_Disable(RASWGL_Context *ctx, int cap);
int RASWGL_IsEnabled(RASWGL_Context *ctx, int cap);
void RASWGL_ClearTexture(RASWGL_Context *ctx);
void RASWGL_BeginPoly(RASWGL_Context *ctx);
void RASWGL_EndPoly(RASWGL_Context *ctx);
void RASWGL_Begin(RASWGL_Context *ctx, int prim);
void RASWGL_End(RASWGL_Context *ctx);
void RASWGL_EndPrim_DrawArrays(RASWGL_Context *ctx, int prim, float *xyz, float *st, float *norm, float *rgba, int nxyz);
void RASWGL_GetWindowOrigin(RASWGL_Context *ctx, int *rxs, int *rys);
void RASWGL_GetWindowSize(RASWGL_Context *ctx, int *rxs, int *rys);
void RASWGL_GetWindowMaxSize(RASWGL_Context *ctx, int *rxs, int *rys);
void RASWGL_GetWindowTexSize(RASWGL_Context *ctx, int *rxs, int *rys);
void RASWGL_GetWindowMaxTexSize(RASWGL_Context *ctx, int *rxs, int *rys);
void RASWGL_EndPrim_DrawTexture(RASWGL_Context *ctx, int prim);
void RASWGL_EndPrim(RASWGL_Context *ctx, int prim);
void RASWGL_VertexDownProject(RASWGL_Context *ctx);
void RASWGL_VertexCheckExpand(RASWGL_Context *ctx);
void RASWGL_Vertex3fv(RASWGL_Context *ctx, float *v);
void RASWGL_Vertex4fv(RASWGL_Context *ctx, float *v);
void RASWGL_TexCoord2fv(RASWGL_Context *ctx, float *v);
void RASWGL_Normal3fv(RASWGL_Context *ctx, float *v);
void RASWGL_Normal4fv(RASWGL_Context *ctx, float *v);
void RASWGL_Normal3sbv(RASWGL_Context *ctx, sbyte *v);
void RASWGL_Color3fv(RASWGL_Context *ctx, float *v);
void RASWGL_Color4fv(RASWGL_Context *ctx, float *v);
void RASWGL_Color4ubv(RASWGL_Context *ctx, byte *v);
void RASWGL_Color3f(RASWGL_Context *ctx, float r, float g, float b);
void RASWGL_Color4f(RASWGL_Context *ctx, float r, float g, float b, float a);
void RASWGL_TexCoord2f(RASWGL_Context *ctx, float s, float t);
void RASWGL_Vertex2f(RASWGL_Context *ctx, float x, float y);
void RASWGL_Vertex3f(RASWGL_Context *ctx, float x, float y, float z);
void RASWGL_Normal3f(RASWGL_Context *ctx, float x, float y, float z);
void RASWGL_Normal4f(RASWGL_Context *ctx, float x, float y, float z, float w);
void RASWGL_Vertex4f(RASWGL_Context *ctx, float x, float y, float z, float w);
void RASWGL_TexCoord_Ptrv(RASWGL_Context *ctx, int size, int type, void *ptr);
void RASWGL_Normal_Ptrv(RASWGL_Context *ctx, int size, int type, void *ptr);
void RASWGL_Color_Ptrv(RASWGL_Context *ctx, int size, int type, void *ptr);
void RASWGL_Vertex_Ptrv(RASWGL_Context *ctx, int size, int type, void *ptr);
void RASWGL_DrawArraysSimpleFlat(RASWGL_Context *ctx, int prim, int base, int nxyz, int xyzsize, int xyztype, int xyzstep, void *xyz);
void RASWGL_DrawArraysNormalFlat(int prim, int base, int nxyz, int xyzsize, int xyztype, int xyzstep, void *xyz,  int normsize, int normtype, int normstep, void *norm);
void RASWGL_DrawArraysNormalFlatRGB(int prim, int base, int nxyz, int xyzsize, int xyztype, int xyzstep, void *xyz,  int normsize, int normtype, int normstep, void *norm, int rgbsize, int rgbtype, int rgbstep, void *rgb);
void RASWGL_DrawArraysNormalTex(int prim, int base, int nxyz, int xyzsize, int xyztype, int xyzstep, void *xyz,  int stsize, int sttype, int ststep, void *st,  int normsize, int normtype, int normstep, void *norm);
void RASWGL_DrawArraysNormalTexRGB(int prim, int base, int nxyz, int xyzsize, int xyztype, int xyzstep, void *xyz,  int stsize, int sttype, int ststep, void *st,  int normsize, int normtype, int normstep, void *norm, int rgbsize, int rgbtype, int rgbstep, void *rgb);
void RASWGL_DrawElementsSimpleFlat(int prim, int nelem, int elemtype, void *elems, int xyzsize, int xyztype, int xyzstep, void *xyz);
void RASWGL_DrawElementsNormalTexRGB(int prim, int nelem, int elemtype, void *elems, int xyzsize, int xyztype, int xyzstep, void *xyz,  int stsize, int sttype, int ststep, void *st,  int normsize, int normtype, int normstep, void *norm, int rgbsize, int rgbtype, int rgbstep, void *rgb);
//AHSRC:raswgl/ragl_clip.c
int RASWGL_TransformClipTriangles(RASWGL_Context *ctx, int nxyz, int nclip, int clipsz, float *clip, int xyzsz,		int stsz, int nvsz,		int rgbsz, float *ixyz,	float *ist, float *inv,		float *irgb, float *oxyz,	float *ost, float *onv,		float *orgb);
int RASWGL_TransformClipQuads(RASWGL_Context *ctx, int nxyz, int nclip, int clipsz, float *clip, int xyzsz,		int stsz, int nvsz,		int rgbsz, float *ixyz,	float *ist, float *inv,		float *irgb, float *oxyz,	float *ost, float *onv,		float *orgb);
//AHSRC:raswgl/ragl_gldrv.c
BGBRASW_API RASWGL_Context *RaGlGetContext();
BGBRASW_API void RaGlSetContext(RASWGL_Context *ctx);
BGBRASW_API RASWGL_Context *RaGlCreateContext(int xs, int ys, int flags);
void RaGlSetErrorNopStub(void);
//AHSRC:raswgl/ragl_glw.c
//AHSRC:raswgl/ragl_light.c
void RASWGL_Lightf(RASWGL_Context *ctx, int target, int pname, float param);
void RASWGL_Lighti(RASWGL_Context *ctx, int target, int pname, int param);
void RASWGL_Lightfv(RASWGL_Context *ctx, int target, int pname, const float *params);
void RASWGL_Lightiv(RASWGL_Context *ctx, int target, int pname, const int *params);
void RASWGL_LightModelf(RASWGL_Context *ctx, int pname, float param);
void RASWGL_LightModeli(RASWGL_Context *ctx, int pname, int param);
void RASWGL_LightModelfv(RASWGL_Context *ctx, int pname, const float *params);
void RASWGL_LightModeliv(RASWGL_Context *ctx, int pname, const int *params);
void RASWGL_Materialf(RASWGL_Context *ctx, int target, int pname, float param);
void RASWGL_Materiali(RASWGL_Context *ctx, int target, int pname, int param);
void RASWGL_Materialfv(RASWGL_Context *ctx, int target, int pname, const float *params);
void RASWGL_Materialiv(RASWGL_Context *ctx, int target, int pname, const int *params);
void RASWGL_CalcLightVertexColor(RASWGL_Context *ctx, float *xyzw, float *srgb, float *drgb);
//AHSRC:raswgl/ragl_raswprim.c
void RASWGL_VertexToViewportPixels(RASWGL_Context *ctx, float *sxyz, int *dxy, bgbrasw_zbuf *dz);
void RASWGL_CoordsToViewportTexels(RASWGL_Context *ctx, BGBRASW_Texture *tex, float *suv, int *duv);
void RASWGL_ColorsToViewportColors(RASWGL_Context *ctx, float *srgb, bgbrasw_pixel *drgb);
void RASWGL_ColorsToViewportColors2(RASWGL_Context *ctx, float *xyzw, float *srgb, bgbrasw_pixel *drgb);
int RASWGL_ClasifyPrimitive_FlatColorP(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
int RASWGL_ClasifyPrimitive_OpaqueP(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
int RASWGL_ClasifyPrimitive_BasicOpaqueP(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
int RASWGL_ClasifyPrimitive_TexValidP(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
int RASWGL_ClasifyPrimitive_NeedsTabsP(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
void RASWGL_ClasifyPrimitive(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
int raswgl_dist2d2(int *xy0, int *xy1);
void raswgl_avgxy2d(int *xy0, int *xy1, int *st0, int *st1, bgbrasw_pixel clra,	bgbrasw_pixel clrb, bgbrasw_zbuf z0, bgbrasw_zbuf z1, int w0, int w1, int *dxy, int *dst, bgbrasw_pixel *dclr, bgbrasw_zbuf *dz, int *dw);
int raswgl_copyxy2d(int *sxy, int *sst, bgbrasw_pixel  sclr, bgbrasw_zbuf  sz, int sw, int *dxy, int *dst, bgbrasw_pixel *dclr, bgbrasw_zbuf *dz, int *dw);
BGBRASW_Primitive *RASWGL_SubdivideInsertTriangle(RASWGL_Context *ctx, BGBRASW_Primitive *plst, BGBRASW_Primitive *ptmp);
BGBRASW_Primitive *RASWGL_SubdivideInsertQuad(RASWGL_Context *ctx, BGBRASW_Primitive *plst, BGBRASW_Primitive *ptmp);
BGBRASW_Primitive *RASWGL_BuildPrimitiveListArrays(RASWGL_Context *ctx, int prim, int base, int nxyz, int xyzsize, float *xyz,  int stsize, float *st,  int normsize, float *norm, int rgbsize, float *rgb);
BGBRASW_Primitive *RASWGL_AllocPrimitiveForPoint(RASWGL_Context *ctx, float *org, float *min, float *max);
void RASWGL_SubmitPrimitive(RASWGL_Context *ctx, BGBRASW_Primitive *prim);
void RASWGL_BuildInsertPrimitiveArrays(RASWGL_Context *ctx, int prim, int base, int nxyz, int xyzsize, float *xyz,  int stsize, float *st,  int normsize, float *norm, int rgbsize, float *rgb);
//AHSRC:raswgl/ragl_teximage.c
char *RASWGL_GetString(RASWGL_Context *ctx, int name);
void RASWGL_TexEnvf(RASWGL_Context *ctx, int target, int pname, float param);
void RASWGL_TexEnvi(RASWGL_Context *ctx, int target, int pname, int param);
void RASWGL_TexEnvfv(RASWGL_Context *ctx, int target, int pname, const float *params);
void RASWGL_TexEnviv(RASWGL_Context *ctx, int target, int pname, const int *params);
void RASWGL_GetTexEnvfv(RASWGL_Context *ctx, int target, int pname, float *params);
void RASWGL_GetTexEnviv(RASWGL_Context *ctx, int target, int pname, int *params);
void RASWGL_TexParameterf(RASWGL_Context *ctx, int target, int pname, float param);
void RASWGL_TexParameteri(RASWGL_Context *ctx, int target, int pname, int param);
void RASWGL_TexParameterfv(RASWGL_Context *ctx, int target, int pname, const float *params);
void RASWGL_TexParameteriv(RASWGL_Context *ctx, int target, int pname, const int *params);
void RASWGL_GetTexParameterfv(RASWGL_Context *ctx, int target, int pname, float *params);
void RASWGL_GetTexParameteriv(RASWGL_Context *ctx, int target, int pname, int *params);
void RASWGL_GetTexLevelParameterfv(RASWGL_Context *ctx, int target, int level, int pname, float *params);
void RASWGL_GetTexLevelParameteriv(RASWGL_Context *ctx, int target, int level, int pname, int *params);
void RASWGL_TexImage1D(RASWGL_Context *ctx, int target, int level, int internalFormat, int width, int border, int format, int type, const void *pixels);
int RASWGL_TexImage_ConvPixelsFromSrc(int width, int height, int format, int type, const void *pixels, bgbrasw_pixel *opix);
int RASWGL_TexImage_ConvSubImagePixelsFromSrc(int xoffs, int yoffs, int width, int height, int format, int type, const void *pixels, bgbrasw_pixel *opix, int tgtxs, int tgtys);
void RASWGL_TexImage2D(RASWGL_Context *ctx, int target, int level, int internalFormat, int width, int height, int border, int format, int type, const void *pixels);
void RASWGL_GetTexImage(RASWGL_Context *ctx, int target, int level, int format, int type, void *pixels);
void RASWGL_TexSubImage1D(RASWGL_Context *ctx, int target, int level, int xoffset, int width, int format, int type, const void *pixels);
void RASWGL_TexSubImage2D(RASWGL_Context *ctx, int target, int level, int xoffset, int yoffset, int width, int height, int format, int type, const void *pixels);
void RASWGL_CopyTexImage1D(RASWGL_Context *ctx, int target, int level, int internalformat, int x, int y, int width, int border);
void RASWGL_CopyTexImage2D(RASWGL_Context *ctx, int target, int level, int internalformat, int x, int y, int width, int height, int border);
void RASWGL_CopyTexSubImage1D(RASWGL_Context *ctx, int target, int level, int xoffset, int x, int y, int width);
void RASWGL_CopyTexSubImage2D(RASWGL_Context *ctx, int target, int level, int xoffset, int yoffset, int x, int y, int width, int height);
void RASWGL_CompressedTexImage2D(RASWGL_Context *ctx, int target, int level, int internalFormat, int width, int height, int border, int imgSize, const void *pixels);
void RASWGL_ReadPixels(RASWGL_Context *ctx,  int x, int y, int width, int height, int format, int type, byte *pixels);
//AHSRC:raswgl/ragl_transform.c
void RASWGL_CheckExpandTransform0Vertices(RASWGL_Context *ctx, int nxyz);
void RASWGL_CheckExpandTransform1Vertices(RASWGL_Context *ctx, int nxyz);
void RASWGL_CheckExpandTransformNVertices(RASWGL_Context *ctx, int nxyz);
void RASWGL_TransformVerticesModelviewXYZ(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
void RASWGL_TransformVerticesProjectionXYZ(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
void RASWGL_TransformNormalsModelviewXYZ(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
void RASWGL_TransformNormalsProjectionXYZ(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
void RASWGL_TransformVerticesModelviewProjectionXYZ(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
void RASWGL_TransformNormalsModelviewProjectionXYZ(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
void RASWGL_TransformVerticesDivide_XYZ_W(RASWGL_Context *ctx, int nxyz, int ixyzsz, float *ixyz, int oxyzsz, float *oxyz);
float raswgl_perspprojxyzw(float *xyz0, float *dxyz);
float raswgl_perspdist2xyzw(float *xyz0, float *xyz1);
float raswgl_avgdist2xyzw(float *xyz0, float *xyz1, float *oxyz, float *st0, float *st1, float *ost, float *nv0, float *nv1, float *onv, float *rgb0, float *rgb1, float *orgb);
int RASWGL_TransformSubdivideInsertTriangle(RASWGL_Context *ctx, int rc, float *xyz0,	float *st0,		float *nv0,		float *rgb0, float *xyz1,	float *st1,		float *nv1,		float *rgb1, float *xyz2,	float *st2,		float *nv2,		float *rgb2, float *oxyz,	float *ost,		float *onv,		float *orgb, float *exyz,	float *est,		float *env,		float *ergb);
int RASWGL_TransformSubdivideInsertQuad(RASWGL_Context *ctx, int rc, float *xyz0,	float *st0,		float *nv0,		float *rgb0, float *xyz1,	float *st1,		float *nv1,		float *rgb1, float *xyz2,	float *st2,		float *nv2,		float *rgb2, float *xyz3,	float *st3,		float *nv3,		float *rgb3, float *oxyz,	float *ost,		float *onv,		float *orgb, float *exyz,	float *est,		float *env,		float *ergb);
int RASWGL_TransformSubdivideCheckCull(RASWGL_Context *ctx,float *xyz0, float *xyz1, float *xyz2);
int RASWGL_TransformSubdivideTriangles(RASWGL_Context *ctx, int ntris, int mtris, float *ixyz,	float *ist, float *inv,		float *irgb, float *oxyz,	float *ost, float *onv,		float *orgb);
int RASWGL_TransformSubdivideQuads(RASWGL_Context *ctx, int nquad, int mquad, float *ixyz,	float *ist, float *inv,		float *irgb, float *oxyz,	float *ost, float *onv,		float *orgb);
void RASWGL_TransformDrawTriangles(RASWGL_Context *ctx, int base, int nxyz, int xyzsize, float *xyz, int stsize, float *st, int normsize, float *norm, int rgbsize, float *rgb);
void RASWGL_TransformDrawQuads(RASWGL_Context *ctx, int base, int nxyz, int xyzsize, float *xyz, int stsize, float *st, int normsize, float *norm, int rgbsize, float *rgb);
void RASWGL_TransformDrawPolygon(RASWGL_Context *ctx, int base, int nxyz, int xyzsz, float *xyz, int stsz, float *st, int nvsz, float *nv, int rgbsz, float *rgb);
void RASWGL_TransformDrawTriangleStrip(RASWGL_Context *ctx, int base, int nxyz, int xyzsz, float *xyz, int stsz, float *st, int nvsz, float *nv, int rgbsz, float *rgb);
void RASWGL_TransformDrawQuadStrip(RASWGL_Context *ctx, int base, int nxyz, int xyzsz, float *xyz, int stsz, float *st, int nvsz, float *nv, int rgbsz, float *rgb);
void RASWGL_SetupTabs(RASWGL_Context *ctx);
void RASWGL_FinishTabs(RASWGL_Context *ctx);
void RASWGL_TransformDrawArrays(RASWGL_Context *ctx, int prim, int base, int nxyz, int xyzsize, float *xyz, int stsize, float *st, int normsize, float *norm, int rgbsize, float *rgb);
void RASWGL_DrawBaseArrays(RASWGL_Context *ctx, int prim, int base, int nxyz, int xyzsize, int xyztype, int xyzstep, void *xyz,  int stsize, int sttype, int ststep, void *st,  int normsize, int normtype, int normstep, void *norm, int rgbsize, int rgbtype, int rgbstep, void *rgb);
//AHSRC:raswgl/ragl_vertexarray.c
int raswgl_barf_arrayStepSize(int vsize, int vtype, int ivstep);
void RASWGL_EnableClientState(RASWGL_Context *ctx, int cap);
void RASWGL_DisableClientState(RASWGL_Context *ctx, int cap);
void RASWGL_VertexPointer(RASWGL_Context *ctx, int size, int type, int stride, const void *ptr);
void RASWGL_TexCoordPointer(RASWGL_Context *ctx, int size, int type, int stride, const void *ptr);
void RASWGL_NormalPointer(RASWGL_Context *ctx, int type, int stride, const void *ptr);
void RASWGL_ColorPointer(RASWGL_Context *ctx, int size, int type, int stride, const void *ptr);
void RASWGL_IndexPointer(RASWGL_Context *ctx, int type, int stride, const void *ptr);
void RASWGL_EdgeFlagPointer(RASWGL_Context *ctx, int stride, const void *ptr);
void RASWGL_GetPointerv(RASWGL_Context *ctx, int pname, void **params);
void RASWGL_ArrayElement(RASWGL_Context *ctx, int i);
void RASWGL_DrawArrays(RASWGL_Context *ctx, int mode, int first, int count);
void RASWGL_DrawElements(RASWGL_Context *ctx, int mode, int count, int type, const void *indices);
void RASWGL_InterleavedArrays(RASWGL_Context *ctx, int format, int stride, const void *pointer);
//AHSRC:vecmath/math_clip.c
//AHSRC:vecmath/math_hullf.c
//AHSRC:vecmath/math_m3.c
//AHSRC:vecmath/math_m4.c
void Mat4F_SetupFrustum(float *mat,float left, float right, float bottom, float top, float nearval, float farval);
void Mat4F_SetupOrtho(float *mat,float left, float right, float bottom, float top, float nearval, float farval);
//AHSRC:vecmath/math_mf.c
//AHSRC:vecmath/math_n.c
//AHSRC:vecmath/math_plane.c
//AHSRC:vecmath/math_quat.c
//AHSRC:vecmath/math_solid.c
//AHSRC:vecmath/math_v3f.c
//AHSRC:vecmath/math_vecnf.c
void *VecNF_Ralloc(int sz);
//AHSRC:vecmath/math_vnf.c
//AHSRC:vecmath/mathlib_d.c
//AHSRC:vecmath/mathlib_f.c
//AHSRC:base/rasw_api.c
//AHSRC:base/rasw_context.c
//AHSRC:base/rasw_drawbox.c
//AHSRC:base/rasw_drawprim.c
//AHSRC:base/rasw_drawline.c
//AHSRC:base/rasw_drawspan.c
//AHSRC:base/rasw_drawtris.c
//AHSRC:base/rasw_drawtris_flat.c
//AHSRC:base/rasw_drawtris_tex.c
//AHSRC:base/rasw_targa.c
//AHSRC:base/rasw_testblend.c
//AHSRC:base/rasw_texture.c
//AHSRC:base/rasw_thread.c
//AHSRC:raswgl/ragl_begin.c
//AHSRC:raswgl/ragl_clip.c
//AHSRC:raswgl/ragl_gldrv.c
//AHSRC:raswgl/ragl_glw.c
//AHSRC:raswgl/ragl_light.c
//AHSRC:raswgl/ragl_raswprim.c
//AHSRC:raswgl/ragl_teximage.c
//AHSRC:raswgl/ragl_transform.c
//AHSRC:raswgl/ragl_vertexarray.c
//AHSRC:vecmath/math_clip.c
VECMATH_API int Hull_LinePlaneIntersect(float *start, float *end,float *norm, float *point);
VECMATH_API int Hull_LinePlaneIntersectST(float *sv, float *sst,float *ev, float *est, float *norm, float *ov, float *ost);
VECMATH_API int Hull_LinePlaneIntersectSTNVRGBA(float *norm, float *sv, float *sst, float *snv, float *sclr, float *ev, float *est, float *env, float *eclr, float *ov, float *ost, float *onv, float *oclr, int flags);
VECMATH_API void Hull_AdjacentNormals(float *norm, float *udir, float *vdir);
VECMATH_API void Hull_AdjacentNormals2(float *norm, float *udir, float *vdir);
VECMATH_API void Hull_AdjacentNormals21(float *norm, float *udir, float *vdir);
VECMATH_API void Hull_AdjacentNormals22(float *norm, float *udir, float *vdir);
VECMATH_API void Hull_AdjacentNormals23(float *norm, float *udir, float *vdir);
VECMATH_API void Hull_MakePlaneFace(float *norm, float *pts);
VECMATH_API int Hull_ClipFace(float *norm,float *ipts, float *opts, int num);
VECMATH_API int Hull_ClipFaceST(float *norm,float *ipts, float *ist, float *opts, float *ost, int num);
VECMATH_API int Hull_ClipFaceSTNVRGBA(float *norm,int xyzsz, int stsz, int nvsz, int clrsz, float *ipts, float *ist, float *inv, float *iclr, float *opts, float *ost, float *onv, float *oclr, int num, int flags);
VECMATH_API void Hull_BoxPlaneExtents(float *mins, float *maxs, float *norm, float *rm, float *rn);
VECMATH_API void Hull_SpherePlaneExtents(float *org, float rad, float *norm, float *rm, float *rn);
//AHSRC:vecmath/math_hullf.c
VECMATH_API void HullF_MakePlaneFace(float *norm, float *pts);
VECMATH_API int HullF_ClipFace(float *norm,float *ipts, float *opts, int num);
VECMATH_API int HullF_BoxHull(float *norm, int num,float *mins, float *maxs);
//AHSRC:vecmath/math_m3.c
VECMATH_API void Mat3F_Copy(float *a, float *b);
VECMATH_API void Mat3F_Transpose(float *a, float *b);
VECMATH_API void Mat3F_Identity(float *a);
VECMATH_API void Mat3F_Mat3Mult(float *a, float *b, float *c);
VECMATH_API void Mat3F_Mat3MultT(float *a, float *b, float *c);
VECMATH_API void Mat3F_Mat3TMult(float *a, float *b, float *c);
VECMATH_API void Mat3F_TransformVector(float *a, float *xform, float *c);
VECMATH_API void Mat3F_Inverse(float *a, float *b);
VECMATH_API void Mat3F_AxisTo3Matrix(float *a, float th, float *b);
VECMATH_API void Mat3F_Filter3Matrix(float *a, float *b);
VECMATH_API void Mat3F_Rotate3Matrix(float *a, float *v, float th, float *b);
VECMATH_API int Mat3F_ArcPolate(float *v0, float *v1, float *v2, float t, float *rnv);
VECMATH_API void Mat3F_LinePolate(float *v0, float *v1, float *v2, float t,float *n);
VECMATH_API void Mat3F_Interpolate(float *m0, float *m1, float *m2, float t);
//AHSRC:vecmath/math_m4.c
VECMATH_API void Mat4F_Identity(float *a);
VECMATH_API void Mat4F_Copy(float *a, float *b);
VECMATH_API int Mat4F_Equal(float *a, float *b);
VECMATH_API void Mat4F_MatMult(float *a, float *b, float *c);
VECMATH_API void Mat4F_MatMultT(float *a, float *b, float *c);
VECMATH_API void Mat4F_Transpose(float *a, float *b);
VECMATH_API void Mat4F_Inverse(float *a, float *b);
VECMATH_API void Mat4F_Inverse(float *a, float *b);
VECMATH_API void Mat4F_InverseTransform(float *a, float *b);
VECMATH_API void Mat4F_InterpolateTransform(float *m0, float *m1, float *m2, float t);
VECMATH_API void Mat4F_TransformVector4(float *a, float *b, float *c);
VECMATH_API void Mat4F_TransformVector4T(float *a, float *b, float *c);
VECMATH_API void Mat4F_TransformPoint34G(float *point, float *xform, float *org);
VECMATH_API void Mat4F_TransformVector(float *dir, float *xform, float *v);
VECMATH_API void Mat4F_TransformVectorT(float *dir, float *xform, float *v);
VECMATH_API void Mat4F_TransformNormal(float *dir, float *xform, float *norm);
VECMATH_API void Mat4F_TransformNormalT(float *dir, float *xform, float *norm);
VECMATH_API void Mat4F_TransformNormalN(float *dir, float *xform, float *norm);
VECMATH_API void Mat4F_TransformNormalNT(float *dir, float *xform, float *norm);
VECMATH_API void Mat4F_TransformPoint(float *point, float *xform, float *org);
VECMATH_API void Mat4F_TransformPointT(float *point, float *xform, float *org);
VECMATH_API void Mat4F_TransformPointN(float *point, float *xform, float *org);
VECMATH_API void Mat4F_TransformPointNT(float *point, float *xform, float *org);
VECMATH_API void Mat4F_TransformPointLocal(float *point, float *xform, float *org);
VECMATH_API void Mat4F_TransformPointLocalT(float *point, float *xform, float *org);
VECMATH_API void Mat4F_AxisTo4Matrix(float *a, float th, float *b);
VECMATH_API void Mat4F_AxisTo4MatrixT(float *a, float th, float *b);
VECMATH_API void Mat4F_Filter4Matrix(float *a, float *b);
VECMATH_API void Mat4F_Rotate4Matrix(float *a, float *v, float th, float *b);
VECMATH_API void Mat4F_Set4MatrixRotXY(float *a, float th);
VECMATH_API void Mat4F_Set4MatrixRotXZ(float *a, float th);
VECMATH_API void Mat4F_Set4MatrixRotYZ(float *a, float th);
VECMATH_API void Mat4F_Set4MatrixRotXW(float *a, float th);
VECMATH_API void Mat4F_Set4MatrixRotYW(float *a, float th);
VECMATH_API void Mat4F_Set4MatrixRotZW(float *a, float th);
VECMATH_API void Mat4F_Rotate4MatrixXY(float *a, float th, float *b);
VECMATH_API void Mat4F_Rotate4MatrixXZ(float *a, float th, float *b);
VECMATH_API void Mat4F_Rotate4MatrixYZ(float *a, float th, float *b);
VECMATH_API void Mat4F_Rotate4MatrixXW(float *a, float th, float *b);
VECMATH_API void Mat4F_Rotate4MatrixYW(float *a, float th, float *b);
VECMATH_API void Mat4F_Rotate4MatrixZW(float *a, float th, float *b);
VECMATH_API void Mat4F_Rotate4MatrixAngles(float *a, float *ang, float *b);
//AHSRC:vecmath/math_mf.c
VECMATH_API void Mat34F_MatMult(float *a, float *b, float *c);
VECMATH_API void Mat34F_Transpose(float *a, float *b);
VECMATH_API void Mat3to4F_Copy(float *a, float *b);
VECMATH_API void Mat3to4F_Transpose(float *a, float *b);
//AHSRC:vecmath/math_n.c
VECMATH_API float Vec3F_DistLine(float *pt, float *v0, float *v1);
VECMATH_API float Vec3F_DistLineSeg(float *pt, float *v0, float *v1);
//AHSRC:vecmath/math_plane.c
VECMATH_API int PlaneF_LinePlaneIntersect(float *start, float *end, float *norm, float *point);
VECMATH_API int PlaneF_CheckLinePlane(float *start, float *end, float *norm);
VECMATH_API int PlaneF_ClipLinePlane(float *start, float *end, float *norm);
VECMATH_API void PlaneF_NearestPointLinePlane(float *start, float *end,float *norm, float *pt);
VECMATH_API int PlaneF_CheckPlaneBox(float *norm, float *mins, float *maxs);
VECMATH_API void PlaneF_BoxAxisExtents(float *mins, float *maxs,float *norm, float *min, float *max);
VECMATH_API void PlaneF_BoxPlaneExtents(float *mins, float *maxs,float *norm, float *min, float *max);
VECMATH_API void PlaneF_TransformVector(float *dir, float *xform, float *v);
VECMATH_API void PlaneF_TransformVectorT(float *dir, float *xform, float *v);
VECMATH_API void PlaneF_TransformNormal(float *dir, float *xform, float *norm);
VECMATH_API void PlaneF_TransformNormalT(float *dir, float *xform, float *norm);
VECMATH_API void PlaneF_TransformNormalN(float *dir, float *xform, float *norm);
VECMATH_API void PlaneF_TransformNormalNT(float *dir, float *xform, float *norm);
VECMATH_API void PlaneF_TransformPoint(float *point, float *xform, float *org);
VECMATH_API void PlaneF_TransformPointT(float *point, float *xform, float *org);
VECMATH_API void PlaneF_TransformPointN(float *point, float *xform, float *org);
VECMATH_API void PlaneF_TransformPointNT(float *point, float *xform, float *org);
VECMATH_API void PlaneF_TransformPointLocal(float *point, float *xform, float *org);
VECMATH_API void PlaneF_AdjacentNormals(float *n, float *x, float *y);
VECMATH_API void PlaneF_AdjacentNormals2(float *norm, float *udir, float *vdir);
//AHSRC:vecmath/math_quat.c
VECMATH_API void QuatF_Identity(float *a);
VECMATH_API void QuatF_Negate(float *a, float *b);
VECMATH_API void QuatF_Conjugate(float *a, float *b);
VECMATH_API void QuatF_Recip(float *a, float *b);
VECMATH_API float QuatF_Magnitude(float *a);
VECMATH_API float QuatF_Normalize(float *a, float *b);
VECMATH_API void QuatF_Multiply(float *a, float *b, float *c);
VECMATH_API void QuatF_ToMatrix(float *a, float *b);
VECMATH_API void QuatF_To3Matrix(float *a, float *b);
VECMATH_API void QuatF_FromMatrix(float *a, float *b);
VECMATH_API void QuatF_From3Matrix(float *a, float *b);
VECMATH_API void QuatF_FromAxis(float *a, float b, float *c);
VECMATH_API void QuatF_ToAxis(float *a, float *b, float *c);
VECMATH_API void QuatF_FromAngles(float *a, float *b);
VECMATH_API void QuatF_ToAngles(float *a, float *b);
VECMATH_API void QuatF_Lerp(float *a, float *b, float t, float *c);
VECMATH_API void QuatF_SLerp(float *a, float *ib, float t, float *c);
//AHSRC:vecmath/math_solid.c
VECMATH_API void SolidAABB_BoxMove(float *bmins, float *bmaxs,float *start, float *end, float *mins, float *maxs);
VECMATH_API int SolidAABB_BoxCollideP(float *ma, float *na, float *mb, float *nb);
VECMATH_API int SolidAABB_BoxCollideAxis(float *ma, float *na, float *mb, float *nb);
VECMATH_API int SolidAABB_PointInBoxP(float *p, float *min, float *max);
VECMATH_API void SolidAABB_BoxIntersection(float *ma, float *na,float *mb, float *nb, float *mc, float *nc);
VECMATH_API int SolidAABB_BoxLineCollideP(float *min, float *max, float *sp, float *ep);
VECMATH_API int SolidOBB_CheckObbPlane(float *mins, float *maxs,float *xform, float *norm, float *io);
VECMATH_API void SolidOBB_GenPoints(float *mins, float *maxs, float *pos, float *pts);
VECMATH_API int SolidOBB_ObbPlaneIntersect(float *mins, float *maxs,float *xform, float *norm, float *io);
VECMATH_API int SolidOBB_ObbPlaneExtents(float *mins, float *maxs,float *xform, float *norm, float *ext);
VECMATH_API void SolidOBB_PlaneExtents(float *mins, float *maxs,float *xform, float *norm, float *min, float *max);
VECMATH_API int SolidOBB_ObbPlaneMoveExtents(float *mins, float *maxs,float *sxform, float *exform, float *norm, float *ext);
VECMATH_API int SolidOBB_CheckObbMovePlane(float *mins, float *maxs,float *sxform, float *exform, float *norm, float *io);
VECMATH_API int SolidOBB_CheckCollide(float *amins, float *amaxs, float *apos, float *bmins, float *bmaxs, float *bpos);
VECMATH_API int SolidOBB_CheckObbMove(float *amins, float *amaxs, float *aspos, float *aepos, float *bmins, float *bmaxs, float *bspos, float *bepos, float *org);
VECMATH_API int SolidOBB_ObbImpactNormal(float *amins, float *amaxs, float *aspos, float *aepos, float *bmins, float *bmaxs, float *bspos, float *bepos, float *vel, float *norm);
VECMATH_API void SolidOBB_Box(float *mins, float *maxs, float *pos,float *bmins, float *bmaxs);
VECMATH_API void SolidOBB_BoxMove(float *mins, float *maxs, float *spos, float *epos, float *bmins, float *bmaxs);
VECMATH_API int SolidOBB_ObbPointCollide(float *mins, float *maxs, float *pos, float *org);
VECMATH_API int SolidOBB_BoxLineCollideP(float *min, float *max, float *pos,float *sp, float *ep);
//AHSRC:vecmath/math_v3f.c
VECMATH_API void Vec3F_Set(float *a, float x, float y, float z);
VECMATH_API void Vec3F_Zero(float *a);
VECMATH_API void Vec3F_Copy(float *a, float *b);
VECMATH_API void Vec3F_Add(float *a, float *b, float *c);
VECMATH_API void Vec3F_Subtract(float *a, float *b, float *c);
VECMATH_API void Vec3F_Scale(float *a, float b, float *c);
VECMATH_API float Vec3F_DotProduct(float *a, float *b);
VECMATH_API float Vec3F_Length(float *a);
VECMATH_API void Vec3F_ScaleAdd(float *a, float b, float *c, float *d);
VECMATH_API void Vec3F_AddScale(float *a, float *b, float c, float *d);
VECMATH_API void Vec3F_ScaleAddScale(float *a, float b, float *c, float d, float *e);
VECMATH_API float Vec3F_Normalize(float *a, float *b);
VECMATH_API float Vec3F_Distance(float *a, float *b);
VECMATH_API void Vec3F_CrossProduct(float *a, float *b, float *c);
VECMATH_API float Vec3F_NearestRayDistance(float *a, float *adir, float *b, float *bdir);
VECMATH_API float Vec3F_NearestRayPoints(float *a, float *adir, float *b, float *bdir,float *pA, float *pB);
VECMATH_API float Vec3F_NearestLinePoints(float *As, float *Ae, float *Bs, float *Be,float *pA, float *pB);
VECMATH_API float Vec3F_NearestLineDistance(float *As, float *Ae, float *Bs, float *Be);
VECMATH_API int Vec3F_BoxTouchP(float *ma, float *na, float *mb, float *nb);
VECMATH_API int Vec3F_BoxCollideP(float *ma, float *na, float *mb, float *nb);
VECMATH_API void Vec3F_LinePlaneIntersect(float *start, float *end, float *norm,float *point);
VECMATH_API int Vec3F_LineBoxCollideP(float *m, float *n, float *s, float *e);
VECMATH_API void Vec3F_QuatToEulerYXZ(float *quat, float *ang);
VECMATH_API void Vec3F_QuatToEulerZXY(float *quat, float *ang);
VECMATH_API void Vec3F_QuatToEulerXYZ(float *quat, float *ang);
VECMATH_API void Vec3F_QuatFromEulerZXY(float *quat, float *ang);
VECMATH_API void Vec3F_Mat3ToEuler(float *mat, float *ang);
VECMATH_API void Vec3F_QuatToEulerZXZ(float *quat, float *ang);
VECMATH_API void Vec3F_AxisToRot3(float *axis, float angle, float *rot);
VECMATH_API void Vec3F_RotatePointAxis(float *src, float *axis, float angle, float *dst);
//AHSRC:vecmath/math_vecnf.c
//AHSRC:vecmath/math_vnf.c
VECMATH_API void VecNF_Copy(float *a, float *b, int n);
VECMATH_API void VecNF2D_Copy(float *a, double *b, int n);
VECMATH_API void VecNF_ResizeVector(float *a, int an, float *b, int bn);
VECMATH_API float VecNF_DotProduct(float *a, float *b, int n);
VECMATH_API float VecNF_Length(float *a, int n);
VECMATH_API float VecNF_Normalize(float *a, float *b, int n);
VECMATH_API float VecNF_Distance(float *a, float *b, int n);
VECMATH_API void VecNF_Add(float *a, float *b, float *c, int n);
VECMATH_API void VecNF_Subtract(float *a, float *b, float *c, int n);
VECMATH_API void VecNF_Scale(float *a, float b, float *c, int n);
VECMATH_API void VecNF_ScaleAdd(float *a, float b, float *c, float *d, int n);
VECMATH_API void VecNF_AddScale(float *a, float *b, float c, float *d, int n);
VECMATH_API void VecNF_ScaleAddScale(float *a, float b, float *c, float d, float *e, int n);
VECMATH_API void VecNF_Zero(float *a, int n);
VECMATH_API void VecNF_Const(float *a, float b, int n);
VECMATH_API void VecNF_ProjectPointOnPlane(float *d, float *v, float *z, int n);
VECMATH_API void VecNF_ProjectPointOnPlane2(float *d, float *v, float *z, int n);
VECMATH_API void VecNF_ProjectPointBelowPlane(float *d, float *v, float *z, int n);
VECMATH_API void VecNF_ProjectPointAbovePlane(float *d, float *v, float *z, int n);
VECMATH_API void VecNF_LinePlaneIntersect(float *start, float *end, float *norm, float *point, int n);
VECMATH_API int VecNF_CheckPlaneBox(float *norm, float *mins, float *maxs, int n);
//AHSRC:vecmath/mathlib_d.c
VECMATH_API void MatND_MatMult(double *a, int ah, int aw, double *b, int bh, int bw, double *c);
VECMATH_API void MatND_MatMultVector(double *a, double *b, double *c, int n);
VECMATH_API void MatND_MatMultScaler(double *a, double b, double *c, int n);
VECMATH_API void MatND_ResizeMatrix(double *a, int ah, int aw, double *b, int bh, int bw);
VECMATH_API void MatND_FillIdentity(double *a, int n);
VECMATH_API void MatND_Copy(double *a, double *b, int h, int w);
VECMATH_API void Vec16D_RotateMatrix(double *amat, double *angles, double *bmat);
VECMATH_API void MatND_GetLowerMatrix(double *a, double *b, int n);
VECMATH_API void MatND_GetUpperMatrix(double *a, double *b, int n);
VECMATH_API void Vec16D_TranslateMatrix(double *amat, double *pos, double *bmat);
VECMATH_API void MatND_SwapOrder(double *a, double *b, int n);
VECMATH_API void MatND_Inverse(double *a, double *b, int n);
VECMATH_API void VecND_Copy(double *a, double *b, int n);
VECMATH_API void VecND2F_Copy(double *a, float *b, int n);
VECMATH_API void VecND_ResizeVector(double *a, int an, double *b, int bn);
VECMATH_API double VecND_DotProduct(double *a, double *b, int n);
VECMATH_API double VecND_Length(double *a, int n);
VECMATH_API double VecND_Normalize(double *a, double *b, int n);
VECMATH_API double VecND_Distance(double *a, double *b, int n);
VECMATH_API void VecND_Add(double *a, double *b, double *c, int n);
VECMATH_API void VecND_Subtract(double *a, double *b, double *c, int n);
VECMATH_API void VecND_Scale(double *a, double b, double *c, int n);
VECMATH_API void VecND_ScaleAdd(double *a, double b, double *c, double *d, int n);
VECMATH_API void VecND_AddScale(double *a, double *b, double c, double *d, int n);
VECMATH_API void VecND_ScaleAddScale(double *a, double b, double *c, double d, double *e, int n);
VECMATH_API void VecND_Zero(double *a, int n);
VECMATH_API void VecND_Const(double *a, double b, int n);
VECMATH_API void VecND_ProjectPointOnPlane(double *d, double *v, double *z, int n);
VECMATH_API void Vec3D_CrossProduct (double *v1, double *v2, double *cross);
VECMATH_API void Vec3D_ToSphericalCoords(double *a, double *b);
VECMATH_API void Vec3D_FromSphericalCoords(double *a, double *b);
VECMATH_API void Vec16D_InvertAngles(double *amat, double *bmat);
VECMATH_API void Vec3D_RotatePoint(double *p, double *angles);
VECMATH_API void Vec3D_CalcAngleVectors(double *angles, double *forward, double *right, double *up);
VECMATH_API void Vec3D_PointToParentSpace(double *a, double *sp, double *b);
//AHSRC:vecmath/mathlib_f.c
VECMATH_API float *VecNF_AllocVec(int sz);
VECMATH_API double *VecND_AllocVec(int sz);
VECMATH_API void MatNF_MatMult(float *a, int ah, int aw, float *b, int bh, int bw, float *c);
VECMATH_API void MatNF_MatMultT(float *a, int ah, int aw, float *b, int bh, int bw, float *c);
VECMATH_API void MatNF_MatMultVector(float *a, float *b, float *c, int n);
VECMATH_API void MatNF_MatMultScaler(float *a, float b, float *c, int n);
VECMATH_API void MatNF_ResizeMatrix(float *a, int ah, int aw, float *b, int bh, int bw);
VECMATH_API void MatNF_FillIdentity(float *a, int n);
VECMATH_API void MatNF_Copy(float *a, float *b, int h, int w);
VECMATH_API void Vec16F_RotateMatrix(float *amat, float *angles, float *bmat);
VECMATH_API void MatNF_GetLowerMatrix(float *a, float *b, int n);
VECMATH_API void MatNF_GetUpperMatrix(float *a, float *b, int n);
VECMATH_API void Vec16F_TranslateMatrix(float *amat, float *pos, float *bmat);
VECMATH_API void Vec16F_ScaleMatrix(float *amat, float *pos, float *bmat);
VECMATH_API void MatNF_SwapOrder(float *a, float *b, int n);
VECMATH_API void MatNF_Inverse(float *a, float *b, int n);
VECMATH_API void Vec3F_ToSphericalCoords(float *a, float *b);
VECMATH_API void Vec3F_FromSphericalCoords(float *a, float *b);
VECMATH_API void Vec16F_InvertAngles(float *amat, float *bmat);
VECMATH_API void Vec3F_RotatePoint(float *p, float *angles);
VECMATH_API void Vec3F_CalcAngleVectors(float *angles, float *forward, float *right, float *up);
VECMATH_API void Vec3F_NormalToParentSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_NormalFromParentSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_NormalToChildSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_NormalFromChildSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_PointToParentSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_PointFromParentSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_PointToChildSpace(float *a, float *sp, float *b);
VECMATH_API void Vec3F_PointFromChildSpace(float *a, float *sp, float *b);
VECMATH_API float Scaler_Random();
VECMATH_API float Scaler_SignRandom();
