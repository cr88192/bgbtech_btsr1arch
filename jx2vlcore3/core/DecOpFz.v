/*
Redo to use new F0 block, and merge F0/F1/F2.
 */

module DecOpFz(
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

reg[5:0]	opRegO_Df2;

reg[32:0]		opImm_imm17s;
reg[32:0]		opImm_imm17u;
reg[32:0]		opImm_imm17n;

reg[32:0]		opImm_imm10s;
reg[32:0]		opImm_imm10u;
reg[32:0]		opImm_imm10n;

reg[32:0]		opImm_disp20s;
reg[32:0]		opImm_disp8s;

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

	tRegRoIsRs	= (istrWord[23:21]==3'b000) || (istrWord[23:20]==4'b1111);
	tRegRnIsRs	= (istrWord[ 7: 5]==3'b000) || (istrWord[ 7: 4]==4'b1111);
	tRegRmIsRs	= (istrWord[ 3: 1]==3'b000) || (istrWord[ 3: 0]==4'b1111);

	opExQ		= istrWord[27];
	opExN		= istrWord[26];
	opExM		= istrWord[25];
	opExI		= istrWord[24];

	opRegN_Dfl	= {tRegRnIsRs, opExN, istrWord[ 7: 4]};
	opRegM_Dfl	= {tRegRmIsRs, opExM, istrWord[ 3: 0]};
	opRegO_Dfl	= {tRegRoIsRs, opExI, istrWord[23:20]};

	opRegO_Df2	= {tRegRmIsRs, istrWord[ 4], istrWord[3:0]};

	opIsNotFx	= (istrWord[15:13]!=3'b111);
	
	tNextMsgLatch	= 0;

	opImm_disp20s = { istrWord[7] ? UV13_FF : UV13_00,
		istrWord[7:0], istrWord[27:16] };

	opImm_imm17u	= {UV24_00, istrWord[24:16]};
	opImm_imm17n	= {UV24_FF, istrWord[24:16]};
	opImm_imm17s	= opImm_imm17u;

	opImm_imm10u	= {UV23_00, istrWord[25:16]};
	opImm_imm10n	= {UV23_FF, istrWord[25:16]};
	opImm_imm10s	= {istrWord[25]?UV23_FF:UV23_00, istrWord[25:16]};

	opImm_disp8s	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};

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
	opIty		= 0;
	opCcty		= JX2_IXC_AL;

	casez(istrWord[11:8])

		4'b0z00:
		casez(istrWord[31:16])
			16'h0zz4: begin		/* F0nm_0eo4 */
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h0zz5: begin		/* F0nm_0eo5 */
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0zz6: begin		/* F0nm_0eo6 */
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0zz7: begin		/* F0nm_0eo7 */
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h0zzC: begin		/* F0nm_0eoC */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h0zzD: begin		/* F0nm_0eoD */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0zzE: begin		/* F0nm_0eoE */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0zzF: begin		/* F0nm_0eoF */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_SL : JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h1zz0: begin		/* F0nm_1eo0 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_ALU_ADD;
			end
			16'h1zz1: begin		/* F0nm_1eo1 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_ALU_SUB;
			end
			16'h1zz2: begin		/* F0nm_1eo2 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_MUL3S;
			end

			16'h1zz5: begin		/* F0nm_1eo5 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_ALU_AND;
			end
			16'h1zz6: begin		/* F0nm_1eo6 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_ALU_OR;
			end
			16'h1zz7: begin		/* F0nm_1eo7 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_ALU_XOR;
			end

			16'h1zz8: begin	/* F0nm_1ez8 */
			end

			16'h1zz9: begin	/* F0nm_1ez9 */
				case(istrWord[23:20])
					4'h0: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_ADD;
					end
					4'h1: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_SUB;
					end
					4'h2: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_ADC;
					end
					4'h3: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_SBB;
					end
					4'h4: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_TSTQ;
						else
							opUCmdIx	= JX2_UCIX_ALU_TST;
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_SB;
					end
					4'h5: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_AND;
					end
					4'h6: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_OR;
					end
					4'h7: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALU_XOR;
					end
					4'h8: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_CONV_MOV;
					end
					4'h9: begin
						opNmid		= JX2_UCMD_MUL3;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_MUL3_MULS;
					end
					4'hA: begin
						opNmid	= JX2_UCMD_MOV_RC;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UB;
					end
					4'hB: begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UW;
					end

					4'hC: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQEQ;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hD: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQHI;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPHI;
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hE: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQGT;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPGT;
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
					end
					4'hF: begin
						opNmid		= JX2_UCMD_MUL3;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_MUL3_MULU;
					end
				endcase
			end

			16'h1zzC: begin	/* F0nm_1ezC */
				case(istrWord[23:20])
					4'h0: begin
//						opNmid	= JX2_UCMD_ALU_NOT;
//						opFmid	= JX2_FMID_REGREG;

//						opNmid		= JX2_UCMD_ALU3;
//						opFmid		= JX2_FMID_REG;
//						opIty		= JX2_ITY_SW;
//						opUCmdIx	= JX2_UCIX_ALU_NOR;

						opNmid		= JX2_UCMD_SHLLN;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= 6'h13;
					end
					4'h1: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SW;
						opUCmdIx	= JX2_UCIX_ALU_SUB;
					end

					4'h5: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_EXTUL;
						else
							opUCmdIx	= JX2_UCIX_CONV_EXTSL;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
					end

					4'h8: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_EXTUB;
						else
							opUCmdIx	= JX2_UCIX_CONV_EXTSB;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h9: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_EXTUW;
						else
							opUCmdIx	= JX2_UCIX_CONV_EXTSW;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
					end

					4'hA: begin
						opNmid	= JX2_UCMD_MOV_RC;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UL;
					end
					4'hB: begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
					end

					default: begin
					end
				endcase
			end

			16'h1zzD: begin	/* F0nm_1ezD */
				case(istrWord[23:20])					
					4'h0: begin
						opNmid		= JX2_UCMD_FLDCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_SG;
					end
					4'h1: begin
						opNmid		= JX2_UCMD_FLDCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_S2G;
					end
					4'h2: begin
						opNmid		= JX2_UCMD_FLDCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_IG;
					end
					4'h3: begin
						opNmid		= JX2_UCMD_FLDCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_HG;
					end

					4'h4: begin
						opNmid		= JX2_UCMD_FSTCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_SG;
					end
					4'h5: begin
						opNmid		= JX2_UCMD_FSTCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_S2G;
					end
					4'h6: begin
						opNmid		= JX2_UCMD_FSTCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_IG;
					end
					4'h7: begin
						opNmid		= JX2_UCMD_FSTCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_HG;
					end
					4'h8: begin
						opNmid		= JX2_UCMD_FIXS;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPIX_FNEG_G;
					end
					4'h9: begin
						opNmid		= JX2_UCMD_FIXS;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPIX_FABS_G;
					end
					4'hA: begin
						opNmid		= JX2_UCMD_FCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_FPU_CMPEQ_G;
					end
					4'hB: begin
						opNmid		= JX2_UCMD_FCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_FPU_CMPGT_G;
					end

					default: begin
					end
				endcase
			end

			16'h3zz0: begin	/* F0zz_1enD */
				casez(istrWord[7:0])

					8'h10: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opBty		= JX2_BTY_SW;
					end
					8'h11: begin
						opNmid		= JX2_UCMD_BSR;
						opFmid		= JX2_FMID_REGPC;
						opBty		= JX2_BTY_SW;
					end
					8'h12: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opBty		= JX2_BTY_SW;
						opCcty		= JX2_IXC_CT;
					end
					8'h13: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opBty		= JX2_BTY_SW;
						opCcty		= JX2_IXC_CF;
					end


					8'h18: begin
						opNmid		= JX2_UCMD_PUSHX;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_PUSH_GR;
					end
					8'h19: begin
						opNmid		= JX2_UCMD_PUSHX;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_PUSH_CR;
					end
					8'h1A: begin
						opNmid		= JX2_UCMD_POPX;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_PUSH_GR;
					end
					8'h1B: begin
						opNmid		= JX2_UCMD_POPX;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_PUSH_GR;
					end

					8'h1C: begin
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALU_CMPGT;
					end
					8'h1D: begin
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALU_CMPGE;
					end

					8'h20: begin
						opNmid		= JX2_UCMD_JMP;
						opFmid		= JX2_FMID_REG;
					end
					8'h21: begin
						opNmid		= JX2_UCMD_JSR;
						opFmid		= JX2_FMID_REG;
					end
					8'h22: begin
						opNmid		= JX2_UCMD_JMP;
						opFmid		= JX2_FMID_REG;
						opCcty		= JX2_IXC_CT;
					end
					8'h23: begin
						opNmid		= JX2_UCMD_JMP;
						opFmid		= JX2_FMID_REG;
						opCcty		= JX2_IXC_CF;
					end

					default: begin
					end

				endcase
			end

			16'h3zz7: begin
				opNmid		= JX2_UCMD_SWAPN;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= { 2'h0, istrWord[3:0] };
			end

			16'h5zz0: begin		/* F0nm_5eo0 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_UB;
				opUCmdIx	= JX2_UCIX_ALU_CSELT;
			end

			16'h5zz4: begin		/* F0nm_5eo4 */
				opNmid	= opExQ ? JX2_UCMD_SHADQ3 : JX2_UCMD_SHAD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h5zz5: begin		/* F0nm_5eo5 */
				opNmid	= opExQ ? JX2_UCMD_SHLDQ3 : JX2_UCMD_SHLD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h5zz8: begin		/* F0nm_5eo8 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FADD_G;
			end
			16'h5zz9: begin		/* F0nm_5eo8 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FSUB_G;
			end
			16'h5zzA: begin		/* F0nm_5eo8 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FMUL_G;
			end

			16'h5zzC: begin		/* F0nm_5eoC */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ? JX2_UCIX_ALU_ADDUL : JX2_UCIX_ALU_ADDSL;
			end
			16'h5zzD: begin		/* F0nm_5eoD */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ? JX2_UCIX_ALU_SUBUL : JX2_UCIX_ALU_SUBSL;
			end


// `ifdef jx2_enable_fpu
`ifdef jx2_enable_fprs
			16'h9zz0: begin		/* F0nm_9eo0 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz1: begin		/* F0nm_9eo1 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz2: begin		/* F0nm_9eo2 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz3: begin		/* F0nm_9eo3 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz4: begin		/* F0nm_9eo4 */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz5: begin		/* F0nm_9eo5 */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz6: begin		/* F0nm_9eo6 */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h9zz7: begin		/* F0nm_9eo7 */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h9zz8: begin		/* F0nm_9eo8 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FADD;
			end
			16'h9zz9: begin		/* F0nm_9eo9 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FSUB;
			end
			16'h9zzA: begin		/* F0nm_9eoA */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FMUL;
			end

// `ifndef JX2_FPU_NOFDIV
`ifndef def_true
			16'h9zzB: begin		/* F0nm_9eoB */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FDIV;
			end
`endif

			16'h9zzC: begin		/* F0nm_9ezC */
				case(istrWord[23:20])
					4'hE: begin
						opNmid		= JX2_UCMD_FPU3;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_FPU_FMOV;
					end
					default: begin
					end
				endcase
			end

			16'h9zzF: begin		/* F0nm_9ezF */
				case(istrWord[23:20])
					4'h1: begin
						opNmid		= JX2_UCMD_FLDCX;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_FPCX_D;
					end

					4'h5: begin
						opNmid		= JX2_UCMD_FSTCX;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_FPCX_D;
					end

					default: begin
					end
				endcase
			end
`endif

			16'hCzzz: begin		/* F0dd_Cddd */
				opNmid	= JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
			end
			16'hDzzz: begin		/* F0dd_Dddd */
				opNmid	= JX2_UCMD_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
			end
			16'hEzzz: begin		/* F0dd_Eddd */
				opNmid	= JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
				opCcty	= JX2_IXC_CT;
			end
			16'hFzzz: begin		/* F0dd_Fddd */
				opNmid	= JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
				opCcty	= JX2_IXC_CF;
			end

			default: begin
			end
		endcase

		4'b0z01: begin		/* F1nm_Xedd */

			case(istrWord[31:28])
				4'h0: begin
					if(opExQ)
					begin
						opNmid	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SB;
				end
				4'h1: begin
					if(opExQ)
					begin
						opNmid	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SW;
				end
				4'h2: begin
					if(opExQ)
					begin
						opNmid	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SL;
				end
				4'h3: begin
					if(opExQ)
					begin
						opNmid	= JX2_UCMD_LEA_MR;
						opFmid	= JX2_FMID_LDREGDISPREG;
					end else begin
						opNmid	= JX2_UCMD_MOV_RM;
						opFmid	= JX2_FMID_REGSTREGDISP;
					end
					opIty	= JX2_ITY_SW;
					opBty	= JX2_BTY_SQ;
				end

				4'h8: begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
				end
				4'h9: begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
				end
				4'hA: begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
				end
				4'hB: begin
					opNmid	= JX2_UCMD_MOV_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SW;
					opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SQ;
				end

`ifdef JX2_EX_ALU_JMPCC
// `ifndef def_true
				4'hC: begin		/* F0dd_Cenm */
					opNmid		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b11, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
				4'hD: begin		/* F0dd_Denm */
					opNmid		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b00, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
				4'hE: begin		/* F0dd_Eenm */
					opNmid		= JX2_UCMD_CF_JCMPCC;
					opFmid		= JX2_FMID_REGPC;
					opUCmdIx	= { 4'h0, istrWord[11], 2'b10, istrWord[8]};
					opIty		= JX2_ITY_UB;
				end
				4'hF: begin		/* F0dd_Fenm */
					opNmid		= JX2_UCMD_CF_JCMPCC;
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
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_ALU_ADD;
				end
				4'h1: begin		/* F2nm_1ejj */
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_NW;
					opUCmdIx	= JX2_UCIX_ALU_ADD;
				end
				4'h2: begin		/* F2nm_2ejj */
					opNmid		= JX2_UCMD_MUL3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_MUL3_MUL3S;
				end
`endif

// `ifdef JX2_EX_ALU_ADDSL
`ifdef def_true
				4'h3: begin		/* F1ej_Cjnm */
					opNmid	= JX2_UCMD_ALU3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
					opUCmdIx	= opExQ ?
						JX2_UCIX_ALU_ADDUL :
						JX2_UCIX_ALU_ADDSL;
				end
				4'h4: begin		/* F1ej_Djnm */
					opNmid	= JX2_UCMD_ALU3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_NW;
					opUCmdIx	= opExQ ?
						JX2_UCIX_ALU_ADDUL :
						JX2_UCIX_ALU_ADDSL;
				end
`endif

`ifdef def_true
				4'h5: begin		/* F2nm_5ejj */
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_ALU_AND;
				end
				4'h6: begin		/* F2nm_6ejj */
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_ALU_OR;
				end
				4'h7: begin		/* F2nm_7ejj */
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_ALU_XOR;
				end
`endif

`ifdef def_true
				4'h8: begin		/* F2nm_8ejj */
					opNmid	= opExQ ?
						JX2_UCMD_SHADQ3 :
						JX2_UCMD_SHAD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
				4'h9: begin		/* F2nm_9ejj */
					opNmid	= opExQ ?
						JX2_UCMD_SHLDQ3 :
						JX2_UCMD_SHLD3;
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
							opNmid	= JX2_UCMD_MOV_IR;
						end

						4'b010z: begin
							opNmid	= JX2_UCMD_ALUCMP;
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_TSTQ :
								JX2_UCIX_ALU_TST;
						end
						4'b011z: begin
							opNmid	= JX2_UCMD_ALUCMP;
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQHS :
								JX2_UCIX_ALU_CMPHS;
						end
						4'b100z: begin
							opNmid	= JX2_UCMD_ALUCMP;
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQHI :
								JX2_UCIX_ALU_CMPHI;
						end
						4'b101z: begin
							opNmid	= JX2_UCMD_ALUCMP;
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQGE :
								JX2_UCIX_ALU_CMPGE;
						end
						4'b110z: begin
							opNmid	= JX2_UCMD_ALUCMP;
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQEQ :
								JX2_UCIX_ALU_CMPEQ;
						end
						4'b111z: begin
							opNmid	= JX2_UCMD_ALUCMP;
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQGT :
								JX2_UCIX_ALU_CMPGT;
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

//		4'h8: begin		/* F8zz_zzzz */
		4'b100z: begin		/* F8zz_zzzz */
			casez(istrWord[7:5])
				3'b000: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
				end
				3'b001: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_NW;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
				end
				3'b010: begin
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SW;
					opUCmdIx	= JX2_UCIX_ALU_ADD;
				end
				3'b011: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_LDI_LDISH16;
				end

`ifndef def_true
				3'b111: begin
					case(istrWord[4:0])
						5'h17: begin
							opNmid		= JX2_UCMD_OP_IXT;
							opUCmdIx	= JX2_UCMD_IX_TRAPB;
							opFmid		= JX2_FMID_IMM8REG;
							opIty		= JX2_ITY_UW;
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

		4'hA: begin		/* FAjj_jjjj */
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM12Z;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end
		4'hB: begin		/* FBjj_jjjj */
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM12N;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end

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

		JX2_FMID_REG: begin
			opUIxt	= { opCcty, opUCmdIx };
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegO_Dfl;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegO_Dfl;
				end

				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_GR_ZZR;
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
			UB: Rm, Rn, Rn
			NB: Rn, Rm, Rn
		*/

		JX2_FMID_REGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;
			opImm	= {UV28_00, istrWord[4:0]};
			opUIxt	= { opCcty, opUCmdIx };

			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_UB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
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

		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= { opCcty, opUCmdIx };

			case(opIty)
				JX2_ITY_SB: begin
//					opImm	= opImm_imm17s;
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

`ifndef def_true
		JX2_FMID_IMM8Z: begin
//			opImm	= {UV25_00, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
			opUIxt	= { opCcty, opUCmdIx };

			opImm = {
				UV9_00,
				istrWord[23:16],
				istrWord[47:32] };
		end
		JX2_FMID_IMM8N: begin
//			opImm	= {UV25_FF, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
			opUIxt	= { opCcty, opUCmdIx };

			opImm = {
				UV9_FF,
				istrWord[23:16],
				istrWord[47:32] };
		end
`endif

`ifdef def_true
		/*
			SB: (Rm), Rn
		 */
		JX2_FMID_LDREGREG: begin
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};

			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_ZZR;
			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
				opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty[2], 2'b00};
			end

		end

		/*
			SW: Rm, (Rn, Disp9u/17s)
		 */
		JX2_FMID_LDREGDISPREG: begin
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			opImm	= opImm_imm17s;

			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
				opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty[2], 2'b00};
			end
		end
`endif

		JX2_FMID_LDDRREGREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;

				if(tRegRmIsRz)
				begin
					opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty[2], 2'b00};
					if(tRegRmIsR1)
					begin
						if(tRegRoIsR1)
						begin
							opRegM	= JX2_GR_TBR;
							opRegO	= JX2_GR_DLR;
						end else begin
							opRegM	= JX2_GR_GBR;
							opRegO	= opRegO_Dfl;
						end
					end else begin
						if(tRegRoIsR1)
						begin
							opRegM	= JX2_GR_DLR;
							opRegO	= JX2_GR_ZZR;
							opUIxt	= {opCcty, opBty[1:0], 1'b1,
								opBty[2], 2'b00};
						end else begin
							opRegM	= JX2_GR_PC;
							opRegO	= opRegO_Dfl;
						end
					end
				end
				else
				begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegO_Dfl;
					opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
				end

			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= JX2_GR_DLR;
				opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
			end
		end

		/*
			SB: Fzeo_zzii		Imm8s, Ro
			SW: Fzeo_iiii		Imm16s, Ro

			SB: Fzeo_zzjj		Imm8u, Ro
			SW: Fzeo_jjjj		Imm16u, Ro

			NB: Fzeo_zzjj		Imm8n, Ro
			NW: Fzeo_jjjj		Imm16n, Ro

			XB: Fzeo_zzjj		Imm8x, Ro
			XW: Fzeo_jjjj		Imm16x, Ro

			SQ: Fzze_zznz_iiii	Imm17s, Rn
			UQ: Fzze_zznz_iiii	Imm17u, Rn
			NQ: Fzze_zznz_iiii	Imm17n, Rn
		 */
	
		JX2_FMID_IMM8REG: begin
			opUIxt	= { opCcty, opUCmdIx };

			opRegM	= JX2_GR_IMM;
			opRegN	= opRegO_Df2;
			opRegO	= opRegO_Df2;
			
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

				JX2_ITY_SQ: begin
					opImm = opImm_imm10s;
					opRegN	= opRegN_Dfl;
					opRegO	= opRegN_Dfl;
				end

`ifndef def_true
				JX2_ITY_UB:
					opImm = { UV25_00, istrWord[23:16] };
`endif
				JX2_ITY_UW:
					opImm = { UV17_00, istrWord[31:16] };

				JX2_ITY_UQ: begin
					opImm = opImm_imm10u;
					opRegN	= opRegN_Dfl;
					opRegO	= opRegN_Dfl;
				end

`ifndef def_true
				JX2_ITY_NB:
					opImm = { UV25_FF, istrWord[23:16] };
`endif
				JX2_ITY_NW:
					opImm = { UV17_FF, istrWord[31:16] };

				JX2_ITY_NQ: begin
					opImm = opImm_imm10n;
					opRegN	= opRegN_Dfl;
					opRegO	= opRegN_Dfl;
				end

				default: begin
					$display("Jx2DecOpFx: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
		end

`ifndef def_true
		JX2_FMID_IMM4ZREG: begin
			opImm	= opImm_imm10u;
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_GR_IMM;
		end

		JX2_FMID_IMM4NREG: begin
			opImm	= opImm_imm10n;
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_GR_IMM;
		end
`endif
		
		/*
			SB: PC, Ro, ZZR
			UB: Rm, #imm8s, Rn
		 */
		JX2_FMID_REGPC: begin
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= JX2_GR_ZZR;
					opRegM	= JX2_GR_PC;
					opRegO	= opRegO_Dfl;
					opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};
				end

`ifdef JX2_EX_ALU_JMPCC
				JX2_ITY_UB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_IMM;
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
		 */
		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_IMM;
			opUIxt	= {opCcty, opBty[1:0], 1'b1, opBty};

			case(opIty)
			JX2_ITY_SB:
				opImm	= {istrWord[7]?UV17_FF:UV17_00,
					istrWord[ 7: 0],
					istrWord[23:16]};
			JX2_ITY_SW:
				opImm = opImm_disp20s;

			default: begin
				$display("Jx2DecOpFx: PcDisp, Bad Ity=%X", opIty);
			end

			endcase
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV9_00, istrWord[7:0], istrWord[31:16]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV9_FF, istrWord[7:0], istrWord[31:16]};
			opRegN	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
		end
		
		JX2_FMID_INV: begin
//			if(istrWord[15:12]==4'b1111)
			if(!opIsNotFx)
			begin
				opUCmd = { opCcty, JX2_UCMD_INVOP };
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
			opUCmd = { opCcty, JX2_UCMD_INVOP };
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
