#include <bgbccc.h>

bool BGBCC_CCXL_IsRegBasicP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) &&
		((reg.val&CCXL_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegArgBasicP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) &&
		((reg.val&CCXL_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegLocalBasicP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL) &&
		((reg.val&CCXL_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegBaseP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) &&
		((reg.val&CCXL_REGID_REGMASK)<16))
			return(1);
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) &&
		((reg.val&CCXL_REGID_REGMASK)<16))
			return(1);
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL) &&
		((reg.val&CCXL_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegBaseExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) &&
		((reg.val&CCXL_REGID_REGMASK)<256))
			return(1);
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) &&
		((reg.val&CCXL_REGID_REGMASK)<256))
			return(1);
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL) &&
		((reg.val&CCXL_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegRegExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) &&
		((reg.val&CCXL_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegArgExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) &&
		((reg.val&CCXL_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegLocalExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL) &&
		((reg.val&CCXL_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegLocalP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
		return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegTempP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
		return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegArgP(
	BGBCC_TransState *ctx, ccxl_register sreg)
{
	if((sreg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
		return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegGlobalP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
		return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegThisIdxP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_THISIDX)
		return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegThisP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_THISIDX)
	{
		if((reg.val&CCXL_REGID_BASEMASK)==CCXL_REGID_BASEMASK)
			return(1);
		return(0);
	}
	return(0);
}

bool BGBCC_CCXL_IsRegZzP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		if((reg.val&CCXL_REGID_BASEMASK)==CCXL_REGID_BASEMASK)
			return(1);
	}
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		if((reg.val&CCXL_REGID_BASEMASK)==CCXL_REGID_BASEMASK)
			return(1);
	}
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		if((reg.val&CCXL_REGID_BASEMASK)==CCXL_REGID_BASEMASK)
			return(1);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		if((reg.val&CCXL_REGID_REGMASK)==CCXL_REGID_REGMASK)
			return(1);
	}
	return(0);
}

int BGBCC_CCXL_GetRegID(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	return(reg.val&CCXL_REGID_REGMASK);
}

ccxl_type BGBCC_CCXL_MakeTypeVTy(
	BGBCC_TransState *ctx, int id)
{
//	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
//	int i, j, k;
	tty.val=id;
	return(tty);
}

ccxl_type BGBCC_CCXL_MakeTypeID(
	BGBCC_TransState *ctx, int id)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i, j, k;
	
	if(id<0)
		{ BGBCC_DBGBREAK }

	switch(id)
	{
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		break;
	case CCXL_TY_F128:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		break;
	case CCXL_TY_VARIANT:
	case CCXL_TY_VARSTRING:
	case CCXL_TY_VAROBJECT:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_VARIANT;
		break;
	case CCXL_TY_F:
	case CCXL_TY_D:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		break;
	}

	if(id<CCXL_TY_BASETYMAX)
	{
		tty.val=CCXL_TY_TYTY_BASIC|id;
		return(tty);
	}
	
	if(id<CCXL_TYB3_BASETYMAX)
	{
		tty.val=CCXL_TY_TYTY_BASIC3|id;
		return(tty);
	}

	memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));

	ovf.base=id;
	i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
	if(i>0)
	{
		return(tty);
	}

	BGBCC_DBGBREAK
	tty.val=0;
	return(tty);
}

ccxl_type BGBCC_CCXL_MakeTypeID_Arr(
	BGBCC_TransState *ctx, int id, int asz)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i, j, k;
	
	if(id<0)
		{ BGBCC_DBGBREAK }

	switch(id)
	{
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		break;
	case CCXL_TY_F128:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		break;
	case CCXL_TY_VARIANT:
	case CCXL_TY_VARSTRING:
	case CCXL_TY_VAROBJECT:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_VARIANT;
		break;
	case CCXL_TY_F:
	case CCXL_TY_D:
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT_FPU;
		break;
	}

	if((id<CCXL_TY_BASETYMAX) && (asz>=0) && (asz<CCXL_TY_BASEARRMAX))
	{
		tty.val=CCXL_TY_TYTY_BASIC|id|(asz<<CCXL_TY_ARRSHL);
		return(tty);
	}

	if((id<CCXL_TYB2_BASETYMAX) && (asz>=0) && (asz<CCXL_TYB2_BASEARRMAX))
	{
		tty.val=CCXL_TY_TYTY_BASIC2|id|(asz<<CCXL_TYB2_ARRSHL);
		return(tty);
	}
	
	if((id<CCXL_TYB3_BASETYMAX) && (asz>=0) && (asz<CCXL_TYB3_BASEARRMAX))
	{
		tty.val=CCXL_TY_TYTY_BASIC3|id|(asz<<CCXL_TYB3_ARRSHL);
		return(tty);
	}

	memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));

	ovf.base=id;
	ovf.asz[0]=asz;
	ovf.an=1;
	i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
	if(i>0)
	{
		return(tty);
	}

	BGBCC_DBGBREAK
	tty.val=0;
	return(tty);
}

ccxl_type BGBCC_CCXL_GetRegType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	BGBCC_CCXL_LiteralInfo *linf;
	char *str;
	u64 regty;
	ccxl_type tty;
	int i, j;
	
	regty=reg.val&CCXL_REGTY_REGMASK;

