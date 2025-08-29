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
RISC-V Base Decoder (32-bit encodings).
 */

`include "CoreDefs.v"

module DecOpRvI(
	/* verilator lint_off UNUSED */
	clock,		reset,		srMod,
	istrWord,	isAltOpB,	istrJBits,
	idRegN,		idRegM,
	idRegO,		idRegP,
	idImm,		idImmB,
	idUCmd,		idUIxt,
	idUFl
	);

input			clock;		//clock
input			reset;		//reset
// input[2:0]		srMod;		//mode
input[11:0]		srMod;		//mode

input[63:0]		istrWord;	//source instruction word
input[3:0]		isAltOpB;
input[27:0]		istrJBits;

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
`output_gpr		idRegP;
output[32:0]	idImm;
output[16:0]	idImmB;
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

wire			srStrictFp;
assign		srStrictFp = srMod[8];

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
reg[16:0]		opImmB;
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
assign	idImmB = opImmB;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;
// assign	idUFl = opUFl;
assign	idUFl = { 6'h0, 5'h0, opULdOp, opUFl };

`reg_gpr	opRegM_Dfl;
`reg_gpr	opRegO_Dfl;
`reg_gpr	opRegN_Dfl;
`reg_gpr	opRegP_Dfl;

`reg_gpr	opRegP_D4R;
reg			opIs4R;

`reg_gpr	opRegM_Fix;
`reg_gpr	opRegO_Fix;
`reg_gpr	opRegN_Fix;

`reg_gpr	opRegM_Fr;
`reg_gpr	opRegO_Fr;
`reg_gpr	opRegN_Fr;
`reg_gpr	opRegP_Fr;

`reg_gpr	opRegO_Df2;

`reg_gpr	opRegM_Df3R;
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

reg[32:0]		opImm_imm17s;
// reg[32:0]		opImm_imm10u;
// reg[32:0]		opImm_imm10n;

reg[32:0]		opImm_imm20s;
reg[32:0]		opImm_imm20u;
reg[32:0]		opImm_imm20n;

reg[32:0]		opImm_disp20s;
reg[32:0]		opImm_disp12b;
reg[32:0]		opImm_disp12ld;
reg[32:0]		opImm_disp12st;
// reg[32:0]		opImm_disp13s;

// reg[32:0]		opImm_disp5u;

reg[32:0]		opImm_imm5u;
//reg[32:0]		opImm_imm5n;

reg[32:0]		opImm_imm8au;
reg[32:0]		opImm_immfprm;

reg[32:0]		opImm_imm32lc;		//48-bit ops
reg[32:0]		opImm_imm21j;		//Jumbo

reg[5:0]	opNmid;
reg[4:0]	opFmid;
reg[2:0]	opBty;
reg[3:0]	opIty;
reg[2:0]	opCcty;

reg[5:0]	opUCmdIx;
reg[2:0]	opUCty;

reg[3:0]	opDoImm;
reg[3:0]	opJSubOp;

reg[32:0]	opImm_immRo;
reg			opDoImmRo;

reg		opExQ;
reg		opExN;
reg		opExM;
reg		opExI;
reg		opIsNotFx;
reg		opIsJumbo;
reg		opIsJumboImm;
reg		opIsJumboAu;
reg		opIsJumbo96;
reg		opIsImm9;

reg		opExWN;
reg		opExWM;
reg		opExWI;
reg		opExWQ;
reg		opIsXGpr;
reg		tOpIsXGprX0;
reg		tOpIsXGprX1;
reg		tOpIsXGprX2;

// reg tRegRmIsRz;
// reg tRegRnIsRz;
// reg tRegRoIsRz;
// reg tRegRmIsR0;
// reg tRegRmIsR1;
// reg tRegRnIsR0;
// reg tRegRnIsR1;
// reg tRegRoIsR0;
// reg tRegRoIsR1;

reg tRegRmIsRs;
reg tRegRnIsRs;
reg tRegRoIsRs;

reg tRegRmIsZr;
reg tRegRoIsZr;
reg tRegRnIsZr;
reg tRegCsrIsCpuid;
reg tRegImm12IsZero;

reg	tMsgLatch;
reg	tNextMsgLatch;

`ifdef jx2_enable_xgpr
wire[31:0]	usrRejectCmMask;
wire[31:0]	usrRejectCnMask;
assign	usrRejectCmMask = 32'b1111_1111_1111_1111_1111_1111_0011_1000;
assign	usrRejectCnMask = 32'b1111_1111_1111_1111_1111_1111_1011_1100;
`else
wire[15:0]	usrRejectCmMask;
wire[15:0]	usrRejectCnMask;
assign	usrRejectCmMask = 16'b1111_1111_0011_1000;
assign	usrRejectCnMask = 16'b1111_1111_1011_1100;
`endif

reg			usrRejectCmR;
reg			usrRejectCmW;
reg			usrRejectCoR;
reg			usrRejectCoW;
reg			usrRejectCnR;
reg			usrRejectCnW;
reg			usrReject;
reg[1:0]	usrSuAllow;
reg			usrSuAllowEn;

always @*
begin

//	tRegRoIsRs	= (istrWord[24:23]==2'b00);
//	tRegRmIsRs	= (istrWord[19:18]==2'b00);
//	tRegRnIsRs	= (istrWord[11:10]==2'b00);

	tRegRoIsRs	= (istrWord[24]==1'b0);
	tRegRmIsRs	= (istrWord[19]==1'b0);
	tRegRnIsRs	= (istrWord[11]==1'b0);

	tRegRmIsZr	= (istrWord[19:15] == 5'h00);
	tRegRoIsZr	= (istrWord[24:20] == 5'h00);
	tRegRnIsZr	= (istrWord[11: 7] == 5'h00);
	tRegImm12IsZero	= (istrWord[31:20] == 12'h000);

	opIsJumbo	= istrJBits[24];
	opIsJumbo96	= istrJBits[25];
	opIsJumboAu	= istrJBits[26] && opIsJumbo;
	opIsJumboImm	= !istrJBits[26] && opIsJumbo;

	opRegO_Dfl	= { 2'b00, istrWord[24:20] };
	opRegM_Dfl	= { 2'b00, istrWord[19:15] };
	opRegN_Dfl	= { 2'b00, istrWord[11: 7] };
//	opRegP_Dfl	= opRegN_Dfl;
	opRegP_Dfl	= { 2'b00, istrWord[31:27] };

	opRegO_Sr = opRegO_Dfl;
	opRegM_Sr = opRegM_Dfl;
	opRegN_Sr = opRegN_Dfl;

	opRegM_Fr	= { 2'b01, istrWord[19:15] };
	opRegO_Fr	= { 2'b01, istrWord[24:20] };
	opRegN_Fr	= { 2'b01, istrWord[11: 7] };
	opRegP_Fr	= { 2'b01, istrWord[31:27] };

	opRegP_D4R	= JX2_GR_ZZR;
	opIs4R		= 0;

	opExWN = 0;
	opExWM = 0;
	opExWI = 0;
	opExWQ = 0;
	opJSubOp	= 0;
	opDoImmRo	= 0;
	opImm_immRo	= 0;

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
		opRegM_Dfl[5] = istrJBits[19];
		opRegO_Dfl[5] = istrJBits[20];
		opRegN_Dfl[5] = istrJBits[18];
		opRegP_Dfl[5] = istrJBits[12];

		tRegRmIsRs	= (istrWord[19]==1'b0) && !istrJBits[19];
		tRegRoIsRs	= (istrWord[24]==1'b0) && !istrJBits[20];
		tRegRnIsRs	= (istrWord[11]==1'b0) && !istrJBits[18];
		
		opExWN		= istrJBits[18];
		opExWM		= istrJBits[19];
		opExWI		= istrJBits[20];
		opExWQ		= istrJBits[11];
		
		opJSubOp	= istrJBits[15:12];

		opRegP_D4R	= { 1'b0, istrJBits[5:0] };
		opIs4R		= opExWQ;

//		opDoImmRo	= istrJBits[11];
//		opImm_immRo	= { opExWI ? UV17_FF : UV17_00,
//			istrJBits[10:0], istrWord[24:20] };

		if(opExWM)
			tRegRmIsZr=0;
		if(opExWI)
			tRegRoIsZr=0;
		if(opExWN)
			tRegRnIsZr=0;
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

`ifdef jx2_reg_spdecswap
	if(srMod[2])
	begin
		if(opRegM_Dfl == JX2_GR_SP)
			opRegM_Dfl = JX2_GR_SSP;
		if(opRegO_Dfl == JX2_GR_SP)
			opRegO_Dfl = JX2_GR_SSP;
		if(opRegN_Dfl == JX2_GR_SP)
			opRegN_Dfl = JX2_GR_SSP;
	end
`endif

	opRegP_Dfl	= opRegN_Dfl;

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
		opRegM_Fr = opRegM_Dfl;
		opRegO_Fr = opRegO_Dfl;
		opRegN_Fr = opRegN_Dfl;
//		opRegP_Fr[5]	= istrJBits[12];
		opRegP_Fr = opRegP_Dfl;
	end
`endif


