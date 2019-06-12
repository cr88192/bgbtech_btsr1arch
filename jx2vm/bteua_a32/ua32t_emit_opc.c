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

	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpRegRegImm(UA32_Context *ctx,
	int nmid, int rd, int rs, int imm)
{
	int opw1, opw2;
	
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
			opw1=0xA000|((rd&7)<<8)|((imm&255)<<0);
			break;
		}
		if((rs&15)==13)
		{
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
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
}

int UA32_TryEmitOpRegImm(UA32_Context *ctx,
	int nmid, int rd, int imm)
{
	int opw1, opw2;
	
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
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
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
	
	if(opw1>=0)
	{
		UA32_EmitDWord(ctx, opw1);
		if(opw2>=0)
			UA32_EmitDWord(ctx, opw2);
		return(1);
	}
	
	return(0);
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