#if 1
	if(regty<CCXL_REGTY_IMM_INT)
	{
		if(regty==CCXL_REGTY_GLOBAL)
		{
			i=reg.val&CCXL_REGID_REGMASK;
			if(!i)
			{
				tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
				BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
				return(tty);
			}
			if(i>=ctx->n_reg_globals)
				{ BGBCC_DBGBREAK }

			tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
#if 0
			if(!tty.val)
			{
				tty=ctx->reg_globals[i]->type;
			}
			else
			{
				if(tty.val==CCXL_TY_UNDEF_I)
					tty.val=CCXL_TY_I;
			}
#endif

			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
	//		BGBCC_CCXL_TypeFromSig(ctx, &tty,
	//			ctx->reg_globals[reg.val&16777215]->sig);
	//		tty.val=CCXL_TY_I;
			return(tty);
		}else
		{
			tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			return(tty);
		}
	}else if(regty<CCXL_REGTY_IMM_LITERAL)
	{
		if(regty<CCXL_REGTY_IMM_STRING)
		{
			if(regty==CCXL_REGTY_IMM_INT)
			{
//				tty.val=CCXL_TY_I;
//				tty.val=(reg.val>>32)&0xFFFF;
//				switch(reg.val>>44)
				switch((reg.val>>52)&15)
				{
					case 0: tty.val=CCXL_TY_I ; break;
					case 1: tty.val=CCXL_TY_UI; break;
					case 2: tty.val=CCXL_TY_L ; break;
					case 3: tty.val=CCXL_TY_UL; break;
					case 4: tty.val=CCXL_TY_SB; break;
					case 5: tty.val=CCXL_TY_UB; break;
					case 6: tty.val=CCXL_TY_SS; break;
					case 7: tty.val=CCXL_TY_US; break;
					case 8:
						tty.val=(reg.val>>32)&0xFFFF;
						break;
					default:
						 tty.val=CCXL_TY_I; break;
				}
				return(tty);
			}
			if(regty==CCXL_REGTY_IMM_LONG)
				{ tty.val=CCXL_TY_L; return(tty); }
			if(regty==CCXL_REGTY_IMM_FLOAT)
				{ tty.val=CCXL_TY_F; return(tty); }
			if(regty==CCXL_REGTY_IMM_DOUBLE)
				{ tty.val=CCXL_TY_D; return(tty); }
		}else
		{
			if(regty==CCXL_REGTY_IMM_STRING)
			{
				switch((reg.val>>52)&15)
				{
				case 0:		tty.val=CCXL_TY_SB|CCXL_TY_PTRIDX1; break;
				case 1:		tty.val=CCXL_TY_UB|CCXL_TY_PTRIDX1; break;
				case 2:		tty.val=CCXL_TY_US|CCXL_TY_PTRIDX1; break;
				case 3:		tty.val=CCXL_TY_I |CCXL_TY_PTRIDX1; break;
				
				case 4:
					i=(reg.val>>40)&0xFFF;
					switch((reg.val>>32)&255)
					{
						case 0x00:		j=CCXL_TY_SBITINT; break;
						case 0x01:		j=CCXL_TY_UBITINT; break;
						default:	j=CCXL_TY_SBITINT; break;
					}
					tty=BGBCC_CCXL_MakeTypeID_Arr(ctx, j, i);
					break;
				
				default:	tty.val=CCXL_TY_SB|CCXL_TY_PTRIDX1; break;
				}
				return(tty);
			}

			if(regty==CCXL_REGTY_IMM_WSTRING)
			{
				switch((reg.val>>52)&15)
				{
				case 0:
				case 1:
				case 2:		tty.val=CCXL_TY_US|CCXL_TY_PTRIDX1; break;
				case 3:		tty.val=CCXL_TY_I |CCXL_TY_PTRIDX1; break;
				default:	tty.val=CCXL_TY_US|CCXL_TY_PTRIDX1; break;
				}
				return(tty);
			}

//			if(regty==CCXL_REGTY_IMM_STRING)
//				{ tty.val=CCXL_TY_SB|CCXL_TY_PTRIDX1; return(tty); }
//			if(regty==CCXL_REGTY_IMM_WSTRING)
//				{ tty.val=CCXL_TY_US|CCXL_TY_PTRIDX1; return(tty); }
		}
	}else if(regty<CCXL_REGTY_THISIDX)
	{
		if(regty==CCXL_REGTY_IMM_LONG_LVT)
		{
			if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL)
				{ tty.val=CCXL_TY_UL; return(tty); }
	
			if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_OF1)
				{ tty.val=(reg.val>>32)&0xFFFF; return(tty); }

			tty.val=CCXL_TY_L; return(tty);
		}
		if(regty==CCXL_REGTY_IMM_DOUBLE_LVT)
			{ tty.val=CCXL_TY_D; return(tty); }

		if(regty==CCXL_REGTY_IMM_I128_LVT)
		{
			switch((reg.val>>48)&63)
			{
			case CCXL_REGVEC_TY_I128:	tty.val=CCXL_TY_I128;		break;
			case CCXL_REGVEC_TY_UI128:	tty.val=CCXL_TY_UI128;		break;
			case CCXL_REGVEC_TY_F128:	tty.val=CCXL_TY_F128;		break;
			case CCXL_REGVEC_TY_V2F:	tty.val=CCXL_TY_VEC2F;		break;
			case CCXL_REGVEC_TY_V3F:	tty.val=CCXL_TY_VEC3F;		break;
			case CCXL_REGVEC_TY_V4F:	tty.val=CCXL_TY_VEC4F;		break;
			case CCXL_REGVEC_TY_QUATF:	tty.val=CCXL_TY_QUATF;		break;
			case CCXL_REGVEC_TY_V2D:	tty.val=CCXL_TY_VEC2D;		break;
			case CCXL_REGVEC_TY_FCPX:	tty.val=CCXL_TY_FCOMPLEX;	break;
			case CCXL_REGVEC_TY_DCPX:	tty.val=CCXL_TY_DCOMPLEX;	break;
			case CCXL_REGVEC_TY_V2SI:	tty.val=CCXL_TY_VEC2SI;		break;
			case CCXL_REGVEC_TY_V2UI:	tty.val=CCXL_TY_VEC2UI;		break;
			case CCXL_REGVEC_TY_V4SW:	tty.val=CCXL_TY_VEC4SW;		break;
			case CCXL_REGVEC_TY_V4UW:	tty.val=CCXL_TY_VEC4UW;		break;
			case CCXL_REGVEC_TY_V4SI:	tty.val=CCXL_TY_VEC4SI;		break;
			case CCXL_REGVEC_TY_V4UI:	tty.val=CCXL_TY_VEC4UI;		break;

			case CCXL_REGVEC_TY_V3FX:	tty.val=CCXL_TY_VEC3FX;		break;
			case CCXL_REGVEC_TY_V3FQ:	tty.val=CCXL_TY_VEC3FQ;		break;
			case CCXL_REGVEC_TY_V4H:	tty.val=CCXL_TY_VEC4H;		break;
			case CCXL_REGVEC_TY_V3H:	tty.val=CCXL_TY_VEC3H;		break;

			case CCXL_REGVEC_TY_BCD64:	tty.val=CCXL_TY_BCD64;		break;
			case CCXL_REGVEC_TY_BCD128:	tty.val=CCXL_TY_BCD128;		break;

			default:					tty.val=CCXL_TY_I128;		break;
			}
			return(tty);
		}
		if(regty==CCXL_REGTY_IMM_F128_LVT)
			{ tty.val=CCXL_TY_F128; return(tty); }
	}else
	{
		if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
			{ tty.val=CCXL_TY_L; return(tty); }
		if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
			{ tty.val=CCXL_TY_D; return(tty); }

		if(regty==CCXL_REGTY_THISIDX)
		{
			tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
			if(!tty.val && ((reg.val&0xFFF)==0xFFF))
			{
				if(ctx->cur_func->thisstr)
				{
					linf=BGBCC_CCXL_LookupStructure(ctx,
						ctx->cur_func->thisstr);
//					tty.val=linf->litid+256;
					tty=BGBCC_CCXL_MakeTypeID(ctx,
						linf->litid+256);
					return(tty);
				}
			}
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			return(tty);
		}
	}
