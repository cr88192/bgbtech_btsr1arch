int BGBCC_JX2_CheckOpsX3_GetRegsImm(
	BGBCC_JX2_Context *sctx,
	u32 opw,
	u16 *rrs, u16 *rrt, u16 *rrn, u16 *rrp,
	u16 *rspr, u16 *rspw, int *rspfl, s64 *rimm)
{
	static u16 gr2spr[4]={
		BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR,
		BGBCC_SH_REG_SP,	BGBCC_SH_REG_GBR };
	static byte ldsc_sc[16]={1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8};
	s64 imm;
	int rm_dfl, rn_dfl, ro_dfl, ru_dfl, rn_df0, ro_df0, sc;
	int rn, rs, rt, rp, fl, rma, rna, ret;
	
	rm_dfl=(opw>>16)&63;
	ro_dfl=(opw>>22)&63;
	rn_dfl=(opw>> 6)&63;
	
	rn_df0=rn_dfl;
	ro_df0=ro_dfl;
	
	if(rm_dfl<4)	rm_dfl=gr2spr[rm_dfl];
	if(rn_dfl<4)	rn_dfl=gr2spr[rn_dfl];
	if(ro_dfl<4)	ro_dfl=gr2spr[ro_dfl];
	
	rn=BGBCC_SH_REG_ZZR;
	rs=BGBCC_SH_REG_ZZR;
	rt=BGBCC_SH_REG_ZZR;
	rp=BGBCC_SH_REG_ZZR;
	rma=BGBCC_SH_REG_ZZR;
	rna=BGBCC_SH_REG_ZZR;
	imm=0;
	fl=0;
	ret=1;

	if(!(opw&2))
	{
		rma=BGBCC_SH_REG_SR;
	}

	switch((opw>>2)&7)
	{
	case 0:
		rn=rn_dfl;
		rs=rm_dfl;
		rt=ro_dfl;
		switch((opw>>12)&15)
		{

#if 1
		case 0x0:
			if(!((opw>>31)&1))
			{
				rp=rn_dfl;
				fl|=BGBCC_WXSPFL_ISMEM;
				fl|=BGBCC_WXSPFL_IS_STORE;
				break;
			}
			fl|=BGBCC_WXSPFL_ISMEM;
			break;		
#endif
	
		case 0x1:
			if((opw&0x20) && ((opw>>29)!=1))
			{
				fl|=BGBCC_WXSPFL_RXN;
				fl|=BGBCC_WXSPFL_RXS;
				fl|=BGBCC_WXSPFL_RXT;
			}else
			{
				if(	((opw>>28)==0x0) ||
					((opw>>28)==0x1) ||
					((opw>>28)==0x5) ||
					((opw>>28)==0x6) ||
					((opw>>28)==0x7) )
				{
					fl|=BGBCC_WXSPFL_LANE2;
					fl|=BGBCC_WXSPFL_LANE3;

					if(((opw>>28)==0x0) && (ro_df0==0))
						fl|=BGBCC_WXSPFL_1STAGE;
				}

				if(	((opw>>28)==0x2) ||
					((opw>>28)==0x3) )
				{
					fl|=BGBCC_WXSPFL_3STAGE;
				}
			}
			break;

#if 1
		case 0x2:
			if(opw&0x20)
			{
				if(	((opw>>28)==0x5) ||
					((opw>>28)==0x6) ||
					((opw>>28)==0x7) ||
					((opw>>28)==0xD) ||
					((opw>>28)==0xE) ||
					((opw>>28)==0xF) )
				{
					fl|=BGBCC_WXSPFL_RXN;
					fl|=BGBCC_WXSPFL_RXS;
					fl|=BGBCC_WXSPFL_RXT;
				}
			}else
			{
				if(	((opw>>28)==0x0) ||
					((opw>>28)==0x1) ||
					((opw>>28)==0x2) ||
					((opw>>28)==0x3) ||
					((opw>>28)==0x8) ||
					((opw>>28)==0x9) )
				{
					fl|=BGBCC_WXSPFL_LANE2;
				}
			}
			break;
		case 0x3:
			if((opw&0x20) &&
				(((opw>>28)==0x2) ||
				 ((opw>>28)==0x3) ||
				 ((opw>>28)==0x4) ||
				 ((opw>>28)==0x5) ||
				 ((opw>>28)==0x6) ))
			{
				fl|=BGBCC_WXSPFL_RXN;
				fl|=BGBCC_WXSPFL_RXS;
				fl|=BGBCC_WXSPFL_RXT;
			}
			break;
#endif

#if 1
		case 0x4:
			if(!((opw>>31)&1))
			{
				rp=rn_dfl;
				fl|=BGBCC_WXSPFL_ISMEM;
				fl|=BGBCC_WXSPFL_IS_STORE;
				break;
			}
			fl|=BGBCC_WXSPFL_ISMEM;
			break;			
		case 0x5:
			if(opw&0x20)
			{
				if( ((opw>>28)==0x8) ||
					((opw>>28)==0x9) ||
					((opw>>28)==0xA) ||
					((opw>>28)==0xB) )
				{
					fl|=BGBCC_WXSPFL_RXN;
					fl|=BGBCC_WXSPFL_RXS;
					fl|=BGBCC_WXSPFL_RXT;
				}
			}else
			{
				if(	((opw>>28)==0x4) ||
					((opw>>28)==0x5) ||
					((opw>>28)==0xC) ||
					((opw>>28)==0xD) ||
					((opw>>28)==0xE) ||
					((opw>>28)==0xF) )
				{
					fl|=BGBCC_WXSPFL_LANE2;
				}

				if(	((opw>>28)==0x1) ||
					((opw>>28)==0x2) ||
					((opw>>28)==0x3) ||
					((opw>>28)==0x8) ||
					((opw>>28)==0x9) ||
					((opw>>28)==0xA) ||
					((opw>>28)==0xB) )
				{
					fl|=BGBCC_WXSPFL_3STAGE;
				}
			}
			break;
#endif

#if 1
		case 0x6:
			if((opw&0x20) &&
				(((opw>>28)==0xA) ||
				 ((opw>>28)==0xB) ))
			{
				fl|=BGBCC_WXSPFL_RXN;
				fl|=BGBCC_WXSPFL_RXS;
				fl|=BGBCC_WXSPFL_RXT;
			}
			break;
#endif

#if 1
		case 0x7:
			break;
#endif

#if 1
		case 0x8:
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
#endif

#if 1
		case 0x9:
			if(opw&0x20)
			{
				if(rn_df0==0)
				{
					rma=BGBCC_SH_REG_SR;
					rna=BGBCC_SH_REG_SR;
				}
			}else
			{
				rp=rn_dfl;
				fl|=BGBCC_WXSPFL_FIXNOMOVE;
			}
			break;
#endif

		case 0xC:
		case 0xD:
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		default:
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			ret=0;
			break;
		}
		break;
	case 1:
		if(((opw>>12)&15)>=12)
		{
			rs=rm_dfl;
			rt=rn_dfl;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		}

		if(((opw>>12)&15)<4)
		{
			sc=ldsc_sc[(opw>>12)&15];
			rs=rm_dfl;
			rp=rn_dfl;
			imm=(((s32)opw)>>22)*sc;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_IS_STORE;
			fl|=BGBCC_WXSPFL_HASDISP;
			break;
		}

		if(((opw>>12)&15)>=8)
		{
			sc=ldsc_sc[(opw>>12)&15];
			rs=rm_dfl;
			rn=rn_dfl;
			imm=(((s32)opw)>>22)*sc;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_HASDISP;
			break;
		}

//		ret=0;
//		break;
#if 1
		sc=(opw&0x20)?2:4;
		if(	(((opw>>12)&15)==5) ||
			(((opw>>12)&15)==7))
				sc=8;

		if(((opw>>12)&15)<6)
		{
			rs=rm_dfl;
			rp=rn_dfl;
			imm=(((s32)opw)>>22)*sc;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_IS_STORE;
			break;
		}else
		{
			rs=rm_dfl;
			rn=rn_dfl;
			imm=(((s32)opw)>>22)*sc;
			fl|=BGBCC_WXSPFL_ISMEM;
			break;
		}
		if(sc==8)
			{ fl|=BGBCC_WXSPFL_RXN; }
		break;
#endif

#if 1
	case 2:
		rs=rm_dfl;
		rn=rn_dfl;
		imm=((s32)opw)>>22;

		if(	(((opw>>12)&15)==0) ||
			(((opw>>12)&15)==1))
		{
			fl|=BGBCC_WXSPFL_LANE2;
			fl|=BGBCC_WXSPFL_LANE3;
		}

		if(	(((opw>>12)&15)==3) ||
			(((opw>>12)&15)==4))
		{
			fl|=BGBCC_WXSPFL_LANE2;
		}

		if(!(opw&0x20))
		{
			if(	(((opw>>12)&15)==5) ||
				(((opw>>12)&15)==6) ||
				(((opw>>12)&15)==7) )
			{
				fl|=BGBCC_WXSPFL_LANE2;
				fl|=BGBCC_WXSPFL_LANE3;
			}

			if(	(((opw>>12)&15)==8) ||
				(((opw>>12)&15)==9) )
			{
				fl|=BGBCC_WXSPFL_LANE2;
			}
		}

		if(	(((opw>>12)&15)==6) ||
			(((opw>>12)&15)==7))
		{
			if(opw&0x20)
			{
				if(rn_df0<2)
				{
					fl|=BGBCC_WXSPFL_FIXNOMOVE;
					break;
				}
				fl|=BGBCC_WXSPFL_RXN;
			}
		}
		break;
#endif

#if 1
	case 4:
		sc=1;
		switch((opw>>12)&15)
		{
		case 0:	 case 1:
			rn=rn_dfl;
			fl|=BGBCC_WXSPFL_LANE2;
			fl|=BGBCC_WXSPFL_LANE3;
			fl|=BGBCC_WXSPFL_1STAGE;
			break;
		case 2: case 3:
			rs=rn_dfl;
			rn=rn_dfl;
			fl|=BGBCC_WXSPFL_LANE2;
			break;
		case 4:
			rn=rn_dfl;
			break;
		case 5:
			sc=4;
			rn=rn_dfl;
			rs=BGBCC_SH_REG_GBR;
			fl|=BGBCC_WXSPFL_ISMEM;
			break;

		case 0xC:
			sc=(opw&0x20)?4:8;
			rn=rn_dfl;
			rs=BGBCC_SH_REG_GBR;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_HASDISP;
			break;
		case 0xD:
			sc=(opw&0x20)?4:8;
			rp=rn_dfl;
			rn=rn_dfl;
			rs=BGBCC_SH_REG_GBR;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_IS_STORE;
			fl|=BGBCC_WXSPFL_HASDISP;
			break;
		}
		imm=(((s32)opw)>>22)*sc;
		break;
#endif

	case 6:
	case 7:
		fl|=BGBCC_WXSPFL_FIXNOMOVE;
		fl|=BGBCC_WXSPFL_JUMBO;
		break;
	
	default:	/* Unknown */
		rn=rn_dfl;
		rs=rm_dfl;
		rt=ro_dfl;
		fl|=BGBCC_WXSPFL_FIXNOMOVE;
		ret=0;
		break;
	}
	
	if((rn>=64) && (rn!=BGBCC_SH_REG_ZZR))
		fl|=BGBCC_WXSPFL_FIXNOMOVE;
	
	*rrs=rs;
	*rrt=rt;
	*rrn=rn;
	*rrp=rp;
	*rspr=rma;
	*rspw=rna;
	*rspfl=fl;
	*rimm=imm;
	return(ret);
}

