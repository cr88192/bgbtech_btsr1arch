#include <bgbrasw.h>

int BGBRASW_YYTestAndBlend_AlphaTest_Never(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
		{ return(0); }
int BGBRASW_YYTestAndBlend_AlphaTest_Always(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
		{ return(1); }

int BGBRASW_YYTestAndBlend_AlphaTest_Equal(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return(BGBRASW_YYPIXEL_A(*srcc)==
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_YYTestAndBlend_AlphaTest_NotEqual(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return(BGBRASW_YYPIXEL_A(*srcc)!=
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}


int BGBRASW_YYTestAndBlend_AlphaTest_Less(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return(BGBRASW_YYPIXEL_A(*srcc)<
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_YYTestAndBlend_AlphaTest_Greater(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return(BGBRASW_YYPIXEL_A(*srcc)>
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_YYTestAndBlend_AlphaTest_LessEqual(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return(BGBRASW_YYPIXEL_A(*srcc)<=
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

int BGBRASW_YYTestAndBlend_AlphaTest_GreaterEqual(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	return(BGBRASW_YYPIXEL_A(*srcc)>=
		(data->ref_clr&BGBRASW_MASK_CLR_ALPHA));
}

bgbrasw_pixel BGBRASW_YYDoBlend_None(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	return(src);
}

bgbrasw_pixel BGBRASW_YYDoBlend_Generic(
	BGBRASW_TestBlendData *data,
	bgbrasw_pixel src, bgbrasw_pixel dst)
{
	int sr0, sg0, sb0, sa0, sr1, sg1, sb1, sa1;
	int sr2, sg2, sb2, sa2, sr3, sg3, sb3, sa3;
	int dr0, dg0, db0, da0, dr1, dg1, db1, da1;
	int dr2, dg2, db2, da2, dr3, dg3, db3, da3;
	int cr0, cg0, cb0, ca0, cr1, cg1, cb1, ca1;
	int cr2, cg2, cb2, ca2, cr3, cg3, cb3, ca3;

	int sr, sg, sb, sa;
	int dr, dg, db, da;
	int cr, cg, cb, ca;
	bgbrasw_pixel clr;

	BGBRASW_UnpackYYPixelComponents(src, 
		&sr0, &sg0, &sb0, &sa0,
		&sr1, &sg1, &sb1, &sa1,
		&sr2, &sg2, &sb2, &sa2,
		&sr3, &sg3, &sb3, &sa3);
	BGBRASW_UnpackYYPixelComponents(dst, 
		&dr0, &dg0, &db0, &da0,
		&dr1, &dg1, &db1, &da1,
		&dr2, &dg2, &db2, &da2,
		&dr3, &dg3, &db3, &da3);

	BGBRASW_Blend_Generic(data,
		sr0, sg0, sb0, sa0, dr0, dg0, db0, da0,
		&cr0, &cg0, &cb0, &ca0);
	BGBRASW_Blend_Generic(data,
		sr1, sg1, sb1, sa1, dr1, dg1, db1, da1,
		&cr1, &cg1, &cb1, &ca1);
	BGBRASW_Blend_Generic(data,
		sr2, sg2, sb2, sa2, dr2, dg2, db2, da2,
		&cr2, &cg2, &cb2, &ca2);
	BGBRASW_Blend_Generic(data,
		sr3, sg3, sb3, sa3, dr3, dg3, db3, da3,
		&cr3, &cg3, &cb3, &ca3);

	clr=BGBRASW_PackYYPixelComponents(
		cr0, cg0, cb0, ca0,		cr1, cg1, cb1, ca1,
		cr2, cg2, cb2, ca2,		cr3, cg3, cb3, ca3);

//	sr=BGBRASW_PIXEL_R(src);	sg=BGBRASW_PIXEL_G(src);
//	sb=BGBRASW_PIXEL_B(src);	sa=BGBRASW_PIXEL_A(src);
//	dr=BGBRASW_PIXEL_R(dst);	dg=BGBRASW_PIXEL_G(dst);
//	db=BGBRASW_PIXEL_B(dst);	da=BGBRASW_PIXEL_A(dst);

//	BGBRASW_Blend_Generic(data,
//		sr, sg, sb, sa, dr, dg, db, da,
//		&cr, &cg, &cb, &ca);
//	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);

	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_Zero_SrcColor(
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

//	BGBRASW_Blend_Generic(data,
//		sr, sg, sb, sa, dr, dg, db, da,
//		&cr, &cg, &cb, &ca);
	cr=(dr*sr)>>8;	cg=(dg*sg)>>8;
	cb=(db*sb)>>8;	ca=(da*sa)>>8;
	
//	cr=(cr<0)?0:((cr>255)?255:cr);
//	cg=(cg<0)?0:((cg>255)?255:cg);
//	cb=(cb<0)?0:((cb>255)?255:cb);
//	ca=(ca<0)?0:((ca>255)?255:ca);
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
#endif

#ifdef BGBRASW_SS_YUVA
	clr=bgbrasw_yycolormul(src, dst);
#else
	clr=bgbrasw_colormul(src, dst);
#endif

//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_Zero_OneMinusSrcColor(
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

//	BGBRASW_Blend_Generic(data,
//		sr, sg, sb, sa, dr, dg, db, da,
//		&cr, &cg, &cb, &ca);
	cr=(dr*(255-sr))>>8;	cg=(dg*(255-sg))>>8;
	cb=(db*(255-sb))>>8;	ca=(da*(255-sa))>>8;
	
//	cr=(cr<0)?0:((cr>255)?255:cr);
//	cg=(cg<0)?0:((cg>255)?255:cg);
//	cb=(cb<0)?0:((cb>255)?255:cb);
//	ca=(ca<0)?0:((ca>255)?255:ca);
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_One_One(
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

//	BGBRASW_Blend_Generic(data,
//		sr, sg, sb, sa, dr, dg, db, da,
//		&cr, &cg, &cb, &ca);
	cr=(dr+sr);	cg=(dg+sg);
	cb=(db+sb);	ca=(da+sa);
	
	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;
//	cr=(cr<0)?0:((cr>255)?255:cr);
//	cg=(cg<0)?0:((cg>255)?255:cg);
//	cb=(cb<0)?0:((cb>255)?255:cb);
//	ca=(ca<0)?0:((ca>255)?255:ca);
	
	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_DstColor_Zero(
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
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
#endif

	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_DstColor_One(
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

//	cr=(dr*sr+dr)>>8;	cg=(dg*sg+dg)>>8;
//	cb=(db*sb+db)>>8;	ca=(da*sa+da)>>8;
	cr=dr+((dr*sr)>>8);	cg=dg+((dg*sg)>>8);
	cb=db+((db*sb)>>8);	ca=da+((da*sa)>>8);
	
	cr=(cr>255)?255:cr;
	cg=(cg>255)?255:cg;
	cb=(cb>255)?255:cb;
	ca=(ca>255)?255:ca;

	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_DstColor_SrcColor(
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
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_DstColor_SrcAlpha(
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
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_DstColor_OneMinusSrcAlpha(
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
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_DstColor_OneMinusDstAlpha(
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
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

bgbrasw_pixel BGBRASW_YYDoBlend_SrcAlpha_OneMinusSrcAlpha(
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

//	BGBRASW_Blend_Generic(data,
//		sr, sg, sb, sa, dr, dg, db, da,
//		&cr, &cg, &cb, &ca);

	nsa=256-sa;
	cr=(sr*sa+dr*nsa)>>8;
	cg=(sg*sa+dg*nsa)>>8;
	cb=(sb*sa+db*nsa)>>8;
	ca=(sa*sa+da*nsa)>>8;

	clr=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
//	clr=(clr&data->mask_clr)|
//		(dst&(~data->mask_clr));
	return(clr);
}

int BGBRASW_YYTestAndBlend_Generic(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
		return(0);
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
		return(0);
	
	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstz=*srcz;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	return(1);
}

int BGBRASW_YYTestAndBlend_DepthBlend(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	bgbrasw_pixel clr;
//	if(!tabs->testAlpha(tabs, srcc, srcz, dstc, dstz))
//		return(0);
	if(!tabs->testDepth(tabs, srcc, srcz, dstc, dstz))
		return(0);
	
	clr=tabs->doBlend(tabs, *srcc, *dstc);
	clr=(clr&tabs->mask_clr)|
		((*dstc)&(~tabs->mask_clr));
	*dstc=clr;
//	*dstz=*srcz;
	*dstz=((*srcz)&tabs->mask_z)|((*dstz)&(~tabs->mask_z));
	return(1);
}

int BGBRASW_YYTestAndBlend_DepthBlend_LessEqual(
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

int BGBRASW_YYTestAndBlend_DepthBlend_LessEqualNoMask(
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

int BGBRASW_YYTestAndBlend_DepthBlend_LessEqual_NoClrMask_NoDepthWrite(
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

int BGBRASW_YYTestAndBlend_AlphaDepth(
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

int BGBRASW_YYTestAndBlend_Depth(
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

int BGBRASW_YYTestAndBlend_AlphaBlend(
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

int BGBRASW_YYTestAndBlend_Blend(
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

int BGBRASW_YYTestAndBlend_Alpha(
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

int BGBRASW_YYTestAndBlend_None(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *srcc, bgbrasw_zbuf *srcz,
	bgbrasw_pixel *dstc, bgbrasw_zbuf *dstz)
{
	*dstc=*srcc;
//	*dstz=*srcz;
	return(1);
}


int BGBRASW_YYSetupTestBlend(BGBRASW_Context *ctx,
	BGBRASW_TestBlendData *tabs)
{
	tabs->drawSpanFlat=BGBRASW_DrawSpanFlatInterpTestBlend;
//	tabs->drawSpanTex=BGBRASW_DrawSpanTextureInterpTestBlend;
//	tabs->drawSpanTex=BGBRASW_DrawSpanTextureLinearInterpTestBlend;

	tabs->drawSpanTex_min=BGBRASW_DrawSpanTextureInterpTestBlend;
	tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;

	tabs->testAndBlend=BGBRASW_YYTestAndBlend_Generic;
//	tabs->doBlend=BGBRASW_YYDoBlend_Generic;

	tabs->drawSpanFlatBasic=BGBRASW_DrawSpanFlatBasic;
	tabs->drawSpanFlatInterp=BGBRASW_DrawSpanFlatInterp;
	tabs->drawSpanFlatBasicZTest=BGBRASW_DrawSpanFlatBasicZTest;
	tabs->drawSpanFlatInterpZTest=BGBRASW_DrawSpanFlatInterpZTest;
	tabs->drawSpanTextureBasic=BGBRASW_DrawSpanTextureBasic;
	tabs->drawSpanTextureInterp=BGBRASW_DrawSpanTextureInterp;
	tabs->drawSpanTextureBasicZTest=BGBRASW_DrawSpanTextureBasicZTest;
	tabs->drawSpanTextureInterpZTest=BGBRASW_DrawSpanTextureInterpZTest;

	if(tabs->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)
	{
		if(tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_Generic; }
			else
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_AlphaDepth; }
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
							BGBRASW_YYTestAndBlend_DepthBlend_LessEqualNoMask;
					}else if((tabs->mask_clr==0xFFFFFFFFUL) &&
						!(tabs->mask_z&BGBRASW_MASK_DEPTH))
					{
						tabs->testAndBlend=
		BGBRASW_YYTestAndBlend_DepthBlend_LessEqual_NoClrMask_NoDepthWrite;
					}else
					{
						tabs->testAndBlend=
							BGBRASW_YYTestAndBlend_DepthBlend_LessEqual;
					}
				}else
				{
					tabs->testAndBlend=BGBRASW_YYTestAndBlend_DepthBlend;
				}
			}
			else
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_Depth; }
		}
	}else
	{
		if(tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_AlphaBlend; }
			else
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_Alpha; }
		}else
		{
			if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_Blend; }
			else
				{ tabs->testAndBlend=BGBRASW_YYTestAndBlend_None; }
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
			break;
#if 0
		case BGBRASW_GL_LINEAR:
		case BGBRASW_GL_LINEAR_MIPMAP_NEAREST:
		case BGBRASW_GL_LINEAR_MIPMAP_LINEAR:
//			tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;
			tabs->drawSpanTex_mag=
				BGBRASW_DrawSpanTextureLinearInterpTestBlend;
			break;
#endif
		default:
			tabs->drawSpanTex_mag=BGBRASW_DrawSpanTextureInterpTestBlend;
			break;
		}
	}

	if(tabs->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
	{
		switch(tabs->alpha_func)
		{
		case BGBRASW_GL_NEVER:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Never;
			break;
		case BGBRASW_GL_ALWAYS:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Always;
			break;
		case BGBRASW_GL_EQUAL:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Equal;
			break;
		case BGBRASW_GL_NOTEQUAL:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Less;
			break;
		case BGBRASW_GL_LESS:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Less;
			break;
		case BGBRASW_GL_LEQUAL:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_LessEqual;
			break;
		case BGBRASW_GL_GREATER:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Greater;
			break;
		case BGBRASW_GL_GEQUAL:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_GreaterEqual;
			break;
		default:
			tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Always;
			break;
		}
	}else
	{
		tabs->testAlpha=BGBRASW_YYTestAndBlend_AlphaTest_Always;
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

	if(tabs->caps_enabled&BGBRASW_ENABLE_BLEND)
	{
		tabs->doBlend=BGBRASW_YYDoBlend_Generic;
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

#if 0
		switch(tabs->blend_src)
		{
		case BGBRASW_GL_ZERO:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_SRC_COLOR:
				tabs->doBlend=BGBRASW_YYDoBlend_Zero_SrcColor;
				break;
			case BGBRASW_GL_ONE_MINUS_SRC_COLOR:
				tabs->doBlend=BGBRASW_YYDoBlend_Zero_OneMinusSrcColor;
				break;
			default:
				break;
			}
			break;
		case BGBRASW_GL_ONE:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_ZERO:
				tabs->doBlend=BGBRASW_YYDoBlend_None;
				break;
			case BGBRASW_GL_ONE:
				tabs->doBlend=BGBRASW_YYDoBlend_One_One;
				break;
			default:
				break;
			}
			break;
		case BGBRASW_GL_SRC_ALPHA:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_ONE_MINUS_SRC_ALPHA:
				tabs->doBlend=BGBRASW_YYDoBlend_SrcAlpha_OneMinusSrcAlpha;
				break;
			default:
				break;
			}
			break;
		case BGBRASW_GL_DST_COLOR:
			switch(tabs->blend_dst)
			{
			case BGBRASW_GL_ZERO:
				tabs->doBlend=BGBRASW_YYDoBlend_DstColor_Zero;
				break;
			case BGBRASW_GL_ONE:
				tabs->doBlend=BGBRASW_YYDoBlend_DstColor_One;
				break;
			case BGBRASW_GL_SRC_COLOR:
				tabs->doBlend=BGBRASW_YYDoBlend_DstColor_SrcColor;
				break;
			case BGBRASW_GL_SRC_ALPHA:
				tabs->doBlend=BGBRASW_YYDoBlend_DstColor_SrcAlpha;
				break;
			case BGBRASW_GL_ONE_MINUS_SRC_ALPHA:
				tabs->doBlend=BGBRASW_YYDoBlend_DstColor_OneMinusSrcAlpha;
				break;
			case BGBRASW_GL_ONE_MINUS_DST_ALPHA:
				tabs->doBlend=BGBRASW_YYDoBlend_DstColor_OneMinusDstAlpha;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
#endif

	}else
	{
		tabs->doBlend=BGBRASW_YYDoBlend_None;
	}
	
	return(0);
}
