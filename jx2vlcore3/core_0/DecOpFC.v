module DecOpFC(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word

output[5:0]		idRegN;
output[5:0]		idRegM;
output[5:0]		idRegO;
output[32:0]	idImm;
output[7:0]		idUCmd;
output[7:0]		idUIxt;

reg[5:0]		opRegN;
reg[5:0]		opRegM;
reg[5:0]		opRegO;
reg[32:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

reg[5:0]	opRegO_Dfl;
reg[5:0]	opRegN_Dfl;
reg[5:0]	opRegM_Dfl;

reg[5:0]	opRegN_Cr;
reg[5:0]	opRegM_Cr;
reg[5:0]	opRegN_Sr;
reg[5:0]	opRegM_Sr;

reg[5:0]	opRegO_DfFC;
reg[5:0]	opRegP_DfFC;

reg[5:0]	opRegN_Jcmp;
reg[5:0]	opRegM_Jcmp;

reg[32:0]		opImm_imm17s;
reg[32:0]		opImm_imm17u;
reg[32:0]		opImm_imm17n;

reg[32:0]		opImm_imm10s;
reg[32:0]		opImm_imm10u;
reg[32:0]		opImm_imm10n;

reg[32:0]		opImm_disp20s;
reg[32:0]		opImm_disp8s;

reg[32:0]		opImm_immWordZx;
reg[32:0]		opImm_immWordNx;

reg[5:0]	opNmid;
reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[1:0]	opCcty;

reg[5:0]	opUCmdIx;

reg		opExQ;
reg		opExN;
reg		opExM;
reg		opExI;
reg		opIsImm32;
reg		opIsImm16;
reg		opIsFC0;
reg		opIsNotFx;

reg tRegRmIsRz;
reg tRegRnIsRz;
reg tRegRoIsRz;
reg tRegRmIsR0;
reg tRegRmIsR1;
reg tRegRnIsR0;
reg tRegRnIsR1;
reg tRegRoIsR0;
reg tRegRoIsR1;

reg	tMsgLatch;
reg	tNextMsgLatch;

always @*
begin

	opRegN_Cr	= {1'b1, istrWord[6], istrWord[23:20]};
	opRegM_Cr	= {1'b1, istrWord[5], istrWord[19:16]};

	opRegO_DfFC	= {1'b0, istrWord[8], istrWord[ 3: 0]};
	opRegP_DfFC	= {1'b0, istrWord[8], istrWord[27:24]};

	opIsFC0		= (istrWord[11:9]==3'b110);
	
	opIsNotFx	= (istrWord[15:12]!=4'b11x1);
	
	tNextMsgLatch	= 0;
	opIsImm16		= 0;

	opExQ		= istrWord[3];
	opExN		= istrWord[2];
	opExM		= istrWord[1];
	opExI		= istrWord[0];

	opRegO_Dfl	= JX2_GR_DLR;
	opRegN_Dfl	= {1'b0, istrWord[2], istrWord[23:20]};
	opRegM_Dfl	= {1'b0, istrWord[1], istrWord[19:16]};
	opRegN_Cr	= {1'b1, istrWord[2], istrWord[23:20]};
	opRegM_Cr	= {1'b1, istrWord[1], istrWord[19:16]};

	opImm_imm17s	= {istrWord[0]?UV17_FF:UV17_00, istrWord[47:32]};
	opImm_imm17u	= {UV16_00,        istrWord[0], istrWord[47:32]};
	opImm_imm17n	= {UV16_FF,        istrWord[0], istrWord[47:32]};

	opImm_imm10u	= opImm_imm17u;
	opImm_imm10n	= opImm_imm17n;
	opImm_imm10s	= opImm_imm17s;

	opImm_immWordZx	={1'b0, istrWord[47:16]};
	opImm_immWordNx	={1'b1, istrWord[47:16]};

	opRegN_Jcmp		= opRegN_Dfl;
	opRegM_Jcmp		= opRegM_Dfl;
	opImm_disp8s	= {istrWord[47]?UV17_FF:UV17_00, istrWord[47:32]};

	tRegRnIsRz	= (opRegN_Dfl[4:1]==4'b0000);
	tRegRmIsRz	= (opRegM_Dfl[4:1]==4'b0000);
	tRegRoIsRz	= (opRegO_Dfl[4:1]==4'b0000);
	tRegRnIsR0	= tRegRnIsRz && !opRegN_Dfl[0];
	tRegRnIsR1	= tRegRnIsRz &&  opRegN_Dfl[0];
	tRegRmIsR0	= tRegRmIsRz && !opRegM_Dfl[0];
	tRegRmIsR1	= tRegRmIsRz &&  opRegM_Dfl[0];
	tRegRoIsR0	= tRegRoIsRz && !opRegO_Dfl[0];
	tRegRoIsR1	= tRegRoIsRz &&  opRegO_Dfl[0];

	opNmid		= JX2_UCMD_INVOP;
	opRegN		= JX2_GR_ZZR;
	opRegM		= JX2_GR_ZZR;
	opRegO		= JX2_GR_ZZR;
	opImm		= 0;
	opFmid		= JX2_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;
	opIsImm32	= 0;
	opIty		= 0;
	opCcty		= JX2_IXC_AL;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[11:8])

		4'b110z: /* FCxx_xxxx_xxxx */
		casez(istrWord[7:0])
			8'h0z: begin /* FC0e_xxxx_xxxx */
				casez(istrWord[31:16])
					16'h00zz: begin		/* FC0e_00nm_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SB;
					end
					16'h01zz: begin		/* FC0e_01nm_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SW;
					end
					16'h02zz: begin		/* FC0e_02nm_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SL;
					end
					16'h03zz: begin		/* FC0e_03nm_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SQ;
					end

					16'h08zz: begin		/* FC0e_08nm_dddd */
//						opUCmd	= opExQ ? JX2_UCMD_MOVUB_MR : JX2_UCMD_MOVB_MR;
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
						opIty	= JX2_ITY_SW;
//						opBty	= JX2_BTY_SB;
						opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
					end
					16'h09zz: begin		/* FC0e_09nm_dddd */
//						opUCmd	= opExQ ? JX2_UCMD_MOVUW_MR : JX2_UCMD_MOVW_MR;
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
						opIty	= JX2_ITY_SW;
//						opBty	= JX2_BTY_SW;
						opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
					end
					16'h0Azz: begin		/* FC0e_0Anm_dddd */
//						opUCmd	= opExQ ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVL_MR;
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
						opIty	= JX2_ITY_SW;
//						opBty	= JX2_BTY_SL;
						opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
					end
					16'h0Bzz: begin		/* FC0e_0Bnm_dddd */
//						opNmid	= JX2_UCMD_MOVQ_MR;
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SQ;
					end

					16'h10zz: begin		/* FC0e_10nm_iiii */
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_UW;
						opUCmdIx	= JX2_UCIX_ALU_ADD;
					end
					16'h11zz: begin		/* FC0e_11nm_iiii */
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_UW;
						opUCmdIx	= JX2_UCIX_ALU_SUB;
					end
					16'h12zz: begin		/* FC0e_12nm_iiii */
						opNmid		= JX2_UCMD_MUL3;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SW;
						opUCmdIx	= JX2_UCIX_MUL3_MUL3S;
					end

					16'h15zz: begin		/* FC0e_15nm_iiii */
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SW;
						opUCmdIx	= JX2_UCIX_ALU_AND;
					end
					16'h16zz: begin		/* FC0e_16nm_iiii */
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SW;
						opUCmdIx	= JX2_UCIX_ALU_OR;
					end
					16'h17zz: begin		/* FC0e_17nm_iiii */
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SW;
						opUCmdIx	= JX2_UCIX_ALU_XOR;
					end

					16'h19zz: begin		/* FC0e_19nz_iiii */
						case(istrWord[19:16])
							4'h4: begin
								opNmid		= JX2_UCMD_ALUCMP;
								opFmid		= JX2_FMID_IMM8REG;
								opIty		= JX2_ITY_SQ;
								opUCmdIx	=  opExQ ?
									JX2_UCIX_ALU_TSTQ :
									JX2_UCIX_ALU_TST;
							end

							4'hC: begin
								opNmid		= JX2_UCMD_ALUCMP;
								opFmid		= JX2_FMID_IMM8REG;
								opIty		= JX2_ITY_SQ;
								opUCmdIx	=  opExQ ?
									JX2_UCIX_ALU_CMPQEQ :
									JX2_UCIX_ALU_CMPEQ;
							end
							4'hD: begin
								opNmid		= JX2_UCMD_ALUCMP;
								opFmid		= JX2_FMID_IMM8REG;
								opIty		= JX2_ITY_SQ;
								opUCmdIx	=  opExQ ?
									JX2_UCIX_ALU_CMPQHI :
									JX2_UCIX_ALU_CMPHI;
							end
							4'hE: begin
								opNmid		= JX2_UCMD_ALUCMP;
								opFmid		= JX2_FMID_IMM8REG;
								opIty		= JX2_ITY_SQ;
								opUCmdIx	=  opExQ ?
									JX2_UCIX_ALU_CMPQGT :
									JX2_UCIX_ALU_CMPGT;
							end
							
							default: begin
							end
						endcase
					end
				endcase
			end

			8'h20: begin
				opNmid	= JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
			end
			8'h21: begin
				opNmid	= JX2_UCMD_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
			end
			8'h22: begin
				opNmid	= JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
				opCcty	= JX2_IXC_CT;
			end
			8'h23: begin
				opNmid	= JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
				opCcty	= JX2_IXC_CF;
			end
			8'h24: begin
				opNmid		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8Z;
				opUCmdIx	= JX2_UCIX_LDI_LDIX;
			end
			8'h25: begin
				opNmid		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8N;
				opUCmdIx	= JX2_UCIX_LDI_LDIX;
			end
			8'h26: begin
				opNmid		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8Z;
				opUCmdIx	= JX2_UCIX_LDI_LDISH32;
			end

			8'h27: begin 	/* FC27_xxxx_xxxx */
				case(istrWord[31:28])
					4'h0: begin 	/* FC27_0ndd_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SB;
					end
					4'h1: begin 	/* FC27_1ndd_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SW;
					end
					4'h2: begin 	/* FC27_2ndd_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SL;
					end
					4'h3: begin 	/* FC27_3ndd_dddd */
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SQ;
					end
					4'h4: begin 	/* FC27_4ndd_dddd */
						opNmid	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end

					4'h8: begin 	/* FC27_8ndd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'h9: begin 	/* FC27_9ndd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SW;
					end
					4'hA: begin 	/* FC27_Andd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SL;
					end
					4'hB: begin 	/* FC27_Bndd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SQ;
					end
					4'hC: begin 	/* FC27_Cndd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_UB;
					end
					4'hD: begin 	/* FC27_Dndd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_UW;
					end
					4'hE: begin 	/* FC27_Endd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_UL;
					end
					4'hF: begin 	/* FC27_Fndd_dddd */
						opNmid	= JX2_UCMD_MOV_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_UL;
					end

					default: begin
					end
				endcase
			end
		
			8'hCz: begin /* FCCx_xxxx_xxxx */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_SL;
				opUCmdIx	= JX2_UCIX_ALU_ADD;
			end
			8'hDz: begin /* FCDx_xxxx_xxxx */
				opNmid		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_UL;
				opUCmdIx	= JX2_UCIX_LDI_LDISH32;
			end
			8'hEz: begin /* FCEx_xxxx_xxxx */
				opNmid		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_UL;
				opUCmdIx	= JX2_UCIX_LDI_LDIX;
			end
			8'hFz: begin /* FCFx_xxxx_xxxx */
				opNmid		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_NL;
				opUCmdIx	= JX2_UCIX_LDI_LDIX;
			end

			default: begin
			end

		endcase

		default: begin
		end
	endcase
	
	if(opIsNotFx)
	begin
		opFmid = JX2_FMID_Z;
	end

	opUCmd = { opCcty, opNmid };
	
	case(opFmid)
		JX2_FMID_Z: begin
		end

		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= { 2'b00, opUCmdIx };

			case(opIty)
				JX2_ITY_SB: begin
				end
				JX2_ITY_SW: begin
					opImm	= opImm_imm17s;
				end
				JX2_ITY_UW: begin
					opImm	= opImm_imm17u;
				end
				JX2_ITY_NW: begin
					opImm	= opImm_imm17n;
				end
				
				default: begin
					$display("Jx2DecOpFx: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end

		JX2_FMID_IMM8Z: begin
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
			opUIxt	= { 2'b00, opUCmdIx };
			opImm = opImm_immWordZx;
		end
		JX2_FMID_IMM8N: begin
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
			opUIxt	= { 2'b00, opUCmdIx };
			opImm = opImm_immWordNx;
		end

`ifdef def_true
		/*
			Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_REGSTREGDISP: begin
			opRegN	= opRegM_Dfl;
			opRegM	= opRegN_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b0, opBty};
			opImm	= opImm_imm17s;

			if(tRegRnIsRz)
			begin
				opRegM	= tRegRnIsR1 ? JX2_GR_GBR : JX2_GR_PC;
				opUIxt	= {opCcty, opBty[1:0], 1'b0, opBty[2], 2'b00};
			end
		end

		/*
			SW: Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_LDREGDISPREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			opImm	= opImm_imm17s;

			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
				opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty[2], 2'b00};
			end
		end
`endif

		/*
			FCXX_Xndd_dddd
		 */
		JX2_FMID_REGSTDRPC: begin
			opRegM	= JX2_GR_PC;
			opRegN	= opRegP_DfFC;

			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, opBty};
			opUIxt	= {opCcty, opBty[1:0], 1'b0, opBty};
			opImm = {
				istrWord[23] ? UV9_FF : UV9_00,
				istrWord[23:16],
				istrWord[47:32] };
		end

		/*
			FCXX_Xndd_dddd
		 */
		JX2_FMID_LDDRPCREG: begin
			opRegN	= opRegP_DfFC;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			opImm = {
				istrWord[23] ? UV9_FF : UV9_00,
				istrWord[23:16],
				istrWord[47:32] };
		end

		/*
			SB: Fzeo_zzii		Imm8s, Ro
			SW: Fzeo_iiii		Imm16s, Ro
			SL: Fzeo_iiii_iiii	Imm32s, Ro

			SB: Fzeo_zzjj		Imm8u, Ro
			SW: Fzeo_jjjj		Imm16u, Ro
			SL: Fzeo_jjjj_jjjj	Imm32u, Ro

			NB: Fzeo_zzjj		Imm8n, Ro
			NW: Fzeo_jjjj		Imm16n, Ro
			NL: Fzeo_jjjj_jjjj	Imm32n, Ro

			XB: Fzeo_zzjj		Imm8x, Ro
			XW: Fzeo_jjjj		Imm16x, Ro
			XL: Fzeo_jjjj_jjjj	Imm32x, Ro

			SQ: Fzze_zznz_iiii	Imm17s, Rn
			UQ: Fzze_zznz_iiii	Imm17u, Rn
			NQ: Fzze_zznz_iiii	Imm17n, Rn
		 */
	
		JX2_FMID_IMM8REG: begin
			opUIxt	= { 2'b00, opUCmdIx };

			if(opIsFC0)
			begin
				opRegM	= JX2_GR_IMM;
				opRegN	= opRegO_DfFC;
				opRegO	= opRegO_DfFC;
			end
			else
			begin
				opRegM	= JX2_GR_IMM;
				opRegN	= opRegO_Dfl;
				opRegO	= opRegO_Dfl;
			end
			
			case(opIty)
				JX2_ITY_SL:
					opImm = { istrWord[47], istrWord[47:16] };
				JX2_ITY_UL:
					opImm = {    1'b0, istrWord[47:16] };
				JX2_ITY_NL:
					opImm = {    1'b1, istrWord[47:16] };

				default: begin
					$display("Jx2DecOpFx: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
		end
		
		/*
			SL: Fzzz_dddd_dddd	(PC, disp32s), DLR
		 */
		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			opImm = { istrWord[47], istrWord[47:16] };
		end
		
		JX2_FMID_INV: begin
			if(!opIsNotFx)
			begin
				opUCmd = {opCcty, JX2_UCMD_INVOP};
				if(!tMsgLatch)
				begin
					$display("Jx2DecOpFx: Invalid FMID (32/48)");
					$display("Jx2DecOpFx: Istr %X-%X-%X",
						istrWord[15:0], istrWord[31:16], istrWord[47:32]);
				end
				tNextMsgLatch=1;
			end
		end
		
		default: begin
			opUCmd = {opCcty, JX2_UCMD_INVOP};
			if(!tMsgLatch)
			begin
				$display("Jx2DecOpFx: Unhandled FMID (32/48) %X", opFmid);
				$display("Jx2DecOpFx: Istr %X-%X-%X",
					istrWord[15:0], istrWord[31:16], istrWord[47:32]);
			end
			tNextMsgLatch=1;
		end
	endcase
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule
