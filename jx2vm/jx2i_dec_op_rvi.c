/*
 MIT No Attribution

 Copyright (c) 2018-2024 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person 
 obtaining a copy of this software and associated documentation 
 files (the "Software"), to deal in the Software without 
 restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell 
 copies of the Software, and to permit persons to whom the 
 Software is furnished to do so.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE.
*/

int BJX2_DecodeOpcode_DecRVI(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	BJX2_Opcode *op1;
	int rn_dfl, rm_dfl, ro_dfl;
	int rn_fr, rm_fr, ro_fr, rp_fr;
	s32 opw, opwb, opwc;
	int opw3, opw4, opw5, opw6;
//	int disp5, eq, eo;
//	int imm8u, imm8n;
	int imm5u;
	s64 imm12l, imm12s, imm12b, imm17l;
	int imm20j, imm20u, imm_ro, do_imm_ro, do_imm_ro_b, sub4op;
	int ret, fnm;

	if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
		BJX2_EXTID_RISCV))
	{
		op->Run=NULL;
		return(-1);
	}

	op->fl|=BJX2_OPFL_TWOWORD;
	op->fl|=BJX2_OPFL_RV64;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;
//	op->pc2=addr;
	ret=0;

	if(addr&3)
	{
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;
	}

	opw=(opw2<<16)|((u16)opw1);

#if 1
//	if((opw&0x707F)==0x401B)
	if(((opw&0x707F)==0x401B) || ((opw&0x707F)==0x403F))
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op->data=op1;

		opw3=BJX2_MemGetWord(ctx, addr+4);
		opw4=BJX2_MemGetWord(ctx, addr+6);

		imm20u=	0x02000000U |
				(((opw>>20)&2047)<< 0) |
				(((opw>>15)&  31)<<11) |
				(((opw>> 7)&  31)<<16) |
				(((opw>>31)&   1)<<24);

//		if((opw3&0x707F)==0x401B)
		if(((opw3&0x707F)==0x401B) || ((opw3&0x707F)==0x403F))
		{
			opwb=(opw4<<16)|((u16)opw3);

			imm20u=(imm20u<<1)|((opw2>>15)&1);
			imm20j=
					(((opwb>>20)&2047)<< 0) |
					(((opwb>>15)&  31)<<11) |
					(((opwb>> 7)&  31)<<16) ;

			opw5=BJX2_MemGetWord(ctx, addr+ 8);
			opw6=BJX2_MemGetWord(ctx, addr+10);

			opwc=(opw6<<16)|((u16)opw5);

			imm17l=
				((opwc  &0xFFFFF000ULL)<< 0) |
				((imm20j&0x000007FFULL)<< 0) |
				((opwb  &0x80000000ULL)>>20) |
				((imm20j&0x001FF800ULL)<<21) |
				((imm20u&0x003FFFFFULL)<<42) ;

			op ->pc=addr;
			op1->pc=addr+6;
			
			op ->opn =opw1;
			op ->opn2=opw2;
			op ->opn3=opw3;
			op1->opn =opw4;
			op1->opn2=opw5;
			op1->opn3=opw6;
			
			op->fl&=~BJX2_OPFL_TWOWORD;
			op->fl|=BJX2_OPFL_JUMBO96;
			op->fl|=BJX2_OPFL_NOWEX;

			rn_dfl=(opw5>>7)&31;
			if(rn_dfl<16)
			{
				switch(rn_dfl)
				{
					case 0:		rn_dfl=BJX2_REG_ZZR;	break;
					case 1:		rn_dfl=BJX2_REG_LR;		break;
					case 2:		rn_dfl=BJX2_REG_SP;		break;
					case 3:		rn_dfl=BJX2_REG_GBR;	break;
					case 14:	rn_dfl=BJX2_REG_R2;		break;
					case 15:	rn_dfl=BJX2_REG_R3;		break;
				}
			}
			
			if((opw5&0x007F)==0x37)
			{
				op ->rn=rn_dfl;
				op ->imm=imm17l;

				op->nmid=BJX2_NMID_JLDI;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;

				return(ret);
			}
			
			return(-1);
		}

		ret=BJX2_DecodeOpcode_DecRVI(ctx, op, addr, opw3, opw4, imm20u);

		op ->opn =opw1;
		op ->opn2=opw2;
		op1->opn =opw3;
		op1->opn2=opw4;

		op ->pc=addr;
		op1->pc=addr+4;

		op->fl&=~BJX2_OPFL_TWOWORD;
		op->fl|=BJX2_OPFL_JUMBO64;

		return(ret);
	}
