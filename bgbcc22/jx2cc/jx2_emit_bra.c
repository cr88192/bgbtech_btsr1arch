/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

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

int BGBCC_JX2_EmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	if(!BGBCC_JX2_TryEmitOpRegLbl(ctx, nmid, lbl, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_JX2_TryEmitOpRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int lbl, int reg)
{
	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

	if(ctx->emit_riscv&1)
		return(BGBCC_JX2RV_TryEmitOpRegLbl(ctx, nmid, lbl, reg));

	if(
		(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVQ) ||
		(nmid==BGBCC_SH_NMID_MOVX2) ||
		(nmid==BGBCC_SH_NMID_MOVUB) ||
		(nmid==BGBCC_SH_NMID_MOVUW) ||
		(nmid==BGBCC_SH_NMID_MOVUL) )
	{
		return(BGBCC_JX2_EmitStoreRegLabelVarRel24(ctx, nmid, reg, lbl));
	}
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
	int opwf, opwf1, usewxj;
	int tr0;
	int i, j, k;

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

	if(ctx->emit_riscv&1)
		return(BGBCC_JX2_TryEmitOpLblReg(ctx, nmid, lbl, reg));

	if(
		(nmid==BGBCC_SH_NMID_MOVB) ||
		(nmid==BGBCC_SH_NMID_MOVW) ||
		(nmid==BGBCC_SH_NMID_MOVL) ||
		(nmid==BGBCC_SH_NMID_MOVQ) ||
		(nmid==BGBCC_SH_NMID_MOVX2) ||
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

	usewxj=0;
	if(ctx->has_jumbo && !ctx->op_is_wex2)
	{
		usewxj=1;
//		prlty=BGBCC_SH_RLC_PBOJ32_BJX;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; rlty=-1; rlty2=-1;
	opwf=-1; opwf1=-1;
	switch(nmid)
	{
//	case BGBCC_SH_NMID_BRA:
//	case BGBCC_SH_NMID_BRAN:
//		rlty=BGBCC_SH_RLC_RELW8_BSR;
//		opw1=0x2000; break;

	case BGBCC_SH_NMID_MOV:
	case BGBCC_SH_NMID_LEAB:
		if(ctx->is_fixed32 || ctx->op_is_wex2 ||
			!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if(!BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				break;
//			if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
//				break;
		
#if 1
			if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
			{
#if 1
				if(usewxj && BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
				{
					rlty=BGBCC_SH_RLC_PBOJ32_BJX;
					opwf=0xF101|((reg&15)<<4);
					opwf1=0x0800|((reg&16)<<6);
					break;
				}
#endif

				rlty=BGBCC_SH_RLC_PBO24_BJX;
				opw1=0xFA00;
				opw2=0x0000;
				if(reg&32)
				{
					opw3=0x47001|((reg&15)<<4)|((reg&32)<<5);
					opw4=0x50804|((reg&16)<<6);
				}else
				{
					opw3=0xF080|((reg&16)<<2);
					opw4=0x0401|((reg&15)<<4);
				}
				break;
			}
#endif

			rlty=BGBCC_SH_RLC_REL24_BJX;
			opw1=0xFBFF;
			opw2=0xFFFC;
//			opw3=0xF000|((reg&16)<<2);
//			opw4=0x4C00|((reg&15)<<4);
			if(reg&32)
			{
				opw3=0x47000|((reg&15)<<4)|((reg&32)<<5);
				opw4=0x50804|((reg&16)<<6);
			}else
			{
				opw3=0xF080|((reg&16)<<2);
				opw4=0x0400|((reg&15)<<4);
			}
			break;
		}

		if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			break;

#if 1
		if(BGBCC_JX2_CheckLabelIsGpRel(ctx, lbl))
		{
#if 1
			if(usewxj && BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
			{
				rlty=BGBCC_SH_RLC_PBOJ32_BJX;
				opwf=0xF101|((reg&15)<<4);
				opwf1=0x0800|((reg&16)<<6);
				break;
			}
#endif

			rlty=BGBCC_SH_RLC_PBO24_BJX;
			opw1=0xFA00;
			opw2=0x0000;
			opw3=0x4C01|((reg&15)<<4);
			break;
		}
#endif

#if 0
		if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl, nmid))
		{
			BGBCC_DBGBREAK

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
//			if(!BGBCC_JX2_CheckPadCross32(ctx))
			if(1)
			{
				rlty=BGBCC_SH_RLC_REL24_BJX;
				opw1=0xFBFF;
				opw2=0xFFFE;
//				opw3=0x310F|((reg&15)<<4);
				opw3=0x4C00|((reg&15)<<4);
				break;
			}

			BGBCC_DBGBREAK

			rlty=BGBCC_SH_RLC_RELW16C_BSR;
			opw1=0x25FF;
			opw2=0x26FE;
//			opw1=0xF0FF;
//			opw2=0x25FE;
//			opw3=0x310F|((reg&15)<<4);
			opw3=0x4C00|((reg&15)<<4);
		}else if(BGBCC_JX2_ModelIsData24P(ctx))
		{
#if 0
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
//			if(!BGBCC_JX2_CheckPadCross32(ctx))
			if(1)
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
			BGBCC_DBGBREAK

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
			BGBCC_DBGBREAK

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

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
//		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		if((opw1>=0) && ((opw1&0xE000)!=0xE000))
		{
			opw1=-1;
		}

		if((opw1>=0) && ((opw1&0xFE00)==0xFC00))
		{
			opw1=-1;
		}
	}

	if(rlty==BGBCC_SH_RLC_PBOJ32_BJX)
	{
		i=BGBCC_JX2_LookupLabelIndex(ctx, lbl);
		if(i>0)
		{
			j=ctx->lbl_sec[i];
			k=ctx->lbl_ofs[i];
			
			if((j==BGBCC_SH_CSEG_DATA) && (k<=480) && (k>=0))
//			if((j==BGBCC_SH_CSEG_DATA) && (k<=496) && (k>=0))
			{
//				rlty=BGBCC_SH_RLC_PBO9_BJX;

//				opw1=opwf;
//				opw2=opwf1|k;
//				rlty=-1;
			}
		}
	}

	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_PBOJ27_BJX)
//			{ opw1=0xF400; opw2=0xC000; opw3=opwf; opw4=opwf1; }
//		if(rlty==BGBCC_SH_RLC_PBOJ31_BJX)
//			{ opw1=0xF400; opw2=0xC000; opw3=opwf; opw4=opwf1; }

		if(rlty==BGBCC_SH_RLC_PBO9_BJX)
			{ opw1=opwf; opw2=opwf1; }

		if(rlty==BGBCC_SH_RLC_PBOJ29_BJX)
			{ opw1=0xFE00; opw2=0x0000; opw3=opwf; opw4=opwf1; }
		if(rlty==BGBCC_SH_RLC_PBOJ32_BJX)
			{ opw1=0xFE00; opw2=0x0000; opw3=opwf; opw4=opwf1; }

		if(rlty==BGBCC_SH_RLC_PBO24_BJX)
			{ opw1=0xFA00; opw2=0x0000; opw3=opwf; opw4=opwf1; }
		if(rlty==BGBCC_SH_RLC_PBO32_BJX)
			{ opw1=0xFA00; opw2=0x0000; opw3=0x2600; opw4=opwf; opw5=opwf1; }
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLblReg(ctx, nmid, lbl, reg);
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack5(ctx, &opw1, &opw2, &opw3, &opw4, &opw5);
//		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitPadForOpWord2(ctx, opw1, opw2);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(rlty2>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty2); }
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<=0xFE00))
		{
//			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4);
//			BGBCC_JX2_EmitPadForOpWord(ctx, opw3);
			BGBCC_JX2_EmitPadForOpWord2(ctx, opw3, opw4);
		}

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

int BGBCC_JX2_TryEmitOpRegRegLbl(BGBCC_JX2_Context *ctx,
	int nmid, int rm, int rn, int lbl)
{
	int opw1, opw2, opw3, opw4, rlty, rlty2;
	int ex2, ex3;

	nmid=BGBCC_JX2_EmitRemapPseudoOp(ctx, nmid);

	if(ctx->emit_riscv&1)
	{
		return(BGBCC_JX2RV_TryEmitOpRegRegLbl(ctx, nmid, rm, rn, lbl));
	}

	opw1=-1;	opw2=-1;
	opw3=-1;	opw4=-1;
	rlty=-1;	rlty2=-1;

	if((opw1<0) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_JX2_EmitCheckRegExtGPR(ctx, rn))
	{
		ex2=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex2|=0x0400;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex2|=0x0200;
	
		ex3=0;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rm)) ex3|=0x0400;
		if(BGBCC_JX2_EmitCheckRegExt4(ctx, rn)) ex3|=0x0200;
	
//		nowxi=0;
//		if(ctx->op_is_wex2&4)
//			nowxi=disp>>8;
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_BREQL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xF000|ex2;
			break;
		case BGBCC_SH_NMID_BREQ:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xF800|ex2;
			break;
		case BGBCC_SH_NMID_BRNEL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xF100|ex2;
			break;
		case BGBCC_SH_NMID_BRNE:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xF900|ex2;
			break;

		case BGBCC_SH_NMID_BRGTL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xD000|ex2;
			break;
		case BGBCC_SH_NMID_BRGT:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xD800|ex2;
			break;
		case BGBCC_SH_NMID_BRLEL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xD100|ex2;
			break;
		case BGBCC_SH_NMID_BRLE:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xD900|ex2;
			break;

		case BGBCC_SH_NMID_BRLTL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xD000|ex3;
			break;
		case BGBCC_SH_NMID_BRLT:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xD800|ex3;
			break;
		case BGBCC_SH_NMID_BRGEL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xD100|ex3;
			break;
		case BGBCC_SH_NMID_BRGE:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xD900|ex3;
			break;

		case BGBCC_SH_NMID_BRGTUL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xE000|ex2;
			break;
		case BGBCC_SH_NMID_BRGTU:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xE800|ex2;
			break;
		case BGBCC_SH_NMID_BRLEUL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xE100|ex2;
			break;
		case BGBCC_SH_NMID_BRLEU:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rn&15)<<4)|((rm&15)<<0);
			opw2=0xE900|ex2;
			break;

		case BGBCC_SH_NMID_BRLTUL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xE000|ex3;
			break;
		case BGBCC_SH_NMID_BRLTU:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xE800|ex3;
			break;
		case BGBCC_SH_NMID_BRGEUL:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xE100|ex3;
			break;
		case BGBCC_SH_NMID_BRGEU:
			rlty2=BGBCC_SH_RLC_RELW8_BSR;
			opw1=0xF100|((rm&15)<<4)|((rn&15)<<0);
			opw2=0xE900|ex3;
			break;

		default:
			break;
		}
	}

	if(opw1>=0)
	{
//		BGBCC_JX2DA_EmitOpLblReg(ctx, nmid, lbl, reg);
	
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord2(ctx, opw1, opw2);
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

	if(ctx->emit_riscv&1)
		return(BGBCC_JX2RV_TryEmitOpLabel(ctx, nmid, lbl));

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
		{
//			rlty=BGBCC_SH_RLC_RELW16_BSR;
//			opw1=0xF000;
//			opw2=0x2000;
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
			opw2=0xC000;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2000; break;
	case BGBCC_SH_NMID_BSR:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
		{
//			rlty=BGBCC_SH_RLC_RELW16_BSR;
//			opw1=0xF000;
//			opw2=0x2100;
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw1=0xF000;
			opw2=0xD000;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2100; break;
	case BGBCC_SH_NMID_BT:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
		{
//			rlty=BGBCC_SH_RLC_RELW16_BSR;
//			opw1=0xF000;
//			opw2=0x2200;
			rlty=BGBCC_SH_RLC_RELW20_BJX;
#ifdef BGBCC_JX2_OLDBTF
			opw1=0xF000;
			opw2=0xE000;
#else
			opw1=0xE000;
			opw2=0xC000;
#endif
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2200; break;
	case BGBCC_SH_NMID_BF:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
		{
//			rlty=BGBCC_SH_RLC_RELW16_BSR;
//			opw1=0xF000;
//			opw2=0x2300;
			rlty=BGBCC_SH_RLC_RELW20_BJX;
#ifdef BGBCC_JX2_OLDBTF
			opw1=0xF000;
			opw2=0xF000;
#else
			opw1=0xE400;
			opw2=0xC000;
#endif
			break;
		}
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2300; break;

	case BGBCC_SH_NMID_BRA2B:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELW8_BSR;
		opw1=0x2000;
		break;
	case BGBCC_SH_NMID_BRA4B:
		rlty=BGBCC_SH_RLC_RELW20_BJX;
		opw1=0xF000;
		opw2=0xC000;
		break;
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

//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
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

		BGBCC_DBGBREAK

		rlty=BGBCC_SH_RLC_RELW24_BSR;
		opw1=0x25FF;
		opw2=0x26FF;
		opw3=0x26FF;
//		opw4=0x3002;
		opw4=0x3100;
		break;


	case BGBCC_SH_NMID_BRAB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x70080;
		break;
	case BGBCC_SH_NMID_BSRB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x700A0;
		break;
	case BGBCC_SH_NMID_BTB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x700C0;
		break;
	case BGBCC_SH_NMID_BFB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x700E0;
		break;

	case BGBCC_SH_NMID_JMP:
		rlty=BGBCC_SH_RLC_ABS48_BJX;
		opw1=0xFF00;
		opw2=0x0000;
		opw3=0xFA00;
		opw4=0x0000;
		break;
	case BGBCC_SH_NMID_JSR:
		rlty=BGBCC_SH_RLC_ABS48_BJX;
		opw1=0xFF00;
		opw2=0x0000;
		opw3=0xFB00;
		opw4=0x0000;
		break;
	}

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
//		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		if((opw1>=0) && ((opw1&0xE000)!=0xE000))
		{
			opw1=-1;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
		BGBCC_JX2_EmitPadForOpWord2(ctx, opw1, opw2);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<=0xFE00))
		{
//			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4);
			BGBCC_JX2_EmitPadForOpWord2(ctx, opw3, opw4);
		}

		if(opw3>=0)
			BGBCC_JX2_EmitWord(ctx, opw3);
		if(opw4>=0)
			BGBCC_JX2_EmitWord(ctx, opw4);
		return(1);
	}

//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_TryEmitOpNear12Label(BGBCC_JX2_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, opw3, opw4, rlty;

#if 1
	opw1=-1; opw2=-1; opw3=-1; opw4=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAN:
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELW12_BSR;
		opw1=0x7000; break;

	case BGBCC_SH_NMID_BRAB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x70080;
		break;
	case BGBCC_SH_NMID_BSRB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x700A0;
		break;
	case BGBCC_SH_NMID_BTB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x700C0;
		break;
	case BGBCC_SH_NMID_BFB:
		if(ctx->is_fixed32 || ctx->op_is_wex2)
			break;
		rlty=BGBCC_SH_RLC_RELB13_OP24;
		opw1=0x9B00;
		opw2=0x700E0;
		break;

	default:
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
	}
