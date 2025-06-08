int X3VM_DecodeOpcodeRV(X3VM_Context *ctx,
	X3VM_Opcode *op, u64 addr, u32 opw, u64 jbits)
{
	u64 jb1;
	u32 opw1;
	byte rs_dfl, rt_dfl, rn_dfl, ru_dfl;
	byte rs_fr, rt_fr, rn_fr, ru_fr;
	byte eq, sub4op, do_imm_ro, do_imm_ro_b;
	s64 imm12s, imm12ld, imm12st, imm12br;
	s64 imm17s, disp20j, imm20i, imm_ro;
	
	if((opw&3)!=3)
	{
		__debugbreak();
	}
	
	rn_dfl=(opw>> 7)&31;
	rs_dfl=(opw>>15)&31;
	rt_dfl=(opw>>20)&31;
	ru_dfl=(opw>>27)&31;

	eq=(opw>>5)&1;
	
	rs_fr=rs_dfl+32;
	rt_fr=rt_dfl+32;
	ru_fr=ru_dfl+32;
	rn_fr=rn_dfl+32;
	
	imm12s=((s32)opw)>>20;
	imm12ld=imm12s;
	imm12st=(imm12s&(~31))|rn_dfl;
	imm17s=(imm12s&(~0xF800))|(rs_dfl<<11);

	imm12br=(imm12st&0xFFFFF7FE)|((imm12st&1)<<11);
	imm12br=(s32)imm12br;

	disp20j=
		(imm12s&0xFFF00000) |
		(opw&0x000FF000) |
		(imm12s&0x7FE) |
		((imm12s&1)<<11) ;
	disp20j=(s32)disp20j;

	imm20i=(s32)(opw&(~4095));
	
	imm_ro=0;
	do_imm_ro=0;
	do_imm_ro_b=0;
	
	if(jbits)
	{
		imm20i|=(jbits&2047)|(((jbits>>21)&1)<<11);
		disp20j=imm20i;

		if(jbits>>32)
		{
			return(0);
		}

		if(jbits&0x08000000)
		{
			imm12s=
				(imm12s&0xFFFFFFFFFFC007FFLL) |
				((jbits&0x0007FF)<<11) ;
			imm12ld=imm12s;

			if(jbits&(1<<18))
				rn_dfl+=32;
			if(jbits&(1<<19))
				rs_dfl+=32;
			if(jbits&(1<<20))
				rt_dfl+=32;
			
			if(jbits&(1<<11))
			{
				imm_ro=((jbits&2047)<<5)|(rt_dfl&31);
				if(jbits&(1<<20))
					imm_ro|=(~0xFFFFLL);
				do_imm_ro=1;
				do_imm_ro_b=1;
			}
			
			sub4op=(jbits>>12)&15;

			rn_fr=rn_dfl;
			rs_fr=rs_dfl;
			rt_fr=rt_dfl;

			if(!do_imm_ro_b)
			{
				imm12st=(imm12s&(~31))|(rn_dfl&31);

				imm12br=
					(imm12br&0xFFFFFFFFFF800FFFLL) |
					((jbits&0x0007FF)<<12) ;
			}
		}else
		{
			imm12s=
				(imm12s&0xFFFFFFFF000007FFLL) |
				((jbits&0x1FFFFF)<<11) ;
			imm12ld=imm12s;
			imm12st=(imm12s&(~31))|rn_dfl;
			
			imm12br=
				(imm12br&0xFFFFFFFE00000FFFLL) |
				((jbits&0x1FFFFF)<<12) ;
			
			imm_ro=((jbits<<5)&0x03FFFFE0LL)|
				(rt_dfl&31);
			if(jbits&(1<<20))
				imm_ro|=0x0FC000000LL;
			do_imm_ro=1;
		}
		
		imm17s=imm12s;
	}
	
	op->opw0=opw;
	op->opw1=0;
	op->opw2=0;
	op->addr=addr;
	op->nmid=0;
	op->fmid=0;
	op->rs=rs_dfl;
	op->rt=rt_dfl;
	op->ru=0;
	op->rn=rn_dfl;
	op->sc=0;
	op->opfl=0;
	op->Run=NULL;

	switch((opw>>2)&31)
	{
	case 0x0F:
		opw1=X3VM_MemLoadU32(ctx, addr+4);
		if(!opw1)
			break;
		if((opw&3)!=3)
			break;
		jb1=
			(imm12s&0x7FF) |
			(rs_dfl<<11) | (rn_dfl<<16) |
			(((((s32)opw)>>31)&127)<<21) |
			(1<<28)|(jbits<<32);
		X3VM_DecodeOpcodeRV(ctx, op, addr+4, opw1, jb1);
		op->opw2=op->opw1;
		op->opw1=op->opw0;
		op->opw0=opw;
		op->addr=addr;
		break;

	case 0x00: /* iiiiiii-iiiii-mmmmm-000-nnnnn-00-00011  (Load) */
		op->imm=imm12ld;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_LB;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS8_3RI;
			break;
		case 1:
			op->nmid=X3VM_NMID_LH;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS16_3RI;
			break;
		case 2:
			op->nmid=X3VM_NMID_LW;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS32_3RI;
			break;
		case 3:
			op->nmid=X3VM_NMID_LD;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;
		case 4:
			op->nmid=X3VM_NMID_LBU;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDU8_3RI;
			break;
		case 5:
			op->nmid=X3VM_NMID_LHU;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDU16_3RI;
			break;
		case 6:
			op->nmid=X3VM_NMID_LWU;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDU32_3RI;
			break;
		case 7:
			op->opfl|=X3VM_OPFL_RN_P;
			op->nmid=X3VM_NMID_LDP;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LD64P_3RI;
			break;
		}
		break;
	case 0x08: /* iiiiiii-ttttt-mmmmm-000-iiiii-01-00011  (Store) */
		op->imm=imm12st;
		op->rs=rs_dfl;
		op->rn=rt_dfl;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_SB;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST8_3RI;
			break;
		case 1:
			op->nmid=X3VM_NMID_SH;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST16_3RI;
			break;
		case 2:
			op->nmid=X3VM_NMID_SW;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST32_3RI;
			break;
		case 3:
			op->nmid=X3VM_NMID_SD;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST64_3RI;
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			op->nmid=X3VM_NMID_SDP;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->opfl|=X3VM_OPFL_RN_P;
			op->Run=X3VM_Opc_ST64P_3RI;
			break;
		}
		break;

	case 0x18: /* ddddddd-ttttt-sssss-000-ddddd-11-00011  (Bcc) */
		op->imm=imm12br;
		op->rs=rs_dfl;
		op->rt=rt_dfl;
		op->opfl|=X3VM_OPFL_CTRLF;
		op->fmid=X3VM_FMID_J3RI;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_BEQ;
			op->Run=X3VM_Opc_BEQ_3RI;
			break;
		case 1:
			op->nmid=X3VM_NMID_BNE;
			op->Run=X3VM_Opc_BNE_3RI;
			break;

		case 4:
			op->nmid=X3VM_NMID_BLT;
			op->Run=X3VM_Opc_BLT_3RI;
			break;
		case 5:
			op->nmid=X3VM_NMID_BGE;
			op->Run=X3VM_Opc_BGE_3RI;
			break;
		case 6:
			op->nmid=X3VM_NMID_BNE;
			op->Run=X3VM_Opc_BLTU_3RI;
			break;
		case 7:
			op->nmid=X3VM_NMID_BNE;
			op->Run=X3VM_Opc_BGEU_3RI;
			break;
		}
		break;

	case 0x01: /* iiiiiii-iiiii-mmmmm-zzz-nnnnn-00-00111 (FP LOAD) */
		op->imm=imm12ld;
		op->rn=rn_fr;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_FLH;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS16_3RI;
			break;
		case 1:
			op->nmid=X3VM_NMID_FLW;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS32_3RI;
			break;
		case 2:
			op->nmid=X3VM_NMID_FLD;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;
		case 3:
			op->nmid=X3VM_NMID_LDP;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->opfl|=X3VM_OPFL_RN_P;
			op->Run=X3VM_Opc_LD64P_3RI;
			break;
		}
		break;
	case 0x09: /* iiiiiii-iiiii-mmmmm-zzz-nnnnn-01-00111 (FP STORE) */
		op->imm=imm12st;
		op->rs=rs_dfl;
		op->rn=rt_fr;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_FSH;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST16_3RI;
			break;
		case 1:
			op->nmid=X3VM_NMID_FSW;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST32_3RI;
			break;
		case 2:
			op->nmid=X3VM_NMID_FSD;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST64_3RI;
			break;
		case 3:
			op->nmid=X3VM_NMID_SDP;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->opfl|=X3VM_OPFL_RN_P;
			op->Run=X3VM_Opc_ST64P_3RI;
			break;
		}
		break;

	case 0x19: /* ddddddd-ddddd-mmmmm-000-nnnnn-11-00111  JALR */
		op->imm=imm12ld;
		op->rs=rs_dfl;
		op->rn=rn_dfl;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_JALR;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_JALR_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			break;
		}
		break;

	case 0x0B: /* zzzzzzz-ooooo-mmmmm-zzz-nnnnn-01-01111  (Atomic) */
		if(((opw>>27)&31)==0x06)
		{
			op->fmid=X3VM_FMID_LDRR;
			op->sc=(opw>>25)&3;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_LB;
				op->Run=X3VM_Opc_LDS8_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_LH;
				op->Run=X3VM_Opc_LDS16_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_LW;
				op->Run=X3VM_Opc_LDS32_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_LD;
				op->Run=X3VM_Opc_LDS64_3R;
				break;
			case 4:
				op->nmid=X3VM_NMID_LBU;
				op->Run=X3VM_Opc_LDU8_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_LHU;
				op->Run=X3VM_Opc_LDU16_3R;
				break;
			case 6:
				op->nmid=X3VM_NMID_LWU;
				op->Run=X3VM_Opc_LDU32_3R;
				break;
			case 7:
				op->nmid=X3VM_NMID_LDP;
				op->opfl|=X3VM_OPFL_RN_P;
				op->Run=X3VM_Opc_LD64P_3R;
				break;
			}
			break;
		}
		if(((opw>>27)&31)==0x07)
		{
			op->fmid=X3VM_FMID_STRR;
			op->sc=(opw>>25)&3;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_SB;
				op->Run=X3VM_Opc_ST8_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_SH;
				op->Run=X3VM_Opc_ST16_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_SW;
				op->Run=X3VM_Opc_ST32_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_SD;
				op->Run=X3VM_Opc_ST64_3R;
				break;
			case 4:
//				op->nmid=X3VM_NMID_LBU;
//				op->Run=X3VM_Opc_LDU8_3R;
				break;
			case 5:
//				op->nmid=X3VM_NMID_LHU;
//				op->Run=X3VM_Opc_LDU16_3R;
				break;
			case 6:
//				op->nmid=X3VM_NMID_LWU;
//				op->Run=X3VM_Opc_LDU32_3R;
				break;
			case 7:
				op->opfl|=X3VM_OPFL_RN_P;
				op->nmid=X3VM_NMID_SDP;
				op->Run=X3VM_Opc_ST64P_3R;
				break;
			}
			break;
		}
		break;

	case 0x10: /* uuuuuxx-ttttt-sssss-zzz-nnnnn-10-00011  FMADD */
		op->rs=rs_fr;
		op->rt=rt_fr;
		op->ru=ru_fr;
		op->rn=rn_fr;
		switch((opw>>25)&3)
		{
		case 0:
			op->nmid=X3VM_NMID_FMADDS;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMACF_4R;
			break;
		case 1:
			op->nmid=X3VM_NMID_FMADDD;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMAC_4R;
			break;
		case 2:
			op->nmid=X3VM_NMID_FMADDH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMACH_4R;
			break;
		case 3:
			break;
		}
		break;
	case 0x11: /* uuuuuxx-ttttt-sssss-zzz-nnnnn-10-00111  FMSUB */
		op->rs=rs_fr;
		op->rt=rt_fr;
		op->ru=ru_fr;
		op->rn=rn_fr;
		switch((opw>>25)&3)
		{
		case 0:
			op->nmid=X3VM_NMID_FMSUBS;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMASF_4R;
			break;
		case 1:
			op->nmid=X3VM_NMID_FMSUBD;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMAS_4R;
			break;
		case 2:
			op->nmid=X3VM_NMID_FMSUBH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMASH_4R;
			break;
		case 3:
			break;
		}
		break;
	case 0x12: /* uuuuuxx-ttttt-sssss-zzz-nnnnn-10-01011  FNMSUB */
		op->rs=rs_fr;
		op->rt=rt_fr;
		op->ru=ru_fr;
		op->rn=rn_fr;
		switch((opw>>25)&3)
		{
		case 0:
			op->nmid=X3VM_NMID_FMNSUBS;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMRSF_4R;
			break;
		case 1:
			op->nmid=X3VM_NMID_FMNSUBD;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMRS_4R;
			break;
		case 2:
			op->nmid=X3VM_NMID_FMNSUBH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMRSH_4R;
			break;
		case 3:
			break;
		}
		break;
	case 0x13: /* uuuuuxx-ttttt-sssss-zzz-nnnnn-10-01111  FNMADD */
		op->rs=rs_fr;
		op->rt=rt_fr;
		op->ru=ru_fr;
		op->rn=rn_fr;
		switch((opw>>25)&3)
		{
		case 0:
			op->nmid=X3VM_NMID_FMNADDS;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMRAF_4R;
			break;
		case 1:
			op->nmid=X3VM_NMID_FMNADDD;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMRA_4R;
			break;
		case 2:
			op->nmid=X3VM_NMID_FMNADDH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMRAH_4R;
			break;
		case 3:
			break;
		}
		break;

	case 0x04: /* iiiiiii-iiiii-mmmmm-000-nnnnn-00-10011 ALU IMM */
		op->imm=imm12s;
		op->rs=rs_dfl;
		op->rn=rn_dfl;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_ADD;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_ADD_3RI;
			if(imm12s==0)
				op->Run=X3VM_Opc_MOV_2R;
			break;
		case 1:
			switch((opw>>26)&63)
			{
			case 0x00:
				op->imm=imm12s&63;
				op->nmid=X3VM_NMID_SLL;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLL_3RI;
				break;
			}
			break;
		case 2:
			op->nmid=X3VM_NMID_SLT;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_SLT_3RI;
			break;
		case 3:
			op->nmid=X3VM_NMID_SLTU;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_SLTU_3RI;
			break;
		case 4:
			op->nmid=X3VM_NMID_XOR;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_XOR_3RI;
			break;
		case 5:
			switch((opw>>26)&63)
			{
			case 0x00:
				op->imm=-(imm12s&63);
				op->nmid=X3VM_NMID_SLL;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLL_3RI;
				break;
			case 0x10:
				op->imm=-(imm12s&63);
				op->nmid=X3VM_NMID_SLA;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLA_3RI;
				break;
			}
			break;
		case 6:
			op->nmid=X3VM_NMID_OR;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_OR_3RI;
			break;
		case 7:
			op->nmid=X3VM_NMID_AND;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_AND_3RI;
			break;
		}
		break;

	case 0x0C: /* zzzzzzz-ttttt-mmmmm-zzz-nnnnn-01-10011  ALU 3R */
		op->imm=imm12s;
		op->rs=rs_dfl;
		op->rn=rn_dfl;
		if(((opw>>25)&127)==0x00)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_ADD;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ADD_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_SLL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLL_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_SLT;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLT_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_SLTU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLTU_3R;
				break;
			case 4:
				op->nmid=X3VM_NMID_XOR;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_XOR_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_SRL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SRL_3R;
				break;
			case 6:
				op->nmid=X3VM_NMID_OR;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_OR_3R;
				break;
			case 7:
				op->nmid=X3VM_NMID_AND;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_AND_3R;
				break;
			}
			break;
		}
		if(((opw>>25)&127)==0x20)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_SUB;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SUB_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_SLL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLL_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_SGE;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SGE_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_SGEU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SGEU_3R;
				break;
			case 4:
