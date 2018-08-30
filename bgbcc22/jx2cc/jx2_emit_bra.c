/* All labels reachable with 16 bits displacement. */
int BGBCC_JX2_ModelIsLabel16P(BGBCC_JX2_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_SMALL24)
		return(1);
	return(0);
}

/* All labels reachable with 24 bit displacement. */
int BGBCC_JX2_ModelIsLabel24P(BGBCC_JX2_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_SMALL24)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_MEDIUM24)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_MEDIUM32)
		return(1);

	if(ctx->use_memmdl==BGBCC_MEMMDL_DEFAULT)
		return(1);
	return(0);
}


/* All labels reachable with 24 bit displacement. */
int BGBCC_JX2_ModelIsData24P(BGBCC_JX2_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_SMALL24)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_MEDIUM24)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_MEDIUM32)
		return(1);

	if(ctx->use_memmdl==BGBCC_MEMMDL_DEFAULT)
	{
		if(ctx->is_rom)
			return(0);
		return(1);
	}

	return(0);
}

/* Model allows absolute 16-bit addressing. */
int BGBCC_JX2_ModelIsAbs16P(BGBCC_JX2_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	return(0);
}

int BGBCC_JX2_EmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	if(!BGBCC_JX2_TryEmitOpLblReg(ctx, nmid, lbl, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitOpLblReg(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	int opw1, opw2, opw3, opw4, opw5, rlty, rlty2;
	int tr0;

	if(
		(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVQ) ||
		(nmid==BGBCC_SH_NMID_MOVUB) ||
		(nmid==BGBCC_SH_NMID_MOVUW) ||
		(nmid==BGBCC_SH_NMID_MOVUL) )
	{
		return(BGBCC_JX2_EmitLoadRegLabelVarRel24(ctx, nmid, reg, lbl));
	}

	if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		{
			tr0=BGBCC_JX2C_ScratchAllocTsReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_TryEmitOpLblReg(ctx, nmid, lbl, tr0);
			BGBCC_JX2_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_MOV, tr0, reg);
			BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
			return(1);
		}

		BGBCC_DBGBREAK
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; rlty=-1; rlty2=-1;
	switch(nmid)
	{
//	case BGBCC_SH_NMID_BRA:
//	case BGBCC_SH_NMID_BRAN:
//		rlty=BGBCC_SH_RLC_RELW8_BSR;
//		opw1=0x2000; break;

	case BGBCC_SH_NMID_MOV:
	case BGBCC_SH_NMID_LEAB:
		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			break;

#if 1
		if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl))
		{
			rlty=BGBCC_SH_RLC_RELW8_BSR;
//			opw1=0x25FE;
			opw1=0x25FF;
//			opw2=0x310F|((reg&15)<<4);
//			opw2=0x310A|((reg&15)<<4);
			opw3=0x4D00|((reg&15)<<4);
			break;
		}
#endif

		if(BGBCC_JX2_ModelIsAbs16P(ctx))
		{
#if 0
			rlty=BGBCC_SH_RLC_ABSW16B_BSR;
//			opw1=0xA000;
//			opw2=0x4800|((reg&15)<<4);
			break;
#endif

#if 1
			rlty=BGBCC_SH_RLC_RELW16A_BSR;
			opw1=0xA000;
			opw2=0x3F00|((reg&15)<<4);
			break;
#endif
		}

		if(BGBCC_JX2_ModelIsLabel16P(ctx))
		{
			rlty=BGBCC_SH_RLC_RELW16C_BSR;
//			opw1=0x25FF;
//			opw2=0x26FE;
			opw1=0xF0FF;
			opw2=0x25FE;
//			opw3=0x310F|((reg&15)<<4);
			opw3=0x4C00|((reg&15)<<4);
		}else if(BGBCC_JX2_ModelIsData24P(ctx))
		{
#if 1
			if(!BGBCC_JX2_CheckPadCross48(ctx))
			{
				if(((reg&15)!=0xA) && ((reg&15)!=0xB))
				{
					rlty2=BGBCC_SH_RLC_REL24_BJX;
					opw1=0xFC27|((reg&16)<<4);
					opw2=0xF000|((reg&15)<<8);
					opw3=0x0000;
					break;
				}
			}
#endif

#if 1
			if(!BGBCC_JX2_CheckPadCross32(ctx))
			{
				rlty=BGBCC_SH_RLC_REL24_BJX;
				opw1=0xFBFF;
				opw2=0xFFFE;
//				opw3=0x310F|((reg&15)<<4);
				opw3=0x4C00|((reg&15)<<4);
				break;
			}
#endif

#if 1
			rlty=BGBCC_SH_RLC_RELW24C_BSR;
			opw1=0x25FF;
			opw2=0x26FF;
			opw3=0x26FE;
//			opw4=0x310F|((reg&15)<<4);
			opw4=0x4C00|((reg&15)<<4);
			break;
#endif


		}else
		{
			rlty=BGBCC_SH_RLC_RELW32C_BSR;
			opw1=0x25FF;
			opw2=0x26FF;
			opw3=0x26FE;
			opw4=0x26FE;
//			opw5=0x310F|((reg&15)<<4);
			opw5=0x4C00|((reg&15)<<4);
		}
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		if(opw5>=0)
			BGBCC_JX2_EmitWord(ctx, opw5);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_JX2_TryEmitOpLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitOpLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
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
		if(0)
		{
//			rlty=BGBCC_SH_RLC_RELW32D_BSR;
//			opw1=0xA000;
//			opw2=0x2600;
//			opw3=0x2600;
//			opw4=0x40F0;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW24_BSR;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x26FF;
		opw4=0x3002;
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitOpFar16Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitOpFar16Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, rlty;

	opw1=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
//		if(1)
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			rlty=BGBCC_SH_RLC_RELW16D_BSR;
//			opw1=0xA000;
//			opw2=0x40F0;
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2000;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
//		opw1=0x2400;
//		opw2=0x2601;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x3002;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRN:
//		if(1)
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			rlty=BGBCC_SH_RLC_RELW16D_BSR;
//			opw1=0xA000;
//			opw2=0x41F0;
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2100;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
//		opw1=0x2400;
//		opw2=0x2601;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x3012;
		break;
	case BGBCC_SH_NMID_BT:
//		if(1)
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			rlty=BGBCC_SH_RLC_RELW16D_BSR;
//			opw1=0xA000;
//			opw2=0x44F0;
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2200;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
//		opw1=0x2400;
//		opw2=0x2601;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x3022;
		break;
	case BGBCC_SH_NMID_BF:
//		if(1)
		if(!BGBCC_JX2_CheckPadCross32(ctx))
		{
//			rlty=BGBCC_SH_RLC_RELW16D_BSR;
//			opw1=0xA000;
//			opw2=0x45F0;
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2300;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW16_BSR;
//		opw1=0x2400;
//		opw2=0x2601;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x3032;
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_JX2_TryEmitOpFar24Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, rlty;

	opw1=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
#if 0
//		rlty=BGBCC_SH_RLC_RELW24D_BSR;
//		opw1=0xA000;
//		opw2=0x2600;
//		opw3=0x40F0;
//		break;
#endif
#if 1
		rlty=BGBCC_SH_RLC_RELW24_BJX;
		opw1=0xFBFF;
		opw2=0xFFFF;
		opw3=0x3002;
		break;
#endif
	case BGBCC_SH_NMID_BSR:
#if 0
//		rlty=BGBCC_SH_RLC_RELW24D_BSR;
//		opw1=0xA000;
//		opw2=0x2600;
//		opw3=0x41F0;
//		break;
#endif
#if 1
		rlty=BGBCC_SH_RLC_RELW24_BJX;
		opw1=0xFBFF;
		opw2=0xFFFF;
		opw3=0x3012;
		break;
#endif
	case BGBCC_SH_NMID_BT:
#if 0
//		rlty=BGBCC_SH_RLC_RELW24D_BSR;
//		opw1=0xA000;
//		opw2=0x2600;
//		opw3=0x44F0;
//		break;
#endif
#if 1
		rlty=BGBCC_SH_RLC_RELW24_BJX;
		opw1=0xFBFF;
		opw2=0xFFFF;
		opw3=0x3022;
		break;
#endif
	case BGBCC_SH_NMID_BF:
#if 0
//		rlty=BGBCC_SH_RLC_RELW24D_BSR;
//		opw1=0xA000;
//		opw2=0x2600;
//		opw3=0x45F0;
//		break;
#endif
#if 1
		rlty=BGBCC_SH_RLC_RELW24_BJX;
		opw1=0xFBFF;
		opw2=0xFFFF;
		opw3=0x3032;
		break;
#endif
	}

	if(opw1>=0)
	{
		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}



int BGBCC_JX2_EmitCheckAutoLabelNear8(
	BGBCC_JX2_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_JX2_LookupSimLabelIndex(ctx, lbl);
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) &&
		(ctx->is_stable || ctx->is_rawasm))
	{
		j=ctx->lbl_ofs[i];

		k=BGBCC_JX2_EmitGetOffs(ctx);

//		rngb=244;
		rngb=248;
//		rngb=252;
		rngw=4080;
		rngw16=65280;
		if(i>=ctx->nlbl)
		{
			if(j<k)
			{
				j=j-szrng;
			}else
			{
				j=j+szrng;
			}

			rngb=88;
			rngw=1536;
			rngw16=65280-(szrng/2);
			
//			rngb=160-(szrng/2);
//			rngb=176-(szrng/2);
//			rngb=192-(szrng/2);
//			rngb=224-(szrng/2);
//			rngb=244-(szrng/2);
//			rngb=248;
			rngb=224;
			rngw=4080-(szrng/2);

//			if(j<k)
//			{
//				rngb=0;
//			}

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
			
		k=BGBCC_JX2_EmitGetOffs(ctx);
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


int BGBCC_JX2_EmitCheckAutoLabelNear16B(
	BGBCC_JX2_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

//	if(!ctx->has_bjx1jmp)
//		return(0);

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

//	i=BGBCC_JX2_LookupSimLabelIndex(ctx, lbl);
	i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
//		rngb=244;
		rngb=248;
//		rngb=252;
		rngw=4080;
		rngw16=65280;
		if(i>=ctx->nlbl)
			return(0);
			
		k=BGBCC_JX2_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;

		if(j<rngb)
			{ return(3); }
		if(j<rngw)
			{ return(2); }
		if(j<rngw16)
			{ return(1); }
		
		if(BGBCC_JX2_ModelIsLabel16P(ctx))
			return(1);
		
//		if(!ctx->need_farjmp)
//			return(1);
		
		return(0);
	}

	if(BGBCC_JX2_ModelIsLabel16P(ctx))
		return(1);
	
	return(0);
}

int BGBCC_JX2_EmitCheckAutoLabelNear24B(
	BGBCC_JX2_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

//	if(!ctx->has_bjx1jmp)
//		return(0);

	if(BGBCC_JX2_ModelIsLabel24P(ctx))
		return(1);

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

//	i=BGBCC_JX2_LookupSimLabelIndex(ctx, lbl);
	i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
		rngb=244;
//		rngb=252;
		rngw=4080;
		rngw16=65280;
		if(i>=ctx->nlbl)
			return(0);
			
		k=BGBCC_JX2_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;

		if(j<rngb)
			{ return(3); }
		if(j<rngw)
			{ return(2); }
		if(j<rngw16)
			{ return(1); }
		
		if(BGBCC_JX2_ModelIsLabel24P(ctx))
			return(1);
		
		return(0);
	}

	if(BGBCC_JX2_ModelIsLabel24P(ctx))
		return(1);
	
	return(0);
}

int BGBCC_JX2_EmitOpAutoLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_JX2_TryEmitOpAutoLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitOpAutoLabel(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int ndfl, flcl, fn16;
	
	ndfl=	(nmid!=BGBCC_SH_NMID_BRA) &&
			(nmid!=BGBCC_SH_NMID_BRAN) &&
			(nmid!=BGBCC_SH_NMID_BSR) &&
			(nmid!=BGBCC_SH_NMID_BT) &&
			(nmid!=BGBCC_SH_NMID_BF);

	flcl=	(nmid==BGBCC_SH_NMID_BRA) ||
			(nmid==BGBCC_SH_NMID_BRAN) ||
			(nmid==BGBCC_SH_NMID_BT) ||
			(nmid==BGBCC_SH_NMID_BF);

	fn16=0;
	if(flcl)
	{
		if(!ctx->need_farjmp)
			fn16=1;
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl) || ndfl)
	{
		return(BGBCC_JX2_TryEmitOpLabel(ctx, nmid, lbl));
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear16B(ctx, lbl) || fn16)
	{
		return(BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl));
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear24B(ctx, lbl))
	{
		return(BGBCC_JX2_TryEmitOpFar24Label(ctx, nmid, lbl));
	}

	return(0);
}
