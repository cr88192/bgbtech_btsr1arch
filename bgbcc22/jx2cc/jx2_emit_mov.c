int BGBCC_JX2_EmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegStReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, ex;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x9000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMOVD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x9100|((rn&15)<<4)|((rm&15)<<0);			break;
	}
	
	if(ctx->is_fixed32)
		opw1=-1;
	
//	opw1=-1;
	
	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF000|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:	
		case BGBCC_SH_NMID_MOVUW:
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0x1000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF001|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:	
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0x2000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF002|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:	
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0x3000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF003|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;

		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
//			opw1=0xF000|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
//			opw1=0xF001|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpRegStReg(ctx, nmid, rm, rn);
	
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, ex;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMOVD:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		opw1=0x9500|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x8000|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32)
		opw1=-1;

//	opw1=-1;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF008|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:	
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF009|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:	
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF00A|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:	
			if(ctx->is_fixed32)
			{
				opw1=0xF100|ex;
				opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF00B|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			if(ctx->is_fixed32)
			{
				opw1=0xF180|ex;
				opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF088|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;
			opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:	
			if(ctx->is_fixed32)
			{
				opw1=0xF180|ex;
				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF089|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;
			opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:	
			if(ctx->is_fixed32)
			{
				opw1=0xF180|ex;
				opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
//			opw1=0xF08A|ex;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;
			opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVDL:
			if(ctx->is_fixed32)
			{
				opw1=0xF180|ex;
				opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
				break;
			}
			opw1=0xF080|ex;
			opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
//			opw1=0xF004|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
//			opw1=0xF005|ex;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;
			opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLdRegReg(ctx, nmid, rm, rn);

		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegStDecReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStDecReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStDecReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, opw3, opw4;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FF|((rn&15)<<8);
		opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FE|((rn&15)<<8);
		opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FC|((rn&15)<<8);
		opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FC|((rn&15)<<8);
		opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32)
		opw1=-1;

	if(opw1<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-1);
//		opw3=0xF000;
//		opw4=0x0000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100;
		opw4=0x0000|((rn&15)<<4)|((rm&15)<<0);
		break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-2);
//		opw3=0xF000;
//		opw4=0x0100|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100;
		opw4=0x1000|((rn&15)<<4)|((rm&15)<<0);
		break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-4);
//		opw3=0xF000;
//		opw4=0x0200|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100;
		opw4=0x2000|((rn&15)<<4)|((rm&15)<<0);
		break;
	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0xF840|(rn&31);
		opw2=(u16)(-8);
//		opw3=0xF000;
//		opw4=0x0300|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF100;
		opw4=0x3000|((rn&15)<<4)|((rm&15)<<0);
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpRegStDecReg(ctx, nmid, rm, rn);
	
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdIncRegReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2, opw3, opw4;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC001|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0900|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0A00|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC004|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0B00|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC008|((rm&15)<<8);
		break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5000|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC001|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x5100|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x8000|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	}

	if(ctx->is_fixed32)
		opw1=-1;

	if(opw1<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100;
		opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)1;
		break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100;
		opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)2;
		break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100;
		opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)4;
		break;
	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF000;
//		opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF100;
		opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)8;
		break;

	case BGBCC_SH_NMID_MOVUB:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF180;
		opw2=0x8000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)1;
		break;
	case BGBCC_SH_NMID_MOVUW:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF180;
		opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)2;
		break;
	case BGBCC_SH_NMID_MOVUL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF180;
		opw2=0xA000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)4;
		break;
	case BGBCC_SH_NMID_MOVDL:	
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
//		opw1=0xF080;
//		opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
		opw1=0xF180;
		opw2=0xB000|((rn&15)<<4)|((rm&15)<<0);
		opw3=0xF840|(rm&31);
		opw4=(u16)4;
		break;
	}


	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLdIncRegReg(ctx, nmid, rm, rn);
	
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		return(1);
	}

	return(0);
}