int BGBCC_JX2_CheckOpsRv_GetRegs(
	BGBCC_JX2_Context *sctx,
	u32 opw,
	u16 *rrs, u16 *rrt, u16 *rrn, u16 *rrp,
	u16 *rspr, u16 *rspw, int *rspfl)
{
	s64 tdisp;
	return(BGBCC_JX2_CheckOpsRv_GetRegsImm(sctx,
		opw, rrs, rrt, rrn, rrp, rspr, rspw, rspfl, &tdisp));
}

int BGBCC_JX2_CheckOpsRv_GetRegsImm(
	BGBCC_JX2_Context *sctx,
	u32 opw,
	u16 *rrs, u16 *rrt, u16 *rrn, u16 *rrp,
	u16 *rspr, u16 *rspw, int *rspfl, u64 *rimm)
{
	static u16 gr2spr[4]={
		BGBCC_SH_REG_ZZR,	BGBCC_SH_REG_LR,
		BGBCC_SH_REG_SP,	BGBCC_SH_REG_GBR };
	s64 imm;
	int rm_dfl, rn_dfl, ro_dfl, ru_dfl, rn_df0, ro_df0;
	int fun3, fun7, imm12l, imm12s, imm20l;
	int rm_fr, rn_fr, ro_fr, ru_fr;
	int rd, rs, rt, rp, fl, rma, rna, ret;
	
	if((opw&3)!=3)
	{
		if(!(sctx->emit_riscv&0x22))
		{
			if(!opw)
			{
				fl=BGBCC_WXSPFL_FIXNOMOVE;
				rs=BGBCC_SH_REG_ZZR;
				imm=0;
				*rrs=rs;	*rrt=rs;
				*rrn=rs;	*rrp=rs;
				*rspr=rs;	*rspw=rs;
				*rspfl=fl;	*rimm=imm;
				return(-1);
			}
			BGBCC_DBGBREAK
			return(-1);
		}
		return(BGBCC_JX2_CheckOpsX3_GetRegsImm(sctx,
			opw, rrs, rrt, rrn, rrp, rspr, rspw, rspfl, rimm));
	}
	
	rm_dfl=(opw>>15)&31;
	ro_dfl=(opw>>20)&31;
	rn_dfl=(opw>> 7)&31;
	ru_dfl=(opw>>27)&31;
	
	rn_df0=rn_dfl;
	ro_df0=ro_dfl;
	
	rm_fr=rm_dfl+32;
	rn_fr=rn_dfl+32;
	ro_fr=ro_dfl+32;
	ru_fr=ru_dfl+32;
	if(rm_dfl<4)	rm_dfl=gr2spr[rm_dfl];
	if(rn_dfl<4)	rn_dfl=gr2spr[rn_dfl];
	if(ro_dfl<4)	ro_dfl=gr2spr[ro_dfl];
	if(ru_dfl<4)	ru_dfl=gr2spr[ru_dfl];
	
	fun3=(opw>>12)&7;
	fun7=(opw>>25)&127;
	
	imm12l=(((s32)opw)>>20);
	imm12s=(imm12l&(~31))|((opw>> 7)&31);
	imm20l=((s32)opw)&(~4095);
	
	rd=BGBCC_SH_REG_ZZR;
	rs=BGBCC_SH_REG_ZZR;
	rt=BGBCC_SH_REG_ZZR;
	rp=BGBCC_SH_REG_ZZR;
	rma=BGBCC_SH_REG_ZZR;
	rna=BGBCC_SH_REG_ZZR;
	fl=0;
	ret=1;
	imm=0;
	
	switch((opw>>2)&31)
	{
#if 1
		case 0x00:	/* LOAD */
			if(fun3==7)
			{
				rd=rn_dfl;
				rs=rm_dfl;
				rt=ro_dfl;
				fl|=BGBCC_WXSPFL_ISMEM;
				fl|=BGBCC_WXSPFL_3STAGE;
				break;
			}
		
			rd=rn_dfl;
			rs=rm_dfl;
			imm=imm12l;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_3STAGE;
			fl|=BGBCC_WXSPFL_HASDISP;
			break;
		case 0x08:	/* STORE */
			rs=rm_dfl;
			rt=ro_dfl;
			rp=rm_dfl;
			imm=imm12s;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_IS_STORE;
			fl|=BGBCC_WXSPFL_HASDISP;
			break;
#endif

#if 1
		case 0x10:	/* FMADD */
		case 0x11:	/* FMSUB */
		case 0x12:	/* FNMSUB */
		case 0x13:	/* FNMADD */
			rd=rn_fr;
			rs=rm_fr;
			rt=ro_fr;
			rp=ru_fr;
			break;
		case 0x18:	/* Bcc */
			rs=rm_dfl;
			rt=ro_dfl;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		case 0x01: /* FP LOAD */
			rd=rn_fr;
			rs=rm_dfl;
			imm=imm12l;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_HASDISP;
			if(fun3==4)
			{
				rd=(rn_fr&31)^1;
				fl|=BGBCC_WXSPFL_RXN;
			}
			break;
		case 0x09:	/* FP STORE */
			rs=rm_dfl;
			rt=ro_fr;
			rp=ro_fr;
			imm=imm12s;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_IS_STORE;
			fl|=BGBCC_WXSPFL_HASDISP;
			if(fun3==4)
			{
				rp=(ro_fr&31)^1;
				fl|=BGBCC_WXSPFL_RXN;
			}
			break;
		case 0x19:	/* JALR */
			rs=rm_dfl;
			rt=ro_dfl;
			imm=imm12l;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		case 0x03:	/* FENCE/Etc */
			rs=rm_dfl;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		case 0x0B:	/* AMO */
			rd=rn_dfl;
			rs=rm_dfl;
			rt=ro_dfl;
			rp=rn_dfl;
			fl|=BGBCC_WXSPFL_ISMEM;
			fl|=BGBCC_WXSPFL_IS_STORE;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;

		case 0x1B:	/* JAL */
			rd=rn_dfl;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
#endif

#if 1
		case 0x04:	/* ALU 3RI */
			rd=rn_dfl;
			rs=rm_dfl;
			imm=imm12l;

			if(fun3==0)
			{
				fl|=BGBCC_WXSPFL_LANE2;
				fl|=BGBCC_WXSPFL_LANE3;

				if(imm12l==0)
					fl|=BGBCC_WXSPFL_1STAGE;
			}

			if(fun3==1)
			{
				if((imm12l&63)==imm12l)
					fl|=BGBCC_WXSPFL_LANE2;
			}

			if((fun3==2) || (fun3==3))
			{
				if(rn_df0==0)
				{
					rma=BGBCC_SH_REG_SR;
					rna=BGBCC_SH_REG_SR;
				}
			}

			if((fun3==4) || (fun3==6) || (fun3==7))
			{
				fl|=BGBCC_WXSPFL_LANE2;
				fl|=BGBCC_WXSPFL_LANE3;
			}
			break;
#endif
#if 1
		case 0x0C:	/* ALU 3R */
			rd=rn_dfl;
			rs=rm_dfl;
			rt=ro_dfl;

			if((fun3==0) || (fun3==4) || (fun3==6) || (fun3==7))
			{
				if(fun7==0)
				{
					fl|=BGBCC_WXSPFL_LANE2;
					fl|=BGBCC_WXSPFL_LANE3;
				}
				
				if((fun3==0) && (fun7==0x20))
				{
					fl|=BGBCC_WXSPFL_LANE2;
					fl|=BGBCC_WXSPFL_LANE3;
				}

				if((fun3==0) && (fun7==0))
				{
					if(ro_df0==0)
						fl|=BGBCC_WXSPFL_1STAGE;
				}
			}

			if((fun3==1) || (fun3==5))
			{
				if((fun7==0) || (fun7==0x20))
				{
					fl|=BGBCC_WXSPFL_LANE2;
				}
			}
			
			if(fun7==1)
				fl|=BGBCC_WXSPFL_3STAGE;

			if((fun3==2) || (fun3==3))
			{
				if(rn_df0==0)
				{
					rma=BGBCC_SH_REG_SR;
					rna=BGBCC_SH_REG_SR;
				}
			}
			break;
#endif

#if 1
		case 0x14:	/* FPU */
			rd=rn_fr;
			rs=rm_fr;
			rt=ro_fr;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		case 0x1C:	/* SYSTEM */
			rd=rn_dfl;
			rs=rm_dfl;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		case 0x05:	/* AUIPC */
			rd=rn_dfl;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			break;
		case 0x0D:	/* LUI */
			rd=rn_dfl;
			imm=imm20l;
			fl|=BGBCC_WXSPFL_LANE2;
			fl|=BGBCC_WXSPFL_LANE3;
			fl|=BGBCC_WXSPFL_1STAGE;
			break;
		case 0x1D:	/* RV-P 3R */
			rd=rn_dfl;
			rs=rm_dfl;
			rt=ro_dfl;
			break;
#endif

#if 1
		case 0x06:	/* ALUW 3RI */
			rd=rn_dfl;
			rs=rm_dfl;
			imm=imm12l;
			
			if(fun3==0)
			{
				fl|=BGBCC_WXSPFL_LANE2;
				fl|=BGBCC_WXSPFL_LANE3;
				if(imm12l==0)
					fl|=BGBCC_WXSPFL_1STAGE;
			}
			if((fun3==1) || (fun3==5))
			{
				fl|=BGBCC_WXSPFL_LANE2;
			}
			
			break;
		case 0x0E:	/* ALUW 3R */
			rd=rn_dfl;
			rs=rm_dfl;
			rt=ro_dfl;

			if((fun3==0) && ((fun7==0x00) || (fun7==0x20)))
			{
				fl|=BGBCC_WXSPFL_LANE2;
				fl|=BGBCC_WXSPFL_LANE3;

				if((fun7==0) && (ro_df0==0))
					fl|=BGBCC_WXSPFL_1STAGE;
			}

			if(((fun3==1) || (fun3==5)) && ((fun7==0x00) || (fun7==0x20)))
			{
				fl|=BGBCC_WXSPFL_LANE2;
			}

			if(fun7==1)
				fl|=BGBCC_WXSPFL_3STAGE;
			break;
#endif

		case 0x07:	/* OP48 */
		case 0x17:	/* OP48 */
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			fl|=BGBCC_WXSPFL_JUMBO;
			break;

		case 0x0F:	/* OP64 */
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			fl|=BGBCC_WXSPFL_JUMBO;
			if(fun3>=6)
				fl|=BGBCC_WXSPFL_JUMBOX2;
			break;

		case 0x1F:	/* OP80/OP96 */
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			fl|=BGBCC_WXSPFL_JUMBO;
			fl|=BGBCC_WXSPFL_JUMBOX2;
			break;

		default:	/* Unknown */
			rd=rn_dfl;
			rs=rm_dfl;
			rt=ro_dfl;
			fl|=BGBCC_WXSPFL_FIXNOMOVE;
			ret=0;
			break;
	}
	
	if((rd>=64) && (rd!=BGBCC_SH_REG_ZZR))
		fl|=BGBCC_WXSPFL_FIXNOMOVE;
	
	*rrs=rs;
	*rrt=rt;
	*rrn=rd;
	*rrp=rp;
	*rspr=rma;
	*rspw=rna;
	*rspfl=fl;
	*rimm=imm;
	return(ret);
}



