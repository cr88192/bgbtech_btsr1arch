#include <bgbccc.h>

bool BGBCC_CCXL_TypeSmallTypeP(
	BGBCC_TransState *ctx, ccxl_type dty, ccxl_type sty)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeSmallFloatP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallFloatP(ctx, sty))
		{
			return(true);
		}

		if(BGBCC_CCXL_TypeSmallDoubleP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeSmallDoubleP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallFloatP(ctx, sty))
		{
			return(true);
		}

		if(BGBCC_CCXL_TypeSmallDoubleP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}
	
	if(dty.val==sty.val)
		return(true);
	
	return(false);
}

/* Check if source type can be promoted to the destination type without any need for a conv step. */
bool BGBCC_CCXL_TypeImplicitPromoteP(
	BGBCC_TransState *ctx, ccxl_type dty, ccxl_type sty)
{
	if(dty.val==CCXL_TY_I)
	{
		if(sty.val==CCXL_TY_SB)
			return(true);
		if(sty.val==CCXL_TY_SS)
			return(true);
		if(sty.val==CCXL_TY_I)
			return(true);
		if(BGBCC_CCXL_TypeBitIntMaxP(ctx, sty, 31))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_UI)
	{
		if(sty.val==CCXL_TY_UB)
			return(true);
		if(sty.val==CCXL_TY_US)
			return(true);
		if(sty.val==CCXL_TY_UI)
			return(true);
		if(BGBCC_CCXL_TypeUBitIntMaxP(ctx, sty, 32))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_NL)
	{
		if(sty.val==CCXL_TY_SB)
			return(true);
		if(sty.val==CCXL_TY_SS)
			return(true);
		if(sty.val==CCXL_TY_I)
			return(true);
		if(sty.val==CCXL_TY_NL)
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_UNL)
	{
		if(sty.val==CCXL_TY_UB)
			return(true);
		if(sty.val==CCXL_TY_US)
			return(true);
		if(sty.val==CCXL_TY_UI)
			return(true);
		if(sty.val==CCXL_TY_UNL)
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_SB)
	{
		if(sty.val==CCXL_TY_SB)
			return(true);
		if(BGBCC_CCXL_TypeBitIntMaxP(ctx, sty, 7))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_UB)
	{
		if(sty.val==CCXL_TY_UB)
			return(true);
		if(BGBCC_CCXL_TypeUBitIntMaxP(ctx, sty, 8))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_SS)
	{
		if(sty.val==CCXL_TY_SB)
			return(true);
		if(sty.val==CCXL_TY_SS)
			return(true);
		if(BGBCC_CCXL_TypeBitIntMaxP(ctx, sty, 15))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_US)
	{
		if(sty.val==CCXL_TY_UB)
			return(true);
		if(sty.val==CCXL_TY_US)
			return(true);
		if(BGBCC_CCXL_TypeUBitIntMaxP(ctx, sty, 16))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_L)
	{
		if(sty.val==CCXL_TY_SB)
			return(true);
		if(sty.val==CCXL_TY_SS)
			return(true);
		if(sty.val==CCXL_TY_I)
			return(true);
		if(sty.val==CCXL_TY_NL)
			return(true);
		if(sty.val==CCXL_TY_L)
			return(true);
		if(BGBCC_CCXL_TypeBitIntMaxP(ctx, sty, 63))
			return(true);
		return(false);
	}

	if(dty.val==CCXL_TY_UL)
	{
		if(sty.val==CCXL_TY_UB)
			return(true);
		if(sty.val==CCXL_TY_US)
			return(true);
		if(sty.val==CCXL_TY_UI)
			return(true);
		if(sty.val==CCXL_TY_UNL)
			return(true);
		if(sty.val==CCXL_TY_UL)
			return(true);
		if(BGBCC_CCXL_TypeUBitIntMaxP(ctx, sty, 64))
			return(true);
		return(false);
	}

	return(false);
}

bool BGBCC_CCXL_TypeSmallIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bty;

//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

#if 1
	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(bty==CCXL_TY_I)		return(true);
	if(bty==CCXL_TY_UI)		return(true);
	if(bty==CCXL_TY_SB)		return(true);
	if(bty==CCXL_TY_UB)		return(true);
	if(bty==CCXL_TY_SS)		return(true);
	if(bty==CCXL_TY_US)		return(true);
	if(ctx->arch_sizeof_long==4)
	{
		if(bty==CCXL_TY_NL)
			return(true);
		if(bty==CCXL_TY_UNL)
			return(true);
	}
#endif

#if 0
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(ctx->arch_sizeof_long==4)
	{
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
			return(true);
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
			return(true);
	}
#endif

	if(BGBCC_CCXL_TypeBitIntMaxP(ctx, ty, 32))
		return(true);

	return(false);
}


bool BGBCC_CCXL_TypeSmallShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bty;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(bty==CCXL_TY_SB)		return(true);
	if(bty==CCXL_TY_UB)		return(true);
	if(bty==CCXL_TY_SS)		return(true);
	if(bty==CCXL_TY_US)		return(true);

	if(BGBCC_CCXL_TypeBitIntMaxP(ctx, ty, 16))
		return(true);

	return(false);
}


bool BGBCC_CCXL_TypeSmallSIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bty;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

#if 1
	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(bty==CCXL_TY_I)		return(true);
	if(bty==CCXL_TY_SB)		return(true);
	if(bty==CCXL_TY_UB)		return(true);
	if(bty==CCXL_TY_SS)		return(true);
	if(bty==CCXL_TY_US)		return(true);
	if(ctx->arch_sizeof_long==4)
	{
		if(bty==CCXL_TY_NL)
			return(true);
	}
#endif

	if(BGBCC_CCXL_TypeBitIntMaxP(ctx, ty, 31))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallUIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bty;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

#if 1
	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
//	if(bty==CCXL_TY_I)		return(true);
//	if(bty==CCXL_TY_SB)		return(true);
	if(bty==CCXL_TY_UB)		return(true);
//	if(bty==CCXL_TY_SS)		return(true);
	if(bty==CCXL_TY_US)		return(true);
	if(bty==CCXL_TY_UI)		return(true);
	if(ctx->arch_sizeof_long==4)
	{
		if(bty==CCXL_TY_UNL)
			return(true);
	}
#endif

	if(BGBCC_CCXL_TypeUBitIntMaxP(ctx, ty, 32))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SI48)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI48)
		return(true);

	if(BGBCC_CCXL_TypeBitIntMaxP(ctx, ty, 64))
		return(true);

	return(false);
}

#if 0
bool BGBCC_CCXL_TypeSgInt128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	return(false);
}
#endif

bool BGBCC_CCXL_TypeSmallInt128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(true);		
	if(BGBCC_CCXL_TypeSgInt128P(ctx, ty))
		return(true);

	if(BGBCC_CCXL_TypeBitIntMaxP(ctx, ty, 128))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeBitIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SBITINT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BITINT;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UBITINT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BITINT;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_OBITINT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BITINT;
		return(true);
	}

//	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
//		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeUBitIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UBITINT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BITINT;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeSBitIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SBITINT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BITINT;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeSmallBitIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallInt128P(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeBitIntP(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeBitIntMaxP(
	BGBCC_TransState *ctx, ccxl_type ty, int max)
{
	int sz;
	if(BGBCC_CCXL_TypeBitIntP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		if(sz<=max)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_TypeUBitIntMaxP(
	BGBCC_TransState *ctx, ccxl_type ty, int max)
{
	int sz;
	if(BGBCC_CCXL_TypeUBitIntP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		if(sz<=max)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_TypeSBitIntMaxP(
	BGBCC_TransState *ctx, ccxl_type ty, int max)
{
	int sz;
	if(BGBCC_CCXL_TypeSBitIntP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		if(sz<=max)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bty;

//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

#if 1
	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(bty==CCXL_TY_UI)		return(true);
	if(bty==CCXL_TY_US)		return(true);
	if(bty==CCXL_TY_UB)		return(true);

	if(bty==CCXL_TY_UL)		return(true);
	if(bty==CCXL_TY_UNL)	return(true);
	if(bty==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}

	if(bty==CCXL_TY_UBITINT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BITINT;
		return(true);
	}

	if(bty==CCXL_TY_BCD64)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BCD;
		return(true);
	}
	if(bty==CCXL_TY_BCD128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BCD;
		return(true);
	}

	if((bty==CCXL_TY_BITFIELD_UB) ||
		(bty==CCXL_TY_BITFIELD_US) ||
		(bty==CCXL_TY_BITFIELD_UI) ||
		(bty==CCXL_TY_BITFIELD_UL))
	{
		return(true);
	}

	if(bty==CCXL_TY_UI48)		return(true);

#endif

#if 0
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
#endif

	return(false);
}

bool BGBCC_CCXL_TypeSignedP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(!BGBCC_CCXL_TypeSmallInt128P(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypeUnsignedP(ctx, ty))
		return(false);
	return(true);
}

bool BGBCC_CCXL_TypeIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);

	if((BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL) &&
		(ctx->arch_sizeof_long==8))
			return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSgIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgNLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgInt128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeFloatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeDoubleP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeFloat128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeFloat16P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeHFloat16P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeBFloat16P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeFloat48P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F48)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeFloat8P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8S)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8U)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeRealP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		return(true);
	}

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8S)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8U)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F48)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeBaseRealP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);

	return(false);
}


bool BGBCC_CCXL_TypeBcd64P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BCD64)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BCD;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeBcd128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BCD128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BCD;
		return(true);
	}

	return(false);
}

bool BGBCC_CCXL_TypeBcdP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BCD64)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BCD;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BCD128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_BCD;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeVariantP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_VARIANT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_VARIANT;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeQuadVariantP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(!BGBCC_CCXL_TypeVariantP(ctx, ty))
		return(false);
	if(ctx->arch_sizeof_ptr==16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVarObjP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bt;

//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	bt=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if((bt==CCXL_TY_VARIANT) || (bt==CCXL_TY_VAROBJECT))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_VARIANT;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeVarRefP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int bt;

//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_TypeRefArrayP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeIsRefObjectP(ctx, ty))
		return(true);

	bt=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(	(bt==CCXL_TY_VARIANT) ||
		(bt==CCXL_TY_VAROBJECT) ||
		(bt==CCXL_TY_VARSTRING) ||
		(bt==CCXL_TY_VARWSTRING) ||
		(bt==CCXL_TY_VARUSTRING) ||
		(bt==CCXL_TY_FIXNUM) ||
		(bt==CCXL_TY_FLONUM) ||
		(bt==CCXL_TY_BIGINT))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_VARIANT;
		return(true);
	}

	return(false);
}

bool BGBCC_CCXL_TypeQuadVarObjP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(!BGBCC_CCXL_TypeVarObjP(ctx, ty))
		return(false);
	if(ctx->arch_sizeof_ptr==16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeQuadVarRefP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(!BGBCC_CCXL_TypeVarRefP(ctx, ty))
		return(false);
	if(ctx->arch_sizeof_ptr==16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeCStringP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	return(ty.val==CCXL_VTY_PCHAR);
}

bool BGBCC_CCXL_TypeCWStringP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	return(ty.val==CCXL_VTY_PWCHAR);
}

bool BGBCC_CCXL_TypeSmallFloatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}

	if((BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16) ||
		(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}

	if(	(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8S) ||
		(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8U))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallDoubleP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}

	if(	(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8S) ||
		(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8U))
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F48)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		return(true);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallFloat128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		return(true);
	}

	if(BGBCC_CCXL_TypeSmallDoubleP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeSmallInt128P(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallVariantP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	return(true);
}

bool BGBCC_CCXL_TypeBaseILFD_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8S)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8U)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SI48)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI48)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F48)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeBaseSmallILFD_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_BF16)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8S)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FP8U)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SI48)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI48)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F48)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeBaseSmallILFDP_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	return(BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, ty));
}

bool BGBCC_CCXL_TypeBaseSmallIL_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
//		return(false);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SI48)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI48)
		return(true);

	return(false);
}

int BGBCC_CCXL_GetTypeBaseType(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
		{ return(ty.val&CCXL_TY_BASEMASK); }

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
		{ return(ty.val&CCXL_TYB2_BASEMASK); }

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
		{ return(ty.val&CCXL_TYB3_BASEMASK); }

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		i=ty.val&CCXL_TYOVF_IDXMASK;
		if((i>ctx->n_tyovf) || !ctx->tyovf[i])
			{ BGBCC_DBGBREAK }
		return(ctx->tyovf[i]->base);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(-1);
}

bool BGBCC_CCXL_TypePointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_P)
			return(true);
		if(ty.val&CCXL_TY_PTRMASK)
		{
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)<CCXL_TY_PN4_REF)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_PTRREF)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0P1)
				return(true);

			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
			{
				if(((ty.val&CCXL_TY_PCLSMASK)>>CCXL_TY_PCLSSHL)!=0)
					return(true);
				return(false);
			}

//			return(true);
		}
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if((ty.val&CCXL_TYB2_BASEMASK)==CCXL_TY_P)
			return(true);
//		if(ty.val&CCXL_TYB2_PTRMASK)
		if((ty.val&CCXL_TYB2_PTRMASK)==CCXL_TYB2_PTRIDX1)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if((ty.val&CCXL_TYB3_BASEMASK)==CCXL_TY_P)
			return(true);
//		if(ty.val&CCXL_TYB3_PTRMASK)
		if((ty.val&CCXL_TYB3_PTRMASK)==CCXL_TYB3_PTRIDX1)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->pn!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeArrayP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int pn;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		pn=((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL);

		if(	(pn==CCXL_TY_PN4_Q1) ||
			(pn==CCXL_TY_PN4_Q2) ||
			(pn==CCXL_TY_PN4_Q3) ||
			(pn==CCXL_TY_PN4_Q1P1)	)
		{
			return(false);
		}

		if(ty.val&CCXL_TY_ARRMASK)
		{
			if(pn==0)
			{
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_SBITINT)
					return(false);
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_UBITINT)
					return(false);

				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_BCDBIG_P0)
					return(false);
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_BCDBIG_P4)
					return(false);

				if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
					((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
				{
					return(false);
				}
			}

			return(true);
		}
//		if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
		if(pn==CCXL_TY_PN4_A0B)
		{
			if(((ty.val&CCXL_TY_PCLSMASK)>>CCXL_TY_PCLSSHL)!=0)
				return(false);
			return(true);
		}
//		if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0P1)
		if(pn==CCXL_TY_PN4_A0P1)
			return(true);
		
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if(ty.val&CCXL_TYB2_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if(ty.val&CCXL_TYB3_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->an!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeArrayOrPointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
#if 1
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(ty.val&CCXL_TY_ARRMASK)
		{
			if(!(ty.val&CCXL_TY_PTRMASK))
			{
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_SBITINT)
					return(false);
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_UBITINT)
					return(false);
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_BCDBIG_P0)
					return(false);
				if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_BCDBIG_P4)
					return(false);

				if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
					((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
				{
					return(false);
				}
			}

			return(true);
		}

		if(ty.val&CCXL_TY_PTRMASK)
		{
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)<CCXL_TY_PN4_REF)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_PTRREF)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0P1)
				return(true);

			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
				return(true);
//			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0P1)
//				return(true);

//			return(true);
		}
//		return(false);

//		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_P)
		if((ty.val&CCXL_TY_BASEMASK)==CCXL_TY_P)
			return(true);
		
		return(false);
	}
#endif

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeRefArrayP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
//		if(ty.val&CCXL_TY_ARRMASK)
//			return(true);
		if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1)
			return(true);
		if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q2)
			return(true);
		if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q3)
			return(true);
		if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1P1)
			return(true);
		
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->qn!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeFatArrayP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FATP_AREF)
		return(true);

#if 0
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_P)
			return(true);
		if(ty.val&CCXL_TY_PTRMASK)
		{
//			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)<CCXL_TY_PN4_REF)
//				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q2)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q3)
				return(true);
			if(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1P1)
				return(true);
//			return(true);
		}
		return(false);
	}
#endif

	return(false);
}

bool BGBCC_CCXL_TypeSquareArrayP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
//		if(ty.val&CCXL_TY_ARRMASK)
//			return(true);
		if(
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1) ||
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q2) ||
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q3) ||
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1P1) )
		{
			i=((ty.val>>CCXL_TY_ARRSHL)&15);
			if((i>=1) && (i<=7))
				return(true);
			return(false);
		}
		
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->qn!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeSquareArrayPtrP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
//		if(ty.val&CCXL_TY_ARRMASK)
//			return(true);
		if(
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1) ||
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q2) ||
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q3) ||
			(((ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_Q1P1) )
		{
			i=((ty.val>>CCXL_TY_ARRSHL)&15);
			if((i>=9) && (i<=15))
				return(true);
			return(false);
		}
		
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->qn!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeBitFieldP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
			return(true);
		}

		return(false);
	}

	return(false);
}

bool BGBCC_CCXL_TypeBitFieldSmallIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int j;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
			j=BGBCC_CCXL_TypeBitFieldGetBits(ctx, ty);
			if(j<32)
				return(true);
			return(false);
		}

		return(false);
	}

	return(false);
}

bool BGBCC_CCXL_TypeBitFieldSmallLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
			return(true);
		}

		return(false);
	}

	return(false);
}

int BGBCC_CCXL_TypeBitFieldGetBits(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
			i=(ty.val>>CCXL_TY_ARRSHL)&63;
			return(i);
		}
		return(-1);
	}
	
	return(-1);
}

int BGBCC_CCXL_TypeBitFieldGetMaxBits(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i, j;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
			i=(ty.val&CCXL_TY_BASEMASK);	j=0;
			if(i==CCXL_TY_BITFIELD_SB)		j=8;
			if(i==CCXL_TY_BITFIELD_UB)		j=8;
			if(i==CCXL_TY_BITFIELD_SS)		j=16;
			if(i==CCXL_TY_BITFIELD_US)		j=16;
			if(i==CCXL_TY_BITFIELD_SI)		j=32;
			if(i==CCXL_TY_BITFIELD_UI)		j=32;
			if(i==CCXL_TY_BITFIELD_SL)		j=64;
			if(i==CCXL_TY_BITFIELD_UL)		j=64;
			return(j);
		}
		return(-1);
	}
	
	return(-1);
}

