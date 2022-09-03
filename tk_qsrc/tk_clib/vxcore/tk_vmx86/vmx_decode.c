int VMX86_Decode_DecodeOpcode(VMX86_Context *ctx, VMX86_Opcode *op, u64 addr)
{
	u64 addrc;
	byte opb, fmid, bwlq, rb;
	u16 opfx, opfxa, nmid;
	
	addrc=addr;
	opfx=0;

	opb=VMX86_Mem_GetByte(ctx, addrc);

	/* Prefix Bytes */
	while(1)
	{
		opfxa=0;
		switch(opb)
		{
			case 0x66:	opfxa=VMX86_OPFX_DATASZ;
			case 0x67:	opfxa=VMX86_OPFX_ADDRSZ;
			case 0xF2:	opfxa=VMX86_OPFX_REPNE;
			case 0xF3:	opfxa=VMX86_OPFX_REPE;
			case 0xF0:	opfxa=VMX86_OPFX_LOCK;
			case 0x0F:	opfxa=VMX86_OPFX_OP2;
			case 0x64:	opfxa=VMX86_OPFX_FS;
			case 0x65:	opfxa=VMX86_OPFX_GS;
			case 0x2E:	opfxa=VMX86_OPFX_CS;
			case 0x36:	opfxa=VMX86_OPFX_SS;
			case 0x3E:	opfxa=VMX86_OPFX_DS;
			case 0x26:	opfxa=VMX86_OPFX_ES;
			default:	opfxa=0;
		}
	
		if(opfxa)
		{
			opfx|=opfxa;
			addrc++;
			opb=VMX86_Mem_GetByte(ctx, addrc);
			continue;
		}
		break;
	}

	/* REX Prefix */
	if(((opb>>4)==0x4) && (ctx->is_x64))
	{
		op->rex=opb;
		addrc++;
		opb=VMX86_Mem_GetByte(ctx, addrc);
	}
	
	nmid=0;
	fmid=0;
	
	if(opfx&VMX86_OPFX_DATASZ)
	{
		bwlq=1;
	}else
	{
		if(op->rex&3)
			bwlq=3;
		else
			bwlq=2;
	}
	
	if(!(opfx&VMX86_OPFX_OP2))
	{
		switch(opb)
		{
		case 0x00:
			nmid=VMX86_NMID_ADD;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x01:
			nmid=VMX86_NMID_ADD;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x02:
			nmid=VMX86_NMID_ADD;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x03:
			nmid=VMX86_NMID_ADD;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x04:
			nmid=VMX86_NMID_ADD;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x05:
			nmid=VMX86_NMID_ADD;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x08:
			nmid=VMX86_NMID_OR;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x09:
			nmid=VMX86_NMID_OR;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x0A:
			nmid=VMX86_NMID_OR;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x0B:
			nmid=VMX86_NMID_OR;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x0C:
			nmid=VMX86_NMID_OR;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x0D:
			nmid=VMX86_NMID_OR;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x10:
			nmid=VMX86_NMID_ADC;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x11:
			nmid=VMX86_NMID_ADC;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x12:
			nmid=VMX86_NMID_ADC;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x13:
			nmid=VMX86_NMID_ADC;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x14:
			nmid=VMX86_NMID_ADC;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x15:
			nmid=VMX86_NMID_ADC;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x18:
			nmid=VMX86_NMID_SBB;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x19:
			nmid=VMX86_NMID_SBB;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x1A:
			nmid=VMX86_NMID_SBB;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x1B:
			nmid=VMX86_NMID_SBB;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x1C:
			nmid=VMX86_NMID_SBB;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x1D:
			nmid=VMX86_NMID_SBB;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x20:
			nmid=VMX86_NMID_AND;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x21:
			nmid=VMX86_NMID_AND;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x22:
			nmid=VMX86_NMID_AND;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x23:
			nmid=VMX86_NMID_AND;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x24:
			nmid=VMX86_NMID_AND;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x25:
			nmid=VMX86_NMID_AND;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x28:
			nmid=VMX86_NMID_SUB;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x29:
			nmid=VMX86_NMID_SUB;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x2A:
			nmid=VMX86_NMID_SUB;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x2B:
			nmid=VMX86_NMID_SUB;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x2C:
			nmid=VMX86_NMID_SUB;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x2D:
			nmid=VMX86_NMID_SUB;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x30:
			nmid=VMX86_NMID_XOR;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x31:
			nmid=VMX86_NMID_XOR;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x32:
			nmid=VMX86_NMID_XOR;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x33:
			nmid=VMX86_NMID_XOR;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x34:
			nmid=VMX86_NMID_XOR;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x35:
			nmid=VMX86_NMID_XOR;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x38:
			nmid=VMX86_NMID_CMP;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x39:
			nmid=VMX86_NMID_CMP;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x3A:
			nmid=VMX86_NMID_CMP;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x3B:
			nmid=VMX86_NMID_CMP;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;
		case 0x3C:
			nmid=VMX86_NMID_CMP;
			fmid=VMX86_FMID_ALI_B;
			break;
		case 0x3D:
			nmid=VMX86_NMID_CMP;
			fmid=VMX86_FMID_ALI_B+bwlq;
			break;

		case 0x40:	case 0x41:
		case 0x42:	case 0x43:
		case 0x44:	case 0x45:
		case 0x46:	case 0x47:
			nmid=VMX86_NMID_INC;
			fmid=VMX86_FMID_OPREG_B+bwlq;
			break;
		case 0x48:	case 0x49:
		case 0x4A:	case 0x4B:
		case 0x4C:	case 0x4D:
		case 0x4E:	case 0x4F:
			nmid=VMX86_NMID_DEC;
			fmid=VMX86_FMID_OPREG_B+bwlq;
			break;
		case 0x50:	case 0x51:
		case 0x52:	case 0x53:
		case 0x54:	case 0x55:
		case 0x56:	case 0x57:
			nmid=VMX86_NMID_PUSH;
			fmid=VMX86_FMID_OPREG_B+bwlq;
			break;
		case 0x58:	case 0x59:
		case 0x5A:	case 0x5B:
		case 0x5C:	case 0x5D:
		case 0x5E:	case 0x5F:
			nmid=VMX86_NMID_POP;
			fmid=VMX86_FMID_OPREG_B+bwlq;
			break;

		case 0x84:
			nmid=VMX86_NMID_TEST;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x85:
			nmid=VMX86_NMID_TEST;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x86:
			nmid=VMX86_NMID_XCHG;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x87:
			nmid=VMX86_NMID_XCHG;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;

		case 0x88:
			nmid=VMX86_NMID_MOV;
			fmid=VMX86_FMID_RMREG_B;
			break;
		case 0x89:
			nmid=VMX86_NMID_MOV;
			fmid=VMX86_FMID_RMREG_B+bwlq;
			break;
		case 0x8A:
			nmid=VMX86_NMID_MOV;
			fmid=VMX86_FMID_REGRM_B;
			break;
		case 0x8B:
			nmid=VMX86_NMID_MOV;
			fmid=VMX86_FMID_REGRM_B+bwlq;
			break;

		case 0xB0:	case 0xB1:
		case 0xB2:	case 0xB3:
		case 0xB4:	case 0xB5:
		case 0xB6:	case 0xB7:
			nmid=VMX86_NMID_MOV;
			fmid=VMX86_FMID_OPREGIMM_B;
			break;
		case 0xB8:	case 0xB9:
		case 0xBA:	case 0xBB:
		case 0xBC:	case 0xBD:
		case 0xBE:	case 0xBF:
			nmid=VMX86_NMID_MOV;
			fmid=VMX86_FMID_OPREGIMM_B+bwlq;
			break;

		}
	}else
	{
		switch(opb)
		{
		default:
			break;
		}
	}
	
	rb=((opb>>0)&7)|((op->rex&1)<<4);

	switch(fmid)
	{
	case VMX86_FMID_ALI_B:
	case VMX86_FMID_ALI_W:
	case VMX86_FMID_ALI_L:
	case VMX86_FMID_ALI_Q:
		op->rb=VMX86_REGS_ZZR;
		op->ri=0;
		op->rn=0;
		break;
	
	case VMX86_FMID_OPREG_B:
	case VMX86_FMID_OPREG_W:
	case VMX86_FMID_OPREG_L:
	case VMX86_FMID_OPREG_Q:
		op->rb=VMX86_REGS_ZZR;
		op->ri=rb;
		op->rn=rb;
		break;

	case VMX86_FMID_OPREGIMM_B:
	case VMX86_FMID_OPREGIMM_W:
	case VMX86_FMID_OPREGIMM_L:
	case VMX86_FMID_OPREGIMM_Q:
		op->rb=VMX86_REGS_ZZR;
		op->ri=rb;
		op->rn=rb;
		break;
	}
	
	op->opfx=opfx;
	op->nmid=nmid;
	op->fmid=fmid;
	
	return((addrc-addr)+1);
}

