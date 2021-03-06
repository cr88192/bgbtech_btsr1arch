int BGBCC_JX2_CheckOps32GetRegs(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2,
	byte *rrs, byte *rrt, byte *rrn,
	byte *rspr, byte *rspw, int *rspfl)
{
	byte rs, rt, rn, spr, spw, spfl;
	
	if((opw1&0xE000)!=0xE000)
		return(-1);
	
	rn=((opw1>>4)&15)|((opw2>>6)&16);
	rs=((opw1   )&15)|((opw2>>5)&16);
	rt=((opw2>>4)&15)|((opw2>>4)&16);
	
	spr=BGBCC_SH_REG_ZZR;
	spw=BGBCC_SH_REG_ZZR;
	spfl=0;

//	return(-1);
	
	if((opw1&0xEB00)==0xE000)
	{
//		return(-1);

		if((opw2&0xF000)>=0xC000)
//			return(0);
			return(-1);

		if((opw2&0xF008)==0x1008)
		{
			rt=rn;

//			return(-1);

			if((opw2&0xF00F)==0x1009)
			{
				switch((opw2>>4)&15)
				{
				case 0x4:	case 0xC:
				case 0xD:	case 0xE:
					spr=BGBCC_SH_REG_SR;
					spw=BGBCC_SH_REG_SR;
					break;
					
				case 0x9:	case 0xF:
					return(0);
				}
			}

			if((opw2&0xF00F)==0x100A)
			{
				spr=BGBCC_SH_REG_SR;
				spw=BGBCC_SH_REG_SR;
			}

			if((opw2&0xF00F)==0x100C)
			{
				switch((opw2>>4)&15)
				{
				case 0x2:
				case 0xE:	case 0xF:
					spr=BGBCC_SH_REG_SR;
					spw=BGBCC_SH_REG_SR;
					break;

				case 0x4:
					return(0);
				}
			}


			*rrs=rn;
			*rrt=rs;
			*rrn=rn;
			*rspr=spr;
			*rspw=spw;
			*rspfl=spfl;
			return(1);
		}

		if((opw2&0xF00F)==0x3000)
		{
//			return(0);
		
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;
			
			rs=rt;
			rn=rt;
			
			switch(opw1&0xFF)
			{
			case 0x18:	case 0x19:
			case 0x1A:	case 0x1B:
			case 0x82:	case 0x83:
				spr=15;
				spw=15;
				break;

			default:
				break;
			}

			*rrs=rt;
			*rrt=rt;
			*rrn=rt;
			*rspr=spr;
			*rspw=spw;
			*rspfl=spfl;
			return(1);
		}

		if((opw2&0xF00F)==0x5000)
		{
//			return(-1);
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;
		}

		if((opw2&0xF00F)==0x900C)
		{
//			return(-1);
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;
		}

		if((opw2&0xF000)==0x0000)
		{
			if((opw2&0xF808)==0x0000)
				return(-1);
		
			spfl|=1;

//			return(-1);

			if(rs==0)
			{
				rs=BGBCC_SH_REG_PC;
				if(rt==1)
					{ rs=0; rt=BGBCC_SH_REG_ZZR; }
				return(-1);
			}
			if(rs==1)
			{
				rs=BGBCC_SH_REG_GBR;
				if(rt==1)
				{
//					rs=BGBCC_SH_REG_TBR; rt=0;
					rt=0;
				}
//				return(-1);
			}

		}

		*rrs=rs;
		*rrt=rt;
		*rrn=rn;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

	if((opw1&0xEB00)==0xE100)
	{
		if((opw2&0xF000)>=0xC000)
		{
			return(-1);
		}

		spfl|=1;

//		return(-1);

		if(rs==0)
			{ rs=BGBCC_SH_REG_PC; }
		if(rs==1)
			{ rs=BGBCC_SH_REG_GBR; }

		*rrs=rs;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

	if((opw1&0xEB00)==0xE200)
	{
//		return(-1);

		if((opw2&0xF000)==0xC000)
		{
			spr=BGBCC_SH_REG_SR;
			spw=BGBCC_SH_REG_SR;

//			return(-1);
		}

		*rrs=rs;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

	if((opw1&0xEE00)==0xE800)
	{
		rn=opw1&0x001F;
		rs=rn;
		rt=BGBCC_SH_REG_ZZR;

		if((opw1&0x00F0)<0x0040)
		{
			rs=BGBCC_SH_REG_ZZR;
		}

		*rrs=rs;
		*rrt=rt;
		*rrn=rn;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

//	return(-1);

	if((opw1&0xEE00)==0xEA00)
	{
		rn=0;
		*rrs=rn;
		*rrt=BGBCC_SH_REG_ZZR;
		*rrn=rn;
		*rspr=spr;
		*rspw=spw;
		*rspfl=spfl;
		return(1);
	}

	return(0);
}

int BGBCC_JX2_CheckOps32ReadsRn(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2)
{
	if((opw1&0xFF00)==0xF000)
	{
		if((opw2&0xF808)==0x0000)
			return(1);
		return(0);
	}

	if((opw1&0xFF00)==0xF100)
	{
		if((opw2&0x8800)==0x0000)
			return(1);
		return(0);
	}

	return(0);
}

int BGBCC_JX2_CheckOps32SequenceOnlyB(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int fl)
{
	byte rs1, rt1, rn1, rspr1, rspw1;
	byte rs2, rt2, rn2, rspr2, rspw2;
	int rspfl1, rspfl2;
	int ret1, ret2;

	if((opw1&0xFF00)==0xF400)
	{
		if((opw1&0xC000)==0xC000)
		{
			return(1);
		}
	}

	ret1=BGBCC_JX2_CheckOps32GetRegs(sctx, opw1, opw2,
		&rs1, &rt1, &rn1, &rspr1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOps32GetRegs(sctx, opw3, opw4,
		&rs2, &rt2, &rn2, &rspr2, &rspw2, &rspfl2);

	if(ret1<0)
		return(ret1);
	if(ret2<0)
		return(ret2);
	
	if(!ret1)
		return(1);
	if(!ret2)
		return(1);
		
	if((rspfl1&1) && (rspfl2&1))
		return(1);
	
	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1) || (rs2==rn1) || (rt2==rn1))
			return(1);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if((rspr2==rspw1) ||	(rn2==rspw1) ||
			(rs2==rspw1) || 	(rt2==rspw1))
				return(1);
	}

//	if(fl&1)
	if((fl&1) || BGBCC_JX2_CheckOps32ReadsRn(sctx, opw3, opw4))
	{
//		if((opw1&0xFE00)==0xF800)
//			return(1);
//		if((opw3&0xFE00)==0xF800)
//			return(1);
	
//		return(1);

		if(rn2!=BGBCC_SH_REG_ZZR)
		{
			if((rn1==rn2) || (rs1==rn2) || (rt1==rn2))
				return(1);
		}

		if(rspw2!=BGBCC_SH_REG_ZZR)
		{
			if((rspr1==rspw2) ||	(rn1==rspw2) ||
				(rs1==rspw2) ||		(rt1==rspw2))
					return(1);
		}
	}

	return(0);
}

int BGBCC_JX2_CheckOps32SequenceOnly(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	return(BGBCC_JX2_CheckOps32SequenceOnlyB(sctx,
		opw1, opw2, opw3, opw4, 0));
}

/* Return true if this operation is immovable and must remain in place. */
int BGBCC_JX2_CheckOps32Immovable(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2)
{
	if((opw1&0xE000)!=0xE000)
		return(1);

	if((opw1&0xFE00)==0xFA00)
		return(1);
	if((opw1&0xFC00)==0xF400)
		return(1);
	if((opw1&0xFF00)==0xF900)
		return(1);

	if((opw1&0xEB00)==0xE000)
	{
		if((opw2&0xF000)>=0xC000)
			return(1);

		if((opw2&0xF000)==0x0000)
		{
			if((opw1&0x000F)==0x0000)
				return(1);
		}

		if((opw2&0xF000)==0x4000)
		{
			if((opw1&0x000F)==0x0000)
				return(1);
		}
		return(0);
	}

	if((opw1&0xEB00)==0xE100)
	{
		if((opw2&0xF000)>=0xC000)
			return(1);
		if((opw1&0x000F)==0x0000)
			return(1);
		return(0);
	}

	return(0);
}


int BGBCC_JX2_CheckCanSwapOps32(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4)
{
	int ret;
	
	ret=BGBCC_JX2_CheckOps32SequenceOnlyB(sctx,
		opw1, opw2, opw3, opw4, 1);
		
	if(ret<0)
		return(0);
	return(!ret);
}

int BGBCC_JX2_CheckOps32ValidWexSuffix(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	return(BGBCC_JX2_CheckOps32ValidWexSuffixFl(sctx, opw1, opw2, 0));
}

int BGBCC_JX2_CheckOps32ValidWexSuffix3W(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	if(sctx->use_wexmd<2)
		return(0);

	return(BGBCC_JX2_CheckOps32ValidWexSuffixFl(sctx, opw1, opw2, 1));
}

int BGBCC_JX2_CheckOps32ValidWexSuffixFl(
	BGBCC_JX2_Context *sctx, int opw1, int opw2, int fl)
{
	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)!=0)
		return(0);

	if((opw1&0xF000)!=0xF000)
		return(0);

	if((opw1&0xFF00)==0xF000)
	{
		if((opw2&0xF000)>=0xC000)
			return(0);

//		if((sctx->use_wexmd==1) || (fl&1))
		if(1)
		{
			if((opw2&0xF808)==0xF000)
//			if((opw2&0xF808)==0x0000)
				return(0);
		}

		if((opw2&0xF000)==0x2000)
		{
			if((opw2&0x000C)==0x0004)
			{
				if(opw2&0x0800)
				{
					/* 128-bit SIMD ops. */
					return(0);
				}
			}
		}

		if((opw2&0xF00F)==0x3000)
		{
			if((opw1&0x00F0)==0x0000)
			{
				return(0);
			}
			return(0);
		}

		if((opw2&0xF003)==0x4000)
		{
			/* MOV.X */
			return(0);
		}

		return(1);
	}

	if((opw1&0xFF00)==0xF100)
	{
//		if((sctx->use_wexmd==1) || (fl&1))
		if(1)
		{
			if((opw2&0x8800)==0x0000)
				return(0);
		}
		return(1);
	}

	if((opw1&0xFF00)==0xF200)
	{
		return(1);
	}

	if((opw1&0xFF00)==0xF800)
		return(1);

	return(0);
}

