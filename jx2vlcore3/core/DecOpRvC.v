/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
RISC-V C Instruction Decoder

Decodes 16-bit RVC Instruction.
 */

`include "CoreDefs.v"

module DecOpRvC(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	srMod,
	idRegN,		idRegM,		idRegO,
	idImm,		idUCmd,
	idUIxt
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word
// input[2:0]		srMod;		//mode
input[11:0]		srMod;		//mode

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
output[32:0]	idImm;
output[8:0]		idUCmd;
output[8:0]		idUIxt;


wire			srUser;				//Usermode
wire			srSuperuser;		//Superuser mode
assign		srUser = srMod[0];
assign		srSuperuser = (srMod[0] && srMod[1]) || (srMod[0] && srMod[2]);

wire			srDbgRVC;
wire			srDbgRVCF;

assign		srDbgRVC = 0;
// assign		srDbgRVC = 1;

// assign		srDbgRVCF = 0;
assign		srDbgRVCF = 0;

wire			srRV;		//RV Mode
// assign		srRV = !srMod[3] && srMod[4];
assign		srRV = (!srMod[3] && srMod[4] && !srMod[5]) || srDbgRVCF;
// assign		srRV = 1;

`reg_gpr		opRegN;
`reg_gpr		opRegM;
`reg_gpr		opRegO;
reg[32:0]		opImm;
reg[8:0]		opUCmd;
reg[8:0]		opUIxt;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;

`reg_gpr	opRegO_Dfl;
`reg_gpr	opRegN_Dfl;
`reg_gpr	opRegM_Dfl;

`reg_gpr	opRegN_Fpr;
`reg_gpr	opRegM_Fpr;

`reg_gpr	opRegN_Cr;
`reg_gpr	opRegM_Cr;
`reg_gpr	opRegN_Sr;
`reg_gpr	opRegM_Sr;

`reg_gpr	opRegO_Er;		//X8..X15
`reg_gpr	opRegN_Er;		//X8..X15
`reg_gpr	opRegM_Er;		//X8..X15

`reg_gpr	opRegN_Fr;		//F8..F15
`reg_gpr	opRegM_Fr;		//F8..F15

`reg_gpr	opRegN_ECr;		//C16..C31
`reg_gpr	opRegN_ESr;		//S16..S31

`reg_gpr	opRegN_Xr;		//3znz 1R, R0..R31
`reg_gpr	opRegN_Yr;		//2znz/6znz 1R, R0..R31
`reg_gpr	opRegN_ZXr;		//zznz 1R, R0..R31 (xxxy -> yxxx0)

/*
Fixed registers for certain ops.
Ro:Rm, may also be used as an immed.
 */
`reg_gpr	opRegO_Fix;
`reg_gpr	opRegN_Fix;
`reg_gpr	opRegM_Fix;

reg[5:0]	opNmid;
reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[2:0]	opCcty;
reg[2:0]	opUCty;

reg[5:0]	opUCmdIx;

reg tRegRmIsRz;
reg tRegRnIsRz;
reg tRegRoIsRz;
reg tRegRmIsR0;
reg tRegRmIsR1;
reg tRegRnIsR0;
reg tRegRnIsR1;

reg tRegRmIsRs;
reg tRegRnIsRs;
reg tRegRoIsRs;

reg opIsFx;

reg	tMsgLatch;
reg	tNextMsgLatch;

reg[15:0]	tMsgLastIstr;
reg[15:0]	tNextMsgLastIstr;

reg	tMsgDbgExclInst;

wire[15:0]	usrRejectCmMask;
wire[15:0]	usrRejectCnMask;
assign	usrRejectCmMask = 16'b1111_1111_0011_1000;
assign	usrRejectCnMask = 16'b1111_1111_1011_1100;

reg		usrRejectCmR;
reg		usrRejectCmW;
reg		usrRejectCnR;
reg		usrRejectCnW;
reg		usrReject;
reg[1:0]	usrSuAllow;
reg			usrSuAllowEn;