int BGBCC_CCXL_TypeBitFieldGetOffset(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
			i=(ty.val>>(CCXL_TY_ARRSHL+6))&63;
			return(i);
		}
		return(-1);
	}
	
	return(-1);
}

int BGBCC_CCXL_TypeBitFieldSetOffset(
	BGBCC_TransState *ctx, ccxl_type *rty, int ofs)
{
	ccxl_type ty;
	int i;

	ty=*rty;
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(	((ty.val&CCXL_TY_BASEMASK)>=CCXL_TY_BITFIELD_SB) &&
			((ty.val&CCXL_TY_BASEMASK)<=CCXL_TY_BITFIELD_UL) )
		{
//			i=(ty.val>>(CCXL_TY_ARRSHL+6))&63;
			ty.val&=~(63ULL<<(CCXL_TY_ARRSHL+6));
			ty.val|=(ofs<<(CCXL_TY_ARRSHL+6));
			*rty=ty;
			return(ofs);
		}
		return(-1);
	}
	
	return(-1);
}

bool BGBCC_CCXL_TypeMethodPointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
//	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FATP_AREF)
//		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FATP_VMTH)
		return(true);
	
	if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
	{
		i=BGBCC_CCXL_GetTypeBaseType(ctx, ty)-256;
		if(ctx->literals[i]->decl->flagsint&BGBCC_TYFL_VIRTUAL)
			return(true);
	}

	return(false);
}


bool BGBCC_CCXL_TypeFatPointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FATP)
		return(true);

//	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FATP_AREF)
	if(BGBCC_CCXL_TypeFatArrayP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_FATP_VMTH)
	if(BGBCC_CCXL_TypeMethodPointerP(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSignedByteP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedByteP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgByteP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSignedShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVoidP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVarArgsP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_VARARGS)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVaListP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(false);
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_VALIST)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeEqualP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(dty.val==sty.val);
}

bool BGBCC_CCXL_TypeBaseEqualP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	if(dty.val==sty.val)
		return(true);

	if(BGBCC_CCXL_TypeIntP(ctx, dty) && BGBCC_CCXL_TypeIntP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeLongP(ctx, dty) && BGBCC_CCXL_TypeLongP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeFloatP(ctx, dty) && BGBCC_CCXL_TypeFloatP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeDoubleP(ctx, dty) && BGBCC_CCXL_TypeDoubleP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, dty) &&
			BGBCC_CCXL_TypePointerP(ctx, sty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeBaseSmallEqualP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int i, j;

	if(dty.val==sty.val)
		return(true);

	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeLongP(ctx, dty) && BGBCC_CCXL_TypeLongP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeFloatP(ctx, dty) && BGBCC_CCXL_TypeFloatP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeDoubleP(ctx, dty) && BGBCC_CCXL_TypeDoubleP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, dty) &&
			BGBCC_CCXL_TypePointerP(ctx, sty))
		return(true);
	
	if(BGBCC_CCXL_TypeValueObjectP(ctx, dty) &&
		BGBCC_CCXL_TypeValueObjectP(ctx, sty))
	{
		i=BGBCC_CCXL_GetTypeBaseType(ctx, dty);
		j=BGBCC_CCXL_GetTypeBaseType(ctx, sty);
		
		if(i==j)
			return(true);
		return(false);
	}
	
	return(false);
}

bool BGBCC_CCXL_TypeOprBasicP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=16)
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_S)
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNDEF_I)
		return(false);

	return(true);
}

int BGBCC_CCXL_TypeObjectLiteralIndex(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)return(-1);
	return(i-256);
}

bool BGBCC_CCXL_TypeObjectP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i>=256)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeFunctionP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		if(obj && obj->littype==CCXL_LITID_FUNCTION)
			return(true);
		if(obj && obj->littype==CCXL_LITID_PROTOTYPE)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_TypeValueObjectP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type ty2;
	int sz;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	
	if(BGBCC_CCXL_TypeIsTypedefP(ctx, ty))
	{
		BGBCC_CCXL_TypeGetTypedefType(ctx, ty, &ty2);
		return(BGBCC_CCXL_TypeValueObjectP(ctx, ty2));
	}
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<16)
		return(false);
	
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		if(!obj)
			return(false);
		if(obj->littype==CCXL_LITID_STRUCT)
			return(true);
		if(obj->littype==CCXL_LITID_UNION)
			return(true);
//		if(obj->littype==CCXL_LITID_CLASS)
//			return(true);
		return(false);
	}

#if 1
	switch(i)
	{
	case CCXL_TY_VALIST:
		return(true);

	case CCXL_TY_SBITINT:
	case CCXL_TY_UBITINT:
	case CCXL_TY_OBITINT:
		sz=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		if(sz<=128)
			return(false);
		return(true);

	case CCXL_TY_BCDBIG_P0:
	case CCXL_TY_BCDBIG_P4:
		return(true);

#if 0
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
	case CCXL_TY_M128:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
	case CCXL_TY_VEC4SI:
	case CCXL_TY_VEC4UI:
		return(true);

	case CCXL_TY_M64:
	case CCXL_TY_VEC2F:
	case CCXL_TY_VEC4SW:
	case CCXL_TY_VEC4UW:
		return(true);
#endif

	}
#endif
	return(false);
}

bool BGBCC_CCXL_TypeIsRefObjectP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
	{
		if(i==CCXL_TY_VAROBJECT)
			return(true);
		return(false);
	}

	obj=ctx->literals[i-256];
	if(!obj)
		return(false);
	if(obj->littype==CCXL_LITID_CLASS)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeRefStringP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
	{
		if(i==CCXL_TY_VARSTRING)
			return(true);
		if(i==CCXL_TY_VARWSTRING)
			return(true);
		if(i==CCXL_TY_VARUSTRING)
			return(true);
		return(false);
	}

//	obj=ctx->literals[i-256];
//	if(!obj)
//		return(false);
//	if(obj->littype==CCXL_LITID_CLASS)
//		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeIsObjectP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

//	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
//		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<16)
		return(false);
	
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		if(!obj)
			return(false);
		if(obj->littype==CCXL_LITID_STRUCT)
			return(true);
		if(obj->littype==CCXL_LITID_UNION)
			return(true);
		if(obj->littype==CCXL_LITID_CLASS)
			return(true);
		return(false);
	}

#if 0
	switch(i)
	{
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:

	case CCXL_TY_M128:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
		return(true);
	}
#endif

	return(false);
}

bool BGBCC_CCXL_TypeVecAs64P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeVec64P(ctx, ty))
		return(1);

	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_VEC4_FP8S:
	case CCXL_TY_VEC4_FP8U:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeVec64P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_M64:
	case CCXL_TY_BCD64:

	case CCXL_TY_VEC2F:
	case CCXL_TY_VEC3FQ:
	case CCXL_TY_VEC4SW:
	case CCXL_TY_VEC4UW:
	case CCXL_TY_VEC4H:
	case CCXL_TY_VEC3H:
		return(true);

	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_FIMAG:
		return(true);

//	case CCXL_TY_VEC4_FP8S:
//	case CCXL_TY_VEC4_FP8U:
//		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeVec128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:

	case CCXL_TY_M128:
	case CCXL_TY_M128P:
	case CCXL_TY_BCD128:

	case CCXL_TY_VEC2D:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC3FX:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
	case CCXL_TY_VEC4SI:
	case CCXL_TY_VEC4UI:
		return(true);

	case CCXL_TY_DCOMPLEX:
	case CCXL_TY_DIMAG:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeVec256P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_GCOMPLEX:
	case CCXL_TY_GIMAG:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeVecP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_VEC2F:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC3FQ:
	case CCXL_TY_VEC3FX:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
		return(true);

	case CCXL_TY_VEC4SW:
	case CCXL_TY_VEC4UW:
	case CCXL_TY_VEC4SI:
	case CCXL_TY_VEC4UI:
		return(true);

	case CCXL_TY_VEC2D:
	case CCXL_TY_VEC4H:
	case CCXL_TY_VEC3H:
		return(true);

	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_DCOMPLEX:
	case CCXL_TY_GCOMPLEX:
	case CCXL_TY_HCOMPLEX:
	case CCXL_TY_FIMAG:
	case CCXL_TY_DIMAG:
	case CCXL_TY_GIMAG:
	case CCXL_TY_HIMAG:
		return(true);

	case CCXL_TY_BCD64:
	case CCXL_TY_BCD128:
		return(true);

	case CCXL_TY_VEC4_FP8S:
	case CCXL_TY_VEC4_FP8U:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeComplexP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_DCOMPLEX:
	case CCXL_TY_GCOMPLEX:
	case CCXL_TY_HCOMPLEX:
	case CCXL_TY_FIMAG:
	case CCXL_TY_DIMAG:
	case CCXL_TY_GIMAG:
	case CCXL_TY_HIMAG:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeSmallFComplexP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(ty.val==CCXL_TY_FCOMPLEX)
		return(true);
	if(ty.val==CCXL_TY_FIMAG)
		return(true);
	if(BGBCC_CCXL_TypeSmallFloatP(ctx, ty))
		return(true);
	return(false);

#if 0	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_DCOMPLEX:
		return(true);
	}
	return(false);
#endif
}

bool BGBCC_CCXL_TypeSmallDComplexP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(		(ty.val==CCXL_TY_DCOMPLEX)	||
			(ty.val==CCXL_TY_FCOMPLEX)	||
			(ty.val==CCXL_TY_FIMAG)		||
			(ty.val==CCXL_TY_DIMAG)		)
		return(true);
	if(BGBCC_CCXL_TypeSmallDoubleP(ctx, ty))
		return(true);
	return(false);

#if 0	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_DCOMPLEX:
		return(true);
	}
	return(false);
#endif
}

bool BGBCC_CCXL_TypeSmallGComplexP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(		(ty.val==CCXL_TY_DCOMPLEX)	||
			(ty.val==CCXL_TY_FCOMPLEX)	||
			(ty.val==CCXL_TY_GCOMPLEX)	||
			(ty.val==CCXL_TY_HCOMPLEX)	||
			(ty.val==CCXL_TY_FIMAG)		||
			(ty.val==CCXL_TY_DIMAG)		||
			(ty.val==CCXL_TY_GIMAG)		||
			(ty.val==CCXL_TY_HIMAG)		)
		return(true);
	if(BGBCC_CCXL_TypeSmallFloat128P(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeQuatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(ty.val==CCXL_TY_QUATF)
		return(true);
	return(false);

#if 0
	int i;

	i=ty.val;
	switch(i)
	{
	case CCXL_TY_QUATF:
//	case CCXL_TY_QUATD:
		return(true);
	}
	return(false);
#endif
}

bool BGBCC_CCXL_TypeSmallQuatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(ty.val==CCXL_TY_QUATF)
		return(true);
	if(BGBCC_CCXL_TypeSmallDComplexP(ctx, ty))
		return(true);
	return(false);

#if 0
	int i;
	
	i=ty.val;
	switch(i)
	{
	case CCXL_TY_QUATF:
//	case CCXL_TY_QUATD:
		return(true);
	}
	return(false);
#endif
}

bool BGBCC_CCXL_TypeIsTypedefP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
		return(false);

	obj=ctx->literals[i-256];
	if(!obj)
		return(false);
	if(obj->littype==CCXL_LITID_TYPEDEF)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeGetTypedefType(
	BGBCC_TransState *ctx, ccxl_type ty, ccxl_type *rty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
	{
		*rty=ty;
		return(false);
	}
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
	{
		*rty=ty;
		return(false);
	}

	obj=ctx->literals[i-256];
	if(!obj)
	{
		*rty=ty;
		return(false);
	}

	if(obj->littype==CCXL_LITID_TYPEDEF)
	{
		*rty=obj->decl->type;
		return(true);
	}
	
	*rty=ty;
	return(false);
}

int BGBCC_CCXL_TypeAsOprBasic(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(CCXL_TY_P);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=16)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_S)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNDEF_I)
		return(-1);

	return(BGBCC_CCXL_GetTypeBaseType(ctx, ty));
}

int BGBCC_CCXL_TypeAsOprExtBasic(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(CCXL_TY_P);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=256)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_S)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNDEF_I)
		return(-1);

	return(BGBCC_CCXL_GetTypeBaseType(ctx, ty));
}

int BGBCC_CCXL_TypeGetLogicalBaseSize(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i, bty, sz;

	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	switch(bty)
	{
	case CCXL_TY_I: sz=4; break;
	case CCXL_TY_L: sz=8; break;
	case CCXL_TY_F: sz=4; break;
	case CCXL_TY_D: sz=8; break;

	case CCXL_TY_SB: sz=1; break;
	case CCXL_TY_UB: sz=1; break;
	case CCXL_TY_SS: sz=2; break;
	case CCXL_TY_US: sz=2; break;
	case CCXL_TY_UI: sz=4; break;
	case CCXL_TY_UL: sz=8; break;

	case CCXL_TY_V: sz=1; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=ctx->arch_sizeof_long;
		if(!sz)
			{ sz=-1; }
		break;
	
	case CCXL_TY_F16:
	case CCXL_TY_BF16:
		sz=2; break;

	case CCXL_TY_FP8S:
	case CCXL_TY_FP8U:
		sz=1; break;

	case CCXL_TY_VEC4_FP8S:
	case CCXL_TY_VEC4_FP8U:
		sz=4; break;

	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
		sz=16; break;

	case CCXL_TY_M128:
	case CCXL_TY_M128P:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC3FX:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
	case CCXL_TY_VEC2D:
	case CCXL_TY_DCOMPLEX:
	case CCXL_TY_VEC4SI:
	case CCXL_TY_VEC4UI:
	case CCXL_TY_BCD128:
		sz=16; break;

	case CCXL_TY_M64:
	case CCXL_TY_VEC2F:
	case CCXL_TY_VEC3FQ:
	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_VEC4SW:
	case CCXL_TY_VEC4UW:
	case CCXL_TY_VEC4H:
	case CCXL_TY_VEC3H:
	case CCXL_TY_BCD64:
		sz=8; break;

	case CCXL_TY_GCOMPLEX:
		sz=32; break;

	case CCXL_TY_FIMAG:
		sz=4; break;
	case CCXL_TY_DIMAG:
		sz=8; break;
	case CCXL_TY_GIMAG:
		sz=16; break;

	case CCXL_TY_SI48:
	case CCXL_TY_UI48:
	case CCXL_TY_F48:
	case CCXL_TY_VEC3H_48:
		sz=6; break;

	case CCXL_TY_VARIANT:
	case CCXL_TY_VARSTRING:
	case CCXL_TY_VARWSTRING:
	case CCXL_TY_VARUSTRING:
	case CCXL_TY_VAROBJECT:
	case CCXL_TY_FIXNUM:
	case CCXL_TY_FLONUM:
		if(ctx->arch_sizeof_ptr==16)
			{ sz=16; break; }
		sz=8; break;

	case CCXL_TY_VALIST:
//		sz=64; break;
		sz=ctx->arch_sizeof_valist;
		if(!sz)
			{ sz=-1; }
		break;

	case CCXL_TY_SBITINT:
	case CCXL_TY_UBITINT:
	case CCXL_TY_OBITINT:
		i=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		
		if(i<=128)
		{
			if(i<=8)
				{ sz=1; break; }
			if(i<=16)
				{ sz=2; break; }
			if(i<=32)
				{ sz=4; break; }
			if(i<=64)
				{ sz=8; break; }
			if(i<=128)
				{ sz=16; break; }
		}
		
		i=(i+127)/128;
//		if(i<=0)i=3;
		sz=16*i; break;

	case CCXL_TY_BCDBIG_P0:
	case CCXL_TY_BCDBIG_P4:
		i=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		if(i<=0)i=3;
		sz=16*i; break;

	default:
		if(bty<256)
			{ BGBCC_DBGBREAK }
		sz=-1; break;
	}
	
	if(ctx->arch_sizeof_ptr)
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, ty))
			return(16);
		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(ctx->arch_sizeof_ptr);
	}
	
	return(sz);
}

