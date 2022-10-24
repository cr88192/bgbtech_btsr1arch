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
Redo to use new F0 block, and merge F0/F1/F2.
 */

module DecOpFz(
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
// input			srUser;		//usermode
input[2:0]		srMod;		//mode

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
output[7:0]		idUFl;

parameter		fpuLowPrec = 0;

wire			isAltOp;
wire			isAltOpC;
wire			isOp24;
assign		isAltOp		= isAltOpB[0];
assign		isOp24		= isAltOpB[1];
assign		isAltOpC	= isAltOpB[0] && isAltOpB[2];

wire			srUser;				//Usermode
wire			srSuperuser;		//Superuser mode
assign		srUser = srMod[0];
assign		srSuperuser = (srMod[0] && srMod[1]) || (srMod[0] && srMod[2]);

`reg_gpr		opRegN;
`reg_gpr		opRegM;
`reg_gpr		opRegO;
`reg_gpr		opRegP;
`reg_gpr		opRegImm16;
`reg_gpr		opRegImm10;
reg[32:0]		opImm;
reg[8:0]		opUCmd;
reg[8:0]		opUIxt;
reg[3:0]		opUFl;			//[0]=Imm16+24, [1]=SplitImm, [2]=FpImm
reg[3:0]		opULdOp;		//Operation for Load/Store, etc

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idRegP = opRegP;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;
// assign	idUFl = opUFl;
assign	idUFl = { opULdOp, opUFl };

`reg_gpr	opRegM_Dfl;
`reg_gpr	opRegO_Dfl;
`reg_gpr	opRegN_Dfl;
`reg_gpr	opRegP_Dfl;

`reg_gpr	opRegM_OrgDfl;
`reg_gpr	opRegN_OrgDfl;

`reg_gpr	opRegM_Fix;
`reg_gpr	opRegO_Fix;
`reg_gpr	opRegN_Fix;

`reg_gpr	opRegO_Df2;
reg			opRegO_Df2_IsSP;

`reg_gpr	opRegM_Cr;
`reg_gpr	opRegN_Cr;
`reg_gpr	opRegO_Cr;

`reg_gpr	opRegM_Sr;
`reg_gpr	opRegN_Sr;

reg[32:0]		opImm_imm9s;
reg[32:0]		opImm_imm9u;
reg[32:0]		opImm_imm9n;

reg[32:0]		opImm_imm10s;
reg[32:0]		opImm_imm10u;
reg[32:0]		opImm_imm10n;

reg[32:0]		opImm_imm16s;
reg[32:0]		opImm_imm16u;
reg[32:0]		opImm_imm16n;

reg[32:0]		opImm_disp20s;
reg[32:0]		opImm_disp8s;
reg[32:0]		opImm_disp9s;
reg[32:0]		opImm_disp13s;

reg[32:0]		opImm_disp5u;

reg[32:0]		opImm_imm5u;
reg[32:0]		opImm_imm5n;

reg[32:0]		opImmRm_imm6u;
reg[32:0]		opImmRm_imm6n;

reg[32:0]		opImm_imm8au;
reg[32:0]		opImm_imm8as;

reg[32:0]		opImm_disp11as;
// reg[32:0]		opImm_disp9as;
reg[2:0]		opImm_dispasc;

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
reg		opIsImm4R;
reg		opIsImmSplit;

reg[3:0]	opIsImmLdOp;

reg			opExWQ;
reg			opExWN;
reg			opExWM;
reg			opExWI;
// reg			opExWIS;
reg[3:0]	opIsExWB2;

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

reg	tMsgLatch;
reg	tNextMsgLatch;

reg tBlockIsF0;
reg tBlockIsF1;
reg tBlockIsF2;
reg tBlockIsF3;

reg tBlockIsF4;
reg tBlockIsF5;
reg tBlockIsF6;
reg tBlockIsF7;
reg tBlockIsWex;
reg	tBlockIsPrWxA;
reg	tBlockIsPrWxB;

reg tBlockIsF8;
reg tBlockIsF9;
reg tBlockIsFA;
reg tBlockIsFB;

reg tBlockIsFC;
reg tBlockIsFD;
reg tBlockIsFE;
reg tBlockIsFF;

reg tBlockIsEz;
reg tBlockIsEA_F0;
reg tBlockIsEA_F2;
reg tBlockIsEA_09;

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

	tRegRoIsRs	= (istrWord[23:21]==3'b000) || (istrWord[23:20]==4'b1111);
	tRegRnIsRs	= (istrWord[ 7: 5]==3'b000) || (istrWord[ 7: 4]==4'b1111);
	tRegRmIsRs	= (istrWord[ 3: 1]==3'b000) || (istrWord[ 3: 0]==4'b1111);

//	tRegRoIsRs	= (istrWord[23:21]==3'b000) | (istrWord[23:20]==4'b1111);
//	tRegRnIsRs	= (istrWord[ 7: 5]==3'b000) | (istrWord[ 7: 4]==4'b1111);
//	tRegRmIsRs	= (istrWord[ 3: 1]==3'b000) | (istrWord[ 3: 0]==4'b1111);

	opExQ		= istrWord[27];
	opExN		= istrWord[26];
	opExM		= istrWord[25];
	opExI		= istrWord[24];

	opIsXGpr	= 0;
	opExWQ		= 0;
	opExWN		= 0;
	opExWM		= 0;
	opExWI		= 0;
//	opExWIS		= 0;
	opIsImm4R	= 0;
	opIsImmLdOp	= 0;
	opIsExWB2	= 0;

	tOpIsXGprX0	= 0;
	tOpIsXGprX1	= 0;
	tOpIsXGprX2	= 0;

//	opIsJumbo	= istrJBits[22];
	opIsJumbo	= istrJBits[24];
//	opIsJumbo96	= istrJBits[25] && opIsJumbo;
	opIsJumbo96	= istrJBits[25];
//	opIsJumboAu	= istrJBits[26];
	opIsJumboAu	= istrJBits[26] && opIsJumbo;

`ifdef jx2_enable_xgpr

	opIsXGpr	=
		(istrWord[15:12]==4'b0111) ||
		(istrWord[15:12]==4'b1001);

	tOpIsXGprX0	= (istrWord[15:12]==4'b0111);
	tOpIsXGprX1	= (istrWord[15:12]==4'b1001) &&  istrWord[ 8];
	tOpIsXGprX2	= (istrWord[15:12]==4'b1001) && !istrWord[ 8];

	opExWN		= istrWord[10] && opIsXGpr;
	opExWM		= istrWord[ 9] && opIsXGpr;
//	opExWI		= istrWord[ 8] && opIsXGpr;
	opExWI		= istrWord[ 8] && tOpIsXGprX0;
//	opExWI		= istrWord[ 8] && opIsXGpr && (istrWord[15:12]!=4'b1001);

//	opExWIS		= opExWI;
//	if(istrWord[15:12]==4'b1001)
//		opExWI		= 0;

	if(opIsJumboAu)
	begin
		opExWQ		= istrJBits[23];
		opExWN		= istrJBits[22];
		opExWM		= istrJBits[21];
		opExWI		= istrJBits[20];
		opIsExWB2	= istrJBits[19:16];
	end

//	opExWN		= 0;
//	opExWM		= 0;
//	opExWI		= 0;

//	if(opExWN)
//		tRegRnIsRs = 0;
//	if(opExWM)
//		tRegRmIsRs = 0;
//	if(opExWI)
//		tRegRoIsRs = 0;

//	opRegN_Dfl	= {tRegRnIsRs && (!opExN), opExWN, opExN, istrWord[ 7: 4]};
//	opRegM_Dfl	= {tRegRmIsRs && (!opExM), opExWM, opExM, istrWord[ 3: 0]};
//	opRegO_Dfl	= {tRegRoIsRs && (!opExI), opExWI, opExI, istrWord[23:20]};

	opRegN_Dfl	= {tRegRnIsRs && (!opExN) && (!opExWN),
		opExWN, opExN, istrWord[ 7: 4]};
	opRegM_Dfl	= {tRegRmIsRs && (!opExM) && (!opExWM),
		opExWM, opExM, istrWord[ 3: 0]};
	opRegO_Dfl	= {tRegRoIsRs && (!opExI) && (!opExWI),
		opExWI, opExI, istrWord[23:20]};
	opRegP_Dfl	= opRegN_Dfl;

	opRegO_Df2	= {tRegRmIsRs && !(istrWord[4]) && !opExWN,
		1'b0, istrWord[4], istrWord[3:0]};

	if(opIsJumbo96 && istrWord[5])
		opRegO_Df2[6:5]	= 2'b01;

//	if(opIsJumbo96)
//		opRegO_Df2	= { 1'b0, istrWord[5:0] };

//	opRegN_Sr	= {1'b1, opExWN, opExN, istrWord[ 7: 4]};
//	opRegM_Sr	= {1'b1, opExWM, opExM, istrWord[ 3: 0]};

	opRegN_Sr	= {2'b10, opExN, istrWord[ 7: 4]};
	opRegM_Sr	= {2'b10, opExM, istrWord[ 3: 0]};

//	opRegN_Cr	= {2'b11, opExN, istrWord[ 7: 4]};
//	opRegM_Cr	= {2'b11, opExM, istrWord[ 3: 0]};
//	opRegO_Cr	= {2'b11, opExI, istrWord[23:20]};

	opRegN_Cr	= {1'b1, !opExWN, opExN, istrWord[ 7: 4]};
	opRegM_Cr	= {1'b1, !opExWM, opExM, istrWord[ 3: 0]};
	opRegO_Cr	= {1'b1, !opExWI, opExI, istrWord[23:20]};

`else

`ifdef jx2_sprs_elrehr
	opRegN_Dfl	= {tRegRnIsRs, opExN, istrWord[ 7: 4]};
	opRegM_Dfl	= {tRegRmIsRs, opExM, istrWord[ 3: 0]};
	opRegO_Dfl	= {tRegRoIsRs, opExI, istrWord[23:20]};

	opRegO_Df2	= {tRegRmIsRs, istrWord[ 4], istrWord[3:0]};
`else
	opRegN_Dfl	= {tRegRnIsRs && (!opExN), opExN, istrWord[ 7: 4]};
	opRegM_Dfl	= {tRegRmIsRs && (!opExM), opExM, istrWord[ 3: 0]};
	opRegO_Dfl	= {tRegRoIsRs && (!opExI), opExI, istrWord[23:20]};
	opRegO_Df2	= {tRegRmIsRs && !(istrWord[ 4]), istrWord[ 4], istrWord[3:0]};
`endif

	opRegN_Sr	= {1'b1, opExN, istrWord[ 7: 4]};
	opRegM_Sr	= {1'b1, opExM, istrWord[ 3: 0]};

	opRegN_Cr	= {1'b1, !opExN, istrWord[ 7: 4]};
	opRegM_Cr	= {1'b1, !opExM, istrWord[ 3: 0]};
	opRegO_Cr	= {1'b1, !opExI, istrWord[23:20]};

`endif

`ifdef jx2_enable_riscv
	if(opRegM_Cr[4:0]==5'h0F)
		opRegM_Cr = JX2_GR_R15;
	if(opRegN_Cr[4:0]==5'h0F)
		opRegN_Cr = JX2_GR_R15;
`endif

	opRegO_Df2_IsSP = (opRegO_Df2 == JX2_GR_SP);


`ifdef jx2_enable_xgpr
	opImm_imm5u	= {UV27_00, opRegO_Dfl[5:0]};
	opImm_imm5n	= {UV27_FF, opRegO_Dfl[5:0]};

//	opImmRm_imm6u	= {UV27_00, opRegM_Dfl[5:0]};
//	opImmRm_imm6n	= {UV27_FF, opRegM_Dfl[5:0]};

//	opImm_disp5u	= opImm_imm5u;
	opImm_disp5u	= {opExWI ? UV27_FF : UV27_00, opRegO_Dfl[5:0]};
	
	opImm_imm5u		= opImm_disp5u;
`else
	opImm_imm5u	= {UV28_00, opRegO_Dfl[4:0]};
	opImm_imm5n	= {UV28_FF, opRegO_Dfl[4:0]};
	opImm_disp5u	= opImm_imm5u;

//	opImmRm_imm6u	= {UV28_00, opRegM_Dfl[4:0]};
//	opImmRm_imm6n	= {UV28_FF, opRegM_Dfl[4:0]};
`endif

	opImmRm_imm6u	= {UV27_00, istrWord[25:24], istrWord[ 3: 0]};
	opImmRm_imm6n	= {UV27_FF, istrWord[25:24], istrWord[ 3: 0]};

	opIsNotFx	= (istrWord[15:13]!=3'b111);
	
	tNextMsgLatch	= 0;
	opImm_imm8au	= 0;
	opImm_imm8as	= opImm_imm5u;

	opImm_disp11as	= 0;
//	opImm_disp9as	= 0;
	opImm_dispasc	= 0;

	if(opIsJumboAu)
	begin
//		opRegO_Df2[5]	= opExWN;

		if(opExWN)
		begin
			opRegO_Df2[6:5]	= 2'b01;
		end

		opRegImm16 = JX2_GR_IMM;
		opRegImm10 = opIsJumbo96 ? JX2_GR_JIMM : JX2_GR_IMM;
	
//		opImm_disp20s	= { istrJBits[11], istrJBits[11:0],
//			istrWord[7:0], istrWord[27:16] };
		opImm_disp20s	= { opExWI, istrJBits[11:0],
			istrWord[7:0], istrWord[27:16] };

//		opImm_imm9s		= { opExI ? UV17_FF : UV17_00,
//			istrJBits[7:0], istrWord [23:16] };
//		opImm_imm9u		= opImm_imm9s;
//		opImm_imm9n		= opImm_imm9s;
//		opImm_imm9u		= { UV16_00, opExI, istrJBits[7:0], istrWord [23:16] };

		opImm_imm9s		= { opExWI ? UV16_FF : UV16_00,
			opExI, istrJBits[7:0], istrWord [23:16] };
		opImm_imm9u		= opImm_imm9s;
//		opImm_imm9n		= opImm_imm9s;
		opImm_imm9n		= { UV16_FF, opExI, istrJBits[7:0], istrWord [23:16] };

//		opImm_imm10s	= opImm_imm9s;
//		opImm_imm10u	= opImm_imm9u;
//		opImm_imm10n	= opImm_imm9n;

		opImm_imm10u	= { opExWI ? UV15_FF : UV15_00,
			opExM, opExI, istrJBits[7:0], istrWord [23:16] };
		opImm_imm10n	= { UV15_FF,
			opExM, opExI, istrJBits[7:0], istrWord [23:16] };
//		opImm_imm10n	= opImm_imm10u;
		opImm_imm10s	= opImm_imm10u;

		opImm_disp9s	= opImm_imm9s;

		opImm_disp8s	= { istrJBits[7] ? UV17_FF : UV17_00,
			istrJBits[7:0], istrWord[23:16] };
//		opImm_disp5u	= {UV19_00,
//			opExWI, opExI, istrJBits[7:0], istrWord[23:20]};

		opIsImm4R	= istrJBits[11];
		opIsImmLdOp	= istrJBits[15:12];

//		opImm_disp5u	= {opExWI ? UV20_FF : UV20_00,
//			opExI, istrJBits[7:0], istrWord[23:20]};

		opImm_disp5u	= {opExWI ? UV17_FF : UV17_00,
			opExI, istrJBits[10:0], istrWord[23:20]};

//		opImm_disp11as	= {
//			istrJBits[10] ? UV22_FF : UV22_00, 
//			istrJBits[10:0] };

		opImm_disp11as	= { UV22_00, istrJBits[10:0] };

		opImm_imm5u		= opImm_disp5u;
		opImm_imm5n		= opImm_disp5u;

		if(opIsImm4R)
		begin
			opImm_imm5u	= {
				istrJBits[10] ? UV22_FF : UV22_00, 
				istrJBits[10:0] };
			opImm_imm5n		= opImm_imm5u;

//			opImm_disp11as	= {
//				istrJBits[10] ? UV24_FF : UV24_00, 
//				istrJBits[8:0] };
//			opImm_dispasc = { 1'b1, istrJBits[10:9] };

			opImm_disp11as	= {	UV24_00, istrJBits[8:0] };
			opImm_dispasc = { 1'b1, istrJBits[10:9] };

//			opImm_disp5u	= {
//				istrJBits[8] ? UV24_FF : UV24_FF, 
//				istrJBits[8:0] };
		end

		opImm_disp13s   = { opExI, istrJBits[19:0],
			istrWord[7:0], istrWord[23:20] };

		opImm_imm8au	= { UV25_00, istrJBits[7:0] };
//		opImm_imm8as	= { istrJBits[7] ? UV25_FF : UV25_FF, istrJBits[7:0] };
		opRegP_Dfl		= { 1'b0, istrJBits[5:0] };

		opImm_imm16s	= { opExWI, istrJBits[15:0], istrWord[31:16] };
		opImm_imm16u	= opImm_imm16s;
		opImm_imm16n	= opImm_imm16s;

//		opImm_imm16u	= { 1'b0, istrJBits[15:0], istrWord[31:16] };
//		opImm_imm16n	= { 1'b1, istrJBits[15:0], istrWord[31:16] };
//		opImm_imm16s	= { istrJBits[15], istrJBits[15:0], istrWord[31:16] };
	end
	else
		if(opIsJumbo)
	begin
		opRegImm16 = JX2_GR_JIMM;
//		opRegImm10 = JX2_GR_IMM;
//		opRegImm10 = opIsJumbo96 ? JX2_GR_JIMM56 : JX2_GR_IMM;
		opRegImm10 = opIsJumbo96 ? JX2_GR_JIMM : JX2_GR_IMM;
	
		opImm_disp20s	= { istrJBits[11], istrJBits[11:0],
			istrWord[7:0], istrWord[27:16] };

//		opImm_imm9u	= { 2'b00, istrJBits[21:0], istrWord[24:16] };
//		opImm_imm9n	= { 2'b11, istrJBits[21:0], istrWord[24:16] };
//		opImm_imm9s	= {
//			istrJBits[  21], istrJBits[   21],
//			istrJBits[21:0], istrWord [24:16] };

//		opImm_imm9u	= { 1'b0, istrJBits[23:0], istrWord[23:16] };
//		opImm_imm9n	= { 1'b1, istrJBits[23:0], istrWord[23:16] };
//		opImm_imm9s	= { istrJBits[  23],
//			istrJBits[23:0], istrWord [23:16] };

		opImm_imm9s		= { opExI, istrJBits[23:0], istrWord [23:16] };
		opImm_imm9u		= opImm_imm9s;
		opImm_imm9n		= opImm_imm9s;
		opImm_imm10s	= opImm_imm9s;
		opImm_imm10u	= opImm_imm9u;
		opImm_imm10n	= opImm_imm9n;

//		opImm_imm9s	= { istrJBits[23:0], istrWord[23:16] };
//		opImm_imm9u = opImm_imm9u;
//		opImm_imm9n = opImm_imm9n;

		opImm_disp9s	= opImm_imm9s;

//		opImm_imm10u	= { 1'b0, istrJBits[21:0], istrWord[25:16] };
//		opImm_imm10n	= { 1'b1, istrJBits[21:0], istrWord[25:16] };
//		opImm_imm10s	= { istrJBits[21],
//			istrJBits[21:0], istrWord[25:16] };

//		opImm_imm10u	= { 1'b0, istrJBits[23:0], istrWord[23:16] };
//		opImm_imm10n	= { 1'b1, istrJBits[23:0], istrWord[23:16] };
//		opImm_imm10s	= { istrJBits[23],
//			istrJBits[23:0], istrWord[23:16] };

//		opImm_imm10s	= { opExI, istrJBits[23:0], istrWord[23:16] };
//		opImm_imm10u	= opImm_imm10s;
//		opImm_imm10n	= opImm_imm10s;

//		opImm_disp8s	= { istrJBits[21]? 3'b111 : 3'b000,
//			istrJBits[21:0], istrWord[7:0] };
//		opImm_disp5u	= {istrJBits[21]? 6'b111111 : 6'b000000,
//			istrJBits[21:0], opExI, istrWord[23:20]};

//		opImm_disp8s	= { istrJBits[23], istrJBits[23:0], istrWord[7:0] };
		opImm_disp8s	= { istrJBits[23], istrJBits[23:0], istrWord[23:16] };
		opImm_disp5u	= {opExI ? 5'b11111 : 5'b00000,
			istrJBits[23:0], istrWord[23:20]};

		opImm_imm5u		= opImm_disp5u;
		opImm_imm5n		= opImm_disp5u;

		opImm_disp13s   = { opExI, istrJBits[19:0],
			istrWord[7:0], istrWord[23:20] };

		opImm_imm16u	= { 1'b0, istrJBits[15:0], istrWord[31:16] };
		opImm_imm16n	= { 1'b1, istrJBits[15:0], istrWord[31:16] };
		opImm_imm16s	= { istrJBits[15], istrJBits[15:0], istrWord[31:16] };
	end
	else
	begin
		opRegImm16 = JX2_GR_IMM;
		opRegImm10 = JX2_GR_IMM;

		opImm_disp20s = { istrWord[7] ? UV13_FF : UV13_00,
			istrWord[7:0], istrWord[27:16] };

		opImm_imm9u	= {UV24_00, istrWord[24:16]};
		opImm_imm9n	= {UV24_FF, istrWord[24:16]};
		opImm_imm9s	= opImm_imm9u;
		opImm_disp9s	= opImm_imm9u;

		opImm_imm10u	= {UV23_00, istrWord[25:16]};
		opImm_imm10n	= {UV23_FF, istrWord[25:16]};
		opImm_imm10s	= {istrWord[25]?UV23_FF:UV23_00, istrWord[25:16]};

//		opImm_disp8s	= {istrWord[7]?UV25_FF:UV25_00, istrWord[7:0]};
		opImm_disp8s	= {istrWord[23]?UV25_FF:UV25_00, istrWord[23:16]};

		opImm_disp13s   = { opExI ? UV21_FF : UV21_00,
			istrWord[7:0], istrWord[23:20] };

//		opImm_disp5u	= {UV28_00, opExI, istrWord[23:20]};
//		opImm_disp5u	= {UV27_00, opExWI, opExI, istrWord[23:20]};
//		opImm_imm5u		= opImm_disp5u;

		opImm_imm16u	= { UV17_00, istrWord[31:16] };
		opImm_imm16n	= { UV17_FF, istrWord[31:16] };
		opImm_imm16s	= {
			istrWord[31] ? UV17_FF : UV17_00,
			istrWord[31:16] };
	end

`ifdef jx2_enable_xgpr
	tRegRnIsRz	= (opRegN_Dfl[5:1]==5'b00000);
	tRegRmIsRz	= (opRegM_Dfl[5:1]==5'b00000);
	tRegRoIsRz	= (opRegO_Dfl[5:1]==5'b00000);
`else
	tRegRnIsRz	= (opRegN_Dfl[4:1]==4'b0000);
	tRegRmIsRz	= (opRegM_Dfl[4:1]==4'b0000);
	tRegRoIsRz	= (opRegO_Dfl[4:1]==4'b0000);
`endif

	tRegRnIsR0	= tRegRnIsRz && !opRegN_Dfl[0];
	tRegRnIsR1	= tRegRnIsRz &&  opRegN_Dfl[0];
	tRegRmIsR0	= tRegRmIsRz && !opRegM_Dfl[0];
	tRegRmIsR1	= tRegRmIsRz &&  opRegM_Dfl[0];
	tRegRoIsR0	= tRegRoIsRz && !opRegO_Dfl[0];
	tRegRoIsR1	= tRegRoIsRz &&  opRegO_Dfl[0];

//	tRegRnIsR0	= tRegRnIsRz & !opRegN_Dfl[0];
//	tRegRnIsR1	= tRegRnIsRz &  opRegN_Dfl[0];
//	tRegRmIsR0	= tRegRmIsRz & !opRegM_Dfl[0];
//	tRegRmIsR1	= tRegRmIsRz &  opRegM_Dfl[0];
//	tRegRoIsR0	= tRegRoIsRz & !opRegO_Dfl[0];
//	tRegRoIsR1	= tRegRoIsRz &  opRegO_Dfl[0];

	usrReject		= 0;
	usrSuAllow		= 0;

`ifdef jx2_enable_xgpr
	usrRejectCmR	= usrRejectCmMask[opRegM_Cr[4:0]];
	usrRejectCmW	= usrRejectCnMask[opRegM_Cr[4:0]];
	usrRejectCoR	= usrRejectCmMask[opRegO_Cr[4:0]];
	usrRejectCoW	= usrRejectCnMask[opRegO_Cr[4:0]];
	usrRejectCnR	= usrRejectCmMask[opRegN_Cr[4:0]];
	usrRejectCnW	= usrRejectCnMask[opRegN_Cr[4:0]];
`else
	usrRejectCmR	= usrRejectCmMask[opRegM_Cr[3:0]];
	usrRejectCmW	= usrRejectCnMask[opRegM_Cr[3:0]];
	usrRejectCoR	= usrRejectCmMask[opRegO_Cr[3:0]];
	usrRejectCoW	= usrRejectCnMask[opRegO_Cr[3:0]];
	usrRejectCnR	= usrRejectCmMask[opRegN_Cr[3:0]];
	usrRejectCnW	= usrRejectCnMask[opRegN_Cr[3:0]];
`endif


	opRegM_OrgDfl	= opRegM_Dfl;
	opRegN_OrgDfl	= opRegN_Dfl;

`ifdef jx2_reg_spdecswap
	if(srMod[2])
	begin
		if(opRegM_Dfl == JX2_GR_SP)
			opRegM_Dfl = JX2_GR_SSP;
		if(opRegO_Dfl == JX2_GR_SP)
			opRegO_Dfl = JX2_GR_SSP;
		if(opRegN_Dfl == JX2_GR_SP)
			opRegN_Dfl = JX2_GR_SSP;
		if(opRegO_Df2 == JX2_GR_SP)
			opRegO_Df2 = JX2_GR_SSP;
			
		if(opRegM_Cr == JX2_GR_SSP)
			opRegM_Cr = JX2_GR_SP;
		if(opRegN_Cr == JX2_GR_SSP)
			opRegN_Cr = JX2_GR_SP;
	end
`endif

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

	tBlockIsF0 =
		(istrWord[11:8] == 4'b0000) ||
		(istrWord[11:8] == 4'b0100);
	tBlockIsF1 =
		(istrWord[11:8] == 4'b0001) ||
		(istrWord[11:8] == 4'b0101);
	tBlockIsF2 =
		(istrWord[11:8] == 4'b0010) ||
		(istrWord[11:8] == 4'b0110);
	tBlockIsF3 =
		(istrWord[11:8] == 4'b0011) ||
		(istrWord[11:8] == 4'b0111);
//	tBlockIsF8 =
//		(istrWord[11:8] == 4'b1000) ||
//		(istrWord[11:8] == 4'b1001);
	tBlockIsF8 =
		(istrWord[11:8] == 4'b1000) ||
		(istrWord[11:8] == 4'b1100);

	tBlockIsF4 =	(istrWord[11:8] == 4'b0100);
//	tBlockIsF5 =	(istrWord[11:8] == 4'b0101);
//	tBlockIsF6 =	(istrWord[11:8] == 4'b0110);
//	tBlockIsF7 =	(istrWord[11:8] == 4'b0111);
//	tBlockIsF9 =	(istrWord[11:8] == 4'b1001);
	tBlockIsF9 =	(istrWord[11:8] == 4'b1100);
//	tBlockIsWex		=
//		tBlockIsF4 || tBlockIsF5 ||
//		tBlockIsF6 || tBlockIsF7 ||
//		tBlockIsF9;
	tBlockIsWex		= istrWord[10];

	tBlockIsFA =
		(istrWord[11:8] == 4'b1010);
	tBlockIsFB =
		(istrWord[11:8] == 4'b1011);

	tBlockIsFE =
		(istrWord[11:8] == 4'b1110);
	tBlockIsFF =
		(istrWord[11:8] == 4'b1111);


	if(opIsNotFx)
//	if(opIsNotFx || (opIsExWB2!=0))
	begin
		tBlockIsF0 = 0;
		tBlockIsF1 = 0;
		tBlockIsF2 = 0;
		tBlockIsF3 = 0;
//		tBlockIsF4 = 0;
//		tBlockIsF5 = 0;
//		tBlockIsF6 = 0;
//		tBlockIsF7 = 0;
		tBlockIsF8 = 0;
		tBlockIsF9 = 0;
		tBlockIsFA = 0;
		tBlockIsFB = 0;

		tBlockIsFE = 0;
		tBlockIsFF = 0;
	end

	if(opIsExWB2!=0)
	begin
		tBlockIsF0 = 0;
		tBlockIsF1 = 0;
		tBlockIsF2 = 0;
		tBlockIsF3 = 0;
//		tBlockIsF4 = 0;
//		tBlockIsF5 = 0;
//		tBlockIsF6 = 0;
//		tBlockIsF7 = 0;
		tBlockIsF8 = 0;
		tBlockIsF9 = 0;
	end

	if(tOpIsXGprX0 && (istrWord[10:8]==0))
	begin
		tOpIsXGprX0 = 0;
		tBlockIsFF = 1;
	end

	if((tOpIsXGprX1 || tOpIsXGprX2) && (istrWord[10:9]==0))
	begin
		tOpIsXGprX1 = 0;
		tOpIsXGprX2 = 0;
		tBlockIsFF = 1;
	end

	if(tOpIsXGprX0)
		tBlockIsF0 = 1;
	if(tOpIsXGprX1)
		tBlockIsF1 = 1;
	if(tOpIsXGprX2)
		tBlockIsF2 = 1;

	if(opIsXGpr)
		tBlockIsWex		= istrWord[11];

	tBlockIsPrWxA =
		(istrWord[11:8] == 4'b1010) ||
		(istrWord[11:8] == 4'b1110);
	tBlockIsPrWxB =
		(istrWord[11:8] == 4'b1011) ||
		(istrWord[11:8] == 4'b1111);

//	if(opIsXGpr)
//	begin
//		tBlockIsPrWxA = 0;
//		tBlockIsPrWxB = 0;
//	end

	tBlockIsEz		= (istrWord[15:12] == 4'b1110);
//	tBlockIsEA_09	= !istrWord[31] || (istrWord[30:29]==0);
	tBlockIsEA_09	= !istrWord[31];
//	tBlockIsEA_F0	= tBlockIsEz && tBlockIsFA && tBlockIsEA_09;
//	tBlockIsEA_F2	= tBlockIsEz && tBlockIsFA && !tBlockIsEA_09;
//	tBlockIsEA_F0	= tBlockIsEz && tBlockIsPrWx && tBlockIsEA_09;
//	tBlockIsEA_F2	= tBlockIsEz && tBlockIsPrWx && !tBlockIsEA_09;

	tBlockIsEA_F0	= tBlockIsEz && tBlockIsPrWxA;
	tBlockIsEA_F2	= tBlockIsEz && tBlockIsPrWxB;

	if(tBlockIsEz)
	begin
		opCcty	= istrWord[10] ? JX2_IXC_CF : JX2_IXC_CT;
`ifdef jx2_enable_pred_s
		if(opIsJumboAu && istrJBits[23])
		begin
			opCcty	= istrWord[10] ? JX2_IXC_CFS : JX2_IXC_CTS;
		end
`endif
	end

	if(tBlockIsF0 || tBlockIsEA_F0)
	begin
		casez(istrWord[31:16])
			16'h0zz0: begin		/* F0nm_0eo0 */
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_NB;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end
			16'h0zz1: begin		/* F0nm_0eo1 */
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_NB;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end
			16'h0zz2: begin		/* F0nm_0eo2 */
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_NB;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end
			16'h0zz3: begin		/* F0nm_0eo3 */
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_NB;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end

			
			16'h0zz4: begin		/* F0nm_0eo4 */
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
			end
			16'h0zz5: begin		/* F0nm_0eo5 */
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
			end
			16'h0zz6: begin		/* F0nm_0eo6 */
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
			end
			16'h0zz7: begin		/* F0nm_0eo7 */
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTDRREG;
				end
			end

			16'h0zz8: begin		/* F0nm_0eo8 */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
			end
			16'h0zz9: begin		/* F0nm_0eo9 */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
			16'h0zzA: begin		/* F0nm_0eoA */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h0zzB: begin		/* F0nm_0eoB */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_SL : JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;

`ifdef jx2_agu_ldtex
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_FMOV_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
//					opBty	= JX2_BTY_SQ;
					opBty	= JX2_BTY_UQ;
				end
`endif

`ifdef jx2_enable_ops24
				if(opExQ && isOp24)
				begin
					opFmid	= JX2_FMID_PCDISP8;
					opBty	= JX2_BTY_SB;
					opIty	= JX2_ITY_UB;
					
					case({opExN, opExM})
						2'b00: opNmid = JX2_UCMD_BRA;
						2'b01: opNmid = JX2_UCMD_BSR;
						2'b10: begin
							opNmid = JX2_UCMD_BRA;
							opCcty	= JX2_IXC_CT;
						end
						2'b11: begin
							opNmid = JX2_UCMD_BRA;
							opCcty	= JX2_IXC_CF;
						end
					endcase
				end
`endif
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
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_ADD;

`ifdef jx2_enable_ops24
				if(isOp24)
					opIty		= JX2_ITY_SW;
`endif

				if(opExQ && !isOp24)
				begin
//					opUCmdIx	= JX2_UCIX_FPU_FSUBX;
					opUCty		= JX2_IUC_WX;
				end
			end
			16'h1zz1: begin		/* F0nm_1eo1 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_SUB;

`ifdef jx2_enable_ops24
				if(isOp24)
					opIty		= JX2_ITY_SW;
`endif

				if(opExQ && !isOp24)
				begin
//					opUCmdIx	= JX2_UCIX_FPU_FSUBX;
					opUCty		= JX2_IUC_WX;
				end
			end
			16'h1zz2: begin		/* F0nm_1eo2 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_MUL3_MUL3S;
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_MULS;
				end
`ifdef jx2_enable_ops24
				if(isOp24)
					opIty		= JX2_ITY_SW;
`endif
			end
			16'h1zz3: begin		/* F0nm_1eo3 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_MUL3_MUL3U;
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_MULU;
				end
`ifdef jx2_enable_ops24
				if(isOp24)
					opIty		= JX2_ITY_SW;
`endif
			end

			16'h1zz4: begin		/* F0nm_1eo4 */
				if(isOp24)
				begin
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_REGREG;
//					opIty		= JX2_ITY_SB;
					opIty		= JX2_ITY_SW;
					opUCmdIx	= JX2_UCIX_ALU_ADDSL;

					if(opExQ)
					begin
						opNmid		= JX2_UCMD_SHAD3;
						opUCmdIx	= JX2_UCIX_SHAD_SHAD3;
					end
				end
			end

			16'h1zz5: begin		/* F0nm_1eo5 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_AND;

				if(opExQ && !isOp24)
				begin
					opUCty		= JX2_IUC_WX;
				end

`ifdef jx2_enable_ops24
				if(opExQ && isOp24)
				begin
					opNmid		= JX2_UCMD_SHAD3;
					opUCmdIx	= JX2_UCIX_SHAD_SHAD3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SL;
				end
`endif
			end
			16'h1zz6: begin		/* F0nm_1eo6 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_OR;

				if(opExQ && !isOp24)
				begin
					opUCty		= JX2_IUC_WX;
				end

`ifdef jx2_enable_ops24
				if(opExQ && isOp24)
				begin
					opNmid		= JX2_UCMD_SHLD3;
					opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SW;
				end
`endif
			end
			16'h1zz7: begin		/* F0nm_1eo7 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_XOR;

				if(opExQ && !isOp24)
				begin
					opUCty		= JX2_IUC_WX;
				end

`ifdef jx2_enable_ops24
				if(opExQ && isOp24)
				begin
					opNmid		= JX2_UCMD_SHLD3;
					opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SL;
				end
`endif
			end

			16'h1zz8: begin	/* F0nm_1ez8 */
				case(istrWord[23:20])

					4'h6: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_VSKG;

						if(opExQ)
						begin
							opNmid		= JX2_UCMD_INVOP;
							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

					4'h7: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_VSKC;
						
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_INVOP;
							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

`ifdef jx2_enable_packbcd
					4'h8: begin		/* F0nm_1e88 */
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_BCDADD;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'h9: begin		/* F0nm_1e98 */
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_BCDSUB;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
`endif

					4'hA: begin		/* F0nm_1eA8 */
						opNmid		= JX2_UCMD_ALUW3;
						opUCmdIx	= JX2_UCIX_ALUW_MOVTA16;
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_SB;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end

					4'hB: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_SNIPEIC;
						else
							opUCmdIx	= JX2_UCIX_CONV_SNIPEDC;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UB;
					end

`ifdef jx2_alu_wx
					4'hC: begin
						opUCmdIx	= JX2_UCIX_ALU_CMPQEQ;
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
						else
						begin
							opNmid		= JX2_UCMD_ALUCMPB;
							opUCmdIx	= JX2_UCIX_ALUW_CMPAEQ;
						end
					end
					4'hD: begin
						opUCmdIx	= JX2_UCIX_ALU_CMPQHI;
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
						else
						begin
							opNmid		= JX2_UCMD_ALUCMPB;
							opUCmdIx	= JX2_UCIX_ALUW_CMPAHI;
						end
					end
					4'hE: begin
						opUCmdIx	= JX2_UCIX_ALU_CMPQGT;
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
						else
						begin
							opNmid		= JX2_UCMD_ALUCMPB;
							opUCmdIx	= JX2_UCIX_ALUW_CMPAHS;
						end
					end
`endif

					4'hF: begin
`ifdef def_true
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_CONV_STFXI;
						if(opExQ)
						begin
//							opUCty		= JX2_IUC_WX;
							opUCmdIx	= JX2_UCIX_CONV_STFLI;
						end
`endif
					end

					default: begin
					end
				endcase
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
`ifdef jx2_enable_pred_s
						if(opExI)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_TSTQ_S :
								JX2_UCIX_ALU_TST_S ;
						end
`endif
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
//						opIty	= JX2_ITY_SB;
						opIty		= JX2_ITY_NB;
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
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
					4'h9: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_CONV_MOV;
						opIty		= JX2_ITY_NW;

						if(opExQ)
						begin
							usrReject	= 0;
							opNmid		= JX2_UCMD_ALUCMPB;
							opFmid		= JX2_FMID_REGREG;
//							opIty		= JX2_ITY_NB;
							opIty		= JX2_ITY_XL;
							opUCmdIx	= JX2_UCIX_ALUW_CMPNATEQ;
						end

`ifndef def_true
						opNmid		= JX2_UCMD_MUL3;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_MUL3_MULS;
						opIty		= JX2_ITY_NB;
`endif
					end

					4'hA: begin
						opNmid	= JX2_UCMD_MOV_RC;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UL;
						usrReject	= 1;
						if(opExQ)
						begin
							opNmid	= JX2_UCMD_NOP;
						end
					end
					4'hB: begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
						usrReject	= 1;
						if(opExQ)
						begin
							usrReject	= 0;
//							opNmid	= JX2_UCMD_NOP;

							opNmid		= JX2_UCMD_ALUCMPB;
							opFmid		= JX2_FMID_REGREG;
//							opIty		= JX2_ITY_NB;
							opIty		= JX2_ITY_XL;
							opUCmdIx	= JX2_UCIX_ALUW_CMPTEQ;
						end
					end

					4'hC: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQEQ;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPEQ;
`ifdef jx2_enable_pred_s
						if(opExI)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQEQ_S :
								JX2_UCIX_ALU_CMPEQ_S ;
						end
`endif
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'hD: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQHI;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPHI;
`ifdef jx2_enable_pred_s
						if(opExI)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQHI_S :
								JX2_UCIX_ALU_CMPHI_S ;
						end
`endif
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'hE: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQGT;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPGT;
`ifdef jx2_enable_pred_s
						if(opExI)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQGT_S :
								JX2_UCIX_ALU_CMPGT_S ;
						end
`endif
						opNmid	= JX2_UCMD_ALUCMP;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
					end
					4'hF: begin
`ifndef def_true
						opNmid		= JX2_UCMD_MUL3;
						opFmid		= JX2_FMID_REGREG;
						opUCmdIx	= JX2_UCIX_MUL3_MULU;
						opIty		= JX2_ITY_NB;
`endif
					end
				endcase
			end

			16'h1zzA: begin	/* F0nm_1ezA */
				case(istrWord[23:20])

					4'hA: begin
						opNmid		= JX2_UCMD_FCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_FPU_PCMPEQ;
					end
					4'hB: begin
						opNmid		= JX2_UCMD_FCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_FPU_PCMPGT;
					end

					4'hC: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_PCMPEQ;
					end
					4'hD: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_PCMPHI;
					end
					4'hE: begin
						opNmid		= JX2_UCMD_ALUW3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_PCMPGT;
					end

					default: begin
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

//						opNmid		= JX2_UCMD_SHLLN;
//						opFmid		= JX2_FMID_REGREG;
//						opUCmdIx	= 6'h13;

						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_NOT;

						if(opExQ)
							opUCty		= JX2_IUC_WX;

					end
					4'h1: begin
						opNmid		= JX2_UCMD_ALU3;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SW;
						opUCmdIx	= JX2_UCIX_ALU_SUB;

						if(opExQ)
							opUCty		= JX2_IUC_WX;
					end

					4'h2: begin
						if(opExQ)
//							opUCmdIx	= JX2_UCIX_CONV_CLZQ;
							opUCmdIx	= JX2_UCIX_ALUN_CLZQ;
						else
//							opUCmdIx	= JX2_UCIX_CONV_CLZ;
							opUCmdIx	= JX2_UCIX_ALUN_CLZ;
//						opNmid	= JX2_UCMD_CONV_RR;
						opNmid	= JX2_UCMD_UNARY;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end
					4'h3: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALUN_CTZQ;
						else
							opUCmdIx	= JX2_UCIX_ALUN_CTZ;
						opNmid	= JX2_UCMD_UNARY;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end

					4'h4: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALUN_BTRNSQ;
						else
							opUCmdIx	= JX2_UCIX_ALUN_BTRNS;
						opNmid	= JX2_UCMD_UNARY;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end

					4'h5: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_EXTUL;
						else
							opUCmdIx	= JX2_UCIX_CONV_EXTSL;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end

					4'h6: begin
						if(opExQ)
							opNmid	= JX2_UCMD_SHADQ3;
						else
							opNmid	= JX2_UCMD_SHAD3;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_SHAD_SHADQ3;
						else
							opUCmdIx	= JX2_UCIX_SHAD_SHAD3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end
					4'h7: begin
						if(opExQ)
							opNmid	= JX2_UCMD_SHLDQ3;
						else
							opNmid	= JX2_UCMD_SHLD3;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
						else
							opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_NB;
					end

					4'h8: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_EXTUB;
						else
							opUCmdIx	= JX2_UCIX_CONV_EXTSB;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end
					4'h9: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_EXTUW;
						else
							opUCmdIx	= JX2_UCIX_CONV_EXTSW;
						opNmid	= JX2_UCMD_CONV_RR;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end

					4'hA: begin
//						opNmid		= JX2_UCMD_MOV_RC;
//						opNmid		= JX2_UCMD_MOV_RR;
						opNmid		= JX2_UCMD_CONV_RR;
						opUCmdIx	= JX2_UCIX_CONV_MOV;

						opFmid		= JX2_FMID_REGREG;
//						opIty		= JX2_ITY_UL;
						opIty		= JX2_ITY_NL;
					end
					4'hB: begin
//						opNmid		= JX2_UCMD_MOV_CR;
//						opNmid		= JX2_UCMD_MOV_RR;
						opNmid		= JX2_UCMD_CONV_RR;
						opUCmdIx	= JX2_UCIX_CONV_MOV;
						opFmid		= JX2_FMID_REGREG;
//						opIty		= JX2_ITY_UQ;
						opIty		= JX2_ITY_NQ;
					end

`ifndef def_true
					4'hC: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALUN_CLNZQ;
						else
							opUCmdIx	= JX2_UCIX_ALUN_CLNZ;
						opNmid	= JX2_UCMD_UNARY;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end
					4'hD: begin
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALUN_CTNZQ;
						else
							opUCmdIx	= JX2_UCIX_ALUN_CTNZ;
						opNmid	= JX2_UCMD_UNARY;
						opFmid	= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
					end
`endif

					4'hE: begin
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQGE;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPGE;
`ifdef jx2_enable_pred_s
						if(opExI)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQGE_S :
								JX2_UCIX_ALU_CMPGE_S ;
						end
`endif
					end
					4'hF: begin
						opNmid		= JX2_UCMD_ALUCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALU_CMPQHS;
						else
							opUCmdIx	= JX2_UCIX_ALU_CMPHS;
`ifdef jx2_enable_pred_s
						if(opExI)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQHS_S :
								JX2_UCIX_ALU_CMPHS_S ;
						end
`endif
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

						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
							opUCmdIx	= JX2_UCIX_FPCX_XE;
						end
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

						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
							opUCmdIx	= JX2_UCIX_FPCX_XI;
						end
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

						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
							opUCmdIx	= JX2_UCIX_FPCX_XE;
						end
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

						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
							opUCmdIx	= JX2_UCIX_FPCX_XI;
						end
					end
					4'h7: begin
						opNmid		= JX2_UCMD_FSTCX;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPCX_HG;
					end
					4'h8: begin
//						opNmid		= JX2_UCMD_FIXS;
//						opFmid		= JX2_FMID_REGREG;
//						opIty		= JX2_ITY_UB;
//						opUCmdIx	= JX2_UCIX_FPIX_FNEG_G;

						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_FNEG;
					end
					4'h9: begin
//						opNmid		= JX2_UCMD_FIXS;
//						opFmid		= JX2_FMID_REGREG;
//						opIty		= JX2_ITY_UB;
//						opUCmdIx	= JX2_UCIX_FPIX_FABS_G;

						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_FABS;
					end
					4'hA: begin
						opNmid		= JX2_UCMD_FCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
//						opUCmdIx	= JX2_UCIX_FPU_CMPEQ_G;
						opUCmdIx	= JX2_UCIX_FPU_CMPEQ;
					end
					4'hB: begin
						opNmid		= JX2_UCMD_FCMP;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
//						opUCmdIx	= JX2_UCIX_FPU_CMPGT_G;
						opUCmdIx	= JX2_UCIX_FPU_CMPGT;
					end

`ifdef jx2_fpu_enable_fdiv
					4'hC: begin		/* F0nm_1eCD */
						opNmid		= JX2_UCMD_FPU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPU_FSQRT;

`ifdef jx2_use_fpu_v2sd
						if(fpuLowPrec)
							opNmid		= JX2_UCMD_FPUV4SF;
`endif

						if(opExQ)
						begin
							opUCmdIx	= JX2_UCIX_FPU_FSQRTX;
							opUCty		= JX2_IUC_WX;
						end
					end
					4'hD: begin		/* F0nm_1eDD */
						opNmid		= JX2_UCMD_FPU3;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_FPU_FSQRTA;

`ifdef jx2_use_fpu_v2sd
						if(fpuLowPrec)
							opNmid		= JX2_UCMD_FPUV4SF;
`endif

						if(opExQ)
						begin
							opUCmdIx	= JX2_UCIX_FPU_FSQRTXA;
							opUCty		= JX2_IUC_WX;
						end
					end
`endif

					default: begin
					end
				endcase
			end

			16'h1zzE: begin	/* F0nm_1ezE */
				case(istrWord[23:20])					

					4'h0: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_RGBSHR1;
					end

					4'h1: begin
						opNmid		= JX2_UCMD_UNARY;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_ALUN_PMORTQ;
						else
							opUCmdIx	= JX2_UCIX_ALUN_PMORTL;
					end

					4'h2: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB5PCK64;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB5PCK32;
					end
					4'h3: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB5UPCK64;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB5UPCK32;
					end

					4'h4: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB32PCK64;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB5PCK32;
					end
					4'h5: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB32UPCK64;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB5UPCK32;
					end


					4'h8: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB32UPCK64FU;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB32UPCK64FS;
					end
					4'h9: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB30AUPCK64F;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB30AUPCK64F;
					end
					4'hA: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB32PCK64FU;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB32PCK64FS;
					end
					4'hB: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_RGB30APCK64F;
						else
							opUCmdIx	= JX2_UCIX_CONV_RGB30APCK64F;
					end

`ifdef jx2_enable_convfp16
					4'hC: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_FP16UPCK32H;
						else
							opUCmdIx	= JX2_UCIX_CONV_FP16UPCK32L;
					end

					4'hD: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_FP16EUPCK32H;
						else
							opUCmdIx	= JX2_UCIX_CONV_FP16EUPCK32L;
					end

					4'hE: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV_FP16PCK32;
						else
							opUCmdIx	= JX2_UCIX_CONV_FP16PCK32;
					end
`endif

					default: begin
					end
				endcase
			end

			16'h1zzF: begin	/* F0nm_1ezF */
				case(istrWord[23:20])
`ifdef jx2_enable_conv_vubtof16
					4'h0: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VUBTOF16L;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VSBTOF16L;
					end
					4'h1: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VUBTOF16H;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VSBTOF16H;
					end
					4'h2: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VUWTOF32L;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VSWTOF32L;
					end
					4'h3: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VUWTOF32H;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VSWTOF32H;
					end

					4'h4: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VF16TOUB;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VF16TOSB;
					end
					4'h5: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VUWTOF16;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VSWTOF16;
					end
					4'h6: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VF32TOUW;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VF32TOSW;
					end
					4'h7: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_VF16TOUW;
						else
							opUCmdIx	= JX2_UCIX_CONV2_VF16TOSW;
					end
`endif

`ifdef jx2_enable_conv_psqrta
					4'h8: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_PSQRTUAH;
						else
							opUCmdIx	= JX2_UCIX_CONV2_PSQRTSAH;
					end
					4'h9: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_PSQRTUAF;
						else
							opUCmdIx	= JX2_UCIX_CONV2_PSQRTSAF;
					end
					4'hA: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_PRELUH;
						else
							opUCmdIx	= JX2_UCIX_CONV2_PRCPAH;
					end
					4'hB: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_PRELUF;
						else
							opUCmdIx	= JX2_UCIX_CONV2_PRCPAF;
					end
`endif

					default: begin
					end
				endcase
			end

			16'h2zz0: begin		/* F0nm_2eo0 */
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_ALU3;
					opUCmdIx	= JX2_UCIX_ALU_PADDL;
				end
				else
				begin
					opNmid		= JX2_UCMD_ALUW3;
					opUCmdIx	= JX2_UCIX_ALUW_PADDW;
				end
			end
			16'h2zz1: begin		/* F0nm_2eo1 */
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_ALU3;
					opUCmdIx	= JX2_UCIX_ALU_PSUBL;
				end
				else
				begin
					opNmid		= JX2_UCMD_ALUW3;
					opUCmdIx	= JX2_UCIX_ALUW_PSUBW;
				end
			end

			16'h2zz2: begin		/* F0nm_2eo2 */
				opNmid	= opExQ ? JX2_UCMD_SHADQ3 : JX2_UCMD_SHAD3;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_SHAD_SHARQ3;
				else
					opUCmdIx	= JX2_UCIX_SHAD_SHAR3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h2zz3: begin		/* F0nm_2eo3 */
				opNmid	= opExQ ? JX2_UCMD_SHLDQ3 : JX2_UCMD_SHLD3;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_SHAD_SHLRQ3;
				else
					opUCmdIx	= JX2_UCIX_SHAD_SHLR3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h2zz4: begin		/* F0nm_2eo4 */
				opNmid		= JX2_UCMD_ALUW3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALUW_PCSELT;

				if(opExQ)
				begin
					opNmid		= JX2_UCMD_ALU3;
					opUCmdIx	= JX2_UCIX_ALU_PCSELT;
				end
			end

			16'h2zz5: begin		/* F0nm_2eo5 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PADD;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_PADDX;
					opUCty		= JX2_IUC_WX;
				end
`ifdef jx2_use_fpu_v4sf
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end
			16'h2zz6: begin		/* F0nm_2eo6 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PSUB;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_PSUBX;
					opUCty		= JX2_IUC_WX;
				end
`ifdef jx2_use_fpu_v4sf
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end
			16'h2zz7: begin		/* F0nm_2eo7 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PMUL;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_PMULX;
					opUCty		= JX2_IUC_WX;
				end
`ifdef jx2_use_fpu_v4sf
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end

			16'h2zz8: begin		/* F0nm_2eo8 */
				opNmid		= JX2_UCMD_ALUW3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_ALUW_MOVLD;
				else
					opUCmdIx	= JX2_UCIX_ALUW_MOVHD;
			end
			16'h2zz9: begin		/* F0nm_2eo9 */
				opNmid		= JX2_UCMD_ALUW3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_ALUW_MOVLHD;
				else
					opUCmdIx	= JX2_UCIX_ALUW_MOVHLD;
			end

			16'h2zzA: begin		/* F0nm_2eoA */
				opNmid		= JX2_UCMD_ALUW3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALUW_PSRCHE;
				if(opExQ)
					opNmid		= JX2_UCMD_ALUB3;

//				if(opExQ)
//					opUCmdIx	= JX2_UCIX_ALUW_MOVLHD;
//				else
//					opUCmdIx	= JX2_UCIX_ALUW_PSRCHE;
			end
			16'h2zzB: begin		/* F0nm_2eoB */
				opNmid		= JX2_UCMD_ALUW3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALUW_PSRCHN;
				if(opExQ)
					opNmid		= JX2_UCMD_ALUB3;

//				if(opExQ)
//					opUCmdIx	= JX2_UCIX_ALUW_MOVLHD;
//				else
//					opUCmdIx	= JX2_UCIX_ALUW_MOVHLD;
			end

`ifdef jx2_enable_btcutx
`ifdef jx2_do_btcutx_alu
			16'h2zzC: begin		/* F0nm_2eoC */
				opNmid		= JX2_UCMD_CONV2_RR;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_CONV2_BLKUTX2;
				else
					opUCmdIx	= JX2_UCIX_CONV2_BLKUTX1;
			end
`else
			16'h2zzC: begin		/* F0nm_2eoC */
				opNmid		= JX2_UCMD_OP_IXS;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_IXS_BLKUTX2;
				else
					opUCmdIx	= JX2_UCIX_IXS_BLKUTX1;
			end
`endif
`endif

			16'h2zzD: begin		/* F0nm_2eoD */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PADDH;
`ifdef jx2_use_fpu_v4sf
				opNmid		= JX2_UCMD_FPUV4SF;
`endif
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PADDXD;
					opUCty		= JX2_IUC_WX;

`ifdef jx2_use_fpu_v2sd
					if(opImm_disp11as[7:2]==6'h02)
						opNmid		= JX2_UCMD_FPUV4SF;
					if(fpuLowPrec)
						opNmid		= JX2_UCMD_FPUV4SF;
`endif
				end
			end
			16'h2zzE: begin		/* F0nm_2eoE */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PSUBH;
`ifdef jx2_use_fpu_v4sf
				opNmid		= JX2_UCMD_FPUV4SF;
`endif
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PSUBXD;
					opUCty		= JX2_IUC_WX;

`ifdef jx2_use_fpu_v2sd
					if(opImm_disp11as[7:2]==6'h02)
						opNmid		= JX2_UCMD_FPUV4SF;
					if(fpuLowPrec)
						opNmid		= JX2_UCMD_FPUV4SF;
`endif
				end
			end
			16'h2zzF: begin		/* F0nm_2eoF */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PMULH;
`ifdef jx2_use_fpu_v4sf
				opNmid		= JX2_UCMD_FPUV4SF;
`endif
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_FPU3;
					opUCmdIx	= JX2_UCIX_FPU_PMULXD;
					opUCty		= JX2_IUC_WX;

`ifdef jx2_use_fpu_v2sd
					if(opImm_disp11as[7:2]==6'h02)
						opNmid		= JX2_UCMD_FPUV4SF;
					if(fpuLowPrec)
						opNmid		= JX2_UCMD_FPUV4SF;
`endif
				end
			end

			16'h3zz0: begin	/* F0zz_3en0 */
				casez(istrWord[7:0])

					8'h00: begin
						case(istrWord[23:20])
							4'h0: begin
								opNmid		= JX2_UCMD_NOP;
								opFmid		= JX2_FMID_Z;
							end
							4'h1: begin
								opNmid		= JX2_UCMD_JMP;
								opFmid		= JX2_FMID_Z;
								opRegM_Fix	= JX2_GR_LR;
							end
							4'h2: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SLEEP;
							end
							4'h3: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_BREAK;
							end
							4'h4: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_CLRT;
							end
							4'h5: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SETT;
							end
							4'h6: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_CLRS;
							end
							4'h7: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SETS;
							end
							4'h8: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_NOTT;
							end
							4'h9: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_NOTS;
							end

							4'hC: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_RTE;
								usrReject	= 1;
							end

							4'hF: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_LDTLB;
								opRegM_Fix	= JX2_GR_DHR;
								opRegN_Fix	= JX2_GR_DLR;
								usrReject	= 1;
							end

							default: begin
							end
						endcase
					end

					8'h02: begin
						case(istrWord[23:20])
							4'h0: begin
								opNmid		= JX2_UCMD_NOP;
								opFmid		= JX2_FMID_Z;
							end
							4'h1: begin
								opNmid		= JX2_UCMD_JMP;
								opFmid		= JX2_FMID_Z;
								opRegM_Fix	= JX2_GR_LR;
							end

							4'h4: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_LDACL;
								opRegM_Fix	= JX2_GR_DHR;
								opRegN_Fix	= JX2_GR_DLR;
								usrReject	= 1;
							end

							4'h8: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SXENTR;
								opIty		= JX2_ITY_SB;
								usrReject	= 1;
//								usrSuAllow	= srMod[1];
								usrSuAllow	= 2;
							end
							4'h9: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SUENTR;
								opIty		= JX2_ITY_SB;
								usrReject	= 1;
//								usrSuAllow	= srMod[1];
								usrSuAllow	= 2;
							end

							4'hA: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SVEKRR;
								opIty		= JX2_ITY_UB;
								opUCty		= JX2_IUC_WX;
								usrReject	= 1;
								usrSuAllow	= 1;
							end

							4'hB: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SVENTR;
								opIty		= JX2_ITY_SB;
								usrReject	= 1;
//								usrSuAllow	= srMod[1] && srMod[2];
								usrSuAllow	= 3;
							end

							4'hC: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_LDEKRR;
//								opIty		= JX2_ITY_UB;
//								opUCty		= JX2_IUC_WX;
								usrReject	= 1;
								usrSuAllow	= 1;
							end
							4'hD: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_LDEKEY;
//								opIty		= JX2_ITY_UB;
//								opUCty		= JX2_IUC_WX;
								usrReject	= 1;
							end
							4'hE: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_LDEENC;
								opIty		= JX2_ITY_UB;
								opUCty		= JX2_IUC_WX;
								usrReject	= 1;
							end

							4'hF: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_INVTLB;
								usrReject	= 1;
							end

							default: begin
							end
						endcase
					end

					8'h10: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SW;
					end
					8'h11: begin
						opNmid		= JX2_UCMD_BSR;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SW;
					end
					8'h12: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SW;
						opCcty		= JX2_IXC_CT;
					end
					8'h13: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SW;
						opCcty		= JX2_IXC_CF;
					end

					8'h14: begin
						opNmid		= JX2_UCMD_NOP;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
//						opBty		= JX2_BTY_SW;
//						opCcty		= JX2_IXC_CF;
					end


`ifndef def_true
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
						usrReject	= 1;
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
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_PUSH_CR;
						usrReject	= 1;
					end
`endif

`ifndef def_true
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
`endif

`ifdef def_true
					8'h1C: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_IXS_INVIC;
						usrReject	= 1;
					end
					8'h1D: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_IXS_INVDC;
						usrReject	= 1;
					end
`endif

					8'h20: begin
						opNmid		= JX2_UCMD_JMP;
						opFmid		= JX2_FMID_REG;
//						opIty		= JX2_ITY_UB;
						opIty		= JX2_ITY_XW;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
					8'h21: begin
						opNmid		= JX2_UCMD_JSR;
						opFmid		= JX2_FMID_REG;
//						opIty		= JX2_ITY_UB;
						opIty		= JX2_ITY_XW;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
					8'h22: begin
						opNmid		= JX2_UCMD_JMP;
						opFmid		= JX2_FMID_REG;
//						opIty		= JX2_ITY_UB;
						opIty		= JX2_ITY_XW;
						opCcty		= JX2_IXC_CT;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
					8'h23: begin
						opNmid		= JX2_UCMD_JMP;
						opFmid		= JX2_FMID_REG;
//						opIty		= JX2_ITY_UB;
						opIty		= JX2_ITY_XW;
						opCcty		= JX2_IXC_CF;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end

`ifndef def_true
					8'h2C: begin
						opNmid		= JX2_UCMD_PUSHX;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_PUSH_GRX2;
					end
					8'h2E: begin
						opNmid		= JX2_UCMD_POPX;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						opUCmdIx	= JX2_UCIX_PUSH_GRX2;
					end
`endif

`ifdef jx2_enable_ops24
					8'h28: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SB;
					end
					8'h29: begin
						opNmid		= JX2_UCMD_BSR;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SB;
					end
					8'h2A: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SB;
						opCcty		= JX2_IXC_CT;
					end
					8'h2B: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SB;
						opCcty		= JX2_IXC_CF;
					end
`endif

`ifdef def_true
					8'h2C: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SL;
					end
					8'h2D: begin
						opNmid		= JX2_UCMD_BSR;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SL;
					end
					8'h2E: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SL;
						opCcty		= JX2_IXC_CT;
					end
					8'h2F: begin
						opNmid		= JX2_UCMD_BRA;
						opFmid		= JX2_FMID_REGPC;
						opIty		= JX2_ITY_SB;
						opBty		= JX2_BTY_SL;
						opCcty		= JX2_IXC_CF;
					end
`endif

					8'h33: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opUCmdIx	= JX2_UCIX_IXS_MOVNT;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end

					8'h36: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_ROTCL;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_ROTCLQ;
					end
					8'h37: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_ROTCR;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_ROTCRQ;
					end

					8'h3E: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opUCmdIx	= JX2_UCIX_IXS_MOVST;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end
					8'h3F: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opUCmdIx	= JX2_UCIX_IXS_MOVPQ;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end

// `ifdef jx2_enable_packbcd
`ifndef def_true
					8'h60: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_DAAQ;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end
					8'h61: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_DASQ;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end
`endif

					8'h68: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opUCmdIx	= JX2_UCIX_IXS_TRAPB;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
						if(opExQ)
							opUCty		= JX2_IUC_WX;
						usrReject	= 1;
					end

					8'h69: begin
`ifdef jx2_enable_wex
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_WEXMD;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
`else
						opNmid		= JX2_UCMD_NOP;
						opFmid		= JX2_FMID_Z;
`endif
					end
					8'h6A: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_CPUID;
						opFmid		= JX2_FMID_REG;
//						opIty		= JX2_ITY_SB;
						opIty		= JX2_ITY_XB;
						usrReject	= 1;
					end
					8'h6B: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_SRTTWID;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end

					8'h6F: begin
						opNmid		= JX2_UCMD_OP_IXS;
						opUCmdIx	= JX2_UCIX_IXS_MOVT;
						opFmid		= JX2_FMID_REG;
						opIty		= JX2_ITY_SB;
					end

					default: begin
					end

				endcase
			end

			16'h3zz1: begin		/* F0nm_3eo1 */
				opNmid		= JX2_UCMD_ALUW3;
				opUCmdIx	= JX2_UCIX_ALUW_MOVTA16;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SW;
				if(opExQ && !opIsJumbo)
//				if((opExQ && !opIsJumboAu) ||
//					(opIsJumboAu && opIsImm4R && !opExQ))
				begin
					opUCmdIx	= JX2_UCIX_ALUW_MOVTA5U;
				end
			end

			16'h3zz2: begin		/* F0nm_3eo2 */
				opNmid		= JX2_UCMD_SHADQ3;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_SHAD_SHARX3;
					opUCty		= JX2_IUC_WX;
				end
				else
				begin
					opUCmdIx	= JX2_UCIX_SHAD_ROTLQ3;
				end
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h3zz3: begin		/* F0nm_3eo3 */
				opNmid	= JX2_UCMD_SHLDQ3;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_SHAD_SHLRX3;
					opUCty		= JX2_IUC_WX;
				end
				else
				begin
					opUCmdIx	= JX2_UCIX_SHAD_ROTRQ3;
				end
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h3zz4: begin		/* F0nm_3eo4 */
				opNmid		= JX2_UCMD_SHADQ3;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_SHAD_SHADX3;
					opUCty		= JX2_IUC_WX;
				end
				else
				begin
//					opUCmdIx	= JX2_UCIX_SHAD_ROTLX3;
//					opNmid		= JX2_UCMD_MUL3;
//					opUCmdIx	= JX2_UCIX_MUL3_MAC3S;

					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_MULHS;
				end
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h3zz5: begin		/* F0nm_3eo5 */
				opNmid	= JX2_UCMD_SHLDQ3;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_SHAD_SHLDX3;
					opUCty		= JX2_IUC_WX;
				end
				else
				begin
//					opUCmdIx	= JX2_UCIX_SHAD_ROTRX3;
//					opNmid		= JX2_UCMD_MUL3;
//					opUCmdIx	= JX2_UCIX_MUL3_MAC3U;

					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_MULHU;
				end
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h3zz6: begin		/* F0nm_3eo6 */
				opNmid	= JX2_UCMD_SHLDQ3;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_SHAD_ROTLX3;
					opUCty		= JX2_IUC_WX;
				end
				else
				begin
					opNmid		= JX2_UCMD_SHLD3;
					opUCmdIx	= JX2_UCIX_SHAD_ROTL3;
				end
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h3zz7: begin
				opNmid		= JX2_UCMD_SWAPN;
				opFmid		= JX2_FMID_REGREG;
//				opUCmdIx	= { 2'h0, istrWord[3:0] };
				opUCmdIx	= { 2'h0, istrWord[23:20] };
			end

			16'h4zz0: begin		/* F0nm_4eo0 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= JX2_BTY_UQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opBty	= JX2_BTY_SQ;
					opIty	= JX2_ITY_UL;
				end
			end

`ifndef def_true
			16'h4zz3: begin		/* F0nm_4eo3 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UL;
			end
`endif
			16'h4zz4: begin		/* F0nm_4eo4 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_UQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opBty	= JX2_BTY_SQ;
					opIty	= JX2_ITY_UL;
				end
			end

`ifdef jx2_enable_fmov
			16'h4zz6: begin		/* F0nm_4eo6 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h4zz7: begin		/* F0nm_4eo7 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
`endif

			16'h4zz8: begin		/* F0nm_4eo8 */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= JX2_BTY_UQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opBty	= JX2_BTY_SQ;
					opIty	= JX2_ITY_UL;
				end
			end

`ifndef def_true
			16'h4zzB: begin		/* F0nm_4eoB */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UL;
			end
`endif

			16'h4zzC: begin		/* F0nm_4eoC */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_UQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
					opBty	= JX2_BTY_SQ;
					opIty	= JX2_ITY_UL;
				end
			end

`ifdef jx2_enable_fmov
			16'h4zzE: begin		/* F0nm_4eoE */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
			end
			16'h4zzF: begin		/* F0nm_4eoF */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
			end
`endif

			16'h5zz0: begin		/* F0nm_5eo0 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
//				opIty		= JX2_ITY_UB;
//				opUCmdIx	= JX2_UCIX_ALU_CSELT;
				opUCmdIx	= opExQ ? JX2_UCIX_ALU_PCSELT : JX2_UCIX_ALU_CSELT;
			end

			16'h5zz1: begin		/* F0nm_5eo1 */
				opNmid		= JX2_UCMD_MULW3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ?
					JX2_UCIX_MUL3_PWMULU :
					JX2_UCIX_MUL3_PWMULS;
				opIty		= JX2_ITY_SB;
			end

			16'h5zz2: begin		/* F0nm_5eo2 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SW;
				opUCmdIx	= JX2_UCIX_MUL3_DMUL3S;
				
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_MULS;
				end
			end
			16'h5zz3: begin		/* F0nm_5eo3 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SW;
				opUCmdIx	= JX2_UCIX_MUL3_DMUL3U;
				
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_MULU;
				end
			end

			16'h5zz4: begin		/* F0nm_5eo4 */
				opNmid	= opExQ ? JX2_UCMD_SHADQ3 : JX2_UCMD_SHAD3;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_SHAD_SHADQ3;
				else
					opUCmdIx	= JX2_UCIX_SHAD_SHAD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end
			16'h5zz5: begin		/* F0nm_5eo5 */
				opNmid	= opExQ ? JX2_UCMD_SHLDQ3 : JX2_UCMD_SHLD3;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
				else
					opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
				opFmid	= JX2_FMID_REGREG;
				opIty	= JX2_ITY_SB;
			end

			16'h5zz6: begin		/* F0nm_5eo6 */
				opNmid		= JX2_UCMD_MULW3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ?
					JX2_UCIX_MUL3_PLMULU :
					JX2_UCIX_MUL3_PLMULS;
				opIty		= JX2_ITY_SB;
			end
			16'h5zz7: begin		/* F0nm_5eo7 */
				opNmid		= JX2_UCMD_MULW3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ?
					JX2_UCIX_MUL3_PHMULU :
					JX2_UCIX_MUL3_PHMULS;
				opIty		= JX2_ITY_SB;
			end

			16'h5zz8: begin		/* F0nm_5eo8 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
//				opUCmdIx	= JX2_UCIX_FPU_FADD_G;
				opUCmdIx	= JX2_UCIX_FPU_FADD;

`ifdef jx2_use_fpu_v2sd
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif

				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_FADDX;
					opUCty		= JX2_IUC_WX;
				end
			end
			16'h5zz9: begin		/* F0nm_5eo9 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
//				opUCmdIx	= JX2_UCIX_FPU_FSUB_G;
				opUCmdIx	= JX2_UCIX_FPU_FSUB;

`ifdef jx2_use_fpu_v2sd
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif

				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_FSUBX;
					opUCty		= JX2_IUC_WX;
				end
			end
			16'h5zzA: begin		/* F0nm_5eoA */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
//				opUCmdIx	= JX2_UCIX_FPU_FMUL_G;
				opUCmdIx	= JX2_UCIX_FPU_FMUL;

`ifdef jx2_use_fpu_v2sd
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif

				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_FMULX;
					opUCty		= JX2_IUC_WX;
				end
			end

			16'h5zzB: begin		/* F0nm_5eoB */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
//				opUCmdIx	= JX2_UCIX_FPU_FMUL_G;
				opUCmdIx	= JX2_UCIX_FPU_FMAC;

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif

				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_FMACX;
					opUCty		= JX2_IUC_WX;
				end
			end

			16'h5zzC: begin		/* F0nm_5eoC */
//`ifdef jx2_addxl_agu
//				opNmid		= JX2_UCMD_LEA_MR;
//				opFmid		= JX2_FMID_REGREG;
//				opUCmdIx	= opExQ ? 6'h04 : 6'h0C;
//`else
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ? JX2_UCIX_ALU_ADDUL : JX2_UCIX_ALU_ADDSL;
//`endif
			end
			16'h5zzD: begin		/* F0nm_5eoD */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= opExQ ? JX2_UCIX_ALU_SUBUL : JX2_UCIX_ALU_SUBSL;
			end

			16'h5zzE: begin		/* F0nm_5eoE */
				opNmid		= JX2_UCMD_MULW3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_MULS;
				opIty		= JX2_ITY_SW;
			end
			16'h5zzF: begin		/* F0nm_5eoF */
				opNmid		= JX2_UCMD_MULW3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_MULU;
				opIty		= JX2_ITY_SW;
			end

`ifdef jx2_alu_dmac
			16'h6zz0: begin		/* F0nm_6eo0 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_MAC3S;
				opIty		= JX2_ITY_SW;
			end
			16'h6zz1: begin		/* F0nm_6eo1 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_MAC3U;
				opIty		= JX2_ITY_SW;
			end

			16'h6zz2: begin		/* F0nm_6eo2 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_DMAC3S;
				opIty		= JX2_ITY_SW;
			end
			16'h6zz3: begin		/* F0nm_6eo3 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
				opUCmdIx	= JX2_UCIX_MUL3_DMAC3U;
				opIty		= JX2_ITY_SW;
			end
`endif

			16'h6zz4: begin		/* F0nm_6eo4 */
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_QMUL_DIVS;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_QMUL_DIVU;
			end
			16'h6zz5: begin		/* F0nm_6eo5 */
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_QMUL_MODS;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_QMUL_MODU;
			end

`ifdef jx2_fpu_enable_fdiv
			16'h6zz6: begin		/* F0nm_6eo6 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FDIV;

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif

				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_FDIVX;
					opUCty		= JX2_IUC_WX;
				end
`ifdef jx2_alu_slomuldiv_fdiv
				if(!opExQ)
				begin
					opNmid		= JX2_UCMD_QMULDIV;
					opUCmdIx	= JX2_UCIX_QMUL_FDIV;
				end
`endif
			end
			16'h6zz7: begin		/* F0nm_6eo7 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FDIVA;

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif

				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_FDIVXA;
					opUCty		= JX2_IUC_WX;
				end
			end
`endif

			16'h6zz8: begin		/* F0nm_6eo8 */
				if(opExQ)
				begin
`ifdef jx2_enable_btcutx3
`ifdef jx2_do_btcutx_alu
					opNmid		= JX2_UCMD_CONV2_RR;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_CONV2_BLKUTX3H;
					opUCty		= JX2_IUC_WX;
`else
					opNmid		= JX2_UCMD_OP_IXS;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_IXS_BLKUTX3H;
					opUCty		= JX2_IUC_WX;
`endif
`endif
				end
				else
				begin
`ifdef jx2_enable_btcutx2
`ifdef jx2_do_btcutx_alu
					opNmid		= JX2_UCMD_CONV2_RR;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_CONV2_BLKUTX2;
`else
					opNmid		= JX2_UCMD_OP_IXS;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_IXS_BLKUTX2;
`endif
`endif
				end
			end
			16'h6zz9: begin		/* F0nm_6eo9 */
				if(opExQ)
				begin
`ifdef jx2_enable_btcutx3
`ifdef jx2_do_btcutx_alu
					opNmid		= JX2_UCMD_CONV2_RR;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_CONV2_BLKUTX3L;
					opUCty		= JX2_IUC_WX;
`else
					opNmid		= JX2_UCMD_OP_IXS;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_IXS_BLKUTX3L;
					opUCty		= JX2_IUC_WX;
`endif
`endif
				end
				else
				begin
`ifdef jx2_enable_blint
					opNmid		= JX2_UCMD_BLINT;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_BLINT_LERP;
`endif
				end
			end

`ifdef jx2_enable_blint
			16'h6zzA: begin		/* F0nm_6eoA */
				opNmid		= JX2_UCMD_BLINT;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_BLINT_BILERP;
				if(opExQ)
					opUCty		= JX2_IUC_WX;
			end
`endif

`ifndef def_true
			16'h6zzB: begin		/* F0nm_6eoB */
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_MUL3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_MUL3_DMAC3U;
				end
				else
				begin
					opNmid		= JX2_UCMD_MUL3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_MUL3_DMAC3S;
				end
			end
`endif

`ifdef jx2_enable_bitsel
			16'h6zzB: begin		/* F0nm_6eoB */
				opNmid		= JX2_UCMD_ALUB3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALUB_BITSEL;
				if(opExQ)
					opUCty		= JX2_IUC_WX;
			end
`endif

			16'h6zzC: begin		/* F0nm_6eoC */
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_MUL3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_MUL3_BLKUAB2;
				end
				else
				begin
					opNmid		= JX2_UCMD_MUL3;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_MUL3_BLKUAB1;
				end
			end

			16'h6zzD: begin		/* F0nm_6eoD */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FADD_G;

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end
			16'h6zzE: begin		/* F0nm_6eoE */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FSUB_G;

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end
			16'h6zzF: begin		/* F0nm_6eoF */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FMUL_G;

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end

			16'h7zz4: begin		/* F0nm_7eo4 */
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_QMUL_DIVSL;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_QMUL_DIVUL;
			end
			16'h7zz5: begin		/* F0nm_7eo5 */
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_QMUL_MODSL;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_QMUL_MODUL;
			end


			16'h8zz0: begin		/* F0nm_8eo0 */
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
				opBty	= JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz1: begin		/* F0nm_8eo1 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz2: begin		/* F0nm_8eo2 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz3: begin		/* F0nm_8eo3 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
				opUCty	= opExQ ? JX2_IUC_WXA : JX2_IUC_WA;
			end

			16'h8zz4: begin		/* F0nm_8eo4 */
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
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz5: begin		/* F0nm_8eo5 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz6: begin		/* F0nm_8eo6 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz7: begin		/* F0nm_8eo7 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
				opUCty	= opExQ ? JX2_IUC_WXA : JX2_IUC_WA;
			end

			16'h8zz8: begin		/* F0nm_8eo8 */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zz9: begin		/* F0nm_8eo9 */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zzA: begin		/* F0nm_8eoA */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zzB: begin		/* F0nm_8eoB */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
				opUCty	= opExQ ? JX2_IUC_WXA : JX2_IUC_WA;
			end

			16'h8zzC: begin		/* F0nm_8eoC */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zzD: begin		/* F0nm_8eoD */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zzE: begin		/* F0nm_8eoE */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
				opUCty	= JX2_IUC_WA;
			end
			16'h8zzF: begin		/* F0nm_8eoF */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
				opIty	= JX2_ITY_UB;
				opUCty	= opExQ ? JX2_IUC_WXA : JX2_IUC_WA;
			end


			16'hCzzz: begin		/* F0dd_Cddd */
				opNmid	= JX2_UCMD_BRA;
//				opNmid	= tBlockIsF4 ? JX2_UCMD_NOP : JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
			end
			16'hDzzz: begin		/* F0dd_Dddd */
				opNmid	= JX2_UCMD_BSR;
//				opNmid	= tBlockIsF4 ? JX2_UCMD_NOP : JX2_UCMD_BSR;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
			end
			16'hEzzz: begin		/* F0dd_Eddd */
//				opNmid	= JX2_UCMD_BRA;
				opNmid	= tBlockIsF4 ? JX2_UCMD_NOP : JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
				opCcty	= JX2_IXC_CT;
			end
			16'hFzzz: begin		/* F0dd_Fddd */
//				opNmid	= JX2_UCMD_BRA;
				opNmid	= tBlockIsF4 ? JX2_UCMD_NOP : JX2_UCMD_BRA;
				opFmid	= JX2_FMID_PCDISP8;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_SW;
				opCcty	= JX2_IXC_CF;
			end

			default: begin
			end
		endcase
	end
	else
		if(tBlockIsF1)		/* F1nm_Xedd */
	begin
		case(istrWord[31:28])
			4'h0: begin		/* F1nm_0edd */
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end
			4'h1: begin		/* F1nm_1edd */
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SW;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end
			4'h2: begin		/* F1nm_2edd */
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SL;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end
			4'h3: begin		/* F1nm_3edd */
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SQ;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SL;
				end else begin
					opNmid	= JX2_UCMD_MOV_RM;
					opFmid	= JX2_FMID_REGSTREGDISP;
				end
			end

`ifdef def_true
			4'h4: begin		/* F1nm_4edd */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opIty	= JX2_ITY_SW;
//				opBty	= JX2_BTY_SL;
//				opBty	= opExQ ? JX2_BTY_SL : JX2_BTY_SW;
				opBty	= opExQ ? JX2_BTY_SW : JX2_BTY_SL;
`endif
			end
			4'h5: begin		/* F1nm_5edd */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
//				opBty	= JX2_BTY_UQ;
				opBty	= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
//				opIty	= JX2_ITY_SW;
				opIty	= opExQ ? JX2_ITY_SW : JX2_ITY_UL;
			end
			4'h6: begin		/* F1nm_6edd */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
//				opBty	= JX2_BTY_SL;
//				opBty	= opExQ ? JX2_BTY_SL : JX2_BTY_SW;
				opBty	= opExQ ? JX2_BTY_SW : JX2_BTY_SL;
				opIty	= JX2_ITY_SW;
`endif
			end
			4'h7: begin		/* F1nm_7edd */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
//				opBty	= JX2_BTY_UQ;
				opBty	= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
//				opIty	= JX2_ITY_SW;
				opIty	= opExQ ? JX2_ITY_SW : JX2_ITY_UL;
			end
`endif

// `ifdef jx2_enable_fpu
// `ifdef jx2_enable_fmov
`ifndef def_true
			4'h4: begin		/* F1nm_4edd */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opIty	= JX2_ITY_SW;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
			end
			4'h5: begin		/* F1nm_5edd */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SQ;
			end
			4'h6: begin		/* F1nm_6edd */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opIty	= JX2_ITY_SW;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
			end
			4'h7: begin		/* F1nm_7edd */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SQ;
			end
`endif

			4'h8: begin		/* F1nm_8edd */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opIty	= JX2_ITY_SW;
				opBty	= opExQ ? JX2_BTY_UB : JX2_BTY_SB;
			end
			4'h9: begin		/* F1nm_9edd */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opIty	= JX2_ITY_SW;
				opBty	= opExQ ? JX2_BTY_UW : JX2_BTY_SW;
			end
			4'hA: begin		/* F1nm_Aedd */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opIty	= JX2_ITY_SW;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
			end
			4'hB: begin		/* F1nm_Bedd */
				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opIty	= JX2_ITY_SW;
				opBty	= opExQ ? JX2_BTY_UL : JX2_BTY_SQ;
			end

`ifdef jx2_alu_jcmp
// `ifndef def_true
			4'hC: begin		/* F1nm_Cedd */
				opNmid		= JX2_UCMD_JCMP;
				opFmid		= JX2_FMID_REGPC;
				opUCmdIx	= { 1'b0, opExQ, 3'b100, opExI};
				opIty		= JX2_ITY_UB;
			end
			4'hD: begin		/* F1nm_Dedd */
				opNmid		= JX2_UCMD_JCMP;
				opFmid		= JX2_FMID_REGPC;
//				opUCmdIx	= { 1'b0, opExQ, 3'b010, opExI};
				opUCmdIx	= { 1'b0, opExQ, 3'b001, !opExI};
				opIty		= JX2_ITY_UB;
			end
			4'hE: begin		/* F1nm_Eedd */
				opNmid		= JX2_UCMD_JCMP;
				opFmid		= JX2_FMID_REGPC;
				opUCmdIx	= { 1'b0, opExQ, 3'b011, opExI};
				opIty		= JX2_ITY_UB;
			end
			4'hF: begin		/* F1nm_Fedd */
				opNmid		= JX2_UCMD_JCMP;
				opFmid		= JX2_FMID_REGPC;
				opUCmdIx	= { 1'b0, opExQ, 3'b000, opExI};
				opIty		= JX2_ITY_UB;
			end
`endif

			default: begin
			end
		endcase
	end
	else
		if(tBlockIsF2 || tBlockIsEA_F2)		/* F2nm_Xedd */
	begin
		case(istrWord[31:28])
`ifdef def_true
			4'h0: begin		/* F2nm_0ejj */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGIMMREG;
				opIty		= JX2_ITY_UW;
				opUCmdIx	= JX2_UCIX_ALU_ADD;
`ifdef jx2_use_fpu_fpimm
				case(opIsImmLdOp)
					4'h8: begin
						opNmid		= JX2_UCMD_FPU3;
						opUCmdIx	= JX2_UCIX_FPU_FADD;
						opIty		= JX2_ITY_UB;
`ifdef jx2_use_fpu_v2sd
						if(fpuLowPrec)
							opNmid		= JX2_UCMD_FPUV4SF;
`endif
					end
					4'h9: begin
						opNmid		= JX2_UCMD_FPU3;
						opUCmdIx	= JX2_UCIX_FPU_FSUB;
						opIty		= JX2_ITY_UB;
`ifdef jx2_use_fpu_v2sd
						if(fpuLowPrec)
							opNmid		= JX2_UCMD_FPUV4SF;
`endif
					end
					4'hA: begin
						opNmid		= JX2_UCMD_FPU3;
						opUCmdIx	= JX2_UCIX_FPU_FMUL;
						opIty		= JX2_ITY_UB;
`ifdef jx2_use_fpu_v2sd
						if(fpuLowPrec)
							opNmid		= JX2_UCMD_FPUV4SF;
`endif
					end
					default: begin
					end
				endcase
`endif
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
			4'h3: begin		/* F2nm_3ejj */
				opNmid	= JX2_UCMD_ALU3;
				opFmid	= JX2_FMID_REGIMMREG;
				opIty	= JX2_ITY_UW;
				opUCmdIx	= opExQ ?
					JX2_UCIX_ALU_ADDUL :
					JX2_UCIX_ALU_ADDSL;
			end
			4'h4: begin		/* F2nm_4ejj */
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
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_ALU_SUB;
					opIty		= JX2_ITY_UL;
				end
			end
			4'h6: begin		/* F2nm_6ejj */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGIMMREG;
				opIty		= JX2_ITY_UW;
				opUCmdIx	= JX2_UCIX_ALU_OR;
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_SHADQ3;
					opUCmdIx	= JX2_UCIX_SHAD_SHADX3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCty		= JX2_IUC_WX;
				end
			end
			4'h7: begin		/* F2nm_7ejj */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGIMMREG;
				opIty		= JX2_ITY_UW;
				opUCmdIx	= JX2_UCIX_ALU_XOR;
				if(opExQ)
				begin
					opNmid		= JX2_UCMD_SHLDQ3;
					opUCmdIx	= JX2_UCIX_SHAD_SHLDX3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCty		= JX2_IUC_WX;
				end
			end
`endif

`ifdef def_true
			4'h8: begin		/* F2nm_8ejj */
				if(opExI)
				begin
					opNmid		= JX2_UCMD_ALUW3;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= opExQ ?
						JX2_UCIX_ALUW_PSHUFW :
						JX2_UCIX_ALUW_PSHUFB ;
				end
				else
				begin
					opNmid	= opExQ ?
						JX2_UCMD_SHADQ3 :
						JX2_UCMD_SHAD3;
					if(opExQ)
						opUCmdIx	= JX2_UCIX_SHAD_SHADQ3;
					else
						opUCmdIx	= JX2_UCIX_SHAD_SHAD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
			end
			4'h9: begin		/* F2nm_9ejj */
				if(opExI)
				begin
				end
				else
				begin
					opNmid	= opExQ ?
						JX2_UCMD_SHLDQ3 :
						JX2_UCMD_SHLD3;
					if(opExQ)
						opUCmdIx	= JX2_UCIX_SHAD_SHLDQ3;
					else
						opUCmdIx	= JX2_UCIX_SHAD_SHLD3;
					opFmid	= JX2_FMID_REGIMMREG;
					opIty	= JX2_ITY_UW;
				end
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
						opNmid		= JX2_UCMD_MOV_IR;
						opUCmdIx	= 0;
					end

					4'b0010: begin
						opNmid		= JX2_UCMD_MOV_IR;
						opUCmdIx	= opIsJumbo ?
							JX2_UCIX_LDI_LDISH32 : JX2_UCIX_LDI_LDISH8;
					end

					4'b0011: begin
`ifndef def_true
		//				if(opRegN_Dfl==JX2_GR_SP)
						if(opExM && opIsJumbo)
						begin
							opNmid	= opExQ ? JX2_UCMD_BSR : JX2_UCMD_BRA;
							opFmid	= JX2_FMID_PCDISP8;
							opBty	= JX2_BTY_SW;
							opIty	= JX2_ITY_UW;
						end
						else
						begin
							opNmid		= JX2_UCMD_CONV_RR;
							opIty		= JX2_ITY_UL;
							opUCmdIx	= opExQ ?
								JX2_UCIX_CONV_LDIQHI :
								JX2_UCIX_CONV_LDIHI;
							if(opIsJumbo)
								opUCmdIx	= opExQ ?
								JX2_UCIX_CONV_LDIQHI32 :
								JX2_UCIX_CONV_LDIQHI16;							
						end
`else
						opNmid		= JX2_UCMD_CONV_RR;
						opIty		= JX2_ITY_UL;
						opUCmdIx	= opExQ ?
							JX2_UCIX_CONV_LDIQHI :
							JX2_UCIX_CONV_LDIHI;
						if(opIsJumbo)
							opUCmdIx	= opExQ ?
								JX2_UCIX_CONV_LDIQHI32 :
								JX2_UCIX_CONV_LDIQHI16;							
`endif
					end

					4'b010z: begin
						opNmid	= JX2_UCMD_ALUCMP;
						opUCmdIx	= opExQ ?
							JX2_UCIX_ALU_TSTQ :
							JX2_UCIX_ALU_TST;
`ifdef jx2_enable_pred_s
						if(opExWM)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_TSTQ_S :
								JX2_UCIX_ALU_TST_S ;
						end
`endif
					end
					4'b011z: begin
						opNmid	= JX2_UCMD_ALUCMP;
						opUCmdIx	= opExQ ?
							JX2_UCIX_ALU_CMPQHS :
							JX2_UCIX_ALU_CMPHS;
`ifdef jx2_enable_pred_s
						if(opExWM)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQHS_S :
								JX2_UCIX_ALU_CMPHS_S ;
						end
`endif
					end
					4'b100z: begin
						opNmid	= JX2_UCMD_ALUCMP;
						opUCmdIx	= opExQ ?
							JX2_UCIX_ALU_CMPQHI :
							JX2_UCIX_ALU_CMPHI;
`ifdef jx2_enable_pred_s
						if(opExWM)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQHI_S :
								JX2_UCIX_ALU_CMPHI_S ;
						end
`endif
					end
					4'b101z: begin
						opNmid	= JX2_UCMD_ALUCMP;
						opUCmdIx	= opExQ ?
							JX2_UCIX_ALU_CMPQGE :
							JX2_UCIX_ALU_CMPGE;
`ifdef jx2_enable_pred_s
						if(opExWM)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQGE_S :
								JX2_UCIX_ALU_CMPGE_S ;
						end
`endif
					end
					4'b110z: begin
						opNmid	= JX2_UCMD_ALUCMP;
						opUCmdIx	= opExQ ?
							JX2_UCIX_ALU_CMPQEQ :
							JX2_UCIX_ALU_CMPEQ;
`ifdef jx2_enable_pred_s
						if(opExWM)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQEQ_S :
								JX2_UCIX_ALU_CMPEQ_S ;
						end
`endif
					end
					4'b111z: begin
						opNmid	= JX2_UCMD_ALUCMP;
						opUCmdIx	= opExQ ?
							JX2_UCIX_ALU_CMPQGT :
							JX2_UCIX_ALU_CMPGT;
`ifdef jx2_enable_pred_s
						if(opExWM)
						begin
							opUCmdIx	= opExQ ?
								JX2_UCIX_ALU_CMPQGT_S :
								JX2_UCIX_ALU_CMPGT_S ;
						end
`endif
					end

					default: begin
					end
				endcase
			end
`endif

`ifdef def_true
			4'hD: begin		/* F2nz_Dejj */
				opIty	= istrWord[0] ?
					JX2_ITY_NQ :
					JX2_ITY_UQ;
				opFmid = JX2_FMID_IMM8REG;

				casez(istrWord[3:0])
					4'b000z: begin
						opNmid		= JX2_UCMD_ALU3;
						opUCmdIx	= JX2_UCIX_ALU_ADD;
					end
					4'b001z: begin
						opNmid		= JX2_UCMD_MULW3;
						opUCmdIx	= opExQ ?
							JX2_UCIX_MUL3_MULU :
							JX2_UCIX_MUL3_MULS;
					end

`ifndef def_true
					4'b0100: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opIty		= JX2_ITY_UL;
						opUCmdIx	= opExQ ?
							JX2_UCIX_CONV_LDIQSIZ :
							JX2_UCIX_CONV_LDISIZ;
					end
					4'b0101: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opIty		= JX2_ITY_UL;
						opUCmdIx	= opExQ ?
							JX2_UCIX_CONV_LDIQSIN :
							JX2_UCIX_CONV_LDISIN;
					end
`endif

`ifdef jx2_enable_ldirot
					4'b0100: begin		/* F2n4_Dejj */
						opNmid		= JX2_UCMD_SHADQ3;
						opFmid		= JX2_FMID_IMM4ZREG;
						if(opExQ)
						begin
							opUCmdIx	= JX2_UCIX_SHAD_ROTLQ3;
							opIty		= JX2_ITY_SW;
						end
						else
						begin
							opUCmdIx	= JX2_UCIX_SHAD_ROTL3;
							opIty		= JX2_ITY_SB;
						end
					end
					4'b0101: begin		/* F2n5_Dejj */
						opNmid		= JX2_UCMD_SHADQ3;
						opFmid		= JX2_FMID_IMM4NREG;
						if(opExQ)
						begin
							opUCmdIx	= JX2_UCIX_SHAD_ROTLQ3;
							opIty		= JX2_ITY_SW;
						end
						else
						begin
							opUCmdIx	= JX2_UCIX_SHAD_ROTL3;
							opIty		= JX2_ITY_SB;
						end
					end
`endif

`ifdef jx2_alu_jcmp
					4'b0110: begin		/* F2n6_Dejj */
						opNmid		= JX2_UCMD_JCMP;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27],
							1'b0, istrWord[25:24]};
						opIty		= JX2_ITY_UW;
					end
					4'b0111: begin		/* F2n7_Dejj */
						opNmid		= JX2_UCMD_JCMP;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27],
							1'b1, istrWord[25:24]};
						opIty		= JX2_ITY_UW;
					end
`endif

`ifdef def_true
					4'b1000: begin
						opNmid		= JX2_UCMD_MOV_RM;
						opFmid		= JX2_FMID_IMM8REG;
						opIty		= JX2_ITY_XL;
						opBty		= JX2_BTY_SL;
//						if(opExQ)
//							opNmid		= JX2_UCMD_LEA_MR;
					end
					4'b1001: begin
						opNmid		= JX2_UCMD_MOV_RM;
						opFmid		= JX2_FMID_IMM8REG;
						opIty		= JX2_ITY_XL;
						opBty		= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
					end
					4'b1010: begin
						opNmid		= JX2_UCMD_MOV_MR;
						opFmid		= JX2_FMID_IMM8REG;
						opIty		= JX2_ITY_XL;
						opBty		= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
					end
					4'b1011: begin
						opNmid		= JX2_UCMD_MOV_MR;
						opFmid		= JX2_FMID_IMM8REG;
						opIty		= JX2_ITY_XL;
						opBty		= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
					end
`endif

					default: begin
					end
				endcase
			end
`endif

			default: begin
			end
		endcase
	end
	else
		if(tBlockIsF8)		/* F8zz_zzzz */
	begin
		casez(istrWord[7:5])
			3'b000: begin
//				if(opIsJumbo)
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_LDI_JLDIX;
				end
				else
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
//					opIty		= JX2_ITY_UW;
					opIty		= JX2_ITY_UB;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
				end
			end
			3'b001: begin
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_LDI_JLDIX;
				end
				else
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
	//				opIty		= JX2_ITY_NW;
					opIty		= JX2_ITY_NB;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
				end
			end
			3'b010: begin
				if(opIsJumboAu && (opExWM || opExWI))
				begin
					if(opExWM)
					begin
						opNmid		= JX2_UCMD_ALUW3;
						opUCmdIx	= JX2_UCIX_ALUW_MOVTA16;
						opFmid		= JX2_FMID_IMM8REG;
						opIty		= JX2_ITY_SB;
						opUCty		= JX2_IUC_WX;
					end
					else
					begin
						opNmid		= JX2_UCMD_ALUW3;
						opUCmdIx	= JX2_UCIX_ALUW_MOVLD;
						opFmid		= JX2_FMID_IMM8REG;
						opIty		= JX2_ITY_SB;
					end
				end
				else
				begin
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_ALU_ADD;
				end
			end
			3'b011: begin
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_ALU3;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_ALU_ADD;
				end
				else
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UB;
//					opUCmdIx	= JX2_UCIX_LDI_LDISH16;
					opUCmdIx	= opIsJumboAu ?
						JX2_UCIX_LDI_LDISH32 :
						JX2_UCIX_LDI_LDISH16;
				end
			end

`ifdef def_true
			3'b100: begin
				if(opIsJumboAu)
				begin
					opNmid		= JX2_UCMD_FLDCX;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_FPCX_SG;

`ifdef jx2_enable_convfp16
					if(opExWM || opExWI)
					begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV_FP16UPCK32L;
					end
					
					if(opExWM)
					begin
						opUCmdIx	= opExWI ?
							JX2_UCIX_CONV_RGB32UPCK64FU :
							JX2_UCIX_CONV_RGB32UPCK64FS;
					end
`endif
				end
				else
				begin
					opNmid		= JX2_UCMD_FLDCX;
	//				opFmid		= JX2_FMID_REGREG;
	//				opIty		= JX2_ITY_UB;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_FPCX_HG;
				end

//				if(opRegO_Df2_IsSP)
//				if(istrWord[4:0]==5'h0F)
				if((istrWord[4:0]==5'h0F) && !opIsJumboAu)
				begin
					/* "FLDCH Imm16, SP" is a BREAK */
					opNmid		= JX2_UCMD_OP_IXT;
					opUCmdIx	= JX2_UCIX_IXT_BREAK;
				end
			end
`endif

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
	else
		if(tBlockIsFA)		/* FAjj_jjjj */
	begin
		opNmid		= JX2_UCMD_MOV_IR;
		opFmid		= JX2_FMID_IMM12Z;
		opUCmdIx	= JX2_UCIX_LDI_LDIX;
		opIty		= JX2_ITY_SB;

		if(opIsJumbo && opIsJumboAu)
		begin
			opNmid		= JX2_UCMD_JMP;
//			opFmid		= JX2_FMID_IMM12Z;
			opIty		= JX2_ITY_UW;
		end
	end
	else
		if(tBlockIsFB)		/* FBjj_jjjj */
	begin
		opNmid		= JX2_UCMD_MOV_IR;
		opFmid		= JX2_FMID_IMM12N;
		opUCmdIx	= JX2_UCIX_LDI_LDIX;
		opIty		= JX2_ITY_SB;

		if(opIsJumbo && opIsJumboAu)
		begin
			opNmid		= JX2_UCMD_JSR;
			opFmid		= JX2_FMID_IMM12Z;
			opIty		= JX2_ITY_UW;
		end
	end
	else
		if(tBlockIsFE || tBlockIsFF)
	begin
		opNmid		= JX2_UCMD_NOP;
		opFmid		= JX2_FMID_IMM12Z;
	end
	
`ifdef jx2_enable_ops24
	if(isOp24)
	begin
		/* If this is a 24-bit op, nuke everything else... */

		if(!tBlockIsF0 || (istrWord[31:29]!=3'b000))
		begin
			opNmid		= JX2_UCMD_INVOP;
			opUCmdIx	= 0;
			opFmid		= JX2_FMID_Z;
			opIty		= JX2_ITY_SB;
		end
	end
`endif

//	if(opIsNotFx)
	if(opIsNotFx && !opIsXGpr)
	begin
		opFmid	= JX2_FMID_Z;
		opIty	= JX2_ITY_SB;
	end

	opUCmd = { opCcty, opNmid };

`ifndef def_true
	if((opNmid==JX2_UCMD_INVOP) && !opIsNotFx && !isAltOp)
	begin
		$display("Jx2DecOpFx: InvOp, Istr %X-%X",
			istrWord[15:0], istrWord[31:16]);
	end
`endif

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

				JX2_ITY_UB: begin
					opRegM	= JX2_GR_DLR;
					opRegO	= JX2_GR_DLR;
					opRegN	= JX2_GR_DLR;
					opRegP	= JX2_GR_DLR;
				end

				default: begin
					$display("Jx2DecOpFx: Bad Reg, Ity=%X", opIty);
					$display("Jx2DecOpFx: Istr %X-%X-%X",
						istrWord[15:0], istrWord[31:16], istrWord[47:32]);
				end
			endcase
		end

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

				JX2_ITY_XW: begin
					opRegM	= opRegO_Dfl;
					opRegO	= JX2_GR_ZZR;
//					opRegN	= JX2_GR_DLR;
//					opRegP	= JX2_GR_DLR;
					opRegN	= JX2_GR_LR;
					opRegP	= JX2_GR_LR;
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
			SW: Rm, Q?Imm5u:Ro, Rn
			SL: Rm, Q?Imm5n:Ro, Rn
			/ SQ: Q?Imm5n:Ro, Rm, Rn

			UB: Rm, Rn, Rn
			UW: Rm, Rn, Rn
			UL: Rm, Cn, Cn
			UQ: Cm, Rn, Rn

			NB: Rn, Rm, Rn
			NW: Sm, Sn, Sn
			NL: Rm, Sn, Sn
			NQ: Sm, Rn, Rn

			SW: Rm, Eq?Imm5:Ro, Rn
		*/

		JX2_FMID_REGREG: begin
`ifndef def_true
			if(opUCty == JX2_IUC_WX)
			begin
				opRegM_Dfl	= opRegM_OrgDfl;
				opRegN_Dfl	= opRegN_OrgDfl;
				opRegM_Dfl[6] = 0;
				opRegN_Dfl[6] = 0;
			end
`endif

			opRegN	= opRegN_Dfl;
//			opRegP	= opRegP_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;

			if(opIsImm4R)
				opRegP	= opRegP_Dfl;

			opImm	= opImm_imm5u;

			opUIxt	= { opUCty, opUCmdIx };

			case(opIty)
				JX2_ITY_SB: begin
					opImm	= opImm_imm8au;
				end

				JX2_ITY_SW: begin
					opImm	= opImm_imm5u;
					if(opExQ)
					begin
						opRegO	= JX2_GR_IMM;
						opRegP	= opRegN_Dfl;
						if(opIsImm4R)
							opRegP	= opRegO_Dfl;
					end
				end

				JX2_ITY_UB: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
					opImm	= opImm_imm8au;
				end

				JX2_ITY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
					opImm	= opImm_imm8au;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
					opImm	= opImm_imm8au;
				end

				JX2_ITY_SL: begin
					opImm	= opImm_imm5n;

					if(opExQ)
					begin
						opRegO	= JX2_GR_IMM;
						opRegP	= opRegN_Dfl;
						if(opIsImm4R)
							opRegP	= opRegO_Dfl;
					end
				end

`ifndef def_true
				JX2_ITY_SQ: begin
					opImm	= opImm_imm5u;
					if(opExQ)
					begin
						opRegM	= JX2_GR_IMM;
						opRegO	= opRegM_Dfl;
						opRegP	= opRegN_Dfl;
						if(opIsImm4R)
							opRegP	= opRegO_Dfl;
					end
				end
`endif

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
					opRegO	= opRegN_Dfl;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					if(usrRejectCmR)
						usrReject = 1;
				end

				JX2_ITY_NW: begin
					opRegM	= opRegM_Sr;
					opRegO	= opRegN_Sr;
					opRegN	= opRegN_Sr;
					opRegP	= opRegN_Sr;
					usrReject = 1;
				end
				
				JX2_ITY_NL: begin
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Sr;
					opRegN	= opRegN_Sr;
					opRegP	= opRegN_Sr;
					usrReject = 1;
				end
				JX2_ITY_NQ: begin
					opRegM	= opRegM_Sr;
					opRegO	= opRegN_Dfl;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					usrReject = 1;
				end

				JX2_ITY_XB: begin
					opImm	= opImm_imm8au;
//					opRegP	= opRegN_Dfl;
					opRegP	= opRegP_Dfl;
				end
				JX2_ITY_XW: begin
					opImm	= opImm_imm8au;
					opRegP	= JX2_GR_IMM;
				end

				JX2_ITY_XL: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_IMM;
					opImm	= opImmRm_imm6u;
				end
				
				default: begin
					$display("Jx2DecOpFx: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end

		/*
		REGIMMREG, Fz:
			SB: Rm, 0, Rn
			SW:	Rm, Imm9s, Rn

			UB:	Rm, Imm9f/Imm16f, Rn
			UW:	Rm, Imm9u, Rn
			UL: Imm9u, Rm, Rn

			NW:	Rm, Imm9n, Rn
		 */
		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_IMM;
			opUIxt	= { opUCty, opUCmdIx };
			opImm	= opImm_imm9u;

			case(opIty)
				JX2_ITY_SB: begin
					opImm	= 0;
				end
				JX2_ITY_SW: begin
					opImm	= opImm_imm9s;
				end

`ifdef jx2_use_fpu_fpimm
				JX2_ITY_UB: begin
					opImm		= opImm_imm9u;
					
					opRegO	= JX2_GR_FPIMM10;
					if(opIsJumboAu)
						opRegO	= JX2_GR_FPIMM16;

//					opRegO		= JX2_GR_JIMM;
//					opUFl[2]	= 1;
//					opUFl[0]	= 0;
				end
`endif

				JX2_ITY_UW: begin
					opImm	= opImm_imm9u;
				end
				JX2_ITY_NW: begin
					opImm	= opImm_imm9n;
				end

				JX2_ITY_UL: begin
					opImm	= opImm_imm9u;

					opRegM	= JX2_GR_IMM;
					opRegO	= opRegM_Dfl;
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
			opRegP	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
			opUIxt	= { opUCty, opUCmdIx };

			opImm = {
				UV9_00,
				istrWord[23:16],
				istrWord[47:32] };
		end
		JX2_FMID_IMM8N: begin
//			opImm	= {UV25_FF, istrWord[7:0]};
			opRegN	= JX2_GR_DLR;
			opRegP	= JX2_GR_DLR;
			opRegM	= JX2_GR_IMM;
			opUIxt	= { opUCty, opUCmdIx };

			opImm = {
				UV9_FF,
				istrWord[23:16],
				istrWord[47:32] };
		end
`endif

// `ifdef def_true
`ifndef def_true
		/*
			SB: (Rm), Rn
		 */
		JX2_FMID_LDREGREG: begin
//			if(opBty == JX2_BTY_UQ)
			if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC))
				opUCty		= JX2_IUC_WX;

//			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};

`ifdef jx2_use_mem_ldop
			opULdOp	= opIsImmLdOp;
`endif

			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= JX2_GR_ZZR;
			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
//				opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty[2], 2'b00};
				opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty[2], 2'b00};
			end

		end
`endif

`ifdef def_true
		/*
			SW: (Rm, Disp9u/17s), Rn
			SL: (Rm, Disp9u/17s), Rn  (Keeps Scale)

			UB: (Rm, Disp5u), Rn
			UW: -
			UL: (Rm, Disp9u/17s), Cn

			NB: (Rm, Disp5u), Rn  (Keeps Scale)
		 */
//		JX2_FMID_LDREGDISPREG: begin
		JX2_FMID_LDREGDISPREG, JX2_FMID_REGSTREGDISP: begin
//			if(opBty == JX2_BTY_UQ)
			if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC))
			begin
				opUCty			= JX2_IUC_WX;
//				opRegN_Dfl		= opRegN_OrgDfl;
//				opRegN_Dfl[6]	= 0;
			end

			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
			opImm	= opImm_disp9s;

`ifdef jx2_use_mem_ldop
			opULdOp	= opIsImmLdOp;
`endif
			
//			if(opIty==JX2_ITY_UB)
			if((opIty==JX2_ITY_UB) || (opIty==JX2_ITY_NB))
			begin
				opImm	= opImm_disp5u;
				if(opIsImm4R)
					opUIxt[1:0] = 0;
			end

			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;

`ifdef jx2_enable_movc
			if(opIty==JX2_ITY_UL)
			begin
				opRegN	= opRegN_Cr;
				opRegP	= opRegN_Cr;
			end
`endif

`ifdef def_true
// `ifndef def_true
			if(opNmid == JX2_UCMD_MOV_RM)
				opRegN	= JX2_GR_ZZR;
			if(opNmid == JX2_UCMD_MOV_MR)
				opRegP	= JX2_GR_ZZR;
`endif

			if(tRegRmIsRz)
			begin
				opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
//				if(opIty!=JX2_ITY_SL)
				if((opIty!=JX2_ITY_SL) && (opIty!=JX2_ITY_NB))
//				if(1'b1)
				begin
//					opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty[2], 2'b00};
					opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty[2], 2'b00};
				end
			end
		end
`endif

		JX2_FMID_LDDRREGREG: begin
//			if(opBty == JX2_BTY_UQ)
//			if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC))
			if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC) &&
				(opNmid != JX2_UCMD_FMOV_MR))
			begin
				opUCty			= JX2_IUC_WX;
//				opRegN_Dfl		= opRegN_OrgDfl;
//				opRegN_Dfl[6]	= 0;
			end

//			if(	(opIty==JX2_ITY_UB) ||
//				(opIty==JX2_ITY_UL) ||
//				(opIty==JX2_ITY_UQ))
			if(1'b1)
			begin
				opRegN	= opRegN_Dfl;
				opRegP	= opRegN_Dfl;
				opImm	= opImm_disp11as;

`ifdef jx2_use_mem_ldop
				opULdOp	= opIsImmLdOp;
`endif


`ifdef jx2_enable_movc
				if(opIty==JX2_ITY_UL)
				begin
					opRegN	= opRegN_Cr;
					opRegP	= opRegN_Cr;
				end
`endif

`ifdef def_true
// `ifndef def_true
				if(opNmid == JX2_UCMD_MOV_RM)
					opRegN	= JX2_GR_ZZR;
				if(opNmid == JX2_UCMD_MOV_MR)
					opRegP	= JX2_GR_ZZR;
`endif

				if(tRegRmIsRz)
				begin
					if(opIty!=JX2_ITY_SL)
//					if(1'b1)
					begin
//						opUIxt	= {opUCty, opBty[1:0],
//							1'b1, opBty[2], 2'b00};
//						opUIxt	= {opUCty, opBty[1:0],
//							1'b0, opBty[2], 2'b00};
						opUIxt	= {opUCty, opBty[1:0],
							opIsJumboAu, opBty[2], 2'b00};
					end
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
//							opUIxt	= {opUCty, opBty[1:0], 1'b1,
//								opBty[2], 2'b00};
						end else begin
							opRegM	= JX2_GR_PC;
							opRegO	= opRegO_Dfl;
						end
					end
				end
				else
				begin
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegO_Dfl;
//					opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
					opUIxt	= {opUCty, opBty[1:0], opIsJumboAu, opBty};
				end

				if(opImm_dispasc[2])
					opUIxt[1:0] = opImm_dispasc[1:0];

			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegP	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= JX2_GR_DLR;
//				opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
				opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
			end
		end

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

			XL: Fznz_zzjj		(GBR, Disp10u), Rn
			XQ: -

		 */
	
		JX2_FMID_IMM8REG: begin
//			$display("IMM8REG: IsJumbo=%d", opIsJumbo);
		
			opUIxt	= { opUCty, opUCmdIx };
			opUFl		= 0;
			opIsImm9	= 0;
			opIsImmSplit	= 0;
			
			if(opUCty == JX2_IUC_WX)
				opIsImmSplit = (opExWM && opExWI);

//			opRegM	= JX2_GR_IMM;
			opRegM	= opRegImm16;
			opRegO	= opRegO_Df2;
			opRegN	= opRegO_Df2;
			opRegP	= opRegO_Df2;
			
			case(opIty)
				JX2_ITY_SB: begin
//					opImm = {
//						istrWord[31] ? UV17_FF : UV17_00,
//						istrWord[31:16] };
					opImm	= opImm_imm16s;
					opRegM	= opRegO_Df2;
//					opRegO	= JX2_GR_IMM;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

				JX2_ITY_SW: begin
					opImm	= opImm_imm16s;
//					opImm = {
//						istrWord[31] ? UV17_FF : UV17_00,
//						istrWord[31:16] };
				end

//`ifndef def_true
`ifdef def_true
				JX2_ITY_SL: begin
//					opImm = {
//						istrWord[31] ? UV17_FF : UV17_00,
//						istrWord[31:16] };
					opImm	= opImm_imm16s;
					opRegM	= JX2_GR_DLR;
//					opRegO	= JX2_GR_IMM;
					opRegO	= opRegImm16;
					opRegN	= JX2_GR_DLR;
					opRegP	= JX2_GR_DLR;
				end
`endif

				JX2_ITY_SQ: begin
					opImm		= opImm_imm10s;
					opRegM		= opRegN_Dfl;
//					opRegO		= JX2_GR_IMM;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

				JX2_ITY_UB: begin
//					opImm	= { UV17_00, istrWord[31:16] };
					opImm	= opImm_imm16u;
					opRegM	= opRegO_Df2;
//					opRegO	= JX2_GR_IMM;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

				JX2_ITY_UW: begin
//					opImm = { UV17_00, istrWord[31:16] };
					opImm	= opImm_imm16u;
				end

// `ifndef def_true
`ifdef def_true
				JX2_ITY_UL: begin
					opImm		= opImm_imm10u;
//					opRegM		= JX2_GR_IMM;
					opRegM		= opRegImm10;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end
`endif

				JX2_ITY_UQ: begin
					opImm		= opImm_imm10u;
					opRegM		= opRegN_Dfl;
//					opRegO		= JX2_GR_IMM;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

				JX2_ITY_NB: begin
//					opImm = { UV17_FF, istrWord[31:16] };
					opImm	= opImm_imm16n;
					opRegM	= opRegO_Df2;
//					opRegO	= JX2_GR_IMM;
					opRegO	= opRegImm16;
					opRegN	= opRegO_Df2;
					opRegP	= opRegO_Df2;
				end

				JX2_ITY_NW: begin
//					opImm = { UV17_FF, istrWord[31:16] };
					opImm	= opImm_imm16n;
				end

				JX2_ITY_NQ: begin
					opImm		= opImm_imm10n;
					opRegM		= opRegN_Dfl;
//					opRegO		= JX2_GR_IMM;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

`ifdef jx2_use_fpu_fpimm
				JX2_ITY_XB: begin
					opImm		= opImm_imm10u;
					opRegM		= JX2_GR_FPIMM10;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

				JX2_ITY_XW: begin
					opImm		= opImm_imm16u;
					opRegM		= JX2_GR_FPIMM16;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end
`endif

				JX2_ITY_XL: begin
					if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC))
						opUCty		= JX2_IUC_WX;

					opImm		= opImm_imm10u;
					opRegM		= JX2_GR_GBR;
					opRegO		= JX2_GR_IMM;
//					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;

//					opRegN		= opRegN_OrgDfl;
//					opRegP		= opRegN_OrgDfl;
//					opRegN[6]	= 0;
//					opRegP[6]	= 0;

					opUIxt		= {opUCty, opBty[1:0], opIsJumboAu, opBty};
					opIsImm9	= 1;
				end

				default: begin
					$display("Jx2DecOpFx: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
			
			opUFl[0]	= opIsImm9;
			opUFl[1]	= opIsImmSplit;
		end

`ifndef def_true
		JX2_FMID_IMM4ZREG: begin
			opImm	= opImm_imm10u;
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= JX2_GR_IMM;
		end

		JX2_FMID_IMM4NREG: begin
			opImm	= opImm_imm10n;
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= JX2_GR_IMM;
		end
`endif
		
`ifdef jx2_enable_ldirot
		JX2_FMID_IMM4ZREG: begin
//			opImm	= opImm_imm10u;
			opImm	= { opImm_imm10u[28:4], 8'h00 };
			opRegM	= JX2_GR_R8IMMH;
			opRegO	= JX2_GR_R8IMML;
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
//			if(opIty == JX2_ITY_SB)
//				opRegO	= JX2_GR_R4IMM1;

			opImm[7:0] = { 2'b00, opImm_imm10u[3:0], 2'b00 };
			if(opIty == JX2_ITY_SB)
				opImm[7:0] = { 3'b000, opImm_imm10u[3:0], 1'b0 };			
			if(opIsJumbo)
				opImm	= opImm_imm10u;
		end

		JX2_FMID_IMM4NREG: begin
//			opImm	= opImm_imm10n;
			opImm	= { opImm_imm10n[28:4], 8'h00 };
			opRegM	= JX2_GR_R8IMMH;
			opRegO	= JX2_GR_R8IMML;
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
//			if(opIty == JX2_ITY_SB)
//				opRegO	= JX2_GR_R4IMM1;
			opImm[7:0] = { 2'b00, opImm_imm10u[3:0], 2'b00 };
			if(opIty == JX2_ITY_SB)
				opImm[7:0] = { 3'b000, opImm_imm10u[3:0], 1'b0 };
			if(opIsJumbo)
				opImm	= opImm_imm10u;
		end
`endif
		
		/*
			SB: (PC, Ro), ZZR
//			UB: Rm, #imm8s, Rn
//			UW: ZZR, #imm8s, Rn

			UB: Rn, Rm, disp8s
			UW: Rn, ZZR, Disp8s
		 */
		JX2_FMID_REGPC: begin
			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= JX2_GR_PC;
					opRegO	= opRegO_Dfl;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;
//					opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
					opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
				end

`ifdef jx2_alu_jcmp
				JX2_ITY_UB: begin
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;
//					opRegM	= opRegM_Dfl;
//					opRegO	= JX2_GR_IMM;
//					opRegO	= opRegN_Dfl;

					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;

//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;

					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;

`ifdef jx2_alu_jcmp_rpimm
					if(opImm_dispasc[2])
					begin
						opRegP	= opRegM_OrgDfl;
						opRegO	= JX2_GR_IMM_RP;

						if(opImm_dispasc[1])
						begin
							opRegM	= JX2_GR_IMM_RP;
							opRegO	= opRegN_Dfl;
						end
					end
`endif

					opImm	= opImm_disp8s;
					opUIxt	= { opUCty, opUCmdIx };
				end

				JX2_ITY_UW: begin
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;
//					opRegM	= JX2_GR_ZZR;
//					opRegO	= JX2_GR_IMM;
//					opRegO	= opRegN_Dfl;

//					opRegM	= JX2_GR_ZZR;
//					opRegO	= opRegN_Dfl;

					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
//					opRegN	= opRegN_Dfl;
//					opRegP	= opRegN_Dfl;

					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;

					opImm	= opImm_disp8s;
					opUIxt	= { opUCty, opUCmdIx };
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

			UB: Fzdd_xxdx		(PC, disp13s), DLR
			UW: Fzdd_xddd		(PC, disp9s), DLR
		 */
		JX2_FMID_PCDISP8: begin
//			opRegN	= JX2_GR_DLR;
			opRegN	= JX2_GR_LR;
//			opRegP	= JX2_GR_DLR;
			opRegP	= JX2_GR_LR;
			opRegM	= JX2_GR_PC;
			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};

			case(opIty)
			JX2_ITY_SB: begin
//				opImm	= {istrWord[7]?UV17_FF:UV17_00,
//					istrWord[ 7: 0],
//					istrWord[23:16]};
				opImm	= opImm_imm16s;
			end
			JX2_ITY_SW: begin
				opImm	= opImm_disp20s;
			end

`ifdef jx2_enable_ops24
			JX2_ITY_UB: begin
				opImm	= opImm_disp13s;
			end
`endif

			JX2_ITY_UW: begin
				opImm	= opImm_disp9s;
			end

			default: begin
				$display("Jx2DecOpFx: PcDisp, Bad Ity=%X", opIty);
			end

			endcase
		end

		JX2_FMID_IMM12Z: begin
			opImm	= {UV9_00, istrWord[7:0], istrWord[31:16]};
//			opRegN	= JX2_GR_DLR;
//			opRegM	= JX2_GR_IMM;
			opRegM	= JX2_GR_DLR;
			opRegO	= JX2_GR_IMM;
			opRegN	= JX2_GR_DLR;
			opRegP	= JX2_GR_DLR;
			opUIxt	= {opUCty, opUCmdIx[5:0]};

			if(opIsJumbo)
			begin
				opImm	= {
					1'b0, istrJBits[7:0],
					istrWord[7:0], istrWord[31:16]};
				opRegO	= JX2_GR_JIMM;
			end

			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_UW: begin
					opRegM	= JX2_GR_ZZR;
					opRegN	= JX2_GR_LR;
					opRegP	= JX2_GR_LR;
				end

				default: begin end
			endcase
		end
		JX2_FMID_IMM12N: begin
			opImm	= {UV9_FF, istrWord[7:0], istrWord[31:16]};
//			opRegN	= JX2_GR_DLR;
//			opRegM	= JX2_GR_IMM;
			opRegM	= JX2_GR_DLR;
			opRegO	= JX2_GR_IMM;
			opRegN	= JX2_GR_DLR;
			opRegP	= JX2_GR_DLR;
			opUIxt	= {opUCty, opUCmdIx[5:0]};

			if(opIsJumbo)
			begin
				opImm	= {
					1'b1, istrJBits[7:0],
					istrWord[7:0], istrWord[31:16]};
				opRegO	= JX2_GR_JIMM;
			end
		end
		
		JX2_FMID_INV: begin
//			if(istrWord[15:12]==4'b1111)
//			if(!opIsNotFx)
			if(!opIsNotFx || opIsXGpr)
			begin
				opUCmd = { opCcty, JX2_UCMD_INVOP };
				if(!tMsgLatch && !isAltOp)
				begin
					$display("Jx2DecOpFx: Invalid FMID (32)");
					$display("Jx2DecOpFx: Istr %X-%X",
						istrWord[15:0], istrWord[31:16]);
				end
				tNextMsgLatch=1;
			end
		end
		
		default: begin
			opUCmd = { opCcty, JX2_UCMD_INVOP };
			if(!tMsgLatch && !isAltOp)
			begin
				$display("Jx2DecOpFx: Unhandled FMID (32) %X", opFmid);
				$display("Jx2DecOpFx: Istr %X-%X",
					istrWord[15:0], istrWord[31:16]);
			end
			tNextMsgLatch=1;
		end
	endcase

	usrSuAllowEn = 0;
	case(usrSuAllow)
		2'b00: usrSuAllowEn = 0;					//Supervisor Only
		2'b01: usrSuAllowEn = srSuperuser;			//Superuser
		2'b10: usrSuAllowEn = srMod[1];				//Secure Execute
		2'b11: usrSuAllowEn = srMod[1] && srMod[2];	//Superuser+Secure Exe
	endcase

//	if(usrReject && srUser && !(usrSuAllow && srSuperuser))
	if(usrReject && srUser && !usrSuAllowEn)
	begin
		$display("DecOpFz: Usermode Reject %X-%X",
			istrWord[15:0], istrWord[31:16]);
		opNmid	= JX2_UCMD_INVOP;
		opFmid	= JX2_FMID_INV;
	end

`ifndef def_true
	if(opIsXGpr && (!isAltOp || opIsJumbo || opIsJumboAu))
	begin
		if(opIsJumboAu)
		begin
			$display("XGPR+Op64: %X %X-%X",
				istrJBits[23:0],
				istrWord[15:0], istrWord[31:16]);
		end
		else if(opIsJumbo)
		begin
			$display("XGPR+Jumbo: %X %X-%X",
				istrJBits[23:0],
				istrWord[15:0], istrWord[31:16]);
		end
		else
		begin
			$display("XGPR: %X-%X", istrWord[15:0], istrWord[31:16]);
		end

		if(tOpIsXGprX0)
		begin
			$display("  X0 Rs=%x Rt=%x Rn=%x Imm5=%x",
				opRegM_Dfl, opRegO_Dfl, opRegN_Dfl, opImm_imm5u);
		end
		
		if(tOpIsXGprX1)
		begin
			$display("  X1 Rm=%x Rn=%x Imm9=%x",
				opRegM_Dfl, opRegN_Dfl, opImm_imm9u);
		end
		
		if(tOpIsXGprX2)
		begin
			$display("  X2 Rm=%x Rn=%x Imm9=%x",
				opRegM_Dfl, opRegN_Dfl, opImm_imm9u);
		end

		$display(" %X-%X  Rm=%X Ro=%X Rn=%X Rp=%X Imm=%X",
			opUCmd,		opUIxt,
			opRegM,		opRegO,
			opRegN,		opRegP,
			opImm);

	end
	else
//		if(opIsJumboAu && (!opIsNotFx || opIsXGpr) && !isAltOpC)
		if(opIsJumboAu && !isAltOpC)
	begin
		$display("Op64: FF%X-%X %X-%X",
			istrJBits[23:16],	istrJBits[15:0],
			istrWord[15:0],		istrWord[31:16]);
		$display("  Rs=%x Rt=%x Rn=%x Imm5=%x Imm9=%x",
			opRegM_Dfl, opRegO_Dfl, opRegN_Dfl, opImm_imm5u, opImm_imm9u);

		$display(" %X-%X  Rm=%X Ro=%X Rn=%X Rp=%X Imm=%X",
			opUCmd,		opUIxt,
			opRegM,		opRegO,
			opRegN,		opRegP,
			opImm);
	end
`endif

`ifndef def_true
	if((opUCty == JX2_IUC_WX) || (opUCty == JX2_IUC_WXA))
	begin
		if(opRegP[6] || opRegN[6])
		begin
			$display("XGPR SP: J=%X-%X Op=%X-%X",
				istrJBits[23:16],	istrJBits[15:0],
				istrWord[15:0],		istrWord[31:16]);
			$display("  Rs=%x Rt=%x Rn=%x Imm5=%x Imm9=%x",
				opRegM_Dfl, opRegO_Dfl, opRegN_Dfl,
				opImm_imm5u, opImm_imm9u);

			$display(" %X-%X  Rm=%X Ro=%X Rn=%X Rp=%X Imm=%X",
				opUCmd,		opUIxt,
				opRegM,		opRegO,
				opRegN,		opRegP,
				opImm);
		end
	end
`endif

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;
end

endmodule