int BGBCC_JX2_CheckOpsRv_IsMem(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 isld_hash_op[256];
	static byte isld_hash_pr[256];
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(isld_hash_op[h]==opw)
	{
		return(isld_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if(rspfl1&BGBCC_WXSPFL_ISMEM)
		rt=1;
	isld_hash_op[h]=opw;
	isld_hash_pr[h]=rt;
	return(rt);
}

int BGBCC_JX2_CheckOpsRv_IsLoad(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 isld_hash_op[256];
	static byte isld_hash_pr[256];
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(isld_hash_op[h]==opw)
	{
		return(isld_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if((rspfl1&BGBCC_WXSPFL_ISMEM) && !(rspfl1&BGBCC_WXSPFL_IS_STORE))
		rt=1;
	isld_hash_op[h]=opw;
	isld_hash_pr[h]=rt;
	return(rt);
}

int BGBCC_JX2_CheckOpsRv_Is2Stage(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 is2s_hash_op[256];
	static byte is2s_hash_pr[256];
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(is2s_hash_op[h]==opw)
	{
		return(is2s_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if(rspfl1&BGBCC_WXSPFL_2STAGE)
		rt=1;
	if(	!(rspfl1&BGBCC_WXSPFL_3STAGE) &&
		!(rspfl1&BGBCC_WXSPFL_1STAGE))
		rt=1;
	is2s_hash_op[h]=opw;
	is2s_hash_pr[h]=rt;
	return(rt);
}


int BGBCC_JX2_CheckOpsRv_Is3Stage(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 is3s_hash_op[256];
	static byte is3s_hash_pr[256];
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(is3s_hash_op[h]==opw)
	{
		return(is3s_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if(rspfl1&BGBCC_WXSPFL_3STAGE)
		rt=1;
	is3s_hash_op[h]=opw;
	is3s_hash_pr[h]=rt;
	return(rt);
}

int BGBCC_JX2_CheckOpsRv_IsFixed(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 isld_hash_op[256];
	static byte isld_hash_pr[256];
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(isld_hash_op[h]==opw)
	{
		return(isld_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if(rspfl1&BGBCC_WXSPFL_FIXNOMOVE)
		rt=1;
	isld_hash_op[h]=opw;
	isld_hash_pr[h]=rt;
	return(rt);
}

int BGBCC_JX2_CheckOpsRv_IsJumbo64(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 isld_hash_op[256];
	static byte isld_hash_pr[256];
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(isld_hash_op[h]==opw)
	{
		return(isld_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if(rspfl1&BGBCC_WXSPFL_JUMBO)
		rt=1;
	if(rspfl1&BGBCC_WXSPFL_JUMBOX2)
		rt=3;
	isld_hash_op[h]=opw;
	isld_hash_pr[h]=rt;
	return(rt);
}

int BGBCC_JX2_CheckOpsRv_IsJumbo96(
	BGBCC_JX2_Context *sctx, u32 opw0, u32 opw1)
{
	int rt0;
	
	rt0=BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw0);
	if(!rt0)
		return(0);
	if((rt0&3)==3)
		return(1);
	if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw1) )
			return(1);
	return(0);
}

int BGBCC_JX2_CheckOpsRv_IsLane2(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	static u32 isld_hash_op[256];
	static byte isld_hash_pr[256];
	u16 rs1, rt1, rn1, rspr1, rp1, rspw1, rs1b, rt1b, rn1b, rspr1b;
	int rspfl1, rspfl2;
	int ret1, ret2;
	int h, rt;

	h=((opw*(65521*251))>>24)&255;
	if(isld_hash_op[h]==opw)
	{
		return(isld_hash_pr[h]);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);

	rt=0;	
	if(rspfl1&BGBCC_WXSPFL_LANE2)
		rt=1;
	if(rspfl1&BGBCC_WXSPFL_LANE3)
		rt=3;
	isld_hash_op[h]=opw;
	isld_hash_pr[h]=rt;
	return(rt);
}

/* Infer whether or not a pair of mem-ops can alias. */
int BGBCC_JX2_CheckOpsRv_MemNoAlias(
	BGBCC_JX2_Context *sctx,
	u32 opw1, u32 opw2, int fl)
{
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1;
	u16 rs1a, rt1a, rn1a, rp1a, rspr1a;
	u16 rs1b, rt1b, rn1b, rp1b, rspr1b;
	u16 rs2, rt2, rn2, rp2, rspr2, rspw2;
	u16 rs2a, rt2a, rn2a, rp2a, rspr2a;
	u16 rs2b, rt2b, rn2b, rp2b, rspr2b;
	s64 rimm1, rimm2;
	int rspfl1, rspfl2;
	int opw1v, opw3v;
	int ret1, ret2, sbi;

	if(!opw1 || !opw2)
		return(0);

	if(sctx->state_alias&4)
	{
		/* if volatile was seen, assume aliasing. */
		return(0);
	}

	ret1=BGBCC_JX2_CheckOpsRv_GetRegsImm(sctx, opw1,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1, &rimm1);
	ret2=BGBCC_JX2_CheckOpsRv_GetRegsImm(sctx, opw2,
		&rs2, &rt2, &rn2, &rp2, &rspr2, &rspw2, &rspfl2, &rimm2);
	if(ret1<=0)		return(0);
	if(ret2<=0)		return(0);

	if(!(rspfl1&BGBCC_WXSPFL_ISMEM))	return(0);
	if(!(rspfl2&BGBCC_WXSPFL_ISMEM))	return(0);

	if(	!(rspfl1&BGBCC_WXSPFL_IS_STORE) &&
		!(rspfl2&BGBCC_WXSPFL_IS_STORE) )
	{
		/* Both loads, assume no-alias. */
		return(1);
	}

	if(
		((rs1==BGBCC_SH_REG_GBR) && (rs2==BGBCC_SH_REG_SP ))	||
		((rs1==BGBCC_SH_REG_SP ) && (rs2==BGBCC_SH_REG_GBR))	)
	{
		/* Stack and global accesses don't alias. */
		return(1);
	}

	if(
		((rs1==BGBCC_SH_REG_GBR) && (rs2!=BGBCC_SH_REG_GBR))	||
		((rs1!=BGBCC_SH_REG_GBR) && (rs2==BGBCC_SH_REG_GBR))	)
	{
		if(!(sctx->state_alias&2))
		{
			/* Assume global accesses don't alias. */
			return(1);
		}
	}

	if(	((rs1==BGBCC_SH_REG_SP) && (rs2!=BGBCC_SH_REG_SP))		||
		((rs1!=BGBCC_SH_REG_SP) && (rs2==BGBCC_SH_REG_SP))		)
	{
		if(!(sctx->state_alias&1))
		{
			/* Assume stack accesses don't alias. */
			return(1);
		}
	}

	if(!(rspfl1&BGBCC_WXSPFL_HASDISP))	return(0);
	if(!(rspfl2&BGBCC_WXSPFL_HASDISP))	return(0);

	if(rs1==rs2)
	{
		sbi=7;
		if(	(rspfl1&BGBCC_WXSPFL_RXN) ||
			(rspfl2&BGBCC_WXSPFL_RXN) )
				sbi=15;
		
		if(
			((rimm1+  0)>>3)!=((rimm2+  0)>>3) &&
			((rimm1+  0)>>3)!=((rimm2+sbi)>>3) &&
			((rimm1+sbi)>>3)!=((rimm2+  0)>>3) &&
			((rimm1+sbi)>>3)!=((rimm2+sbi)>>3) )
		{
			return(1);
		}
	}

	return(0);
}


int BGBCC_JX2_CheckOpsRv_SequenceOnlyB(
	BGBCC_JX2_Context *sctx,
	u32 opw1, u32 opw2, int fl)
{
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1;
	u16 rs1a, rt1a, rn1a, rp1a, rspr1a;
	u16 rs1b, rt1b, rn1b, rp1b, rspr1b;
	u16 rs2, rt2, rn2, rp2, rspr2, rspw2;
	u16 rs2a, rt2a, rn2a, rp2a, rspr2a;
	u16 rs2b, rt2b, rn2b, rp2b, rspr2b;
	int rspfl1, rspfl2;
	int opw1v, opw3v;
	int ret1, ret2;

	if(!opw1 || !opw2)
		return(1);

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw1,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw2,
		&rs2, &rt2, &rn2, &rp2, &rspr2, &rspw2, &rspfl2);

	if(ret1<0)
		return(ret1);
	if(ret2<0)
		return(ret2);
	
	if(!ret1)
		return(1);
	if(!ret2)
		return(1);

	if(	((opw1&2)==0) &&
		((opw2&2)==0) &&
		((opw1^opw2)&1))
	{
		rn1=BGBCC_SH_REG_ZZR;
		rn2=BGBCC_SH_REG_ZZR;
		rp1=BGBCC_SH_REG_ZZR;
		rp2=BGBCC_SH_REG_ZZR;
		
		rspfl1&=~BGBCC_WXSPFL_IS_STORE;
		rspfl2&=~BGBCC_WXSPFL_IS_STORE;
	}

	if(	(rspfl1&1) && (rspfl2&1) &&
		((rp1!=BGBCC_SH_REG_ZZR) || (rp2!=BGBCC_SH_REG_ZZR)) &&
		!BGBCC_JX2_CheckOpsRv_MemNoAlias(sctx, opw1, opw2, 0))
	{
		return(1);
	}

	if((rs1==BGBCC_SH_REG_PC) || (rs2==BGBCC_SH_REG_PC))
		return(1);

	if(rspfl1&2)
	{
		if(rspw2==BGBCC_SH_REG_SR)
			return(1);
	}

	if(rspfl2&2)
	{
		if(rspw1==BGBCC_SH_REG_SR)
			return(1);
	}
	
	rs1b=BGBCC_SH_REG_ZZR;		rs2b=BGBCC_SH_REG_ZZR;
	rt1b=BGBCC_SH_REG_ZZR;		rt2b=BGBCC_SH_REG_ZZR;
	rp1b=BGBCC_SH_REG_ZZR;		rp2b=BGBCC_SH_REG_ZZR;
	rn1b=BGBCC_SH_REG_ZZR;		rn2b=BGBCC_SH_REG_ZZR;
	rspr1b=BGBCC_SH_REG_ZZR;	rspr2b=BGBCC_SH_REG_ZZR;

	BGBCC_JX2_RemapReg5Xn(sctx, rs1, &rs1a, &rs1b, rspfl1, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt1, &rt1a, &rt1b, rspfl1, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1a, &rn1b, rspfl1, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1a, &rp1b, rspfl1, 4);

	BGBCC_JX2_RemapReg5Xn(sctx, rs2, &rs2a, &rs2b, rspfl2, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt2, &rt2a, &rt2b, rspfl2, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2a, &rn2b, rspfl2, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2a, &rp2b, rspfl2, 4);

	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if(		(rn2==rn1) || (rs2==rn1) ||
				(rt2==rn1) || (rp2==rn1) ||
				(rspr2==rn1))
			return(1);
		if(		(rn2a==rn1) || (rs2a==rn1) ||
				(rt2a==rn1) || (rp2a==rn1) )
			return(1);
		if(		(rn2b==rn1) || (rs2b==rn1) ||
				(rt2b==rn1) || (rp2b==rn1) )
			return(1);
	}
	if(rn1a!=BGBCC_SH_REG_ZZR)
	{
		if(		(rn2==rn1a) || (rs2==rn1a) ||
				(rt2==rn1a) || (rp2==rn1a) )
			return(1);
		if(		(rn2a==rn1a) || (rs2a==rn1a) ||
				(rt2a==rn1a) ||	(rp2a==rn1a) )
			return(1);
		if(		(rn2b==rn1a) || (rs2b==rn1a) ||
				(rt2b==rn1a) || (rp2b==rn1a) )
			return(1);
	}
	if(rn1b!=BGBCC_SH_REG_ZZR)
	{
		if(		(rn2==rn1b) || (rs2==rn1b) ||
				(rt2==rn1b) || (rp2==rn1b) ||
				(rspr2==rn1b))
			return(1);
		if(		(rn2a==rn1b) || (rs2a==rn1b) ||
				(rt2a==rn1b) ||	(rp2a==rn1b) )
			return(1);
		if(		(rn2b==rn1b) || (rs2b==rn1b) ||
				(rt2b==rn1b) || (rp2b==rn1b) ||
				(rspr2b==rn1b))
			return(1);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if(	(rspr2==rspw1)	||	(rn2==rspw1) ||
			(rs2==rspw1)	|| 	(rt2==rspw1) ||
			(rp2==rspw1))
				return(1);
		if(	(rn2b==rspw1)	|| (rs2b==rspw1) ||
			(rt2b==rspw1)	|| (rspr2b==rspw1)	||
			(rp2b==rspw1))
			return(1);
	}

	if(fl&1)
	{
		if(	(rspfl1&BGBCC_WXSPFL_ISMEM) &&
			(rspfl2&BGBCC_WXSPFL_ISMEM) && 
			(	(rspfl1&BGBCC_WXSPFL_IS_STORE) ||
				(rspfl2&BGBCC_WXSPFL_IS_STORE)))
		{
			if(!BGBCC_JX2_CheckOpsRv_MemNoAlias(sctx,
				opw1, opw2, 0))
			{
				return(1);
			}
		}

		if(rn2!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2) || (rs1==rn2) ||
				(rt1==rn2) || (rp1==rn2) ||
				(rspr1==rn2))
					return(1);

			if(	(rn1a==rn2) || (rs1a==rn2) ||
				(rt1a==rn2) || (rp1a==rn2) )
					return(1);

			if(	(rn1b==rn2) || (rs1b==rn2) ||
				(rt1b==rn2) || (rp1b==rn2) ||
				(rspr1b==rn2))
					return(1);
		}

		if(rn2a!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2a) || (rs1==rn2a) ||
				(rt1==rn2a) || (rp1==rn2a) )
					return(1);
			if(	(rn1a==rn2a) || (rs1a==rn2a) ||
				(rt1a==rn2a) || (rp1a==rn2a) )
					return(1);
			if(	(rn1b==rn2a) || (rs1b==rn2a) ||
				(rt1b==rn2a) || (rp1b==rn2a) )
					return(1);
		}

		if(rn2b!=BGBCC_SH_REG_ZZR)
		{
			if(	(rn1==rn2b) || (rs1==rn2b) ||
				(rt1==rn2b) || (rp1==rn2b) ||
				(rspr1==rn2b))
					return(1);

			if(	(rn1a==rn2b) || (rs1a==rn2b) ||
				(rt1a==rn2b) || (rp1a==rn2b) )
					return(1);

			if(	(rn1b==rn2b) || (rs1b==rn2b) ||
				(rt1b==rn2b) || (rp1b==rn2b) ||
				(rspr1b==rn2b))
					return(1);
		}

		if(rspw2!=BGBCC_SH_REG_ZZR)
		{
			if(	(rspr1==rspw2)	||	(rn1==rspw2)	||
				(rs1==rspw2)	||	(rt1==rspw2)	||
				(rp1==rspw2))
					return(1);
		}
	}

	return(0);
}


u32 bgbcc_jx2_inferinterlock_op1[4096];
u32 bgbcc_jx2_inferinterlock_op2[4096];
u32 bgbcc_jx2_inferinterlock_op3[4096];
byte bgbcc_jx2_inferinterlock_res[4096];


int BGBCC_JX2_InferOpsRv_Interlock(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int fl)
{
	int oph, res;

	oph=opw1+(opw2*5)+(opw3*9);
	oph=oph+(oph>>17);
	oph=((oph*65521)>>16)&4095;
	
	if(	(bgbcc_jx2_inferinterlock_op1[oph]==opw1) &&
		(bgbcc_jx2_inferinterlock_op2[oph]==opw2) &&
		(bgbcc_jx2_inferinterlock_op3[oph]==opw3))
	{
		return(bgbcc_jx2_inferinterlock_res[oph]);
	}
	
	res=BGBCC_JX2_InferOpsRv_InterlockI(sctx,
		opw1, opw2, opw3, fl);
	bgbcc_jx2_inferinterlock_op1[oph]=opw1;
	bgbcc_jx2_inferinterlock_op2[oph]=opw2;
	bgbcc_jx2_inferinterlock_op3[oph]=opw3;
	bgbcc_jx2_inferinterlock_res[oph]=res;
	return(res);
}

int BGBCC_JX2_InferOpsRv_InterlockEx2(
	BGBCC_JX2_Context *sctx,
	int opw1, int opw2, int opw3, int opw4, int opw5, int fl)
{
	int cf1, cf2, cf;
	
	cf1=BGBCC_JX2_InferOpsRv_Interlock(sctx,
		opw1, opw2, opw3, fl);
	cf2=BGBCC_JX2_InferOpsRv_Interlock(sctx,
		opw1, opw4, opw5, fl);

#if 1
	if(cf1>1)
	{
		cf=cf1+3;
		if(cf1==3)
			cf+=1;
	}
	else if(cf2>1)
		{ cf=cf2-1; }
	else
		{ cf=1; }

	return(cf);
#endif
}

int BGBCC_JX2_InferOpsRv_InterlockI(
	BGBCC_JX2_Context *sctx,
	u32 opw1, u32 opw2, u32 opw3, int fl)
{
	u16 rs1, rt1, rn1, rp1, rspr1, rspw1;
	u16 rs1a, rt1a, rn1a, rp1a, rspr1a;
	u16 rs1b, rt1b, rn1b, rp1b, rspr1b;
	u16 rs2, rt2, rn2, rp2, rspr2, rspw2;
	u16 rs2a, rt2a, rn2a, rp2a, rspr2a;
	u16 rs2b, rt2b, rn2b, rp2b, rspr2b;
	u16 rs3, rt3, rn3, rp3, rspr3, rspw3;
	u16 rs3a, rt3a, rn3a, rp3a, rspr3a;
	u16 rs3b, rt3b, rn3b, rp3b, rspr3b;
	int rspfl1, rspfl2, rspfl3;
	int ret1, ret2, ret3;

	ret1=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw1,
		&rs1, &rt1, &rn1, &rp1, &rspr1, &rspw1, &rspfl1);
	ret2=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw2,
		&rs2, &rt2, &rn2, &rp2, &rspr2, &rspw2, &rspfl2);
	ret3=BGBCC_JX2_CheckOpsRv_GetRegs(sctx, opw3,
		&rs3, &rt3, &rn3, &rp3, &rspr3, &rspw3, &rspfl3);

	if(ret1<0)
		return(0);
	if(ret2<0)
		return(0);
	if(ret3<0)
		return(0);
	
	if(!ret1)
		return(0);
	if(!ret2)
		return(0);
	if(!ret3)
		return(0);

	if(rspfl1&BGBCC_WXSPFL_IS_STORE)
	{
		/* Stores do not trigger Rn interlock. */
		return(1);
	}

	rs1b=BGBCC_SH_REG_ZZR;	rs2b=BGBCC_SH_REG_ZZR;	rs3b=BGBCC_SH_REG_ZZR;
	rt1b=BGBCC_SH_REG_ZZR;	rt2b=BGBCC_SH_REG_ZZR;	rt3b=BGBCC_SH_REG_ZZR;
	rn1b=BGBCC_SH_REG_ZZR;	rn2b=BGBCC_SH_REG_ZZR;	rn3b=BGBCC_SH_REG_ZZR;
	rspr1b=BGBCC_SH_REG_ZZR;
	rspr2b=BGBCC_SH_REG_ZZR;
	rspr3b=BGBCC_SH_REG_ZZR;

	BGBCC_JX2_RemapReg5Xn(sctx, rs1, &rs1a, &rs1b, rspfl1, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt1, &rt1a, &rt1b, rspfl1, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn1, &rn1a, &rn1b, rspfl1, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp1, &rp1a, &rp1b, rspfl1, 4);

	BGBCC_JX2_RemapReg5Xn(sctx, rs2, &rs2a, &rs2b, rspfl2, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt2, &rt2a, &rt2b, rspfl2, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn2, &rn2a, &rn2b, rspfl2, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp2, &rp2a, &rp2b, rspfl2, 4);

	BGBCC_JX2_RemapReg5Xn(sctx, rs3, &rs3a, &rs3b, rspfl3, 1);
	BGBCC_JX2_RemapReg5Xn(sctx, rt3, &rt3a, &rt3b, rspfl3, 2);
	BGBCC_JX2_RemapReg5Xn(sctx, rn3, &rn3a, &rn3b, rspfl3, 3);
	BGBCC_JX2_RemapReg5Xn(sctx, rp3, &rp3a, &rp3b, rspfl3, 4);

	if(rn1!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1) || (rs2==rn1) || (rt2==rn1) || (rspr2==rn1))
			return(3);
		if((rn2b==rn1) || (rs2b==rn1) || (rt2b==rn1) || (rspr2b==rn1))
			return(3);
		if((rn2a==rn1) || (rs2a==rn1) || (rt2a==rn1) )
			return(3);

		if((rn3==rn1) || (rs3==rn1) || (rt3==rn1) || (rspr3==rn1))
			return(2);
		if((rn3b==rn1) || (rs3b==rn1) || (rt3b==rn1) || (rspr3b==rn1))
			return(2);
		if((rn3a==rn1) || (rs3a==rn1) || (rt3a==rn1) )
			return(2);
	}
	if(rn1b!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1b) || (rs2==rn1b) || (rt2==rn1b) || (rspr2==rn1b))
			return(3);
		if((rn2b==rn1b) || (rs2b==rn1b) || (rt2b==rn1b) || (rspr2b==rn1b))
			return(3);
		if((rn2a==rn1b) || (rs2a==rn1b) || (rt2a==rn1b) )
			return(3);

		if((rn3==rn1b) || (rs3==rn1b) || (rt3==rn1b) || (rspr3==rn1b))
			return(2);
		if((rn3b==rn1b) || (rs3b==rn1b) || (rt3b==rn1b) || (rspr3b==rn1b))
			return(2);
		if((rn3a==rn1b) || (rs3a==rn1b) || (rt3a==rn1b) )
			return(2);
	}

	if(rn1a!=BGBCC_SH_REG_ZZR)
	{
		if((rn2==rn1a) || (rs2==rn1a) || (rt2==rn1a) )
			return(3);
		if((rn2b==rn1a) || (rs2b==rn1a) || (rt2b==rn1a) )
			return(3);
		if((rn2a==rn1a) || (rs2a==rn1a) || (rt2a==rn1a) )
			return(3);

		if((rn3==rn1a) || (rs3==rn1a) || (rt3==rn1a) )
			return(2);
		if((rn3b==rn1a) || (rs3b==rn1a) || (rt3b==rn1a) )
			return(2);
		if((rn3a==rn1a) || (rs3a==rn1a) || (rt3a==rn1a) )
			return(2);
	}

	if(rspw1!=BGBCC_SH_REG_ZZR)
	{
		if(	(rspr2==rspw1)	||	(rn2==rspw1) ||
			(rs2==rspw1)	|| 	(rt2==rspw1))
				return(3);
		if(	(rn2b==rspw1)	|| (rs2b==rspw1) ||
			(rt2b==rspw1)	|| (rspr2b==rspw1))
			return(3);

		if(	(rspr3==rspw1)	||	(rn3==rspw1) ||
			(rs3==rspw1)	|| 	(rt3==rspw1))
				return(2);
		if(	(rn3b==rspw1)	|| (rs3b==rspw1) ||
			(rt3b==rspw1)	|| (rspr3b==rspw1))
			return(2);
	}

	return(1);
}

