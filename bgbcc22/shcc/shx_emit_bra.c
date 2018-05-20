#include <bgbccc.h>

int BGBCC_SHX_EmitOpLblReg(BGBCC_SHX_Context *ctx,
	int nmid, int lbl, int reg)
{
	if(!BGBCC_SHX_TryEmitOpLblReg(ctx, nmid, lbl, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLblReg(BGBCC_SHX_Context *ctx,
	int nmid, int lbl, int reg)
{
	int opw, opw2, rlty;

//	if(ctx->use_onlyimm)
//		{ BGBCC_DBGBREAK }

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	opw=-1; opw2=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		rlty=BGBCC_SH_RLC_RELW8L;
		opw=0xD000|((reg&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVA:
		if(reg!=BGBCC_SH_REG_R0)
			break;
		rlty=BGBCC_SH_RLC_RELW8L;
		opw=0xC700;
		break;
	}

	if(opw>=0)
	{
		if(rlty>0)
			{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}

	return(0);
}

int BGBCC_SHX_EmitOpLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, rlty;

	opw=-1; opw2=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000; break;
	case BGBCC_SH_NMID_BSR:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xB000; break;

#if 0
	case BGBCC_SH_NMID_BRAN:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x0009;
		break;
#endif

	case BGBCC_SH_NMID_BRAN:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW8;
			opw=0x8200;
		}else
		{
			rlty=BGBCC_SH_RLC_RELW12;
			opw=0xA000;
			opw2=0x0009;
		}
		break;

	case BGBCC_SH_NMID_BT:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8900; break;
	case BGBCC_SH_NMID_BF:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8B00; break;
	case BGBCC_SH_NMID_BTS:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8D00; break;
	case BGBCC_SH_NMID_BFS:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8F00; break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(rlty>0)
			{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpNear12Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpNear12Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpNear12Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, opw3, rlty;

	opw=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
		if(ctx->is_betav)
			break;
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000; break;
	case BGBCC_SH_NMID_BSR:
		if(ctx->is_betav)
			break;
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xB000; break;

	case BGBCC_SH_NMID_BT:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8900;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8B01;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BF:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8B00;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8901;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BTS:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8D00;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8D00;
		break;
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


	case BGBCC_SH_NMID_BRAN:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8200;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
//		opw2=0x8D00;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BSRN:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8300;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xB000;
//		opw2=0x8D00;
		opw3=0x0009;
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
}

int BGBCC_SHX_EmitOpFarLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpFarLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpFarLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, opw3, rlty;
	int tr0;

	opw=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAF:
		opw=0x0023;
//		opw2=0xA002;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRF:
		opw=0x0003;
		break;

	case BGBCC_SH_NMID_BT:
		opw=0x0023;
		opw2=0x8B02;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BF:
		opw=0x0023;
		opw2=0x8902;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BRAN:
//	case BGBCC_SH_NMID_BRAF:
		opw=0x0023;
//		opw2=0xA002;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BTS:
		opw=0x0023;
		opw2=0x8B01;
		break;
	case BGBCC_SH_NMID_BFS:
		opw=0x0023;
		opw2=0x8901;
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);

		if(ctx->sreg_live&1)
			tr0=BGBCC_SHXC_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
		else
			tr0=0;

//		tr0=BGBCC_SHXC_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
//		BGBCC_SHX_EmitLoadRegLabelRel(ctx, BGBCC_SH_REG_R0, lbl, 6);
		BGBCC_SHX_EmitLoadRegLabelRel(ctx, tr0, lbl, 6);
		opw|=(tr0<<8)&0x0F00;
		BGBCC_SHX_EmitWord(ctx, opw);

		if(ctx->sreg_live&1)
			BGBCC_SHXC_ScratchReleaseReg(NULL, ctx, tr0);

		if(opw3>=0)
			BGBCC_SHX_EmitWord(ctx, opw3);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpFar16Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpFar16Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpFar16Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, opw3, rlty;
	int tr0;

	if(ctx->has_bjx1jmp)
	{
		if((nmid==BGBCC_SH_NMID_BT) ||
			(nmid==BGBCC_SH_NMID_BF) ||
			(nmid==BGBCC_SH_NMID_BTS) ||
			(nmid==BGBCC_SH_NMID_BFS) ||
			(nmid==BGBCC_SH_NMID_BRAN) ||
			(nmid==BGBCC_SH_NMID_BSRN))
		{
			return(BGBCC_SHX_TryEmitOpNear12Label(ctx, nmid, lbl));
		}
		
		opw=-1; opw2=-1; opw3=-1; rlty=-1;
		if((nmid==BGBCC_SH_NMID_BRA) && !ctx->is_betav)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw=0x8E00;
			opw3=0xA000;
		}else if((nmid==BGBCC_SH_NMID_BSR) && !ctx->is_betav)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw=0x8E00;
			opw3=0xB000;
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
	}

	opw=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAF:
		opw=0x0023;
//		opw2=0xA002;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRF:
		opw=0x0003;
		break;

	case BGBCC_SH_NMID_BRAN:
		opw=0x0023;
//		opw2=0xA002;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BT:
		opw=0x0023;
		opw2=0x8B02;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BF:
		opw=0x0023;
		opw2=0x8902;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BTS:
		opw=0x0023;
		opw2=0x8B01;
		break;
	case BGBCC_SH_NMID_BFS:
		opw=0x0023;
		opw2=0x8901;
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);

		tr0=BGBCC_SHXC_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
//		BGBCC_SHX_EmitLoadRegLabelRel(ctx, BGBCC_SH_REG_R0, lbl, 6);
		BGBCC_SHX_EmitLoadRegLabelRel16(ctx, tr0, lbl, 6);
		opw|=(tr0<<8)&0x0F00;
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHXC_ScratchReleaseReg(NULL, ctx, tr0);

		if(opw3>=0)
			BGBCC_SHX_EmitWord(ctx, opw3);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitCheckAutoLabelNear8(
	BGBCC_SHX_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_SHX_LookupSimLabelIndex(ctx, lbl);
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
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
			
		k=BGBCC_SHX_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;
		
		if(j<rngb)
		{
			return(1);
		}
		
		return(0);
	}
	
	if(!ctx->need_n12jmp)
		return(1);
	return(0);
}


int BGBCC_SHX_EmitCheckAutoLabelNear16B(
	BGBCC_SHX_Context *ctx, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	if(!ctx->has_bjx1jmp)
		return(0);

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

//	i=BGBCC_SHX_LookupSimLabelIndex(ctx, lbl);
	i=BGBCC_SHX_LookupLabelIndex(ctx, lbl);
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
			
		k=BGBCC_SHX_EmitGetOffs(ctx);
		j=j-(k+4);
		if(j<0)j=-j;

		if(j<rngb)
			{ return(3); }
		if(j<rngw)
			{ return(2); }
		if(j<rngw16)
			{ return(1); }
		
		return(0);
	}
	
	return(0);
}


int BGBCC_SHX_EmitOpAutoLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_SHX_LookupSimLabelIndex(ctx, lbl);
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
//		rngb=244;
		rngb=252;
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
	
//	return(BGBCC_SHX_EmitOpFarLabel(ctx, nmid, lbl));

	if(ctx->need_farjmp)
		return(BGBCC_SHX_EmitOpFarLabel(ctx, nmid, lbl));
	else if(ctx->need_f16jmp)
		return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
	else if(ctx->need_n12jmp)
	{
//		return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
		return(BGBCC_SHX_EmitOpNear12Label(ctx, nmid, lbl));
	}
	else
	{
		i=BGBCC_SHX_EmitOpLabel(ctx, nmid, lbl);
//		if((nmid==BGBCC_SH_NMID_BRA) ||
//			(nmid==BGBCC_SH_NMID_BSR))
//				BGBCC_SHX_EmitWord(ctx, 0x0009);
		return(i);
	}
}


int BGBCC_SHX_TryEmitOpCmpRegLabel(BGBCC_SHX_Context *ctx,
	int nmid, int reg, int lbl)
{
	int opw1, opw2;
	int nm1, nm2, rlty;

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))
		{ BGBCC_DBGBREAK }

#if 0
	if(ctx->has_bjx1breq && ((reg&0xF0)==0) &&
		BGBCC_SHX_EmitCheckAutoLabelNear8(ctx, lbl)>0)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_BREQ:
			opw1=0x8EFF; opw2=0x4005|((reg&15)<<8); break;
		case BGBCC_SH_NMID_BRNE:
			opw1=0x8EFF; opw2=0x4015|((reg&15)<<8); break;
