#if 1

u64 TKRA_Blend_Zero_Zero(u64 sclr, u64 dclr)
	{ return(0); }
u64 TKRA_Blend_Zero_One(u64 sclr, u64 dclr)
	{ return(dclr); }

u64 TKRA_Blend_Zero_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	cval=tkra_padduhw_sat(sval, dval);

//	amod=0;
//	dmod=sval;
//	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_Zero_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
	amod=0;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}


u64 TKRA_Blend_Zero_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=0;
	dmod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_Zero_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
	amod=0;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_Zero_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=0;
//	dmod=~sval;

//	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);

	cval=tkra_pmuluhw(dval, ~sval);

	return(cval);
}

u64 TKRA_Blend_Zero_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
	amod=0;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_Zero_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=0;
	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_Zero_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
	amod=0;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}


u64 TKRA_Blend_One_Zero(u64 sclr, u64 dclr)
	{ return(sclr); }
u64 TKRA_Blend_One_One(u64 sclr, u64 dclr)
{
	return(tkra_padduhw_sat(sclr, dclr));
//	return(sclr+dclr);
}

u64 TKRA_Blend_One_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=0xFFFFFFFFFFFFFFFFULL;
	dmod=sval;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_One_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
//	amod=0xFFFFFFFFFFFFFFFFULL;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}


u64 TKRA_Blend_One_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=0xFFFFFFFFFFFFFFFFULL;
	dmod=dval;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_One_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
//	amod=0xFFFFFFFFFFFFFFFFULL;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_One_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=0xFFFFFFFFFFFFFFFFULL;
	dmod=~sval;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_One_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
//	amod=0xFFFFFFFFFFFFFFFFULL;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_One_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=0xFFFFFFFFFFFFFFFFULL;
	dmod=~dval;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_One_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
//	amod=0xFFFFFFFFFFFFFFFFULL;

//	cval=tkra_pmuluhw(sval, amod);
	cval=sval;
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}



