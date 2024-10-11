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
RISC-V Jumbo96 Only
 */

`include "CoreDefs.v"

module DecOpRvJO(
	/* verilator lint_off UNUSED */
	clock,		reset,		srMod,
	istrWord,	isAltOpB,	istrJBits,
	idRegN,		idRegM,
	idRegO,		idRegP,
	idImm,		idUCmd,
	idUIxt,		idUFl
	);

input			clock;		//clock
input			reset;		//reset
// input[2:0]		srMod;		//mode
input[7:0]		srMod;		//mode

input[63:0]		istrWord;	//source instruction word
input[3:0]		isAltOpB;
input[27:0]		istrJBits;

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
`output_gpr		idRegP;
output[32:0]	idImm;
output[8:0]		idUCmd;
output[8:0]		idUIxt;
// output[3:0]		idUFl;
// output[7:0]		idUFl;
output[18:0]		idUFl;

wire			isAltOp;
wire			isOp24;
wire			isOpRiscV;
wire			isOpWxe;
assign		isAltOp		= isAltOpB[0];
assign		isOp24		= isAltOpB[1];
assign		isOpWxe		= isAltOpB[2];
// assign		isOpRiscV	= isAltOpB[3];

wire			srUser;				//Usermode
wire			srSuperuser;		//Superuser mode
assign		srUser = srMod[0];
assign		srSuperuser = (srMod[0] && srMod[1]) || (srMod[0] && srMod[2]);

wire			srXG2;		//XG2 Mode
assign		srXG2 = srMod[3];

wire			srXGRV;		//XGRV Mode
assign		srXGRV = srMod[3] && srMod[4];

wire			srRV;		//RV Mode
assign		srRV = !srMod[3] && srMod[4];

assign		isOpRiscV	= srRV;


`reg_gpr		opRegN;
`reg_gpr		opRegM;
`reg_gpr		opRegO;
`reg_gpr		opRegP;
`reg_gpr		opRegImm16;
`reg_gpr		opRegImm10;
reg[32:0]		opImm;
reg[8:0]		opUCmd;
reg[8:0]		opUIxt;
reg[3:0]		opUFl;
reg[3:0]		opULdOp;

reg[3:0]		amoLdOp;

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idRegP = opRegP;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;
// assign	idUFl = opUFl;
assign	idUFl = { 6'h0, 5'h0, opULdOp, opUFl };

`reg_gpr	opRegM_Dfl;
`reg_gpr	opRegO_Dfl;
`reg_gpr	opRegN_Dfl;
`reg_gpr	opRegP_Dfl;

`reg_gpr	opRegM_Fix;
`reg_gpr	opRegO_Fix;
`reg_gpr	opRegN_Fix;

`reg_gpr	opRegM_Fr;
`reg_gpr	opRegO_Fr;
`reg_gpr	opRegN_Fr;
`reg_gpr	opRegP_Fr;

`reg_gpr	opRegO_Df2;

`reg_gpr	opRegO_Df3R;

`reg_gpr	opRegM_Cr;
`reg_gpr	opRegN_Cr;
`reg_gpr	opRegO_Cr;

