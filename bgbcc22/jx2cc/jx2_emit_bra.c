/* All labels reachable with 16 bits displacement. */
int BGBCC_JX2_ModelIsLabel16P(BGBCC_JX2_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
	if(ctx->use_memmdl==BGBCC_MEMMDL_SMALL24)
		return(1);
	
	if(!ctx->need_n16dat)
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

	if(!ctx->need_n24dat)
		return(1);

	return(0);
}

/* All labels reachable with 20 bit displacement. */
int BGBCC_JX2_ModelIsLabel20P(BGBCC_JX2_Context *ctx)
{
	if(ctx->use_memmdl==BGBCC_MEMMDL_TINY16)
		return(1);
//	if(ctx->use_memmdl==BGBCC_MEMMDL_SMALL24)
//		return(1);
//	if(ctx->use_memmdl==BGBCC_MEMMDL_MEDIUM24)
//		return(1);
//	if(ctx->use_memmdl==BGBCC_MEMMDL_MEDIUM32)
//		return(1);

//	if(ctx->use_memmdl==BGBCC_MEMMDL_DEFAULT)
//		return(1);

	if(!ctx->need_n20dat)
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

	if(!ctx->need_n24dat)
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

#if 0
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
#endif

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; rlty=-1; rlty2=-1;
	switch(nmid)
	{
//	case BGBCC_SH_NMID_BRA:
//	case BGBCC_SH_NMID_BRAN:
//		rlty=BGBCC_SH_RLC_RELW8_BSR;
//		opw1=0x2000; break;

	case BGBCC_SH_NMID_MOV:
	case BGBCC_SH_NMID_LEAB:
		if(ctx->is_fixed32 || !BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;
//			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//				break;

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			opw3=0xF000|((reg&16)<<2);
//			opw4=0x4C00|((reg&15)<<4);
			opw3=0xF080|((reg&16)<<2);
			opw4=0x0400|((reg&15)<<4);
			break;
		}

		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			break;

#if 1
		if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl, nmid))
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

#if 0
			rlty=BGBCC_SH_RLC_RELW16A_BSR;
			opw1=0xA000;
			opw2=0x3F00|((reg&15)<<4);
			break;
#endif
		}

		if(BGBCC_JX2_ModelIsLabel16P(ctx))
		{
			if(!BGBCC_JX2_CheckPadCross32(ctx))
			{
				rlty=BGBCC_SH_RLC_REL24_BJX;
				opw1=0xFBFF;
				opw2=0xFFFE;
//				opw3=0x310F|((reg&15)<<4);
				opw3=0x4C00|((reg&15)<<4);
				break;
			}

			rlty=BGBCC_SH_RLC_RELW16C_BSR;
			opw1=0x25FF;
			opw2=0x26FE;
//			opw1=0xF0FF;
//			opw2=0x25FE;
//			opw3=0x310F|((reg&15)<<4);
			opw3=0x4C00|((reg&15)<<4);
		}else if(BGBCC_JX2_ModelIsData24P(ctx))
		{
#if 1
			if(!BGBCC_JX2_CheckPadCross48(ctx) && !ctx->is_fixed32)
			{
				if(((reg&15)!=0xA) && ((reg&15)!=0xB))
				{
					rlty2=BGBCC_SH_RLC_REL24_BJX;
					opw1=0xFC27|((reg&16)<<4);
//					opw2=0xF000|((reg&15)<<8);
					opw2=0x4000|((reg&15)<<8);
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

	if(ctx->is_fixed32)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1;
		}

		if((opw1>=0) && ((opw1&0xFE00)==0xFC00))
		{
			opw1=-1;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLblReg(ctx, nmid, lbl, reg);
	
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
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2000;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2000; break;
	case BGBCC_SH_NMID_BSR:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2100;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2100; break;
	case BGBCC_SH_NMID_BT:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2200;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2200; break;
	case BGBCC_SH_NMID_BF:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW16_BSR;
			opw1=0xF000;
			opw2=0x2300;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2300; break;

	case BGBCC_SH_NMID_BRA8B:
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
			opw2=0xC000;
			opw3=0xF003;
			opw4=0x3000;
			break;
		}

		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFE;
			opw3=0xF000;
			opw4=0x3002;
			break;
		}

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
//		opw4=0x3002;
		opw4=0x3100;
		break;
	}

	if(ctx->is_fixed32)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
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
//		if(ctx->is_fixed32)
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
//			opw2=0xA000;
			opw2=0xC000;
			break;
		}
	
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
//		opw3=0x3002;
		opw3=0x3100;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRN:
//		if(ctx->is_fixed32)
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
//			opw2=0xB000;
			opw2=0xD000;
			break;
		}
	
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
//		opw3=0x3012;
		opw3=0x3101;
		break;
	case BGBCC_SH_NMID_BT:
//		if(ctx->is_fixed32)
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
			opw2=0xE000;
			break;
		}

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
//		opw3=0x3022;
		opw3=0x3102;
		break;
	case BGBCC_SH_NMID_BF:
//		if(ctx->is_fixed32)
		if(1)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
			opw2=0xF000;
			break;
		}
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
//		opw3=0x3032;
		opw3=0x3103;
		break;
	}

	if(ctx->is_fixed32)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
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


int BGBCC_JX2_EmitOpFar20Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_JX2_TryEmitOpFar20Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitOpFar20Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, opw4, rlty;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
		rlty=BGBCC_SH_RLC_RELW20_BJX;
//		opw1=0xF0FF;
//		opw2=0xAFFE;
		opw1=0xF000;
//		opw2=0xA000;
		opw2=0xC000;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRN:
		rlty=BGBCC_SH_RLC_RELW20_BJX;
//		opw1=0xF0FF;
//		opw2=0xBFFE;
		opw1=0xF000;
//		opw2=0xB000;
		opw2=0xD000;
		break;

	case BGBCC_SH_NMID_BT:
		rlty=BGBCC_SH_RLC_RELW20_BJX;
		opw1=0xF000;
		opw2=0xE000;
		break;
	case BGBCC_SH_NMID_BF:
		rlty=BGBCC_SH_RLC_RELW20_BJX;
		opw1=0xF000;
		opw2=0xF000;
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
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

	return(0);
}


int BGBCC_JX2_TryEmitOpFar24Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, opw4, rlty;

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFE;
//			opw3=0xF000;
//			opw4=0x3002;
			opw3=0xF000;
			opw4=0x3010;
			break;
		}

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
//		opw3=0x3002;
		opw3=0x3100;
		break;
#endif
	case BGBCC_SH_NMID_BSR:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFE;
//			opw3=0xF001;
//			opw4=0x3002;
			opw3=0xF000;
			opw4=0x3011;
			break;
		}
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
//		opw3=0x3012;
		opw3=0x3101;
		break;
#endif
	case BGBCC_SH_NMID_BT:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFE;
//			opw3=0xF002;
//			opw4=0x3002;
			opw3=0xF000;
			opw4=0x3012;
			break;
		}
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
//		opw3=0x3022;
		opw3=0x3102;
		break;
#endif
	case BGBCC_SH_NMID_BF:
		if(ctx->is_fixed32)
		{
			rlty=BGBCC_SH_RLC_RELW24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFE;
//			opw3=0xF003;
//			opw4=0x3002;
			opw3=0xF000;
			opw4=0x3013;
			break;
		}
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
//		opw3=0x3032;
		opw3=0x3103;
		break;