#endif

#if 0	
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL))
	{
		tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
		BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
		return(tty);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=reg.val&CCXL_REGID_REGMASK;
		if(i>=ctx->n_reg_globals)
			{ BGBCC_DBGBREAK }
		tty=ctx->reg_globals[i]->type;
		BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
//		BGBCC_CCXL_TypeFromSig(ctx, &tty,
//			ctx->reg_globals[reg.val&16777215]->sig);
//		tty.val=CCXL_TY_I;
		return(tty);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_THISIDX)
	{
		tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
		BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
		return(tty);
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		{ tty.val=CCXL_TY_L; return(tty); }
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		{ tty.val=CCXL_TY_D; return(tty); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		{ tty.val=CCXL_TY_I; return(tty); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		{ tty.val=CCXL_TY_L; return(tty); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		{ tty.val=CCXL_TY_F; return(tty); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		{ tty.val=CCXL_TY_D; return(tty); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
		{ tty.val=CCXL_TY_SB|CCXL_TY_PTRIDX1; return(tty); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
		{ tty.val=CCXL_TY_US|CCXL_TY_PTRIDX1; return(tty); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		{ tty.val=CCXL_TY_L; return(tty); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
		{ tty.val=CCXL_TY_D; return(tty); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
		{ tty.val=CCXL_TY_I128; return(tty); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		{ tty.val=CCXL_TY_F128; return(tty); }
#endif
	
	tty.val=CCXL_TY_UNDEF_I;
	return(tty);
}

ccxl_type BGBCC_CCXL_GetRegStorageType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	BGBCC_CCXL_RegisterInfo *ri;
	int i, j, k;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		i=reg.val&CCXL_REGID_BASEMASK;
		if(i!=CCXL_REGID_BASEMASK)
		{
			ri=ctx->cur_func->regs[i];
			return(ri->type);
		}
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		i=reg.val&CCXL_REGID_BASEMASK;
		if(i!=CCXL_REGID_BASEMASK)
		{
			ri=ctx->cur_func->args[i];
			return(ri->type);
		}
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		i=reg.val&CCXL_REGID_BASEMASK;
		if(i!=CCXL_REGID_BASEMASK)
		{
			ri=ctx->cur_func->locals[i];
			return(ri->type);
		}
	}
	
	return(BGBCC_CCXL_GetRegType(ctx, reg));
}

int BGBCC_CCXL_GetRegAsType(
	BGBCC_TransState *ctx,
	ccxl_register reg, ccxl_type tty, ccxl_register *rtreg)
{
	ccxl_register treg;
	ccxl_type sty;
	int i;

	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL))
	{
		/* Disallow if underlying storage doesn't match. */
//		sty=BGBCC_CCXL_GetRegStorageType(ctx, reg);
//		if(!BGBCC_CCXL_TypeCompatibleStorageP(ctx, sty, tty))
//			return(-1);
	
//		tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
		treg.val=(reg.val&(~CCXL_REGID_TYPEMASK))|
			(((u64)tty.val)<<CCXL_REGID_TYPESHIFT);
		*rtreg=treg;
		return(1);
	}

#if 0
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		*rtreg=reg;

		i=reg.val&CCXL_REGID_REGMASK;
		if(!i)
			{ return(-1); }
		if(i>=ctx->n_reg_globals)
			{ return(-1); }

		sty=ctx->reg_globals[i]->type;

		if(	BGBCC_CCXL_TypeCompatibleP(ctx, tty, sty) ||
			BGBCC_CCXL_TypeCompatibleBothPointerP(ctx, tty, sty))
		{
			sty.val=tty.val;
			if(!tty.val)
				sty.val=CCXL_TY_UNDEF_I;
			treg.val=(reg.val&(~CCXL_REGID_TYPEMASK))|
				(((u64)sty.val)<<CCXL_REGID_TYPESHIFT);
			*rtreg=treg;
			return(1);
		}
	}
#endif

#if 1
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		treg.val=(reg.val&(~CCXL_REGINT_STOXMASK));

		if(tty.val==CCXL_TY_I)
			{ treg.val|=CCXL_REGINT_ST_I; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_UI)
			{ treg.val|=CCXL_REGINT_ST_UI; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_L)
			{ treg.val|=CCXL_REGINT_ST_L; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_UL)
			{ treg.val|=CCXL_REGINT_ST_UL; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_SB)
			{ treg.val|=CCXL_REGINT_ST_SB; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_UB)
			{ treg.val|=CCXL_REGINT_ST_UB; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_SS)
			{ treg.val|=CCXL_REGINT_ST_SS; *rtreg=treg; return(1); }
		if(tty.val==CCXL_TY_US)
			{ treg.val|=CCXL_REGINT_ST_US; *rtreg=treg; return(1); }

		if(tty.val<0xFFFF)
		{
			treg.val|=CCXL_REGINT_ST_OF1|(((u64)tty.val)<<32);
			*rtreg=treg;
			return(1);
		}
	}
#endif

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_THISIDX)
	{
//		tty.val=(reg.val&CCXL_REGID_TYPEMASK)>>CCXL_REGID_TYPESHIFT;
		treg.val=(reg.val&(~CCXL_REGID_TYPEMASK))|
			(((u64)tty.val)<<CCXL_REGID_TYPESHIFT);
		*rtreg=treg;
		return(1);
	}

	*rtreg=reg;
	return(-1);
}

ccxl_type BGBCC_CCXL_GetRegDerefType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type bty, dty;
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_TypeDerefType(ctx, bty, &dty);
	return(dty);
}

ccxl_type BGBCC_CCXL_GetTypeReturnType(
	BGBCC_TransState *ctx, ccxl_type bty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	char *sig;
	ccxl_type dty;
	int i, j, k;

	//HACK
	if(BGBCC_CCXL_TypeFunctionP(ctx, bty))
	{
		i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, bty);
		obj=ctx->literals[i];

		sig=obj->decl->sig;
		sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
		BGBCC_CCXL_TypeFromSig(ctx,
			&dty, sig);
		BGBCC_CCXL_TypeGetTypedefType(ctx, dty, &dty);
		return(dty);
	}

	BGBCC_CCXL_StubWarn(ctx);
	dty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
	return(dty);
}

ccxl_type BGBCC_CCXL_GetRegReturnType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	BGBCC_CCXL_LiteralInfo *obj;
	char *sig;
	ccxl_type bty, dty;
	int i, j, k;

	dty.val=0;

	//HACK
	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		sig=ctx->reg_globals[i]->sig;
		if(*sig=='(')
		{
			sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
			BGBCC_CCXL_TypeFromSig(ctx,
				&dty, sig);
			return(dty);
		}
		
		BGBCC_CCXL_TypeFromSig(ctx,
			&bty, sig);
		BGBCC_CCXL_TypeGetTypedefType(ctx, bty, &bty);

		if(BGBCC_CCXL_TypeFunctionP(ctx, bty))
		{
			i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, bty);
			obj=ctx->literals[i];

			sig=obj->decl->sig;
			sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
			BGBCC_CCXL_TypeFromSig(ctx,
				&dty, sig);
			BGBCC_CCXL_TypeGetTypedefType(ctx, dty, &dty);
			return(dty);
		}

		dty.val=0;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(dty);
	}

	//More HACK
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeFunctionP(ctx, bty))
	{
		i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, bty);
		obj=ctx->literals[i];

		sig=obj->decl->sig;
		sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
		BGBCC_CCXL_TypeFromSig(ctx,
			&dty, sig);
		BGBCC_CCXL_TypeGetTypedefType(ctx, dty, &dty);
		return(dty);
	}

	printf("BGBCC_CCXL_GetRegReturnType: ty=%08X reg=%016llX\n",
		bty.val, reg.val);

	BGBCC_CCXL_StubWarn(ctx);
	dty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);

