/*
ARM Thumb Ops
 */

int UA32T_CheckRegLo(int r0)
{
	if(r0>=8)
		return(0);
	return(1);
}

int UA32T_CheckReg2Lo(int r0, int r1)
{
	if(r0>=8)
		return(0);
	if(r1>=8)
		return(0);
	return(1);
}

int UA32T_CheckReg3Lo(int r0, int r1, int r2)
{
	if(r0>=8)
		return(0);
	if(r1>=8)
		return(0);
	if(r2>=8)
		return(0);
	return(1);
}


int UA32T_CheckRegGpr(int r0)
{
	if(r0>=16)
		return(0);
	return(1);
}

int UA32T_CheckReg2Gpr(int r0, int r1)
{
	if(r0>=16)
		return(0);
	if(r1>=16)
		return(0);
	return(1);
}

int UA32T_CheckReg3Gpr(int r0, int r1, int r2)
{
	if(r0>=16)
		return(0);
	if(r1>=16)
		return(0);
	if(r2>=16)
		return(0);
	return(1);
}

int UA32T_MapImmToImm12(int imm)
{
	int i0, i1, i2, i3;
	int n;

	if(imm<0)
		return(-1);
	if(imm<256)
		return(imm);

	i0=((imm&255)<<16)|(imm&255);
	i1=((imm&255)<<24)|((imm&255)<<8);
	i2=((imm&255)<<24)|((imm&255)<<16)|((imm&255)<<8)|(imm&255);
	
	if(i0==imm)
		return(0x100|(imm&255));
	if(i1==imm)
		return(0x200|(imm&255));
	if(i2==imm)
		return(0x300|(imm&255));
	
	i0=imm; n=0;
	while(n<32)
	{
		if((i0>=128) && (i0<=255))
			break;
		i0=(i0<<1)|((i0>>31)&1);
		n++;
	}
	
	if((n<32) && (n>7))
		return((n<<7)|(i0&127));
	
	return(-1);
}