int BGBCC_JX2_EmitOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	if(!BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStRegDisp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStRegDisp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw1, opw2, opw3, odr, ex;

	if(!disp)
	{
		return(BGBCC_JX2_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

	ex=0;
	opw1=-1; opw2=-1; opw3=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
	case BGBCC_SH_NMID_MOVUB:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x1F)==disp)
			{	opw1=0xF000|(disp&31);
				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if((disp&0x1FF)==disp)
			{	opw1=0xF100|((disp>>4)&31);
				opw2=0x0000|((disp&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0xFFFF)==disp)
			if(((disp&0xFFFF)==disp) && !((disp&0x0FFF)==disp))
			{	opw1=0xFC00;
				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)disp;
				break;	}
		}
		odr=1; opw1=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:	
	case BGBCC_SH_NMID_MOVUW:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;

//		if((rn==BGBCC_SH_REG_SP) && ((disp&0x1E)==disp) && !ctx->is_addr64)
//			{ odr=0; opw1=0x2900|((rm&15)<<4)|((disp>>1)&15); break; }

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x3E)==disp)
			{	opw1=0xF000|((disp>>1)&31);
				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if((disp&0x3FE)==disp)
			{	opw1=0xF100|((disp>>5)&31);
				opw2=0x1000|(((disp>>1)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0x1FFFE)==disp)
			if(((disp&0x1FFFE)==disp) && !((disp&0x1FFE)==disp))
			{	opw1=0xFC00;
				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>1);
				break;	}
		}
//		if(disp&1)break;
		odr=2; opw1=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		
		if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x2800|((rm&15)<<4)|((disp>>2)&15); break; }
//		if((rn==BGBCC_SH_REG_SP) && (((disp&0x3C)|0x40)==disp))
//			{ odr=0; opw1=0x2900|((rm&15)<<4)|((disp>>2)&15); break; }

#if 0
		if((rn==BGBCC_SH_REG_SP) && ((disp&0xFFC)==disp))
		{
			odr=0;
			opw1=0xF000|(rm&31)|((disp>>5)&0x0060);
			opw2=0x2800|((disp>>2)&255);
			break;
		}
#endif

		if(disp && ((disp&0x1C)==disp))
			{ odr=0; opw1=0x8000|((rn&15)<<4)|
				((rm&15)<<0)|((disp<<6)&0x0700); break; }

#if 0
//		if(disp==4)
//			{ odr=0; opw1=0x4100|((rn&15)<<4)|((rm&15)<<0); break; }
//		if(disp==8)
//			{ odr=0; opw1=0x4200|((rn&15)<<4)|((rm&15)<<0); break; }
//		if(disp==12)
//			{ odr=0; opw1=0x4300|((rn&15)<<4)|((rm&15)<<0); break; }
//		if(disp==16)
//			{ odr=0; opw1=0x4000|((rn&15)<<4)|((rm&15)<<0); break; }
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x7C)==disp)
			{	opw1=0xF000|((disp>>2)&31);
				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x7FC)==disp)
			{	opw1=0xF100|((disp>>6)&31);
				opw2=0x2000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0x3FFFC)==disp)
			if(((disp&0x3FFFC)==disp) && !((disp&0x3FFC)==disp))
			{	opw1=0xFC00;
				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>2);
				break;	}
		}

//		if(disp&3)		break;
		odr=4; opw1=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVQ:	
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((rn==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
			{ odr=0; opw1=0x2900|((rm&15)<<4)|((disp>>3)&15); break; }

#if 0
		if((rn==BGBCC_SH_REG_SP) && ((disp&0x1FF8)==disp))
		{
			odr=0;
			opw1=0xF000|(rm&31)|((disp>>6)&0x0060);
			opw2=0x2900|((disp>>3)&255);
			break;
		}
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0xF8)==disp)
			{	opw1=0xF000|((disp>>3)&31);
				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0xFF8)==disp)
			{	opw1=0xF100|((disp>>7)&31);
				opw2=0x3000|(((disp>>3)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
//			if((disp&0x7FFF8)==disp)
			if(((disp&0x7FFF8)==disp) && !((disp&0x7FF8)==disp))
			{	opw1=0xFC00;
				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>3);
				break;	}
		}

//		if(disp&7)		break;
		odr=8; opw1=0x0700|((rn&15)<<4)|((rm&15)<<0);	break;

	case BGBCC_SH_NMID_FMOVS:	
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x7C)==disp)
			{	opw1=0xF000|((disp>>2)&31);
				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				break;		}
		}
//		if(disp&3)		break;
		odr=4; opw1=0x9200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_FMOVD:	
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0xF8)==disp)
			{	opw1=0xF000|((disp>>3)&31);
				opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);
				break;		}
		}
//		if(disp&7)		break;
		odr=8; opw1=0x9300|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32)
		{ opw1=-1; odr=0; }

//	opw1=-1; odr=0;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
		case BGBCC_SH_NMID_MOVB:
			if(!ctx->is_fixed32)
				if((disp&0x1F)==disp)
			{	opw1=0xF000|ex|((disp>>0)&31);
				opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x1FF)==disp)
			{	opw1=0xF100|ex|((disp>>4)&31);
				opw2=0x0000|(((disp>>0)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0xFFFF)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>0);
					break;	}
			}

//			opw1=0xF004|ex; odr=1;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=1;
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_MOVW:
			if(!ctx->is_fixed32)
				if((disp&0x3E)==disp)
			{	opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			if(disp&1)		break;
//			opw1=0xF005|ex; odr=2;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0x3FE)==disp)
			{	opw1=0xF100|ex|((disp>>5)&31);
				opw2=0x1000|(((disp>>1)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0x1FFFE)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>1);
					break;	}
			}

			opw1=0xF000|ex;	odr=2;
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_MOVL:
			if(!ctx->is_fixed32)
				if((disp&0x7C)==disp)
			{	opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			if(disp&3)		break;
//			opw1=0xF006|ex; odr=4;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0x7FC)==disp)
			{	opw1=0xF100|ex|((disp>>6)&31);
				opw2=0x2000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0x3FFFC)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>2);
					break;	}
			}

			opw1=0xF000|ex;	odr=4;
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			if(!ctx->is_fixed32)
				if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			if(disp&7)		break;
//			opw1=0xF007|ex; odr=8;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0xFF8)==disp)
			{	opw1=0xF100|ex|((disp>>7)&31);
				opw2=0x3000|(((disp>>3)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0x7FFF8)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0300|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>3);
					break;	}
			}

			opw1=0xF000|ex;	odr=8;
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			if((disp&0x7C)==disp)
			{	opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x9000|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			if(disp&3)		break;
//			opw1=0xF002|ex; odr=4;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=4;
			opw2=0x9200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x9100|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			if(disp&7)		break;
//			opw1=0xF003|ex; odr=8;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=8;
			opw2=0x9300|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	
		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdRegDispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw1, opw2, opw3, odr, ex;

	if(!disp)
	{
		return(BGBCC_JX2_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

	opw1=-1; opw2=-1; opw3=-1; odr=0; ex=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0x1F)==disp)
			{	opw1=0xF000|(disp&31);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x1FF)==disp)
			{	opw1=0xF100|ex|((disp>>4)&31);
				opw2=0x8000|(((disp>>0)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif
			if((disp&0x1F)==disp)
				break;
			if((disp&0x1FF)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0xFFFF)==disp)
			if(((disp&0xFFFF)==disp) && !((disp&0x0FFF)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>0);
				break;	}
#endif
			if(((disp&0xFFFF)==disp) && !((disp&0x0FFF)==disp))
				break;
		}

		odr=1; opw1=0x0C00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;

//		if((rm==BGBCC_SH_REG_SP) && ((disp&0x1E)==disp) && !ctx->is_addr64)
//			{ odr=0; opw1=0x2B00|((rn&15)<<4)|((disp>>1)&15); break; }
		
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0x3E)==disp)
			{	opw1=0xF000|((disp>>1)&31);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x3FE)==disp)
			{	opw1=0xF100|ex|((disp>>5)&31);
				opw2=0x9000|(((disp>>1)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif

			if((disp&0x3E)==disp)
				break;
			if((disp&0x3FE)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0x1FFFE)==disp)
			if(((disp&0x1FFFE)==disp) && !((disp&0x1FFE)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>1);
				break;	}
#endif
			if(((disp&0x1FFFE)==disp) && !((disp&0x1FFE)==disp))
				break;
		}

		odr=2; opw1=0x0D00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;

		if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x2A00|((rn&15)<<4)|((disp>>2)&15); break; }
//		if((rm==BGBCC_SH_REG_SP) && (((disp&0x3C)|0x40)==disp))
//			{ odr=0; opw1=0x2B00|((rn&15)<<4)|((disp>>2)&15); break; }

#if 0
		if((rm==BGBCC_SH_REG_SP) && ((disp&0xFFC)==disp))
		{
			odr=0;
			opw1=0xF000|(rn&31)|((disp>>5)&0x0060);
			opw2=0x2A00|((disp>>2)&255);
			break;
		}
#endif

		if(disp && ((disp&0x1C)==disp))
			{ odr=0; opw1=0x8800|((rn&15)<<4)|
				((rm&15)<<0)|((disp<<6)&0x0700); break; }

#if 0
		if(disp==16)
			{ odr=0; opw1=0x4400|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==4)
			{ odr=0; opw1=0x4500|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==8)
			{ odr=0; opw1=0x4600|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==12)
			{ odr=0; opw1=0x4700|((rn&15)<<4)|((rm&15)<<0); break; }
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0x7C)==disp)
			{	opw1=0xF000|((disp>>2)&31);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x7FC)==disp)
			{	opw1=0xF100|ex|((disp>>6)&31);
				opw2=0xA000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif
			if((disp&0x7C)==disp)
				break;
			if((disp&0x7FC)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0x3FFFC)==disp)
			if(((disp&0x3FFFC)==disp) && !((disp&0x3FFC)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>2);
				break;	}
#endif

			if(((disp&0x3FFFC)==disp) && !((disp&0x3FFC)==disp))
				break;
		}

		odr=4; opw1=0x0E00|((rn&15)<<4)|((rm&15)<<0);	break;

	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;

		if((rm==BGBCC_SH_REG_SP) && ((disp&0x78)==disp))
			{ odr=0; opw1=0x2B00|((rn&15)<<4)|((disp>>3)&15); break; }

#if 0
		if((rm==BGBCC_SH_REG_SP) && ((disp&0x1FF8)==disp))
		{
			odr=0;
			opw1=0xF000|(rn&31)|((disp>>6)&0x0060);
			opw2=0x2B00|((disp>>3)&255);
			break;
		}
#endif

		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
#if 0
			if((disp&0xF8)==disp)
			{	opw1=0xF000|((disp>>3)&31);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0xFF8)==disp)
			{	opw1=0xF100|ex|((disp>>7)&31);
				opw2=0xB000|(((disp>>3)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
#endif
			if((disp&0xF8)==disp)
				break;
			if((disp&0xFF8)==disp)
				break;
		}

		if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
		{
#if 0
//			if((disp&0x7FFF8)==disp)
			if(((disp&0x7FFF8)==disp) && !((disp&0x7FF8)==disp))
			{	opw1=0xFC00|(ex>>4);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				opw3=(u16)(disp>>3);
				break;	}
#endif
			if(((disp&0x7FFF8)==disp) && !((disp&0x7FF8)==disp))
				break;
		}

		odr=8; opw1=0x0F00|((rn&15)<<4)|((rm&15)<<0);	break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if((disp&0x1F)==disp)		break;
		odr=1; opw1=0x5200|((rn&15)<<4)|((rm&15)<<0);		break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if((disp&0x3E)==disp)		break;
		odr=2; opw1=0x5300|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVUL:
//	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if((disp&0x7C)==disp)		break;
		odr=4; opw1=0x8800|((rn&15)<<4)|((rm&15)<<0);	break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0x7C)==disp)
			{	opw1=0xF000|((disp>>2)&31);
				opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);
				break;		}
		}
		odr=4; opw1=0x9600|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_FMOVD:
		if(!BGBCC_JX2_EmitCheckRegBaseFPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
			if((disp&0xF8)==disp)
			{	opw1=0xF000|((disp>>3)&31);
				opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);
				break;		}
		}
		odr=8; opw1=0x9700|((rn&15)<<4)|((rm&15)<<0);	break;
	}

	if(ctx->is_fixed32)
		{ opw1=-1; odr=0; }

//	opw1=-1; odr=0;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!ctx->is_fixed32)
				if((disp&0x1F)==disp)
			{	opw1=0xF000|ex|((disp>>0)&31);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			opw1=0xF00C|ex; odr=1;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0x1FF)==disp)
			{	opw1=0xF100|ex|((disp>>4)&31);
				opw2=0x8000|(((disp>>0)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0xFFFF)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>0);
					break;	}
			}

			opw1=0xF000|ex;	odr=1;
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!ctx->is_fixed32)
				if((disp&0x3E)==disp)
			{	opw1=0xF000|ex|((disp>>1)&31);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			opw1=0xF00D|ex; odr=2;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0x3FE)==disp)
			{	opw1=0xF100|ex|((disp>>5)&31);
				opw2=0x9000|(((disp>>1)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0x1FFFE)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>1);
					break;	}
			}

			opw1=0xF000|ex;	odr=2;
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!ctx->is_fixed32)
				if((disp&0x7C)==disp)
			{	opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			opw1=0xF00E|ex; odr=4;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0x7FC)==disp)
			{	opw1=0xF100|ex|((disp>>6)&31);
				opw2=0xA000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0x3FFFC)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>2);
					break;	}
			}

			opw1=0xF000|ex;	odr=4;
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			if(!ctx->is_fixed32)
				if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			opw1=0xF00F|ex; odr=8;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;

			if((disp&0xFF8)==disp)
			{	opw1=0xF100|ex|((disp>>7)&31);
				opw2=0xB000|(((disp>>3)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if((disp&0x7FFF8)==disp)
				{	opw1=0xFC00|(ex>>4);
					opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
					opw3=(u16)(disp>>3);
					break;	}
			}

			opw1=0xF000|ex;	odr=8;
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			if(!ctx->is_fixed32)
				if((disp&0x1F)==disp)
			{	opw1=0xF080|ex|((disp>>0)&31);
				opw2=0x0800|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x1FF)==disp)
			{	opw1=0xF180|ex|((disp>>4)&31);
				opw2=0x8000|(((disp>>0)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

//			opw1=0xF08C|ex; odr=1;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;	odr=1;
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!ctx->is_fixed32)
				if((disp&0x3E)==disp)
			{	opw1=0xF080|ex|((disp>>1)&31);
				opw2=0x0900|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x3FE)==disp)
			{	opw1=0xF180|ex|((disp>>5)&31);
				opw2=0x9000|(((disp>>1)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

//			opw1=0xF08D|ex; odr=2;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;	odr=2;
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
			if(!ctx->is_fixed32)
				if((disp&0x7C)==disp)
			{	opw1=0xF080|ex|((disp>>2)&31);
				opw2=0x0A00|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x7FC)==disp)
			{	opw1=0xF180|ex|((disp>>6)&31);
				opw2=0xA000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}

//			opw1=0xF08E|ex; odr=4;
//			opw2=0x1800|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF080|ex;	odr=4;
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVDL:
			if(!ctx->is_fixed32)
				if((disp&0x7C)==disp)
			{	opw1=0xF080|ex|((disp>>2)&31);
				opw2=0x0B00|((rn&15)<<4)|((rm&15)<<0);
				break;		}

			if((disp&0x7FC)==disp)
			{	opw1=0xF180|ex|((disp>>6)&31);
				opw2=0xB000|(((disp>>2)&15)<<8)|
					((rn&15)<<4)|((rm&15)<<0);
				break;	}
			opw1=0xF080|ex;	odr=4;
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			if((disp&0x7C)==disp)
			{	opw1=0xF000|ex|((disp>>2)&31);
				opw2=0x9400|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			opw1=0xF006|ex; odr=4;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=4;
			opw2=0x9600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			if((disp&0xF8)==disp)
			{	opw1=0xF000|ex|((disp>>3)&31);
				opw2=0x9500|((rn&15)<<4)|((rm&15)<<0);
				break;		}
//			opw1=0xF007|ex; odr=8;
//			opw2=0x9C00|((rn&15)<<4)|((rm&15)<<0);			break;
			opw1=0xF000|ex;	odr=8;
			opw2=0x9700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);

		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_JX2_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStReg2(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStReg2(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3, ex;

	if(ro==BGBCC_SH_REG_DLR)
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else if(ro!=BGBCC_SH_REG_R0)
	{
		opw1=-1; opw2=-1; opw3=-1;

#if 1
		if(BGBCC_JX2_CheckPadCross32(ctx))
		{
			switch(nmid)
			{
			case BGBCC_SH_NMID_MOVB:
			case BGBCC_SH_NMID_MOVUB:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVW:
			case BGBCC_SH_NMID_MOVUW:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVL:
			case BGBCC_SH_NMID_MOVUL:
			case BGBCC_SH_NMID_MOVDL:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVQ:
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
	//			opw1=0x1A40|(ro&15);
				opw1=0x1800|(ro&15);
				opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
			}
		}
#endif
	}else
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3500|((rm&15)<<4);						break;
			opw3=0x0000|((rm&15)<<0);						break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3501|((rm&15)<<4);						break;
			opw3=0x0100|((rm&15)<<0);						break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3502|((rm&15)<<4);						break;
			opw3=0x0200|((rm&15)<<0);						break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rn&15)<<4);
			opw2=0x1000|(rn&15);
//			opw3=0x3503|((rm&15)<<4);						break;
			opw3=0x0300|((rm&15)<<0);						break;
		}
	}

	if(ctx->is_fixed32)
		{ opw1=-1; }

//	opw1=-1;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
		case BGBCC_SH_NMID_MOVB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
		case BGBCC_SH_NMID_MOVW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
		case BGBCC_SH_NMID_MOVL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;

		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			opw1=0xF000|ex|(ro&15);
			opw2=0x9200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			opw1=0xF000|ex|(ro&15);
			opw2=0x9300|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpRegStReg2(ctx, nmid, rm, ro, rn);

		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdReg2Reg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpLdReg2Reg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3, ex;

	if(ro==BGBCC_SH_REG_DLR)
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x5200|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x5300|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x8800|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else if(ro!=BGBCC_SH_REG_R0)
	{
		opw1=-1; opw2=-1; opw3=-1;

#if 1
		if(BGBCC_JX2_CheckPadCross32(ctx))
		{
			switch(nmid)
			{
#if 1
			case BGBCC_SH_NMID_MOVB:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVW:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVL:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVQ:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVUB:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x5200|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVUW:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x5300|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_MOVUL:
			case BGBCC_SH_NMID_MOVDL:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
				opw1=0x1800|(ro&15);
				opw2=0x8800|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
			case BGBCC_SH_NMID_LEAB:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_LEAW:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_LEAL:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
			case BGBCC_SH_NMID_LEAQ:
				if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
				if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
				opw1=0x1800|(ro&15);
				opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif
			}
		}
#endif
	}else
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x3508|((rn&15)<<4);						break;
			opw3=0x0800|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x3509|((rn&15)<<4);						break;
			opw3=0x0900|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x350A|((rn&15)<<4);						break;
			opw3=0x0A00|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
//			opw2=0x360C|((rm&15)<<4);
			opw2=0x1000|(rm&15);
//			opw3=0x350B|((rn&15)<<4);						break;
			opw3=0x0B00|((rn&15)<<4);						break;

		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1800|(ro&15);
			opw2=0x1000|(rm&15);
			opw3=0x5200|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1800|(ro&15);
			opw2=0x1000|(rm&15);
			opw3=0x5300|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVUL:
		case BGBCC_SH_NMID_MOVDL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1800|(ro&15);
			opw2=0x1000|(rm&15);
			opw3=0x8800|((rn&15)<<4);						break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//			opw1=0x1A40|(ro&15);
			opw1=0x1800|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(ctx->is_fixed32)
		{ opw1=-1; }

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_MOVUB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVDL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

#if 0
		case BGBCC_SH_NMID_LEAB:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			opw1=0xF000|ex|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
#endif

#if 1
		case BGBCC_SH_NMID_LEAB:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			opw1=0xF080|ex|(ro&15);
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
#endif
		}
	}

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtFPR(ctx, rn) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, ro) &&
		(ro!=BGBCC_SH_REG_R0))
	{
		ex=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex|=0x0040;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex|=0x0020;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, ro)) ex|=0x0010;

		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			opw1=0xF000|ex|(ro&15);
			opw2=0x9600|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_FMOVD:	
			opw1=0xF000|ex|(ro&15);
			opw2=0x9700|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);

		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(!BGBCC_JX2_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpRegStReg2Disp(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_EmitOpLdReg2DispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(!BGBCC_JX2_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_ProbeEmitOpLdReg2DispReg(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_JX2_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_JX2_TryEmitOpRegStReg2Disp(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw1, opw2, opw3;
	int odr;

	if(ro==BGBCC_SH_REG_DLR)
	{
		BGBCC_DBGBREAK
	}

	opw1=-1; opw2=-1; opw3=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=1;
		opw1=0x1000|(ro&15); odr=1;
		opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=2;
		opw1=0x1000|(ro&15); odr=2;
		opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=4;
		opw1=0x1000|(ro&15); odr=4;
		opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=8;
		opw1=0x1000|(ro&15); odr=8;
		opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32)
		{ opw1=-1; odr=0; }

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp);
	
		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_JX2_TryEmitOpLdReg2DispReg(
	BGBCC_JX2_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	int opw1, opw2, opw3;
	int odr;

	if(ro==BGBCC_SH_REG_DLR)
	{
		BGBCC_DBGBREAK
	}

	opw1=-1; opw2=-1; opw3=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=1;
		opw1=0x1000|(ro&15); odr=1;
		opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=2;
		opw1=0x1000|(ro&15); odr=2;
		opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=4;
		opw1=0x1000|(ro&15); odr=4;
		opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=8;
		opw1=0x1000|(ro&15); odr=8;
		opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=1;
		opw1=0x1000|(ro&15); odr=1;
		opw2=0x5200|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=2;
		opw1=0x1000|(ro&15); odr=2;
		opw2=0x5300|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVDL:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_JX2_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, ro))		break;
//		opw1=0x360C|((ro&15)<<4); odr=4;
		opw1=0x1000|(ro&15); odr=4;
		opw2=0x8800|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(ctx->is_fixed32)
		{ opw1=-1; odr=0; }

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn);

		if(odr)
		{
			if(disp%odr)
				{ BGBCC_DBGBREAK }
			BGBCC_JX2_EmitLoadDrImm(ctx, disp/odr);
		}
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}