#endif

	rn_dfl=(opw>> 7)&31;
	rm_dfl=(opw>>15)&31;
	ro_dfl=(opw>>20)&31;
	imm20u=(opw)>>12;
	imm12l=(opw)>>20;
	imm12s=(imm12l&(~31))|((opw>>7)&31);

	imm17l=
		(((opw>>20)&2047)<< 0)|
		(((opw>>15)&  31)<<11)|
		((((s32)opw)>>31)<<16);

	rn_fr=32+((opw>> 7)&31);
	rm_fr=32+((opw>>15)&31);
	ro_fr=32+((opw>>20)&31);
	rp_fr=32+((opw>>27)&31);

	imm5u=ro_dfl;

	imm_ro=0;
	do_imm_ro=0;
	do_imm_ro_b=0;
	sub4op=0;

	if(jbits&0x02000000U)
	{
		if(jbits&0x01000000U)
		{
			if(jbits&(1<<18))
				rn_dfl+=32;
			if(jbits&(1<<19))
				rm_dfl+=32;
			if(jbits&(1<<20))
				ro_dfl+=32;
			
			if(jbits&(1<<11))
			{
				imm_ro=((jbits&2047)<<5)|(ro_dfl&31);
				if(jbits&(1<<20))
					imm_ro|=(~0xFFFFLL);
				do_imm_ro=1;
				do_imm_ro_b=1;
			}
			
			sub4op=(jbits>>12)&15;

			rn_fr=rn_dfl;
			rm_fr=rm_dfl;
			ro_fr=ro_dfl;
		}
	}
	
	if(rn_dfl<16)
	{
		switch(rn_dfl)
		{
			case 0:		rn_dfl=BJX2_REG_ZZR;	break;
			case 1:		rn_dfl=BJX2_REG_LR;		break;
			case 2:		rn_dfl=BJX2_REG_SP;		break;
			case 3:		rn_dfl=BJX2_REG_GBR;	break;
//			case 4:		rn_dfl=BJX2_REG_TBR;	break;
//			case 5:		rn_dfl=BJX2_REG_DHR;	break;
			case 14:	rn_dfl=BJX2_REG_R2;		break;
			case 15:	rn_dfl=BJX2_REG_R3;		break;
		}
	}

	if(rm_dfl<16)
	{
		switch(rm_dfl)
		{
			case 0:		rm_dfl=BJX2_REG_ZZR;	break;
			case 1:		rm_dfl=BJX2_REG_LR;		break;
			case 2:		rm_dfl=BJX2_REG_SP;		break;
			case 3:		rm_dfl=BJX2_REG_GBR;	break;
//			case 4:		rm_dfl=BJX2_REG_TBR;	break;
//			case 5:		rm_dfl=BJX2_REG_DHR;	break;
			case 14:	rm_dfl=BJX2_REG_R2;		break;
			case 15:	rm_dfl=BJX2_REG_R3;		break;
		}
	}

	if(ro_dfl<16)
	{
		switch(ro_dfl)
		{
			case 0:		ro_dfl=BJX2_REG_ZZR;	break;
			case 1:		ro_dfl=BJX2_REG_LR;		break;
			case 2:		ro_dfl=BJX2_REG_SP;		break;
			case 3:		ro_dfl=BJX2_REG_GBR;	break;
//			case 4:		ro_dfl=BJX2_REG_TBR;	break;
//			case 5:		ro_dfl=BJX2_REG_DHR;	break;
			case 14:	ro_dfl=BJX2_REG_R2;		break;
			case 15:	ro_dfl=BJX2_REG_R3;		break;
		}
	}

	imm12b =
		((opw>> 8)&0x0000000F) |
		((opw>>21)&0x000003F0) |
		((opw<< 3)&0x00000400) |
		((opw>>20)&0xFFFFF800) ;
	imm20j=
		((opw>>20)&0x000007FE) |
		((opw>> 9)&0x00000800) |
		((opw>> 0)&0x000FF000) |
		((opw>>12)&0xFFF00000) ;

	imm12b=(s32)imm12b;
	imm20j=(s32)imm20j;

#if 1
	if(jbits&0x02000000U)
	{
		if(jbits&0x01000000U)
		{
			imm12l&=(~0x003FF800LL);
			imm12l|=(jbits<<11)&0x003FF800LL;
			
//			if(!do_imm_ro)
			if(!do_imm_ro_b)
//			if(!(jbits&(1<<11)))
			{
				imm12s&=(~0x003FF800LL);
				imm12b&=(~0x003FF800LL);

				imm12s|=(jbits<<11)&0x003FF800LL;
				imm12b|=(jbits<<11)&0x003FF800LL;
			}
		}else
		{
			imm12l&=(~0x0FFFFF800LL);
			imm12s&=(~0x0FFFFF800LL);
			imm12b&=(~0x0FFFFF800LL);
			imm12l|=(jbits<<11)&0x0FFFFF800LL;
			imm12s|=(jbits<<11)&0x0FFFFF800LL;
			imm12b|=(jbits<<11)&0x0FFFFF800LL;

			imm_ro=((jbits<<5)&0x03FFFFE0LL)|
				(imm5u&31);
			if(jbits&(1<<20))
				imm_ro|=0x0FC000000LL;
			do_imm_ro=1;
		}
	}
