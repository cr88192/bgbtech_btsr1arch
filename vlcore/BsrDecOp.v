/*
BSR1 Instruction Decoder

Only takes a 16 bits instruction word as ops are fixed width and
functionally independent.
 */

`include "BsrCoreDefs.v"

module BsrDecOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,
	idRegN,		idRegM,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[15:0]		istrWord;	//source instruction word

output[5:0]		idRegN;
output[5:0]		idRegM;
output[31:0]	idImm;
output[7:0]		idUCmd;
output[7:0]		idUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

reg[5:0]		opRegN;
reg[5:0]		opRegM;
reg[31:0]		opImm;
reg[7:0]		opUCmd;
reg[7:0]		opUIxt;

reg[5:0]	opRegO_Dfl;
reg[5:0]	opRegN_Dfl;
reg[5:0]	opRegM_Dfl;

reg[5:0]	opRegN_Cr;
reg[5:0]	opRegM_Cr;
reg[5:0]	opRegN_Sr;
reg[5:0]	opRegM_Sr;

reg[4:0]	opFmid;
reg[2:0]	opBty;

reg[7:0]	opUCmdIx;

// reg opIs32;

always @*
begin

	opRegO_Dfl	= {2'b00, istrWord[11:8]};
	opRegN_Dfl	= {2'b00, istrWord[ 7:4]};
	opRegM_Dfl	= {2'b00, istrWord[ 3:0]};
	opRegN_Cr	= {2'b01, istrWord[ 7:4]};
	opRegM_Cr	= {2'b01, istrWord[ 3:0]};
	opRegN_Sr	= {2'b10, istrWord[ 7:4]};
	opRegM_Sr	= {2'b10, istrWord[ 3:0]};

	opUCmd		= BSR_UCMD_INVOP;
	opRegN		= BSR_REG_ZZR;
	opRegM		= BSR_REG_ZZR;
	opImm		= 0;
	opFmid		= BSR_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord)

		16'h00zz: begin
			opUCmd	= BSR_UCMD_MOVB_RM;
			opFmid	= BSR_FMID_REGSTREG;
			opBty	= BSR_BTY_SB;
		end
		16'h01zz: begin
			opUCmd	= BSR_UCMD_MOVW_RM;
			opFmid	= BSR_FMID_REGSTREG;
			opBty	= BSR_BTY_SW;
		end
		16'h02zz: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTREG;
			opBty	= BSR_BTY_SL;
		end
		16'h03zz: begin
			opUCmd	= BSR_UCMD_MOVUB_MR;
			opFmid	= BSR_FMID_LDREGREG;
			opBty	= BSR_BTY_UB;
		end

		16'h04zz: begin
			opUCmd	= BSR_UCMD_MOVB_RM;
			opFmid	= BSR_FMID_REGSTDRREG;
			opBty	= BSR_BTY_SB;
		end
		16'h05zz: begin
			opUCmd	= BSR_UCMD_MOVW_RM;
			opFmid	= BSR_FMID_REGSTDRREG;
			opBty	= BSR_BTY_SW;
		end
		16'h06zz: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDRREG;
			opBty	= BSR_BTY_SL;
		end
		16'h07zz: begin
			opUCmd	= BSR_UCMD_MOVUB_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_UB;
		end

		16'h08zz: begin
			opUCmd	= BSR_UCMD_MOVB_MR;
			opFmid	= BSR_FMID_LDREGREG;
			opBty	= BSR_BTY_SB;
		end
		16'h09zz: begin
			opUCmd	= BSR_UCMD_MOVW_MR;
			opFmid	= BSR_FMID_LDREGREG;
			opBty	= BSR_BTY_SW;
		end
		16'h0Azz: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDREGREG;
			opBty	= BSR_BTY_SL;
		end
		16'h0Bzz: begin
			opUCmd	= BSR_UCMD_MOVUW_MR;
			opFmid	= BSR_FMID_LDREGREG;
			opBty	= BSR_BTY_UW;
		end

		16'h0Czz: begin
			opUCmd	= BSR_UCMD_MOVB_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SB;
		end
		16'h0Dzz: begin
			opUCmd	= BSR_UCMD_MOVW_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SW;
		end
		16'h0Ezz: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SL;
		end
		16'h0Fzz: begin
			opUCmd	= BSR_UCMD_MOVUW_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_UW;
		end


		16'h10zz: begin
			opUCmd	= BSR_UCMD_ALU_ADD;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h11zz: begin
			opUCmd	= BSR_UCMD_ALU_SUB;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h12zz: begin
			opUCmd	= BSR_UCMD_ALU_ADC;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h13zz: begin
			opUCmd	= BSR_UCMD_ALU_SBB;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h14zz: begin
			opUCmd	= BSR_UCMD_ALU_TST;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h15zz: begin
			opUCmd	= BSR_UCMD_ALU_AND;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h16zz: begin
			opUCmd	= BSR_UCMD_ALU_OR;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h17zz: begin
			opUCmd	= BSR_UCMD_ALU_XOR;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h18zz: begin
			opUCmd	= BSR_UCMD_MOV_RR;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h19zz: begin
			opUCmd	= BSR_UCMD_ALU_MULS;
			opFmid	= BSR_FMID_REGREG;
		end

		16'h1Azz: begin
			opUCmd	= BSR_UCMD_MOV_RR;
			opFmid	= BSR_FMID_REGREG;
			opBty	= BSR_BTY_UB;
		end
		16'h1Bzz: begin
			opUCmd	= BSR_UCMD_MOV_RR;
			opFmid	= BSR_FMID_REGREG;
			opBty	= BSR_BTY_UW;
		end

		16'h1Czz: begin
			opUCmd	= BSR_UCMD_ALU_CMPEQ;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h1Dzz: begin
			opUCmd	= BSR_UCMD_ALU_CMPHI;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h1Ezz: begin
			opUCmd	= BSR_UCMD_ALU_CMPGT;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h1Fzz: begin
			opUCmd	= BSR_UCMD_ALU_MULU;
			opFmid	= BSR_FMID_REGREG;
		end

		16'h20zz: begin
			opUCmd	= BSR_UCMD_CF_BRA;
			opFmid	= BSR_FMID_PCDISP8;
			opBty	= BSR_BTY_SW;
		end
		16'h21zz: begin
			opUCmd	= BSR_UCMD_CF_BSR;
			opFmid	= BSR_FMID_PCDISP8;
			opBty	= BSR_BTY_SW;
		end
		16'h22zz: begin
			opUCmd	= BSR_UCMD_CF_BT;
			opFmid	= BSR_FMID_PCDISP8;
			opBty	= BSR_BTY_SW;
		end
		16'h23zz: begin
			opUCmd	= BSR_UCMD_CF_BF;
			opFmid	= BSR_FMID_PCDISP8;
			opBty	= BSR_BTY_SW;
		end
		16'h24zz: begin
			opUCmd	= BSR_UCMD_ALU_LDIX;
			opFmid	= BSR_FMID_IMM8Z;
		end
		16'h25zz: begin
			opUCmd	= BSR_UCMD_ALU_LDIX;
			opFmid	= BSR_FMID_IMM8N;
		end
		16'h26zz: begin
			opUCmd	= BSR_UCMD_ALU_LDISH;
			opFmid	= BSR_FMID_IMM8Z;
		end

		16'h28zz: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDI4SP;
			opBty	= BSR_BTY_SL;
		end
		16'h29zz: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDI4SP;
			opBty	= BSR_BTY_UL;
		end
		16'h2Azz: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDI4SPREG;
			opBty	= BSR_BTY_SL;
		end
		16'h2Bzz: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDI4SPREG;
			opBty	= BSR_BTY_UL;
		end
		16'h2Czz: begin
			opUCmd	= BSR_UCMD_ALU_CMPEQ;
			opFmid	= BSR_FMID_IMM4ZREG;
		end
		16'h2Dzz: begin
			opUCmd	= BSR_UCMD_ALU_CMPEQ;
			opFmid	= BSR_FMID_IMM4NREG;
		end
		16'h2Ezz: begin
			opUCmd	= BSR_UCMD_ALU_CMPGT;
			opFmid	= BSR_FMID_IMM4ZREG;
		end
		16'h2Fzz: begin
			opUCmd	= BSR_UCMD_ALU_CMPGE;
			opFmid	= BSR_FMID_IMM4ZREG;
		end

		16'h3000: begin
			opUCmd	= BSR_UCMD_NOP;
			opFmid	= BSR_FMID_Z;
		end
		16'h3010: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_RTS;
		end
		16'h3020: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_SLEEP;
		end
		16'h3030: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_BREAK;
		end
		16'h3040: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_CLRT;
		end
		16'h3050: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_SETT;
		end
		16'h3060: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_CLRS;
		end
		16'h3070: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_SETS;
		end
		16'h3080: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_NOTT;
		end
		16'h3090: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_NOTS;
		end

		16'h30A0: begin
			opUCmd	= BSR_UCMD_ALU_LDISH16;
			opFmid	= BSR_FMID_Z;
			opImm	= 32'h00000000;
		end
		16'h30B0: begin
			opUCmd	= BSR_UCMD_ALU_LDISH16;
			opFmid	= BSR_FMID_Z;
			opImm	= 32'h0000FFFF;
		end

		16'h30C0: begin
			opUCmd		= BSR_UCMD_OP_IXT;
			opFmid		= BSR_FMID_Z;
			opUCmdIx	= BSR_UCMD_IX_RTE;
		end

		16'h3002: begin
			opUCmd		= BSR_UCMD_CF_BRA;
			opFmid		= BSR_FMID_DRPC;
			opBty		= BSR_BTY_SW;
		end
		16'h3012: begin
			opUCmd		= BSR_UCMD_CF_BSR;
			opFmid		= BSR_FMID_DRPC;
			opBty		= BSR_BTY_SW;
		end
		16'h3022: begin
			opUCmd		= BSR_UCMD_CF_BT;
			opFmid		= BSR_FMID_DRPC;
			opBty		= BSR_BTY_SW;
		end
		16'h3032: begin
			opUCmd		= BSR_UCMD_CF_BF;
			opFmid		= BSR_FMID_DRPC;
			opBty		= BSR_BTY_SW;
		end


		16'h30z4: begin
			opUCmd	= BSR_UCMD_MOVB_RM;
			opFmid	= BSR_FMID_REGSTDRPC;
			opBty	= BSR_BTY_SB;
		end
		16'h30z5: begin
			opUCmd	= BSR_UCMD_MOVW_RM;
			opFmid	= BSR_FMID_REGSTDRPC;
			opBty	= BSR_BTY_SB;
		end
		16'h30z6: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDRPC;
			opBty	= BSR_BTY_SB;
		end
		16'h30z7: begin
			opUCmd	= BSR_UCMD_MOVUB_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SB;
		end

		16'h30z8: begin
			opUCmd	= BSR_UCMD_MOV_PUSH;
			opFmid	= BSR_FMID_REG;
			opBty	= BSR_BTY_SL;
		end
		16'h30z9: begin
			opUCmd	= BSR_UCMD_MOV_PUSH;
			opFmid	= BSR_FMID_REG;
			opBty	= BSR_BTY_UB;
		end
		16'h30zA: begin
			opUCmd	= BSR_UCMD_MOV_POP;
			opFmid	= BSR_FMID_REG;
			opBty	= BSR_BTY_SL;
		end
		16'h30zB: begin
			opUCmd	= BSR_UCMD_MOV_POP;
			opFmid	= BSR_FMID_REG;
			opBty	= BSR_BTY_UB;
		end

		16'h30zC: begin
			opUCmd	= BSR_UCMD_MOVB_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SB;
		end
		16'h30zD: begin
			opUCmd	= BSR_UCMD_MOVW_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SB;
		end
		16'h30zE: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SB;
		end
		16'h30zF: begin
			opUCmd	= BSR_UCMD_MOVUW_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SB;
		end

		16'h31z0: begin
			opUCmd	= BSR_UCMD_ALU_ADD;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z1: begin
			opUCmd	= BSR_UCMD_ALU_SUB;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z2: begin
			opUCmd	= BSR_UCMD_ALU_ADC;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z3: begin
			opUCmd	= BSR_UCMD_ALU_SBB;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z4: begin
			opUCmd	= BSR_UCMD_ALU_TST;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z5: begin
			opUCmd	= BSR_UCMD_ALU_AND;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z6: begin
			opUCmd	= BSR_UCMD_ALU_OR;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z7: begin
			opUCmd	= BSR_UCMD_ALU_XOR;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31z8: begin
			opUCmd	= BSR_UCMD_ALU_CMPGT;
			opFmid	= BSR_FMID_REG;
		end
		16'h31z9: begin
			opUCmd	= BSR_UCMD_ALU_CMPGE;
			opFmid	= BSR_FMID_REG;
		end
		16'h31zA: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SW;
		end
		16'h31zB: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SL;
		end
		16'h31zC: begin
			opUCmd	= BSR_UCMD_ALU_CMPEQ;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31zD: begin
			opUCmd	= BSR_UCMD_ALU_CMPHI;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31zE: begin
			opUCmd	= BSR_UCMD_ALU_CMPGT;
			opFmid	= BSR_FMID_DRREG;
		end
		16'h31zF: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRPCREG;
			opBty	= BSR_BTY_SB;
		end

		16'h32z0: begin
			opUCmd		= BSR_UCMD_CF_JMP;
			opFmid		= BSR_FMID_REG;
		end
		16'h32z1: begin
			opUCmd		= BSR_UCMD_CF_JSR;
			opFmid		= BSR_FMID_REG;
		end
		16'h32z2: begin
			opUCmd		= BSR_UCMD_CF_JT;
			opFmid		= BSR_FMID_REG;
		end
		16'h32z3: begin
			opUCmd		= BSR_UCMD_CF_JF;
			opFmid		= BSR_FMID_REG;
		end


		16'h32z4: begin
			opUCmd		= BSR_UCMD_ALU_EXTUB;
			opFmid		= BSR_FMID_REG;
		end
		16'h32z5: begin
			opUCmd		= BSR_UCMD_ALU_EXTUW;
			opFmid		= BSR_FMID_REG;
		end
		16'h32z6: begin
			opUCmd		= BSR_UCMD_ALU_EXTSB;
			opFmid		= BSR_FMID_REG;
		end
		16'h32z7: begin
			opUCmd		= BSR_UCMD_ALU_EXTSW;
			opFmid		= BSR_FMID_REG;
		end

		16'h32z8: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDRGBR;
			opBty	= BSR_BTY_UL;
		end
		16'h32z9: begin
			opUCmd	= BSR_UCMD_MOVW_RM;
			opFmid	= BSR_FMID_REGSTDRGBR;
			opBty	= BSR_BTY_UW;
		end
		16'h32zA: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_UL;
		end
		16'h32zB: begin
			opUCmd	= BSR_UCMD_MOVW_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_UW;
		end

		16'h32zC: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SB;
		end
		16'h32zD: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SW;
		end
		16'h32zE: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_UB;
		end
		16'h32zF: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_UW;
		end

		16'h33z0: begin
			opUCmd		= BSR_UCMD_ALU_NOT;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z1: begin
			opUCmd		= BSR_UCMD_ALU_NEG;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z2: begin
			opUCmd		= BSR_UCMD_ALU_NEGC;
			opFmid		= BSR_FMID_REG;
		end

		16'h33z4: begin
			opUCmd		= BSR_UCMD_ALU_ROTL;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z5: begin
			opUCmd		= BSR_UCMD_ALU_ROTR;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z6: begin
			opUCmd		= BSR_UCMD_ALU_ROTCL;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z7: begin
			opUCmd		= BSR_UCMD_ALU_ROTCR;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z8: begin
			opUCmd		= BSR_UCMD_ALU_SHLL;
			opFmid		= BSR_FMID_REG;
		end
		16'h33z9: begin
			opUCmd		= BSR_UCMD_ALU_SHLR;
			opFmid		= BSR_FMID_REG;
		end
		16'h33zA: begin
			opUCmd		= BSR_UCMD_ALU_SHAR;
			opFmid		= BSR_FMID_REG;
		end

		16'h34z0: begin
			opUCmd		= BSR_UCMD_ALU_SHLLN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 1;
		end
		16'h34z1: begin
			opUCmd		= BSR_UCMD_ALU_SHLLN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 2;
		end
		16'h34z2: begin
			opUCmd		= BSR_UCMD_ALU_SHLLN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 4;
		end
		16'h34z3: begin
			opUCmd		= BSR_UCMD_ALU_SHLLN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 8;
		end
		16'h34z4: begin
			opUCmd		= BSR_UCMD_ALU_SHLRN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 1;
		end
		16'h34z5: begin
			opUCmd		= BSR_UCMD_ALU_SHLRN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 2;
		end
		16'h34z6: begin
			opUCmd		= BSR_UCMD_ALU_SHLRN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 4;
		end
		16'h34z7: begin
			opUCmd		= BSR_UCMD_ALU_SHLRN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 8;
		end
		16'h34z8: begin
			opUCmd		= BSR_UCMD_ALU_SHARN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 1;
		end
		16'h34z9: begin
			opUCmd		= BSR_UCMD_ALU_SHARN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 2;
		end
		16'h34zA: begin
			opUCmd		= BSR_UCMD_ALU_SHARN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 4;
		end
		16'h34zB: begin
			opUCmd		= BSR_UCMD_ALU_SHARN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 8;
		end

		16'h34zC: begin
			opUCmd		= BSR_UCMD_ALU_SHLLN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 16;
		end
		16'h34zD: begin
			opUCmd		= BSR_UCMD_ALU_SHLRN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 16;
		end
		16'h34zE: begin
			opUCmd		= BSR_UCMD_ALU_SHARN;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 16;
		end

		16'h34zF: begin
			opUCmd		= BSR_UCMD_ALU_SHARSX;
			opFmid		= BSR_FMID_REG;
			opUCmdIx	= 31;
		end


		16'h35z0: begin
			opUCmd	= BSR_UCMD_MOVB_RM;
			opFmid	= BSR_FMID_REGSTDLR;
			opBty	= BSR_BTY_SB;
		end
		16'h35z1: begin
			opUCmd	= BSR_UCMD_MOVW_RM;
			opFmid	= BSR_FMID_REGSTDLR;
			opBty	= BSR_BTY_SW;
		end
		16'h35z2: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDLR;
			opBty	= BSR_BTY_SL;
		end
		16'h35z3: begin
			opUCmd	= BSR_UCMD_MOVUB_MR;
			opFmid	= BSR_FMID_REGSTDLR;
			opBty	= BSR_BTY_SB;
		end

/*
		16'h35z4: begin
			opUCmd	= BSR_UCMD_MOVB_RM;
			opFmid	= BSR_FMID_REGSTDRGBR;
			opBty	= BSR_BTY_SB;
		end
		16'h35z5: begin
			opUCmd	= BSR_UCMD_MOVW_RM;
			opFmid	= BSR_FMID_REGSTDRGBR;
			opBty	= BSR_BTY_SW;
		end
		16'h35z6: begin
			opUCmd	= BSR_UCMD_MOVL_RM;
			opFmid	= BSR_FMID_REGSTDRGBR;
			opBty	= BSR_BTY_SL;
		end
		16'h35z7: begin
			opUCmd	= BSR_UCMD_MOVUB_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SB;
		end
*/

		16'h35z8: begin
			opUCmd	= BSR_UCMD_MOVB_MR;
			opFmid	= BSR_FMID_LDDLRREG;
			opBty	= BSR_BTY_SB;
		end
		16'h35z9: begin
			opUCmd	= BSR_UCMD_MOVW_MR;
			opFmid	= BSR_FMID_LDDLRREG;
			opBty	= BSR_BTY_SW;
		end
		16'h35zA: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDLRREG;
			opBty	= BSR_BTY_SL;
		end
		16'h35zB: begin
			opUCmd	= BSR_UCMD_MOVUW_MR;
			opFmid	= BSR_FMID_LDDLRREG;
			opBty	= BSR_BTY_SW;
		end

/*
		16'h35zC: begin
			opUCmd	= BSR_UCMD_MOVB_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SB;
		end
		16'h35zD: begin
			opUCmd	= BSR_UCMD_MOVW_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SW;
		end
		16'h35zE: begin
			opUCmd	= BSR_UCMD_MOVL_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SL;
		end
		16'h35zF: begin
			opUCmd	= BSR_UCMD_MOVUW_MR;
			opFmid	= BSR_FMID_LDDRGBRREG;
			opBty	= BSR_BTY_SW;
		end
*/

		16'h36zC: begin
			opUCmd	= BSR_UCMD_ALU_ADD;
			opFmid	= BSR_FMID_DRREG;
			opBty	= BSR_BTY_UL;
		end

		16'h3Fzz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDR4PCREG;
			opBty	= BSR_BTY_SB;
		end


		16'h40zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BRA;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_SW;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVB_RM;
				opFmid	= BSR_FMID_REGSTDR4PC;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h41zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BSR;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_SW;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVW_RM;
				opFmid	= BSR_FMID_REGSTDR4PC;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h42zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BRA;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_UL;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVL_RM;
				opFmid	= BSR_FMID_REGSTDR4PC;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h43zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BSR;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_UL;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVUB_MR;
				opFmid	= BSR_FMID_LDDR4PCREG;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h44zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BT;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_SW;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVB_MR;
				opFmid	= BSR_FMID_LDDR4PCREG;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h45zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BF;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_SW;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVW_MR;
				opFmid	= BSR_FMID_LDDR4PCREG;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h46zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BT;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_UL;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVL_MR;
				opFmid	= BSR_FMID_LDDR4PCREG;
				opBty	= BSR_BTY_SB;
			end
		end
		16'h47zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= BSR_UCMD_CF_BF;
				opFmid	= BSR_FMID_DR4PC;
				opBty	= BSR_BTY_UL;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVUW_MR;
				opFmid	= BSR_FMID_LDDR4PCREG;
				opBty	= BSR_BTY_SB;
			end
		end

		16'h48zz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDR4PCREG;
			opBty	= BSR_BTY_UL;
		end
		16'h49zz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDR4PCREG;
			opBty	= BSR_BTY_UQ;
		end

		16'h4Azz: begin
			opUCmd	= BSR_UCMD_MOV_RR;
			opFmid	= BSR_FMID_REGREG;
			opBty	= BSR_BTY_UL;
		end
		16'h4Bzz: begin
			opUCmd	= BSR_UCMD_MOV_RR;
			opFmid	= BSR_FMID_REGREG;
			opBty	= BSR_BTY_UQ;
		end

		16'h4Czz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SB;
		end
		16'h4Dzz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SW;
		end
		16'h4Ezz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SL;
		end
		16'h4Fzz: begin
			opUCmd	= BSR_UCMD_LEA_MR;
			opFmid	= BSR_FMID_LDDRREGREG;
			opBty	= BSR_BTY_SQ;
		end

		16'h58zz: begin
			opUCmd	= BSR_UCMD_ALU_ADD2;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h59zz: begin
			opUCmd	= BSR_UCMD_ALU_SUB2;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h5Azz: begin
			opUCmd	= BSR_UCMD_ALU_AND2;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h5Bzz: begin
			opUCmd	= BSR_UCMD_ALU_OR2;
			opFmid	= BSR_FMID_REGREG;
		end
		16'h5Czz: begin
			opUCmd	= BSR_UCMD_ALU_XOR2;
			opFmid	= BSR_FMID_REGREG;
		end

		16'h8zzz: begin
			if(istrWord[11])
			begin
				opUCmd	= BSR_UCMD_MOVL_RM;
				opFmid	= BSR_FMID_LDREGREG;
				opBty	= BSR_BTY_UL;
			end
			else
			begin
				opUCmd	= BSR_UCMD_MOVL_RM;
				opFmid	= BSR_FMID_REGSTREG;
				opBty	= BSR_BTY_UL;
			end
		end

		16'hAzzz: begin
			opUCmd	= BSR_UCMD_ALU_LDIX;
			opFmid	= BSR_FMID_IMM12Z;
		end
		16'hBzzz: begin
			opUCmd	= BSR_UCMD_ALU_LDIX;
			opFmid	= BSR_FMID_IMM12N;
		end
		16'hCzzz: begin
			opUCmd	= BSR_UCMD_ALU_ADD;
			opFmid	= BSR_FMID_IMM8REG;
		end

		16'hEzzz: begin
			opUCmd	= BSR_UCMD_MOV_IR;
			opFmid	= BSR_FMID_IMM8REG;
		end
	
		default: begin
			opUCmd	= BSR_UCMD_INVOP;
			opFmid	= BSR_FMID_INV;
		end

	endcase

	
	case(opFmid)
		BSR_FMID_INV: begin
			$display("BsrDecOp: Inv %x", istrWord);
		end

		BSR_FMID_Z: begin
//			opRegN=BSR_REG_ZZR;
//			opRegM=BSR_REG_ZZR;
//			opImm=0;
			opUIxt	= opUCmdIx;
		end

		BSR_FMID_REG: begin
			case(opBty)
				BSR_BTY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= BSR_REG_ZZR;
				end
				BSR_BTY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= BSR_REG_ZZR;
				end
				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= BSR_REG_ZZR;
				end
			endcase
			opUIxt	= opUCmdIx;
		end
		BSR_FMID_IMM8: begin
			opImm	= {istrWord[7]?UV24_FF:UV24_00, istrWord[7:0]};
			opRegN	= BSR_REG_DLR;
		end
		BSR_FMID_REGREG: begin
			case(opBty)
			
			BSR_BTY_UB: begin
				opRegN	= opRegN_Cr;
				opRegM	= opRegM_Dfl;
			end
			BSR_BTY_UW: begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Cr;
			end
			BSR_BTY_UL: begin
				opRegN	= opRegN_Sr;
				opRegM	= opRegM_Dfl;
			end
			BSR_BTY_UQ: begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Sr;
			end
			
			default: begin
				opRegN	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
			end
			endcase
		end
		BSR_FMID_IMM8REG: begin
			opImm	= {istrWord[7]?UV24_FF:UV24_00, istrWord[7:0]};
			opRegM	= BSR_REG_IMM;
			opRegN	= opRegO_Dfl;
		end

		BSR_FMID_IMM8Z: begin
			opImm	= {UV24_00, istrWord[7:0]};
			opRegN	= BSR_REG_DLR;
			opRegM	= BSR_REG_IMM;
		end
		BSR_FMID_IMM8N: begin
			opImm	= {UV24_FF, istrWord[7:0]};
			opRegN	= BSR_REG_DLR;
			opRegM	= BSR_REG_IMM;
		end

		BSR_FMID_REGSTREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opBty)
				BSR_BTY_UL: begin
					opUIxt	= {BSR_IXT_RDI, 1'b0, BSR_BTY_SL};
					opImm	= {UV29_00, istrWord[10:8]};
				end
				default:
				begin
					opUIxt	= {BSR_IXT_REG, 1'b0, opBty};
				end
			endcase
		end
		BSR_FMID_LDREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opBty)
				BSR_BTY_UL: begin
					opUIxt	= {BSR_IXT_RDI, 1'b1, BSR_BTY_SL};
					opImm	= {UV29_00, istrWord[10:8]};
				end
				default:
				begin
					opUIxt	= {BSR_IXT_REG, 1'b1, opBty};
				end
			endcase
		end

		BSR_FMID_REGSTDRREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opUIxt	= {BSR_IXT_RDL, 1'b0, opBty};
		end
		BSR_FMID_LDDRREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opUIxt	= {BSR_IXT_RDL, 1'b1, opBty};
		end

		BSR_FMID_REGSTDRPC: begin
			opRegN	= BSR_REG_PC;
			opRegM	= opRegN_Dfl;
			opUIxt	= {BSR_IXT_RDL, 1'b0, opBty};
		end
		BSR_FMID_LDDRPCREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= BSR_REG_PC;
			opUIxt	= {BSR_IXT_RDL, 1'b1, opBty};
		end
		BSR_FMID_IMM4ZREG: begin
			opImm	= {UV28_00, istrWord[3:0]};
			opRegN	= opRegN_Dfl;
			opRegM	= BSR_REG_IMM;
		end
		BSR_FMID_IMM4NREG: begin
			opImm	= {UV28_FF, istrWord[3:0]};
			opRegN	= opRegN_Dfl;
			opRegM	= BSR_REG_IMM;
		end

		BSR_FMID_DR4PC: begin
			opRegN	= BSR_REG_DLR;
			opImm	= {UV28_00, istrWord[3:0]};
			case(opBty)
				BSR_BTY_UL: begin
					opRegM	= BSR_REG_ZZR;
					opUIxt	= {BSR_IXT_RD4, 1'b1, 3'b000};
				end
				default: begin
					opRegM	= BSR_REG_PC;
					opUIxt	= {BSR_IXT_RD4, 1'b1, opBty};
				end
			endcase
		end

		BSR_FMID_DRPC: begin
			opRegN	= BSR_REG_DLR;
			opRegM	= BSR_REG_PC;
			opUIxt	= {BSR_IXT_RDL, 1'b1, opBty};
		end

		BSR_FMID_DRREG: begin
			case(opBty)
				BSR_BTY_UL: begin
					opRegM	= opRegN_Dfl;
					opRegN	= BSR_REG_DLR;
				end
				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= BSR_REG_DLR;
				end
			endcase
		end

		BSR_FMID_PCDISP8: begin
			opRegN	= BSR_REG_DLR;
			opRegM	= BSR_REG_PC;
			opUIxt	= {BSR_IXT_RDI, 1'b1, opBty};
			opImm	= {istrWord[7]?UV24_FF:UV24_00, istrWord[7:0]};
		end

		BSR_FMID_IMM12Z: begin
			opImm	= {UV20_00, istrWord[11:0]};
			opRegN	= BSR_REG_DLR;
			opRegM	= BSR_REG_IMM;
		end
		BSR_FMID_IMM12N: begin
			opImm	= {UV20_FF, istrWord[11:0]};
			opRegN	= BSR_REG_DLR;
			opRegM	= BSR_REG_IMM;
		end

		BSR_FMID_REGSTDLR: begin
			opRegM	= opRegN_Dfl;
			opRegN	= BSR_REG_DLR;
			opUIxt	= {BSR_IXT_REG, 1'b0, opBty};
		end
		
		BSR_FMID_LDDLRREG: begin
			opRegM	= BSR_REG_DLR;
			opRegN	= opRegN_Dfl;
			opUIxt	= {BSR_IXT_REG, 1'b1, opBty};
		end

		BSR_FMID_REGSTDRGBR: begin
			opRegN	= opBty[2] ? BSR_REG_TBR: BSR_REG_GBR;
			opRegM	= opRegN_Dfl;
			opUIxt	= {BSR_IXT_RDL, 1'b0, opBty};
		end
		BSR_FMID_LDDRGBRREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opBty[2] ? BSR_REG_TBR: BSR_REG_GBR;
			opUIxt	= {BSR_IXT_RDL, 1'b1, opBty};
		end

		BSR_FMID_REGSTDR4PC: begin
			opRegN	= BSR_REG_PC;
			opRegM	= opRegN_Dfl;
			opUIxt	= {BSR_IXT_RD4, 1'b0, opBty};
//			opUIxt	= {BSR_IXT_RD4, 1'b0, 3'b000};
			opImm	= {UV28_00, istrWord[3:0]};
		end
		BSR_FMID_LDDR4PCREG: begin
			case(opBty)
				BSR_BTY_UL: begin
					opRegN	= opRegN_Dfl;
					opRegM	= BSR_REG_ZZR;
					opUIxt	= {BSR_IXT_RD4, 1'b1, 3'b000};
					opImm	= {UV28_00, istrWord[3:0]};
				end
				BSR_BTY_UQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opUIxt	= {BSR_IXT_RD4, 1'b1, 3'b000};
					opImm	= {UV28_00, istrWord[3:0]};
				end
				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= BSR_REG_PC;
					opUIxt	= {BSR_IXT_RD4, 1'b1, opBty};
//					opUIxt	= {BSR_IXT_RD4, 1'b1, 3'b000};
					opImm	= {UV28_00, istrWord[3:0]};
				end
			endcase
		end

		BSR_FMID_REGSTDI4SP: begin
			opRegN	= BSR_REG_SP;
			opRegM	= opRegN_Dfl;
			opUIxt	= {BSR_IXT_RDI, 1'b0, opBty};
			opImm	= {UV24_00, 3'b0, opBty[2], istrWord[3:0]};
		end
		BSR_FMID_LDDI4SPREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= BSR_REG_SP;
			opUIxt	= {BSR_IXT_RDI, 1'b1, opBty};
			opImm	= {UV24_00, 3'b0, opBty[2], istrWord[3:0]};
		end
		
		default: begin
		end

	endcase
end

endmodule