#endif
	}

	if(ctx->is_fixed32)
	{
		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		{
			opw1=-1;
		}

		if((opw3>=0) && ((opw3&0xF000)!=0xF000))
		{
			if((opw3&0xF000)==0x3000)
			{
				opw4=0x3000|((opw3>>4)&0x00F0)|(opw3&0x000F);
				opw3=0xF000|((opw3>>4)&0x000F);
			}else
			{
				opw3=-1;
			}
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
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



int BGBCC_JX2_EmitCheckAutoLabelNear8(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

//	return(0);

//	if(nmid==BGBCC_SH_NMID_BSR)
//		return(0);
//	if(nmid==BGBCC_SH_NMID_BSRN)
//		return(0);

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
//			rngb=224;
			rngb=208;
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

#if 0
int BGBCC_JX2_EmitCheckAutoLabelNear16B(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
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
		{
			if(BGBCC_JX2_ModelIsLabel16P(ctx))
				return(1);

			if((ctx->lbl_sec[i]==ctx->sec) &&
				(ctx->sec==BGBCC_SH_CSEG_TEXT))
			{
				if(!ctx->need_n16bsr)
					return(1);
			}

			return(0);
		}
			
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
		
		if((ctx->lbl_sec[i]==ctx->sec) &&
			(ctx->sec==BGBCC_SH_CSEG_TEXT))
		{
			if(!ctx->need_n16bsr)
				return(1);
		}
		
		return(0);
	}

	if(BGBCC_JX2_ModelIsLabel16P(ctx))
		return(1);
	
	return(0);
}

int BGBCC_JX2_EmitCheckAutoLabelNear24B(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

//	if(!ctx->has_bjx1jmp)
//		return(0);

	if(BGBCC_JX2_ModelIsLabel24P(ctx))
		return(1);

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_JX2_LookupSimLabelIndex(ctx, lbl);
//	i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
		rngb=244;
//		rngb=252;
		rngw=4080;
		rngw16=65280;
		if(i>=ctx->nlbl)
		{
			if(BGBCC_JX2_ModelIsLabel24P(ctx))
				return(1);

			if((ctx->lbl_sec[i]==ctx->sec) &&
				(ctx->sec==BGBCC_SH_CSEG_TEXT))
			{
				if(!ctx->need_n24bsr)
					return(1);
			}

			return(0);
		}
			
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

		if((ctx->lbl_sec[i]==ctx->sec) &&
			(ctx->sec==BGBCC_SH_CSEG_TEXT))
		{
			if(!ctx->need_n24bsr)
				return(1);
		}
		
		return(0);
	}

	if(BGBCC_JX2_ModelIsLabel24P(ctx))
		return(1);
	
	return(0);
}
#endif

int BGBCC_JX2_EmitCheckAutoLabelNear16B(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if((ra>0) && (ra<=3))
		return(ra);
	return(0);
}

int BGBCC_JX2_EmitCheckAutoLabelNear20B(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if((ra>0) && (ra<=4))
		return(ra);
	return(0);
}


int BGBCC_JX2_EmitCheckAutoLabelNear24B(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if((ra>0) && (ra<=5))
		return(ra);
	return(0);
}

int BGBCC_JX2_EmitCheckAutoLabelNear16(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if(((ra&15)>0) && ((ra&15)<=3))
		return(ra);
	return(0);
}

int BGBCC_JX2_EmitCheckAutoLabelNear20(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if(((ra&15)>0) && ((ra&15)<=4))
		return(ra);
	return(0);
}


int BGBCC_JX2_EmitCheckAutoLabelNear24(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if(((ra&15)>0) && ((ra&15)<=5))
		return(ra);
	return(0);
}

int BGBCC_JX2_EmitCheckAutoLabelNearClass(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int isbra;
	int i, j, k, rngb, rngw, rngw16, rngw20, rngw24, szrng;

//	if(BGBCC_JX2_ModelIsLabel20P(ctx))
//		return(1);

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_JX2_LookupSimLabelIndex(ctx, lbl);
//	i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	{
		j=ctx->lbl_ofs[i];
		rngb=244;
//		rngb=252;
		rngw=4080;
//		rngw16=65280;
//		rngw16=64000;

		rngw16=65536-4096;
		rngw20=1048576-4096;
		rngw24=16777216-65536;

		if(i>=ctx->nlbl)
		{
			if((ctx->lbl_sec[i]==ctx->sec) &&
				(ctx->sec==BGBCC_SH_CSEG_TEXT))
			{
				if(!ctx->need_n16bsr)
					return(16+3);
				if(!ctx->need_n20bsr)
					return(16+4);
				if(!ctx->need_n24bsr)
					return(16+5);
			}

			if(BGBCC_JX2_ModelIsLabel20P(ctx))
				return(16+4);
			if(BGBCC_JX2_ModelIsLabel24P(ctx))
				return(16+5);

			return(0);
		}
			
		k=BGBCC_JX2_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;

		if(j<rngb)
			{ return(1); }
		if(j<rngw)
			{ return(2); }
		if(j<rngw16)
			{ return(3); }
		if(j<rngw20)
			{ return(4); }
		if(j<rngw20)
			{ return(5); }
		
		if((ctx->lbl_sec[i]==ctx->sec) &&
			(ctx->sec==BGBCC_SH_CSEG_TEXT))
		{
			if(!ctx->need_n16bsr)
				return(3);
			if(!ctx->need_n20bsr)
				return(4);
			if(!ctx->need_n24bsr)
				return(5);
		}
		
		if(BGBCC_JX2_ModelIsLabel16P(ctx))
			return(3);
		if(BGBCC_JX2_ModelIsLabel20P(ctx))
			return(4);
		if(BGBCC_JX2_ModelIsLabel24P(ctx))
			return(5);

		return(0);
	}

	isbra=	(nmid==BGBCC_SH_NMID_BRA) ||
			(nmid==BGBCC_SH_NMID_BRAN) ||
			(nmid==BGBCC_SH_NMID_BSR) ||
			(nmid==BGBCC_SH_NMID_BSRN) ||
			(nmid==BGBCC_SH_NMID_BT) ||
			(nmid==BGBCC_SH_NMID_BF);

	if(isbra)
	{
		if(!ctx->need_n16bsr)
			return(16+3);
		if(!ctx->need_n20bsr)
			return(16+4);
		if(!ctx->need_n24bsr)
			return(16+5);
	}

	if(BGBCC_JX2_ModelIsLabel16P(ctx))
		return(16+3);
	if(BGBCC_JX2_ModelIsLabel20P(ctx))
		return(16+4);
	if(BGBCC_JX2_ModelIsLabel24P(ctx))
		return(16+5);
	
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
	int ndfl, flcl, fn16, flc20;
	
	ndfl=	(nmid!=BGBCC_SH_NMID_BRA) &&
			(nmid!=BGBCC_SH_NMID_BRAN) &&
			(nmid!=BGBCC_SH_NMID_BSR) &&
			(nmid!=BGBCC_SH_NMID_BSRN) &&
			(nmid!=BGBCC_SH_NMID_BT) &&
			(nmid!=BGBCC_SH_NMID_BF);

	flcl=	(nmid==BGBCC_SH_NMID_BRA) ||
			(nmid==BGBCC_SH_NMID_BRAN) ||
			(nmid==BGBCC_SH_NMID_BT) ||
			(nmid==BGBCC_SH_NMID_BF);

	flc20=	(nmid==BGBCC_SH_NMID_BRA) ||
			(nmid==BGBCC_SH_NMID_BRAN) ||
			(nmid==BGBCC_SH_NMID_BSR) ||
			(nmid==BGBCC_SH_NMID_BSRN);

	fn16=0;
	if(flcl)
	{
		if(!ctx->need_farjmp)
			fn16=1;
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl, nmid) || ndfl)
	{
		if(!ndfl && ctx->is_fixed32)
		{
			return(BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl));
		}
	
		return(BGBCC_JX2_TryEmitOpLabel(ctx, nmid, lbl));
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear20(ctx, lbl, nmid) && flc20)
	{
		return(BGBCC_JX2_TryEmitOpFar20Label(ctx, nmid, lbl));
	}

//	if(BGBCC_JX2_EmitCheckAutoLabelNear16B(ctx, lbl) || fn16)
	if(BGBCC_JX2_EmitCheckAutoLabelNear16(ctx, lbl, nmid) || fn16)
	{
		return(BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl));
	}

//	if(BGBCC_JX2_EmitCheckAutoLabelNear24B(ctx, lbl))
	if(BGBCC_JX2_EmitCheckAutoLabelNear24(ctx, lbl, nmid))
	{
		return(BGBCC_JX2_TryEmitOpFar24Label(ctx, nmid, lbl));
	}

	return(0);
}