/* Return true if this operation is immovable and must remain in place. */
int BGBCC_JX2_CheckOpsRv_Immovable(
	BGBCC_JX2_Context *sctx, u32 opw)
{
	return(BGBCC_JX2_CheckOpsRv_ImmovableFl(sctx, opw, 0));
}

int BGBCC_JX2_CheckOpsRv_ImmovableFl(
	BGBCC_JX2_Context *sctx,
	u32 opw, int flag)
{
	if(BGBCC_JX2_CheckOpsRv_IsFixed(sctx, opw))
		return(1);
	return(0);
}


u64		bgbcc_jx2_seqonly_opex[4096];
byte	bgbcc_jx2_seqonly_res[4096];

int BGBCC_JX2_CheckOpsRv_SequenceOnly(
	BGBCC_JX2_Context *sctx,
	u32 opw1, u32 opw2)
{
	u64 opex, opeh;
	int ret, h, res;
	
	opex=	(((u64)opw1)<< 0) |
			(((u64)opw2)<<32) ;

	opeh=opex+(opex>>7)+(opex>>13)+(opex>>21);
	h=((opeh*65521)>>16)&4095;
	
	if(bgbcc_jx2_seqonly_opex[h]==opex)
	{
		return(bgbcc_jx2_seqonly_res[h]);
	}
	
	ret=BGBCC_JX2_CheckOpsRv_SequenceOnlyB(sctx,
		opw1, opw2, 1);
	
	res=(ret!=0);
	bgbcc_jx2_seqonly_opex[h]=opex;
	bgbcc_jx2_seqonly_res[h]=res;

	return(res);
}