int UA32T_TryEmitOpLabel(UA32_Context *ctx, int nmid, int lbl)
{
	int opw1, opw2, rty;
	
	opw1=-1; opw2=-1; rty=0;
	
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_B:
		if(nmid&UA32_NMCC_MASK)
		{
			opw1=0xD000|(UA32_GetCcForNmid(ctx, nmid)<<8);
			rty=UA32_RLC_REL8T;
			break;
		}
		opw1=0xE000;
		rty=UA32_RLC_REL11T;
		break;
	case UA32_NMID_BL:
		opw1=0x0xF000;
		opw2=0x0xF800;
		rty=UA32_RLC_REL22T;
		break;
	}
	
	if(opw1>=0)
	{
		if(rty>0)
			UA32_EmitRelocTy(ctx, lbl, rty);
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32T_EmitOpLabel(UA32_Context *ctx, int nmid, int lbl)
{
	int i;
	i=UA32T_TryEmitOpLabel(ctx, nmid, lbl);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

int UA32T_TryEmitOpRegRegReg(UA32_Context *ctx,
	int nmid, int rd, int rs, int rt)
{
	int opw1, opw2;
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_ADD:
		if(!UA32T_CheckReg3Lo(rd, rs, rt))
			break;
		opw1=0x1800|((rt&7)<<6)|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_SUB:
		if(!UA32T_CheckReg3Lo(rd, rs, rt))
			break;
		opw1=0x1A00|((rt&7)<<6)|((rs&7)<<3)|(rd&7);
		break;
	}

	if(opw1<0)
	{
		switch(nmid&UA32_NMID_MASK)
		{
		case UA32_NMID_ADC:
		case UA32_NMID_ADCS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB40|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_ADD:
		case UA32_NMID_ADDS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB00|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_AND:
		case UA32_NMID_ANDS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA00|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_BIC:
		case UA32_NMID_BICS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA20|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_CMN:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB10|(rt&15);
			opw2=0x0F00|(rs&15);
			break;
		case UA32_NMID_CMP:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEBB0|(rt&15);
			opw2=0x0F00|(rs&15);
			break;
		case UA32_NMID_EOR:
		case UA32_NMID_EORS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA80|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_MOV:
		case UA32_NMID_MOVS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA4F|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_MVN:
		case UA32_NMID_MVNS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA6F|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_ORN:
		case UA32_NMID_ORNS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA60|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_ORR:
		case UA32_NMID_ORRS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA40|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_RSB:
		case UA32_NMID_RSBS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEBC0|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_SBC:
		case UA32_NMID_SBCS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB60|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_SUB:
		case UA32_NMID_SUBS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEBA0|(rt&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_TEQ:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA80|(rt&15);
			opw2=0x0F00|(rs&15);
			break;
		case UA32_NMID_TST:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA00|(rt&15);
			opw2=0x0F00|(rs&15);
			break;
		}
	}

	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32T_EmitOpRegRegReg(UA32_Context *ctx,
	int nmid, int rd, int rs, int rt)
{
	int i;
	i=UA32T_TryEmitOpRegRegReg(ctx, nmid, rd, rs, rt);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

int UA32_TryEmitOpRegRegImm(UA32_Context *ctx,
	int nmid, int rd, int rs, int imm)
{
	int im12;
	int opw1, opw2;
	
	im12=UA32T_MapImmToImm12(imm);
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_LSL:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x0000|((imm&31)<<6)|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_LSR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x0800|((imm&31)<<6)|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_ASR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x1000|((imm&31)<<6)|((rs&7)<<3)|(rd&7);
		break;

	case UA32_NMID_ADD:
		if((rs&15)==15)
		{
			if(!UA32T_CheckRegLo(rd))
				break;
			opw1=0xA000|((rd&7)<<8)|((imm&255)<<0);
			break;
		}
		if((rs&15)==13)
		{
			if(!UA32T_CheckRegLo(rd))
				break;
			opw1=0xA800|((rd&7)<<8)|((imm&255)<<0);
			break;
		}
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		if((imm&7)!=imm)
			break;
		opw1=0x1C00|((imm&7)<<6)|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_SUB:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		if((imm&7)!=imm)
			break;
		opw1=0x1E00|((imm&7)<<6)|((rs&7)<<3)|(rd&7);
		break;
	}
	
	if(opw1<0)
	{
		switch(nmid&UA32_NMID_MASK)
		{
		case UA32_NMID_ADC:
		case UA32_NMID_ADCS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF140|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_ADD:
		case UA32_NMID_ADDS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF100|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_AND:
		case UA32_NMID_ANDS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF000|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_BIC:
		case UA32_NMID_BICS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF020|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_CNM:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF110|((im12>>1)&0x0400)|(rs&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		case UA32_NMID_CMP:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF1F0|((im12>>1)&0x0400)|(rs&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		case UA32_NMID_EOR:
		case UA32_NMID_EORS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF080|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_MOV:
		case UA32_NMID_MOVS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF04F|((im12>>1)&0x0400)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_MVN:
		case UA32_NMID_MVNS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF06F|((im12>>1)&0x0400)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_ORN:
		case UA32_NMID_ORNS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF060|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_ORR:
		case UA32_NMID_ORRS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF040|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_RSB:
		case UA32_NMID_RSBS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF1C0|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_SBC:
		case UA32_NMID_SBCS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF160|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_SUB:
		case UA32_NMID_SUBS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF1A0|((im12>>1)&0x0400)|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_TEQ:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF090|((im12>>1)&0x0400)|(rs&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		case UA32_NMID_TST:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xF010|((im12>>1)&0x0400)|(rs&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		}
	}
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32T_EmitOpRegRegImm(UA32_Context *ctx,
	int nmid, int rd, int rs, int imm)
{
	int i;
	i=UA32T_TryEmitOpRegRegImm(ctx, nmid, rd, rs, imm);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

int UA32_TryEmitOpRegImm(UA32_Context *ctx,
	int nmid, int rd, int imm)
{
	int im12;
	int opw1, opw2;
	
	im12=UA32T_MapImmToImm12(imm);
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_MOV:
		if(!UA32T_CheckRegLo(rd))
			break;
		if((imm&255)!=imm)
			break;
		opw1=0x2000|((rd&7)<<8)|(imm&255);
		break;
	case UA32_NMID_CMP:
		if(!UA32T_CheckRegLo(rd))
			break;
		if((imm&255)!=imm)
			break;
		opw1=0x2800|((rd&7)<<8)|(imm&255);
		break;
	case UA32_NMID_ADD:
		if((rd&15)==13)
		{
			if(imm>=0)
				{ opw1=0xB000|((imm>>2)&127); }
			else
				{ opw1=0xB080|(((-imm)>>2)&127); }
			break;
		}
	
		if(!UA32T_CheckRegLo(rd))
			break;
		if((imm&255)!=imm)
			break;
		opw1=0x3000|((rd&7)<<8)|(imm&255);
		break;
	case UA32_NMID_SUB:
		if(!UA32T_CheckRegLo(rd))
			break;
		if((imm&255)!=imm)
			break;
		opw1=0x3800|((rd&7)<<8)|(imm&255);
		break;
	}

	if(opw1<0)
	{
		switch(nmid&UA32_NMID_MASK)
		{
		case UA32_NMID_ADC:
		case UA32_NMID_ADCS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF140|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_ADD:
		case UA32_NMID_ADDS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF100|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_AND:
		case UA32_NMID_ANDS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF000|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_BIC:
		case UA32_NMID_BICS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF020|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_CNM:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF110|((im12>>1)&0x0400)|(rd&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		case UA32_NMID_CMP:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF1F0|((im12>>1)&0x0400)|(rd&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		case UA32_NMID_EOR:
		case UA32_NMID_EORS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF080|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_MOV:
		case UA32_NMID_MOVS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF04F|((im12>>1)&0x0400)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_MVN:
		case UA32_NMID_MVNS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF06F|((im12>>1)&0x0400)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_ORN:
		case UA32_NMID_ORNS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF060|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_ORR:
		case UA32_NMID_ORRS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF040|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_RSB:
		case UA32_NMID_RSBS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF1C0|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_SBC:
		case UA32_NMID_SBCS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF160|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_SUB:
		case UA32_NMID_SUBS:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF1A0|((im12>>1)&0x0400)|(rd&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((im12<<4)&0x7000)|((rd&15)<<8)|(im12&255);
			break;
		case UA32_NMID_TEQ:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF090|((im12>>1)&0x0400)|(rd&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		case UA32_NMID_TST:
			if(!UA32T_CheckRegGpr(rd))			break;
			if(im12<0)							break;
			opw1=0xF010|((im12>>1)&0x0400)|(rd&15);
			opw2=0x0F00|((im12<<4)&0x7000)|(im12&255);
			break;
		}
	}
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32T_EmitOpRegImm(UA32_Context *ctx,
	int nmid, int rd, int imm)
{
	int i;
	i=UA32T_TryEmitOpRegImm(ctx, nmid, rd, imm);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

int UA32T_TryEmitOpRegReg(UA32_Context *ctx,
	int nmid, int rd, int rs)
{
	int opw1, opw2;
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_ADD:
		if((rd&8)||(rs&8))
		{
			opw1=0x4400|((rs&7)<<3)|(rd&7)|((rs&8)<<3)|((rd&8)<<4);
			break;
		}
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x1800|((rs&7)<<6)|((rd&7)<<3)|(rd&7);
		break;
	case UA32_NMID_SUB:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x1A00|((rs&7)<<6)|((rd&7)<<3)|(rd&7);
		break;

	case UA32_NMID_AND:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4000|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_EOR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4040|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_LSL:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4080|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_LSR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x40C0|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_ASR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4100|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_ADC:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4140|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_SBC:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4180|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_ROR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x41C0|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_TST:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4200|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_NEG:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4240|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_CMP:
		if((rd&8)||(rs&8))
		{
			opw1=0x4500|((rs&7)<<3)|(rd&7)|((rs&8)<<3)|((rd&8)<<4);
			break;
		}
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4280|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_CMN:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x42C0|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_ORR:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4300|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_MUL:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4340|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_BIC:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x4380|((rs&7)<<3)|(rd&7);
		break;
	case UA32_NMID_MVN:
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x43C0|((rs&7)<<3)|(rd&7);
		break;

	case UA32_NMID_MOV:
		if((rd&8)||(rs&8))
		{
			opw1=0x4600|((rs&7)<<3)|(rd&7)|((rs&8)<<3)|((rd&8)<<4);
			break;
		}
		if(!UA32T_CheckReg2Lo(rd, rs))
			break;
		opw1=0x0000|(0<<6)|((rs&7)<<3)|(rd&7);
//		opw1=0x43C0|((rs&7)<<3)|(rd&7);
		break;
	}

	if(opw1<0)
	{
		switch(nmid&UA32_NMID_MASK)
		{
		case UA32_NMID_ADC:
		case UA32_NMID_ADCS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB40|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_ADD:
		case UA32_NMID_ADDS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB00|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_AND:
		case UA32_NMID_ANDS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA00|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_BIC:
		case UA32_NMID_BICS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA20|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_CMN:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB10|(rs&15);
			opw2=0x0F00|(rd&15);
			break;
		case UA32_NMID_CMP:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEBB0|(rs&15);
			opw2=0x0F00|(rd&15);
			break;
		case UA32_NMID_EOR:
		case UA32_NMID_EORS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA80|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_MOV:
		case UA32_NMID_MOVS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA4F|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_MVN:
		case UA32_NMID_MVNS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA6F|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rs&15);
			break;
		case UA32_NMID_ORN:
		case UA32_NMID_ORNS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA60|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_ORR:
		case UA32_NMID_ORRS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA40|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_RSB:
		case UA32_NMID_RSBS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEBC0|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_SBC:
		case UA32_NMID_SBCS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEB60|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_SUB:
		case UA32_NMID_SUBS:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEBA0|(rs&15)|((nmid>>3)&0x0010);
			opw2=0x0000|((rd&15)<<8)|(rd&15);
			break;
		case UA32_NMID_TEQ:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA80|(rs&15);
			opw2=0x0F00|(rd&15);
			break;
		case UA32_NMID_TST:
			if(!UA32T_CheckReg2Gpr(rd, rs))		break;
			if(im12<0)							break;
			opw1=0xEA00|(rs&15);
			opw2=0x0F00|(rd&15);
			break;
		}
	}
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32T_EmitOpRegReg(UA32_Context *ctx,
	int nmid, int rd, int rs)
{
	int i;
	i=UA32T_TryEmitOpRegReg(ctx, nmid, rd, rs);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}


int UA32_TryEmitOpReg(UA32_Context *ctx,
	int nmid, int rd)
{
	int opw1, opw2;
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_BX:
		if(!UA32T_CheckRegLo(rd))
			break;
		opw1=0x4700|((rd&15)<<3);
		break;
	}
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}


int UA32T_EmitOpReg(UA32_Context *ctx,
	int nmid, int rd)
{
	int i;
	i=UA32T_TryEmitOpReg(ctx, nmid, rd);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

int UA32_TryEmitOpMemRegRegDisp(UA32_Context *ctx,
	int nmid, int rd, int rb, int disp)
{
	int opw1, opw2;
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_LDR:
		if((rb&15)==15)
		{
			if(!UA32T_CheckRegLo(rd))
				break;
			if((disp&0x3FC)!=disp)
				return(0);
			opw1=0x4800|((rd&7)<<8)|((disp>>2)&255);
			break;
		}
		if((rb&15)==13)
		{
			if(!UA32T_CheckRegLo(rd))
				break;
			if((disp&0x3FC)!=disp)
				return(0);
			opw1=0x9800|((rd&7)<<8)|((disp>>2)&255);
			break;
		}
		if(!UA32T_CheckReg2Lo(rd, rb))
			break;
		if((disp&0x7C)!=disp)
			return(0);
		opw1=0x6800|(((disp>>2)&31)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_STR:
		if((rb&15)==13)
		{
			if(!UA32T_CheckRegLo(rd))
				break;
			opw1=0x9000|((rd&7)<<8)|(disp&255);
			break;
		}
		if(!UA32T_CheckReg2Lo(rd, rb))
			break;
		if((disp&0x7C)!=disp)
			return(0);
		opw1=0x6000|(((disp>>2)&31)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_LDRB:
		if(!UA32T_CheckReg2Lo(rd, rb))
			break;
		if((disp&0x1F)!=disp)
			return(0);
		opw1=0x7800|((disp&31)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_STRB:
		if(!UA32T_CheckReg2Lo(rd, rb))
			break;
		if((disp&0x1F)!=disp)
			return(0);
		opw1=0x7000|((disp&31)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_LDRH:
		if(!UA32T_CheckReg2Lo(rd, rb))
			break;
		if((disp&0x3E)!=disp)
			return(0);
		opw1=0x8800|(((disp>>1)&31)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_STRH:
		if(!UA32T_CheckReg2Lo(rd, rb))
			break;
		if((disp&0x3E)!=disp)
			return(0);
		opw1=0x8000|(((disp>>1)&31)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	}
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32_EmitOpMemRegRegDisp(UA32_Context *ctx,
	int nmid, int rd, int rb, int disp)
{
	int i;
	i=UA32_TryEmitOpMemRegRegDisp(ctx, nmid, rd, rb, disp);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

int UA32_TryEmitOpMemRegReg2(UA32_Context *ctx,
	int nmid, int rd, int rb, int ro)
{
	int opw1, opw2;
	
	opw1=-1; opw2=-1;
	switch(nmid&UA32_NMID_MASK)
	{
	case UA32_NMID_LDR:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5800|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_STR:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5000|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_LDRB:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5C00|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_STRB:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5400|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;


	case UA32_NMID_LDRH:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5A00|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_STRH:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5200|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_LDRSH:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5E00|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	case UA32_NMID_LDRSB:
		if(!UA32T_CheckReg3Lo(rd, rb, ro))
			break;
		opw1=0x5600|((ro&7)<<6)|((rb&7)<<3)|((rd&7)<<0);
		break;
	}
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32_EmitOpMemRegReg2(UA32_Context *ctx,
	int nmid, int rd, int rb, int ro)
{
	int i;
	i=UA32_TryEmitOpMemRegReg2(ctx, nmid, rd, rb, ro);
	if(i<=0) { BJX2_DBGBREAK }
	return(i);
}