int BGBCC_JX2_CheckOps32ValidWexPrefix(
	BGBCC_JX2_Context *sctx, int opw1, int opw2)
{
	int ret;

	if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)!=0)
		return(0);

	if((opw1&0xF000)!=0xF000)
		return(0);

	if((opw1&0xFF00)==0xF000)
	{
//		return(0);

		ret=0;
		switch((opw2>>12)&15)
		{
		case 0x0:
			ret=0;
			break;
		case 0x1:
			switch(opw2&15)
			{
			case 0x0:	case 0x1:
			case 0x5:	case 0x6:
			case 0x7:
				ret=1;
				break;

			case 0x9:
				switch((opw2>>4)&15)
				{
				case 0x0:	case 0x1:
				case 0x5:	case 0x6:
				case 0x7:	case 0x8:
					ret=1;
					break;
				default:
					break;
				}
				break;
			
			case 0xC:
				switch((opw2>>4)&15)
				{
				case 0x0:	case 0x1:
				case 0x5:	case 0x6:
				case 0x7:	case 0x8:
				case 0x9:
					ret=1;
					break;
				default:
					break;
				}
				break;
			
			default:
				break;
			}
			break;
		case 0x2:
			switch(opw2&15)
			{
			case 0x0:	case 0x1:
			case 0x8:	case 0x9:
				ret=1;
				break;
			default:
				break;
			}
			break;
		case 0x3:
			break;
		case 0x4:
			switch(opw2&15)
			{
			case 0x0:	case 0x4:
			case 0x8:	case 0xC:
				ret=0;
				break;
			default:
				break;
			}
			break;
		case 0x5:
			switch(opw2&15)
			{
			case 0x0:
			case 0x1:
			case 0x4:	case 0x5:
			case 0x6:	case 0x7:
			case 0xC:	case 0xD:
			case 0xE:	case 0xF:
				ret=1;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		return(ret);
	}

	if((opw1&0xFF00)==0xF100)
	{
		return(0);
	}

	if((opw1&0xFF00)==0xF200)
	{
//		return(0);

		ret=0;
		switch((opw2>>12)&15)
		{
		case 0x0:	case 0x1:
		case 0x3:
		case 0x4:	case 0x5:
		case 0x6:	case 0x7:
		case 0x8:	case 0x9:
			ret=1;
			break;
			
		case 0x2:
			ret=0;
			break;

		case 0xC:
			ret=0;
			switch(opw1&15)
			{
			case 0x0:
			case 0x1:
			case 0x2:
				ret=1;
				break;

			default:
				break;
			}
			break;

		}
		return(ret);
	}

	if((opw1&0xFF00)==0xF800)
	{
//		if((opw1&0xE)==0x0)
//			return(0);
//		if((opw1&0xF)==0xF)
//			return(0);
	
		ret=0;
		switch((opw1>>4)&15)
		{
		case 0:		case 1:		ret=1;	break;
		case 2:		case 3:		ret=1;	break;
		case 4:		case 5:		ret=1;	break;
		case 6:		case 7:		ret=1;	break;
		default:
			break;
		}
		return(ret);

//		return(0);

		return(1);
	}
	return(0);
}

ccxl_status BGBCC_JX2_AdjustWexifyOp(
	BGBCC_JX2_Context *sctx,
	int *ropw1, int *ropw2)
{
	int opw1, opw2;

	opw1=*ropw1;
	opw2=*ropw2;
	
	if((opw1&0xFF00)==0xF800)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0100;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		return(1);
	}

	if((opw1&0xFC00)==0xF000)
	{
		sctx->opcnt_hi8[(opw1>>8)&0xFF]--;
		opw1|=0x0400;
		sctx->opcnt_hi8[(opw1>>8)&0xFF]++;
		*ropw1=opw1;
		return(1);
	}

	return(0);
}

ccxl_status BGBCC_JX2_CheckWexify(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	int opw1, opw2, opw3, opw4, opw5, opw6;
	int dp, cp;
	
	dp=epos-spos;
	if(dp&3)
	{
		BGBCC_DBGBREAK
		return(0);
	}

//	return(0);
	
	cp=spos;
	while((cp+7)<epos)
	{
		opw1=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 0);
		opw2=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 2);
		opw3=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 4);
		opw4=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 6);
		opw5=BGBCC_JX2_EmitGetOffsWord(sctx, cp+ 8);
		opw6=BGBCC_JX2_EmitGetOffsWord(sctx, cp+10);
		
		if(!((cp+11)<epos))
		{
			opw5=0;
			opw6=0;
		}

		if((opw1&0xE000)!=0xE000)
			{ BGBCC_DBGBREAK }
		if((opw3&0xE000)!=0xE000)
			{ BGBCC_DBGBREAK }

		/* Skip predicated ops. */
		if((opw1&0xF000)==0xE000)
			{ cp+=4; continue; }

		/* Skip DLR loads. */
		if((opw1&0xFE00)==0xFA00)
			{ cp+=4; continue; }

		/* Skip Jumbo Forms */
		if(((opw1&0xFF00)==0xF400) &&
			((opw2&0xC000)==0xC000))
		{
			if(((opw3&0xFF00)==0xF400) &&
				((opw4&0xC000)==0xC000))
			{
				cp+=12;
				continue;
			}

			cp+=8;
			continue;
		}

		if(((opw1&0xFC00)==0xF400) ||
			((opw1&0xFF00)==0xF900))
		{
			/* Wexify Ignores existing WEX sequences. */
			if((opw3&0xFC00)==0xF000)
				{ cp+=8; continue; }
			if((opw3&0xFC00)==0xF400)
				{ cp+=4; continue; }
			if((opw3&0xFF00)==0xF900)
				{ cp+=4; continue; }
			cp+=8; continue;
		}
		
		/* If ops are immovable, skip. */
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw1, opw2)>0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_CheckOps32Immovable(sctx, opw3, opw4)>0)
			{ cp+=4; continue; }
		
		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2))
		{
#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw1, opw2) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx,
					opw1, opw2, opw3, opw4))
			{
				if(sctx->is_simpass)
					{ cp+=4; continue; }
			
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw4);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+4, opw1);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+6, opw2);
				continue;
			}