u64		bgbcc_jx2_canswap_opex[256];
byte	bgbcc_jx2_canswap_res[256];

int BGBCC_JX2_CheckOpsRv_CanSwap(
	BGBCC_JX2_Context *sctx,
	u32 opw1, u32 opw2)
{
	u64 opex, opeh;
	int ret, h, res;
	
	opex=	(((u64)opw1)<< 0) |
			(((u64)opw2)<<32) ;
	opeh=opex+(opex>>7)+(opex>>13)+(opex>>21);
	h=((opeh*65521)>>16)&255;
	
	if(bgbcc_jx2_canswap_opex[h]==opex)
	{
		return(bgbcc_jx2_canswap_res[h]);
	}

	if(	BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw1) ||
		BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw2))
	{
		bgbcc_jx2_canswap_opex[h]=opex;
		bgbcc_jx2_canswap_res[h]=0;
		return(0);
	}
	
	ret=BGBCC_JX2_CheckOpsRv_SequenceOnlyB(sctx,
		opw1, opw2, 1);

	res=!ret;
	if(ret<0)
		res=0;

	bgbcc_jx2_canswap_opex[h]=opex;
	bgbcc_jx2_canswap_res[h]=res;

	return(res);
}

int BGBCC_JX2_CheckOpsRv_CanSwapOrJ(
	BGBCC_JX2_Context *sctx,
	u32 opw1, u32 opw2)
{
	if(BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw1, opw2))
		return(1);
	if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw1))
		return(1);
	if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw2))
		return(1);
	return(0);
}