int BGBCC_CCXL_TypeGetLogicalBaseAlign(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz, bty, nsz, bsz;

	bty=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	switch(bty)
	{
	case CCXL_TY_I: sz=4; break;
//	case CCXL_TY_L: sz=8; break;
	case CCXL_TY_F: sz=4; break;
//	case CCXL_TY_D: sz=8; break;

	case CCXL_TY_L:
	case CCXL_TY_D:
	case CCXL_TY_UL:
		sz=8; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_SB: sz=1; break;
	case CCXL_TY_UB: sz=1; break;
	case CCXL_TY_SS: sz=2; break;
	case CCXL_TY_US: sz=2; break;
	case CCXL_TY_UI: sz=4; break;
//	case CCXL_TY_UL: sz=8; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=ctx->arch_sizeof_long;
		if(!sz)
			{ sz=-1; }
		break;

	case CCXL_TY_V:
		sz=1; break;
	
	case CCXL_TY_F16:
	case CCXL_TY_BF16:
		sz=2; break;

	case CCXL_TY_FP8S:
	case CCXL_TY_FP8U:
		sz=1; break;

	case CCXL_TY_VEC4_FP8S:
	case CCXL_TY_VEC4_FP8U:
		sz=4; break;
	
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
		sz=16; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_SBITINT:
	case CCXL_TY_UBITINT:
	case CCXL_TY_OBITINT:
		bsz=BGBCC_CCXL_TypeGetBitIntSize(ctx, ty);
		if(bsz<=128)
		{
			if(bsz<=8)
				{ sz=1; }
			else if(bsz<=16)
				{ sz=2; }
			else if(bsz<=32)
				{ sz=4; }
			else if(bsz<=64)
				{ sz=8; }
			else if(bsz<=128)
				{ sz=16; }
		}else
		{
			sz=16;
		}

		nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_BCDBIG_P0:
	case CCXL_TY_BCDBIG_P4:
		sz=16; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_M128:
	case CCXL_TY_M128P:
	case CCXL_TY_VEC3F:
	case CCXL_TY_VEC3FX:
	case CCXL_TY_VEC4F:
	case CCXL_TY_QUATF:
	case CCXL_TY_VEC2D:
	case CCXL_TY_DCOMPLEX:
	case CCXL_TY_VEC4SI:
	case CCXL_TY_VEC4UI:
	case CCXL_TY_BCD128:
		sz=16; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_M64:
	case CCXL_TY_VEC2F:
	case CCXL_TY_VEC3FQ:
	case CCXL_TY_FCOMPLEX:
	case CCXL_TY_VEC4SW:
	case CCXL_TY_VEC4UW:
	case CCXL_TY_VEC4H:
	case CCXL_TY_VEC3H:
	case CCXL_TY_BCD64:
		sz=8; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_FIMAG:
		sz=4; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;
	case CCXL_TY_DIMAG:
		sz=8; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_GIMAG:
		sz=16; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;
	case CCXL_TY_GCOMPLEX:
		sz=32; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_SI48:
	case CCXL_TY_UI48:
	case CCXL_TY_F48:
	case CCXL_TY_VEC3H_48:
		sz=2;
		break;

	case CCXL_TY_VARIANT:
	case CCXL_TY_VARSTRING:
	case CCXL_TY_VARWSTRING:
	case CCXL_TY_VARUSTRING:
	case CCXL_TY_VAROBJECT:
		if(ctx->arch_sizeof_ptr==16)
		{
			sz=16; nsz=ctx->arch_align_max;
			if(nsz && (sz>nsz))
				sz=nsz;
			break;
		}

		sz=8; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;
	case CCXL_TY_VALIST:
		sz=ctx->arch_sizeof_valist;
		nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		if(!sz)
			{ sz=-1; }
		break;

	default:
		if(bty<256)
			{ BGBCC_DBGBREAK }
		sz=-1; break;
	}
	
	if(ctx->arch_sizeof_ptr)
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, ty))
			return(16);
		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(ctx->arch_sizeof_ptr);
	}
	
	return(sz);
}

int BGBCC_CCXL_TypeGetLogicalBaseShift(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz;
	sz=BGBCC_CCXL_TypeGetLogicalBaseSize(ctx, ty);
	
	if(sz<0)return(sz);
	if(sz<1)return(-1);
	if(sz==1)return(0);
	if(sz==2)return(1);
	if(sz==4)return(2);
	if(sz==8)return(3);
	if(sz==16)return(4);
	if(sz==32)return(5);
	if(sz==64)return(6);
	if(sz==128)return(7);
	if(sz==256)return(8);
	return(-1);
}

int BGBCC_CCXL_TypeGetLogicalSize(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type tty;
	int sz, asz, sz1;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		obj=BGBCC_CCXL_LookupStructureForType(ctx, ty);
		if(obj && obj->decl && obj->decl->fxmsize &&
			(obj->decl->fxmsize==obj->decl->fxnsize))
		{
			return(obj->decl->fxmsize);
		}

		if(!obj)
		{
			sz=BGBCC_CCXL_TypeGetLogicalBaseSize(ctx, ty);
			if(sz>0)
				return(sz);
		}

		return(-1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, ty, &tty);
		sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
//		asz=BGBCC_CCXL_TypeGetArraySize(ctx, ty);
		asz=BGBCC_CCXL_TypeGetArrayDimSize(ctx, ty);
		sz=sz1*asz;
		if(sz>0)
			return(sz);
		
		if((ty.val&CCXL_TY_PTRMASK)==CCXL_TYB1_PTRIDXE)
			return(0);
		
		return(-1);
	}

	
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, ty))
			return(16);

		if(ctx->arch_sizeof_ptr)
			return(ctx->arch_sizeof_ptr);
		return(-1);
	}
	
	if(BGBCC_CCXL_TypeObjectP(ctx, ty))
	{
		obj=BGBCC_CCXL_LookupStructureForType(ctx, ty);

		if(obj->littype==CCXL_LITID_TYPEDEF)
		{
			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->decl->type);
			return(sz);
		}

		if(	(obj->littype==CCXL_LITID_STRUCT) ||
			(obj->littype==CCXL_LITID_UNION))
		{
			if(!(obj->decl))
				return(-1);

			if(obj && obj->decl && obj->decl->fxmsize &&
				(obj->decl->fxmsize==obj->decl->fxnsize))
			{
				return(obj->decl->fxmsize);
			}
		}

		if(obj->littype==CCXL_LITID_CLASS)
		{
			if(!(obj->decl))
				return(-1);

			if(obj && obj->decl && (obj->decl->fxmsize>0) &&
				(obj->decl->fxmsize==obj->decl->fxnsize))
			{
				return(obj->decl->fxmsize);
			}

			return(-1);
		}

		if(obj->littype==CCXL_LITID_FUNCTION)
			return(-1);

		BGBCC_DBGBREAK
		return(-1);
	}

	sz=BGBCC_CCXL_TypeGetLogicalBaseSize(ctx, ty);
	if(sz>0)
		return(sz);

	return(-1);
}

int BGBCC_CCXL_TypeGetLogicalAlign(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type tty;
	int sz, asz, sz1, al;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		obj=BGBCC_CCXL_LookupStructureForType(ctx, ty);
		if(obj && obj->decl && obj->decl->fxmalgn &&
			(obj->decl->fxmalgn==obj->decl->fxnalgn))
		{
			return(obj->decl->fxmalgn);
		}

		if(!obj)
		{
			al=BGBCC_CCXL_TypeGetLogicalBaseAlign(ctx, ty);
			if(al>0)
				return(al);
		}

		return(-1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, ty, &tty);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
		return(al);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, ty))
			return(16);

		if(ctx->arch_sizeof_ptr)
			return(ctx->arch_sizeof_ptr);

		return(-1);
	}
	
	if(BGBCC_CCXL_TypeObjectP(ctx, ty))
	{
		obj=BGBCC_CCXL_LookupStructureForType(ctx, ty);

		if(obj->littype==CCXL_LITID_TYPEDEF)
		{
			sz=BGBCC_CCXL_TypeGetLogicalAlign(ctx, obj->decl->type);
			return(sz);
		}

		if(	(obj->littype==CCXL_LITID_STRUCT) ||
			(obj->littype==CCXL_LITID_UNION))
		{
			if(!(obj->decl))
				return(-1);

			if(obj && obj->decl && obj->decl->fxmalgn &&
				(obj->decl->fxmalgn==obj->decl->fxnalgn))
			{
				return(obj->decl->fxmalgn);
			}
		}

		if(obj->littype==CCXL_LITID_CLASS)
			return(-1);
		if(obj->littype==CCXL_LITID_FUNCTION)
			return(-1);

		BGBCC_DBGBREAK
		return(-1);
	}

	al=BGBCC_CCXL_TypeGetLogicalBaseAlign(ctx, ty);
	return(al);
}

int BGBCC_CCXL_TypeGetLogicalPadSize(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz, sz1, al;
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, ty);
	al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, ty);
	
	if(al>ctx->arch_align_max)
		al=ctx->arch_align_max;
	
	sz1=sz;
	
	if(sz&(al-1))
	{
		BGBCC_DBGBREAK
	}
	
	if(al>1)
		sz1=(sz+(al-1))&(~(al-1));
	return(sz1);
}

ccxl_status BGBCC_CCXL_TypeDerefType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int bt, pn;
	int i, j, k;

//	if(!BGBCC_CCXL_TypePointerP(ctx, ty))

	BGBCC_CCXL_TypeGetTypedefType(ctx, sty, &sty);

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		pn=(sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;

//		if(((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
		if(pn==CCXL_TY_PN4_A0B)
		{
			if(sty.val&CCXL_TY_PCLSMASK)
			{
				tty.val=sty.val&CCXL_TY_BASEMASK;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
		}

//		switch((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)
		switch(pn)
		{
		case CCXL_TY_PN4_Q1:
		case CCXL_TY_PN4_Q2:
		case CCXL_TY_PN4_Q3:
			i=(sty.val>>CCXL_TY_ARRSHL)&15;

			if(((i==1) || (i==9)) &&
				(pn==CCXL_TY_PN4_Q1))
			{
				tty.val=sty.val&CCXL_TY_BASEMASK;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}

			if((i>=2) && (i<=7))
			{
				j=8+(i-1);
//				tty.val=sty.val-(1<<CCXL_TY_ARRSHL);
				tty.val=(sty.val&(~(15<<CCXL_TY_ARRSHL)))|
					(j<<CCXL_TY_ARRSHL);
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
			if((i>=10) && (i<=15))
			{
				j=i-1;
//				tty.val=sty.val-(1<<CCXL_TY_ARRSHL);
				tty.val=(sty.val&(~(15<<CCXL_TY_ARRSHL)))|
					(j<<CCXL_TY_ARRSHL);
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
			break;
		}

	
//		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
//		if(sty.val&CCXL_TY_ARRMASK)
		if((sty.val&CCXL_TY_ARRMASK) && (pn<=7))
		{	
			tty.val=sty.val&(CCXL_TY_BASEMASK|CCXL_TY_PTRMASK);
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
//			if(tty.val==0x961)
//				k=1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

//		if(sty.val&CCXL_TY_PTRMASK)
		if(sty.val&CCXL_TY_PTRMASK)
		{
//			if(((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)<CCXL_TY_PN4_REF)
			if((sty.val&CCXL_TY_PTRMASK)<CCXL_TYB1_PTRIDX8)
			{
				tty.val=sty.val-CCXL_TY_PTRIDX1;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
	//			if(tty.val==0x961)
	//				k=1;
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
			
			switch((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)
			{
			case CCXL_TY_PN4_Q1:
			case CCXL_TY_PN4_A0B:
				tty.val=sty.val&(~CCXL_TY_PTRMASK);
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			case CCXL_TY_PN4_Q2:
			case CCXL_TY_PN4_Q3:
				tty.val=sty.val-CCXL_TY_PTRIDX1;
				tty.val&=~CCXL_TY_ARRMASK;
				tty.val|=((sty.val&CCXL_TY_ARRMASK)>>4)&CCXL_TY_ARRMASK;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			case CCXL_TY_PN4_A0P1:
				tty.val=(sty.val&(~CCXL_TY_PTRMASK))|CCXL_TY_PTRIDX1;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);

			case CCXL_TY_PN4_Q1P1:
				tty.val=(sty.val&(~CCXL_TY_PTRMASK))|CCXL_TY_PTRIDX1;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
		}
		
		switch(sty.val&CCXL_TY_BASEMASK)
		{
		case CCXL_TY_VEC2F:		case CCXL_TY_VEC3F:
		case CCXL_TY_VEC4F:		case CCXL_TY_QUATF:
		case CCXL_TY_FCOMPLEX:	case CCXL_TY_FIMAG:
		case CCXL_TY_VEC3FQ:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(CCXL_STATUS_YES);
		case CCXL_TY_VEC2D:		case CCXL_TY_VEC3D:
		case CCXL_TY_VEC4D:		case CCXL_TY_QUATD:
		case CCXL_TY_DCOMPLEX:	case CCXL_TY_DIMAG:
		case CCXL_TY_VEC3FX:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(CCXL_STATUS_YES);
		case CCXL_TY_GCOMPLEX:
		case CCXL_TY_GIMAG:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F128);
			return(CCXL_STATUS_YES);
		case CCXL_TY_VEC4SW:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_SS);
			return(CCXL_STATUS_YES);
		case CCXL_TY_VEC4UW:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_US);
			return(CCXL_STATUS_YES);
		case CCXL_TY_VEC4SI:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			return(CCXL_STATUS_YES);
		case CCXL_TY_VEC4UI:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI);
			return(CCXL_STATUS_YES);
		case CCXL_TY_VEC4H:
		case CCXL_TY_VEC3H:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F16);
			return(CCXL_STATUS_YES);

		case CCXL_TY_VARIANT:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);

		case CCXL_TY_VARSTRING:
		case CCXL_TY_VARWSTRING:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_US);
			return(CCXL_STATUS_YES);

		case CCXL_TY_VARUSTRING:
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UB);
			return(CCXL_STATUS_YES);

		default:
			break;
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
//			tty.val=sty.val&(CCXL_TYB2_BASEMASK|
//				CCXL_TYB2_PTRMASK|CCXL_TY_TYTY_MASK);
			bt=sty.val&CCXL_TYB2_BASEMASK;
			pn=(sty.val>>6)&3;
			tty.val=bt|(pn<<12)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if(sty.val&CCXL_TYB2_PTRMASK)
		{
			tty.val=sty.val-CCXL_TYB2_PTRIDX1;
			if(!(tty.val&(CCXL_TYB2_ARRMASK|CCXL_TYB2_PTRMASK)))
				tty.val=(tty.val&CCXL_TYB2_BASEMASK)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
			tty.val=sty.val&(CCXL_TYB3_BASEMASK|
				CCXL_TYB3_PTRMASK|CCXL_TY_TYTY_MASK);

//			bt=sty.val&CCXL_TYB3_BASEMASK;
//			pn=(sty.val>>6)&3;
//			tty.val=bt|(pn<<12)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if(sty.val&CCXL_TYB3_PTRMASK)
		{
			tty.val=sty.val-CCXL_TYB3_PTRIDX1;
//			if(!(tty.val&(CCXL_TYB3_ARRMASK|CCXL_TYB3_PTRMASK)))
//				tty.val=(tty.val&CCXL_TYB2_BASEMASK)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
			for(i=0; i<15; i++)
				ovf.asz[i]=ovf.asz[i+1];
			ovf.asz[15]=0;
			ovf.an--;
//			ovf.asz[ovf.an]=0;
		}else if(ovf.an)
		{
			ovf.an=0;
		}else if(ovf.pn)
		{
			ovf.pn--;
		}else
		{
			BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
				sty.val);
			*rdty=sty;
			return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
		}
	
		i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
		BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
		*rdty=tty;
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypePointerType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i, j, k, pn;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		pn=(sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;

		switch(pn)
		{
		case CCXL_TY_PN4_Q1:
		case CCXL_TY_PN4_Q2:
		case CCXL_TY_PN4_Q3:
			i=(sty.val>>CCXL_TY_ARRSHL)&15;
			if((i>=1) && (i<=7))
			{
				j=8+i;
				tty.val=(sty.val&(~(15<<CCXL_TY_ARRSHL)))|
					(j<<CCXL_TY_ARRSHL);
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
			if((i>=10) && (i<=15))
			{
				tty.val=sty.val;
				*rdty=tty;
				return(CCXL_STATUS_YES);
			}
			break;
		}

//		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		if(sty.val&CCXL_TY_ARRMASK)
		{
			tty.val=sty.val&(CCXL_TY_BASEMASK|CCXL_TY_PTRMASK);
			tty.val=tty.val+CCXL_TY_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if((sty.val&CCXL_TY_PTRMASK)<CCXL_TYB1_PTRIDX7)
		{
			tty.val=sty.val+CCXL_TY_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if((sty.val&CCXL_TY_PTRMASK)==CCXL_TYB1_PTRIDXE)
		{
			tty.val=sty.val+CCXL_TY_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(CCXL_TYB2_BASEMASK|
				CCXL_TYB2_PTRMASK|CCXL_TY_TYTY_MASK);
			tty.val|=CCXL_TY_TYTY_BASIC2;
			tty.val=tty.val+CCXL_TYB2_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		tty.val=sty.val+CCXL_TYB2_PTRIDX1;
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(CCXL_TYB3_BASEMASK|
				CCXL_TYB3_PTRMASK|CCXL_TY_TYTY_MASK);
			tty.val|=CCXL_TY_TYTY_BASIC3;
			tty.val=tty.val+CCXL_TYB3_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		tty.val=sty.val+CCXL_TYB3_PTRIDX1;
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an)
		{
			ovf.an=0;
			ovf.pn++;
		}else
		{
			ovf.pn++;
		}
	
		i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
		*rdty=tty;
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypeAutoPromoteType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	ccxl_type tty;
	int i, j, k;

	if(	BGBCC_CCXL_TypeSgIntP(ctx, sty) ||
		BGBCC_CCXL_TypeSgLongP(ctx, sty))
	{
		if(rdty)*rdty=sty;
		return(CCXL_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty) &&
		!BGBCC_CCXL_TypeSgIntP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeFloat16P(ctx, sty) ||
		BGBCC_CCXL_TypeBFloat16P(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_F);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty) &&
		!BGBCC_CCXL_TypeSgLongP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_L);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeBitFieldP(ctx, sty))
	{
		j=BGBCC_CCXL_TypeBitFieldGetBits(ctx, sty);

		if(j<32)
		{
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
			if(rdty)*rdty=tty;
		}else
		{
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_L);
			if(rdty)*rdty=tty;
		}

		return(CCXL_STATUS_YES);
	}

#if 0
	if(BGBCC_CCXL_TypeSmallFloatP(ctx, sty) &&
		!BGBCC_CCXL_TypeFloatP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_F);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}
#endif

#if 0
	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty) &&
		!BGBCC_CCXL_TypeLongP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_L);
		*rdty=ty;
		return(CCXL_STATUS_YES);
	}
#endif

	if(rdty)*rdty=sty;
	return(CCXL_STATUS_NO);
}

