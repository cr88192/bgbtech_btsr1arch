int X3VM_DecodeOpcodeXG3(X3VM_Context *ctx,
	X3VM_Opcode *op, u64 addr, u32 opw, u64 jbits)
{
	u64 jb1;
	u32 opw1;
	byte rs, rt, rn, eq, jwi;
	s64 imm10u, imm10n, imm10s, imm6s;
	s64 imm16u, imm16n, imm16s, disp23;
	
	if((opw&3)==3)
	{
		__debugbreak();
	}
	
	rn=(opw>> 6)&63;
	rs=(opw>>16)&63;
	rt=(opw>>22)&63;
	eq=(opw>>5)&1;
	
	imm10u=(opw>>22)&1023;
	imm10n=imm10u|(~1023);
	imm10s=((s32)opw)>>22;

	imm16u=(opw>>16)&0xFFFFLL;
	imm16n=imm16u|(~0xFFFFLL);
	imm16s=((s32)opw)>>16;

	disp23=
		((opw>>16)&0xFFFFLL) |
		(((opw>> 6)&63)<<16) |
//		(((s32)(opw<<26))>>9) ;
		(((s32)((opw>>5)<<31))>>9) ;
	disp23=disp23<<2;

	imm6s=rt;
	if(rt>=32)imm6s|=~63;
	jwi=0;

	if(jbits)
	{
		imm10s=
			((jbits&0xFFFFFF)<<8) |
			((opw>>22)&0xFF) |
			(((s64)(((s32)(opw<<1))>>31))<<32) ;
		imm10u=imm10s;
		imm10n=imm10s;
		
		jwi=(opw>>31)&1;

		imm16s=
			((((opw>>5)&1LL)<<63)>>31) |
			((jbits&0xFFFF)<<16) |
			(((u32)opw)>>16);
		imm16u=(u32)imm16s;
		imm16n=imm16s|0xFFFFFFFF00000000LL;

		imm6s=
			((((rt>>5)&1LL)<<63)>>31) |
			(((jbits>>24)&7LL)<<29) |
			(((rt>>4)&1LL)<<28) |
			((jbits&0xFFFFFFLL)<<4) |
			(rt&15) ;

		if(jbits>>32)
		{
			imm10s=
				((u32)imm10s) |
				(((jbits>>32)&0x07FFFFFFLL)<<32) |
				(((jbits>>24)&0x00000007LL)<<59) |
				(((opw>>30)&0x3LL)<<62) ;
			imm10u=imm10s;
			imm10n=imm10s;
			
			imm16u=
				(((opw  >>16)&0x00FFFFLL)<< 0) |
				(((jbits>> 0)&0xFFFFFFLL)<<16) |
				(((jbits>>32)&0xFFFFFFLL)<<40) ;
			imm16n=imm16u;
			imm16s=imm16u;
		}
	}

	op->opw0=opw;
	op->opw1=0;
	op->opw2=0;
	op->addr=addr;
	op->nmid=0;
	op->fmid=0;
	op->rs=rs;
	op->rt=rt;
	op->ru=0;
	op->rn=rn;
	op->sc=0;
	op->opfl=0;
	op->Run=NULL;

	switch((opw>>2)&15)
	{
	case 0x6:	case 0x7:
	case 0xE:	case 0xF:
		/* Jumbo Prefix */
		opw1=X3VM_MemLoadU32(ctx, addr+4);
		if(!opw1)
			break;
		if((opw&3)!=2)
			break;
		jb1=
			(((opw>>16)&0xFFFF)<< 0)|
			(((opw>> 8)&0x00FF)<<16)|
			(((opw>> 5)&0x0007)<<24)|
			(((opw>> 2)&0x0001)<<27)|
			(1<<28)|(jbits<<32);
		X3VM_DecodeOpcodeXG3(ctx, op, addr+4, opw1, jb1);
		op->opw2=op->opw1;
		op->opw1=op->opw0;
		op->opw0=opw;
		op->addr=addr;
		break;
	
	/* F0 Block */
	case 0x0:	/* zzzz-oooooo-mmmmmm-zzzz-nnnnnn-00-0010 */
	case 0x8:	/* zzzz-oooooo-mmmmmm-zzzz-nnnnnn-10-0010 */
		switch((opw>>12)&15)
		{
		case 0x0:	/* zzzz-oooooo-mmmmmm-0000-nnnnnn-00-0010 */
			switch((opw>>28)&15)
			{
			case 0x4:
				if(eq)
				{	op->nmid=X3VM_NMID_LEAB;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=0;
					op->Run=X3VM_Opc_LEA_3R;
					break;	}
				op->nmid=X3VM_NMID_SB;
				op->fmid=X3VM_FMID_STRR;
				op->sc=0;
				op->Run=X3VM_Opc_ST8_3R;
				break;
			case 0x5:
				if(eq)
				{	op->nmid=X3VM_NMID_LEAH;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=1;
					op->Run=X3VM_Opc_LEA_3R;
					break;	}
				op->nmid=X3VM_NMID_SH;
				op->fmid=X3VM_FMID_STRR;
				op->sc=1;
				op->Run=X3VM_Opc_ST16_3R;
				break;
			case 0x6:
				if(eq)
				{	op->nmid=X3VM_NMID_LEAW;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=2;
					op->Run=X3VM_Opc_LEA_3R;
					break;	}
				op->nmid=X3VM_NMID_SW;
				op->fmid=X3VM_FMID_STRR;
				op->sc=2;
				op->Run=X3VM_Opc_ST32_3R;
				break;
			case 0x7:
				if(eq)
				{	op->nmid=X3VM_NMID_LEAD;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=3;
					op->Run=X3VM_Opc_LEA_3R;
					break;	}
				op->nmid=X3VM_NMID_SD;
				op->fmid=X3VM_FMID_STRR;
				op->sc=3;
				op->Run=X3VM_Opc_ST64_3R;
				break;

			case 0xC:
				if(eq)
				{	op->nmid=X3VM_NMID_LBU;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=0;
					op->Run=X3VM_Opc_LDU8_3R;
					break;	}
				op->nmid=X3VM_NMID_LB;
				op->fmid=X3VM_FMID_LDRR;
				op->sc=0;
				op->Run=X3VM_Opc_LDS8_3R;
				break;
			case 0xD:
				if(eq)
				{	op->nmid=X3VM_NMID_LHU;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=1;
					op->Run=X3VM_Opc_LDU16_3R;
					break;	}
				op->nmid=X3VM_NMID_LH;
				op->fmid=X3VM_FMID_LDRR;
				op->sc=1;
				op->Run=X3VM_Opc_LDS16_3R;
				break;
			case 0xE:
				if(eq)
				{	op->nmid=X3VM_NMID_LWU;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=2;
					op->Run=X3VM_Opc_LDU32_3R;
					break;	}
				op->nmid=X3VM_NMID_LW;
				op->fmid=X3VM_FMID_LDRR;
				op->sc=2;
				op->Run=X3VM_Opc_LDS32_3R;
				break;
			case 0xF:
				if(eq)
					{	break;	}
				op->nmid=X3VM_NMID_LD;
				op->fmid=X3VM_FMID_LDRR;
				op->sc=3;
				op->Run=X3VM_Opc_LDS64_3R;
				break;

			default:
				break;
			}
			break;
		case 0x1:	/* zzzz-oooooo-mmmmmm-0001-nnnnnn-00-0010 */
			switch((opw>>28)&15)
			{
			case 0x0:
				if(eq)
				{	op->nmid=X3VM_NMID_ADDX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_ADDX_3R;
					break;	}
				op->nmid=X3VM_NMID_ADD;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ADD_3R;
				if(op->rt==0)
					{ op->Run=X3VM_Opc_MOV_2R; }
				break;
			case 0x1:
				if(eq)
				{	op->nmid=X3VM_NMID_SUBX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SUBX_3R;
					break;	}
				op->nmid=X3VM_NMID_SUB;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SUB_3R;
				break;
			case 0x2:
				if(eq)
				{	op->nmid=X3VM_NMID_MULQ;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_MULQ_3R;
					break;	}
				op->nmid=X3VM_NMID_MULW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULW_3R;
				break;
			case 0x3:
				if(eq)
				{	op->nmid=X3VM_NMID_MULQU;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_MULQU_3R;
					break;	}
				op->nmid=X3VM_NMID_MULWU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MULWU_3R;
				break;
			case 0x4:
				if(eq)
				{	op->nmid=X3VM_NMID_MAX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_MAX_3R;
					break;	}
				op->nmid=X3VM_NMID_MIN;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MIN_3R;
				break;
			case 0x5:
				if(eq)
				{	op->nmid=X3VM_NMID_ANDX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_ANDX_3R;
					break;	}
				op->nmid=X3VM_NMID_AND;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_AND_3R;
				break;
			case 0x6:
				if(eq)
				{	op->nmid=X3VM_NMID_ORX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_ORX_3R;
					break;	}
				op->nmid=X3VM_NMID_OR;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_OR_3R;
				break;
			case 0x7:
				if(eq)
				{	op->nmid=X3VM_NMID_XORX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_XORX_3R;
					break;	}
				op->nmid=X3VM_NMID_XOR;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_XOR_3R;
				break;
			
			case 0x8:
				switch((opw>>22)&63)
				{
				default:
					break;
				}
				break;
			case 0xC:
				switch((opw>>22)&63)
				{
				case 0x08:
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_EXTSB;
					op->Run=X3VM_Opc_EXTSB_2R;
					if(eq)
					{
						op->nmid=X3VM_NMID_EXTUB;
						op->Run=X3VM_Opc_EXTUB_2R;
					}
					break;
				case 0x09:
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_EXTSH;
					op->Run=X3VM_Opc_EXTSH_2R;
					if(eq)
					{
						op->nmid=X3VM_NMID_EXTUH;
						op->Run=X3VM_Opc_EXTUH_2R;
					}
					break;
				default:
					break;
				}
				break;
			case 0xD:
				switch((opw>>22)&63)
				{
				case 0x00:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTDS;
					op->Run=X3VM_Opc_FCVTDS_2R;
					break;
				case 0x01:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTDUS;
					op->Run=X3VM_Opc_FCVTDUS_2R;
					break;
				case 0x02:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTDI;
					op->Run=X3VM_Opc_FCVTDI_2R;
					break;

				case 0x03:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTDH;
					op->Run=X3VM_Opc_FCVTDH_2R;
					break;
				case 0x04:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTSD;
					op->Run=X3VM_Opc_FCVTSD_2R;
					break;
				case 0x05:
					if(eq)
						break;
//					op->fmid=X3VM_FMID_2R;
//					op->nmid=X3VM_NMID_FCVTDH;
//					op->Run=X3VM_Opc_FCVTDH_2R;
					break;

				case 0x06:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTID;
					op->Run=X3VM_Opc_FCVTID_2R;
					break;

				case 0x07:
					if(eq)
						break;
					op->fmid=X3VM_FMID_2R;
					op->nmid=X3VM_NMID_FCVTHD;
					op->Run=X3VM_Opc_FCVTHD_2R;
					break;

				case 0x08:
					if(eq)
						break;
//					op->fmid=X3VM_FMID_2R;
					op->fmid=X3VM_FMID_3R;
//					op->nmid=X3VM_NMID_FNEG;
					op->nmid=X3VM_NMID_FSGNJN;
//					op->Run=X3VM_Opc_FNEG_2R;
					op->rt=0;
					op->Run=X3VM_Opc_FSGNJN_3R;
					break;
				case 0x09:
					if(eq)
						break;
//					op->fmid=X3VM_FMID_2R;
					op->fmid=X3VM_FMID_3R;
//					op->nmid=X3VM_NMID_FABS;
					op->nmid=X3VM_NMID_FSGNJ;
					op->rt=0;
					op->Run=X3VM_Opc_FSGNJ_3R;
					break;
				}
				break;
			}
			break;

		case 0x2:
			switch((opw>>28)&15)
			{
			case 0x0:
				if(eq)
				{	op->nmid=X3VM_NMID_PADDL;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PADDL_3R;
					break;	}
				op->nmid=X3VM_NMID_PADDW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PADDW_3R;
				break;
			case 0x1:
				if(eq)
				{	op->nmid=X3VM_NMID_PSUBL;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PSUBL_3R;
					break;	}
				op->nmid=X3VM_NMID_PSUBW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PSUBW_3R;
				break;
			case 0x2:
				if(jbits)
				{
					op->imm=jbits&0xFFFFFF;
					op->nmid=X3VM_NMID_BITMOVS;
					op->fmid=X3VM_FMID_4RI;
					op->Run=X3VM_Opc_BITMOVS_4RI;
					break;
				}
				if(eq)
				{	op->nmid=X3VM_NMID_SRA;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SRA_3R;
					break;	}
				op->nmid=X3VM_NMID_SRAW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SRAW_3R;
				break;
			case 0x3:
				if(jbits)
				{
					op->imm=jbits&0xFFFFFF;
					op->nmid=X3VM_NMID_BITMOV;
					op->fmid=X3VM_FMID_4RI;
					op->Run=X3VM_Opc_BITMOV_4RI;
					break;
				}
				if(eq)
				{	op->nmid=X3VM_NMID_SRL;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SRL_3R;
					break;	}
				op->nmid=X3VM_NMID_SRLW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SRLW_3R;
				break;

			case 0x5:
				if(eq)
				{	op->nmid=X3VM_NMID_PADDXF;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PADDXF_3R;
					break;	}
				op->nmid=X3VM_NMID_PADDF;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PADDF_3R;
				break;
			case 0x6:
				if(eq)
				{	op->nmid=X3VM_NMID_PSUBXF;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PSUBXF_3R;
					break;	}
				op->nmid=X3VM_NMID_PSUBF;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PSUBF_3R;
				break;
			case 0x7:
				if(eq)
				{	op->nmid=X3VM_NMID_PMULXF;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PMULXF_3R;
					break;	}
				op->nmid=X3VM_NMID_PMULF;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PMULF_3R;
				break;
			case 0x8:
				if(eq)
				{	op->nmid=X3VM_NMID_PACK;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PACKLL_3R;
					break;	}
				op->nmid=X3VM_NMID_PACKU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PACKUU_3R;
				break;
			case 0x9:
				if(eq)
				{	op->nmid=X3VM_NMID_PACKLU;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PACKLU_3R;
					break;	}
				op->nmid=X3VM_NMID_PACKUL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PACKUL_3R;
				break;

			case 0xD:
				if(eq)
				{	op->nmid=X3VM_NMID_PADDXD;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PADDXD_3R;
					break;	}
				op->nmid=X3VM_NMID_PADDH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PADDH_3R;
				break;
			case 0xE:
				if(eq)
				{	op->nmid=X3VM_NMID_PSUBXD;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PSUBXD_3R;
					break;	}
				op->nmid=X3VM_NMID_PSUBH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PSUBH_3R;
				break;
			case 0xF:
				if(eq)
				{	op->nmid=X3VM_NMID_PMULXD;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PMULXD_3R;
					break;	}
				op->nmid=X3VM_NMID_PMULH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PMULH_3R;
				break;
			}
			break;

		case 0x3:
			op->imm=imm6s;
			switch((opw>>28)&15)
			{
			case 0x0:
				break;
			case 0x1:
				if(eq)
				{	op->nmid=X3VM_NMID_MOVTT;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_MOVTT5_3RI;
					if(jbits)
						op->Run=X3VM_Opc_MOVTT_3RI;
					break;	}
				op->nmid=X3VM_NMID_MOVTT;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MOVTT_3R;
				break;
			case 0x2:
				if(eq)
				{
					op->nmid=X3VM_NMID_SRAX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SRAX_3R;
					break;
				}
				op->nmid=X3VM_NMID_ROTLQ;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ROTLQ_3R;
				break;
			case 0x3:
				if(eq)
				{
					op->nmid=X3VM_NMID_SRLX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SRLX_3R;
					break;
				}
				op->nmid=X3VM_NMID_ROTRQ;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ROTRQ_3R;
				break;

			case 0x4:
				if(eq)
				{
					op->nmid=X3VM_NMID_SLAX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SLAX_3R;
					break;
				}
//				op->nmid=X3VM_NMID_MULHS;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_MULHS_3R;
				break;
			case 0x5:
				if(eq)
				{
					op->nmid=X3VM_NMID_SLLX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SLLX_3R;
					break;
				}
//				op->nmid=X3VM_NMID_MULHU;
//				op->fmid=X3VM_FMID_3R;
//				op->Run=X3VM_Opc_MULHU_3R;
				break;
			}
			break;

		case 0x4:
			op->imm=imm6s;
			switch((opw>>28)&15)
			{
			case 0x0:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATB;
					op->fmid=X3VM_FMID_LDRI;
					op->sc=0;
					op->Run=X3VM_Opc_LEAT_3RI;
					break;	}
				break;
			case 0x1:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATH;
					op->fmid=X3VM_FMID_LDRI;
					op->sc=1;
					op->Run=X3VM_Opc_LEAT_3RI;
					break;	}
				break;
			case 0x2:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATW;
					op->fmid=X3VM_FMID_LDRI;
					op->sc=2;
					op->Run=X3VM_Opc_LEAT_3RI;
					break;	}
				break;
			case 0x3:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATD;
					op->fmid=X3VM_FMID_LDRI;
					op->sc=3;
					op->Run=X3VM_Opc_LEAT_3RI;
					break;	}
				break;

			case 0x4:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATB;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=0;
					op->Run=X3VM_Opc_LEAT_3R;
					break;	}
				break;
			case 0x5:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATH;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=1;
					op->Run=X3VM_Opc_LEAT_3R;
					break;	}
				break;

			case 0x6:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATW;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=2;
					op->Run=X3VM_Opc_LEAT_3R;
					break;	}
				op->nmid=X3VM_NMID_FSD2W;
				op->fmid=X3VM_FMID_STRR;
				op->sc=2;
				op->Run=X3VM_Opc_FST64TO32_3R;
				break;
			case 0x7:
				if(eq)
				{	op->nmid=X3VM_NMID_LEATD;
					op->fmid=X3VM_FMID_LDRR;
					op->sc=3;
					op->Run=X3VM_Opc_LEAT_3R;
					break;	}
				op->nmid=X3VM_NMID_FSD2H;
				op->fmid=X3VM_FMID_STRR;
				op->sc=1;
				op->Run=X3VM_Opc_FST64TO16_3R;
				break;

			case 0xE:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_FLW2D;
				op->fmid=X3VM_FMID_LDRR;
				op->sc=2;
				op->Run=X3VM_Opc_FLD32TO64_3R;
				break;
			case 0xF:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_FLH2D;
				op->fmid=X3VM_FMID_LDRR;
				op->sc=1;
				op->Run=X3VM_Opc_FLD16TO64_3R;
				break;
			}
			break;

		case 0x5:
			switch((opw>>28)&15)
			{
			case 0x0:
				break;
			case 0x1:
				if(eq)
				{	op->nmid=X3VM_NMID_PMULUW;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PMULUW_3R;
					break;	}
				op->nmid=X3VM_NMID_PMULSW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PMULSW_3R;
				break;

			case 0x2:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_DMULW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DMULW_3R;
				break;
			case 0x3:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_DMULWU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DMULWU_3R;
				break;

			case 0x4:
				if(eq)
				{	op->nmid=X3VM_NMID_SLA;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SLA_3R;
					break;	}
				op->nmid=X3VM_NMID_SLAW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLAW_3R;
				break;
			case 0x5:
				if(eq)
				{	op->nmid=X3VM_NMID_SLL;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SLL_3R;
					break;	}
				op->nmid=X3VM_NMID_SLLW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SLLW_3R;
				break;

			case 0x6:
				if(eq)
				{	op->nmid=X3VM_NMID_PMULULW;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PMULULW_3R;
					break;	}
				op->nmid=X3VM_NMID_PMULSLW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PMULSLW_3R;
				break;
			case 0x7:
				if(eq)
				{	op->nmid=X3VM_NMID_PMULUHW;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_PMULUHW_3R;
					break;	}
				op->nmid=X3VM_NMID_PMULSHW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_PMULSHW_3R;
				break;

			case 0x8:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_FADD;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FADD_3R;
				break;
			case 0x9:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_FSUB;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FSUB_3R;
				break;
			case 0xA:
				if(eq)
					{ break; }
				op->nmid=X3VM_NMID_FMUL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FMUL_3R;
				break;
			case 0xB:
				if(eq)
					{ break; }
				op->ru=rn;
				op->nmid=X3VM_NMID_FMAC;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FMAC_4R;
				break;

			case 0xC:
				if(eq)
				{	op->nmid=X3VM_NMID_ADDWU;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_ADDWU_3R;
					break;	}
				op->nmid=X3VM_NMID_ADDW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_ADDW_3R;
				break;
			case 0xD:
				if(eq)
				{	op->nmid=X3VM_NMID_SUBWU;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SUBWU_3R;
					break;	}
				op->nmid=X3VM_NMID_SUBW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_SUBW_3R;
				break;
			case 0xE:
				if(eq)
				{	op->imm=rt;
					op->nmid=X3VM_NMID_DMULH;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_DMULH_3RI;
					break;	}
				op->nmid=X3VM_NMID_DMULH;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DMULH_3R;
				break;
			case 0xF:
				if(eq)
				{	op->imm=rt;
					op->nmid=X3VM_NMID_DMULHU;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_DMULHU_3RI;
					break;	}
				op->nmid=X3VM_NMID_DMULHU;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DMULHU_3R;
				break;
			}
			break;

		case 0x6:
			switch((opw>>28)&15)
			{
#if 0
			case 0x0:
				if(eq)
				{
					op->nmid=X3VM_NMID_MACSW;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_MACSW_3R;
					break;
				}
				op->nmid=X3VM_NMID_MACSW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_MACSW_3R;
				break;
#endif

			case 0x4:
				if(eq)
				{
					op->nmid=X3VM_NMID_DIVU;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_DIVU_3R;
					break;
				}
				op->nmid=X3VM_NMID_DIV;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DIV_3R;
				break;
			case 0x5:
				if(eq)
				{
					op->nmid=X3VM_NMID_REMU;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_REMU_3R;
					break;
				}
				op->nmid=X3VM_NMID_REM;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_REM_3R;
				break;

			case 0x6:
			case 0x7:
				if(eq)
				{
					break;
				}
				op->nmid=X3VM_NMID_FDIV;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FDIV_3R;
				break;

			case 0xD:
				if(eq)
				{
					break;
				}
				op->nmid=X3VM_NMID_FADD;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FADD_3R;
				break;
			case 0xE:
				if(eq)
				{
					break;
				}
				op->nmid=X3VM_NMID_FSUB;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FSUB_3R;
				break;
			case 0xF:
				if(eq)
				{
					break;
				}
				op->nmid=X3VM_NMID_FMUL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FMUL_3R;
				break;
			}
			break;

		case 0x7:
			switch((opw>>28)&15)
			{
			case 0x0:
				if(eq)
				{
					op->nmid=X3VM_NMID_FMIN;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_FMIN_3R;
					break;
				}
				op->nmid=X3VM_NMID_FADD;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FADD_3R;
				break;
			case 0x1:
				if(eq)
				{
					op->nmid=X3VM_NMID_FMAX;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_FMAX_3R;
					break;
				}
				op->nmid=X3VM_NMID_FSUB;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FSUB_3R;
				break;
			case 0x2:
				if(eq)
				{
					break;
				}
				op->nmid=X3VM_NMID_FMUL;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_FMUL_3R;
				break;

			case 0x4:
				if(eq)
				{
					op->nmid=X3VM_NMID_DIVUW;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_DIVUW_3R;
					break;
				}
				op->nmid=X3VM_NMID_DIVW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_DIVW_3R;
				break;
			case 0x5:
				if(eq)
				{
					op->nmid=X3VM_NMID_REMUW;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_REMUW_3R;
					break;
				}
				op->nmid=X3VM_NMID_REMW;
				op->fmid=X3VM_FMID_3R;
				op->Run=X3VM_Opc_REMW_3R;
				break;
			}
			break;

		case 0x8:
			op->imm=imm6s;
			switch((opw>>28)&15)
			{
			case 0x1:
				if(eq)
				{
					op->fmid=X3VM_FMID_3RI;
//					op->nmid=X3VM_NMID_STST;
//					op->Run=X3VM_Opc_STST_3R;
					op->nmid=X3VM_NMID_SNTST;
					op->Run=X3VM_Opc_SNTST_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSNTST_3R;
					break;
				}
				break;
			case 0x2:
				if(eq)
				{
					op->fmid=X3VM_FMID_3RI;
//					op->nmid=X3VM_NMID_SNTST;
//					op->Run=X3VM_Opc_SNTST_3R;
					op->nmid=X3VM_NMID_STST;
					op->Run=X3VM_Opc_STST_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSTST_3R;
					break;
				}
				break;
			}
			break;

		case 0x9:
			op->imm=imm6s;
			switch((opw>>28)&15)
			{
			case 0x0:
				if(eq)
				{
					op->nmid=X3VM_NMID_SEQ;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_SEQ_3RI;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSEQ_3RI;
					break;
				}
				break;
			case 0x1:
				if(eq)
				{
					op->nmid=X3VM_NMID_SGT;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_SGT_3RI;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSGT_3RI;
					break;
				}
				break;
			case 0x2:
				if(eq)
				{
					op->nmid=X3VM_NMID_SNE;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_SNE_3RI;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSNE_3RI;
					break;
				}
				break;
			case 0x3:
				if(eq)
				{
					op->nmid=X3VM_NMID_SLT;
					op->fmid=X3VM_FMID_3RI;
					op->Run=X3VM_Opc_SLT_3RI;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSLT_3RI;
					break;
				}
				break;

			case 0x4:
				if(eq)
				{
					op->nmid=X3VM_NMID_SEQ;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SEQ_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSEQ_3R;
					break;
				}
				break;
			case 0x5:
				if(eq)
				{
					op->rs=rt;
					op->rt=rs;
					op->nmid=X3VM_NMID_SLT;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SLT_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSLT_3R;
					break;
				}
				break;
			case 0x6:
				if(eq)
				{
					op->nmid=X3VM_NMID_SNE;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SNE_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSNE_3R;
					break;
				}
				break;
			case 0x7:
				if(eq)
				{
					op->nmid=X3VM_NMID_SGE;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_SGE_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZSGE_3R;
					break;
				}
				break;

			case 0xD:
				if(eq)
				{
					op->nmid=X3VM_NMID_FEQ;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_FEQ_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZFEQ_3R;
					break;
				}
				break;
			case 0xE:
				if(eq)
				{
					op->rs=rt;
					op->rt=rs;
					op->nmid=X3VM_NMID_FLT;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_FLT_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZFLT_3R;
					break;
				}
				break;
			case 0xF:
				if(eq)
				{
//					op->rs=rt;
//					op->rt=rs;
					op->nmid=X3VM_NMID_FGE;
					op->fmid=X3VM_FMID_3R;
					op->Run=X3VM_Opc_FGE_3R;
					if(op->rn==0)
						op->Run=X3VM_Opc_ZFGE_3R;
					break;
				}
				break;
			}
			break;

		case 0xC:
			op->rn=X3VM_REG_ZR;
			op->imm=disp23;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_JAL;
			op->fmid=X3VM_FMID_J2RI;
			op->Run=X3VM_Opc_BRA_2RI;
			break;
		case 0xD:
			op->rn=X3VM_REG_LR;
			op->imm=disp23;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_JAL;
			op->fmid=X3VM_FMID_J2RI;
			op->Run=X3VM_Opc_BSR_2RI;
			break;

		default:
			break;
		}
		break;


	/* F1 Block */
	case 0x1:	/* iiii-iiiiii-mmmmmm-zzzz-nnnnnn-00-0110 */
		op->imm=imm10s;
		switch((opw>>12)&15)
		{
		case 0x0:
			op->nmid=X3VM_NMID_SB;
			op->fmid=X3VM_FMID_STRI;
			op->sc=0;
			op->Run=X3VM_Opc_ST8_3RI;
			break;
		case 0x1:
			op->nmid=X3VM_NMID_SH;
			op->fmid=X3VM_FMID_STRI;
			op->sc=1;
			op->Run=X3VM_Opc_ST16_3RI;
			break;
		case 0x2:
			op->nmid=X3VM_NMID_SW;
			op->fmid=X3VM_FMID_STRI;
			op->sc=2;
			op->Run=X3VM_Opc_ST32_3RI;
			break;
		case 0x3:
			op->nmid=X3VM_NMID_SD;
			op->fmid=X3VM_FMID_STRI;
			op->sc=3;
			op->Run=X3VM_Opc_ST64_3RI;
			break;

		case 0x4:
			op->nmid=X3VM_NMID_FSD2W;
			op->fmid=X3VM_FMID_STRI;
			op->sc=2;
			op->Run=X3VM_Opc_FST64TO32_3RI;
			break;

		case 0x6:
			op->nmid=X3VM_NMID_FLW2D;
			op->fmid=X3VM_FMID_STRI;
			op->sc=2;
			op->Run=X3VM_Opc_FLD32TO64_3RI;
			break;

		case 0x8:
			op->nmid=X3VM_NMID_LB;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDS8_3RI;
			break;
		case 0x9:
			op->nmid=X3VM_NMID_LH;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=1;
			op->Run=X3VM_Opc_LDS16_3RI;
			break;
		case 0xA:
			op->nmid=X3VM_NMID_LW;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=2;
			op->Run=X3VM_Opc_LDS32_3RI;
			break;
		case 0xB:
			op->nmid=X3VM_NMID_LD;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=3;
			op->Run=X3VM_Opc_LDS64_3RI;
			break;

		case 0xC:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BTSTT;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BTSTT_3RI;
			break;
		case 0xD:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BLT;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BLT_3RI;
			break;
		case 0xE:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BLTU;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BLTU_3RI;
			break;
		case 0xF:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BEQ;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BEQ_3RI;
			break;
		}
		break;
	case 0x9:	/* iiii-iiiiii-mmmmmm-zzzz-nnnnnn-10-0110 */
		op->imm=imm10s;
		switch((opw>>12)&15)
		{
		case 0x0:
			op->nmid=X3VM_NMID_LEAB;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LEA_3RI;
			break;
		case 0x1:
			op->nmid=X3VM_NMID_LEAH;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=1;
			op->Run=X3VM_Opc_LEA_3RI;
			break;
		case 0x2:
			op->nmid=X3VM_NMID_LEAW;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=2;
			op->Run=X3VM_Opc_LEA_3RI;
			break;
		case 0x3:
			op->nmid=X3VM_NMID_LEAD;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=3;
			op->Run=X3VM_Opc_LEA_3RI;
			break;

		case 0x4:
			op->nmid=X3VM_NMID_FSD2H;
			op->fmid=X3VM_FMID_STRI;
			op->sc=1;
			op->Run=X3VM_Opc_FST64TO16_3RI;
			break;
		case 0x5:
			op->nmid=X3VM_NMID_SDP;
			op->fmid=X3VM_FMID_STRI;
			op->sc=3;
			op->Run=X3VM_Opc_ST64P_3RI;
			break;
		case 0x6:
			op->nmid=X3VM_NMID_FLH2D;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=1;
			op->Run=X3VM_Opc_FLD16TO64_3RI;
			break;
		case 0x7:
			op->nmid=X3VM_NMID_LDP;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=3;
			op->Run=X3VM_Opc_LD64P_3RI;
			break;

		case 0x8:
			op->nmid=X3VM_NMID_LBU;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=0;
			op->Run=X3VM_Opc_LDU8_3RI;
			break;
		case 0x9:
			op->nmid=X3VM_NMID_LHU;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=1;
			op->Run=X3VM_Opc_LDU16_3RI;
			break;
		case 0xA:
			op->nmid=X3VM_NMID_LWU;
			op->fmid=X3VM_FMID_LDRI;
			op->sc=2;
			op->Run=X3VM_Opc_LDU32_3RI;
			break;
		case 0xB:
			break;

		case 0xC:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BTSTF;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BTSTF_3RI;
			break;
		case 0xD:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BGE;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BGE_3RI;
			break;
		case 0xE:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BGEU;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BGEU_3RI;
			break;
		case 0xF:
			op->rs=rs;
			op->rt=rn;
			op->imm=imm10s<<2;
			op->opfl|=X3VM_OPFL_CTRLF;
			op->nmid=X3VM_NMID_BNE;
			op->fmid=X3VM_FMID_J3RI;
			op->Run=X3VM_Opc_BNE_3RI;
			break;
		}
		break;

	/* F2 Block */
	case 0x2:	/* iiii-iiiiii-mmmmmm-zzzz-nnnnnn-00-0110 */
	case 0xA:	/* iiii-iiiiii-mmmmmm-zzzz-nnnnnn-10-0110 */
		op->imm=imm10u;
		switch((opw>>12)&15)
		{
		case 0x0:
			op->imm=imm10u;
			op->nmid=X3VM_NMID_ADD;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_ADD_3RI;
			break;
		case 0x1:
			op->imm=imm10n;
			op->nmid=X3VM_NMID_ADD;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_ADD_3RI;
			break;
		case 0x2:
			op->imm=imm10u;
			op->nmid=X3VM_NMID_MULW;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_MULW_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_MULWU;
				op->Run=X3VM_Opc_MULWU_3RI;
			}
			break;
		case 0x3:
			op->imm=imm10u;
			op->nmid=X3VM_NMID_ADDW;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_ADDW_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_ADDWU;
				op->Run=X3VM_Opc_ADDWU_3RI;
			}
			break;
		case 0x4:
			op->imm=imm10n;
			op->nmid=X3VM_NMID_ADDW;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_ADDW_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_ADDWU;
				op->Run=X3VM_Opc_ADDWU_3RI;
			}
			break;
		case 0x5:
			op->imm=imm10s;
			op->nmid=X3VM_NMID_AND;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_AND_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_RSUB;
				op->Run=X3VM_Opc_RSUB_3RI;
			}
			break;
		case 0x6:
			op->imm=imm10u;
			op->nmid=X3VM_NMID_OR;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_OR_3RI;
			if(eq)
			{
				if(rn<2)
				{
					op->nmid=X3VM_NMID_JALR;
					op->Run=X3VM_Opc_JALR_3RI;
					op->opfl|=X3VM_OPFL_CTRLF;
					break;
				}
				op->nmid=X3VM_NMID_SLAX;
				op->Run=X3VM_Opc_SLAX_3RI;
			}
			break;
		case 0x7:
			op->imm=imm10u;
			op->nmid=X3VM_NMID_XOR;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_XOR_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_SLLX;
				op->Run=X3VM_Opc_SLLX_3RI;
			}
			break;
		case 0x8:
//			if((imm10u>>8)==1)
			if(((imm10u>>8)==1) || (imm>>16))
			{
//				op->imm=(byte)imm10u;
				op->imm=imm10u;
				op->fmid=X3VM_FMID_3RI;
				if(eq)
				{
					op->nmid=X3VM_NMID_PSHUFH;
					op->Run=X3VM_Opc_PSHUFH_3RI;
				}else
				{
					op->nmid=X3VM_NMID_PSHUFB;
					op->Run=X3VM_Opc_PSHUFB_3RI;
				}
				break;
			}
			if((imm10u>>8)==3)
			{
				op->imm=(byte)imm10u;
				op->fmid=X3VM_FMID_3RI;
				if(eq)
				{
					op->nmid=X3VM_NMID_PMULTH;
					op->Run=X3VM_Opc_PMULTH_3RI;
				}
				break;
			}

			op->imm=(sbyte)imm10u;
			op->nmid=X3VM_NMID_SLAW;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_SLAW_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_SLA;
				op->Run=X3VM_Opc_SLA_3RI;
			}
			break;
		case 0x9:
			op->imm=(sbyte)imm10u;
			op->nmid=X3VM_NMID_SLLW;
			op->fmid=X3VM_FMID_3RI;
			op->Run=X3VM_Opc_SLLW_3RI;
			if(eq)
			{
				op->nmid=X3VM_NMID_SLL;
				op->Run=X3VM_Opc_SLL_3RI;
			}
			break;
		}
		break;

	/* F8 Block */
	case 0x4:	/* iiii-iiiiii-iiiiii-zzzz-nnnnnn-01-0010 */
	case 0xC:	/* iiii-iiiiii-iiiiii-zzzz-nnnnnn-11-0010 */
		op->imm=imm16u;
		switch((opw>>12)&15)
		{
		case 0x0:
			op->imm=imm16u;
			op->nmid=X3VM_NMID_LI;
			op->fmid=X3VM_FMID_2RI;
			op->rs=0;
//			op->Run=X3VM_Opc_ADD_3RI;
			op->Run=X3VM_Opc_MOV_2RI;
			break;
		case 0x1:
			if(jbits)
			{
				break;
			}
			op->imm=imm16n;
			op->nmid=X3VM_NMID_LI;
			op->fmid=X3VM_FMID_2RI;
			op->rs=0;
//			op->Run=X3VM_Opc_ADD_3RI;
			op->Run=X3VM_Opc_MOV_2RI;
			break;
		case 0x2:
			op->imm=imm16s;
			op->nmid=X3VM_NMID_ADD;
			op->fmid=X3VM_FMID_2RI;
			op->rs=rn;
			op->Run=X3VM_Opc_ADD_3RI;
			break;
		case 0x3:
			if(jbits)
			{
				op->imm=imm16u;
				op->nmid=X3VM_NMID_SHORI32;
				op->fmid=X3VM_FMID_2RI;
				op->rs=rn;
				op->Run=X3VM_Opc_SHORI32_3RI;
				if(eq)
				{
					op->imm=((u64)imm16u)<<32;
					op->nmid=X3VM_NMID_LI;
					op->Run=X3VM_Opc_MOV_2RI;
				}
				break;
			}
			op->imm=imm16u;
			op->nmid=X3VM_NMID_SHORI;
			op->fmid=X3VM_FMID_2RI;
			op->rs=rn;
			op->Run=X3VM_Opc_SHORI_3RI;
			break;
		case 0x4:
			if(jbits)
			{
				op->fmid=X3VM_FMID_2RI;
				op->nmid=X3VM_NMID_LI;
				op->Run=X3VM_Opc_MOV_2RI;
				op->imm=X3VM_ConvFp32ToFp64(imm16u);
				if(eq)
					{ op->imm=X3VM_Conv2xFp16To2xFp32(imm16u); }
				break;
			}
			if(rn==2)
			{
				op->imm=imm16u;
				op->nmid=X3VM_NMID_EBREAK;
				op->Run=X3VM_Opc_EBREAK_NONE;
				op->opfl|=X3VM_OPFL_CTRLF;
				break;
			}
//			op->imm=imm16u;
			op->imm=X3VM_ConvFp16ToFp64(imm16u);
//			op->nmid=X3VM_NMID_FLDCH;
			op->nmid=X3VM_NMID_LI;
			op->fmid=X3VM_FMID_2RI;
			op->rs=rn;
//			op->Run=X3VM_Opc_FLDCH_2RI;
			op->Run=X3VM_Opc_MOV_2RI;
			break;
		case 0x5:
			if(jbits)
			{
				break;
			}
			op->imm=imm16u;
			op->nmid=X3VM_NMID_LEA;
			op->fmid=X3VM_FMID_2RI;
			op->rs=3;
			op->sc=3;
			op->Run=X3VM_Opc_LEA_3RI;
			if(eq)
			{
				op->sc=2;
				op->nmid=X3VM_NMID_LWU;
				op->Run=X3VM_Opc_LDU32_3RI;
			}
			break;

		case 0xC:
			if(jbits)
			{
				break;
			}
			op->imm=imm16u;
			op->nmid=X3VM_NMID_SD;
			op->fmid=X3VM_FMID_STRI;
			op->rs=3;
			op->sc=3;
			op->Run=X3VM_Opc_ST64_3RI;
			if(eq)
			{
				op->sc=2;
				op->nmid=X3VM_NMID_SW;
				op->Run=X3VM_Opc_ST32_3RI;
			}
			break;
		case 0xD:
			if(jbits)
			{
				break;
			}
			op->imm=imm16u;
			op->nmid=X3VM_NMID_LD;
			op->fmid=X3VM_FMID_LDRI;
			op->rs=3;
			op->sc=3;
			op->Run=X3VM_Opc_LDS64_3RI;
			if(eq)
			{
				op->sc=2;
				op->nmid=X3VM_NMID_LW;
				op->Run=X3VM_Opc_LDS32_3RI;
			}
			break;
		}
		break;

	default:
		break;
	}

	if(	(op->fmid==X3VM_FMID_LDRI) ||
		(op->fmid==X3VM_FMID_STRI))
	{
		if(op->rs==0)
			{ op->rs=X3VM_REG_BPC; op->sc=0; }
		if(op->rs==1)
			{ op->rs=X3VM_REG_GBR; op->sc=0; }

		if(jwi)
		{
			op->sc=0;
		}
	}
	
	if(	(op->fmid==X3VM_FMID_LDRR) ||
		(op->fmid==X3VM_FMID_STRR))
	{
		if(op->rs==0)
			{ op->rs=X3VM_REG_BPC; op->sc=0; }
		if(op->rs==1)
			{ op->rs=X3VM_REG_GBR; op->sc=0; }
	}
	
	if(!op->Run)
	{
		op->Run=X3VM_Opc_INVOP_NONE;
		op->opfl|=X3VM_OPFL_CTRLF;
	}
	
	return(0);
}
