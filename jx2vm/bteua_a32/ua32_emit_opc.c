int UA32_GetCcForNmid(UA32_Context *ctx, int nmid)
{
	if((nmid&UA32_NMCC_MASK)==UA32_NMCC_AL)
		return(UA32_CC_AL);
	if((nmid&UA32_NMCC_MASK)==UA32_NMCC_EQ)
		return(UA32_CC_EQ);
	return((nmid>>UA32_NMCC_SHL)&15);
}

int UA32_TryEmitOpLabel(UA32_Context *ctx, int nmid, int lbl)
{
	u32 opw, opw1, rty;
	
	opw=0; rty=0;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_B:
		opw=0x0A000000;
		rty=UA32_RLC_REL24W;
		break;
	case UA32_NMID_BL:
		opw=0x0B000000;
		rty=UA32_RLC_REL24W;
		break;
	}
	
	if(opw)
	{
		opw1=(UA32_GetCcForNmid(ctx, nmid)<<28)|opw;
		UA32_EmitRelocTy(ctx, lbl, rty);
		UA32_EmitDWord(ctx, opw1);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpRegRegReg(UA32_Context *ctx,
	int nmid, int rd, int rs, int rt, int shl)
{
	u32 opw, opw1;
	int md;
	
	opw=0xFFFFFFFF; md=-1;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_AND:		opw=0x00000000;	md=1;	break;
	case UA32_NMID_EOR:		opw=0x00200000;	md=1;	break;
	case UA32_NMID_SUB:		opw=0x00400000;	md=1;	break;
	case UA32_NMID_RSB:		opw=0x00600000;	md=1;	break;
	case UA32_NMID_ADD:		opw=0x00800000;	md=1;	break;
	case UA32_NMID_ADC:		opw=0x00A00000;	md=1;	break;
	case UA32_NMID_SBC:		opw=0x00C00000;	md=1;	break;
	case UA32_NMID_RSC:		opw=0x00E00000;	md=1;	break;
	case UA32_NMID_TST:		opw=0x01100000;	md=1;	break;
	case UA32_NMID_TEQ:		opw=0x01300000;	md=1;	break;
	case UA32_NMID_CMP:		opw=0x01500000;	md=1;	break;
	case UA32_NMID_CMN:		opw=0x01700000;	md=1;	break;
	case UA32_NMID_ORR:		opw=0x01800000;	md=1;	break;
	case UA32_NMID_MOV:		opw=0x01A00000;	md=1;	break;
	case UA32_NMID_BIC:		opw=0x01C00000;	md=1;	break;
	case UA32_NMID_MVN:		opw=0x01E00000;	md=1;	break;

	case UA32_NMID_ANDS:	opw=0x00100000;	md=1;	break;
	case UA32_NMID_EORS:	opw=0x00300000;	md=1;	break;
	case UA32_NMID_SUBS:	opw=0x00500000;	md=1;	break;
	case UA32_NMID_RSBS:	opw=0x00700000;	md=1;	break;
	case UA32_NMID_ADDS:	opw=0x00900000;	md=1;	break;
	case UA32_NMID_ADCS:	opw=0x00B00000;	md=1;	break;
	case UA32_NMID_SBCS:	opw=0x00D00000;	md=1;	break;
	case UA32_NMID_RSCS:	opw=0x00F00000;	md=1;	break;
	case UA32_NMID_TSTS:	opw=0x01100000;	md=1;	break;
	case UA32_NMID_TEQS:	opw=0x01300000;	md=1;	break;
	case UA32_NMID_CMPS:	opw=0x01500000;	md=1;	break;
	case UA32_NMID_CMNS:	opw=0x01700000;	md=1;	break;
	case UA32_NMID_ORRS:	opw=0x01900000;	md=1;	break;
	case UA32_NMID_MOVS:	opw=0x01B00000;	md=1;	break;
	case UA32_NMID_BICS:	opw=0x01D00000;	md=1;	break;
	case UA32_NMID_MVNS:	opw=0x01F00000;	md=1;	break;

	case UA32_NMID_MUL:		opw=0x00000090;	md=2;	break;
	case UA32_NMID_MLA:		opw=0x00200090;	md=2;	break;
	case UA32_NMID_MULS:	opw=0x00100090;	md=2;	break;
	case UA32_NMID_MLAS:	opw=0x00300090;	md=2;	break;

	case UA32_NMID_SMULL:	opw=0x00800090;	md=2;	break;
	case UA32_NMID_SMLAL:	opw=0x00A00090;	md=2;	break;
	case UA32_NMID_SMULL:	opw=0x00C00090;	md=2;	break;
	case UA32_NMID_SMLAL:	opw=0x00E00090;	md=2;	break;
	case UA32_NMID_UMULLS:	opw=0x00900090;	md=2;	break;
	case UA32_NMID_UMLALS:	opw=0x00B00090;	md=2;	break;
	case UA32_NMID_UMULLS:	opw=0x00D00090;	md=2;	break;
	case UA32_NMID_UMLALS:	opw=0x00F00090;	md=2;	break;
	}

	if(md==1)
	{
//		shl=((shl<<3)&0xF8)|((shl>>5)&0x07);
		opw|=((rs&15)<<16)|((rd&15)<<12)|((rt&15)<<0);
//		opw|=((shl&255)<<4);
		if(shl&0x20)
			opw|=((shl<<8)&0xF00)|((shl>>1)&0x070);
		else
			opw|=((shl<<7)&0xF80)|((shl>>1)&0x070);
	}
	
	if(md==2)
	{
		opw|=((rd&15)<<16)|((shl&15)<<12)|
			((rs&15)<<8)|((rt&15)<<0);
	}
	
	if(opw!=0xFFFFFFFF)
	{
		opw1=(UA32_GetCcForNmid(ctx, nmid)<<28)|opw;
		UA32_EmitDWord(ctx, opw1);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpRegReg(UA32_Context *ctx,
	int nmid, int rd, int rt)
{
	return(UA32_TryEmitOpRegRegReg(ctx, nmid, rd, rd, rt,
		UA32_SHL_LSL_I|0));
}

int UA32_TryEmitOpRegRegImm(UA32_Context *ctx,
	int nmid, int rd, int rs, int imma)
{
	int imm, shl;
	u32 opw, opw1;

	if(imm<0)
		return(0);

	imm=imma; shl=0;
	while((imm>255) && !(imm&3))
		{ imm=imm>>2; shl+=2; }
	if(imm>255)
		return(0);
	
	opw=0xFFFFFFFF;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_AND:		opw=0x02000000;	break;
	case UA32_NMID_EOR:		opw=0x02200000;	break;
	case UA32_NMID_SUB:		opw=0x02400000;	break;
	case UA32_NMID_RSB:		opw=0x02600000;	break;
	case UA32_NMID_ADD:		opw=0x02800000;	break;
	case UA32_NMID_ADC:		opw=0x02A00000;	break;
	case UA32_NMID_SBC:		opw=0x02C00000;	break;
	case UA32_NMID_RSC:		opw=0x02E00000;	break;
	case UA32_NMID_TST:		opw=0x03100000;	break;
	case UA32_NMID_TEQ:		opw=0x03300000;	break;
	case UA32_NMID_CMP:		opw=0x03500000;	break;
	case UA32_NMID_CMN:		opw=0x03700000;	break;
	case UA32_NMID_ORR:		opw=0x03800000;	break;
	case UA32_NMID_MOV:		opw=0x03A00000;	break;
	case UA32_NMID_BIC:		opw=0x03C00000;	break;
	case UA32_NMID_MVN:		opw=0x03E00000;	break;
	case UA32_NMID_ANDS:	opw=0x02100000;	break;
	case UA32_NMID_EORS:	opw=0x02300000;	break;
	case UA32_NMID_SUBS:	opw=0x02500000;	break;
	case UA32_NMID_RSBS:	opw=0x02700000;	break;
	case UA32_NMID_ADDS:	opw=0x02900000;	break;
	case UA32_NMID_ADCS:	opw=0x02B00000;	break;
	case UA32_NMID_SBCS:	opw=0x02D00000;	break;
	case UA32_NMID_RSCS:	opw=0x02F00000;	break;
	case UA32_NMID_TSTS:	opw=0x03100000;	break;
	case UA32_NMID_TEQS:	opw=0x03300000;	break;
	case UA32_NMID_CMPS:	opw=0x03500000;	break;
	case UA32_NMID_CMNS:	opw=0x03700000;	break;
	case UA32_NMID_ORRS:	opw=0x03900000;	break;
	case UA32_NMID_MOVS:	opw=0x03B00000;	break;
	case UA32_NMID_BICS:	opw=0x03D00000;	break;
	case UA32_NMID_MVNS:	opw=0x03F00000;	break;
	}

	if(((opw>>26)&3)==0)
	{
		opw|=((rs&15)<<16)|((rd&15)<<12);
		opw|=((shl&30)<<8)|(imm&255);
	}
	
	if(opw!=0xFFFFFFFF)
	{
		opw1=(UA32_GetCcForNmid(ctx, nmid)<<28)|opw;
		UA32_EmitDWord(ctx, opw1);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpRegImm(UA32_Context *ctx,
	int nmid, int rd, int imma)
{
	return(UA32_TryEmitOpRegRegImm(ctx, nmid, rd, rd, imma));
}

int UA32_TryEmitOpMemDisp(UA32_Context *ctx,
	int nmid, int rd, int rb, int disp)
{
	int neg, md;
	u32 opw, opw1;

	neg=0;
	if(disp<0)
	{ disp=-disp; neg=1; }
	
	opw=0xFFFFFFFF; md=0;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_LDR:		opw=0x04100000; md=1; break;
	case UA32_NMID_STR:		opw=0x04000000; md=1; break;
	case UA32_NMID_LDRB:	opw=0x04500000; md=1; break;
	case UA32_NMID_STRB:	opw=0x04400000; md=1; break;

	case UA32_NMID_LDRH:	opw=0x005000B0; md=2; break;
	case UA32_NMID_STRH:	opw=0x004000B0; md=2; break;
	case UA32_NMID_LDRSB:	opw=0x005000D0; md=2; break;
	case UA32_NMID_STRSB:	opw=0x004000D0; md=2; break;
	}
	
	if(md==1)
	{
		opw|=((rd&15)<<16)|((rb&15)<<12)|(disp&4095);
	}

	if(md==2)
	{
		opw|=((rb&15)<<16)|((rd&15)<<12);
		opw|=((disp<<8)&0xF00)|(disp&0x00F);
	}

	if(opw!=0xFFFFFFFF)
	{
		opw1=(UA32_GetCcForNmid(ctx, nmid)<<28)|opw;
		UA32_EmitDWord(ctx, opw1);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpMemIdx(UA32_Context *ctx,
	int nmid, int rd, int rb, int ri, int sh)
{
	int neg, md;
	u32 opw, opw1;

	opw=0xFFFFFFFF; md=0;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_LDR:		opw=0x06100000; md=1; break;
	case UA32_NMID_STR:		opw=0x06000000; md=1; break;
	case UA32_NMID_LDRB:	opw=0x06500000; md=1; break;
	case UA32_NMID_STRB:	opw=0x06400000; md=1; break;

	case UA32_NMID_LDRH:	opw=0x001000B0; md=2; break;
	case UA32_NMID_STRH:	opw=0x000000B0; md=2; break;
	case UA32_NMID_LDRSB:	opw=0x001000D0; md=2; break;
	case UA32_NMID_STRSB:	opw=0x000000D0; md=2; break;
	}
	
	if(md==1)
	{
		opw|=((rd&15)<<16)|((rb&15)<<12);
		opw|=((ri&15)<<0);

		if(shl&0x20)
			opw|=((shl<<8)&0xF00)|((shl>>1)&0x070);
		else
			opw|=((shl<<7)&0xF80)|((shl>>1)&0x070);

		if(nmid&UA32_NMPI_MASK)
			opw|=(nmid&UA32_NMPI_MASK)<<(21-UA32_NMPI_SHL);
		else
			opw|=0x01000000;
	}

	if(md==2)
	{
		opw|=((rb&15)<<16)|((rd&15)<<12);
		opw|=((ri&15)<<0);

		if(nmid&UA32_NMPI_MASK)
			opw|=(nmid&UA32_NMPI_MASK)<<(21-UA32_NMPI_SHL);
		else
			opw|=0x01000000;
	}

	if(opw!=0xFFFFFFFF)
	{
		opw1=(UA32_GetCcForNmid(ctx, nmid)<<28)|opw;
		UA32_EmitDWord(ctx, opw1);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpReg(UA32_Context *ctx,
	int nmid, int rd)
{
	int neg, md;
	u32 opw, opw1;
	int i;

	if((nmid&UA32_NMID_MASK)==UA32_NMID_PUSH)
	{
		i=UA32_TryEmitOpMemDisp(ctx,
			(nmid&UA32_NMCC_MASK)|
			UA32_NMID_STR|UA32_NMPI_DB,
			rd, UA32_REG_SP, 4);
		return(i);
	}

	if((nmid&UA32_NMID_MASK)==UA32_NMID_POP)
	{
		i=UA32_TryEmitOpMemDisp(ctx,
			(nmid&UA32_NMCC_MASK)|
			UA32_NMID_LDR|UA32_NMPI_IA,
			rd, UA32_REG_SP, 4);
		return(i);
	}

	if((nmid&UA32_NMID_MASK)==UA32_NMID_B)
	{
		i=UA32_TryEmitOpReg(ctx,
			(nmid&UA32_NMCC_MASK)|
			UA32_NMID_BX,
			rd);
		return(i);
	}

	if((nmid&UA32_NMID_MASK)==UA32_NMID_BL)
	{
		i=UA32_TryEmitOpRegReg(ctx,
			(nmid&UA32_NMCC_MASK)|
			UA32_NMID_MOV,
			UA32_REG_LR, UA32_REG_PC);
		if(i<=0)
			return(i);
//		i=UA32_TryEmitOpReg(ctx,
//			(nmid&UA32_NMCC_MASK)|
//			UA32_NMID_BX,
//			rd);
		i=UA32_TryEmitOpRegReg(ctx,
			(nmid&UA32_NMCC_MASK)|
			UA32_NMID_MOV,
			UA32_REG_PC, rd);
		return(i);
	}

	opw=0xFFFFFFFF; md=0;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_BX:		opw=0x012FFF10; md=1; break;
	}
	
	if(md==1)
	{
		opw|=(rd&15);
	}

	if(opw!=0xFFFFFFFF)
	{
		opw1=(UA32_GetCcForNmid(ctx, nmid)<<28)|opw;
		UA32_EmitDWord(ctx, opw1);
		return(1);
	}
	
	return(0);
}