ccxl_status BGBCC_CCXL_TypeVectorOfType(
	BGBCC_TransState *ctx,
	ccxl_type sty, int sz, ccxl_type *rdty)
{
	ccxl_type tty;
	int st;
	
	st=sty.val;

	if(st==CCXL_TY_SS)
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC4SW);
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}
	if(st==CCXL_TY_US)
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC4UW);
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}
	if(st==CCXL_TY_F16)
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC4H);
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if(st==CCXL_TY_I)
	{
		if(sz<=2)
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC2SI);
		else
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC4SI);
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}
	if(st==CCXL_TY_UI)
	{
		if(sz<=2)
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC2UI);
		else
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC4UI);
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}
	if(st==CCXL_TY_F)
	{
		if(sz<=2)
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC2F);
		else if(sz==3)
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC3F);
		else
			tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_VEC4F);
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if(rdty)*rdty=sty;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TypeGetArraySize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
			return(0);

		i=(sty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		i=(sty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		i=(sty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
			i=1;
			for(j=0; j<ovf.an; j++)
				i=i*ovf.asz[j];
		}else if(ovf.an)
		{
			i=ovf.asz[0];
		}else
		{
			i=0;
		}
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(0);
}

int BGBCC_CCXL_TypeArrayAdjustSize(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty,
	int newsz)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if((newsz>0) && (newsz<CCXL_TY_BASEARRMAX))
		{
			tty.val=(sty.val)&(~CCXL_TY_ARRMASK);
			tty.val=tty.val|(newsz<<CCXL_TY_ARRSHL);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if((newsz>0) && (newsz<CCXL_TYB2_BASEARRMAX))
		{
			tty.val=(sty.val)&(~CCXL_TYB2_ARRMASK);
			tty.val=tty.val|(newsz<<CCXL_TYB2_ARRSHL);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if((newsz>0) && (newsz<CCXL_TYB3_BASEARRMAX))
		{
			tty.val=(sty.val)&(~CCXL_TYB3_ARRMASK);
			tty.val=tty.val|(newsz<<CCXL_TYB3_ARRSHL);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TypeGetArrayDimSize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
			return(0);

		i=(sty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		i=(sty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		i=(sty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
//			i=1;
//			for(j=0; j<ovf.an; j++)
//				i=i*ovf.asz[j];
			i=ovf.asz[0];
//			i=ovf.asz[ovf.an-1];
		}else if(ovf.an)
		{
			i=ovf.asz[0];
		}else
		{
			i=0;
		}
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(0);
}

int BGBCC_CCXL_TypeGetBitIntSize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int st, sz;
	
	if(BGBCC_CCXL_TypeBitIntP(ctx, sty))
	{
		sz=BGBCC_CCXL_TypeGetArrayFinalSize(ctx, sty);
		if(sz<1)sz=1;
		return(sz);
	}
	
	st=sty.val;
	if(st<16)
	{
		sz=0;
		if((st==CCXL_TY_UB) || (st==CCXL_TY_SB))
			sz=8;
		if((st==CCXL_TY_US) || (st==CCXL_TY_SS))
			sz=16;
		if((st==CCXL_TY_UI) || (st==CCXL_TY_I))
			sz=32;
		if((st==CCXL_TY_UL) || (st==CCXL_TY_L))
			sz=64;
		return(sz);
	}
	
	return(0);
}

int BGBCC_CCXL_TypeGetArrayFinalSize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(((sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL)==CCXL_TY_PN4_A0B)
			return(0);

		i=(sty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		i=(sty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		i=(sty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
//			i=ovf.asz[0];
			i=ovf.asz[ovf.an-1];
		}else if(ovf.an)
		{
			i=ovf.asz[0];
		}else
		{
			i=0;
		}
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(0);
}

int BGBCC_CCXL_TypeGetPointerClass(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		i=(sty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;

//		if(i==CCXL_TY_PN4_PTRREF)
		if(i==CCXL_TY_PN4_A0B)
		{
			if(sty.val&CCXL_TY_PCLSMASK)
			{
				i=(sty.val&CCXL_TY_PCLSMASK)>>CCXL_TY_PCLSSHL;
				return(i&63);
			}
		}

//		i=(sty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
//		return(i);
		return(CCXL_PCLS_DFL);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		return(CCXL_PCLS_DFL);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		return(CCXL_PCLS_DFL);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		return(ovf.pcls);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(0);
}

int BGBCC_CCXL_TypeNearPointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
//	return(pcls==CCXL_PCLS_NEAR);
	return((pcls&CCXL_PCLS_HGMASK)==CCXL_PCLS_NEAR);
}

int BGBCC_CCXL_TypeFarPointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
//	return(pcls==CCXL_PCLS_FAR);
	return((pcls&CCXL_PCLS_HGMASK)==CCXL_PCLS_FAR);
}

int BGBCC_CCXL_TypeHugePointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
//	return(pcls==CCXL_PCLS_FAR);
	return((pcls&CCXL_PCLS_HGMASK)==CCXL_PCLS_HUGE);
}

int BGBCC_CCXL_TypeQuadPointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	if(	!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
		!BGBCC_CCXL_TypeFunctionP(ctx, sty))
		return(0);

	if(ctx->arch_sizeof_ptr==16)
	{
		if(BGBCC_CCXL_TypeNearPointerP(ctx, sty))
			return(0);
		return(1);
	}

	if(BGBCC_CCXL_TypeHugePointerP(ctx, sty))
		return(1);
	if(BGBCC_CCXL_TypeFarPointerP(ctx, sty))
		return(1);
	return(0);
}

int BGBCC_CCXL_TypeVolatilePointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
	return((pcls&CCXL_PCLS_ALIASMASK)==CCXL_PCLS_VOLATILE);
}

int BGBCC_CCXL_TypeRestrictPointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
	return((pcls&CCXL_PCLS_ALIASMASK)==CCXL_PCLS_RESTRICT);
}

int BGBCC_CCXL_TypeMayaliasPointerP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
	return((pcls&CCXL_PCLS_ALIASMASK)==CCXL_PCLS_MAYALIAS);
}

int BGBCC_CCXL_TypeBigEndianP(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	int pcls;
	
	if(!BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
		return(0);

	pcls=BGBCC_CCXL_TypeGetPointerClass(ctx, sty);
	return((pcls&CCXL_PCLS_ALIASMASK)==CCXL_PCLS_BIGEND);
}

int BGBCC_CCXL_TypeUnpackOverflow(
	BGBCC_TransState *ctx,
	ccxl_type ty,
	BGBCC_CCXL_TypeOverflow *rovf)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	byte tpn;
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&CCXL_TY_BASEMASK;
		tpn=(ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;
		if(tpn<CCXL_TY_PN4_REF)
			{ ovf.pn=tpn; }
		else if(tpn==CCXL_TY_PN4_REF)
			{ ovf.rn=1; }
		else if(tpn<CCXL_TY_PN4_PTRREF)
			{ ovf.qn=tpn-8; }
		else if(tpn==CCXL_TY_PN4_PTRREF)
			{ ovf.rn=1; ovf.pn=1; }
		else if(tpn==CCXL_TY_PN4_Q1P1)
			{ ovf.qn=1; ovf.pn=1; }
		else if(tpn==CCXL_TY_PN4_A0B)
			{ ovf.an=1; }
		else if(tpn==CCXL_TY_PN4_A0P1)
			{ ovf.an=1; ovf.pn=1; }

//		else if(tpn)
//			{ ovf.qn=tpn-12; ovf.pn=1; }
		if((tpn!=CCXL_TY_PN4_A0B) && (tpn!=CCXL_TY_PN4_A0P1))
		{
			ovf.asz[0]=(ty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
			ovf.an=ovf.asz[0]?1:0;
		}else
		{
			ovf.asz[0]=0;
			if(ty.val&CCXL_TY_PCLSMASK)
			{
				ovf.pn=1;
				ovf.pcls=(ty.val&CCXL_TY_PCLSMASK)>>CCXL_TY_PCLSSHL;
			}
			else
			{
				ovf.an=1;
			}
		}
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&CCXL_TYB2_BASEMASK;
		tpn=(ty.val&CCXL_TYB2_PTRMASK)>>CCXL_TYB2_PTRSHL;
		if(tpn<2)
			{ ovf.pn=tpn; }
		else if(tpn==2)
			{ ovf.rn=1; }
		else if(tpn==3)
			{ ovf.qn=1; }
		ovf.asz[0]=(ty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		ovf.an=ovf.asz[0]?1:0;
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&CCXL_TYB3_BASEMASK;
//		ovf.pn=(ty.val&CCXL_TYB3_PTRMASK)>>CCXL_TYB3_PTRSHL;
		tpn=(ty.val&CCXL_TYB3_PTRMASK)>>CCXL_TYB3_PTRSHL;
		if(tpn<2)
			{ ovf.pn=tpn; }
		else if(tpn==2)
			{ ovf.rn=1; }
		else if(tpn==3)
			{ ovf.qn=1; }

		ovf.asz[0]=(ty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		ovf.an=ovf.asz[0]?1:0;
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]);
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}
	
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
//	*rovf=ovf;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TypeIndexOverflow(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_TypeOverflow ovf)
{
	int i, j, k;
	
	if(!ctx->tyovf)
	{
		k=16384;
		ctx->tyovf=bgbcc_malloc(k*sizeof(BGBCC_CCXL_TypeOverflow *));
		ctx->n_tyovf=0;
		ctx->m_tyovf=k;
	}
	
	for(i=0; i<ctx->n_tyovf; i++)
	{
		if(!memcmp(ctx->tyovf[i], &(ovf),
			sizeof(BGBCC_CCXL_TypeOverflow)))
				return(i);
	}
	
	if(ctx->n_tyovf>=ctx->m_tyovf)
	{
		k=ctx->m_tyovf;		k=k+(k>>1);
		ctx->tyovf=bgbcc_realloc(ctx->tyovf,
			k*sizeof(BGBCC_CCXL_TypeOverflow *));
		ctx->m_tyovf=k;
	}

	i=ctx->n_tyovf++;
	ctx->tyovf[i]=bgbcc_malloc(sizeof(BGBCC_CCXL_TypeOverflow));
	memcpy(ctx->tyovf[i], &(ovf), sizeof(BGBCC_CCXL_TypeOverflow));
	return(i);
}

ccxl_status BGBCC_CCXL_TypeFromOverflow(
	BGBCC_TransState *ctx,
	ccxl_type *rty,
	BGBCC_CCXL_TypeOverflow ovf)
{
	ccxl_type tty;
	int i, j, k;
	
	if(!ovf.an && !ovf.pn)
		ovf.pcls=0;

	if((ovf.base>=0) && (ovf.base<CCXL_TY_BASETYMAX) &&
		(ovf.an==0) &&
		(ovf.pn< 8) &&
		(ovf.qn==0) &&
		(ovf.rn==0) &&
		(ovf.pcls==0))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TY_PTRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

#if 0
	if((ovf.base>=0) && (ovf.base<CCXL_TY_BASETYMAX) &&
		(ovf.an==0) &&
		(ovf.pn==1) &&
		(ovf.qn==0) &&
		(ovf.rn==0) &&
		(ovf.pcls>0) && (ovf.pcls<32))
	{
		tty.val=ovf.base|(CCXL_TY_PN4_A0B<<CCXL_TY_PTRSHL)|
			(ovf.pcls<<CCXL_TY_PCLSSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}
#endif

	if((ovf.base>=0) && (ovf.base<CCXL_TYB3_BASETYMAX) &&
		(ovf.an==0) && (ovf.pn<2) && (ovf.qn==0) && (ovf.rn==0) &&
		(ovf.pcls==0))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TYB3_PTRSHL)|
			CCXL_TY_TYTY_BASIC3;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TY_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) && (ovf.pn<8) &&
		(ovf.qn==0) && (ovf.rn==0) &&
		(ovf.asz[0]<CCXL_TY_BASEARRMAX) &&
		(ovf.pcls==0))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TY_PTRSHL)|
			(ovf.asz[0]<<CCXL_TY_ARRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TYB2_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) && (ovf.pn<2) &&
		(ovf.qn==0) && (ovf.rn==0) &&
		(ovf.asz[0]<CCXL_TYB2_BASEARRMAX) &&
		(ovf.pcls==0))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TYB2_PTRSHL)|
			(ovf.asz[0]<<CCXL_TYB2_ARRSHL)|
			CCXL_TY_TYTY_BASIC2;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TYB3_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) && (ovf.pn<2) &&
		(ovf.qn==0) && (ovf.rn==0) &&
		(ovf.asz[0]<CCXL_TYB3_BASEARRMAX) &&
		(ovf.pcls==0))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TYB3_PTRSHL)|
			(ovf.asz[0]<<CCXL_TYB3_ARRSHL)|
			CCXL_TY_TYTY_BASIC3;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	i=BGBCC_CCXL_TypeIndexOverflow(ctx, ovf);
	if(i>=0)
	{
		tty.val=i|CCXL_TY_TYTY_OVF1;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
//	*rdty=sty;
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypeFromSig(
	BGBCC_TransState *ctx,
	ccxl_type *rty, char *sig)
{
	int asz[16];
	BGBCC_CCXL_TypeOverflow ovf;
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_type tty;
	char *s;
	int an, pn, qn, rn, bty, pn4, pcls, bits;
	int i, j, k;
	
	s=sig; an=0; pn=0; bty=-1;
	qn=0; rn=0; pcls=0;
	bits=0;

	while(*s=='R')
		{ rn++; s++; }

	while((*s=='A') && (s[1]>='a') && (s[1]<='z'))
	{
		s++;
		i=*s++;
		switch(i)
		{
		case 'n': pcls|=CCXL_PCLS_NEAR; break;
		case 'f': pcls|=CCXL_PCLS_FAR; break;
		case 'p': pcls|=CCXL_PCLS_PACKED; break;
		case 'b': pcls|=CCXL_PCLS_BIGEND; break;
		case 'l': pcls|=CCXL_PCLS_LTLEND; break;
		case 'r': pcls|=CCXL_PCLS_RESTRICT; break;
		case 'v': pcls|=CCXL_PCLS_VOLATILE; break;
		case 'a': pcls|=CCXL_PCLS_MAYALIAS; break;
		default:
			break;
		}
		
//		BGBCC_DBGBREAK
	}


	while((*s=='A') && (s[1]>='0') && (s[1]<='9'))
	{
		s++; i=0;
		while(*s)
		{
			if(*s==';')
			{
				asz[an++]=i;
				s++; break;
			}
			if(*s==',')
			{
				asz[an++]=i;
				i=0; s++;
				continue;
			}
			if((*s>='0') && (*s<='9'))
			{
				i=(i*10)+((*s++)-'0');
				continue;
			}
			asz[an++]=i;
			break;
		}
	}

	while(*s=='Q')
	{
		if((s[1]>='0') && (s[1]<='9'))
		{
			break;
#if 0
			s++; i=0;
			while((*s>='0') && (*s<='9'))
				{ i=(i*10)+((*s++)-'0'); }
			qn+=i;
			continue;
#endif
		}

		qn++; s++;
	}

	if((s[0]=='Q') && (s[1]>='0') && (s[1]<='9'))
	{
		qn=1;
		an=1;
		asz[0]=s[1]-'0';
		s+=2;
	}

	while(*s=='P')
	{
		pn++; s++;

#if 1
		if((*s=='A') && (s[1]>='0') && (s[1]<='9'))
		{
			s++; i=0;
			while(*s)
			{
				if(*s==';')
					{ s++; break; }
				if(*s==',')
					{ s++; continue; }
				if((*s>='0') && (*s<='9'))
					{ s++; continue; }
				break;
			}
		}
#endif
	}

	while((*s=='B') && (s[1]>='0') && (s[1]<='9'))
	{
		s++; i=0;
		while(*s)
		{
			if(*s==';')
			{
				s++; break;
			}
			if((*s>='0') && (*s<='9'))
			{
				i=(i*10)+((*s++)-'0');
				continue;
			}
			break;
		}
		bits=i;
	}

	switch(*s)
	{
	case 'a': bty=CCXL_TY_SB; break;
	case 'b': bty=CCXL_TY_SB; break;
	case 'c': bty=CCXL_TY_SB; break;
	case 'd': bty=CCXL_TY_D; break;
//	case 'e': bty=CCXL_TY_D; break;
	case 'e': bty=CCXL_TY_F128; break;
	case 'f': bty=CCXL_TY_F; break;
	case 'g': bty=CCXL_TY_F128; break;
	case 'h': bty=CCXL_TY_UB; break;
	case 'i': bty=CCXL_TY_I; break;
	case 'j': bty=CCXL_TY_UI; break;
	case 'k': bty=CCXL_TY_F16; break;
	case 'l': bty=CCXL_TY_NL; break;
	case 'm': bty=CCXL_TY_UNL; break;
	case 'n': bty=CCXL_TY_I128; break;
	case 'o': bty=CCXL_TY_UI128; break;
	case 'p':
		bty=CCXL_TY_NL;
		if(ctx->arch_sizeof_ptr==16)
			bty=CCXL_TY_I128;
		if(ctx->arch_sizeof_ptr==8)
			bty=CCXL_TY_L;
		if(ctx->arch_sizeof_ptr==4)
//			bty=CCXL_TY_I;
			bty=CCXL_TY_I;
		if(ctx->arch_sizeof_ptr==2)
//			bty=CCXL_TY_SS;
			bty=CCXL_TY_US;
		break;
	case 'q':
		bty=CCXL_TY_UNL;
		if(ctx->arch_sizeof_ptr==16)
			bty=CCXL_TY_UI128;
		if(ctx->arch_sizeof_ptr==8)
			bty=CCXL_TY_UL;
		if(ctx->arch_sizeof_ptr==4)
//			bty=CCXL_TY_I;
			bty=CCXL_TY_UI;
		if(ctx->arch_sizeof_ptr==2)
//			bty=CCXL_TY_SS;
			bty=CCXL_TY_US;
		break;
//		bty=-1; break;
	case 'r': bty=CCXL_TY_VARIANT; break;
	case 's': bty=CCXL_TY_SS; break;
	case 't': bty=CCXL_TY_US; break;
	case 'u': bty=CCXL_TY_BF16; break;
	case 'v': bty=CCXL_TY_V; break;
	case 'w': bty=CCXL_TY_US; break;
	case 'x': bty=CCXL_TY_L; break;
	case 'y': bty=CCXL_TY_UL; break;
	case 'z': bty=CCXL_TY_VARARGS; break;
	case 'C':
		switch(s[1])
		{
		case 'a':	bty=CCXL_TY_VEC2F; break;
		case 'b':	bty=CCXL_TY_VEC3F; break;
		case 'c':	bty=CCXL_TY_VEC4F; break;
		case 'd':	bty=CCXL_TY_DCOMPLEX; break;
		case 'e':	bty=CCXL_TY_GCOMPLEX; break;
		case 'f':	bty=CCXL_TY_FCOMPLEX; break;
		case 'g':	bty=CCXL_TY_GCOMPLEX; break;
		case 'h':	bty=CCXL_TY_VEC2D; break;
		case 'i':	bty=CCXL_TY_VEC4SI; break;
		case 'j':	bty=CCXL_TY_VEC4UI; break;
		case 'k':	bty=CCXL_TY_HCOMPLEX; break;
		case 'l':	bty=CCXL_TY_VEC4H; break;
		case 'm':	bty=CCXL_TY_M64; break;
		case 'n':	bty=CCXL_TY_M128; break;
		case 'o':	bty=CCXL_TY_VAROBJECT; break;
		case 'p':	bty=CCXL_TY_M128P; break;
		case 'q':	bty=CCXL_TY_QUATF; break;
		case 'r':	bty=CCXL_TY_VARIANT; break;
		case 's':	bty=CCXL_TY_VARSTRING; break;
		case 't':	bty=CCXL_TY_VARSYMBOL; break;
		case 'u':	bty=CCXL_TY_HCOMPLEX; break;
		case 'v':	bty=CCXL_TY_VEC4SW; break;
		case 'w':	bty=CCXL_TY_VEC4UW; break;
		case 'x':	bty=CCXL_TY_VAROBJECT; break;
		case 'y':	bty=CCXL_TY_VARCLASS; break;
		case 'z':	bty=CCXL_TY_VARARR; break;
		default:  bty=-1; break;
		}
		break;
	case 'D':
		switch(s[1])
		{

		case 'h':	bty=CCXL_TY_VEC3FX; break;

		case 'k':	bty=CCXL_TY_VEC3H; break;
		case 'l':	bty=CCXL_TY_VEC3FQ; break;

		case 'm':	bty=CCXL_TY_BCD64; break;
		case 'o':	bty=CCXL_TY_BCD128; break;

		case 's':	bty=CCXL_TY_VARWSTRING; break;
		case 't':	bty=CCXL_TY_VARUSTRING; break;

		case 'u':	bty=CCXL_TY_VEC3H_48; break;

		case 'v':	bty=CCXL_TY_VEC4_FP8S; break;
		case 'w':	bty=CCXL_TY_VEC4_FP8U; break;

		case 'x':
			bty=CCXL_TY_SBITINT;
			s+=2; i=0;
			while((*s>='0') && (*s<='9'))
				{ i=(i*10)+((*s++)-'0'); }
			asz[an++]=i;
			break;
		case 'y':
			bty=CCXL_TY_UBITINT;
			s+=2; i=0;
			while((*s>='0') && (*s<='9'))
				{ i=(i*10)+((*s++)-'0'); }
			asz[an++]=i;
			break;
		case 'p':
			bty=CCXL_TY_OBITINT;
			s+=2; i=0;
			while((*s>='0') && (*s<='9'))
				{ i=(i*10)+((*s++)-'0'); }
			asz[an++]=i;
			break;

//		case 'a': bty=CCXL_TY_VEC2D; break;
		case 'z': bty=CCXL_TY_VALIST; break;
		default:  bty=-1; break;
		}
		break;
	case 'G':
		switch(s[1])
		{
		case 'a': bty=CCXL_TY_FP8S; break;

		case 'd': bty=CCXL_TY_DIMAG; break;
		case 'e': bty=CCXL_TY_GIMAG; break;
		case 'f': bty=CCXL_TY_FIMAG; break;
		case 'g': bty=CCXL_TY_GIMAG; break;

		case 'h': bty=CCXL_TY_FP8U; break;

		case 'i': bty=CCXL_TY_SI48; break;
		case 'j': bty=CCXL_TY_UI48; break;

		case 'k': bty=CCXL_TY_HIMAG; break;
		case 'l': bty=CCXL_TY_VEC3H_48; break;

		case 'w': bty=CCXL_TY_BIGINT; break;
		case 'x': bty=CCXL_TY_FIXNUM; break;
		case 'y': bty=CCXL_TY_FLONUM; break;

		default:  bty=-1; break;
		}
		break;

	case 'X': case 'Y':
//	case 'U':
		st=BGBCC_CCXL_LookupStructureForSig(ctx, s);
		if(ctx->ril3_norec && !st)
			st=BGBCC_CCXL_GetStructureForSig(ctx, s);

		if(st)
			{ bty=st->litid+256; }
		else
			{ bty=-1; }
		break;
	case 'U':
		st=BGBCC_CCXL_LookupTypedefForSig(ctx, s);
		if(ctx->ril3_norec && !st)
			st=BGBCC_CCXL_GetTypedefForSig(ctx, s);

		if(st)
			{ bty=st->litid+256; }
		else
			{ bty=-1; }
		break;

	case '(':
		st=BGBCC_CCXL_GetLiteralRawSig(ctx, s);
		if(st)
			{ bty=st->litid+256; }
		else
			{ bty=-1; }
		break;

	default:  bty=-1; break;
	}
	
	if(bits)
	{
		switch(bty)
		{
		case CCXL_TY_SB:	bty=CCXL_TY_BITFIELD_SB;	break;
		case CCXL_TY_UB:	bty=CCXL_TY_BITFIELD_UB;	break;
		case CCXL_TY_SS:	bty=CCXL_TY_BITFIELD_SS;	break;
		case CCXL_TY_US:	bty=CCXL_TY_BITFIELD_US;	break;
		case CCXL_TY_I:		bty=CCXL_TY_BITFIELD_SI;	break;
		case CCXL_TY_UI:	bty=CCXL_TY_BITFIELD_UI;	break;
		case CCXL_TY_L:		bty=CCXL_TY_BITFIELD_SL;	break;
		case CCXL_TY_UL:	bty=CCXL_TY_BITFIELD_UL;	break;
		}
		asz[an++]=bits;
	}
	
	if((an==0) && (pn==0) && (bty>=256))
	{
		st=ctx->literals[bty-256];
		if(st && st->decl &&
			(st->littype==CCXL_LITID_TYPEDEF))
		{
			if(st->decl->type.val)
			{
				tty=st->decl->type;
				*rty=tty;
				return(CCXL_STATUS_YES);
			}
		}
	}
	
	pn4=-1;
	if((pn>=0) && (pn<8) && !qn && !rn)
		pn4=pn;
	if((qn>0) && (qn<4) && !pn && !rn)
		pn4=8+qn;
//	if((qn>0) && (qn<4) && (pn==1) && !rn)
//		pn4=12+qn;
	if((rn==1) && !pn && !qn)
		pn4=8;
	if((rn==1) && (pn==1) && !qn)
		pn4=12;
	if((qn==1) && (pn==1) && !rn)
		pn4=13;
	
//	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) && (an==0) && (pn<8))
	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) && (an==0) && (pn4>=0) && !pcls)
	{
//		tty.val=bty|(pn<<CCXL_TY_PTRSHL)|
		tty.val=bty|(pn4<<CCXL_TY_PTRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

#if 0
	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) && (an==0) && (pn4==1) &&
		(pcls>0) && (pcls<32))
	{
//		tty.val=bty|(1<<CCXL_TY_PTRSHL)|
//			(pcls<<CCXL_TY_PCLSSHL)|
//			CCXL_TY_TYTY_BASIC;
//		*rty=tty;
//		return(CCXL_STATUS_YES);
	}
#endif

	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) &&
//		(an==1) && (asz[0]>0) && (pn<8) &&
		(an==1) && (asz[0]>0) && (pn4>=0) &&
		(asz[0]<CCXL_TY_BASEARRMAX) && !pcls)
	{
//		tty.val=bty|(pn<<CCXL_TY_PTRSHL)|
		tty.val=bty|(pn4<<CCXL_TY_PTRSHL)|
			(asz[0]<<CCXL_TY_ARRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

#if 1
	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) &&
		(an==1) && (asz[0]==0) &&
		!pn && !qn && !rn && !pcls)
	{
//		tty.val=bty|(pn<<CCXL_TY_PTRSHL)|
		tty.val=bty|(CCXL_TY_PN4_A0B<<CCXL_TY_PTRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}
#endif

#if 1
	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) &&
		(an==1) && (asz[0]==0) &&
		(pn==1) && !qn && !rn && !pcls)
	{
//		tty.val=bty|(pn<<CCXL_TY_PTRSHL)|
		tty.val=bty|(CCXL_TY_PN4_A0P1<<CCXL_TY_PTRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}
#endif

	if((bty>=0) && (bty<CCXL_TYB2_BASETYMAX) &&
		(an==1) && (asz[0]>0) && (pn<2) &&
		!qn && !rn &&
		(asz[0]<CCXL_TYB2_BASEARRMAX) && !pcls)
	{
		tty.val=bty|(pn<<CCXL_TYB2_PTRSHL)|
			(asz[0]<<CCXL_TYB2_ARRSHL)|
			CCXL_TY_TYTY_BASIC2;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((bty>=0) && (bty<CCXL_TYB3_BASETYMAX) &&
		(an==0) && (pn<2) && !qn && !rn && !pcls)
	{
		tty.val=bty|(pn<<CCXL_TYB3_PTRSHL)|
			CCXL_TY_TYTY_BASIC3;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((bty>=0) && (bty<CCXL_TYB2_BASETYMAX) &&
		(an==1) && (asz[0]>0) && (pn<2) &&
		!qn && !rn &&
		(asz[0]<CCXL_TYB2_BASEARRMAX) && !pcls)
	{
		tty.val=bty|(pn<<CCXL_TYB2_PTRSHL)|
			(asz[0]<<CCXL_TYB2_ARRSHL)|
			CCXL_TY_TYTY_BASIC2;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if(bty>=0)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=bty;
		ovf.pn=pn;
		ovf.an=an;
		ovf.qn=qn;
		ovf.rn=rn;
		ovf.pcls=pcls;
		for(i=0; i<16; i++)
			ovf.asz[i]=0;
		for(i=0; i<an; i++)
			ovf.asz[i]=asz[i];

		i=BGBCC_CCXL_TypeIndexOverflow(ctx, ovf);
		if(i>=0)
		{
			tty.val=i|CCXL_TY_TYTY_OVF1;
			*rty=tty;
			return(CCXL_STATUS_YES);
		}
	}

#if 1
	printf("Debug Warning, Bad Sig %s\n", sig);
	tty.val=CCXL_TY_I;
	*rty=tty;
	return(CCXL_STATUS_NO);
#endif

	BGBCC_DBGBREAK
	if(*s=='(')
	{
	}

	tty.val=0;
	*rty=tty;
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);

	tty.val=0;
	*rty=tty;
	return(CCXL_STATUS_NO);
}

char *BGBCC_CCXL_TypeGetSig(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	char tb[256];
	int bsz[16];
	BGBCC_CCXL_TypeOverflow ovf;
	BGBCC_CCXL_LiteralInfo *st;
	char *s, *t;
	int pn, an, asz, bt, qn, rn;
	int i, j, k, l;

#if 1
	BGBCC_CCXL_TypeUnpackOverflow(ctx, ty, &ovf);
	bt=ovf.base;	asz=ovf.asz[0];
	pn=ovf.pn;		an=ovf.an;
	qn=ovf.qn;		rn=ovf.rn;
	for(i=0; i<an; i++)
		bsz[i]=ovf.asz[i];
#endif

#if 0
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		pn=(ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;
		asz=(ty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		bt=ty.val&CCXL_TY_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		pn=(ty.val&CCXL_TYB2_PTRMASK)>>CCXL_TYB2_PTRSHL;
		asz=(ty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		bt=ty.val&CCXL_TYB2_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		pn=(ty.val&CCXL_TYB3_PTRMASK)>>CCXL_TYB3_PTRSHL;
		asz=(ty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		bt=ty.val&CCXL_TYB3_BASEMASK;
		an=asz?1:0;
	}
#endif

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		i=(ty.val&CCXL_TYOVF_IDXMASK);
		bt=ctx->tyovf[i]->base;
		pn=ctx->tyovf[i]->pn;
		an=ctx->tyovf[i]->an;
		
		asz=1;
		for(j=0; j<an; j++)
		{
			k=ctx->tyovf[i]->asz[j];
			bsz[j]=k;
			asz=asz*k;
		}
	}
		
	t=tb;

	if(rn)
	{
		i=rn;
		while(i--)*t++='R';
		*t=0;
	}
	
	if((bt==CCXL_TY_SBITINT) || (bt==CCXL_TY_UBITINT) ||
		(bt==CCXL_TY_OBITINT) ||
		(bt==CCXL_TY_BCDBIG_P0) || (bt==CCXL_TY_BCDBIG_P4))
	{
		if(an>0)
		{
			an--;
		}else
		{
			bsz[an]=3;
		}
	}

	if(	(bt>=CCXL_TY_BITFIELD_SB) &&
		(bt<=CCXL_TY_BITFIELD_UL) )
	{
		sprintf(t, "B%u", bsz[0]);
		t+=strlen(t);

		an=0;
		
		switch(bt)
		{
			case CCXL_TY_BITFIELD_SB:	bt=CCXL_TY_SB;	break;
			case CCXL_TY_BITFIELD_UB:	bt=CCXL_TY_UB;	break;
			case CCXL_TY_BITFIELD_SS:	bt=CCXL_TY_SS;	break;
			case CCXL_TY_BITFIELD_US:	bt=CCXL_TY_US;	break;
			case CCXL_TY_BITFIELD_SI:	bt=CCXL_TY_I;	break;
			case CCXL_TY_BITFIELD_UI:	bt=CCXL_TY_UI;	break;
			case CCXL_TY_BITFIELD_SL:	bt=CCXL_TY_L;	break;
			case CCXL_TY_BITFIELD_UL:	bt=CCXL_TY_UL;	break;
		}
	}
	
	if(an>1)
	{
		sprintf(t, "A%u", bsz[0]);
		t+=strlen(t);
		for(i=1; i<an; i++)
		{
			sprintf(t, ",%u", bsz[i]);
			t+=strlen(t);
		}
		*t++=';';
		*t=0;
	}else if(an==1)
	{
//		sprintf(t, "A%d;", asz);
		sprintf(t, "A%u", asz);
		t+=strlen(t);
	}

	if(qn)
	{
		i=qn;
		while(i--)*t++='Q';
		*t=0;
	}

	if(pn)
	{
		i=pn;
		while(i--)*t++='P';
		*t=0;
	}
		
	if(bt<256)
	{
		switch(bt)
		{
		case CCXL_TY_I: *t++='i'; break;
		case CCXL_TY_L: *t++='x'; break;
		case CCXL_TY_F: *t++='f'; break;
		case CCXL_TY_D: *t++='d'; break;
		case CCXL_TY_P:
			*t++='P'; *t++='v'; break;
		case CCXL_TY_V: *t++='v'; break;
		case CCXL_TY_NL: *t++='l'; break;
		case CCXL_TY_SB: *t++='c'; break;
		case CCXL_TY_UB: *t++='h'; break;
		case CCXL_TY_SS: *t++='s'; break;
		case CCXL_TY_US: *t++='t'; break;
		case CCXL_TY_UI: *t++='j'; break;
		case CCXL_TY_UL: *t++='y'; break;
		case CCXL_TY_UNL: *t++='m'; break;

		case CCXL_TY_S: *t++='i'; break;

		case CCXL_TY_I128: *t++='n'; break;
		case CCXL_TY_UI128: *t++='o'; break;
		case CCXL_TY_F128: *t++='g'; break;
		case CCXL_TY_F16: *t++='k'; break;
		case CCXL_TY_BF16: *t++='u'; break;
		case CCXL_TY_VARIANT: *t++='r'; break;
		case CCXL_TY_VARARGS:		*t++='z'; break;

		case CCXL_TY_VALIST:		*t++='D'; *t++='z'; break;

		case CCXL_TY_VARSTRING:		*t++='C'; *t++='s'; break;
		case CCXL_TY_VAROBJECT:		*t++='C'; *t++='o'; break;

		case CCXL_TY_VARWSTRING:	*t++='D'; *t++='s'; break;
		case CCXL_TY_VARUSTRING:	*t++='D'; *t++='t'; break;

		case CCXL_TY_VEC2F:			*t++='C'; *t++='a'; break;
		case CCXL_TY_VEC3F:			*t++='C'; *t++='b'; break;
		case CCXL_TY_VEC4F:			*t++='C'; *t++='c'; break;
		case CCXL_TY_QUATF:			*t++='C'; *t++='q'; break;

		case CCXL_TY_VEC2D:			*t++='C'; *t++='h'; break;

		case CCXL_TY_M64:			*t++='C'; *t++='m'; break;
		case CCXL_TY_M128:			*t++='C'; *t++='n'; break;
		case CCXL_TY_M128P:			*t++='C'; *t++='p'; break;

		case CCXL_TY_FCOMPLEX:		*t++='C'; *t++='f'; break;
		case CCXL_TY_DCOMPLEX:		*t++='C'; *t++='d'; break;
		case CCXL_TY_GCOMPLEX:		*t++='C'; *t++='g'; break;
		case CCXL_TY_HCOMPLEX:		*t++='C'; *t++='k'; break;

		case CCXL_TY_FIMAG:			*t++='G'; *t++='f'; break;
		case CCXL_TY_DIMAG:			*t++='G'; *t++='d'; break;
		case CCXL_TY_GIMAG:			*t++='G'; *t++='g'; break;
		case CCXL_TY_HIMAG:			*t++='G'; *t++='k'; break;

		case CCXL_TY_VEC4SI:		*t++='C'; *t++='i'; break;
		case CCXL_TY_VEC4UI:		*t++='C'; *t++='j'; break;
		case CCXL_TY_VEC4SW:		*t++='C'; *t++='v'; break;
		case CCXL_TY_VEC4UW:		*t++='C'; *t++='w'; break;
		case CCXL_TY_VEC4H:			*t++='C'; *t++='l'; break;
		case CCXL_TY_VEC3H:			*t++='D'; *t++='k'; break;

		case CCXL_TY_BCD64:			*t++='D'; *t++='m'; break;
		case CCXL_TY_BCD128:		*t++='D'; *t++='o'; break;

		case CCXL_TY_FP8S:			*t++='G'; *t++='a'; break;
		case CCXL_TY_FP8U:			*t++='G'; *t++='h'; break;
		case CCXL_TY_VEC4_FP8S:		*t++='D'; *t++='v'; break;
		case CCXL_TY_VEC4_FP8U:		*t++='D'; *t++='w'; break;

		case CCXL_TY_SI48:			*t++='G'; *t++='i'; break;
		case CCXL_TY_UI48:			*t++='G'; *t++='j'; break;
		case CCXL_TY_F48:			*t++='D'; *t++='u'; break;
		case CCXL_TY_VEC3H_48:		*t++='G'; *t++='l'; break;

		case CCXL_TY_SBITINT:
			*t++='D';
			*t++='x';
			i=bsz[an];
			if(i>=10000)
				*t++='0'+((i/10000)%10);
			if(i>=1000)
				*t++='0'+((i/1000)%10);
			if(i>=100)
				*t++='0'+((i/100)%10);
			if(i>=10)
				*t++='0'+((i/10)%10);
			*t++='0'+(i%10);
			break;
		case CCXL_TY_UBITINT:
			*t++='D';
			*t++='y';
			i=bsz[an];
			if(i>=10000)
				*t++='0'+((i/10000)%10);
			if(i>=1000)
				*t++='0'+((i/1000)%10);
			if(i>=100)
				*t++='0'+((i/100)%10);
			if(i>=10)
				*t++='0'+((i/10)%10);
			*t++='0'+(i%10);
			break;
		case CCXL_TY_OBITINT:
			*t++='D';
			*t++='p';
			i=bsz[an];
			if(i>=10000)
				*t++='0'+((i/10000)%10);
			if(i>=1000)
				*t++='0'+((i/1000)%10);
			if(i>=100)
				*t++='0'+((i/100)%10);
			if(i>=10)
				*t++='0'+((i/10)%10);
			*t++='0'+(i%10);
			break;

		case CCXL_TY_UNDEF_I:
//			*t++='l';
			*t++='i';
			break;

		default:
			BGBCC_DBGBREAK
			break;
		}
		*t++=0;
	}else
	{
		if(ctx->ril_ip)
		{
			st=ctx->literals[bt-256];
			if(st && st->name)
			{
				sprintf(t, "X%s;", st->name);
			}else
			{
				BGBCC_DBGBREAK
			}
		}else
		{
//			st=ctx->literals[bt-256];
			sprintf(t, "X%d", bt-256);
		}
	}
	
	return(bgbcc_strdup(tb));
}

ccxl_type BGBCC_CCXL_TypeWrapBasicType(int ty)
{
	ccxl_type tty;

	if(ty<CCXL_TY_BASETYMAX)
	{
		tty.val=CCXL_TY_TYTY_BASIC|ty;
		return(tty);
	}

	if(ty<CCXL_TYB3_BASETYMAX)
	{
		tty.val=CCXL_TY_TYTY_BASIC3|ty;
		return(tty);
	}
	
	BGBCC_DBGBREAK

	tty.val=CCXL_TY_TYTY_BASIC3|ty;
	return(tty);
}

/* Returns true if both types are pointer types.
 * Ignores types of pointers if that can be implicitly converted.
 */
int BGBCC_CCXL_TypeCompatibleBothPointerP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
//	if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty) &&
	if(	BGBCC_CCXL_TypePointerP(ctx, dty) &&
		BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if(	 BGBCC_CCXL_TypeQuadPointerP(ctx, dty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
		if(	!BGBCC_CCXL_TypeQuadPointerP(ctx, dty) &&
			 BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
		return(1);
	}

	return(0);

//	return(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 0));
}

/* Values are compatible as per the abstract model.
 * Allows implicit promotion to int.
 * Distinguishes conversions which may be equivalent on the target.
 */
int BGBCC_CCXL_TypeCompatibleP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 0));
}

/* Values are compatible on the target. */
int BGBCC_CCXL_TypeCompatibleArchP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	if(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 2))
//	if(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 0))
		return(1);
	return(0);
}