`reg_gpr	opRegM_Sr;
`reg_gpr	opRegN_Sr;
`reg_gpr	opRegO_Sr;

reg[32:0]		opImm_imm12s;
reg[32:0]		opImm_imm12u;
reg[32:0]		opImm_imm12n;

reg[32:0]		opImm_imm10s;
reg[32:0]		opImm_imm10u;
reg[32:0]		opImm_imm10n;

reg[32:0]		opImm_imm16s;
reg[32:0]		opImm_imm16u;
reg[32:0]		opImm_imm16n;

reg[32:0]		opImm_disp20s;
reg[32:0]		opImm_disp8s;
reg[32:0]		opImm_disp12ld;
reg[32:0]		opImm_disp12st;
reg[32:0]		opImm_disp13s;

reg[32:0]		opImm_disp5u;

reg[32:0]		opImm_imm5u;
reg[32:0]		opImm_imm5n;

reg[32:0]		opImm_imm8au;

reg[5:0]	opNmid;
reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[2:0]	opCcty;

reg[5:0]	opUCmdIx;
reg[2:0]	opUCty;

reg		opExQ;
reg		opExN;
reg		opExM;
reg		opExI;
reg		opIsNotFx;
reg		opIsJumbo;
reg		opIsJumboAu;
reg		opIsJumbo96;
reg		opIsImm9;

reg		opExWN;
reg		opExWM;
reg		opExWI;
reg		opIsXGpr;
reg		tOpIsXGprX0;
reg		tOpIsXGprX1;
reg		tOpIsXGprX2;

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

reg tRegRmIsZr;
reg tRegRnIsZr;
reg tRegCsrIsCpuid;

reg	tMsgLatch;
reg	tNextMsgLatch;

always @*
begin

	tRegRoIsRs	= (istrWord[24]==1'b0);
	tRegRmIsRs	= (istrWord[19]==1'b0);
	tRegRnIsRs	= (istrWord[11]==1'b0);

	tRegRmIsZr	= (istrWord[19:15] == 5'h00);
	tRegRnIsZr	= (istrWord[11: 7] == 5'h00);

	opIsJumbo	= istrJBits[24];
	opIsJumbo96	= istrJBits[25];
	opIsJumboAu	= istrJBits[26] && opIsJumbo;

	opRegO_Dfl	= { 2'b00, istrWord[24:20] };
	opRegM_Dfl	= { 2'b00, istrWord[19:15] };
	opRegN_Dfl	= { 2'b00, istrWord[11: 7] };
//	opRegP_Dfl	= opRegN_Dfl;

	opRegO_Sr = opRegO_Dfl;
	opRegM_Sr = opRegM_Dfl;
	opRegN_Sr = opRegN_Dfl;

	opRegM_Fr	= { 2'b01, istrWord[19:15] };
	opRegO_Fr	= { 2'b01, istrWord[24:20] };
	opRegN_Fr	= { 2'b01, istrWord[11: 7] };
	opRegP_Fr	= { 2'b01, istrWord[31:27] };

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
		opRegM_Dfl[5] = istrJBits[19];
		opRegO_Dfl[5] = istrJBits[20];
		opRegN_Dfl[5] = istrJBits[18];

		tRegRmIsRs	= (istrWord[19]==1'b0) && !istrJBits[19];
		tRegRoIsRs	= (istrWord[24]==1'b0) && !istrJBits[20];
		tRegRnIsRs	= (istrWord[11]==1'b0) && !istrJBits[18];
	end
`endif

	case(istrWord[23:20])
		4'b0000: opRegO_Sr = JX2_GR_ZZR;
		4'b0001: opRegO_Sr = JX2_GR_LR;
		4'b0010: opRegO_Sr = JX2_GR_SP;
		4'b0011: opRegO_Sr = JX2_GR_GBR;
		4'b0100: opRegO_Sr = JX2_GR_R4;
		4'b0101: opRegO_Sr = JX2_GR_R5;
		4'b0110: opRegO_Sr = JX2_GR_R6;
		4'b0111: opRegO_Sr = JX2_GR_R7;
		4'b1000: opRegO_Sr = JX2_GR_R8;
		4'b1001: opRegO_Sr = JX2_GR_R9;
		4'b1010: opRegO_Sr = JX2_GR_R10;
		4'b1011: opRegO_Sr = JX2_GR_R11;
		4'b1100: opRegO_Sr = JX2_GR_R12;
		4'b1101: opRegO_Sr = JX2_GR_R13;
		4'b1110: opRegO_Sr = JX2_GR_R2;
		4'b1111: opRegO_Sr = JX2_GR_R3;
	endcase

	case(istrWord[18:15])
		4'b0000: opRegM_Sr = JX2_GR_ZZR;
		4'b0001: opRegM_Sr = JX2_GR_LR;
		4'b0010: opRegM_Sr = JX2_GR_SP;
		4'b0011: opRegM_Sr = JX2_GR_GBR;
		4'b0100: opRegM_Sr = JX2_GR_R4;
		4'b0101: opRegM_Sr = JX2_GR_R5;
		4'b0110: opRegM_Sr = JX2_GR_R6;
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
		4'b0010: opRegN_Sr = JX2_GR_SP;
		4'b0011: opRegN_Sr = JX2_GR_GBR;
		4'b0100: opRegN_Sr = JX2_GR_R4;
		4'b0101: opRegN_Sr = JX2_GR_R5;
		4'b0110: opRegN_Sr = JX2_GR_R6;
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

	if(tRegRoIsRs)
		opRegO_Dfl = opRegO_Sr;
	if(tRegRmIsRs)
		opRegM_Dfl = opRegM_Sr;
	if(tRegRnIsRs)
		opRegN_Dfl = opRegN_Sr;

	opRegP_Dfl	= opRegN_Dfl;

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
		opRegM_Fr = opRegM_Dfl;
		opRegO_Fr = opRegO_Dfl;
		opRegN_Fr = opRegN_Dfl;
		opRegP_Fr[5]	= istrJBits[12];
	end