int BGBCC_JX2_CheckOpsRv_InferInterlockCost(
	BGBCC_JX2_Context *sctx,
	u32 opwn7, u32 opwn5,
	u32 opwn3, u32 opwn1,
	u32 opw1, u32 opw3, u32 opw5, u32 opw7,
	u32 opw9, u32 opw11)
{
	int cfn1, cfn3, cfn5, cfn7, cf1, cf2, cf3, cf4;
	int cn7, cn5, cn3, cn1, c1, c2, c3, c4, c5;

	cfn7=BGBCC_JX2_InferOpsRv_InterlockEx2(sctx,
		opwn7, opwn5, opwn3, opwn1, opw1, 0);
	cfn5=BGBCC_JX2_InferOpsRv_InterlockEx2(sctx,
		opwn5, opwn3, opwn1, opw1, opw3, 0);
	cfn3=BGBCC_JX2_InferOpsRv_InterlockEx2(sctx,
		opwn3, opwn1, opw1, opw3, opw5, 0);

#if 1
	cfn1=BGBCC_JX2_InferOpsRv_InterlockEx2(sctx,
		opwn1, opw1, opw3, opw5, opw7, 0);
	cf1=BGBCC_JX2_InferOpsRv_InterlockEx2(sctx,
		opw1, opw3, opw5, opw7, opw9, 0);
	cf2=BGBCC_JX2_InferOpsRv_InterlockEx2(sctx,
		opw3, opw5, opw7, opw9, opw11, 0);
#endif

	cf3=BGBCC_JX2_InferOpsRv_Interlock(sctx,
		opw5, opw7, opw9, 0);
	cf4=BGBCC_JX2_InferOpsRv_Interlock(sctx,
		opw7, opw9, opw11, 0);

	cn7=1;	cn5=1;
	cn3=1;	cn1=1;
	c1=1;	c2=1;
	c3=1;	c4=1;

	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opwn7) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opwn7) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opwn7)))
	{
		cn7=cfn7;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opwn7))
	{
		cn7=cfn7-1;
		if(cn7<1)
			cn7=1;
	}else
	{
		cn7=cfn7-3;
		if(cn7<1)
			cn7=1;
	}

	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opwn5) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opwn5) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opwn5)))
	{
		cn5=cfn5;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opwn5))
	{
		cn5=cfn5-1;
		if(cn5<1)
			cn5=1;
	}else
	{
		cn5=cfn5-3;
		if(cn5<1)
			cn5=1;
	}
	
	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opwn3) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opwn3) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opwn3)))
	{
		cn3=cfn3;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opwn3))
	{
		cn3=cfn3-1;
		if(cn3<1)
			cn3=1;
	}else
	{
		cn3=cfn3-3;
		if(cn3<1)
			cn3=1;
	}
	
	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opwn1) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opwn1) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opwn1)))
	{
		cn1=cfn1;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opwn1))
	{
		cn1=cfn1-1;
		if(cn1<1)
			cn1=1;
	}else
	{
		cn1=cfn1-3;
		if(cn1<1)
			cn1=1;
	}

	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opw1) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opw1) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opw1)))
	{
		c1=cf1;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opw1))
	{
		c1=cf1-1;
		if(c1<1)
			c1=1;
	}else
	{
		c1=cf1-3;
		if(c1<1)
			c1=1;
	}

	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opw3) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opw3) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opw3)))
	{
		c2=cf2;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opw3))
	{
		c2=cf2-1;
		if(c2<1)
			c2=1;
	}else
	{
		c2=cf2-3;
		if(c2<1)
			c2=1;
	}

