module DecOpF2(
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

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

reg[5:0]		opRegN;
reg[5:0]		opRegM;
reg[5:0]		opRegO;
reg[32:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

reg[5:0]	opRegO_Df2;
reg[5:0]	opRegN_Df2;
reg[5:0]	opRegM_Df2;

reg[5:0]	opRegN_Jcmp;
reg[5:0]	opRegM_Jcmp;

reg[32:0]		opImm_imm17s;
reg[32:0]		opImm_imm17u;
reg[32:0]		opImm_imm17n;

reg[32:0]		opImm_imm10s;
reg[32:0]		opImm_imm10u;
reg[32:0]		opImm_imm10n;

reg[32:0]		opImm_disp8s;

reg[32:0]		opImm_immWordZx;
reg[32:0]		opImm_immWordNx;

reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[1:0]	opCcty;

reg[7:0]	opUCmdIx;

reg		opIsImm32;
reg		opIsImm16;
reg		opIsNotFx;

reg		opExQ2;
reg		opExN2;
reg		opExM2;
reg		opExI2;

reg tRegRm2IsRz;
reg tRegRn2IsRz;
reg tRegRm2IsR0;
reg tRegRm2IsR1;
reg tRegRn2IsR0;
reg tRegRn2IsR1;

reg tRegRm2IsRs;
reg tRegRn2IsRs;
reg tRegRo2IsRs;

reg	tMsgLatch;
reg	tNextMsgLatch;

always @*
begin
	tRegRn2IsRs	= (istrWord[7:5]==3'b000) || (istrWord[7:4]==4'b1111);
	tRegRm2IsRs	= (istrWord[3:1]==3'b000) || (istrWord[3:0]==4'b1111);
	tRegRo2IsRs	= tRegRm2IsRs;

	opExQ2		= istrWord[27];
	opExN2		= istrWord[26];
	opExM2		= istrWord[25];
	opExI2		= istrWord[24];

	opRegO_Df2	= {tRegRo2IsRs, istrWord[ 4], istrWord[3:0]};
	opRegN_Df2	= {tRegRn2IsRs, istrWord[26], istrWord[7:4]};
	opRegM_Df2	= {tRegRm2IsRs, istrWord[25], istrWord[3:0]};

	opRegN_Jcmp	= opRegN_Df2;
	opRegM_Jcmp	= opRegM_Df2;

	opIsNotFx	= (istrWord[15:12]!=4'b1111);
	
	tNextMsgLatch	= 0;
	opIsImm16		= 0;

	opImm_imm17u	= {UV24_00, istrWord[24:16]};
	opImm_imm17n	= {UV24_FF, istrWord[24:16]};
	opImm_imm17s	= opImm_imm17u;

	opImm_imm10u	= {UV23_00, istrWord[25:16]};
	opImm_imm10n	= {UV23_FF, istrWord[25:16]};
	opImm_imm10s	= {istrWord[25]?UV23_FF:UV23_00, istrWord[25:16]};

	opImm_immWordZx	={UV17_00, istrWord[31:16]};
	opImm_immWordNx	={UV17_FF, istrWord[31:16]};

//	opImm_disp8s	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
	opImm_disp8s	= {istrWord[7]?UV25_FF:UV25_00, istrWord[23:16]};

	tRegRn2IsRz	= (opRegN_Df2[4:1]==4'b0000);
	tRegRm2IsRz	= (opRegM_Df2[4:1]==4'b0000);
	tRegRn2IsR0	= tRegRn2IsRz && !opRegN_Df2[0];
	tRegRn2IsR1	= tRegRn2IsRz &&  opRegN_Df2[0];
	tRegRm2IsR0	= tRegRm2IsRz && !opRegM_Df2[0];
	tRegRm2IsR1	= tRegRm2IsRz &&  opRegM_Df2[0];

	opUCmd		= JX2_UCMD_INVOP;
	opRegN		= JX2_REG_ZZR;
	opRegM		= JX2_REG_ZZR;
	opRegO		= JX2_REG_ZZR;
	opImm		= 0;
	opFmid		= JX2_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;
	opIsImm32	= 0;
	opIty		= 0;
	opCcty		= JX2_IXC_AL;

	casez(istrWord[11:8])
		4'b0z01: begin		/* F1nm_Xedd */

			case(istrWord[31:28])
				4'h0: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVB_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h1: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVW_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'h2: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVL_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'h3: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVQ_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SQ;
				end

				4'h8: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUB_MR : JX2_UCMD_MOVB_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h9: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUW_MR : JX2_UCMD_MOVW_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'hA: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVL_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'hB: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVQ_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= opExQ2 ? JX2_BTY_SL : JX2_BTY_SQ;
				end

`ifdef JX2_EX_ALU_JMPCC
				4'hC: begin		/* F0dd_Cenm */
					opUCmd		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b11, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
				4'hD: begin		/* F0dd_Denm */
					opUCmd		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b00, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
				4'hE: begin		/* F0dd_Eenm */
					opUCmd		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b10, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
				4'hF: begin		/* F0dd_Fenm */
					opUCmd		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b01, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
`endif

				default: begin
				end
			endcase

		end

		4'b0z10: begin		/* F2nm_Xedd */

			case(istrWord[31:28])
`ifdef def_true
				4'h0: begin		/* F2nm_0ejj */
					opUCmd	= JX2_UCMD_ALU_ADD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
				4'h1: begin		/* F2nm_1ejj */
					opUCmd	= JX2_UCMD_ALU_ADD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_NW;
				end
				4'h2: begin		/* F2nm_2ejj */
					opUCmd	= JX2_UCMD_ALU_MUL3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
`endif

`ifdef JX2_EX_ALU_ADDSL
				4'h3: begin		/* F1ej_Cjnm */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_ADDUL :
						JX2_UCMD_ALU_ADDSL;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
				4'h4: begin		/* F1ej_Djnm */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_ADDUL :
						JX2_UCMD_ALU_ADDSL;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_NW;
				end
`endif

`ifdef def_true
				4'h5: begin		/* F2nm_5ejj */
					opUCmd	= JX2_UCMD_ALU_AND3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
				4'h6: begin		/* F2nm_6ejj */
					opUCmd	= JX2_UCMD_ALU_OR3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
				4'h7: begin		/* F2nm_7ejj */
					opUCmd	= JX2_UCMD_ALU_XOR3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
`endif

`ifdef def_true
				4'h8: begin		/* F2nm_8ejj */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_SHADQ3 :
						JX2_UCMD_ALU_SHAD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
				4'h9: begin		/* F2nm_9ejj */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_SHLDQ3 :
						JX2_UCMD_ALU_SHLD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
`endif

`ifdef def_true
				4'hC: begin		/* F2nz_Cejj */
					opIty	= istrWord[0] ?
						JX2_ITY_NQ :
						JX2_ITY_UQ;
					opFmid = JX2_FMID_IMM8REG;

					casez(istrWord[3:0])
						4'b000z: begin
							opUCmd	= JX2_UCMD_MOV_IR;
						end

						4'b010z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_TSTQ :
								JX2_UCMD_ALU_TST;
						end
						4'b011z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQHS :
								JX2_UCMD_ALU_CMPHS;
						end
						4'b100z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQHI :
								JX2_UCMD_ALU_CMPHI;
						end
						4'b101z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQGE :
								JX2_UCMD_ALU_CMPGE;
						end
						4'b110z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQEQ :
								JX2_UCMD_ALU_CMPEQ;
						end
						4'b111z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQGT :
								JX2_UCMD_ALU_CMPGT;
						end

						default: begin
						end
					endcase
				end
`endif

				default: begin
				end
			endcase
		end

		4'h8: begin		/* F8zz_zzzz */
			casez(istrWord[7:5])
				3'b000: begin
					opUCmd		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
				end
				3'b001: begin
					opUCmd		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_NW;
				end
				3'b010: begin
					opUCmd		= JX2_UCMD_ALU_ADD3;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SW;
				end
				3'b011: begin
					opUCmd		= JX2_UCMD_ALU_LDISH16;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
				end

				3'b111: begin
					case(istrWord[4:0])
						5'h17: begin
							opUCmd		= JX2_UCMD_OP_IXT;
							opUCmdIx	= JX2_UCMD_IX_TRAPB;
							opFmid		= JX2_FMID_IMM8REG;
							opIty		= JX2_ITY_UW;
						end
						default: begin
						end
					endcase
				end

				default: begin
				end
			endcase
		end

		4'hA: begin		/* FAjj_jjjj */
			opUCmd	= JX2_UCMD_ALU_LDIX;
			opFmid	= JX2_FMID_IMM12Z;
		end
		4'hB: begin		/* FBjj_jjjj */
			opUCmd	= JX2_UCMD_ALU_LDIX;
			opFmid	= JX2_FMID_IMM12N;
		end

		default: begin
		end
	endcase
	
	if(opIsNotFx)
	begin
		opFmid = JX2_FMID_Z;
	end
	
	case(opFmid)
		JX2_FMID_Z: begin
		end

		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Df2;
			opRegM	= opRegM_Df2;
			opRegO	= JX2_REG_IMM;
			opUIxt	= opUCmdIx;
			opImm	= opImm_imm17s;

			case(opIty)
				JX2_ITY_SB: begin
				end
				JX2_ITY_SW: begin
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
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
			opUIxt	= opUCmdIx;

			opImm = {
				UV9_00,
				istrWord[23:16],
				istrWord[47:32] };
		end
		JX2_FMID_IMM8N: begin
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
			opUIxt	= opUCmdIx;

			opImm = {
				UV9_FF,
				istrWord[23:16],
				istrWord[47:32] };
		end

`ifdef def_true
		/*
			Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_REGSTREGDISP: begin
			opRegO	= JX2_REG_IMM;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, opBty};
			opImm	= opImm_imm17s;

			opRegN	= opRegM_Df2;
			opRegM	= opRegN_Df2;
			if(tRegRn2IsRz)
			begin
				opRegM	= tRegRn2IsR1 ? JX2_REG_GBR : JX2_REG_PC;
				opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
			end
		end

		/*
			SW: Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_LDREGDISPREG: begin
			opRegO	= JX2_REG_IMM;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};
			opImm	= opImm_imm17s;

			opRegN	= opRegN_Df2;
			opRegM	= opRegM_Df2;
			if(tRegRm2IsRz)
			begin
				opRegM	= tRegRm2IsR1 ? JX2_REG_GBR : JX2_REG_PC;
				opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, JX2_BTY_SB};
			end
		end
`endif

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
			opUIxt	= opUCmdIx;

			opRegM	= JX2_REG_IMM;
			opRegN	= opRegO_Df2;
			opRegO	= opRegO_Df2;
			
			case(opIty)
				JX2_ITY_SW:
					opImm = {
						istrWord[31] ? UV17_FF : UV17_00,
						istrWord[31:16] };

				JX2_ITY_SQ: begin
					opImm = opImm_imm10s;
					opRegN	= opRegN_Df2;
					opRegO	= opRegN_Df2;
				end

				JX2_ITY_UW:
					opImm = { UV17_00, istrWord[31:16] };

				JX2_ITY_UQ: begin
					opImm = opImm_imm10u;
					opRegN	= opRegN_Df2;
					opRegO	= opRegN_Df2;
				end

				JX2_ITY_NW:
					opImm = { UV17_FF, istrWord[31:16] };

				JX2_ITY_NQ: begin
					opImm = opImm_imm10n;
					opRegN	= opRegN_Df2;
					opRegO	= opRegN_Df2;
				end

				default: begin
					$display("Jx2DecOpFx: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
		end
		
		/*
			SB: PC, Ro, ZZR
			UB: Rm, #imm8s, Rn
		 */
		JX2_FMID_REGPC: begin
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= JX2_REG_ZZR;
					opRegM	= JX2_REG_PC;
					opRegO	= opRegO_Df2;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};
				end

`ifdef JX2_EX_ALU_JMPCC
				JX2_ITY_UB: begin
					opRegN	= opRegN_Jcmp;
					opRegM	= opRegM_Jcmp;
					opRegO	= JX2_REG_IMM;
					opImm	= opImm_disp8s;
					opUIxt	= opUCmdIx;
				end
`endif
				
				default: begin
					$display("Jx2DecOpFx: RegPc Bad Ity=%X", opIty);
				end
			endcase
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV9_00, istrWord[7:0], istrWord[31:16]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV9_FF, istrWord[7:0], istrWord[31:16]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end
		
		JX2_FMID_INV: begin
			if(!opIsNotFx)
			begin
				opUCmd = JX2_UCMD_INVOP;
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
			opUCmd = JX2_UCMD_INVOP;
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
