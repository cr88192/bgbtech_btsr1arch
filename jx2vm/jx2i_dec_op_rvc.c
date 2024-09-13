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

int BJX2_DecodeOpcode_DecRVC(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1)
{
	int rs1r, rs2r;
	int rs1p, rs2p;
	int imm_ofs6_lwld;
	int imm_ofs6_qwld;
	int imm_ofs6_lwst;
	int imm_ofs6_qwst;
	int imm_imm6_alu;
	int imm_imm6_a16sp;
	int imm_imm8_i4add;
	int imm_imm8_bcc;

	int imm_ofs4_lwld;
	int imm_ofs4_qwld;
	int imm_ofs4_lwst;
	int imm_ofs4_qwst;

	int ret;

	op->fl|=BJX2_OPFL_RV64;
	op->opn=opw1;
	op->pc=addr;

	rs1r=(opw1>>7)&31;
	rs2r=(opw1>>2)&31;
	rs1p=8+((opw1>>7)&7);
	rs2p=8+((opw1>>2)&7);
	ret=0;

	if(rs1r<16)
	{
		switch(rs1r)
		{
			case 0:		rs1r=BJX2_REG_ZZR;		break;
			case 1:		rs1r=BJX2_REG_LR;		break;
			case 2:		rs1r=BJX2_REG_SP;		break;
			case 3:		rs1r=BJX2_REG_GBR;		break;
//			case 4:		rs1r=BJX2_REG_TBR;		break;
//			case 5:		rs1r=BJX2_REG_DHR;		break;
			case 14:	rs1r=BJX2_REG_R2;		break;
			case 15:	rs1r=BJX2_REG_R3;		break;
		}
	}

	if(rs2r<16)
	{
		switch(rs2r)
		{
			case 0:		rs2r=BJX2_REG_ZZR;		break;
			case 1:		rs2r=BJX2_REG_LR;		break;
			case 2:		rs2r=BJX2_REG_SP;		break;
			case 3:		rs2r=BJX2_REG_GBR;		break;
//			case 4:		rs2r=BJX2_REG_TBR;		break;
//			case 5:		rs2r=BJX2_REG_DHR;		break;
			case 14:	rs2r=BJX2_REG_R2;		break;
			case 15:	rs2r=BJX2_REG_R3;		break;
		}
	}

	switch(rs1p)
	{
		case 14:	rs1p=BJX2_REG_R2;		break;
		case 15:	rs1p=BJX2_REG_R3;		break;
	}

	switch(rs2p)
	{
		case 14:	rs2p=BJX2_REG_R2;		break;
		case 15:	rs2p=BJX2_REG_R3;		break;
	}

	imm_ofs4_lwld=
		((opw1<<1)&0x040)|
		((opw1>>4)&0x004)|
		((opw1>>7)&0x038);
	imm_ofs4_qwld=
		((opw1<<1)&0x0C0)|
		((opw1>>7)&0x038);
	imm_ofs4_lwst=imm_ofs4_lwld;
	imm_ofs4_qwst=imm_ofs4_qwld;

	imm_ofs6_lwld=
		((opw1>>1)&0x01C)|
		((opw1<<4)&0x0C0)|
		((opw1>>7)&0x020);
	imm_ofs6_qwld=
		((opw1>>1)&0x018)|
		((opw1<<4)&0x1C0)|
		((opw1>>7)&0x020);

	imm_ofs6_lwst=
		((opw1>>1)&0x0C0)|
		((opw1>>7)&0x03C);
	imm_ofs6_qwst=
		((opw1>>1)&0x1C0)|
		((opw1>>7)&0x038);
	
	imm_imm8_i4add=
		((opw1>>2)&0x008) |
		((opw1>>4)&0x004) |
		((opw1>>1)&0x3C0) |
		((opw1>>7)&0x030) ;
	
	imm_imm6_alu=
		((opw1>>2)&0x01F) |
		((opw1>>7)&0x020) ;
	
	imm_imm6_a16sp=
		((opw1<<3)&0x020) |
		((opw1<<4)&0x180) |
		((opw1<<1)&0x040) |
		((opw1>>2)&0x010) ;

	imm_imm8_bcc=
		((opw1<<3)&0x020) |
		((opw1>>2)&0x006) |
		((opw1<<1)&0x0C0) |
		((opw1>>7)&0x018) ;

	if(opw1&0x1000)
	{
		imm_imm6_alu|=(~63);
		imm_imm6_a16sp|=(~511);
		imm_imm8_bcc|=(~255);
	}
	
	switch(opw1&3)
	{
	case 0:
		switch((opw1>>13)&7)
		{
		case 0:
			if(imm_imm8_i4add!=0)
			{
				op->rm=BJX2_REG_SP;
				op->rn=rs2p;
				op->imm=imm_imm8_i4add;
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_ADD_RegImmReg;
				break;
			}else
			{
				op->nmid=BJX2_NMID_BREAK;
				op->fmid=BJX2_FMID_Z;
				op->Run=BJX2_Op_BREAK_None;
				op->fl|=BJX2_OPFL_CTRLF;
			}
			break;
		case 1:
			op->rn=32+rs2p;
			op->rm=rs1p;
			op->imm=imm_ofs4_qwld;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 2:
			op->rn=rs2p;
			op->rm=rs1p;
			op->imm=imm_ofs4_lwld;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 3:
			op->rn=rs2p;
			op->rm=rs1p;
			op->imm=imm_ofs4_qwld;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;

		case 5:
			op->rm=32+rs2p;
			op->rn=rs1p;
			op->imm=imm_ofs4_qwst;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 6:
			op->rm=rs2p;
			op->rn=rs1p;
			op->imm=imm_ofs4_lwst;

			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 7:
			op->rm=rs2p;
			op->rn=rs1p;
			op->imm=imm_ofs4_qwst;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		}
		break;
	case 1:
		switch((opw1>>13)&7)
		{
		case 0:
			if(rs1r!=0)
			{
				op->rm=rs1r;
				op->rn=rs1r;
				op->imm=imm_imm6_alu;
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_ADD_RegImmReg;
				break;
			}else
			{
				op->nmid=BJX2_NMID_NOP;
				op->Run=BJX2_Op_NOP_None;
			}
			break;
		case 1:
			if(rs1r!=0)
			{
				op->rm=rs1r;
				op->rn=rs1r;
				op->imm=imm_imm6_alu;
				op->nmid=BJX2_NMID_ADDSL;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_ADDSL_RegImmReg;
				break;
			}else
			{
				op->nmid=BJX2_NMID_NOP;
				op->Run=BJX2_Op_NOP_None;
			}
			break;
		case 2:
			if(rs1r!=0)
			{
				op->rn=rs1r;
				op->imm=imm_imm6_alu;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;
				break;
			}else
			{
				op->nmid=BJX2_NMID_NOP;
				op->Run=BJX2_Op_NOP_None;
			}
			break;
		case 3:
			if((rs1r!=0) && (rs1r!=BJX2_REG_SP))
			{
				op->rn=rs1r;
				op->imm=imm_imm6_alu<<12;
				op->nmid=BJX2_NMID_MOV;
				op->fmid=BJX2_FMID_IMMREG;
				op->Run=BJX2_Op_MOV_ImmReg;
				break;
			}else
				if(rs1r!=0)
			{
#if 1
				op->rm=BJX2_REG_SP;
				op->rn=BJX2_REG_SP;
				op->imm=imm_imm6_a16sp;
				op->nmid=BJX2_NMID_ADD;
				op->fmid=BJX2_FMID_REGIMMREG;
				op->Run=BJX2_Op_ADD_RegImmReg;
#endif
				break;
			}else
			{
				op->nmid=BJX2_NMID_NOP;
				op->Run=BJX2_Op_NOP_None;
			}
			break;
		case 6:
			op->rn=rs1p;
			op->rm=BJX2_REG_ZZR;
			op->imm=(imm_imm8_bcc>>1)-1;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			op->nmid=BJX2_NMID_BREQ;
			op->fmid=BJX2_FMID_REGPCDISP;
			op->Run=BJX2_Op_BREQ_RegRegPcDisp;
			break;
		case 8:
			op->rn=rs1p;
			op->rm=BJX2_REG_ZZR;
			op->imm=(imm_imm8_bcc>>1)-1;

			op->fl|=BJX2_OPFL_CTRLF;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX;

			op->nmid=BJX2_NMID_BRNE;
			op->fmid=BJX2_FMID_REGPCDISP;
			op->Run=BJX2_Op_BRNE_RegRegPcDisp;
			break;
		}
		break;
	case 2:
		op->rn=rs1r;
		op->rm=BJX2_REG_SP;
//		op->imm=imm12l;
		op->fl|=BJX2_OPFL_NOWEX;
		op->fl|=BJX2_OPFL_NOWEXSFX3W;

		switch((opw1>>13)&7)
		{
		case 0:
			break;
		case 1:
			op->rn=32+rs1r;
			op->imm=imm_ofs6_qwld;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 2:
			op->rn=rs1r;
			op->rm=BJX2_REG_SP;
			op->imm=imm_ofs6_lwld;
			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVL_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 3:
			op->rn=rs1r;
			op->rm=BJX2_REG_SP;
			op->imm=imm_ofs6_qwld;
			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_LDREGDISP1REG;
			op->Run=BJX2_Op_MOVQ_LdRegDisp1Reg;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 4:
			if(opw1&(1<<12))
			{
				if((rs1r!=BJX2_REG_ZZR) && (rs2r!=BJX2_REG_ZZR))
				{
					op->rm=rs2r;
					op->rn=rs1r;

					op->nmid=BJX2_NMID_ADD;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_ADD_RegReg;
				}else if(rs1r!=0)
				{
					op->rn=rs1r;

					op->nmid=BJX2_NMID_BSR;
					op->fmid=BJX2_FMID_REG;
					op->Run=BJX2_Op_BSR_Reg;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX|BJX2_OPFL_NOWEXSFX;
				}else
				{
					op->nmid=BJX2_NMID_SYSCALL;
					op->fmid=BJX2_FMID_Z;
					op->Run=BJX2_Op_SYSCALL_None;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX;
					op->fl|=BJX2_OPFL_NOWEXSFX;
				}
			}else
			{
				if((rs1r!=BJX2_REG_ZZR) && (rs2r!=BJX2_REG_ZZR))
				{
					op->rm=rs2r;
					op->rn=rs1r;

					op->nmid=BJX2_NMID_MOV;
					op->fmid=BJX2_FMID_REGREG;
					op->Run=BJX2_Op_MOV_RegReg;
				}else if(rs1r!=0)
				{
					op->rn=rs1r;

					op->nmid=BJX2_NMID_BRA;
					op->fmid=BJX2_FMID_REG;
					op->Run=BJX2_Op_BRA_Reg;
					op->fl|=BJX2_OPFL_CTRLF;
					op->fl|=BJX2_OPFL_NOWEX|BJX2_OPFL_NOWEXSFX;
				}else
				{
					op->nmid=BJX2_NMID_NOP;
					op->Run=BJX2_Op_NOP_None;
				}
			}
			break;
		case 5:
			op->rm=32+rs2r;
			op->rn=BJX2_REG_SP;
			op->imm=imm_ofs6_qwst;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 6:
			op->rm=rs2r;
			op->rn=BJX2_REG_SP;
			op->imm=imm_ofs6_lwst;

			op->nmid=BJX2_NMID_MOVL;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVL_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		case 7:
			op->rm=rs2r;
			op->rn=BJX2_REG_SP;
			op->imm=imm_ofs6_qwst;

			op->nmid=BJX2_NMID_MOVQ;
			op->fmid=BJX2_FMID_REGSTREGDISP1;
			op->Run=BJX2_Op_MOVQ_RegStRegDisp1;
			op->fl|=BJX2_OPFL_NOWEX;
			op->fl|=BJX2_OPFL_NOWEXSFX3W;
			break;
		}
		break;
	default:
		ret=-1;
		break;
	}

	return(ret);
}
