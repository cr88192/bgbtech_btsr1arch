int BJX2_DecodeOpcode_DecF1(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2, u32 jbits)
{
	int rn_dfl, rm_dfl, ro_dfl;
	int disp5, eq, eo;
//	int imm8u, imm8n;
//	int imm10u, imm10n;
	int imm9u, imm9n, imm9us;
	int ret, fnm;

	op->fl|=BJX2_OPFL_TWOWORD;
	op->fl|=BJX2_OPFL_NOWEX;
	op->opn=opw1;
	op->opn2=opw2;
	op->pc=addr;

	if(1)
	{
#ifdef BJX2_FLIPSTNM
		fnm=0;
		if(	((opw2&0xC800)==0x0000)||
			((opw2&0xE800)==0x4000))
				fnm=1;
		if(fnm)
		{
			opw1=(opw1&0xFF00)|
				((opw1<<4)&0x00F0)|
				((opw1>>4)&0x000F);
			opw2=(opw2&0xF9FF)|
				((opw2<<1)&0x0400)|
				((opw2>>1)&0x0200);
		}
#endif
	}


	rn_dfl=(opw1>>4)&15;
	rm_dfl=(opw1   )&15;
//	ro_dfl=(opw1   )&15;
	if(opw2&0x0400)rn_dfl+=16;
	if(opw2&0x0200)rm_dfl+=16;
//	if(opw2&0x0010)ro_dfl+=16;

//	disp5=(opw1   )&31;
	eq=(opw2&0x0800)?1:0;
	eo=(opw2&0x0100)?1:0;

//	imm8u=opw2&255;
//	imm8n=opw2|(~255);
//	imm10u=(opw2&255)|((opw1&0x0060)<<3);
//	imm10n=(opw2&255)|((opw1&0x0060)<<3)|(~1023);

//	imm9u=((opw1&31)<<4)|((opw2>>8)&15);
//	imm9n=((opw1&31)<<4)|((opw2>>8)&15)|((-1)<<9);

	imm9u=(opw2&511);
//	imm9n=(opw2&511)|((-1)<<9);
	imm9n=(opw2&511)|(~511);
	imm9us=imm9u;

	if(jbits)
	{
//		imm9u=(opw2&511)|(jbits<<9);
//		imm9n=(opw2&511)|(jbits<<9)|((~0U)<<31);
//		imm9us=((s32)(imm9u<<1))>>1;

		imm9u=(opw2&255)|(jbits<<8);
		imm9n=(opw2&255)|(jbits<<8)|((~0U)<<31);
		imm9us=(s32)imm9u;
	}

	op->rn=rn_dfl;
	op->rm=rm_dfl;
	op->imm=imm9us;

	ret=0;
	switch((opw2>>12)&15)
	{
	case 0x0:	/* F1ed_0dzz */
		if(eq)
		{
			if(op->rm==0)
			{
				break;
			}
			if(op->rm==1)
				op->rm=BJX2_REG_GBR;
			op->nmid=BJX2_NMID_LEAB;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAB_LdRegDispReg;
			break;
		}
		if(op->rn==1)
			op->rn=BJX2_REG_GBR;			
		op->nmid=BJX2_NMID_MOVB;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVB_RegStRegDisp;
		if(op->rn==0)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVB_RegStPcDisp;
		}
		break;
	case 0x1:	/* F1ed_1dzz */
		if(eq)
		{
			if(op->rm==0)
				break;
			if(op->rm==1)
			{
				if(op->imm&1)break;
				op->imm=(op->imm>>1);
				op->rm=BJX2_REG_GBR;
			}
			op->nmid=BJX2_NMID_LEAW;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAW_LdRegDispReg;
			break;
		}
		if(op->rn==1)
		{
			if(op->imm&1)break;
			op->imm=(op->imm>>1);
			op->rn=BJX2_REG_GBR;
		}
		op->nmid=BJX2_NMID_MOVW;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVW_RegStRegDisp;
		if(op->rn==0)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVW_RegStPcDisp;
		}
		break;
	case 0x2:	/* F1ed_2dzz */
		if(eq)
		{
			if(op->rm==0)
				break;
			if(op->rm==1)
			{
				if(op->imm&3)break;
				op->imm=(op->imm>>2);
				op->rm=BJX2_REG_GBR;
			}
			op->nmid=BJX2_NMID_LEAD;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAD_LdRegDispReg;
			break;
		}
		if(op->rn==1)
		{
			if(op->imm&3)break;
			op->imm=(op->imm>>2);
			op->rn=BJX2_REG_GBR;
		}
		op->nmid=BJX2_NMID_MOVL;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVL_RegStRegDisp;
		if(op->rn==0)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVL_RegStPcDisp;
		}
		break;
	case 0x3:	/* F1ed_3dzz */
		if(eq)
		{
			if(op->rm==0)
				break;
			if(op->rm==1)
			{
				if(op->imm&7)break;
				op->imm=(op->imm>>3);
				op->rm=BJX2_REG_GBR;
			}
			op->nmid=BJX2_NMID_LEAQ;
			op->fmid=BJX2_FMID_LDREGDISPREG;
			op->Run=BJX2_Op_LEAQ_LdRegDispReg;
			break;
		}
		if(op->rn==1)
		{
			if(op->imm&7)break;
			op->imm=(op->imm>>3);
			op->rn=BJX2_REG_GBR;
		}
		op->nmid=BJX2_NMID_MOVQ;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_MOVQ_RegStRegDisp;
		if(op->rn==0)
		{
			op->rn=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVQ_RegStPcDisp;
		}
		break;

	case 0x4:	/* F1nm_4gnm */
		if(op->rn==1)
			op->rn=BJX2_REG_GBR;			
		if(op->rn==0)
			break;
		op->nmid=BJX2_NMID_FMOVS;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_FMOVS_RegStRegDisp;
		if(eq)
		{
			op->nmid=BJX2_NMID_FMOVUS;
			op->Run=BJX2_Op_FMOVS_GRegStRegDisp;
		}
		break;
	case 0x5:	/* F1nm_5gnm */
		if(op->rn==1)
			op->rn=BJX2_REG_GBR;			
		if(op->rn==0)
			break;
		op->nmid=BJX2_NMID_FMOVD;
		op->fmid=BJX2_FMID_REGSTREGDISP;
		op->Run=BJX2_Op_FMOVD_RegStRegDisp;
		break;
	case 0x6:	/* F1nm_6gdd */
		if(op->rm==0)
			break;
		if(op->rm==1)
			op->rm=BJX2_REG_GBR;			
		op->nmid=BJX2_NMID_FMOVS;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_FMOVS_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_FMOVUS;
			op->Run=BJX2_Op_FMOVS_LdRegDispGReg;
		}
		break;
	case 0x7:	/* F1nm_7gdd */
		if(op->rm==0)
			break;
		if(op->rm==1)
			op->rm=BJX2_REG_GBR;			
		op->nmid=BJX2_NMID_FMOVD;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_FMOVD_LdRegDispReg;
		break;

#if 0
	case 0x4:	/* F1ed_4dzz */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
	case 0x5:	/* F1ed_5dzz */
		op->imm=imm9n;
		op->nmid=BJX2_NMID_ADD;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADD_RegImmReg;
		break;
#endif

	case 0x8:	/* F1ed_8dzz */
//		if(op->rm==1)
//			op->rm=BJX2_REG_GBR;			
		if(op->rm==1)
		{
//			op->imm=(op->imm>>1);
			op->rm=BJX2_REG_GBR;
		}

		op->nmid=BJX2_NMID_MOVB;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVB_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUB;
			op->Run=BJX2_Op_MOVUB_LdRegDispReg;
		}
		if(op->rm==0)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVB_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUB;
				op->Run=BJX2_Op_MOVUB_LdPcDispReg;
			}
		}
		break;
	case 0x9:	/* F1ed_9dzz */
		if(op->rm==1)
		{
			if(op->imm&1)break;
			op->imm=(op->imm>>1);
			op->rm=BJX2_REG_GBR;
		}
		op->nmid=BJX2_NMID_MOVW;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVW_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUW;
			op->Run=BJX2_Op_MOVUW_LdRegDispReg;
		}
		if(op->rm==0)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVW_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUW;
				op->Run=BJX2_Op_MOVUW_LdPcDispReg;
			}
		}
		break;
	case 0xA:	/* F1ed_Adzz */
		if(op->rm==1)
		{
			if(op->imm&3)break;
			op->imm=(op->imm>>2);
			op->rm=BJX2_REG_GBR;
		}
		op->nmid=BJX2_NMID_MOVL;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVL_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVUL;
			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
		}
		if(op->rm==0)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVL_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVUL;
				op->Run=BJX2_Op_MOVUL_LdPcDispReg;
			}
		}
		break;
	case 0xB:	/* F1ed_Bdzz */
		if(op->rm==1)
		{
			if(eq)
			{
				if(op->imm&3)break;
				op->imm=(op->imm>>2);
			}else
			{
				if(op->imm&7)break;
				op->imm=(op->imm>>3);
			}
			op->rm=BJX2_REG_GBR;
		}
		op->nmid=BJX2_NMID_MOVQ;
		op->fmid=BJX2_FMID_LDREGDISPREG;
		op->Run=BJX2_Op_MOVQ_LdRegDispReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_MOVDL;
			op->Run=BJX2_Op_MOVUL_LdRegDispReg;
		}
		if(op->rm==0)
		{
			op->rm=BJX2_REG_PC;
			op->Run=BJX2_Op_MOVQ_LdPcDispReg;
			if(eq)
			{
				op->nmid=BJX2_NMID_MOVDL;
				op->Run=BJX2_Op_MOVUL_LdPcDispReg;
			}
		}
		break;

#if 0
	case 0xC:	/* F1ed_Cdzz */
		op->imm=imm9u;
		op->nmid=BJX2_NMID_ADDSL;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADDSL_RegImmReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_ADDUL;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
		}
		break;
	case 0xD:	/* F1ed_Ddzz */
		op->imm=imm9n;
		op->nmid=BJX2_NMID_ADDSL;
		op->fmid=BJX2_FMID_REGIMMREG;
		op->Run=BJX2_Op_ADDSL_RegImmReg;
		if(eq)
		{
			op->nmid=BJX2_NMID_ADDUL;
			op->Run=BJX2_Op_ADDSL_RegImmReg;
		}
		break;
#endif

	default:
		ret=-1;
		break;
	}
	
	return(ret);
}

int BJX2_DecodeOpcode_DecF5(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int opw2b;
	int ret;
	
	if(!(ctx->regs[BJX2_REG_SR]&(1<<27)))
	{
		op1=BJX2_ContextAllocOpcode(ctx);
		op1->pc=addr;

		opw2b=opw2&0xFEFF;
		ret=BJX2_DecodeOpcode_DecF1(ctx, op1, addr, opw1, opw2b, 0);

		if(opw2&0x0100)
		{
			op->nmid=BJX2_NMID_PRED_F;
			op->fmid=BJX2_FMID_CHAIN;
			op->Run=BJX2_Op_PREDF_Chn;
			op->data=op1;
		}else
		{
			op->nmid=BJX2_NMID_PRED_T;
			op->fmid=BJX2_FMID_CHAIN;
			op->Run=BJX2_Op_PREDT_Chn;
			op->data=op1;
		}

		return(ret);
	}
	
	ret=BJX2_DecodeOpcode_DecF1(ctx, op, addr, opw1, opw2, 0);
	return(ret);
}

int BJX2_DecodeOpcode_DecD5(BJX2_Context *ctx,
	BJX2_Opcode *op, bjx2_addr addr, int opw1, int opw2)
{
	BJX2_Opcode *op1;
	int ret;
	
	op->fl|=BJX2_OPFL_TWOWORD;
	op->opn=opw1;
	op->opn2=opw2;

	op1=BJX2_ContextAllocOpcode(ctx);
	op1->pc=addr;

	ret=BJX2_DecodeOpcode_DecF1(ctx, op1, addr, opw1, opw2, 0);

	if(opw1&0x0400)
	{
		op->nmid=BJX2_NMID_PRED_F;
		op->fmid=BJX2_FMID_CHAIN;
		op->Run=BJX2_Op_PREDF_Chn;
		op->data=op1;
	}else
	{
		op->nmid=BJX2_NMID_PRED_T;
		op->fmid=BJX2_FMID_CHAIN;
		op->Run=BJX2_Op_PREDT_Chn;
		op->data=op1;
	}

	return(ret);
}