#endif

//	return(BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl));
	return(BGBCC_JX2_TryEmitOpFar20Label(ctx, nmid, lbl));
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
		
		BGBCC_DBGBREAK
		
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

		BGBCC_DBGBREAK

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
#ifdef BGBCC_JX2_OLDBTF
			opw1=0xF000;
			opw2=0xE000;
#else
			opw1=0xE000;
			opw2=0xC000;
#endif
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

		BGBCC_DBGBREAK

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
#ifdef BGBCC_JX2_OLDBTF
			opw1=0xF000;
			opw2=0xF000;
#else
			opw1=0xE400;
			opw2=0xC000;
#endif
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

		BGBCC_DBGBREAK

		rlty=BGBCC_SH_RLC_RELW16_BSR;
//		opw1=0x2400;
//		opw2=0x2601;
		opw1=0x25FF;
		opw2=0x26FF;
//		opw3=0x3032;
		opw3=0x3103;
		break;
	}

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
//		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		if((opw1>=0) && ((opw1&0xE000)!=0xE000))
		{
			opw1=-1;
		}
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack3(ctx, &opw1, &opw2, &opw3);
//		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitPadForOpWord2(ctx, opw1, opw2);
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
#ifdef BGBCC_JX2_OLDBTF
		opw1=0xF000;
		opw2=0xE000;
