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

int BJX2_DecodeOpcode_DecRVI(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int rn_fr, rm_fr, ro_fr;
	s32 opw;
	int opw3, opw4;
	int disp5, eq, eo;
	int imm8u, imm8n;
	int imm12l, imm12s, imm12b;
	int imm20j, imm20u;
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
	ret=0;

	opw=(opw2<<16)|((u16)opw1);

	rn_dfl=(opw>> 7)&31;
	rm_dfl=(opw>>15)&31;
	ro_dfl=(opw>>20)&31;
	imm20u=(opw)>>12;
	imm12l=(opw)>>20;
	imm12s=(imm12l&(~31))|((opw>>7)&31);

	rn_fr=32+((opw>> 7)&31);
	rm_fr=32+((opw>>15)&31);
	ro_fr=32+((opw>>20)&31);
	
	if(rn_dfl<16)
	{
		switch(rn_dfl)
		{
			case 0:		rn_dfl=BJX2_REG_ZZR;	break;
			case 1:		rn_dfl=BJX2_REG_LR;		break;
			case 2:		rn_dfl=BJX2_REG_SP;		break;
			case 3:		rn_dfl=BJX2_REG_GBR;	break;
			case 4:		rn_dfl=BJX2_REG_TBR;	break;
			case 5:		rn_dfl=BJX2_REG_DHR;	break;
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
			case 4:		rm_dfl=BJX2_REG_TBR;	break;
			case 5:		rm_dfl=BJX2_REG_DHR;	break;
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
			case 4:		ro_dfl=BJX2_REG_TBR;	break;
			case 5:		ro_dfl=BJX2_REG_DHR;	break;
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

	switch((opw>>2)&0x1F)
	{
	case 0x00: /* 00-000 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVB_LdRegDisp1Reg;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVW_LdRegDisp1Reg;
			break;
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
		case 4:
			op->nmid=BJX2_NMID_MOVUB;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUB_LdRegDisp1Reg;
			break;
		case 5:
			op->nmid=BJX2_NMID_MOVUW;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUW_LdRegDisp1Reg;
			break;
		case 6:
			op->nmid=BJX2_NMID_MOVUL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVUL_LdRegDisp1Reg;
			break;
		case 7:
			op->nmid=BJX2_NMID_MOVX2;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVX2_LdRegDisp1Reg;
			break;
		}
		break;

	case 0x08: /* 01-000 */
		op->rn=rm_dfl;
		op->rm=ro_dfl;
		op->imm=imm12s;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVB;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVB_RegStRegDisp1;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVW;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVW_RegStRegDisp1;
			break;
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
			break;
		}
		break;

	case 0x18: /* 11-000 */
		op->fl|=BJX2_OPFL_CTRLF;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX;

		op->rn=rm_dfl;
		op->rm=ro_dfl;
		op->imm=imm12b;
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
		break;


	case 0x01: /* 00-001 */
		op->rn=rn_fr;
		op->rm=rm_dfl;
		op->imm=imm12l;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			break;
		}
		break;

	case 0x09: /* 01-001 */
		op->rn=rm_dfl;
		op->rm=ro_fr;
		op->imm=imm12s;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			break;
		case 1:
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			break;
		}
		break;

	case 0x19: /* 11-001 */
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

	case 0x1B: /* 11-011 */
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

	case 0x04: /* 00-100 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->imm=imm12l;

		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=BJX2_NMID_ADD;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_ADD_RegImmReg;
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
			break;
		case 3:
			op->nmid=BJX2_NMID_SLTU;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_SLTIU_RegImmReg;
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

	case 0x0C: /* 01-100 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;

//		if(((opw>>25)&31)==0)
		if(1)
		{
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
					break;
				case 1:
					op->nmid=BJX2_NMID_MULSHQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_MULSHQ_RegRegReg;
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
					break;
				case 1:
					op->nmid=BJX2_NMID_MULUHQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_MULUHQ_RegRegReg;
					break;
				}
				break;


			case 4:
				switch((opw>>25)&127)
				{
				case 0:
					op->nmid=BJX2_NMID_XOR;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_XOR_RegRegReg;
					break;
				case 1:
					op->nmid=BJX2_NMID_DIVSQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_DIVSQ_RegRegReg;
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
					break;
				}
				break;
			}
			break;
		}
		break;

	case 0x14: /* 10-100 */
		op->rn=rn_fr;
		op->rm=rm_fr;
		op->ro=ro_fr;
//		op->imm=imm12l;

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
		case 0x0C:
//			op->nmid=BJX2_NMID_FDIV;
//			op->fmid=BJX2_FMID_REGREGREG;
//			op->Run=BJX2_Op_PMULF_RegRegReg;
//			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x0D:
			op->nmid=BJX2_NMID_FDIV;
			op->fmid=BJX2_FMID_REGREGREG;
			op->Run=BJX2_Op_FDIVD_RegRegReg;
			op->fl|=BJX2_OPFL_NOWEX_FP2;
			break;

		case 0x20:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FSGNJS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJS_GRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FSGNJNS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJNS_GRegReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_FSGNJXS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJXS_GRegReg;
				break;
			}
			break;
		case 0x21:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FSGNJ;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJ_GRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FSGNJN;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJN_GRegReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_FSGNJX;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FSGNJX_GRegReg;
				break;
			}
			break;

		case 0x24:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FMINS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMINS_GRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FMAXS;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMAXS_GRegRegReg;
				break;
			}
			break;
		case 0x25:
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FMIN;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMIN_GRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FMAX;
				op->fmid=BJX2_FMID_REGREG;
				op->Run=BJX2_Op_FMAX_GRegRegReg;
				break;
			}
			break;

		case 0x40:
			op->nmid=BJX2_NMID_FLDCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FLDCF_GRegReg;
			break;
		case 0x41:
			op->nmid=BJX2_NMID_FSTCF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSTCF_GRegReg;
			break;

		case 0x50:
			op->rm=ro_fr;
			op->ro=rm_fr;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FCMPGE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPSGE_GRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPSGT_GRegRegReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPSEQ_GRegRegReg;
				break;
			}
			break;
		case 0x51:
			op->rm=ro_fr;
			op->ro=rm_fr;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_FCMPGE;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPGE_GRegRegReg;
				break;
			case 1:
				op->nmid=BJX2_NMID_FCMPGT;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPGT_GRegRegReg;
				break;
			case 2:
				op->nmid=BJX2_NMID_FCMPEQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_FCMPEQ_GRegRegReg;
				break;
			}
			break;

		case 0x54:
			op->nmid=BJX2_NMID_PSQRTAF;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_PSQRTAF_RegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;
		case 0x55:
			op->nmid=BJX2_NMID_FSQRT;
			op->fmid=BJX2_FMID_REGREG;
			op->Run=BJX2_Op_FSQRT_GRegReg;
			op->fl|=BJX2_OPFL_NOWEX;
			break;

		case 0x61:
			op->nmid=BJX2_NMID_FLDCI;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FLDCI_GRegReg;
			break;
		case 0x69:
			op->nmid=BJX2_NMID_FSTCI;
			op->fmid=BJX2_FMID_FREGREG;
			op->Run=BJX2_Op_FSTCI_GRegReg;
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

	case 0x1C: /* 11-100 */
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
			break;
		case 2:
			op->nmid=BJX2_NMID_CSRRS;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRS_RegImmReg;
			break;
		case 3:
			op->nmid=BJX2_NMID_CSRRC;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRC_RegImmReg;
			break;

		case 5:
			rm_dfl=(opw>>15)&31;
			op->rm=rm_dfl;

			op->nmid=BJX2_NMID_CSRRWI;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRWI_RegImmReg;
			break;
		case 6:
			rm_dfl=(opw>>15)&31;
			op->rm=rm_dfl;

			op->nmid=BJX2_NMID_CSRRSI;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRSI_RegImmReg;
			break;
		case 7:
			rm_dfl=(opw>>15)&31;
			op->rm=rm_dfl;

			op->nmid=BJX2_NMID_CSRRCI;
			op->fmid=BJX2_FMID_REGIMMREG;
			op->Run=BJX2_Op_CSRRCI_RegImmReg;
			break;
		}
		break;

	case 0x05: /* 00-101 */
		op->rn=rn_dfl;
//		op->imm=addr+(imm20u<<12)+4;
		op->imm=addr+(imm20u<<12)+0;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;

	case 0x0D: /* 01-101 */
		op->rn=rn_dfl;
		op->imm=imm20u<<12;
		op->nmid=BJX2_NMID_MOV;
		op->fmid=BJX2_FMID_IMMREG;
		op->Run=BJX2_Op_MOV_ImmReg;
		break;


	case 0x06: /* 00-110 */
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
		}
		break;

	case 0x0E: /* 01-110 */
		op->rn=rn_dfl;
		op->rm=rm_dfl;
		op->ro=ro_dfl;

		if(((opw>>25)&31)==0)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_ADDSL;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_ADDSL_RegRegReg;
				if((opw>>30)&1)
				{
					op->nmid=BJX2_NMID_SUBSL;
					op->Run=BJX2_Op_SUBSL_RegRegReg;
				}
				break;

			case 1:
				op->nmid=BJX2_NMID_SHLDQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_SHLDQ_RegRegReg;
				break;

			case 4:
				op->nmid=BJX2_NMID_XOR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_XOR_RegRegReg;
				break;

			case 5:
				op->imm=-(imm12l&63);
				if((opw>>30)&1)
				{
					op->nmid=BJX2_NMID_SHARQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_SHARQ_RegRegReg;
				}else
				{
					op->nmid=BJX2_NMID_SHLRQ;
					op->fmid=BJX2_FMID_REGREGREG;
					op->Run=BJX2_Op_SHLRQ_RegRegReg;
				}
				break;

			case 6:
				op->nmid=BJX2_NMID_OR;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_OR_RegRegReg;
				break;
			case 7:
				op->nmid=BJX2_NMID_AND;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_AND_RegRegReg;
				break;
			}
			break;
		}

		if(((opw>>25)&31)==1)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=BJX2_NMID_MULS;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MULS_RegRegReg;
				break;

			case 4:
				op->nmid=BJX2_NMID_DIVSQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_DIVSQ_RegRegReg;
				break;
			case 5:
				op->nmid=BJX2_NMID_DIVUQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_DIVUQ_RegRegReg;
				break;
			case 6:
				op->nmid=BJX2_NMID_MODSQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MODSQ_RegRegReg;
				break;
			case 7:
				op->nmid=BJX2_NMID_MODUQ;
				op->fmid=BJX2_FMID_REGREGREG;
				op->Run=BJX2_Op_MODUQ_RegRegReg;
				break;
			}
		}
		break;

	default:
		ret=-1;
		break;
	}

	return(ret);
}
