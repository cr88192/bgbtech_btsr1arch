int BGBCC_SHX_EmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;
	int rm3, rn3;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;

#if 1
	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC100|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}
#endif

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(ctx->is_betav)
			{ opw=0x2003|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x2001|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			if(opw1==0xCC00)
				{ __debugbreak(); }
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw1|=8;
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
//			opw1|=1;
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
//			opw1|=1;
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw1&=~8;
//			opw1&=~8;	opw1|=1;
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			opw=opw1; opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
		
		if(ctx->no_ext32)
			opw=-1;
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0040;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1; opw2=0x2003|((rn&15)<<8)|((rm&15)<<4); break;

#if 1
		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
//			opw=opw1;
			opw=(opw1&0xFF0F)|((opw1&0x00C0)>>1);
			opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;
#endif

		default:
			break;
		}

		if(ctx->no_ext32)
			opw=-1;
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;
	int rm3, rn3;

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;

#if 1
	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC108|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}
#endif

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6000|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x6001|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6002|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x6003|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x6001|((rn&15)<<8)|((rm&15)<<4);		break;

//	case BGBCC_SH_NMID_SWAPB:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
//		opw=0x6008|((rn&15)<<8)|((rm&15)<<4);		break;
//	case BGBCC_SH_NMID_SWAPW:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
//		opw=0x6009|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF008|((rn&15)<<8)|((rm&15)<<4);		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00;
			opw2=0x600C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00;
			opw2=0x600D|((rn&15)<<8)|((rm&15)<<4);
			break;
		default:
			break;
		}

		if(ctx->no_ext32)
			opw=-1;
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			opw=opw1; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1|8; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw1|=1;
			opw=opw1; opw2=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw1|=1;
			opw=opw1; opw2=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw1&=~8;	opw1|=1;
			opw=opw1; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=opw1&(~8); opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);	break;

		default:
			break;
		}

		if(ctx->no_ext32)
			opw=-1;
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0040;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=opw1; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1; opw2=0x6003|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw=opw1; opw2=0x600C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw=opw1; opw2=0x600D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw=opw1; opw2=0x600E|((rn&15)<<8)|((rm&15)<<4); break;

#if 1
		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=(opw1&0xFF0F)|((opw1&0x00C0)>>1);
			opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

		default:
			break;
		}

		if(ctx->no_ext32)
			opw=-1;
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStDecReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpRegStDecReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STSL:
	case BGBCC_SH_NMID_STCL:

		if(ctx->no_fpu)
		{
			if((rm==BGBCC_SH_REG_FPUL) ||
				(rm==BGBCC_SH_REG_FPSCR) ||
				(rn==BGBCC_SH_REG_FPUL) ||
				(rn==BGBCC_SH_REG_FPSCR))
					break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((rm&0xF0)==BGBCC_SH_REG_CR0)
			{ opw=0x4003|((rn&15)<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==BGBCC_SH_REG_SR0)
			{ opw=0x4002|((rn&15)<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x2005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(ctx->is_betav)
			{ opw=0x2007|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x2005|((rn&15)<<8)|((rm&15)<<4);		break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0xF00B|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x2004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x2005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			opw=opw1; opw2=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1|8; opw2=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0020;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=(opw1&(~0x80))|0x00;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=(opw1&(~0x80))|0x01;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=(opw1&(~0x80))|0x02;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw1&(~0x80))|0x03;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_LDSL:
	case BGBCC_SH_NMID_LDCL:
		if(ctx->no_fpu)
		{
			if((rm==BGBCC_SH_REG_FPUL) ||
				(rm==BGBCC_SH_REG_FPSCR) ||
				(rn==BGBCC_SH_REG_FPUL) ||
				(rn==BGBCC_SH_REG_FPSCR))
					break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if((rn&0xF0)==BGBCC_SH_REG_CR0)
			{ opw=0x4007|((rm&15)<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==BGBCC_SH_REG_SR0)
			{ opw=0x4006|((rm&15)<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6004|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x6005|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6006|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x6007|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x6005|((rn&15)<<8)|((rm&15)<<4);		break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF009|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			opw=opw1; opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1|8; opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw1|=1;
			opw=opw1; opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw1|=1;
			opw=opw1; opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw1&=~8;	opw1|=1;
			opw=opw1; opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0020;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=(opw1&(~0x80))|0x04;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=(opw1&(~0x80))|0x05;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=(opw1&(~0x80))|0x06;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw1&(~0x80))|0x07;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStRegDisp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	if(!BGBCC_SHX_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpRegStRegDisp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpRegStRegDisp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw, opw1, opw2, opw3;
	int i;

	if(disp==0)
	{
		i=0;
		if((nmid==BGBCC_SH_NMID_MOVQ) && ((rn&31)==15) &&
				ctx->has_bjx1r3mov)
			i=1;
	
		if(!i)
			return(BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(ctx->is_betav)
			break;

		if(rm&31)					break;
//		if(rm!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rn==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(disp>=256)			break;
			opw=0xC000|disp;		break;
		}

//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		
		if(disp>=16)				break;
		opw=0x8000|((rn&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(ctx->is_betav)
			break;

		if(rm&31)					break;
//		if(rm!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rn==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if((disp>>1)>=256)		break;
			opw=0xC100|disp;		break;
		}

//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))			break;

		if((disp>>1)>=16)			break;
		opw=0x8100|((rn&15)<<4)|(disp>>1);
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if((disp&3) || (disp<0))	break;
		if(rn==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(rm!=BGBCC_SH_REG_R0)	break;
			if((disp>>2)>=256)		break;
			opw=0xC200|disp;		break;
		}

//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((disp>>2)>=16)			break;
		opw=0x1000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(ctx->is_betav)
			break;

		if(rm&31)					break;
		if((disp&7) || (disp<0))	break;
		if((disp>>3)>=16)			break;

		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))			break;

		opw=0x8100|((rn&15)<<4)|(disp>>3);
		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov && (ctx->is_simpass!=1))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVL:
			if(!ctx->has_bjx1r3mov)
				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)< 16)			break;
			if((disp>>2)>=32)			break;
			if(rn!=15)					break;
#if 1
			if(rm==0)
			{
				opw=0x8370|((disp>>2)&15);
				break;
			}
#endif
//			if((rm<8) || (rm>14))		break;
			if(((rm&31)<8) || ((rm&31)>14))	break;
			opw=0x8300|((rm&7)<<4)|((disp>>2)&15);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if(!ctx->has_bjx1r3mov)
				break;
			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))	break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if((disp&3) || (disp<0))			break;
			if(rn!=15)							break;
			if(((rm&15)<8) || ((rm&15)>15))		break;
			if((disp>>2)<16)
				{ opw=0x8600|((rm&7)<<4)|((disp>>2)&15); break; }
			break;
		}
	}

	if((opw<0) && ctx->is_addr64)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVQ:
			if(!ctx->is_addr64)			break;
			if((disp&7) || (disp<0))	break;
			if((disp>>3)>=16)			break;
			if((rn!=BGBCC_SH_REG_R15) && (rn!=BGBCC_SH_REG_RQ15))
				break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			opw=0xC000|((rm&15)<<4)|((disp>>3)&15);
			break;

		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_movi20)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(disp<0)					break;
			if(disp>=4096)				break;

//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x0000|((disp)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (disp<0))	break;
			if((disp>>1)>=4096)			break;

//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x1000|((disp>>1)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;

//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x2000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVS:
			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x3000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVD:
//			if(!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
//				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>3)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x3000|((disp>>3)&0x0FFF);
			break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(((sbyte)disp)!=disp)		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x2000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x2001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((sbyte)(disp>>3))!=(disp>>3)))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>3)&255);
			opw2=0x2003|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(ctx->is_mergece)
			{
				opw=0x8E00|((disp>>2)&255);
				opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2004|((rn&15)<<8)|((rm&15)<<4);
			break;

//		case BGBCC_SH_NMID_FMOVS:
//			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
//			opw2=0x7000|((disp>>2)&0x0FFF);
//			break;
		default:
			break;
		}

		if(ctx->no_ext32)
		{
//			__debugbreak();
//			opw=-1;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw3=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
#if 1
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((disp&63)!=disp)							break;
			opw=opw3|((disp)&63);
			opw2=0x2000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((disp>>1)&63)!=(disp>>1)))	break;
			opw=opw3|((disp>>1)&63);
			opw2=0x2001|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (((disp>>2)&63)!=(disp>>2)))	break;
			opw=opw3|((disp>>2)&63);
			opw2=0x2002|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_MOVQ:
//			BGBCC_DBGBREAK
			if((disp&7) || (((disp>>3)&63)!=(disp>>3)))	break;
			opw=opw3|((disp>>3)&63);
			opw2=0x2003|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((disp>>2)&15)!=(disp>>2)))	break;
			opw=(opw3&0xFF00)|((opw3&0x00C0)>>1)|((disp>>2)&15);
			opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw, opw1, opw2, opw3;
	int i;

//	if(disp==0)
	if((disp==0) &&
		(nmid!=BGBCC_SH_NMID_MOVUB) &&
		(nmid!=BGBCC_SH_NMID_MOVUW))
	{
		i=0;
		if((nmid==BGBCC_SH_NMID_MOVQ) && ((rm&31)==15))
			i=1;
	
		if(!i)
			return(BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(ctx->is_betav)
			break;

		if(rn&31)					break;
//		if(rn!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(disp>=256)			break;
			opw=0xC400|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		
		if(disp>=16)				break;
		opw=0x8400|((rm&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
		if(ctx->is_betav)
			break;

		if(rn&31)					break;
//		if(rn!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if((disp>>1)>=256)		break;
			opw=0xC500|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))				break;

		if((disp>>1)>=16)			break;
		opw=0x8500|((rm&15)<<4)|(disp>>1);
		break;

	case BGBCC_SH_NMID_MOVL:
		if((disp&3) || (disp<0))	break;
		if(rm==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(rm!=BGBCC_SH_REG_R0)	break;
			if((disp>>2)>=256)		break;
			opw=0xC600|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;

		if((disp>>2)>=16)			break;
		opw=0x5000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(ctx->is_betav)
			break;

		if(rn&31)					break;
		if((disp&7) || (disp<0))	break;
		if((disp>>3)>=16)			break;

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))			break;

		opw=0x8500|((rm&15)<<4)|(disp>>3);
		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov && (ctx->is_simpass!=1))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVL:
			if(!ctx->has_bjx1r3mov)
				break;

			if((disp&3) || (disp<0))	break;
			if((disp>>2)< 16)			break;
			if((disp>>2)>=32)			break;
			if(rm!=15)					break;
#if 1
			if(rn==0)
			{
				opw=0x83F0|((disp>>2)&15);
				break;
			}
#endif			
			if(((rn&31)<8) || ((rn&31)>14))	break;
			opw=0x8380|((rn&7)<<4)|((disp>>2)&15);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if(!ctx->has_bjx1r3mov)
				break;

			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))	break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			if((disp&3) || (disp<0))			break;
			if(rm!=15)							break;
			if(((rn&15)<8) || ((rn&15)>15))		break;
			if((disp>>2)<16)
				{ opw=0x8680|((rn&7)<<4)|((disp>>2)&15); break; }
			break;
		}
	}

	if((opw<0) && ctx->is_addr64)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVQ:
			if(!ctx->is_addr64)			break;
			if((disp&7) || (disp<0))	break;
			if((disp>>3)>=16)			break;
			if((rm!=BGBCC_SH_REG_R15) && (rm!=BGBCC_SH_REG_RQ15))
				break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw=0xC400|((rn&15)<<4)|((disp>>3)&15);
			break;

		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_movi20)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(disp<0)					break;
			if(disp>=4096)				break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x4000|((disp)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (disp<0))	break;
			if((disp>>1)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x5000|((disp>>1)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x6000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x7000|((disp>>2)&0x0FFF);
			break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(((sbyte)disp)!=disp)							break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
			opw=0x8E00|((disp)&255);
			opw2=0x6000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))	break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x6001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))	break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x6002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((sbyte)(disp>>3))!=(disp>>3)))	break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>3)&255);
			opw2=0x6003|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))			break;
			if(ctx->is_mergece)
			{	opw=0x8E00|((disp>>2)&255);
				opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);
				break;			}
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2006|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_MOVUB:
			if(((sbyte)disp)!=disp)		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x600C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x600D|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_LEAB:
			if(((sbyte)disp)!=disp)		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x6008|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_LEAW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x6009|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_LEAL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x600A|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_LEAQ:
			if((disp&7) || (((sbyte)(disp>>3))!=(disp>>3)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>3)&255);
			opw2=0x600B|((rn&15)<<8)|((rm&15)<<4);
			break;

//		case BGBCC_SH_NMID_FMOVS:
//			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
//			opw2=0x7000|((disp>>2)&0x0FFF);
//			break;
		default:
			break;
		}

//		if(ctx->no_ext32)
//			opw=-1;
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)))
	{
		opw3=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((disp&63)!=disp)							break;
			opw=opw3|((disp)&63);
			opw2=0x6000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((disp>>1)&63)!=(disp>>1)))	break;
			opw=opw3|((disp>>1)&63);
			opw2=0x6001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((disp>>2)&63)!=(disp>>2)))	break;
			opw=opw3|((disp>>2)&63);
			opw2=0x6002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((disp>>3)&63)!=(disp>>3)))	break;
			opw=opw3|((disp>>3)&63);
			opw2=0x6003|((rn&15)<<8)|((rm&15)<<4);
			break;

#if 1
		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((disp>>2)&15)!=(disp>>2)))	break;
			opw=(opw3&0xFF00)|((opw3&0x00C0)>>1)|((disp>>2)&15);
			opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpRegStReg2(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpRegStReg2(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw, opw1, opw2, opw3, opw4;
	int rm3, rn3;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
		return(BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	if((ro!=BGBCC_SH_REG_R0) && (rn==BGBCC_SH_REG_R0))
		{ i=ro; ro=rn; rn=i; }
	if((ro!=BGBCC_SH_REG_RQ0) && (rn==BGBCC_SH_REG_RQ0))
		{ i=ro; ro=rn; rn=i; }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

#if 1
	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		!(ro&31) && ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC180|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}
#endif

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0004|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
//		if(BGBCC_SHX_EmitCheckModeDQ(ctx))				break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))	break;
		opw=0x0005|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0006|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVQ:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))			break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x0007|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x0005|((rn&15)<<8)|((rm&15)<<4);			break;

	case BGBCC_SH_NMID_FMOVS:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rn==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rm))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			opw=0x40C1|((rm&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
			opw=0x40D1|((rm&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
			opw=0x40E1|((rm&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=0x40A1|((rm&15)<<8); break;
		}
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rn==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rm))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			opw=0x40B1|((rm&15)<<8); break;
		}
	}
	
#if 1
	if((opw<0) && ctx->has_bjx1mov && !ctx->is_mergece)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x404E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x405E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x406E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x407E|((rn&15)<<8); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x403E|((rn&15)<<8); break;

		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x0004|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x0005|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw1&=~8;
				opw=opw1;
				opw2=0x0006|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!(opw1&8))	break;
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1|8;
				opw2=0x0006|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;
//			opw=opw1|8;
//			opw2=0xA000|((rn&15)<<8)|((rm&15)<<4)|(ro&15); break;

		case BGBCC_SH_NMID_FMOVS:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			opw=opw1; opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

#if 1
		case BGBCC_SH_NMID_FMOVS:
//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;
#endif

		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdReg2Reg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpLdReg2Reg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_TryEmitOpLdReg2Reg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw, opw1, opw2, opw3, opw4;
	int rm3, rn3;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
//		return(BGBCC_SHX_EmitOpRegLdReg(ctx, nmid, rm, rn));
		return(BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	if((ro!=BGBCC_SH_REG_R0) && (rm==BGBCC_SH_REG_R0))
		{ i=ro; ro=rm; rm=i; }
	if((ro!=BGBCC_SH_REG_RQ0) && (rm==BGBCC_SH_REG_RQ0))
		{ i=ro; ro=rm; rm=i; }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

#if 1
	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		!(ro&31) && ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC188|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}
#endif

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000C|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVW:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))			break;
		opw=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVL:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000E|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVQ:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x000F|((rn&15)<<8)|((rm&15)<<4); break; }
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))			break;
		opw=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FMOVS:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rm==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rn))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=0x40C5|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
			opw=0x40D5|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
			opw=0x40E5|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=0x40A5|((rn&15)<<8); break;

		case BGBCC_SH_NMID_LEAB:
		case BGBCC_SH_NMID_LEAW:
		case BGBCC_SH_NMID_LEAL:
		case BGBCC_SH_NMID_LEAQ:
			opw=0x40F5|((rn&15)<<8); break;
		}
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rm==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rn))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			opw=0x40B5|((rn&15)<<8); break;
		}
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov && !ctx->is_mergece)
//	if(0)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40CE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40DE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40EE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40FE|((rn&15)<<8); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x400E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x401E|((rn&15)<<8); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x402E|((rn&15)<<8); break;

#if 1
		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x408E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x409E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40AE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40BE|((rn&15)<<8); break;
#endif

		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;
		opw3=0xCE00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;

//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
//			opw=opw1;
//			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
//			break;
		case BGBCC_SH_NMID_MOVW:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
//			opw=opw1;
//			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
//			break;
		case BGBCC_SH_NMID_MOVL:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				if(opw1&8)
					opw1|=1;
				opw=opw1;
				opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
//			if(opw1&8)
//			{
//				if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//					break;
//				opw=opw1;
//				opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
//				break;
//			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
//			opw=opw1&(~8);
//			opw2=0xB000|((rn&15)<<8)|((rm&15)<<4)|(ro&15); break;
		case BGBCC_SH_NMID_MOVQ:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1|8;
				opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;
//			opw=opw1|8;
//			opw2=0xB000|((rn&15)<<8)|((rm&15)<<4)|(ro&15); break;


		case BGBCC_SH_NMID_MOVUB:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw=opw1|1;
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw=opw1|1;
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUL:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw1&=~8;	opw=opw1|1;
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=opw1; opw2=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 &&
		(ctx->has_bjx1egpr && ctx->has_bjx1mov) &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw3=0xCE00;
		opw4=0xCE00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw4|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw4|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw4|=0x0020;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw4|=0x0010;

		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;

#if 1
		case BGBCC_SH_NMID_LEAB:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAW:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAL:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAQ:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6007|((rn&15)<<8)|((rm&15)<<4); break;
#endif

#if 1
		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0xF006|((rn&15)<<8)|((rm&15)<<4); break;
#endif

		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2Disp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpRegStReg2Disp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_SHX_EmitOpLdReg2DispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_ProbeEmitOpLdReg2DispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_SHX_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

#if 1

int BGBCC_SHX_TryEmitOpRegStReg2Disp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw, opw2;
	int i;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;
	
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((disp<0) || (disp>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp<0) || (disp&1) || ((disp>>1)>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>1);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp<0) || (disp&3) || ((disp>>2)>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>2);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp<0) || (disp&7) || ((disp>>3)>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>3);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_TryEmitOpLdReg2DispReg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	int opw, opw2;
	int i, d;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((disp<0) || (disp>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			if((disp<0) || (disp&1) || ((disp>>1)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>1);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			if((disp<0) || (disp&3) || ((disp>>2)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>2);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp<0) || (disp&7) || ((disp>>3)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>3);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

#endif
