/* All labels reachable with 16 bits displacement. */
int BGBCC_BSR_ModelIsLabel16P(BGBCC_BSR_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_SMALL24)
		return(1);
	return(0);
}

/* All labels reachable with 24 bit displacement. */
int BGBCC_BSR_ModelIsLabel24P(BGBCC_BSR_Context *ctx)
{
	return(1);
}

/* Model allows absolute 16-bit addressing. */
int BGBCC_BSR_ModelIsAbs16P(BGBCC_BSR_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	return(0);
}

int BGBCC_BSR_EmitOpLblReg(BGBCC_BSR_Context *ctx,
	int nmid, int lbl, int reg)
{
	if(!BGBCC_BSR_TryEmitOpLblReg(ctx, nmid, lbl, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_TryEmitOpLblReg(BGBCC_BSR_Context *ctx,
	int nmid, int lbl, int reg)
{
	int opw1, opw2, opw3, opw4, rlty;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	switch(nmid)
	{
//	case BGBCC_SH_NMID_BRA:
//	case BGBCC_SH_NMID_BRAN:
//		rlty=BGBCC_SH_RLC_RELW8_BSR;
//		opw1=0x2000; break;

	case BGBCC_SH_NMID_MOV:
	case BGBCC_SH_NMID_LEAB:
#if 1
		if(BGBCC_BSR_EmitCheckAutoLabelNear8(ctx, lbl))
		{
			rlty=BGBCC_SH_RLC_RELW8_BSR;
//			opw1=0x25FE;
			opw1=0x25FF;
//			opw2=0x310F|((reg&15)<<4);
			opw2=0x310A|((reg&15)<<4);
			break;
		}
#endif

		if(BGBCC_BSR_ModelIsAbs16P(ctx))
		{
#if 0
			rlty=BGBCC_SH_RLC_ABSW16B_BSR;
			opw1=0xA000;
			opw2=0x4800|((reg&15)<<4);
			break;
#endif

#if 1
			rlty=BGBCC_SH_RLC_RELW16A_BSR;
			opw1=0xA000;
			opw2=0x3F00|((reg&15)<<4);
			break;
#endif
		}

		if(BGBCC_BSR_ModelIsLabel16P(ctx))
		{
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0x25FF;
			opw2=0x26FE;
			opw3=0x310F|((reg&15)<<4);
		}else
		{
			rlty=BGBCC_SH_RLC_RELW24_BSR;
			opw1=0x25FF;
			opw2=0x26FF;
			opw3=0x26FE;
			opw4=0x310F|((reg&15)<<4);
		}
		break;
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_BSR_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_BSR_EmitWord(ctx, opw4);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitOpLabel(BGBCC_BSR_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_BSR_TryEmitOpLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_TryEmitOpLabel(BGBCC_BSR_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, opw4, rlty;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2000; break;
	case BGBCC_SH_NMID_BSR:
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2100; break;
	case BGBCC_SH_NMID_BT:
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2200; break;
	case BGBCC_SH_NMID_BF:
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2300; break;

	case BGBCC_SH_NMID_BRA8B:
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW32D_BSR;
			opw1=0xA000;
			opw2=0x2600;
			opw3=0x2600;
			opw4=0x40F0;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW24_BSR;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x26FE;
		opw4=0x3002;
		break;
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_BSR_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_BSR_EmitWord(ctx, opw4);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_BSR_EmitOpFar16Label(BGBCC_BSR_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_BSR_TryEmitOpFar16Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_TryEmitOpFar16Label(BGBCC_BSR_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, rlty;

	opw1=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW16D_BSR;
			opw1=0xA000;
			opw2=0x40F0;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
		opw1=0x2400;
		opw2=0x2601;
		opw3=0x3002;
		break;
	case BGBCC_SH_NMID_BSR:
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW16D_BSR;
			opw1=0xA000;
			opw2=0x41F0;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
		opw1=0x2400;
		opw2=0x2601;
		opw3=0x3012;
		break;
	case BGBCC_SH_NMID_BT:
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW16D_BSR;
			opw1=0xA000;
			opw2=0x44F0;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
		opw1=0x2400;
		opw2=0x2601;
		opw3=0x3022;
		break;
	case BGBCC_SH_NMID_BF:
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW16D_BSR;
			opw1=0xA000;
			opw2=0x45F0;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
		opw1=0x2400;
		opw2=0x2601;
		opw3=0x3032;
		break;
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_BSR_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_BSR_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_BSR_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_BSR_EmitWord(ctx, opw3);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}



int BGBCC_BSR_EmitCheckAutoLabelNear8(
	BGBCC_BSR_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_BSR_LookupSimLabelIndex(ctx, lbl);
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) &&
		(ctx->is_stable || ctx->is_rawasm))
	{
		j=ctx->lbl_ofs[i];
//		rngb=244;
		rngb=252;
		rngw=4080;
		rngw16=65280;
		if(i>=ctx->nlbl)
		{
			j=j+szrng;
			rngb=88;
			rngw=1536;
			rngw16=65280-(szrng/2);
			
			rngb=244-(szrng/2);
			rngw=4080-(szrng/2);

			if(!ctx->is_stable)
			{
				rngb=999999;
				rngw=999999999;
				rngw16=999999999;
				if(ctx->need_n12jmp)
					rngb=0;
				if(ctx->need_f16jmp)
					rngw=0;
				if(ctx->need_farjmp)
					rngw16=16;
			}
		}
			
		k=BGBCC_BSR_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;
		
		if(j<rngb)
		{
			return(1);
		}
		
		return(0);
	}
	
//	if(!ctx->need_n12jmp)
//		return(1);
	return(0);
}


int BGBCC_BSR_EmitCheckAutoLabelNear16B(
	BGBCC_BSR_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

//	if(!ctx->has_bjx1jmp)
//		return(0);

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

//	i=BGBCC_BSR_LookupSimLabelIndex(ctx, lbl);
	i=BGBCC_BSR_LookupLabelIndex(ctx, lbl);
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
//		rngb=244;
		rngb=252;
		rngw=4080;
		rngw16=65280;
		if(i>=ctx->nlbl)
			return(0);
			
		k=BGBCC_BSR_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;

		if(j<rngb)
			{ return(3); }
		if(j<rngw)
			{ return(2); }
		if(j<rngw16)
			{ return(1); }
		
		if(BGBCC_BSR_ModelIsLabel16P(ctx))
			return(1);
		
		return(0);
	}

	if(BGBCC_BSR_ModelIsLabel16P(ctx))
		return(1);
	
	return(0);
}

int BGBCC_BSR_EmitOpAutoLabel(BGBCC_BSR_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_BSR_TryEmitOpAutoLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_BSR_TryEmitOpAutoLabel(BGBCC_BSR_Context *ctx, int nmid, int lbl)
{
	int ndfl;
	
	ndfl=	(nmid!=BGBCC_SH_NMID_BRA) &&
			(nmid!=BGBCC_SH_NMID_BRAN) &&
			(nmid!=BGBCC_SH_NMID_BSR) &&
			(nmid!=BGBCC_SH_NMID_BT) &&
			(nmid!=BGBCC_SH_NMID_BF);

	if(BGBCC_BSR_EmitCheckAutoLabelNear8(ctx, lbl) || ndfl)
	{
		return(BGBCC_BSR_TryEmitOpLabel(ctx, nmid, lbl));
	}

	if(BGBCC_BSR_EmitCheckAutoLabelNear16B(ctx, lbl))
	{
		return(BGBCC_BSR_TryEmitOpFar16Label(ctx, nmid, lbl));
	}

	return(0);
}