`endif


	opRegM_Cr	= 0;
	opRegO_Cr	= 0;
	opRegN_Cr	= 0;

	opRegO_Df2	= opRegN_Dfl;

	opRegO_Df3R	= opRegO_Dfl;

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
		if(istrJBits[11])
			opRegO_Df3R	= JX2_GR_IMM;
	end
`endif
	

	opImm_imm5u	= {UV27_00, opRegO_Dfl[5:0]};
	opImm_imm5n	= {UV27_FF, opRegO_Dfl[5:0]};

	opImm_disp5u	= opImm_imm5u;
	opImm_imm8au	= opImm_imm5u;

	tNextMsgLatch	= 0;

	opRegImm16 = JX2_GR_IMM;
	opRegImm10 = JX2_GR_IMM;

	opImm_disp20s = {
		istrWord[31] ? UV14_FF : UV14_00,
		istrWord[19:12],
		istrWord[20],
		istrWord[30:21] };

	opImm_imm12u	= {UV21_00, istrWord[31:20]};
	opImm_imm12n	= {UV21_FF, istrWord[31:20]};
	
	opImm_disp13s   = 0;
	opImm_disp12ld	= 0;
	opImm_disp12st	= 0;

	opImm_imm16u	= { 1'b0, istrWord[31:12], 12'h000 };
	opImm_imm16n	= { 1'b1, istrWord[31:12], 12'h000 };
//	opImm_imm16s	= opImm_imm16u;

	opImm_imm12s	= istrWord[31] ? opImm_imm12n : opImm_imm12u;
	opImm_imm16s	= istrWord[31] ? opImm_imm16n : opImm_imm16u;

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
//		opImm_imm12u[21:11]		= istrJBits[10:0];
//		opImm_imm12n[21:11]		= istrJBits[10:0];
		opImm_imm12s[21:11]		= istrJBits[10:0];
		
//		opImm_disp12ld[21:11]	= istrJBits[10:0];
//		opImm_disp12st[21:11]	= istrJBits[10:0];
		opImm_disp8s  [21:11]	= istrJBits[10:0];

		opImm_imm8au[15:5]		= istrJBits[10:0];
	end
	else
		if(opIsJumbo)
	begin
//		opImm_imm12u[31:11] = istrJBits[20:0];
//		opImm_imm12n[31:11] = istrJBits[20:0];
		opImm_imm12s[31:11] = istrJBits[20:0];

//		opImm_disp12ld[31:11] = istrJBits[20:0];
//		opImm_disp12st[31:11] = istrJBits[20:0];
		opImm_disp8s  [31:11] = istrJBits[20:0];
	end

	if(opIsJumbo)
	begin
//		opImm_imm16u[11:0] = { istrJBits[23], istrJBits[10:0] };
//		opImm_imm16n[11:0] = { istrJBits[23], istrJBits[10:0] };
		opImm_imm16s[11:0] = { istrJBits[23], istrJBits[10:0] };
	end
`endif

//	opImm_imm10u	= opImm_imm12u;
//	opImm_imm10n	= opImm_imm12n;

	opImm_imm10s	= opImm_imm12s;
	opImm_disp12ld	= opImm_imm12s;

`ifdef jx2_enable_rvjumbo
	if(opIsJumbo)
	begin
		opImm_disp20s	= opImm_imm16s;
	end
`endif

	opIsNotFx = (istrWord[1:0] != 2'b11) && !isOpWxe;
	if(!isOpRiscV)
		opIsNotFx = 1;

	opNmid		= JX2_UCMD_INVOP;
	opRegN		= JX2_GR_ZZR;
	opRegM		= JX2_GR_ZZR;
	opRegO		= JX2_GR_ZZR;
	opRegP		= JX2_GR_ZZR;
	opImm		= 0;
	opFmid		= JX2_FMID_INV;
	opUIxt		= 0;
	opUCmdIx	= 0;
	opBty		= 0;
	opIty		= 0;
	opCcty		= JX2_IXC_AL;
	opUCty		= JX2_IUC_SC;
	opRegM_Fix	= JX2_GR_ZZR;
	opRegO_Fix	= JX2_GR_ZZR;
	opRegN_Fix	= JX2_GR_ZZR;
	amoLdOp		= 0;

	casez(istrWord[6:2])

`ifndef def_true
		5'b11_001: begin /* JALR */
			opNmid		= JX2_UCMD_JSR;
			opFmid		= JX2_FMID_REGIMMREG;
			opUCmdIx	= 0;
			opIty		= JX2_ITY_SW;

			if(istrWord[11:7]==5'h00)
			begin
				opNmid		= JX2_UCMD_JMP;
			end
		end


		5'b01_011: begin /* AMOxx, (Rm), Rn */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_REGIMMREG;
			opBty		= istrWord[14:12];
			opUCmdIx	= { opBty[1:0], 1'b0, opBty[2], 2'b00 };
			opIty		= JX2_ITY_NB;
			
			case(istrWord[31:27])
				5'b00000:	amoLdOp = 2;
				5'b00001:	amoLdOp = 1;
				5'b00010: begin
					opNmid		= JX2_UCMD_MOV_MR;
					amoLdOp = 0;
				end
				5'b00011:	amoLdOp = 0;

				5'b00100:	amoLdOp = 7;
				5'b01100:	amoLdOp = 5;
				5'b01000:	amoLdOp = 6;
				default:	amoLdOp = 0;
			endcase
			
			if(istrWord[26])
				amoLdOp[3] = 1;
		end
`endif

		5'b11_011: begin /* JAL */
			opNmid		= JX2_UCMD_JSR;
			opFmid		= JX2_FMID_PCDISP8;
			opBty		= JX2_BTY_SW;
			opIty		= JX2_ITY_SW;

			if(istrWord[11:7]==5'h00)
			begin
				opNmid		= JX2_UCMD_JMP;
			end
		end


`ifndef def_true
		5'b00_100: begin /* ALU OP, 3RI */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGIMMREG;
			opIty		= JX2_ITY_SW;

			case(istrWord[14:12])
				3'b000: opUCmdIx = JX2_UCIX_ALU_ADD;
				3'b001: begin
					opNmid		= JX2_UCMD_SHADQ3;
					opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
				end

				3'b010: opUCmdIx = JX2_UCIX_ALU_SLTSQ;
				3'b011: opUCmdIx = JX2_UCIX_ALU_SLTUQ;

				3'b100: opUCmdIx = JX2_UCIX_ALU_XOR;
				3'b101: begin
					opNmid		= JX2_UCMD_SHADQ3;
					opUCmdIx = istrWord[30] ?
						JX2_UCIX_SHAD_SHARQ3 : JX2_UCIX_SHAD_SHLRQ3;
				end
				3'b110: opUCmdIx = JX2_UCIX_ALU_OR;
				3'b111: opUCmdIx = JX2_UCIX_ALU_AND;
			endcase
		end
`endif

		5'b00_101: begin /* AUIPC */
			opNmid		= JX2_UCMD_LEA_MR;
			opFmid		= JX2_FMID_PCDISP8;
//			opIty		= JX2_ITY_UB;
			opIty		= JX2_ITY_SB;
			opBty		= JX2_BTY_SB;
		end

		5'b01_101: begin /* LUI */
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM8REG;
			opIty		= JX2_ITY_SB;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
		end


`ifndef def_true
		5'b00_110: begin /* ALU OP, 3RI */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGIMMREG;
			opIty		= JX2_ITY_SW;

			if(istrWord[30:25]==6'h01)
			begin
`ifndef def_true
				opNmid		= JX2_UCMD_QMULDIV;
				case(istrWord[14:12])
					3'b000: opUCmdIx = JX2_UCIX_QMUL_MULSL;
					3'b001: opUCmdIx = JX2_UCIX_QMUL_MULHSL;
					3'b010: opUCmdIx = JX2_UCIX_QMUL_MULUL;
					3'b011: opUCmdIx = JX2_UCIX_QMUL_MULHUL;
					3'b100: opUCmdIx = JX2_UCIX_QMUL_DUVSL;
					3'b101: opUCmdIx = JX2_UCIX_QMUL_DIVUL;
					3'b110: opUCmdIx = JX2_UCIX_QMUL_MODSL;
					3'b111: opUCmdIx = JX2_UCIX_QMUL_MODUL;
				endcase
`endif
			end
			else
			begin
				case(istrWord[14:12])
					3'b000: opUCmdIx = JX2_UCIX_ALU_ADDSL;
					3'b001: begin
						opNmid		= JX2_UCMD_SHAD3;
						opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
					end

					3'b010: opUCmdIx = JX2_UCIX_ALU_SLTSL;
					3'b011: opUCmdIx = JX2_UCIX_ALU_SLTUL;

					3'b100: opUCmdIx = JX2_UCIX_ALU_XOR;
					3'b101: begin
						opNmid		= JX2_UCMD_SHAD3;
						opUCmdIx = istrWord[30] ?
							JX2_UCIX_SHAD_SHAR3 : JX2_UCIX_SHAD_SHLR3;
					end
					3'b110: opUCmdIx = JX2_UCIX_ALU_OR;
					3'b111: opUCmdIx = JX2_UCIX_ALU_AND;
				endcase
			end
		end
`endif

		default: begin
		end
	endcase


	if(opNmid == JX2_UCMD_INVOP)
		opFmid = JX2_FMID_INV;

	if(opIsNotFx || !opIsJumbo96)
	begin
		opFmid	= JX2_FMID_Z;
		opIty	= JX2_ITY_SB;
	end

	opUCmd = { opCcty, opNmid };

	opUFl	= 0;
	opULdOp	= 0;

	case(opFmid)
		JX2_FMID_Z: begin
			opUIxt	= {opUCty, opUCmdIx[5:0]};
			opRegM	= opRegM_Fix;
			opRegO	= opRegO_Fix;
			opRegN	= opRegN_Fix;
			opRegP	= opRegN_Fix;
			case(opIty)
				JX2_ITY_SB: begin
				end

				default: begin
					$display("Jx2DecOpRvJO: Bad Reg, Ity=%X", opIty);
					$display("Jx2DecOpRvJO: Istr %X-%X-%X",
						istrWord[15:0], istrWord[31:16], istrWord[47:32]);
				end
			endcase
		end

`ifndef def_true
		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= { opUCty, opUCmdIx };

			case(opIty)
				JX2_ITY_SB: begin
				end
				JX2_ITY_SW: begin
					opImm	= opImm_imm12s;
				end
				JX2_ITY_UW: begin
					opImm	= opImm_imm12u;
				end

				JX2_ITY_NB: begin
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegP	= opRegO_Dfl;
					opRegN	= opRegN_Dfl;

					opULdOp	= amoLdOp;
				end

				JX2_ITY_NW: begin
					opImm	= opImm_imm12n;
				end
				
				default: begin
					$display("Jx2DecOpRvJO: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end
`endif

		/*
			SB: Fzeo_iiii		Ro, Imm16s, Ro
			SW: Fzeo_iiii		Imm16s, Ro, Ro
			SL: Fzeo_iiii		Imm16s, DLR, DLR
			SQ: Fzze_zznz_iiii	Rn, Imm10s, Rn

			UB: Fzeo_jjjj		Ro, Imm16u, Ro
			UW: Fzeo_jjjj		Imm16u, Ro, Ro
			UL: Fznz_zejj		Imm10u, Rn, Rn
			UQ: Fznz_zejj		Rn, Imm10u, Rn

			NB: Fzeo_jjjj		Ro, Imm16n, Ro
			NW: Fzeo_jjjj		Imm16n, Ro, Ro
			NL:
			NQ: Fznz_zejj		Rn, Imm10n, Rn

			XB: / Fzeo_zzjj		Imm8x, Ro
			XW: / Fzeo_jjjj		Imm16x, Ro

		 */
	
		JX2_FMID_IMM8REG: begin
//			$display("IMM8REG: IsJumbo=%d", opIsJumbo);
		
			opUIxt	= { opUCty, opUCmdIx };
			opUFl		= 0;
			opIsImm9	= 0;

			opRegM	= opRegImm16;
			opRegO	= opRegO_Df2;
			opRegN	= opRegO_Df2;
			opRegP	= opRegO_Df2;
			
			case(opIty)
				JX2_ITY_SB: begin
					opImm	= opImm_imm16s;
					opRegM	= opRegO_Df2;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

				JX2_ITY_SW: begin
					opImm	= opImm_imm16s;
				end

				JX2_ITY_SQ: begin
					opImm		= opImm_imm10s;
					opRegM		= opRegN_Dfl;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

				JX2_ITY_UB: begin
					opImm	= opImm_imm16u;
					opRegM	= opRegO_Df2;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

				default: begin
					$display("Jx2DecOpRvJO: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
			
			opUFl[0]	= opIsImm9;
		end

		/*
			SB: Fzdd_xxdd		(PC, disp16s), Rn
			SW: Fzdd_xddd		(PC, disp20s), Rn

			UB: Fzdd_xxdx		(PC, disp13s), Rn
			UW: Fzdd_xddd		(PC, disp9s), Rn
		 */
		JX2_FMID_PCDISP8: begin
//			opRegN	= JX2_GR_DLR;
//			opRegP	= JX2_GR_DLR;
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= JX2_GR_BPC;
			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};

			case(opIty)
			JX2_ITY_SB: begin
				opImm	= opImm_imm16s;
			end
			JX2_ITY_SW: begin
				opImm	= opImm_disp20s;
//				opRegM	= JX2_GR_BPC;	/* JAL Quirk */
			end

			JX2_ITY_UB: begin
				opImm	= opImm_imm16u;
			end

			JX2_ITY_UW: begin
				opImm	= opImm_disp12st;
			end

			default: begin
				$display("Jx2DecOpRvJO: PcDisp, Bad Ity=%X", opIty);
			end

			endcase
		end

		JX2_FMID_INV: begin
			if(!opIsNotFx)
			begin
				opUCmd = { opCcty, JX2_UCMD_INVOP };
				if(!tMsgLatch && isOpRiscV)
				begin
					$display("Jx2DecOpRvJO: Invalid FMID (32)");
					$display("Jx2DecOpRvJO: Istr %X-%X Mod=%X",
						istrWord[15:0], istrWord[31:16], srMod);
				end
				tNextMsgLatch=1;
			end
		end
		
		default: begin
			opUCmd = { opCcty, JX2_UCMD_INVOP };
			if(!tMsgLatch && !isAltOp)
			begin
				$display("Jx2DecOpRvJO: Unhandled FMID (32) %X", opFmid);
				$display("Jx2DecOpRvJO: Istr %X-%X",
					istrWord[15:0], istrWord[31:16]);
			end
			tNextMsgLatch=1;
		end
	endcase
	
	if(isOpRiscV && opIsJumbo96)
	begin
		if(opNmid == JX2_UCMD_INVOP)
		begin
			$display("DecOpRvJO: Saw INVOP %X-%X",
				istrWord[15:0], istrWord[31:16]);
		end
	end
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule
