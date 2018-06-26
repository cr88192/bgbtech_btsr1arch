int BGBCC_JX2_EmitCheckRegQuad(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
	{
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegExt4(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
	{
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegExtGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegExtAddrGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if(BGBCC_JX2_EmitCheckRegAddrGPR(ctx, reg))
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
//		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegBaseGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegAddrGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		return(1);
	}

	return(0);
}

int BGBCC_JX2_EmitCheckRegNonBaseGPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(0);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(0);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(0);

	return(1);
}


int BGBCC_JX2_EmitCheckRegBaseFPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);

	return(0);
}

int BGBCC_JX2_EmitCheckRegExtFPR(BGBCC_JX2_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);

//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR16)
//		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
//		return(1);

	return(0);
}

int BGBCC_JX2_EmitLoadDrImm(
	BGBCC_JX2_Context *ctx, s64 imm)
{
	int opw1, opw2, opw3;

#if 1
	if((imm&255)==imm)
	{
		opw1=0x2400|(imm&255);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm|(~255))==imm)
	{
		opw1=0x2500|(imm&255);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}
#endif

#if 1
	if(1)
	{
		if((imm&4095)==imm)
		{
			opw1=0xA000|(imm&4095);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}

		if((imm|(~4095))==imm)
		{
			opw1=0xB000|(imm&4095);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}
	}
#endif

#if 1
	if(1)
	{
		if((imm&16777215)==imm)
		{
			opw1=0xFA00|((imm>>16)&255);
			opw2=(u16)imm;
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			return(1);
		}

		if((imm|(~16777215))==imm)
		{
			opw1=0xFB00|((imm>>16)&255);
			opw2=(u16)imm;
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			return(1);
		}
	}
#endif

#if 1
	if(1)
	{
		if(((u32)imm)==imm)
		{
			opw1=0xFC24;
			opw2=(u16)(imm    );
			opw3=(u16)(imm>>16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			return(1);
		}

		if(((s32)imm)==imm)
		{
			opw1=0xFC25;
			opw2=(u16)(imm    );
			opw3=(u16)(imm>>16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			return(1);
		}
	}
#endif

#if 1
	if((imm&0xFFFFFFFFULL)==0x00000000ULL)
	{
		opw1=0x30C2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFFFFFFFULL)==0xFFFFFFFFULL)
	{
		opw1=0x30D2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}
#endif

#if 0
	if((imm&0xFFFFF)==0x00000)
	{
		opw1=0x30A2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>20);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFFFF)==0xFFFFF)
	{
		opw1=0x30B2;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>20);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}
#endif
	
#if 1
	if((imm&0xFFFF)==0x0000)
	{
		opw1=0x30A0;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>16);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFFF)==0xFFFF)
	{
		opw1=0x30B0;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>16);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}
#endif

#if 0
	if((imm&0xFFF)==0x000)
	{
		opw1=0x3082;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>12);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if((imm&0xFFF)==0xFFF)
	{
		opw1=0x3092;
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>12);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}
#endif

#if 1
	opw1=0xFC26;
	opw2=(u16)(imm    );
	opw3=(u16)(imm>>16);
	BGBCC_JX2_EmitLoadDrImm(ctx, imm>>32);
	BGBCC_JX2_EmitWord(ctx, opw1);
	BGBCC_JX2_EmitWord(ctx, opw2);
	BGBCC_JX2_EmitWord(ctx, opw3);
	return(1);
#endif

#if 0
	opw1=0x2600|(imm&255);
	BGBCC_JX2_EmitLoadDrImm(ctx, imm>>8);
	BGBCC_JX2_EmitWord(ctx, opw1);
	return(1);
#endif
}

int BGBCC_JX2_EmitLoadRegImm(
	BGBCC_JX2_Context *ctx, int nmid, int reg, s32 imm)
{
	u32 imm_f32;
	u16 imm_f16;
	int rt1;
	int opw1, opw2, opw3;

	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(((sbyte)imm)==imm)
		{
			opw1=0xE000|((reg&15)<<8)|(imm&255);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}

		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
			opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
			opw2=(imm&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			return(1);
		}

		if(((s32)imm)==imm)
		{
			opw1=0xFCE0|(reg&15);
			opw2=((imm    )&65535);
			opw3=((imm>>16)&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			return(1);
		}

		opw1=0xF860|(reg&31);
		opw2=(imm&65535);
		BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>16);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>8);

//		opw1=0x4800|((reg&15)<<4)|(imm&15);
//		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>4);
//		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
	{
		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
			opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
			opw2=(imm&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			return(1);
		}

		if(((s32)imm)==imm)
		{
			opw1=0xFDE0|(reg&15);
			opw2=((imm    )&65535);
			opw3=((imm>>16)&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			return(1);
		}

		opw1=0xF860|(reg&31);
		opw2=(imm&65535);
		BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>16);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);