#endif

			cp+=4; continue;
		}

		if(!BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
			!sctx->is_simpass && ((cp+11)<epos))
		{
#if 1
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw5, opw6) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
//				!BGBCC_JX2_CheckOps32SequenceOnly(ctx, sctx,
				BGBCC_JX2_CheckCanSwapOps32(sctx,
					opw3, opw4, opw5, opw6) &&
//				(	!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//						opw1, opw2, opw5, opw6) ||
//					BGBCC_JX2_CheckOps32SequenceOnly(sctx,
//						opw1, opw2, opw3, opw4)))
				(	!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
						opw1, opw2, opw5, opw6) &&
					BGBCC_JX2_CheckOps32SequenceOnly(sctx,
						opw1, opw2, opw3, opw4)))
			{
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 4, opw5);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 6, opw6);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsWord(sctx, cp+10, opw4);
				continue;
			}
#endif
		}

		/* If next op begins WEX sequence, skip. */
		if(((opw3&0xFC00)==0xF400) ||
			((opw3&0xFF00)==0xF900))
				{ cp+=4; continue; }

		/* If next op is a DLR load, skip. */
		if((opw3&0xFE00)==0xFA00)
			{ cp+=4; continue; }

		/* Skip Mem Load/Store (Lane 1 only) */
		if((opw1&0xFF00)==0xF100)
			{ cp+=4; continue; }

