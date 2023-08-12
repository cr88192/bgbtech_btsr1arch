double TKUCC_UnpackBinary32(u32 val)
{
	float f;
	memcpy(&f, &val, 4);
	return(f);
}

double TKUCC_UnpackBinary64(u64 val)
{
	double f;
	memcpy(&f, &val, 8);
	return(f);
}

u32 TKUCC_PackBinary32(double val)
{
	u32 tv;
	float f;
	f=val;
	memcpy(&tv, &f, 4);
	return(tv);
}

u64 TKUCC_PackBinary64(double val)
{
	u64 tv;
	double f;
	f=val;
	memcpy(&tv, &f, 8);
	return(tv);
}

u32 TKUCC_RepackBinary16to32(u16 val)
{
	u32 tv;
	int ex;

	ex=(val>>10)&31;
	if((ex>0) && (ex<31))
		{ ex=ex+112; }
	else
		{ ex=ex?255:0; }
	tv=((val&0x8000)<<16)|(ex<<23)|((val&1023)<<13);
	return(tv);
}

u16 TKUCC_RepackBinary32to16(u32 val)
{
	u32 tv;
	int ex;
	
	ex=(val>>23)&255;
	ex-=112;
	if(ex<=0)
		return(0);
	if(ex>=31)
		return(0x7C00|((val>>16)&0x8000));
	tv=((val>>16)&0x8000)|(ex<<10)|((val>>13)&1023);
	return(tv);
}

double TKUCC_UnpackBinary16(u16 val)
{
	u32 tv;
	tv=TKUCC_RepackBinary16to32(val);
	return(TKUCC_UnpackBinary32(tv));
}

u16 TKUCC_PackBinary16(double val)
{
	u32 tv;
	u16 tv1;
	tv=TKUCC_PackBinary32(val);
	tv1=TKUCC_RepackBinary32to16(tv);
	return(tv1);
}

u64 TKUCC_GetValueForLongtabIndex(TKUCC_MainContext *ctx, int idx)
{
	u64 *p;

	if(idx>>16)
		return(0);
	p=ctx->longtab_v[idx>>8];
	if(!p)
		return(0);
	return(p[idx&255]);
}

int TKUCC_LookupLongtabIndexForValue(TKUCC_MainContext *ctx, u64 val)
{
	u64 *pv;
	u16 *pc;
	int i, h;

	if(ctx->longtab_rov<=0)
	{
		for(i=0; i<256; i++)
			ctx->longtab_h[i]=0;
		ctx->longtab_rov=1;
	}

	h=(val*0xFAD3E395C412EBULL)>>56;
	i=ctx->longtab_h[h];
	while(i>0)
	{
		pv=ctx->longtab_v[i>>8];
		pc=ctx->longtab_c[i>>8];
		if(pv[i&255]==val)
			return(i);
		i=pc[i&255];
	}

	return(0);
}

int TKUCC_GetLongtabIndexForValue(TKUCC_MainContext *ctx, u64 val)
{
	u64 *pv;
	u16 *pc;
	int i, h;

	if(ctx->longtab_rov<=0)
	{
		for(i=0; i<256; i++)
			ctx->longtab_h[i]=0;
		ctx->longtab_rov=1;
	}

	h=(val*0xFAD3E395C412EBULL)>>56;
	i=ctx->longtab_h[h];
	while(i>0)
	{
		pv=ctx->longtab_v[i>>8];
		pc=ctx->longtab_c[i>>8];
		if(pv[i&255]==val)
			return(i);
		i=pc[i&255];
	}
	
	i=ctx->longtab_rov++;
	pv=ctx->longtab_v[i>>8];
	pc=ctx->longtab_c[i>>8];

	if(!pv)
	{
		pv=tkucc_malloc(256*sizeof(u64));
		pc=tkucc_malloc(256*sizeof(u16));
		ctx->longtab_v[i>>8]=pv;
		ctx->longtab_c[i>>8]=pc;
	}
	
	pv[i&255]=val;
	pc[i&255]=ctx->longtab_h[h];
	ctx->longtab_h[h]=i;

	return(i);
}


int TKUCC_VregCheckIdentP(TKUCC_MainContext *ctx,
	tkucc_valreg val1, tkucc_valreg val2)
{
	if(val1.val==val2.val)
		return(1);

	if((val1.val&TKUCC_VALREG_VTYPE_MSK) != (val2.val&TKUCC_VALREG_VTYPE_MSK))
		return(0);

	if((val1.val&TKUCC_VALREG_VTYPE_MSK)==TKUCC_VALREG_VTYPE_LOCAL)
	{
		if((val1.val&0xFFF) != (val2.val&0xFFF))
			return(0);
		return(1);
	}

	if((val1.val&TKUCC_VALREG_VTYPE_MSK)==TKUCC_VALREG_VTYPE_GLOBAL)
	{
		if((val1.val&0xFFFFFF) != (val2.val&0xFFFFFF))
			return(0);
		return(1);
	}

	return(0);
}

