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

reg[6:0]	opRegN_Df2;
reg[6:0]	opRegM_Df2;

reg[6:0]	opRegN_Cr;
reg[6:0]	opRegM_Cr;
reg[6:0]	opRegN_Sr;
reg[6:0]	opRegM_Sr;

reg[6:0]	opRegO_DfFC;
reg[6:0]	opRegP_DfFC;

reg[6:0]	opRegN_Jcmp;
reg[6:0]	opRegM_Jcmp;

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

reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[1:0]	opCcty;

reg[7:0]	opUCmdIx;

reg		opExQ;
reg		opExN;
reg		opExM;
reg		opExI;
reg		opIsImm32;
reg		opIsImm16;
reg		opIsFC0;
// reg		opIsF10;
// reg		opIsF20;
reg		opIsNotFx;

reg		opExQ2;
reg		opExN2;
reg		opExM2;
reg		opExI2;

reg tRegRmIsRz;
reg tRegRnIsRz;
reg tRegRoIsRz;
reg tRegRmIsR0;
reg tRegRmIsR1;
reg tRegRnIsR0;
reg tRegRnIsR1;
reg tRegRoIsR0;
reg tRegRoIsR1;

reg tRegRm2IsRz;
reg tRegRn2IsRz;
reg tRegRm2IsR0;
reg tRegRm2IsR1;
reg tRegRn2IsR0;
reg tRegRn2IsR1;

reg	tMsgLatch;
reg	tNextMsgLatch;

always @*
begin

	opExQ		= istrWord[7];
	opExN		= istrWord[6];
	opExM		= istrWord[5];
	opExI		= istrWord[4];

	opExQ2		= istrWord[27];
	opExN2		= istrWord[26];
	opExM2		= istrWord[25];
	opExI2		= istrWord[24];

	opRegO_Dfl	= {2'b00, istrWord[4], istrWord[ 3: 0]};
	opRegN_Dfl	= {2'b00, istrWord[6], istrWord[23:20]};
	opRegM_Dfl	= {2'b00, istrWord[5], istrWord[19:16]};
	opRegN_Cr	= {istrWord[6], 2'b10, istrWord[23:20]};
	opRegM_Cr	= {istrWord[5], 2'b10, istrWord[19:16]};
	opRegN_Sr	= {istrWord[6], 2'b11, istrWord[23:20]};
	opRegM_Sr	= {istrWord[5], 2'b11, istrWord[19:16]};

	opRegO_DfFC	= {2'b00, istrWord[8], istrWord[ 3: 0]};
	opRegP_DfFC	= {2'b00, istrWord[8], istrWord[27:24]};

	opRegN_Df2	= {2'b00, istrWord[26], istrWord[7:4]};
	opRegM_Df2	= {2'b00, istrWord[25], istrWord[3:0]};

	opRegN_Jcmp	= opRegN_Df2;
	opRegM_Jcmp	= opRegM_Df2;

//	opIsF20		= (istrWord[11:10]==2'b00) &&
//		((istrWord[9:8]==2'b01) ||
//		 (istrWord[9:8]==2'b10) );

	opIsFC0		= (istrWord[11:9]==3'b110);
	
	opIsNotFx	= (istrWord[15:12]!=4'b1111);
	
	tNextMsgLatch	= 0;
	opIsImm16		= 0;

	opImm_disp20s = { istrWord[7] ? UV13_FF : UV13_00,
		istrWord[7:0], istrWord[27:16] };

//	opImm_imm17s	= 0;
//	opImm_imm17u	= 0;

//	opImm_imm17u	= {UV24_00, istrWord[4:0], istrWord[27:24]};
//	opImm_imm17n	= {UV24_FF, istrWord[4:0], istrWord[27:24]};
	opImm_imm17u	= {UV24_00, istrWord[24:16]};
	opImm_imm17n	= {UV24_FF, istrWord[24:16]};
	opImm_imm17s	= opImm_imm17u;

	opImm_imm10u	= {UV23_00, istrWord[25:16]};
	opImm_imm10n	= {UV23_FF, istrWord[25:16]};
	opImm_imm10s	= {istrWord[25]?UV23_FF:UV23_00, istrWord[25:16]};

	opImm_immWordZx	={UV17_00, istrWord[31:16]};
	opImm_immWordNx	={UV17_FF, istrWord[31:16]};

	opImm_disp8s	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};

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

		opExQ2		= istrWord[3];
		opExN2		= istrWord[2];
		opExM2		= istrWord[1];
		opExI2		= istrWord[0];

		opRegO_Dfl	= JX2_REG_DLR;
		opRegN_Dfl	= {2'b00, istrWord[2], istrWord[23:20]};
		opRegM_Dfl	= {2'b00, istrWord[1], istrWord[19:16]};
		opRegN_Cr	= {2'b01, istrWord[2], istrWord[23:20]};
		opRegM_Cr	= {2'b01, istrWord[1], istrWord[19:16]};
		opRegN_Sr	= {2'b10, istrWord[2], istrWord[23:20]};
		opRegM_Sr	= {2'b10, istrWord[1], istrWord[19:16]};

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

		opRegN_Df2	= opRegN_Dfl;
		opRegM_Df2	= opRegM_Dfl;
	end
//	else
//		if(opIsF20)
//	begin
//		opRegO_Dfl	= JX2_REG_IMM;
//		opRegN_Dfl	= {2'b00, istrWord[26], istrWord[7:4]};
//		opRegM_Dfl	= {2'b00, istrWord[25], istrWord[3:0]};
//	end

	tRegRnIsRz	= (opRegN_Dfl[4:1]==4'b0000);
	tRegRmIsRz	= (opRegM_Dfl[4:1]==4'b0000);
	tRegRoIsRz	= (opRegO_Dfl[4:1]==4'b0000);
	tRegRnIsR0	= tRegRnIsRz && !opRegN_Dfl[0];
	tRegRnIsR1	= tRegRnIsRz &&  opRegN_Dfl[0];
	tRegRmIsR0	= tRegRmIsRz && !opRegM_Dfl[0];
	tRegRmIsR1	= tRegRmIsRz &&  opRegM_Dfl[0];
	tRegRoIsR0	= tRegRoIsRz && !opRegO_Dfl[0];
	tRegRoIsR1	= tRegRoIsRz &&  opRegO_Dfl[0];

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

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[11:8])

		4'h0:
		casez(istrWord[31:16])
			16'h04zz: begin		/* F0ez_04zz */
				if(opExQ)
				begin
					opUCmd	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opUCmd	= JX2_UCMD_MOVB_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h05zz: begin		/* F0ez_05zz */
				if(opExQ)
				begin
					opUCmd	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opUCmd	= JX2_UCMD_MOVW_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h06zz: begin		/* F0ez_06zz */
				if(opExQ)
				begin
					opUCmd	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opUCmd	= JX2_UCMD_MOVL_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h07zz: begin		/* F0ez_07zz */
				if(opExQ)
				begin
					opUCmd	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opUCmd	= JX2_UCMD_MOVL_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
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
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUL_MR;
				else
					opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				if(opExQ)
					opBty	= JX2_BTY_SL;
				else
					opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h10zz: begin		/* F0ez_10zz */
				opUCmd		= JX2_UCMD_ALU_ADD3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_UB;
			end
			16'h11zz: begin		/* F0ez_11zz */
				opUCmd		= JX2_UCMD_ALU_SUB3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_UB;
			end
			16'h12zz: begin		/* F0ez_12zz */
				opUCmd	= JX2_UCMD_ALU_MUL3;
				opFmid	= JX2_FMID_REGREG;
//				opIty	= JX2_ITY_UB;
			end

			16'h15zz: begin		/* F0ez_15zz */
				opUCmd	= JX2_UCMD_ALU_AND3;
				opFmid	= JX2_FMID_REGREG;
//				opIty	= JX2_ITY_UB;
			end
			16'h16zz: begin		/* F0ez_16zz */
				opUCmd	= JX2_UCMD_ALU_OR3;
				opFmid	= JX2_FMID_REGREG;
//				opIty	= JX2_ITY_UB;
			end
			16'h17zz: begin		/* F0ez_17zz */
				opUCmd	= JX2_UCMD_ALU_XOR3;
				opFmid	= JX2_FMID_REGREG;
//				opIty	= JX2_ITY_UB;
			end

			16'h18zz: begin	/* F0ez_18nm */
			end

			16'h19zz: begin	/* F0ez_19nm */
				case(istrWord[3:0])
					4'h0: begin
						opUCmd		= JX2_UCMD_ALU_ADD3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'h1: begin
						opUCmd		= JX2_UCMD_ALU_SUB3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'h2: begin
						opUCmd		= JX2_UCMD_ALU_ADC3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'h3: begin
						opUCmd		= JX2_UCMD_ALU_SBB3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
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
						opIty	= JX2_ITY_UB;
					end
					4'hB: begin
						opUCmd	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UW;
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

`ifdef JX2_EX_GPRSIMD
			16'h1Azz: begin	/* F0ez_1Anm */
				case(istrWord[3:0])
					4'h0: begin
						if(opExQ)
							opUCmd		= JX2_UCMD_ALU_PADDL;
						else
							opUCmd		= JX2_UCMD_ALU_PADDW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'h1: begin
						if(opExQ)
							opUCmd		= JX2_UCMD_ALU_PSUBL;
						else
							opUCmd		= JX2_UCMD_ALU_PSUBW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'h2: begin
					end
					4'h3: begin
					end
					4'h4: begin
					end
					4'h5: begin
						opUCmd		= JX2_UCMD_GSV_PFPHF;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= 1;
					end
					4'h6: begin
						opUCmd		= JX2_UCMD_GSV_PFPHF;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= 2;
					end
					4'h7: begin
						opUCmd		= JX2_UCMD_GSV_PFPHF;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= 3;
					end
					4'h8: begin
						opUCmd		= JX2_UCMD_MOV_RR;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= opExQ ? 5 : 4;
					end
					4'h9: begin
					end
					4'hA: begin
						opFmid		= JX2_FMID_REGREG;
						opUCmd		= JX2_UCMD_MOV_RR;
						opUCmdIx	= opExQ ? 7 : 6;
					end

					4'hB: begin
						opUCmd		= JX2_UCMD_ALU_PSELT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= { 6'h00, opExQ, opExI };
					end

					4'hC: begin
						opUCmd	= JX2_UCMD_ALU_CMPEQ;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hD: begin
						opUCmd	= JX2_UCMD_ALU_CMPHI;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hE: begin
						if(opExI)
							opUCmd	= JX2_UCMD_ALU_CMPGTHF;
						else
							opUCmd	= JX2_UCMD_ALU_CMPGT;
						opFmid	= JX2_FMID_REGREG;
					end

					4'hF: begin
						opFmid	= JX2_FMID_REGREG;
						opUCmd		= JX2_UCMD_ALU_PMULUW;
					end
				endcase
			end
`endif

			16'h1Bzz: begin	/* F0ez_1Bnm */
					opCcty	= {1'b1, opExI};

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
						opUCmd	= (opExQ) ?
							JX2_UCMD_MOVUB_MR : 
							JX2_UCMD_MOVB_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SB;
					end
					4'h9: begin
						opUCmd	= (opExQ) ?
							JX2_UCMD_MOVUW_MR : 
							JX2_UCMD_MOVW_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SW;
					end
					4'hA: begin
						opUCmd	= (opExQ) ?
							JX2_UCMD_MOVUL_MR : 
							JX2_UCMD_MOVL_MR;
						opFmid	= JX2_FMID_LDREGREG;
						opBty	= JX2_BTY_SL;
					end
					4'hB: begin
						opUCmd	= (opExQ) ?
							JX2_UCMD_MOVUL_MR : 
							JX2_UCMD_MOVQ_MR;
						opBty	= (opExQ) ?
							JX2_BTY_SL :
							JX2_BTY_SQ;
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
						opUCmd	= (opExQ) ?
							JX2_UCMD_MOVUL_MR : 
							JX2_UCMD_MOVQ_MR;
						opBty	= (opExQ) ?
							JX2_BTY_SL :
							JX2_BTY_SQ;
						opFmid	= JX2_FMID_LDDRREGREG;
					end
				endcase
			end

			16'h1Czz: begin	/* F0ez_1Cnm */
				case(istrWord[3:0])
					4'h0: begin
						opUCmd	= JX2_UCMD_ALU_NOT;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h1: begin
						opUCmd	= JX2_UCMD_ALU_SUB3;
						opFmid	= JX2_FMID_REG;
						opIty	= JX2_ITY_SW;
					end

					4'h5: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_EXTUL;
						else
							opUCmd	= JX2_UCMD_ALU_EXTSL;
						opFmid	= JX2_FMID_REGREG;
					end

					4'h8: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_EXTUB;
						else
							opUCmd	= JX2_UCMD_ALU_EXTSB;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h9: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_EXTUW;
						else
							opUCmd	= JX2_UCMD_ALU_EXTSW;
						opFmid	= JX2_FMID_REGREG;
					end

					4'hA: begin
						opUCmd	= JX2_UCMD_MOV_RC;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UL;
					end
					4'hB: begin
						opUCmd	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
					end

					default: begin
					end
				endcase
			end

			16'h1Dzz: begin	/* F0ez_1Dnm */
				case(istrWord[3:0])					
					default: begin
					end
				endcase
			end

			16'h3010: begin
				opUCmd		= JX2_UCMD_CF_BRA;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
			end
			16'h3011: begin
				opUCmd		= JX2_UCMD_CF_BSR;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
			end
			16'h3012: begin
				opUCmd		= JX2_UCMD_CF_BRA;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
				opCcty		= JX2_IXC_CT;
			end
			16'h3013: begin
				opUCmd		= JX2_UCMD_CF_BRA;
				opFmid		= JX2_FMID_REGPC;
				opBty		= JX2_BTY_SW;
				opCcty		= JX2_IXC_CF;
			end


			16'h3018: begin
				opUCmd	= JX2_UCMD_MOV_PUSH;
				opFmid	= JX2_FMID_REG;
				opIty	= JX2_ITY_SB;
			end
			16'h3019: begin
				opUCmd	= JX2_UCMD_MOV_PUSHC;
				opFmid	= JX2_FMID_REG;
				opIty	= JX2_ITY_NB;
			end
			16'h301A: begin
				opUCmd	= JX2_UCMD_MOV_POP;
				opFmid	= JX2_FMID_REG;
				opIty	= JX2_ITY_SB;
			end
			16'h301B: begin
				if(istrWord[7:4]==0)
				begin
					opUCmd	= JX2_UCMD_RET;
					opFmid	= JX2_FMID_Z;
					opBty	= JX2_BTY_UB;
				end
				else
				begin
					opUCmd	= JX2_UCMD_MOV_POPC;
					opFmid	= JX2_FMID_REG;
					opIty	= JX2_ITY_UB;
				end
			end

			16'h301C: begin
				opUCmd	= JX2_UCMD_ALU_CMPGT;
				opFmid	= JX2_FMID_REG;
				opIty	= JX2_ITY_SB;
			end
			16'h301D: begin
				opUCmd	= JX2_UCMD_ALU_CMPGE;
				opFmid	= JX2_FMID_REG;
				opIty	= JX2_ITY_SB;
			end

			16'h3020: begin
				opUCmd		= JX2_UCMD_CF_JMP;
				opFmid		= JX2_FMID_REG;
			end
			16'h3021: begin
				opUCmd		= JX2_UCMD_CF_JSR;
				opFmid		= JX2_FMID_REG;
			end
			16'h3022: begin
				opUCmd		= JX2_UCMD_CF_JMP;
				opFmid		= JX2_FMID_REG;
				opCcty		= JX2_IXC_CT;
			end
			16'h3023: begin
				opUCmd		= JX2_UCMD_CF_JMP;
				opFmid		= JX2_FMID_REG;
				opCcty		= JX2_IXC_CF;
			end

			16'h37zz: begin
				opUCmd		= JX2_UCMD_ALU_SWAPN;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SL;
				opUCmdIx	= { 4'h0, istrWord[3:0] };
			end

			16'h50zz: begin		/* F0ez_50zz */
				opUCmd		= JX2_UCMD_ALU_CSELT;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_UB;
				opUCmdIx	= 0;
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

			16'h5Czz: begin		/* F0ez_5Czz */
				opUCmd		= opExQ ? JX2_UCMD_ALU_ADDUL : JX2_UCMD_ALU_ADDSL;
				opFmid		= JX2_FMID_REGREG;
			end
			16'h5Dzz: begin		/* F0ez_5Dzz */
				opUCmd		= opExQ ? JX2_UCMD_ALU_SUBUL : JX2_UCMD_ALU_SUBSL;
				opFmid		= JX2_FMID_REGREG;
			end


`ifndef JX2_FPU_NOFPU
			16'h90zz: begin		/* F0ez_90zz */
				opUCmd	= JX2_UCMD_FMOVS_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h91zz: begin		/* F0ez_91zz */
				opUCmd	= JX2_UCMD_FMOVD_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end
			16'h92zz: begin		/* F0ez_92zz */
				opUCmd	= JX2_UCMD_FMOVS_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h93zz: begin		/* F0ez_93zz */
				opUCmd	= JX2_UCMD_FMOVD_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end
			16'h94zz: begin		/* F0ez_94zz */
				opUCmd	= JX2_UCMD_FMOVS_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h95zz: begin		/* F0ez_95zz */
				opUCmd	= JX2_UCMD_FMOVD_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end
			16'h96zz: begin		/* F0ez_96zz */
				opUCmd	= JX2_UCMD_FMOVS_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h97zz: begin		/* F0ez_97zz */
				opUCmd	= JX2_UCMD_FMOVD_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h98zz: begin		/* F0ez_98zz */
				opUCmd	= JX2_UCMD_FPU_FADD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h99zz: begin		/* F0ez_99zz */
				opUCmd	= JX2_UCMD_FPU_FSUB3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h9Azz: begin		/* F0ez_9Azz */
				opUCmd	= JX2_UCMD_FPU_FMUL3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

`ifndef JX2_FPU_NOFDIV
			16'h9Bzz: begin		/* F0ez_9Bzz */
				opUCmd	= JX2_UCMD_FPU_FDIV3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
`endif

			16'h9Czz: begin		/* F0ez_9Czz */
				case(istrWord[3:0])
					4'hE: begin
						opUCmd	= JX2_UCMD_FPU_FMOV;
						opFmid	= JX2_FMID_REGREG;
					end
					default: begin
					end
				endcase
			end

			16'h9Fzz: begin		/* F0ez_9Fnm */
				case(istrWord[3:0])
					4'h1: begin
						opUCmd	= JX2_UCMD_FPU_FLDCD;
						opFmid	= JX2_FMID_REGREG;
					end

					4'h5: begin
						opUCmd	= JX2_UCMD_FPU_FSTCD;
						opFmid	= JX2_FMID_REGREG;
					end

					default: begin
					end
				endcase
			end
`endif

`ifdef def_true
			16'hCzzz: begin		/* F0dd_Cddd */
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
			end
			16'hDzzz: begin		/* F0dd_Dddd */
				opUCmd	= JX2_UCMD_CF_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
			end
			16'hEzzz: begin		/* F0dd_Eddd */
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
				opCcty	= JX2_IXC_CT;
			end
			16'hFzzz: begin		/* F0dd_Fddd */
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
				opCcty	= JX2_IXC_CF;
			end
`endif

			default: begin
			end
		endcase

		4'h1: begin		/* F1nm_Xedd */

			case(istrWord[31:28])
				4'h0: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVB_RM;
						opFmid	= JX2_FMID_REGSTREG;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h1: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVW_RM;
						opFmid	= JX2_FMID_REGSTREG;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'h2: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVL_RM;
						opFmid	= JX2_FMID_REGSTREG;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'h3: begin
					if(opExQ2)
					begin
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGREG;
					end else begin
						opUCmd	= JX2_UCMD_MOVQ_RM;
						opFmid	= JX2_FMID_REGSTREG;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SQ;
				end

				4'h8: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUB_MR : JX2_UCMD_MOVB_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h9: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUW_MR : JX2_UCMD_MOVW_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'hA: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVL_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'hB: begin
					opUCmd	= opExQ2 ? JX2_UCMD_MOVUL_MR : JX2_UCMD_MOVQ_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opIty	= JX2_ITY_SW;
					opBty	= opExQ2 ? JX2_BTY_SL : JX2_BTY_SQ;
//					opBty	= JX2_BTY_SQ;
				end

`ifdef JX2_EX_ALU_JMPCC
// `ifndef def_true
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

		4'h2: begin		/* F2nm_Xedd */

			case(istrWord[31:28])
// `ifndef def_true
`ifdef def_true
				4'h0: begin		/* F2nm_0ejj */
					opUCmd	= JX2_UCMD_ALU_ADD3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
				4'h1: begin		/* F2nm_1ejj */
					opUCmd	= JX2_UCMD_ALU_ADD3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_NW;
				end
				4'h2: begin		/* F2nm_2ejj */
					opUCmd	= JX2_UCMD_ALU_MUL3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
`endif

//`ifndef def_true
// `ifdef def_true
`ifdef JX2_EX_ALU_ADDSL
				4'h3: begin		/* F1ej_Cjnm */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_ADDUL :
						JX2_UCMD_ALU_ADDSL;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
				4'h4: begin		/* F1ej_Djnm */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_ADDUL :
						JX2_UCMD_ALU_ADDSL;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_NW;
				end
`endif

// `ifndef def_true
`ifdef def_true
				4'h5: begin		/* F2nm_5ejj */
					opUCmd	= JX2_UCMD_ALU_AND3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
				4'h6: begin		/* F2nm_6ejj */
					opUCmd	= JX2_UCMD_ALU_OR3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
				4'h7: begin		/* F2nm_7ejj */
					opUCmd	= JX2_UCMD_ALU_XOR3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
`endif

//`ifndef def_true
`ifdef def_true
				4'h8: begin		/* F2nm_8ejj */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_SHADQ3 :
						JX2_UCMD_ALU_SHAD3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
				4'h9: begin		/* F2nm_9ejj */
					opUCmd	= opExQ2 ?
						JX2_UCMD_ALU_SHLDQ3 :
						JX2_UCMD_ALU_SHLD3;
					opFmid	= JX2_FMID_REGREG;
					opIty	= JX2_ITY_UW;
				end
`endif

// `ifndef def_true
`ifdef def_true
				4'hC: begin		/* F2nz_Cejj */
//					opFmid	= istrWord[0] ?
//						JX2_FMID_IMM4NREG :
//						JX2_FMID_IMM4ZREG;

					opIty	= istrWord[0] ?
						JX2_ITY_NQ :
						JX2_ITY_UQ;
				
					casez(istrWord[3:0])
						4'b010z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_TSTQ :
								JX2_UCMD_ALU_TST;
							opFmid = JX2_FMID_IMM8REG;
						end
						4'b011z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQHS :
								JX2_UCMD_ALU_CMPHS;
							opFmid = JX2_FMID_IMM8REG;
						end
						4'b100z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQHI :
								JX2_UCMD_ALU_CMPHI;
							opFmid = JX2_FMID_IMM8REG;
						end
						4'b101z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQGE :
								JX2_UCMD_ALU_CMPGE;
							opFmid = JX2_FMID_IMM8REG;
						end
						4'b110z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQEQ :
								JX2_UCMD_ALU_CMPEQ;
							opFmid = JX2_FMID_IMM8REG;
						end
						4'b111z: begin
							opUCmd	= opExQ2 ?
								JX2_UCMD_ALU_CMPQGT :
								JX2_UCMD_ALU_CMPGT;
							opFmid = JX2_FMID_IMM8REG;
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
//					opIsImm16	= 1;
				end
				3'b001: begin
					opUCmd		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_NW;
//					opIsImm16	= 1;
				end
				3'b010: begin
					opUCmd		= JX2_UCMD_ALU_ADD3;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SW;
//					opIsImm16	= 1;
//					opUCmdIx	= JX2_IXTB_RRI_LDSB;
				end
				3'b011: begin
					opUCmd		= JX2_UCMD_ALU_LDISH16;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
//					opIsImm16	= 1;
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
						opUCmd		= JX2_UCMD_ALU_ADD3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UW;
						opUCmdIx	= JX2_IXTB_RRI_LDSB;
					end
					16'h11zz: begin		/* FC0e_11nm_iiii */
						opUCmd		= JX2_UCMD_ALU_SUB3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UW;
						opUCmdIx	= JX2_IXTB_RRI_LDUB;
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

					16'h19zz: begin		/* FC0e_19nz_iiii */
						case(istrWord[19:16])
							4'h4: begin
								opUCmd	= opExQ ?
									JX2_UCMD_ALU_TSTQ :
									JX2_UCMD_ALU_TST;
								opFmid	= JX2_FMID_IMM8REG;
								opIty	= JX2_ITY_SQ;
							end

							4'hC: begin
								opUCmd	= opExQ ?
									JX2_UCMD_ALU_CMPQEQ :
									JX2_UCMD_ALU_CMPEQ;
								opFmid	= JX2_FMID_IMM8REG;
								opIty	= JX2_ITY_SQ;
							end
							4'hD: begin
								opUCmd	= opExQ ?
									JX2_UCMD_ALU_CMPQHI :
									JX2_UCMD_ALU_CMPHI;
								opFmid	= JX2_FMID_IMM8REG;
								opIty	= JX2_ITY_SQ;
							end
							4'hE: begin
								opUCmd	= opExQ ?
									JX2_UCMD_ALU_CMPQGT :
									JX2_UCMD_ALU_CMPGT;
								opFmid	= JX2_FMID_IMM8REG;
								opIty	= JX2_ITY_SQ;
							end
							
							default: begin
							end
						endcase
					end
				endcase
			end

			8'h20: begin
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
//				opIsImm32	= 1;
			end
			8'h21: begin
				opUCmd	= JX2_UCMD_CF_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
//				opIsImm32	= 1;
			end
			8'h22: begin
//				opUCmd	= JX2_UCMD_CF_BT;
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
				opCcty	= JX2_IXC_CT;
//				opIsImm32	= 1;
			end
			8'h23: begin
//				opUCmd	= JX2_UCMD_CF_BF;
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SL;
				opCcty	= JX2_IXC_CF;
//				opIsImm32	= 1;
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

`ifndef def_true
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
					4'h4: begin 	/* FC27_4ndd_dddd */
						opUCmd	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
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
						opUCmd	= JX2_UCMD_MOVUL_MR;
						opFmid	= JX2_FMID_LDDRPCREG;
						opBty	= JX2_BTY_SB;
					end

//					4'hF: begin 	/* FC27_Fndd_dddd */
//						opUCmd	= JX2_UCMD_LEA_MR;
//						opFmid	= JX2_FMID_LDDRPCREG;
//						opBty	= JX2_BTY_SB;
//					end

					default: begin
					end
				endcase
			end
`endif
		
			8'hCz: begin /* FCCx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_ALU_ADD3;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_SL;
//				opIsImm32	= 1;
				opUCmdIx	= JX2_IXTB_RRI_LDSB;
			end
			8'hDz: begin /* FCDx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_ALU_LDISH32;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_UL;
//				opIsImm32	= 1;
			end
			8'hEz: begin /* FCEx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
//				opIty		= JX2_ITY_SL;
				opIty		= JX2_ITY_UL;
//				opIsImm32	= 1;
			end
			8'hFz: begin /* FCFx_xxxx_xxxx */
				opUCmd		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
//				opIty		= JX2_ITY_XL;
				opIty		= JX2_ITY_NL;
//				opIsImm32	= 1;
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
	
	case(opFmid)
		JX2_FMID_Z: begin
		end

		JX2_FMID_REG: begin
			opUIxt	= opUCmdIx;
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegO_Dfl;
					opRegM	= JX2_REG_ZZR;
					opRegO	= opRegO_Dfl;
				end

				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_ZZR;
					opRegO	= opRegM_Dfl;
				end

				default: begin
					$display("Jx2DecOpFx: Bad Reg, Ity=%X", opIty);
					$display("Jx2DecOpFx: Istr %X-%X-%X",
						istrWord[15:0], istrWord[31:16], istrWord[47:32]);
				end
			endcase
		end

		/*
		RegReg
			SB: Rm, Ro, Rn
			SW: Rm, disp17s, Rn

			UB: Rm, Q?Imm5:Ro, Rn
			UW: Rm, disp17u, Rn

			NB: Rn, Rm, Rn
			NW: Rm, disp17n, Rn
			
			XB: Rm, Q?Imm5n:Imm5u, Rn
			XW: Rm, Q?(Imm5n-32):(Imm5u+32), Rn
		*/

		JX2_FMID_REGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;
			opImm	= {UV28_00, istrWord[4:0]};
			opUIxt	= opUCmdIx;

			case(opIty)
				JX2_ITY_SB: begin
//					opRegO	= opRegO_Dfl;
				end
				JX2_ITY_SW: begin
					opRegN	= opRegN_Df2;
					opRegM	= opRegM_Df2;
					opRegO_Dfl	= JX2_REG_IMM;
					opImm	= opImm_imm17s;
				end

`ifndef def_true
				JX2_ITY_UB: begin
					opRegO	= opExQ?JX2_REG_IMM:opRegO_Dfl;
					opImm	= {UV28_00, istrWord[4:0]};
				end
`endif

				JX2_ITY_UW: begin
					opRegN	= opRegN_Df2;
					opRegM	= opRegM_Df2;
					opRegO	= JX2_REG_IMM;
					opImm	= opImm_imm17u;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
				end

				JX2_ITY_NW: begin
					opRegN	= opRegN_Df2;
					opRegM	= opRegM_Df2;
					opRegO	= JX2_REG_IMM;
					opImm	= opImm_imm17n;
				end

`ifndef def_true
				JX2_ITY_XB: begin
					opRegO	= JX2_REG_IMM;
					opImm	= {opExQ?UV28_FF:UV28_00, istrWord[4:0]};
				end
				JX2_ITY_XW: begin
					opRegO	= JX2_REG_IMM;
					opImm	= {opExQ?UV27_FF:UV27_00, ~opExQ, istrWord[4:0]};
				end
`endif
				
				default: begin
					$display("Jx2DecOpFx: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end

		JX2_FMID_IMM8Z: begin
//			opImm	= {UV25_00, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
			opUIxt	= opUCmdIx;

		 	if(opIsFC0)
			begin
				opImm = opImm_immWordZx;
			end
			else
			begin
				opImm = {
					UV9_00,
					istrWord[23:16],
					istrWord[47:32] };
			end
		end
		JX2_FMID_IMM8N: begin
//			opImm	= {UV25_FF, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
			opUIxt	= opUCmdIx;

		 	if(opIsFC0)
			begin
				opImm = opImm_immWordNx;
			end
			else
			begin
				opImm = {
					UV9_FF,
					istrWord[23:16],
					istrWord[47:32] };
			end
		end

		/*
			SB: Rm, (Rn)
			UB: Rm, (Rn, Disp5)
			SW: Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_REGSTREG: begin
			opRegO	= JX2_REG_IMM;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, opBty};

			if(opIty==JX2_ITY_SW)
			begin
				opRegN	= opRegM_Df2;
				opRegM	= opRegN_Df2;
				if(tRegRn2IsRz)
				begin
					opRegM	= tRegRn2IsR1 ? JX2_REG_GBR : JX2_REG_PC;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
				end
			end else begin
				opRegN	= opRegM_Dfl;
				opRegM	= opRegN_Dfl;
				if(tRegRnIsRz)
				begin
					opRegM	= tRegRnIsR1 ? JX2_REG_GBR : JX2_REG_PC;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
				end
			end

			if(opIty==JX2_ITY_UB)
			begin
				opImm	= {UV28_00, istrWord[4:0]};
			end
			else
				if(opIty==JX2_ITY_SW)
			begin
				opImm	= opImm_imm17s;
			end else begin
				opRegO	= JX2_REG_ZZR;
			end
		end

		/*
			SB: (Rm), Rn
			UB: (Rm, Disp5), Rn
			SW: Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_LDREGREG: begin
			opRegO	= JX2_REG_IMM;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};

			if(opIty==JX2_ITY_SW)
			begin
				opRegN	= opRegN_Df2;
				opRegM	= opRegM_Df2;
				if(tRegRm2IsRz)
				begin
					opRegM	= tRegRm2IsR1 ? JX2_REG_GBR : JX2_REG_PC;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, JX2_BTY_SB};
				end
			end else begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				if(tRegRmIsRz)
				begin
					opRegM	= tRegRmIsR1 ? JX2_REG_GBR : JX2_REG_PC;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, JX2_BTY_SB};
				end
			end

			if(opIty==JX2_ITY_UB)
			begin
				opImm	= {UV28_00, istrWord[4:0]};
			end
			else
				if(opIty==JX2_ITY_SW)
			begin
				opImm	= opImm_imm17s;
			end
			else
			begin
				opRegO	= JX2_REG_ZZR;
			end
		end

		JX2_FMID_REGSTDRREG: begin
			if(opIty==JX2_ITY_UB)
			begin
//				opRegM	= opRegM_Dfl;
				opRegN	= opRegM_Dfl;

				if(tRegRnIsRz)
				begin
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
					if(tRegRnIsR1)
					begin
						if(tRegRoIsR1)
						begin
//							opRegN	= JX2_REG_TBR;
							opRegM	= JX2_REG_TBR;
							opRegO	= JX2_REG_DLR;
						end else begin
//							opRegN	= JX2_REG_GBR;
							opRegM	= JX2_REG_GBR;
							opRegO	= opRegO_Dfl;
						end
					end else begin
						if(tRegRoIsR1)
						begin
//							opRegN	= JX2_REG_DLR;
							opRegM	= JX2_REG_DLR;
							opRegO	= JX2_REG_ZZR;
							opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
						end else begin
//							opRegN	= JX2_REG_PC;
							opRegM	= JX2_REG_PC;
							opRegO	= opRegO_Dfl;
						end
					end
				end
				else
				begin
//					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegO_Dfl;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, opBty};
				end

			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= JX2_REG_DLR;
				opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, opBty};
			end
		end
		JX2_FMID_LDDRREGREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;

				if(tRegRmIsRz)
				begin
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, JX2_BTY_SB};
					if(tRegRmIsR1)
					begin
						if(tRegRoIsR1)
						begin
							opRegM	= JX2_REG_TBR;
							opRegO	= JX2_REG_DLR;
						end else begin
							opRegM	= JX2_REG_GBR;
							opRegO	= opRegO_Dfl;
						end
					end else begin
						if(tRegRoIsR1)
						begin
							opRegM	= JX2_REG_DLR;
							opRegO	= JX2_REG_ZZR;
							opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, JX2_BTY_SB};
						end else begin
							opRegM	= JX2_REG_PC;
							opRegO	= opRegO_Dfl;
						end
					end
				end
				else
				begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegO_Dfl;
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};
				end

			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= JX2_REG_DLR;
				opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};
			end
		end

		/*
			FCXX_Xndd_dddd
		 */
		JX2_FMID_REGSTDRPC: begin
//			opRegN	= JX2_REG_PC;
//			opRegM	= opRegP_DfFC;
			opRegM	= JX2_REG_PC;
			opRegN	= opRegP_DfFC;

			opRegO	= JX2_REG_IMM;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, opBty};
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
			opRegO	= JX2_REG_IMM;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};
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
			opUIxt	= opUCmdIx;

//			if(opIsImm32 || opIsImm16)
//			if(opIsImm32)
			if(opIsFC0)
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
			
			case(opIty)
`ifndef def_true
				JX2_ITY_SB:
					opImm = {
						istrWord[23] ? UV25_FF : UV25_00,
						istrWord[23:16] };
`endif
				JX2_ITY_SW:
					opImm = {
						istrWord[31] ? UV17_FF : UV17_00,
						istrWord[31:16] };
				JX2_ITY_SL:
					opImm = { istrWord[47], istrWord[47:16] };

				JX2_ITY_SQ: begin
					opImm = opImm_imm10s;
					opRegN	= opRegN_Df2;
					opRegO	= opRegN_Df2;
				end

`ifndef def_true
				JX2_ITY_UB:
					opImm = { UV25_00, istrWord[23:16] };
`endif
				JX2_ITY_UW:
					opImm = { UV17_00, istrWord[31:16] };
				JX2_ITY_UL:
					opImm = {    1'b0, istrWord[47:16] };

				JX2_ITY_UQ: begin
					opImm = opImm_imm10u;
					opRegN	= opRegN_Df2;
					opRegO	= opRegN_Df2;
				end

`ifndef def_true
				JX2_ITY_NB:
					opImm = { UV25_FF, istrWord[23:16] };
`endif
				JX2_ITY_NW:
					opImm = { UV17_FF, istrWord[31:16] };
				JX2_ITY_NL:
					opImm = {    1'b1, istrWord[47:16] };

				JX2_ITY_NQ: begin
					opImm = opImm_imm10n;
					opRegN	= opRegN_Df2;
					opRegO	= opRegN_Df2;
				end

`ifndef def_true
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
`endif

				default: begin
					$display("Jx2DecOpFx: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
		end

`ifndef def_true
		JX2_FMID_IMM4ZREG: begin
//			opImm	= {UV23_00, istrWord[25:16]};
			opImm	= opImm_imm10u;
			opRegN	= opRegN_Df2;
//			opImm	= {UV23_00, istrWord[6:5], istrWord[23:16]};
//			opRegN	= opRegO_Dfl;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_IMM4NREG: begin
//			opImm	= {UV23_FF, istrWord[25:16]};
			opImm	= opImm_imm10n;
			opRegN	= opRegN_Df2;
//			opImm	= {UV23_FF, istrWord[6:5], istrWord[23:16]};
//			opRegN	= opRegO_Dfl;
			opRegM	= JX2_REG_IMM;
		end
`endif
		
		/*
			SB: PC, Ro, ZZR
			UB: Rm, #imm8s, Rn
		 */
		JX2_FMID_REGPC: begin
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= JX2_REG_ZZR;
					opRegM	= JX2_REG_PC;
					opRegO	= opRegO_Dfl;
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

		/*
			SB: Fzdd_xxdd		(PC, disp16s), DLR
			SW: Fzdd_xddd		(PC, disp20s), DLR
			SL: Fzzz_dddd_dddd	(PC, disp32s), DLR
		 */
		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_PC;
			opRegO	= JX2_REG_IMM;
//			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
//			opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};

			case(opIty)
			JX2_ITY_SB:
				opImm	= {istrWord[7]?UV17_FF:UV17_00,
					istrWord[ 7: 0],
					istrWord[23:16]};
			JX2_ITY_SW:
				opImm = opImm_disp20s;
			JX2_ITY_SL:
				opImm = { istrWord[47], istrWord[47:16] };

			default: begin
				$display("Jx2DecOpFx: PcDisp, Bad Ity=%X", opIty);
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
//			if(istrWord[15:12]==4'b1111)
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