int VMX86_Decode_DecodeModRm(VMX86_Context *ctx, VMX86_Opcode *op, u64 addr)
{
	byte mod, sib;
	byte rn, rb, rbs, ris;

	mod=VMX86_Mem_GetByte(ctx, addr);
	sib=VMX86_Mem_GetByte(ctx, addr+1);
	
	rn=((mod>>3)&7)|((op->rex&4)<<2);
	rb=((mod>>0)&7)|((op->rex&1)<<4);
	rbs=((sib>>0)&7)|((op->rex&1)<<4);
	ris=((sib>>3)&7)|((op->rex&2)<<3);
	
	if((mod>>6)==3)
	{
		op->rn=rn;
		op->rb=VMX86_REGS_ZZR;
		op->ri=rb;
		op->sc=0;
		op->disp=0;
		return(1);
	}

	if(((sib>>3)&7)==4)
		ris=VMX86_REGS_ZZR;

	if((mod>>6)==0)
	{
		if((mod&6)==4)
		{
			if(mod&1)
			{
				op->rn=rn;
				op->rb=VMX86_REGS_ZZR;
				op->ri=VMX86_REGS_ZZR;
				if(ctx->is_x64)
					op->rb=VMX86_REGS_RIP;
				op->sc=0;
				op->disp=VMX86_Mem_GetDWord(ctx, addr+1);
				return(5);
			}else
			{
				op->rn=rn;
				op->rb=rbs;
				op->ri=ris;
				op->sc=sib>>6;
				op->disp=0;
				return(2);
			}
		}else
		{
			op->rn=rn;
			op->rb=rb;
			op->ri=VMX86_REGS_ZZR;
			op->sc=0;
			op->disp=0;
			return(1);
		}
	}

	if((mod>>6)==1)
	{
		if((mod&7)==4)
		{
			op->rn=rn;
			op->rb=rbs;
			op->ri=ris;
			op->sc=sib>>6;
			op->disp=VMX86_Mem_GetByte(ctx, addr+2);
			return(3);
		}else
		{
			op->rn=rn;
			op->rb=rb;
			op->ri=VMX86_REGS_ZZR;
			op->sc=0;
			op->disp=sib;
			return(2);
		}
	}

	if((mod>>6)==2)
	{
		if((mod&7)==4)
		{
			op->rn=rn;
			op->rb=rbs;
			op->ri=ris;
			op->sc=sib>>6;
			op->disp=VMX86_Mem_GetDWord(ctx, addr+2);
			return(6);
		}else
		{
			op->rn=rn;
			op->rb=rb;
			op->ri=VMX86_REGS_ZZR;
			op->sc=0;
			op->disp=VMX86_Mem_GetDWord(ctx, addr+1);
			return(5);
		}
	}
	
	return(1);
}

VMX86_Opcode *VMX86_Decode_OpForAddress(VMX86_Context *ctx, u64 addr)
{
	VMX86_Opcode *op;
	byte opreg;
	int sz0, sz1, sz2;

	op=VMX86_Decode_AllocOpcode(ctx);

	sz0=VMX86_Decode_DecodeOpcode(ctx, op, addr);

	switch(op->fmid)
	{
	case VMX86_FMID_OPREG_B:
	case VMX86_FMID_OPREG_W:
	case VMX86_FMID_OPREG_L:
	case VMX86_FMID_OPREG_Q:
	case VMX86_FMID_ALI_B:
	case VMX86_FMID_ALI_W:
	case VMX86_FMID_ALI_L:
	case VMX86_FMID_ALI_Q:
	case VMX86_FMID_OPREGIMM_B:
	case VMX86_FMID_OPREGIMM_W:
	case VMX86_FMID_OPREGIMM_L:
	case VMX86_FMID_OPREGIMM_Q:
		sz1=0;
		break;

	case VMX86_FMID_REGRM_B:
	case VMX86_FMID_REGRM_W:
	case VMX86_FMID_REGRM_L:
	case VMX86_FMID_REGRM_Q:
		sz1=VMX86_Decode_DecodeModRm(ctx, op, addr+sz0);
		break;
	case VMX86_FMID_RMREG_B:
	case VMX86_FMID_RMREG_W:
	case VMX86_FMID_RMREG_L:
	case VMX86_FMID_RMREG_Q:
		sz1=VMX86_Decode_DecodeModRm(ctx, op, addr+sz0);
		break;

	case VMX86_FMID_RMIMM_B:
	case VMX86_FMID_RMIMM_W:
	case VMX86_FMID_RMIMM_L:
	case VMX86_FMID_RMIMM_Q:
		sz1=VMX86_Decode_DecodeModRm(ctx, op, addr+sz0);
		break;

	default:
		sz1=0;
		break;
	}

	switch(op->fmid)
	{
	case VMX86_FMID_ALI_B:
	case VMX86_FMID_OPREGIMM_B:
	case VMX86_FMID_RMIMM_B:
		op->imm=VMX86_Mem_GetByte(ctx, addr+sz0+sz1);
		sz2=1;
		break;
	case VMX86_FMID_ALI_W:
	case VMX86_FMID_OPREGIMM_W:
	case VMX86_FMID_RMIMM_W:
		op->imm=VMX86_Mem_GetWord(ctx, addr+sz0+sz1);
		sz2=2;
		break;
	case VMX86_FMID_ALI_L:
	case VMX86_FMID_OPREGIMM_L:
	case VMX86_FMID_RMIMM_L:
		op->imm=VMX86_Mem_GetDWord(ctx, addr+sz0+sz1);
		sz2=4;
		break;
	case VMX86_FMID_ALI_Q:
	case VMX86_FMID_OPREGIMM_Q:
	case VMX86_FMID_RMIMM_Q:
		op->imm=VMX86_Mem_GetQWord(ctx, addr+sz0+sz1);
		sz2=8;
		break;

	default:
		sz2=0;
		break;
	}

	switch(op->fmid)
	{
	case VMX86_FMID_OPREG_B:
	case VMX86_FMID_ALI_B:
	case VMX86_FMID_OPREGIMM_B:
		break;

	case VMX86_FMID_OPREG_W:
	case VMX86_FMID_ALI_W:
	case VMX86_FMID_OPREGIMM_W:
		break;

	case VMX86_FMID_OPREG_L:
	case VMX86_FMID_ALI_L:
	case VMX86_FMID_OPREGIMM_L:
		break;

	case VMX86_FMID_OPREG_Q:
	case VMX86_FMID_ALI_Q:
	case VMX86_FMID_OPREGIMM_Q:
		break;

	case VMX86_FMID_REGRM_B:
	case VMX86_FMID_RMREG_B:
	case VMX86_FMID_RMIMM_B:
		op->GetMem=VMX86_Mem_GetMemValueGenByte;
		op->SetMem=VMX86_Mem_SetMemValueGenByte;
		break;

	case VMX86_FMID_REGRM_W:
	case VMX86_FMID_RMREG_W:
	case VMX86_FMID_RMIMM_W:
		op->GetMem=VMX86_Mem_GetMemValueGenWord;
		op->SetMem=VMX86_Mem_SetMemValueGenWord;
		break;

	case VMX86_FMID_REGRM_L:
	case VMX86_FMID_RMREG_L:
	case VMX86_FMID_RMIMM_L:
		op->GetMem=VMX86_Mem_GetMemValueGenDWord;
		op->SetMem=VMX86_Mem_SetMemValueGenDWord;
		
		if(op->rb==VMX86_REGS_ZZR)
		{
			op->GetMem=VMX86_Mem_GetMemValueRegDWord;
			op->SetMem=VMX86_Mem_SetMemValueRegDWord;
		}

		break;

	case VMX86_FMID_REGRM_Q:
	case VMX86_FMID_RMREG_Q:
	case VMX86_FMID_RMIMM_Q:
		op->GetMem=VMX86_Mem_GetMemValueGenQWord;
		op->SetMem=VMX86_Mem_SetMemValueGenQWord;

		if(op->rb==VMX86_REGS_ZZR)
		{
			op->GetMem=VMX86_Mem_GetMemValueRegQWord;
			op->SetMem=VMX86_Mem_SetMemValueRegQWord;
		}

		break;
	}

	op->sz=sz0+sz1+sz2;
	
	return(op);
}
