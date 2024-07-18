int TKRA_ZaTest_BasicZalAal(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	return(1);
}

int TKRA_ZaTest_BasicZalAle(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	return(1);
}

int TKRA_ZaTest_BasicZalAge(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	return(1);
}


int TKRA_ZaTest_BasicZleAal(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	return(zsrc<=ztgt);
}

int TKRA_ZaTest_BasicZleAle(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	return(zsrc<=ztgt);
}

int TKRA_ZaTest_BasicZleAge(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	return(zsrc<=ztgt);
}



int TKRA_ZaTest_BasicZgeAal(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	return(zsrc>=ztgt);
}

int TKRA_ZaTest_BasicZgeAle(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	return(zsrc>=ztgt);
}

int TKRA_ZaTest_BasicZgeAge(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	return(zsrc>=ztgt);
}


int TKRA_ZaTest_BasicZeqAal(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	return(zsrc==ztgt);
}

int TKRA_ZaTest_BasicZeqAle(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	return(zsrc==ztgt);
}

int TKRA_ZaTest_BasicZeqAge(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	return(zsrc==ztgt);
}

#if 1
int TKRA_ZaTest_BasicZalAalNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	*zmod=ztgt;
	return(1);
}

int TKRA_ZaTest_BasicZalAleNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	*zmod=ztgt;
	if((csrc>>48)>(cref>>48))
		return(0);
	return(1);
}

int TKRA_ZaTest_BasicZalAgeNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	*zmod=ztgt;
	if((csrc>>48)<(cref>>48))
		return(0);
	return(1);
}

int TKRA_ZaTest_BasicZleAalNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	*zmod=ztgt;
	return(zsrc<=ztgt);
}

int TKRA_ZaTest_BasicZleAleNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	*zmod=ztgt;
	return(zsrc<=ztgt);
}

int TKRA_ZaTest_BasicZleAgeNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	*zmod=ztgt;
//	*cmod=0xFFFFFFFF0000FFFFULL;
	return(zsrc<=ztgt);
}

int TKRA_ZaTest_BasicZgeAalNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	return(zsrc>=ztgt);
}

int TKRA_ZaTest_BasicZgeAleNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	*zmod=ztgt;
	return(zsrc>=ztgt);
}

int TKRA_ZaTest_BasicZgeAgeNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	*zmod=ztgt;
	return(zsrc>=ztgt);
}


int TKRA_ZaTest_BasicZeqAalNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	return(zsrc==ztgt);
}

int TKRA_ZaTest_BasicZeqAleNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)>(cref>>48))
		return(0);
	*zmod=ztgt;
	return(zsrc==ztgt);
}

int TKRA_ZaTest_BasicZeqAgeNz(
	s32 zsrc, s32 ztgt, s32 zref, s32 *zmod,
	u64 csrc, u64 ctgt, u64 cref, u64 *cmod)
{
	if((csrc>>48)<(cref>>48))
		return(0);
	*zmod=ztgt;
	return(zsrc==ztgt);
}
#endif

int TKRA_SetupDrawZaTest(TKRA_Context *ctx)
{
	TKRA_SvContext *sctx;
	int zfn, afn, aref;
	int isbasic;
	
	sctx=ctx->svctx;
	
	zfn=ctx->zat_zfunc;
	afn=ctx->zat_alfunc;
	
	if(!(ctx->stateflag1&TKRA_STFL1_ALPHATEST))
		afn=TKRA_ZAT_AL;
	if(!(ctx->stateflag1&TKRA_STFL1_DEPTHTEST))
		zfn=TKRA_ZAT_AL;

	if(ctx->stateflag1&TKRA_STFL1_NODEPTHWRITE)
		zfn|=TKRA_ZAT_NZ;

	aref=(ctx->zat_cref>>24)&255;

	isbasic=0;
	switch(zfn)
	{
	case TKRA_ZAT_AL:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZalAal;
			isbasic=1;
			if(	(ctx->stateflag1&TKRA_STFL1_ALPHATEST) ||
				(ctx->stateflag1&TKRA_STFL1_DEPTHTEST))
					isbasic=0;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZalAle;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZalAge;
			isbasic=1;
			if(ctx->stateflag1&TKRA_STFL1_DEPTHTEST)
					isbasic=0;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_ZAT_LT:
	case TKRA_ZAT_LE:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZleAal;
			isbasic=1;
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				isbasic=0;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZleAle;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZleAge;
			if((aref>0x60) && (aref<0xA0))
				{ isbasic=1; }
			else
				{ isbasic=0; }
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_ZAT_GT:
	case TKRA_ZAT_GE:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZgeAal;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZgeAle;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZgeAge;
			break;
		default:
			__debugbreak();
			break;
		}
		break;


	case TKRA_ZAT_AL_NZ:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZalAalNz;
			isbasic=1;
			if(	(ctx->stateflag1&TKRA_STFL1_ALPHATEST) ||
				(ctx->stateflag1&TKRA_STFL1_DEPTHTEST))
					isbasic=0;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZalAleNz;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZalAgeNz;
			if((aref>0x60) && (aref<0xA0))
				{ isbasic=1; }
			else
				{ isbasic=0; }
			if(ctx->stateflag1&TKRA_STFL1_DEPTHTEST)
				isbasic=0;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_ZAT_LT_NZ:
	case TKRA_ZAT_LE_NZ:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZleAalNz;
			isbasic=1;
			if(ctx->stateflag1&TKRA_STFL1_ALPHATEST)
				isbasic=0;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZleAleNz;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZleAgeNz;
			if((aref>0x60) && (aref<0xA0))
				{ isbasic=1; }
			else
				{ isbasic=0; }
			if(sctx->bfn_flag&(TKRA_TRFL_DOLMAP|TKRA_TRFL_DOBLEND))
				isbasic=0;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_ZAT_GT_NZ:
	case TKRA_ZAT_GE_NZ:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZgeAalNz;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZgeAleNz;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZgeAgeNz;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_ZAT_EQ:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZeqAal;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZeqAle;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZeqAge;
			break;
		default:
			__debugbreak();
			break;
		}
		break;

	case TKRA_ZAT_EQ_NZ:
		switch(afn)
		{
		case TKRA_ZAT_AL:
			sctx->ZaTest=TKRA_ZaTest_BasicZeqAalNz;
			break;
		case TKRA_ZAT_LT:
		case TKRA_ZAT_LE:
			sctx->ZaTest=TKRA_ZaTest_BasicZeqAleNz;
			break;
		case TKRA_ZAT_GT:
		case TKRA_ZAT_GE:
			sctx->ZaTest=TKRA_ZaTest_BasicZeqAgeNz;
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

	if(!isbasic)
	{
//		sctx->bfn_flag|=TKRA_TRFL_DOBLEND;
		sctx->bfn_flag|=TKRA_TRFL_DOZABLEND;
	}
	return(0);
}