#if 1
		if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
			BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw3, opw4) &&
			BGBCC_JX2_CheckOps32ValidWexSuffix3W(sctx, opw5, opw6) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw3, opw4) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw3, opw4, opw5, opw6) &&
			!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
				opw1, opw2, opw5, opw6))
		{
			BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);
			BGBCC_JX2_AdjustWexifyOp(sctx, &opw3, &opw4);

			BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+2, opw2);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+4, opw3);
			BGBCC_JX2_EmitSetOffsWord(sctx, cp+6, opw4);
			cp+=12;
			continue;
		}
#endif

		if((opw1&0xFF00)==0xF800)
		{
			if(BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
				!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
					opw1, opw2, opw3, opw4))
			{
//				opw1|=0x0100;
//				sctx->opcnt_hi8[0xF8]--;
//				sctx->opcnt_hi8[0xF9]++;

				BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);

				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
				cp+=8;
				continue;
			}

			cp+=4;
			continue;
		}

		if(((opw1&0xFF00)==0xF000) ||
			((opw1&0xFF00)==0xF200))
		{
			if(	BGBCC_JX2_CheckOps32ValidWexPrefix(sctx, opw1, opw2) &&
				BGBCC_JX2_CheckOps32ValidWexSuffix(sctx, opw3, opw4) &&
				!BGBCC_JX2_CheckOps32SequenceOnly(sctx,
					opw1, opw2, opw3, opw4))
			{
//				sctx->opcnt_hi8[(opw1>>8)&255]--;
//				opw1|=0x0400;
//				sctx->opcnt_hi8[(opw1>>8)&255]++;

				BGBCC_JX2_AdjustWexifyOp(sctx, &opw1, &opw2);

				BGBCC_JX2_EmitSetOffsWord(sctx, cp+0, opw1);
				cp+=8;
				continue;
			}

			cp+=4;
			continue;
		}

		/* Dunno, Leave it as-is. */
		cp+=4;
	}
	
	return(1);
}

ccxl_status BGBCC_JX2_BeginWex(
	BGBCC_JX2_Context *sctx)
{
	if(!sctx->use_wexmd)
		return(0);

	if(!(sctx->is_fixed32&16))
	{
//		if(!BGBCC_JX2_CheckPadAlign32(sctx))
		if(sctx->op_wex_align && !BGBCC_JX2_CheckPadAlign32(sctx))
		{
			BGBCC_JX2_EmitWord(sctx, 0x3000);
		}

		sctx->is_fixed32|=16;
		sctx->wex_ofs_begin=BGBCC_JX2_EmitGetOffs(sctx);
		return(1);
	}
	return(0);
}

ccxl_status BGBCC_JX2_EndWex(
	BGBCC_JX2_Context *sctx)
{
	int i0, i1;

	if(sctx->is_fixed32&16)
	{
		i0=sctx->wex_ofs_begin;
		i1=BGBCC_JX2_EmitGetOffs(sctx);
		BGBCC_JX2_CheckWexify(sctx, i0, i1);
		sctx->is_fixed32&=(~16);
		return(1);
	}
	return(0);
}