#endif

	switch((opw>>2)&0x1F)
	{
	case 0x00: /* 00-000, LD */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX3W;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVB_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVW_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 2:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 3:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 4:
			op->nmid=BJX2_NMID_MOVUB;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUB_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 5:
			op->nmid=BJX2_NMID_MOVUW;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUW_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 6:
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 7:
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVX2_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;
		}
		break;

	case 0x08: /* 01-000, ST */
		op->rn=rm_dfl;
		op->rm=ro_dfl;
		op->imm=imm12s;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX3W;
		op->immb=imm_ro;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVB_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVW_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 2:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 3:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVX2_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;
		}

		if(do_imm_ro_b)
		{
			op->rm=BJX2_REG_IMM;
			op->fmid=BJX2_FMID_IMMBSTREGDISP1;
		}
		break;

	case 0x10: /* 10-000, FMADD */
	case 0x11: /* 10-001, FMSUB */
	case 0x12: /* 10-010, FNMSUB */
	case 0x13: /* 10-011, FNMADD */
		op->rn=rn_fr;
		op->rm=rm_fr;
		op->ro=ro_fr;
		op->rp=rp_fr;

		switch((opw>>2)&3)
		{
		case 0:
			switch((opw>>25)&3)
			{
			case 0:
				op->nmid=BJX2_NMID_FMADDS;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FMADDS_RegRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FMADDD;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FMADDD_RegRegRegReg;
				break;
			}
			break;
		case 1:
			switch((opw>>25)&3)
			{
			case 0:
				op->nmid=BJX2_NMID_FMSUBS;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FMSUBS_RegRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FMSUBD;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FMSUBD_RegRegRegReg;
				break;
			}
			break;
		case 2:
			switch((opw>>25)&3)
			{
			case 0:
				op->nmid=BJX2_NMID_FNMSUBS;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FNMSUBS_RegRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FNMSUBD;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FNMSUBD_RegRegRegReg;
				break;
			}
			break;
		case 3:
			switch((opw>>25)&3)
			{
			case 0:
				op->nmid=BJX2_NMID_FNMADDS;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FNMADDS_RegRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FNMADDD;
				op->fmid=BJX2_FMID_REGREGREGREG;
				op->Run=BJX2_Op_FNMADDD_RegRegRegReg;
				break;
			}
			break;
		}
		break;

	case 0x18: /* 11-000, Bcc */
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		op->rn=rm_dfl;
		op->rm=ro_dfl;
		op->imm=imm12b;
		op->immb=imm_ro;
		
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_BREQ;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BREQ_RegRegPc0Disp;
			break;
		case 1:
			op->nmid=BJX2_NMID_BRNE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRNE_RegRegPc0Disp;
			break;

		case 4:
			op->nmid=BJX2_NMID_BRLT;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRLT_RegRegPc0Disp;
			break;
		case 5:
			op->nmid=BJX2_NMID_BRGE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRGE_RegRegPc0Disp;
			break;
		case 6:
			op->nmid=BJX2_NMID_BRBI;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRBI_RegRegPc0Disp;
			break;
		case 7:
			op->nmid=BJX2_NMID_BRHE;
			op->fmid=BJX2_FMID_REGREGPCDISP;
			op->Run=BJX2_Op_BRHE_RegRegPc0Disp;
			break;
		}

		if(do_imm_ro_b)
		{
			op->rm=BJX2_REG_IMM;
			op->fmid=BJX2_FMID_IMMBREGPCDISP;
		}
		break;


	case 0x01: /* 00-001, FPU-LD */
		op->rn=rn_fr;
		op->rm=rm_dfl;
		op->imm=imm12l;
		switch((opw>>12)&7)
		{
		case 2:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			break;
		case 3:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			break;
		}
		break;

	case 0x09: /* 01-001, FPU-ST */
		op->rn=rm_dfl;
		op->rm=ro_fr;
		op->imm=imm12s;

		switch((opw>>12)&7)
		{
		case 2:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			break;
		case 3:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			break;
		}
		break;

	case 0x19: /* 11-001, JALR */
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(rn_dfl==BJX2_REG_ZZR)
		{
			op->rn=rm_dfl;
			op->imm=imm12l;
			op->nmid=BJX2_NMID_BRA;
			op->fmid=BJX2_FMID_LDREGDISP1;
			op->Run=BJX2_Op_BRA_RegDisp1;
			break;
		}

		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;
		op->nmid=BJX2_NMID_BSR;
		op->fmid=BJX2_FMID_LDREGDISP1REG;
		op->Run=BJX2_Op_BSR_RegRegDisp1;
		break;

	case 0x0B: /* 01-011, AMO / Atomic */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
//		op->ro=ro_dfl;
		op->imm=0;

		switch((opw>>27)&31)
		{
		case 0:
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=0;

			op->nmid=BJX2_NMID_AMOADDL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_AMOADDL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;

		case 1:
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->imm=0;

			op->nmid=BJX2_NMID_AMOSWAPL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_AMOLRL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;

		case 2:
			op->rn=rn_dfl;
			op->rm=rm_dfl;
			op->imm=0;

			op->nmid=BJX2_NMID_AMOLRL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_AMOLRL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 3:
			op->rn=rm_dfl;
			op->rm=ro_dfl;
			op->ro=rn_dfl;
			op->imm=0;

			op->nmid=BJX2_NMID_AMOSCL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_AMOSCL_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;

		case 6:
			op->rm=rm_dfl;
			op->ro=ro_dfl;
			op->rn=rn_dfl;
			op->imm=0;

			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVB_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVW_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 2:
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVL_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 3:
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVQ_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 4:
				op->nmid=BJX2_NMID_MOVUB;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVUB_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 5:
				op->nmid=BJX2_NMID_MOVUW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVUW_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 6:
				op->nmid=BJX2_NMID_MOVUL;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVUL_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 7:
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_MOVX2_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
			}
			break;

		case 7:
			op->rm=rn_dfl;
			op->ro=ro_dfl;
			op->rn=rm_dfl;
			op->imm=0;

			switch((opw>>12)&7)
			{
			case 0:
			case 4:
				op->nmid=BJX2_NMID_MOVB;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVB_RegStReg2;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
			case 5:
				op->nmid=BJX2_NMID_MOVW;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVW_RegStReg2;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 2:
			case 6:
				op->nmid=BJX2_NMID_MOVL;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVL_RegStReg2;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 3:
				op->nmid=BJX2_NMID_MOVQ;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVQ_RegStReg2;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 7:
				op->nmid=BJX2_NMID_MOVX2;
				op->fmid=BJX2_FMID_REGSTREG2;
				op->Run=BJX2_Op_MOVX2_RegStReg2;
				op->fl|=BJX2_OPFL_NOWEX;
				op->fl|=BJX2_OPFL_NOWEXSFX;
				break;
			}
			break;
		}
		break;

	case 0x1B: /* 11-011, JAL */
		if(jbits&0x02000000U)
		{
			break;
		}

		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		if(rn_dfl==BJX2_REG_ZZR)
		{
			op->rn=rn_dfl;
			op->imm=imm20j>>1;
			op->nmid=BJX2_NMID_BRA;
//			op->nmid=BJX2_NMID_JAL;
			op->fmid=BJX2_FMID_REGPCDISP;
			op->Run=BJX2_Op_BRA_Pc0Disp;
			break;
		}

		op->rn=rn_dfl;
		op->imm=imm20j>>1;
		op->nmid=BJX2_NMID_BSR;
//		op->nmid=BJX2_NMID_JAL;
		op->fmid=BJX2_FMID_REGPCDISP;
		op->Run=BJX2_Op_BSR_RegPc0Disp;
		break;

	case 0x04: /* 00-100, ALU 3RI */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADD_RegImmReg;
			
			if(rm_dfl==BJX2_REG_ZZR)
			{
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;
			}else
				if(imm12l==0)
			{
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_MOV_RegReg;
			}
			break;

		case 1:
			op->nmid=BJX2_NMID_SHLDQ;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLDQ_RegImmReg;
			break;

		case 2:
			op->nmid=BJX2_NMID_SLT;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SLTI_RegImmReg;
			
			if(rn_dfl==BJX2_REG_ZZR)
			{
				op->rn=rm_dfl;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_CMPQLT_ImmReg;
			}
			break;
		case 3:
			op->nmid=BJX2_NMID_SLTU;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SLTIU_RegImmReg;
			if(rn_dfl==BJX2_REG_ZZR)
			{
				op->rn=rm_dfl;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_CMPQLTU_ImmReg;
			}
			break;

		case 4:
			op->nmid=BJX2_NMID_XOR;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_XOR_RegImmReg;
			break;

		case 5:
			op->imm=-(imm12l&63);
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SHADQ;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHADQ_RegImmReg;
			}else
			{
				op->nmid=BJX2_NMID_SHLDQ;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHLDQ_RegImmReg;
			}
			break;

		case 6:
			op->nmid=BJX2_NMID_OR;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_OR_RegImmReg;
			break;
		case 7:
			op->nmid=BJX2_NMID_AND;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_AND_RegImmReg;
			break;
		}
		break;

	case 0x0C: /* 01-100, ALU 3R */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;

		switch((opw>>12)&7)
		{
		case 0:
			switch((opw>>25)&127)
			{
			case 0:
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_ADD_RegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_MULSQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULSQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 32:
				op->nmid=BJX2_NMID_SUB;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SUB_RegRegReg;
				break;
			}
			break;

		case 1:
			switch((opw>>25)&127)
			{
			case 0:
				op->nmid=BJX2_NMID_SHLDQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLDQ_RegRegReg;
				break;
			}
			break;

		case 2:
			switch((opw>>25)&127)
			{
			case 0:
				op->nmid=BJX2_NMID_SLT;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SLT_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				
				switch(sub4op)
				{
				case 1:
					op->nmid=BJX2_NMID_CMPQEQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_SEQ_RegRegReg;
					break;
				case 2:
					op->nmid=BJX2_NMID_CMPQGE;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_SGE_RegRegReg;
					break;
				case 3:
					op->nmid=BJX2_NMID_CMPQNE;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_SNE_RegRegReg;
					break;

				case 5:
					op->nmid=BJX2_NMID_TSTNQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_TSTNQ_RegRegReg;
					break;
				case 7:
					op->nmid=BJX2_NMID_TSTQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_TSTQ_RegRegReg;
					break;
				}
				
				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=ro_dfl;
					op->rm=rm_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_CMPQGT_RegReg;
				}
				break;
			case 1:
				op->nmid=BJX2_NMID_MULSHQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULSHQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x10:
				op->rm=ro_dfl;
				op->ro=rm_dfl;

				op->nmid=BJX2_NMID_LEAW;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAW_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x20:
				op->nmid=BJX2_NMID_CMPQGE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SGE_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x40:
				op->nmid=BJX2_NMID_CMPQEQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SEQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x60:
				op->nmid=BJX2_NMID_CMPQNE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SNE_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;

		case 3:
			switch((opw>>25)&127)
			{
			case 0:
				op->nmid=BJX2_NMID_SLTU;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SLTU_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->rn=ro_dfl;
					op->rm=rm_dfl;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_CMPQHI_RegReg;
				}
				break;
			case 1:
				op->nmid=BJX2_NMID_MULUHQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULUHQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x20:
				op->nmid=BJX2_NMID_CMPQHS;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SGEU_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x40:
				op->nmid=BJX2_NMID_TSTNQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_TSTNQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x60:
				op->nmid=BJX2_NMID_TSTQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_TSTQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			}
			break;


		case 4:
			switch((opw>>25)&127)
			{
			case 0x00:
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_XOR_RegRegReg;
				break;
			case 0x01:
				op->nmid=BJX2_NMID_DIVSQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_DIVSQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x04:
				op->nmid=BJX2_NMID_MOVLD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVLD_RegRegReg;
				break;

			case 0x10:
				op->rm=ro_dfl;
				op->ro=rm_dfl;

				op->nmid=BJX2_NMID_LEAD;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAD_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;

			case 0x24:
				op->nmid=BJX2_NMID_MOVHD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVHD_RegRegReg;
				break;
			}
			break;

		case 5:
			switch((opw>>25)&127)
			{
			case 0:
				op->imm=-(imm12l&63);
				op->nmid=BJX2_NMID_SHLRQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLRQ_RegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_DIVUQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_DIVUQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 32:
				op->imm=-(imm12l&63);
				op->nmid=BJX2_NMID_SHARQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHARQ_RegRegReg;
				break;
			}
			break;

		case 6:
			switch((opw>>25)&127)
			{
			case 0:
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_OR_RegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_MODSQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MODSQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x10:
				op->rm=ro_dfl;
				op->ro=rm_dfl;

				op->nmid=BJX2_NMID_LEAQ;
				op->fmid=BJX2_FMID_LDREG2REG;
				op->Run=BJX2_Op_LEAQ_LdReg2Reg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		case 7:
			switch((opw>>25)&127)
			{
			case 0:
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_AND_RegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_MODUQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MODUQ_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		}
		break;

	case 0x14: /* 10-100, FPU */
		op->rn=rn_fr;
		op->rm=rm_fr;
		op->ro=ro_fr;
		op->imm=0;

		switch((opw>>25)&127)
		{
		case 0x00:
			op->nmid=BJX2_NMID_PADDF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x01:
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			break;
		case 0x02:
			op->nmid=BJX2_NMID_PADDH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDH_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0x04:
			op->nmid=BJX2_NMID_PSUBF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x05:
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			break;
		case 0x06:
			op->nmid=BJX2_NMID_PSUBH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBH_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0x08:
			op->nmid=BJX2_NMID_PMULF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULF_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x09:
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			break;
		case 0x0A:
			op->nmid=BJX2_NMID_PMULH;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULH_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0x0C:
			op->nmid=BJX2_NMID_FDIVS;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FDIVS_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x0D:
			op->nmid=BJX2_NMID_FDIV;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FDIVD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			break;

		case 0x10:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FSGNJS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJS_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_FSGNJNS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJNS_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 2:
				op->nmid=BJX2_NMID_FSGNJXS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJXS_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		case 0x11:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FSGNJ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJ_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_FSGNJN;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJN_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 2:
				op->nmid=BJX2_NMID_FSGNJX;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJX_GRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;

		case 0x14:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FMINS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMINS_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_FMAXS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMAXS_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		case 0x15:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FMIN;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMIN_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_FMAX;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMAX_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;

		case 0x20:
			op->nmid=BJX2_NMID_FSTCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCF_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			
			switch((opw>>20)&31)
			{
			case 1:
				op->nmid=BJX2_NMID_FSTCF;
				op->Run=BJX2_Op_FSTCF_GRegReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_PLDCH;
				op->Run=BJX2_Op_PLDCH_RegReg;
				break;

			case 10:
				op->nmid=BJX2_NMID_PLDCHH;
				op->Run=BJX2_Op_PLDCHH_RegReg;
				break;
			
			default:
				op->Run=NULL;
				break;
			}
			break;
		case 0x21:
			op->nmid=BJX2_NMID_FLDCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FLDCF_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			switch((opw>>20)&31)
			{
			case 0:
				op->nmid=BJX2_NMID_FLDCF;
				op->Run=BJX2_Op_FLDCF_GRegReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_FLDCH;
				op->Run=BJX2_Op_FLDCH_GRegReg;
				break;
			default:
				op->Run=NULL;
				break;
			}
			break;

		case 0x22:
			op->nmid=BJX2_NMID_FSTCH;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCH_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			switch((opw>>20)&31)
			{
			case 0:
				op->nmid=BJX2_NMID_PSTCH;
				op->Run=BJX2_Op_PSTCH_RegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FSTCH;
				op->Run=BJX2_Op_FSTCH_GRegReg;
				break;
//			case 2:
//				op->nmid=BJX2_NMID_FLDCH;
//				op->Run=BJX2_Op_FLDCH_GRegReg;
//				break;
			default:
				op->Run=NULL;
				break;				
			}
			break;

		case 0x50:
			op->rm=ro_fr;
			op->ro=rm_fr;
			op->rn=rn_dfl;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_PCMPGEF;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPSGE_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_PCMPGTF;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPSGT_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 2:
				op->nmid=BJX2_NMID_PCMPEQF;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPSEQ_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		case 0x51:
			op->rm=ro_fr;
			op->ro=rm_fr;
			op->rn=rn_dfl;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FCMPGE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPGE_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 1:
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPGT_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 2:
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPEQ_GRegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;

		case 0x54:
//			op->nmid=BJX2_NMID_PSQRTAF;
//			op->fmid=BJX2_FMID_REGREG;
//			op->Run=BJX2_Op_PSQRTAF_RegReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x55:
			op->nmid=BJX2_NMID_FSQRT;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSQRT_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0x60:
			op->rn=rn_dfl;
			op->rm=rm_fr;
			op->nmid=BJX2_NMID_FSTCIS;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCIS_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
#if 0
			switch((opw>>20)&31)
			{
			case 0:
				op->Run=BJX2_Op_FSTCISL_GRegReg;
				break;
			case 1:
				op->Run=BJX2_Op_FSTCISUL_GRegReg;
				break;
			case 2:
				op->Run=BJX2_Op_FSTCIS_GRegReg;
				break;
			case 3:
				op->Run=NULL;
				break;
			default:
				op->Run=NULL;
				break;
			}
#endif
			break;

		case 0x61:
			op->rn=rn_dfl;
			op->rm=rm_fr;
			op->nmid=BJX2_NMID_FSTCI;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCI_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
#if 0
			switch((opw>>20)&31)
			{
			case 0:
				op->Run=BJX2_Op_FSTCIL_GRegReg;
				break;
			case 1:
				op->Run=BJX2_Op_FSTCIUL_GRegReg;
				break;
			case 2:
				op->Run=BJX2_Op_FSTCI_GRegReg;
				break;
			case 3:
				op->Run=NULL;
				break;
			default:
				op->Run=NULL;
				break;
			}
#endif
			break;


		case 0x68:
			op->rn=rn_fr;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_FLDCIS;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FLDCIS_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

#if 1
			switch((opw>>20)&31)
			{
			case 0:
				op->Run=BJX2_Op_FLDCISL_GRegReg;
				break;
			case 1:
				op->Run=BJX2_Op_FLDCISUL_GRegReg;
				break;
			case 2:
				op->Run=BJX2_Op_FLDCIS_GRegReg;
				break;
			case 3:
				op->Run=NULL;
				break;
			default:
				op->Run=NULL;
				break;
			}
#endif
			break;

		case 0x69:
			op->rn=rn_fr;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_FLDCI;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FLDCI_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;

			switch((opw>>20)&31)
			{
			case 0:
				op->Run=BJX2_Op_FLDCIL_GRegReg;
				break;
			case 1:
				op->Run=BJX2_Op_FLDCIUL_GRegReg;
				break;
			case 2:
				op->Run=BJX2_Op_FLDCI_GRegReg;
				break;
			case 3:
				op->Run=NULL;
				break;
			default:
				op->Run=NULL;
				break;
			}
			break;

		case 0x70:
			op->rn=rn_dfl;
			op->rm=rm_fr;
			op->nmid=BJX2_NMID_EXTSL;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_EXTSL_RegReg;
			break;

		case 0x78:
			op->rn=rn_fr;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_EXTSL;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_EXTSL_RegReg;
			break;

		case 0x71:
			op->rn=rn_dfl;
			op->rm=rm_fr;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;

		case 0x79:
			op->rn=rn_fr;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_MOV_RegReg;
			break;
		}
		break;

#if 0
	case 0x14: /* 10-100 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		
		switch(opw>>25)
		{
		case 0x00:
			op->nmid=BJX2_NMID_PADDF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PADDF_RegRegReg;
			break;
		case 0x01:
			op->nmid=BJX2_NMID_FADD;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FADDD_RegRegReg;
			break;

		case 0x04:
			op->nmid=BJX2_NMID_PSUBF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PSUBF_RegRegReg;
			break;
		case 0x05:
			op->nmid=BJX2_NMID_FSUB;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FSUBD_RegRegReg;
			break;

		case 0x08:
			op->nmid=BJX2_NMID_PMULF;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_PMULF_RegRegReg;
			break;
		case 0x09:
			op->nmid=BJX2_NMID_FMUL;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FMULD_RegRegReg;
			break;

		case 0x20:
			op->nmid=BJX2_NMID_FLDCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FLDCF_GRegReg;
			break;
		case 0x21:
			op->nmid=BJX2_NMID_FSTCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCF_GRegReg;
			break;
		}
		break;
#endif

	case 0x1C: /* 11-100, SYSTEM */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=imm12l;
		
		switch((opw>>12)&7)
		{
		case 0:
			op->fmid=BJX2_FMID_Z;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			switch((opw>>20)&0xFFF)
			{
			case 0x000:
				op->nmid=BJX2_NMID_SYSCALL;
				op->Run=BJX2_Op_SYSCALL_None;
				break;
			case 0x001:
				op->nmid=BJX2_NMID_BREAK;
				op->Run=BJX2_Op_BREAK_None;
				break;
			case 0x002:
			case 0x102:
			case 0x302:
				op->nmid=BJX2_NMID_RTE;
				op->Run=BJX2_Op_RTE_None;
				break;
			case 0x105:
				op->nmid=BJX2_NMID_SLEEP;
				op->Run=BJX2_Op_SLEEP_None;
				break;
			}
			break;

		case 1:
			op->nmid=BJX2_NMID_CSRRW;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRW_RegImmReg;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;

		case 2:
			op->nmid=BJX2_NMID_CSRRS;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRS_RegImmReg;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;

		case 3:
			op->nmid=BJX2_NMID_CSRRC;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRC_RegImmReg;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;

		case 5:
			rm_dfl=(opw>>15)&31;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_CSRRWI;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRWI_RegImmReg;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;

		case 6:
			rm_dfl=(opw>>15)&31;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_CSRRSI;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRSI_RegImmReg;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;

		case 7:
			rm_dfl=(opw>>15)&31;
			op->rm=rm_dfl;
			op->nmid=BJX2_NMID_CSRRCI;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRCI_RegImmReg;
			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;
			break;
		}
		break;

	case 0x05: /* 00-101, AUIPC */
		if(jbits&0x02000000U)
		{
			break;
		}
		op->rn=rn_dfl;
//		op->imm=addr+(imm20u<<12)+4;
//		op->imm=addr+(imm20u<<12)+0;
		op->imm=addr+(s32)(imm20u<<12)+0;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;

	case 0x0D: /* 01-101, LUI */
		if(jbits&0x02000000U)
		{
			break;
		}
		op->rn=rn_dfl;
//		op->imm=imm20u<<12;
		op->imm=(s32)(imm20u<<12);
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;

	case 0x15: /* 10-101, OP-V */
		break;

	case 0x1D: /* 11-101, OP-P */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=imm12l;
		
		switch((opw>>12)&7)
		{
		case 0:
			switch((opw>>25)&127)
			{
			default:
				break;
			}
			break;
		case 1:
			switch((opw>>25)&127)
			{
			case 0x07:
				op->nmid=BJX2_NMID_MOVLW;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVLW_RegRegReg;
				break;
			case 0x0F:
//				op->nmid=BJX2_NMID_MOVLHW;
//				op->fmid=BJX2_FMID_REGREGREG;
//				op->Run=BJX2_Op_MOVLHW_RegRegReg;
				break;
			case 0x17:
//				op->nmid=BJX2_NMID_MOVHLW;
//				op->fmid=BJX2_FMID_REGREGREG;
//				op->Run=BJX2_Op_MOVHLW_RegRegReg;
				break;
			case 0x1F:
				op->nmid=BJX2_NMID_MOVHW;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVHW_RegRegReg;
				break;
			default:
				break;
			}
			break;
		case 2:
			switch((opw>>25)&127)
			{
			case 0x07:
				op->nmid=BJX2_NMID_MOVLD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVLD_RegRegReg;
				break;
			case 0x0F:
				op->nmid=BJX2_NMID_MOVLHD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVLHD_RegRegReg;
				break;
			case 0x17:
				op->nmid=BJX2_NMID_MOVHLD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVHLD_RegRegReg;
				break;
			case 0x1F:
				op->nmid=BJX2_NMID_MOVHD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MOVHD_RegRegReg;
				break;
			default:
				break;
			}
			break;
		}
		break;


	case 0x06: /* 00-110, OP-32 3RI */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADDSL;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
			break;

		case 1:
			op->nmid=BJX2_NMID_SHLD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SHLD_RegImmReg;
			break;

		case 5:
			op->imm=-(imm12l&63);
			if((opw>>30)&1)
			{
				op->nmid=BJX2_NMID_SHAD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHAD_RegImmReg;
			}else
			{
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_SHLD_RegImmReg;
			}
			break;
		case 6:
			if(imm17l>=0)
			{
				if(jbits&0x02000000U)
				{
					if(jbits&0x01000000U)
					{
						op->imm=imm12l;
						op->nmid=BJX2_NMID_LDISH16;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_LDISH16_RegImmReg;
						break;
					}
					op->imm=imm12l;
					op->nmid=BJX2_NMID_LDISH32;
//					op->fmid=BJX2_FMID_IMMREG;
//					op->Run=BJX2_Op_LDISH32_ImmReg;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_LDISH32_RegImmReg;
					break;
				}

				op->imm=imm17l;
				op->nmid=BJX2_NMID_LDISH16;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_LDISH16_ImmReg;
				if(rn_dfl==BJX2_REG_ZZR)
				{
					op->nmid=BJX2_NMID_BREAK;
					op->Run=BJX2_Op_BREAK_None;
					op->fl|=BJX2_OPFL_CTRLF;
				}
				break;
			}
			break;
		case 7:
			op->imm=imm17l;
			op->nmid=BJX2_NMID_MOV;
			op->fmid=BJX2_FMID_IMMREG;
			op->Run=BJX2_Op_MOV_ImmReg;
			break;
		}
		break;


	case 0x0E: /* 01-110, OP-32 3R */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;
		op->imm=imm_ro;

#if 1
		switch((opw>>12)&7)
		{
		case 0:
			switch((opw>>25)&127)
			{
			case 0x00:
			case 0x20:
				if(do_imm_ro)
				{
					if((opw>>30)&1)
						break;
					op->nmid=BJX2_NMID_ADDSL;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_ADDSL_RegImmReg;
					break;
				}
				
				op->nmid=BJX2_NMID_ADDSL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_ADDSL_RegRegReg;
				if((opw>>30)&1)
				{
					op->nmid=BJX2_NMID_SUBSL;
					op->Run=BJX2_Op_SUBSL_RegRegReg;
				}
				break;
			
			case 0x01:
				op->nmid=BJX2_NMID_MULS;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULS_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(do_imm_ro)
				{
					op->imm=imm_ro;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_MULS_RegImmReg;
				}
				break;
			
			case 0x04:	case 0x05:
			case 0x24:	case 0x25:
				op->nmid=BJX2_NMID_ADDUL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_ADDUL_RegRegReg;
				
				if((ro_dfl==BJX2_REG_ZZR) && !do_imm_ro)
				{
					op->nmid=BJX2_NMID_EXTUL;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_EXTUL_RegReg;
				}
				
				if(do_imm_ro)
				{
					op->imm=imm_ro;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_ADDUL_RegImmReg;
				}

				if((opw>>30)&1)
				{
					op->nmid=BJX2_NMID_SUBUL;
					op->Run=BJX2_Op_SUBUL_RegRegReg;

					if(do_imm_ro)
					{
						op->imm=imm_ro;
						op->fmid=BJX2_FMID_REGIMMREG;
						op->Run=BJX2_Op_SUBUL_RegImmReg;
					}
				}
				break;
			}
			break;

		case 1:
			switch((opw>>25)&127)
			{
			case 0x00:
				op->nmid=BJX2_NMID_SHLD;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLD_RegRegReg;
				break;
			}
			break;

		case 2:
			switch((opw>>25)&127)
			{
			case 0x01:
				op->nmid=BJX2_NMID_MULSL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULSL_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(do_imm_ro)
				{
					op->imm=imm_ro;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_MULSL_RegImmReg;
				}
				break;
			}
			break;

		case 3:
			switch((opw>>25)&127)
			{
			case 0x01:
				op->nmid=BJX2_NMID_MULUL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULUL_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				if(do_imm_ro)
				{
					op->imm=imm_ro;
					op->fmid=BJX2_FMID_REGIMMREG;
					op->Run=BJX2_Op_MULUL_RegImmReg;
				}
				break;
			}
			break;

		case 4:
			switch((opw>>25)&127)
			{
			case 0x00:
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_XOR_RegRegReg;
				break;
			case 0x01:
				op->nmid=BJX2_NMID_DIVSL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_DIVSL_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;

		case 5:
			switch((opw>>25)&127)
			{
			case 0x00:
				op->imm=-(imm12l&63);
				op->nmid=BJX2_NMID_SHLR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLR_RegRegReg;
				break;
			case 0x01:
				op->nmid=BJX2_NMID_DIVUL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_DIVUL_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			case 0x20:
				op->imm=-(imm12l&63);
				op->nmid=BJX2_NMID_SHAR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHAR_RegRegReg;
				break;
			}
			break;

		case 6:
			switch((opw>>25)&127)
			{
			case 0x00:
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_OR_RegRegReg;
				break;
			case 0x01:
				op->nmid=BJX2_NMID_MODSL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MODSL_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		case 7:
			switch((opw>>25)&127)
			{
			case 0x00:
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_AND_RegRegReg;
				break;
			case 0x01:
				op->nmid=BJX2_NMID_MODUL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MODUL_RegRegReg;
				op->fl|=BJX2_OPFL_NOWEX;
				break;
			}
			break;
		}
#endif

		break;

	default:
		ret=-1;
		break;
	}

	return(ret);
}


int BJX2_DecodeOpcode_DecRVL(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, int opw3)
{
	BJX2_Opcode *op1;
	int rn_dfl, rm_dfl, ro_dfl;
	s64 imm32lc;
	int ret, fnm;

	if(!BJX2_DecodeOpcode_CheckExtEnabled(ctx,
		BJX2_EXTID_RISCV))
	{
		op->Run=NULL;
		return(-1);
	}

	op->fl|=BJX2_OPFL_TRIWORD;
	op->fl|=BJX2_OPFL_RV64;
	op->opn=opw1;
	op->opn2=opw2;
	op->opn3=opw3;
	op->pc=addr;
//	op->pc2=addr;
	ret=0;

	op->fl|=BJX2_OPFL_NOWEX;
	op->fl|=BJX2_OPFL_NOWEXSFX;

	imm32lc=(s32)(opw2 | (opw3<<16));

	rn_dfl=(opw1>>7)&31;

	if(rn_dfl<16)
	{
		switch(rn_dfl)
		{
			case 0:		rn_dfl=BJX2_REG_ZZR;	break;
			case 1:		rn_dfl=BJX2_REG_LR;		break;
			case 2:		rn_dfl=BJX2_REG_SP;		break;
			case 3:		rn_dfl=BJX2_REG_GBR;	break;
			case 14:	rn_dfl=BJX2_REG_R2;		break;
			case 15:	rn_dfl=BJX2_REG_R3;		break;
		}
	}
	
	op->rn=rn_dfl;
	op->rm=rn_dfl;

	switch((opw1>>12)&15)
	{
	case 0:
		op->imm=imm32lc;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;
	case 8:
		op->imm=imm32lc;
		op->nmid=BJX2_NMID_LDISH32;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_LDISH32_ImmReg;
		break;
	}


//	opw=(opw2<<16)|((u16)opw1);

	return(ret);
}
