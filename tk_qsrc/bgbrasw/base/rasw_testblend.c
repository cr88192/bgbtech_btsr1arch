#include <bgbrasw.h>

int BGBRASW_TestAndBlend_AlphaTest_Never(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
		{ return(0); }
int BGBRASW_TestAndBlend_AlphaTest_Always(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
		{ return(1); }

int BGBRASW_TestAndBlend_AlphaTest_Equal(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return((*srcc&BGBRASW_MASK_CLR_ALPHA)==
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_TestAndBlend_AlphaTest_NotEqual(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return((*srcc&BGBRASW_MASK_CLR_ALPHA)!=
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}


int BGBRASW_TestAndBlend_AlphaTest_Less(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return((*srcc&BGBRASW_MASK_CLR_ALPHA)<
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_TestAndBlend_AlphaTest_Greater(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return((*srcc&BGBRASW_MASK_CLR_ALPHA)>
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_TestAndBlend_AlphaTest_LessEqual(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return((*srcc&BGBRASW_MASK_CLR_ALPHA)<=
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_TestAndBlend_AlphaTest_GreaterEqual(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return((*srcc&BGBRASW_MASK_CLR_ALPHA)>=
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_TestAndBlend_DepthTest_Never(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(0); }
int BGBRASW_TestAndBlend_DepthTest_Always(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(1); }

int BGBRASW_TestAndBlend_DepthTest_Less(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return((*srcz&BGBRASW_MASK_DEPTH)<(*dstz&BGBRASW_MASK_DEPTH)); }

int BGBRASW_TestAndBlend_DepthTest_LessEqual(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return((*srcz&BGBRASW_MASK_DEPTH)<=(*dstz&BGBRASW_MASK_DEPTH)); }

int BGBRASW_TestAndBlend_DepthTest_Greater(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return((*srcz&BGBRASW_MASK_DEPTH)>(*dstz&BGBRASW_MASK_DEPTH)); }

int BGBRASW_TestAndBlend_DepthTest_GreaterEqual(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return((*srcz&BGBRASW_MASK_DEPTH)>=(*dstz&BGBRASW_MASK_DEPTH)); }

int BGBRASW_TestAndBlend_DepthTest_Equal(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return((*srcz&BGBRASW_MASK_DEPTH)==(*dstz&BGBRASW_MASK_DEPTH)); }

int BGBRASW_TestAndBlend_DepthTest_NotEqual(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return((*srcz&BGBRASW_MASK_DEPTH)!=(*dstz&BGBRASW_MASK_DEPTH)); }


#if 1
int BGBRASW_TestAndBlend_StencilTest_Never(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(0); }
int BGBRASW_TestAndBlend_StencilTest_Always(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(1); }

int BGBRASW_TestAndBlend_StencilTest_Less(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(((data->ref_sten)&(data->mask_sten))<
		(*dstz&(data->mask_sten))); }

int BGBRASW_TestAndBlend_StencilTest_LessEqual(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(((data->ref_sten)&(data->mask_sten))<=
		(*dstz&(data->mask_sten))); }

int BGBRASW_TestAndBlend_StencilTest_Greater(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(((data->ref_sten)&(data->mask_sten))>
		(*dstz&(data->mask_sten))); }

int BGBRASW_TestAndBlend_StencilTest_GreaterEqual(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(((data->ref_sten)&(data->mask_sten))>=
		(*dstz&(data->mask_sten))); }

int BGBRASW_TestAndBlend_StencilTest_Equal(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(((data->ref_sten)&(data->mask_sten))==
		(*dstz&(data->mask_sten))); }

int BGBRASW_TestAndBlend_StencilTest_NotEqual(
		BGBRASW_TestBlendData *data,
		bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
		bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
	{ return(((data->ref_sten)&(data->mask_sten))!=
		(*dstz&(data->mask_sten))); }
#endif

#if 1
void BGBRASW_TestAndBlend_StencilOp_Keep(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
	{ *dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z)); }

void BGBRASW_TestAndBlend_StencilOp_Zero(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
	{ *dstz=((*srcz)&tabs->mask_z); }

void BGBRASW_TestAndBlend_StencilOp_Replace(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
{
	*dstz=((*srcz)&tabs->mask_z)|((tabs->ref_sten)&(~tabs->mask_z));
}

void BGBRASW_TestAndBlend_StencilOp_Incr(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
{
	bgbrasw_zbuf i;
	i=((*dstz)&BGBRASW_MASK_STENCIL)+1;
	if(i&BGBRASW_MASK_STENCIL_INV)
		i=BGBRASW_MASK_STENCIL;
	*dstz=((*srcz)&tabs->mask_z)|i;
}

void BGBRASW_TestAndBlend_StencilOp_Decr(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
{
	bgbrasw_zbuf i;
	i=((*dstz)&BGBRASW_MASK_STENCIL)-1;
	if(i&BGBRASW_MASK_STENCIL_INV)
		i=0;
	*dstz=((*srcz)&tabs->mask_z)|i;
}

void BGBRASW_TestAndBlend_StencilOp_IncrWrap(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
{
	bgbrasw_zbuf i;
	i=((*dstz)&BGBRASW_MASK_STENCIL)+1;
	i=i&BGBRASW_MASK_STENCIL;
	*dstz=((*srcz)&tabs->mask_z)|(i&BGBRASW_MASK_STENCIL);
}

void BGBRASW_TestAndBlend_StencilOp_DecrWrap(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
{
	bgbrasw_zbuf i;
	i=((*dstz)&BGBRASW_MASK_STENCIL)-1;
	*dstz=((*srcz)&tabs->mask_z)|(i&BGBRASW_MASK_STENCIL);
}

void BGBRASW_TestAndBlend_StencilOp_Invert(
		BGBRASW_TestBlendData *tabs,
		bgbrasw_zbuf *srcz, bgbrasw_zbuf *dstz)
{
	bgbrasw_zbuf i;
	i=(~(*dstz))&BGBRASW_MASK_STENCIL;
	*dstz=((*srcz)&tabs->mask_z)|i;
}
#endif

void BGBRASW_BlendFunc_Zero(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{ *rfr=0;	*rfg=0;	*rfb=0;	*rfa=0; }
void BGBRASW_BlendFunc_One(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{ *rfr=256;	*rfg=256;	*rfb=256;	*rfa=256; }
void BGBRASW_BlendFunc_SrcColor(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=sr;	*rfg=sg;	*rfb=sb;	*rfa=sa;	}
void BGBRASW_BlendFunc_OneMinusSrcColor(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=256-sr;	*rfg=256-sg;	*rfb=256-sb;	*rfa=256-sa;	}
void BGBRASW_BlendFunc_DstColor(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=dr;	*rfg=dg;	*rfb=db;	*rfa=da;	}
void BGBRASW_BlendFunc_OneMinusDstColor(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=256-dr;	*rfg=256-dg;	*rfb=256-db;	*rfa=256-da;	}
void BGBRASW_BlendFunc_SrcAlpha(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=sa;	*rfg=sa;	*rfb=sa;	*rfa=sa;	}
void BGBRASW_BlendFunc_OneMinusSrcAlpha(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=256-sa;	*rfg=256-sa;	*rfb=256-sa;	*rfa=256-sa;	}
void BGBRASW_BlendFunc_DstAlpha(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=da;	*rfg=da;	*rfb=da;	*rfa=da;	}
void BGBRASW_BlendFunc_OneMinusDstAlpha(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rfr, int *rfg, int *rfb, int *rfa)
		{	*rfr=256-da;	*rfg=256-da;	*rfb=256-da;	*rfa=256-da;	}

void BGBRASW_Blend_Generic(
	BGBRASW_TestBlendData *data,
	int sr, int sg, int sb, int sa,
	int dr, int dg, int db, int da,
	int *rr, int *rg, int *rb, int *ra)
{
	int sfr, sfg, sfb, sfa;
	int dfr, dfg, dfb, dfa;
	int cr, cg, cb, ca;
	
	data->blendSfunc(data,
		sr, sg, sb, sa,		dr, dg, db, da,	
		&sfr, &sfg, &sfb, &sfa);
	data->blendDfunc(data,
		sr, sg, sb, sa,		dr, dg, db, da,	
		&dfr, &dfg, &dfb, &dfa);
	
	cr=(sr*sfr+dr*dfr)>>8;
	cg=(sg*sfg+dg*dfg)>>8;
	cb=(sb*sfb+db*dfb)>>8;
	ca=(sa*sfa+da*dfa)>>8;
	
	cr=(cr<0)?0:((cr>255)?255:cr);
	cg=(cg<0)?0:((cg>255)?255:cg);
	cb=(cb<0)?0:((cb>255)?255:cb);
	ca=(ca<0)?0:((ca>255)?255:ca);
	
	*rr=cr;	*rg=cg;
	*rb=cb;	*ra=ca;
}

bgbrasw_pixel BGBRASW_DoBlend_None(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	return(src);
}

bgbrasw_pixel BGBRASW_DoBlend_Generic(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	BGBRASW_Blend_Generic(data,
		sr, sg, sb, sa, dr, dg, db, da,
		&cr, &cg, &cb, &ca);
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_Zero_SrcColor(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

#if 0
	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=(dr*sr)>>8;	cg=(dg*sg)>>8;
	cb=(db*sb)>>8;	ca=(da*sa)>>8;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
#endif

#ifdef BGBRASW_SS_YUVA
	clr=bgbrasw_yycolormul(src, dst);
#else
	clr=bgbrasw_colormul(src, dst);
#endif
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_Zero_OneMinusSrcColor(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=(dr*(255-sr))>>8;	cg=(dg*(255-sg))>>8;
	cb=(db*(255-sb))>>8;	ca=(da*(255-sa))>>8;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_One_One(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=(dr+sr);	cg=(dg+sg);
	cb=(db+sb);	ca=(da+sa);
	
	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_DstColor_Zero(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

#if 0
	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=(dr*sr)>>8;	cg=(dg*sg)>>8;
	cb=(db*sb)>>8;	ca=(da*sa)>>8;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
#endif


#ifdef BGBRASW_SS_YUVA
	clr=bgbrasw_yycolormul(src, dst);
#else
	clr=bgbrasw_colormul(src, dst);
#endif

	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_DstColor_One(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=dr+((dr*sr)>>8);	cg=dg+((dg*sg)>>8);
	cb=db+((db*sb)>>8);	ca=da+((da*sa)>>8);
	
	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;

	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_DstColor_SrcColor(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=(dr*sr)>>7;	cg=(dg*sg)>>7;
	cb=(db*sb)>>7;	ca=(da*sa)>>7;

	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_DstColor_SrcAlpha(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	cr=(dr*sr+dr*sa)>>8;	cg=(dg*sg+dg*sa)>>8;
	cb=(db*sb+db*sa)>>8;	ca=(da*sa+da*sa)>>8;

	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_DstColor_OneMinusSrcAlpha(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa, nsa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	nsa=255-sa;
	cr=(dr*sr+dr*nsa)>>8;	cg=(dg*sg+dg*nsa)>>8;
	cb=(db*sb+db*nsa)>>8;	ca=(da*sa+da*nsa)>>8;

	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_DstColor_OneMinusDstAlpha(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa, nsa;
	int dr, dg, db, da, nda;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	nda=255-da;
	cr=(dr*sr+dr*nda)>>8;	cg=(dg*sg+dg*nda)>>8;
	cb=(db*sb+db*nda)>>8;	ca=(da*sa+da*nda)>>8;

	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

bgbrasw_pixel BGBRASW_DoBlend_SrcAlpha_OneMinusSrcAlpha(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr, sg, sb, sa, nsa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

	nsa=256-sa;
	cr=(sr*sa+dr*nsa)>>8;
	cg=(sg*sa+dg*nsa)>>8;
	cb=(sb*sa+db*nsa)>>8;
	ca=(sa*sa+da*nsa)>>8;

	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
	return(clr);
}

int BGBRASW_TestAndBlend_Generic(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
		return(0);
	if(!tabs->testStencil(tabs, srcc, srcz, dstc, dstz))
	{
		tabs->stenOpSfail(tabs, srcz, dstz);
		return(0);
	}
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
	{
		tabs->stenOpDpFail(tabs, srcz, dstz);
		return(0);
	}
	
	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	tabs->stenOpDpPass(tabs, srcz, dstz);
	return(1);
}

int BGBRASW_TestAndBlend_DepthBlend(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
	{
//		tabs->stenOpDpFail(tabs, srcz, dstz);
		return(0);
	}
	
	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstz=*srcz;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
//	tabs->stenOpDpPass(tabs, srcz, dstz);
	return(1);
}

int BGBRASW_TestAndBlend_DepthBlend_LessEqual(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
//	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
//		return(0);
//	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
//		return(0);

	if((*srcz)<=(*dstz))
	{
		clr=tabs->doBlend(tabs, *srcc, *dstc);
		clr=(clr&tabs->mask_clr)|
			((*dstc)&(~tabs->mask_clr));
		*dstc=clr;
		*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
		return(1);
	}
	return(0);
}

int BGBRASW_TestAndBlend_DepthBlend_LessEqualNoMask(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
//	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
//		return(0);
//	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
//		return(0);

	if((*srcz)<=(*dstz))
	{
		clr=tabs->doBlend(tabs, *srcc, *dstc);
//		clr=(clr&data->mask_clr)|
//			((*dstc)&(~data->mask_clr));
		*dstc=clr;
		*dstz=*srcz;
//		*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
		return(1);
	}
	return(0);
}

int BGBRASW_TestAndBlend_DepthBlend_LessEqual_NoClrMask_NoDepthWrite(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
//	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
//		return(0);
//	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
//		return(0);

	if((*srcz)<=(*dstz))
	{
		clr=tabs->doBlend(tabs, *srcc, *dstc);
//		clr=(clr&data->mask_clr)|
//			((*dstc)&(~data->mask_clr));
		*dstc=clr;
//		*dstz=*srcz;
//		*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
		return(1);
	}
	return(0);
}

int BGBRASW_TestAndBlend_AlphaDepth(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
		return(0);
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
		return(0);
	
//	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=*srcc;
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));	
	*dstc=clr;
//	*dstz=*srcz;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	return(1);
}

int BGBRASW_TestAndBlend_Depth(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
		return(0);
	clr=*srcc;	
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstc=*srcc;
//	*dstz=*srcz;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	return(1);
}

int BGBRASW_TestAndBlend_DepthStencil(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testStencil(tabs, srcc, srcz, dstc, dstz))
	{
		tabs->stenOpSfail(tabs, srcz, dstz);
		return(0);
	}
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
	{
		tabs->stenOpDpFail(tabs, srcz, dstz);
		return(0);
	}
	clr=*srcc;	
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstc=*srcc;
//	*dstz=*srcz;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	tabs->stenOpDpPass(tabs, srcz, dstz);
	return(1);
}

int BGBRASW_TestAndBlend_Stencil(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testStencil(tabs, srcc, srcz, dstc, dstz))
	{
		tabs->stenOpSfail(tabs, srcz, dstz);
		return(0);
	}
	clr=*srcc;	
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	tabs->stenOpDpPass(tabs, srcz, dstz);
	return(1);
}

int BGBRASW_TestAndBlend_AlphaBlend(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
		return(0);	
	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstz=*srcz;
	return(1);
}

int BGBRASW_TestAndBlend_Blend(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstz=*srcz;
	return(1);
}

int BGBRASW_TestAndBlend_Alpha(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
		return(0);	
	*dstc=*srcc;
//	*dstz=*srcz;
	return(1);
}

int BGBRASW_TestAndBlend_None(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	*dstc=*srcc;
//	*dstz=*srcz;
	return(1);
}

BGBRASW_StencilOpFunc_ft BGBRASW_TestAndBlend_GetStencilOpFunc(
	BGBRASW_Context *ctx,
	BGBRASW_TestBlendData *tabs,
	int op)
{
	BGBRASW_StencilOpFunc_ft opf;
	switch(op)
	{
	case BGBRASW_GL_KEEP:
		opf=BGBRASW_TestAndBlend_StencilOp_Keep;
		break;
	case BGBRASW_GL_ZERO:
		opf=BGBRASW_TestAndBlend_StencilOp_Zero;
		break;
	case BGBRASW_GL_REPLACE:
		opf=BGBRASW_TestAndBlend_StencilOp_Replace;
		break;
	case BGBRASW_GL_INCR:
		opf=BGBRASW_TestAndBlend_StencilOp_Incr;
		break;
	case BGBRASW_GL_DECR:
		opf=BGBRASW_TestAndBlend_StencilOp_Decr;
		break;
	case BGBRASW_GL_INCR_WRAP:
		opf=BGBRASW_TestAndBlend_StencilOp_IncrWrap;
		break;
	case BGBRASW_GL_DECR_WRAP:
		opf=BGBRASW_TestAndBlend_StencilOp_DecrWrap;
		break;
//	case BGBRASW_GL_INVERT:
//		opf=BGBRASW_TestAndBlend_StencilOp_Invert;
//		break;
	default:
		opf=BGBRASW_TestAndBlend_StencilOp_Keep;
		break;
	}
	return(opf);
}


int BGBRASW_SetupTestBlend(BGBRASW_Context *ctx,
	BGBRASW_TestBlendData *tabs)
{
#ifdef BGBRASW_SS_YUVA
	return(BGBRASW_YYSetupTestBlend(ctx, tabs));
#endif

	tabs->drawSpanFlat=BGBRASW_DrawSpanFlatInterpTestBlend;
//	tabs->drawSpanTex=BGBRASW_DrawSpanTextureInterpTestBlend;
//	tabs->drawSpanTex=BGBRASW_DrawSpanTextureLinearInterpTestBlend;

	tabs->drawSpanTex_min=BGBRASW_DrawSpanTextureInterpTestBlend;
	tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;
	tabs->drawSpanTex_mag2=BGBRASW_DrawSpanTextureInterpTestBlend;

	tabs->testAndBlend=BGBRASW_TestAndBlend_Generic;
//	tabs->doBlend=BGBRASW_DoBlend_Generic;

	tabs->drawSpanFlatBasic=BGBRASW_DrawSpanFlatBasic;
	tabs->drawSpanFlatInterp=BGBRASW_DrawSpanFlatInterp;
	tabs->drawSpanFlatBasicZTest=BGBRASW_DrawSpanFlatBasicZTest;
	tabs->drawSpanFlatInterpZTest=BGBRASW_DrawSpanFlatInterpZTest;
	tabs->drawSpanTextureBasic=BGBRASW_DrawSpanTextureBasic;
	tabs->drawSpanTextureInterp=BGBRASW_DrawSpanTextureInterp;
	tabs->drawSpanTextureBasicZTest=BGBRASW_DrawSpanTextureBasicZTest;
	tabs->drawSpanTextureInterpZTest=BGBRASW_DrawSpanTextureInterpZTest;

	tabs->stenOpSfail=BGBRASW_TestAndBlend_StencilOp_Keep;
	tabs->stenOpDpFail=BGBRASW_TestAndBlend_StencilOp_Keep;
	tabs->stenOpDpPass=BGBRASW_TestAndBlend_StencilOp_Keep;

	if(tabs->caps_enabled&BGBRASW_ENABLE_STENCIL_TEST)
	{
		tabs->stenOpSfail=BGBRASW_TestAndBlend_GetStencilOpFunc(
			ctx, tabs, tabs->stencil_op_sfail);
		tabs->stenOpDpFail=BGBRASW_TestAndBlend_GetStencilOpFunc(
			ctx, tabs, tabs->stencil_op_dpfail);
		tabs->stenOpDpPass=BGBRASW_TestAndBlend_GetStencilOpFunc(
			ctx, tabs, tabs->stencil_op_dppass);
	
		if((tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST) ||
			(tabs->caps_enabled&BGBRASW_ENABLE_BLEND))
		{
			tabs->testAndBlend=BGBRASW_TestAndBlend_Generic;
		}else
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)
			{
				tabs->testAndBlend=BGBRASW_TestAndBlend_DepthStencil;
			}else
			{
				tabs->testAndBlend=BGBRASW_TestAndBlend_Stencil;
			}
		}
	}else if(tabs->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)
	{
		if(tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_Generic; }
			else
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_AlphaDepth; }
		}else
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
			{
				if(tabs->depth_func==BGBRASW_GL_LEQUAL)
				{
					if((tabs->mask_clr==0xFFFFFFFFUL) &&
						(tabs->mask_z==0xFFFFFFFFUL))
					{
						tabs->testAndBlend=
							BGBRASW_TestAndBlend_DepthBlend_LessEqualNoMask;
					}else if((tabs->mask_clr==0xFFFFFFFFUL) &&
						!(tabs->mask_z&BGBRASW_MASK_DEPTH))
					{
						tabs->testAndBlend=
		BGBRASW_TestAndBlend_DepthBlend_LessEqual_NoClrMask_NoDepthWrite;
					}else
					{
						tabs->testAndBlend=
							BGBRASW_TestAndBlend_DepthBlend_LessEqual;
					}
				}else
				{
					tabs->testAndBlend=BGBRASW_TestAndBlend_DepthBlend;
				}
			}
			else
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_Depth; }
		}
	}else
	{
		if(tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_AlphaBlend; }
			else
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_Alpha; }
		}else
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_Blend; }
			else
				{ tabs->testAndBlend=BGBRASW_TestAndBlend_None; }
		}
	}

	if(tabs->tex)
	{
		switch(tabs->tex->tex_min_filter)
		{
		case BGBRASW_GL_LINEAR:
		case BGBRASW_GL_NEAREST:
		case BGBRASW_GL_NEAREST_MIPMAP_NEAREST:
		case BGBRASW_GL_NEAREST_MIPMAP_LINEAR:
			tabs->drawSpanTex_min=BGBRASW_DrawSpanTextureInterpTestBlend;
			break;
#if 0
//		case BGBRASW_GL_LINEAR:
		case BGBRASW_GL_LINEAR_MIPMAP_NEAREST:
		case BGBRASW_GL_LINEAR_MIPMAP_LINEAR:
//			tabs->drawSpanTex_min=BGBRASW_DrawSpanTextureInterpTestBlend;
			tabs->drawSpanTex_min=
				BGBRASW_DrawSpanTextureLinearInterpTestBlend;
			break;
#endif
		default:
			tabs->drawSpanTex_min=BGBRASW_DrawSpanTextureInterpTestBlend;
			break;
		}
		switch(tabs->tex->tex_mag_filter)
		{
		case BGBRASW_GL_NEAREST:
		case BGBRASW_GL_NEAREST_MIPMAP_NEAREST:
		case BGBRASW_GL_NEAREST_MIPMAP_LINEAR:
			tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;
			tabs->drawSpanTex_mag2=BGBRASW_DrawSpanTextureInterpTestBlend;
			break;
#if 1
		case BGBRASW_GL_LINEAR:
		case BGBRASW_GL_LINEAR_MIPMAP_NEAREST:
		case BGBRASW_GL_LINEAR_MIPMAP_LINEAR:
			tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;
			tabs->drawSpanTex_mag2=
				BGBRASW_DrawSpanTextureLinearInterpTestBlend;
			break;
#endif
		default:
			tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;
			tabs->drawSpanTex_mag2=BGBRASW_DrawSpanTextureInterpTestBlend;
			break;
		}
	}

	if(tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
	{
		switch(tabs->alpha_func)
		{
		case BGBRASW_GL_NEVER:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Never;
			break;
		case BGBRASW_GL_ALWAYS:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Always;
			break;
		case BGBRASW_GL_EQUAL:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Equal;
			break;
		case BGBRASW_GL_NOTEQUAL:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Less;
			break;
		case BGBRASW_GL_LESS:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Less;
			break;
		case BGBRASW_GL_LEQUAL:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_LessEqual;
			break;
		case BGBRASW_GL_GREATER:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Greater;
			break;
		case BGBRASW_GL_GEQUAL:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_GreaterEqual;
			break;
		default:
			tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Always;
			break;
		}
	}else
	{
		tabs->testAlpha=BGBRASW_TestAndBlend_AlphaTest_Always;
	}

	if(tabs->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)
	{
		switch(tabs->depth_func)
		{
		case BGBRASW_GL_NEVER:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Never;
			break;
		case BGBRASW_GL_ALWAYS:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Always;
			break;
		case BGBRASW_GL_EQUAL:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Equal;
			break;
		case BGBRASW_GL_NOTEQUAL:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Less;
			break;
		case BGBRASW_GL_LESS:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Less;
			break;
		case BGBRASW_GL_LEQUAL:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_LessEqual;
			break;
		case BGBRASW_GL_GREATER:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Greater;
			break;
		case BGBRASW_GL_GEQUAL:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_GreaterEqual;
			break;
		default:
			tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_LessEqual;
			break;
		}
	}else
	{
		tabs->testDepth=BGBRASW_TestAndBlend_DepthTest_Always;
	}

	if(tabs->caps_enabled&BGBRASW_ENABLE_STENCIL_TEST)
	{
		switch(tabs->stencil_func)
		{
		case BGBRASW_GL_NEVER:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Never;
			break;
		case BGBRASW_GL_ALWAYS:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Always;
			break;
		case BGBRASW_GL_EQUAL:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Equal;
			break;
		case BGBRASW_GL_NOTEQUAL:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Less;
			break;
		case BGBRASW_GL_LESS:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Less;
			break;
		case BGBRASW_GL_LEQUAL:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_LessEqual;
			break;
		case BGBRASW_GL_GREATER:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Greater;
			break;
		case BGBRASW_GL_GEQUAL:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_GreaterEqual;
			break;
		default:
			tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_LessEqual;
			break;
		}
	}else
	{
		tabs->testStencil=BGBRASW_TestAndBlend_StencilTest_Always;
	}

	if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
	{
		tabs->doBlend=BGBRASW_DoBlend_Generic;
		switch(tabs->blend_src)
		{
		case BGBRASW_GL_ZERO:
			tabs->blendSfunc=BGBRASW_BlendFunc_Zero; break;
		case BGBRASW_GL_ONE:
			tabs->blendSfunc=BGBRASW_BlendFunc_One; break;
		case BGBRASW_GL_SRC_COLOR:
			tabs->blendSfunc=BGBRASW_BlendFunc_SrcColor; break;
		case BGBRASW_GL_ONE_MINUS_SRC_COLOR:
			tabs->blendSfunc=BGBRASW_BlendFunc_OneMinusSrcColor; break;
		case BGBRASW_GL_SRC_ALPHA:
			tabs->blendSfunc=BGBRASW_BlendFunc_SrcAlpha; break;
		case BGBRASW_GL_ONE_MINUS_SRC_ALPHA:
			tabs->blendSfunc=BGBRASW_BlendFunc_OneMinusSrcAlpha; break;
		case BGBRASW_GL_DST_COLOR:
			tabs->blendSfunc=BGBRASW_BlendFunc_DstColor; break;
		case BGBRASW_GL_ONE_MINUS_DST_COLOR:
			tabs->blendSfunc=BGBRASW_BlendFunc_OneMinusDstColor; break;
		case BGBRASW_GL_DST_ALPHA:
			tabs->blendSfunc=BGBRASW_BlendFunc_DstAlpha; break;
		case BGBRASW_GL_ONE_MINUS_DST_ALPHA:
			tabs->blendSfunc=BGBRASW_BlendFunc_OneMinusDstAlpha; break;
		default:
			tabs->blendSfunc=BGBRASW_BlendFunc_SrcAlpha;
			break;
		}
		switch(tabs->blend_dst)
		{
		case BGBRASW_GL_ZERO:
			tabs->blendDfunc=BGBRASW_BlendFunc_Zero; break;
		case BGBRASW_GL_ONE:
			tabs->blendDfunc=BGBRASW_BlendFunc_One; break;
		case BGBRASW_GL_SRC_COLOR:
			tabs->blendDfunc=BGBRASW_BlendFunc_SrcColor; break;
		case BGBRASW_GL_ONE_MINUS_SRC_COLOR:
			tabs->blendDfunc=BGBRASW_BlendFunc_OneMinusSrcColor; break;
		case BGBRASW_GL_SRC_ALPHA:
			tabs->blendDfunc=BGBRASW_BlendFunc_SrcAlpha; break;
		case BGBRASW_GL_ONE_MINUS_SRC_ALPHA:
			tabs->blendDfunc=BGBRASW_BlendFunc_OneMinusSrcAlpha; break;
		case BGBRASW_GL_DST_COLOR:
			tabs->blendDfunc=BGBRASW_BlendFunc_DstColor; break;
		case BGBRASW_GL_ONE_MINUS_DST_COLOR:
			tabs->blendDfunc=BGBRASW_BlendFunc_OneMinusDstColor; break;
		case BGBRASW_GL_DST_ALPHA:
			tabs->blendDfunc=BGBRASW_BlendFunc_DstAlpha; break;
		case BGBRASW_GL_ONE_MINUS_DST_ALPHA:
			tabs->blendDfunc=BGBRASW_BlendFunc_OneMinusDstAlpha; break;
		default:
			tabs->blendDfunc=BGBRASW_BlendFunc_OneMinusSrcAlpha;
			break;
		}

		switch(tabs->blend_src)
		{
		case BGBRASW_GL_ZERO:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_SRC_COLOR:
				tabs->doBlend=BGBRASW_DoBlend_Zero_SrcColor;
				break;
			case BGBRASW_GL_ONE_MINUS_SRC_COLOR:
				tabs->doBlend=BGBRASW_DoBlend_Zero_OneMinusSrcColor;
				break;
			default:
				break;
			}
			break;
		case BGBRASW_GL_ONE:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_ZERO:
				tabs->doBlend=BGBRASW_DoBlend_None;
				break;
			case BGBRASW_GL_ONE:
				tabs->doBlend=BGBRASW_DoBlend_One_One;
				break;
			default:
				break;
			}
			break;
		case BGBRASW_GL_SRC_ALPHA:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_ONE_MINUS_SRC_ALPHA:
				tabs->doBlend=BGBRASW_DoBlend_SrcAlpha_OneMinusSrcAlpha;
				break;
			default:
				break;
			}
			break;
		case BGBRASW_GL_DST_COLOR:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_ZERO:
				tabs->doBlend=BGBRASW_DoBlend_DstColor_Zero;
				break;
			case BGBRASW_GL_ONE:
				tabs->doBlend=BGBRASW_DoBlend_DstColor_One;
				break;
			case BGBRASW_GL_SRC_COLOR:
				tabs->doBlend=BGBRASW_DoBlend_DstColor_SrcColor;
				break;
			case BGBRASW_GL_SRC_ALPHA:
				tabs->doBlend=BGBRASW_DoBlend_DstColor_SrcAlpha;
				break;
			case BGBRASW_GL_ONE_MINUS_SRC_ALPHA:
				tabs->doBlend=BGBRASW_DoBlend_DstColor_OneMinusSrcAlpha;
				break;
			case BGBRASW_GL_ONE_MINUS_DST_ALPHA:
				tabs->doBlend=BGBRASW_DoBlend_DstColor_OneMinusDstAlpha;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

	}else
	{
		tabs->doBlend=BGBRASW_DoBlend_None;
	}
	
	return(0);
}

void BGBRASW_CopyTestBlend(BGBRASW_Context *ctx,
	BGBRASW_TestBlendData *srctabs,
	BGBRASW_TestBlendData *dsttabs)
{
	dsttabs->tex=srctabs->tex;
	dsttabs->ref_clr=srctabs->ref_clr;
	dsttabs->ref_z=srctabs->ref_z;
	dsttabs->ref_sten=srctabs->ref_sten;
	dsttabs->mask_clr=srctabs->mask_clr;
	dsttabs->mask_z=srctabs->mask_z;
	dsttabs->mask_sten=srctabs->mask_sten;
	dsttabs->caps_enabled=srctabs->caps_enabled;

	dsttabs->blend_src=srctabs->blend_src;
	dsttabs->blend_dst=srctabs->blend_dst;
	dsttabs->depth_func=srctabs->depth_func;
	dsttabs->alpha_func=srctabs->alpha_func;

	dsttabs->stencil_func=srctabs->stencil_func;
	dsttabs->stencil_op_sfail=srctabs->stencil_op_sfail;
	dsttabs->stencil_op_dpfail=srctabs->stencil_op_dpfail;
	dsttabs->stencil_op_dppass=srctabs->stencil_op_dppass;

	dsttabs->drawSpanFlat=srctabs->drawSpanFlat;
	dsttabs->drawSpanTex_min=srctabs->drawSpanTex_min;
	dsttabs->drawSpanTex_mag=srctabs->drawSpanTex_mag;
	dsttabs->drawSpanTex_mag2=srctabs->drawSpanTex_mag2;
	dsttabs->testAndBlend=srctabs->testAndBlend;
	dsttabs->testAlpha=srctabs->testAlpha;
	dsttabs->testDepth=srctabs->testDepth;
	dsttabs->testStencil=srctabs->testStencil;
	dsttabs->blendSfunc=srctabs->blendSfunc;
	dsttabs->blendDfunc=srctabs->blendDfunc;

	dsttabs->stenOpSfail=srctabs->stenOpSfail;
	dsttabs->stenOpDpFail=srctabs->stenOpDpFail;
	dsttabs->stenOpDpPass=srctabs->stenOpDpPass;

	dsttabs->doBlend=srctabs->doBlend;

	dsttabs->drawSpanFlatBasic=srctabs->drawSpanFlatBasic;
	dsttabs->drawSpanFlatInterp=srctabs->drawSpanFlatInterp;
	dsttabs->drawSpanFlatBasicZTest=srctabs->drawSpanFlatBasicZTest;
	dsttabs->drawSpanFlatInterpZTest=srctabs->drawSpanFlatInterpZTest;
	dsttabs->drawSpanTextureBasic=srctabs->drawSpanTextureBasic;
	dsttabs->drawSpanTextureInterp=srctabs->drawSpanTextureInterp;
	dsttabs->drawSpanTextureBasicZTest=srctabs->drawSpanTextureBasicZTest;
	dsttabs->drawSpanTextureInterpZTest=srctabs->drawSpanTextureInterpZTest;
}