reg[11:0]	tOffsJmp;
reg[9:0]	tOffsJmpZ;

reg[11:0]	tImmOfs6_lwLd;
reg[11:0]	tImmOfs6_qwLd;
reg[11:0]	tImmOfs6_lwSt;
reg[11:0]	tImmOfs6_qwSt;

reg[11:0]	tImmOfs5_lw;
reg[11:0]	tImmOfs5_qw;

reg[11:0]	tImmImm6_Alu;
reg[11:0]	tImmImm6_AluZ;
reg[11:0]	tImmImm6_A16Sp;
reg[11:0]	tImmImm8_i4add;
reg[11:0]	tImmImm8_bcc;
reg[11:0]	tImmImm8_i4spn;
reg[11:0]	tImmSel;

reg[3:0]	tSelImm;

always @*
begin

//	tRegRoIsRs	= (istrWord[12]==1'b0);
	tRegRmIsRs	= (istrWord[ 6]==1'b0);
	tRegRnIsRs	= (istrWord[11]==1'b0);

	opRegM_Dfl	= { 2'b00, istrWord[ 6:2]};
	opRegN_Dfl	= { 2'b00, istrWord[11:7]};

	case(istrWord[5:2])
		4'b0000: opRegM_Sr = JX2_GR_ZZR;
		4'b0001: opRegM_Sr = JX2_GR_LR;
		4'b0010: opRegM_Sr = JX2_GR_SP_RV;
		4'b0011: opRegM_Sr = JX2_GR_GBR;

//		4'b0100: opRegM_Sr = JX2_GR_TBR;
//		4'b0101: opRegM_Sr = JX2_GR_DHR;
		4'b0100: opRegM_Sr = JX2_GR_R4;
		4'b0101: opRegM_Sr = JX2_GR_R5;

		4'b0110: opRegM_Sr = JX2_GR_DLR;
		4'b0111: opRegM_Sr = JX2_GR_R7;
		4'b1000: opRegM_Sr = JX2_GR_R8;
		4'b1001: opRegM_Sr = JX2_GR_R9;
		4'b1010: opRegM_Sr = JX2_GR_R10;
		4'b1011: opRegM_Sr = JX2_GR_R11;
		4'b1100: opRegM_Sr = JX2_GR_R12;
		4'b1101: opRegM_Sr = JX2_GR_R13;
		4'b1110: opRegM_Sr = JX2_GR_R2;
		4'b1111: opRegM_Sr = JX2_GR_R3;
	endcase

	case(istrWord[10:7])
		4'b0000: opRegN_Sr = JX2_GR_ZZR;
		4'b0001: opRegN_Sr = JX2_GR_LR;
		4'b0010: opRegN_Sr = JX2_GR_SP_RV;
		4'b0011: opRegN_Sr = JX2_GR_GBR;

//		4'b0100: opRegN_Sr = JX2_GR_TBR;
//		4'b0101: opRegN_Sr = JX2_GR_DHR;
		4'b0100: opRegN_Sr = JX2_GR_R4;
		4'b0101: opRegN_Sr = JX2_GR_R5;

		4'b0110: opRegN_Sr = JX2_GR_DLR;
		4'b0111: opRegN_Sr = JX2_GR_R7;
		4'b1000: opRegN_Sr = JX2_GR_R8;
		4'b1001: opRegN_Sr = JX2_GR_R9;
		4'b1010: opRegN_Sr = JX2_GR_R10;
		4'b1011: opRegN_Sr = JX2_GR_R11;
		4'b1100: opRegN_Sr = JX2_GR_R12;
		4'b1101: opRegN_Sr = JX2_GR_R13;
		4'b1110: opRegN_Sr = JX2_GR_R2;
		4'b1111: opRegN_Sr = JX2_GR_R3;
	endcase

	if(tRegRmIsRs)
		opRegM_Dfl = opRegM_Sr;
	if(tRegRnIsRs)
		opRegN_Dfl = opRegN_Sr;

	case(istrWord[4:2])
		3'b000: opRegM_Er = JX2_GR_R8;
		3'b001: opRegM_Er = JX2_GR_R9;
		3'b010: opRegM_Er = JX2_GR_R10;
		3'b011: opRegM_Er = JX2_GR_R11;
		3'b100: opRegM_Er = JX2_GR_R12;
		3'b101: opRegM_Er = JX2_GR_R13;
		3'b110: opRegM_Er = JX2_GR_R2;
		3'b111: opRegM_Er = JX2_GR_R3;
	endcase

	case(istrWord[9:7])
		3'b000: opRegN_Er = JX2_GR_R8;
		3'b001: opRegN_Er = JX2_GR_R9;
		3'b010: opRegN_Er = JX2_GR_R10;
		3'b011: opRegN_Er = JX2_GR_R11;
		3'b100: opRegN_Er = JX2_GR_R12;
		3'b101: opRegN_Er = JX2_GR_R13;
		3'b110: opRegN_Er = JX2_GR_R2;
		3'b111: opRegN_Er = JX2_GR_R3;
	endcase
	
//	opRegM_Fr = JX2_GR_R32;
//	opRegN_Fr = JX2_GR_R32;
//	opRegM_Fr[3:0] = opRegM_Er[3:0];
//	opRegN_Fr[3:0] = opRegN_Er[3:0];

	opRegM_Fr	= { 4'b0101, istrWord[ 4:2]};
	opRegN_Fr	= { 4'b0101, istrWord[ 9:7]};

	opRegM_Fpr	= { 2'b01, istrWord[ 6:2]};
	opRegN_Fpr	= { 2'b01, istrWord[11:7]};

	tOffsJmp = {
		istrWord[12],	istrWord[ 8], 
		istrWord[10],	istrWord[ 9], 
		istrWord[ 6],	istrWord[ 7],
		istrWord[ 2],	istrWord[11],
		istrWord[ 5],	istrWord[ 4],
		istrWord[ 3],	1'b0 };

`ifndef def_true
	tOffsJmpZ = {
		istrWord[12],
		istrWord[ 6],	istrWord[ 5],
		istrWord[ 2],	istrWord[11],
		istrWord[10],	istrWord[ 4],
		istrWord[ 3],	1'b0 };
`endif

	tImmOfs6_lwLd = {4'b0000,
		istrWord[3:2], istrWord[12], istrWord[6:4], 2'b00};
	tImmOfs6_qwLd = {3'b000,
		istrWord[4], istrWord[3:2], istrWord[12], istrWord[6:5], 3'b000};
	tImmOfs6_lwSt = {4'b0000,
		istrWord[8:7], istrWord[12:9], 2'b00};
	tImmOfs6_qwSt = {3'b000,
		istrWord[9:7], istrWord[12:10], 3'b000};

	tImmOfs5_lw = { 5'b00000, istrWord[5], istrWord[12:10], istrWord[6], 2'b00};
	tImmOfs5_qw = { 4'b0000, istrWord[6:5], istrWord[12:10], 3'b000};

	tImmImm6_Alu = { istrWord[12]?7'h7F:7'h00, istrWord[6:2] };
	tImmImm6_AluZ = { 6'h00, istrWord[12], istrWord[6:2] };

	tImmImm6_A16Sp = { istrWord[12] ? 3'b111:3'b000,
		istrWord[4:3], istrWord[5], istrWord[2], istrWord[6], 4'h0 };
	tImmImm8_i4add = { 2'h0,
		istrWord[10:7], istrWord[12:11], istrWord[5], istrWord[6], 2'h0 };
	tImmImm8_bcc = { istrWord[12] ? 4'hF : 4'h0,
		istrWord[6:5], istrWord[2], istrWord[11:10], istrWord[4:3], 1'b0 };

	tImmImm8_i4spn = {
		2'b00,
		istrWord[10], istrWord[ 9],
		istrWord[ 8], istrWord[ 7], 
		istrWord[12], istrWord[11],
		istrWord[ 5], istrWord[ 6], 2'b00
		};

	tSelImm		= 0;
	tImmSel		= 0;

	usrReject	= 0;
	usrSuAllow	= 0;

	opRegN_Fix	= JX2_GR_ZZR;
	opRegM_Fix	= JX2_GR_ZZR;
	opRegO_Fix	= JX2_GR_ZZR;

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
	opIsFx		= 0;
	opCcty		= JX2_IXC_AL;
	opUCty		= JX2_IUC_SC;

	tNextMsgLatch	= 0;
	tNextMsgLastIstr	= tMsgLastIstr;

//	opIs32 = (istrWord[15:8] == 8'b1z001110);

	casez(istrWord[15:0])

		16'b000_zzz_zzz_zz_zzz_00: begin	/* ADD SP, Imm8, Rn5 */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_ALU_ADD;
			opIty		= JX2_ITY_UL;
			
			if(istrWord[12:5]==0)
			begin
				opNmid		= JX2_UCMD_OP_IXT;
				opFmid		= JX2_FMID_Z;
				opUCmdIx	= JX2_UCIX_IXT_BREAK;
			end
		end
		16'b001_zzz_zzz_zz_zzz_00: begin	/* FLD */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_SW;
		end
		16'b010_zzz_zzz_zz_zzz_00: begin	/* LW */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= JX2_BTY_SL;
			opIty		= JX2_ITY_SB;
		end
		16'b011_zzz_zzz_zz_zzz_00: begin	/* LD */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_SB;
		end

		16'b100_zzz_zzz_zz_zzz_00: begin
			opNmid	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_INV;
		end

		16'b101_zzz_zzz_zz_zzz_00: begin	/* FSD */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_UW;
		end
		16'b110_zzz_zzz_zz_zzz_00: begin	/* SW */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= JX2_BTY_SL;
			opIty		= JX2_ITY_UB;
		end
		16'b111_zzz_zzz_zz_zzz_00: begin	/* SD */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_UB;
		end


		16'b000_zzz_zzz_zz_zzz_01: begin	/* ADDI Imm6, Rn5 */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_ALU_ADD;
			opIty		= JX2_ITY_SB;
		end
		16'b001_zzz_zzz_zz_zzz_01: begin	/* ADDIW Imm6, Rn5 */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_ALU_ADDSL;
			opIty		= JX2_ITY_SB;
		end
		16'b010_zzz_zzz_zz_zzz_01: begin	/* LI Imm6, Rn5 */
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
			opIty		= JX2_ITY_SB;
		end
		16'b011_zzz_zzz_zz_zzz_01: begin	/* LUI Imm6, Rn5 */
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
			opIty		= JX2_ITY_SW;
			
			if(istrWord[11:7]==5'h02)	/* ADDI16SP */
			begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_IMM8REG;
				opUCmdIx	= JX2_UCIX_ALU_ADD;
				opIty		= JX2_ITY_UB;
			end
		end

		16'b100_z00_zzz_zz_zzz_01: begin	/* SRL */
			opNmid		= JX2_UCMD_SHAD3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_SHAD_SHLRQ3;
			opIty		= JX2_ITY_NW;
		end

		16'b100_z01_zzz_zz_zzz_01: begin	/* SRA */
			opNmid		= JX2_UCMD_SHAD3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_SHAD_SHARQ3;
			opIty		= JX2_ITY_NW;
		end

		16'b100_z10_zzz_zz_zzz_01: begin	/* ANDI */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_ALU_AND;
			opIty		= JX2_ITY_UW;
		end

		16'b100_011_zzz_00_zzz_01: begin	/* SUB */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_SUB;
			opIty		= JX2_ITY_UB;
		end
		16'b100_011_zzz_01_zzz_01: begin	/* XOR */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_XOR;
			opIty		= JX2_ITY_UB;
		end
		16'b100_011_zzz_10_zzz_01: begin	/* OR */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_OR;
			opIty		= JX2_ITY_UB;
		end
		16'b100_011_zzz_11_zzz_01: begin	/* AND */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_AND;
			opIty		= JX2_ITY_UB;
		end

		16'b100_111_zzz_00_zzz_01: begin	/* SUBW */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_SUBSL;
			opIty		= JX2_ITY_UB;
		end
		16'b100_111_zzz_01_zzz_01: begin	/* ADDW */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_ADDSL;
			opIty		= JX2_ITY_UB;
		end

		16'b101_zzz_zzz_zz_zzz_01: begin	/* J Disp11 */
			opNmid		= JX2_UCMD_BRA;
			opFmid		= JX2_FMID_PCDISP8;
			opIty		= JX2_ITY_SB;
		end


		16'b110_zzz_zzz_zz_zzz_01: begin	/* BEQ */
			opNmid		= JX2_UCMD_JCMP;
			opFmid		= JX2_FMID_REGPC;
			opUCmdIx	= JX2_UCIX_JCMP_QEQ;
			opIty		= JX2_ITY_UW;
		end

		16'b111_zzz_zzz_zz_zzz_01: begin	/* BNE */
			opNmid		= JX2_UCMD_JCMP;
			opFmid		= JX2_FMID_REGPC;
			opUCmdIx	= JX2_UCIX_JCMP_QNE;
			opIty		= JX2_ITY_UW;
		end


		16'b000_zzz_zzz_zz_zzz_10: begin	/* SLLI */
			opNmid		= JX2_UCMD_SHAD3;
			opFmid		= JX2_FMID_IMM8REG;
			opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
			opIty		= JX2_ITY_NB;
		end

		16'b001_zzz_zzz_zz_zzz_10: begin	/* FLD */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDDI4SPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_SW;
		end
		16'b010_zzz_zzz_zz_zzz_10: begin	/* LW */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDDI4SPREG;
			opBty		= JX2_BTY_SL;
			opIty		= JX2_ITY_SB;
		end
		16'b011_zzz_zzz_zz_zzz_10: begin	/* LD */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDDI4SPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_SB;
		end

		16'b100_0zz_zzz_zz_zzz_10: begin	/* MV */
//			opNmid		= JX2_UCMD_MOV_IR;
//			opFmid		= JX2_FMID_REGREG;
//			opUCmdIx	= JX2_UCIX_LDI_LDIX;
//			opIty		= JX2_ITY_SB;

			opNmid		= JX2_UCMD_CONV_RR;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SW;
			opUCmdIx	= JX2_UCIX_CONV_MOV;
`ifdef jx2_gprs_mergecm
			opNmid		= JX2_UCMD_MOV_RR;
`endif

			if(istrWord[6:2]==5'h00)
			begin
				opNmid		= JX2_UCMD_JMP;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SB;

				if(istrWord[11:7]==5'h00)
				begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
//					opUCmdIx	= JX2_UCIX_IXT_SYSE;
					opUCmdIx	= JX2_UCIX_IXT_BREAK;
				end
			end
		end

		16'b100_1zz_zzz_zz_zzz_10: begin	/* ADD */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opUCmdIx	= JX2_UCIX_ALU_ADD;
			opIty		= JX2_ITY_SB;

			if(istrWord[6:2]==5'h00)
			begin
				opNmid		= JX2_UCMD_JSR;
				opFmid		= JX2_FMID_REG;
				opIty		= JX2_ITY_SB;

				if(istrWord[11:7]==5'h00)
				begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
//					opUCmdIx	= JX2_UCIX_IXT_SYSE;
					opUCmdIx	= JX2_UCIX_IXT_BREAK;
				end
			end
		end

		16'b101_zzz_zzz_zz_zzz_10: begin	/* FSD */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_LDDI4SPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_UW;
		end
		16'b110_zzz_zzz_zz_zzz_10: begin	/* SW */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_LDDI4SPREG;
			opBty		= JX2_BTY_SL;
			opIty		= JX2_ITY_UB;
		end
		16'b111_zzz_zzz_zz_zzz_10: begin	/* SD */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_LDDI4SPREG;
			opBty		= JX2_BTY_SQ;
			opIty		= JX2_ITY_UB;
		end

		16'bzzz_zzz_zzz_zz_zzz_11: begin
			opNmid	= JX2_UCMD_NOP;
			opFmid	= JX2_FMID_Z;
		end

		default: begin
			opNmid	= JX2_UCMD_INVOP;
			opFmid	= JX2_FMID_INV;
		end

	endcase

	if(!srRV)
	begin
//		opNmid	= JX2_UCMD_NOP;
//		opFmid	= JX2_FMID_Z;
	end

	opUCmd = { opCcty, opNmid };
	
	case(opFmid)
		JX2_FMID_INV: begin
			if(srRV)
			begin
				$display("DecOpRvC: Inv %X-%X-%X",
					istrWord[15:0], istrWord[31:16], istrWord[47:32]);
			end
		end

		JX2_FMID_Z: begin
			opUIxt	= {opUCty, opUCmdIx[5:0]};
			opRegN	= opRegN_Fix;
			opRegM	= opRegM_Fix;
			opRegO	= opRegO_Fix;
		end		

		JX2_FMID_REG: begin
			opRegM	= opRegN_Dfl;
			opRegO	= JX2_GR_ZZR;
			opRegN	= JX2_GR_LR;
			opUIxt	= {opUCty, opUCmdIx[5:0]};

			case(opIty)
				JX2_ITY_SB: begin
				end

				default: begin
				end
			endcase
		end

		/*
			SB: Rn, Rm, Rn
			SW: Rm, Rn, Rn

			UB: Rn3, Rm3, Rn3
		 */
		
		JX2_FMID_REGREG: begin
			opRegM	= opRegN_Dfl;
			opRegO	= opRegM_Dfl;
			opRegN	= opRegN_Dfl;
			opUIxt	= {opUCty, opUCmdIx[5:0]};

			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_SW: begin
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
				end

				JX2_ITY_UB: begin
					opRegM	= opRegN_Er;
					opRegO	= opRegM_Er;
					opRegN	= opRegN_Er;
				end

				default: begin
				end
			endcase
		end

		JX2_FMID_REGIMMREG: begin
			opRegM	= opRegM_Er;
			opRegO	= JX2_GR_IMM;
			opRegN	= opRegN_Er;

			opUIxt	= {opUCty, opUCmdIx[5:0]};
			tSelImm = JX2_RVCIMM_IMM6ALU;

			case(opIty)

				JX2_ITY_SB: begin
				end

				default: begin
				end
			endcase
		end

		JX2_FMID_LDREGDISPREG: begin
			opRegM	= opRegN_Er;
			opRegO	= JX2_GR_IMM;
			opRegN	= opRegM_Er;

			opUIxt	= {opUCty, opBty[1:0], 1'b0, 3'b000};

			tSelImm = JX2_RVCIMM_IMM4LW;
			if(opBty[1:0]==2'b11)
				tSelImm = JX2_RVCIMM_IMM4QW;

			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= opRegN_Er;
					opRegN	= opRegM_Er;
				end

				JX2_ITY_SW: begin
//					opRegM	= opRegM_Er;
//					opRegN	= opRegN_Fr;

					opRegM	= opRegN_Er;
					opRegN	= opRegM_Fr;
				end

				JX2_ITY_UB: begin
//					opRegM	= opRegN_Er;
//					opRegN	= opRegM_Er;
				end

				JX2_ITY_UW: begin
//					opRegM	= opRegN_Er;
//					opRegN	= opRegM_Fr;

					opRegN	= opRegM_Fr;
				end

				default: begin
				end
			endcase
		end

		JX2_FMID_LDDI4SPREG: begin
			opRegM	= JX2_GR_SP_RV;
			opRegO	= JX2_GR_IMM;
			opRegN	= opRegN_Dfl;

			opUIxt	= {opUCty, opBty[1:0], 1'b0, 3'b000};

			tSelImm = JX2_RVCIMM_IMM6LWLD;
			if(opBty[1:0]==2'b11)
				tSelImm = JX2_RVCIMM_IMM6QWLD;
			
			if((opIty==JX2_ITY_UB) || (opIty==JX2_ITY_UW))
			begin
				tSelImm = JX2_RVCIMM_IMM6LWST;
				if(opBty[1:0]==2'b11)
					tSelImm = JX2_RVCIMM_IMM6QWST;
			end

			case(opIty)
				JX2_ITY_SB: begin
					opRegN	= opRegN_Dfl;
				end
				JX2_ITY_SW: begin
					opRegN	= opRegN_Fpr;
				end

				JX2_ITY_UB: begin
					opRegN	= opRegM_Dfl;
				end
				JX2_ITY_UW: begin
					opRegN	= opRegM_Fpr;
				end

				default: begin
					opRegN	= opRegN_Dfl;
				end
			endcase
		end

		JX2_FMID_IMM8REG: begin
			opRegM	= opRegN_Dfl;
			opRegO	= JX2_GR_IMM;
			opRegN	= opRegN_Dfl;
			opUIxt	= {opUCty, opUCmdIx[5:0]};
			tSelImm = JX2_RVCIMM_IMM6ALU;

			case(opIty)
				JX2_ITY_SB: begin
				end
				
				JX2_ITY_SW: begin
					tSelImm = JX2_RVCIMM_IMM6LUI;
				end

				JX2_ITY_UB: begin
					tSelImm = JX2_RVCIMM_IMM6A16SP;
				end

				JX2_ITY_UW: begin
					opRegM	= opRegN_Er;
					opRegN	= opRegN_Er;
				end

				JX2_ITY_UL: begin
					opRegM	= JX2_GR_SP_RV;
					opRegN	= opRegM_Er;
					tSelImm = JX2_RVCIMM_IMM8SPN;
				end

				JX2_ITY_NB: begin
					tSelImm = JX2_RVCIMM_IMM6ALUZ;
				end

				JX2_ITY_NW: begin
					opRegM	= opRegN_Er;
					opRegN	= opRegN_Er;
					tSelImm = JX2_RVCIMM_IMM6ALUZ;
				end

				default: begin
				end
			endcase
		end

		JX2_FMID_REGPC: begin
			opRegM	= JX2_GR_BPC;
			opRegO	= opRegN_Dfl;
			opRegN	= JX2_GR_ZZR;
			opUIxt	= {opUCty, 2'b01, 1'b1, 3'b001};
			tSelImm = JX2_RVCIMM_IMM8JCC;
			
			if(opIty==JX2_ITY_UW)
			begin
				opRegM	= opRegN_Er;
				opRegO	= JX2_GR_ZZR;
				opRegN	= JX2_GR_ZZR;
			end
		end

		JX2_FMID_PCDISP8: begin
			opRegM	= JX2_GR_BPC;
			opRegO	= JX2_GR_IMM;
			opRegN	= JX2_GR_ZZR;
			opUIxt	= {opUCty, 2'b01, 1'b1, 3'b001};
			tSelImm = JX2_RVCIMM_IMM11J;
		end

		default: begin
//			opUCmd = {opCcty, JX2_UCMD_INVOP};
			if(!tMsgLatch)
			begin
				$display("Unhandled FMID (16) %X", opFmid);
				$display("DecOpRvC: Istr %X",
					istrWord[15:0]);
			end
			tNextMsgLatch=1;
		end

	endcase
	
	case(tSelImm)
		JX2_RVCIMM_NONE:		tImmSel = 0;
		JX2_RVCIMM_IMM6ALU:		tImmSel = tImmImm6_Alu;
		JX2_RVCIMM_IMM4LW:		tImmSel = tImmOfs5_lw;
		JX2_RVCIMM_IMM4QW:		tImmSel = tImmOfs5_qw;
		JX2_RVCIMM_IMM6LWLD:	tImmSel = tImmOfs6_lwLd;
		JX2_RVCIMM_IMM6QWLD:	tImmSel = tImmOfs6_qwLd;
		JX2_RVCIMM_IMM6LWST:	tImmSel = tImmOfs6_lwSt;
		JX2_RVCIMM_IMM6QWST:	tImmSel = tImmOfs6_qwSt;
		JX2_RVCIMM_IMM6A16SP:	tImmSel = tImmImm6_A16Sp;
		JX2_RVCIMM_IMM6LUI:		tImmSel = tImmImm6_Alu;
		JX2_RVCIMM_IMM11J:		tImmSel = { tOffsJmp[11], tOffsJmp[11:1] };
		JX2_RVCIMM_IMM8JCC:		tImmSel = {
			tImmImm8_bcc[11], tImmImm8_bcc[11:1] };
		JX2_RVCIMM_IMM6ALUZ:	tImmSel = tImmImm6_AluZ;
		JX2_RVCIMM_IMM8SPN:		tImmSel = tImmImm8_i4spn;

		default:			tImmSel = 0;
	endcase
	
	opImm = { tImmSel[11] ? UV21_FF : UV21_00, tImmSel };

	if(tSelImm == JX2_RVCIMM_IMM6LUI)
		opImm[23:0] = { tImmImm6_Alu, 12'h000 };

	usrSuAllowEn = 0;
	case(usrSuAllow)
		2'b00: usrSuAllowEn = 0;
		2'b01: usrSuAllowEn = srSuperuser;
		2'b10: usrSuAllowEn = srMod[1];
		2'b11: usrSuAllowEn = srMod[1] && srMod[2];
	endcase

	if(usrReject && srUser && !usrSuAllowEn)
	begin
		$display("DecOpRvC: Usermode Reject %X", istrWord[15:0]);
		opNmid	= JX2_UCMD_INVOP;
		opFmid	= JX2_FMID_INV;
	end

//	if(istrWord[1:0]!=2'b11)
	if(srRV && (istrWord[1:0]!=2'b11) && srDbgRVC)
//	if(1'b0)
	begin
		tMsgDbgExclInst = 0;

`ifndef def_true
		case(istrWord[15:12])
			4'hF, 4'hC, 4'h3:
				tMsgDbgExclInst = 1;
			default:
				tMsgDbgExclInst = 0;
		endcase
		
		if(opNmid == JX2_UCMD_MOV_RM)
			tMsgDbgExclInst = 1;
		if(opNmid == JX2_UCMD_MOV_MR)
			tMsgDbgExclInst = 1;
		if(opNmid == JX2_UCMD_MOV_IR)
			tMsgDbgExclInst = 1;
		if(opNmid == JX2_UCMD_ALU3)
			tMsgDbgExclInst = 1;
`endif

		tNextMsgLastIstr = istrWord[15:0];
		if((istrWord[15:0] != tMsgLastIstr) && !tMsgDbgExclInst)
		begin
			$display("DecOpRVC %X  %X-%X rm=%X ro=%X rn=%X imm=%X",
				istrWord[15:0],
				opUCmd, opUIxt,
				opRegM, opRegO, opRegN,
				opImm);
		end
	end

end

always @(posedge clock)
begin
	tMsgLatch		<= tNextMsgLatch;
	tMsgLastIstr	<= tNextMsgLastIstr;
end

endmodule