#if 1
	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opw5) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opw5) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opw5)))
	{
		c3=cf3;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opw5))
	{
		c3=cf3-1;
		if(c3<1)
			c3=1;
	}else
	{
		c3=cf3-3;
		if(c3<1)
			c3=1;
	}

	if(	BGBCC_JX2_CheckOpsRv_IsLoad(sctx, opw7) ||
		(BGBCC_JX2_CheckOpsRv_Is3Stage(sctx, opw7) &&
		!BGBCC_JX2_CheckOpsRv_IsMem(sctx, opw7)))
	{
		c4=cf4;
	}else if(BGBCC_JX2_CheckOpsRv_Is2Stage(sctx, opw7))
	{
		c4=cf4-1;
		if(c4<1)
			c4=1;
	}else
	{
		c4=cf4-3;
		if(c4<1)
			c4=1;
	}
#endif	

	c5=0;

	if(	!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw1) &&
		!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw3) )
			c5+=1;
	if(	!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw3) &&
		!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw5) )
			c5+=1;
	if(	!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw5) &&
		!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw7) )
			c5+=1;
	if(	!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw7) &&
		!BGBCC_JX2_CheckOpsRv_IsLane2(sctx, opw9) )
			c5+=1;

	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn7, opwn5))	c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn5, opwn3))	c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn3, opwn1))	c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn1, opw1))	c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw1, opw3))		c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw3, opw5))		c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw5, opw7))		c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw7, opw9))		c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw9, opw11))	c5+=2;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn7, opwn3))	c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn5, opwn1))	c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn3, opw1))	c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opwn1, opw3))	c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw1, opw5))		c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw3, opw7))		c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw5, opw9))		c5+=1;
	if(BGBCC_JX2_CheckOpsRv_SequenceOnly(sctx, opw7, opw11))	c5+=1;
	
	c5>>=1;
	
	if(c1<1)	c1=1;
	if(c2<1)	c2=1;
	if(c3<1)	c3=1;
	if(c4<1)	c4=1;
	return(cn7+cn5+cn3+cn1+c1+c2+c3+c4+c5);
}


int BGBCC_JX2_LookupRelocCsOffs_Is32P(BGBCC_JX2_Context *ctx, int ofs)
{
	int tg;
	
	tg=BGBCC_JX2_LookupRelocTagCsOffs(ctx, ofs);
	if(!tg)
		return(0);
	return(1);
}

int BGBCC_JX2_LookupRelocCsOffs_Is64P(BGBCC_JX2_Context *ctx, int ofs)
{
	int tg;
	
	tg=BGBCC_JX2_LookupRelocTagCsOffs(ctx, ofs);
	if(!tg)
		return(0);
	if(tg==BGBCC_SH_RLC_REL32UI_RVI)	return(1);
	if(tg==BGBCC_SH_RLC_PBO32_RVI)		return(1);
	if(tg==BGBCC_SH_RLC_PBO32J_RVI)		return(1);
	if(tg==BGBCC_SH_RLC_PBO32LD_RVI)	return(1);
	if(tg==BGBCC_SH_RLC_PBO32ST_RVI)	return(1);
	if(tg==BGBCC_SH_RLC_PBO32STJ_RVI)	return(1);
	if(tg==BGBCC_SH_RLC_RELW33J_RVI)	return(1);

	if(tg==BGBCC_SH_RLC_RELB33_XG3)		return(1);
	if(tg==BGBCC_SH_RLC_RELW33_XG3)		return(1);
	if(tg==BGBCC_SH_RLC_PBOJ33_XG3)		return(1);
	return(0);
}

int BGBCC_JX2_LookupRelocCsOffs_Is96P(BGBCC_JX2_Context *ctx, int ofs)
{
	int tg;
	
	tg=BGBCC_JX2_LookupRelocTagCsOffs(ctx, ofs);
	if(!tg)
		return(0);
	if(tg==BGBCC_SH_RLC_PBO32LD_RVI)	return(1);
	if(tg==BGBCC_SH_RLC_PBO32ST_RVI)	return(1);
	return(0);
}

ccxl_status BGBCC_JX2_OptX3Interlock_DoSwaps(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	u32 opw0, opw1, opw2, opw3, opw4, opw5, opwn1, opwn2, opwn3, opwn4;
	int imv0, imv1, imv2, imv3, imv4, imv5, imvn1, imvn2;
	int dp, cp, nswap, nadvl;
	int i, j, k;

	nswap=0;	nadvl=256;
	cp=spos;
	while((cp+7)<epos)
//	while((cp+11)<epos)
//	while((cp+15)<epos)
	{
		if(nadvl>128)
			nadvl=128;
	
		nadvl--;
		if(nadvl<=0)
		{
			cp+=4;
			nadvl+=8;
			continue;
		}

		opw0 =BGBCC_JX2_EmitGetOffsDWord(sctx, cp+ 0);
		opw1 =BGBCC_JX2_EmitGetOffsDWord(sctx, cp+ 4);
		opw2 =BGBCC_JX2_EmitGetOffsDWord(sctx, cp+ 8);
		opw3 =BGBCC_JX2_EmitGetOffsDWord(sctx, cp+12);
		opw4 =BGBCC_JX2_EmitGetOffsDWord(sctx, cp+16);
		opw5 =BGBCC_JX2_EmitGetOffsDWord(sctx, cp+20);
		opwn1=BGBCC_JX2_EmitGetOffsDWord(sctx, cp- 4);
		opwn2=BGBCC_JX2_EmitGetOffsDWord(sctx, cp- 8);
		opwn3=BGBCC_JX2_EmitGetOffsDWord(sctx, cp-12);
		opwn4=BGBCC_JX2_EmitGetOffsDWord(sctx, cp-16);

		if(!(sctx->emit_riscv&0x22))
		{
			if(((cp-16)<spos) && ((opwn4&3)!=3))	opwn4=0;
			if(((cp-12)<spos) && ((opwn3&3)!=3))	opwn3=0;
			if(((cp- 8)<spos) && ((opwn2&3)!=3))	opwn2=0;
			if(((cp- 4)<spos) && ((opwn1&3)!=3))	opwn1=0;
		}

		if((cp+ 8)>=epos)	opw2=0;
		if((cp+12)>=epos)	opw3=0;
		if((cp+16)>=epos)	opw4=0;
		if((cp+20)>=epos)	opw5=0;

		imv0=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw0)>0);
		imv1=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw1)>0);
		imv2=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw2)>0);
		imv3=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw3)>0);
		imv4=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw4)>0);
		imv5=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opw5)>0);
		imvn1=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opwn1)>0);
		imvn2=(BGBCC_JX2_CheckOpsRv_Immovable(sctx, opwn2)>0);

		if((cp+ 8)>=epos)	imv2|=1;
		if((cp+12)>=epos)	imv3|=1;
		if((cp+16)>=epos)	imv4|=1;
		if((cp+20)>=epos)	imv5|=1;

		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opwn2))	{ imvn1|=1; imvn2|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opwn1))	{ imv0|=1; imvn1|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw0))	{ imv1|=1; imv0|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw1))	{ imv2|=1; imv1|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw2))	{ imv3|=1; imv2|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw3))	{ imv4|=1; imv3|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo64(sctx, opw4))	{ imv5|=1; imv4|=1; }

		if(BGBCC_JX2_CheckOpsRv_IsJumbo96(sctx, opwn2, opwn1))
			{ imv0|=1; imvn1|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo96(sctx, opwn1, opw0))
			{ imv1|=1; imv0|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo96(sctx, opw0, opw1))
			{ imv2|=1; imv1|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo96(sctx, opw1, opw2))
			{ imv3|=1; imv2|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo96(sctx, opw2, opw3))
			{ imv4|=1; imv3|=1; }
		if(BGBCC_JX2_CheckOpsRv_IsJumbo96(sctx, opw3, opw4))
			{ imv5|=1; imv4|=1; }

		if(BGBCC_JX2_LookupRelocCsOffs_Is32P(sctx, cp+ 0))	imv0|=1;
		if(BGBCC_JX2_LookupRelocCsOffs_Is32P(sctx, cp+ 4))	imv1|=1;
		if(BGBCC_JX2_LookupRelocCsOffs_Is32P(sctx, cp+ 8))	imv2|=1;
		if(BGBCC_JX2_LookupRelocCsOffs_Is32P(sctx, cp+12))	imv3|=1;

		if(BGBCC_JX2_LookupRelocCsOffs_Is64P(sctx, cp- 4))	imv0|=1;
		if(BGBCC_JX2_LookupRelocCsOffs_Is64P(sctx, cp+ 0))	imv1|=1;
		if(BGBCC_JX2_LookupRelocCsOffs_Is64P(sctx, cp+ 4))	imv2|=1;
		if(BGBCC_JX2_LookupRelocCsOffs_Is64P(sctx, cp+ 8))	imv3|=1;

		if(BGBCC_JX2_LookupRelocCsOffs_Is96P(sctx, cp- 8))
			 { imv0|=1; imvn1|=1; imvn2|=1; }
		if(BGBCC_JX2_LookupRelocCsOffs_Is96P(sctx, cp- 4))
			{ imv1|=1; imv0|=1; imvn1|=1; }
		if(BGBCC_JX2_LookupRelocCsOffs_Is96P(sctx, cp+ 0))
			{ imv2|=1; imv1|=1; imv0|=1; }
		if(BGBCC_JX2_LookupRelocCsOffs_Is96P(sctx, cp+ 4))
			{ imv3|=1; imv2|=1; imv1|=1; }

		if(BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+ 4)>=0)
			{ cp+=4; continue; }
		if(BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+ 8)>=0)
			{ imv2|=1; imv3|=1; imv4|=1; imv5|=1; }
		if(BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+12)>=0)
			{ imv3|=1; imv4|=1; imv5|=1; }
		if(BGBCC_JX2_LookupLabelAtOffsNoLLn(sctx, sctx->sec, cp+16)>=0)
			{ imv4|=1; imv5|=1; }

		if(imv0)
		{
			cp+=4;
			nadvl+=32;
			continue;
		}

#if 1
		if(	BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw1) && !(imv0|imv1))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw1, opw0, opw2, opw3, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw1);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw0);
				nswap++;
				continue;
			}
		}
#endif

#if 1
		if(	BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw1) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw2) && 
			!(imv0|imv1|imv2))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw1, opw2, opw0, opw3, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw1);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw2);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 8, opw0);
				nswap++;
				continue;
			}
		}
#endif

#if 1
		if(	BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw1, opw2) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw2) && 
			!(imv0|imv1|imv2))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw2, opw0, opw1, opw3, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw2);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw0);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 8, opw1);
				nswap++;
				continue;
			}
		}
#endif


#if 1
		if(	BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw1) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw2) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw3) && 
			!(imv0|imv1|imv2|imv3))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw1, opw2, opw3, opw0, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw1);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw2);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 8, opw3);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+12, opw0);
				nswap++;
				continue;
			}
		}
#endif

#if 1
		if(	
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw2, opw3) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw1, opw3) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw3) && 
			!(imv0|imv1|imv2|imv3))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw3, opw0, opw1, opw2, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw3);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw0);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 8, opw1);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+12, opw2);
				nswap++;
				continue;
			}
		}
#endif


#if 1
		if(	
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw1) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw1, opw2) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw2) && 
			!(imv0|imv2))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw2, opw1, opw0, opw3, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw2);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw1);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 8, opw0);
				nswap++;
				continue;
			}
		}
#endif


#if 1
		if(	BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw1) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw2) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw1, opw3) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw2, opw3) && 
			BGBCC_JX2_CheckOpsRv_CanSwap(sctx, opw0, opw3) && 
			!(imv0|imv3))
		{
			if(	BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw3, opw1, opw2, opw0, opw4, opw5) <
				BGBCC_JX2_CheckOpsRv_InferInterlockCost(sctx,
					opwn4, opwn3, opwn2, opwn1,
					opw0, opw1, opw2, opw3, opw4, opw5))
			{
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 0, opw3);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 4, opw1);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+ 8, opw2);
				BGBCC_JX2_EmitSetOffsDWord(sctx, cp+12, opw0);
				nswap++;
				continue;
			}
		}
#endif

		cp+=4;
		nadvl+=32;
		continue;
	}
	
	return(nswap>0);
}

ccxl_status BGBCC_JX2_CheckX3Wexify_DoSwaps(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	return(0);
}

ccxl_status BGBCC_JX2_CheckX3Wexify(
	BGBCC_JX2_Context *sctx,
	int spos, int epos)
{
	return(0);
}
