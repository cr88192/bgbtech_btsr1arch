#include <bgbccc.h>


int BGBCC_SHX_EmitIndexAddImm16(BGBCC_SHX_Context *ctx, int imm)
{
	int i, n, o;

	for(i=0; i<ctx->const_ns16; i++)
	{
		if((ctx->const_s16tab[i]==imm) &&
			!(ctx->const_s16tab_rlc[i]))
		{
			n=ctx->nofs_s16tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s16tab[n]=o;
			ctx->idx_s16tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns16++;
	ctx->const_s16tab[i]=imm;
	ctx->const_s16tab_rlc[i]=0;

	n=ctx->nofs_s16tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s16tab[n]=o;
	ctx->idx_s16tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm32(BGBCC_SHX_Context *ctx, int imm)
{
	int i, n, o;

	if(!imm)
	{
		n=-1;
	}

	for(i=0; i<ctx->const_ns32; i++)
	{
		if((ctx->const_s32tab[i]==imm) &&
			!(ctx->const_s32tab_rlc[i]))
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns32++;
	ctx->const_s32tab[i]=imm;
	ctx->const_s32tab_rlc[i]=0;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImmPair32(BGBCC_SHX_Context *ctx,
	s32 imma, s32 immb)
{
	int i, n, o;

	if(!imma && !immb)
	{
		n=-1;
	}

	for(i=0; i<(ctx->const_ns32-1); i++)
	{
		if(	(ctx->const_s32tab[i+0]==imma) &&
			(ctx->const_s32tab[i+1]==immb) &&
			!(ctx->const_s32tab_rlc[i+0]) &&
			!(ctx->const_s32tab_rlc[i+1]))
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns32;
	ctx->const_ns32+=2;
	ctx->const_s32tab[i+0]=imma;
	ctx->const_s32tab[i+1]=immb;
	ctx->const_s32tab_rlc[i+0]=0;
	ctx->const_s32tab_rlc[i+1]=0;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm32LblOfs(
	BGBCC_SHX_Context *ctx, int lbl, int ofs)
{
	int rlc;
	int i, n, o;

	rlc=lbl|(BGBCC_SH_RLC_REL32<<24);

	i=ctx->const_ns32++;
	ctx->const_s32tab[i]=ofs;
	ctx->const_s32tab_rlc[i]=rlc;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm32LblAbs(
	BGBCC_SHX_Context *ctx, int lbl)
{
	int rlc;
	int i, n, o;

	rlc=lbl|(BGBCC_SH_RLC_ABS32<<24);

	for(i=0; i<ctx->const_ns32; i++)
	{
		if(ctx->is_simpass==1)	//debug
			break;

		if((ctx->const_s32tab[i]==0) &&
			(ctx->const_s32tab_rlc[i]==rlc))
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}

	i=ctx->const_ns32++;
	ctx->const_s32tab[i]=0;
	ctx->const_s32tab_rlc[i]=rlc;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm16LblOfs(
	BGBCC_SHX_Context *ctx, int lbl, int ofs)
{
	int rlc;
	int i, n, o;

	rlc=lbl|(BGBCC_SH_RLC_REL16<<24);

	i=ctx->const_ns16++;
	ctx->const_s16tab[i]=ofs;
	ctx->const_s16tab_rlc[i]=rlc;

	n=ctx->nofs_s16tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s16tab[n]=o;
	ctx->idx_s16tab[n]=i;
	return(i);
}

int bgbcc_shx_hf16tof32i(u16 hf)
{
	int sg, ex, fr;
	u32 v;
	
	sg=(hf>>15)&1;
	ex=(hf>>10)&31;
	fr=hf&0x03FF;
	
	ex+=112;
	fr=fr<<13;
	
	v=(sg<<31)|(ex<<23)|fr;
	return(v);
}

int BGBCC_SHX_EmitLoadRegImm(
	BGBCC_SHX_Context *ctx, int nmid, int reg, s32 imm)
{
	int i, j;
	
	j=ctx->has_bjx1mov;

	if(ctx->no_ext32)
		ctx->has_bjx1mov=0;
	
	i=BGBCC_SHX_EmitLoadRegImmI(ctx, nmid, reg, imm);

	ctx->has_bjx1mov=j;
	
	return(i);
}

int BGBCC_SHX_EmitLoadRegImmI(
	BGBCC_SHX_Context *ctx, int nmid, int reg, s32 imm)
{
	u32 v, v1;
	byte isgpr;
	int sg, ex, fr;
	int opw, opw1, opw2, opw3, opw4, opw5, opw6, opw7;
	int i0, i1, i2, i3;
	int i, j, k;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg) &&
		!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
	{
		if(ctx->no_fpu)
			{ BGBCC_DBGBREAK }

		if((imm==0x00000000) &&
			!BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
		{
			opw=0xF08D|(((reg+0)&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			return(1);
		}

#if 1
		if((imm==0x3F800000) &&
			!BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
		{
			opw=0xF09D|(((reg+0)&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			return(1);
		}
#endif
	
#if 0
		if(ctx->has_movi20)
		{
			if(((imm<<12)>>12)==imm)
			{
				opw=0x0000|((reg&15)<<8)|((imm>>12)&0xF0);
				opw2=imm&0xFFFF;
				BGBCC_SHX_EmitWord(ctx, opw);
				BGBCC_SHX_EmitWord(ctx, opw2);
				return(1);
			}
		}
#endif

//		if(ctx->has_bjx1mov &&
//			!BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
		if(ctx->has_bjx1mov)
		{
			sg=(imm>>31)&1;
			ex=(imm>>23)&255;
			fr=imm&0x7FFFFF;

//			if(ex>1)
			if(1)
			{
				ex=(ex-127)+15;
				if((ex<1) || (ex>30))
//				if((ex<0) || (ex>31))
					{ ex=0; fr=0; sg=-1; }
				if(fr&0x1FFF)
					sg=-1;
				fr=fr>>13;
				v=(sg<<15)|(ex<<10)|fr;
			}else
			{
				v=0;
			}

			if(sg>=0)
			{
//				if(ctx->sreg_live&1)
//					{ BGBCC_DBGBREAK }
			
				opw1=0x8E00|((v>>8)&255);
				opw2=0x8A00|(v&0x00FF);
				opw3=0xF00D|((reg&15)<<8);
				
				v1=bgbcc_shx_hf16tof32i(v);
				if(v1!=imm)
					{ BGBCC_DBGBREAK }

				BGBCC_SHX_EmitWord(ctx, opw1);
				BGBCC_SHX_EmitWord(ctx, opw2);
				BGBCC_SHX_EmitWord(ctx, opw3);
				return(1);
			}
		}

#if 1
		if(ctx->has_bjx1mov && !(imm&0x0000FFFF))
		{
			v=imm>>16;

			opw1=0x8E00|((v>>8)&0x00FF);
			opw2=0x8C00|((v   )&0x00FF);
			opw3=0xF00D|((reg&15)<<8);

			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			return(1);
		}
#endif

		if(!imm)
		{
			opw1=0xE000;
			opw2=0x405A;
			opw3=0xF00D|((reg&15)<<8);

			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			return(1);
		}

		if(!ctx->use_onlyimm)
//		if(1)
		{
			opw=0xC700;
			opw2=0xF009|(((reg+0)&15)<<8);

			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitIndexAddImm32(ctx, imm);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}

		if(!(ctx->sreg_live&1))
		{
			opw1=0x405A;
			opw2=0xF00D|((reg&15)<<8);
			BGBCC_SHX_EmitLoadRegImm(ctx, 0, BGBCC_SH_REG_R0, imm);
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
	
		BGBCC_DBGBREAK
		return(0);
	}

#if 1
	ctx->stat_tot_imm++;
	if(((sbyte)imm)==imm)
		{ ctx->stat_tot_imm8++; }
	else if(((s16)imm)==imm)
		{ ctx->stat_tot_imm16++; }
	else
	{
		for(i=0; i<16; i++)
		{
			i0=(((imm>>(i*2))&255)<<(i*2));
			i1=i0|(~(255<<(i*2)));
			if(i0==imm)
				break;
			if(i1==imm)
				break;
		}
		if(i<16)
			{ ctx->stat_tot_imm8r++; }
		else
			{ ctx->stat_tot_imm32++; }
	}
#endif


	if(BGBCC_SHXC_EmitRegIsLpReg(NULL, ctx, reg))
		{ return(BGBCC_SHX_EmitLoadRegImm64P(ctx, reg, imm)); }

//	isgpr=BGBCC_SHX_EmitCheckRegGPR(ctx, reg);
	isgpr=BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg);

//	if((((sbyte)imm)==imm) && ((reg&0xF0)==0x00))
//	if((((sbyte)imm)==imm) && BGBCC_SHX_EmitCheckRegGPR(ctx, reg))
	if((((sbyte)imm)==imm) && isgpr)
	{
		opw=0xE000|((reg&15)<<8)|(imm&255);
		BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}

#if 0
	if(ctx->has_movi20 || ctx->has_bjx1mov)
	{
		if((((s16)imm)==imm) && ((reg&0xF0)==0x00))
		{
			opw=0x9000|((reg&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitIndexAddImm16(ctx, imm);
			return(1);
		}
	}
#endif

	if(ctx->has_movi20)
//	if(0)
	{
//		if((((imm<<12)>>12)==imm) && ((reg&0xF0)==0x00))
		if((((imm<<12)>>12)==imm) && isgpr)
		{
			opw=0x0000|((reg&15)<<8)|((imm>>12)&0xF0);
			opw2=imm&0xFFFF;
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}

//		if(((((imm<<4)>>4)&(~255))==imm) && ((reg&0xF0)==0x00))
		if(((((imm<<4)>>4)&(~255))==imm) && isgpr)
		{
			opw=0x0001|((reg&15)<<8)|((imm>>20)&0xF0);
			opw2=(imm>>8)&0xFFFF;
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
	}

	if(ctx->has_bjx1mov)
//	if(0)
//	if(1)
	{
#if 1
//		if((((imm<<8)>>8)==imm) && (reg==BGBCC_SH_REG_R0))
		if((((imm<<8)>>8)==imm) && isgpr && !(reg&31))
		{
			opw=0x8A00|((imm>>16)&0xFF);
			opw2=imm&0xFFFF;
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
#endif

#if 1
//		if((((imm<<16)>>16)==imm) && ((reg&0xF0)==0x00))
//		if((((imm<<16)>>16)==imm) && isgpr && !(reg&31))
		if((((imm<<16)>>16)==imm) && isgpr)
		{
			opw=0x8E00|((imm>>8)&0xFF);
			opw2=0xE000|((reg&15)<<8)|(imm&0xFF);
//			opw=0x8E00|(imm&0xFF);
//			opw2=0xE000|((reg&15)<<8)|((imm>>8)&0xFF);
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
#endif
	}

#if 0
	if(!(imm&3) && (((sbyte)(imm>>2))==(imm>>2)) && ((reg&0xF0)==0x00))
	{
		opw=0xE000|((reg&15)<<8)|((imm>>2)&255);
		opw2=0x4008|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

//	if(!(imm&255) && (((sbyte)(imm>>8))==(imm>>8)) && ((reg&0xF0)==0x00))
	if(!(imm&255) && (((sbyte)(imm>>8))==(imm>>8)) && isgpr)
	{
		opw=0xE000|((reg&15)<<8)|((imm>>8)&255);
		opw2=0x4018|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}

//	if(!(imm&65535) && (((sbyte)(imm>>16))==(imm>>16)) && ((reg&0xF0)==0x00))
	if(!(imm&65535) && (((sbyte)(imm>>16))==(imm>>16)) && isgpr)
	{
		opw=0xE000|((reg&15)<<8)|((imm>>16)&255);
		opw2=0x4028|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}

	if(ctx->has_bjx1mov)
	{
#if 1
//		if(ctx->use_onlyimm&2)

//		if((((imm<<8)>>8)==imm) && ((reg&0xF0)==0x00))
		if((((imm<<8)>>8)==imm) && isgpr)
		{
			v=imm;
//			if(!ctx->is_betav)
			if(1)
			{
				if(v&0x00008000)v+=0x00010000;
			}

			if(((((s32)v)<<8)>>8)==v)
			{
				opw1=0xE000|((v>>16)&0xFF)|((reg&15)<<8);
				opw2=0x8E00|((v>> 8)&0xFF);
				opw3=0x9000|((v    )&0xFF)|((reg&15)<<8);

				BGBCC_SHX_EmitWord(ctx, opw1);
				BGBCC_SHX_EmitWord(ctx, opw2);
				BGBCC_SHX_EmitWord(ctx, opw3);
				return(1);
			}
		}
#endif
	}

//	if((((s16)imm)==imm) && ((reg&0xF0)==0x00) && !ctx->use_onlyimm)
	if((((s16)imm)==imm) && isgpr && !ctx->use_onlyimm &&
		BGBCC_SHX_EmitCheckModeClrDQ(ctx, BGBCC_SH_NMID_MOVW))
	{
		opw=0x9000|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm);
		return(1);
	}

#if 1
//	if(!(imm&255) && (((s16)(imm>>8))==(imm>>8)) &&
//		((reg&0xF0)==0x00) && !ctx->use_onlyimm)
	if(!(imm&255) && (((s16)(imm>>8))==(imm>>8)) &&
		isgpr && !ctx->use_onlyimm &&
		BGBCC_SHX_EmitCheckModeClrDQ(ctx, BGBCC_SH_NMID_MOVW))
	{
		opw=0x9000|((reg&15)<<8);
		opw2=0x4018|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm>>8);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

#if 1
//	if(!(imm&65535) && (((s16)(imm>>16))==(imm>>16)) &&
//		((reg&0xF0)==0x00) && !ctx->use_onlyimm)
	if(!(imm&65535) && (((s16)(imm>>16))==(imm>>16)) &&
		isgpr && !ctx->use_onlyimm &&
		BGBCC_SHX_EmitCheckModeClrDQ(ctx, BGBCC_SH_NMID_MOVW))
	{
		opw=0x9000|((reg&15)<<8);
		opw2=0x4028|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm>>16);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

//	if((((s32)imm)==imm) && ((reg&0xF0)==0x00) && !ctx->use_onlyimm)
	if((((s32)imm)==imm) && isgpr && !ctx->use_onlyimm)
	{
	
		if(((s16)imm)==imm)
		{
			BGBCC_DBGBREAK
		}
	
		opw=0xD000|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm32(ctx, imm);
		return(1);
	}

	if(ctx->has_bjx1mov)
//	if(0)
	{
#if 1
//		if(((imm&(~255))==imm) && (reg==BGBCC_SH_REG_R0))
		if(((imm&(~255))==imm) && isgpr && !(reg&31))
		{
			opw1=0x8A00|((imm>>24)&0xFF);
			opw2=(imm>>8)&0xFFFF;
			opw3=0x4018;
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			return(1);
		}
#endif

#if 1
		if((((imm<<8)>>8)==imm) && isgpr && !(ctx->sreg_live&1))
		{
			opw=0x8A00|((imm>>16)&0xFF);
			opw2=imm&0xFFFF;
			opw3=0x6003|((reg&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			return(1);
		}
#endif

#if 0
		if(!(ctx->sreg_live&1))
		{
			opw=0x8A00|((imm>>24)&0xFF);
			opw2=(imm>>8)&0xFFFF;
			opw3=0x4018;
			opw4=0x7000|(imm&0xFF);
			opw5=0x6003|((reg&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			BGBCC_SHX_EmitWord(ctx, opw4);
			BGBCC_SHX_EmitWord(ctx, opw5);
			return(1);
		}
#endif

//		if((ctx->use_onlyimm&2) && isgpr)
//		if(BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		if(isgpr)
		{
			v=imm;
//			if(!ctx->is_betav)
			if(1)
			{
				if(v&0x00008000)v+=0x00010000;
			}

			opw1=0x8E00|((v>>24)&0xFF);
			opw2=0xE000|((v>>16)&0xFF)|((reg&15)<<8);
			opw3=0x8E00|((v>> 8)&0xFF);
			opw4=0x9000|((v    )&0xFF)|((reg&15)<<8);

			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			BGBCC_SHX_EmitWord(ctx, opw4);
			return(1);
		}

		if(isgpr)
		{
			v=imm;
			if(v&0x00008000)v+=0x00010000;

			opw1=0x8E00|((v>>24)&0xFF);
			opw2=0xE000|((v>>16)&0xFF)|((reg&15)<<8);
			opw3=0x4028|((reg&15)<<8);
			opw4=0x8E00|((v>> 8)&0xFF);
			opw5=0x7000|((v    )&0xFF)|((reg&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			BGBCC_SHX_EmitWord(ctx, opw4);
			BGBCC_SHX_EmitWord(ctx, opw5);
			return(1);
		}
	}

	if(isgpr)
	{
		v=imm;
		if(v&0x00000080)v+=0x00000100;
		if(v&0x00008000)v+=0x00010000;
		if(v&0x00800000)v+=0x01000000;
	
		opw1=0xE000|((v>>24)&0xFF)|((reg&15)<<8);
		opw2=0x4018|((reg&15)<<8);
		opw3=0x7000|((v>>16)&0xFF)|((reg&15)<<8);
		opw4=0x4018|((reg&15)<<8);
		opw5=0x7000|((v>> 8)&0xFF)|((reg&15)<<8);
		opw6=0x4018|((reg&15)<<8);
		opw7=0x7000|((v    )&0xFF)|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw1);
		BGBCC_SHX_EmitWord(ctx, opw2);
		BGBCC_SHX_EmitWord(ctx, opw3);
		BGBCC_SHX_EmitWord(ctx, opw4);
		BGBCC_SHX_EmitWord(ctx, opw5);
		BGBCC_SHX_EmitWord(ctx, opw6);
		BGBCC_SHX_EmitWord(ctx, opw7);
		return(1);
	}
	
	if(BGBCC_SHX_EmitCheckRegExtGPR(ctx, reg))
	{
#if 0
		if(((sbyte)imm)==imm)
		{
			opw1=0xCC00|((reg>>2)&4);
			opw2=0xE000|((reg&15)<<8)|(imm&255);
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
#endif

#if 1
//		if(((((s32)imm)<<18)>>18)==imm)
		if((((s16)imm)==imm) && (reg&0x10))
		{
//			opw1=0xCE00|((reg<<3)&0x0080)|((imm>>8)&63);
			opw1=0xCE00|((imm>>8)&255);
			opw2=0xE000|((reg&15)<<8)|(imm&255);
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
#endif

#if 1
		if(reg&0x10)
		{
			v=imm;
//			if(!ctx->is_betav)
			if(1)
			{
				if(v&0x00008000)v+=0x00010000;
			}

			opw1=0xCE00|((v>>24)&0xFF);
			opw2=0xE000|((v>>16)&0xFF)|((reg&15)<<8);
			opw3=0xCE00|((v>> 8)&0xFF);
			opw4=0x9000|((v    )&0xFF)|((reg&15)<<8);

			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			BGBCC_SHX_EmitWord(ctx, opw4);
			return(1);
		}
#endif

		if(!(ctx->sreg_live&1))
		{
//			if(BGBCC_SHX_EmitCheckModeDQ(ctx))
//			if(1)
			if(!BGBCC_SHX_EmitCheckRegQuad(ctx, reg))
			{
				BGBCC_SHX_EmitLoadRegImm(ctx, BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_RD0, imm);
				BGBCC_SHX_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_RD0, reg);
				return(1);
			}else
			{
				BGBCC_SHX_EmitLoadRegImm(ctx, BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_RD0, imm);
//				BGBCC_SHX_EmitOpRegReg(ctx, BGBCC_SH_NMID_EXTSL,
//					BGBCC_SH_REG_RQ0, reg);
				BGBCC_SHX_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_RQ0, reg);
				return(1);
			}
		}

		BGBCC_DBGBREAK
	}
	
	BGBCC_DBGBREAK
	return(0);
}

int BGBCC_SHX_EmitLoadRegImm64P(
	BGBCC_SHX_Context *ctx, int reg, s64 imm)
{
	int opw1, opw2, opw3, opw4;
	int opw5, opw6, opw7, opw8;
	u64 lv;
	s32 lo, hi;
	int i;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	if(BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg) &&
		!BGBCC_SHX_EmitCheckRegQuad(ctx, reg) &&
		((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_LR0))
	{
		BGBCC_SHX_EmitLoadRegImm(ctx,
			BGBCC_SH_NMID_MOVL, reg, (s32)imm);
		return(1);
	}

	if(BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg) &&
		BGBCC_SHX_EmitCheckRegQuad(ctx, reg))
	{
		if(((s32)imm)==imm)
		{
			BGBCC_SHX_EmitLoadRegImm(ctx,
				BGBCC_SH_NMID_MOVL, reg, imm);
			return(1);
		}
	
		if(ctx->has_bjx1mov)
		{
			lv=imm;
//			if(!ctx->is_betav)
			if(1)
			{
				if(lv&0x0000000000008000)lv+=0x0000000000010000;
				if(lv&0x0000000080000000)lv+=0x0000000100000000;
				if(lv&0x0000800000000000)lv+=0x0001000000000000;
			}

			opw1=-1; opw2=-1; opw3=-1; opw4=-1;
			opw5=-1; opw6=-1; opw7=-1; opw8=-1;

#if 1
			if(lv>>56)
//			if(lv>>55)
			{
				opw1=0x8E00|((lv>>56)&0xFF);
				opw2=0xE000|((lv>>48)&0xFF)|((reg&15)<<8);
				if((lv>>32)&65535)
				{
					opw3=0x8E00|((lv>>40)&0xFF);
					opw4=0x9000|((lv>>32)&0xFF)|((reg&15)<<8);
				}else
				{
					opw3=0x4028|((reg&15)<<8);
				}
			}else if(lv>>48)
//			}else if(lv>>47)
			{
//				opw1=0x8E00|((lv>>56)&0xFF);
				opw2=0xE000|((lv>>48)&0xFF)|((reg&15)<<8);
//				opw3=0x8E00|((lv>>40)&0xFF);
//				opw4=0x9000|((lv>>32)&0xFF)|((reg&15)<<8);
				if((lv>>32)&65535)
				{
					opw3=0x8E00|((lv>>40)&0xFF);
					opw4=0x9000|((lv>>32)&0xFF)|((reg&15)<<8);
				}else
				{
					opw3=0x4028|((reg&15)<<8);
				}
			}else if(lv>>40)
//			}else if(lv>>39)
			{
				opw3=0x8E00|((lv>>40)&0xFF);
				opw4=0xE000|((lv>>32)&0xFF)|((reg&15)<<8);
			}else
			{
				opw4=0xE000|((lv>>32)&0xFF)|((reg&15)<<8);
			}
#endif

#if 0
//			if(ctx->is_betav)
			if(((lv>>32)&1)==((imm>>32)&1))
			{
				opw1=-1; opw2=-1; opw3=-1; opw4=-1;
				BGBCC_SHX_EmitLoadRegImm(ctx,
					BGBCC_SH_NMID_MOVL, reg, imm>>32);
			}
#endif
			
			if(((u32)lv)!=0)
			{
				if((lv>>16)&65535)
				{
					opw5=0x8E00|((lv>>24)&0xFF);
					opw6=0x9000|((lv>>16)&0xFF)|((reg&15)<<8);
				}else
				{
					opw5=0x4028|((reg&15)<<8);
				}
				if((lv    )&65535)
				{
					opw7=0x8E00|((lv>> 8)&0xFF);
					opw8=0x9000|((lv    )&0xFF)|((reg&15)<<8);
				}else
				{
					opw7=0x4028|((reg&15)<<8);
				}
			}else
			{
				opw5=0x4038|((reg&15)<<8);
			}

			if(opw1>=0)
				BGBCC_SHX_EmitWord(ctx, opw1);
			if(opw2>=0)
				BGBCC_SHX_EmitWord(ctx, opw2);
			if(opw3>=0)
				BGBCC_SHX_EmitWord(ctx, opw3);
			if(opw4>=0)
				BGBCC_SHX_EmitWord(ctx, opw4);
			if(opw5>=0)
				BGBCC_SHX_EmitWord(ctx, opw5);
			if(opw6>=0)
				BGBCC_SHX_EmitWord(ctx, opw6);
			if(opw7>=0)
				BGBCC_SHX_EmitWord(ctx, opw7);
			if(opw8>=0)
				BGBCC_SHX_EmitWord(ctx, opw8);
			return(1);
		}else
		{
			lv=imm;
			if(lv&0x0000000000000080ULL)lv+=0x0000000000000100ULL;
			if(lv&0x0000000000008000ULL)lv+=0x0000000000010000ULL;
			if(lv&0x0000000000800000ULL)lv+=0x0000000001000000ULL;
			if(lv&0x0000000080000000ULL)lv+=0x0000000100000000ULL;
			if(lv&0x0000008000000000ULL)lv+=0x0000010000000000ULL;
			if(lv&0x0000800000000000ULL)lv+=0x0001000000000000ULL;
			if(lv&0x0080000000000000ULL)lv+=0x0100000000000000ULL;

			opw1=-1; opw2=-1; opw3=-1; opw4=-1;
			opw5=-1; opw6=-1; opw7=-1; opw8=-1;

#if 0
			if(lv>>55)
			{
				opw1=0xE000|((lv>>56)&0xFF)|((reg&15)<<8);
				opw2=0x4018|((reg&15)<<8);
				if((lv>>48)&255)
					opw3=0x7000|((lv>>48)&0xFF)|((reg&15)<<8);
			}else
			{
				opw1=0xE000|((lv>>48)&0xFF)|((reg&15)<<8);
			}

			if((lv>>32)&65535)
			{
				if((lv>>40)&255)
				{
					opw4=0x4018|((reg&15)<<8);
					opw5=0x7000|((lv>>40)&0xFF)|((reg&15)<<8);
					opw6=0x4018|((reg&15)<<8);
				}else
				{
					opw4=0x4028|((reg&15)<<8);
				}
				if((lv>>32)&255)
					opw7=0x7000|((lv>>32)&0xFF)|((reg&15)<<8);
			}else
			{
				opw4=0x4028|((reg&15)<<8);
			}
#endif

#if 0
//			if(ctx->is_betav)
			if(((lv>>32)&1)==((imm>>32)&1))
			{
				opw1=-1; opw2=-1; opw3=-1; opw4=-1;
				opw5=-1; opw6=-1; opw7=-1; opw8=-1;
				BGBCC_SHX_EmitLoadRegImm(ctx,
					BGBCC_SH_NMID_MOVL, reg, imm>>32);
			}
#endif

			BGBCC_SHX_EmitLoadRegImm(ctx,
				BGBCC_SH_NMID_MOVL, reg, (imm>>32)+((imm>>31)&1));

			if(opw1>=0)
				BGBCC_SHX_EmitWord(ctx, opw1);
			if(opw2>=0)
				BGBCC_SHX_EmitWord(ctx, opw2);
			if(opw3>=0)
				BGBCC_SHX_EmitWord(ctx, opw3);
			if(opw4>=0)
				BGBCC_SHX_EmitWord(ctx, opw4);
			if(opw5>=0)
				BGBCC_SHX_EmitWord(ctx, opw5);
			if(opw6>=0)
				BGBCC_SHX_EmitWord(ctx, opw6);
			if(opw7>=0)
				BGBCC_SHX_EmitWord(ctx, opw7);

//			if(((u32)lv)!=0)
			if(lv&0xFFFFFF00)
			{
				opw1=-1; opw2=-1; opw3=-1; opw4=-1;
				opw5=-1; opw6=-1; opw7=-1; opw8=-1;

				if((lv>>16)&65535)
				{
					if((lv>>24)&255)
					{
						opw1=0x4018|((reg&15)<<8);
						opw2=0x7000|((lv>>24)&0xFF)|((reg&15)<<8);
						opw3=0x4018|((reg&15)<<8);
					}else
					{
						opw1=0x4028|((reg&15)<<8);
					}
					if((lv>>16)&255)
						opw4=0x7000|((lv>>16)&0xFF)|((reg&15)<<8);
				}else
				{
					opw1=0x4028|((reg&15)<<8);
				}

				if((lv    )&65535)
				{
					if((lv>>8)&255)
					{
						opw5=0x4018|((reg&15)<<8);
						opw6=0x7000|((lv>> 8)&0xFF)|((reg&15)<<8);
						opw7=0x4018|((reg&15)<<8);
					}else
					{
						opw5=0x4028|((reg&15)<<8);
					}
					if((lv   )&255)
						opw8=0x7000|((lv    )&0xFF)|((reg&15)<<8);
				}else
				{
					opw5=0x4028|((reg&15)<<8);
				}

				if(opw1>=0)
					BGBCC_SHX_EmitWord(ctx, opw1);
				if(opw2>=0)
					BGBCC_SHX_EmitWord(ctx, opw2);
				if(opw3>=0)
					BGBCC_SHX_EmitWord(ctx, opw3);
				if(opw4>=0)
					BGBCC_SHX_EmitWord(ctx, opw4);
				if(opw5>=0)
					BGBCC_SHX_EmitWord(ctx, opw5);
				if(opw6>=0)
					BGBCC_SHX_EmitWord(ctx, opw6);
				if(opw7>=0)
					BGBCC_SHX_EmitWord(ctx, opw7);
				if(opw8>=0)
					BGBCC_SHX_EmitWord(ctx, opw8);
			}else
			{
				opw1=-1; opw2=-1; opw3=-1; opw4=-1;

				opw1=0x4038|((reg&15)<<8);
				if((lv   )&255)
					opw2=0x7000|((lv    )&0xFF)|((reg&15)<<8);

				if(opw1>=0)
					BGBCC_SHX_EmitWord(ctx, opw1);
				if(opw2>=0)
					BGBCC_SHX_EmitWord(ctx, opw2);
//				BGBCC_SHX_EmitWord(ctx, opw1);
			}
			return(1);
		}
	}

	if(BGBCC_SHX_EmitCheckRegExtGPR(ctx, reg) &&
		ctx->is_addr64 && ctx->has_bjx1egpr)
	{
		if(ctx->has_bjx1mov)
		{
			lv=imm;
//			if(!ctx->is_betav)
			if(1)
			{
				if(lv&0x0000000000008000)lv+=0x0000000000010000;
				if(lv&0x0000000080000000)lv+=0x0000000100000000;
				if(lv&0x0000800000000000)lv+=0x0001000000000000;
			}
			opw1=-1; opw2=-1; opw3=-1; opw4=-1;

			opw5=0xCE00|((lv>>24)&0xFF);
			opw6=0x9000|((lv>>16)&0xFF)|((reg&15)<<8);
			opw7=0xCE00|((lv>> 8)&0xFF);
			opw8=0x9000|((lv    )&0xFF)|((reg&15)<<8);

			if(lv>>48)
//			if(lv>>47)
			{
				opw1=0xCE00|((lv>>56)&0xFF);
				opw2=0xE000|((lv>>48)&0xFF)|((reg&15)<<8);
				opw3=0xCE00|((lv>>40)&0xFF);
				opw4=0x9000|((lv>>32)&0xFF)|((reg&15)<<8);
			}else if(lv>>32)
//			}else if(lv>>31)
			{
				opw3=0xCE00|((lv>>40)&0xFF);
				opw4=0xE000|((lv>>32)&0xFF)|((reg&15)<<8);
			}else if(lv>>16)
//			}else if(lv>>15)
			{
				opw5=0xCE00|((lv>>24)&0xFF);
				opw6=0xE000|((lv>>16)&0xFF)|((reg&15)<<8);
				opw7=0xCE00|((lv>> 8)&0xFF);
				opw8=0x9000|((lv    )&0xFF)|((reg&15)<<8);
			}else
			{
				opw5=-1;
				opw6=-1;
				opw7=0xCE00|((lv>> 8)&0xFF);
				opw8=0xE000|((lv    )&0xFF)|((reg&15)<<8);
			}
			
			if(opw1>=0)
				BGBCC_SHX_EmitWord(ctx, opw1);
			if(opw2>=0)
				BGBCC_SHX_EmitWord(ctx, opw2);
			if(opw3>=0)
				BGBCC_SHX_EmitWord(ctx, opw3);
			if(opw4>=0)
				BGBCC_SHX_EmitWord(ctx, opw4);
			if(opw5>=0)
				BGBCC_SHX_EmitWord(ctx, opw5);
			if(opw6>=0)
				BGBCC_SHX_EmitWord(ctx, opw6);
			if(opw7>=0)
				BGBCC_SHX_EmitWord(ctx, opw7);
			if(opw8>=0)
				BGBCC_SHX_EmitWord(ctx, opw8);
			return(1);
		}

		for(i=0; i<8; i++)
		{
			if(!(ctx->sreg_live&(1<<i)))
			{
				BGBCC_SHX_EmitLoadRegImm64P(ctx, BGBCC_SH_REG_RQ0+i, imm);

//				opw1=0xCC00|((reg>>2)&4);
//				opw2=0x6003|((reg&15)<<8)|(i<<4);
//				BGBCC_SHX_EmitWord(ctx, opw1);
//				BGBCC_SHX_EmitWord(ctx, opw2);
				BGBCC_SHX_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
					BGBCC_SH_REG_RQ0+i, reg);

				return(1);
			}
		}

		BGBCC_DBGBREAK
		return(0);
	}

	lo=imm; hi=imm>>32;
	i=BGBCC_SHX_EmitLoadRegImmPair32(ctx, reg, lo, hi);
	return(i);
}

int BGBCC_SHX_EmitLoadRegImmPair32(
	BGBCC_SHX_Context *ctx, int reg, s32 imma, s32 immb)
{
	int reg2;
	u32 v;
	int sg, ex, fr;
	int opw, opw1, opw2, opw3, opw4;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	reg2=reg;
//	if(BGBCC_SHXC_EmitRegIsDpReg(NULL, ctx, reg))
//		{ reg2=BGBCC_SH_REG_FR0+((reg&7)*2); }
	if(BGBCC_SHXC_EmitRegIsDpReg(NULL, ctx, reg))
		{ reg2=BGBCC_SH_REG_FR0+(reg&15); }
	if(BGBCC_SHXC_EmitRegIsLpReg(NULL, ctx, reg))
		{ reg2=BGBCC_SH_REG_R0+(reg&15); }

#if 1
	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2) &&
		BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
	{
		if(ctx->has_bjx1mov)
		{
			sg=(immb>>31)&1;
			ex=(immb>>20)&2047;
			fr=immb&0x000FFFFF;
			if(imma || (fr&0x03FF))
				sg=-1;

//			if(ex>1)
			if(1)
			{
				ex=(ex-1023)+15;
				if((ex<1) || (ex>30))
					{ ex=0; fr=0; sg=-1; }
				fr=fr>>10;
				v=(sg<<15)|(ex<<10)|fr;
			}else
			{
				v=0;
			}

			if(sg>=0)
			{
				opw1=0x8E00|((v>>8)&0x00FF);
				opw2=0x8A00|(v&0x00FF);
				opw3=0xF0AD|((reg2&15)<<8);

				BGBCC_SHX_EmitWord(ctx, opw1);
				BGBCC_SHX_EmitWord(ctx, opw2);
				BGBCC_SHX_EmitWord(ctx, opw3);
				return(1);
			}
		}
	}
#endif

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2) &&
		!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx) &&
		!(ctx->sreg_live&1) &&
		!ctx->use_onlyimm)
//		1)
	{
#if 1
		if(!imma && !immb)
		{
			opw1=0xE000;
			opw2=0x405A;
			opw3=0xF00D|(((reg2+1)&15)<<8);
			opw4=0xF00D|(((reg2+0)&15)<<8);

			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			BGBCC_SHX_EmitWord(ctx, opw3);
			BGBCC_SHX_EmitWord(ctx, opw4);
			return(1);
		}
#endif

		if(!imma)
		{
			BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+0, immb);
			BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+1, imma);
			return(1);
		}

		opw=0xC700;
		opw1=0xF009|(((reg2+1)&15)<<8);
		opw2=0xF009|(((reg2+0)&15)<<8);

		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImmPair32(ctx, imma, immb);
		BGBCC_SHX_EmitWord(ctx, opw1);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2) &&
		BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx) &&
		!(ctx->sreg_live&1) &&
		!ctx->use_onlyimm)
//		1)
	{
		opw=0xC700;
		opw1=0xF009|(((reg2+0)&15)<<8);

		BGBCC_SHX_EmitWord(ctx, opw);
		if(BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
			BGBCC_SHX_EmitIndexAddImmPair32(ctx, imma, immb);
		else
			BGBCC_SHX_EmitIndexAddImmPair32(ctx, immb, imma);
		BGBCC_SHX_EmitWord(ctx, opw1);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2))
//	if(0)
	{
		BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+0, immb);
		BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+1, imma);
		return(1);
	}
	
	BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+0, imma);
	BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+1, immb);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelRel(
	BGBCC_SHX_Context *ctx, int reg, int lbl, int ofs)
{
	int opw0, opw1, opw2, opw3;
	int opw;

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

#if 1
	if(ctx->has_bjx1mov &&
		!(ctx->sreg_live&1))
	{
		opw=-ofs;
		opw0=0x8A00|((opw>>16)&0x00FF);
		opw1=(opw&0xFFFF);
//		opw2=0x40F5|((reg&15)<<8);
		opw2=-1;
		if((reg&15)!=0)
			opw2=0x6003|((reg&15)<<8);

		BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL24_BJX);
		BGBCC_SHX_EmitWord(ctx, opw0);
		BGBCC_SHX_EmitWord(ctx, opw1);
		if(opw>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

//	if(ctx->use_onlyimm)
//		{ BGBCC_DBGBREAK }

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0xD000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm32LblOfs(ctx, lbl, -ofs);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelAbs(
	BGBCC_SHX_Context *ctx, int reg, int lbl)
{
	int opw;

	if(ctx->use_onlyimm)
		{ BGBCC_DBGBREAK }

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0xD000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm32LblAbs(ctx, lbl);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelRel32(
	BGBCC_SHX_Context *ctx, int reg, int lbl, int ofs)
{
	int opw0, opw1, opw2, opw3;
	int opw;

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0xD000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm32LblOfs(ctx, lbl, -ofs);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelRel16(
	BGBCC_SHX_Context *ctx, int reg, int lbl, int ofs)
{
	int opw;

	if(ctx->use_onlyimm)
		{ BGBCC_DBGBREAK }

//	if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, BGBCC_SH_NMID_MOVW))
	if(1)
	{
//		BGBCC_DBGBREAK
		BGBCC_SHX_EmitLoadRegLabelRel32(ctx, reg, lbl, ofs);
//		BGBCC_SHX_EmitLoadRegLabelRel(ctx, reg, lbl, ofs);
		return(1);
	}

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0x9000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm16LblOfs(ctx, lbl, -ofs);
	return(1);
}


#if 1
int BGBCC_SHX_EmitLoadRegLabelRel24(
	BGBCC_SHX_Context *ctx, int reg, int lbl)
{
	int opw0, opw1, opw2;
	int ob;

	if(!ctx->has_bjx1mov ||
		(ctx->sreg_live&1))
//	if(1)
	{
//		__debugbreak();
	
		BGBCC_SHX_EmitLoadRegLabelAbs(ctx, reg, lbl);
		return(1);
	}

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
	{
//		BGBCC_DBGBREAK
	
		BGBCC_SHXC_ScratchSafeStompReg(NULL, ctx, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitLoadRegLabelRel24(ctx, BGBCC_SH_REG_R0, lbl);
		BGBCC_SHX_EmitOpRegReg(ctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, reg);
		BGBCC_SHXC_ScratchReleaseReg(NULL, ctx, BGBCC_SH_REG_R0);
		return(1);
//		BGBCC_DBGBREAK
	}

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);
	ob=BGBCC_SHX_EmitGetOffs(ctx);

//	opw0=0x8A00;
//	opw1=(ob&2)?0x0006:0x0008;
//	opw1=0x0008;
	opw0=0x8AFF;
	opw1=0xFFF8;
	opw2=0x40F5|((reg&15)<<8);

	BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL24_BJX);
	BGBCC_SHX_EmitWord(ctx, opw0);
	BGBCC_SHX_EmitWord(ctx, opw1);
	BGBCC_SHX_EmitWord(ctx, opw2);

//	opw=0xD000|((reg&15)<<8);
//	BGBCC_SHX_EmitWord(ctx, opw);
//	BGBCC_SHX_EmitIndexAddImm32LblAbs(ctx, lbl);
	return(1);
}
#endif

#if 1
int BGBCC_SHX_EmitLoadRegLabelVarRel24(
	BGBCC_SHX_Context *ctx, int op, int reg, int lbl)
{
	int rngw, rngw16, rngw20;
	int opw0, opw1, opw2, opw3;
	int ob;
	int i, j, k;

	if(!ctx->has_bjx1mov)
	{
//		BGBCC_SHX_EmitLoadRegLabelAbs(ctx, reg, lbl);
		BGBCC_DBGBREAK
		return(1);
	}

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

	if(ctx->sreg_live&1)
	{
		BGBCC_DBGBREAK
	}

	if(op==BGBCC_SH_NMID_JSR)
	{
		i=BGBCC_SHX_LookupSimLabelIndex(ctx, lbl);
		if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && (i<ctx->nlbl))
		{
			j=ctx->lbl_ofs[i];
			rngw=4080;
			rngw16=65280;
			rngw20=983040;

#if 1
			k=BGBCC_SHX_EmitGetOffs(ctx);
			j=j-(k+4);
			if(j<0)j=-j;
			
			if((j<rngw) && !ctx->is_betav)
			{
				BGBCC_SHX_EmitOpNear12Label(ctx,
					BGBCC_SH_NMID_BSR, lbl);
				BGBCC_SHX_EmitOpNone(ctx,
					BGBCC_SH_NMID_NOP);
				return(1);
			}

#if 1
			if((j<rngw16) && ctx->has_bjx1jmp)
			{
				BGBCC_SHX_EmitOpFar16Label(ctx,
					BGBCC_SH_NMID_BSRN, lbl);
//				BGBCC_SHX_EmitOpNone(ctx,
//					BGBCC_SH_NMID_NOP);
				return(1);
			}
#endif

#if 0
			if(j<rngw20)
			{
				BGBCC_SHX_EmitOpFar16Label(ctx,
					BGBCC_SH_NMID_BSR, lbl);
				BGBCC_SHX_EmitOpNone(ctx,
					BGBCC_SH_NMID_NOP);
				return(1);
			}
#endif

#endif
		}
	}

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);
	ob=BGBCC_SHX_EmitGetOffs(ctx);

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }
	
	opw3=-1;

	if(op==BGBCC_SH_NMID_MOVQ)
	{
		opw0=0x8AFF;
		opw1=0xFFF8+(ob&2);
//		if(BGBCC_SHX_EmitCheckModeDQ(ctx))
		if(0)
			{ opw2=0x40D5|((reg&15)<<8); }
		else
		{
			opw2=0x40A5|((reg&15)<<8);
//			opw2=0xCC08; opw3=0x40E5|((reg&15)<<8);
		}
	}else if(op==BGBCC_SH_NMID_MOVL)
	{
		opw0=0x8AFF;
		opw1=0xFFF8+(ob&2);
		opw2=0x40E5|((reg&15)<<8);
	}else if(op==BGBCC_SH_NMID_FMOVS)
	{
		if(ctx->no_fpu)
			{ BGBCC_DBGBREAK }

		opw0=0x8AFF;
		opw1=0xFFF8+(ob&2);
		opw2=0x40B5|((reg&15)<<8);
	}else
	{
		opw0=0x8AFF;
		opw1=0xFFF8;
		if(op==BGBCC_SH_NMID_MOVB)
			opw2=0x40C5|((reg&15)<<8);
		if(op==BGBCC_SH_NMID_MOVW)
			opw2=0x40D5|((reg&15)<<8);
		if(op==BGBCC_SH_NMID_JSR)
		{
			opw2=0x0003;
			opw3=0x0009;
		}
	}

	BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL24_BJX);
	BGBCC_SHX_EmitWord(ctx, opw0);
	BGBCC_SHX_EmitWord(ctx, opw1);
	BGBCC_SHX_EmitWord(ctx, opw2);
	if(opw3>0)
		BGBCC_SHX_EmitWord(ctx, opw3);
	return(1);
}
#endif

#if 0
	i=BGBCC_SHX_LookupSimLabelIndex(ctx, lbl);
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
		rngb=244;
//		rngw=2040;
		rngw=4080;
//		rngw16=32760;
//		rngw16=30720;
		rngw16=65280;
		if(i>=ctx->nlbl)
		{
//			j=j+ctx->sim_voffs;
//			j=j+ctx->sim_voffs+szrng;
			j=j+szrng;
//			rngb=44;
			rngb=88;
//			rngb=122;
//			rngb=144;
//			rngb=166;
//			rngw=480;
//			rngw=960;
			rngw=1536;
//			rngw=2040;
//			rngw16=24576;
			rngw16=65280-(szrng/2);
			
			rngb=244-(szrng/2);
//			rngw=2040-(szrng/2);
			rngw=4080-(szrng/2);
//			if(rngb<0)rngb=0;
//			if(rngw<0)rngw=0;
			
//			if(!ctx->need_n12jmp)
//				rngb=256;
//			if(!ctx->need_f16jmp)
//				rngw=4096;
//			if(!ctx->need_farjmp)
//				rngw16=65536;
			
//			if(!ctx->is_stable || (ctx->is_stable&2))
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
			
//		k=ctx->sim_voffs; k=k^(k>>31);
//		rngb-=k;
//		rngw-=k;
		
//		if(k && ctx->is_stable)
//		if((k>8) && ctx->is_stable)
//			ctx->is_stable|=2;
		
//		if(ctx->is_simpass)
//			{ rngb=0; rngw=0; }

		k=BGBCC_SHX_EmitGetOffs(ctx);
//		j=k-j;
//		j=(k+4)-j;
		j=j-(k+4);
		if(j<0)j=-j;
		
//		if(j<104)
		if(j<rngb)
//		if(0)
		{
			return(BGBCC_SHX_EmitOpLabel(ctx, nmid, lbl));
		}

//		if(j<1984)
		if(j<rngw)
//		if(0)
		{
			return(BGBCC_SHX_EmitOpNear12Label(ctx, nmid, lbl));
//			return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
		}

		if(j<rngw16)
//		if(0)
		{
			return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
		}

		return(BGBCC_SHX_EmitOpFarLabel(ctx, nmid, lbl));
	}


	case BGBCC_SH_NMID_BFS:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8F00;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8900;
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);

		if(rlty>0)
			{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw3>=0)
			BGBCC_SHX_EmitWord(ctx, opw3);
		return(1);
	}
	return(0);
#endif


#if 1
int BGBCC_SHX_EmitStoreRegLabelVarRel24(
	BGBCC_SHX_Context *ctx, int op, int reg, int lbl)
{
	int rngw, rngw16, rngw20;
	int opw0, opw1, opw2, opw3;
	int ob;
	int i, j, k;

	if(!ctx->has_bjx1mov)
	{
//		BGBCC_SHX_EmitLoadRegLabelAbs(ctx, reg, lbl);
		BGBCC_DBGBREAK
		return(1);
	}

	if(ctx->sreg_live&1)
	{
		BGBCC_DBGBREAK
	}

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);
	ob=BGBCC_SHX_EmitGetOffs(ctx);

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }
	
	opw3=-1;

	if(op==BGBCC_SH_NMID_MOVQ)
	{
		opw0=0x8AFF;
		opw1=0xFFF8+(ob&2);
//		if(BGBCC_SHX_EmitCheckModeDQ(ctx))
		if(0)
			{ opw2=0x40D1|((reg&15)<<8); }
		else
		{
//			opw2=0xCC08; opw3=0x40E1|((reg&15)<<8);
			opw2=0x40A1|((reg&15)<<8);
		}
	}else if(op==BGBCC_SH_NMID_MOVL)
	{
		opw0=0x8AFF;
		opw1=0xFFF8+(ob&2);
		opw2=0x40E1|((reg&15)<<8);
	}else if(op==BGBCC_SH_NMID_FMOVS)
	{
		if(ctx->no_fpu)
			{ BGBCC_DBGBREAK }
		opw0=0x8AFF;
		opw1=0xFFF8+(ob&2);
		opw2=0x40B1|((reg&15)<<8);
	}else
	{
		opw0=0x8AFF;
		opw1=0xFFF8;
		if(op==BGBCC_SH_NMID_MOVB)
			opw2=0x40C1|((reg&15)<<8);
		if(op==BGBCC_SH_NMID_MOVW)
			opw2=0x40D1|((reg&15)<<8);
	}

	BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL24_BJX);
	BGBCC_SHX_EmitWord(ctx, opw0);
	BGBCC_SHX_EmitWord(ctx, opw1);
	BGBCC_SHX_EmitWord(ctx, opw2);
	if(opw3>0)
		BGBCC_SHX_EmitWord(ctx, opw3);
	return(1);
}
#endif


int BGBCC_SHX_EmitFlushIndexImm16(BGBCC_SHX_Context *ctx)
{
	int ob, opc, opw, opd, oba, obd2;
	int i, j, k;
	
	if(!ctx->const_ns16)
		return(0);
	
	BGBCC_SHX_EmitBAlign(ctx, 2);
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	for(i=0; i<ctx->const_ns16; i++)
	{
		j=ctx->const_s16tab_rlc[i];
		if(j)
			{ BGBCC_SHX_EmitRelocTy(ctx, j&0x00FFFFFF, (j>>24)&255); }
		BGBCC_SHX_EmitWord(ctx, ctx->const_s16tab[i]);
	}

	for(i=0; i<ctx->nofs_s16tab; i++)
	{
		j=ctx->idx_s16tab[i];
		k=ctx->const_s16tab_rlc[j];

		opc=ctx->ofs_s16tab[i];
//		opd=((ob-(opc+4))/2)+(ctx->idx_s16tab[i]);
//		opd=((ob-(opc+2))/2)+(ctx->idx_s16tab[i]);
		opd=((ob-(opc+2))/2)+j;
//		opd=((ob-opc)/2)+(ctx->idx_s16tab[i]);
		opw=BGBCC_SHX_EmitGetOffsWord(ctx, opc-2);

		if((opw&0xF000)!=0x9000)
			{ BGBCC_DBGBREAK }
		if(opd>>8)
			{ BGBCC_DBGBREAK }

		opw+=opd;
		BGBCC_SHX_EmitSetOffsWord(ctx, opc-2, opw);

		if(((k>>24)&255)==BGBCC_SH_RLC_REL16)
		{
			oba=ob+j*2;
			obd2=BGBCC_SHX_EmitGetOffsWord(ctx, oba);
			obd2+=oba-(opc-2);
			BGBCC_SHX_EmitSetOffsWord(ctx, oba, obd2);
		}
	}
	
	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	return(1);
}

int BGBCC_SHX_EmitFlushIndexImm32(BGBCC_SHX_Context *ctx)
{
	int ob, opc, opw, opd, oba, obd2;
	int i, j, k;
	
	if(!ctx->const_ns32)
		return(0);

	BGBCC_SHX_EmitBAlign(ctx, 4);
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	for(i=0; i<ctx->const_ns32; i++)
	{
		j=ctx->const_s32tab_rlc[i];
		if(j)
			{ BGBCC_SHX_EmitRelocTy(ctx, j&0x00FFFFFF, (j>>24)&255); }
		BGBCC_SHX_EmitDWord(ctx, ctx->const_s32tab[i]);
	}

	for(i=0; i<ctx->nofs_s32tab; i++)
	{
		j=ctx->idx_s32tab[i];
		k=ctx->const_s32tab_rlc[j];
		opc=ctx->ofs_s32tab[i];
//		opd=((ob-(opc+4))/4)+j;
//		opd=((ob-(opc+2))/4)+j;
		opd=((ob-opc)/4)+j;
		opw=BGBCC_SHX_EmitGetOffsWord(ctx, opc-2);

//		if((opw&0xF000)!=0xD000)
		if(((opw&0xF000)!=0xD000) && ((opw&0xFF00)!=0xC700))
			{ BGBCC_DBGBREAK }
		if(opd>>8)
			{ BGBCC_DBGBREAK }

		opw+=opd;
		BGBCC_SHX_EmitSetOffsWord(ctx, opc-2, opw);
		
		if(((k>>24)&255)==BGBCC_SH_RLC_REL32)
		{
			oba=ob+j*4;
			obd2=BGBCC_SHX_EmitGetOffsDWord(ctx, oba);
			obd2+=oba-(opc-2);
			BGBCC_SHX_EmitSetOffsDWord(ctx, oba, obd2);
		}
	}
	
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;
	return(1);
}

int BGBCC_SHX_EmitCheckFlushIndexImmP(BGBCC_SHX_Context *ctx)
{
	int ob, obe16, obe32;
	int oo;

	if(!ctx->const_ns16 && !ctx->const_ns32)
		return(0);
	
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	ob=(ob+3)&(~3);
	obe16=ob+(((ctx->const_ns16+1)&(~1))*2);
	obe32=obe16+((ctx->const_ns32)*4);
	
	if(ctx->nofs_s16tab)
	{
		oo=ctx->ofs_s16tab[0];
//		if((obe16-oo)>=480)
		if((obe16-oo)>=420)
			return(1);
	}

	if(ctx->nofs_s32tab)
	{
		oo=ctx->ofs_s32tab[0];
		if((obe32-oo)>=960)
			return(1);
	}
	
	return(0);
}

int BGBCC_SHX_EmitCheckFlushIndexImm(BGBCC_SHX_Context *ctx)
{
	int ob, eo, nsc;
	int l0;

	if(!BGBCC_SHX_EmitCheckFlushIndexImmP(ctx))
		return(0);
	
	l0=BGBCC_SHX_GenLabelTemp(ctx);

	if(ctx->has_bjx1jmp)
	{
		ob=BGBCC_SHX_EmitGetOffs(ctx);
		eo=(ctx->const_ns16^(ob>>1))&1;
		nsc=ctx->const_ns16+(ctx->const_ns32*2);
		
		if(eo && (nsc<126))
//		if(!eo && (nsc<126))
		{
			BGBCC_SHX_EmitRelocRelW8(ctx, l0);
			BGBCC_SHX_EmitWord(ctx, 0x8200);
		}else
		{
			BGBCC_SHX_EmitRelocRelW12(ctx, l0);
			BGBCC_SHX_EmitWord(ctx, 0xA000);
			BGBCC_SHX_EmitWord(ctx, 0x0009);
		}
		
	}else
	{
		BGBCC_SHX_EmitRelocRelW12(ctx, l0);
		BGBCC_SHX_EmitWord(ctx, 0xA000);
		BGBCC_SHX_EmitWord(ctx, 0x0009);
	}

	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);

	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;

	BGBCC_SHX_EmitLabel(ctx, l0);
	return(1);
}

int BGBCC_SHX_EmitForceFlushIndexImm(BGBCC_SHX_Context *ctx)
{
	int ob, eo, nsc;
	int l0;

//	if(!BGBCC_SHX_EmitCheckFlushIndexImmP(ctx))
//		return(0);
	
	if(!ctx->const_ns16 && !ctx->const_ns32)
		return(0);
	
	l0=BGBCC_SHX_GenLabelTemp(ctx);

	if(ctx->has_bjx1jmp)
	{
		ob=BGBCC_SHX_EmitGetOffs(ctx);
		eo=(ctx->const_ns16^(ob>>1))&1;
		nsc=ctx->const_ns16+(ctx->const_ns32*2);
		
		if(eo && (nsc<126))
//		if(!eo && (nsc<126))
		{
			BGBCC_SHX_EmitRelocRelW8(ctx, l0);
			BGBCC_SHX_EmitWord(ctx, 0x8200);
		}else
		{
			BGBCC_SHX_EmitRelocRelW12(ctx, l0);
			BGBCC_SHX_EmitWord(ctx, 0xA000);
			BGBCC_SHX_EmitWord(ctx, 0x0009);
		}
		
	}else
	{
		BGBCC_SHX_EmitRelocRelW12(ctx, l0);
		BGBCC_SHX_EmitWord(ctx, 0xA000);
		BGBCC_SHX_EmitWord(ctx, 0x0009);
	}

	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);

	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;

	BGBCC_SHX_EmitLabel(ctx, l0);
	return(1);
}

int BGBCC_SHX_EmitFlushIndexImmBasic(BGBCC_SHX_Context *ctx)
{
	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);
	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;
	return(1);
}