int TKUCC_VregIsLocalP(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	if((val.val&TKUCC_VALREG_VTYPE_MSK)==TKUCC_VALREG_VTYPE_LOCAL)
		return(1);
	return(0);
}

int TKUCC_VregIsGlobalP(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	if((val.val&TKUCC_VALREG_VTYPE_MSK)==TKUCC_VALREG_VTYPE_GLOBAL)
		return(1);
	return(0);
}

int TKUCC_VregIsSmallitP(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	if((val.val&TKUCC_VALREG_VTYPE_MSK)==TKUCC_VALREG_VTYPE_SMALLIT)
		return(1);
	return(0);
}

int TKUCC_VregIsLongitP(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	if((val.val&TKUCC_VALREG_VTYPE_MSK)==TKUCC_VALREG_VTYPE_SMALLIT)
		return(1);
	return(0);
}

int TKUCC_VregIsIntegerP(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	tkucc_valtype ty;
	u64 vrty;
	
	vrty=(val.val&TKUCC_VALREG_VTYPE_MSK);

	if(	(vrty==TKUCC_VALREG_VTYPE_SMALLIT)	||
		(vrty==TKUCC_VALREG_VTYPE_LONGIT)	)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);
		if(TKUCC_TypeSmallLongP(ctx, ty))
			return(1);
		return(0);
	}

	return(0);
}

int TKUCC_VregIsRealP(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	tkucc_valtype ty;
	u64 vrty;
	
	vrty=(val.val&TKUCC_VALREG_VTYPE_MSK);

	if(	(vrty==TKUCC_VALREG_VTYPE_SMALLIT)	||
		(vrty==TKUCC_VALREG_VTYPE_LONGIT)	)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);
		if(TKUCC_TypeSmallRealP(ctx, ty))
			return(1);
		return(0);
	}

	return(0);
}

tkucc_valtype TKUCC_VregGetType(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	tkucc_valtype ty;
	u64 vrty;
	
	vrty=(val.val&TKUCC_VALREG_VTYPE_MSK);
	if(	(vrty==TKUCC_VALREG_VTYPE_LOCAL)	||
		(vrty==TKUCC_VALREG_VTYPE_GLOBAL)	)
	{
		ty.val=(u32)(val.val>>24);
		return(ty);
	}

	if(	(vrty==TKUCC_VALREG_VTYPE_SMALLIT)	||
		(vrty==TKUCC_VALREG_VTYPE_LONGIT)	)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);
		return(ty);
	}

	ty.val=TKUCC_VALTYPE_TY_UNDEF;
	return(ty);
}

s64 TKUCC_VregGetValueInt(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	tkucc_valtype ty;
	u64 vrty;
	s64 tv;
	
	vrty=(val.val&TKUCC_VALREG_VTYPE_MSK);

	if(vrty==TKUCC_VALREG_VTYPE_SMALLIT)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);
		if(TKUCC_TypeSmallLongP(ctx, ty))
		{
			if(TKUCC_TypeUnsignedP(ctx, ty))
				{ tv=(u32)(val.val); }
			else
				{ tv=(s32)(val.val); }
			return(tv);
		}
		return(0);
	}

	if(vrty==TKUCC_VALREG_VTYPE_LONGIT)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);

		if(TKUCC_TypeSmallLongP(ctx, ty))
		{
			tv=TKUCC_GetValueForLongtabIndex(ctx, val.val&0xFFFF);
			return(tv);
		}
		return(0);
	}
	
	return(0);
}

double TKUCC_VregGetValueReal(TKUCC_MainContext *ctx,
	tkucc_valreg val)
{
	tkucc_valtype ty;
	double f;
	u64 vrty;
	s64 tv;
	
	vrty=(val.val&TKUCC_VALREG_VTYPE_MSK);

	if(vrty==TKUCC_VALREG_VTYPE_SMALLIT)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);
		if(TKUCC_TypeSmallLongP(ctx, ty))
		{
			if(TKUCC_TypeUnsignedP(ctx, ty))
				{ tv=(u32)(val.val); }
			else
				{ tv=(s32)(val.val); }
			return(tv);
		}
		if(TKUCC_TypeSmallRealP(ctx, ty))
		{
			f=TKUCC_UnpackBinary32((u32)val.val);
			return(f);
		}
		return(0);
	}

	if(vrty==TKUCC_VALREG_VTYPE_LONGIT)
	{
		ty=TKUCC_TypeFromShort24Type(ctx, (val.val>>32)&0xFFFFFF);

		if(TKUCC_TypeSmallLongP(ctx, ty))
		{
			tv=TKUCC_GetValueForLongtabIndex(ctx, val.val&0xFFFF);
			return(tv);
		}
		if(TKUCC_TypeSmallRealP(ctx, ty))
		{
			tv=TKUCC_GetValueForLongtabIndex(ctx, val.val&0xFFFF);
			f=TKUCC_UnpackBinary64(tv);
			return(f);
		}
		return(0);
	}
	
	return(0);
}