//				op->nmid=X3VM_NMID_XOR;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_XOR_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_SRA;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SRA_3R;
				break;
			case 6:
//				op->nmid=X3VM_NMID_OR;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_OR_3R;
				break;
			case 7:
//				op->nmid=X3VM_NMID_AND;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_AND_3R;
				break;
			}
			break;
		}

		if(((opw>>25)&127)==0x01)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_MULQ;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULQ_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_MULQH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULQH_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_MULQHSU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULQHSU_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_MULQHU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULQHU_3R;
				break;
			case 4:
				op->nmid=X3VM_NMID_DIV;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DIV_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_DIVU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DIVU_3R;
				break;
			case 6:
				op->nmid=X3VM_NMID_REM;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_REM_3R;
				break;
			case 7:
				op->nmid=X3VM_NMID_REMU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_REMU_3R;
				break;
			}
			break;
		}

		if(((opw>>25)&127)==0x10)
		{
			switch((opw>>12)&7)
			{
			case 0:
//				op->nmid=X3VM_NMID_SUB;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SUB_3R;
				break;
			case 1:
//				op->nmid=X3VM_NMID_SLL;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SLL_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_SH1ADD;
				op->fmid=X3VM_FMID_3R;
				op->sc=1;
				op->Run=X3VM_Opc_SHNADD_3R;
				break;
			case 3:
//				op->nmid=X3VM_NMID_SGEU;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SGEU_3R;
				break;
			case 4:
				op->nmid=X3VM_NMID_SH2ADD;
				op->fmid=X3VM_FMID_3R;
				op->sc=2;
				op->Run=X3VM_Opc_SHNADD_3R;
				break;
			case 5:
//				op->nmid=X3VM_NMID_SRA;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SRA_3R;
				break;
			case 6:
				op->nmid=X3VM_NMID_SH3ADD;
				op->fmid=X3VM_FMID_3R;
				op->sc=3;
				op->Run=X3VM_Opc_SHNADD_3R;
				break;
			case 7:
//				op->nmid=X3VM_NMID_AND;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_AND_3R;
				break;
			}
			break;
		}
		if(((opw>>25)&127)==0x40)
		{
			switch((opw>>12)&7)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				op->nmid=X3VM_NMID_SEQ;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SEQ_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_STST;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_STST_3R;
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			}
			break;
		}
		if(((opw>>25)&127)==0x60)
		{
			switch((opw>>12)&7)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				op->nmid=X3VM_NMID_SNE;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SNE_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_SNTST;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SNTST_3R;
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			}
			break;
		}
		break;



	case 0x06: /* iiiiiii-iiiii-mmmmm-zzz-nnnnn-00-11011  ALUW IMM */
		op->imm=imm12s;
		op->rs=rs_dfl;
		op->rn=rn_dfl;
		switch((opw>>12)&7)
		{
		case 0:
			op->nmid=X3VM_NMID_ADDW;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_ADDW_3RI;
			break;
		case 1:
			switch((opw>>26)&63)
			{
			case 0x00:
				op->imm=imm12s&63;
				op->nmid=X3VM_NMID_SLAW;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLAW_3RI;
				break;
			}
			break;
		case 2:
//			op->nmid=X3VM_NMID_SLT;
//			op->fmid=X3VM_FMID_3RI;
//			op->Run=X3VM_Opc_SLT_3RI;
			break;
		case 3:
//			op->nmid=X3VM_NMID_SLTU;
//			op->fmid=X3VM_FMID_3RI;
//			op->Run=X3VM_Opc_SLTU_3RI;
			break;
		case 4:
//			op->nmid=X3VM_NMID_XOR;
//			op->fmid=X3VM_FMID_3RI;
//			op->Run=X3VM_Opc_XOR_3RI;
			break;
		case 5:
			switch((opw>>26)&63)
			{
			case 0x00:
				op->imm=-(imm12s&63);
				op->nmid=X3VM_NMID_SLLW;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLLW_3RI;
				break;
			case 0x10:
				op->imm=-(imm12s&63);
				op->nmid=X3VM_NMID_SLAW;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLAW_3RI;
				break;
			}
			break;
		case 6:
			op->imm=imm17s;
			op->rs=rn_dfl;
			op->rn=rn_dfl;
			op->fmid=X3VM_FMID_2RI;
			
			if((opw>>31)&1)
			{
				if(jbits)
					break;

				op->rn=rn_fr;
				op->nmid=X3VM_NMID_FLDCH;
				op->Run=X3VM_Opc_FLDCH_2RI;
				break;
			}
			
			if(rn_dfl==0)
			{
				op->nmid=X3VM_NMID_EBREAK;
				op->Run=X3VM_Opc_EBREAK_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
				break;
			}
			
			if(jbits)
			{
				op->imm=imm12s;
				op->rs=rs_dfl;
				op->rn=rn_dfl;
				op->fmid=X3VM_FMID_3RI;
				op->nmid=X3VM_NMID_SHORI32;
				op->Run=X3VM_Opc_SHORI32_3RI;
				break;
			}
			
			op->nmid=X3VM_NMID_SHORI;
			op->Run=X3VM_Opc_SHORI_3RI;
			break;
		case 7:
			if(jbits)
				break;

			op->imm=imm17s;
			op->rs=0;
			op->rn=rn_dfl;
			op->nmid=X3VM_NMID_LI;
			op->fmid=X3VM_FMID_2RI;
//			op->Run=X3VM_Opc_ADD_3RI;
			op->Run=X3VM_Opc_MOV_2RI;
			break;
		}
		break;

	case 0x0E: /* 0000000-ttttt-mmmmm-000-nnnnn-01-11011  ALUW 3R */
		op->imm=imm12s;
		op->rs=rs_dfl;
		op->rn=rn_dfl;
		if(((opw>>25)&127)==0x00)
		{
			switch((opw>>12)&7)
			{
			case 0:
				if(do_imm_ro)
				{
					op->imm=imm_ro;
					op->nmid=X3VM_NMID_ADDW;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_ADDW_3RI;
					break;
				}
				op->nmid=X3VM_NMID_ADDW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ADDW_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_SLAW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLAW_3R;
				break;
			case 2:
//				op->nmid=X3VM_NMID_SLT;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SLT_3R;
				break;
			case 3:
//				op->nmid=X3VM_NMID_SLTU;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SLTU_3R;
				break;
			case 4:
//				op->nmid=X3VM_NMID_XOR;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_XOR_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_SRLW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SRLW_3R;
				break;
			case 6:
//				op->nmid=X3VM_NMID_OR;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_OR_3R;
				break;
			case 7:
//				op->nmid=X3VM_NMID_AND;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_AND_3R;
				break;
			}
			break;
		}
		if(((opw>>25)&127)==0x20)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_SUBW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SUBW_3R;
				break;
				break;
			case 1:
				op->nmid=X3VM_NMID_SLLW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLLW_3R;
				break;
			case 2:
//				op->nmid=X3VM_NMID_SGE;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SGE_3R;
				break;
			case 3:
//				op->nmid=X3VM_NMID_SGEU;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SGEU_3R;
				break;
			case 4:
//				op->nmid=X3VM_NMID_XOR;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_XOR_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_SRAW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SRAW_3R;
				break;
			case 6:
//				op->nmid=X3VM_NMID_OR;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_OR_3R;
				break;
			case 7:
//				op->nmid=X3VM_NMID_AND;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_AND_3R;
				break;
			}
			break;
		}
		if(((opw>>25)&127)==0x01)
		{
			switch((opw>>12)&7)
			{
			case 0:
				if(do_imm_ro)
				{
					op->imm=imm_ro;
					op->nmid=X3VM_NMID_MULW;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_MULW_3RI;
					break;
				}
				op->nmid=X3VM_NMID_MULW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULW_3R;
				break;
			case 1:
//				op->nmid=X3VM_NMID_SLLW;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_SLLW_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_DMULW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DMULW_3R;
				break;
			case 3:
				op->nmid=X3VM_NMID_DMULWU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DMULWU_3R;
				break;
			case 4:
				op->nmid=X3VM_NMID_DIVW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DIVW_3R;
				break;
			case 5:
				op->nmid=X3VM_NMID_DIVUW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DIVUW_3R;
				break;
			case 6:
				op->nmid=X3VM_NMID_REMW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_REMW_3R;
				break;
			case 7:
				op->nmid=X3VM_NMID_REMUW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_REMUW_3R;
				break;
			}
			break;
		}
		if(((opw>>25)&127)==0x04)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_ADDUW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ADDUW_3R;
				break;
			}
		}
		if(((opw>>25)&127)==0x05)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_ADDWU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ADDWU_3R;
				break;
			}
		}
		if(((opw>>25)&127)==0x25)
		{
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_SUBWU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SUBWU_3R;
				break;
			}
		}
		break;

	case 0x14: /* 0000000-ttttt-mmmmm-rrr-nnnnn-10-10011 FPU */
		op->rs=rs_fr;
		op->rt=rt_fr;
		op->ru=0;
		op->rn=rn_fr;

		switch((opw>>25)&127)
		{
		case 0x00:
			op->nmid=X3VM_NMID_PADDF;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PADDF_3R;
			break;
		case 0x01:
			op->nmid=X3VM_NMID_FADD;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FADD_3R;
			break;
		case 0x02:
			op->nmid=X3VM_NMID_PADDH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PADDH_3R;
			break;

		case 0x04:
			op->nmid=X3VM_NMID_PSUBF;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PSUBF_3R;
			break;
		case 0x05:
			op->nmid=X3VM_NMID_FSUB;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FSUB_3R;
			break;
		case 0x06:
			op->nmid=X3VM_NMID_PSUBH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PSUBH_3R;
			break;


		case 0x08:
			op->nmid=X3VM_NMID_PMULF;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMULF_3R;
			break;
		case 0x09:
			op->nmid=X3VM_NMID_FMUL;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMUL_3R;
			break;
		case 0x0A:
			op->nmid=X3VM_NMID_PMULH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMULH_3R;
			break;


		case 0x0C:
			op->nmid=X3VM_NMID_PDIVF;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PDIVF_3R;
			break;
		case 0x0D:
			op->nmid=X3VM_NMID_FDIV;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FDIV_3R;
			break;
		case 0x0E:
			op->nmid=X3VM_NMID_PDIVH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PDIVH_3R;
			break;

		case 0x10:
			op->fmid=X3VM_FMID_3R;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_FSGNJF;
				op->Run=X3VM_Opc_FSGNJF_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_FSGNJNF;
				op->Run=X3VM_Opc_FSGNJNF_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_FSGNJXF;
				op->Run=X3VM_Opc_FSGNJXF_3R;
				break;
			}
			break;

		case 0x11:
			op->fmid=X3VM_FMID_3R;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_FSGNJ;
				op->Run=X3VM_Opc_FSGNJ_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_FSGNJN;
				op->Run=X3VM_Opc_FSGNJN_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_FSGNJX;
				op->Run=X3VM_Opc_FSGNJX_3R;
				break;
			}
			break;
		case 0x12:
			op->fmid=X3VM_FMID_3R;
			switch((opw>>12)&7)
			{
			case 0:
				op->nmid=X3VM_NMID_FSGNJH;
				op->Run=X3VM_Opc_FSGNJH_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_FSGNJNH;
				op->Run=X3VM_Opc_FSGNJNH_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_FSGNJXH;
				op->Run=X3VM_Opc_FSGNJXH_3R;
				break;
			}
			break;


		case 0x14:
			op->nmid=X3VM_NMID_PMINF;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMINF_3R;
			break;
		case 0x15:
			op->nmid=X3VM_NMID_FMIN;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMIN_3R;
			break;
		case 0x16:
			op->nmid=X3VM_NMID_PMINH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMINH_3R;
			break;

		case 0x18:
			op->nmid=X3VM_NMID_PMAXF;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMAXF_3R;
			break;
		case 0x19:
			op->nmid=X3VM_NMID_FMAX;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_FMAX_3R;
			break;
		case 0x1A:
			op->nmid=X3VM_NMID_PMAXH;
			op->fmid=X3VM_FMID_3R;
			op->Run=X3VM_Opc_PMAXH_3R;
			break;

		case 0x20:
			if(rt_dfl==1)
			{
				op->nmid=X3VM_NMID_FCVTSD;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTSD_2R;
				break;
			}
			if(rt_dfl==2)
			{
				op->nmid=X3VM_NMID_PCVTSH;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_PCVTSH_2R;
				break;
			}
			break;
		case 0x21:
			if(rt_dfl==0)
			{
				op->nmid=X3VM_NMID_FCVTDS;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTDS_2R;
				break;
			}
			if(rt_dfl==2)
			{
				op->nmid=X3VM_NMID_FCVTDH;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTDH_2R;
				break;
			}
			break;

		case 0x2C:
			op->nmid=X3VM_NMID_PSQRTF;
			op->fmid=X3VM_FMID_2R;
			op->Run=X3VM_Opc_PSQRTF_3R;
			break;
		case 0x2D:
			op->nmid=X3VM_NMID_FSQRT;
			op->fmid=X3VM_FMID_2R;
			op->Run=X3VM_Opc_FSQRT_3R;
			break;
		case 0x2E:
			op->nmid=X3VM_NMID_PSQRTH;
			op->fmid=X3VM_FMID_2R;
			op->Run=X3VM_Opc_PSQRTH_3R;
			break;

		case 0x50:
			op->rn=rn_dfl;
			switch((opw>>12)&7)
			{
			case 0:
				op->rs=rt_fr;
				op->rt=rs_fr;
				op->nmid=X3VM_NMID_FGEF;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FGEF_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_FLTF;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FLTF_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_FEQF;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FEQF_3R;
				break;
			}
			break;
		case 0x51:
			op->rn=rn_dfl;
			switch((opw>>12)&7)
			{
			case 0:
				op->rs=rt_fr;
				op->rt=rs_fr;
				op->nmid=X3VM_NMID_FGE;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FGE_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_FLT;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FLT_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_FEQ;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FEQ_3R;
				break;
			}
			break;
		case 0x52:
			op->rn=rn_dfl;
			switch((opw>>12)&7)
			{
			case 0:
				op->rs=rt_fr;
				op->rt=rs_fr;
				op->nmid=X3VM_NMID_FGEH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FGEH_3R;
				break;
			case 1:
				op->nmid=X3VM_NMID_FLTH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FLTH_3R;
				break;
			case 2:
				op->nmid=X3VM_NMID_FEQH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FEQH_3R;
				break;
			}
			break;

		case 0x60:
			switch(rt_dfl)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				op->nmid=X3VM_NMID_FCVTIS;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTIS_2R;
				break;
			}
			break;

		case 0x61:
			switch(rt_dfl)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				op->nmid=X3VM_NMID_FCVTID;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTID_2R;
				break;
			}
			break;

		case 0x68:
			switch(rt_dfl)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				op->nmid=X3VM_NMID_FCVTSI;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTSI_2R;
				break;
			}
			break;

		case 0x69:
			switch(rt_dfl)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				op->nmid=X3VM_NMID_FCVTDI;
				op->fmid=X3VM_FMID_2R;
				op->Run=X3VM_Opc_FCVTDI_2R;
				break;
			}
			break;
		
		case 0x70:
		case 0x71:
			op->rs=rs_dfl;
			op->rn=rn_fr;
			op->imm=0;
			op->nmid=X3VM_NMID_ADD;
			op->fmid=X3VM_FMID_3RI;
//			op->Run=X3VM_Opc_ADD_3RI;
			op->Run=X3VM_Opc_MOV_2R;
			break;
		case 0x78:
		case 0x79:
			op->rs=rs_fr;
			op->rn=rn_dfl;
			op->imm=0;
			op->nmid=X3VM_NMID_ADD;
			op->fmid=X3VM_FMID_3RI;
//			op->Run=X3VM_Opc_ADD_3RI;
			op->Run=X3VM_Opc_MOV_2R;
			break;
		}
		break;

	case 0x1C:
		op->imm=imm12s;
		op->rs=rs_dfl;
		op->rn=rn_dfl;
		switch((opw>>12)&7)
		{
		case 0:
			switch(opw>>20)
			{
			case 0:
				op->nmid=X3VM_NMID_ECALL;
				op->fmid=X3VM_FMID_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
				op->Run=X3VM_Opc_ECALL_NONE;
				break;
			case 1:
				op->nmid=X3VM_NMID_EBREAK;
				op->fmid=X3VM_FMID_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
				op->Run=X3VM_Opc_EBREAK_NONE;
				break;
			}
			break;
		case 1:
			op->nmid=X3VM_NMID_CSRRW;
			op->fmid=X3VM_FMID_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->Run=X3VM_Opc_CSRRW_3RI;
			break;
		case 2:
			op->nmid=X3VM_NMID_CSRRS;
			op->fmid=X3VM_FMID_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->Run=X3VM_Opc_CSRRS_3RI;
			break;
		case 3:
			op->nmid=X3VM_NMID_CSRRC;
			op->fmid=X3VM_FMID_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->Run=X3VM_Opc_CSRRC_3RI;
			break;

		case 5:
			op->nmid=X3VM_NMID_CSRRWI;
			op->fmid=X3VM_FMID_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->Run=X3VM_Opc_CSRRWI_3RI;
			break;
		case 6:
			op->nmid=X3VM_NMID_CSRRSI;
			op->fmid=X3VM_FMID_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->Run=X3VM_Opc_CSRRSI_3RI;
			break;
		case 7:
			op->nmid=X3VM_NMID_CSRRCI;
			op->fmid=X3VM_FMID_3RI;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->Run=X3VM_Opc_CSRRCI_3RI;
			break;
		}
		break;

	case 0x1B:
		op->imm=disp20j;
		op->rs=X3VM_REG_BPC;
		op->rn=rn_dfl;
		op->sc=0;
		op->opfl|=X3VM_OPFL_CTRLF;
		op->nmid=X3VM_NMID_JAL;
		op->fmid=X3VM_FMID_J2RI;
		op->Run=X3VM_Opc_JAL_2RI;
		if(rn_dfl==0)
			{ op->Run=X3VM_Opc_BRA_2RI; }
		if(rn_dfl==1)
			{ op->Run=X3VM_Opc_BSR_2RI; }
		break;
	case 0x05:
		op->imm=imm20i;
		op->rs=X3VM_REG_BPC;
		op->rn=rn_dfl;
		op->sc=0;
		op->nmid=X3VM_NMID_AUIPC;
		op->fmid=X3VM_FMID_2RI;
		op->Run=X3VM_Opc_LEA_3RI;
		break;
	case 0x0D:
		op->imm=imm20i;
		op->rs=0;
		op->rn=rn_dfl;
		op->sc=0;
		op->nmid=X3VM_NMID_LUI;
		op->fmid=X3VM_FMID_2RI;
//		op->Run=X3VM_Opc_ADD_3RI;
		op->Run=X3VM_Opc_MOV_2RI;
		break;
	}
	
	if(op->opw2)
	{
		__debugbreak();
	}
	
	if(do_imm_ro && (op->fmid==X3VM_FMID_3R))
	{
		op->Run=NULL;
	}

	if(!op->Run)
	{
		op->Run=X3VM_Opc_INVOP_NONE;
		op->opfl|=X3VM_OPFL_CTRLF;
	}
	
	return(0);
}

int X3VM_DecodeOpcodeRVC(X3VM_Context *ctx,
	X3VM_Opcode *op, u64 addr, u32 opw)
{
	int rs1r, rs2r;
	int rs1p, rs2p;

	int rs1r_fr, rs2r_fr;
	int rs1p_fr, rs2p_fr;

	int imm_ofs6_lwld;
	int imm_ofs6_qwld;
	int imm_ofs6_lwst;
	int imm_ofs6_qwst;
	int imm_imm6_alu;
	int imm_imm6_a16sp;
	int imm_imm8_i4add;
	int imm_imm8_bcc;
	int imm_imm10_j;

	int imm_ofs4_lwld;
	int imm_ofs4_qwld;
	int imm_ofs4_lwst;
	int imm_ofs4_qwst;

	int ret;

	op->opw0=opw;
	op->addr=addr;
	op->sc=0;	
	
	op->opfl|=X3VM_OPFL_OP16;
	
	rs1r=(opw>>7)&31;
	rs2r=(opw>>2)&31;
	rs1p=8+((opw>>7)&7);
	rs2p=8+((opw>>2)&7);
	ret=0;

	rs1r_fr=32+rs1r;
	rs2r_fr=32+rs2r;
	rs1p_fr=32+rs1p;
	rs2p_fr=32+rs2p;

	imm_ofs4_lwld=
		((opw<<1)&0x040)|
		((opw>>4)&0x004)|
		((opw>>7)&0x038);
	imm_ofs4_qwld=
		((opw<<1)&0x0C0)|
		((opw>>7)&0x038);
	imm_ofs4_lwst=imm_ofs4_lwld;
	imm_ofs4_qwst=imm_ofs4_qwld;

	imm_ofs6_lwld=
		((opw>>2)&0x01C)|
		((opw<<4)&0x0C0)|
		((opw>>7)&0x020);
	imm_ofs6_qwld=
		((opw>>2)&0x018)|
		((opw<<4)&0x1C0)|
		((opw>>7)&0x020);

	imm_ofs6_lwst=
		((opw>>1)&0x0C0)|
		((opw>>7)&0x03C);
	imm_ofs6_qwst=
		((opw>>1)&0x1C0)|
		((opw>>7)&0x038);
	
	imm_imm8_i4add=
		((opw>>2)&0x008) |
		((opw>>4)&0x004) |
		((opw>>1)&0x3C0) |
		((opw>>7)&0x030) ;
	
	imm_imm6_alu=
		((opw>>2)&0x01F) |
		((opw>>7)&0x020) ;
	
	imm_imm6_a16sp=
		((opw<<3)&0x020) |
		((opw<<4)&0x180) |
		((opw<<1)&0x040) |
		((opw>>2)&0x010) ;

	imm_imm8_bcc=
		((opw<<3)&0x020) |
		((opw>>2)&0x006) |
		((opw<<1)&0x0C0) |
		((opw>>7)&0x018) ;

	if(opw&0x1000)
	{
		imm_imm6_alu|=(~63);
		imm_imm6_a16sp|=(~511);
		imm_imm8_bcc|=(~255);
	}
	
	imm_imm10_j=
		((opw>>2)&0x00E) |
		((opw>>7)&0x010) |
		((opw<<3)&0x020) |
		((opw>>1)&0x040) |
		((opw<<1)&0x080) |
		((opw>>1)&0x300) |
		((opw<<2)&0x400) |
		((opw>>1)&0x800) ;

	if(imm_imm10_j&0x800)
		imm_imm10_j|=~0xFFF;
	
	switch(opw&3)
	{
	case 0:
		switch((opw>>13)&7)
		{
		case 0:
			if(imm_imm8_i4add!=0)
			{
				op->rs=X3VM_REG_SP;
				op->rn=rs2p;
				op->imm=imm_imm8_i4add;
				op->nmid=X3VM_NMID_ADD;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_ADD_3RI;
				break;
			}else
			{
				op->nmid=X3VM_NMID_EBREAK;
				op->fmid=X3VM_FMID_NONE;
				op->Run=X3VM_Opc_EBREAK_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
			}
			break;
		case 1:
			op->rn=rs2p_fr;
			op->rs=rs1p;
			op->imm=imm_ofs4_qwld;
			op->nmid=X3VM_NMID_FLD;
			op->fmid=X3VM_FMID_LDRI;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;
		case 2:
			op->rn=rs2p;
			op->rs=rs1p;
			op->imm=imm_ofs4_lwld;
			op->nmid=X3VM_NMID_LW;
			op->fmid=X3VM_FMID_LDRI;
			op->Run=X3VM_Opc_LDS32_3RI;
			break;
		case 3:
			op->rn=rs2p;
			op->rs=rs1p;
			op->imm=imm_ofs4_qwld;
			op->nmid=X3VM_NMID_LD;
			op->fmid=X3VM_FMID_LDRI;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;

		case 5:
			op->rs=rs2p_fr;
			op->rn=rs1p;
			op->imm=imm_ofs4_qwst;

			op->nmid=X3VM_NMID_FSD;
			op->fmid=X3VM_FMID_STRI;
			op->Run=X3VM_Opc_ST64_3RI;
			break;
		case 6:
			op->rs=rs2p;
			op->rn=rs1p;
			op->imm=imm_ofs4_lwst;

			op->nmid=X3VM_NMID_SW;
			op->fmid=X3VM_FMID_STRI;
			op->Run=X3VM_Opc_ST32_3RI;
			break;
		case 7:
			op->rs=rs2p;
			op->rn=rs1p;
			op->imm=imm_ofs4_qwst;

			op->nmid=X3VM_NMID_SD;
			op->fmid=X3VM_FMID_STRI;
			op->Run=X3VM_Opc_ST64_3RI;
			break;
		}
		break;
	case 1:
		switch((opw>>13)&7)
		{
		case 0:
			if(rs1r!=0)
			{
				op->rs=rs1r;
				op->rn=rs1r;
				op->imm=imm_imm6_alu;
				op->nmid=X3VM_NMID_ADD;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_ADD_3RI;
				break;
			}else
			{
				op->nmid=X3VM_NMID_NOP;
				op->Run=X3VM_Opc_NOP_NONE;
			}
			break;
		case 1:
			if(rs1r!=0)
			{
				op->rs=rs1r;
				op->rn=rs1r;
				op->imm=imm_imm6_alu;
				op->nmid=X3VM_NMID_ADDW;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_ADDW_3RI;
				break;
			}else
			{
				op->nmid=X3VM_NMID_NOP;
				op->Run=X3VM_Opc_NOP_NONE;
			}
			break;
		case 2:
			if(rs1r!=0)
			{
				op->rn=rs1r;
				op->imm=imm_imm6_alu;
				op->nmid=X3VM_NMID_LI;
				op->fmid=X3VM_FMID_2RI;
				op->Run=X3VM_Opc_MOV_2RI;
				break;
			}else
			{
				op->nmid=X3VM_NMID_NOP;
				op->Run=X3VM_Opc_NOP_NONE;
			}
			break;
		case 3:
			if((rs1r!=0) && (rs1r!=X3VM_REG_SP))
			{
				op->rn=rs1r;
				op->imm=imm_imm6_alu<<12;
				op->nmid=X3VM_NMID_LI;
				op->fmid=X3VM_FMID_2RI;
				op->Run=X3VM_Opc_MOV_2RI;
				break;
			}else
				if(rs1r!=0)
			{
				op->rs=X3VM_REG_SP;
				op->rn=X3VM_REG_SP;
				op->imm=imm_imm6_a16sp;
				op->nmid=X3VM_NMID_ADD;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_ADD_3RI;
				break;
			}else
			{
				op->nmid=X3VM_NMID_NOP;
				op->Run=X3VM_Opc_NOP_NONE;
			}
			break;
		case 4:
			switch((opw>>10)&3)
			{
			case 0:
				op->rs=rs1p;
				op->rn=rs1p;
				op->imm=-(imm_imm6_alu&63);
				op->nmid=X3VM_NMID_SLL;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLL_3RI;
				break;
			case 1:
				op->rs=rs1p;
				op->rn=rs1p;
				op->imm=-(imm_imm6_alu&63);
				op->nmid=X3VM_NMID_SLA;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLA_3RI;
				break;
			case 2:
				op->rs=rs1p;
				op->rn=rs1p;
				op->imm=imm_imm6_alu;
				op->nmid=X3VM_NMID_AND;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_AND_3RI;
				break;
			case 3:
				op->rs=rs1p;
				op->rt=rs2p;
				op->rn=rs1p;
				if((opw>>12)&1)
				{
					switch((opw>>5)&3)
					{
					case 0:
						op->nmid=X3VM_NMID_SUBW;
						op->fmid=X3VM_FMID_3R;
						op->Run=X3VM_Opc_SUBW_3R;
						break;
					case 1:
						op->nmid=X3VM_NMID_ADDW;
						op->fmid=X3VM_FMID_3R;
						op->Run=X3VM_Opc_ADDW_3R;
						break;
					case 2:
						break;
					case 3:
						break;
					}
				}else
				{
					switch((opw>>5)&3)
					{
					case 0:
						op->nmid=X3VM_NMID_SUB;
						op->fmid=X3VM_FMID_3R;
						op->Run=X3VM_Opc_SUB_3R;
						break;
					case 1:
						op->nmid=X3VM_NMID_XOR;
						op->fmid=X3VM_FMID_3R;
						op->Run=X3VM_Opc_XOR_3R;
						break;
					case 2:
						op->nmid=X3VM_NMID_OR;
						op->fmid=X3VM_FMID_3R;
						op->Run=X3VM_Opc_OR_3R;
						break;
					case 3:
						op->nmid=X3VM_NMID_AND;
						op->fmid=X3VM_FMID_3R;
						op->Run=X3VM_Opc_AND_3R;
						break;
					}
				}
			}
			break;
		case 5:
			op->opfl|=X3VM_OPFL_CTRLF;

			op->rn=X3VM_REG_ZR;
			op->imm=imm_imm10_j;
			op->nmid=X3VM_NMID_JAL;
			op->fmid=X3VM_FMID_J2RI;
			op->Run=X3VM_Opc_BRA_2RI;
			break;
		case 6:
			op->rs=rs1p;
			op->rt=X3VM_REG_ZR;
			op->imm=imm_imm8_bcc;

			op->opfl|=X3VM_OPFL_CTRLF;

			op->nmid=X3VM_NMID_BEQ;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BEQ_3RI;
			break;
		case 7:
			op->rs=rs1p;
			op->rt=X3VM_REG_ZR;
			op->imm=imm_imm8_bcc;

			op->opfl|=X3VM_OPFL_CTRLF;

			op->nmid=X3VM_NMID_BNE;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BNE_3RI;
			break;
		}
		break;
	case 2:
		op->rn=rs1r;
		op->rs=X3VM_REG_SP;

		switch((opw>>13)&7)
		{
		case 0:
			if(rs1r!=0)
			{
				op->rs=rs1r;
				op->rn=rs1r;
				op->imm=(imm_imm6_alu&63);
				op->nmid=X3VM_NMID_SLL;
				op->fmid=X3VM_FMID_3RI;
				op->Run=X3VM_Opc_SLL_3RI;
				break;
			}else
			{
				op->nmid=X3VM_NMID_NOP;
				op->Run=X3VM_Opc_NOP_NONE;
			}
			break;
		case 1:
			op->rn=rs1r_fr;
			op->imm=imm_ofs6_qwld;
			op->nmid=X3VM_NMID_FLD;
			op->fmid=X3VM_FMID_LDRI;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;
		case 2:
			op->rn=rs1r;
			op->rs=X3VM_REG_SP;
			op->imm=imm_ofs6_lwld;
			op->nmid=X3VM_NMID_LW;
			op->fmid=X3VM_FMID_LDRI;
			op->Run=X3VM_Opc_LDS32_3RI;
			break;
		case 3:
			op->rn=rs1r;
			op->rs=X3VM_REG_SP;
			op->imm=imm_ofs6_qwld;
			op->nmid=X3VM_NMID_LD;
			op->fmid=X3VM_FMID_LDRI;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;
		case 4:
			if(opw&(1<<12))
			{
				if((rs1r!=X3VM_REG_ZR) && (rs2r!=X3VM_REG_ZR))
				{
					op->rn=rs1r;
					op->rs=rs2r;
					op->rt=rs1r;

					op->nmid=X3VM_NMID_ADD;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_ADD_3R;
				}else if(rs1r!=0)
				{
					op->rn=1;
					op->rs=rs1r;

					op->nmid=X3VM_NMID_JALR;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_JALR_3RI;
					op->opfl|=X3VM_OPFL_CTRLF;
				}else
				{
					op->nmid=X3VM_NMID_ECALL;
					op->fmid=X3VM_FMID_NONE;
					op->Run=X3VM_Opc_ECALL_NONE;
					op->opfl|=X3VM_OPFL_CTRLF;
				}
			}else
			{
				if((rs1r!=X3VM_REG_ZR) && (rs2r!=X3VM_REG_ZR))
				{
					op->rs=rs2r;
					op->rn=rs1r;

					op->nmid=X3VM_NMID_ADD;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_MOV_2R;
				}else if(rs1r!=0)
				{
					op->rn=0;
					op->rs=rs1r;

					op->nmid=X3VM_NMID_JALR;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_JALR_3RI;
					op->opfl|=X3VM_OPFL_CTRLF;
				}else
				{
					op->nmid=X3VM_NMID_NOP;
					op->Run=X3VM_Opc_NOP_NONE;
				}
			}
			break;
		case 5:
			op->rn=rs2r_fr;
			op->rs=X3VM_REG_SP;
			op->imm=imm_ofs6_qwst;

			op->nmid=X3VM_NMID_FSD;
			op->fmid=X3VM_FMID_STRI;
			op->Run=X3VM_Opc_ST64_3RI;
			break;
		case 6:
			op->rn=rs2r;
			op->rs=X3VM_REG_SP;
			op->imm=imm_ofs6_lwst;

			op->nmid=X3VM_NMID_SW;
			op->fmid=X3VM_FMID_STRI;
			op->Run=X3VM_Opc_ST32_3RI;
			break;
		case 7:
			op->rn=rs2r;
			op->rs=X3VM_REG_SP;
			op->imm=imm_ofs6_qwst;

			op->nmid=X3VM_NMID_SD;
			op->fmid=X3VM_FMID_STRI;
			op->Run=X3VM_Opc_ST64_3RI;
			break;
		}
		break;
	default:
		ret=-1;
		break;
	}

	return(ret);
}