//	bty=BGBCC_CCXL_GetRegType(ctx, reg);
//	BGBCC_CCXL_TypeDerefType(ctx, bty, &dty);
	return(dty);
}

int BGBCC_CCXL_IsRegGlobalFunctionP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type bty;
	int i, j, k;

	if(!BGBCC_CCXL_IsRegGlobalP(ctx, reg))
		return(0);
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(!BGBCC_CCXL_TypeFunctionP(ctx, bty))
		return(0);

	return(1);
}

ccxl_type BGBCC_CCXL_GetRegPointerToType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type bty, dty;
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_TypePointerType(ctx, bty, &dty);
	return(dty);
}

bool BGBCC_CCXL_IsRegIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeIntP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegLongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeLongP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegSgLongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeSgLongP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegFloatP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegDoubleP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegPointerP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypePointerP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegSmallIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeSmallIntP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegFloat16P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeFloat16P(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegBFloat16P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeBFloat16P(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegVariantP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeVariantP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegVec128P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeVec128P(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegHugePointerP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeHugePointerP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegQuadPointerP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeQuadPointerP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegVolatileP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	int i;
	
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeVolatilePointerP(ctx, tty))
		return(true);

	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		if(ctx->reg_globals[i]->flagsint&BGBCC_TYFL_VOLATILE)
			return(true);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegAliasedP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	int i;
	
//	tty=BGBCC_CCXL_GetRegType(ctx, reg);
//	if(BGBCC_CCXL_TypeVolatilePointerP(ctx, tty))
//		return(true);

	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		if(ctx->reg_globals[i]->regflags&BGBCC_REGFL_ALIASPTR)
			return(true);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImmIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	s64 li;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_I)
			return(true);
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UI)
		{
			li=((u32)reg.val);
			if(((s32)li==li))
				return(true);
			return(false);
		}

		if(	((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_L) ||
			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL) )
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
			return(((s32)li)==li);
		}

		if(	((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_SB) ||
			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UB) ||
			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_SS) ||
			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_US) )
				return(true);

		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmLongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_L)
			return(true);
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL)
			return(true);
		return(false);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmULongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
//		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_L)
//			return(true);
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL)
			return(true);
		return(false);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
//	{
//		return(true);
//	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
	{
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL)
			return(true);
		return(false);
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmUIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	s64 li;

#if 0
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UI)
			return(true);
		return(false);
	}
#endif

#if 1
	if(	((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG) ||
		((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG) )
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		return(((u32)li)==li);
//		return(true);
	}
#endif

	return(false);
}

bool BGBCC_CCXL_IsRegImmUnsignedP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	s64 li;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UI)
		{
			return(true);
		}

		if(
			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL) )
		{
			return(true);
		}

//		if(	((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_SB) ||
//			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UB) ||
//			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_SS) ||
//			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_US) )
//				return(true);

		return(false);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
	{
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UI)
		{
			return(true);
		}

		if(
			((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL) )
		{
			return(true);
		}

		return(false);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImmSmallLongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
//		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_L)
//			return(true);
//		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL)
//			return(true);
		return(true);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	return(false);
}


bool BGBCC_CCXL_IsRegImmFloatP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmDoubleP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmX64P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type	bty;
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
	{
		bty=BGBCC_CCXL_GetRegType(ctx, reg);

		if(BGBCC_CCXL_TypeVec64P(ctx, bty))
			return(true);

		return(false);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
	{
		bty=BGBCC_CCXL_GetRegType(ctx, reg);
		if(BGBCC_CCXL_TypeVec64P(ctx, bty))
			return(true);
		return(false);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImmX128P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt128P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
	{
		if((reg.val&CCXL_REGVEC_STMASK)==CCXL_REGVEC_ST_I128)
			return(true);
		if((reg.val&CCXL_REGVEC_STMASK)==CCXL_REGVEC_ST_UI128)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmFloat128P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmFieldNameP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FIELDNAME)
	{
		if((((reg.val>>52)&15)==0) || (((reg.val>>52)&15)==1))
			return(true);
		return(false);
//		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmStringP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
	{
		if((((reg.val>>52)&15)==0) || (((reg.val>>52)&15)==1))
			return(true);
		return(false);
//		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmU8StringP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
	{
		if(((reg.val>>52)&15)==1)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmWStringP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
	{
		if((((reg.val>>52)&15)==0) || (((reg.val>>52)&15)==2))
			return(true);
		return(false);
//		return(true);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
	{
		if(((reg.val>>52)&15)==2)
			return(true);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImmW4StringP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
	{
		if(((reg.val>>52)&15)==3)
			return(true);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
	{
		if(((reg.val>>52)&15)==3)
			return(true);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImmBigIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
	{
		if(((reg.val>>52)&15)==4)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmLiteralP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmGlobalAddrP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_GBLADDR)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt12P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		if((i>=(-2048)) && (i<2048))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImm12P(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f, g;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, reg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, reg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
			if((i>=(-2048)) && (i<2048))
				return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_EncodeFloat12(f);
		g=BGBCC_DecodeFloat12(i);
		return(f==g);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImm32P(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f, g;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
		{
//			i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
//			if((i>=(-2048)) && (i<2048))
				return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
//		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
//		i=BGBCC_EncodeFloat12(f);
//		g=BGBCC_DecodeFloat12(i);
		g=(float)f;
		return(f==g);
	}

	return(false);
}

#if 0
bool BGBCC_CCXL_IsRegImmIntLit12P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i, j;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS32(ctx, i);
		if((j>=0) && (j<4096))
			return(true);
//		if((i>=(-2048)) && (i<2048))
//			return(true);
	}
	return(false);
}
#endif

bool BGBCC_CCXL_IsRegImmInt16P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		if((i>=(-32768)) && (i<32768))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt24P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		if((i>=(-16777216)) && (i<16777216))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt32P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
//		if((i>=(-16777216)) && (i<16777216))
			return(true);
	}
	return(false);
}


bool BGBCC_CCXL_IsRegNonReachTraceP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL))
			return(true);

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		return(true);
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
//		return(true);
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
//		return(true);
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
//		return(true);
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_GBLADDR)
//		return(true);

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		return(true);

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		return(true);

	return(false);
}


bool BGBCC_CCXL_IsRegImmP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_WSTRING)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_GBLADDR)
		return(true);

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		return(true);

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		return(true);

	return(false);
}