#else
		opw1=0xE000;
		opw2=0xC000;
#endif
		break;
	case BGBCC_SH_NMID_BF:
		rlty=BGBCC_SH_RLC_RELW20_BJX;
#ifdef BGBCC_JX2_OLDBTF
		opw1=0xF000;
		opw2=0xF000;
#else
		opw1=0xE400;
		opw2=0xC000;
#endif
		break;
	}

	if(opw1>=0)
	{
		BGBCC_JX2DA_EmitOpLabel(ctx, nmid, lbl);
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
//		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitPadForOpWord2(ctx, opw1, opw2);
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
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
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
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
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
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
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
//		if(ctx->is_fixed32)
		if(ctx->is_fixed32 || ctx->op_is_wex2)
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

//	if(ctx->is_fixed32)
	if(ctx->is_fixed32 || ctx->op_is_wex2)
	{
//		if((opw1>=0) && ((opw1&0xF000)!=0xF000))
		if((opw1>=0) && ((opw1&0xE000)!=0xE000))
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
	
//		BGBCC_JX2_EmitOpCheckRepack(ctx, &opw1, &opw2);
		BGBCC_JX2_EmitOpCheckRepack4(ctx, &opw1, &opw2, &opw3, &opw4);
//		BGBCC_JX2_EmitPadForOpWord(ctx, opw1);
		BGBCC_JX2_EmitPadForOpWord2(ctx, opw1, opw2);
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
		if(opw2>=0)
			BGBCC_JX2_EmitWord(ctx, opw2);

//		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<0xFC00))
		if(((opw1&0xFE00)>=0xF000) && ((opw1&0xFE00)<=0xFE00))
		{
//			BGBCC_JX2_EmitOpCheckRepack(ctx, &opw3, &opw4);
//			BGBCC_JX2_EmitPadForOpWord(ctx, opw3);
			BGBCC_JX2_EmitPadForOpWord2(ctx, opw3, opw4);
		}


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

		rngb=224;
//		rngb=244;
//		rngb=248;
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
//			rngb=208;
			rngb=128;
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

int BGBCC_JX2_EmitCheckAutoLabelNear12B(
	BGBCC_JX2_Context *ctx, int lbl, int nmid)
{
	int ra;
	
	ra=BGBCC_JX2_EmitCheckAutoLabelNearClass(ctx, lbl, nmid);
	if((ra>0) && (ra<=2))
		return(ra);
	return(0);
}

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
		rngb=224;
//		rngb=244;
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
	int ndfl, flcl, fn16, flc20, nbra;
	
	ndfl=	(nmid!=BGBCC_SH_NMID_BRA) &&
			(nmid!=BGBCC_SH_NMID_BRAN) &&
			(nmid!=BGBCC_SH_NMID_BSR) &&
			(nmid!=BGBCC_SH_NMID_BSRN) &&
			(nmid!=BGBCC_SH_NMID_BT) &&
			(nmid!=BGBCC_SH_NMID_BF);
	nbra =	(nmid!=BGBCC_SH_NMID_BRA) &&
			(nmid!=BGBCC_SH_NMID_BRAN);

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

	if(ndfl)
	{
		return(BGBCC_JX2_TryEmitOpLabel(ctx, nmid, lbl));
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl, nmid))
	{
		if(ctx->is_fixed32)
		{
			return(BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl));
		}
		
		if(ctx->is_align_wexj&2)
		{
			if(BGBCC_JX2_CheckPadAlign32(ctx) && nbra)
			{
				return(BGBCC_JX2_TryEmitOpFar16Label(ctx, nmid, lbl));
			}
		}
	
		return(BGBCC_JX2_TryEmitOpLabel(ctx, nmid, lbl));
	}

#if 0
	if(BGBCC_JX2_EmitCheckAutoLabelNear12B(ctx, lbl, nmid) && flcl)
	{
		return(BGBCC_JX2_TryEmitOpNear12Label(ctx, nmid, lbl));
	}
#endif

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