#if 1
		case BGBCC_SH_NMID_BRGT:
			opw1=0x8EFF; opw2=0x4025|((reg&15)<<8); break;
		case BGBCC_SH_NMID_BRLE:
			opw1=0x8EFF; opw2=0x4035|((reg&15)<<8); break;
		case BGBCC_SH_NMID_BRGE:
			opw1=0x8EFF; opw2=0x4045|((reg&15)<<8); break;
		case BGBCC_SH_NMID_BRLT:
			opw1=0x8EFF; opw2=0x4055|((reg&15)<<8); break;
#endif
		default:
			opw1=-1; opw2=-1; break;
		}

		rlty=BGBCC_SH_RLC_RELW8;

		if(opw1>=0)
		{
			BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty);
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
	}
#endif

	nm1=-1;
	nm2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BREQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case BGBCC_SH_NMID_BRNE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case BGBCC_SH_NMID_BRGT:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case BGBCC_SH_NMID_BRLE:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case BGBCC_SH_NMID_BRGE:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case BGBCC_SH_NMID_BRLT:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	}
	
	if((nm1>0) && (nm2>0))
	{
		BGBCC_SHX_EmitOpReg(ctx, nm1, reg);
		BGBCC_SHX_EmitOpAutoLabel(ctx, nm2, lbl);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_TryEmitOpCmpRegRegLabel(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int lbl)
{
	int opw1, opw2, rlty, flip;
	int nm1, nm2;

	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))
		{ BGBCC_DBGBREAK }
	if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))
		{ BGBCC_DBGBREAK }

#if 0
	if(ctx->has_bjx1breq &&
		((rm&0xF8)==8) && ((rn&0xF8)==8) &&
		(BGBCC_SHX_EmitCheckAutoLabelNear8(ctx, lbl)>0))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_BREQ:
			opw1=0x8EFF; opw2=0x4006|((rn&7)<<8)|((rm&7)<<4); break;
		case BGBCC_SH_NMID_BRNE:
			opw1=0x8EFF; opw2=0x4007|((rn&7)<<8)|((rm&7)<<4); break;

#if 1
		case BGBCC_SH_NMID_BRGT:
			opw1=0x8EFF; opw2=0x4086|((rn&7)<<8)|((rm&7)<<4); break;
		case BGBCC_SH_NMID_BRLE:
			opw1=0x8EFF; opw2=0x4087|((rn&7)<<8)|((rm&7)<<4); break;
		case BGBCC_SH_NMID_BRGE:
			opw1=0x8EFF; opw2=0x4806|((rn&7)<<8)|((rm&7)<<4); break;
		case BGBCC_SH_NMID_BRLT:
			opw1=0x8EFF; opw2=0x4807|((rn&7)<<8)|((rm&7)<<4); break;
#endif
		default:
			opw1=-1; opw2=-1; break;
		}

		rlty=BGBCC_SH_RLC_RELW8;

		if(opw1>=0)
		{
			BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty);
			BGBCC_SHX_EmitWord(ctx, opw1);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
	}
#endif

	flip=0;
	nm1=-1;
	nm2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BREQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case BGBCC_SH_NMID_BRNE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case BGBCC_SH_NMID_BRGT:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case BGBCC_SH_NMID_BRLE:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case BGBCC_SH_NMID_BRGE:
//		nm1=BGBCC_SH_NMID_CMPGE;
//		nm2=BGBCC_SH_NMID_BT;

		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BF;
		flip=1;
		break;
	case BGBCC_SH_NMID_BRLT:
//		nm1=BGBCC_SH_NMID_CMPGE;
//		nm2=BGBCC_SH_NMID_BF;

		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BT;
		flip=1;
		break;
	}
	
	if((nm1>0) && (nm2>0))
	{
		if(flip)
			BGBCC_SHX_EmitOpRegReg(ctx, nm1, rn, rm);
		else
			BGBCC_SHX_EmitOpRegReg(ctx, nm1, rm, rn);
		BGBCC_SHX_EmitOpAutoLabel(ctx, nm2, lbl);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpCmpRegLabel(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpCmpRegLabel(ctx, nmid, rm, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_EmitOpCmpRegRegLabel(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpCmpRegRegLabel(ctx, nmid, rm, rn, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

