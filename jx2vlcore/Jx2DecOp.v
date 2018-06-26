/*
BJX2 Instruction Decoder

Takes a 16/32/48 bit instruction word.
 */

`include "Jx2CoreDefs.v"

`include "Jx2DecOpFx.v"

module Jx2DecOp(
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

reg[6:0]	opRegO_Er;
reg[6:0]	opRegN_Er;
reg[6:0]	opRegM_Er;

reg[6:0]	opRegN_ECr;
reg[6:0]	opRegN_ESr;

reg[4:0]	opFmid;
reg[2:0]	opBty;

reg[7:0]	opUCmdIx;

// reg opIs32;

reg opIsFx;

wire[6:0]		decOpFx_idRegN;
wire[6:0]		decOpFx_idRegM;
wire[6:0]		decOpFx_idRegO;
wire[31:0]		decOpFx_idImm;
wire[7:0]		decOpFx_idUCmd;
wire[7:0]		decOpFx_idUIxt;

Jx2DecOpFx	decOpFx(
	clock,		reset,
	istrWord,
	decOpFx_idRegN,		decOpFx_idRegM,
	decOpFx_idRegO,		decOpFx_idImm,
	decOpFx_idUCmd,		decOpFx_idUIxt
	);

always @*
begin

	opRegO_Dfl	= {3'b000, istrWord[11:8]};
	opRegN_Dfl	= {3'b000, istrWord[ 7:4]};
	opRegM_Dfl	= {3'b000, istrWord[ 3:0]};
	opRegN_Cr	= {3'b010, istrWord[ 7:4]};
	opRegM_Cr	= {3'b010, istrWord[ 3:0]};
	opRegN_Sr	= {3'b100, istrWord[ 7:4]};
	opRegM_Sr	= {3'b100, istrWord[ 3:0]};

	opRegO_Er	= {3'b001, istrWord[11:8]};
	opRegN_Er	= {3'b001, istrWord[ 7:4]};
	opRegM_Er	= {3'b001, istrWord[ 3:0]};

	opRegN_ECr	= {3'b011, istrWord[ 7:4]};
	opRegN_ESr	= {3'b101, istrWord[ 7:4]};

	opUCmd		= JX2_UCMD_INVOP;
	opRegN		= JX2_REG_ZZR;
	opRegM		= JX2_REG_ZZR;
	opRegO		= JX2_REG_ZZR;
	opImm		= 0;
	opFmid		= JX2_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;
	opIsFx		= 0;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[15:0])

		16'h00zz: begin
			opUCmd	= JX2_UCMD_MOVB_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SB;
		end
		16'h01zz: begin
			opUCmd	= JX2_UCMD_MOVW_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SW;
		end
		16'h02zz: begin
			opUCmd	= JX2_UCMD_MOVL_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SL;
		end
		16'h03zz: begin
			opUCmd	= JX2_UCMD_MOVQ_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h04zz: begin
			opUCmd	= JX2_UCMD_MOVB_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h05zz: begin
			opUCmd	= JX2_UCMD_MOVW_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SW;
		end
		16'h06zz: begin
			opUCmd	= JX2_UCMD_MOVL_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SL;
		end
		16'h07zz: begin
			opUCmd	= JX2_UCMD_MOVQ_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h08zz: begin
			opUCmd	= JX2_UCMD_MOVB_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h09zz: begin
			opUCmd	= JX2_UCMD_MOVW_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h0Azz: begin
			opUCmd	= JX2_UCMD_MOVL_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h0Bzz: begin
			opUCmd	= JX2_UCMD_MOVQ_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h0Czz: begin
			opUCmd	= JX2_UCMD_MOVB_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h0Dzz: begin
			opUCmd	= JX2_UCMD_MOVW_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h0Ezz: begin
			opUCmd	= JX2_UCMD_MOVL_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h0Fzz: begin
			opUCmd	= JX2_UCMD_MOVQ_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h10zz: begin
			opUCmd	= JX2_UCMD_ALU_ADD;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h11zz: begin
			opUCmd	= JX2_UCMD_ALU_SUB;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h12zz: begin
			opUCmd	= JX2_UCMD_ALU_ADC;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h13zz: begin
			opUCmd	= JX2_UCMD_ALU_SBB;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h14zz: begin
			opUCmd	= JX2_UCMD_ALU_TST;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h15zz: begin
			opUCmd	= JX2_UCMD_ALU_AND;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h16zz: begin
			opUCmd	= JX2_UCMD_ALU_OR;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h17zz: begin
			opUCmd	= JX2_UCMD_ALU_XOR;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h18zz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h19zz: begin
			opUCmd	= JX2_UCMD_ALU_MULS;
			opFmid	= JX2_FMID_REGREG;
		end

		16'h1Azz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opBty	= JX2_BTY_UB;
		end
		16'h1Bzz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opBty	= JX2_BTY_UW;
		end

		16'h1Czz: begin
			opUCmd	= JX2_UCMD_ALU_CMPEQ;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h1Dzz: begin
			opUCmd	= JX2_UCMD_ALU_CMPHI;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h1Ezz: begin
			opUCmd	= JX2_UCMD_ALU_CMPGT;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h1Fzz: begin
			opUCmd	= JX2_UCMD_ALU_MULU;
			opFmid	= JX2_FMID_REGREG;
		end

		16'h20zz: begin
			opUCmd	= JX2_UCMD_CF_BRA;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
		end
		16'h21zz: begin
			opUCmd	= JX2_UCMD_CF_BSR;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
		end
		16'h22zz: begin
			opUCmd	= JX2_UCMD_CF_BT;
			opFmid	= JX2_FMID_PCDISP8;
			opBty	= JX2_BTY_SW;
		end
		16'h23zz: begin
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

		16'h3000: begin
			opUCmd	= JX2_UCMD_NOP;
			opFmid	= JX2_FMID_Z;
		end
		16'h3010: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_RTS;
		end
		16'h3020: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_SLEEP;
		end
		16'h3030: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_BREAK;
		end
		16'h3040: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_CLRT;
		end
		16'h3050: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_SETT;
		end
		16'h3060: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_CLRS;
		end
		16'h3070: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_SETS;
		end
		16'h3080: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_NOTT;
		end
		16'h3090: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_NOTS;
		end

		16'h30A0: begin
			opUCmd	= JX2_UCMD_ALU_LDISH16;
			opFmid	= JX2_FMID_Z;
			opImm	= 32'h00000000;
		end
		16'h30B0: begin
			opUCmd	= JX2_UCMD_ALU_LDISH16;
			opFmid	= JX2_FMID_Z;
			opImm	= 32'h0000FFFF;
		end

		16'h30C0: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_RTE;
		end

		16'h3002: begin
			opUCmd		= JX2_UCMD_CF_BRA;
			opFmid		= JX2_FMID_DRPC;
			opBty		= JX2_BTY_SW;
		end
		16'h3012: begin
			opUCmd		= JX2_UCMD_CF_BSR;
			opFmid		= JX2_FMID_DRPC;
			opBty		= JX2_BTY_SW;
		end
		16'h3022: begin
			opUCmd		= JX2_UCMD_CF_BT;
			opFmid		= JX2_FMID_DRPC;
			opBty		= JX2_BTY_SW;
		end
		16'h3032: begin
			opUCmd		= JX2_UCMD_CF_BF;
			opFmid		= JX2_FMID_DRPC;
			opBty		= JX2_BTY_SW;
		end


		16'h30z4: begin
			opUCmd	= JX2_UCMD_MOVB_RM;
			opFmid	= JX2_FMID_REGSTDRPC;
			opBty	= JX2_BTY_SB;
		end
		16'h30z5: begin
			opUCmd	= JX2_UCMD_MOVW_RM;
			opFmid	= JX2_FMID_REGSTDRPC;
			opBty	= JX2_BTY_SB;
		end
		16'h30z6: begin
			opUCmd	= JX2_UCMD_MOVL_RM;
			opFmid	= JX2_FMID_REGSTDRPC;
			opBty	= JX2_BTY_SB;
		end
		16'h30z7: begin
			opUCmd	= JX2_UCMD_MOVQ_RM;
			opFmid	= JX2_FMID_REGSTDRPC;
			opBty	= JX2_BTY_SB;
		end

		16'h30z8: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
//			opBty	= JX2_BTY_SL;
		end
		16'h30z9: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
			opBty	= JX2_BTY_UB;
		end
		16'h30zA: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
//			opBty	= JX2_BTY_SL;
		end
		16'h30zB: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
			opBty	= JX2_BTY_UB;
		end

		16'h30zC: begin
			opUCmd	= JX2_UCMD_MOVB_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end
		16'h30zD: begin
			opUCmd	= JX2_UCMD_MOVW_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end
		16'h30zE: begin
			opUCmd	= JX2_UCMD_MOVL_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end
		16'h30zF: begin
			opUCmd	= JX2_UCMD_MOVQ_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end

		16'h31z0: begin
			opUCmd	= JX2_UCMD_ALU_ADD;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z1: begin
			opUCmd	= JX2_UCMD_ALU_SUB;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z2: begin
			opUCmd	= JX2_UCMD_ALU_ADC;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z3: begin
			opUCmd	= JX2_UCMD_ALU_SBB;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z4: begin
			opUCmd	= JX2_UCMD_ALU_TST;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z5: begin
			opUCmd	= JX2_UCMD_ALU_AND;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z6: begin
			opUCmd	= JX2_UCMD_ALU_OR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z7: begin
			opUCmd	= JX2_UCMD_ALU_XOR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31z8: begin
			opUCmd	= JX2_UCMD_ALU_CMPGT;
			opFmid	= JX2_FMID_REG;
		end
		16'h31z9: begin
			opUCmd	= JX2_UCMD_ALU_CMPGE;
			opFmid	= JX2_FMID_REG;
		end
		16'h31zA: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SW;
		end
		16'h31zB: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SL;
		end
		16'h31zC: begin
			opUCmd	= JX2_UCMD_ALU_CMPEQ;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31zD: begin
			opUCmd	= JX2_UCMD_ALU_CMPHI;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31zE: begin
			opUCmd	= JX2_UCMD_ALU_CMPGT;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h31zF: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end

		16'h32z0: begin
			opUCmd		= JX2_UCMD_CF_JMP;
			opFmid		= JX2_FMID_REG;
		end
		16'h32z1: begin
			opUCmd		= JX2_UCMD_CF_JSR;
			opFmid		= JX2_FMID_REG;
		end
		16'h32z2: begin
			opUCmd		= JX2_UCMD_CF_JT;
			opFmid		= JX2_FMID_REG;
		end
		16'h32z3: begin
			opUCmd		= JX2_UCMD_CF_JF;
			opFmid		= JX2_FMID_REG;
		end


		16'h32z4: begin
			opUCmd		= JX2_UCMD_ALU_EXTUB;
			opFmid		= JX2_FMID_REG;
		end
		16'h32z5: begin
			opUCmd		= JX2_UCMD_ALU_EXTUW;
			opFmid		= JX2_FMID_REG;
		end
		16'h32z6: begin
			opUCmd		= JX2_UCMD_ALU_EXTSB;
			opFmid		= JX2_FMID_REG;
		end
		16'h32z7: begin
			opUCmd		= JX2_UCMD_ALU_EXTSW;
			opFmid		= JX2_FMID_REG;
		end

		16'h32z8: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
			opBty	= JX2_BTY_SW;
		end
		16'h32z9: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
			opBty	= JX2_BTY_UW;
		end
		16'h32zA: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
			opBty	= JX2_BTY_SW;
		end
		16'h32zB: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
			opBty	= JX2_BTY_UW;
		end
		16'h32zC: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_UW;
		end
		16'h32zD: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SW;
		end
		16'h32zE: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_UQ;
		end
		16'h32zF: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SQ;
		end


		16'h33z0: begin
			opUCmd		= JX2_UCMD_ALU_NOT;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z1: begin
			opUCmd		= JX2_UCMD_ALU_NEG;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z2: begin
			opUCmd		= JX2_UCMD_ALU_NEGC;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z3: begin
			opUCmd		= JX2_UCMD_OP_IXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_IX_MOVNT;
		end
		16'h33z4: begin
			opUCmd		= JX2_UCMD_ALU_ROTL;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z5: begin
			opUCmd		= JX2_UCMD_ALU_ROTR;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z6: begin
			opUCmd		= JX2_UCMD_ALU_ROTCL;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z7: begin
			opUCmd		= JX2_UCMD_ALU_ROTCR;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z8: begin
			opUCmd		= JX2_UCMD_ALU_SHLL;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z9: begin
			opUCmd		= JX2_UCMD_ALU_SHLR;
			opFmid		= JX2_FMID_REG;
		end
		16'h33zA: begin
			opUCmd		= JX2_UCMD_ALU_SHAR;
			opFmid		= JX2_FMID_REG;
		end
		16'h33zB: begin
			opUCmd	= JX2_UCMD_ALU_ADD;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_UL;
		end

		16'h33zE: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_UL;
		end
		16'h33zF: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SL;
		end

		16'h34z0: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 1;
		end
		16'h34z1: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 2;
		end
		16'h34z2: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 4;
		end
		16'h34z3: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 8;
		end
		16'h34z4: begin
			opUCmd		= JX2_UCMD_ALU_SHLRN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 1;
		end
		16'h34z5: begin
			opUCmd		= JX2_UCMD_ALU_SHLRN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 2;
		end
		16'h34z6: begin
			opUCmd		= JX2_UCMD_ALU_SHLRN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 4;
		end
		16'h34z7: begin
			opUCmd		= JX2_UCMD_ALU_SHLRN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 8;
		end
		16'h34z8: begin
			opUCmd		= JX2_UCMD_ALU_SHARN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 1;
		end
		16'h34z9: begin
			opUCmd		= JX2_UCMD_ALU_SHARN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 2;
		end
		16'h34zA: begin
			opUCmd		= JX2_UCMD_ALU_SHARN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 4;
		end
		16'h34zB: begin
			opUCmd		= JX2_UCMD_ALU_SHARN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 8;
		end

		16'h34zC: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 16;
		end
		16'h34zD: begin
			opUCmd		= JX2_UCMD_ALU_SHLRN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 16;
		end
		16'h34zE: begin
			opUCmd		= JX2_UCMD_ALU_SHARN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 16;
		end
		16'h34zF: begin
			opUCmd		= JX2_UCMD_ALU_SHARSX;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 63;
		end


		16'h35z0: begin
			opUCmd	= JX2_UCMD_MOVB_RM;
			opFmid	= JX2_FMID_REGSTDLR;
			opBty	= JX2_BTY_SB;
		end
		16'h35z1: begin
			opUCmd	= JX2_UCMD_MOVW_RM;
			opFmid	= JX2_FMID_REGSTDLR;
			opBty	= JX2_BTY_SW;
		end
		16'h35z2: begin
			opUCmd	= JX2_UCMD_MOVL_RM;
			opFmid	= JX2_FMID_REGSTDLR;
			opBty	= JX2_BTY_SL;
		end
		16'h35z3: begin
			opUCmd	= JX2_UCMD_MOVQ_RM;
			opFmid	= JX2_FMID_REGSTDLR;
			opBty	= JX2_BTY_SQ;
		end
		16'h35z4: begin
			opUCmd	= JX2_UCMD_MOVUB_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end
		16'h35z5: begin
			opUCmd	= JX2_UCMD_MOVUW_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end
		16'h35z6: begin
			opUCmd	= JX2_UCMD_MOVUL_MR;
			opFmid	= JX2_FMID_LDDRPCREG;
			opBty	= JX2_BTY_SB;
		end

		16'h35z8: begin
			opUCmd	= JX2_UCMD_MOVB_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h35z9: begin
			opUCmd	= JX2_UCMD_MOVW_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SW;
		end
		16'h35zA: begin
			opUCmd	= JX2_UCMD_MOVL_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SL;
		end
		16'h35zB: begin
			opUCmd	= JX2_UCMD_MOVQ_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SQ;
		end
		16'h35zC: begin
			opUCmd	= JX2_UCMD_MOVUB_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h35zD: begin
			opUCmd	= JX2_UCMD_MOVUW_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SW;
		end
		16'h35zE: begin
			opUCmd	= JX2_UCMD_MOVUL_MR;
			opFmid	= JX2_FMID_LDDLRREG;
			opBty	= JX2_BTY_SL;
		end

		16'h36z0: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 32;
		end
		16'h36z1: begin
			opUCmd		= JX2_UCMD_ALU_SHLRN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 32;
		end
		16'h36z2: begin
			opUCmd		= JX2_UCMD_ALU_SHARN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= 32;
		end
		16'h36z3: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_IX_TRAPA;
		end
		16'h36z4: begin
			opUCmd		= JX2_UCMD_ALU_EXTUL;
			opFmid		= JX2_FMID_REG;
		end
		16'h36z5: begin
			opUCmd		= JX2_UCMD_ALU_EXTSL;
			opFmid		= JX2_FMID_REG;
		end
		16'h36z6: begin
			opUCmd	= JX2_UCMD_ALU_SHAD;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end
		16'h36z7: begin
			opUCmd	= JX2_UCMD_ALU_SHLD;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_SB;
		end

		16'h36zC: begin
			opUCmd	= JX2_UCMD_ALU_ADD;
			opFmid	= JX2_FMID_DRREG;
			opBty	= JX2_BTY_UB;
		end

		16'h38z0: begin
			opUCmd	= JX2_UCMD_NOP;
			opFmid	= JX2_FMID_Z;
		end

		16'h3Fzz: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDR4PCREG;
			opBty	= JX2_BTY_SB;
		end

		16'h40zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_SW;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVB_RM;
				opFmid	= JX2_FMID_REGSTDR4PC;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h41zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BSR;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_SW;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVW_RM;
				opFmid	= JX2_FMID_REGSTDR4PC;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h42zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BRA;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_UL;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVL_RM;
				opFmid	= JX2_FMID_REGSTDR4PC;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h43zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BSR;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_UL;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVQ_RM;
				opFmid	= JX2_FMID_REGSTDR4PC;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h44zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BT;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_SW;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVB_MR;
				opFmid	= JX2_FMID_LDDR4PCREG;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h45zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BF;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_SW;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVW_MR;
				opFmid	= JX2_FMID_LDDR4PCREG;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h46zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BT;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_UL;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVL_MR;
				opFmid	= JX2_FMID_LDDR4PCREG;
				opBty	= JX2_BTY_SB;
			end
		end
		16'h47zz: begin
			if(istrWord[7:4]==4'b1111)
			begin
				opUCmd	= JX2_UCMD_CF_BF;
				opFmid	= JX2_FMID_DR4PC;
				opBty	= JX2_BTY_UL;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVQ_MR;
				opFmid	= JX2_FMID_LDDR4PCREG;
				opBty	= JX2_BTY_SB;
			end
		end

		16'h48zz: begin
//			opUCmd	= JX2_UCMD_LEA_MR;
			opUCmd	= JX2_UCMD_MOV_DLR4R;
			opFmid	= JX2_FMID_LDDR4PCREG;
			opBty	= JX2_BTY_UL;
		end
		16'h49zz: begin
//			opUCmd	= JX2_UCMD_LEA_MR;
			opUCmd	= JX2_UCMD_ADD_DLR4R;
			opFmid	= JX2_FMID_LDDR4PCREG;
			opBty	= JX2_BTY_UQ;
		end

		16'h4Azz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opBty	= JX2_BTY_UL;
		end
		16'h4Bzz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opBty	= JX2_BTY_UQ;
		end

		16'h4Czz: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h4Dzz: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h4Ezz: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h4Fzz: begin
			opUCmd	= JX2_UCMD_LEA_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h50zz: begin
			opUCmd	= JX2_UCMD_MOVUB_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h51zz: begin
			opUCmd	= JX2_UCMD_MOVUW_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h52zz: begin
			opUCmd	= JX2_UCMD_MOVUB_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SB;
		end
		16'h53zz: begin
			opUCmd	= JX2_UCMD_MOVUW_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h54zz: begin
			opUCmd	= JX2_UCMD_ALU_TSTQ;
			opFmid	= JX2_FMID_REGREG;
		end

		16'h56zz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h57zz: begin
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opBty	= JX2_BTY_SW;
		end
		16'h58zz: begin
			opUCmd	= JX2_UCMD_ALU_ADD2;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h59zz: begin
			opUCmd	= JX2_UCMD_ALU_SUB2;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h5Azz: begin
			opUCmd	= JX2_UCMD_ALU_AND2;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h5Bzz: begin
			opUCmd	= JX2_UCMD_ALU_OR2;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h5Czz: begin
			opUCmd	= JX2_UCMD_ALU_XOR2;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h5Dzz: begin
			opUCmd	= JX2_UCMD_ALU_CMPQHI;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h5Ezz: begin
			opUCmd	= JX2_UCMD_ALU_CMPQGT;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h5Fzz: begin
			opUCmd	= JX2_UCMD_ALU_CMPQEQ;
			opFmid	= JX2_FMID_REGREG;
		end

		16'h8zzz: begin
			if(istrWord[10:8]!=0)
			begin
				if(istrWord[11])
				begin
					opUCmd	= JX2_UCMD_MOVL_RM;
					opFmid	= JX2_FMID_LDREGREG;
					opBty	= JX2_BTY_UL;
				end
				else
				begin
					opUCmd	= JX2_UCMD_MOVL_RM;
					opFmid	= JX2_FMID_REGSTREG;
					opBty	= JX2_BTY_UL;
				end
			end
			else
			begin
				if(istrWord[11])
				begin
					opUCmd	= JX2_UCMD_MOVUL_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_SL;
				end
				else
				begin
					opUCmd	= JX2_UCMD_MOVUL_MR;
					opFmid	= JX2_FMID_LDREGREG;
					opBty	= JX2_BTY_SL;
				end
			end
		end


		16'h90zz: begin
			opUCmd	= JX2_UCMD_FMOVS_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SL;
		end
		16'h91zz: begin
			opUCmd	= JX2_UCMD_FMOVD_RM;
			opFmid	= JX2_FMID_REGSTREG;
			opBty	= JX2_BTY_SQ;
		end
		16'h92zz: begin
			opUCmd	= JX2_UCMD_FMOVS_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SL;
		end
		16'h93zz: begin
			opUCmd	= JX2_UCMD_FMOVD_RM;
			opFmid	= JX2_FMID_REGSTDRREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h94zz: begin
			opUCmd	= JX2_UCMD_FMOVS_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h95zz: begin
			opUCmd	= JX2_UCMD_FMOVD_MR;
			opFmid	= JX2_FMID_LDREGREG;
			opBty	= JX2_BTY_SQ;
		end
		16'h96zz: begin
			opUCmd	= JX2_UCMD_FMOVS_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SL;
		end
		16'h97zz: begin
			opUCmd	= JX2_UCMD_FMOVD_MR;
			opFmid	= JX2_FMID_LDDRREGREG;
			opBty	= JX2_BTY_SQ;
		end

		16'h98zz: begin
			opUCmd	= JX2_UCMD_FPU_FADD;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h99zz: begin
			opUCmd	= JX2_UCMD_FPU_FSUB;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h9Azz: begin
			opUCmd	= JX2_UCMD_FPU_FMUL;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h9Bzz: begin
			opUCmd	= JX2_UCMD_FPU_FDIV;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h9Czz: begin
			opUCmd	= JX2_UCMD_FPU_FCMPEQ;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h9Dzz: begin
			opUCmd	= JX2_UCMD_FPU_FCMPGT;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h9Ezz: begin
			opUCmd	= JX2_UCMD_FPU_FMOV;
			opFmid	= JX2_FMID_REGREG;
		end

		16'h9Fz0: begin
			opUCmd	= JX2_UCMD_FPU_FLDCF;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz1: begin
			opUCmd	= JX2_UCMD_FPU_FLDCD;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz2: begin
			opUCmd	= JX2_UCMD_FPU_FLDCI;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz3: begin
			opUCmd	= JX2_UCMD_FPU_FLDCH;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz4: begin
			opUCmd	= JX2_UCMD_FPU_FSTCF;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz5: begin
			opUCmd	= JX2_UCMD_FPU_FSTCD;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz6: begin
			opUCmd	= JX2_UCMD_FPU_FSTCI;
			opFmid	= JX2_FMID_REG;
		end
		16'h9Fz7: begin
			opUCmd	= JX2_UCMD_FPU_FSTCH;
			opFmid	= JX2_FMID_REG;
		end

		16'h9Fz8: begin
			opUCmd		= JX2_UCMD_FPU_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_FPIX_FNEG;
		end
		16'h9Fz9: begin
			opUCmd		= JX2_UCMD_FPU_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_FPIX_FABS;
		end
		16'h9FzA: begin
			opUCmd		= JX2_UCMD_FPU_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_FPIX_FRCPA;
		end
		16'h9FzB: begin
			opUCmd		= JX2_UCMD_FPU_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_FPIX_FSQRTA;
		end
		16'h9FzC: begin
			opUCmd		= JX2_UCMD_FPU_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_FPIX_FRCP;
		end
		16'h9FzD: begin
			opUCmd		= JX2_UCMD_FPU_FIXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_FPIX_FSQRT;
		end

		16'hAzzz: begin
			opUCmd	= JX2_UCMD_ALU_LDIX;
			opFmid	= JX2_FMID_IMM12Z;
		end
		16'hBzzz: begin
			opUCmd	= JX2_UCMD_ALU_LDIX;
			opFmid	= JX2_FMID_IMM12N;
		end
		16'hCzzz: begin
			opUCmd	= JX2_UCMD_ALU_ADD;
			opFmid	= JX2_FMID_IMM8REG;
		end

		16'hEzzz: begin
			opUCmd	= JX2_UCMD_MOV_IR;
			opFmid	= JX2_FMID_IMM8REG;
		end

		16'hFzzz: begin
			opIsFx	= 1;
		end
	
		default: begin
			opUCmd	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_INV;
		end

	endcase

	
	case(opFmid)
		JX2_FMID_INV: begin
			$display("Jx2DecOp: Inv %x", istrWord);
		end

		JX2_FMID_Z: begin
//			opRegN=JX2_REG_ZZR;
//			opRegM=JX2_REG_ZZR;
//			opImm=0;
			opUIxt	= opUCmdIx;
		end

		JX2_FMID_REG: begin
			case(opBty)
				JX2_BTY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_BTY_SW: begin
					opRegN	= opRegN_Er;
					opRegM	= JX2_REG_ZZR;
				end

				JX2_BTY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_BTY_UW: begin
					opRegN	= opRegN_ECr;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_BTY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_BTY_UQ: begin
					opRegN	= opRegN_ESr;
					opRegM	= JX2_REG_ZZR;
				end
				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_ZZR;
				end
			endcase
			opUIxt	= opUCmdIx;
		end
		JX2_FMID_IMM8: begin
			opImm	= {istrWord[7]?UV24_FF:UV24_00, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
		end
		JX2_FMID_REGREG: begin
			case(opBty)

				JX2_BTY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
				end
				JX2_BTY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Er;
				end
				JX2_BTY_SL: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegM_Dfl;
				end
				JX2_BTY_SQ: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegM_Er;
				end
				
				JX2_BTY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= opRegM_Dfl;
				end
				JX2_BTY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Cr;
				end
				JX2_BTY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= opRegM_Dfl;
				end
				JX2_BTY_UQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Sr;
				end
				
				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
				end
			endcase
		end
		JX2_FMID_IMM8REG: begin
			opImm	= {istrWord[7]?UV24_FF:UV24_00, istrWord[7:0]};
			opRegM	= JX2_REG_IMM;
			opRegN	= opRegO_Dfl;
		end

		JX2_FMID_IMM8Z: begin
			opImm	= {UV24_00, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end
		JX2_FMID_IMM8N: begin
			opImm	= {UV24_FF, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_REGSTREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opBty)
				JX2_BTY_UL: begin
					opUIxt	= {JX2_IXT_RDI, 1'b0, JX2_BTY_SL};
					opImm	= {UV29_00, istrWord[10:8]};
				end
				default:
				begin
					opUIxt	= {JX2_IXT_REG, 1'b0, opBty};
				end
			endcase
		end
		JX2_FMID_LDREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opBty)
				JX2_BTY_UL: begin
					opUIxt	= {JX2_IXT_RDI, 1'b1, JX2_BTY_SL};
					opImm	= {UV29_00, istrWord[10:8]};
				end
				default:
				begin
					opUIxt	= {JX2_IXT_REG, 1'b1, opBty};
				end
			endcase
		end

		JX2_FMID_REGSTDRREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opUIxt	= {JX2_IXT_RDL, 1'b0, opBty};
		end
		JX2_FMID_LDDRREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
		end

		JX2_FMID_REGSTDRPC: begin
			opRegN	= JX2_REG_PC;
			opRegM	= opRegN_Dfl;
			opUIxt	= {JX2_IXT_RDL, 1'b0, opBty};
		end
		JX2_FMID_LDDRPCREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_PC;
			opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
		end
		JX2_FMID_IMM4ZREG: begin
			opImm	= {UV28_00, istrWord[3:0]};
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_IMM;
		end
		JX2_FMID_IMM4NREG: begin
			opImm	= {UV28_FF, istrWord[3:0]};
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_DR4PC: begin
			opRegN	= JX2_REG_DLR;
			opImm	= {UV28_00, istrWord[3:0]};
			case(opBty)
				JX2_BTY_UL: begin
					opRegM	= JX2_REG_ZZR;
					opUIxt	= {JX2_IXT_RD4, 1'b1, 3'b000};
				end
				default: begin
					opRegM	= JX2_REG_PC;
					opUIxt	= {JX2_IXT_RD4, 1'b1, opBty};
				end
			endcase
		end

		JX2_FMID_DRPC: begin
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_PC;
			opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
		end

		JX2_FMID_DRREG: begin
			case(opBty)
				JX2_BTY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_DLR;
				end
				JX2_BTY_SW: begin
					opRegN	= opRegN_Er;
					opRegM	= JX2_REG_DLR;
				end
				JX2_BTY_SL: begin
					opRegN	= opRegN_Cr;
					opRegM	= JX2_REG_DLR;
				end
				JX2_BTY_SQ: begin
					opRegN	= opRegN_ECr;
					opRegM	= JX2_REG_DLR;
				end


				JX2_BTY_UB: begin
					opRegM	= opRegN_Dfl;
					opRegN	= JX2_REG_DLR;
				end
				JX2_BTY_UW: begin
					opRegM	= opRegN_Er;
					opRegN	= JX2_REG_DLR;
				end
				JX2_BTY_UL: begin
					opRegM	= opRegN_Cr;
					opRegN	= JX2_REG_DLR;
				end
				JX2_BTY_UQ: begin
					opRegM	= opRegN_ECr;
					opRegN	= JX2_REG_DLR;
				end

				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_DLR;
				end
			endcase
		end

		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_PC;
			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
			opImm	= {istrWord[7]?UV24_FF:UV24_00, istrWord[7:0]};
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV20_00, istrWord[11:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV20_FF, istrWord[11:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_REGSTDLR: begin
			opRegM	= opRegN_Dfl;
			opRegN	= JX2_REG_DLR;
			opUIxt	= {JX2_IXT_REG, 1'b0, opBty};
		end
		
		JX2_FMID_LDDLRREG: begin
			opRegM	= JX2_REG_DLR;
			opRegN	= opRegN_Dfl;
			opUIxt	= {JX2_IXT_REG, 1'b1, opBty};
		end

/*
		JX2_FMID_REGSTDRGBR: begin
			opRegN	= opBty[2] ? JX2_REG_TBR: JX2_REG_GBR;
			opRegM	= opRegN_Dfl;
			opUIxt	= {JX2_IXT_RDL, 1'b0, opBty};
		end
		JX2_FMID_LDDRGBRREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opBty[2] ? JX2_REG_TBR: JX2_REG_GBR;
			opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
		end
*/

		JX2_FMID_REGSTDR4PC: begin
			opRegN	= JX2_REG_PC;
			opRegM	= opRegN_Dfl;
			opUIxt	= {JX2_IXT_RD4, 1'b0, opBty};
//			opUIxt	= {JX2_IXT_RD4, 1'b0, 3'b000};
			opImm	= {UV28_00, istrWord[3:0]};
		end
		JX2_FMID_LDDR4PCREG: begin
			case(opBty)
				JX2_BTY_UL: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_ZZR;
					opUIxt	= {JX2_IXT_RD4, 1'b1, 3'b000};
					opImm	= {UV28_00, istrWord[3:0]};
				end
				JX2_BTY_UQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opUIxt	= {JX2_IXT_RD4, 1'b1, 3'b000};
					opImm	= {UV28_00, istrWord[3:0]};
				end
				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_PC;
					opUIxt	= {JX2_IXT_RD4, 1'b1, opBty};
//					opUIxt	= {JX2_IXT_RD4, 1'b1, 3'b000};
					opImm	= {UV28_00, istrWord[3:0]};
				end
			endcase
		end

		JX2_FMID_REGSTDI4SP: begin
			opRegN	= JX2_REG_SP;
			opRegM	= opRegN_Dfl;
			opUIxt	= {JX2_IXT_RDI, 1'b0, opBty};
			opImm	= {UV24_00, 3'b0, opBty[2], istrWord[3:0]};
		end
		JX2_FMID_LDDI4SPREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_SP;
			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
			opImm	= {UV24_00, 3'b0, opBty[2], istrWord[3:0]};
		end
		
		default: begin
		end

	endcase
	
	if(opIsFx)
	begin
		opRegN	= decOpFx_idRegN;
		opRegM	= decOpFx_idRegM;
		opRegO	= decOpFx_idRegO;
		opImm	= decOpFx_idImm;
		opUCmd	= decOpFx_idUCmd;
		opUIxt	= decOpFx_idUIxt;
	end
end

endmodule
