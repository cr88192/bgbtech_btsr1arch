/*
BJX2 Instruction Decoder

Takes a 16/32/48 bit instruction word.

RegReg
  SB: Rm,Rn => Rm,Rn,Rn
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

reg[6:0]	opRegO_Er;
reg[6:0]	opRegN_Er;
reg[6:0]	opRegM_Er;

reg[6:0]	opRegN_ECr;
reg[6:0]	opRegN_ESr;

reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;

reg[7:0]	opUCmdIx;

// reg opIs32;

reg tRegRmIsRz;
reg tRegRnIsRz;
reg tRegRmIsR0;
reg tRegRmIsR1;
reg tRegRnIsR0;
reg tRegRnIsR1;

reg opIsFx;

reg	tMsgLatch;
reg	tNextMsgLatch;

wire[6:0]		decOpFx_idRegN;
wire[6:0]		decOpFx_idRegM;
wire[6:0]		decOpFx_idRegO;
wire[32:0]		decOpFx_idImm;
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

	tRegRnIsRz	= (istrWord[ 7:5]==3'b000);
	tRegRmIsRz	= (istrWord[ 3:1]==3'b000);
	tRegRnIsR0	= tRegRnIsRz && !istrWord[4];
	tRegRnIsR1	= tRegRnIsRz &&  istrWord[4];
	tRegRmIsR0	= tRegRmIsRz && !istrWord[0];
	tRegRmIsR1	= tRegRmIsRz &&  istrWord[0];


	opUCmd		= JX2_UCMD_INVOP;
	opRegN		= JX2_REG_ZZR;
	opRegM		= JX2_REG_ZZR;
	opRegO		= JX2_REG_ZZR;
	opImm		= 0;
	opFmid		= JX2_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;
	opIty		= 0;
	opIsFx		= 0;

	tNextMsgLatch	= 0;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[15:0])

		16'h00zz: begin
			if(istrWord[7:0]==8'h00)
			begin
				opUCmd	= JX2_UCMD_NOP;
				opFmid	= JX2_FMID_Z;
			end
			else
			begin
				opUCmd	= JX2_UCMD_MOVB_RM;
				opFmid	= JX2_FMID_REGSTREG;
				opBty	= JX2_BTY_SB;
			end
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
			opUCmd		= JX2_UCMD_ALU_ADD3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_IXTB_RRI_LDSB;
		end
		16'h11zz: begin
			opUCmd		= JX2_UCMD_ALU_SUB3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end
		16'h12zz: begin
			opUCmd		= JX2_UCMD_ALU_ADC3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_IXTB_RRI_LDSB;
		end
		16'h13zz: begin
			opUCmd		= JX2_UCMD_ALU_SBB3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NB;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end
		16'h14zz: begin
			opUCmd	= JX2_UCMD_ALU_TST;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h15zz: begin
			opUCmd	= JX2_UCMD_ALU_AND3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h16zz: begin
			opUCmd	= JX2_UCMD_ALU_OR3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h17zz: begin
			opUCmd	= JX2_UCMD_ALU_XOR3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end

		16'h18zz: begin		/* MOV Rm, Rn */
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h19zz: begin		/* MOV Rj, Rn */
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SW;
		end
		16'h1Azz: begin		/* MOV Rm, Rk */
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SL;
		end
		16'h1Bzz: begin		/* MOV Rj, Rk */
			opUCmd	= JX2_UCMD_MOV_RR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SQ;
		end

		16'h1Czz: begin
			opUCmd	= JX2_UCMD_ALU_CMPEQ;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h1Dzz: begin
			opUCmd	= JX2_UCMD_ALU_CMPHI;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h1Ezz: begin
			opUCmd	= JX2_UCMD_ALU_CMPGT;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
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
			opImm	= 33'h00000000;
		end
		16'h30B0: begin
			opUCmd	= JX2_UCMD_ALU_LDISH16;
			opFmid	= JX2_FMID_Z;
			opImm	= 33'h0000FFFF;
		end

		16'h30C0: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_Z;
			opUCmdIx	= JX2_UCMD_IX_RTE;
		end


		16'h31z0: begin
			opUCmd		= JX2_UCMD_CF_BRA;
			opFmid		= JX2_FMID_REGPC;
			opBty		= JX2_BTY_SW;
//			opIty		= JX2_ITY_UB;
		end
		16'h31z1: begin
			opUCmd		= JX2_UCMD_CF_BSR;
			opFmid		= JX2_FMID_REGPC;
			opBty		= JX2_BTY_SW;
//			opIty		= JX2_ITY_UB;
		end
		16'h31z2: begin
			opUCmd		= JX2_UCMD_CF_BT;
			opFmid		= JX2_FMID_REGPC;
			opBty		= JX2_BTY_SW;
//			opIty		= JX2_ITY_UB;
		end
		16'h31z3: begin
			opUCmd		= JX2_UCMD_CF_BF;
			opFmid		= JX2_FMID_REGPC;
			opBty		= JX2_BTY_SW;
//			opIty		= JX2_ITY_UB;
		end


		16'h31z8: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_SB;
		end
		16'h31z9: begin
			opUCmd	= JX2_UCMD_MOV_PUSHC;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_NB;
		end
		16'h31zA: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_SB;
		end
		16'h31zB: begin
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

		16'h31zC: begin
			opUCmd	= JX2_UCMD_ALU_CMPGT;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_SB;
		end
		16'h31zD: begin
			opUCmd	= JX2_UCMD_ALU_CMPGE;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_SB;
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
			opIty		= JX2_ITY_SL;
		end
		16'h32z5: begin
			opUCmd		= JX2_UCMD_ALU_EXTUW;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SL;
		end
		16'h32z6: begin
			opUCmd		= JX2_UCMD_ALU_EXTSB;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SL;
		end
		16'h32z7: begin
			opUCmd		= JX2_UCMD_ALU_EXTSW;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SL;
		end

		16'h32z8: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_SW;
		end
		16'h32z9: begin
			opUCmd	= JX2_UCMD_MOV_PUSH;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_UW;
		end
		16'h32zA: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_SW;
		end
		16'h32zB: begin
			opUCmd	= JX2_UCMD_MOV_POP;
			opFmid	= JX2_FMID_REG;
			opIty	= JX2_ITY_UW;
		end


		16'h33z0: begin
			opUCmd		= JX2_UCMD_ALU_NOT;
			opFmid		= JX2_FMID_REG;
		end
		16'h33z1: begin
//			opUCmd		= JX2_UCMD_ALU_NEG;
			opUCmd		= JX2_UCMD_ALU_SUB3;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end
		16'h33z2: begin
//			opUCmd		= JX2_UCMD_ALU_NEGC;
			opUCmd		= JX2_UCMD_ALU_SBB3;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end
		16'h33z3: begin
			opUCmd		= JX2_UCMD_OP_IXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_IXS_MOVNT;
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
			opUCmd		= JX2_UCMD_ALU_ADD3;
			opFmid		= JX2_FMID_DRREG;
			opIty		= JX2_ITY_UL;
			opUCmdIx	= JX2_IXTB_RRI_LDSB;
		end

		16'h33zC: begin
			opUCmd		= JX2_UCMD_ALU_SUB3;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_XB;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end
		16'h33zD: begin
			opUCmd		= JX2_UCMD_ALU_SUB3;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_XW;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end

		16'h34zz: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= { 4'h0, istrWord[3:0] };
		end

/*
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
*/

		16'h36z0: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= { 4'h1, istrWord[3:0] };
		end
		16'h36z1: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= { 4'h1, istrWord[3:0] };
		end
		16'h36z2: begin
			opUCmd		= JX2_UCMD_ALU_SHLLN;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= { 4'h1, istrWord[3:0] };
		end
		16'h36z3: begin
			opUCmd		= JX2_UCMD_OP_IXT;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_IX_TRAPA;
		end
		16'h36z4: begin
			opUCmd		= JX2_UCMD_ALU_EXTUL;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SL;
		end
		16'h36z5: begin
			opUCmd		= JX2_UCMD_ALU_EXTSL;
			opFmid		= JX2_FMID_REG;
			opIty		= JX2_ITY_SL;
		end
		16'h36z6: begin
			opUCmd	= JX2_UCMD_ALU_SHAD3;
			opFmid	= JX2_FMID_DRREG;
			opIty	= JX2_ITY_NB;
		end
		16'h36z7: begin
			opUCmd	= JX2_UCMD_ALU_SHLD3;
			opFmid	= JX2_FMID_DRREG;
			opIty	= JX2_ITY_NB;
		end

		16'h36zD: begin
			opUCmd	= JX2_UCMD_ALU_CMPHS;
			opFmid	= JX2_FMID_DRREG;
//			opIty	= JX2_ITY_SB;
		end
		16'h36zE: begin
			opUCmd	= JX2_UCMD_ALU_CMPGE;
			opFmid	= JX2_FMID_DRREG;
//			opIty	= JX2_ITY_SB;
		end
		16'h36zF: begin
			opUCmd		= JX2_UCMD_OP_IXS;
			opFmid		= JX2_FMID_REG;
			opUCmdIx	= JX2_UCMD_IXS_MOVT;
		end


		16'h38z0: begin
			opUCmd	= JX2_UCMD_NOP;
			opFmid	= JX2_FMID_Z;
		end

		16'h38z6: begin
			opUCmd	= JX2_UCMD_ALU_SHADQ3;
			opFmid	= JX2_FMID_DRREG;
			opIty	= JX2_ITY_NB;
		end
		16'h38z7: begin
			opUCmd	= JX2_UCMD_ALU_SHLDQ3;
			opFmid	= JX2_FMID_DRREG;
			opIty	= JX2_ITY_NB;
		end


		16'h48zz: begin
			opUCmd	= JX2_UCMD_MOV_RC;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UB;
		end
		16'h49zz: begin
			opUCmd	= JX2_UCMD_MOV_CR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UW;
		end
		16'h4Azz: begin
			opUCmd	= JX2_UCMD_MOV_RC;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UL;
		end
		16'h4Bzz: begin
			opUCmd	= JX2_UCMD_MOV_CR;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_UQ;
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
			opIty	= JX2_ITY_SB;
		end
		16'h55zz: begin
			opUCmd	= JX2_UCMD_ALU_CMPQEQ;
			opFmid	= JX2_FMID_REGREG;
		end
		16'h56zz: begin
			opUCmd	= JX2_UCMD_ALU_MULS;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h57zz: begin
			opUCmd	= JX2_UCMD_ALU_MULU;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end

		16'h58zz: begin
			opUCmd		= JX2_UCMD_ALU_ADD3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_IXTB_RRI_LDSB;
		end
		16'h59zz: begin
			opUCmd		= JX2_UCMD_ALU_SUB3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_IXTB_RRI_LDUB;
		end
		16'h5Azz: begin
			opUCmd	= JX2_UCMD_ALU_MUL3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h5Bzz: begin
			opUCmd	= JX2_UCMD_ALU_CMPQHI;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h5Czz: begin
			opUCmd	= JX2_UCMD_ALU_CMPQGT;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end

		16'h5Dzz: begin
			opUCmd	= JX2_UCMD_ALU_AND3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h5Ezz: begin
			opUCmd	= JX2_UCMD_ALU_OR3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
		end
		16'h5Fzz: begin
			opUCmd	= JX2_UCMD_ALU_XOR3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_SB;
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
			opUCmd	= JX2_UCMD_FPU_FADD3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h99zz: begin
			opUCmd	= JX2_UCMD_FPU_FSUB3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h9Azz: begin
			opUCmd	= JX2_UCMD_FPU_FMUL3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h9Bzz: begin
			opUCmd	= JX2_UCMD_FPU_FDIV3;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end

		16'h9Czz: begin
			opUCmd	= JX2_UCMD_FPU_FCMPEQ;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h9Dzz: begin
			opUCmd	= JX2_UCMD_FPU_FCMPGT;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
		end
		16'h9Ezz: begin
			opUCmd	= JX2_UCMD_FPU_FMOV;
			opFmid	= JX2_FMID_REGREG;
			opIty	= JX2_ITY_NB;
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
			opUCmd		= JX2_UCMD_ALU_ADD3;
			opFmid		= JX2_FMID_IMM8REG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_IXTB_RRI_LDSB;
		end

		16'hEzzz: begin
			opUCmd	= JX2_UCMD_MOV_IR;
			opFmid	= JX2_FMID_IMM8REG;
		end

		16'hFzzz: begin
			opIsFx	= 1;
			opUCmd	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_Z;
		end
	
		default: begin
			opUCmd	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_INV;
		end

	endcase

	
	case(opFmid)
		JX2_FMID_INV: begin
//			$display("Jx2DecOp: Inv %x", istrWord);
			$display("Jx2DecOp: Inv %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
		end

		JX2_FMID_Z: begin
			opUIxt	= opUCmdIx;
		end

		JX2_FMID_REG: begin
			opUIxt	= opUCmdIx;
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_ZZR;
					opRegO	= opRegN_Dfl;
				end
				JX2_ITY_SW: begin
					opRegN	= opRegN_Er;
					opRegM	= JX2_REG_ZZR;
					opRegO	= opRegN_Er;
				end

				JX2_ITY_SL: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_REG_ZZR;
				end
				JX2_ITY_SQ: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegN_Er;
					opRegO	= JX2_REG_ZZR;
				end

				JX2_ITY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_ECr;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_ITY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= JX2_REG_ZZR;
				end
				JX2_ITY_UQ: begin
					opRegN	= opRegN_ESr;
					opRegM	= JX2_REG_ZZR;
				end

				JX2_ITY_NB: begin
					opRegN	= JX2_REG_ZZR;
					opRegM	= opRegN_Cr;
				end
				JX2_ITY_NW: begin
					opRegN	= JX2_REG_ZZR;
					opRegM	= opRegN_ECr;
				end
				JX2_ITY_NL: begin
					opRegN	= JX2_REG_ZZR;
					opRegM	= opRegN_Sr;
				end
				JX2_ITY_NQ: begin
					opRegN	= JX2_REG_ZZR;
					opRegM	= opRegN_ESr;
				end

				JX2_ITY_XB: begin
					opRegN	= JX2_REG_DLR;
					opRegM	= JX2_REG_ZZR;
					opRegO	= opRegN_Dfl;
				end
				JX2_ITY_XW: begin
					opRegN	= JX2_REG_DLR;
					opRegM	= JX2_REG_ZZR;
					opRegO	= opRegN_Er;
				end

				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_ZZR;
				end
			endcase
		end
		JX2_FMID_IMM8: begin
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
		end
		
		
		/*
			SB: Rm, Rn / Rm, DLR, Rn
			SW: Rj, Rn / Rj, DLR, Rn
			SL: Rm, Rk / Rm, DLR, Rk
			SQ: Rj, Rk / Rj, DLR, Rk
			
			UB: Rm, Cn
			UW: Cm, Rn
			UL: Rm, Sn
			UQ: Sn, Rn
			
			NB: Rn, Rm, Rn
		 */
		
		JX2_FMID_REGREG: begin
			opUIxt	= opUCmdIx;

			case(opIty)

				JX2_ITY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_REG_DLR;
				end

				JX2_ITY_SW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Er;
					opRegO	= JX2_REG_DLR;
				end
				JX2_ITY_SL: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_REG_DLR;
				end
				JX2_ITY_SQ: begin
					opRegN	= opRegN_Er;
					opRegM	= opRegM_Er;
					opRegO	= JX2_REG_DLR;
				end
				
				JX2_ITY_UB: begin
					opRegN	= opRegN_Cr;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_REG_DLR;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Cr;
					opRegO	= JX2_REG_DLR;
				end
				JX2_ITY_UL: begin
					opRegN	= opRegN_Sr;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_REG_DLR;
				end
				JX2_ITY_UQ: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Sr;
					opRegO	= JX2_REG_DLR;
				end
				
				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
				end

				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_REG_DLR;
				end
			endcase
		end
		JX2_FMID_IMM8REG: begin
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
			opRegM	= JX2_REG_IMM;
			opRegN	= opRegO_Dfl;
			opRegO	= opRegO_Dfl;
			opUIxt	= opUCmdIx;

/*
			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_NB: begin
					opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
					opRegN	= opRegO_Dfl;
					opRegM	= opRegO_Dfl;
					opRegO	= JX2_REG_IMM;
				end

				default: begin
					opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
					opRegM	= JX2_REG_IMM;
					opRegN	= opRegO_Dfl;
				end
			endcase
*/
		end

		JX2_FMID_IMM8Z: begin
			opImm	= {UV25_00, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end
		JX2_FMID_IMM8N: begin
			opImm	= {UV25_FF, istrWord[7:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_REGSTREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opBty)
				JX2_BTY_UL: begin
					opRegO	= JX2_REG_IMM;
//					opUIxt	= {JX2_IXT_RDI, 1'b0, JX2_BTY_SL};
					opUIxt	= {JX2_IXT_RRI, 1'b0, JX2_BTY_SL};
					opImm	= {UV30_00, istrWord[10:8]};
				end
				default:
				begin
					opRegO	= JX2_REG_ZZR;
//					opUIxt	= {JX2_IXT_REG, 1'b0, opBty};
					opUIxt	= {JX2_IXT_RRI, 1'b0, opBty};
				end
			endcase
		end

		JX2_FMID_LDREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			case(opBty)
				JX2_BTY_UL: begin
					opRegO	= JX2_REG_IMM;
//					opUIxt	= {JX2_IXT_RDI, 1'b1, JX2_BTY_SL};
					opUIxt	= {JX2_IXT_RRI, 1'b1, JX2_BTY_SL};
					opImm	= {UV30_00, istrWord[10:8]};
				end
				default:
				begin
					opRegO	= JX2_REG_ZZR;
//					opUIxt	= {JX2_IXT_REG, 1'b1, opBty};
					opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
				end
			endcase
		end

		JX2_FMID_REGSTDRREG: begin
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_REG_DLR;
			if(tRegRnIsRz)
			begin
				opRegN	= tRegRnIsR1 ? JX2_REG_GBR : JX2_REG_PC;
//				opUIxt	= {JX2_IXT_RDL, 1'b0, JX2_BTY_SB};
				opUIxt	= {JX2_IXT_RRI, 1'b0, JX2_BTY_SB};
			end else begin
				opRegN	= opRegN_Dfl;
//				opUIxt	= {JX2_IXT_RDL, 1'b0, opBty};
				opUIxt	= {JX2_IXT_RRI, 1'b0, opBty};
			end
		end

		JX2_FMID_LDDRREGREG: begin
			opRegN	= opRegN_Dfl;
			opRegO	= JX2_REG_DLR;
			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_REG_GBR : JX2_REG_PC;
//				opUIxt	= {JX2_IXT_RDL, 1'b1, JX2_BTY_SB};
				opUIxt	= {JX2_IXT_RRI, 1'b1, JX2_BTY_SB};
			end else begin
				opRegM	= opRegM_Dfl;
//				opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
				opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
			end
			
//			$display("LDDRREGREG: %X %X %X %d",
//				istrWord[15:0], opRegN, opRegM, tRegRmIsRz);
		end

		JX2_FMID_REGSTDRPC: begin
			opRegN	= JX2_REG_PC;
			opRegM	= opRegN_Dfl;
			opRegO	= JX2_REG_DLR;
//			opUIxt	= {JX2_IXT_RDL, 1'b0, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b0, opBty};
		end

		JX2_FMID_LDDRPCREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_PC;
			opRegO	= JX2_REG_DLR;
//			opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
		end

		JX2_FMID_IMM4ZREG: begin
			opImm	= {UV29_00, istrWord[3:0]};
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_IMM4NREG: begin
			opImm	= {UV29_FF, istrWord[3:0]};
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_IMM;
		end

//		JX2_FMID_DRPC: begin
//			opRegN	= JX2_REG_DLR;
//			opRegM	= JX2_REG_PC;
//			opUIxt	= {JX2_IXT_RDL, 1'b1, opBty};
//		end

		JX2_FMID_REGPC: begin
//			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_PC;
			opRegO	= opRegN_Dfl;
			opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
		end

		JX2_FMID_DRREG: begin
			opUIxt	= opUCmdIx;
			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_DLR;
					opRegO	= JX2_REG_DLR;
				end

				JX2_ITY_UL: begin
					opRegM	= opRegN_Cr;
					opRegN	= JX2_REG_DLR;
					opRegO	= JX2_REG_DLR;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_REG_DLR;
				end
				JX2_ITY_NW: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_DLR;
					opRegO	= opRegN_Dfl;
				end

				default: begin
					opRegN	= opRegN_Dfl;
					opRegM	= JX2_REG_DLR;
					opRegO	= JX2_REG_DLR;
				end
			endcase
		end

		JX2_FMID_PCDISP8: begin
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_PC;
//			opRegO	= JX2_REG_DLR;
			opRegO	= JX2_REG_IMM;
//			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
			opImm	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV21_00, istrWord[11:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV21_FF, istrWord[11:0]};
			opRegN	= JX2_REG_DLR;
			opRegM	= JX2_REG_IMM;
		end

		JX2_FMID_REGSTDLR: begin
			opRegM	= opRegN_Dfl;
			opRegN	= JX2_REG_DLR;
			opRegO	= JX2_REG_ZZR;
//			opUIxt	= {JX2_IXT_REG, 1'b0, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b0, opBty};
		end
		
		JX2_FMID_LDDLRREG: begin
			opRegM	= JX2_REG_DLR;
			opRegN	= opRegN_Dfl;
			opRegO	= JX2_REG_ZZR;
//			opUIxt	= {JX2_IXT_REG, 1'b1, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
		end

		JX2_FMID_REGSTDI4SP: begin
			opRegN	= JX2_REG_SP;
			opRegM	= opRegN_Dfl;
			opRegO	= JX2_REG_IMM;
//			opUIxt	= {JX2_IXT_RDI, 1'b0, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b0, opBty};
			opImm	= {UV25_00, 3'b0, opBty[2], istrWord[3:0]};
		end
		JX2_FMID_LDDI4SPREG: begin
			opRegN	= opRegN_Dfl;
			opRegM	= JX2_REG_SP;
			opRegO	= JX2_REG_IMM;
//			opUIxt	= {JX2_IXT_RDI, 1'b1, opBty};
			opUIxt	= {JX2_IXT_RRI, 1'b1, opBty};
			opImm	= {UV25_00, 3'b0, opBty[2], istrWord[3:0]};
		end

		default: begin
			opUCmd = JX2_UCMD_INVOP;
			if(!tMsgLatch)
			begin
				$display("Unhandled FMID (16) %X", opFmid);
				$display("Jx2DecOp: Istr %X",
					istrWord[15:0]);
			end
			tNextMsgLatch=1;
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
	
	if(opUCmd == JX2_UCMD_INVOP)
	begin
		$display("Jx2DecOp: Inv %X-%X-%X",
			istrWord[15:0], istrWord[31:16], istrWord[47:32]);
	end
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule
