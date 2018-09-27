module Jx2DecOpFx(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[47:0]		istrWord;	//source instruction word

output[6:0]		idRegN;
output[6:0]		idRegM;
output[6:0]		idRegO;
output[32:0]	idImm;
output[7:0]		idUCmd;
output[7:0]		idUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

reg[6:0]		opRegN;
reg[6:0]		opRegM;
reg[6:0]		opRegO;
reg[32:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

reg[6:0]	opRegO_Dfl;
reg[6:0]	opRegN_Dfl;
reg[6:0]	opRegM_Dfl;

reg[6:0]	opRegN_Cr;
reg[6:0]	opRegM_Cr;
reg[6:0]	opRegN_Sr;
reg[6:0]	opRegM_Sr;

reg[6:0]	opRegO_DfFC;
reg[6:0]	opRegP_DfFC;

reg[32:0]		opImm_imm17s;
reg[32:0]		opImm_imm17u;

reg[4:0]	opFmid;
reg[2:0]	opBty;

reg[3:0]	opIty;

reg[7:0]	opUCmdIx;

reg		opExQ;
reg		opExN;
reg		opExM;
reg		opExI;
reg		opIsImm32;
reg		opIsImm16;
reg		opIsFC0;

always @*
begin

	opExQ		= istrWord[7];
	opExN		= istrWord[6];
	opExM		= istrWord[5];
	opExI		= istrWord[4];

	opRegO_Dfl	= {2'b00, istrWord[4], istrWord[ 3: 0]};
	opRegN_Dfl	= {2'b00, istrWord[6], istrWord[23:20]};
	opRegM_Dfl	= {2'b00, istrWord[5], istrWord[19:16]};
	opRegN_Cr	= {2'b01, istrWord[6], istrWord[23:20]};
	opRegM_Cr	= {2'b01, istrWord[5], istrWord[19:16]};
	opRegN_Sr	= {2'b10, istrWord[6], istrWord[23:20]};
	opRegM_Sr	= {2'b10, istrWord[5], istrWord[19:16]};

	opRegO_DfFC	= opRegO_Dfl;
	opRegP_DfFC	= {2'b00, istrWord[8], istrWord[27:24]};

	opIsFC0		= (istrWord[11:9]==3'b110);

//	opImm_imm17s	= 0;
//	opImm_imm17u	= 0;

	opImm_imm17u	= {UV24_00, istrWord[4:0], istrWord[27:24]};
	opImm_imm17s	= opImm_imm17u;

	/*
		FCze/FDze block moves 'e' and loses Ro in typical case.
		But, typical cases have 17-bit immediate values.
	 */
	if(opIsFC0)
	begin
		opExQ		= istrWord[3];
		opExN		= istrWord[2];
		opExM		= istrWord[1];
		opExI		= istrWord[0];

		opRegO_Dfl	= JX2_REG_DLR;
		opRegN_Dfl	= {2'b00, istrWord[2], istrWord[23:20]};
		opRegM_Dfl	= {2'b00, istrWord[1], istrWord[19:16]};
		opRegN_Cr	= {2'b01, istrWord[2], istrWord[23:20]};
		opRegM_Cr	= {2'b01, istrWord[1], istrWord[19:16]};
		opRegN_Sr	= {2'b10, istrWord[2], istrWord[23:20]};
		opRegM_Sr	= {2'b10, istrWord[1], istrWord[19:16]};

		opImm_imm17s	= {istrWord[0]?UV17_FF:UV17_00, istrWord[47:32]};
		opImm_imm17u	= {UV16_00,        istrWord[0], istrWord[47:32]};
	end

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

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[11:8])

		4'h0:
		casez(istrWord[31:16])
			16'h00zz: begin		/* F0ez_00zz */
				opUCmd	= JX2_UCMD_MOVB_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h01zz: begin		/* F0ez_01zz */
				opUCmd	= JX2_UCMD_MOVW_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h02zz: begin		/* F0ez_02zz */
				opUCmd	= JX2_UCMD_MOVL_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h03zz: begin		/* F0ez_03zz */
				opUCmd	= JX2_UCMD_MOVQ_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h04zz: begin		/* F0ez_04zz */
				opUCmd	= JX2_UCMD_MOVB_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h05zz: begin		/* F0ez_05zz */
				opUCmd	= JX2_UCMD_MOVW_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h06zz: begin		/* F0ez_06zz */
				opUCmd	= JX2_UCMD_MOVL_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h07zz: begin		/* F0ez_07zz */
				opUCmd	= JX2_UCMD_MOVQ_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h08zz: begin		/* F0ez_08zz */
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUB_MR;
				else
					opUCmd	= JX2_UCMD_MOVB_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h09zz: begin		/* F0ez_09zz */
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUW_MR;
				else
					opUCmd	= JX2_UCMD_MOVW_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0Azz: begin		/* F0ez_0Azz */
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUL_MR;
				else
					opUCmd	= JX2_UCMD_MOVL_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0Bzz: begin		/* F0ez_0Bzz */
				opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h0Czz: begin		/* F0ez_0Czz */
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUB_MR;
				else
					opUCmd	= JX2_UCMD_MOVB_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h0Dzz: begin		/* F0ez_0Dzz */
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUW_MR;
				else
					opUCmd	= JX2_UCMD_MOVW_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0Ezz: begin		/* F0ez_0Ezz */
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUL_MR;
				else
					opUCmd	= JX2_UCMD_MOVL_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0Fzz: begin		/* F0ez_0Fzz */
				opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h10zz: begin		/* F0ez_10zz */
				opUCmd	= JX2_UCMD_ALU_ADD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_UB;
			end
			16'h11zz: begin		/* F0ez_11zz */
				opUCmd	= JX2_UCMD_ALU_SUB3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_UB;
			end
			16'h12zz: begin		/* F0ez_12zz */
				opUCmd	= JX2_UCMD_ALU_MUL3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_UB;
			end

			16'h15zz: begin		/* F0ez_15zz */
				opUCmd	= JX2_UCMD_ALU_AND3;
				opFmid	= JX2_FMID_REGREG;
			end
			16'h16zz: begin		/* F0ez_16zz */
				opUCmd	= JX2_UCMD_ALU_OR3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_UB;
			end
			16'h17zz: begin		/* F0ez_17zz */
				opUCmd	= JX2_UCMD_ALU_XOR3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_UB;
			end

			16'h18zz: begin	/* F0ez_18nm */
/*
				case(istrWord[3:0])
					4'h0: begin
						opUCmd	= JX2_UCMD_MOVB_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opBty	= JX2_BTY_SB;
					end
					4'h1: begin
						opUCmd	= JX2_UCMD_MOVW_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opBty	= JX2_BTY_SW;
					end
					4'h2: begin
						opUCmd	= JX2_UCMD_MOVL_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opBty	= JX2_BTY_SL;
					end
					4'h3: begin
						opUCmd	= JX2_UCMD_MOVQ_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opBty	= JX2_BTY_SQ;
					end

					4'h4: begin
						opUCmd	= JX2_UCMD_MOVB_RM;
						opFmid	= JX2_FMID_REGSTDRREG;
						opBty	= JX2_BTY_SB;
					end
					4'h5: begin
						opUCmd	= JX2_UCMD_MOVW_RM;
						opFmid	= JX2_FMID_REGSTDRREG;
						opBty	= JX2_BTY_SW;
					end
					4'h6: begin
						opUCmd	= JX2_UCMD_MOVL_RM;
						opFmid	= JX2_FMID_REGSTDRREG;
						opBty	= JX2_BTY_SL;
					end
					4'h7: begin
						opUCmd	= JX2_UCMD_MOVQ_RM;
						opFmid	= JX2_FMID_REGSTDRREG;
						opBty	= JX2_BTY_SQ;
					end

					4'h8: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_MOVUB_MR;
						else
							opUCmd	= JX2_UCMD_MOVB_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SB;
					end
					4'h9: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_MOVUW_MR;
						else
							opUCmd	= JX2_UCMD_MOVW_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SW;
					end
					4'hA: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_MOVUL_MR;
						else
							opUCmd	= JX2_UCMD_MOVL_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SL;
					end
					4'hB: begin
						opUCmd	= JX2_UCMD_MOVQ_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SQ;
					end

					4'hC: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_MOVUB_MR;
						else
							opUCmd	= JX2_UCMD_MOVB_MR;
						opFmid	= JX2_FMID_LDDRREGREG;
						opBty	= JX2_BTY_SB;
					end
					4'hD: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_MOVUW_MR;
						else
							opUCmd	= JX2_UCMD_MOVW_MR;
						opFmid	= JX2_FMID_LDDRREGREG;
						opBty	= JX2_BTY_SW;
					end
					4'hE: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_MOVUL_MR;
						else
							opUCmd	= JX2_UCMD_MOVL_MR;
						opFmid	= JX2_FMID_LDDRREGREG;
						opBty	= JX2_BTY_SL;
					end
					4'hF: begin
						opUCmd	= JX2_UCMD_MOVQ_MR;
						opFmid	= JX2_FMID_LDDRREGREG;
						opBty	= JX2_BTY_SQ;
					end
				endcase
*/
			end

			16'h19zz: begin	/* F0ez_19nm */
				case(istrWord[3:0])
					4'h0: begin
						opUCmd	= JX2_UCMD_ALU_ADD3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h1: begin
						opUCmd	= JX2_UCMD_ALU_SUB3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h2: begin
						opUCmd	= JX2_UCMD_ALU_ADC3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h3: begin
						opUCmd	= JX2_UCMD_ALU_SBB3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h4: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_TSTQ;
						else
							opUCmd	= JX2_UCMD_ALU_TST;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_SB;
					end
					4'h5: begin
						opUCmd	= JX2_UCMD_ALU_AND3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h6: begin
						opUCmd	= JX2_UCMD_ALU_OR3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h7: begin
						opUCmd	= JX2_UCMD_ALU_XOR3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h8: begin
						opUCmd	= JX2_UCMD_MOV_RR;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h9: begin
						opUCmd	= JX2_UCMD_ALU_MULS;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hA: begin
						opUCmd	= JX2_UCMD_MOV_RC;
						opFmid	= JX2_FMID_REGREG;
						opBty	= JX2_BTY_UB;
					end
					4'hB: begin
						opUCmd	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opBty	= JX2_BTY_UW;
					end
					4'hC: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_CMPQEQ;
						else
							opUCmd	= JX2_UCMD_ALU_CMPEQ;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hD: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_CMPQHI;
						else
							opUCmd	= JX2_UCMD_ALU_CMPHI;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hE: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_CMPQGT;
						else
							opUCmd	= JX2_UCMD_ALU_CMPGT;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hF: begin
						opUCmd	= JX2_UCMD_ALU_MULU;
						opFmid	= JX2_FMID_REGREG;
					end
				endcase
			end

			16'h1Dzz: begin	/* F0ez_1Dnm */
				case(istrWord[3:0])
/*
					4'h8: begin
						opUCmd	= JX2_UCMD_ALU_ADD2;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h9: begin
						opUCmd	= JX2_UCMD_ALU_SUB2;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hA: begin
						opUCmd	= JX2_UCMD_ALU_AND2;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hB: begin
						opUCmd	= JX2_UCMD_ALU_OR2;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hC: begin
						opUCmd	= JX2_UCMD_ALU_XOR2;
						opFmid	= JX2_FMID_REGREG;
					end
*/
					
					default: begin
					end
				endcase
			end

			16'h20zz: begin		/* F0ez_20zz */
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
			end
			16'h21zz: begin		/* F0ez_21zz */
				opUCmd	= JX2_UCMD_CF_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
			end
			16'h22zz: begin		/* F0ez_22zz */
				opUCmd	= JX2_UCMD_CF_BT;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
			end
			16'h23zz: begin		/* F0ez_23zz */
				opUCmd	= JX2_UCMD_CF_BF;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
			end

			16'h24zz: begin
				opUCmd	= JX2_UCMD_ALU_LDIX;
				opFmid	= JX2_FMID_IMM8Z;
			end
			16'h25zz: begin
				opUCmd	= JX2_UCMD_ALU_LDIX;
				opFmid	= JX2_FMID_IMM8N;
			end
			16'h26zz: begin
				opUCmd	= JX2_UCMD_ALU_LDISH;
				opFmid	= JX2_FMID_IMM8Z;
			end

			16'h27zz: begin

			end

			16'h28zz: begin
				opUCmd	= JX2_UCMD_MOVL_RM;
				opFmid	= JX2_FMID_REGSTDI4SP;
				opBty	= JX2_BTY_SL;
			end
			16'h29zz: begin
				opUCmd	= JX2_UCMD_MOVQ_RM;
				opFmid	= JX2_FMID_REGSTDI4SP;
				opBty	= JX2_BTY_SQ;
			end
			16'h2Azz: begin
				opUCmd	= JX2_UCMD_MOVL_MR;
				opFmid	= JX2_FMID_LDDI4SPREG;
				opBty	= JX2_BTY_SL;
			end
			16'h2Bzz: begin
				opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDDI4SPREG;
				opBty	= JX2_BTY_SQ;
			end
			16'h2Czz: begin
				opUCmd	= JX2_UCMD_ALU_CMPEQ;
				opFmid	= JX2_FMID_IMM4ZREG;
			end
			16'h2Dzz: begin
				opUCmd	= JX2_UCMD_ALU_CMPEQ;
				opFmid	= JX2_FMID_IMM4NREG;
			end
			16'h2Ezz: begin
				opUCmd	= JX2_UCMD_ALU_CMPGT;
				opFmid	= JX2_FMID_IMM4ZREG;
			end
			16'h2Fzz: begin
				opUCmd	= JX2_UCMD_ALU_CMPGE;
				opFmid	= JX2_FMID_IMM4ZREG;
			end



			16'h4Czz: begin		/* F0eo_4Cnm */
				opUCmd	= JX2_UCMD_LEA_MR;
				if(opExQ)	opFmid	= JX2_FMID_LDREGREG;
				else		opFmid	= JX2_FMID_LDDRREGREG;
				opIty	= JX2_ITY_UB;
				opBty	= JX2_BTY_SB;
			end
			16'h4Dzz: begin		/* F0eo_4Dnm */
				opUCmd	= JX2_UCMD_LEA_MR;
				if(opExQ)	opFmid	= JX2_FMID_LDREGREG;
				else		opFmid	= JX2_FMID_LDDRREGREG;
				opIty	= JX2_ITY_UB;
				opBty	= JX2_BTY_SW;
			end
			16'h4Ezz: begin		/* F0eo_4Enm */
				opUCmd	= JX2_UCMD_LEA_MR;
				if(opExQ)	opFmid	= JX2_FMID_LDREGREG;
				else		opFmid	= JX2_FMID_LDDRREGREG;
				opIty	= JX2_ITY_UB;
				opBty	= JX2_BTY_SL;
			end
			16'h4Fzz: begin		/* F0eo_4Fnm */
				opUCmd	= JX2_UCMD_LEA_MR;
				if(opExQ)	opFmid	= JX2_FMID_LDREGREG;
				else		opFmid	= JX2_FMID_LDDRREGREG;
				opIty	= JX2_ITY_UB;
				opBty	= JX2_BTY_SQ;
			end

			16'h50zz: begin		/* F0ez_50zz */
				opUCmd	= JX2_UCMD_ALU_CSELT;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_UB;
			end

			16'h54zz: begin		/* F0ez_54zz */
				opUCmd	= opExQ ? JX2_UCMD_ALU_SHADQ3 : JX2_UCMD_ALU_SHAD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h55zz: begin		/* F0ez_55zz */
				opUCmd	= opExQ ? JX2_UCMD_ALU_SHLDQ3 : JX2_UCMD_ALU_SHLD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h56zz: begin		/* F0ez_56zz */
				opUCmd	= JX2_UCMD_ALU_SHAD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_XB;
			end
			16'h57zz: begin		/* F0ez_57zz */
				opUCmd	= JX2_UCMD_ALU_SHLD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_XB;
			end
			16'h58zz: begin		/* F0ez_58zz */
				opUCmd	= JX2_UCMD_ALU_SHADQ3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_XB;
			end
			16'h59zz: begin		/* F0ez_59zz */
				opUCmd	= JX2_UCMD_ALU_SHLDQ3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_XB;
			end
			16'h5Azz: begin		/* F0ez_5Azz */
				opUCmd	= JX2_UCMD_ALU_SHADQ3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_XW;
			end
			16'h5Bzz: begin		/* F0ez_5Bzz */
				opUCmd	= JX2_UCMD_ALU_SHLDQ3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_XW;
			end

			default: begin
			end
		endcase

		4'h1: begin		/* F1ed_Xdnm */

			case(istrWord[15:12])
				4'h0: begin
					opUCmd	= JX2_UCMD_MOVB_RM;
					opFmid	= JX2_FMID_REGSTREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h1: begin
					opUCmd	= JX2_UCMD_MOVW_RM;
					opFmid	= JX2_FMID_REGSTREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'h2: begin
					opUCmd	= JX2_UCMD_MOVL_RM;
					opFmid	= JX2_FMID_REGSTREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'h3: begin
					opUCmd	= JX2_UCMD_MOVQ_RM;
					opFmid	= JX2_FMID_REGSTREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SQ;
				end

				4'h8: begin
					opUCmd	= opExQ ? JX2_UCMD_MOVUB_MR : JX2_UCMD_MOVB_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h9: begin
					opUCmd	= opExQ ? JX2_UCMD_MOVUW_MR : JX2_UCMD_MOVW_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'hA: begin
					opUCmd	= opExQ ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVL_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'hB: begin
					opUCmd	= JX2_UCMD_MOVQ_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SQ;
				end

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
					opIsImm16	= 1;
				end
				3'b001: begin
					opUCmd		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_NW;
					opIsImm16	= 1;
				end
				3'b010: begin
					opUCmd		= JX2_UCMD_ALU_ADD3;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SW;
					opIsImm16	= 1;
				end
				3'b011: begin
					opUCmd		= JX2_UCMD_ALU_LDISH16;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opIsImm16	= 1;
				end
				default: begin
				end
			endcase
		end

		4'b110z: /* FCxx_xxxx_xxxx */
		casez(istrWord[7:0])
			8'h0z: begin /* FC0e_xxxx_xxxx */
//				opIsFC0		= 1;
				casez(istrWord[31:16])
					16'h00zz: begin		/* FC0e_00nm_dddd */
						opUCmd	= JX2_UCMD_MOVB_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SB;
					end
					16'h01zz: begin		/* FC0e_01nm_dddd */
						opUCmd	= JX2_UCMD_MOVW_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SW;
					end
					16'h02zz: begin		/* FC0e_02nm_dddd */
						opUCmd	= JX2_UCMD_MOVL_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SL;
					end
					16'h03zz: begin		/* FC0e_03nm_dddd */
						opUCmd	= JX2_UCMD_MOVQ_RM;
						opFmid	= JX2_FMID_REGSTREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SQ;
					end

					16'h08zz: begin		/* FC0e_08nm_dddd */
						opUCmd	= opExQ ? JX2_UCMD_MOVUB_MR : JX2_UCMD_MOVB_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SB;
					end
					16'h09zz: begin		/* FC0e_09nm_dddd */
						opUCmd	= opExQ ? JX2_UCMD_MOVUW_MR : JX2_UCMD_MOVW_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SW;
					end
					16'h0Azz: begin		/* FC0e_0Anm_dddd */
						opUCmd	= opExQ ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVL_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SL;
					end
					16'h0Bzz: begin		/* FC0e_0Bnm_dddd */
						opUCmd	= JX2_UCMD_MOVQ_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opIty	= JX2_ITY_SW;
						opBty	= JX2_BTY_SQ;
					end

					16'h10zz: begin		/* FC0e_10nm_iiii */
						opUCmd	= JX2_UCMD_ALU_ADD3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UW;
					end
					16'h11zz: begin		/* FC0e_11nm_iiii */
						opUCmd	= JX2_UCMD_ALU_SUB3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UW;
					end
					16'h12zz: begin		/* FC0e_12nm_iiii */
						opUCmd	= JX2_UCMD_ALU_MUL3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_SW;
					end

					16'h15zz: begin		/* FC0e_15nm_iiii */
						opUCmd	= JX2_UCMD_ALU_AND3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_SW;
					end
					16'h16zz: begin		/* FC0e_16nm_iiii */
						opUCmd	= JX2_UCMD_ALU_OR3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_SW;
					end
					16'h17zz: begin		/* FC0e_17nm_iiii */
						opUCmd	= JX2_UCMD_ALU_XOR3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_SW;
					end
				endcase
			end

			8'h20: begin
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIsImm32	= 1;
			end
			8'h21: begin
				opUCmd	= JX2_UCMD_CF_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIsImm32	= 1;
			end
			8'h22: begin
				opUCmd	= JX2_UCMD_CF_BT;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIsImm32	= 1;
			end
			8'h23: begin
				opUCmd	= JX2_UCMD_CF_BF;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIsImm32	= 1;
			end
			8'h24: begin
				opUCmd	= JX2_UCMD_ALU_LDIX;
				opFmid	= JX2_FMID_IMM8Z;
				opIsImm32	= 1;
			end
			8'h25: begin
				opUCmd	= JX2_UCMD_ALU_LDIX;
				opFmid	= JX2_FMID_IMM8N;
				opIsImm32	= 1;
			end
			8'h26: begin
				opUCmd	= JX2_UCMD_ALU_LDISH;
				opFmid	= JX2_FMID_IMM8Z;
				opIsImm32	= 1;
			end

			8'h27: begin 	/* FC27_xxxx_xxxx */
				case(istrWord[31:28])
					4'h0: begin 	/* FC27_0ndd_dddd */
						opUCmd	= JX2_UCMD_MOVB_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SB;
					end
					4'h1: begin 	/* FC27_1ndd_dddd */
						opUCmd	= JX2_UCMD_MOVW_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SB;
					end
					4'h2: begin 	/* FC27_2ndd_dddd */
						opUCmd	= JX2_UCMD_MOVL_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SB;
					end
					4'h3: begin 	/* FC27_3ndd_dddd */
						opUCmd	= JX2_UCMD_MOVQ_RM;
						opFmid	= JX2_FMID_REGSTDRPC;
						opBty	= JX2_BTY_SB;
					end

					4'h8: begin 	/* FC27_8ndd_dddd */
						opUCmd	= JX2_UCMD_MOVB_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'h9: begin 	/* FC27_9ndd_dddd */
						opUCmd	= JX2_UCMD_MOVW_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'hA: begin 	/* FC27_Andd_dddd */
						opUCmd	= JX2_UCMD_MOVL_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'hB: begin 	/* FC27_Bndd_dddd */
						opUCmd	= JX2_UCMD_MOVQ_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'hC: begin 	/* FC27_Cndd_dddd */
						opUCmd	= JX2_UCMD_MOVUB_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'hD: begin 	/* FC27_Dndd_dddd */
						opUCmd	= JX2_UCMD_MOVUW_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'hE: begin 	/* FC27_Endd_dddd */
						opUCmd	= JX2_UCMD_MOVUL_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end
					4'hF: begin 	/* FC27_Fndd_dddd */
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end

					default: begin
					end
				endcase
			end
		
			8'hCz: begin /* FCCx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_ALU_ADD3;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_SL;
				opIsImm32	= 1;
			end
			8'hDz: begin /* FCDx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_ALU_LDISH32;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_UL;
				opIsImm32	= 1;
			end
			8'hEz: begin /* FCEx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_SL;
				opIsImm32	= 1;
			end
			8'hFz: begin /* FCFx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_XL;
				opIsImm32	= 1;
			end

			default: begin
			end

		endcase

		default: begin
		end
	endcase
	
	case(opFmid)
		/*
		RegReg
			SB: Rm, Ro, Rn
			SW: Rm, disp17s, Rn

			UB: Rm, Q?Imm5:Ro, Rn
			UW: Rm, disp17u, Rn
			
			XB: Rm, Q?Imm5n:Imm5u, Rn
			XW: Rm, Q?(Imm5n-32):(Imm5u+32), Rn
		*/

		JX2_FMID_REGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;
			opImm	= {UV28_00, istrWord[4:0]};

			case(opIty)
				JX2_ITY_SB: begin
//					opRegO	= opRegO_Dfl;
				end
				JX2_ITY_SW: begin
					opRegO_Dfl	= JX2_REG_IMM;
					opImm	= opImm_imm17s;
				end

				JX2_ITY_UB: begin
					opRegO	= opExQ?JX2_REG_IMM:opRegO_Dfl;
					opImm	= {UV28_00, istrWord[4:0]};
				end

				JX2_ITY_UW: begin
					opRegO_Dfl	= JX2_REG_IMM;
					opImm	= opImm_imm17u;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
				end

				JX2_ITY_XB: begin
					opRegO	= JX2_REG_IMM;
					opImm	= {opExQ?UV28_FF:UV28_00, istrWord[4:0]};
				end
				JX2_ITY_XW: begin
					opRegO	= JX2_REG_IMM;
					opImm	= {opExQ?UV27_FF:UV27_00, ~opExQ, istrWord[4:0]};
				end
				
				default: begin
				end

			endcase
		end

		/*
			SB: Rm, (Rn)
			UB: Rm, (Rn, Disp5)
			SW: Rm, (Rn, Disp17s)
		 */
		JX2_FMID_REGSTREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opImm	= {UV28_00, istrWord[4:0]};
				opUIxt	= {JX2_IXT_RDI, 1'b0, opBty};
			end
			else
//			 	if(opIsFC0)
				if(opIty==JX2_ITY_SW)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_RDI, 1'b0, opBty};
				opImm	= opImm_imm17s;
			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_REG, 1'b0, opBty};
			end
		end

		/*
			SB: (Rm), Rn
			UB: (Rm, Disp5), Rn
			SW: Rm, (Rn, Disp17s)
		 */
		JX2_FMID_LDREGREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opImm	= {UV28_00, istrWord[4:0]};
				opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
			end
			else
//			 	if(opIsFC0)
				if(opIty==JX2_ITY_SW)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
				opImm	= opImm_imm17s;
			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_REG, 1'b1, opBty};
			end
		end

		JX2_FMID_REGSTDRREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= opRegO_Dfl;
				opUIxt	= {JX2_IXT_RRI, 1'b0, opBty};
			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_RDL, 1'b0, opBty};
			end
		end
		JX2_FMID_LDDRREGREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= opRegO_Dfl;
				opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
			end
		end

		/*
			FCXX_Xndd_dddd
		 */
		JX2_FMID_REGSTDRPC: begin
			opRegN	= JX2_REG_PC;
			opRegM	= opRegP_DfFC;
			opUIxt	= {JX2_IXT_RDI, 1'b0, opBty};
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
			opRegM	= JX2_REG_PC;
			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
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
		 */
	
		JX2_FMID_IMM8REG: begin
			case(opIty)
				JX2_ITY_SB:
					opImm = {
						istrWord[23] ? UV25_FF : UV25_00,
						istrWord[23:16] };
				JX2_ITY_SW:
					opImm = {
						istrWord[31] ? UV17_FF : UV17_00,
						istrWord[31:16] };
				JX2_ITY_SL:
					opImm = { istrWord[47], istrWord[47:16] };

				JX2_ITY_UB:
					opImm = { UV25_00, istrWord[23:16] };
				JX2_ITY_UW:
					opImm = { UV17_00, istrWord[31:16] };
				JX2_ITY_UL:
					opImm = {    1'b0, istrWord[47:16] };

				JX2_ITY_NB:
					opImm = { UV25_FF, istrWord[23:16] };
				JX2_ITY_NW:
					opImm = { UV17_FF, istrWord[31:16] };
				JX2_ITY_NL:
					opImm = {    1'b1, istrWord[47:16] };

				JX2_ITY_XB:
					opImm = {
						istrWord[23] ? UV25_00 : UV25_FF,
						istrWord[23:16] };
				JX2_ITY_XW:
					opImm = {
						istrWord[31] ? UV17_00 : UV17_FF,
						istrWord[31:16] };
				JX2_ITY_XL:
					opImm = { ~istrWord[47], istrWord[47:16] };

				default: begin
				end
			endcase
			
			if(opIsImm32 || opIsImm16)
			begin
				opRegM	= JX2_REG_IMM;
				opRegN	= opRegO_DfFC;
				opRegO	= opRegO_DfFC;
			end
			else
			begin
				opRegM	= JX2_REG_IMM;
				opRegN	= opRegO_Dfl;
				opRegO	= opRegO_Dfl;
			end
		end
		
		/*
			SB: Fzdd_xxdd	(PC, disp16s), DLR
		 */
		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_PC;
			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
			if(opIsImm32)
			begin
				opImm = { istrWord[47], istrWord[47:16] };
			end
			else
			begin
				opImm	= {istrWord[7]?UV17_FF:UV17_00,
					istrWord[ 7: 0],
					istrWord[23:16]};
			end
		end
		
		default: begin
		end
	endcase
end

endmodule