//	tRegRmIsRz = (opRegM_Dfl == JX2_GR_ZZR);
//	tRegRoIsRz = (opRegO_Dfl == JX2_GR_ZZR);
//	tRegRnIsRz = (opRegN_Dfl == JX2_GR_ZZR);

	opRegM_Cr	= 0;
	opRegO_Cr	= 0;
	opRegN_Cr	= 0;

	opRegO_Df2	= opRegN_Dfl;

	opRegM_Df3R	= opRegM_Dfl;
	opRegO_Df3R	= opRegO_Dfl;

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
//		if(istrJBits[11])
		if(opExWQ)
		begin
//			opRegM_Df3R	= JX2_GR_IMM;
			opRegO_Df3R	= JX2_GR_IMM;
		end
	end
	else
		if(opIsJumbo)
	begin
		opRegO_Df3R	= JX2_GR_IMM;
	end
`endif


	tRegCsrIsCpuid = 0;
	casez(istrWord[31:20])
		12'b0111_11zz_zzzz: begin
			opRegM_Cr = { 1'b1, !istrWord[25], istrWord[24:20] };
		end
		12'b1011_11zz_zzzz: begin
			opRegM_Cr = { 1'b0, istrWord[25:20] };
		end
		12'b1111_11zz_zzzz: begin
			tRegCsrIsCpuid = 1;
			opRegM_Cr = { 1'b0, istrWord[25:20] };
		end

		12'b1100_0000_00zz: begin
			tRegCsrIsCpuid = 1;
			casez(istrWord[21:20])
				2'b00: opRegM_Cr = JX2_GR_R30;
				2'b01: opRegM_Cr = JX2_GR_R28;
				2'b10: opRegM_Cr = JX2_GR_R28;
				2'b11: opRegM_Cr = JX2_GR_R31;
			endcase
		end

		12'b1111_0001_0zzz: begin
			tRegCsrIsCpuid = 1;
			casez(istrWord[22:20])
				3'b001: opRegM_Cr = JX2_GR_R0;
				3'b010: opRegM_Cr = JX2_GR_R0;
				3'b011: opRegM_Cr = JX2_GR_R0;
				3'b100: opRegM_Cr = JX2_GR_R1;
				default: opRegM_Cr = JX2_GR_R0;
			endcase
		end

		default: begin
		end
	endcase

`ifdef jx2_reg_spdecswap
	if(srMod[2])
	begin
		if(opRegM_Cr == JX2_GR_SSP)
			opRegM_Cr = JX2_GR_SP;
	end
`endif

	opRegN_Cr = opRegM_Cr;
	


//	opImm_imm5u	= {UV27_00, opRegO_Dfl[5:0]};
	opImm_imm5u	= {UV28_00, istrWord[24:20]};
//	opImm_imm5n	= {UV27_FF, opRegO_Dfl[5:0]};
//	opImm_imm5n	= {UV28_FF, istrWord[24:20]};

//	opImm_disp5u	= opImm_imm5u;
	opImm_imm8au	= opImm_imm5u;
	
	opImm_immfprm	= {UV30_00, istrWord[14:12]};	//FPU rounding mode

	tNextMsgLatch	= 0;

	opRegImm16	= JX2_GR_IMM;
	opRegImm10	= JX2_GR_IMM;
	opDoImm		= JX2_RVIMM_NONE;

	opImm_disp20s = {
		istrWord[31] ? UV14_FF : UV14_00,
		istrWord[19:12],
		istrWord[20],
		istrWord[30:21] };

	opImm_imm12u	= {UV21_00, istrWord[31:20]};
	opImm_imm12n	= {UV21_FF, istrWord[31:20]};
	
//	opImm_imm8au	= opImm_imm12u;

//	opImm_disp12ld	= opImm_imm12u;
//	opImm_disp12ld	= opImm_imm12s;
//	opImm_disp12st	= {UV21_00, istrWord[31:25], istrWord[11:7]};
	opImm_disp12st	= {
		istrWord[31] ? UV21_FF : UV21_00,
		istrWord[31:25], istrWord[11:7]};

//	opImm_imm10u	= opImm_imm12u;
//	opImm_imm10n	= opImm_imm12n;
//	opImm_imm10u	= {UV17_00, istrWord[19:15], istrWord[30:20]};
//	opImm_imm10n	= {UV17_FF, istrWord[19:15], istrWord[30:20]};

	opImm_imm17s	= {
		istrWord[31] ? UV17_FF : UV17_00,
		istrWord[19:15], istrWord[30:20]};

	opImm_disp12b	= {
		istrWord[31]?UV21_FF:UV21_00,
		istrWord[   31],
		istrWord[    7],
		istrWord[30:25],
		istrWord[11: 8]
		};

//	opImm_disp13s   = 0;

	opImm_imm20u	= { 1'b0, istrWord[31:12], 12'h000 };
	opImm_imm20n	= { 1'b1, istrWord[31:12], 12'h000 };
//	opImm_imm20s	= opImm_imm20u;

	opImm_imm12s	= istrWord[31] ? opImm_imm12n : opImm_imm12u;
	opImm_imm20s	= istrWord[31] ? opImm_imm20n : opImm_imm20u;
//	opImm_imm17s	= istrWord[31] ? opImm_imm10n : opImm_imm10u;

`ifdef jx2_enable_riscv_op48
	opImm_imm32lc	= {
		istrWord[47],
		istrWord[47:32],
		istrWord[31:16] };
`else
	opImm_imm32lc	= 0;
`endif

	opImm_imm21j	= { istrJBits[20] ? UV13_FF : UV13_00, istrJBits[19:0] };

`ifdef jx2_enable_rvjumbo
	if(opIsJumboAu)
	begin
//		opImm_imm12u[21:11]		= istrJBits[10:0];
//		opImm_imm12n[21:11]		= istrJBits[10:0];
		opImm_imm12s[21:11]		= istrJBits[10:0];
		
//		opImm_disp12ld[21:11]	= istrJBits[10:0];

		if(!opExWQ)
		begin
			opImm_disp12st[21:11]	= istrJBits[10:0];
			opImm_disp12b [21:11]	= istrJBits[10:0];
		end

		opImm_imm8au[15: 5]		= istrJBits[10:0];
		opImm_imm8au[32:16]		= istrJBits[20] ? UV17_FF : UV17_00;
		
//		$display("DecOpRvI: JumboAu istr=%X JBits=%X Imm12=%X",
//			istrWord[31:0], istrJBits, opImm_imm12s);
	end
	else
		if(opIsJumbo)
	begin
//		opImm_imm12u[31:11] = istrJBits[20:0];
//		opImm_imm12n[31:11] = istrJBits[20:0];
		opImm_imm12s[31:11] = istrJBits[20:0];

//		opImm_disp12ld[31:11] = istrJBits[20:0];
		opImm_disp12st[31:11] = istrJBits[20:0];
		opImm_disp12b [31:11] = istrJBits[20:0];

		opImm_imm8au[25:5]		= istrJBits[20:0];
		opImm_imm8au[32:26]		= istrJBits[20] ? UV7_FF : UV7_00;

//		$display("DecOpRvI: Jumbo istr=%X JBits=%X Imm12=%X",
//			istrWord[31:0], istrJBits, opImm_imm12s);
	end

	if(opIsJumbo)
	begin
//		opImm_imm20u[11:0]	= { istrJBits[23], istrJBits[10:0] };
//		opImm_imm20n[11:0]	= { istrJBits[23], istrJBits[10:0] };
		opImm_imm20s[11:0]	= { istrJBits[23], istrJBits[10:0] };
		opImm_imm20s[32]	= istrJBits[20];
	end
`endif

//	opImm_imm10u	= opImm_imm12u;
//	opImm_imm10n	= opImm_imm12n;

//	opImm_imm12s	= istrWord[31] ? opImm_imm12n : opImm_imm12u;
//	opImm_imm20s	= istrWord[31] ? opImm_imm20n : opImm_imm20u;
//	opImm_imm17s	= istrWord[31] ? opImm_imm10n : opImm_imm10u;

//	opImm_imm17s	= opImm_imm12s;
	opImm_disp12ld	= opImm_imm12s;

`ifdef jx2_enable_rvjumbo
	if(opIsJumbo)
	begin
//		opImm_disp20s	= opImm_imm20s;
		opImm_disp20s	= { opImm_imm20s[32], opImm_imm20s[32:1] };
	end
`endif

	usrReject		= 0;
	usrSuAllow		= 0;

	usrRejectCmR	= 0;
	usrRejectCmW	= 0;
	usrRejectCoR	= 0;
	usrRejectCoW	= 0;
	usrRejectCnR	= 0;
	usrRejectCnW	= 0;

//	if(istrWord[11:7]==5'h4)
//		usrReject		= 1;

//	opIsNotFx = (istrWord[1:0] != 2'b11) && !isOpWxe;
	opIsNotFx = (istrWord[1:0] != 2'b11);
	if(!isOpRiscV)
		opIsNotFx = 1;
	
	if(srMod[7:6]!=0)
		opIsNotFx = 1;

	opNmid		= JX2_UCMD_INVOP;
	opRegN		= JX2_GR_ZZR;
	opRegM		= JX2_GR_ZZR;
	opRegO		= JX2_GR_ZZR;
	opRegP		= JX2_GR_ZZR;
	opImm		= 0;
	opImmB		= 0;
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

		5'b00_000: begin /* LOAD, (Rm, Disp) */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= istrWord[14:12];
			opIty		= JX2_ITY_SB;
		end

		5'b01_000: begin /* STORE, (Rm, Disp) */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_REGSTREGDISP;
			opBty		= istrWord[14:12];
			opIty		= JX2_ITY_SB;

			case(istrWord[14:12])
				3'b101: begin
					//Load Indexed
					amoLdOp		= 0;
					opNmid		= JX2_UCMD_MOV_MR;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opBty		= istrWord[29:27];
					opUCmdIx	= {
						opBty[1:0], 1'b0, opBty[2], istrWord[31:30] };
				end
				3'b110: begin
					//Store Indexed
					amoLdOp		= 0;
					opNmid		= JX2_UCMD_MOV_RM;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opBty		= istrWord[29:27];
					opUCmdIx	= {
						opBty[1:0], 1'b0, opBty[2], istrWord[31:30] };
				end

				default: begin
				end
			endcase
		end

`ifdef jx2_fpu_fmac
		5'b10_000: begin /* FMADD */
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NW;
			opUCmdIx	= JX2_UCIX_FPU_FMAC;

			if(istrWord[26:25]==2'b00)
				opUCmdIx	= JX2_UCIX_FPU_PMAC;
		end
`endif

		5'b11_000: begin /* BRANCH */
			opNmid		= JX2_UCMD_JCMP;
			opFmid		= JX2_FMID_REGPC;
//			opUCmdIx	= { 4'h0, istrWord[11], 2'b11, istrWord[8]};
			opIty		= JX2_ITY_UB;

			case(istrWord[14:12])
				3'b000: opUCmdIx = JX2_UCIX_JCMP_QEQ;
				3'b001: opUCmdIx = JX2_UCIX_JCMP_QNE;
				3'b010: opUCmdIx = JX2_UCIX_JCMP_QEQ;
				3'b011: opUCmdIx = JX2_UCIX_JCMP_QEQ;
				3'b100: opUCmdIx = JX2_UCIX_JCMP_QLT;
				3'b101: opUCmdIx = JX2_UCIX_JCMP_QGE;
				3'b110: opUCmdIx = JX2_UCIX_JCMP_QBL;
				3'b111: opUCmdIx = JX2_UCIX_JCMP_QHS;
			endcase
		end


		5'b00_001: begin /* FP_LOAD, (Rm, Disp) */
			opNmid		= JX2_UCMD_MOV_MR;
			opFmid		= JX2_FMID_LDREGDISPREG;
			opBty		= { 2'b01, istrWord[12] };
			opIty		= JX2_ITY_XB;
			
			if(istrWord[14:12]==3'b010)
			begin
				/* One Extended special case. */
				opNmid	= JX2_UCMD_FMOV_MR;
				opBty	= JX2_BTY_SB;
			end
		end

		5'b01_001: begin /* FP_STORE, (Rm, Disp) */
			opNmid		= JX2_UCMD_MOV_RM;
			opFmid		= JX2_FMID_REGSTREGDISP;
			opBty		= { 2'b01, istrWord[12] };
			opIty		= JX2_ITY_XB;

			casez(istrWord[14:12])
				3'b000: opBty = JX2_BTY_SB;		//'V'
				3'b001: opBty = JX2_BTY_SW;		//Binary16
				3'b010: opBty = JX2_BTY_SL;		//Binary32
				3'b011: opBty = JX2_BTY_SQ;		//Binary64
				3'b100: opBty = JX2_BTY_UQ;		//Pair
				3'b101: opBty = JX2_BTY_UQ;		//'V'
				3'b110: opBty = JX2_BTY_UQ;		//'V'
				3'b111: opBty = JX2_BTY_UQ;		//'V'
			endcase
		end

`ifdef jx2_fpu_fmac
		5'b10_001: begin /* FMSUB */
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NW;
			opUCmdIx	= JX2_UCIX_FPU_FMAS;

			if(istrWord[26:25]==2'b00)
				opUCmdIx	= JX2_UCIX_FPU_PMAS;
		end
`endif

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

`ifdef jx2_fpu_fmac
		5'b10_010: begin /* FNMSUB */
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NW;
			opUCmdIx	= JX2_UCIX_FPU_FMRS;

			if(istrWord[26:25]==2'b00)
				opUCmdIx	= JX2_UCIX_FPU_PMRS;
		end
`endif

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

				5'b00110: begin
					//Load Indexed
					amoLdOp		= 0;
					opNmid		= JX2_UCMD_MOV_MR;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opBty		= istrWord[14:12];
					opUCmdIx	= {
						opBty[1:0], 1'b0, opBty[2], istrWord[26:25] };
				end
				5'b00111: begin
					//Store Indexed
					amoLdOp		= 0;
					opNmid		= JX2_UCMD_MOV_RM;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opBty		= istrWord[14:12];
					opUCmdIx	= {
						opBty[1:0], 1'b0, opBty[2], istrWord[26:25] };
				end

				5'b00100:	amoLdOp = 7;
				5'b01100:	amoLdOp = 5;
				5'b01000:	amoLdOp = 6;
				default:	amoLdOp = 0;
			endcase
			
//			if(istrWord[26])
			if(istrWord[26] && (opIty != JX2_ITY_SB))
				amoLdOp[3] = 1;
		end

`ifdef jx2_fpu_fmac
		5'b10_011: begin /* FNMADD */
			opNmid		= JX2_UCMD_FPU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_NW;
			opUCmdIx	= JX2_UCIX_FPU_FMRA;
			
			if(istrWord[26:25]==2'b00)
				opUCmdIx	= JX2_UCIX_FPU_PMRA;
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


		5'b00_100: begin /* ALU OP, 3RI */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGIMMREG;
			opIty		= JX2_ITY_SW;

			case(istrWord[14:12])
				3'b000: begin
					opUCmdIx = JX2_UCIX_ALU_ADD;
					if(!opIsJumbo)
					begin
						if(tRegRmIsZr)
						begin
							/* Special Case: MOV Imm, Rn */
							opNmid		= JX2_UCMD_MOV_IR;
							opUCmdIx	= JX2_UCIX_LDI_LDIX;
						end
`ifndef def_true
						else
							if(tRegImm12IsZero)
						begin
							/* Special Case: MOV Rm, Rn */
							opNmid		= JX2_UCMD_MOV_IR;
							opUCmdIx	= JX2_UCIX_LDI_LDIX;
							opIty		= JX2_ITY_UL;
						end
`endif
					end

`ifdef def_true
// `ifndef def_true
					if(opIsJumbo)
					begin
						if(tRegRmIsZr)
						begin
							/* Special Case: MOV Imm, Rn */
							opNmid		= JX2_UCMD_MOV_IR;
							opUCmdIx	= JX2_UCIX_LDI_LDIX;
						end
					end
`endif

// `ifdef def_true
`ifndef def_true
					if(opIsJumbo)
					begin
						if(istrWord[19:15]==5'h03)
						begin
							opNmid		= JX2_UCMD_LEA_MR;
							opFmid		= JX2_FMID_LDREGDISPREG;
							opBty		= JX2_BTY_SB;
							opIty		= JX2_ITY_SB;
						end
					end
`endif
				end
				3'b001: begin
					case(istrWord[31:26])
						6'h00: begin
							opNmid		= JX2_UCMD_SHADQ3;
							opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
						end
						6'h02: begin
							opNmid		= JX2_UCMD_SHADQ3;
							opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
						end
						default: begin
							opNmid		= JX2_UCMD_INVOP;
						end
					endcase
				end

				3'b010: begin
					opUCmdIx = JX2_UCIX_ALU_SLTSQ;
					opIty		= JX2_ITY_SW;
//					opIty		= JX2_ITY_SL;
					if(opJSubOp!=0)
					begin
						opNmid		= JX2_UCMD_ALUCMP3R;
						case(opJSubOp)
							4'h1: 		opUCmdIx = JX2_UCIX_ALU_CMPQEQ;
							4'h2: 		opUCmdIx = JX2_UCIX_ALU_CMPQGE;
							4'h3: 		opUCmdIx = JX2_UCIX_ALU_CMPQNE;
							4'h5: 		opUCmdIx = JX2_UCIX_ALU_TSTNQ;
							4'h7: 		opUCmdIx = JX2_UCIX_ALU_TSTQ;
							default:	opUCmdIx = JX2_UCIX_ALU_CMPQGT;
						endcase
					end
				end
				3'b011: opUCmdIx = JX2_UCIX_ALU_SLTUQ;

				3'b100: opUCmdIx = JX2_UCIX_ALU_XOR;
				3'b101: begin
//					opNmid		= JX2_UCMD_SHADQ3;
//					opUCmdIx = istrWord[30] ?
//						JX2_UCIX_SHAD_SHARQ3 : JX2_UCIX_SHAD_SHLRQ3;
					case(istrWord[31:26])
						6'h00: begin
							opNmid		= JX2_UCMD_SHADQ3;
							opUCmdIx	= JX2_UCIX_SHAD_SHLRQ3;
						end
						6'h10: begin
							opNmid		= JX2_UCMD_SHADQ3;
							opUCmdIx	= JX2_UCIX_SHAD_SHARQ3;
						end
						default: begin
							opNmid		= JX2_UCMD_INVOP;
						end
					endcase
				end
				3'b110: opUCmdIx = JX2_UCIX_ALU_OR;
				3'b111: opUCmdIx = JX2_UCIX_ALU_AND;
			endcase
		end

		5'b01_100: begin /* ALU OP, 3R */
//			if(istrWord[30:25]==6'h01)
			if(istrWord[29:25]==5'h01)
			begin
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;

				case(istrWord[14:12])
					3'b000: opUCmdIx = JX2_UCIX_QMUL_MULS;
					3'b001: opUCmdIx = JX2_UCIX_QMUL_MULHS;
					3'b010: opUCmdIx = JX2_UCIX_QMUL_MULHSU;
					3'b011: opUCmdIx = JX2_UCIX_QMUL_MULHU;
					3'b100: opUCmdIx = JX2_UCIX_QMUL_DIVS;
					3'b101: opUCmdIx = JX2_UCIX_QMUL_DIVU;
					3'b110: opUCmdIx = JX2_UCIX_QMUL_MODS;
					3'b111: opUCmdIx = JX2_UCIX_QMUL_MODU;
				endcase
			end

			if(istrWord[29:25]==5'h00)
			begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;

				case(istrWord[14:12])
					3'b000: begin
						opUCmdIx = istrWord[30] ?
							JX2_UCIX_ALU_SUB : JX2_UCIX_ALU_ADD;

`ifndef def_true
						if(!opIsJumbo && tRegRoIsZr)
						begin
							/* Special Case: MOV Rm, Rn */
							opNmid		= JX2_UCMD_MOV_IR;
							opUCmdIx	= JX2_UCIX_LDI_LDIX;
							opIty		= JX2_ITY_NB;
						end
`endif
					end

					3'b001: begin
						opNmid		= JX2_UCMD_SHADQ3;
						opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
					end

					3'b010: begin
						opUCmdIx = JX2_UCIX_ALU_SLTSQ;
//						opNmid		= JX2_UCMD_ALUCMP3R;
//						opIty		= JX2_ITY_SQ;
//						opUCmdIx	= JX2_UCIX_ALU_CMPQGT;
						
						if(istrWord[30])
						begin
							opNmid		= JX2_UCMD_ALUCMP3R;
							opIty		= JX2_ITY_SB;
							opUCmdIx	= JX2_UCIX_ALU_CMPQGE;
						end

						if(istrWord[31])
						begin
							opNmid		= JX2_UCMD_ALUCMP3R;
							opIty		= JX2_ITY_SB;
							opUCmdIx	= JX2_UCIX_ALU_CMPQEQ;
							if(istrWord[30])
								opUCmdIx	= JX2_UCIX_ALU_CMPQNE;
						end
						
						if(opJSubOp!=0)
						begin
							opNmid		= JX2_UCMD_ALUCMP3R;
							opIty		= JX2_ITY_SB;
//							opIty		= JX2_ITY_SQ;

							case(opJSubOp)
								4'h1: 		opUCmdIx = JX2_UCIX_ALU_CMPQEQ;
								4'h2: 		opUCmdIx = JX2_UCIX_ALU_CMPQGE;
								4'h3: 		opUCmdIx = JX2_UCIX_ALU_CMPQNE;
								4'h5: 		opUCmdIx = JX2_UCIX_ALU_TSTNQ;
								4'h7: 		opUCmdIx = JX2_UCIX_ALU_TSTQ;
								default:	opUCmdIx = JX2_UCIX_ALU_CMPQGT;
							endcase
						end
					end

					3'b011:
					begin
						opUCmdIx = JX2_UCIX_ALU_SLTUQ;
//						opNmid		= JX2_UCMD_ALUCMP3R;
//						opIty		= JX2_ITY_SQ;
//						opUCmdIx	= JX2_UCIX_ALU_CMPQHI;
						
						if(istrWord[30])
						begin
							opNmid		= JX2_UCMD_ALUCMP3R;
							opIty		= JX2_ITY_SB;
							opUCmdIx	= JX2_UCIX_ALU_CMPQHS;
						end

						if(istrWord[31])
						begin
							opNmid		= JX2_UCMD_ALUCMP3R;
							opIty		= JX2_ITY_SB;
							opUCmdIx	= JX2_UCIX_ALU_TSTNQ;
							if(istrWord[30])
								opUCmdIx	= JX2_UCIX_ALU_TSTQ;
						end
						
					end

					3'b100: opUCmdIx = JX2_UCIX_ALU_XOR;
					3'b101: begin
						opNmid		= JX2_UCMD_SHADQ3;
						opUCmdIx = istrWord[30] ?
							JX2_UCIX_SHAD_SHARQ3 : JX2_UCIX_SHAD_SHLRQ3;
						if(opIsJumboImm)
						begin
							opUCmdIx	= JX2_UCIX_SHAD_SHLDMSKL3;
							opFmid		= JX2_FMID_REGIMMREG;
							opIty		= JX2_ITY_XQ;
							if(istrWord[30])
							begin
								opUCmdIx	= JX2_UCIX_SHAD_SHLDMSKLX3;
								opUCty		= JX2_IUC_WX;
							end
						end
					end
					3'b110: opUCmdIx = JX2_UCIX_ALU_OR;
					3'b111: opUCmdIx = JX2_UCIX_ALU_AND;
				endcase
			end

			if(istrWord[29:25]==5'h04)
			begin
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;

				case(istrWord[14:12])
					3'b100: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLD;
						if(istrWord[30])
							opUCmdIx	= JX2_UCIX_ALUW_MOVHD;
					end
					default: begin
					end
				endcase
			end

			if(istrWord[29:25]==5'h10)
			begin
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SB;
				opIty		= JX2_ITY_SQ;

				case(istrWord[14:12])
					3'b010: begin
						opNmid		= JX2_UCMD_LEA_MR;
						opUCmdIx	= 6'h01;
					end
					3'b100: begin
						opNmid		= JX2_UCMD_LEA_MR;
						opUCmdIx	= 6'h02;
					end
					3'b110: begin
						opNmid		= JX2_UCMD_LEA_MR;
						opUCmdIx	= 6'h03;
					end
					default: begin
					end
				endcase
			end
		end

		5'b10_100: begin /* FPU OP, 3R */
//			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_XB;

			casez(istrWord[31:25])
				7'b0000000: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PADD;
`ifdef jx2_use_fpu_v4sf
`ifdef jx2_fpu_v4sf_fullsp
					if	(istrWord[14:13]==2'b00 ||
						(istrWord[14:12]==3'b101) ||
						(istrWord[14:12]==3'b110) ||
						(!srStrictFp && (istrWord[14:12]!=3'b111)))
							opNmid		= JX2_UCMD_FPUV4SF;
`endif
`endif
				if((istrWord[14:12]==3'b101) || (istrWord[14:12]==3'b110))
					opUCty		= JX2_IUC_WX;

				end
				7'b0000001: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_FADD;
					if(istrWord[14:12]==3'b111)
						opUCmdIx	= JX2_UCIX_FPU_FADD_G;
				end
				7'b0000010: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PADDH;
`ifdef jx2_use_fpu_v4sf
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
				end

				7'b0000100: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PSUB;
`ifdef jx2_use_fpu_v4sf
`ifdef jx2_fpu_v4sf_fullsp
					if	(istrWord[14:13]==2'b00 ||
						(istrWord[14:12]==3'b101) ||
						(istrWord[14:12]==3'b110) ||
						(!srStrictFp && (istrWord[14:12]!=3'b111)))
							opNmid		= JX2_UCMD_FPUV4SF;
`endif
`endif
				if((istrWord[14:12]==3'b101) || (istrWord[14:12]==3'b110))
					opUCty		= JX2_IUC_WX;

				end
				7'b0000101: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_FSUB;
					if(istrWord[14:12]==3'b111)
						opUCmdIx	= JX2_UCIX_FPU_FSUB_G;
				end
				7'b0000110: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PSUBH;
`ifdef jx2_use_fpu_v4sf
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
				end

				7'b0001000: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PMUL;
`ifdef jx2_use_fpu_v4sf
`ifdef jx2_fpu_v4sf_fullsp
					if	(istrWord[14:13]==2'b00 ||
						(istrWord[14:12]==3'b101) ||
						(istrWord[14:12]==3'b110) ||
						(!srStrictFp && (istrWord[14:12]!=3'b111)))
							opNmid		= JX2_UCMD_FPUV4SF;
`endif
`endif
				if((istrWord[14:12]==3'b101) || (istrWord[14:12]==3'b110))
					opUCty		= JX2_IUC_WX;

				end
				7'b0001001: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_FMUL;
					if(istrWord[14:12]==3'b111)
						opUCmdIx	= JX2_UCIX_FPU_FMUL_G;
				end
				7'b0001010: begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PMULH;
`ifdef jx2_use_fpu_v4sf
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
				end

				7'b0001100: begin
`ifdef jx2_fpu_enable_fdiv
					opNmid		= JX2_UCMD_OP_IXT;
					opUCmdIx	= JX2_UCIX_IXT_TRAPFPU;

//					opNmid		= JX2_UCMD_FPU3;
//					opUCmdIx	= JX2_UCIX_FPU_PMUL;
`ifdef jx2_alu_slomuldiv_fdivs
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_FDIVS;
`endif
`else
					opNmid		= JX2_UCMD_OP_IXT;
					opUCmdIx	= JX2_UCIX_IXT_TRAPFPU;
`endif
				end
				7'b0001101: begin
`ifdef jx2_fpu_enable_fdiv
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_FDIV;
`ifdef jx2_alu_slomuldiv_fdiv
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_FDIV;
`endif
`else
					opNmid		= JX2_UCMD_OP_IXT;
					opUCmdIx	= JX2_UCIX_IXT_TRAPFPU;
`endif
				end


				7'b0010000: begin
					opNmid		= JX2_UCMD_UNARY;
					case(istrWord[14:12])
						3'b000:		opUCmdIx	= JX2_UCIX_ALUN_FSGNJS;
						3'b001:		opUCmdIx	= JX2_UCIX_ALUN_FSGNJNS;
						3'b010:		opUCmdIx	= JX2_UCIX_ALUN_FSGNJXS;
						default:	opUCmdIx	= JX2_UCIX_ALUN_FSGNJS;
					endcase
				end

				7'b0010001: begin
					opNmid		= JX2_UCMD_UNARY;
					case(istrWord[14:12])
						3'b000:		opUCmdIx	= JX2_UCIX_ALUN_FSGNJ;
						3'b001:		opUCmdIx	= JX2_UCIX_ALUN_FSGNJN;
						3'b010:		opUCmdIx	= JX2_UCIX_ALUN_FSGNJX;
						default:	opUCmdIx	= JX2_UCIX_ALUN_FSGNJ;
					endcase
				end

				7'b0010100: begin
					opNmid		= JX2_UCMD_UNARY;
					case(istrWord[14:12])
						3'b000:		opUCmdIx	= JX2_UCIX_ALUN_FMINS;
						3'b001:		opUCmdIx	= JX2_UCIX_ALUN_FMAXS;
						default:	opUCmdIx	= JX2_UCIX_ALUN_FMINS;
					endcase
				end

				7'b0010101: begin
					opNmid		= JX2_UCMD_UNARY;
					case(istrWord[14:12])
						3'b000:		opUCmdIx	= JX2_UCIX_ALUN_FMIN;
						3'b001:		opUCmdIx	= JX2_UCIX_ALUN_FMAX;
						default:	opUCmdIx	= JX2_UCIX_ALUN_FMIN;
					endcase
				end

				7'b0100000: begin
					opNmid		= JX2_UCMD_FSTCX;
					opUCmdIx	= JX2_UCIX_FPCX_SG;
					opIty		= JX2_ITY_XW;
					case(istrWord[21:20])
						2'b00: begin
							opNmid		= JX2_UCMD_CONV_RR;
							opUCmdIx	= JX2_UCIX_CONV_MOV;
						end
						2'b01: begin
							opUCmdIx	= JX2_UCIX_FPCX_SG;
						end
						2'b10: begin
//							opUCmdIx	= JX2_UCIX_FPCX_HG;
							opNmid		= JX2_UCMD_CONV2_RR;
							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
							if(istrWord[23])
								opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32H;
						end
						2'b11:	opUCmdIx	= JX2_UCIX_FPCX_SG;	// -
					endcase

				end

				7'b0100001: begin
					opNmid		= JX2_UCMD_FLDCX;
					opUCmdIx	= JX2_UCIX_FPCX_SG;
					opIty		= JX2_ITY_XW;
					case(istrWord[21:20])
						2'b00: begin
							opUCmdIx	= JX2_UCIX_FPCX_SG;
						end
						2'b01: begin
							opNmid		= JX2_UCMD_CONV_RR;
							opUCmdIx	= JX2_UCIX_CONV_MOV;
						end
						2'b10: begin
							opUCmdIx	= JX2_UCIX_FPCX_HG;
						end
						2'b11:	opUCmdIx	= JX2_UCIX_FPCX_SG;	// -
					endcase
				end

				7'b0100010: begin
					opNmid		= JX2_UCMD_FSTCX;
					opUCmdIx	= JX2_UCIX_FPCX_SG;
					opIty		= JX2_ITY_XW;
					case(istrWord[21:20])
						2'b00: begin
							opUCmdIx	= JX2_UCIX_FPCX_HG;
						end
						2'b01: begin
							opNmid		= JX2_UCMD_CONV2_RR;
							opUCmdIx	= JX2_UCIX_CONV2_FP16PCK32;
						end
						2'b10: begin
							opNmid		= JX2_UCMD_CONV_RR;
							opUCmdIx	= JX2_UCIX_CONV_MOV;
						end
						2'b11: begin
						end
					endcase
				end

				7'b0101100: begin
//					opNmid		= JX2_UCMD_FPU3;
//					opUCmdIx	= JX2_UCIX_FPU_PMUL;
					opNmid		= JX2_UCMD_CONV2_RR;
					opUCmdIx	= JX2_UCIX_CONV2_PSQRTUAF;
					opIty		= JX2_ITY_XW;
				end
				7'b0101101: begin
`ifndef def_true
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_FSQRT;
					opIty		= JX2_ITY_XW;
`else
					opNmid		= JX2_UCMD_OP_IXT;
					opUCmdIx	= JX2_UCIX_IXT_TRAPFPU;
`endif
				end

				7'b1010000: begin
					opNmid		= JX2_UCMD_FCMP;
//					opIty		= JX2_ITY_XL;
					opIty		= JX2_ITY_XQ;

					case(istrWord[14:12])
						3'b000:		opUCmdIx	= JX2_UCIX_FCMP_PCMPGE_R;
						3'b001:		opUCmdIx	= JX2_UCIX_FCMP_PCMPGT_R;
						3'b010:		opUCmdIx	= JX2_UCIX_FCMP_PCMPEQ_R;
						default:	opUCmdIx	= JX2_UCIX_FCMP_PCMPEQ_R;
					endcase

				end
				7'b1010001: begin
					opNmid		= JX2_UCMD_FCMP;
//					opIty		= JX2_ITY_XL;
					opIty		= JX2_ITY_XQ;

					case(istrWord[14:12])
						3'b000:		opUCmdIx	= JX2_UCIX_FCMP_CMPGE_R;
						3'b001:		opUCmdIx	= JX2_UCIX_FCMP_CMPGT_R;
						3'b010:		opUCmdIx	= JX2_UCIX_FCMP_CMPEQ_R;
						default:	opUCmdIx	= JX2_UCIX_FCMP_CMPEQ_R;
					endcase
				end


//				7'b1100000: begin
				7'b1101000: begin
					opNmid		= JX2_UCMD_FLDCX;
					opUCmdIx	= JX2_UCIX_FPCX_SIG;
					opIty		= JX2_ITY_NQ;

					case(istrWord[22:20])
						3'b000:		opUCmdIx=JX2_UCIX_FPCX_SISL;
						3'b001:		opUCmdIx=JX2_UCIX_FPCX_SIUL;
						3'b010:		opUCmdIx=JX2_UCIX_FPCX_SIG;
						3'b011:		opUCmdIx=JX2_UCIX_FPCX_SIU;
						default:	opUCmdIx=JX2_UCIX_FPCX_SIG;
					endcase
				end

//				7'b1100001: begin
				7'b1101001: begin
					opNmid		= JX2_UCMD_FLDCX;
					opUCmdIx	= JX2_UCIX_FPCX_IG;
					opIty		= JX2_ITY_NQ;

					case(istrWord[22:20])
						3'b000:		opUCmdIx=JX2_UCIX_FPCX_ISL;
						3'b001:		opUCmdIx=JX2_UCIX_FPCX_IUL;
						3'b010:		opUCmdIx=JX2_UCIX_FPCX_I;
						3'b011:		opUCmdIx=JX2_UCIX_FPCX_IU;
						default:	opUCmdIx=JX2_UCIX_FPCX_IG;
					endcase
				end

//				7'b1101000: begin
				7'b1100000: begin
					opNmid		= JX2_UCMD_FSTCX;
					opUCmdIx	= JX2_UCIX_FPCX_SIG;
					opIty		= JX2_ITY_NL;
				end
				
//				7'b1101001: begin
				7'b1100001: begin
					opNmid		= JX2_UCMD_FSTCX;
					opUCmdIx	= JX2_UCIX_FPCX_IG;
					opIty		= JX2_ITY_NL;
				end


				7'b111000z: begin
					opNmid		= JX2_UCMD_CONV_RR;
					opUCmdIx	= JX2_UCIX_CONV_MOV;
//					opIty		= JX2_ITY_NQ;
					opIty		= JX2_ITY_NL;
				end

				7'b111100z: begin
					opNmid		= JX2_UCMD_CONV_RR;
					opUCmdIx	= JX2_UCIX_CONV_MOV;
//					opIty		= JX2_ITY_NL;
					opIty		= JX2_ITY_NQ;
				end

				default: begin
				end

			endcase
		end


		5'b11_100: begin /* SYSTEM */
			opFmid		= JX2_FMID_REGREG;
			opIty		= JX2_ITY_SB;

			case(istrWord[14:12])
				3'b000: begin
					case(istrWord[23:20])
						4'h0: begin
							opNmid		= JX2_UCMD_OP_IXT;
							opFmid		= JX2_FMID_Z;
							opUCmdIx	= JX2_UCIX_IXT_SYSE;
						end
						4'h1: begin
							opNmid		= JX2_UCMD_OP_IXT;
							opFmid		= JX2_FMID_Z;
							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
						4'h2: begin
							opNmid		= JX2_UCMD_OP_IXT;
							opFmid		= JX2_FMID_Z;
							opUCmdIx	= JX2_UCIX_IXT_RTE;
						end

						4'h5: begin
							opNmid		= JX2_UCMD_OP_IXT;
							opFmid		= JX2_FMID_Z;
							opUCmdIx	= JX2_UCIX_IXT_SLEEP;
						end

						default: begin
						end
					endcase
				end

				3'b001: begin
					if(tRegRnIsZr)
					begin
						opNmid	= JX2_UCMD_MOV_RC;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UL;
					end

					if(tRegRmIsZr)
					begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
					end

					if(tRegCsrIsCpuid)
					begin
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_CPUID;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UQ;
					end
				end

				3'b010: begin
					if(tRegRmIsZr)
					begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
					end
					
					if(tRegCsrIsCpuid)
					begin
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_CPUID;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UQ;
					end
				end

				3'b011: begin
					if(tRegRmIsZr)
					begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
					end
				end
				
				default: begin
				end
			endcase
		end

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
//			opIty		= JX2_ITY_SB;
//			opIty		= JX2_ITY_UB;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
			
			if(opExWM)
			begin
				opNmid		= JX2_UCMD_CONV_RR;
				opUCmdIx	= JX2_UCIX_CONV_LDIQHI32;
				opIty		= JX2_ITY_SW;
			end

			if(opExWQ)
			begin
				opNmid		= JX2_UCMD_CONV2_RR;
				opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
				opIty		= JX2_ITY_SW;

				if(opExWM)
					opUCmdIx	= JX2_UCIX_CONV2_FP16UPCKFP8;
				
			end
		end

		5'b10_101: begin /* OP-V */
		end

		5'b11_101: begin /* OP-P */
			case(istrWord[14:12])
			3'b001: begin
				case(istrWord[31:25])
					7'b0000111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLW;
					end
					7'b0001111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLHW;
					end
					7'b0010111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLHW;
					end
					7'b0011111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLHW;
					end
					default: begin
					end
				endcase
			end

			3'b010: begin
				case(istrWord[31:25])
					7'b0000111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLD;
					end
					7'b0001111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLHD;
					end
					7'b0010111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLHD;
					end
					7'b0011111: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLHD;
					end
					default: begin
					end
				endcase
			end

			default: begin
			end
			endcase
		end

		5'b00_110: begin /* ALU OP, 32-bit, 3RI */
			opNmid		= JX2_UCMD_ALU3;
			opFmid		= JX2_FMID_REGIMMREG;
			opIty		= JX2_ITY_SW;

			case(istrWord[14:12])
				3'b000: opUCmdIx = JX2_UCIX_ALU_ADDSL;
				3'b001: begin
					casez(istrWord[31:26])
						6'b000000: begin
							opNmid		= JX2_UCMD_SHAD3;
							opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
						end
						6'b0100zz: begin
							opNmid		= JX2_UCMD_ALUW3;
							opUCmdIx	= JX2_UCIX_ALUW_PSHUFW;
						end
						6'b0110zz: begin
							opNmid		= JX2_UCMD_ALUW3;
							opUCmdIx	= JX2_UCIX_ALUW_PSHUFW;
							opIty		= JX2_ITY_XB;
						end
						default: begin
						end
					endcase
				end

				3'b010: opUCmdIx = JX2_UCIX_ALU_SLTSL;
				3'b011: opUCmdIx = JX2_UCIX_ALU_SLTUL;

				3'b100:
				begin
					opNmid		= JX2_UCMD_NOP;
					opFmid		= JX2_FMID_Z;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_ALU_ADD;
				end
				3'b101: begin
					opNmid		= JX2_UCMD_SHAD3;
					opUCmdIx = istrWord[30] ?
						JX2_UCIX_SHAD_SHAR3 : JX2_UCIX_SHAD_SHLR3;
				end
				3'b110: begin
//					opUCmdIx = JX2_UCIX_ALU_OR;

					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SQ;
					opUCmdIx	= JX2_UCIX_LDI_LDISH16;
					
					if(istrWord[31])
					begin
						opUCmdIx	= JX2_UCIX_LDI_FLDCH;
					end

					if(tRegRnIsZr)
					begin
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_BREAK;
					end

					if(opIsJumboAu)
					begin
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SW;
					end
					else
					if(opIsJumbo)
					begin
						opUCmdIx	= JX2_UCIX_LDI_LDISH32;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SW;
					end
				end
				3'b111:
				begin
//					opUCmdIx = JX2_UCIX_ALU_AND;

					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SQ;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
				end
			endcase
		end

		5'b01_110: begin /* ALU OP, 32-bit, 3R */

			if(istrWord[29:25]==5'h00)
			begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;

				case(istrWord[14:12])
					3'b000: begin
						opUCmdIx = istrWord[30] ?
							JX2_UCIX_ALU_SUBSL : JX2_UCIX_ALU_ADDSL;
					end

					3'b001: begin
						opNmid		= JX2_UCMD_SHAD3;
						opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
					end

					3'b010: begin
						opUCmdIx = JX2_UCIX_ALU_SLTSL;
					end
					3'b011: begin
						opUCmdIx = JX2_UCIX_ALU_SLTUL;
					end

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

//			if(istrWord[30:25]==6'h01)
			if(istrWord[29:25]==5'h01)
			begin
//				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opNmid		= JX2_UCMD_QMULDIV;

				case(istrWord[14:12])
					3'b000:
					begin
						opNmid		= JX2_UCMD_MUL3;
						opUCmdIx	= JX2_UCIX_MUL3_MUL3S;
					end
					3'b001: opUCmdIx = JX2_UCIX_QMUL_MULHSL;
					3'b010: begin
						opNmid		= JX2_UCMD_MUL3;
						opUCmdIx	= JX2_UCIX_MUL3_DMUL3S;
					end
					3'b011: begin
						opNmid		= JX2_UCMD_MUL3;
						opUCmdIx	= JX2_UCIX_MUL3_DMUL3U;
					end
					3'b100: opUCmdIx = JX2_UCIX_QMUL_DIVSL;
					3'b101: opUCmdIx = JX2_UCIX_QMUL_DIVUL;
					3'b110: opUCmdIx = JX2_UCIX_QMUL_MODSL;
					3'b111: opUCmdIx = JX2_UCIX_QMUL_MODUL;
				endcase
			end

			if(istrWord[29:25]==5'h04)
			begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;

				case(istrWord[14:12])
					3'b000: begin
						opUCmdIx = istrWord[30] ?
							JX2_UCIX_ALU_SUBUL : JX2_UCIX_ALU_ADDUL;
					end
					default: begin
						opNmid		= JX2_UCMD_INVOP;
					end
				endcase
			end

			if(istrWord[29:25]==5'h05)
			begin
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;

				case(istrWord[14:12])
					3'b000: begin
						opUCmdIx = istrWord[30] ?
							JX2_UCIX_ALU_SUBUL : JX2_UCIX_ALU_ADDUL;
					end
					default: begin
						opNmid		= JX2_UCMD_INVOP;
					end
				endcase
			end

		end

		5'b00_111: begin /* Extended Op, 48b */
//			opNmid		= JX2_UCMD_NOP;
//			opFmid		= JX2_FMID_Z;
//			opIty		= JX2_ITY_SB;

`ifdef jx2_enable_riscv_op48
			casez(istrWord[15:12])
				4'b0000: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UQ;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
				end

				4'b1000: begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UQ;
					opUCmdIx	= JX2_UCIX_LDI_LDISH32;
				end
				
				default: begin
				end
			endcase
`endif
		end

		5'b01_111: begin /* Extended Op, 64b */
			opNmid		= JX2_UCMD_NOP;
			opFmid		= JX2_FMID_Z;
			opIty		= JX2_ITY_SB;
		end

		5'b10_111: begin /* Extended Op, 48b */
			opNmid		= JX2_UCMD_NOP;
			opFmid		= JX2_FMID_Z;
			opIty		= JX2_ITY_SB;
		end

		5'b11_111: begin /* Extended Op, 80+b */
			opNmid		= JX2_UCMD_NOP;
			opFmid		= JX2_FMID_Z;
			opIty		= JX2_ITY_SB;
		end

		default: begin
		end
	endcase


//	if(opNmid == JX2_UCMD_INVOP)
//		opFmid = JX2_FMID_INV;

	if(opIsNotFx)
	begin
//		opFmid	= JX2_FMID_Z;
//		opIty	= JX2_ITY_SB;
	end

	if(opIsNotFx)
		opFmid = JX2_FMID_INV;

	opUCmd = { opCcty, opNmid };

	opUFl	= 0;
	opULdOp	= 0;
	opDoImm		= JX2_RVIMM_NONE;

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

`ifndef def_true
				JX2_ITY_UB: begin
					opRegM	= JX2_GR_DLR;
					opRegO	= JX2_GR_DLR;
					opRegN	= JX2_GR_DLR;
					opRegP	= JX2_GR_DLR;
				end
`endif

				default: begin
					$display("Jx2DecOpRvI: Bad Reg, Ity=%X", opIty);
					$display("Jx2DecOpRvI: Istr %X-%X-%X",
						istrWord[15:0], istrWord[31:16], istrWord[47:32]);
				end
			endcase
		end

`ifndef def_true
		JX2_FMID_REG: begin
			opUIxt	= { opUCty, opUCmdIx };
			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegO_Dfl;
					opRegN	= opRegO_Dfl;
					opRegP	= opRegO_Dfl;
				end

				JX2_ITY_SW: begin
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegM_Dfl;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
				end

				JX2_ITY_UB: begin
					opRegM	= opRegO_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegN	= opRegO_Dfl;
					opRegP	= opRegO_Dfl;
				end

				JX2_ITY_NB: begin
					opRegM	= opRegO_Cr;
					opRegO	= JX2_GR_ZZR;
					opRegN	= opRegO_Cr;
					opRegP	= opRegO_Cr;
					if(usrRejectCoW)
						usrReject = 1;
				end

				JX2_ITY_XB: begin
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegO_Dfl;
					opRegN	= JX2_GR_DLR;
					opRegP	= JX2_GR_DLR;
				end

				default: begin
					$display("Jx2DecOpRvI: Bad Reg, Ity=%X", opIty);
					$display("Jx2DecOpRvI: Istr %X-%X",
						istrWord[15:0], istrWord[31:16]);
				end
			endcase
		end
`endif

		/*
		RegReg
			SB: Rm, Ro|Imm17au, Rn
			SW: -
			SL: -
			SQ: / Ro, Rm|Imm17au, Rn
			SQ: Ro|Imm17au, Rm, Rn

			UB: Rm, Rn, Rn
			UW: Ro, Rn, Rn
			UL: Rm, Cn, Cn
			UQ: Cm, Cm, Rn

			NB: Rn, Rm, Rn
			NW: Fm, Fo, Fp, Fn
			NL: Fm, Fm, Rn
			NQ: Rm, Rm, Fn
			
			XB: Fm, Fo, Fn
			XW: Fm, Fn, Fn
			XL: Fo, Fm, Fn
			XQ: Fo, Fm, Rn
		*/

		JX2_FMID_REGREG: begin
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
//			opRegO	= opRegO_Dfl;
			opRegO	= opRegO_Df3R;

//			opImm	= opImm_imm5u;
//			opImm	= opImm_imm8au;
			opDoImm	= JX2_RVIMM_IMMAU;

			opUIxt	= { opUCty, opUCmdIx };

`ifdef jx2_reg_rp
			if(opNmid == JX2_UCMD_MOV_RM)
				opRegN	= JX2_GR_ZZR;
			if(opNmid == JX2_UCMD_MOV_MR)
				opRegP	= JX2_GR_ZZR;

			if(opNmid == JX2_UCMD_ALU3)
				opRegP	= JX2_GR_ZZR;
`endif

			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_SQ: begin
//					opRegM	= opRegO_Dfl;
//					opRegO	= opRegM_Df3R;

					opRegM	= opRegO_Df3R;
					opRegO	= opRegM_Dfl;
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;
				end

				JX2_ITY_UB: begin
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
//					opImm	= opImm_imm8au;
				end

				JX2_ITY_UW: begin
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;
//					opRegM	= opRegO_Dfl;
					opRegM	= opRegO_Df3R;
					opRegO	= opRegN_Dfl;
//					opImm	= opImm_imm8au;
				end

				JX2_ITY_UL: begin
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Cr;
					opRegN	= opRegN_Cr;
					opRegP	= opRegN_Cr;
					if(usrRejectCnW)
						usrReject = 1;
				end
				JX2_ITY_UQ: begin
					opRegM	= opRegM_Cr;
//					opRegO	= opRegN_Dfl;
					opRegO	= opRegM_Cr;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					if(usrRejectCmR)
						usrReject = 1;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
//					opImm	= opImm_imm8au;
				end

				JX2_ITY_NW: begin
					opRegM	= opRegM_Fr;
					opRegO	= opRegO_Fr;
					opRegP	= opRegP_Fr;
					opRegN	= opRegN_Fr;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				JX2_ITY_NL: begin
					opRegM	= opRegM_Fr;
					opRegO	= opRegM_Fr;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				JX2_ITY_NQ: begin
					opRegM	= opRegM_Dfl;
					opRegO	= opRegM_Dfl;
					opRegN	= opRegN_Fr;
					opRegP	= opRegN_Fr;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				JX2_ITY_XB: begin
					opRegM	= opRegM_Fr;
					opRegO	= opRegO_Fr;
					opRegN	= opRegN_Fr;
					opRegP	= opRegN_Fr;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				JX2_ITY_XW: begin
					opRegM	= opRegM_Fr;
					opRegO	= opRegN_Fr;
					opRegN	= opRegN_Fr;
					opRegP	= opRegN_Fr;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				JX2_ITY_XL: begin
					opRegM	= opRegO_Fr;
					opRegO	= opRegM_Fr;
					opRegN	= opRegN_Fr;
					opRegP	= opRegN_Fr;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				JX2_ITY_XQ: begin
					opRegM	= opRegO_Fr;
					opRegO	= opRegM_Fr;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_RVIMM_IMMFPRM;
				end

				default: begin
					$display("Jx2DecOpRvI: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end

		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= { opUCty, opUCmdIx };

`ifdef jx2_reg_rp
			if(opNmid == JX2_UCMD_ALU3)
				opRegP	= JX2_GR_ZZR;
`endif

			case(opIty)
				JX2_ITY_SB: begin
				end
				JX2_ITY_SW: begin
//					opImm	= opImm_imm12s;
					opDoImm	= JX2_RVIMM_IMM12S;
				end

//				JX2_ITY_UW: begin
//					opImm	= opImm_imm12u;
//				end

				JX2_ITY_NB: begin
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegP	= opRegO_Dfl;
					opRegN	= opRegN_Dfl;

					opULdOp	= amoLdOp;
				end

//				JX2_ITY_NW: begin
//					opImm	= opImm_imm12n;
//				end

				JX2_ITY_XB: begin
					opDoImm	= JX2_RVIMM_IMM12S;
					opRegN	= opRegN_Fr;
					opRegP	= opRegN_Fr;
					opRegM	= opRegM_Fr;
				end

				JX2_ITY_XQ: begin
					opRegM	= opRegM_Dfl;
					opRegO	= JX2_GR_IMM;
					opRegP	= opRegO_Dfl;
					opRegN	= opRegN_Dfl;
					opDoImm	= JX2_RVIMM_IMM21J;
				end
				
				default: begin
					$display("Jx2DecOpRvI: RegImmReg, Bad Ity=%X", opIty);
				end

			endcase
		end

`ifdef def_true
		/*
			SW: (Rm, Disp9u/17s), Rn
			UB: (Rm, Disp5u), Rn

			XB: (Rm, Disp9u), Fn
		 */
		JX2_FMID_LDREGDISPREG: begin
			if(opBty == JX2_BTY_UQ)
				opUCty		= JX2_IUC_WX;

			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty[2], 2'b00};
//			opImm	= opImm_disp12ld;
			opDoImm	= JX2_RVIMM_DISP12LD;

			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;

			if(opIty == JX2_ITY_XB)
			begin
				opRegN	= opRegN_Fr;
				opRegP	= opRegN_Fr;
			end

`ifdef jx2_reg_rp
			opRegP	= JX2_GR_ZZR;
`endif
		end

		/*
			SW: Ro, (Rm, Disp9u/17s)
			UB: Ro, (Rm, Disp5u)

			XB: Fo, (Rm, Disp9u)
		 */
		JX2_FMID_REGSTREGDISP: begin
			if(opBty == JX2_BTY_UQ)
				opUCty		= JX2_IUC_WX;

			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty[2], 2'b00};
//			opImm	= opImm_disp12st;
			opDoImm	= JX2_RVIMM_DISP12ST;
	
			opRegN	= opRegO_Dfl;
			opRegP	= opRegO_Dfl;
			opRegM	= opRegM_Dfl;

			if(opIty == JX2_ITY_XB)
			begin
				opRegN	= opRegO_Fr;
				opRegP	= opRegO_Fr;
			end

`ifdef jx2_reg_rp
			opRegN	= JX2_GR_ZZR;
`endif
		end
`endif

		/*
			SB: Rn, Imm20, Rn
			SW: Imm20, Rn, Rn
			SL: -
			SQ: Rn, Imm17s, Rn

			UB: ZR, Imm20, Rn
			UW: -
			UL: -
			UQ: Rn, Imm32lc, Rn

			NB: -
			NW: -
			NL: -
			NQ: -

			XB: -
			XW: -

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
//					opImm	= opImm_imm20s;
					opDoImm	= JX2_RVIMM_IMM20UI;
					opRegM	= opRegO_Df2;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

				JX2_ITY_SW: begin
					opDoImm	= JX2_RVIMM_IMM20UI;
					opRegM	= opRegImm16;
					opRegO	= opRegO_Df2;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

//				JX2_ITY_SW: begin
//					opImm	= opImm_imm20s;
//				end

				JX2_ITY_SQ: begin
//					opImm		= opImm_imm17s;
					opDoImm		= JX2_RVIMM_IMM17S;
					opRegM		= opRegN_Dfl;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
//					opIsImm9	= 1;
				end

				JX2_ITY_UB: begin
					opDoImm	= JX2_RVIMM_IMM20UI;
					opRegM	= JX2_GR_ZZR;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

//				JX2_ITY_UB: begin
//					opImm	= opImm_imm20u;
//					opRegM	= opRegO_Df2;
//					opRegO	= opRegImm16;
//					opRegN	= opRegO_Df2;
//					opRegP	= opRegO_Df2;
//				end

				JX2_ITY_UQ: begin
					opDoImm		= JX2_RVIMM_IMM32LC;
					opRegM		= opRegN_Dfl;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
				end

				default: begin
					$display("Jx2DecOpRvI: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
			
			opUFl[0]	= opIsImm9;
		end

//		JX2_FMID_IMM4ZREG: begin
//		end

//		JX2_FMID_IMM4NREG: begin
//		end
		
		/*
			SB: (PC, Ro), ZZR
			UB: Rm, Rn, Imm12b
		 */
		JX2_FMID_REGPC: begin
			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= JX2_GR_BPC;
					opRegO	= opRegO_Dfl;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;
					opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
				end

				JX2_ITY_UB: begin
					opRegM	= opRegM_Dfl;
					opRegO	= opRegO_Dfl;

`ifdef jx2_enable_riscv_immb
					if(opExWQ)
						opRegO	= JX2_GR_IMMB;
`endif

//					opImm	= opImm_disp12b;
					opDoImm	= JX2_RVIMM_DISP12B;
					opUIxt	= {opUCty, opUCmdIx};
				end
				
				default: begin
					$display("Jx2DecOpRvI: RegPc Bad Ity=%X", opIty);
				end
			endcase
		end

		/*
			SB: Fzdd_xxdd		(PC, disp20ui), Rn
			SW: Fzdd_xddd		(PC, disp20j), Rn

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
//				opImm	= opImm_imm20s;
				opDoImm	= JX2_RVIMM_IMM20UI;
			end
			JX2_ITY_SW: begin
//				opImm	= opImm_disp20s;
				opDoImm	= JX2_RVIMM_DISP20J;
//				opRegM	= JX2_GR_BPC;	/* JAL Quirk */
			end

//			JX2_ITY_UB: begin
//				opImm	= opImm_imm20u;
//			end

//			JX2_ITY_UW: begin
//				opImm	= opImm_disp12st;
//				opDoImm	= JX2_RVIMM_DISP12ST;
//			end

			default: begin
				$display("Jx2DecOpRvI: PcDisp, Bad Ity=%X", opIty);
			end

			endcase
		end

		JX2_FMID_INV: begin
			if(!opIsNotFx && !isAltOp)
			begin
				opUCmd = { opCcty, JX2_UCMD_INVOP };
				if(!tMsgLatch && isOpRiscV)
				begin
					$display("Jx2DecOpRvI: Invalid FMID (32)");
					$display("Jx2DecOpRvI: Istr %X-%X Mod=%X",
						istrWord[15:0], istrWord[31:16], srMod);
				end
				tNextMsgLatch=1;
			end
		end
		
		default: begin
			opUCmd = { opCcty, JX2_UCMD_INVOP };
			if(!tMsgLatch && !isAltOp)
			begin
				$display("Jx2DecOpRvI: Unhandled FMID (32) %X", opFmid);
				$display("Jx2DecOpRvI: Istr %X-%X",
					istrWord[15:0], istrWord[31:16]);
			end
			tNextMsgLatch=1;
		end
	endcase

	case(opDoImm)
		JX2_RVIMM_NONE:		begin end
		JX2_RVIMM_IMMAU:	opImm	= opImm_imm8au;
		JX2_RVIMM_IMM20UI:	opImm	= opImm_imm20s;
		JX2_RVIMM_DISP20J:	opImm	= opImm_disp20s;
		JX2_RVIMM_DISP12LD:	opImm	= opImm_disp12ld;
		JX2_RVIMM_DISP12ST: opImm	= opImm_disp12st;
		JX2_RVIMM_DISP12B:	opImm	= opImm_disp12b;
		JX2_RVIMM_IMM17S:	opImm	= opImm_imm17s;
//		JX2_RVIMM_IMM12S:	opImm	= opImm_imm12s;
		JX2_RVIMM_IMM32LC:	opImm	= opImm_imm32lc;
		JX2_RVIMM_IMM21J:	opImm	= opImm_imm21j;
		JX2_RVIMM_IMMFPRM:	opImm	= opImm_immfprm;
		default:			opImm	= 0;
	endcase

//	if(opExWQ)
	if(1'b1)
	begin
		opImmB	= opImm_imm8au[16:0];
	end

	usrSuAllowEn = 0;
	case(usrSuAllow)
		2'b00: usrSuAllowEn = 0;					//Supervisor Only
		2'b01: usrSuAllowEn = srSuperuser;			//Superuser
		2'b10: usrSuAllowEn = srMod[1];				//Secure Execute
		2'b11: usrSuAllowEn = srMod[1] && srMod[2];	//Superuser+Secure Exe
	endcase

//	if(usrReject && srUser && !(usrSuAllow && srSuperuser))
//	if(usrReject && srUser && !usrSuAllowEn)
	if(usrReject && srUser && !usrSuAllowEn && !opIsNotFx && !isAltOp)
	begin
		$display("DecOpRvI: Usermode Reject %X-%X",
			istrWord[15:0], istrWord[31:16]);
		opNmid	= JX2_UCMD_INVOP;
		opFmid	= JX2_FMID_INV;
	end
	
	if(isOpRiscV && !opIsNotFx && !isAltOp)
	begin
		if(opNmid == JX2_UCMD_INVOP)
		begin
			$display("DecOpRvI: Saw INVOP %X-%X",
				istrWord[15:0], istrWord[31:16]);
		end
	end

`ifndef def_true
// `ifdef def_true
//	if(opIsJumboAu)
	if(opIsJumboAu && (opNmid == JX2_UCMD_ALU3))
	begin
		if(!tMsgLatch)
		begin
			$display(
			"DecOpRvI: Dbg-JA %X.%X jb=%X  %X-%X m=%X o=%X p=%X n=%X i=%X",
				istrWord[31:16], istrWord[15:0], istrJBits,
				opUCmd, opUIxt,
				opRegM, opRegO, opRegP, opRegN,
				opImm);
		end
		tNextMsgLatch=1;
	end
	else
//		if(opIsJumbo)
		if(opIsJumbo && (opNmid == JX2_UCMD_ALU3))
	begin
		if(!tMsgLatch)
		begin
			$display(
			"DecOpRvI: Dbg-J  %X.%X jb=%X  %X-%X m=%X o=%X p=%X n=%X i=%X",
				istrWord[31:16], istrWord[15:0], istrJBits,
				opUCmd, opUIxt,
				opRegM, opRegO, opRegP, opRegN,
				opImm);
		end
		tNextMsgLatch=1;
	end
`endif
end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule
