int BGBCC_BSR_EmitOpRegStReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_BSR_TryEmitOpRegStReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegStReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegStReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegStReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpLdRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_BSR_TryEmitOpLdRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpLdRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpLdRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpLdRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0300|((rn&15)<<4)|((rm&15)<<0);			break;

	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0800|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0900|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0A00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0B00|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpRegStDecReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_BSR_TryEmitOpRegStDecReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegStDecReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegStDecReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegStDecReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FF|((rn&15)<<8);
		opw2=0x0000|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FE|((rn&15)<<8);
		opw2=0x0100|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw1=0xC0FC|((rn&15)<<8);
		opw2=0x0200|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpLdIncRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_BSR_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpLdIncRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpLdIncRegReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw1, opw2;

	opw1=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0300|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC001|((rm&15)<<8);
		break;

	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0800|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC001|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0900|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0A00|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC004|((rm&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw1=0x0B00|((rn&15)<<4)|((rm&15)<<0);
		opw2=0xC002|((rm&15)<<8);
		break;
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}


int BGBCC_BSR_EmitOpRegStRegDisp(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	if(!BGBCC_BSR_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegStRegDisp(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegStRegDisp(
	BGBCC_BSR_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw1, opw2, odr;

	if(!disp)
	{
		return(BGBCC_BSR_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

	opw1=-1; opw2=-1; odr=0;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		odr=1; opw1=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:	
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		odr=2; opw1=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:	
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		
		if((rn==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x2800|((rm&15)<<4)|((disp>>2)&15); break; }

		if(disp && ((disp&0x1C)==disp))
			{ odr=0; opw1=0x8000|((rn&15)<<4)|
				((rm&15)<<0)|((disp<<6)&0x0700); break; }

#if 0
		if(disp==4)
			{ odr=0; opw1=0x4100|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==8)
			{ odr=0; opw1=0x4200|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==12)
			{ odr=0; opw1=0x4300|((rn&15)<<4)|((rm&15)<<0); break; }
		if(disp==16)
			{ odr=0; opw1=0x4000|((rn&15)<<4)|((rm&15)<<0); break; }
#endif

		odr=4; opw1=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(opw1>=0)
	{
		if(odr)
			BGBCC_BSR_EmitLoadDrImm(ctx, disp/odr);
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpLdRegDispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	if(!BGBCC_BSR_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpLdRegDispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpLdRegDispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw1, opw2, odr;

	if(!disp)
	{
		return(BGBCC_BSR_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

	opw1=-1; opw2=-1; odr=1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		odr=1; opw1=0x0700|((rn&15)<<4)|((rm&15)<<0);		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		odr=1; opw1=0x0C00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		odr=2; opw1=0x0D00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((rm==BGBCC_SH_REG_SP) && ((disp&0x3C)==disp))
			{ odr=0; opw1=0x2A00|((rn&15)<<4)|((disp>>2)&15); break; }

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

		odr=4; opw1=0x0E00|((rn&15)<<4)|((rm&15)<<0);	break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		odr=2; opw1=0x0F00|((rn&15)<<4)|((rm&15)<<0);	break;
	}

	if(opw1>=0)
	{
		if(odr)
			BGBCC_BSR_EmitLoadDrImm(ctx, disp/odr);
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpRegStReg2(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_BSR_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegStReg2(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegStReg2(
	BGBCC_BSR_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3;

	if(ro==BGBCC_SH_REG_DLR)
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw1=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else if(ro!=BGBCC_SH_REG_R0)
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rn&15);
			opw3=0x3500|((rm&15)<<4);						break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rn&15);
			opw3=0x3501|((rm&15)<<4);						break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rn&15);
			opw3=0x3502|((rm&15)<<4);						break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpLdReg2Reg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_BSR_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpLdReg2Reg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpLdReg2Reg(
	BGBCC_BSR_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw1, opw2, opw3;

	if(ro==BGBCC_SH_REG_DLR)
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else if(ro!=BGBCC_SH_REG_R0)
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}else
	{
		opw1=-1; opw2=-1; opw3=-1;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rm&15);
			opw3=0x3503|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rm&15);
			opw3=0x3508|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rm&15);
			opw3=0x3509|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rm&15);
			opw3=0x350A|((rn&15)<<4);						break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			opw1=0x1A30|(ro&15);	opw2=0x360C|(rm&15);
			opw3=0x350B|((rn&15)<<4);						break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4C00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4D00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4E00|((rn&15)<<4)|((rm&15)<<0);			break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw1=0x1A30|(ro&15);
			opw2=0x4F00|((rn&15)<<4)|((rm&15)<<0);			break;
		}
	}

	if(opw1>=0)
	{
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_EmitOpRegStReg2Disp(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(!BGBCC_BSR_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpRegStReg2Disp(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_EmitOpLdReg2DispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(!BGBCC_BSR_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_ProbeEmitOpLdReg2DispReg(BGBCC_BSR_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	int i, l;
	l=ctx->emit_isprobe;
	ctx->emit_isprobe=1;
	i=BGBCC_BSR_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn);
	ctx->emit_isprobe=l;
	return(i);
}

int BGBCC_BSR_TryEmitOpRegStReg2Disp(
	BGBCC_BSR_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw1, opw2, opw3;
	int odr;

	if(ro==BGBCC_SH_REG_DLR)
	{
		BGBCC_DBGBREAK
	}

	opw1=-1; opw2=-1; opw3=-1; odr=1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0400|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0500|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0600|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(opw1>=0)
	{
		if(odr)
			BGBCC_BSR_EmitLoadDrImm(ctx, disp);
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

int BGBCC_BSR_TryEmitOpLdReg2DispReg(
	BGBCC_BSR_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	int opw1, opw2, opw3;
	int odr;

	if(ro==BGBCC_SH_REG_DLR)
	{
		BGBCC_DBGBREAK
	}

	opw1=-1; opw2=-1; opw3=-1; odr=1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0700|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0C00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0D00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0E00|((rn&15)<<4)|((rm&15)<<0);			break;
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
		opw1=0x360C|((ro&15)<<4);
		opw2=0x0F00|((rn&15)<<4)|((rm&15)<<0);			break;
	}

	if(opw1>=0)
	{
		if(odr)
			BGBCC_BSR_EmitLoadDrImm(ctx, disp);
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		return(1);
	}

	return(0);
}