//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		opw1=0x4800|((reg&15)<<4)|(imm&15);
//		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>4);
//		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsDpReg(NULL, ctx, reg) ||
		BGBCC_JX2C_EmitRegIsFpReg(NULL, ctx, reg))
	{
		imm_f32=imm;
		rt1=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
		
		if(rt1>0)
		{
			opw1=0x9F03|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}

		opw1=0x9F00|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm_f32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_JX2_ConstConvDoubleToFloat(u64 v, u32 *rv)
{
	int exp, sgn;
	u64 fra, fra1;
	u32 v1;
	int ret;
	
	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;

	fra=v&0x000FFFFFFFFFFFFFULL;
	exp=(v>>52)&2047;
	sgn=(v>>63)&1;
	
	exp=(exp-1023)+127;
	fra1=fra>>29;
	
	if((fra1<<29)!=fra)
		ret=0;
	
	if(exp<=0)
	{
		ret=0;
		exp=0;
		fra1=0;
	}
	
	if(exp>=255)
	{
		ret=-1;
		exp=255;
		fra1=0;
	}

	v1=(sgn<<31)|(exp<<23)|fra1;
	*rv=v1;
	return(ret);
}


int BGBCC_JX2_ConstConvFloatToHalf(u32 v, u16 *rv)
{
	int exp, sgn;
	u32 fra, fra1;
	u16 v1;
	int ret;
	
	if(!v)
	{
		*rv=0;
		return(1);
	}
	
	ret=1;

	fra=v&0x007FFFFF;
	exp=(v>>23)&255;
	sgn=(v>>31)&1;
	
	exp=(exp-127)+15;
	fra1=fra>>13;
	
	if((fra1<<13)!=fra)
		ret=0;
	
	if(exp<=0)
	{
		ret=0;
		exp=0;
		fra1=0;
	}
	
	if(exp>=31)
	{
		ret=-1;
		exp=31;
		fra1=0;
	}

	v1=(sgn<<15)|(exp<<10)|fra1;
	*rv=v1;
	return(ret);
}

int BGBCC_JX2_EmitLoadRegImm64P(
	BGBCC_JX2_Context *ctx, int reg, s64 imm)
{
	u32 imm_f32;
	u16 imm_f16;
	s32 lo, hi;
	int rt1, rt2;
	int opw1, opw2, opw3;

	if(BGBCC_JX2C_EmitRegIsLpReg(NULL, ctx, reg))
	{
		lo=imm;		hi=imm>>32;
		return(BGBCC_JX2_EmitLoadRegImmPair32(ctx, reg, lo, hi));
	}

	if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(((sbyte)imm)==imm)
		{
			opw1=0xE000|((reg&15)<<8)|(imm&255);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}

		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
			opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
			opw2=(imm&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			return(1);
		}

		if(((s32)imm)==imm)
		{
			opw1=0xFCE0|(reg&15);
			opw2=((imm    )&65535);
			opw3=((imm>>16)&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			return(1);
		}

#if 1
		opw1=0xFCD0|(reg&15);
		opw2=(u16)(imm    );
		opw3=(u16)(imm>>16);
		BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
#endif

#if 0
//		opw1=0xD000|((reg&15)<<8)|(imm&255);
//		BGBCC_JX2_EmitLoadRegImm(ctx, nmid, reg, imm>>8);

		opw1=0x4800|((reg&15)<<4)|(imm&15);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm>>4);

		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
#endif
	}

	if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
	{
		if((((s16)imm)==imm) || (((u16)imm)==imm))
		{
			opw1=0xF800|(reg&31)|((imm>>16)&0x0020);
			opw2=(imm&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			return(1);
		}

		if(((s32)imm)==imm)
		{
			opw1=0xFDE0|(reg&15);
			opw2=((imm    )&65535);
			opw3=((imm>>16)&65535);
			BGBCC_JX2_EmitWord(ctx, opw1);
			BGBCC_JX2_EmitWord(ctx, opw2);
			BGBCC_JX2_EmitWord(ctx, opw3);
			return(1);
		}

#if 1
		opw1=0xFDD0|(reg&15);
		opw2=(u16)(imm    );
		opw3=(u16)(imm>>16);
		BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>32);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		BGBCC_JX2_EmitWord(ctx, opw3);
		return(1);
#endif

#if 0
		opw1=0xF860|(reg&31);
		opw2=(imm&65535);
		BGBCC_JX2_EmitLoadRegImm64P(ctx, reg, imm>>16);
		BGBCC_JX2_EmitWord(ctx, opw1);
		BGBCC_JX2_EmitWord(ctx, opw2);
		return(1);
#endif
	}

	if(BGBCC_JX2C_EmitRegIsDpReg(NULL, ctx, reg) ||
		BGBCC_JX2C_EmitRegIsFpReg(NULL, ctx, reg))
	{
		rt1=BGBCC_JX2_ConstConvDoubleToFloat(imm, &imm_f32);
		rt2=BGBCC_JX2_ConstConvFloatToHalf(imm_f32, &imm_f16);
		
		if(rt1<=0)rt2=0;

		if(rt2>0)
		{
			opw1=0x9F03|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f16);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}

		if(rt1>0)
		{
			opw1=0x9F00|((reg&15)<<4);
			BGBCC_JX2_EmitLoadDrImm(ctx, imm_f32);
			BGBCC_JX2_EmitWord(ctx, opw1);
			return(1);
		}

		opw1=0x9F01|((reg&15)<<4);
		BGBCC_JX2_EmitLoadDrImm(ctx, imm);
		BGBCC_JX2_EmitWord(ctx, opw1);
		return(1);
	}

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegImmPair32(
	BGBCC_JX2_Context *ctx, int reg, s32 imma, s32 immb)
{
	int reg2;

	if(BGBCC_JX2C_EmitRegIsLpReg(NULL, ctx, reg))
	{
		reg2=BGBCC_SH_REG_R0+(reg&15);
		BGBCC_JX2_EmitLoadRegImm(ctx, 0, reg2+0, imma);
		BGBCC_JX2_EmitLoadRegImm(ctx, 0, reg2+1, immb);
		return(1);
	}

	if(BGBCC_JX2C_EmitRegIsDpReg(NULL, ctx, reg))
	{
		BGBCC_JX2_EmitLoadRegImm64P(ctx, reg,
			(((u64)immb)<<32)|((u32)imma));
		return(1);
	}

	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitCheckFlushIndexImmP(BGBCC_JX2_Context *ctx)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitCheckFlushIndexImm(BGBCC_JX2_Context *ctx)
{
	return(0);
}

int BGBCC_JX2_EmitForceFlushIndexImm(BGBCC_JX2_Context *ctx)
{
	return(0);
}

int BGBCC_JX2_EmitFlushIndexImmBasic(BGBCC_JX2_Context *ctx)
{
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel(
	BGBCC_JX2_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelAbs(
	BGBCC_JX2_Context *ctx, int reg, int lbl)
{
	BGBCC_JX2_EmitOpLblReg(ctx, BGBCC_SH_NMID_MOV, lbl, reg);
//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel32(
	BGBCC_JX2_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel16(
	BGBCC_JX2_Context *ctx, int reg, int lbl, int ofs)
{
	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelRel24(
	BGBCC_JX2_Context *ctx, int reg, int lbl)
{
	BGBCC_JX2_EmitOpLblReg(ctx, BGBCC_SH_NMID_MOV, lbl, reg);
	return(1);

//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_JX2_EmitLoadRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opw5, opwf, rlty, prlty;
	int tr0;

	if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitLoadRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_MOV, tr0, reg);
			BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
			return(1);
		}

		BGBCC_DBGBREAK
	}

	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl))
		{ prlty=BGBCC_SH_RLC_RELW8_BSR;}
	else if(BGBCC_JX2_ModelIsLabel16P(ctx))
	{
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW16A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW16C_BSR;
		if((nmid==BGBCC_SH_NMID_MOVUB) ||
			(nmid==BGBCC_SH_NMID_MOVUW) ||
			(nmid==BGBCC_SH_NMID_MOVUL))
				prlty=BGBCC_SH_RLC_RELW16C_BSR;
	}
	else if(BGBCC_JX2_ModelIsData24P(ctx))
	{
#if 0
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW24A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW24C_BSR;
		if((nmid==BGBCC_SH_NMID_MOVUB) ||
			(nmid==BGBCC_SH_NMID_MOVUW) ||
			(nmid==BGBCC_SH_NMID_MOVUL))
				prlty=BGBCC_SH_RLC_RELW24C_BSR;
#endif
		prlty=BGBCC_SH_RLC_REL24_BJX;
	}
	else
	{
		prlty=BGBCC_SH_RLC_RELW32C_BSR;
	}

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; rlty=-1;
	opwf=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4400|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x300C|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVW:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4500|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x300D|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVL:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4600|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x300E|((reg&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_MOVQ:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4700|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x300F|((reg&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVUB:
		rlty=prlty;
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opwf=0x3504|((reg&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_MOVUW:
		rlty=prlty;
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opwf=0x3505|((reg&15)<<4); break; }
		break;
	case BGBCC_SH_NMID_MOVUL:
		rlty=prlty;
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
			{ opwf=0x3506|((reg&15)<<4); break; }
		break;
	}
	
	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_RELW8C_BSR)
//			{ opw1=0x25FE; opw2=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
//			{ opw1=0x25FF; opw2=0x26FE; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
			{ opw1=0xF0FF; opw2=0x25FE; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW24C_BSR)
			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW32C_BSR)
			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=0x26FE; opw5=opwf; }

		if(rlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ opw1=0xA000; opw2=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW24A_BSR)
			{ opw1=0xA000; opw2=0x2600; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW32A_BSR)
			{ opw1=0xA000; opw2=0x2600; opw3=0x2600; opw4=opwf; }

		if(rlty==BGBCC_SH_RLC_REL24_BJX)
			{ opw1=0xFBFF; opw2=0xFFFE; opw3=opwf; }
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
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

	BGBCC_DBGBREAK
	return(0);

//	BGBCC_DBGBREAK
//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_JX2_EmitStoreRegLabelVarRel24(
	BGBCC_JX2_Context *ctx, int nmid, int reg, int lbl)
{
	int opw1, opw2, opw3, opw4, opw5, opwf, rlty, prlty;

	int tr0;

	if(!BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
	{
		if(BGBCC_JX2_EmitCheckRegExtGPR(ctx, reg))
		{
			tr0=BGBCC_JX2C_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
			BGBCC_JX2_EmitOpRegReg(ctx,
				BGBCC_SH_NMID_MOV, reg, tr0);
			BGBCC_JX2_EmitStoreRegLabelVarRel24(ctx, nmid, tr0, lbl);
			BGBCC_JX2C_ScratchReleaseReg(NULL, ctx, tr0);
			return(1);
		}

		BGBCC_DBGBREAK
	}
	if(BGBCC_JX2_EmitCheckAutoLabelNear8(ctx, lbl))
		{ prlty=BGBCC_SH_RLC_RELW8_BSR;}
	else if(BGBCC_JX2_ModelIsLabel16P(ctx))
	{
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW16A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW16C_BSR;
	}
	else if(BGBCC_JX2_ModelIsData24P(ctx))
	{
#if 0
//		prlty=BGBCC_SH_RLC_RELW16C_BSR;
		prlty=BGBCC_SH_RLC_RELW24A_BSR;
		if((reg&15)==15)
			prlty=BGBCC_SH_RLC_RELW24C_BSR;
#endif
		prlty=BGBCC_SH_RLC_REL24_BJX;
	}
	else
		{ prlty=BGBCC_SH_RLC_RELW32C_BSR; }

	opw1=-1; opw2=-1; opw3=-1; opw4=-1; opw5=-1; rlty=-1;
	opwf=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVUB:
	case BGBCC_SH_NMID_MOVB:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4000|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x3004|((reg&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVUW:
	case BGBCC_SH_NMID_MOVW:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4100|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x3005|((reg&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVUL:
	case BGBCC_SH_NMID_MOVL:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4200|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x3006|((reg&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(BGBCC_JX2_EmitCheckRegBaseGPR(ctx, reg))
		{
			if((prlty==BGBCC_SH_RLC_RELW16A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW24A_BSR) ||
				(prlty==BGBCC_SH_RLC_RELW32A_BSR))
				{ rlty=prlty; opwf=0x4300|((reg&15)<<4); break; }
			rlty=prlty;
			opwf=0x3007|((reg&15)<<4);
			break;
		}
		break;
	}
	
	if((rlty>=0) && (opw1<0) && (opwf>=0))
	{
//		if(rlty==BGBCC_SH_RLC_RELW8C_BSR)
//			{ opw1=0x25FE; opw2=opwf; }
//		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
//			{ opw1=0x25FF; opw2=0x26FE; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW16C_BSR)
			{ opw1=0xF0FF; opw2=0x25FE; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW24C_BSR)
			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW32C_BSR)
			{ opw1=0x25FF; opw2=0x26FF; opw3=0x26FE; opw4=0x26FE; opw5=opwf; }

		if(rlty==BGBCC_SH_RLC_RELW16A_BSR)
			{ opw1=0xA000; opw2=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW24A_BSR)
			{ opw1=0xA000; opw2=0x26FF; opw3=opwf; }
		if(rlty==BGBCC_SH_RLC_RELW32A_BSR)
			{ opw1=0xA000; opw2=0x26FF; opw3=0x26FF; opw4=opwf; }

		if(rlty==BGBCC_SH_RLC_REL24_BJX)
			{ opw1=0xFBFF; opw2=0xFFFE; opw3=opwf; }
	}

	if(opw1>=0)
	{
		if(rlty>0)
			{ BGBCC_JX2_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_JX2_EmitWord(ctx, opw1);
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

	BGBCC_DBGBREAK
	return(0);
}