u64 TKRA_Blend_SrcColor_Zero(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=sval;
	dmod=0;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_One(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=sval;
//	dmod=0xFFFFFFFFFFFFFFFFULL;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=sval;
	dmod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
	amod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=sval;
	dmod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
	amod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=sval;
	dmod=~amod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
	amod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=sval;
	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcColor_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
	amod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}



u64 TKRA_Blend_SrcAlpha_Zero(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

//	dmod=0;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_One(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

//	dmod=0xFFFFFFFFFFFFFFFFULL;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

	dmod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

	dmod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

	dmod=(dval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_SrcAlpha_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

	dmod=(dval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;
	dmod=~dmod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}



#if 1
u64 TKRA_Blend_OneMinusSrcColor_Zero(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;
//	dmod=0;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_One(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;
//	dmod=0xFFFFFFFFFFFFFFFFULL;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;
	dmod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;

	dmod=(sval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;
	dmod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;

	dmod=(dval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;
	dmod=~sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;

	dmod=(sval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;
	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcColor_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=~sval;

	dmod=(dval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;
	dmod=~dmod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}



u64 TKRA_Blend_OneMinusSrcAlpha_Zero(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

//	dmod=0;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_One(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

//	dmod=0xFFFFFFFFFFFFFFFFULL;

	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;

	dmod=amod;
	amod=~amod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	
//	cval = 0xFFFFFFFF0000FFFFULL;
	
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=(dval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=~amod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_OneMinusSrcAlpha_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	amod=~amod;

	dmod=(dval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;
	dmod=~dmod;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}
#endif


#if 1

u64 TKRA_Blend_DstColor_Zero(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=dval;
//	dmod=0;
	cval=tkra_pmuluhw(sval, dval);

//	cval=tkra_pmuluhw(sval, amod);
//	dval=tkra_pmuluhw(dval, dmod);
//	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_DstColor_One(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

//	amod=dval;
//	dmod=0xFFFFFFFFFFFFFFFFULL;

//	cval=tkra_pmuluhw(sval, amod);
	cval=tkra_pmuluhw(sval, dval);
//	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	
//	cval = 0xFFFFFFFF0000FFFFULL;

	return(cval);
}

u64 TKRA_Blend_DstColor_SrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=dval;
	dmod=sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);

//	cval = 0xFFFFFFFF0000FFFFULL;

	return(cval);
}

u64 TKRA_Blend_DstColor_SrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=dval;
	dmod=(sval>>48)&65535;
	dmod|=dmod<<16;
	dmod|=dmod<<32;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);

//	cval = 0xFFFFFFFF0000FFFFULL;

	return(cval);
}

u64 TKRA_Blend_DstColor_DstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=dval;
	dmod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_DstColor_DstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=amod;
	amod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_DstColor_OneMinusSrcColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=dval;
	dmod=~sval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_DstColor_OneMinusSrcAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(sval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
	amod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_DstColor_OneMinusDstColor(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=dval;
	dmod=~dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);
	return(cval);
}

u64 TKRA_Blend_DstColor_OneMinusDstAlpha(u64 sval, u64 dval)
{
	u64 amod, dmod, cval;

	amod=(dval>>48)&65535;
	amod|=amod<<16;
	amod|=amod<<32;
	dmod=~amod;
	amod=dval;

	cval=tkra_pmuluhw(sval, amod);
	dval=tkra_pmuluhw(dval, dmod);
	cval=tkra_padduhw_sat(cval, dval);

//	cval = 0xFFFFFFFF0000FFFFULL;

	return(cval);
}

#endif

int TKRA_SetupDrawBlend(TKRA_Context *ctx)
{
	u64 (*Blend)(u64 sval, u64 dval);
	int sfn, dfn, isbase;

	if(ctx->blend_isready)
		return(0);
	ctx->blend_isready=1;

	Blend=TKRA_Blend_Zero_Zero;
	sfn=ctx->blend_sfunc;
	dfn=ctx->blend_dfunc;
	isbase=0;
	switch(sfn)
	{
	case TKRA_BLEND_ZERO:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_Zero_Zero;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_Zero_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_Zero_SrcColor;
			isbase=4;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_Zero_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_Zero_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_Zero_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_Zero_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_Zero_OneMinusSrcAlpha;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_Zero_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_Zero_OneMinusDstAlpha;
			break;
		}
		break;
	case TKRA_BLEND_ONE:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_One_Zero;
			isbase=1;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_One_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_One_SrcColor;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_One_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_One_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_One_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_One_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_One_OneMinusSrcAlpha;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_One_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_One_OneMinusDstAlpha;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_BLEND_SRC_COLOR:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_SrcColor_Zero;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_SrcColor_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_SrcColor_SrcColor;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_SrcColor_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_SrcColor_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_SrcColor_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_SrcColor_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_SrcColor_OneMinusSrcAlpha;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_SrcColor_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_SrcColor_OneMinusDstAlpha;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_BLEND_SRC_ALPHA:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_SrcAlpha_Zero;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_SrcAlpha_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_SrcAlpha_SrcColor;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_SrcAlpha_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_SrcAlpha_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_SrcAlpha_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_SrcAlpha_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_SrcAlpha_OneMinusSrcAlpha;
			isbase=3;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_SrcAlpha_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_SrcAlpha_OneMinusDstAlpha;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_OneMinusSrcColor_Zero;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_OneMinusSrcColor_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_OneMinusSrcColor_SrcColor;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcColor_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_OneMinusSrcColor_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcColor_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_OneMinusSrcColor_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcColor_OneMinusSrcAlpha;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_OneMinusSrcColor_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcColor_OneMinusDstAlpha;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_OneMinusSrcAlpha_Zero;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_OneMinusSrcAlpha_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_OneMinusSrcAlpha_SrcColor;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcAlpha_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_OneMinusSrcAlpha_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcAlpha_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_OneMinusSrcAlpha_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcAlpha_OneMinusSrcAlpha;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_OneMinusSrcAlpha_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_OneMinusSrcAlpha_OneMinusDstAlpha;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_BLEND_DST_COLOR:
		switch(dfn)
		{
		case TKRA_BLEND_ZERO:
			Blend=TKRA_Blend_DstColor_Zero;
			isbase=4;
			break;
		case TKRA_BLEND_ONE:
			Blend=TKRA_Blend_DstColor_One;
			break;

		case TKRA_BLEND_SRC_COLOR:
			Blend=TKRA_Blend_DstColor_SrcColor;
			break;
		case TKRA_BLEND_SRC_ALPHA:
			Blend=TKRA_Blend_DstColor_SrcAlpha;
			break;
		case TKRA_BLEND_DST_COLOR:
			Blend=TKRA_Blend_DstColor_DstColor;
			break;
		case TKRA_BLEND_DST_ALPHA:
			Blend=TKRA_Blend_DstColor_DstAlpha;
			break;

		case TKRA_BLEND_ONE_MINUS_SRC_COLOR:
			Blend=TKRA_Blend_DstColor_OneMinusSrcColor;
			break;
		case TKRA_BLEND_ONE_MINUS_SRC_ALPHA:
			Blend=TKRA_Blend_DstColor_OneMinusSrcAlpha;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_COLOR:
			Blend=TKRA_Blend_DstColor_OneMinusDstColor;
			break;
		case TKRA_BLEND_ONE_MINUS_DST_ALPHA:
			Blend=TKRA_Blend_DstColor_OneMinusDstAlpha;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	default:
		__debugbreak();
		break;
	}
	
	ctx->bfn_flag=0;
	if((isbase&4) && (ctx->stateflag1&TKRA_STFL1_BLEND))
//	if(0)
	{
		ctx->bfn_flag|=TKRA_TRFL_DOLMAP;
	}else
		if((isbase&1) || !(ctx->stateflag1&TKRA_STFL1_BLEND))
	{
		if(!(ctx->stateflag1&TKRA_STFL1_BLEND))
			ctx->bfn_flag|=TKRA_TRFL_NOALPHA;
		if(isbase==1)
			ctx->bfn_flag|=TKRA_TRFL_NOALPHA;

		if(!isbase)
			Blend=TKRA_Blend_SrcAlpha_OneMinusSrcAlpha;
	}else
	{
		ctx->bfn_flag|=TKRA_TRFL_DOBLEND;
	}
	
	if(ctx->stateflag1&TKRA_STFL1_NODEPTHWRITE)
		ctx->bfn_flag|=TKRA_TRFL_NOZWRITE;

	if(ctx->stateflag1&TKRA_STFL1_TEX_REPLACE)
		ctx->bfn_flag|=TKRA_TRFL_NOCMOD;
	
	ctx->Blend=Blend;

	ctx->span_trifl=-1;
	ctx->span_tex_cur=NULL;

	TKRA_SetupDrawZaTest(ctx);
	return(0);
}

#endif