/* Values are compatible, ignoring sign-extension. */
int BGBCC_CCXL_TypeCompatibleValueExtP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 3));
}

/* True if two values are type-format compatible.
 */
int BGBCC_CCXL_TypeCompatibleFormatP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 6));
}

/* True if two values can share the same type of storage.
 * Glosses over specifics about actual type being held there.
 */
int BGBCC_CCXL_TypeCompatibleStorageP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 7));
}

/*
 1 &= Ignore Value-Type Differences (Eg: Sign Conversion)
 2 &= Ignore Difference if equivalent on target.
 4 &= Ignore differences for FP/Vector conversion.
 */

int BGBCC_CCXL_TypeCompatibleFlP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty, int fl)
{
	ccxl_type dty2, sty2;
	int rt;

	if(dty.val==sty.val)
		return(1);

//	return(0);
//	fl&=1;

	if((dty.val==CCXL_TY_PIL) && ((fl&7)==7))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
			return(1);
		if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(1);

		if(	BGBCC_CCXL_TypeFloatP(ctx, sty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, sty) ||
			BGBCC_CCXL_TypeVec64P(ctx, sty))
				return(1);
	}

	if((sty.val==CCXL_TY_PIL) && ((fl&7)==7))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			return(1);
		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
			return(1);

		if(	BGBCC_CCXL_TypeFloatP(ctx, dty) ||
			BGBCC_CCXL_TypeDoubleP(ctx, dty) ||
			BGBCC_CCXL_TypeVec64P(ctx, dty))
				return(1);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
		{
			if(dty.val==sty.val)
				return(1);
		
			rt=0;
			switch(dty.val)
			{
			case CCXL_TY_SB:
				rt=(dty.val==sty.val) || (fl&1);	break;
			case CCXL_TY_UB:
				rt=(dty.val==sty.val) || (fl&1);	break;
			case CCXL_TY_SS:
				switch(sty.val)
				{
				case CCXL_TY_SB:	case CCXL_TY_UB:
				case CCXL_TY_SS:
					rt=1; break;				
				default: rt=fl&1; break;
				}
				break;
			case CCXL_TY_US:
				switch(sty.val)
				{
				case CCXL_TY_UB:	case CCXL_TY_US:
					rt=1; break;
				default: rt=fl&1; break;
				}
				break;
			case CCXL_TY_I:
				if(fl&1)
					{ rt=1; break; }
				if(sty.val==CCXL_TY_UI)
					{ rt=fl&1; break; }
				if(sty.val==CCXL_TY_UNL)
					{ rt=0; break; }
				if(((sty.val==CCXL_TY_NL) || (sty.val==CCXL_TY_UNL)) &&
					(ctx->arch_sizeof_long!=4))
						{ rt=0; break; }
				rt=1; break;
			case CCXL_TY_UI:
				if(fl&1)
					{ rt=1; break; }
				if((sty.val==CCXL_TY_I) || (sty.val==CCXL_TY_NL))
					{ rt=0; break; }
				if(((sty.val==CCXL_TY_NL) || (sty.val==CCXL_TY_UNL)) &&
					(ctx->arch_sizeof_long!=4))
						{ rt=0; break; }
				rt=1; break;

			case CCXL_TY_NL:
				if(ctx->arch_sizeof_long!=4)
					{ rt=0; break; }
				if(fl&1)
					{ rt=1; break; }
				if((sty.val==CCXL_TY_UI) || (sty.val==CCXL_TY_UNL))
					{ rt=0; break; }
				rt=1; break;

			case CCXL_TY_UNL:
				if(ctx->arch_sizeof_long!=4)
					{ rt=0; break; }
				if(fl&1)
					{ rt=1; break; }
				if((sty.val==CCXL_TY_I) || (sty.val==CCXL_TY_NL))
					{ rt=0; break; }
				rt=1; break;

			default:
				rt=0; break;
			}
			return(rt);
//			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgNLongP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSgNLongP(ctx, sty))
			return(1);
		
		if(!BGBCC_IsEmitRil(ctx))
		{
			if(ctx->arch_sizeof_long==8)
			{
				if(BGBCC_CCXL_TypeSgLongP(ctx, sty))
					return(1);
			}

#if 0
			if(!BGBCC_CCXL_TypeQuadPointerP(ctx, sty) &&
				(ctx->arch_sizeof_long==ctx->arch_sizeof_ptr))
			{
				if(BGBCC_CCXL_TypePointerP(ctx, sty))
				{
					if(fl&2)
						return(1);
				}
			}
#endif
		}
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSgLongP(ctx, sty))
			return(1);

		if(!BGBCC_IsEmitRil(ctx))
		{
			if(ctx->arch_sizeof_long==8)
			{
				if(BGBCC_CCXL_TypeSgNLongP(ctx, sty))
					return(1);
			}

#if 0
			if(!BGBCC_CCXL_TypeQuadPointerP(ctx, sty) &&
				(ctx->arch_sizeof_ptr==8))
			{
				if(BGBCC_CCXL_TypePointerP(ctx, sty))
				{
					if(fl&2)
						return(1);
				}
			}
#endif
		}
		
		if(fl&4)
		{
			if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
				return(1);
		}
	}

	if(dty.val==CCXL_TY_M64)
	{
		if(dty.val==sty.val)
			return(1);

		if(fl&4)
		{
			if(	(sty.val==CCXL_TY_D) ||
				(sty.val==CCXL_TY_FCOMPLEX)	 ||
				(sty.val==CCXL_TY_FIMAG)	)
					return(1);
		}

		if(	(sty.val==CCXL_TY_VEC2F)	||
			(sty.val==CCXL_TY_VEC4SW)	||
			(sty.val==CCXL_TY_VEC4UW)	||
			(sty.val==CCXL_TY_VEC4H)	||
			(sty.val==CCXL_TY_VEC3H)	||
			(sty.val==CCXL_TY_BCD64)	)
				return(1);

		if	(sty.val==CCXL_TY_VEC3H_48)
			return(1);
	}

	if((dty.val==CCXL_TY_M128) || (dty.val==CCXL_TY_M128P))
	{
		if(dty.val==sty.val)
			return(1);
		if(	(sty.val==CCXL_TY_VEC3F) ||
			(sty.val==CCXL_TY_VEC4F) ||
			(sty.val==CCXL_TY_QUATF) ||
			(sty.val==CCXL_TY_VEC4SI) ||
			(sty.val==CCXL_TY_VEC4UI) ||
			(sty.val==CCXL_TY_VEC2D)  ||
			(sty.val==CCXL_TY_BCD128) )
		{
			return(1);
		}

		if(fl&4)
		{
			if(	(sty.val==CCXL_TY_DCOMPLEX)	 ||
				(sty.val==CCXL_TY_DIMAG)	)
					return(1);
		}
	}

	if(dty.val==CCXL_TY_VEC2F)
	{
		if(dty.val==sty.val)
			return(1);
		if(sty.val==CCXL_TY_M64)
			return(1);
	}

	if(dty.val==CCXL_TY_FCOMPLEX)
	{
		if(dty.val==sty.val)
			return(1);
		if(fl&4)
		{
			if(sty.val==CCXL_TY_M64)
				return(1);
		}
	}

	if(	(dty.val==CCXL_TY_VEC4SW) ||
		(dty.val==CCXL_TY_VEC4UW) ||
		(dty.val==CCXL_TY_VEC4H) ||
		(dty.val==CCXL_TY_VEC3H) ||
		(dty.val==CCXL_TY_VEC3H_48))
	{
		if(dty.val==sty.val)
			return(1);
		if(sty.val==CCXL_TY_M64)
			return(1);
		if(	(sty.val==CCXL_TY_VEC4SW) ||
			(sty.val==CCXL_TY_VEC4UW) ||
			(sty.val==CCXL_TY_VEC4H) ||
			(sty.val==CCXL_TY_VEC3H))
			return(1);
	}

	if(dty.val==CCXL_TY_BCD64)
	{
		if(dty.val==sty.val)
			return(1);
		if(sty.val==CCXL_TY_M64)
			return(1);
	}

	if(dty.val==CCXL_TY_BCD128)
	{
		if(dty.val==sty.val)
			return(1);
		if(sty.val==CCXL_TY_M128)
			return(1);
	}

	if(dty.val==CCXL_TY_DCOMPLEX)
	{
		if(dty.val==sty.val)
			return(1);

		if(fl&4)
		{
			if((sty.val==CCXL_TY_M128) || (sty.val==CCXL_TY_M128P))
				return(1);
		}
	}

	if(	(dty.val==CCXL_TY_VEC3F) ||
		(dty.val==CCXL_TY_VEC4F) ||
		(dty.val==CCXL_TY_QUATF) ||
		(dty.val==CCXL_TY_VEC2D))
	{
		if(dty.val==sty.val)
			return(1);
		if((sty.val==CCXL_TY_M128) || (sty.val==CCXL_TY_M128P))
			return(1);
	}
	
	if(	(dty.val==CCXL_TY_VEC4SI) ||
		(dty.val==CCXL_TY_VEC4UI))
	{
		if(dty.val==sty.val)
			return(1);
		if((sty.val==CCXL_TY_M128) || (sty.val==CCXL_TY_M128P))
			return(1);
	}

	if(fl&2)
	{
		if(	(dty.val==CCXL_TY_M64) ||
			(dty.val==CCXL_TY_VEC2F) ||
			(dty.val==CCXL_TY_VEC4SW) ||
			(dty.val==CCXL_TY_VEC4UW) ||
			(dty.val==CCXL_TY_VEC4H) ||
			(dty.val==CCXL_TY_VEC3H) ||
			(dty.val==CCXL_TY_VEC3H_48) ||
			(dty.val==CCXL_TY_D) ||
			(dty.val==CCXL_TY_FCOMPLEX) ||
			(dty.val==CCXL_TY_FIMAG) ||
			(dty.val==CCXL_TY_L) ||
			(dty.val==CCXL_TY_UL) ||
			(dty.val==CCXL_TY_BCD64) )
		{
			if(	(sty.val==CCXL_TY_M64) ||
				(sty.val==CCXL_TY_VEC2F) ||
				(sty.val==CCXL_TY_VEC4SW) ||
				(sty.val==CCXL_TY_VEC4UW) ||
				(sty.val==CCXL_TY_VEC4H) ||
				(sty.val==CCXL_TY_VEC3H) ||
				(sty.val==CCXL_TY_VEC3H_48) ||
				(sty.val==CCXL_TY_D) ||
				(sty.val==CCXL_TY_FCOMPLEX) ||
				(sty.val==CCXL_TY_FIMAG) ||
				(sty.val==CCXL_TY_L) ||
				(sty.val==CCXL_TY_UL) ||
				(sty.val==CCXL_TY_BCD64) )
			{
				return(1);
			}
		}

		if(	(dty.val==CCXL_TY_M128)		||
			(dty.val==CCXL_TY_M128P)	||
			(dty.val==CCXL_TY_VEC3F)	||
			(dty.val==CCXL_TY_VEC4F)	||
			(dty.val==CCXL_TY_VEC2D)	||
			(dty.val==CCXL_TY_QUATF)	||
			(dty.val==CCXL_TY_VEC4SI)	||
			(dty.val==CCXL_TY_VEC4UI)	||
			(dty.val==CCXL_TY_I128)		||
			(dty.val==CCXL_TY_UI128)	||
			(dty.val==CCXL_TY_DCOMPLEX)	||
			(dty.val==CCXL_TY_DIMAG)	||
			(dty.val==CCXL_TY_BCD128)	)
		{
			if(	(sty.val==CCXL_TY_M128)		||
				(sty.val==CCXL_TY_M128P)	||
				(sty.val==CCXL_TY_VEC3F)	||
				(sty.val==CCXL_TY_VEC4F)	||
				(sty.val==CCXL_TY_VEC2D)	||
				(sty.val==CCXL_TY_QUATF)	||
				(sty.val==CCXL_TY_VEC4SI)	||
				(sty.val==CCXL_TY_VEC4UI)	||
				(sty.val==CCXL_TY_I128)		||
				(sty.val==CCXL_TY_UI128)	||
				(sty.val==CCXL_TY_DCOMPLEX)	||
				(sty.val==CCXL_TY_DIMAG)	||
				(sty.val==CCXL_TY_BCD128)	)
			{
				return(1);
			}
		}

		if(	(dty.val==CCXL_TY_SI48) ||
			(dty.val==CCXL_TY_UI48) ||
			(dty.val==CCXL_TY_F48) ||
			(dty.val==CCXL_TY_VEC3H_48) )
		{
			if(	(sty.val==CCXL_TY_SI48) ||
				(sty.val==CCXL_TY_UI48) ||
				(sty.val==CCXL_TY_F48) ||
				(sty.val==CCXL_TY_VEC3H_48) )
			{
				return(1);
			}
		}
	}

	if(BGBCC_CCXL_TypePointerP(ctx, dty) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
			return(0);

		if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
			BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			if(fl&3)
				return(1);

			BGBCC_CCXL_TypeDerefType(ctx, dty, &dty2);
			BGBCC_CCXL_TypeDerefType(ctx, sty, &sty2);
			
			if(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty2))
				return(1);

			return(0);
		}

		if(ctx->arch_sizeof_long==ctx->arch_sizeof_ptr)
		{
			if(BGBCC_CCXL_TypeSgNLongP(ctx, sty))
			{
				if(fl&2)
					return(1);
			}
		}

		if(ctx->arch_sizeof_ptr==8)
		{
			if(BGBCC_CCXL_TypeSgLongP(ctx, sty))
			{
				if(fl&2)
					return(1);
			}
		}
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, dty) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
			return(0);

		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			BGBCC_CCXL_TypeDerefType(ctx, dty, &dty2);
			BGBCC_CCXL_TypeDerefType(ctx, sty, &sty2);
			
			if(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty2))
				return(1);
		}

		if(BGBCC_CCXL_TypePointerP(ctx, sty))
		{
			BGBCC_CCXL_TypeDerefType(ctx, dty, &dty2);
			BGBCC_CCXL_TypeDerefType(ctx, sty, &sty2);
			
			if(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty2))
				return(1);
		}
	}
	
	if(BGBCC_CCXL_TypeVarObjP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeVarRefP(ctx, sty))
		{
			if(fl&1)
				return(1);
			if(BGBCC_CCXL_TypeEqualP(ctx, dty, sty))
				return(1);
			return(0);
		}
	}

	if(BGBCC_CCXL_TypeVarRefP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeVarObjP(ctx, sty))
		{
			if(fl&1)
				return(1);
			return(0);
		}

		if(BGBCC_CCXL_TypeVarRefP(ctx, sty))
		{
//			if(fl&3)
			if(fl&1)
				return(1);
			return(0);
		}

		return(0);
	}
	
	if(BGBCC_CCXL_TypeQuadPointerP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
			return(1);
		if((fl&2) && BGBCC_CCXL_TypeSgInt128P(ctx, sty))
			return(1);
		return(0);
	}
	
	if(BGBCC_CCXL_TypeSgInt128P(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSgInt128P(ctx, sty))
			return(1);
		if((fl&2) && BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
			return(1);

//		if(fl&4)
//		{
//			if(BGBCC_CCXL_TypeSmallInt128P(ctx, sty))
//				return(1);
//		}

		if(fl&4)
		{
			if((sty.val==CCXL_TY_M128) || (sty.val==CCXL_TY_M128P))
				return(1);
		}

		return(0);
	}

	if((fl&2) && (ctx->arch_sizeof_ptr==2))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallShortP(ctx, sty))
			{
				return(1);
			}
		}

#if 1
//		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
		if(	BGBCC_CCXL_TypeSgShortP(ctx, dty) ||
			BGBCC_CCXL_TypeSgIntP(ctx, dty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallShortP(ctx, sty))
			{
				return(1);
			}
		}
#endif
	}

	if((fl&2) && (ctx->arch_sizeof_ptr==4))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallIntP(ctx, sty) ||
				BGBCC_CCXL_TypeSgNLongP(ctx, sty))
			{
				return(1);
			}
		}

#if 1
//		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
		if(BGBCC_CCXL_TypeSgIntP(ctx, dty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
//				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallIntP(ctx, sty))
			{
				return(1);
			}
		}
#endif
	}

//	if((fl&2) && (ctx->arch_sizeof_ptr==8))
	if((fl&2) && (ctx->arch_sizeof_ptr==8))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSgLongP(ctx, sty) ||
				BGBCC_CCXL_TypeSgNLongP(ctx, sty))
			{
				return(1);
			}
		}

#if 1
		if(BGBCC_CCXL_TypeSgLongP(ctx, dty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty))
			{
				return(1);
			}
		}
#endif

#if 0
		if(BGBCC_CCXL_TypeSgLongP(ctx, dty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, dty))
		{
			if(BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
				return(0);
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallLongP(ctx, sty))
			{
				return(1);
			}
		}
#endif
	}

#if 0
	if(fl&4)
	{
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
		{
		}
	}
#endif

	return(0);
}


#if 0
// int BGBCC_CCXL_GetTypeConvMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
//	if(BGBCC_CCXL_TypeIntP(ctx, sty))
	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeLongP(ctx, dty))
			{ return(CCXL_OPCONV_I2L); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(CCXL_OPCONV_I2F); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(CCXL_OPCONV_I2D); }
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
			{ return(CCXL_OPCONV_I2P); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeIntP(ctx, dty))
			{ return(CCXL_OPCONV_L2I); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(CCXL_OPCONV_L2F); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(CCXL_OPCONV_L2D); }
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
			{ return(CCXL_OPCONV_L2P); }

		if(BGBCC_CCXL_TypeUnsignedIntP(ctx, dty))
			{ return(CCXL_OPCONV_L2I); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(CCXL_OPCONV_F2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(CCXL_OPCONV_F2L); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(CCXL_OPCONV_F2D); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(CCXL_OPCONV_D2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(CCXL_OPCONV_D2L); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(CCXL_OPCONV_D2F); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(CCXL_OPCONV_P2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(CCXL_OPCONV_P2L); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

//	BGBCC_CCXL_TagError(ctx,
//		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

// int BGBCC_CCXL_GetTypeConvIMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

	if(!BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	if(!BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	return(BGBCC_CCXL_GetTypeConvILMode(ctx, dty, sty));
}		

// int BGBCC_CCXL_GetTypeConvLMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

	if(!BGBCC_CCXL_TypeSmallLongP(ctx, dty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	if(!BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	return(BGBCC_CCXL_GetTypeConvILMode(ctx, dty, sty));
}		

// int BGBCC_CCXL_GetTypeConvILMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

//	if(!BGBCC_CCXL_TypeSmallIntP(ctx, dty))
//		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
//	if(!BGBCC_CCXL_TypeSmallIntP(ctx, sty))
//		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
		
	dt=dty.val; st=sty.val;
	
	if(dt==CCXL_TY_I)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_SXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_SXS); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_SXI); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if(dt==CCXL_TY_UI)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if((dt==CCXL_TY_L) || (dt==CCXL_TY_NL))
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_SXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_SXS); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_SXI); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if((dt==CCXL_TY_UL) || (dt==CCXL_TY_UNL))
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_ZXI); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}
	
	if(dt==CCXL_TY_SB)
		{ return(CCXL_OPCONV2_SXB); }
	if(dt==CCXL_TY_UB)
		{ return(CCXL_OPCONV2_ZXB); }

	if(dt==CCXL_TY_SS)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_SXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		return(CCXL_OPCONV2_SXS);
	}

	if(dt==CCXL_TY_US)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		return(CCXL_OPCONV2_ZXS);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}
#endif

bool BGBCC_CCXL_TypeSupportsOperatorP(
	BGBCC_TransState *ctx, ccxl_type ty, int opr)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeSgNLongP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeBitIntP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, ty))
	{
		return(true);
	}

	if(	BGBCC_CCXL_TypeFloatP(ctx, ty) ||
		BGBCC_CCXL_TypeDoubleP(ctx, ty) ||
		BGBCC_CCXL_TypeFloat16P(ctx, ty) ||
		BGBCC_CCXL_TypeFloat48P(ctx, ty) ||
		BGBCC_CCXL_TypeBFloat16P(ctx, ty) ||
		BGBCC_CCXL_TypeFloat128P(ctx, ty))
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
			return(true);
		if(opr==CCXL_BINOP_MUL)
			return(true);
		if(opr==CCXL_BINOP_DIV)
			return(true);
		return(false);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
			return(true);
		return(false);
	}

	if(	(ty.val==CCXL_TY_FCOMPLEX) ||
		(ty.val==CCXL_TY_DCOMPLEX) ||
		(ty.val==CCXL_TY_GCOMPLEX) ||
		(ty.val==CCXL_TY_HCOMPLEX) ||
		(ty.val==CCXL_TY_FIMAG) ||
		(ty.val==CCXL_TY_DIMAG) ||
		(ty.val==CCXL_TY_GIMAG) ||
		(ty.val==CCXL_TY_HIMAG))
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
			return(true);
		if(opr==CCXL_BINOP_MUL)
			return(true);
		if(opr==CCXL_BINOP_DIV)
			return(true);

		return(false);
	}

	if(	(ty.val==CCXL_TY_VEC2F) ||
		(ty.val==CCXL_TY_VEC3F) ||
		(ty.val==CCXL_TY_VEC4F) ||
		(ty.val==CCXL_TY_QUATF) ||
		(ty.val==CCXL_TY_VEC2D) ||
		(ty.val==CCXL_TY_VEC4H) ||
		(ty.val==CCXL_TY_VEC3H_48) ||
		(ty.val==CCXL_TY_VEC3H) ||
		(ty.val==CCXL_TY_BCD64) ||
		(ty.val==CCXL_TY_BCD128))
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
			return(true);
		if(opr==CCXL_BINOP_MUL)
			return(true);
		if(opr==CCXL_BINOP_DIV)
			return(true);

		if(opr==CCXL_BINOP_MOD)
		{
			if(	(ty.val==CCXL_TY_VEC3F) ||
				(ty.val==CCXL_TY_VEC4F) ||
				(ty.val==CCXL_TY_QUATF) )
					return(true);
			return(false);
		}

		if(opr==CCXL_BINOP_XOR)
		{
			if(	(ty.val==CCXL_TY_VEC2F) ||
				(ty.val==CCXL_TY_VEC3F) ||
				(ty.val==CCXL_TY_VEC4F) ||
				(ty.val==CCXL_TY_QUATF) ||
				(ty.val==CCXL_TY_VEC2D) )
					return(true);
			return(false);
		}
	}

	if(	(ty.val==CCXL_TY_VEC2SI) ||
		(ty.val==CCXL_TY_VEC2UI) ||
		(ty.val==CCXL_TY_VEC4SI) ||
		(ty.val==CCXL_TY_VEC4UI) ||
		(ty.val==CCXL_TY_VEC4SW) ||
		(ty.val==CCXL_TY_VEC4UW) )
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
			return(true);
		if(opr==CCXL_BINOP_MUL)
			return(true);
		if(opr==CCXL_BINOP_DIV)
			return(true);

		if(opr==CCXL_BINOP_AND)
			return(true);
		if(opr==CCXL_BINOP_OR)
			return(true);
		if(opr==CCXL_BINOP_XOR)
			return(true);

		return(false);
	}
	
	return(false);
}

ccxl_status BGBCC_CCXL_GetTypeBinaryDest(
	BGBCC_TransState *ctx, int opr,
	ccxl_type lty, ccxl_type rty,
	ccxl_type *rdty)
{
	return(BGBCC_CCXL_GetTypeBinaryDestB(ctx, opr, lty, rty, rdty, 0));
}

ccxl_status BGBCC_CCXL_GetTypeBinaryDestB(
	BGBCC_TransState *ctx, int opr,
	ccxl_type lty, ccxl_type rty,
	ccxl_type *rdty, int flag)
{
	ccxl_type dty2;
	int i0, i1, i2, sgz, ltyb, rtyb, dtyb, dtyc;
	int j;

	ltyb=BGBCC_CCXL_GetTypeBaseType(ctx, lty);
	rtyb=BGBCC_CCXL_GetTypeBaseType(ctx, rty);
	dtyb=0;

	dtyc=-1;
	if(flag&2)
	{
		dty2=*rdty;
		dtyc=BGBCC_CCXL_GetTypeBaseType(ctx, dty2);
	}

	if((ltyb==CCXL_TY_UI) && (rtyb==CCXL_TY_UI))
	{
		dtyb=-1;
	}

	if(	(opr==CCXL_BINOP_SHL) ||
		(opr==CCXL_BINOP_SHR) ||
		(opr==CCXL_BINOP_SHRR))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty) &&
			!BGBCC_CCXL_TypeSgIntP(ctx, lty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}
	
		if(BGBCC_CCXL_TypeSmallInt128P(ctx, lty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}
	}

	if(	BGBCC_CCXL_TypeBitIntP(ctx, lty) &&
		BGBCC_CCXL_TypeBitIntP(ctx, rty) )
	{
		i0=BGBCC_CCXL_TypeGetBitIntSize(ctx, lty);
		i1=BGBCC_CCXL_TypeGetBitIntSize(ctx, rty);
		
		if(i0==i1)
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(i1>i0)
		{
			BGBCC_CCXL_Warn(ctx, "BitInt operation size mismatch %d vs %d\n",
				i0, i1);
			if(i0>=i1)
				*rdty=lty;
			else
				*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}
	}

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, lty) &&
//		BGBCC_CCXL_TypeSmallIntP(ctx, rty))

	if(	(BGBCC_CCXL_TypeSmallIntP(ctx, lty) ||
			BGBCC_CCXL_TypeSgIntP(ctx, lty) ||
			BGBCC_CCXL_TypeBitFieldSmallIntP(ctx, lty)) &&
		(BGBCC_CCXL_TypeSmallIntP(ctx, rty) ||
			BGBCC_CCXL_TypeSgIntP(ctx, rty) ||
			BGBCC_CCXL_TypeBitFieldSmallIntP(ctx, rty)))
	{
//		ltyb=lty.val&CCXL_TY_BASEMASK;
//		rtyb=rty.val&CCXL_TY_BASEMASK;
		ltyb=BGBCC_CCXL_GetTypeBaseType(ctx, lty);
		rtyb=BGBCC_CCXL_GetTypeBaseType(ctx, rty);
		dtyb=CCXL_TY_I;
		
		if((ltyb>=CCXL_TY_BITFIELD_SB) && (ltyb<=CCXL_TY_BITFIELD_UL))
			ltyb=CCXL_TY_I;
		if((rtyb>=CCXL_TY_BITFIELD_SB) && (rtyb<=CCXL_TY_BITFIELD_UL))
			rtyb=CCXL_TY_I;
		
		switch(ltyb)
		{
		case CCXL_TY_I:
			switch(rtyb)
			{
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		case CCXL_TY_UI:
			dtyb=CCXL_TY_UI; break;

		case CCXL_TY_UNL:
			dtyb=CCXL_TY_UNL; break;

		case CCXL_TY_SB:
			switch(rtyb)
			{
			case CCXL_TY_SB:	dtyb=CCXL_TY_SB; break;
			case CCXL_TY_UB:	dtyb=CCXL_TY_UB; break;
			case CCXL_TY_SS:	dtyb=CCXL_TY_SS; break;
			case CCXL_TY_US:	dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		case CCXL_TY_UB:
			switch(rtyb)
			{
			case CCXL_TY_SB:	case CCXL_TY_UB:
				dtyb=CCXL_TY_UB; break;
			case CCXL_TY_SS:	dtyb=CCXL_TY_SS; break;
			case CCXL_TY_US:	dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;

		case CCXL_TY_SS:
			switch(rtyb)
			{
			case CCXL_TY_SB: case CCXL_TY_UB:
			case CCXL_TY_SS:
				dtyb=CCXL_TY_SS; break;
			case CCXL_TY_US:	dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		case CCXL_TY_US:
			switch(rtyb)
			{
			case CCXL_TY_SB:	case CCXL_TY_UB:
			case CCXL_TY_SS:	case CCXL_TY_US:
				dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		default:			dtyb=CCXL_TY_I; break;
		}

#if 0
		if(
			(opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_SUB) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_DIV) ||
			(opr==CCXL_BINOP_MOD) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR)
			)
#endif

#if 0
		if(dtyc==CCXL_TY_I)
		{
			if(	(dtyb==CCXL_TY_SB) ||
				(dtyb==CCXL_TY_UB) ||
				((dtyb==CCXL_TY_SS) ||
				(dtyb==CCXL_TY_US)) )
			{
				dtyb=CCXL_TY_I;
			}
		}
#endif

#if 1
		if(!(flag&1))
		{
//			if(	((dtyb==CCXL_TY_SB) ||
//				(dtyb==CCXL_TY_UB)) &&
//				((dtyc!=CCXL_TY_SB) && (dtyc!=CCXL_TY_UB)))
//			{
//				dtyb=CCXL_TY_SS;
//			}

			if(	(dtyb==CCXL_TY_SB) ||
				(dtyb==CCXL_TY_UB) )
			{
				if(	(dtyc!=CCXL_TY_SB) &&
					(dtyc!=CCXL_TY_UB))
				{
					dtyb=CCXL_TY_SS;
				}else
				{
					dtyb=dtyc;
				}

				if(
					(opr==CCXL_BINOP_ADD) ||
					(opr==CCXL_BINOP_SUB) ||
					(opr==CCXL_BINOP_MUL) ||
					(opr==CCXL_BINOP_SHL) )
				{
					dtyb=CCXL_TY_SS;
				}
			}

//			if(	((dtyb==CCXL_TY_SS) ||
//				(dtyb==CCXL_TY_US)) &&
//				((dtyc!=CCXL_TY_SS) && (dtyc!=CCXL_TY_US)) )
//			{
//				if(ctx->arch_sizeof_int!=2)
//				{
//					dtyb=CCXL_TY_I;
//				}
//			}

			if(	(dtyb==CCXL_TY_SS) ||
				(dtyb==CCXL_TY_US) )
			{
				if((dtyc!=CCXL_TY_SS) && (dtyc!=CCXL_TY_US))
				{
					if(ctx->arch_sizeof_int!=2)
					{
						dtyb=CCXL_TY_I;
					}
				}else
				{
					dtyb=dtyc;
				}

				if(
					(opr==CCXL_BINOP_ADD) ||
					(opr==CCXL_BINOP_SUB) ||
					(opr==CCXL_BINOP_MUL) ||
					(opr==CCXL_BINOP_SHL) )
				{
					if(ctx->arch_sizeof_int!=2)
					{
						dtyb=CCXL_TY_I;
					}
				}
			}
		}
#endif

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, dtyb);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if((BGBCC_CCXL_TypeSmallLongP(ctx, lty) ||
		BGBCC_CCXL_TypeBitFieldSmallLongP(ctx, lty)) &&
		(BGBCC_CCXL_TypeSmallLongP(ctx, rty) ||
		BGBCC_CCXL_TypeBitFieldSmallLongP(ctx, rty)))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}
		
		if(BGBCC_CCXL_TypeUnsignedLongP(ctx, lty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeUnsignedLongP(ctx, rty))
		{
			*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
//			if(	BGBCC_CCXL_TypeUnsignedP(ctx, lty) ||
//				BGBCC_CCXL_TypeUnsignedP(ctx, rty)	)
//				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI); }
//			else
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I); }
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
//			if(	BGBCC_CCXL_TypeUnsignedP(ctx, lty) ||
//				BGBCC_CCXL_TypeUnsignedP(ctx, rty)	)
			if(BGBCC_CCXL_TypeUnsignedP(ctx, rty))
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UL); }
			else
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L); }
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallInt128P(ctx, rty))
		{
//			if(	BGBCC_CCXL_TypeUnsignedP(ctx, lty) ||
//				BGBCC_CCXL_TypeUnsignedP(ctx, rty)	)
			if(BGBCC_CCXL_TypeUnsignedP(ctx, rty))
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128); }
			else
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128); }
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, rty))
//		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
//			return(CCXL_STATUS_YES);
//		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallDComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_DCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallGComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_GCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallQuatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_QUATF);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(	BGBCC_CCXL_TypeSmallDoubleP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallDoubleP(ctx, rty) &&
		(	BGBCC_CCXL_TypeRealP(ctx, lty) ||
			BGBCC_CCXL_TypeRealP(ctx, rty) )	)
	{
		if(	(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR)  ||
			(opr==CCXL_BINOP_XOR) )
		{
			/* HACK: TypeScript or similar. */
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			return(CCXL_STATUS_YES);
		}
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			if(	(BGBCC_CCXL_TypeUnsignedP(ctx, lty) &&
				 BGBCC_CCXL_TypeSgLongP(ctx, lty)) ||
				(BGBCC_CCXL_TypeUnsignedP(ctx, rty) &&
				 BGBCC_CCXL_TypeSgLongP(ctx, rty))
				 )
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UL);
			}else
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			}

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			if(	(BGBCC_CCXL_TypeUnsignedP(ctx, lty) &&
				 BGBCC_CCXL_TypeSgLongP(ctx, lty)) ||
				(BGBCC_CCXL_TypeUnsignedP(ctx, rty) &&
				 BGBCC_CCXL_TypeSgLongP(ctx, rty))
				 )
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UL);
			}else
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			}

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallInt128P(ctx, rty))
		{
//			if(	BGBCC_CCXL_TypeUnsignedP(ctx, lty) ||
//				BGBCC_CCXL_TypeUnsignedP(ctx, rty)	)
			if(BGBCC_CCXL_TypeUnsignedP(ctx, rty))
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128); }
			else
				{ *rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128); }
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(	BGBCC_CCXL_TypeFloatP(ctx, rty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, rty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallDComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_DCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallGComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_GCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallQuatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_QUATF);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, dty))
//			{ return(CCXL_OPCONV_L2P); }
		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, lty) &&
		BGBCC_CCXL_TypeSgInt128P(ctx, rty))
	{
		if(BGBCC_CCXL_TypeUnsignedP(ctx, lty) ||
			BGBCC_CCXL_TypeUnsignedP(ctx, rty)	)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128);
		}else
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);
		}
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallInt128P(ctx, lty) &&
		BGBCC_CCXL_TypeSmallInt128P(ctx, rty))
	{
		if(	(BGBCC_CCXL_TypeUnsignedP(ctx, lty) &&
			 BGBCC_CCXL_TypeSgInt128P(ctx, lty)) ||
			(BGBCC_CCXL_TypeUnsignedP(ctx, rty) &&
			 BGBCC_CCXL_TypeSgInt128P(ctx, rty))
			 )
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UI128);
		}
		else
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);
		}

		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	BGBCC_CCXL_TypeSmallBitIntP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallBitIntP(ctx, rty))
	{
		i0=BGBCC_CCXL_TypeGetBitIntSize(ctx, lty);
		i1=BGBCC_CCXL_TypeGetBitIntSize(ctx, rty);
		i2=(i0>=i1)?i0:i1;
		sgz=0;
		
		if(i0==i1)
		{
			if(	BGBCC_CCXL_TypeUnsignedP(ctx, lty) ||
				BGBCC_CCXL_TypeUnsignedP(ctx, rty)	)
					sgz=1;
		}else if(i0>i1)
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, lty))
				sgz=1;
		}else
		{
			if(BGBCC_CCXL_TypeUnsignedP(ctx, rty))
				sgz=1;
		}
	
//		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);

		if(sgz)
			{ *rdty=BGBCC_CCXL_MakeTypeID_Arr(ctx, CCXL_TY_UBITINT, i2); }
		else
			{ *rdty=BGBCC_CCXL_MakeTypeID_Arr(ctx, CCXL_TY_SBITINT, i2); }

		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}


	if(BGBCC_CCXL_TypeFloatP(ctx, lty) ||
		BGBCC_CCXL_TypeFloat16P(ctx, lty) ||
		BGBCC_CCXL_TypeBFloat16P(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, rty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallFComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_FCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallDComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_DCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallGComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_GCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallQuatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_QUATF);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, rty) ||
			BGBCC_CCXL_TypeBFloat16P(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallDComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_DCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallGComplexP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_GCOMPLEX);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallQuatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_QUATF);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, lty) ||
		BGBCC_CCXL_TypeVariantP(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
		((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB)))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			if(BGBCC_CCXL_TypeArrayP(ctx, lty))
				{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=lty;
			if(BGBCC_CCXL_TypeArrayP(ctx, lty))
				{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			if(opr==CCXL_BINOP_SUB)
			{
				if(	BGBCC_CCXL_TypeQuadPointerP(ctx, lty) ||
					BGBCC_CCXL_TypeQuadPointerP(ctx, rty))
				{
					*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);
					return(CCXL_STATUS_YES);
				}

				if(ctx->arch_sizeof_ptr==8)
				{
					*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
					return(CCXL_STATUS_YES);
				}

				if(ctx->arch_sizeof_ptr==16)
				{
					*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);
					return(CCXL_STATUS_YES);
				}

				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_ADD)
			{
				*rdty=lty;
				return(CCXL_STATUS_YES);
			}
		}

		printf("Ty %X %X\n", lty.val, rty.val);

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty) &&
		(opr==CCXL_BINOP_ADD))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, lty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}
	}

	if(BGBCC_CCXL_TypeSmallFloat128P(ctx, lty) &&
		BGBCC_CCXL_TypeSmallFloat128P(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F128);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	BGBCC_CCXL_TypeSmallFComplexP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallFComplexP(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_FCOMPLEX);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	BGBCC_CCXL_TypeSmallDComplexP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallDComplexP(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_DCOMPLEX);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	BGBCC_CCXL_TypeSmallGComplexP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallGComplexP(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_GCOMPLEX);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	BGBCC_CCXL_TypeSmallQuatP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallQuatP(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_QUATF);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	((lty.val==CCXL_TY_VEC3F) ||
		 (lty.val==CCXL_TY_VEC3FQ)) &&
		((rty.val==CCXL_TY_VEC3F) ||
		 (rty.val==CCXL_TY_VEC3FQ)) )
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VEC3F);

		if(opr==CCXL_BINOP_XOR)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
		}

		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, lty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(	((lty.val==CCXL_TY_VEC3D) ||
		 (lty.val==CCXL_TY_VEC3FX)) &&
		((rty.val==CCXL_TY_VEC3D) ||
		 (rty.val==CCXL_TY_VEC3FX)) )
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VEC3D);

		if(opr==CCXL_BINOP_XOR)
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
		}

		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, lty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(lty.val==rty.val)
	{
		if(	(lty.val==CCXL_TY_M64) ||
			(lty.val==CCXL_TY_M128) ||
			(lty.val==CCXL_TY_M128P) ||
			(lty.val==CCXL_TY_VEC2F) ||
			(lty.val==CCXL_TY_VEC3F) ||
			(lty.val==CCXL_TY_VEC4F) ||
			(lty.val==CCXL_TY_QUATF) ||
			(lty.val==CCXL_TY_VEC2D) ||
			(lty.val==CCXL_TY_VEC3D) ||
			(lty.val==CCXL_TY_VEC4D) ||
			(lty.val==CCXL_TY_VEC4H) ||
			(lty.val==CCXL_TY_VEC3H) ||
			(lty.val==CCXL_TY_VEC3H_48) ||
			(lty.val==CCXL_TY_VEC3FQ) ||
			(lty.val==CCXL_TY_VEC3FX) ||
			(lty.val==CCXL_TY_VEC2SI) ||
			(lty.val==CCXL_TY_VEC2UI) ||
			(lty.val==CCXL_TY_VEC4SW) ||
			(lty.val==CCXL_TY_VEC4UW) ||
			(lty.val==CCXL_TY_VEC4SI) ||
			(lty.val==CCXL_TY_VEC4UI) ||
			(lty.val==CCXL_TY_BCD64) ||
			(lty.val==CCXL_TY_BCD128) )
		{
			*rdty=lty;
			
			if(lty.val==CCXL_TY_VEC3FQ)
				rdty->val=CCXL_TY_VEC3F;
			if(lty.val==CCXL_TY_VEC3FX)
				rdty->val=CCXL_TY_VEC3D;

			if(lty.val==CCXL_TY_VEC2F)
			{
				if((opr==CCXL_BINOP_MOD) || (opr==CCXL_BINOP_XOR))
				{
					*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
				}
			}
			else if(lty.val==CCXL_TY_VEC2D)
			{
				if((opr==CCXL_BINOP_MOD) || (opr==CCXL_BINOP_XOR))
				{
					*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
				}
			}
			else if(	(lty.val==CCXL_TY_VEC3F)	||
						(lty.val==CCXL_TY_VEC4F)	||
						(lty.val==CCXL_TY_QUATF)	||
						(lty.val==CCXL_TY_VEC3D)	||
						(lty.val==CCXL_TY_VEC4D)	||
						(lty.val==CCXL_TY_VEC3FQ)	||
						(lty.val==CCXL_TY_VEC3FX)	||
						(lty.val==CCXL_TY_VEC4H)	||
						(lty.val==CCXL_TY_VEC3H)	||
						(lty.val==CCXL_TY_VEC3H_48)	)
			{
				if(opr==CCXL_BINOP_XOR)
				{
					*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
				}
			}
			
			
//			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, lty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}
	}

	if((lty.val==CCXL_TY_UNDEF_I) || (rty.val==CCXL_TY_UNDEF_I))
	{
		/*
		 * Inference with incomplete types.
		 */

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_UNDEF_I);
		return(CCXL_STATUS_NO);
	}

	*rdty=lty;
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_GetTypeCompareBinaryDest(
	BGBCC_TransState *ctx, int opr,
	ccxl_type lty, ccxl_type rty,
	ccxl_type *rdty)
{
	ccxl_type bty;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
		BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
	{
		bty=lty;
		if(	 BGBCC_CCXL_TypeQuadPointerP(ctx, rty) &&
			!BGBCC_CCXL_TypeQuadPointerP(ctx, lty))
				bty=rty;

		*rdty=bty;
		if(BGBCC_CCXL_TypeArrayP(ctx, bty))
			{ BGBCC_CCXL_TypePointerType(ctx, bty, rdty); }
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, lty) &&
		BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, rty))
	{
		return(BGBCC_CCXL_GetTypeBinaryDestB(ctx,
			CCXL_BINOP_ADD, lty, rty, rdty, 1));
	}

	if(BGBCC_CCXL_TypeSmallInt128P(ctx, lty) &&
		BGBCC_CCXL_TypeSmallInt128P(ctx, rty))
	{
		return(BGBCC_CCXL_GetTypeBinaryDestB(ctx,
			CCXL_BINOP_ADD, lty, rty, rdty, 1));
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeSmallFloat128P(ctx, lty) &&
		BGBCC_CCXL_TypeSmallFloat128P(ctx, rty))
	{
		return(BGBCC_CCXL_GetTypeBinaryDestB(ctx,
			CCXL_BINOP_ADD, lty, rty, rdty, 1));
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
//		BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		BGBCC_CCXL_TypeSmallLongP(ctx, rty))
	{
		*rdty=lty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty) &&
//		BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		BGBCC_CCXL_TypeSmallLongP(ctx, lty))
	{
		*rdty=rty;
		return(CCXL_STATUS_YES);
	}
	
	if(BGBCC_CCXL_TypeVariantP(ctx, lty) &&
		BGBCC_CCXL_TypeVariantP(ctx, rty))
	{
		*rdty=lty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, lty) &&
		(BGBCC_CCXL_TypeSmallLongP(ctx, rty) ||
		BGBCC_CCXL_TypeSmallDoubleP(ctx, rty) ||
		BGBCC_CCXL_TypeVarRefP(ctx, rty)))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, rty) &&
		(BGBCC_CCXL_TypeSmallLongP(ctx, lty) ||
		BGBCC_CCXL_TypeSmallDoubleP(ctx, lty) ||
		BGBCC_CCXL_TypeVarRefP(ctx, lty)))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
		return(CCXL_STATUS_YES);
	}

	if(opr==CCXL_CMP_NV)
	{
		if(lty.val==rty.val)
		{
			*rdty=lty;
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			return(CCXL_STATUS_YES);
		}
	}

	if(BGBCC_CCXL_TypePointerP(ctx, lty) &&
		BGBCC_CCXL_TypeFunctionP(ctx, rty))
	{
		*rdty=lty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, rty) &&
		BGBCC_CCXL_TypeFunctionP(ctx, lty))
	{
		*rdty=rty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeBcd64P(ctx, rty) &&
		BGBCC_CCXL_TypeBcd64P(ctx, lty))
	{
		*rdty=rty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeBcd128P(ctx, rty) &&
		BGBCC_CCXL_TypeBcd128P(ctx, lty))
	{
		*rdty=rty;
		return(CCXL_STATUS_YES);
	}

	*rdty=lty;
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}


ccxl_status BGBCC_CCXL_TypeCheckConvImplicit(
	BGBCC_TransState *ctx, int opr,
	ccxl_type dty, ccxl_type sty)
{
	ccxl_type dty2, sty2;
	byte	pred_pvoid;
	byte	pred_pbyte;
	byte	pred_pshort;
	byte	pred_pint;
	byte	pred_plong;

	if(	BGBCC_CCXL_TypeArrayOrPointerP(ctx, dty) &&
		BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, dty, &dty2);
		BGBCC_CCXL_TypeDerefType(ctx, sty, &sty2);
		

		if(dty2.val!=sty2.val)
		{
			pred_pvoid = (dty2.val==0x05) || (sty2.val==0x05);
			pred_pbyte =
				((dty2.val==0x08) && (sty2.val==0x09)) ||
				((dty2.val==0x09) && (sty2.val==0x08));
			pred_pshort =
				((dty2.val==0x0A) && (sty2.val==0x0B)) ||
				((dty2.val==0x0B) && (sty2.val==0x0A));
			pred_pint =
				((dty2.val==0x0C) && (sty2.val==0x00)) ||
				((dty2.val==0x00) && (sty2.val==0x0C));
			pred_plong =
				((dty2.val==0x0D) && (sty2.val==0x01)) ||
				((dty2.val==0x01) && (sty2.val==0x0D));
		}
		
		if((dty2.val!=sty2.val) &&
			!pred_pvoid &&
			!pred_pbyte &&
			!pred_pshort &&
			!pred_pint &&
			!pred_plong)
		{
			BGBCC_CCXL_Warn(ctx, "Implicit Pointer Conversion "
				"%08X->%08X\n", sty2.val, dty2.val);
			return(CCXL_STATUS_YES);
		}
	}
	
	return(CCXL_STATUS_NO);
}

/*
 * Check if we can convert via simply relabeling the types.
 * For integer types, ensures sign extension.
 */
ccxl_status BGBCC_CCXL_TypeCheckConvTransparentP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dbt, sbt, pred;

	if(	BGBCC_CCXL_TypePointerP(ctx, dty) &&
		BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, dty) &&
		!BGBCC_CCXL_TypeQuadPointerP(ctx, sty))
	{
		return(CCXL_STATUS_YES);
	}

//	dbt=BGBCC_CCXL_GetTypeBaseType(ctx, dty);
//	sbt=BGBCC_CCXL_GetTypeBaseType(ctx, sty);
	dbt=dty.val;
	sbt=sty.val;

	pred=CCXL_STATUS_NO;

	switch(dbt)
	{
	case CCXL_TY_UB:
		if(sbt==CCXL_TY_UB)
			pred=CCXL_STATUS_YES;
		break;
	case CCXL_TY_SB:
		if(sbt==CCXL_TY_SB)
			pred=CCXL_STATUS_YES;
		break;

	case CCXL_TY_SS:
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_SB:
		case CCXL_TY_SS:
			pred=CCXL_STATUS_YES; break;
		}
		break;
	case CCXL_TY_US:
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_US:
			pred=CCXL_STATUS_YES; break;
		}
		break;

	case CCXL_TY_I:
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_SB:
		case CCXL_TY_US:
		case CCXL_TY_SS:
		case CCXL_TY_I:
			pred=CCXL_STATUS_YES; break;
		}
		break;

	case CCXL_TY_UI:
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_US:
		case CCXL_TY_UI:
			pred=CCXL_STATUS_YES; break;
		}
		break;

	case CCXL_TY_L:
	case CCXL_TY_NL:
		if(ctx->arch_sizeof_long!=8)
			break;
	
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_SB:
		case CCXL_TY_US:
		case CCXL_TY_SS:
		case CCXL_TY_I:
		case CCXL_TY_UI:
		case CCXL_TY_L:
		case CCXL_TY_NL:
		case CCXL_TY_SI48:
		case CCXL_TY_UI48:
			pred=CCXL_STATUS_YES; break;
		}
		break;

	case CCXL_TY_UL:
	case CCXL_TY_UNL:
		if(ctx->arch_sizeof_long!=8)
			break;

		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_US:
		case CCXL_TY_UI:
		case CCXL_TY_UL:
		case CCXL_TY_UNL:
		case CCXL_TY_UI48:
			pred=CCXL_STATUS_YES; break;
		}
		break;


	case CCXL_TY_SI48:	
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_SB:
		case CCXL_TY_US:
		case CCXL_TY_SS:
		case CCXL_TY_I:
		case CCXL_TY_UI:
		case CCXL_TY_SI48:
			pred=CCXL_STATUS_YES; break;
		}
		break;

	case CCXL_TY_UI48:
		switch(sbt)
		{
		case CCXL_TY_UB:
		case CCXL_TY_US:
		case CCXL_TY_UI:
		case CCXL_TY_UI48:
			pred=CCXL_STATUS_YES; break;
		}
		break;
	}

	return(pred);
}
