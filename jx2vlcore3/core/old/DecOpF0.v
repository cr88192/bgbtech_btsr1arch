module DecOpF0(
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

reg[5:0]	opRegO_Dfl;
reg[5:0]	opRegN_Dfl;
reg[5:0]	opRegM_Dfl;

reg[5:0]	opRegN_Cr;
reg[5:0]	opRegM_Cr;
reg[5:0]	opRegN_Sr;
reg[5:0]	opRegM_Sr;

reg[32:0]	opImm_disp20s;

reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[1:0]	opCcty;

reg[7:0]	opUCmdIx;

reg		opExQ;
reg		opExN;
reg		opExM;
reg		opExI;
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

reg tRegRmIsRs;
reg tRegRnIsRs;
reg tRegRoIsRs;

reg	tMsgLatch;
reg	tNextMsgLatch;

always @*
begin

	tRegRoIsRs	= (istrWord[ 3: 1]==3'b000) || (istrWord[ 3: 0]==4'b1111);
	tRegRnIsRs	= (istrWord[23:21]==3'b000) || (istrWord[23:20]==4'b1111);
	tRegRmIsRs	= (istrWord[19:17]==3'b000) || (istrWord[19:16]==4'b1111);

	opExQ		= istrWord[7];
	opExN		= istrWord[6];
	opExM		= istrWord[5];
	opExI		= istrWord[4];

	opRegO_Dfl	= {tRegRoIsRs, istrWord[4], istrWord[ 3: 0]};
	opRegN_Dfl	= {tRegRnIsRs, istrWord[6], istrWord[23:20]};
	opRegM_Dfl	= {tRegRmIsRs, istrWord[5], istrWord[19:16]};

	opRegN_Cr	= {1'b1, istrWord[6], istrWord[23:20]};
	opRegM_Cr	= {1'b1, istrWord[5], istrWord[19:16]};

	opIsNotFx	= (istrWord[15:8]!=8'b11z10z00);
	
	tNextMsgLatch	= 0;

	opImm_disp20s = { istrWord[7] ? UV13_FF : UV13_00,
		istrWord[7:0], istrWord[27:16] };

	tRegRnIsRz	= (opRegN_Dfl[4:1]==4'b0000);
	tRegRmIsRz	= (opRegM_Dfl[4:1]==4'b0000);
	tRegRoIsRz	= (opRegO_Dfl[4:1]==4'b0000);
	tRegRnIsR0	= tRegRnIsRz && !opRegN_Dfl[0];
	tRegRnIsR1	= tRegRnIsRz &&  opRegN_Dfl[0];
	tRegRmIsR0	= tRegRmIsRz && !opRegM_Dfl[0];
	tRegRmIsR1	= tRegRmIsRz &&  opRegM_Dfl[0];
	tRegRoIsR0	= tRegRoIsRz && !opRegO_Dfl[0];
	tRegRoIsR1	= tRegRoIsRz &&  opRegO_Dfl[0];

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
		end
		16'h11zz: begin		/* F0ez_11zz */
			opUCmd		= JX2_UCMD_ALU_SUB3;
			opFmid		= JX2_FMID_REGREG;
		end
		16'h12zz: begin		/* F0ez_12zz */
			opUCmd	= JX2_UCMD_ALU_MUL3;
			opFmid	= JX2_FMID_REGREG;
		end

		16'h15zz: begin		/* F0ez_15zz */
			opUCmd	= JX2_UCMD_ALU_AND3;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h16zz: begin		/* F0ez_16zz */
			opUCmd	= JX2_UCMD_ALU_OR3;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h17zz: begin		/* F0ez_17zz */
			opUCmd	= JX2_UCMD_ALU_XOR3;
			opFmid	= JX2_FMID_REGREG;
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
			NB: Rn, Rm, Rn
		*/

		JX2_FMID_REGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;
			opUIxt	= opUCmdIx;

			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
				end
				
				default: begin
					$display("Jx2DecOpFx: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end

		JX2_FMID_REGSTDRREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegM_Dfl;

				if(tRegRnIsRz)
				begin
					opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
					if(tRegRnIsR1)
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
							opUIxt	= {opCcty, JX2_IX2_RRI, 1'b0, JX2_BTY_SB};
						end else begin
							opRegM	= JX2_REG_PC;
							opRegO	= opRegO_Dfl;
						end
					end
				end
				else
				begin
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
			SB: PC, Ro, ZZR
		 */
		JX2_FMID_REGPC: begin
			opRegN	= JX2_REG_ZZR;
			opRegM	= JX2_REG_PC;
			opRegO	= opRegO_Dfl;
			opUIxt	= {opCcty, JX2_IX2_RRI, 1'b1, opBty};
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
