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
output[31:0]	idImm;
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
reg[31:0]		opImm;
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
			16'h00zz: begin
				opUCmd	= JX2_UCMD_MOVB_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h01zz: begin
				opUCmd	= JX2_UCMD_MOVW_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h02zz: begin
				opUCmd	= JX2_UCMD_MOVL_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h03zz: begin
				opUCmd	= JX2_UCMD_MOVQ_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h04zz: begin
				opUCmd	= JX2_UCMD_MOVB_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h05zz: begin
				opUCmd	= JX2_UCMD_MOVW_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h06zz: begin
				opUCmd	= JX2_UCMD_MOVL_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h07zz: begin
				opUCmd	= JX2_UCMD_MOVQ_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h08zz: begin
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUB_MR;
				else
					opUCmd	= JX2_UCMD_MOVB_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h09zz: begin
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUW_MR;
				else
					opUCmd	= JX2_UCMD_MOVW_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0Azz: begin
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUL_MR;
				else
					opUCmd	= JX2_UCMD_MOVL_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0Bzz: begin
				opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h0Czz: begin
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUB_MR;
				else
					opUCmd	= JX2_UCMD_MOVB_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h0Dzz: begin
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUW_MR;
				else
					opUCmd	= JX2_UCMD_MOVW_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0Ezz: begin
				if(opExQ)
					opUCmd	= JX2_UCMD_MOVUL_MR;
				else
					opUCmd	= JX2_UCMD_MOVL_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0Fzz: begin
				opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
			end

			16'h10zz: begin
				opUCmd	= JX2_UCMD_ALU_ADD3;
				opFmid	= JX2_FMID_REGREG;
			end
			16'h11zz: begin
				opUCmd	= JX2_UCMD_ALU_SUB3;
				opFmid	= JX2_FMID_REGREG;
			end
			16'h12zz: begin
				opUCmd	= JX2_UCMD_ALU_MUL3;
				opFmid	= JX2_FMID_REGREG;
			end

			16'h15zz: begin
				opUCmd	= JX2_UCMD_ALU_AND3;
				opFmid	= JX2_FMID_REGREG;
			end
			16'h16zz: begin
				opUCmd	= JX2_UCMD_ALU_OR3;
				opFmid	= JX2_FMID_REGREG;
			end
			16'h17zz: begin
				opUCmd	= JX2_UCMD_ALU_XOR3;
				opFmid	= JX2_FMID_REGREG;
			end

			16'h18zz: begin	/* F0ez_18nm */
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
			end

			16'h19zz: begin	/* F0ez_19nm */
				case(istrWord[3:0])
					4'h0: begin
						opUCmd	= JX2_UCMD_ALU_ADD;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h1: begin
						opUCmd	= JX2_UCMD_ALU_SUB;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h2: begin
						opUCmd	= JX2_UCMD_ALU_ADC;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h3: begin
						opUCmd	= JX2_UCMD_ALU_SBB;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h4: begin
						if(opExQ)
							opUCmd	= JX2_UCMD_ALU_TSTQ;
						else
							opUCmd	= JX2_UCMD_ALU_TST;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h5: begin
						opUCmd	= JX2_UCMD_ALU_AND;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h6: begin
						opUCmd	= JX2_UCMD_ALU_OR;
						opFmid	= JX2_FMID_REGREG;
					end
					4'h7: begin
						opUCmd	= JX2_UCMD_ALU_XOR;
						opFmid	= JX2_FMID_REGREG;
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
						opUCmd	= JX2_UCMD_MOV_RR;
						opFmid	= JX2_FMID_REGREG;
						opBty	= JX2_BTY_UB;
					end
					4'hB: begin
						opUCmd	= JX2_UCMD_MOV_RR;
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

			16'h1Dzz: begin	/* F0ez_19nm */
				case(istrWord[3:0])
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
					
					default: begin
					end
				endcase
			end

			default: begin
			end
		endcase

		4'h8: begin
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
					opUCmd		= JX2_UCMD_ALU_ADD;
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

		4'b110z:
		casez(istrWord[7:0])
			8'hCz: begin
				opUCmd		= JX2_UCMD_ALU_ADD;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_SL;
				opIsImm32	= 1;
			end
			8'hDz: begin
				opUCmd		= JX2_UCMD_ALU_LDISH32;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_UL;
				opIsImm32	= 1;
			end
			8'hEz: begin
				opUCmd		= JX2_UCMD_MOV_IR;
				opFmid		= JX2_FMID_IMM8REG;
				opIty		= JX2_ITY_SL;
				opIsImm32	= 1;
			end

			default: begin
			end

		endcase

		default: begin
		end
	endcase
	
	case(opFmid)
		JX2_FMID_REGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;
		end
	
		JX2_FMID_REGSTREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opImm	= {UV27_00, istrWord[4:0]};
				opUIxt	= {JX2_IXT_RDI, 1'b0, opBty};
			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opUIxt	= {JX2_IXT_REG, 1'b0, opBty};
			end
		end
		JX2_FMID_LDREGREG: begin
			if(opIty==JX2_ITY_UB)
			begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opImm	= {UV27_00, istrWord[4:0]};
				opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
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
	
		JX2_FMID_IMM8REG: begin
			case(opIty)
				JX2_ITY_SB:
					opImm = {
						istrWord[23] ? UV24_FF : UV24_00,
						istrWord[23:16] };
				JX2_ITY_SW:
					opImm = {
						istrWord[31] ? UV16_FF : UV16_00,
						istrWord[31:16] };
				JX2_ITY_SL:
					opImm = istrWord[47:16];

				JX2_ITY_UB:
					opImm = { UV24_00, istrWord[23:16] };
				JX2_ITY_UW:
					opImm = { UV16_00, istrWord[31:16] };
				JX2_ITY_UL:
					opImm = istrWord[47:16];

				JX2_ITY_NB:
					opImm = { UV24_FF, istrWord[23:16] };
				JX2_ITY_NW:
					opImm = { UV16_FF, istrWord[31:16] };
				JX2_ITY_NL:
					opImm = istrWord[47:16];

				default: begin
				end
			endcase
			
			if(opIsImm32 || opIsImm16)
			begin
				opRegM	= JX2_REG_IMM;
				opRegN	= opRegO_DfFC;
			end
			else
			begin
				opRegM	= JX2_REG_IMM;
				opRegN	= opRegO_Dfl;
			end
		end
		default: begin
		end
	endcase
end

endmodule