bool BGBCC_CCXL_IsRegImmZeroP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		{ return(!(reg.val&CCXL_REGINT_MASK)); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		{ return(!(reg.val&CCXL_REGLONG_MASK)); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		{ return(!(reg.val&CCXL_REGINT_MASK)); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		{ return(!(reg.val&CCXL_REGLONG_MASK)); }

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		{ return(!(reg.val&CCXL_REGLONG2_MASK)); }
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		{ return(!(reg.val&CCXL_REGLONG2_MASK)); }

	return(false);
}

bool BGBCC_CCXL_IsRegImmILP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmILFDP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
		return(true);

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
		return(true);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
		return(true);

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
		return(true);
	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
		return(true);

	return(false);
}

#if 0
int BGBCC_CCXL_GetRegImmLit12(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	s64 li;
	double lf;
	int i, j;

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS32(ctx, i);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS64(ctx, li);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmFloatValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF32(ctx, lf);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF64(ctx, lf);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

	return(-1);
}
#endif

#if 0
int BGBCC_CCXL_GetRegImmLit12(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	int i;
	i=BGBCC_CCXL_GetRegImmLitIndex(ctx, reg, type);
	if((i>=0) && (i<4096))
		return(i);
	return(-1);
}

int BGBCC_CCXL_GetRegImmLit16(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	int i;
	i=BGBCC_CCXL_GetRegImmLitIndex(ctx, reg, type);
	if((i>=0) && (i<65536))
		return(i);
	return(-1);
}

int BGBCC_CCXL_GetRegImmLit24(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	int i;
	i=BGBCC_CCXL_GetRegImmLitIndex(ctx, reg, type);
	if((i>=0) && (i<16777216))
		return(i);
	return(-1);
}

int BGBCC_CCXL_GetRegImmLitIndex(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	s64 li;
	double lf;
	int i, j;

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS32(ctx, i);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS64(ctx, li);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmFloatValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF32(ctx, lf);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF64(ctx, lf);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmZeroP(ctx, reg))
		{
			return(2);
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, reg))
		{
			j=reg.val&CCXL_REGINT_MASK;
			j=(j<<2)|2;
			return(j);
//			if((j>=0) && (j<65536))
//				return(j);
//			return(-1);
		}

		if(BGBCC_CCXL_IsRegImmWStringP(ctx, reg))
		{
			j=reg.val&CCXL_REGINT_MASK;
			j=(j<<3)|1;
			return(j);
//			if((j>=0) && (j<65536))
//				return(j);
//			return(-1);
		}

		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
		{
			j=reg.val&CCXL_REGINT_MASK;
			j=(j<<2)|0;
			return(j);
		}

		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_GBLADDR)
		{
			j=reg.val&CCXL_REGINT_MASK;
			j=(j<<3)|5;
			return(j);
		}
		
		return(-1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmZeroP(ctx, reg))
		{
			return(2);
		}

		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
		{
			j=reg.val&CCXL_REGINT_MASK;
			j=(j<<2)|0;
			return(j);
		}
		
		return(-1);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmZeroP(ctx, reg))
		{
			return(2);
		}

		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LITERAL)
		{
			j=reg.val&CCXL_REGINT_MASK;
			j=(j<<2)|0;
			return(j);
		}
		
		return(-1);
	}

	return(-1);
}
#endif

int BGBCC_CCXL_GetRegImmIntValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	double dv;
	s64 v;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		{ return((int)(reg.val&CCXL_REGINT_MASK)); }

	if(	((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT) ||
		((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG) )
	{
		v=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		if(((s32)v)!=v)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Long to Int\n");
//			BGBCC_DBGBREAK
		}
		
		return(v);
	
//		return((int)(reg.val&CCXL_REGINT_MASK));
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
//		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
	{
		dv=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		if(((s32)dv)!=dv)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Real to Int\n");
//			BGBCC_DBGBREAK
		}
		
		return((s32)dv);

//		BGBCC_DBGBREAK
//		return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
	}

	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT) ||
		((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE))
	{
		dv=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		if(((s32)dv)!=dv)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Real to Int\n");
//			BGBCC_DBGBREAK
		}
		
		return((s32)dv);
	
//		return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
	}

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
//		{ return((int)(reg.val&CCXL_REGINT_MASK)); }
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
//		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
//		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }

//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
//		{ return(BGBCC_CCXL_GetRegImmLongValue(ctx, reg)); }
//	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
//		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }

//	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
//		{ return(BGBCC_CCXL_GetRegImmLongValue(ctx, reg)); }
//	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
//		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }

	__debugbreak();

	return(0);
}

s64 BGBCC_CCXL_GetRegImmLongValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	double dv;
	int i;
	s64 v;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
//		return((int)(reg.val&CCXL_REGINT_MASK));

		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_I)
			return((s32)(reg.val&CCXL_REGINT_MASK));
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UI)
			return((u32)(reg.val&CCXL_REGINT_MASK));
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_L)
			return((s32)(reg.val&CCXL_REGINT_MASK));
		if((reg.val&CCXL_REGINT_STMASK)==CCXL_REGINT_ST_UL)
			return((u32)(reg.val&CCXL_REGINT_MASK));

		if((reg.val&CCXL_REGINT_STOFPMASK)==CCXL_REGINT_ST_OF_PTR)
			return((u32)(reg.val&CCXL_REGINT_MASK));

		return((s32)(reg.val&CCXL_REGINT_MASK));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
	{
		v=reg.val&CCXL_REGLONG_MASK;
		if(v&((~CCXL_REGLONG_MASK)>>1))
			v|=~CCXL_REGLONG_MASK;
		return(v);
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	{
		dv=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		if(((s64)dv)!=dv)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Real to Long\n");
//			BGBCC_DBGBREAK
		}
		
		return((s64)dv);

//		BGBCC_DBGBREAK
//		return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
	}
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	{
		dv=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		if(((s64)dv)!=dv)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Real to Long\n");
//			BGBCC_DBGBREAK
		}
		
		return((s64)dv);

//		BGBCC_DBGBREAK
//		return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
	}
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
	{
		dv=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		if(((s64)dv)!=dv)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Real to Long\n");
//			BGBCC_DBGBREAK
		}
		
		return((s64)dv);

//		BGBCC_DBGBREAK
//		return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
	{
		i=reg.val&CCXL_REGINT_MASK;
		v=ctx->ctab_lvt8[i];
		return(v);
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
	{
		v=reg.val&CCXL_REGLONG2_MASK;
		if(v&((~CCXL_REGLONG2_MASK)>>1))
			v|=~CCXL_REGLONG2_MASK;
		return(v);
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
	{
		dv=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		if(((s64)dv)!=dv)
		{
			BGBCC_CCXL_Warn(ctx, "Constant truncation from Real to Long\n");
//			BGBCC_DBGBREAK
		}
		
		return((s64)dv);

//		return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
	}

	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT))
	{
		i=reg.val&CCXL_REGINTPL_MASK;
		v=ctx->ctab_lvt8[i];
		return(v);
	}

	__debugbreak();

	return(0);
}

int BGBCC_CCXL_GetRegImmX128Value(
	BGBCC_TransState *ctx, ccxl_register reg,
	s64 *rval_lo, s64 *rval_hi)
{
	s64 vl, vh;
	int i, j, k;

	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT))
	{
		i=reg.val&CCXL_REGINTPL_MASK;
		j=(reg.val>>CCXL_REGINTPH_SHL)&CCXL_REGINTPL_MASK;
		vl=ctx->ctab_lvt8[i];
		vh=ctx->ctab_lvt8[j];
		*rval_lo=vl;
		*rval_hi=vh;
		return(1);
	}
	
	vl=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
	*rval_lo=vl;
	*rval_hi=vl>>63;
	return(1);
}

int BGBCC_CCXL_GetRegImmInt128Value(
	BGBCC_TransState *ctx, ccxl_register reg,
	s64 *rval_lo, s64 *rval_hi)
{
	s64 vl, vh;
	int i, j, k;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_I128_LVT)
	{
		i=BGBCC_CCXL_GetRegImmX128Value(ctx,
			reg, rval_lo, rval_hi);
		return(i);
	}

	vl=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
	*rval_lo=vl;
	*rval_hi=vl>>63;
	return(1);
}

int BGBCC_CCXL_GetRegImmFloat128Value(
	BGBCC_TransState *ctx, ccxl_register reg,
	s64 *rval_lo, s64 *rval_hi)
{
	u64 vf, vh, vl;
	double f;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_F128_LVT)
		return(BGBCC_CCXL_GetRegImmX128Value(ctx, reg, rval_lo, rval_hi));
	
	f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
	vf=*(u64 *)(&f);
	
	vh=((vf&0x7FFFFFFFFFFFFFFFLL)>>4)+
		(((u64)(16383-1023))<<48)+
		(vf&0x8000000000000000LL);
	vl=vf<<60;

	*rval_lo=vl;
	*rval_hi=vh;
	return(1);
}

int BGBCC_CCXL_GetRegImmBigIntValue(
	BGBCC_TransState *ctx, ccxl_register reg,
	u64 *rvala, int cnt)
{
	u64 tv;
	char *str, *s;
	int i, j, n, n1;
	
	str=BGBCC_CCXL_GetRegImmStringValue(ctx, reg);
	
	n1=strlen(str)/11;
	n=n1;
	if(cnt<n)
		n=cnt;

	s=str; tv=0;
	for(i=0; i<n; i++)
	{
		tv=	(((u64)(s[ 0]-'0'))<< 0) |
			(((u64)(s[ 1]-'0'))<< 6) |
			(((u64)(s[ 2]-'0'))<<12) |
			(((u64)(s[ 3]-'0'))<<18) |
			(((u64)(s[ 4]-'0'))<<24) |
			(((u64)(s[ 5]-'0'))<<30) |
			(((u64)(s[ 6]-'0'))<<36) |
			(((u64)(s[ 7]-'0'))<<42) |
			(((u64)(s[ 8]-'0'))<<58) |
			(((u64)(s[ 9]-'0'))<<54) |
			(((u64)(s[10]-'0'))<<60) ;
		rvala[i]=tv;
		s+=11;
	}

	tv=~((tv>>63)-1);
	for(i=n; i<cnt; i++)
	{
		rvala[i]=tv;
	}
	return(0);
}

double BGBCC_CCXL_GetRegImmFloatValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
}

double BGBCC_CCXL_GetRegImmDoubleValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	s64 li;
	u64 fl;
	u32 fi;
	int i, j, k;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		{ return((int)(reg.val&CCXL_REGINT_MASK)); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		{ return(BGBCC_CCXL_GetRegImmLongValue(ctx, reg)); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	{
		fi=reg.val&CCXL_REGINT_MASK;
		return(*(float *)(&fi));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	{
		fl=reg.val&CCXL_REGLONG_MASK;
		fl=fl<<8;
		return(*(double *)(&fl));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG_LVT)
	{
		i=reg.val&CCXL_REGINT_MASK;
		fl=ctx->ctab_lvt8[i];
		return((double)((s64)fl));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE_LVT)
	{
		i=reg.val&CCXL_REGINT_MASK;
		fl=ctx->ctab_lvt8[i];
		return(*(double *)(&fl));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
	{
		fl=reg.val&CCXL_REGLONG2_MASK;
		fl=fl<<3;
		return(*(double *)(&fl));
	}

	return(0);
}

char *BGBCC_CCXL_GetRegImmStringValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	u64 fl;
	u32 fi;

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_STRING)
		{ return(ctx->strtab+(reg.val&CCXL_REGINT_MASK)); }

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FIELDNAME)
		{ return(ctx->strtab+(reg.val&CCXL_REGINT_MASK)); }

	return(NULL);
}

int BGBCC_CCXL_GetRegImm12Value(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		return(i&4095);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_EncodeFloat12(f);
		return(i);
	}

#if 0
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
		{ return(((int)(reg.val&CCXL_REGINT_MASK))&4095); }
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
		{ return(((int)(reg.val&CCXL_REGINT_MASK))&4095); }

	if(((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT) ||
		((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_EncodeFloat12(f);
		return(i&4095);
	}
#endif

	return(0);
}

int BGBCC_CCXL_GetRegImm32Value(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f;
	float f2;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		return(i);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
//		i=BGBCC_EncodeFloat12(f);
		f2=f;
		i=*(int *)(&f2);
		
		return(i);
	}

	return(0);
}

ccxl_status BGBCC_CCXL_GetRegForSmallIntValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val, int stv)
{
	ccxl_register treg;
	
//	if((stv>8) && ((stv&0xFFFF)==stv))
	if((stv>=16) && ((stv&0xFFFF)==stv))
	{
		treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT|
			CCXL_REGINT_ST_OF1|(((u64)stv)<<32);
		*rreg=treg;
		return(CCXL_STATUS_YES);
	}
	
//	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT;
	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT|
//		(((u64)(stv&15))<<44);
		(((u64)(stv&15))<<52);
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForIntValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val)
{
	ccxl_register treg;
	
//	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT;
	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT|CCXL_REGINT_ST_I;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForLongValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s64 val)
{
	ccxl_register treg;
	int i, j, k;
	
//	if(((val<<8)>>8)!=val)
	if(((val<<3)>>3)!=val)
	{
		i=BGBCC_CCXL_IndexLitS64(ctx, val);
		treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_LONG_LVT;
		*rreg=treg;
		return(CCXL_STATUS_YES);
	}
	
//	treg.val=(val&CCXL_REGLONG_MASK)|CCXL_REGTY_IMM_LONG;
	treg.val=(val&CCXL_REGLONG2_MASK)|CCXL_REGTY2_IMM_LONG;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForUIntValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, u32 val)
{
	ccxl_register treg;
	
//	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT;
	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT|CCXL_REGINT_ST_UI;
	*rreg=treg;
	return(CCXL_STATUS_YES);

//	return(BGBCC_CCXL_GetRegForLongValue(ctx, rreg, val));
}

ccxl_status BGBCC_CCXL_GetRegForULongValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s64 val)
{
	ccxl_register treg;
	int i, j, k;

	if(((u32)val)==val)
	{
		treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_INT|CCXL_REGINT_ST_UL;
		*rreg=treg;
		return(CCXL_STATUS_YES);
	}
	
	i=BGBCC_CCXL_IndexLitS64(ctx, val);
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_LONG_LVT|CCXL_REGINT_ST_UL;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForFloatValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, f32 val)
{
	ccxl_register treg;
	
	treg.val=((*(u32 *)(&val))&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_FLOAT;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForDoubleValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, f64 val)
{
	u64 uli;
	ccxl_register treg;
	
//	treg.val=(((*(u64 *)(&val))>>8)&CCXL_REGLONG_MASK)|CCXL_REGTY_IMM_DOUBLE;
//	treg.val=(((*(u64 *)(&val))>>3)&CCXL_REGLONG2_MASK)|CCXL_REGTY2_IMM_DOUBLE;

	uli=*(u64 *)(&val); uli=(uli+3)>>3;
	treg.val=(uli&CCXL_REGLONG2_MASK)|CCXL_REGTY2_IMM_DOUBLE;

	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForInt128Value(
	BGBCC_TransState *ctx, ccxl_register *rreg,
	s64 val_lo, s64 val_hi)
{
	ccxl_register treg;
	int i, j, k;
	
	i=BGBCC_CCXL_IndexLitS64(ctx, val_lo);
	j=BGBCC_CCXL_IndexLitS64(ctx, val_hi);
	treg.val=
		(i&CCXL_REGINTPL_MASK)|
		((j&CCXL_REGINTPL_MASK)<<CCXL_REGINTPH_SHL)|
		CCXL_REGTY_IMM_I128_LVT;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForFloat128Value(
	BGBCC_TransState *ctx, ccxl_register *rreg,
	s64 val_lo, s64 val_hi)
{
	ccxl_register treg;
	int i, j, k;
	
	i=BGBCC_CCXL_IndexLitS64(ctx, val_lo);
	j=BGBCC_CCXL_IndexLitS64(ctx, val_hi);
	treg.val=
		(i&CCXL_REGINTPL_MASK)|
		((j&CCXL_REGINTPL_MASK)<<CCXL_REGINTPH_SHL)|
		CCXL_REGTY_IMM_F128_LVT;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForX128Value(
	BGBCC_TransState *ctx, ccxl_register *rreg,
	s64 val_lo, s64 val_hi, int vty)
{
	ccxl_register treg;
	int i, j, k;
	
	i=BGBCC_CCXL_IndexLitS64(ctx, val_lo);
	j=BGBCC_CCXL_IndexLitS64(ctx, val_hi);
	treg.val=
		(i&CCXL_REGINTPL_MASK)|
		((j&CCXL_REGINTPL_MASK)<<CCXL_REGINTPH_SHL)|
		((vty&63LL)<<CCXL_REGINTTY_SHL)|
		CCXL_REGTY_IMM_I128_LVT;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}


ccxl_status BGBCC_CCXL_GetRegForStringValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	if(i<0)
		{ BGBCC_DBGBREAK }
	
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_STRING;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForWStringValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_WSTRING;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForU8StringValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	if(i<0)
		{ BGBCC_DBGBREAK }
	
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_STRING|(1LL<<52);
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForW4StringValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	if(i<0)
		{ BGBCC_DBGBREAK }
	
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_WSTRING|(3LL<<52);
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForBigIntValueStr(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str, int vty)
{
	ccxl_register treg;
	int i, j;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	if(i<0)
		{ BGBCC_DBGBREAK }
	
	j=strlen(str)/11;
	j=(j+1)/2;
	
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_STRING|
		(4LL<<52)|(((u64)j)<<40)|(((u64)vty)<<32);

	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForBigIntValueN(
	BGBCC_TransState *ctx, ccxl_register *rreg,
	u64 *rval, int n, int vty)
{
	char tb[512];
	ccxl_register treg;
	u64 tv;
	char *t;
	int i, j;
	
	t=tb;
	for(i=0; i<n; i++)
	{
		tv=rval[i];
		t[ 0]='0'+((tv>> 0)&63);
		t[ 1]='0'+((tv>> 6)&63);
		t[ 2]='0'+((tv>>12)&63);
		t[ 3]='0'+((tv>>18)&63);
		t[ 4]='0'+((tv>>24)&63);
		t[ 5]='0'+((tv>>30)&63);
		t[ 6]='0'+((tv>>36)&63);
		t[ 7]='0'+((tv>>42)&63);
		t[ 8]='0'+((tv>>48)&63);
		t[ 9]='0'+((tv>>54)&63);
		t[10]='0'+((tv>>60)&63);
		t+=11;
	}
	*t=0;
	
	i=BGBCC_CCXL_GetRegForBigIntValueStr(ctx, rreg, tb, vty);
	return(i);
//	*rreg=treg;
//	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForFieldIdValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	if(i<0)
		{ BGBCC_DBGBREAK }
	
	treg.val=(i&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_FIELDNAME;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForLiteralValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val)
{
	ccxl_register treg;
	
	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_LITERAL;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForGlobalAddrValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val)
{
	ccxl_register treg;
	
	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_GBLADDR;
	*rreg=treg;
	return(CCXL_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForGlobalAddrValueDisp(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val, int disp)
{
	ccxl_register treg;
	
	treg.val=(val&CCXL_REGINT_MASK)|CCXL_REGTY_IMM_GBLADDR;
	treg.val|=CCXL_REGGBLA_ST_D|(((u64)(disp&0xFFFFF))<<32);
	*rreg=treg;
	return(CCXL_STATUS_YES);
}


#if 0
// int BGBCC_CCXL_GetRegOprMode1(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
		return(CCXL_OPMODE_RR);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
		return(CCXL_OPMODE_RA);
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
		return(CCXL_OPMODE_RL);
	return(-1);
}

// int BGBCC_CCXL_GetRegOprMode2(
	BGBCC_TransState *ctx, ccxl_register rega, ccxl_register regb)
{
	if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE_RR);
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE_RA);
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE_RL);
	}

	if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE_AR);
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE_AA);
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE_AL);
	}

	if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE_LR);
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE_LA);
		if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE_LL);
	}

	return(-1);
}

// int BGBCC_CCXL_GetRegOprMode3(
	BGBCC_TransState *ctx, ccxl_register dst,
	ccxl_register rega, ccxl_register regb)
{
	if((dst.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
		{ return(BGBCC_CCXL_GetRegOprMode2(ctx, rega, regb)); }

	if((dst.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
		{
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
				return(CCXL_OPMODE_ARR);
		}
	}

	if((dst.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
		{
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
				return(CCXL_OPMODE_LRR);
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
				return(CCXL_OPMODE_LRA);
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
				return(CCXL_OPMODE_LRL);
		}

		if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
		{
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
				return(CCXL_OPMODE_LAR);
		}

		if((rega.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
		{
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
				return(CCXL_OPMODE_LLR);
			if((regb.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
				return(CCXL_OPMODE_LLL);
		}
	}

	return(-1);
}

// int BGBCC_CCXL_GetRegOprModeW(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE2_IR);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE2_IA);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE2_IL);
	}
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE2_LR);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE2_LA);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE2_LL);
	}
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE2_FR);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE2_FA);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE2_FL);
	}
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE2_DR);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE2_DA);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE2_DL);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
	{
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE2_PR);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE2_PA);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE2_PL);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
			return(CCXL_OPMODE2_PR);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
			return(CCXL_OPMODE2_PA);
		if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
			return(CCXL_OPMODE2_PL);
	}
	return(-1);
}
#endif

int BGBCC_CCXL_GetTypeOperationZ(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(CCXL_TY_P);
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(CCXL_TY_P);

	if(BGBCC_CCXL_TypeRefArrayP(ctx, ty))
		return(CCXL_TY_V);

	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<16)
		return(i);
	
	if(i==CCXL_TY_VARIANT)
		return(CCXL_TY_V);

	if(i==CCXL_TY_F16)
		return(CCXL_TY_F);
	if(i==CCXL_TY_BF16)
		return(CCXL_TY_F);
	
	if(BGBCC_CCXL_TypeFatArrayP(ctx, ty))
		return(CCXL_TY_FATP_AREF);
	if(BGBCC_CCXL_TypeMethodPointerP(ctx, ty))
		return(CCXL_TY_FATP_VMTH);

	if(i==CCXL_TY_VARSTRING)
		return(CCXL_TY_V);
	if(i==CCXL_TY_VAROBJECT)
		return(CCXL_TY_V);
	
	if(BGBCC_CCXL_TypeVec64P(ctx, ty))
		return(CCXL_TY_M64);
	if(BGBCC_CCXL_TypeVec128P(ctx, ty))
		return(CCXL_TY_M128);
	
	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
		return(CCXL_TY_S);

	return(-1);
//	return(CCXL_TY_UNDEF_I);
}

int BGBCC_CCXL_GetTypeOperationExtZ(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(CCXL_TY_P);
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(CCXL_TY_P);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
		return(CCXL_TY_S);
	
	if(BGBCC_CCXL_TypeRefArrayP(ctx, ty))
		return(CCXL_TY_V);

	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
		return(i);
	
	if(i==CCXL_TY_VARIANT)
		return(CCXL_TY_V);
	
	return(-1);
//	return(CCXL_TY_UNDEF_I);
}

int BGBCC_CCXL_GetTypeOperationBaseZ(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int z, z1;

	z=BGBCC_CCXL_GetTypeOperationZ(ctx, ty);
	switch(z)
	{
	case CCXL_TY_I:		case CCXL_TY_UI:
	case CCXL_TY_SB:	case CCXL_TY_UB:
	case CCXL_TY_SS:	case CCXL_TY_US:
		z1=CCXL_TY_I; break;
	case CCXL_TY_L:		case CCXL_TY_UL:
	case CCXL_TY_V:
		z1=CCXL_TY_L; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		if(ctx->arch_sizeof_long==4)
			{ z1=CCXL_TY_I; break; }
		z1=CCXL_TY_L; break;

	case CCXL_TY_F:		case CCXL_TY_D:
		z1=z; break;
	case CCXL_TY_P:		case CCXL_TY_S:
		z1=CCXL_TY_P; break;
	
	case CCXL_TY_F16:
	case CCXL_TY_BF16:
		z1=CCXL_TY_F; break;
	
	case CCXL_TY_FATP:
	case CCXL_TY_FATP_AREF:
	case CCXL_TY_FATP_VMTH:
		z1=CCXL_TY_FATP;
		break;
		
	case CCXL_TY_M64:
		z1=CCXL_TY_M64;
		break;
	case CCXL_TY_M128:
		z1=CCXL_TY_M128;
		break;
	
	default:
		z1=CCXL_TY_P; break;
//		z1=z; break;
	}
	return(z1);
}
